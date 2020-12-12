#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>

using namespace std;

// part 1
int manhattan_dist(const vector<pair<char, int>> instructions);
int manhattan_dist2(const vector<pair<char, int>> instructions);

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
  vector<pair<char, int>> instructions;
  char c;
  int num;
  ifstream input;
  input.open(filename);
  while (input >> c >> num) {
    instructions.push_back({c, num});
  }
  input.close();

  for (auto a : instructions) {
    cout << a.first << " " << a.second << endl;
  }

  int dist = manhattan_dist(instructions);
  cout << dist << endl;
  
  // part 1
  cout << manhattan_dist2(instructions) << endl;
  return 0;
}

int manhattan_dist(const vector<pair<char, int>> instructions) {
  int x = 0;
  int y = 0;
  int face = 1; // 0: north, 1: east, 2: south, 3: west
  for (auto a : instructions) {
    switch (a.first) {
    case 'N':
      y += a.second;
      break;
    case 'E':
      x += a.second;
      break;
    case 'S':
      y -= a.second;
      break;
    case 'W':
      x -= a.second;
      break;
    case 'F':
      switch (face) {
      case 0:
        y += a.second;
        break;
      case 1:
        x += a.second;
        break;
      case 2:
        y -= a.second;
        break;
      case 3:
        x -= a.second;
      }
      break;
    case 'L':
      // turn left is equivalent to turn right 360 - angle
      a.second = 360 - a.second;
      // fallthrough intended
    case 'R':
      face += a.second / 90;
      face %= 4;
      break;
    default:
      cout << "something went wrong" << endl;
    }
  }
  x = x > 0 ? x : -x;
  y = y > 0 ? y : -y;
  return x + y;
}

int manhattan_dist2(const vector<pair<char, int>> instructions) {
  int wayx = 10;
  int wayy = 1;
  int x = 0;
  int y = 0;
  for (auto a : instructions) {
    switch (a.first) {
    case 'N':
      wayy += a.second;
      break;
    case 'E':
      wayx += a.second;
      break;
    case 'S':
      wayy -= a.second;
      break;
    case 'W':
      wayx -= a.second;
      break;
    case 'F':
      // "move forward"
      x += a.second*wayx;
      y += a.second*wayy;
      break;
    case 'R':
      // turn right is equivalent to turn left 360 - angle
      a.second = 360 - a.second;
      // fallthrough intended
    case 'L':
      // apply a manual rotation matrix
      int a00, a01, a10, a11, b;
      switch (a.second) {
      case 90:
        a00 = 0;
        a01 = -1;
        a10 = 1;
        a11 = 0;
        break;
      case 180:
        a00 = -1;
        a01 = 0;
        a10 = 0;
        a11 = -1;
        break;
      case 270:
        a00 = 0;
        a01 = 1;
        a10 = -1;
        a11 = 0;
        break;
      case 0:
        a00 = 1;
        a01 = 0;
        a10 = 0;
        a11 = 1;
        break;
      default:
        cout << "something went wrong" << endl;
      }
      b = a00*wayx + a01*wayy;
      wayy = a10*wayx + a11*wayy;
      wayx = b;
      break;
    default:
      cout << "something went wrong" << endl;
    }
  }
  x = x > 0 ? x : -x;
  y = y > 0 ? y : -y;
  return x + y;
}
