#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>

using namespace std;

// copy from https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

string polymerize(string polymer, const unordered_map<string, string>& rules);

template<class T>
T find_diff(unordered_map<char, T> char_count);

unordered_map<char, unsigned long long> polymerize(string polymer, const unordered_map<string, string>& rules,
    int n, unordered_map<pair<string, int>, unordered_map<char, unsigned long long>, pair_hash>& memo);

template<class T>
void combine_counts(unordered_map<char, T>& a, unordered_map<char, T>& b);

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
    string polymer;
    while (getline(input, line)) {
        if (line.empty()) {break;}
        istringstream ss(line);
        polymer = line;
    }
    string a, b, c;
    unordered_map<string, string> rules;
    while (input >> a >> b >> c) {
        rules[a] = c;
    }

    cout << polymer << endl;
    for (auto p : rules) {
        cout << p.first << "->" << p.second << endl;
    }

    input.close();

    // part 1
    string polymer_start = polymer;
    for (int i = 0; i < 10; i++) {
        polymer = polymerize(polymer, rules);
    }
    unordered_map<char, int> char_count;
    for (char c : polymer) {
        char_count[c]++;
    }
    cout << "part1: " << find_diff(char_count) << endl;

    // part 2
    unordered_map<pair<string, int>, unordered_map<char, unsigned long long>, pair_hash> memo;
    unordered_map<char, unsigned long long> result = polymerize(polymer_start, rules, 40, memo);
    cout << "done" << endl;
    cout << "part1: " << find_diff(result) << endl;
    
    return 0;
}

string polymerize(string polymer, const unordered_map<string, string>& rules) {
    string result;
    for (int i = 0; i < polymer.size() - 1; i++) {
        string key = polymer.substr(i, 2);
        result.push_back(polymer[i]);
        if (rules.find(key) != rules.end()) {
            result.push_back(rules.at(key)[0]);
        }
    }
    result.push_back(*polymer.rbegin());

    return result;
}

template<class T>
T find_diff(unordered_map<char, T> char_count) {
    T least = char_count.begin()->second;
    T most = 0;
    for (auto p : char_count) {
        most = max(most, p.second);
        least = min(least, p.second);
    }
    return most - least;
}

unordered_map<char, unsigned long long> polymerize(string polymer, const unordered_map<string, string>& rules,
    int n, unordered_map<pair<string, int>, unordered_map<char, unsigned long long>, pair_hash>& memo) {
    unordered_map<char, unsigned long long>  result;
    if (polymer.size() > 2) {
        for (int i = 0; i < polymer.size() - 1; i++) {
            string polymer_pair = polymer.substr(i, 2);
            unordered_map<char, unsigned long long>  temp = polymerize(polymer_pair, rules, n, memo);
            combine_counts(result, temp);
            result[polymer_pair[1]]--;
        }
        result[*polymer.rbegin()]++;
        return result;
    }

    if (memo.find({polymer, n}) == memo.end()) {
        if (n == 1) {
            result[polymer[0]]++;
            result[polymer[1]]++;
            result[rules.at(polymer)[0]]++;
            memo[{polymer, n}] = result;
            goto finished;
        }

        string new_thing = rules.at(polymer);
        result = polymerize(polymer.substr(0,1) + new_thing, rules, n - 1, memo);
        unordered_map<char, unsigned long long>  temp = polymerize(new_thing + polymer.substr(1,1), rules, n - 1, memo);
        combine_counts(result, temp);
        result[new_thing[0]]--;
        memo[{polymer, n}] = result;
        goto finished;
    }
finished:
    return memo[{polymer, n}];
}

template<class T>
void combine_counts(unordered_map<char, T>& a, unordered_map<char, T>& b) {
    for (auto p : b) {
        a[p.first] += p.second;
    }
}