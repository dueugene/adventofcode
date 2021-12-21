#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

using namespace std;

int execute_single_instruction(int ind, pair<string, int> ins, int& accumulator);

int main(int argc, char** argv) {
  // this will allow different input files to be passed
  string filename;
  if (argc > 1) {
    filename = argv[1];
  } else {
    // default input
    filename = "input.txt";
  }

  // read the input file, store each instruction in a vector
  vector<pair<string, int>> instructions;
  string line;
  ifstream input;
  input.open(filename);
  while (getline(input,line)) {
    istringstream ss(line);
    string a;
    int num;
    ss >> a >> num;
    instructions.push_back({a, num});    
  }
  input.close();
  
  // print out the instructions for debugging purposes
  /*
  for (auto c : instructions) {
    cout << c.first << " " << c.second << endl;
  }
  */

  // part 1
  int accumulator = 0;
  int n = instructions.size();
  vector<bool> visited(n, false);
  int ind = 0;
  int count = 0;
  while (true) {
    if (visited[ind] == true) {break;}
    visited[ind] = true;
    auto c = instructions[ind];
    ind = execute_single_instruction(ind, c, accumulator);
    count++;
  }
  cout << "Part 1: accumulator: " << accumulator << ", total instructions: " << n << ", instructions in loop: " << count << endl;

  // part 2
  // using bfs, we first accumulate a list of instructions that can reach the last instruction
  // this is an O(n^2) operation
  vector<bool> can_terminate(n, false);
  queue<int> q;
  q.push(n-1);
  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    can_terminate[curr] = true;
    int temp = 0;
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && execute_single_instruction(i, instructions[i], temp) == curr) {
        q.push(i);
      }
    }
  }

  // from part 1, visited contains all instructions that form the loop,
  // we loop through each instruction in visited and see if we can modify it such that it can hit any of can_terminate
  for (int i = 0; i < n; i++) {
    if (visited[i] == true) {
      // check if we can swap a jmp or nop operation
      auto c = instructions[i];
      int temp = 0;
      int next = -1;
      if (c.first == "jmp") {
        c.first = "nop";
        next = execute_single_instruction(i, c, temp);
      } else if (c.first == "nop") {
        c.first = "jmp";
        next = execute_single_instruction(i, c, temp);
      }
      if (next >= 0 && next < n && can_terminate[next]) {
        instructions[i] = c;
        break;
      }
    }
  }
  
  // rerun the "program"
  ind = 0;
  accumulator = 0;
  count = 0;
  while (ind < instructions.size()) {
    auto c = instructions[ind];
    ind = execute_single_instruction(ind, c, accumulator);
    count++;
  }
  cout << "Part 2: accumulator: " << accumulator << ", executed instructions: " << count << endl; 
         
  return 0;
}

// executes a single instruction and return the theoretial location of the next instruction to be executed
// properly accumulates accumulator if instruction requires so
int execute_single_instruction(int ind, pair<string, int> ins, int& accumulator) {
  if (ins.first == "jmp") {
    ind += ins.second;
  } else if (ins.first == "acc") {
    accumulator += ins.second;
    ind++; 
  } else { // nop
    ind++;
  }
  return ind;
}


