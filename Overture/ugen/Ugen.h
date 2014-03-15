#ifndef KKC_UGEN

#include "AdvancingFront.h"
#include "TriangleWrapper.h"
#include "MappingInformation.h"
//#include "PlotStuff.h"
#include "GenericGraphicsInterface.h"
#include "AbstractException.h"
#include "Ogen.h"

class Ugen 
{

public:
  
  // helps keep track of which mesh generator is currently being used
  enum GeneratorType {
    TriWrap,
    AdvFront
  };

  Ugen();
  Ugen(GenericGraphicsInterface &ps_);

  ~Ugen();

  // build a hybrid grid interactively
  void updateHybrid(CompositeGrid & cg, MappingInformation & mapInfo );

  // build a hybrid grid automatically
  void updateHybrid(CompositeGrid & cg);

  void buildHybridInterfaceMappings(MappingInformation &mapInfo, 
				    CompositeGrid &cg,
				    Ugen::GeneratorType genWith,
				    intArray * & gridIndex2UnstructuredVertex,
				    intArray   & unstructuredVertex2GridIndex,
				    intArray * & gridVertex2UnstructuredVertex,
				    intArray   & initialFaceZones);

  UnstructuredMapping *getUnstructuredMapping() const 
  { 
    
    intArray elementList;
    elementList = ((AdvancingFront &)advancingFront).generateElementList();
    
    Range R(0,advancingFront.getNumberOfVertices()-1);
    
    realArray xyz = advancingFront.getVertices()(R,Range(advancingFront.getRangeDimension()));
    
    if ( elementList.getLength(0) )
      {
	UnstructuredMapping *um = new UnstructuredMapping;
	um->setNodesAndConnectivity(xyz,elementList);
	return um;
      }
    else 
      return (UnstructuredMapping *)NULL;

  }

protected:

  int initialize();

  void plot(const aString & title, CompositeGrid &cg, bool plotComponentGrids, bool plotTriangle, bool plotAdvFront);

  //  void buildHybridInterfaceMappings(CompositeGrid &cg);

  void preprocessCompositeGridMasks(CompositeGrid &cg);

  void initializeGeneration(CompositeGrid &cg, intArray *vertexIndex, 
			    intArray &numberOfVertices, intArray *vertexIDMap, 
			    intArray &vertexGridIndexMap, intArray *gridIndexVertexMap, 
			    intArray &initialFaces, intArray &initialFaceZones,
			    realArray & xyz_initial);


  void removeHangingFaces(CompositeGrid &cg);

  void buildHybridVertexMappings(CompositeGrid &cg, intArray *vertexIndex, intArray &numberOfVertices, intArray *vertexIDMap, intArray &vertexGridIndexMap, intArray *gridIndexVertexMap, realArray & xyz_initial);

  //void generateInitialFaceList(CompositeGrid &cg, intArray *vertexIndex, intArray &numberOfVertices, intArray *vertexIDMap, intArray &vertexGridIJKMap, intArray &initialFaces);
  void generateInitialFaceList(CompositeGrid &cg, intArray *vertexIndex, 
			       intArray &numberOfVertices, intArray *vertexIDMap, 
			       intArray &vertexGridIndexMap, intArray *gridIndexVertexMap, 
			       intArray &initialFaces, intArray &initialFaceZones);

  //void generateBoundaryMappings(CompositeGrid &cg);

  void computeZoneMasks(CompositeGrid &cg, intArray * &zoneMasks, intArray &numberOfMaskedZones);

  void generateSpacingControlMesh(CompositeGrid &cg, const intArray & initialFaceZones, const realArray & xyz_initial);
  void generateSpacingControlMeshForSurface(const intArray & initialFaces, const realArray &initial_vertices);

  void enlargeHole(CompositeGrid &cg, intArray &vertexGridIndexMap, int egrd=-1);
  void generateHoleLists(CompositeGrid &cg, intArray * vertexIndex, intArray &numberOfVertices);

  void sealHoles( CompositeGrid &cg, intArray *gridIndexVertexMap, intArray &initialFaces, realArray &xyz_initial, intArray &initialSurfaceMapping);

  void generateWithAdvancingFront();
  void generateWithTriangle();

  void sealHoles3D( CompositeGrid &cg, intArray *gridIndexVertexMap, intArray &boundaryHoleVertices, 
		    intArray &initialFaces, realArray &xyz_initial );

  mutable AdvancingFront advancingFront;
  TriangleWrapper triangleWrapper;
  UnstructuredMapping delaunayMesh;

  GenericGraphicsInterface *ps;
  GraphicsParameters psp;

};
  

class UnstructuredGeneratorError : public AbstractException 
{
public:
  virtual void debug_print() const { cerr << "\nUnstructuredGeneratorError"; }
};

class PreProcessingError : public UnstructuredGeneratorError
{
public:
  void debug_print() const
  {
    cerr<<": PreProcessingError : problems were found pre-processing the CompositeGrid";
  }
};

#endif
