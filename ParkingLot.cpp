//
//  ParkingLot.hpp
//  test
//
//  Created by SiranWang on 8/13/19.
//  Copyright © 2019 SiranWang. All rights reserved.
//

#ifndef ParkingLot_hpp
#define ParkingLot_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

namespace ParkingLot{

enum SpotType{Motor, Sedan, Bus};

class Vehicle {
public:
    Vehicle(string& p, SpotType st) : plate(p), spottype_(st) {}
    virtual ~Vehicle() {}
    string GetPlate() const { return plate; }
    SpotType GetSpotType() const { return spottype_; }
private:
    string plate;
    SpotType spottype_;
};

class Motor : public Vehicle {
public:
    Motor(string& p, SpotType st) : Vehicle(p, st) {}
};

class Car : public Vehicle {
public:
    Car(string& p, SpotType st) : Vehicle(p, st) {}
};

class Bus : public Vehicle {
public:
    Bus(string& p, SpotType st) : Vehicle(p, st) {}
};


class Spot {
public:
    enum SpotType{Motor, Sedan, Bus};
    Spot(SpotType type) : spottype_(type), vehicle_(nullptr), available(true) { }
    bool IsAvailable() { return available; }
    void AssignVehicle(Vehicle* v) {
        available = false;
        vehicle_ = v;
    }
    void FreeSpot() {
        available = true;
        vehicle_ = nullptr;
    }
private:
    int id;
    SpotType spottype_;
    Vehicle* vehicle_;
    bool available;
};

class SmallSpot : public Spot {
public:
    SmallSpot(SpotType type) : Spot(type) { }
};

class MediumSpot : public Spot {
public:
    MediumSpot(SpotType type) : Spot(type) { }
};

class LargeSpot : public Spot {
public:
    LargeSpot(SpotType type) : Spot(type) { }
};

class ParkingLevel {
public:
    ParkingLevel(int num) : level(num), id_(0) {}
    void AssignVehicleToSpot(Vehicle* vehicle, Spot* spot) {
        spot->AssignVehicle(vehicle);
        spots_[id_++] = spot;
    }
    void FreeSpot(Spot* spot) {
        spot->FreeSpot();
        spots_.erase(id_);
    }
    
    bool IsFull() {
        return false;
    }
private:
    unordered_map<int, Spot*> spots_;
    int level;
    int id_;
};

class Lot {
public:
    static Lot& Instance() {
        static Lot* instance(new Lot());
        return *instance;
    }
    
    bool IsFull() {
        for (auto it : levels) {
            if (!it.second->IsFull())
                return false;
        }
        return true;
    }
private:
    string name_;
    string address_;
    unordered_map<int, ParkingLevel*> levels;
    
    Lot() {
        for (int id = 0; id < 5; ++id) {
            levels[id] = new ParkingLevel(id);
        }
    }
    ~Lot() { }
};

}



#endif /* ParkingLot_hpp */
