#include "Overture.h"
#include "Ogen.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>
#include "display.h"
#include "conversion.h"
#include "GridStatistics.h"
#include "ReparameterizationTransform.h"
#include "ShowFileReader.h"
#include "ParallelUtility.h"
#include "LoadBalancer.h"
#include "App.h"

// Macro to extract a local array with ghost boundaries
//  type = int/float/double/real
//  xd = distributed array
//  xs = serial array 
#ifdef USE_PPP
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
 #define GET_LOCAL_CONST(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray & xs = xd
 #define GET_LOCAL_CONST(type,xd,xs)\
    const type ## SerialArray & xs = xd
#endif

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )


// we need to define these here for gcc
// typedef CompositeGridData_BoundaryAdjustment       BoundaryAdjustment;
typedef TrivialArray<BoundaryAdjustment,Range>     BoundaryAdjustmentArray;
typedef TrivialArray<BoundaryAdjustmentArray,Range>BoundaryAdjustmentArray2;

static real holePointSize=-1;

// extendIndexRange: = indexRange except on interpolation edges where = indexRange +/- (dw-1)/2 

static inline int 
decode(int mask )
// decode the mask ->  1=interior, 2=ghost, -2,3=interiorBoundaryPoint,  <0 =interp 
{
  int m=0;
  if( (mask & MappedGrid::ISdiscretizationPoint) && (mask & MappedGrid::ISinteriorBoundaryPoint) )
    m=3;
  else if( mask & MappedGrid::ISdiscretizationPoint )
    m=1;
  if( mask<0 && mask>-100 )
    m=mask;
  else if( mask<0 )
    m=-1;
  else if( mask & MappedGrid::ISghostPoint )
    m=2;

  if( mask<0 && (mask & MappedGrid::ISinteriorBoundaryPoint) )
    m=-2;

  return m;
}


// ====================================================================================================
/// \brief Print the locations of orphan points.
// ====================================================================================================
int Ogen::
printOrphanPoints(CompositeGrid & cg )
{
   
  printF("\n-----------------------------------------------------------------------------------------------\n");
  if( numberOfOrphanPoints<=0 )
  {
    printF("There are no orphan points.\n");
  }
  const int nc=cg.numberOfDimensions();
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfBaseGrids = cg.numberOfBaseGrids();
  Range Rx=numberOfDimensions;
  RealArray x(1,3), r(1,3);
  x=0.;
  for( int n=0; n<numberOfOrphanPoints; n++ )
  {
    int grid=int(fabs(orphanPoint(n,numberOfDimensions)+.5));
    if( grid<0 || grid >= numberOfBaseGrids )
    {
      printF("ERROR: orphanPoint %i has grid=%i which is invalid! Skipping this point...\n",n,grid);
      continue;
    }
	
    MappedGrid & g = cg[grid];
    Mapping & map = g.mapping().getMapping();
    // invert x to get iv[]
    r=-1.;  
    x(0,Rx)=orphanPoint(n,Rx);
    #ifdef USE_PPP
      map.inverseMapS(x,r);
    #else
      map.inverseMap(x,r);
    #endif
    int iv[3]={0,0,0};
    for( int axis=0; axis<numberOfDimensions; axis++ )
      iv[axis]=int(r(0,axis)/g.gridSpacing(axis)+.5)+g.gridIndexRange(0,axis);

    printF("  orphan pt %i: (grid,i1,i2,i3) = (%i, %i,%i,%i), x=(%8.2e,%8.2e,%8.2e) (grid %s)\n",
             n,grid,iv[0],iv[1],iv[2], x(0,0),x(0,1),x(0,2), (const char*)g.getName());

  }
  printF("-----------------------------------------------------------------------------------------------\n");
  return 0;
}



//\begin{>ogenInclude.tex}{\subsubsection{saveGridToAFile}}
int Ogen::
saveGridToAFile(CompositeGrid & cg, aString & gridFileName, aString & gridName ) 
// =======================================================================================
// /Description:
//   This function will output a CompositeGrid to a file.
//
// /cg (input) : grid to save
// /gridFileName : name of the file to save such as "myGrid.hdf"
// /gridName : save the grid under this name in the data base file.
//
//\end{ogenInclude.tex}
// =========================================================================================
{
  printF("Ogen::saveGridToAFile: Saving the CompositeGrid in %s\n",(const char*)gridFileName);

  HDF_DataBase dataFile;
  dataFile.mount(gridFileName,"I");

  int streamMode=1; // save in compressed form.
  dataFile.put(streamMode,"streamMode");
  if( !streamMode )
    dataFile.setMode(GenericDataBase::noStreamMode); // this is now the default
  else
  {
    dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
  }
           
  cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
  cg.put(dataFile,gridName);
    
  dataFile.unmount();


  return 0;
}


//\begin{>ogenInclude.tex}{\subsubsection{buildACompositeGrid}}
int Ogen::
buildACompositeGrid(CompositeGrid & cg, 
		    MappingInformation & mapInfo, 
		    const IntegerArray & mapList,
		    const int & numberOfMultigridLevels /* =1 */,
		    const bool useOldGrid /* =false */)
// =========================================================================================
// /Description:
//   Build a CompositeGrid with optional multigrid levels from a set of Mappings.
//  This routine will build a CompositeGrid from a list of Mapping's. 
//  
// /cg (input/output): if useAnOldGrid==true then this holds the old grid on input. On output cg
// holds the new grid.
// /mapInfo (input): This object holds a list of Mappings from which we choose a subset to build the grid from.
// /mapList (input): indicate which Mappings in mapInfo.mappingList should be added to the CompositeGrid.
//    Add mapInfo.mappingList[mapList(i)] for each element of mapList.
// /useAnOldGrid (input) : this means we are adding new grids to an existing overlapping grid.
//\end{ogenInclude.tex}
// =========================================================================================
{
  int numberOfOldGrids = useOldGrid ? cg.numberOfComponentGrids() : 0;
  int numberOfNewGrids = sum( mapList >=0 );

  int numberOfGrids= numberOfOldGrids + numberOfNewGrids;
  int numberOfDimensions=mapInfo.mappingList[mapList(0)].getRangeDimension();


// Is this needed? 980118
//  CompositeGrid cg2(numberOfDimensions, numberOfGrids);
//  cg.reference(cg2);
  
//   cg.setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids); // this doesn't seem to work
  real time0=getCPU();
  CompositeGrid *cgOld=NULL;
  if( useAnOldGrid )
  {
    // ----- copy stuff from old grids ------------

    // *no* cg.setNumberOfGrids(numberOfGrids);
    cgOld =new CompositeGrid;
    *cgOld=cg;
    
    cg = CompositeGrid(numberOfDimensions, numberOfGrids);
    int g;
    for( g=0; g<numberOfOldGrids; g++ )
      cg[g]=(*cgOld)[g];
      
    cg.numberOfInterpolationPoints=0;
    Range G0(0,numberOfOldGrids-1);
    cg.numberOfInterpolationPoints(G0)=(*cgOld).numberOfInterpolationPoints(G0);
    
    cg.update(
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeGrid           |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpolationCoordinates,
      CompositeGrid::COMPUTEnothing);

    Range Rx(0,numberOfDimensions-1);
    for( g=0; g<numberOfOldGrids; g++)
    {
      if( cg.numberOfInterpolationPoints(g)>0 )
      {
	Range I(0,cg.numberOfInterpolationPoints(g)-1);
	cg.interpoleeGrid[g](I)             = (*cgOld).interpoleeGrid[g](I);
	cg.interpolationPoint[g](I,Rx)      = (*cgOld).interpolationPoint[g](I,Rx);
	cg.interpoleeLocation[g](I,Rx)      = (*cgOld).interpoleeLocation[g](I,Rx);
	cg.interpolationCoordinates[g](I,Rx)= (*cgOld).interpolationCoordinates[g](I,Rx);
      }
    }
    display(cg.numberOfInterpolationPoints,"cg.numberOfInterpolationPoints");
    
    const int numberOfBaseGrids = cg.numberOfBaseGrids();
    const int numberOfOldBaseGrids = (*cgOld).numberOfBaseGrids();
    
    if( (*cgOld).rcData->boundaryAdjustment.getNumberOfElements() )
    {
      // If there was a boundary adjustment array on the old grid, copy over existing values:
      BoundaryAdjustmentArray2 & boundaryAdjustment = cg.rcData->boundaryAdjustment;

      boundaryAdjustment.redim(numberOfGrids,numberOfBaseGrids);

      for( int k1=0; k1<numberOfOldGrids; k1++ )
      {
	for( int k2=0; k2<numberOfOldBaseGrids; k2++ )
	{
	  boundaryAdjustment(k1,k2)=(*cgOld).rcData->boundaryAdjustment(k1,k2);

	}
      }
    }
    

  }
  else
  {
    cg = CompositeGrid(numberOfDimensions, numberOfGrids);

    const int numberOfGrids = cg.numberOfComponentGrids();
    const int numberOfBaseGrids = cg.numberOfBaseGrids();
    BoundaryAdjustmentArray2 & boundaryAdjustment = cg.rcData->boundaryAdjustment;
    boundaryAdjustment.redim(numberOfGrids,numberOfBaseGrids);
  }
  
  if( info & 4 ) printF("time for creating a CompositeGrid=%e \n",getCPU()-time0);
  
//  cg.numberOfCompleteMultigridLevels() = 1;

// From specifyOverlap:
  int k,l;
  for (l=1; l<numberOfMultigridLevels; l++) 
  {
    for (k=0; k<numberOfGrids; k++)
    {
      IntegerArray factor(3); factor = 1;
      cg.addMultigridCoarsening(factor, l, k);
    } // end for
  }
  cg.numberOfCompleteMultigridLevels() = numberOfMultigridLevels;

  if (numberOfMultigridLevels > 1) 
  {
    // cg.makeCompleteMultigridLevels(); // ***needed ??
  
    cg.update(CompositeGrid::THEmultigridLevel);
  }




  for( l=0; l<cg.numberOfMultigridLevels(); l++ )
  {
    CompositeGrid & c = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];
    
    int k1,side,axis,map=-1;
    for( k1=0; k1<numberOfGrids; k1++) 
    {
      MappedGrid & g1 = c[k1];
      if( k1>=numberOfOldGrids )
      {
	map++;
	while( mapList(map)<0 )  // find next positive map number
	  map++;
    
	Mapping & mapping = mapInfo.mappingList[mapList(map)].getMapping();

	mapping.incrementReferenceCount();
	g1.reference(mapping);
	if (mapping.decrementReferenceCount() == 0) delete &mapping;
      }
      
      int numberOfInterpolationPoints=0;
      int interpolationWidth=3;
  
      for(axis=0; axis<numberOfDimensions; axis++) 
      {
	// use defaults here: uncomment to change

	// g1.discretizationWidth()(axis)   = 3;


	// g1.numberOfGhostPoints()(1,axis) = 0;
	// g1.isCellCentered()(axis) = LogicalFalse;
	for( side=0; side<2; side++)
	{
	  // g1.boundaryDiscretizationWidth(side,axis) = 3;
	  // g1.gridIndexRange()(side,axis) = range(side+1,axis+1)-numberOfGhostPoints;  // *wdh* set base to 0

  	  g1.setNumberOfGhostPoints(side,axis,Ogen::defaultNumberOfGhostPoints); // *wdh* April 1, 2007

	  g1.setSharedBoundaryTolerance(side,axis,.1);
	} 
      } 

      cg.maximumHoleCuttingDistance(nullRange,nullRange,k1)=SQRT(.1*REAL_MAX); // this will be squared
      c.maximumHoleCuttingDistance(nullRange,nullRange,k1)=SQRT(.1*REAL_MAX);

      if( k1>=numberOfOldGrids )
        cg.numberOfInterpolationPoints(k1) = numberOfInterpolationPoints;
      for (int k2=0; k2<cg.numberOfComponentGrids(); k2++) 
      {
	cg.interpolationIsImplicit(k1,k2,l) = defaultInterpolationIsImplicit;
	// cg.backupInterpolationIsImplicit(k1,k2,l)     = LogicalTrue;

	for( axis=0; axis<numberOfDimensions; axis++ )
	{
	  cg.interpolationWidth(axis,k1,k2,l)              =interpolationWidth;
	  cg.interpolationOverlap(axis,k1,k2,l)            = .5;
	  // cg.backupInterpolationOverlap(axis,k1,k2,l)      = .5; 
	  cg.multigridCoarseningRatio(axis,k1,l)           = 2;
	  cg.multigridProlongationWidth(axis,k1,l)         = 3;
	  cg.multigridRestrictionWidth(axis,k1,l)          = 3;
	}
	// note: may cut holes does not have multigrid levels.
	cg.mayCutHoles(k1,k2)=true;   
	c.mayCutHoles(k1,k2)=true;   
	cg.sharedSidesMayCutHoles(k1,k2)=false;
	c.sharedSidesMayCutHoles(k1,k2)=false;
      
	for( axis=numberOfDimensions; axis<3; axis++ )
	{
	  cg.interpolationWidth(axis,k1,k2,l)              =1;
	  cg.interpolationOverlap(axis,k1,k2,l)            = -.5;
	  // cg.backupInterpolationOverlap(axis,k1,k2,l)      = -.5;
	  cg.multigridCoarseningRatio(axis,k1,l)           = 1;
	  cg.multigridProlongationWidth(axis,k1,l)         = 1;
	  cg.multigridRestrictionWidth(axis,k1,l)          = 1;
	}
      
	// cg.interpolationConditionLimit(k1,k2,l)       = 0.;
	// cg.backupInterpolationConditionLimit(k1,k2,l) = -.5; // **wdh** I use this for the min. overlap if >0
	cg.interpolationPreference(k1,k2,l)           = k1;
	cg.mayInterpolate(k1,k2,l)                    = LogicalTrue;
	// cg.mayBackupInterpolate(k1,k2,l)              = LogicalFalse;
	  
      } // for k2

      // now copy values form the old grid

      if( k1<numberOfOldGrids )
      {
        Range G0(0,numberOfOldGrids-1);
        cg.mayCutHoles(G0,G0)=(*cgOld).mayCutHoles(G0,G0);
        cg.sharedSidesMayCutHoles(G0,G0)=(*cgOld).sharedSidesMayCutHoles(G0,G0);
        cg.interpolationIsImplicit(G0,G0,l)=(*cgOld).interpolationIsImplicit(G0,G0,l);
        cg.maximumHoleCuttingDistance(nullRange,nullRange,G0)=
                 (*cgOld).maximumHoleCuttingDistance(nullRange,nullRange,G0);
      }
#ifdef USE_PPP
      // In parallel we perform a consistency check on the number of grid points on each proc.
      if( true )
      {
        for( int axis=0; axis<numberOfDimensions; axis++ )for( int side=0; side<=1; side++ )
	{
	  int nMin = ParallelUtility::getMinValue(g1.gridIndexRange(side,axis));
	  int nMax = ParallelUtility::getMaxValue(g1.gridIndexRange(side,axis));
	  if( nMin!=nMax )
	  {
	    printF("buildACompositeGrid:ERROR: inconsistent values in parallel for gridIndexRange for grid %i\n"
                   " The values on different processors do not match!\n", k1);
	
	    printf("buildACompositeGrid: myid=%i grid k1=%i : gridIndexRange=[%i,%i][%i,%i][%i,%i]\n",
		   myid,k1,
		   g1.gridIndexRange(0,0),g1.gridIndexRange(1,0),
		   g1.gridIndexRange(0,1),g1.gridIndexRange(1,1),
		   g1.gridIndexRange(0,2),g1.gridIndexRange(1,2));
            Mapping & map1 = g1.mapping().getMapping();
            printf(" myid=%i map.getGridDimensions=[%i,%i,%i]\n",
                   myid,map1.getGridDimensions(0),map1.getGridDimensions(1),map1.getGridDimensions(2));
	    fflush(0);
	    Overture::abort("error");
	  }
	}
      }
#endif      

    } // end for k1
  } // end for l

//   printF("*** At end of buildACompositeGrid***\n");
//   int grid;
//   for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//   {
//     cg[grid].displayComputedGeometry();
//   }
  
  // load balance: (is this ok to do it here?)
  if( loadBalanceGrids )
  {
    // From GridCollection.C : 

    LoadBalancer loadBalancer; // could there already be a load balancer with cg ? 
    GridDistributionList & gridDistributionList = cg->gridDistributionList;
    
    // work-loads per grid are based on the number of grid points by default:
    loadBalancer.assignWorkLoads( cg,gridDistributionList );

    loadBalancer.determineLoadBalance( gridDistributionList );

    // From GenericGridCollection.C: get: 
    // Assign parallel distribution (if the info is there)
    for( int i=0; i<numberOfGrids; i++ )
    {
      int pStart=-1,pEnd=0;
      gridDistributionList[i].getProcessorRange(pStart,pEnd);
      if( debug & 2 )
	printF("Ogen:assignLoadBalance: assign grid %i to processors=[%i,%i]\n",i,pStart,pEnd);
      cg[i].specifyProcesses(Range(pStart,pEnd));
    }

  }
  



  #ifdef USE_PPP
    cg.displayDistribution("Ogen::after creating grids from Mappings");
  #endif
  
  delete cgOld;
  return 0;
}



//\begin{>>ogenInclude.tex}{\subsubsection{setGridParameters}}
int Ogen::
setGridParameters(CompositeGrid & cg )
// ====================================================================================================
//  /Description:
//     Specify the number of grid points on each grid based on the number of multigrid levels.
//
//\end{ogenUpdateInclude.tex}
// ====================================================================================================
{
  if( cg.numberOfMultigridLevels()<2 )
    return 0;

  IntegerArray gridIndexRange(2,cg.numberOfDimensions(),cg.numberOfComponentGrids());
  int l,grid,side,axis,grid1,grid2;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if (cg.numberOfMultigridLevels() == 1)
      assert(cg.componentGridNumber(grid) == grid);
    else 
      assert(cg.multigridLevel[0].componentGridNumber(grid) == grid);
      
    MappedGrid & g0 = cg.numberOfMultigridLevels() == 1 ?  cg[grid] : cg.multigridLevel[0][grid];
    for( axis=0; axis<g0.numberOfDimensions(); axis++ )
    {
      Integer & nra = gridIndexRange(0,axis,grid), &nrb = gridIndexRange(1,axis,grid);

      nra=0;
      nrb=g0.mapping().getGridDimensions(axis)-1;
      
      // * g0.setIsCellCentered(axis, nra == nrb ?  LogicalFalse : isCellCentered(axis,grid));
      // Make sure mrs is a multiple of the right power of two.
      // Take into account degenerate dimensions.
      Integer mrs = 1;
      grid1 = cg.numberOfMultigridLevels() == 1 ? cg.gridNumber(grid) : cg.multigridLevel[0].gridNumber(grid);
      for (l=1; l<cg.numberOfMultigridLevels(); l++, grid1=grid2)
      {
	if (cg.numberOfMultigridLevels() == 1)
	  assert(cg.componentGridNumber(grid) == grid);
	else
	  assert(cg.multigridLevel[l].componentGridNumber(grid)==grid);
	MappedGrid & g2 = cg.numberOfMultigridLevels() == 1 ?  cg[grid] : cg.multigridLevel[l][grid];
	grid2 = cg.numberOfMultigridLevels() == 1 ? cg.gridNumber(grid) : cg.multigridLevel[l].gridNumber(grid);
	if (nrb == nra) 
	{
//	  cg.multigridCoarseningFactor(axis,grid2)   = 1;
//	  cg.multigridCoarseningRatio(axis,grid,l)   = 1;
//	  cg.multigridProlongationWidth(axis,grid,l) = 1;
//	  cg.multigridRestrictionWidth(axis,grid,l)  = 1;
	} 
	else 
	{
          assert( grid1>=0 );
	  // * cg.multigridCoarseningRatio(axis,grid,l)   =  multigridCoarseningRatio(axis,grid,l);
//	  cg.multigridCoarseningFactor(axis,grid2)   =  cg.multigridCoarseningFactor(axis,grid1) *
//                                                       cg.multigridCoarseningRatio(axis,grid,l);
	  // * cg.multigridProlongationWidth(axis,grid,l) = multigridProlongationWidth(axis,grid,l);
// *	  cg.multigridRestrictionWidth(axis,grid,l)  =  multigridRestrictionWidth(axis,grid,l);
	  if (g0.isCellCentered(axis))
	  {
//                              multigridCoarseningRatio and
//                              multigridRestrictionWidth must
//                              be both odd or both even.
//	    if ((cg.multigridRestrictionWidth(axis,grid,l) -cg.multigridCoarseningRatio(axis,grid,l)) % 2)
//	      cg.multigridRestrictionWidth(axis,grid,l)++;
	  } 
	  else
	  {
//                              multigridRestrictionWidth must be odd.
	    if (cg.multigridRestrictionWidth(axis,grid,l) % 2== 0) 
              cg.multigridRestrictionWidth(axis,grid,l)++;
//                              multigridProlongationWidth must be even.
	    if (cg.multigridProlongationWidth(axis,grid,l) % 2)
	      cg.multigridProlongationWidth(axis,grid,l)++;
	  } // end if
	} // end if
	mrs *= cg.multigridCoarseningRatio(axis,grid,l);
      } // end for
      g0.setGridIndexRange(0, axis, nra >= 0 ?  nra - (nra % mrs) :  nra - (((-nra) * (mrs - 1)) % mrs) );
      g0.setGridIndexRange(1, axis, nrb >= 0 ?  nrb + (nrb * (mrs - 1)) % mrs : nrb + ((-nrb) % mrs) );
      for (l=1; l<cg.numberOfMultigridLevels(); l++)
      {
	MappedGrid &g_l1 = cg.multigridLevel[ l ][grid], &g_l2 = cg.multigridLevel[l-1][grid];
	for( side=Start; side<=End; side++ )
	{
	  g_l1.setGridIndexRange(side, axis,
				 g_l2.gridIndexRange(side,axis) >= 0 ?
				 ( g_l2.gridIndexRange(side,axis)) / cg.multigridCoarseningRatio(axis,grid,l) :
				 (-g_l2.gridIndexRange(side,axis)) / cg.multigridCoarseningRatio(axis,grid,l));

          g_l1.setNumberOfGhostPoints(side,axis,g0.numberOfGhostPoints(side,axis));
	}
      } // end for
    } // end for_1
    for (l=1; l<cg.numberOfMultigridLevels(); l++)
    {
      MappedGrid& g = cg.numberOfMultigridLevels() == 1 ? cg[grid] : cg.multigridLevel[l][grid];
      for( axis=0; axis<g.numberOfDimensions(); axis++ )
      {
        //                      Set the extra boundary conditions.
//	g.setIsCellCentered(axis, g0.isCellCentered(axis));
//	Integer m21 =  g.gridIndexRange(1,axis) - g.gridIndexRange(0,axis);
//	if (g.isCellCentered(axis) || g.isPeriodic(axis) != Mapping::notPeriodic) m21--;
        //                      Make sure the discretization is not too wide.
	// * g.setDiscretizationWidth(axis, min0(discretizationWidth(axis,grid,l),
	// *			      g.isPeriodic(axis) != Mapping::notPeriodic ? 2*m21+1 : (m21-1)/2*2+1));
	for( side=Start; side<=End; side++ )
	{
          //                          Make sure boundary discretization is not too wide.
	  // * g.setBoundaryDiscretizationWidth(side, axis, min0(boundaryDiscretizationWidth(side,axis,grid,l),m21));
	} // end for_1
      } // end for_1
//      g.update(MappedGrid:: NOTHING); // Compute dimensions, etcg.
    } // end for_1
  } // end for_1
  //
  //          Compute the actual values of CompositeGrid parameters.
  //
  Range all;
  for (l=0; l<cg.numberOfMultigridLevels(); l++)
  {
    CompositeGrid & cgl = cg.multigridLevel[l];

    cgl.interpolationIsImplicit      =cg.interpolationIsImplicit(all,all,0);
    // cgl.backupInterpolationIsImplicit=cg.backupInterpolationIsImplicit(all,all,0);
    cgl.interpolationWidth           =cg.interpolationWidth(all,all,all,0);
    cgl.interpolationOverlap         =cg.interpolationOverlap(all,all,all,0);
    // cgl.backupInterpolationOverlap   =cg.backupInterpolationOverlap(all,all,all,0);
    cgl.multigridCoarseningRatio     =cg.multigridCoarseningRatio(all,all,0);
    cgl.multigridProlongationWidth   =cg.multigridProlongationWidth(all,all,0);
    cgl.multigridRestrictionWidth    =cg.multigridRestrictionWidth(all,all,0);
    cgl.mayCutHoles                  =cg.mayCutHoles(all,all,0);
    cgl.sharedSidesMayCutHoles       =cg.sharedSidesMayCutHoles(all,all,0);
  }
  
  
  cg.destroy(CompositeGrid::EVERYTHING & ~CompositeGrid::THElists);
  return 0;
}


int Ogen::
getNormal(const int & numberOfDimensions, 
          const int & side, const int & axis, 
          const RealArray & xr, 
          real signForJacobian,
          RealArray & normal )
// =========================================================================================
// /Description:
//    Compute the normal given the jacobian derivatives xr 
//
//  /side,axis (input) : derivative comes from this face of a mapping
//  /xr (input) : Jacobian derivatives
//  /signForJacobian (input) : sign for the Jacobian of the mapping
//  /normal (ouptut) : the outward normal
// =========================================================================================
{
//   real jac = side ? 1. : -1.;
//   switch (numberOfDimensions)
//   {
//   case 1:
//     jac *= xr(0,0,0);
//     break;
//   case 2:
//     jac *= xr(0,0,0) * xr(0,1,1) - xr(0,0,1) * xr(0,1,0);
//     break;
//   case 3:
//     jac *=
//       (xr(0,0,0)*xr(0,1,1)-xr(0,0,1)*xr(0,1,0))*xr(0,2,2) +
//       (xr(0,0,1)*xr(0,1,2)-xr(0,0,2)*xr(0,1,1))*xr(0,2,0) +
//       (xr(0,0,2)*xr(0,1,0)-xr(0,0,0)*xr(0,1,2))*xr(0,2,1);
//     break;
//   } // end switch
//   jac = (jac < 0.) ? -1. : (jac > 0.) ? 1. : 0.;

  real signForNormal = signForJacobian*(2*side-1);   // outward normal 

  int ap1=(axis+1) % numberOfDimensions;
  const real eps = REAL_MIN*100.;
  real an1,an2,an3, scale;
  if( numberOfDimensions==2 )
  {
    signForNormal*=(1-2*axis);

    if( xr.getLength(0)==1 && normal.numberOfDimensions()==1 )
    {
      // *wdh* 070314 -- the 2d version of this was never correct ! -- now fixed --

      // fn(d1,d2,d3,l) = (Real)((1-2*l)*(1-2*kd)) * xr(d1,d2,d3,1-l,1-kd);
      an1= xr(0,1,ap1);                         
      an2=-xr(0,0,ap1);
      scale = signForNormal/max( sqrt(SQR(an1)+SQR(an2)), eps);
      normal(0)=an1*scale;
      normal(1)=an2*scale;
    }
    else
    {
      for( int i=xr.getBase(0); i<=xr.getBound(0); i++ )
      {
	an1= xr(i,1,ap1); 
	an2=-xr(i,0,ap1);
	scale = signForNormal/max( sqrt(SQR(an1)+SQR(an2)), eps);  

        normal(i,0)=an1*scale;
        normal(i,1)=an2*scale;
      }
    }
  }
  else
  {
    int ap2=(axis+2) % numberOfDimensions;
    if( xr.getLength(0)==1 && normal.numberOfDimensions()==1 )
    {
      an1=xr(0,1,ap1)*xr(0,2,ap2)-xr(0,2,ap1)*xr(0,1,ap2);
      an2=xr(0,2,ap1)*xr(0,0,ap2)-xr(0,0,ap1)*xr(0,2,ap2);
      an3=xr(0,0,ap1)*xr(0,1,ap2)-xr(0,1,ap1)*xr(0,0,ap2);

      scale = signForNormal/max( sqrt(SQR(an1)+SQR(an2)+SQR(an3)), eps);
      normal(0)=an1*scale;
      normal(1)=an2*scale;
      normal(2)=an3*scale;
    }
    else
    {
      for( int i=xr.getBase(0); i<=xr.getBound(0); i++ )
      {
	an1=xr(i,1,ap1)*xr(i,2,ap2)-xr(i,2,ap1)*xr(i,1,ap2);
	an2=xr(i,2,ap1)*xr(i,0,ap2)-xr(i,0,ap1)*xr(i,2,ap2);
	an3=xr(i,0,ap1)*xr(i,1,ap2)-xr(i,1,ap1)*xr(i,0,ap2);

        scale = signForNormal/max( sqrt(SQR(an1)+SQR(an2)+SQR(an3)), eps);
	normal(i,0)=an1*scale;
	normal(i,1)=an2*scale;
	normal(i,2)=an3*scale;
      }
      
    }
    
  }
//   if( l2Norm==0. )
//   {
//     cout << "Ogen::getNormal::ERROR: normal has length zero!\n";
//     normal=0.;
//     return 1;
//   }

  return 0;
}


int Ogen::
estimateSharedBoundaryTolerance(CompositeGrid & cg)
// ======================================================================================
//
// /Description:
//   Estimate the tolerance for shared boundaries. This is not currently used.
//
// ======================================================================================
{
  return 0;

/* ----
  Index I1,I2,I3;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & g = cg[grid];
    Mapping & map = g.mapping().getMapping();
    const RealArray & x = g.vertex();
    const realMappedGridFunction & xr = g.vertexDerivative();

    int side,axis;
    int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
    is1=is2=is3=0;
    
    for( axis=axis1; axis<cg.numberOfDimensions(); axis++ )
    {
      const int axisp1 = (axis+1) % cg.numberOfDimensions();
      for( side=Start; side<=End; side++ )
      {
        if( g.sharedBoundaryFlag(side,axis) != 0 )
	{
          getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);
          real tol;
          if( cg.numberOfDimensions()==2 )
	  {
            is[axisp1]=1;
	    tol=
	      .5*max(
		(fabs((x(I1+is1,I2+is2,I3,0)-2.*x(I1,I2,I3,0)+x(I1-is1,I2-is2,I3,0)))+
		 fabs((x(I1+is1,I2+is2,I3,1)-2.*x(I1,I2,I3,1)+x(I1-is1,I2-is2,I3,1))))
		/(fabs(xr(I1,I2,I3,0,axis))+fabs(xr(I1,I2,I3,1,axis)))
		);
            is[axisp1]=0;
	  }
          else
	  {
            RealArray denom = 1./(fabs(xr(I1,I2,I3,0,axis))+fabs(xr(I1,I2,I3,1,axis))+fabs(xr(I1,I2,I3,2,axis)));
            tol=0.;
            for( int dir=0; dir<=1; dir++ )
	    {
              int axisp=(axis+1+dir) % cg.numberOfDimensions();
              is[axisp]=1;
	      tol=max(tol,max(
		(fabs((x(I1+is1,I2+is2,I3+is3,0)-2.*x(I1,I2,I3,0)+x(I1-is1,I2-is2,I3-is3,0)))+
		 fabs((x(I1+is1,I2+is2,I3+is3,1)-2.*x(I1,I2,I3,1)+x(I1-is1,I2-is2,I3-is3,1)))+
		 fabs((x(I1+is1,I2+is2,I3+is3,2)-2.*x(I1,I2,I3,2)+x(I1-is1,I2-is2,I3-is3,2))))*denom));
              is[axisp]=0;
	    }
	    tol/=3.;
	  }
	  // g.sharedBoundaryTolerance()(side,axis)=max(g.sharedBoundaryTolerance(side,axis),tol/g.gridSpacing(axis));
          if( debug & 2 )
  	    printF("estimated shared boundary tolerance =%e (%e dr) for grid=%s, (side,axis)=(%i,%i)\n",
		 tol,tol/g.gridSpacing(axis),(const char*)map.getName(Mapping::mappingName),side,axis);
	}
      }
    }
  }
  return 0;
---- */
}


int Ogen::
interpolateMixedBoundary(CompositeGrid & cg, int mixedBoundaryNumber )
// =================================================================================================
// /Description:
//   Interpolate a mixed boundary. A mixed boundary is used to build a c-grid, h-grid or
// block-structured grid. This function interpolates the mixed boundaries of all specied mixed boundaries.
// ================================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  const int n = mixedBoundaryNumber;
  int grid=mixedBoundary(n,0);
  int side=mixedBoundary(n,1);
  int axis=mixedBoundary(n,2);
  int grid2=mixedBoundary(n,3);

  int iva[3], &i1a=iva[0], &i2a=iva[1], &i3a=iva[2];
  int ivb[3], &i1b=ivb[0], &i2b=ivb[1], &i3b=ivb[2];
  
  i1a=mixedBoundary(n,4);
  i1b=mixedBoundary(n,5);
  i2a=mixedBoundary(n,6);
  i2b=mixedBoundary(n,7);
  i3a=mixedBoundary(n,8);
  i3b=mixedBoundary(n,9);

  const bool autoDetect= i1a>i1b; // if true we automatically detect mixed boundary interp points.
  

  bool checkAll= grid2<0;
  int grid2Start= checkAll ? 0 : grid2;
  int grid2End=   checkAll ? numberOfBaseGrids-1 : grid2;

  for( grid2=grid2Start; grid2<=grid2End; grid2++ )
  {
    if( checkAll && grid==grid2 )
      continue;

    assert( grid2>=0 && grid2<numberOfBaseGrids );
    MappedGrid & g =cg[grid];
    MappedGrid & g2=cg[grid2];

    int offset[3]={0,0,0}; //

    cg.numberOfInterpolationPoints(grid)=0;  // ***** fix this -- 

    real rOffset[3]={0.,0.,0.};
    real rScale[3]={1.,1.,1.};
    if( grid!=grid2 )
    {
      interpolateMixedBoundary(cg,n,side,axis,grid,g,grid2,g2,offset,rScale,rOffset);
    }
    else
    {
      // mixed boundary where a grid interpolates from itself -- must be a c-grid or alike.
      // Split the grid into two parts so we can determine interpolation


      // Look for a user specified splitAxis and splitIndex
      int splitAxis=mixedBoundary(n,10);
      int splitIndex=mixedBoundary(n,11);
      const bool autoSplit=splitAxis<0;
      if( autoSplit )
        splitAxis= (axis+1) % cg.numberOfDimensions();  // default splitAxis

      Mapping & map = g.mapping().getMapping();
      real rb[2][3]= {0.,0.,0., 1.,1.,1.};//
      ReparameterizationTransform map1(map,ReparameterizationTransform::restriction);

      int num=map.getGridDimensions(splitAxis);
      assert( num>1 );
      int iEnd1;  
      int iStart2;
      if( autoSplit )
      {
	int num2=(num-1)/2+1;
	iEnd1=num2-1-1; // num2-1. // skip last point
        iStart2=num2-1+1; 
      }
      else
      {
        iEnd1=splitIndex-1;  // splitIndex // skip last point so we don't interpolate a point from itself
        iStart2=splitIndex+1;
      }
	
      real rEnd1=iEnd1/(num-1.);  // grid block 1 ends here
      

      rb[0][splitAxis]=0.;
      rb[1][splitAxis]=rEnd1;

      map1.setGridDimensions(splitAxis,iEnd1+1);
      map1.setBounds(rb[0][0],rb[1][0],rb[0][1],rb[1][1],rb[0][2],rb[1][2]);
      map1.setBoundaryCondition(side,axis,g.boundaryCondition(side,axis));
      
      // we need to use default inverse so that we avoid inverting the original mapping as part
      // of the inverse of the composition mapping.
      map1.useDefaultMappingInverse(true);
	  
      ReparameterizationTransform map2(map,ReparameterizationTransform::restriction);


      // const int iStart2=num2-1+1; // num2-1.
      real rStart2=iStart2/(num-1.); // grid block 2 starts here
      rb[0][splitAxis]=rStart2;
      rb[1][splitAxis]=1.;
      map2.setGridDimensions(splitAxis,num-iStart2);
      map2.setBounds(rb[0][0],rb[1][0],rb[0][1],rb[1][1],rb[0][2],rb[1][2]);
      map2.setBoundaryCondition(side,axis,g.boundaryCondition(side,axis));

      // display(map2.getGrid(),"map2: grid",logFile);
    
      map2.useDefaultMappingInverse(true);

      MappedGrid mg1(map1), mg2(map2);
      mg1.update(MappedGrid::THEvertex);
      mg2.update(MappedGrid::THEvertex);
         
          // ps->erase();
          // ps->plot(mg1);
          // ps->plot(mg2);

      // With autoDetect we look for interpolation from mg1 to mg2 and vice versa -- should this be optional?

      if( autoDetect || ivb[splitAxis] <=iStart2)
      {

	rOffset[splitAxis]=rStart2;   // r position where grid2 really starts
	rScale[splitAxis]=1.-rStart2; // r scale factor for grid2 
	interpolateMixedBoundary(cg,n,side,axis, 
				 grid,mg1, grid2,mg2,  // interpolate mg1 from mg2
				 offset,rScale,rOffset);
	
      }
      if( autoDetect || iva[splitAxis] >=iEnd1 )
      {
        // Now check the other half 
	rOffset[splitAxis]=0.;
	rScale[splitAxis]=rEnd1;
	offset[splitAxis]=iStart2;
	interpolateMixedBoundary(cg,n,side,axis, 
				 grid2,mg2, grid,mg1,  // interpolate mg2 from mg1
				 offset,rScale,rOffset);
      }
    }
  }
  
  return 0;
}



int Ogen::
interpolateMixedBoundary(CompositeGrid & cg, int mixedBoundaryNumber,
                         int side, int axis, int grid, MappedGrid & g, int grid2, MappedGrid & g2,
                         int o[3], real rScale[3], real rOffset[3] )
// =================================================================================================
// /Description:
//   Try to interpolate ghost points from this mixed physical/interpolation  boundary.
//  Interpolate points on portions of the physical boundary of "grid" from "grid2".
//  This is used, for example, to define interp. on the edge of a c-grid or h-grid
//
// /o (input) : offsets in index space of grid g from cg[grid] -- used for computing a c-grid when
//  cg[grid] is split into two pieces.
// /rScale,rOffset : scale factor and offset for grid2 (used for a c-grid)
//
// /cg.inverseGrid, cg.inverseCoordinates, cg[grid].mask (output): These arrays are assigned values.
//
// /Note: It is better to have c-grid or h-grid boundaries overlap by epsilon rather be separated
//    by epsilon for then this routine will always get the matching boundary correct.
// ===============================================================================================
{
  // debug |= 8;
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  
  assert( grid>=0 && grid <=numberOfBaseGrids );
  assert( grid2>=0 && grid2 <=numberOfBaseGrids );
  
  g.update(MappedGrid::THEvertex | MappedGrid::THEcenter);  // fix this for rectangular grids
  
  intArray & mask = cg[grid].mask();
  const realArray & vertex = g.vertex();
  realArray & rI = cg.inverseCoordinates[grid];
  intArray & inverseGrid = cg.inverseGrid[grid];
  
  Mapping & map2 = g2.mapping().getMapping();

  const int n=mixedBoundaryNumber;
  int iva[3], &i1a=iva[0], &i2a=iva[1], &i3a=iva[2];
  int ivb[3], &i1b=ivb[0], &i2b=ivb[1], &i3b=ivb[2];
  
  i1a=mixedBoundary(n,4);
  i1b=mixedBoundary(n,5);
  i2a=mixedBoundary(n,6);
  i2b=mixedBoundary(n,7);
  i3a=mixedBoundary(n,8);
  i3b=mixedBoundary(n,9);
  const int markNonCutting=mixedBoundary(n,12);

  const bool autoDetect= i1a>i1b; // if true we automatically detect mixed boundary interp points.

  if( debug & 4 )
  {
    printF("Check for mixed boundary on grid %s from grid %s, (side,axis)=(%i,%i) tol=%6.2e\n",
	   (const char*)g.getName(),(const char*)g2.getName(),side,axis,mixedBoundaryValue(mixedBoundaryNumber,0));
 
    fprintf(logFile,"Check for mixed boundary on grid %s (%i) from grid %s (%i), (side,axis)=(%i,%i) tol=%6.2e\n",
	    (const char*)g.getName(),grid,(const char*)g2.getName(),grid2,side,axis,
            mixedBoundaryValue(mixedBoundaryNumber,0));
  }
  
  const int axisp1=(axis+1) % cg.numberOfDimensions();
  const int axisp2=(axis+2) % cg.numberOfDimensions();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range R,Rx(0,cg.numberOfDimensions()-1);
  const int extra=1;
  getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3,extra);

  if( !autoDetect )
  {
    // If the grid has been split in two we need to shift the user specified
    // region of interpolation
    for( int dir=0; dir<g.numberOfDimensions(); dir++ )
    {
      iva[dir]-=o[dir];
      ivb[dir]-=o[dir];
    }
    
    // shift to user specified boundary of ghost line:
    Iv[axis]=Range(iva[axis],ivb[axis]);
  }
  

  intArray m(I1,I2,I3);  // m as one layer of ghost values

  m=0;  // should be 1 on interpolation ends
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  int dir;
  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
  {
    J1=I1; J2=I2; J3=I3;
    if( dir!=axis )
    {
      if( g.boundaryCondition(Start,dir)==0 )
      {
	Jv[dir]=Iv[dir].getBase();
	m(J1,J2,J3)=1;
      }
      if( g.boundaryCondition(End,dir)==0 )
      {
	Jv[dir]=Iv[dir].getBound();
	m(J1,J2,J3)=1;
      }
    }
  }
  
  realArray x,r,r2;
  
  const real tol = mixedBoundaryValue(mixedBoundaryNumber,0);
  assert( tol>= -1. && tol<1. );
  real xtol = mixedBoundaryValue(mixedBoundaryNumber,1);
  
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
  getGhostIndex(g.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

  if( autoDetect )
  {
    // *******************************************************************
    // **** Automatically determine mixed boundary interpolation points ***
    // *******************************************************************

    getBoundaryIndex(g.gridIndexRange(),side,axis,I1,I2,I3);
	
	
  
    // first try to interpolate boundary points from the other grid

    int num=I1.getLength()*I2.getLength()*I3.getLength();
    R=Range(0,num-1);

    x.redim(I1,I2,I3,Rx); r.redim(R,Rx);
    x=vertex(I1,I2,I3,Rx);
    x.reshape(R,Rx);
	
    r=-1.;
    map2.inverseMap(x,r);

    // project points onto the boundary of grid2 so we can measure the distance of x points to the
    // the boundary of g2.
    realArray r2(R,Rx),x2(R,Rx), dist(R);
    r2=r;
    // project to boundary
  

    int side2=-1, dir2=-1;
    if( grid==grid2 )
    {
      // c-grid
      side2=side;
      dir2=axis;
    }
    else
    {
      for( dir=0; dir<cg.numberOfDimensions() && side2<0; dir++ )
      {
	for( int s=Start; s<=End; s++ )
	{
	  if( g2.boundaryFlag(s,dir)==MappedGrid::mixedPhysicalInterpolationBoundary )
	  {
	    side2=s;
	    dir2=dir;
	    break;
	  }
	}
      }
    }
  
    if( side2>=0 )
    {
      // project r2 onto the boundary of grid2
      for( dir=0; dir<cg.numberOfDimensions(); dir++ )
	if( dir!=dir2 )
	  r2(R,dir)=max(rBound(Start,dir,grid2),min(rBound(End,dir,grid2),r2(R,dir)));
    
      r2(R,dir2)=(real)side2;
      map2.map(r2,x2);  // r2 is on the boundary of g2, so is x2.
    }
    else
    {
      printF("interpolateMixedBoundary:WARNING: grid %s (side,axis)=(%i,%i) has a mixed boundary condition\n"
	     "    with grid2=%s but grid2 has no mixed boundary condition. \n"
	     "    I will NOT use the x-tolerance for matching the boundaries.\n",
	     (const char*)g.getName(),side,axis,(const char*)g2.getName());
      x2=0.;
      xtol=0.;
    }

    r.reshape(I1,I2,I3,Rx);

  // const real eps = g2.gridSpacing(??);

  // match(I1,I2,I3) : true is a grid point line up from one grid to the next.
  //                   Used to adjust the matching points by +/- one on the ends.
// ***  IntegerArray match(I1,I2,I3); 

  // point must be very near or inside the other grid
    if( cg.numberOfDimensions()==2 )
    {
      m(I1,I2,I3)= fabs(r(I1,I2,I3,0)-.5)<.5+tol && fabs(r(I1,I2,I3,1)-.5)<.5+tol;

      dist(R)=SQR(x2(R,0)-x(R,0))+SQR(x2(R,1)-x(R,1));
      dist.reshape(I1,I2,I3);
      m(I1,I2,I3)= m(I1,I2,I3) || dist(I1,I2,I3) < xtol*xtol;
    }
    else
    {
      m(I1,I2,I3)= fabs(r(I1,I2,I3,0)-.5)<.5+tol && fabs(r(I1,I2,I3,1)-.5)<.5+tol && fabs(r(I1,I2,I3,2)-.5)<.5+tol;
      dist(R)=SQR(x2(R,0)-x(R,0))+SQR(x2(R,1)-x(R,1))+SQR(x2(R,2)-x(R,2));
      dist.reshape(I1,I2,I3);
      m(I1,I2,I3)= m(I1,I2,I3) || dist(I1,I2,I3) < xtol*xtol;
    }

    if( side2>=0 )
    {
      real rMismatch=-1.;
      where( m(I1,I2,I3) )
	rMismatch=max(0., max(fabs(r(I1,I2,I3,dir2)-.5))-.5);
  
      printF("interpolateMixedBoundary : grid=%s : largest r-mismatch at boundary = %e \n"
	     "   This is the largest separation between the matching boundaries, restricted by rtol=%e, xtol=%e\n",
	     (const char*)g.getName(),rMismatch,tol,xtol);
    }
  

  } //
  else
  {
    // *** USER specify region to interpolate *****
    I1=Range(i1a,i1b);
    I2=Range(i2a,i2b);
    I3=Range(i3a,i3b);

    int num=I1.getLength()*I2.getLength()*I3.getLength();
    R=Range(0,num-1);

    x.redim(I1,I2,I3,Rx);
    x=vertex(I1,I2,I3,Rx);

    x.reshape(R,Rx);
    r.redim(R,Rx);
    r=-1.;
    map2.inverseMap(x,r);

    // display(r,"r after inverting on the boundary","%8.2e ");
    

    r.reshape(I1,I2,I3,Rx);
    x.reshape(I1,I2,I3,Rx);

    if( cg.numberOfDimensions()==2 )
    {
      m(I1,I2,I3)= fabs(r(I1,I2,I3,0)-.5)<.5+tol && fabs(r(I1,I2,I3,1)-.5)<.5+tol;
    }
    else
    {
      m(I1,I2,I3)= fabs(r(I1,I2,I3,0)-.5)<.5+tol && fabs(r(I1,I2,I3,1)-.5)<.5+tol && fabs(r(I1,I2,I3,2)-.5)<.5+tol;
    }

    int numInterpolated=sum(m(I1,I2,I3));
    printF(" interpolateMixedBoundaries: INFO: user specified region [%i,%i][%i,%i][%i,%i] has %i points\n"
           "          Number of points actually interpolated = %i\n",
                              i1a+o[0],i1b+o[0],i2a+o[1],i2b+o[1],i3a+o[2],i3b+o[2],num,numInterpolated);

    if( true )
    {
      for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
	if( m(i1,i2,i3)==0 )
	{
	  printF(" interpolateMixedBoundaries: pt (%i,%i,%i) was not interpolated. x=(%9.3e,%9.3e,%9.3e)"
                 " r=(%9.3e,%9.3e,%9.3e) tol=%8.2e\n",
                 i1,i2,i3,
		 x(i1,i2,i3,0),x(i1,i2,i3,1),( cg.numberOfDimensions()==2 ? 0. : x(i1,i2,i3,2)),
		 r(i1,i2,i3,0),r(i1,i2,i3,1),( cg.numberOfDimensions()==2 ? 0. : r(i1,i2,i3,2)),tol);
	}
      }
    }

    int side2=-1, dir2=-1;
    for( dir=0; dir<cg.numberOfDimensions() && side2<0; dir++ )
    {
      for( int s=Start; s<=End; s++ )
      {
	if( g2.boundaryFlag(s,dir)==MappedGrid::mixedPhysicalInterpolationBoundary )
	{
	  side2=s;
	  dir2=dir;
	  break;
	}
      }
    }
    if( side2>=0 )
    {
      real rMismatch=-1.;
      where( m(I1,I2,I3) )
	rMismatch=max(0., max(fabs(r(I1,I2,I3,dir2)-.5))-.5);
  
      printF("interpolateMixedBoundary : grid=%s : largest r-mismatch at boundary = %e \n"
	     "   This is the largest separation between the matching boundaries, restricted by rtol=%e, xtol=%e\n",
	     (const char*)g.getName(),rMismatch,tol,xtol);
    }
    else
    {
      printF("interpolateMixedBoundary:WARNING: grid %s (side,axis)=(%i,%i) has a mixed boundary condition\n"
	     "    with grid2=%s but grid2 has no mixed boundary condition. \n",
	     (const char*)g.getName(),side,axis,(const char*)g2.getName());
    }
    
  }
  
  

  if( debug & 8 )
  {
    fprintf(logFile," *** I1=[%i,%i] o[0]=%i\n",I1.getBase(),I1.getBound(),o[0]);
    
    display(x,"x on boundary",logFile);
    display(r,"r after inverse on boundary",logFile);
    fprintf(logFile,"---INFO: m=1 at points that can interpolate\n");
    display(m,"m after inverse on boundary",logFile);
    // display(match,"match: true if grid points match on the two grids",logFile);
  }

  // displayMask(mask,"mask before mixed BC");
  bool newWay=true;
  
  if( grid==grid2 && autoDetect )
  {
    // I think this next step was for a C-grid to only interpolate points where we
    // do not already interpolate from another grid (?)
    m(I1,I2,I3) = m(I1,I2,I3) && 
      (mask(I1+o[0],I2+o[1],I3+o[2]) & MappedGrid::ISdiscretizationPoint);  
  }
  
  if( debug & 8 )
  {
    fprintf(logFile," *** o[0]=(%i,%i,%i)\n",o[0],o[1],o[2]);
    displayMask(mask(I1+o[0],I2+o[1],I3+o[2]),"mask(I1+o[0],I2+o[1],I3+o[2])",logFile);
    display(m,"m after inverse on boundary and fixup",logFile);
  }

  if( autoDetect && ( !newWay || grid<grid2 || (grid==grid2 && rOffset[axisp1]>0.) ) )
  {
    // *** If grid is a lower priority grid we will interpolate the ghost points 
    // *** THEREFORE set the boundary points to be disretization 
    where( m(I1,I2,I3) && inverseGrid(I1+o[0],I2+o[1],I3+o[2])==grid2 )
    {
      mask(I1+o[0],I2+o[1],I3+o[2])=MappedGrid::ISdiscretizationPoint;  // undo interp on boundaries
    }
  }
  else
  {
    // ****** grid is a higher priority grid than grid2 *******
    // Interpolate the boundary if we interpolate from a lower priority grid
    where( m(I1,I2,I3) )
    {
      mask(I1+o[0],I2+o[1],I3+o[2])=MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint
        |  ISnonCuttingBoundaryPoint; 

      bool interpGhostLine=Iv[axis].getBase()==(g.gridIndexRange(side,axis)+2*side-1);
      if( !autoDetect && interpGhostLine )
      {
        // if we are interpolating the ghost line then mark the boundary points as non-cutting interior
        // boundary points.
        J1=I1, J2=I2, J3=I3;
	Jv[axis]=g.gridIndexRange(side,axis);
        mask(J1+o[0],J2+o[1],J3+o[2])=MappedGrid::ISdiscretizationPoint |  MappedGrid::ISinteriorBoundaryPoint |
                                     ISnonCuttingBoundaryPoint;
      }
      
      
      // we need to mark the ghost line too for the BC mask
      // ** mask(Ig1+o[0],Ig2+o[1],Ig3+o[2])=MappedGrid::ISghostPoint | MappedGrid::ISinteriorBoundaryPoint;

      inverseGrid(I1+o[0],I2+o[1],I3+o[2])= grid2;      //  can interpolate from grid 2
      for( dir=0; dir<cg.numberOfDimensions(); dir++ )
	rI(I1+o[0],I2+o[1],I3+o[2],dir)=rScale[dir]*r(I1,I2,I3,dir)+rOffset[dir];  // save interp. coordinates
    }
    if( !autoDetect && markNonCutting ) // *wdh* added 020709
    {
      // Mark all discretization points of a user specified region as non-cutting
      where( mask(I1,I2,I3) & MappedGrid::ISdiscretizationPoint )
      {
	mask(I1,I2,I3) |= ISnonCuttingBoundaryPoint;
      }
    }

    if( debug & 8 )
    {
      displayMask(mask,"mask after mixed boundary interpolation (boundary interp)",logFile);
    }


    return 0; // ************************** we are done in this case *********************

  }

  // ***** CASE: grid<grid2 : interpolating from a higher priority grid *********

  // Now attempt to interpolate ghost points
  
  x.reshape(Ig1,Ig2,Ig3,Rx);
  x=vertex(Ig1,Ig2,Ig3,Rx);

  x.reshape(R,Rx);
  r.reshape(R,Rx);
  r2=r;   // save r
  map2.inverseMap(x,r);

  r.reshape(Ig1,Ig2,Ig3,Rx);

  // valid points must be able interpolate from both bndry and ghost line
//  const real onePlusTol=1.+tol;

  if( cg.numberOfDimensions()==2 )
    m(I1,I2,I3)=m(I1,I2,I3) && fabs(r(Ig1,Ig2,Ig3,0)-.5)<.5+tol && fabs(r(Ig1,Ig2,Ig3,1)-.5)<.5+tol;
//    m(I1,I2,I3)=m(I1,I2,I3) && r(Ig1,Ig2,Ig3,0)>=-tol && r(Ig1,Ig2,Ig3,0)<=onePlusTol &&  
//                               r(Ig1,Ig2,Ig3,1)>=-tol && r(Ig1,Ig2,Ig3,1)<=onePlusTol; 
  else 
    m(I1,I2,I3)=m(I1,I2,I3) && fabs(r(Ig1,Ig2,Ig3,0)-.5)<.5+tol && fabs(r(Ig1,Ig2,Ig3,1)-.5)<.5+tol &&
                fabs(r(Ig1,Ig2,Ig3,2)-.5)<.5+tol;
//    m(I1,I2,I3)=m(I1,I2,I3) && r(Ig1,Ig2,Ig3,0)>=-tol && r(Ig1,Ig2,Ig3,0)<=onePlusTol &&  
//      r(Ig1,Ig2,Ig3,1)>=-tol && r(Ig1,Ig2,Ig3,1)<=onePlusTol &&  
//      r(Ig1,Ig2,Ig3,2)>=-tol && r(Ig1,Ig2,Ig3,2)<=onePlusTol;

  // display(x,"x after inverse mixed on ghost points",logFile);
  if( debug & 8 )
  {
    display(r,"r after inverse on ghost points",logFile);
    display(m,"m after inverse on ghost points",logFile);
  }
  

  //
  if( false ) // ***wdh* 001007  I don't think we need this (cf. twoBlock.cmd or backStep)
  {
    // The last interior point that matches should be a BC point
    // we need to change any 1's in the mask that are next to a 0 to be a 0.
    if( cg.numberOfDimensions()==2 )
    {
      if( axis==0 )
	where( m(I1,I2,I3) && ( m(I1,I2+1,I3)==0  || m(I1,I2-1,I3)==0 ) )
	  m(I1,I2,I3)=0;	 
      else		 
	where( m(I1,I2,I3) && ( m(I1+1,I2,I3)==0  || m(I1-1,I2,I3)==0  ) )
	  m(I1,I2,I3)=0;	 
    }			 
    else			 
    {			 
      if( axis==0 )	 
	where( m(I1,I2,I3) && (m(I1,I2+1,I3)==0 || m(I1,I2-1,I3)==0  ||
			       m(I1,I2,I3+1)==0 || m(I1,I2,I3-1)==0  ) )
	  m(I1,I2,I3)=0;	 
      else if( axis==1 )	 
	where( m(I1,I2,I3) && (m(I1+1,I2,I3)==0 || m(I1-1,I2,I3)==0  || 
			       m(I1,I2,I3+1)==0 || m(I1,I2,I3-1)==0  ) )
	  m(I1,I2,I3)=0;	 
      else 		 
	where( m(I1,I2,I3) && (m(I1+1,I2,I3)==0 || m(I1-1,I2,I3)==0  || 
			       m(I1,I2+1,I3)==0 || m(I1,I2-1,I3)==0  ) )
	  m(I1,I2,I3)=0;
    }
  }
  
  m(I1,I2,I3) = m(I1,I2,I3) && 
            (mask(I1+o[0],I2+o[1],I3+o[2]) & MappedGrid::ISdiscretizationPoint);  
  where( m(I1,I2,I3) )
  {
    mask(Ig1+o[0],Ig2+o[1],Ig3+o[2]) = MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint; 
    mask(I1+o[0],I2+o[1],I3+o[2]) = MappedGrid::ISdiscretizationPoint | ISnonCuttingBoundaryPoint;

    inverseGrid(Ig1+o[0],Ig2+o[1],Ig3+o[2])= grid2;      //  can interpolate from grid 2
    for( dir=0; dir<cg.numberOfDimensions(); dir++ )
      rI(Ig1+o[0],Ig2+o[1],Ig3+o[2],dir)=rScale[dir]*r(Ig1,Ig2,Ig3,dir)+rOffset[dir];  // save interp. coordinates
  }
    

  if( cg.numberOfDimensions()==2 )
  {
    // Print an informational message:
    intArray ia;
    ia=m(I1,I2,I3).indexMap();
    // display(m(I1,I2,I3),"m(I1,I2,I3)");
    // display(ia,"m(I1,I2,I3).indexMap()");
    int i, bound=ia.getBound(axisp1);
    for( i=ia.getBase(axisp1); i<bound; i++ )
    {
      int i0=i;
      while( i<bound && ia(i+1)==ia(i)+1 )
      {
	i++;
      }
      printF("mixedBoundary: contiguous interval found: [%i,%i]   gridIndexRange=[%i,%i] \n",
	     ia(i0)+o[axisp1],ia(i)+o[axisp1],cg[grid].gridIndexRange(Start,axisp1),cg[grid].gridIndexRange(End,axisp1));
    }
  }
  else
  {
    // Print an informational message:
    intArray ia;
    ia=m(I1,I2,I3).indexMap();

    Range R=ia.dimension(0);
    
    int iva[3], ivb[3];
    for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
    {
      if( dir<=ia.getBound(1) )
      {
	iva[dir]=min(ia(R,dir));
	ivb[dir]=max(ia(R,dir));
      }
      else
      {
        iva[dir]=cg[grid].gridIndexRange(side,axis);
	ivb[dir]=iva[dir];
      }
    }
    printF("mixedBoundary: grid=%s : Mixed boundary interpolation points lie inside [%i,%i]x[%i,%i]x[%i,%i] \n",
	   (const char*)g.getName(), iva[0],ivb[0],iva[1],ivb[1],iva[2],ivb[2]);
    

  }
  
  // End conditions:
  //   At the end of a mixed boundary :
  //     (1) make the end point a boundary point if the opposite boundary is a physical BC
  //         EXCEPT if it lines up with the end of the other grid
  //                       |      |     |    |
  //                   -------------------------
  //                       |      |     |    |
  //                       |      |     |    |
  //                   -----------------------------
  //                ---I---I---I---I---I---X      <---- point X is turned into a bndry pt
  //                   |   |   |   |   |   |            I=interiorBoundaryPoint
  //                   |   |   |   |   |   |
  //                -----------------------B <---  bc>0 boundary
  //                   |   |   |   |   |   |
  //
  //
  //                     |      |     |    |
  //                     |      |     |    |
  //               -------------------------
  //                ---I---I---I---I---I---I      <---- last point remains an interiorBoundaryPoint
  //                   |   |   |   |   |   |            I=interiorBoundaryPoint
  //                   |   |   |   |   |   |
  //                -----------------------B <---  bc>0 boundary
  //                   |   |   |   |   |   |
  r2.reshape(I1,I2,I3,Rx);
  for( dir=0; dir<cg.numberOfDimensions(); dir++ )
  {
    J1=I1; J2=I2; J3=I3;
    if( dir!=axis )
    {
      for( int s=Start; s<=End; s++ )
      {
	if( g.boundaryCondition(s,dir)>0 )
	{
 	  Jv[dir]= s==Start ? Iv[dir].getBase() : Iv[dir].getBound() ;
	  where( m(J1,J2,J3) )
	  {
	    m(J1,J2,J3)=0;
	  }
/* ---- don't do this *wdh* 991106
          if( g.numberOfDimensions()==2 )
	  {
	    where(m(J1,J2,J3) && (fabs(fabs(r2(J1,J2,J3,0)-.5)-.5)>tol || 
	    		  fabs(fabs(r2(J1,J2,J3,1)-.5)-.5)>tol ) ) 
	    {
	      m(J1,J2,J3)=0;
	    }
	  }
	  else
	  {
	    where(m(J1,J2,J3) && (fabs(fabs(r2(J1,J2,J3,0)-.5)-.5)<tol || 
				  fabs(fabs(r2(J1,J2,J3,1)-.5)-.5)<tol ||
				  fabs(fabs(r2(J1,J2,J3,2)-.5)-.5)<tol ) )
	    {
	      m(J1,J2,J3)=0;
	    }
	  }
---- */
	}
      }
    }
  }
  
  where( m(I1,I2,I3) )
  {
    // mark BC points for plotting
    mask(I1+o[0],I2+o[1],I3+o[2]) = MappedGrid::ISdiscretizationPoint |   // ** wdh don't do 991106
      MappedGrid::ISinteriorBoundaryPoint | ISnonCuttingBoundaryPoint; 
//     else
//     {
//       mask(I1+o[0],I2+o[1],I3+o[2]) = MappedGrid::ISdiscretizationPoint | 
// 	ISnonCuttingBoundaryPoint; 
//     }
  }
  

  // There may have been some ghost points on this mixed boundary that were set
  // to interpolation by the boundaryAdjustment -- we need to undo these. They will
  // be interpolation points that are not marked as ISinteriorBoundaryPoint points.
  // This case occurs for hgrid3d.cmd
  getGhostIndex(g.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
  where( ( mask(Ig1,Ig2,Ig3) & MappedGrid::ISinterpolationPoint ) &&
	 !(mask(Ig1,Ig2,Ig3) & MappedGrid::ISinteriorBoundaryPoint ) )
  {
    mask(Ig1,Ig2,Ig3)= MappedGrid::ISdiscretizationPoint;
  }

  // display(x,"x after inverse mixed on boundary",logFile);
  if( debug & 8 )
  {
    display(r,"r after inverse mixed on boundary",logFile);
    display(m,"m after reducing by one from 'ends'",logFile);
    displayMask(mask,"mask after mixed boundary interpolation",logFile);
  }

  return 0;
}


int Ogen::
checkInterpolationOnBoundaries(CompositeGrid & cg)
// ======================================================================================
//
// /Description:
// For each physical boundary of each grid:
//     Find all points on the boundary that can be interpolated from other grids.
//
// ======================================================================================
{
  real time0=getCPU();
  if( info & 4 )
    printF("checking interpolation on boundaries...\n");
  if( info & 2 )
  {
    // Overture::printMemoryUsage("interpolate boundaries (start)");
    Overture::checkMemoryUsage("interpolate boundaries (start)");
  }
  
  checkArrayIDs("Ogen::checkInterpolationOnBoundaries:start");

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  const int & numberOfDimensions = cg.numberOfDimensions();
  real angleDiff = 1.-maximumAngleDifferenceForNormalsOnSharedBoundaries;


  estimateSharedBoundaryTolerance(cg);

  // This next function does not currently do anything
  preInterpolateGrids( cg );

  real timeForBoundaryAdjustment=0.;
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range R,Rx(0,cg.numberOfDimensions()-1);
  RealArray xx(1,3),rr(1,3);
  rr=-1.;
  
  int i;
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
  real x0,x1,x2;

  IntegerArray sidesShare(2,3,2,3); 

  // local arrays to hold interpolation data temporarily

  // *wdh* 990417  cg.numberOfInterpolationPoints=0;

  bool isInteriorBoundaryPoint=false;

  int dir;
    
  for( int grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
    intArray & maskd = g.mask();
    Mapping & map1 = g.mapping().getMapping();
    realArray & rId = cg.inverseCoordinates[grid];
    intArray & inverseGridd = cg.inverseGrid[grid];
    // const realArray & center = g.center();
    // const realArray & vertex = g.vertex();
    const bool isRectangular = g.isRectangular();


    GET_LOCAL(int,maskd,mask);
    GET_LOCAL(int,inverseGridd,inverseGrid);
    GET_LOCAL(real,rId,rI);
    
    #ifdef USE_PPP
      realSerialArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(g.center(),center);
      realSerialArray vertex; if( !isRectangular ) getLocalArrayWithGhostBoundaries(g.vertex(),vertex);
    #else
      const realSerialArray & center = g.center();
      const realSerialArray & vertex = g.vertex();
    #endif


    const real cvShift = g.isAllCellCentered()? .0 : .5;  // shift to get nearest point
    const IntegerArray & extended = extendedGridIndexRange(g);

//     int maxNumberOfInterpolationPoints;
//       maxNumberOfInterpolationPoints=(extended(End,axis1)-extended(Start,axis1)+2)
// 	                            *(extended(End,axis2)-extended(Start,axis2)+2);
//     if( cg.numberOfDimensions()==3 )
//       maxNumberOfInterpolationPoints*=(extended(End,axis3)-extended(Start,axis3)+2);

    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}}, xabc[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
      g.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<g.numberOfDimensions(); dir++ )
      {
	iv0[dir]=g.gridIndexRange(0,dir);
        xabc[0][dir]=xab[0][dir]; xabc[1][dir]=xab[1][dir]; 
	if( g.isAllCellCentered() )
	  xabc[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }
		
    }
    // center:
    #define XC(iv,axis) (xabc[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))
    // vertex:
    #define XV(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))
    #define XV0(i1,i2,i3) (xab[0][0]+dvx[0]*(i1-iv0[0]))
    #define XV1(i1,i2,i3) (xab[0][1]+dvx[1]*(i2-iv0[1]))
    #define XV2(i1,i2,i3) (xab[0][2]+dvx[2]*(i3-iv0[2]))

    if( info & 4 ) 
      fPrintF(logFile,"\n\n ***** checking boundaries of grid: %s for interpolation points\n",
                   (const char*)g.mapping().getName(Mapping::mappingName));

    if( !isNew(grid) && cg.numberOfInterpolationPoints(grid)>0 )
    {
      // ***** Incremental update : this grid is old and can be reused. *****
      #ifdef USE_PPP
      Overture::abort("checkInterpolationOnBoundaries:finish this for P++");
      #else
      Range I(0,cg.numberOfInterpolationPoints(grid)-1);

      const intArray & ip = cg.interpolationPoint[grid];
      if( numberOfDimensions==2 )
      {
        mask(ip(I,0),ip(I,1),0)=MappedGrid::ISinterpolationPoint;
	
	inverseGrid(ip(I,0),ip(I,1),0)=cg.interpoleeGrid[grid](I);
	for( int dir=0; dir<numberOfDimensions; dir++ )
          rI(ip(I,0),ip(I,1),0,dir)=cg.interpolationCoordinates[grid](I,dir);
      }
      else  
      {
        mask(ip(I,0),ip(I,1),ip(I,2))=MappedGrid::ISinterpolationPoint;

	inverseGrid(ip(I,0),ip(I,1),ip(I,2))=cg.interpoleeGrid[grid](I);
	for( int dir=0; dir<numberOfDimensions; dir++ )
          rI(ip(I,0),ip(I,1),ip(I,2),dir)=cg.interpolationCoordinates[grid](I,dir);
      }
      #endif
    }


    for( int grid2=numberOfBaseGrids-1; grid2>=0; grid2-- )
    {
      MappedGrid & g2 = cg[grid2];
      Mapping & map2 = g2.mapping().getMapping();

      if( grid2==grid || (!isNew(grid) && !isNew(grid2)) // skip two old grids
          || !cg.mayInterpolate(grid,grid2,0) ) // *wdh* added 020127
      {
        // skip this case
      }
      else
      {
	if( info & 2 )
	{
	  // Overture::printMemoryUsage(sPrintF("interpolate boundaries begin: grid=%i grid2=%i",grid,grid2));
	}

	const realArray & center2 = g2.center();

	intSerialArray iag[6];   // could *new* to be 2*nd
	realSerialArray rg[6], xg[6];

	if( useBoundaryAdjustment )
	{
	  real time0=getCPU();

          // ******************************************
          // *** adjust boundaries on shared sides ****
          // ******************************************
          //  This function will compute interpolation coordinates for boundary points -> rg
	  updateBoundaryAdjustment(cg,grid,grid2,iag,rg,xg,sidesShare);

	  timeForBoundaryAdjustment+=getCPU()-time0;
	}

	for( int axis=axis1; axis<numberOfDimensions; axis++ )
	{
	  // (ip1,ip2,ip3) : for cell centered option, this is the opposite corner of the face
	  int ip1 = axis==axis1 ? 0 : 1;
	  int ip2 = axis==axis2 ? 0 : 1;
	  int ip3 = axis==axis3 || cg.numberOfDimensions()==2 ? 0 : 1;
      
	  for( int side=Start; side<=End; side++ )
	  {

	    if( info & 2 )
	      Overture::checkMemoryUsage(sPrintF("interpolate boundaries AA: grid=%i grid2=%i",grid,grid2));
            // NOTE: map1.intersects( map2,... ) builds bounding boxes and intialized ApproxGlobalInverse!
	    if( g.boundaryCondition(side,axis) > 0  &&
		map1.intersects( map2, side,axis,-1,-1,.1 ) &&
		map1.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity )
	    {
	      // grid2 intersects with the face of grid 1
	      if( info & 4 || debug & 4 )
		fPrintF(logFile,"\n *** grid %s, (side,axis)=(%i,%i) intersects with grid %s \n",
		       (const char*)g.mapping().getName(Mapping::mappingName),side,axis,
		       (const char*)g2.mapping().getName(Mapping::mappingName));

  	      if( info & 2 ) 
	        Overture::checkMemoryUsage(sPrintF("interpolate boundaries AB: grid=%i grid2=%i",grid,grid2));

              #ifdef USE_PPP
                const realSerialArray & normal = g.vertexBoundaryNormalArray(side,axis);
              #else
                const realSerialArray & normal = g.vertexBoundaryNormal(side,axis);
              #endif

	      getBoundaryIndex(extendedGridIndexRange(g),side,axis,I1,I2,I3);   // ** 980406
	      for( dir=0; dir<g.numberOfDimensions(); dir++ )
	      {
		if( g.isAllVertexCentered() && g.isPeriodic(dir) )
		{
		  if( dir!=axis )
		    Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound()+1);  // include periodic (is this needed?)
		}
		else if( g.isAllCellCentered() && dir!=axis && g.boundaryCondition(End,dir)>0 )
		  Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound()-1);  // 980814 do not include last point on CC grids
      
	      }

              bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);

	      intSerialArray & ia = iag[side+2*axis]; // these arrays were computed in updateBoundaryAdjustment
	      realSerialArray & r = rg[side+2*axis];
	      realSerialArray & x = xg[side+2*axis];
	  
              // printF(" grid=%s, grid2=%s\n",
              //      (const char*)g.mapping().getName(Mapping::mappingName),
              //      (const char*)g2.mapping().getName(Mapping::mappingName));
              // display(sidesShare(side,axis,nullRange,nullRange),"sidesShare");

	      #ifdef USE_PPP
  	        bool checkAnyway=false; // true;  // check for interpolation between the boundaries
              #else
                bool checkAnyway=false;
              #endif

              const bool interpolationOnsharedSidesComputed=max(sidesShare(side,axis,nullRange,nullRange))>0 ;
	      int numberToCheck=0;
	      if( interpolationOnsharedSidesComputed )
	      {
		// In this case the interpolation on boundaries has already been computed
		// by updateBoundaryAdjustment
                if( info & 4 || debug & 4 )
                  fPrintF(logFile," >>>> No need to recompute interpolation on this boundary since it should have been"
                        "done by updateBoundaryAdjustment <<<<\n");
		
		Range R = r.dimension(0);
		numberToCheck=R.getLength();
		if( numberToCheck==0 )
		  continue;
	    
	      }
              // ****** this should be a separate option to allow boundary points to interpolate from the interior 
              // ****** instead of using !mayCutHoles
	      else if( !cg.mayCutHoles(grid,grid2) || checkAnyway )
	      {
		// if there are no shared sides we still need to check for interpolation of the 
		// boundary from the interior (if we do not cut holes)

              // Extend the bounding box by a bit 
		RealArray boundingBox; boundingBox = g2.boundingBox();
		for( dir=0; dir<g.numberOfDimensions(); dir++ )
		{
		  real delta=(boundingBox(End  ,dir)-boundingBox(Start,dir))*.05;
		  boundingBox(Start,dir)-=delta;
		  boundingBox(End  ,dir)+=delta;
		}
		// real epsBB = .1;
		// make a list of the boundary points that we should check: those that
		// lie in the bounding box of the other grid
		int numberOfPoints = I1.length()*I2.length()*I3.length();
		ia.redim(numberOfPoints,3);
	      
		int i=0;
		if( ok )
		{
		  FOR_3D(i1,i2,i3,I1,I2,I3)
		  {
		    if( g.isAllVertexCentered() )
		    {
		      if( !isRectangular )
		      {
			x0=vertex(i1,i2,i3,axis1);
			x1=vertex(i1,i2,i3,axis2);
			if( cg.numberOfDimensions() > 2 )
			  x2=vertex(i1,i2,i3,axis3);
		      }
		      else
		      {
			x0=XV0(i1,i2,i3); 
			x1=XV1(i1,i2,i3);
			if( cg.numberOfDimensions() > 2 )
			  x2=XV2(i1,i2,i3);
		      }
			
		    }
		    else
		    { // In the cell centered case : check the face centered point (** average diagonals ***)
		      // we need to check a point on the boundary since the cell center could
		      // be a long way away from the boundary of the other grid!
		      if( !isRectangular )
		      {
			x0=.5*(vertex(i1,i2,i3,axis1)+vertex(i1+ip1,i2+ip2,i3+ip3,axis1));   
			x1=.5*(vertex(i1,i2,i3,axis2)+vertex(i1+ip1,i2+ip2,i3+ip3,axis2));
			if( cg.numberOfDimensions() > 2 )
			  x2=.5*(vertex(i1,i2,i3,axis3)+vertex(i1+ip1,i2+ip2,i3+ip3,axis3));
		      }
		      else
		      {
			x0=.5*(XV0(i1,i2,i3)+XV0(i1+ip1,i2+ip2,i3+ip3));   
			x1=.5*(XV1(i1,i2,i3)+XV1(i1+ip1,i2+ip2,i3+ip3));
			if( cg.numberOfDimensions() > 2 )
			  x2=.5*(XV2(i1,i2,i3)+XV2(i1+ip1,i2+ip2,i3+ip3));
		      }
		    }
		    if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) &&
			x1 >= boundingBox(Start,axis2) && x1 <= boundingBox(End,axis2) &&
			( cg.numberOfDimensions()==2 || 
			  (x2 >= boundingBox(Start,axis3) && x2 <= boundingBox(End,axis3)) ) )
		    {
		      ia(i,0)=i1;
		      ia(i,1)=i2;
		      ia(i,2)=i3;
		      i++;
		    }
		  }
		} // end if ok 
		numberToCheck=i; 
		if( numberToCheck==0 )
		  continue;               // no points to check on this grid
                
		R=Range(0,numberToCheck-1);
		x.redim(R,Rx);
		r.redim(R,Rx); r=-1.;
		if( g.isAllVertexCentered() )
		{
                  if( !isRectangular )
		  {
		    for( dir=0; dir<g.numberOfDimensions(); dir++ )
		      x(R,dir)=vertex(ia(R,0),ia(R,1),ia(R,2),dir);
		  }
		  else
		  {
                    for( int i=R.getBase(); i<=R.getBound(); i++ )
		    {
		      iv[0]=ia(i,0), iv[1]=ia(i,1), iv[2]=ia(i,2);
		      for( dir=0; dir<g.numberOfDimensions(); dir++ )
			x(i,dir)=XV(iv,dir);
		    }
		  }
		}
		else
		{
                  if( !isRectangular )
		  {
		    for( dir=0; dir<g.numberOfDimensions(); dir++ )
		      for( int i=0; i<numberToCheck; i++ )
			x(i,dir)=.5*(vertex(ia(i,0),ia(i,1),ia(i,2),dir)+vertex(ia(i,0)+ip1,ia(i,1)+ip2,ia(i,2)+ip3,dir));
		  }
		  else
		  {
		    for( int i=0; i<numberToCheck; i++ )
		    {
		      iv[0]=ia(i,0),   iv[1]=ia(i,1),   iv[2]=ia(i,2);
                      jv[0]=iv[0]+ip1, jv[1]=iv[1]+ip2, jv[2]=iv[2]+ip3;
		      for( dir=0; dir<g.numberOfDimensions(); dir++ )
			x(i,dir)=.5*(XV(iv,dir)+XV(jv,dir));
		    }
		  }
		  
		}
		// no need to adjust the boundary here as we are just checking for interp from the interior ??
                #ifdef USE_PPP
  		  map2.inverseMapS(x,r); 
                #else
  		  map2.inverseMap(x,r);
                #endif
	      } // end else if !maycutHoles
	      

              // **********************************************************************
              // **** Now take the list of potential interpolation points in ia,x,r ***
              // ****  and check to see which ones are valid.                       ***
              // **********************************************************************
              // *** NOTE: we double check any interpolation points generated by updateBoundaryAdjustment ****

	      // 100516 note used: real tol = max(g2.sharedBoundaryTolerance()(Range(0,1),Rx))*max(g2.gridSpacing()(Rx));
	      // printF("tol = %e \n",tol);
	      // g2.sharedBoundaryTolerance().display("g2.sharedBoundaryTolerance()");
	      // g2.gridSpacing().display("g2.gridSpacing()");

              real rBound2a[6];
              #define rBound2(side,axis) rBound2a[(side)+2*(axis)]
              for( int dir=0; dir<3; dir++ )
	      {
		rBound2(0,dir)=rBound(0,dir,grid2)-g2.sharedBoundaryTolerance(0,dir)*g2.gridSpacing(dir);
		rBound2(1,dir)=rBound(1,dir,grid2)+g2.sharedBoundaryTolerance(1,dir)*g2.gridSpacing(dir);
	      }

	      // add any valid points to the interpolation point lists
	      for( i=0; i<numberToCheck; i++ )
	      {
		i1=ia(i,0);
		i2=ia(i,1);
		i3=ia(i,2);
		if( g.isAllCellCentered() )
		{
		  if( side==End )
		    iv[axis]--; // take cell-centered point
		}  
		if( inverseGrid(i1,i2,i3)>grid2 ) // *wdh* 011027 : but does this mess up if a grid is changed?
                  continue;                   // this point has already been interpolated from a higher priority grid

		if( debug & 4 )
		{
		  if( cg.numberOfDimensions()==2 )
		    fprintf(plogFile," grid=%i, grid2=%i, (i1,i2)=(%i,%i), x=(%8.1e,%8.1e), r_2=(%5.2f,%5.2f), "
			    "rBound2=(%5.2f,%5.2f)x(%5.2f,%5.2f) \n",grid,grid2,ia(i,0),ia(i,1),x(i,0),x(i,1),
                             r(i,0),r(i,1),rBound2(0,0),rBound2(1,0),rBound2(0,1),rBound2(1,1));
		  else
		  {
		    fprintf(plogFile,"grid=%i, grid2=%i, (i1,i2,i3)=(%i,%i,%i), x=(%7.3e,%7.3e,%7.3e),"
                            " r_2=(%7.3e,%7.3e,%7.3e)\n",
			    grid, grid2,ia(i,0),ia(i,1),ia(i,2), x(i,0),x(i,1),x(i,2),r(i,0),r(i,1),r(i,2));
                    //   char ans;
		    // RealArray xx(1,3), rr(1,3);
		    // for( ;;) 
		    // {
		    //   cout << "Enter a char to continue\n";
		    //   cin >> ans;
		    //   xx(0,Rx)=x(i,Rx);
		    //   map2.inverseMap(xx,rr);
		    //   printF(" rr=(%7.3e,%7.3e,%7.3e) \n",rr(0,0),rr(0,1),rr(0,2));
		    // } 

		  }
		}
		
                // *wdh* 070313 -- allow interpolation from not just the interior so that we can catch
                //      shared sides that have not been marked as such
// 		if( r(i,0)>rBound(Start,0,grid2) && r(i,0)<rBound(End,0,grid2) &&
// 		    r(i,1)>rBound(Start,1,grid2) && r(i,1)<rBound(End,1,grid2) &&
// 		    (numberOfDimensions==2 || (r(i,2)>rBound(Start,2,grid2) && r(i,2)<rBound(End,2,grid2)) ) )
		if( 
                    r(i,0)>=rBound2(Start,0) && r(i,0)<=rBound2(End,0) &&
		    r(i,1)>=rBound2(Start,1) && r(i,1)<=rBound2(End,1) &&
		    (numberOfDimensions==2 || (r(i,2)>=rBound2(Start,2) && r(i,2)<=rBound2(End,2)) ) )
		{
		  bool canInterpolate=true;
		  int normalDirection=-1;
		  // Check to see if we are close to a physical boundary of another grid.
                  if( !interpolationOnsharedSidesComputed )
		  {
		    canInterpolate=false;
		    for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		    {
		      for( int side2=Start; side2<=End; side2++ )
		      {
			if( g2.boundaryCondition(side2,dir)>0 &&
			    map2.getTypeOfCoordinateSingularity(side2,dir)!=Mapping::polarSingularity &&
			    ( fabs(r(i,dir)-side2) < boundaryEps || 
			      ( g.sharedBoundaryFlag(side,axis)!=0 && 
				g.sharedBoundaryFlag(side,axis)==g2.sharedBoundaryFlag(side2,dir) &&
				fabs(r(i,dir)-side2) < g2.sharedBoundaryTolerance(side2,dir)*g2.gridSpacing(dir) )
			      ) )
			{
			  // double check that the normals to the surfaces are both in the same direction

			    // *wdh* 070313 -- always compute normal on map2 instead of using the closest pt
			    // Then we do not need normal2 -- this works in parallel

			  realSerialArray r2(1,3),xr2(1,3,3),n2(3);
			  r2(0,Rx)=r(i,Rx);

#ifdef USE_PPP
			  map2.mapS(r2,Overture::nullRealArray(),xr2);
#else
			  map2.map(r2,Overture::nullRealDistributedArray(),xr2);
#endif

			  const real signForJacobian2=map2.getSignForJacobian();
			  getNormal(cg.numberOfDimensions(),side2,dir,xr2,signForJacobian2, n2);

			  real cosAngle=normal(ia(i,0),ia(i,1),ia(i,2),0)*n2(0)+
			    normal(ia(i,0),ia(i,1),ia(i,2),1)*n2(1);
			  if( cg.numberOfDimensions()==3 ) cosAngle+=normal(ia(i,0),ia(i,1),ia(i,2),2)*n2(2);

// 			  printF(" face=(g,s,a)=(%i,%i,%i) : normal=(%g,%g,%g),   face=(%i,%i,%i) n2=(%g,%g,%g)\n",
// 				 grid,side,axis,
// 				 normal(ia(i,0),ia(i,1),ia(i,2),0),normal(ia(i,0),ia(i,1),ia(i,2),1),
// 				 (numberOfDimensions==3 ? normal(ia(i,0),ia(i,1),ia(i,2),2) : 0.),
// 				 grid2,side2,dir,n2(0),n2(1),n2(2));

			  if( debug & 2 && cosAngle>.3 )
			  {
			    fprintf(plogFile,"interpolateBoundaries:WARNING: a boundary point on grid %s can "
				    "interpolate from the boundary of grid %s,\n"
				    "   but the cosine of the angle between the surface normals is %e (too small).\n"
				    "   No interpolation assumed. r=(%e,%e,%e)\n",
				    (const char*)map1.getName(Mapping::mappingName),
				    (const char*)map2.getName(Mapping::mappingName),cosAngle,
				    r(i,0),r(i,1),(cg.numberOfDimensions()==2 ? 0. : r(i,2)));
			  }


			  if( cosAngle>angleDiff )  // .8 // if cosine of the angle between normals > ?? 
			  {
			    canInterpolate=true; 
			    normalDirection=dir;
			    break;
			  }
			  
			} // if inside
		      } // end for side2
		    }  // end for dir

		  
		    if( canInterpolate )
		    { // tangential directions to the boundary have a stricter tolerance
		      for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		      {
			if( dir!=normalDirection && (r(i,dir) <= rBound2(Start,dir) ||
						     r(i,dir) >= rBound2(End  ,dir)) )
			{
			  canInterpolate=false;
			  break;
			}
		      }
		      if( canInterpolate )
		      {
			// check here that the share flag is set for these boundaries, if not issue some INFO
			int dir=normalDirection;
			for( int side2=Start; side2<=End; side2++ )
			{
			  if( !warnForSharedSides(grid,side+2*axis,grid2,side2+2*dir) )
			  {
			    if( fabs(r(i,dir)-(real)side2) < boundaryEps && 
				( g.sharedBoundaryFlag(side,axis)==0 || 
				  g.sharedBoundaryFlag(side,axis)!=g2.sharedBoundaryFlag(side2,dir) ) )
			    {
			      // We have to avoid a message when interpolating from a corner or when
			      // we interpolate a corner
			      int dirp1= (dir+1) % cg.numberOfDimensions();  // tangential direction
			      int dirp2= cg.numberOfDimensions()>2 ?  (dir+2) % cg.numberOfDimensions() : dirp1;

			      int axisp1= (axis+1) % cg.numberOfDimensions();  // tangential direction
			      int axisp2= cg.numberOfDimensions()>2 ?  (axis+2) % cg.numberOfDimensions() : axisp1;
                            
			      if( fabs(r(i,dirp1)-.5)<.48 && fabs(r(i,dirp2)-.5)<.48 && // *** arbitrary ***** fix
				  ia(i,axisp1)!=g.extendedIndexRange(Start,axisp1) && 
				  ia(i,axisp1)!=g.extendedIndexRange(End  ,axisp1) &&
				  ia(i,axisp2)!=g.extendedIndexRange(Start,axisp2) && 
				  ia(i,axisp2)!=g.extendedIndexRange(End  ,axisp2) )
			      {
				// this could be a manual shared boundary
				bool manualFound=false;
				for( int n=0; n<numberOfManualSharedBoundaries; n++ )
				{
				  if( manualSharedBoundary(n,0)==grid && manualSharedBoundary(n,3)==grid2 &&
				      manualSharedBoundary(n,1)==side && manualSharedBoundary(n,2)==axis &&
				      manualSharedBoundary(n,4)==side2&& manualSharedBoundary(n,5)==dir )
				  {
				    manualFound=true;
				    break;
				  
				  }
				}
				if( manualFound )
				  continue;
				
				warnForSharedSides(grid,side+2*axis,grid2,side2+2*dir)=true;
				warnForSharedSides(grid2,side2+2*dir,grid,side+2*axis)=true;
				for( int msg=0; msg<=1; msg++ )
				{
				// write message to the logfile and to the screen
				  FILE *file = msg==0 ? plogFile : stdout;
				  fprintf(file,"WARNING: boundary (side,axis)=(%i,%i) of grid %s with share=%i "
					  " looks like it should have the share flag to\n   match boundary "
					  "(side,axis)=(%i,%i) of grid %s with share=%i, r=(%6.2e,%6.2e,%6.2e)\n",
					  side,axis, (const char*)g.mapping().getName(Mapping::mappingName),
					  g.sharedBoundaryFlag()(side,axis),
					  side2,dir,(const char*)g2.mapping().getName(Mapping::mappingName),
					  g2.sharedBoundaryFlag()(side2,dir),
					  r(i,0),r(i,1),cg.numberOfDimensions()>2 ? r(i,2) : 0.);
				  fprintf(file," i=%i, ia= (%i,%i,%i), x=(%e,%e) \n",i,ia(i,0),ia(i,1),ia(i,2),
					  x(i,0),x(i,1));
				}
			      }
			    }
			  }
			}
		      }
		    }
		  } // end if( !interpolationOnsharedSidesComputed )

		  
		  // ** if( !canInterpolate && !cg.mayCutHoles(grid2,grid) && !cg.mayCutHoles(grid,grid2) &&
		  if( !canInterpolate &&  !cg.mayCutHoles(grid,grid2) &&
		      max(fabs(r(i,Rx)-.5))<=.5+boundaryEps )
		  { // we are in the interior of a grid that we do not cut holes in
		    canInterpolate=true;
		    isInteriorBoundaryPoint=true;
		  }
		  if( canInterpolate )
		  {
		    
		    // this point can interpolate from the boundary of another grid ***** check the boundary for bc>0
		    // OR we interpolate from the interior of a grid that we do not cut holes in

		    i1=ia(i,0);
		    i2=ia(i,1);
		    i3=ia(i,2);
		    if( g.isAllCellCentered() )
		    {
		      if( side==End )
			iv[axis]--; // take cell-centered point
		      // for a cell-centered grid we must find the coordinates of the cell center, as opposed
		      // to the cell face
                      if( !isRectangular )
		      {
			for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
			  xx(0,dir)=center(i1,i2,i3,dir);
		      }
		      else
		      {
			for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
			  xx(0,dir)=XC(iv,dir);
		      }
		      #ifdef USE_PPP
		        map2.inverseMapS(xx,rr);
                      #else
		        map2.inverseMap(xx,rr);
                      #endif

		      for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
			r(i,dir)=rr(0,dir); 
		    }

		    mask(i1,i2,i3)= MappedGrid::ISinterpolationPoint; //  can interpolate from grid 2

		    if( isInteriorBoundaryPoint )
		    {
		      mask(i1,i2,i3)|=MappedGrid::ISinteriorBoundaryPoint | ISnonCuttingBoundaryPoint; 
		      isInteriorBoundaryPoint=false;
		    }
		    
		    if( debug & 4 )
		      fprintf(plogFile,"***interp pt %i : grid=%i, (i1,i2,i3)=(%i,%i,%i), x=(%7.3e,%7.3e,%7.3e), grid2=%i "
			      "r=(%4.1f,%4.1f,%4.1f)\n",i,grid,i1,i2,i3,x(i,0),x(i,1),
			      (cg.numberOfDimensions()>2 ? x(i,2) : 0.),
			      grid2,r(i,0),r(i,1),
			      (cg.numberOfDimensions()>2 ? r(i,2) : 0. ));


		    inverseGrid(i1,i2,i3)= grid2; //  can interpolate from grid 2
		    for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
		    {
                      // assert( r(i,dir)!=Mapping::bogus );  // TEMP *wdh* 120418 
		      
		      rI(i1,i2,i3,dir)=r(i,dir);  // save coordinates 
		    }
		  }
		}
		else
		{
		  if( debug & 4 )
		  {
		    fprintf(plogFile,"   ... is outside grid2\n");
		  }
		}
	      }
	    }
	  }
	}
      }
    } // end for( int grid2...
    

    // Check for mixed boundaries, a physical/interpolation boundary, such as a c-grid or h-grid
    for( int n=0; n<numberOfMixedBoundaries; n++ )
    {
      if( mixedBoundary(n,0)==grid )
      {
	interpolateMixedBoundary(cg,n);
      }
    }

    if( debug & 2 )
      displayMask(g.mask(),sPrintF(buff,"Mask after marking interpolation on boundaries, grid=%s",
       (const char*)g.getName()),logFile);


  }  // end for grid
  

  //  Allocate and fill in interpolation data.
  cg.numberOfInterpolationPoints=1;
  cg.update(
    CompositeGrid::THEinterpolationPoint       |
    CompositeGrid::THEinterpoleeGrid           |
    CompositeGrid::THEinterpoleeLocation       |
    CompositeGrid::THEinterpolationCoordinates,
    CompositeGrid::COMPUTEnothing);

  real time=getCPU();
  timeInterpolateBoundaries=ParallelUtility::getMaxValue(time-time0);
  real total = ParallelUtility::getMaxValue(time-totalTime);

  if( info & 2 )
  {
    Overture::checkMemoryUsage("interpolate boundaries (end)");
    printF(" time for interpolate boundaries..........................%e (total=%e)\n",
	   timeInterpolateBoundaries,total);
    printF("   includes time for boundary adjustment..................%e (total=%e)\n",
	   timeForBoundaryAdjustment,total);
    if( timePreInterpolate>0. ) 
      printF("   includes time for preInterpolateGrids..................%e (total=%e)\n",
	     timePreInterpolate,total);
  }
  
  checkArrayIDs("Ogen::checkInterpolationOnBoundaries:end");

  return 0;
}


int Ogen::
preInterpolateGrids(CompositeGrid & cg)
// ======================================================================================
//
// /Description:
//    Pre-interpolate grids. If grid2 is basically a refinement of grid1 then one may pre-interpolate
//      grid1 from grid2. grid2 will thus effectively cut a hole in grid1 and remove any points in
//     grid1 that lie in the interior of grid2.
//     This may be necessary if a small feature would otherwise not cut holes properly
//      in grid1 (but does cut holes properly in grid2).
//
// NOTE: this routine is assumed called before interpolate of mixed boundaries; otherwise we would
// have to be more careful not to change mixed boundary interpolation points.
// ======================================================================================
{
#ifdef USE_PPP
  if( debug & 2 )
    printF("Ogen::preInterpolateGrids::Not implemented yet for parallel. Do nothing...\n");
#else

  const int numToPreInterpolate=preInterpolate.getLength(0);
  if( numToPreInterpolate==0 ) return 0;
  
  real time0=getCPU();
  
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  const int numberOfDimensions=cg.numberOfDimensions();
  int axis;
  int iv[3];
  int & i1=iv[0], & i2=iv[1], & i3=iv[2];
  real x0,x1,x2;
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  intArray ia, interpolates, useBackupRules;
  realArray x,r;
  Range R, Rx=numberOfDimensions;

  for( int n=0; n<numToPreInterpolate; n++ )
  {

    int grid1=preInterpolate(n,0);
    int grid2=preInterpolate(n,1);
    assert( grid1>=0 && grid1<numberOfBaseGrids );
    assert( grid2>=0 && grid2<numberOfBaseGrids );
    assert( grid1!=grid2 );
    
    if( true || info & 2 ) printF(" PreInterpolate grid %s from grid %s\n", 
        (const char*)cg[grid1].getName(),(const char*)cg[grid2].getName());
    
    MappedGrid & c = cg[grid1];
    intArray & mask = c.mask();
    intArray & inverseGrid = cg.inverseGrid[grid1];
    realArray & center = c.center();
    realArray & rI = cg.inverseCoordinates[grid1];
   
    // include interpolation boundaries but not physical boundaries since these will
    // be done by the boundary interpolation.
    getIndex(c.extendedIndexRange(),I1,I2,I3);
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      if( c.boundaryCondition(Start,axis)>0 )
	Iv[axis]=Range(Iv[axis].getBase()+1,Iv[axis].getBound());
      if( c.boundaryCondition(End,axis)>0 )
	Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);
    }
    
    const RealArray & boundingBox = cg[grid2].boundingBox();
    
    ia.redim(I1.length()*I2.length()*I3.length(),3);  // *** this is too big

    // **** make a list of points to check ****
    int k=0;
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  if( numberOfDimensions==2 )
	  {
	    x0=center(i1,i2,i3,axis1);
	    x1=center(i1,i2,i3,axis2);
	    if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) &&
		x1 >= boundingBox(Start,axis2) && x1 <= boundingBox(End,axis2) )
	    {
	      ia(k,0)=i1;
	      ia(k,1)=i2;
	      ia(k,2)=i3;
	      k++;
	    }
	  }
	  else if( numberOfDimensions==3 )
	  {
	    x0=center(i1,i2,i3,axis1);
	    x1=center(i1,i2,i3,axis2);
	    x2=center(i1,i2,i3,axis3);
	    if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) &&
		x1 >= boundingBox(Start,axis2) && x1 <= boundingBox(End,axis2) &&
		x2 >= boundingBox(Start,axis3) && x2 <= boundingBox(End,axis3) )
	    {
	      ia(k,0)=i1;
	      ia(k,1)=i2;
	      ia(k,2)=i3;
	      k++;
	    }
	  }
	  else
	  {
	    x0=center(i1,i2,i3,axis1);
	    if( x0 >= boundingBox(Start,axis1) && x0 <= boundingBox(End,axis1) )
	    {
	      ia(k,0)=i1;
	      ia(k,1)=i2;
	      ia(k,2)=i3;
	      k++;
	    }
	  }
	}
      }
    }

    int numberToCheck=k; 
    if( numberToCheck==0 )
    continue;               // no points to check on this grid

    R=Range(0,numberToCheck-1);
    x.redim(R,Rx);
    r.redim(R,Rx); r=-1.;
    for( axis=0; axis<numberOfDimensions; axis++ )
      x(R,axis)=center(ia(R,0),ia(R,1),ia(R,2),axis);

    if( useBoundaryAdjustment )
      adjustBoundary(cg,grid1,grid2,ia(R,Rx),x);    // adjust boundary points on shared sides

    if( info & 2 ) 
      printF("*** preInterpolate: try to interpolate %i points of grid %s from grid %s \n",
	     numberToCheck,(const char *)c.getName(),(const char *)cg[grid2].getName());

    Mapping & map2 = cg[grid2].mapping().getMapping();

    // the dpm inverse requires quite a bit of temporary storage. Therefore we reduce
    // the number of points that we invert at any one time. This seems to be faster too.
    const int maxNumberToCheck=5000;     
    if( numberToCheck<maxNumberToCheck )
    {
      map2.inverseMap(x,r);
    }
    else
    {
      for( int i=0; i<(numberToCheck+maxNumberToCheck-1)/maxNumberToCheck; i++ )
      {
	Range S(i*maxNumberToCheck,min((i+1)*maxNumberToCheck-1,numberToCheck-1));
	// printF("i=%i S=[%i,%i] \n",i,S.getBase(),S.getBound());
	// map2.inverseMap(x(S,Rx),r(S,Rx));

	Range R0=S-S.getBase();                 // work around for A++ bug.
	realArray rr(R0,Rx), xx(R0,Rx);
	xx(R0,Rx)=x(S,Rx);
	rr=-1.;
	map2.inverseMap(xx,rr);
	r(S,Rx)=rr(R0,Rx);
	// display(r,"r","%4.1f ");
      }
    }
	
    interpolates.redim(numberToCheck); interpolates=true;
    useBackupRules.redim(numberToCheck);  useBackupRules=false;

    checkForOneSided=false;
    cg.rcData->canInterpolate(cg.gridNumber(grid1),cg.gridNumber(grid2), r, interpolates, 
			       useBackupRules, checkForOneSided );

    where( interpolates(R) )
    {
      mask(ia(R,0),ia(R,1),ia(R,2))=MappedGrid::ISinterpolationPoint;
      inverseGrid(ia(R,0),ia(R,1),ia(R,2)) = grid2;   // can interpolate from grid2
      for( axis=0; axis<numberOfDimensions; axis++ )
	rI(ia(R,0),ia(R,1),ia(R,2),axis)=r(R,axis);   // save coordinates
    }

    
    // now remove un-needed interpolation points.
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  if (!isNeededForDiscretization(c, iv) )
	  {
	    mask(i1,i2,i3) = 0;   // mark as unused
	  }
	}
      }
    }
    


  }
  
  timePreInterpolate=getCPU()-time0;
#endif  
  return 0;
}



//! Find the closest boundary point -- use x-distance!
// \param x[3], iv[3] input: point to check, and indicies of the nearest grid point
// \param ivb[3],sideb,axisb (output) : boundary point and face.
int Ogen::
findClosestBoundaryPoint( MappedGrid & mg, real *x, int *iv, int *ivb, int & sideb, int & axisb )
{
  Mapping & map = mg.mapping().getMapping();
  const realArray & center = mg.center();

  sideb=axisb=-1;
  
  real minDist=REAL_MAX;
  for( int side=0; side<=1; side++ )
  {
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      if( mg.boundaryCondition(side,axis)>0 )
      {
	if( axis==0 && map.getTypeOfCoordinateSingularity(side,0)==Mapping::polarSingularity )
	  continue;  // skip this boundary since it is probably a singular point

	// project the point onto the boundary and find the distance
	int ivb[3];
	ivb[0]=iv[0], ivb[1]=iv[1], ivb[2]=iv[2];
	ivb[axis]=mg.gridIndexRange(side,axis);
	int dir;
	for( dir=0; dir<mg.numberOfDimensions(); dir++ )
	  ivb[dir]=max(mg.dimension(0,dir),min(mg.dimension(1,dir),ivb[dir]));

	real dist=0.;
	for( dir=0; dir<mg.numberOfDimensions(); dir++ )
	  dist+=fabs(x[dir]-center(ivb[0],ivb[1],ivb[2],dir));

	if( dist<minDist )
	{
	  minDist=dist;
	  sideb=side;
	  axisb=axis;
	}
	      
      }
    }
  }
  if( sideb>=0 ) 
    return 0;
  else
    return 1;
}





int Ogen::
queryAPoint(CompositeGrid & cg) 
// ===========================================================================================
// /Description:
//    Utility routine to allow the user to input a grid and grid point and have some info
//  printed about the point (x position and mask) and to have the point plotted as a black mark.
// ===========================================================================================
{
  assert( ps!=NULL );
  
  GenericGraphicsInterface & gi = *ps;

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  GUIState gui;
  gui.setWindowTitle("Query the Grid");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString pbLabels[] = {"change the plot",
                        "print orphan points",
                        "fix orphan points",
			""};
  int numRows=2;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  aString pickedPointColour="yellow";
  real pickedPointSize=8.;
  
  const int numberOfTextStrings=8;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];
  int nt=0;
  textLabels[nt] = "pt: grid,i1,i2,i3:"; 
  sPrintF(textStrings[nt], "%i %i %i %i",-1,0,0,0); nt++; 
  textLabels[nt] = "pt colour:"; 
  sPrintF(textStrings[nt], "%s",(const char*)pickedPointColour); nt++; 
  textLabels[nt] = "pt size:"; 
  sPrintF(textStrings[nt], "%3.0f pixels",pickedPointSize); nt++; 

  int infoLevel=1;
  textLabels[nt] = "info level:"; 
  sPrintF(textStrings[nt], "%i",infoLevel); nt++; 

  textLabels[nt] = "Mapping::debug:"; 
  sPrintF(textStrings[nt], "%i",Mapping::debug); nt++; 
    // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

  // addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  enum PickingOptionsEnum
  {
    pickToChooseABoundaryPoint,
    pickToChooseAPoint,
    pickingOff
  };
  PickingOptionsEnum pickingOption=pickToChooseAPoint;
  
  aString opLabel1[] = {"choose a boundary point","choose a point","off", ""};
  int numberOfColumns=1;
  dialog.addRadioBox("Picking:", opLabel1,opLabel1,(int)pickingOption,numberOfColumns);


  bool interpolatePickedPoint=false;
  bool checkInterpolationCoords=false;
  bool changePoint=false;
  
  aString tbCommands[] = {"interpolate point",
                          "check interpolation coords",
			  "change the point",
			  ""};

  int tbState[10];
  tbState[0] = interpolatePickedPoint==true;
  tbState[1] = checkInterpolationCoords==true; 
  tbState[2] = changePoint==true; 
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("query>"); // set the default prompt

  SelectionInfo select; select.nSelect=0;

  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  i1=i2=i3=0;
  const int numberOfDimensions=cg.numberOfDimensions();
  Range Rx=numberOfDimensions;

  aString answer;
  int len=0;
  for( ;; )
  {
//     aString menu[] =
//     {
//       "enter values at command prompt",
//       "done",
//       ""
//     };
//    gi.getMenuItem(menu,answer,"Enter a pt: grid,i1,i2,i3");
         
    gi.getAnswer(answer,"", select);

    bool checkPoint=false;
    int grid=-1;
    int sidec=-1, axisc=-1, value;

    if( answer=="done" || answer=="exit" )
      break;
    else if( answer.matches("choose a point") )
    {
      pickingOption=pickToChooseAPoint;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("choose a boundary point") )
    {
      pickingOption=pickToChooseABoundaryPoint;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("off") )
    {
      pickingOption=pickingOff;
      dialog.getRadioBox(1).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer=="change the plot" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);      
      gi.erase();
      PlotIt::plot(gi,cg,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);      
    }
    else if( (len=answer.matches("pt: grid,i1,i2,i3:")) )
    {
      sScanF(answer(len,answer.length()-1),"%i %i %i %i",&grid,&i1,&i2,&i3);
      checkPoint=true;
    }
    else if( (len=answer.matches("pt colour:")) )
    {
      pickedPointColour=answer(len+1,answer.length());
      printF("pickedPointColour=[%s]\n",(const char*)pickedPointColour);
      
    }
    else if( (len=answer.matches("pt size:")) )
    {
      sScanF(answer(len,answer.length()-1),"%e",&pickedPointSize);
      dialog.setTextLabel(2,sPrintF(buff, "%3.0f pixels",pickedPointSize));      
    }
    else if( (len=answer.matches("info level:")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&infoLevel);
      printF("Setting infoLevel=%i\n",infoLevel);
    }
    else if( (len=answer.matches("Mapping::debug:")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&Mapping::debug);
      printF("Setting Mapping::debug=%i\n",Mapping::debug);
    }
    else if( (len=answer.matches("interpolate point")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&value); interpolatePickedPoint=value;
      dialog.setToggleState(0,interpolatePickedPoint);
    }
    else if( (len=answer.matches("check interpolation coords")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&value); checkInterpolationCoords=value;
      dialog.setToggleState(1,checkInterpolationCoords);
    }
    else if( answer=="print orphan points" )
    {
      printOrphanPoints(cg);
    }
    else if( (len=answer.matches("change the point")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&value); changePoint=value;
      dialog.setToggleState(2,changePoint);
    }
    else if( select.nSelect && pickingOption!=pickingOff )  
    {
      printF("\n ========================Selection ===============================\n");

      printF("Picked pt: (%11.5e,%11.5e,%11.5e) : ", select.x[0], select.x[1], select.x[2]);
      grid=-1;
      for( int i=0; i<select.nSelect && grid==-1; i++)
      {
	for( int g=0; g<numberOfBaseGrids; g++ )
	{
	  // printF(" cg[g].getGlobalID()=%i =? selection %i\n",cg[g].getGlobalID(),select.selection(i,0));
	  if( cg[g].getGlobalID()==select.selection(i,0) )
	  {
            grid=g;
	    break;
	  }
	}
      }
      if( grid>=0 )
      {
	printF(" grid %i (%s) was chosen\n",grid,(const char*)cg[grid].getName());
      }
      else
      {
	printF(" no grid was picked (?)\n");
	continue;
      }
      checkPoint=true;

      realArray r(1,3), x(1,3);
      x(0,0)=select.x[0], x(0,1)=select.x[1], x(0,2)=select.x[2];
      MappedGrid & mg=cg[grid];
      
      Mapping & map = mg.mapping().getMapping();
      r=-1.;
      map.inverseMap(x,r);
      
      if( fabs(r(0,0)-.5)>9. )
      {
	printF(" >>>>>>>>>>>>>>inverseMap failed : call inverseMap for grid1=%s with debug info on:\n",
                (const char*)mg.getName());
	map.inverseMap(x,r);
	printF("<<<<<<<<<<<<<<< return from inverseMap\n");

        printF(" ** Since the inverseMap failed, find closest grid point instead.\n");
        #ifndef USE_PPP
    	  map.approximateGlobalInverse->findNearestGridPoint(0,0,x,r );
        #else
	  Overture::abort("ERROR: fix me for parallel");
        #endif
      }
      

      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
        const real shift = r(0,axis)>=0. ? .5 : -.5;
	iv[axis]=int(r(0,axis)/mg.gridSpacing(axis)+shift)+mg.gridIndexRange(0,axis);
      }
      printF(" r=(%9.3e,%9.3e,%9.3e) : closest grid point: iv=(%i,%i,%i)",r(0,0),r(0,1),r(0,2),i1,i2,i3);

      if( pickingOption==pickToChooseABoundaryPoint )
      {
	// find the closest boundary point -- use x-distance!
        int ivb[3];
        findClosestBoundaryPoint( mg, &x(0,0), iv, ivb, sidec, axisc );

	if( sidec>=0 )
	{
	  iv[axisc]=mg.gridIndexRange(sidec,axisc);
          printF(" --> closest boundary point: iv=(%i,%i,%i) face=(%i,%i)\n",i1,i2,i3,sidec,axisc);
	}
        else
	{
          printF(" --> no closest boundary point (?)\n");
	}
      }
      else
        printF("\n");
      
      // further checks on this point are done below...


    }
    else if( answer=="fix orphan points" )
    {
      printF("There are %i orphan points\n",numberOfOrphanPoints);
      
      int numberOfNewOrphanPoints=0;
      RealArray newOrphanPoint=orphanPoint;
      Range all;
      
      for( int n=0; n<numberOfOrphanPoints; n++ )
      {
	int grid=int(fabs(orphanPoint(n,numberOfDimensions)+.5));
        if( grid<0 || grid >= numberOfBaseGrids )
	{
          printF("ERROR: orphanPoint %i has grid=%i which is invalid! Skipping this point...\n",n,grid);
	  continue;
	}
	
	MappedGrid & g = cg[grid];
	Mapping & map = g.mapping().getMapping();
        // invert x to get iv[]
	RealArray x(1,3), r(1,3);
	r=-1.;  
	x(0,Rx)=orphanPoint(n,Rx);
        #ifdef USE_PPP
	  map.inverseMapS(x,r);
        #else
	  map.inverseMap(x,r);
        #endif
        int iv[3]={0,0,0};
	for( int axis=0; axis<numberOfDimensions; axis++ )
	  iv[axis]=int(r(0,axis)/g.gridSpacing(axis)+.5)+g.gridIndexRange(0,axis);

        printF("\n-------------------------------------------------------------------------------------\n"
               "    orphan pt n=%i: iv=(%i,%i,%i) grid %i : %s\n",n,iv[0],iv[1],iv[2],grid,(const char*)g.getName());
	
	
        // try to interpolate from other grids
	bool checkBoundaryPoint=false;
	bool checkInterpolationCoords=false;
        bool interpolatePoint=true;
        bool ok=interpolateAPoint(cg, grid, iv, interpolatePoint, checkInterpolationCoords, checkBoundaryPoint, 
                                infoLevel );
	if( !ok )
	{
	  newOrphanPoint(numberOfNewOrphanPoints,all)=orphanPoint(n,all);
	  numberOfNewOrphanPoints++;
	}
	
	// ask: which grid should I interpolate from?
//         gi.inputString(answer,"Interpolate from which other grid?");
//         int grid2;
// 	sScanF(answer,"%i",&grid2);
	
        // reduce interp width or shift coordinates as required.

      }
      numberOfOrphanPoints=numberOfNewOrphanPoints;
      if( numberOfOrphanPoints>0 )
      {
        printF("***WARNING: NOT all orphan points were interpolated. There remain %i ****\n",numberOfOrphanPoints);
	Range R=numberOfOrphanPoints;
	orphanPoint.redim(R,orphanPoint.dimension(1));
	orphanPoint(R,all)=  newOrphanPoint(R,all);
      }
      else
      {
        printF("***SUCCESS: all orphan points were interpolated ****\n");
	
	orphanPoint.redim(0);
      }
      if( Ogen::debug & 1 )
      {
	intSerialArray *iInterp = new intSerialArray [ numberOfBaseGrids ];  // **** fix this ****
	IntegerArray numberOfInterpolationPoints(numberOfBaseGrids);
	for( int grid=0; grid<numberOfBaseGrids; grid++ )
	{
	  const IntegerArray & extended = cg[grid].extendedRange();
	  int dim=(extended(1,0)-extended(0,0)+1)*(extended(1,1)-extended(0,1)+1)*(extended(1,2)-extended(0,2)+1);
	  iInterp[grid].redim(dim,3);
	}
	generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );  // need iInterp
	delete [] iInterp;
    
	plot( " ",cg,false);  
      }
  
    }
    else
    {
      cout << "Unknown response: " << answer << endl;
      gi.stopReadingCommandFile();
    }
    
	
    if( checkPoint )
    {
      if( grid<0 || grid>=numberOfBaseGrids )
      {
	printF("ERROR: grid should >0 and < %i\n",numberOfBaseGrids);
	continue;
      }

      MappedGrid & c = cg[grid];
      Mapping & map = c.mapping().getMapping();
      c.update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex );
      
      const realArray & vertex = c.vertex();
      const intArray & mask = c.mask();
	  
      const int dw = c.discretizationWidth(0); // discretization width 
      const int hw = (dw-1)/2;  // half width 
	  
      if( i1>=c.dimension(0,0) && i1<=c.dimension(1,0) &&
	  i2>=c.dimension(0,1) && i2<=c.dimension(1,1) &&
	  i3>=c.dimension(0,2) && i3<=c.dimension(1,2) )
      {
        // -- In parallel we print from the processor that owns the point ---
        OV_GET_SERIAL_ARRAY_CONST(real,vertex,vertexLocal);
        OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
	
        #ifdef USE_PPP
         const int proc = mask.Array_Descriptor.findProcNum( iv );  // point this on this processor
        #else
         const int proc = 0;
        #endif

	if( myid==proc )
	{
	  printf("=================================================================================================================================\n");
	  printf(" **** checkPoint: (grid,i1,i2,i3)=(%i,%i,%i,%i) (%s) :  x=(%11.5e,%11.5e,%11.5e), mask=%i decode=%i\n",
		 grid,i1,i2,i3,(const char*)c.getName(),
		 vertexLocal(i1,i2,i3,0),vertexLocal(i1,i2,i3,1),
		 (cg.numberOfDimensions()>2 ? vertexLocal(i1,i2,i3,2) : 0.),
		 maskLocal(i1,i2,i3),decode(maskLocal(i1,i2,i3)));
	  printf(" Here is the mask surrounding this point (grid,i1,i2,i3)=(%i,%i,%i,%i)\n", grid,i1,i2,i3);
	  printf("   mask: 1=interior, 2=ghost, -2,3=interiorBoundaryPoint,  <0 =interp \n");
	  for( int j3=i3-hw; j3<=i3+hw; j3++ )
	  {
	    if( j3<c.dimension(0,2) || j3>c.dimension(1,2) )
	      continue;
	    for( int j2=i2-hw; j2<=i2+hw; j2++ )
	    {
	      if( j2<c.dimension(0,1) || j2>c.dimension(1,1) )
		continue;
	      printf("   mask(%i:%i,%i,%i) =",i1-hw,i1+hw,j2,j3);

	      for( int j1=i1-hw; j1<=i1+hw; j1++ )
	      {
		if( j1>=c.dimension(0,0) && j1<=c.dimension(1,0) )
		{
		  printf(" %3i ",decode(maskLocal(j1,j2,j3)));
		}
	      }
	      printf("\n");
	    }
	  }
	  fflush(0);
	}
	
        // plot the picked point
        const int numberOfDimensions=c.numberOfDimensions();
	const realArray & center = c.center();
        OV_GET_SERIAL_ARRAY_CONST(real,center,centerLocal);
        int axis;

	plot( " ",cg,false);  // first replot
	real pointSize;
	psp.get(GI_POINT_SIZE,pointSize);
        psp.set(GI_POINT_SIZE,pickedPointSize*gi.getLineWidthScaleFactor());      // point size in pixels
	psp.set(GI_POINT_COLOUR,pickedPointColour); 

        RealArray pickedPoint;
	if( myid==proc )
	{
	  pickedPoint.redim(1,3);
	  pickedPoint=0.;
	  for( axis=0; axis<numberOfDimensions; axis++ )
	    pickedPoint(0,axis)=centerLocal(iv[0],iv[1],iv[2],axis);    
	}
	
        gi.plotPoints(pickedPoint,psp);

	psp.set(GI_POINT_SIZE,(real)pointSize);      // reset


        if( interpolatePickedPoint )
	{
	  bool checkBoundaryPoint=false; // we already projected onto the bndry,pickingOption==pickToChooseABoundaryPoint;
	  interpolateAPoint(cg, grid, iv, false, checkInterpolationCoords, checkBoundaryPoint, infoLevel );
	}
	
      }
      else
      {
        printF("===================================================================================================================================\n"
               " **** checkPoint (grid,i1,i2,i3)=(%i,%i,%i) (%s): ****\n",
               grid,i1,i2,i3,(const char*)c.getName());
	printF("   ERROR: (i1,i2,i3) should be in the ranges [%i,%i]x[%i,%i]x[%i,%i]\n",
	       c.dimension(0,0),c.dimension(1,0),c.dimension(0,1),c.dimension(1,1),
	       c.dimension(0,2),c.dimension(1,2));
      }
      printF("====================================================================================================================================\n");
    }
  }

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();
  return 0;
}

namespace
{

int 
plotPoints(GenericGraphicsInterface & gi, const RealArray & points, GraphicsParameters & parameters )
// ==================================================================================================
// /Description:
//    Plot points defined in serial arrays on different processors. 
//    In parallel we need to collect up the points on all processors 
// ==================================================================================================
{

#ifdef USE_PPP

  int numPoints = ParallelUtility::getSum(points.getLength(0));

  // printF("plotPoints: numPoints=%i\n",numPoints);

  int p0=0;  // copy results to this processor
  Partitioning_Type partition;
  partition.SpecifyProcessorRange(Range(p0,p0));

  realArray pts;
  pts.partition(partition);
  pts.redim(numPoints,points.dimension(1));
  realSerialArray ptsLocal; getLocalArrayWithGhostBoundaries(pts,ptsLocal);
    
  Index Iv[2];
  Iv[0]=points.dimension(0);
  Iv[1]=points.dimension(1);
  CopyArray::getAggregateArray( (RealArray &)points, Iv, ptsLocal, p0);  // results go into ptsLocal

  // ::display(pts,"plotPoints: pts");
  
  gi.plotPoints(pts,parameters);

#else
  gi.plotPoints(points,parameters);
#endif
  return 0;
}
 
}

int Ogen::
plot(const aString & title,
     CompositeGrid & cg,
     const int & queryForChanges /* =true */ )
// ==================================================================================
//   Plot the grid and holePoints and orphanPoints.
//
// /queryForChanges (input): if true allow the user to optionally make changes
//   to the way the grid is plotted.
// /Return values: 0=normal return, resetTheGrid=reset and start again
//   
// ==================================================================================
{
  
  GenericGraphicsInterface & gi = *ps;

  // ------------------------------
  // ----- Ogen runtime menu ------
  // ------------------------------

  int plotInterpolationPoints;
  psp.get(GI_PLOT_INTERPOLATION_POINTS,plotInterpolationPoints);
  
  bool plotHolePointsToggle= plotHolePoints.getLength(0)>0 && plotHolePoints(0)>0;
  bool plotOrphanPointsToggle= plotOrphanPoints.getLength(0)>0 && plotOrphanPoints(0)>0;

  // --- Only build the dialog if we query for changes ---
  GUIState dialog;
  bool buildDialog=queryForChanges;
  if( buildDialog )
  {
    dialog.setWindowTitle("Ogen runtime");
    dialog.setExitCommand("continue", "continue");

    aString cmds[] = {"continue",
                      "finish",
		      "change the plot",
		      "reset the grid",
		      "query a point",
		      ""};

    int numberOfPushButtons=7;  // number of entries in cmds
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    aString tbCommands[] = {"plot interpolation points",
			    "plot hole points",
			    "plot orphan points",
                            "plot explicit hole cutters",
			    ""};


    int tbState[10];
    tbState[0] = plotInterpolationPoints;
    tbState[1] = plotHolePointsToggle;
    tbState[2] = plotOrphanPointsToggle;
    tbState[3] = plotExplicitHoleCutters;  

    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

    const int numberOfTextStrings=5;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",debug);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


    // *old* menu
    aString menu[] =
      {
	"!Ogen::plot",
	// "continue",
	// "change the plot",
	// "reset the grid",
	// "finish",
	// "plot interpolation points (toggle)",
	"wire frame",
	"set debug",
	// "query a point",
	"incremental hole sweep",
	">hole points",
	// "plot hole points (toggle)",
	"colour hole points by grid",
	"colour hole points black",
	"toggle hole points by grid",
	"<>orphan points",
	// "plot orphan points (toggle)",
	"colour orphan points by grid",
	"colour orphan points black",
	"toggle orphan points by grid",
	"print orphan points",
	"<display the mask",
	"output grids to a file",
	"abort",
	""
      };
  
    dialog.buildPopup(menu);

    dialog.addInfoLabel("See popup for more options.");

    gi.pushGUI(dialog);
  }
  

  if( plotTitles )
    psp.set(GI_TOP_LABEL,title);      
  else
    psp.set(GI_TOP_LABEL,"");      

  if( cg.numberOfDimensions()==3 && (numberOfHolePoints>0 || numberOfOrphanPoints>0) )
    psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  int returnValue=0;

  aString answer;

  int plotObject=true;
  for(int it=0; ; it++)
  {
    if( it==0 )
    {
      plotObject=true;
    }
    else if( queryForChanges )
    {
      // *old way* gi.getMenuItem(menu,answer,"choose an option");

      gi.getAnswer(answer,"");
      
      if( answer=="change the plot" )
      {
// 	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);      
//         gi.erase();
// 	PlotIt::plot(gi,cg,psp);
// 	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);      
        plotObject=2;
      }
      else if( answer=="reset the grid" )
      {
	returnValue=resetTheGrid;
        break;
      }
      else if( answer=="finish" )
      {
        debug=0;
        if( incrementalHoleSweep>0 ) 
	{
	  incrementalHoleSweep=INT_MAX;  // force the incremental sweep to finish
	}
	
        if( cg.numberOfDimensions()==3 )
          psp.set(GI_PLOT_INTERPOLATION_POINTS,false);
        printF("debug mode is now off\n");
        break;
      }
      else if( answer=="incremental hole sweep" )
      {
	gi.inputString(answer,"Enter number of incremental sweeps (enter 0 for full sweep)");
	sScanF(answer,"%i",&incrementalHoleSweep);
	printF("Setting incrementalHoleSweep=%i \n",incrementalHoleSweep);
      }
      else if( answer=="abort" )
      {
	printF("abort from Ogen\n");
        exit(0);
      }
      else if( answer=="plot interpolation points (toggle)" ) // *old* way
      {
        int plotInterp;
        psp.get(GI_PLOT_INTERPOLATION_POINTS,plotInterp);
        plotInterp=!plotInterp;
        psp.set(GI_PLOT_INTERPOLATION_POINTS,plotInterp);

        plotObject=true;
      }
      else if( dialog.getToggleValue(answer,"plot explicit hole cutters",plotExplicitHoleCutters) )
      {
	plotObject=true;
      }
      else if( dialog.getToggleValue(answer,"plot interpolation points",plotInterpolationPoints) )
      {
        psp.set(GI_PLOT_INTERPOLATION_POINTS,plotInterpolationPoints);
        plotObject=true;
      }
  
      else if( answer=="wire frame" )
      {
        psp.set(GI_PLOT_SHADED_SURFACE_GRIDS,false);
        psp.set(GI_PLOT_BLOCK_BOUNDARIES,true);
	psp.set(GI_PLOT_GRID_LINES,false);

        plotObject=true;
      }
      else if( answer=="plot hole points (toggle)" ) // old way 
      {
	plotHolePoints=-plotHolePoints;
        plotObject=true;
      }
      else if( dialog.getToggleValue(answer,"plot hole points",plotHolePointsToggle) )
      {
	if( plotHolePointsToggle )
          plotHolePoints=abs(plotHolePoints);
	else
          plotHolePoints=-abs(plotHolePoints);
        plotObject=true;
      }
      else if( answer=="colour hole points by grid" )
      {
        where( plotHolePoints>0 )
  	  plotHolePoints=2;
        otherwise()
  	  plotHolePoints=-2;
        plotObject=true;
      }
      else if( answer=="colour hole points black" )
      {
        where( plotHolePoints>0 )
  	  plotHolePoints=1;
        otherwise()
  	  plotHolePoints=-1;
        plotObject=true;
      }
      else if( answer=="plot orphan points (toggle)" ) // *old* way
      {
	plotOrphanPoints=-plotOrphanPoints;
        plotObject=true;
      }
      else if( dialog.getToggleValue(answer,"plot orphan points",plotOrphanPointsToggle) )
      {
	if( plotOrphanPointsToggle )
          plotOrphanPoints=abs(plotOrphanPoints);
	else
          plotOrphanPoints=-abs(plotOrphanPoints);
        plotObject=true;
      }
      else if( answer=="colour orphan points by grid" )
      {
        where( plotOrphanPoints>0 )
  	  plotOrphanPoints=2;
        otherwise()
  	  plotOrphanPoints=-2;
        plotObject=true;
      }
      else if( answer=="colour orphan points black" )
      {
        where( plotOrphanPoints>0 )
  	  plotOrphanPoints=1;
        otherwise()
  	  plotOrphanPoints=-1;
        plotObject=true;
      }
      else if( answer=="print orphan points" )
      {
	printOrphanPoints(cg);
      }
      else if( answer=="toggle hole points by grid" || answer=="toggle orphan points by grid" )
      {
	aString prompt;
        if(answer=="toggle hole points by grid" ) 
	  prompt="toggle hole points";
	else
	  prompt="toggle orphan points"; 

        IntegerArray & points = answer=="toggle hole points by grid" ? plotHolePoints : plotOrphanPoints;
	
        gi.appendToTheDefaultPrompt(prompt+">");

	aString *gridMenu = new aString [numberOfBaseGrids+3];
	gridMenu[numberOfBaseGrids  ]="all";
	gridMenu[numberOfBaseGrids+1]="done";
	gridMenu[numberOfBaseGrids+2]="";

        for( ;; )
	{
	  int grid;
	  for( grid=0; grid<numberOfBaseGrids; grid++ )
	    gridMenu[grid]=cg[grid].getName()+ (points(grid)>0 ? " (on)" : "(off)") ;

	  grid=gi.getMenuItem(gridMenu,answer,prompt);
          if( answer=="done" )
	  {
            break;
	  }
	  else if( answer=="all" )
	  {
            points=-points;
	  }
	  else if( grid>=0 && grid<numberOfBaseGrids )
	  {
            points(grid)=-points(grid);
	  }
	  else
	  {
	    printF("Unknown response: %s \n",(const char*)answer);
	    gi.stopReadingCommandFile();
	  }
	}
        delete [] gridMenu;
        gi.unAppendTheDefaultPrompt();
        plotObject=true;
      }
      else if( answer=="display the mask" )
      {
	int grid;
	for( grid=0; grid<numberOfBaseGrids; grid++ )
	{
	  displayMask(cg[grid].mask(),sPrintF(buff,"mask on grid %i",grid));
	}
	for( grid=0; grid<numberOfBaseGrids; grid++ )
	{
	  int numberOfBackupInterpolationPoints = sum(cg[grid].mask() & CompositeGrid::USESbackupRules);
	  if( true || numberOfBackupInterpolationPoints>0 )
	    printF("There were %i backup interpolation points on grid %i\n",numberOfBackupInterpolationPoints,grid);
	}
      }
      else if( answer=="set debug" )
      {
	gi.inputString(answer,sPrintF(buff,"Enter the value for debug variable (current=%i)",debug));
	if( answer!="" )
	{
	  sscanf(answer,"%i",&debug);
	  info = info | debug;
	  printF(" debug=%i \n",debug);
	}
      }
      else if( dialog.getTextValue(answer,"debug:","%i",debug) )
      {
	info = info | debug;
	printF(" debug=%i, info=%i. \n",debug,info);
      }

      else if( answer=="continue" )
        break;
      else if( answer=="query a point" )
      {
        queryAPoint(cg);
      }
      else if( answer=="output grids to a file" )
      {
	aString gridFileName, gridName;
        gi.inputString(gridFileName,"Enter the name of the file such as `myGrid.hdf'");
        gi.inputString(gridName,"Save the grid under what name in the file");
	saveGridToAFile( cg,gridFileName,gridName );
      }
      else
      {
	printF("Unknown response: %s \n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
    }
    else
      break;

    if( plotObject )
    {
      gi.erase();
      if( plotObject==2 ) psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);      
      PlotIt::plot(gi,cg,psp);
      if( plotObject==2 ) psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);      
      plotObject=false;


      const int maxNumberOfHolePoints=ParallelUtility::getMaxValue(numberOfHolePoints);
      if( maxNumberOfHolePoints>0 )
      {

	psp.set(GI_USE_PLOT_BOUNDS,true);
	Range I=numberOfHolePoints, R(0,cg.numberOfDimensions()-1);

	real pointSize;
	psp.get(GI_POINT_SIZE,pointSize);
	if( holePointSize>0. )
	  psp.set(GI_POINT_SIZE,holePointSize*1.67*gi.getLineWidthScaleFactor());      // point size in pixels
	else
	  psp.get(GI_POINT_SIZE,pointSize);
	psp.set(GI_POINT_COLOUR,"black"); 

 	int plotAllHolePointsBlack=max(abs(plotHolePoints-1))==0;
        plotAllHolePointsBlack=ParallelUtility::getMaxValue(plotAllHolePointsBlack);
	if( plotAllHolePointsBlack )
	{
	  if( numberOfHolePoints > 0 )
            gi.plotPoints(holePoint(I,R),psp);
	  else
	    gi.plotPoints(Overture::nullRealArray(),psp);
	}
	else
	{
	  // we need to sort the hole points by component grid
	  for( int grid=0; grid<numberOfBaseGrids; grid++ )
	  {
	    if( plotHolePoints(grid)>0 )
	    {
	      const int nc=cg.numberOfDimensions();
	      intSerialArray ia;
	      if( numberOfHolePoints>0 )
		ia=(fabs(holePoint(I,nc)-(real)grid) < .5).indexMap();

              int totalNumberOfHolePointsOnThisGrid=ParallelUtility::getSum(ia.getLength(0));
	      if( totalNumberOfHolePointsOnThisGrid==0 ) 
                continue;

	      if( plotHolePoints(grid)==1 )
		psp.set(GI_POINT_COLOUR,"black");
	      else
		psp.set(GI_POINT_COLOUR,
			gi.getColourName((grid % GenericGraphicsInterface::numberOfColourNames)));
	      if( ia.getLength(0)>0 )
	      {
		Range J=ia.getLength(0);
		realSerialArray hole(J,R);
		for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
		  hole(J,axis)=holePoint(ia(J,0),axis);
	    
		gi.plotPoints(hole,psp);
	      }
	      else
	      {
		gi.plotPoints(Overture::nullRealArray(),psp);
	      }
		
	    }
	  }
	}
	
	psp.set(GI_POINT_SIZE,(real)pointSize);      // reset

	// psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	psp.set(GI_USE_PLOT_BOUNDS,false);

      } // end if( maxNumberOfHolePoints>0 ) 

      int totalNumberOfOrphanPoints=ParallelUtility::getSum(numberOfOrphanPoints);
      if( totalNumberOfOrphanPoints>0 )
      {
        printF("plot orphan points: there are %i orphan points\n",totalNumberOfOrphanPoints);
	if( false )
	{
	  for( int i=0; i<numberOfOrphanPoints; i++ )
	  {
	    printf("myid=%i i=%i orphan=(%8.2e,%8.2e,%8.2e)\n",myid,i,
		   orphanPoint(i,0),orphanPoint(i,1),(cg.numberOfDimensions()==2 ? 0. : orphanPoint(i,2)));
	  }
	  fflush(0);
	}
	

	psp.set(GI_USE_PLOT_BOUNDS,true);
	Range I=numberOfOrphanPoints, R(0,cg.numberOfDimensions()-1);

	real pointSize;
	psp.get(GI_POINT_SIZE,pointSize);
	if( cg.numberOfDimensions()==2 )
	  psp.set(GI_POINT_SIZE,real(pointSize+4.));      // point size in pixels
	else
	  psp.set(GI_POINT_SIZE,real(pointSize+5.));      // point size in pixels

        int plotAllOrphanPointsBlack=max(abs(plotOrphanPoints-1))==0;
        plotAllOrphanPointsBlack=ParallelUtility::getMaxValue(plotAllOrphanPointsBlack);
        if( plotAllOrphanPointsBlack )
	{
  	  psp.set(GI_POINT_COLOUR,"black"); 
	  if( numberOfOrphanPoints > 0 )
	  {
	    gi.plotPoints(orphanPoint(I,R),psp);
	  }
	  else
	  {
	    gi.plotPoints(Overture::nullRealArray(),psp);
	  }
	}
        else
	{
	  // we need to sort the orphan points by component grid
	  for( int grid=0; grid<numberOfBaseGrids; grid++ )
	  {
	    if( plotOrphanPoints(grid)>0 )
	    {
	      const int nc=cg.numberOfDimensions();
	      intSerialArray ia;
	      if( numberOfOrphanPoints>0 )
	      {
		ia=(fabs(orphanPoint(I,nc)-(real)grid) < .5).indexMap();
	      }
	      // ia.display(" *** ia ***");
	
              int totalNumberOfOrphanPointsOnThisGrid=ParallelUtility::getSum(ia.getLength(0));
	      if( totalNumberOfOrphanPointsOnThisGrid==0 ) 
                continue;

	      if( plotOrphanPoints(grid)==1 )
	      {
		psp.set(GI_POINT_COLOUR,"black");
	      }
	      else
	      {
		psp.set(GI_POINT_COLOUR,
			gi.getColourName((grid % GenericGraphicsInterface::numberOfColourNames)));
	      }
	      
	      if( ia.getLength(0)>0 )
	      {
		Range J=ia.getLength(0);
		realSerialArray orphan(J,R);
		for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
		  orphan(J,axis)=orphanPoint(ia(J,0),axis);
	    
                // orphan(J,R).display("Ogen:plot: orphan");

		gi.plotPoints(orphan,psp);
	      }
	      else
	      {
		gi.plotPoints(Overture::nullRealArray(),psp);
	      }
	    }
	  }
        }
	
	if( info & 8 ) printF("plot: numberOfOrphanPoints=%i \n",numberOfOrphanPoints);
	// orphanPoint(I,R).display("here are the orphan points");

	psp.set(GI_POINT_SIZE,(real)pointSize);      // reset
	psp.set(GI_USE_PLOT_BOUNDS,false);
      }
      if( plotExplicitHoleCutters && explicitHoleCutter.size()>0 )
      { 
        // -- Plot the explicit hole cutter mappings (if any) ---

	if( debug & 1 )
	  printF("plot mappings that are explicit hole cutters...\n");
	
	psp.set(GI_USE_PLOT_BOUNDS,true);

	for( int hc=0; hc<explicitHoleCutter.size(); hc++ )
	  PlotIt::plot(gi,explicitHoleCutter[hc].holeCutterMapping.getMapping(),(GraphicsParameters&)psp);

	psp.set(GI_USE_PLOT_BOUNDS,false);
      }


      if( cutMapInfo.mappingList.getLength()>0 )
      { // plot mappings that cut holes for embedded boundary grids (if any)
	psp.set(GI_USE_PLOT_BOUNDS,true);

	for( int grid=0; grid<cutMapInfo.mappingList.getLength(); grid++ )
	  PlotIt::plot(gi,cutMapInfo.mappingList[grid].getMapping(),(GraphicsParameters&)psp);

	psp.set(GI_USE_PLOT_BOUNDS,false);
      }

    }
    
  }

  if( buildDialog )
    gi.popGUI();

  return returnValue;
}

int Ogen::
conformToCmpgrd( CompositeGrid & cg )
// =================================================================================================
//  Make changes to the grid to conform to what the CMPGRD grid generator built.
// =================================================================================================
{

  Index I1,I2,I3;

  int grid;
  for( int l=0; l<cg.numberOfMultigridLevels(); l++ )
  {
    CompositeGrid & m = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];
    for( grid=0; grid<m.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = m[grid];
      intArray & mask = c.mask();
      intArray & inverseGrid = m.inverseGrid[grid];
//    RealArray & rC = m.inverseCoordinates[grid];
//    RealArray & rI = inverseCoordinates[grid];

      // make a new list of all the interpolation points
      getIndex(c.extendedIndexRange(),I1,I2,I3); 
      int i1,i2,i3;
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    if( mask(i1,i2,i3) & MappedGrid::ISinterpolationPoint )
	    {                               
//            for( int axis=0; axis<m.numberOfDimensions(); axis++ )
//              rC(axis,i1,i2,i3)=rI(i1,i2,i3,axis);
	      mask(i1,i2,i3) |= inverseGrid(i1,i2,i3);
	    }
	    else if( mask(i1,i2,i3)!=0 )
	    {
	      mask(i1,i2,i3) &= ~MappedGrid::GRIDnumberBits;   // zero out grid number
	      mask(i1,i2,i3) |= grid;                          // set lower order bits to this grid number
	    }
	  }
	}
      }
    }
  }

  return 0;
}


int Ogen::
buildBounds( CompositeGrid & cg )
// ==========================================================================================
// /Description:
//    Build the rBound array and define the boundaryEps parameters
// rBound holds the range for values for valid interpolation, larger than [0,1] if we interp. ghost pts.
// /cg (input):
// =========================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  // boundaryEps : We can still interpolate from a grid provided we are this close (in r) to 0 or 1
  // ***NOTE this must be the same as the on in the CompositeGrid since canInterpolate uses the later.
  //  *** 990328 boundaryEps= FLT_EPSILON==REAL_EPSILON ? 5.*cg.epsilon() : 100.*cg.epsilon();
  // *** 990328: canInterpolate uses 2.cg.epsilon() ***
  boundaryEps= 1.9*cg.epsilon(); 

  // rBound holds the range for values for valid interpolation, larger than [0,1] if we interp. ghost pts.
  rBound.redim(2,3,numberOfBaseGrids);

  int grid;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];

    // rBound holds the range for values for valid interpolation, larger than [0,1] if we interp. ghost pts.
    for( int axis=0; axis<3; axis++ )
    {
      rBound(Start,axis,grid)=   (g.extendedIndexRange(Start,axis)-g.indexRange(Start,axis))*g.gridSpacing(axis) 
                                  -boundaryEps;
      rBound(End  ,axis,grid)=1.+(g.extendedIndexRange(End  ,axis)-g.indexRange(End  ,axis))*g.gridSpacing(axis) 
                                  +boundaryEps;
    }
    if( debug & 4 )
      fprintf(logFile," grid=%i, rBound=[%6.2e,%6.2e]x[%6.2e,%6.2e]\n",grid,
	     rBound(Start,axis1,grid),rBound(End  ,axis1,grid),
	     rBound(Start,axis2,grid),rBound(End  ,axis2,grid));

  }

  return 0;
}




int Ogen::
updateGeometry(CompositeGrid & cg,
               CompositeGrid & cgOld,
	       const bool & movingGrids /* =false */, 
	       const IntegerArray & hasMoved /* = Overture::nullIntArray() */) 
// =======================================================================================================
// /Description:
//   Update the geometry on the component grids. In the moving grid case, share data with non moving grids.
// /cg (input) : grid to update.
// /cgOld (input) : share data with this grid if grids are moving.
// /movingGrids (input) : true if some grids are moving.
// /hasMoved (input) : hasMoved(grid)=true if the component grid has moved.
// /geometryNeedsUpdating (implicit input): if geometryNeedsUpdating(grid)==true then update the geometry.
// =======================================================================================================
{
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  if( numberOfGridsHasChanged || gridScale.getLength(0)<numberOfBaseGrids )
  {
    // compute gridScale: gridScale(grid) = maximum length of the bounding box for a grid
    gridScale.redim(numberOfBaseGrids);
    // warnForSharedSides : true if we have warned about possible shared sides not being marked properly
    warnForSharedSides.redim(numberOfBaseGrids,6,numberOfBaseGrids,6);
    warnForSharedSides=false;
  }

  buildBounds(cg); // compute rBound, boundaryEps

  real time0=getCPU();
  Range Rx(0,cg.numberOfDimensions()-1);
  int grid;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];

    if( geometryNeedsUpdating(grid) || movingGrids )
    {
      if( movingGrids && hasMoved(grid) )
      {
        //  Mark new or moved grids as having invalid geometric data.
	if( computeGeometryForMovingGrids )
	  g.geometryHasChanged(~MappedGrid::THEmask);
      }
      
      if( isNew(grid) )
      {
	g.update(MappedGrid::THEmask, MappedGrid::COMPUTEnothing);
	// mask : 
	//  The lower order bits ( GRIDnumberBits = bits 0..22) conatin the preference for a point:
	//    either the grid we interpolate from or the grid number of the current grid.
	// By default all grids try to interpolate from the first preference:
	g.mask() = MappedGrid::ISdiscretizationPoint;  // **** should use highest priority

	g->computedGeometry |= MappedGrid::THEmask;
      }
      

      // *wdh* 060529 -- only build center/vertex on Cartesian grids if needed
      const bool isRectangular = g.isRectangular();
      int updateList = MappedGrid::THEvertexBoundaryNormal | MappedGrid::THEboundingBox;
      if( !isRectangular )
          updateList |= MappedGrid::THEcenter               |
		        MappedGrid::THEvertex;   // we need this even for cell centred grids

      if( geometryNeedsUpdating(grid) || (movingGrids && hasMoved(grid)) )
      {
	g.update( updateList );
      }
      else
      {
        // share data
	g.update(cgOld[grid],updateList );
      }
      geometryNeedsUpdating(grid)=false;
 
      gridScale(grid)=max(cg[grid].boundingBox()(End,Rx)-cg[grid].boundingBox()(Start,Rx));
    }
  }
  cg->computedGeometry |=    CompositeGrid::THEmask;

  if( debug & 2 ) printF("updateGeometry: time to build vertex, vertexBoundaryNormal = %e\n",getCPU()-time0);
  

  // build the arrays inverseCoordinates, inverseGrid and inverseCondition to be used by the overlap algorithm
  cg.update(CompositeGrid::THEinverseMap, CompositeGrid::COMPUTEnothing);
  // cg[0].vertexDerivative().display("vertex derivative");
  // change the shape (orignally (nd,all,all,all)
  Range all;
//  cg.inverseCoordinates.updateToMatchGrid(cg,all,all,all,cg.numberOfDimensions());
  
  cg.inverseCoordinates=0;
  cg.inverseGrid=-1;
   
  if( (info & 16 && numberOfGridsHasChanged) )
  {
    gridScale.display("Ogen::updateGeometry: Here is gridScale");
    rBound.display("Ogen::updateGeometry: Here is rBound");
  }
  numberOfGridsHasChanged=false;
  
/* ---
  for( grid=0; grid<cg.numberOfBaseGrids; grid++ )
  {
    MappedGrid & mg= cg[grid];
    mg.indexRange().display("indexRange");
    mg.extendedIndexRange().display("extendedIndexRange");
  }
--- */  
/* ---
  for( grid=0; grid<cg.numberOfBaseGrids; grid++ )
  {
    MappedGrid & mg= cg[grid];
    mg->extendedIndexRange=mg.indexRange();
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	if( mg.boundaryCondition()(side,axis)==0  )
	  mg->extendedIndexRange(side,axis)=mg.indexRange()(side,axis)+(2*side-1)*
	    min((mg.discretizationWidth()(axis)-1)/2,mg.numberOfGhostPoints()(side,axis));
	  mg->extendedIndexRange(side,axis)=max(mg.dimension()(Start,axis),min(mg.dimension()(End,axis),
									    mg.extendedIndexRange(side,axis)));
      }
    }
  }
--- */
  return 0;
}




int Ogen::
resetGrid( CompositeGrid & cg )
// =========================================================================================
// /Description:
//    Reset the overlapping grid.
// =========================================================================================
{
  cg.numberOfInterpolationPoints=0;
  cg.update(MappedGrid::THEmask);
  for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
  {
    intArray & mask = cg[grid].mask();
    GET_LOCAL(int,mask,maskLocal);
    maskLocal=MappedGrid::ISdiscretizationPoint;   // should use highest priority *****
  }
  numberOfHolePoints=0;
  numberOfOrphanPoints=0;
  cg.inverseCoordinates=0;
  cg.inverseGrid=-1;

  useAnOldGrid=false;
  isNew=true;

  return 0;
}


//\begin{>ogenUpdateInclude.tex}{\subsubsection{Interactive updateOverlap}}
int Ogen::
updateOverlap( CompositeGrid & cg, MappingInformation & mapInfo )
//===============================================================================================
//  
// /Description:
//   Use this function to interactively create a composite grid.
//
// /mapInfo (input) : a MappingInformation object that contains a list of
//   Mappings that can be used to make the composite grid. NOTE: If mapInfo.graphXInterface==NULL
//   then it will be assumed that mapInfo is to be ignored and that 
//   the input CompositeGrid cg will already have a set of grids in it to use.
//
//
// Here is a description of some of the commands that are available from the
// {\tt updateOverlap} function of {\tt Ogen}. This function is called when you 
// choose ``{\tt generate overlapping grid}'' from the {\tt ogen} program.
// 
// \begin{description}
//   \item[compute overlap] : this will compute the overlapping grid. As the grid is generated various
//     information messages are printed out. Some of these messages may only make sense to the 
//      joker who wrote this code.
//   \item[change parameters] : make changes to parameters. See the next section for details.
//   \item[display intermediate results] : this will toggle a debugging mode. When this mode
//     is on, and you choose {\tt compute overlap} to generate the grid, then the overlapping grid 
//     will be plotted at various stages in its algorithm. The algorithm is described in section
//     (\ref{algorithm}). The program will pause at the end of each stage of the algorithm and
//     allow you to either {\tt continue} or to {\tt change the plot} as described next.
//      Experienced users will be able to see when something goes wrong and hopefully detect the cause.
//   \item[change the plot] : this will cause the grid to be re-plotted. You will be in the grid plotter
//     menu and you can make changes to the style of the plot (toggle grids on and off, plot interpolation
//     points etc.). These changes will be retained when you exit back to the grid generator.
// 
// \end{description}
//
//\end{ogenUpdateInclude.tex}
//===============================================================================================
{
  bool useMapInfo = mapInfo.graphXInterface!=NULL; // *wdh* 090705 

  if( useMapInfo )
    ps = (GenericGraphicsInterface*)mapInfo.graphXInterface;
  else
  {
    ps = Overture::getGraphicsInterface();
    mapInfo.graphXInterface=ps; // *wdh* 090806
  }
  

  GenericGraphicsInterface & gi = *ps;
    
  aString answer,line;

  gi.appendToTheDefaultPrompt("checkOverlap>");

  if( true )
  {
    Overture::checkMemoryUsage("Ogen::updateOverlap (start)");
    
    const int np= max(1,Communication_Manager::numberOfProcessors());
    
    real mem=0., maxMem=0., minMem=0., totalMem=0., aveMem=0., maxMemRecorded=0.;
    mem=Overture::getCurrentMemoryUsage();
    maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
    minMem=ParallelUtility::getMinValue(mem);  // min over all processors
    totalMem=ParallelUtility::getSum(mem);     // sum of all processors
    aveMem=totalMem/np;
    maxMemRecorded=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());

    printF("== checkOverlap:start: np=%i, memory per-proc: [min=%g,ave=%g,max=%g](Mb), max-recorded=%g (Mb), "
           "total=%g (Mb)\n",np,minMem,aveMem,maxMem,maxMemRecorded,totalMem);
  }

  int numberOfOldGrids=0;
  // Choose the mappings that will be used in the overlapping grid

  // Make a menu containing the names of all the Mapping's
  int num=mapInfo.mappingList.getLength();

//    const int maxNumToDisplay=200;  // fix this with cascading.... ***********

//    if( num>maxNumToDisplay )
//    {
//      cout << "Error: there are too many mappings to display in the menu, number = " << num << endl;
//    }
//    num=min(num,maxNumToDisplay);
  
  int numberOfGrids=0;
  int numberOfMultigridLevels=1;

  IntegerArray mapList(num); mapList=-1;
  const int maxMenuItems=num+7;
  aString *mapMenu = new aString[maxMenuItems];
  int mappingListStart=0, mappingListEnd=num-1;
  for( int i=0; i<num; i++ )
    mapMenu[i]=mapInfo.mappingList[i].getName(Mapping::mappingName);
  // add extra menu items
  int extra=num;
  // mapMenu[extra++]=" ";
  mapMenu[extra++]="done choosing mappings";
  mapMenu[extra++]="read in an old grid";
  mapMenu[extra++]="specify number of multigrid levels";
  mapMenu[extra++]="";   // null string terminates the menu
  assert( extra<= maxMenuItems );

  // replace menu with a new cascading menu if there are too many items. (see viewMappings.C)
  gi.buildCascadingMenu( mapMenu,mappingListStart,mappingListEnd );

  bool queryForGrids=true;
  if( queryForGrids )
  {
    // --- Query for grids ---
    GUIState dialog;
    dialog.setWindowTitle("Choose grids");
    dialog.setExitCommand("done", "done");

    aString cmds[] = {"read in an old grid",
		      ""};

    int numberOfPushButtons=1;  // number of entries in cmds
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    bool readRefinementsAsBaseGrids=false;
    aString tbCommands[] = {"read refinements as base grids",
			    ""};
    int tbState[10];
    tbState[0] = readRefinementsAsBaseGrids;
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);


    // int numberOfDimensions=0;

    dialog.buildPopup(mapMenu);

    dialog.addInfoLabel("Choose grids from the popup menu");

    gi.pushGUI(dialog);


    useAnOldGrid=false;

    if( useMapInfo )
    {
      // prompt for a set of Mappings or grids to use in the composite grid.

      for( int i=0; i<max(1,num);  )
      {

	// *new way* *wdh* 2012/03/05 
	gi.getAnswer(answer,"");
	int map=-1;
	for( int j=0; j<num; j++ )
	{
	  if( answer==mapInfo.mappingList[j].getName(Mapping::mappingName) )
	  {
	    map=j;
	    break;
	  }
	}
      
//       int map = gi.getMenuItem(mapMenu,answer,
// 			       sPrintF(buff,"Choose a mapping for grid %i (in order of increasing priority)",i));

//       gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

	if( map>=0 && map<num )
	{
	  if( numberOfGrids>0 && min(abs(mapList(Range(0,numberOfGrids-1))-map))==0 )
	  {
	    printF("ERROR:This mapping has already been chosen!\n");
	    continue;
	  }
	  MappingRC & mapping = mapInfo.mappingList[map];
	  if( mapping.getDomainDimension()!=mapping.getRangeDimension() )
	  {
	    printF("ERROR:This mapping does not have domainDimension==rangeDimension \n");
	    continue;
	  }
	  mapList(i++)=map;
	  numberOfGrids=i;
	}
	else if( answer=="done choosing mappings" || answer=="done" )
	{
	  break;
	}
	else if( dialog.getToggleValue(answer,"read refinements as base grids",readRefinementsAsBaseGrids) )
	{
	  if( readRefinementsAsBaseGrids )
	  {
	    printF("readRefinementsAsBaseGrids is true: when reading grids from a file, refinement grids will be\n"
		   "  added a base grids.\n");
	  }
	  else
	  {
	    printF("readRefinementsAsBaseGrids is false: when reading grids from a file, refinement grids will NOT be\n"
		   "  added a base grids (and will thus be ignored when generating the grid).\n");
	  }
	}
	else if( answer=="read in an old grid" )
	{
	  printF(" To read a grid from a show file, the file name should end in `.show'\n");
      
	  gi.inputString(answer,"Enter the name of the old overlapping grid (or show file)");
	  if( answer!="" )
	  {
	    const int len=answer.length();
	    if( answer(len-5,len-1)==".show" )
	    {
	      // *** this is a show file ****

	      ShowFileReader showFileReader(answer);

	      int numberOfFrames=showFileReader.getNumberOfFrames();
	      int numberOfSolutions = max(1,numberOfFrames);
	      int solutionNumber=numberOfSolutions;  // use last

	      gi.inputString(answer,sPrintF(buff,"Enter the solution to use, from 1 to %i (-1=use last)",
					    numberOfSolutions));

	      if( answer!="" )
	      {
		sScanF(answer,"%i",&solutionNumber);
		if( solutionNumber<0 || solutionNumber>numberOfSolutions )
		{
		  solutionNumber=numberOfSolutions;
		}
	      }
	      if( !readRefinementsAsBaseGrids )
	      {
		// refinement grids is the file will remain refinement grids (and thus ignored by ogen)
		showFileReader.getAGrid(cg,solutionNumber);     
	      }
	      else
	      {
		CompositeGrid cgSF;
		// CompositeGrid & cgSF = *new CompositeGrid;
		showFileReader.getAGrid(cgSF,solutionNumber); 
		for( int grid=0; grid<cgSF.numberOfComponentGrids(); grid++ )
		{
		  printF("Add grid %i [%s]\n",grid,(const char*)cgSF[grid].getName());
		
		  cg.add(cgSF[grid]);

		  printF("cg[grid].isRefinementGrid()=%i\n",(int)cg[grid].isRefinementGrid());
		}
		// cg.update(MappedGrid::THEmask);

		if( cg.interpolationPoint.getLength()!=cg.numberOfComponentGrids() )
		{
		  printF("read an old grid:INFO: building the interpolationPoint array etc...\n");
		  // Sometimes a failed grid does not have the interpolationPoint arrays built
		  cg.update(
		    CompositeGrid::THEinterpolationPoint       |
		    CompositeGrid::THEinterpoleeGrid           |
		    CompositeGrid::THEinterpoleeLocation       |
		    CompositeGrid::THEinterpolationCoordinates,
		    CompositeGrid::COMPUTEnothing);
		}

		// for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
		// {
		// }
	      

	      }
	    
	      useAnOldGrid=true;
	      numberOfOldGrids=cg.numberOfBaseGrids();

	    }
	    else
	    {
	      getFromADataBase(cg,answer);
	      useAnOldGrid=true;
	      numberOfOldGrids=cg.numberOfBaseGrids();

	      if( cg.interpolationPoint.getLength()!=cg.numberOfComponentGrids() )
	      {
		printF("read an old grid:INFO: building the interpolationPoint array etc...\n");
		// Sometimes a failed grid does not have the interpolationPoint arrays built
		cg.update(
		  CompositeGrid::THEinterpolationPoint       |
		  CompositeGrid::THEinterpoleeGrid           |
		  CompositeGrid::THEinterpoleeLocation       |
		  CompositeGrid::THEinterpolationCoordinates,
		  CompositeGrid::COMPUTEnothing);
	      }
	    }
#ifdef USE_PPP
	    cg.displayDistribution("Ogen::updateOverlap: cg read from a file");
#endif
	    break;

	  }
	}
	else if( answer=="specify number of multigrid levels" )
	{

	  gi.inputString(answer,"Enter the number of multigrid levels");
	  if( answer!="" )
	  {
	    sScanF(answer,"%i",&numberOfMultigridLevels);
	    if( numberOfMultigridLevels<=0 )
	    {
	      printF("ERROR: numberOfMultigridLevels should be > 0, setting to 1\n");
	      numberOfMultigridLevels=1;
	    }
	  }
	}
	else
	{
	  cout << "Unknown response=" << answer << endl;
	  gi.stopReadingCommandFile();
	}
      }
  

      // ==============================================
      // build a composite grid from the mappings chosen
      // ==============================================

      info=3;  // turn on informational messages
  
      real time0=getCPU();
  
      if( numberOfGrids>0 )
      {
	buildACompositeGrid(cg,mapInfo,mapList,numberOfMultigridLevels,useAnOldGrid);
	if( info & 2 ) printF("time for buildACompositeGrid = %e\n",getCPU()-time0);
      }
//   printF("cg.numberOfComponentGrids()=%i, cg.numberOfBaseGrids()=%i\n",cg.numberOfComponentGrids(), 
//                   cg.numberOfBaseGrids());
  
      geometryNeedsUpdating.redim(cg.numberOfBaseGrids()); 
      geometryNeedsUpdating=true; // true if the geometry needs to be updated after changes in parameters
      numberOfGridsHasChanged=true;

      isNew.redim(cg.numberOfBaseGrids());  // this grid is in the list of new grids (for the incremental algorithm)
      isNew=true;
      isNew(Range(0,numberOfOldGrids-1))=false;
  
      plotHolePoints.redim(cg.numberOfBaseGrids());
      plotHolePoints=1;
  

      if( useAnOldGrid )
      {
	resetGrid(cg); // *wdh* 2012/03/05
	updateParameters(cg);
      }
    

      if( info & 2 )
	printF(" time to initialize the grid..............................%e\n",getCPU()-time0);

      if( info & 4 )
	printF("cg.numberOfGrids= %i, cg.numberOfCompositeGrids()=%i \n",cg.numberOfGrids(),cg.numberOfComponentGrids());

    } // end if useMapInfo
    else
    {
      // *wdh* 090806: add this for when we are called in cg:moveGrids
      geometryNeedsUpdating.redim(cg.numberOfBaseGrids()); 
      geometryNeedsUpdating=true; // true if the geometry needs to be updated after changes in parameters
      numberOfGridsHasChanged=true;

      isNew.redim(cg.numberOfBaseGrids());  // this grid is in the list of new grids (for the incremental algorithm)
      isNew=true;
      isNew(Range(0,numberOfOldGrids-1))=false;
  
      plotHolePoints.redim(cg.numberOfBaseGrids());
      plotHolePoints=1;
    }

    gi.popGUI(); // restore the previous GUI
    
  } // end query for grids 
  
  // -- set some default plotting options --

  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  psp.set(GI_PLOT_SHADED_SURFACE_GRIDS,true);
  psp.set(GI_PLOT_LINES_ON_GRID_BOUNDARIES,true);
  psp.set(GI_PLOT_BLOCK_BOUNDARIES,cg.numberOfDimensions()!=3);
  psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);
  if( cg.numberOfDimensions()==3 )
    psp.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByGrid); // colour boundaries by grid number in 3d

  if( useAnOldGrid )
    psp.set(GI_PLOT_INTERPOLATION_POINTS,true);



  // ---------------------------
  // ----- Main Ogen menu ------
  // ---------------------------

  GUIState dialog;
  dialog.setWindowTitle("Ogen");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"compute overlap",
                    "change parameters",
		    "reset grid",
		    "query a point",
		    "print grid statistics",
                    "change the plot",
		    "plot",
                    "help",
		    ""};

  int numberOfPushButtons=7;  // number of entries in cmds
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  bool displayIntermediateResults=false;
  int plotInterpolationPoints;
  psp.get(GI_PLOT_INTERPOLATION_POINTS,plotInterpolationPoints);

  aString tbCommands[] = {"display intermediate results",
                          "plot explicit hole cutters",
                          "plot interpolation points",
			  ""};
  int tbState[10];
  tbState[0] = displayIntermediateResults;
  tbState[1] = plotExplicitHoleCutters;
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=5;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",debug);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  // *old* popup
  aString menu[] = 
    {
      "!   Ogen   ",
      //  "compute overlap",
      // "change parameters",
      // "display intermediate results",
      "add grids",
      "remove a grid",
      // "reset grid",
      // "query a point",
      "build a cutout",
      // "plot",
      // "print grid statistics",
      ">options",
        "project ghost points on shared sides",
        "allow hanging interpolation",
        "do not allow hanging interpolation",
        "plot titles",
        "do not plot titles",
        "make adjustments for nearby boundaries",
        "do not make adjustments for nearby boundaries",
        "set point size for plotting holes",
      "<>debug options",
      // "set debug parameter",
        "output inverse statistics",
        "check mappings",
        "check interpolation on boundaries",
        "check interpolation for a grid",
        "cut holes with physical boundaries",
        "remove exterior points",
        "cut holes and remove exterior points",
        "find true boundary",
        "display the mask",
        "display computed geometry",
        "double check interpolation",
      // "<change the plot",
      "<plot parallel distribution",
      ">plot bounds",
        "set plot bounds",
        "use default plot bounds",
      "<open graphics",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "compute overlap",
      "change parameters  : change grid parameters", 
      "display intermediate results  : display intermediate results for debugging"
      "add grids : add a new grid to the current overlapping grid",
      "remove a grid",
      "reset grid         : reset the grid to the original state",
      "project ghost points on shared sides: adjust ghost points on DataPointMapping's to match a shared boundary",
      "allow hanging interpolation : for building an incomplete grid",
      "do not allow hanging interpolation",
      "build a cutout     : build a cutout (`embedded boundary grid') *IN DEVELOPMENT*",
      "print grid statistics : output info about the grid",
//      "build hybrid grid  : build a hybrid unstructured-structured grid *IN DEVELOPMENT*",
      "check interpolation on boundaries: find all points on boundaries that can interpolate",
      "check interpolation for a grid : ",
      "cut holes with physical boundaries",
      "remove exterior points",
      "find true boundary",
      "change the plot    : plot the grid (and change plotting parameters)",
      "plot parallel distribution : plot grid points coloured by processor number",
      "set plot bounds            : specify fixed bounds for plotting. Useful for movies.",
      "use default plot bounds    : let plotStuff determine the plotting bounds",
      "open graphics      : open a graphics window if one is not already open",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  dialog.buildPopup(menu);

  dialog.addInfoLabel("See popup menu for more options.");

  gi.pushGUI(dialog);


  numberOfOrphanPoints=0;
  numberOfHolePoints=0;

  bool plotObject= gi.graphicsIsOn() && !gi.readingFromCommandFile(); // true;


  bool firstTimeToComputeOverlap=true;
  bool overlapComputed=false;
  bool gridHasBeenPlotted=false;
  answer="";
  for(int it=0;; it++)
  {
    // Plot the grid by default if graphics is on and we are not reading from a command file.
    // Note that plotting the grid requires a computation of the grid points.
    if( !gridHasBeenPlotted && gi.graphicsIsOn() && !gi.readingFromCommandFile() )  
      answer="plot";
    else
    {
      if( !gridHasBeenPlotted ) gi.outputString(">>>Choose `plot' to plot the grid.");
      // gi.getMenuItem(menu,answer,"choose an option");
      // *new way* *wdh* 2012/03/19 
      gi.getAnswer(answer,"");
    }
    
    if( answer=="compute overlap" )
    {
      if( !firstTimeToComputeOverlap )
      {
        resetGrid(cg);
      }
      firstTimeToComputeOverlap=false;
      
      setGridParameters(cg);  // set parameters for multigrid levels
      
      totalTime=getCPU(); // starting value for totalTime

      int numberOfErrors = 0;
      // Compute multigrid levels -- starting from the coarsest grid
      // debug=1;
      if( info & 4 )
        printF("cg.numberOfMultigridLevels()=%i \n",cg.numberOfMultigridLevels());
      // cg[0].vertex().display(sPrintF(buff,"level=%i, cg[0][0].vertex",0));
      for( int l=cg.numberOfMultigridLevels()-1; l>=0; l-- )
      {
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  maskRatio[axis]= (int)pow(2,l);  

	CompositeGrid & m = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];

	geometryNeedsUpdating=true;
	numberOfGridsHasChanged=true;

        real time0=getCPU();
	updateGeometry(m,m);               // build some geometry arrays used by the overlap algorithm
	timeUpdateGeometry=getCPU()-time0;

	if( info & 2 )
	{
	  Overture::checkMemoryUsage("Ogen::update the geometry");
	  printF(" time to update the geometry..............................%e \n",
		 timeUpdateGeometry);
	}
	
	numberOfHolePoints=0;
	numberOfOrphanPoints=0;

	// m[0].vertex().display(sPrintF(buff,"level=%i, m[0].vertex",l));
        if( info & 4 )
	  printF("Compute the grid for multigrid level =%i \n",l);


        // ****************************************************
        // ********** Generate the grid ***********************
        // ****************************************************
	numberOfErrors = computeOverlap( cg,cg,l );


        if( numberOfErrors==(-resetTheGrid) )
	{
	  resetGrid(cg);
	  continue;
	}
	else if( numberOfErrors > 0  )
	{
          if( cg.numberOfMultigridLevels()>1 )
  	    printF(" ===== overlap computation failed for mulitgrid level = %i. Try requesting fewer levels \n",l);
          else
  	    printF(" ===== overlap computation failed ======\n");
          break;
	}
        else if( numberOfErrors < 0  )
	{
  	  printF(" ===== The overlap computation completed but there were non-fatal errors. ======\n"
  	         " It could be that backup rules were used for some points. These will appear as black\n"
                 " marks if the grid is being plotted. The resulting grid may not give good results \n"
                 " when used to solve a PDE. You can also plot the grid and turn on backup interpolation points\n"
                 " to see any questionable points. *Check* the ogen.log file for further info. \n");
          numberOfErrors=0;
	}
      }
      
      // add interpolation data from the coarse grids into the "base" CompositeGrid
      if( numberOfErrors ==0 && cg.numberOfMultigridLevels()>1 )
      {
        int l;
        for( l=0; l<cg.numberOfMultigridLevels(); l++ )
	{
	  CompositeGrid & m = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];
	  // fill-in numberOfInterpolationPoints from multigrid-level data back into the base CompositeGrid.
	  for( int g=0; g<m.numberOfBaseGrids(); g++ )
	  {
            if( debug & 2 )
  	      printF("l=%i: m.componentGridNumber(%i) = %i, m.gridNumber(%i)=%i, m.baseGridNumber(%i)=%i \n",
		     l,g, m.componentGridNumber(g), g,m.gridNumber(g),g,m.baseGridNumber(g));
             
	    int grid = m.gridNumber(g);
	    cg.numberOfInterpolationPoints(grid)=m.numberOfInterpolationPoints(g);
	  }
	}
        // display(cg.numberOfInterpolationPoints,"cg.numberOfInterpolationPoints");
	// cg.numberOfInterpolationPoints.display("cg.numberOfInterpolationPoints");

	// now we know how many interpolation points there are so we can create the arrays in the cg.
//	const int theLists  =
//	  (cg.numberOfRefinementLevels() == 1 ? CompositeGrid::NOTHING : CompositeGrid::THErefinementLevel) |
//	  (cg.numberOfMultigridLevels()  == 1 ? CompositeGrid::NOTHING : CompositeGrid::THEmultigridLevel );

	cg.update(
	  CompositeGrid::THEinterpolationPoint       |
	  CompositeGrid::THEinterpoleeGrid           |
	  CompositeGrid::THEinterpoleeLocation       |
	  CompositeGrid::THEinterpolationCoordinates ,
	  CompositeGrid::COMPUTEnothing);

	// fill-in  multigrid-level data back into the base CompositeGrid.
	for( l=0; l<cg.numberOfMultigridLevels(); l++ )
	{
	  CompositeGrid & m = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];
	  for( int g=0; g<m.numberOfBaseGrids(); g++ )
	  {
	    int grid = m.gridNumber(g);
	    cg.interpoleeGrid[grid]            =m.interpoleeGrid[g];
	    cg.variableInterpolationWidth[grid]=m.variableInterpolationWidth[g];
	    cg.interpolationPoint[grid]        =m.interpolationPoint[g];
	    cg.interpoleeLocation[grid]        =m.interpoleeLocation[g];
	    cg.interpolationCoordinates[grid]  =m.interpolationCoordinates[g];

            // printF(" m.interpolationPoint.getLength() = %i \n",m.interpolationPoint.getLength());
            // display(m.interpolationPoint[g],sPrintF(buff," l=%i, g=%i, interpolationPoint ",l,g));
	  }
	}
  
	//  Tell the CompositeGrid that the interpolation data have been computed:
	cg->computedGeometry |=
	  CompositeGrid::THEmask                     |
	  CompositeGrid::THEinterpolationCoordinates |
	  CompositeGrid::THEinterpolationPoint       |
	  CompositeGrid::THEinterpoleeLocation       |
	  CompositeGrid::THEinterpoleeGrid           |
          CompositeGrid::THEmultigridLevel;  // *wdh*

      }

      if( numberOfErrors==0 )
      {
	determineMinimalIndexRange(cg);
      }

      plotInterpolationPoints=cg.numberOfDimensions()==2;
      psp.set(GI_PLOT_INTERPOLATION_POINTS,plotInterpolationPoints);
      dialog.setToggleState("plot interpolation points",plotInterpolationPoints);
      
      overlapComputed=true;

      plotObject=gi.graphicsIsOn() && !gi.readingFromCommandFile(); 

    }
    else if( answer=="project ghost points on shared sides" )
    {
      // adjust the ghost points on data point mappings to lie on any shared
      // surfaces -- normally the ghost points are extrapolated and may not
      // match correctly on shared boundaries (the error can be especially large
      // for highly stretched grids).

      // ** projectGhostPoints(cg);
      
    }
    else if( answer=="check mappings" )
    {
      for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
      {
	if( cg[grid].mapping().mapPointer==NULL )
	{
	  cout << "ERROR: This grid has no mappings! \n";
	  break;
	}
	cg[grid].mapping().checkMapping();
      }
    }
    else if( answer=="check interpolation on boundaries" )
    {
      updateGeometry(cg,cg);
      checkInterpolationOnBoundaries(cg);

      numberOfOrphanPoints= checkForOrphanPointsOnBoundaries(cg);
      printF("number of orphan points on the surface =%i \n",numberOfOrphanPoints);

      psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
      plotObject=true;
    }
    else if( answer=="set debug parameter" || answer=="debug" ) // *old way*
    {
      gi.inputString(line,sPrintF("Enter the value for debug variable (current=%i)",debug));
      if( line!="" )
      {
	sscanf(line,"%i",&debug);
        info = info | debug;
	printF(" debug=%i \n",debug);
      }
      continue;
    }
    else if( dialog.getTextValue(answer,"debug:","%i",debug) )
    {
      info = info | debug;
      printF(" debug=%i, info=%i. \n",debug,info);
    }
    else if( answer=="output inverse statistics" )
    {
      ApproximateGlobalInverse::printStatistics();
    }
    else if( answer=="query a point" )
    {
      queryAPoint(cg);
    }
    else if( answer=="check interpolation for a grid" )
    {
      gi.outputString("this is not implemented yet");
    }
    else if( answer=="cut holes with physical boundaries" )
    {
      updateGeometry(cg,cg);
      numberOfHolePoints=cutHoles(cg);
      printF("numberOfHolePoints = %i \n",numberOfHolePoints);
    }
    else if( answer=="remove exterior points" )
    {
      updateGeometry(cg,cg);
      numberOfHolePoints=removeExteriorPoints(cg);
      printF("numberOfHolePoints = %i \n",numberOfHolePoints);
/* ----
      if( false )
      {
	for( int i=0; i<numberOfHolePoints; i++ )
	  printF("hole point %i : x=(%e,%e,%e) \n",i,holePoint(i,0),holePoint(i,1),
             cg.numberOfDimensions()==2 ? 0. : holePoint(i,2));
      }
--- */
    }
    else if( answer=="cut holes and remove exterior points" )
    {
      updateGeometry(cg,cg);
      numberOfHolePoints=cutHoles(cg);
      printF("numberOfHolePoints = %i \n",numberOfHolePoints);

      numberOfHolePoints=removeExteriorPoints(cg,true);
      printF("numberOfHolePoints = %i \n",numberOfHolePoints);
/* ----
      if( false )
      {
	for( int i=0; i<numberOfHolePoints; i++ )
	  printF("hole point %i : x=(%e,%e,%e) \n",i,holePoint(i,0),holePoint(i,1),
             cg.numberOfDimensions()==2 ? 0. : holePoint(i,2));
      }
------ */
    }
    else if( answer=="find true boundary" )
    {
      updateGeometry(cg,cg);
      checkInterpolationOnBoundaries(cg);

      numberOfHolePoints=cutHoles(cg);
      printF("numberOfHolePoints = %i \n",numberOfHolePoints);

      findTrueBoundary(cg);
      psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
      plotObject=true;
    }
    else if( answer=="display computed geometry" )
    {
      for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
	cg[grid].displayComputedGeometry();
    }
    else if( answer=="double check interpolation" )
    {
      doubleCheckInterpolation=true;
      printF("I will double check the interpolation points in checkOverlappingGrid (parallel version)\n");
    }
    else if( answer=="display the mask" )
    {
      int grid;
      for( grid=0; grid<cg.numberOfBaseGrids(); grid++ )
      {
	displayMask(cg[grid].mask(),sPrintF(buff,"mask on grid %i",grid));
      }
      for( grid=0; grid<cg.numberOfBaseGrids(); grid++ )
      {
	int numberOfBackupInterpolationPoints = sum(cg[grid].mask() & CompositeGrid::USESbackupRules);
	if( true || numberOfBackupInterpolationPoints>0 )
	  printF("There were %i backup interpolation points on grid %i\n",numberOfBackupInterpolationPoints,grid);
      }
    }
    else if( answer=="change parameters" )
    {
      changeParameters( cg, &mapInfo );
      continue;
    }
    else if( answer=="add grids" )
    {
      int i=0;
      numberOfOldGrids=cg.numberOfBaseGrids();
      mapList=-1;
      for( ;; )
      {
	int map = gi.getMenuItem(mapMenu,answer,sPrintF(buff,"Choose a mapping to add"));
	if( map>=0 && map<num )
	{
	  mapList(i)=map;
          i++;
	}
	else 
	{
          break;
	}
      }
      if( i>0 )
      {

	useAnOldGrid=true;
        buildACompositeGrid(cg,mapInfo,mapList,numberOfMultigridLevels,useAnOldGrid);

	geometryNeedsUpdating.redim(cg.numberOfBaseGrids()); 
	geometryNeedsUpdating=true; // true if the geometry needs to be updated after changes in parameters
	numberOfGridsHasChanged=true;

	isNew.redim(cg.numberOfBaseGrids());  // this grid is in the list of new grids (for the incremental algorithm)
	isNew=true;
	isNew(Range(0,numberOfOldGrids-1))=false;


      }
    }
    else if( answer=="remove a grid" )
    {
      // **** fix this ****
      int map0 = gi.getMenuItem(mapMenu,answer,sPrintF(buff,"Choose a mapping to remove"));
      int map=-1;
      for( int i=0; i<cg.numberOfBaseGrids()-1; i++ )
      {
        if( mapList(i)==map0 )
	{
	  map=i;
          break;
	}
      }
      if( map>=0 && map<num )
      {
	for( int i=map; i<cg.numberOfBaseGrids()-1; i++ )
          mapList(i)=mapList(i+1);
        mapList(cg.numberOfBaseGrids()-1)=-1;

        buildACompositeGrid(cg,mapInfo,mapList);
        numberOfGridsHasChanged=true;
      }
      else
        printF("ERROR: unable to remove this grid, it is not in the list\n");
    }
    else if( answer=="print grid statistics" )
    {
      GridStatistics::printGridStatistics(cg);
    }
    else if( answer=="reset grid" )
    {
      resetGrid(cg);
/* ----      
      cg.numberOfInterpolationPoints=0;
      for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
      {
	cg[grid].mask()=MappedGrid::ISdiscretizationPoint;   // should use highest priority *****
      }
      numberOfHolePoints=0;
      numberOfOrphanPoints=0;
      cg.inverseCoordinates=0;
      cg.inverseGrid=-1;

      useAnOldGrid=false;
      isNew=true;
----- */      
      plotObject=true;
    }
    else if( answer=="allow hanging interpolation" )
    {
      allowHangingInterpolation=true;
      continue;
    }
    else if( answer=="do not allow hanging interpolation" )
    {
      allowHangingInterpolation=false;
      continue;
    }
    else if( answer=="plot titles" )
    {
      plotTitles=true;
    }
    else if( answer=="do not plot titles" )
    {
      plotTitles=false;
    }
    else if( answer=="make adjustments for nearby boundaries" )
    {
      makeAdjustmentsForNearbyBoundaries=true;
    }
    else if( answer=="do not make adjustments for nearby boundaries" )
    {
      makeAdjustmentsForNearbyBoundaries=false;
    }
    else if( answer=="display intermediate results" ) // *old* way
    {
      // toggle the debug mode
      if( debug & 1 )
      {
        debug=0;
        printF("debug mode is off\n");
      }
      else
      {
        debug=1;
        printF("debug mode is on, choose `compute overlap' to see intermediate results \n");
      }
      continue;
    }
    else if( dialog.getToggleValue(answer,"plot interpolation points",plotInterpolationPoints) )
    {
      psp.set(GI_PLOT_INTERPOLATION_POINTS,plotInterpolationPoints);
    }
    else if( dialog.getToggleValue(answer,"plot explicit hole cutters",plotExplicitHoleCutters) ){} // 
    else if( dialog.getToggleValue(answer,"display intermediate results",displayIntermediateResults) )
    {
      if( displayIntermediateResults )
      {

	printF("displayIntermediateResults is on: display results at sub-stages in the grid generation algorithm.\n");
        printF("Choose `compute overlap' to see intermediate results.\n");
	debug=1;
	
      }
      else
      {
	printF("displayIntermediateResults is off.\n");
	debug=0;
      }
      dialog.setTextLabel("debug:",sPrintF(buff,"%i",debug));
    }

    else if( answer=="build a cutout" )
    {
      cutMapInfo.graphXInterface=&gi;
      // int numberOfCutMappings=0;
      
      int map = gi.getMenuItem(mapMenu,answer,sPrintF(buff,"Choose a mapping to cut a hole."));
      if( map>=0 && map<num )
      {
        cutMapInfo.mappingList.addElement(mapInfo.mappingList[map]);
        buildCutout(cg,cutMapInfo);
      }
    }
    else if( answer=="build hybrid grid" )
    {
      // buildHybridGrid=true;
    }
    else if( answer=="set plot bounds" )
    {
      RealArray xBound(2,3);
      xBound=0.;
      xBound(1,Range(0,2))=1.;
      if( cg.numberOfDimensions()==2 )
	gi.inputString(line,sPrintF(buff,"Enter bounds xa,xb, ya,yb "));
      else
	gi.inputString(line,sPrintF(buff,"Enter bounds xa,xb, ya,yb, za,zb "));
      if( line!="" )
	sScanF(line,"%e %e %e %e %e %e",&xBound(0,0),&xBound(1,0),&xBound(0,1),&xBound(1,1),
	       &xBound(0,2),&xBound(1,2));
	
      gi.resetGlobalBound(gi.getCurrentWindow());
      gi.setGlobalBound(xBound);
	
      psp.set(GI_PLOT_BOUNDS,xBound); // set plot bounds
      psp.set(GI_USE_PLOT_BOUNDS,true);  // use the region defined by the plot bounds
    }
    else if( answer=="use default plot bounds" )
    {
      psp.set(GI_USE_PLOT_BOUNDS,false);  // use the region defined by the plot bounds
    }
    else if( answer=="check mappings with grid" )
    {
      for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
      {
	if( cg[grid].mapping().mapPointer==NULL )
	{
	  cout << "ERROR: This grid has no mappings! \n";
	  break;
	}
	cg[grid].mapping().checkMapping();
      }
    }
    else if( answer=="plot" )
    {
      plotObject=true;
    }
    else if( answer=="change the plot" )
    {
      gi.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);      
      PlotIt::plot(gi,cg,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);      
      plotObject=true;
    }
    else if( answer=="plot parallel distribution" )
    {
      gi.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);      
      psp.set(GI_TOP_LABEL,"Parallel distribution");
      PlotIt::plotParallelGridDistribution(cg,gi,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);      
    }
    else if( answer=="open graphics" )
    {
      if( !gi.graphicsIsOn() )
        gi.createWindow("ogen: Overlapping Grid Generator");
    }
    else if( answer=="set point size for plotting holes" )
    {
      gi.inputString(line,"Enter the point size for plotting holes");
      sScanF(line,"%e",&holePointSize);
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( answer(0,3)=="done" )  // the "done choosing mappings" is optional in the first menu so catch it here
      continue;                     // if the user gives it in a command file
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
      plotObject=true;
    }

    if( plotObject )
    {
      plot( " ",cg,false);

      gridHasBeenPlotted=true;
    }
  }
  
  if( !overlapComputed )
  {
    printF("Ogen::updateOverlap:WARNING: No overlapping grid was computed. You probably should have\n"
	   "chosen the menu item `compute overlap' before exiting\n");
  }
  else
  {
    // change the mask to conform to the CMPGRD grid generator
    // *wdh* 030831  conformToCmpgrd( cg );
  }
  
  if( debug & 4 )
  {
    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }
  
  printF("***Size of composite grid=%7.2f (Mb)\n",cg.sizeOf()/SQR(1024.));

  isNew.redim(0);
  
  delete [] mapMenu;
  gi.erase();
  gi.unAppendTheDefaultPrompt();

  gi.popGUI(); // restore the previous GUI

  if( !useMapInfo )
    mapInfo.graphXInterface=NULL; // reset

  return 0;
  
}


