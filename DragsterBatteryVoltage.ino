#include "TroykaLedMatrix.h"
#include "Wire.h"

TroykaLedMatrix matrix;


int getDigit(int num, int i)
{
  for (int j = 0; j < i; j++)
    num /= 10;
  return num % 10;
}

 
void setup() {
  matrix.begin();
  matrix.selectFont(FONT_8X8_BASIC);
  Serial.begin(9600);
  pinMode(A1, INPUT);
}
 
void loop() {
  int adcValue = analogRead(A1);
  float V = adcValue * 5.0 / 1023.0;
  float Vbat = V * (R1 + R2) / R2;

  Serial.println("Battery voltage: " + String(Vbat));
 
  // 00,00
  // 1200 -> 1200 % 100 = 12 -> 12 - n %
  int n = Vbat * 100.0;
  for (int i = 3; i >= 0; i--)
  {
    int digit = getDigit(n, i);
    if (i > 2 && digit == 0)
      continue;
    Serial.println(String(n) + " " + String(digit));
    matrix.drawSymbol('0' + digit);
    delay(300);
    matrix.clear();
    delay(100);

    if (i == 2)
    {
      matrix.drawSymbol(',');
      delay(300);
      matrix.clear();
      delay(100);
    }
  }
  delay(3000);
}





//
