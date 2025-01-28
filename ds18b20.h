/*
Code modified from https://github.com/AGMLProjects/WFlow/blob/main/wflowembedded/MSP430/WFlow/ds18b20.h
*/


#ifndef DS18B20_H_
#define DS18B20_H_

#include <stdio.h>
#include <stdint.h>
#include "msp430g2553.h"
#include <msp430.h>
#include <HAL_MSP430.h>


// Commands to handle the sensor
#define DS18B20_SKIP_ROM             0xCC
#define DS18B20_READ_SCRATCHPAD      0xBE
#define DS18B20_CONVERT_T            0x44
#define DS1820_READ_ROM              0x33


#define DS18B20_LO  { DS18B20_PORT_DIR |= DS18B20_PORT_PIN; DS18B20_PORT_REN &= ~DS18B20_PORT_PIN; DS18B20_PORT_OUT &= ~DS18B20_PORT_PIN; } 
#define DS18B20_HI  { DS18B20_PORT_DIR |= DS18B20_PORT_PIN; DS18B20_PORT_REN &= ~DS18B20_PORT_PIN; DS18B20_PORT_OUT |= DS18B20_PORT_PIN; }
#define DS18B20_RLS { DS18B20_PORT_DIR &= ~DS18B20_PORT_PIN; DS18B20_PORT_REN |= DS18B20_PORT_PIN; DS18B20_PORT_OUT  |= DS18B20_PORT_PIN; }
#define DS18B20_IN  ( DS18B20_PORT_IN & DS18B20_PORT_PIN )

// Function prototypes
// void ds18b20_init_port(void);
//**********************************************************************************************************************************************************
void ds18b20_init_pins(void);
uint8_t ds18b20_reset();
void ds18b20_write_bit(uint8_t);
uint8_t ds18b20_read_bit();
void ds18b20_write_byte(uint8_t);
uint8_t ds18b20_read_byte();
uint16_t ds18b20_read_temp_registers(void);
uint64_t ds18b20_read_rom (void);
uint16_t ds18b20_get_temp(void);

#endif /* DS18B20_H_ */

