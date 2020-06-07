#pragma once

#include <lora_driver.h>

typedef struct DataPacketHandler DataPacketHandler;
typedef struct DataPacketHandler* DataPacketHandler_t;

DataPacketHandler_t dph_Create();
void dph_SetCO2 (uint16_t CO2ppm, DataPacketHandler_t self);
void dph_SetTemperature (uint16_t temperature, DataPacketHandler_t self);
void dph_SetHumidity (uint16_t humidity, DataPacketHandler_t self);
void dph_SetDb (uint16_t dB, DataPacketHandler_t self);
lora_payload_t dph_Assemble(DataPacketHandler_t self);