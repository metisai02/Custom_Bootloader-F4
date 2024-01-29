/*
 * boot.c
 *
 *  Created on: Jan 26, 2024
 *      Author: ADMIN
 */
#include "boot.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
extern UART_HandleTypeDef huart6;
extern CRC_HandleTypeDef hcrc;
#define UART_BOOT huart6
#define UART_USER huart6

void boot_main()
{
    boot_cmd_t command;
    while (1)
    {
        HAL_UART_Receive(&UART_BOOT, (uint8_t *)&command, sizeof(boot_cmd_t), HAL_MAX_DELAY);
        if (command.header == BOOT_HEADER)
        {
            switch (command.command)
            {
            case BOOT_WRITE_CMD:
                boot_send_ack();
                if (boot_write_handler() != HAL_OK)
                {
                    boot_send_nack();
                }
                break;
            case BOOT_READ_CMD:
                boot_send_ack();
                if (boot_read_handler() != HAL_OK)
                {
                    boot_send_nack();
                }
                break;
            case BOOT_ERASE_CMD:
                boot_send_ack();
                if (boot_erase_handler() != HAL_OK)
                {
                    boot_send_nack();
                }
                break;
            default:
                break;
            }
        }
    }
}
void boot_send_ack()
{
    HAL_UART_Transmit(&UART_BOOT, (uint8_t *)BOOT_ACK, sizeof(BOOT_ACK), 1000);
}
void boot_send_nack()
{
    HAL_UART_Transmit(&UART_BOOT, (uint8_t *)BOOT_NACK, sizeof(BOOT_ACK), 1000);
}
HAL_StatusTypeDef boot_read_handler()
{
    boot_read_t boot_read;
    if (HAL_UART_Receive(&UART_BOOT, (uint8_t *)&boot_read, sizeof(boot_read_t), 20) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }
    // check crc
    uint32_t new_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&boot_read, (sizeof(boot_read) / 4) - 1);
    if (new_crc != boot_read.crc)
    {
        return HAL_ERROR;
    }
    // read byte from flash
    char temp_hex[4] = {0};
    uint8_t *temp = malloc(boot_read.length);
    flash_read_byte((uint8_t *)boot_read.address, temp, boot_read.length);
    for (int i = 0; i < boot_read.length; i++)
    {
        sprintf(temp_hex, "%x\n", *(temp++));
        HAL_UART_Transmit(&UART_BOOT, (uint8_t *)temp_hex, sizeof(temp_hex), 1000);
    }
    free(temp);
    return HAL_OK;
}
HAL_StatusTypeDef boot_write_handler()
{
    boot_write_t boot_write;
    if (HAL_UART_Receive(&UART_BOOT, (uint8_t *)&boot_write, sizeof(boot_write_t), 1000) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }
    // check crc
    uint32_t new_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&boot_write, (sizeof(boot_write) / 4) - 1);
    if (new_crc != boot_write.crc)
    {
        return HAL_ERROR;
    }
    // write bytes to flash
    flash_write_byte((uint8_t *)boot_write.address, boot_write.data, boot_write.length);
    return HAL_OK;
}
HAL_StatusTypeDef boot_erase_handler()
{
    boot_erase_t boot_erase;
    if (HAL_UART_Receive(&UART_BOOT, (uint8_t *)&boot_erase, sizeof(boot_erase), 1000) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }
    // check crc
    uint32_t new_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&boot_erase, (sizeof(boot_erase) / 4) - 1);
    if (new_crc != boot_erase.crc)
    {
        return HAL_ERROR;
    }
    // implement erase
    flash_erase(boot_erase.address, 1);
    return HAL_OK;
}
