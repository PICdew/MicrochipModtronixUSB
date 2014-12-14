/* 
 * File:   userlib.h
 * Author: user
 *
 * Created on December 13, 2014, 5:25 PM
 */

#ifndef USERLIB_H
#define	USERLIB_H

void InitializeUSART(void);
void delay_ms(uint16 t);
void delay_us(uint16 t);
unsigned char eeprom_read_byte(unsigned char addr);
void eeprom_write_byte(char address,char data);
void eeprom_write_block(void *ptr, unsigned short addr, unsigned char len);
void eeprom_write_string(char *s, unsigned char addr);
void eeprom_read_string(char* s, unsigned int addr);

#endif	/* USERLIB_H */

