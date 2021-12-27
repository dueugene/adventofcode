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

void dfs(const unordered_map<string, unordered_set<string>>& connectivity, unordered_set<string>& visited, string curr, int& count);
void dfs2(const unordered_map<string, unordered_set<string>>& connectivity, unordered_map<string, int>& visit_count, string curr, int& count);
bool is_big_cave(string s);

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
    unordered_map<string, unordered_set<string>> connectivity;
    while (getline(input, line)) {
        istringstream ss(line);
        string a, b;
        getline(ss, a, '-');
        getline(ss, b);
        connectivity[a].insert(b);
        connectivity[b].insert(a);
    }
    input.close();

    for (auto item : connectivity) {
        cout << item.first << ": ";
        for (auto a : item.second) {
            cout << a << " ";
        }
        cout << endl;
    }

    // part 1
    // we could "prune" the dead end nodes,
    // but I don't think this will improve time, since they
    // are "leaf" nodes already
    
    // I beleive a dfs with backtracking will still work, 
    // even though we can re-visit large caves
    // I assume no two large caves will be connected to each 
    // other, since this will lead to infiite paths
    // our dfs will also not end in a cyclic path because of this
    int count = 0;
    unordered_set<string> visited;
    dfs(connectivity, visited, "start", count);
    cout << "part1: " << count << endl;

    // part 2
    count = 0;
    unordered_map<string, int> visit_count;
    for (auto item : connectivity) {
        visit_count[item.first] = 0;
    }
    // hack to not visit start more than once
    visit_count["start"] = 3;
    dfs2(connectivity, visit_count, "start", count);
    cout << "part2: " << count << endl;
    return 0;
}

void dfs(const unordered_map<string, unordered_set<string>>& connectivity, unordered_set<string>& visited, string curr, int& count) {
    if (curr == "end") {
        count++;
        return;
    }
    if (is_big_cave(curr) == false) {
        visited.insert(curr);
    }
    const auto& nexts = connectivity.at(curr);
    for (string next : nexts) {
        if (visited.find(next) == visited.end()) {
            dfs(connectivity, visited, next, count);
        }
    }
    visited.erase(curr);
}

void dfs2(const unordered_map<string, unordered_set<string>>& connectivity, unordered_map<string, int>& visit_count, string curr, int& count) {
    if (curr == "end") {
        count++;
        return;
    }
    if (is_big_cave(curr) == false) {
        visit_count[curr]++;
    }
    bool can_double_visit = true;
    for (const auto& item : visit_count) {
        if (item.second == 2) {
            can_double_visit = false;
        }
    }
    const auto& nexts = connectivity.at(curr);
    for (string next : nexts) {
        if (visit_count[next] == 0 || (visit_count[next] == 1 && can_double_visit)) {
            dfs2(connectivity, visit_count, next, count);
        }
    }
    if (is_big_cave(curr) == false) {
        visit_count[curr]--;
    }
}

bool is_big_cave(string s) {
    return s[0] >= 'A' && s[0] <= 'Z';
}