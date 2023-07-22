5.3 Code for Room Microcontroller :
#include <SoftwareSerial.h>
SoftwareSerial HC12(2, 3);
#include "LiquidCrystal.h"
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
int L = 4;
int F = 5;
int AC_F = 6;
float R1 = 30000.0;
float R2 = 7500.0;
float R3 = 30000.0;
float R4 = 7500.0;
float R5 = 30000.0;
float R6 = 7500.0;
float average = 0.0;
int buttonPushCounter = 0;
int pushState = 0;
int lastpushState = 0;
const int push = 7;
float analog, analog1, analog2, analog3, vOUT1, vOUT2, vOUT;
float vIN, vIN1, vIN2;
int power, power1, power2, power3;
int digital, digital1, digital2, digital3, value, value2, value1, value3;
int p;
char rcv;
int x, y, z;
void L_ON();
void AC_F_ON();
void F_ON();
void R_coler_ON();
void L_OFF();
void AC_F_OFF();
void F_OFF();
void R_coler_OFF();
void Power();
void calculation();

int decimalPrecision = 2;
float voltageSampleRead  = 0;
float voltageLastSample  = 0;
float voltageSampleSum   = 0;
float voltageSampleCount = 0;
float voltageMean ;
float RMSVoltageMean ;
float adjustRMSVoltageMean;
float Voltage;
float voltageOffset1 = 0.00 ;
float voltageOffset2 = 0.00;
const int sensorIn = A3;
double voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;


void setup() {

  Serial.begin(9600);
  HC12.begin(9600);
  lcd.begin(16, 2);
  pinMode(L, OUTPUT);
  digitalWrite(L, HIGH);
  pinMode(AC_F, OUTPUT);
  digitalWrite(AC_F, HIGH);
  pinMode(F, OUTPUT);
  digitalWrite(F, HIGH);
  pinMode(push, INPUT);

}

void loop()
{
  calculation();
  if (HC12.available() > 0)
  { rcv = HC12.read();
    Serial.println(rcv);
    switch (rcv)
    {
      case 'a':
        L_ON();
        break;

      case 'b':
        L_OFF();
        break;

      case 'd':
        AC_F_OFF();
        break;

      case 'c':
        AC_F_ON();
        break;

      case 'f':
        F_OFF();
        break;

      case 'e':
        F_ON();
        break;

    }
  }
  delay(200);

  disp();
  Power();

}
void L_OFF()
{
  delay(100);
  x = digitalRead(L);
  if (x == LOW)
  {
    HC12.write(2);
    digitalWrite (L, HIGH);
  }

}
void L_ON()
{
  delay(100);
  if (digitalRead(L) == HIGH)
  {
    HC12.write(1);
    digitalWrite (L, LOW);
  }

}
void AC_F_OFF()
{
  delay(100);
  y = digitalRead(AC_F);
  if (y == LOW)
  {
    HC12.write(4);
    digitalWrite (AC_F, HIGH);
  }

}
void AC_F_ON()
{
  delay(100);
  if (digitalRead(AC_F) == HIGH)
  {
    HC12.write(3);
    digitalWrite (AC_F, LOW);
  }

}
void F_OFF()
{
  delay(100);
  z = digitalRead(F);
  if (z == LOW)
  {
    HC12.write(6);
    digitalWrite (F, HIGH);
  }
}
void F_ON()
{
  delay(100);
  if (digitalRead(F) == HIGH)
  {
    HC12.write(5);
    digitalWrite (F, LOW);
  }
}


void calculation()
{
  digital = analogRead(A0);                  // L1
  analog = (digital - 512) / 20.48;
  value = analogRead(A1);                   //voltage sensor
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2 / (R1 + R2)) - 1;
  power = analog * vIN;



  digital1 = analogRead(A2);                 //F
  analog1 = (digital1 - 512) / 20.48;



  value1 = analogRead(A3);                   //voltage sensor
  vOUT1 = (value1 * 5.0) / 1024.0;
  vIN1 = vOUT1 / (R4 / (R3 + R4));
  power1 = analog1 * vIN1;



  digital2 = analogRead(A4);                 //AC1
  analog2 = ((digital2 - 512) / 20.48) * (-1);



  if (Voltage <= 40)
  {
    Voltage = 0;
  }
  voltageSampleSum = 0;
  voltageSampleCount = 0;

}
}

power2 = Voltage * analog2;

}


void Power()
{
  if (digitalRead(L) == LOW)
  { delay(2);
    HC12.write(power);
  }

  if (digitalRead(AC_F) == LOW)
  { delay(2);
    HC12.write(power2);
  }

  if (digitalRead(F) == LOW)
  {
    delay(2);
    HC12.write(power1);

  }
}

void disp()
{

  pushState = digitalRead(push);

  if (pushState != lastpushState)
  {
    if (pushState == HIGH)
    {
      buttonPushCounter++;
    }
  }
  lastpushState = pushState;
  if (buttonPushCounter > 3)
  {
    buttonPushCounter = 0;
    lcd.clear();
  }
  else if (buttonPushCounter == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("I1=");
    lcd.print(analog);
    lcd.setCursor(9, 0);
    lcd.print("v1=");
    lcd.print(vIN);
    lcd.setCursor(0, 1);
    lcd.print("Power1=");
    lcd.print(power);
    lcd.print("W");
  }
  else if (buttonPushCounter == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("I2=");
    lcd.print(analog1);
    lcd.setCursor(9, 0);
    lcd.print("V2=");
    lcd.print(vIN1);
    lcd.setCursor(0, 1);
    lcd.print("Power2=");
    lcd.print(power1);
    lcd.print("W");
  }
  else if (buttonPushCounter == 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("I3=");
    lcd.print(analog2);
    lcd.print("A");
    lcd.setCursor(9, 1);
    lcd.print("V3=");
    lcd.setCursor(11, 1);
    lcd.print(Voltage);
    lcd.print("V");
    lcd.setCursor(0, 1);
    lcd.print("Power3=");
    lcd.print(power2);
    lcd.print("W");
  }
}
