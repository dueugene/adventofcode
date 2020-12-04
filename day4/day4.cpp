#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

bool is_valid_passport(const unordered_map<string, string>& passport);
bool validate_further(const unordered_map<string, string>& passport);
bool validate_number(const string& word, int digits, int lb, int ub);

int main() {
  string line;
  unordered_map<string, string> passport;
  int count = 0, count2 = 0;
  ifstream input;
  input.open("input.txt");
  while (getline(input,line)) { 
    if (line.size() == 0) {
      // empty line represents begining of a new passport
      if (is_valid_passport(passport)) {
        count++;
        if (validate_further(passport)) {
          count2++;
        }
      }
      passport.clear();
    }
    // process line, and insert fields into passport
    istringstream ss(line);
    string word1;
    while (ss >> word1) {
      // here we know that the "key" is exactly 3 chars long, otherwise we could also search for the delimeter ':'
      passport[word1.substr(0,3)] = word1.substr(4);
    }
  }
  input.close();
  
  cout << "Total valid passports: " << count << endl;
  cout << "Total valid part2 passports: " << count2 << endl;
  return 0;
  
}

// verify if passport is valid or not, by checking the fields based on part1
bool is_valid_passport(const unordered_map<string, string>& passport) {
  // we do not need to confirm if cid is a valid field
  vector<string> fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
  for (int i = 0; i < fields.size(); i++) {
    if (passport.find(fields[i]) == passport.end()) {
      return false;
    }
  }
  return true;
}

bool validate_further(const unordered_map<string, string>& passport) {
  // process byr
  string word = passport.at("byr");
  if (!validate_number(word, 4, 1920, 2002)) {return false;}

  // process iyr
  word = passport.at("iyr");
  if (!validate_number(word, 4, 2010, 2020)) {return false;}
  
  // process eyr
  word = passport.at("eyr");
  if (!validate_number(word, 4, 2020, 2030)) {return false;}
  
  // process hgt
  word = passport.at("hgt");
  int n = word.size();
  string type = word.substr(n-2);
  if (type == "cm") {
    if (!validate_number(word.substr(0,n-2), 3, 150, 193)) {return false;}
  } else if (type == "in") {
    if (!validate_number(word.substr(0,n-2), 2, 59, 76)) {return false;}
  } else {
    return false;
  }

  // process hcl
  word = passport.at("hcl");
  if (word.size() != 7) {return false;}
  if (word[0] != '#') {return false;}
  for (int i = 1; i < word.size(); i++) {
    if (!(word[i] >= '0' && word[i] <= '9') && !(word[i] >= 'a' && word[i] <= 'f')) {return false;}
  }
  
  // process ecl
  word = passport.at("ecl");
  bool flag = false;
  vector<string> valid_ecl = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
  for (int i = 0; i < valid_ecl.size(); i++) {
    flag = flag || (word == valid_ecl[i]);
  }
  if (!flag) {return false;}
  
  // process pid
  word = passport.at("pid");
  if (!validate_number(word, 9, 0, 999999999)) {return false;}
  
  return true;
}

bool validate_number(const string& word, int digits, int lb, int ub) {
  if (word.size() != digits) {return false;}
  int num = stoi(word);
  if (num < lb || num > ub) {return false;}
  return true;
}
