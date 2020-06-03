/*
 * Temp_Hum.h
 *
 * Created: 5/31/2020 8:43:07 PM
 *  Author: greta
 */ 

#include <stdint.h>
#include <hih8120.h>

void T_HSensor(void *pvParameters);
uint16_t get_Hum();
uint16_t get_Temp();