#include <iostream>
#include <vector>
#include <memory>
using namespace std;

/*
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
}*/

/*using namespace std;


enum class Color { White, Black };
enum class GameStatus { Active, Forfeit, Resignation, White_Win, Black_Win};

class Piece;
class Spot {
private:
int x_, y_;
Piece* piece_;
public:
Spot(int x, int y) :
x_(x), y_(y), piece_(nullptr) {}
int GetX() const { return x_; }
void SetX(int x) { x_ = x; }
int GetY() const { return y_; }
void SetY(int y) { y_ = y; }
Piece* GetPiece() const { return piece_; }
bool IsAvailable() { return piece_ == nullptr; }
void SetOccupied(Piece& piece) {
if (!IsAvailable())
piece.SetAvailable(false);
piece_ = &piece;
}
void ReleaseSpot() { piece_ = nullptr; }
bool OutOfBound() {
return x_ < 0 || y_ < 0 || x_ >= 7 || y_ >= 7;
}
};

class Piece {
protected:
Color color_;
bool available_;

public:
Piece() {}
Piece(Color color, bool available) :
color_(color), available_(available) {}
bool IsAvailable() { return available_; }
void SetAvailable(bool availablity) { available_ = availablity; }
Color GetColor() { return color_; }
virtual bool IsKing() { return false; }
virtual bool CanMove(Spot start, Spot end) {}
};

class King : public Piece {
public:
King(Color color, bool available) {
color_ = color;
available_ = available;
}

bool IsKing() { return true; }

bool CanMove(Spot start, Spot end) {
if (start.OutOfBound() || end.OutOfBound())
return false;
int x = abs(start.GetX() - end.GetX());
int y = abs(start.GetY() - end.GetY());
return (x + y) == 1;
}
};

class Move {
private:
Player player_;
Spot start_, end_;

public:
Move(Player& player, Spot start, Spot end) :
player_(player), start_(start), end_(end){}
Spot GetStart() { return start_; }
Spot GetEnd() { return end_; }
};

class Board {
private:
vector<vector<Spot> > the_board;
public:
Board() { }
void Reset() {
the_board.resize(8, vector<Spot>(8));
for (int row = 0; row < 8; ++row) {
for (int col = 0; col < 8; ++col)
the_board[row][col] = Spot(row, col);
}
}
};

class Player {
private:
Color side_;
public:
Player() {}
void SelectSide(Color side) { side_ = side; }
Color GetSide() { return side_; }
};

class Game {
private:
Board the_board;
vector<Player> players;
vector<Move> moves;
Color turn;
GameStatus status_;
Game() {
players.resize(2);
players[0].SelectSide(Color::White);
players[1].SelectSide(Color::Black);
the_board.Reset();
vector<Move> tmp;
moves.swap(tmp);
}
~Game() {}
public:
static Game& GetInstance() {
static Game game;
return game;
}

bool IsOver() { return status_ != GameStatus::Active; }
GameStatus GetStatus() { return status_; }
void SetStatus(GameStatus status) { status_ = status; }
bool PlayerMove(Player& player, int startx, int starty,
int endx, int endy) {
Spot start = Spot(startx, starty);
Spot end = Spot(endx, endy);
Move mv = Move(player, start, end);
return MakeMove(player, mv);
}

bool MakeMove(Player& player, Move& move) {
Piece* srcpiece = move.GetStart().GetPiece();
if (srcpiece == nullptr) return false;
if (srcpiece->GetColor() != player.GetSide()) return false;
if (!srcpiece->CanMove(move.GetStart(), move.GetEnd()))
return false;
Piece* despiece = move.GetEnd().GetPiece();
if (despiece != nullptr) {
despiece->SetAvailable(false);
}
moves.push_back(move);
move.GetEnd().SetOccupied(*srcpiece);
move.GetStart().ReleaseSpot();
if (despiece && despiece->IsKing()) {
if (player.GetSide() == Color::White)
SetStatus(GameStatus::Black_Win);
else
SetStatus(GameStatus::White_Win);
}

turn = player.GetSide() == players[0].GetSide() ?
players[1].GetSide() : players[0].GetSide();
}
};*/

using namespace std;

namespace Chess {
	enum class Color { WHITE, BLACK };
	template<typename T>
	class Board {
	private:
		int rows_;
		int cols_;
		vector<T> data_;
	public:
		void Resize(int row, int col) {
			data_.resize(row*col);
			rows_ = row;
			cols_ = col;
		}

		T& At(int row, int col) {
			return data_.at(row*cols_ + col);
		}
		int GetRows() const { return rows_; }
		int GetCols() const { return cols_; }
	};
	class Piece;
	class Spot {
	private:
		int row_;
		int col_;
		Piece* piece_;
	public:
		Spot(int row, int col, Piece* piece = nullptr) :
			row_(row), col_(col), piece_(piece) {}
		int GetRow() { return row_; }
		int GetCol() { return col_; }
		Piece* GetPiece() { return piece_; }
		bool HasPiece() { return piece_ != nullptr; }
		void SetRow(int row) { row_ = row; }
		void SetCol(int col) { col_ = col; }
		void SetPiece(Piece* piece) { piece_ = piece; }
		void ClearPiece() { piece_ = nullptr; }
	};

	class Piece {
	private:
		bool available_;
		Color color_;
	public:
		Piece(Color color) : color_(color), available_(true) {}
		Color GetColor() const { return color_; }
		bool IsAvailable() { return available_; }
		bool IsValidMove(Spot start, Spot end) {
			return start.GetRow() >= 0 && start.GetCol() >= 0 &&
				start.GetRow() < 8 && start.GetCol() < 8 &&
				end.GetRow() >= 0 && end.GetCol() >= 0 &&
				end.GetRow() < 8 && end.GetCol() < 8 && available_;
		}
		void SetKilled() { available_ = false; }
		virtual char PrintPiece() = 0;
		virtual bool IsKing() { return false; }
		virtual bool CanMove(Spot start, Spot end) = 0;
	};

	class King : public Piece {
	public:
		King(Color color) : Piece(color) {}
		bool IsKing() { return true; }
		char PrintPiece() { return 'K'; }
		bool CanMove(Spot start, Spot end) {
			if (!IsValidMove(start, end)) return false;
			int x = abs(end.GetCol() - start.GetCol());
			int y = abs(end.GetRow() - start.GetRow());
			return x + y == 1;
		}
	};

	class Knight : public Piece {
	public:
		Knight(Color color) : Piece(color) {}
		char PrintPiece() { return 'N'; }
		bool CanMove(Spot start, Spot end) {
			if (!IsValidMove(start, end)) return false;
			int x = abs(end.GetCol() - start.GetCol());
			int y = abs(end.GetRow() - start.GetRow());
			return x * y == 2;
		}
	};

	class Rook : public Piece {
	public:
		Rook(Color color) : Piece(color) {}
		char PrintPiece() { return 'R'; }
		bool CanMove(Spot start, Spot end) {
			if (!IsValidMove(start, end)) return false;
			int x = abs(end.GetCol() - start.GetCol());
			int y = abs(end.GetRow() - start.GetRow());
			return x == 0 || y == 0;
		}
	};

	class Queen : public Piece {
	public:
		Queen(Color color) : Piece(color) {}
		char PrintPiece() { return 'Q'; }
		bool CanMove(Spot start, Spot end) {
			if (!IsValidMove(start, end)) return false;
			int x = abs(end.GetCol() - start.GetCol());
			int y = abs(end.GetRow() - start.GetRow());
			return x == y;
		}
	};

	class Pawn : public Piece {
	public:
		Pawn(Color color) : Piece(color) {}
		char PrintPiece() { return 'P'; }
		bool CanMove(Spot start, Spot end) {
			if (!IsValidMove(start, end)) return false;
			int x = abs(end.GetCol() - start.GetCol());
			int y = abs(end.GetRow() - start.GetRow());
			return x == 0 && y == 1;
		}
	};

	class Bishop : public Piece {
	public:
		Bishop(Color color) : Piece(color) {}
		char PrintPiece() { return 'B'; }
		bool CanMove(Spot start, Spot end) {
			if (!IsValidMove(start, end)) return false;
			int x = abs(end.GetCol() - start.GetCol());
			int y = abs(end.GetRow() - start.GetRow());
			return x == y && x != 0 && y != 0;
		}
	};


	class Move {
	private:
		Color side_;
		Spot start_;
		Spot end_;
		Piece* piece_moved;
		Piece* piece_killed;
		bool is_castling;
	public:
		Move(Color side, Spot& start, Spot& end) :
			side_(side), start_(start), end_(end),
			is_castling(false), piece_moved(start.GetPiece()) {}
		Piece* GetPieceMoved() { return piece_moved; }
		void SetKilled() { piece_killed->SetKilled(); }
		Color GetSide() { return side_; }
		Spot GetStart() { return start_; }
		Spot GetEnd() { return end_; }
	};

	class Game {
	private:
		bool is_gameover;
		Color player_turn_;
		Board<Spot*> chess_board;
		vector<Move*> move_played;
	public:
		Game() {
			is_gameover = false;
			player_turn_ = Color::WHITE;
			chess_board.Resize(8, 8);
			chess_board.At(7, 0) = new Spot(7, 0, new Rook(Color::BLACK));
			chess_board.At(7, 1) = new Spot(7, 1, new Knight(Color::BLACK));
			chess_board.At(7, 2) = new Spot(7, 2, new Bishop(Color::BLACK));
			chess_board.At(7, 3) = new Spot(7, 3, new Queen(Color::BLACK));
			chess_board.At(7, 4) = new Spot(7, 4, new King(Color::BLACK));
			chess_board.At(7, 5) = new Spot(7, 5, new Bishop(Color::BLACK));
			chess_board.At(7, 6) = new Spot(7, 6, new Knight(Color::BLACK));
			chess_board.At(7, 7) = new Spot(7, 7, new Rook(Color::BLACK));

			chess_board.At(0, 0) = new Spot(0, 0, new Rook(Color::WHITE));
			chess_board.At(0, 1) = new Spot(0, 1, new Knight(Color::WHITE));
			chess_board.At(0, 2) = new Spot(0, 2, new Bishop(Color::WHITE));
			chess_board.At(0, 3) = new Spot(0, 3, new Queen(Color::WHITE));
			chess_board.At(0, 4) = new Spot(0, 4, new King(Color::WHITE));
			chess_board.At(0, 5) = new Spot(0, 5, new Bishop(Color::WHITE));
			chess_board.At(0, 6) = new Spot(0, 6, new Knight(Color::WHITE));
			chess_board.At(0, 7) = new Spot(0, 7, new Rook(Color::WHITE));
			for (int i = 0; i < chess_board.GetCols(); ++i) {
				chess_board.At(1, i) = new Spot(1, i, new Pawn(Color::WHITE));
				chess_board.At(6, i) = new Spot(6, i, new Pawn(Color::BLACK));
			}
			for (int i = 2; i < chess_board.GetRows() - 2; ++i) {
				for (int j = 0; j < chess_board.GetCols(); ++j)
					chess_board.At(i, j) = new Spot(i, j);
			}
		}

		~Game() {
			for (int i = 0; i < move_played.size(); ++i)
				delete move_played[i];
			move_played.swap(vector<Move*>());
		}

		void Run() {
			do {
				Print();

				cout << "Please input the move start position and end position\n";
				int xs, xe, ys, ye;
				cin >> xs >> xe >> ys >> ye;
				if (PlayerMove(player_turn_, xs, xe, ys, ye))
					AlternateTurn();
			} while (!is_gameover);
			Print();
		}

		void AlternateTurn() {
			player_turn_ = (player_turn_ == Color::WHITE ?
				Color::BLACK : Color::WHITE);
		}

		bool PlayerMove(Color side, int xs, int xe, int ys, int ye) {
			Move mv = Move(side, *chess_board.At(xs, xe),
				*chess_board.At(ys, ye));
			if (mv.GetPieceMoved()->GetColor() != mv.GetSide() ||
				(mv.GetEnd().HasPiece() &&
					mv.GetPieceMoved()->GetColor() ==
					mv.GetEnd().GetPiece()->GetColor())) {
				cout << "Invalid Move! \n";
				return false;
			}
			if (mv.GetEnd().HasPiece()) {
				chess_board.At(ys, ye)->GetPiece()->SetKilled();
			}
			move_played.push_back(&mv);
			chess_board.At(ys, ye)->SetPiece(chess_board.At(xs, xe)->GetPiece());
			chess_board.At(xs, xe)->ClearPiece();
			return true;
		}

		void Print() {
			for (int row = 0; row < chess_board.GetRows(); ++row) {
				for (int col = 0; col < chess_board.GetCols(); ++col) {
					if (chess_board.At(row, col)->HasPiece()) {
						if (chess_board.At(row, col)->GetPiece()->GetColor() == Color::WHITE)
							cout << "W";
						else
							cout << "B";
						cout << chess_board.At(row, col)->GetPiece()->PrintPiece() << " ";
					}
					else
						cout << " . ";
				}
				cout << "\n";
			}
		}
	};
}
