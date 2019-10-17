#include "xc.h"
#include <string.h>

#include "Include/TCPIP Stack/SPIFlash.h"
#include "mcc_generated_files/spi1.h"
#include "../mcc_generated_files/pin_manager.h"
#include "Configs/HWP TARPUQ.h"

static DWORD dwWriteAddr;
uint8_t flashId;

static void _SendCmd(BYTE cmd);
static void _WaitWhileBusy(void);

uint8_t AT45_Read_ReadCommand(uint8_t cmd)
{
    uint8_t env[2], rec[2];
    SPIFLASH_CS_SetLow();
    env[0] = cmd; //AT45_JEDEC_ID;
    SPI1_Exchange8bitBuffer(env, 2, rec);
    SPIFLASH_CS_SetHigh();
    return rec[1];
}

//Lee la Identificación del fabricante

uint8_t AT45_Read_ManufacturerID(void)
{
    return AT45_Read_ReadCommand(AT45_JEDEC_ID);
}

//Lee la Identificación JEDEC

void AT45_Read_JedecID(uint8_t *buffer)
{
    uint8_t env[4];
    SPIFLASH_CS_SetLow();
    env[0] = AT45_JEDEC_ID;
    SPI1_Exchange8bitBuffer(env, 4, buffer);
    SPIFLASH_CS_SetHigh();
}

void AT45_ComunesDireccion(uint8_t cmd, DWORD address)
{
    SPI1_Exchange8bit(cmd); //comando
    SPI1_Exchange8bit((address >> 16)&0xFF); //Direccion A23-A16
    SPI1_Exchange8bit((address >> 8)&0xFF); //Direccion A15-A8
    SPI1_Exchange8bit(address & 0xFF); //Direccion A7-A0
}

//Lee la memoria desde la direccion de inicio hasta

void SPIFlashReadArray(DWORD address, uint8_t *buffer, DWORD len)
{
    DWORD a;
    SPIFLASH_CS_SetLow();
    AT45_ComunesDireccion(AT45_READ_DATA, address);
    for (a = 0; a < len; a++)
        *(buffer + a) = SPI1_Exchange8bit(0);
    SPIFLASH_CS_SetHigh();
}

void SPIFlashBeginWrite(DWORD dwAddr)
{
    dwWriteAddr = dwAddr;
}

//Lee la posición de memoria especificada y retorna el

uint8_t SPIFlashReadByte(DWORD address)
{
    uint8_t data;
    SPIFLASH_CS_SetLow();
    AT45_ComunesDireccion(AT45_READ_DATA, address);
    data = SPI1_Exchange8bit(0);
    SPIFLASH_CS_SetHigh();
    return data;
}

//Escribe en la memoria un byte en la direccion preestablecida

void SPIFlashWrite(BYTE vData)
{
    if ((dwWriteAddr & SPI_FLASH_SECTOR_MASK) == 0u)
        SPIFlashEraseSector(dwWriteAddr);
    SPIFLASH_CS_SetLow();
    AT45_ComunesDireccion(AT45_BUFFER_WRITE, dwWriteAddr);
    SPI1_Exchange8bit(vData); //comando
    dwWriteAddr++;
    SPIFLASH_CS_SetHigh(); //INICIA EL CICLO DE ESCRITURA
    _WaitWhileBusy();
}

//Lee la memoria desde la direccion de inicio hasta

void SPIFlashWriteArray(BYTE *buffer, WORD wLen)
{
    if (wLen == 0u)
        return;
    while (wLen >= 1)
    {
        SPIFlashWrite(*buffer++); //dwWriteAddr++;
        wLen--;
    }
}

//Borrar pagina de 512 bytes

void SPIFlashEraseSector(DWORD dwAddr)
{
    SPIFLASH_CS_SetLow();
    AT45_ComunesDireccion(AT45_PAGE_ERASE, dwAddr);
    SPIFLASH_CS_SetHigh(); //INICIA EL CICLO DE ESCRITURA
    _WaitWhileBusy();
}

static void _SendCmd(BYTE cmd)
{
    SPIFLASH_CS_SetLow();
    SPI1_Exchange8bit(cmd); //comando
    SPIFLASH_CS_SetHigh(); //INICIA EL CICLO DE ESCRITURA
}

BYTE GetStatus(void)
{
    return AT45_Read_ReadCommand(AT45_STATUS_REGISTER);
}

BYTE GetReady(void)
{
    return (GetStatus() >> 7) & 1;
}

static void _WaitWhileBusy(void)
{
    while (!GetReady());
}

//Configuar Hardware

void SPIFlashInit(/*uint8_t SpiSST, volatile DWORD port, uint8_t cs*/)
{
    SPIFLASH_CS_SetHigh();
}
