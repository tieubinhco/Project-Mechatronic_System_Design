#include <SPI.h>
#define SS3 4
#define SS2 3
#define SS1 2
#define MISO 12
#define MOSI 11
#define SCK 13
#define btn 2
byte spi_receiver_1;
byte spi_receiver_2;
byte spi_receiver_3;
int CB1 = A0;
int CB2 = A1;
int CB3 = A2;
int CB4 = A3;
int CB5 = A4;
int CB6 = A5;
int CB7 = A6;
float S1, S2, S3, S4, S5, S6, S7, e;
float y1_calib, y2_calib, y3_calib, y4_calib, y5_calib, y6_calib, y7_calib;
float in_min[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float in_max[8] = {0, 0, 0, 0, 0, 0, 0, 0};
long speedcar;
byte byteSend = 120;

void setup()
{
  Serial.begin(9600);
  pinMode(SS, OUTPUT);
  pinMode(SS1, OUTPUT);
  pinMode(SS2, OUTPUT);
  pinMode(SS3, OUTPUT);
  pinMode(CB1, INPUT);
  pinMode(CB2, INPUT);
  pinMode(CB3, INPUT);
  pinMode(CB4, INPUT);
  pinMode(CB5, INPUT);
  pinMode(CB6, INPUT);
  pinMode(CB7, INPUT);
  pinMode(btn, INPUT);
  digitalWrite(SS1, HIGH);
  digitalWrite(SS2, HIGH);
  digitalWrite(SS3, HIGH);
  //  SPCR |= _BV(MSTR);
  SPI.begin();
  //attachInterrupt(digitalPinToInterrupt(btn), buttonPress, RISING);
}

int mapping(long speedcar)
{
  return map(speedcar, 0, 4000, 0, 255);
}
void white(void)
{
  in_min[1] = analogRead(CB1);
  delay(50);
  in_min[2] = analogRead(CB2);
  delay(50);
  in_min[3] = analogRead(CB3);
  delay(50);
  in_min[4] = analogRead(CB4);
  delay(50);
  in_min[5] = analogRead(CB5);
  delay(50);
  in_min[6] = analogRead(CB6);
  delay(50);
  in_min[7] = analogRead(CB7);
  delay(50);
}
// getting initial value on black background
void black(void)
{
  in_max[1] = analogRead(CB1);
  delay(50);
  in_max[2] = analogRead(CB2);
  delay(50);
  in_max[3] = analogRead(CB3);
  delay(50);
  in_max[4] = analogRead(CB4);
  delay(50);
  in_max[5] = analogRead(CB5);
  delay(50);
  in_max[6] = analogRead(CB6);
  delay(50);
  in_max[7] = analogRead(CB7);
  delay(50);
}

void calib(void)
{
  int thres = 50;
  S1 = analogRead(CB1);
  S2 = analogRead(CB2);
  S3 = analogRead(CB3);
  S4 = analogRead(CB4);
  S5 = analogRead(CB5);
  S6 = analogRead(CB6);
  S7 = analogRead(CB7);

  y1_calib = 100 * ((S1 - in_min[1]) / (in_max[1] - in_min[1])); //calibrate sensor (based on the formula)
  if (y1_calib > 100)
    y1_calib = 100; //limiting the value
  if (y1_calib < 0)
    y1_calib = 0; //limiting the value
  // if (y1_calib > thres)
  //     digi[1] = 1;
  // else
  //     digi[1] = 0;

  y2_calib = 100 * ((S2 - in_min[2]) / (in_max[2] - in_min[2])); //calibrate sensor (based on the formula)
  if (y2_calib > 100)
    y2_calib = 100; //limiting the value
  if (y2_calib < 0)
    y2_calib = 0; //limiting the value
  // if (y2_calib > thres)
  //     digi[2] = 1;
  // else
  //     digi[2] = 0;

  y3_calib = 100 * ((S3 - in_min[3]) / (in_max[3] - in_min[3])); //calibrate sensor (based on the formula)
  if (y3_calib > 100)
    y3_calib = 100; //limiting the value
  if (y3_calib < 0)
    y3_calib = 0; //limiting the value
  // if (y3_calib > thres)
  //     digi[3] = 1;
  // else
  //     digi[3] = 0;

  y4_calib = 100 * ((S4 - in_min[4]) / (in_max[4] - in_min[4])); //calibrate sensor (based on the formula)
  if (y4_calib > 100)
    y4_calib = 100; //limiting the value
  if (y4_calib < 0)
    y4_calib = 0; //limiting the value
  // if (y4_calib > thres)
  //     digi[4] = 1;
  // else
  //     digi[4] = 0;

  y5_calib = 100 * ((S5 - in_min[5]) / (in_max[5] - in_min[5])); //calibrate sensor (based on the formula)
  if (y5_calib > 100)
    y5_calib = 100; //limiting the value
  if (y5_calib < 0)
    y5_calib = 0; //limiting the value
  // if (y5_calib > thres)
  //     digi[5] = 1;
  // else
  //     digi[5] = 0;

  y6_calib = 100 * ((S6 - in_min[6]) / (in_max[6] - in_min[6])); //calibrate sensor (based on the formula)
  if (y6_calib > 100)
    y6_calib = 100; //limiting the value
  if (y6_calib < 0)
    y6_calib = 0; //limiting the value
  // if (y6_calib > thres)
  //     digi[6] = 1;
  // else
  //     digi[6] = 0;

  y7_calib = 100 * ((S7 - in_min[7]) / (in_max[7] - in_min[7])); //calibrate sensor (based on the formula)
  if (y7_calib > 100)
    y7_calib = 100; //limiting the value
  if (y7_calib < 0)
    y7_calib = 0; //limiting the value
  // if (y7_calib > thres)
  //     digi[7] = 1;
  // else
  //     digi[7] = 0;
}

void sendData(int sending, int slave)
{
  switch (slave)
  {
  case 2:
  {
    digitalWrite(SS2, LOW);
    spi_receiver_2 = SPI.transfer(sending);
    digitalWrite(SS2, HIGH);
    break;
  }
  case 3:
  {
    digitalWrite(SS3, LOW);
    spi_receiver_2 = SPI.transfer(sending);
    digitalWrite(SS3, HIGH);
    break;
  }
  }
}

float e_thres = 0.5;
//Lyapunov parameter
float k1 = 1;
float k2 = 450;
float k3 = 0;

float v, w, v_current;
float v_ref = 0.3; //rpm
float w_ref = 0;
// Calculate e2 and find V and W
//void vomegaref() {
// e2_prev=e2;
//   e2=((3*(y7_calib-y1_calib)+2*(y6_calib-y2_calib)+(y5_calib-y3_calib))/(y1_calib+y2_calib+y3_calib+y4_calib+y5_calib+y6_calib+y7_calib))*17;
//   if (e2 >-(e_thres) && e2 <(e_thres)) e2=0;
//
//
////   //when the sensor are completely off the line, we will get weird error value, to fix it, we use this pseudocode
////   if ( digi[3]==1 || digi[4]==1 || digi[5]==1 )
////   {
////      if (e2>0) dir =1;
////      else dir =-1;
////   }
////   else if (dir == 1 ) e2=34;
////   else e2=-34;
//  // calculate e3
//   //v_current = (r/2)*(rpm1+rpm2);
//   //e3=atan((e2-e2_prev)/(v_current*0.02));
//
//    //path tracking lyapunov-based control
//        v = v_ref*cos(e3) + k1*e1;
//        w = k2*e2*v_ref*0.001 + k3*sin(e3) + w_ref;
//  }

int c = 0;

void loop()
{
  if (digitalRead(btn) == 1 && c == 0)
  {
    delay(500);
    c == 1;
    for (int i = 0; i <= 50; i++) //chay thang
    {
      sendData(125, 2);
      sendData(87, 3);
      delay(50);
    }

    for (int i = 0; i <= 20; i++) //queo phai
    {
      sendData(125, 2);
      sendData(110, 3);
      delay(50);
    }

    for (int i = 0; i <= 30; i++) //chay xeo
    {
      sendData(125, 2);
      sendData(90, 3);
      delay(50);
    }

    for (int i = 0; i <= 50; i++) //cong trai
    {
      sendData(125, 2);
      sendData(60, 3);
      delay(50);
    }
  }

  sendData(0, 2);
  sendData(90, 3);
}