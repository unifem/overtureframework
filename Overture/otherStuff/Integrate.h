#ifndef INTEGRATE_H
#define INTEGRATE_H "Integrate.h"



#include "Overture.h"
#include "BodyDefinition.h"

class Oges;
class SurfaceStitcher;

// =======================================================================================
/// \brief Use this class to integrate functions on overlapping grids, both volume
///  and surface integrals.
// =======================================================================================
class Integrate
{
 public:

  Integrate();
  Integrate( CompositeGrid & cg );
  ~Integrate();
  
  int computeAllWeights();

  int defineSurface( const int & surfaceNumber, const int & numberOfFaces_, IntegerArray & boundary ); 

  // interactively define surface and compute weights
  int defineSurfacesAndComputeWeights( GenericGraphicsInterface & gi );

  // get from a database file:
  int get( const GenericDataBase & dir, const aString & name);    

  int getFace(const int surfaceNumber,const int face, 
              int & side, int & axis, int & grid) const;

  const BodyDefinition & getBodyDefinition() const;
  
  int numberOfFacesOnASurface(const int surfaceNumber) const;

  int numberOfSurfaces() const;

  // return a pointer to the surface stitcher (if it exists)
  SurfaceStitcher* getSurfaceStitcher() const;

  // put to a database file:
  int put( GenericDataBase & dir, const aString & name) const;    

  // Delete the solver used to compute the weights after the weights have been computed (to save space)
  void setDeleteSolverAfterUse( bool trueOrFalse );

  // turn on interactive stitching (for debugging)
  void setInteractiveStitching( bool trueOrFalse );

  void setRadialAxis( int axis );

  int setTolerance( const real tol );

  // Compute the surface area.
  real surfaceArea(const int & surfaceNumber = -1 );

  // Compute the surface integral 
  real surfaceIntegral(const RealCompositeGridFunction & u, const int & surfaceNumber = -1 );

  // Compute the surface integrals of some components
  int surfaceIntegral(const RealCompositeGridFunction & u, 
                      const Range & C, 
                      RealArray & integral,
                      const int & surfaceNumber = -1 );

  int updateToMatchGrid( CompositeGrid & cg );

  // use AMR grids when computing integrals on grid functions that have AMR
  void useAdaptiveMeshRefinementGrids(bool trueOrFalse = true );

  // use hybrid grids to compute integrals
  void useHybridGrids( bool trueOrFalse = true );

  int updateForAMR(CompositeGrid &cg);  // call this function when AMR grids have changed.

  // Compute the total volume:
  real volume();

  // Compute the volume integral of a component of a grid function
  real volumeIntegral( const RealCompositeGridFunction & u, const int component=0 );

  // Compute the volume integrals of specified components.
  int volumeIntegral( const RealCompositeGridFunction & u, 
	  	      const Range & C, 
		      RealArray & integral );

  RealCompositeGridFunction & integrationWeights();
  RealCompositeGridFunction & leftNullVector();


  static int debug;

 protected:


  int initialize();
  int computeWeights();
  int computeSurfaceWeights(int surfaceNumber=-1 );
  
  int computeLeftNullVector();
  int surfaceIndex( int surfaceNumber );

  // kkc 030225 added surface stitching using ugen for surface integrals
  int computeStitchedSurfaceWeightsOld();

  int computeStitchedSurfaceWeights(int surfaceNumber=-1);

  int buildAdaptiveMeshRefinementSurfaceWeights(CompositeGrid & cgu, 
					        const int & surfaceNumber = -1 );
  int destroyAdaptiveMeshRefinementIntegrationArrays();


  aString className;         // Name of the Class

  bool useSurfaceStitching; 
  bool interactiveStitcher;  // call the stitcher in an interactive mode (for debugging)
  SurfaceStitcher *pSurfaceStitcher;  // for stitching surfaces

  BodyDefinition bd;
  CompositeGrid cg;

  int orderOfAccuracy;
  IntegerArray boundaryHasOverlap;
  IntegerArray faceWeightsDefined, surfaceWeightsDefined;
  
  realCompositeGridFunction weights, nullVector;
  
  Oges *solver;
  real tolerance;  // tolerance for sparse solver -- multiplied by the numberOfGridPoints

  bool weightsComputed, leftNullVectorComputed;
  bool allFaceWeightsDefined, weightsUpdatedToMatchGrid;

  bool deleteSolverAfterUse; // delete the solver used to compute the weights after the weights have been computed

  // Weights for AMR grids 
  bool useAMR;
  int *numberOfFacesPerSurface;  // total number of faces for each surface, including AMR grids
  int numberOfBoundarySurfaces;
  int numberOfBoundaryRefinementLevels;
  int *numberOfBoundaryFaces;
  RealArray ***boundaryWeights;
  int **pNumberOfBoundaryGrids;
  int ***pBoundaryGrid;

  // axisymmetric stuff
  int radialAxis;

  FILE *debugFile;
};

#endif
