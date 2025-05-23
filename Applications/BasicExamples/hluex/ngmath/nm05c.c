/*
 *      $Id: nm05c.c,v 1.4 1998/06/28 21:31:41 fred Exp $
 */
/************************************************************************
*                                                                       *
*                Copyright (C)  1997                                    *
*        University Corporation for Atmospheric Research                *
*                All Rights Reserved                                    *
*                                                                       *
************************************************************************/
/*
 *  File:       nm05c.c
 *
 *  Author:     Mary Haley (taken from one of Fred Clare's examples)
 *          National Center for Atmospheric Research
 *          PO 3000, Boulder, Colorado
 *
 *  Date:       Tue Dec 16 10:05:38 MST 1997
 *
 *  Description: How to vary the exponent of the distances in a
 *               2D interpolation.
 */

#include <stdio.h>
#include <ncarg/ncargC.h>
#include <ncarg/gks.h>
#include <ncarg/ngmath.h>
#include <math.h>
#include <ncarg/hlu/App.h>
#include <ncarg/hlu/XWorkstation.h>
#include <ncarg/hlu/NcgmWorkstation.h>
#include <ncarg/hlu/PSWorkstation.h>


#define NUM     6
#define NX     61
#define NY     61

main()
{
  int  appid,wid,gkswid;
  int  srlist, grlist;
  int  NCGM=1, X11=0, PS=0;
  int  i, j, k, ier;
  float xi[] = {0.00, 1.00, 0.00, 1.00, 0.40, 0.75};
  float yi[] = {0.00, 0.00, 1.00, 1.00, 0.20, 0.65};
  float zi[] = {0.00, 0.00, 0.00, 0.00, 1.25, 0.80};
  float xo[NX], yo[NY], xinc, yinc, *output, outr[NY][NX];
  float rho = 3., theta = -45., phi =  55.;

/*
 *  Create the output grid.
 */
  xinc = 1./ (float) (NX-1); 
  yinc = 1./ (float) (NY-1); 
  for (i = 0; i < NX; i++) {
    xo[i] = xinc * (float) i;
  }
  for (j = 0; j < NY; j++) {
    yo[j] = yinc * (float) j;
  }

/*
 *  Exponent equals 0.5
 */
  c_dssetr("exp", 0.5);
  output = c_dsgrid2s(NUM, xi, yi, zi, NX, NY, xo, yo, &ier);
  if (ier != 0) {
    printf(" Error %d returned from nm05c\n",ier);
    exit(1);
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
    grlist = NhlRLCreate(NhlGETRL);
    NhlRLClear(srlist);
    NhlRLSetString(srlist,NhlNappUsrDir,"./");
    NhlCreate(&appid,"nm05",NhlappClass,NhlDEFAULT_APP,srlist);

    if (NCGM) {
/*
 * Create a meta file workstation.
 */
        NhlRLClear(srlist);
        NhlRLSetString(srlist,NhlNwkMetaName,"./nm05c.ncgm");
        NhlCreate(&wid,"nm05Work",
                  NhlncgmWorkstationClass,NhlDEFAULT_APP,srlist);
    }
    else if (X11) {
/*
 * Create an X workstation.
 */
        NhlRLClear(srlist);
        NhlRLSetInteger(srlist,NhlNwkPause,True);
        NhlCreate(&wid,"nm05Work",NhlxWorkstationClass,NhlDEFAULT_APP,srlist);
    }
    else if (PS) {
/*
 * Create a meta file workstation.
 */
        NhlRLClear(srlist);
        NhlRLSetString(srlist,NhlNwkPSFileName,"./nm05c.ps");
        NhlCreate(&wid,"nm05Work",NhlpsWorkstationClass,NhlDEFAULT_APP,srlist);
    }
/*
 * Get Workstation ID.
 */
	NhlRLClear(grlist);
	NhlRLGetInteger(grlist,NhlNwkGksWorkId,&gkswid);
	NhlGetValues(wid,grlist);
/*
 * There's no HLU object for surface plots yet, so we need to call the
 * LLUs to get a surface plot.
 */
	gactivate_ws (gkswid);

/*
 *  Reverse the array indices for plotting with tdez2d, since
 *  c_dsgrid returns its array in column dominate order.
 */
        for (i = 0; i < NX; i++) {
          for (j = 0; j < NY; j++) {
            outr[j][i] = output[i*NY+j];
          }
        }
	c_tdez2d(NX, NY, xo, yo, &outr[0][0], rho, theta, phi, 6);
	gdeactivate_ws (gkswid);
        NhlFrame(wid);
/*
 *  Exponent equals 1.0
 */
	c_dssetr("exp", 1.0);
	output = c_dsgrid2s(NUM, xi, yi, zi, NX, NY, xo, yo, &ier);
	if (ier != 0) {
	  printf(" Error %d returned from nm05c\n",ier);
	  exit(1);
	}
	gactivate_ws (gkswid);

/*
 *  Reverse the array indices for plotting with tdez2d, since
 *  c_dsgrid returns its array in column dominate order.
 */
        for (i = 0; i < NX; i++) {
          for (j = 0; j < NY; j++) {
            outr[j][i] = output[i*NY+j];
          }
        }
	c_tdez2d(NX, NY, xo, yo, &outr[0][0], rho, theta, phi, 6);
	gdeactivate_ws (gkswid);
        NhlFrame(wid);
/*
 *  Exponent equals 5.0
 */
	c_dssetr("exp", 5.0);
	output = c_dsgrid2s(NUM, xi, yi, zi, NX, NY, xo, yo, &ier);
	if (ier != 0) {
	  printf(" Error %d returned from nm05c\n",ier);
	  exit(1);
	}
	gactivate_ws (gkswid);
/*
 *  Reverse the array indices for plotting with tdez2d, since
 *  c_dsgrid returns its array in column dominate order.
 */
        for (i = 0; i < NX; i++) {
          for (j = 0; j < NY; j++) {
            outr[j][i] = output[i*NY+j];
          }
        }
	c_tdez2d(NX, NY, xo, yo, &outr[0][0], rho, theta, phi, 6);
	gdeactivate_ws (gkswid);
        NhlFrame(wid);
/*
 * NhlDestroy destroys the given id and all of its children.
 */
    NhlDestroy(wid);
/*
 * Restores state.
 */
    NhlClose();
    exit(0);
}
