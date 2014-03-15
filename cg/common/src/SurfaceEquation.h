#ifndef SURFACE_EQUATION_H
#define SURFACE_EQUATION_H

#include "Overture.h"
#include "Parameters.h"
#include "GenericGraphicsInterface.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <vector>
#include <list>
#else
#include <vector.h>
#include <list.h>
#endif

// This little class holds the information about a face where the surface equation is applied. 
class SurfaceEquationFace
{
public:
SurfaceEquationFace();
SurfaceEquationFace(int grid, int side, int axis);
int operator == ( const SurfaceEquationFace & face ) const;

int grid, side,axis;

};


// This class keeps track of all the faces where a surface equation is applied. 
class SurfaceEquation
{
public:

enum SurfaceEquationType
{
  heatEquation=0,
  numberOfEquationTypes
};


SurfaceEquation();
~SurfaceEquation();

SurfaceEquationType getSurfaceEquationType() const;
int setSurfaceEquationType(SurfaceEquationType type);

int 
update(CompositeGrid & cg, const IntegerArray & originalBoundaryCondition, 
       GenericGraphicsInterface & gi, 
       const aString & command =nullString,
       DialogData *interface=NULL  );


std::vector<SurfaceEquationFace> faceList;  // list of faces of grids where the surface equation applies. 


int numberOfSurfaceEquationVariables;
real kThermal, Cp, rho;
 

protected:

SurfaceEquationType surfaceEquationType;

};



#endif
