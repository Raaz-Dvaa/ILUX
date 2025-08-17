
#include "stm32f4xx.h"
#include "ds18b20.h"

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

void USART2_init (){
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER2_1;
	GPIOA->MODER |= GPIO_MODER_MODER3_1;
	GPIOA->AFR[0] |= (7 << (2*4)) | (7 << (3*4));
	USART2->BRR = 13440000/115200; // Вот тут не понял
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;


	USART2->CR1 |= USART_CR1_UE;

}

uint8_t GetLenOfNumber(uint16_t number){
	if (number == 0) return 1;
	uint16_t temp = number;
	uint8_t result = 0;
	while (temp > 0){
		temp /= 10;
		result++;
	}
	return result;
}

void NumberToChar (uint8_t len, uint16_t num, char* data){
	for (int i = len-1; i >= 0; i--){
		data[i] = (num % 10) + '0';
		num /= 10;
	}
}

uint16_t SumDigit (uint8_t len, char* data){
	uint16_t sum = 0;
	for (int i = 0; i < len; i++){
		sum += (data[i] - '0');
	}
	return sum;
}

void SendPacket (uint16_t number, char adr){
	char data[5];
	char sum[2];
	uint8_t len = GetLenOfNumber(number);
	NumberToChar(len, number, data);
	uint16_t sumdigit = SumDigit(len, data);
	uint8_t lensum = GetLenOfNumber(sumdigit);
	NumberToChar(lensum, sumdigit, sum);

	char pack[2 + len + lensum];

	pack[0] = adr;
	pack[1] = (len + '0');

	for (int i = 0; i < len; i++){
		pack[2 + i] = data[i];
	}

	for (int t = 0; t < lensum; t++){
		pack[2 + len + t] = sum[t];
	}

	for (int y = 0; y < 2 + len + lensum; y++) {
	        while (!(USART2->SR & USART_SR_TXE));
	        USART2->DR = pack[y];
	    }

}

void send_float (float value, uint8_t accuracy){
	uint16_t whole = (uint16_t)value;
	uint16_t frac = (uint16_t)((value - whole) * (10 * accuracy) + 0.5f);
	char buf[16];
	char wh[7];
	char fr[7];

	if (value < 0){
		while (!(USART2->SR & USART_SR_TXE));
		USART2->DR = '-';
	}

	uint8_t lenwhole = GetLenOfNumber(whole);
	uint8_t lenfrac = GetLenOfNumber(frac);
	NumberToChar(lenwhole, whole, wh);
	NumberToChar(lenfrac, frac, fr);
	for (int i = 0; i < lenwhole; i++){
		buf[i] = wh[i];
	}

	buf[lenwhole] = '.';

	for (int t = 0; t < lenfrac; t++){
		buf[t + lenwhole + 1] = fr[t];
	}

	for (int y = 0; y < 1 + lenwhole + lenfrac; y++){
		 while (!(USART2->SR & USART_SR_TXE));
		 USART2->DR = buf[y];
	}
	while (!(USART2->SR & USART_SR_TXE));
			USART2->DR = ' ';

}

int main(void)
{
  //RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  //GPIOA->MODER |= GPIO_MODER_MODER6_0;
  //InitTIM4();
  //NVIC_EnableIRQ(TIM4_IRQn);

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	USART2_init();
	uint16_t rawtemp = 0;
	float temper = 0;
  while (1)
  {
	  //SendPacket(735, 'q');
	  start_measure();
	  rawtemp = read_data();
	  temper = rawtemp * 0.0625f;
	  send_float(temper, 4);
	  delay_mcs(100000);
  }
}
