#include <stdio.h>
#include <math.h>
#include "Global.h"
#include <string.h>
#include <iostream>

void friction_constant()
{
	int i;
	if (g_CellNum<=0)
	{
		return;
	}
	for (i=1;i<=g_CellNum;i++)
	{
		friction_con[i]=g*cell_info[i][5]*cell_info[i][5];   //g*n*n
	}
}


void slope(double b1,double b2,double b3,double x1,double x2,double x3,double y1,double y2,double y3,double *C_aera,double *slope_x,double *slope_y)
{
	*C_aera=0.5*((x1-x2)*(y1+y2)+(x2-x3)*(y2+y3)+(x3-x1)*(y3+y1));
	*slope_x=((y2-y3)*b1+(y3-y1)*b2+(y1-y2)*b3)/2.0/(*C_aera);      //4-68
	*slope_y=((x3-x2)*b1+(x1-x3)*b2+(x2-x1)*b3)/2.0/(*C_aera);
}


double pow1(double x, double i)
{
	double y;

	if (x>=0)
	{ 
		y=1/(pow(x,4.0/3.0)) ;
		return y;	
	}
	
	if (x<0)
	{ 
		printf("%s\n","水深出现负值");

		printf("错误网格位置Cell_i=%d\n",i);
	
	
		
		return 0;
		;
	
	}
	 
	
}

double chuL (double x, double y)
{
	if (y==0)
	{
		return 0;
	}
	if (y!=0)
	{
		return x/y;
	}
}


