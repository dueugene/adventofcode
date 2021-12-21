#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

unsigned int binaryStringToUint(string s);

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
    vector<string> vals;
    string val;
    input.open(filename);
    while (input >> val) {
        vals.push_back(val);
    }
    input.close();

    // part 1
    int n_bits = vals[0].size();
    vector<int> bits_count(n_bits, 0);
    int count = 0;
    for (int i = 0; i < vals.size(); i++) {
        for (int j = 0; j < n_bits; j++) {
            bits_count[j] += (vals[i][j] == '1');
        }
    }
    unsigned int gamma = 0;
    for (int j = 0; j < n_bits; j++) {
        gamma = gamma << 1;
        if (bits_count[j] > vals.size() / 2) {
            gamma |= 1;
        }
    }
    unsigned int mask = (1 << n_bits) - 1;
    cout << "gamma: " <<  gamma << endl;
    cout << "beta: " <<(~gamma & mask) << endl;
    cout << "result: " << gamma * (~gamma & mask) << endl;
    
    // part 2
    unordered_set<string> valid_nums(vals.begin(), vals.end());
    
    for (int j = 0; j < n_bits; j++) {
        int bit_count = 0;
        for (string val : valid_nums) {
            bit_count += (val[j] == '1');
        }
        unordered_set<string> temp;
        int n = valid_nums.size();
        bool bit_value = n % 2 == 0 ? bit_count >= n / 2 : bit_count > n / 2 ;
        if (bit_value) {
            for (string val : valid_nums) {
                if (val[j] == '1') {
                    temp.insert(val);
                }
            }
        } else {
            for (string val : valid_nums) {
                if (val[j] == '0') {
                    temp.insert(val);
                }
            }
        }
        valid_nums = temp;
        if (valid_nums.size() == 1) {
            break;
        }
    }
    unsigned int oxygen = binaryStringToUint(*valid_nums.begin());

    valid_nums.clear();
    valid_nums.insert(vals.begin(), vals.end());
    for (int j = 0; j < n_bits; j++) {
        int bit_count = 0;
        for (string val : valid_nums) {
            bit_count += (val[j] == '1');
        }
        unordered_set<string> temp;
        int n = valid_nums.size();
        bool bit_value = n % 2 == 0 ? bit_count < n / 2 : bit_count <= n / 2 ;
        if (bit_value) {
            for (string val : valid_nums) {
                if (val[j] == '1') {
                    temp.insert(val);
                }
            }
        } else {
            for (string val : valid_nums) {
                if (val[j] == '0') {
                    temp.insert(val);
                }
            }
        }
        valid_nums = temp;
        if (valid_nums.size() == 1) {
            break;
        }
    }
    unsigned int co2 = binaryStringToUint(*valid_nums.begin());
    cout << "oxygen: " << oxygen << endl;
    cout << "co2: " << co2 << endl;
    cout << "result: " << oxygen * co2 << endl;
    return 0;
}

unsigned int binaryStringToUint(string s) {
    unsigned int result = 0;
    for (int i = 0; i < s.size(); i++) {
        result = result << 1;
        result |= (s[i] == '1');
    }
    return result;
}