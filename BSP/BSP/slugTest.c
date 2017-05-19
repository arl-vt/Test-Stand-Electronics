#include <stdint.h>
#include "slug.h"

uint8_t Red = 0, Green = 0, Blue = 0;

int main(void){
    uint32_t period = 2000; //clock/freq: 20MHz/10KHZ
    Clock_set_fastest();
    //RGBled_Init(0, 1, 1); // Initialize all
    //Button1_Init();
    Motor_Init(period); // Initialize with 10KHZ

    while(1){
        // Test RGB LEDS
        /*RGBled_Set(1, 1, 0);
        Delay_cycle(2000000);
        RGBled_Set(1, 0, 0);
        Delay_cycle(2000000);
        */

        // Test Toggle Behavior
        /*RGBled_Toggle(1, 0, 0);
        Delay_cycle(2000000);
        RGBled_Toggle(0, 1, 0);
        Delay_cycle(2000000);
        */

        //Test Button input
        /*if(Button1_Input()==0){
            RGBled_Set(0, 1, 0); //Set green on Press
        }else{
            RGBled_Set(0, 0, 1); // Set Blue on Release
        }*/

        //Test Motor
        enableMotor();
        Motor_SetDuty(.01, period);
        delayMS(1000);
        disableMotor();
        delayMS(1000);

    }
}

