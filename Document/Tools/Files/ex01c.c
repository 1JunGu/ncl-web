#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void *cquad(float a, float b, float c,int nq, float *x, float *quad)
{
  int i;
  /*
   *  Calculate quadratic polynomial values.
   */
  for(i = 0; i < nq; i++ ) quad[i] = a*pow(x[i],2) + b*x[i] + c;
}
}

float arcln(int numpnt, float *pointx, float *pointy)
{
  int i;
  float pdist, a;
  /*
   *  Calculate arc lengths.
   */

  if(numpnt < 2) {
    printf("arcln: number of points must be at least 2\n");
    return;
  }
  a = 0.;
  for( i=1; i < numpnt; i++ ) {
    pdist = sqrt(pow(pointx[i]-pointx[i-1],2) + pow(pointy[i]-pointy[i-1],2));
    a += pdist;
  }
  return(a);
}
