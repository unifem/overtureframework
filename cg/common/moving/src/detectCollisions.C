#include "DetectCollisions.h"
#include "StretchTransform.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "SquareMapping.h"
#include "BoxMapping.h"

// ====================================================================================
///  \function detectCollisions
///  \brief Detect collisions.
///  \details Currently This routine only works in some special cases: 2D cylinders and spheres.
///
/// \param t (input) : current time.
/// \param gc (input) : grid collection
/// \param numberOfRigidBodies (input) : number of rigid bodies to check for collisions.
/// \param body (input) : array of rigid bodies 
/// \param bodyDefinition (input) : defines the bodies.
/// \param minimumSeparation (input) : minimum allowable separation (in grid lines). 
// ===================================================================================
int
detectCollisions( real t, 
		  GridCollection & gc, 
		  int numberOfRigidBodies, 
		  RigidBodyMotion **body,
                  const BodyDefinition & bodyDefinition,
                  const real minimumSeparation /* =2.5 */ )
{

  

  if( numberOfRigidBodies<=0 )
    return 0;
  
  int debug=0; // 1; 
  if( debug & 1 )
     printF("detectCollisions: t=%8.3e, numberOfRigidBodies=%i\n",t,numberOfRigidBodies);
  
  int returnValue=0;
  const int numberOfDimensions=gc.numberOfDimensions();
  Range Rx=numberOfDimensions;
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
  // ----- rigid body info ------
  // xCM(.,b) : center of mass of body b
  // vCM(.,b) : velocity of the center of mass of body b

  // radius(b) : radius of the body (assumed circular)
  // spacing(b) : distance between the boundary and grid line number "separationLine" 

  Index Ib1,Ib2,Ib3,Ip1,Ip2,Ip3;
  RealArray xCM(3,numberOfRigidBodies); xCM=0.;
  RealArray vCM(3,numberOfRigidBodies); vCM=0.;
  RealArray x0(3), radius(numberOfRigidBodies), spacing(numberOfRigidBodies);

  // separationLine : closest line less than or equal to a distance of "minimumSeparation"
  const int separationLine = int(minimumSeparation+REAL_EPSILON*10.);
  // const real deltaSeparation=minimumSeparation-separationLine;

  // *********************************************************
  // **** Compute radius and spacing for each rigid body *****
  // *********************************************************
  Range all;
  for( int b=0; b<numberOfRigidBodies; b++ )
  {
    const RealArray & xCMb0 = xCM(all,b);  RealArray & xCMb = (RealArray&) xCMb0;
    const RealArray & vCMb0 = vCM(all,b);  RealArray & vCMb = (RealArray&) vCMb0;

    body[b]->getPosition(t,xCMb);
    body[b]->getVelocity(t,vCMb);

    // x0 : holds xCM for this body 
    x0=xCM(all,b);
    
    // for each body, get the grid(s) that form the body
    const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(b);
    for( int face=0; face<numberOfFaces; face++ )
    {
      // **NOTE: we break after finding one face **** could do better

      int side=-1,axis,grid;
      bodyDefinition.getFace(b,face,side,axis,grid);
      assert( side>=0 && side<=1 && axis>=0 && axis<gc.numberOfDimensions());
      assert( grid>=0 && grid<gc.numberOfComponentGrids());

      // printF("\nMovingGrids::detectCollisions: body %i : face %i: (side,axis,grid)=(%i,%i,%i)\n",
      //   b,face,side,axis,grid);

      MappedGrid & c = gc[grid];
      
      getBoundaryIndex(c.indexRange(),side,axis,Ib1,Ib2,Ib3);
      const int num=Ib1.getLength()*Ib2.getLength()*Ib3.getLength();

      // ** compute the spacing to a line offset from the surface ***
      getGhostIndex(c.indexRange(),side,axis,Ip1,Ip2,Ip3,-separationLine);

      // const realArray & x = c.vertex();
      bool ok=true;
      #ifdef USE_PPP
  	realSerialArray x; getLocalArrayWithGhostBoundaries(c.vertex(),x);

        int includeGhost=0;
        ok = ParallelUtility::getLocalArrayBounds(c.vertex(),x,Ib1,Ib2,Ib3,includeGhost);
        bool okp = ParallelUtility::getLocalArrayBounds(c.vertex(),x,Ip1,Ip2,Ip3,includeGhost);
        assert ( ok==okp );
    
      #else
        const realSerialArray & x = c.vertex();
      #endif 
     
      // ***** No need to recompute the radius of the bodies every time step !! ****
      if( true )
      {
	// new way for parallel  *wdh* 100227
        radius(b)=0.;
        spacing(b)=0.;
	if( ok )
	{
	  if( numberOfDimensions==2 )
	    radius(b) = (sum(SQR(x(Ib1,Ib2,Ib3,0)-x0(0))+
			     SQR(x(Ib1,Ib2,Ib3,1)-x0(1))) );
	  else
	    radius(b) = (sum(SQR(x(Ib1,Ib2,Ib3,0)-x0(0))+
			     SQR(x(Ib1,Ib2,Ib3,1)-x0(1))+
			     SQR(x(Ib1,Ib2,Ib3,2)-x0(2))) );
	  if( numberOfDimensions==2 )   
	    spacing(b)=(sum(SQR(x(Ip1,Ip2,Ip3,0)-x(Ib1,Ib2,Ib3,0))+
			    SQR(x(Ip1,Ip2,Ip3,1)-x(Ib1,Ib2,Ib3,1))) );
	  else
	    spacing(b)=(sum(SQR(x(Ip1,Ip2,Ip3,0)-x(Ib1,Ib2,Ib3,0))+
			    SQR(x(Ip1,Ip2,Ip3,1)-x(Ib1,Ib2,Ib3,1))+
			    SQR(x(Ip1,Ip2,Ip3,2)-x(Ib1,Ib2,Ib3,2))) );
	}
	
	radius(b) = ParallelUtility::getSum(radius(b));
	radius(b) = sqrt( radius(b)/num );

	// ** compute the spacing to a line offset from the surface ***
	spacing(b) = ParallelUtility::getSum(spacing(b));
	spacing(b) = sqrt( spacing(b)/num );

      }
      else
      {
	// old way
	if( numberOfDimensions==2 )
	  radius(b) = SQRT(sum(SQR(x(Ib1,Ib2,Ib3,0)-x0(0))+SQR(x(Ib1,Ib2,Ib3,1)-x0(1)))/num);
	else
	  radius(b) = SQRT(sum(SQR(x(Ib1,Ib2,Ib3,0)-x0(0))+
			       SQR(x(Ib1,Ib2,Ib3,1)-x0(1))+
			       SQR(x(Ib1,Ib2,Ib3,2)-x0(2)))/num);


	// ** compute the spacing to a line offset from the surface ***
	getGhostIndex(c.indexRange(),side,axis,Ip1,Ip2,Ip3,-separationLine);
	if( numberOfDimensions==2 )   
	  spacing(b)=SQRT(sum(SQR(x(Ip1,Ip2,Ip3,0)-x(Ib1,Ib2,Ib3,0))+
			      SQR(x(Ip1,Ip2,Ip3,1)-x(Ib1,Ib2,Ib3,1)))/num);
	else
	  spacing(b)=SQRT(sum(SQR(x(Ip1,Ip2,Ip3,0)-x(Ib1,Ib2,Ib3,0))+
			      SQR(x(Ip1,Ip2,Ip3,1)-x(Ib1,Ib2,Ib3,1))+
			      SQR(x(Ip1,Ip2,Ip3,2)-x(Ib1,Ib2,Ib3,2)))/num);
	
      }
      
      break;
    }
    
    // printF(" body %i : radius=%e, spacing to line %i = %e. (\n",b,radius(b),separationLine,spacing(b));

  }
  
  // ***********************************************************************
  // **** Check for collisions with domain boundaries and other bodies *****
  // ***********************************************************************
  for( int b=0; b<numberOfRigidBodies; b++ ) 
  {

    // *********************************************************************
    // *** check for a collision with any rectangular "background" grids ***
    // *********************************************************************

    for( int grid=0; grid<gc.numberOfBaseGrids(); grid++ )
    {
      MappedGrid & mg = gc[grid];

      // dx[side][axis] : set equal to the distance to the "separationLine" from the boundary
      real dx[2][3],xab[2][3];

      bool checkThisGrid=false;
      
      if( mg.isRectangular() )
      {
        checkThisGrid=true;
        real dxr[3];
	mg.getRectangularGridParameters( dxr, xab );
        // dx[side][axis] should be the distance to the closest line
        for( int side=0; side<=1; side++ )
	  for(int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    dx[side][axis]=dxr[axis]*separationLine;
	
      }
      else if( mg.mapping().getClassName()=="StretchTransform" )
      {
        // printF(">>detect collisions: grid %i is a StretchTransform\n",grid);

	StretchTransform & stretch = (StretchTransform&)(mg.mapping().getMapping());
	if( stretch.map2.getClassName()=="SquareMapping" )
	{
	  // ------ this is a stretched square mapping ---------

          // printF(">>>detect collisions: grid %i is a StretchTransform of a SquareMapping \n",grid);

          checkThisGrid=true;
	  
	  if( true )
	  {
	    // *new* way for parallel  *wdh* 2015/05/25 
            SquareMapping & squareMap = (SquareMapping&)(stretch.map2.getMapping());
	  
	    squareMap.getVertices( xab[0][0],xab[1][0], xab[0][1],xab[1][1] );
	    xab[0][2]=0.; xab[1][2]=1.;
            RealArray r(1,3),x(1,3);
	    for(int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    {
	      // -- evaluate the stretched square on the "separationLine" --
	      const real drc = mg.gridSpacing(axis)*separationLine;
	      r=.5;
	      for( int side=0; side<=1; side++ )
	      {
		r(0,axis) = side==0 ? drc : 1.-drc;
		stretch.mapS(r,x);
		dx[side][axis]= fabs(x(0,axis)-xab[side][axis]);
	      }
	    
	    }
	  }
	  else
	  {
	    // --old way --
	    OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);
	  
	    for( int side=0; side<=1; side++ )
	    {
	      for(int axis=0; axis<mg.numberOfDimensions(); axis++ )
	      {
		// here we assume that the grid is rectangular (but not Cartesian)
		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

		// -- point on the boundary 
		xab[side][axis]=xLocal(Ib1.getBase(),Ib2.getBase(),Ib3.getBase(),axis);
	      
		// -- distance to grid line "separationLine" inside the domain:
		getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-separationLine);
		dx[side][axis]=fabs(xLocal(Ip1.getBase(),Ip2.getBase(),Ip3.getBase(),axis)-
				    xLocal(Ib1.getBase(),Ib2.getBase(),Ib3.getBase(),axis));
		// printF(" grid=%i (side,axis)=(%i,%i) xab=%8.2e, dx=%8.2e\n",grid,side,axis,
		//       xab[side][axis],dx[side][axis]);
	      
	      }
	    }
	  } // end -old way-
	  

	} // end if stretch.map2 == square 
	else if( stretch.map2.getClassName()=="BoxMapping" )
	{
	  // ------ this is a stretched BoxMapping ---------

          // printF(">>>detect collisions: grid %i is a StretchTransform of a BoxMapping \n",grid);

          checkThisGrid=true;
	  // *wdh* 2015/05/25 
	  BoxMapping & boxMap = (BoxMapping&)(stretch.map2.getMapping());
	  
	  boxMap.getVertices( xab[0][0],xab[1][0], xab[0][1],xab[1][1], xab[0][2],xab[1][2] );
	  RealArray r(1,3),x(1,3);
	  for(int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    // -- evaluate the stretched box on the "separationLine" --
	    const real drc = mg.gridSpacing(axis)*separationLine;
	    r=.5;
	    for( int side=0; side<=1; side++ )
	    {
	      r(0,axis) = side==0 ? drc : 1.-drc;
	      stretch.mapS(r,x);
	      dx[side][axis]= fabs(x(0,axis)-xab[side][axis]);
	    }
	  }
	} // end if stretch.map2 == BoxMapping
	
      }
      
      if( checkThisGrid )
      {
	// IntegerArray bcLocal; 
	// ParallelGridUtility::getLocalBoundaryConditions(mg,bcLocal);

        int collisionDetected[2][3] ={0,0, 0,0, 0,0};// 
	for( int side=0; side<=1; side++ )
	{
	  for(int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    if( mg.boundaryCondition(side,axis)>0 )
	    {
              // This is a wall (could be inflow or outflow -- but ignore this possibility)

              real separationDistance = fabs(xCM(axis,b)-xab[side][axis])-radius(b);
              real minDist=max(dx[side][axis],spacing(b))/separationLine*(minimumSeparation);

              if( debug & 2 )
		printF(" check for collision with walls: grid=%i, (side,axis)=(%i,%i) sepDist=%8.2e, minDist=%8.2e\n",
		       grid,side,axis,separationDistance,minDist);
	      
              if( separationDistance< minDist )
	      {
                collisionDetected[side][axis]=true;
	      }
	    }
	  }
	}
        // All processors should agree there is a collision: 
	int collisionDetectedAnywhere[2][3];
	ParallelUtility::getMaxValues( &(collisionDetected[0][0]), &(collisionDetectedAnywhere[0][0]),6 );

	for( int side=0; side<=1; side++ )
	{
	  for(int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    if( collisionDetectedAnywhere[side][axis]!=collisionDetected[side][axis] )
	    {
	      printf("detectCollisions: ERROR: myid=%i : collision detection NOT consistent amongs processors!\n",
		     myid);
	      OV_ABORT("ERROR");
	    }
	    if( collisionDetectedAnywhere[side][axis] )
	    {
	      
              real separationDistance = fabs(xCM(axis,b)-xab[side][axis])-radius(b);
              real minDist=max(dx[side][axis],spacing(b))/separationLine*(minimumSeparation);	
		
	      printF("\n ....................possible collision with a wall.................................\n");

	      // For the collision -- build a virtual "image" body on the opposite side of the wall that
	      // is moving toward the actual body. For now the image body generates an elastic collision.
	      RealArray xCM2(3),vCM2(3);  // here is the image body
	      x0=0.; xCM2=0.; vCM2=0.;

	      xCM2(Rx)=xCM(Rx,b);
	      xCM2(axis)=2.*xab[side][axis] - xCM(axis,b);
	      vCM2(Rx)=vCM(Rx,b);
	      vCM2(axis)=-vCM(axis,b);
		
	      // x0 : vector from xCM(b) to xCM2
		
	      x0(Rx)=xCM2(Rx)-xCM(Rx,b);   
	      real dist = SQRT(sum(SQR(x0(Rx))));
	      if( dist!=0. )
		x0/=dist;

	      real m1 = body[b]->getMass();
	      real m2 = m1;
		
	      // compute u1 = relative velocity of body b  along the line x0
	      // compute u2 = relative velocity of body b2 along the line x0
	      real u1=vCM(0,b )*x0(0)+vCM(1,b )*x0(1)+vCM(2,b )*x0(2);
	      real u2=vCM2(0  )*x0(0)+vCM2(1  )*x0(1)+vCM2(2  )*x0(2);  // should equal -u1

              // All processors should again agree on the collision
              int haveCollided = u1>u2; // only transfer momentum if body b is moving towards the wall
	      int someHaveCollided=ParallelUtility::getMaxValue(haveCollided);
	      if( someHaveCollided !=haveCollided )
	      {
                printf("detectCollisions: ERROR: myid=%i :haveCollided != someHaveCollided!\n",
                   myid);
		OV_ABORT("ERROR");
	      }

	      if( someHaveCollided ) 
	      {
		real uCM=(m1*u1+m2*u2)/(m1+m2);  // should equal zero
		// u1p=-u1+2.*uCM;
		// u2p=-u2+2.*uCM;
		RealArray v1(3); v1=0.;
		v1(Rx)=vCM(Rx,b )+(-2.*u1+2.*uCM)*x0(Rx);
		body[b]->momentumTransfer( t,v1 );
		
		printF("\n ********************collision with a wall***************************\n"
		       " Collision of body %i with a wall: grid %s (%i) (side,axis)=(%i,%i) wall: x[%i]=%9.3e\n"
		       "    body %i :  xCM=(%8.2e,%8.2e,%8.2e) vCM=(%8.2e,%8.2e,%8.2e) \n"
		       "    separation-distance = %9.3e, (collison distance=%g lines)  \n"
		       "...collision: u1=%9.3e, u2=%9.3e, uCM=%9.3e, v1=(%8.2e,%8.2e,%8.2e) (new velocity)\n"
		       " ********************end collision with a wall***************************\n",
		       b,(const char*)mg.getName(),grid,side,axis,axis,xab[side][axis], 
                       b,xCM(0,b),xCM(1,b),xCM(2,b),vCM(0,b),vCM(1,b),vCM(2,b),
		       separationDistance, minimumSeparation, u1,u2,uCM,v1(0),v1(1),v1(2));
		  
	      } // end someHaveCollided

	    } // end if collisonDetectedAnywhere
	  } // end for axis
	} // end for side
      }
    } // end for grid
    

    // ----------------------------------------------------------------------------------------------
    // ---- For each rigid body: check for collisions with other (higher-numbered) rigid bodies -----
    // ----------------------------------------------------------------------------------------------

    x0=0.;
    for( int b2=b+1; b2<numberOfRigidBodies; b2++ )
    {
      
      // x0 : vector from xCM(b) to xCM(b2)
      x0(Rx)=xCM(Rx,b2)-xCM(Rx,b);
      real dist = SQRT(sum(SQR(x0(Rx))));

      // only allow the bodies to approach by a distance d
      // -> we want to be able to interpolate a point on the second ghost line
      real d = radius(b)+radius(b2)+max(spacing(b),spacing(b2))*(minimumSeparation/separationLine);

      real separationDistance=dist-radius(b)-radius(b2);
      
      if( debug & 1 || dist<d )
	printF("---detectCollisions: dist between body %i (r=%8.2e,dr=%8.2e) and body %i (r=%8.2e,dr=%8.2e) is %8.2e \n"
	       "                     separation dist=%8.2e approx grid lines=%5.2f, %5.2f \n",
	       b,radius(b),spacing(b)/separationLine, b2,radius(b2),spacing(b2)/separationLine,
	       dist,separationDistance,separationDistance/(spacing(b)/separationLine),
	       separationDistance/(spacing(b2)/separationLine));

      if( dist!=0. )
        x0/=dist;
      
      // x0 = unit vector pointing from the xCM(b) to xCM(b2)
      // compute u1 = relative velocity of body b  along the line x0
      // compute u2 = relative velocity of body b2 along the line x0
      real u1=vCM(0,b )*x0(0)+vCM(1,b )*x0(1)+vCM(2,b )*x0(2);
      real u2=vCM(0,b2)*x0(0)+vCM(1,b2)*x0(1)+vCM(2,b2)*x0(2);
      if( debug & 2 ) printF("----- u1=%8.2e, u2=%8.2e, rel. velocity=%8.2e\n",u1,u2,u1-u2);


      // All processors should agree on the collision
      int haveCollided = dist<d && u1>u2; // only transfer momentum if body b is moving towards the wall
      int someHaveCollided=ParallelUtility::getMaxValue(haveCollided);
      if( someHaveCollided !=haveCollided )
      {
	printf("detectCollisions: ERROR: myid=%i :haveCollided != someHaveCollided for two bodies!\n",
	       myid);
	OV_ABORT("ERROR");
      }

      if( someHaveCollided )
      {
	printF(" =================================================================================\n"
               "  detectCollisions: collision detected at t=%9.3e : dist=%e < d=%e***\n"
               "    Body b=%i xCM=(%8.2e,%8.2e,%8.2e) vCM=(%8.2e,%8.2e,%8.2e) \n"
               "    Body b=%i xCM=(%8.2e,%8.2e,%8.2e) vCM=(%8.2e,%8.2e,%8.2e) \n"
               " =================================================================================\n",
               t,dist,d,
               b,xCM(0,b),xCM(1,b),xCM(2,b),vCM(0,b),vCM(1,b),vCM(2,b),
               b2,xCM(0,b2),xCM(1,b2),xCM(2,b2),vCM(0,b2),vCM(1,b2),vCM(2,b2)
                   );
	returnValue=1;
        real m1 = body[b ]->getMass();
        real m2 = body[b2]->getMass();
	
        real uCM=(m1*u1+m2*u2)/(m1+m2);
	// u1p=-u1+2.*uCM;
	// u2p=-u2+2.*uCM;
        RealArray v1(3),v2(3); v1=0.; v2=0.;

        v1(Rx)=vCM(Rx,b )+(-2.*u1+2.*uCM)*x0(Rx);
        v2(Rx)=vCM(Rx,b2)+(-2.*u2+2.*uCM)*x0(Rx);
	
        body[b ]->momentumTransfer( t,v1 );
        body[b2]->momentumTransfer( t,v2 );
	
      }
      
    }
  }
  return returnValue;
}

