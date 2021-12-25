#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

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
    getline(input, line);
    input.close();

    string holder;
    vector<int> nums;
    istringstream ss(line);
    while (getline(ss, holder, ',')) {
        nums.push_back(stoi(holder));
    }

    // for (auto item : nums) {
    //     cout << item << endl;
    // }

    // part 1
    sort(nums.begin(), nums.end());
    int best = 0;
    int n = nums.size();
    for (int i = 0; i < n / 2; i++) {
        best += (nums[n - 1 - i] - nums[i]);
    }
    cout << "part1: " << best << endl;

    // part 2
    // O(n^2) - ish solution, not sure if there is a better way
    // O(n) - ish extra space
    int largest = nums[n-1];
    vector<int> costs(largest+1, 0);
    for (int i = 0; i < n; i++) {
        int count = 0;
        int cost = 0;
        for (int j = nums[i]; j <= largest; j++) {
            cost += count;
            costs[j] += cost;
            count++;
        }
        count = 1;
        cost = 0;
        for (int j = nums[i] - 1; j >= 0; j--) {
            cost += count;
            costs[j] += cost;
            count++;
        }
    }
    sort(costs.begin(), costs.end());
    cout << "part2: " << costs[0] << endl;
    return 0;
}