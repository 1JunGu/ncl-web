/************************************************************************
*                                                                       *
*                Copyright (C)  1995                                    *
*        University Corporation for Atmospheric Research                *
*                All Rights Reserved                                    *
*                                                                       *
************************************************************************/
/*
 *      File:           lg02c.c
 *
 *      Author:         Bob Lackman
 *          National Center for Atmospheric Research
 *          PO 3000, Boulder, Colorado
 *
 *      Date:           Fri Jan 13 18:31:18 MDT 1995
 *
 *      Description:    Demonstrates a Legend of 5 markers.
 */
#include <stdio.h>
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/ResList.h>
#include <ncarg/hlu/App.h>
#include <ncarg/hlu/Legend.h>
#include <ncarg/hlu/XWorkstation.h>
#include <ncarg/hlu/NcgmWorkstation.h>        
#include <ncarg/hlu/PSWorkstation.h>        

main()
{
    int appid, wid, pid;
    int rlist;
    char *labels[5];
    NhlColorIndex colors[5];
    NhlMarkerIndex item_ind[5];
    float mkthik[5];
    int NCGM=0, X11=1, PS=0;

/*
 * Initialize data values
 */
    labels[0] = "Marker_0";
    labels[1] = "Marker_1";
    labels[2] = "Marker_2";
    labels[3] = "Marker_3";
    labels[4] = "Marker_4";
    colors[0] = 3;
    colors[1] = 5;
    colors[2] = 7;
    colors[3] = 9;
    colors[4] = 11;
    mkthik[0] = 2.;
    mkthik[1] = 3.;
    mkthik[2] = 4.;
    mkthik[3] = 5.;
    mkthik[4] = 6.;
    item_ind[0] =  2;
    item_ind[1] =  3;
    item_ind[2] =  4;
    item_ind[3] =  5;
    item_ind[4] =  6;

/*
 * Initialize the high level utility library
 */
    NhlInitialize();

/*
 * Create an application context. Set the app dir to the current directory
 * so the application looks for a resource file in the working directory.
 * In this example the resource file supplies the plot title only.
 */
    rlist = NhlRLCreate(NhlSETRL);
    NhlRLClear(rlist);
    NhlRLSetString(rlist,NhlNappDefaultParent,"True");
    NhlRLSetString(rlist,NhlNappUsrDir,"./");
    NhlCreate(&appid,"lg02",NhlappClass,NhlDEFAULT_APP,rlist);

    if (NCGM) {
/*
 * Create a meta file workstation.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkMetaName,"./lg02c.ncgm");
        NhlCreate(&wid,"lg02Work",NhlncgmWorkstationClass,NhlDEFAULT_APP,
                  rlist);
    }
    else if (X11) {
/*
 * Create an X Workstation.
 */
        NhlRLClear(rlist);
        NhlRLSetInteger(rlist,NhlNwkPause,True);
        NhlCreate(&wid,"lg02Work",NhlxWorkstationClass,NhlDEFAULT_APP,
                  rlist);
    }
    else if (PS) {
/*
 * Create a PS workstation.
 */
        NhlRLClear(rlist);
        NhlRLSetString(rlist,NhlNwkPSFileName,"./lg02c.ps");
        NhlCreate(&wid,"lg02Work",NhlpsWorkstationClass,NhlDEFAULT_APP,
                  rlist);
    }
/*
 * Specify the viewport extent of the object.
 */
    NhlRLClear(rlist);
    NhlRLSetFloat(rlist,NhlNvpXF,0.);
    NhlRLSetFloat(rlist,NhlNvpYF,1.);
    NhlRLSetFloat(rlist,NhlNvpWidthF,1.);
    NhlRLSetFloat(rlist,NhlNvpHeightF,1.);

/*
 * Specify the type of markers for the legend.
 */
    NhlRLSetInteger(rlist,NhlNlgItemCount,5);
    NhlRLSetInteger(rlist,NhlNlgMonoItemType,True);
    NhlRLSetInteger(rlist,NhlNlgItemType,NhlMARKERS);
    NhlRLSetStringArray(rlist,NhlNlgLabelStrings,labels,5);
    NhlRLSetIntegerArray(rlist,NhlNlgMarkerColors,colors,5);
    NhlRLSetIntegerArray(rlist,NhlNlgMarkerIndexes,item_ind,5);
    NhlRLSetString(rlist,NhlNlgMonoMarkerThickness,"False");
    NhlRLSetFloatArray(rlist,NhlNlgMarkerThicknesses,mkthik,5);
    NhlRLSetFloat(rlist,NhlNlgMarkerSizeF,.05);
    NhlCreate(&pid,"Legend",NhllegendClass,wid,rlist);

    NhlDraw(pid);
    NhlFrame(wid);
    NhlDestroy(pid);
    NhlDestroy(wid);
    NhlDestroy(appid);
    NhlClose();
    exit(0);
}
