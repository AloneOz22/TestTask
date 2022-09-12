#include "Pump.h"

Pump::Pump() {
	speed = 0;
}

//Pump::~Pump() {
//	delete[] &speed;
//}

void Pump::turnOn(int s) {
	if (s == 0) {
		turnOff();
	}
	else {
		speed = s;
	}
}

void Pump::turnOff() {
	speed = 0;
}

int Pump::currentSpeed() {
	return speed;
}

bool Pump::isWorking() {
	bool result = false;
	if (speed != 0) {
		result = true;
	}
	return result;
}