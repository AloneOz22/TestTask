#pragma once
class Sensor
{
private:
	int pressure;
public:
	Sensor();
	//~Sensor();
	void setPressure(int p);
	void reset();
	int currentPressure();
};

