/*
 * Temp_Hum.c
 *
 * Created: 5/31/2020 8:43:29 PM
 *  Author: greta
 */ 


#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <stdio_driver.h>
#include "../Headers/Temp_Hum.h"



uint16_t latest_Hum;
uint16_t latest_Temp;
//SemaphoreHandle_t xTestSemaphore;

void T_HSensor(void *pvParameters){
	//xTestSemaphore = pvParameters;
	while(1)
	{
		vTaskDelay(1000);
		int r = hih8120Wakeup();
		if(r != HIH8120_OK && r != HIH8120_TWI_BUSY) {
			//puts("temp-wake error:");
		}
		vTaskDelay(100);
		r = hih8120Meassure();
		if(r != HIH8120_OK && r != HIH8120_TWI_BUSY) {
			//puts("Temp-read error: ");

		}
		vTaskDelay(100);
		
		
		///////////////////semaphore:
		//xSemaphoreTake(xTestSemaphore, portMAX_DELAY);
		//printf("Hum: %d  Temp: %d\n", (int)hih8120GetHumidity(), (int)hih8120GetTemperature());
		//xSemaphoreGive(xTestSemaphore);
	}
}

uint16_t get_Hum()
{
	latest_Hum = hih8120GetHumidity();
	return latest_Hum;
}

uint16_t get_Temp()
{
	latest_Temp = hih8120GetTemperature();
	return latest_Temp;
}