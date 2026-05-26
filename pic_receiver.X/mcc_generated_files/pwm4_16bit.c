/**
  PWM4_16BIT Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    pwm4_16bit.c

  @Summary
    This is the generated driver implementation file for the PWM4_16BIT driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for PWM4_16BIT.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F26Q84
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above or later
        MPLAB             :  MPLAB X 6.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "pwm4_16bit.h"

//Pointers to PWM4_16BIT interrupt handlers
//User can use them in application code to initialize with custom ISRs
static void (*PWM4_16BIT_Slice1Output1_InterruptHandler)(void);   //SaP1IF and hence PWMxIF is set
static void (*PWM4_16BIT_Slice1Output2_InterruptHandler)(void);   //SaP2IF and hence PWMxIF is set
static void (*PWM4_16BIT_Period_InterruptHandler)(void);          //PWMxPIF is set
static void PWM4_16BIT_Slice1Output1_DefaultInterruptHandler(void);
static void PWM4_16BIT_Slice1Output2_DefaultInterruptHandler(void);
static void PWM4_16BIT_Period_DefaultInterruptHandler(void);

void PWM4_16BIT_Initialize(void)
{
    //PWMERS External Reset Disabled; 
    PWM4ERS = 0x00;

    //PWMCLK FOSC; 
    PWM4CLK = 0x02;

    //PWMLDS Autoload disabled; 
    PWM4LDS = 0x00;

    //PWMPRL 255; 
    PWM4PRL = 0xFF;

    //PWMPRH 124; 
    PWM4PRH = 0x7C;

    //PWMCPRE No prescale; 
    PWM4CPRE = 0x00;

    //PWMPIPOS No postscale; 
    PWM4PIPOS = 0x00;

    //PWMS1P2IF PWM2 output match did not occur; PWMS1P1IF PWM1 output match did not occur; 
    PWM4GIR = 0x00;

    //PWMS1P2IE disabled; PWMS1P1IE disabled; 
    PWM4GIE = 0x00;

    //PWMPOL2 disabled; PWMPOL1 disabled; PWMPPEN disabled; PWMMODE Left aligned mode; 
    PWM4S1CFG = 0x00;

    //PWMS1P1L 192; 
    PWM4S1P1L = 0xC0;

    //PWMS1P1H 123; 
    PWM4S1P1H = 0x7B;

    //PWMS1P2L 192; 
    PWM4S1P2L = 0xC0;

    //PWMS1P2H 123; 
    PWM4S1P2H = 0x7B;
    
    //Clear PWM4_16BIT period interrupt flag
    PIR9bits.PWM4PIF = 0;
    
    //Clear PWM4_16BIT interrupt flag
    PIR9bits.PWM4IF = 0;
    
    //Clear PWM4_16BIT slice 1, output 1 interrupt flag
    PWM4GIRbits.S1P1IF = 0;
    
    //Clear PWM4_16BIT slice 1, output 2 interrupt flag
    PWM4GIRbits.S1P2IF = 0;
    
    //PWM4_16BIT interrupt enable bit
    PIE9bits.PWM4IE = 0;
    
    //PWM4_16BIT period interrupt enable bit
    PIE9bits.PWM4PIE = 0;
    
    //Set default interrupt handlers
    PWM4_16BIT_Slice1Output1_SetInterruptHandler(PWM4_16BIT_Slice1Output1_DefaultInterruptHandler);
    PWM4_16BIT_Slice1Output2_SetInterruptHandler(PWM4_16BIT_Slice1Output2_DefaultInterruptHandler);
    PWM4_16BIT_Period_SetInterruptHandler(PWM4_16BIT_Period_DefaultInterruptHandler);

    //PWMEN enabled; PWMLD disabled; PWMERSPOL disabled; PWMERSNOW disabled; 
    PWM4CON = 0x80;
}

void PWM4_16BIT_Enable()
{
    PWM4CON |= _PWM4CON_EN_MASK;
}

void PWM4_16BIT_Disable()
{
    PWM4CON &= (~_PWM4CON_EN_MASK);
}

void PWM4_16BIT_WritePeriodRegister(uint16_t periodCount)
{
    PWM4PRL = (uint8_t)periodCount;
    PWM4PRH = (uint8_t)(periodCount >> 8);
}

void PWM4_16BIT_SetSlice1Output1DutyCycleRegister(uint16_t registerValue)
{    
    PWM4S1P1L = (uint8_t)(registerValue);
    PWM4S1P1H = (uint8_t)(registerValue >> 8);
}

void PWM4_16BIT_SetSlice1Output2DutyCycleRegister(uint16_t registerValue)
{        
    PWM4S1P2L = (uint8_t)(registerValue);
    PWM4S1P2H = (uint8_t)(registerValue >> 8);
}

void PWM4_16BIT_LoadBufferRegisters(void)
{
    //Load the period and duty cycle registers on the next period event
    PWM4CONbits.LD = 1;
}

void PWM4_16BIT_PWMI_ISR(void)
{
    PIR9bits.PWM4IF = 0;
    if((PWM4GIEbits.S1P1IE == 1) && (PWM4GIRbits.S1P1IF == 1))
    {
        PWM4GIRbits.S1P1IF = 0;
        if(PWM4_16BIT_Slice1Output1_InterruptHandler != NULL)
            PWM4_16BIT_Slice1Output1_InterruptHandler();
    }
    else if((PWM4GIEbits.S1P2IE == 1) && (PWM4GIRbits.S1P2IF == 1))
    {
        PWM4GIRbits.S1P2IF = 0;
        if(PWM4_16BIT_Slice1Output2_InterruptHandler != NULL)
            PWM4_16BIT_Slice1Output2_InterruptHandler();
    }
}

void PWM4_16BIT_PWMPI_ISR(void)
{
    PIR9bits.PWM4PIF = 0;
    if(PWM4_16BIT_Period_InterruptHandler != NULL)
        PWM4_16BIT_Period_InterruptHandler();
}

void PWM4_16BIT_Slice1Output1_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM4_16BIT_Slice1Output1_InterruptHandler = InterruptHandler;
}

void PWM4_16BIT_Slice1Output2_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM4_16BIT_Slice1Output2_InterruptHandler = InterruptHandler;
}

void PWM4_16BIT_Period_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM4_16BIT_Period_InterruptHandler = InterruptHandler;
}

static void PWM4_16BIT_Slice1Output1_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use PWM4_16BIT_Slice1Output1_SetInterruptHandler() function to use Custom ISR
}

static void PWM4_16BIT_Slice1Output2_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use PWM4_16BIT_Slice1Output2_SetInterruptHandler() function to use Custom ISR
}

static void PWM4_16BIT_Period_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use PWM4_16BIT_Period_SetInterruptHandler() function to use Custom ISR
}
