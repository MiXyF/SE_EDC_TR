#include<linux/init.h>
#include<linux/module.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

#include"3718.h"

MODULE_LICENSE("GPL");

/* define pour tache periodique */
#define STACK_SIZE  2000
#define TICK_PERIOD 1000000    //  1 ms
#define PERIODE_CONTROL 20000000 //20ms
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE 1

static RT_TASK tache_horloge,tache_can;
static int temps=0;
u16 ADC_out;
int temp;
/* RT_TASK */
/* tache generation dent de scie */
void saw(long arg)
{

	ADC_out = ReadAD();
	printk("ADC value : %d \r\n",ADC_out);
	rt_task_wait_period();


 
}


static int tpcan_init(void) {

  int ierr;
  RTIME now;
	printk("Initializing ADC\r\n");
	printk("Init successfull, Initializing Channel and range\r\n");
	setChannel(1);
	ADRangeSelect(1,1);
    /* creation tache périodiques*/
   rt_set_oneshot_mode();
   ierr = rt_task_init(&tache_horloge,saw,0,STACK_SIZE, PRIORITE, 0, 0);  
  start_rt_timer(nano2count(TICK_PERIOD));
  now = rt_get_time();
 
  rt_task_make_periodic(&tache_horloge, now, nano2count(PERIODE_CONTROL));
 
 
 
 	return(0);
}


static void tpcan_exit(void) {
 stop_rt_timer(); 
 rt_task_delete(&tache_horloge);

}



module_init(tpcan_init);
module_exit(tpcan_exit);
