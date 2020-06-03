/*
 * Temp_Hum.c
 *
 * Created: 5/31/2020 8:43:29 PM
 *  Author: greta
 */ 
#include <ATMEGA_FreeRTOS.h>
#include <stdint.h>
#include "../drivers/hih8120.h"
#include <stdio.h>
#include <stdio_driver.h>
#include "../Headers/Temp_Hum.h"
#include "event_groups.h"
#include <stdlib.h>
//#include "Windows.h"

struct Temp_Hum {
	float temperature;
	float humidity;
};

static int driverInitialized = 0;

Temp_Hum_t createTempHumid() {

	if(driverInitialized == 0) {
		hih8120DriverReturnCode_t rc = hih8120Create();

		if (rc == HIH8120_OUT_OF_HEAP){
			Logging_Error("There is not enough HEAP memory to create the driver.");
		}
		else if(rc == HIH8120_OK){
			Logging_Info("Driver initialized correctly.");
		}
	}
	else{
	Logging_Error("A temperature and humidity driver has already been initialized.");
	}

	Temp_Hum_t sensor = calloc(sizeof(Temp_Hum), 1);

	sensor->humidity = (float)-1;
	sensor->temperature = (float)-274;

	return sensor;
}

void wakeup() {

	hih8120DriverReturnCode_t rc = hih8120Wakeup();

	while (rc != HIH8120_OK)
	{
		if (rc == HIH8120_DRIVER_NOT_CREATED) {
			Logging_Error("The driver was not created, attempting to create driver.");
			createTempHumid();
			
			//delay 100ms
			vTaskDelay(500);
			rc = hih8120Wakeup();
		}
		else if (rc == HIH8120_TWI_BUSY) {
			Logging_Error("The two wire/I2C interface is busy, trying again.");
			//Sleep(100);
			rc = hih8120Wakeup();
		}
	}

	Logging_Info("The temperature and humidity driver has been awoken.");
}

void measureTempHumid() {

	hih8120DriverReturnCode_t rc = hih8120Meassure();

	while (rc != HIH8120_OK){
		if (rc == HIH8120_DRIVER_NOT_CREATED){
			Logging_Error("The driver was not created, attempting to create driver.");
			createTempHumid();
			//Sleep(50);

			wakeup();
			//Sleep(100);

			rc = hih8120Meassure();
		}
		else if (rc == HIH8120_TWI_BUSY) {
			Logging_Error("The two wire/I2C interface is busy, trying again.");
			//Sleep(100);
			rc = hih8120Meassure();
		}
	}

	Logging_Info("Temperature and Humidty measured.");

}

void setDataTempHumid(Temp_Hum_t self) {
	
	int rc = hih8120IsReady();
	
	if (rc){
		self->humidity = hih8120GetHumidity();
		self->temperature = hih8120GetTemperature();
		Logging_Info("Temperature and humidity values has been set.");
	}
	
	while (!rc)
	{
		Logging_Error("Results are not ready yet, measuring is in progress, trying again.");
		rc = hih8120IsReady();
	}
	

}


float getHum(Temp_Hum_t self) {
	if (self->humidity == -1)
	{
		Logging_Error("The returned temperature value is invalid.");
		return self->humidity;
	}
	else
	{
		return self->humidity;
	}
}

float getTemp(Temp_Hum_t self) {
	if (self->temperature == -274)
	{
		Logging_Error("The returned temperature value is invalid.");
		return self->temperature;
	}
	else
	{
		return self->temperature;
	}
}

void temp_humid_task(void* pvParameters) {
	const Temp_Hum_t sensor = ((void**)pvParameters)[0];
	const EventGroupHandle_t measureEventGroup = ((void**)pvParameters)[1];
	const EventGroupHandle_t dataReadyEventGroup = ((void**)pvParameters)[2];


	while (1) {
		EventBits_t uxBits = xEventGroupWaitBits(measureEventGroup,
			TEMP_HUM_MEASURE_BIT,
			pdFALSE,
			pdTRUE,
			1000);

		if ((uxBits & (TEMP_HUM_MEASURE_BIT)) == (TEMP_HUM_MEASURE_BIT)) {

			wakeup();

			//should be 100ms delay
			vTaskDelay(500);

			measureTempHumid();

			//should be 10ms delay
			vTaskDelay(500);

			setDataTempHumid(sensor);

			xEventGroupSetBits(dataReadyEventGroup, TEMP_HUM_READY_BIT);
			xEventGroupClearBits(measureEventGroup, TEMP_HUM_MEASURE_BIT);
			Logging_Info("TEMP_HUM_READY_BIT set.");
		}
		else {
			Logging_Error("Timed-out waiting for TEMP_HUM_MEASURE_BIT to be set in measureEventGroup.");
		}
	}
}

//void T_HSensor(void *pvParameters){
//	xTestSemaphore = pvParameters;
//	while(1)
//	{
//		vTaskDelay(1000);
//		int r = hih8120Wakeup();
//		if(r != HIH8120_OK && r != HIH8120_TWI_BUSY) {
//			puts("temp-wake error:");
//		}
//		vTaskDelay(100);
//		r = hih8120Meassure();
//		if(r != HIH8120_OK && r != HIH8120_TWI_BUSY) {
//			puts("Temp-read error: ");
//
//		}
//		vTaskDelay(100);
//		///////////////////semaphore:
//		xSemaphoreTake(xTestSemaphore, portMAX_DELAY);
//		printf("Hum: %d  Temp: %d\n", (int)hih8120GetHumidity(), (int)hih8120GetTemperature());
//		xSemaphoreGive(xTestSemaphore);
//	}
//}