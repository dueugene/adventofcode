#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <queue>
#include <set>

using namespace std;

int find_bags(const unordered_map<string,vector<pair<string,int>>> graph, string start);
long long count_bags(const unordered_map<string,vector<pair<string,int>>> graph, string start);

int main() {

  // read the input file, and create a directed graph
  // create a another graph indicating the "reverse" direction
  unordered_map<string,vector<pair<string,int>>> graph;
  unordered_map<string,vector<pair<string,int>>> reverse_graph;
  string line;
  ifstream input;
  input.open("input.txt");
  while (getline(input,line)) {
    istringstream ss(line);
    string a, b, c, d, temp1, temp2;
    // "a" "b" "bags" "contain"
    ss >> a >> b >> temp1 >> temp2;
    int num;
    while (ss >> num) {
      // "c" "d" "bags"
      ss >> c >> d >> temp1;
      reverse_graph[c+d].push_back({a+b, num});
      graph[a+b].push_back({c+d, num});
    }
  }
  input.close();
  // print out the graph for debugging purposes
  for (auto it = reverse_graph.begin(); it != reverse_graph.end(); it++) {
    cout << it->first << " : ";
    for (int i = 0; i < it->second.size(); i++) {
      cout << it->second[i].second << it->second[i].first << " ";
    }
    cout << endl;
  }
  
  cout << "Part 1: "<< find_bags(reverse_graph, "shinygold") << endl;
  cout << "Part 2: "<< count_bags(graph, "shinygold") << endl;
  
  return 0;
}

// find all bags that eventually contain a bag given by start
// performs a breadth first search
int find_bags(const unordered_map<string,vector<pair<string,int>>> graph, string start) {
  queue<string> q; // used to processes bags 
  set<string> visited; // used to indicate which bags have been visited
  q.push(start);
  int result = 0;
  while (!q.empty()) {
    string curr = q.front();
    q.pop();
    auto it = graph.find(curr);
    if (it != graph.end()) {
      for (int i = 0; i < it->second.size(); i++) {
        string next = it->second[i].first;
        if (visited.find(next) == visited.end()) {
          q.push(next);
          visited.insert(next);
          result++;
        }
      } 
    }
  }
  return result;
}

// count all bags, and nested bags containted within starting from start
// this assumes that there is no cycles, or else we will have infinite bags
long long count_bags(const unordered_map<string,vector<pair<string,int>>> graph, string start) {
  queue<pair<string, long long>> q; // used to processes bags
  q.push({start, 1});
  long long result = -1; // the initial starting bag doesnt count
  while (!q.empty()) {
    pair<string, long long> curr = q.front();
    q.pop();
    cout << curr.first << " " << curr.second << endl;
    long long count = curr.second;
    result += count;
    auto it = graph.find(curr.first);
    if (it != graph.end()) {
      for (int i = 0; i < it->second.size(); i++) {
        string next = it->second[i].first;
        int bags = it->second[i].second;
        q.push({next, count*bags});
      } 
    }
  }
  return result;
}


