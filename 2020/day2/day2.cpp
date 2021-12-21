#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

bool isValidPassword(const string&, char, int, int);
bool isValidPasswordPart2(const string&, char, int, int);

int main() {
  ifstream input;
  input.open("input.txt");
  int num_low, num_high;
  string password;
  char to_match, temp;
  int part1 = 0, part2 = 0, count = 0;
  while (input >> num_low) {
    input >> temp;
    input >> num_high;
    input >> to_match;
    input >> temp;
    input >> password;
    cout << num_low << " " << num_high << " " << to_match << " " << password << endl;
    if (isValidPassword(password, to_match, num_low, num_high)) {
      part1++;
    }
    if (isValidPasswordPart2(password, to_match, num_low, num_high)) {
      part2++;
    }
    count++;
  }
  input.close();

  cout << "Part1 valid passwords: " << part1 << endl;
  cout << "Part2 valid passwords: " << part2 << endl;
  cout << "Total passwords: " << count << endl;
  return 0;
  
}

bool isValidPassword(const string& password, char to_match, int low, int high) {
  int count = 0;
  for (int i = 0; i < password.size(); i++) {
    if (password[i] == to_match) {
      count++;
    }
  }
  return (count >= low && count <= high);
}

bool isValidPasswordPart2(const string& password, char to_match, int low, int high) {
  int n = password.size();
  bool a = (low-1 < n && password[low-1] == to_match);
  bool b = (high-1 < n && password[high-1] == to_match);
  return (a != b);
}


