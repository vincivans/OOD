#include "stdafx.h"
#include "VendingMachine.h"

using namespace VendingMachine;

/*Payment* Payment::ChoosePayment(int choice) {
	if (choice == 1)
		return new CardPayment;
	else if (choice == 2)
		return new CashPayment;
	else {
		cout << "Please input the correct choice. \n";
		return nullptr;
	}
}*/

StillState* MachineState::stilling_ = new StillState();
PayState* MachineState::paying_ = new PayState();

Payment* PaymentFactory::ChosePayment(int choice) {
	if (choice == 1)
		return new CardPayment;
	else if (choice == 2)
		return new CashPayment;
	else {
		cout << "Please input the correct choice. \n";
		return nullptr;
	}
}

void StillState::HandleInput(Machine& machine_, Input input) {
	switch (input) {
	case Input::ENTER: 
		cout << "Welcome to the machine. The menu list as below:\n";
		machine_.PrintMenu();
	case Input::SELECT:
		cout << "Please select the item you want:\n";
		machine_.ChangeState(new SelectState);
		break;
	case Input::PAY:
		machine_.ChangeState(MachineState::paying_);
		break;
	}
}

void StillState::Update(Machine& machine_) {
	cout << "Now we have a still machine\n";
}

void SelectState::HandleInput(Machine& machine_, Input input) {
	switch (input) {
	case Input::SELECT:
		machine_.ChangeState(new SelectState);
	case Input::PAY:
		machine_.ChangeState(MachineState::paying_);
	case Input::EXIT:
		machine_.ChangeState(MachineState::stilling_);
		break;
	default:
		cout << "You cannot do that after selection, back to still state\n";
		machine_.ChangeState(MachineState::stilling_);
	}
}

void SelectState::Update(Machine& machine_) {
	cout << "Machine enter the select state, please select an item\n";
	cout << "Please select item, input format is row, col\n";
	int row = 0, col = 0;
	cin >> row >> col;
	machine_.SelectItem(row, col);
	cout << "Item at (" << row << "," << col << ") " << 
		    machine_.GetProductName(row, col) <<", Price is: $" <<
		    machine_.GetBalance()<<"\n";
	int choice;
	cout << "Please select an action\n";
	std::cout << std::endl << SELECT << ") Reselect  " << PAY << ") PAY  " << EXIT
		<< ") Exit  " << std::endl;
	std::cin >> choice;
	const Input input1 = static_cast<Input>(choice);
	machine_.HandleInput(input1);
	
}

void PayState::HandleInput(Machine& machine_, Input input) {
	cout << "Thanks for selecting and making the payment\n";
	machine_.ChangeState(MachineState::stilling_);
}

void PayState::Update(Machine& machine_) {
	cout << "Machine enter the pay state, please pay the balance below:\n";
	int choice = 0;
	cout << "Please select payment method: 1.Card 2.Cash\n";
	cin >> choice;
	PaymentFactory::ChosePayment(choice)->Transaction(machine_.GetBalance());
}

void Client::Actions(Machine& machine) {
	int choice;
	auto chooseaction = [&choice](Machine& m) {
		cout << "Please selet an action\n" << ENTER + 1 << ") Enter " <<
			SELECT + 1 << ") Select " << PAY + 1 << ") Pay " << EXIT + 1 << ") Exit \n";
		cin >> choice;
		const Input input = static_cast<Input>(choice - 1);
		m.HandleInput(input);
	};
	chooseaction(machine);
}


void Client::SelectItem(Machine& machine) {
	cout << "Please select item, input format is row, col\n";
	int row = 0, col = 0;
	machine.SelectItem(row, col);
}

bool Client::MakePayment(Machine& machine) {
	machine.HandleInput(Input::PAY);
	int choice = 0;
	cout << "Please select payment method: 1.Card 2.Cash\n";
	cin >> choice;
	return PaymentFactory::ChosePayment(choice);
}

Machine::Machine(int id) {
	id_ = id;
	state_ = MachineState::stilling_;
	slots.Resize(6, 3);
	for (int r = 0; r < slots.GetRows(); ++r) {
		slots.At(r, 0) = new Product(2.5, "COKE");
		slots.At(r, 1) = new Product(1, "Water");
		slots.At(r, 2) = new Product(3.5, "JUICE");
	}
}

void Machine::HandleInput(Input input) {
	state_->HandleInput(*this, input);
}

void Machine::UpdateWithNewState() {
	state_->Update(*this);
}

string Machine::GetProductName(int row, int col){
	return slots.At(row, col)->GetName();
}

void Machine::PrintMenu() {
	for (int r = 0; r < slots.GetRows(); ++r) {
		for (int c = 0; c < slots.GetCols(); ++c) {
			cout << slots.At(r, c)->GetName() << " $ " <<
				slots.At(r, c)->GetPrice() << "   ";
		}
		cout << "\n";
	}
}


