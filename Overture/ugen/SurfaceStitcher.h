#ifndef SURFACE_STITCHER_H
#define SURFACE_STITCHER_H

// ***********************************************************************
//   This class can be used to create an unstructured grid in the
//   the region between overlapping surface grids.
// ***********************************************************************

#include "Overture.h"

class Ugen;
class BodyDefinition;


class SurfaceStitcher
{
public:

SurfaceStitcher();
~SurfaceStitcher();

// define the grid faces that belong to different surfaces
int defineSurfaces( CompositeGrid & cg, BodyDefinition *bodyDefinition =NULL );

// Automatically enlarge the gap to a specified amount (plus extra)
int enlargeGapWidth(real minGapSizeInGridLines = .5, int extraGapWidth = 0 );

// enlarge the gap between surface patches by a fixed amount
int enlargeGap( int gapWidth, int gridToChange = -1 );


CompositeGrid* getSurfaceCompositeGrid();

UnstructuredMapping* getUnstructuredGrid();

Ugen* getUnstructuredGridGenertator();


int stitchSurfaceCompositeGrid(int option = 1 );

enum SurfaceMaskEnum
{
  originalMask,
  enlargedHoleMask
};

// set the mask in the surface grid to either the original mask or the mask for the enlarged gap.
int setMask( SurfaceMaskEnum option );


protected:

void buildSurfaceCompositeGrid(CompositeGrid &cg, BodyDefinition *bodyDefinition =NULL );

CompositeGrid *pCgSurf;  // holds the surface CompositeGrid
Ugen *pUgen;             // for computing the unstructured stitching grid

SurfaceMaskEnum maskOption; // is the mask in surface grid the original mask ?
intArray *surfMask;  // save masks from surface grids here so that we can reset to original or enlarged hole

};


#endif
