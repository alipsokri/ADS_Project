#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

/*
    ================================================================
    OPTIMIZED SOLUTION: PRIORITY QUEUE (MIN-HEAP)
    ================================================================
    - 2.1  Orders are assigned a priority based on drink count.
           1 = High (1-3 drinks), 2 = Medium (4-10 drinks), 3 = Low (11 or above drinks)
    - 2.2  Starvation prevention: priority escalates (value decreases)
           every AGING_INTERVAL_SECONDS the order remains unserved.
    - 2.3  Implemented as a binary heap (manual array-based heap) giving
           O(log N) insert/remove and O(1) access to the best order.

	NOTE: AGING_INTERVAL_SECONDS is set to 300 (5min). For demo, lower the value to a smaller number to observe escalation without waiting 5 real minutes.

*/

const int AGING_INTERVAL_SECONDS = 300; // 5mins

struct Order {
    string customerName;
    int drinks;
    int basePriority;      // priority originally assigned from drink count (never changes)
    int currentPriority;   // priority after aging/escalation
    int orderId;           
    time_t arrivalTime;
};

// 2.1 assign priority based on num of drinks 
int assignPriority(int drinks) {
    if (drinks <= 3)  return 1; // High
    if (drinks <= 10) return 2; // Medium
    return 3;                   // Low
}

string priorityLabel(int p) {
    switch (p) {
        case 1: return "High (1)";
        case 2: return "Medium (2)";
        case 3: return "Low (3)";
        default: return "Unknown";
    }
}

// True if order 'a' should be served strictly before order 'b'
bool betterThan(const Order& a, const Order& b) {
    if (a.currentPriority != b.currentPriority)
        return a.currentPriority < b.currentPriority; // smaller value = higher priority
    return a.orderId < b.orderId; // tie-break: first come, first served
}

// Min-Heap operations
// Moves the element at idx up until the heap property is restored.
void siftUp(vector<Order>& heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (betterThan(heap[idx], heap[parent])) {
            swap(heap[idx], heap[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

// Moves element at idx down until heap property is restored.
void siftDown(vector<Order>& heap, int idx) {
    int n = (int)heap.size();
    while (true) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int best = idx;

        if (left < n && betterThan(heap[left], heap[best]))
            best = left;
        if (right < n && betterThan(heap[right], heap[best]))
            best = right;

        if (best == idx) break;

        swap(heap[idx], heap[best]);
        idx = best;
    }
}

// Rebuild heap from scratch (used after priorities change).
void buildHeap(vector<Order>& heap) {
    for (int i = (int)heap.size() / 2 - 1; i >= 0; i--) {
        siftDown(heap, i);
    }
}

// Inserts new order and restores heap property. O(log N)
void pushOrder(vector<Order>& heap, const Order& o) {
    heap.push_back(o);
    siftUp(heap, (int)heap.size() - 1);
}

// Removes and returns the best order. O(log N)
Order popOrder(vector<Order>& heap) {
    Order top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) siftDown(heap, 0);
    return top;
}

// 2.2 Starvation prevention
// Recomputes currentPriority for every order based on elapsed waiting time,
// then rebuilds heap, called before any view/serve operation.
void updatePriorities(vector<Order>& heap) {
    time_t now = time(nullptr);
    for (auto& order : heap) {
        int elapsedSeconds = (int)difftime(now, order.arrivalTime);
        int escalations = elapsedSeconds / AGING_INTERVAL_SECONDS;
        int newPriority = order.basePriority - escalations;
        if (newPriority < 1) newPriority = 1;
        order.currentPriority = newPriority;
    }
    buildHeap(heap);
}

string formatWaitTime(const Order& order) {
    time_t now = time(nullptr);
    int totalSeconds = (int)difftime(now, order.arrivalTime);
    int mins = totalSeconds / 60;
    int secs = totalSeconds % 60;
    string secStr = (secs < 10 ? "0" : "") + to_string(secs);
    return to_string(mins) + "m " + secStr + "s";
}

int main() {
    vector<Order> orderHeap; // manual array-based Min-Heap
    int nextOrderId = 1;
    int choice;

    do {
        cout << "\n===== PRIORITY QUEUE SYSTEM MIXUE (Min-Heap) =====\n";
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

            newOrder.basePriority = assignPriority(newOrder.drinks);
            newOrder.currentPriority = newOrder.basePriority;
            newOrder.orderId = nextOrderId++;
            newOrder.arrivalTime = time(nullptr);

            pushOrder(orderHeap, newOrder);

            cout << "Order added successfully. Assigned Priority: "
                 << priorityLabel(newOrder.basePriority) << "\n";
            break;
        }

        case 2: {
            if (orderHeap.empty()) {
                cout << "Queue is empty.\n";
            }
            else {
                updatePriorities(orderHeap); // apply any pending aging/escalation

                // Copy the heap and pop from the copy so the real queue is left untouched
                vector<Order> temp = orderHeap;

                cout << "\nCurrent Queue (by serving priority):\n";
                int position = 1;
                while (!temp.empty()) {
                    Order o = popOrder(temp);
                    cout << position << ". " << o.customerName
                         << " (" << o.drinks << " drinks) - "
                         << priorityLabel(o.currentPriority)
                         << " - Waiting: " << formatWaitTime(o) << "\n";
                    position++;
                }
            }
            break;
        }

        case 3: {
            if (orderHeap.empty()) {
                cout << "No orders to serve.\n";
            }
            else {
                updatePriorities(orderHeap); // ensure starvation-prevention is applied first

                Order served = popOrder(orderHeap);

                cout << "\nServing: " << served.customerName
                     << " (" << served.drinks << " drinks) - "
                     << priorityLabel(served.currentPriority)
                     << " - Total Wait: " << formatWaitTime(served) << "\n";
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
