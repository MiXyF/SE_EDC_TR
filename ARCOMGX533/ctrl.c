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

MODULE_LICENSE("GPL");

float A_obs_d[4][4] ;
float B_obs_d[4]    ;
int   C_obs_d[4][4] ;
int   D_obs_d[2]    ;

Adc= {
	     {  0.6300 , -0.1206 , -0.0008 , 0.0086 },
	     { -0.0953 ,  0.6935 ,  0.0107 , 0.0012 },
	     { -0.2896 , -1.9184 ,  1.1306 , 0.2351 },
	     { -3.9680 , -1.7733 , -0.1546 , 0.7222 }	 
    } ;

Bdc = 	{
		{ 0.3658 , 0.1200 },
		{ 0.0993 , 0.3070 },
		{ 1.0887 , 2.0141 },
		{ 3.1377 , 1.6599 }
	};


Cdc = 	{ -80.3092,-9.6237 ,-14.1215 ,-23.6260   } ;


D_obs_d =  { 0 ; 0 } ;
