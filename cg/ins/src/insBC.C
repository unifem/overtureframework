// This file automatically generated from insBC.bC with bpp.
#include "Cgins.h"
#include "Parameters.h"
#include "turbulenceModels.h"
#include "Insbc4WorkSpace.h"
#include "App.h"
#include "ParallelUtility.h"
#include "DeformingBodyMotion.h"
#include "BeamModel.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )

// This next include file defines the setTemperatureBC macro
// ----------------------------------------------------------------------------
// Macro: Apply BC's on the Temperature 
// 
//   There are 3 cases: 
//      (1) apply a dirichlet BC                       (OPTION=dirichlet)
//      (2) extrapolate ghost pts on dirichlet BC's     (OPTION=extrapolateGhost)
//      (3) apply a mixed BC                           (OPTION=mixed)
// 
// Macro args:
// 
// tc : component to assign
// NAME : name of of the calling function (for comments)
// BCNAME : noSlipWall, inflowWithVelocityGiven etc. 
// OPTION: dirichlet, mixed, extrapolateGhost
// ----------------------------------------------------------------------------


//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)


// ==========================================================================
// *****************************************************
// ***********STEADY STATE BC's ************************
// *****************************************************
// ==========================================================================

static int numberOfOutflowPointsAtInflowMessages=0;

//\begin{>>MappedGridSolverInclude.tex}{\subsection{applyBoundaryConditionsINS}} 
int Cgins::
applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
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
// /NOTE on the bcData array: 
//  Boundary condition parameter values are stored in the array bcData. Let nc=numberOfComponents,
//  then the values 
//
//          bcData(i,side,axis,grid)  : i=0,1,...,nc-1 
//
//  would normally represent the RHS values for dirichlet BC's on component i, such as
//            u(i1,i2,i3,i) = bcData(i,side,axis,grid) 
// 
//  For a Mixed-derivative boundary condition, the parameters (a0,a1,a2) in the mixed BC:
//               a1*u(i1,i2,i3,i) + a2*u(i1,i2,i3,i)_n = a0
//  are stored in
//          a_j = bcData(i+nc*(j),side,axis,grid),  j=0,1,2
// 
//  Thus bcData(i,side,axis,grid) still holds the RHS value for the mixed-derivative condition
// 
//\end{MappedGridSolverInclude.tex}  
//=========================================================================================
{
    if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return 0;

    real time0=getCPU();

    if( debug() & 4 )
    {
        printF(">>>>> Cgins::applyBoundaryConditions  t=%9.3e <<<<<<<\n",t);
        fPrintF(parameters.dbase.get<FILE* >("debugFile"),">>>>> Cgins::applyBoundaryConditions t=%9.3e <<<<<<<\n",t);
    }
    
//   printF(" Cgins::applyBoundaryConditions **START**\n");
//   cg[0].displayComputedGeometry(); // ************************

    checkArrayIDs(" insBC: start"); 

    MappedGrid & mg = *u.getMappedGrid();
    const int numberOfDimensions = mg.numberOfDimensions();
    
    const bool isRectangular = mg.isRectangular();
    
  // *** turn off for stretched c-grid at outflow 
    bool applyDivergenceBoundaryCondition=true; // false; // true;
    bool applyDivergenceBoundaryConditionAtOutflow=true;
    
//   MappedGrid & mg = *u.getMappedGrid();
//   printf("applyBoundaryConditionsINS: grid=%i variableBoundaryData=%i\n",grid,variableBoundaryData);
//   display(mg.boundaryCondition(),sPrintF(buff,"grid=%i applyBoundaryConditionsINS: mg.boundaryCondition()",grid));
      
    const bool gridIsMoving = parameters.gridIsMoving(grid);

    const int uc = parameters.dbase.get<int >("uc");
    const int vc = parameters.dbase.get<int >("vc");
    const int wc = parameters.dbase.get<int >("wc");
    const int tc = parameters.dbase.get<int >("tc");
    const int & nc = parameters.dbase.get<int >("nc");
    const int orderOfAccuracy=min(4,parameters.dbase.get<int >("orderOfAccuracy"));
  //kkc 100216 !!! turn this assertion off for testing the compact schemes  assert( orderOfAccuracy==2 || orderOfAccuracy==4 );
    
    const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");

//   if( true ) // *************** TEMP *******
//   {
//     int side=0, axis=0;
//     RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
//     ::display(bd,sPrintF("insBC: bd: grid=%i side=%i axis=%i",grid,side,axis),
//               parameters.dbase.get<FILE* >("pDebugFile"),"%5.2f ");
//   }
    

    typedef int BoundaryCondition;
    
    const BoundaryCondition & noSlipWall = Parameters::noSlipWall;
    const BoundaryCondition & slipWall   = Parameters::slipWall;
    const BoundaryCondition & inflowWithVelocityGiven = InsParameters::inflowWithVelocityGiven;
    const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
                              = InsParameters::inflowWithPressureAndTangentialVelocityGiven;
    const BoundaryCondition & outflow = InsParameters::outflow;
    const BoundaryCondition & tractionFree = InsParameters::tractionFree;
    const BoundaryCondition & symmetry = Parameters::symmetry;
    const BoundaryCondition & dirichletBoundaryCondition = Parameters::dirichletBoundaryCondition;
    const BoundaryCondition & neumannBoundaryCondition = Parameters::neumannBoundaryCondition;
    const BoundaryCondition & axisymmetric = Parameters::axisymmetric;
    const BoundaryCondition & freeSurfaceBoundaryCondition = Parameters::freeSurfaceBoundaryCondition;
    
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


  // Here is the array that defines the domain interfaces, interfaceType(side,axis,grid) 
    const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
    const BoundaryCondition & interfaceBoundaryCondition = Parameters::interfaceBoundaryCondition;


    bool assignSlipWall=false;
    bool assignNoSlipWall=false;
    bool assignInflowWithVelocityGiven=false;
    bool assignOutflow=false;
    bool assignTractionFree=false;
    bool assignAxisymmetric=false;
    bool assignSymmetry=false;
    bool assignDirichletBoundaryCondition=false;
    bool assignInflowWithPressureAndTangentialVelocityGiven=false;
    bool assignNeumannBoundaryCondition=false;
    bool assignFreeSurfaceBoundaryCondition=false;

    bool assignInflowOutflow=false;

    int side,axis;
    for( axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        for( side=0; side<=1; side++ )
        {
            int bc=mg.boundaryCondition(side,axis);
            switch (bc)
            {
            case 0 : break;
            case -1: break;
            case Parameters::slipWall:                   assignSlipWall=true; break;
            case Parameters::noSlipWall :                assignNoSlipWall=true; break;
            case InsParameters::inflowWithVelocityGiven: assignInflowWithVelocityGiven=true; break;
            case InsParameters::outflow:                 assignOutflow=true; break;
            case InsParameters::tractionFree:            assignTractionFree=true; break;
            case Parameters::axisymmetric:               assignAxisymmetric=true; break;
            case Parameters::symmetry :                  assignSymmetry=true; break;
            case Parameters::dirichletBoundaryCondition: assignDirichletBoundaryCondition=true; break;
            case Parameters::neumannBoundaryCondition:   assignNeumannBoundaryCondition=true; break;
            case InsParameters::inflowWithPressureAndTangentialVelocityGiven :
      	assignInflowWithPressureAndTangentialVelocityGiven=true; break;
            case InsParameters::freeSurfaceBoundaryCondition: assignFreeSurfaceBoundaryCondition=true; break;
            case InsParameters::inflowOutflow:           assignInflowOutflow=true; break;
            case InsParameters::penaltyBoundaryCondition: break;
            default: 
                printF("insBC:ERROR: unknown boundary condition =%i on grid %i, side=%i, axis=%i\n",bc,grid,side,axis);
                OV_ABORT("error");
            }
        }
    }

    const int numberOfGhostPointsNeeded = parameters.numberOfGhostPointsNeeded();

  // **************************************************************************
  //  apply boundary conditions in order of increasing priority (so corners
  //    take the values from the bc that is applied last)
  // **************************************************************************

    const int numberOfComponents =parameters.dbase.get<int >("numberOfComponents");

    Range C(0,numberOfComponents-1);  // ***** is this correct ******
    Range V = Range(uc,uc+numberOfDimensions-1);
    const Range & Rt = parameters.dbase.get<Range >("Rt"); // time dependent parameters (u,v,w,[T]). 
    
    BoundaryConditionParameters extrapParams;
    BoundaryConditionParameters bcParams;

  // BoundaryData : this class holds info about the boundary data, such as the RHS to BC's and
  //                variable coefficients.
  //  typedef RealArray *BoundaryDataArray[2][3];
  //  pBoundaryData : RealArray *pBoundaryData[2][3];
  //  I think bd.boundaryData == pBoundaryData
    BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid); // this will create the BDA if it is not there
    BoundaryData & bd = parameters.dbase.get<std::vector<BoundaryData> >("boundaryData")[grid];
    assert( bd.boundaryData == pBoundaryData );
    
    const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
    
    Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");

    const bool assignTemperature = pdeModel==InsParameters::BoussinesqModel ||
                                                                  pdeModel==InsParameters::viscoPlasticModel;
    

    const Parameters::TimeSteppingMethod & timeSteppingMethod = 
                                            parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
    const Parameters::ImplicitMethod &method = parameters.dbase.get<Parameters::ImplicitMethod>("implicitMethod");

    Range twoPhaseFlowComponents;
    if( pdeModel==InsParameters::twoPhaseFlowModel )
    {
        assert( nc==(tc+1) ) ;
      twoPhaseFlowComponents=Range(tc,nc);
    }

  // -- evaluate the known solution ----
    const Parameters::KnownSolutionsEnum & knownSolution = 
                        parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");
    
    realArray *uKnownPointer=NULL;
    if( knownSolution!=InsParameters::noKnownSolution )
    {
        int extra=2;
        Index I1,I2,I3;
        getIndex(mg.gridIndexRange(),I1,I2,I3,extra);  // **************** fix this -- only evaluate near boundaries --

        if( false )// ************** TEMP ********************
        {
            realArray & x = mg.vertex();
            int i1=0, i2=0, i3=0;
            printF("--INSBC-- before getKnown: grid=%i: t=%12.5e, point (i1=0,i2=0) (x,y)=(%20.12e,%20.12e)\n",
           	     grid,t,x(i1,i2,i3,0),x(i1,i2,i3,1));
        }

        uKnownPointer = &parameters.getKnownSolution( t,grid,I1,I2,I3 );
    }
    realArray & uKnown = uKnownPointer!=NULL ? *uKnownPointer : u;
    OV_GET_SERIAL_ARRAY(real,uKnown,uKnownLocal);
    
  // #ifdef USE_PPP
  //   const realSerialArray & uKnownLocal = uKnown.getLocalArray();
  // #else
  //   const realSerialArray & uKnownLocal = uKnown;
  // #endif  


  // =======================================================================================================
  //  Project the velocity on the interface for FSI problems
  // =======================================================================================================
    const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
    const bool & projectAddedMassVelocity = parameters.dbase.get<bool>("projectAddedMassVelocity");
    const bool & projectNormalComponentOfAddedMassVelocity =
                              parameters.dbase.get<bool>("projectNormalComponentOfAddedMassVelocity");
    const int initialConditionsAreBeingProjected = parameters.dbase.get<int>("initialConditionsAreBeingProjected");
    if( useAddedMassAlgorithm && projectAddedMassVelocity && parameters.gridIsMoving(grid)
              && !initialConditionsAreBeingProjected 
              && t!=0.  // ****************************************** TEST ********************
        )
    {

    // // TRY THIS: set no-slip walls before velocity projection
    // if( assignNoSlipWall )
    // {
    //   if( !gridIsMoving )
    //   {
    // 	u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,pBoundaryData,t,bcParams,grid);
    //   }
    // }

        projectInterfaceVelocity( t,u,gridVelocity,grid,dt );
            
    } // end if useAddedMass 
  // =======================================================================================================
  // =======================================================================================================





    const real & tInitial = parameters.dbase.get<real >("tInitial"); // *wdh* 090819
    if( timeSteppingMethod==Parameters::steadyStateRungeKutta && 
            t>tInitial )  // apply all boundary conditions at t=0
    {
    // *****************************************************
    // ***********STEADY STATE BC's ************************
    // *****************************************************

      // we only need to apply a limited number of BC's for the steady state solver since most
      // have already been done.
            const Parameters::TimeSteppingMethod & timeSteppingMethod = 
                parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
            const Parameters::ImplicitMethod &method = parameters.dbase.get<Parameters::ImplicitMethod>("implicitMethod");
            if( assignSlipWall )
            {
        // on a slip wall we need to extrapolate points that lie outside interpolation pts on the bndry.
                u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0, slipWall,0.,t);
                if( numberOfDimensions==3 )
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
                    u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t);
                    u.applyBoundaryCondition(V,BCTypes::generalizedDivergence,noSlipWall,0.,t); 	
                }
        // NOTE: dirichlet or neumann BC's for T are already done in the lineSolver
            }
            if( assignDirichletBoundaryCondition ) // *wdh* added 070605
            {
                u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,0.,t,bcParams);
                u.applyBoundaryCondition(C,extrapolate,dirichletBoundaryCondition,0.,t,bcParams);
            }
            if( assignNeumannBoundaryCondition ) // kkc added 100812
            {
                u.applyBoundaryCondition(V,neumann,neumannBoundaryCondition,0.,t,bcParams);
            }
            if( assignOutflow && 
                    parameters.dbase.get<int>("outflowOption")==0 && 
                    parameters.dbase.get<int >("checkForInflowAtOutFlow")==1 )
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
              	for( axis=0; axis<mg.numberOfDimensions(); axis++ )
              	{
                	  for( side=Start; side<=End; side++ )
                	  {
                  	    if( mg.boundaryCondition(side,axis)==outflow )
                  	    {
                    	      RealDistributedArray & normal  = mg.vertexBoundaryNormal(side,axis);  
                    	      getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                    	      intArray & mask = bcParams.mask();
                    	      mask.redim(I1,I2,I3);   // mask lives on ghost line.
                    	      getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                    	      if( isRectangular )
                    	      {
                    		mask = u(I1,I2,I3,uc+axis)*(2*side-1) <0.;
                    	      }
                    	      else
                    	      {
                    		if( mg.numberOfDimensions()==2 )
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
                u.applyBoundaryCondition(V,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,
                                 			       Overture::defaultBoundaryConditionParameters(),grid);
                bool assignGhostWithDirichlet=false;
                for( axis=0; axis<mg.numberOfDimensions(); axis++ )
                {
              	for( side=0; side<=1; side++ )
              	{ // this could fail if there are two inflow sides!
                        if( mg.boundaryCondition(side,axis)==inflowWithVelocityGiven &&
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
                    u.applyBoundaryCondition(V,extrapolate,inflowWithVelocityGiven,0.,t);
                    u.applyBoundaryCondition(V,generalizedDivergence,inflowWithVelocityGiven,0.,t);
          // add for 4th order dissipation
                    if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
              	{
                	  u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0,inflowWithVelocityGiven,0.,t);
                	  if( numberOfDimensions==3 )
                  	    u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1,inflowWithVelocityGiven,0.,t);
              	}
                }
        // NOTE: dirichlet or neumann BC's for T are already done in the lineSolver
            }
            if( assignNoSlipWall )
            {  // Extrapolate ghost points on dirichlet BC's for T 
        // NOTE: dirichlet or neumann BC's for T are already done in the lineSolver
                  if( assignTemperature )
                  {
                      FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
                      FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
                      ForBoundary(side,axis)
                      {
                          if( mg.boundaryCondition(side,axis)==noSlipWall )
                          {
                              if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                              { // This is an interface between domains
                 // for now we only know about interfaces at no-slip walls: 
                                  assert( mg.boundaryCondition(side,axis)==noSlipWall );
        	 // what about BC's applied at t=0 before the boundary data is set ??
        	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
        	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
        	 // to use the boundary data instead.
                                  #ifdef USE_PPP
                                      realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                                  #else
                                      const realSerialArray & uLocal = u;
                                  #endif
                       	 Index Ib1,Ib2,Ib3;
                       	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                       	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
                       	 if( debug() & 4 )
                       	 {
                         	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                              		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                         	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                         			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                         	   if( pBoundaryData[side][axis]==NULL )
                         	   {
                           	     if( !ok )
                                              fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                          else
                                              fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                         	   }
                         	   else
                         	   {
        	     // RealArray & bd = *pBoundaryData[side][axis];
        	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                         	   }
                       	 }
                                  assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
                       	 if( ok && pBoundaryData[side][axis]==NULL )
                       	 {
                   // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
                   // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
                   // based on the current solution
                                      RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                                      bd=0.;
                                      #ifdef USE_PPP
                           	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                                      #else
                           	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                                      #endif
                         	   real a0=mixedCoeff(tc,side,axis,grid);
                         	   real a1=mixedNormalCoeff(tc,side,axis,grid);
                   // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                                      MappedGridOperators & op = *(u.getOperators());
                                      Range N(tc,tc);
                                      RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                         	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                         	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                                      if( mg.numberOfDimensions()==2 )
                         	   {
                                          bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                            				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                         	   }
                         	   else
                         	   {
                                          RealArray uz(Ib1,Ib2,Ib3,N);
                           	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                           	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                            				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                            				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                         	   }
                                      const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                         	   if( false && twilightZoneFlow ) //  *******************************************************
                         	   {
                                          fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                           	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                           	     realArray & x= mg.center();
                #ifdef USE_PPP
                           	     realSerialArray xLocal; 
                           	     if( !rectangular || twilightZoneFlow ) 
                             	       getLocalArrayWithGhostBoundaries(x,xLocal);
                #else
                           	     const realSerialArray & xLocal = x;
                #endif
                           	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                           	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                           	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                           	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                           	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                           	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                           	     if( mg.numberOfDimensions()==2 )
                           	     {
                             	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                              				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                           	     }
                           	     else
                           	     {
                             	       realSerialArray uez(Ib1,Ib2,Ib3);
                             	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                             	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                              				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                              				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                           	     }
                         	   } // *******************************************
                       	 }
                       	 if( pBoundaryData[side][axis]!=NULL )
                         	   u.getOperators()->setTwilightZoneFlow( false );
                       	 else
                       	 {
                                      if( t>0. || debug() & 4 )
                         	   {
                                          if( ok )
                                     	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                         	   }
                       	 }
                              }
                              if( debug() & 4 )
                       	 printF("++++insBC: noSlipWall: (grid,side,axis)=(%i,%i,%i) : "
                            		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                            		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                            		grid,side,axis, 
                            		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                            		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                            		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                         	   );
        //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
        //        {
        // 	 mixedBoundaryConditionOnTemperature=true;
        // 	 if( debug() & 4 )
        // 	   printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
        // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
        //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
        // 		  grid,side,axis, 
        //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
        //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
        //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
        // 	     );
        //        }
                              if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                              {
        	 // Dirichlet
                       	 u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
        // 	      u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
        // 				       bcParams,grid);
                              }
                              else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                              {
                 // -- Variable Coefficient Temperature (const coeff.) BC --- 
                       	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                            		grid,side,axis);
                 // BC is : a0(x)*T + an(x)*T.n = g 
                 //  a0 = varCoeff(i1,i2,i3,0)
                 //  an = varCoeff(i1,i2,i3,1)
                       	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                                BoundaryData::variableCoefficientTemperatureBC,side,axis );
                                  bcParams.setVariableCoefficientsArray(&varCoeff);
                       	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                          				  bcParams,grid);
                                  bcParams.setVariableCoefficientsArray(NULL);  // reset 
                              } 
                              else
                              {
        	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                              }
                              if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                              { // reset TZ
                       	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                              }
                          } // end if bc = noSlipWall
                      } // end for boundary
           // ************ try this ********* 080909
           // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
                  } // end if assignTemperature
            }
            if( assignInflowWithVelocityGiven )
            {
        // Extrapolate ghost points on dirichlet BC's for T 
        // NOTE: dirichlet or neumann BC's for T are already done in the lineSolver
                  if( assignTemperature )
                  {
                      FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
                      FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
                      ForBoundary(side,axis)
                      {
                          if( mg.boundaryCondition(side,axis)==inflowWithVelocityGiven )
                          {
                              if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                              { // This is an interface between domains
                 // for now we only know about interfaces at no-slip walls: 
                                  assert( mg.boundaryCondition(side,axis)==noSlipWall );
        	 // what about BC's applied at t=0 before the boundary data is set ??
        	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
        	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
        	 // to use the boundary data instead.
                                  #ifdef USE_PPP
                                      realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                                  #else
                                      const realSerialArray & uLocal = u;
                                  #endif
                       	 Index Ib1,Ib2,Ib3;
                       	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                       	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
                       	 if( debug() & 4 )
                       	 {
                         	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                              		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                         	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                         			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                         	   if( pBoundaryData[side][axis]==NULL )
                         	   {
                           	     if( !ok )
                                              fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                          else
                                              fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                         	   }
                         	   else
                         	   {
        	     // RealArray & bd = *pBoundaryData[side][axis];
        	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                         	   }
                       	 }
                                  assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
                       	 if( ok && pBoundaryData[side][axis]==NULL )
                       	 {
                   // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
                   // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
                   // based on the current solution
                                      RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                                      bd=0.;
                                      #ifdef USE_PPP
                           	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                                      #else
                           	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                                      #endif
                         	   real a0=mixedCoeff(tc,side,axis,grid);
                         	   real a1=mixedNormalCoeff(tc,side,axis,grid);
                   // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                                      MappedGridOperators & op = *(u.getOperators());
                                      Range N(tc,tc);
                                      RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                         	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                         	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                                      if( mg.numberOfDimensions()==2 )
                         	   {
                                          bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                            				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                         	   }
                         	   else
                         	   {
                                          RealArray uz(Ib1,Ib2,Ib3,N);
                           	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                           	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                            				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                            				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                         	   }
                                      const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                         	   if( false && twilightZoneFlow ) //  *******************************************************
                         	   {
                                          fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                           	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                           	     realArray & x= mg.center();
                #ifdef USE_PPP
                           	     realSerialArray xLocal; 
                           	     if( !rectangular || twilightZoneFlow ) 
                             	       getLocalArrayWithGhostBoundaries(x,xLocal);
                #else
                           	     const realSerialArray & xLocal = x;
                #endif
                           	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                           	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                           	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                           	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                           	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                           	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                           	     if( mg.numberOfDimensions()==2 )
                           	     {
                             	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                              				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                           	     }
                           	     else
                           	     {
                             	       realSerialArray uez(Ib1,Ib2,Ib3);
                             	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                             	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                              				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                              				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                           	     }
                         	   } // *******************************************
                       	 }
                       	 if( pBoundaryData[side][axis]!=NULL )
                         	   u.getOperators()->setTwilightZoneFlow( false );
                       	 else
                       	 {
                                      if( t>0. || debug() & 4 )
                         	   {
                                          if( ok )
                                     	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                         	   }
                       	 }
                              }
                              if( debug() & 4 )
                       	 printF("++++insBC: inflowWithVelocityGiven: (grid,side,axis)=(%i,%i,%i) : "
                            		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                            		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                            		grid,side,axis, 
                            		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                            		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                            		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                         	   );
        //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
        //        {
        // 	 mixedBoundaryConditionOnTemperature=true;
        // 	 if( debug() & 4 )
        // 	   printF("++++insBC: inflowWithVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
        // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
        //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
        // 		  grid,side,axis, 
        //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
        //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
        //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
        // 	     );
        //        }
                              if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                              {
        	 // Dirichlet
                       	 u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
        // 	      u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
        // 				       bcParams,grid);
                              }
                              else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                              {
                 // -- Variable Coefficient Temperature (const coeff.) BC --- 
                       	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                            		grid,side,axis);
                 // BC is : a0(x)*T + an(x)*T.n = g 
                 //  a0 = varCoeff(i1,i2,i3,0)
                 //  an = varCoeff(i1,i2,i3,1)
                       	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                                BoundaryData::variableCoefficientTemperatureBC,side,axis );
                                  bcParams.setVariableCoefficientsArray(&varCoeff);
                       	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                          				  bcParams,grid);
                                  bcParams.setVariableCoefficientsArray(NULL);  // reset 
                              } 
                              else
                              {
        	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                              }
                              if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                              { // reset TZ
                       	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                              }
                          } // end if bc = inflowWithVelocityGiven
                      } // end for boundary
           // ************ try this ********* 080909
           // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
                  } // end if assignTemperature
            }
            if( turbulenceModel!=Parameters::noTurbulenceModel )
            {
                turbulenceModelBoundaryConditions(t,u,parameters,grid,pBoundaryData);
            }
            if( parameters.dbase.get<int >("extrapolateInterpolationNeighbours")!=0 ||  // new way 
                    parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") ||     // get rid of these checks
                    pdeModel==InsParameters::viscoPlasticModel  )
            {
                assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours")!=0 );
        //  NOTE: The visco-plastic model uses 2 ghost lines since the coefficient of viscosity depends on the first derivatives of u.
                assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
                const int orderOfExtrapolationSaved=extrapParams.orderOfExtrapolation;
                extrapParams.orderOfExtrapolation=3; // *********
        // u.applyBoundaryCondition(V,BCTypes::extrapolateInterpolationNeighbours);
                u.applyBoundaryCondition(C,BCTypes::extrapolateInterpolationNeighbours,allBoundaries,0.,t,extrapParams);
        // *** assign the 2nd ghost line ***
                extrapParams.orderOfExtrapolation=3; // *********
                extrapParams.ghostLineToAssign=2;
                if( true )
                {
    	// printF("insBC: extrapolate 2nd ghost line on all boundaries...\n");
              	u.applyBoundaryCondition(C,extrapolate,allBoundaries,0.,t,extrapParams);
                }
        // reset: 
                extrapParams.ghostLineToAssign=1;
                extrapParams.orderOfExtrapolation=orderOfExtrapolationSaved;
            }
            else
            {
                assert( !parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
            }
            if( orderOfAccuracy==2 ) // ** moved from generic applyBC - 060907
                u.finishBoundaryConditions();
            parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-time0;
        
        return 0;
    } 


  // ********************************************************************
  // ************Non-steady state case***********************************
  // ********************************************************************

    char buff[200];

  // ----  the dirichletBoundaryCondition is for testing TZ flow. ----
    bool applyExactSolutionAtGhost=true;  // if true apply exact solution at ghost points, this sometimes gives bad results
    bool applyDivOnDirichletBoundaryCondition=false;  // try this 
    if( assignDirichletBoundaryCondition )
    {
        if( debug() & 32  )
        {
            display(u,sPrintF(buff,"insBC: u before assignDirichletBoundaryCondition, grid=%i, t=%e",grid,t),
                    parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
        }

        if( knownSolution!=InsParameters::noKnownSolution ) 
        {
            if( false )
                printF("--INSBC--  *** assign dirichletBoundaryCondition to known at t=%9.3e\n",t);

      // apply any known solution at dirichlet BC's   *wdh* 2013/07/25
            bcParams.extraInTangentialDirections=2;
            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); 

            u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,uKnownLocal,t,bcParams);
            
            bcParams.lineToAssign=0;  // reset
            bcParams.extraInTangentialDirections=0;
            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

            if( debug() & 64 )
            {
      	::display(uKnown,"insBC: known solution: uKnown",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
      	::display(u,"insBC: after assign dirichlet BC (from known solution)",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
            }

        }
        else
        {
            u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,0.,t);
        }


        checkArrayIDs(" insBC: after dirichlet"); 

        if( debug() & 32  )
        {
            display(u,sPrintF(buff,"insBC: u after dirichlet, grid=%i, t=%e",grid,t),
                    parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
        }

        if( applyExactSolutionAtGhost )
        {
      // Assign ghost values with exact solution
            BoundaryConditionParameters extrapParams;
            extrapParams.orderOfExtrapolation = orderOfAccuracy+1;
            const int discretizationHalfWidth = orderOfAccuracy/2;

            for ( int ghostLineToAssign=1; ghostLineToAssign<=discretizationHalfWidth; ghostLineToAssign++ )
            {
      	extrapParams.ghostLineToAssign = ghostLineToAssign;
      	extrapParams.lineToAssign = ghostLineToAssign;

      	if( knownSolution!=InsParameters::noKnownSolution ) 
      	{
	  // apply any known solution at dirichlet BC's   *wdh* 2013/09/28
        	  extrapParams.extraInTangentialDirections=2;
        	  extrapParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); 

        	  u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,uKnownLocal,t,extrapParams);
            
        	  extrapParams.extraInTangentialDirections=0;
        	  extrapParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

      	}
      	else
      	{
        	  u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,0.,t,extrapParams);
      	}

                extrapParams.lineToAssign=0;  // reset
      	extrapParams.ghostLineToAssign = 1;
      	
	//u.applyBoundaryCondition(Rt,extrapolate,dirichletBoundaryCondition,0.,t,extrapParams);
            }
        }
        else
        {
      // *** EXTRAPOLATE GHOST FOR dirichletBoundaryCondition ****

      // kkc 100521 adjust the order of extrapolation at the ghost lines for the accuracy of the scheme
            BoundaryConditionParameters extrapParams;
            const int discretizationHalfWidth = orderOfAccuracy/2;

            for ( int ghostLineToAssign=1; ghostLineToAssign<=discretizationHalfWidth; ghostLineToAssign++ )
            {
      	extrapParams.lineToAssign = ghostLineToAssign;
      	u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,0.,t,extrapParams);
            }
        }
        
        if( applyDivOnDirichletBoundaryCondition && orderOfAccuracy==2 ) // *try this *wdh* 2014/06/30
        {
            u.applyBoundaryCondition(V,generalizedDivergence,dirichletBoundaryCondition,0.,t);
        }
        
        
        checkArrayIDs(" insBC: after extrapolate (1)"); 
        if( debug() & 32  )
        {
            display(u,sPrintF(buff,"insBC: u after extrapolate, assignDirichletBoundaryCondition, grid=%i, t=%e",grid,t),
                    parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
        }


    }
    

  // **************************************************************************
  // ***** STAGE I : Apply dirichlet type boundary conditions *****************
  // **************************************************************************


  // either use boundaryData or use bcData.
    if( assignInflowWithVelocityGiven )
    {
    // *NOTE* cannot assign on extended range unless we increase the size of the boundaryData
        u.applyBoundaryCondition(V,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,bcParams,grid);

    // assign dirichlet BC's on T here -- other BC's are done later
          if( assignTemperature )
          {
              FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
              FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
              ForBoundary(side,axis)
              {
                  if( mg.boundaryCondition(side,axis)==inflowWithVelocityGiven )
                  {
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // This is an interface between domains
             // for now we only know about interfaces at no-slip walls: 
                          assert( mg.boundaryCondition(side,axis)==noSlipWall );
    	 // what about BC's applied at t=0 before the boundary data is set ??
    	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
    	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
    	 // to use the boundary data instead.
                          #ifdef USE_PPP
                              realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                          #else
                              const realSerialArray & uLocal = u;
                          #endif
               	 Index Ib1,Ib2,Ib3;
               	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
               	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
               	 if( debug() & 4 )
               	 {
                 	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                      		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                 			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   if( pBoundaryData[side][axis]==NULL )
                 	   {
                   	     if( !ok )
                                      fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                  else
                                      fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                 	   }
                 	   else
                 	   {
    	     // RealArray & bd = *pBoundaryData[side][axis];
    	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                 	   }
               	 }
                          assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
               	 if( ok && pBoundaryData[side][axis]==NULL )
               	 {
               // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
               // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
               // based on the current solution
                              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                              bd=0.;
                              #ifdef USE_PPP
                   	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                              #else
                   	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                              #endif
                 	   real a0=mixedCoeff(tc,side,axis,grid);
                 	   real a1=mixedNormalCoeff(tc,side,axis,grid);
               // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                              MappedGridOperators & op = *(u.getOperators());
                              Range N(tc,tc);
                              RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                              if( mg.numberOfDimensions()==2 )
                 	   {
                                  bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                 	   else
                 	   {
                                  RealArray uz(Ib1,Ib2,Ib3,N);
                   	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                   	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                              const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                 	   if( false && twilightZoneFlow ) //  *******************************************************
                 	   {
                                  fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                   	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                   	     realArray & x= mg.center();
        #ifdef USE_PPP
                   	     realSerialArray xLocal; 
                   	     if( !rectangular || twilightZoneFlow ) 
                     	       getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
                   	     const realSerialArray & xLocal = x;
        #endif
                   	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                   	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                   	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                   	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                   	     if( mg.numberOfDimensions()==2 )
                   	     {
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                   	     else
                   	     {
                     	       realSerialArray uez(Ib1,Ib2,Ib3);
                     	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                 	   } // *******************************************
               	 }
               	 if( pBoundaryData[side][axis]!=NULL )
                 	   u.getOperators()->setTwilightZoneFlow( false );
               	 else
               	 {
                              if( t>0. || debug() & 4 )
                 	   {
                                  if( ok )
                             	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                 	   }
               	 }
                      }
                      if( debug() & 4 )
               	 printF("++++insBC: inflowWithVelocityGiven: (grid,side,axis)=(%i,%i,%i) : "
                    		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                    		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                    		grid,side,axis, 
                    		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                    		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                    		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                 	   );
    //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
    //        {
    // 	 mixedBoundaryConditionOnTemperature=true;
    // 	 if( debug() & 4 )
    // 	   printF("++++insBC: inflowWithVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
    // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
    //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
    // 		  grid,side,axis, 
    //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
    //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
    //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
    // 	     );
    //        }
                      if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                      {
    	 // Dirichlet
               	 u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                      }
                      else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                      {
             // -- Variable Coefficient Temperature (const coeff.) BC --- 
               	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                    		grid,side,axis);
             // BC is : a0(x)*T + an(x)*T.n = g 
             //  a0 = varCoeff(i1,i2,i3,0)
             //  an = varCoeff(i1,i2,i3,1)
               	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );
                          bcParams.setVariableCoefficientsArray(&varCoeff);
               	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                          bcParams.setVariableCoefficientsArray(NULL);  // reset 
                      } 
                      else
                      {
    	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                      }
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // reset TZ
               	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                      }
                  } // end if bc = inflowWithVelocityGiven
              } // end for boundary
       // ************ try this ********* 080909
       // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
          } // end if assignTemperature

        if( pdeModel==InsParameters::twoPhaseFlowModel )
        {
            u.applyBoundaryCondition(twoPhaseFlowComponents,dirichlet,inflowWithVelocityGiven,bcData,
                                                              pBoundaryData,t,bcParams,grid);
        }
        
    }

    if( assignInflowWithPressureAndTangentialVelocityGiven )
    {
    //  inflowWithPressureAndTangentialVelocityGiven
    //     give tangential velocity = 0 
    //     extrapolate (u,v,w)
    //     set div(u)=0
        bcParams.extraInTangentialDirections=orderOfAccuracy/2;
        u.applyBoundaryCondition(V,tangentialComponent,inflowWithPressureAndTangentialVelocityGiven,0.,t,bcParams);
        bcParams.extraInTangentialDirections=0;  // reset
        
        if( true ) // new: *wdh* 2012/09/14 -- check me 
        {
              if( assignTemperature )
              {
                  FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
                  FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
                  ForBoundary(side,axis)
                  {
                      if( mg.boundaryCondition(side,axis)==inflowWithPressureAndTangentialVelocityGiven )
                      {
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // This is an interface between domains
               // for now we only know about interfaces at no-slip walls: 
                              assert( mg.boundaryCondition(side,axis)==noSlipWall );
      	 // what about BC's applied at t=0 before the boundary data is set ??
      	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
      	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
      	 // to use the boundary data instead.
                              #ifdef USE_PPP
                                  realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                              #else
                                  const realSerialArray & uLocal = u;
                              #endif
                   	 Index Ib1,Ib2,Ib3;
                   	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                   	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
                   	 if( debug() & 4 )
                   	 {
                     	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                          		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                     			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   if( pBoundaryData[side][axis]==NULL )
                     	   {
                       	     if( !ok )
                                          fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                      else
                                          fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                     	   }
                     	   else
                     	   {
      	     // RealArray & bd = *pBoundaryData[side][axis];
      	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                     	   }
                   	 }
                              assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
                   	 if( ok && pBoundaryData[side][axis]==NULL )
                   	 {
                 // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
                 // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
                 // based on the current solution
                                  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                                  bd=0.;
                                  #ifdef USE_PPP
                       	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                                  #else
                       	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                                  #endif
                     	   real a0=mixedCoeff(tc,side,axis,grid);
                     	   real a1=mixedNormalCoeff(tc,side,axis,grid);
                 // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                                  MappedGridOperators & op = *(u.getOperators());
                                  Range N(tc,tc);
                                  RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                                  if( mg.numberOfDimensions()==2 )
                     	   {
                                      bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                     	   else
                     	   {
                                      RealArray uz(Ib1,Ib2,Ib3,N);
                       	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                       	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                                  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                     	   if( false && twilightZoneFlow ) //  *******************************************************
                     	   {
                                      fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                       	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                       	     realArray & x= mg.center();
            #ifdef USE_PPP
                       	     realSerialArray xLocal; 
                       	     if( !rectangular || twilightZoneFlow ) 
                         	       getLocalArrayWithGhostBoundaries(x,xLocal);
            #else
                       	     const realSerialArray & xLocal = x;
            #endif
                       	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                       	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                       	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                       	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                       	     if( mg.numberOfDimensions()==2 )
                       	     {
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                       	     else
                       	     {
                         	       realSerialArray uez(Ib1,Ib2,Ib3);
                         	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                     	   } // *******************************************
                   	 }
                   	 if( pBoundaryData[side][axis]!=NULL )
                     	   u.getOperators()->setTwilightZoneFlow( false );
                   	 else
                   	 {
                                  if( t>0. || debug() & 4 )
                     	   {
                                      if( ok )
                                 	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                     	   }
                   	 }
                          }
                          if( debug() & 4 )
                   	 printF("++++insBC: inflowWithPressureAndTangentialVelocityGiven: (grid,side,axis)=(%i,%i,%i) : "
                        		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                        		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                        		grid,side,axis, 
                        		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                        		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                        		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                     	   );
      //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
      //        {
      // 	 mixedBoundaryConditionOnTemperature=true;
      // 	 if( debug() & 4 )
      // 	   printF("++++insBC: inflowWithPressureAndTangentialVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
      // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
      //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
      // 		  grid,side,axis, 
      //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
      //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
      //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
      // 	     );
      //        }
                          if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                          {
      	 // Dirichlet
                   	 u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                          }
                          else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                          {
               // -- Variable Coefficient Temperature (const coeff.) BC --- 
                   	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                        		grid,side,axis);
               // BC is : a0(x)*T + an(x)*T.n = g 
               //  a0 = varCoeff(i1,i2,i3,0)
               //  an = varCoeff(i1,i2,i3,1)
                   	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                            BoundaryData::variableCoefficientTemperatureBC,side,axis );
                              bcParams.setVariableCoefficientsArray(&varCoeff);
                   	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                              bcParams.setVariableCoefficientsArray(NULL);  // reset 
                          } 
                          else
                          {
      	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                          }
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // reset TZ
                   	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                          }
                      } // end if bc = inflowWithPressureAndTangentialVelocityGiven
                  } // end for boundary
         // ************ try this ********* 080909
         // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
              } // end if assignTemperature
        }
        else if( assignTemperature ) // *wdh* OLD
        {
            u.applyBoundaryCondition(tc,dirichlet,inflowWithPressureAndTangentialVelocityGiven,bcData,pBoundaryData,t,
                                                              bcParams,grid);
        }
        if( pdeModel==InsParameters::twoPhaseFlowModel )
        {
            u.applyBoundaryCondition(twoPhaseFlowComponents,dirichlet,inflowWithPressureAndTangentialVelocityGiven,bcData,
                                                              pBoundaryData,t,bcParams,grid);
        }    

    }

    if( assignInflowOutflow ) // *wdh* 110827
    {
    // --- apply Stage I of the inflow/outflow BC ---
    // applyInflowOutflowBC( );
        OV_ABORT("insBC: inflowOutflow BC: FINISH ME");
    }
    


    if( assignNeumannBoundaryCondition ) // kkc added 100812
    {
        u.applyBoundaryCondition(C,neumann,neumannBoundaryCondition,0.,t,bcParams);
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

    if( assignSlipWall )//&& !(timeSteppingMethod==Parameters::implicit && method==Parameters::approximateFactorization))
    {
        if( gridIsMoving )
            u.applyBoundaryCondition(V,normalComponent,slipWall,gridVelocity,t,bcParams);
        else
            u.applyBoundaryCondition(V,normalComponent,slipWall,0.,t,bcParams);
    }


  // old:  const  int nc=parameters.dbase.get<int >("numberOfComponents");
    
    if( assignNoSlipWall )
    {
        if( gridIsMoving )
        {
            
            u.applyBoundaryCondition(V,dirichlet,noSlipWall,gridVelocity,t,bcParams);

            if( false )
            {
      	display(u,"--insBC-- u after moving noSlipWall","%6.3f ");
      	display(gridVelocity,"--insBC-- gridVelocity after moving noSlipWall","%6.3f ");
            }
            
        }
        
        else
        {
      // old: u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,t,bcParams,grid);
      // We now allow for variable inflow on a wall: *wdh* 110829
            u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,pBoundaryData,t,bcParams,grid);
        }
        
    // assign dirichlet BC's on T here -- other BC's are done later
          if( assignTemperature )
          {
              FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
              FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
              ForBoundary(side,axis)
              {
                  if( mg.boundaryCondition(side,axis)==noSlipWall )
                  {
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // This is an interface between domains
             // for now we only know about interfaces at no-slip walls: 
                          assert( mg.boundaryCondition(side,axis)==noSlipWall );
    	 // what about BC's applied at t=0 before the boundary data is set ??
    	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
    	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
    	 // to use the boundary data instead.
                          #ifdef USE_PPP
                              realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                          #else
                              const realSerialArray & uLocal = u;
                          #endif
               	 Index Ib1,Ib2,Ib3;
               	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
               	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
               	 if( debug() & 4 )
               	 {
                 	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                      		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                 			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   if( pBoundaryData[side][axis]==NULL )
                 	   {
                   	     if( !ok )
                                      fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                  else
                                      fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                 	   }
                 	   else
                 	   {
    	     // RealArray & bd = *pBoundaryData[side][axis];
    	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                 	   }
               	 }
                          assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
               	 if( ok && pBoundaryData[side][axis]==NULL )
               	 {
               // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
               // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
               // based on the current solution
                              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                              bd=0.;
                              #ifdef USE_PPP
                   	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                              #else
                   	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                              #endif
                 	   real a0=mixedCoeff(tc,side,axis,grid);
                 	   real a1=mixedNormalCoeff(tc,side,axis,grid);
               // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                              MappedGridOperators & op = *(u.getOperators());
                              Range N(tc,tc);
                              RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                              if( mg.numberOfDimensions()==2 )
                 	   {
                                  bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                 	   else
                 	   {
                                  RealArray uz(Ib1,Ib2,Ib3,N);
                   	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                   	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                              const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                 	   if( false && twilightZoneFlow ) //  *******************************************************
                 	   {
                                  fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                   	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                   	     realArray & x= mg.center();
        #ifdef USE_PPP
                   	     realSerialArray xLocal; 
                   	     if( !rectangular || twilightZoneFlow ) 
                     	       getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
                   	     const realSerialArray & xLocal = x;
        #endif
                   	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                   	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                   	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                   	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                   	     if( mg.numberOfDimensions()==2 )
                   	     {
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                   	     else
                   	     {
                     	       realSerialArray uez(Ib1,Ib2,Ib3);
                     	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                 	   } // *******************************************
               	 }
               	 if( pBoundaryData[side][axis]!=NULL )
                 	   u.getOperators()->setTwilightZoneFlow( false );
               	 else
               	 {
                              if( t>0. || debug() & 4 )
                 	   {
                                  if( ok )
                             	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                 	   }
               	 }
                      }
                      if( debug() & 4 )
               	 printF("++++insBC: noSlipWall: (grid,side,axis)=(%i,%i,%i) : "
                    		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                    		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                    		grid,side,axis, 
                    		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                    		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                    		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                 	   );
    //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
    //        {
    // 	 mixedBoundaryConditionOnTemperature=true;
    // 	 if( debug() & 4 )
    // 	   printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
    // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
    //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
    // 		  grid,side,axis, 
    //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
    //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
    //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
    // 	     );
    //        }
                      if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                      {
    	 // Dirichlet
               	 u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                      }
                      else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                      {
             // -- Variable Coefficient Temperature (const coeff.) BC --- 
               	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                    		grid,side,axis);
             // BC is : a0(x)*T + an(x)*T.n = g 
             //  a0 = varCoeff(i1,i2,i3,0)
             //  an = varCoeff(i1,i2,i3,1)
               	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );
                          bcParams.setVariableCoefficientsArray(&varCoeff);
               	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                          bcParams.setVariableCoefficientsArray(NULL);  // reset 
                      } 
                      else
                      {
    	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                      }
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // reset TZ
               	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                      }
                  } // end if bc = noSlipWall
              } // end for boundary
       // ************ try this ********* 080909
       // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
          } // end if assignTemperature

    // display(u,"u after dirichlet noSlipWall",parameters.dbase.get<FILE* >("debugFile"));
    }
    
    bcParams.extraInTangentialDirections=0; // reset
    


    if( assignFreeSurfaceBoundaryCondition )
    {
    // Set any dirichlet Temperature BC's for the the free surface
          if( assignTemperature )
          {
              FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
              FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
              ForBoundary(side,axis)
              {
                  if( mg.boundaryCondition(side,axis)==freeSurfaceBoundaryCondition )
                  {
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // This is an interface between domains
             // for now we only know about interfaces at no-slip walls: 
                          assert( mg.boundaryCondition(side,axis)==noSlipWall );
    	 // what about BC's applied at t=0 before the boundary data is set ??
    	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
    	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
    	 // to use the boundary data instead.
                          #ifdef USE_PPP
                              realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                          #else
                              const realSerialArray & uLocal = u;
                          #endif
               	 Index Ib1,Ib2,Ib3;
               	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
               	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
               	 if( debug() & 4 )
               	 {
                 	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                      		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                 			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   if( pBoundaryData[side][axis]==NULL )
                 	   {
                   	     if( !ok )
                                      fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                  else
                                      fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                 	   }
                 	   else
                 	   {
    	     // RealArray & bd = *pBoundaryData[side][axis];
    	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                 	   }
               	 }
                          assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
               	 if( ok && pBoundaryData[side][axis]==NULL )
               	 {
               // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
               // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
               // based on the current solution
                              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                              bd=0.;
                              #ifdef USE_PPP
                   	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                              #else
                   	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                              #endif
                 	   real a0=mixedCoeff(tc,side,axis,grid);
                 	   real a1=mixedNormalCoeff(tc,side,axis,grid);
               // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                              MappedGridOperators & op = *(u.getOperators());
                              Range N(tc,tc);
                              RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                              if( mg.numberOfDimensions()==2 )
                 	   {
                                  bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                 	   else
                 	   {
                                  RealArray uz(Ib1,Ib2,Ib3,N);
                   	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                   	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                              const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                 	   if( false && twilightZoneFlow ) //  *******************************************************
                 	   {
                                  fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                   	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                   	     realArray & x= mg.center();
        #ifdef USE_PPP
                   	     realSerialArray xLocal; 
                   	     if( !rectangular || twilightZoneFlow ) 
                     	       getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
                   	     const realSerialArray & xLocal = x;
        #endif
                   	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                   	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                   	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                   	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                   	     if( mg.numberOfDimensions()==2 )
                   	     {
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                   	     else
                   	     {
                     	       realSerialArray uez(Ib1,Ib2,Ib3);
                     	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                 	   } // *******************************************
               	 }
               	 if( pBoundaryData[side][axis]!=NULL )
                 	   u.getOperators()->setTwilightZoneFlow( false );
               	 else
               	 {
                              if( t>0. || debug() & 4 )
                 	   {
                                  if( ok )
                             	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                 	   }
               	 }
                      }
                      if( debug() & 4 )
               	 printF("++++insBC: freeSurfaceBoundaryCondition: (grid,side,axis)=(%i,%i,%i) : "
                    		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                    		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                    		grid,side,axis, 
                    		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                    		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                    		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                 	   );
    //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
    //        {
    // 	 mixedBoundaryConditionOnTemperature=true;
    // 	 if( debug() & 4 )
    // 	   printF("++++insBC: freeSurfaceBoundaryCondition:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
    // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
    //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
    // 		  grid,side,axis, 
    //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
    //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
    //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
    // 	     );
    //        }
                      if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                      {
    	 // Dirichlet
               	 u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                      }
                      else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                      {
             // -- Variable Coefficient Temperature (const coeff.) BC --- 
               	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                    		grid,side,axis);
             // BC is : a0(x)*T + an(x)*T.n = g 
             //  a0 = varCoeff(i1,i2,i3,0)
             //  an = varCoeff(i1,i2,i3,1)
               	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );
                          bcParams.setVariableCoefficientsArray(&varCoeff);
               	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                          bcParams.setVariableCoefficientsArray(NULL);  // reset 
                      } 
                      else
                      {
    	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                      }
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // reset TZ
               	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                      }
                  } // end if bc = freeSurfaceBoundaryCondition
              } // end for boundary
       // ************ try this ********* 080909
       // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
          } // end if assignTemperature
    }



    if( assignOutflow  && orderOfAccuracy==2 )
    {
    // outflow:
    // (1) extrapolate (u,v,w,p)    (default case)
    // (2) set alpha p + beta p.n =   (done in assignPressureRHS)

        if( parameters.dbase.get<int>("outflowOption")==1 ||
                parameters.dbase.get<int >("checkForInflowAtOutFlow")==2 )
        {
      // expect inflow at an outflow boundary -- use Neumann BC instead of extrapolating
      // printF("insBC: apply neumann BC at outflow\n");
            if( true )
            {
      	for( int n=0; n<numberOfDimensions; n++ )
      	{
        	  u.applyBoundaryCondition(uc+n,neumann,outflow,0.,t);
      	}
            }
            else
            {
                u.applyBoundaryCondition(V,neumann,outflow,0.,t);
            }
            
            if( assignTemperature )
            {
      	const int orderOfExtrapolation=extrapParams.orderOfExtrapolation;
      	if( parameters.dbase.get<int >("orderOfExtrapolationForOutflow")>= 0 )
        	  extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
      	u.applyBoundaryCondition(tc,extrapolate,outflow,0.,t,extrapParams);
      	extrapParams.orderOfExtrapolation=orderOfExtrapolation;  // reset
            }
            
        }
        else
        {
      // default outflow BC's (Note: temperature will be done here too)
            const int orderOfExtrapolation=extrapParams.orderOfExtrapolation;
            if( parameters.dbase.get<int >("orderOfExtrapolationForOutflow")>= 0 )
      	extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
            u.applyBoundaryCondition(Rt,extrapolate,outflow,0.,t,extrapParams);
            extrapParams.orderOfExtrapolation=orderOfExtrapolation;  // reset
        }
    }
    
    if( assignFreeSurfaceBoundaryCondition )
    {
    // Free surface BC:
    //    p = given
    //    n. sigma . tau_m = 0 , 
    //    div( v ) = 0 

    // ::display(u,"insBC: solution BEFORE freeSurface",parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");


    // For now just apply a neumann BC  ***FIX ME**
        u.applyBoundaryCondition(V,neumann,freeSurfaceBoundaryCondition,0.,t);  
    //  u.applyBoundaryCondition(uc,neumann,freeSurfaceBoundaryCondition,0.,t);  
    //  u.applyBoundaryCondition(vc,neumann,freeSurfaceBoundaryCondition,0.,t);  

    // ::display(u,"insBC: solution AFTER freeSurface",parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");

   // Set any mixed Temperature BC's for the the free surface
          if( assignTemperature )
          {
              FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
              FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
              ForBoundary(side,axis)
              {
                  if( mg.boundaryCondition(side,axis)==freeSurfaceBoundaryCondition )
                  {
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // This is an interface between domains
             // for now we only know about interfaces at no-slip walls: 
                          assert( mg.boundaryCondition(side,axis)==noSlipWall );
    	 // what about BC's applied at t=0 before the boundary data is set ??
    	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
    	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
    	 // to use the boundary data instead.
                          #ifdef USE_PPP
                              realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                          #else
                              const realSerialArray & uLocal = u;
                          #endif
               	 Index Ib1,Ib2,Ib3;
               	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
               	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
               	 if( debug() & 4 )
               	 {
                 	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                      		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                 			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   if( pBoundaryData[side][axis]==NULL )
                 	   {
                   	     if( !ok )
                                      fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                  else
                                      fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                 	   }
                 	   else
                 	   {
    	     // RealArray & bd = *pBoundaryData[side][axis];
    	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                 	   }
               	 }
                          assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
               	 if( ok && pBoundaryData[side][axis]==NULL )
               	 {
               // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
               // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
               // based on the current solution
                              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                              bd=0.;
                              #ifdef USE_PPP
                   	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                              #else
                   	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                              #endif
                 	   real a0=mixedCoeff(tc,side,axis,grid);
                 	   real a1=mixedNormalCoeff(tc,side,axis,grid);
               // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                              MappedGridOperators & op = *(u.getOperators());
                              Range N(tc,tc);
                              RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                              if( mg.numberOfDimensions()==2 )
                 	   {
                                  bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                 	   else
                 	   {
                                  RealArray uz(Ib1,Ib2,Ib3,N);
                   	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                   	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                              const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                 	   if( false && twilightZoneFlow ) //  *******************************************************
                 	   {
                                  fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                   	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                   	     realArray & x= mg.center();
        #ifdef USE_PPP
                   	     realSerialArray xLocal; 
                   	     if( !rectangular || twilightZoneFlow ) 
                     	       getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
                   	     const realSerialArray & xLocal = x;
        #endif
                   	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                   	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                   	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                   	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                   	     if( mg.numberOfDimensions()==2 )
                   	     {
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                   	     else
                   	     {
                     	       realSerialArray uez(Ib1,Ib2,Ib3);
                     	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                 	   } // *******************************************
               	 }
               	 if( pBoundaryData[side][axis]!=NULL )
                 	   u.getOperators()->setTwilightZoneFlow( false );
               	 else
               	 {
                              if( t>0. || debug() & 4 )
                 	   {
                                  if( ok )
                             	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                 	   }
               	 }
                      }
                      if( debug() & 4 )
               	 printF("++++insBC: freeSurfaceBoundaryCondition: (grid,side,axis)=(%i,%i,%i) : "
                    		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                    		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                    		grid,side,axis, 
                    		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                    		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                    		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                 	   );
    //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
    //        {
    // 	 mixedBoundaryConditionOnTemperature=true;
    // 	 if( debug() & 4 )
    // 	   printF("++++insBC: freeSurfaceBoundaryCondition:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
    // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
    //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
    // 		  grid,side,axis, 
    //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
    //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
    //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
    // 	     );
    //        }
                      if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                      {
    	 // Dirichlet
                      }
                      else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                      {
             // -- Variable Coefficient Temperature (const coeff.) BC --- 
               	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                    		grid,side,axis);
             // BC is : a0(x)*T + an(x)*T.n = g 
             //  a0 = varCoeff(i1,i2,i3,0)
             //  an = varCoeff(i1,i2,i3,1)
               	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );
                          bcParams.setVariableCoefficientsArray(&varCoeff);
               	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                          bcParams.setVariableCoefficientsArray(NULL);  // reset 
                      } 
                      else
                      {
    	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
    	 // Mixed BC or Neumann
               	 real a0=mixedCoeff(tc,side,axis,grid);
               	 real a1=mixedNormalCoeff(tc,side,axis,grid);
               	 bcParams.a.redim(3);
               	 if( a0==0. && a1==1. )
               	 {
                 	   if( debug() & 4 )
                   	     printF("++++insBC: freeSurfaceBoundaryCondition:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
                        		    grid,side,axis);
    //                 real b0=bcData(tc+2,side,axis,grid);
    // 		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),b0,t); // b0 ignored??
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid);  // this is not used -- this does not work
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
               	 }
               	 else
               	 {
                 	   if( debug() & 4 )
                 	   {
                   	     fPrintF(pDebugFile,"++++insBC:freeSurfaceBoundaryCondition:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
                        		    "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f (t=%8.2e)\n",
                         		     grid,side,axis,a0,a1,bcData(tc,side,axis,grid),t);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ib1,Ib2,Ib3;
                    	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                                    RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                    	      ::display(bd(Ib1,Ib2,Ib3,tc),"insBC:freeSurfaceBoundaryCondition:T: RHS for mixed BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%5.2f ");
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:freeSurfaceBoundaryCondition:T: BEFORE mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid); 
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:freeSurfaceBoundaryCondition:T: AFTER mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
               	 }
                      }
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // reset TZ
               	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                      }
                  } // end if bc = freeSurfaceBoundaryCondition
              } // end for boundary
       // ************ try this ********* 080909
       // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
          } // end if assignTemperature

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
        
    if( !parameters.dbase.get<bool >("twilightZoneFlow") &&  
            assignOutflow && orderOfAccuracy==2 && 
            parameters.dbase.get<int>("outflowOption")==0 && 
            parameters.dbase.get<int >("checkForInflowAtOutFlow")==1 )
    {
    // check for inflow at the outflow boundary    
        for( axis=0; axis<mg.numberOfDimensions(); axis++ )
        {
            for( side=Start; side<=End; side++ )
            {
      	if( mg.boundaryCondition(side,axis)==outflow )
      	{
        	  RealDistributedArray & normal  = mg.vertexBoundaryNormal(side,axis);  

        	  getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
        	  intArray & mask = bcParams.mask();
        	  mask.redim(I1,I2,I3);   // mask lives on ghost line.
        	  getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
        	  if( isRectangular )
        	  {
          	    mask = u(I1,I2,I3,uc+axis)*(2*side-1) <0.;
        	  }
        	  else
        	  {
          	    if( mg.numberOfDimensions()==2 )
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
	  // printF("---> insBC: number of outflow points that are inflow = %i\n",count);
        	  if( count>0 )
        	  {
          	    if( numberOfOutflowPointsAtInflowMessages<=50 &&
            		(debug()& 2 || debug() & 4) )
          	    {
            	      numberOfOutflowPointsAtInflowMessages++;
            	      printF("--INSBC-- number of outflow points that are inflow = %i\n",count);
            	      if( numberOfOutflowPointsAtInflowMessages==50 )
            		printF("--INSBC--WARNING: too many 'number of outflow points that are inflow' messages."
                                              " I will not print anymore.\n");
          	    }
          	    
          	    bcParams.setUseMask(TRUE);

                        if( false )  // add this as an option 
          	    {
              // *wdh* 2014/06/08 -- try a mixed BC
            	      bcParams.a.redim(3);
            	      bcParams.a(0)=1.;
            	      bcParams.a(1)=.01;
            	      bcParams.a(2)=0.;

            	      u.applyBoundaryCondition(V,mixed,BCTypes::boundary(side,axis),0.,t,bcParams);
              // real alpha=.1;
	      // u.applyBoundaryCondition(V,normalComponent,BCTypes::boundary(side,axis),alpha,t);
          	    }
          	    else
          	    {
            	      u.applyBoundaryCondition(V,neumann,BCTypes::boundary(side,axis),0.,t,bcParams);
          	    }
          	    

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

        if( true ) // 061015: use this again to be consistent with implicit time stepping BC's
            u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t);   
        else
        { // use this 981130
            u.applyBoundaryCondition(V,extrapolate,    slipWall,0.,t);
            if( true )
            {
      	u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0, slipWall,0.,t);
      	if( numberOfDimensions==3 )
        	  u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1, slipWall,0.,t);
            }
        }

        if( assignTemperature )
        {
            u.applyBoundaryCondition(tc,neumann,slipWall,0.,t);
        }
        if( pdeModel==InsParameters::twoPhaseFlowModel )
        {
            u.applyBoundaryCondition(twoPhaseFlowComponents,neumann,slipWall,0.,t);
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

          if( assignTemperature )
          {
              FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
              FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
              ForBoundary(side,axis)
              {
                  if( mg.boundaryCondition(side,axis)==inflowWithVelocityGiven )
                  {
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // This is an interface between domains
             // for now we only know about interfaces at no-slip walls: 
                          assert( mg.boundaryCondition(side,axis)==noSlipWall );
    	 // what about BC's applied at t=0 before the boundary data is set ??
    	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
    	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
    	 // to use the boundary data instead.
                          #ifdef USE_PPP
                              realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                          #else
                              const realSerialArray & uLocal = u;
                          #endif
               	 Index Ib1,Ib2,Ib3;
               	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
               	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
               	 if( debug() & 4 )
               	 {
                 	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                      		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                 			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   if( pBoundaryData[side][axis]==NULL )
                 	   {
                   	     if( !ok )
                                      fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                  else
                                      fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                 	   }
                 	   else
                 	   {
    	     // RealArray & bd = *pBoundaryData[side][axis];
    	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                 	   }
               	 }
                          assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
               	 if( ok && pBoundaryData[side][axis]==NULL )
               	 {
               // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
               // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
               // based on the current solution
                              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                              bd=0.;
                              #ifdef USE_PPP
                   	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                              #else
                   	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                              #endif
                 	   real a0=mixedCoeff(tc,side,axis,grid);
                 	   real a1=mixedNormalCoeff(tc,side,axis,grid);
               // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                              MappedGridOperators & op = *(u.getOperators());
                              Range N(tc,tc);
                              RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                              if( mg.numberOfDimensions()==2 )
                 	   {
                                  bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                 	   else
                 	   {
                                  RealArray uz(Ib1,Ib2,Ib3,N);
                   	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                   	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                              const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                 	   if( false && twilightZoneFlow ) //  *******************************************************
                 	   {
                                  fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                   	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                   	     realArray & x= mg.center();
        #ifdef USE_PPP
                   	     realSerialArray xLocal; 
                   	     if( !rectangular || twilightZoneFlow ) 
                     	       getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
                   	     const realSerialArray & xLocal = x;
        #endif
                   	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                   	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                   	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                   	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                   	     if( mg.numberOfDimensions()==2 )
                   	     {
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                   	     else
                   	     {
                     	       realSerialArray uez(Ib1,Ib2,Ib3);
                     	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                 	   } // *******************************************
               	 }
               	 if( pBoundaryData[side][axis]!=NULL )
                 	   u.getOperators()->setTwilightZoneFlow( false );
               	 else
               	 {
                              if( t>0. || debug() & 4 )
                 	   {
                                  if( ok )
                             	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                 	   }
               	 }
                      }
                      if( debug() & 4 )
               	 printF("++++insBC: inflowWithVelocityGiven: (grid,side,axis)=(%i,%i,%i) : "
                    		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                    		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                    		grid,side,axis, 
                    		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                    		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                    		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                 	   );
    //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
    //        {
    // 	 mixedBoundaryConditionOnTemperature=true;
    // 	 if( debug() & 4 )
    // 	   printF("++++insBC: inflowWithVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
    // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
    //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
    // 		  grid,side,axis, 
    //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
    //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
    //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
    // 	     );
    //        }
                      if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                      {
    	 // Dirichlet
                      }
                      else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                      {
             // -- Variable Coefficient Temperature (const coeff.) BC --- 
               	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                    		grid,side,axis);
             // BC is : a0(x)*T + an(x)*T.n = g 
             //  a0 = varCoeff(i1,i2,i3,0)
             //  an = varCoeff(i1,i2,i3,1)
               	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );
                          bcParams.setVariableCoefficientsArray(&varCoeff);
               	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                          bcParams.setVariableCoefficientsArray(NULL);  // reset 
                      } 
                      else
                      {
    	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
    	 // Mixed BC or Neumann
               	 real a0=mixedCoeff(tc,side,axis,grid);
               	 real a1=mixedNormalCoeff(tc,side,axis,grid);
               	 bcParams.a.redim(3);
               	 if( a0==0. && a1==1. )
               	 {
                 	   if( debug() & 4 )
                   	     printF("++++insBC: inflowWithVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
                        		    grid,side,axis);
    //                 real b0=bcData(tc+2,side,axis,grid);
    // 		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),b0,t); // b0 ignored??
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid);  // this is not used -- this does not work
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
               	 }
               	 else
               	 {
                 	   if( debug() & 4 )
                 	   {
                   	     fPrintF(pDebugFile,"++++insBC:inflowWithVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
                        		    "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f (t=%8.2e)\n",
                         		     grid,side,axis,a0,a1,bcData(tc,side,axis,grid),t);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ib1,Ib2,Ib3;
                    	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                                    RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                    	      ::display(bd(Ib1,Ib2,Ib3,tc),"insBC:inflowWithVelocityGiven:T: RHS for mixed BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%5.2f ");
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:inflowWithVelocityGiven:T: BEFORE mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid); 
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:inflowWithVelocityGiven:T: AFTER mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
               	 }
                      }
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // reset TZ
               	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                      }
                  } // end if bc = inflowWithVelocityGiven
              } // end for boundary
       // ************ try this ********* 080909
       // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
          } // end if assignTemperature
    }
    
  // *wdh* 090804: 
    if( assignInflowWithPressureAndTangentialVelocityGiven )
    {
        u.applyBoundaryCondition(Rt,extrapolate,inflowWithPressureAndTangentialVelocityGiven,0.,t);
    // u.applyBoundaryCondition(Rt,neumann,inflowWithPressureAndTangentialVelocityGiven,0.,t);
          if( assignTemperature )
          {
              FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
              FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
              ForBoundary(side,axis)
              {
                  if( mg.boundaryCondition(side,axis)==inflowWithPressureAndTangentialVelocityGiven )
                  {
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // This is an interface between domains
             // for now we only know about interfaces at no-slip walls: 
                          assert( mg.boundaryCondition(side,axis)==noSlipWall );
    	 // what about BC's applied at t=0 before the boundary data is set ??
    	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
    	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
    	 // to use the boundary data instead.
                          #ifdef USE_PPP
                              realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                          #else
                              const realSerialArray & uLocal = u;
                          #endif
               	 Index Ib1,Ib2,Ib3;
               	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
               	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
               	 if( debug() & 4 )
               	 {
                 	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                      		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                 			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   if( pBoundaryData[side][axis]==NULL )
                 	   {
                   	     if( !ok )
                                      fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                  else
                                      fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                 	   }
                 	   else
                 	   {
    	     // RealArray & bd = *pBoundaryData[side][axis];
    	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                 	   }
               	 }
                          assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
               	 if( ok && pBoundaryData[side][axis]==NULL )
               	 {
               // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
               // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
               // based on the current solution
                              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                              bd=0.;
                              #ifdef USE_PPP
                   	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                              #else
                   	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                              #endif
                 	   real a0=mixedCoeff(tc,side,axis,grid);
                 	   real a1=mixedNormalCoeff(tc,side,axis,grid);
               // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                              MappedGridOperators & op = *(u.getOperators());
                              Range N(tc,tc);
                              RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                              if( mg.numberOfDimensions()==2 )
                 	   {
                                  bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                 	   else
                 	   {
                                  RealArray uz(Ib1,Ib2,Ib3,N);
                   	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                   	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                              const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                 	   if( false && twilightZoneFlow ) //  *******************************************************
                 	   {
                                  fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                   	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                   	     realArray & x= mg.center();
        #ifdef USE_PPP
                   	     realSerialArray xLocal; 
                   	     if( !rectangular || twilightZoneFlow ) 
                     	       getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
                   	     const realSerialArray & xLocal = x;
        #endif
                   	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                   	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                   	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                   	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                   	     if( mg.numberOfDimensions()==2 )
                   	     {
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                   	     else
                   	     {
                     	       realSerialArray uez(Ib1,Ib2,Ib3);
                     	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                 	   } // *******************************************
               	 }
               	 if( pBoundaryData[side][axis]!=NULL )
                 	   u.getOperators()->setTwilightZoneFlow( false );
               	 else
               	 {
                              if( t>0. || debug() & 4 )
                 	   {
                                  if( ok )
                             	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                 	   }
               	 }
                      }
                      if( debug() & 4 )
               	 printF("++++insBC: inflowWithPressureAndTangentialVelocityGiven: (grid,side,axis)=(%i,%i,%i) : "
                    		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                    		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                    		grid,side,axis, 
                    		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                    		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                    		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                 	   );
    //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
    //        {
    // 	 mixedBoundaryConditionOnTemperature=true;
    // 	 if( debug() & 4 )
    // 	   printF("++++insBC: inflowWithPressureAndTangentialVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
    // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
    //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
    // 		  grid,side,axis, 
    //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
    //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
    //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
    // 	     );
    //        }
                      if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                      {
    	 // Dirichlet
                      }
                      else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                      {
             // -- Variable Coefficient Temperature (const coeff.) BC --- 
               	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                    		grid,side,axis);
             // BC is : a0(x)*T + an(x)*T.n = g 
             //  a0 = varCoeff(i1,i2,i3,0)
             //  an = varCoeff(i1,i2,i3,1)
               	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );
                          bcParams.setVariableCoefficientsArray(&varCoeff);
               	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                          bcParams.setVariableCoefficientsArray(NULL);  // reset 
                      } 
                      else
                      {
    	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
    	 // Mixed BC or Neumann
               	 real a0=mixedCoeff(tc,side,axis,grid);
               	 real a1=mixedNormalCoeff(tc,side,axis,grid);
               	 bcParams.a.redim(3);
               	 if( a0==0. && a1==1. )
               	 {
                 	   if( debug() & 4 )
                   	     printF("++++insBC: inflowWithPressureAndTangentialVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
                        		    grid,side,axis);
    //                 real b0=bcData(tc+2,side,axis,grid);
    // 		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),b0,t); // b0 ignored??
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid);  // this is not used -- this does not work
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
               	 }
               	 else
               	 {
                 	   if( debug() & 4 )
                 	   {
                   	     fPrintF(pDebugFile,"++++insBC:inflowWithPressureAndTangentialVelocityGiven:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
                        		    "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f (t=%8.2e)\n",
                         		     grid,side,axis,a0,a1,bcData(tc,side,axis,grid),t);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ib1,Ib2,Ib3;
                    	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                                    RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                    	      ::display(bd(Ib1,Ib2,Ib3,tc),"insBC:inflowWithPressureAndTangentialVelocityGiven:T: RHS for mixed BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%5.2f ");
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:inflowWithPressureAndTangentialVelocityGiven:T: BEFORE mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid); 
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:inflowWithPressureAndTangentialVelocityGiven:T: AFTER mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
               	 }
                      }
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // reset TZ
               	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                      }
                  } // end if bc = inflowWithPressureAndTangentialVelocityGiven
              } // end for boundary
       // ************ try this ********* 080909
       // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
          } // end if assignTemperature
    }
    
    if( assignNoSlipWall && orderOfAccuracy==2 )
    {
    // noSlipWall stage (2) : 

    // (1) set (u,v,w)=
    // (2) extrapolate (u,v,w,p)
    //     Assign T: a0*T + a1*T_n  g 
    // (3) set div(u)=0. (done further below)

        u.applyBoundaryCondition(Rt,extrapolate,noSlipWall,0.,t);

    // *** TEST***
        if( FALSE && useAddedMassAlgorithm )
            u.applyBoundaryCondition(V,neumann,noSlipWall,0.,t);
        

    // extrapParams.dbase.get< >("orderOfExtrapolation")=4; // *****  why??
    // u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t,extrapParams);

    // display(u,"u before mixed noSlipWall",parameters.dbase.get<FILE* >("debugFile"));

          if( assignTemperature )
          {
              FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
              FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
              ForBoundary(side,axis)
              {
                  if( mg.boundaryCondition(side,axis)==noSlipWall )
                  {
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // This is an interface between domains
             // for now we only know about interfaces at no-slip walls: 
                          assert( mg.boundaryCondition(side,axis)==noSlipWall );
    	 // what about BC's applied at t=0 before the boundary data is set ??
    	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
    	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
    	 // to use the boundary data instead.
                          #ifdef USE_PPP
                              realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                          #else
                              const realSerialArray & uLocal = u;
                          #endif
               	 Index Ib1,Ib2,Ib3;
               	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
               	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
               	 if( debug() & 4 )
               	 {
                 	   printP("insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                      		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   fprintf(pDebugFile,"insBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                 			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                 	   if( pBoundaryData[side][axis]==NULL )
                 	   {
                   	     if( !ok )
                                      fprintf(pDebugFile," insBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                  else
                                      fprintf(pDebugFile," insBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                 	   }
                 	   else
                 	   {
    	     // RealArray & bd = *pBoundaryData[side][axis];
    	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                 	   }
               	 }
                          assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
               	 if( ok && pBoundaryData[side][axis]==NULL )
               	 {
               // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
               // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
               // based on the current solution
                              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                              bd=0.;
                              #ifdef USE_PPP
                   	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                              #else
                   	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                              #endif
                 	   real a0=mixedCoeff(tc,side,axis,grid);
                 	   real a1=mixedNormalCoeff(tc,side,axis,grid);
               // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                              MappedGridOperators & op = *(u.getOperators());
                              Range N(tc,tc);
                              RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                 	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                              if( mg.numberOfDimensions()==2 )
                 	   {
                                  bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                 	   else
                 	   {
                                  RealArray uz(Ib1,Ib2,Ib3,N);
                   	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                   	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                    				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                 	   }
                              const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                 	   if( false && twilightZoneFlow ) //  *******************************************************
                 	   {
                                  fprintf(pDebugFile," insBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                   	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                   	     realArray & x= mg.center();
        #ifdef USE_PPP
                   	     realSerialArray xLocal; 
                   	     if( !rectangular || twilightZoneFlow ) 
                     	       getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
                   	     const realSerialArray & xLocal = x;
        #endif
                   	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                   	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                   	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                   	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                   	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                   	     if( mg.numberOfDimensions()==2 )
                   	     {
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                   	     else
                   	     {
                     	       realSerialArray uez(Ib1,Ib2,Ib3);
                     	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                     	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                      				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                   	     }
                 	   } // *******************************************
               	 }
               	 if( pBoundaryData[side][axis]!=NULL )
                 	   u.getOperators()->setTwilightZoneFlow( false );
               	 else
               	 {
                              if( t>0. || debug() & 4 )
                 	   {
                                  if( ok )
                             	       printP("$$$$ insBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                 	   }
               	 }
                      }
                      if( debug() & 4 )
               	 printF("++++insBC: noSlipWall: (grid,side,axis)=(%i,%i,%i) : "
                    		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                    		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                    		grid,side,axis, 
                    		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                    		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                    		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                 	   );
    //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
    //        {
    // 	 mixedBoundaryConditionOnTemperature=true;
    // 	 if( debug() & 4 )
    // 	   printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
    // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
    //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
    // 		  grid,side,axis, 
    //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
    //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
    //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
    // 	     );
    //        }
                      if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                      {
    	 // Dirichlet
                      }
                      else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                      {
             // -- Variable Coefficient Temperature (const coeff.) BC --- 
               	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                    		grid,side,axis);
             // BC is : a0(x)*T + an(x)*T.n = g 
             //  a0 = varCoeff(i1,i2,i3,0)
             //  an = varCoeff(i1,i2,i3,1)
               	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );
                          bcParams.setVariableCoefficientsArray(&varCoeff);
               	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                  				  bcParams,grid);
                          bcParams.setVariableCoefficientsArray(NULL);  // reset 
                      } 
                      else
                      {
    	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
    	 // Mixed BC or Neumann
               	 real a0=mixedCoeff(tc,side,axis,grid);
               	 real a1=mixedNormalCoeff(tc,side,axis,grid);
               	 bcParams.a.redim(3);
               	 if( a0==0. && a1==1. )
               	 {
                 	   if( debug() & 4 )
                   	     printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
                        		    grid,side,axis);
    //                 real b0=bcData(tc+2,side,axis,grid);
    // 		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),b0,t); // b0 ignored??
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid);  // this is not used -- this does not work
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
               	 }
               	 else
               	 {
                 	   if( debug() & 4 )
                 	   {
                   	     fPrintF(pDebugFile,"++++insBC:noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
                        		    "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f (t=%8.2e)\n",
                         		     grid,side,axis,a0,a1,bcData(tc,side,axis,grid),t);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ib1,Ib2,Ib3;
                    	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                                    RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                    	      ::display(bd(Ib1,Ib2,Ib3,tc),"insBC:noSlipWall:T: RHS for mixed BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%5.2f ");
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:noSlipWall:T: BEFORE mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
                 	   bcParams.a(0)=a0;
                 	   bcParams.a(1)=a1;
                 	   bcParams.a(2)=mixedRHS(tc,side,axis,grid); 
                 	   if( false )
                 	   {  // **** TEMP FIX ****
                   	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                 	   }
                 	   else
                 	   {
                   	     u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				      bcParams,grid);
                 	   }
                 	   if( debug() & 4 )
                 	   {
                                  #ifndef USE_PPP
                        	      Index Ig1,Ig2,Ig3;
    	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                    	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                    ::display(u(Ig1,Ig2,Ig3,tc),"insBC:noSlipWall:T: AFTER mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                  #endif
                 	   }
               	 }
                      }
                      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                      { // reset TZ
               	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                      }
                  } // end if bc = noSlipWall
              } // end for boundary
       // ************ try this ********* 080909
       // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
          } // end if assignTemperature

    // display(u,"u after mixed noSlipWall",parameters.dbase.get<FILE* >("debugFile"));
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
    
        if( assignTemperature )
        {
            u.applyBoundaryCondition(tc,neumann,axisymmetric,0.,t);
        }
        if( pdeModel==InsParameters::twoPhaseFlowModel )
        {
            u.applyBoundaryCondition(twoPhaseFlowComponents,neumann,axisymmetric,0.,t);
        }    

    }
    if( assignSymmetry )
    {
    // symmetry BC:
        u.applyBoundaryCondition(V,vectorSymmetry,symmetry,0.,t);   
        if( assignTemperature )
        {
            u.applyBoundaryCondition(tc,BCTypes::evenSymmetry,symmetry,0.,t);
        }
        if( pdeModel==InsParameters::twoPhaseFlowModel )
        {
            u.applyBoundaryCondition(twoPhaseFlowComponents,BCTypes::evenSymmetry,symmetry,0.,t);
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
    
        if( assignFreeSurfaceBoundaryCondition )
        { // *wdh* 2014/12/24
            u.applyBoundaryCondition(V,generalizedDivergence,freeSurfaceBoundaryCondition,0.,t);
        }
        

        if( !parameters.isAxisymmetric() )
        {
            if( assignNoSlipWall )
      	u.applyBoundaryCondition(V,generalizedDivergence,noSlipWall,0.,t);
        }
        else
        {
      // div(u) = u.x + v.y + v/y = 0
      //   For y>0 and noSlipWall (u=v=0) --> u.x + v.y = 0

            if( assignNoSlipWall )
      	u.applyBoundaryCondition(V,generalizedDivergence,noSlipWall,0.,t);

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

    if( (true || numberOfDimensions!=3)  &&   // **** for now turn this off in 3D -- problesm outside interp pts*******
            applyDivergenceBoundaryCondition && assignSlipWall && orderOfAccuracy==2 ) 
    {
    // on a slip wall we need to extrapolate points that lie outside 
    // interpolation pts on the bndry. (*wdh* 061015)
        u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0, slipWall,0.,t);
        if( numberOfDimensions==3 )
            u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1, slipWall,0.,t);
        
        u.applyBoundaryCondition(V,generalizedDivergence,slipWall,0.,t); 
    }
    
    if( assignInflowWithPressureAndTangentialVelocityGiven && orderOfAccuracy==2 )
    {
    //  inflowWithPressureAndTangentialVelocityGiven
    //     give tangential velocity = 0 
    //     extrapolate (u,v,w)
    //     set div(u)=0
        if( false ) // *wdh* this is done above now 2012/09/14
        {
            u.applyBoundaryCondition(V,tangentialComponent,inflowWithPressureAndTangentialVelocityGiven,0.,t);
            if( assignTemperature )
            {
      	u.applyBoundaryCondition(tc,dirichlet,inflowWithPressureAndTangentialVelocityGiven,bcData,pBoundaryData,t,
                         				 bcParams,grid);
            }
            if( pdeModel==InsParameters::twoPhaseFlowModel )
            {
      	u.applyBoundaryCondition(twoPhaseFlowComponents,dirichlet,inflowWithPressureAndTangentialVelocityGiven,bcData,
                         				 pBoundaryData,t,bcParams,grid);
            }    

            u.applyBoundaryCondition(Rt,extrapolate,inflowWithPressureAndTangentialVelocityGiven,0.,t);
        }
        
        u.applyBoundaryCondition(V,generalizedDivergence,inflowWithPressureAndTangentialVelocityGiven,0.,t);
    }
    
  // **** if we do this we probably don't have to check for inflow points at outflow.
  // *** turn off for stretched c-grid  
  // 090221 -- SS solver can work better with div bc at outflow
  // if( applyDivergenceBoundaryCondition && orderOfAccuracy==2 )
    if( applyDivergenceBoundaryConditionAtOutflow && orderOfAccuracy==2 )
    {
        if( assignOutflow )
        {
      // 080909 -- turn this off for now : this BC can result in a boundary layer in div(u) at outflow
      //           which may actually increase div(u) overall (solid-fuel rods example). 
      // 100607 -- do not apply div(u)=0 if we expect inflow at outflow (c.f. surfaceFlow.cmd example)
      // 
            if( parameters.dbase.get<int>("outflowOption")==0 && !parameters.isAxisymmetric() &&
                    parameters.dbase.get<int >("checkForInflowAtOutFlow")!=2 ) // *wdh* 100607 
            {
        // printF("insBC: apply generalizedDivergence BC at outflow\n");
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
        const int sc = parameters.dbase.get<int >("sc");
        if( assignInflowWithVelocityGiven )
        {
            u.applyBoundaryCondition(sc,dirichlet,inflowWithVelocityGiven,bcData,pBoundaryData,t,
                         			       Overture::defaultBoundaryConditionParameters(),grid);
            u.applyBoundaryCondition(sc,extrapolate,inflowWithVelocityGiven,0.,t);
        }
        if( assignOutflow )
            u.applyBoundaryCondition(sc,extrapolate,outflow,0.,t);

        if( assignNoSlipWall )
            u.applyBoundaryCondition(sc,neumann,noSlipWall,bcData,pBoundaryData,t,
                         			       Overture::defaultBoundaryConditionParameters(),grid);
        if( assignSlipWall )
            u.applyBoundaryCondition(sc,neumann,slipWall,bcData,pBoundaryData,t,
                         			       Overture::defaultBoundaryConditionParameters(),grid);

    // ** u.applyBoundaryCondition(sc,extrapolate,allBoundaries,0.,t);
    }

  // if( true ) return 0; // * ok ---


  // extrapolate the neighbours of interpolation points -- these values are used
  // by the fourth-order artificial viscosity 

    if( orderOfAccuracy==2 )
    {
        const int discretizationWidth = mg.discretizationWidth(0);
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
            extrapParams.orderOfExtrapolation=orderOfAccuracy+1; // 3;
            u.applyBoundaryCondition(Rt,extrapolate,allBoundaries,0.,t,extrapParams);

            if( parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion") )
                extrapParams.orderOfExtrapolation=3; // orderOfAccuracy; // 3;
            else
                extrapParams.orderOfExtrapolation=3;
            
            assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
            extrapParams.orderOfExtrapolation=orderOfAccuracy; // *wdh* 100611 
            u.applyBoundaryCondition(Rt,BCTypes::extrapolateInterpolationNeighbours,allBoundaries,0.,t,extrapParams);
        }
        else if( timeSteppingMethod==Parameters::rKutta /*|| 
                                          						      parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization */)
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

        if( false &&   // *wdh* 110313 - this is done in applyFourthOrderBoundaryConditions now 
                assignTemperature )
        {
      // Order 4: Extrapolate the Temperature on the second ghost line on ALL boundaries
            extrapParams.ghostLineToAssign=2;
            extrapParams.orderOfExtrapolation=orderOfAccuracy+1; // 3;
            u.applyBoundaryCondition(tc,extrapolate,allBoundaries,0.,t,extrapParams);
            extrapParams.ghostLineToAssign=1;  // reset 
        }
        
        if( assignSlipWall )
        {
      // On a slip wall use vector symmetry on both ghost lines
            bcParams.ghostLineToAssign=1;
            u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t);   
            if( false &&   // *wdh* 110313 - this is done in applyFourthOrderBoundaryConditions now 
                    assignTemperature )
      	u.applyBoundaryCondition(tc,BCTypes::evenSymmetry, slipWall,0.,t); 

            bcParams.ghostLineToAssign=2;
            u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,bcParams);   
            bcParams.ghostLineToAssign=1;
            if( false &&   // *wdh* 110313 - this is done in applyFourthOrderBoundaryConditions now 
                    assignTemperature )
      	u.applyBoundaryCondition(tc,BCTypes::evenSymmetry,slipWall,0.,t,bcParams);
        }
            
        if( assignInflowWithPressureAndTangentialVelocityGiven )
        {
      // --- do this for now : first implementation ----
      // we should set div(u)=0 

            if( true )
            {
        // Even Symmetry on n.u :  
      	u.applyBoundaryCondition(Rt,BCTypes::evenSymmetry,inflowWithPressureAndTangentialVelocityGiven,0.,t);
            }
            else
            {
	// extrapolate 1nd ghost
      	extrapParams.ghostLineToAssign=1;
      	extrapParams.orderOfExtrapolation=orderOfAccuracy+1; 
      	u.applyBoundaryCondition(Rt,extrapolate,inflowWithPressureAndTangentialVelocityGiven,0.,t,extrapParams);
            }
            
      // set tangential components to exact on 1st ghost
            bcParams.lineToAssign=1;
            bcParams.extraInTangentialDirections=2;
            u.applyBoundaryCondition(V,tangentialComponent,inflowWithPressureAndTangentialVelocityGiven,0.,t,bcParams);
            
      // extrapolate 2nd ghost
            if( true )
            {
        // Even Symmetry on n.u :  
      	bcParams.lineToAssign=2;
      	bcParams.ghostLineToAssign=2;
      	u.applyBoundaryCondition(Rt,BCTypes::evenSymmetry,inflowWithPressureAndTangentialVelocityGiven,0.,t,bcParams);
            }
            else
            {
      	extrapParams.ghostLineToAssign=2;
      	extrapParams.orderOfExtrapolation=orderOfAccuracy+1; 
      	u.applyBoundaryCondition(Rt,extrapolate,inflowWithPressureAndTangentialVelocityGiven,0.,t,extrapParams);
            }
            
      // do this for now -- set tangential components to exact on 2nd ghost
            bcParams.lineToAssign=2;
            u.applyBoundaryCondition(V,tangentialComponent,inflowWithPressureAndTangentialVelocityGiven,0.,t,bcParams);
            bcParams.lineToAssign=0;  // reset 
            bcParams.extraInTangentialDirections=0;
            
            if( debug() & 8 )
      	::display(u,"u after inflowWithPressureAndTangentialVelocityGiven",parameters.dbase.get<FILE* >("debugFile"));

        }
        
    // This next call also assigns T for Boussinesq *wdh* 110313
        applyFourthOrderBoundaryConditions( u,t,grid,gridVelocity ); // *new* calling sequence 111124


        if( true // *wdh* 2012/07/24 -- turn this off : MAKE THIS AN OPTION 
                && assignInflowWithVelocityGiven && !parameters.dbase.get<bool >("twilightZoneFlow") )
        {
      // --- At inflow with fourth-order : set ghost values equal to the boundary values ---
      //  (There can otherwise be trouble at inflow, cf. surfaceFlow.cmd)
            OV_GET_SERIAL_ARRAY(real,u,uLocal);
        	  
            Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
            Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
            Index Ipv[3], &Ip1=Ipv[0], &Ip2=Ipv[1], &Ip3=Ipv[2];
            for( int axis=0; axis<numberOfDimensions; axis++ )
            {
      	for( int side=0; side<=1; side++ )
      	{
        	  if( mg.boundaryCondition(side,axis)==inflowWithVelocityGiven )
        	  {
	    // assign 2 ghost : 
          	    const int numGhost=2;
          	    const int extra=numGhost; // extra values in the tangential direction
	    // const int extra=0; // extra values in the tangential direction
          	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra);  
          	    bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,1);
          	    for( int ghost=1; ghost<=numGhost; ghost++ )
          	    {
            	      getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3, ghost,extra);  
            	      getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-ghost,extra);  
            	      ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,1);
            	      ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ip1,Ip2,Ip3,1);
            	      if( ok )
            	      {
		// uLocal(Ig1,Ig2,Ig3,V)=uLocal(Ib1,Ib2,Ib3,V);
            		uLocal(Ig1,Ig2,Ig3,V)=2.*uLocal(Ib1,Ib2,Ib3,V)-uLocal(Ip1,Ip2,Ip3,V); // *wdh* 2014/06/02
            	      }
            	      
          	    }
            	      
        	  }
      	}
            }
        }

    // **** assign symmetry conditions on slip walls at corner points too
    //     *** for now reassign all points on the extended region
        if( assignSlipWall )
        {
      // On a slip wall use vector symmetry on both ghost lines
            bcParams.ghostLineToAssign=1;
            bcParams.extraInTangentialDirections=2; // include 2 ghost points
            u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,bcParams);   
            if( false &&   // *wdh* 110313 - this is done in applyFourthOrderBoundaryConditions now 
                    assignTemperature )
      	u.applyBoundaryCondition(tc,BCTypes::evenSymmetry, slipWall,0.,t,bcParams); 

            bcParams.ghostLineToAssign=2;
            u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,bcParams);   
            if( false &&   // *wdh* 110313 - this is done in applyFourthOrderBoundaryConditions now 
                    assignTemperature )
      	u.applyBoundaryCondition(tc,BCTypes::evenSymmetry, slipWall,0.,t,bcParams); 
            bcParams.ghostLineToAssign=1;
            bcParams.extraInTangentialDirections=0; // reset
        }

        
        if( true && parameters.dbase.get<bool >("twilightZoneFlow") && assignDirichletBoundaryCondition )
        { 
      // ** 110314 - make sure corners are set near dirichlet BC's
      //           - also set ghost points outside interp pts 
      // We could fix applyFourthOrderBoundaryConditions to do this!

            BoundaryConditionParameters extrapParams;
            extrapParams.orderOfExtrapolation = orderOfAccuracy+1;
            const int discretizationHalfWidth = orderOfAccuracy/2;
            extrapParams.extraInTangentialDirections=2; // include 2 ghost points

            for ( int ghostLineToAssign=0; ghostLineToAssign<=discretizationHalfWidth; ghostLineToAssign++ )
            {
      	extrapParams.ghostLineToAssign = ghostLineToAssign;
      	extrapParams.lineToAssign = ghostLineToAssign;
      	if( knownSolution!=InsParameters::noKnownSolution ) 
      	{
	  // apply any known solution at dirichlet BC's   *wdh* 2013/09/28
        	  extrapParams.extraInTangentialDirections=2;
        	  extrapParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); 

        	  u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,uKnownLocal,t,extrapParams);
            
        	  extrapParams.extraInTangentialDirections=0;
        	  extrapParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

      	}
      	else
      	{
        	  u.applyBoundaryCondition(Rt,dirichlet,dirichletBoundaryCondition,0.,t,extrapParams);
      	}
      	

            }
        }
        
          

    }  // end of fourth order 
    
    

//   if( turbulenceModel==Parameters::kEpsilon )
//   {
//     // here are some fake k-epsilon equations
//     Range KE(parameters.dbase.get<int >("kc"),parameters.dbase.get<int >("epsc"));
//     u.applyBoundaryCondition(KE,dirichlet,allBoundaries,0.,t);
//     u.applyBoundaryCondition(KE,extrapolate,allBoundaries,0.,t);
//   }

  // apply turbulence model boundary conditions
    turbulenceModelBoundaryConditions(t,u,parameters,grid,pBoundaryData);

  // display(u,sPrintF(buff,"u at end of insBC grid=%i",grid),parameters.dbase.get<FILE* >("debugFile"));

  // update corners and periodic edges
  // *** not here  u.finishBoundaryConditions();

    checkArrayIDs(" insBC: end"); 


    if( orderOfAccuracy==2 ) // ** moved from generic applyBC - 060907
    {
        
        if( turbulenceModel==Parameters::noTurbulenceModel )
        {
            u.finishBoundaryConditions();
        }
        else
        {
            extrapParams.orderOfExtrapolation=1;  // low order extrapolation to keep k and eps positive
            u.finishBoundaryConditions(extrapParams);
        }
        
    }

  // 111205 add calls to bcModifier functions to adjust boundary conditions
    IntegerArray &bcInfo = parameters.dbase.get<IntegerArray>("bcInfo");
    for ( int side=0; side<2; side++ )
        for ( int axis=0; axis<cg.numberOfDimensions(); axis++ )
            {
      	if (bcInfo(2,side,axis,grid))
        	  {
          	    Parameters::BCModifier *bcMod = parameters.bcModifiers[bcInfo(2,side,axis,grid)];
          	    bcMod->applyBC(parameters, 
                     			   t, dt,
                     			   u,
                     			   grid,
                     			   side,
                     			   axis,
                     			   (gridIsMoving ? &gridVelocity : 0));
        	  }
            }

    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-time0;
    return 0;
}




