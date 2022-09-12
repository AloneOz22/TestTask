#include "Sensor.h"

Sensor::Sensor() {
	pressure = 0;
}

//Sensor::~Sensor() {
//	delete[] &pressure;
//}

void Sensor::setPressure(int p) {
	pressure = p;
}

void Sensor::reset() {
	pressure = 0;
}

int Sensor::currentPressure() {
	return pressure;
}