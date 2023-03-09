/*****


*****/


#include <stdio.h>
#include <math.h>
#include "Global.h"
#include "FUNC.h"




int WetorDry (int Cell_ID)
{
	
	int celltype,edge_i,cell_around;
	
	celltype=1;

	if(cell_info[Cell_ID][1] > h_dry)
	{
		celltype=1;
		

		return celltype;
	}
	

	else if (cell_info[Cell_ID][1] <= h_dry)
	{
		for(edge_i=1;edge_i<=3;edge_i++)
		{
			cell_around = edge_info[Cell_ID*3-3+edge_i][3];
			if (cell_info[cell_around][1]<=h_dry || edge_info[Cell_ID*3-3+edge_i][6]==1) //==0
			{
				celltype=0;
				
			}

			if (cell_info[cell_around][1]>h_dry||edge_info[Cell_ID*3-3+edge_i][6]>1) //>=1
			{
				celltype=1;
				return celltype;
				
			}
		}
	}
	
	return celltype;
	
}

int subotsup  (int i)
{ 
	int edgeid, cell_r_id,k=0,edgeid2;
	double hr, hl,fenzi,fenmu,delta;
	if (Fr[i]>0.8)
	{
		for (int Cell_k=1; Cell_k<=3; Cell_k++)
		{
			edgeid=triangle_info[i][4+Cell_k];
			cell_r_id=edge_info[edgeid][3];
			if (cell_r_id>0)
			{
				k=edge_info[edgeid][8];
				edgeid2=3*cell_r_id-(3-k);
				if (k!=0)
				{
					Fr_edge[edgeid]=1;			
					Fr_edge[edgeid2]=1;
				}
			
				
			}
			if (cell_r_id=0)
			{
				Fr_edge[edgeid]=1;	
			}
			k=0;

		}
	}
	/*for (int Cell_k=1; Cell_k<=3; Cell_k++)
	{
		edgeid=triangle_info[i][4+Cell_k];
		cell_r_id=edge_info[edgeid][3];
		if (cell_r_id>0)
		{
			k=edge_info[edgeid][8];
			edgeid2=3*cell_r_id-(3-k);
			hr=cell_info[cell_r_id][1];
			hl=cell_info[i][1];
			fenzi= fabs(hr-hl);
			fenmu=MIN(hr,hl);
			if (fenmu>0.001)
			{
				delta= fenzi/fenmu;
				if (delta>0.01)
				{
					Fr_edge[edgeid]=1;			
					Fr_edge[edgeid2]=1;
				}
			}

			k=0;

		}

	}*/

		
	


	return 1;

	


}



