/*
 * sspi.h
 *
 *  Created on: Mar 23, 2021
 *      Author: Eugenijus
 *
 *
 ******************************************************************************
 *
 * Software SPI interface
 *
 ******************************************************************************
 *
 */

#ifndef INC_SSPI_H_
#define INC_SSPI_H_

#include "stm32f3xx_hal.h"

typedef struct {
	GPIO_TypeDef * mosi_port;
	GPIO_TypeDef * cs_port;
	GPIO_TypeDef * clk_port;

	uint16_t mosi_pin;
	uint16_t cs_pin;
	uint16_t clk_pin;

	uint32_t delay;

} SSPI_TypeDef;

void sspi_init(SSPI_TypeDef _sspi);
void sspi_send_byte(uint8_t b);
void sspi_send_half_word(uint16_t hw);
void sspi_send_buff(uint8_t* buff, uint32_t size);

#endif /* INC_SSPI_H_ */
