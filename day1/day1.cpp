#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>

using namespace std;

int findTwoNums(const vector<int>&, int, int);
int findThreeNums(const vector<int>&, int, int);

int main() {
  vector<bool> found(2021, false);
  vector<int> input_nums;
  ifstream input;
  input.open("input.txt");
  int num;
  while (input >> num) {
    input_nums.push_back(num);
  }
  input.close();

  // part 1
  findTwoNums(input_nums, 2020, 0);
  // part 2
  findThreeNums(input_nums, 2020, 0);
  
  return 0;
  
}

int findTwoNums(const vector<int>& nums, int target, int start) {
  unordered_set<int> found;
  for (int i = start; i < nums.size(); i++) {
    if (nums[i] <= target && found.find(target-nums[i]) != found.end()) {
      cout << nums[i] << "*" << target-nums[i] << " = " <<  nums[i] * (target-nums[i]) << endl;
      return nums[i]*(target-nums[i]);
    }
    found.insert(nums[i]);
  }
  return -1;
}

int findThreeNums(const vector<int>& nums, int target, int start) {
  for (int i = start; i < nums.size(); i++) {
    int num = nums[i];
    long long int num2 = findTwoNums(nums, target - num, i+1);
    if (num2 > 0) {
      cout << num << "*" << num2 << " = " <<  num * num2 << endl;
      return num * num2;
    }
  }
  return -1;
}
