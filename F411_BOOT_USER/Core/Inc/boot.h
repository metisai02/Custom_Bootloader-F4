/*
 * boot.h
 *
 *  Created on: Jan 26, 2024
 *      Author: ADMIN
 */

#ifndef INC_BOOT_H_
#define INC_BOOT_H_
#include "main.h"
#include "flash.h"
#define BOOT_HEADER 0xFFFF
#define BOOT_READ_CMD 0x6969
#define BOOT_WRITE_CMD 0x9696
#define BOOT_ERASE_CMD 0x7777
const uint32_t BOOT_ACK = 0xFFFFEEEE;
const uint32_t BOOT_NACK = 0xFFFFBBBB;
typedef struct
{
    uint16_t header;
    uint16_t length;
    uint32_t address;
    uint32_t crc;
} boot_read_t;
typedef struct
{
    uint16_t header;
    uint16_t length;
    uint32_t address;
    uint8_t data[20];
    uint32_t crc;
} boot_write_t;
typedef struct
{
    uint16_t header;
    uint32_t address;
    uint32_t crc;
} boot_erase_t;

typedef struct
{
    uint16_t header;
    uint16_t command;
} boot_cmd_t;
void boot_send_ack();
void boot_send_nack();
HAL_StatusTypeDef boot_write_handler();
HAL_StatusTypeDef boot_read_handler();
HAL_StatusTypeDef boot_erase_handler();
#endif /* INC_BOOT_H_ */
