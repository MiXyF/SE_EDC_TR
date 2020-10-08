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
#include <rtai_fifos.h>**/

MODULE_LICENSE("GPL");


int affichage_mat(int row, int col, float matrice[row][col]);
float obscont(float Y[2],float X[4]);





