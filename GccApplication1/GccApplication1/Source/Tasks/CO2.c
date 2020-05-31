/*
 * CO2.c
 *
 * Created: 5/28/2020 6:33:07 PM
 *  Author: greta
 */ 
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <stdio_driver.h>
#include "../Headers/CO2.h"
#include <semphr.h>
#include "mh_z19.h"

SemaphoreHandle_t xTestSemaphore;

void CO2Sensor(void *pvParameters){
	xTestSemaphore = pvParameters;
	while(1){
		vTaskDelay(1000);
		int r = mh_z19_take_meassuring();
		if(r != MHZ19_OK)
		{
			printf("CO2 sensor: %d", r);
		}
		vTaskDelay(9000);
	}
}

void my_co2_call_back(uint16_t ppm){
	xSemaphoreTake (xTestSemaphore, portMAX_DELAY);
	printf("CO2 measured: %u \n", ppm);
	xSemaphoreGive(xTestSemaphore);
}
