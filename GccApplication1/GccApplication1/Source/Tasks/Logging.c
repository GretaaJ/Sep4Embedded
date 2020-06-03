#include <ATMEGA_FreeRTOS.h>
#include "../Headers/Logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semphr.h"

SemaphoreHandle_t semaphoreMutex;

static char* _FormattingWithDataUInt(uint16_t, char*, char*);
static char* _FormattingWithDataFloat(float, char*, char*);
static char* _FormattingString(char*, char*);
static void _ProtectResources(char*);

void Logging_MeasureInt(char* message, uint16_t data)
{
	char* measureMsg = _FormattingWithDataUInt(data, message, "MEASUREMENT");
	_ProtectResoucres(measureMsg);
	free(measureMsg);
}

void Logging_MeasureFloat(char* message, float data)
{
	char* measureMsg = _FormattingWithDataFloat(data, message, "MEASUREMENT");
	_ProtectResoucres(measureMsg);
	free(measureMsg);
}

void Logging_Info(char* message)
{
	char* infoMsg = _FormattingString("INFO", message);
	_ProtectResoucres(infoMsg);
	free(infoMsg);
}

void Logging_Error(char* message)
{
	char* errorMsg = _FormattingString("ERROR", message);
	_ProtectResoucres(errorMsg);
	free(errorMsg);
}


static char* _FormattingWithDataUInt(uint16_t data, char* message, char* prefix) {

	char* stringBuffer = calloc(sizeof(char), 50);
	sprintf(stringBuffer, "[%s] %s %u", prefix, message, data);
	return stringBuffer;
}

char* _FormattingWithDataFloat(float data, char* message, char* prefix) {

	char* stringBuffer = calloc(sizeof(char), 50);
	sprintf(stringBuffer, "[%s] %s %f", prefix, message, data);
	return stringBuffer;
}

char* _FormattingString(char* prefix, char* message) {

	char* stringBuffer = calloc(sizeof(char), strlen(prefix) + 10);
	sprintf(stringBuffer, "[%s] %s",prefix , message);
	return stringBuffer;
}

void _ProtectResoucres(char* message) {
	if (semaphoreMutex == NULL)
		semaphoreMutex = xSemaphoreCreateMutex();
	
	xSemaphoreTake(semaphoreMutex, portMAX_DELAY);
	puts(message);
	xSemaphoreGive(semaphoreMutex);
}