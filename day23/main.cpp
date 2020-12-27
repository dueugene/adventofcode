#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <map>

using namespace std;

int play_cups(list<int>& cups, int curr);
int play_cups2(list<int>& cups, int curr, const vector<list<int>::iterator>& cup_pos);
  
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
  list<int> cups;
  char c;
  input.open(filename);
  while (input >> c) {
    cups.push_back(c- '0');
  }
  input.close();

  // print cups
  for (auto n : cups) {
    cout << n << " ";
  }
  cout  << endl;
  // make a copy of cups for part2
  list<int> cups2 = cups;
  

  // part 1
  int count = 100;
  int next = cups.front();
  while (count--) {
    next = play_cups(cups, next);
  }
  // find the 1, and rearange the list
  auto it = std::find(cups.begin(), cups.end(), 1);
  cups.splice(cups.end(), cups, cups.begin(), it);
  cups.pop_front();
  cout << "Part 1: ";
  for (auto n : cups) {
    cout << n;
    }
  cout << endl;


  // part 2
  // add up to a million more cups
  int cup = 10;
  while (cup < 1e6 + 1) {
    cups2.push_back(cup);
    cup++;
  }
  
  // in this case the linear search will kill us, so we store a vector which points to each member in this list
  vector<list<int>::iterator> cup_position(1e6 + 1);
  for (auto it = cups2.begin(); it != cups2.end(); it++) {
    cup_position[*it] = it;
  }

  count = 1e7;
  next = cups2.front();
  while (count--) {
    next = play_cups2(cups2, next, cup_position);
  }

  // find the 1, and print out the next two
  it = cup_position[1];
  if (it != cups2.begin() && it != std::next(cups2.begin(),1)) {
    cups2.splice(cups2.end(), cups2, cups2.begin(), std::next(cups2.begin(),2));
  }
  cout << "Part 2: " << *std::next(it,1) << " " << *std::next(it,2) << endl;
  
  
  return 0;
}

int play_cups(list<int>& cups, int curr) {
  int n = cups.size();
  // since the size of this list is small, I don't mind using find, linear time
  auto it_curr = find(cups.begin(), cups.end(), curr);
  // "rotate" the array so it_curr is at begining
  if (it_curr != cups.begin()) {
    cups.splice(cups.end(), cups, cups.begin(), it_curr);
  }
  list<int> removed;
  removed.splice(removed.begin(), cups, std::next(cups.begin(),1), std::next(cups.begin(),4));

  int dest = curr - 1;
  while (dest == 0 || find(removed.begin(), removed.end(), dest) != removed.end()) {
    dest--;
    if (dest <= 0) {
      dest = n;
    }
  }

  auto it_dest = find(cups.begin(), cups.end(), dest);
  cups.splice(std::next(it_dest,1), removed);
  
  return *std::next(it_curr,1);
}

int play_cups2(list<int>& cups, int curr, const vector<list<int>::iterator>& cup_pos) {
  int n = cups.size();

  // this search is no O(1) time
  auto it_curr = cup_pos[curr];
  
  // "rotate" the array so we know for sure it_curr is not near the end
  if (it_curr != cups.begin() && it_curr != next(cups.begin(),1) && it_curr != next(cups.begin(),2) && it_curr != next(cups.begin(),3)) {
    cups.splice(cups.end(), cups, cups.begin(), next(cups.begin(),4));
  }

  // we will not remove items from this list anymore, but just check the 3 of them
  int dest = curr - 1;
  while (dest == 0 || dest == *next(it_curr,1) || dest == *next(it_curr,2) || dest == *next(it_curr,3)) {
    dest--;
    if (dest <= 0) {
      dest = n;
    }
  }

  auto it_dest = cup_pos[dest];
  cups.splice(next(it_dest,1), cups, next(it_curr,1), next(it_curr,4));
  
  return *std::next(it_curr,1);

}
