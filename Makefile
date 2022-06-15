#
# Makefile for IRI
# [HIS] 6/8/2022 (kazu)
#

CC		= gcc
FC		= gfortran
CFLAGS		= -O2 -std=c99 -fPIC
FFLAGS		= -fconvert=big-endian -fPIC
LDFLAGS		= -shared -O2 -fPIC
CLIBS		= -lm


#### Files
LIBIRI		= libiri.so
LIBIRIOBJS	= irisub.o irifun.o iritec.o iridreg.o igrf.o cira.o iriflip.o
IRIEXE		= iriexe
IRIEXEOBJS	= iriexe_main.o irisub_wrapper.o
GIS_RADAR_EXE	= gis_radar_convert
GIS_RADAR_OBJS	= gis_radar_convert.o


#### Implicit rules
.SUFFIXES:	.for .f90


#### Building rules
all:	$(LIBIRI) $(IRIEXE) $(GIS_RADAR_EXE)

$(LIBIRI):	$(LIBIRIOBJS)
	$(FC) $(LDFLAGS) $^ -o $(LIBIRI)

$(IRIEXE):	$(IRIEXEOBJS) $(LIBIRI)
	$(FC) -o $@ $^

$(GIS_RADAR_EXE):	$(GIS_RADAR_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLIBS) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.for.o:
	$(FC) $(LIBRARIES) -c -I$(INCDIR) $(FFLAGS) $<

.f90.o:
	$(FC) $(LIBRARIES) -c -I$(INCDIR) $(FFLAGS) $<

clean:
	rm *.o 	*~ $(IRIEXE) $(GIS_RADAR_EXE) $(LIBIRI)

