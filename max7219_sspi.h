/*
 * max7219_sspi.h
 *
 *  Created on: Mar 25, 2021
 *      Author: Eugenijus
 */

#ifndef INC_MAX7219_SSPI_H_
#define INC_MAX7219_SSPI_H_

#include "sspi.h"

void max7219_wake_up(void);
void max7219_shutdown(void);

void max7219_set_intensity(uint16_t i);
void max7219_set_number_of_digits(uint16_t n);
void max7219_test_mode_on(void);
void max7219_test_mode_off(void);
void max7219_set_no_decode();
void max7219_update(uint8_t* buff);
void max7219_clear(void);
void max7219_01234567(void);
void max7219_start_default(void);
void max7219_show_uint(uint32_t u, uint8_t z);

#endif /* INC_MAX7219_SSPI_H_ */
