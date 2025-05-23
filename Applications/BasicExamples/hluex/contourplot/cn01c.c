/*
 *      $Id: cn01c.c,v 1.7 1996/01/04 16:46:34 haley Exp $
 */
/************************************************************************
*                                                                       *
*                Copyright (C)  1993                                    *
*        University Corporation for Atmospheric Research                *
*                All Rights Reserved                                    *
*                                                                       *
************************************************************************/
/*
 *  File:       cn01c.c
 *
 *  Author:     David Brown
 *          National Center for Atmospheric Research
 *          PO 3000, Boulder, Colorado
 *
 *  Date:       Tue Oct  4 18:48:08 MDT 1994
 *
 *  Description:    Given a simple mathematically generated data set,
 *                  demonstrates ContourPlot with all resources (other than
 *                  cnScalarFieldData) set to their default value.
 */

#include <math.h>
#include <stdio.h>
#include <ncarg/gks.h>
#include <ncarg/ncargC.h>
#include <ncarg/hlu/hlu.h>

/*
 * Include a header file for each object created
 */

#include <ncarg/hlu/App.h>
#include <ncarg/hlu/XWorkstation.h>
#include <ncarg/hlu/NcgmWorkstation.h>
#include <ncarg/hlu/PSWorkstation.h>
#include <ncarg/hlu/ScalarField.h>
#include <ncarg/hlu/ContourPlot.h>

static const int M=25, N=25;

main(int argc, char *argv[])
{
    float   T[25 * 25];
    int     len_dims[2];
    int     appid,wid,dataid,cnid;
    int     srlist;
    float   x,y;
    int     i,j;
    int     NCGM=0, X11=1, PS=0;

/* create a simple bull's eye pattern test data set */

#define PI  3.14159
    for (i=-N/2;i<=N/2;i++) {
        for (j=-M/2;j<=M/2;j++) {
            x = 8.0 * i;
            y = 8.0 * j;
            *(T+(M*(i+N/2)+j+M/2)) = 
                100.0 - sqrt((double)(x*x + y*y));
        }
    }
/*
 * Initialize the high level utility library
 */
    NhlInitialize();
/*
 * Create an application context. Set the app dir to the current directory
 * so the application looks for a resource file in the working directory.
 * In this example the resource file supplies the plot title only.
 */
    srlist = NhlRLCreate(NhlSETRL);
    NhlRLClear(srlist);
    NhlRLSetString(srlist,NhlNappUsrDir,"./");
    NhlCreate(&appid,"cn01",NhlappClass,NhlDEFAULT_APP,srlist);

    if (NCGM) {
/*
 * Create a meta file workstation.
 */
        NhlRLClear(srlist);
        NhlRLSetString(srlist,NhlNwkMetaName,"./cn01c.ncgm");
        NhlCreate(&wid,"cn01Work",
                  NhlncgmWorkstationClass,appid,srlist);
    }
    else if (X11) {
/*
 * Create an X workstation.
 */
        NhlRLClear(srlist);
        NhlRLSetInteger(srlist,NhlNwkPause,True);
        NhlCreate(&wid,"cn01Work",NhlxWorkstationClass,appid,srlist);
    }
    else if (PS) {
/*
 * Create a meta file workstation.
 */
        NhlRLClear(srlist);
        NhlRLSetString(srlist,NhlNwkPSFileName,"./cn01c.ps");
        NhlCreate(&wid,"cn01Work",NhlpsWorkstationClass,appid,srlist);
    }
/*
 * Create a ScalarField data object using the data set defined above.
 * By default the array bounds will define the data boundaries (zero-based,
 * as in C language conventions)
 */

    NhlRLClear(srlist);
    len_dims[0] = N;
    len_dims[1] = M;
    NhlRLSetMDFloatArray(srlist,NhlNsfDataArray,T,2,len_dims);
    NhlCreate(&dataid,"bullseye",NhlscalarFieldClass,appid,
              srlist);
/*
 * Create a ContourPlot object, supplying the ScalarField object as data
 */
    NhlRLClear(srlist);
    NhlRLSetInteger(srlist,NhlNcnScalarFieldData,dataid);
    NhlCreate(&cnid,"ContourPlot1",NhlcontourPlotClass,wid,srlist);

/*
 * Draw a plot illustrating the basic default behavior of
 * the ContourPlot object. The contours appear as solid lines with unboxed
 * labels in a linear coordinate system with the origin at the lower left. 
 * Tickmarks with labels show the data coordinate range, and an 
 * informational label at the lower right gives the minimum and maximum
 * data values and the contour interval spacing.
 */
    NhlDraw(cnid);
    NhlFrame(wid);
    NhlDestroy(dataid);
/*
 * Destroy the objects created, close the HLU library and exit.
 */
    NhlDestroy(cnid);
    NhlDestroy(wid);
    NhlDestroy(appid);

    NhlClose();
    exit(0);
}
