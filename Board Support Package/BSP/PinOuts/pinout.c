//*****************************************************************************
//
// Configure the device pins for different signals
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

// This file was automatically generated on 6/14/2017 at 8:55:52 PM
// by TI PinMux version 4.0.1485 
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "pinout.h"

//*****************************************************************************
//
//! \addtogroup pinout_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Configures the device pins for the customer specific usage.
//!
//! \return None.
//
//*****************************************************************************
void
PinoutSet(void)
{
    //
    // Enable Peripheral Clocks 
    //
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Configure the GPIO Pin Mux for PC5
	// for PHA1
    //
	MAP_GPIOPinConfigure(GPIO_PC5_PHA1);
	MAP_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);

    //
    // Configure the GPIO Pin Mux for PC4
	// for IDX1
    //
	MAP_GPIOPinConfigure(GPIO_PC4_IDX1);
	MAP_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_4);

    //
    // Configure the GPIO Pin Mux for PC6
	// for PHB1
    //
	MAP_GPIOPinConfigure(GPIO_PC6_PHB1);
	MAP_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);

    //
    // Configure the GPIO Pin Mux for PE3
	// for AIN0
    //
	MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    //
    // Configure the GPIO Pin Mux for PD1
	// for AIN6
    //
	MAP_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);

    //
    // Configure the GPIO Pin Mux for PE2
	// for AIN1
    //
	MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);

    //
    // Configure the GPIO Pin Mux for PE0
	// for AIN3
    //
	MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);

    //
    // Configure the GPIO Pin Mux for PB7
	// for GPIO_PB7
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7);

    //
    // Configure the GPIO Pin Mux for PB6
	// for GPIO_PB6
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);

    //
    // Configure the GPIO Pin Mux for PF4
	// for GPIO_PF4
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);

    //
    // Configure the GPIO Pin Mux for PE1
	// for GPIO_PE1
    //
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);

    //
    // Configure the GPIO Pin Mux for PF1
	// for M1PWM5
    //
	MAP_GPIOPinConfigure(GPIO_PF1_M1PWM5);
	MAP_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

	//
	// Unlock the Port Pin and Set the Commit Bit
	//
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR)   |= GPIO_PIN_0;
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = 0x0;
	
    //
    // Configure the GPIO Pin Mux for PF0
	// for SSI1RX
    //
	MAP_GPIOPinConfigure(GPIO_PF0_SSI1RX);
	MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_0);

    //
    // Configure the GPIO Pin Mux for PF3
	// for SSI1FSS
    //
	MAP_GPIOPinConfigure(GPIO_PF3_SSI1FSS);
	MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_3);

    //
    // Configure the GPIO Pin Mux for PD3
	// for SSI1TX
    //
	MAP_GPIOPinConfigure(GPIO_PD3_SSI1TX);
	MAP_GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_3);

    //
    // Configure the GPIO Pin Mux for PF2
	// for SSI1CLK
    //
	MAP_GPIOPinConfigure(GPIO_PF2_SSI1CLK);
	MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_2);

    //
    // Configure the GPIO Pin Mux for PA4
	// for SSI0RX
    //
	MAP_GPIOPinConfigure(GPIO_PA4_SSI0RX);
	MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_4);

    //
    // Configure the GPIO Pin Mux for PA3
	// for SSI0FSS
    //
	MAP_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_3);

    //
    // Configure the GPIO Pin Mux for PA5
	// for SSI0TX
    //
	MAP_GPIOPinConfigure(GPIO_PA5_SSI0TX);
	MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5);

    //
    // Configure the GPIO Pin Mux for PA2
	// for SSI0CLK
    //
	MAP_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2);

    //
    // Configure the GPIO Pin Mux for PA6
	// for I2C1SCL
    //
	MAP_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	MAP_GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);

    //
    // Configure the GPIO Pin Mux for PA7
	// for I2C1SDA
    //
	MAP_GPIOPinConfigure(GPIO_PA7_I2C1SDA);
	MAP_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

