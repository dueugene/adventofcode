#include <iostream>
#include <list>
#include <sstream>

#include "input_reader.hpp"

using namespace std;

class Code {
   public:
    Code(const string& s) : code(s) {
        for (int i = 0; i < code.size(); ++i) {
            if (code[i] == '-' || code[i] == '=') {
                operation_ind = i;
                break;
            }
        }
    }
    char get_operation() const { return code[operation_ind]; }
    string get_label() const { return code.substr(0, operation_ind); }
    int get_value() const {
        if (get_operation() == '-') {
            return -1;
        }
        return stoi(code.substr(operation_ind + 1));
    }
    string get_code() const { return code; }
    bool operator==(const Code& other) const { return this->get_label() == other.get_label(); }

   private:
    string code;
    int operation_ind;
};

int calc_hash(const string& s, bool stop_early = false) {
    int result = 0;
    for (const auto& c : s) {
        if (stop_early && (c == '-' || c == '=')) {
            break;
        }
        result += c;
        result *= 17;
        result %= 256;
    }
    return result;
}

class CodeHashMap {
   public:
    CodeHashMap() : hash_map(256) {}

    void apply(const Code& code) {
        size_t hash = calc_hash(code.get_label());
        list<Code>& l = hash_map[hash];
        auto it = find(l.begin(), l.end(), code);
        switch (code.get_operation()) {
            case '-':
                if (it != l.end()) {
                    l.erase(it);
                }
                break;
            case '=':
                if (it != l.end()) {
                    *it = code;
                } else {
                    l.push_back(code);
                }
                break;
        }
    }

    int focusing_power() const {
        int result = 0;
        for (int i = 0; i < hash_map.size(); ++i) {
            if (hash_map[i].size() > 0) {
                int j = 0;
                for (auto c : hash_map[i]) {
                    result += (i + 1) * (j + 1) * c.get_value();
                    j++;
                }
            }
        }
        return result;
    }

    void print_map() const {
        for (int i = 0; i < hash_map.size(); ++i) {
            if (hash_map[i].size() > 0) {
                cout << "Box " << i << ": ";
                for (auto c : hash_map[i]) {
                    cout << "[" << c.get_label() << " " << c.get_value() << "] ";
                }
                cout << endl;
            }
        }
    }

   private:
    vector<list<Code>> hash_map;
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
    vector<string> lines = input_reader::read_as_strings(filename);
    vector<Code> codes;
    for (const auto& line : lines) {
        // cout << line << endl;
        istringstream ss(line);
        string temp;
        while (getline(ss, temp, ',')) {
            codes.push_back(Code(temp));
        }
    }

    long long int part1 = 0;
    for (auto& m : codes) {
        cout << m.get_code() << " ";
        cout << calc_hash(m.get_code()) << endl;
        part1 += calc_hash(m.get_code());
    }
    cout << "part1: " << part1 << endl;

    // part 2
    ulong part2 = 0;
    CodeHashMap hm;
    for (auto& m : codes) {
        cout << m.get_code() << " ";
        cout << calc_hash(m.get_label()) << " " << m.get_value() << endl;
        hm.apply(m);
        hm.print_map();
    }
    part2 = hm.focusing_power();
    cout << "part2: " << part2 << endl;

    return 0;
}
