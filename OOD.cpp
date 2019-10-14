// OOD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "ParkingLot.h"
//#include "MineSweeper.cpp"
//#include "CardGames.h"
//#include "VendingMachine.h"
//#include "Elevator.h"
//using namespace Elevator;
//WorkerThread workerthread1("W1");
//WorkerThread workerthread2("W2");

//#include "ChessGame.h"
//using namespace Chess;

#include "LibraryManagment.h"
using namespace Library;


int main()
{
	/*using namespace BlackJack;
	char again = 'y';
	vector<string> names = { "Jack", "Marry", "Calvein" };
	while (again != 'n' && again != 'N') {
		cout << "\n-----------------------------Let's go:\n";
		Game game(names);
		game.Play();
		cout << "\nDo you want to play again? (Y/N)\n";
		cin >> again;
	}*/
	/*using namespace VendingMachine;
	Machine machine(3, 1);
	machine.SelectItem();*/
	/*workerthread1.CreateThread();
	workerthread2.CreateThread();
	//workerthread2.CreateThread();
	FloorButton fbt1 = FloorButton(4, Direction::UP);
	FloorButton fbt2 = FloorButton(11, Direction::UP);
	FloorButton fbt3 = FloorButton(12, Direction::DOWN);
	FloorButton fbt4 = FloorButton(26, Direction::UP);
	CarButton cbt1 = CarButton(1, Direction::DOWN, 2);
	FloorButton fbt5 = FloorButton(7, Direction::DOWN);
	CarButton cbt2 = CarButton(4, Direction::UP, 4);
	workerthread1.AddRequest(&fbt1);
	workerthread2.AddRequest(&fbt5);
	workerthread2.AddRequest(&fbt2);
	this_thread::sleep_for(5s);
	//workerthread1.AddRequest(&fbt3);
	workerthread2.AddRequest(&cbt1);
	workerthread1.AddRequest(&fbt4);
	workerthread1.AddRequest(&fbt3);
	workerthread2.AddRequest(&cbt2);
	//this_thread::sleep_for(2s);
	workerthread1.ExitThread();
	workerthread2.ExitThread();
	//fbt1.PushButton(); fbt2.PushButton(); fbt3.PushButton();
	//fbt4.PushButton(); cbt1.PushButton(); fbt5.PushButton();*/
	BookItem bki1;
	bki1.SetPrice(120.10);
	bki1.CheckOut("123");
	return 0;
}
