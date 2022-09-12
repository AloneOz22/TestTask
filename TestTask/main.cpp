#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include "Device.h"

using namespace std;

void menu(Device &device, queue<int> &orders, bool &menuIsClosed, queue<int> &speeds, queue<int> &pressuresD1, queue<int> &pressuresD2, chrono::milliseconds &period) {
	int order;
	cout << "Select:\n1 - Pump\n2 - First sensor\n3 - Second sensor\n4 - Status\n5 - Change period\n Other key - Exit\n";
	cin >> order;
	switch (order) {
	case 1:
		int pumpOrder;
		cout << "Select order:\n1 - Set speed\n2 - Turn of\nOther key - Exit\n";
		cin >> pumpOrder;
		switch (pumpOrder) {
		case 1:
			int speed;
			cout << "Enter speed\n";
			cin >> speed;
			speeds.push(speed);
			orders.push(11);
			break;
		case 2:
			orders.push(12);
			break;
		default:
			break;
		}
		menu(device, orders, menuIsClosed, speeds, pressuresD1, pressuresD2, period);
		break;
	case 2:
		int sensor1Order;
		cout << "Select order:\n1 - Set pressure\n2 - Turn pressure to zero\nOther key - Exit\n";
		cin >> sensor1Order;
		switch (sensor1Order) {
		case 1:
			int pressure;
			cout << "Enter pressure\n";
			cin >> pressure;
			pressuresD1.push(pressure);
			orders.push(21);
			break;
		case 2:
			orders.push(22);
			break;
		default:
			break;
		}
		menu(device, orders, menuIsClosed, speeds, pressuresD1, pressuresD2, period);
		break;
	case 3:
		int sensor2Order;
		cout << "Select order:\n1 - Set pressure\n2 - Turn pressure to zero\nOther key - Exit\n";
		cin >> sensor2Order;
		switch (sensor2Order) {
		case 1:
			int pressure;
			cout << "Enter pressure\n";
			cin >> pressure;
			pressuresD2.push(pressure);
			orders.push(31);
			break;
		case 2:
			orders.push(32);
			break;
		default:
			break;
		}
		menu(device, orders, menuIsClosed, speeds, pressuresD1, pressuresD2, period);
		break;
	case 4:
		device.status();
		menu(device, orders, menuIsClosed, speeds, pressuresD1, pressuresD2, period);
		break;
	case 5:
		int intPeriod;
		cout << "Enter period in milliseconds\n";
		cin >> intPeriod;
		period = (chrono::milliseconds)intPeriod;
		this_thread::yield();
		menu(device, orders, menuIsClosed, speeds, pressuresD1, pressuresD2, period);
		break;
	default:
		menuIsClosed = true;
		this_thread::yield();
		break;
	}
}

void process(Device& device, queue<int>& orders, bool& menuIsClosed, queue<int>& speeds, queue<int>& pressuresD1, queue<int>& pressuresD2, chrono::milliseconds& period) {
	try{
		while (menuIsClosed == false) {
			if (orders.empty()) {
				device.status();
			}
			else {
				int order = orders.front();
				orders.pop();
				switch (order) {
				case 11:
					//m.lock();
					device.pump.turnOn(speeds.front());
					speeds.pop();
					//m.unlock();
					break;
				case 12:
					//m.lock();
					device.pump.turnOff();
					//m.unlock();
					break;
				case 21:
					//m.lock();
					device.D1.setPressure(pressuresD1.front());
					pressuresD1.pop();
					//m.unlock();
					break;
				case 22:
					//m.lock();
					device.D1.reset();
					//m.unlock();
					break;
				case 31:
					//m.lock();
					device.D2.setPressure(pressuresD2.front());
					pressuresD2.pop();
					//m.unlock();
					break;
				case 32:
					//m.lock();
					device.D2.setPressure(pressuresD2.front());
					pressuresD2.pop();
					//m.unlock();
					break;
				}
			}
			this_thread::sleep_for(period);
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}
int main() {
	try {
		//mutex m;
		Device device = Device();
		queue<int> orders;
		bool menuIsClosed = false;
		queue<int> speeds;
		queue<int> pressuresD1;
		queue<int> pressuresD2;
		chrono::milliseconds msPeriod = 20000ms;
		thread menuThread([&]()
			{
				menu(device, orders, menuIsClosed, speeds, pressuresD1, pressuresD2, msPeriod);
			});
		thread processThread([&]()
			{
				process(device, orders, menuIsClosed, speeds, pressuresD1, pressuresD2, msPeriod);
			});
		menuThread.join();
		processThread.join();
		return 0;
	}
	catch (exception e) {
		cout << e.what() << endl;
		return -1;
	}
}