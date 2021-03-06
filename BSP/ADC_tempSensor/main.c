/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"

//------------------tempSensor_init()---------------------------
//Initialize the Temperature Sensor on Board
//Input: None
//Output: None
void tempSensor_init(){
    // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Configure Sequencer, sample sequencer 1, processor triggered - 1 MSPS, highest priority
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS); // ADC_CTL_TS is the temp sensor
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
    // Configure interrupt flag to be set on last sample and notify that this is last sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);

    //Enable ADC sequencer 1
    ADCSequenceEnable(ADC0_BASE, 1);

}

int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                            SYSCTL_OSC_MAIN);  // Setup system clock at 40MHz from PLL with Crystal

    uint32_t TempSensorValue[4];
    volatile uint32_t TempAvg;
    volatile uint32_t TempValueC;
    volatile uint32_t TempValueF;

    tempSensor_init();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    while(1){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    ADCIntClear(ADC0_BASE, 1);
    ADCProcessorTrigger(ADC0_BASE, 1);

    while(!ADCIntStatus(ADC0_BASE, 1, false)){
    }

    ADCSequenceDataGet(ADC0_BASE, 1, TempSensorValue);
    TempAvg = (TempSensorValue[0] + TempSensorValue[1] + TempSensorValue[2] + TempSensorValue[3] + 2)/4;
    TempValueC = (1475 - ((2475*TempAvg))/4096)/10;
    TempValueF = ((TempValueC*9)+160)/5;

    }
}
