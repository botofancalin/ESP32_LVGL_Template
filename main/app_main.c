// Copyright 2015-2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* component includes */
#include <stdio.h>

/* freertos includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_freertos_hooks.h"

/* lvgl includes */
#include "iot_lvgl.h"

/* lvgl test includes */
#include "demo.h"

/* esp includes */
#include "esp_log.h"

/**********************
 *      MACROS
 **********************/
#define TAG "example_lvgl"

/**********************
 *  STATIC VARIABLES
 **********************/

static void user_task(void *pvParameter)
{
    demo_create();
    while (1)
    {
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void app_main()
{
    /* Initialize LittlevGL GUI */
    lvgl_init();

    /* littlevgl demo */

    xTaskCreate(
        user_task,   //Task Function
        "user_task", //Task Name
        4096,        //Stack Depth
        NULL,        //Parameters
        1,           //Priority
        NULL);       //Task Handler

    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
}
