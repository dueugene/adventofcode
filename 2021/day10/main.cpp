#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>

using namespace std;

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "input.txt";
    }

    // read the input file
    ifstream input;
    input.open(filename);
    string line;
    vector<string> lines;
    while (getline(input, line)) {
        lines.push_back(line);
    }
    input.close();


    // part 1
    vector<string> incomplete;
    unordered_map<char, char> mapping;
    mapping[')'] = '(';
    mapping[']'] = '[';
    mapping['}'] = '{';
    mapping['>'] = '<';
    unordered_map<char, int> scoring;
    scoring[')'] = 3;
    scoring[']'] = 57;
    scoring['}'] = 1197;
    scoring['>'] = 25137;
    int score = 0;
    for (string s : lines) {
        stack<char> st;
        bool invalid = false;
        for (char c : s) {
            if (mapping.find(c) != mapping.end()) {
                if (!st.empty() && st.top() == mapping[c]) {
                    st.pop();
                } else {
                    score += scoring[c];
                    invalid = true;
                    break;
                }
            } else {
                st.push(c);
            }
        }
        if (invalid == false) {
            incomplete.push_back(s);
        }
    }
    cout << "part1: " << score << endl;

    // part 2
    vector<long long> scores;
    unordered_map<char, char> rev_mapping;
    rev_mapping['('] = ')';
    rev_mapping['['] = ']';
    rev_mapping['{'] = '}';
    rev_mapping['<'] = '>';
    scoring[')'] = 1;
    scoring[']'] = 2;
    scoring['}'] = 3;
    scoring['>'] = 4;
    for (string s : incomplete) {
        long long score = 0;
        stack<char> st;
        for (char c : s) {
            if (mapping.find(c) != mapping.end()) {
                if (!st.empty() && st.top() == mapping[c]) {
                    st.pop();
                } else {
                    // should never be here
                }
            } else {
                st.push(c);
            }
        }
        while (!st.empty()) {
            char c = st.top();
            c = rev_mapping[c];
            score *= 5;
            score += scoring[c];
            st.pop();
        }
        scores.push_back(score);
    }
    sort(scores.begin(), scores.end());
    cout << "part2: " << scores[scores.size() / 2] << endl;

    return 0;
}