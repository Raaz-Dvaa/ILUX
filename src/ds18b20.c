#include "stm32f4xx.h"

void delay_mcs(volatile int count) {
	for (int i = 0; i < count * 3 ; i++);
}

uint8_t ds18b20_init(){
	GPIOE->MODER |= GPIO_MODER_MODER0_0;
	GPIOE->ODR &= ~GPIO_ODR_ODR_0;
	delay_mcs(500);
	GPIOE->MODER &= ~GPIO_MODER_MODER0_0;// xor?
	delay_mcs(75);
	uint8_t status = (GPIOE->IDR & GPIO_IDR_IDR_0) ? 0 : 1;
	delay_mcs(500);
	return status;
}

void write_bit(uint8_t bit){
	if (bit){
		GPIOE->ODR &= ~GPIO_ODR_ODR_0;
		delay_mcs(15);
		GPIOE->ODR |= GPIO_ODR_ODR_0;
		delay_mcs(70);
	}
	else{
		GPIOE->ODR &= ~GPIO_ODR_ODR_0;
		delay_mcs(70);
		GPIOE->ODR |= GPIO_ODR_ODR_0;
		delay_mcs(1);
	}
}

void write_byte(uint8_t byte){
	GPIOE->MODER |= GPIO_MODER_MODER0_0;
	for (int i = 0; i < 8; i++){
		write_bit(byte & 1);
		byte >>= 1;
	}
}

uint16_t read_bit (){
	uint16_t result = 0;
	GPIOE->MODER |= GPIO_MODER_MODER0_0;
	GPIOE->ODR &= ~GPIO_ODR_ODR_0;
	delay_mcs(10);
	GPIOE->MODER &= ~GPIO_MODER_MODER0_0;
	delay_mcs(15);
	result = GPIOE->IDR & GPIO_IDR_IDR_0;
	return result;
}

uint16_t read_data (){
	uint16_t data = 0x0000;
	uint16_t bit = 0;
	for (int i = 0; i < 16; i++){
		bit = read_bit();
		data |= bit << i;
		delay_mcs(40);
	}
	return data;
}

void start_measure(){
	ds18b20_init();
	write_byte(0xCC);
	write_byte(0x44);
	delay_mcs(7500);
	ds18b20_init();
	write_byte(0xCC);
}







