#include <vector>
#include <iostream>
class Bingo {

public:

    Bingo(std::vector<std::vector<int>> board_in);
        
    int get_score() const;

    bool call_number(int num);
    
    void print() const;

private:
    int board_size_;
    std::vector<std::vector<int>> board_;
    std::vector<std::vector<bool>> marked_;
    std::vector<int> called_nums_;

    bool is_a_bingo() const;
  
};

Bingo::Bingo(std::vector<std::vector<int>> board_in){
    board_ = board_in;
    board_size_ = board_.size();
    if (board_size_ < 1) {
        // uh oh
    }
    if (board_size_ != board_[0].size()) {
        // uh oh
    }
    marked_ = std::vector<std::vector<bool>>(board_size_, std::vector<bool>(board_size_, false));
}

bool Bingo::call_number(int num) {
    called_nums_.push_back(num);

    for (int i = 0; i < board_size_; i++) {
        for (int j = 0; j < board_size_; j++) {
            if (marked_[i][j] == false && board_[i][j] == num) {
                marked_[i][j] = true;
                return is_a_bingo();
            }
        }   
    }
    // future impovement, map of num -> index, O(1) lookup
    
    return false;
}

int Bingo::get_score() const{
    int score = 0;
    for (int i = 0; i < board_size_; i++) {
        for (int j = 0; j < board_size_; j++) {
            if (marked_[i][j] == false) {
                score += board_[i][j];
            }
        }   
    }
    // assumes most recent call was the winning call
    score *= called_nums_[called_nums_.size() - 1];
    return score;

}

bool Bingo::is_a_bingo() const {

    for (int i = 0; i < board_size_; i++) {
        bool all_marked = true;
        for (int j = 0; j < board_size_; j++) {
            if (marked_[i][j] == false) {
                all_marked = false;
                break;
            }
        }
        if (all_marked) {return true;}
    }

    for (int j = 0; j < board_size_; j++) {
        bool all_marked = true;
        for (int i = 0; i < board_size_; i++) {
            if (marked_[i][j] == false) {
                all_marked = false;
                break;
            }
        }
        if (all_marked) {return true;}
    }
    
    return false;
}

void Bingo::print() const {
    for (int i = 0; i < board_size_; i++) {
        for (int j = 0; j < board_size_; j++) {
            std::cout << board_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}