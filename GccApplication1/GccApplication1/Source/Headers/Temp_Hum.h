/*
 * Temp_Hum.h
 *
 * Created: 5/31/2020 8:43:07 PM
 *  Author: greta
 */ 

#include <stdint.h>
#include <hih8120.h>
#include "../Headers/Logging.h"
#include "config.h"


typedef struct Temp_Hum Temp_Hum;
typedef struct Temp_Hum* Temp_Hum_t;

Temp_Hum_t create();
void wakeup();
void measure();
void setData(Temp_Hum_t self);
float getHum(Temp_Hum_t self);
float getTemp(Temp_Hum_t self);
void temp_humid_task(void* pvParameters);