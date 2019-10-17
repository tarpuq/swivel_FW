///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                             DS1307.c                              ////
////                                                                   ////
////                       Driver for C compiler                       ////
////                                                                   ////
////     Driver for Maxim DS1307 serial I2C real-time clock (RTC).     ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     https://simple-circuit.com/                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////

#include <xc.h>
#include <stdint.h>
#include "../mcc_generated_files/device_config.h"

#include "RTC.h"
#include "I2CSW.h"

// converts BCD to decimal

uint8_t bcd_to_decimal(uint8_t number)
{
    return ( (number >> 4) * 10 + (number & 0x0F));
}

// converts decimal to BCD

uint8_t decimal_to_bcd(uint8_t number)
{
    return (((number / 10) << 4) + (number % 10));
}

// sets time and date

void RTC_Set(RTC_Time *time_t)
{
    uint8_t sec_reg = RTC_Read_Reg(DS1307_REG_SECONDS);
    // convert decimal to BCD
    time_t->day = decimal_to_bcd(time_t->day);
    time_t->month = decimal_to_bcd(time_t->month);
    time_t->year = decimal_to_bcd(time_t->year);
    time_t->hours = decimal_to_bcd(time_t->hours);
    time_t->minutes = decimal_to_bcd(time_t->minutes);
    time_t->seconds = decimal_to_bcd(time_t->seconds);
    // test if oscillator is disabled (CH bit = 1)
    if (sec_reg & 0x80)
        time_t->seconds |= 0x80;
    // write data to the RTC chip
    StartI2C();
    SendByteI2C(DS1307_ADDRESS);
    SendByteI2C(DS1307_REG_SECONDS);
    SendByteI2C(time_t->seconds);
    SendByteI2C(time_t->minutes);
    SendByteI2C(time_t->hours);
    SendByteI2C(time_t->dow);
    SendByteI2C(time_t->day);
    SendByteI2C(time_t->month);
    SendByteI2C(time_t->year);
    StopI2C();
}

// reads time and date

void RTC_Get(RTC_Time *tm)
{
    StartI2C();
    SendByteI2C(DS1307_ADDRESS);
    SendByteI2C(DS1307_REG_SECONDS);
    RestartI2C();
    SendByteI2C(DS1307_ADDRESS | 0x01);
    tm->seconds = ReceiveByteI2C(ACK) & 0x7F;
    tm->minutes = ReceiveByteI2C(ACK);
    tm->hours = ReceiveByteI2C(ACK);
    tm->dow = ReceiveByteI2C(ACK);
    tm->day = ReceiveByteI2C(ACK);
    tm->month = ReceiveByteI2C(ACK);
    tm->year = ReceiveByteI2C(NACK);
    StopI2C();
    // convert BCD to decimal
    tm->seconds = bcd_to_decimal(tm->seconds);
    tm->minutes = bcd_to_decimal(tm->minutes);
    tm->hours = bcd_to_decimal(tm->hours);
    tm->day = bcd_to_decimal(tm->day);
    tm->month = bcd_to_decimal(tm->month);
    tm->year = bcd_to_decimal(tm->year);
}

// writes 'reg_value' to register of address 'reg_address'

void RTC_Write_Reg(uint8_t reg_address, uint8_t reg_value)
{
    StartI2C();
    SendByteI2C(DS1307_ADDRESS);
    SendByteI2C(reg_address);
    SendByteI2C(reg_value);
    StopI2C();
}

// returns the value stored in register of address 'reg_address'

uint8_t RTC_Read_Reg(uint8_t reg_address)
{
    uint8_t reg_data;

    StartI2C();
    SendByteI2C(DS1307_ADDRESS);
    SendByteI2C(reg_address);
    RestartI2C();
    SendByteI2C(DS1307_ADDRESS | 0x01);
    reg_data = ReceiveByteI2C(1);
    StopI2C();

    return reg_data;
}

// sets SQWE pin output

void SQWE_Set(SQWE pin_out)
{
    StartI2C();
    SendByteI2C(DS1307_ADDRESS);
    SendByteI2C(DS1307_REG_CONTROL);
    SendByteI2C(pin_out);
    StopI2C();
}

// enables RTC oscillator

void OSC_Enable()
{
    uint8_t sec_reg = RTC_Read_Reg(DS1307_REG_SECONDS);
    sec_reg &= 0x7F;
    RTC_Write_Reg(DS1307_REG_SECONDS, sec_reg);
}

// disables RTC oscillator

void OSC_Disable()
{
    uint8_t sec_reg = RTC_Read_Reg(DS1307_REG_SECONDS);
    sec_reg |= 0x80;
    RTC_Write_Reg(DS1307_REG_SECONDS, sec_reg);
}

void InitRTC()
{
    OSC_Enable();
}