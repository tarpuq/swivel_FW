/* 
 * File:   
 * Author:      dortega
 * Comments:
 * Revision history: 
 */

#include "I2CSW.h"
#include "../mcc_generated_files/device_config.h"
#include "../mcc_generated_files/pin_manager.h"

//PBClock

#define SCL_In    SCLc_TRIS = 1
#define SCL_Out   SCLc_TRIS = 0
#define SCL_set   SCLc_SetHigh()
#define SCL_clr   SCLc_SetLow()
#define SCL_tgg   SCLc_Toggle()
#define SCL_sta   SCLc_GetValue()

#define SDA_In    SDAc_TRIS = 1
#define SDA_Out   SDAc_TRIS = 0
#define SDA_set   SDAc_SetHigh()
#define SDA_clr   SDAc_SetLow()
#define SDA_tgg   SDAc_Toggle()
#define SDA_sta   SDAc_GetValue()

#define DemoraCC()  __delay_us(3)
#define DemoraMC()  __delay_us(6)
#define DemoraTC()  __delay_us(9)

uint8_t ultSttSDA = 1;

void SetHighSCL(void)
{
    SCLc_TRIS = 1;
}

void SetLowSCL(void)
{
    SCLc_SetLow();
    SCLc_TRIS = 0;
}

void SetHighSDA(void)
{
    SDAc_TRIS = 1;
    ultSttSDA = 1;
}

void SetLowSDA(void)
{
    SDAc_SetLow();
    SDAc_TRIS = 0;
    ultSttSDA = 0;
}

void RestartI2C(void)
{
    SetHighSCL();
    DemoraMC();
    if (ultSttSDA)
        SetLowSDA();
    else
        SetHighSDA();
    DemoraCC();
    SetHighSDA();
    DemoraMC();
    SetLowSDA();
}

//Configurar Pines I2C

void InitI2C(void)
{
    SetHighSCL();
    DemoraMC();
    SetHighSDA();
    __delay_ms(1);
}

//Configurar Pines I2C

void StartI2C(void)
{
    DemoraCC();
    SetHighSCL();
    DemoraTC();
    SetLowSDA();
    DemoraMC();
}

//Configurar Pines I2C

void StopI2C(void)
{
    SetHighSCL();
    DemoraMC();
    if (ultSttSDA)
        SetLowSDA();
    else
        SetHighSDA();
    DemoraCC();
    SetHighSDA();
    DemoraTC();
}

uint8_t SendByteI2C(uint8_t data)
{
    uint8_t r, ACKr;
    SetLowSCL();
    DemoraMC();
    for (r = 0; r < 8; r++)
    {
        if (data & 0x80)
            SetHighSDA();
        else
            SetLowSDA();
        DemoraCC();
        SetHighSCL();
        data <<= 1;
        DemoraMC();
        SetLowSCL();
        DemoraCC();
    }
    SDA_In;
    DemoraCC();
    SetHighSCL();
    DemoraCC();
    ACKr = SDA_sta;
    DemoraCC();
    SetLowSCL();
    DemoraTC();
    return ACKr;
}

uint8_t ReceiveByteI2C(uint8_t nAck)
{
    char x, d;
    SDA_In;
    for (x = 0; x < 8; x++)
    {
        d <<= 1;
        DemoraCC();
        SetHighSCL();
        DemoraCC();
        if (SDA_sta)
            d |= 1;
        DemoraCC();
        SetLowSCL();
        DemoraCC();
    }
    if (nAck)
        SetHighSDA();
    else
        SetLowSDA();
    DemoraCC();
    SetHighSCL();
    DemoraMC();
    SetLowSCL();
    DemoraTC();
    SDA_In;
    return d;
}