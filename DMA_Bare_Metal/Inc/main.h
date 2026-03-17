/*
 * main.h
 *
 *  Created on: 16-Mar-2026
 *      Author: mudit
 */

#ifndef MAIN_H_
#define MAIN_H_

void button_init(void);
void uart2_init(void);
void dma1_init(void);
void send_some_data(void);
void enable_dma1_stream6(void);
void dma1_interrupt_configuration(void);

void HT_Complete_callback(void);
void FT_Complete_callback(void);
void TE_Complete_callback(void);
void FE_Complete_callback(void);
void DME_Complete_callback(void);



#endif /* MAIN_H_ */
