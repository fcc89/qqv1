/**
  TMR3 Generated Driver API Header File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr3.h

  @Summary
    This is the generated header file for the TMR3 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for TMR3. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.166.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.30
        MPLAB 	          :  MPLAB X v5.30
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _TMR3_H
#define _TMR3_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

#define TMR3_INTERRUPT_TICKER_FACTOR    512

/**
  Section: Interface Routines
*/

/**
  @Summary
    Initializes hardware and data for the given instance of the TMR module

  @Description
    This routine initializes hardware for the instance of the TMR module,
    using the hardware initialization given data.  It also initializes all
    necessary internal data.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    bool statusTimer1;
    uint16_t period;
    uint16_t value;

    period = 0x20;

    TMR3_Initialize();

    TMR3_Period16BitSet(period);

    if((value = TMR3_Period16BitGet())== period)
    {
        TMR3_Start();
    }

    while(1)
    {
        TMR3_Tasks();
        if( (statusTimer1 = TMR3_GetElapsedThenClear()) == true)
        {
            TMR3_Stop();
        }
    }
    </code>
*/
void TMR3_Initialize (void);


/**
  @Summary
    Updates 16-bit timer value

  @Description
    This routine updates 16-bit timer value

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of TMR3_Initialize();
*/

void TMR3_Period16BitSet( uint16_t value );

/**

  @Summary
    Provides the timer 16-bit period value

  @Description
    This routine provides the timer 16-bit period value

  @Param
    None.

  @Returns
    Timer 16-bit period value
 
  @Example 
    Refer to the example of TMR3_Initialize();
*/

uint16_t TMR3_Period16BitGet( void );

/**
  @Summary
    Updates the timer's 16-bit value

  @Description
    This routine updates the timer's 16-bit value

  @Param
    None.

  @Returns
    None

  @Example 
    <code>
    uint16_t value=0xF0F0;

    TMR3_Counter16BitSet(value));

    while(1)
    {
        TMR3_Tasks();
        if( (value == TMR3_Counter16BitGet()))
        {
            TMR3_Stop();
        }
    }
    </code>
*/

void TMR3_Counter16BitSet ( uint16_t value );

/**
  @Summary
    Provides 16-bit current counter value

  @Description
    This routine provides 16-bit current counter value

  @Param
    None.

  @Returns
    16-bit current counter value
 
  @Example 
    Refer to the example of TMR3_Counter16BitSet();
*/

uint16_t TMR3_Counter16BitGet( void );

/**
  @Summary
    Assigns a function pointer with a callback address.

  @Description
    This routine assigns a function pointer with a callback address.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        TMR3_SetInterruptHandler(&TMR3_CallBack);
    </code>
*/

void TMR3_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Starts the TMR

  @Description
    This routine starts the TMR

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of TMR3_Initialize();
*/

void TMR3_Start( void );

/**
  @Summary
    Stops the TMR

  @Description
    This routine stops the TMR

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of TMR3_Initialize();
*/

void TMR3_Stop( void );

/**
  @Summary
    Returns the elapsed status of the timer and clears if flag is set.

  @Description
    This routine returns the elapsed status of the timer and clears 
    flag if its set.

  @Param
    None.

  @Returns
    True - Timer has elapsed.
    False - Timer has not elapsed.
 
  @Example 
    Refer to the example of TMR3_Initialize();
*/

bool TMR3_GetElapsedThenClear(void);

/**
  @Summary
    Returns the software counter value.

  @Description
    This routine returns the software counter value.

  @Param
    None.

  @Returns
    Software counter value.
 
  @Example 
    Refer to the example of TMR3_Initialize();
*/

int TMR3_SoftwareCounterGet(void);

/**
  @Summary
    Clears the software counter value.

  @Description
    This routine clears the software counter value.

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of TMR3_Initialize();
*/

void TMR3_SoftwareCounterClear(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_TMR3_H

/**
 End of File
*/
