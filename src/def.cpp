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
	int ID;                            //边界编号
	int B_Kind;                        // 边界类型  !边界类型编号：1代表急流开边界条件，2代表水位边界条件，3代表流速边界条件，4代表流量边界条件，5单宽流量边界条件，0代表陆地边界,6代表正向连接，7代表侧向连接
	int CellTypeID;                    //对应边界网格编号           对于边界的读取，如果是流量边界的话需要填1，急流边界填4；计算的时候1为急流，4为流量。
	int PositionID;                    // 对应边界内部序号
	int Isconstant;                    //是否常量
	double ConstantValue;                //若为常值，则赋常值
	double CurrentValue;                 //若为常值，则赋常值
	int TsID;                          //在时间序列文件中编号
	char TsPath[80];                       //时间序列文件路径
	char StartTime[20];                    //序列开始时间
	double Dtime;                        //序列时间增量
	int Num;                           //时间序列数据条数，初始设为0
	double TS[100][3];
};
struct canshu
{
	int id;
	int IsGreen;
	int IsRoad;
};
canshu *CS;


TFile                            //这里跟fortran版本的有些出入，编程时需注意
				Fmesh,                  //mesh file
				Fboundary,              //boundary file
				Fresult,                //result file
				Finit,                  //initial condition file
				Ftecplot;               //tecplot file
 
int 
				g_CellNodeNum,          //节点数目
				g_CellEdgeNum,          //单元边数目
				g_CellNum,              //单元数目
				g_CellBoundary_num;     //单元数目

int 
	CellRecNum,                         //结果输出步数
	CellRec_DataNums,                   //结果文件中记录结果数目的位置
	CellOutputResultNums,               //记录结果文件中输出了多少个时刻的结果
	Iteration_MaxNum;                 //迭代最大允许次数
	

double
	h_dry,Max_Control_Velocity,
	Time_total,Time_total_plan,  //计算步长
	OutputTimeInterval,TecplotTimeInterval, CurrentRainQ,      OutPutTotal,waterbalance;                                            //最大最小物理计算步长，结果输出间隔

int 
	Q_num,                             //流量边界个数
	step_num,                         //计划总步数
	Rainfall_num;		              //降雨时间序列 数据个数

char RainfallFile[100];




int *IsNeedVirt,
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
	

	 //单元源项


	(*Previous_Result)[5],
	(*edge_info_c)[7],
	*boundary_series, //边界数据
	(*slope_con)[3], //底坡项常项
	*friction_con,
	(*gradient_con)[6],
	(*RainfallQ)[2];

	

CellBoundary g_CellBoundary_info[100];
TFile CellMeshFile,CellBoundaryFile,CellResultFile,CellTecplotFile;

//步长设置
double
	g_CellTotalTimeInTS,
	cputimebegin,                            //系统开始运行时间
	cputimeend;
int
	g_CellCurrentTimeStep;                   //

//结果文件变量
int
	g_CellOutputStep,                        //输出结果频率
	g_CellResultFileId,                      //结果输出文件号
	g_CellVariNum,                           //结果输出变量数目
	g_IsCellOutputResult;                    //是否输出结果


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

double lateralLink[10000][12];//0:-1;1:No.;2:2D网格单元编号;3:最近的river点1;4:最近的river点2;5:堰顶高程;6:节点1;7:节点2;8:2D水位;9:1D水位；10：堰宽；11：所属河段。
double llJunction[10000][2];//0:-1;1:junction No.
int lLinkNum;
int llJunctionNum;
int flOutfallNum;        //记录正向连接所连接的outfall个数
int flOutfallid[10000];  //记录正向连接所连接的outfall序号
int fLinkNum;            //记录正向连接所连接的个数，也就是2Dcell的个数

double flink[10000][11];//0:-1;1:No.;2:2D网格单元编号;3:与之连接的river点;4:2D单元水位;5:;6:节点1;7:节点2;8:;9:；10：堰宽。
double junctions[10000][10];//0:ID;1:name;2:X;3:Y;4:fulldepth;5:invertElvt;6:assigned cellID;7:water depth;8:exchanged discharges;9:volume
int Nnode;
int Njunction,
		   llinkNum1,
		   llinkNum2,
		   flinkNum1,  //边界号
		   flinkID;    //正向耦合边界在bd文件中的顺序（从1开始）
int is_include_llink,is_include_flink;

double cw,co;  //堰流系数，孔口流量系数
double A_node,zhouchang;                 //节点面积
int open1Dmodel,open2Dmodel;
char inpfile[100];
char rptfile[100];
char outfile[100];
char fLinkfile[100];
