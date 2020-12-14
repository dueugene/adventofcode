#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <bitset>

using namespace std;

vector<int> make_bitmask(string in);
long long apply_bitmask(const vector<int>& bitmask, long long num);
vector<long long> apply_bitmask_v2(const vector<int>& bitmask, long long num);

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
  // vector to store bitmask information
  vector<int> bitmask(36, -1); 
  // use unordered map to simulate the memory, since they do not specify the size and range of memory
  // we require at least 36 bits, so we will use long long to store our bits
  unordered_map<string, long long> memory;
  int leave_time;
  string a, b, c;
  ifstream input;
  input.open(filename);
  while (input >> a >> b >> c) {
    if (a == "mask") {
      bitmask = make_bitmask(c);
    } else {
      long long num = stoll(c);
      memory[a] = apply_bitmask(bitmask, num);
      // for debugging
      // cout << num << " " << apply_bitmask(bitmask, num) << endl;
      // cout << bitset<64>(num) << " " << bitset<64>(apply_bitmask(bitmask, num)) << endl;
    }
  }
  input.close();

  long long ans = 0;
  for (auto it = memory.begin(); it != memory.end(); it++) {
    ans += it->second;
  }
  cout << "Part 1: " << ans << endl;


  // part 2
  // we will simply re-read the input file, and apply the bitmask version2
  unordered_map<long long, long long> memory2;
  input.open(filename);
  while (input >> a >> b >> c) {
    if (a == "mask") {
      bitmask = make_bitmask(c);
    } else {
      long long num = stoll(c);
      a.erase(a.size()-1);
      long long mem_loc = stoll(a.substr(4));
      vector<long long> mem_locs = apply_bitmask_v2(bitmask, mem_loc);
      for (long long mem : mem_locs) {
        memory2[mem] = num;
      }
    }
  }
  input.close();
  ans = 0;
  for (auto it = memory2.begin(); it != memory2.end(); it++) {
    ans += it->second;
  }
  cout << "Part 2: " << ans << endl;
  
  return 0;
}

// store bitmask representation in a vector
vector<int> make_bitmask(string in) {
  vector<int> result(36, -1);
  // 0: apply 0
  // 1: apply 1
  // -1: dont apply anything
  for (int i = 0; i < in.size(); i++) {
    if (in[in.size()- 1 - i] == '1') {
      result[i] = 1;
    } else if (in[in.size()- 1 - i] == '0') {
      result[i] = 0;
    } else {
      result[i] = -1;
    }
  }
  return result;
}

long long apply_bitmask(const vector<int>& bitmask, long long num) {
  for (int i = 0; i < bitmask.size(); i++) {
    if (bitmask[i] == 0) {
      num &= ~(1LL << i);
    } else if (bitmask[i] == 1) {
      num |= 1LL << i;
    }
  }
  return num;
}

vector<long long> apply_bitmask_v2(const vector<int>& bitmask, long long num) {
  vector<long long> result = {num};
  for (int i = 0; i < bitmask.size(); i++) {
    if (bitmask[i] == 0) {
      // do nothing
    } else if (bitmask[i] == 1) {
      // set the ith bit to 1, in all numbers in result
      for (int j = 0; j < result.size(); j++) {
        result[j] |= 1LL << i;
      }
    } else {
      // floating bit
      // for every number in result, add an extra with the ith bit toggled
      int n = result.size();
      for (int j = 0; j < n; j++) {
        long long temp = result[j];
        result.push_back(temp ^ 1LL << i);
      }
    }
  }
  return result;
}
