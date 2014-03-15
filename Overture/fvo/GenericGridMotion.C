#include "GenericGridMotion.h"

//\begin{>>GenericGridMotion.tex}{\subsubsection{GenericGridMotion default constructor}}
GenericGridMotion::
GenericGridMotion ()
//
// /Purpose: Default constructor for the GenericGridMotion base class.
//
//\end{GenericGridMotion.tex}
{
  cout << "GenericGridMotion: default constructor called" << endl;
  numberOfGrids_ = -1;
  numberOfLevels_= -1;
  analyticVelocityAvailable_ = LogicalFalse;
  movingMappingHasBeenSetup  = LogicalFalse;
}

GenericGridMotion::
GenericGridMotion (CompositeGrid & cg_, 
		   const int& numberOfLevels__)
{
  initialize (cg_, numberOfLevels__);
}

void GenericGridMotion::
initialize (CompositeGrid & cg_,
	    const int& numberOfLevels__)
{
  
  cout << "GenericGridMotion(CompositeGrid&, int&) constructor called " << endl;
  movingMappingHasBeenSetup = LogicalFalse;
  
  cg.reference(cg_);
  numberOfLevels_     = numberOfLevels__;
  numberOfGrids_      = cg.numberOfGrids();
  numberOfDimensions_ = cg.numberOfDimensions();

  setupMovingMappingArray (numberOfGrids_, numberOfLevels_);

  hasMoved_.resize(numberOfGrids_);
  hasMoved_ = LogicalFalse;

  analyticVelocityAvailable_ = LogicalFalse;
  isNonlinear_ = LogicalFalse;

}

//\begin{>>GenericGridMotion.tex}{\subsubsection{GenericGridMotion copy constructor}}
GenericGridMotion::
GenericGridMotion (const GenericGridMotion& ggm)
  :
  numberOfLevels_            (ggm.numberOfLevels_),
  numberOfGrids_             (ggm.numberOfGrids_),
  numberOfDimensions_        (ggm.numberOfDimensions_),
  hasMoved_                  (ggm.hasMoved_),
  analyticVelocityAvailable_ (ggm.analyticVelocityAvailable_),
  isNonlinear_               (ggm.isNonlinear_)
//
// /Purpose: copy constructor
//
//\end{GenericGridMotion.tex}
{
  cout << "GenericGridMotion::copy constructor called" << endl;
  cg.reference (ggm.cg);
  //...deep copy of movingMapping array
  //...first set it up
  setupMovingMappingArray (numberOfGrids_, numberOfLevels_);
  //...then copy the Mapping's
  copyMovingMappingArray (ggm.movingMapping);
}

GenericGridMotion& GenericGridMotion::
operator= (const GenericGridMotion& ggm)
{
  cout << "GenericGridMotion operator= called..." << endl;
  
  numberOfLevels_ =            ggm.numberOfLevels_;
  numberOfGrids_  =            ggm.numberOfGrids_;
  numberOfDimensions_ =        ggm.numberOfDimensions_;
  hasMoved_           =        ggm.hasMoved_;
  analyticVelocityAvailable_ = ggm.analyticVelocityAvailable_;
  isNonlinear_ =               ggm.isNonlinear_;
  
  cg.reference (ggm.cg);

  //...deep copy of the movingMapping array
  //...first set it up
  setupMovingMappingArray (numberOfGrids_, numberOfLevels_);
  //...then copy the Mapping's
  copyMovingMappingArray (ggm.movingMapping);

  return *this;
  
}


GenericGridMotion::
~GenericGridMotion()
{
  cout << "GenericGridMotion: destructor called" << endl;
  //...delete the mappings
  destructMovingMappingArray();
  
}

void GenericGridMotion::
swapMappings ()
//
// /Purpose: this function swaps the Mappings corresponding to the 
//           first and last levels in the Mapping list.
//           This is needed if the grids are being swapped in a similar
//           way by the DynamicMovingGrids class.
{
  Mapping** tempM;
  tempM = new Mapping*[numberOfGrids_];
  assert (tempM != NULL);

  for (int grid=0; grid<numberOfGrids_; grid++)
  {
    tempM[grid] = movingMapping[grid][numberOfLevels_-1];
    movingMapping[grid][numberOfLevels_-1] = movingMapping[grid][0];
    movingMapping[grid][0] = tempM[grid];
  }
}


void GenericGridMotion::
setupMovingMappingArray (int& numberOfGrids, int& numberOfLevels)
//
// /Purpose: this (protected) function sets up the mappingPointerArray, and 
//           sets all the Mapping*'s to be NULL. The derived class must
//           actually make (new) the derived Mappings as appropriate
// /Note:    Changes to this routine will require changes to 
//           GenericGridMotion::destructMovingMappingArray() as well
{
  assert ( !movingMappingHasBeenSetup );

  int nGrids = numberOfGrids;
  int nLevels = numberOfLevels;
  
  movingMapping = new Mapping**[nGrids];
  assert (movingMapping != NULL);
  
  for (int grid=0; grid<nGrids; grid++)
  {
    movingMapping[grid] = new Mapping* [nLevels];
    assert (movingMapping[grid] != NULL);
    for (int level=0; level<nLevels; level++)
      movingMapping[grid][level] = NULL;
  }
  
  movingMappingHasBeenSetup = LogicalTrue;
  
}

void GenericGridMotion::
copyMovingMappingArray (Mapping*** movingMapping_)
//
// /Purpose: protected function to make a deep copy of the
//           movingMapping_ array; it is copied into this->movingMapping.
//
{
  cout << "GenericGridMotion::copyMovingMappingArray " << endl;

  assert (movingMappingHasBeenSetup);
  
  for (int grid=0; grid<numberOfGrids_; grid++)
    for (int level=0; level<numberOfLevels_; level++)
      *movingMapping[grid][level] = *movingMapping_[grid][level];

}

void GenericGridMotion::
destructMovingMappingArray ()
//
// /Purpose: protected function to destruct Mapping*** movingMapping.
//           This should be the inverse of setupMovingMappingArray
//
{
  cout << "GenericGridMotion::destructMovingMappingArray called" << endl;
  
  for (int grid=0; grid<numberOfGrids_; grid++)
    delete[] movingMapping[grid];
  
  delete[] movingMapping;

}

// PURE VIRTUAL FUNCTIONS

/*--
//\begin{>>GenericGridMotion.tex}{\subsubsection{moveMappings}}
virtual void GenericGridMotion::
moveMappings (const real & time,
	      const real & timestep, 
	      const int & level)=0
//
// /Purpose:   
//   This pure virtual function describes the interface by which the grids are "moved".
//   The derived class must implement this function to move the mappings for a given level in the 
//   movingMapping list an amount corresponding to the value of "timestep".
//   The base class doesn't know how to do this, so this is a pure virtual fn.
//\end{GenericGridMotion.tex}
--*/


/*--
//\begin{>>GenericGridMotion.tex}{\subsubsection{moveMappings (nonlinear case)}}
virtual void GenericGridMotion::
moveMappings (const real & time,
	      const real & timestep,
	      const realCompositeGridFunction & u,
	      const int & level)=0
//
// /Purpose:
//   This pure virtual function describes the interface by which the grids are "moved".
//   The derived class must implement this function to move the mappings for a given level in the 
//   movingMapping list an amount corresponding to the value of "timestep".
//   The base class doesn't know how to do this, so this is a pure virtual fn.
//   This is the "nonlinear" version of this function, used when the grid
//   motion depends on the solution of the PDE
//\end{GenericGridMotion.tex}
--*/

/*--
//\begin{>>GenericGridMotion.tex}{\subsubsection{getAnalyticVelocity}}
virtual void GenericGridMotion::
getAnalyticVelocity(realMappedGridFunction& velocity,
		    const int& grid,
		    const int& level,
		    const real& time,
		    CompositeGrid** cgMoving,
		    const Index& I1=nullIndex,
		    const Index& I2=nullIndex,
		    const Index& I3=nullIndex)=0
//
// /Purpose:
//    This pure virtual function defines the interface for returning the grid velocity at
//    all points on a mappedGrid.  It is optionally called by the 
//    DynamicMovingGrids class in order to return the grid velocity on an overlapping
//    grid.  This function is normally implemented in a derived class
//    if an analytic function is available that describes the grid velocity.  If this 
//    function is implemented, then the class member analyticVelocityAvailable\_ must
//    be set to LogicalTrue in the derived class'es constructor.
// /velocity (output):  the returned values of the grid velocity
// /grid (input):  which grid to return the velocity for
// /level (input): which level (time substep) t
// /time (input): what time
// /cgMoving (input): pointer to list of CompositeGrid's at the different time levels
//\end{GenericGridMotion}

--*/

/*--
//\begin{>>GenericGridMotion.tex}{\subsubsection{initializeMappings}}
virtual void GenericGridMotion::
initializeMappings()
//
// /Purpose:
//   initialize the moving Mappings and their parameters.  This function is pure virtual
//   since the base class doesn't acutally know what mappings are involved.  See the 
//   MatrixTransformGridMotion class to see an example of what needs to be done.
//
//\end{GenericGridMotion.tex}
--*/



