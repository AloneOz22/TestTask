#pragma once
class Pump
{
private: 
	int speed;
public:
	Pump();
	//~Pump();
	void turnOn(int s);
	void turnOff();
	int currentSpeed();
	bool isWorking();
};

