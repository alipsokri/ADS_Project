#include <iostream>
#include <queue>
#include <string>
using namespace std;

struct Order {
	string customerName;
	int drinks;
};

int main() {
	queue<Order> orderQueue;
	int choice;

	do {
		cout << "\n===== QUEUE SYSTEM MIXUE =====\n";
		cout << "1. Add Order\n";
		cout << "2. View Queue\n";
		cout << "3. Serve Next Order\n";
		cout << "4. Exit\n";
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {

		case 1: {
			Order newOrder;

			cout << "Customer Name: ";
			cin.ignore();
			getline(cin, newOrder.customerName);

			cout << "Number of Drinks: ";
			cin >> newOrder.drinks;

			orderQueue.push(newOrder);

			cout << "Order added successfully.\n";
			break;
		}

		}
	} while ();

	return 0;
}



	