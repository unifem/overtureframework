//------------------------------------------------------------------------------------
// This file contains macros used by the interface routines.
//------------------------------------------------------------------------------------



// ===========================================================================
// Get/set the interface RHS for a heat flux interface
// ===========================================================================
#beginMacro heatFluxInterfaceRightHandSide(LABEL)

real *a = info.a;

if( debug() & 4 )
{
  printP("LABEL::interfaceRHS:heatFlux %s RHS for (side,axis,grid)=(%i,%i,%i) a=[%5.2f,%5.2f]"
	 " t=%9.3e gfIndex=%i (current=%i)\n",
	 (option==0 ? "get" : "set"),side,axis,grid,a[0],a[1],t,gfIndex,current);
}

const int tc = parameters.dbase.get<int >("tc");   
assert( tc>=0 );
Range N(tc,tc);

// We could optimize this for rectangular grids 
mg.update(MappedGrid::THEvertexBoundaryNormal);
#ifdef USE_PPP
const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
#else
const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
#endif


if( option==setInterfaceRightHandSide )
{
  // **** set the RHS *****
  //   (TZ is done below)

  bd(I1,I2,I3,tc)=f(I1,I2,I3,tc);
  if( false )
  {
    ::display(bd(I1,I2,I3,tc)," RHS values","%4.2f ");
  }
      
}
else if( option==getInterfaceRightHandSide )
{

  // **** get the RHS ****

  realMappedGridFunction & u = gf[gfIndex].u[grid];
#ifdef USE_PPP
  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
#else
  realSerialArray & uLocal = gf[gfIndex].u[grid];
#endif


  f(I1,I2,I3,tc) = a[0]*uLocal(I1,I2,I3,tc);

  if( a[1]!=0. )
  {
    // add on a[1]*( nu*u.n ) on the boundary 

    // **be careful** -- the normal changes sign on the two sides of the interface ---
    MappedGridOperators & op = *(u.getOperators());

    realSerialArray ux(I1,I2,I3,N), uy(I1,I2,I3,N);

    op.derivative(MappedGridOperators::xDerivative,uLocal,ux,I1,I2,I3,N);
    op.derivative(MappedGridOperators::yDerivative,uLocal,uy,I1,I2,I3,N);

    if( cg.numberOfDimensions()==2 )
    {
      f(I1,I2,I3,tc) += a[1]*( normal(I1,I2,I3,0)*ux + normal(I1,I2,I3,1)*uy );
    }
    else
    {
      realSerialArray uz(I1,I2,I3);
      op.derivative(MappedGridOperators::zDerivative,uLocal,uz,I1,I2,I3,N);
      f(I1,I2,I3,tc) += a[1]*( normal(I1,I2,I3,0)*ux + normal(I1,I2,I3,1)*uy + normal(I1,I2,I3,2)*uz );
    }

  }

  if( debug() & 4 )
  {
    ::display(f(I1,I2,I3,tc) ,sPrintF("getRHS:  %f*u + %f*( u.n ) ",a[0],a[1]));
  }
      
}
else
{
  printF("LABEL::interfaceRightHandSide:ERROR: unknown option=%i\n",option);
  Overture::abort("error");
}

if( // false &&  // turn this off for testing the case where the same TZ holds across all domains
  parameters.dbase.get<bool >("twilightZoneFlow") )
{
  // ---add forcing for twlight-zone flow---

  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

  const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

  if( !isRectangular )
    mg.update(MappedGrid::THEcenter);

  realArray & x= mg.center();
#ifdef USE_PPP
  realSerialArray xLocal; 
  if( !isRectangular ) 
    getLocalArrayWithGhostBoundaries(x,xLocal);
#else
  const realSerialArray & xLocal = x;
#endif

  realSerialArray ue(I1,I2,I3,N);
  if( a[0]!=0. )
  {
    e.gd( ue ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,N,t);  // exact solution 

    ue(I1,I2,I3,N) = a[0]*ue(I1,I2,I3,N);
  }
  else
  {
    ue(I1,I2,I3,N) =0.;
  }
    
  if( a[1]!=0. )
  {
    realSerialArray uex(I1,I2,I3,N), uey(I1,I2,I3,N);

    e.gd( uex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,N,t);
    e.gd( uey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,N,t);
    if( numberOfDimensions==2 )
    {
      ue(I1,I2,I3,N) += a[1]*( normal(I1,I2,I3,0)*uex + normal(I1,I2,I3,1)*uey );
    }
    else
    {
      realSerialArray uez(I1,I2,I3,N);
      e.gd( uez ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,N,t);

      ue(I1,I2,I3,N) += a[1]*( normal(I1,I2,I3,0)*uex + normal(I1,I2,I3,1)*uey + normal(I1,I2,I3,2)*uez ); 
    }
  }
    
  if( option==getInterfaceRightHandSide )
  { // get 
    //   subtract off TZ flow:
    //   f <- f - ( a[0]*ue + a[1]*( nu*ue.n ) )
    if( false )
    {
      ::display(f(I1,I2,I3,tc) ," a[0]*u + a[1]*( k u.n )");
      ::display(ue(I1,I2,I3,tc)," a[0]*ue + a[1]*( k ue.n )");
    }
    f(I1,I2,I3,tc) -= ue(I1,I2,I3,N);
    if( false )
    {
      ::display(f(I1,I2,I3,tc) ," a[0]*u + a[1]*( k u.n ) - [a[0]*ue + a[1]*( k ue.n )]");
    }
  }
  else if( option==setInterfaceRightHandSide )
  { // set 
    //   add on TZ flow:
    //   bd <- bd + a[0]*ue + a[1]*( nu*ue.n )
    bd(I1,I2,I3,tc) += ue(I1,I2,I3,N);

    if( false )
    {
      bd(I1,I2,I3,tc) = ue(I1,I2,I3,N);
    }
	

  }
  else
  {
    Overture::abort("error");
  }
    
} // end if TZ 
#endMacro
