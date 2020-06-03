#pragma once

#include <stdint.h>

void Logging_MeasureInt(char* message, uint16_t data);
void Logging_MeasureFloat(char* message, float data);
void Logging_Info(char* message);
void Logging_Error(char* message);