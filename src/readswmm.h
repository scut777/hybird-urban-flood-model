//-----------------------------------------------------------------------------
//   readSWMM.h
//
//   Project: OFM
//   Version: 1.0
//   Date:    11/09/21   (Build 5.0.010)         
//   Author:  W. Chen
//
//
//-----------------------------------------------------------------------------
#include <stdlib.h>
#ifndef _READSWMM_H_                            //��ֹ�ظ�����
#define _READSWMM_H_
#define   SEPSTR    " \t\n\r" 
#define UCHAR(x) (((x) >= 'a' && (x) <= 'z') ? ((x)&~32) : (x))
/* uppercase char of x   */
//#define MIN(x,y) (((x)<=(y)) ? (x) : (y))        /* minimum of x and y    */
//#define MAX(x,y) (((x)>=(y)) ? (x) : (y))        /* maximum of x and y    */
// Input File Sections
#define  ws_TITLE            "[TITLE"
#define  ws_OPTION           "[OPTION"
#define  ws_FILE             "[FILE"
#define  ws_RAINGAGE         "[RAINGAGE"
#define  ws_TEMP             "[TEMPERATURE"
#define  ws_EVAP             "[EVAP"
#define  ws_SUBCATCH         "[SUBCATCHMENT"
#define  ws_SUBAREA          "[SUBAREA"
#define  ws_INFIL            "[INFIL"
#define  ws_AQUIFER          "[AQUIFER"
#define  ws_GROUNDWATER      "[GROUNDWATER"
#define  ws_SNOWMELT         "[SNOWPACK"
#define  ws_JUNCTION         "[JUNC"
#define  ws_OUTFALL          "[OUTFALL"
#define  ws_STORAGE          "[STORAGE"
#define  ws_DIVIDER          "[DIVIDER"
#define  ws_CONDUIT          "[CONDUIT"
#define  ws_PUMP             "[PUMP"
#define  ws_ORIFICE          "[ORIFICE"
#define  ws_WEIR             "[WEIR"
#define  ws_OUTLET           "[OUTLET"
#define  ws_XSECTION         "[XSECT"
#define  ws_TRANSECT         "[TRANSECT"
#define  ws_LOSS             "[LOSS"
#define  ws_CONTROL          "[CONTROL"
#define  ws_POLLUTANT        "[POLLUT"
#define  ws_LANDUSE          "[LANDUSE"
#define  ws_BUILDUP          "[BUILDUP"
#define  ws_WASHOFF          "[WASHOFF"
#define  ws_COVERAGE         "[COVERAGE"
#define  ws_INFLOW           "[INFLOW"
#define  ws_DWF              "[DWF"
#define  ws_PATTERN          "[PATTERN"
#define  ws_RDII             "[RDII"
#define  ws_UNITHYD          "[HYDROGRAPH"
#define  ws_LOADING          "[LOADING"
#define  ws_TREATMENT        "[TREATMENT"
#define  ws_CURVE            "[CURVE"
#define  ws_TIMESERIES       "[TIMESERIES"
#define  ws_REPORT           "[REPORT"
#define  ws_MAP              "[MAP"
#define  ws_COORDINATE       "[COORDINATE"
#define  ws_VERTICES         "[VERTICES"
#define  ws_POLYGON          "[POLYGON"
#define  ws_SYMBOL           "[SYMBOL"
#define  ws_LABEL            "[LABEL"
#define  ws_BACKDROP         "[BACKDROP"
#define  ws_TAG              "[TAG"
#define  ws_PROFILE          "[PROFILE"
#define  ws_LID_CONTROL      "[LID_CONTROL"
#define  ws_LID_USAGE        "[LID_USAGE"
#define  ws_GW_FLOW          "[GW_FLOW"     //Deprecated                       //(5.1.007)
#define  ws_GWF              "[GWF"                                            //(5.1.007)
#define  ws_ADJUST           "[ADJUSTMENT"                                     //(5.1.007)

//extern double junctions[10000][10];//0:ID;1:name;2:X;3:Y;4:fulldepth;5:invertElvt;6:assigned cellID;7:water depth;8:exchanged discharges;9:volume
char   *Tok[40]; 
int    Ntokens;
//extern int	Nnode;
//extern int  Njunction;
char* SectWords[]          = {  ws_TITLE,          ws_OPTION,
								ws_FILE,           ws_RAINGAGE,
								ws_TEMP,           ws_EVAP, 
								ws_SUBCATCH,       ws_SUBAREA,
								ws_INFIL,          ws_AQUIFER,
								ws_GROUNDWATER,    ws_SNOWMELT,
								ws_JUNCTION,       ws_OUTFALL,
								ws_STORAGE,        ws_DIVIDER,
								ws_CONDUIT,        ws_PUMP,
								ws_ORIFICE,        ws_WEIR,
								ws_OUTLET,         ws_XSECTION,
								ws_TRANSECT,       ws_LOSS,
								ws_CONTROL,        ws_POLLUTANT,
								ws_LANDUSE,        ws_BUILDUP,
								ws_WASHOFF,        ws_COVERAGE,
								ws_INFLOW,         ws_DWF,
								ws_PATTERN,        ws_RDII, 
								ws_UNITHYD,        ws_LOADING,
								ws_TREATMENT,      ws_CURVE,
								ws_TIMESERIES,     ws_REPORT,
								ws_COORDINATE,     ws_VERTICES,
								ws_POLYGON,        ws_LABEL,
								ws_SYMBOL,         ws_BACKDROP, 
								ws_TAG,            ws_PROFILE,
								ws_MAP,            ws_LID_CONTROL,
								ws_LID_USAGE,      ws_GWF,                      //(5.1.007)
								ws_ADJUST,        NULL};                       //(5.1.007)

#endif