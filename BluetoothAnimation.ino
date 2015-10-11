#include <Servo.h>

/**
* Project Name: Bluetooth Animatronics
* Project Type: Halloween
* Date: October 2015
*
*
* Allows controlling the eyes are mouth of 
* a simple animatronic via Bluetooth Android App.
*
* Command Format:
* Length 3 Bytes + Terminator Byte (\n or \r)
* Byte # 3: Motor ID ( 1 for Eyes, 2 for Mouth)
* Byte # 2: Motion X Component (Between 0-200)
* Byte # 1: Motion Y Component (Between 0-200) 
**/

const int EYES_HORIZONTAL_MIN = 40;
const int EYES_HORIZONTAL_MAX = 130;

const int EYES_VERTICAL_MIN = 60;
const int EYES_VERTICAL_MAX = 115;

const int MOUTH_MIN = 0;
const int MOUTH_MAX = 170;

const int PIN_EYES_HORIZONTAL = 7;
const int PIN_EYES_VERTICAL = 8;
const int PIN_MOUTH = 9;

Servo m_ServoEyesHorizontal, m_ServoEyesVertical;
Servo m_ServoMouthVertical;

int m_IncommingByte;
int m_Command;

int m_CommandEntered, m_CommandComplete;

void setup() 
{
  m_Command = 0x0000;
  initializeServos();
  
  Serial.begin(9600);
  while(!Serial)
  {
    
  }
  Serial.println("Serial Ready");
  
}

void initializeServos()
{
  m_ServoEyesHorizontal.attach(PIN_EYES_HORIZONTAL);
  m_ServoEyesVertical.attach(PIN_EYES_VERTICAL);
  m_ServoMouthVertical.attach(PIN_MOUTH);

  // Look Left Max - 40
  // Center 90
  // Look Right Max - 130
  m_ServoEyesHorizontal.write(90);
  // up max - 115
  // Center 90
  // down max - 60
  m_ServoEyesVertical.write(90);
  m_ServoMouthVertical.write(90);  
}

void loop() 
{
    if(!m_CommandComplete)
    {
      return;
    }
    
    if(!m_CommandEntered)
    {
       return; 
    }
    
    int motorId = (m_Command & 0x0F00) >> 16;
    int motionX = (m_Command & 0x00F0) >> 8;
    int motionY = (m_Command & 0x000F);
    
    // Handle motor motion.
    handleMotors(motorId, motionX, motionY);
    
    m_Command = 0x0000;
    m_CommandEntered = false;
    m_CommandComplete = false;
}

void handleMotors(int motorId, int motionX, int motionY)
{
   int motorAngle = 90;
   if(motorId = 1)
   {
       // This is meant for moving the eyes.
   } 
   else if(motorId == 2)
   {
      // This is meant for moving the mouth;
      if(motorAngle >  MOUTH_MAX)
      {
         motorAngle = MOUTH_MAX; 
      }
      else if(motorAngle < MOUTH_MIN)
      {
         motorAngle = MOUTH_MIN; 
      }
      
      m_ServoMouthVertical.write(motorAngle);
      
   }
}

void serialEvent()
{
   handleCommand(); 
}

void handleCommand()
{
  while(Serial.available())
  {
     m_IncommingByte = Serial.read();
     
     if(m_IncommingByte == (int)'\n'
       || m_IncommingByte == (int)'\r')
       {
          m_CommandComplete = true;
       }
       else
       {
          // We have parts of a command.
          m_Command = m_Command << 8;
          m_Command = m_Command | m_IncommingByte;
          break; 
       }
  }
}
