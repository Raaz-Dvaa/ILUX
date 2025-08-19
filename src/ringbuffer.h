/*
 * ringbuffer.h
 *
 *  Created on: 19 авг. 2025 г.
 *      Author: Арсений
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

typedef struct {
	uint8_t data[64];
	uint16_t head;
	uint16_t tail;

} RINGBUFFER;

void RINGBUFFER_init(RINGBUFFER* buf);
int RINGBUFFER_put(uint8_t val, RINGBUFFER* buf);
int RINGBUFFER_get(uint8_t* val, RINGBUFFER* buf);

#endif /* RINGBUFFER_H_ */
