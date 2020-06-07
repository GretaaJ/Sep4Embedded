#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"

#include <stdint.h>

typedef struct Temperature_Humidity Temperature_Humidity;
typedef struct Temperature_Humidity* Temperature_Humidity_t;

void initTempDriver();
Temperature_Humidity_t createTemp(EventGroupHandle_t measureEventGroup, EventGroupHandle_t dataReadyEventGroup, SemaphoreHandle_t semaphore);
uint16_t getHum(Temperature_Humidity_t self);
uint16_t getTemp(Temperature_Humidity_t self);
void th_Task(void* pvParameters);