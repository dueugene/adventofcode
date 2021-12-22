#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

long long simulated_fish(vector<int> fish_count, int days);

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
    getline(input, line);
    input.close();

    string holder;
    vector<int> nums;
    istringstream ss(line);
    while (getline(ss, holder, ',')) {
        nums.push_back(stoi(holder));
    }

    for (auto item : nums) {
        cout << item << endl;
    }

    // part 1
    vector<int> fish_count(9, 0);
    for (int num : nums) {
        fish_count[num]++;
    }
    cout << "part1: " << simulated_fish(fish_count, 80) << endl;
    // part 2
    cout << "part2: " << simulated_fish(fish_count, 256) << endl;

    return 0;
}

long long simulated_fish(vector<int> fish_count, int days) {
    vector<long long> long_fish_count(9, 0);
    for (int j = 0; j < 9; j++) {
        long_fish_count[j] = (long long)fish_count[j];
    }

    for (int i = 0; i < days; i++) {
        vector<long long> next_day_count(9, 0);
        for (int j = 1; j < 9; j++) {
            next_day_count[j - 1] = long_fish_count[j];
        }
        next_day_count[6] += long_fish_count[0];
        next_day_count[8] = long_fish_count[0];

        long_fish_count = next_day_count;
    }
    long long count = 0;
    for (long long num : long_fish_count) {
        // cout << num << " ";
        count += num;
    }
    return count;
}
