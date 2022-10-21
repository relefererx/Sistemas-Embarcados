//*****************************************************************************
//
// project0.c - Example to demonstrate minimal TivaWare setup
//
// Copyright (c) 2012-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.2.0.295 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "tm4c1294ncpdt.h"


//*****************************************************************************
//
// Define pin to LED mapping.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Project Zero (project0)</h1>
//!
//! This example demonstrates the use of TivaWare to setup the clocks and
//! toggle GPIO pins to make the LED blink. This is a good place to start
//! understanding your launchpad and the tools that can be used to program it.
//
//*****************************************************************************

#define USER_LED1  GPIO_PIN_0
#define USER_LED2  GPIO_PIN_1

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

volatile unsigned long clk_count = 0;
volatile uint32_t tempo = 0;

void habilitaSystickInt(void);
void desabilitaSystickInt(void);

void HANDLER_SYSTICK(void){  
  if(tempo < 2999 ){
    tempo ++;
 }
  else{
    printf("\nTempo limite atingido\n");
    desabilitaSystickInt();
  }
}

void HANDLER_GPIOJ(void){
   GPIOPinWrite(GPIO_PORTN_BASE, USER_LED1, USER_LED1);
   GPIOIntClear(GPIO_PORTJ_BASE, GPIO_PIN_0);
   clk_count = (unsigned long)(((float)tempo/1000)/(float)((float)1/120000000));
   printf("\nTempo de reação: %d ms equivalente a %lu clocks", tempo, clk_count);
   desabilitaSystickInt();
   
}

void habilitaSystickInt(void){
    NVIC_ST_RELOAD_R  = 120000;
    NVIC_ST_CURRENT_R  = 0;
    
    SysTickIntRegister(HANDLER_SYSTICK); 
    SysTickPeriodSet(120000); //CPU_CLOCK / ( 1 / time_unit )
    SysTickIntEnable();
     
    SysTickEnable();
}

void desabilitaSystickInt(void){
    SysTickIntDisable();
}


//*****************************************************************************
//
// Main 'C' Language entry point.  Toggle an LED using TivaWare.
//
//*****************************************************************************
int
main(void)
{
    uint32_t ui32SysClock;
    uint32_t clock;

    //
    // Run from the PLL at 120 MHz.
    // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
    // later to better reflect the actual VCO speed due to SYSCTL#22.
    //
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_240), 120000000);

    //
    // Enable and wait for the port to be ready for access
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }
    
    //
    // Configure the GPIO port for the LED operation.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, (USER_LED1|USER_LED2));

    // Pin J0 setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);       // Enable port J
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);	// Init PJ0 as input
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Enable weak pullup resistor for PJ0
    
    // Interrupt setup
    GPIOIntDisable(GPIO_PORTJ_BASE, GPIO_PIN_0);        // Disable interrupt for PJ0 (in case it was enabled)
    GPIOIntClear(GPIO_PORTJ_BASE, GPIO_PIN_0);		// Clear pending interrupts for PJ0
    GPIOIntRegister(GPIO_PORTJ_BASE, HANDLER_GPIOJ);	// Register our handler function for port J
    GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);				// Configure PJ0 for falling edge trigger
    GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_PIN_0);		// Enable interrupt for PJ0
   
    habilitaSystickInt();
    
    GPIOPinWrite(GPIO_PORTN_BASE, USER_LED2, USER_LED2); //Acende led para inicio do jogo
    
    //
    // Loop Forever
    //
    while(1)
    {

    }
}
