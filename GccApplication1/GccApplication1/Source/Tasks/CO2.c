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
#include "../drivers/mh_z19.h"
#include "../Headers/Logging.h"
#include "event_groups.h"
#include "../Headers/config.h"
#include <stdlib.h>

struct CO2 {
	uint16_t lastCO2ppm;
};


static int driverInitialized = 0;

CO2_t createCO2() {

		if (!driverInitialized) {
			mh_z19_create(ser_USART3, &my_co2_call_back);
			driverInitialized = 1;
			Logging_Info("CO2 Driver Initialized.");
		}
		else {
			Logging_Error("A CO2 Driver Has Already Been Initialized.");
		}

		CO2_t sensor = calloc(sizeof(CO2_t), 1);

		//setting lastCO2ppm to -1, so if the system returns a value of -1 we know it didn't measure.
		//-1 is used because you cannot have negative co2 in your room.
		sensor->lastCO2ppm = -1;

		return sensor;
}


//void create(void *pvParameters){
//	xTestSemaphore = pvParameters;
//	while(1){
//		vTaskDelay(1000);
//		int r = mh_z19_take_meassuring();
//		if(r != MHZ19_OK)
//		{
//			printf("CO2 sensor: %d", r);
//		}
//		vTaskDelay(9000);
//	}
//}


void my_co2_call_back(uint16_t ppm){
	Logging_MeasureInt("CO2 Measured: ", ppm);
}

uint16_t getCO2(CO2_t self)
{
	if (self->lastCO2ppm == -1) {
		Logging_Error("CO2 measurement not saved correctly.");
		return self->lastCO2ppm;
	}
	else
	{
		return self->lastCO2ppm;
	}
}

void CO2_task(void* pvParameters) {
	const CO2_t sensor = ((void**)pvParameters)[0];
	const EventGroupHandle_t measureEventGroup = ((void**)pvParameters)[1];
	const EventGroupHandle_t dataReadyEventGroup = ((void**)pvParameters)[2];


	while (1) {
		EventBits_t uxBits = xEventGroupWaitBits(measureEventGroup,
			CO2_MEASURE_BIT,
			pdFALSE,
			pdTRUE,
			1000);

		if ((uxBits & (CO2_MEASURE_BIT)) == (CO2_MEASURE_BIT)) {
			mh_z19_return_code_t rc = mh_z19_take_meassuring();

			//make this delay 500 ms
			vTaskDelay(500);

			if (rc == MHZ19_OK)
			{
				if (mh_z19_get_co2_ppm(&sensor->lastCO2ppm) == MHZ19_OK)
				{
					xEventGroupSetBits(dataReadyEventGroup, CO2_READY_BIT);
					xEventGroupClearBits(measureEventGroup, CO2_MEASURE_BIT);
					Logging_Info("CO2_READY_BIT set.");
				}
				else
				{
					Logging_Error("Something went wrong when saving the data.");
				}
			}
			else
			{
				Logging_Error("Measuring unsuccessful.");
			}
		}
		else
		{
			Logging_Error("Timed-out waiting for CO2_MEASURE_BIT to be set in measureEventGroup.");
		}

	}
}
