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

using namespace std;

// snailfish numbers seem to have a binary stucture
// I will set it up here, and see if its useful
class SnailFishNum : public std::enable_shared_from_this<SnailFishNum>{
public:
    SnailFishNum(int n) : val_(n), left_(nullptr), right_(nullptr), parent_(nullptr)
    {};

    SnailFishNum(shared_ptr<SnailFishNum> left, shared_ptr<SnailFishNum> right)
     : val_(-1), left_(left), right_(right), parent_(nullptr) {};

    void print() {
        if (left_ == nullptr) {
            cout << val_;
        } else {
            cout << '[';
            left_->print();
            cout << ',';
            right_->print();
            cout << ']';
        }
    };
    
    int magnitude();

    friend shared_ptr<SnailFishNum> operator+(shared_ptr<SnailFishNum> a, shared_ptr<SnailFishNum> b);
    static shared_ptr<SnailFishNum> string_to_snailfishnum(string num, int& ind);

private:
    int val_;
    shared_ptr<SnailFishNum> left_;
    shared_ptr<SnailFishNum> right_;
    shared_ptr<SnailFishNum> parent_;
    void reduce();
    void split();
    void post_order_leafs(vector<pair<shared_ptr<SnailFishNum>, int>>& nodes, int depth);
};

shared_ptr<SnailFishNum> operator+(shared_ptr<SnailFishNum> a, shared_ptr<SnailFishNum> b) {
    if (a == nullptr) {
        return b;
    }
    shared_ptr<SnailFishNum> result = make_shared<SnailFishNum>(a, b);
    result->left_->parent_ = result;
    result->right_->parent_ = result;
    result->reduce();
    return result;
}

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "sample2.txt";
    }

    // read the input file
    vector<string> nums = read_input(filename);
    shared_ptr<SnailFishNum> number;
    for (auto num : nums) {
        cout << num << endl;
        int ind = 0;
        shared_ptr<SnailFishNum> result = SnailFishNum::string_to_snailfishnum(num, ind);
        number = number + result;
    }
    
    // part 1
    cout << "part1: " << number->magnitude() << endl;
    number->print();
    cout << endl;
    // part 2
    // bruteforce combination
    int result = 0;
    for (int i = 0; i < nums.size(); i++) {
        for (int j = i + 1; j < nums.size(); j++) {
            // since I didnt do deep copy, I have to do this
            {
                int ind = 0;
                shared_ptr<SnailFishNum> a = SnailFishNum::string_to_snailfishnum(nums[i], ind);
                ind = 0;
                shared_ptr<SnailFishNum> b = SnailFishNum::string_to_snailfishnum(nums[j], ind);
                shared_ptr<SnailFishNum> temp = a + b;
                result = max(result, temp->magnitude());
            }
            {
                int ind = 0;
                shared_ptr<SnailFishNum> a = SnailFishNum::string_to_snailfishnum(nums[i], ind);
                ind = 0;
                shared_ptr<SnailFishNum> b = SnailFishNum::string_to_snailfishnum(nums[j], ind);
                shared_ptr<SnailFishNum> temp = b + a;
                result = max(result, temp->magnitude());
            }
        }
    }
    cout << "part2: " << result << endl;

    return 0;
}

shared_ptr<SnailFishNum> SnailFishNum::string_to_snailfishnum(string num, int& ind) {
    shared_ptr<SnailFishNum> result = make_shared<SnailFishNum>(-1);
    if (num[ind] == '[') {
        ind++;
        result->left_ = string_to_snailfishnum(num, ind);
        result->left_->parent_ = result;
        ind++;
        result->right_ = string_to_snailfishnum(num, ind);
        result->right_->parent_ = result;
        ind++;
    } else {
        result->val_ = num[ind] - '0';
        ind++;
    }
    return result;
}

void SnailFishNum::reduce() {
    // post order traversal to procude leafs, and depth
    bool reducing = true;
    while (reducing) {
        reducing = false;
        vector<pair<shared_ptr<SnailFishNum>, int>> nodes;
        post_order_leafs(nodes, 0);
        for (int i = 0; i < nodes.size(); i++) {
            auto p = nodes[i];
            if (p.second > 4) {
                // explode parent
                auto parent = p.first->parent_;
                if (i - 1 >= 0) {
                    nodes[i-1].first->val_ += parent->left_->val_;
                }
                if (i + 2 < nodes.size()) {
                    nodes[i+2].first->val_ += parent->right_->val_;
                }
                parent->left_ = nullptr;
                parent->right_ = nullptr;
                parent->val_ = 0;
                reducing = true;
                break;
            }
        }

        if (reducing == false) {
            nodes.clear();
            post_order_leafs(nodes, 0);
            for (int i = 0; i < nodes.size(); i++) {
                auto p = nodes[i];
                if (p.first->val_ >= 10) {
                    p.first->split();
                    reducing = true;
                    break;
                }
            }
        }
    }

};

void SnailFishNum::split() {
    int val_left = val_/2;
    int val_right = val_left + (val_ % 2 ? 1 : 0);
    left_ = make_shared<SnailFishNum>(val_left);
    right_ = make_shared<SnailFishNum>(val_right);
    left_->parent_ = shared_from_this();
    right_->parent_ = shared_from_this();
    val_ = -1;
}

void SnailFishNum::post_order_leafs(vector<pair<shared_ptr<SnailFishNum>, int>>& nodes, int depth) {
    if (left_ != nullptr) {
        left_->post_order_leafs(nodes, depth+1);
        right_->post_order_leafs(nodes, depth+1);
    } else {
        nodes.push_back({shared_from_this(), depth});
    }

};

int SnailFishNum::magnitude() {
    int result = 0;
    if (left_ != nullptr) {
        result += 3*left_->magnitude();
        result += 2*right_->magnitude();
    } else {
        result = val_;
    }
    return result;;
}