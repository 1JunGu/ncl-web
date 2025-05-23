/*
 *      $Id: st02c.c,v 1.1 1996/04/13 02:51:49 dbrown Exp $
 */
/***********************************************************************
*                                                                      *
*                Copyright (C)  1993                                   *
*        University Corporation for Atmospheric Research               *
*                All Rights Reserved                                   *
*                                                                      *
***********************************************************************/
/*
 *  File:       st02c.c
 *
 *  Author:     David Brown
 *              National Center for Atmospheric Research
 *              PO 3000, Boulder, Colorado
 *
 *  Date:       Wed Apr  3 17:00:55 MST 1996
 *
 *   Description:   Given a simple mathematically generated data set,
 *		    demonstrates line-drawn streamline arrows and the use
 *                  of some basic StreamlinePlot resources
 */

#include <math.h>
#include <ncarg/gks.h>
#include <ncarg/ncargC.h>
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/App.h>
#include <ncarg/hlu/NcgmWorkstation.h>
#include <ncarg/hlu/PSWorkstation.h>
#include <ncarg/hlu/XWorkstation.h>
#include <ncarg/hlu/StreamlinePlot.h>


#define M 25
#define N 30
#define PI  3.14159    

main(int argc, char *argv[])
{
    int NCGM=0, X11=1, PS=0;
    int appid,wid,stid,vfid;
    int rlist,grlist;
    int len_dims[2];
    float stepsize,arrowlength,spacing;
    float U[N][M],V[N][M];

/*
 * Generate vector data arrays
 */
    {
	    float igrid, jgrid;
	    int i,j;
	    igrid = 2.0 * PI / (float) M;
	    jgrid = 2.0 * PI / (float) N;
	    for (j = 0; j < N; j++) {
		    for (i = 0; i < M; i++) {
			    U[j][i] = 10.0 * cos(jgrid * (float) j);
			    V[j][i] = 10.0 * cos(igrid * (float) i);
		    }
	    }
    }

/*
 * Initialize the high level utility library
 */
    NhlInitialize();
/*
 * Create an application context. Set the app dir to the current
 * directory so the application looks for a resource file in the working
 * directory. 
 */
    rlist = NhlRLCreate(NhlSETRL);
    grlist = NhlRLCreate(NhlGETRL);
    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNappUsrDir,"./");
    NhlCreate(&appid,"st02",NhlappClass,NhlDEFAULT_APP,rlist);

    if (NCGM) {
/*
 * Create a meta file workstation.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkMetaName,"./st02c.ncgm");
        NhlCreate(&wid,"st02Work",
                  NhlncgmWorkstationClass,NhlDEFAULT_APP,rlist);
    }
    else if (X11) {
/*
 * Create an X workstation.
 */
        NhlRLClear(rlist);
        NhlRLSetInteger(rlist,NhlNwkPause,True);
        NhlCreate(&wid,"st02Work",NhlxWorkstationClass,appid,rlist);
    }

    else if (PS) {
/*
 * Create a PS workstation.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkPSFileName,"st02c.ps");
        NhlCreate(&wid,"st02Work",NhlpsWorkstationClass,appid,rlist);
    }
/*
 * Create a VectorField data object using the data set defined above.
 * By default the array bounds will define the data boundaries (zero-based,
 * as in C language conventions)
 */

    len_dims[0] = N;
    len_dims[1] = M;
    NhlRLClear(rlist);
    NhlRLSetMDFloatArray(rlist,NhlNvfUDataArray,&U[0][0],2,len_dims);
    NhlRLSetMDFloatArray(rlist,NhlNvfVDataArray,&V[0][0],2,len_dims);
    NhlCreate(&vfid,"vectorfield",NhlvectorFieldClass,appid,rlist);


/*
 * Create a StreamlinePlot object, supplying the VectorField object as data
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,
		   "Modifying StreamlinePlot resources");
    NhlRLSetInteger(rlist,NhlNstVectorFieldData,vfid);
    NhlCreate(&stid,"streamlineplot",NhlstreamlinePlotClass,wid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);

/* 
 * Get the values of several resources that are set dynamically based
 * on the assumed NDC size of a grid cell. Each of this will be separately
 * modified in the course of this example to illustrate their effect.
 */
      
    NhlRLClear(grlist);
    NhlRLGetFloat(grlist,NhlNstStepSizeF,&stepsize);
    NhlRLGetFloat(grlist,NhlNstArrowLengthF,&arrowlength);
    NhlRLGetFloat(grlist,NhlNstMinLineSpacingF,&spacing);
    NhlGetValues(stid,grlist);

/* 
 * Increase the step size 
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,"Larger Step Size");
    NhlRLSetFloat(rlist,NhlNstStepSizeF,stepsize * 4.0);
    NhlSetValues(stid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);
/* 
 * Decrease the step size 
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,"Smaller Step Size");
    NhlRLSetFloat(rlist,NhlNstStepSizeF,stepsize * 0.25);
    NhlSetValues(stid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);

/* 
 * Increase the minimum line spacing 
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,"Larger Minimum Line Spacing");
    NhlRLSetFloat(rlist,NhlNstStepSizeF,stepsize);
    NhlRLSetFloat(rlist,NhlNstMinLineSpacingF, spacing * 4.0);
    NhlSetValues(stid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);
/* 
 * Decrease the minimum line spacing
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,"Smaller Minimum Line Spacing");
    NhlRLSetFloat(rlist,NhlNstMinLineSpacingF,spacing * 0.25);
    NhlSetValues(stid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);

/* 
 * Increase the line starting grid stride 
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,"Larger Line Starting Grid Stride");
    NhlRLSetFloat(rlist,NhlNstMinLineSpacingF,spacing);
    NhlRLSetInteger(rlist,NhlNstLineStartStride,3);
    NhlSetValues(stid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);
/* 
 * Decrease the line starting grid stride
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,"Smaller Line Starting Grid Stride");
    NhlRLSetInteger(rlist,NhlNstLineStartStride,1);
    NhlSetValues(stid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);

/*
 * Increase the arrow size
 */

    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNtiMainString,"Larger Arrows");
    NhlRLSetInteger(rlist,NhlNstLineStartStride,2);
    NhlRLSetFloat(rlist,NhlNstArrowLengthF, arrowlength * 2.0);
    NhlSetValues(stid,rlist);

    NhlDraw(stid);
    NhlFrame(wid);

/*
 * Destroy the objects created, close the HLU library and exit.
 */
    NhlDestroy(appid);
    NhlClose();
    exit(0);
}
