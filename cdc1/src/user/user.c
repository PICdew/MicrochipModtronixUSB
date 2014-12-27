/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        user.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 ********************************************************************/

/** I N C L U D E S **********************************************************/
/* must have C:\Program Files (x86)\Microchip\mplabc18\v3.46\h\ in the include path*/
#include <p18cxxx.h>
#include <usart.h>
#include <stdio.h>
#include <string.h>

#include "system\typedefs.h"
#include "system\usb\usb.h"
#include "io_cfg.h"             // I/O pin mapping
#include "user\user.h"
//#include "user\temperature.h"
#include "projdefs.h"
#include "mxmc18\tick.h"    // Modtronix library
#include "user\userlib.h"

/** V A R I A B L E S ********************************************************/
#pragma udata



/** P R I V A T E  P R O T O T Y P E S ***************************************/
void pulseClock(void);

// A1=CON1 pin 9, A3 = pin 11
// gnd = pin 5, vcc = pin7
#define LED_DATA_PIN LATAbits.LATA1
#define LED_CLOCK_PIN LATAbits.LATA3

/** D E C L A R A T I O N S **************************************************/
#pragma code

void pulseClock()
{
    delay_us(5);
    LED_CLOCK_PIN = 1;
    delay_us(10);
    LED_CLOCK_PIN = 0;
    delay_us(5);
}

void UserInit(void)
{
    mInitAllLEDs();
    mInitRA1();
    mInitRA3();
    InitializeUSART();
}

void sendToLED(uint8 red, uint8 green, uint8 blue)
{
    uint8 i;
    for (i=0; i<8; i++)
    {
        LED_DATA_PIN = ((red>>(7-i))& 0x01);
        pulseClock();
    }
    for (i=0; i<8; i++)
    {
        LED_DATA_PIN = ((green>>(7-i))& 0x01);
        pulseClock();
    }
    for (i=0; i<8; i++)
    {
        LED_DATA_PIN = ((blue>>(7-i))& 0x01);
        pulseClock();
    }
}

/******************************************************************************
 * Function:        void ProcessIO(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *****************************************************************************/
void ProcessIO(void)
{
   #define NUM_OF_LEDS 50
   #define SIZE_OF_ARRAY 25

    const uint8 maxState = NUM_OF_LEDS;
    static signed int state = NUM_OF_LEDS;
    uint8 i;
    uint8 j;
    uint8 n;
    uint8 r;
    uint8 g;
    uint8 b = 0;
    uint8 red[SIZE_OF_ARRAY];
    uint8 green[SIZE_OF_ARRAY];
    uint8 blue[SIZE_OF_ARRAY];
    uint8 addr = 1;
    char msg[20];

    LED_DATA_PIN = 0;
    LED_CLOCK_PIN = 0;
    delay_ms(200);

    for (i=0;i<SIZE_OF_ARRAY;i++)
    {
        red[i] = eeprom_read_byte(addr++);
        green[i] = eeprom_read_byte(addr++);
        blue[i] = eeprom_read_byte(addr++);
    }

    for (j=0; j<NUM_OF_LEDS; j++)
    {
         i = (j+state) % SIZE_OF_ARRAY;
         r = red[i]; g = green[i]; b = blue[i];
         sendToLED(r,g,b);
    }
    delay_ms(200);

    state--;
    if (state<0)
        state = maxState;
}





