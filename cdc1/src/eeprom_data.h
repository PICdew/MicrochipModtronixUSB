/* 
 * File:   eeprom_data.h
 * Author: user
 *
 * Created on December 13, 2014, 6:25 PM
 */

#ifndef EEPROM_DATA_H
#define	EEPROM_DATA_H

//DO NOT USE LOCATION AT EEPROM ADDRESS 0x000
//This location is reserved and not available for user storage.
#pragma romdata eedata_scn=0xf00000
    rom unsigned char my_eeprom_values[256] = {0x00, 0x00, 0x00, 0xFF,
                                                     0xFF, 0xFF, 0xFF,
                                                     0x00, 0x00, 0xFF,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0xFF, 0xC0, 0x00,
                                                     0xFF, 0xFF, 0xFF,
                                                     0xFF, 0xC0, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0x00, 0xFF, 0x00,
                                                     0xFF, 0x00, 0x00,
                                                     0x00, 0xFF, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0xFF, 0x00, 0x00,
                                                     0xFF, 0xBC, 0x00,
                                                     0x00, 0xFF, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0x00, 0x00, 0xFF,
                                                     0xFF, 0xFF, 0xFF,
                                                     0x00, 0x00, 0xFF,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0x00, 0xFF, 0x00,
                                                     0xFF, 0x00, 0x00,
                                                     0x00, 0xFF, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0x00, 0x00, 0xFF,
                                                     0xFF, 0xFF, 0xFF,
                                                     0x00, 0x00, 0xFF,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,

                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00};
#pragma romdata

#endif	/* EEPROM_DATA_H */

