/*
 * CO2.h
 *
 * Created: 5/31/2020 5:38:34 PM
 *  Author: greta
 */ 
#include <stdint.h>
#include <mh_z19.h>
#include <semphr.h>

void CO2Sensor(void *pvParameters);
void my_co2_call_back(uint16_t ppm);
//mh_z19_create(ser_USART3, &my_co2_call_back);