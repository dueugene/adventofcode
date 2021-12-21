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

struct Food {
  unordered_set<string> ingredients;
  unordered_set<string> known_allergens;
  Food(unordered_set<string> ing_in, unordered_set<string> all_in) : ingredients(ing_in), known_allergens(all_in) {};
};

unordered_set<string> intersection(const unordered_set<string>& a, const unordered_set<string>& b);
  

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
  vector<Food> foods;
  // read the input file
  ifstream input;
  string line;
  input.open(filename);
  while (getline(input, line)) {
    istringstream ss(line);
    string s;
    // process the ingredients
    unordered_set<string> ing;
    while (getline(ss, s, ' ') && s[0] != '(') {
      ing.insert(s);
    }
    // process the allergens
    unordered_set<string> all;
    while (getline(ss, s, ' ')) {
      all.insert(s.substr(0,s.size()-1));
    }

    Food food(ing, all);
    foods.push_back(food);
  }

  // create a map of all allergens and their possible corresponding ingredients
  unordered_map<string, unordered_set<string>> possible_allergens;
  for (auto food : foods) {
    for (auto a : food.known_allergens) {
      if (possible_allergens.find(a) == possible_allergens.end()) {
        possible_allergens[a] = food.ingredients;
      } else {
        possible_allergens[a] = intersection(food.ingredients, possible_allergens[a]);
      }
    }
  }

  // eleminate allergen possiblities by deduction, not sure if this is the best method
  queue<unordered_map<string, unordered_set<string>>::iterator> q;
  list<pair<string, string>> allergen_list;
  for (auto it = possible_allergens.begin(); it != possible_allergens.end(); it++) {
    q.push(it);
  }
  while (!q.empty()) {
    auto it = q.front();
    q.pop();
    if (it->second.size() == 1) {
      // add to identified allergen list
      allergen_list.push_back({it->first, *(it->second.begin())});
      // remove this allergen possibiliy from other foods
      for (auto it2 = possible_allergens.begin(); it2 != possible_allergens.end(); it2++) {
        if (it != it2) {
          it2->second.erase(*(it->second.begin()));
        }
      }
    } else {
      q.push(it);
    }
  }
  
  // print the allergen results
  cout << "Allergens:" << endl;
  for (auto it = allergen_list.begin(); it != allergen_list.end(); it++) {
    cout << it->first << ": " << it->second << endl;
  }

  // part 1, count all the instances of non allergens
  int count = 0;
  for (auto food : foods) {
    count += food.ingredients.size();
    for (auto it = allergen_list.begin(); it != allergen_list.end(); it++) {
      if (food.ingredients.find(it->second) != food.ingredients.end()) {
        count--;
      }
    }
  }
  cout << "Part 1: " << count << endl;

  // part 2
  // sort the allergens list alphbetically
  // note: this comparison operator is not safe for all scenarios, but works for ours
  auto comp = [] (const pair<string, string>& a, const pair<string, string>& b) {
    int i = 0;
    while (a.first[i] == b.first[i]) {
      i++;
    }
    return a.first[i] < b.first[i];
  };
  allergen_list.sort(comp);
  
  string canonical_dangerous_ingredient;
  for (auto it = allergen_list.begin(); it != allergen_list.end(); it++) {
    canonical_dangerous_ingredient += it->second;
    canonical_dangerous_ingredient += ",";
  }
  canonical_dangerous_ingredient.erase(canonical_dangerous_ingredient.size()-1);
  cout << "Part 2: " << canonical_dangerous_ingredient << endl;
  
  return 0;
}

unordered_set<string> intersection(const unordered_set<string>& a, const unordered_set<string>& b) {
  unordered_set<string> result;
  for (auto it = a.begin(); it != a.end(); it++) {
    if (b.find(*it) != b.end()) {
      result.insert(*it);
    }
  }
  return result;
}
