/* 
 * File:   AT45DB161D.h
 * Author: Daniel Ortega
 *
 * Created on 10/10/2019, 11:14 AM
 */

#ifndef SPIFLASH_H
#define	SPIFLASH_H

#include <stdint.h>
#include <GenericTypeDefs.h>
//#define DESHABILITA	1
//#define HABILITA	0

/*******************************************************************************
*                               LIST OF COMMANDS
*******************************************************************************/
#define AT45_STATUS_REGISTER	0xD7
#define AT45_READ_DATA	        0x03
#define AT45_FAST_READ	        0x0B
#define AT45_BUFFER_WRITE	0x82
#define AT45_PAGE_ERASE	        0x81	//512
#define AT45_JEDEC_ID		0x9F

/*******************************************************************************
*                           FLASH INFORMATION
*******************************************************************************/
#define AT45_PAGE_SIZE_BYTE	512
#define SPI_FLASH_SECTOR_SIZE	512
#define SPI_FLASH_SECTOR_MASK	(SPI_FLASH_SECTOR_SIZE - 1)

/*******************************************************************************
*		STATUS READ RESULT and TIMING PARAMETERS
*******************************************************************************/
#define SST_25_RS_SUCCESS	0	// 0 after Read Status command -> successful
#define SST_25_RS_FAIL		1	// 1 after Read Status command -> fail
#define SST_25_RB_TIMEOUT	2	// RB pin is used to monitor if an operation has completed
					// a timeout error indicates problem with hardware
//*********************************************************************************
//                      Tipos de memoria
//*********************************************************************************
#define ATMEL	0x1F
#define WINBOND	0xEF
#define SST	0xBF


/*****************************************************************************/
/* Definicion de prototipos */
/******************************************************************************/

/*******************************************************************************
*                         	MANUFACTURER ID
*
* Description : Esta funcion lee la Identificación del fabricante
*
* Function    : SST_25_MANUFACTURER_ID(void)
*
* Arguments   : *buffer    - data buffer for data to be read
*
* Returns     : 2 Bytes con la ID del fabricante
*
* Note		  :
*******************************************************************************/
extern uint8_t AT45_Read_ManufacturerID(void);

/*******************************************************************************
*                         	JEDEC ID
*
* Description : Esta funcion lee la Identificación JEDEC
*
* Function    : SST_25_SST_25_Read_JedecID
*
* Arguments   : *buffer    - data buffer for data to be read
*
* Returns     : 3 Bytes con la ID del Jedec
*
* Note		  :
*******************************************************************************/
extern void AT45_Read_JedecID(uint8_t *buffer);

/*******************************************************************************
*                         	Lectura
*
* Description : Esta funcion lee la memoria desde la direccion de inicio hasta
*				el numero de bytes deseados
*
* Function    : SST_25_ReadPage
*
* Arguments   : Direccion  - Apunta la direccion de 24Bits a Leer
				*buffer    - data buffer for data to be read
				Len        - Indica el numero de bytes a ser leidos (se puede leer toda la memoria)

*
* Returns     : *buffer    - contiene la informacion de la memoria
*
* Note		  :
*******************************************************************************/
extern void SPIFlashReadArray(DWORD address, uint8_t *buffer, DWORD len);
extern void SPIFlashBeginWrite(DWORD dwAddr);
/*******************************************************************************
*                         	Lectura de byte
*
* Description : Esta funcion lee la posición de memoria especificada y retorna el
*				dato. Se lee byte a byte
* Function    : SST_25_ReadPage
*
* Arguments   : Direccion  - Apunta la direccion de 24Bits a Leer
				*buffer    - data buffer for data to be read
				Len        - Indica el numero de bytes a ser leidos (se puede leer toda la memoria)

*
* Returns     : *buffer    - contiene la informacion de la memoria
*
* Note		  :
*******************************************************************************/
extern uint8_t SPIFlashReadByte(DWORD Address);
extern void SPIFlashWrite(BYTE vData);
/*******************************************************************************
*                         	Escritura
*
* Description : Esta funcion lee la memoria desde la direccion de inicio hasta
*				el numero de bytes deseados
*
* Function    : SST_25_WritePage
*
* Arguments   : Direccion  - Apunta la direccion de 24Bits a Leer
				*buffer    - data buffer for data to be read
				Len        - Indica el numero de bytes a escribir

*
* Returns     : *buffer    - contiene la informacion de la memoria
*
* Note		  : La cantidad de datos grabados no debe exeder al tamaño de la
				la página, caso contrario es sobreescribiran las primeras posiciones
*******************************************************************************/
extern void SPIFlashWriteArray(BYTE *vData, WORD wLen);

/*******************************************************************************
*                         	Borrar Bloque
*
* Description : Esta funcion Borra un bloque de 64Kb (256 paginas de 256 bytes)
*
* Function    : SST_25_EraseBlock
*
* Arguments   : Address  - Apunta la direccion de 24Bits a borrar
*
* Returns     : None
*
* Note		  : Borra 64Kbytes
*
*******************************************************************************/
extern void SPIFlashEraseSector(DWORD dwAddr);

/*******************************************************************************
*                         	Config Hardware
*
* Description : Esta funcion habilita la escritura en la Memoria SST_25
*
* Function    : ConfigHWSST(SpiChannel SpiSST, uint32_t *port, uint32_t cs)
*
* Arguments   : Canal SPI, Puerto(LATx), CHIP SELECT (PIN DEL PUERTO)
*
* Returns     : none
*
* Note		  :
*******************************************************************************/
extern void SPIFlashInit(/*uint8_t SpiSST, volatile DWORD port, uint8_t cs*/);

#endif	/* SST25VF_H */

