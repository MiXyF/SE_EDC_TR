#include <stdio.h>
#include <stdlib.h>


float obscont(float Y[2],float X[4]) 
{	
	float U;	

float   Adc[4][4]= 
	{
	     {  0.6300 , -0.1206 , -0.0008 , 0.0086 },
	     { -0.09530 ,  .6935 ,  0.0107 , 0.0012 },
	     { -0.28960 , -1.9184 ,  1.1306 , 0.2351 },
	     { -3.9680 , -1.7733 , -0.1546 , 0.7222 }	 
	};

float Bdc[4][2] = 
	{
		{ .3658 , 0.1200 },
		{ 0.993 , 0.3070 },
		{ 1.0887 , 2.0141 },
		{ 3.1377 , 1.6599 }
	};


float Cdc[4] = 	{ 80.3092,-9.6237 ,-14.1215 ,23.6260   } ;
	
 	//printk("\n Valeur de X avant operation X[0] = %d,X[1] = %d , X[2] = %d , X[3] =%d \n", (int)(X[0]),(int)(X[1]),(int)(X[2]),(int)(X[3])) ;
float	res11 =     (Adc[0][0]*  X[0] +    Adc[0][1]*  X[1] +     Adc[0][2]*  X[2] +     Adc[0][3]*  X[3])  +    Bdc[0][0]*Y[0] +   Bdc[1][0]*Y[1] ; 

float	res21 =     (Adc[1][0]*  X[0] +    Adc[1][1]*  X[1] +     Adc[1][2]*  X[2] +     Adc[1][3]*  X[3])  +    Bdc[0][1]*Y[0] +   Bdc[1][1]*Y[1] ; 
 
float	res31 =     (Adc[2][0]*  X[0] +    Adc[2][1]*  X[1] +     Adc[2][2]*  X[2] +     Adc[2][3]*  X[3])  +    Bdc[0][2]*Y[0] +   Bdc[1][2]*Y[1] ; 

float	res41 =     (Adc[3][0]*  X[0] +    Adc[3][1]*  X[1] +     Adc[3][2]*  X[2] +     Adc[3][3]*  X[3])  +    Bdc[0][3]*Y[0] +   Bdc[1][3]*Y[1] ;

X[0] = res11;
X[1] = res21;
X[2] = res31;
X[3] = res41;	

printf("x res = %d,%d,%d,%d\n", res11,res21,res31,res41) ;

U = Cdc[0]* X[0] + Cdc[1]* X[1] + Cdc[2]* X[2]  + Cdc[3]* X[3] ; 

return U;
//printk("u res = %f ", U) ;
}

float convertToRad(unsigned int ADC_in) {
	float outputRad ; 
	if (ADC_in >= 2060)
	{
 	outputRad= ((float)ADC_in)*0.15/2048.0 ;
	}
	else if (ADC_in <= 2030)
	{
	outputRad = (4095.0f-(float)ADC_in)*(-0.15)/2048.0 ; 
	}
	else {
	outputRad = 0 ;
	}
	printf("Angle en radian int %d \r\n",(int)(outputRad*1000.0));
	printf("Angle en radian float %f \r\n",(outputRad));
	return outputRad ; // Etendu en radian = 0.3*2  Etendu en tension = 4095	
  
}


int main()
{
	
	float Y[2] = {0.0, 0.0};
	float X[4] = {0.0,0.0,0.0,0.0};
	int i = 0;
	float U;
	unsigned int test = 3000;
	Y[0] = convertToRad(test);
	printf(" angle : %f \n",Y[0]);
	for (i = 0; i < 50; i++)
	{
		U = obscont(Y,X);
	}
	printf("U= %f\n",U); 
	return 0;
}
