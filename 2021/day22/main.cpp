#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>
#include <list>
#include <memory>

#include "../utils/file_read.cpp"
#include "../utils/hash.cpp"

using namespace std;

// part 1
vector<int> parse_command(string command);
bool within_region(vector<int> limits);
void execute_step(string command, unordered_set<vector<int>, VectorHasher>& on_cubes);

// part 2
struct Cuboid {
    Cuboid(vector<int> limits) {
        x1_ = limits[0];
        x2_ = limits[1];
        y1_ = limits[2];
        y2_ = limits[3];
        z1_ = limits[4];
        z2_ = limits[5];
    };
    unsigned long long volume() {
        unsigned long long result = 1;
        result *= (x2_ - x1_);
        result *= (y2_ - y1_);
        result *= (y2_ - y1_);
        return result;
    };
    int x1_;
    int x2_;
    int y1_;
    int y2_;
    int z1_;
    int z2_;
};

void add_cuboid(list<Cuboid>& cubes, Cuboid B);
void subtract_cuboid(list<Cuboid>& cubes, Cuboid B);
vector<Cuboid> remove(Cuboid a, Cuboid b);
vector<Cuboid> add(Cuboid a, Cuboid b);

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "sample.txt";
    }

    // read the input file
    vector<string> reboot_steps = read_input(filename);
    
    // part 1
    // problem makes reference to aoc 2020 day17, which luckily I did
    // this seems to indicate points in 3D space, which I can store in sparse format
    // I will use vector<int> and a vector hash, as I did in aoc 2020 day17
    unordered_set<vector<int>, VectorHasher> on_cubes;
    for (string step : reboot_steps) {
        execute_step(step, on_cubes);
    }
    cout << "part1: " << on_cubes.size() << endl;

    // part 2
    // evidently using part1 strategy wont work, and I realised this problem is not really that 
    // "sparse"
    // thoughts 
    // store cuboid regions, and when two intersect, we have to split into more cuboids, representing
    // the resultant volume after intersect
    list<Cuboid> cubes;
    for (string step : reboot_steps) {
        vector<int> command = parse_command(step);
        Cuboid B = Cuboid(command);
        if (command[6] == 1) {
            add_cuboid(cubes, B);
        } else {
            subtract_cuboid(cubes, B);
        }
    }
    
    unsigned long long result = 0;
    for (Cuboid B : cubes) {
        result += B.volume();
    }
    cout << "part2: " << result << endl;

    return 0;
}

vector<int> parse_command(string command) {
    // [x1, x2, y1, y2, z1, z2, on/off]
    vector<int> result(7, 0);
    istringstream ss(command);
    string line;
    getline(ss, line, ' ');
    if (line == "on") {
        result[6] = 1;
    }
    int ind = 0;
    while (getline(ss, line, ',')) {
        cout << line << endl;
        int i = 0;
        while (line[i] != '.') {
            i++;
        }
        result[ind] = stoi(line.substr(2, i - 2));
        result[ind+1] = stoi(line.substr(i+2));
        ind += 2;
    }

    return result;
}

bool within_region(vector<int> limits) {
    if (limits[0] < -50) {return false;}
    if (limits[1] > 50) {return false;}
    if (limits[2] < -50) {return false;}
    if (limits[3] > 50) {return false;}
     if (limits[4] < -50) {return false;}
    if (limits[5] > 50) {return false;}
    return true;
}

void execute_step(string command, unordered_set<vector<int>, VectorHasher>& on_cubes) {
    vector<int> limits = parse_command(command);
    if (within_region(limits)) {
        if (limits[6] == 1) {
            // on
            for (int i = limits[0]; i <= limits[1]; i++) {
                for (int j = limits[2]; j <= limits[3]; j++) {
                    for (int k = limits[4]; k <= limits[5]; k++) {
                        vector<int> coord = {i, j, k};
                        on_cubes.insert(coord);
                    }
                }
            }
        } else {
            // off
            for (int i = limits[0]; i <= limits[1]; i++) {
                for (int j = limits[2]; j <= limits[3]; j++) {
                    for (int k = limits[4]; k <= limits[5]; k++) {
                        vector<int> coord = {i, j, k};
                        on_cubes.erase(coord);
                    }
                }
            }
        }
    }

}

void add_cuboid(list<Cuboid>& cubes, Cuboid B) {
    list<Cuboid> update;
    for (Cuboid A : cubes) {
        vector<Cuboid> intersect = add(A, B);

    }
};
void subtract_cuboid(unordered_set<Cuboid>& cubes, Cuboid B);
vector<Cuboid> remove(Cuboid a, Cuboid b);
vector<Cuboid> add(Cuboid a, Cuboid b);