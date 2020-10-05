/***********************************************************************/
/* Carte d'acquisition DAC (advantech PCM-3712) PC-104.  	           */
/* sous RTAI 3.4                                                       */
/* auteurs : JF et SY									               */    
/***********************************************************************/

/**#include<linux/init.h>
#include<linux/module.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>**/

//MODULE_LICENSE("GPL");

/**float Adc[4][4] ;
float Bdc[4]    ;
int   Cdc[4][4] ;
int   Ddc[2]    ;*/

float t_echant ;
typedef struct
{
	float A[4][4] 	  ;
	float B[4][2]    	  ;
	float   C[4] 	  ;
	float   D[2]    	  ;
	int   Theta   	  ;		//Acquisition Angle
	int   pos     	  ;		//Acquisition position chariot
	int   X[4][1] 	  ;	
	int   Y[2][1]	  ;
	int   U;
	int   ThetaChannel;
	int   PosChannel  ;
	int   UChannel	  ;

}data_struct;





