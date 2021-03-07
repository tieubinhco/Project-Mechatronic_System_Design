#include <Encoder.h>

#define IN1 8
#define IN2 7
#define PWM 5
#define EN_A 2
#define EN_B 3
#define STBY 4

//Declare encoder function
Encoder Enc(EN_A, EN_B);

//Time variables
long previousMillis = 0;
long currentMillis = 0;

//Encoder variables
volatile long currentEncoder;
volatile long previousEncoder = 0;
volatile long oldPosition = 0;
volatile long newPosition;

long positionMain  = -999;
int rotation=0;
float old_rot_speed=0;
const int Encoder_1_round = 44; //define number of pulses in one round of encoder
    
    
float rot_speed;           //rotating speed in rad/s
const int interval = 5; //choose interval is 1 second (1000 milliseconds)

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(57600); //Set the band rate to your Bluetooth module.
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(EN_A, INPUT_PULLUP);
  pinMode(EN_B,  INPUT_PULLUP); 
  Serial.println("Speed Test");
}
/*float read_speed(void)
{
    //read velocity of selected motor
    //return velocity in rad/s
  
    currentEncoder = Enc.read();
    currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        rot_speed = (float)((currentEncoder - previousEncoder)*100*60/(Encoder_1_round));
        previousEncoder = currentEncoder;
        return rot_speed;
    }
}*/
void w(int rotation, int direct)
{
    //Control rotation of motor
    //variable "rotation" gets value from 0 (0% power) to 255 (100% power)
    analogWrite(PWM, rotation);
    if (direct == 1)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
    else if (direct == -1)
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }
}

void loop() {
digitalWrite(STBY,HIGH);
w(255,1);

    //read velocity of selected motor
    //return velocity in rad/s
  
    currentEncoder = Enc.read();
    currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        rot_speed = (float)((currentEncoder - previousEncoder)*200*60/(Encoder_1_round));
        previousEncoder = currentEncoder;
        Serial.print("Speed=\t");
        Serial.print(rot_speed);
        Serial.print("\t");
        Serial.print(millis());
        Serial.println();
    }
//float new_rot_speed;
//new_rot_speed=read_speed();
//if (new_rot_speed != old_rot_speed) {
//Serial.print("Speed=\t");
//Serial.print(new_rot_speed);
//Serial.print("\t");
//Serial.print(millis());
//Serial.println();
//old_rot_speed = new_rot_speed;
}
