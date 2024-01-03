#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "input_reader.hpp"

using namespace std;

class SpringConfigurator {
   public:
    ulong get_n_configurations2(string pattern, vector<int> springs) {
        int n = pattern.size();
        int m = springs.size();
        springs.insert(springs.begin(), -1);
        pattern.insert(pattern.begin(), 'X');
        cout << pattern << endl;
        for (auto a : springs) {
            cout << a << " ";
        }
        cout << endl;

        vector<vector<ulong>> dp(n + 1, vector<ulong>(m + 1, 0));
        vector<vector<ulong>> dp2(n + 1, vector<ulong>(m + 1, 0));
        // let dp[i+1][j+1] be the number of ways to springs[0..j] into
        // pattern[0..i] if pattern[i] is a '.' let dp2[i+1][j+1] be the number
        // of ways to springs[0..j] into pattern[0..i] if pattern[i] is a '#'
        dp[0][0] = 1;
        for (int i = 1; i <= n; i++) {
            dp[i][0] = pattern[i] != '#' ? dp[i - 1][0] : 0;
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                int spring_size = springs[j];
                char c = pattern[i];
                bool fits = true;
                if (i >= spring_size) {
                    for (int k = 0; k < spring_size; k++) {
                        if (pattern[i - k] == '.') {
                            fits = false;
                        }
                    }
                } else {
                    fits = false;
                }

                switch (c) {
                    case '.':
                        dp[i][j] = dp[i - 1][j] + dp2[i - 1][j];
                        dp2[i][j] = 0;
                        break;
                    case '#':
                        dp[i][j] = 0;
                        if (fits) {
                            dp2[i][j] = dp[i - spring_size][j - 1];
                        } else {
                            dp2[i][j] = 0;
                        }
                        break;
                    case '?':
                        dp[i][j] = dp[i - 1][j] + dp2[i - 1][j];
                        if (fits) {
                            dp2[i][j] = dp[i - spring_size][j - 1];
                        } else {
                            dp2[i][j] = 0;
                        }
                        break;
                }
            }
        }

        //   for (int j = 0; j <= m; j++) {
        //         for (int i = 0; i <= n; i++) {
        //     cout << dp[i][j] << " ";
        //   }
        //   cout << endl;
        // }
        // cout << endl;

        //   for (int j = 0; j <= m; j++) {
        //         for (int i = 0; i <= n; i++) {

        //     cout << dp2[i][j] << " ";
        //   }
        //   cout << endl;
        // }
        // cout << endl;

        ulong result = 0;
        // cout << dp[n][m] << " " << dp2[n][m] << endl;
        return dp[n][m] + dp2[n][m];
    }

    // attempt at improved method
    int get_n_configurations(string &pattern, vector<int> &springs) {
        pat = pattern;
        spr = springs;

        int result = 0;
        pat_ind = 0;
        spr_ind = 0;
        curr_spr_count = 0;
        int n_available = count(pattern.begin(), pattern.end(), '?');
        n_available += count(pattern.begin(), pattern.end(), '#');
        int n_springs = accumulate(springs.begin(), springs.end(), 0);

        recurse2(pat_ind, spr_ind, n_available, n_springs, result);

        return result;
    }

    void recurse2(int pat_ind, int spr_ind, int n_available, int n_springs,
                  int &result) {
        if (n_available < n_springs) {
            return;
        }

        // at each '?'
        // 1. can add a '#'
        //    if creates an invalid pattern, backtrack (when number of # exceeds
        //    allowed springs) else recurse
        // 2. can add a '.'
        //    if creates an invalid pattern, backtrack (when number of # is not
        //    zero and below allowed springs) recurse
        while (pat_ind < pat.size()) {
            if (pat[pat_ind] == '#') {
                // fill up to valid amount of springs
                int avail_springs = spr[spr_ind];
                while (avail_springs--) {
                    if (pat_ind >= pat.size() || pat[pat_ind] == '.') {
                        return;
                    }
                    n_available--;
                    n_springs--;
                    pat_ind++;
                }
                spr_ind++;
                // cout << pat.substr(0, pat_ind) << " " << spr_ind << endl;
                if (pat_ind == pat.size()) {
                    recurse2(pat_ind, spr_ind, n_available, n_springs, result);
                }
                if (pat[pat_ind] == '#') {
                    // invalid pattern
                    return;
                }
                pat_ind++;
            } else if (pat[pat_ind] == '.') {
                pat_ind++;
            } else {
                // '?'
                pat[pat_ind] = '#';
                recurse2(pat_ind, spr_ind, n_available, n_springs, result);
                pat[pat_ind] = '.';
                recurse2(pat_ind + 1, spr_ind, n_available - 1, n_springs,
                         result);
                pat[pat_ind] = '?';
                return;
            }
        }
        if (pat_ind == pat.size()) {
            if (spr_ind == spr.size()) {
                result++;
                // cout << pat << endl;
            }
            return;
        }
        return;
    }

   private:
    bool valid_pattern(string &pattern, vector<int> &springs) {
        vector<int> result;
        int count = 0;
        for (char c : pattern) {
            if (c == '#') {
                count++;
            } else {
                if (count > 0) {
                    result.push_back(count);
                }
                count = 0;
            }
        }
        if (count > 0) {
            result.push_back(count);
        }
        return result == springs;
    }

    void recurse(string &pattern, vector<int> inds, int i, vector<int> &springs,
                 int &result) {
        if (i == inds.size()) {
            if (valid_pattern(pattern, springs)) {
                result++;
            }
            return;
        }
        int ind = inds[i];
        pattern[ind] = '#';
        recurse(pattern, inds, i + 1, springs, result);
        pattern[ind] = '.';
        recurse(pattern, inds, i + 1, springs, result);
        pattern[ind] = '?';
        return;
    }

    string pat;
    int pat_ind;
    vector<int> spr;
    int spr_ind;
    int curr_spr_count;
};

int main(int argc, char **argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "input.txt";
    }

    // read the input file
    vector<string> lines = input_reader::read_as_strings(filename);

    // process pattern, and springs
    vector<string> patterns;
    vector<vector<int>> springss;
    for (auto s : lines) {
        istringstream ss(s);
        string pattern;
        string temp;
        int num;
        vector<int> springs;
        ss >> pattern;
        while (getline(ss, temp, ',')) {
            springs.push_back(stoi(temp));
        }
        patterns.push_back(pattern);
        springss.push_back(springs);
    }

    SpringConfigurator sc;
    // part 1
    long long int part1 = 0;
    for (int i = 0; i < patterns.size(); i++) {
        int res = sc.get_n_configurations2(patterns[i], springss[i]);
        part1 += res;
        cout << res << endl;
        res = sc.get_n_configurations(patterns[i], springss[i]);
        cout << res << endl;
    }
    cout << "part1: " << part1 << endl;

    // part 2
    ulong part2 = 0;
    for (int i = 0; i < patterns.size(); i++) {
        string pat;
        vector<int> spr;
        for (int j = 0; j < 5; j++) {
            pat += patterns[i];
            pat.push_back('?');
            spr.insert(spr.end(), springss[i].begin(), springss[i].end());
        }

        pat.pop_back();
        cout << pat << " ";
        for (auto a : spr) {
            cout << a << " ";
        }
        cout << endl;
        // int res = 0;
        ulong res = sc.get_n_configurations2(pat, spr);
        cout << res << endl;
        // cout << sc.get_n_configurations(pat, spr) << endl;
        part2 += res;
    }
    cout << "part2: " << part2 << endl;

    return 0;
}
