//-----------------------------------------------------------------------------
//   Global.h
//
//   Date:    05/17/2019     
//   Author:  WQ.WANG
//
//   
//   All the global variables were defined in def.cpp file. This file is the  declaration for Global Variables.
//   (extern double/int/char)
//   purpose: enable .cpp files to use the global variables and avoid duplicate definition.

//-----------------------------------------------------------------------------
#ifndef  _GLOBAL_H_                //防止重复定义
#define  _GLOBAL_H_
#define  EXTERN extern
#define  MAXFNAME 100
#define  MAXLENGTH 900000    //将这个定义成最大的cell和node较大的一个（最好是定义一个不会溢出的数）
#define  Cr  9.8
#define  e   0.000000001
#include <stdio.h>
#include <malloc.h>
struct TFile
{
	char          name[MAXFNAME+1];     // file name
	char          mode;                 // NO_FILE, SCRATCH, USE, or SAVE
	char          state;                // current state (OPENED, CLOSED)
	FILE*         file;                 // FILE structure pointer
};
struct CellBoundary
{
	int ID;                            //边界编号
	int B_Kind;                        // 边界类型  !边界类型编号：1代表急流开边界条件，2代表水位边界条件，3代表流速边界条件，4代表流量边界条件，5单宽流量边界条件，0代表陆地边界,6代表正向连接，7代表侧向连接
	int CellTypeID;                    //对应边界网格编号 (4 开  1流量)          对于边界的读取，如果是流量边界的话需要填1，急流边界填4；计算的时候1为急流，4为流量。
	int PositionID;                    // 对应边界内部序号
	int Isconstant;                    //是否常量
	double ConstantValue;                //若为常值，则赋常值
	double CurrentValue;                 //若为常值，则赋常值
	int TsID;                          //在时间序列文件中编号
	char TsPath[80];                       //时间序列文件路径
	char StartTime[20];                    //序列开始时间
	double Dtime;                        //序列时间增量
	int Num;                           //时间序列数据条数，初始设为0
	double TS[100][3];                //777777777注意这里，不能太大，寻求变大的方法7777777//
};
struct canshu
{
	int id;
	int IsGreen;
	int IsRoad;
};
extern canshu *CS;

extern TFile                            //这里跟fortran版本的有些出入，编程时需注意
				Fmesh,                  //mesh file
				Fboundary,              //boundary file
				Fresult,                //result file
				Finit,                  //initial condition file
				Ftecplot;               //tecplot file
 
extern int 
				g_CellNodeNum,          //节点数目
				g_CellEdgeNum,          //单元边数目
				g_CellNum,              //单元数目
				g_CellBoundary_num;     //单元数目

extern int 
	CellRecNum,                         //结果输出步数
	CellRec_DataNums,                   //结果文件中记录结果数目的位置
	CellOutputResultNums,               //记录结果文件中输出了多少个时刻的结果
	Iteration_MaxNum;                  //迭代最大允许次数
	

extern double
	h_dry,Max_Control_Velocity,
	Time_total,Time_total_plan,   //计算步长
	OutputTimeInterval,TecplotTimeInterval,CurrentRainQ, OutPutTotal,waterbalance;                                                   //结果输出间隔

extern int 
	Q_num,                             //流量边界个数
	step_num,                          //计划总步数
	Rainfall_num;

extern char RainfallFile[100];


	

extern int *IsNeedVirt,
	*IsNodeNeedVirt,
	(*node_info_cell)[15],
	*node_type,
	*cell_type,
	(*triangle_info)[8],
	(*edge_info)[8],
	(*boundary_info)[5],
	(*boundary_Q)[20],   //流量边界系列
	*C_Index,
	*Fr_edge;



extern double
	(*node_info)[5],
	(*node_distance)[12],
	(*node_distance_square)[12],
	(*node_cha)[7],
	(*triangle_c)[8],
	(*cell_info)[10],
	(*cell_grad)[3],	
	*lim_rec,
	*Fr,
	radio,

	

	 //单元源项

	(*Previous_Result)[5],
	(*edge_info_c)[7],
	*boundary_series, //边界数据
	(*slope_con)[3], //底坡项常项
	*friction_con,
	(*gradient_con)[6],
	(*RainfallQ)[2];

	

extern CellBoundary g_CellBoundary_info[100]; //注意这里同样不能太大7777777//
extern TFile CellMeshFile,CellBoundaryFile,CellResultFile,CellTecplotFile;

//步长设置
extern double
	g_CellTotalTimeInTS,
	cputimebegin,                            //系统开始运行时间
	cputimeend;
extern int
	g_CellCurrentTimeStep;                   //是否输出结果

//结果文件变量
extern int
	g_CellOutputStep,                        //输出结果频率
	g_CellResultFileId,                      //结果输出文件号
	g_CellVariNum,                           //结果输出变量数目
	g_IsCellOutputResult;                    //是否输出结果


extern double 	
	(*U)[4],
	//1125 +++
	(*U_new)[4],
	time_step_real,
	time_step_plan,
	*time_limit,
	CFL,
	*cell_MaxDepth,
	*cell_MaxVelcity,
	Max_Xcoord,
	Max_Ycoord,
	Min_Xcoord,
	Min_Ycoord;

extern const double 
	pi,
	g;
extern double

*cell_source;

#endif