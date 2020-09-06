//
//  Elevator.hpp
//  test
//
//  Created by SiranWang on 9/5/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Elevator_hpp
#define Elevator_hpp

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <chrono>

namespace ElevatorSystem {

//class ElevatorControl;

enum Direction {
    Up,
    Down,
    Stale
};


class Event {
public:
    Event(unsigned int t) : time(t) {}
    virtual ~Event() {} 
    virtual void ProcessEvent() = 0;
    virtual bool IsFreeEvent() const= 0;
    const unsigned int time;
};

struct EventComparator {
    bool operator()(const Event* lhs, const Event* rhs) {
        return lhs->time > rhs->time;
    }
};

class InCarEvent : public Event {
public:
    InCarEvent(unsigned int t, int lvl, Direction dirc) :
    Event(t), level(lvl), direction(dirc) {}
    virtual void ProcessEvent();
    virtual bool IsFreeEvent() const { return false; }
    virtual Direction GetDirection() { return direction; }
    virtual int GetLevel() { return level; }
private:
    int level;
    Direction direction;
};

class InHallEvent : public Event {
public:
    InHallEvent(unsigned int t, int lvl, Direction dirc) :
    Event(t), level(lvl), direction(dirc) {}
    virtual void ProcessEvent();
    virtual bool IsFreeEvent() const { return false; }
    virtual Direction GetDirection() { return direction; }
    virtual int GetLevel() { return level; }
private:
    int level;
    Direction direction;
};

class FreeElevatorEvent : public Event {
public:
    FreeElevatorEvent(unsigned int t, int elevatorid, int level) :
    Event(t), id(elevatorid), level_(level) { }
    virtual void ProcessEvent();
    virtual bool IsFreeEvent() const { return true; }
private:
    int id;
    int level_;
};

class Elevator {
public:
    Elevator(int id): id_(id), level(1), direction_(Direction::Stale) {}
    int GetId() { return id_; }
    int GetLevel() { return level; }
    Direction GetDirection() { return direction_; }
    void SetDirection(Direction direction) { direction_ = direction; }
    void SetLevel(int lvl) { level = lvl; }
    bool IsStale() { return event_q.empty(); }
    void AddEvent(int lvl) { event_q.push(lvl); }
    void RemoveEvent() { event_q.pop(); }
private:
    int id_;
    int level;
    Direction direction_;
    std::queue<int> event_q;
};


class ElevatorSimulation {
public:
    ElevatorSimulation() {}
    void ScheduleEvent(Event* event) {
        event_q.push(event);
    }
    void Run() {
        while(!event_q.empty()) {
            auto cur_event = event_q.top();
            event_q.pop();
            cur_event->ProcessEvent();
            delete cur_event;
        }
    }
private:
    std::priority_queue<Event*,
                        std::vector<Event*>,
                        EventComparator> event_q;
};

class ElevatorControl : public ElevatorSimulation {
public:
    static ElevatorControl& GetInstance() {
        static ElevatorControl theElevatorControl;
        return theElevatorControl;
    }
    
    int AssignElevators(Direction direction, int level) {
        int id = -1;
        for (auto elevator : elevators) {
            if(elevator->GetDirection() == Direction::Stale) {
                elevator->SetDirection(direction);
                elevator->SetLevel(level);
                elevator->AddEvent(level);
                return elevator->GetId();
            }
        }
        return id;
    }
    
    bool FreeElevator(int id, int level) {
        elevators[id]->SetLevel(level);
        elevators[id]->RemoveEvent();
        if(elevators[id]->IsStale())
            elevators[id]->SetDirection(Direction::Stale);
        return true;
    }
    
private:
    std::vector<Elevator*> elevators;
    int num = 4;
    
    ElevatorControl() : ElevatorSimulation() {
        elevators.resize(num);
        for (int idx = 0; idx < num; ++idx)
            elevators[idx] = new Elevator(idx);
    }
};

}

#endif /* Elevator_hpp */

