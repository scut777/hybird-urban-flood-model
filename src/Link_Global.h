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
#ifndef _LINK_GLOBAL_H_                            //��ֹ�ظ�����
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
	int           VLinkNodeNum;         //��ֱ���ӽڵ���
	int           NodesID[MAXLENGTH]; 
	int           IsOverFlow;           //�Ƿ���ڻ���
	int           IsBackFlow;           //�Ƿ��������
	double        ExchangeInTotal;      //ĳʱ�̵�Ԫ��ͬ�ڵ㽻���������� 
	double        BackFlowInTotal;      //�������ۻ���
	double        OverFlowInTotal;      //�ڵ��������ۻ���
} ;
//int L_TimeStepNum=1;                    //ģ�ͼƻ����㲽��
//int L_TimeStep=0;                       //ģ�͵�ǰ���㲽��
//double L_DTime=1.0;                     //ģ�ͼ��㲽��
//double L_CurrentTimeTotal=0;            //ģ�͵�ǰ������ʱ��
//double L_PlanTimeTotal=0;               //ģ�ͼƻ�������ʱ��
extern double lateralLink[10000][12];//0:-1;1:No.;2:2D����Ԫ���;3:�����river��1;4:�����river��2;5:�߶��߳�;6:�ڵ�1;7:�ڵ�2;8:2Dˮλ;9:1Dˮλ��10���߿�11�������ӶΡ�
extern double llJunction[10000][2];//0:-1;1:junction No.
extern int lLinkNum;
extern int llJunctionNum;
extern int flOutfallNum;        //��¼�������������ӵ�outfall����
extern int flOutfallid[10000];  //��¼�������������ӵ�outfall���
extern int fLinkNum;            //��¼�������������ӵĸ�����Ҳ����2Dcell�ĸ���
extern double flink[10000][11];//0:-1;1:No.;2:2D����Ԫ���;3:��֮���ӵ�river��;4:2D��Ԫˮλ;5:;6:�ڵ�1;7:�ڵ�2;8:;9:��10���߿�
extern double junctions[10000][10];//0:ID;1:name;2:X;3:Y;4:fulldepth;5:invertElvt;6:assigned cellID;7:water depth;8:exchanged discharges;9:volume
extern int Nnode;
extern int Njunction,
		   llinkNum1,
		   llinkNum2,
		   flinkNum1,  //�߽��
		   flinkID;    //������ϱ߽���bd�ļ��е�˳�򣨴�1��ʼ��
extern int is_include_llink,is_include_flink;
#define  COORDINATE 40
#define  JUNCTION   12
extern double cw,co;  //����ϵ�����׿�����ϵ��
extern double A_node,zhouchang;                 //�ڵ����
extern int open1Dmodel,open2Dmodel;
extern char inpfile[100];
extern char rptfile[100];
extern char outfile[100];
extern char fLinkfile[100];
#endif