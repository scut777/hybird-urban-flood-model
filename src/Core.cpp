//-----------------------------------------------------------------------------
//   
//        
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <cmath>
#include <string.h>
#include "Global.h"
#include "FUNC.h"
#include <iostream>
#include "link_global.h"

using namespace std;

void Flux_compute(double totaltime, int cellnum, int current_step)
{
	int Cell_i, Cell_k, edge_ID, Edge_L;
	double sweno,limno;


	double Mx, My, Sf_h, RainSource,waterbalance2,wetcell;
	double flux_edge[4][4], h_left, h_right, hu_left, hu_right, hv_left, hv_right, b_i, b_j, h_i, h_j, n1, n2, b_face, S_b[4][4], water_level_i,del_b;
	void Riemann_states (int edge_ID, int *Edge_L,double *h_left, double *h_right, double *hu_right, double *hu_left, double *hv_left, double *hv_right, double *b_i,double *b_j,double *h_i, double *h_j,double *n1, double *n2,double *b_face, double *water_level_i , double *del_b);
	void Roe (int Edge_L,double h_left, double h_right, double hu_left, double hu_right, double hv_left, double hv_right,double n1, double n2, double *flux_1, double *flux_2, double *flux_3);
	void bed_slope (double n1, double n2, double h_j,double h_i,double b_i, double h_left, double b_face, double water_level_i, double del_b,double *S_b1, double *S_b2, double *S_b3);
	void Boundary_pre(int edge_ID, int current_step, double *flux_1, double *flux_2, double *flux_3);
	void friction_constant();
	double pow1(double x, double i);
	int WetorDry (int Cell_ID);
	void GWM (int Edge_L,double h_left, double h_right, double hu_left, double hu_right, double hv_left, double hv_right,double n1, double n2, double *flux_1, double *flux_2, double *flux_3);
	void Roe_weight (int Edge_L,double h_left, double h_right, double hu_left, double hu_right, double hv_left, double hv_right,double n1, double n2, double *flux_1, double *flux_2, double *flux_3);
	void Roe_swm (int Edge_L,double h_left, double h_right, double hu_left, double hu_right, double hv_left, double hv_right,double n1, double n2, double *flux_1, double *flux_2, double *flux_3);
	int subotsup  (int i);
	//ͨ������
	
	sweno=limno=0;
	wetcell=1;
	waterbalance2=0;
		
	for (Cell_i=1; Cell_i<=g_CellNum; Cell_i++)
	{
		cell_type[Cell_i] = WetorDry(Cell_i);
		if (cell_type[Cell_i]==1){wetcell=triangle_c[Cell_i][1]+wetcell;}
		subotsup  (Cell_i);
	}
	
	
	for (Cell_i=1; Cell_i<=g_CellNum; Cell_i++)
	{
		
        if (cell_type[Cell_i]==1)//  ʪ��Ԫ���ߵ�Ԫ�Ǳ߽�
		{
			for (Cell_k=1; Cell_k<=3; Cell_k++)
			{
				edge_ID = triangle_info[Cell_i][4+Cell_k];//�ߵ�ID
				
				if (edge_info[edge_ID][6]==0)//�ڲ��߽���������
				{
					
					Riemann_states (edge_ID, &Edge_L, &h_left, &h_right, &hu_right, &hu_left, &hv_left, &hv_right, &b_i, &b_j,&h_i, &h_j,&n1, &n2,&b_face, &water_level_i,&del_b); 
					if (Fr_edge[edge_ID]==0)
					{
						Roe (Edge_L, h_left, h_right, hu_left, hu_right, hv_left, hv_right, n1, n2, &flux_edge[1][Cell_k], &flux_edge[2][Cell_k], &flux_edge[3][Cell_k]);
						limno=limno+1;
					}
					if (0<Fr_edge[edge_ID]>1)
					{
						Roe_weight (Edge_L, h_left, h_right, hu_left, hu_right, hv_left, hv_right, n1, n2, &flux_edge[1][Cell_k], &flux_edge[2][Cell_k], &flux_edge[3][Cell_k]);
						limno=limno+1;
					}
					else if (Fr_edge[edge_ID]==1)
					{
						Roe_swm (Edge_L, h_left, h_right, hu_left, hu_right, hv_left, hv_right, n1, n2, &flux_edge[1][Cell_k], &flux_edge[2][Cell_k], &flux_edge[3][Cell_k]);
						sweno=sweno+1;
					}
					Fr_edge[edge_ID]=0;// 9-7 jia 

				/*GWM (Edge_L, h_left, h_right, hu_left, hu_right, hv_left, hv_right, n1, n2, &flux_edge[1][Cell_k], &flux_edge[2][Cell_k], &flux_edge[3][Cell_k]);*/
				bed_slope (n1, n2, h_j, h_i, b_i, h_left, b_face, water_level_i,del_b, &S_b[1][Cell_k],  &S_b[2][Cell_k], &S_b[3][Cell_k]);
					
                }
				else if (edge_info[edge_ID][6]!=0)
				{
					Boundary_pre(edge_ID, current_step, &flux_edge[1][Cell_k], &flux_edge[2][Cell_k], &flux_edge[3][Cell_k]);
					S_b[1][Cell_k]=0;
					S_b[2][Cell_k]=0;
					S_b[3][Cell_k]=0;
					/*limno=limno+1;*/
				}
			}

			cell_source[Cell_i]=cell_source[Cell_i];  // �ɹ����������µ�cellsource
			
			/////////////////////////////////////////////////////////////////////////////// R-I  
			 
			//RainSource=CurrentRainQ*triangle_c[Cell_i][1];  //���㽵�������Դ�� ���굥λm/s Դ�λm3/s7777777
			
			//cell_source[Cell_i]=cell_source[Cell_i]+RainSource;  //junction+R-I ��Դ��
			//////////////////////////////////////////////////////////////////////////////
				
U_new[Cell_i][1]=  (U[Cell_i][1] - time_step_real * ((flux_edge[1][1] * edge_info_c[triangle_info[Cell_i][5]][3] + flux_edge[1][2]*edge_info_c[triangle_info[Cell_i][6]][3]+flux_edge[1][3]*edge_info_c[triangle_info[Cell_i][7]][3])-cell_source[Cell_i])/triangle_c[Cell_i][1])/*-waterbalance/wetcell*/;   
			
			
				// friction ���� Xilin Xia(2018)
				Mx=U_new[Cell_i][2]-time_step_real*((flux_edge[2][1]-S_b[2][1])*edge_info_c[triangle_info[Cell_i][5]][3]+(flux_edge[2][2]-S_b[2][2])*edge_info_c[triangle_info[Cell_i][6]][3]+(flux_edge[2][3]-S_b[2][3])*edge_info_c[triangle_info[Cell_i][7]][3])/triangle_c[Cell_i][1];
				
				My=U_new[Cell_i][3]-time_step_real*((flux_edge[3][1]-S_b[3][1])*edge_info_c[triangle_info[Cell_i][5]][3]+(flux_edge[3][2]-S_b[3][2])*edge_info_c[triangle_info[Cell_i][6]][3]+(flux_edge[3][3]-S_b[3][3])*edge_info_c[triangle_info[Cell_i][7]][3])/triangle_c[Cell_i][1];
				
				if(U_new[Cell_i][1] <= h_dry)
				{
					U_new[Cell_i][2]= Mx;
					U_new[Cell_i][3]= My;
					
				}
				
				else if(U_new[Cell_i][1]>h_dry)
				{
					//sf_h ˮ���ж�
					Sf_h = time_step_real*friction_con[Cell_i]  *   pow1(U_new[Cell_i][1],Cell_i)   * sqrt((Mx/U_new[Cell_i][1])*(Mx/U_new[Cell_i][1])+(My/U_new[Cell_i][1])*(My/U_new[Cell_i][1]));
					

			
					if (Sf_h<=0.00001)
					{
					U_new[Cell_i][2]= Mx;
					U_new[Cell_i][3]= My;

					}
					else if (Sf_h>0.00001)
					{
					U_new[Cell_i][2]= (Mx-Mx*sqrt((1+4*Sf_h)))/((-2)*Sf_h);
					U_new[Cell_i][3]= (My-My*sqrt((1+4*Sf_h)))/((-2)*Sf_h);
					}	
				}

					

				
				

				if (U_new[Cell_i][1]<0)
				{
					/*cout<<Cell_i<<U_new[Cell_i][1]<<endl;*/

					waterbalance2=waterbalance2-U_new[Cell_i][1]*triangle_c[Cell_i][1];
					U_new[Cell_i][1]=0;
										
				}

				cell_info[Cell_i][1]=U_new[Cell_i][1];  //����h
				cell_info[Cell_i][2]=cell_info[Cell_i][1]+cell_info[Cell_i][8];//ˮλ=ˮ��+���Ĵ�z����Ҫ��������
				if (cell_info[Cell_i][1]>h_dry)
				{


					if (sqrt(U_new[Cell_i][2]/U_new[Cell_i][1]*U_new[Cell_i][2]/U_new[Cell_i][1]+U_new[Cell_i][3]/U_new[Cell_i][1]*U_new[Cell_i][3]/U_new[Cell_i][1])>=50)   //6.15 + ˮ���С�����ټ�������޶����ٸ��ϸ�ʱ�䲽һ�£�
					{
						cell_info[Cell_i][3]=cell_info[Cell_i][3];
						cell_info[Cell_i][4]=cell_info[Cell_i][4];
						U_new[Cell_i][2]=cell_info[Cell_i][3]*U_new[Cell_i][1];
						U_new[Cell_i][3]=cell_info[Cell_i][4]*U_new[Cell_i][1];
					}
					else 
					{
						cell_info[Cell_i][3]=U_new[Cell_i][2]/U_new[Cell_i][1]; //����u
						cell_info[Cell_i][4]=U_new[Cell_i][3]/U_new[Cell_i][1]; //����v 
					} ///6.15 +

				//CFL ʱ�䲽��
				  //      if (Fr[Cell_i]<0)
						//{
						/*time_limit[Cell_i]= CFL*triangle_c[Cell_i][7]/(sqrt(g*U[Cell_i][1])+sqrt(cell_info[Cell_i][3]*cell_info[Cell_i][3]+cell_info[Cell_i][4]*cell_info[Cell_i][4]));*/	//��ͳSWE CFL
						//}
						//else
						//{
						time_limit[Cell_i]= CFL*triangle_c[Cell_i][7]/(sqrt(g*cell_info[Cell_i][1]));    //CFL for SSWE	
						/*}*/

				
					
					if (time_limit[Cell_i]<0.001)
					{
						cout<<time_limit[Cell_i]<<endl;
						cout<<Cell_i<<endl;
						cout<<cell_info[Cell_i][1]<<endl;
						system("pause");
					}

				}
				
				if (cell_info[Cell_i][1]<=h_dry)
				{
					cell_info[Cell_i][3]=0; //����u
					cell_info[Cell_i][4]=0; //����v
					time_limit[Cell_i]=777;    
					U_new[Cell_i][1]=cell_info[Cell_i][1];
					U_new[Cell_i][2]=0;
					U_new[Cell_i][3]=0;
				}
			Fr[Cell_i]=sqrt(cell_info[Cell_i][3]*cell_info[Cell_i][3]+cell_info[Cell_i][4]*cell_info[Cell_i][4])/(sqrt(g*U[Cell_i][1]));
		}

		
				

		if (cell_type[Cell_i]==0)//�ɵ�Ԫ
		{
			
			//cell_source[Cell_i]=cell_source[Cell_i];  // �ɹ����������µ�cellsource			
			//// R-I  
			//RainSource=CurrentRainQ*triangle_c[Cell_i][1];  //���㽵�������Դ�� ���굥λm/s Դ�λm3/s
			//cell_source[Cell_i]=cell_source[Cell_i]+RainSource;  //junction+R-I ��Դ��
		
			if (cell_source[Cell_i]!=0)  // ��龮��������н��꣬����ˮ����������һ���ټ���
			{			
				cell_info[Cell_i][1]=cell_info[Cell_i][1]+cell_source[Cell_i]/triangle_c[Cell_i][1];
				cell_info[Cell_i][2]=cell_info[Cell_i][1]+cell_info[Cell_i][8];
				cell_info[Cell_i][3]=0;
				cell_info[Cell_i][4]=0;	
				U_new[Cell_i][1]=cell_info[Cell_i][1];
				U_new[Cell_i][2]=0;
				U_new[Cell_i][3]=0;
				time_limit[Cell_i]= CFL*triangle_c[Cell_i][7]/sqrt(g*U[Cell_i][1]);
			}

			if (cell_source[Cell_i]==0)  //�ɵ�Ԫ������
			{
				cell_info[Cell_i][1]=cell_info[Cell_i][1];
				cell_info[Cell_i][2]=cell_info[Cell_i][2];
				cell_info[Cell_i][3]=0;
				cell_info[Cell_i][4]=0;	
				U_new[Cell_i][1]=0;
				U_new[Cell_i][2]=0;
				U_new[Cell_i][3]=0;
				time_limit[Cell_i]= 777;
			}
			Fr[Cell_i]=0;
		}
		


}
	waterbalance=waterbalance2;
	

	time_step_real=1;   //���������ʼ�׶�ʱ�䲽����С���ļ�����0.05���С��Ȼ����������


	//���±���������һʱ�䲽�ļ���
	for (Cell_i=1; Cell_i<=g_CellNum; Cell_i++)
	{
		U[Cell_i][1]=U_new[Cell_i][1];
		U[Cell_i][2]=U_new[Cell_i][2];
		U[Cell_i][3]=U_new[Cell_i][3];
	}


	for (Cell_i=1; Cell_i<=g_CellNum; Cell_i++)
	{
		
		time_step_real=MIN(time_step_real,time_limit[Cell_i]);	

		
		
	}
	radio=sweno/(sweno+limno);
	
	/*if (time_step_real<0.001)
	{

	time_step_real=0.001;
	}*/

}


void Cell_ProTreatment() //����Ԥ����
{
	void CellInitialSetting();
	int readMesh(char filename_inp[]);
	

	
	void SetCellInitialCondition();    //����ֵ
	void friction_constant(); 
	void CellInitialSetting(); //��ȡ��������
	void Read_Cell_Boundary_File(int Cell_Boundary_Num,char bdfile[]);
	void ProSetCellBoundaryInfo(); //�Ա߽����Ԥ����
	void boundary_Qnum(); //�����߽�ͳ��
	void CellInflowInitaillyDry(); //���������߽�ȫΪ�����
	void Read_Rainfall_File(char filename[]);
	int errorum;
	void canshuchang();
    CellInitialSetting();
	g_CellNodeNum=0; //Ĭ��û�нڵ�
	errorum=readMesh(CellMeshFile.name); //��ȡ�����ļ�
	if(g_CellNodeNum<=0) 
	{
		printf("%s\n","��ȡ�����ļ�ʧ�ܣ����񶥵�������");
		return; //��������ļ���ȡ��������û������
	}
	if(g_CellNum<=0)
	{
		printf("%s\n","��ȡ�����ļ�ʧ�ܣ�����Ԫ������");
		return;
	} //��������ļ���ȡ��������û������
	printf("%s\n","�ɹ���ȡ�����ļ�");
	/*canshuchang();*/
	/*Read_Rainfall_File(RainfallFile);*/
	SetCellInitialCondition();    //����ֵ
	friction_constant();    //����Ħ�����
	Read_Cell_Boundary_File(g_CellBoundary_num,CellBoundaryFile.name); //��ȡ�߽�����
	
	printf("%s\n","�ɹ���ȡ�߽��ļ�");
	ProSetCellBoundaryInfo(); //�Ա߽����Ԥ����
	boundary_Qnum(); //�����߽�ͳ��
	CellInflowInitaillyDry(); //���������߽�ȫΪ�����
}

