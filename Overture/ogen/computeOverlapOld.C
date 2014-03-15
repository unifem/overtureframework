#include "Overture.h"
#include "Ogen.h"
#include "PlotStuff.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>
#include "conversion.h"
#include "display.h"

int 
checkOverlappingGrid( const CompositeGrid & cg, const int & option=0 );

int Ogen::
computeOverlap(CompositeGrid & cg_, 
               CompositeGrid & cgOld,
               const int & level /* =0 */,
	       const bool & movingGrids /* =FALSE */, 
	       const IntegerArray & hasMoved /* = Overture::nullIntArray() */ )
// ==============================================================================================
// /Description:
//    Compute the overlap between grids.
// /level (input) : multigrid level to compute
// /Return value: 0=succuss, otherwise the number of errors encountered.
// ==============================================================================================
{
  assert( ps!=NULL );
  
  PlotStuff & gi = *ps;

  CompositeGrid & cg = level==0 ? cg_ : cg_.multigridLevel[level];

  if( info & 4 )
    printf("Find interpolation points on boundaries..\n");
  psp.set(GI_PLOT_INTERPOLATION_POINTS,TRUE);

  checkInterpolationOnBoundaries(cg);
  if( Ogen::debug & 1 )
  {
    // cg.numberOfInterpolationPoints.display("cg.numberOfInterpolationPoints");
    // for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    //  cg.interpolationPoint[grid].display("cg.interpolationPoint[grid]");
	
    plot( "After check interpolation on boundaries",cg );
  }

  if( info & 4 ) printf("cut holes...\n");
  if( holeCuttingOption==0 )
    numberOfHolePoints=cutHoles(cg);
  else
    numberOfHolePoints=cutHolesNew(cg);

  if( info & 4 ) printf("number Of hole points = %i \n",numberOfHolePoints);
  if( Ogen::debug & 1 )
  {
    if( holeCuttingOption==1 )
    {
      IntegerArray *iInterp = new IntegerArray [ cg.numberOfComponentGrids() ];  // **** fix this ****
      IntegerArray numberOfInterpolationPoints(cg.numberOfComponentGrids());
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	int dim=(cg[grid].indexRange(End,0)-cg[grid].indexRange(Start,0)+1)
	  *(cg[grid].indexRange(End,1)-cg[grid].indexRange(Start,1)+1)
	  *(cg[grid].indexRange(End,2)-cg[grid].indexRange(Start,2)+1);
	iInterp[grid].redim(dim,3);
      }
      generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );  // need iInterp
      delete [] iInterp;
    }
    
    plot( "After cut holes",cg);
  }
  
  if( holeCuttingOption==0 )
  {
    if( info & 4 ) printf("find the true boundary...");
    findTrueBoundary(cg);
    if( Ogen::debug & 1 )
      plot( "After find true boundaries",cg);
  }
  
  if( info & 4 ) printf("remove exterior points...\n");
  if( holeCuttingOption==0 )
    numberOfHolePoints=removeExteriorPoints(cg,TRUE);
  else
    numberOfHolePoints=removeExteriorPointsNew(cg,TRUE);
  if( info & 4 ) printf("number of exterior points = %i \n",numberOfHolePoints);

  if( Ogen::debug & 1 )
    plot( "After remove exterior points",cg);
      
  numberOfHolePoints=0;
  int numberOfErrors=classifyPoints( cg,orphanPoint,numberOfOrphanPoints,level,cg_ );
  fflush(NULL);  // flush all output streams (including the log file).

  if( numberOfErrors!=0 )
  {
    printf("====================================================================\n"
	   " The overlap algorithm failed. Check the file ogen.log for more info\n"
	   "====================================================================\n");
    plot( "Overlap algorithm failed",cg);
    gi.stopReadingCommandFile();
  }
  else
  {
    if( debug & 1 || info & 4 ) printf("Checking validity of the overlapping grid...\n");
    numberOfErrors=checkOverlappingGrid(cg);
    totalTime=getCPU()-totalTime;
    if( numberOfErrors==0 )
    {
      if( debug & 1 || info & 4 ) printf("Overlapping grid is valid.\n");
    }
    else
    {
      printf("Checking validity of the overlapping grid, Grid is not valid! Number of errors=%i\n",numberOfErrors);
    }
    for( int outfile=0; outfile<=1; outfile++ )
    { // write this info to the checkFile and to stdout if appropriate
      FILE * file;
      if( outfile==0 )
	file=checkFile;
      else if( info & 2 )
	file=stdout;
      else
	break;
      Index I1,I2,I3;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid ++ )
      {
	getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);
	int numberOfDiscretizationPoints=sum(cg[grid].mask()(I1,I2,I3)>0);
	fprintf(file,"grid %3i: discretization points = %8i, interpolation points=%7i, name=%s\n",
	       grid,numberOfDiscretizationPoints,cg.numberOfInterpolationPoints(grid),
	       (const char *)cg[grid].mapping().getName(Mapping::mappingName));
      }
    }
    if( info & 2 )
    {
      // print statistics
      real timeSum=timeUpdateGeometry+timeInterpolateBoundaries+timeCutHoles
//       +timeFindTrueBoundary
       +timeRemoveExteriorPoints+timeImproperInterpolation+timeProperInterpolation
       +timeAllInterpolation+timeRemoveRedundant;
      printf("------------------------------------------------------------------------\n"
	     "                                                     cpu     percentage \n"
	     "update geometry.....................................%8.2e   %6.2f       \n"
	     "interpolate boundaries..............................%8.2e   %6.2f       \n"
	     "cut holes...........................................%8.2e   %6.2f       \n"
//	     "find true boundary..................................%8.2e   %6.2f       \n"
	     "remove exterior points..............................%8.2e   %6.2f       \n"
	     "improper interpolation..............................%8.2e   %6.2f       \n"
	     "proper interpolation................................%8.2e   %6.2f       \n"
	     "all interpolation...................................%8.2e   %6.2f       \n"
	     "remove redundant points.............................%8.2e   %6.2f       \n"
	     "sum of above........................................%8.2e   %6.2f       \n"
	     "total...............................................%8.2e   %6.2f       \n"
	     "------------------------------------------------------------------------\n",
	     timeUpdateGeometry,timeUpdateGeometry/totalTime*100.,
	     timeInterpolateBoundaries,timeInterpolateBoundaries/totalTime*100.,
	     timeCutHoles,timeCutHoles/totalTime*100.,
//	     timeFindTrueBoundary,timeFindTrueBoundary/totalTime*100.,
	     timeRemoveExteriorPoints,timeRemoveExteriorPoints/totalTime*100.,
	     timeImproperInterpolation,timeImproperInterpolation/totalTime*100.,
	     timeProperInterpolation,timeProperInterpolation/totalTime*100.,
	     timeAllInterpolation,timeAllInterpolation/totalTime*100.,
	     timeRemoveRedundant,timeRemoveRedundant/totalTime*100.,
	     timeSum,timeSum/totalTime*100.,
	     totalTime,totalTime/totalTime*100.);
    }
    else if( info & 1 )
      printf("Ogen::updateOverlap: Time to compute overlap = %8.2e (including update geometry=%8.2e)"
             " (full algorithm)\n",totalTime, totalTime+timeUpdateGeometry);
  }
  return numberOfErrors;
}


//\begin{>>ogenUpdateInclude.tex}{\subsubsection{Moving Grid updateOverlap}}
int Ogen::
updateOverlap(CompositeGrid & cg, 
	      CompositeGrid & cgOld, 
	      const LogicalArray & hasMoved, 
	      const MovingGridOption & option /* =useOptimalAlgorithm */ )
// ========================================================================================
// /Description:
//    Determine an overlapping grid when one or more grids has moved.
//   {\bf NOTE:} If the number of grid points changes then you should use the 
//   {\tt useFullAlgorithm} option.
// 
// /cg (input) : grid to update
// /cgOld (input) : for grids that have not moved, share data with this CompositeGrid.
// /hasMoved (input): specify which grids have moved with hasMoved(grid)=TRUE
// /option (input) : An option from one of:
// {\footnotesize
// \begin{verbatim}
//   enum MovingGridOption
//   {
//     useOptimalAlgorithm=0,
//     minimizeOverlap=1,
//     useFullAlgorithm
//   };
// \end{verbatim}
// }
//  The {\tt useOptimalAlgorithm} may result in the overlap increasing as the grid is moved.
// 
//\end{ogenUpdateInclude.tex}
// ========================================================================================
{
  info=1;
  real time0=getCPU();
  boundaryEps=2.*cgOld.epsilon();   // We can still interpolate from a grid provided we are this close (in r) to 0 or 1

  if( gridScale.getLength(0)<cg.numberOfComponentGrids() )
  {
    // compute gridScale: gridScale(grid) = maximum length of the bounding box for a grid
    gridScale.redim(cg.numberOfComponentGrids());
    // rBound holds the range for values for valid interpolation, larger than [0,1] if we interp. ghost pts.
    rBound.redim(2,3,cg.numberOfComponentGrids());
    // warnForSharedSides : TRUE if we have warned about possible shared sides not being marked properly
    warnForSharedSides.redim(cg.numberOfComponentGrids(),6,cg.numberOfComponentGrids(),6);
    warnForSharedSides=FALSE;
  }

  bool sameNumberOfGridPoints=TRUE;
  Range Rx(0,cg.numberOfDimensions()-1);
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & g = cg[grid];
    // rBound holds the range for values for valid interpolation, larger than [0,1] if we interp. ghost pts.
    for( int axis=0; axis<3; axis++ )
    {
      rBound(Start,axis,grid)=   (g.extendedIndexRange(Start,axis)-g.indexRange(Start,axis))*g.gridSpacing(axis) -.01;
      rBound(End  ,axis,grid)=1.+(g.extendedIndexRange(End  ,axis)-g.indexRange(End  ,axis))*g.gridSpacing(axis) +.01;
    }

    (g.indexRange()-cgOld[grid].indexRange()).display("g.indexRange()-cgOld[grid].indexRange()");
    
    if( max(abs(g.indexRange()-cgOld[grid].indexRange()))!=0 ||
        max(abs(g.gridIndexRange()-cgOld[grid].gridIndexRange()))!=0 ||
        max(abs(g.extendedIndexRange()-cgOld[grid].extendedIndexRange()))!=0 )
    {
      sameNumberOfGridPoints=FALSE;
    }
    

    if( hasMoved(grid) )
    {
      g.geometryHasChanged(~MappedGrid::THEmask);
      g.update( MappedGrid::THEcenter               |
		MappedGrid::THEvertex               |   // we need this even for cell centred grids
		MappedGrid::THEvertexDerivative     |   // This was needed to prevent an error in 3D with ghost=2
		MappedGrid::THEcenterDerivative     |   // This was needed to prevent an error in 3D with ghost=2
		MappedGrid::THEvertexBoundaryNormal |
		MappedGrid::THEboundingBox      );  
    }
    else
    {
      // share data
      g.update(cgOld[grid],
	       MappedGrid::THEcenter               |
	       MappedGrid::THEvertex               |   // we need this even for cell centred grids
	       MappedGrid::THEvertexDerivative     |   // This was needed to prevent an error in 3D with ghost=2
	       MappedGrid::THEcenterDerivative     |   // This was needed to prevent an error in 3D with ghost=2
	       MappedGrid::THEvertexBoundaryNormal |
	       MappedGrid::THEboundingBox      );  
    }
  }
  cg->computedGeometry |=    CompositeGrid::THEmask;

  // ***** First we check to see if the old interpolation points are valid ********

  timeUpdateGeometry=getCPU()-time0;
  totalTime=getCPU()-timeUpdateGeometry;

  if( option==useOptimalAlgorithm && sameNumberOfGridPoints ) 
  {
    //      Resize the interpolation data based on the old grid.
    cg.numberOfInterpolationPoints = cgOld.numberOfInterpolationPoints;
    cg.update(
      CompositeGrid::THEmask                     |
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpoleeGrid           |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpolationPoint       |
      // THEinterpolationCondition   |
      // theLists                    ,
      CompositeGrid::COMPUTEnothing              );

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & g = cg[grid];
      MappedGrid & gOld = cgOld[grid];
      //    Copy the mask from the old grid.
      g.mask() = gOld.mask();
      // g.mask().display("mask");
    
      g->computedGeometry |= CompositeGrid::THEmask;
      // Copy the interpolation data from the old grid.
      if( cg.numberOfInterpolationPoints(grid) )
      {
	cg.interpolationCoordinates[grid] = cgOld.interpolationCoordinates[grid];
	cg.interpoleeGrid[grid] =           cgOld.interpoleeGrid[grid];
	cg.interpoleePoint[grid] =          cgOld.interpoleePoint[grid];
	cg.interpoleeLocation[grid] =       cgOld.interpoleeLocation[grid];
	cg.interpolationPoint[grid] =       cgOld.interpolationPoint[grid];
      }
    }
    //      The interpolationPoint and interpoleeGrid in c
    //      should be marked up-to-date if they were in c0.
    cg->computedGeometry &= ~(
      CompositeGrid::THEinterpolationPoint          |
      CompositeGrid::THEinterpoleeGrid              );
    cg->computedGeometry |= cgOld.computedGeometry() & (
      CompositeGrid::THEinterpolationPoint          |
      CompositeGrid::THEinterpoleeGrid              );

    //      Try to recompute the rest of the interpolation data from
    //      interpoleeGrid and interpolationPoint.
/* -----
  Integer update = cg.update(
    CompositeGrid::THEinterpolationCoordinates |
    CompositeGrid::THEinterpoleeLocation       |
    // theLists                    ,
    CompositeGrid::COMPUTEgeometry             );
---- */
    bool ok=TRUE;
    checkForOneSided=TRUE;
    Range Axes(0,cg.numberOfDimensions()-1);
    const int maxInterp = max(cg.numberOfInterpolationPoints);
    RealArray r(maxInterp,cg.numberOfDimensions()), x(maxInterp,cg.numberOfDimensions());
    IntegerArray ia(maxInterp,cg.numberOfDimensions()), useBackupRules(maxInterp);
    LogicalArray interpolates(maxInterp);
    useBackupRules=FALSE;
  
  // cg.interpolationOverlap.display("cg.interpolationOverlap");
  
    int axis;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];

      IntegerArray & mask = c.mask();
      // IntegerArray & inverseGrid = cg.inverseGrid[grid];
      // RealArray & rI = cg.inverseCoordinates[grid];   // may not exist

      const RealArray & center = c.center();
      IntegerArray & interpolationPoint = cg.interpolationPoint[grid];
      IntegerArray & interpoleeGrid = cg.interpoleeGrid[grid];
      IntegerArray & interpoleeLocation = cg.interpoleeLocation[grid];
      RealArray & interpolationCoordinates = cg.interpolationCoordinates[grid];
    
      int startIndex, endIndex=0;
      const int & numberOfInterpolationPoints = cg.numberOfInterpolationPoints(grid);
      const int & i3 = c.extendedIndexRange(Start,axis3);
    
      for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
      {
	if( grid2==grid ) continue;
      
	// update points on c that interpolate from c2
	startIndex=endIndex;
	while( endIndex<numberOfInterpolationPoints && interpoleeGrid(endIndex)==grid2 )
	  endIndex++;

	if( endIndex>startIndex && (hasMoved(grid) || hasMoved(grid2)) )
	{
	  MappedGrid & c2 = cg[grid2];
	  // make a list (points are ordered by grid2
	  Range Ra(startIndex,endIndex-1);
	  Range R(0,endIndex-startIndex-1);  // base 0 version of Ra
	  ia(R,Axes)=interpolationPoint(Ra,Axes);
	
	  Mapping & map2 = c2.mapping().getMapping();

	  for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	  {
	    if( cg.numberOfDimensions()==2 )
	      x(R,axis)=center(ia(R,0),ia(R,1),i3,axis);
	    else
	      x(R,axis)=center(ia(R,0),ia(R,1),ia(R,2),axis);
	  }
	  if( info & 4 )
	    printf("Ogen: moving grid update: re-interpolate points (%i,%i) of grid %i from grid %i\n",startIndex,
		   endIndex,grid,grid2);
	
	  map2.inverseMap(x(R,Axes),r);
	  // r.display("Here are the inverse coordinates");
	  // cgOld.interpolationCoordinates[grid].display("cgOld.interpolationCoordinates");
	  // interpoleeLocation.display("interpoleeLocation");
	

	  interpolates(R)=TRUE;  // could do at the start

	  ok = cg.rcData->canInterpolate(grid,grid2, r(R,Axes), interpolates, useBackupRules, checkForOneSided );
	  if( !ok ) 
	  {
	    printf("Ogen: moving grid update: unable to re-interpolate grid %i from grid %i\n",grid,grid2);
	    // interpolates.display("interpolates?");
	  
	    break;
	  }
	  // fill-in the new interpolation values
	  interpolationCoordinates(Ra,Axes)=r(R,Axes);
	  // ... finish .....	
	}
      }
      if( !ok ) break;
    }
    

    if( ok )
    {
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];

	// Fill in the interpoleeLocation -- lower left corner of the stencil
	int l=0;  // multigrid level
	IntegerArray & interpoleeLoc = cg.interpoleeLocation[grid];
	const IntegerArray & interpoleeGrid = cg.interpoleeGrid[grid];
	const RealArray & interpolationCoord = cg.interpolationCoordinates[grid];
      
    
	// ** could vectorize this I think if we put gridSpacing into an array: spacing(grid,axis)   
	for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
	{
	  int grid2=interpoleeGrid(i);
	  const IntegerArray & interpolationWidth = cg.interpolationWidth(Axes,grid,grid2,l);
	  MappedGrid & g2 = cg[grid2];
	  for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	  {
	    // Get the lower-left corner of the interpolation cube.
	    int intLoc=int(floor(interpolationCoord(i,axis)/g2.gridSpacing()(axis) + g2.indexRange()(0,axis) -
				 .5 * interpolationWidth(axis) + (g2.isCellCentered()(axis) ? .5 : 1.)));
	    if (!g2.isPeriodic()(axis)) 
	    {
	      if( (intLoc < g2.extendedIndexRange()(0,axis)) && (g2.boundaryCondition()(Start,axis)>0) )
	      {
		//                        Point is close to a BC side.
		//                        One-sided interpolation used.
		intLoc = g2.extendedIndexRange()(0,axis);
	      }
	      if( (intLoc + interpolationWidth(axis) - 1 > g2.extendedIndexRange()(1,axis))
		  && (g2.boundaryCondition()(End,axis)>0) )
	      {
		//                        Point is close to a BC side.
		//                        One-sided interpolation used.
		intLoc = g2.extendedIndexRange()(1,axis) - interpolationWidth(axis) + 1;
	      }
	    } // end if
	    interpoleeLoc(i,axis) = intLoc;
	  } 
	}
      }
    

      // printf("Ogen:updateOverlap for moving grids, using same interpolation points\n");
      if( info & 2 )
	printf(" time to update the geometry..............................%e (total=%e)\n",
	       timeUpdateGeometry,timeUpdateGeometry);

      totalTime=getCPU()-totalTime;
      if( info & 1 )
	printf("Ogen::updateOverlap: Time to compute overlap = %8.2e (including update geometry=%8.2e) "
	       "(using same interpolation points)\n",
	       totalTime, totalTime+timeUpdateGeometry);

      return 0;  // success
    }
    if( debug & 1 )
      printf("Ogen:updateOverlap for moving grids, resort to full algorithm...\n");
  }
  else
  {
    cg.update(
      CompositeGrid::THEmask                     |
      CompositeGrid::COMPUTEnothing              );
  }
  // ***** Use full algorithm *******  

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & g = cg[grid];
      
    // g.update(MappedGrid::THEmask, MappedGrid::COMPUTEnothing);

    // mask : 
    //  The lower order bits ( GRIDnumberBits = bits 0..22) conatin the preference for a point:
    //    either the grid we interpolate from or the grid number of the current grid.
    // By default all grids try to interpolate from the first preference:
    g.mask() = MappedGrid::ISdiscretizationPoint;  // **** should use highest priority

    g->computedGeometry |= MappedGrid::THEmask;

    gridScale(grid)=max(cg[grid].boundingBox()(End,Rx)-cg[grid].boundingBox()(Start,Rx));
    
  }



  // build the arrays inverseCoordinates, inverseGrid and inverseCondition to be used by the overlap algorithm
  cg.update(CompositeGrid::THEinverseMap, CompositeGrid::COMPUTEnothing);
  // cg[0].vertexDerivative().display("vertex derivative");
  // change the shape (orignally (nd,all,all,all)
  Range all;
  cg.inverseCoordinates.updateToMatchGrid(cg,all,all,all,cg.numberOfDimensions());
  
  cg.inverseCoordinates=0;
  cg.inverseGrid=-1;
   
  if( info & 2 && numberOfGridsHasChanged )
  {
   gridScale.display("Ogen::updateGeometry: Here is gridScale");
  }

  timeUpdateGeometry=getCPU()-time0;
  if( info & 2 )
    printf(" time to update the geometry..............................%e (total=%e)\n",
	   timeUpdateGeometry,timeUpdateGeometry);

  totalTime=getCPU()-timeUpdateGeometry;

  numberOfOrphanPoints=0;
  numberOfHolePoints=0;
  bool movingGrids=TRUE;
  bool firstTimeToComputeOverlap=FALSE;


/* ----
  cg->computedGeometry &= ~CompositeGrid::THEboundingBox;
  cg.update(
    CompositeGrid::THEmask               |
    CompositeGrid::THEcenter             |
    CompositeGrid::THEvertex             |
    CompositeGrid::THEcenterDerivative   |
    CompositeGrid::THEvertexDerivative   |
//    CompositeGrid::THEinverseMap         |
    CompositeGrid::THEboundingBox   );
//    CompositeGrid::theLists              );
---- */

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    // We need to recompute the bounding boxes -- we could optimize this for the MatrixTransform
    if( hasMoved(grid) && cg[grid].mapping().getMapping().approximateGlobalInverse!=NULL )
      cg[grid].mapping().getMapping().approximateGlobalInverse->reinitialize(); 
  }
  computeOverlap(cg,cgOld,0,movingGrids,hasMoved);

  return 0;
}





//\begin{>>ogenUpdateInclude.tex}{\subsubsection{updateRefinement : Adapative Grid updateOverlap}}
int Ogen::
updateRefinement(CompositeGrid & cg, 
                 const int & refinementLevel /* = -1 */ )
// ===================================================================================
// /Description:
//    Update the refinement levels. This is not completed yet.
//
// /refinementLevel (input): update this refinement level. By default update all refinement levels.
// /Notes:
//  A refinement grid prefers to interpolate from
//   \begin{enumerate}
//     \item Another refinement at the same level and same base grid
//     \item Another refinement at the same level and different base grid
//   \end{enumerate}
//\end{ogenUpdateInclude.tex}
// ===================================================================================
{
  // ***need to update the geometry***
  if( FALSE )
  {
    printf("Ogen::updateOverlap for refinementLevels, ERROR: not finished implementing yet\n");
    throw "error";
  }
  debug=7;

  cg.update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask );

  Range Rx(0,cg.numberOfDimensions()-1);
  const int & numberOfDimensions = cg.numberOfDimensions();

  Index Iv[3], &I1 = Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1 = Jv[0], &J2=Jv[1], &J3=Jv[2];
  Range Ivr[3], &I1r = Ivr[0], &I2r=Ivr[1], &I3r=Ivr[2];
  Range Ivb[3], &I1b = Ivb[0], &I2b=Ivb[1], &I3b=Ivb[2];
      
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
  int ie[3], &ie1=ie[0], &ie2=ie[1], &ie3=ie[2];
  int iBase[3],iBound[3];

  RealArray rr(1,3),xx(1,3);
  LogicalArray interpolates(1), useBackupRules(1);
  useBackupRules=FALSE;
  const int notAssigned = INT_MIN;
  const int mgLevel=0;  // *** multigrid level
  
  // 
  // If checkForOneSided=TRUE then canInterpolate will not allow a one-sided interpolation
  // stencil to use ANY interiorBoundaryPoint's -- this is actually too strict. We really
  // only want to disallow interpolation that has less than the minimum overlap distance
  //
  checkForOneSided=FALSE;  
  int axis,dir;
  int l;

  // allocate temporary arrays to hold the new interpolation points on the refinement level.
  IntegerArray **interpolationPoints = new IntegerArray* [cg.numberOfRefinementLevels()];



  for( l=1; l<cg.numberOfRefinementLevels(); l++ )
    interpolationPoints[l] = new IntegerArray [cg.refinementLevel[l].numberOfGrids()];
  
  for( l=1; l<cg.numberOfRefinementLevels(); l++ )
  {
    GridCollection & rl = cg.refinementLevel[l];
    // IntegerArray *interpolationPoints = new IntegerArray [rl.numberOfGrids()];
    
    int g;
    for( g=0; g<rl.numberOfGrids(); g++ )
    {
      int grid =rl.gridNumber(g);        // index into cg
      int bg = cg.baseGridNumber(grid);  // base grid for this refinement
      
      printf("updateOverlap(refinements): update level=%i, g=%i, grid=%i from base grid %i\n",l,g,grid,bg);

      MappedGrid & cr = rl[g];              // refined grid
      MappedGrid & cb = cg[bg];             // base grid
      const IntegerArray & maskb = cb.mask();
      IntegerArray & maskr = cr.mask();
      
      if( FALSE )
      {
        displayMask(maskb,"maskb");
        // displayMask(maskr,"maskr at start");
      }


      int rf[3];  // refinement factors
      rf[0]=rl.refinementFactor(0,g);
      rf[1]=rl.refinementFactor(1,g);
      rf[2]=rl.refinementFactor(2,g);
      
      assert( rf[0]>0 && rf[1]>0 && rf[2]>0 );
      // make a copy of the mask which is larger than the extended index range
      getIndex(cr.extendedIndexRange(),I1,I2,I3);
      J3=I3;
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	int base  = Iv[axis].getBase();
	int bound = Iv[axis].getBound();
	// note -1 % 2 == -1
        base = base%rf[axis] == 0 ? base-rf[axis] : base - abs(base % rf[axis]);
	bound= bound%rf[axis]==0 ? bound+rf[axis] : bound +rf[axis]-abs(bound % rf[axis]);
	assert( base%rf[axis] ==0 && bound%rf[axis] ==0 );

	Iv[axis]=Range(max(base,cb.dimension(Start,axis)*rf[axis]), min(bound,cb.dimension(End,axis)*rf[axis]));

        base =min(cr.dimension(Start,axis),base);
	bound=max(cr.dimension(End  ,axis),bound);
        Jv[axis]=Range(base,bound);
      }
      IntegerArray mask(J1,J2,J3);
      getIndex(cr.dimension(),J1,J2,J3);
      mask=0;
      mask(J1,J2,J3)=maskr(J1,J2,J3);

      // getIndex(cr.indexRange(),I1,I2,I3);
      printf("refinement level=%i, g=%i, indexRange=(%i,%i)X(%i,%i) Iv[0]=(%i,%i)\n",l,g,cr.indexRange()(0,0),
             cr.indexRange()(1,0),cr.indexRange()(0,1),cr.indexRange()(1,1),Iv[0].getBase(),Iv[0].getBound());
            

      for( axis=0; axis<3; axis++ )
      {
	Ivr[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound(),rf[axis]);
	Ivb[axis]=Range(Iv[axis].getBase()/rf[axis],
                        Iv[axis].getBound()/rf[axis]);
      }

/* -----
      // 
      //       X--.--X--.--X
      //       |  |  |  |  |
      //       .  0  0  0  .
      //       |  |  |  |  |
      //       X--0--0--0--X
      //       |  |  |  |  |
      //       .  0  0  0  .
      //       |  |  |  |  |
      //       X--.--X--.--X
      //
---- */
      if( FALSE && g==1 )
      {
        maskb.display("g=1, maskb");
        maskb(I1b,I2b,I3b).display("g=1, b(I1b,I2b,I3b)");
        mask(I1,I2,I3).display("g=1, mask after 1");
      }
      
      // special case if refinement aligns with the extendedIndexRange of a base grid
      // interpolation side.
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( cb.boundaryCondition(side,axis)==0 && 
	      cr.indexRange(side,axis)/rf[axis]==cb.extendedIndexRange(side,axis) )
	  {
	    getBoundaryIndex(cr.dimension(),side,axis,I1,I2,I3);
	    Iv[axis]=cr.indexRange(side,axis);

	    getBoundaryIndex(cr.dimension(),side,axis,J1,J2,J3);
	    for( int ghost=1; ghost<=cr.numberOfGhostPoints(side,axis); ghost++ )
	    {
	      Jv[axis]=cr.indexRange(side,axis)-ghost*(1-2*side);
	      mask(J1,J2,J3)=0;
	    }
	  }
	}
      }
      
      // ---- mark refinement holes that coincide with base grid holes. ----
      where( maskb(I1b,I2b,I3b)==0 )
      {
        mask(I1r,I2r,I3r)=0;
      }
      int r;

      // Now mark off refinement holes that lie directly between
      // base grid holes along each axis.
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
        // decrease ranges by "1" in the axis direction. -- needed if rf[axis]>number of ghost lines
  	Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()-rf[axis],rf[axis]);
  	Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()-1);

        const IntegerArray & maskb0 = evaluate(maskb(I1b,I2b,I3b)==0);
        Ivb[axis]=Ivb[axis]+1;
	where( maskb0 || maskb(I1b,I2b,I3b)==0 )
	{
	  for( r=1; r<rf[axis]; r++ )
	  {
            Ivr[axis]=Ivr[axis]+1;
	    mask(I1r,I2r,I3r)=0;
	  }
          Ivr[axis]=Ivr[axis]-(rf[axis]-1);
	}
        Ivb[axis]=Ivb[axis]-1;

        // reset    
	Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()+rf[axis],rf[axis]);
	Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()+1);
      }
      // Now mark off-axis points as holes if any corner of the cell is a hole.
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()-rf[axis],rf[axis]);
	Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()-1);
      }
      if( numberOfDimensions==2 )
      {
        const IntegerArray & maskb0 = evaluate(maskb(I1b,I2b,I3b)==0);
	where(  maskb0                   || (maskb(I1b+1,I2b  ,I3b)==0) ||
	       (maskb(I1b,I2b+1,I3b)==0) || (maskb(I1b+1,I2b+1,I3b)==0) )
	{
	  for( int r2=1; r2<rf[1]; r2++ )
	  {
	    for( int r1=1; r1<rf[0]; r1++ )
	    {
    	      mask(I1r+r1,I2r+r2,I3r)=0;
	    }
	  }
	}
      }
      else
      {
        Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()+rf[axis3],rf[axis3]);
	Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()+1);
	where( (maskb(I1b,I2b,I3b)==0)   || (maskb(I1b+1,I2b  ,I3b)==0) ||
	       (maskb(I1b,I2b+1,I3b)==0) || (maskb(I1b+1,I2b+1,I3b)==0) )
	{
	  for( int r2=1; r2<rf[1]; r2++ )
	  {
	    for( int r1=1; r1<rf[0]; r1++ )
	    {
    	      mask(I1r+r1,I2r+r2,I3r)=0;
	    }
	  }
	}
        Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()-rf[axis3],rf[axis3]);
	Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()-1);

        Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()+rf[axis1],rf[axis1]);
	Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()+1);
	where( (maskb(I1b,I2b,I3b)==0)   || (maskb(I1b  ,I2b+1,I3b)==0) ||
	       (maskb(I1b,I2b,I3b+1)==0) || (maskb(I1b  ,I2b+1,I3b+1)==0) )
	{
	  for( int r3=1; r3<rf[2]; r3++ )
	  {
	    for( int r2=1; r2<rf[1]; r2++ )
	    {
    	      mask(I1r,I2r+r2,I3r+r3)=0;
	    }
	  }
	}
        Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()-rf[axis1],rf[axis1]);
	Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()-1);

        Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()+rf[axis2],rf[axis2]);
	Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()+1);
	where( (maskb(I1b,I2b,I3b)==0)   || (maskb(I1b+1,I2b,I3b)==0) ||
	       (maskb(I1b,I2b,I3b+1)==0) || (maskb(I1b+1,I2b,I3b+1)==0) )
	{
	  for( int r3=1; r3<rf[2]; r3++ )
	  {
	    for( int r1=1; r1<rf[0]; r1++ )
	    {
    	      mask(I1r+r1,I2r,I3r+r3)=0;
	    }
	  }
	}
        Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()-rf[axis2],rf[axis2]);
	Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()-1);


	where( (maskb(I1b,I2b,I3b)==0)       || (maskb(I1b+1,I2b  ,I3b  )==0) ||
	       (maskb(I1b  ,I2b+1,I3b  )==0) || (maskb(I1b+1,I2b+1,I3b  )==0) ||
	       (maskb(I1b  ,I2b  ,I3b+1)==0) || (maskb(I1b+1,I2b  ,I3b+1)==0) ||
	       (maskb(I1b  ,I2b+1,I3b+1)==0) || (maskb(I1b+1,I2b+1,I3b+1)==0) )
	{
	  for( int r3=1; r3<rf[2]; r3++ )
	  {
	    for( int r2=1; r2<rf[1]; r2++ )
	    {
	      for( int r1=1; r1<rf[0]; r1++ )
	      {
		mask(I1r+r1,I2r+r2,I3r+r3)=0;
	      }
	    }
	  }
	}
      }

      // Finally mark extra ghost line values of the refinement.
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
          getBoundaryIndex(cr.dimension(),side,axis,I1,I2,I3);
          Iv[axis]=cr.indexRange(side,axis);

          getBoundaryIndex(cr.dimension(),side,axis,J1,J2,J3);
          for( int ghost=2; ghost<=cr.numberOfGhostPoints(side,axis); ghost++ )
	  {
            Jv[axis]=cr.indexRange(side,axis)-ghost*(1-2*side);
            // special case if refinement aligns with the extendedIndexRange of a base grid
            // interpolation side.
	    where( mask(I1,I2,I3)==0 )
	    {
	      mask(J1,J2,J3)=0;
	    }
	    elsewhere( mask(J1,J2,J3)!=0 )  // *wdh* 981017
	    {
	      mask(J1,J2,J3)=MappedGrid::ISghostPoint;
	    }
	  }
	}
      }

      
/* -----
      // only zero middle points with 2 or more 0 neighbours
      //        X---O   O---O
      //        | x |   | o |
      //        X---X   X---X
      where( ( (maskb(I1b,I2b  ,I3b)==0) + (maskb(I1b+1,I2b  ,I3b)==0) +
	       (maskb(I1b,I2b+1,I3b)==0) + (maskb(I1b+1,I2b+1,I3b)==0) ) > 1 )  
      {
	mask(I1r+1,I2r+1,I3r)=0;
      }
------- */
      
      // mark interpolation points on the refinement
      IntegerArray & ip = interpolationPoints[l][g];
      ip.redim(cg.numberOfInterpolationPoints(bg)*(max(rf[0],rf[1],rf[2])+1)+100,3);

      getIndex(cr.indexRange(),I1,I2,I3);

      IntegerArray maskI(I1,I2,I3);
      if( max(abs(cr.discretizationWidth()(Rx)-3))==0 )
      {
        if( numberOfDimensions==2 )
	{
	  maskI = mask(I1,I2,I3)>0 && 
            (mask(I1-1,I2-1,I3)==0 || mask(I1,I2-1,I3)==0 || mask(I1+1,I2-1,I3)==0 ||
             mask(I1-1,I2  ,I3)==0 ||                         mask(I1+1,I2  ,I3)==0 ||
             mask(I1-1,I2+1,I3)==0 || mask(I1,I2+1,I3)==0 || mask(I1+1,I2+1,I3)==0 );
	}
	else
	{
	  maskI = mask(I1,I2,I3)>0 &&
            (mask(I1-1,I2-1,I3-1)==0 || mask(I1,I2-1,I3-1)==0 || mask(I1+1,I2-1,I3-1)==0 ||
             mask(I1-1,I2  ,I3-1)==0 || mask(I1,I2  ,I3-1)==0 || mask(I1+1,I2  ,I3-1)==0 ||
             mask(I1-1,I2+1,I3-1)==0 || mask(I1,I2+1,I3-1)==0 || mask(I1+1,I2+1,I3-1)==0 ||

             mask(I1-1,I2-1,I3  )==0 || mask(I1,I2-1,I3  )==0 || mask(I1+1,I2-1,I3  )==0 ||
             mask(I1-1,I2  ,I3  )==0 ||                           mask(I1+1,I2  ,I3  )==0 ||
             mask(I1-1,I2+1,I3  )==0 || mask(I1,I2+1,I3  )==0 || mask(I1+1,I2+1,I3  )==0 ||

             mask(I1-1,I2-1,I3+1)==0 || mask(I1,I2-1,I3+1)==0 || mask(I1+1,I2-1,I3+1)==0 ||
             mask(I1-1,I2  ,I3+1)==0 || mask(I1,I2  ,I3+1)==0 || mask(I1+1,I2  ,I3+1)==0 ||
             mask(I1-1,I2+1,I3+1)==0 || mask(I1,I2+1,I3+1)==0 || mask(I1+1,I2+1,I3+1)==0 );
	}
      }
      else
      {
        printf("updateRefinement:ERROR:sorry, not implemented yet for this discretizationWidth\n");
	cr.discretizationWidth().display("discretizationWidth");
	throw "error";
      }
      getIndex(cr.dimension(),J1,J2,J3);
      
      maskr(J1,J2,J3)=mask(J1,J2,J3);   // copy back to the original mask.

      if( FALSE )
        displayMask(maskr,"************ maskr after marking holes ***************");


      int numberOfInterpolationPoints=0;
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
        if( maskI(i1,i2,i3)>0 )
	{
          maskr(i1,i2,i3)=numberOfInterpolationPoints+1; 
          // make a list of overlapping grid style interpolation points
          for( axis=0; axis<3; axis++ )
  	    ip(numberOfInterpolationPoints,axis)=iv[axis];

          numberOfInterpolationPoints++;
	}
      }
      // throw away any unnecessary interpolation points.
      int dw[3];
      dw[0]=cr.discretizationWidth(axis1)/2;
      dw[1]=cr.discretizationWidth(axis2)/2;
      dw[2]=cr.discretizationWidth(axis3)/2;
      const bool ISneeded = MappedGrid::ISinterpolationPoint | MappedGrid::ISdiscretizationPoint;
      i3=cr.indexRange(Start,axis3);
      iBase[2]=iBound[2]=i3;
      
      int i, ii=0;
      for( i=0; i<numberOfInterpolationPoints; i++ )
      {
        bool pointIsNeeded=FALSE;
        for( axis=0; axis<3; axis++ )
	{
          iBase[axis] =max(cr.indexRange(Start,axis),ip(i,axis)-dw[axis]);
	  iBound[axis]=min(cr.indexRange(End,  axis),ip(i,axis)+dw[axis]);
	}
        for( int s3=iBase[2]; s3<=iBound[2]; s3++ )
        for( int s2=iBase[1]; s2<=iBound[1]; s2++ )
        for( int s1=iBase[0]; s1<=iBound[0]; s1++ )
	{
	  if( maskr(s1,s2,s3) & ISneeded )
	  {
	    pointIsNeeded=TRUE;
	    break;
	  }
	}

        if( pointIsNeeded )
	{
	  ip(ii,Rx)=ip(i,Rx);
          ii++;
	}
        else
	{
          // printf(" ***** throw away an unneeded point\n");
          maskr(ip(i,0),ip(i,1),ip(i,2))=0;
	}
      }
      if( numberOfInterpolationPoints!=ii )
      {
        // Some un-necessary points were removed.
	// Make sure ghost line values are marked properly. This is needed if we
	// removed un-necessary interpolation points
	for( axis=0; axis<numberOfDimensions; axis++ )
	{
	  for( int side=Start; side<=End; side++ )
	  {
	    getBoundaryIndex(cr.dimension(),side,axis,I1,I2,I3);
	    Iv[axis]=cr.indexRange(side,axis);

	    getBoundaryIndex(cr.dimension(),side,axis,J1,J2,J3);
	    for( int ghost=1; ghost<=cr.numberOfGhostPoints(side,axis); ghost++ )
	    {
	      Jv[axis]=cr.indexRange(side,axis)-ghost*(1-2*side);
	      where( maskr(I1,I2,I3)==0 )
	      {
		maskr(J1,J2,J3)=0;
	      }
	    }
	  }
	}
      }
      numberOfInterpolationPoints=ii;
      if( numberOfInterpolationPoints>0 )
      {
	if( numberOfDimensions==2 )
	  for( i=0; i<numberOfInterpolationPoints; i++ )
	    maskr(ip(i,0),ip(i,1),i3)=-(i+1);
	else    
	  for( i=0; i<numberOfInterpolationPoints; i++ )
	    maskr(ip(i,0),ip(i,1),ip(i,2))=-(i+1);
      }

      printf("number of interpolation points on grid=%i is %i \n",grid,numberOfInterpolationPoints);
      cg.numberOfInterpolationPoints(grid)=numberOfInterpolationPoints;

    
      if( FALSE )
        displayMask(maskr,"*************** maskr after marking interpolation *************");

    }
  }

  // we can build the interpolation arrays now that we know how many grids points we have.
  cg.update(
    CompositeGrid::THEinterpolationPoint       |
    CompositeGrid::THEinterpoleeGrid           |
    CompositeGrid::THEinterpoleeLocation       |
    CompositeGrid::THEinterpolationCoordinates, 
    CompositeGrid::COMPUTEnothing);
  
  for( l=1; l<cg.numberOfRefinementLevels(); l++ )
  {
    GridCollection & rl = cg.refinementLevel[l];
    for( int g=0; g<rl.numberOfGrids(); g++ )
    {
      int grid =rl.gridNumber(g);        // index into cg
      Range R(0,cg.numberOfInterpolationPoints(grid)-1);
      cg.interpolationPoint[grid](R,Rx)=interpolationPoints[l][g](R,Rx);
      cg.interpoleeGrid[grid](R)=grid;
      cg.interpoleeLocation[grid](R,Rx)=notAssigned;
      cg.interpolationCoordinates[grid](R,Rx)=0.;
      
    }
  }
  // delete temp arrays
  for( l=1; l<cg.numberOfRefinementLevels(); l++ )
    delete [] interpolationPoints[l];
  delete interpolationPoints;
  
  // Now fill in the interpolation data
  IntegerArray baseGridMarked(cg.numberOfBaseGrids());
  baseGridMarked=FALSE;
  
  for( l=1; l<cg.numberOfRefinementLevels(); l++ )
  {
    GridCollection & rl = cg.refinementLevel[l];
    int g;

    
    for( g=0; g<rl.numberOfGrids(); g++ )
    {
      int grid =rl.gridNumber(g);           // index into cg
      int bg = cg.baseGridNumber(grid);     // base grid for this refinement
      MappedGrid & cr = rl[g];              // refined grid
      MappedGrid & cb = cg[bg];             // base grid
      const IntegerArray & maskb = cb.mask();
      IntegerArray & maskr = cr.mask();
      const RealArray & vertex = cr.vertex();
      
      IntegerArray & ipBG = cg.interpolationPoint[bg];
      IntegerArray & interpoleeGridBG = cg.interpoleeGrid[bg];
      RealArray & interpolationCoordinatesBG = cg.interpolationCoordinates[bg];

      int rf[3];  // refinement factors (to the BASE GRID!)
      rf[0]=rl.refinementFactor(0,g);
      rf[1]=rl.refinementFactor(1,g);
      rf[2]=rl.refinementFactor(2,g);

      const real ccOffset= !cr.isAllCellCentered() ? 0. : .5;

      IntegerArray & ip = cg.interpolationPoint[grid];
      IntegerArray & interpoleeGrid = cg.interpoleeGrid[grid];
      IntegerArray & interpoleeLocation = cg.interpoleeLocation[grid];
      RealArray & interpolationCoordinates = cg.interpolationCoordinates[grid];
      const IntegerArray & indexRange = cr.indexRange();

      // mark base grid interpolation points with an index into its interpolation arrays
      if( !baseGridMarked(bg) )
      {
        baseGridMarked(bg)=TRUE;
        i3=cb.indexRange(Start,axis3);
        if( numberOfDimensions==2 )
	{
	  for( int i=0; i<cg.numberOfInterpolationPoints(bg); i++ )
	    maskb(ipBG(i,0),ipBG(i,1),i3)=-(i+1);
	}
	else
	{
	  for( int i=0; i<cg.numberOfInterpolationPoints(bg); i++ )
	    maskb(ipBG(i,0),ipBG(i,1),ipBG(i,2))=-(i+1);
	}
      }

      // ==== First mark refinement interpolation points that coincide with a base grid interp. pt. =====
      i3=cr.indexRange(Start,axis3);
      j3=cb.indexRange(Start,axis3);
      for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ ) // refinement interpolation points
      {
        bool checkThisPoint=TRUE;
        for( axis=0; axis<numberOfDimensions; axis++ )
	{
          if( ip(i,axis) % rf[axis] != 0 )
	  {
	    checkThisPoint=FALSE;     // this pt does not lie on a base grid pt
	    break;
	  }
          iv[axis]=ip(i,axis);
	  jv[axis]=iv[axis]/rf[axis];  // corresponding base grid point
	}
	if( !checkThisPoint )
          continue;

	int ib=-maskb(j1,j2,j3)-1;
	assert( ib>=0 && ib<cg.numberOfInterpolationPoints(bg) );
	
        if( debug & 2)
          printf("Interpolation point %6i=(%i,%i,%i) (refined grid%i) corresponds to interp pt %i from base grid %i\n",
                 i,i1,i2,i3,grid,ib,bg);
	
        // check to see if there is a refinement grid on the interpolee base grid that we can interpolate from.
        // ie[3]=={ie1,ie2,ie3} : nearest point on the interpolee grid
	for( axis=0; axis<numberOfDimensions; axis++ )
	{
          ie[axis]=(interpolationCoordinatesBG(ib,axis)/cg[interpoleeGridBG(ib)].gridSpacing(axis))*rf[axis]+
	    cb.indexRange(Start,axis)+.5;  // closest point (cell centered??)
	}
        // check other refinement grids
        bool canInterpolate=FALSE;
        for( int g2=0; g2<rl.numberOfGrids(); g2++ )
	{
	  if( rl.baseGridNumber(g2)==interpoleeGridBG(ib) )
	  {
	    if( ie1<rl[g2].indexRange(Start,0) || ie1>rl[g2].indexRange(End,0) ||
		ie2<rl[g2].indexRange(Start,1) || ie2>rl[g2].indexRange(End,1) )        
	      continue;
            if( numberOfDimensions==3 && (ie3<rl[g2].indexRange(Start,2) || ie3>rl[g2].indexRange(End,2)) )
              continue;

            // we are inside this refinement grid.
	    interpoleeGrid(i)=rl.gridNumber(g2);
	    MappedGrid & ig = cg[interpoleeGrid(i)];  
	    MappedGrid & ibg = cg[interpoleeGridBG(ib)];
	    for( axis=0; axis<numberOfDimensions; axis++ )
	    {
	      const int rf = rl.refinementFactor(axis,g2);
	      interpolationCoordinates(i,axis)=
		(interpolationCoordinatesBG(ib,axis)*rf/ibg.gridSpacing(axis)
		 -(ig.indexRange(Start,axis)-ibg.indexRange(Start,axis)*rf) )*ig.gridSpacing(axis);
	    }
	    interpolates(0)=TRUE;
	    rr(0,Rx)=interpolationCoordinates(i,Rx);
	    cg.rcData->canInterpolate(grid,interpoleeGrid(i), rr, interpolates, useBackupRules, checkForOneSided );

	    if( interpolates(0) )
	    {
	      canInterpolate=TRUE;
              if( debug & 2)
	        printf("  Interp. point %5i (refine grid %i) can interp from refinement grid %i, r=(%6.2e,%6.2e)\n",
		     i,grid,rl.gridNumber(g2),interpolationCoordinates(i,0),interpolationCoordinates(i,1));

	      // assign all these below : interpoleeLocation(i,Rx)=0;  // ******
  	      break;
	    }
	  }
	}
	if( !canInterpolate )
	{
          // if we cannot interpolate from another refinement then interpolate from the same grid that the
          // base grid interpolates from.
          interpoleeGrid(i)=interpoleeGridBG(ib);
  	  interpolationCoordinates(i,Rx)=interpolationCoordinatesBG(ib,Rx);
          if( debug & 2)
 	    printf("  Interp. point %5i (refine grid %i) interps from base grid %i, r=(%6.2e,%6.2e,%6.2e)\n",
		       i,grid,interpoleeGrid(i),interpolationCoordinates(i,0),interpolationCoordinates(i,1), 
                       numberOfDimensions==2 ? 0. :interpolationCoordinates(i,2) );

	  if( debug & 2 )
	  {
	    // double check
	    interpolates(0)=TRUE;
	    rr(0,Rx)=interpolationCoordinates(i,Rx);
	    cg.rcData->canInterpolate(grid,interpoleeGrid(i), rr, interpolates, useBackupRules, checkForOneSided );

	    if( !interpolates(0) )
	    {
	      printf("  ERROR: Interp. point %5i CANNOT interp from base grid %i. Something is wrong here!\n",
		       i,grid,interpoleeGrid(i));
	    }
	  }
	}
        maskr(i1,i2,i3)=-maskr(i1,i2,i3);  // make positive to indicate we have processed this point
      }

      // now fill in the "in-between" interpolation points 
      i3=cr.indexRange(Start,axis3);
      j3=cb.indexRange(Start,axis3);
      for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
      {
        for( axis=0; axis<numberOfDimensions; axis++ )
	{
          iv[axis]=ip(i,axis);
          jv[axis]=iv[axis];
	}
	if( maskr(i1,i2,i3) <0 ) // if we have not assigned values for this interp. point yet
	{
          // check for a neighbouring point that has already been assigned.
          int interpoleeFound=0; // 1=found but from a base grid, 2=found from a refinement.
          for( axis=0; axis<numberOfDimensions && !interpoleeFound; axis++ )
	  {
            // we only need to check this axis if we lie on the base grid in one of the other directions.
            //
            //   In 2D we only need to look along one axis to find a neighbouring base grid point 0
            //   for each in-between point +
            //                     0       
            //                     |
            //                     +
            //                     |
            //               0--+--0--+--0
            // 
            //   In 3D there are two cases. In one case point + lies between two 0's and we only need
            //   two check two 0's. In the other case we need to check 4 0's if the point + is not on
            //   the same line as two 0's.
            //               0--+--0       
            //               |  |  |
            //               +--+--+
            //               |  |  |
            //               0--+--0
            const int axisp1 = (axis+1) % numberOfDimensions;
            const int axisp2 = numberOfDimensions==2 ? axisp1 : (axis+2) % numberOfDimensions;
            const int numAligned = (iv[axisp1] % rf[axisp1] ==0) + (iv[axisp2] % rf[axisp2] == 0);

            // if( (iv[axisp1] % rf[axisp1] ==0) || (iv[axisp2] % rf[axisp2] == 0) )
            if( (iv[axis] % rf[axis] !=0) && numAligned>0 )
	    {
	      for( int side=Start; side<=End; side++ ) // check both directions along direction axis
	      {
		// shift to a point that sits on top of a coarse grid point. 
                // ****** harder in 3D ******
  		jv[axis]=iv[axis]-( (iv[axis]-cr.indexRange(Start,axis)) % rf[axis] )+ rf[axis]*side;

                
                const int end2 = (numAligned==1 && numberOfDimensions==3) ? 1 : 0;
                for( int side2=0; side2<=end2; side2++ ) // here is where we possibly check 4 neighbours
		{
		  if( (iv[axisp1] % rf[axisp1]) !=0 )
		    jv[axisp1]=iv[axisp1]-( (iv[axisp1]-cr.indexRange(Start,axisp1)) % rf[axisp1] )+ rf[axisp1]*side2;
		  else if( (iv[axisp2] % rf[axisp2]) !=0 )
		    jv[axisp2]=iv[axisp2]-( (iv[axisp2]-cr.indexRange(Start,axisp2)) % rf[axisp2] )+ rf[axisp2]*side2;
                  
                  if( debug & 2)
		    printf("  grid=%i, i=%i, iv=(%i,%i,%i) `base' grid neighbour:jv=(%i,%i,%i), maskr(jv)=%i \n",
			   grid,i,i1,i2,i3,j1,j2,j3, maskr(j1,j2,j3));
		  

		  if( maskr(j1,j2,j3)>0 && maskr(j1,j2,j3)<=cg.numberOfInterpolationPoints(grid) )
		  {
		    int ii=maskr(j1,j2,j3)-1; // ***note maskr holds the interpolation point number
		    assert( ii>=0 && ii<cg.numberOfInterpolationPoints(grid) );
		    int interpolee=interpoleeGrid(ii);  // use interpolee grid from this point.
		  
                    if( debug & 2)
		      printf("  grid=%i, i=%i, ii=%i, interpoleeGrid(ii)=%i interpoleeGrid(i)=%i\n",
                        grid,i,ii,interpolee,interpoleeGrid(i));
		  
		    // try to interpolate from the same interpolee grid if the interpolee
		    // grid is not equal to the base grid for this refinement.
		    // ***** check all points on the segment jv[axis]+1,...jv[axis]+rf[axis]-1
		    if( interpolee!=interpoleeGrid(i) ) // don't check a grid we have already tried.
		    {

		      // invert the mapping to locate the point.
		      for( dir=0; dir<numberOfDimensions; dir++ )
			xx(0,dir)=vertex(i1,i2,i3,dir);
		      cg[interpolee].mapping().inverseMap(xx(0,Rx),rr);
		      interpolates(0)=TRUE;
		      cg.rcData->canInterpolate(grid,interpolee, rr, interpolates, useBackupRules, checkForOneSided );
                      if( debug & 2)
 		        printf("  grid=%i, i=%i, interpolee=%i, canInterpolate=%i,r=(%6.2e,%6.2e,%6.2e) \n",
                               grid,i,interpolee,interpolates(0),rr(0,0),rr(0,1),rr(0,2));
		    
		      if( interpolates(0) )
		      {
			interpoleeGrid(i)=interpolee;
			interpolationCoordinates(i,Rx)=rr(0,Rx);
			// assign below : interpoleeLocation(i,Rx)=0;  // ******
			maskr(i1,i2,i3)=-maskr(i1,i2,i3);
			if( interpolee!=cg.baseGridNumber(interpolee) )
			{
                          if( debug & 2)
			    printf("Inbetween interp.point %5i (refine=%i, base=%i) can interp from grid %i,"
				 " r=(%6.2e,%6.2e) \n",i,grid,bg,interpolee,rr(0,0),rr(0,1));

			  interpoleeFound=2;
			  break;
			}
			else
			  interpoleeFound=1; // 1=found but from a base grid, keep looking
		      }
                      else if( interpoleeFound==0 )
		      {
                        interpoleeGrid(i)=interpolee;  // indicates we have tried this interpolee grid.
		      }
		    }
		  }
		  
		} // end for side2
		// *** jv[axis]=iv[axis];  // reset
	      }
	    }
	  }// end for axis
	  
	  if( interpoleeFound==1 )
 	  {
	    if( debug & 2)
	      printf("Inbetween interp. point %i, (refine=%i, base=%i) can only interpolate from the base grid %i\n",
		     i,grid,bg,interpoleeGrid(i));
	  }
	  else if( interpoleeFound==0 )
	  {
            // as a last resort we interpolate from the same base grid.
	    interpoleeGrid(i)=bg;
            for( dir=0; dir<numberOfDimensions; dir++ )
	    {
  	      interpolationCoordinates(i,dir)=( iv[dir]/real(rf[dir]) -cb.indexRange(Start,dir) )*
                cb.gridSpacing(dir)+ccOffset;
	    }
            // we need to shift the interpolatee location off center since we are interpolating 
            // "one sided" from an interpolation boudary. It is not really one sided since the interpolation
            // weights will be all zero for the off center points.
            // 
            j1=i1, j2=i2, j3=i3;
            for( dir=0; dir<numberOfDimensions; dir++ )
	    {
	      if( iv[dir] % rf[dir] == 0 )
	      {
                // we can shift the stencil in this direction since we are exactly on the coordinate line
                // of the interpolee grid.
                jv[dir]=max(iv[dir]-1,cr.dimension(Start,dir));
                if( maskr(j1,j2,j3)==0 ) 
                  interpoleeLocation(i,dir)=iv[dir]/rf[dir]; // lower left corner of the stencil
                else 
		{
		  jv[dir]=min(iv[dir]+1,cr.dimension(End,dir));
                  if( maskr(j1,j2,j3)==0 )
		    interpoleeLocation(i,dir)=iv[dir]/rf[dir]-(cg.interpolationWidth(dir,grid,bg,mgLevel)-1);
		  else
		  {
		    printf("ERROR: Inbetween interp. point %i, (refine=%i, base=%i) can NOT interpolate "
			   "from the SAME base grid %i. iv=(%i,%i,%i) \n",
			   i,grid,bg,interpoleeGrid(i),i1,i2,i3);
		    throw "error";
		  }
		}
	      }
	    }

	    maskr(i1,i2,i3)=-maskr(i1,i2,i3);
	    if( TRUE || debug & 2)
	      printf("WARNING: Inbetween interp. point %i, (refine=%i, base=%i) can only interpolate "
                     "from the SAME base grid %i. iv=(%i,%i,%i), interpolee=(%i,%i)\n",
		     i,grid,bg,interpoleeGrid(i),i1,i2,i3,interpoleeLocation(i,0),interpoleeLocation(i,1));
	  }
	  
	}
	// maskr(i1,i2,i3)=MappedGrid::ISinterpolationPoint;
      }

      if( cg.numberOfInterpolationPoints(grid)>0 )
      {
	Range R(0,cg.numberOfInterpolationPoints(grid)-1);
	if( numberOfDimensions==2 )
	{
	  i3=cr.indexRange(Start,axis3);
	  maskr(ip(R,0),ip(R,1),i3)=MappedGrid::ISinterpolationPoint;   
	}
	else
	  maskr(ip(R,0),ip(R,1),ip(R,2))=MappedGrid::ISinterpolationPoint;   
      }
      // assign interpolationLocations for this grid:  ******
      for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
      {
	int grid2=interpoleeGrid(i);
	const IntegerArray & interpolationWidth = cg.interpolationWidth(Rx,grid,grid2,mgLevel);
	MappedGrid & g2 = cg[grid2];
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
          if( interpoleeLocation(i,axis)==notAssigned )
	  {
	    // Get the lower-left corner of the interpolation cube.
	    int intLoc=int(floor(interpolationCoordinates(i,axis)/g2.gridSpacing(axis) + g2.indexRange(0,axis) -
				 .5 * interpolationWidth(axis) + (g2.isCellCentered(axis) ? .5 : 1.)));
	    if (!g2.isPeriodic(axis)) 
	    {
	      if( (intLoc < g2.extendedIndexRange(0,axis)) && (g2.boundaryCondition(Start,axis)>0) )
	      {
		//                        Point is close to a BC side.
		//                        One-sided interpolation used.
		intLoc = g2.extendedIndexRange(0,axis);
	      }
	      if( (intLoc + interpolationWidth(axis) - 1 > g2.extendedIndexRange(1,axis))
		  && (g2.boundaryCondition(End,axis)>0) )
	      {
		//                        Point is close to a BC side.
		//                        One-sided interpolation used.
		intLoc = g2.extendedIndexRange(1,axis) - interpolationWidth(axis) + 1;
	      }
	    } // end if
	    interpoleeLocation(i,axis) = intLoc;
	  }
	} // end for_1
      }


    } // done refinement grids
      
  } // done refinement levels

  // reset interpolation points on the base grids.
  for( int bg=0; bg<cg.numberOfBaseGrids(); bg++ )
  {
    if( baseGridMarked(bg) && cg.numberOfInterpolationPoints(bg)>0 )
    {
      IntegerArray & ip = cg.interpolationPoint[bg];
      Range R(0,cg.numberOfInterpolationPoints(bg)-1);
      IntegerArray & mask = cg[bg].mask();
      if( numberOfDimensions==2 )
      {
        i3=cg[bg].indexRange(Start,axis3);
        mask(ip(R,0),ip(R,1),i3)=MappedGrid::ISinterpolationPoint;  
      }
      else
        mask(ip(R,0),ip(R,1),ip(R,2))=MappedGrid::ISinterpolationPoint;   
    }
  }
  

/* -----
      where( maskb(I1b,I2b,I3b) )
      {
	// maskb(I1b,I2b,I3b) = maskb(I1b,I2b,I3b) | MappedGrid::IShiddenByRefinement;
	maskb(I1b,I2b,I3b) |= MappedGrid::IShiddenByRefinement;
      }
---- */

  return 0;
}

