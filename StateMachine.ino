//DOME CALIBRATION- State Machine 

# define BUFFER_SIZE 8

enum States
{
  IDLE_STATE,
  CALIBRATION_STATE,
  END_CALIBRATION_STATE,
  MOTOR_TURNING_STATE,
  END_MOTOR_TURNING_STATE 
};

typedef struct
{
  int ldrPin;
  int count; 
  bool lastState;
  char counterName;
}SensorInfo;

typedef struct
{
  float startingRelativePosition;
  float targetRelativePosition;
  int motorTurningDirection;
  int gearCountsToTarget;
}MotorMovementInfo;

typedef struct
{
  char bufferData[BUFFER_SIZE];
  int index;
  bool hasData;
}CommandInfo;

typedef struct
{
  float realativePosition;   // how many degrees to move from current position
  float absolutePosition;    //between 0-359 degrees
  float gearCountPerDomeDegree;
}DomeInfo;

SensorInfo counterGear;
SensorInfo counterDome;
CommandInfo commandInfo;
DomeInfo domeInfo;
MotorMovementInfo motorMovementInfo;

bool calculateCurrentState(int ldrStatus);
void updateCounter(SensorInfo* counter);
States currentState;
void readCommands();
bool updateMovement();
bool updateDeceleration();
void clearCommands();
void handleCalibrateCommand();
void handleMovementCommand();
void handleGetPositionCommand();
void handleParkCommand();

const int SENSITIVITY_THRESHOLD_MIN = 550; // The value that we'll consider to be high or low for the photoresistor
const int SENSITIVITY_THRESHOLD_MAX = 900;  //Dead zone is between threshold MIN and MAX
int gearPerDomeDegree;

float countsToTurn;
bool motorDirection;

void setup() 
{
  Serial.begin(9600);   //speed in bits per second, talking to USB cable
  counterGear.ldrPin = A0;
  counterGear.count = 0;
  counterGear.counterName = 'G'; //G for Gear
  counterDome.ldrPin = A1;
  counterDome.count = 0;
  counterDome.counterName = 'D';  //D for Dome
  
  pinMode(counterGear.ldrPin, INPUT); //initialize photoresistor on gear
  pinMode(counterDome.ldrPin, INPUT);  //initialize photoresistor on dome

  counterGear.lastState = calculateCurrentState(analogRead(counterGear.ldrPin));
  counterDome.lastState = calculateCurrentState(analogRead(counterDome.ldrPin));  

  currentState = IDLE_STATE;
  
}

void loop() 
{
 switch(currentState)
 {
  case IDLE_STATE:
  {
    serialEvent();
    if(commandInfo.hasData == true)
    {
      clearCommands();
      Serial.println("I made it here 2!");
    }
    break;
  }
  case CALIBRATION_STATE:
    {
       updateCounter(&counterDome);
       if(counterDome.count > 1)
       {
        currentState = END_CALIBRATION_STATE;
       }
       
       if(counterDome.count == 1)
       {
        updateCounter(&counterGear);
       }
      break;
    }
  case END_CALIBRATION_STATE:
  {
    gearPerDomeDegree = float(counterGear.count)/360.0f;  //number of 1/3 turns per one degree of dome turning 
    Serial.print("Gear 1/3 Rotations per One Degree of Dome Rotation: ");
    Serial.println(gearPerDomeDegree);
    currentState = IDLE_STATE;
    break; 
  }
  case MOTOR_TURNING_STATE:
  {
    break;
  }
 }

}

bool calculateCurrentState(int ldrStatus)
{
  return (ldrStatus < SENSITIVITY_THRESHOLD_MIN);
}

void updateCounter(SensorInfo* counter)
{
   int ldrStatus = analogRead(counter->ldrPin);   //reads status of LDR value
   Serial.print(counter->counterName);
   Serial.print(" LDR Status: ");
   Serial.println(ldrStatus);

    if(ldrStatus < SENSITIVITY_THRESHOLD_MIN || ldrStatus > SENSITIVITY_THRESHOLD_MAX)
    {

        // True if photoresister has been covered (according to the average reading during our sample size)
        bool currentState = calculateCurrentState(ldrStatus);
        Serial.print(counter->counterName);
        Serial.print(" CurrentState: ");
        Serial.println(currentState);
    
        if(currentState == true && counter->lastState == false)
        {
            Serial.print(counter->counterName);
            Serial.println(" currentState == true && lastState == false. Incrementing Counter");
            counter->lastState = true;
            counter->count++;
            Serial.print(counter->counterName);
            Serial.print(" Number of turns: ");
            Serial.println(counter->count); 
        }
        else if(currentState == false && counter->lastState == true)
        {
            Serial.print(counter->counterName);
            Serial.println(" currentState == false && lastState == true");      
            counter->lastState = false;  
        }
    }


}

void serialEvent()  //reads data from the serial connection when data sent
{   
  commandInfo.index = 0; 
  while(Serial.available())
  { 
   char currentChar = (char)Serial.read();
   commandInfo.bufferData[commandInfo.index] = currentChar;
   if(currentChar == ';')
   {
    commandInfo.hasData = true;
   }
   commandInfo.index++;
  }
  readCommands();
  
  for(int i=0; i<BUFFER_SIZE ; i++)
  {
    Serial.print(commandInfo.bufferData[i]);
  }

  currentState = IDLE_STATE;
 

}
void readCommands()
{
  if(commandInfo.index >= 3 && commandInfo.bufferData[0] == '+')
  {
    switch(commandInfo.bufferData[1])
    {
      case 'C':
      {
        handleCalibrateCommand();
        break;
      }
      case 'M':
      {
        handleMovementCommand(commandInfo.bufferData); // do I need to pass any specific values or the whole array??
        break;
      }
      case 'G':
      {
        handleGetPositionCommand();
        break;
      }
      case 'P':
      {
        handleParkCommand();
        break;
      }
    }
  }
}

void clearCommands()
{
  commandInfo.hasData = false;
  memset(commandInfo.bufferData,0,BUFFER_SIZE); //sets buffer array to zero
  commandInfo.index = 0;
  Serial.println();
}

bool updateMovement()
{
  return false;
}

bool updateDeceleration()
{
  return false;
}

void handleCalibrateCommand()
{
  
}

void handleMovementCommand(char* commandBuffer)
{
  bool isRelativeMovement = false;
  int moveDegrees; 
    
  switch(commandBuffer[2])
  {
    case 'A':
    {
     isRelativeMovement = false;
     Serial.println("I am in case A mudafucka");
     break;
    }
    case 'R':
    {
      isRelativeMovement = true;
      Serial.println("I am in case R");
      break;
    }
  }

  moveDegrees = atoi(&commandInfo.bufferData[3]);
  Serial.print("Degrees needed to move: ");
  Serial.println(moveDegrees);
}

void handleGetPositionCommand()
{
  
}
void handleParkCommand()
{
  
}

