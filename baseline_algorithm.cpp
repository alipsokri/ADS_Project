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

		case 2: {
			if (orderQueue.empty()) {
				cout << "Queue is empty.\n";
			}
			else {
				queue<Order> temp = orderQueue;

				cout << "\nCurrent Queue:\n";

				int position = 1;
				while (!temp.empty()) {
					Order current = temp.front();
					cout << position << ". " << current.customerName << " (" << current.drinks << " drinks)\n";
					temp.pop();
					position++;
				}
			}
			break;
		}

		case 3: {
			if (orderQueue.empty()) {
				cout << "No orders to serve.\n";
			}
			else {
				Order current = orderQueue.front();
				orderQueue.pop();

				cout << "\nServing: " << current.customerName << " (" << current.drinks << " drinks)\n";
			}
			break;
		}
		case 4:
			cout << "Program Ended.\n";
			break;

		default:
			cout << "Invalid choice.\n";
		}
	} while (choice != 4);
	return 0;
}



	