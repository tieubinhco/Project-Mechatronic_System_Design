#include <Servo.h>

Servo myservo;
int servoPin = 4;       // Khai báo chân điều khiển servo

void setup ()
{
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void loop ()
{
  Serial.println("----");
  Serial.print(millis());
  Serial.println("----");
  int servoPos[19] = {0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180};

  for (int i=0;i<19;i++)
  {
    myservo.write(servoPos[i]);
    Serial.print(myservo.read());
    Serial.print("\t");
    Serial.print(millis());
    Serial.println();
  }
  delay(10000);
  /*for (servoPos = 0; servoPos <= 180; servoPos += 5)
  {
    myservo.write(servoPos);
    Serial.println(myservo.read());
    //delay(50);
  }

  for (servoPos = 180; servoPos >= 0; servoPos -= 5)
  {
    myservo.write(servoPos);
    Serial.println(myservo.read());
    //delay(50);
  }*/
}
