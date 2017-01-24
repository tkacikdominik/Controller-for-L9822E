#include <SPI.h>
#include <RFM69.h>

  #define MASTERADDRESS 1
  #define CODELOCKADDRESS 2
  #define NETWORKADDRESS 0
  #define FREQUENCY     RF69_868MHZ
  #define ENCRYPTKEY    "TOPSECRETPASSWRD" 
  #define USEACK        true // Request ACKs or not
  RFM69 radio;


/******P I N S********/
const byte relay1 = 11;
const byte relay2 = 3;

/*******C O D E******/ 
const byte massageLength = 10;
const byte maxCodeLength = 8;
const char emptySymbol = 'x';
          
char readCode[maxCodeLength];
char code[maxCodeLength] = {'0','9','0','5','1','9','9','7'};
/**V A R I A B L E**/
boolean check = true;

//Status
byte processStatus;

void setup() 
{

  Serial.begin(9600);
  Serial.print("ArduinoMaster");
  radio.initialize(FREQUENCY, MASTERADDRESS, NETWORKADDRESS);
  radio.encrypt(ENCRYPTKEY);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  resetCode();
  statusReset();
}

void loop() 
{
  switch(processStatus)
  {
    case 0:Serial.println("Prijmam Kod,Status");
    Serial.println(processStatus);
      if (radio.receiveDone())
      {
        Serial.print("message [");
        for (int i = 0; i < radio.DATALEN; i++)
        {
          readCode[i] = (char)radio.DATA[i];
          Serial.print((char)radio.DATA[i]);
        }
        Serial.print("], RSSI ");
        Serial.println(radio.RSSI);
        if (radio.ACKRequested())
        {
          radio.sendACK();
          Serial.println("Prijata sprava"); 
        }
      }
    break;
    
    case 1:Serial.println("Prijmam Kod");
    Serial.println(processStatus);
    if (verifyCode()) statusUp();
    else statusReset;
    break;
  }
}

boolean verifyCode()
{
  for(int i=0;i<maxCodeLength;i++)
  {
    if(readCode[i] != code[i])
    {
      statusReset();
      return false; 
    }
  }
  statusUp();
  resetCode();
  return true;
}

void statusUp()
{
  processStatus=processStatus++;
}

void statusDown()
{
  processStatus=processStatus--;
}

void statusReset()
{
  processStatus=0;
}

void resetCode()
{
  for(byte i=0;i<maxCodeLength;i++)
  {
    readCode[i]=0;  
  }
}
