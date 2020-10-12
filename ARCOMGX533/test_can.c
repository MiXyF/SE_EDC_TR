#include<linux/init.h>
#include<linux/module.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

#include "3718.h"
#include "3712.h"
#include "clcmat.h"

MODULE_LICENSE("GPL");

/* define pour tache periodique */
#define STACK_SIZE  2000
#define TICK_PERIOD 1000000    //  1 ms
#define PERIODE_CONTROL 100000000 //20ms
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE 1

static RT_TASK tache_horloge;
//,tache_can;
//static int temps=0;
u16 ADC_out;
float Y[2]; 
float X[4];
float U;
int temp;
/* RT_TASK */
/* tache generation dent de scie */
void saw(long arg)
{
		while(1)
		{
		setChannel(SINGLE_CHANNEL_0);
		ADC_out = ReadAD();
		Y[0]=convertTomV(ADC_out);
		//Y[0] = (float)ADC_out;
		temp = (int)Y[0];
		printk("ADC value on channel 0 : %d Y[0] = %d \r\n",ADC_out,temp);
		rt_task_wait_period();

		setChannel(SINGLE_CHANNEL_1);
		ADC_out = ReadAD();
		Y[1]=convertTomV(ADC_out);
		temp = (int)Y[1];
		printk("ADC value on channel 1 : %d Y[1] = %d \r\n",ADC_out,temp);
		
		U = obscont(Y,X);
		printk("U = %d \r\n",(int)U);
		//affichage_mat(4,1,X[4]);
		//affichage_mat(2,1,Y[2]);
		//SetDA(SINGLE_CHANNEL_1,U);	
		rt_task_wait_period();
		}

}


static int tpcan_init(void) {

  int ierr;
  RTIME now;
	printk("Initializing ADC\r\n");
	printk("Init successfull, Initializing Channel and range\r\n");	
    /* creation tache périodiques*/
   rt_set_oneshot_mode();
   ierr = rt_task_init(&tache_horloge,saw,0,STACK_SIZE, PRIORITE, 1, 0);  // avant dernier paramètre à un pour initialiser les calculs en virgules flottante
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
