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
#define PERIODE_CONTROL 50000000 //50ms
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE 1

static RT_TASK tache_horloge;
//,tache_can;
//static int temps=0;
u16 ADC_out;
float Y[2] = {0.0, 0.0};
float X[4] = {0.0,0.0,0.0,0.0};


float U;
int temp;
u16 DAC_out;
/* RT_TASK */
/* tache generation dent de scie */
void saw(long arg)
{

			// ON THE PENDULUM 0 = angle 1 = POSITION 

		while(1)
		{
		
		setChannel(SINGLE_CHANNEL_0);
		//rt_busy_sleep(TICK_PERIOD);
		rt_task_wait_period();
		ADC_out = ReadAD();
		//printk("Channel 0 : Acquisition terminée. Début de conversion \n");
		//rt_busy_sleep(TICK_PERIOD);
		Y[0]=convertToRad(ADC_out);
		//printk("Channel 0 : Conversion terminée \n");
		temp = (int)(Y[0] * 1000.0);
		//printk("ADC angle value on channel %d : %d Y[0] = %d \r\n",inb(BASE)&0x0F,ADC_out,temp);
		setChannel(SINGLE_CHANNEL_1);
		rt_task_wait_period();		
		//rt_busy_sleep(TICK_PERIOD);		
		ADC_out = ReadAD();
		//printk("Channel 1 : Acquisition terminée. Début de conversion \n");
		Y[1]=convertToMet(ADC_out);
		//printk("Channel 1 : Conversion terminée \n");
		temp = (int)(Y[1] * 1000.0);
		//printk("ADC position value on channel %d : %d Y[1] = %d \r\n",inb(BASE)&0x0F,ADC_out,temp);
		//rt_busy_sleep(PERIODE_CONTROL);

		U = obscont(Y,X);
		//printk("commande U = %d\n",(int)(U*1000.0));
		//affichage_mat(4,1,X[4]);
		//affichage_mat(2,1,Y[2]);
		//printk("Conversion vers DAC\n\n");		
		DAC_out = convert4DAC(U);
		//printk("DAC_out = %d\n",DAC_out);
		SetDA(SINGLE_CHANNEL_0,DAC_out);	
		rt_task_wait_period();
		}

}


static int tpcan_init(void) {

  int ierr;
  RTIME now;
	printk(" Initializing ADC\r\n");
	printk(" Init successfull, Initializing Channel and range\r\n");
	//setChannel(0x10);	
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
