#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int count_trees(const vector<string>& map, int x, int y);

int main() {
  string line;
  vector<string> map;
  ifstream input;
  input.open("input.txt");
  while (input >> line) {
    map.push_back(line);
  }
  input.close();
  long long total = 1;
  vector<vector<int>> slopes = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
  for (int i = 0; i < slopes.size(); i++) {
    int x = slopes[i][0];
    int y = slopes[i][1];
    int count = count_trees(map, x, y);
    cout << x << " right " << y << " down: " << count << endl;
    total *= count;
  }
  cout << "Total multiplied: " << total << endl;
  return 0;
  
}

// count the number of trees encounted when going along path specified by x, y
int count_trees(const vector<string>& map, int x, int y) {
  int n = map.size();
  int m = map[0].size();
  int i = 0, j = 0;
  int count = 0;
  while (i < n) {
    if (map[i][j] == '#') {
      count++;
    }
    j += x;
    j %= m;
    i += y;
  }
  return count;

}
