#include "stdafx.h"
#include "Elevator.h"

namespace Elevator {
	//std::mutex Elevators::mtx_;
	//std::condition_variable Elevators::mcv_;
	vector<Elevators*> WorkerThread::elevators;
	//int Elevators::floor_ = 1;
	//Direction Elevators::direction_ = Direction::NONE;
	WorkerThread workerthread1("W1");
	WorkerThread workerthread2("W2");

	void FloorButton::PushButton() {
		//System::GetInstance().AddRequests(this);
		workerthread1.AddRequest(this);
	}

	void CarButton::PushButton() {
		//System::GetInstance().AddRequests(this);
		workerthread2.AddRequest(this);
	}

}
