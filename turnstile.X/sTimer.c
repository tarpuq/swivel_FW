/* 
 * File:   sTimer.c
 * Author:      gguzman
 * Comments:
 * Revision history: 
 */
#include <stdint.h>

#include "sTimers.h"

uint8_t readerTimeout = 0;
uint8_t readerTimeoutFlag = 0;

void softwareTimersHandler(void)
{
    if(readerTimeout){
        readerTimeout--;
        
        if(readerTimeout == 0)
            readerTimeoutFlag = 1;
    }
}
