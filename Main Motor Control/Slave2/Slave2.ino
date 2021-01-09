#include <SPI.h>
#include <PID_v1.h>
#include <Encoder.h>

#define PWM 5
#define STBY 4
#define A 2 //Encoder A
#define B 3 //Encoder B
#define AIN1 8
#define AIN2 7
#define MISO 12
#define MOSI 11
#define SCK 13
#define SS 10

//Declare encoder function
Encoder Enc(A, B);

//Time variables
long previousMillis = 0;
long currentMillis = 0;

//Encoder variables
volatile long currentEncoder;
volatile long previousEncoder = 0;
volatile long oldPosition = 0;
volatile long newPosition;

long positionMain  = -999;
int rotation = 0;
float old_rot_speed;
const int Encoder_1_round = 44; //define number of pulses in one round of encoder

float rot_speed;           //rotating speed in rad/s
const int interval = 5; //choose interval is 1 second (1000 milliseconds)

//Set PID constants
double kp = 0.00545, ki = 0.0352, kd = 0;  //0.0096;
double input = 0, output = 0, setpoint = 0;
const int K = 13117;

//Declare PID functions
PID MainPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);

long spi_receiver=0;
void PID_setup(void)
{
  //PID initial settings
  MainPID.SetMode(AUTOMATIC);
  MainPID.SetSampleTime(0.005);
  MainPID.SetOutputLimits(0, 255);
}
float read_speed(void)
{
  //read velocity of selected motor
  //return velocity in rad/s
  currentEncoder = Enc.read();
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    rot_speed = (float)((currentEncoder - previousEncoder) * 200 * 60 / (Encoder_1_round));
    previousEncoder = currentEncoder;
    return rot_speed;
  }
}
void w(int rotation, int direct)
{
  //Control rotation of motor
  //variable "rotation" gets value from 0 (0% power) to 255 (100% power)
  analogWrite(PWM, rotation);
  if (direct == 1)
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }
  else if (direct == -1)
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  if (rotation == 0) //Floating
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    analogWrite(PWM, 0);
  }
  if (direct == 0) //Brake
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWM, 0);
  }
}

void control_PID(float u)
{
  //control the wheel using PID
  //Get sign of rotating velocity of wheels
  u = abs(u);              //get the absolute value of input speed (because we have the variable u_sign)
  setpoint = u;
  MainPID.Compute();
  w(output, 1);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600); //Set the band rate to your Bluetooth module.
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(A, INPUT_PULLUP);
  pinMode(B,  INPUT_PULLUP);
  digitalWrite(STBY, HIGH);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);  //turn on SPI in slave mode
  SPCR |= _BV(SPIE); // turn on interrupts
  SPI.attachInterrupt;
  PID_setup();
}
ISR(SPI_STC_vect)
{
  spi_receiver=map(SPDR,0,255,0,3000);
}
void loop()
{
  input = read_speed();
  control_PID(spi_receiver);

  //Serial.println(spi_receiver);
  if (digitalRead(SS) == HIGH)
  {
    SPDR = 0;
  }

}
