#include "LiquidCrystal_I2C.h"
#include <SoftwareSerial.h >
#include <DS1302.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <SoftwareSerial.h>
SoftwareSerial Serial2(11, 7); // RX, TX


DS1302 rtc(4, 5, 6);
SoftwareSerial HC12(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);               // Address, SDA, SCL
const int voltageSensor  = A0;
float vOUT = 0.0;
float vIN = 0.0;
int batC = 0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;

void L_ON();
void DC_F_ON();
void F_ON();
void R_coler_ON();
void L_OFF();
void DC_F_OFF();
void F_OFF();
void R_coler_OFF();
void AC_F_ON();
void AC_F_OFF();


Time t1, t2, t3, t4, t5, t6;
int timeend1, timestart1, ontime, ontime1, timeend2, timestart2, ontime2, timeend3, timestart3, ontime3, y;
int rcv, x1, x2;
int lastbatC = 0;
int lastButtonState1 = 0;
int lastButtonState2 = 0;
int b1 = 0;
int b2 = 0;
const int btn1 = 10;  //10
const int btn2 = 9; //9
const int grid = 12;
const int solar = 13;
int lasty;
int p3, p2, p1, p;
float E1, E2, E3;

int buttonPushCounter = 0;
int pushState = 0;
int lastpushState = 0;
const int push = 8;// previous on 11
//const int chipSelect = 7;
const int currentPin = A1;
int sensitivity = 100;
int adcValue = 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double current = 0;
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
int lastx1, lastx2;

void setup()
{
  lcd.init();
  lcd.clear();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("USMAN ALI (MANI)");
  lcd.setCursor(0, 1);
  lcd.print("Shams Ul Islam");
  Serial2.begin(115200);  //New communication
  Serial.begin(115200);
  HC12.begin(9600);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(grid, OUTPUT);
  pinMode(solar, OUTPUT);

}

void loop()
{


  value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2 / (R1 + R2));
  if (vIN < 10.89)
  {
    batC = 0;
  }

  else if (vIN >= 10.89 && vIN < 11.30)
  {
    batC = 10;
  }

  else if (vIN > 11.29 && vIN < 11.70)
  {
    batC = 20;
  }

  else if (vIN > 11.69 && vIN < 12.10 )
  {
    batC = 30;
  }

  else if (vIN > 12.09 && vIN < 12.50)
  {
    batC = 40;
  }

  else if (vIN > 12.49 && vIN < 12.90)
  {
    batC = 50;
  }

  else if (vIN > 12.89 && vIN < 13.30)
  {
    batC = 60;
  }

  else if (vIN > 13.29 && vIN < 13.70)
  {
    batC = 70;
  }

  else if (vIN > 13.69 && vIN < 14.10)
  {
    batC = 80;
  }

  else if (vIN > 14.09 && vIN < 14.50 )
  {
    batC = 90;
  }


  else if (vIN >= 14.50)
  {
    batC = 100;
  }

  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  current = ((adcVoltage - offsetVoltage ) / sensitivity);
  if (current >= 4)
  {
    if (batC < 30)
    {
      digitalWrite(grid, HIGH);
      digitalWrite(solar, HIGH);
    }
    else if (batC > 50)
    {
      digitalWrite(grid, LOW);
      digitalWrite(solar, LOW);
    }
  }

  if (current < 4)
  {
    if (batC < 30)
    {
      digitalWrite(grid, HIGH);
    }
    else if (batC > 30)
    {
      digitalWrite(solar, LOW);
    }
  }

  for (int i = 0; i < 100; i++)
  { if (micros() >= voltageLastSample + 100 )
    {
      voltageSampleRead = (analogRead(A2) - 512) + voltageOffset1;
      voltageSampleSum = voltageSampleSum + sq(voltageSampleRead) ;

      voltageSampleCount = voltageSampleCount + 1;
      voltageLastSample = micros() ;
    }
    if (voltageSampleCount == 100)
    {
      voltageMean = voltageSampleSum / voltageSampleCount;
      RMSVoltageMean = (sqrt(voltageMean)) * 1.5;
      adjustRMSVoltageMean = RMSVoltageMean + voltageOffset2;
      Voltage = RMSVoltageMean + voltageOffset2;
      if (Voltage <= 40)
      {
        Voltage = 0;
      }
      voltageSampleSum = 0;
      voltageSampleCount = 0;
    }
  }

  b1 = digitalRead(btn1);
  b2 = digitalRead(btn2);
  // Serial.println(b2);
  //Serial.println(b1);




  //button code
  if (b1 != lastButtonState1 || b2 != lastButtonState2  )
  {
    if (b1 == LOW && b2 == LOW)
    {
      y = 1;
    }
    //flag1=0;}
    else if (b1 == LOW && b2 == HIGH)
    {
      y = 2;
    }
    //  flag2=0;}
    else if (b1 == HIGH && b2 == LOW)
    {
      y = 3;
    }
    //flag3=0;}
    else if (b1 == HIGH && b2 == HIGH)
    {
      y = 4;
    }
    //flag4=0;}


    lastButtonState2 = b2;
    lastButtonState1 = b1;
  }


  if (y != lasty)
  {
    if (y == 1)

    {
      L_ON();
      AC_F_ON();
      F_ON();
    }
    else if (y == 2)
    {
      F_OFF();
    }

    else if (y == 3)
    {
      L_OFF();
    }

    else if (y == 4)
    {
      AC_F_OFF();
    }

    lasty = y;
  }

  //battery percentage
  if (Voltage >= 130)                      //Grid is available
  {
    x1 = 1;

    if (batC > 20)
    {
      x2 = 1;
    }
  }

  if (x1 != lastx1 || x2 != lastx2)
  {
    if (x1 == 1)
    {
      F_ON();
      R_coler_ON();
    }
    if (x2 == 1)
    {
      L_ON();
      DC_F_ON();
    }

    lastx1 = x1;
    lastx2 = x2;
  }


  if (Voltage < 130)               //Grid isn't available
  {
    if (batC > 80)
    { y = 1;

    }
    else if (batC >= 50 && batC < 81)
    { y = 2;

    }
    else if (batC >= 20 && batC < 50)
    { y = 3;

    }
    else if (batC < 20)
    { y = 4;

    }
  }
}

if (y != lasty)
{
  if (y == 1)

  {
    L_ON();
    AC_F_ON();
    F_ON();
    //R_coler_ON();
  }

  else if (y == 2)
  {
    F_OFF();
  }
  else if (y == 3)
  {
    L_OFF();
  }
  else if (y == 4)
  {
    AC_F_OFF();
  }
  lasty = y;
}
* /



if (HC12.available() > 0)
{ rcv = HC12.read();
  Serial.println(rcv);
  switch (rcv)
  {
    case 1:
      t1 = rtc.getTime();
      timestart1 = ((t1.hour * 60 * 60) + (t1.min * 60) + t1.sec);
      delay(5);
      break;
    case 2:
      t2 = rtc.getTime();
      timeend1 = ((t2.hour * 60 * 60) + (t2.min * 60) + t2.sec);
      delay(1);
      ontime1 = timeend1 - timestart1;
      E1 = p1 * ontime1;
      E1 = E1 / 3600;
      break;
    case 3:
      t3 = rtc.getTime();
      timestart2 = ((t3.hour * 60 * 60) + (t3.min * 60) + t3.sec);
      delay(5);
      break;

    case 4:
      t4 = rtc.getTime();
      timeend2 = ((t4.hour * 60 * 60) + (t4.min * 60) + t4.sec);
      delay(1);
      ontime2 = timeend2 - timestart2;
      E2 = p2 * ontime2;
      E2 = E2 / 3600;
      break;
    case 5:
      t5 = rtc.getTime();
      timestart3 = ((t5.hour * 60 * 60) + (t5.min * 60) + t5.sec);
      delay(5);
      break;
    case 6:
      t6 = rtc.getTime();
      timeend3 = ((t6.hour * 60 * 60) + (t6.min * 60) + t6.sec);
      delay(1);
      ontime3 = timeend3 - timestart3;
      E3 = p3 * ontime3;
      E3 = E3 / 3600;
      break;
    default :
      p = rcv;
  }
}
delay(100);
if (p >= 8 && p <= 15)
{
  p1 = p;
}

else if (p >= 95 && p <= 120)
{
  p3 = p;
}

else if (p >= 18 && p <= 30)
{
  p2 = p;
}

disp();

Serial2.print(vIN);// new1
Serial2.print(",");
Serial2.print(batC);  //new2
Serial2.print(",");
Serial2.print(E1);  //new3
Serial2.print(",");
Serial2.print(E2);  //new4
Serial2.print(",");
Serial2.print(E3);  //new5
Serial2.print(",");

delay(1000);
//}

}

void L_ON()        {
  HC12.write('a');
}
void AC_F_ON()     {
  HC12.write('c');
}
void F_ON()        {
  HC12.write('e');
}
void R_coler_ON()  {
  HC12.write('g');
}
void L_OFF()       {
  HC12.write('b');
}
void AC_F_OFF()    {
  HC12.write('d');
}
void F_OFF()       {
  HC12.write('f');
}
void R_coler_OFF() {
  HC12.write('h');
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
  if (buttonPushCounter > 4)
  {
    buttonPushCounter = 0;
    //lcd.clear();
  }
  else if (buttonPushCounter == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Voltage= ");
    lcd.print(vIN);

    lcd.setCursor(0, 1);
    lcd.print("Battery =");
    lcd.print(batC);
    lcd.print("%");

  }
  else if (buttonPushCounter == 2)     //L_ON
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time1=");
    lcd.print(ontime1);
    lcd.setCursor(10, 0);
    lcd.print("P1=");
    lcd.print(p1);
    lcd.setCursor(0, 1);
    lcd.print("E1=");
    lcd.print(E1);
    lcd.print("WHr");

  }
  else if (buttonPushCounter == 3)       //AC_Fan
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time2=");
    lcd.print(ontime2);
    lcd.setCursor(10, 0);
    lcd.print("P2=");
    lcd.print(p2);
    lcd.setCursor(0, 1);
    lcd.print("E2=");
    lcd.print(E2);
    lcd.print("WHr");

  }
  else if (buttonPushCounter == 4)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time3=");
    lcd.print(ontime3);
    lcd.setCursor(10, 0);
    lcd.print("P3=");
    lcd.print(p3);
    lcd.setCursor(0, 1);
    lcd.print("E3=");
    lcd.print(E3);
    lcd.print("WHr");


  }
}
