/***********************************************************************
*                                                                      *
*                Copyright (C)  1995                                   *
*        University Corporation for Atmospheric Research               *
*                All Rights Reserved                                   *
*                                                                      *
***********************************************************************/
/*
 *      File:           ti03c.c
 *
 *      Author:         Bob Lackman
 *          National Center for Atmospheric Research
 *          PO 3000, Boulder, Colorado
 *
 *      Date:           Fri Jan 06 18:31:18 MDT 1995
 *
 *      Description:    Demonstrates Title Object resource changes.
 */
#include <stdio.h>
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/ResList.h>
#include <ncarg/hlu/App.h>
#include <ncarg/hlu/Title.h>
#include <ncarg/hlu/XWorkstation.h>
#include <ncarg/hlu/NcgmWorkstation.h>
#include <ncarg/hlu/PSWorkstation.h>

main()
{
    int appid, wid, pid;
    int rlist;
    int NCGM=0, X11=1, PS=0;
	int len[2];
	float cmap[4][3];
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
    NhlRLSetString(rlist,NhlNappDefaultParent,"True");
    NhlRLSetString(rlist,NhlNappUsrDir,"./");
    NhlCreate(&appid,"ti03",NhlappClass,NhlDEFAULT_APP,rlist);

/*
 * Modify the color map.  Color indices '0' and '1' are the background
 * and foreground colors respectively.
 */
    cmap[0][0] = cmap[0][1] = cmap[0][2] = 0.;
    cmap[1][0] = 0.0; cmap[1][1] = 1.0; cmap[1][2] = 1.0;
    cmap[2][0] = 1.0; cmap[2][1] = 0.5; cmap[2][2] = 0.0;
    cmap[3][0] = 1.0; cmap[3][1] = 1.0; cmap[3][2] = 0.0;
    len[0] = 4;  len[1] = 3;

    if (NCGM) {
/*
 * Create a meta file workstation object.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkMetaName,"./ti03c.ncgm");
        NhlRLSetMDFloatArray(rlist,NhlNwkColorMap,&cmap[0][0],2,len);
        NhlCreate(&wid,"ti03Work",NhlncgmWorkstationClass,
                  NhlDEFAULT_APP,rlist);
    }
    else if (X11) {
/*
 * Create an XWorkstation object.
 */
        NhlRLClear(rlist);
        NhlRLSetInteger(rlist,NhlNwkPause,True);
		NhlRLSetMDFloatArray(rlist,NhlNwkColorMap,&cmap[0][0],2,len);
        NhlCreate(&wid,"ti03Work",NhlxWorkstationClass,
                  NhlDEFAULT_APP,rlist);
    }
    else if (PS) {
/*
 * Create a PSWorkstation object.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkPSFileName,"./ti03c.ps");
        NhlCreate(&wid,"ti03Work",NhlpsWorkstationClass,
                  NhlDEFAULT_APP,rlist);
    }       
/*
 * Specify the viewport extent of the object.
 */

    NhlRLClear(rlist);
    NhlRLSetFloat(rlist,NhlNvpXF,.2);
    NhlRLSetFloat(rlist,NhlNvpYF,.8);
    NhlRLSetFloat(rlist,NhlNvpWidthF,.6);
    NhlRLSetFloat(rlist,NhlNvpHeightF,.6);

    NhlCreate(&pid,"Titles", NhltitleClass,wid,rlist);

    NhlDraw(pid);
    NhlFrame(wid);
    NhlDestroy(pid);
    NhlDestroy(wid);
    NhlDestroy(appid);
    NhlClose();
    exit(0);
}
