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
#ifndef  _GLOBAL_H_                //��ֹ�ظ�����
#define  _GLOBAL_H_
#define  EXTERN extern
#define  MAXFNAME 100
#define  MAXLENGTH 900000    //��������������cell��node�ϴ��һ��������Ƕ���һ���������������
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
	int ID;                            //�߽���
	int B_Kind;                        // �߽�����  !�߽����ͱ�ţ�1���������߽�������2����ˮλ�߽�������3�������ٱ߽�������4���������߽�������5���������߽�������0����½�ر߽�,6�����������ӣ�7�����������
	int CellTypeID;                    //��Ӧ�߽������� (4 ��  1����)          ���ڱ߽�Ķ�ȡ������������߽�Ļ���Ҫ��1�������߽���4�������ʱ��1Ϊ������4Ϊ������
	int PositionID;                    // ��Ӧ�߽��ڲ����
	int Isconstant;                    //�Ƿ���
	double ConstantValue;                //��Ϊ��ֵ���򸳳�ֵ
	double CurrentValue;                 //��Ϊ��ֵ���򸳳�ֵ
	int TsID;                          //��ʱ�������ļ��б��
	char TsPath[80];                       //ʱ�������ļ�·��
	char StartTime[20];                    //���п�ʼʱ��
	double Dtime;                        //����ʱ������
	int Num;                           //ʱ������������������ʼ��Ϊ0
	double TS[100][3];                //777777777ע���������̫��Ѱ����ķ���7777777//
};
struct canshu
{
	int id;
	int IsGreen;
	int IsRoad;
};
extern canshu *CS;

extern TFile                            //�����fortran�汾����Щ���룬���ʱ��ע��
				Fmesh,                  //mesh file
				Fboundary,              //boundary file
				Fresult,                //result file
				Finit,                  //initial condition file
				Ftecplot;               //tecplot file
 
extern int 
				g_CellNodeNum,          //�ڵ���Ŀ
				g_CellEdgeNum,          //��Ԫ����Ŀ
				g_CellNum,              //��Ԫ��Ŀ
				g_CellBoundary_num;     //��Ԫ��Ŀ

extern int 
	CellRecNum,                         //����������
	CellRec_DataNums,                   //����ļ��м�¼�����Ŀ��λ��
	CellOutputResultNums,               //��¼����ļ�������˶��ٸ�ʱ�̵Ľ��
	Iteration_MaxNum;                  //��������������
	

extern double
	h_dry,Max_Control_Velocity,
	Time_total,Time_total_plan,   //���㲽��
	OutputTimeInterval,TecplotTimeInterval,CurrentRainQ, OutPutTotal,waterbalance;                                                   //���������

extern int 
	Q_num,                             //�����߽����
	step_num,                          //�ƻ��ܲ���
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
	(*boundary_Q)[20],   //�����߽�ϵ��
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

	

	 //��ԪԴ��

	(*Previous_Result)[5],
	(*edge_info_c)[7],
	*boundary_series, //�߽�����
	(*slope_con)[3], //�������
	*friction_con,
	(*gradient_con)[6],
	(*RainfallQ)[2];

	

extern CellBoundary g_CellBoundary_info[100]; //ע������ͬ������̫��7777777//
extern TFile CellMeshFile,CellBoundaryFile,CellResultFile,CellTecplotFile;

//��������
extern double
	g_CellTotalTimeInTS,
	cputimebegin,                            //ϵͳ��ʼ����ʱ��
	cputimeend;
extern int
	g_CellCurrentTimeStep;                   //�Ƿ�������

//����ļ�����
extern int
	g_CellOutputStep,                        //������Ƶ��
	g_CellResultFileId,                      //�������ļ���
	g_CellVariNum,                           //������������Ŀ
	g_IsCellOutputResult;                    //�Ƿ�������


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