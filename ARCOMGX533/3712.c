/***********************************************************************/
/* Carte d'acquisition DAC (advantech PCM-3712) PC-104.  	           */
/* sous RTAI 3.4                                                       */
/* auteurs : JF et SY									               */    
/***********************************************************************/

#include<linux/init.h>
#include<linux/module.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

#include"3712.h"



MODULE_LICENSE("GPL");

#define BASE_3712 0x300
#define PCM3712_DA0_LSB  BASE_3712
#define PCM3712_DA0_MSB (BASE_3712+1)
#define PCM3712_DA1_LSB (BASE_3712 +2)
#define PCM3712_DA1_MSB (BASE_3712 +3)
#define PCM3712_SYNC 	(BASE_3712 +4)
#define PCM3712_OUT 	(BASE_3712 +5)



int initPCM3712(void)
{
	outb(0x80,PCM3712_OUT); // Set bit 7 (ZD) of Output Control to 1 to enable output
	return 0 ;
}
	
	
void SetDA(int channel, u16 value)
{
	u8 value_lsb ;
	u8 value_msb ;

	//printk("valeur d'entrée de setDA : %d\r\n",value);
	value_msb = value/256 ;
	value_lsb = value % 256 ;

	//printk("MSB= %d LSB = %d \r\n",value_msb,value_lsb); 
	// Value sur 12 bits or int sur 16 bits
	if (channel == 0){
		outb(value_lsb,PCM3712_DA0_LSB); // DEMANDER DECCALAGE OU APPLIQUER UN MASQuE
		outb(value_msb,PCM3712_DA0_MSB);
		}
	if (channel == 1) {
		outb(value_lsb,PCM3712_DA1_LSB);
		outb(value_msb,PCM3712_DA1_MSB);
		}
}




static void exit(void) {

}
  


module_init(initPCM3712);
module_exit(exit);
EXPORT_SYMBOL(initPCM3712) ; // Rend public les fonction pour les autres modules (.c)
EXPORT_SYMBOL(SetDA) ;
