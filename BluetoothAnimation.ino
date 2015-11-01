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

const int MOUTH_MIN = 40;//80;
const int MOUTH_MAX = 100;//170;

const int PIN_MOUTH = 7;
const int PIN_EYES_HORIZONTAL = 8;
const int PIN_EYES_VERTICAL = 9;

Servo m_ServoEyesHorizontal, m_ServoEyesVertical;
Servo m_ServoMouthVertical;

int m_IncommingByte;
long m_Command;

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
    
    int motorId = (m_Command & 0x00FF0000) >> 16;
    int motionX = (m_Command & 0x0000FF00) >> 8;
    int motionY = (m_Command & 0x000000FF);
    
    Serial.println();
    // Handle motor motion.
    handleMotors(motorId, motionX, motionY);
    
    m_Command = 0x0000;
    m_CommandEntered = false;
    m_CommandComplete = false;
}

void handleMotors(int motorId, int motionX, int motionY)
{
   if(motorId == 1)
   {
       // This is meant for moving the eyes.
       moveEyes(motionX, motionY);
   } 
   else if(motorId == 2)
   {
      // This is meant for moving the mouth;
      moveMouth(motionY);
      
   }
}

void moveEyes(int horziontalDelta, int verticalDelta)
{
  int horizontalAngle = EYES_HORIZONTAL_MIN 
  + horziontalDelta;
  
  if(horizontalAngle > EYES_HORIZONTAL_MAX)
  {
    horizontalAngle = EYES_HORIZONTAL_MAX;
  }
  else if(horizontalAngle < EYES_HORIZONTAL_MIN)
  {
    horizontalAngle = EYES_HORIZONTAL_MIN;
  }
  
  int verticalAngle = EYES_VERTICAL_MIN
  + verticalDelta;
  
  if(verticalAngle > EYES_VERTICAL_MAX)
  {
    verticalAngle = EYES_VERTICAL_MAX;
  }
  else if (verticalAngle < EYES_VERTICAL_MIN)
  {
    verticalAngle = EYES_VERTICAL_MIN;
  }
  
  m_ServoEyesHorizontal.write(horizontalAngle);
  m_ServoEyesVertical.write(verticalAngle);
  
}

void moveMouth(int verticalDelta)
{
  int motorAngle = MOUTH_MIN + verticalDelta;
  
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


void serialEvent()
{
   handleCommand(); 
}

void handleCommand()
{
  while(Serial.available())
  {
     m_IncommingByte = Serial.read();
  
     m_CommandEntered = true;
     
     if(m_IncommingByte == (int)'\n'
       || m_IncommingByte == (int)'\r')
       {
          m_CommandComplete = true;
       }
       else
       {
          // We have parts of a command.
          m_Command = m_Command << 8;
          m_Command = m_Command | (0x00FF & m_IncommingByte);
       }
  }
}
