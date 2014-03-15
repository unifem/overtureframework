#ifndef RAP_H
#define RAP_H

#include "GenericDataBase.h"
#include "ArraySimple.h"
#include "Mapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "MappingsFromCAD.h"
#include "NurbsMapping.h"
#include "RevolutionMapping.h"
#include "SweepMapping.h"
#include "CompositeSurface.h"
#include "SplineMapping.h"
#include "TFIMapping.h"
#include "PlaneMapping.h"
#include "SphereMapping.h"
#include "IntersectionMapping.h"
#include "HyperbolicMapping.h"
#include "MappingBuilder.h"
#include "TrimmedMapping.h"

#include "GenericGraphicsInterface.h"

#include "display.h"
#include "SplineMapping.h"
#include "GUIState.h"

#include "HDF_DataBase.h"
#include "CompositeTopology.h"

#include "Edge.h"
#include "Point.h"

#define SC (char *)(const char *)


//  bool
//  rapNewModel(GenericGraphicsInterface & ps, MappingInformation &mapInfo, CompositeSurface &model);

//  void
//  rapCheckModel(GenericGraphicsInterface & ps);

//  void
//  rapSimpleGeometry(MappingInformation &mapInfo, CompositeSurface & model, ListOfMappingRC &curveList,
//  		  PointList & points);

//  void
//  rapEditModel(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & deletedSurfaces,
//  	     ListOfMappingRC &curveList, PointList & points);

void
rapSurfaceGrids(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & sGrids, 
		GraphicsParameters & surfaceParameters);

void
rapVolumeGrids(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & sGrids, 
	       CompositeSurface & vGrids, GraphicsParameters & volumeParameters);

//  Edge*
//  getClosestCurve(int &s, CompositeSurface &model, SelectionInfo &select, GenericGraphicsInterface &gi,
//  		bool buildSpline = false);

//  Edge*
//  closestEdgeOnSurface(real x, real y, real z, CompositeSurface &model, int s, bool buildSpline = false);

bool
addPrefix(aString cmd[], const aString & prefix);

//  bool
//  addPlaneToModel(real planeCoordinates[3][3], int &planePoints, CompositeSurface &model, 
//  		GenericGraphicsInterface &gi);

#endif
