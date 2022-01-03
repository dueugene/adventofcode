#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>
#include <list>
#include <memory>

#include "../utils/file_read.cpp"
#include "../utils/hash.cpp"

using namespace std;

class ALU {
public:
    ALU() : w_(0), x_(0), y_(0), z_(0) {};
    int compute(vector<string> instructions, string num, vector<int> init = {0,0,0,0});
    vector<int> compute_verbose(vector<string> instructions, string num, vector<int> init);
private:
    int w_;
    int x_;
    int y_;
    int z_;

    int* point_to_var(char c);
};

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "sample2.txt";
    }

    // read the input file
    vector<string> instructions = read_input(filename);
    
    // part 1
    // thoughts
    // to test all possible numbers is 9^14, which is too large to test
    // there are only 4 variables, w,x,y,z so presumbly, only 4 digits are important?
    // that is actually not true
    // could I go reverse? or build a map of the operations?
    // string model_number = "36111111111119";
    string model_number = "13579246899999";
    ALU alu;
    int z = alu.compute(instructions, model_number);
    // it appears some of the instructions are redundant or don't do much, could it be I need to 
    // simplify the instructions?
    // example div x 1, mul x 1, add x 0, do nothing
    // observations:
    // imp always to w, occurs 14 times for each digit, state of w does not need to be carried forward
    // after imp w always followed by mul x 0, state of x does not need to be carried forward
    // mul y 0 in every segment as well, before it is ever used, state of y does not need carry forward
    // strategy: for each segment, try digits 1-9, record output
    // mapping[i] stores a map of [{digit i,prev z}] -> output of z
    
    // vector<unordered_map<pair<int,int>, int, PairHasher>> mapping(19);
    // mapping[i] stores a map of [output z] -> all possible {digit, prev z} that results in z
    vector<unordered_map<int, list<pair<int,int>>>> mapping(18);
    for (int i = 0; i < 4; i++) {
        int start = 0*18;
        vector<string> segment(instructions.begin() + start, instructions.begin()+start+18);
        unordered_map<int, list<pair<int,int>>> possible_z_state;
        if (i != 0) {
           possible_z_state = mapping[i-1]; 
        } else {
            // dummy initial state
            possible_z_state[0] = list<pair<int,int>>(0);
        }
        vector<int> init(4, 0);
        for (auto p : possible_z_state) {
            init[3] = p.first;
            for (char c = '1'; c <= '9'; c++) {
                int z = alu.compute(segment, string(1,c), init);
                cout << z << " ";
                cout << endl;
                mapping[i][z].push_back({c-'0', p.first});
            }
        }
        cout << i << ": map size " << mapping[i].size() << endl;
    }
    cout << "part1: " << z << endl;

    // part 2
    int c = 0;
    cout << "part2: " << c << endl;

    return 0;
}


int * ALU::point_to_var(char c) {
    int *a;
    switch (c) {
        case 'w':
        a = &w_;
        break;
    case 'x':
        a = &x_;
        break;
    case 'y':
        a = &y_;
        break;
    case 'z':
        a = &z_;
        break;
    }
    return a;
}

int ALU::compute(vector<string> instructions, string num, vector<int> init) {
    int ind = 0;
    w_ = init[0];
    x_ = init[1];
    y_ = init[2];
    z_ = init[3];
    for (string line : instructions) {
        string op = line.substr(0, 3);
        char var = line[4];
        int* a = point_to_var(var);

        if (op == "inp") {
            *a = num[ind] - '0';
            ind++;
        } else if (op == "add") {
            if (line[6] >= 'w' && line[6] <= 'z') {
                int* b = point_to_var(line[6]);
                *a += *b;
            } else {
                int b = stoi(line.substr(6));
                *a += b;
            }
        } else if (op == "mul") {
            if (line[6] >= 'w' && line[6] <= 'z') {
                int* b = point_to_var(line[6]);
                *a *= *b;
            } else {
                int b = stoi(line.substr(6));
                *a *= b;
            }
        } else if (op == "div") {
            if (line[6] >= 'w' && line[6] <= 'z') {
                int* b = point_to_var(line[6]);
                *a /= *b;
            } else {
                int b = stoi(line.substr(6));
                *a /= b;
            }
        } else if (op == "mod") {
            if (line[6] >= 'w' && line[6] <= 'z') {
                int* b = point_to_var(line[6]);
                *a %= *b;
            } else {
                int b = stoi(line.substr(6));
                *a %= b;
            }
        } else if (op == "eql") {
            if (line[6] >= 'w' && line[6] <= 'z') {
                int* b = point_to_var(line[6]);
                *a = *a == *b;
            } else {
                int b = stoi(line.substr(6));
                *a = *a == b;
            }
        }

    }
    return z_;
}

vector<int> ALU::compute_verbose(vector<string> instructions, string num, vector<int> init) {


    compute(instructions, num, init);

    init[0] = w_;
    init[1] = x_;
    init[2] = y_;
    init[3] = z_;

    return init;
}