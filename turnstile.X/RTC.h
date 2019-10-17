///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                             DS1307.h                              ////
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


#include <stdint.h>

#define DS1307_ADDRESS      0xD0
#define DS1307_REG_SECONDS  0x00
#define DS1307_REG_CONTROL  0x07

typedef enum
{
    OUT_OFF = 0x00,
    OUT_ON = 0x80,
    OUT_1Hz = 0x10,
    OUT_4096Hz = 0x11,
    OUT_8192Hz = 0x12,
    OUT_32768Hz = 0x13
} SQWE;

typedef enum
{
    SUNDAY = 1,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
} RTC_DOW;

typedef enum
{
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
} RTC_Month;

typedef struct rtc_tm
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    RTC_DOW dow;
    uint8_t day;
    RTC_Month month;
    uint8_t year;
} RTC_Time;

extern uint8_t bcd_to_decimal(uint8_t number);
extern uint8_t decimal_to_bcd(uint8_t number);
extern void RTC_Set(RTC_Time *time_t);
extern void RTC_Get(RTC_Time *tm);
extern void RTC_Write_Reg(uint8_t reg_address, uint8_t reg_value);
extern uint8_t RTC_Read_Reg(uint8_t reg_address);
extern void SQWE_Set(SQWE pin_out);
extern void OSC_Enable();
extern void OSC_Disable();
extern void InitRTC();
