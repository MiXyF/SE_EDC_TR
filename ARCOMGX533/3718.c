
#include<linux/init.h>
#include<linux/module.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

#include"3718.h"


static int init3718(void)
{
	
	setChannel(0);
	outb(CTR_MODE_NO_INT,CONTROL); //Interrupt enabled for DMA | Interrupt level N/A | DMA enabled | Trigger source : software
	printk("DATA register on init : 0x%x\r\n",inb(DATA_REG));
	//if (inb(STATUS) == STATUS_INIT)
	//{
	//	printk("init complete, MUX on 16 channels mode, ready to convert \r\n");
	//	return 0;
	//}
	//else
	//{
	//	printk("ERROR on init, abort setup\r\n");
	//	return 1;
	//}
	return(0);
}

void setChannel(int in_channel)
{
	outb(MUX_SCAN,in_channel);
	printk("Channel set to n°%d\r\n",in_channel);
}

void ADRangeSelect(int channel, int range)
{
	if ((range > 8) | (range < 0) ) 
	{
		printk("Range value %d not usable \r\n",range);
		return -1;
	}	
	outb(channel,MUX_SCAN);
	outb(range,DATA_REG);
}

u16 ReadAD(void)
{
	u16 output;
	printk("starting AD conversion. Writing in Register BASE \r\n");
	printk("STATUS register before conversion: 0x%x\r\n",(inb(STATUS)));
	outb(0x01,BASE);
	printk("STATUS register during conversion: 0x%x\r\n",(inb(STATUS))); 	
	printk("BASE register : 0x%x\r\n",inb(BASE));
	printk("DATA register : 0x%x\r\n",inb(DATA_REG));
	/**inb(STATUS);
	printk("Conversion successfull. Writing data from register to memory\r\n");
	output =  (inb(DATA_REG) << 8) | (inb(BASE) & 0xff);
	printk("Value from ADC : %d \r\n",output);
	return output;**/
	if ( (inb(STATUS) & 0x70 ) )
	{
		printk("STATUS register : 0x%x\r\n",(inb(STATUS)));
		printk("Conversion successfull. Writing data from register to memory\r\n");
		output =  ((inb(DATA_REG) << 4) | ((inb(BASE) >> 4) & 0x0f));
		printk(" DATA_REG register : 0x%x, BASE register : 0x%x\r\n",inb(DATA_REG),inb(BASE)); 
		printk("Value from ADC : 0x%x \r\n",output);

	return output;
	}
	
	
	else
	{
		printk("Error on AD conversion, check STATUS register 0x%02x \r\n",inb(STATUS));
		return NULL;	
	}
	
}
void exit2()
{}

//u16 convert(u16 adc_raw_output, int range)
//{
//	int sel;
//}
module_init(init3718);
module_exit(exit2);

EXPORT_SYMBOL(ReadAD);
EXPORT_SYMBOL(ADRangeSelect);
EXPORT_SYMBOL(setChannel);

