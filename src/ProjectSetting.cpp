//-----------------------------------------------------------------------------
//   Cell_Setting.c
//
//   Project: OFM
//   Version: 1.0
//   Date:    08/30/2016
//   Author:  WJ. Chen
//
//   设置步长、初始条件等参数.
//             
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <cmath>
#include <string.h>
#include "FUNC.h"
#include "Global.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void canshuchang()   //参数场，主要为了实现绿地下渗作用（糙率场），以及道路行洪作用
{	
	int i;
	for (i=1;i<=g_CellNum;i++)
	{
		
		CS[i].id=0;
		CS[i].IsGreen=0;
		CS[i].IsRoad=0;

	}
	string temp;
	ifstream fpp;
	stringstream ss;
	fpp.open("G:\\postgraduate\\海甸岛模型结果\\FD\\参数文件2.txt");
	getline(fpp, temp);
	for (i=1;i<=g_CellNum;i++)
	{
		getline(fpp, temp);
		ss<<temp;
		ss>>CS[i].id;
		ss>>CS[i].IsGreen;
		ss>>CS[i].IsRoad;
		ss.clear();
	}
	fpp.close();
	// 道路行洪作用，将道路降低15-20cm
	for (i=1;i<=g_CellNum;i++)
	{
		if (CS[i].IsRoad==1)
		{
			cell_info[i][8]=cell_info[i][8]-0.2;
		}

	}
}
void CellInitialSetting()
{
	
	
	
	//设置项目参数
	string temp;
	string strMeshfile;
	string strBoundaryfile;
	string strResultfile;
	string strTecplotfile;
	string strRainfallfile;
	ifstream fp;
	stringstream ss;;
	
	fp.open("G:\\postgraduate\\民治模型结果\\HHS\\parameter.txt");

	getline(fp, temp);
	getline(fp, strMeshfile);
	getline(fp, temp);
	getline(fp, strBoundaryfile);
	getline(fp, temp);
	getline(fp, strResultfile);
	getline(fp, temp);
	getline(fp, strTecplotfile);
	getline(fp, temp);
	getline(fp, strRainfallfile);
	
	getline(fp, temp);
	getline(fp, temp);
	ss<<temp;
	ss>>OutputTimeInterval;
	ss.clear();
	
	getline(fp, temp);
	getline(fp, temp);
	ss<<temp;
	ss>>time_step_plan;
	ss.clear();
	
	getline(fp, temp);
	getline(fp, temp);
	ss<<temp;
	ss>>Time_total_plan;
	ss.clear();
	fp.close();
	

	sprintf(CellMeshFile.name,"%s",strMeshfile.c_str());  //网格文件
	sprintf(CellBoundaryFile.name,"%s",strBoundaryfile.c_str());     //边界文件
	sprintf(CellResultFile.name,"%s",strResultfile.c_str()); //结果文件
	sprintf(CellTecplotFile.name,"%s",strTecplotfile.c_str());    //Tecplot文件
	/*sprintf(RainfallFile,"%s",strRainfallfile.c_str()); */ //降雨文件
	/*cout<<strMeshfile;
	cout<<CellBoundaryFile.name;
	cout<<CellResultFile.name;
	cout<<CellTecplotFile.name;*/

	
	
	time_step_real=time_step_plan;
	/*设置默认参数*/
	

	Time_total=0.0; //注意此处
	g_CellTotalTimeInTS=0;

	/*Time_total_plan=step_num*time_step_plan; *///step_num*step_dt

	/*干湿边界选项*/
	h_dry=0.0001; //低于此值，不参与计算，drying_value设为零时，相当于不起作用。
	
	Max_Control_Velocity=32; //允许最大流速
	Max_Xcoord=0;
	Max_Ycoord=0;
	radio=0;

	CFL=0.8;
	/*结果出选项*/
	g_IsCellOutputResult=1; //是否输出结果文件
	g_CellOutputStep=1; //结果输出频率
	//OutputTimeInterval=360; //结果输出间隔，程序中用这个作为输出间隔，没有采用上面的g_CellOutputStep控制
}

void SetCellInitialCondition()
{
	int Cell_i,i,j;
	
	int WetorDry(int Cell_ID);
	
	for (Cell_i=1;Cell_i<=g_CellNum;Cell_i++)
	{
		cell_MaxDepth[Cell_i]=0;
		cell_info[Cell_i][1]=0;
		Fr[Cell_i]=0;
		/*if (cell_info[Cell_i][8]>9.7)
		{
			cell_info[Cell_i][1]=0;
		}
		else if (cell_info[Cell_i][8]<=9.7)
		{
			cell_info[Cell_i][1]=9.7-cell_info[Cell_i][8];
		}*/
	
		/*cell_info[Cell_i][2]=1.5;
		cell_info[Cell_i][1]=cell_info[Cell_i][2]-cell_info[Cell_i][8];
		if (cell_info[Cell_i][1]<=0)
		{
			cell_info[Cell_i][1]=0;
		}
*/
	

		/***************stocker dambreak case******************/
	/*	if (cell_info[Cell_i][6]<=0)
		{
			cell_info[Cell_i][1]=10;
		}
		else if (cell_info[Cell_i][6]>0)
		{
			cell_info[Cell_i][1]=5;
		}*/
		
		/**************circle_dam**********************/
	 //   if( (cell_info[Cell_i][6]-2.5)*(cell_info[Cell_i][6]-2.5)+(cell_info[Cell_i][7]-2.5)* (cell_info[Cell_i][7]-2.5)<=0.25)
		//{
		//	cell_info[Cell_i][1]=2;
		//}
		//else 
		//{
		//cell_info[Cell_i][1]=1;
		//}

		/***************Malpasset Dam***********************/
			//double P1x, P1y, P2x, P2y, dis;
			//int i;
			//P1x= 4701.1830;
			//P1y= 4143.4070;
			//P2x= 4655.5530;
			//P2y= 4392.1040;       // 坝址两个端点 //

			//for (i=0;i<=g_CellNum;i++)
			//{ 
			//	dis= (P2y-P1y)*cell_info[i][6]+(P1x-P2x)*cell_info[i][7]+P2x*P1y-P1x*P2y;
			//	 if (dis<=0)			//判断在这两个点的左侧
			//	 {
			//		 cell_info[i][1]=100-cell_info[i][8];
			//		 cell_info[i][3]=0;
			//		 cell_info[i][4]=0;
			//	 }
			//	 else
			//		 cell_info[i][1]=0;
			//		 cell_info[i][3]=0;
			//		 cell_info[i][4]=0;
			//	if(((cell_info[i][6]-4500)*(cell_info[i][6]-4500)+(cell_info[i][7]-5350)*(cell_info[i][7]-5350))<=40000)  //排除上边的角
			//	{
			//		 cell_info[i][1]=0;
			//		 cell_info[i][3]=0;
			//		 cell_info[i][4]=0;
			//	}

			//}
	
			   
		
		/***************非平底溃坝 case*******************/
	/*	 if (cell_info[Cell_i][6]<=16)
		{
			cell_info[Cell_i][1]=1.875;
		}
		else if (cell_info[Cell_i][6]>16)
		{
			cell_info[Cell_i][1]=0;
		} */
		/********************/
		
		cell_info[Cell_i][2]=cell_info[Cell_i][1]+cell_info[Cell_i][8];
		//shuishen(triangle_c[Cell_i][4],triangle_c[Cell_i][5],triangle_c[Cell_i][6],&cell_info[Cell_i][2],&cell_info[Cell_i][1]);
	}
	
	

	for (i=1;i<=g_CellNum;i++)
	{
		cell_info[i][3]=0;   //速度u
		cell_info[i][4]=0.0;   //速度v
		cell_info[i][5]=0.033; //Maning N
		cell_type[0]=0;
		cell_info[0][2]=0.1;  //本初值无任何意义，仅防止数组边界越位
		cell_info[0][1]=0.1;  //本初值无任何意义，仅防止数组边界越位
		triangle_c[0][6]=-1;      //本初值无任何意义，仅防止数组边界越位
	

		U[i][1]=cell_info[i][1]; //h
		U[i][2]=0;				//hu
		U[i][3]=0;				//hv
		U_new[i][1]=cell_info[i][1]; //h
		U_new[i][2]=0;				//hu
		U_new[i][3]=0;				//hv

		cell_source[i]=0;

		Previous_Result[i][1]=cell_info[i][1];
		Previous_Result[i][2]=cell_info[i][2];
		Previous_Result[i][3]=cell_info[i][3];
		Previous_Result[i][4]=cell_info[i][4];
	}

	

	 CurrentRainQ=0;	 
	
	// MIN&MAX of X&Y coordinate
	for (i=1;i<=g_CellNodeNum;i++)
	{
		Max_Xcoord=MAX(Max_Xcoord,node_info[i][2]);
	}
	for (i=1;i<=g_CellNodeNum;i++)
	{
		Max_Ycoord=MAX(Max_Ycoord,node_info[i][3]);
	}
	for (i=1;i<=g_CellNodeNum;i++)
	{
		Min_Xcoord=MIN(Min_Xcoord,node_info[i][2]);
	}
	for (i=1;i<=g_CellNodeNum;i++)
	{
		Min_Ycoord=MIN(Min_Ycoord,node_info[i][3]);
	}


	
	
	for (Cell_i=1;Cell_i<=g_CellNum;Cell_i++)
	{
		cell_type[Cell_i]=WetorDry(Cell_i);
	}
	for (Cell_i=1;Cell_i<=g_CellEdgeNum;Cell_i++)
	{
		Fr_edge[Cell_i]=0;
	}
}




void CellInflowInitaillyDry()
{
	int Cell_i,Cell_k,edge_ID,boundary_i;
	
	if (g_CellBoundary_num<=1) return;    //没有开边界
	for (boundary_i=1;boundary_i<=g_CellBoundary_num-1;boundary_i++)
	{
		if (g_CellBoundary_info[boundary_i].B_Kind!=4) continue;
		for (Cell_i=1;Cell_i<=g_CellNum;Cell_i++)
		{
			for (Cell_k=1;Cell_k<=3;Cell_k++)
			{
				edge_ID=triangle_info[Cell_i][4+Cell_k];
				if(edge_info[edge_ID][6]==g_CellBoundary_info[boundary_i].CellTypeID)
				{
					cell_info[Cell_i][1]=0.05; //水位等于高程加水深
					           //++++++计算对应水深
				}
			}
		}
	}

}

void ReSetCellState()
{
	int i;
	for (i=1;i<=g_CellNum;i++)
	{
		cell_source[i]=0;
		CurrentRainQ=0;
	}
}