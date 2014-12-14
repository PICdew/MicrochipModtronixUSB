/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        main.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 ********************************************************************/
//#define SBC44UC_BOARD
//#define EXAMPLE1
//#define EXAMPLE2

/////////////////////////////////////////////////
// Includes
// add thisinclude directory to the project settings in the IDE
// C:\Program Files (x86)\Microchip\mplabc18\v3.46\h
#include <p18cxxx.h>
#include "system\typedefs.h"                        // Required
#include "system\usb\usb.h"                         // Required
#include "io_cfg.h"                                 // Required

#include "system\usb\usb_compile_time_validation.h" // Optional
#include "user\user.h"                              // Modifiable

#include "projdefs.h"
#include "mxmc18\tick.h"                            // Modtronix library

#include "eeprom_data.h"                            //EEPROM initialization

/////////////////////////////////////////////////
// Varaiables
// udata means data memory, contains statically
// allocated uninitialized user variables.
#pragma udata


/////////////////////////////////////////////////
// Prototypes
static void InitializeSystem(void);
void USBTasks(void);
void high_isr(void);
void low_isr(void);
void BlinkUSBStatus(void);
/////////////////////////////////////////////////
// Vector Remapping required for bootloader. Bootloader is from 0 - 0x7ff
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x000800
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code

#pragma code _HIGH_INTERRUPT_VECTOR = 0x000808
void _high_ISR (void)
{
    _asm goto high_isr _endasm
}
#pragma code

#pragma code _LOW_INTERRUPT_VECTOR = 0x000818
void _low_ISR (void)
{
    _asm goto low_isr _endasm
}
#pragma code

/////////////////////////////////////////////////
//Low Interrupt ISR
#pragma interrupt low_isr
void low_isr(void) {
}
#pragma code

/////////////////////////////////////////////////
//High Interrupt ISR
#pragma interrupt high_isr
void high_isr(void) {
    //static word isrTmp = 0;
    
    //TMR2 is used for the ticks
    if (PIR1bits.TMR2IF)
    {   
        PIR1bits.TMR2IF = 0;
        tickService();

        // TEST TEST
       //if (isrTmp == 0) {
       //    isrTmp = 500;
       //    mLED_1 = !mLED_1;   // TEST TEST
       //}
       //isrTmp--;
    }
}
#pragma code

/**
 * Main function
 * Main program entry point.
 *
 */
void main(void)
{
    InitializeSystem();

    tickInit();

    while(1)
    {
        USBTasks();
        BlinkUSBStatus();
        if (!((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)))
           ProcessIO();  // See user\user.c & .h
    }
}

/**
 * InitializeSystem is a centralize initialization routine.
 * All required USB initialization routines are called from here.
 *
 * User application initialization routine should also be
 * called from here.
 *
 */
static void InitializeSystem(void)
{
    ADCON1 |= 0x0F; // Default all pins to digital

    /////////////////////////////////////////////////
    //Setup timer2 as a 1ms timer
    /*
    //Prescaler of 16 at 48MHz = 48,000,000 / 16 = 3,000,000 hz
    //Postcaler of 12 at 48MHz = 3,000,000 / 12 = 250,000
    //Set PR2 = 249. This causes timer to reset when it reaches 249 = 250,000 / 250 = 1000hz = 1ms
    T2CON = 0x5e;   //xxxx xx10 - Prescaler = 16
                    //xxxx x1xx - Tmr2 on
                    //x101 1xxx - Postscaler = 11 (0=1, 1=2 ... 11=12)
    PR2 = 249;
    */

    //Prescaler of 4 at 12MHz = 12,000,000 / 16 = 3,000,000 hz
    //Postcaler of 12 at 12MHz = 3,000,000 / 12 = 250,000
    //Set PR2 = 249. This causes timer to reset when it reaches 249 = 250,000 / 250 = 1000hz = 1ms
    T2CON = 0x5d;           //xxxx xx01 - Prescaler = 4
                            //xxxx x1xx - Tmr2 on
                            //x101 1xxx - Postscaler = 11 (0=1, 1=2 ... 11=12)
    //PR2 = 249;
    PR2 = 249;
    PIE1bits.TMR2IE = 1;    //Enable Timer2 interrupt

    /////////////////////////////////////////////////
    //USB stack defines
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See io_cfg.h
    #endif
    
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;
    #endif
    
    mInitializeUSBDriver();         // See usbdrv.h
    UserInit();                     // See user.c & .h
    /////////////////////////////////////////////////
    //Global interrupt enable
    INTCONbits.PEIE = 1;            //Enable Peripheral interrups (TMR2, ....)
    INTCONbits.GIE = 1;             //Global interrupt enable
}

/**
 * Service loop for USB tasks.
 *
 * @PreCondition    InitializeSystem has been called.
 */
void USBTasks(void)
{
    /*
     * Servicing Hardware
     */
    USBCheckBusStatus();                    // Must use polling method
    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 // Interrupt or polling method
    
    #if defined(USB_USE_CDC)
    CDCTxService();
    #endif

}


/******************************************************************************
 * Function:        void BlinkUSBStatus(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        BlinkUSBStatus blinks the LED from 1 to 8 times corresponding
 *                  to the USB device state.
 *
 * Note:            mLED macros can be found in io_cfg.h
 *                  usb_device_state is declared in usbmmap.c and is modified
 *                  in usbdrv.c, usbctrltrf.c, and usb9.c
 *****************************************************************************/
void BlinkUSBStatus(void)
{
    static word led_blink_cnt=0;    //Current blink st
    static byte led_blinks=0;       //Indicate how many blinks that LED should have

    static uint8 tmrLed;

    /////////////////////////////////////////////////
    //Enter every n ms
    #define BLINKTIME 150
    if (tick8GetDiff(tmrLed) > BLINKTIME) {
        tmrLed += BLINKTIME;
        if (led_blink_cnt == 0) {
            led_blink_cnt = 32;
        }
        led_blink_cnt--;
    }

    //Check if LED must be on
    if ((led_blink_cnt&0x03)==0x03) {
        if ((led_blinks*4) > led_blink_cnt)
            mLED_1 = 1;
        else
            mLED_1 = 0;
    }
    else {
        mLED_1 = 0;
    }

    //Each USB state will blink the LED different number of times
    if(UCONbits.SUSPND == 1)
    {
        led_blinks=1;
    }
    else
    {
        if(usb_device_state == DETACHED_STATE)
        {
            led_blinks=2;
        }
        else if(usb_device_state == ATTACHED_STATE)
        {
            led_blinks=3;
        }
        else if(usb_device_state == POWERED_STATE)
        {
            led_blinks=4;
        }
        else if(usb_device_state == DEFAULT_STATE)
        {
            led_blinks=5;
        }
        else if(usb_device_state == ADDRESS_STATE)
        {
            led_blinks=6;
        }
        else if(usb_device_state == CONFIGURED_STATE)
        {
            led_blinks=7;
        }
    }
}
