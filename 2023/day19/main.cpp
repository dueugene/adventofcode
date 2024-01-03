#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

struct Part {
    Part(string input) {
        stringstream ss(input);
        string temp;
        getline(ss, temp, '=');
        getline(ss, temp, ',');
        x = stoi(temp);
        getline(ss, temp, '=');
        getline(ss, temp, ',');
        m = stoi(temp);
        getline(ss, temp, '=');
        getline(ss, temp, ',');
        a = stoi(temp);
        getline(ss, temp, '=');
        getline(ss, temp, '}');
        s = stoi(temp);
        cout << x << " " << m << " " << a << " " << s << endl;
    }
    int x;
    int m;
    int a;
    int s;
};

struct Condition {
    Condition(string input) {
        op = input[1];
        if (op == '<' || op == '>') {
          var = input[0];
          int ind = input.find(':');
          val = stoi(input.substr(2, ind - 2));
          return_val = input.substr(ind + 1);
          cout << var << " " << op << " " << val << " " << return_val << endl;
        } else {
          return_val = input;
          cout << return_val << endl;
        }
    }
    string operator()(Part& input) const {
        if (op != '<' && op != '>') {
          return return_val;
        }
        int* var_val;// = var == 'x' ? input.x : (var == 'm' ? input.m : (var == 'a' ? input.a : input.s));
        switch (var) {
        case 'x':
          var_val = &input.x;
          break;
        case 'm':
          var_val = &input.m;
          break;
        case 'a':
          var_val = &input.a;
          break;
        case 's':
          var_val = &input.s;
          break;
        }

        switch (op) {
        case '<':
          return *var_val < val ? return_val : "";
        case '>':
          /* code */
          return *var_val > val ? return_val : "";
        default:
          return return_val;
        }
    }
    char op;
    char var;
    int val;
    string return_val;
};

struct Workflow {
    Workflow() {}
    Workflow(string input) {
        int ind = input.find('{');
        name = input.substr(0, ind);
        input = input.substr(ind + 1);
        while (input.size() > 0) {
            ind = input.find(',');
            if (ind == -1) {
                ind = input.find('}');
                conditions.push_back(Condition(input.substr(0, ind)));
                break;
            }
            conditions.push_back(Condition(input.substr(0, ind)));
            input = input.substr(ind + 1);
        }
    }
    string operator()(Part& input) const {
        for (auto& condition : conditions) {
            auto res = condition(input);
            cout << res << endl;
            if (!res.empty()) {
                return res;
            }
        }
        return "";
    }
    vector<pair<string, unordered_map<char, pair<int, int>>>> apply(unordered_map<char, pair<int, int>> ranges) const {
        vector<pair<string, unordered_map<char, pair<int, int>>>> result;
        for (auto& condition : conditions) {
            char op = condition.op;
            if (op == '<') {
              char c = condition.var;
              int val = condition.val;
              if (ranges[c].first >= val) {
                continue;
              } else if (ranges[c].second < val) {
                result.push_back({condition.return_val, ranges});
                return result;
              }
              int temp = ranges[c].second;
              ranges[c].second = val - 1;
              result.push_back({condition.return_val, ranges});
              ranges[c].first = val;
              ranges[c].second = temp;
            } else if (op == '>') {
              char c = condition.var;
              int val = condition.val;
              if (ranges[c].second <= val) {
                continue;
              } else if (ranges[c].first > val) {
                result.push_back({condition.return_val, ranges});
                return result;
              }
              int temp = ranges[c].first;
              ranges[c].first = val + 1;
              result.push_back({condition.return_val, ranges});
              ranges[c].first = temp;
              ranges[c].second = val;

            } else {
              result.push_back({condition.return_val, ranges});
            }
        }
        return result;
    }
    string name;
    vector<Condition> conditions;
};

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
    vector<string> input_data = input_reader::read_as_strings(filename);
    vector<Part> parts;
    vector<Workflow> workflows;
    bool start = false;
    for (auto& line : input_data) {
        if (line.size() <= 1) {
            start = true;
            continue;
        }
        if (start) {
            parts.push_back(Part(line));
        } else {
            workflows.push_back(Workflow(line));
        }
    }
    unordered_map<string, Workflow> workflow_map;
    for (auto w : workflows) {
        workflow_map[w.name] = w;
    }
    ulong part1 = 0;
    for (auto p : parts) {
        string curr = "in";
        while (curr != "R" && curr != "A") {
            curr = workflow_map[curr](p);
        }
        cout  << curr << endl;
        if (curr == "A") {
            part1 += p.x + p.m + p.a + p.s;
        }
    }

    cout << endl << "part 2" << endl << endl;
    unsigned long long part2 = 0;
    unordered_map<char, pair<int, int>> ranges;
    ranges['x'] = {1, 4000};
    ranges['m'] = {1, 4000};
    ranges['a'] = {1, 4000};
    ranges['s'] = {1, 4000};
    queue<pair<string, unordered_map<char, pair<int, int>>>> q;
    q.push({"in", ranges});
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        auto& curr_name = curr.first;
        cout << curr_name << endl;
        for (auto next : workflow_map[curr_name].apply(curr.second)) {
            if (next.first == "A") {
                cout << "found" << endl;
                cout << next.second['x'].first << " " << next.second['x'].second << endl;
                cout << next.second['m'].first << " " << next.second['m'].second << endl;
                cout << next.second['a'].first << " " << next.second['a'].second << endl;
                unsigned long long a = next.second['x'].second - next.second['x'].first + 1;
                unsigned long long b = next.second['m'].second - next.second['m'].first + 1;
                unsigned long long c = next.second['a'].second - next.second['a'].first + 1;
                unsigned long long d = next.second['s'].second - next.second['s'].first + 1;
                part2 += a * b * c * d;
            } else if (next.first == "R") {
                // nothing
            } else {
                q.push(next);
            }
        }
    }

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}
