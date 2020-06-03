#include <ATMEGA_FreeRTOS.h>
#include "../Headers/Sound.h"
#include "../Headers/config.h"
#include "../Headers/Logging.h"
#include "event_groups.h"
#include <stdio.h>


struct Sound {
	uint16_t dBMeasured;
};

Sound_t createSound() {
	Sound_t self = calloc(sizeof(Sound), 1);

	if (self == NULL){
		return NULL;
	}

	return self;
}

void measure(Sound_t self) {
	self->dBMeasured = (rand() % (150 - 0 + 1) + 0);
	Logging_MeasureInt("Sound: ", self->dBMeasured);
}

uint16_t getData(Sound_t self) {
	return self->dBMeasured;
}

void sound_task(void* pvParameters) {
	const Sound_t self = ((void**)pvParameters)[0];
	const EventGroupHandle_t measureEventGroup = ((void**)pvParameters)[1];
	const EventGroupHandle_t dataReadyEventGroup = ((void**)pvParameters)[2];

	while (1){
		EventBits_t uxBits = xEventGroupWaitBits(measureEventGroup, 
			SOUND_MEASURE_BIT,
			pdFALSE,
			pdTRUE,
			1000);

		if ((uxBits & (SOUND_MEASURE_BIT)) == (SOUND_MEASURE_BIT)) {

			measure(self);

			//Make this a 10ms delay
			vTaskDelay(500);

			xEventGroupSetBits(dataReadyEventGroup, SOUND_READY_BIT);
			xEventGroupClearBits(measureEventGroup, SOUND_MEASURE_BIT);
			Logging_Info("SOUND_READY_BIT set.");
		}
		else{
			Logging_Error("Timed-out waiting for SOUND_MEASURE_BIT to be set in measureEventGroup.");
		}
		
		
	}
}

