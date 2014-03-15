#ifndef AMR_UTIL_H
#define AMR_UTIL_H

#include "Overture.h"
#include "OGFunction.h"

enum InitialConditionEnum
{
  topHat
};



void printInfo( CompositeGrid & cg, int option=0 );

void printInfo( GridCollection & cg, int option=0 );

int getTrueSolution( realGridCollectionFunction & u, 
                 real t, 
                 RealArray & topHatCentre,
                 RealArray & topHatVelocity,
                 real topHatRadius,
		     InitialConditionEnum type = topHat );

real checkError( realCompositeGridFunction & u, real t, 
                 OGFunction & exact, 
                 const aString & label, 
                 FILE *file=NULL, 
                 int debug=0,
                 realGridCollectionFunction *pErr=NULL
                );

int outputRefinementInfo( GridCollection & gc, 
                      int refinementRatio, 
                      const aString & gridFileName, 
			  const aString & fileName );

#endif 
