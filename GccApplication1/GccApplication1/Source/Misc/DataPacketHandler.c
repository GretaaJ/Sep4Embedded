#include <ATMEGA_FreeRTOS.h>
#include "../Headers/DataPacketHandler.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <queue.h>
#include <stdlib.h>

struct DataPacketHandler{
	//if it works it ain't stupid.
	uint16_t dummy;
	uint16_t temperature;
	uint16_t ppm;
	uint16_t humidity;
	uint16_t dB;	
};

DataPacketHandler_t dph_Create(){
	DataPacketHandler_t self = malloc(sizeof(DataPacketHandler));
	
	if (self == NULL){
		return NULL;
	}
	
	self->dummy = 0;
	self->temperature = 0;
	self->ppm = 0;
	self->humidity = 0;
	self->dB =0;
	
	return self;
}

void dph_SetCO2 (uint16_t CO2ppm, DataPacketHandler_t self)
{
	self->ppm = CO2ppm;
	
}

void dph_SetTemperature (uint16_t temperature, DataPacketHandler_t self)
{
	self->temperature = temperature;
	
}

void dph_SetHumidity (uint16_t humidity, DataPacketHandler_t self)
{
	self->humidity = humidity;
	
}

void dph_SetDb (uint16_t dB, DataPacketHandler_t self)
{
	self->dB = dB;
	
}

lora_payload_t dph_Assemble(DataPacketHandler_t self)
{
	lora_payload_t _uplink_payload;
	
	_uplink_payload.len = 8;
	_uplink_payload.port_no = 2;
	
	
	_uplink_payload.bytes[0] = self->humidity >> 8;
	_uplink_payload.bytes[1] = self->humidity & 0xFF;
	_uplink_payload.bytes[2] = self->temperature >> 8;
	_uplink_payload.bytes[3] = self->temperature & 0xFF;
	_uplink_payload.bytes[4] = self->ppm >> 8;
	_uplink_payload.bytes[5] = self->ppm & 0xFF;
	_uplink_payload.bytes[6] = self->dB >> 8;
	_uplink_payload.bytes[7] = self->dB & 0xFF;

	return _uplink_payload;

}