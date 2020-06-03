/*
 * DataPacketHandler.c
 *
 * Created: 6/2/2020 7:20:57 PM
 *  Author: greta
 */ 
#include <ATMEGA_FreeRTOS.h>
#include "../Source/Headers/DataPacketHandler.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <queue.h>

uint16_t ppm;
uint16_t temp;
uint16_t hum;
uint16_t noise;

void setCO2 (uint16_t latest_ppm)
{
	ppm = latest_ppm;
	
}

void setTemperature (uint16_t latest_temp)
{
	temp = latest_temp;
	
}

void setHumidity (uint16_t latest_hum)
{
	hum = latest_hum;
	
}

void setNoise (uint16_t latest_noise)
{
	hum = latest_noise;
	
}

lora_payload_t getLoraPayload()
{
	lora_payload_t _uplink_payload;
	
	_uplink_payload.len = 8;
	_uplink_payload.port_no = 2;
	
	
	_uplink_payload.bytes[0] = hum >> 8;
	_uplink_payload.bytes[1] = hum & 0xFF;
	_uplink_payload.bytes[2] = temp >> 8;
	_uplink_payload.bytes[3] = temp & 0xFF;
	_uplink_payload.bytes[4] = ppm >> 8;
	_uplink_payload.bytes[5] = ppm & 0xFF;
	_uplink_payload.bytes[6] = noise >> 8;
	_uplink_payload.bytes[7] = noise & 0xFF;

	return _uplink_payload;

}

