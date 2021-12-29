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

int triangle_num(int n);
bool hit_target(int x_vel, int y_vel, int x1, int x2, int y1, int y2);
bool in_target(int x, int y, int x1, int x2, int y1, int y2);

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
    int x1, x2, y1, y2;
    if (filename == "input.txt") {
        x1 = 14;
        x2 = 50;
        y1 = -225;
        y2 = -267;
    } else {
        cout << filename << endl;
        x1 = 20;
        x2 = 30;
        y1 = -5;
        y2 = -10;
    }

    // part 1
    // intuitive thoughts
    // x, and y are independent
    // we can find an initial x, such that we can get into the box
    // when y is in the box range, therefore only need to look at y.
    // if we shoot up with y-vel = YY, we will always end up back at
    // y = 0, with y-vel = -YY
    // next step we will be at y = -YY - 1
    // so we should just be able to find largest YY such that -YY - 1 < y2
    int yy = -(y2 + 1);
    cout << "part1: " << triangle_num(yy) << endl;
    
    // part 2
    // thoughts
    // all shots that make it in 1 step -> trivial
    // find all shots that make it in 2 steps
    // find all shots that make it in 3 steps..
    // and so on.... ?
    // limit will be largest YY from part 1.
    // which is 35511 ish steps...

    // duality? for every shot downwards, there is another shot upwards?
    // I dont think this is necessarily true, as x wont line up
    
    // could bruteforce every possible value..
    // which I will try here
    int result = 0;
    for (int i = y2; i <= yy; i++) {
        for (int j = 0; j <= x2; j++) {
            if (hit_target(j, i, x1, x2, y1, y2)) {
                result++;
            }
        }
    }
    cout << "part2: " << result << endl;

    return 0;
}

int triangle_num(int n) {
    if (n == 1) {return n;}
    return n + triangle_num(n-1);
}

bool hit_target(int x_vel, int y_vel, int x1, int x2, int y1, int y2) {
    int x = 0;
    int y = 0;
    while (x <= x2 && y >= y2) {
        x += x_vel;
        y += y_vel;
        if (in_target(x, y, x1, x2, y1, y2)) {
            return true;
        }
        x_vel--;
        x_vel = x_vel < 0 ? 0 : x_vel;
        y_vel--;
    }
    return false;
}

bool in_target(int x, int y, int x1, int x2, int y1, int y2) {
    if (x >= x1 && x <= x2) {
        if (y >= y2 && y <= y1) {
            return true;
        }
    }
    return false;
}