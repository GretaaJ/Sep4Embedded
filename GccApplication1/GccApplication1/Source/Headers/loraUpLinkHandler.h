#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"

typedef struct LoraUpLink LoraUpLink;
typedef struct LoraUpLink* LoraUpLink_t;

LoraUpLink_t createLoraUpLink(EventGroupHandle_t loraReadyEventGroup, QueueHandle_t payloadQueue);
void loraUpLinkTask(void* pvParameters);
