#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

int seven_seg_decode(const vector<string>& input, const vector<string>& output);
char find_missing_char(string s);

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
    vector<vector<string>> inputs, outputs;
    while (getline(input, line)) {
        istringstream ss(line);
        string a;
        vector<string> inputt;
        while (ss >> a && a != "|") {
            inputt.push_back(a);
        }
        vector<string> output;
        while (ss >> a) {
            output.push_back(a);
        }
        inputs.push_back(inputt);
        outputs.push_back(output);
    }
    input.close();

    // part 1
    int count = 0;
    for (auto item : outputs) {
        for (string a : item) {
            int l = a.size();
            if (l == 2 || l == 4 || l == 3 || l ==7) {
                count++;
            }
        }
    }
    cout << "part1: " << count << endl;

    // part 2
    int ans = 0;
    for (int i = 0; i < inputs.size(); i++) {
        ans += seven_seg_decode(inputs[i], outputs[i]);
    }
    cout << "part2: " << ans << endl;

    return 0;
}

int seven_seg_decode(const vector<string>& input, const vector<string>& output) {
    unordered_map<string, int> mapping;
    unordered_map<int, string> rev_mapping;
    unordered_set<string> five_char;
    unordered_set<string> six_char;
    for (string s : input) {
        sort(s.begin(), s.end());
        int l = s.size();
        switch (l) {
        case 2:
            mapping[s] = 1;
            rev_mapping[1] = s;
            break;
        case 3:
            mapping[s] = 7;
            break;
        case 4:
            mapping[s] = 4;
            break;
        case 7:
            mapping[s] = 8;
            break;
        case 5:
            five_char.insert(s);
            break;
        case 6:
            six_char.insert(s);
            break;
        default:
            cout << "uh oh" << endl;
        }
    }
    // identify the other nums
    // 0, 6, 9 are six chars
    // 2, 3, 5 are five chars
    // 0 is missing a char that exists in all of the 5 chars
    for (string s : six_char) {
        char missing = find_missing_char(s);
        bool found_in_all = true;
        for (string t : five_char) {
            if (t.find(missing) == t.npos) {
                found_in_all = false;
                break;
            }
        }
        if (found_in_all == true) {
            // cout << "found 0: " << s << endl;
            mapping[s] = 0;
            six_char.erase(s);
            break;
        }
    }
    // 6 has a mssing char, that is shared with 1
    for (string s : six_char) {
        char missing = find_missing_char(s);
        string one = rev_mapping[1];
        if (one.find(missing) != one.npos) {
            // cout << "found 6: " << s << endl;
            mapping[s] = 6;
            six_char.erase(s);
            // the 6 shares a missing char with 5
            for (string t : five_char) {
                if (t.find(missing) == t.npos) {
                    // cout << "found 5: " << t << endl;
                    mapping[t] = 5;
                    five_char.erase(t);
                    break;
                }
            }
            break;
        }
    }
    // remaining six char is 9
    for (string s : six_char) {
        // cout << "found 9: " << s << endl;
        mapping[s] = 9;
        char missing = find_missing_char(s);
        // 9 shares missing char with 3
        for (string t : five_char) {
            if (t.find(missing) == t.npos) {
                // cout << "found 3: " << t << endl;
                mapping[t] = 3;
            } else {
                // cout << "found 2: " << t << endl;
                mapping[t] = 2;
            }
        }

    }
    int ans = 0;
    for (string s : output) {
        sort(s.begin(), s.end());
        ans *= 10;
        ans += mapping[s];
    }
    return ans;
}

char find_missing_char(string s) {
    // assumes is a 6 length string, with only one missing char
    // will return first missing char
    string possible = "abcdefg";
    for (char c : possible) {
        if (s.find(c) == s.npos) {
            return c;
        }
    }
    return 'h';
}