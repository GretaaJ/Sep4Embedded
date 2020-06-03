/*
 * DataPacketHandler.h
 *
 * Created: 6/3/2020 3:25:56 PM
 *  Author: greta
 */ 
#include <lora_driver.h>

void setCO2 (uint16_t latest_ppm);
void setTemperature (uint16_t latest_temp);
void setHumidity (uint16_t latest_hum);
void setNoise (uint16_t latest_noise);
lora_payload_t getLoraPayload();