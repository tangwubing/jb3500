#include <litecore.h>
#include <drivers/tdk6515.h>
#include "system.h"
#include "display.h"
#include "upcomm.h"
#include "meter.h"
#include "para.h"



//Public Variables
volatile uint_t g_sys_status = 3;




//OS thread declares
os_thd_declare(Upcom1, 1280);
os_thd_declare(Upcom2, 1024);
os_thd_declare(Daemon, 1024);
os_thd_declare(Display, 1024);
os_thd_declare(Meter, 1280);


void tsk_Daemon(void *args)
{
	uint_t nTick, nSpan;

    for (nTick = 0; ; nTick++) {
		os_thd_Sleep(100);
		if (g_sys_status & BITMASK(0))
			nSpan = 7;
		else
			nSpan = 3;
		if ((nTick & nSpan) == 0)
			LED_RUN(1);
		if ((nTick & nSpan) == 1)
			LED_RUN(0);
	}
}

void app_Entry()
{
	uint_t i = 1;

	BEEP(0);
	if (i)
		os_thd_Create(Daemon, 160);
	if (i)
		os_thd_Create(Display, 140);
	if (i)
		os_thd_Create(Upcom1, 120);
	if (i)
		os_thd_Create(Upcom2, 100);
	if (i)
		os_thd_Create(Meter, 80);
}

void hold()
{
	uint_t i;

	for (i = 1; i; );
}

int main(void)
{

//	hold();
	sys_Start();
}

