#include <Wire.h>
#include <TroykaLedMatrix.h>
#include <Dragster.h>
#include <Octoliner.h>


Dragster dragster;
TroykaLedMatrix matrix;
Octoliner octoliner(42);

//                    center -> edge, right corner
float speeds[4][2] = {{1, 0.7}, {1, 0.5}, {1, 0.3}, {1, 0}};
float motorsSpeed[2];
volatile float sensors[8];
float setSpeedThreeshold = 0.5;

void getSensorValues();
void printSensorValues();
void drawSensorValues();
void printSpeedValues();
void computeSpeed();


void setup()
{
  Serial.begin(9600);
  dragster.begin();
  matrix.begin();
  octoliner.begin();

  //setupInterrupt();
}


void loop()
{
  getSensorValues();
  computeSpeed();
  
  printSensorValues();
  printSpeedValues();
  drawSensorValues();
  Serial.println("\n\n\n");
}



void getSensorValues()
{
  for (int i = 0; i < 8; i++)
    sensors[i] = 1.0 - float(octoliner.analogRead(i)) / 4096.0;
}

void printSensorValues()
{
  Serial.print("Sensors ");
  for (int i = 0; i < 8; i++)
    Serial.print("[" + String(i) + "]: " + String(sensors[i]) + " ");
  Serial.println();
}

void drawSensorValues()
{
  for (int i = 0; i < 8; i++)
    for (int x = 0; x < 8; x++)
      if (sensors[i] >= float(x) / 8.0)
        matrix.drawPixel(7 - x, i);
      else
        matrix.clearPixel(7 - x, i);
  //matrix.customUpdate();
}

void printSpeedValues()
{
  Serial.println("Motors[0]: " + String(motorsSpeed[0]) + " Motors[1]: " + String(motorsSpeed[1]));
}

void computeSpeed()
{
  motorsSpeed[0] = 1.0;
  motorsSpeed[1] = 1.0;
  for (int i = 0; i < 8; i++)
  {
    if (sensors[i] >= setSpeedThreeshold)
    {
      motorsSpeed[0] = speeds[abs(4 - i)][(i < 4) ? 0 : 1];
      motorsSpeed[1] = speeds[abs(4 - i)][(i < 4) ? 1 : 0];
      break;
    }
  }
}



ISR(TIMER1_OVF_vect)
{
  static bool state = false;
  state = !state;
  dragster.led(state);
}












//
