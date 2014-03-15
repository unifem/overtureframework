#include "Cgcns.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "App.h"
#include "GenericGraphicsInterface.h"
#include "CnsParameters.h"


// ===================================================================================================================
/// \brief Return a grid function for plotting and for output that may contain extra variables, such
/// as errors or such as the pressure for the compressible NS.
///
/// \param gf0 (input) : input grid function.
/// \param v (input) : grid function to hold the result, IF extra variables area added.
/// \return a realCompositeGridFunction holding the possibly augmented solution.
/// 
// *wdh* 100610 -- cleaned up the treatment of plotting extra variables ---
// ==================================================================================================================
realCompositeGridFunction & Cgcns::
getAugmentedSolution( GridFunction & gf0, realCompositeGridFunction & v )
{
  checkArrayIDs(sPrintF("getAugmentedSolution: start") ); 

  // No need to compute the augmented solution if graphics plotting is off
  if( parameters.dbase.get<GenericGraphicsInterface* >("ps")!=NULL && 
      !parameters.dbase.get<GenericGraphicsInterface* >("ps")->graphicsIsOn() ) return gf0.u;

  CompositeGrid & cg = gf0.cg;
  realCompositeGridFunction & u = gf0.u;

  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const CnsParameters::PDE & pde = parameters.dbase.get<CnsParameters::PDE >("pde");
  const Parameters::TimeSteppingMethod & timeSteppingMethod = 
                         parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");

  const bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
  

  // --- first make a count of all the variables we will save ---

  // numberOfAugmentedComponents = the number of components in the augmented grid function v
  int numberOfAugmentedComponents=0;

  numberOfAugmentedComponents += numberOfComponents;  // we always plot the components of u 

  if( twilightZoneFlow )
  {
    numberOfAugmentedComponents += numberOfComponents;   // plot errors for twilight zone
  }
  else
  {
    // real run: 
  
    if( pde==CnsParameters::compressibleNavierStokes )
    {
      numberOfAugmentedComponents +=1; // plot the pressure too
    }
    else if( pde==CnsParameters::compressibleMultiphase )
    {
      numberOfAugmentedComponents += 2; // plot two pressures
    }
    else
    {
      OV_ABORT("error");
    }

    if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")!=CnsParameters::noKnownSolution )
    {
      // For a known (non-TZ) solution we plot errors and true solution
      numberOfAugmentedComponents += numberOfComponents*2;
    }
  }

  int nAmrErr=-1;
  if( parameters.isAdaptiveGridProblem() && parameters.dbase.get<int >("showAmrErrorFunction") )
  {
    // show the AMR error function
    nAmrErr=numberOfAugmentedComponents;  // here is where we should store the amr error function 
    numberOfAugmentedComponents += 1;     
  }
  
  int nResid=-1;
  if( parameters.dbase.get<int >("showResiduals") || 
      timeSteppingMethod==Parameters::steadyStateRungeKutta )
  {
    // plot residuals 
    nResid = numberOfAugmentedComponents;  // here is where we should store the residual
    numberOfAugmentedComponents += numberOfComponents; 
  }
  
  int nGridVelocity=-1;
  if( parameters.isMovingGridProblem() && parameters.dbase.get<int >("plotGridVelocity") )
  {
    // plot the grid velocity
    nGridVelocity=numberOfAugmentedComponents; // here is where we should store the grid velocity
    numberOfAugmentedComponents += cg.numberOfDimensions();  
  }
  
  
  // --- return here with the current solution if there are no extra components to plot
  if( numberOfAugmentedComponents==numberOfComponents )
    return gf[current].u;

  // Now we can now allocate space for all the components that we will plot
  Range all;
  v.updateToMatchGrid(cg,all,all,all,numberOfAugmentedComponents);

  Index I1,I2,I3;
  Range N(0,numberOfComponents-1);
  // --- first copy the solution ---
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.dimension(),I1,I2,I3);
    OV_GET_SERIAL_ARRAY_CONST(real,u[grid],uLocal);
    OV_GET_SERIAL_ARRAY_CONST(real,v[grid],vLocal);

    const int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost); 

    if( ok )
    {
      vLocal(I1,I2,I3,N)=uLocal(I1,I2,I3,N);
    }
  }
  // -- set component names --
  for( int n=0; n<numberOfComponents; n++ )
    v.setName(u.getName(n),n);

  // offset = current component position in v 
  int offset=numberOfComponents;

  if( twilightZoneFlow )
  {
    // For twilight zone flow we plot the errors
    OGFunction & exact = *parameters.dbase.get<OGFunction* >("exactSolution");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.dimension(),I1,I2,I3);

      const realArray & x= mg.center();
      OV_GET_SERIAL_ARRAY_CONST(real,x,xLocal);
      OV_GET_SERIAL_ARRAY_CONST(real,u[grid],uLocal);
      OV_GET_SERIAL_ARRAY_CONST(real,v[grid],vLocal);

      const int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost); 

      if( ok )
      {
        bool isRectangular=false;
        realSerialArray u0(I1,I2,I3);
	for( int n=N.getBase(); n<=N.getBound(); n++ )
	{
          //kkc 070920 some TW functions (like Pulse) need the last index to match n in the call below
	  u0.reshape(I1,I2,I3,Range(n,n)); 
	  exact.gd( u0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,n,gf0.t);
	  vLocal(I1,I2,I3,n+offset)=vLocal(I1,I2,I3,n)-u0;
	}
      }

    }
    for( int n=0; n<numberOfComponents; n++ )
      v.setName(u.getName(n)+" (error)",offset+n);

    offset+=numberOfComponents;
    
  }
  else 
  {
    // -- real run ---

    const int rc=parameters.dbase.get<int >("rc");
    const int tc=parameters.dbase.get<int >("tc");
    const int sc=parameters.dbase.get<int >("sc");
    
    // evaluate the pressure:
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      parameters.getDerivedFunction("pressure",u[grid],v[grid],grid,offset,gf0.t,parameters);
    }
    
    if( pde==CnsParameters::compressibleNavierStokes )
    {
      v.setName("p",offset); offset++;
    }
    else if( pde==CnsParameters::compressibleMultiphase )
    { // there are two pressures added in this case
      v.setName("ps",offset); offset++;  
      v.setName("pg",offset); offset++;
    }

    // --- fill in the a known solution and errors ---
    if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")!=CnsParameters::noKnownSolution )
    {
      realCompositeGridFunction & uKnown = parameters.getKnownSolution( cg, gf0.t );

      const int rc=parameters.dbase.get<int >("rc");
      const int uc=parameters.dbase.get<int >("uc");
      const int vc=parameters.dbase.get<int >("vc");
      const int wc=parameters.dbase.get<int >("wc"); // for swirl equation if present
      const int tc=parameters.dbase.get<int >("tc");
      const int pc=parameters.dbase.get<int >("pc");

      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);
	OV_GET_SERIAL_ARRAY(real,v[grid],vLocal);

	const int includeGhost=1;
	bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost); 
	if( !ok ) continue;

	// save the true solution
	OV_GET_SERIAL_ARRAY_CONST(real,u[grid],uLocal);
	OV_GET_SERIAL_ARRAY_CONST(real,uKnown[grid],uKnownLocal);

        int cc=offset;

        vLocal(I1,I2,I3,cc)=uKnownLocal(I1,I2,I3,rc); cc++;
        vLocal(I1,I2,I3,cc)=uKnownLocal(I1,I2,I3,uc); cc++;
        vLocal(I1,I2,I3,cc)=uKnownLocal(I1,I2,I3,vc); cc++;
	if( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
	{
	  vLocal(I1,I2,I3,cc)=uKnownLocal(I1,I2,I3,wc); cc++;
	}
        vLocal(I1,I2,I3,cc)=uKnownLocal(I1,I2,I3,tc); cc++;
       
        // here are the errors
        vLocal(I1,I2,I3,cc)=uLocal(I1,I2,I3,rc)-uKnownLocal(I1,I2,I3,rc); cc++;
        vLocal(I1,I2,I3,cc)=uLocal(I1,I2,I3,uc)-uKnownLocal(I1,I2,I3,uc); cc++;
        vLocal(I1,I2,I3,cc)=uLocal(I1,I2,I3,vc)-uKnownLocal(I1,I2,I3,vc); cc++;
	if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
	{
	  vLocal(I1,I2,I3,cc)=uLocal(I1,I2,I3,wc)-uKnownLocal(I1,I2,I3,wc); cc++;
	}
        vLocal(I1,I2,I3,cc)=uLocal(I1,I2,I3,tc)-uKnownLocal(I1,I2,I3,tc); cc++;

      }
      int cc=offset;
      v.setName("rhoTrue",cc); cc++;
      v.setName("uTrue"  ,cc); cc++;
      v.setName("vTrue"  ,cc); cc++;
      if( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
      {
	v.setName("wTrue"  ,cc); cc++;
      }
      v.setName("TTrue"  ,cc); cc++;
      
      v.setName("rhoErr",cc); cc++;
      v.setName("uErr"  ,cc); cc++;
      v.setName("vErr"  ,cc); cc++;
      if( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
      {
	v.setName("wErr"  ,cc); cc++;
      }
      v.setName("TErr"  ,cc); cc++;

      offset =cc;
      
    }
    
  } // if !twilight zone
  
  if( parameters.isAdaptiveGridProblem() && parameters.dbase.get<int >("showAmrErrorFunction") )
  {
    // add on the AMR error estimate
    assert( nAmrErr==offset );
    real timeAmr=getCPU();

    const int ec = offset;
    #ifndef USE_PPP
      realCompositeGridFunction error;
      error.link(v,Range(ec,ec));
      bool computeOnFinestLevel=true;
      getAmrErrorFunction(u,gf0.t,error,computeOnFinestLevel);
    #else
      realCompositeGridFunction error(cg);
      bool computeOnFinestLevel=true;
      getAmrErrorFunction(u,gf0.t,error,computeOnFinestLevel);
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        assign(v[grid],all,all,all,ec, u[grid],all,all,all, 0 );
      }
      
    #endif
    v.setName("error estimate",ec);

    // do NOT count time spent here as part of AMR, but rather part of plotting
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrErrorFunction"))-=getCPU()-timeAmr; 

    offset++;
  }
  
  if( parameters.dbase.get<int >("showResiduals") || timeSteppingMethod==Parameters::steadyStateRungeKutta )
  {
    // --- plot residuals ---
    assert( nResid==offset );
    realCompositeGridFunction & residual = fn[0];   // pointer to du/dt

    // printf(" getAug: *** residual.numberOfComponentGrids=%i **\n",residual.numberOfComponentGrids());
    // printf(" getAug: *** v.numberOfComponentGrids=%i **\n",v.numberOfComponentGrids());
    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);

      OV_GET_SERIAL_ARRAY(real,v[grid],vLocal);

      const int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost); 
      if( !ok ) continue;

      OV_GET_SERIAL_ARRAY_CONST(real,residual[grid],residualLocal);
      OV_GET_SERIAL_ARRAY_CONST(int,cg[grid].mask(),maskLocal);

      vLocal(I1,I2,I3,N+offset)=residualLocal(I1,I2,I3,N);

      where( maskLocal(I1,I2,I3)<=0 )
      {
        for( int n=N.getBase(); n<=N.getBound(); n++ )
          vLocal(I1,I2,I3,n+offset)=0.;   // set residual to zero at unused and interp points
      }
      
    }
    for( int n=0; n<numberOfComponents; n++ )
      v.setName(u.getName(n)+" (residual)",n+offset);

    offset+=numberOfComponents;
  }
  
  if( parameters.isMovingGridProblem() && parameters.dbase.get<int >("plotGridVelocity") )
  {
    // plot the grid velocity 
    assert( nGridVelocity==offset );
    Range Rx=cg.numberOfDimensions();
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      OV_GET_SERIAL_ARRAY(real,v[grid],vLocal);
      const int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost); 
      if( !ok ) continue;

      if( parameters.gridIsMoving(grid) )
      {
	realMappedGridFunction & gridVelocity = gf0.getGridVelocity(grid);
        OV_GET_SERIAL_ARRAY(real,gridVelocity,gridVelocityLocal);

	vLocal(I1,I2,I3,Rx+offset)=gridVelocityLocal(I1,I2,I3,Rx);
      }
      else
      {
        vLocal(I1,I2,I3,Rx+offset)=0.;
      }
      
    }
    for( int n=0; n<cg.numberOfDimensions(); n++ )
      v.setName(sPrintF("gridVelocity%i",n),n+offset);

    offset+=cg.numberOfDimensions();
  }

  assert( offset==numberOfAugmentedComponents );
  
  checkArrayIDs(sPrintF("getAugmentedSolution: near end") ); 

  return v;
}
