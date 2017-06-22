#include <stdint.h>
#include "../slug.h"
#include "utils/uartstdio.h"

void BlueledTest(void);

int mainc(void){

    Clock_set_fastest();

    BlueledTest();
}

// Test LED
void BlueledTest(){
    BlueLED_Init();
    BlueLED_Set();
}
