#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>

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
  vector<long long> nums;
  long long num;
  ifstream input;
  input.open(filename);
  while (input >> num) {
    nums.push_back(num);
  }
  input.close();
  
  int n = nums.size();
  // part 1
  int front = 0;
  int back = 24;
  int next = 25;

  while (next < n && make_sum(nums, front, back, nums[next])) {
    front++;
    back++;
    next++;
  }
  
  cout << "Part 1: " << nums[next] << endl;

  // part 2
  long long curr_sum = 0;
  long long target = nums[next];
  deque<int> smallest;
  deque<int> largest;
  front = 0;
  back = 0;
  while (back < n && curr_sum != target) {
    while (curr_sum < target) {
      curr_sum += nums[back];
      while (!smallest.empty() && nums[back] <= nums[smallest.back()]) {
        smallest.pop_back();
      }
      smallest.push_back(back);
      while (!largest.empty() && nums[back] >= nums[largest.back()]) {
        largest.pop_back();
      }
      largest.push_back(back);
      back++;
    }
    while (curr_sum > target) {
      curr_sum -= nums[front];
      front++;
      while (!smallest.empty() && smallest.front() < front) {
        smallest.pop_front();
      }
      while (!largest.empty() && largest.front() < front) {
        largest.pop_front();
      }
    }
  }

  cout << "Part 2: smallest: " << nums[smallest.front()] << ", largest: " << nums[largest.front()] << ", sum: " << nums[smallest.front()] + nums[largest.front()] << endl; 
         
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

