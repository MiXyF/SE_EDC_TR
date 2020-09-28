/***********************************************************************/
/* SIL2 squelette tp2 communications CAN  sur ARCOMGX533               */
/* sous RTAI 3.4                                                       */
/* a completer pour ajouter COM et sortie analogique voie 2            */       /* KOCIK R.                                                            */
/***********************************************************************/

#include<linux/init.h>
#include<linux/module.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

#include"3712.h"

/* define pour gestion registres CAN */
#define CTRL 0x180
#define CMD (CTRL+1)
#define STATU (CTRL+2)
#define INTERRUPT (CTRL+3)
#define ACR (CTRL+4)
#define AMR (CTRL+5)
#define BTR0 (CTRL+6)
#define BTR1 (CTRL+7)
#define OCR (CTRL+8)

#define TX_ID (CTRL+10)
#define TX_DLC(CTRL+11)

#define RX_ID (CTRL+20)
#define RX_DLC(CTRL+21)


MODULE_LICENSE("GPL");

/* define pour tache periodique */
#define STACK_SIZE  2000
#define TICK_PERIOD 1000000    //  1 ms
#define PERIODE_CONTROL 20000000 //20ms
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE 1

/* RT_TASK */
static RT_TASK tache_horloge, ma_tache_receive,;
static int temps=0;
static int value = 0;
static int globalID = 7;
static int globalBUFFER = 0;
static int globalDLC = 2;

/* tache generation dent de scie */
void saw(long arg) {

  int i=0;
   while (1) 
   {
    if(i>=4096){
      i=0;}
    else{i=i+100;}
    // On est sur 2 octects
   
    PCM3712setda0(i);
    envoie_can();
    rt_task_wait_period();
 }
}

void reception_can(void)
{
	while(1)
	{
		u8 VAL ;
	    u8 DLC ;
	   	u8 recu[8] ;
	   	u8 data[8];
	   	u16 debID;
	   	u16 finID;
    	u8 addr11;
    	u16 id; 
		
		value=inb(STATU);
		printk(VAL);
		if(VAL == 0x01) //ON verifie si le SR0 est egal a 1 donc STATU = 0000 0001
      	{
//********************************* Modification de l'id ********************************* //
	      	
	      	debID = inb(RX_ID); // On recupere les bits de poids fort pour l adresse 10
	      	addr11 = inb(RX_DLC);
	      	finID = addr11>>5; // On decale de 5b pour conserver que les 3 bits de poids fort
	      	id = (debID <<3) + finID; // Assemblage de l id sur 16 bits 

//********************************* Lecture du DLC  ********************************* //    
	        
	        DLC = (0xOF & inb(RX_DLC);
	        
			for (int i=0, i<= DLC,i++)
	      	{
		      	recu[i] = inb(RX_DLC+i);
		      	printk("Nous recevons la donnee:  %d /n",recu[i]);
		      	
	      	}
	      	
	      	memcpy(data,recu,DLC);
	      	printk("L'identifiant est  %d /n",id);
	      	printk("La taille de la donnee est  %d /n",DLC);
	      	printk("La donnee recu est %d /n",data);
			globalBUFFER = data ; // On ecrit la donne recu dans la zone memoire 
	      	sortie();
	      	
	      	}	
	rt_task_wait_period();
	}
}


void envoie_can(void). //Mettre les variable en miniscule et define en miniscule
{
		   
	   		u8 VAL ;
	    	u8 RTR = 0 ;
	    	u8 data[8] ;
	   		u8 finID ;
		
			int ID = globalID;
			int msg = globalBUFFER;
			int DLC = globalDLC;
		
			VAL=inb(STATU);
			printk(VAL);
			if(VAL == 0x04) //ON verifie si le SR2 est egal a 1 donc STATU = 0000 0100
      		{
//********************************* Modification de l'id *********************************//
	      	
	      		outb(ID >>3,TX_ID); //On recupere les bits de poids fort pour l adresse 10
	   			finID = ID <<8; //On recupere les LSB de id pour les mettre dans l adresse
	   			outb(finID+RTR+DLC,TX_DLC); //Les premiere valeurs sont les 3 dernieres de l id suivi du 															rtr en position 4 et enfin les bits du dlc
	   			memcpy(data,msg,DLC);
	       
	   			for (int i=0, i<= DLC,i++)
	   			{
		   			outb(data[i],TX_DLC+i);
		      		printk("Nous envoyons la donnee:  %d /n",data[i]);
		      	
				}
	     		outb(0x01,CMD)	; //Tranmission request
	     		value = 1; 
	     	}	

}

void sortie(void) {
	
    PCM3712setda1(globalBUFFER);
    rt_task_wait_period();

}




static int tpcan_init(void) {

  int ier,ierr,ierrr, ierrrr;
  RTIME now;


    /* creation tache périodiques*/
   rt_set_oneshot_mode();
    
   
   rt_set_oneshot_mode();
   ier = rt_task_init(&ma_tache_send,envoie_can,0,STACK_SIZE, PRIORITE, 0, 0);
   ierr = rt_task_init(&tache_horloge,saw,0,STACK_SIZE, PRIORITE, 0, 0);
   ierrr = rt_task_init(&ma_tache_receive,reception_can,0,STACK_SIZE, PRIORITE, 0, 0); 
   ierrrr = rt_task_init(&tache_can,sortie,0,STACK_SIZE, PRIORITE, 0, 0); 
   
   start_rt_timer(nano2count(TICK_PERIOD));
   now = rt_get_time();
   
   rt_task_make_periodic(&ma_tache_send, now, nano2count(PERIODE_CONTROL/10));
   rt_task_make_periodic(&ma_tache_receive, now, nano2count(TICK_PERIOD));
   rt_task_make_periodic(&tache_horloge, now, nano2count(PERIODE_CONTROL));
   rt_task_make_periodic(&tache_can, now, nano2count(PERIODE_CONTROL));
 
 
 
 return(0);
}

static void tpcan_exit(void) {
 stop_rt_timer(); 
 rt_task_delete(&ma_tache_send);
  rt_task_delete(&ma_tache_receive);
   rt_task_delete(&tache_horloge);
    rt_task_delete(&tache_can);

}



module_init(tpcan_init);
module_exit(tpcan_exit);

