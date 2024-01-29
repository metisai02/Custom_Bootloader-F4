
#include "flash.h"
static uint8_t flash_unlock();
static uint8_t GetSector(uint32_t Address)
{
    uint8_t sector = 0;

    if ((Address < 0x08003FFF) && (Address >= 0x08000000))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((Address < 0x08007FFF) && (Address >= 0x08004000))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((Address < 0x0800BFFF) && (Address >= 0x08008000))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((Address < 0x0800FFFF) && (Address >= 0x0800C000))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((Address < 0x0801FFFF) && (Address >= 0x08010000))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((Address < 0x0803FFFF) && (Address >= 0x08020000))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((Address < 0x0805FFFF) && (Address >= 0x08040000))
    {
        sector = FLASH_SECTOR_6;
    }
    else if ((Address < 0x0807FFFF) && (Address >= 0x08060000))
    {
        sector = FLASH_SECTOR_7;
    }
    return sector;
}

void flash_erase(uint32_t adress, uint8_t length_of_sector)
{
    uint8_t sector = GetSector(adress);
    flash_unlock();
    uint32_t *p_temp = NULL;
    // check BYS
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x0C); // FLASH_SR
    while ((*p_temp) & (1 << 16))
        ;
    // enable erase
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    *p_temp |= (1 << 1);
    // set sector to erase
    for (int i = 0; i < length_of_sector; i++)
    {
        *p_temp &= ~(0xf << 3);
        *p_temp |= (sector++ << 3);
        *p_temp |= (1 << 16);
        // wating BYS clear
        p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x0C);
        while ((*p_temp) & (1 << 16))
            ;
    }
    // lock flash
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    *p_temp |= (1 << 31);
}
static uint8_t flash_unlock()
{
    uint32_t *p_temp = NULL;
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    if (!((*p_temp) >> 31))
        return 0;
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x04);
    *p_temp = FLASH_KEY_1;
    *p_temp = FLASH_KEY_2;
    // check lock
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    if ((*p_temp) >> 31)
    {
        return 1;
    }
    return 0;
}
void flash_write_word(uint32_t *addr, uint32_t *buffer, uint32_t length)
{
    flash_erase((uint32_t)addr, (uint32_t)(GetSector((uint32_t)addr) - GetSector((uint32_t)(addr + length))));
    flash_unlock();
    uint32_t *p_temp = NULL;
    // check BYS
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x0C); // FLASH_SR
    while ((*p_temp) & (1 << 16))
        ;
    // set option for wrtiting byte to byte
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    *p_temp &= ~(0x3 << 8);
    *p_temp |= (0b10 << 8);
    *p_temp |= (1 << 0);
    for (int i = 0; i < length; i--)
    {
        *addr++ = *buffer++;
    }
    // lock flash
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    *p_temp |= (1 << 31);
}
void flash_write_byte(uint8_t *addr, uint8_t *buffer, uint32_t length)
{
    flash_erase((uint32_t)addr, (uint32_t)(GetSector((uint32_t)addr) - GetSector((uint32_t)(addr + length))));
    flash_unlock();
    uint32_t *p_temp = NULL;
    // check BYS
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x0C); // FLASH_SR
    while ((*p_temp) & (1 << 16))
        ;
    // set option for wrtiting byte to byte
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    *p_temp &= ~(0x3 << 8);
    // enable programming
    *p_temp |= (1 << 0);
    for (int i = 0; i < length; i++)
    {
        *addr++ = *buffer++;
    }
    // lock flash
    p_temp = (uint32_t *)(FlASH_BASSE_ADR + 0x10);
    *p_temp |= (1 << 31);
}
void flash_read_word(uint32_t *addr, uint32_t *dest, uint32_t length)
{
    for (int i = 0; i < length; i++)
    {
        *dest++ = *addr++;
    }
}
void flash_read_byte(uint8_t *addr, uint8_t *dest, uint32_t length)
{
    for (int i = 0; i < length; i   ++)
    {
        *dest++ = *addr++;
    }
}
