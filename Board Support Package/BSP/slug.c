// slug.c
// Runs on TM4C123 with Tiva shield version 2.0
// The Board Support package is an abstraction layer forming a bridge between the low level and the high level software.
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
// unconnected pins

#include "slug.h"

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

// ****** Constants ******
// Sample Frequency for load cell triggering
const int sampleFreq = 500; //500 Hz
const int clockFreq = 80000000; //80MHz
const int PWMclockFreq = 40000000; //80MHz

// Error
const double MinSteadyError = 0;
const double MaxSteadyError = 100;

// Controller
const double DEADBAND = 0.01;
const int BIAS = 5; //5% duty bias

// ****** Variables ******
uint16_t samplePeriod; //For load cell sampling period calculation
uint32_t rawTemp[1];
uint32_t loadCellValue[1];

volatile uint32_t goalReached;

double pwmPeriod;
volatile double pwmDuty;

// ******* PID Control *********************
double goalPos;
volatile double ERROR;
volatile double LAST_ERROR = 0;
volatile double TOTAL_ERROR = 0;
volatile double PID_OUT;
volatile double P, D, I;

//PID VALUES
volatile double Kp; //P from PID 20
volatile double Kbar = .1; //P from PID 0.08
volatile double Ki = 0.01; //.01; //I from PID 1
volatile double Kd = 0.0; //D from PID

// *********Globals******************************
double globalDutyCycle;
double globalDirection;

uint32_t dummy = 0;


//------------------Clock_set_fastest---------------------------
//Configure system clock to run at fastest settings
// 16MHZ crystal on main oscillator which drives the PLL (400 MHZ).
// There is a default /2 divider in clock path and we specify /5
// Final Clock frequency: 400/10 = 40 MHZ
//Input: none
//Output: None
void Clock_set_fastest(void){
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                        SYSCTL_OSC_MAIN);  // Setup system clock at 80MHz from PLL with Crystal
}

//------------------Clock_get_frequency---------------------------
//Get Current clock frequency
//Input: none
//Output: Frequency
uint32_t Clock_get_frequency(void){
    return SysCtlClockGet();
}

//------------------EnableInterrupts()---------------------------
//Enable all interuupts system wide
//Input: none
//Output: None
void EnableInterrupts(void){
    IntMasterEnable();
}

// ---------------Button1_Init----------------------
//Initializes the GPIO pin for the button PF0 (J2.17) as input.
//Input: none
//Output: none
void Button1_Init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
       } //Enable Peripheral

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

//----------------Button1_Input---------------------
// Read and return the immediate status of the button1.
// Button debouncing not considered
// Input: none
// Output: Button not pressed - non zero
//         Button pressed - zero
// Call Button1_init() first
uint8_t Button1_Input(void){
    if(((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))&GPIO_PIN_4) == 0) //Button Pressed
        return 0;
    else
        return 1;
}

// ----------------RGBled_Init-----------------------
// Initializes the RGB LED on PF1, PF2 and PF3
void RGBled_Init(uint8_t red, uint8_t green, uint8_t blue){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
       } // Enable and wait for the port to be ready for access

    //Configure port for LED operation
    if(red>0){
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    }if(blue>0){
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    }if(green>0){
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
    }
}

// ----------------RGBled_Set-----------------------
// Sets the RGB LED on PF1, PF2 and PF3
// Input: non zero input sets the LEDs
// Output: none
void RGBled_Set(uint8_t red, uint8_t green, uint8_t blue){
    if(red>0){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIO_PIN_1);
    }
    if(blue>0){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
    }
    if(green>0){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
    }
}

// ----------------RGBled_Toggle-----------------------
// Toggles the RGB LED on PF1, PF2 and PF3
// Input: non zero input toggles the LEDs
//         red is toggle for red
//         blue is toggle for blue
//         green is toggle for green
// Output: none
void RGBled_Toggle(uint8_t red, uint8_t green, uint8_t blue){
    if(red){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)^GPIO_PIN_1);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIO_PIN_1);
    }
    if(green){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)^GPIO_PIN_3);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
    }
    if(blue){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)^GPIO_PIN_2);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
    }
}

// ----------------RedledTimer_Init-----------------------
// Initializes the Red LED on PF1
void RedledTimer_Init(int period){
    uint32_t periods; // Timer delays

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    //Configure Timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Define period
    periods = (SysCtlClockGet()/period)/2; //We want 10 Hz toggle frequency therefore the period should be the
    //system clock / desired toggle freq/2
    TimerLoadSet(TIMER0_BASE, TIMER_A, periods-1);

    //Enable iNTERRUPTS
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();

    //Enable Timer
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// ----------------ledTimer_Toggle-----------------------
// Toggle Red LED on PF1 based on Timer interrupt
void RedledTimer_Toggle(){
    //Read current state and write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, GPIO_PIN_2);
    }
}

//// ----------------Timer0IntHandler-----------------------
//// ISR for Blink LED
//void Timer0IntHandler_LED(void){
//    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//
//    RedledTimer_Toggle();
//}
//
//void Timer1IntHandler(void){
//    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//
//    RedledTimer_Toggle();
//}

//------------------Delay_cycle----------------------------
//Delays the execution by approximately the given number of cycles
// It is not accurate
//Input: delay cycles
void Delay_cycle(uint32_t delay){
    SysCtlDelay(delay);
}

//------------------delayMS----------------------------
//Delays in MS
// It is not accurate
//Input: ms
void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

//------------------SerialMonitor_Init()---------------------------
//Initialize the serial monitor using UART - Use SerialMonitorConfigure for
// printf facility
//Input: None
//Output: None
void SerialMonitor_Init(){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay(3); //Just to be sure that the peripherals were enabled

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay(3); //Just to be sure that the peripherals were enabled

    //Configure Pins for UART functionality
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);

    // Set UART functionality - Baud rate, parity etc
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));

    //Enable Interrupts
    //IntMasterEnable();
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT); //Enable RX and RT interrupt sources only

    // Initial UART message
       UARTCharPut(UART0_BASE, 'S');
       UARTCharPut(UART0_BASE, 't');
       UARTCharPut(UART0_BASE, 'a');
       UARTCharPut(UART0_BASE, 'r');
       UARTCharPut(UART0_BASE, 't');
       UARTCharPut(UART0_BASE, 'u');
       UARTCharPut(UART0_BASE, 'p');
       UARTCharPut(UART0_BASE, '!');
       UARTCharPut(UART0_BASE, ' ');
}

//------------------SerialMonitor_Loop()---------------------------
//Receive data from the serial monitor using UART and
// loop it back
//Input: None
//Output: None
void SerialMonitor_Loop(){
    if(UARTCharsAvail(UART0_BASE))
        UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
}

//------------------UARTIntHandler()---------------------------
//ISR for UART
//Input: None
//Output: None
void UARTIntHandler(void){
    uint32_t status;
    status = UARTIntStatus(UART0_BASE, true); //get the interrupt status
    UARTIntClear(UART0_BASE, status); //clear the interrupts
    while(UARTCharsAvail(UART0_BASE)){
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE)); //Echo Character
    }
}

//------------------initConsole()---------------------------
//Initialize the serial monitor using UART, uses the Utilities library as well
// Much easier to work with
//Input: None
//Output: None
void initConsole(){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay(3); //Just to be sure that the peripherals were enabled

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay(3); //Just to be sure that the peripherals were enabled

    //Configure Pins for UART functionality
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);

    // Configure UART clock and Baud rate
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC); //Precision internal clock
    UARTStdioConfig(0, 115200, 16000000); // UART Module 0, Baud Rate 115200 and System clock 16MHZ

    UARTprintf("\n System Startup!\n");
}

//------------------SerialMonitor_Send()---------------------------
//Send Data to the serial monitor using UART
//Input: None
//Output: None
void SerialMonitor_Send(char* input){
    UARTprintf(input);
}

//------------------SerialMonitor_Receive()---------------------------
//Receive data from the serial monitor using UART
//Input: None
//Output: None
void SerialMonitor_Receive();

//------------------tempSensor_init()---------------------------
//Initialize the Temperature Sensor on Board, processor triggered
//Input: None
//Output: None
void tempSensor_init(){
    // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlDelay(2);

    //Hardware Averaging
    ADCHardwareOversampleConfigure(ADC0_BASE, 16); //average n samples (can be 2,4,8,16,32 or 64)

    // Configure Sequencer, sample sequencer 1, processor triggered - 1 MSPS, highest priority
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

    // Configure interrupt flag to be set on last sample and notify that this is last sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);

    //Enable ADC sequencer 1
    ADCSequenceEnable(ADC0_BASE, 1);

    // Interrupt enable
    ADCIntClear(ADC0_BASE, 1);
    ADCIntEnable(ADC0_BASE, 1);
    IntEnable(INT_ADC0SS1);
}

//------------------tempSensor_handler()---------------------------
//Interrupt handler for Temp Sensor
//Input: None
//Output: None
void tempSensor_handler(void){
    ADCIntClear(ADC0_BASE, 1);
    ADCSequenceDataGet(ADC0_BASE, 1, rawTemp);
}

//------------------getAvgTemp()---------------------------
//Get Average Temperature
//Input: None
//Output: Average Temp
uint32_t getAvgTemp(){
    return rawTemp[0];
}

//------------------tempSensor_startConversion()---------------------------
//Start the sequencer for Temp Sensor
//Input: None
//Output: None
void tempSensor_startConversion(void){
    ADCProcessorTrigger(ADC0_BASE, 1);
}

//------------------convert2C()---------------------------
//Convert Average Temperature to degree Celsius
//Input: None
//Output: None
uint32_t convert2C(uint32_t TempAvg){
    uint32_t TempValueC;
    TempValueC = (1475 - ((2475*TempAvg))/4096)/10;
    return TempValueC;
}

//------------------convert2F()---------------------------
//Convert Average Temperature to degree Farenhiet
//Input: None
//Output: None
uint32_t convert2F(uint32_t TempAvg){
    uint32_t TempValueF, TempValueC;
    TempValueC = (1475 - ((2475*TempAvg))/4096)/10;
    TempValueF = ((TempValueC*9)+160)/5;
    return TempValueF;
}

//------------------LoadCell_init()---------------------------
//Initialize the Load Cell input on Board
// Load Cell connected to PE3, ADC0, seq 3
//Input: None
//Output: None
void LoadCell_init(){

    //Enable Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlDelay(2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlDelay(2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlDelay(2);

    // Configure PE3 as ADC input
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    //ADC0
    ADCHardwareOversampleConfigure(ADC0_BASE, 16); //Average 4 readings
    //ADCReferenceSet(ADC0_BASE, ADC_REF_INT);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0); //Base, Seq Num, Trigger source, Priority
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END); // Base, Seq Num, Step, Config

    //Config: produce interrupt when step is complete
    ADCSequenceEnable(ADC0_BASE, 3); // Enable sequencer 3

    // Interrupt enable
    ADCIntClear(ADC0_BASE, 3);
    ADCIntEnable(ADC0_BASE, 3);
    IntEnable(INT_ADC0SS3);

    //Timer0
    // It acts as the trigger source
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    samplePeriod = SysCtlClockGet()/sampleFreq;
    TimerLoadSet(TIMER0_BASE, TIMER_A, samplePeriod-1);
    TimerControlTrigger(TIMER0_BASE, TIMER_A, true);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

//------------------getLoadCellValue()---------------------------
//Get Load Cell Value
//Input: None
//Output: Load Cell value ADC units
uint32_t getLoadCellValue(){
    return loadCellValue[0];
}

//------------------measuredLoad()---------------------------
//Get Load Cell Value
//Input: None
//Output: Load Cell value in pounds
double measuredLoad(void){
    uint32_t loadADC;
    double OutputLoad;

    loadADC = getLoadCellValue();
    OutputLoad = Vol2Load(adc2Vol(loadADC));
    return OutputLoad;
}

//------------------getLoadCellValue()---------------------------
//Get Load Cell Value
//Input: None
//Output: Load Cell value
double adc2Vol(uint32_t adcRead){
    double output;
    output = ((adcRead*3.3)/4095);
    return output;
}

//------------------Vol2Load()---------------------------
//Covert raw voltage value to force in pound
//Input: Raw Volage
//Output: Pound
double Vol2Load(double vol){
    double output;
    output = vol*25.0;
    return output;
}
//Interrupts
// Timer 0A is being used to trigger load cell ADC
void LoadCellTrigger(void){
    TimerIntClear(ADC0_BASE, 3);
}

// Handler for ADC Load cell
void LoadCellIntHandler(void){
    ADCIntClear(ADC0_BASE, 3);
   // while(!ADCIntStauts(ADC0_BASE, 3, false)){}
    ADCSequenceDataGet(ADC0_BASE, 3, loadCellValue);
}

// ********************************************************
// ********************* Motor ****************************
// ********************************************************
//------------------Motor_Init()---------------------------
//Initialize the Motor
//Input: None
//Output: None
void Motor_Init(uint32_t PWMFreq){

    pwmPeriod = PWMclockFreq/PWMFreq;

    /* PWM clock: systemClock/2 = 80/2 MHZ */
    SysCtlPWMClockSet(SYSCTL_PWMDIV_2);

    /*Enable Peripheral Direction pin*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}

    /* Enable PWM*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); // Module 1 for PF1
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //PF1

    // Configure direction pin
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7); //PB7

    // Configure PWM pin
    GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);

    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, (int)pwmPeriod);

    // Enable the PWM generator
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 0); //Turn OFF
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

}

//------------------Motor_SetDuty()---------------------------
//Set Duty cycle for the motor
//Input: Target Duty Cycle 0-100 (percent)
//Output: None
void Motor_SetDuty(uint32_t dutyCycle){
    pwmDuty = dutyCycle*((float)getMotorPWMPeriod()/100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, pwmDuty);
}


//------------------convert2PWMDuty()---------------------------
//Set Duty cycle for the motor
//Input: Target Duty Cycle (in percent - 0:100)
//Output: Command to send to PWM
double convert2PWMDuty(uint32_t dutyCycle){
    pwmDuty = ((float)dutyCycle/100)*(getMotorPWMPeriod());
    return pwmDuty;
}

//------------------motorSendCommand()---------------------------
//Sends the final commands to the motor driver
//Input: duty cycle, direction
//Output: None
void motorSendCommand(uint32_t dutyCycle, int direction){
    double dutyCycleApplied = convert2PWMDuty(dutyCycle);
    setglobals4Motor(dutyCycleApplied, direction);

    if(direction == 0){
        clearDirection();
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, dutyCycleApplied);
    }else{
        setDirection();
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, dutyCycleApplied);
    }

    if(dutyCycle == 0){
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, dutyCycleApplied);
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
    }else{
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
    }

}

//------------------getMotorPWMPeriod()---------------------------
//Get the global variable period
//Input: period
//Output: None
double getMotorPWMPeriod(void){
    return pwmPeriod;
}

//------------------setDirection()---------------------------
//Set Direction Pin High
//Input: None
//Output: None
void setDirection(){
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);
}

//------------------clearDirection()---------------------------
//Clear Direction Pin Low
//Input: None
//Output: None
void clearDirection(){
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, ~(GPIO_PIN_7));
}

//------------------enableMotor()---------------------------
//Enables the motor
//Input: None
//Output: None
void enableMotor(){
    // Turn off the Output pins
     PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
}

//------------------disableMotor()---------------------------
//Disables the motor
//Input: None
//Output: None
void disableMotor(){
    // Turn off the Output pins
     PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
}

//------------------setglobals4Motor()---------------------------
//Set global variable duty and direction
//Input: Duty cycle, Direction
//Output: None
void setglobals4Motor(double duty, int direction){
    globalDutyCycle = duty;
    globalDirection = direction;
}

//------------------getglobalduty()---------------------------
//Get global variable duty and direction
//Input: None
//Output: Duty cycle
double getglobalduty(void){
    return globalDutyCycle;
}

//------------------getglobaldirection()---------------------------
//Get global variable duty and direction
//Input: None
//Output: Direction
int getglobaldirection(void){
    return globalDirection;
}

//------------------checkLimits()---------------------------
//Check duty cycle limit
//Input: Duty Cycle
//Output: None
void checkLimits(double duty){
    int direction;
    if(duty<0){
        direction = 0;
        duty = -1*duty;
    }else{
        direction = 1;
    }

    if(duty > 100){
        duty = 100;
    }
    motorSendCommand(duty, direction);
}


// ********************************************************
// ****************** Controller **************************
// ********************************************************
//------------------Force control loop --------------------
void ControllerIntHandler(void){
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //check if goal reached
    if(~getGoalFlag()){

        //calculate error
        ERROR = (getGoalForce() - measuredLoad());

        //Goal reaching criteria
        if(ERROR < 0.05){
            setGoalFlag(1);
            RGBled_Set(0, 1, 0); //turn on Blue
        }

            Kp = Kbar*abs(ERROR);
            P = Kp*ERROR;

            D = Kd*(ERROR-LAST_ERROR);
            LAST_ERROR = ERROR;

            TOTAL_ERROR = TOTAL_ERROR + ERROR;
            TOTAL_ERROR = checkIntegralLimit(TOTAL_ERROR);
            I = Ki*TOTAL_ERROR;

            PID_OUT = P + D + I;
    }else{
        PID_OUT = 0;
    }
    checkLimits(PID_OUT);
}

//------------------Controller_Init()---------------------------
//Initializes a timer routine for the controller
//Input: None
//Output: None
void Controller_Init(uint32_t Controllerfreq){
        uint32_t periods; // Timer delays
        TOTAL_ERROR = 0;
        setGoalFlag(0);
        //Configure Timer
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        // wait for timer module 1 to be ready
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1))
            {
            }

        TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

        // set timer 1 to run off of system clock
       // TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);

        // Define period
        periods = (SysCtlClockGet()/Controllerfreq);
        //periods = (clockFreq/Controllerfreq);
        TimerLoadSet(TIMER1_BASE, TIMER_A, periods-1);

        // register the timer interrupt service routine
        TimerIntRegister(TIMER1_BASE, TIMER_A, *ControllerIntHandler);

        // clear rollover interrupt and then enable it
        TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

        IntEnable(INT_TIMER1A);
}

//------------------ControllerEnable()---------------------------
//Enable Timer 1A
//Input: None
//Output: None
void ControllerEnable(){
    //Enable Timer
    TimerEnable(TIMER1_BASE, TIMER_A);
}


//------------------checkIntegralLimit()---------------------------
//Wrap up integral Error
//Input: None
//Output: None
double checkIntegralLimit(double e){
    if(e>MaxSteadyError)
        e = MaxSteadyError;
    if(e<MinSteadyError)
        e = MinSteadyError;
    return e;
}

//------------------deadBandCheck()---------------------------
//Dead Band for PID loop
//Input: None
//Output: flag of whether goal reached or not
uint32_t deadBandCheck(double error){
    if (abs(error)<DEADBAND){
        return 1;
    }else{
        return 0;
    }
}
//------------------getGoalFlag()---------------------------
//Returns controller flag variable
//Input: None
//Output: None
uint32_t getGoalFlag(){
    return goalReached;
}

//------------------setGoalFlag()---------------------------
//Returns controller flag variable
//Input: None
//Output: None
void setGoalFlag(uint32_t g){
    goalReached = g;
}


//------------------setGoalForce()---------------------------
//Set the global variable RefForce
//Input: Ref Force
//Output: None
void setGoalForce(double ref){
    goalPos = ref;
}

//------------------getGoalForce()---------------------------
//Get the global variable RefForce
//Input: Ref Force
//Output: None
double getGoalForce(void){
    return goalPos;
}

//------------------getKp()---------------------------
//Gep proportional gain
//Input: None
//Output: Kp
double getKp(void){
    return Kp;
}


//------------------getError()---------------------------
//Get Error in PID
//Input: None
//Output: Error
double getError(void){
    return ERROR;
}

//------------------getPIDoutput()---------------------------
//Get output in PID
//Input: None
//Output: out
double getPIDoutput(void){
    return PID_OUT;
}

//------------------Force control loop -----------------------
void LoggerIntHandler(void){
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    //only for conversions
    int intload, fracload, intErr, fracErr, intPWM, fracPWM, dir, intgDuty, fracgDuty;
    int sign = 1;
    double loadCellOut, error, OutDuty, gDuty;

    //Measure Load Cell value
    loadCellOut = measuredLoad();
    intload = loadCellOut;
    fracload = (loadCellOut - intload)*1000;

    //Measure Error
    error = getError();
    if(error<0){
        sign = -1;
        error = error*sign;
    }
    intErr = error*sign;
    fracErr = (error-intErr)*1000;

    //PWM output
    OutDuty = getPIDoutput();
    if(OutDuty<0){
        sign = -1;
        OutDuty = OutDuty*sign;
    }
    intPWM = OutDuty*sign;
    fracPWM = (OutDuty-intPWM)*1000;

    //Direction
    dir = getglobaldirection();

    //Global duty
    gDuty = getglobalduty();
    intgDuty = gDuty;
    fracgDuty = (gDuty-intgDuty)*1000;

    UARTprintf("%d.%3d, %d.%3d, %d.%3d, %d, %d, %d.%3d\n", intload, fracload,intErr, fracErr, intPWM, fracPWM, dir, getGoalFlag(), intgDuty, fracgDuty);

}

//------------------Logger_Init()---------------------------
//Initializes a timer routine for the controller
//Input: None
//Output: None
void Logger_Init(uint32_t LoggerFreq){
        uint32_t periods; // Timer delays

        //Configure Timer
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        // wait for timer module 1 to be ready
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2))
            {
            }

        TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);

        // set timer 1 to run off of system clock
       // TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);

        // Define period
        periods = (SysCtlClockGet()/LoggerFreq);
        //periods = (clockFreq/Controllerfreq);
        TimerLoadSet(TIMER2_BASE, TIMER_A, periods-1);

        // register the timer interrupt service routine
        TimerIntRegister(TIMER2_BASE, TIMER_A, *LoggerIntHandler);

        // clear rollover interrupt and then enable it
        TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

        IntEnable(INT_TIMER2A);

        TimerEnable(TIMER2_BASE, TIMER_A);
}
