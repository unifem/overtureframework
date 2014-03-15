#include "DomainSolver.h"
#include "MatrixTransform.h"
#include "Ogen.h"
#include "MovingGrids.h"
#include "ParallelUtility.h"

static int numberOfMoves=0;

typedef MatrixTransform *MatrixTransformPointer;

#define ForBoundary(side,axis)   for( int axis=0; axis<cg.numberOfDimensions(); axis++ ) \
for( int side=0; side<=1; side++ )

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{moveGrids}} 
void DomainSolver::
moveGrids(const real & t1, 
	  const real & t2, 
	  const real & t3,
	  const real & dt0,
	  GridFunction & cgf1,  
	  GridFunction & cgf2,
	  GridFunction & cgf3 )
//==================================================================================
// /Description:
//  Move component grids on a Composite Grid and have Ogen generate the new grid
// \begin{verbatim}
//          g3(t3) <- g1(t1) + (t3-t1)*d(g2(t2))/dt
// \end{verbatim}
//
//  The grid generator generates cgf3.cg from cgf1.cg. 
//  The gridVelocities are computed on cgf2 and cgf3.
//  cg1 and cgf3 must be DIFFERENT
//
//  /t1,cgf1 (input): grid and solution at time t1
//  /t2,cgf2 (input): grid velocity is taken from this time
//
//  /cgf3 (output): new grid at time t3 *** also holds a grid velocity (needed by BC's) ****
//  /cgf2 (output): holds new gridVelocity at time t2. I believe that cg2 can be the same as cgf1 or cgf3
//
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  real cpu0 = getCPU();
  if( !parameters.isMovingGridProblem() )
    return;
  
  assert( &cgf1 != &cgf3 );

  if( false )
  {
    fprintf(parameters.dbase.get<FILE* >("moveFile"),
            "moveGrids: (t1,t2,t3)=(%9.3e,%9.3e,%9.3e) (cgf1,cgf2,cgf3).t=(%9.3e,%9.3e,%9.3e)\n",
	    t1,t2,t3,  cgf1.t,cgf2.t,cgf3.t);
    fflush(parameters.dbase.get<FILE* >("moveFile"));
  }
  
  if( t3<t2 )
  {
    // This must be an initialization step when the grid is moved backwards
    
    // If there are interfaces we must set the boundary data array that defines the location
    // of the interface for negative times

    setInterfacesAtPastTimes( t1,t2,t3,dt0,cgf1,cgf2,cgf3 );

  }


  // ---------------------------------------------
  // ------------- Move the Grids ----------------
  // ---------------------------------------------
  parameters.dbase.get<MovingGrids >("movingGrids").moveGrids(t1,t2,t3,dt0,cgf1,cgf2,cgf3 );



  Ogen *gridGenerator = parameters.dbase.get<Ogen* >("gridGenerator");
  // Now regenerate the grid
  IntegerArray hasMoved(cgf3.cg.numberOfComponentGrids()); 
  int grid;
  for( grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ )
    hasMoved(grid)=parameters.gridIsMoving(grid);
  
  if( debug() & 2 )
    printF("moveGrids: call the grid generator to updateOverlap...\n");

  real time0 = getCPU();

  const int fullUpdateFreq=max(1,parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"));
  bool resetToFirstPriority = (numberOfMoves % fullUpdateFreq) == (fullUpdateFreq-1); 

  if( resetToFirstPriority )
  {
    if( debug() & 2 )
      printF("\n +++++++++ moveGrids: use resetToFirstPriority (full update) in Ogen::updateOverlap +++++++++++++ \n");
  }
  //  GridGenerator.updateOverlap(cgf3.cg);  // use full algorithm
  if( debug() & 128 )
  {
    display(cgf3.cg.interpolationWidth,"before move: cgf3.cg.interpolationWidth",parameters.dbase.get<FILE* >("debugFile"));
    for( int grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ )
      display(cgf3.cg.interpolationCoordinates[grid],"beforer move: interpolationCoordinates",parameters.dbase.get<FILE* >("debugFile"));
  }

  //  enum MovingGridOption
  //  {
  //    useOptimalAlgorithm=0,
  //    minimizeOverlap=1,
  //    useFullAlgorithm
  //  };

  // resetToFirstPriority=true;  // *** uncomment this line to force a full update ***
  
  if( false ) 
    printf(" ***move:BEFORE updateOverlap  cgf3.cg-> computedGeometry & THErefinementLevel=%i \n",
	   cgf3.cg->computedGeometry & CompositeGrid::THErefinementLevel);

  // No need for Ogen to invalidate the geometry because we now do this in MovingGrids::moveGrid
  gridGenerator->set(Ogen::THEcomputeGeometryForMovingGrids,false);

  // output the grid if the algorithm fails. 
  gridGenerator->set(Ogen::THEoutputGridOnFailure,true);
  // abort the program if we are not running interactively
  bool abortOnAlgorithmFailure = true;
  if( parameters.dbase.get<GenericGraphicsInterface* >("ps")!=NULL && 
      parameters.dbase.get<GenericGraphicsInterface* >("ps")->graphicsIsOn() )
    abortOnAlgorithmFailure=false;
  gridGenerator->set(Ogen::THEabortOnAlgorithmFailure,abortOnAlgorithmFailure);


  checkArrays(" moveGrids: before gridGenerator->updateOverlap");

  if( false ) // ******************** TEMP *********************
  {
    aString gridFileName; sPrintF(gridFileName,"gridStep%i.hdf",numberOfMoves);
    aString gridName="gridFileName";
    printF("Saving the current grid in the file %s.\n",(const char*)gridFileName);
    Ogen::saveGridToAFile( cgf3.cg,gridFileName,gridName );
  }

  // This next param is set in the "general options..." dialog
  bool useInteractiveGridGenerator=parameters.dbase.get<bool >("useInteractiveGridGenerator");
  if( parameters.dbase.get<int>("simulateGridMotion")<=1 ) // simulateGridMotion>1 : do not call ogen
  {
    if( !useInteractiveGridGenerator )
    {

      // normal way: 
      if( true )
      {
	gridGenerator->updateOverlap(cgf3.cg,cgf1.cg,hasMoved,
				     resetToFirstPriority ? Ogen::useFullAlgorithm : Ogen::useOptimalAlgorithm);
      }
      else
      { // ** TRY creating a new ogen at each step ***
	printF("MOVE: create a new Ogen!\n");
	Ogen ogen(*parameters.dbase.get<GenericGraphicsInterface* >("ps"));
        ogen.updateOverlap(cgf3.cg,cgf1.cg,hasMoved,
				     resetToFirstPriority ? Ogen::useFullAlgorithm : Ogen::useOptimalAlgorithm);
      }

      if( false )
      {
	aString gridFileName="gridFailed.hdf", gridName="gridFailed";
	printF("Saving the current grids in the file %s (option outputGridOnFailure=true)\n",(const char*)gridFileName);
	printF("To see what went wrong, use ogen to generate this grid using the commands: \n"
	       "ogen\ngenerate an overlapping grid\n read in an old grid\n  %s\n  reset grid"
	       "\n display intermediate results\n compute overlap\n",
	       (const char*)gridFileName); 
	Ogen::saveGridToAFile( cgf3.cg,gridFileName,gridName );
      }
    }
    else 
    { // *** for debugging we call the grid generator interactively -- trouble here in parallel, interp pts?
      // new way: 
      printF("DomainSolver::moveGrids: Entering the interactive update so you can step through the grid computation\n");
      MappingInformation mapInfo;
      mapInfo.graphXInterface=NULL; // This means that we do not provide Mappings 
      gridGenerator->updateOverlap( cgf3.cg, mapInfo );
    }
  }
  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGridGeneration"))+=getCPU()-cpu0;

  checkArrays(" moveGrids: after gridGenerator->updateOverlap");

  if( false ) 
   printf(" ***move:AFTER updateOverlap  cgf3.cg-> computedGeometry & THErefinementLevel=%i \n",
	   cgf3.cg->computedGeometry & CompositeGrid::THErefinementLevel);
  numberOfMoves++;
    
  // *** Mark refinement level grids to indicate that the geometry has changed *wdh* 040315
  if( false )
  {
    for( grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ )
    {
      if( cgf3.cg.refinementLevelNumber(grid)>0 )
      {
	const int base = cgf3.cg.baseGridNumber(grid);
	if( hasMoved(grid) )
	{
	  printf("\n===> mark AMR grid %i geometryHasChanged\n");
	  cgf3.cg[grid].geometryHasChanged(~MappedGrid::THEmask);  // is this needed?

	  Mapping & map = cgf3.cg[grid].mapping().getMapping();
	  Mapping & mapBase = cgf3.cg[base].mapping().getMapping();

	  assert( map.getClassName()=="ReparameterizationTransform" );
	  ReparameterizationTransform & transform = (ReparameterizationTransform&)map;
	  Mapping & map2 = transform.map2.getMapping();
	
	  printf("===>  AMR grid =%i has mapping ptr=%i, base grid=%i has mapping ptr=%i\n",
		 grid,&map2,base,&mapBase);
	}
      }
    }
  }
  

  if( debug() & 128 )
  {
    display(cgf3.cg.interpolationWidth,"after move: cgf3.cg.interpolationWidth",parameters.dbase.get<FILE* >("debugFile"));
    for( int grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ )
      display(cgf3.cg.interpolationCoordinates[grid],"after move: interpolationCoordinates",parameters.dbase.get<FILE* >("debugFile"));
  }
    
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForMovingGrids"))+=getCPU()-cpu0;

  
}

void DomainSolver::
getGridVelocity( GridFunction & gf0, const real & tGV )
//=================================================================================
// /Description:
//    Determine the gridVelocity for this grid function (if tGV!=gf0.gridVelocityTime)
//    Get gf0.gridVelocity at time gf0.t
// ***NOTE*** parameters here must be consistent with those in move
//=================================================================================
{
  parameters.dbase.get<MovingGrids >("movingGrids").getGridVelocity(gf0,tGV);
}



// =================================================================================
/// \brief Return the maximum relative change in the moving grid correction scheme.
///    This is usually only an issue for "light" bodies. 
// =================================================================================
real DomainSolver::
getMovingGridMaximumRelativeCorrection()
{
  return parameters.dbase.get<MovingGrids >("movingGrids").getMaximumRelativeCorrection();
}

// =================================================================================
/// \brief Return true if the correction steps for moving grids have converged.
///    This is usually only an issue for "light" bodies. 
// =================================================================================
bool DomainSolver::
getMovingGridCorrectionHasConverged()
{
  return parameters.dbase.get<MovingGrids >("movingGrids").getCorrectionHasConverged();
}


//==================================================================================
/// \brief Corrector step for moving grids.
/// \details This function is called at the corrector step to update the moving grids. For example,
///  in a predictor corrector type algorithm we may want to correct the forces and torques
///   on bodies since the solution can depend on these (For INS the pressure BC depends on
///  the acceleration on the boundary ).
/// \param t1,cgf1 (input) : solution at the old time
/// \param t2,cgf2 (input) : solution at the new time (these are valid values)
//==================================================================================
void DomainSolver::
correctMovingGrids(const real t1,
                   const real t2, 
                   GridFunction & cgf1,
		   GridFunction & cgf2 )
{
  // real dt0=0.;
  // parameters.dbase.get<MovingGrids >("movingGrids").rigidBodyMotion(t3,t3,t3,dt0,cgf3,cgf3,cgf3);
  parameters.dbase.get<MovingGrids >("movingGrids").correctGrids(t1,t2,cgf1,cgf2);
}

//==================================================================================
/// \brief Assign interface positions for "negative" times.
/// \details Some time-steppers take an initial step or two backwards. We need to set
///  appropriate locations for any interfaces for these past times.
///  \param t1,cgf1 (input): grid and solution at time t1
///  \param t2,cgf2 (input): grid velocity is taken from this time
///  \param cgf3 (input) : this will hold the new grid and solution
//==================================================================================
void DomainSolver::
setInterfacesAtPastTimes(const real & t1, 
			 const real & t2, 
			 const real & t3,
			 const real & dt0,
			 GridFunction & cgf1,  
			 GridFunction & cgf2,
			 GridFunction & cgf3 )
{
  // This must be an initialization step when the grid is moved backwards
    
  // If there are interfaces we must set the boundary data array that defines the location
  // of the interface for negative times

  // Here is the array that defines the domain interfaces, interfaceType(side,axis,grid) 
  const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

  CompositeGrid & cg = cgf3.cg; // use grid from cgf3 : is this correct?
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( parameters.gridIsMoving(grid) )
    {
      ForBoundary(side,axis)
      {
	if( interfaceType(side,axis,grid)==Parameters::tractionInterface ||
	    interfaceType(side,axis,grid)==Parameters::tractionAndHeatFluxInterface )
	{
	  // This is a deforming interface

          // Use the gridVelocity to determine the location of the interface at past times

	  MappedGrid & mg = cgf3.cg[grid];
	  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

          // Use the interface location from cgf1:
          realArray & x = cgf1.cg[grid].vertex();
          // Use the grid velocity from cgf2: 
	  realArray & gridVelocity = cgf2.getGridVelocity(grid);
	  #ifdef USE_PPP
            realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
            realSerialArray gridVelocityLocal; getLocalArrayWithGhostBoundaries(gridVelocity,gridVelocityLocal);
          #else
            realSerialArray & xLocal = x;
            realSerialArray & gridVelocityLocal = gridVelocity;
          #endif

	  Range Rx=cg.numberOfDimensions();
	  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
	  int extra=1;
	  getBoundaryIndex(mg.extendedIndexRange(),side,axis,I1,I2,I3,extra);
	  int includeGhost=1;
	  bool ok=ParallelUtility::getLocalArrayBounds(x,xLocal,I1,I2,I3,includeGhost);
	  if( ok )
	  {
            real dt = t3-t1;

	    if( debug() & 4 )
	    {
	      fprintf(parameters.dbase.get<FILE* >("pDebugFile"),"setInterfacesAtPastTimes: Setting the interface"
		     " at time t1=%9.3e, t2=%9.3e, t3=%9.3e, dt=%9.3e\n",t1,t2,t3,dt);
	      ::display(bd,sPrintF("setInterfacesAtPastTimes:boundaryData for (grid,side,axis)=(%i,%i,%i)"
                                                " t3=%9.3e, (BEFORE)",
				grid,side,axis,t3),parameters.dbase.get<FILE* >("pDebugFile"),"%7.3f ");
	    }
	    // RealArray bd(I1,I2,I3,Rx);

	    const int uc = parameters.dbase.get<int >("uc"); // ** fix me **

	    bd(I1,I2,I3,Rx+uc) = xLocal(I1,I2,I3,Rx) + gridVelocityLocal(I1,I2,I3,Rx)*dt;
	    if( debug() & 4 )
	    {
	      ::display(bd,sPrintF("setInterfacesAtPastTimes:boundaryData for (grid,side,axis)=(%i,%i,%i)"
                                                " t3=%9.3e (AFTER)",
				grid,side,axis,t3),parameters.dbase.get<FILE* >("pDebugFile"),"%7.3f ");
	    }

	  }
	    
	}
      }
    }
  }

}

