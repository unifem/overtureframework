#ifndef TRIMMED_MAPPING_BUILDER_H
#define TRIMMED_MAPPING_BUILDER_H

#include "Mapping.h"
#include "GenericGraphicsInterface.h"

// forward declarations: 
class NurbsMapping;
class TrimmedMapping;
class MappingInformation;

// ==================================================================================
/// \brief This class is used to build trimmed mappings for CAD geometries 
///        from the intersection of surfaces.
// ==================================================================================
class TrimmedMappingBuilder
{
public :

TrimmedMappingBuilder();
~TrimmedMappingBuilder();

// build one or more trimmed mappings 
int buildTrimmedMapping( MappingInformation & mapInfo, Mapping *surface = NULL );


protected:


int addCurve( NurbsMapping & curve, NurbsMapping & pCurve );

int constructOuterBoundaryCurves(Mapping & surface, NurbsMapping *curve, NurbsMapping *pCurve);

int deleteCurves();

int plotCurvesAndSurfaces( MappingInformation & mapInfo );

int resetTrimCurves();

int setOptionMenus( MappingInformation & mapInfo, DialogData & dialog, int createOrUpdate );

// --------  Member Data ------------

 Mapping *pSurface;

  // List of all possible trim curves:
 int numberOfTrimCurves;
 NurbsMapping **trimCurve;
 NurbsMapping **trimParametricCurve;

 // Outer trim curve (NULL=use surface boundary)
 NurbsMapping *outerTrimCurve;

 // Inner trim curves
 int numberOfInnerTrimCurves;
 NurbsMapping **innerTrimCurve;


 TrimmedMapping *trimmedMapping;


 bool plotReferenceSurface;
 bool plotTrimCurves;
 bool plotTrimmedMappings;

 bool newSurface;

 bool plotCuttingSurface;
 Mapping *cuttingSurface; // surface we cut with 

 GraphicsParameters parameters;
 GraphicsParameters referenceSurfaceParameters;

};

#endif
