#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>

using namespace std;

// part 1
bool simulate_one_round(vector<vector<int>>& grid);
bool no_occupied_adjacent(const vector<vector<int>>& grid, const int i, const int j);
bool at_least_four_occupied_adjacent(const vector<vector<int>>& grid, const int i, const int j);
// for part 2
bool simulate_one_round2(vector<vector<int>>& grid);
int count_adjacent_seats(const vector<vector<int>>& grid, const int i, const int j);

int main(int argc, char** argv) {
  // this will allow different input files to be passed
  string filename;
  if (argc > 1) {
    filename = argv[1];
  } else {
    // default input
    filename = "input.txt";
  }
  
  // read the input file, store grid in a matrix
  // we will encode the following: 0 is empty, 1 is occupied, -1 is "floor"
  vector<vector<int>> grid;
  string line;
  ifstream input;
  input.open(filename);
  while (input >> line) {
    int n = line.size();
    vector<int> temp(n, 0);
    for (int i = 0; i < n; i++) {
      if (line[i] == '.') {
        // floor
        temp[i] = -1;
      } else if (line[i] == '#') {
        // occupied
        temp[i] = 1;
      }
      // we do not need a case for empty, since it is initialized to empty
    }
    grid.push_back(temp);
  }
  input.close();
  // make a copy for part 2
  vector<vector<int>> grid_copy = grid;
  
  // part 1
  int count = 0;
  while (simulate_one_round(grid)) {
    count++;
  }
  
  // print out the grid
  int occupied_seats = 0;
  string key = ".L#";
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      cout << key[grid[i][j] + 1];
      if (grid[i][j] == 1) {
        occupied_seats++;
      }
    }
    cout << endl;
  }
  cout << "Part 1: " << count << ", seats: " << occupied_seats << endl;
  
  // part 2
  grid = grid_copy;
  count = 0;
  while (simulate_one_round2(grid)) {
    count++;
  }

  // print out final grid
  occupied_seats = 0;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      cout << key[grid[i][j] + 1];
      if (grid[i][j] == 1) {
        occupied_seats++;
      }
    }
    cout << endl;
  }
  cout << "Part 2: " << count << ", seats: " << occupied_seats << endl;
  
  return 0;
}

// simulates one round of the people moving, returns true if change occured, returns false if no change (part1)
bool simulate_one_round(vector<vector<int>>& grid) {
  // we could probably perform the alogithm in place, or with reduced memory, however a copy will help
  vector<vector<int>> copy = grid;
  bool changed = false;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (copy[i][j] == 0 && no_occupied_adjacent(copy, i, j)) {
        grid[i][j] = 1;
        changed = true;
      } else if (copy[i][j] == 1 && at_least_four_occupied_adjacent(copy, i, j)) {
        grid[i][j] = 0;
        changed = true;
      }
    }
  }
  return changed;
}

// return true if all adjacent seats are empty
bool no_occupied_adjacent(const vector<vector<int>>& grid, const int i, const int j) {
  // determine start and end indices, (which we must restrict near grid edges
  int istart = i == 0 ? 0 : i - 1;
  int iend = i == grid.size() - 1 ? grid.size() - 1 : i + 1;
  int jstart = j == 0 ? 0 : j - 1;
  int jend = j == grid[0].size() - 1 ? grid[0].size() - 1 : j + 1;
  
  for (int ii = istart; ii <= iend; ii++) {
    for (int jj = jstart; jj <= jend; jj++) {
      if (grid[ii][jj] == 1) {
        return false;
      }
    }
  }
  return true;
}

// return true if there are at least 4 adjacent seats
bool at_least_four_occupied_adjacent(const vector<vector<int>>& grid, const int i, const int j) {
  // determine start and end indices, (which we must restrict near grid edges
  int istart = i == 0 ? 0 : i - 1;
  int iend = i == grid.size() - 1 ? grid.size() - 1 : i + 1;
  int jstart = j == 0 ? 0 : j - 1;
  int jend = j == grid[0].size() - 1 ? grid[0].size() - 1 : j + 1;
  int count = 0;
  
  for (int ii = istart; ii <= iend; ii++) {
    for (int jj = jstart; jj <= jend; jj++) {
      if (grid[ii][jj] == 1) {
        count++;;
      }
    }
  }
  // remove the count at grid[i][j]
  count--;
  return count >= 4;
}

bool simulate_one_round2(vector<vector<int>>& grid) {
  // we could probably perform the alogithm in place, or with reduced memory, however a copy will help
  vector<vector<int>> copy = grid;
  bool changed = false;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (copy[i][j] == 0 && count_adjacent_seats(copy, i, j) == 0) {
        grid[i][j] = 1;
        changed = true;
      } else if (copy[i][j] == 1 && count_adjacent_seats(copy, i, j) >= 5) {
        grid[i][j] = 0;
        changed = true;
      }
    }
  }
  return changed;
}

// count adjacent seats, until you hit a seat
int count_adjacent_seats(const vector<vector<int>>& grid, const int i, const int j) {
  // we will just manually traverse in the 8 directions
  // i am not sure if there is a better way or not
  int count = 0;
  // west
  int ii = i;
  int jj = j - 1;
  while (jj >= 0) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    jj--;
  }
  // east
  ii = i;
  jj = j + 1;
  while (jj < grid[0].size()) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    jj++;
  }
  // north
  ii = i - 1;
  jj = j;
  while (ii >= 0) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    ii--;
  }
  // south
  ii = i + 1;
  jj = j;
  while (ii < grid.size()) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    ii++;
  }
  // northwest
  ii = i - 1;
  jj = j - 1;
  while (ii >= 0 && jj >= 0) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    ii--;
    jj--;
  }
  // northeast
  ii = i - 1;
  jj = j + 1;
  while (ii >= 0 && jj < grid[0].size()) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    ii--;
    jj++;
  }
  // southwest
  ii = i + 1;
  jj = j - 1;
  while (ii < grid.size() && jj >= 0) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    ii++;
    jj--;
  }
  // southeast
  ii = i + 1;
  jj = j + 1;
  while (ii < grid.size() && jj < grid[0].size()) {
    if (grid[ii][jj] == 1) {
      count++;
      break;
    }
    if (grid[ii][jj] == 0) {
      break;
    }
    ii++;
    jj++;
  } 
  
  return count;
}

