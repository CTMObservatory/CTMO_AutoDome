# CTMO_AutoDome

# Break down of StateMachine.ino
# _________________________________________
 define BUFFER_SIZE 8
This statement defines the object BUFFER_SIZE to a constant value of 8.

enum States
	{
	  IDLE_STATE,
	  CALIBRATION_STATE,
	  END_CALIBRATION_STATE,
	  MOTOR_TURNING_STATE,
	  END_MOTOR_TURNING_STATE 
	};
	

States is declared of the type enum which will allow an object to be of the type States which contains five values. Enumeration objects can be assigned to other values as a type and can be set equal one if the five states.

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

SensorInfo, MotorMovementInfo, CommandInfo and DomeInfo have all been made structure type and a typedef which allows any of the objects (SensorInfo, MotorMovementInfo, CommandInfo and DomeInfo) to be used as a type. Once an object has been declared with one of the typedef the values inside the struct can be accessed by using the member access operator or the ‘.’.

SensorInfo counterGear;
SensorInfo counterDome;
CommandInfo commandInfo;
DomeInfo domeInfo;
MotorMovementInfo motorMovementInfo;

The structure types which were defined above are being used to declare an object. 

States currentState;
The enum type States makes the object currentState.

bool calculateCurrentState(int ldrStatus);
bool calculateCurrentState(int ldrStatus)
{
  return (ldrStatus < SENSITIVITY_THRESHOLD_MIN);
}
The function will return true if ldrStatus is less than the SENSITIVITY_THRESHOLD_MIN which is 550.

void updateCounter(SensorInfo* counter);
void updateCounter(SensorInfo* counter)
{
   int ldrStatus = analogRead(counter->ldrPin);   //reads status of LDR value
   Serial.print(counter->counterName);
   Serial.print(" LDR Status: ");
   Serial.println(ldrStatus);

The function declares an object of the type in which equals a number from 0-1023 depending on what the ldrPin is. The serial function is used to communicate to the Arduino from a computer. The next three lines print counterName, “LDR Status: “ and ldrStatus to the serial port.
  


    if(ldrStatus < SENSITIVITY_THRESHOLD_MIN || ldrStatus > SENSITIVITY_THRESHOLD_MAX)
    {

        // True if photoresister has been covered (according to the average reading during our sample size)
        bool currentState = calculateCurrentState(ldrStatus);
        Serial.print(counter->counterName);
        Serial.print(" CurrentState: ");
        Serial.println(currentState);
    
The “if” statement attempts to address if the ldrStatus is not in the dead zone which is between the SENSITIVITY_THRESHOLD_MIN and SENSITIVITY_THRESHOLD_MAX. If ldrStatus is not in the dead zone the “if” statement will make an object of bool type set equal to the call of the calculateCurrentState function and then will try to print the counterName, the string “ CurrentState: “ and currentstate to the serial port.

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
The next “if” statement address the issue if you aren’t at the LastState or the LastState equal false.if the “if” statement applies counterName, the string “ currentState == true && lastState == false. Incrementing Counter" prints to the serial port then sets the lastState to true increments the counter plus one print the string “  Number of turns: “ and prints count to the serial port.


        else if(currentState == false && counter->lastState == true)
        {
            Serial.print(counter->counterName);
            Serial.println(" currentState == false && lastState == true");      
            counter->lastState = false;  
        }
    }
}

If the lastState was already equal to true and the currentState equal to false it prints the counterName, the string “ currentState == false && lastState == true” to the serial port and set lastState to false.

void readCommands();
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

The readCommands() function reads a given command only if the “if” statement is satisfied. If the index greater than or equal to 3 and the array bufferData[0] equals the char ‘+’, then it will begin calling the handleCalibrateCommand, handleMovementCommand, handleGetPositionCommand and handleParkCommand functions depending what char variable the array bufferData[1] holds.

bool updateMovement();
bool updateMovement()
{
  return false;
}
The updateMovement function returns false as a value.




bool updateDeceleration();
bool updateDeceleration()
{
  return false;
}
The updateDecelaration function returns false as a value.

void clearCommands();
void clearCommands()
{
  commandInfo.hasData = false;
  memset(commandInfo.bufferData,0,BUFFER_SIZE); //sets buffer array to zero
  commandInfo.index = 0;
  Serial.println();
}

The clearCommands function clears the objects. Memset function is used to clear arrays.

void handleCalibrateCommand();
void handleCalibrateCommand()
{
  
}

Empty function?

void handleMovementCommand();
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

The handleMovement function passes a char variable and is used in a switch statement. If the commandBuffer[2] equals A the string, and I quote “ “I am in case A mudafucka” “ prints to the serial port and sets isRelativeMovement to false. If the commandBuffer[2] equals R, isRelativeMovement is set to true and prints “I am in case R” to the serial port. After the switch statement moveDegrees is set to the conversion of &commandInfo.bufferData[3] string to an integer. Then prints the string “Degrees needed to move: “ and the moveDegrees  integer to the serial port.

void handleGetPositionCommand();
void handleGetPositionCommand()
{
  
}
Empty function?

void handleParkCommand();
void handleParkCommand()
{
  
}
Empty function?


const int SENSITIVITY_THRESHOLD_MIN = 550; // The value that we'll consider to be high or low for the photoresistor
const int SENSITIVITY_THRESHOLD_MAX = 900;  //Dead zone is between threshold MIN and MAX

Declaration initializes a constant global variable.

int gearPerDomeDegree;
float countsToTurn;
bool motorDirection;

variables to store data, but I could not find them anywhere else in the code?

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

Void setup() is used to initialize variables to begin communicating with the Arduino. Serial.begin(9600) initializes the rate at which the data will transmit to the Arduino. pinMode() is used to set a pin number as either input or output.

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

Void loop() is used to switch the currentState and perform a different task depending in what state (IDLE_STATE,  CALIBRATION_STATE,END_CALIBRATION_STATE, MOTOR_TURNING_STATE, END_MOTOR_TURNING_STATE) currentState equals.
# _________________________________________
