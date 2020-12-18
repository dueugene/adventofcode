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
#include <stack>

using namespace std;

long long solve_math_question(const string& s, int& ind);
long long solve_advanced_math_question(const string& s, int& ind);

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
  vector<string> math_questions;
  input.open(filename);
  while (getline(input, line)) {
    math_questions.push_back(line);
  }
  input.close();

  // part 1
  long long ans = 0;
  for (string s : math_questions) {
    int ind = 0;
    ans += solve_math_question(s, ind);
  }
  cout << "Part 1: " << ans << endl;

  // part 2
  ans = 0;
  int i = 0;
  for (string s : math_questions) {
    int ind = 0;
    ans += solve_advanced_math_question(s, ind);
    ind = 0;
    cout << i << ": " << solve_advanced_math_question(s, ind) << endl;
    i++;
  }
  cout << "Part 2: " << ans << endl;
  
  return 0;
}

// from the start indice, solve the math quesiton specified by in, until the end of in, or a ) is reached, whichever comes first
// this only works if the in string is valid
long long solve_math_question(const string& s, int& ind) {
  long long prev = 0;
  char next_op = '+';
  while (ind < s.size() && s[ind] != ')') {
    long long next;
    if (s[ind] == '(') {
      ind++;
      next = solve_math_question(s, ind);
      ind++;
    } else {
      // luckily for this example the number is between 0 - 9
      next = s[ind] - '0';
      ind++;
    }
    // apply the operator
    if (next_op == '+') {
      prev += next;
    } else {
      prev *= next;
    }
    
    if (ind == s.size() || s[ind] == ')') {break;}

    // find the next operator
    // space
    ind++;
    next_op = s[ind];
    //space
    ind++;
    ind++;
  }

  return prev;
}

long long solve_advanced_math_question(const string& s, int& ind) {
  stack<long long> to_mult;
  long long prev = 0;
  char next_op = '+';
  while (ind < s.size() && s[ind] != ')') {
    long long next;
    if (s[ind] == '(') {
      ind++;
      next = solve_advanced_math_question(s, ind);
      ind++;
    } else {
      // luckily for this example the number is between 0 - 9
      next = s[ind] - '0';
      ind++;
    }
    // apply the operator
    if (next_op == '+') {
      prev += next;
    } else {
      to_mult.push(prev);
      prev = next;
    }
    
    if (ind == s.size() || s[ind] == ')') {break;}

    // find the next operator
    // space
    ind++;
    next_op = s[ind];
    //space
    ind++;
    ind++;
  }

  while (!to_mult.empty()) {
    prev *= to_mult.top();
    to_mult.pop();
  }

  return prev;
}
