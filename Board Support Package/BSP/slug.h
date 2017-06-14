// Slug.h
// Runs on TM4C123 with Tiva shield version 2.0
// The Board Support package is an abstraction layer forming a bridge between the low level and the high level softwrae.
// This file contains function prototypes for the software interface to the Tiva Shield

// Shriya Shah
// May 18, 2017

//  J1   J3               J4   J2
// [ 1] [21]             [40] [20]
// [ 2] [22]             [39] [19]
// [ 3] [23]             [38] [18]
// [ 4] [24]             [37] [17]
// [ 5] [25]             [36] [16]
// [ 6] [26]             [35] [15]
// [ 7] [27]             [34] [14]
// [ 8] [28]             [33] [13]
// [ 9] [29]             [32] [12]
// [10] [30]             [31] [11]


// Connected pins and Peripherals
// power and reset
// J1.1 +3.3V (power)
// J3.21 +5V (power)
// J3.22 GND (ground)
// J2.20 GND (ground)
// J2.16 nothing (reset)
//--------------------------------------------------
// Motors
// PB7 - Direction
// PF1 - PWM
//--------------------------------------------------
// Load Cells
// PE3
//--------------------------------------------------
// Thermocouple
//--------------------------------------------------
// Current Sensor
//--------------------------------------------------
// Absolute Encoders
//--------------------------------------------------
// Display LEDS
// PF1, PF2, PF3
//--------------------------------------------------
// Button
// PF0
//--------------------------------------------------
// Serial Monitor
// PA0, PA1
//--------------------------------------------------
// unconnected pins

#include <stdint.h>


//------------------Clock_set_fastest---------------------------
//Configure system clock to run at fastest settings
//Input: none
//Output: None
void Clock_set_fastest(void);

//------------------Clock_get_frequency---------------------------
//Get Current clock frequency
//Input: none
//Output: Frequency
uint32_t Clock_get_frequency(void);

//------------------EnableInterrupts()---------------------------
//Enable all interuupts system wide
//Input: none
//Output: None
void EnableInterrupts(void);

// ---------------Button1_Init----------------------
//Initializes the GPIO pin for the button PF0 (J2.17) as input.
//Input: none
//Output: none
uint8_t Button1_Init(void);

//----------------Button1_Input---------------------
// Read and return the immediate status of the button1.
// Button debouncing not considered
// Input: none
// Output: Button not pressed - non zero
//         Button pressed - zero
// Call Button1_init() first
uint8_t Button1_Input(void);

// ----------------RGBled_Init-----------------------
// Initializes the RGB LED on PF1, PF2 and PF3
void RGBled_Init(uint8_t red, uint8_t green, uint8_t blue);

// ----------------RGBled_Set-----------------------
// Sets the RGB LED on PF1, PF2 and PF3
// Input: non zero input sents the LEDs
// Output: none
void RGBled_Set(uint8_t red, uint8_t green, uint8_t blue);

// ----------------RGBled_Toggle-----------------------
// Toggles the RGB LED on PF1, PF2 and PF3
// Input: non zero input toggles the LEDs
//         red is toggle for red
//         blue is toggle for blue
//         green is toggle for green
// Output: none
void RGBled_Toggle(uint8_t red, uint8_t green, uint8_t blue);

// ----------------RedledTimer_Init-----------------------
// Use Timer 0A to generate an interrupt at every given period
// Initializes the Red LED on PF1
void RedledTimer_Init(int period);


//------------------Delay_cycle----------------------------
//Delays the execution by approximately the given number of cycles
// It is not accurate
//Input: delay cycles
void Delay_cycle(uint32_t delay);

//------------------delayMS----------------------------
//Delays in MS
// It is not accurate
//Input: ms
void delayMS(int);

//------------------SerialMonitor_Init()---------------------------
//Initialize the serial monitor using UART
//Input: None
//Output: None
void SerialMonitor_Init(void);

//------------------SerialMonitor_Loop()---------------------------
//Receive data from the serial monitor using UART and
// loop it back
//Input: None
//Output: None
void SerialMonitor_Loop(void);

//------------------initConsole()---------------------------
//Initialize the serial monitor using UART, uses the Utilities library as well
//Input: None
//Output: None
void initConsole(void);

//------------------SerialMonitor_Send()---------------------------
//Send Data to the serial monitor using UART
//Input: None
//Output: None
void SerialMonitor_Send(char* input);

//------------------SerialMonitor_Receive()---------------------------
//Receive data from the serial monitor using UART
//Input: None
//Output: None
void SerialMonitor_Receive(void);


//------------------tempSensor_init()---------------------------
//Initialize the Temperature Sensor on Board
//Input: None
//Output: None
void tempSensor_init(void);

//------------------tempSensor_startConversion()---------------------------
//Start the sequencer for Temp Sensor
//Input: None
//Output: None
void tempSensor_startConversion(void);

//------------------tempSensor_handler()---------------------------
//Interrupt handler for Temp Sensor
//Input: None
//Output: None
void tempSensor_handler(void);

//------------------getAvgTemp()---------------------------
//Get Average Temperature
//Input: None
//Output: Average Temp
uint32_t getAvgTemp(void);

//------------------convert2C()---------------------------
//Convert Average Temperature to degree Celsius
//Input: None
//Output: None
uint32_t convert2C(uint32_t);

//------------------convert2F()---------------------------
//Convert Average Temperature to degree Farenhiet
//Input: None
//Output: None
uint32_t convert2F(uint32_t);


//------------------LoadCell_init()---------------------------
//Initialize the Load Cell input on Board
//Input: None
//Output: None
void LoadCell_init(void);

//------------------getLoadCellValue()---------------------------
//Get Load Cell Value
//Input: None
//Output: Load Cell value ADC units
uint32_t getLoadCellValue(void);

//------------------measuredLoad()---------------------------
//Get Load Cell Value
//Input: None
//Output: Load Cell value in pounds
double measuredLoad(void);

//------------------adc2Vol()---------------------------
//Covert raw ADC value to Voltage
//Input: Raw ADC
//Output: Voltage
double adc2Vol(uint32_t);

//------------------Vol2Load()---------------------------
//Covert raw voltage value to force in pound
//Input: Raw Volage
//Output: Pound
double Vol2Load(double);


//------------------Motor_Init()---------------------------
//Initialize the Motor
//Input: frequency of operation (Eg: 10 (KHZ))
//Output: Period
void Motor_Init(uint32_t period);

//------------------Motor_SetDuty()---------------------------
//Set Duty cycle for the motor
//Input: Target Duty Cycle (in percent - 0:100)
//Output: None
void Motor_SetDuty(float duty);

//------------------setMotorPWMFreq()---------------------------
//Set the global variable period
//Input: period
//Output: None
void setMotorPWMFreq(uint32_t period);

//------------------getMotorPWMFreq()---------------------------
//Get the global variable period
//Input: period
//Output: None
uint32_t getMotorPWMFreq(void);

//------------------motorSendCommand()---------------------------
//Sends the final commands to the motor driver
//Input: duty cycle, direction
//Output: None
void motorSendCommand(float duty, int direction);

//------------------checkLimits()---------------------------
//Check duty cycle limit
//Input: Duty Cycle
//Output: None
void checkLimits(float duty);

//------------------enableMotor()---------------------------
//Enables the motor
//Input: None
//Output: None
void enableMotor(void);

//------------------disableMotor()---------------------------
//Disables the motor
//Input: None
//Output: None
void disableMotor(void);

//------------------setglobals4Motor()---------------------------
//Set global variable duty and direction
//Input: Duty cycle, Direction
//Output: None
void setglobals4Motor(float duty, int direction);

//------------------getglobalduty()---------------------------
//Get global variable duty and direction
//Input: None
//Output: Duty cycle
float getglobalduty(void);

//------------------getglobaldirection()---------------------------
//Get global variable duty and direction
//Input: None
//Output: Direction
int getglobaldirection(void);

//------------------Controller_Init()---------------------------
//Initializes a timer routine for the controller
//Input: None
//Output: None
void Controller_Init(uint32_t Controllerfreq);

//------------------ControllerEnable()---------------------------
//Enable Timer 1A
//Input: None
//Output: None
void ControllerEnable(void);

//------------------getControllerFlag()---------------------------
//Returns controller flag variable
//Input: None
//Output: None
uint32_t getControllerFlag(void);


//------------------setGoalForce()---------------------------
//Set the global variable RefForce
//Input: Ref Force
//Output: None
void setGoalForce(uint32_t);

//------------------getGoalForce()---------------------------
//Get the global variable RefForce
//Input: None
//Output: Goal Force
uint32_t getGoalForce(void);

//------------------getRefForce()---------------------------
//Get the global variable RefForce
//Input: None
//Output: Ref Force
uint32_t getRefForce(void);

//------------------PID()---------------------------
//Use PID
//Input: None
//Output: Ref Force
