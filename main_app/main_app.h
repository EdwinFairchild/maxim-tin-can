#ifndef _MAIN_APP_H
#define _MAIN_APP_H

#include "board.h"
#include "led.h"
#include "wsf_types.h"
#include "wsf_trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

enum{
    BUTTON_2_PRESSED = 0x01<<2,
    BUTTON_1_PRESSED = 0x01<<1,

};


void led_task(void *pvParameters);
void createAppTasks(void);

#endif