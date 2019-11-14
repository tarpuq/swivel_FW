/* 
 * File:   beep.c
 * Author:      gguzman
 * Comments:
 * Revision history: 
 */

#include "mcc_generated_files/mcc.h"
#include "sTimers.h"

#define BUFFER_LEN 10

typedef struct
{
    uint16_t times;
    uint16_t width;
    uint16_t delay;
    uint16_t interlude;
} beep_t;

beep_t beepBuffer[BUFFER_LEN];
uint8_t beepBufferIndex;

timer_t beepDelayTimer;
timer_t beepBeepingTimer;
timer_t beepInterludeTimer;

typedef enum
{
    BeepInit,
    BeepIdle,
    BeepDelay,
    BeepTurnOn,
    BeepBeeping,
    BeepTurnOff,
    BeepInterlude
} beepStates_t;

beepStates_t beepState;
bool bufferFilled;

void rotateBuffers()
{
    int i;

    for (i = 0; i < BUFFER_LEN - 1; i++)
    {
        beepBuffer[i].times = beepBuffer[i + 1].times;
        beepBuffer[i].width = beepBuffer[i + 1].width;
        beepBuffer[i].delay = beepBuffer[i + 1].delay;
        beepBuffer[i].interlude = beepBuffer[i + 1].interlude;
    }
    beepBuffer[BUFFER_LEN - 1].times = 0;
    beepBuffer[BUFFER_LEN - 1].width = 0;
    beepBuffer[BUFFER_LEN - 1].delay = 0;
    beepBuffer[BUFFER_LEN - 1].interlude = 0;

    beepBufferIndex--;

    if (!beepBufferIndex)
        bufferFilled = false;
}

void beep(uint8_t times, uint16_t width, uint16_t delay, uint16_t interlude)
{
    beepBuffer[beepBufferIndex].times = times;
    beepBuffer[beepBufferIndex].width = width;
    beepBuffer[beepBufferIndex].delay = delay ? delay : 1;
    beepBuffer[beepBufferIndex].interlude = interlude ? interlude : 1;
    beepBufferIndex++;
    bufferFilled = true;
}

void beepInit(void)
{
    beepState = BeepInit;
    beepBufferIndex = 0;
    bufferFilled = false;
}

void beepProcess(void)
{
    processTimer(&beepDelayTimer);
    processTimer(&beepBeepingTimer);
    processTimer(&beepInterludeTimer);

    switch (beepState)
    {
    case BeepInit: //  Init
        BUZZER_SetLow();
        beepState++;
        break;
    case BeepIdle: //  Idle
        if (beepBufferIndex)
        {
            beepState = BeepDelay;
            beepDelayTimer.timeout = (uint32_t) beepBuffer[0].delay;
        }
        break;
    case BeepDelay: //  Delay
        if (timerIsDone(&beepDelayTimer))
            beepState++;
        break;
    case BeepTurnOn: //  Turn On
        BUZZER_SetHigh();
        beepBeepingTimer.timeout = (uint32_t) beepBuffer[0].width;
        beepState++;
        break;
    case BeepBeeping: //  Beeping
        if (timerIsDone(&beepBeepingTimer))
            beepState++;
        break;
    case BeepTurnOff: //  Turn Off
        BUZZER_SetLow();
        beepInterludeTimer.timeout = (uint32_t) beepBuffer[0].interlude;
        beepState++;
        break;
    case BeepInterlude: //  Interlude
        if (timerIsDone(&beepInterludeTimer))
        {
            if (beepBuffer[0].times)
            {
                beepBuffer[0].times--;

                if (beepBuffer[0].times == 0)
                    rotateBuffers();

                beepState = BeepIdle;
            }
        }
        break;
    default:
        beepState = BeepInit;
        break;
    }

}
