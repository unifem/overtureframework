// This file automatically generated from InterpolatePointsOnAGrid.bC with bpp.
#include "Overture.h"
#include "display.h"
#include "InterpolatePointsOnAGrid.h"
#include "MappingProjectionParameters.h"
#include "ParallelUtility.h"
// #include "ParallelOverlappingGridInterpolatePoints.h"
#include "CanInterpolate.h"

#include "Ogen.h"

int InterpolatePointsOnAGrid::debug=0;

static int numberOfPointsNotAssignedMessages=0;


// The macro MODR shifts a point back into the main periodic region
#define NRM(axis)  ( indexRange(End,axis)-indexRange(Start,axis)+1 )
#define MODR(i,axis)  ( ( (i-indexRange(Start,axis)+NRM(axis)) % NRM(axis)) +indexRange(Start,axis) )
#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)


static int localDebug=0;   // 1+2+4+8;

InterpolatePointsOnAGrid::
InterpolatePointsOnAGrid()
// ===============================================================================================================
/// Default constructor.
// ===============================================================================================================
{
    interpolationIsInitialized=false;  // this is set to true when the interpolation has been initialized.

    indirection=NULL;
    interpolationLocation=NULL;
    variableInterpolationWidth=NULL;
    interpolationCoordinates=NULL;

    interpolationOffset=2.5;  // offset in grid lines from the unit cube where we are allowed to interpolate

    infoLevel=0;              // bit flag, set to 1, 1+2=3, ...

    interpolationWidth=2;     // width of the interpolation stencil.
    interpolationType=implicitInterpolation;  // use implicit interpolation by default 
    
    assignAllPoints=false;  // if true, always assign all points.

    numberOfValidGhostPoints=defaultNumberOfValidGhostPoints;
  //  pogip=NULL;  // for parallel interpolation of points

  // These hold values about the last interpolation operation:
    numberOfBackupInterpolation=-1;
    numberOfExtrapolated=-1;
    numberOfInterpolated=-1;
    numberOfUnassigned=-1;

  // For interpolateAll:
    totalNumToInterpolate=0;
    pInterpAllIndirection=NULL;
    numToInterpolatePerGrid=NULL;

    logFile=NULL;
    plogFile=NULL;


  // from pogip: 
    POGI_COMM = Overture::OV_COMM;  // use this communicator by default

    debugFile=NULL;
    
  // explicitInterpolationStorageOption=precomputeAllCoefficients; // precomputeNoCoefficients;
    explicitInterpolationStorageOption=precomputeNoCoefficients;

    numberOfDimensions=0;
    numberOfComponentGrids=0;
    numberOfBaseGrids=0;
    maxInterpolationWidth=0;
    coeffWidthDimension=0;

    allGridsHaveLocalData=false;
    onlyAmrGridsHaveLocalData=true;
    noGridsHaveLocalData=false;

    maximumRefinementLevelToInterpolate=INT_MAX/2;

    npr=0; nps=0;
    pMapr=NULL; pMaps=NULL;
}

InterpolatePointsOnAGrid::
~InterpolatePointsOnAGrid()
{
    delete [] indirection;
    delete [] interpolationLocation;
//  delete [] interpolationLocationPlus;
    delete [] interpolationCoordinates;
    delete [] variableInterpolationWidth;
    
    delete [] pInterpAllIndirection;
    delete [] numToInterpolatePerGrid;


    destroy();
    
    const int myid = Communication_Manager::My_Process_Number;
    if( myid==0 && debugFile!=NULL )
        fclose(debugFile);

  // delete pogip;
}

int InterpolatePointsOnAGrid::
getNumberBackupInterpolation() const
// ===================================================================================================
/// \brief: return the number of points that were interpolated with backup rules (i.e. using a lower than
///      requested interpolation width).
/// \details: The function returns info about the last interpolation operation such as interpolateAllPoints.
///    In parallel this value corresponds to the current processor (not the sum over all processors)
// ==================================================================================================
{
    return numberOfBackupInterpolation;
}

int InterpolatePointsOnAGrid::
getNumberExtrapolated() const
// ===================================================================================================
/// \brief: return the number of points that were extrapolated.
/// \details: The function returns info about the last interpolation operation such as interpolateAllPoints.
///    In parallel this value corresponds to the current processor (not the sum over all processors)
// ==================================================================================================
{
    return numberOfExtrapolated;
}

int InterpolatePointsOnAGrid::
getNumberInterpolated() const
// ===================================================================================================
/// \brief: return the number of points that were interpolated with the full interpolation width.
/// \details: The function returns info about the last interpolation operation such as interpolateAllPoints.
///    In parallel this value corresponds to the current processor (not the sum over all processors)
// ==================================================================================================
{
    return numberOfInterpolated;
}

int InterpolatePointsOnAGrid::
getNumberUnassigned() const
// ===================================================================================================
/// \brief: return the number of points that were not assigned.
/// \details: The function returns info about the last interpolation operation such as interpolateAllPoints.
///    In parallel this value corresponds to the current processor (not the sum over all processors)
// ==================================================================================================
{
    return numberOfUnassigned;
}

int InterpolatePointsOnAGrid::
getTotalNumberOfPointsAssigned( int grid )
// ===================================================================================================
/// \brief: Return the total number of points that are interpolate from a grid (sum across all processors).
/// \param (grid) : donor grid
/// \return value : the number of points assigned or -1 means an error occured, invalid value for grid.
// ==================================================================================================
{
    if( grid>=0 && grid<=numberOfInterpolationPoints.getBound(0) )
    {
        int num = numberOfInterpolationPoints(grid);
        #ifdef USE_PPP
            num = ParallelUtility::getSum(num);
        #endif
        return num;
    }
    else
    {
        printF("InterpolatePointsOnAGrid::ERROR:getTotalNumberOfPointsAssigned: grid=%i is not \n"
                      " valid: the numberOfInterpolationPoints has only %i grids\n",grid,
                      numberOfInterpolationPoints.getBound(0)+1);
        
        return -1;
    }
}


int InterpolatePointsOnAGrid::
setAssignAllPoints( bool trueOrFalse /* =true */ )
// ===================================================================================================
/// \brief:  Specify whether all points should be assigned, using extrapolation if necessary.
/// \param trueOrFalse (input) : if true assign all points, using extrapolation if necessary.
///   If false, there may be points left assigned that are outside the domain.
/// \notes: It may be more expensive to find a way to assign all points. 
// ==================================================================================================
{
    assignAllPoints=trueOrFalse;
    return 0;
}



int InterpolatePointsOnAGrid::
setNumberOfValidGhostPoints( int numValidGhost /* =defaultNumberOfValidGhostPoints */ )
// ===================================================================================================
/// \brief: Set the number of valid ghost points that can be used when interpolating from a grid function
///
// ==================================================================================================
{
    numberOfValidGhostPoints=numValidGhost;
    return 0;
}



int InterpolatePointsOnAGrid::
setInfoLevel( int info )
// ==================================================================================
/// \brief Set the flag for specfying what information messages should be printed.
/// info=0 mean no info. info=1, 1+2, 1+2+4, ... gives succesively more info.
// ==================================================================================
{
    infoLevel=info;
    return 0;
}



int InterpolatePointsOnAGrid::
setInterpolationOffset( real widthInGridLines )
// ==================================================================================
/// \brief Set the offset in grid lines from the unit cube where we are allowed to interpolate
// ==================================================================================
{
    interpolationOffset=widthInGridLines;
    return 0;
}

int InterpolatePointsOnAGrid::
setInterpolationType( InterpolationTypeEnum interpType )
// ==================================================================================
/// \brief Set the offset in grid lines from the unit cube where we are allowed to interpolate
// ==================================================================================
{
    interpolationType=interpType;
    return 0;
}

int InterpolatePointsOnAGrid::
setInterpolationWidth( int width )
// ==================================================================================
/// \brief Set the width the interpolation stencil.
// ==================================================================================
{
    interpolationWidth=width;
    return 0;
}


const IntegerArray & InterpolatePointsOnAGrid::
getStatus() const
// ==================================================================================
/// \brief Return the status array for the last interpolation. The values in status are from the
/// InterpolationStatusEnum.
// ==================================================================================
{
    return status;
}


int InterpolatePointsOnAGrid::
getInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues, IntegerArray & interpoleeGrid, RealArray & interpolationCoordinates ) const
// ============================================================================================================================================
/// \brief Return the index values, interpoleeGrid and interpolationCoordinates for the last interpolation.
///
/// \param cg (input) : CompositeGrid.
/// \param indexValues (output) : indexValues(i,0:d-1) lower left corner of the interpolation stencil for point i.
/// \param interpoleeGrid (output) : interpoleeGrid(i) donor grid for point i.
/// \param interpolationCoordinates (output) : interpolationCoordinates(i,0:d-1) unit square coordinates for point i.
/// 
// ============================================================================================================================================
{
    int option=1;
    return getInternalInterpolationInfo(cg,indexValues,interpoleeGrid,interpolationCoordinates,option );
}

int InterpolatePointsOnAGrid::
getInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues,IntegerArray & interpoleeGrid) const
// ==================================================================================
/// \brief Return the index values and interpoleeGrid for the last interpolation.
///
/// \param cg (input) : CompositeGrid.
/// \param indexValues (output) : indexValues(i,0:d-1) lower left corner of the interpolation stencil for point i.
/// \param interpoleeGrid (output) : interpoleeGrid(i) donor grid for point i.
/// 
// ==================================================================================
{
    int option=0;
    return getInternalInterpolationInfo(cg,indexValues,interpoleeGrid,Overture::nullRealArray(),option );
}


int InterpolatePointsOnAGrid::
getInternalInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues,IntegerArray & interpoleeGrid, 
                                                          RealArray & interpolationCoords, const int option ) const
// ==================================================================================
/// \brief Return the index values and interpoleeGrid for the last interpolation.
///
/// \param cg (input) : CompositeGrid.
/// \param indexValues (output) : indexValues(i,0:d-1) lower left corner of the interpolation stencil for point i.
/// \param interpoleeGrid (output) : interpoleeGrid(i) donor grid for point i.
/// \param interpolationCoords (output) : if option==1 then return interpolationCoordinates(i,0:d-1) unit square coordinates for point i.
/// \param option (input) : option=0 : do NOT return interpolationCoordinates, option==1 : do return  interpolationCoordinates.
///
// ==================================================================================
{
    const int numberOfDimensions = cg.numberOfDimensions();
    const int numberOfComponentGrids = cg.numberOfComponentGrids();
    const int rangeDimension = numberOfDimensions;
    const int domainDimension = cg[0].domainDimension();

    int totalNumberOfInterpolationPoints=sum(numberOfInterpolationPoints);
    
    if( totalNumberOfInterpolationPoints==0 )
    {
        indexValues.redim(0);
        interpoleeGrid.redim(0);
        return 1;
    }
    
    assert( indirection!=NULL );

    indexValues.redim(totalNumberOfInterpolationPoints,3);
    indexValues=0;
    interpoleeGrid.redim(totalNumberOfInterpolationPoints);
    interpoleeGrid=0;
    if( option==1 )
    {
        interpolationCoords.redim(totalNumberOfInterpolationPoints,3);
        interpolationCoords=0.;
    }
    
    
  // We must check the highest priority grid first since this was the order the points were generated.
  // A point may be extrapolated on a higher priority grid but then interpolated on a lower priority grid.
    int grid;
    for( grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
    {
        const int num=numberOfInterpolationPoints(grid);
        if( num>0 )
        {
            IntegerArray & ia = indirection[grid];
            IntegerArray & ip = interpolationLocation[grid];
            RealArray & ci = interpolationCoordinates[grid];

            const int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]
            const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
            const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]
            const real *cip = ci.Array_Descriptor.Array_View_Pointer1;
            const int ciDim0=ci.getRawDataSize(0);
#define CI(i0,i1) cip[i0+ciDim0*(i1)]

            for( int i=0; i<num; i++ )
            {
      	for( int axis=0; axis<domainDimension; axis++ )
        	  indexValues(IA(i),axis)=IP(i,axis);
      	interpoleeGrid(IA(i))=grid;
      	if( option==1 )
      	{
        	  for( int axis=0; axis<domainDimension; axis++ )
          	    interpolationCoords(IA(i),axis)=CI(i,axis);
      	}
      	
            }
            
        }
    }
    
    return 0;

}

#undef IP
#undef IA

int InterpolatePointsOnAGrid::
getInterpolationStencil( MappedGrid & mg, const int width, const real *rv, int *iv )
// ====================================================================================
/// \brief Return the lower left corner of the interpolation stencil.
/// \width (input) : interpolation stencil width
/// \rv (input) : rv[axis] : unit cube coordinates of the point
/// \iv (output) : iv[axis] : lower left corner of the stencil
// ====================================================================================
{

    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
    // Get the lower-left corner of the interpolation cube.
        int intLoc=int(floor(rv[axis]/mg.gridSpacing(axis) + mg.indexRange(0,axis) -
                   			 .5 * width + (mg.isCellCentered(axis) ? .5 : 1.)));
        if (!mg.isPeriodic(axis)) 
        {
            if( (intLoc < mg.extendedIndexRange(0,axis)) && (mg.boundaryCondition(Start,axis)>0) )
            {
	// Point is close to a BC side, one-sided interpolation used.
      	intLoc = mg.extendedIndexRange(0,axis);
            }
            if( (intLoc + width - 1 > mg.extendedIndexRange(1,axis))
        	  && (mg.boundaryCondition(End,axis)>0) )
            {
	// Point is close to a BC side, one-sided interpolation used.
      	intLoc = mg.extendedIndexRange(1,axis) - width + 1;
            }
        } // end if
        iv[axis] = intLoc;
    } // end for axis
    return 0;
}


// *********** this is no longer used: 
// int InterpolatePointsOnAGrid::
// checkCanInterpolate(CompositeGrid & cg ,
// 		    int grid, int donor, 
//                     RealArray & r, 
//                     IntegerArray & interpolates,
// 		    IntegerArray & useBackupRules )
// // ========================================================================================
// // /Description:
// //    Here is the new (serial/parallel) canInterpolate function
// //
// // /grid, donor (input): check for interpolation of points on "grid" from donor
// // /r (input) : r(i,0:nd-1) -- coordinates of the points to check  
// // 
// // /interpolates (output) : return true if point i can be interpolated
// // / useBackupRules (input) : if useBackupRules(i)==true then allow backup interpolation rules.
// // ========================================================================================
// {
//   int numberToCheck= r.getLength(0);
//   // NO: if( numberToCheck==0 ) return 0;

//   const int interpWidth = cg.interpolationWidth(0,grid,donor,0); // assumes the same width in all directions
//   const int numberOfDimensions = cg.numberOfDimensions();
    
//   using namespace CanInterpolate;
    
//   CanInterpolateQueryData *cid = new CanInterpolateQueryData[max(1,numberToCheck)];
//   const int rBase0=r.getBase(0);
//   for( int n=0; n<numberToCheck; n++ )
//   {
//     int i =n+rBase0;
//     cid[n].id=n; cid[n].i=i; cid[n].grid=grid; cid[n].donor=donor;
//     for( int axis=0; axis<numberOfDimensions; axis++ )
//       cid[n].rv[axis]=r(i,axis);

//   }
    
//   // Allocate space for results
//   CanInterpolateResultData *cir =new CanInterpolateResultData[max(1,numberToCheck)];

//   // --------------------------------
//   // -------check canInterpolate-----
//   // --------------------------------

//   // this function will find any valid interpolation by default (i.e. backup results too)
//   // this function also computes the interpolation stencil
//   CanInterpolate::canInterpolate( cg, numberToCheck,cid, cir );

//   // process the canInterpolate results
//   const int interpolatesBase0=interpolates.getBase(0);
//   for( int n=0; n<numberToCheck; n++ )
//   {
//     int width = cir[n].width;   // interpolation width (=0 if invalid)
//     // int i=cid[n].i;
//     // int grid=cid[n].grid;
//     // cir[i].il[0];
//     // cir[i].il[1];
//     // cir[i].il[2];

//     int i0=n+interpolatesBase0;
//     if( width == interpWidth )
//     {
//       interpolates(i0)=true;
//     }
//     else if( useBackupRules(i0) && width>0 ) // what to do here ? 
//     {
//       interpolates(i0)=true;
//     }
//     else
//     {
//       interpolates(i0)=false;
//     }
//   }

// //     intSerialArray & variableInterpolationWidth = cg->variableInterpolationWidthLocal[grid]; 
// //     // Choose the donor point with largest width (and use the first one with that width) : 
// //     if( width>variableInterpolationWidth(i) )
// //     {
// //       int interpolee=cid[n].donor;
// //     }

//   delete [] cid;
//   delete [] cir;
    
//   return 0;
// }



int InterpolatePointsOnAGrid::
buildInterpolationInfo(const RealArray & positionToInterpolate, 
                                              CompositeGrid & cg,
                                              RealArray *projectedPoints /* =NULL */,
                                              IntegerArray *checkTheseGrids /* =NULL */ )
// =================================================================================================
/// \brief Build the interpolation location arrays that can be used to interpolate a grid function
///  at some specified points. For surface grids, optionally return the points projected onto one
///  of the surface grids. 
///
/// \param positionToInterpolate (input) : positionToInterpolate(i,0:domainDimension-1) position of point i
/// \param cg (input) : Composite grid. 
/// \param projectedPoints (output) : If projectedPoints!=NULL AND the CompositeGrid consists of surface grids,
///     then this array will hold the values of the point in positionToInterpolate that have been
///     projected onto the surface (underlying the grid that the point was interpolated from).   
/// \param checkTheseGrids (input): if not NULL then this array indicates which grids to check for an
///   interpolation point, i.e. (*checkTheseGrids)(grid)!=0 means this grid can be used for interpolation.
///
/// \return if negative then the absolute value of the return value is the number of points 
///   not assigned.
///
/// \details
/// Check grids starting from the highest priority grid. 
/// Keep looking until we find a grid that we can interpolate from properly.
/// If, before finding a grid we can interpolate from,  we find we can extrapolate, then save the
/// extrapolation info. Thus, when finished, either we can interpolate, extrapolate or 
/// we could not assign a point at all.
/// 
/// /note wdh: 070228 -- support for surface grids in 3D
// =================================================================================================
{
    interpolationIsInitialized=true;
    
    if( cg.numberOfComponentGrids()==0 )
    {
        return 0;
    }

    destroy();
    

    real time=getCPU();
    real timeMap=0., timeCanInterpolate=0.;
    
  // debug=7; // *********************

    numberOfDimensions = cg.numberOfDimensions();
    numberOfComponentGrids = cg.numberOfComponentGrids();
    numberOfBaseGrids = cg.numberOfBaseGrids();

    const int myid=max(0,Communication_Manager::My_Process_Number);
    const int np=Communication_Manager::numberOfProcessors();
    
    if( debug>0 && logFile==NULL )
    {
      #ifdef USE_PPP
        logFile = fopen(sPrintF("ipogNP%i.log",np),"w" ); 
        plogFile = fopen(sPrintF("ipogNP%i.p%i.log",np,myid),"w" ); 
        fprintf(plogFile,
                        " ********************************************************************************************* \n"
          	    " **** InterpolatePointsOnAGrid log file, myid=%i, NP=%i interp-width=%i pts=%i ********* \n"
          	    " ********************************************************************************************* \n\n",
                                                                  myid,np,interpolationWidth,positionToInterpolate.getLength(0));
      #else
        logFile = fopen("ipog.log","w" ); 
        plogFile=logFile;
        fprintf(plogFile,
                        " *************************************************************************************** \n"
          	    " **** InterpolatePointsOnAGrid log file, interp-width=%i pts=%i ********* \n"
          	    " *************************************************************************************** \n\n",
                                                                  interpolationWidth,positionToInterpolate.getLength(0));
      #endif
    }
    

  // cg.update(MappedGrid::THEboundingBox);
  // *wdh* 070618 -- we need to make sure the bounding box is created properly ---
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        if( cg[grid].isRectangular() )
            cg[grid].update( MappedGrid::THEboundingBox );
        else
            cg[grid].update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEboundingBox );
    }



    const int rangeDimension = numberOfDimensions;
    const int domainDimension = cg[0].domainDimension();

    const bool surfaceGrid=domainDimension!=rangeDimension;
    const bool projectSurfaceGridPoints = surfaceGrid && projectedPoints!=NULL;
    

    Range Axes=domainDimension;

    int grid, axis;
    for( grid=0; grid<numberOfComponentGrids; grid++ )
    {
        if( !cg[grid].isAllVertexCentered() && !cg[grid].isAllCellCentered() )
        {
            printF("InterpolatePointsOnAGrid::buildInterpolationInfo:ERROR: grids must be either vertex or cell centered, no mongrels! \n");
            return 1;
        }      
    }

    int numberOfPointsToInterpolate=positionToInterpolate.getLength(0);

    const real epsi=1.e-3;
    int extrap,pointWasExtrapolated;
    int returnValue=0;  // 0=ok, >0 error, <0 some points extrapolated


    delete [] indirection;
    delete [] interpolationLocation;
//  delete [] interpolationLocationPlus;
    delete [] variableInterpolationWidth;   // new 091113
    delete [] interpolationCoordinates;

    indirection = new IntegerArray [numberOfComponentGrids];
    interpolationLocation = new IntegerArray[numberOfComponentGrids];
//  interpolationLocationPlus = new IntegerArray[numberOfComponentGrids];
    variableInterpolationWidth = new IntegerArray[numberOfComponentGrids];
    interpolationCoordinates = new RealArray[numberOfComponentGrids];

    numberOfInterpolationPoints.redim(numberOfComponentGrids);
    numberOfInterpolationPoints=0;
    int *numberOfInterpolationPointsp = numberOfInterpolationPoints.Array_Descriptor.Array_View_Pointer0;
#define NUMBEROFINTERPOLATIONPOINTS(i0) numberOfInterpolationPointsp[i0]

  // *****************************************************
  // ******** Find an interpolation stencil to use *******
  // *****************************************************

    Range R=numberOfPointsToInterpolate;
    Range D=numberOfDimensions;
    Range Rx=numberOfDimensions;

    IntegerArray ia0(R);
    status.redim(R);
    int *statusp = status.Array_Descriptor.Array_View_Pointer0;
#define STATUS(i0) statusp[i0]
    int *ia0p = ia0.Array_Descriptor.Array_View_Pointer0;
#define IA0(i0) ia0p[i0]


  // status(i) : will be set to the interpWidth found -- we keep looking until we find a grid
  //             which can interpolate with "interpolationWidth" 
    status=notInterpolated;  // ==0 

  // *wdh* 091113 -- do this for pogip for now: -- is this right?  ****************************************
  // RealArray rp(R,D);
  // IntegerArray donor(R), il(R,D), viw(R);
    


    const RealArray & x =positionToInterpolate;
    const real *xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
    #define X(i0,i1) xp[i0+xDim0*(i1)]

    real bb[2][3];

  // For projecting points onto surfaces:
    MappingProjectionParameters mpParams;
    const realArray & rProject = surfaceGrid ? mpParams.getRealArray(MappingProjectionParameters::r) : 
                                      Overture::nullRealDistributedArray();

    const RealArray & xProject = projectSurfaceGridPoints ? *projectedPoints : x;
    if( projectSurfaceGridPoints )
    {
        if( xProject.dimension(0)!=x.dimension(0) )
        {
            ((realArray &)xProject).redim(x.dimension(0),Range(rangeDimension));
        }
    }

    real *xProjectp = xProject.Array_Descriptor.Array_View_Pointer1;
    const int xProjectDim0=xProject.getRawDataSize(0);
#define XPROJECT(i0,i1) xProjectp[i0+xProjectDim0*(i1)]
  
  // 
  // Check grids starting from the highest priority grid. 
  // Keep looking until we find a grid that we can interpolate from properly.
  // If, before finding a grid we can interpolate from,  we find we can extrapolate, then save the
  // extrapolation info. Thus, when finished, either we can interpolate, extrapolate or 
  // we could not assign a point at all.
  // 
    for( grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
    { 
        if( checkTheseGrids!=NULL && (*checkTheseGrids)(grid)==0 ) continue; // skip this grid
        
        MappedGrid & mg = cg[grid];
        if( mg.getGridType()==MappedGrid::unstructuredGrid )
        {
      // for now we skip unstructured grids
            printF("InterpolatePointsOnAGrid::buildInterpolationInfo: skipping grid=%i since it is an unstructured grid.\n",grid);
        }
        

        Mapping & mapping = mg.mapping().getMapping();

        const intArray & mask = mg.mask();
        
        const RealArray & gridSpacing = mg.gridSpacing();
        const IntegerArray & indexRange = mg.indexRange();
        const IntegerArray & dimension  = mg.dimension();
        const IntegerArray & isPeriodic = mg.isPeriodic();
        const real shift = (bool)mg.isAllVertexCentered() ? 0. : .5; // shift position for cell centered grids

        const int *dimensionp = dimension.Array_Descriptor.Array_View_Pointer1;
        const int dimensionDim0=dimension.getRawDataSize(0);
#define DIMENSION(i0,i1) dimensionp[i0+dimensionDim0*(i1)]
        const int *indexRangep = indexRange.Array_Descriptor.Array_View_Pointer1;
        const int indexRangeDim0=indexRange.getRawDataSize(0);
#define INDEXRANGE(i0,i1) indexRangep[i0+indexRangeDim0*(i1)]
        const real *gridSpacingp = gridSpacing.Array_Descriptor.Array_View_Pointer0;
#define GRIDSPACING(i0) gridSpacingp[i0]


    // get the bounding box for this grid --- increase bounding box a bit 

        const RealArray & boundingBox = mg.boundingBox();
    // boundingBox.display("Here is the boundingBox");

    // increase the size of the bounding box to allow for interp. from ghost point
        real scale=0.;
        for( axis=0; axis<numberOfDimensions; axis++ )
            scale=max(scale,boundingBox(1,axis)-boundingBox(0,axis));
  
        const IntegerArray & egir = extendedGridIndexRange(mg);
    // rbb(side,axis) : bounding box for valid points on the unit square, including ghost lines ob bc=0 boundaries
        real prbb[6];
        #define rbb(side,axis) (prbb[(side)+2*(axis)])  

        const real delta=scale*.25;  
        for( axis=0; axis<rangeDimension; axis++ )
        {
            bb[0][axis]=boundingBox(0,axis)-delta;
            bb[1][axis]=boundingBox(1,axis)+delta;

            for( int side=0; side<=1; side++ )
            {
      	rbb(side,axis)= side + (egir(side,axis)-mg.gridIndexRange(side,axis))*mg.gridSpacing(axis) - (1-2*side)*REAL_EPSILON*100.;
	// rbb(side,axis)= side + (egir(side,axis)-mg.gridIndexRange(side,axis))*mg.gridSpacing(axis);
            }
        }
        
    // printF(" ********  grid=%i r-bounding box=[%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
    // 	   grid,rbb(0,0),rbb(1,0),rbb(0,1),rbb(1,1),rbb(0,2),rbb(1,2));


    // make a list of points inside the bounding box
        int j=0;
        if( rangeDimension==2 )
        {
            for( int i=0; i<numberOfPointsToInterpolate; i++ )
            {
      	if( STATUS(i) < interpolationWidth )  // either not interpolated or not accurate enough
      	{
        	  real x0=X(i,0);
        	  real y0=X(i,1);
        	  if( x0>=bb[0][0] && x0<=bb[1][0] &&
            	      y0>=bb[0][1] && y0<=bb[1][1] ) 
        	  {
          	    IA0(j)=i;
          	    j++;
        	  }
      	}
            }
        }
        else
        {
            for( int i=0; i<numberOfPointsToInterpolate; i++ )
            {
      	if( STATUS(i) < interpolationWidth )  // either not interpolated or not accurate enough
      	{
        	  real x0=X(i,0);
        	  real y0=X(i,1);
        	  real z0=X(i,2);
        	  if( x0>=bb[0][0] && x0<=bb[1][0] &&
            	      y0>=bb[0][1] && y0<=bb[1][1] &&
            	      z0>=bb[0][2] && z0<=bb[1][2] ) 
        	  {
          	    IA0(j)=i;
          	    j++;
        	  }
      	}
            }
        }
        
        
        int numberToCheck=j;
        Range I=numberToCheck;
        RealArray ra,xa;
        IntegerArray & ia = indirection[grid];
        if( numberToCheck>0 )
        {
            ra.redim(I,domainDimension); 
            xa.redim(I,rangeDimension);
            ia.redim(I);
        }
        
        real *rap = ra.Array_Descriptor.Array_View_Pointer1;
        const int raDim0=ra.getRawDataSize(0);
#define RA(i0,i1) rap[i0+raDim0*(i1)]

        real *xap = xa.Array_Descriptor.Array_View_Pointer1;
        const int xaDim0=xa.getRawDataSize(0);
#define XA(i0,i1) xap[i0+xaDim0*(i1)]


        int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]


        if( numberToCheck>0 )
        {
      // attempt to interpolate points from this grid.

            ia(I)=ia0(I);

            if( rangeDimension==2 )
            {
      	for( int i=0; i<numberToCheck; i++ )
      	{
        	  XA(i,0)=X(IA(i),0);
        	  XA(i,1)=X(IA(i),1);
      	}
            }
            else
            {
      	for( int i=0; i<numberToCheck; i++ )
      	{
        	  XA(i,0)=X(IA(i),0);
        	  XA(i,1)=X(IA(i),1);
        	  XA(i,2)=X(IA(i),2);
      	}
            }
            
            ra=-1;
        }
        
        real timea=getCPU();
        if( !surfaceGrid )
        {
      // mapping.useRobustInverse(true); // *****************
            
#ifdef USE_PPP
            mapping.inverseMapS(xa,ra);
#else
            mapping.inverseMap(xa,ra);
#endif
        }
        else
        {
      // mapping.inverseMap(xa,ra);  // fix this -- get ra from mp
            ((RealArray&)rProject).redim(I,domainDimension);

#ifdef USE_PPP
            mapping.projectS(xa,mpParams);
#else
            mapping.project(xa,mpParams);
#endif

            real *rProjectp = rProject.Array_Descriptor.Array_View_Pointer1;
            const int rProjectDim0=rProject.getRawDataSize(0);
#define RPROJECT(i0,i1) rProjectp[i0+rProjectDim0*(i1)]

            for( int i=0; i<numberToCheck; i++ )
            {
      	RA(i,0)=RPROJECT(i,0);
      	RA(i,1)=RPROJECT(i,1); 
            }
        }
        timeMap+= getCPU()-timea;
            
        if( debug & 8 )
        {
            ::display(ra,sPrintF("InterpolatePointsOnAGrid:: ra after inversion (grid=%i)",grid),"%6.3f ");
            if( surfaceGrid )
      	::display(xa,sPrintF("InterpolatePointsOnAGrid:: xa after projection (grid=%i)",grid),"%6.3f ");
        }
            

    // ********************************************************************
    // **** compress possible points based on unit square coordinates *****
    // ********************************************************************
        
        real offset0, offset1, offset2;
        
    // allowExtrapolation : If we turn this on we also need to make sure a point outside is not marked at interpolated,
    //                      cf. sib grids 
        bool allowExtrapolation=false;  
        if( allowExtrapolation )
        {
            offset0=offset1=offset2=9.;
        }
        else
        {
            offset0=.5+gridSpacing(0)*interpolationOffset;  
            offset1=.5+gridSpacing(1)*interpolationOffset;  
            offset2=.5+gridSpacing(2)*interpolationOffset;  
        }
        

    // *NOTE* we do not bother compressing XA
        j=0;
        int i;
        if( domainDimension==2 )
        {
            for( int i=0; i<numberToCheck; i++ )
            {
      	if( fabs(RA(i,0)-.5)<offset0 && fabs(RA(i,1)-.5)<offset1 )
      	{
        	  if( i!=j )
        	  {
          	    IA(j)=IA(i);
          	    RA(j,0)=RA(i,0);
          	    RA(j,1)=RA(i,1);
          	    if( projectSurfaceGridPoints )
          	    { // we need to save the projected xa points too in this case
            	      XA(j,0)=XA(i,0);
            	      XA(j,1)=XA(i,1);
            	      XA(j,2)=XA(i,2);
          	    }
            	      
        	  }
        	  j++;
      	}
            }
        }
        else
        {
            for( int i=0; i<numberToCheck; i++ )
            {
      	if( fabs(RA(i,0)-.5)<offset0 && fabs(RA(i,1)-.5)<offset1 && fabs(RA(i,2)-.5)<offset2 )
      	{
          // printF(" InterpolatePointsOnAGrid: grid=%i i=%i IA=%i x=(%9.3e,%9.3e,%9.3e) r=(%9.3e,%9.3e,%9.3e)\n",grid,i,IA(i),
          //         XA(i,0),XA(i,1),XA(i,2),RA(i,0),RA(i,1),RA(i,2));
        	  
        	  if( i!=j )
        	  {
          	    IA(j)=IA(i);
          	    RA(j,0)=RA(i,0);
          	    RA(j,1)=RA(i,1);
          	    RA(j,2)=RA(i,2);
        	  }
        	  j++;
      	}
            }
        }
            
        numberToCheck=j;

    // if( numberToCheck> 0 )  // NOTE: we cannot break here since there is communication below 

        if( numberToCheck> 0 )
            I=numberToCheck;
        else
            I=1; // just set to one -- then we can allocate arrays below without checking for numberToCheck==0
            
        IntegerArray & ip  = interpolationLocation[grid];
        IntegerArray & viw = variableInterpolationWidth[grid];

    // Note: we could reduce the following sizes if not all points are used: **************************** FINISH
        ip.redim(I,numberOfDimensions);  viw.redim(I);
        viw=interpolationWidth;  // default

        int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
        const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]
//       int *ip1p = ip1.Array_Descriptor.Array_View_Pointer1;
//       const int ip1Dim0=ip1.getRawDataSize(0);
// #define IP1(i0,i1) ip1p[i0+ip1Dim0*(i1)]

        RealArray & ci = interpolationCoordinates[grid];
        ci.redim(I,numberOfDimensions);
        real *cip = ci.Array_Descriptor.Array_View_Pointer1;
        const int ciDim0=ci.getRawDataSize(0);
#define CI(i0,i1) cip[i0+ciDim0*(i1)]


//     RealArray dra; 
//       dra.redim(I,numberOfDimensions);
//       real *drap = dra.Array_Descriptor.Array_View_Pointer1;
//       const int draDim0=dra.getRawDataSize(0);
// #define DRA(i0,i1) drap[i0+draDim0*(i1)]

//       RealArray dr(I,numberOfDimensions);
//       real *drp = dr.Array_Descriptor.Array_View_Pointer1;
//       const int drDim0=dr.getRawDataSize(0);
// #define DR(i0,i1) drp[i0+drDim0*(i1)]




//    IntegerArray interpolates(I), useBackupRules(I);
//    interpolates=true; // ** false;
//    useBackupRules=true;

    // If checkForOneSided=TRUE then canInterpolate will not allow a one-sided interpolation
    // stencil to use ANY interiorBoundaryPoint's -- this is actually too strict. We really
    // only want to disallow interpolation that has less than the minimum overlap distance
    //
//    bool checkForOneSided;
    // checkForOneSided=false;  // check for one side interp from non-conforming grids *******

    // Now we check for one sided interpolation from near boundaries of nonconforming grids that
    // are interpolating
//    checkForOneSided=true; 

    // NOTE: checkCanInterpolate will use cg.interpolationWidth and interpolationOverlap 
    //   We need to define a "grid-number" for the receptor grid. 
        int donor = grid;
        int receptor = 0; // Is this OK if donor=receptor ? 

//     RealArray rv;  // ***********************************************************    fix me -- 
//     if( numberToCheck> 0 )
//     {
//       rv.redim(I,D);
//       rv=ra(I,D);         // just reference or pass a view below ??
//     }
        
    // temporarily change these for the canInterpolate function:
        const int widthOld = cg.interpolationWidth(0,grid,donor,0);
        const real ovOld =  cg.interpolationOverlap(0,grid,donor,0);
            
    // Can we be sure that cgCanInterpolate is consistent with getInterpolationStencil??
    // cgCanInterpolate:         ia=rr-ov,      ib=rr+ov+1  -> width = 2*ov+1 -> ov=(width-1)/2
    // getInterpolationStencil : ia=rr-.5*iw+1  ib=rr+.5*iw -> ov=.5*iw-1 = .5*(width-2)
    // Ogen: changeParameters: ov = .5*(iw-2) for implicit interp
        real ov=max(0., (interpolationWidth-2.)/2. ) ; // Is this correct?
        if( interpolationType==explicitInterpolation )
        {
            const int discretizationWidth = cg[donor].discretizationWidth(0);
            ov = ( interpolationWidth + discretizationWidth -3 )*.5;  // See Ogen::updateParameters
        }
        cg.interpolationWidth(Rx,receptor,donor,0)=interpolationWidth;  // target interp. width
        cg.interpolationOverlap(Rx,receptor,donor,0)=ov;

    // We need a more general canInterpolate function:
    //   - check for different IW
    //   - check for extrap. 
    //   - optionally use ghost points
    //   - return il and width
    // See: parallel/CanInterpolate.C
    // 
        real timeb=getCPU();
        
        using namespace CanInterpolate;
    // Allocate space for queries: 
        CanInterpolateQueryData *cid = new CanInterpolateQueryData[max(1,numberToCheck)];

    // fill in the query data:
        const int rBase0=ra.getBase(0);
        for( int n=0; n<numberToCheck; n++ )
        {
            int i =n+rBase0;
            cid[n].id=n; cid[n].i=i; 
            cid[n].grid=receptor; 
            cid[n].donor=donor;  // NOTE: we could process multiple donor grids at once as an optimization
            for( int axis=0; axis<numberOfDimensions; axis++ )
      	cid[n].rv[axis]=RA(i,axis);

        }
    // Allocate space for results
        CanInterpolateResultData *cir =new CanInterpolateResultData[max(1,numberToCheck)];

    // new: this version is for serial and parallel:
    // this function will find any valid interpolation by default (i.e. backup results too)
    // this function also computes the interpolation stencil

    // *wdh* 091118 : now we can allow interpolation from ghost points.
        const int numValidGhost = numberOfValidGhostPoints>0 ? numberOfValidGhostPoints : 0;
        CanInterpolate::canInterpolate( cg, numberToCheck,cid, cir, numValidGhost );

        timeCanInterpolate+=getCPU()-timeb;

    //checkCanInterpolate(cg,receptor,donor, rv, interpolates, useBackupRules);  // This is a collective (parallel) operation

    // #ifdef USE_PPP
    //   Ogen::checkCanInterpolate(cg,receptor,donor, rv, interpolates, useBackupRules);
    // #else
    // 	cg.rcData->canInterpolate(receptor,donor, rv, interpolates, useBackupRules, checkForOneSided );
    // #endif

    // reset
        cg.interpolationWidth(Rx,receptor,donor,0)=widthOld;
        cg.interpolationOverlap(Rx,receptor,donor,0)=ovOld;

        if( debug & 4 )
        {
            if( numberToCheck>0 )
            {
      	fprintf(plogFile,"--- buildInterpolationInfo: attempt to interp from donor grid=%i : \n",donor);
      	for( int i=I.getBase(); i<=I.getBound(); i++ )
      	{
          // XA not valid here -- not compressed --
                    int iai=IA(i);
        	  fprintf(plogFile," pt %i interpolates=%i width=%i from donor=%i il=(%i,%i,%i) "
              		  " r=(%8.2e,%8.2e,%8.2e), x=(%8.2e,%8.2e,%8.2e)\n",
              		  iai,int(cir[i].width>0),cir[i].width,donor,cir[i].il[0],cir[i].il[1],cir[i].il[2],
              		  RA(i,0),RA(i,1),(numberOfDimensions==2 ? 0. : RA(i,2)),
              		  X(iai,0),X(iai,1),(numberOfDimensions==2 ? 0. : X(iai,2)));
          // *** TEMP ***
	  // printF("POGIG: pt %i interpolates=%i width=%i from donor=%i il=(%i,%i,%i) "
	// 	  " r=(%8.2e,%8.2e,%8.2e), x=(%8.2e,%8.2e,%8.2e)\n",
	// 	  iai,int(cir[i].width>0),cir[i].width,donor,cir[i].il[0],cir[i].il[1],cir[i].il[2],
	// 	  RA(i,0),RA(i,1),(numberOfDimensions==2 ? 0. : RA(i,2)),
	// 	  X(iai,0),X(iai,1),(numberOfDimensions==2 ? 0. : X(iai,2)));
      	}
            }
        }
        
    // *************** Assign points that could be interpolated  *****************
    // real rrv[3];
    // int iv[3];

        j=0;
        for( int i=0; i<numberToCheck; i++ )
        {
            int width = cir[i].width;   // interpolation width (=0 if invalid)

            if( width>STATUS(IA(i)) ) // Is this a better interpolation width ? 
            {
      	viw(j)=width;

        // 	for( int axis=0; axis<domainDimension; axis++ )
        // 	  rrv[axis]=RA(i,axis);
        // 	getInterpolationStencil( mg, interpolationWidth, rrv, iv );

      	for( int axis=0; axis<domainDimension; axis++ )
      	{
        	  CI(j,axis)=RA(i,axis); // rrv[axis];
        	  IP(j,axis)=cir[i].il[axis]; // iv[axis]; // lower left corner of the interpolation stencil
      	}
        // we need to check the quality of the result:
        //  smaller width than requested?
        //  extrapolated? <- will likely mean width=2 and CI outside [il..il+1]


	// STATUS(IA(i)) = insideUnitSquare ? interpolated : extrapolated;
	// STATUS(IA(i)) = interpolated;  // do this for now 
                if( debug & 2 && STATUS(IA(i)) > 0  )
      	{
                    fprintf(plogFile,"**** improved status point found, grid=%i, i=%i, old-width=%i, new-width=%i****\n",grid,i,
                            STATUS(IA(i)),width  );
      	}
      	
      	STATUS(IA(i)) = width;  // if staus==interpolationWidth then we have the best possible donor

      	IA(j)=IA(i);
      	j++;
            }
        }
        NUMBEROFINTERPOLATIONPOINTS(grid)=j;  // number of pts interpolated from "grid"

        delete [] cid;
        delete [] cir;

    } // end for grid
    
    


  // ************ TO-DO *******************
    
  // If we are requested to assign a value to ALL points then 
  //   Make a list of un-assigned points
  //   
  //   Find the nearest valid grid point --> then use zeroeth order extrapolation 
    if( assignAllPoints )
    {
    // Count the number of unassigned points
        int numNotAssigned=0;
        for( int i=0; i<numberOfPointsToInterpolate; i++ )
        {
            if( STATUS(i)==0 )
            {
      	numNotAssigned++;
            }
        }
        int totalNotAssigned=ParallelUtility::getSum(numNotAssigned);

        if( (debug & 1 || totalNotAssigned!=0) && numberOfPointsNotAssignedMessages<10 )
        {
            numberOfPointsNotAssignedMessages++;
            if( numberOfPointsNotAssignedMessages<10 )
            {
      	printF("InterpPtsOnAGrid:INFO: some points not assigned by interpolation. These are likely outside the domain. "
             	       "numberOfPointsToInterpolate=%i, totalNotAssigned=%i.\n",numberOfPointsToInterpolate,totalNotAssigned);
            }
            else
            {
      	printF("InterpPtsOnAGrid:INFO: Too many `some points not assigned by interpolation.' I am not printing anymore.\n");
            }
        	  
        }

        
        if( totalNotAssigned>0 )
        {
      // --- there are some unassigned points ---

            if( false )
            {  // ************ for testing return here ****************
      	return -totalNotAssigned;
            }
            

            Range R=numNotAssigned;
            
            IntegerArray ia0; // indirection array for un-assigned points 
      //      RealArray dista;  // keep track of the distance to the nearest grid point
            IntegerArray il;  // keeps [donor,i1,i2,i3] for the nearest grid point 
            RealArray xa,ra;
            if( numNotAssigned>0 )
            {
                ia0.redim(numNotAssigned);
                xa.redim(R,numberOfDimensions);
                ra.redim(R,numberOfDimensions);
                il.redim(R,numberOfDimensions+1);
        //        dista.redim(R); dista=REAL_MAX;
            }
            int *ia0p= ia0.Array_Descriptor.Array_View_Pointer0;
            #define IA0(i0) ia0p[i0]

            real *rap = ra.Array_Descriptor.Array_View_Pointer1;
            const int raDim0=ra.getRawDataSize(0);
            #define RA(i0,i1) rap[i0+raDim0*(i1)]

            real *xap = xa.Array_Descriptor.Array_View_Pointer1;
            const int xaDim0=xa.getRawDataSize(0);
            #define XA(i0,i1) xap[i0+xaDim0*(i1)]

            int *ilp = il.Array_Descriptor.Array_View_Pointer1;
            const int ilDim0=il.getRawDataSize(0);
            #define IL(i0,i1) ilp[i0+ilDim0*(i1)]

//       real *pdista=dista.Array_Descriptor.Array_View_Pointer0;
//       #define DISTA(i0) pdista[i0]

            if( numNotAssigned>0 )
            {
	// -- make a list of any un-assigned pts --
                int j=0;
      	for( int i=0; i<numberOfPointsToInterpolate; i++ )
      	{
        	  if( STATUS(i)==0 )
        	  {
          	    IA0(j)=i; 
                        for( int axis=0; axis<numberOfDimensions; axis++ )
            	      XA(j,axis)=X(i,axis);
          	    j++;
        	  }
      	}
      	assert( j==numNotAssigned );
            }


      // *new way* 
      // Find the nearest valid grid point (in parallel too)

            findNearestValidGridPoint( cg, xa, il, ra );


//       // ==== This next section could be its own "parallel find nearest point" function ===
//       #ifdef USE_PPP
//         // parallel: see inverseMap.C for the parallel inverseMapS



//         // In parallel we should have a routine similiar to the inversMapS in inverseMap.C for
//         // finding the closest point.

//       // --- For now we do the following: ----

//       if( numNotAssigned<numberOfPointsToInterpolate )
//       {
// 	OV_ABORT("IPOG:ERROR: finish me ");
//       }



//       #endif


//       real dx[3],xab[2][3];
//       int iv0[3]={0,0,0};

//       // --- Find the "closest" point to extrapolate from ---
//       for( int grid=numberOfComponentGrids-1; grid>=0; grid-- )
//       {

//         MappedGrid & mg = cg[grid];
// 	Mapping & map = mg.mapping().getMapping();
//         const IntegerArray & gridIndexRange = mg.gridIndexRange();
//         const RealArray & dr = mg.gridSpacing();

//         // TODO: check dist to bounding box and compare to dista
//         // Make a sublist of points to check ...



// 	const bool isRectangular=mg.isRectangular();
// 	if( !isRectangular )
// 	  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
// 	if( isRectangular )
// 	{
// 	  mg.getRectangularGridParameters( dx, xab );
// 	  iv0[0]=mg.gridIndexRange(0,0);
// 	  iv0[1]=mg.gridIndexRange(0,1);
// 	  iv0[2]=mg.gridIndexRange(0,2);
// 	}

//         // Here are the grid points for rectangular grids:
//         #define COORD(i0,i1,i2,axis) (xab[0][axis]+dx[axis]*(i0-iv0[axis]))

//         #ifdef USE_PPP
//           intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
// 	  realSerialArray center; if( !isRectangular ){ getLocalArrayWithGhostBoundaries(mg.center(),center);} //
//         #else
//           const intSerialArray & mask = mg.mask();
// 	  const realSerialArray & center = mg.center();
//         #endif

// 	const real *centerp = center.Array_Descriptor.Array_View_Pointer3;
// 	const int centerDim0=center.getRawDataSize(0);
// 	const int centerDim1=center.getRawDataSize(1);
// 	const int centerDim2=center.getRawDataSize(2);
// #define CENTER(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]

// 	const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
// 	const int maskDim0=mask.getRawDataSize(0);
// 	const int maskDim1=mask.getRawDataSize(1);
// #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]


//         // Do not check the bounding box when finding the nearest grid point:
//         map.setParameter( MappingParameters::THEfindBestGuess,true );
      	
//         // we really want the nearest VALID grid point (mask!=0) and the distance to it.
//         // This is not yet parallel: *** FIX ME FOR PARALLEL **
//         map.approximateGlobalInverse->findNearestGridPoint(0,numNotAssigned-1,xa,ra );
      	
// 	real rv[3]={0.,0.,0.}, xv[3]={0.,0.,0.};
// 	int iv[3]={0,0,0};
//         for( int j=0; j<numNotAssigned; j++ )
// 	{
//           // nearest point:
// 	  for( int axis=0; axis<numberOfDimensions; axis++ )
// 	  {
// 	    iv[axis]=int( ra(j,axis)/dr(axis)+gridIndexRange(0,axis) +.5 ); // nearest grid point
//             // limit the location to valid values: 
//             iv[axis]=max( gridIndexRange(0,axis)-numberOfValidGhostPoints,
//                      min( gridIndexRange(1,axis)+numberOfValidGhostPoints,iv[axis]));
//             iv[axis]=max( mask.getBase(axis),min( mask.getBound(axis), iv[axis] ));
// 	  }
        	  
// 	  if( debug & 2 )
// 	  {
// 	    fprintf(plogFile,"Check unassigned pt:=%i x=(%8.2e,%8.2e) donor=%i closest pt=(%i,%i) mask=%i :",
// 		    j,XA(j,0),XA(j,1),grid,iv[0],iv[1],MASK(iv[0],iv[1],iv[2]));
// 	  }
        	  

// 	  // check the mask on the nearest point
//           if( MASK(iv[0],iv[1],iv[2])!=0 ) // *** FIX ME FOR PARALLEL **
// 	  {
//             // we can extrapolate from this valid point
//             real dist=0;
// 	    for( int axis=0; axis<numberOfDimensions; axis++ )
// 	    {
//               if( isRectangular )
// 	        xv[axis]=COORD(iv[0],iv[1],iv[2],axis);
//               else
//                 xv[axis]=CENTER(iv[0],iv[1],iv[2],axis);
//               dist+= SQR( xv[axis]-XA(j,axis) );
// 	    }
//             if( dist< dista(j) )
// 	    {
//               // save this point
//               STATUS(IA0(j))=extrapolated;  // this point can now be extrapolated
            	      
//               dista(j)=dist;  // current smallest distance
//               // save donor, il(j,axis)=iv[axis];
//               IL(j,0)=grid;   // donor grid 
//               for( int axis=0; axis<numberOfDimensions; axis++ )
// 		IL(j,axis+1)=iv[axis];
            	      
// 	      if( debug & 2 )
// 		fprintf(plogFile," ... can extrap, dist=%8.2e \n",sqrt(dist));

// 	    }
// 	    else
// 	    {
// 	      if( debug & 2 )
// 		fprintf(plogFile," ... dist=%8.2e > current dist=%8.2e .\n",sqrt(dist),sqrt(dista(j)));
// 	    }
          	    
// 	  }
//           else
// 	  {
//             // The nearest point has mask==0 : we could look nearby for a pt with mask!=0 
// 	      if( debug & 2 )
// 		fprintf(plogFile," ... CANNOT extrap\n");
// 	  }
// 	}

//         // reset:
//         map.setParameter( MappingParameters::THEfindBestGuess,false );
      	
//       } // end for grid

            
            if( numNotAssigned>0 )
            {
        // Count up the number of extrapolate points for each grid so we can
        // allocate extra space if needed.
                int *numExtra = new int [numberOfComponentGrids];
      	for( int grid=0; grid<numberOfComponentGrids; grid++ )
        	  numExtra[grid]=0;
  
      	for( int j=0; j<numNotAssigned; j++ )
      	{
                    int iaj = IA0(j);
        	  if( RA(j,0)==Mapping::bogus ) 
        	  {
	    // this point is still NOT assigned -- this should be unlikely to happen
                        OV_ABORT("*** IPOG: Failure to find a closest grid point! This should not happen! ***");
        	  }

                    STATUS(iaj)=extrapolated;  // this point can now be extrapolated

        	  const int donor=IL(j,0);
        	  assert( donor>=0 && donor <numberOfComponentGrids );
        	  numExtra[donor]++;

      	}
      	for( int grid=0; grid<numberOfComponentGrids; grid++ )
      	{
            	  IntegerArray & viw = variableInterpolationWidth[grid];
                        int numNew=NUMBEROFINTERPOLATIONPOINTS(grid)+numExtra[grid];
        	  if( numNew > viw.getBound(0) )
        	  {
	    // increase array sizes : indirection[grid], variableInterpolationWidth[grid], ...
          	    if( false )
            	      printF("IPOG: numNotAssigned=%i, donor=%i, numInterp=%i, numExtra=%i, viw.getBound(0)=%i\n",
                 		     numNotAssigned,grid,NUMBEROFINTERPOLATIONPOINTS(grid),numExtra[grid],viw.getBound(0));
          	    
          	    IntegerArray & ia = indirection[grid];
          	    IntegerArray & ip  = interpolationLocation[grid];
          	    IntegerArray & viw = variableInterpolationWidth[grid];
          	    RealArray & ci = interpolationCoordinates[grid];

            // ia.resize(numNew);
	    // assert( ia.getLength(0)>=numNew );  // ia should be big enough already
                        if( ia.getLength(0)<numNew )
          	    {
                            ia.resize(numNew);  // *wdh* 100723
          	    }
          	    ip.resize(numNew,numberOfDimensions);
          	    viw.resize(numNew);
          	    ci.resize(numNew,numberOfDimensions);

	    // OV_ABORT("IPOG: increase array sizes for extrapolate points, finish me");
        	  }
        	  
      	}
      	delete [] numExtra;
            } // end if numNotAssigned>0
            

      // Now fill in the interpolation arrays:
            for( int grid=numberOfComponentGrids-1; grid>=0; grid-- )
            {
                IntegerArray & ia = indirection[grid];
      	int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]

      	IntegerArray & ip  = interpolationLocation[grid];
      	IntegerArray & viw = variableInterpolationWidth[grid];
      	int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
      	const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]
      	RealArray & ci = interpolationCoordinates[grid];
      	real *cip = ci.Array_Descriptor.Array_View_Pointer1;
      	const int ciDim0=ci.getRawDataSize(0);
#define CI(i0,i1) cip[i0+ciDim0*(i1)]

      	for( int j=0; j<numNotAssigned; j++ )
      	{
        	  const int donor=IL(j,0);
        	  if( donor==grid )
        	  {
          	    int & num = NUMBEROFINTERPOLATIONPOINTS(grid);
          	    assert( num<=viw.getBound(0) );

                        IA(num)=IA0(j);
          	    int width=1;  // extrap to order 1
          	    viw(num)=width;
          	    for( int axis=0; axis<domainDimension; axis++ )
          	    {
            	      CI(num,axis)=0.; // this value doesn't matter
            	      IP(num,axis)=IL(j,axis+1);
          	    }
          	    num++;

        	  }
      	}
            } // end for grid
        } // end if totalUnassigned>0
        
    } // end if assignAllPoints
    
    




    int numberInterpolated=0;
    int numberOfBackupInterpolated=0;
    int numberExtrapolated=0;
    int j=0;
    for( int i=0; i<numberOfPointsToInterpolate; i++ )
    {
    // printf(" DONE: i=%i status=%i\n",i,STATUS(i));

        if( STATUS(i)==interpolationWidth )
        {
            numberInterpolated++;
        }
        else if( STATUS(i)>1 )
        { // width=1 is counted as extrapolation -- check this --
            numberOfBackupInterpolated++;
        }
        else
        {
            if( STATUS(i)==extrapolated ) 
      	numberExtrapolated++;
            else
            {
      	if( infoLevel & 2 )
        	  printF("buildInterpolationInfo: WARNING: point not assigned: i=%i xv=(%9.3e,%9.3e,%9.3e)\n",
             		 i,x(i,0),x(i,1),(numberOfDimensions==2 ? 0 : x(i,2)));
      	
      	if( debug )
        	  fprintf(plogFile,"buildInterpolationInfo: WARNING: point not assigned: i=%i xv=(%9.3e,%9.3e,%9.3e)\n",
             		 i,x(i,0),x(i,1),(numberOfDimensions==2 ? 0 : x(i,2)));
      	
            }
            
      	
      // IA0(j)=i;
      // j++;
        }
    }
    int numNotAssigned=numberOfPointsToInterpolate -numberInterpolated -numberOfBackupInterpolated -numberExtrapolated;
        
    if( infoLevel & 1 )
    {
        int numInterpolated= ParallelUtility::getSum( numberInterpolated);
        int numBackup      = ParallelUtility::getSum( numberOfBackupInterpolated );
        int numExtrap      = ParallelUtility::getSum( numberExtrapolated );
        int numNot         = ParallelUtility::getSum( numNotAssigned );
        printF("InterpolatePointsOnAGrid::buildInterpolationInfo: total interpolated=%i, backup=%i, extrapolated=%i "
                      "not assigned=%i\n",
         	   numInterpolated,numBackup,numExtrap,numNot);

        if( numNot>0 )  
            printF("InterpolatePointsOnAGrid::buildInterpolationInfo: WARNING: %i points not assigned!\n",numNotAssigned);
    }
    
    if( debug & 4 )
    {
        fprintf(plogFile,"InterpolatePointsOnAGrid::buildInterpolationInfo:SUMMARY:\n");
        for( int grid=numberOfComponentGrids-1; grid>=0; grid-- )
        {
            IntegerArray & ia = indirection[grid];
            int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]

            for( int i=0; i<numberOfInterpolationPoints(grid); i++ )
            {
                int iai=IA(i);
      	fprintf(plogFile," grid=%i, i=%i, IA(i)=%i x=(%8.2e,%8.2e,%8.2e) status=%i (=interp. width)\n",
            		grid,i,iai,x(iai,0),x(iai,1),(numberOfDimensions==2 ? 0. : x(iai,2)),STATUS(iai));
            }
            
        }
    }
    

  // set the values that are returned by the query functions getNumberBackupInterpolation() etc.
    numberOfBackupInterpolation=numberOfBackupInterpolated;
    numberOfExtrapolated=numberExtrapolated;
    numberOfInterpolated=numberInterpolated;
    numberOfUnassigned=numNotAssigned;

  // ===================================
    if( false )
    {
        fclose(logFile);
        fclose(plogFile);
        OV_ABORT("stop for now");
    }
  // ===================================

    if( plogFile!=NULL )
    {
        fflush(plogFile);
    }
    

  // Here is the new parallel interpolator: 
#ifdef USE_PPP

  // --- Now build a parallel communication schedule for interpolating the points ---
    parallelSetup( cg,x, numberOfInterpolationPoints, interpolationCoordinates, 
            		indirection,interpolationLocation,variableInterpolationWidth );

  // In parallel we can destroy the arrays interpolationCoordinates, indirection, etc. 
    delete [] indirection;              
    delete [] interpolationLocation;
    delete [] variableInterpolationWidth;
    delete [] interpolationCoordinates;
    indirection=NULL;
    interpolationLocation=NULL;
    interpolationCoordinates=NULL;
    variableInterpolationWidth=NULL;
    
#else

  // --- serial setup ---
    maxInterpolationWidth=interpolationWidth;
    npr=nps=1;
    initializeExplicitInterpolation( cg );

#endif

    if( debug )
    {
        time=getCPU()-time;
        time=ParallelUtility::getMaxValue(time);
        timeMap=ParallelUtility::getMaxValue(timeMap);
        timeCanInterpolate=ParallelUtility::getMaxValue(timeCanInterpolate);
        printF(" ++ InterpolatePointsOnAGrid: Time for buildInterpolationInfo =%8.2e(s) "
                      " (inverseMap=%8.2e(s), canInterp=%8.2e(s) ) ++\n",
                      time,timeMap,timeCanInterpolate);
        fflush(stdout);
    }


    returnValue=-numNotAssigned;
    return returnValue;
}

#undef DIMENSION
#undef INDEXRANGE
#undef GRIDSPACING
#undef gridIndexRange
#undef iRange
#undef X
#undef XA
#undef STATUS
#undef IA0
#undef IA
#undef RA
#undef IP
#undef IP1
#undef DR
#undef DRA



int InterpolatePointsOnAGrid::
interpolationCoefficients(const CompositeGrid &cg,
                    			  RealArray & uInterpolationCoeff )

//=======================================================================================================
/// \details 
///     Return the coefficients for the interpolation of a grid function u at some points in space. (kkc)
///     If interpolation
///     is not possible then extrapolate from the nearest grid point. The extrapolation is zero-order
///     so that the value is just set equal to the value from the boundary.
/// \param cg (input): interpolate values from this grid 
/// \param uInterpolationCoeff (output): uInterpolationCoeff(0:numberOfPointsToInterpolate-1, 2^numberOfDimensions)
///       interpolation coefficients
// ==========================================================================================================
{
  // See InterpolatePoints.C 
    OV_ABORT("InterpolatePointsOnAGrid::interpolationCoefficients: finish me!");

    return 0;

}

#undef IP
#undef IP1
#undef UG
#undef IA
#undef UINTERPOLATED


int InterpolatePointsOnAGrid::
interpolatePoints(const RealArray & positionToInterpolate,
                 		     const realCompositeGridFunction & u,
                 		     RealArray & uInterpolated, 
                 		     const Range & R0/* =nullRange */,           
                 		     const Range & R1/* =nullRange */,
                 		     const Range & R2/* =nullRange */,
                 		     const Range & R3/* =nullRange */,
                 		     const Range & R4/* =nullRange */ )
//=======================================================================================================
/// \details 
///     Given some points in space, determine the values of a grid function u. If interpolation
///     is not possible then extrapolate from the nearest grid point. The extrapolation is zero-order
///     so that the value is just set equal to the value from the boundary : IS this still true??
/// \param Note: The first time this function is called the interpolation schedule will be generated.
///       On subsequent calls it is assumes that the positionToInterpolate values do not change.
/// 
/// \param positionToInterpolate (input):
///      positionToInterpolate(0:numberOfPointsToInterpolate-1,0:numberOfDimensions-1) : (x,y[,z]) positions
///           to interpolate. The first dimension of this array determines how many points to interpolate.
/// \param u (input): interpolate values from this grid function
/// \param uInterpolated (output): uInterpolated(0:numberOfPointsToInterpolate-1,R0,R1,R2,R3,R4) : interpolated
///       values
/// \param R0,R1,...,R4 (input): interpolate these components of the grid function. R0 is the range of values for
///      the first component of u, R1 the values for the second component, etc. By default all components
///       of u are interpolated.
/// \param indexGuess (input/ouput): indexGuess(0:numberOfPointsToInterpolate-1,0:numberOfDimensions-1) : 
///     (i1,i2[,i3]) values for initial 
///         guess for searches. Not required by default.
/// \param interpoleeGrid(.) (input/output): interpoleeGrid(0:numberOfPointsToInterpolate-1) : try
///         this grid first. Not required by default. 
/// \param wasInterpolated(.) (output) : If provided as an argument, on output wasInterpolated(i)=TRUE if the point
///      was successfully interpolated, or wasInterpolated(i)=FALSE if the point was extrapolated.
/// \param Errors:  This routine in principle should always be able to interpolate or extrapolate.
/// \param Return Values:
///     <ul>
///       <li> 0 = success
///       <li> 1 = error, unable to interpolate (this should never happen)
///       <li> -N = could not interpolate N points, but could extrapolate -- extrapolation was performed
///          from the nearest grid point.
///     </ul>
/// \author: WDH
// =======================================================================================================
{

    int returnValue=0;
    CompositeGrid & cg = *u.getCompositeGrid();
    
    if( !interpolationIsInitialized )
    { // -- We only need to initialize once --
        buildInterpolationInfo(positionToInterpolate,cg );
    }
    
    returnValue=interpolatePoints(u,uInterpolated,R0,R1,R2,R3,R4);
    
    return returnValue;
    
}

#undef NRM
#undef MODR


int InterpolatePointsOnAGrid::
interpolateAllPoints(const realCompositeGridFunction & uFrom,
                 		     realCompositeGridFunction & uTo, 
                 		     const Range & componentsFrom /* =nullRange */, 
                 		     const Range & componentsTo /* =nullRange */,
                                          const int numberOfGhostPointsToInterpolate /* =interpolateAllGhostPoints */ )
//==============================================================================
/// 
/// \details 
///      Interpolate all values on one CompositeGridFunction, 'uTo',  
///    from the values of another CompositeGridFunction,
///    'uFrom'. Values on 'uTo' are extrapolated if they lie outside the region covered by 'uFrom'.
///    This routine calls the 'interpolatePoints' function.
/// \param uFrom (input):
///       Use these values to interpolate from.
/// \param uTo (output):
///       Fill in all values on this grid (including ghost-points).
/// \param componentsFrom (input) : interpolate these components from uFrom (by default interpolate all components)
/// \param componentsTo   (input) : interpolate these components to uTo
/// \param numberOfGhostPointsToInterpolate (input) : only interpolate this many ghost points (by default interpolate all)
/// \param Errors:  This routine in principle should always be able to interpolate or extrapolate all
///    values.
/// \param Return Values:
///      <ul>
///        <li> 0 = success
///        <li> 1 = error, unable to interpolate 
///        <li> -N = could not interpolate N points, but could extrapolate -- extrapolation was performed
///           from the nearest grid point.
///      </ul>
/// 
/// \author: WDH
/// 
//==============================================================================
{

    if( uFrom.numberOfGrids()==0 || uTo.numberOfGrids()==0 )
    {
        if( uFrom.numberOfGrids()==0 )
            printF("InterpolatePointsOnAGrid::interpolateAllPoints:ERROR: source grid function uFrom has NO GRIDS!\n");
        if( uTo.numberOfGrids()==0 )
            printF("InterpolatePointsOnAGrid::interpolateAllPoints:ERROR: target grid function uTo has NO GRIDS!\n");

        OV_ABORT("ERROR");
    }
    

    CompositeGrid & cgTo   = (CompositeGrid&) *uTo.gridCollection;
    CompositeGrid & cgFrom = (CompositeGrid&) *uFrom.gridCollection;
    const int numberOfComponentGridsTo=cgTo.numberOfComponentGrids();
    if( numberOfComponentGridsTo==0 || cgFrom.numberOfComponentGrids()==0 )
        return 0;

    Range C0 = componentsTo  ==nullRange ? Range(uTo.getComponentBase(0),uTo.getComponentBound(0)) : componentsTo;
    Range C1 = componentsFrom==nullRange ? Range(uFrom.getComponentBase(0),uFrom.getComponentBound(0)) : componentsFrom;

    if( C0.getLength()!=C1.getLength() )
    {
        printF("InterpolatePointsOnAGrid::interpolateAllPoints:ERROR: Trying to interpolate %i components "
                      "from uFrom to %i components in uTo\n"
                      "                                               These must be the same number of components!\n",
         	   C1.getLength(),C0.getLength());
        OV_ABORT("InterpolatePointsOnAGrid::interpolateAllPoints:ERROR");
    }

  // debug=3; // *********************

    const int myid=max(0,Communication_Manager::My_Process_Number);
    const int np=Communication_Manager::numberOfProcessors();

    if( !interpolationIsInitialized )
    {
    // --- Initialization Stage ---
        if( debug & 2 )
            printF("== InterpolatePointsOnAGrid::interpolateAllPoints:INFO: initialization stage debug=%i ==\n",debug);

    // -------------------------------------------------------------------------------------
    //  Make a list of points to interpolate. No need to interpolate points with mask==0
    // -------------------------------------------------------------------------------------

        pInterpAllIndirection = new IntegerArray[numberOfComponentGridsTo];  
        numToInterpolatePerGrid = new int [numberOfComponentGridsTo];

        totalNumToInterpolate=0;
        for( int grid=0; grid<cgTo.numberOfComponentGrids(); grid++)
        {
            MappedGrid & mg= cgTo[grid];
    
            mg.update(MappedGrid::THEmask);
    
#ifdef USE_PPP
            intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
#else
            const intSerialArray & mask = mg.mask();
#endif
        
            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            if( numberOfGhostPointsToInterpolate==interpolateAllGhostPoints )
            {
      	getIndex(mg.dimension(),I1,I2,I3);
            }
            else
            {
      	assert( numberOfGhostPointsToInterpolate>-20 && numberOfGhostPointsToInterpolate<20 );  // sanity check 
        
      	getIndex(extendedGridIndexRange(mg),I1,I2,I3,numberOfGhostPointsToInterpolate);
      	for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      	{ // limit bounds to the dimension array
        	  int ia=max(Iv[axis].getBase() ,mg.dimension(0,axis));
        	  int ib=min(Iv[axis].getBound(),mg.dimension(1,axis));
        	  if( ib<ia ) ib=ia;  // what should we do in this case? Just return ?
        	  Iv[axis]=Range(ia,ib);
      	}
            }
    
            int includeGhost=0;
            bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);

            intSerialArray &  ia = pInterpAllIndirection[grid];
            if( ok ) // there are pts on this processor
            {
      	const int count = I1.getLength()*I2.getLength()*I3.getLength();
      	ia.redim(count,3);
	// ia = (mask!=0).indexMap();   // interpolate pts with mask!=0 

      	int *iap = ia.Array_Descriptor.Array_View_Pointer1;
      	const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]

      	const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
      	const int maskDim0=mask.getRawDataSize(0);
      	const int maskDim1=mask.getRawDataSize(1);
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

	// Here is the list of points that we will try to interpolate: 
      	int i=0;
      	int i1,i2,i3;
      	FOR_3D(i1,i2,i3,I1,I2,I3)
      	{
        	  if( MASK(i1,i2,i3)!=0 )
        	  {
          	    IA(i,0)=i1; IA(i,1)=i2; IA(i,2)=i3;
          	    i++;
        	  }
      	}
      	numToInterpolatePerGrid[grid]=i;
            }
            else
            {
                numToInterpolatePerGrid[grid]=0;
      	ia.redim(0);
            }
            
            totalNumToInterpolate+=numToInterpolatePerGrid[grid];

            if( debug & 2 )
            {
      	printf("== myid=%i, interpolateAllPoints:INFO: interp %i pts from grid=%i\n",myid,numToInterpolatePerGrid[grid],grid);
            }
            
        } // end for grid 
    
        if( totalNumToInterpolate==0 )
        {
            delete [] pInterpAllIndirection; pInterpAllIndirection=NULL;
            delete [] numToInterpolatePerGrid; numToInterpolatePerGrid=NULL;
        }
        

        if( totalNumToInterpolate==0 )
        {
            printF("InterpolatePointsOnAGrid::interpolateAllPoints:WARNING: There are no points to interpolate!\n");
            return -1;
        }
        if( debug & 2 )
        {
            printf("== interpolateAllPoints:INFO: myid=%i : total num to interp = %i pts\n",myid,totalNumToInterpolate);
        }

    // --- Now we know how many pts will be interpolated from all grids ---
    //     We can allocate arrays to hold the points ---

        Range I=totalNumToInterpolate;
        RealArray x(I,cgTo.numberOfDimensions());
        
        real *xp = x.Array_Descriptor.Array_View_Pointer1;
        const int xDim0=x.getRawDataSize(0);
#define X(i0,i1) xp[i0+xDim0*(i1)]

        int k=0;  // index over all pts to interp
        for( int grid=0; grid<cgTo.numberOfComponentGrids(); grid++)
        {
            MappedGrid & mg= cgTo[grid];
            bool isRectangular=mg.isRectangular();
            if( !isRectangular )
      	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

            if( numToInterpolatePerGrid[grid]==0 ) continue;

            intSerialArray &  ia = pInterpAllIndirection[grid];
            int *iap = ia.Array_Descriptor.Array_View_Pointer1;
            const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]

            if( isRectangular )
            {
      	real dx[3],xab[2][3];
      	mg.getRectangularGridParameters( dx, xab );

      	const int i0a=mg.gridIndexRange(0,0);
      	const int i1a=mg.gridIndexRange(0,1);
      	const int i2a=mg.gridIndexRange(0,2);

      	const real xa=xab[0][0], dx0=dx[0];
      	const real ya=xab[0][1], dy0=dx[1];
      	const real za=xab[0][2], dz0=dx[2];
      	
#define COORD0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define COORD1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define COORD2(i0,i1,i2) (za+dz0*(i2-i2a))

      	if( mg.numberOfDimensions()==2 )
      	{
        	  for( int i=0; i<numToInterpolatePerGrid[grid]; i++ )
        	  {
          	    X(k,0)=COORD0(IA(i,0),IA(i,1),0);
          	    X(k,1)=COORD1(IA(i,0),IA(i,1),0);
          	    k++;
        	  }
      	}
      	else
      	{
        	  for( int i=0; i<numToInterpolatePerGrid[grid]; i++ )
        	  {
          	    X(k,0)=COORD0(IA(i,0),IA(i,1),IA(i,2));
          	    X(k,1)=COORD1(IA(i,0),IA(i,1),IA(i,2));
          	    X(k,2)=COORD2(IA(i,0),IA(i,1),IA(i,2));
          	    k++;
        	  }
      	}
            }
            else
            {
            
#ifdef USE_PPP
      	realSerialArray center; getLocalArrayWithGhostBoundaries(mg.center(),center);
#else
      	const realSerialArray & center = mg.center();
#endif

      	const real *centerp = center.Array_Descriptor.Array_View_Pointer3;
      	const int centerDim0=center.getRawDataSize(0);
      	const int centerDim1=center.getRawDataSize(1);
      	const int centerDim2=center.getRawDataSize(2);
#define CENTER(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]
      	const int i3=center.getBase(2);
      	if( mg.numberOfDimensions()==2 )
      	{
        	  for( int i=0; i<numToInterpolatePerGrid[grid]; i++ )
        	  {
          	    X(k,0)=CENTER(IA(i,0),IA(i,1),i3,0);
          	    X(k,1)=CENTER(IA(i,0),IA(i,1),i3,1);
          	    k++;
        	  }
      	}
      	else
      	{
        	  for( int i=0; i<numToInterpolatePerGrid[grid]; i++ )
        	  {
          	    X(k,0)=CENTER(IA(i,0),IA(i,1),IA(i,2),0);
          	    X(k,1)=CENTER(IA(i,0),IA(i,1),IA(i,2),1);
          	    X(k,2)=CENTER(IA(i,0),IA(i,1),IA(i,2),2);
          	    k++;
        	  }
      	}
            }
            assert( k<=totalNumToInterpolate );
            
      //     if( numToInterpolatePerGrid>26112 )   // ***********************************************
      //     {
      //       int i=26112;
      //       printF(" @@@ Interp: fill point i=%i IA=(%i,%i,%i) x=(%9.3e,%9.3e,%9.3e) \n",i,IA(i,0),IA(i,1),IA(i,2),
      // 	     X(i,0),X(i,1),X(i,2));
      //     }
            
        } // end for grid
        assert( k==totalNumToInterpolate );
        


    // Build the interpolate schedule: 
        buildInterpolationInfo(x,cgFrom);
        

    } // end initialization stage
    
    
    assert( interpolationIsInitialized );

    if( totalNumToInterpolate==0 )
    {
        printF("InterpolatePointsOnAGrid::interpolateAllPoints:WARNING: There are no points to interpolate!\n");
        return -1;
    }



  // ----- Interpolate the points --------------
    Range I=totalNumToInterpolate;
    RealArray uInterpolated(I,C1);

    if( debug & 2 )
        uInterpolated=-99.;
    
    interpolatePoints(uFrom ,uInterpolated, C1);

  // ------------------------------------------------------------
  // --- Copy interpolated values back into the grid function ---
  // ------------------------------------------------------------

    real *uInterpolatedp = uInterpolated.Array_Descriptor.Array_View_Pointer1;
    const int uInterpolatedDim0=uInterpolated.getRawDataSize(0);
#define UINTERPOLATED(i0,i1) uInterpolatedp[i0+uInterpolatedDim0*(i1)]

    int k=0;  // index over all pts to interp
    for( int grid=0; grid<cgTo.numberOfComponentGrids(); grid++)
    {
        if( numToInterpolatePerGrid[grid]>0 )
        {
            MappedGrid & mg= cgTo[grid];

            intSerialArray &  ia = pInterpAllIndirection[grid];
            int *iap = ia.Array_Descriptor.Array_View_Pointer1;
            const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]
            
#ifdef USE_PPP
            realSerialArray u; getLocalArrayWithGhostBoundaries(uTo[grid],u);
#else
            realSerialArray & u = uTo[grid];
#endif

            real *up = u.Array_Descriptor.Array_View_Pointer3;
            const int uDim0=u.getRawDataSize(0);
            const int uDim1=u.getRawDataSize(1);
            const int uDim2=u.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

            if( mg.numberOfDimensions()==2 )
            {
      	const int i3=u.getBase(2);
      	for( int i=0; i<numToInterpolatePerGrid[grid]; i++ )
      	{
        	  for( int c0=C0.getBase(), c1=C1.getBase(); c0<=C0.getBound(); c0++,c1++ ) 
        	  {
          	    U(IA(i,0),IA(i,1),i3,c0)=UINTERPOLATED(k,c1);
	    // if( debug & 2 )
	    //  printF(" -- assign interp: grid=%i u(%i,%i)=%8.2e (k=%i)\n",grid,IA(i,0),IA(i,1),UINTERPOLATED(k,c1),k);
        	  
        	  }
        	  k++;
      	}
            }
            else
            {
      	for( int i=0; i<numToInterpolatePerGrid[grid]; i++ )
      	{
        	  for( int c0=C0.getBase(), c1=C1.getBase(); c0<=C0.getBound(); c0++,c1++ ) 
          	    U(IA(i,0),IA(i,1),IA(i,2),c0)=UINTERPOLATED(k,c1);
        	  k++;
      	}
            
            }
        }

        uTo[grid].updateGhostBoundaries();
        uTo[grid].periodicUpdate();  // *wdh* 080324 

    } // end for grid
    assert( k==totalNumToInterpolate );

    return 0;

}



int InterpolatePointsOnAGrid::
interpolateAllPoints(const realCompositeGridFunction & uFrom,
                                          realMappedGridFunction & uTo, 
                 		     const Range & componentsFrom /* =nullRange */, 
                 		     const Range & componentsTo /* =nullRange */,
                                          const int numberOfGhostPointsToInterpolate /* =interpolateAllGhostPoints */ )
//==============================================================================
/// 
/// \brief Interpolate all values on a realMappedGridFunction.
/// 
/// \details 
///      Interpolate all values on a realMappedGridFunction, {\ff uTo},  
///    from the values of another CompositeGridFunction,
///    {\ff uFrom}. Values on {\ff uTo} are extrapolated if they lie outside the region covered by {\ff uFrom}.
///    This routine calls the {\ff interpolatePoints} function.
/// \param uFrom (input):
///       Use these values to interpolate from.
/// \param uTo (output):
///       Fill in all values on this grid (including ghost-points).
/// \param componentsFrom (input) : interpolate these components from uFrom (by default interpolate all components)
/// \param componentsTo   (input) : interpolate these components to uTo
/// \param numberOfGhostPointsToInterpolate (input) : only interpolate this many ghost points (by default interpolate all)
/// \param Errors:  This routine in principle should always be able to interpolate or extrapolate all
///    values.
/// \param Return Values:
///      <ul>
///        <li> 0 = success
///        <li> 1 = error, unable to interpolate 
///        <li> -N = could not interpolate N points, but could extrapolate -- extrapolation was performed
///           from the nearest grid point.
///      </ul>
/// 
/// \author WDH
/// 
//==============================================================================
{
    int numberOfExtrapolatedPoints=0;
    
    OV_ABORT("InterpolatePointsOnAGrid::interpolateAllPoints: finish me!");
    
    return numberOfExtrapolatedPoints;

}

#undef CENTER
#undef IA
#undef X
#undef UINTERPOLATED
#undef COORD0
#undef COORD1
#undef COORD2
#undef U




int InterpolatePointsOnAGrid::
interpolatePoints(const realCompositeGridFunction & u,
              		  RealArray & uInterpolated, 
              		  const Range & R0/* =nullRange */,           
              		  const Range & R1/* =nullRange */,
              		  const Range & R2/* =nullRange */,
              		  const Range & R3/* =nullRange */,
              		  const Range & R4/* =nullRange */ )
//=======================================================================================================
/// \brief Given some points in space, determine the values of a grid function u.
///
/// \details 
///     Given some points in space, determine the values of a grid function u. If interpolation
///     is not possible then extrapolate from the nearest grid point. The extrapolation is zero-order
///     so that the value is just set equal to the value from the boundary.
/// \param u (input): interpolate values from this grid function
/// \param uInterpolated (output): uInterpolated(0:numberOfPointsToInterpolate-1,R0,R1,R2,R3,R4) : interpolated
///       values
/// \param R0,R1,...,R4 (input): interpolate these components of the grid function. R0 is the range of values for
///      the first component of u, R1 the values for the second component, etc. By default all components
///       of u are interpolated.
// ==========================================================================================================
{

  // --- Here is the parallel version : ---
#ifdef USE_PPP
  // assert( pogip!=NULL );
    int returnValue = parallelInterpolate( uInterpolated,u,R0,R1,R2 );
    return returnValue;

#else

  // Here is the serial version 


  // ***************************************************
  // ****** Interpolate points given the stencil *******
  // ***************************************************
    CompositeGrid & cg = *u.getCompositeGrid();

    const int numberOfDimensions = cg.numberOfDimensions();
    const int numberOfComponentGrids = cg.numberOfComponentGrids();

    const int rangeDimension = numberOfDimensions;
    const int domainDimension = cg[0].domainDimension();

  // determine component ranges to use:
    Range Ra[5] = {R0,R1,R2,R3,R4};  
    int i;
    for( i=0; i<5; i++ )
    {
        if( Ra[i].length()<=0 ) //     if( Ra[i]==nullRange )
        {
      // Ra[i] = Range(u.getComponentBase(i),u.getComponentBound(i));  
      // *wdh* 050515 -- take bounds from uInterpolated
            Ra[i] = Range(uInterpolated.getBase(i+1),uInterpolated.getBound(i+1));  
        }
        if( Ra[i].getBase()<u.getComponentBase(i) || Ra[i].getBound()>u.getComponentBound(i) )
        {
            cout << "interpolatePoints:ERROR: the component Range R" << i << " is out of range! \n";
            printf("R%i =(%i,%i) but the dimensions for component %i of u are (%i,%i) \n",i,
           	     Ra[i].getBase(),Ra[i].getBound(),i,u.getComponentBase(i),u.getComponentBound(i));
            Overture::abort("error");
        }
        else if( i<3 && (Ra[i].getBase()<uInterpolated.getBase(i+1) || Ra[i].getBound()>uInterpolated.getBound(i+1)) )
        {
            cout << "interpolatePoints:ERROR: the component Range R" << i << " is out of range! \n";
            printf("R%i =(%i,%i) but the dimensions for index %i of uInterpolated are (%i,%i) \n",i,
           	     Ra[i].getBase(),Ra[i].getBound(),i+1,uInterpolated.getBase(i+1),uInterpolated.getBound(i+1));
            Overture::abort("error");
        }
    }

  // -- new way --
    int returnValue = internalInterpolate( uInterpolated,u,Ra[0],Ra[1],Ra[2] );

//   // We must check the highest priority grid first since this was the order the points were generated.
//   // A point may be extrapolated on a higher priority grid but then interpolated on a lower priority grid.
//   for( int grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
//   {
//     // interpolate from this grid.

//     const int num=numberOfInterpolationPoints(grid);
//     if( num>0 )
//     {
//       // printf("----interpolatePointsNew: interp %i points from grid %i\n",num,grid);

//       IntegerArray & ia = indirection[grid];
//       IntegerArray & ip = interpolationLocation[grid];
//       IntegerArray & ip1= interpolationLocationPlus[grid];
//       RealArray & dra = interpolationCoordinates[grid];
//       const IntegerArray & gid = cg[grid].gridIndexRange();
            
//       // display(ia,"ia");
//       // display(ip,"ip");
//       // display(dra,"dra");
            
//       #ifdef USE_PPP
//         realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
//       #else
//         const realSerialArray & ug = u[grid];
//       #endif

//       const int *iap = ia.Array_Descriptor.Array_View_Pointer0;
// #define IA(i0) iap[i0]

//       const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
//       const int ipDim0=ip.getRawDataSize(0);
// #define IP(i0,i1) ipp[i0+ipDim0*(i1)]
//       const int *ip1p = ip1.Array_Descriptor.Array_View_Pointer1;
//       const int ip1Dim0=ip1.getRawDataSize(0);
// #define IP1(i0,i1) ip1p[i0+ip1Dim0*(i1)]

//       const real *drap = dra.Array_Descriptor.Array_View_Pointer1;
//       const int draDim0=dra.getRawDataSize(0);
// #define DRA(i0,i1) drap[i0+draDim0*(i1)]

//       real *uInterpolatedp = uInterpolated.Array_Descriptor.Array_View_Pointer1;
//       const int uInterpolatedDim0=uInterpolated.getRawDataSize(0);
// #define UINTERPOLATED(i0,i1) uInterpolatedp[i0+uInterpolatedDim0*(i1)]


//       // ...........Bi-Linear Interpolation:
//       if( domainDimension==2 )
//       {
// 	const real *ugp = ug.Array_Descriptor.Array_View_Pointer2;
// 	const int ugDim0=ug.getRawDataSize(0);
// 	const int ugDim1=ug.getRawDataSize(1);
// #define UG(i0,i1,i2) ugp[i0+ugDim0*(i1+ugDim1*(i2))]

// 	for( int c0=Ra[0].getBase(); c0<=Ra[0].getBound(); c0++)  // *** add more components ****
// 	{
// 	  for( int i=0; i<num; i++ )
// 	  {
// 	    UINTERPOLATED(IA(i),c0)= 
// 	      (1.-DRA(i,1))*(
// 		(1.-DRA(i,0))*UG(IP (i,0),IP(i,1),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP(i,1),c0))
// 	      + DRA(i,1) *(
// 		(1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP1(i,1),c0));

// //  	    if( c0==3 )
// //  	    {
// //  	      printf(" grid=%i i=%i ia=%i dra=(%7.1e,%7.1e) ip=(%i,%i) ip1=(%i,%i) gid=%i,%i "
// //                       "u=(%9.3e,%9.3e,%9.3e,%9.3e) uI=%9.3e\n",
// //  		     grid,i,ia(i),dra(i,0),dra(i,1),
// //  		     ip(i,0),ip(i,1),ip1(i,0),ip1(i,1),
// //                       gid(0,0),gid(1,0),
// //                       UG(IP (i,0),IP(i,1),c0),UG(IP1(i,0),IP(i,1),c0),
// //                       UG( IP(i,0),IP1(i,1),c0),UG(IP1(i,0),IP1(i,1),c0),
// //                       uInterpolated(ia(i),c0));
// //  	    }

// 	  }
// 	}
//       }
//       else // 3D
//       {
// 	const real *ugp = ug.Array_Descriptor.Array_View_Pointer3;
// 	const int ugDim0=ug.getRawDataSize(0);
// 	const int ugDim1=ug.getRawDataSize(1);
// 	const int ugDim2=ug.getRawDataSize(2);
// #undef UG
// #define UG(i0,i1,i2,i3) ugp[i0+ugDim0*(i1+ugDim1*(i2+ugDim2*(i3)))]

// 	for( int c0=Ra[0].getBase(); c0<=Ra[0].getBound(); c0++)  // *** add more components ****
// 	{
// 	  for( int i=0; i<num; i++ )
// 	  {

// 	    UINTERPOLATED(IA(i),c0)= 
//               (1.-DRA(i,2))*(
//   	        (1.-DRA(i,1))*(
// 		  (1.-DRA(i,0))*UG(IP (i,0),IP(i,1),IP(i,2),c0)
// 		     +DRA(i,0) *UG(IP1(i,0),IP(i,1),IP(i,2),c0))
// 	          + DRA(i,1) *(
// 	  	  (1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),IP(i,2),c0)
// 		     +DRA(i,0) *UG(IP1(i,0),IP1(i,1),IP(i,2),c0))
// 		            )
//                  + DRA(i,2)*(
//   	        (1.-DRA(i,1))*(
// 	  	  (1.-DRA(i,0))*UG(IP (i,0),IP(i,1),IP1(i,2),c0)
// 		     +DRA(i,0) *UG(IP1(i,0),IP(i,1),IP1(i,2),c0))
// 	         + DRA(i,1) *(
// 	  	  (1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),IP1(i,2),c0)
// 		     +DRA(i,0) *UG(IP1(i,0),IP1(i,1),IP1(i,2),c0))
// 		           );

// // 	    if( i==26112 ) // IA(i)==30843 )
// // 	    {
// // 	      printF(" @@interp: i=%i ia=%i ip=(%i,%i,%i) ip1=(%i,%i,%i) dra=(%9.3e,%9.3e,%9.3e)\n",
// // 		     i,IA(i),IP (i,0),IP(i,1),IP(i,2),IP1(i,0),IP1(i,1),IP1(i,2),DRA(i,0),DRA(i,1),DRA(i,2));
// //               printF(" @@interp:  uInterp=%9.3e  u=[%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
// // 		     UINTERPOLATED(IA(i),c0), UG(IP (i,0),IP(i,1),IP(i,2),c0),UG(IP1(i,0),IP(i,1),IP(i,2),c0),
// // 		     UG( IP(i,0),IP1(i,1),IP(i,2),c0),UG(IP1(i,0),IP1(i,1),IP(i,2),c0),
// //                      UG(IP (i,0),IP(i,1),IP1(i,2),c0),UG(IP1(i,0),IP(i,1),IP1(i,2),c0),
// //                      UG( IP(i,0),IP1(i,1),IP1(i,2),c0),UG(IP1(i,0),IP1(i,1),IP1(i,2),c0));
// // 	    }

// 	  }
// 	}
//       }
//     } // end if num>0
        
//   } // end of grid
    
    return returnValue;

  #endif
}

#undef IP
#undef IP1
#undef UG
#undef IA
#undef UINTERPOLATED
