#include "main_app.h"


TaskHandle_t button_actions_hdl;
TaskHandle_t button_1_handle;

void led_task(void *pvParameters)
{
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
    LED_Off(0);
    LED_Off(1);
    LED_Off(2);
    volatile uint32_t counter = 0;
    while(1)
    {
        APP_TRACE_INFO1("Hello from led task!\r\nCount : %d\n", counter++);
        vTaskDelay(xDelay);
    }
}

void button_1_task(void *pvParameters)
{
    while(1)
    {

    }
}

void button_actions_task(void *pvParameters)
{
    uint32_t notifiedValue;
    while(1)
    {
        //ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        xTaskNotifyWait(0,0xFFFFFFFF,&notifiedValue,portMAX_DELAY);
        switch (notifiedValue){
            case BUTTON_1_PRESSED:
                break;

            case BUTTON_2_PRESSED:
                APP_TRACE_INFO0("Woooh recevied");
                break;
        }
    }
}
void createAppTasks(void)
{
    BaseType_t status = 0 ;
    status = xTaskCreate(led_task, "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	if (status != pdPASS)
		APP_TRACE_INFO0("Error creating LED task task\n");

    status = xTaskCreate(button_actions_task, "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &button_actions_hdl);
	if (status != pdPASS)
		APP_TRACE_INFO0("Error creating LED task task\n");

    // status = xTaskCreate(button_1_task, "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &button_1_handle);
	// if (status != pdPASS)
	// 	APP_TRACE_INFO0("Error creating LED task task\n");

}

