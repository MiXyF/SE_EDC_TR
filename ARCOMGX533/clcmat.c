#include<linux/init.h>
#include<linux/module.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>

MODULE_LICENSE("GPL");
float obscont(float Y[2],float X[4]) //calculs effectués en mV
{	
	float U;	
	//float   X[4][1] 	 = { 0, 0 , 0, 0 } ;
	//float   Y[2][1] 	 = { 0 , 0 } ;



float   Adc[4][4]= 
	{
	     {  630.0 , -120.6 , -0.8 , 8.6 },
	     { -0095.3 ,  0693.5 ,  0010.7 , 0001.2 },
	     { -0.2896 , -1.9184 ,  1.1306 , 0235.1 },
	     { -3968.0 , -1773.3 , -0154.6 , 0722.2 }	 
	    };

float Bdc[4][2] = 	{
		{ 0365.8 , 0120.0 },
		{ 0.0993 , 0307.0 },
		{ 1088.7 , 2014.1 },
		{ 3137.7 , 1659.9 }
	};


float Cdc[4] = 	{ -80309.2,-9623.7 ,-14121.5 ,-23626.0   } ;

float Ddc[2] =  { 0.0 , 0.0 } ;	
	
 
float	res11 =     Adc[0][0]*  X[0] +     Adc[0][1]*  X[1] +     Adc[0][2]*  X[2] +     Adc[0][3]*  X[3]  +    Bdc[0][0]*Y[0] +   Bdc[0][1]*Y[1] ; 

float	res21 =     Adc[1][0]* X[0] +     Adc[1][1]*  X[1] +     Adc[1][2]*  X[2] +     Adc[1][3]*  X[3]  +    Bdc[1][0]*Y[0] +   Bdc[1][1]*Y[1] ; 

float	res31 =     Adc[2][0]*  X[0]+     Adc[2][1]*  X[1]+     Adc[2][2]*  X[2] +     Adc[2][3]*  X[3]  +    Bdc[2][0]*Y[0] +   Bdc[2][1]*Y[1]; 

float	res41 =     Adc[3][0]*  X[0]+     Adc[3][1]*  X[1]+     Adc[3][2]*  X[2] +     Adc[3][3]*  X[3]  +    Bdc[3][0]*Y[0]+   Bdc[3][1]*Y[1] ;

X[0] = res11;
X[1] = res21;
X[2] = res31;
X[3] = res41;	



U = Cdc[0]* X[0] + Cdc[1]* X[1] + Cdc[2]* X[2]  + Cdc[3]* X[3]  ; 

return U;
//printk("u res = %f ", U) ;
}




int affichage_mat(int row, int col, float matrice[row][col]){
	int i,j ;
 	printk(" Mat = \n");
  	for(i=0; i < row; i++)
  	{
    	for(j = 0; j < col; j++)
    	{
    		//print("i =  
      		printk("%f \t",matrice[i][j]);
    	}
    	printk("\n");
  	}
  	printk("\n\n");
  	return 0 ;
}
float convertTomV(u16 ADC_in) // a corriger et compléter
{
	float temp;
	if (ADC_in >= 2058)
	{
		temp = ADC_in * 2,441;	
	}
	else if (ADC_in <= 2038)
	{
		temp = -(ADC_in * 2,441);
	}

	else
	{
		temp = 0;	
	}
printk("converted value in V: %d \r\n",(int)(temp/1000));
	return temp;
}
/**u16 convert4DAC(float Command)
{
	
}**/

EXPORT_SYMBOL(obscont) ; // Rend public les fonction pour les autres modules (.c)
EXPORT_SYMBOL(affichage_mat) ;
EXPORT_SYMBOL(convertTomV);

//module_init(initclc);
//module_exit(exitclc);


