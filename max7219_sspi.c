/*
 * max7219_sspi.c
 *
 *  Created on: Mar 25, 2021
 *      Author: Eugenijus
 */

#include "max7219_sspi.h"

/**
 * @note	If segment display is common anode, assuming it
 * 			is connected as:
  *
  * Segment display with common anode wiring
  *=============================================================================
  * MAX7219	| Display
  *=============================================================================
  * dig 1	| seg A
  *-----------------------------------------------------------------------------
  * dig 2	| seg B
  *-----------------------------------------------------------------------------
  * dig 3	| seg C
  *-----------------------------------------------------------------------------
  * dig 4	| seg D
  *-----------------------------------------------------------------------------
  * dig 5	| seg E
  *-----------------------------------------------------------------------------
  * dig 6	| seg F
  *-----------------------------------------------------------------------------
  * dig 7	| seg G
  *-----------------------------------------------------------------------------
  * dig 8	| seg DP
  *-----------------------------------------------------------------------------
  * seg A	| dig 1
  *-----------------------------------------------------------------------------
  * seg B	| dig 2
  *-----------------------------------------------------------------------------
  * seg C	| dig 3
  *-----------------------------------------------------------------------------
  * seg D	| dig 4
  *-----------------------------------------------------------------------------
  * seg E	| dig 5
  *-----------------------------------------------------------------------------
  * seg F	| dig 6
  *-----------------------------------------------------------------------------
  * seg G	| dig 7
  *-----------------------------------------------------------------------------
  * seg DP	| dig 8
  *=============================================================================
 *
 */
#define COMMON_ANODE

#ifdef COMMON_ANODE
uint16_t dig[8]={0x0040,0x0020,0x0010,0x0008,0x0004,0x0002,0x0001,0x0080};
uint16_t seg[8]={0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700,0x0800};
#endif

uint8_t number_of_digits;

/**
  * 0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F,-, ,o
  */
#define MAX_SYM 19
uint8_t sym[MAX_SYM]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0xEE,0x3E,0x9C,0x7A,0x9E,0x8E,0x02,0x00,0x3A};

void max7219_wake_up(void){
	sspi_send_half_word( 0x0C01 );
}
void max7219_shutdown(void){
	sspi_send_half_word( 0x0C00 );
}
/**
  * @brief	Set display intensity
  * @param	i: intensity value in range 0x0 - 0xF
  * @retval	None
  */
void max7219_set_intensity(uint8_t i){
	sspi_send_half_word( 0x0A00 | (i & 0x0F) );
}
/**
  * @brief	Set number of digits
  * @param	n: number of digits in range 0 - 7
  * 		0 - one digit, 7 - eight digits
  * @note	If using with common anode segment display
  * 		n corresponds to number of segments.
  * 		Set value to 7 for use of all 7 segments and DP
  * @retval	None
  */
void max7219_set_number_of_digits(uint8_t n){
	number_of_digits = (n & 0x07);
}
void max7219_test_mode_on(void){
	sspi_send_half_word( 0x0F01 );
}
void max7219_test_mode_off(void){
	sspi_send_half_word( 0x0F00 );
}
void max7219_set_no_decode(){
	sspi_send_half_word( 0x0900 );
}

/**
 * @brief	Whole display update with values from buffer
 * @param	*buff:	pointer to buffer with values
 * @note	buffer size corresponse to number of digits.
 * @retval	None
 */
void max7219_update(uint8_t* buff){
#ifdef COMMON_ANODE
	uint16_t i,j;
	uint16_t data;
	for(i=0; i<8; i++){
		data = seg[i];
		for(j=0; j<number_of_digits; j++){
			if(buff[j] & (0x80 >> i)){
				data |= dig[j];
			}
		}
		sspi_send_half_word( data );
	}
#endif
}
void max7219_clear(void){
	uint8_t data[number_of_digits];
	uint8_t i;
	for(i=0; i<number_of_digits; i++){
		data[i] = sym[17];
	}
	max7219_update(data);
}
void max7219_01234567(void){
	uint8_t data[number_of_digits];
	uint8_t i;
	for(i=0; i<number_of_digits; i++){
		data[i] = sym[i];
	}
	max7219_update(data);
}

void max7219_start_default(void){

	max7219_set_no_decode();
	max7219_set_intensity(1);
	max7219_set_number_of_digits(4);
	max7219_wake_up();
	max7219_test_mode_on();
	max7219_test_mode_off();
	max7219_clear();

}
/**
 * @brief	Display a uint value
 * @param	u:	value
 * @note	If there are not enought digits, then display "o"
 * @param	z: if z==0 then show leading zeroes
 * @retval	None
 */
void max7219_show_uint(uint32_t u, uint8_t z){
	uint8_t data[number_of_digits];
	uint8_t i;
	uint32_t limit=1;
	i = number_of_digits;
	while(i){
		limit *= 10;
		i--;
	}
	limit--;
	if(u > limit){
		for(i=0; i<(number_of_digits-1); i++){
			data[i] = sym[17];
		}
		data[number_of_digits-1] = sym[18];
	}else{
		for(i=0; i<number_of_digits; i++){
			if(u>0){
			data[number_of_digits-1-i] = sym[u%10];
			u = u/10;
			}else{
				if(z==0){
					data[number_of_digits-1-i] = sym[u%10];
				}else{
					data[number_of_digits-1-i] = sym[17];
				}
			}
		}
	}
	max7219_update(data);
}
