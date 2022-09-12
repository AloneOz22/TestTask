#include "Device.h"

Device::Device() {
	pump = Pump();
	D1 = Sensor();
	D2 = Sensor();
}

//Device::~Device() {
//	delete[] &pump;
//	delete[] &D1;
//	delete[] &D2;
//}

void Device::status() {
	std::cout << "Pump:\nCurrent mode - ";
	if (pump.isWorking()) {
		std::cout << "Speed" << std::endl;
	}
	else {
		std::cout << "Stopped" << std::endl;
	}
	std::cout << "Current speed - ";
	if (pump.isWorking()) {
		std::cout << pump.currentSpeed() + (std::rand() % 4) << std::endl;
	}
	else {
		std::cout << 0 << std::endl;
	}
	std::cout << "Pressure in sensors:\n First - " << D1.currentPressure() + (std::rand() % 4) << "\nSecond - " << D2.currentPressure() + (std::rand() % 4) << "Difference - "
		<< (D1.currentPressure() + (std::rand() % 4)) - (D2.currentPressure() + (std::rand() % 4)) << std::endl;
}