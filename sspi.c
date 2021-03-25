/*
 * sspi.c
 *
 *  Created on: Mar 23, 2021
 *      Author: Eugenijus
 */

#include "sspi.h"

SSPI_TypeDef sspi;

__STATIC_INLINE void DelayMicro(uint32_t __IO micros)
{
  micros *=(SystemCoreClock/1000000)/5;
  while(micros--);
}

void sspi_init(SSPI_TypeDef _sspi){
	sspi=_sspi;
}

static void _sspi_mosi_l(){
	HAL_GPIO_WritePin(sspi.mosi_port, sspi.mosi_pin, GPIO_PIN_RESET);
}
static void _sspi_mosi_h(){
	HAL_GPIO_WritePin(sspi.mosi_port, sspi.mosi_pin, GPIO_PIN_SET);
}

static void _sspi_clk_l(){
	HAL_GPIO_WritePin(sspi.clk_port, sspi.clk_pin, GPIO_PIN_RESET);
}
static void _sspi_clk_h(){
	HAL_GPIO_WritePin(sspi.clk_port, sspi.clk_pin, GPIO_PIN_SET);
}

static void _sspi_cs_l(){
	HAL_GPIO_WritePin(sspi.cs_port, sspi.cs_pin, GPIO_PIN_RESET);
}
static void _sspi_cs_h(){
	HAL_GPIO_WritePin(sspi.cs_port, sspi.cs_pin, GPIO_PIN_SET);
}

static void _sspi_send_byte_msb(uint8_t b){
	uint8_t i;
	uint8_t mask = 0x80;
	for(i = 0; i < 8; i++){
		if ( b & ( mask >> i ) ){
			_sspi_mosi_h();
		}
		else{
			_sspi_mosi_l();
		}
		_sspi_clk_h();
		DelayMicro(sspi.delay);
		_sspi_clk_l();
		DelayMicro(sspi.delay);
	}
}

void sspi_send_byte(uint8_t b){
	_sspi_cs_l();
	DelayMicro(sspi.delay);
	_sspi_send_byte_msb(b);
	_sspi_cs_h();
	DelayMicro(sspi.delay);
}

void sspi_send_half_word(uint16_t hw){
	uint8_t msb_hw = (uint8_t)((hw & 0xFF00) >> 8);
	uint8_t lsb_hw = (uint8_t)(hw & 0x00FF);
	_sspi_cs_l();
	DelayMicro(sspi.delay);
	_sspi_send_byte_msb(msb_hw);
	_sspi_send_byte_msb(lsb_hw);
	_sspi_cs_h();
	DelayMicro(sspi.delay);
}

void sspi_send_buff(uint8_t* buff, uint32_t size){
	int i;
	_sspi_cs_l();
	for(i = 0; i < size; i++){
		_sspi_send_byte_msb(buff[i]);
	}
	_sspi_cs_h();
	DelayMicro(sspi.delay);
}
