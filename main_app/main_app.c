#include "main_app.h"

void led_task(void *pvParameters)
{
    const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;
    LED_Off(0);
    LED_Off(1);
    LED_Off(2);
    volatile uint32_t counter = 0;
    while(1)
    {
        //LED_Toggle(0);
        //LED_Toggle(1);
        //LED_Toggle(1);
        LED_Off(0);
        LED_Off(1);
        LED_Off(2);
        APP_TRACE_INFO1("Hello from led task!\nCount : %d\n", counter++);

        vTaskDelay(xDelay);
    }

}
void createAppTasks(void)
{
    int status = 0 ;
    status = xTaskCreate(led_task, "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	if (status == -1)
		APP_TRACE_INFO0("Error creating LED task task\n");

}

