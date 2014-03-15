#include "MatrixTransformGridMotion.h"

MatrixTransformGridMotion::
MatrixTransformGridMotion ()
{
  cout << "MatrixTransformGridMotion: default constructor called" << endl;
  paramsSet = LogicalFalse;
  
}

/*
MatrixTransformGridMotion::
MatrixTransformGridMotion (CompositeGrid & cg_, 
			   const int& numberOfLevels__,
			   MatrixTransformGridMotionParameters* params_):
  GenericGridMotion (cg_, numberOfLevels__)
{
  cout << "MatrixTransformGridMotion(CompositeGrid&, int&) constructor called " << endl;
  params =  params_;
  paramsSet = LogicalTrue;

  //...the derived class is responsible for correctly setting the following parameters:
  //...look in params to find hasMoved info
  hasMoved_ = params->movingGrid;
  analyticVelocityAvailable_ = LogicalTrue;
  isNonlinear_ = LogicalFalse;

  //...the derived class is responsible for the Mapping initialization specific to this class
  initializeMappings ();
  
}
*/
MatrixTransformGridMotion::
MatrixTransformGridMotion (CompositeGrid & cg_, 
			   const int& numberOfLevels__,
			   MatrixTransformGridMotionParameters* params_)
{
  initialize (cg_, numberOfLevels__, params_);
}

void MatrixTransformGridMotion::
initialize(CompositeGrid & cg_, 
	   const int& numberOfLevels__,
	   MatrixTransformGridMotionParameters* params_)
{
  GenericGridMotion::initialize (cg_, numberOfLevels__);
  
  cout << "MatrixTransformGridMotion(CompositeGrid&, int&) constructor called " << endl;
  params =  params_;
  paramsSet = LogicalTrue;

  //...the derived class is responsible for correctly setting the following parameters:
  //...look in params to find hasMoved info
  hasMoved_ = params->movingGrid;
  analyticVelocityAvailable_ = LogicalTrue;
  isNonlinear_ = LogicalFalse;

  //...the derived class is responsible for the Mapping initialization specific to this class
  initializeMappings ();
  
}


MatrixTransformGridMotion::
~MatrixTransformGridMotion()
{
  cout << "MatrixTransformGridMotion destructor called" << endl;
}


//\begin{>>MatrixTransformGridMotion.tex}{\subsubsection{moveMappings}}
void MatrixTransformGridMotion::
moveMappings (const real & time,
	      const real & timestep,
	      const int & level)
//
// /Purpose: derived-class-specific function to "move" the Mapping's
// /time: time at beginning of timestep
// /timestep: size of timestep
// /level: which fractional timestep level
//\end{MatrixTransformGridMotion.tex}

{
  cout << "***MatrixTransformGridMotion::moveMappings ("<< timestep <<","<<level<<") called" << endl;

  real angle, angle0, angle1, tnew;
  real trans[3];
//  real degRadConv = Pi/static_cast<real>(180.);
//  real degConv    = 360.;
  real radConv    = Pi*static_cast<real>(2.0);

  int i, axis;
  
  for (int grid=0; grid<numberOfGrids_; grid++)
  { 
    if (params->movingGrid(grid))
    {
      switch (params->useMotionFunction(grid))
      {
      case LogicalFalse:

	angle = params->rotationRate(grid)*radConv*timestep;
	for (i=0; i<3; i++) trans[i] = params->translationRate(grid,i)*timestep;
	break;
	
      case LogicalTrue:
	tnew = time + timestep;
	angle0 = params->motionFunction(grid)->rotationalMotion(time);
	angle1 = params->motionFunction(grid)->rotationalMotion(tnew);
	angle = (angle1-angle0)*radConv;
	
	for (axis=0; axis<numberOfDimensions_; axis++) 
	  trans[axis] = params->motionFunction(grid)->translationalVelocity(time,axis)*timestep;
	break;
      };
      	
//	(dynamic_cast<MatrixTransform*>
         ((MatrixTransform*)
	 (movingMapping[grid][level]))->rotate(axis3,angle);
//	(dynamic_cast<MatrixTransform*>
	 ((MatrixTransform*)
	 (movingMapping[grid][level]))->shift (trans[0],trans[1],trans[2]);
    }
  }
  
}
void MatrixTransformGridMotion::
moveMappings (const real & time,
	      const real & timestep,
	      const realCompositeGridFunction &u,
	      const int & level)
{
  cout << "***MatrixTransformGridMotion::moveMappings (real&, realCompositeGridFunction &int&) called" << endl;
  moveMappings (time, timestep, level);
}

//\begin{>>MatrixTransformGridMotion.tex}{\subsubsection{getAnalyticVelocity}}
void MatrixTransformGridMotion::
getAnalyticVelocity (realMappedGridFunction& gridVelocity,
		     const int & grid,
		     const int & level,
		     const real & time,
		     CompositeGrid** cgMoving,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3)
//
// /Purpose: return the grid velocity on a given component grid at a given
//           level and given time
//\end{MatrixTransformGridMotion.tex}
{
  cout << "MatrixTransformGridMotion::getAnalyticVelocity called..." << endl;
  
  assert (paramsSet);
  // ***only 2D right now
  assert (numberOfDimensions_ == 2);

  real radConv    = Pi*static_cast<real>(2.0);

  Index ND;
  ND = Index(0,numberOfDimensions_);
  realMappedGridFunction x,y;

  MappedGrid& mg = (*cgMoving[level])[grid];
    
  switch (hasMoved_(grid))
  {
  case LogicalFalse:

    gridVelocity(I1,I2,I3,ND) = static_cast<real>( 0.0 );
    break;
      
  case LogicalTrue:

    switch (params->useMotionFunction(grid))
    {
    case LogicalTrue:
      gridVelocity(I1,I2,I3,axis1) = 
	- params->motionFunction(grid)->rotationalVelocity(time) * radConv * mg.center()(I1,I2,I3,axis2)
	+ params->motionFunction(grid)->translationalVelocity(time,axis1);
      gridVelocity(I1,I2,I3,axis2) =
	+ params->motionFunction(grid)->rotationalVelocity(time) * radConv * mg.center()(I1,I2,I3,axis1)
	+ params->motionFunction(grid)->translationalVelocity(time,axis2);
      break;
      
    case LogicalFalse:
      gridVelocity(I1,I2,I3,axis1) = 
	-params->rotationRate(grid) * radConv * mg.center()(I1,I2,I3,axis2) +
	params->translationRate(grid,axis1);
      gridVelocity(I1,I2,I3,axis2) =  
	params->rotationRate(grid) * radConv * mg.center()(I1,I2,I3,axis1) +
	params->translationRate(grid,axis2);
      break;
    };
      
    break;
  };


}

/* *** OBSOLETE CODE
void MatrixTransformGridMotion::
getAnalyticVelocity (realCompositeGridFunction& gridVelocity,
		     const int & level,
		     const real & time,
		     CompositeGrid** cgMoving)
//
// /Purpose: return the grid velocity
// /level:  which level the velocity is to be computed for
// /cgMoving: pointer to list of "moving" CompositeGrid's
//
{

  cout << "MatrixTransformGridMotion::getAnalyticVelocity called..." << endl;
  
  assert (paramsSet);
  // ***only 2D right now
  assert (numberOfDimensions_ == 2);

  gridVelocity.updateToMatchGrid
    (*cgMoving[level], GridFunctionParameters::defaultCentering, numberOfDimensions_);

  Index I1,I2,I3,ND;
  ND = Index(0,numberOfDimensions_);
  realMappedGridFunction x,y;

  for (int grid=0; grid<numberOfGrids_; grid++)
  {
    MappedGrid& mg = (*cgMoving[level])[grid];
    getIndex (mg.dimension(), I1, I2, I3);
    
    switch (hasMoved_(grid))
    {
    case LogicalFalse:
      //gridVelocity[grid](I1,I2,I3,ND) = (real) 0.0;
      gridVelocity[grid](I1,I2,I3,ND) = static_cast<real>( 0.0 );
      break;
      
    case LogicalTrue:

      switch (params->useMotionFunction(grid))
      {
      case LogicalTrue:
	// ***FIX THIS (CODE MISSING)
      case LogicalFalse:
	gridVelocity[grid](I1,I2,I3,axis1) = 
	  -params->rotationRate(grid)*Pi/180.*mg.center()(I1,I2,I3,axis2) +
	  params->translationRate(grid,axis1);
	gridVelocity[grid](I1,I2,I3,axis2) =  
	  params->rotationRate(grid)*Pi/180.*mg.center()(I1,I2,I3,axis1) +
	  params->translationRate(grid,axis2);
	break;
      };
      
      break;
    };
  }

}
     
*/      
void MatrixTransformGridMotion::
initializeMappings ()
// 
// /Purpose: initialization specific to the use of the MatrixMapping class is
//           done here.
{

  for (int grid=0; grid<numberOfGrids_; grid++)
  {
    if (params->movingGrid(grid))
    {
      //...get a pointer to the corresponding mapping in the composite grid
      Mapping &mappingToMove = *(cg[grid].mapping().mapPointer);

      //...make MatrixMapping's and store them in the movingMapping array
      for (int level=0; level<numberOfLevels_; level++)
	movingMapping[grid][level] = new MatrixTransform(mappingToMove);
    }
  }
}
