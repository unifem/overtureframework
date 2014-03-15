#ifndef MODEL_BUILDER_H
#define MODEL_BUILDER

#include "Overture.h"
#include "CompositeSurface.h"

#include "Edge.h"
#include "Point.h"
#include "SphereLoading.h"

class ModelBuilder
{

public:
ModelBuilder();
~ModelBuilder();

bool addPlaneToModel(real planeCoordinates[3][3], int &planePoints, CompositeSurface &model, 
		     GenericGraphicsInterface &gi);

void checkModel(GenericGraphicsInterface & gi);

bool newModel(GenericGraphicsInterface & ps, MappingInformation & mapInfo, CompositeSurface &model);

void editModel(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & deletedSurfaces,
	       ListOfMappingRC &curveList, PointList & points);

Edge* getClosestCurve(int &s, CompositeSurface &model, SelectionInfo &select, GenericGraphicsInterface &gi, 
		      bool buildSpline = false );

Edge* closestEdgeOnSurface(real x, real y, real z, CompositeSurface &model, int s, bool buildSpline = false );


int linerGeometry( CompositeSurface & model, GenericGraphicsInterface& gi, PointList & points, 
                   SphereLoading & sphereLoading);

void simpleGeometry(MappingInformation &mapInfo, CompositeSurface & model, ListOfMappingRC &curveList,
		    PointList & points);


int update( MappingInformation & mapInfo, const aString & modelFileName=nullString );

void rapVolumeGrids(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & sGrids, 
	       CompositeSurface & vGrids, GraphicsParameters & volumeParameters);

void rapSurfaceGrids(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & sGrids, 
		     GraphicsParameters & surfaceParameters);

protected:

  bool addPrefix(aString cmd[], const aString & prefix);

  CompositeSurface model;
  CompositeSurface deletedSurfaces;
  CompositeSurface sGrids;
  CompositeSurface vGrids;

  PointList points; 
  ListOfMappingRC curveList;

};

#endif
