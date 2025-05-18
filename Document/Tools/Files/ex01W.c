


#include <stdio.h>
/*
* The following are the required NCAR Graphics include files.
* They should be located in ${NCARG_ROOT}/include
*/
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/NresDB.h>
#include <ncarg/ncl/defs.h>
#include <ncarg/ncl/NclDataDefs.h>
#include <ncarg/ncl/NclBuiltInSupport.h>
#include <ncarg/gks.h>
#include <ncarg/ncl/NclBuiltIns.h>





NhlErrorTypes cquad_W( void ) {
	int i;
	float *a;
	float *b;
	float *c;
	int *nq;
	float *x;
	float *quad;
	long x_dimsizes[NCL_MAX_DIMENSIONS];
	int x_ndims;
	long quad_dimsizes[NCL_MAX_DIMENSIONS];
	int quad_ndims;

	quad = (float*) NclGetArgValue(
		5,
		6,
		&quad_ndims,
		quad_dimsizes,
		NULL,
		NULL,
		NULL,
		1);


	x = (float*) NclGetArgValue(
		4,
		6,
		&x_ndims,
		x_dimsizes,
		NULL,
		NULL,
		NULL,
		1);


	nq = (int*) NclGetArgValue(
		3,
		6,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		1);


	c = (float*) NclGetArgValue(
		2,
		6,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		1);


	b = (float*) NclGetArgValue(
		1,
		6,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		1);


	a = (float*) NclGetArgValue(
		0,
		6,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		1);

	if(*nq != (int)x_dimsizes[0]) {
		NhlPError(NhlFATAL,NhlEUNKNOWN,"cquad: dimension size of dimension (0) of x must be equal to the value of nq");
		return(NhlFATAL);
	}
	if(*nq != (int)quad_dimsizes[0]) {
		NhlPError(NhlFATAL,NhlEUNKNOWN,"cquad: dimension size of dimension (0) of quad must be equal to the value of nq");
		return(NhlFATAL);
	}
	NGCALLF(cquad,CQUAD)(a,b,c,nq,x,quad);

	return(NhlNOERROR);
}
extern float NGCALLF(arcln,ARCLN)();


NhlErrorTypes arcln_W( void ) {
	int i;
	float arcln_ret;
	long arcln_ret_dimsizes[NCL_MAX_DIMENSIONS];
	int *numpnt;
	float *pointx;
	float *pointy;
	long pointx_dimsizes[NCL_MAX_DIMENSIONS];
	int pointx_ndims;
	long pointy_dimsizes[NCL_MAX_DIMENSIONS];
	int pointy_ndims;

	pointy = (float*) NclGetArgValue(
		2,
		3,
		&pointy_ndims,
		pointy_dimsizes,
		NULL,
		NULL,
		NULL,
		1);


	pointx = (float*) NclGetArgValue(
		1,
		3,
		&pointx_ndims,
		pointx_dimsizes,
		NULL,
		NULL,
		NULL,
		1);


	numpnt = (int*) NclGetArgValue(
		0,
		3,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		1);

	if(*numpnt != (int)pointx_dimsizes[0]) {
		NhlPError(NhlFATAL,NhlEUNKNOWN,"arcln: dimension size of dimension (0) of pointx must be equal to the value of numpnt");
		return(NhlFATAL);
	}
	if(*numpnt != (int)pointy_dimsizes[0]) {
		NhlPError(NhlFATAL,NhlEUNKNOWN,"arcln: dimension size of dimension (0) of pointy must be equal to the value of numpnt");
		return(NhlFATAL);
	}
	arcln_ret = NGCALLF(arcln,ARCLN)(numpnt,pointx,pointy);

	arcln_ret_dimsizes[0] = 1;
	return(NclReturnValue(&arcln_ret,1,arcln_ret_dimsizes,NULL,NCL_float,1));
}



void Init(void){
	void *args;
	long dimsizes[NCL_MAX_DIMENSIONS];
	int nargs;


	nargs = 0;
	args = NewArgs(3);
	dimsizes[0] = -1;
	SetArgTemplate(args,2,"float",1,dimsizes);nargs++;
	dimsizes[0] = -1;
	SetArgTemplate(args,1,"float",1,dimsizes);nargs++;
	dimsizes[0] = 1;
	SetArgTemplate(args,0,"integer",1,dimsizes);nargs++;
	NclRegisterFunc(arcln_W,args,"ARCLN",nargs);

	NclRegisterFunc(arcln_W,args,"arcln",nargs);

	nargs = 0;
	args = NewArgs(6);
	dimsizes[0] = -1;
	SetArgTemplate(args,5,"float",1,dimsizes);nargs++;
	dimsizes[0] = -1;
	SetArgTemplate(args,4,"float",1,dimsizes);nargs++;
	dimsizes[0] = 1;
	SetArgTemplate(args,3,"integer",1,dimsizes);nargs++;
	dimsizes[0] = 1;
	SetArgTemplate(args,2,"float",1,dimsizes);nargs++;
	dimsizes[0] = 1;
	SetArgTemplate(args,1,"float",1,dimsizes);nargs++;
	dimsizes[0] = 1;
	SetArgTemplate(args,0,"float",1,dimsizes);nargs++;
	NclRegisterProc(cquad_W,args,"CQUAD",nargs);

	NclRegisterProc(cquad_W,args,"cquad",nargs);

}
