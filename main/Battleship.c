#include <stdio.h>
#include "espnow_comms.h"
#include "freertos/task.h"

void createTasks(void) {
    xTaskCreatePinnedToCore(handleDataTask, "handle_data", 4096, NULL, TASK_PRIO_0, NULL, 0);
}

void app_main(void)
{

}
