#include <GateOpenerCommunicator.h>

  #define MASTERADDRESS 1
  #define CODELOCKADDRESS 2
  #define GATEADDRESS 3 
  #define NETWORKADDRESS 0
  #define FREQUENCY     RF69_868MHZ
  #define ENCRYPTKEY    "TOPSECRETPASSWRD" 
  #define USEACK        true // Request ACKs or not

Logger logger;
GateOpenerCommunicator communicator;

byte gateLed = 3;

void setup() 
{
  pinMode(3, OUTPUT);
  logger.init();
  communicator.init(FREQUENCY, GATEADDRESS, NETWORKADDRESS, ENCRYPTKEY);
}

void loop() 
{
  if (communicator.receive())
  {
    processMessage();
  }
}

void processMessage()
{
  switch(communicator.getHeader())
  {
    case OPENGATEMSG:
    {
      openGateMsgHandler();
      break;
    }
    default:
    {
      unknownMsgHandler();
      break;
    }  
  }  
}

void openGateMsgHandler()
{
  OpenGateMsg openGateMsg = OpenGateMsg(communicator.RecvMessage, communicator.MessageLength);
  logger.log(openGateMsg, communicator.SenderId, RECV);
  openGate();
}

void openGate()
{
  digitalWrite(gateLed, HIGH);
  delay(2000);
  digitalWrite(gateLed, LOW); 
}

void unknownMsgHandler()
{
  UnknownMsg unknownMsg = UnknownMsg(communicator.RecvMessage, communicator.MessageLength);
  logger.log(unknownMsg, communicator.SenderId);
}
