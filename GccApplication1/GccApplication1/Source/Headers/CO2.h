/*
 * CO2.h
 *
 * Created: 5/31/2020 5:38:34 PM
 *  Author: greta
 */ 


#include <stdint.h>
#include <mh_z19.h>
#include <semphr.h>

typedef struct CO2 CO2;
typedef struct CO2* CO2_t;

CO2_t createCO2();
void my_co2_call_back(uint16_t ppm);
uint16_t getCO2();
void CO2_task(void* pvParameters);