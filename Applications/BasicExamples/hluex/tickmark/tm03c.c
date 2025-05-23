/***********************************************************************
*                                                                      *
*                Copyright (C)  1995                                   *
*        University Corporation for Atmospheric Research               *
*                All Rights Reserved                                   *
*                                                                      *
***********************************************************************/
/*
 *      File:           tm03c.c
 *
 *      Author:         Bob Lackman
 *          National Center for Atmospheric Research
 *          PO 3000, Boulder, Colorado
 *
 *      Date:           Fri Jan 06 18:31:18 MDT 1995
 *
 *      Description:    Demonstrates the TickMark Object
 *                      with reversed and log y axis.
 */
#include <stdio.h>
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/ResList.h>
#include <ncarg/hlu/App.h>
#include <ncarg/hlu/TickMark.h>
#include <ncarg/hlu/XWorkstation.h>
#include <ncarg/hlu/NcgmWorkstation.h>
#include <ncarg/hlu/PSWorkstation.h>

#include "tm03c.h"

/*
 * Define label strings for EXPLICIT mode tick mark placement
 */
char *labels[] = { "90:S:o:N:S", "60:S:o:N:S", "30:S:o:N:S", "EQ", 
            "30:S:o:N:N", "60:S:o:N:N", "90:S:o:N:N" };
/*
 * Specify data locations for above labels
 */
float labellocs[] = { -90.0, -60.0, -30.0, 0.0, 30.0, 60.0, 90.0 };
        
main()
{
    int appid, wid, pid;
    int rlist;
    int NCGM=0, X11=1, PS=0;
/*
 * Initialize the high level utility library
 */
    NhlInitialize();
/*
 * Create an application context. Set the app dir to the current
 * directory so the application looks for a resource file in the
 * working directory. In this example the resource file supplies the
 * plot title only.
 */
    rlist = NhlRLCreate(NhlSETRL);
    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNappUsrDir,"./");
    NhlRLSetString(rlist,NhlNappDefaultParent,"True");
    NhlCreate(&appid,"tm03",NhlappClass,NhlDEFAULT_APP,rlist);

    if (NCGM) {
/*
 * Create a meta file workstation object.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkMetaName,"./tm03c.ncgm");
        NhlCreate(&wid,"tm03Work",NhlncgmWorkstationClass,
                  NhlDEFAULT_APP,rlist);
    }
    else if (X11) {
/*
 * Create an XWorkstation object.
 */
        NhlRLClear(rlist);
        NhlRLSetInteger(rlist,NhlNwkPause,True);
        NhlCreate(&wid,"tm03Work",NhlxWorkstationClass,
                  NhlDEFAULT_APP,rlist);
    }
    else if (PS) {
/*
 * Create a PS workstation.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkPSFileName,"./tm03c.ps");
        NhlCreate(&wid,"tm03Work",NhlpsWorkstationClass,
                  NhlDEFAULT_APP,rlist);
    }
    NhlRLClear(rlist);
    NhlRLSetFloat(rlist,NhlNvpXF,.2);
    NhlRLSetFloat(rlist,NhlNvpYF,.8);
    NhlRLSetFloat(rlist,NhlNvpWidthF,.6);
    NhlRLSetFloat(rlist,NhlNvpHeightF,.6);
    NhlRLSetFloat(rlist,NhlNtmYLDataTopF,100.0);
    NhlRLSetFloat(rlist,NhlNtmYLDataBottomF,1000.0);
    NhlRLSetFloat(rlist,NhlNtmXBDataRightF,90.0);
    NhlRLSetFloat(rlist,NhlNtmXBDataLeftF,-90.0);
    NhlRLSetInteger(rlist,NhlNtmYLStyle,NhlIRREGULAR);
    NhlRLSetInteger(rlist,NhlNtmXBMode,NhlEXPLICIT);
    NhlRLSetInteger(rlist,NhlNtmXBMinorOn,False);
    NhlRLSetFloatArray(rlist,
               NhlNtmXBValues,labellocs,NhlNumber(labellocs));

/*
 * Array 'level' contains original grid point data locations in Y
 * direction. Providing the grid points to the TickMark object as the
 * control points for the IRREGULAR style transformation, means that
 * these points will be evenly spaced along the Y axis. Since this is
 * how CONPACK thinks the points are spaced, the tick marks will
 * correctly correspond with the  data coordinates. See the HLU User's
 * Guide for a complete discussion of IRREGULAR style transformations.
*/
    NhlRLSetStringArray(rlist,
                NhlNtmXBLabels,labels,NhlNumber(labels));
    NhlRLSetFloatArray(rlist,
               NhlNtmYLIrregularPoints,level,NhlNumber(level));

    NhlCreate(&pid,"TickMarks",NhltickMarkClass,wid,rlist);

    NhlDraw(pid);
    NhlFrame(wid);
    NhlDestroy(pid);
    NhlDestroy(wid);
    NhlDestroy(appid);
    NhlClose();
    exit(0);
}
