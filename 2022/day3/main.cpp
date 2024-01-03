#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <bitset>

using namespace std;

vector<char> find_repeat(const vector<string>& lines);
int get_priority(vector<char>& input);
vector<char> find_common(const vector<string>& lines);

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
    int curr = 0;
    string line;
    input.open(filename);
    vector<string> lines;
    while (getline(input, line)) {
        lines.push_back(line);
    }
    input.close();
    
    // part 1
    vector<char> repeated = find_repeat(lines);
    int score = get_priority(repeated);
    cout << "part1: " << score << endl;

    // part 2
    repeated = find_common(lines);
    for (char c : repeated) {
        cout << c << endl;
    }
    score = get_priority(repeated);
    cout << "part2: " << score << endl;
    return 0;
}

vector<char> find_repeat(const vector<string>& lines) {
    vector<char> result;
    for (const string& s : lines) {
        int n = s.size();
        set<char> unique_chars;
        for (int i = 0; i < n /2; i++) {
            unique_chars.insert(s.at(i));
        }
        for (int i = n / 2; i < n; i++) {
            if (unique_chars.find(s.at(i)) != unique_chars.end()) {
                result.push_back(s.at(i));
                break;
            }
        }
    }
    return result;
}

int get_priority(vector<char>& input) {
    int score = 0;
    for (const char& c : input) {
        int a = c >= 'a' && c <= 'z' ? c - 'a' + 1 : c - 'A' + 27;
        score += a;
    }
    return score;
}

unsigned long long get_chars(const string& s) {
    unsigned long long result = 0;
    for (const char& c : s) {
        unsigned offset = c >= 'a' && c <= 'z' ? c - 'a' + 1 : c - 'A' + 27;
        result |= 1ULL << offset;
    }
    return result;
}

char get_blah(unsigned long long blah) {
    int a = 0;
    cout << (blah & 1ULL) << endl;
    while ((blah & 1ULL) == 0) {
        a++;
        blah >>= 1ULL;
    }
    cout << a << endl;
    return a <= 26 ? 'a' + a - 1 : 'A' + a - 27;
}
vector<char> find_common(const vector<string>& lines) {
    vector<char> result;
    int n = lines.size();
    for (int i = 0; i < n / 3; i++) {
        unsigned long long blah = ULLONG_MAX;
        for (int j = 0; j < 3; j++) {
            blah &= get_chars(lines.at(i*3+j));
        }
        cout << bitset<64>(blah) << endl;
        result.push_back(get_blah(blah));
    }
    return result;
}