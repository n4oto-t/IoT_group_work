#include <Servo.h>

//サーボモータ
Servo servo;
#define servoPIN 7
int servoAngle = 0;
bool AngleIsZero = true;


void setup() {
  servo.attach(servoPIN , 500 , 2400);

  Serial.begin(9600);
  delay(1000);
}

void loop() {
    if(Serial.available() >0){
      char receivedByte = Serial.read();
      if (receivedByte =='a'){
        servoAngle = AngleIsZero ? 180 : 0;
        AngleIsZero = !AngleIsZero;

        servo.write(servoAngle);
        delay(1000);
      }
    }
}