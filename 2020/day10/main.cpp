#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>

using namespace std;

bool make_sum(const vector<long long>& nums, int front, int back, long long target);

int main(int argc, char** argv) {
  // this will allow different input files to be passed
  string filename;
  if (argc > 1) {
    filename = argv[1];
  } else {
    // default input
    filename = "input.txt";
  }
  
  // read the input file, store each number in a vector
  // the input file has numbers that do not fit in int, so we will use long long
  vector<int> nums;
  int num;
  ifstream input;
  input.open(filename);
  while (input >> num) {
    nums.push_back(num);
  }
  input.close();
  
  int n = nums.size();
  // part 1
  sort(nums.begin(), nums.end());
  int count1 = 0;
  int count2 = 0;
  int count3 = 0;
  int prev = 0;
  for (int i = 0; i < n; i++) {
    int diff = nums[i] - prev;
    if (diff == 3) {
      count3++;
    } else if (diff == 2) {
      count2++;
    } else {
      count1++;
    }
    prev = nums[i];
  }
  count3++;
  cout << "Part 1: " << count1*count3 << endl;

  // part 2
  // insert 0 as starting point of nums
  nums.insert(nums.begin(), 0);
  // use a dp table
  vector<long long> dp(n+1, 0); // dp i represents the number of ways we can reach adapter i
  dp[0] = 1;
  for (int i = 1; i < n+1; i++) {
    int j = 1;
    while (i-j >= 0 && nums[i] - nums[i-j] <= 3) {
      dp[i] += dp[i-j];
      j++;
    }
  }
  cout << "Part 2: " << dp[n] << endl;
  return 0;
}

bool make_sum(const vector<long long>& nums, int front, int back, long long target) {
  for (int i = front; i <= back; ++i) {
    for (int j = i + 1; j <= back; ++j) {
      if (nums[i] + nums[j] == target) {return true;}
    }
  }
  return false;
}

