// Emtpy main.c file generated by the wizard
#include <cpu/irq.h>
#include <cfg/debug.h>
#include <drv/timer.h>
#include <io/lpc23xx.h>

#define STATUS_LED (1<<19)
#define LED_ON()  do { IOCLR1 =  STATUS_LED; } while (0)
#define LED_OFF() do { IOSET1 =  STATUS_LED; } while (0)

static void init(void)
{
	IRQ_ENABLE;
	kdbg_init();
	timer_init();
	proc_init();
	timer_delay(3000);
	kprintf("NXP LPC2378 BeRTOS port test\n");
	timer_delay(3000);
	/* Turn off boot led */
	IODIR0 = (1<<21);	
	IOCLR0 = (1<<21);	
	/* Init status led */
	IODIR1 |= STATUS_LED;
	LED_OFF();
}

static void NORETURN bertos_up(void)
{
	char spinner[] = {'/', '-', '\\', '|'};
	int i = 0;
	while (1)
	{
		i++;
		proc_forbid();
		kprintf("BeRTOS is up & running: %c\r",
			spinner[i % countof(spinner)]);
		proc_permit();
		timer_delay(100);
	}
}

static void NORETURN status(void)
{
	while (1)
	{
		LED_ON();
		timer_delay(250);
		
		LED_OFF();
		timer_delay(250);
	}
}

int main(void)
{

	init();
	proc_testRun();
	proc_new(bertos_up, NULL, KERN_MINSTACKSIZE * 3, NULL);
	proc_new(status, NULL, 0, NULL);
	while (1)
	{
	}

	return 0;
}
