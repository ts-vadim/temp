#include <TroykaLedMatrix.h>
#include <Octoliner.h>
#include <Dragster.h>
#include <PID_dragster.h>

//	Controls
const float weights[] = {
	0.25,
	0.5,
	0.75,
	1
};
const float KProp = 0.7;
const float startK = 0.35;



Dragster robot;
TroykaLedMatrix matrix;
Octoliner octoliner;
PID pid(KProp, 0, 0);

byte diagram[8];
float error = 0;
float speed[2] = {0, 0};



void computeError(float& error);
void computeSpeed(float error, float& left, float& right);
void computeSpeedPID(float error, float& left, float& right);
void printStats();


void setup() {
	robot.begin();
	octoliner.begin(200);
	matrix.begin();
	Serial.begin(9600);
}

void loop() {
	computeError(error);
	//computeSpeed(error, speed[0], speed[1]);
	computeSpeedPID(error, speed[0], speed[1]);


	robot.driveF(speed[0], speed[1]);
	matrix.drawBitmap(diagram);


	//Serial.println();
}

void serialEvent()
{
	String msg;
	char chr;
	/*
	while (Serial.available() > 0)
	{
		chr +=
		if (chr == '\n')
			break;
		msg += chr;
	}
	*/
	Serial.println("Typed: " + Serial.read());
}



void printStats()
{
	Serial.println("PID:");
	Serial.println(
		"  (p" + String(pid.kp) +
		", i" + String(pid.ki) +
		", d" + String(pid.kd) +
		", min" + String(pid.limitMin) +
		", max" + String(pid.limitMax) +
		")"
	);
	Serial.println("  prev = " + String(pid.prev) + " acc = " + String(pid.acc));
	Serial.println("Error:" + String(error) + ", Speed: " + String(speed[0]) + ", " + String(speed[1]));
}


void computeError(float& error)
{
	float sum = 0;
	float sumWeighted = 0;
	float adcValue;
	for (int i = 0; i < 8; i++)
	{
		adcValue = octoliner.analogRead(i);
		diagram[i] = matrix.map(adcValue, 0, 4095);
		sum += adcValue;
		sumWeighted += adcValue * ((i < 4) ? -1.0 : 1.0) * weights[abs(3 - i)];
	}
	if (sum != 0.0)
		error = sumWeighted / sum;
}


void computeSpeed(float error, float& left, float& right)
{
	left = startK + error * KProp;
	right = startK - error * KProp;
}


void computeSpeedPID(float error, float& left, float& right)
{
	float output = pid.computeReturn(error);
	left = startK + output;
	right = startK - output;
}







//
