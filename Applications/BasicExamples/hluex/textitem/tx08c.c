/*
 *  $Id: tx08c.c,v 1.1 1996/01/04 17:08:36 haley Exp $
 */
/***********************************************************************
*                                                                      *
*                Copyright (C)  1995                                   *
*        University Corporation for Atmospheric Research               *
*                All Rights Reserved                                   *
*                                                                      *
***********************************************************************/
/*
 *      File:           tx08c.c
 *
 *      Author:         Jeff Boote (converted to C by Mary Haley)
 *                      National Center for Atmospheric Research
 *                      PO 3000, Boulder, Colorado
 *
 *      Date:           Thu Jan  4 09:44:52 MST 1996
 *
 *      Description:    Simple annotation example.
 */

#include <stdio.h>
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/ResList.h>
#include <ncarg/hlu/App.h>
#include <ncarg/hlu/TextItem.h>
#include <ncarg/hlu/XWorkstation.h>
#include <ncarg/hlu/NcgmWorkstation.h>
#include <ncarg/hlu/PSWorkstation.h>
#include <ncarg/hlu/XyPlot.h>

main()
{
    int ixwk, list, ixyplot, itx, ianno;
    int NCGM=0, X11=1, PS=0;
/*
 * Create application.
 */ 
    NhlOpen();
    list = NhlRLCreate(NhlSETRL);

    if (NCGM) {
/*
 * Create a meta file workstation.
 */
        NhlRLClear(list);
        NhlRLSetString(list,NhlNwkMetaName,"./tx08c.ncgm");
        NhlCreate(&ixwk,"tx08Work",NhlncgmWorkstationClass,
                  NhlDEFAULT_APP,list);
    }
    else if (X11) {
/*
 * Create an XWorkstation object.
 */
        NhlRLClear(list);
        NhlCreate(&ixwk,"tx08Work",
                  NhlxWorkstationClass,NhlDEFAULT_APP, list);
    }
    else if (PS) {
/*
 * Create a PS workstation.
 */
        NhlRLClear(list);
        NhlRLSetString(list,NhlNwkPSFileName,"./tx08c.ps");
        NhlCreate(&ixwk,"tx08Work",NhlpsWorkstationClass,
                  NhlDEFAULT_APP,list);
    }
/*
 * Create Plot object - no data, just illustration annotations.
 */
    NhlRLClear(list);
    NhlRLSetFloat(list,"vpXF",0.2);
    NhlRLSetFloat(list,"vpYF",.7);
    NhlRLSetFloat(list,"vpWidthF",.3);
    NhlRLSetFloat(list,"vpHeightF",.3);
    NhlCreate(&ixyplot,"xyPlot",NhlxyPlotClass,ixwk,list);

    NhlRLClear(list);
    NhlRLSetString(list,"txString","Second Line");
    NhlCreate(&itx,"txItem",NhltextItemClass,ixwk,list);
/*
 * There is currently a bug in the HLU library - you have to specify
 * ier - it should be returned in ianno if there is an error, but
 * this is what you currently have to use.
 */
    ianno = NhlAddAnnotation(ixyplot,itx);
/*
 * Just set the "zone" to something fairly large, so it is outside
 * of all "PlotManager" defined annotations.
 * Set the "side" to top - "amJust" defaults to centercenter - but
 * setting "amSide" to top makes that effectively bottomcenter.  So,
 * the textitem would be drawn so its bottomcenter is placed at the
 * top of the viewport of the plot, on the left side.  To get the
 * text centered over the plot - set "amParallelPosF" to .5 to move
 * the textitem over .5 the width of the plot.
 * Set "amOrthogonalPosF" to .1 to give a little bit of spacing
 * in the y direction from the plot (the previous zone).
 */
    NhlRLClear(list);
    NhlRLSetInteger(list,"amZone",10);
    NhlRLSetString(list,"amSide","top");
    NhlRLSetString(list,"amJust","bottomcenter");
    NhlRLSetFloat(list,"amParallelPosF",0.5);
    NhlRLSetFloat(list,"amOrthogonalPosF",0.1);
    NhlSetValues(ianno,list);

    NhlRLClear(list);
    NhlRLSetString(list,"txString","First Line");
    NhlCreate(&itx,"txItem",NhltextItemClass,ixwk,list);

    ianno = NhlAddAnnotation(ixyplot,itx);
/*
 * Add this textitem as an annotation with the same charactoristics
 * as the first one, but make the zone one higher - so it is just
 * outside of the first annotation.  (With a .1 distance away due
 * to the "amOrthogonalPosF".
 */
    NhlRLClear(list);
    NhlRLSetInteger(list,"amZone",11);
    NhlRLSetString(list,"amSide","top");
    NhlRLSetString(list,"amJust","bottomcenter");
    NhlRLSetFloat(list,"amParallelPosF",0.5);
    NhlRLSetFloat(list,"amOrthogonalPosF",0.1);
    NhlSetValues(ianno,list);
/*
 * Draw and advance frame.
 *   Notice that drawing the main plot automatically draw's the textitem
 *   since it is now a "member plot" of the xyplot.  In fact, you can
 *   no-longer draw the textitem indepentently.
 */
    NhlDraw(ixyplot);
    NhlFrame(ixwk);
/*
 * Now, if we move the base plot, the annoation stays with the plot.
 * It is drawn in its relative position to the xyplot.
 */
    NhlRLClear(list);
    NhlRLSetFloat(list,"vpXF",0.5);
    NhlRLSetFloat(list,"vpYF",.4);
    NhlSetValues(ixyplot,list);

    NhlDraw(ixyplot);
    NhlFrame(ixwk);
/*
 * Close automatically destroys all hlu objects that currently exist,
 * and destroys all "res-list's" that currently exist as well.
 * It is really only important to explicitly destroy those things
 * for long running applications, so memory use doesn't grow too
 * large.
 */
    NhlClose();
    exit(0);
}
