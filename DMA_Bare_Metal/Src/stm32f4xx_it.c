#include <stdint.h>
#include "stm32f446xx.h"
#include "main.h"

#define is_it_HT()	DMA1->HISR & (1 << 20)
#define is_it_FT()	DMA1->HISR & (1 << 21)
#define is_it_TE()	DMA1->HISR & (1 << 19)
#define is_it_FE()	DMA1->HISR & (1 << 16)
#define is_it_DME()	DMA1->HISR & (1 << 18)

void EXTI15_10_IRQHandler(void)
{

	//USART2_TX sends DMA request to DMA 1 Controller
	USART2->CR3 |= (1 << 7);
	if(EXTI->PR & ( 1 << 13))
	{
		EXTI->PR |= ( 1 << 13 ); //Clearing the pending bit
	}
}

//IRQ handler for DMA1 Stream6 global interrupt
void DMA1_Stream6_IRQHandler(void)
{
	if(is_it_HT())
	{
		DMA1->HIFCR |= (1<<20);
		HT_Complete_callback();
	}else if(is_it_FT())
	{
		DMA1->HIFCR |= (1<<21);
		FT_Complete_callback();
	}else if(is_it_TE())
	{
		DMA1->HIFCR |= (1<<19);
		TE_Complete_callback();
	}else if(is_it_FE())
	{
		DMA1->HIFCR |= (1<<16);
		FE_Complete_callback();
	}else if(is_it_DME())
	{
		DMA1->HIFCR |= (1<<18);
		DME_Complete_callback();
	}else{

	}

}
