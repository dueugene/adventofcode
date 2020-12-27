#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <unordered_set>

using namespace std;

// unordered_set has no pair hasher, so we define one we found on
// https://www.techiedelight.com/use-pair-key-std-unordered_set-cpp/
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1, T2> const &pair) const
    {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);
 
        return h1 ^ h2;
    }
};

pair<int, int> get_location(const string& tile);
  
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
  vector<string> tiles;
  string line;
  input.open(filename);
  while (input >> line) {
    tiles.push_back(line);
  }
  input.close();
  
  // part 1
  // we use a set to keep track of black tile "coordinates"
  unordered_set<pair<int,int>, pair_hash> black_tiles;
  for (auto tile : tiles) {
    pair<int, int> loc = get_location(tile);
    if (black_tiles.find(loc) == black_tiles.end()) {
      black_tiles.insert(loc);
    } else {
      black_tiles.erase(loc);
    }
  }
  
  cout << "Part 1: " << black_tiles.size() << endl;


  // part 2
  // we can approach this similar to the conway cubes problem, where we use our custom hexagonal coords
  
  // define all neighbour coordinates (relative)
  vector<pair<int, int>> neighbours = {{2, 0},
                                       {-2, 0},
                                       {1, 1},
                                       {-1, 1},
                                       {1, -1},
                                       {-1, -1}};

  int days = 100;
  while (days--) {
    // create a set of all coords to check, black tiles and immediate neighbours
    unordered_set<pair<int,int>, pair_hash> to_check;
    for (auto it = black_tiles.begin(); it != black_tiles.end(); it++) {
      for (pair<int, int> p : neighbours) {
        pair<int, int> coord = *it;
        coord.first += p.first;
        coord.second += p.second;
        to_check.insert(coord);
      }
    }
    
    unordered_set<pair<int,int>, pair_hash> next_black_tiles;
    // loop through all tiles to check
    // then apply the flipping rules
    for (auto it = to_check.begin(); it != to_check.end(); it++) {

      // count the number of black tile neighbours
      int count = 0;
      for (pair<int, int> p : neighbours) {
        pair<int, int> coord = *it;
        coord.first += p.first;
        coord.second += p.second;
        if (black_tiles.find(coord) != black_tiles.end()) {
          count++;
        }
      }

      if (black_tiles.find(*it) != black_tiles.end()) {
        // black tile criteria
        if (!(count == 0 || count > 2)) {
          next_black_tiles.insert(*it);
        }
      } else {
        // white tile criteria
        if (count == 2) {
          next_black_tiles.insert(*it);
        }
      }
      
    }
    black_tiles = next_black_tiles;
  }
  
  cout << "Part 2: " << black_tiles.size() << endl;
  
  return 0;
}


// interpret the string as a set of coordinates
// going east or west is +/- 2 in the x coord
// going south/north (followed by east/west) is +/- 1 in the y, and then +/- 1 in the x
pair<int, int> get_location(const string& tile) {
  pair<int, int> result = {0, 0};
  int i = 0;
  while (i < tile.size()) {
    switch (tile[i]) {
    case 'e':
      result.first += 2;
      break;
    case 'w':
      result.first -= 2;
      break;
    case 's':
      result.second++;
      i++;
      if (tile[i] == 'e') {
        result.first++;
      } else {
        result.first--;
      }
      break;
    case 'n':
      result.second--;
      i++;
      if (tile[i] == 'e') {
        result.first++;
      } else {
        result.first--;
      }
      break;
    }
    i++;
  }
  
  return result;
}
