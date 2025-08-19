/*
 * USART_transmit.h
 *
 *  Created on: 19 авг. 2025 г.
 *      Author: Арсений
 */

#ifndef USART_TRANSMIT_H_
#define USART_TRANSMIT_H_

uint8_t GetLenOfNumber(uint16_t number);
void NumberToChar (uint8_t len, uint16_t num, char* data);
uint16_t SumDigit (uint8_t len, char* data);
void SendPacket (uint16_t number, char adr);
void send_float (float value, uint8_t accuracy);
void USART2_SendByte (uint8_t byte);
void USART2_init ();

#endif /* USART_TRANSMIT_H_ */
