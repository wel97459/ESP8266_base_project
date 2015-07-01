#include "mem.h"
#include "c_types.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "espconn.h"
#include "mystuff.h"

#define procTaskPrio        0
#define procTaskQueueLen    1

static volatile os_timer_t some_timer;

os_event_t    procTaskQueue[procTaskQueueLen];
static void ICACHE_FLASH_ATTR
procTask(os_event_t *events)
{
	system_os_post(procTaskPrio, 0, 0 );
	if( events->sig == 0 && events->par == 0 )
	{
		//Idle Event.
	}
}

//Timer event.
static void ICACHE_FLASH_ATTR
 myTimer(void *arg)
{
	uart0_sendStr(".");
}

void ICACHE_FLASH_ATTR at_recvTask()
{
	//Called from UART.
}


void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	int wifiMode = wifi_get_opmode();

	uart0_sendStr("\r\nHello from ESP8266:\r\n");

	printf( "This is a test of the printf funtion in: %s", "mystuff.c");

	//Add a process
	system_os_task(procTask, procTaskPrio, procTaskQueue, procTaskQueueLen);

	//Timer example
	os_timer_disarm(&some_timer);
	os_timer_setfn(&some_timer, (os_timer_func_t *)myTimer, NULL);
	os_timer_arm(&some_timer, 500, 1);

	system_os_post(procTaskPrio, 0, 0 );
}
