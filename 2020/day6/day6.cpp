#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

void count_yeses(const vector<int>& nums, int& a, int& b, int people);
int main() {
  string line;
  ifstream input;
  int part1 = 0, part2 = 0;
  int people = 0;
  vector<int> seen(26, 0);
  input.open("input.txt");
  while (getline(input,line)) {
    if (line.empty()) {
      // marks the end of a group add it to the total score, and reset group
      int a, b;
      count_yeses(seen, a, b, people);
      part1 += a;
      part2 += b;
      people = 0;
      fill(seen.begin(), seen.end(), 0);
    } else {
      people++;
      for (char c : line) {
        if (c >= 'a' && c <= 'z') {
          seen[c - 'a']++;
        }
      }
    }
  }
  input.close();
  cout << "Part1: " << part1 << endl;
  cout << "Part2: " << part2 << endl;
  return 0;
}

void count_yeses(const vector<int>& nums, int& a, int& b, int people) {
  a = 0;
  b = 0;
  for (int i = 0; i < nums.size(); i++) {
    if (nums[i] > 0) {a++;}
    if (nums[i] == people) {b++;}
  }
  return;
}


