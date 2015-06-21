#include "Parameters.h"
#include "Ogen.h"
#include "DomainSolver.h"

// =================================================================================================
///  \brief Regenerate the overlapping grid after some of the grids have changed 
///
/// \param cg (output) :
/// \param cgOld (input) : previous grid 
// =================================================================================================
int Parameters::
regenerateOverlappingGrid( CompositeGrid & cg , CompositeGrid & cgOld, bool resetToFirstPriority /* = false */  )
{
  real cpu0 = getCPU();
  Parameters & parameters = *this;
  const int debug = dbase.get<int >("debug");

  Ogen *gridGenerator = parameters.dbase.get<Ogen* >("gridGenerator");
  // Now regenerate the grid
  IntegerArray hasMoved(cg.numberOfComponentGrids()); 
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    hasMoved(grid)=parameters.gridIsMoving(grid);
  
  if( debug & 4 )
    printF("--ROG-- regenerateOverlappingGrid: call the grid generator to updateOverlap ...\n");

  // const int fullUpdateFreq=max(1,parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"));
  // bool resetToFirstPriority = (numberOfMoves % fullUpdateFreq) == (fullUpdateFreq-1); 

  if( resetToFirstPriority )
  {
    if( debug & 4 )
      printF("\n --ROG-- +++++++++ moveGrids: use resetToFirstPriority (full update) in Ogen::updateOverlap +++++++++++++ \n");
  }
  //  GridGenerator.updateOverlap(cg);  // use full algorithm
  if( debug & 128 )
  {
    display(cg.interpolationWidth,"before move: cg.interpolationWidth",parameters.dbase.get<FILE* >("debugFile"));
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      display(cg.interpolationCoordinates[grid],"beforer move: interpolationCoordinates",parameters.dbase.get<FILE* >("debugFile"));
  }

  //  enum MovingGridOption
  //  {
  //    useOptimalAlgorithm=0,
  //    minimizeOverlap=1,
  //    useFullAlgorithm
  //  };

  // resetToFirstPriority=true;  // *** uncomment this line to force a full update ***
  
  // if( false ) 
  //   printf(" ***move:BEFORE updateOverlap  cg-> computedGeometry & THErefinementLevel=%i \n",
  // 	   cg->computedGeometry & CompositeGrid::THErefinementLevel);

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


  DomainSolver::checkArrays("--ROG--: before gridGenerator->updateOverlap");

  // if( false ) // ******************** TEMP *********************
  // {
  //   aString gridFileName; sPrintF(gridFileName,"gridStep%i.hdf",numberOfMoves);
  //   aString gridName="gridFileName";
  //   printF("Saving the current grid in the file %s.\n",(const char*)gridFileName);
  //   Ogen::saveGridToAFile( cg,gridFileName,gridName );
  // }

  // This next param is set in the "general options..." dialog
  bool useInteractiveGridGenerator=parameters.dbase.get<bool >("useInteractiveGridGenerator");
  if( parameters.dbase.get<int>("simulateGridMotion")<=1 ) // simulateGridMotion>1 : do not call ogen
  {
    if( !useInteractiveGridGenerator )
    {
      // normal way: 
      if( true )
      {
	if( &cg!=&cgOld )
	{
	  gridGenerator->updateOverlap(cg,cgOld,hasMoved,
				       resetToFirstPriority ? Ogen::useFullAlgorithm : Ogen::useOptimalAlgorithm);
	}
	else
 	{
          // -- there is no previous grid ---
	  gridGenerator->updateOverlap(cg);
	}
	
	
      }
      else
      { // ** TRY creating a new ogen at each step ***
	printF("MOVE: create a new Ogen!\n");
	Ogen ogen(*parameters.dbase.get<GenericGraphicsInterface* >("ps"));
        ogen.updateOverlap(cg,cgOld,hasMoved,
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
	Ogen::saveGridToAFile( cg,gridFileName,gridName );
      }
    }
    else 
    { // *** for debugging we call the grid generator interactively -- trouble here in parallel, interp pts?
      // new way: 
      printF("DomainSolver::moveGrids: Entering the interactive update so you can step through the grid computation\n");
      MappingInformation mapInfo;
      mapInfo.graphXInterface=NULL; // This means that we do not provide Mappings 
      gridGenerator->updateOverlap( cg, mapInfo );
    }
  }
  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGridGeneration"))+=getCPU()-cpu0;

  DomainSolver::checkArrays("--ROG--  after gridGenerator->updateOverlap");



  return 0;
}

