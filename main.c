/** ****************************************************************************************
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*        _____   ____
*       |  ___| |  _ \
*       | |     | |_| |
*       |  _|   |  _ /
*       | |     | |_| |
*   www.|_|     |_____/-labs.blogspot.it, https://github.com/filippo-bilardo 
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* \mainpage STML476VG Framework
*
* @brief FRAMEWORK per l'utilizzo del microcontrollore ARM Cortex-M4 presente nelle
* schede STML476VG Discovery e supporto al kernel FreeRTOS
* 
* @author Filippo Bilardo
* @date 09/10/21 
* @version 1.0 09/10/21 Versione iniziale 
*
* @section LICENSE
* Open Source Licensing 
* This program is free software: you can redistribute it and/or modify it under the terms 
* of the GNU General Public License as published by the Free Software Foundation, either 
* version 3 of the License, or (at your option) any later version.
* This program is distributed in the hope that it will be useful,but WITHOUT ANY WARRANTY; 
* withouteven the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
* You should have received a copy of the GNU General Public License along with this
* program. If not, see(http://www.gnu.org/licenses/).
*/
//------------------------------------------------------------------------------------------
//=== INCLUDES =============================================================================
//------------------------------------------------------------------------------------------
#include "FreeRTOSConfig.h"             // ARM.FreeRTOS::RTOS:Config
#include "FreeRTOS.h"   
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
//#include "queue.h"                    // ARM.FreeRTOS::RTOS:Core
#include <stdio.h>
#include "Led.h"
#include "Puls.h"
#include "clock.h"
#include "Arduino.h"
#include "event_groups.h" 

#define EV_OBJECT_DETECTED 1 << 0 //bit
EventGroupHandle_t xEventGroup;
void vTaskLedRossoToggle(void * pvParameters);  //buzzer
void vTaskPA0(void * pvParameters);  //test eventgroup ultrasuoni 

int main(void) {
		
	pllInit(0,10,0);
	SwInit();
	LedInit();

	xTaskCreate(vTaskLedRossoToggle, /* Pointer to the function that implements the task.              */
		"Task 1 (Toggle LED rosso)",    /* Text name for the task.  This is to facilitate debugging only. */
		50,   /* Stack depth in words.                */
		NULL,  /* We are not using the task parameter. */
		1,     /* This task will run at priority 1.    */
		NULL); /* We are not using the task handle.    */
	
	xTaskCreate(vTaskPA0, /* Pointer to the function that implements the task.              */
		"Task 2 (PA0 check)",    /* Text name for the task.  This is to facilitate debugging only. */
		50,   /* Stack depth in words.                */
		NULL,  /* We are not using the task parameter. */
		1,     /* This task will run at priority 1.    */
		NULL); /* We are not using the task handle.    */
	
	xEventGroup=xEventGroupCreate();
	vTaskStartScheduler();	
}

void vTaskLedRossoToggle(void * pvParameters){
	const EventBits_t xBitsToWaitFor = EV_OBJECT_DETECTED;
	while(1){
		xEventGroupWaitBits(xEventGroup,xBitsToWaitFor,pdTRUE,pdTRUE,portMAX_DELAY);
		LedRossoToggle();
	}
}

void vTaskPA0(void * pvParameters){
	while(1){
		while(!digitalRead(PA0));
		while(digitalRead(PA0)){;}
			xEventGroupSetBits(xEventGroup,EV_OBJECT_DETECTED);
			
	}	
}
