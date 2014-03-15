#include "Projection.h"
// ===============================================================================
void Projection::
applyRightHandSideBoundaryConditions (REALCompositeGridFunction & ellipticRightHandSide,
				      const REALCompositeGridFunction & uStar,
				      const int & level,
				      const real & vTime,
				      const real & pTime)
//
// /Purpose:
//    This routine sets the right-hand-side forcing for the boundary conditions
//    for the elliptic solve. It must be called after the right hand side interior
//    values have been set, but before calling the Oges.solve routine
//    Values of uStar are extrapolated to the boundary face using a three point formula
//    old routine (two point average with ghost cell value) in applyRightHandBoundaryConditions.C.961101
//
//    960911: implemented homogeneousNeumann, fixedWallNeumann, movingWallNeumann
//            cases
//    /vTime: used for twilight zone case: time level for velocity
//    /pTime: used for twilightzone case: time level for pressure
//
//    /Limitations: assumes that there are at least three interior cells to extrapolate values
//                  of uStar and uGrid from.
// /Author: D. L. Brown
//    
// 
// ===============================================================================
{
//  bool TESTING_ONLY = FALSE; // *** try forcing with gradient of exact pressure in fixedWall bcs
  // int pCpt = numberOfDimensions;
  
// ========================================
// declarations:
// ========================================

  REAL ZERO = (real)0.0;
  REAL HALF = (real)0.5;
  int grid;
  int side;
  int axis; 
  int sideSign;
  Index I1b, I2b, I3b;         //Index'es of row of boundary cells
  Index I1g, I2g, I3g;         //Index'es of row of ghost cells
  Index I1f, I2f, I3f;         //Index'es for faceCentered boundary quantities
  Index I1m1, I2m1, I3m1, I1m2, I2m2, I3m2;

  real e0 =  1.875;  //third order extrapolation coefficients
  real e1 = -1.25;
  real e2 =  0.375;

  int xCpt = xComponent;
  int yCpt = yComponent;
  int zCpt = zComponent;
  
  
//  int bcComponent = 0;                              //component always 0 since its a scalar problem
//  int bcIndex = 0;                                  //index always 0 since there is only one bc on each side
  int cmpgrdBC;

  OGFunction *e = twilightZoneFlowFunction;
  if (twilightZoneFlow) assert (twilightZoneFlowFunction != NULL);

  ProjectionType projType = getProjectionType (uStar);
  
  // ... find out if any boundaries are moving; if so, allocate space for cellVelocity

  bool needCellVelocity = FALSE;
  
  ForAllGrids (grid)
    {
      MappedGrid & mg = compositeGrid[grid];
      ForBoundary (side,axis)
	{
	  cmpgrdBC = mg.boundaryCondition()(side,axis);
	  if (listOfBCs[cmpgrdBC] == movingWallNeumann) needCellVelocity = TRUE;
	}
    }

  if (needCellVelocity)
  {
    assert (movingGridsPointerIsSet);
    assert (movingGridsPointer!=0);
  }
  
  
  realCompositeGridFunction uGrid;
#ifdef V15
  if (needCellVelocity) movingGridsPointer->getGridVelocity (uGrid, level, vTime);
#else  
  if (needCellVelocity) uGrid = movingGridsPointer->getGridVelocity (level);
#endif
      

// ========================================
// Loop over all grids:
// ========================================
  
  ForAllGrids (grid)
  {
    MappedGrid & mg = compositeGrid[grid];

// ========================================    
//   Loop over all boundaries:
// ========================================
    ForBoundary (side, axis)
    {
      sideSign = side==0 ? -1 : 1;  // ... used to orient normal vector correctly depending on side of grid

// ========================================
//     If non trivial boundary conditions:
// ========================================

      cmpgrdBC = mg.boundaryCondition()(side,axis);
      if (cmpgrdBC > 0)
      {
	

// ========================================
//       If an exact velocity is being used:
//         get Indexes for boundary and ghost rows
// ========================================
      
	if (useExactVelocity)
	{
	  getGhostIndex    (mg.indexRange(), side, axis, I1g, I2g, I3g, +1);  //ghost cells
	  getBoundaryIndex (mg.indexRange(), side, axis, I1b, I2b, I3b);      //boundary cells
	  getGhostIndex    (mg.indexRange(), side, axis, I1m1, I2m1, I3m1, -1);      //first row inside
	  getGhostIndex    (mg.indexRange(), side, axis, I1m2, I2m2, I3m2, -2);      //second row inside
		
	  I1f = I1b + inc(axis,0)*side;                                     //boundary face Index'es
	  I2f = I2b + inc(axis,1)*side;
	  I3f = I3b + inc(axis,2)*side;
	}
	

// ========================================
//       Determine which boundary condition to use from cmpgrd BCs
//       Switch on the boundary condition:
//         If exact velocity, set boundaryConditionRightHandSide
//         else: set a constant value
// ========================================
	
	switch (listOfBCs[cmpgrdBC]){

	  //====================
	case cheapNeumann: //this really isn't Neumann, just a (-1,1) difference at the boundary
	  //====================
	  
	// ... 981110 this isn't going to work at all since FVO's do Neumann correctly now.

	  if (useExactVelocity)
	  {
	    cout << "WARNING: applyRightHandSideBoundaryConditions:: cheapNeuman/useExactVelocity " << endl;
	    cout << "    THIS WILL BE COMPLETELY WRONG" << endl;
	    ellipticRightHandSide[grid](I1g,I2g,I3g) =
	      exactPhi[grid](I1b,I2b,I3b) - exactPhi[grid](I1g,I2g,I3g);
	  }
	  else
	  {
	    ellipticRightHandSide[grid](I1g,I2g,I3g) = ZERO;
	  }
	  break; //cheapNeumann

	  //====================
	case homogeneousNeumann: //...this really is homogeneous, in case you don't want me messing with
	  //====================      extrapolating uStar and so forth (i.e. you want d(phi)/dn == 0, really).

	  ellipticRightHandSide[grid](I1g,I2g,I3g) = ZERO;
	    

	  break; //homogeneousNeumann
	  //====================
	case valueGiven:
	  //====================
	  if (useExactVelocity)
	  {
	    ellipticRightHandSide[grid](I1g,I2g,I3g) =
	      HALF*(exactPhi[grid](I1b,I2b,I3b) + exactPhi[grid](I1g,I2g,I3g));
	  }
	  else
	  {
	    ellipticRightHandSide[grid](I1g,I2g,I3g) = ZERO;
	    cout << "project: Warning: a valueGiven BC has been specified; setting value to ZERO..." << endl;
	  }
	  break; //valueGiven
//	  break;

	  //====================
	case normalValueGiven:
          //====================
	  cout << "Projection::applyRightHandSideBoundaryConditions: normalValueGiven is not a reasonable choice for an elliptic BC" << endl;
	  assert (listOfBCs[cmpgrdBC] != normalValueGiven);
	  break; //normalValueGiven

	   
	  //====================
	case normalDerivativeGiven: //...obsolete; retained for back-compatibility
	case fixedWallNeumann:
	  //====================
	  // ... set RHS to n.uStar since n.uNew assumed zero; implementation depends on projectionType
	  // ... 961104: use 3rd order extrapolation from interior on uStar and e->u

	  switch (projType)
	  {
	  case (approximateProjection):

	    switch (numberOfDimensions){
	    
	    case (oneDimension):
	      ellipticRightHandSide[grid](I1g,I2g,I3g) =
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		(e0*uStar[grid](I1b ,I2b ,I3b ,xCpt) + 
		 e1*uStar[grid](I1m1,I2m1,I3m1,xCpt) + 
		 e2*uStar[grid](I1m2,I2m2,I3m2,xCpt));


	      break;
	    
	    case (twoDimensions):
	      ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		 (e0*uStar[grid](I1b ,I2b ,I3b ,xCpt) + 
		  e1*uStar[grid](I1m1,I2m1,I3m1,xCpt) + 
		  e2*uStar[grid](I1m2,I2m2,I3m2,xCpt)) +

		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		 (e0*uStar[grid](I1b ,I2b ,I3b ,yCpt) + 
		  e1*uStar[grid](I1m1,I2m1,I3m1,yCpt) + 
		  e2*uStar[grid](I1m2,I2m2,I3m2,yCpt)));


	      break;
	    
	    case (threeDimensions):
	      ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		 (e0*uStar[grid](I1b ,I2b ,I3b ,xCpt) + 
		  e1*uStar[grid](I1m1,I2m1,I3m1,xCpt) + 
		  e2*uStar[grid](I1m2,I2m2,I3m2,xCpt)) +

		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		 (e0*uStar[grid](I1b ,I2b ,I3b ,yCpt) + 
		  e1*uStar[grid](I1m1,I2m1,I3m1,yCpt) + 
		  e2*uStar[grid](I1m2,I2m2,I3m2,yCpt)) +

		 mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*
		 (e0*uStar[grid](I1b ,I2b ,I3b ,zCpt) + 
		  e1*uStar[grid](I1m1,I2m1,I3m1,zCpt) + 
		  e2*uStar[grid](I1m2,I2m2,I3m2,zCpt)));
	    
	      break;
	    }; //numberOfDimensions
	    break; //approximateProjection

	  case (macProjection):  //doesn't require extrapolation since values live on the faces
	    switch (numberOfDimensions){
	    
	    case (oneDimension):
	      ellipticRightHandSide[grid](I1g,I2g,I3g) =
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(uStar[grid](I1f,I2f,I3f,xCpt,axis));
	      break;
	    
	    case (twoDimensions):
	      ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*uStar[grid](I1f,I2f,I3f,xCpt,axis) +
		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*uStar[grid](I1f,I2f,I3f,yCpt,axis));

	      break;
	    
	    case (threeDimensions):
	      ellipticRightHandSide[grid](I1g,I2g,I3g) = 

		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*uStar[grid](I1f,I2f,I3f,xCpt,axis) +
		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*uStar[grid](I1f,I2f,I3f,yCpt,axis) +
		 mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*uStar[grid](I1f,I2f,I3f,zCpt,axis));
	    
	      break;
	    }; //numberOfDimensions

	    break; //macProjection
	    
	  case (macProjectionOfNormalVelocity):  //note that the usual normalVelocity is faceArea weighted and so has to be scaled for the BC
	    ellipticRightHandSide[grid](I1g,I2g,I3g) = sideSign*uStar[grid](I1f,I2f,I3f,axis)/mg.faceArea()(I1f,I2f,I3f,axis);

	    break; //macProjectionOfNormalVelocity
	      
	  }; //projType

	  
	  //... for twilightZoneFlow, subtract out the exact normal velocity from RHS for fixedWallNeumannn BCs
	  //... 981130: I don't think this is correct. BCs are determined by u = u* - grad(phi), which
	  //... doesn't involve the TZ function at all.

	  
	  if (twilightZoneFlow && LogicalFalse)
	    {
	      switch (numberOfDimensions){

	      case (oneDimension):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -=
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		  (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime) + 
		   e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		   e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime));


		break;
	    
	      case (twoDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -= 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime)) +

		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,yCpt,vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime)));

		break;
	    
	      case (threeDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -= 

		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		     (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime) + 
		      e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		      e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime)) +

		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		     (e0*(*e)(mg,I1b ,I2b ,I3b ,yCpt,vTime) + 
		      e1*(*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime) + 
		      e2*(*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime)) +

		   mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*
		     (e0*(*e)(mg,I1b ,I2b ,I3b ,zCpt,vTime) + 
		      e1*(*e)(mg,I1m1,I2m1,I3m1,zCpt,vTime) + 
		      e2*(*e)(mg,I1m2,I2m2,I3m2,zCpt,vTime)));
	    
		break;
	      }; //switch (numberOfDimensions)
	    } //if(twilightZoneFlow)


	  if (useExactVelocity)
	  //... the exactVelocity case is the same as twilightZone flow, except for the way that the "exact"
	  //    velocity is specified; 981130: no this is different.
	    {
	      switch (numberOfDimensions){

	      case (oneDimension):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -=
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		  mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(
		    e0*exactVelocity[grid](I1b ,I2b ,I3b ,xCpt) + 
		    e1*exactVelocity[grid](I1m1,I2m1,I3m1,xCpt) + 
		    e2*exactVelocity[grid](I1m2,I2m2,I3m2,xCpt,vTime)
		    );
		break;
	    
	      case (twoDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -= 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*(
		    
		    mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(
		      e0*exactVelocity[grid](I1b ,I2b ,I3b ,xCpt) + 
		      e1*exactVelocity[grid](I1m1,I2m1,I3m1,xCpt) + 
		      e2*exactVelocity[grid](I1m2,I2m2,I3m2,xCpt)) +

		    mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*(
		      e0*exactVelocity[grid](I1b ,I2b ,I3b ,yCpt) + 
		      e1*exactVelocity[grid](I1m1,I2m1,I3m1,yCpt) + 
		      e2*exactVelocity[grid](I1m2,I2m2,I3m2,yCpt))
		    );

		break;
	    
	      case (threeDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -= 

		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*(
		    
		    mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(
		      e0*exactVelocity[grid](I1b ,I2b ,I3b ,xCpt) + 
		      e1*exactVelocity[grid](I1m1,I2m1,I3m1,xCpt) + 
		      e2*exactVelocity[grid](I1m2,I2m2,I3m2,xCpt)) +

		    mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*(
		      e0*exactVelocity[grid](I1b ,I2b ,I3b ,yCpt) + 
		      e1*exactVelocity[grid](I1m1,I2m1,I3m1,yCpt) + 
		      e2*exactVelocity[grid](I1m2,I2m2,I3m2,yCpt)) +
		   
		    mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*(
		     e0*exactVelocity[grid](I1b ,I2b ,I3b ,zCpt) + 
		     e1*exactVelocity[grid](I1m1,I2m1,I3m1,zCpt) + 
		     e2*exactVelocity[grid](I1m2,I2m2,I3m2,zCpt))
		    );
	    
		break;
	      }; //switch (numberOfDimensions)
	    } //if(useExactVelocity)
	  
	  break; // fixedWallNeumann
	  
	  //====================
	case movingWallNeumann:
	  //====================

	  // ... set RHS to n.(uStar[grid] - uMoving)

	  if (!movingGridsPointerIsSet){
	    cout << "projection: ERROR: moving wall boundary condition specified, but no moving grid info" << endl;
	    cout << "  available. use Projection(CompositeGrid&, MovingGrids*) constructor to set the " << endl;
	    cout << "  MovingGrids object. " << endl;
	    throw "ERROR: Projection::movingGrids error";
	  }

	  // ... for the twilightZoneFlow case, the actual boundary velocity is not used; use the twz function instead
	  if (twilightZoneFlow)
	  {
	    switch (projType)
	    {
	    case (approximateProjection):
	      switch (numberOfDimensions){

	      case (oneDimension):
		ellipticRightHandSide[grid](I1g,I2g,I3g) =
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		    (e0*(uStar[grid](I1b ,I2b ,I3b ,xCpt) - (*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime)) +
		     e1*(uStar[grid](I1m1,I2m1,I3m1,xCpt) - (*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime)) +
		     e2*(uStar[grid](I1m2,I2m2,I3m2,xCpt) - (*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime)));
		break;
	    
	      case (twoDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,xCpt) - (*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,xCpt) - (*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,xCpt) - (*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime))) +
		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,yCpt) - (*e)(mg,I1b ,I2b ,I3b ,yCpt,vTime)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,yCpt) - (*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,yCpt) - (*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime))));
								    
		break;
	    
	      case (threeDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,xCpt) - (*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,xCpt) - (*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,xCpt) - (*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime))) +
		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,yCpt) - (*e)(mg,I1b ,I2b ,I3b ,yCpt,vTime)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,yCpt) - (*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,yCpt) - (*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime))) +
		 mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,zCpt) - (*e)(mg,I1b ,I2b ,I3b ,zCpt,vTime)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,zCpt) - (*e)(mg,I1m1,I2m1,I3m1,zCpt,vTime)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,zCpt) - (*e)(mg,I1m2,I2m2,I3m2,zCpt,vTime))));


		break;
	      }; //switch (numberOfDimensions)
	      break; //case(approximateProjection)

	    case (macProjection):
	      switch (numberOfDimensions){

	      case (oneDimension):
		ellipticRightHandSide[grid](I1g,I2g,I3g) =
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		  (uStar[grid] (I1f,I2f,I3f,xCpt,axis) -
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime)));

		break;
	    
	      case (twoDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,xCpt,axis) - 
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime))) +

		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		    (uStar[grid] (I1f,I2f,I3f,yCpt) -
		     (e0*(*e)(mg,I1b ,I2b ,I3b ,yCpt,vTime) + 
		      e1*(*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime) + 
		      e2*(*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime))));
			    
		break;
	    
	      case (threeDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,xCpt,axis) -
		    (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt,vTime) + 
		     e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		     e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime))) +

		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,yCpt,axis) -
		    (e0*(*e)(mg,I1b ,I2b ,I3b ,yCpt,vTime) + 
		     e1*(*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime) + 
		     e2*(*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime))) +

		   mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,zCpt,axis) -
		    (e0*(*e)(mg,I1b ,I2b ,I3b ,zCpt,vTime) + 
		     e1*(*e)(mg,I1m1,I2m1,I3m1,zCpt,vTime) + 
		     e2*(*e)(mg,I1m2,I2m2,I3m2,zCpt,vTime))));
	    
		break;

	      }; //switch (numberOfDimensions)
	      break; //case (macProjection)

	    case (macProjectionOfNormalVelocity):

	      // ... we already have normal velocities
	      ellipticRightHandSide[grid](I1g,I2g,I3g) = sideSign*uStar[grid](I1f,I2f,I3f,axis)/mg.faceArea()(I1f,I2f,I3f,axis);

	      // ... now subtract out tz function
	      switch (numberOfDimensions){

	      case (oneDimension):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -=
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		  (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt, vTime) + 
		   e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		   e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime));
		
		break;
	    
	      case (twoDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -= 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt, vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime)) +
		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,yCpt, vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime)));
		
		break;
	    
	      case (threeDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) -= 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,xCpt, vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,xCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,xCpt,vTime)) +
		   
		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,yCpt, vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,yCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,yCpt,vTime)) +

		   mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*
		   (e0*(*e)(mg,I1b ,I2b ,I3b ,zCpt, vTime) + 
		    e1*(*e)(mg,I1m1,I2m1,I3m1,zCpt,vTime) + 
		    e2*(*e)(mg,I1m2,I2m2,I3m2,zCpt,vTime)));
		
		break;

	      }; //switch (numberOfDimensions)
	      
	      break; //case (macProjectionOfNormalVelocity)
	      
	    }; //switch (projType)
	    	    
	    
	  } // if (twilightZoneFlow)
	  
	  
	  else // ...if not twilightZoneFlow
	  {
	    switch (projType)
	    {
	    case (approximateProjection):
	      switch (numberOfDimensions){

	      case (oneDimension):
		ellipticRightHandSide[grid](I1g,I2g,I3g) =
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		    (e0*(uStar[grid](I1b ,I2b ,I3b ,xCpt) - uGrid[grid](I1b ,I2b ,I3b ,xCpt)) +
		     e1*(uStar[grid](I1m1,I2m1,I3m1,xCpt) - uGrid[grid](I1m1,I2m1,I3m1,xCpt)) +
		     e2*(uStar[grid](I1m2,I2m2,I3m2,xCpt) - uGrid[grid](I1m2,I2m2,I3m2,xCpt)));
		break;
	    
	      case (twoDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,xCpt) - uGrid[grid](I1b ,I2b ,I3b ,xCpt)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,xCpt) - uGrid[grid](I1m1,I2m1,I3m1,xCpt)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,xCpt) - uGrid[grid](I1m2,I2m2,I3m2,xCpt))) +
		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,yCpt) - uGrid[grid](I1b ,I2b ,I3b ,yCpt)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,yCpt) - uGrid[grid](I1m1,I2m1,I3m1,yCpt)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,yCpt) - uGrid[grid](I1m2,I2m2,I3m2,yCpt))));
								    
		break;
	    
	      case (threeDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		(mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,xCpt) - uGrid[grid](I1b ,I2b ,I3b ,xCpt)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,xCpt) - uGrid[grid](I1m1,I2m1,I3m1,xCpt)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,xCpt) - uGrid[grid](I1m2,I2m2,I3m2,xCpt))) +
		 mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,yCpt) - uGrid[grid](I1b ,I2b ,I3b ,yCpt)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,yCpt) - uGrid[grid](I1m1,I2m1,I3m1,yCpt)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,yCpt) - uGrid[grid](I1m2,I2m2,I3m2,yCpt))) +
		 mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*(
		  e0*(uStar[grid](I1b ,I2b ,I3b ,zCpt) - uGrid[grid](I1b ,I2b ,I3b ,zCpt)) +
		  e1*(uStar[grid](I1m1,I2m1,I3m1,zCpt) - uGrid[grid](I1m1,I2m1,I3m1,zCpt)) +
		  e2*(uStar[grid](I1m2,I2m2,I3m2,zCpt) - uGrid[grid](I1m2,I2m2,I3m2,zCpt))));


		break;
	      }; //switch (numberOfDimensions)
	      break; //case(approximateProjection)

	    case (macProjection):
	      switch (numberOfDimensions){

	      case (oneDimension):
		ellipticRightHandSide[grid](I1g,I2g,I3g) =
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		  (uStar[grid] (I1f,I2f,I3f,xCpt,axis) -
		   (e0*uGrid[grid](I1b ,I2b ,I3b ,xCpt) + 
		    e1*uGrid[grid](I1m1,I2m1,I3m1,xCpt) + 
		    e2*uGrid[grid](I1m2,I2m2,I3m2,xCpt)));

		break;
	    
	      case (twoDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*
		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,xCpt,axis) - 
		   (e0*uGrid[grid](I1b ,I2b ,I3b ,xCpt) + 
		    e1*uGrid[grid](I1m1,I2m1,I3m1,xCpt) + 
		    e2*uGrid[grid](I1m2,I2m2,I3m2,xCpt))) +
	
		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		    (uStar[grid] (I1f,I2f,I3f,yCpt) -
		     (e0*uGrid[grid](I1b ,I2b ,I3b ,yCpt) + 
		      e1*uGrid[grid](I1m1,I2m1,I3m1,yCpt) + 
		      e2*uGrid[grid](I1m2,I2m2,I3m2,yCpt))));
		
	    
		break;
	    
	      case (threeDimensions):
		ellipticRightHandSide[grid](I1g,I2g,I3g) = 
		  sideSign/mg.faceArea()(I1f,I2f,I3f,axis)*

		  (mg.faceNormal()(I1f,I2f,I3f,xAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,xCpt,axis) -
		    (e0*uGrid[grid](I1b ,I2b ,I3b ,xCpt) + 
		     e1*uGrid[grid](I1m1,I2m1,I3m1,xCpt) + 
		     e2*uGrid[grid](I1m2,I2m2,I3m2,xCpt))) +

		   mg.faceNormal()(I1f,I2f,I3f,yAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,yCpt,axis) -
		    (e0*uGrid[grid](I1b ,I2b ,I3b ,yCpt) + 
		     e1*uGrid[grid](I1m1,I2m1,I3m1,yCpt) + 
		     e2*uGrid[grid](I1m2,I2m2,I3m2,yCpt))) +

		   mg.faceNormal()(I1f,I2f,I3f,zAxis,axis)*
		   (uStar[grid] (I1f,I2f,I3f,zCpt,axis) -
		    (e0*uGrid[grid](I1b ,I2b ,I3b ,zCpt) + 
		     e1*uGrid[grid](I1m1,I2m1,I3m1,zCpt) + 
		     e2*uGrid[grid](I1m2,I2m2,I3m2,zCpt))));
	    
		break;
	      }; //switch numberOfDimensions
	      
	      break; //case (macProjection)
	      
	    }; //switch (projType)
	    
	  } // if (not twilightZoneFlow)
	  	  
	  break; // case (movingWallNeumann
	  
	  	      
	default:
	    cout << "Projection::applyRightHandSideBoundaryConditions: unknown bc code " << listOfBCs[cmpgrdBC] << endl;
	    assert (FALSE);
	  break;
	};//       End switch
      }//     End if
    }//   End loop over boundaries
  }// End loop over grids

  
}

  
