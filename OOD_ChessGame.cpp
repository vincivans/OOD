#include <iostream>
#include <vector>
#include <memory>
using namespace std;

namespace Chess {
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
        
        int GetRow() const { return row_; }
        int GetCol() const { return col_; }
        T& At(int row, int col) {
            return data_.at(row*col_+col);
        }
    };
    
    class Piece {
    protected:
        char color_;
    public:
        Piece(char color): color_(color) {}
        virtual ~Piece() {}
        char GetColor() const { return color_; }
        virtual char GetPiece() const { return ' '; }
        bool IsValidMove(int startx, int starty, int endx, int endy) {
            return startx>=0 && starty>=0 && startx<8 && starty<8 &&
                   endx>=0 && endy>=0 && endx<8 && endy<8;
        }
        
        virtual bool CanMove(int startx, int starty, int endx, int endy) { return false; };
        virtual bool IsKing() { return false; }
    };
    
    class Rook : public Piece {
    public:
        Rook(char color) : Piece(color) {}
        char GetPiece() const { return 'R'; }
        bool CanMove(int startx, int starty, int endx, int endy) {
            if(!IsValidMove(startx, starty, endx, endy)) return false;
            int x = abs(endx - startx);
            int y = abs(endy - starty);
            return x==0 || y==0;
        }
    };
    
    class Knight : public Piece {
    public:
        Knight(char color): Piece(color) {}
        char GetPiece() const { return 'N'; }
        bool CanMove(int startx, int starty, int endx, int endy) {
            if(!IsValidMove(startx, starty, endx, endy)) return false;
            int x = abs(endx - startx);
            int y = abs(endy - starty);
            return x*y == 2;
        }
    };
    
    class King : public Piece {
    public:
        King(char color): Piece(color) {}
        char GetPiece() const { return 'K'; }
        bool CanMove(int startx, int starty, int endx, int endy) {
            if(!IsValidMove(startx, starty, endx, endy)) return false;
            int x = abs(endx - startx);
            int y = abs(endy - starty);
            return x + y == 1;
        }
        bool IsKing() { return true; }
    };
    
    class Bishop : public Piece {
    public:
        Bishop(char color): Piece(color) {}
        char GetPiece() const { return 'B'; }
        bool CanMove(int startx, int starty, int endx, int endy) {
            if(!IsValidMove(startx, starty, endx, endy)) return false;
            int x = abs(endx - startx);
            int y = abs(endy - starty);
            return x==y && x!=0 && y!=0;
        }
    };
    
    class Queen : public Piece {
    public:
        Queen(char color) : Piece(color) {}
        char GetPiece() const { return 'Q'; }
        bool CanMove(int startx, int starty, int endx, int endy) {
            if(!IsValidMove(startx, starty, endx, endy)) return false;
            int x = abs(endx - startx);
            int y = abs(endy - starty);
            return x == y;
        }
    };
    
    class Pawn : public Piece {
    public:
        Pawn(char color) : Piece(color) {}
        char GetPiece() const { return 'P'; }
        bool CanMove(int startx, int starty, int endx, int endy) {
            if(!IsValidMove(startx, starty, endx, endy)) return false;
            int x = abs(endx - startx);
            int y = abs(endy - starty);
            return x==1 && y==0;
        }
    };
    
    
    class Game {
    private:
        bool is_gameover_;
        char player_turn_;
        Board<unique_ptr<Piece> > chess_board;
    public:
        Game() {
            is_gameover_ = false;
            player_turn_ = 'W';
            chess_board.Reset(8, 8);
            for(int i = 0; i < chess_board.GetCol(); ++i) {
                chess_board.At(1, i) = make_unique<Pawn>('W');
                chess_board.At(6, i) = make_unique<Pawn>('B');
                for(int j = 2; j <= 5; ++j)
                    chess_board.At(j, i) = make_unique<Piece>(' ');
            }
            chess_board.At(7, 0) = make_unique<Rook>('B');
            chess_board.At(7, 1) = make_unique<Knight>('B');
            chess_board.At(7, 2) = make_unique<Bishop>('B');
            chess_board.At(7, 3) = make_unique<Queen>('B');
            chess_board.At(7, 4) = make_unique<King>('B');
            chess_board.At(7, 5) = make_unique<Bishop>('B');
            chess_board.At(7, 6) = make_unique<Knight>('B');
            chess_board.At(7, 7) = make_unique<Rook>('B');
            
            chess_board.At(0, 0) = make_unique<Rook>('W');
            chess_board.At(0, 1) = make_unique<Knight>('W');
            chess_board.At(0, 2) = make_unique<Bishop>('W');
            chess_board.At(0, 3) = make_unique<Queen>('W');
            chess_board.At(0, 4) = make_unique<King>('W');
            chess_board.At(0, 5) = make_unique<Bishop>('W');
            chess_board.At(0, 6) = make_unique<Knight>('W');
            chess_board.At(0, 7) = make_unique<Rook>('W');
        }
        ~Game() {}
        
        void Run() {
            do {
                GetNextMove();
                AlternateTurn();
            }while(!is_gameover_);
            Print();
        }
        
        void AlternateTurn() {
            player_turn_ = (player_turn_=='W' ? 'B' : 'W');
        }
        
        void GetNextMove() {
            bool valid_move = false;
            do {
                Print();
                
                cout << player_turn_ << "'s Move From: ";
                int startx = 0, starty = 0;
                cin >> startx >> starty;
                
                int endx = 0, endy = 0;
                cin >> endx >> endy;
                cout << " To: \n";
                bool can_move = chess_board.At(startx, starty)->CanMove(startx, starty, endx, endy);
                if(chess_board.At(startx, starty)->GetColor()!= player_turn_ || !can_move) {
                    cout << "Invalid Move \n";
                }
                else if(chess_board.At(endx, endx)->IsKing()){
                    is_gameover_ = true;
                    cout << player_turn_ << " win! \n";
                }
                else {
                    valid_move = true;
                    chess_board.At(endx, endy) = move(chess_board.At(startx, starty));
                    chess_board.At(startx, starty) = make_unique<Piece>(' ');
                }
            }while(!valid_move);
        }
        
        void Print() {
            for(int row = 0; row < chess_board.GetRow(); ++row) {
                for(int col = 0; col < chess_board.GetCol(); ++col) {
                    if(chess_board.At(row, col)->GetPiece() != ' ')
                        cout << chess_board.At(row, col)->GetColor()
                             << chess_board.At(row, col)->GetPiece() << " ";
                    else
                        cout << ".  ";
                }
                cout << "\n";
            }
            cout << "\n";
        }
    };
}
