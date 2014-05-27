#include "Cgad.h"
#include "AdParameters.h"
#include "App.h"
#include "ParallelUtility.h"

//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)


int Cgad::
applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
			realMappedGridFunction & gridVelocity,
			const int & grid,
			const int & option /* =-1 */,
			realMappedGridFunction *puOld /* =NULL */,  
			realMappedGridFunction *pGridVelocityOld /* =NULL */,
			const real & dt /* =-1. */ )
//=========================================================================================
// /Description:
//   Apply boundary conditions for the advection diffusion equations
// 
// /t (input):
// /u (input/output) : apply to this grid function.
// /gridIsMoving (input) : true if this grid is moving.
// /gridVelocity (input) : the grid velocity if gridIsMoving==true.
// /variableBoundaryData (input) : true if there is boundary data that depends on the position along the boundary.
// /boundaryData (input) : boundary data used if variableBoundaryData==true.
// grid (input) : the grid number if this MappedGridFunction is part of a CompositeGridFunction.
// option (input): not used here.
//
// /Note:
// ***Remember to also change the BC routine for implicit time stepping if changes are made here
// applyBoundaryConditionsForImplicitTimeStepping
//
// 
//\end{MappedGridSolverInclude.tex}  
//=========================================================================================
{
  real time0=getCPU();

  const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
  assert( orderOfAccuracy==2 || orderOfAccuracy==4 );

  BoundaryConditionParameters bcParams;
  const int numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  Range C(0,numberOfComponents-1);

  const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
  BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);

  if( parameters.getGridIsImplicit(grid) )
  {
    // *wdh* 080829 -- only apply some BC's for implicit time stepping grids

    u.applyBoundaryCondition(C,BCTypes::dirichlet,Parameters::dirichletBoundaryCondition,bcData,pBoundaryData,t,
			     bcParams,grid); 
    u.applyBoundaryCondition(C,BCTypes::neumann,Parameters::neumannBoundaryCondition,bcData,pBoundaryData,t,bcParams,grid);

    // assigned extended boundaries: *wdh* 080829 -- treat the case when a dirichlet-BC is next to an interface
    if( false )
    {
      bcParams.extraInTangentialDirections= orderOfAccuracy==2 ? 1 : 2;
      u.applyBoundaryCondition(C,BCTypes::dirichlet,Parameters::dirichletBoundaryCondition,bcData,pBoundaryData,t,
			       bcParams,grid); 
      bcParams.extraInTangentialDirections=0;
    }
    
    return 0;
  }
  

  if( debug() & 8 )
  {
    printF(">>>>> Cgad::applyBoundaryConditions <<<<<<<\n");
  }
  checkArrayIDs("  Cgad::applyBoundaryConditions: start"); 

  const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

  MappedGrid & mg = *u.getMappedGrid();
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) 
  {
    for( int side=0; side<=1; side++ )
    {
      int bc = mg.boundaryCondition(side,axis);
      if( !(bc==AdParameters::dirichletBoundaryCondition ||
            bc==AdParameters::neumannBoundaryCondition ||
            bc==AdParameters::mixedBoundaryCondition ||
            bc == AdParameters::interfaceBoundaryCondition ||
            bc == AdParameters::axisymmetric ||
            bc <=0 ) )
      {
        printF("Cgad:applyBoundaryConditions:ERROR: unexpected boundary condition bc=%i for (grid,side,axis)=(%i,%i,%i)\n",
               bc,grid,side,axis);
	Overture::abort("error");
      }
      // --- check that interface boundaries use the correct bc ---
      if( parameters.dbase.get<int>("applyInterfaceBoundaryConditions")==1 &&
          interfaceType(side,axis,grid)!=Parameters::noInterface &&
          bc!=AdParameters::mixedBoundaryCondition )
      {
	printP("Cgad:applyBoundaryConditions:ERROR:the interface on (side,axis,grid)=(%i,%i,%i)\n"
               " should have a mixed boundary condition associated with it, but bc=%i\n",
	       side,axis,grid,bc);
        Overture::abort("error");
      }
    }
  }
  

  u.applyBoundaryCondition(C,BCTypes::dirichlet,Parameters::dirichletBoundaryCondition,bcData,pBoundaryData,t,
			   bcParams,grid); 

  BoundaryConditionParameters extrapParams;
  extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfAccuracy")+1;
  // *wdh* 071125 u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,extrapParams);
  u.applyBoundaryCondition(C,BCTypes::extrapolate,Parameters::dirichletBoundaryCondition,0.,t,extrapParams);


  u.applyBoundaryCondition(C,BCTypes::neumann,Parameters::neumannBoundaryCondition,bcData,pBoundaryData,t,bcParams,grid);

  // An interface could have a mixed-BC which is really a dirichlet BC -- we need to check this 
  // u.applyBoundaryCondition(C,BCTypes::neumann,Parameters::mixedBoundaryCondition,bcData,pBoundaryData,t,bcParams,grid);
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) 
  {
    for( int side=0; side<=1; side++ )
    {
      // printP("applyBC: interfaceType(side=%i,axis=%i,grid=%i)=%i (noInterface=%i)\n",side,axis,grid,interfaceType(side,axis,grid),
      // 	     (int)Parameters::noInterface );
      
      if( mg.boundaryCondition(side,axis)==AdParameters::mixedBoundaryCondition )
      {
	if( interfaceType(side,axis,grid)!=Parameters::noInterface )
	{ // This is an interface between domains

          if( parameters.dbase.get<int>("applyInterfaceBoundaryConditions")==0 )
	  {
	    printP("Cgad:applyBC:skip interface bc: interfaceType(side=%i,axis=%i,grid=%i)=%i\n",
		   side,axis,grid,interfaceType(side,axis,grid));
	    continue;
	  }

          // what about BC's applied at t=0 before the boundary data is set ??
          // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****

	  // if this is an interface we should turn off the TZ forcing for the boundary condition since we want
	  // to use the boundary data instead.
          if( debug() & 4 )
	    printP("Cgad:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
		   side,axis,grid,mixedCoeff(0,side,axis,grid),mixedNormalCoeff(0,side,axis,grid));

          // RealArray *&pBoundaryData = boundaryData[grid].boundaryData[side][axis];
          // if( pBoundaryData!=NULL )
          if( pBoundaryData[side][axis]!=NULL )
	    u.getOperators()->setTwilightZoneFlow( false );
	  else
	  {
	    printP("$$$$ Cgad:applyBC:INFO:interface: pBoundaryData is NULL for [side,axis,grid]=[%i,%i,%i], "
                   "t=%9.3e.\n",
		   side,axis,grid,t );
	  }
	  
	}
	for( int c=0; c<numberOfComponents; c++ )
	{
          if( mixedNormalCoeff(c,side,axis,grid)!=0. ) // coeff of T.n is non-zero
	  {
	    real a0=mixedCoeff(c,side,axis,grid);
	    real a1=mixedNormalCoeff(c,side,axis,grid);
	    bcParams.a.redim(3);
	    bcParams.a(0)=a0;
	    bcParams.a(1)=a1;
	    bcParams.a(2)=0.;
            u.applyBoundaryCondition(c,BCTypes::mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,
                                     t,bcParams,grid);
	  }
	  else
	  {
            printP("Cgad:applyBC: apply a dirichlet BC on the interface (side,axis,grid)=(%i,%i,%i)\n",side,axis,grid);
            real a0=mixedCoeff(c,side,axis,grid);
	    assert( a0==1. );
            u.applyBoundaryCondition(c,BCTypes::dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,
                                     t,bcParams,grid);

            // do this for now:  -- we could do better --
            u.applyBoundaryCondition(c,BCTypes::extrapolate,BCTypes::boundary(side,axis),0.,t,extrapParams);
	  }
	}
	if( interfaceType(side,axis,grid)!=Parameters::noInterface )
	{ // reset TZ
          u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
	}
      }
    }
  }
  


  u.applyBoundaryCondition(C,BCTypes::neumann,Parameters::axisymmetric,0.,t);


  u.finishBoundaryConditions();

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-time0;

  return 0;
}


int Cgad::
applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & u, 
					       realMappedGridFunction & uL,
					       realMappedGridFunction & gridVelocity,
					       real t,
					       int scalarSystem,
					       int grid )
// =======================================================================================================
//  /Description:
//      Fill in the RHS values for implicit time-stepping.
//
// /u (output) : the RHS
// /uL (input) : for linearized equations
// 
// See ins/src/implicit.C for another example of this function is written.
// =======================================================================================================
{
//  Overture::abort("Cgad::applyBoundaryConditionsForImplicitTimeStepping:ERROR: not implemented");
//  return 0;


  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");

  Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables

  typedef int BoundaryCondition;
  
  const BoundaryCondition & dirichletBoundaryCondition = Parameters::dirichletBoundaryCondition;
  const BoundaryCondition & neumannBoundaryCondition = Parameters::neumannBoundaryCondition;
  const Parameters::BoundaryCondition & interfaceBoundaryCondition= Parameters::interfaceBoundaryCondition;
  
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
  //   extrapolate           = BCTypes::extrapolate,
  //   allBoundaries         = BCTypes::allBoundaries,
    normalComponent       = BCTypes::normalComponent;

  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

  // *** assign boundary conditions for the implicit method ***** 

  if( parameters.getGridIsImplicit(grid) )
  {
    // ** Note that we are assigning the RHS for the implicit solve ***

    // The RHS for the interface equations is placed in the BoundaryDataArray
    BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);

    u.applyBoundaryCondition(N,dirichlet,dirichletBoundaryCondition,bcData,pBoundaryData,t,
			     Overture::defaultBoundaryConditionParameters(),grid);
    // ** Note: neumann BC is applied below since we fill in the RHS 
    // u.applyBoundaryCondition(N,neumann,neumannBoundaryCondition,bcData,t,
    // 		                Overture::defaultBoundaryConditionParameters(),grid);
    // u.applyBoundaryCondition(N,dirichlet,interfaceBoundaryCondition,bcData,t,
    // 			     Overture::defaultBoundaryConditionParameters(),grid);
    
    MappedGrid & mg = *u.getMappedGrid();
    const bool isRectangular=mg.isRectangular();
    if( !isRectangular || twilightZoneFlow ) // we need this now
      mg.update(MappedGrid::THEvertexBoundaryNormal); 

    #ifdef USE_PPP
      const realSerialArray & uLocal = u.getLocalArray();
    #else
      realSerialArray & uLocal = u; 
    #endif

    const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
      
    realArray & x= mg.center();
    #ifdef USE_PPP
      realSerialArray xLocal; 
      if( !rectangular || twilightZoneFlow ) 
        getLocalArrayWithGhostBoundaries(x,xLocal);
    #else
      const realSerialArray & xLocal = x;
    #endif
    
    const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

    const bool interfaceBoundaryConditionsAreSpecified=parameters.dbase.has_key("interfaceCondition");
    IntegerArray & interfaceCondition = (interfaceBoundaryConditionsAreSpecified ? 
					 parameters.dbase.get<IntegerArray>("interfaceCondition") :
					 Overture::nullIntArray() );

    int side,axis;
    Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
    Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];

    Index Jbv[3], &Jb1=Jbv[0], &Jb2=Jbv[1], &Jb3=Jbv[2];
    Index Jgv[3], &Jg1=Jgv[0], &Jg2=Jgv[1], &Jg3=Jgv[2];

    Index Ip1,Ip2,Ip3;
    for( axis=0; axis<mg.numberOfDimensions(); axis++ ) 
    {
      for( side=0; side<=1; side++ )
      {

	int bc = mg.boundaryCondition(side,axis);
	// if( bc == Parameters::interfaceBoundaryCondition && interfaceBoundaryConditionsAreSpecified )
	if( interfaceType(side,axis,grid) != Parameters::noInterface )
	{
          // this face is on a domain interface
	  bc = interfaceCondition(side,axis,grid);
	  assert( bc==Parameters::dirichletInterface || bc==Parameters::neumannInterface );
	}

	if( bc==Parameters::dirichletInterface )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  const int includeGhost=1;
	  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	  if( !ok ) continue;
	    
	  if( debug() & 4 )
	    printP("** Cgad:applyImpBC: setting RHS for a dirichlet interface bc(%i,%i,%i)=%i\n",side,axis,grid,bc);
	  // u.applyBoundaryCondition(N,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
	  // 			   Overture::defaultBoundaryConditionParameters(),grid);

          RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	  
	  if( debug() & 4 )
	  {
	    ::display(uLocal(Ib1,Ib2,Ib3,N),"RHS for dirichlet BC: uLocal(Ib1,Ib2,Ib3,N)","%7.4f ");

            if( twilightZoneFlow )
	    {
              
	      OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	      RealArray ue(Ib1,Ib2,Ib3,N);
	      int rectangular=0;
	      e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,N,t);

              ::display(ue(Ib1,Ib2,Ib3,N),"Cgad:imp: EXACT RHS for dirichlet BC: bd(Ib1,Ib2,Ib3,N)","%7.4f ");

	      // printP("Cgad:imp: set RHS to exact\n");
	      // bd(Ib1,Ib2,Ib3,N)=ue(Ib1,Ib2,Ib3,N);  // *****************************************************************

	    }
	  }

	  uLocal(Ib1,Ib2,Ib3,N)=bd(Ib1,Ib2,Ib3,N);
	  
	}
	else if( bc==neumannBoundaryCondition || 
                 bc==AdParameters::mixedBoundaryCondition ||
                 bc==Parameters::neumannInterface ||
                 bc==Parameters::axisymmetric )
	{
	  // ****** neumann boundary condition *****
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

          #ifdef USE_PPP
           RealArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
           const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
          #else
           const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
          #endif

          const int includeGhost=1;
          bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
          ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
	  if( !ok ) continue;
	    
	  if( bc==Parameters::neumannInterface )
	  {
            // The RHS for the interface equations is placed in the BoundaryDataArray
            // (This includes TZ forcing)
	    if( debug() & 4 )
	      printP("** Cgad:applyImpBC: setting RHS for a neumann interface bc(%i,%i,%i)=%i\n",side,axis,grid,bc);
            RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	    if( debug() & 4 && twilightZoneFlow )
	    {
	      ::display(bd(Ib1,Ib2,Ib3,N),"Cgad:imp: RHS for Neumann BC: bd(Ib1,Ib2,Ib3,N)","%7.4f ");

	      OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	      RealArray ue(Ib1,Ib2,Ib3,N), uex(Ib1,Ib2,Ib3,N), uey(Ib1,Ib2,Ib3,N);
	      int rectangular=0;
              e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,N,t);
	      e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,N,t);
	      e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,N,t);

	      if( mg.numberOfDimensions()==2 )
	      {
		for( int n=N.getBase(); n<=N.getBound(); n++ )
		{
                  real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
		  ue(Ib1,Ib2,Ib3,n)=a1*(uex(Ib1,Ib2,Ib3,n)*normal(Ib1,Ib2,Ib3,0)+
				        uey(Ib1,Ib2,Ib3,n)*normal(Ib1,Ib2,Ib3,1))+ a0*ue(Ib1,Ib2,Ib3,n);
                  ::display(ue(Ib1,Ib2,Ib3,n),"Cgad:imp: EXACT RHS for Neumann BC: bd(Ib1,Ib2,Ib3,N)","%7.4f ");
                  ::display(fabs(bd(Ib1,Ib2,Ib3,n)-ue(Ib1,Ib2,Ib3,n)),
                           "Cgad:imp: ERROR in RHS for Neumann BC: bd(Ib1,Ib2,Ib3,N)","%7.4f ");

		  // ** bd(Ib1,Ib2,Ib3,N) *= 1./a1;
		  
		}
	      }
	      // printP("Cgad:imp: set RHS to exact\n");
	      // bd(Ib1,Ib2,Ib3,N)=ue(Ib1,Ib2,Ib3,N);  // ***********************************************************
	    }
	    
            uLocal(Ig1,Ig2,Ig3,N)=bd(Ib1,Ib2,Ib3,N);

            // If the adjacent face is a dirichlet BC then we change the right-hand-side on the extended
            // boundary point: 
            // 
            //   bc=dirichlet
            //       ------c--o   <-  adjust RHS to neuman interface here 
            //             |
            //       ------+--+   <- interface side 
            //             |
            // Since the interior equation is not applied at the corner pt "c" we can not just impose
            // [ k T.n ]=0 since we need another equation to determine the 2 ghost point values (there is
            // another ghost pt value on the other side of the interface ). Therefore we set 
            //  k*T.n = given at point "c" 

            // loop over adjacent sides
	    for( int dir=1; dir<mg.numberOfDimensions(); dir++ ) for( int side2=0; side2<=1; side2++ )
	    {
	      int dir2 = (axis+dir) % mg.numberOfDimensions();
              if( mg.boundaryCondition(side2,dir2)==dirichletBoundaryCondition )
	      {
		Jb1=Ib1, Jb2=Ib2, Jb3=Ib3;
		Jg1=Ig1, Jg2=Ig2, Jg3=Ig3;
                // check for parallel: 
                if( Jbv[dir2].getBase()  > mg.gridIndexRange(side2,dir2) ||
                    Jbv[dir2].getBound() < mg.gridIndexRange(side2,dir2) )
		{  
		  ok=false;
		  continue;
		}
                Jbv[dir2]=mg.gridIndexRange(side2,dir2);
		Jgv[dir2]=mg.gridIndexRange(side2,dir2);

		if( debug() & 8 )
		{
		  printP("Cgad:imp: set RHS to exact where interface meets adj-dirichlet "
			 " (side,axis)=(%i,%i) (side2,dir2)=(%i,%i) Jv=[%i,%i][%i,%i][%i,%i]\n",
			 side,axis,side2,dir2,Jb1.getBase(),Jb1.getBound(),Jb2.getBase(),Jb2.getBound(),
			 Jb3.getBase(),Jb3.getBound() );
		}
		
		if( twilightZoneFlow )
		{
		  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
		  RealArray ue(Jb1,Jb2,Jb3,N), uex(Jb1,Jb2,Jb3,N), uey(Jb1,Jb2,Jb3,N);
		  int rectangular=0;
		  e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Jb1,Jb2,Jb3,N,t);
		  e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Jb1,Jb2,Jb3,N,t);
		  e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Jb1,Jb2,Jb3,N,t);

		  if( mg.numberOfDimensions()==2 )
		  {
		    for( int n=N.getBase(); n<=N.getBound(); n++ )
		    {
		      real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
		      ue(Jb1,Jb2,Jb3,n)=a1*(uex(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,0)+
					    uey(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,1))+ a0*ue(Jb1,Jb2,Jb3,n);
		    }
                    uLocal(Jg1,Jg2,Jg3,N)=ue(Jb1,Jb2,Jb3,N);
		  }
                  else 
		  {
                    RealArray uez(Jb1,Jb2,Jb3,N);
		    e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Jb1,Jb2,Jb3,N,t);
		    for( int n=N.getBase(); n<=N.getBound(); n++ )
		    {
		      real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
		      ue(Jb1,Jb2,Jb3,n)=a1*(uex(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,0)+
					    uey(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,1)+
					    uez(Jb1,Jb2,Jb3,n)*normal(Jb1,Jb2,Jb3,2))+ a0*ue(Jb1,Jb2,Jb3,n);
		    }
                    uLocal(Jg1,Jg2,Jg3,N)=ue(Jb1,Jb2,Jb3,N);
		  }
		  
		}
		else
		{
                  //MappedGridOperators & op = *(u.getOperators());
		  //op.derivative(MappedGridOperators::xDerivative,uLocal,ux  ,Jb1,Jb2,Jb3,N);
		  //op.derivative(MappedGridOperators::yDerivative,uLocal,uy  ,Jb1,Jb2,Jb3,N);
		  //op.derivative(MappedGridOperators::zDerivative,uLocal,uz  ,Jb1,Jb2,Jb3,N);

                  // Here we assume the Dirichlet BC is a constant value so that the normal derivative  *fix me*
                  // of the solution along the Dirichlet BC is zero: 
                  // *NOTE* if we fix this for the case of a variable Dirichlet BC then we must also adjust the 
                  //   interface getRHS to eval   k*u.n - k*ue.n so that the residuals in the interface equations 
                  //   will go to zero. 
		  for( int n=N.getBase(); n<=N.getBound(); n++ )
		  {
		    real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
                    // note: uLocal(Jb1,Jb2,Jb3,n) has been set above by the Dirichlet BC 
		    uLocal(Jg1,Jg2,Jg3,n)=a0*uLocal(Jb1,Jb2,Jb3,n);
		  }
		    
		}
	      }
	    }
	      


	  }
	  else if( !twilightZoneFlow )
	  {
	    if( bc==neumannBoundaryCondition || 
                bc==AdParameters::mixedBoundaryCondition )
	    {
	      for( int n=N.getBase(); n<=N.getBound(); n++ )
	      {
		uLocal(Ig1,Ig2,Ig3,n)=mixedRHS(n,side,axis,grid);  // set ghost value to RHS for neumann oe mixed BC
	      }
	    }
            else
	    {
              uLocal(Ig1,Ig2,Ig3,N)=0.;                          // axisymmetric
	    }
	  }
	  else
	  {
            // Twilight-zone forcing:

	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    RealArray ue(Ib1,Ib2,Ib3,N), uex(Ib1,Ib2,Ib3,N), uey(Ib1,Ib2,Ib3,N);
	    int rectangular=0;
	    e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,N,t);
	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,N,t);
	    e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,N,t);

	    if( mg.numberOfDimensions()==2 )
	    {
              for( int n=N.getBase(); n<=N.getBound(); n++ )
	      {
                real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
		// printF(" ***** BC: a0=%8.2e a1=%8.2e\n",a0,a1);
		
		uLocal(Ig1,Ig2,Ig3,n)=a1*(uex(Ib1,Ib2,Ib3,n)*normal(Ib1,Ib2,Ib3,0)+
				          uey(Ib1,Ib2,Ib3,n)*normal(Ib1,Ib2,Ib3,1)) + a0*ue(Ib1,Ib2,Ib3,n);
	      }
	    }
	    else
	    {
	      RealArray uez(Ib1,Ib2,Ib3,N);
	      e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,N,t);
              for( int n=N.getBase(); n<=N.getBound(); n++ )
	      {
                real a0=mixedCoeff(n,side,axis,grid), a1=mixedNormalCoeff(n,side,axis,grid);
		uLocal(Ig1,Ig2,Ig3,n)=a1*(uex(Ib1,Ib2,Ib3,n)*normal(Ib1,Ib2,Ib3,0)+
					  uey(Ib1,Ib2,Ib3,n)*normal(Ib1,Ib2,Ib3,1)+
					  uez(Ib1,Ib2,Ib3,n)*normal(Ib1,Ib2,Ib3,2)) + a0*ue(Ib1,Ib2,Ib3,n);
	      }
	    }

	  }
	}
      }
    } // end for axis
        
    // ************ try this for interfaces ********* 080909
    u.updateGhostBoundaries();

  } // end if grid is implicit
  

  return 0;


}

