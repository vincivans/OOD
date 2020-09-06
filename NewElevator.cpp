//
//  Elevator.cpp
//  test
//
//  Created by SiranWang on 9/5/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#include "Elevator.hpp"
#include <chrono>
#include <thread>
using namespace std;

namespace ElevatorSystem {

    void InCarEvent::ProcessEvent() {
         int id = ElevatorControl::GetInstance().AssignElevators(direction, level);
         if(id == -1) {
            ElevatorControl::GetInstance().ScheduleEvent(new InCarEvent(time+rand()%2, this->level, this->direction));
        }
         else {
             cout <<"At time: " << this->time <<" Elevator id: " << id <<
             " is going "<< (direction==Direction::Up ? "Up" : "Down")<< " to " << level << " floor!\n";
            
             ElevatorControl::GetInstance().ScheduleEvent(new FreeElevatorEvent(time+rand()%3, id, this->level));
             
         }
    }

    void InHallEvent::ProcessEvent() {
         int id = ElevatorControl::GetInstance().AssignElevators(direction, level);
         if (id == -1) {
            ElevatorControl::GetInstance().ScheduleEvent(new InHallEvent(time+rand()%2, this->level, this->direction));
         }
         else {
             cout << "At time: " << this->time <<" Elevator id: " << id <<
             " is going "<< (direction==Direction::Up ? "Up" : "Down")<< " to " << level << " floor!\n";
            
             ElevatorControl::GetInstance().ScheduleEvent(new FreeElevatorEvent(time+rand()%3, id, this->level));
         }
    }

    void FreeElevatorEvent::ProcessEvent() {
        ElevatorControl::GetInstance().FreeElevator(this->id, this->level_);
    }
}

 

