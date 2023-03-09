//-----------------------------------------------------------------------------
//   swmm5.h
//
//   Project: EPA SWMM5
//   Version: 5.1
//   Date:    03/24/14  (Build 5.1.001)
//   Author:  L. Rossman
//
//   Prototypes for SWMM5 functions exported to swmm5.dll.
//
//-----------------------------------------------------------------------------
#ifndef SWMM5_H
#define SWMM5_H

// --- define WINDOWS

#undef WINDOWS
#ifdef _WIN32
  #define WINDOWS
#endif
#ifdef __WIN32__
  #define WINDOWS
#endif

// --- define DLLEXPORT

#ifdef WINDOWS
  #define DLLEXPORT __declspec(dllexport) __stdcall
#else
  #define DLLEXPORT
#endif

// --- use "C" linkage for C++ programs

#ifdef __cplusplus
extern "C" { 
#endif 

int  DLLEXPORT   swmm_run(char* f1, char* f2, char* f3);
int  DLLEXPORT   swmm_open(char* f1, char* f2, char* f3);
int  DLLEXPORT   swmm_start(int saveFlag);
int  DLLEXPORT   swmm_step(double* elapsedTime);
int  DLLEXPORT   swmm_end(void);
int  DLLEXPORT   swmm_report(void);
int  DLLEXPORT   swmm_getMassBalErr(float* runoffErr, float* flowErr,
                 float* qualErr);
int  DLLEXPORT   swmm_close(void);
int  DLLEXPORT   swmm_getVersion(void);
void  DLLEXPORT  C_link(int NodeID,double *fulldepthORelapsedtime,double *invertElvtORtimeStep);
int  DLLEXPORT   C_swmm_2D(int NodeID,double *node_depth_to_2D,double *node_volume);
int  DLLEXPORT   C_2D_swmm(int NodeID,double exchanged_flow_to_swmm);
int  DLLEXPORT   C_test(int NodeID);
void  DLLEXPORT  resetOutfall(int outID,double fixlevel);
double  DLLEXPORT getNodeinflowQ(int nodeID);
void  DLLEXPORT  C_resetRoutingStep(int include2D, double routeStep2D);
void  DLLEXPORT C_llinkQ(int conduitID,double llQ);
int DLLEXPORT C_findlatConduit(int node1,int node2);
int DLLEXPORT LinkNode12(int conduitID);  //10.30 ¼Ó
void DLLEXPORT llinkQ_initial();//4.20+

#ifdef __cplusplus 
}   // matches the linkage specification from above */ 
#endif

#endif
