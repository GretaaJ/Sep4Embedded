/*
 * Controller.c
 *
 * Created: 6/3/2020 2:36:30 PM
 *  Author: greta
 */ 
#include <ATMEGA_FreeRTOS.h>
#include "../Source/Headers/Temp_Hum.h"
#include "../Source/Headers/CO2.h"
#include "../Source/Headers/DataPacketHandler.h"
#include "../Source/Headers/Controller.h"
#include <lora_driver.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <queue.h>

uint16_t latest_Hum;
uint16_t latest_Temp;



void ControllerTask(void *pvParameters){
	QueueHandle_t xQueue = ((void**)pvParameters)[0];
	lora_payload_t payload = getLoraPayload();
	//event group stuff
	
	for (;;)
	{ 
		setHumidity(get_Hum());
		setTemperature(get_Temp());
		setCO2(get_CO2());
		setNoise(1);
		
		payload = getLoraPayload();
		
		xQueueSend(xQueue, &payload, portMAX_DELAY);
	}
}

