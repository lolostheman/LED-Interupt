#include "stm32l476xx.h"
#include "stdint.h"

void GPIO_Clock_Enable(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
}
/*void wait_ms(unit32_t ms){
	unit32_t i, j;
	for(i = 0; i < ms; i++)		
		for(j = 0; j<255; j++);
}*/

void GPIO_Pin_Init(){
	//red
	GPIOB->MODER &= ~(3UL<<4); //clears bits
	GPIOB->MODER |= 1UL<<4; //changes mode to output
	//green
	GPIOE->MODER &= ~(3UL<<16);
	GPIOE->MODER |= 1UL<<16;
	//red
	GPIOB->OTYPER &= ~(1<<2); //sets push-pull
	//green
	GPIOE->OTYPER &= ~(1<<8);	
	//red
	GPIOB->PUPDR &= ~(3UL<<4); //sets output type to no pullup no pulldown
	//green
	GPIOE->PUPDR &= ~(3UL<<16);
	
	GPIOA->MODER &= ~3U << 6;
	GPIOA->PUPDR &= ~3U << 6;
	GPIOA->PUPDR |= 2U << 6; 
	NVIC_EnableIRQ(EXTI3_IRQn);
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
	EXTI->IMR1  |= EXTI_IMR1_IM3;
	EXTI->RTSR1 |= EXTI_RTSR1_RT3;
	
}

void EXTI3_IRQHandler(void) { 
	while ((EXTI->PR1 & EXTI_PR1_PIF3) != 0) {
		GPIOE->ODR |= 1UL<<8;
		
		EXTI->PR1 |= EXTI_PR1_PIF3;
	}//else{
		//GPIOE->ODR &= ~(1UL<<8);
		
	//	EXTI->PR1 |= EXTI_PR1_PIF3;
	//}
}
int main(void){

	
	int i = 0;
	while(1){
	GPIO_Clock_Enable();//calls function that enables clock
	GPIO_Pin_Init();//calls function that initializes the pin setup
	
	GPIOB->ODR |= 1UL<<2; //turns on red LED
	
		if(i<5000){
			GPIOB->ODR &= ~(3UL<<2); //turns off red LED
		}
		if(i>5000){
			GPIOB->ODR |= 1UL<<2; //turns on red LED
		}
		if(i==10000){
			i = 0;
		}
		EXTI3_IRQHandler();
		
	
	
	
	//wait_ms(50);
	//    GPIOE->ODR |= 1UL<<8; //turns on green LED
	//wait_ms(50); depending on if i use my laptop or desktop to run this program, the wait_ms function varies. this is why it is commented.
	  i++;
	}
	
  // Dead loop & program hangs here
	
}
