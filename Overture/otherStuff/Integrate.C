//#define BOUNDS_CHECK

#include "Integrate.h"
#include "GenericGraphicsInterface.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "SparseRep.h"
#include "UnstructuredMapping.h"
#include "Geom.h"
#include "InterpolatePoints.h"
#include "display.h"
#include "SurfaceStitcher.h"
#include "ParallelUtility.h"

int Integrate::debug =0;

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)

Integrate::
Integrate()
// ================================================================================================
/// \brief
///    Default constructor.
/// \details 
///    The Integrate class is used to perform volume and surface integrals on overlapping grids.
/// \author WDH.
// ================================================================================================
{
  debugFile=NULL;
  initialize();
}

Integrate::
Integrate(CompositeGrid & cg_)
// ================================================================================================
// \brief
///    Integrate constructor taking a Compositegrid.
/// \details 
///    The Integrate class is used to perform volume and surface integrals on overlapping grids.
/// 
/// \param cg_ (input) : supply a grid on which to integrate.
// ================================================================================================
{
  debugFile=NULL;
  initialize();
  updateToMatchGrid(cg_);
}

Integrate::
~Integrate()
// ================================================================================================
/// \brief
///    Destructor.
// ================================================================================================
{

  delete solver;

  delete pSurfaceStitcher;
  
  // *** delete weights for AMR grids ***
  destroyAdaptiveMeshRefinementIntegrationArrays();
  
  if( debugFile!=NULL ) fclose(debugFile);
}

int Integrate::
initialize()
// ================================================================================================
/// \brief
///    Initialize parameters and data.
// ================================================================================================
{
  className="Integrate";

  tolerance=REAL_EPSILON;  // tolerance for sparse solver -- multiplied by the numberOfGridPoints
  
  orderOfAccuracy=2;
  solver=0;
  weightsComputed=false;
  leftNullVectorComputed=false;
  allFaceWeightsDefined=false;
  weightsUpdatedToMatchGrid=false;
  
  useSurfaceStitching=false;   // false=old method, true=new method
  interactiveStitcher=false;    // true -> call the stitcher in an interactive mode (for debugging)
  pSurfaceStitcher =NULL;

  // weights for AMR grids:
  useAMR=false;
  numberOfFacesPerSurface=NULL;
  numberOfBoundarySurfaces=0;
  numberOfBoundaryRefinementLevels=0;
  numberOfBoundaryFaces=NULL;

  boundaryWeights=NULL;
  pNumberOfBoundaryGrids=NULL;
  pBoundaryGrid=NULL;

  radialAxis=-1;

  deleteSolverAfterUse=true; // delete the solver used to compute the weights after the weights have been computed

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();

  if( debug>0 && debugFile==NULL )
  {
#ifdef USE_PPP
    debugFile = fopen(sPrintF("integrateNP%i.debug",np),"w" ); 
    fprintf(debugFile,
	    " ********************************************************************************************* \n"
	    " ************************** Integate debug file, myid=%i, NP=%i ****************************** \n"
	    " ********************************************************************************************* \n\n",
	    myid,np);
#else
    debugFile = fopen("integrate.debug","w" ); 
    fprintf(debugFile,
	    " *************************************************************************************** \n"
	    " ***************************** Integate debug file  ************************************ \n"
	    " *************************************************************************************** \n\n");

#endif
  }

  return 0;
}

int Integrate::
computeAllWeights()
// ============================================================================================
///  \brief Compute all integration weights for the volume and surfaces.
///  This routine will do nothing if the weights have already been computed.
// ============================================================================================
{
  if( !weightsComputed )
    computeWeights();
  else
    printF("Integrate:computeAllWeights:NFO: The surface and volume integration weights are already computed.\n");

  return 0;
}



int Integrate::
setTolerance( const real tol )
// ============================================================================================
/// \brief Set the tolerance for sparse solver used to compute the integration weights.
// ============================================================================================
{
  tolerance =tol;
}


int Integrate::
updateToMatchGrid( CompositeGrid & cg_ )
// ================================================================================================
/// \brief
///     Call this routine to supply a grid or to indicate that the grid has changed.
/// \param cg (input) : supply a grid on which to integrate.
// ================================================================================================
{
  cg.reference(cg_);
  weightsComputed=false;

  // compute the order of accuracy.
  int grid;
  int dwMin=3, dwMax=3;
  Range Rx(0,cg.numberOfDimensions()-1);
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    dwMin=min(cg[grid].discretizationWidth()(Rx));
    dwMax=max(cg[grid].discretizationWidth()(Rx));
  }
  if( dwMin==5 && dwMax==5 )
    orderOfAccuracy=4;
  else 
    orderOfAccuracy=2;
  
  surfaceWeightsDefined.redim(0);
  faceWeightsDefined.redim(2,3,cg.numberOfComponentGrids());
  faceWeightsDefined=false;
  
  // boundaryHasOverlap(side,axis,grid ) = true if this face has interpolation points on it
  //                                     = false if this face has no interp. pts
  //                                     = -1 : don't know yet
  boundaryHasOverlap.redim(2,3, cg.numberOfComponentGrids());
  boundaryHasOverlap=-1; // -1 means that we do not yet know if this face has interpolation points.

  allFaceWeightsDefined=false;
  weightsUpdatedToMatchGrid=false;
  
  if( debug & 1 )
    printF("Integrate::updateToMatchGrid: orderOfAccuracy= %i\n",orderOfAccuracy);
  return 0;
}


int Integrate::
defineSurface(const int & surfaceNumber, const int & numberOfFaces_, IntegerArray & boundary )
// ================================================================================================
/// \brief Define a new surface.
/// \details 
///     Specify the sides of grids that define a "surface". A surface represents some subset of the
///   boundary of an entire domain. For example, for the sphere-in-a-box grid a surface could
///   represent the surface of the sphere. To define a surface you must supply:
/// 
/// \param surfaceNumber (input) : a surface identifier. This value must be bigger than or equal to zero. Normally surfaces
///   should be numbered starting from zero.
/// \param numberOfFaces (input) : the number of faces that make up the surface.
/// \param boundary (input): boundary(3,numberOfFaces) : (side,axis,grid)=boundary(0:2,i) i=0,1,...numberOfFaces.
///    To define a surface you must supply a list of sides of grids.
// ================================================================================================
{
  bd.defineSurface(surfaceNumber,numberOfFaces_,boundary);
  if( bd.numberOfSurfaces>surfaceWeightsDefined.getLength(0) )
  {
    const int oldDim=surfaceWeightsDefined.getLength(0);
    const int newDim=oldDim+10;
      
    surfaceWeightsDefined.resize(newDim);
    surfaceWeightsDefined(Range(oldDim,newDim-1))=false;
  }

  if( boundaryWeights!=NULL )
  {
    // something has changed -- destroy the weights 
    // we could be more careful here and not delete still valid weights
    printf("Integrate::defineSurface: INFO: AMR weights are being destroyed since a new surface has been defined\n");
    
    destroyAdaptiveMeshRefinementIntegrationArrays();
  }

  return 0;
}

int Integrate::
updateForAMR(CompositeGrid & cgu )
// =========================================================================
/// \brief Update the Integrate object when AMR grids have changed.
/// \details 
///    Call this function when AMR grids have changed. This will cause the 
///  arrays of integration weights for AMR grids to be destroyed. They will
///  be regenerated as needed.
// =========================================================================
{
  // delete any existing integration weights for AMR
  destroyAdaptiveMeshRefinementIntegrationArrays();

  if( useAMR && cgu.numberOfRefinementLevels()>1 )
  { // build AMR integration weights for this surface if they are not already built.
    for( int s=0; s<bd.totalNumberOfSurfaces(); s++ )
    {
      int surfaceNumber=bd.getSurfaceNumber(s);
      buildAdaptiveMeshRefinementSurfaceWeights(cgu,surfaceNumber);
    }
      
  }
  return 0;
}


/* -----
int Integrate:: 
setOrderOfAccuracy( const int & order )
// ================================================================================================
// /Description:
//    Choose the order of accuracy, 2 or 4;
// /order (input) : order of accuracy, 2 or 4;
// \author: WDH
// ================================================================================================
{
  if( order!=2 && order!=4 )
  {
    printf("Integrate::setOrderOfAccuracy:ERROR: order=%i is not available. Only 2 and 4\n",order);
    return 1;
  }
  orderOfAccuracy=order;
}
---- */

int Integrate::
numberOfFacesOnASurface(const int surfaceNumber) const
// ================================================================================================
/// \brief
///     Return the number of faces that form a given surface. 
/// \details 
///   For AMR grids this will return of the total number of faces including AMR grids (assuming
///     useAdaptiveMeshRefinementGrids(true) has been set).
/// 
/// \return  number of faces for surface.
// ================================================================================================
{
  if( !useAMR || numberOfFacesPerSurface==NULL )
  {
    return bd.numberOfFacesOnASurface(surfaceNumber);
  }
  else
  {
    int surface=bd.surfaceIndex(surfaceNumber);
    assert( numberOfFacesPerSurface!=NULL );
    return numberOfFacesPerSurface[surface];
  }
  
}

int Integrate::
numberOfSurfaces() const
// ================================================================================================
/// \brief
///    Return the number of surfaces currently defined.
/// \return : number surfaces.
// ================================================================================================
{
  return numberOfBoundarySurfaces;
}

const BodyDefinition & Integrate::
getBodyDefinition() const
// ================================================================================================
/// \brief
///     Return the BodyDefinition object which defines the relationship between grids and boundaries.
// ================================================================================================
{
  return bd;
}


int Integrate::
getFace(const int surfaceNumber, const int face, 
        int & side, int & axis, int & grid) const
// ================================================================================================
/// \brief
///     Return the data for a particular face of a surface.
/// 
/// \details
///   For AMR grids this will return the data for AMR grids as well (assuming
///     useAdaptiveMeshRefinementGrids(true) has been set).
/// 
/// \param surface,face (input) : return info for this surface and face.
/// \param side,axis,grid (output): this face corresponds to these values.
/// \return  0 for success.
// ================================================================================================
{
  if( !useAMR  || numberOfFacesPerSurface==NULL )
  {
    return bd.getFace(surfaceNumber,face,side,axis,grid);
  }
  else
  {
    #define numberOfBoundaryGrids(surface,level,face) \
               pNumberOfBoundaryGrids[surface][(level)+(numberOfBoundaryRefinementLevels)*(face)]
    #define boundaryGrid(surface,level,face,g) \
               pBoundaryGrid[surface][(level)+(numberOfBoundaryRefinementLevels)*(face)][g]

    assert( numberOfBoundaryRefinementLevels>0 );


    const int surface=bd.surfaceIndex(surfaceNumber);

    const int numberOfFaces=bd.numberOfFaces(surface);
    int num=0;
    for( int i=0; i<numberOfFaces; i++ )
    {
      for( int level=0; level<numberOfBoundaryRefinementLevels; level++ )
      {
        for( int g=0; g<numberOfBoundaryGrids(surface,level,i); g++ )
	{
	  num++; 
	  if( face<num )
	  {
            const int baseGridFaceNumber=i;
            const int baseGrid=boundaryGrid(surface,0,baseGridFaceNumber,0);   // here is the baseGrid
	    
            // once we know the face number of the base grid we can look-up (side,axis)
            bd.getFace(surfaceNumber,baseGridFaceNumber,side,axis,grid);
            assert( grid==baseGrid);   // this should be true 
	    
	    grid = boundaryGrid(surface,level,i,g);
	    return 0; 
	  }
	}
      }
    }
    // error - invalid face --
    printf("Integrate::getFace:ERROR: surfaceNumber=%i face=%i NOT found!\n",surfaceNumber,face);
    return 1;
    
    #undef numberOfBoundaryGrids
    #undef boundaryGrid
  }
  
}

SurfaceStitcher* Integrate::
getSurfaceStitcher() const
// ================================================================================================
/// \brief
///     Return a pointer to the surface stitcher (if it exists)
// ================================================================================================
{
  return pSurfaceStitcher;
}

void Integrate::
setInteractiveStitching( bool trueOrFalse )
// ================================================================================================
/// \brief 
///      Turn on interactive stitching (for debugging)
// ================================================================================================
{
  interactiveStitcher=trueOrFalse;
}



RealCompositeGridFunction & Integrate::
integrationWeights()
// ================================================================================================
/// \brief 
///     Return the integration weights.
/// \return  a grid function that holds the integration weights. 
// ================================================================================================
{
  if( !weightsComputed )
    computeWeights();

  return weights;
}

RealCompositeGridFunction & Integrate::
leftNullVector()
// ================================================================================================
/// \brief 
///     Return the left null vector of the Neumann problem. This vector is related to the
///  integration weights.
/// \return  a grid function that holds the left null vector.
// ================================================================================================
{
  if( !leftNullVectorComputed )
    computeLeftNullVector();

  return nullVector;
}

int Integrate::
surfaceIndex( int surfaceNumber )
// ================================================================================================
/// \brief Protected function to return a surface index.
/// \details 
///    For a given surfaceNumber determine the surfaceIndex such that
///    surfaceIdentifier(surfaceIndex)==surfaceNumber. Return -1 if no match is found.
/// \param surfaceNumber (input) : the surface ID for a user defined surface.
/// \return  the index into the surfaceIdentifier array, or -1 if no match exists.
// ================================================================================================
{
  return bd.surfaceIndex(surfaceNumber);
}

real Integrate::
surfaceArea( const int & surfaceNumber /* = -1 */ )
// ================================================================================================
/// \brief  Compute the total surface area or the area of a surface.
/// \note: The surface area is recomputed every time this function is called (i.e. it is not saved internally).
///
/// \param surfaceNumber (input) : the surface identifier as defined through a call to {\tt defineSurface}.
///     If no   surfaceNumber is specified then the entire surface will be integrated.
/// \return  The surface area.
/// \author WDH
// ================================================================================================
{
  Range C(0,0);
  RealArray integral(1);

  realCompositeGridFunction u(cg);    // We could create an optimized routine that doesn't need u
  u=1.;
  surfaceIntegral(u,C,integral,surfaceNumber);
  return integral(0);
}



real Integrate::
surfaceIntegral(const RealCompositeGridFunction & u, const int & surfaceNumber /* = -1 */ )
// ================================================================================================
/// \brief
///     Compute the surface integral of u. 
/// \param u (input) : function to integrate. This function must be defined at the appropriate points.
/// \param surfaceNumber (input) : the surface identifier as defined through a call to {\tt defineSurface}.
///     If no   surfaceNumber is specified then the entire surface will be integrated.
/// \return  The integral of u.
/// \author WDH
// ================================================================================================
{
  Range C(0,0);
  RealArray integral(1);
  surfaceIntegral(u,C,integral,surfaceNumber);
  return integral(0);
}

  
// Delete the solver used to compute the weights after the weights have been computed (to save space)
void Integrate::
setDeleteSolverAfterUse( bool trueOrFalse )
{
  deleteSolverAfterUse=trueOrFalse;
}



void Integrate::
useAdaptiveMeshRefinementGrids(bool trueOrFalse /* = true */ )
// ================================================================================================
/// \brief
///    Indicate whether AMR grids should be used 
///    when computing integrals on grid functions that have AMR. If false, only the base grids
///    are used in the integration.
/// \param trueOrFalse (input) : 
/// \author WDH
// ================================================================================================
{
  useAMR=trueOrFalse;
  if( cg.numberOfDimensions()==3 && cg.numberOfRefinementLevels()>1 )
  {
    printf(" ***** Integrate::useAdaptiveMeshRefinementGrids AMR option not implemented in 3D yet ***\n");
    useAMR=false;
  }
  
}

void Integrate::
useHybridGrids( bool trueOrFalse /* = true */ )
// ================================================================================================
/// \brief
///    Indicate whether hybrid grids should be used to compute the weights for integrals.
/// \param trueOrFalse (input) : 
/// \author WDH
// ================================================================================================
{
  useSurfaceStitching=trueOrFalse;
}


#include "BoxLib.H"
#include "Box.H"
#include "BoxList.H"

static Box 
cellCenteredBox( MappedGrid & mg, int ratio =1 )
// ================================================================================================
/// \brief
///   Build a cell centered box from a MappedGrid.
// ===============================================================================================
{
  Box box = mg.box();      // we could keep a list for below
  int axis;
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    if( mg.isPeriodic(axis) )
    {
      box.setSmall(axis,mg.gridIndexRange(0,axis)-1);
      box.setBig(axis,  mg.gridIndexRange(1,axis)+1);
    }
  }

  box.convert(IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL)));  // build a cell centered box

  if( ratio>1 )
    box.refine( ratio );
  else if( ratio<-1 )
    box.coarsen( -ratio );

  for( axis=mg.numberOfDimensions(); axis<3; axis++ )
  {
    box.setSmall(axis,0); // +widthOfProperNesting);
    box.setBig(axis,0);   // +widthOfProperNesting);
  }
  return box;
}

static int
buildBoundaryBoxes(GridCollection & gc, 
		    int baseGrid,
		    int refinementLevel,
		    BoxList & boundaryBoxes,
                    int side, int axis  )
// ============================================================================================
/// \brief Build a list of boundary boxes.
///
/// \details
///   Build a list of boxes for grids on refinement level "refinementLevel" 
///  that covers part of a given boundary (baseGrid,side,axis). The list of boxes is in the
///  index space of the lower level, refinementLevel-1.
///  This list is used to adjust the integration weights on lower levels of refinement.
///  
///
/// \param baseGrid (input): boxes for this base grid.
/// \param refinementLevel (input) : build a list for this level (>1)
/// \param boundaryBoxes : a list of boxes that cover a part of the boundary -- NOTE: These boxes are in the
///    index space of refinementLevel-1.
/// \param side,axis : Defines the boundary of the baseGrid
///
// ==========================================================================================
{
  int debug=0;

  if( gc.numberOfRefinementLevels()<=1 || refinementLevel<1 )
    return 0;
  
  GridCollection & rl = gc.refinementLevel[refinementLevel];

  // Note: Use "grid=0" when getting a refinementFactor since this grid will always be there
  int ratio=rl.refinementFactor(0,0)/gc.refinementLevel[refinementLevel-1].refinementFactor(0,0); 
//    printf(" ratio=%i rf=%i rf=%i \n",ratio,rl.refinementFactor(0,0),
//  	 gc.refinementLevel[refinementLevel-1].refinementFactor(0,0));
  

  // ** NOTE: intersects requires all directions to be the same centering (cannot have unused directions to be NODE)
  // IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));

  MappedGrid & bg = gc[baseGrid];

  // find all refinement grids on this level that have the same base grid
  // and that share the boundary
  for( int gr=0; gr<rl.numberOfComponentGrids(); gr++ )
  {
    if( rl.baseGridNumber(gr)==baseGrid && rl[gr].boundaryCondition(side,axis)>0 )
    {
      // build a box for the fine grid on the boundary
      MappedGrid & mg = gc[rl.gridNumber(gr)];

      Box box= cellCenteredBox(mg);
      // cout << "level=" << refinementLevel << "box =" << box << endl;
      
      box.coarsen(ratio);  // coarsen to level-1
      // cout << "ratio=" << ratio << " coarsened: box =" << box << endl;
      
      box.setSmall(axis,mg.gridIndexRange(side,axis)/ratio);  // box lives on the boundary only
      box.setBig  (axis,mg.gridIndexRange(side,axis)/ratio);

      // cout << "level=" << refinementLevel << " boundary: box =" << box << endl;

      boundaryBoxes.add(box); 
    }
  }

  boundaryBoxes.simplify(); 

  if( debug & 1 )
  {
    printF(" buildBoundaryBoxes: baseGrid=%i (side,axis)=(%i,%i) level=%i \n",baseGrid,side,axis,refinementLevel);
    
    cout << " boundaryBoxes: (to mark level-1)" << boundaryBoxes << endl;
  }
  
  return 0;
}

int Integrate::
destroyAdaptiveMeshRefinementIntegrationArrays()
// ==============================================================================================
///  \brief
///     Destroy the arrays that we use to store weights for AMR integration.
// ==============================================================================================
{
  if( boundaryWeights!=NULL )
  {
    for( int s=0; s<numberOfBoundarySurfaces; s++ )
    {
      if( boundaryWeights[s] !=NULL )
      {
	for( int level=0; level<numberOfBoundaryRefinementLevels; level++ )
	{
	  for( int face=0; face<numberOfBoundaryFaces[s]; face++ )
	  {
	    delete [] boundaryWeights[s][(level)+(numberOfBoundaryRefinementLevels)*(face)];
	    delete [] pBoundaryGrid  [s][(level)+(numberOfBoundaryRefinementLevels)*(face)];
	  }
	}
      }
      delete [] boundaryWeights[s];
      delete [] pNumberOfBoundaryGrids[s];
      delete [] pBoundaryGrid[s];
    } // end for s

    delete [] boundaryWeights;         boundaryWeights=NULL;
    delete [] pNumberOfBoundaryGrids;  pNumberOfBoundaryGrids=NULL;
    delete [] pBoundaryGrid;           pBoundaryGrid=NULL;

    numberOfBoundarySurfaces=0;
    numberOfBoundaryRefinementLevels=0;
    delete [] numberOfBoundaryFaces;  numberOfBoundaryFaces=NULL;
    
    delete [] numberOfFacesPerSurface; numberOfFacesPerSurface=NULL;
  }
  return 0;
}




int Integrate::
buildAdaptiveMeshRefinementSurfaceWeights(CompositeGrid & cgu, 
					  const int & surfaceNumber /* = -1 */ )
// =======================================================================================
/// \brief Build AMR surface weights. This is a protected routine.
/// 
/// \details
/// Allocate space for the integration weights on the boundary for all refinement levels,
/// and compute the weights. For points on coarser grids that are hidden by refinement grids
/// we need to adjust the weights.
/// 
// ======================================================================================
{
  const bool adaptiveGrid = cgu.numberOfRefinementLevels()>1;
  if( !adaptiveGrid )
    return 0;

  if( boundaryWeights!=NULL )
  {
    if( cgu.numberOfRefinementLevels()!=numberOfBoundaryRefinementLevels ||
	bd.numberOfSurfaces!=numberOfBoundarySurfaces )
    {
      // something has changed -- destroy weights and rebuild them
      // we could be more careful here and not delete still valid weights
      destroyAdaptiveMeshRefinementIntegrationArrays();
    }
  }
  
  int debug=0; // =3
  if( debug  )
    printf("\n **** Entering Integrate::buildAdaptiveMeshRefinementSurfaceWeights, surfaceNumber=%i\n",surfaceNumber);

  if( surfaceNumber==-1 )
  {
    // ignore this case for now
    printf("buildAdaptiveMeshRefinementSurfaceWeights:ERROR: surfaceNumber=-1 not handled yet\n");
    return 1;
  }


  int i, surface=-1;
  surface=bd.surfaceIndex(surfaceNumber);
  if( surface<0 || surface >= bd.numberOfSurfaces )
  {
    printf("buildAdaptiveMeshRefinementSurfaceWeights: invalid surface index = %i for surfaceNumber=%i \n",
       surface,surfaceNumber);
    return 1;
  }

  if( boundaryWeights!=NULL && boundaryWeights[surface]!=NULL )
  {
    // The AMR weights must already have been computed.
    return 0;
  }
  if( bd.numberOfFaces(surface)>1 || cgu.numberOfDimensions()!=2 )
  {
    printf("Integrate:ERROR: integration of AMR grids not implemented for 3D or for overlapping surface grids\n");
    printf("Integrate:Solution on AMR grids will be ignored, integration will proceed on level 0 grids\n");
    return 1;
  }

  // ***************************************************************************************************
  // We save the following info about the integration weights on the boundary
  //   realArray boundaryWeights(surface,level,face,g)  : integration weights, g=0,...,ng 
  //   int numberOfBoundaryGrids(surface,level,face)    : =ng, number of grids with weights
  //   int boundaryGrid(surface,level,face,g)           : =grid : weights for "g" belong to componentGrid "grid"
  // ***************************************************************************************************

  const int numberOfDimensions=cgu.numberOfDimensions();
  const int numberOfRefinementLevels = cgu.numberOfRefinementLevels();
  
  if( boundaryWeights==NULL )
  {

    numberOfBoundarySurfaces=bd.numberOfSurfaces;
    numberOfBoundaryRefinementLevels=numberOfRefinementLevels;

    numberOfFacesPerSurface = new int[numberOfBoundarySurfaces];
    numberOfBoundaryFaces= new int[numberOfBoundarySurfaces];

    boundaryWeights = new RealArray** [numberOfBoundarySurfaces];
    pNumberOfBoundaryGrids = new int* [numberOfBoundarySurfaces];
    pBoundaryGrid         = new int** [numberOfBoundarySurfaces];
    for( int s=0; s<bd.numberOfSurfaces; s++ )
    {
      numberOfFacesPerSurface[s]=bd.numberOfFaces(surface);
      numberOfBoundaryFaces[s]=0;
      boundaryWeights[s]=NULL;
      pNumberOfBoundaryGrids[s]=NULL;
      pBoundaryGrid[s]=NULL;
    }
  }

  if( boundaryWeights[surface]==NULL )
  {
    numberOfBoundaryFaces[surface]=bd.numberOfFaces(surface);
    
    boundaryWeights[surface] = new RealArray* [numberOfRefinementLevels*bd.numberOfFaces(surface)];

    pNumberOfBoundaryGrids[surface] = new int [numberOfRefinementLevels*bd.numberOfFaces(surface)];
    pBoundaryGrid[surface] = new int * [numberOfRefinementLevels*bd.numberOfFaces(surface)];
    #define numberOfBoundaryGrids(surface,level,face) \
               pNumberOfBoundaryGrids[surface][(level)+(numberOfRefinementLevels)*(face)]
    #define boundaryGrid(surface,level,face,g) \
               pBoundaryGrid[surface][(level)+(numberOfRefinementLevels)*(face)][g]
	
    #define bWeights(surface,level,face,grid) \
       boundaryWeights[surface][(level)+(numberOfRefinementLevels)*(face)][grid]

    int level;
    for( level=0; level<numberOfRefinementLevels; level++ )
    {
      for( int face=0; face<bd.numberOfFaces(surface); face++ )
      {
	boundaryWeights[surface][(level)+(numberOfRefinementLevels)*(face)]=NULL;
	pBoundaryGrid  [surface][(level)+(numberOfRefinementLevels)*(face)]=NULL;
        numberOfBoundaryGrids(surface,level,face)=0;
      }
    }
        
    // realArray & weights = bWeights(surface,level,face,grid);
    // numberOfBoundaryGrids(surface,level,face)
    // boundaryGrid(surface,level,face,g) g=0,1,...,numberOfBoundaryGrids(surface,level,face)

    for( i=0; i<bd.numberOfFaces(surface); i++ )  // loop over faces in this surface
    {
      const int side = bd.boundaryFaces(0,i,surface);
      const int axis = bd.boundaryFaces(1,i,surface);
      const int grid = bd.boundaryFaces(2,i,surface);

      if( debug )
	display(cgu[grid].boundaryCondition(),"cgu[grid].boundaryCondition","%3i ");
	
      if( cgu[grid].boundaryCondition(side,axis)>0 )
      {
	// *** Count how many boundary grids live on each level ****

        // There is always 1 grid on level=0
	int level=0;
	int nbg=1;
	numberOfBoundaryGrids(surface,level,i)=nbg;
        pBoundaryGrid[surface][level+numberOfRefinementLevels*(i)] = new int[nbg];
	boundaryWeights[surface][level+numberOfRefinementLevels*(i)] = new RealArray[nbg];

	boundaryGrid(surface,level,i,0)=grid;
	    

	for( level=1; level<cgu.numberOfRefinementLevels(); level++ )
	{
	  GridCollection & rl = cgu.refinementLevel[level];
	  if( rl.numberOfComponentGrids()==0 )
	    continue;
	    
	  int *bGrid = new int[rl.numberOfComponentGrids()]; // there are at most this many boundary grids
	      
	  nbg=0;  // counts boundary grids at this level
	  for( int g=0; g<rl.numberOfComponentGrids(); g++ )
	  {
	    int gg = rl.gridNumber(g);        // index into cgu
	    int bg = cgu.baseGridNumber(gg);   // base grid for this refinement

            if( debug ) 
            printf(" check level=%i g=%i gg=%i bg=%i BC=%i \n",level,g,gg,bg,rl[g].boundaryCondition(side,axis));
	    
	    if( bg==grid && rl[g].boundaryCondition(side,axis)>0 )
	    {
              if( debug  )
		printf("surfaceIntegral:INFO: AMR correction: level=%i, g=%i, gg=%i grid=%i from base grid %i\n",
		       level,g,gg,grid,bg);

	      bGrid[nbg]=gg; 
	      nbg++;
	    }
	  }
	  numberOfBoundaryGrids(surface,level,i)=nbg;

          numberOfFacesPerSurface[surface]+=nbg;  // counts total number of faces  for this surface.
	  
	  if( nbg>0 )
	  {
	    boundaryWeights[surface][level+numberOfRefinementLevels*(i)] = new RealArray[nbg];
	    pBoundaryGrid[surface][level+numberOfRefinementLevels*(i)] = new int[nbg];

	    for( int g=0; g<nbg; g++ )
	      boundaryGrid(surface,level,i,g)=bGrid[g]; 
	  }
	  else
	  {
	    boundaryWeights[surface][level+numberOfRefinementLevels*(i)] = NULL;
	    pBoundaryGrid[surface][level+numberOfRefinementLevels*(i)] = NULL;
	  }
	  delete [] bGrid;
	      
	}
	    
      }
    }
  }

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  Index Ib1,Ib2,Ib3;

  for( i=0; i<bd.numberOfFaces(surface); i++ )
  {
    // ************* Add a contribution from a particular face ***************

    const int side = bd.boundaryFaces(0,i,surface);
    const int axis = bd.boundaryFaces(1,i,surface);
    const int grid = bd.boundaryFaces(2,i,surface);

    if( cgu[grid].boundaryCondition(side,axis)>0 &&
        boundaryWeights[surface][0+numberOfRefinementLevels*(i)]!=NULL  )
    {

      #ifdef USE_PPP
        realSerialArray weightsLocal; getLocalArrayWithGhostBoundaries(weights[grid],weightsLocal);
      #else
        const realSerialArray & weightsLocal = weights[grid];
      #endif

      int extra=1; // include the corner. is this needed??
      getBoundaryIndex(cgu[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3,1,extra);
      getGhostIndex(cgu[grid].gridIndexRange(),side,axis,I1,I2,I3,1,extra);
      int includeGhost=0;  // do NOT include parallel ghost
      bool ok = ParallelUtility::getLocalArrayBounds(weights[grid],weightsLocal,Ib1,Ib2,Ib3,includeGhost);
      ok = ParallelUtility::getLocalArrayBounds(weights[grid],weightsLocal,I1,I2,I3,includeGhost);

      int level=0, g=0;
      RealArray & baseGridWeights = bWeights(surface,level,i,g);
      // Note: coarse grid weights live on the ghost points, but make all AMR boundary weights live on
      // the boundary 
      if( ok )
      {
	baseGridWeights.redim(Ib1,Ib2,Ib3);
	baseGridWeights(Ib1,Ib2,Ib3) = weightsLocal(I1,I2,I3); // save a copy of the weights from the base grid
      }
      else
      {
	baseGridWeights.redim(0);
      }
      

      // ***********************************************************************
      //     For each level: 
      //        (1) compute weights on this level
      //        (2) adjust the weights on hidden points on the next lower level
      // ************************************************************************
      const int axisp1 = (axis+1) % numberOfDimensions;
      for( level=1; level<cgu.numberOfRefinementLevels(); level++ )
      {
	GridCollection & rl = cgu.refinementLevel[level];
	int g;
	for( g=0; g<numberOfBoundaryGrids(surface,level,i); g++ )
	{
          int gg = boundaryGrid(surface,level,i,g);

	  MappedGrid & cr = cgu[gg];             // refined grid
	  const intArray & mask = cr.mask();   // here is the mask 

	  cr.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseVertexDerivative);  
      
	  const realArray & jacobian    = cr.centerJacobian();
	  const realArray & rx          = cr.inverseVertexDerivative();
	  const RealArray & gridSpacing = cr.gridSpacing();

          #ifdef USE_PPP
            realSerialArray jacobianLocal; getLocalArrayWithGhostBoundaries(jacobian,jacobianLocal);
            realSerialArray rxLocal;       getLocalArrayWithGhostBoundaries(rx,rxLocal);
          #else
            const realSerialArray & jacobianLocal = jacobian;
            const realSerialArray & rxLocal       = rx;
          #endif


	  getBoundaryIndex(cr.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
          int includeGhost=0; 
          bool ok = ParallelUtility::getLocalArrayBounds(rx,rxLocal,Ib1,Ib2,Ib3,includeGhost);
	  if( !ok ) continue;
	  
          // *** Assign weights ***
	  RealArray & weights = bWeights(surface,level,i,g);
	  weights.redim(Ib1,Ib2,Ib3);
		
#define RXB(m,n) rxLocal(Ib1,Ib2,Ib3,m+numberOfDimensions*(n))
	  // where( mask(Ib1,Ib2,Ib3)!=0 )

	  if( numberOfDimensions==2 )
	  {
	    weights(Ib1,Ib2,Ib3)=( SQRT(SQR(RXB(axis,axis1))+SQR(RXB(axis,axis2)))*
				   fabs(jacobianLocal(Ib1,Ib2,Ib3))*gridSpacing(axisp1) );
		  
	    // weight end points by .5  (NOTE: If this grid has a neighbouring sibling, then the common
            //    end point will contribute twice with a factor or .5 -- giving the correct value) 

	    if( false )  // *wdh* 100226
	    {
	      weights(Ib1.getBase() ,Ib2.getBase() ,Ib3.getBase() )*=.5;  
	      weights(Ib1.getBound(),Ib2.getBound(),Ib3.getBound())*=.5;
	    }
	    else
	    {
	      // for parallel : 
	      for( int dir=0; dir<=1; dir++ )
	      {
		int ib1 = dir== 0 ? Ib1.getBase() : Ib1.getBound();
		int ib2 = dir== 0 ? Ib2.getBase() : Ib2.getBound();
		int ib3 = dir== 0 ? Ib3.getBase() : Ib3.getBound();
	      
		if( ( ib1==cr.gridIndexRange(0,0) || ib1==cr.gridIndexRange(1,0) ) &&
		    ( ib2==cr.gridIndexRange(0,1) || ib2==cr.gridIndexRange(1,1) ) &&
		    ( ib3==cr.gridIndexRange(0,2) || ib3==cr.gridIndexRange(1,2) ) )
		{
		  weights(ib1,ib2,ib3)*=.5;  // weight ends by .5 
		}
	      }
	    }
	    
            if( debug  )
              display(weights,sPrintF("weights for grid=%i (level=%i)",gg,level),debugFile,"%6.3f ");
	  }
	  else
	  {
	    printf(" ****Integrate: ERROR: AMR case not implemented in 3D yet. *****\n");
	  }
	
	}  // end for g
	

        // ********************************************************************************************
	// *** Make a list of boxes for all refinement grids at this level that cover this boundary ***
        // ********************************************************************************************

        #ifdef USE_PPP
	  printF("Integrate: AMR: finish me for parallel Bill!\n");
	  OV_ABORT("ERROR");
        #endif
   
        // NOTE: The boxes are coarsened to live on level-1 
	BoxList boundaryBoxes;
	buildBoundaryBoxes(cgu,grid,level,boundaryBoxes,side,axis );


	// *** adjust weights on the grids on the next coarser level ****
	for( int gc=0; gc<numberOfBoundaryGrids(surface,level-1,i); gc++ )
	{
	  int gridCoarse = boundaryGrid(surface,level-1,i,gc);
	  MappedGrid & mg = cgu[gridCoarse];
	      
	  RealArray & weightsCoarse = bWeights(surface,level-1,i,gc);
	      
	  Box box= cellCenteredBox(mg);
	  box.setSmall(axis,mg.gridIndexRange(side,axis));  // box lives on the boundary only
	  box.setBig  (axis,mg.gridIndexRange(side,axis));

          if( debug )
            cout << "Coarse grid box=" << box << endl; 

	  // Find the intersection between the boundaryBoxes of level with this the box for this grid

	  BoxList coarseBoxList;
	  coarseBoxList=intersect(boundaryBoxes,box); // **** boundaryBoxes should be coarsened above ****
	      
	  coarseBoxList.simplify(); // is this needed?

	  for( BoxListIterator bli(coarseBoxList); bli; ++bli)
	  {
	    Box box = coarseBoxList[bli];

	    if( debug ) cout << "intersection: box=" << box << endl;
            // ***** check this *****
	    for( int dir=0; dir<cgu.numberOfDimensions(); dir++ )
	    {
	      // int base = floorDiv(box.smallEnd(dir)+ratio-1,ratio); // round up
	      // int bound= floorDiv(box.bigEnd(dir)+1        ,ratio); // add one since we create node centered grids

	      int base = box.smallEnd(dir); 
   	      int bound= box.bigEnd(dir); 
              if( dir!=axis ) bound++;  // add one since we create node centered grids
	      Iv[dir]=Range(base,bound);
	    }


	    // Adjust the weights for hidden points 
	    //     --weight end points by 1/2 and interior hidden points by 0                

	    // Relative weights:
	    // 
	    //    .5  1  1  1  .5
	    //     +--+--+--+--+
	    //     +-----+-----+-----+-----+  ....
	    //     0     0     .5    1     1
	    //
	    K1=I1; K2=I2; K3=I3;
	    for( int dir=0; dir<cgu.numberOfDimensions(); dir++ )
	    {
              // (base bound) will mark "interior" hidden points where weights should be zero
	      int base =Iv[dir].getBase();    
	      int bound=Iv[dir].getBound();
		  
	      if( dir!=axis )
	      {
		if( Iv[dir].getBase()!=mg.gridIndexRange(0,dir) )
		{
		  base++;
		  J1=I1; J2=I2; J3=I3;
		  Jv[dir]=Iv[dir].getBase();
		  weightsCoarse(J1,J2,J3)*=.5;
		}
		if( Iv[dir].getBound()!=mg.gridIndexRange(1,dir) )
		{
		  bound--;
		  J1=I1; J2=I2; J3=I3;
		  Jv[dir]=Iv[dir].getBound();
		  weightsCoarse(J1,J2,J3)*=.5;
		}
	      }
	      Kv[dir]=Range(base,bound);
	    }

	    weightsCoarse(K1,K2,K3)=0.;   // zero out hidden interior weights on the coarse grid

	    if( debug  )
	    {
              printf(" Adjust weights for gridCoarse=%i : zero points Kv=[%i,%i][%i,%i][%i,%i]\n",
		     gridCoarse,K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound());
	      display(weightsCoarse,sPrintF("weights on the coarse grid, gridCoarse=%i",gridCoarse),"%6.3f ");
	    }
		
	  }
	} // end for gc

	boundaryBoxes.clear();
	    
      }  // end for level
    } // end if bc>0
  } // end for i (face)
    
  return 0;
}







int Integrate::
surfaceIntegral(const RealCompositeGridFunction & u, 
		const Range & C, 
		RealArray & integral,
		const int & surfaceNumber /* = -1 */ )
// ================================================================================================
/// \brief 
///     Compute the surface integral of u, one or more components.
/// \param u (input) : function to integrate. This function must be defined at the appropriate points.
/// \param C (input) : integrate these components.
/// \param integral (output): array of values, {\tt integral(C)}, the integrals of the components.
/// \param surfaceNumber (input) : the surface identifier as defined through a call to {\tt defineSurface}.
///     If no   surfaceNumber is specified then the entire surface will be integrated.
/// 
/// \param Note: For AMR grids one should call updateForAMR(cg) after a AMR regridding step and before
///      calling this function.
/// \param Note: Currently the surface integral for AMR grids only works in 2D and when there is a
///     single grid on the surface -- i.e. overlapping surface grids are not handled yet.
/// \author WDH
// ================================================================================================
{
  // **** determine surface ****
  const int myid=max(0,Communication_Manager::My_Process_Number);

  int side,axis,grid;
  Index I1,I2,I3,Ib1,Ib2,Ib3;

  CompositeGrid & cgu = *u.getCompositeGrid();
  const bool adaptiveGrid = cgu.numberOfRefinementLevels()>1 &&
                            cgu.numberOfDimensions()==2;  // ********************************************************

  if( adaptiveGrid && useAMR )
  { // build AMR integration weights for this surface if they are not already built.
    buildAdaptiveMeshRefinementSurfaceWeights(cgu,surfaceNumber);
  }

  const int cBase=C.getBase();

  integral(C)=0.;

  if( surfaceNumber==-1 )
  {
    // **************************************
    // **** Integrate over all boundaries ***
    // **************************************


    if( !allFaceWeightsDefined )
    {
      // we should first determine if we can compute the weights  for this surface directly in the
      // case when there are no overlapping grids on the boundary.

      if( computeSurfaceWeights(surfaceNumber) !=0 )
	computeWeights();  // fall back method
    }

    real *sumi = new real [C.getLength()];

    // integrate the entire boundary
    for( grid=0; grid<cg.numberOfBaseGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];

      #ifdef USE_PPP
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        realSerialArray weightsLocal; getLocalArrayWithGhostBoundaries(weights[grid],weightsLocal);
      #else
        const realSerialArray & uLocal = u[grid];
        const realSerialArray & weightsLocal = weights[grid];
      #endif

      int extra=1; // include the corner. is this needed??
      bool ok;
      for( axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	for( side=Start; side<=End; side++ )
	{
          if( c.boundaryCondition(side,axis)>0 )
	  {
	    getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3,1,extra);
	    getGhostIndex(cg[grid].gridIndexRange(),side,axis,I1,I2,I3,1,extra);
            int includeGhost=0;  // do NOT include parallel ghost
            ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,Ib1,Ib2,Ib3,includeGhost);
            ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);
	    if( !ok ) continue;
	    
            for( int n=cBase; n<=C.getBound(); n++ )
	    {
  	      integral(n)+=sum(uLocal(Ib1,Ib2,Ib3,n)*weightsLocal(I1,I2,I3));
              // printf(" surfaceIntegral: myid=%i integral=%8.2e\n",myid,integral(n));
	    }
	    
	  }
	}
      }
    }

    CompositeGrid & cgu = *u.getCompositeGrid();
    if( cgu.numberOfRefinementLevels()>1 )
    {
      // This is an AMR grid -- make corrections to the integral for refinement grids

//  	  if( numberOfDimensions==2 )
//  	  {
//  	    weightsg(Ig1,Ig2,Ig3)=SQRT(SQR(RX(axis,axis1))+SQR(RX(axis,axis2)))*
//  	      fabs(jacobian(I1,I2,I3))*gridSpacing(axisp1);
//  	  }
//  	  else
//  	  {
//  	    weightsg(Ig1,Ig2,Ig3)=SQRT(SQR(RX(axis,axis1))+SQR(RX(axis,axis2))+SQR(RX(axis,axis3)))*
//  	      fabs(jacobian(I1,I2,I3))*gridSpacing(axisp1)*gridSpacing(axisp2);
//  	  }
	  
    }
    

  }
  else
  {
    // ***********************************************************************
    // **** In this case we only integrate over a sub-set of the boundary ****
    // ***********************************************************************

    int i, surface=-1;
    surface=bd.surfaceIndex(surfaceNumber);
    if( surface<0 || surface >= bd.numberOfSurfaces )
    {
      printF("surfaceIntegral: invalid surface index = %i for surfaceNumber=%i \n",surface,surfaceNumber);
      return 1;
    }

    if( !surfaceWeightsDefined(surface) )
    {
      // we should first determine if we can compute the weights  for this surface directly in the
      // case when there are no overlapping grids on the boundary.

      if( computeSurfaceWeights(surfaceNumber) !=0 )
	computeWeights();  // fall back method
    }

    CompositeGrid & cgu = *u.getCompositeGrid();
    // const bool adaptiveGrid = cgu.numberOfRefinementLevels()>1;

    const int numberOfDimensions=cg.numberOfDimensions();
    const int numberOfRefinementLevels = cgu.numberOfRefinementLevels();
    
    for( i=0; i<bd.numberOfFaces(surface); i++ )
    {
      // ************* Add a contribution from a particular face ***************

      side = bd.boundaryFaces(0,i,surface);
      axis = bd.boundaryFaces(1,i,surface);
      grid = bd.boundaryFaces(2,i,surface);

      if( cg[grid].boundaryCondition(side,axis)>0 )
      {

	if( !adaptiveGrid || !useAMR )
	{
	  MappedGrid & mg = cg[grid];
          #ifdef USE_PPP
	    realSerialArray uLocal;        getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	    realSerialArray weightsLocal;  getLocalArrayWithGhostBoundaries(weights[grid],weightsLocal);
          #else
	    const realSerialArray & uLocal        = u[grid];
	    const realSerialArray & weightsLocal  = weights[grid];
          #endif


	  int extra=1; // include the corner. is this needed??
	  getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3,1,extra);
	  getGhostIndex(cg[grid].gridIndexRange(),side,axis,I1,I2,I3,1,extra);

	  int includeGhost=0;
	  bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,Ib1,Ib2,Ib3,includeGhost); 
	  ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost); 
	  if( !ok ) continue;

          if( debug & 2 ) 
            printf(" ***** surfaceIntegral: body=%i face=%i (grid,side,axis)=(%i,%i,%i)\n",surface,i,
                    grid,side,axis);

	  for( int n=C.getBase(); n<=C.getBound(); n++ )
	  {
	    integral(n)+=sum(uLocal(Ib1,Ib2,Ib3,n)*weightsLocal(I1,I2,I3));
	    if( debug & 4  )
	    {
	      printf("surfaceIntegral: n=%i (side,axis,grid)=(%i,%i,%i), integral=%9.3e\n",n,side,axis,grid,integral(n));
	      display(u[grid](Ib1,Ib2,Ib3,n),"surfaceIntegral: u[grid](Ib1,Ib2,Ib3,n)","%5.2f ");
	      display(weights[grid](I1,I2,I3),"surfaceIntegral: weights[grid](I1,I2,I3)","%5.2f ");
	    }
	  }
	}
	else
	{
	  // ************* adaptive grid case ********************

          // numberOfBoundaryGrids(surface,level,face)
          // boundaryGrid(surface,level,face,g) g=0,1,...,numberOfBoundaryGrids(surface,level,face)

	  for( int level=0; level<numberOfRefinementLevels; level++ )
	  {
            if( debug & 2 ) 
              printf(" ***** surfaceIntegral:AMR body=%i face=%i level=%i numberOfBoundaryGrids=%i\n",
                surface,i,level,numberOfBoundaryGrids(surface,level,i));
	    
            for( int g=0; g<numberOfBoundaryGrids(surface,level,i); g++ )
	    {
	      int gg = boundaryGrid(surface,level,i,g);
              MappedGrid & mg = cgu[gg];
	      
    	      RealArray & weights = bWeights(surface,level,i,g);

              #ifdef USE_PPP
	        realSerialArray uLocal;        getLocalArrayWithGhostBoundaries(u[gg],uLocal);
              #else
	        const realSerialArray & uLocal        = u[gg];
              #endif

              Ib1=weights.dimension(0);
	      Ib2=weights.dimension(1);
	      Ib3=weights.dimension(2);

	      int includeGhost=0;
	      bool ok = ParallelUtility::getLocalArrayBounds(u[gg],uLocal,Ib1,Ib2,Ib3,includeGhost); 
              if( !ok ) continue;
	      
	      for( int n=C.getBase(); n<=C.getBound(); n++ )
	      {
		// Add contribution from the base grid or refinement grid 
                if( debug & 4 )
                  display(weights(Ib1,Ib2,Ib3),sPrintF("surface integral: weights for grid gg=%i (level=%i)",gg,level),"%6.3f ");
		
		real partialSum=sum(uLocal(Ib1,Ib2,Ib3,n)*weights(Ib1,Ib2,Ib3));

                if( debug & 4 )
		{
                  printf(" ... contribution to integral from grid gg=%i is %10.4e\n",gg,partialSum);
                  display(weights(Ib1,Ib2,Ib3),"weights","%4.2f ");
		}
		
		integral(n)+=partialSum; 
	      }
	    }
	  }
	} // end if adaptive grid
	
      } // end if bc>0
      
    } // end for i (face)

  }
    
  #ifdef USE_PPP
   real *sumi = new real [C.getLength()];
   // sum over processors
   ParallelUtility::getSums( &integral(cBase),sumi,C.getLength() );
   for( int n=cBase; n<=C.getBound(); n++ )
     integral(n)=sumi[n-cBase];
   delete [] sumi;
  #endif
  
  return 0;
}

real Integrate::
volume()
// ================================================================================================
/// \details 
///     Return the total volume. Compute weights as needed.
/// \author: WDH
// ================================================================================================
{
  if( !weightsComputed )
    computeWeights();

  real vIntegral=0.;
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];

    #ifdef USE_PPP
      realSerialArray weightsLocal;  getLocalArrayWithGhostBoundaries(weights[grid],weightsLocal);
    #else
      const realSerialArray & weightsLocal  = weights[grid];
    #endif

    real *weightsp = weightsLocal.Array_Descriptor.Array_View_Pointer2;
    const int weightsDim0=weightsLocal.getRawDataSize(0);
    const int weightsDim1=weightsLocal.getRawDataSize(1);
#undef WEIGHTS
#define WEIGHTS(i0,i1,i2) weightsp[i0+weightsDim0*(i1+weightsDim1*(i2))]

    getIndex(c.gridIndexRange(),I1,I2,I3);
    int includeGhost=0; // do NOT include parallel ghost 
    bool ok = ParallelUtility::getLocalArrayBounds(weights[grid],weightsLocal,I1,I2,I3,includeGhost); 
    if( !ok ) continue;
    
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      vIntegral += WEIGHTS(i1,i2,i3);
    }
  }
  real volumeIntegral = ParallelUtility::getSum(vIntegral);
  
  return volumeIntegral;
}


real Integrate::
volumeIntegral( const RealCompositeGridFunction & u, const int component /* =0 */ )
// ================================================================================================
/// \brief
///     Compute the volume integral of u. 
/// \param u (input) : function to integrate. This function must be defined at the appropriate points.
/// \param component (input) : integrate this component
/// \author WDH
// ================================================================================================
{
  Range C(component,component);
  RealArray integral(C);
  volumeIntegral( u,C,integral );
  return integral(component);
}


int Integrate::
volumeIntegral( const RealCompositeGridFunction & u, 
		const Range & C, 
		RealArray & integral )
// ================================================================================================
/// \brief
///     Compute the volume integral of some components of u. 
/// \param u (input) : function to integrate. This function must be defined at the appropriate points.
/// \param C (input) : integrate these components.
/// \param integral(C) : return results here.
/// \author WDH
// ================================================================================================
{
  if( !weightsComputed )
    computeWeights();

  const int cBase=C.getBase();
  integral(C)=0.;

  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];

    #ifdef USE_PPP
      realSerialArray uLocal;        getLocalArrayWithGhostBoundaries(u[grid],uLocal);
      realSerialArray weightsLocal;  getLocalArrayWithGhostBoundaries(weights[grid],weightsLocal);
    #else
      const realSerialArray & uLocal        = u[grid];
      const realSerialArray & weightsLocal  = weights[grid];
    #endif

    real *weightsp = weightsLocal.Array_Descriptor.Array_View_Pointer2;
    const int weightsDim0=weightsLocal.getRawDataSize(0);
    const int weightsDim1=weightsLocal.getRawDataSize(1);
#undef WEIGHTS
#define WEIGHTS(i0,i1,i2) weightsp[i0+weightsDim0*(i1+weightsDim1*(i2))]
    real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=uLocal.getRawDataSize(0);
    const int uDim1=uLocal.getRawDataSize(1);
    const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    getIndex(c.gridIndexRange(),I1,I2,I3);
    int includeGhost=0; // do NOT include parallel ghost 
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost); 
    if( !ok ) continue;
    
    int i1,i2,i3;
    for( int n=cBase; n<=C.getBound(); n++ )
    {
      real vIntegral=0.;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	vIntegral += U(i1,i2,i3,n)*WEIGHTS(i1,i2,i3);
      }
      integral(n) += vIntegral;
    }
  }
  #ifdef USE_PPP
   real *sumi = new real [C.getLength()];
   // sum over processors
   ParallelUtility::getSums( &integral(cBase),sumi,C.getLength() );
   for( int n=cBase; n<=C.getBound(); n++ )
     integral(n)=sumi[n-cBase];
   delete [] sumi;
  #endif

  return 0;
}

  
int Integrate::
computeWeights()
// ================================================================================================
/// \brief  Compute the integration weights. This is a protected routine.
///
/// \details This routine will solve for all the surface and volume weights by 
///   solving a linear system of equations. 
/// 
/// \author WDH
// ================================================================================================
{
  const int numberOfDimensions = cg.numberOfDimensions();
  int grid;
  
  if( debug & 1 )
  {
    printF("Integrate::computeWeights: orderOfAccuracy= %i\n",orderOfAccuracy);

    if( debugFile!=NULL )
      fprintf(debugFile,"Integrate::computeWeights: orderOfAccuracy= %i\n",orderOfAccuracy);
  }
  
  // make a grid function to hold the coefficients
  Range all;
  const int width = orderOfAccuracy==2 ? 3 : 5;
  
  const int stencilSize=int( pow(width,numberOfDimensions)+1 );  // add 1 for interpolation equations
  if( !weightsUpdatedToMatchGrid )
  {
    weightsUpdatedToMatchGrid=true;
    weights.updateToMatchGrid(cg);
  }
  
  
  bool isAxisymmetric = radialAxis!=-1 && cg.numberOfDimensions()==2;

  RealCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  const int numberOfGhostLines= orderOfAccuracy==2 ? 1 : 2;
  coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
    
  realCompositeGridFunction f(cg);

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  op.setOrderOfAccuracy(orderOfAccuracy);
  coeff.setOperators(op);
  
  // coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // *wdh* 100224 -- fix for parallel: 
  coeff=0.; // this is needed
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
    op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid],I1,I2,I3);
  }

  if ( isAxisymmetric )
  {
    cg.update(MappedGrid::THEcenter);

    // finish me for parallel ...

    realCompositeGridFunction urCoeff;
    urCoeff = radialAxis==0 ? op.xCoefficients() : op.yCoefficients();

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid &mg = cg[grid];
      realArray &center = mg.center();
      Index I1,I2,I3;
      getIndex(mg.indexRange(),I1,I2,I3,numberOfGhostLines,numberOfGhostLines);
      realArray radius(center(I1,I2,I3,radialAxis));
      radius.reshape(1,I1,I2,I3);
      for ( int s=0; s<stencilSize; s++ )
      {
	where ( radius>FLT_EPSILON )
	{
	  coeff[grid](s,I1,I2,I3) += urCoeff[grid](s,I1,I2,I3)/radius;
	}
      }
      radius.reshape(I1,I2,I3);
    }
  }

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( !cg[grid].isRectangular() )
      cg[grid].update(MappedGrid::THEvertexBoundaryNormal );
  }
  

  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::allBoundaries); // kkc for the axisymmetric case this needs to be mixed...?
  if( orderOfAccuracy==4 )
  {
    BoundaryConditionParameters extrapParams;
    extrapParams.ghostLineToAssign=2;
    extrapParams.orderOfExtrapolation=4;
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries,extrapParams);
  }
  coeff.finishBoundaryConditions();

  if( solver==NULL )
    solver = new Oges(cg);
  else
    solver->updateToMatchGrid(cg);
  
  bool useIterativeSolver= numberOfDimensions==3; // *************
  if( true || useIterativeSolver )
  {
    solver->set(OgesParameters::THEbestIterativeSolver); // *************
    // solver->set(OgesParameters::PETSc);
    // solver->set(OgesParameters::SLAP);
    // count the total number of grid points.
    int numberOfGridPoints=0;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      numberOfGridPoints+=cg[grid].mask().elementCount();

    // Oges::debug=7;
    solver->set(OgesParameters::THEtolerance,tolerance*numberOfGridPoints);
  }    

  solver->set(OgesParameters::THEsolveForTranspose,true); // solve the transpose system (we want the left null vector)
  solver->set(OgesParameters::THEfixupRightHandSide,false);     // no need to zero out equations at special points.
  // *wdh* 100225 : we must not rescale the row norms for parallel PETSc since we scale before taking the transpose
  //    which does not give the intended result
  solver->set(OgesParameters::THErescaleRowNorms,false);   

  bool solveSingularProblem=false;  // false -> use new method where we know some weights a priori

  if( debug & 1 )
    printF("Integrate:computeWeights: solver to compute weights: %s\n",(const char *)solver->parameters.getSolverName());

  // assign the rhs: f=0 except for the rhs to the compatibility equation which we set to 1
  // (this will cause the sum of the interior weights to be 1)
  f=0.;
#define RX(m,n) rx(I1,I2,I3,m+numberOfDimensions*(n))
#define RXLocal(m,n) rxLocal(I1,I2,I3,m+numberOfDimensions*(n))
  if( solveSingularProblem )
  {
/* ----
    solver->setCoefficientArray( coeff );     // supply coefficients
    if( solveSingularProblem )
      solver->set(OgesParameters::THEcompatibilityConstraint,true); // system is singular so add an extra equation
    solver->initialize();

    // find the equation where the compatibility constraint is put (some unused point)
    int n,i1e,i2e,i3e,gride;
    solver->equationToIndex(solver->extraEquationNumber(0),n,i1e,i2e,i3e,gride);
    f[gride](i1e,i2e,i3e,n)=1.;
     

    if( useIterativeSolver )
    {
      // choose a smart initial guess for iterative solvers
      weights=0.;
      cg.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseVertexDerivative);
      Index Ig1,Ig2,Ig3;
    
      f[gride](i1e,i2e,i3e,n)=0.;
      for(grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];
	const RealArray & jacobian = c.centerJacobian();
	const RealArray & rx = c.inverseVertexDerivative();
	const IntegerDistributedArray & classify = coeff[grid].sparse->classify;
	const RealArray & gridSpacing = c.gridSpacing();

	real dr = c.gridSpacing(0)*c.gridSpacing(1)*c.gridSpacing(2);
	// printf(" dr = %e\n",dr);
      
	getIndex(c.gridIndexRange(),I1,I2,I3);
	where( classify(I1,I2,I3)==SparseRepForMGF::interior || classify(I1,I2,I3)==SparseRepForMGF::boundary )
	{
	  weights[grid](I1,I2,I3)=jacobian(I1,I2,I3)*dr;
	}
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  for( int side=Start; side<=End; side++ )
	  {
	    if( c.boundaryCondition(side,axis)>0 )
	    {
	      getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
	      getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	      if( numberOfDimensions==2 )
	      {
		weights[grid](Ig1,Ig2,Ig3)=-SQRT(SQR(RX(axis,axis1))+SQR(RX(axis,axis2)))*
		  fabs(jacobian(I1,I2,I3))*gridSpacing(1-axis);
	      }
	      else
	      {
		weights[grid](Ig1,Ig2,Ig3)=-SQRT(SQR(RX(axis,axis1))+SQR(RX(axis,axis2))+SQR(RX(axis,axis3)))*
		  fabs(jacobian(I1,I2,I3))*
		  gridSpacing((axis+1)%numberOfDimensions)*gridSpacing((axis+2)%numberOfDimensions);
	      }
	    }
	  }
	}
	getIndex(c.dimension(),I1,I2,I3);
	f[gride](i1e,i2e,i3e)+=sum(solver->rightNullVector[grid](I1,I2,I3)*weights[grid](I1,I2,I3));
      }
    }
  
    solver->solve( weights,f );   // solve for the (unscaled) weights

    // weights.display("Here are the unscaled weights");
  
    // scale the weights (by a constant) so that they will be integration weights

    // Oges::debug=31; 
    printf("scaleIntegrationCoefficients...\n");
    solver->scaleIntegrationCoefficients( weights ); 
    printf("...done scaling\n");
 ---- */

  }
  else
  {
    // Assign the weights at known points -- replace PDE by the identity on the COLUMN (since we solve the transpose)

    // NOTE We could form the sparse matrix (ia,ja,aij) -> transpose it and then replace the equations


    cg.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseVertexDerivative);

    Index Ig1,Ig2,Ig3;
    bool someWeightsHaveBeenSet=false;
    
    for(int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      const realArray & jacobian = c.centerJacobian();
      const realArray & rx = c.inverseVertexDerivative();
      const IntegerDistributedArray & classify = coeff[grid].sparse->classify;
      const RealArray & gridSpacing = c.gridSpacing();
      const realArray &center = isAxisymmetric ? c.center() : Overture::nullRealDistributedArray();
      IntegerDistributedArray & mask = c.mask();

      const int *pgid = &(c.gridIndexRange(0,0));
      #define GID(side,axis) (pgid[(side)+2*(axis)])
      // #define GID(side,axis) (c.gridIndexRange(side,axis))

      realArray & coeffg = coeff[grid];
      realArray & weightsg = weights[grid];
      realArray & fg = f[grid];
      Range M(0,stencilSize-1);
      // const int m0=width*width/2;

      real dr = c.gridSpacing(0)*c.gridSpacing(1)*c.gridSpacing(2);  // volume element in r
      // printf(" dr = %e\n",dr);
      
      #ifdef USE_PPP
        realSerialArray fLocal;        getLocalArrayWithGhostBoundaries(fg,fLocal);
        realSerialArray weightsLocal;  getLocalArrayWithGhostBoundaries(weightsg,weightsLocal);
        realSerialArray jacobianLocal; getLocalArrayWithGhostBoundaries(jacobian,jacobianLocal);
        realSerialArray coeffLocal;    getLocalArrayWithGhostBoundaries(coeffg,coeffLocal);
        intSerialArray maskLocal;      getLocalArrayWithGhostBoundaries(mask,maskLocal);
        intSerialArray classifyLocal;  getLocalArrayWithGhostBoundaries(classify,classifyLocal);
        realSerialArray rxLocal;       getLocalArrayWithGhostBoundaries(rx,rxLocal);
        realSerialArray centerLocal;   getLocalArrayWithGhostBoundaries(center,centerLocal);
      #else
        const realSerialArray & fLocal        = fg;
        realSerialArray & weightsLocal  = weightsg;
        const realSerialArray & jacobianLocal = jacobian;
        const realSerialArray & coeffLocal    = coeffg;
        const intSerialArray & maskLocal      = mask; 
        const intSerialArray & classifyLocal  = classify; 
        const realSerialArray & rxLocal       = rx;
        const realSerialArray & centerLocal   = center;
      #endif

      real *coeffp = coeffLocal.Array_Descriptor.Array_View_Pointer3;
      const int coeffDim0=coeffLocal.getRawDataSize(0);
      const int coeffDim1=coeffLocal.getRawDataSize(1);
      const int coeffDim2=coeffLocal.getRawDataSize(2);
#undef COEFF
#define COEFF(i0,i1,i2,i3) coeffp[i0+coeffDim0*(i1+coeffDim1*(i2+coeffDim2*(i3)))]

      const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
      const int maskDim0=maskLocal.getRawDataSize(0);
      const int maskDim1=maskLocal.getRawDataSize(1);
      const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

      real *weightsp = weightsLocal.Array_Descriptor.Array_View_Pointer2;
      const int weightsDim0=weightsLocal.getRawDataSize(0);
      const int weightsDim1=weightsLocal.getRawDataSize(1);
#undef WEIGHTS
#define WEIGHTS(i0,i1,i2) weightsp[i0+weightsDim0*(i1+weightsDim1*(i2))]

      real *centerp = centerLocal.Array_Descriptor.Array_View_Pointer3;
      const int centerDim0=centerLocal.getRawDataSize(0);
      const int centerDim1=centerLocal.getRawDataSize(1);
      const int centerDim2=centerLocal.getRawDataSize(2);
#undef CENTER
#define CENTER(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]

      real *fp = fLocal.Array_Descriptor.Array_View_Pointer2;
      const int fDim0=fLocal.getRawDataSize(0);
      const int fDim1=fLocal.getRawDataSize(1);
#undef F
#define F(i0,i1,i2) fp[i0+fDim0*(i1+fDim1*(i2))]

      real *jacobianp = jacobianLocal.Array_Descriptor.Array_View_Pointer2;
      const int jacobianDim0=jacobianLocal.getRawDataSize(0);
      const int jacobianDim1=jacobianLocal.getRawDataSize(1);
#undef JACOBIAN
#define JACOBIAN(i0,i1,i2) jacobianp[i0+jacobianDim0*(i1+jacobianDim1*(i2))]

      int *classifyp = classifyLocal.Array_Descriptor.Array_View_Pointer2;
      const int classifyDim0=classifyLocal.getRawDataSize(0);
      const int classifyDim1=classifyLocal.getRawDataSize(1);
#undef CLASSIFY
#define CLASSIFY(i0,i1,i2) classifyp[i0+classifyDim0*(i1+classifyDim1*(i2))]

      weightsLocal=0.;

      int i1,i2,i3;
      getIndex(c.gridIndexRange(),I1,I2,I3);

      bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3); 
      if( ok ) 
      {
	if ( !isAxisymmetric )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( CLASSIFY(i1,i2,i3)==SparseRepForMGF::interior || CLASSIFY(i1,i2,i3)==SparseRepForMGF::boundary )
	    {
	      WEIGHTS(i1,i2,i3)=JACOBIAN(i1,i2,i3)*dr;
	    }
	  }
	}
	else
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( CLASSIFY(i1,i2,i3)==SparseRepForMGF::interior || CLASSIFY(i1,i2,i3)==SparseRepForMGF::boundary )
	    {
	      WEIGHTS(i1,i2,i3)=CENTER(i1,i2,i3,radialAxis)*JACOBIAN(i1,i2,i3)*dr;
	    }
	  }
	}
      }
      
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( c.boundaryCondition(side,axis)>0 )
	  {
	    getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
	    getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
            int includeGhost=1; // what should this be ? 
	    ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
	    ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ig1,Ig2,Ig3,includeGhost);
            if( ok )
	    {
	      if( numberOfDimensions==2 )
	      {
		if ( !isAxisymmetric )
		{
		  weightsLocal(Ig1,Ig2,Ig3)=-SQRT(SQR(RXLocal(axis,axis1))+SQR(RXLocal(axis,axis2)))*
		    fabs(jacobianLocal(I1,I2,I3))*gridSpacing(1-axis);
		}
		else
		{
		  weightsLocal(Ig1,Ig2,Ig3)=-SQRT(SQR(RXLocal(axis,axis1))+SQR(RXLocal(axis,axis2)))*
		    fabs(jacobianLocal(I1,I2,I3))*gridSpacing(1-axis)* centerLocal(I1,I2,I3,radialAxis);
		}
	      }
	      else
	      {
		weightsLocal(Ig1,Ig2,Ig3)=( -SQRT(SQR(RXLocal(axis,axis1))+
						  SQR(RXLocal(axis,axis2))+
						  SQR(RXLocal(axis,axis3)))*
					    fabs(jacobianLocal(I1,I2,I3))*
					    gridSpacing((axis+1)%numberOfDimensions)*
					    gridSpacing((axis+2)%numberOfDimensions) );
	      }
	    } // end if ok
	    
            // for now only specify values on boundaries that have no interpolation points.

            // *wdh* 100225 -- we have already determined if the boundary has interp pts on it
            int count = (int)boundaryHasOverlap(side,axis,grid);
	    if( count<0 ) // this means we have not determined boundaryHasOverlap yet
	    {
              // *wdh* 2011/12/10 -- sometimes we have not already determined boundaryHasOverlap
              // eg. if we compute volume weights first
              Index Ib1,Ib2,Ib3;
	      getBoundaryIndex(c.extendedIndexRange(),side,axis,Ib1,Ib2,Ib3);
	      int includeGhost=1; 
	      bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ib1,Ib2,Ib3,includeGhost);
	      count=0;
	      if( ok ) 
		count=sum(maskLocal(Ib1,Ib2,Ib3)<0);
	      count=ParallelUtility::getSum(count);

              if( count==0 )
                boundaryHasOverlap(side,axis,grid)=false;
	      else
                boundaryHasOverlap(side,axis,grid)=true;

	      if( debug & 1 )
		printF("Integrate::computeWeights:INFO:computed: boundaryHasOverlap(side=%i,axis=%i,grid=%i)=%i.\n",
		       side,axis,grid,boundaryHasOverlap(side,axis,grid));

	      // OV_ABORT("Error");
	    }
	    
	    if( count==0 )
	    {
              if( debug & 1 )
		printF("Integrate: we can specify exact weights for (side,axis)=(%i,%i) of grid %s \n",side,axis,
		       (const char*)c.getName());
	    }
	    else
	    {
	      // We only set boundaries with interp. pts if we have not already encountered a bndry with
              // no interp. pts: 
	      if( someWeightsHaveBeenSet )
	        continue;
	    }
	    
	    someWeightsHaveBeenSet=true;

	    // getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);

	    const int extra= -(width/2);
	    getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3,extra);  // avoid boundaries !
	    getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,extra);

            includeGhost=1; // what should this be ? 
	    ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
	    ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ig1,Ig2,Ig3,includeGhost);

            if( !ok ) continue;

	    const int halfWidth=width/2;
#undef M2
#define M2(m1,m2) ((m1)+halfWidth+width*((m2)+halfWidth))
#undef M3
#define M3(m1,m2,m3) ((m1)+halfWidth+width*((m2)+halfWidth+width*((m3)+halfWidth)))
	    int is1 = (axis==axis1) ? 1-2*side : 0;   
	    int is2 = (axis==axis2) ? 1-2*side : 0;           
	    int is3 = (axis==axis3) ? 1-2*side : 0;           
	    // coefficient index for ghost value:
	    int mGhost = numberOfDimensions==2 ? M2(-is1,-is2) : M3(-is1,-is2,-is3);   
	    // ***** we need to set the COLUMN equation to the identity!
	    //       [ -1  0  1               ]    [ a11 a12 a13 ...
	    // A   = [  1 -2  1               ]    [ a21 a22 a23 ...
	    //       [  0  1 -2  1            ]  = [ a31 a32 a33 ...
	    //       [  0  0  1 -2 1  ...     ]    [ 
	    // 
	    // The transpose looks like: 
	    //       [ -1  1                 ]    [ a11 a21 a31 ...
	    // A^T = [  0 -2  1              ]    [ a12 a22 a32
	    //       [ +1  1 -2  1           ]  = [ a13 a23 a33 ...
	    //       [  0  0  1 -2 1  ...    ]    [ 
	    //        
	    // 
	    // To set the first equation in A^T to be the identity we need to
	    // set elements a11=1 and a21=0
	    //
	    // NOTE: we need to set or zero out all matrix entries of A that multiply the value U_ij whose
	    //       equation we are trying to set in A^T since these will be the entries in A^T that form the equation for U_ij
	    // scale the equation for the non-orthogonal case : Make the diagonal the biggest entry by far so that
	    // we solve :  
	    //       scale*u(ghost) = scale*true_surface_area + (possible other smaller stuff)
	    real scale=1.e10;  
	    int j1,j2,j3;
	    if( count==0 )
	    {
	      // **** This assumes an orthogonal grid *****
	      FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,Ig1,Ig2,Ig3)
	      {
		if( MASK(i1,i2,i3)>0 )
		{
		  COEFF(mGhost,i1,i2,i3)=0.;        // Equation on the boundary has an entry that multiplies the value on the ghost-pt
		  COEFF(mGhost,j1,j2,j3)=scale;     // Neumann BC is centered on the ghost point -- specify solution on ghost = surface area
		  F(j1,j2,j3)=scale*WEIGHTS(j1,j2,j3);
		}
	      }
	    }
	    else
	    {
	      // If the boundary has interp pts on it then only set pts that are not close to interp pts. 
	      // This should usually work but is not a guarantee!

	      if( numberOfDimensions==2 )
	      {
		FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,Ig1,Ig2,Ig3)
		{ // only set the surface weight if there are no interp pts nearby:
		  if( MASK(i1,i2,i3)>0 && MASK(i1+is1,i2+is2,i3+is3)>0 && MASK(i1-is1,i2-is2,i3-is3)>0 && 
		      MASK(i1+2*is1,i2+2*is2,i3+2*is3)>0 && MASK(i1-2*is1,i2-2*is2,i3-2*is3)>0 
		    )
		  {
		    COEFF(mGhost,i1,i2,i3)=0.;    
		    COEFF(mGhost,j1,j2,j3)=1.;   
		    F(j1,j2,j3)=WEIGHTS(j1,j2,j3);
		  }
		}
	      }
	      else // 3D
	      {

		// int width1=5, width2=5, width3=5;
		// int width1=4, width2=4, width3=4;
		int width1=3, width2=3, width3=3;
		// int width1=2, width2=2, width3=2;
		// int width1=1, width2=1, width3=1;
		const int i1a = GID(0,0)-1, i1b=GID(1,0)+1;
		const int i2a = GID(0,1)-1, i2b=GID(1,1)+1;
		const int i3a = GID(0,2)-1, i3b=GID(1,2)+1;
                bool ok;
		FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,Ig1,Ig2,Ig3)
		{
		  ok = MASK(i1  ,i2  ,i3  )>0;
		  for( int w3=-width3; w3<=width3 && ok; w3++ )
		  {
		    if( i3+w3>=i3a && i3+w3<=i3b )
		    {
		      for( int w2=-width2; w2<=width2 && ok; w2++ )
		      {
			if( i2+w2>=i2a && i2+w2<=i2b )
			{
			  for( int w1=-width1; w1<=width1; w1++ )
			  {
			    if( i1+w1>=i1a && i1+w1<=i1b &&  
				MASK(i1+w1,i2+w2,i3+w3)<=0 )
			    {
			      ok=false;
			      break;
			    }
			  }
			}
		      }
		    }
		  }
		      
		  if( ok )
		  {
		    COEFF(mGhost,i1,i2,i3)=0.;    
		    COEFF(mGhost,j1,j2,j3)=scale;   
		    F(j1,j2,j3)=scale*WEIGHTS(j1,j2,j3);
		  }
		}
	      }
	    }
	      
	  }

	}

      }
    }
    if( !someWeightsHaveBeenSet )
    {
      printf("Integrate:ERROR: no weights have been set before the solve! This will not work!\n");
      OV_ABORT("error");
    }
    if( debug & 2 )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	displayCoeff(coeff[grid],"coeff before");
      }
      
    }
    

    solver->setCoefficientArray( coeff );     // supply coefficients
    // Oges::debug=63; 
    if( debug & 2  )
      weights.display("Here is the initial guess for the weights","%8.1e");
    if( debug & 2  )
      f.display("Here is the RHS","%8.1e");
    
    if( debug & 1  )
    {
      printF("Integrate:call solver->solve() to solve the linear system for the weights...\n");
    }
    real cpu0=getCPU();

    solver->solve( weights,f );   // solve for the scaled weights

    if( debug & 1  )
    {
      printF("Integrate: ...done solver. cpu=%8.2e(s)\n",getCPU()-cpu0);
    }

    if( debug & 2  )
      weights.display("Here are the weights after solve","%8.1e");

    if( debug & 2  )
    {
      printF("Integrate:After solve: maximum residual=%e \n",solver->getMaximumResidual());
    }
    
  
  }

  // zero out the weights at unused points. (is this really needed?)
  // weights are non-zero at interpolation points -- the RHS should be zero at these
  // points anyway.
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    const IntegerDistributedArray & classify = coeff[grid].sparse->classify;
    realArray & weightsg = weights[grid];

    #ifdef USE_PPP
      realSerialArray weightsLocal;  getLocalArrayWithGhostBoundaries(weightsg,weightsLocal);
      intSerialArray classifyLocal;  getLocalArrayWithGhostBoundaries(classify,classifyLocal);
    #else
      const realSerialArray & weightsLocal   = weightsg;
      const intSerialArray & classifyLocal   = classify; 
    #endif

      real *weightsp = weightsLocal.Array_Descriptor.Array_View_Pointer2;
      const int weightsDim0=weightsLocal.getRawDataSize(0);
      const int weightsDim1=weightsLocal.getRawDataSize(1);
#undef WEIGHTS
#define WEIGHTS(i0,i1,i2) weightsp[i0+weightsDim0*(i1+weightsDim1*(i2))]
      int *classifyp = classifyLocal.Array_Descriptor.Array_View_Pointer2;
      const int classifyDim0=classifyLocal.getRawDataSize(0);
      const int classifyDim1=classifyLocal.getRawDataSize(1);
#undef CLASSIFY
#define CLASSIFY(i0,i1,i2) classifyp[i0+classifyDim0*(i1+classifyDim1*(i2))]


    getIndex(c.dimension(),I1,I2,I3);
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(weightsg,weightsLocal,I1,I2,I3,includeGhost); 
    if( !ok ) continue;
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      if( CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::interior && 
          CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::boundary &&
	  CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::ghost1 )
      {
	WEIGHTS(i1,i2,i3)=0.;
      }
    }
    
    if( !solveSingularProblem )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( CLASSIFY(i1,i2,i3)==SparseRepForMGF::ghost1 )
	{
	  WEIGHTS(i1,i2,i3)=-WEIGHTS(i1,i2,i3);
	}
      }
      
    }
  }
  if( debug & 2  )
    weights.display("Here are the scaled weights","%8.1e");
  

  if( deleteSolverAfterUse )
  { 
    // Note: there is currently a problem in parallel if we delete this solver
    // and PETSc is closed down, but then if PETSc is started up there is trouble in PETSc
    delete solver;  // remove solver to save space
    solver=NULL;
  }
  
  // go back and define exact weights on boundaries where we know them
  faceWeightsDefined=false;
  computeSurfaceWeights();

  weightsComputed=true;
  surfaceWeightsDefined=true;
  faceWeightsDefined=true;
  allFaceWeightsDefined=true;
  
  return 0;
}

int Integrate::
computeSurfaceWeights(int surfaceNumber /* =-1 */ )
// ================================================================================================
/// \brief
///    Compute the integration weights for a given surfaceNumber. This routine attempts to
///  compute the surface weights in a more efficient manner than the general case.
///
/// \param surfaceNumber (input) : compute weights for this surfaceNumber. By default do all surfaces.
/// 
/// \author WDH
// ================================================================================================
{

  if ( useSurfaceStitching && cg.numberOfDimensions()==3 )
  {
    computeStitchedSurfaceWeights(surfaceNumber);
    return 0;
  }
  if( debug & 1 )
  {
    if( surfaceNumber<0 )
      printF(" **** computeSurfaceWeights for all surfaces ****\n");
    else
      printF(" **** computeSurfaceWeights for surface = %i****\n",surfaceNumber);
  }
  
  if( !weightsUpdatedToMatchGrid )
  {
    weightsUpdatedToMatchGrid=true;
    weights.updateToMatchGrid(cg);
    weights=0.;
  }
  
  int returnValue=0;

  const int numberOfDimensions = cg.numberOfDimensions();
  int side,axis,grid;

  Index I1,I2,I3;
  Index Ig1,Ig2,Ig3;
    
  const bool checkAllFaces = surfaceNumber==-1;

  int surface=0;
  if( !checkAllFaces )
  {
    surface=bd.surfaceIndex(surfaceNumber);
    if( surface<0 || surface>= bd.numberOfSurfaces )
    {
      printf("Integrate::computeSurfaceWeights: Unknown surfaceNumber=%i \n",surfaceNumber);
      Overture::abort("error");
    }
  }
  const int numberOfFacesToCheck=checkAllFaces ? 2*numberOfDimensions*cg.numberOfBaseGrids() :
    bd.numberOfFaces(surface);
  
  int i;
  for( i=0; i<numberOfFacesToCheck; i++ ) // loop over all faces that make up this surface
  {
    side = !checkAllFaces ? bd.boundaryFaces(0,i,surface) : i % 2;
    axis = !checkAllFaces ? bd.boundaryFaces(1,i,surface) : (i/2) % numberOfDimensions;
    grid = !checkAllFaces ? bd.boundaryFaces(2,i,surface) : i/(2*numberOfDimensions);
    assert( side>=0 && side<=1 && axis>=0 && axis<numberOfDimensions && grid>=0 && grid<cg.numberOfComponentGrids() );

    if( cg[grid].boundaryCondition(side,axis)<=0 )
      continue;
    
    if( !faceWeightsDefined(side,axis,grid) )
    {
      // weights have not yet been defined for this face
      // Try to determine the surface weights directly

      MappedGrid & c = cg[grid];
      c.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseVertexDerivative | MappedGrid::THEmask);  

      const realArray & jacobian = c.centerJacobian();
      const realArray & rx = c.inverseVertexDerivative();
      const RealArray & gridSpacing = c.gridSpacing();
      IntegerDistributedArray & mask = c.mask();

      realArray & weightsg = weights[grid];

      #ifdef USE_PPP
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
        realSerialArray jacobianLocal; getLocalArrayWithGhostBoundaries(jacobian,jacobianLocal);
        realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
        realSerialArray weightsLocal; getLocalArrayWithGhostBoundaries(weightsg,weightsLocal);
      #else
        const intSerialArray & maskLocal = mask;
        const realSerialArray & jacobianLocal = jacobian;
        const realSerialArray & rxLocal = rx;
        realSerialArray & weightsLocal = weightsg;
      #endif


      real dr = c.gridSpacing(0)*c.gridSpacing(1)*c.gridSpacing(2);  // volume element in r
      
      int axisp1= (axis+1) % numberOfDimensions;
      int axisp2= numberOfDimensions==2 ? axisp1 : (axis+2) % numberOfDimensions;
      if( c.boundaryCondition(side,axis)>0 && 
          c.boundaryCondition(Start,axisp1)!=0 && c.boundaryCondition(End,axisp1)!=0 &&
          c.boundaryCondition(Start,axisp2)!=0 && c.boundaryCondition(End,axisp2)!=0 )
      {
	// for now only specify values on boundaries that have no interpolation points.

	getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);
        int includeGhost=1; 
        bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);

	int count=0;
        if( ok ) 
          count=sum(maskLocal(I1,I2,I3)<0);
        count=ParallelUtility::getSum(count);

	if( count==0 )
	{
          if( debug & 1 )
	  {
	    printF("Integrate:surfaceWeights: we can specify exact weights for (side,axis)=(%i,%i) of grid %s \n",
		   side,axis,(const char*)c.getName());
	    printF("Integrate:surfaceWeights: There are NO interpolation points on this side.\n");
	  }
	  
	  boundaryHasOverlap(side,axis,grid)=false;
          faceWeightsDefined(side,axis,grid)=true;
	    
	  getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
	  getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	  ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
	  ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ig1,Ig2,Ig3,includeGhost);

	  if( ok )
	  {
	    if( numberOfDimensions==2 )
	    {
	      weightsLocal(Ig1,Ig2,Ig3)=( SQRT(SQR(RXLocal(axis,axis1))+SQR(RXLocal(axis,axis2)))*
					   fabs(jacobianLocal(I1,I2,I3))*gridSpacing(axisp1) );
	      // weight end points by .5
	      weightsLocal(Ig1.getBase(),Ig2.getBase(),Ig3.getBase())*=.5;
	      weightsLocal(Ig1.getBound(),Ig2.getBound(),Ig3.getBound())*=.5;
	    }
	    else
	    {

	      weightsLocal(Ig1,Ig2,Ig3)=( SQRT(SQR(RXLocal(axis,axis1))+
						SQR(RXLocal(axis,axis2))+
						SQR(RXLocal(axis,axis3)))*
					   fabs(jacobianLocal(I1,I2,I3))*gridSpacing(axisp1)*gridSpacing(axisp2) );
	      // weight ends by .5 (corners by .25) -- this is ok if periodic provided we integrate
	      // the periodic image as well.
	      if( axis!=axis1 )
	      {
		weightsLocal(Ig1.getBase() ,Ig2,Ig3)*=.5;
		weightsLocal(Ig1.getBound(),Ig2,Ig3)*=.5;
	      }
	      if( axis!=axis2 )
	      {
		weightsLocal(Ig1,Ig2.getBase() ,Ig3)*=.5;
		weightsLocal(Ig1,Ig2.getBound(),Ig3)*=.5;
	      }
	      if( axis!=axis3 )
	      {
		weightsLocal(Ig1,Ig2,Ig3.getBase() )*=.5;
		weightsLocal(Ig1,Ig2,Ig3.getBound())*=.5;
	      }
	    
	    }
	  }

	}
      }
      else
      {
	boundaryHasOverlap(side,axis,grid)=true;
      }
    }
    if( !faceWeightsDefined(side,axis,grid) )
    {
      // unable to determine weights for this surface -- we need to use the fall-back approach.
      returnValue=1;
    }
  } // end for i

  // mark surfaces that can now be integrated.
  if( !checkAllFaces )
    surfaceWeightsDefined(surface) = returnValue==0;
  else
  {
    for( int s=0; s<bd.numberOfSurfaces; s++ )
    {
      surfaceWeightsDefined(s)=true;
      for( i=0; i<bd.numberOfFaces(s); i++ ) 
      {
	side = bd.boundaryFaces(0,i,surface);
	axis = bd.boundaryFaces(1,i,surface);
	grid = bd.boundaryFaces(2,i,surface);
        if( !faceWeightsDefined(side,axis,grid) )
	{
	  surfaceWeightsDefined(s)=false;
	  break;
	}
      }
    }
  }

  // check to see if the weights for all faces have been defined.
  allFaceWeightsDefined=true;
  for( grid=0; grid<cg.numberOfBaseGrids() && allFaceWeightsDefined; grid++ )
  {
    for( axis=0; axis<numberOfDimensions && allFaceWeightsDefined; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( !faceWeightsDefined(side,axis,grid) && cg[grid].boundaryCondition(side,axis)>0 )
	{
	  allFaceWeightsDefined=false;
	  break;
	}
      }
    }
  }
  if( debug & 1 ) 
  {
    printF("---computeSurfaceWeights:  allFaceWeightsDefined=%i\n",allFaceWeightsDefined);
    if( debugFile!=NULL )
      fprintf(debugFile,"---computeSurfaceWeights:  allFaceWeightsDefined=%i\n",allFaceWeightsDefined);
  }
  
//   if( false )
//     weights.display("Here are the scaled weights","%6.2 e");
  

  return returnValue;
}

void Integrate::
setRadialAxis( int axis )
// ================================================================================================
/// \brief
///    Compute the left null vector to the Neumann problem.
/// 
/// \author KKC
// ================================================================================================
{
  if ( axis<0 || axis>1 )
  {
    cout<<"ERROR : Integrate::setRadialAxis : radial axis must be either 0 or 1, input was : "<<axis<<endl;
    return;
  }
  radialAxis = axis;
}


int Integrate::
computeLeftNullVector()
// ================================================================================================
/// \brief
///    Compute the left null vector to the Neumann problem.
/// 
/// \author WDH
// ================================================================================================
{
  const int numberOfDimensions = cg.numberOfDimensions();
  int grid;
  int dwMin=3, dwMax=3;
  Range Rx(0,cg.numberOfDimensions()-1);
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    dwMin=min(cg[grid].discretizationWidth()(Rx));
    dwMax=max(cg[grid].discretizationWidth()(Rx));
  }
  if( dwMin==5 && dwMax==5 )
    orderOfAccuracy=4;
  else 
    orderOfAccuracy=2;
  
  printf("\n\nIntegrate::computeLeftNullVector: orderOfAccuracy= %i\n",orderOfAccuracy);

  // make a grid function to hold the coefficients
  Range all;
  const int width = orderOfAccuracy==2 ? 3 : 5;
  
  const int stencilSize=int( pow(width,numberOfDimensions)+1 );  // add 1 for interpolation equations
  weights.updateToMatchGrid(cg);
  
  RealCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  const int numberOfGhostLines= orderOfAccuracy==2 ? 1 : 2;
  coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
    
  realCompositeGridFunction f(cg);

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  op.setOrderOfAccuracy(orderOfAccuracy);
  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::allBoundaries);
  if( orderOfAccuracy==4 )
  {
    BoundaryConditionParameters extrapParams;
    extrapParams.ghostLineToAssign=2;
    extrapParams.orderOfExtrapolation=4;
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries,extrapParams);
  }
  coeff.finishBoundaryConditions();

  if( solver==0 )
    solver = new Oges(cg);
  else
    solver->updateToMatchGrid(cg);
  
  // count the total number of grid points.
  int numberOfGridPoints=0;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    numberOfGridPoints+=cg[grid].mask().elementCount();

  Index I1,I2,I3;
  bool useIterativeSolver= true || numberOfDimensions==3;
  if( useIterativeSolver )
  {
    solver->set(OgesParameters::THEbestIterativeSolver);
    // solver->set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
    // solver->setConjugateGradientPreconditioner(Oges::diagonal);
    // Oges::debug=7;
    solver->set(OgesParameters::THEtolerance,tolerance*numberOfGridPoints);

    // *wdh* 2012/12/23 : use more ILU levels and allow more iterations
    const int maxit=1000;  // this should depend on the number of points on the coarse grid!
    solver->set(OgesParameters::THEmaximumNumberOfIterations,maxit);
    solver->set(OgesParameters::THEnumberOfIncompleteLULevels,5);
  }    

  solver->set(OgesParameters::THEsolveForTranspose,true); // solve the transpose system (we want the left null vector)
  solver->set(OgesParameters::THEfixupRightHandSide,false);     // no need to zero out equations at special points.

  solver->set(OgesParameters::THErescaleRowNorms,false);  // *wdh* 2014/12/22 do NOT rescale rows! fix for parallel

  bool solveSingularProblem=true;


  // assign the rhs: f=0 except for the rhs to the compatibility equation which we set to the numberOfGridPoints.
  // (this will cause the sum of the interior weights to be numberOfGridPoints so that each value should be about 1)
  f=0.;
  solver->setCoefficientArray( coeff );     // supply coefficients
  if( solveSingularProblem )
    solver->set(OgesParameters::THEcompatibilityConstraint,true); // system is singular so add an extra equation
  solver->initialize();

  // find the equation where the compatibility constraint is put (some unused point)
  int n,i1e,i2e,i3e,gride;
  solver->equationToIndex(solver->extraEquationNumber(0),n,i1e,i2e,i3e,gride);
  f[gride](i1e,i2e,i3e,n)=numberOfGridPoints;
     
#define RX(m,n) rx(I1,I2,I3,m+numberOfDimensions*(n))

  nullVector.updateToMatchGrid(cg);
  if( useIterativeSolver )
    nullVector=1.;
  
  solver->solve( nullVector,f );   // solve for the (unscaled weights)

  // zero out the nullVector at unused points. 
  // nullVector values are non-zero at interpoaltion points -- the RHS should be zero at these
  // points anyway.
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    const IntegerDistributedArray & classify = coeff[grid].sparse->classify;

    getIndex(c.dimension(),I1,I2,I3);
    where( classify(I1,I2,I3)!=(int)SparseRepForMGF::interior && classify(I1,I2,I3)!=(int)SparseRepForMGF::boundary &&
           classify(I1,I2,I3)!=(int)SparseRepForMGF::ghost1 )
    {
      nullVector[grid](I1,I2,I3)=0.;
    }
  }
  if( false )
    nullVector.display("Here is the leftNullVector","%5.1e ");

  delete solver;  // remove solver to save space
  solver=0;
  
  leftNullVectorComputed=true;

  return 0;
}

// extern int stitchCompositeGrid(CompositeGrid &);

static inline int KC_IDX(int i1,int i2, int i3) { return i1 + 2*(i2 + 2*i3); }

static inline void addWeight(int i0, int i1, int i2, Index &Ib1, Index &Ib2, Index &Ib3,
			     Index &Ig1, Index &Ig2, Index &Ig3, real w, realArray &weightsg)
{

  if ( i0==Ib1.getBase() )
    weightsg(Ig1.getBase(), max(min(i1,Ib2.getBound()),Ib2.getBase()), max(min(i2,Ib3.getBound()),Ib3.getBase())) += w;
  else if ( i0==Ib1.getBound() )
    weightsg(Ig1.getBound(),max(min(i1,Ib2.getBound()),Ib2.getBase()), max(min(i2,Ib3.getBound()),Ib3.getBase())) += w;
  else if ( i1==Ib2.getBase() )
    weightsg( max(min(i0,Ib1.getBound()),Ib1.getBase()), Ig2.getBase(), max(min(i2,Ib3.getBound()),Ib3.getBase()) ) += w;
  else if ( i1==Ib2.getBound() )
    weightsg( max(min(i0,Ib1.getBound()),Ib1.getBase()), Ig2.getBound(), max(min(i2,Ib3.getBound()),Ib3.getBase()) ) += w;
  else if ( i2==Ib3.getBase() )
    weightsg( max(min(i0,Ib1.getBound()),Ib1.getBase()), max(min(i1,Ib2.getBound()),Ib2.getBase()), Ig3.getBase() ) += w;
  else if ( i2==Ib3.getBound() )
    weightsg( max(min(i0,Ib1.getBound()),Ib1.getBase()), max(min(i1,Ib2.getBound()),Ib2.getBase()), Ig3.getBound() ) += w;
  //  else
  //    return;
  //    cout<<"off boundary vertex with weight "<<w<<endl;
  
  //cout<<"w was "<<w<<endl;
}


int Integrate::
computeStitchedSurfaceWeightsOld()
// ================================================================================================
/// \brief
///    This is a protected routine that computes surface weights by building a hybrid grid
///  on the surface using the advacing front algorithm.
/// 
/// \author Kyle Chand.
// ================================================================================================int 
{
  printf(" ***Integrate: computeStitchedSurfaceWeights ****\n");

  if( !weightsUpdatedToMatchGrid )
  {
    weightsUpdatedToMatchGrid=true;
    weights.updateToMatchGrid(cg);
  }

  if ( cg.numberOfDimensions()!=3 )
  {
    cout<<"Surface stitching only works in 3D, grid was "<<cg.numberOfDimensions()<<"D"<<endl;
    return 1;
  }

  if ( !cg.getSurfaceStitching() )
  {
    // stitchCompositeGrid(cg);

    // new way: 
    SurfaceStitcher stitcher;

    stitcher.defineSurfaces(cg);  // choose all boundary surfaces

    real overlapWidth=0.;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      const IntegerArray & dw = mg.discretizationWidth();
      for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
      {
	MappedGrid & mg2 = cg[grid];
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  int width, l=0;
	  if( cg.interpolationIsImplicit(grid,grid2,l) || cg.refinementLevelNumber(grid)>0 )
	    width = max(cg.interpolationWidth(axis,grid,grid2,l),mg2.discretizationWidth(axis)) - 2;
	  else
	    width=cg.interpolationWidth(axis,grid,grid2,l) + mg2.discretizationWidth(axis) - 3;

	  overlapWidth =  max(overlapWidth, .5*width );
	}
      }
    }

    // By default increase the gap between overlapping surface grids by this many extra grid lines:
    int gapWidth=int(overlapWidth/2. + 2.5);  

    stitcher.enlargeGap(gapWidth);

    bool interactiveStitcher=true;
    stitcher.stitchSurfaceCompositeGrid(interactiveStitcher);

    cg.setSurfaceStitching( stitcher.getUnstructuredGrid() );

  }

  // first compute the weights for the structured parts of the surfaces
  //
  //   The stitching algorithm ignores all interpolation and hole points in the grids.
  //   Only surface cells that are composed entirely of discretization points add to the
  //      integral contributions from the structured patches.  The rest of the integral comes
  //      from the surface stitching. NOTE: cells floating in emptiness (a hole) are also ignored!
  //      This information is kept track of by by building a temporary mask array.

  //   much of the loop code is from Ugen::computeZoneMasks and buildSurfaceCG

  // right now we check and compute weights for all the faces that have bc>0
  int grid;
  int maxShare = 0;
  int side,axis;
  
  int currshare, currbc, gid;
  
  gid=0;
  
  int offset[3];
  offset[0]=offset[1]=offset[2]=0;
  // loop through all the grids and add any physical boundaries to surf_cg
  for ( grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      
      real sj = 1;
      mg.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseVertexDerivative);  
      
      const realArray & jacobian = mg.centerJacobian();
      const realArray & rx = mg.inverseVertexDerivative();
      const RealArray & gridSpacing = mg.gridSpacing();
      IntegerDistributedArray & mask = mg.mask();

      realArray & weightsg = weights[grid];

      real dr = mg.gridSpacing(0)*mg.gridSpacing(1)*mg.gridSpacing(2);  // volume element in r
      
      // loop through each side of each axis of mg looking for surfaces to add
      for ( int a1=0; a1<mg.domainDimension(); a1++ )
	for ( int s1=0; s1<2; s1++ )
	  {
	    offset[0]=offset[1]=offset[2]=1;
	    offset[a1]=0;

	    if ( mg.boundaryCondition(s1,a1)>0 && //==MappedGrid::physicalBoundary ||
		  //		  mg.boundaryFlag(s1,a1)==MappedGrid::mixedPhysicalInterpolationBoundary) &&
		 // -20 is magic meaning don't add this physical surface to the surface grid
		 mg.sharedBoundaryFlag(s1,a1)!=-20) 
	      {
		//		cout<<"adding weights for grid "<<grid<<" side,axis "<<s1<<", "<<a1<<endl;
		int extra=1; 
		Index Ib1,Ib2,Ib3,Ibb1,Ibb2,Ibb3;
		//		getBoundaryIndex(mg.gridIndexRange(),s1,a1,Ib1,Ib2,Ib3,1,extra);
		getBoundaryIndex(mg.gridIndexRange(),s1,a1,Ib1,Ib2,Ib3);//,1,extra);
		getBoundaryIndex(mg.gridIndexRange(),s1,a1,Ibb1,Ibb2,Ibb3,1,extra);

		IntegerArray tmpMask(Ibb1,Ibb2,Ibb3),zoneMask(Ib1,Ib2,Ib3);
		tmpMask = 0;
		zoneMask = 1;
		// mask all the non-discretization points (interpolation + hole)
		where ( mg.mask()(Ib1,Ib2,Ib3)>0 )
		  {
		    tmpMask(Ib1,Ib2,Ib3) = 1;
		  }

// 		for ( int i1=Ib1.getBase(); i1<=Ib1.getBound()-offset[0]; i1++ )
// 		  for ( int i2=Ib2.getBase(); i2<=Ib2.getBound()-offset[1]; i2++ )
// 		    for ( int i3=Ib3.getBase(); i3<=Ib3.getBound()-offset[2]; i3++ )
		for ( int i1=Ib1.getBase(); i1<=Ib1.getBound(); i1++ )
		  for ( int i2=Ib2.getBase(); i2<=Ib2.getBound(); i2++ )
		    for ( int i3=Ib3.getBase(); i3<=Ib3.getBound(); i3++ )
		      {
			bool maskIt = false;
			if ( a1==axis1 )
			  {
			    maskIt= tmpMask(i1,i2,i3)==0 ||
			      tmpMask(i1,i2+1,i3)==0 ||
			      tmpMask(i1,i2+1,i3+1)==0 ||
			      tmpMask(i1,i2,i3+1)==0;
			  }
			else if ( a1==axis2 )
			  {		
			    maskIt= tmpMask(i1,i2,i3)==0 ||
			      tmpMask(i1+1,i2,i3)==0 ||
			      tmpMask(i1+1,i2,i3+1)==0 ||
			      tmpMask(i1,i2,i3+1)==0;
			    
			  }
			else
			  {
			    maskIt= tmpMask(i1,i2,i3)==0 ||
			      tmpMask(i1+1,i2+1,i3)==0 ||
			      tmpMask(i1,i2+1,i3)==0 ||
			      tmpMask(i1+1,i2,i3)==0;
			  }

			if ( maskIt ) 
			  zoneMask(i1,i2,i3) = 0;
		      }

		// now, finally, mask out any vertices that are part of floating cells and compute the weights

		//				if ( grid ) tmpMask.display("tmpMask");
		//				if ( grid ) zoneMask.display("zone mask");

		Index Ig1,Ig2,Ig3;
		getGhostIndex(mg.gridIndexRange(),s1,a1,Ig1,Ig2,Ig3,1,1);
		weightsg = 0.;


#define KRX(m,n) m+3*(n)
// 		for ( int i1=Ib1.getBase()+offset[0]; i1<=Ib1.getBound()-offset[0]; i1++ )
// 		  for ( int i2=Ib2.getBase()+offset[1]; i2<=Ib2.getBound()-offset[1]; i2++ )
// 		    for ( int i3=Ib3.getBase()+offset[2]; i3<=Ib3.getBound()-offset[2]; i3++ )
		for ( int i1=Ib1.getBase(); i1<=Ib1.getBound()-offset[0]; i1++ )
		  for ( int i2=Ib2.getBase(); i2<=Ib2.getBound()-offset[1]; i2++ )
		    for ( int i3=Ib3.getBase(); i3<=Ib3.getBound()-offset[2]; i3++ )
		      {
			if ( zoneMask(i1,i2,i3) )
			  {
			    bool maskit = false;
			    if ( a1==axis1 )
			      {
				maskit = zoneMask(i1,i2+1,i3)==0 &&
				  zoneMask(i1,i2,i3+1)==0 &&
				  zoneMask(i1,i2-1,i3)==0 &&
				  zoneMask(i1,i2,i3-1)==0 ;
				if ( maskit )
				  tmpMask(i1,i2,i3) = tmpMask(i1,i2+1,i3) =
				    tmpMask(i1,i2,i3+1) = tmpMask(i1,i2+1,i3+1) = 0;
				else
				  {
				    weightsg(Ig1.getBase(),i2,i3) += 
				      .25*SQRT( SQR(rx(i1,i2,i3,KRX(a1,axis1))) + SQR(rx(i1,i2,i3,KRX(a1,axis2))) + SQR(rx(i1,i2,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2,i3))*gridSpacing(axis2)*gridSpacing(axis3);

				    weightsg(Ig1.getBase(),i2+1,i3) += 
				      .25*SQRT( SQR(rx(i1,i2+1,i3,KRX(a1,axis1))) + SQR(rx(i1,i2+1,i3,KRX(a1,axis2))) + SQR(rx(i1,i2+1,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2+1,i3))*gridSpacing(axis2)*gridSpacing(axis3);

				    weightsg(Ig1.getBase(),i2,i3+1) += 
				      .25*SQRT( SQR(rx(i1,i2,i3+1,KRX(a1,axis1))) + SQR(rx(i1,i2,i3+1,KRX(a1,axis2))) + SQR(rx(i1,i2,i3+1,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2,i3+1))*gridSpacing(axis2)*gridSpacing(axis3);

				    weightsg(Ig1.getBase(),i2+1,i3+1) += 
				      .25*SQRT( SQR(rx(i1,i2+1,i3+1,KRX(a1,axis1))) + SQR(rx(i1,i2+1,i3+1,KRX(a1,axis2))) + SQR(rx(i1,i2+1,i3+1,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2+1,i3+1))*gridSpacing(axis2)*gridSpacing(axis3);
				  }
			      }
			    else if ( a1==axis2 )
			      {
				maskit = zoneMask(i1+1,i2,i3)==0 &&
				  zoneMask(i1,i2,i3+1)==0 &&
				  zoneMask(i1-1,i2,i3)==0 &&
				  zoneMask(i1,i2,i3-1)==0 ;

				if ( maskit )
				  tmpMask(i1,i2,i3) = tmpMask(i1+1,i2,i3) =
				    tmpMask(i1,i2,i3+1) = tmpMask(i1+1,i2,i3+1) = 0;
				else
				  {
				    weightsg(i1,Ig2.getBase(),i3) += 
				      .25*SQRT( SQR(rx(i1,i2,i3,KRX(a1,axis1))) + SQR(rx(i1,i2,i3,KRX(a1,axis2))) + SQR(rx(i1,i2,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2,i3))*gridSpacing(axis1)*gridSpacing(axis3);

				    weightsg(i1+1,Ig2.getBase(),i3) += 
				      .25*SQRT( SQR(rx(i1+1,i2,i3,KRX(a1,axis1))) + SQR(rx(i1+1,i2,i3,KRX(a1,axis2))) + SQR(rx(i1+1,i2,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1+1,i2,i3))*gridSpacing(axis1)*gridSpacing(axis3);

				    weightsg(i1,Ig2.getBase(),i3+1) += 
				      .25*SQRT( SQR(rx(i1,i2,i3+1,KRX(a1,axis1))) + SQR(rx(i1,i2,i3+1,KRX(a1,axis2))) + SQR(rx(i1,i2,i3+1,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2,i3+1))*gridSpacing(axis1)*gridSpacing(axis3);

				    weightsg(i1+1,Ig2.getBase(),i3+1) += 
				      .25*SQRT( SQR(rx(i1+1,i2,i3+1,KRX(a1,axis1))) + SQR(rx(i1+1,i2,i3+1,KRX(a1,axis2))) + SQR(rx(i1+1,i2,i3+1,KRX(a1,axis3))) )*
				      fabs(jacobian(i1+1,i2,i3+1))*gridSpacing(axis1)*gridSpacing(axis3);

				  }
			      }
			    else 
			      {
				maskit = zoneMask(i1+1,i2,i3)==0 &&
				  zoneMask(i1,i2+1,i3)==0 &&
				  zoneMask(i1-1,i2,i3)==0 &&
				  zoneMask(i1,i2-1,i3)==0;
				if ( maskit )
				  tmpMask(i1,i2,i3) = tmpMask(i1+1,i2,i3) =
				    tmpMask(i1,i2+1,i3) = tmpMask(i1+1,i2+1,i3) = 0;
				else
				  {
				    weightsg(i1,i2,Ig3.getBase()) += 
				      .25*SQRT( SQR(rx(i1,i2,i3,KRX(a1,axis1))) + SQR(rx(i1,i2,i3,KRX(a1,axis2))) + SQR(rx(i1,i2,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2,i3))*gridSpacing(axis1)*gridSpacing(axis2);

				    weightsg(i1+1,i2,Ig3.getBase()) += 
				      .25*SQRT( SQR(rx(i1+1,i2,i3,KRX(a1,axis1))) + SQR(rx(i1+1,i2,i3,KRX(a1,axis2))) + SQR(rx(i1+1,i2,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1+1,i2,i3))*gridSpacing(axis1)*gridSpacing(axis2);

				    weightsg(i1,i2+1,Ig3.getBase()) += 
				      .25*SQRT( SQR(rx(i1,i2+1,i3,KRX(a1,axis1))) + SQR(rx(i1,i2+1,i3,KRX(a1,axis2))) + SQR(rx(i1,i2+1,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1,i2+1,i3))*gridSpacing(axis1)*gridSpacing(axis2);

				    weightsg(i1+1,i2+1,Ig3.getBase()) += 
				      .25*SQRT( SQR(rx(i1+1,i2+1,i3,KRX(a1,axis1))) + SQR(rx(i1+1,i2+1,i3,KRX(a1,axis2))) + SQR(rx(i1+1,i2+1,i3,KRX(a1,axis3))) )*
				      fabs(jacobian(i1+1,i2+1,i3))*gridSpacing(axis1)*gridSpacing(axis2);
				  }
			      }

			  }
		      }

		//		  weightsg(Ig1,Ig2,Ig3).display("weightsg");

	      }
	  }
    }
  
  // now compute the weight contributions from the stitching

  if ( !cg.getSurfaceStitching() )
    {
      allFaceWeightsDefined=true;
      faceWeightsDefined = true;
      surfaceWeightsDefined = true;
      
      return 0;
    }

  UnstructuredMapping &umap = *cg.getSurfaceStitching();

  realArray & verts = (realArray &)umap.getNodes();
  intArray & tris = (intArray &) umap.getEntities(UnstructuredMapping::Face);

  realArray tCenters(umap.size(UnstructuredMapping::Face), cg.numberOfDimensions()), 
            tAreas(umap.size(UnstructuredMapping::Face));
  
  ArraySimpleFixed<real,3,1,1,1> v0,v1,v2;

  for ( int e=0; e<umap.size(UnstructuredMapping::Face); e++ )
    {
      for ( int a=0; a<3; a++ )
	{
	  v0[a] = verts(tris(e,0),a);
	  v1[a] = verts(tris(e,1),a);
	  v2[a] = verts(tris(e,2),a);
	  tCenters(e,a) = (v0[a] + v1[a] + v2[a])/3;
	}

      tAreas(e) = sqrt(ASmag2(areaNormal3D(v0,v1,v2)));
    }

  InterpolatePoints interpolate;
  realArray interpCoeff(umap.size(UnstructuredMapping::Face),8);
  interpCoeff = 0;

  #ifndef USE_PPP
    interpolate.buildInterpolationInfo(tCenters, cg);
    interpolate.interpolationCoefficients(cg,interpCoeff);
  #else
    Overture::abort("finish me for parallel");
  #endif

  IntegerArray indexValues,interpoleeGrid;
  interpolate.getInterpolationInfo(cg,indexValues,interpoleeGrid);

  real areasums = 0;

  //  interpCoeff.display("interpCoeff");

  for ( int e=0; e<umap.size(UnstructuredMapping::Face); e++ )
    {
      // for each point in tCenters, examine the vertices that are used to interpolate 
      //    the face centered value of the function.  Any vertices that are on the boundaries
      //    and are used for interpolation get thier weights added to the weights sitting in
      //    the adjacent ghost boundary index. NOTE: right now, if the off-boundary vertices are
      //    used (which they will be in general) they are IGNORED.  If the point is near the surface
      //    then the coefficients for the off boundary vertices *should* be small.  We do this
      //    because we only store surface integral weights on the ghost boundary; in other words,
      //    we have no other location to put the additional weight info ( ANOTHER NOTE : we could
      //    have 2 ghost lines and store all 8 weights...)

      int grid = interpoleeGrid(e);
      int i0 = indexValues(e,0);
      int i1 = indexValues(e,1);
      int i2 = indexValues(e,2);

      //cout<<"interpolating face "<<e<<" from grid "<<grid<<" at "<<i0<<"  "<<i1<<"  "<<i2<<endl;

      MappedGrid & mg = cg[grid];

      Index Ig1,Ig2,Ig3;
      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1,1);

      Index Ib1,Ib2,Ib3;
      getIndex(mg.gridIndexRange(),Ib1,Ib2,Ib3);

      // check all 8 vertices that are part of cell i0,i1,i2 in mg. add weight*tArea to the ghost boundary weight

      realArray & weightsg = weights[grid];

      real area = tAreas(e);

      //      cout<<"area is "<<area<<endl;

      real wsum=0;

      real w = area*interpCoeff(e,KC_IDX(0,0,0));
      addWeight(i0,i1,i2,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      w = area*interpCoeff(e,KC_IDX(1,0,0));
      addWeight(i0+1,i1,i2,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      w = area*interpCoeff(e,KC_IDX(0,1,0));
      addWeight(i0,i1+1,i2,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      w = area*interpCoeff(e,KC_IDX(1,1,0));
      addWeight(i0+1,i1+1,i2,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      w = area*interpCoeff(e,KC_IDX(0,0,1));
      addWeight(i0,i1,i2+1,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      w = area*interpCoeff(e,KC_IDX(1,0,1));
      addWeight(i0+1,i1,i2+1,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      w = area*interpCoeff(e,KC_IDX(0,1,1));
      addWeight(i0,i1+1,i2+1,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      w = area*interpCoeff(e,KC_IDX(1,1,1));
      addWeight(i0+1,i1+1,i2+1,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3, w, weightsg);

      wsum+=w;

      //      cout<<"sum of the weights = "<<wsum/area<<endl;

      areasums += area;

    }

  //  cout<<"areasums was "<<areasums<<endl;
  allFaceWeightsDefined=true;
  faceWeightsDefined = true;
  surfaceWeightsDefined = true;

  return 0;
}

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)



int Integrate::
computeStitchedSurfaceWeights(int surfaceNumber /* =-1 */ )
// ================================================================================================
/// \brief
///     This is a protected routine that computes surface weights by building a hybrid grid
///   on the surface using the advacing front algorithm.
///  
/// \author 
///    Initial version: Kyle Chand. 
///    New version by wdh. 
// ================================================================================================int 
{
  printf(" ***Integrate: computeStitchedSurfaceWeights ****\n");

  if( !weightsUpdatedToMatchGrid )
  {
    weightsUpdatedToMatchGrid=true;
    weights.updateToMatchGrid(cg);
  }

  if ( cg.numberOfDimensions()!=3 )
  {
    cout<<"Surface stitching only works in 3D, grid was "<<cg.numberOfDimensions()<<"D"<<endl;
    return 1;
  }

  // We should save the stitcher...

  const bool useAllBoundaries = surfaceNumber==-1;
  
  BodyDefinition & bodyDefinition = !useAllBoundaries ? bd : *new BodyDefinition();

  if( useAllBoundaries )
  {
    // Create a BodyDefinition that holds all boundary faces:

    const int maxNumberOfFaces=cg.numberOfGrids()*6;
    IntegerArray boundary(3,maxNumberOfFaces);  
    
    int numberOfFaces=0;  // counts boundary faces
    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      // loop through each side of each axis of mg looking for surfaces to add
      for( int axis=0; axis<mg.domainDimension(); axis++ )
      {
	for( int side=0; side<2; side++ )
	{
	  if( (mg.boundaryFlag(side,axis)==MappedGrid::physicalBoundary ||
	       mg.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary) &&
               // -20 is magic meaning don't add this physical surface to the surface grid: 
	      mg.sharedBoundaryFlag(side,axis)!=-20) 
	  {	    
	    boundary(0,numberOfFaces)=side;
	    boundary(1,numberOfFaces)=axis;
	    boundary(2,numberOfFaces)=grid;
	    numberOfFaces++;
	  }
	}
      }
    }
    if( numberOfFaces>0 )
      bodyDefinition.defineSurface( 0,numberOfFaces,boundary ); 
  }

  const int numberOfSurfaces=bodyDefinition.totalNumberOfSurfaces();

  delete pSurfaceStitcher;
  pSurfaceStitcher = new SurfaceStitcher;
  
  SurfaceStitcher & stitcher = *pSurfaceStitcher;

  stitcher.defineSurfaces(cg,&bodyDefinition); 
  
  

  // compute overlap width so we know how how to enlarge the gap between overlapping surface grids
  real overlapWidth=0.;
  for( int surf=0; surf<numberOfSurfaces; surf++ )
  {
    const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(surf);
    for( int face=0; face<numberOfFaces; face++ )
    {
      int grid,side,axis;
      bodyDefinition.getFace(surf,face,side,axis,grid);

      MappedGrid & mg = cg[grid];
      const IntegerArray & dw = mg.discretizationWidth();
      for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
      {
	MappedGrid & mg2 = cg[grid];
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  int width, l=0;
	  if( cg.interpolationIsImplicit(grid,grid2,l) || cg.refinementLevelNumber(grid)>0 )
	    width = max(cg.interpolationWidth(axis,grid,grid2,l),mg2.discretizationWidth(axis)) - 2;
	  else
	    width=cg.interpolationWidth(axis,grid,grid2,l) + mg2.discretizationWidth(axis) - 3;

	  overlapWidth =  max(overlapWidth, .5*width );
	}
      }
    }
  }
  
  // By default increase the gap between overlapping surface grids by this many extra grid lines:
  int gapWidth=int(overlapWidth/2. + 2.5);  

  stitcher.enlargeGap(gapWidth);

  stitcher.stitchSurfaceCompositeGrid(interactiveStitcher);

  // Get the CompositeGrid that holds the surfaces
  assert( stitcher.getSurfaceCompositeGrid()!=NULL );
  CompositeGrid & cgSurf = *stitcher.getSurfaceCompositeGrid();

  // *** cg.setSurfaceStitching( stitcher.getUnstructuredGrid() );


  // first compute the weights for the structured parts of the surfaces
  //
  //   The stitching algorithm ignores all interpolation and hole points in the grids.
  //   Only surface cells that are composed entirely of discretization points add to the
  //      integral contributions from the structured patches.  The rest of the integral comes
  //      from the surface stitching. NOTE: cells floating in emptiness (a hole) are also ignored!
  //      This information is kept track of by by building a temporary mask array.

  //   much of the loop code is from Ugen::computeZoneMasks and buildSurfaceCG

  int i1,i2,i3;
  int grid, side,axis;
  
  int offset[3];

  int surfaceGrid=-1;
  for( int surf=0; surf<numberOfSurfaces; surf++ )
  {
    const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(surf);
    for( int face=0; face<numberOfFaces; face++ )
    {
      bodyDefinition.getFace(surf,face,side,axis,grid);

      MappedGrid & mg = cg[grid];          // volume grid

      surfaceGrid++;
      MappedGrid & mgSurf = cgSurf[surfaceGrid]; // surface grid 
      
      // we should be able to compute the jacobian and rx for the surface grid -- doesn't work yet

      mg.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseVertexDerivative);  
      
      const realArray & jacobian = mg.centerJacobian();
      const realArray & rx = mg.inverseVertexDerivative();
      const RealArray & gridSpacing = mg.gridSpacing();
      IntegerDistributedArray & mask = mg.mask();

      IntegerDistributedArray & maskSurf = mgSurf.mask();


      realArray & weightsg = weights[grid];

      offset[0]=offset[1]=offset[2]=1;
      offset[axis]=0;

      //		cout<<"adding weights for grid "<<grid<<" side,axis "<<side<<", "<<axis<<endl;
      int extra=1; 
      Index Ib1,Ib2,Ib3,Ibb1,Ibb2,Ibb3;

      Index J1,J2,J3;  // for the surface grid

      //		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,1,extra);
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);//,1,extra);
      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ibb1,Ibb2,Ibb3,1,extra);

      getIndex(mgSurf.gridIndexRange(),J1,J2,J3);
      if( false )
      {
	::displayMask(mask(Ib1,Ib2,Ib3),sPrintF("mask on volume grid face (grid,side,axis)=(%i,%i,%i)",
                                                 grid,side,axis));
	::displayMask(maskSurf(J1,J2,J3),sPrintF("mask on the surface grid (grid,side,axis)=(%i,%i,%i)",
                                                 grid,side,axis));
      }


      IntegerArray zoneMask(Ib1,Ib2,Ib3);
      zoneMask = 0;

      // mask all the non-discretization points (interpolation + hole)
      int j1,j2,j3;
      FOR_3IJD(i1,i2,i3,Ib1,Ib2,Ib3,j1,j2,j3,J1,J2,J3) 
      {
        // NOTE: Use the mask from surface grid -- 
        if( maskSurf(j1  ,j2  ,j3)>0 && maskSurf(j1+1,j2  ,j3)>0 &&   
            maskSurf(j1  ,j2+1,j3)>0 && maskSurf(j1+1,j2+1,j3)>0 )
	  zoneMask(i1,i2,i3)=1;
      }

//       FOR_3(i1,i2,i3,Ib1,Ib2,Ib3)
//       {
// 	if ( tmpMask(i1,i2  ,i3)==0 || tmpMask(i1+1,i2+1,i3)==0 ||
// 	     tmpMask(i1,i2+1,i3)==0 || tmpMask(i1+1,i2  ,i3)==0 ) 
// 	  zoneMask(i1,i2,i3) = 0;
//       }

      // now, finally, mask out any vertices that are part of floating cells and compute the weights
      if( false )
      {
	::displayMask(zoneMask,sPrintF("zoneMask (grid,side,axis)=(%i,%i,%i)",
				       grid,side,axis));
      }
      // tmpMask.display("tmpMask");
      // zoneMask.display("zone mask");

//       Index Ig1,Ig2,Ig3;
//       getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,1);

//       // first assign weights into this temp array:
//       RealSerialArray wt(Ibb1,Ibb2,Ibb3);  
//       wt = 0.;


#define KRX(m,n) m+3*(n)
#define AREA1(i1,i2,i3) ( SQRT( SQR(rx(i1,i2,i3,KRX(axis,axis1))) + \
                                SQR(rx(i1,i2,i3,KRX(axis,axis2))) + \
                                SQR(rx(i1,i2,i3,KRX(axis,axis3))) )* \
		          fabs(jacobian(i1,i2,i3))*gridSpacing(axis2)*gridSpacing(axis3) )

#define AREA2(i1,i2,i3) ( SQRT( SQR(rx(i1,i2,i3,KRX(axis,axis1))) + \
                                SQR(rx(i1,i2,i3,KRX(axis,axis2))) + \
                                SQR(rx(i1,i2,i3,KRX(axis,axis3))) )* \
		          fabs(jacobian(i1,i2,i3))*gridSpacing(axis1)*gridSpacing(axis3) )

#define AREA3(i1,i2,i3) ( SQRT( SQR(rx(i1,i2,i3,KRX(axis,axis1))) + \
                                SQR(rx(i1,i2,i3,KRX(axis,axis2))) + \
                                SQR(rx(i1,i2,i3,KRX(axis,axis3))) )* \
	                  fabs(jacobian(i1,i2,i3))*gridSpacing(axis1)*gridSpacing(axis2) )

      // now, finally, mask out any vertices that are part of floating cells and compute the weights

      //				if ( grid ) tmpMask.display("tmpMask");
      //				if ( grid ) zoneMask.display("zone mask");

      Index Ig1,Ig2,Ig3;
      getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,1);
      weightsg = 0.;

      const int i1g = Ig1.getBase();
      const int i2g = Ig2.getBase();
      const int i3g = Ig3.getBase();
      
#define KRX(m,n) m+3*(n)
      for ( int i1=Ib1.getBase(); i1<=Ib1.getBound()-offset[0]; i1++ )
	for ( int i2=Ib2.getBase(); i2<=Ib2.getBound()-offset[1]; i2++ )
	  for ( int i3=Ib3.getBase(); i3<=Ib3.getBound()-offset[2]; i3++ )
	  {
	    if ( zoneMask(i1,i2,i3) )
	    {
	      bool maskit = false;
	      if ( axis==axis1 )
	      {
                // Look for an isolated zone -- 4 adjacent zones are empty
		maskit = (zoneMask(i1,i2+1,i3)==0 &&  zoneMask(i1,i2,i3+1)==0 &&
		          zoneMask(i1,i2-1,i3)==0 &&  zoneMask(i1,i2,i3-1)==0 );
		if ( !maskit )
		{
		  weightsg(i1g,i2  ,i3  ) += .25*AREA1(i1,i2  ,i3  );
		  weightsg(i1g,i2+1,i3  ) += .25*AREA1(i1,i2+1,i3  );
		  weightsg(i1g,i2  ,i3+1) += .25*AREA1(i1,i2  ,i3+1);
		  weightsg(i1g,i2+1,i3+1) += .25*AREA1(i1,i2+1,i3+1);
		}
	      }
	      else if ( axis==axis2 )
	      {
                // Look for an isolated zone -- 4 adjacent zones are empty
		maskit = (zoneMask(i1+1,i2,i3)==0 && zoneMask(i1,i2,i3+1)==0 &&
		          zoneMask(i1-1,i2,i3)==0 && zoneMask(i1,i2,i3-1)==0);

		if( !maskit )
		{
		  weightsg(i1  ,i2g,i3  ) += .25*AREA2(i1  ,i2,i3  );
		  weightsg(i1+1,i2g,i3  ) += .25*AREA2(i1+1,i2,i3  ); 
		  weightsg(i1  ,i2g,i3+1) += .25*AREA2(i1  ,i2,i3+1); 
		  weightsg(i1+1,i2g,i3+1) += .25*AREA2(i1+1,i2,i3+1); 
		}
	      }
	      else 
	      {
		maskit = (zoneMask(i1+1,i2,i3)==0 && zoneMask(i1,i2+1,i3)==0 &&
		          zoneMask(i1-1,i2,i3)==0 && zoneMask(i1,i2-1,i3)==0);
		if( !maskit )
		{
		  weightsg(i1  ,i2  ,i3g) += .25*AREA3(i1  ,i2  ,i3);
		  weightsg(i1+1,i2  ,i3g) += .25*AREA3(i1+1,i2  ,i3);
		  weightsg(i1  ,i2+1,i3g) += .25*AREA3(i1  ,i2+1,i3);
		  weightsg(i1+1,i2+1,i3g) += .25*AREA3(i1+1,i2+1,i3);
		}
	      }

	    }
	  }
      


//       bool maskit = false;
//       FOR_3(i1,i2,i3,Ibb1,Ibb2,Ibb3)
//       {
// 	if ( zoneMask(i1,i2,i3) )
// 	{
// 	  maskit = zoneMask(i1+1,i2,i3)==0 && zoneMask(i1,i2+1,i3)==0 && 
//                    zoneMask(i1-1,i2,i3)==0 && zoneMask(i1,i2-1,i3)==0;
// 	  if( !maskit )
// 	  {
//             // *wdh* --- check this ---
// 	    wt(i1  ,i2  ,i3) += .25*deltaS(i1  ,i2  ,i3);
// 	    wt(i1+1,i2  ,i3) += .25*deltaS(i1+1,i2  ,i3);
// 	    wt(i1  ,i2+1,i3) += .25*deltaS(i1  ,i2+1,i3);
// 	    wt(i1+1,i2+1,i3) += .25*deltaS(i1+1,i2+1,i3);
// 	  }
// 	}
//       }

      // Copy the wt array into the weightsg array
/* --
      FOR_3D()
      {
	if ( axis==axis1 )
	  weightsg(Ig1.getBase(),i2,i3)=wt(i1,i2);
	else if( axis==axis2 )      
	  weightsg(i1,Ig2.getBase(),i3)=wt(i1,i2);
	else
	  weightsg(i1,i2,Ig3.getBase())=wt(i1,i2);
      }
    -- */

      //		  weightsg(Ig1,Ig2,Ig3).display("weightsg");

    } // end for face
  } // end for surf
  

  
  // now compute the weight contributions from the stitching
  UnstructuredMapping &umap = *stitcher.getUnstructuredGrid();
  const int numberOfTriangles=umap.size(UnstructuredMapping::Face);
  printF("computeStitchedSurfaceWeights: Number of triangles on the unstructured stitcher grid = %i\n",
        numberOfTriangles);

  if( numberOfTriangles==0 )
  {
    allFaceWeightsDefined=true;  // *************** fix this *** only some face weights may be defined
    faceWeightsDefined = true;
    surfaceWeightsDefined = true;
      
    return 0;
  }

//   UnstructuredMapping &umap = *cg.getSurfaceStitching();


  realArray & verts = (realArray &)umap.getNodes();
  intArray & tris = (intArray &) umap.getEntities(UnstructuredMapping::Face);

  realArray tCenters(umap.size(UnstructuredMapping::Face), cg.numberOfDimensions()), 
            tAreas(umap.size(UnstructuredMapping::Face));
  
  ArraySimpleFixed<real,3,1,1,1> v0,v1,v2;

  
  for ( int e=0; e<numberOfTriangles; e++ )
  {
    for ( int a=0; a<3; a++ )
    {
      v0[a] = verts(tris(e,0),a);
      v1[a] = verts(tris(e,1),a);
      v2[a] = verts(tris(e,2),a);
      tCenters(e,a) = (v0[a] + v1[a] + v2[a])/3;
    }

    tAreas(e) = sqrt(ASmag2(areaNormal3D(v0,v1,v2)));
  }

  // Before interpolating we set the mask on the surface grids to their original values
  // so that we can determine valid interpolation points
  stitcher.setMask(SurfaceStitcher::originalMask);

  InterpolatePoints interpolate;
  realArray xp;  // points projected onto the surface 

  // We do not interpolate from the last grid in cgSurf (which is the unstructured grid)
  IntegerArray checkTheseGrids(cgSurf.numberOfComponentGrids());
  checkTheseGrids=1; checkTheseGrids(cgSurf.numberOfComponentGrids()-1)=0;

  #ifndef USE_PPP
    interpolate.buildInterpolationInfo(tCenters, cgSurf, &xp, &checkTheseGrids);
  #else
    Overture::abort("finish me for parallel");
  #endif

  stitcher.setMask(SurfaceStitcher::enlargedHoleMask);  // we could reset the mask if needed

  // Retrieve the interpolation coefficients, interpolationLocations and donor grids
  const int interpWidth=2;  // assume bi-linear interpolation on the surface
  const int interpolationStencilSize = interpWidth*interpWidth;

  RealArray interpCoeff(numberOfTriangles,interpolationStencilSize);
  interpCoeff = 0;
  interpolate.interpolationCoefficients(cgSurf,interpCoeff);

  IntegerArray interpolationLocation,interpoleeGrid;
  interpolate.getInterpolationInfo(cgSurf,interpolationLocation,interpoleeGrid);

  real areasums = 0;

  //  interpCoeff.display("interpCoeff");

  // We need to know how to map cgSurf[surfGrid] --> (side,axis,grid) on cg 
  int *gridSurf = new int [cgSurf.numberOfComponentGrids()];
  int *sideSurf = new int [cgSurf.numberOfComponentGrids()];
  int *axisSurf = new int [cgSurf.numberOfComponentGrids()];
			   
  surfaceGrid=-1;
  for( int surf=0; surf<numberOfSurfaces; surf++ )
  {
    const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(surf);
    for( int face=0; face<numberOfFaces; face++ )
    {
      bodyDefinition.getFace(surf,face,side,axis,grid);
      surfaceGrid++;
      sideSurf[surfaceGrid]=side;
      axisSurf[surfaceGrid]=axis;
      gridSurf[surfaceGrid]=grid;
      
    }
  }
  assert( surfaceGrid==cgSurf.numberOfComponentGrids()-2 ); // note last grid in cgSurf is the unstructured grid
  
  for( int e=0; e<numberOfTriangles; e++ )
  {
    // Each triangle centroid, tCenters(e) can be interpolated from points on a surface grid
    //       u(tCenters(e)) = SUM_{j1,j2} w_{j1,j2} u[surfGrid](j1,j2)
    //
    //   The integral on the triangles is 
    //      Integral(u) = SUM_e { u(tCenters(e))*area(e) } 
    //                  = SUM_e { SUM_{j1,j2} w_{j1,j2}*area(e) u[surfGrid](j1,j2) } 
    //  
    // The weights for the integral for point u[surfGrid](j1,j2) is thus 
    //                     SUM_e w_{j1,j2}*area(e)
    // 

    const int surfGrid = interpoleeGrid(e);  // interpolation is from cgSurf[surfGrid]
    assert( surfGrid>=0 && surfGrid<cgSurf.numberOfComponentGrids() );
    // Location on the surface grid of the lower left corner of the interpolation stencil:
    const int j1 = interpolationLocation(e,0), j2 = interpolationLocation(e,1);

    // Here is the corresponding face on cg:
    const int side=sideSurf[surfGrid];
    const int axis=axisSurf[surfGrid];
    const int grid=gridSurf[surfGrid];

    MappedGrid & mg = cg[grid];

    // Add the weights into: 
    //       weightsg([i1a,i1b],[i2a,i2b],[i3a,i3b]) 
    //
    // NOTE: the relationship between the indices (j1,j2) on the surface grid and the indicies
    //   on the volume grid (i1,i2,i3) is determined by the definition in the ReductionMapping
    int i1a,i1b, i2a,i2b, i3a,i3b;
    if( axis==0 )
    {
      i1a=i1b=mg.gridIndexRange(side,axis) + 2*side -1;  // weights are stored on the ghost line
      i2a=j1; i2b=i2a+1;
      i3a=j2; i3b=i3a+1;
    }
    else if( axis==1 )
    {
      i1a=j1; i1b=i1a+1;
      i2a=i2b=mg.gridIndexRange(side,axis) + 2*side -1;  // weights are stored on the ghost line
      i3a=j2; i3b=i3a+1;
    }
    else
    {
      i1a=j1; i1b=i1a+1;
      i2a=j2; i2b=i2a+1;
      i3a=i3b=mg.gridIndexRange(side,axis) + 2*side -1;  // weights are stored on the ghost line
    }
    
    // printF(" Triangle %i : (j1,j2)=(%i,%i) on surface -> (i1a,i2a,i3a)=(%i,%i,%i)\n",e,j1,j2,i1a,i2a,i3a);

    realArray & weightsg = weights[grid];

    real area = tAreas(e);

    //      cout<<"area is "<<area<<endl;

    real wsum=0;

    int m=0;
    for( int i3=i3a; i3<=i3b; i3++ )
    for( int i2=i2a; i2<=i2b; i2++ )
    for( int i1=i1a; i1<=i1b; i1++ )
    {
      real w = area*interpCoeff(e,m);
      weightsg(i1,i2,i3)+=w;

      m++;
      wsum+=w;
    }

    //      cout<<"sum of the weights = "<<wsum/area<<endl;

    areasums += area;

  }

  delete [] gridSurf;
  delete [] sideSurf;
  delete [] axisSurf;

  //  cout<<"areasums was "<<areasums<<endl;
  allFaceWeightsDefined=true;
  faceWeightsDefined = true;
  surfaceWeightsDefined = true;

  if( useAllBoundaries ) delete &bodyDefinition;

  return 0;
}



int Integrate::
get( const GenericDataBase & dir, const aString & name)
// =====================================================================================
/// \brief Get the Integrate object from the directory "name" of the data base.
// =====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Integrate");
  // subDir.setMode(GenericDataBase::streamInputMode);

  subDir.get( className,"className" );

  subDir.get( useSurfaceStitching,"useSurfaceStitching" ); 

  bd.get( subDir,"bd");

  subDir.get( orderOfAccuracy,"orderOfAccuracy" ); 
  subDir.get( boundaryHasOverlap,"boundaryHasOverlap" ); 
  subDir.get( faceWeightsDefined,"faceWeightsDefined" ); 
  subDir.get( surfaceWeightsDefined,"surfaceWeightsDefined" ); 

  weights.updateToMatchGrid(cg);
  weights.get( subDir,"weights");
  // nullVector.get( subDir,"nullVector");  // do we need to save this ?
  
  subDir.get( weightsComputed,"weightsComputed" ); 
  subDir.get( leftNullVectorComputed,"leftNullVectorComputed" ); 
  subDir.get( allFaceWeightsDefined,"allFaceWeightsDefined" ); 
  subDir.get( weightsUpdatedToMatchGrid,"weightsUpdatedToMatchGrid" ); 

  subDir.get( useAMR,"useAMR" ); 

  // -- for now we do NOT save the AMR info --
  if( useAMR )
  {
    printF("Integrate::get:WARNING: The AMR info was NOT saved in the data-base. \n");
    
  }
  
  subDir.get( radialAxis,"radialAxis" ); 
  
  delete & subDir;
  return true;
}


int Integrate::
put( GenericDataBase & dir, const aString & name) const
// =====================================================================================
/// \brief Put this Integrate object in a sub-directory called "name" of the data base
// =====================================================================================
{
  if( name=="Integrate" )
  {
    printF("Integrate::put:ERROR: the name of the object cannot equal `Integrate' as this is the class name.\n");
    OV_ABORT("error");
  }

  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Integrate");                         // create a sub-directory 

  // subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( className,"className" );

  subDir.put( useSurfaceStitching,"useSurfaceStitching" ); 

  bd.put( subDir,"bd");

  subDir.put( orderOfAccuracy,"orderOfAccuracy" ); 
  subDir.put( boundaryHasOverlap,"boundaryHasOverlap" ); 
  subDir.put( faceWeightsDefined,"faceWeightsDefined" ); 
  subDir.put( surfaceWeightsDefined,"surfaceWeightsDefined" ); 

  weights.put( subDir,"weights");
  // nullVector.put( subDir,"nullVector");  // do we need to save this ?
  
  subDir.put( weightsComputed,"weightsComputed" ); 
  subDir.put( leftNullVectorComputed,"leftNullVectorComputed" ); 
  subDir.put( allFaceWeightsDefined,"allFaceWeightsDefined" ); 
  subDir.put( weightsUpdatedToMatchGrid,"weightsUpdatedToMatchGrid" ); 

  subDir.put( useAMR,"useAMR" ); 

  // -- for now we do NOT save the AMR info --

//   // Weights for AMR grids 
//   bool useAMR;
//   int *numberOfFacesPerSurface;  // total number of faces for each surface, including AMR grids
//   int numberOfBoundarySurfaces;
//   int numberOfBoundaryRefinementLevels;
//   int *numberOfBoundaryFaces;
//   RealArray ***boundaryWeights;
//   int **pNumberOfBoundaryGrids;
//   int ***pBoundaryGrid;

//   // axisymmetric stuff
//   int radialAxis;

  subDir.put( radialAxis,"radialAxis" ); 

  delete &subDir;
  return true;
}


int Integrate::
defineSurfacesAndComputeWeights( GenericGraphicsInterface & gi ) 
//================================================================================
/// \brief Interactively define surfaces and compute weights.
///
/// \details
///  Use this function to interactively define surfaces and compute integration weights.
//================================================================================
{

  char buff[180];  // buffer for sprintf

  GUIState dialog;

  dialog.setWindowTitle("Integrate");
  dialog.setExitCommand("exit", "exit");

  // option menus
//     dialog.setOptionMenuColumns(1);

//     aString opCommand1[] = {"unit square",
// 			    "helical wire",
//                             "fillet for two cylinders",
//                             "blade",
// 			    ""};
    
//     dialog.addOptionMenu( "type:", opCommand1, opCommand1, mappingType); 


//   aString colourBoundaryCommands[] = { "colour by bc",
// 			               "colour by share",
// 			               "" };
//   // dialog.addRadioBox("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );
//   dialog.addOptionMenu("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );

  aString cmds[] = {"define a surface",
		    "define surfaces from share flags",
                    "compute defined surface weights",
                    "compute all surface weights",
                    "compute volume and surface weights",
                    "compute volume weights",
                    "change the plot",
		    "help",
		    ""};
  int numberOfPushButtons=8;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"compute areas and volumes",
                          "use surface stitcher",
  			  ""};
  int tbState[10];

  bool computeAreasAndVolumes=true;
  tbState[0] = computeAreasAndVolumes;
  tbState[1] = useSurfaceStitching;
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=7;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;
  textLabels[nt] = "tolerance:";  sPrintF(textStrings[nt],"%g",tolerance);  nt++; 
  textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%g",debug);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


//   // make a dialog sibling for setting general mapping parameters
//   DialogData & mappingParametersDialog = dialog.getDialogSibling();
//   buildMappingParametersDialog( mappingParametersDialog );

//   dialog.buildPopup(menu);

  gi.pushGUI(dialog);

  int len=0;
  aString answer,line; 

  bool plotObject=true;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  parameters.set(GI_LABEL_GRIDS_AND_BOUNDARIES,true); // turn on plotting of coloured squares

  gi.appendToTheDefaultPrompt("Integrate>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getAnswer(answer,"");  // gi.getMenuItem(menu,answer);
 

    if( answer=="help" )
    {
      printF("---------------------------------------------------------------------------------------\n"
             "This routine can be used to compute integration weights for overlapping grids.\n"
             "Integration weights can be used to compute surface and volume integrals.\n"
             "Here are some things you can do:\n"
             " - define a surface : define a surface from a set of grid faces.\n"
             " - define surfaces from share flags : automatically define surfaces based on the \n"
             "     the share flag associated with the grid. A different surface will be defined\n"
             "     for each non-zero share value.\n"
             " - compute defined surface weights : compute the weights for all defined surfaces.\n"
             " - compute all surface weights : compute weights for all surfaces.\n"
             " - compute volume and surface weights : compute volume weights and all surface weights\n"
             " - compute volume weights : compute volume weights\n"
             " - change the plot : enter the grid plotter so that plotting options can be changed.\n"
             "---------------------------------------------------------------------------------------\n"
             );
    }
    else if( answer=="change the plot" )
    {
      // plot the grid and wait for changes to to the plot 
      gi.erase();
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(gi,cg,parameters);
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

    }
    else if( answer=="define a surface" )
    {
//       gi.inputString(line,sPrintF(buff,"Enter xa,xb, ya,yb (default=[%e,%e]x[%e,%e]): ",
//           xa,xb,ya,yb));
//       if( line!="" ) sScanF(line,"%e %e %e %e ",&xa,&xb,&ya,&yb);
    }
    else if( answer=="define surfaces from share flags" )
    {
      int numberOfSurfaces=0;
      // -- first count the number of distinct share flags ---

      std::map<int, int> shareList;

      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	for( int axis=0; axis<cg.numberOfDimensions(); axis++)for( int side=0; side<=1; side++ )
	{
          int share=mg.sharedBoundaryFlag(side,axis);
	  if( share!=0 )
	  {
	    if( shareList.count(share)==0 )
	    {
	      // this is a new share value
	      shareList[share]=numberOfSurfaces;
	      numberOfSurfaces++;
	    }
	  }
	}
      }
      printF("Integrate:There were %i distinct surfaces found.\n",numberOfSurfaces);

      const int maxNumberOfFaces = cg.numberOfComponentGrids()*2*cg.numberOfDimensions();
      IntegerArray boundary(3,maxNumberOfFaces); // we could do better here
      for( map<int,int>::iterator ii=shareList.begin(); ii!=shareList.end(); ++ii)
      {
        const int surfaceNumber=(*ii).second;
	const int share=(*ii).first;
	printF("Surface %i has share value %i.\n",surfaceNumber,share);

	int numberOfFaces=0;
	boundary=-1;
	
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = cg[grid];
	  for( int axis=0; axis<cg.numberOfDimensions(); axis++)for( int side=0; side<=1; side++ )
	  {
	    if( mg.sharedBoundaryFlag(side,axis)==share )
	    {
	      boundary(0,numberOfFaces)=side;
	      boundary(1,numberOfFaces)=axis;
	      boundary(2,numberOfFaces)=grid;
	      printF("Add (side,axis,grid)=(%i,%i,%i=%s) to surface %i share=%i\n",
                     side,axis,grid,(const char*)mg.getName(),surfaceNumber,share);
	      numberOfFaces++;
	    }
	  }
	}
	assert( numberOfFaces>0 );
	
	defineSurface( surfaceNumber,numberOfFaces,boundary ); 
	printF("Integrate: define surface %i, share=%i, numberOfFaces=%i.\n",surfaceNumber,share,numberOfFaces);
	
      }

//       for( int surface=0; surface<numberOfSurfaces; surface++ )
//       {
// 	int share = 

// 	  {
// 	    boundary(0,numberOfFaces)=side;
// 	    boundary(1,numberOfFaces)=axis;
// 	    boundary(2,numberOfFaces)=grid;

// 	    numberOfFaces++;
// 	  }
// 	}
//       }
//       if( numberOfFaces>0 )
//       {
// 	integrate.defineSurface( surfaceNumber,numberOfFaces,boundary ); 
//       }

    } // end define surfaces from share flags
    else if( answer=="compute defined surface weights" )
    {
      const int numberOfSurfaces=bd.totalNumberOfSurfaces();
      if( numberOfSurfaces==0 )
      {
	printF("Integrate:WARNING:There are no surfaces defined yet!\n");
	continue;
      }
      if( computeAreasAndVolumes )
      { // compute areas
	realCompositeGridFunction u(cg);
	u=1.;
	for( int surface=0; surface<numberOfSurfaces; surface++ )
	{
	  real surfaceArea = surfaceIntegral(u,surface);
	  printF("Surface %i : surfaceArea = %11.5e.\n",surface,surfaceArea);
	}
      }
      else
      {
	for( int surface=0; surface<numberOfSurfaces; surface++ )
	{
	  if( !surfaceWeightsDefined(surface) )
	  {
	    // We first determine if we can compute the weights  for this surface directly in the
	    // case when there are no overlapping grids on the boundary.
	    if( computeSurfaceWeights(surface) !=0 )
	      computeWeights();  // fall back method
	  }
	}
      }
      if( useSurfaceStitching && pSurfaceStitcher!=NULL  )
      {
        // plot the stitched surface:

	CompositeGrid & cgSurf = *(pSurfaceStitcher->getSurfaceCompositeGrid());
	gi.erase();
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	PlotIt::plot(gi,cgSurf,parameters);
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      }
      
    }
    else if( answer=="compute all surface weights" )
    {
      const int surface=-1; // this mean the total surface area
      if( computeAreasAndVolumes )
      { // compute surface area
	realCompositeGridFunction u(cg);
	u=1.;
	real surfaceArea = surfaceIntegral(u,surface);
	printF("Total surfaceArea = %11.5e.\n",surfaceArea);
      }
      else
      {
	if( !surfaceWeightsDefined(surface) )
	{
	  // We first determine if we can compute the weights  for this surface directly in the
	  // case when there are no overlapping grids on the boundary.
	  if( computeSurfaceWeights(surface) !=0 )
	    computeWeights();  // fall back method
	  else
	    printF("Integrate:INFO: The surface integration weights are already computed.\n");
	}
      }
    }
    else if( answer=="compute volume weights" ||
             answer=="compute volume and surface weights" )
    {
      if( computeAreasAndVolumes )
      { // compute volumes and surface areas
	realCompositeGridFunction u(cg);
	u=1.;
	real volume = volumeIntegral(u);
	real surfaceArea = surfaceIntegral(u);
	printF("Volume=%11.5e, total surfaceArea = %11.5e.\n",volume,surfaceArea);
      }
      else
      {
	if( !weightsComputed )
	  computeWeights();
	else
	  printF("Integrate:INFO: The surface and volume integration weights are already computed.\n");
      }
    }
    else if( dialog.getTextValue(answer,"tolerance:","%e",tolerance) )
    {
      printF("Integrate:INFO: Setting tolerance=%11.5e. This tolerance is for the sparse solver which is used\n"
             " to solve the linear equations defining the integration weights.\n");
    }
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){}//
    else if( dialog.getToggleValue(answer,"compute areas and volumes",computeAreasAndVolumes) ){}//
    else if( dialog.getToggleValue(answer,"use surface stitcher",useSurfaceStitching) )
    {
      if( useSurfaceStitching )
	printF("Surface weights will be computed from the surface stitching alogorithm in Ugen.\n");

    }
    else if( answer=="plotObject" )
    {
      plotObject=true;
    }
    else if( answer=="exit" )
      break;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if( plotObject )
    {
      gi.erase();
      PlotIt::plot(gi,cg,parameters);   // *** recompute every time ?? ***

      plotObject=false;
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset

  gi.popGUI(); // restore the previous GUI

  return 0;
}
