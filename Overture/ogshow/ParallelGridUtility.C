// This file automatically generated from ParallelGridUtility.bC with bpp.
#include "ParallelGridUtility.h"
#include "ParallelUtility.h"

#ifndef OV_USE_DOUBLE
#define MPI_Real MPI_FLOAT
#else
#define MPI_Real MPI_DOUBLE
#endif


// ==============================================================================
//  Define redistribute functions for GridCollection's and CompositeGrid's
// ==============================================================================


// REDISTRIBUTE(GridCollection,realGridCollectionFunction)
void ParallelGridUtility::
redistribute(const GridCollection & gc, 
                          GridCollection & gcP,
                          const Range & Processors )
// ==================================================================================
// /Description:
// resdistribute a grid to live on a new set of processors
// /gc (input): grid to redistribute
// /gcP (output) : redistributed grid. NOTE: no geometry data is created, you must do this
//    yourself with a call to gcP.update(...)
// /Processors : range of processors to distribute to
// ==================================================================================
{
    gcP.setNumberOfDimensionsAndGrids(gc.numberOfDimensions(),gc.numberOfComponentGrids());
    gcP.rcData->numberOfComponentGrids=gc.numberOfComponentGrids();
    if( true )
    {
    // ** Here is the new way ** wdh 010120 
    // we need to make sure we copy all the GridCollection specific info too (like AMR stuff)
//      for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
//      {
//        gcP[grid].specifyProcesses(Processors);  // This grid only lives on these processors
//      }
        gcP.specifyProcesses(Processors);  // This grid only lives on these processors
    // tell the GridCollection to use it's own parallel distribution:
        gcP.keepGridDistributionOnCopy(true);
        gcP=gc;
    // reset 
        gcP.keepGridDistributionOnCopy(false);
    }
    else
    {
        for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
        {
            const MappedGrid & mg = gc[grid];
            if( TRUE )
            {
      	MappedGrid mgP;
      	mgP.specifyProcesses(Processors);  // This grid only lives on these processors
	//      mgP.rcData->partition.SpecifyProcessorRange(Processors);
      	mgP=mg;                              // deep copy -- effective but inefficient
	//   mgP.mask().display("here is mgP.mask()");
      	gcP[grid].reference(mgP);
            }
            else
            {
	// This  used to work but no more
      	Mapping *map = mg.mapping().mapPointer;
      	MappedGrid mgP(*map); 
	// ***************************** things could be missing here! ******************
      	Range all;
      	mgP->isAllCellCentered =mg.isAllCellCentered() ;
      	mgP->isAllVertexCentered=mg.isAllVertexCentered();
      	mgP->gridSpacing=mg.gridSpacing();
      	mgP->isCellCentered=mg.isCellCentered();
      	mgP->discretizationWidth=mg.discretizationWidth();
      	mgP->isPeriodic=mg.isPeriodic();
	// mgP->minimumEdgeLength=mg.minimumEdgeLength();
	// mgP->maximumEdgeLength=mg.maximumEdgeLength();
      	mgP->boundaryCondition=mg.boundaryCondition();
      	mgP->boundaryDiscretizationWidth=mg.boundaryDiscretizationWidth();
      	mgP->sharedBoundaryFlag=mg.sharedBoundaryFlag();
      	mgP->sharedBoundaryTolerance=mg.sharedBoundaryTolerance();
      	mgP->gridIndexRange(all,all)=mg.gridIndexRange()(all,all);
      	mgP->indexRange(all,all)=mg.indexRange()(all,all);
      	mgP->numberOfGhostPoints=mg.numberOfGhostPoints();
      	mgP->discretizationWidth=mg.discretizationWidth();
      	mgP->dimension(all,all)=mg.dimension()(all,all);
      	mgP.updateReferences();
	//  mg.dimension().display("redistribute: Here is mg.dimension()");
	// mgP.dimension().display("redistribute: Here is mgP.dimension()");
      	mgP.specifyProcesses(Processors);
	// mgP.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask);
      	mgP.update(MappedGrid::THEmask);
      	mgP=0;
      	mgP.mask().display("redistribute: Here is mask before mgP.mask()=mg.mask()");
      	mgP.mask()=mg.mask();
      	mgP.mask().display("redistribute: Here is mask after");
      	mgP.updateReferences();
      	gcP[grid].reference(mgP);
            }
      // gcP[grid].dimension().display("redistribute: Here is gcP[grid].dimension()");
        }
        gcP.updateReferences();
    }
}
void ParallelGridUtility::
redistribute(const realGridCollectionFunction & u, 
                          GridCollection & gcP,
                          realGridCollectionFunction & v, 
                          const Range & Processors )
// ==================================================================================
// /Description:
// resdistribute a grid and grid function to live on a new set of processors
// /u (input): grid function to redistribute (holding grid to redistribute)
// /gcP (output) : redistributed grid. NOTE: no geometry data is created, you must do this
//    yourself with a call to gcP.update(...)
// /v (output) : redistributed version of u
// /Processors : range of processors to distribute to
// ==================================================================================
{
//   #If "GridCollection" == "GridCollection"
        GridCollection & gc = *u.getGridCollection();
    redistribute( gc,gcP,Processors );
    Range all;
    Range R[8] = { all,all,all,all,all,all,all,all };
    int component;
    for( component=0; component<5; component++ )
        R[u.positionOfComponent(component)]= u.getComponentDimension(component)>0 ? 
                                      Range(u.getComponentBase(component),u.getComponentBound(component))
                                    : all;
    v.updateToMatchGrid(gcP,R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
    MappedGrid & mg = gcP[0];
  // mg.dimension().display("Here is gcP[0].dimension()");
  // v[0].display("Here is v[0]");
  // u[0].display("Here is u[0]");
  // v.dataCopy(u); // *wdh* 050327 -- need to change this after we changed dataCopy
    for( int grid=0; grid<gcP.numberOfComponentGrids(); grid++ )
    {
    // this involved communication:
        realArray & vv = v[grid];
        const realArray & uu = u[grid];
        vv=uu;
    }
    v.setName(u.getName());
    for( component=0; component<u.getComponentDimension(0); component++ )
        v.setName(u.getName(component),component);
}


// REDISTRIBUTE(CompositeGrid,realCompositeGridFunction)
void ParallelGridUtility::
redistribute(const CompositeGrid & gc, 
                          CompositeGrid & gcP,
                          const Range & Processors )
// ==================================================================================
// /Description:
// resdistribute a grid to live on a new set of processors
// /gc (input): grid to redistribute
// /gcP (output) : redistributed grid. NOTE: no geometry data is created, you must do this
//    yourself with a call to gcP.update(...)
// /Processors : range of processors to distribute to
// ==================================================================================
{
    gcP.setNumberOfDimensionsAndGrids(gc.numberOfDimensions(),gc.numberOfComponentGrids());
    gcP.rcData->numberOfComponentGrids=gc.numberOfComponentGrids();
    if( true )
    {
    // ** Here is the new way ** wdh 010120 
    // we need to make sure we copy all the GridCollection specific info too (like AMR stuff)
//      for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
//      {
//        gcP[grid].specifyProcesses(Processors);  // This grid only lives on these processors
//      }
        gcP.specifyProcesses(Processors);  // This grid only lives on these processors
    // tell the GridCollection to use it's own parallel distribution:
        gcP.keepGridDistributionOnCopy(true);
        gcP=gc;
    // reset 
        gcP.keepGridDistributionOnCopy(false);
    }
    else
    {
        for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
        {
            const MappedGrid & mg = gc[grid];
            if( TRUE )
            {
      	MappedGrid mgP;
      	mgP.specifyProcesses(Processors);  // This grid only lives on these processors
	//      mgP.rcData->partition.SpecifyProcessorRange(Processors);
      	mgP=mg;                              // deep copy -- effective but inefficient
	//   mgP.mask().display("here is mgP.mask()");
      	gcP[grid].reference(mgP);
            }
            else
            {
	// This  used to work but no more
      	Mapping *map = mg.mapping().mapPointer;
      	MappedGrid mgP(*map); 
	// ***************************** things could be missing here! ******************
      	Range all;
      	mgP->isAllCellCentered =mg.isAllCellCentered() ;
      	mgP->isAllVertexCentered=mg.isAllVertexCentered();
      	mgP->gridSpacing=mg.gridSpacing();
      	mgP->isCellCentered=mg.isCellCentered();
      	mgP->discretizationWidth=mg.discretizationWidth();
      	mgP->isPeriodic=mg.isPeriodic();
	// mgP->minimumEdgeLength=mg.minimumEdgeLength();
	// mgP->maximumEdgeLength=mg.maximumEdgeLength();
      	mgP->boundaryCondition=mg.boundaryCondition();
      	mgP->boundaryDiscretizationWidth=mg.boundaryDiscretizationWidth();
      	mgP->sharedBoundaryFlag=mg.sharedBoundaryFlag();
      	mgP->sharedBoundaryTolerance=mg.sharedBoundaryTolerance();
      	mgP->gridIndexRange(all,all)=mg.gridIndexRange()(all,all);
      	mgP->indexRange(all,all)=mg.indexRange()(all,all);
      	mgP->numberOfGhostPoints=mg.numberOfGhostPoints();
      	mgP->discretizationWidth=mg.discretizationWidth();
      	mgP->dimension(all,all)=mg.dimension()(all,all);
      	mgP.updateReferences();
	//  mg.dimension().display("redistribute: Here is mg.dimension()");
	// mgP.dimension().display("redistribute: Here is mgP.dimension()");
      	mgP.specifyProcesses(Processors);
	// mgP.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask);
      	mgP.update(MappedGrid::THEmask);
      	mgP=0;
      	mgP.mask().display("redistribute: Here is mask before mgP.mask()=mg.mask()");
      	mgP.mask()=mg.mask();
      	mgP.mask().display("redistribute: Here is mask after");
      	mgP.updateReferences();
      	gcP[grid].reference(mgP);
            }
      // gcP[grid].dimension().display("redistribute: Here is gcP[grid].dimension()");
        }
        gcP.updateReferences();
    }
}
void ParallelGridUtility::
redistribute(const realCompositeGridFunction & u, 
                          CompositeGrid & gcP,
                          realCompositeGridFunction & v, 
                          const Range & Processors )
// ==================================================================================
// /Description:
// resdistribute a grid and grid function to live on a new set of processors
// /u (input): grid function to redistribute (holding grid to redistribute)
// /gcP (output) : redistributed grid. NOTE: no geometry data is created, you must do this
//    yourself with a call to gcP.update(...)
// /v (output) : redistributed version of u
// /Processors : range of processors to distribute to
// ==================================================================================
{
//   #If "CompositeGrid" == "GridCollection"
//   #Else
        CompositeGrid & gc = *u.getCompositeGrid();
    redistribute( gc,gcP,Processors );
    Range all;
    Range R[8] = { all,all,all,all,all,all,all,all };
    int component;
    for( component=0; component<5; component++ )
        R[u.positionOfComponent(component)]= u.getComponentDimension(component)>0 ? 
                                      Range(u.getComponentBase(component),u.getComponentBound(component))
                                    : all;
    v.updateToMatchGrid(gcP,R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
    MappedGrid & mg = gcP[0];
  // mg.dimension().display("Here is gcP[0].dimension()");
  // v[0].display("Here is v[0]");
  // u[0].display("Here is u[0]");
  // v.dataCopy(u); // *wdh* 050327 -- need to change this after we changed dataCopy
    for( int grid=0; grid<gcP.numberOfComponentGrids(); grid++ )
    {
    // this involved communication:
        realArray & vv = v[grid];
        const realArray & uu = u[grid];
        vv=uu;
    }
    v.setName(u.getName());
    for( component=0; component<u.getComponentDimension(0); component++ )
        v.setName(u.getName(component),component);
}



// =============================================================================================================
// /Description:
//    Compute a new indexRange, dimension
//             and boundaryCondition array that will be valid for the local grid on a processor.
// 
//    Set the indexRange to match the ends of the local array (does NOT include parallel ghost).
//    Set the bc(side,axis) to -1 for internal boundaries between processors
//
// NOTES: In parallel we cannot assume the rsxy array is defined on all ghost points -- it will not
// be set on the extra ghost points put at the far ends of the array. -- i.e. internal boundary ghost 
// points will be set but not external
// =============================================================================================================
void ParallelGridUtility::
getLocalIndexBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                					  IntegerArray & indexRangeLocal, 
                                					  IntegerArray & dimensionLocal, 
                                					  IntegerArray & bcLocal,
                                					  int internalGhostBC /*= -1*/) // 101102 kkc added internalGhostBC
{

    MappedGrid & mg = *a.getMappedGrid();
    
    const IntegerArray & dimension = mg.dimension();
    const IntegerArray & indexRange = mg.indexRange();
    const IntegerArray & bc = mg.boundaryCondition();
    
    indexRangeLocal = indexRange;
    bcLocal = bc;
    dimensionLocal=dimension;
    
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
//      printf(" axis=%i indexRangeLocal(0,axis)=%i a.getLocalBase(axis)=%i  dimension(0,axis)=%i\n",axis,indexRangeLocal(0,axis),
//                        a.getLocalBase(axis),dimension(0,axis));
//      printf(" axis=%i indexRangeLocal(1,axis)=%i a.getLocalBound(axis)=%i dimension(0,axis)=%i\n",axis,indexRangeLocal(1,axis),
//                        a.getLocalBound(axis),dimension(1,axis));
        if( a.getLocalBase(axis) == a.getBase(axis) ) 
        {
            assert( dimension(0,axis)==a.getLocalBase(axis) );
            indexRangeLocal(0,axis) = indexRange(0,axis); 
            dimensionLocal(0,axis) = dimension(0,axis); 
        }
        else
        {
            indexRangeLocal(0,axis) = a.getLocalBase(axis)+a.getGhostBoundaryWidth(axis);
            dimensionLocal(0,axis) = a.getLocalBase(axis); 
      // for internal ghost mark as periodic since these behave in the same was as periodic
      // ** we cannot mark as "0" since the mask may be non-zero at these points and assignBC will 
      // access points out of bounds
            bcLocal(0,axis) = internalGhostBC; //kkc  -1; // bc(0,axis)>=0 ? 0 : -1;
        }
        
        if( a.getLocalBound(axis) == a.getBound(axis) ) 
        {
            assert( dimension(1,axis) == a.getLocalBound(axis) );
            
            indexRangeLocal(1,axis) = indexRange(1,axis); 
            dimensionLocal(1,axis) = dimension(1,axis); 
        }
        else
        {
            indexRangeLocal(1,axis) = a.getLocalBound(axis)-a.getGhostBoundaryWidth(axis);
            dimensionLocal(1,axis) = a.getLocalBound(axis);
      // for internal ghost mark as periodic since these behave in the same was as periodic
            bcLocal(1,axis) = internalGhostBC; //kkc -1; // bc(1,axis)>=0 ? 0 : -1;
        }
        
    }
}

void ParallelGridUtility::
getLocalBoundaryConditions( const realMappedGridFunction & a, 
                      			    IntegerArray & bcLocal )
// ======================================================================================================
// /Description:
//    Compute a boundaryCondition array that will be valid for the local grid on a processor.
// 
//    Set the bc(side,axis) to -1 for internal boundaries between processors
// ======================================================================================================
{

    MappedGrid & mg = *a.getMappedGrid();
    
    const IntegerArray & bc = mg.boundaryCondition();
    bcLocal = bc;
    
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        if( a.getLocalBase(axis) == a.getBase(axis) ) 
        {
        }
        else
        {
      // for internal ghost mark as periodic since these behave in the same was as periodic
            bcLocal(0,axis) = -1; // bc(0,axis)>=0 ? 0 : -1;
        }
        
        if( a.getLocalBound(axis) == a.getBound(axis) ) 
        {
        }
        else
        {
      // for internal ghost mark as periodic since these behave in the same was as periodic
            bcLocal(1,axis) = -1; // bc(1,axis)>=0 ? 0 : -1;
        }
        
    }
}




void ParallelGridUtility::
sortLocalInterpolationPoints( CompositeGrid & cg )
// ============================================================================================
///  \brief  Sort the local interpolation points by donor grid and build the interpolationStartEndIndex
///
// ============================================================================================
{
    const int numberOfComponentGrids=cg.numberOfComponentGrids();
    const int numberOfDimensions=cg.numberOfDimensions();

//   interpolationStartEndIndex.redim(4,numberOfComponentGrids,numberOfComponentGrids);
//   interpolationStartEndIndex = -1;

    IntegerArray & interpolationStartEndIndex = cg.interpolationStartEndIndex;
    interpolationStartEndIndex=-1;

    IntegerArray gridStart(numberOfComponentGrids), ng(numberOfComponentGrids);
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
        const int nig=cg->numberOfInterpolationPointsLocal(grid);

        if( nig > 0 )
        {
            intSerialArray & interpoleeGrid             = cg->interpoleeGridLocal[grid]; 
            intSerialArray & interpolationPoint         = cg->interpolationPointLocal[grid];
            intSerialArray & interpoleeLocation         = cg->interpoleeLocationLocal[grid];
            intSerialArray & variableInterpolationWidth = cg->variableInterpolationWidthLocal[grid];
            realSerialArray & interpolationCoordinates  = cg->interpolationCoordinatesLocal[grid];


            int *interpoleeLocationp = interpoleeLocation.Array_Descriptor.Array_View_Pointer1;
            const int interpoleeLocationDim0=interpoleeLocation.getRawDataSize(0);
#define INTERPOLEELOCATION(i0,i1) interpoleeLocationp[i0+interpoleeLocationDim0*(i1)]
        	  
            int *interpolationPointp = interpolationPoint.Array_Descriptor.Array_View_Pointer1;
            const int interpolationPointDim0=interpolationPoint.getRawDataSize(0);
#define INTERPOLATIONPOINT(i0,i1) interpolationPointp[i0+interpolationPointDim0*(i1)]
        	  
            real *interpolationCoordinatesp = interpolationCoordinates.Array_Descriptor.Array_View_Pointer1;
            const int interpolationCoordinatesDim0=interpolationCoordinates.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES(i0,i1) interpolationCoordinatesp[i0+interpolationCoordinatesDim0*(i1)]
        	  
            int * interpoleeGridp = interpoleeGrid.Array_Descriptor.Array_View_Pointer0;
#define INTERPOLEEGRID(i0) interpoleeGridp[i0]

            int * variableInterpolationWidthp = variableInterpolationWidth.Array_Descriptor.Array_View_Pointer0;
#define VARIABLEINTERPOLATIONWIDTH(i0) variableInterpolationWidthp[i0]

        	  
      // temp arrays to hold sorted arrays:
            intSerialArray interpoleeGrid1(nig);
            intSerialArray interpolationPoint1(nig,numberOfDimensions);
            intSerialArray interpoleeLocation1(nig, numberOfDimensions);
            intSerialArray variableInterpolationWidth1(nig);
            realSerialArray interpolationCoordinates1(nig,numberOfDimensions);

            int *interpoleeLocation1p = interpoleeLocation1.Array_Descriptor.Array_View_Pointer1;
            const int interpoleeLocation1Dim0=interpoleeLocation1.getRawDataSize(0);
#define INTERPOLEELOCATION1(i0,i1) interpoleeLocation1p[i0+interpoleeLocation1Dim0*(i1)]
        	  
            int *interpolationPoint1p = interpolationPoint1.Array_Descriptor.Array_View_Pointer1;
            const int interpolationPoint1Dim0=interpolationPoint1.getRawDataSize(0);
#define INTERPOLATIONPOINT1(i0,i1) interpolationPoint1p[i0+interpolationPoint1Dim0*(i1)]
        	  
            real *interpolationCoordinates1p = interpolationCoordinates1.Array_Descriptor.Array_View_Pointer1;
            const int interpolationCoordinates1Dim0=interpolationCoordinates1.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES1(i0,i1) interpolationCoordinates1p[i0+interpolationCoordinates1Dim0*(i1)]
        	  
            int * interpoleeGrid1p = interpoleeGrid1.Array_Descriptor.Array_View_Pointer0;
#define INTERPOLEEGRID1(i0) interpoleeGrid1p[i0]

            int * variableInterpolationWidth1p = variableInterpolationWidth1.Array_Descriptor.Array_View_Pointer0;
#define VARIABLEINTERPOLATIONWIDTH1(i0) variableInterpolationWidth1p[i0]

            int * ngp = ng.Array_Descriptor.Array_View_Pointer0;
#define NG(i0) ngp[i0]

            int * gridStartp = gridStart.Array_Descriptor.Array_View_Pointer0;
#define GRIDSTART(i0) gridStartp[i0]
    
            if( false )
            {
                printF(" grid=%i, nig=%i\n",grid,nig);
      	::display(interpolationPoint,"interpolationPoint");
      	::display(interpoleeGrid,"interpoleeGrid");
            }
            

      // order the interpolation points by interpolee grid.
            ng=0;
            const int ia=interpolationPoint.getBase(0), ib=interpolationPoint.getBound(0);
            assert( ia==0 );
            for( int i=ia; i<=ib; i++ )
      	NG(INTERPOLEEGRID(i))++;

      //	  ng.display("NG");

            GRIDSTART(0)=0;
            int grid2;
            for( grid2=1; grid2<numberOfComponentGrids; grid2++ )
      	GRIDSTART(grid2)=GRIDSTART(grid2-1)+NG(grid2-1);
        	  
      //	  gridStart.display("GRIDSTART");
      // ***** we need to assign the interpolationStartEndIndex 
      // **** this needs to be set on multigridLevel[0] too ********
        	  
      // for now we assume that the interpolation is implicit on coarser levels *** fix this ***
        	  
      //kkc left over from ogmg	  cg1.interpolationIsAllExplicit()=false;
      //kkc left over from ogmg	  cg1.interpolationIsAllImplicit()=true;

      //kkc left over from ogmg :
            for( grid2=0; grid2<numberOfComponentGrids; grid2++ )
            {
	//interpolationStartEndIndex(0,grid,grid2)=-1;
	//interpolationStartEndIndex(1,grid,grid2)=-1;
	//interpolationStartEndIndex(2,grid,grid2)=-1;
	//interpolationStartEndIndex(3,grid,grid2)=-1;
      	
      	if( NG(grid2)>0 )
      	{
        	  interpolationStartEndIndex(0,grid,grid2)=GRIDSTART(grid2);              // start value
        	  interpolationStartEndIndex(1,grid,grid2)=GRIDSTART(grid2)+NG(grid2)-1;  // end value
        	  if( true || cg.interpolationIsImplicit(grid,grid2,0) )
          	    interpolationStartEndIndex(2,grid,grid2)= interpolationStartEndIndex(1,grid,grid2);
	  // fix this: put any implicit points first
	  // 	   else if( ngi(grid2)>0 )
	  // 	     interpolationStartEndIndex(2,grid,grid2)=GRIDSTART(grid2)+ngi(grid2)-1; // end value for implicit pts.
      	}
            }
        	  
            if( numberOfDimensions==2 )
            {
      	for( int i=ia; i<=ib; i++ )
      	{
        	  grid2=INTERPOLEEGRID(i);
        	  int j=GRIDSTART(grid2);

        	  INTERPOLEEGRID1(j)=grid2;
        	  INTERPOLATIONPOINT1(j,0)=INTERPOLATIONPOINT(i,0);
        	  INTERPOLATIONPOINT1(j,1)=INTERPOLATIONPOINT(i,1);
        	  INTERPOLEELOCATION1(j,0)=INTERPOLEELOCATION(i,0);
        	  INTERPOLEELOCATION1(j,1)=INTERPOLEELOCATION(i,1);
        	  INTERPOLATIONCOORDINATES1(j,0)=INTERPOLATIONCOORDINATES(i,0);
        	  INTERPOLATIONCOORDINATES1(j,1)=INTERPOLATIONCOORDINATES(i,1);
        	  VARIABLEINTERPOLATIONWIDTH1(j)=VARIABLEINTERPOLATIONWIDTH(i);
              		  
        	  GRIDSTART(grid2)++;
      	}
            }
            else
            {
      	for( int i=ia; i<=ib; i++ )
      	{
        	  grid2=INTERPOLEEGRID(i);
        	  int j=GRIDSTART(grid2);
        	  INTERPOLEEGRID1(j)=grid2;
        	  INTERPOLATIONPOINT1(j,0)=INTERPOLATIONPOINT(i,0);
        	  INTERPOLATIONPOINT1(j,1)=INTERPOLATIONPOINT(i,1);
        	  INTERPOLATIONPOINT1(j,2)=INTERPOLATIONPOINT(i,2);
        	  INTERPOLEELOCATION1(j,0)=INTERPOLEELOCATION(i,0);
        	  INTERPOLEELOCATION1(j,1)=INTERPOLEELOCATION(i,1);
        	  INTERPOLEELOCATION1(j,2)=INTERPOLEELOCATION(i,2);
        	  INTERPOLATIONCOORDINATES1(j,0)=INTERPOLATIONCOORDINATES(i,0);
        	  INTERPOLATIONCOORDINATES1(j,1)=INTERPOLATIONCOORDINATES(i,1);
        	  INTERPOLATIONCOORDINATES1(j,2)=INTERPOLATIONCOORDINATES(i,2);
        	  VARIABLEINTERPOLATIONWIDTH1(j)=VARIABLEINTERPOLATIONWIDTH(i);
              		  
        	  GRIDSTART(grid2)++;
      	}
            }
        	  
      //      interpoleeLocation1.display("IL after");

            interpoleeGrid.reference(interpoleeGrid1);
            interpolationPoint.reference(interpolationPoint1);
            interpoleeLocation.reference(interpoleeLocation1);
            variableInterpolationWidth.reference(variableInterpolationWidth1);
            interpolationCoordinates.reference(interpolationCoordinates1);
            

        } // if nig>0
        
    }
  //  interpolationStartEndIndex.display("ISTARTEND");
        	  
}
