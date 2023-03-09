//-----------------------------------------------------------------------------
//   Link_Global.h
//
//   Date:    09/08/2020     
//   Author:  WQ.WANG (copy from WJ.Chen)
//
//   All the global variables were defined in def.cpp file. This file is the declaration for Global Variables.
//   (extern double/int/char)
//   purpose:  enable .cpp files to usd the global variables and avoid duplicate definition
//-----------------------------------------------------------------------------
#pragma once
#ifndef _LINK_GLOBAL_H_                            //防止重复定义
#define _LINK_GLOBAL_H_
#define   CMHperCFS   101.94
#define   CMDperCFS   2446.6
#define   MLDperCFS   2.4466
#define   M3perFT3    0.028317
#define   LperFT3     28.317
#define   MperFT      0.3048

struct vlink
{
	int           LinkID;               // file name
	int           CellID;               // NO_FILE, SCRATCH, USE, or SAVE
	int           VLinkNodeNum;         //垂直连接节点数
	int           NodesID[MAXLENGTH]; 
	int           IsOverFlow;           //是否存在回流
	int           IsBackFlow;           //是否存在溢流
	double        ExchangeInTotal;      //某时刻单元不同节点交换流量总量 
	double        BackFlowInTotal;      //回流量累积量
	double        OverFlowInTotal;      //节点溢流量累积量
} ;
//int L_TimeStepNum=1;                    //模型计划计算步数
//int L_TimeStep=0;                       //模型当前计算步数
//double L_DTime=1.0;                     //模型计算步长
//double L_CurrentTimeTotal=0;            //模型当前计算总时长
//double L_PlanTimeTotal=0;               //模型计划计算总时长
extern double lateralLink[10000][12];//0:-1;1:No.;2:2D网格单元编号;3:最近的river点1;4:最近的river点2;5:堰顶高程;6:节点1;7:节点2;8:2D水位;9:1D水位；10：堰宽；11：所属河段。
extern double llJunction[10000][2];//0:-1;1:junction No.
extern int lLinkNum;
extern int llJunctionNum;
extern int flOutfallNum;        //记录正向连接所连接的outfall个数
extern int flOutfallid[10000];  //记录正向连接所连接的outfall序号
extern int fLinkNum;            //记录正向连接所连接的个数，也就是2Dcell的个数
extern double flink[10000][11];//0:-1;1:No.;2:2D网格单元编号;3:与之连接的river点;4:2D单元水位;5:;6:节点1;7:节点2;8:;9:；10：堰宽。
extern double junctions[10000][10];//0:ID;1:name;2:X;3:Y;4:fulldepth;5:invertElvt;6:assigned cellID;7:water depth;8:exchanged discharges;9:volume
extern int Nnode;
extern int Njunction,
		   llinkNum1,
		   llinkNum2,
		   flinkNum1,  //边界号
		   flinkID;    //正向耦合边界在bd文件中的顺序（从1开始）
extern int is_include_llink,is_include_flink;
#define  COORDINATE 40
#define  JUNCTION   12
extern double cw,co;  //堰流系数，孔口流量系数
extern double A_node,zhouchang;                 //节点面积
extern int open1Dmodel,open2Dmodel;
extern char inpfile[100];
extern char rptfile[100];
extern char outfile[100];
extern char fLinkfile[100];
#endif