#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <memory>
#include <random>
#include <string>
using namespace std;

namespace BlackJack {
	class Card {
	public:
		enum Suit {SPADE, HEART, CLUD, DIAMOND};
		Card(Suit suit, int value) : suit_(suit), facevalue_(value) {}
		Suit GetSuit() const { return suit_; }
		int GetPoint() const { return facevalue_; }
		void Print() {
			switch (suit_) {
			case Card::SPADE:
				cout << "\u2660"; break;
			case Card::HEART:
				cout << "\u2665"; break;
			case Card::CLUD:
				cout << "\u2663"; break;
			case Card::DIAMOND:
				cout << "\u2666"; break;
			}
			switch (facevalue_) {
			case 1: cout << "A "; break;
			case 2: cout << "2 "; break;
			case 3: cout << "3 "; break;
			case 4: cout << "4 "; break;
			case 5: cout << "5 "; break;
			case 6: cout << "6 "; break;
			case 7: cout << "7 "; break;
			case 8: cout << "8 "; break;
			case 9: cout << "9 "; break;
			case 10: cout << "10 "; break;
			case 11: cout << "J "; break;
			case 12: cout << "Q "; break;
			case 13: cout << "K "; break;
			}
		}
	private:
		Suit suit_;
		int facevalue_;
	};

	class GenericPlayer {
	protected:
		string name_;
		vector<Card*> hands_;
	public:
		GenericPlayer(const string& name) : name_(name) {}

		string GetName() const { return name_; }

		int GetTotal() const {
			int total = 0;
			bool contain_ace = false;
			for (auto& card : hands_) {
				total += card->GetPoint();
				if (card->GetPoint() == 1)
					contain_ace = true;
			}
			if (contain_ace && total <= 11) total += 10;
			return total;
		}

		void AddCard(Card* card) {
			hands_.push_back(card);
		}

		bool IsBurst() {
			return GetTotal() > 21;
		}

		bool IsBlackJack() {
			return GetTotal() == 21;
		}

		void DisplayHand() {
			for (auto card : hands_)
				card->Print();
		}

		virtual bool IsHitting() = 0;
	};

	class Player : public GenericPlayer {
	public:
		Player(const string& name) : GenericPlayer(name) {}
		bool IsHitting() {
			char choice;
			cout << "Your current value is: " <<
				GetTotal() << "\n" <<
				"Do you want to hit? (y/n) \n";
			cin >> choice;
			return (choice == 'y' || choice == 'Y');
		}
	};

	class Dealer : public GenericPlayer {
	public:
		Dealer(const string& name = "Dealer") : GenericPlayer(name) {}
		bool IsHitting() {
			return GetTotal() < 17;
		}
	};

	class Deck {
	private:
		vector<Card*> cards_;
		int dealindex_;
		bool is_faceup_;
	public:
		Deck() {
			is_faceup_ = false;
			CreateCard();
			Shuffle();
		}

		void CreateCard() {
			for (int i = Card::SPADE; i <= Card::DIAMOND; ++i) {
				for (int p = 1; p <= 13; ++p) {
					cards_.push_back(new Card(
									 static_cast<Card::Suit>(i), p));
				}
			}
		}

		void Shuffle() {
			mt19937 eng{ static_cast<unsigned>(
						 chrono::high_resolution_clock::now().
						time_since_epoch().
						count()) };
			shuffle(cards_.begin(), cards_.end(), eng);
		}

		void Deal(GenericPlayer* player) {
			if (cards_.empty()) {
				cout << "No cards left!" << "\n";
				return;
			}
			player->AddCard(cards_.back());
			cards_.pop_back();
		}

		void FlipFace() { is_faceup_ = !is_faceup_; }
	};


	class Game {
	private:
		vector<Player*> players_;
		Dealer* dealer_;
		Deck deck_of_cards_;
	public:
		Game(vector<string>& names) {
			for (string& name : names)
				players_.push_back(new Player(name));
			dealer_ = new Dealer();
		}

		~Game() {
			for (auto& p : players_)
				delete p;
			vector<Player*> tmp;
			players_.swap(tmp);
			delete dealer_;
		}
		
		void Play() {
			for (int i = 0; i < 2; ++i) {
				for (auto player : players_)
					deck_of_cards_.Deal(player);
				deck_of_cards_.Deal(dealer_);
			}
			PrintResult();
		}

		void PrintResult() {
			cout << dealer_->GetName() << " ";
			dealer_->DisplayHand();
			cout << "\n";
			if (dealer_->IsBurst()) {
				cout << "Dealer Burst \n";
				for (auto& player : players_) {
					if (!player->IsBurst())
						cout << player->GetName() << " ";
						player->DisplayHand();
						cout << " Wins. \n";
				}
			}
			else {
				int dealer_total = dealer_->GetTotal();
				for (auto player : players_) {
					if (!player->IsBurst()) {
						int player_total = player->GetTotal();
						if (player->IsBlackJack()) {
							cout << "Congratulations! " << player->GetName() << " ";
							player->DisplayHand();
							cout << " BlackJack!!! \n";
						}
						else if (player_total > dealer_total) {
							cout << player->GetName() << " ";
							player->DisplayHand();
							cout << " Wins. \n";
						}
						else {
							cout << player->GetName() << " ";
							player->DisplayHand();
							cout << " Lose. \n";
						}
					}
				}
			}
		}
	};
}

int main()
{
	using namespace BlackJack;
	char again = 'y';
	vector<string> names = { "Jack", "Marry", "Calvein" };
	while (again != 'n' && again != 'N') {
		cout << "\n-----------------------------Let's go:\n";
		Game game(names);
		game.Play();
		cout << "\nDo you want to play again? (Y/N)\n";
		cin >> again;
	}
	return 0;
}



