//*****************************************************************************
// ADC connect with interrupt


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

//*****************************************************************************

//! This example shows how to setup ADC0 to read the internal temperature
//! sensor.
//! - UART0 peripheral
//! - GPIO Port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!

//**********Function Prototype***********
void initConsole(void);
void initTempSensor(void);
void initLED(void);
void setRedLed(void);
void clearRedLed(void);
void setBlueLed(void);
void clearBlueLed(void);

//**********Constants*********************
const int sampleFreq = 8000;

//**********Variables*********************
uint32_t ADC0Value[1];
uint16_t samplePeriod;

void main(){
    volatile uint32_t TempValueC;
    volatile uint32_t TempValueF;
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                           SYSCTL_OSC_MAIN);  // Setup system clock at 40MHz from PLL with Crystal

    initLED();
    initConsole();

    initTempSensor();

    IntMasterEnable();


    while(1){
        setRedLed();
        //vol = ADC0Value[0]*(.000805664);
        TempValueC = (1475 - ((2475*ADC0Value[0]))/4096)/10;
        TempValueF = ((TempValueC*9)+160)/5;

        UARTprintf("Temp Sensor reading is: %04u, *C is: %04u, *F is: %04u\n", ADC0Value[0], TempValueC, TempValueF);
        SysCtlDelay( (SysCtlClockGet()/(3*1000))*5 ) ;
    }
}

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


void initTempSensor(){
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
    ADCHardwareOversampleConfigure(ADC0_BASE, 4); //Average 4 readings
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

void initLED(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
           } //Enable Peripheral

        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

}

void setRedLed(){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
}

void clearRedLed(){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIO_PIN_1);
}

void setBlueLed(){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

void clearBlueLed(){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
}
//Interrupts
void timer0IntHandler(void){
    TimerIntClear(ADC0_BASE, 3);
}

void ADC0SS3IntHandler(void){
    setBlueLed();
    ADCIntClear(ADC0_BASE, 3);
   // while(!ADCIntStauts(ADC0_BASE, 3, false)){}
    clearBlueLed();
    ADCSequenceDataGet(ADC0_BASE, 3, ADC0Value);
}
