#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "Device.h"

using namespace std;

mutex queueLocker;
mutex conditionLocker;
condition_variable check;
bool periodBool = false;
bool processBool = false;
bool endBool = false;

Device device = Device();
queue<int> orders;
queue<int> speeds;
queue<int> pressuresD1;
queue<int> pressuresD2;
chrono::milliseconds msPeriod = 10000ms;

void newMenu() {
	try {
		while (!endBool) {
			int order;
			cout << "Select:\n1 - Pump\n2 - First sensor\n3 - Second sensor\n4 - Status\n5 - Change period\n Other key - Exit\n";
			cin >> order;
			{
				switch (order) {
				case 1:
					int pumpOrder;
					cout << "Select order:\n1 - Set speed\n2 - Turn of\nOther key - Back\n";
					cin >> pumpOrder;
					switch (pumpOrder) {
					case 1:
						int speed;
						cout << "Enter speed\n";
						cin >> speed;
						queueLocker.lock();
						speeds.push(speed);
						orders.push(11);
						queueLocker.unlock();
						processBool = true;
						break;
					case 2:
						queueLocker.lock();
						orders.push(12);
						queueLocker.unlock();
						processBool = true;
						break;
					default:
						break;
					}
					break;
				case 2:
					int sensor1Order;
					cout << "Select order:\n1 - Set pressure\n2 - Turn pressure to zero\nOther key - Back\n";
					cin >> sensor1Order;
					switch (sensor1Order) {
					case 1:
						int pressure;
						cout << "Enter pressure\n";
						cin >> pressure;
						queueLocker.lock();
						pressuresD1.push(pressure);
						orders.push(21);
						queueLocker.unlock();
						processBool = true;
						break;
					case 2:
						queueLocker.lock();
						orders.push(22);
						queueLocker.unlock();
						processBool = true;
						break;
					default:
						break;
					}
					break;
				case 3:
					int sensor2Order;
					cout << "Select order:\n1 - Set pressure\n2 - Turn pressure to zero\nOther key - Back\n";
					cin >> sensor2Order;
					switch (sensor2Order) {
					case 1:
						int pressure;
						cout << "Enter pressure\n";
						cin >> pressure;
						queueLocker.lock();
						pressuresD2.push(pressure);
						orders.push(31);
						queueLocker.unlock();
						processBool = true;
						break;
					case 2:
						queueLocker.lock();
						orders.push(32);
						queueLocker.unlock();
						processBool = true;
						break;
					default:
						break;
					}
					break;
				case 4:
					device.status();
					break;
				case 5:
					int intPeriod;
					cout << "Enter period in milliseconds\n";
					cin >> intPeriod;
					queueLocker.lock();
					msPeriod = (chrono::milliseconds)intPeriod;
					queueLocker.unlock();
					periodBool = true;
					check.notify_one();
					break;
				default:
					endBool = true;
					check.notify_one();
					break;
				}
			}
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

void process() {
	try{
		unique_lock<mutex> uLocker(conditionLocker);
		while (!endBool) {
			do {
				if (orders.empty()) {
					device.status();
				}
				check.wait_for(uLocker, msPeriod);
			} while (!periodBool && !processBool && !endBool);

			periodBool = false;
			queueLocker.lock();
			if (!orders.empty()) {
				int order = orders.front();
				orders.pop();
				switch (order) {
				case 11:
					device.pump.turnOn(speeds.front());
					speeds.pop();
					if (orders.empty()) {
						processBool = false;
					}
					break;
				case 12:
					device.pump.turnOff();
					if (orders.empty()) {
						processBool = false;
					}
					break;
				case 21:
					device.D1.setPressure(pressuresD1.front());
					pressuresD1.pop();
					if (orders.empty()) {
						processBool = false;
					}
					break;
				case 22:
					device.D1.reset();
					if (orders.empty()) {
						processBool = false;
					}
					break;
				case 31:
					device.D2.setPressure(pressuresD2.front());
					pressuresD2.pop();
					if (orders.empty()) {
						processBool = false;
					}
					break;
				case 32:
					device.D2.reset();
					if (orders.empty()) {
						processBool = false;
					}
					break;
				}
			}
			queueLocker.unlock();
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}
int main() {
	try {
		thread processThread(process);
		newMenu();
		processThread.join();
		return 0;
	}
	catch (exception e) {
		cout << e.what() << endl;
		return -1;
	}
}