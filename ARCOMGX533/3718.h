//Librairie de l'ADC 3718//
#ifndef __3718__
#define	__3718__
#include<asm/io.h>

#define BASE					0x320	//Write any value to trigger conversion in 4 first bits
#define DATA_REG			BASE+1	//Data register with BASE 4 High bits
#define MUX_SCAN			BASE+2	//Multiplexer channel selection
#define DIO 					BASE+3	//Digital I/O registers
#define STATUS				BASE+8	// Information on ADC status. If write any, cleans. p30 doc
#define CONTROL				BASE+9	//Control Register
#define CONTROL_EN		BASE+10
#define DIO_HIGH_BYTE	BASE+11
#define CTR_0					BASE+12
#define CTR_1					BASE+13
#define CTR_2					BASE+14
#define CTR_CTRL			BASE+15

#define CTR_MODE_NO_INT 		0
#define CTR_MODE_INT_DMA		0x10000100
#define STATUS_INIT					0x00100000
#define	RANGE_5_NEG_PLS			0
#define SINGLE_CHANNEL			0
#define MUX_CHANNEL0TO1			0x00010000

#define	RANGE_2_5_NEG_PLS		1
#define RANGE_1_25_NEG_PLS	2
#define RANGE_0_625_NEG_PLS	3
#define RANGE_0_TO_10				4
#define RANGE_0_TO_5				5
#define RANGE_0_TO_2_5			6
#define RANGE_0_TO_1_25			7
#define RANGE_10_NEG_PLS		8

int init3718(void);
void setChannel(int in_channel);
void ADRangeSelect(int channel, int range);
u16 ReadAD(void);

#endif
