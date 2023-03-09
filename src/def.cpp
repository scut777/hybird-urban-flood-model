//-----------------------------------------------------------------------------
//   Definition of global variables 
//

//   Date:    05/17/2019     
//   Author:  WQ.WANG
//
//   Definition for Global Variables. 
//   All the global variables are defined in this .cpp file.
//   This variables were declared in .h file, and were called in other .cpp files.
//
//-----------------------------------------------------------------------------

#define  MAXFNAME 100
#define  MAXLENGTH 900000 

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
	int CellTypeID;                    //��Ӧ�߽�������           ���ڱ߽�Ķ�ȡ������������߽�Ļ���Ҫ��1�������߽���4�������ʱ��1Ϊ������4Ϊ������
	int PositionID;                    // ��Ӧ�߽��ڲ����
	int Isconstant;                    //�Ƿ���
	double ConstantValue;                //��Ϊ��ֵ���򸳳�ֵ
	double CurrentValue;                 //��Ϊ��ֵ���򸳳�ֵ
	int TsID;                          //��ʱ�������ļ��б��
	char TsPath[80];                       //ʱ�������ļ�·��
	char StartTime[20];                    //���п�ʼʱ��
	double Dtime;                        //����ʱ������
	int Num;                           //ʱ������������������ʼ��Ϊ0
	double TS[100][3];
};
struct canshu
{
	int id;
	int IsGreen;
	int IsRoad;
};
canshu *CS;


TFile                            //�����fortran�汾����Щ���룬���ʱ��ע��
				Fmesh,                  //mesh file
				Fboundary,              //boundary file
				Fresult,                //result file
				Finit,                  //initial condition file
				Ftecplot;               //tecplot file
 
int 
				g_CellNodeNum,          //�ڵ���Ŀ
				g_CellEdgeNum,          //��Ԫ����Ŀ
				g_CellNum,              //��Ԫ��Ŀ
				g_CellBoundary_num;     //��Ԫ��Ŀ

int 
	CellRecNum,                         //����������
	CellRec_DataNums,                   //����ļ��м�¼�����Ŀ��λ��
	CellOutputResultNums,               //��¼����ļ�������˶��ٸ�ʱ�̵Ľ��
	Iteration_MaxNum;                 //��������������
	

double
	h_dry,Max_Control_Velocity,
	Time_total,Time_total_plan,  //���㲽��
	OutputTimeInterval,TecplotTimeInterval, CurrentRainQ,      OutPutTotal,waterbalance;                                            //�����С������㲽�������������

int 
	Q_num,                             //�����߽����
	step_num,                         //�ƻ��ܲ���
	Rainfall_num;		              //����ʱ������ ���ݸ���

char RainfallFile[100];




int *IsNeedVirt,
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



double
	(*node_info)[5],
	(*node_distance)[12],
	(*node_distance_square)[12],
	(*node_cha)[7],
	(*triangle_c)[8],
	(*cell_info)[10],
	(*cell_grad)[3],
	(*lim_rec),
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

	

CellBoundary g_CellBoundary_info[100];
TFile CellMeshFile,CellBoundaryFile,CellResultFile,CellTecplotFile;

//��������
double
	g_CellTotalTimeInTS,
	cputimebegin,                            //ϵͳ��ʼ����ʱ��
	cputimeend;
int
	g_CellCurrentTimeStep;                   //

//����ļ�����
int
	g_CellOutputStep,                        //������Ƶ��
	g_CellResultFileId,                      //�������ļ���
	g_CellVariNum,                           //������������Ŀ
	g_IsCellOutputResult;                    //�Ƿ�������


double 
	(*U)[4],

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

double *cell_source;

double lateralLink[10000][12];//0:-1;1:No.;2:2D����Ԫ���;3:�����river��1;4:�����river��2;5:�߶��߳�;6:�ڵ�1;7:�ڵ�2;8:2Dˮλ;9:1Dˮλ��10���߿�11�������ӶΡ�
double llJunction[10000][2];//0:-1;1:junction No.
int lLinkNum;
int llJunctionNum;
int flOutfallNum;        //��¼�������������ӵ�outfall����
int flOutfallid[10000];  //��¼�������������ӵ�outfall���
int fLinkNum;            //��¼�������������ӵĸ�����Ҳ����2Dcell�ĸ���

double flink[10000][11];//0:-1;1:No.;2:2D����Ԫ���;3:��֮���ӵ�river��;4:2D��Ԫˮλ;5:;6:�ڵ�1;7:�ڵ�2;8:;9:��10���߿�
double junctions[10000][10];//0:ID;1:name;2:X;3:Y;4:fulldepth;5:invertElvt;6:assigned cellID;7:water depth;8:exchanged discharges;9:volume
int Nnode;
int Njunction,
		   llinkNum1,
		   llinkNum2,
		   flinkNum1,  //�߽��
		   flinkID;    //������ϱ߽���bd�ļ��е�˳�򣨴�1��ʼ��
int is_include_llink,is_include_flink;

double cw,co;  //����ϵ�����׿�����ϵ��
double A_node,zhouchang;                 //�ڵ����
int open1Dmodel,open2Dmodel;
char inpfile[100];
char rptfile[100];
char outfile[100];
char fLinkfile[100];
