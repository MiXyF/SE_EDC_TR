/***********************************************************************/
/* Carte d'acquisition DAC (advantech PCM-3712) PC-104.  	           */
/* sous RTAI 3.4                                                       */
/* auteurs : JF et SY									               */    
/***********************************************************************/

//#include<linux/init.h>
//#include<linux/module.h>
#include <stdio.h>
#include <stdlib.h>
//#include <asm/io.h>
//#include <asm/irq.h>
//#include <rtai.h>
//#include <rtai_sched.h>
//#include <rtai_fifos.h>

#include "ctrl.h"

//MODULE_LICENSE("GPL");

float Adc[4][4]= {
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


float Dcd [2]=  { 0 , 0 } ;

data_struct* init_data(data_struct* data)
{
data->A = Adc,
data->B = Bdc,
data->C = Cdc,
data->D = Dcd;
return data;
}

void Observateur_Continue(data_struct *data, const double Y[2])
{
	int i0,i1;	
	double b_data[4], c_data[4]  ;
	double d0 ;
	
	for(i0 = 0;i0 < 4; i0++){
		b_data[i0] = 0.0 ;
		for(i1 = 0;i1 < 4; i1++){
			b_data[i0] += data -> data.A[i0 +(i1<<2)]* data -> data.X[i1] ;
			
		}
	c_data[i0] += data -> data.B[i0 +(i1<<2)]* data -> data.Y[i1] ;	
	}
	d0 = 0.0 ;
	
	for(i0 = 0;i0 < 4; i0++){
		data -> data.X[i0] = b_data[i0] +  c_data[i0]	;
		d0 += data -> data.C[i0] * data -> data.X[i0] ;
	}
	data -> data.U = d0 ;
	
}
	
	//data.X = data.A*data.X+data.B*Y;
	//data.U = data.C*data.X;
/*
	res11 = data.A[1][1]*data.X[1] + data.A[1][2]*data.X[2] + data.A[1][3]*data.X[3] + data.A[1][4]*data.X[4]   +  data.B[1][1]*Y[1] + data.B[1][2]*Y[2]; 
	res21 = data.A[2][1]*data.X[1] + data.A[2][2]*data.X[2] + data.A[2][3]*data.X[3] + data.A[2][4]*data.X[4]  ; 
	res31 = data.A[3][1]*data.X[1] + data.A[3][2]*data.X[2] + data.A[3][3]*data.X[3] + data.A[3][4]*data.X[4]  ; 
	res41 = data.A[4][1]*data.X[1] + data.A[4][2]*data.X[2] + data.A[4][3]*data.X[3] + data.A[4][4]*data.X[4]  ; 
	
*/
	//return data;
}

int main()
{
	int Y[2][1];	
	data_struct* command_data;
	command_data=init_data(command_data);
	Observateur_Continue(command_data,Y); 
	return 0;
}


