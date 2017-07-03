// Slug.h
// Runs on TM4C123 with TIVA shield v2.0
// The Board Support package is an abstraction layer forming a bridge between the low level
// and the high level software.
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
// Internally connected on launchpad
// PD0 - PB6
// PD1 - PB7
//--------------------------------------------------
// Motors
// PB7 - Direction
// PF1 - PWM
// Current Monitor - PE5
//--------------------------------------------------
// Load Cells
// PE3
//--------------------------------------------------
// Thermocouple
// PE0
//--------------------------------------------------
// Absolute Encoders
// PA5 - Tx
// PA4 - Rx
//--------------------------------------------------
// Incremental Encoders
// PC5 - ChA
// PC6 - ChB
// PC4 - ChI
//--------------------------------------------------
// Display LEDS
// Blue - PD7
// Yellow - PC7
//--------------------------------------------------
// Reset Button
// RST
//--------------------------------------------------
// Serial Monitor
// PA0, PA1
//--------------------------------------------------
// CAN
// PB5 - Tx
// PB4 - Rx
//--------------------------------------------------
// Additional ADC
// PE2
//--------------------------------------------------
// Additional SPI
// PF0 - Rx
// PF2 - CLK
// PF3 - CS
// PD3 - TX
//--------------------------------------------------
// I2C
// PA6 - SCL
// PA7 - SDA
//--------------------------------------------------
// GPIO
// PB6, PF4, PE1
//--------------------------------------------------

// ************* includes ******************************************

#include <stdint.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "utils/uartstdio.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"

// ********************************************************
// *************** Clock and timing ***********************
// ********************************************************
//------------------Clock_set_40MHz---------------------------
// Configure system clock to run at 40MHZ settings
// Input: None
// Output: None
void Clock_set_40MHz(void);

//------------------Clock_set_80MHz---------------------------
// Configure system clock to run at 80MHZ settings
// Input: None
// Output: None
void Clock_set_80MHz(void);

//------------------Clock_get_frequency---------------------------
// Get Current clock frequency
// Input: none
// Output: Frequency
uint32_t Clock_get_frequency(void);

//------------------EnableInterrupts()---------------------------
// Enable all interrupts system wide
// Input: none
// Output: None
void EnableInterrupts(void);

//------------------Delay_cycle----------------------------
// Delays the execution by approximately the given number of cycles
// It is not accurate and blocking
// Input: delay cycles
void Delay_cycle(uint32_t delay);

//------------------delayMS----------------------------
// Delays in millisecond
// It is not accurate and blocking
// Input: ms
void delayMS(int);

// ********************************************************
// *************** LEDs and Buttons ***********************
// ********************************************************
// ----------------BlueLED_Init-----------------------
// Initializes the Blue LED on PD7
// Input: None
// Output: None
void BlueLED_Init(void);

// ----------------BlueLED_Set-----------------------
// Set Blue LED
// Input: None
// Output: None
void BlueLED_Set(void);

// ----------------BlueLED_Clear-----------------------
// Clear Blue LED
// Input: None
// Output: None
void BlueLED_Clear(void);

// ----------------BlueLED_Toggle-----------------------
// Toggle Blue LED
// Input: None
// Output: None
void BlueLED_Toggle(void);

// ----------------YellowLED_Init-----------------------
// Initializes the Yellow LED on PC7
// Input: None
// Output: None
void YellowLED_Init(void);

// ----------------YellowLED_Set-----------------------
// Set Yellow LED
// Input: None
// Output: None
void YellowLED_Set(void);

// ----------------BYellowLED_Clear-----------------------
// Clear Yellow LED
// Input: None
// Output: None
void YellowLED_Clear(void);

// ----------------YellowLED_Toggle-----------------------
// Toggle Yellow LED
// Input: None
// Output: None
void YellowLED_Toggle(void);

// ---------------Button1_Init----------------------
// Initializes the GPIO pin for the button PF4 (J2.17) as input.
// Input: none
// Output: none
void Button1_Init(void);

//----------------Button1_Input---------------------
// Read and return the immediate status of the button1.
// Button debouncing not considered
// Input: None
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
// Output: None
void RGBled_Set(uint8_t red, uint8_t green, uint8_t blue);

// ----------------RGBled_Toggle-----------------------
// Toggles the RGB LED on PF1, PF2 and PF3
// Input: non zero input toggles the LEDs
//         red is toggle for red
//         blue is toggle for blue
//         green is toggle for green
// Output: None
void RGBled_Toggle(uint8_t red, uint8_t green, uint8_t blue);

// ********************************************************
// ******************* Timers *****************************
// ********************************************************
//------------------initTimer0---------------------------
//Initialize Timer0A as a periodic timer
//Input: frequency
//Output: None
void initTimer0(int frequency);

// ********************************************************
// *************** Console & Logger ***********************
// ********************************************************

//------------------initConsole()---------------------------
//Initialize the serial monitor using UART, uses the Utilities library as well
//Input: Baud rate
//Output: None
void initConsole(int);

//------------------Console_Send()---------------------------
//Send Data to the serial monitor using UART
//Input: Data to Send
//Output: None
void Console_Send(char* input);

//------------------Logger_Init()---------------------------
//Initializes a Timer which logs data at a specified rate
//Input: Logger frequency and Baud Rate
//Output: None
void Logger_Init(uint32_t LoggerFreq,  int BaudRate);

//------------------LoggerIntHandler()---------------------------
//Interrupt Handler for the Logger
//Input: None
//Output: None
void LoggerIntHandler(void);

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

//------------------SerialMonitor_Receive()---------------------------
//Receive data from the serial monitor using UART
//Input: None
//Output: None
void SerialMonitor_Receive(void);

// ********************************************************
// *************** Temperature Sensor *********************
// ********************************************************

//------------------tempSensor_init()---------------------------
//Initialize the Temperature Sensor on Board
//Input: hardwareAverage (can be 2,4,8,16,32 or 64)
//Output: None
void tempSensor_Init(int hardwareAverage);

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

// ********************************************************
// ********************* Motor ****************************
// ********************************************************
//------------------Motor_Init()---------------------------
//Initialize the Motor
//Input: frequency of operation (Eg: 10 (KHZ))
//Output: Period
void Motor_Init(uint32_t period);

//------------------Motor_SetDuty()---------------------------
//Set Duty cycle for the motor
//Input: Target Duty Cycle (in percent - 0:100)
//Output: None
void Motor_SetDuty(uint32_t duty);

//------------------convert2PWMDuty()---------------------------
//Set Duty cycle for the motor
//Input: Target Duty Cycle (in percent - 0:100)
//Output: Command to send to PWM
double convert2PWMDuty(uint32_t duty);

//------------------motorSendCommand()---------------------------
//Sends the final commands to the motor driver
//Input: duty cycle, direction
//Output: None
void motorSendCommand(uint32_t duty, int direction);

//------------------getMotorPWMPeriod()---------------------------
//Get the global variable period
//Input: period
//Output: None
double getMotorPWMPeriod(void);

//------------------setDirection()---------------------------
//Set Direction Pin High
//Input: None
//Output: None
void setDirection(void);

//------------------clearDirection()---------------------------
//Clear Direction Pin Low
//Input: None
//Output: None
void clearDirection(void);

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
void setglobals4Motor(uint32_t duty, int direction);

//------------------getglobalduty()---------------------------
//Get global variable duty and direction
//Input: None
//Output: Duty cycle
uint32_t getglobalduty(void);

//------------------getglobaldirection()---------------------------
//Get global variable duty and direction
//Input: None
//Output: Direction
int getglobaldirection(void);

//------------------checkLimits()---------------------------
//Check duty cycle limit
//Input: Duty Cycle
//Output: None
void checkLimits(double duty);

// ********************************************************
// ****************** Load cell **************************
// ********************************************************

//------------------LoadCell_init()---------------------------
//Initialize the Load Cell input on Board
//Input: Hardware averaging, ADCsampleFreq sets the timer trigger freq
//Output: None
void LoadCell_init(int hardwareAveraging, int ADCsampleFreq);

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


//------------------Controller_Init()---------------------------
//Initializes a timer routine for the controller
//Input: Controller Frequency
//Output: None
void Controller_Init(uint32_t Controllerfreq);

//------------------ControllerIntHandler()---------------------------
//Interrupt handler for the controller
//Input: None
//Output: None
void ControllerIntHandler(void);

//------------------Swing_control()---------------------------
//Implement simple feedforward swing motion on leg
//Input: None
//Output: None
void Swing_control(void);

//------------------PID_conrol()---------------------------
//PID control function
//Input: None
//Output: None
void PID_control(void);

//------------------ControllerEnable()---------------------------
//Enable Timer 1A
//Input: None
//Output: None
void ControllerEnable(void);

//------------------getGoalFlag()---------------------------
//Returns controller flag variable
//Input: None
//Output: None
uint32_t getGoalFlag(void);


//------------------setGoalFlag()---------------------------
//Returns controller flag variable
//Input: None
//Output: None
void setGoalFlag(uint32_t);

//------------------setGoalForce()---------------------------
//Set the global variable RefForce
//Input: Ref Force
//Output: None
void setGoalForce(double);

//------------------getGoalForce()---------------------------
//Get the global variable RefForce
//Input: None
//Output: Goal Force
double getGoalForce(void);

//------------------getKp()---------------------------
//Gep proportional gain
//Input: None
//Output: Kp
double getKp(void);

//------------------getError()---------------------------
//Get Error in PID
//Input: None
//Output: Error
double getError(void);

//------------------getPIDoutput()---------------------------
//Get output in PID
//Input: None
//Output: out
double getPIDoutput(void);

//------------------checkIntegralLimit()---------------------------
//Wrap up integral Error
//Input: None
//Output: None
double checkIntegralLimit(double);

//------------------deadBandCheck()---------------------------
//Dead Band for PID loop
//Input: None
//Output: flag of whether goal reached or not
uint32_t deadBandCheck(double);

//------------------logger_PID_ForceControl()---------------------------
//Logger function to be called for logging data for force control PID
//Input: None
//Output: None
void logger_PID_ForceControl(void);

void addADCIntHandler(void);

void addADC_Init(int hardwareAveraging, int ADCsampleFreq);

uint32_t getaddADCVal(void);

void logPID(void);