#include <BlynkSimpleEsp32.h>
#define RXp2 16
#define TXp2 17
//#include <SoftwareSerial.h>


char auth[] = "Q-NryPXTgUZWmJLnxcKVMUE81LGCRn_f";

//SoftwareSerial mySerial(D5, D6); // RX, TX

void setup() {
  // put your setup code here, to run once:
 // Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);
   Blynk.begin(auth, "789", "123456789");
}

void loop(){


  if (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n');
    float vIN = data.substring(0, data.indexOf(',')).toInt();
    data = data.substring(data.indexOf(',') + 1);
    float batC = data.substring(0, data.indexOf(',')).toInt();
    data = data.substring(data.indexOf(',') + 1);
   float E1 = data.substring(0, data.indexOf(',')).toInt();
    data = data.substring(data.indexOf(',') + 1);
    float E2 = data.substring(0, data.indexOf(',')).toInt();
    data = data.substring(data.indexOf(',') + 1);
    float E3 = data.substring(0, data.indexOf(',')).toInt();

     
    Blynk.virtualWrite(V0, vIN);
    Blynk.virtualWrite(V1, batC);
    Blynk.virtualWrite(V2, E1);
    Blynk.virtualWrite(V3, E2);
    Blynk.virtualWrite(V4, E3);

  }

  Blynk.run();
}
}
