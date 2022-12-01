#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"

uint32_t SysClock;
int led = 1;
int counter = 0;
int lastCaracter = 0;
volatile int32_t caracter;
uint32_t porta;
int cont = 0;

//defines para resolver problemas de import (provisório)
#define GPIO_PE7_NMI            0x00041C08
#define GPIO_PF0_M0PWM0         0x00050006

void setupSystick(void);
void incrementaContador(void);

void enviaString(const uint8_t *Buffer, uint32_t tamanho)
{
    while(tamanho--)
    {
        UARTCharPutNonBlocking(UART0_BASE, *Buffer++);
    }
}

void handlerDaUART(void)
{
    uint32_t Status;
    
    Status = UARTIntStatus(UART0_BASE, true);                     
    UARTIntClear(UART0_BASE, Status);                           
    
    while(UARTCharsAvail(UART0_BASE)) 
    {
        // Para fazer a conversao
        caracter = UARTCharGetNonBlocking(UART0_BASE); 
       
        UARTCharPutNonBlocking(UART0_BASE, caracter); 
    }

}

void setup()
{
    uint32_t SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | 
                                   SYSCTL_OSC_MAIN | 
                                   SYSCTL_USE_PLL | 
                                   SYSCTL_CFG_VCO_240), 120000000);
    
    //Configura UART usando funções prontas do C++ (substitui parte manual do assembly)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

       
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    
    //interrupção habilitada
    IntMasterEnable();
    
    //Configura o RX (Reciever) e TX(Transmitter)
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4);
    
    UARTConfigSetExpClk(UART0_BASE, SysClock, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); // Configura clock da UART
    
    //Habilita a UART0
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    
}

void setupPwm(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  GPIOPinConfigure(GPIO_PF0_M0PWM0);

  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
  
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));
  
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 200000);
  
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 200);
  
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);
  PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT | PWM_OUT_3_BIT), true);
}

void setupSystick(void) {
  
  // Disable Systick in order to avoid trash
  SysTickDisable();
  // The clock is 120MHz
  // Configure the SysTick counter.
  // Every 1000 flag makes 1 milisecond
  SysTickPeriodSet(120000-1-12-6);
  // Pointer to a interruption
  SysTickIntRegister(incrementaContador);
  // Enable the SysTick as an Interruption.
  SysTickIntEnable();
  // Enables SysTick - counter
  SysTickEnable();
}
  
void altera_pwm(void) {
  if (caracter != lastCaracter && caracter >= 49 && caracter <= 52){
 
    //ALtera Frequencia e DutyCycle
    if (caracter == 49) {
      PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400);
      PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 200);
    }
    if (caracter == 50) {
      PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 500);
      PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 400);
    }
    if (caracter == 51) {
      PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 600);
      PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 100);
    }
    if (caracter == 52) {
      PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 700);
      PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 300);
    }
    lastCaracter = caracter;
  }
}

void incrementaContador()
{
  cont++;
}

int main(void)
{
    setup();
    setupPwm();
    setupSystick();
    enviaString((uint8_t *)"Modo: ", 6); //chama função para imprimir string
    while(1)
    {
      altera_pwm();
      
    }
}