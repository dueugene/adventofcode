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

using namespace std;

vector<int> process_tickets(string line);
bool check_for_valid_numbers(const unordered_map<string, vector<int>>& ticket_check, const vector<int>& tickets, int& sum);
vector<unsigned int> validate_ticket(const unordered_map<string, vector<int>>& ticket_check, const vector<int>& tickets);

int main(int argc, char** argv) {
  // this will allow different input files to be passed
  string filename;
  if (argc > 1) {
    filename = argv[1];
  } else {
    // default input
    filename = "input.txt";
  }

  unordered_map<string, vector<int>> ticket_check;
  // read the input file
  // this input file is a little complicated, so we will process in sections
  ifstream input;
  string line;
  input.open(filename);
  getline(input, line);
  while (!line.empty()) {
    istringstream ss(line);
    string a, b, c, d, e, f;
    getline(ss, a, ':');
    getline(ss, b, '-');
    getline(ss, c, ' ');
    getline(ss, d, ' ');
    getline(ss, e, '-');
    getline(ss, f);
    vector<int> temp = {stoi(b), stoi(c), stoi(e), stoi(f)};
    ticket_check[a] = temp;
    getline(input, line);
  }

  getline(input, line); // "your ticket"
  getline(input, line);
  vector<int> my_tickets = process_tickets(line);

  getline(input, line); // "nearby tickets"
  getline(input, line);
  vector<vector<int>> other_tickets;
  while (getline(input, line)) {
    other_tickets.push_back(process_tickets(line));
  }
  input.close();

  // part one
  // to prepare for part two, we indicate which rows to delete
  set<int, greater<int>> to_delete;
  int ans = 0;
  for (int i = 0; i < other_tickets.size(); i++) {
    int invalid_sum;
    if (!check_for_valid_numbers(ticket_check, other_tickets[i], invalid_sum)) {
      to_delete.insert(i);
      ans += invalid_sum;
    }
  }
  cout << "Part 1: " << ans << endl;

  // part 2
  // remove invalid tickets first
  // we go from highest ind to lowest ind, to avoid errors due to indices shifting
  for (auto it = to_delete.begin(); it != to_delete.end(); it++) {
    other_tickets.erase(other_tickets.begin() + *it);
  }

  // perform a check that we have removed every invalid ticket
  for (int i = 0; i < other_tickets.size(); i++) {
    int temp;
    if (!check_for_valid_numbers(ticket_check, other_tickets[i], temp)) {
      cout << "error" << endl;
    }
  }

  // set a mapping from number to ticket item, set to ones
  vector<unsigned int> mapping(ticket_check.size(), ~0);
  // insert our ticket into the other tickets
  other_tickets.push_back(my_tickets);
  for (int i = 0; i < other_tickets.size(); i++) {
    vector<unsigned int> valids = validate_ticket(ticket_check, other_tickets[i]);
    for (int j = 0; j < mapping.size(); j++) {
      mapping[j] &= valids[j];
    }
  }

  // different numbers can be valid for multiple fields, therefore we must elminate them 1 by 1
  // this method is probably not the most efficient
  deque<vector<unsigned int>::iterator> q;
  vector<int> mapping_2(ticket_check.size(), 0); // this maps the ticket_check iterator pos to the correct ticket number position
  for (auto it = mapping.begin(); it != mapping.end(); it++) {
    q.push_back(it);
  }
  while (!q.empty()) {
    auto it = q.front();
    q.pop_front();
    // check if its a power of 2, which means only one possibility
    if ((*it & (*it - 1)) == 0) {
      for (auto it2 = q.begin(); it2 != q.end(); it2++) {
        *(*it2) &= ~(*it);
      }
    } else {
      q.push_back(it);
    }
  }
  for (int j = 0; j < mapping.size(); j++) {
    cout << bitset<32> (mapping[j]) << endl;
  }

  // calculate the final answer
  int count = 0;
  long long ans2 = 1;
  for (auto it = ticket_check.begin(); it != ticket_check.end(); it++) {
    // cout << it->first << endl;
    // find the corresponding ticket number position
    if (it->first.substr(0,9) == "departure") {
      int ind = 0;
      for (int i = 0; i < mapping.size(); i++) {
        if (mapping[i] == 1U << count) {
          ind = i;
          break;
        }
      }
      ans2 *= my_tickets[ind];
    }
    count++;
  }

  cout << "Part 2: " << ans2 << endl;
  return 0;
}

// indicates which numbers in the ticket can belong to which field
vector<unsigned int> validate_ticket(const unordered_map<string, vector<int>>& ticket_check, const vector<int>& tickets) {
  vector<unsigned int> result(tickets.size(), 0);
  for (int i = 0; i < tickets.size(); i++) {
    int count = 0;
    for (auto it = ticket_check.begin(); it != ticket_check.end(); it++) {
      if (tickets[i] >= it->second[0] && tickets[i] <= it->second[1]) {
        // set bit to 1
        result[i] |= 1U << count;
      } else if (tickets[i] >= it->second[2] && tickets[i] <= it->second[3]) {
        // set bit to 1
        result[i] |= 1U << count;
      } else {
        // do nothing
      }
      count++;
    }
  }
  return result;
}

vector<int> process_tickets(string line) {
  istringstream ss(line);
  vector<int> result;
  string temp;
  while (getline(ss, temp, ',')) {
    result.push_back(stoi(temp));
  }
  return result;
}

bool check_for_valid_numbers(const unordered_map<string, vector<int>>& ticket_check, const vector<int>& tickets, int& sum) {
  bool passed = true;
  sum = 0;
  for (int i = 0; i < tickets.size(); i++) {
    bool local_passed = false;
    for (auto it = ticket_check.begin(); it != ticket_check.end(); it++) {
      if (tickets[i] >= it->second[0] && tickets[i] <= it->second[1]) {
        local_passed = true;
        break;
      } else if (tickets[i] >= it->second[2] && tickets[i] <= it->second[3]) {
        local_passed = true;
        break;
      }
    }
    if (local_passed == false) {
      passed = false;
      sum += tickets[i];
    }
  }
  return passed;
}
