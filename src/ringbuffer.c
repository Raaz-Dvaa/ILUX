#include "stm32f4xx.h"
#include "ringbuffer.h"


void RINGBUFFER_init(RINGBUFFER* buf){
	buf->head = 0;
	buf->tail = 0;
}

int RINGBUFFER_put(uint8_t val, RINGBUFFER* buf){
	uint16_t next = (buf->head + 1) % 64;
	if (next == buf->tail){
		return 0;
	}
	buf->data[buf->head] = val;
	buf->head = next;
	return 1;
}

int RINGBUFFER_get(uint8_t* val, RINGBUFFER* buf){
	if (buf->head == buf->tail){
		return 0;
	}
	*val = buf->data[buf->tail];
	buf->tail = (buf->tail + 1) % 64;
	return 1;
}

int RINGBUFFER_IsEmpty(RINGBUFFER* buf){
	return (buf->head == buf->tail);
}

int RINGBUFFER_IsFull(RINGBUFFER* buf){
	return((buf->head + 1) % 64 == buf->tail);
}
