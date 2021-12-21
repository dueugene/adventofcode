#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

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
    vector<int> meassures;
    int c;
    input.open(filename);
    while (input >> c) {
        meassures.push_back(c);
    }
    input.close();

    // part 1
    int count = 0;
    for (int i = 1; i < meassures.size(); i++) {
        if (meassures[i] > meassures[i-1]) {
            count++;
        }
    }
    cout << "Number of times meassurement increased: " << count << endl;

    // part 2
    count = 0;
    for (int i = 3; i < meassures.size(); i++) {
        if (meassures[i] > meassures[i-3]) {
            count++;
        }
    }
    cout << "Sliding sum meassurement increased: " << count << endl;
    return 0;
}