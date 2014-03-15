#include "DynamicMovingGrids.h"

//\begin{>>DynamicMovingGrids.tex}{\subsubsection{DynamicMovingGrids default  constructor}}
DynamicMovingGrids::
DynamicMovingGrids()
//
// /Purpose: 
//   This is the default constructor for the DynamicMovingGrids class,
//   If this constructor is used,
//   then DynamicMovingGrids::initialize must be called. 
//
//   This is not the recommended
//   constructor for this class.  Use
//   DynamicMovingGrids(CompositeGrid\&, GenericGridMotion*, Ogen*, GenericGraphicsInterface*)
//   instead.
//
//\end{DynamicMovingGrids.tex}
{
  cout << "DynamicMovingGrids: default constructor called" << endl;
  compositeGridListInitialized = LogicalFalse;
  movingGridMappingsSet        = LogicalFalse;
  useOgen                      = LogicalFalse;
  gridsHaveMoved               = LogicalFalse;
  mainConstructorCalled     = LogicalFalse;
  
}

//\begin{>>DynamicMovingGrids.tex}{\subsubsection{DynamicMovingGrids default debugging constructor}}
DynamicMovingGrids::
DynamicMovingGrids(GenericGraphicsInterface * ps_)
//
// /Purpose: 
//   This is the default debugging constructor for the DynamicMovingGrids class,
//    If this constructor is used,
//   then DynamicMovingGrids::initialize must be called. 
//
//   This is not the recommended
//   constructor for this class.  Use
//   DynamicMovingGrids(CompositeGrid\&, GenericGridMotion*, Ogen*, GenericGraphicsInterface*)
//   instead.
// /ps\_: optional pointer to a GenericGraphicsInterface object; used in debugging mode
//       to make intermediate plots.
//
//\end{DynamicMovingGrids.tex}

{
  cout << "DynamicMovingGrids: default constructor called" << endl;
  compositeGridListInitialized = LogicalFalse;
  movingGridMappingsSet        = LogicalFalse;
  useOgen                      = LogicalFalse;
  gridsHaveMoved               = LogicalFalse;
  mainConstructorCalled       = LogicalFalse;
  ps                          = ps_;
}

DynamicMovingGrids::
~DynamicMovingGrids()
{
  cout << "DynamicMovingGrids::destructor called..." << endl;
  destructCompositeGridList();
}

/* *** REMOVED V16
DynamicMovingGrids::
DynamicMovingGrids (CompositeGrid& compositeGrid_,
		    GenericGridMotion* genericGridMotion_,
		    Cgsh* gridGenerator_,
		    GenericGraphicsInterface* ps_)
{


  if (LogicalFalse) 
  {
    useOgen           = LogicalFalse;
    ps                = ps_;
    gridGenerator     = gridGenerator_;
    initialize (compositeGrid_, genericGridMotion_);
  }
  else
  {
    cout << "DynamicMovingGrids(CompositeGrid&, GenericGridMotion*, Cgsh*, GenericGraphicsInterface*) is obsolete " << endl;
    cout << "Please use DynamicMovingGrids(CompositeGrid&, GenericGridMotion*, Ogen*, GenericGraphicsInterface*) instead" << endl;
  }
  
}

****** */

//\begin{>>DynamicMovingGrids.tex}{\subsubsection{DynamicMovingGrids preferred constructor}}
DynamicMovingGrids::
DynamicMovingGrids (CompositeGrid& compositeGrid_,
		    GenericGridMotion* genericGridMotion_,
		    Ogen* ogenGridGenerator_,
		    GenericGraphicsInterface* ps_)
//
// /Purpose: 
//   This is the preferred constructor for the DynamicMovingGrids class.
//
// /compositeGrid\_: use this basic initial grid from which to build
//     a list of component grids that are to move
// /genericGridMotion\_: information about how the grids are to move
//     is encapsulated in an object whose class is derived from 
//     the GenericGridMotion base class
// /ogenGridGenerator\_: use this instance of the grid generator to 
//     overlap the moving grids
// /ps\_: optional pointer to a GenericGraphicsInterface object; used in debugging mode
//       to make intermediate plots.
//
//\end{DynamicMovingGrids.tex}
{
  useOgen           = LogicalTrue;
  ps                = ps_;
  ogenGridGenerator = ogenGridGenerator_;
  initialize (compositeGrid_, genericGridMotion_);
}

/* ****** GONE V16
void DynamicMovingGrids::
initialize (CompositeGrid& compositeGrid_,
	    GenericGridMotion* genericGridMotion_,
	    Cgsh* gridGenerator_)
//
// This version of initialize is obsolete in Overture v15.
//

{
  cout << "This version of DynamicMovingGrids::initialize is obsolete with Overture v15" << endl;
  cout << "Please use initialize(CompositeGrid&, GenericGridMotion*, Ogen*) instead " << endl;
  exit (-1);
  
  if (LogicalFalse) 
  {
    useOgen            = LogicalFalse;
    gridGenerator      = gridGenerator_;
    initialize (compositeGrid_, genericGridMotion_);
  }
  
}

********** */

//\begin{>>DynamicMovingGrids.tex}{\subsubsection{initialize}}
void DynamicMovingGrids::
initialize (CompositeGrid& compositeGrid_,
	    GenericGridMotion* genericGridMotion_,
	    Ogen* ogenGridGenerator_)
//
// /Purpose:
//    Initialize objects in the DynamicMovingGrids class.  Normally
//    this function is not called directly by the user, unless the
//    default constructor has been called.
//
// /compositeGrid\_: use this basic initial grid from which to build
//     a list of component grids that are to move
// /genericGridMotion\_: information about how the grids are to move
//     is encapsulated in an object whose class is derived from 
//     the GenericGridMotion base class
// /ogenGridGenerator\_: use this instance of the grid generator to 
//     overlap the moving grids
//\end{DynamicMovingGrids.tex}
{
  useOgen            = LogicalTrue;
  ogenGridGenerator  = ogenGridGenerator_;
  initialize (compositeGrid_, genericGridMotion_);
}


void DynamicMovingGrids::
initialize (CompositeGrid& compositeGrid_,
	    GenericGridMotion* genericGridMotion_)
//
// Private initialization function for DynamicMovingGrids
//
{
  mainConstructorCalled = LogicalTrue;
  
  compositeGridListInitialized = LogicalFalse;
  movingGridMappingsSet        = LogicalFalse;

  //...copy pointers of input GenericGridMotion object and CompositeGrid
  genericGridMotion = genericGridMotion_;
  gridsHaveMoved   = LogicalFalse;
  
  //...internal copies of important constants
  numberOfGrids      = compositeGrid_.numberOfGrids();
  numberOfLevels     = genericGridMotion->numberOfLevels();
  numberOfDimensions = compositeGrid_.numberOfDimensions();

  //...get hasMoved array from GenericGridMotion object's notion of it
  hasMoved.resize(numberOfGrids);
  //hasMoved = genericGridMotion->getHasMoved();
  genericGridMotion->getHasMoved (hasMoved);

  //...this is what gets updated when the grids are moved
  whatToUpdate = CompositeGrid::THEcenter;
  
  //...meaningless for less than two levels
  assert(numberOfLevels >= 2);
  initializeCompositeGridList (compositeGrid_);
  setMovingGridMappings();

  firstTimeToMoveGrids = LogicalTrue;
}


void DynamicMovingGrids::
initializeCompositeGridList (CompositeGrid& compositeGrid_)
//
// /Purpose: This protected function makes a list of pointers to CompositeGrid's
//           which will be used to store the grids at the different time levels
//
// 
{
   Overture::abort("This no longer works");
/* ---  
  cout << "DynamicsMovingGrids::constructing CompositeGrid** cgMoving..." << endl;
  
  cgMoving = new CompositeGrid*[numberOfLevels]();
  assert (cgMoving != NULL);
  int level;
  
  for (level=0; level<numberOfLevels; level++)
  {
    cgMoving[level] = new CompositeGrid();
  }
  //...first one is a reference; rest are copies
  cgMoving[0]->reference(compositeGrid_);
  
  for (level=1; level<numberOfLevels; level++)                             // *** SHOULD THESE BE REFS TOO? OR WILL THAT WORK?
    *cgMoving[level] = compositeGrid_;

  compositeGridListInitialized = LogicalTrue;

  //...if we know about plotting then plot the grids
  if (ps != NULL)
  {
    psp.set (GI_TOP_LABEL, "DynamicMovingGrids: initial grids");
    for (int level=0; level<numberOfLevels; level++) PlotIt::plot(*ps,*cgMoving[level],psp);
  }
  --- */  
}

void DynamicMovingGrids::
destructCompositeGridList()
//
// /Purpose: (protected class function) this routine should be the inverse of
//           DynamicMovingGrids::initializeCompositeGridList
// /Author: DLB 980327
{
  cout << "DynamicMovingGrids::destructing CompositeGrid** cgMoving.." << endl;
  
//...shouldn't have to do this because we use delete[] below
//  for (int level=0; level<numberOfLevels; level++)
//    delete cgMoving[level];

  delete[] cgMoving;
  
  compositeGridListInitialized = LogicalFalse;
}

  
void DynamicMovingGrids::
setMovingGridMappings ()
{
  cout << "DynamicMovingGrids::setMovingGridMappings called..." << endl;
  
  //...(1) Loop over grids and mappings and figure out which mappings will "move".
  //   (2) Reference the CompositeGrid mappings to these new mappings
  //   (3) Make sure the hasMoved array is consistent with this
  //   Get this information from the GenericGridMotion object
  //

  // *** debug: before changing the mappings, see if the center array is there
  //(*cgMoving[0])[0].center().display("center array at beginning of setMovingGridMappings");
  
  int level;
  
  for (level=0; level<numberOfLevels; level++)
  {
    for (int grid=0; grid<numberOfGrids; grid++)
    {
      Mapping* thisMovingMapping = genericGridMotion->mapping(grid,level);
      if (thisMovingMapping != NULL)
      {
	assert (hasMoved(grid));
	(*cgMoving[level])[grid].reference(*thisMovingMapping);
      }
      if (thisMovingMapping == NULL) assert (!hasMoved(grid));
    }
    cgMoving[level]->update();
  }
    //...destroy all the data on the level>0 grids to save space; they will share data 
  //   with cgMoving[0]

  // *** NOTE: since the 0th and lst grids get swapped, we really want to
  // destroy the data on the 0th grid but not the last
  for (level=0; level<numberOfLevels-1; level++)
    cgMoving[level]->destroy(CompositeGrid::EVERYTHING);

  movingGridMappingsSet = LogicalTrue;
}

void DynamicMovingGrids::
swapCompositeGrids()
//
// /Purpose: a protected helper function to swap the composite grids
//           called by movedGrids
//
{
  CompositeGrid* temp;
  temp = cgMoving[0];
  cgMoving[0] = cgMoving[numberOfLevels-1];
  cgMoving[numberOfLevels-1] = temp;
}

//\begin{>>DynamicMovingGrids.tex}{\subsubsection{updateMovedGrids}}
void DynamicMovingGrids::
updateMovedGrids (const real & time,
		  const real & timestep)

//
// /Purpose: This is the function that calls the GenericGridMotion object's
//           function to "move" the mappings, and then calls the grid generator
//           to update the overlap.  To get the list of moving grids, use the 
//           DynamicMovingGrids::movedGrids() function.
//
// /time:     time at beginning of timestep
// /timestep: size of this timestep (determines how far to move the grids)
//
//\end{DynamicMovingGrids.tex}

{
  //... this is not the right interface for the nonlinear problem, so punt if
  //    the gridMotion class says its nonlinear

  bool isNonlinear = genericGridMotion->isNonlinear();
  assert (!isNonlinear);

  //...swap pointers between the 0th and last grid since the old last grid becomes the
  //   new 0th grid at each timestep

  assert (mainConstructorCalled);
  
  if (!compositeGridListInitialized || !movingGridMappingsSet)
  {

    cout << "DynamicMovingGrids::movedGrids: class data has not been initialized" << endl;
    cout << "Use DynamicMovingGrids(CompositeGrid&, GenericGridMotion*, Ogen*) to initialize" << endl;
    assert (compositeGridListInitialized);
    assert (movingGridMappingsSet);
  }
  
  swapCompositeGrids();
  genericGridMotion->swapMappings();
  
    real fracTimestep;
    int level;
    
    switch(firstTimeToMoveGrids)
    {
    case LogicalTrue:
      //
      //...first time through, "move" from original grid, ie. a fractional timestep
      //
      for (level=1; level<numberOfLevels; level++)
      {
	fracTimestep = level*timestep/(numberOfLevels-1);
	genericGridMotion->moveMappings (time, fracTimestep, level);
      }
      firstTimeToMoveGrids = LogicalFalse;
      
      break;

    case LogicalFalse:
      //
      //...if not first time through, "move" from same level grid at previous timestep
      //...so this is a full timestep, except for the final level, which moves TWO
      //...timesteps since it is actually swapped
      //
      for (level=1; level<numberOfLevels; level++)
      {
	fracTimestep = level != numberOfLevels-1 ? timestep : timestep*2.;
	genericGridMotion->moveMappings (time, fracTimestep, level);
      }
      break;
    };

// ***WE SHOULD BE MORE SOPHISTICATED ABOUT THIS
//    bool resetToFirstPriority = LogicalTrue;
    Ogen::MovingGridOption minimizeOverlap = Ogen::minimizeOverlap;


    //...now recompute overlap on all the levels but the initial one

    for (level=1; level<numberOfLevels; level++)
    {
     
      if (useOgen)
	 ogenGridGenerator->updateOverlap(*cgMoving[level], *cgMoving[level-1], hasMoved, minimizeOverlap);
      else
      {
//	 gridGenerator->updateOverlap(*cgMoving[level], *cgMoving[level-1], hasMoved, resetToFirstPriority);
	cout << "DynamicMovingGrids::updateMovedGrids: ERROR: initialize apparently not called" << endl;
	exit (-1);
      }
      
      cgMoving[level]->update(whatToUpdate);
    }

    gridsHaveMoved = LogicalTrue;
    
}


//\begin{>>DynamicMovingGrids.tex}{\subsubsection{movedGrids}}
CompositeGrid** DynamicMovingGrids::
movedGrids()
//
// /Purpose: return pointer to list of pointers to moved grids
//\end{DynamicMovingGrids}
{
  assert (gridsHaveMoved);
  
  CompositeGrid** cgtemp = cgMoving;
  return (cgtemp);
}

//\begin{>>DynamicMovingGrids.tex}{\subsubsection{movedGrid}}
CompositeGrid* DynamicMovingGrids::
movedGrid (const int & level)
//
// /Purpose: return pointer to one moved grid at a particular level
// /level: which level (time substep) to return the grid for
//\end{DynamicMovingGrids}
{
  assert (gridsHaveMoved);
  return (cgMoving[level]);
}


//\begin{>>DynamicMovingGrids.tex}{\subsubsection{getGridVelocity}}
void DynamicMovingGrids::
getGridVelocity (realCompositeGridFunction& velocity,
		 const int & level,
		 const real & time)
//  /Purpose: return the grid velocity function for the level'th grid; this can call
//   a function in GenericGridMotion, or compute it by differencing (the latter 
//   not implemented 980326)
//  /velocity: velocity is returned in this gridFunction
//  /level:    velocity returned for this level (time substep)
//  /time:     velocity returned for this time (note that this is the time at the 
//             level of interest, not at the beginning of the timestep.
//\end{DynamicMovingGrids.tex}
//
{
  assert (mainConstructorCalled);
  
  bool analyticVelocityAvailable = genericGridMotion->analyticVelocityAvailable();
  Index I1, I2, I3;
  velocity.updateToMatchGrid 
    (*cgMoving[level], GridFunctionParameters::defaultCentering, numberOfDimensions);

  int grid;
  
  switch (analyticVelocityAvailable)
  {
  case (LogicalTrue):

    for (grid=0; grid<numberOfGrids; grid++)
    {
      MappedGrid& mg = (*cgMoving[level])[grid];
      getIndex (mg.dimension(), I1, I2, I3);
      genericGridMotion->getAnalyticVelocity 
	(velocity[grid], grid, level, time, cgMoving, I1, I2, I3);
    }
    
    break;
    
  case (LogicalFalse):
    cout << "DynamicMovingGrids::getGridVelocity: " << endl;
    cout << "the GenericGridMotion object associated with this class does not have " << endl;
    cout << "  an analytic velocity function available." << endl;
    cout << "In principle this class should now compute the velocity by differencing, but" << endl;
    cout << "  that's not implemented yet. Sorry." << endl;
    assert (LogicalFalse);
    break;
  };
}

  
    
  
  


  

	
	    

  
  
  
