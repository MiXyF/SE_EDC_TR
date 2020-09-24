/**********************************************************/
/* squelette TP CAN carte 7841 communication CAN PCI      */
/* communication bus can                                  */
/* version : rtai 3.4                                     */
/* auteur: KOCIK R.                                       */
/**********************************************************/

#include<linux/init.h>
#include<linux/module.h>

#include <asm/io.h>
#include <linux/pci.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

MODULE_LICENSE("GPL");

/* define pour gestion tâche périodique */
#define STACK_SIZE  2000
#define TICK_PERIOD 1000000    //  1 ms
#define PERIODE_CONTROL 100000000 //1s
#define PERIODE_CONTROL2 1000000 //1ms
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE 0
#define PRIORITE2 1

/* define pour gestion PCI CARTE CAN 7841 */
#define CAN_VENDOR_ID 0x144A
#define CAN_DEVICE_ID 0x7841

/* define pour gestion registres CAN 7841*/

#define CTRL base
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







/* déclaration variables pour addresse et irq */
/*  de la carte CAN PCI 7841 */    
static u32 base;  //adresse de base SJA1000 1
static u32 base2; //adresse de base sJA1000 2
static unsigned int irq_7841; // IRQ carte CAN

/*déclaration de tâches */
static RT_TASK ma_tache;
static RT_TASK ma_reception;


/*******************************************************/
/* recherche adresse et IRQ de la carte CAN sur le PCI */
/******************************************************/

int init_7841(void)  //Permet de positionner dans base l'aresse du SJA1000 de la carte et dans base 2 @second SJA1000
{
   u16 vendor,device;   
   u8 revision;
 struct pci_dev *dev=NULL;
 
  // RECHERCHE DE L'ADRESSE DE LA CARTE SUR LE BUS PCI
      dev = pci_get_device(CAN_VENDOR_ID, CAN_DEVICE_ID,dev);
      pci_read_config_dword(dev,PCI_BASE_ADDRESS_2,&base);
      if(dev==NULL) 
	{
	  printk("cannot init PCI 7841 card\n");
	  return -1;
	}
 
  base &= PCI_BASE_ADDRESS_IO_MASK; //adresse de base du 1er SJA1000
  base2 = base + 0x80; //adresse de base du 2eme SJA1000
  printk("CARTE 7841 : BASE  0x%4x\n",base);
 
  
  irq_7841 = dev->irq;  //on récupère l'irq de la carte CAN
  
  printk("CARTE 7841 Irq: %d\n", irq_7841);
  pci_read_config_byte(dev,PCI_REVISION_ID,&revision);
  printk("CARTE 7841 Revision : %u\n", revision);
  pci_read_config_word(dev,PCI_VENDOR_ID,&vendor);
  printk("CARTE 7841, VendorId : %x\n", vendor);
  pci_read_config_word(dev,PCI_DEVICE_ID,&device);
  printk("CARTE 7841 DeviceId : %x\n", device);
  printk("CARTE 7841 Base : %x\n", base);
  
  outb(0,base+9);            //Clear FIFO
  
  outb(0,base2+9);           //Clear FIFO du 2eme SJA1000
  
  printk(",CARTE PCI 7841 Init Ok ");
  return(0);  
}

/********************************************************/
/*   Initialisation du/des SJA1000                      */
/********************************************************/
void init_can(void) 
{
 outb(0x01,CTRL); // on passe en mode reset pour pouvoir ecrire sur les registres
 outb(0xFF,ACR); // on met tout les bits a 1 : aucun des bits utilise pour le filtrage
 outb(0xFF,AMR); // on laisse tout les bit passee
 outb(0x03,BTR0); 
 outb(0x1C,BTR1); 
 outb(0xFA,OCR);


 outb(0x00,CTRL); // on repasse en mode operating + Positionnement des interuptions en disable a modifier pour activer interuption

 
}

/************************************************/
/* squelette Tache périodique                   */
/************************************************/
void tache_periodique(long id)
{
  while(1)
    {
	    
		int DLC;
		char buffer[] = "hello world";
		
		DLC = sizeof(buffer);
	    envoie_can(id,DLC,buffer);
	    
	    
     rt_task_wait_period();
    }
}

void tache_periodique2()
{
  while(1)
    {
		reception_can();

    }
 }

void envoie_can(int id, int DLC, void *msg). //Mettre les variable en miniscule et define en miniscule
	   {
		   
	    u8 value ;
	    u8 rtr = 0;
	    u8 data[8] ;
	    u8 finID;
		
		value=inb(STATU);
		printk(value);
      	if(value == 0x04) //ON verifie si le SR2 est egal a 1 donc STATU = 0000 0100
      	      	{
	      	//********************************* Modification de l'id ******************************************************************//
	      	
	      	outb(id>>3,TX_ID); // On recupere les bits de poids fort pour l adresse 10
	      	finID = id<<8; // On recupere les bit de point faible de id pour les mettre dans l adresse
			outb(finID+rtr+DLC,TX_DLC); // Les premiere valeurs sont les 3 dernieres de l id suivi du rtr en position 4 et enfin les bits du dlc
	        memcpy(data,msg,DLC);
	     
	      	for (int i=0, i<= DLC,i++)
	      		{
		      		outb(data[i],TX_DLC+i);
		      		printk("Nous envoyons la donnee:  %d /n",data[i]);
		      	
	      		}
	     outb(0x01,CMD)	; //Tranmission request
	      
	      
      	}


}

void reception_can()

     {
	    u8 value ;
	    u8 DLC ;
	    u8 recu[8] ;
	    u8 data[8];
	    u16 debID;
	    u16 finID;
	    u8 addr11
	    u16 id; 
		
		value=inb(STATU);
		printk(value);
      	if(value == 0x01) //ON verifie si le SR0 est egal a 1 donc STATU = 0000 0001
      	  {
	    //********************************* Modification de l'id ******************************************************************//
	      	
	      	debID = inb(RX_ID); // On recupere les bits de poids fort pour l adresse 10
	      	addr11 = inb(RX_DLC);
	      	finID = addr11>>5; // On decale de 5b pour conserver que les 3 bits de poids fort
	      	id = (debID <<3) + finID; // Assemblage de l id sur 16 bits 
	      	
	      	
	    //********************************* Lecture du DLC  ******************************************************************//    
	        
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
	      	
	      		
	      		
	      		
	        }	
	      
	  }
	      		
	      		
	      		
	      		
	      		

int caninit(void) {

  int ierr, ierrr;
  RTIME now;

   /* init can  */
   ierr=init_7841(); //metre en com pour la carte arcom
   init_can();

   /* creation taches périodiques d'envoie */
   rt_set_oneshot_mode();
   ierr = rt_task_init(&ma_tache,tache_periodique(7),0,STACK_SIZE, PRIORITE, 0, 0); 
   start_rt_timer(nano2count(TICK_PERIOD));
   now = rt_get_time();
   rt_task_make_periodic(&ma_tache, now, nano2count(PERIODE_CONTROL));
 
  /* creation taches périodiques de reception */
   ierrr = rt_task_init(&ma_reception,tache_periodique2,0,STACK_SIZE, PRIORITE2, 0, 0); 
   rt_task_make_periodic(&ma_reception, now, nano2count(PERIODE_CONTROL2));
 
 return(0);
 
}

void canexit(void) {
  printk("uninstall 7841 CAN PCI driver\n");
  stop_rt_timer(); 
  rt_task_delete(&ma_tache);
  rt_task_delete(&ma_reception);
}

module_init(caninit);
module_exit(canexit);
