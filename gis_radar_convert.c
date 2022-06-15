#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif
#define ER 6378.1


int GIS2Radar (double *range,
	       double *bearing,
	       double glonInit,
	       double glatInit,
	       double glonFinal,
	       double glatFinal);

int RtoG (double range,
	  double bearing,
	  double glonInit,
	  double glatInit,
	  double *glonFinal,
	  double *glatFinal );

int main(){
  int i;
  double glonInit  = -75.;
  double glatInit  =  37.;
  double glonFinal = -66.;
  double glatFinal =  18.;
  double range, bearing, glonFinal2, glatFinal2;

  printf("\n### gis_radar_convert ###\n");
  printf("Initial location: lat: %3.1f, long: %4.1f\n", glatInit,  glonInit);
  printf("Final   location: lat: %3.1f, long: %4.1f\n", glatFinal, glonFinal);

  i = GIS2Radar(&range, &bearing, glonInit, glatInit, glonFinal, glatFinal);
  printf("\n");
  printf("### GIS2Radar ###\n");
  printf("Radar coordinates: bearing: %4.1f (deg), range : %5.1f (km)\n", bearing, range);

  i = RtoG(range, bearing, glonInit, glatInit, &glonFinal2, &glatFinal2);
  printf("\n");
  printf("### RtoG ###\n");
  printf("Final   location: lat: %3.1f, long: %4.1f\n\n", glatFinal2, glonFinal2);

  return 0;
}

int GIS2Radar (double *range,
	       double *bearing,
	       double glonInit,
	       double glatInit,
	       double glonFinal,
	       double glatFinal){

  double lon1 = glonInit  * M_PI / 180;
  double lat1 = glatInit  * M_PI / 180;
  double lon2 = glonFinal * M_PI / 180;
  double lat2 = glatFinal * M_PI / 180;
  double dlon, dlat, a, c;
  double x, y, brng;
  
  dlat = lat2 - lat1;
  dlon = lon2 - lon1;
  a = pow(sin(dlat/2),2) + cos(lat1)*cos(lat2)*pow(sin(dlon/2),2);
  c = 2*atan2(sqrt(a), sqrt(1-a));
  (*range) = ER * c;

  y = sin(dlon)*cos(lat2);
  x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dlon);
  brng = atan2(y, x);
  (*bearing) = (brng*180/M_PI);

  return 0;
}

int RtoG (double range,
	  double bearing,
	  double glonInit,
	  double glatInit,
	  double *glonFinal,
	  double *glatFinal ){

  double lon1 = glonInit * M_PI / 180;
  double lat1 = glatInit * M_PI / 180;
  double brng = bearing  * M_PI / 180;  /* in radian; */
  double lon2, lat2;

  lat2 = asin( sin(lat1)*cos(range/ER) + cos(lat1)*sin(range/ER)*cos(brng));
  lon2 = lon1\
    + atan2( sin(brng) * sin(range/ER) * cos(lat1), cos(range/ER) - sin(lat1) * sin(lat2) );
  (*glatFinal) = lat2 * 180 / M_PI;
  (*glonFinal) = lon2 * 180 / M_PI;

  return 0;
}

