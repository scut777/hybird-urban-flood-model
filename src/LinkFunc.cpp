//-----------------------------------------------------------------------------
//   link_functions.c
//
//   Project: OFM
//   Version: 1.0
//   Date:    11/09/16   (Build 5.0.010)         
//   Author:  W. Chen
//
//   Some functions for the connection of swmm and 2D model. 
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "swmm5.h"
#include "FUNC.h"
#include "Global.h"
#include "readswmm.h"
#include "Link_Global.h"

//c++ ��������ǰ������
int readNodeXY(char *inpFile);
static int getTokens(char *s);
static int findmatch(char *s, char *keyword[]);
static int match(char *str, char *substr);
static int parseLine(int sect, char *line);
static int readNode(int num);
static int junc_readParams(int num, char* tok[], int ntoks);
static int getDouble(char *s, double *y);
int Link_PointOnCell(int nodeNum);
static void IsPointOnCell(double Po1,double Po2,double P11,double P12,double P21,double P22,double P31,double P32,int *IsIn);
void setlateralLink();
void setlLinkJunc();
void fineNearllJunction(int llinkid,int cellid);
void find1DLevel(int llinkid);
void setfLink(char flinkfile[]);
int Sgn(double x);

void linkSetting()
{
	sprintf(inpfile,"%s","G:\\postgraduate\\����ģ�ͽ��\\MZ.inp");
	sprintf(rptfile,"%s","G:\\postgraduate\\����ģ�ͽ��\\HY\\1D\\0.3132132.rpt");
	sprintf(outfile,"%s","G:\\postgraduate\\����ģ�ͽ��\\HY\\1D\\0.35422121.out");
	/*sprintf(fLinkfile,"%s","G:\\postgraduate\\CODE\\Model\\couple_test\\LLink\\fL_10m.inp"); */ //�������������ļ�
	is_include_llink=0;
	is_include_flink=0;
	open1Dmodel=1;
	open2Dmodel=1;
	cw=0.065;        //between 0.53-0.67 
	co=0.4;        //between 0.39-0.46
	A_node=1.167;
	zhouchang=3.83;
	//������������������
	llinkNum1=4;
	/*llinkNum2=5;*/
	//flinkNum1=4;  //�߽��
	//flinkID=3;  //������ϱ߽���bd�ļ��е�˳�򣨴�1��ʼ��
}
int readNodeXY(char *inpFile)
{
	long  lineCount = 0;
	int   newsect,sect;
	int   inperr;
	char  line[1025];        // line from input data file
	char  wLine[1025];       // working copy of input line
	FILE* fp;
	Nnode=0;
	Njunction=0;
	fp=fopen(inpFile,"r+");     //rt�Ǵ�һ���ı��ļ����ļ�������ڣ�ֻ�������
	rewind(fp);
	while ( fgets(line, 1024, fp) != NULL )
	{
		// --- make copy of line and scan for tokens
		lineCount++;
		strcpy(wLine, line);
		Ntokens = getTokens(wLine);

		// --- skip blank lines and comments
		if ( Ntokens == 0 ) continue;
		if ( *Tok[0] == ';' ) continue;

		// --- check if at start of a new input section
		if (*Tok[0] == '[')
		{
			// --- match token against list of section keywords
			newsect = findmatch(Tok[0], SectWords);
			if (newsect >= 0)
			{
				// --- begin a new input section
				sect = newsect;
				//printf("%d\n",sect);
				continue;
			}
		 }
		else
		{
			inperr = parseLine(sect, line);    //��ȡ���ݵ�ÿ��object��ȥ
		}
	}
	   
	fclose(fp);
	printf("%s\n","��ȡһάģ�ͳɹ�");
	return 0;
}
//=============================================================================

int  getTokens(char *s)
	//
	//  Input:   s = a character string
	//  Output:  returns number of tokens found in s
	//  Purpose: scans a string for tokens, saving pointers to them
	//           in shared variable Tok[].
	//
	//  Notes:   Tokens can be separated by the characters listed in SEPSTR
	//           (spaces, tabs, newline, carriage return) which is defined
	//           in CONSTS.H. Text between quotes is treated as a single token.
	//
{
	int  len, m, n;
	char *c;

	// --- begin with no tokens
	for (n = 0; n < 40; n++) Tok[n] = NULL;
	n = 0;

	// --- truncate s at start of comment 
	c = strchr(s,';');
	if (c) *c = '\0';
	len = strlen(s);

	// --- scan s for tokens until nothing left
	while (len > 0 && n < 40)
	{
		m = strcspn(s,SEPSTR);              // find token length 
		if (m == 0) s++;                    // no token found
		else
		{
			if (*s == '"')                  // token begins with quote
			{
				s++;                        // start token after quote
				len--;                      // reduce length of s
				m = strcspn(s,"\"\n");      // find end quote or new line
			}
			s[m] = '\0';                    // null-terminate the token
			Tok[n] = s;                     // save pointer to token 
			n++;                            // update token count
			s += m+1;                       // begin next token
		}
		len -= m+1;                         // update length of s
	}
	return(n);
}

//=============================================================================

int  findmatch(char *s, char *keyword[])
	//
	//  Input:   s = character string
	//           keyword = array of keyword strings
	//  Output:  returns index of matching keyword or -1 if no match found  
	//  Purpose: finds match between string and array of keyword strings.
	//
{
	int i = 0;
	while (keyword[i] != NULL)
	{
		if (match(s, keyword[i])) return(i);
		i++;
	}
	return(-1);
}

//=============================================================================

int  match(char *str, char *substr)
	//
	//  Input:   str = character string being searched
	//           substr = sub-string being searched for
	//  Output:  returns 1 if sub-string found, 0 if not
	//  Purpose: sees if a sub-string of characters appears in a string
	//           (not case sensitive).
	//
{
	int i,j;

	// --- fail if substring is empty
	if (!substr[0]) return(0);

	// --- skip leading blanks of str
	for (i = 0; str[i]; i++)
	{
		if (str[i] != ' ') break;
	}

	// --- check if substr matches remainder of str
	for (i = i,j = 0; substr[j]; i++,j++)
	{
		if (!str[i] || UCHAR(str[i]) != UCHAR(substr[j])) return(0);
	}
	return(1);
}

//=============================================================================

int  parseLine(int sect, char *line)
	//
	//  Input:   sect  = current section of input file
	//           *line = line of text read from input file
	//  Output:  returns error code or 0 if no error found
	//  Purpose: parses contents of a tokenized line of text read from input file.
	//
{
	switch (sect)
	{
	case JUNCTION:
		Njunction++;
		//printf("%d\n",Nnode);
		return 0; 
	case COORDINATE:
		Nnode++;
		//printf("%d\n",Nnode);
		return readNode(Nnode); 
		
	}
}

//=============================================================================

int readNode(int num)
	//
	//  Input:   type = type of node
	//  Output:  returns error code
	//  Purpose: reads data for a node from a line of input.
	//
{
	int err = junc_readParams(num,Tok, Ntokens);
	return err;
}

//=============================================================================

int junc_readParams(int num, char* tok[], int ntoks)
	//
	//  Input:   j = node index
	//           k = junction index
	//           tok[] = array of string tokens
	//           ntoks = number of tokens
	//  Output:  returns an error message
	//  Purpose: reads a junction's properties from a tokenized line of input.
	//
	//  Format of input line is:
	//     nodeID  elev  maxDepth  initDepth  surDepth  aPond 
{
	int    i;
	int    err;
	// --- parse invert elev., max. depth, init. depth, surcharged depth,
	//     & ponded area values
	for ( i = 1; i <= 2; i++ )
	{
		junctions[num][i+1] = 0.0;
		if ( i < ntoks )
		{
			err=getDouble(tok[i], &junctions[num][i+1]);
		}
	}
	return 0;
}

//=============================================================================

int  getDouble(char *s, double *y)
	//
	//  Input:   s = a character string
	//  Output:  y = converted value of s,
	//           returns 1 if conversion successful, 0 if not
	//  Purpose: converts a string to a double precision floating point number.
	//
{
	char *endptr;
	*y = strtod(s, &endptr);
	if (*endptr > 0) return(0);
	return(1);
}

//=============================================================================

int Link_PointOnCell(int nodeNum)
{
	int i,j;
	int IsInThisCell;
	//int tstart,tend;
	void IsPointOnCell(double Po1,double Po2,double P11,double P12,double P21,double P22,double P31,double P32,int *IsIn);
	IsInThisCell=-1;
	//tstart=clock();
   /* #pragma omp parallel num_threads(4)
	{
		#pragma omp for*/
		for (i=1;i<=nodeNum;i++)
		{
			/*if (Node[i].IsAllowOverFlow==0)
			{
				continue;
			}*/
		
				for (j=1;j<=g_CellNum;j++)
				{
					IsPointOnCell(junctions[i][2],junctions[i][3],node_info[triangle_info[j][2]][2],node_info[triangle_info[j][2]][3],node_info[triangle_info[j][3]][2],node_info[triangle_info[j][3]][3],node_info[triangle_info[j][4]][2],node_info[triangle_info[j][4]][3],&IsInThisCell);
					if (IsInThisCell==1)
					{
						junctions[i][6]=j;  //��ȡ�ڵ����ڵ�Ԫ���
						/**************************����龮�ı����ر���ƽ5-29******/
						if (junctions[i][4]+junctions[i][5]>cell_info[j][8])
						{
							junctions[i][5]=cell_info[j][8]-junctions[i][4];
						}
						if (junctions[i][4]+junctions[i][5]<cell_info[j][8])
						{
							junctions[i][5]=cell_info[j][8]-junctions[i][4];
						}
						break;
					}
				}
		//if (Node[i].VlinkCellID==-1)
		//{
		//	Node[i].IsAllowOverFlow=0;  //���û�ж�Ӧ��Ԫ����������
		//}
		}
	/*}*/
	//tend=clock();
	//printf("%d\n",tend-tstart);
	return 0;
}
void IsPointOnCell(double Po1,double Po2,double P11,double P12,double P21,double P22,double P31,double P32,int *IsIn)
{
	double So,S1,S2,S3;
	if ((Po1>MAX(MAX(P11,P21),MAX(P21,P31)))||(Po1<MIN(MIN(P11,P21),MIN(P21,P31)))||(Po2>MAX(MAX(P12,P22),MAX(P22,P32)))||(Po2<MIN(MIN(P12,P22),MIN(P22,P32))))
	{
		*IsIn=0;
	}
	else
	{
		So=fabs(0.5*((P11-P31)*(P22-P32)-(P21-P31)*(P12-P32)));
		S3=fabs(0.5*((P11-Po1)*(P22-Po2)-(P21-Po1)*(P12-Po2)));
		S2=fabs(0.5*((P11-P31)*(Po2-P32)-(Po1-P31)*(P12-P32)));
		S1=fabs(0.5*((Po1-P31)*(P22-P32)-(P21-P31)*(Po2-P32)));
		if (fabs(So-S1-S2-S3)<0.000001)
		{
			*IsIn=1;
		}
	}
}

void setlateralLink()
{
	int cell_i;
	int lLink_i;
	int node1,node2;
	double weir1,weir2;
	void fineNearllJunction(int llinkid,int cellid);
	//int C_findlatConduit(int node1,int node2);
	lLink_i=0;
	for (cell_i=1;cell_i<=g_CellNum;cell_i++)
	{
		if (cell_info[cell_i][9]==1)
		{
			lLink_i=lLink_i+1;
			lateralLink[lLink_i][1]=lLink_i;   //���ò������ӵı��
			lateralLink[lLink_i][2]=cell_i;    //���ò������ӵ�2D cell���
		}
	}
	lLinkNum=lLink_i;  //�������ӵ���Ŀ

	for (lLink_i=1;lLink_i<=lLinkNum;lLink_i++)  //���ò������ӵ�����2d�ڵ�
	{
		node1=-1;
		node2=-1;
		if (node_type[triangle_info[(int)lateralLink[lLink_i][2]][2]]>0&&node_type[triangle_info[(int)lateralLink[lLink_i][2]][3]]>0) //�жϵ�1������
		{
			node1=triangle_info[(int)lateralLink[lLink_i][2]][2];
			node2=triangle_info[(int)lateralLink[lLink_i][2]][3];
		}
		else if (node_type[triangle_info[(int)lateralLink[lLink_i][2]][2]]>0&&node_type[triangle_info[(int)lateralLink[lLink_i][2]][4]]>0)
		{
			node1=triangle_info[(int)lateralLink[lLink_i][2]][2];
			node2=triangle_info[(int)lateralLink[lLink_i][2]][4];
		}
		else
		{
			node1=triangle_info[(int)lateralLink[lLink_i][2]][3];
			node2=triangle_info[(int)lateralLink[lLink_i][2]][4];
		}
		lateralLink[lLink_i][6]=(double)node1;
		lateralLink[lLink_i][7]=(double)node2;
	}
	for (lLink_i=1;lLink_i<=lLinkNum;lLink_i++)  //���ò������ӵ�����2d�ڵ�
	{
		lateralLink[lLink_i][10]=sqrt(pow(node_info[(int)lateralLink[lLink_i][6]][2]-node_info[(int)lateralLink[lLink_i][7]][2],2)+pow(node_info[(int)lateralLink[lLink_i][6]][3]-node_info[(int)lateralLink[lLink_i][7]][3],2));
	}
	for (lLink_i=1;lLink_i<=lLinkNum;lLink_i++) //���ò������ӵ�����river��
	{
		fineNearllJunction(lLink_i,(int)lateralLink[lLink_i][2]);  //����llink��cell�������������������river��
	}

	for (lLink_i=1;lLink_i<=lLinkNum;lLink_i++)//�����߶��߳�
	{
		weir1=(node_info[(int)lateralLink[lLink_i][6]][4]+node_info[(int)lateralLink[lLink_i][7]][4])*0.5;
		weir2=(junctions[(int)lateralLink[lLink_i][3]][4]+junctions[(int)lateralLink[lLink_i][3]][5]+junctions[(int)lateralLink[lLink_i][4]][4]+junctions[(int)lateralLink[lLink_i][4]][5])*0.5;
		lateralLink[lLink_i][5]=MAX(weir1,weir2);
		//*Ϊ�˲�����ϰ����޸�*/
		//if(cell_info[(int)lateralLink[lLink_i][2]][6]<154&&cell_info[(int)lateralLink[lLink_i][2]][6]>146) //���ڲ�����ϰ������������ӵ�Ԫ��xֵ�ж�����λ�ã������߶��߳�
		//{
		//	lateralLink[lLink_i][5]=2;
		//}
		//else
		//{
		//	lateralLink[lLink_i][5]=10;//���ô�ֵ��ֹ����λ�ô���ˮ������
		//}
		//*Ϊ�˲�����ϰ����޸�*/
	}
	for (lLink_i=1;lLink_i<=lLinkNum;lLink_i++) //���ò������ӵ�����river��
	{
		lateralLink[lLink_i][11]=C_findlatConduit((int)lateralLink[lLink_i][3]-1,(int)lateralLink[lLink_i][4]-1);//����llink������river����Ѱ�Ҷ�Ӧ�Ӷ�
		
		if (lateralLink[lLink_i][11]==-1)
		{
			printf("%s%lf\n","�������ӷ��������ڣ�",lateralLink[lLink_i][2]);

		free(IsNeedVirt);
		free(IsNodeNeedVirt);
		free(node_info_cell);
		free(node_type);
		free(cell_type);
		free(Fr_edge);
		free(triangle_info);
		free(edge_info);
		free(boundary_info);
		free(boundary_Q);   //�����߽�ϵ��
		

		free(cell_source);
		free(node_info);
		
		free(node_distance);
		free(node_distance_square);
		free(node_cha);
		free(triangle_c);
		free(cell_info);
		free(cell_grad);
		free(Previous_Result);
		
		free(edge_info_c);
		free(boundary_series);
		free(slope_con);
		free(friction_con);
		free(gradient_con);
		
		free(U);
		free(U_new);
		free(cell_MaxDepth);
		free(cell_MaxVelcity);
		free(lim_rec);
		

			exit(0);
		}
	}
	//0:-1;1:No.;2:2D����Ԫ���;3:�����river��1;4:�����river��2;5:�߶��߳�;6:�ڵ�1;7:�ڵ�2;8:2Dˮλ;9:1Dˮλ;10:�߿�;11:�Ӷ���š�
}

void setlLinkJunc()
{
	int junction_i;
	int llJunction_i;
	llJunction_i=0;
	for (junction_i=1;junction_i<=Njunction;junction_i++)
	{
		if (junctions[junction_i][6]==-1)
		{
			llJunction_i++;
			llJunction[llJunction_i][1]=junction_i;
		}
	}
	llJunctionNum=llJunction_i;
}

void fineNearllJunction(int llinkid,int cellid)
{
	int llJunction_i;
	double distance1;
	double delta_x;
	double delta_y;
	double mindis1,mindis2;
	int mindisjunc1,mindisjunc2;
	mindis1=0;
	mindis2=0;
	
	for (llJunction_i=1;llJunction_i<=llJunctionNum;llJunction_i++)
	{
		delta_x=cell_info[cellid][6]-junctions[(int)llJunction[llJunction_i][1]][2];
		delta_y=cell_info[cellid][7]-junctions[(int)llJunction[llJunction_i][1]][3];
		distance1=pow(delta_x*delta_x+delta_y*delta_y,0.5);
		if (llJunction_i==1)
		{
			mindis1=distance1;
			mindisjunc1=(int)llJunction[llJunction_i][1];
		}
		if (llJunction_i==2)
		{
			if (mindis1>=distance1)
			{
				mindis2=mindis1;
				mindisjunc2=mindisjunc1;
				mindis1=distance1;
				mindisjunc1=(int)llJunction[llJunction_i][1];
			}
			else
			{
				mindis2=distance1;
				mindisjunc2=(int)llJunction[llJunction_i][1];
			}
		}
		if (llJunction_i>2)
		{
			if (distance1<=mindis1)
			{
				mindis2=mindis1;
				mindisjunc2=mindisjunc1;
				mindis1=distance1;
				mindisjunc1=(int)llJunction[llJunction_i][1];
			}
			else if (distance1>mindis1&&distance1<mindis2)
			{
				mindis2=distance1;
				mindisjunc2=(int)llJunction[llJunction_i][1];
			}
		}
	}
	lateralLink[llinkid][3]=mindisjunc1;
	lateralLink[llinkid][4]=mindisjunc2;
}

void find1DLevel(int llinkid)
{
	double junc_x1,junc_x2,junc_y1,junc_y2;
	double cell_x,cell_y;
	double junc_a,cell_a;        //y=junc_a*x+junc_b
	double xx,yy;//��������
	double junc_b,cell_b;
	double temp1,temp_z1,temp_z2;
	junc_x1=junctions[(int)lateralLink[llinkid][3]][2];
	junc_y1=junctions[(int)lateralLink[llinkid][3]][3];       //��һ��junction��xy����
	junc_x2=junctions[(int)lateralLink[llinkid][4]][2];
	junc_y2=junctions[(int)lateralLink[llinkid][4]][3];       //�ڶ���junction��xy����
	cell_x=cell_info[(int)lateralLink[llinkid][2]][6];
	cell_y=cell_info[(int)lateralLink[llinkid][2]][7];        //lateral link��2D cell����������
	if (junc_x1==junc_x2)
	{
		xx=junc_x1;
		yy=cell_y;
	} 
	else if (junc_y1==junc_y2)
	{
		xx=cell_x;
		yy=junc_y1;
	}
	else
	{
		junc_a=(junc_y1-junc_y2)/(junc_x1-junc_x2);
		junc_b=junc_y1-(junc_y1-junc_y2)/(junc_x1-junc_x2)*junc_x1;
		cell_a=-1/junc_a;
		cell_b=cell_y-cell_a*cell_x;
		xx=(junc_b-cell_b)/(cell_a-junc_a);
		yy=cell_a*xx+cell_b;
	}
	if (xx<=MAX(junc_x1,junc_x2)&&xx>=MIN(junc_x1,junc_x2))
	{
		temp1=(pow(junc_x2-junc_x1,2)+pow(junc_y2-junc_y1,2))/(pow(xx-junc_x1,2)+pow(yy-junc_y1,2));
		temp_z1=junctions[(int)lateralLink[llinkid][3]][5]+junctions[(int)lateralLink[llinkid][3]][7];
		temp_z2=junctions[(int)lateralLink[llinkid][4]][5]+junctions[(int)lateralLink[llinkid][4]][7];
		lateralLink[llinkid][9]=temp_z1+(temp_z2-temp_z1)/temp1;
	} 
	else if (xx>MAX(junc_x1,junc_x2))
	{
		if (junc_x1>junc_x2)
		{
			lateralLink[llinkid][9]=junctions[(int)lateralLink[llinkid][3]][5]+junctions[(int)lateralLink[llinkid][3]][7];
		}
		else
		{
			lateralLink[llinkid][9]=junctions[(int)lateralLink[llinkid][4]][5]+junctions[(int)lateralLink[llinkid][4]][7];
		}
	}
	else if (xx<MIN(junc_x1,junc_x2))
	{
		if (junc_x1<junc_x2)
		{
			lateralLink[llinkid][9]=junctions[(int)lateralLink[llinkid][3]][5]+junctions[(int)lateralLink[llinkid][3]][7];
		}
		else
		{
			lateralLink[llinkid][9]=junctions[(int)lateralLink[llinkid][4]][5]+junctions[(int)lateralLink[llinkid][4]][7];
		}
	}
	
}

int Sgn(double x)  
{  
	if (x>0) return(1);          
	if (x<0) return(-1);        
	if (x==0) return (0);           
}

void setfLink(char flinkfile[])
{
	int floutf_i,flout_id,flink_cell_num,i,flink_cell_num_temp;
	int fLink_i;
	int node1,node2;
	FILE* fp;
	char temp[100];
	fp=fopen(flinkfile,"r");
	flink_cell_num=0;
	fgets(temp,99,fp);
	fscanf_s(fp,"%d\n",&flOutfallNum);
	fgetc(fp);
	for (floutf_i=1;floutf_i<=flOutfallNum;floutf_i++)
	{
		fgets(temp,99,fp);
		fscanf_s(fp,"%d",&flout_id);  //�������(��ˮ�ڱ��밴��inp�ļ���˳��������)
		flOutfallid[floutf_i]=flout_id;
		fgetc(fp);
		fgets(temp,99,fp);
		fscanf_s(fp,"%d",&flink_cell_num_temp);  //���ӵر�Ԫ����Ŀ
		fgetc(fp);
		fgets(temp,99,fp);
		for (i=1;i<=flink_cell_num_temp;i++)
		{
			fscanf_s(fp,"%lf",&flink[i+flink_cell_num][2]);  //���ӵر�Ԫ�����
			flink[i+flink_cell_num][1]=i+flink_cell_num;
			flink[i+flink_cell_num][3]=flout_id;
		}
		flink_cell_num=flink_cell_num+flink_cell_num_temp;
		fgetc(fp);
	}
	fLinkNum=flink_cell_num;
	for (fLink_i=1;fLink_i<=fLinkNum;fLink_i++)  //�������������ӵ�����2d�ڵ�
	{
		node1=-1;
		node2=-1;
		if (node_type[triangle_info[(int)flink[fLink_i][2]][2]]>0&&node_type[triangle_info[(int)flink[fLink_i][2]][3]]>0) //�жϵ�1������
		{
			node1=triangle_info[(int)flink[fLink_i][2]][2];
			node2=triangle_info[(int)flink[fLink_i][2]][3];
		}
		else if (node_type[triangle_info[(int)flink[fLink_i][2]][2]]>0&&node_type[triangle_info[(int)flink[fLink_i][2]][4]]>0)
		{
			node1=triangle_info[(int)flink[fLink_i][2]][2];
			node2=triangle_info[(int)flink[fLink_i][2]][4];
		}
		else
		{
			node1=triangle_info[(int)flink[fLink_i][2]][3];
			node2=triangle_info[(int)flink[fLink_i][2]][4];
		}
		flink[fLink_i][6]=(double)node1;
		flink[fLink_i][7]=(double)node2;
	}
	for (fLink_i=1;fLink_i<=fLinkNum;fLink_i++)  //���ò������ӵ�����2d�ڵ�
	{
		flink[fLink_i][10]=sqrt(pow(node_info[(int)flink[fLink_i][6]][2]-node_info[(int)flink[fLink_i][7]][2],2)+pow(node_info[(int)flink[fLink_i][6]][3]-node_info[(int)flink[fLink_i][7]][3],2));
	}
}



