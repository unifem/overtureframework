// ---------------------------------------------------------------------------------------------------------------------------
// 
// bodyForcingMacros.h: 
//   This file defines macros used for body forces (bodyForcing.bC) and boundary forces (defineVariableBoundaryValues.bC)
//
// ---------------------------------------------------------------------------------------------------------------------------


// =================================================================
// Macro to compute the grid point coordinates.
// =================================================================
#beginMacro getGridCoordinates(xv)
  if( isRectangular )
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
      xv[axis]=XC(iv,axis);
  }
  else
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
      xv[axis]=vertexLocal(i1,i2,i3,axis);
  }
#endMacro

// ============================================================================================
// This macro computes the directions in which the region box is longest.
// ============================================================================================
#beginMacro getWidestBoxDirectionsMacro(dir1,dir2)
 if( numberOfDimensions==2 )
 {
   // dir1 = the longest axes of the box (in 2D) 
   // xb-xa > yb-ya : assume the boundary is horizontal, else vertical
   dir1 = xWidth > yWidth ? 0 : 1;
 }
 else
 {
   // Find the two directions (dir1,dir2) that define the two longest axes of the box
   // 
   if( xWidth < min(yWidth,zWidth) )
   {
     dir1=1; dir2=2;
   }
   else if( yWidth < min(xWidth,zWidth) )
   {
     dir1=0; dir2=2;
   }
   else
   {
     dir1=0; dir2=1;
   }
 }
#endMacro      

// ===================================================================================
// Macro: Add a body force or boundary force (i.e. assign the RHS to a BC):
//
//  This macro will add a body/boundary force over the appropriate region.
//
// Parameters:
//  TYPE : body or boundary to indicate whether this is a body forcing or BC forcing
//  I1,I2,I3 : apply forcing over this indicies.
//
// Implied Parameters:
//   regionType : a string (e.g. "box") denoting the region.
//   bodyForce : a BodyForce object that contains info on the region.
// NOTE: 
// This macro expects the perl variable $statements to hold the statements that assign 
// the body/boundary force at a single point 
// ===================================================================================

#beginMacro addBodyForceMacro(TYPE,I1,I2,I3)

  const int addBodyForce=0, addBoundaryForce=1;
  #If #TYPE eq "body"
   const int forcingType=addBodyForce;
  #Elif #TYPE eq "boundary"
   const int forcingType=addBoundaryForce;
  #Else
    OV_ABORT("addBodyForceMacro: UNKNOWN argument =TYPE");
  #End

  real profileFactor=1.;  // The forcing is multiplied by this factor (changed below for parabolic, ...)

  if( regionType=="box" )
  {
    // -- drag is applied over a box (square in 2D) --

    const real *boxBounds =  bodyForce.dbase.get<real[6] >("boxBounds");

    #define xab(side,axis) boxBounds[(side)+2*(axis)]
    const real & xa = xab(0,0), &xb = xab(1,0);
    const real & ya = xab(0,1), &yb = xab(1,1);
    const real & za = xab(0,2), &zb = xab(1,2);
    
    const aString & profileType = bodyForce.dbase.get<aString>("profileType");

    // if( debug() & 4 )
    // printF("computeBodyForce: profileType=%s, box bounds = [%e,%e]x[%e,%e][%e,%e]\n",(const char*)profileType,xa,xb,ya,yb,za,zb);


    if( profileType=="uniform" )
    {
      // --- uniform profile ---
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	getGridCoordinates(xv);
      
	// Turn on the drag if we are inside the box
	if( xv[0]>=xa && xv[0]<=xb && xv[1]>=ya && xv[1]<=yb && xv[2]>=za && xv[2]<=zb )
	{
          #peval $statements
	}
	  
      } // end FOR_3D
    }
    else if( profileType=="parabolic" )
    {
      // -- Parabolic profile --

      // Near each edge of the region the parabolic profile looks like: 
      //     u(x) = U(x)*( 1 - (1-d(x)/W)^2 ),  for d(x) < W
      //     u(x) = U(x) ,                      for d(x) > W
      // where d(x) is the distance from the point x to the box that defines the region,
      // and W=parabolicProfileDepth is the width of the parabolic profile. 

      const real & parabolicProfileDepth = bodyForce.dbase.get<real>("parabolicProfileDepth");
      const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;

      // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
      int dir1, dir2;  
      getWidestBoxDirectionsMacro(dir1,dir2);
      
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	getGridCoordinates(xv);

        real dist;
        #If #TYPE eq "body"
	  // Body force (volume): compute minimum distance to any side of the box:
  	  dist = min( xv[0]-xab(0,0), xab(1,0)-xv[0], xv[1]-xab(0,1), xab(1,1)-xv[1] );
          if( numberOfDimensions==3 )
  	    dist=min( dist, xv[2]-xab(0,2), xab(1,2)-xv[2] );
        #Elif #TYPE eq "boundary"
          // Boundary: compute the minimum distance to the box edges (ignore box faces in the normal direction to the boundary):
          dist = min( xv[dir1]-xab(0,dir1), xab(1,dir1)-xv[dir1] );
          if( numberOfDimensions==3 )
  	    dist=min( dist, xv[dir2]-xab(0,dir2), xab(1,dir2)-xv[dir2] );
         #Else
           OV_ABORT("addBodyForceMacro: UNKNOWN argument =TYPE");
         #End

	// printF("parabolic: (i1,i2)=(%i,%i) x=(%g,%g) dist=%g \n",i1,i2,xv[0],xv[1],dist);
	
	if( dist>=0. )
	{  
          dist /= parabolicProfileDepth;
          if( dist<1. )
	  {
            // 1 - (1-d)^2 = 2*d-d^2 = d*(2-d)
            profileFactor = dist*(2.-dist);
	  }
	  else
	  {
            profileFactor=1.;
	  }
	  // printF("         : profileFactor=%g \n",profileFactor);
	  
          #peval $statements
	}
	  
      } // end FOR_3D
    }
    else if( profileType=="tanh" )
    {
      // -- Tanh profile:
      // The one-dimensional tanh profile is of the form:
      //     u = U(x) *[  .5*( tanh( b*(x-xa) ) - tanh( b*(x-xb) ) ) ]
      // 

      const real & b  = bodyForce.dbase.get<real>("tanhProfileExponent");
      const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;

      // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
      int dir1, dir2;  
      getWidestBoxDirectionsMacro(dir1,dir2);
      
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	getGridCoordinates(xv);
	
        // --> we could have a cutoff if we are far away from the transition zone, to avoid
        //  evaluating the tanh's.

        #If #TYPE eq "body"
	  // Body force force:
          profileFactor =  .5*( tanh( b*(xv[0]-xab(0,0)) ) - tanh( b*(xv[0]-xab(1,0)) ) );
          profileFactor *= .5*( tanh( b*(xv[1]-xab(0,1)) ) - tanh( b*(xv[1]-xab(1,1)) ) );
	  if( numberOfDimensions==3 )
            profileFactor *= .5*( tanh( b*(xv[2]-xab(0,2)) ) - tanh( b*(xv[2]-xab(1,2)) ) );
        #Elif #TYPE eq "boundary"
          // Boundary force:
          profileFactor = .5*( tanh( b*(xv[dir1]-xab(0,dir1)) ) - tanh( b*(xv[dir1]-xab(1,dir1)) ) );
	  if( numberOfDimensions==3 )
            profileFactor *= .5*( tanh( b*(xv[dir2]-xab(0,dir2)) ) - tanh( b*(xv[dir2]-xab(1,dir2)) ) );
         #Else
           OV_ABORT("addBodyForceMacro: UNKNOWN argument =TYPE");
         #End
	
        #peval $statements

	  
      } // end FOR_3D
    }
    else
    {
      printF("addBodyForceMacro: ERROR: unknown profileType=%s\n",(const char*)profileType);
      OV_ABORT("ERROR");
    }
    
    
  }
  else if( regionType=="ellipse" )
  {
    // -- drag is applied over an ellipse --
    //   [(x-xe)/ae]^2 + [(y-ye)/be]^2 + [(z-ze)/ce]^2 = 1

    const real *ellipse =  bodyForce.dbase.get<real[6] >("ellipse");

    const real ae = ellipse[0];
    const real be = ellipse[1];
    const real ce = ellipse[2];
    const real xe = ellipse[3];
    const real ye = ellipse[4];
    const real ze = ellipse[5];


    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      // Get the grid coordinates xv[axis]:
      getGridCoordinates(xv);
      
      real rad;
      if( numberOfDimensions==2 )
      {
	real xa = (xv[0]-xe)/ae;
	real ya = (xv[1]-ye)/be;
	rad = xa*xa+ya*ya;
      }
      else
      {
	real xa = (xv[0]-xe)/ae;
	real ya = (xv[1]-ye)/be;
	real za = (xv[2]-ze)/ce;
	rad = xa*xa+ya*ya+za*za;
      }
      

      //       // amp = 1 inside the circle and 0 outside
      //       // -- here is a smooth transition from 0 to damp at "radius" rad0
      //       real amp = .5*damp*(tanh( -beta*(rad-rad0) )+1.);
      //       fg(i1,i2,i3,uc) =  -amp*ug(i1,i2,i3,uc);
      //       fg(i1,i2,i3,vc) =  -amp*ug(i1,i2,i3,vc);
       
      // here we turn on the drag as a step function at rad=rad0
      if( rad < 1. )
      {
	#peval $statements
      }
	  
    } // end FOR_3D
  }
  else if( regionType=="maskFromGridFunction" )
  {
    // ---- The region is defined by a grid function that holds a mask ----

    if( !parameters.dbase.has_key("bodyForceMaskGridFunction") )
    {
      printF("ERROR: regionType==`maskFromGridFunction' but the grid function does not exist!\n");
      OV_ABORT("ERROR");
    }
    
    // printF("Setting a body force for regionType==maskFromGridFunction for grid=%i\n",grid);

    realCompositeGridFunction *maskPointer = 
                      parameters.dbase.get<realCompositeGridFunction*>("bodyForceMaskGridFunction");
    assert( maskPointer!=NULL );
    realCompositeGridFunction & bodyForceMask = *maskPointer;

    realArray & bfMask = bodyForceMask[grid];
    OV_GET_SERIAL_ARRAY(real,bfMask,bfMaskLocal);

    getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
    // restrict bounds to local processor, include ghost
    bool ok = ParallelUtility::getLocalArrayBounds(bfMask,bfMaskLocal,I1,I2,I3,1);

    if( ok )
    {

      profileFactor=1.;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( bfMaskLocal(i1,i2,i3)<=0. )  // signed distance 
	{
          #peval $statements
	}
	
      }
    }
    
    
  }
  else if( regionType=="mapping" )
  {
    // --- region is defined by a Mapping ---
    //   2D : closed curve
    //   3D : water-tight surface

    if( !bodyForce.dbase.has_key("bodyForceMapping") )
    {
      printF("computeBodyForcing:WARNING: there is no body force Mapping!\n");
      continue;
    }
    MappingRC *& pBodyForceMapping = bodyForce.dbase.get<MappingRC*>("bodyForceMapping");
    if( pBodyForceMapping==NULL )
    {
      printF("computeBodyForcing:WARNING: the body force Mapping is NULL!\n");
      continue;
    }
    Mapping & bodyForceMapping = pBodyForceMapping->getMapping();
      
    if( numberOfDimensions==2 )
    {
      IntegerArray cross(1);
      RealArray xa(1,3);
      xa=0.;
      assert( bodyForceMapping.approximateGlobalInverse !=NULL );
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	getGridCoordinates(xv);
      
	// Turn on the drag if we are inside the body

        // ----- OPTIMZE ME -- could save a mask ---
	cross=0;
        xa(0,0)=xv[0]; xa(0,1)=xv[1];
	bodyForceMapping.approximateGlobalInverse->countCrossingsWithPolygon( xa,cross );
        int inside = (cross(0) % 2 == 0) ? 0 : +1;
	// printF("computeBodyForcing: point (%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),inside);
	if( inside )
	{
          #peval $statements
	}
	  
      } // end FOR_3D
    }
    else
    {
      IntegerArray inside(1); 
      RealArray xa(1,3);
      xa=0.;
      assert( bodyForceMapping.getClassName()=="UnstructuredMapping" );
      UnstructuredMapping & uMap = (UnstructuredMapping&)bodyForceMapping;

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// Get the grid coordinates xv[axis]:
	getGridCoordinates(xv);
      
	// Turn on the drag if we are inside the body

        // ----- OPTIMZE ME -- could do many pts at once, save a mask ---
        xa(0,0)=xv[0]; xa(0,1)=xv[1]; xa(0,2)=xv[2];
        #ifndef USE_PPP
  	  uMap.insideOrOutside(xa,inside);
        #else
          OV_ABORT("finish me for parallel");
        #endif

	// printF("computeBodyForcing: point (%8.2e,%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),xa(0,2),inside(0));
	if( inside(0) )
	{
          #peval $statements
	}
	  
      } // end FOR_3D

    }
    
  }
  else
  {
    printF("computeBodyForcing:ERROR: unexpected regionType=%s\n",(const char*)regionType);
    OV_ABORT("ERROR: finish me...");
  }

#endMacro      

// =====================================================================================
// Save info about the body force region and profile in the bodyForce object.
// =====================================================================================
#beginMacro saveBodyForceRegionInfoMacro(bodyForce,regionPar)
{
  // Here is the region type ("box", "ellipse", ... ) chosen by the user
  const aString & regionType = regionPar.dbase.get<aString>("regionType");

  // Save the region type:
  if( !bodyForce.dbase.has_key("regionType") )
    bodyForce.dbase.put<aString>("regionType");  // region type
  bodyForce.dbase.get<aString>("regionType")=regionType;

  if( !bodyForce.dbase.has_key("linesToPlot") )
    bodyForce.dbase.put<int[3]>("linesToPlot"); 
  int *linesToPlot = bodyForce.dbase.get<int[3]>("linesToPlot"); 
  int *lines =  regionPar.dbase.get<int[3] >("linesToPlot");
  for( int i=0; i<3; i++ )
    linesToPlot[i]=lines[i];

  if( regionType=="box" )
  {
    bodyForce.dbase.put<real[6] >("boxBounds");
    real *boxBounds =  bodyForce.dbase.get<real[6]>("boxBounds");
    const real *bpar = regionPar.dbase.get<real[6]>("boxBounds");
    for( int i=0; i<6; i++ )
      boxBounds[i]=bpar[i];
  }
  else if( regionType=="ellipse" )
  {
    bodyForce.dbase.put<real[6] >("ellipse");
    real *ellipse    =  bodyForce.dbase.get<real[6]>("ellipse");
    const real *epar =  regionPar.dbase.get<real[6]>("ellipse");
    for( int i=0; i<6; i++ )
      ellipse[i]=epar[i];
  }
  else if( regionType=="maskFromGridFunction" )
  {
    // region defined from a mask in a grid function
  }
  else if( regionType=="mapping" )
  {
    // region defined by a closed curve in 2D or a 3D surface
    // -- Make a copy of the Mapping that was temporarily saved in the regionPar --
    assert( regionPar.dbase.has_key("bodyForceMapping") );
    MappingRC *& regionParMapping = regionPar.dbase.get<MappingRC*>("bodyForceMapping"); 
    assert( regionParMapping!=NULL );
    printF("saveBodyForce: regionParMapping referenceCount=%i BEFORE\n",regionParMapping->getMapping().getReferenceCount());
    
    if( !bodyForce.dbase.has_key("bodyForceMapping") )
    {
      bodyForce.dbase.put<MappingRC*>("bodyForceMapping");
      bodyForce.dbase.get<MappingRC*>("bodyForceMapping")=NULL;
    }
    MappingRC *& bodyForceMapping = bodyForce.dbase.get<MappingRC*>("bodyForceMapping");
    if( bodyForceMapping==NULL )
      bodyForceMapping = new MappingRC();
    bodyForceMapping->reference(*regionParMapping);

    printF("saveBodyForce: regionParMapping referenceCount=%i\n",regionParMapping->getMapping().getReferenceCount());
    printF("saveBodyForce: bodyForceMapping referenceCount=%i\n",bodyForceMapping->getMapping().getReferenceCount());
    
  }
  else
  {
    printF("defineVariableBoundaryValues:ERROR: unexpected regionType=%s\n",(const char*)regionType);
    OV_ABORT("ERROR: finish me...");
  }

  // Save the profile type
  const aString & profileType = regionPar.dbase.get<aString>("profileType");
  bodyForce.dbase.put<aString>("profileType");  
  bodyForce.dbase.get<aString>("profileType")=profileType;
  if( profileType=="parabolic" )
  {
    bodyForce.dbase.put<real>("parabolicProfileDepth");
    bodyForce.dbase.get<real>("parabolicProfileDepth")=regionPar.dbase.get<real>("parabolicProfileDepth"); 
  }
  else if( profileType=="tanh" )
  {
    bodyForce.dbase.put<real>("tanhProfileExponent");
    bodyForce.dbase.get<real>("tanhProfileExponent")=regionPar.dbase.get<real>("tanhProfileExponent"); 
  }
}
#endMacro
