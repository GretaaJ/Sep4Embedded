/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <hal_defs.h>
#include <ihal.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>
#include <queue.h>
#include "Source/Headers/rcServo.h"
#include "mh_z19.h"
#include "rcServo.h"
#include "Source/Headers/CO2.h"
// Needed for LoRaWAN
#include <lora_driver.h>
#include <hih8120.h>

QueueHandle_t xQueue;



// define two Tasks
void task1( void *pvParameters );
void task2( void *pvParameters );
void CO2Sensor(void *pvParameters);
void T_HSensor(void *pvParameters);
void ControllerTask(void *pvParameters);

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;



// Prototype for LoRaWAN handler
void lora_handler_create(UBaseType_t lora_handler_task_priority, QueueHandle_t xQueue);

/*-----------------------------------------------------------*/



void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
	
	xQueue = xQueueCreate(10, sizeof(lora_payload_t));

	
	xTaskCreate(
	CO2Sensor
	, "CO2 sensor"
	, configMINIMAL_STACK_SIZE
	, xTestSemaphore
	, 3
	, NULL);
	
	xTaskCreate(
	T_HSensor
	, "TH sensor"
	, configMINIMAL_STACK_SIZE
	, xTestSemaphore
	, 3
	, NULL);
	
	xTaskCreate(rcServoTask,
	"RC Servo task",
	configMINIMAL_STACK_SIZE,
	NULL,
	3,
	NULL);
	
	xTaskCreate(ControllerTask,
	"Controller task",
	configMINIMAL_STACK_SIZE,
	xQueue,
	3,
	NULL);
}

/*-----------------------------------------------------------*/
void task1( void *pvParameters )
{
	#if (configUSE_APPLICATION_TASK_TAG == 1)
	// Set task no to be used for tracing with R2R-Network
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	#endif

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		//puts("Task1"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		PORTA ^= _BV(PA0);
	}
}

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
void initialiseSystem()
{
	
	rcServoCreate();
	
	mh_z19_create(ser_USART3, &my_co2_call_back);
	
	if ( HIH8120_OK != hih8120Create() )
	{
		puts("driver not created OK");
		return 1;
	}
	
	
	
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);
	// Initialise the trace-driver to be used together with the R2R-Network
	trace_init();
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdioCreate(ser_USART0 );
	// Let's create some tasks
	create_tasks_and_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Initialise the HAL layer and use 5 for LED driver priority
	hal_create(5);
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_create(LORA_USART, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_create(3, xQueue);
	
	
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}


