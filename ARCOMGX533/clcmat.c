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

float   Adc[4][4]= 
	{
	     {  0.6300 , -0.1206 , -0.0008 , 0.0086 },
	     { -0.0953 ,  0.6935 ,  0.0107 , 0.0012 },
	     { -0.2896 , -1.9184 ,  1.1306 , 0.2351 },
	     { -3.9680 , -1.7733 , -0.1546 , 0.7222 }	 
	};

float Bdc[4][2] = 
	{
		{ 0.3658 , 0.1200 },
		{ 0.0993 , 0.3070 },
		{ 1.0887 , 2.0141 },
		{ 3.1377 , 1.6599 }
	};


float Cdc[4] = 	{ 80.3092,-9.6237 ,-14.1215 ,23.6260   } ;

float Ddc[2] =  { 0.0 , 0.0 } ;	
	
 	//printk("\n Valeur de X avant operation X[0] = %d,X[1] = %d , X[2] = %d , X[3] =%d \n", (int)(X[0]),(int)(X[1]),(int)(X[2]),(int)(X[3])) ;
float	res11 =     Adc[0][0]*  X[0] +    Adc[0][1]*  X[1] +     Adc[0][2]*  X[2] +     Adc[0][3]*  X[3]  +    Bdc[0][0]*Y[0] +   Bdc[0][1]*Y[1] ; 

float	res21 =     Adc[1][0]*  X[0] +    Adc[1][1]*  X[1] +     Adc[1][2]*  X[2] +     Adc[1][3]*  X[3]  +    Bdc[1][0]*Y[0] +   Bdc[1][1]*Y[1] ; 

float	res31 =     Adc[2][0]*  X[0] +    Adc[2][1]*  X[1] +     Adc[2][2]*  X[2] +     Adc[2][3]*  X[3]  +    Bdc[2][0]*Y[0] +   Bdc[2][1]*Y[1] ; 

float	res41 =     Adc[3][0]*  X[0] +    Adc[3][1]*  X[1] +     Adc[3][2]*  X[2] +     Adc[3][3]*  X[3]  +    Bdc[3][0]*Y[0] +   Bdc[3][1]*Y[1] ;

X[0] = res11;
X[1] = res21;
X[2] = res31;
X[3] = res41;	

//printk("x res = %d,%d,%d,%d", res11,res21,res31,res41) ;

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
	float outputmV;
	if (ADC_in >= 2060)
	{
		outputmV = (ADC_in * 4.8828125) - 10000.0;	
	}
	else if (ADC_in <= 2030)
	{
		outputmV = (-1)*(10000.0 - ADC_in * 4.8828125);
	}

	else
	{
		outputmV = 0;	
	}
printk("converted value : %d mV \r\n",(int)(outputmV));
	return outputmV;
}


float convertToMet(u16 ADC_in) {
	float outputMet ;
	
	if (ADC_in >= 2060)
	{
 	outputMet= ((float)ADC_in)/4095.0f ; // Etendu position = 0.5*2 Etendu en tension = 4095

	}
	else if (ADC_in <= 2030)
	{
	outputMet = ((float)ADC_in)/4095.0f ; // Etendu position = 0.5*2 Etendu en tension = 4095
	}
	else {
	outputMet = 0 ;
	}
	printk("Position en metre: %d \r\n",(int)(outputMet*1000.0));
	return outputMet ; 
}

float convertToRad(u16 ADC_in) {
	float outputRad ; 
	if (ADC_in >= 2060)
	{
 	outputRad= ((float)ADC_in)*0.6/4095.0 ; // Etendu position = 0.5*2 Etendu en tension = 4095
	}
	else if (ADC_in <= 2030)
	{
	outputRad = ((float)ADC_in)*(-0.6)/4095.0 ; // Etendu position = 0.5*2 Etendu en tension = 4095
	}
	else {
	outputRad = 0 ;
	}
	printk("Angle en radian %d \r\n",(int)(outputRad*1000.0));
	return outputRad ; // Etendu en radian = 0.3*2  Etendu en tension = 4095	
  
}

/**u16 convert4DAC(float Command)
{
	
}**/

EXPORT_SYMBOL(obscont) ; // Rend public les fonction pour les autres modules (.c)
EXPORT_SYMBOL(affichage_mat) ;
EXPORT_SYMBOL(convertTomV);
EXPORT_SYMBOL(convertToRad);
EXPORT_SYMBOL(convertToMet);

//module_init(initclc);
//module_exit(exitclc);


