#include "math.h"
#include <stdio.h>
#include "Global.h"
#include "FUNC.h"
#include <iostream>
const double pi=3.14159265358979,g=9.8;

// Computational function for rotation
int _rotate(double *q, double n1, double n2) {
  /*Rotate the last  2 coordinates of q (q[1], q[2])
    from x,y coordinates to coordinates based on normal vector (n1, n2).

    Result is returned in array 2x1 r
    To rotate in opposite direction, call rotate with (q, n1, -n2)

    Contents of q are changed by this function */


  double q1, q2;

  // Shorthands
  q1 = q[1];  // x coordinate
  q2 = q[2];  // y coordinate

  // Rotate
  q[1] =  n1*q1 + n2*q2;
  q[2] = -n2*q1 + n1*q2;

  return 0;
}

//roe格式通量求解
void Riemann_states (int edge_ID, int *Edge_L,double *h_left, double *h_right, double *hu_right, double *hu_left, double *hv_left, double *hv_right, double *b_i,double *b_j,double *h_i, double *h_j,double *n1, double *n2,double *b_face, double *water_level_i, double *del_b)
                  
{
	int  cell_l, cell_r, Edge_R,edge_id_r;
	
	
	double water_level_left, water_level_right,  water_level_j, b_left, b_right,r_x,r_y,bb_left,bb_right,r_x_r,r_y_r;
    double q_left[3],q_right[3];
	

	
	cell_l = edge_info[edge_ID][2];
	cell_r = edge_info[edge_ID][3];
	
	/*6.16+++*/
	r_x=edge_info_c[edge_ID][4];   //位置向量X  
	r_y=edge_info_c[edge_ID][5];   //位置向量Y

    /*6.16++*/

	*Edge_L = edge_info[edge_ID][7];
    Edge_R = edge_info[edge_ID][8];
	edge_id_r= 3*cell_r-3+Edge_R;
	r_x_r=edge_info_c[edge_id_r][4];
	r_y_r=edge_info_c[edge_id_r][5];

	bb_left=cell_info[cell_l][8]+(cell_grad[cell_l][1]*r_x+cell_grad[cell_l][2]*r_y)*lim_rec[cell_l];   //计算边界中点左右两侧的高程值

	bb_right=cell_info[cell_r][8]+(cell_grad[cell_r][1]*r_x_r+cell_grad[cell_r][2]*r_y_r)*lim_rec[cell_r];
	*del_b=bb_right-bb_left;
	


	


    *n1 = edge_info_c[edge_ID][1];
	*n2 = edge_info_c[edge_ID][2];


   //一阶精度，未重构变量值的
	q_left[0] =U[cell_l][1];    //h u v 
	q_right[0] =U[cell_r][1];

	if (U[cell_l][1]<=h_dry)                    
   {
	   q_left[1]=0;
	   q_left[2]=0;
   }
	if(U[cell_l][1]>h_dry)
   {
       q_left[1] =U[cell_l][2]/U[cell_l][1];
	   q_left[2] =U[cell_l][3]/U[cell_l][1];
   }

	if (U[cell_r][1]<=h_dry)             
   {
	   q_right[1]=0;
	   q_right[2]=0;
   }
	if(U[cell_r][1]>h_dry)
   {
       q_right[1] =U[cell_r][2]/U[cell_r][1];
	   q_right[2] =U[cell_r][3]/U[cell_r][1];
   }

	
	






	
  // 边界左右两侧守恒量的值
	*h_i = q_left[0];
	*h_j = q_right[0];


	// SRM 水面重构
	*water_level_i =U[cell_l][1]+cell_info[cell_l][8];
	water_level_j = U[cell_r][1]+cell_info[cell_r][8];
	
	*b_i= cell_info[cell_l][8];
	*b_j= cell_info[cell_r][8];
	water_level_left = *water_level_i+MAX(0,MIN(*b_j-*b_i-*del_b,water_level_j-*water_level_i));
	water_level_right= water_level_j+MAX(0,MIN(*b_i-*b_j+*del_b,*water_level_i-water_level_j));
	
	b_left = water_level_left - *h_i;
	b_right= water_level_right- *h_j;
	*b_face = MAX(b_left,b_right);

	//Riemann states of the flow depth
	*h_left = MAX(0,water_level_left-*b_face);
	*h_right= MAX(0,water_level_right-*b_face);
    


	*hu_left = q_left[1] * *h_left;
    *hv_left = q_left[2] * *h_left;

	
	*hu_right = q_right[1] * *h_right;
    *hv_right = q_right[2] * *h_right;

	/*u_left = hu_left/h_left;
	u_right= hu_right/h_right;
	v_left = hv_left/h_left;
	v_right= hv_right/h_right;
*/
}




void Roe (int Edge_L,double h_left, double h_right, double hu_left, double hu_right, double hv_left, double hv_right,double n1, double n2, double *flux_1, double *flux_2, double *flux_3)
{  
	int i,j;
	double c_roe;
	double eigenvals[3], alpha[3],flux_right[4], flux_left[4], flux_Jacob[4];
	// C=SQRT(0.5*G*H)的roe平均
   
	c_roe = sqrt(0.5*g*(h_right+h_left));
	

	// critical flow fix
	//
	//
	//
  // 特征值
	eigenvals[0]= c_roe;
	eigenvals[1]= 0;
	eigenvals[2]= -c_roe;

  // 特征向量（3*3）
	double eigenvec[3][3] = {{1,0,1},{c_roe*n1,-c_roe*n2,-c_roe*n1},{c_roe*n2,c_roe*n1,-c_roe*n2}};
	
  // α的值用于，roe的简便算法	
   if(c_roe>0)
   {
	alpha[0] = 0.5*(h_right - h_left) + 1/(2*c_roe)*((hu_right-hu_left)*n1+(hv_right-hv_left)*n2);//注意这个0.25这个系数，
	alpha[1] = (1/c_roe)*((hv_right-hv_left)*n1-(hu_right-hu_left)*n2);//注意这个0.5这个系数，
	alpha[2] = 0.5*(h_right - h_left) - 1/(2*c_roe)*((hu_right-hu_left)*n1+(hv_right-hv_left)*n2);//注意这个0.25这个系数，
	
	

	

  // 内测单元每个边roe平均	
	for (j=1;j<=3;j++)
	{
			flux_Jacob[j]= 0;				
	}
	for (j=1;j<=3;j++)
	{
		for (i=0;i<=2;i++)
		{
			flux_Jacob[j] += fabs(eigenvals[i])*alpha[i]*eigenvec[j-1][i];			
		}
	}

  // 每个边的数值通量计算
	flux_left[1] =	hu_left*n1+hv_left*n2;
    flux_left[2] = 0.5*g*h_left*h_left*n1;
    flux_left[3] = 0.5*g*h_left*h_left*n2;
	

	flux_right[1] = hu_right*n1+hv_right*n2;
    flux_right[2] = 0.5*g*h_right*h_right*n1;
    flux_right[3] = 0.5*g*h_right*h_right*n2;

	*flux_1 = 0.5*(flux_left[1]+flux_right[1]-flux_Jacob[1]);
	*flux_2 = 0.5*(flux_left[2]+flux_right[2]-flux_Jacob[2]);
	*flux_3 = 0.5*(flux_left[3]+flux_right[3]-flux_Jacob[3]);
   }

   if (c_roe==0)
   {
		*flux_1 = 0;
		*flux_2 = 0;
		*flux_3 = 0;
   }
}
	
 
void bed_slope (double n1, double n2, double h_j,double h_i,double b_i, double h_left, double b_face, double water_level_i, double del_b, double *S_b1, double *S_b2, double *S_b3)
{		
	//wet-dry
	double b_face2=0;
	if (h_j<h_dry)
	{
		b_face2 = b_face-MAX(0,b_face-water_level_i);

	}
	
	else if (h_j>=h_dry)
	{
		b_face2 =b_face- MAX(0,MIN(del_b,b_face-water_level_i));
	}

	*S_b1= 0 ;
	*S_b2= 0.5*g*(h_i+h_left)*(b_i-b_face2)*n1 ;
	*S_b3= 0.5*g*(h_i+h_left)*(b_i-b_face2)*n2 ;
}

void GWM(int Edge_L,double h_left, double h_right, double hu_left, double hu_right, double hv_left, double hv_right,double n1, double n2, double *flux_1, double *flux_2, double *flux_3)
{
	int i,j;
	double c_roe;
	c_roe = sqrt(0.5*g*(h_right+h_left));

	if (c_roe==0)
   {
		*flux_1 = 0;
		*flux_2 = 0;
		*flux_3 = 0;
   }

	if (c_roe>0)
	{

	
		*flux_1 = (h_left-h_right)*c_roe+(hu_right-hu_left)*n1+(hv_right-hv_left)*n2;
		*flux_2 = 0.5*g*n1*(h_left*h_left+h_right*h_right)+c_roe*n1*n1*(hu_left+hu_right)+c_roe*n1*n2*(hv_left+hv_right);
		*flux_3 = 0.5*g*n2*(h_left*h_left+h_right*h_right)+c_roe*n2*n2*(hv_left+hv_right)+c_roe*n1*n2*(hu_left+hu_right);


	}


}






  void Roe_swm (int Edge_L,double h_left, double h_right, double hu_left, double hu_right, double hv_left, double hv_right,double n1, double n2, double *flux_1, double *flux_2, double *flux_3)
{  
	int i,j;
	double c_roe,u_roe,v_roe;
	double eigenvals[3], alpha[3],flux_right[4], flux_left[4], flux_Jacob[4];
	double chuL (double x, double y);
	// C=SQRT(0.5*G*H)的roe平均
   
	c_roe = sqrt(0.5*g*(h_right+h_left));
	if(h_right==0 & h_left!=0)
	{
		u_roe = hu_left/h_left;
		v_roe = hv_left/h_left;
	}
	else if(h_right!=0 & h_left==0)
	{
		u_roe = hu_right/h_right;
		v_roe = hv_right/h_right;
	}
	else
	{
		u_roe = (hu_right/sqrt(h_right)+hu_left/sqrt(h_left))/(sqrt(h_right)+sqrt(h_left));
		v_roe = (hv_right/sqrt(h_right)+hv_left/sqrt(h_left))/(sqrt(h_right)+sqrt(h_left));
	}
	// critical flow fix
	//
	//
	//
  // 特征值
	eigenvals[0]= u_roe*n1+v_roe*n2-c_roe;
	eigenvals[1]= u_roe*n1+v_roe*n2;
	eigenvals[2]= u_roe*n1+v_roe*n2+c_roe;

  // 特征向量（3*3）
	double eigenvec[3][3] = {{1,0,1},{u_roe-c_roe*n1,-c_roe*n2,u_roe+c_roe*n1},{v_roe-c_roe*n2,c_roe*n1,v_roe+c_roe*n2}};

	
  // α的值用于，roe的简便算法	
   if(c_roe>0)
   {
	alpha[0] = 0.5*(h_right - h_left) - 1/(2*c_roe)*((hu_right-hu_left)*n1+(hv_right-hv_left)*n2-(u_roe*n1+v_roe*n2)*(h_right - h_left));//注意这个0.25这个系数，
	alpha[1] = (1/c_roe)*(((hv_right-hv_left)-v_roe*(h_right - h_left))*n1-((hu_right-hu_left)-u_roe*(h_right - h_left))*n2);//注意这个0.5这个系数，
	alpha[2] = 0.5*(h_right - h_left) + 1/(2*c_roe)*((hu_right-hu_left)*n1+(hv_right-hv_left)*n2-(u_roe*n1+v_roe*n2)*(h_right - h_left));//注意这个0.25这个系数，
	
	

	

  // 内测单元每个边roe平均	
	for (j=1;j<=3;j++)
	{
			flux_Jacob[j]= 0;				
	}
	for (j=1;j<=3;j++)
	{
		for (i=0;i<=2;i++)
		{
			flux_Jacob[j] += fabs(eigenvals[i])*alpha[i]*eigenvec[j-1][i];			
		}
	}

  // 每个边的数值通量计算
	flux_left[1] = hu_left*n1+hv_left*n2;
    flux_left[2] = (0.5*g*h_left*h_left)*n1+hu_left*(u_roe*n1+v_roe*n2);
    flux_left[3] = (0.5*g*h_left*h_left)*n2+hv_left*(u_roe*n1+v_roe*n2);

	flux_right[1] = hu_right*n1+hv_right*n2;
    flux_right[2] = (0.5*g*h_right*h_right)*n1+hu_right*(u_roe*n1+v_roe*n2);
	flux_right[3] = (0.5*g*h_right*h_right)*n2+hv_right*(u_roe*n1+v_roe*n2);



	*flux_1 = 0.5*(flux_left[1]+flux_right[1]-flux_Jacob[1]);
	*flux_2 = 0.5*(flux_left[2]+flux_right[2]-flux_Jacob[2]);
	*flux_3 = 0.5*(flux_left[3]+flux_right[3]-flux_Jacob[3]);
   }

   if (c_roe==0)
   {
		*flux_1 = 0;
		*flux_2 = 0;
		*flux_3 = 0;
   }
}