//review
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;
template<typename T>
class Board {
private:
    int row_;
    int col_;
    vector<T> data_;
    
public:
    void Reset(int row, int col) {
        data_.resize(row*col);
        row_ = row;
        col_ = col;
    }
    
    int GetRows() const { return row_; }
    int GetCols() const { return col_; }
    
    T& At(int row, int col) {
        return data_.at(row*col_ + col);
    }
};


class Cell {
public:
    char value_ = '0';
    bool is_covered = true;
};

class MineField {
private:
    Board<Cell> field;
    bool is_gameover = false;
    int count_steps = 0;
    
public:
    MineField(int row, int col, int numofmines) {
        field.Reset(row, col);
        int mines_placed = 0;
        while(mines_placed < numofmines) {
            int r = rand() % row;
            int c = rand() % col;
            if(field.At(r,c).value_ == 'M') continue;
            ++mines_placed;
            for(int i = max(0, r-1); i <= min(r+1, row-1); ++i) {
                for(int j = max(0,c-1); j <= min(c+1, col-1); ++j) {
                    if(i==r && j==c) field.At(i,j).value_ = 'M';
                    else if(field.At(i,j).value_ != 'M')
                        field.At(i,j).value_ += 1;
                }
            }
        }
    }
    
    bool IsValid(int row, int col) {
        return row>=0 && row<field.GetRows() &&
        col>=0 && col<field.GetCols() &&
        field.At(row, col).is_covered;
    }
    
    bool IsClick(int row, int col) {
        if(!IsValid(row, col)) return false;
        if(field.At(row, col).value_=='M') {
            cout << "Game Over!!! You Lose!!!" << endl;
            is_gameover = true;
            return true;
        }
        ++count_steps;
        if(count_steps == field.GetRows()*field.GetCols()) {
            cout << "Congratulations, You Win" << endl;
            is_gameover = true;
            return true;
        }
        if(field.At(row, col).value_ != '0') return false;
        field.At(row, col).is_covered = false;
        IsClick(row-1, col);
        IsClick(row+1, col);
        IsClick(row, col-1);
        IsClick(row, col+1);
        IsClick(row-1, col-1);
        IsClick(row+1, col+1);
        IsClick(row-1, col+1);
        IsClick(row+1, col-1);
        return false;
    }
    
    void Print(bool isshow) {
        for(int i = 0; i < field.GetRows(); ++i) {
            for(int j = 0; j < field.GetCols(); ++j) {
                if(!field.At(i, j).is_covered || isshow)
                    cout << field.At(i, j).value_ << " ";
                else
                    cout << ". ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    
};
