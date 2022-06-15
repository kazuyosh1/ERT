#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define TMPFILE "tmp.asc"

extern void read_namelist( int *jmag, float *xlat, float *xlon,
			   int *iy, int *imd, float *hour, int *iut,
			   float *vbeg, float *vend, float *vstp );

extern void iri_sub_wrapper( bool jf_c[], int *jmag, float *xlat, float *xlon,
			     int *iy, int *imd, float *hour,
			     float *vbeg, float *vend, float *vstp );

extern float outf [1000][20];
extern float oar  [100];

char *trimspace(char *str);

int draw_edp();

int main()
{
  bool jf[51], jf_c[50]; /* define js this way so that jf subscripts are consistent 
			    with those in fortran, i.e., jf[0] is a dummy */
  int   jmag, iut, iy, imd, numstp;
  float xlat, xlon, hour, vbeg, vend, vstp, xcor;
  FILE  *fp;
  char  fig_title[50];
  char  y4m2d2[8], h2[5], lonlat[20];

  
  /* read input data by way of fortran namelist */
  
  read_namelist(&jmag, &xlat, &xlon, &iy, &imd, &hour, &iut, &vbeg, &vend, &vstp);

  /* switch options: assume default settings */
  for(int i = 0; i < 51; i++){
    jf[i] = true;
  }
  jf[4]  = false;
  jf[5]  = false;
  jf[6]  = false;
  jf[21] = false;
  jf[23] = false;
  jf[28] = false;
  jf[29] = false;
  jf[30] = false;
  jf[33] = false;
  jf[35] = false;
  jf[39] = false;
  jf[40] = false;
  jf[47] = false;

  for(int i = 0; i < 50; i++){
    jf_c[i] = jf[i+1];
  }

  hour = hour + iut*25;

  /* pass the input variables to the fortran irisub routines */

  iri_sub_wrapper(jf_c, &jmag, &xlat, &xlon, &iy, &imd, &hour, &vbeg, &vend, &vstp );

  numstp = floor((vend-vbeg)/vstp+1);
  xcor = vbeg;

  
  /* write down the output data to the TMPFILE */

  fp = fopen(TMPFILE, "w");

  fprintf(fp, "z(km)  Ne(m-3)  Tn(K)  Ti(K)  Te(K)\n");
  for(int i = 0; i < numstp; i++){
    fprintf(fp, "%.f  %.3e  %.1f  %.1f  %.1f %.1f\n", xcor, outf[i][0], outf[i][1], 
	    outf[i][2], outf[i][3], outf[i][4]);
    xcor = xcor + vstp;
  }
  fclose(fp);
  
  /* draw EDP plot using gnuplot */

  draw_edp( fig_title ); 

   return 0;
}

int draw_edp( ){
  int num_commands=6;
  int i;
  char * commandsForGnuplot[] = {
	     "set terminal x11 font \"arial,10\" size 600, 600",
	     "set xlabel \"Electron Density Profile m^-3\"",
             "set ylabel \"Altitude (km)\"",
             "set key autotitle columnhead",
             "unset key",
             "plot [0:3.5e+11] \""TMPFILE"\" using 2:1 w l",
  };

  FILE * gnuplotPipe = popen ("gnuplot -persistent", "w"); 

  for (i=0; i < num_commands; i++)
    {
      fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }

      return 0;
}

