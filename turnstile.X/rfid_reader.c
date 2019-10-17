#include "mcc_generated_files/mcc.h"
#include "rfid_reader.h"
#include "sTimers.h"

typedef union
{
    uint32_t wiegand26;

    struct
    {
        uint8_t trailingParity : 1;
        uint8_t cardNumberL : 8;
        uint8_t cardNumberH : 8;
        uint8_t facilityCode : 8;
        uint8_t leadingParity : 1;
        uint8_t na : 6;
    } fields;
} wiegand26_t;

static uint32_t dataBufferA = 0;
static uint32_t wiegandA;
static uint24_t cardNumberIntA;
static uint16_t facilityCodeA = 0;
static uint16_t cardNumberA = 0;
static uint8_t rfidADataReady = 0;
static uint8_t indexA = 0;

static uint32_t dataBufferB = 0;
static uint32_t wiegandB;
static uint24_t cardNumberIntB;
static uint16_t facilityCodeB = 0;
static uint16_t cardNumberB = 0;
static uint8_t rfidBDataReady = 0;
static uint8_t indexB = 0;

uint8_t rfidAIsDataReady(void)
{
    if (rfidADataReady == 1)
    {
        rfidADataReady = 0;
        return 1;
    }
    return 0;
}

uint8_t rfidBIsDataReady(void)
{
    if (rfidBDataReady == 1)
    {
        rfidBDataReady = 0;
        return 1;
    }
    return 0;
}

void rfidTask()
{
    if (readerTimeoutFlag)
    {
        readerTimeoutFlag = 0;

        indexA = 0;
        dataBufferA = 0;
        indexB = 0;
        dataBufferB = 0;
    }
}

void rfidAProcess(void)
{
    uint8_t data_bit = !READER_A_DATA_GetValue();
    dataBufferA |= (uint32_t) (data_bit & 0x01) << (25 - indexA);
    indexA++;

    readerTimeout = 10;

    if (indexA >= 26)
    {
        wiegandA = dataBufferA;
        cardNumberIntA = (wiegandA >> 1) & 0x00FFFFFF;
        cardNumberA = cardNumberIntA & 0x000FFFF;
        facilityCodeA = (cardNumberIntA & 0x00FF0000) >> 16;

        rfidADataReady = 1;

        readerTimeout = 0;

        dataBufferA = 0;
        indexA = 0;
    }
}

void rfidBProcess(void)
{
    uint8_t data_bit = !READER_B_DATA_GetValue();
    dataBufferB |= (uint32_t) (data_bit & 0x01) << (25 - indexB);
    indexB++;

    readerTimeout = 10;

    if (indexB >= 26)
    {
        wiegandB = dataBufferB;
        cardNumberIntB = (wiegandB >> 1) & 0x00FFFFFF;
        cardNumberB = cardNumberIntB & 0x000FFFF;
        facilityCodeB = (cardNumberIntB & 0x00FF0000) >> 16;

        rfidBDataReady = 1;

        readerTimeout = 0;

        dataBufferB = 0;
        indexB = 0;
    }
}

uint24_t rfidAGetCardNumberInt()
{
    return cardNumberIntA;
}

uint24_t rfidBGetCardNumberInt()
{
    return cardNumberIntB;
}
