#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
using namespace std;

namespace RestaurantManagement {
	class Reservation;
	class Branch;
	class Meal;

	template<typename T>
	class Table {
	private:
		int available_seats;
		int table_id;
		int rows_;
		int cols_;
		vector<T> data_;
	public:
		void Reset(int row, int col) {
			rows_ = row;
			cols_ = col;
			data_.resize(row*col);
		}
		int GetRow() const { return rows_; }
		int GetCol() const { return cols_; }
		T& At(int row, int col) const { return data_.at(row*cols_ + col); }
		int GetTableId() { return table_id; }
		int GetAvailableSeats() { return available_seats; }
	};

	class TableSeat {
	public:
		enum SeatType { REGULAR, KID, ACCESSORY, OTHER };
		int seat_id;
		SeatType type_;
		TableSeat(int id, SeatType type = SeatType::REGULAR) :
			seat_id(id), type_(type) {}
		bool UpdateSeatType(SeatType& t) { type_ = t; }
	};

	class Person {
	protected:
		string name_;
		string email_;
		string phone_;
	public:
		Person(string name, string email, string phone) :
			name_(name), email_(email), phone_(phone) {}
		string GetName() const { return name_; }
		string GetEmail() const { return email_; }
		string GetPhone() const { return phone_; }
	};

	class Employee : public Person {
	protected:
		int employee_id;
		time_t date_joined;
	public:
		Employee(string name, string phone, string email, int id) :
			Person(name, phone, email), employee_id(id) {}
	};

	class Manager : public Employee {
	public:
		bool AddEmployee(Employee* employee) {

		}
	};


	class Receipionst : public Employee {
	public:
		bool AddReservation(Reservation* reserve) {

		}
	};

	class Waiter : public Receipionst {
	public:
		bool CreateOrder() {
			return true;
		}
	};

	class Chef : public Employee {
		bool TakeOrder() {
			return true;
		}
	};

	class Customer : public Person {
	public:
		Customer(string name, string email, string phone) :
			Person(name, email, phone) {}
		bool MakeReservation(Reservation* reserve);
	};


	class Restaurant
	{
	public:
		Restaurant(string name) : name_(name) {}
		~Restaurant() {}

		bool AddBranch(Branch* branch);
	protected:
		string name_;
		vector<Branch*> branchs_;
	};

	class Branch : public Restaurant {
	public:
		Branch(string name) : Restaurant(name) {}
		string GetLocation() { return location_; }
	private:
		string location_;
	};

	class Order {
	public:
		enum OrderStatus {COMPLETE, CANCEL, PENDING};
		Order(int id) : order_id(id), create_time(time(0)) {}
		void AddMeal(Meal* meal) {
			meals.push_back(meal);
		}
		void RemoveMeal(Meal* meal) {
			int to_delete = -1;
			for (int i = 0; i < meals.size(); ++i) {
				if (meal == meals[i]) {
					to_delete = i;
					break;
				}
			}
			meals.erase(meals.begin() + to_delete);
		}

		void SetOrderStatus(OrderStatus status) { order_status = status; }
	private:
		int order_id;
		time_t create_time;
		vector<Meal*> meals;
		Table<TableSeat*> table;
		OrderStatus order_status;
	};

	class MenuItem {
	private:
		int item_id;
		string name_;
		string description_;
		double price_;
	public:
		MenuItem(string name, double price, int id, string des = "") :
			name_(name), price_(price), description_(des), item_id(id) {}
	};

	class Menu {
	private:
		int menu_id;
		string title_;
		vector<MenuItem*> items_;
	public:
		Menu(int id, string title = "Menu") : menu_id(id), title_(title) {}
		string GetTitle() { return title_; }
		bool AddMenuItem(MenuItem* item) {
			items_.push_back(item);
			return true;
		}

		void Print() {

		}
	};

	class MealItem {
	private:
		int quantity_;
		MenuItem* item_;
	public:
		MealItem(MenuItem* item, int q) : item_(item), quantity_(q) {}
		bool UpdateQuantity(int q) {
			return quantity_ = q;
		}
	};

	class Meal {
	private:
		int meal_id;
		TableSeat* serve_seat;
		vector<MealItem*> items_;
	public:
		Meal(int id, TableSeat* seat) :
			meal_id(id), serve_seat(seat) { }

		bool AddMenuItem(MealItem* item) {
			items_.push_back(item);
		}
	};

	class Reservation {
	public:
		enum ReservationStatus { CHECK_IN, CANCEL, PENDING, REQUESTED, CONFIRMED, ABANDONED };
		Reservation(Customer* guest, int groupnum, string note = "") :
			customer_(guest), group_number(groupnum), notes_(note),
			date_of_reservation(time(0)){}
		bool UpdateGroupNumber(int number) { return group_number = number; }
		ReservationStatus GetReservationStatus() { return status_; }
	private:
		time_t date_of_reservation;
		time_t checkin_time;
		int reveration_id;
		int group_number;
		ReservationStatus status_;
		string notes_;
		Customer* customer_;
	};
}

