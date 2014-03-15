#ifndef MAPPING_BUILDER_H
#define MAPPING_BUILDER_H

#include "Mapping.h"
#include "GenericGraphicsInterface.h"
#include "CompositeSurface.h"

class MappingInformation;

//----------------------------------------------------------------------
/// \brief Class to build multiple surface and volume grids on (composite) surfaces.
//----------------------------------------------------------------------
class MappingBuilder 
{
public:

  enum bcPlotOptionEnum
  {
    colourBoundariesByGridNumber,
    colourBoundariesByBCNumber,
    colourBoundariesByShareNumber
  };

  MappingBuilder();
  ~MappingBuilder();
  
  int build(  MappingInformation & mapInfo, Mapping *surface = NULL );

 protected:

  int assignBoundaryConditions(MappingInformation & mapInfo );
  int buildBoxGrid( MappingInformation & mapInfo );
  int buildCurveOnSurface(MappingInformation & mapInfo);

  int buildSurfacePatch(MappingInformation & mapInfo);
  int getBoundaryCurves();

  int plot(MappingInformation & mapInfo );

  bool plotReferenceSurface;
  bool choosePlotBoundsFromReferenceSurface;
  bool plotSurfaceGrids;
  bool plotVolumeGrids;
  bool plotBoundaryConditionMappings;
  bool plotBoundaryCurves;
  bool plotNonPhysicalBoundaries;
  bool plotEdgeCurves;
  bool plotGhostPoints;
  int numberOfGhostLinesToPlot;
  bool plotBlockBoundaries;
  bool plotGridLines;

  int numberOfSurfaceGrids;
  int numberOfVolumeGrids;
  int numberOfBoxGrids;
  int numberOfBoundaryCurves;

  bcPlotOptionEnum bcPlotOption;

  real targetGridSpacing[2];
  

  int numberOfExtraBoundaryCurves;
  int maxNumberOfExtraBoundaryCurves;
  Mapping **extraBoundaryCurve;

  Mapping **boundaryCurves;

  // We store the surface and volume grids in CompositeSurface's so we can easily replot them
  CompositeSurface surfaceGrids, volumeGrids;

  Mapping *pSurface;
  GraphicsParameters parameters, referenceSurfaceParameters;

};


#endif  
