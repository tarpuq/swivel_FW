/* 
 * File:   turnstile_app.c
 * Author:      gguzman
 * Comments:
 * Revision history: 
 */

#include "turnstile_app.h"

#include "mcc_generated_files/mcc.h"

#include "rfid_reader.h"
#include "RTC.h"

//  For AppConfig
#include "TCPIP Stack/TCPIP.h"
extern APP_CONFIG AppConfig;

static enum _MessageType
{
    MSG_TIMESTAMP = 0,
    MSG_ENTRY_REQUIRED,
    MSG_ENTRY_RESULT,
    MSG_EXIT_REQUIRED,
    MSG_EXIT_RESULT
};

uint8_t validationMsg = 0;
uint8_t peopleCounter = 0;

//  RTC
RTC_Time localTime;
uint8_t rtcUpdate = 1;

void turnstileTask(void)
{
    static uint8_t entryExit = 0;
    static DWORD Timer;
    uint8_t dataBuffer[10];
    static uint24_t cardNumber;
    static uint8_t testRequired = 0;
    static uint8_t testTimeout = 0;
    static uint8_t testStatus = 0;
    uint8_t sendDataLen = 0;
    uint8_t devId;
    uint8_t validCard;
    static uint8_t msgType;

    static enum _TurnstileState
    {
        SM_INIT = 0,
        SM_WAITING_EVENTS,
        SM_PREPARE_FRAME,
        SM_WAIT_SERVER_RESPONSE,
        SM_CHECKING_ESD,
        SM_ACCESS_GRANTED,
        SM_ACCESS_DENIED,
        SM_WAITING_PASS,
        SM_RESTARTING,
        SM_IDLE
    } TurnstileState = SM_INIT;

    switch (TurnstileState)
    {
    case SM_INIT:
        BUZZER_SetLow();
        RTC_Get(&localTime);
        TurnstileState = SM_WAITING_EVENTS;
        break;
    case SM_WAITING_EVENTS:
        validationMsg = 0;

        //  Check READERS
        if (rfidAIsDataReady())
        {
            cardNumber = rfidAGetCardNumberInt();

            if (AppConfig.Direction == ENTRY_DIRECTION_A2B)
            {
                entryExit = REQUIRE_ENTRY;
                msgType = MSG_ENTRY_REQUIRED;
            }
            else if (AppConfig.Direction == ENTRY_DIRECTION_B2A)
            {
                entryExit = REQUIRE_EXIT;
                msgType = MSG_EXIT_REQUIRED;
            }

            TurnstileState = SM_PREPARE_FRAME;
        }

        if (rfidBIsDataReady())
        {
            cardNumber = rfidBGetCardNumberInt();

            if (AppConfig.Direction == ENTRY_DIRECTION_A2B)
            {
                entryExit = REQUIRE_EXIT;
                msgType = MSG_EXIT_REQUIRED;
            }
            else if (AppConfig.Direction == ENTRY_DIRECTION_B2A)
            {
                entryExit = REQUIRE_ENTRY;
                msgType = MSG_ENTRY_REQUIRED;
            }

            TurnstileState = SM_PREPARE_FRAME;
        }

        if (rtcUpdate)
        {
            rtcUpdate = 0;
            cardNumber = 0;
            msgType = MSG_TIMESTAMP;
            TurnstileState = SM_PREPARE_FRAME;
        }

        break;
    case SM_PREPARE_FRAME:
        if (!entryExit)
        {
            dataBuffer[sendDataLen++] = FRAME_DIRECTION_ENTRY | AppConfig.DeviceID;
        }
        else
        {
            dataBuffer[sendDataLen++] = FRANE_DIRECTION_EXIT | AppConfig.DeviceID;
        }
        dataBuffer[sendDataLen++] = msgType;

        if (cardNumber)
        {
            dataBuffer[sendDataLen++] = (uint8_t) cardNumber;
            dataBuffer[sendDataLen++] = (uint8_t) (cardNumber >> 8);
            dataBuffer[sendDataLen++] = (uint8_t) (cardNumber >> 16);

            if (validationMsg)
            {
                dataBuffer[sendDataLen++] = testStatus;
            }
        }

        setFrame(dataBuffer, sendDataLen);

        Timer = TickGet();

        TurnstileState = SM_WAIT_SERVER_RESPONSE;
        break;
    case SM_WAIT_SERVER_RESPONSE:
        //  Process answer
        if (isServerDataReady())
        {
            getFrame(dataBuffer);

            devId = dataBuffer[0] & 0x7F;
            msgType = dataBuffer[1];

            if (devId == AppConfig.DeviceID)
            {
                switch (msgType)
                {
                case MSG_TIMESTAMP:
                    localTime.day = dataBuffer[2];
                    localTime.month = dataBuffer[3];
                    localTime.year = dataBuffer[4];
                    localTime.hours = dataBuffer[5];
                    localTime.minutes = dataBuffer[6];
                    localTime.seconds = dataBuffer[7];

                    RTC_Set(&localTime);
                    break;
                case MSG_ENTRY_REQUIRED:
                    validCard = dataBuffer[2];

                    if (validCard == 1)
                    {
                        BUZZER_SetHigh();
                        __delay_ms(100);
                        BUZZER_SetLow();

                        testRequired = dataBuffer[3];
                        testTimeout = dataBuffer[4];

                        TurnstileState = SM_CHECKING_ESD;
                    }
                    else
                    {
                        //  Card not valid
                        TurnstileState = SM_RESTARTING;
                    }
                    break;
                case MSG_ENTRY_RESULT:
                    if (testStatus)
                    {
                        peopleCounter++;
                    }
                    else
                    {

                    }
                    TurnstileState = SM_RESTARTING;
                    break;
                case MSG_EXIT_REQUIRED:
                    validCard = dataBuffer[2];

                    if (validCard == 1)
                    {
                        BUZZER_SetHigh();
                        __delay_ms(100);
                        BUZZER_SetLow();

                        testTimeout = AppConfig.ExitTimeout;
                        TurnstileState = SM_ACCESS_GRANTED;

                    }
                    else
                    {
                        //  Card not valid
                        TurnstileState = SM_RESTARTING;
                    }
                    break;
                case MSG_EXIT_RESULT:
                    if (testStatus)
                    {
                        if (peopleCounter)
                        {
                            peopleCounter--;
                        }
                    }
                    else
                    {

                    }
                    TurnstileState = SM_RESTARTING;
                    break;
                default:
                    break;
                }

                Timer = TickGet();
            }
        }

        // Time out if too much time is spent in this state
        if (TickGet() - Timer > 5 * TICK_SECOND)
        {
            TurnstileState = SM_RESTARTING;
        }
        break;
    case SM_CHECKING_ESD:
        if (MSG_GetValue() == 0)
        {
            testStatus = 1;
            TurnstileState = SM_ACCESS_GRANTED;

            Timer = TickGet();
        }

        // Time out if too much time is spent in this state
        if (TickGet() - Timer > AppConfig.ESDCheckTimeout * TICK_SECOND)
        {
            BUZZER_SetHigh();
            __delay_ms(100);
            BUZZER_SetLow();
            __delay_ms(100);
            BUZZER_SetHigh();
            __delay_ms(100);
            BUZZER_SetLow();

            TurnstileState = SM_ACCESS_DENIED;
        }
        break;
    case SM_ACCESS_DENIED:
        validationMsg = 1;
        testStatus = 0;
        msgType = MSG_ENTRY_RESULT;
        TurnstileState = SM_PREPARE_FRAME;
        break;
    case SM_ACCESS_GRANTED:
        BUZZER_SetHigh();

        if (AppConfig.Direction == ENTRY_DIRECTION_A2B)
        {
            if (entryExit == REQUIRE_ENTRY)
            {
                ROTATE_A2B_SetHigh();
            }
            else if(entryExit == REQUIRE_EXIT)
            {
                ROTATE_B2A_SetHigh();
                __delay_ms(250);
            }
        }
        else if (AppConfig.Direction == ENTRY_DIRECTION_B2A)
        {
            if (entryExit == REQUIRE_ENTRY)
            {
                ROTATE_B2A_SetHigh();
            }
            else if(entryExit == REQUIRE_EXIT)
            {
                ROTATE_A2B_SetHigh();
                __delay_ms(250);
            }
        }

        BUZZER_SetLow();
        TurnstileState = SM_WAITING_PASS;
        break;
    case SM_WAITING_PASS:
        validationMsg = 1;

        if (!entryExit)
        {
            msgType = MSG_ENTRY_RESULT;
        }
        else
        {
            msgType = MSG_EXIT_RESULT;
        }

        if (ALARM_GetValue() == 0)
        {
            testStatus = 1;
            TurnstileState = SM_PREPARE_FRAME;
        }

        // Time out if too much time is spent in this state
        if (TickGet() - Timer > testTimeout * TICK_SECOND)
        {
            testStatus = 0;

            BUZZER_SetHigh();
            __delay_ms(100);
            BUZZER_SetLow();
            __delay_ms(100);
            BUZZER_SetHigh();
            __delay_ms(100);
            BUZZER_SetLow();

            TurnstileState = SM_PREPARE_FRAME;
        }
        break;
    case SM_RESTARTING:
        ROTATE_A2B_SetLow();
        ROTATE_B2A_SetLow();
        rfidReset();
        TurnstileState = SM_WAITING_EVENTS;
        break;
    case SM_IDLE:
        break;
    }
}
