#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <memory>
using namespace std;

/*
namespace VendingMachine {
	class Payment {
	protected:
		time_t transaction_time_;
		int amount_;
		bool is_payed;
	public:
		static Payment* ChoosePayment(int choice);
		virtual bool Transaction(int amount) = 0;
	};

	class CardPayment : public Payment {
	public:
		string name_on_card_;
		bool Transaction(int amount) {
			transaction_time_ = time(0);
			tm now;
			localtime_s(&now, &transaction_time_);
			cout <<"Payed at: " << 
				(now.tm_year + 1900) << "-" <<
				(now.tm_mon + 1) << "-" <<
				now.tm_mday << "\n";
			is_payed = true;
			return is_payed;
		}
	};

	class CashPayment : public Payment {
	public:
		int cash_value_;
		bool Transaction(int amount) {
			is_payed = (amount >= amount_);
			transaction_time_ = time(0);
			tm now;
			localtime_s(&now, &transaction_time_);
			cout << "Payed at: " <<
				(now.tm_year + 1900) << "-" <<
				(now.tm_mon + 1) << "-" <<
				now.tm_mday << "\n";
			return is_payed;
		}
	};

	class Product {
	private:
		const int MAX_STOCK = 10;
		int remaining_stock_{10};
		int price_;
		string name_;
	public:
		enum Item {COKE, SODA, SNACK};
		Product():price_(0), name_("") {}
		Product(const int price, const string& name) :
			price_(price), name_(name) {}
		int GetStock() const { return remaining_stock_; }
		int GetPrice() const { return price_; }
		int GetRemainStock() const { return remaining_stock_; }
		string GetName() const { return name_; }
		void DeductStock() { remaining_stock_ -= 1; }
	};

	template<typename T>
	class Slot {
	private:
		int rows_ = 0;
		int cols_ = 0;
		vector<T> data_;
	public:
		void Resize(int row, int col) {
			rows_ = row;
			cols_ = col;
			data_.resize(row*col);
		}
		int GetRow() const { return rows_; }
		int GetCol() const { return cols_; }

		T& At(int row, int col) {
			return data_.at(row*cols_ + col);
		}
	};

	class Machine {
	private:
		Slot<unique_ptr<Product> > slots_;
		Payment* payment_;
		int balance_;
	public:
		Machine(int row, int col) {
			slots_.Resize(row, col);
			balance_ = 0;
			payment_ = nullptr;
			for (int r = 0; r < slots_.GetRow(); ++r) {
				for (int c = 0; c < slots_.GetCol(); ++c) {
					if(r == 0)
						slots_.At(r, c) = make_unique<Product>(Product(25, "Coke"));
					else if(r == 1)
						slots_.At(r, c) = make_unique<Product>(Product(5, "Soda"));
					else if(r == 2)
						slots_.At(r, c) = make_unique<Product>(Product(10, "Snack"));
				}
			}
			PrintMenu();
		}

		~Machine() { delete payment_; }

		bool MakePayment(int price, int row, int col) {
			int choice = 2;
			while (payment_ == nullptr) {
				cout << "Please chose payment method: \n" <<
					"(1) CardPayment (2) CashPayment \n";
				cin >> choice;
				payment_ = Payment::ChoosePayment(choice);
			}
			if (payment_->Transaction(price)) {
				DispenseItem(row, col);
				return true;
			}
			return false;
		}

		void GetRefund() {
			cout << "Refund: $" << balance_ << "\n";
			balance_ = 0;
		}

		void SelectItem() {
			int row = 0, col = 0;
			cout << "Please input the item row & col: \n";
			cin >> row >> col;
			if (slots_.At(row, col)->GetStock() == 0) {
				cout << "Sorry, out of stock. \n";
				return;
			}
			int price = slots_.At(row, col)->GetPrice();
			MakePayment(price, row, col);
		}

		void DispenseItem(int row, int col) {
			slots_.At(row, col)->DeductStock();
			cout << "Your item comes out on bottom \n";
		}

		void PrintMenu() {
			cout << "Current menu: \n \n";
			cout << "Name                Price                Stock \n\n";
			for (int row = 0; row < slots_.GetRow(); ++row) {
				for (int col = 0; col < slots_.GetCol(); ++col) {
					cout << slots_.At(row, col)->GetName() <<
						"                $: " <<
						slots_.At(row, col)->GetPrice() << "                " <<
						slots_.At(row, col)->GetRemainStock() << "\n";
				}
			}
		}
	};
}
*/


namespace VendingMachine {
	enum Input {ENTER, SELECT, PAY, EXIT};

	class Product {
	private:
		const int MAXSTOCK{ 10 };
		int remaining_stock;
		int price_;
		string name_;
	public:
		enum Item{COKE, WATER, JUICE};
		Product() :price_(0), name_("") {}
		Product(const int price, const string name) :
			price_(price), name_(name) {}
		int GetPrice() const { return price_; }
		int GetRemainStock() const { return remaining_stock; }
		string GetName() const { return name_; }
		void DeductOne() { remaining_stock -= 1; }
	};

	template<typename T>
	class Slot {
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

		int GetRows() const { return rows_; }
		int GetCols() const { return cols_; }
		T& At(int row, int col) { return data_.at(row*cols_ + col); }
	};

	class Machine;
	class User {
	protected:
		string name_;
		string email_;
		int phone_;
	public:
		User(const string name, const string email="") :
			name_(name), email_(email) {}
		string GetName() const {}
		string GetEmail() const {}
		int GetPhoneNumber() const {}
		virtual void SelectItem(Machine& machine) {}
	};

	class Client : public User {
	public:
		Client(string name = "user") : User(name) {}

		void Actions(Machine& machine);

		void SelectItem(Machine& machine);

		bool MakePayment(Machine& machine);
	};

	class Mechanic : public User {
	public:
		Mechanic(string name, string email) : User(name, email) {}
		void SelectItem(Machine& machine) {

		}

		void FillStock(Machine& machine) {

		}
	};


	class MachineState;
	class Machine {
	private:
		MachineState* state_;
		int id_;
		double balance_;
		Slot<Product*> slots;
	public:
		Machine(int id);

		double GetBalance() { return balance_; }
		void SetBalance(int balance) { balance_ += balance; }

		string GetProductName(int row, int col);

		void SelectItem(int row, int col) {
			double price = slots.At(row, col)->GetPrice();
			SetBalance(price);
		}

		void ChangeState(MachineState* newstate) {
			state_ = newstate;
			UpdateWithNewState();
		}

		void PrintMenu();

		virtual void HandleInput(Input input);

		virtual void UpdateWithNewState();
	};

	class StillState; class SelectState; class PayState;
	class MachineState {
	public:
		static StillState* stilling_;
		static PayState* paying_;
		virtual ~MachineState() = default;
		virtual void HandleInput(Machine&, Input) = 0;
		virtual void Update(Machine&) = 0;
	};

	class StillState : public MachineState {
		void HandleInput(Machine&, Input);
		void Update(Machine&);
	};

	class SelectState : public MachineState {
		void HandleInput(Machine&, Input);
		void Update(Machine&);
	};

	class PayState : public MachineState {
		void HandleInput(Machine&, Input);
		void Update(Machine&);
	};

	class Payment {
	protected:
		double amount_;
		bool payment_succeed;
		time_t transaction_time;
	public:
		virtual ~Payment() {}
		virtual bool Transaction(double money) = 0;
	};

	class CardPayment : public Payment{
	public:
		~CardPayment() {}
		bool Transaction(double money) {
			transaction_time = time(0);
			tm now;
			localtime_s(&now, &transaction_time);
			cout << "Payed at: " <<
				now.tm_year + 1900 << "-" <<
				now.tm_mon + 1 << "-" <<
				now.tm_mday << " " <<
				now.tm_hour << ":" <<
				now.tm_min << "\n";
			payment_succeed = true;
			return true;
		}
	};

	class CashPayment : public Payment {
	public:
		~CashPayment() {}
		bool Transaction(double money) {
			transaction_time = time(0);
			tm now;
			localtime_s(&now, &transaction_time);
			cout << "Payed at: " <<
				now.tm_year + 1900 << "-" <<
				now.tm_mon + 1 << "-" <<
				now.tm_mday << " " <<
				now.tm_hour << ":" <<
				now.tm_min << "\n";
			payment_succeed = money*100 - amount_ * 100;
			return payment_succeed;
		}
	};

	class PaymentFactory {
	public:
		static Payment* ChosePayment(int choice);
	};
}
