#include<linux/init.h>
#include<linux/module.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>**/

MODULE_LICENSE("GPL");
float obscont(float Y[2],float X[4])
{
	int i;	
	//float   X[4][1] 	 = { 0, 0 , 0, 0 } ;
	//float   Y[2][1] 	 = { 0 , 0 } ;
	float  U;



float   Adc[4][4]= 
	{
	     {  0.6300 , -0.1206 , -0.0008 , 0.0086 },
	     { -0.0953 ,  0.6935 ,  0.0107 , 0.0012 },
	     { -0.2896 , -1.9184 ,  1.1306 , 0.2351 },
	     { -3.9680 , -1.7733 , -0.1546 , 0.7222 }	 
	    } ;

float Bdc[4][2] = 	{
		{ 0.3658 , 0.1200 },
		{ 0.0993 , 0.3070 },
		{ 1.0887 , 2.0141 },
		{ 3.1377 , 1.6599 }
	};


float Cdc[4] = 	{ -80.3092,-9.6237 ,-14.1215 ,-23.6260   } ;

float Ddc[2] =  { 0.0 , 0.0 } ;	
	
 
float	res11 =     Adc[0][0]*  X[0][0] +     Adc[0][1]*  X[1][0] +     Adc[0][2]*  X[2][0] +     Adc[0][3]*  X[3][0]   +    Bdc[0][0]*Y[0][0] +   Bdc[0][1]*Y[1][0] ; 

float	res21 =     Adc[1][0]*  X[0][0] +     Adc[1][1]*  X[1][0] +     Adc[1][2]*  X[2][0] +     Adc[1][3]*  X[3][0]   +    Bdc[1][0]*Y[0][0] +   Bdc[1][1]*Y[1][0] ; 

float	res31 =     Adc[2][0]*  X[0][0] +     Adc[2][1]*  X[1][0] +     Adc[2][2]*  X[2][0] +     Adc[2][3]*  X[3][0]   +    Bdc[2][0]*Y[0][0] +   Bdc[2][1]*Y[1][0] ; 

float	res41 =     Adc[3][0]*  X[0][0] +     Adc[3][1]*  X[1][0] +     Adc[3][2]*  X[2][0] +     Adc[3][3]*  X[3][0]   +    Bdc[3][0]*Y[0][0] +   Bdc[3][1]*Y[1][0] ;

X[0] = res11;
X[1] = res21;
x[2] = res31;
x[3] = res41;	



U = Cdc[0][0]* X[0][0] + Cdc[1][0]* X[1][0] + Cdc[2][0]* X[2][0]  + Cdc[3][0]* X[3][0]  ; 


printf("u res = %f ", U) ;

	return U;
}




int affichage_mat(int row, int col, float matrice[row][col]){
	int i,j ;
 	printk(" Mat = \n");
  	for(i=0; i < row; i++)
  	{
    	for(j = 0; j < col; j++)
    	{
    		//print("i =  
      		printf("%f \t",matrice[i][j]);
    	}
    	printk("\n");
  	}
  	printk("\n\n");
  	return 0 ;
}


