#include <stdio.h>
#include <math.h>
#include "FUNC.h"
#include "Global.h"



void ton_gradient(int cell_id) 
{
	double grad_1,grad_2,Temp,b1,b2,b3; //�м����
	b1=node_info[triangle_info[cell_id][2]][4];
	b2=node_info[triangle_info[cell_id][3]][4];
	b3=node_info[triangle_info[cell_id][4]][4];
	Temp=1.0/gradient_con[cell_id][1];

	grad_1=b2-b1;
	grad_2=b3-b1;
	cell_grad[cell_id][1]=(gradient_con[cell_id][5]*grad_1-gradient_con[cell_id][4]*grad_2)*Temp;     //cell_grad[1][2]
	cell_grad[cell_id][2]=(gradient_con[cell_id][2]*grad_2-gradient_con[cell_id][3]*grad_1)*Temp;    //cell_grad[2][2]	
}

void Limitfunction()  //�����ع�ʱ�����ƺ���
{
	double p_rec_un[4]; //δ�����ع�ֵ
	double p;          //pΪ����Ԫ���Ĵ�����ֵ��p_ncΪ�ڽӵ�Ԫ���Ĵ�ֵ
	double lim[4],p_nc[4];
	int i,j,cell_l,cell_r_1, cell_r_2,cell_r_3,edge_k,edge_ID;

	for (i=1;i<=g_CellNum;i++)
	{
			p=cell_info[i][8];

					
			
		cell_r_1=edge_info[triangle_info[i][5]][3];  //��һ�����Ҳ൥Ԫ
		cell_r_2=edge_info[triangle_info[i][6]][3];  //�ڶ������Ҳ൥Ԫ
		cell_r_3=edge_info[triangle_info[i][7]][3];  //���������Ҳ൥Ԫ
		if (abs(cell_r_1)==0)
		{
			cell_r_1=i;
		}
		if (abs(cell_r_2)==0)
		{
			cell_r_2=i;
		}
		if (abs(cell_r_3)==0)
		{
			cell_r_3=i;
		}

	
		
		p_nc[1]= cell_info[cell_r_1][8];
		p_nc[2]= cell_info[cell_r_2][8];
		p_nc[3]= cell_info[cell_r_3][8];

		//δ�����ع�ֵ
		p_rec_un[1]=p+edge_info_c[triangle_info[i][5]][4]*cell_grad[i][1]+edge_info_c[triangle_info[i][5]][5]*cell_grad[i][2];
		p_rec_un[2]=p+edge_info_c[triangle_info[i][6]][4]*cell_grad[i][1]+edge_info_c[triangle_info[i][6]][5]*cell_grad[i][2];	
		p_rec_un[3]=p+edge_info_c[triangle_info[i][7]][4]*cell_grad[i][1]+edge_info_c[triangle_info[i][7]][5]*cell_grad[i][2];
	

		for (j=1;j<=3;j++)
		{
			if(p_rec_un[j]>p)
			{
				lim[j]=MIN(1.0,MAX(0.0,(p_nc[j]-p))/(p_rec_un[j]-p));
			}
			else if(p_rec_un[j]<p)
			{
				lim[j]=MIN(1.0,MIN(0.0,(p_nc[j]-p))/(p_rec_un[j]-p));
			}
			else
			{
				lim[j]=1.0;                                                         
			}
		}
		
		lim_rec[i]=MIN(MIN(lim[1],lim[2]),MIN(lim[2],lim[3]));
	
	}
}

