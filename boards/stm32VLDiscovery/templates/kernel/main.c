#include <cpu/irq.h>
#include "hw/hw_led.h"
#include <drv/timer.h>

#include <kern/proc.h>
#include <kern/monitor.h>

static void init(void)
{
	IRQ_ENABLE;

        /* Initialize system timer */
        timer_init();
        /* Initialize LED driver */
        LED_INIT();

        /*
         * Kernel initialization: processes (allow to create and dispatch
         * processes using proc_new()).
         */
        proc_init();

	// insert initialization calls here
}

static void NORETURN led_process(void)
{
        int i;

        /* Periodically blink the led (toggle each 100 ms) */
        for (i = 0; ; i = !i)
        {
                if (i)
                        LED_ON();
                else
                        LED_OFF();
                timer_delay(500);
        }
}


int main(void)
{
#if 1
	int i = 5;
        /* Hardware initialization */
        init();
	
	while(i--){
		LED_ON();
		timer_delay(100);
		LED_OFF();
		timer_delay(100);
	}

        /* Create a new child process */
        //proc_new(led_process, NULL, KERN_MINSTACKSIZE * 2, NULL);
	
	proc_new(led_process, NULL, 0x100, 0x20000200);
	

	while (1)
        {
                monitor_report();
                timer_delay(1000);
        }



        /*
         * The main process is kept to periodically report the stack
         * utilization of all the processes (1 probe per second).
         */
        while (1){
                //monitor_report();
		LED_ON();
                timer_delay(1000);
		LED_OFF();
		timer_delay(1000);
        }

#else
	int i;
	
	init();
	while(1){
		LED_ON();
		i=0x30000;
		while(i--);
		LED_OFF();
		i=0x80000;
		while(i--);	
	}
#endif
}
