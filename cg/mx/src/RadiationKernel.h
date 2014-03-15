#ifndef RADIATION_KERNEL
#define RADIATION_KERNEL


#include "Overture.h"


class RadiationKernel
{
public:

enum KernelTypeEnum
{
  planar,
  slab,
  cylindrical,
  spherical
};


RadiationKernel();
~RadiationKernel();

int setKernelType( KernelTypeEnum type );
KernelTypeEnum getKernelType() const;

int initialize( int numberOfGridPoints_, int numberOfFields_, 
		int numberOfModes_, real period_, real c_, 
		int orderOfTimeStepping_, int numberOfPoles_, 
                real radius=1. );

int evaluateKernel( double dt, RealArray & u, RealArray & Hu );

static real cpuTime;

protected:

KernelTypeEnum kernelType;

int numberOfGridPoints, numberOfFields,numberOfModes,ns,numberOfPoles,orderOfTimeStepping,bcinit;
double c,period,radius;
double *ploc,*fold,*phi,*amc,*fftsave;

double *alpha, *beta;
int *npoles;

};

#endif
