#include <SPI.h>

#define SCK  PB3
#define MISO PB4
#define MOSI PB5
#define SS   PA15

int CB1=PA0;
int CB2=PA1;
int CB3=PA2;
int CB4=PA3;
int CB5=PA4;
int CB6=PA5;
int CB7=PA6;
float S1, S2, S3, S4, S5, S6, S7, e;

void setupSPI(void)
{
  // The clock value is not used
  // SPI1 is selected by default
  // MOSI, MISO, SCK and NSS PINs are set by the library
  SPI.beginTransactionSlave(SPISettings(18000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(CB1, INPUT);
  pinMode(CB2, INPUT);
  pinMode(CB3, INPUT);
  pinMode(CB4, INPUT);
  pinMode(CB5, INPUT);
  pinMode(CB6, INPUT);
  pinMode(CB7, INPUT);
  setupSPI();
}


void loop() {
  // put your main code here, to run repeatedly:
  float S1= analogRead(CB1);
  float S2= analogRead(CB2);
  float S3= analogRead(CB3);
  float S4= analogRead(CB4);
  float S5= analogRead(CB5);
  float S6= analogRead(CB6);
  float S7= analogRead(CB7);

// calib sensor
  S1 = 110 + ((950-140)/(964-150))*(155-150);
  S2 = 110 + ((950-140)/(950-128))*(145-128);
  S3 = 110 + ((950-140)/(960-148))*(142-148);
  S4 = 110 + ((950-140)/(964-147))*(152-147);
  S5 = 110 + ((950-140)/(954-127))*(141-127);
  S6 = 110 + ((950-140)/(951-127))*(132-127);
  S7 = 110 + ((950-140)/(941-147))*(155-147);

      e  = (3*(S7-S1)+2*(S6-S2)+(S5-S3))/(S1+S2+S3+S4+S5+S6+S7); // thuật toán tính e

      if ((S1+S7) > 2000 )                                        //gặp ngã giao
          e = 0;                                                  // master quẹo phải trong ... giây
      else if ((S4+S5+S6+S7) > 4100 && S6 > 900)                  // gặp ngã ba 
          e = 1;                                                  // master cho đi thẳng ... giây khi gặp ngã ba
      else if ((S1+S2+S3+S4+S5+S6+S7) > 100  )                    //gặp ngã giao lần 2
          e = 2;                                                  // master cho đi thẳng ... giây khi gặp ngã giao lần 2
      else if ((S1+S2+S3+S4) > 4100 && S1 > 900)                  //gặp ngã giao lần 3
          e = 3;                                                  // master cho xe đi thẳng ... giây
      else if ((S1+S2+S3+S4+S5+S6+S7) > 100  )                    // gặp giao lộ lần cuối
          e = 4;                                                  // xe quẹo trái ... giây rồi đi về đích
    
  Serial.println(S1);
}
