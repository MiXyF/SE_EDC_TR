/***********************************************************************/
/* SIL2 squelette tp2 communications CAN  sur ARCOMGX533               */
/* sous RTAI 3.4                                                       */
/* a completer pour ajouter COM et sortie analogique voie 2            */       /* KOCIK R.                                                            */
/***********************************************************************/

#include<linux/init.h>
#include<linux/module.h>
#include <stdio.h>
#include <errno.h>
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



/* RT_TASK */
static RT_TASK tache_horloge,tache_can;
static int temps=0;

/* tache generation dent de scie */
void saw(long arg) {

  int i=0;
   while (1) 
   {
    if(i>=4096){
      i=0;}
    else{i=i+100;}
   
    PCM3712setda0(i);
    rt_task_wait_period();
 }
}


static int tpcan_init(void) {

  int ierr;
  RTIME now;


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

static int init3718(void)
{
	
	setChannel(0);
	outb(CTR_MODE_NO_INT,CONTROL); //Interrupt enabled for DMA | Interrupt level N/A | DMA enabled | Trigger source : software
	if (inb(STATUS) == STATUS_INIT)
	{
		printk("init complete, MUX on 16 channels mode, ready to convert \r\n");
		return 0;
	}
	else
	{
		printk("ERROR on init, abort setup\r\n")
		exit(EXIT_FAILURE);
	}
}

static void setChannel(int in_channel)
{
	outb(MUX_SCAN,in_channel);
	printk("Channel set to n°%d\r\n",in_channel);
}

static void ADRangeSelect(int channel, int range)
{
	if (range >= 8 | range <= 0 ) 
	{
		printk("Range value %d not usable \r\n",range);
		exit(EXIT_FAILURE);
	}	
	outb(channel,MUX_SCAN);
	outb(range,DATA_REG);
}

static u16 ReadAD(void)
{
	u16 output;
	printk("starting AD conversion. Writing in Register BASE \r\n");
	outb(0,BASE);
	if ( (inb(STATUS) & (1 << 4) ) == 1)
	{
		printk("Conversion successfull. Writing data from register to memory\r\n");
		output =  (inb(DATA_REG) << 8) | (inb(BASE) & 0xff);
		printk("Value from ADC : %d \r\n",ouput);
		return ouput;
	}
	else
	{
		printk("Error on AD conversion, check STATUS register %x \r\n",inb(STATUS);
	}
	
}
module_init(tpcan_init);
module_exit(tpcan_exit);

