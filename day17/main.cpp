#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <set>
#include <bitset>
#include <deque>

#include "conwaycubes.cpp"

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
  // this input file is a little complicated, so we will process in sections
  ifstream input;
  string line;
  int i = 0;
  vector<vector<int>> active_coords;
  input.open(filename);
  while (getline(input, line)) {
    vector<int> coord(3, 0);
    for (int j = 0; j < line.size(); j++) {
      if (line[j] == '#') {
        coord[0] = i;
        coord[1] = j;
        active_coords.push_back(coord);
      }
    }
    i++;
  }
  input.close();

  // part 1
  // declare conwaycube of dimension 3
  ConWayCubes<3> cube_simulator(active_coords);

  int iteration = 0;
  while (iteration != 6) {
    cube_simulator.advance_once();
    iteration++;
  }

  cout << "Part 1: " << cube_simulator.get_active_cubes() << endl;

  // part 2
  // first we insert an extra dimension into active coords
  for (auto& coord : active_coords) {
    coord.push_back(0);
  }

  // declare new conwaycube of dimension 4
  ConWayCubes<4> cube_simulator2(active_coords);

  iteration = 0;
  while (iteration != 6) {
    cube_simulator2.advance_once();
    iteration++;
  }
  
  cout << "Part 2: " << cube_simulator2.get_active_cubes() << endl;
  
  return 0;
}
