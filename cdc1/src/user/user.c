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

char input_buffer[64];
char output_buffer[64];

rom char welcome[]={"PIC18F4550 Full-Speed USB - CDC RS-232 Emulation Demo\r\n\r\n"};
rom char ansi_clrscr[]={"\x1b[2J"};         // ANSI Clear Screen Command

/** P R I V A T E  P R O T O T Y P E S ***************************************/
byte Exercise_Example(void);
void Example1(void);
void Example2(void);
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

   #define NUM_OF_LEDS 26

    static uint8 state = 0;
    uint8 i;
    uint8 j;
    uint8 n;
    uint8 r;
    uint8 g;
    uint8 b = 0;
    uint8 red[30];
    uint8 green[30];
    uint8 blue[30];

    red[0] = 0;   green[0] = 0; blue[0] = 0;
    red[1] = 100; green[1] = 0; blue[1] = 0;
    red[2] = 0;   green[2] = 100; blue[2] = 0;
    red[3] = 100; green[3] = 0; blue[3] = 0;
    red[4] = 0;   green[4] = 100; blue[4] = 0;
    red[5] = 100; green[5] = 0; blue[5] = 0;
    red[6] = 0;   green[6] = 100; blue[6] = 0;
    red[7] = 100; green[7] = 0; blue[7] = 0;
    red[8] = 0;   green[8] = 100; blue[8] = 0;
    red[9] = 100; green[9] = 0; blue[9] = 0;
    red[10] = 000; green[10] = 100; blue[10] = 000;
    red[11] = 100; green[11] = 000; blue[11] = 000;
    red[12] = 000; green[12] = 100; blue[12] = 000;
    red[13] = 100; green[13] = 000; blue[13] = 000;
    red[14] = 000; green[14] = 100; blue[14] = 000;
    red[15] = 100; green[15] = 000; blue[15] = 000;
    red[16] = 000; green[16] = 100; blue[16] = 000;
    red[17] = 100; green[17] = 000; blue[17] = 000;
    red[18] = 000; green[18] = 100; blue[18] = 000;
    red[19] = 100; green[19] = 000; blue[19] = 000;
    red[20] = 000; green[20] = 100; blue[20] = 000;
    red[21] = 100; green[21] = 000; blue[21] = 000;
    red[22] = 000; green[22] = 100; blue[22] = 000;
    red[23] = 100; green[23] = 000; blue[23] = 000;
    red[24] = 000; green[24] = 100; blue[24] = 000;
    red[25] = 100; green[25] = 000; blue[25] = 000;
    red[26] = 000; green[26] = 100; blue[26] = 000;
    red[27] = 100; green[27] = 000; blue[27] = 000;
    red[28] = 000; green[28] = 100; blue[28] = 000;
    red[29] = 100; green[29] = 000; blue[29] = 000;
    
    n = 0;
    LED_DATA_PIN = 0;
    LED_CLOCK_PIN = 0;

    delay_ms(200);
    /*
    for (i=0; i<30; i++)
    {
       red[i] = 0; green[i] = 0; blue[i] = 255;
    }
    */
    /*


    if (state == 0)
    {
        for (j=1; j<=NUM_OF_LEDS; j++)
        {
           r = red[j]; g = green[j]; b = blue[j];
           sendToLED(r,g,b);
         }
    }
    if (state == 1)
    {
        for (j=1; j<=NUM_OF_LEDS; j++)
        {
           r = red[j+1]; g = green[j+1]; b = blue[j+1];
           sendToLED(r,g,b);

        }
    }
    state++;
    if (state>1)
        state = 0;
*/

}

/**
 * Modtronix Example 1 for the SBC44UC board.
 * This example writes "Hello World!" to the virtual serial port every second.
 */
void Example1(void)
{
    static uint16 tmr1sec = 0;
    
    /////////////////////////////////////////////////
    //Enter every 60ms
    if (tick16GetDiff(tmr1sec) > 1000) {
        tmr1sec += 1000;

        if(mUSBUSARTIsTxTrfReady())
        {
            putrsUSBUSART("\rHello World\r\n");
        }
    }
}

/**
 * Modtronix Example 2 for the SBC44UC board.
 */
void Example2(void)
{
    static byte bytesRead;
    char msg[20];
    char msg2[20];
    char foo;

    static byte smEx2State = 0;
    // State Machine states
    typedef enum SM_CMD
    {
        EX2_WAIT_FOR_INPUT,     // Wait for input from user via virtual serial port
        EX2_WAIT_TX_READY       // Wait for driver ready to accept a new string to write to the USB
    } SM_CMD;

    switch (smEx2State) {
        case EX2_WAIT_FOR_INPUT:
            //Check if any data was received via the virtual serial port
            if(bytesRead = getsUSBUSART(input_buffer, 8)) {
                smEx2State = EX2_WAIT_TX_READY;
            }
        break;
        case EX2_WAIT_TX_READY:
            //Write "Received: ", followed by data received, to virtual serial port
            if(mUSBUSARTIsTxTrfReady())
            {
                
                output_buffer[0] = '\r';
                output_buffer[1] = 'R';
                output_buffer[2] = 'e';
                output_buffer[3] = 'c';
                output_buffer[4] = 'e';
                output_buffer[5] = 'i';
                output_buffer[6] = 'v';
                output_buffer[7] = 'e';
                output_buffer[8] = 'd';
                output_buffer[9] = ':';
                output_buffer[10] = ' ';
                output_buffer[11] = input_buffer[0];
                output_buffer[12] = '\r';
                output_buffer[13] = '\n';
                output_buffer[14] = '\0';
                //putsUSBUSART(output_buffer);
                
                /*
                 * sprintf gives a warning 2066  type qualifier mismatch in assignment
                 * which the manual describes as:
                 * Pointer assignment where the source and destination pointers point to
                 * objects of compatible type, but the source pointer points to an object which
                 * is const or volatile qualified and the destination pointer does not.
                 *
                 * Well, yeah.
                 */

                //output_buffer is in udata, this works
                //sprintf(output_buffer, "got a <%c> \r\n\0",input_buffer[0]);
                //putsUSBUSART(output_buffer);
                //this works if msg is a local variable
                //sprintf(msg, "got a <%c> \r\n\0",input_buffer[0]);
                //putsUSBUSART(msg);


               // eeprom_write_byte(2,42);
               // foo = eeprom_read_byte(2);
               // sprintf(msg, "read <%u> \r\n\0",foo);
               // putsUSBUSART(msg);

           //    sprintf(msg, "ABC123\r\n");
           //    eeprom_write_block(msg, 4, strlen(msg));

              // char* eeprom_read_string(char* s, unsigned char addr);
           //    eeprom_read_string(msg2, 4);
           //    putsUSBUSART(msg2);

               //putrsUSBUSART(msg);//for pgm memory (rom) only

                //Back to wait for input state
                smEx2State = EX2_WAIT_FOR_INPUT;
            }
        break;
    }
}

byte Exercise_Example(void)
{
    static byte start_up_state = 0;
    
    if(start_up_state == 0)
    {
        if(mUSBUSARTIsTxTrfReady())
        {
            putrsUSBUSART(ansi_clrscr);
            start_up_state++;
        }
    }
    else if(start_up_state == 1)
    {
        if(mUSBUSARTIsTxTrfReady())
        {
            putrsUSBUSART("\rMicrochip Technology Inc., 2004\r\n");
            start_up_state++;
        }
    }
    else if(start_up_state == 2)
    {
        if(mUSBUSARTIsTxTrfReady())
        {
            putrsUSBUSART(welcome);
            start_up_state++;
        }
    }

    if (start_up_state == 3) return 1;

    return 0;   //Not finished
    
}//end Exercise_Example


