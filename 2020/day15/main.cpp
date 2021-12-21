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
  string a;
  vector<int> nums;
  input.open(filename);
  while (getline(input, a, ',')) {
    nums.push_back(stoi(a));
  }
  input.close();

  // since the number being said next depends on when the prev number 
  // was last spoken, all the information can be stored in a size 2020 vector
  vector<int> last_said(2020,-1);
  int prev = nums[0];
  int count = 1;
  for (int i =  1; i < nums.size(); i++) {
    int curr = nums[i];
    // process prev
    last_said[prev] = count;
    // set prev to curr
    prev = curr;
    count++;
  }
  while (count < 2020) {
    int curr;
    if (last_said[prev] == -1) {
      // prev has never been said before
      curr = 0;
    } else {
      curr = count - last_said[prev];
    }
    // process prev
    last_said[prev] = count;
    // set prev to curr 
    prev = curr;
    count++;
  }
  
  cout << "Part 1: " << prev <<endl;

  // since there are now 30,000,000 turns
  // we can consider storing the info in an unordered_map
  unordered_map<int, int> last_said2;
  prev = nums[0];
  count = 1;
  for (int i =  1; i < nums.size(); i++) {
    int curr = nums[i];
    // process prev
    last_said2[prev] = count;
    // set prev to curr
    prev = curr;
    count++;
  }
   while (count < 30000000) {
    int curr;
    if (last_said2.find(prev) == last_said2.end()) {
      // prev has never been said before
      curr = 0;
    } else {
      curr = count - last_said2[prev];
    }
    // process prev
    last_said2[prev] = count;
    // set prev to curr 
    prev = curr;
    count++;
  }

  cout << "Part 2: " << prev << endl;
  cout << last_said2.size() << endl;

  return 0;
}
