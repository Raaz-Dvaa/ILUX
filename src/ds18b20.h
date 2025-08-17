/*
 * ds18b20.h
 *
 *  Created on: 16 авг. 2025 г.
 *      Author: Арсений
 */

#ifndef DS18B20_H_
#define DS18B20_H_

void delay_mcs(volatile int count);
uint8_t ds18b20_init();
void write_bit(uint8_t bit);
void write_byte(uint8_t byte);
uint16_t read_bit ();
uint16_t read_data ();
void start_measure();

#endif /* DS18B20_H_ */
