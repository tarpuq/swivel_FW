/* 
 * File:   inputs.c
 * Author:      gguzman
 * Comments:
 * Revision history: 
 */

#include "inputs.h"
#include "mcc_generated_files/mcc.h"

input_t alarm;
input_t msg;
input_t exit;

void processInput(input_t *input)
{
    if ((input->prevValue ^ input->value) && !input->debounce.running)
    {
        input->debounce.timeout = 5;
    }

    if (input->debounce.done)
    {
        input->debounce.done = false;

        if (!input->prevState && !input->value)
        {
            input->rise = true;
            input->state = true;
            input->riseEdge.timeout = 5000; //  5 seconds to rise edge expire
        }

        if (input->prevState && input->value)
        {
            input->fall = true;
            input->state = false;
            input->fallEdge.timeout = 5000; //  5 seconds to fall edge expire
        }
    }

    if (input->riseEdge.done)
    {
        input->riseEdge.done = false;
        input->rise = false;
    }

    if (input->fallEdge.done)
    {
        input->fallEdge.done = false;
        input->fall = false;
    }

    input->prevState = input->state;
    input->prevValue = input->value;
}

bool inputIsRise(input_t *input)
{
    if (input->rise == true)
    {
        input->riseEdge.timeout = 0;
        input->rise = false;
        return true;
    }
    return false;
}

void inputsProcess(void)
{
    alarm.value = ALARM_GetValue();
    msg.value = MSG_GetValue();
    exit.value = EXIT_GetValue();

    processTimer(&alarm.debounce);
    processTimer(&msg.debounce);
    processTimer(&exit.debounce);

    processTimer(&alarm.riseEdge);
    processTimer(&msg.riseEdge);
    processTimer(&exit.riseEdge);

    processTimer(&alarm.fallEdge);
    processTimer(&msg.fallEdge);
    processTimer(&exit.fallEdge);

    processInput(&alarm);
    processInput(&msg);
    processInput(&exit);
}
