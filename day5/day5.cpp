#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int get_seat_id(const string& word);

int main() {
  string line;
  int max_id = 0;
  vector<bool> seats(1024, false);
  ifstream input;
  input.open("input.txt");
  while (getline(input,line)) { 
    int id = get_seat_id(line);
    seats[id] = true;
    max_id = max(id, max_id);
  }
  input.close();
  cout << "Max id: " << max_id << endl;
  for (int i = 0; i < 1024; i++) {
    if (seats[i] == false) {
      cout << "missing seat: " << i << endl;
    }
  }
  return 0;
}

// calculate the seat id
int get_seat_id(const string& word) {
  int low = 0;
  int high = 127;
  int ind = 0;
  while (low < high) {
    int mid = (low + high) / 2;
    if (word[ind] == 'F') {
      high = mid;
    } else {
      low = mid + 1;
    }
    ind++;
  }
  int id = low*8;
  low = 0;
  high = 7;
  while (low < high) {
    int mid = (low + high) / 2;
    if (word[ind] == 'L') {
      high = mid;
    } else {
      low = mid + 1;
    }
    ind++;
  }
  id += low;
  return id;
}
