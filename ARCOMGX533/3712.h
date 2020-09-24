/*******************************************************************/
/* Fichier entete pour utilisation carte advantech PCM-3712 PC-104 */
/* version : 1.0                                                   */
/* auteurs : RK                                                    */
/*******************************************************************/

#ifndef __3712__
#define __3712__

#include<asm/io.h>

#define BASE_3712 0x300
#define PCM3712_DA0_LSB  BASE_3712
#define PCM3712_DA0_MSB (BASE_3712+1)
#define PCM3712_DA1_LSB (BASE_3712 +2)
#define PCM3712_DA1_MSB (BASE_3712 +3)
#define PCM3712_SYNC 	(BASE_3712 +4)
#define PCM3712_OUT 	(BASE_3712 +5)


int initPCM3712(void);
void SetDA(int channel, u16 value);


#endif









