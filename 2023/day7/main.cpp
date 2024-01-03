#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

using ulong = unsigned long long;

enum TYPE{HIGH, ONE_PAIR, TWO_PAIR, THREE_KIND, FULL_HOUSE, FOUR_KIND, FIVE_KIND};
class Hand {
public:
    Hand(const string& input, int bid) : cards_(input), bid_(bid) {
        compute_type();
    }
    bool operator<(const Hand& other) const {
        if (this->type_ == other.type_) {
            for (int i = 0; i < cards_.size(); i++) {
                const char& c = this->cards_[i];
                const char& b = other.cards_[i];
                if (c != b) {
                    return card_ranking_[c] < card_ranking_[b];
                }
            }
        }
        return this->type_ < other.type_;
    }
    int bid() {
        return bid_;
    }

    TYPE type() {
        return type_;
    }

    string cards() {
        return cards_;
    }
    static void set_card_ranking(vector<int> card_ranking) {
        card_ranking_ = card_ranking;
    }

    void recompute_type() {
        unordered_map<char, int> map;
        for (char c : cards_) {
            map[c]++;
        }
        int n_j = map.find('J') == map.end() ? 0 : map['J'];
        map.erase('J');
        cout << cards_ << " " << n_j << " " << type_ << " ";
        type_ = compute_type(map);
        switch (n_j) {
        case 5:
        case 4:
            type_ = FIVE_KIND;
            break;
        case 3:
            if (type_ == ONE_PAIR) {
                type_ = FIVE_KIND;
            } else {
                type_ = FOUR_KIND;
            }
            break;
        case 2:
            if (type_ == THREE_KIND) {
                type_ = FIVE_KIND;
            } else if (type_ == ONE_PAIR) {
                type_ = FOUR_KIND;
            } else {
                type_ = THREE_KIND;
            }
            break;
        case 1:
            if (type_ == THREE_KIND) {
                type_ = FOUR_KIND;
            } else if (type_ == ONE_PAIR) {
                type_ = THREE_KIND;
            } else if (type_ == TWO_PAIR) {
                type_ = FULL_HOUSE;
            } else {
                type_ = TYPE(int(type_) + 1);
            }
            break;
        }
        cout << type_ << endl;
    }
private:
    void compute_type() {
        unordered_map<char, int> map;
        for (char c : cards_) {
            map[c]++;
        }
        type_ = compute_type(map);
    }
    TYPE compute_type(unordered_map<char, int>& map) {
        vector<int> counts;
        for (auto p : map) {
            counts.push_back(p.second);
        }
        sort(counts.begin(), counts.end(), greater<int>());
        if (counts.empty()) return HIGH;
        switch(counts[0]) {
        case 5:
            return FIVE_KIND;
        case 4:
            return FOUR_KIND;
        case 3:
            if (counts.size() >=2 && counts[1] == 2) {
                return FULL_HOUSE;
            }
            return THREE_KIND;
        case 2:
            if (counts.size() >=2 && counts[1] == 2) {
                return TWO_PAIR;
            }
            return ONE_PAIR;
        case 1:
            return HIGH;
        }
        return HIGH;
    }
    TYPE type_;
    int bid_;
    string cards_;
    static vector<int> card_ranking_;
};

vector<int> card_ranking(255);
vector<int> Hand::card_ranking_;

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
    ifstream input;
    input.open(filename);
    vector<Hand> hands;
    string a;
    int num;
    while (input >> a >> num) {
        hands.push_back(Hand(a, num));
    }
    input.close();
    
    ulong part1 = 0;
    card_ranking['A'] = 13;
    card_ranking['K'] = 12;
    card_ranking['Q'] = 11;
    card_ranking['J'] = 10;
    card_ranking['T'] = 9;
    card_ranking['9'] = 8;
    card_ranking['8'] = 7;
    card_ranking['7'] = 6;
    card_ranking['6'] = 5;
    card_ranking['5'] = 4;
    card_ranking['4'] = 3;
    card_ranking['3'] = 2;
    card_ranking['2'] = 1;
    Hand::set_card_ranking(card_ranking);
    sort(hands.begin(), hands.end());
    for (int i = 0; i < hands.size(); i++) {
        part1 += hands[i].bid() * (i+1);
    }

    card_ranking['J'] = 0;
    Hand::set_card_ranking(card_ranking);
    ulong part2 = 0;
    for (auto& h : hands) {
        h.recompute_type();
    }
    sort(hands.begin(), hands.end());
    // for (auto& h : hands) {
    //     h.recompute_type();
    // }
    for (int i = 0; i < hands.size(); i++) {
        // cout << hands[i].cards() << " " << hands[i].type() << endl;
        part2 += hands[i].bid() * (i+1);
    }

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}