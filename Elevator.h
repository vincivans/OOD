#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <random>
using namespace std;

#define MSG_EXIT_THREAD			1
#define MSG_POST_USER_DATA		2
#define MSG_TIMER				3

namespace Elevator {
	
	enum class Direction { UP, DOWN, NONE };
	enum class Door { OPEN, CLOSE };

	class Elevators {
	private:
		int id_;
		int floor_;
		Direction direction_;
		Door door_;
		mutex mtx_;
		condition_variable mcv_;
	public:
		Elevators(int id) : id_(id), floor_(1), 
						  direction_(Direction::NONE), door_(Door::CLOSE) {}
		int GetPosition() { return floor_; }
		Direction GetState() { return direction_; }
		Door GetDoorStatus() { return door_; }
		bool StateModified() { return direction_ != Direction::NONE; }
		void SetState(Direction direction) { direction_ = direction; }
		void GoToDestination(int floor, Direction direction) {
			unique_lock<mutex> lk(mtx_);
			SetState(direction);
			if (direction == Direction::UP) {
				for (int i = floor_; i < floor; ++i) {
					this_thread::sleep_for(1s);
					cout << this_thread::get_id() << " Elevator:" << id_ << " is at:" << floor_ << "\n";
					++floor_;
				}
			}
			else {
				for (int i = floor_; i > floor; --i) {
					this_thread::sleep_for(1s);
					cout << this_thread::get_id() << " Elevator:" << id_ << " is at:" << floor_ << "\n";
					--floor_;
				}
			}
			//floor_ = floor;
			door_ = Door::OPEN;
			this_thread::sleep_for(2s);
			door_ = Door::CLOSE;
			//direction_ = Direction::NONE;
			cout << "Elevator:" << id_ << " arrives floor:" << floor_ << "\n";
			mcv_.notify_one();
		}
	};

	
	class System;
	class Button {
	private:
		int floor_;
		Direction direction_;
	public:
		Button(int floor, Direction direction) : floor_(floor), direction_(direction) {}
		virtual ~Button() {}
		int GetFloor() { return floor_; }
		Direction GetDirection() { return direction_; }
		virtual void PushButton() = 0;
		virtual int GetElevatorId() { return 0; }
	};

	class FloorButton : public Button {
	public:
		FloorButton(int floor, Direction direction) : Button(floor, direction) {}
		void PushButton();
	};

	class CarButton : public Button {
	private:
		int id_;
	public:
		CarButton(int floor, Direction direction, int id) : Button(floor, direction), id_(id) {}
		int GetElevatorId() { return id_; }
		void PushButton();
	};

	struct ThreadMsg
	{
		ThreadMsg(int i, Button* m) { id = i; msg = m; }
		int id;
		Button* msg;
	};

	class WorkerThread {
	public:
		WorkerThread(const string name) : m_thread(0),
			m_timer_exit(false), thread_name(name) {
			elevators.resize(4);
			for (int i = 0; i < 4; ++i)
				elevators[i] = new Elevators(i + 1);
		}

		~WorkerThread() { ExitThread(); }

		bool CreateThread() {
			if (!m_thread)
				m_thread = new thread(&WorkerThread::Process, this);
			return true;
		}

		void ExitThread() {
			if (!m_thread) return;

			ThreadMsg* threadMsg = new ThreadMsg(MSG_EXIT_THREAD, 0);
			{
				lock_guard<mutex> lock(m_mutex);
				requests.push(threadMsg);
				m_cv.notify_one();
			}
			m_thread->join();
			delete m_thread;
			m_thread = 0;
		}

		thread::id GetCurrentThreadId() {
			if (m_thread);
				return this_thread::get_id();
		}

		void AddRequest(Button* request) {
			ThreadMsg* threadMsg = new ThreadMsg(MSG_POST_USER_DATA, request);

			unique_lock<mutex> lk(m_mutex);
			requests.push(threadMsg);
			m_cv.notify_one();
		}

	private:
		thread* m_thread;
		queue<ThreadMsg*> requests;
		static vector<Elevators*> elevators;
		mutex m_mutex;
		condition_variable m_cv;
		atomic<bool> m_timer_exit;
		const string thread_name;

		WorkerThread(const WorkerThread&);
		WorkerThread& operator=(const WorkerThread&);

		void Process() {
			m_timer_exit = false;
			thread timerthread(&WorkerThread::TimerThread, this);

			while (1) {
				ThreadMsg* msg = 0;
				{
					unique_lock<mutex> lck(m_mutex);
					while (requests.empty())
						m_cv.wait(lck);
					if (requests.empty())
						continue;

					msg = requests.front();
					requests.pop();
				}
				switch (msg->id)
				{
					case MSG_POST_USER_DATA:
					{
						Button* cur_request = msg->msg;
						int id = cur_request->GetElevatorId();
						int floor = cur_request->GetFloor();
						Direction direction = cur_request->GetDirection();
						Elevators* assigned_elevator = nullptr;
						if (id) {
							assigned_elevator = elevators[id-1];
						}
						else {
							int dis = 100, minDis = 100;
							for (int i = 0; i < elevators.size(); ++i) {
								if (elevators[i]->GetState() == Direction::NONE) {
									assigned_elevator = elevators[i];
									break;
								}
								else if (elevators[i]->GetState() == direction) {
									dis = abs(elevators[i]->GetPosition() - floor);
									if (dis < minDis) {
										minDis = dis;
										assigned_elevator = elevators[i];
									}
								}
								else {
									assigned_elevator = elevators[rand() % elevators.size()];
									break;
								}
							}
						}
						assigned_elevator->GoToDestination(floor, direction);
						delete msg;
						break;
					}
					case MSG_TIMER:
						cout << "Timer expired on " << thread_name << endl;
						delete msg;
						break;
					case MSG_EXIT_THREAD:
					{
						m_timer_exit = true;
						timerthread.join();

						delete msg;
						std::unique_lock<std::mutex> lk(m_mutex);
						while (!requests.empty())
						{
							msg = requests.front();
							requests.pop();
							delete msg;
						}

						cout << "Exit thread on " << thread_name << "\n";
						return;
					}
				}
				
			}
		}

		void TimerThread() {
			while (!m_timer_exit) {
				this_thread::sleep_for(250ms);
				ThreadMsg* threadMsg = new ThreadMsg(MSG_TIMER, 0);
				unique_lock<mutex> lck(m_mutex);
				requests.push(threadMsg);
				m_cv.notify_one();
			}
		}
	};


	/*class System {
	private:
		WorkerThread workerthread1("w1");
		WorkerThread workerthread2("w2");
		vector<Elevators*> elevators;
		queue<Button*> requests;
		System() {
			elevators.resize(4);
			for (int i = 0; i < 4; ++i)
				elevators[i] = new Elevators(i+1);
			workerthread1.CreateThread();
			workerthread2.CreateThread();
		}
		~System() {
			for (int i = 0; i < 4; ++i)
				delete elevators[i];
			vector<Elevators*> tmp;
			elevators.swap(tmp);
		}
	public:
		static System& GetInstance() {
			static System instance;
			return instance;
		}

		void Run() {
			while (true) {
				Process();
			}
		}

		void Process() {
			while (!requests.empty()) {
				Button* cur_request = requests.front();
				int id = cur_request->GetElevatorId();
				int floor = cur_request->GetFloor();
				Direction direction = cur_request->GetDirection();
				if (id) {
					elevators[id]->GoToDestination(floor, direction);
				}
				else {
					for (int i = 0; i < elevators.size(); ++i) {
						if (elevators[i]->GetState() == Direction::NONE)
							elevators[i]->GoToDestination(floor, direction);
					}
				}
			}
		}

		void AddRequests(Button* request) {
			requests.push(request);
		}
	};*/

}