#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>

#include "../utils/file_read.cpp"

using namespace std;

struct Player {
    int pos = 0;
    unsigned long long wins = 0;
    int cur_score = 0;
};

void simulate_game(const unordered_map<int, int>& dice_possibilities, Player& p1, Player& p2, bool p1_turn, unsigned long long count = 1);

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "sample.txt";
    }

    // I'm a little lazy so no read imputfile
    int p1_init, p2_init;
    if (filename == "input.txt") {
        p1_init = 3;
        p2_init = 7;
    } else {
        p1_init = 4;
        p2_init = 8;
    }

    // part 1
    int p1_score = 0;
    int p2_score = 0;
    int p1 = p1_init;
    int p2 = p2_init;
    int dice = 0;
    while (p1_score < 1000 && p2_score < 1000) {
        int roll = 0;
        for (int i = 0; i < 3; i++) {
            dice++;
            roll += dice;
        }
        p1 = (p1 + roll - 1) % 10 + 1;
        p1_score += p1;
        if (p1_score >= 1000) {break;}
        roll = 0;
        for (int i = 0; i < 3; i++) {
            dice++;
            roll += dice;
        }
        p2 = (p2 + roll - 1) % 10 + 1;
        p2_score += p2;
    }
    cout << p1_score << ", " << p2_score << ", " << dice << endl;
    cout << "part1: " << min(p1_score, p2_score) * dice << endl;
    // part 2
    unordered_map<int, int> dice_possibilities;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            for (int k = 1; k <= 3; k++) {
                dice_possibilities[i+j+k]++;
            }
        }
    }
    for (auto p : dice_possibilities) {
        cout << p.first << ": " << p.second << endl;
    }
    Player a, b;
    a.pos = p1_init;
    b.pos = p2_init;
    simulate_game(dice_possibilities, a, b, true);
    int result = 0;
    cout << a.wins << ", " << b.wins << endl;

    cout << "part2: " << result << endl;

    return 0;
}

void simulate_game(const unordered_map<int, int>& dice_possibilities, Player& p1, Player& p2, bool p1_turn, unsigned long long count) {
    if (p1.cur_score >= 21 || p2.cur_score >= 21) {
        if (p1.cur_score >= 21){ 
            p1.wins += count;

        } else {
            p2.wins += count;
        }
        return;
    }

    Player* p;
    if (p1_turn) {
        p = &p1;
    } else {
        p = &p2;
    }
    for (auto dice : dice_possibilities) {
        int prev_pos = p->pos;
        p->pos = (p->pos + dice.first - 1) % 10 + 1;
        p->cur_score += p->pos;
        simulate_game(dice_possibilities, p1, p2, !p1_turn, count * dice.second);
        p->cur_score -= p->pos;
        p->pos = prev_pos;
    }
}