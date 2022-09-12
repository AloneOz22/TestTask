#pragma once
#include <iostream>
#include "Pump.h"
#include "Sensor.h"

class Device
{
public:
	Pump pump;
	Sensor D1, D2;
	Device();
	//~Device();
	void status();
};

