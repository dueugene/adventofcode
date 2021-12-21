#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;

// vector hasher from before in day17, now modified for lists
struct ListHasher {
  int operator()(const list<int> &V) const {
    int hash = V.size();
    for(auto &i : V) {
        hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
  }
};

int play_game1(list<int> player1, list<int> player2);
int get_score(list<int> nums);
bool play_game2(list<int>& player1, list<int>& player2);

int main(int argc, char** argv) {
  // this will allow different input files to be passed
  string filename;
  if (argc > 1) {
    filename = argv[1];
  } else {
    // default input
    filename = "input.txt";
  }

  // declare variables
  list<int> player1, player2;
  // read the input file
  ifstream input;
  string line;
  input.open(filename);
  // player 1
  getline(input, line);
  while (getline(input, line) && !line.empty()) {
    player1.push_back(stoi(line));
  }
  // player2
  getline(input, line);
  while (getline(input, line) && !line.empty()) {
    player2.push_back(stoi(line));
  }
  for (int a : player1) {
    cout << a << " ";
  }
  cout << endl;
  for (int a : player2) {
    cout << a << " ";
  }
  cout << endl;

  int result = play_game1(player1, player2);
  cout << "Part 1: " << result << endl;


  // part 2
  play_game2(player1, player2);
  if (player1.empty()) {
    result = get_score(player2);
  } else {
    result = get_score(player1);
  }
  cout << "Part 2: " << result << endl;
  return 0;
}

int play_game1(list<int> player1, list<int>player2) {
  while (!player1.empty() && !player2.empty()) {
    int a = player1.front();
    player1.pop_front();
    int b = player2.front();
    player2.pop_front();
    if (a > b) {
      player1.push_back(a);
      player1.push_back(b);
    } else {
      player2.push_back(b);
      player2.push_back(a);
    }
  }

  int result;
  if (player1.empty()) {
    result = get_score(player2);
  } else {
    result = get_score(player1);
  }
  return result;
}

int get_score(list<int> nums) {
  int result = 0;
  int count = 1;
  for (auto it = nums.rbegin(); it != nums.rend(); it++) {
    result += (*it) * count;
    count++;
  }
  return result;
}

bool play_game2(list<int>& player1, list<int>& player2) {
  unordered_set<list<int>, ListHasher> seen1;
  unordered_set<list<int>, ListHasher> seen2;
  while (!player1.empty() && !player2.empty()) {
    // instantly end condition
    if (seen1.find(player1) != seen1.end() && seen2.find(player2) != seen2.end()) {
      return true;
    }
    
    seen1.insert(player1);
    seen2.insert(player2);
    
    bool player1_won = false;
    int a = player1.front();
    player1.pop_front();
    int b = player2.front();
    player2.pop_front();
    
    if (a <= player1.size() && b <= player2.size()) {
      list<int> copy1(player1.begin(), next(player1.begin(), a));
      list<int> copy2(player2.begin(), next(player2.begin(), b));
      player1_won = play_game2(copy1, copy2);
    } else {
      player1_won = a > b;
    }
    if (player1_won) {
      player1.push_back(a);
      player1.push_back(b);
    } else {
      player2.push_back(b);
      player2.push_back(a);
    }
  }
  
  if (player1.empty()) {
    return false;
  } else {
    return true;
  }
}
