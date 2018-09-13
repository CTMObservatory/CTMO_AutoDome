# Tech Design for Arduino Dome Controller Code

**Creator:** Latifah Maasarani (LatifahM@knights.ucf.edu) 

Last Update: Latifah M, August 8, 2017

***

**Data types, functions, states, etc are bolded**

Variables are bulleted with their type in <spam style="color:blue;">blue</spam>

_Comment information is italicized_

<mark>If highlighted, not yet implemented in code</mark>

This is PSEUDOcode, only explains the State Machine, do not copy paste into IDE

## __Objects (Structs)__

**SensorInfo: Two global variables for this type:
sensorInfo.Gear & sensorInfo.Dome**

*	<spam style="color:blue;">Int</spam> inputPin 
*	<spam style="color:blue;">Int</spam> count
*	<spam style="color:blue;">Bool</spam> lastState
*	<spam style="color:blue;">Char</spam> counterName

**DomeInfo : One global variable of this type: domeInfo**

*	<spam style="color:blue;">float</spam> relativePosition (e.g. -40 degrees)
	-_Number of degrees needed to move CW(positive) or CCW(negative)_
*	<spam style="color:blue;">float</spam> absolutePosition (e.g. 320 degrees)
	-_corresponds to azimuth position of telescope_
*	<spam style="color:blue;">float</spam> gearCountPerDomeDegree
	 -_number of hits of the gear sensor corresponding to one degree of dome rotation_

**MotorMovementInfo: Single global for this type: motorMovementInfo**

*	<spam style="color:blue;">int</spam> turningDirection 
	-_Tells motor which direction to turn (i.e. CW/CCW or FORWARD/BACKWARD)_
*	<spam style="color:blue;">int</spam> gearCountsToTarget
	-_Relative number of degrees needed to move to reach wanted absolute position_

**CommandInfo (Single global of this type)**

*	<spam style="color:blue;">char</spam> buffer[BUFFER_SIZE] 

	-_BUFFER_SIZE is # defined_ 

	-_Where serial characters are temporarily stored so commands can be read using function readCommands_
*	<spam style="color:blue;">int</spam> index
*	<spam style="color:blue;">bool</spam> hasData


<mark>**ShutterMovementInfo : single global type: shutterMovementInfo** </mark>


## __State Machine__
**Idle State**

-_The base state we start in and always return to eventually_

-_Implemented in case there is code that needs to be read before entering into a different state_


**Calibration State**
-_Turns the motor in one direction while pumping updateCounter function for both sensors_

-_Once dome has completed one full rotation, enter END_CALIBRATION_STATE_

**End Calibration State**

-_Calculates the Gear rotation count per dome degree and saves in to domeInfo struct (domeInfo.gearCountPerDomeDegree) (needs to be devided by 360 for implementation, dividing by small number for debugging code)_

-_when finished, make currentState = IDLE_STATE_

**Motor Turning State**

-<mark>_will be used to tell the dome which direction to turn and how far to rotate_</mark>

**End Motor Turning State**

-_Send “finished moving” command to driver_

-_set currentState = IDLE_STATE when finished_

## __Functions__
<spam style="color:blue;">Void</spam> **serialEvent():**

- _Reads data from the serial connection immediately when data sent._
- _Built in function (https://www.arduino.cc/en/Tutorial/SerialEvent)_
- _creates array from command read_
- _calls readCommands function_
- _ends by calling clearCommands function_

<spam style="color:blue;">Void</spam> **readCommands():**

- _Reads array from serialEvent if the command begins with a “+” and has an array size of 3 or more, and ends in “;” (meaning array positions 0,1,&2 minimum) (Example: +MA23;)_
- _if array[1] is C then “handleCalibrateCommand()” function is called_
- _if array[1] is M then “handleMovementCommand(commandInfo.bufferData)” function is called_
- _if array[1] is G then “handleGetPositionCommand()” function is called_
- _if array[1] is P then “handleParkCommand()” function is called_

<spam style="color:blue;">Void</spam> **clearCommands():**

- _resets buffer array to zeroes_
- _clears commandInfo variables_

<spam style="color:blue;">bool</spam> **updateMovement():**

- currently empty

<spam style="color:blue;">bool</spam> **updateDeceleration():**

- _currently empty_
- _may not need since motor is quickly responsive to end movement commands_

<spam style="color:blue;">void</spam>  **handleCalibrateCommand():**

<spam style="color:blue;">void</spam>  **handleMovementCommand():**


<spam style="color:blue;">void</spam> **handleGetPositionCommand():**

- _prints a string of data to be parsed by the driver_
- _"#" initializer is our syntax for sending a command from the Arduino to the driver, while the “;” signifies the end of the string_

<spam style="color:blue;">void</spam> **handleParkCommand():**
- _will be used to implement home function in driver_


## __Command Syntax__
**Start Calibration**

- _Command: “+C” from driver will tell Arduino to enter Calibration State ;_
- _Calls handleCalibrationCommand_

**Movement**

- _Command: +M<movementType><degrees> from driver will tell Arduino to enter Motor Turning State ;_
- _Movement Type can be “A” for absolute or “R” for relative_
- _Examples:_
    * _+MA90; will move to absolute position 90 degrees_
    * _+MR40; will move 40 degrees clockwise of current position_
    * _+MR-20; will move 20 degrees counter-clockwise of current position_
 
- _The atoi() function in the standard library will help a LOT here, especially since it can handle negative signs too._
- _Calls handleMovementCommand_

**Get Position**

- _Command: “+G” from driver will tell Arduino to send the current absolute position to driver;_
- _Calls handleGetPositionCommand_

**Park**

- _Command: +P;_
- _Calls handleParkCommand_
- _Park in ASCOM driver is know as “home”_


**TODO:**

- <mark>_Optical sensor on dome to detect if its moving._</mark> 
    * <mark>_If dome not moving but gear is counting: Stop movement!</mark>_
- <mark>_Absolute & relative positions need to be read as floating point numbers rather than as an integer_</mark>


