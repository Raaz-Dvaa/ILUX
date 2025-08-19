
#include "stm32f4xx.h"
#include "ds18b20.h"
#include "USART_transmit.h"
#include "ringbuffer.h"

RINGBUFFER RX_BUF;

void delay(){
	for (volatile int i = 0; i < 1000000; i++);
}

void InitTIM4(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC = 999;
	TIM4->ARR = 25000;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_IRQHandler(){
	if (TIM4->SR & TIM_SR_UIF){
		TIM4->SR &= ~TIM_SR_UIF;
	}
	GPIOA->ODR ^= GPIO_ODR_ODR_6;
}

void USART2_IRQHandler(void){
	if (USART2->SR & USART_SR_RXNE){
		uint8_t byte = USART2->DR;
		RINGBUFFER_put(byte, &RX_BUF);
	}
}

void USART2_Return(){
	uint8_t byte;
	while (RINGBUFFER_get(&byte, &RX_BUF)){
		USART2_SendByte(byte);
	}
}

int main(void)
{
	RINGBUFFER_init(&RX_BUF);
	USART2_init();

  while (1)
  {
	  USART2_Return();
  }
}
