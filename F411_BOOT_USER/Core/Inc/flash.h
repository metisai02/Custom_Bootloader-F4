/*
 * flash.h
 *
 *  Created on: Jan 27, 2024
 *      Author: ADMIN
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "main.h"
#define FlASH_BASSE_ADR 0x40023C00
#define FLASH_KEY_1 0x45670123UL
#define FLASH_KEY_2 0xCDEF89ABUL
void flash_write_word(uint32_t *addr, uint32_t *buffer, uint32_t length);
void flash_write_byte(uint8_t *addr, uint8_t *buffer, uint32_t length);
void flash_read_word(uint32_t *addr, uint32_t *dest, uint32_t length);
void flash_read_byte(uint8_t *addr, uint8_t *dest, uint32_t length);
void flash_erase(uint32_t adress, uint8_t length_of_sector);

#endif /* INC_FLASH_H_ */
