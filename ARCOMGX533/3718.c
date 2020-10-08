
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
	
	//setChannel(1);
	outb(CTR_MODE_NO_INT,CONTROL); //Interrupt enabled for DMA | Interrupt level N/A | DMA enabled | Trigger source : software
	ADRangeSelect(SINGLE_CHANNEL_1,RANGE_10_NEG_PLS);
	ADRangeSelect(SINGLE_CHANNEL_0,RANGE_10_NEG_PLS);
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
	outb(in_channel,MUX_SCAN);
	printk("Channel set to n°%d\r\n",in_channel);
}

void ADRangeSelect(u8 channel, int range)
{
	u8 start;
	u8 end;
	u8 transitoire;
	int i ;
	
	//if ((range > 8) | (range < 0) ) 
	//{
	//	printk("Range value %d not usable \r\n",range);
		//return -1; C'est un void on est pas sencé retourné une  valeur ou changer le type de prototypage
	//}

	
	start = channel & 0x0F ; //Collect the start channel
	end = channel >> 4 ; // Collect the end channel
	printk("\n Start channel is n°%d\r\n",start);
	printk("End channel is n°%d\r\n \n",end);
	if(start != end)
	{
		if(start > end)
		{
			transitoire = start ;
			start = end ;
			end = transitoire ;
			printk("\n start et end sont différent ") ;
		}	

		for (i = start ; i <= end ; i++) // Set the range for all channel used or for just 1 if start = end
		{
			printk("i = %d\r\n",i);
			outb(i,MUX_SCAN);
			outb(range,DATA_REG);
		}
	//setChannel(channel) ; 
	
	}
	
	else 
	{
	setChannel(channel) ;
	printk(" \n start et end sont identique") ;
	outb(range,DATA_REG);
	}
	

	// Set back the original channel
	//outb(channel,MUX_SCAN);
	//outb(range,DATA_REG);
}

u16 ReadAD(void)
{
	u16 output;
	float ouputF; 
	printk("starting AD conversion. Writing in Register BASE \r\n");
	//printk("STATUS register before conversion: 0x%x\r\n",(inb(STATUS)));
	outb(0x01,BASE);
	//printk("STATUS register during conversion: 0x%x\r\n",(inb(STATUS))); 	
	//printk("BASE register : 0x%x\r\n",inb(BASE));
	//printk("DATA register : 0x%x\r\n",inb(DATA_REG));
	/**inb(STATUS);
	printk("Conversion successfull. Writing data from register to memory\r\n");
	output =  (inb(DATA_REG) << 8) | (inb(BASE) & 0xff);
	printk("Value from ADC : %d \r\n",output);
	return output;**/
	if ( (inb(STATUS) & 0x7f ) )
	{
		//printk("Acquisition ready, status register ready\r\n");
		//printk("BASE register : 0x%x read register : %d\r\n", inb(BASE), (inb(BASE) %2));
		output =  ((inb(DATA_REG) << 4) | ((inb(BASE) >> 4) & 0x0f));
		//printk("hex value from adc : 0x%x \r\n",output);
		switch (inb(STATUS) %2 )
		{
			case 1:
				printk("Value from ADC : %d from channel n°1 \r\n",output);	
			break;
			
			case 0:
				printk("Value from ADC : %d from channel n°0 \r\n",output);	
			break;
		}
		
		//printk("STATUS register : 0x%x\r\n",(inb(STATUS)));
		//printk("Conversion successfull. Writing data from register to memory\r\n");
		
		//printk(" DATA_REG register : 0x%x, BASE register : 0x%x\r\n",inb(DATA_REG),inb(BASE)); 
		//printk("Value from ADC : 0x%x \r\n",output);
	ouputF = output;
	rt_printk("ouputF = %x \r\n",ouputF);
	return output;
	}
	
	
	else
	{
		printk("Error on AD conversion, check STATUS register 0x%02x \r\n",inb(STATUS));
		return -1 ;	
	}
	
}
static void exit2(void)
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

