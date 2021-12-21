#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <sstream>

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
  int leave_time;
  string line;
  ifstream input;
  input.open(filename);
  input >> leave_time;
  input >> line;
  input.close();

  // retrieve buses from input line
  istringstream ss(line);
  vector<int> buses;
  vector<int> pos;
  int count = 0;
  string token;
  while (getline(ss, token, ',')) {
    if (token != "x") {
      buses.push_back(stoi(token));
      pos.push_back(count);
    }
    count++;
  }

  // part 1
  int best_bus = 0;
  int earliest = leave_time*2;
  for (int bus : buses) {
    int cand = (leave_time / bus) * bus + bus;
    if (cand < earliest) {
      best_bus = bus;
      earliest = cand; 
    }
  }
  
  cout << "Part 1: best bus: " << best_bus << ", leave time: " << earliest << ", ans: " << best_bus*(earliest - leave_time) << endl;

  // part 2

  // first idea, however it ran for too long
  // we will simply take first bus, and increment it, until all constraints are satisfied

  // second idea, realize that for bus1*a = bus2*b + c. the next time this is satisfied is at bus1*bus2
  // so we loop through each bus, solving the above problem.
  // at next iteration, we solve bus1_next*a_new = bus3*d + e, where bus1_next = bus1*bus2
  long long ans = buses[0];
  long long add = buses[0];
  for (int i = 1; i < buses.size(); i++) {
    while ((ans + pos[i]) % buses[i] != 0) {
      ans += add;
    }
    add *= buses[i]; 
  }
  cout << "Part 2: " << ans << endl;
  
  return 0;
}
