/* 
 * File:   sTimers.c
 * Author:      gguzman
 * Comments:
 * Revision history: 
 */
#include <stdint.h>

#include "sTimers.h"
#include "inputs.h"
#include "beep.h"
#include "GenericTypeDefs.h"

timer_t readerTimer = {
    0,
    FALSE,
    FALSE
};

bool timerIsDone(timer_t *timer){
    if(timer->done){
        timer->done = false;
        return true;
    }
    return false;
}

void processTimer(timer_t *timer)
{
    if (timer->timeout)
    {
        timer->timeout--;

        timer->running = TRUE;

        if (timer->timeout == 0)
        {
            timer->done = TRUE;
            timer->running = FALSE;
        }
    }
}

void softwareTimersHandler(void)
{
    processTimer(&readerTimer);

    //  Inputs
    inputsProcess();

    //  Beep
    beepProcess();
}
