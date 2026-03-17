#include <stdint.h>
#include "stm32f446xx.h"
#include <string.h>
#include "main.h"

char data_Stream[]="Hello World\r\n";

int main(void)
{
	button_init();
	uart2_init();
	dma1_init();
	enable_dma1_stream6();
	dma1_interrupt_configuration();

	while(1);

}

void button_init(void)
{
	//User button at PC13

	//1. Enable GPIOC peripheral clock
	RCC->AHB1ENR |= (1 << 2);

	//2.GPIO pin in input mode
	GPIOC->MODER &= ~(3 << 26);

	//3.Enable the interrupt over that GPIO pin
	EXTI->IMR |= (1 << 13);

	//4.Enabling SYSCFG Clock
	RCC->APB2ENR |= (1 << 14);

	//5.Select PC13 for EXTI13
	SYSCFG->EXTICR[3] &= ~(0xF << 4);
	SYSCFG->EXTICR[3] |=  (0x2 << 4);

	//6. Configure the edge detection on the GPIO pin
	EXTI->FTSR |= (1 << 13);

	//7.Enable the IRQ for the GPIO in NVIC
	NVIC->ISER[1] |= (1 << 8);
}

void uart2_init(void)
{
	//1.Enable UART2 peripheral clock
	RCC->APB1ENR |= ( 1<< 17);

	//2.Configure the GPIO pins for UART_TX and UART_RX
	//PA2 as TX and PA3 as RX

	//i. Enable GPIOA Peripheral clock
	RCC->AHB1ENR |= (1 << 0);

	//ii. Change the mode to AF
	GPIOA->MODER &= ~(3 << 4);	//PA2
	GPIOA->MODER |= (2 << 4);

	GPIOA->MODER &= ~(3 << 6); //PA2
	GPIOA->MODER |= (2 << 6);

	GPIOA->AFR[0] &= ~(0xF << 8);
	GPIOA->AFR[0] |= (7 << 8);	//PA2->AF7

	GPIOA->AFR[0] &= ~(0xF << 12);
	GPIOA->AFR[0] |= (7 << 12);	//PA3->AF7

	//iii.Enable or disable Pull-up resistors if required
	GPIOA->PUPDR &= ~(3 << 4);//PA2
	GPIOA->PUPDR |= (1 << 4);

	GPIOA->PUPDR &= ~(3 << 6);//PA3
	GPIOA->PUPDR |= (1 << 6);

	//3.Configure the baud rate
	USART2->BRR = 0x8B;

	//4.Configure the data width,no of stop bits, etc.
	//Default values are used

	//5.Enable TX engine of the UART peripheral
	USART2->CR1 |= (1 << 3);

	//6.Enable the UART Peripheral
	USART2->CR1 |= (1 << 13);
}

void dma1_init(void)
{
	//1.Enable the peripheral clock for DMA1
	RCC->AHB1ENR |= (1 << 21);

	//2.Identify the stream that is suitable for the peripheral
	//Stream 6

	//3.Identify the channel number on which UART2 peripheral sends DMA requests
	//channel 4
	DMA1_Stream6->CR &= ~(7 << 25);
	DMA1_Stream6->CR |= (4 << 25);

	//4.Program the source address
	//Memory
	DMA1_Stream6->M0AR = (uint32_t)(data_Stream);

	//5.Program the destination address
	//Peripheral
	DMA1_Stream6->PAR = (uint32_t)&USART2->DR;

	//6.Program the number of data items to send
	uint8_t len = sizeof(data_Stream);
	DMA1_Stream6->NDTR =(uint16_t)len;

	//7.The direction of data transfer
	//Program M2P
	DMA1_Stream6->CR |= (1 << 6);

	//8.Program the source and destination data width
	DMA1_Stream6->CR &= ~(3 << 11);
	DMA1_Stream6->CR &= ~(3 << 13);

	//8a.Enable Memory auto increment
	DMA1_Stream6->CR |= (1 <<10);

	//9.Direct mode or FIFO
	DMA1_Stream6->FCR |= (1 << 2);//direct mode disabled

	//10.Select FIFO Threshold
	DMA1_Stream6->FCR &= ~(3 << 0);
	DMA1_Stream6->FCR |= (3 << 0);

	//11.Enable the circular mode
	//By default disabled

	//12.Single transfer or burst transfer
	//By default Single Transfer

	//13.Configure the stream priority
	//Use by default


}

void enable_dma1_stream6(void)
{
	//Enable the stream
	DMA1_Stream6->CR |= (1 << 0);
}

void dma1_interrupt_configuration(void)
{
	//Enable HTIE
	DMA1_Stream6->CR |= (1 << 3);

	//Enable TCIE
	DMA1_Stream6->CR |= (1 << 4);

	//Enable TEIE
	DMA1_Stream6->CR |= (1 << 2);

	//Enable FEIE
	DMA1_Stream6->FCR |= (1 << 7);

	//Enable DMEIE
	DMA1_Stream6->CR |= (1 << 1);

	//Enable the IRQ for DMA1 stream6 global interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void HT_Complete_callback(void)
{

}

void FT_Complete_callback(void)
{
	USART2->CR3 &= ~(1 << 7);

	uint8_t len = sizeof(data_Stream);
	DMA1_Stream6->NDTR =(uint16_t)len;

	enable_dma1_stream6();
}

void TE_Complete_callback(void)
{
	while(1);
}

void FE_Complete_callback(void)
{

}

void DME_Complete_callback(void)
{

}
