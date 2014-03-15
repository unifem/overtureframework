#include "OB_MappedGridSolver.h"
#include "Parameters.h"
#include "OB_MappedGridSolver.h"
#include "turbulenceModels.h"
#include "Insbc4WorkSpace.h"
// #include "ParallelUtility.h"
#include "App.h"

void
applyFourthOrderBoundaryConditions( realMappedGridFunction & u0, real t, int grid, Insbc4WorkSpace & ws,
				    IntegerArray & ipar, RealArray & rpar, OGFunction & exact, Parameters & parameters );

//\begin{>>MappedGridSolverInclude.tex}{\subsection{applyBoundaryConditionsINS}} 
int OB_MappedGridSolver::
applyBoundaryConditionsINS(const real & t, realMappedGridFunction & u, 
			   realMappedGridFunction & gridVelocity,
                           const int & grid,
			   const int & option /* =-1 */,
			   realMappedGridFunction *puOld /* =NULL */,  
                           realMappedGridFunction *pGridVelocityOld /* =NULL */,
			   const real & dt /* =-1. */ )
//=========================================================================================
// /Description:
//   Apply boundary conditions for the incompressibleNavierStokes (explicit time stepping).
// 
// /t (input):
// /u (input/output) : applyt to this grid function.
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
  checkArrayIDs(" insBC: start"); 

  MappedGrid & c = *u.getMappedGrid();
  const bool isRectangular = c.isRectangular();
  
  // *** turn off for stretched c-grid at outflow 
  bool applyDivergenceBoundaryCondition=true; // false; // true;
  
//   MappedGrid & mg = *u.getMappedGrid();
//   printf("applyBoundaryConditionsINS: grid=%i variableBoundaryData=%i\n",grid,variableBoundaryData);
//   display(c.boundaryCondition(),sPrintF(buff,"grid=%i applyBoundaryConditionsINS: c.boundaryCondition()",grid));
   
  const bool gridIsMoving = parameters.gridIsMoving(grid);

  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");
  const int tc = parameters.dbase.get<int >("tc");
  // const int & pc = parameters.dbase.get<int >("pc");
  const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
  assert( orderOfAccuracy==2 || orderOfAccuracy==4 );
  
  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");

  typedef Parameters::BoundaryCondition BoundaryCondition;
  
  const BoundaryCondition & noSlipWall = Parameters::noSlipWall;
  const BoundaryCondition & slipWall   = Parameters::slipWall;
  const BoundaryCondition & inflowWithVelocityGiven = Parameters::inflowWithVelocityGiven;
  const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
               = Parameters::inflowWithPressureAndTangentialVelocityGiven;
  const BoundaryCondition & outflow = Parameters::outflow;
  const BoundaryCondition & tractionFree = Parameters::tractionFree;
  const BoundaryCondition & symmetry = Parameters::symmetry;
  const BoundaryCondition & dirichletBoundaryCondition = Parameters::dirichletBoundaryCondition;
  const BoundaryCondition & axisymmetric = Parameters::axisymmetric;
  const BoundaryCondition & interfaceBoundaryCondition = Parameters::interfaceBoundaryCondition;
  
  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   mixed                 = BCTypes::mixed,
                   extrapolate           = BCTypes::extrapolate,
                   normalComponent       = BCTypes::normalComponent,
                 //   aDotU                 = BCTypes::aDotU,
                   generalizedDivergence = BCTypes::generalizedDivergence,
                   tangentialComponent   = BCTypes::tangentialComponent,
                   vectorSymmetry        = BCTypes::vectorSymmetry,
                   allBoundaries         = BCTypes::allBoundaries; 


  bool assignSlipWall=false;
  bool assignNoSlipWall=false;
  bool assignInflowWithVelocityGiven=false;
  bool assignOutflow=false;
  bool assignTractionFree=false;
  bool assignAxisymmetric=false;
  bool assignSymmetry=false;
  bool assignDirichletBoundaryCondition=false;
  bool assignInflowWithPressureAndTangentialVelocityGiven=false;
  bool assignNoSlipInterface=false;
  
  int side,axis;
  for( axis=0; axis<c.numberOfDimensions(); axis++ )
  {
    for( side=0; side<=1; side++ )
    {
      int bc=c.boundaryCondition(side,axis);
      switch (bc)
      {
      case 0 : break;
      case -1: break;
      case Parameters::slipWall:                   assignSlipWall=true; break;
      case Parameters::noSlipWall :                assignNoSlipWall=true; break;
      case Parameters::inflowWithVelocityGiven:    assignInflowWithVelocityGiven=true; break;
      case Parameters::outflow:                    assignOutflow=true; break;
      case Parameters::tractionFree:               assignTractionFree=true; break;
      case Parameters::axisymmetric:               assignAxisymmetric=true; break;
      case Parameters::symmetry :                  assignSymmetry=true; break;
      case Parameters::dirichletBoundaryCondition: assignDirichletBoundaryCondition=true; break;
      case Parameters::inflowWithPressureAndTangentialVelocityGiven :
	assignInflowWithPressureAndTangentialVelocityGiven=true; break;
      case Parameters::interfaceBoundaryCondition : assignNoSlipInterface=true; break;
      default: 
        printf("insBC:ERROR: unknown boundary condition =%i on grid %i, side=%i, axis=%i\n",bc,grid,side,axis);
        throw "error";
      break;
      }
    }
  }


  const int numberOfGhostPointsNeeded = parameters.numberOfGhostPointsNeeded();

  // **************************************************************************
  //  apply boundary conditions in order of increasing priority (so corners
  //    take the values from the bc that is applied last)
  // **************************************************************************

  Range C(0,parameters.dbase.get<int >("numberOfComponents")-1);  // ***** is this correct ******
  Range V = Range(uc,uc+parameters.dbase.get<int >("numberOfDimensions")-1);
  const Range & Rt = parameters.dbase.get<Range >("Rt"); // time dependent parameters (u,v,w,[T]). 
  
  BoundaryConditionParameters extrapParams;
  BoundaryConditionParameters bcParams;

  const bool assignTemperature = parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel ||
                                 parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel;
  

  if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta && 
      t>0. )  // apply all boundary conditions at t=0
  {
    // *****************************************************
    // ***********STEADY STATE CASE*************************
    // *****************************************************

    // we only need to apply a limited number of BC's for the steady state solver since most
    // have already been done.

    if( assignSlipWall )
    {
      // on a slip wall we need to extrapolate points that lie outside interpolation pts on the bndry.
      u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0, slipWall,0.,t);
      if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1, slipWall,0.,t);

      if( !isRectangular ) // rectangular case is already done
        u.applyBoundaryCondition(V,BCTypes::generalizedDivergence,Parameters::slipWall,0.,t); 	
    }
    
    if( assignNoSlipWall )
    {
      
      if( false )
      { // for testing
	BoundaryConditionParameters bcParams;
	bcParams.lineToAssign=1;
	u.applyBoundaryCondition(vc,dirichlet,noSlipWall,0.,t,bcParams);
        u.applyBoundaryCondition(V,BCTypes::generalizedDivergence,noSlipWall,0.,t); 	
      }
      else
      {
        u.applyBoundaryCondition(V,extrapolate,interfaceBoundaryCondition,0.,t);
        u.applyBoundaryCondition(V,BCTypes::generalizedDivergence,interfaceBoundaryCondition,0.,t); 	
      }
      
    }
    if( assignNoSlipInterface )
    {
        u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t);
        u.applyBoundaryCondition(V,BCTypes::generalizedDivergence,Parameters::noSlipWall,0.,t); 	
    }
    
    
    if( parameters.dbase.get<int >("checkForInflowAtOutFlow")==1 && assignOutflow )
    {
      // *wdh* 030603 ** add these for Kyle's bug ??
      const int orderOfExtrapolation=extrapParams.orderOfExtrapolation;
      extrapParams.orderOfExtrapolation=2;
      u.applyBoundaryCondition(V,extrapolate,    outflow,0.,t,extrapParams);

      // **check for local inflow at an outflow boundary**
      // where( inflow ) give u.n=0
      Index I1,I2,I3;
    
      if( !parameters.dbase.get<bool >("twilightZoneFlow") &&  assignOutflow && orderOfAccuracy==2 )
      {
    
	for( axis=0; axis<c.numberOfDimensions(); axis++ )
	{
	  for( side=Start; side<=End; side++ )
	  {
	    if( c.boundaryCondition(side,axis)==outflow )
	    {
	      RealDistributedArray & normal  = c.vertexBoundaryNormal(side,axis);  

	      getGhostIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
	      intArray & mask = bcParams.mask();
	      mask.redim(I1,I2,I3);   // mask lives on ghost line.
	      getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
	      if( isRectangular )
	      {
		mask = u(I1,I2,I3,uc+axis)*(2*side-1) <0.;
	      }
	      else
	      {
		if( c.numberOfDimensions()==2 )
		{
		  mask = (u(I1,I2,I3,uc)*normal(I1,I2,I3,0)+
			  u(I1,I2,I3,vc)*normal(I1,I2,I3,1)) <0; 
		}
		else
		{
		  mask = (u(I1,I2,I3,uc)*normal(I1,I2,I3,0)+
			  u(I1,I2,I3,vc)*normal(I1,I2,I3,1)+
			  u(I1,I2,I3,wc)*normal(I1,I2,I3,2)) <0; 
		}
	      }
	
	      int count=sum(mask);
	      if( count>0 )
	      {
		if( debug() & 4 )
		  printf("insBC: number of outflow points that are inflow = %i\n",count);
		bcParams.setUseMask(TRUE);
		// u.applyBoundaryCondition(V,neumann,outflow,0.,t,bcParams);
		u.applyBoundaryCondition(V,neumann,BCTypes::boundary(side,axis),0.,t,bcParams);
		bcParams.setUseMask(FALSE);
	      }
	    }
	  }
	}
      }



      extrapParams.orderOfExtrapolation=orderOfExtrapolation;  // reset
      u.applyBoundaryCondition(V,generalizedDivergence,outflow,0.,t);
    }
    
    
    if( assignInflowWithVelocityGiven )
    {
      // the inflow value can be time dependent
      if( assignInflowWithVelocityGiven )
	u.applyBoundaryCondition(V,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,
				 Overture::defaultBoundaryConditionParameters(),grid);

      //      u.applyBoundaryCondition(V,extrapolate,inflowWithVelocityGiven,0.,t);
      bool assignGhostWithDirichlet=false;
      
      for( axis=0; axis<<c.numberOfDimensions(); axis++ )
      {
	for( side=0; side<=1; side++ )
	{ // this could fail if there are two inflow sides!
          if( c.boundaryCondition(side,axis)==inflowWithVelocityGiven &&
              parameters.bcType(side,axis,grid)==Parameters::blasiusProfile  )
	  {
	    assignGhostWithDirichlet=true;
	  }
	}
      }
      if( assignGhostWithDirichlet )
      {
        // this only works for Blasius (or parabolic inflow)

        // ** added by Kyle for Blasius inflow ***
	BoundaryConditionParameters gDirParams;
	gDirParams.lineToAssign=1;
	u.applyBoundaryCondition(V,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t, gDirParams,grid);
      }
      else
      {
        u.applyBoundaryCondition(V,generalizedDivergence,inflowWithVelocityGiven,0.,t);
        // add for 4th order dissipation
        if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
	{
	  u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0,inflowWithVelocityGiven,0.,t);
	  if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	    u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1,inflowWithVelocityGiven,0.,t);
	}
      }
    }


    turbulenceModelBoundaryConditionsINS(t,u,parameters,grid,pBoundaryData);

    if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
    {
      assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
      u.applyBoundaryCondition(V,BCTypes::extrapolateInterpolationNeighbours);
    }
    else
    {
      assert( !parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
    }
    
    return 0;
  }
  




  // ********************************************************************
  // ************Non-steady state case***********************************
  // ********************************************************************


  // the dirichletBoundaryCondition is for testing TZ flow.
  if( assignDirichletBoundaryCondition )
  {
    if( debug() & 32  )
    {
      display(u,sPrintF(buff,"insBC: u before assignDirichletBoundaryCondition, grid=%i, t=%e",grid,t),
          parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
    }

    u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,0.,t);
    checkArrayIDs(" insBC: after dirichlet"); 

    if( debug() & 32  )
    {
      display(u,sPrintF(buff,"insBC: u after dirichlet, grid=%i, t=%e",grid,t),
          parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
    }

    u.applyBoundaryCondition(Rt,extrapolate,dirichletBoundaryCondition,0.,t);
    checkArrayIDs(" insBC: after extrapolate (1)"); 
    if( debug() & 32  )
    {
      display(u,sPrintF(buff,"insBC: u after extrapolate, assignDirichletBoundaryCondition, grid=%i, t=%e",grid,t),
          parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
    }


  }
  

  // ** new format *** first do all dirichlet type boundary conditions



  // either use boundaryData or use bcData.
  if( assignInflowWithVelocityGiven )
  {
    // *NOTE* cannot assign on extended range unless we increase the size of the boundaryData
    u.applyBoundaryCondition(V,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,bcParams,grid);
    if( assignTemperature )
    {
      u.applyBoundaryCondition(tc,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,bcParams,grid);
    }
  }
  
  // assigned extended boundaries for 4th order:
  bcParams.extraInTangentialDirections= orderOfAccuracy==2 ? 0 : 2;

//   if( variableBoundaryData )
//   {
//     // boundaryData.display("insBC: variableBoundaryData=TRUE, boundaryData");
//     u.applyBoundaryCondition(V,dirichlet,  inflowWithVelocityGiven,bcData,pBoundaryData,t);
//     // u.applyBoundaryCondition(V,dirichlet,  inflowWithVelocityGiven,boundaryData,t);
//   }
//   else
//   {
//     getTimeDependentBoundaryConditions( t, grid); // ************ fix this *****
//     u.applyBoundaryCondition(V,dirichlet,  inflowWithVelocityGiven,bcData,t,Overture::defaultBoundaryConditionParameters(),grid);
//   }
  if( false )
  {
    display(u,sPrintF(buff,"u after inflowWithVelocityGiven, grid=%i, t=%e",grid,t),parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
  }
    
  if( assignSlipWall )
  {
    if( gridIsMoving )
      u.applyBoundaryCondition(V,normalComponent,slipWall,gridVelocity,t,bcParams);
    else
      u.applyBoundaryCondition(V,normalComponent,slipWall,0.,t,bcParams);
  }

  bool adiabaticNoSlipWall=false;
  const  int nc=parameters.dbase.get<int >("numberOfComponents");
  
  if( assignNoSlipWall )
  {
    if( gridIsMoving )
      u.applyBoundaryCondition(V,dirichlet,noSlipWall,gridVelocity,t,bcParams);
    else
      u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,t,bcParams,grid);

    if( assignTemperature )
    {
      for( int side=0; side<=1; side++ )
      {
	for( int axis=0; axis<c.numberOfDimensions(); axis++ )
	{
	  if( c.boundaryCondition(side,axis)==noSlipWall &&
              parameters.bcData(nc+1,side,axis,grid)!=0. )
	  {
	    adiabaticNoSlipWall=true;
	    printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
                   "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f\n",
		   grid,side,axis,parameters.bcData(nc,side,axis,grid),
		   parameters.bcData(nc+1,side,axis,grid),parameters.bcData(nc+2,side,axis,grid));
	  }
	}
      }
      if( !adiabaticNoSlipWall )
      { // BC: is T=given
	u.applyBoundaryCondition(tc,dirichlet,noSlipWall,bcData,t,bcParams,grid);
      }
      else
      {
        // Some noSlipWall are adiabatic: A Neumann or Mixed BC on T
	for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
	  {
            if( c.boundaryCondition(side,axis)==noSlipWall )
	    {
	      if( parameters.bcData(nc+1,side,axis,grid)==0. ) // coeff of T.n 
	      {
		// Dirichlet
		u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
					 bcParams,grid);
	      }
	      else
	      {
                // Mixed or Neumann -- this case is done below when the ghost points are assigned
	      }
	    }
	  }
	}
      }
    }

    // display(u,"u after dirichlet noSlipWall",parameters.dbase.get<FILE* >("debugFile"));
  }
  if( assignNoSlipInterface )
  {
    // assign velocity on an interface -- do not assign Temperature as this will be done by the interface condition.
    if( gridIsMoving )
      u.applyBoundaryCondition(V,dirichlet,interfaceBoundaryCondition,gridVelocity,t,bcParams);
    else
      u.applyBoundaryCondition(V,dirichlet,interfaceBoundaryCondition,bcData,t,bcParams,grid);
  }
  
  bcParams.extraInTangentialDirections=0; // reset
  


  if( assignOutflow  && orderOfAccuracy==2 )
  {
    // outflow:
    // (1) extrapolate (u,v,w,p)    (default case)
    // (2) set alpha p + beta p.n =   (done in assignPressureRHS)

    const int orderOfExtrapolation=extrapParams.orderOfExtrapolation;
    if( parameters.dbase.get<int >("orderOfExtrapolationForOutflow")>= 0 )
      extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
    if( parameters.dbase.get<int >("checkForInflowAtOutFlow")==2 )
    {
      // expect inflow at an outflow boundary -- use Neumann BC instead of extrapolating
      u.applyBoundaryCondition(V,neumann,outflow,0.,t);
    }
    else
    {
      // default outflow BC's
      u.applyBoundaryCondition(Rt,extrapolate,outflow,0.,t,extrapParams);
    }
    extrapParams.orderOfExtrapolation=orderOfExtrapolation;  // reset
  }
  
  if( assignTractionFree )
  {
    // tractionFree:
    //   ** for now just apply a neumann BC **
    u.applyBoundaryCondition(Rt,neumann,tractionFree,0.,t);  
    // u.applyBoundaryCondition(V,extrapolate,tractionFree,0.,t);  
  }
  
  // **check for local inflow at an outflow boundary**
  // where( inflow ) give u.n=0
  Index I1,I2,I3;
    
  if( !parameters.dbase.get<bool >("twilightZoneFlow") &&  assignOutflow && orderOfAccuracy==2 && parameters.dbase.get<int >("checkForInflowAtOutFlow")==1 )
  {
    // check for inflow at the outflow boundary    
    for( axis=0; axis<c.numberOfDimensions(); axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( c.boundaryCondition(side,axis)==outflow )
	{
	  RealDistributedArray & normal  = c.vertexBoundaryNormal(side,axis);  

	  getGhostIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
	  intArray & mask = bcParams.mask();
	  mask.redim(I1,I2,I3);   // mask lives on ghost line.
	  getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
	  if( isRectangular )
	  {
	    mask = u(I1,I2,I3,uc+axis)*(2*side-1) <0.;
	  }
	  else
	  {
	    if( c.numberOfDimensions()==2 )
	    {
	      mask = (u(I1,I2,I3,uc)*normal(I1,I2,I3,0)+
		      u(I1,I2,I3,vc)*normal(I1,I2,I3,1)) <0; 
	    }
	    else
	    {
	      mask = (u(I1,I2,I3,uc)*normal(I1,I2,I3,0)+
		      u(I1,I2,I3,vc)*normal(I1,I2,I3,1)+
		      u(I1,I2,I3,wc)*normal(I1,I2,I3,2)) <0; 
	    }
	  }
	
	  int count=sum(mask);
	  if( count>0 )
	  {
	    if( debug() & 4 )
	      printf("insBC: number of outflow points that are inflow = %i\n",count);
	    bcParams.setUseMask(TRUE);
	    // u.applyBoundaryCondition(V,neumann,outflow,0.,t,bcParams);
	    u.applyBoundaryCondition(V,neumann,BCTypes::boundary(side,axis),0.,t,bcParams);
	    bcParams.setUseMask(FALSE);
	  }
	}
      }
    }
  }
  
  if( assignSlipWall && orderOfAccuracy==2 )
  {
    // finish slipWall
    // (2) vector symmetry (is this really true on a curved wall??)
    // (3) div(u)=0 (done further below)

    if( false )
      u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t);   
    else
    { // use this 981130
      u.applyBoundaryCondition(V,extrapolate,    slipWall,0.,t);
      if( true )
      {
        // c.update(MappedGrid::THEcenterBoundaryTangent);

	u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0, slipWall,0.,t);
	if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	  u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1, slipWall,0.,t);
      }
      
      if( assignTemperature )
      {
        u.applyBoundaryCondition(tc,neumann,slipWall,0.,t);
      }
      

    }
    
  }
  
  // Before we can apply a generalizedDivergence at a corner we need to first get some values
  // at all ghostpoints  -- therefore we first extrapolate all remaining BC's 

  if( assignInflowWithVelocityGiven && orderOfAccuracy==2 )
  {
    // inflowWithVelocityGiven:
    // (1) set (u,v,w)=
    // (2) extrapolate (u,v,w,p)
    // (3) set div(u)=0.   (done further below)
//  u.applyBoundaryCondition(V,dirichlet,  inflowWithVelocityGiven,inflowWithVelocityGivenData,t);
    u.applyBoundaryCondition(Rt,extrapolate,inflowWithVelocityGiven,0.,t);

  }
  
  if( assignNoSlipWall && orderOfAccuracy==2 )
  {
    // noSlipWall:
    // (1) set (u,v,w)=
    // (2) extrapolate (u,v,w,p)
    // (3) set div(u)=0. (done further below)

    u.applyBoundaryCondition(Rt,extrapolate,noSlipWall,0.,t);

    // extrapParams.orderOfExtrapolation=4; // *****  why??
    // u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t,extrapParams);

    if( adiabaticNoSlipWall )
    {
      // Some noSlipWall are adiabatic: A Neumann or Mixed BC on T
      bcParams.a.redim(3);
      bcParams.a=0.;
      for( int side=0; side<=1; side++ )
      {
	for( int axis=0; axis<c.numberOfDimensions(); axis++ )
	{
	  if( c.boundaryCondition(side,axis)==noSlipWall )
	  {
	    if( parameters.bcData(nc+1,side,axis,grid)==0. ) // coeff of T.n 
	    {
	      // Dirichlet BC on T -- extrap the ghost points (this is done above)
	      // u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
	    }
	    else
	    {
	      // Mixed BC or Neumann
	      real a0=parameters.bcData(nc  ,side,axis,grid);
	      real a1=parameters.bcData(nc+1,side,axis,grid);
	      if( a0==0. && a1==1. )
	      {
		printf("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
			 grid,side,axis);

//                 real b0=parameters.bcData(nc+2,side,axis,grid);
// 		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),b0,t); // b0 ignored??

		bcParams.a(0)=a0;
		bcParams.a(1)=a1;
		bcParams.a(2)=parameters.bcData(nc+2,side,axis,grid);  // this is not used -- this does not work
		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
					 bcParams,grid);

	      }
	      else
	      {
		printf("++++insBC:noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
		       "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f\n",
			 grid,side,axis,a0,a1,parameters.bcData(nc+2,side,axis,grid));

		bcParams.a(0)=a0;
		bcParams.a(1)=a1;
		bcParams.a(2)=parameters.bcData(nc+2,side,axis,grid);
		u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
					 bcParams,grid);
	      }
		
	    }
	    
	  }
	}
      }
    }  // end if adiabatic

  }


  if( assignNoSlipInterface && orderOfAccuracy==2 )
  {
    u.applyBoundaryCondition(V,extrapolate,interfaceBoundaryCondition,0.,t);
  }
  
  if( parameters.isAxisymmetric() && assignAxisymmetric )
  {
    // cylindrically symmetric BC: v=v.yy=u.y=0
    u.applyBoundaryCondition(vc,dirichlet,axisymmetric,0.,t);
    u.applyBoundaryCondition(uc,neumann,axisymmetric,0.,t);
    extrapParams.lineToAssign=1;
    extrapParams.orderOfExtrapolation=2;

    // Extrap to higher order here needed for some reason -- this is fixed below with the BC u.x+2*u.y=0
    extrapParams.orderOfExtrapolation=4; // **** test this ****

    u.applyBoundaryCondition(vc,extrapolate,axisymmetric,0.,t,extrapParams);
  
  }
  if( assignSymmetry )
  {
    // symmetry BC:
    u.applyBoundaryCondition(V,vectorSymmetry,symmetry,0.,t);   
    if( assignTemperature )
    {
      u.applyBoundaryCondition(tc,BCTypes::evenSymmetry,symmetry,0.,t);
    }
  }
  
  // *wdh* 000929 : also need to update periodic boundaries here
  u.periodicUpdate();


  checkArrayIDs(" insBC: before generalizedDivergence"); 

  if( orderOfAccuracy==2 )
  {
    if( assignInflowWithVelocityGiven )
    {
      // *note* that when this condition is applied on adjacent boundaries the ghost points next to corners
      //  are assigned in a certain order and some symmetry will be lost.
      u.applyBoundaryCondition(V,generalizedDivergence,inflowWithVelocityGiven,0.,t);
    }
  
    if( !parameters.isAxisymmetric() )
    {
      if( assignNoSlipWall )
	u.applyBoundaryCondition(V,generalizedDivergence,noSlipWall,0.,t);

      if( assignNoSlipInterface )
        u.applyBoundaryCondition(V,generalizedDivergence,interfaceBoundaryCondition,0.,t);
    }
    else
    {
      // div(u) = u.x + v.y + v/y = 0
      //   For y>0 and noSlipWall (u=v=0) --> u.x + v.y = 0

      if( assignNoSlipWall )
	u.applyBoundaryCondition(V,generalizedDivergence,noSlipWall,0.,t);
      if( assignNoSlipInterface )
        u.applyBoundaryCondition(V,generalizedDivergence,interfaceBoundaryCondition,0.,t);

      if( assignAxisymmetric )
      {
	// BC u.x + v.y + v/y =0  at y=0: u.x + 2 v.y =0
	bcParams.a.redim(3);
	bcParams.a(0)=1.;
	bcParams.a(1)=2.;
	bcParams.a(2)=0.;
	u.applyBoundaryCondition(V,generalizedDivergence,axisymmetric,0.,t,bcParams);
      }
    }
  }
  
  // display(u,sPrintF(buff,"u after generalized divergence insBC grid=%i",grid),parameters.dbase.get<FILE* >("debugFile"));

  if( applyDivergenceBoundaryCondition && assignSlipWall && orderOfAccuracy==2 )
    u.applyBoundaryCondition(V,generalizedDivergence,slipWall,0.,t); 
  
  if( assignInflowWithPressureAndTangentialVelocityGiven && orderOfAccuracy==2 )
  {
    //  inflowWithPressureAndTangentialVelocityGiven
    //     give tangetial velocity = 0 
    //     extrapolate (u,v,w)
    //     set div(u)=0
    u.applyBoundaryCondition(V,tangentialComponent,inflowWithPressureAndTangentialVelocityGiven,0.,t);
    if( assignTemperature )
    {
      u.applyBoundaryCondition(tc,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,bcParams,grid);
    }
    u.applyBoundaryCondition(Rt,extrapolate,inflowWithPressureAndTangentialVelocityGiven,0.,t);

    u.applyBoundaryCondition(V,generalizedDivergence,inflowWithPressureAndTangentialVelocityGiven,0.,t);
  }
  
  // **** if we do this we probably don't have to check for inflow points at outflow.
  // *** turn off for stretched c-grid  
  if( applyDivergenceBoundaryCondition && orderOfAccuracy==2 )
  {
    if( assignOutflow )
    {
      
      if( !parameters.isAxisymmetric() )
      {
	u.applyBoundaryCondition(V,generalizedDivergence,outflow,0.,t); // ****wdh***** 990827
      }
      else
      {
	// div(u) = u.x + v.y + v/y = 0

        // *** For now do nothing in this case ***
//  	bcParams.a.redim(3);
//  	bcParams.a(0)=1.;
//  	bcParams.a(1)=2.;
//  	bcParams.a(2)=0.;
//  	u.applyBoundaryCondition(V,generalizedDivergence,axisymmetric,0.,t,bcParams);

      }
    }

    if( assignTractionFree )
      u.applyBoundaryCondition(V,generalizedDivergence,tractionFree,0.,t); 
  }
  
  checkArrayIDs(" insBC: after generalizedDivergence"); 


  // Boundary conditions for the passive scalar.
  if( parameters.dbase.get<bool >("advectPassiveScalar") )
  {
    if( assignInflowWithVelocityGiven )
    {
      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),extrapolate,inflowWithVelocityGiven,0.,t);
    }
    if( assignOutflow )
      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),extrapolate,outflow,0.,t);

    if( assignNoSlipWall )
      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),neumann,noSlipWall,bcData,pBoundaryData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
    if( assignNoSlipInterface )
      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),neumann,interfaceBoundaryCondition,bcData,pBoundaryData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
    if( assignSlipWall )
      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),neumann,slipWall,bcData,pBoundaryData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);

    // ** u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),extrapolate,allBoundaries,0.,t);
  }


  // extrapolate the neighbours of interpolation points -- these values are used
  // by the fourth-order artificial viscosity 

  if( parameters.dbase.get<int >("orderOfAccuracy")==2 )
  {
    const int discretizationWidth = c.discretizationWidth(0);
    // if( discretizationWidth!=3 ) printf(" INSBC: discretizationWidth=%i\n",discretizationWidth);
    
    if(  discretizationWidth<5 &&
         parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") && (parameters.dbase.get<real >("ad41")!=0. || parameters.dbase.get<real >("ad42")!=0.)  )
    { // double check
      assert( numberOfGhostPointsNeeded>=2 );
    }
    
    if( discretizationWidth<5 && 
        numberOfGhostPointsNeeded>=2 )
    {
      extrapParams.ghostLineToAssign=2;
      extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfAccuracy")+1; // 3;
      u.applyBoundaryCondition(Rt,extrapolate,allBoundaries,0.,t,extrapParams);

      if( parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion") )
        extrapParams.orderOfExtrapolation=3; // parameters.dbase.get<int >("orderOfAccuracy"); // 3;
      else
        extrapParams.orderOfExtrapolation=3;
      
      assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
      u.applyBoundaryCondition(Rt,BCTypes::extrapolateInterpolationNeighbours,allBoundaries,0.,t,extrapParams);
    }
    else if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::rKutta )
    {
      // semi-implicit method needs du/dt at interpolation points.
      assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
      u.applyBoundaryCondition(Rt,BCTypes::extrapolateInterpolationNeighbours);
    }
    else
    {
      assert( !parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
    }
    
  }
  
  if( orderOfAccuracy==4 )
  {
    // apply BC's for fourth-order accuracy

    if( false )
    {
      bcParams.lineToAssign=1;
      u.applyBoundaryCondition(V,dirichlet,allBoundaries,0.,t,bcParams);
      bcParams.lineToAssign=2;
      u.applyBoundaryCondition(V,dirichlet,allBoundaries,0.,t,bcParams);
      bcParams.lineToAssign=1;
    }
    else
    {
      // we need initial guesses for all ghost points -- this is done elsewhere --

//         extrapParams.ghostLineToAssign=1;
//         extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfAccuracy")+1;
//         u.applyBoundaryCondition(V,extrapolate,allBoundaries,0.,t,extrapParams);

//         extrapParams.ghostLineToAssign=2;
//         extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfAccuracy")+1; // 3;
//         u.applyBoundaryCondition(V,extrapolate,allBoundaries,0.,t,extrapParams);


      // ************* fix this ***************************************************************
//      c.update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex |
//	       MappedGrid::THEinverseVertexDerivative | MappedGrid::THEvertexBoundaryNormal );

// ----
      if( false )
      {
        // **** for debugging ***** set p at corners

	const realArray & vertex = c.vertex();
	OGFunction & e = *parameters.dbase.get<OGFunction* >("exactSolution");
	int side,axis;
	for( axis=0; axis<parameters.dbase.get<int >("numberOfDimensions"); axis++ )
	{
	  for( side=0; side<=1; side++ )
	  {
	    if( c.boundaryCondition(side,axis)>0 )
	    {
	      int iv[3]={0,0,0};// 
	      int is[3]={0,0,0};// 
	      int axisp1 = (axis+1)%parameters.dbase.get<int >("numberOfDimensions");
	      for( int side2=0; side2<=1; side2++ )
	      {
		if( c.boundaryCondition(side2,axisp1)>0 )
		{
		  // *** corner points
		  iv[axis]=c.gridIndexRange()(side,axis);
		  iv[axisp1]=c.gridIndexRange()(side2,axisp1);
	      
		  is[axis]  =1-2*side;
		  is[axisp1]=1-2*side2;
		
                  // set p at the corners
		  for( int ks2=-2; ks2<=2; ks2++ )
		  {
  		    for( int ks1=-2; ks1<=2; ks1++ )
  		    {
  		      int i1=iv[0]-ks1*is[0], i2=iv[1]-ks2*is[1], i3=iv[2]-is[2];
		      real x0=vertex(i1,i2,i3,0);
		      real y0=vertex(i1,i2,i3,1);
		      real z0=0.;
		      u(i1,i2,i3,parameters.dbase.get<int >("pc"))=e(x0,y0,z0,parameters.dbase.get<int >("pc"),t);
//  		      for( int c=uc; c<uc+parameters.dbase.get<int >("numberOfDimensions"); c++ )
//  			u(i1,i2,i3,c)=e(x0,y0,z0,c,t);
		  
		    }
		  }
                  iv[0]=iv[1]=0;

		}
	      }
	    
	    }
	  }
	}
      }
// -------------

      if( assignSlipWall )
      {
        // On a slip wall use vector symmetry on both ghost lines
        bcParams.ghostLineToAssign=1;
	u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t);   
	if( assignTemperature )
	  u.applyBoundaryCondition(tc,BCTypes::evenSymmetry, slipWall,0.,t); 
        bcParams.ghostLineToAssign=2;
	u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,bcParams);   
        bcParams.ghostLineToAssign=1;
	if( assignTemperature )
	  u.applyBoundaryCondition(tc,BCTypes::evenSymmetry,slipWall,0.,t,bcParams);
      }
      

      IntegerArray ipar(20);  // fix this 
      RealArray rpar(20);
      bool useWhereMask=false;
      
      ipar(0) = parameters.dbase.get<int >("debug");
      ipar(1) = parameters.dbase.get<int >("pc");
      ipar(2) = parameters.dbase.get<int >("uc");
      ipar(3) = parameters.dbase.get<int >("vc");
      ipar(4) = parameters.dbase.get<int >("wc");
      ipar(5) = grid;
      ipar(6) = parameters.dbase.get<int >("orderOfAccuracy");
      ipar(7) = parameters.gridIsMoving(grid);
      ipar(8) = useWhereMask;
      ipar(9) = parameters.getGridIsImplicit(grid);
      ipar(10)= parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
      ipar(11)= parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
      ipar(12)= parameters.isAxisymmetric();
      ipar(13)= parameters.dbase.get<bool >("twilightZoneFlow");

      real dx,dy,dz;
      rpar(0) = t;
      rpar(1) = dx;
      rpar(2) = dy;
      rpar(3) = dz;
      rpar(4) = parameters.dbase.get<real >("nu");
      rpar(5) = getSignForJacobian(c); // c.mapping().getSignForJacobian();
      rpar(6) = parameters.dbase.get<real >("anl");

      // printf(" applyFourthOrderBoundaryConditions at t=%e\n",t);
      assert( parameters.dbase.get<Insbc4WorkSpace* >("bc4workSpacePointer")!=NULL );
      
      if( assignTemperature )
      {
	Overture::abort("insBC:ERROR: 4th-order BC's with Temperature not finished");
      }
      

      applyFourthOrderBoundaryConditions( u,t,grid,*parameters.dbase.get<Insbc4WorkSpace* >("bc4workSpacePointer"),ipar,rpar,
                      *parameters.dbase.get<OGFunction* >("exactSolution"),parameters );

      // **** assign symmetry conditions on slip walls at corner points too
      //     *** for now reassign all points on the extended region
      if( assignSlipWall )
      {
        // On a slip wall use vector symmetry on both ghost lines
        bcParams.ghostLineToAssign=1;
        bcParams.extraInTangentialDirections=2; // include 2 ghost points
	u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,bcParams);   
	if( assignTemperature )
	  u.applyBoundaryCondition(tc,BCTypes::evenSymmetry, slipWall,0.,t,bcParams); 
        bcParams.ghostLineToAssign=2;
	u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,bcParams);   
	if( assignTemperature )
	  u.applyBoundaryCondition(tc,BCTypes::evenSymmetry, slipWall,0.,t,bcParams); 
        bcParams.ghostLineToAssign=1;
	bcParams.extraInTangentialDirections=0; // reset
      }


      // **** for now assign BC along extended boundaries ****
      if( false )
      {
	OGFunction & e = *parameters.dbase.get<OGFunction* >("exactSolution");
	int side,axis;
	for( axis=0; axis<parameters.dbase.get<int >("numberOfDimensions"); axis++ )
	{
	  for( side=0; side<=1; side++ )
	  {
	    if( c.boundaryCondition(side,axis)>0 )
	    {
	      getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3,2);  
	      u(I1,I2,I3,V)=e(c,I1,I2,I3,V,t);
//	      u(I1,I2,I3,C)=e(c,I1,I2,I3,C,t);
	    
	    }
	  }
	}
      }


    }
    
    
  }
  

//   if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
//   {
//     // here are some fake k-epsilon equations
//     Range KE(parameters.dbase.get<int >("kc"),parameters.dbase.get<int >("epsc"));
//     u.applyBoundaryCondition(KE,dirichlet,allBoundaries,0.,t);
//     u.applyBoundaryCondition(KE,extrapolate,allBoundaries,0.,t);
//   }

  // apply turbulence model boundary conditions
  turbulenceModelBoundaryConditionsINS(t,u,parameters,grid,pBoundaryData);

  // display(u,sPrintF(buff,"u at end of insBC grid=%i",grid),parameters.dbase.get<FILE* >("debugFile"));

  // update corners and periodic edges
  // *** not here  u.finishBoundaryConditions();

  checkArrayIDs(" insBC: end"); 


  return 0;
}




