#include "DomainSolver.h"
#include "Cgins.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "App.h"
#include "GenericGraphicsInterface.h"

// int 
// getAmrErrorFunction(realCompositeGridFunction & u, 
//                     real t,
//                     Parameters & parameters,
//                     realCompositeGridFunction & error,
//                     bool computeOnFinestLevel =false );


realCompositeGridFunction & Cgins::
getAugmentedSolution( GridFunction & gf0, realCompositeGridFunction & v )
// ========================================================================================
// /Description:
//    Return a grid function for plotting and for output that may contain extra variables, such
// as errors or such as the pressure for the compressible NS.
// /gf0 (input) : input grid function.
// /v (input) : grid function to hold the result, IF extra variables area added.
// /Return values: The possibly augmented solution.
// ========================================================================================
{
  checkArrayIDs(sPrintF("getAugmentedSolution: start") ); 

  // No need to compute the augmented solution if graphics plotting is off
  if( (parameters.dbase.get<GenericGraphicsInterface* >("ps")!=NULL && 
      !parameters.dbase.get<GenericGraphicsInterface* >("ps")->graphicsIsOn()) ||
      parameters.dbase.get<int>("simulateGridMotion")>0  )
  {
    return gf0.u;
  }
  
  CompositeGrid & cg = gf0.cg;
  realCompositeGridFunction & u = gf0.u;

  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  

  bool plotMoreComponents = FALSE;
  int extra=0;
  int dc;   // save divergence here
  dc=numberOfComponents;   // location of divergence
  extra+=1;  // plot the divergence too
  if( parameters.dbase.get<InsParameters::PDEModel >("pdeModel")==InsParameters::viscoPlasticModel )
    extra+=2;  // plot visco-plastic yield-surface, and || sigma ||

  if( parameters.isAdaptiveGridProblem() && parameters.dbase.get<int >("showAmrErrorFunction") )
    extra+=1;

  if( parameters.dbase.get<int >("showResiduals") || 
      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
  {
    // plot residuals too
    extra+=numberOfComponents;
    dc=2*numberOfComponents;
  }
  
  

  if( parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    // For twilight zone flow we also plot the errors
    plotMoreComponents=true;
    dc+=numberOfComponents;
    
    Range all;
    Index I1,I2,I3;
    v.updateToMatchGrid(cg,all,all,all,numberOfComponents*2+extra);  // extra space for errors
    Range N(0,numberOfComponents-1);
    Range N2(numberOfComponents,2*numberOfComponents-1);

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.dimension(),I1,I2,I3);

      const realArray & x= mg.center();
      bool ok=true;
      #ifdef USE_PPP
        realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v[grid],vLocal);
        const int includeGhost=1;
        ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost); 
      #else
        const realSerialArray & xLocal = x;
        const realSerialArray & uLocal = u[grid];
        const realSerialArray & vLocal = v[grid];
      #endif

      if( ok )
      {
        vLocal(I1,I2,I3,N)=uLocal(I1,I2,I3,N);

        bool isRectangular=false;
        realSerialArray u0(I1,I2,I3);
	for( int n=N.getBase(), n2=N2.getBase(); n<=N.getBound(); n++, n2++ )
	{
	  parameters.dbase.get<OGFunction* >("exactSolution")->gd( u0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,n,gf0.t);
	  vLocal(I1,I2,I3,n2)=vLocal(I1,I2,I3,n)-u0;
	}
      }

//       v[grid](I1,I2,I3,N)=u[grid](I1,I2,I3,N);
//       v[grid](I1,I2,I3,N2)=v[grid](I1,I2,I3,N)-(*parameters.dbase.get<OGFunction* >("exactSolution"))(cg[grid],I1,I2,I3,N,gf0.t);

    }
    for( int n=0; n<numberOfComponents; n++ )
    {
      v.setName(u.getName(n),n);
      v.setName(u.getName(n)+" (error)",n+numberOfComponents);
    }
  }
  else if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")!=InsParameters::noKnownSolution )
  {
    // --- Known Solution ----
    plotMoreComponents=true;
    dc+=2*numberOfComponents; // plot errors and true solution

   realCompositeGridFunction & uKnown = parameters.getKnownSolution( cg, gf0.t );

    Range all;
    Index I1,I2,I3;
    v.updateToMatchGrid(cg,all,all,all,numberOfComponents*3+extra);  // extra space for errors
    Range N(0,numberOfComponents-1);

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.dimension(),I1,I2,I3);
      OV_GET_SERIAL_ARRAY_CONST(real,uKnown[grid],uKnownLocal);

      const realArray & x= mg.center();
      bool ok=true;
      #ifdef USE_PPP
        realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v[grid],vLocal);
        const int includeGhost=1;
        ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost); 
      #else
        const realSerialArray & xLocal = x;
        const realSerialArray & uLocal = u[grid];
        const realSerialArray & vLocal = v[grid];
      #endif

      if( ok )
      {
        vLocal(I1,I2,I3,N)=uLocal(I1,I2,I3,N);
        vLocal(I1,I2,I3,N+numberOfComponents)=uLocal(I1,I2,I3,N)-uKnownLocal(I1,I2,I3,N);
        vLocal(I1,I2,I3,N+2*numberOfComponents)=uKnownLocal(I1,I2,I3,N);
      }

    }
    for( int n=0; n<numberOfComponents; n++ )
    {
      v.setName(u.getName(n),n);
      v.setName(u.getName(n)+"-err",n+numberOfComponents);
      v.setName(u.getName(n)+"-true",n+2*numberOfComponents);
    }
  }
  

  const int uc=parameters.dbase.get<int >("uc");
  const int vc=parameters.dbase.get<int >("vc");
  const int wc=parameters.dbase.get<int >("wc");
  int grid;
  Index Ib1,Ib2,Ib3,all;
  Index I1,I2,I3;
  
  if( !plotMoreComponents )
  {
    Range all;
    v.updateToMatchGrid(cg,all,all,all,numberOfComponents+extra); 
    plotMoreComponents=true;
      
    Range N=numberOfComponents;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      // v[grid](all,all,all,N)=u[grid](all,all,all,N);    // copy existing components
      assign( v[grid],all,all,all,N, u[grid],all,all,all,N); // *wdh* 110204
    }
    
    for( int n=0; n<numberOfComponents; n++ )
      v.setName(u.getName(n),n);
  }
  
  
  // ---------------------------
  // --- Plot the divergence ---
  // ---------------------------
  CompositeGridOperators & cgop = *gf0.u.getOperators();
  v.setOperators(cgop);
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    MappedGridOperators & op = cgop[grid];
      
    // fixed for P++ 060928 *wdh*
#ifdef USE_PPP
    realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v[grid],vLocal);
#else
    realSerialArray & vLocal = v[grid];
#endif

    getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
#ifdef USE_PPP
    // restrict bounds to local processor, include ghost
    bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,1);   
    if( !ok ) continue;  // no points on this processor
#endif
    vLocal(all,all,all,dc)=0.;
      
    getIndex(mg.gridIndexRange(),I1,I2,I3); // only compute divergence here
#ifdef USE_PPP
    ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,1);   
    if( !ok ) continue;  // no points on this processor
#endif
      
    realSerialArray ux(I1,I2,I3);
    op.derivative(MappedGridOperators::xDerivative,vLocal,ux,I1,I2,I3,uc);  // u.x
    vLocal(I1,I2,I3,dc)=ux;
    if( cg.numberOfDimensions()>1 )
    {
      op.derivative(MappedGridOperators::yDerivative,vLocal,ux,I1,I2,I3,vc);   // v.y
      vLocal(I1,I2,I3,dc)+=ux;
    }
    if( cg.numberOfDimensions()>2 )
    {
      op.derivative(MappedGridOperators::zDerivative,vLocal,ux,I1,I2,I3,wc);   // w.z
      vLocal(I1,I2,I3,dc)+=ux;
    }
      
      
    if( cg.numberOfDimensions()==2 )
    {
      // fix for axisymmetric
	
      if( parameters.isAxisymmetric() )
      {
	// div(u) = u.x + v.y + v/y for y>0   or u.x + 2 v.y at y=0
#ifdef USE_PPP
	realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
#else
	const realSerialArray & uLocal = u[grid];
	const realSerialArray & xLocal = mg.vertex();
#endif

	RealArray radiusInverse(I1,I2,I3);
	radiusInverse = 1./max(REAL_MIN,xLocal(I1,I2,I3,axis2));
	Index Ib1,Ib2,Ib3;
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
	    if( cg[grid].boundaryCondition(side,axis)==Parameters::axisymmetric )
	    {
	      getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	      bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,Ib1,Ib2,Ib3,1);   
	      if( !ok ) continue;  // no points on this processor
		
	      radiusInverse(Ib1,Ib2,Ib3)=0.;
	      op.derivative(MappedGridOperators::yDerivative,uLocal,ux,Ib1,Ib2,Ib3,vc);  // v.y
	      vLocal(Ib1,Ib2,Ib3,dc)+=ux(Ib1,Ib2,Ib3);  // add v.y on the axis instead of v/y
	    }
	  }
	}
	// add v/y (except on the axis where radiusInverse=0)
	vLocal(I1,I2,I3,dc)+=uLocal(I1,I2,I3,vc)*radiusInverse;     
      }
    }
    else
    {
    }
      
    
  } // end for grid 
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    v[grid].updateGhostBoundaries();  // *wdh* 110223
  }
  
  
  v.setName("div",dc);

  if( parameters.dbase.get<InsParameters::PDEModel >("pdeModel")==InsParameters::viscoPlasticModel )
  {
    // save the visco-plastic yield-surface, and || sigma ||
    const int ysc = dc+1;  // position of the yield-surface
    parameters.getDerivedFunction("viscoPlasticVariables",u,v,ysc,gf0.t,parameters);
    v.setName("yield",ysc);
    v.setName("eDot",ysc+1);
    v.interpolate(Range(ysc,ysc+1));
  }
    

  // *wdh* 110222 - interpolate the divergence (in parallel too)
  v.interpolate(Range(dc,dc));
  
  if( parameters.isAdaptiveGridProblem() && parameters.dbase.get<int >("showAmrErrorFunction") )
  {
    // add on the AMR error estimate
    Range all;
    if( !plotMoreComponents )
    {
      v.updateToMatchGrid(cg,all,all,all,numberOfComponents+extra); 
      plotMoreComponents=true;
    }
    
    int ec = v.getComponentBound(0);
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
  }
  
  if( true && (parameters.dbase.get<int >("showResiduals") || 
      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta) )
  {
    const Parameters::ImplicitMethod & implicitMethod = 
      parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
    if( implicitMethod==Parameters::approximateFactorization )
    {
      printF("Cgins::getAugmentedSolution:ERROR: currently unable to plot residuals for the "
              "approximateFactorization scheme. FIX ME!\n");
      OV_ABORT("ERROR");
    }
    

    if( !plotMoreComponents )
    {
      plotMoreComponents=true;

      Range all;
      v.updateToMatchGrid(cg,all,all,all,numberOfComponents+extra); 

      Range N=numberOfComponents;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	// v[grid](all,all,all,N)=u[grid](all,all,all,N);    // copy existing components
        assign(v[grid],all,all,all,N, u[grid],all,all,all,N ); // Assign two arrays without communication
      }
      
      for( int n=0; n<numberOfComponents; n++ )
	v.setName(u.getName(n),n);
    }

    Range N(0,numberOfComponents-1);
    int offset=numberOfComponents;
    if( parameters.dbase.get<bool >("twilightZoneFlow") )
      offset+=numberOfComponents;

    // ******** fix this ***********
    realCompositeGridFunction * pResidual= &fn[0];  // default location for the residual
    
    // if( true && 
    //    parameters.dbase.get<int>("useNewImplicitMethod")==1 &&  //  ** fix this **
    //    parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod")==Parameters::implicit )
    if( parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod")==Parameters::implicit )
    {
	
      pResidual = &fn[2]; //  uti;  // save residual here -- check this **************
      if( parameters.isMovingGridProblem() )
      {
	pResidual->updateToMatchGrid(gf0.cg);
      }
      
      getResidual( gf0.t,dt,gf0,*pResidual );
      // output residual info: 
      real maximumResidual, maximuml2;
      getResidualInfo( gf0.t, *pResidual, maximumResidual, maximuml2, stdout );
    }

    realCompositeGridFunction & residual = * pResidual;
  
    // printf(" getAug: *** residual.numberOfComponentGrids=%i **\n",residual.numberOfComponentGrids());
    // printf(" getAug: *** v.numberOfComponentGrids=%i **\n",v.numberOfComponentGrids());
  
    Index I1,I2,I3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);

      #ifdef USE_PPP
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);
	realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v[grid],vLocal);
	realSerialArray rLocal; getLocalArrayWithGhostBoundaries(residual[grid],rLocal);
      #else
        const intSerialArray & maskLocal = cg[grid].mask();
	const realSerialArray & vLocal = v[grid];
	const realSerialArray & rLocal = residual[grid];
      #endif

      bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,1);   
      if( !ok ) continue;  // no points on this processor

      vLocal(I1,I2,I3,N+offset)=rLocal(I1,I2,I3,N);
      where( maskLocal(I1,I2,I3)<=0 )
      {
	for( int n=N.getBase(); n<=N.getBound(); n++ )
	  vLocal(I1,I2,I3,n+offset)=0.;   // set residual to zero at unused and interp points
      }
      
    }
    for( int n=0; n<numberOfComponents; n++ )
    {
      v.setName(u.getName(n)+" (residual)",n+offset);
    }
  }
  checkArrayIDs(sPrintF("getAugmentedSolution: near end") ); 
  
  realCompositeGridFunction & ur = plotMoreComponents ? v : gf[current].u;
  //   if( true ) // *wdh* 060228
  //   {
  //     gf0.u.display("\n\n getAugmentedSolution: gf0.u","%5.2f ");
  //     ur.display("\n\n getAugmentedSolution: ur","%5.2f ");
  //   }
  return ur;
}
