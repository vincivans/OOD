//
//  OnlineBooking.hpp
//  test
//
//  Created by SiranWang on 10/29/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef OnlineBooking_hpp
#define OnlineBooking_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <unordered_map>
using  namespace::std;

namespace MovieBooking {

enum SeatType {Regular, Premium, Other};

class Seat {
public:
    Seat(int id, SeatType type=SeatType::Regular): id_(id),
    type_(type), available(true) {}
    int GetId() const { return id_; }
    bool CheckAvailablity() { return available; }
    void SetUnavailable() { available = false; }
    void FreeSeat() { available = true; }
private:
    int id_;
    bool available;
    SeatType type_;
};

class ShowHall {
public:
    ShowHall(long create) : create_on(create) {}
    time_t GetStartTime() { return  start_time; }
    time_t GetEndTime() { return end_time; }
    void SetCreateTime() {
        auto now = chrono::system_clock::now();
        create_on = chrono::system_clock::to_time_t(now);
    }
    
    bool CanBook(string& row, string& col) {
        return seats[row+col]->CheckAvailablity();
    }
    
    void Book(string& row, string& col) {
        if (CanBook(row, col))
            seats[row+col]->SetUnavailable();
    }
private:
    int show_id;
    time_t create_on;
    time_t start_time;
    time_t end_time;
    unordered_map<string, Seat*> seats;
};


class Movie {
public:
    Movie(string& nm, string& des, string& dir,
          string& lan) : name(nm), description(des),
          director(dir), language(lan) {}
    
    vector<ShowHall*> GetShows() { return showhalls_; }
    
    ShowHall* FindShowHall(time_t starttime) {
        for (int id = 0; id < showhalls_.size(); ++id) {
            if (showhalls_[id]->GetStartTime() <= starttime &&
                showhalls_[id]->GetEndTime() > starttime) {
                return showhalls_[id];
            }
        }
        return nullptr;
    }
    
    void AddShowHall(ShowHall* showhall) {
        showhalls_.push_back(showhall);
    }
    
    bool Book(time_t starttime, string& row, string& col) {
        auto showhall = FindShowHall(starttime);
        if (showhall) {
            showhall->Book(row, col);
            return true;
        }
        return false;
    }
private:
    string name;
    string director;
    string description;
    string language;
    vector<ShowHall*> showhalls_;
};



class Booking {
public:
    enum BookingStatus {Pending, Success, Cancel};
    Booking() {}
    Booking(Movie* movie, time_t starttime,
            string& r, string& c) : row(r), col(c) { movies_.push_back(movie); create_on = starttime;}
    int GetBookingId() const { return booking_id; }
    int GetSeats() const { return number_of_seats; }
    long GetBookingTimestamp() const { return create_on; }
    void CancelBooking() { status_ = BookingStatus::Cancel; }
    bool MakeReservation() {
        if (CanBook()) status_ = BookingStatus::Success;
        return status_ == BookingStatus::Success;
    }
    
    bool CanBook() {
        bool allcan;
        for (auto& m : movies_) m->Book(create_on, row, col);
        return true;
    }
private:
    string row, col;
    int booking_id;
    int number_of_seats;
    time_t create_on;
    BookingStatus status_;
    vector<Movie*> movies_;
};



class Person {
private:
    string name;
    string address;
    string email;
};

class Customer : public Person {
public:
    bool MakeReservation(Booking* booking) {
        return booking->MakeReservation();
    }
    
    vector<Booking*> GetReservations () {
        return reservations;
    }
    
    void CancelReservation(Booking* booking) {
        for (int id = 0; id < reservations.size(); ++id) {
            if (booking->GetBookingId() == reservations[id]->GetBookingId()) {
                reservations[id]->CancelBooking();
                reservations.erase(reservations.begin()+id);
                break;
            }
        }
    }
private:
    vector<Booking*> reservations;
};

class Admin : public Person {
public:
    void AddMovie(Movie* movie) {
        
    }
    
    void AddShowHall(Movie* movie, ShowHall* showhall) {
        movie->AddShowHall(showhall);
    }
};

class Theater {
public:
    Theater(string& nm, string& addr) : name(nm), address(addr) {}
    void AddMovie(Admin* ad, Movie* movie) {
        ad->AddMovie(movie);
        movies.push_back(movie);
    }
private:
    string name;
    string address;
    vector<Movie*> movies;
};

}

#endif /* OnlineBooking_hpp */
