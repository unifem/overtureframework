#define TURN_OFF true

#include "Cgins.h"

#include "Parameters.h"
#include "MappedGridOperators.h"
#include "Ogmg.h"
#include "ParallelUtility.h"
#include "SparseRep.h"

#include "DeformingBodyMotion.h"
#include "BeamModel.h"

#include <float.h>

#include "turbulenceParameters.h"

#include "EquationDomain.h"


#define POW2(x) pow((x),2)

#define ForBoundary(side,axis)   for( int axis=0; axis<c.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )


//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

//\begin{>>MappedGridSolverInclude.tex}{\subsection{assignPressureRHS}} 
void Cgins::
updatePressureEquation(CompositeGrid & cg0, GridFunction & cgf )
//======================================================================
// /Description:
//\end{MappedGridSolverInclude.tex}  
//======================================================================
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return;

  if( debug() & 4 )
    printF("Cgins::updatePressureEquation t=%9.3e, dt=%8.2e...\n",cgf.t,dt);

  checkArrays("Cgins::updatePressureEquation: start");
  Overture::checkMemoryUsage("Cgins::updatePressureEquation: start");

  int & updateTimeIndependentVariables = parameters.dbase.get<int>("updateTimeIndependentVariables");
  updateTimeIndependentVariables=false;
  
  // *new* 2014/06/30 
  const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");


  CompositeGridOperators & cgop = *cgf.u.getOperators();
  

  const int & pc = parameters.dbase.get<int >("pc");

  CompositeGrid & m = cg0;

  pressureRightHandSide.updateToMatchGrid(cg0);  // could be the same as pressure for non-iterative solvers
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
  

  IntegerArray boundaryConditions(2,3,cg0.numberOfComponentGrids());     // for Oges
  boundaryConditions=0;
  RealArray boundaryConditionData(2,2,3,cg0.numberOfComponentGrids());   // for Oges
  boundaryConditionData=0.;
  
  bool singularPressureEquation=true;  // change this depending on the boundary conditions
  bool neumannBoundaryConditions=true;  // true if all BC's are neumann

  const int dirichletInterfaceCondition=parameters.numberOfBCNames+100;  // choose an unused value

  // We need to set the stencilSize etc. in case the implicit time stepper has set them
  //kkc 100216 fix for testing with compact ops  int stencilWidth = parameters.dbase.get<int >("orderOfAccuracy") + 1;
  int stencilWidth = min(4,parameters.dbase.get<int >("orderOfAccuracy")) + 1;
  int stencilSize=int( pow(stencilWidth,cg.numberOfDimensions())+1 );   // add 1 for interpolation equations
  cgop.setStencilSize(stencilSize);
  cgop.setNumberOfComponentsForCoefficients(1); // set to 1 for the pressure equation

  int grid,side,axis;
  for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg0[grid];
    
    // *wdh* 100404 -- for moving grids and multigrid there is a problem with the 
    // boundingbox in ExposedPoints -> InterpolatePointsOnAGrid since the grid associated with the mapping is built
    // on the coarsest MG level -- for now do this: 
    if( parameters.gridIsMoving(grid) && poisson->parameters.getSolverType()==OgesParameters::multigrid )
    {
      c.update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEboundingBox );
    }
    

    Parameters *pde = &parameters;
    if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
    {
      ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));  
      const int numberOfEquationDomains=equationDomainList.size();
      const int equationDomainNumber=equationDomainList.gridDomainNumberList[grid];
      assert( equationDomainNumber>=0 && equationDomainNumber<numberOfEquationDomains );
      EquationDomain & equationDomain = equationDomainList[equationDomainNumber];

      pde = equationDomain.getPDE();
    }

    real beamMassPerUnitLength[2][3]={-1.,-1.,-1.,-1.,-1.,-1.};  // For beam models
    if( useAddedMassAlgorithm && parameters.gridIsMoving(grid) )
    {
      if( cgf.t <= dt )
        printF("Cgins::updatePressureEquation: USE AMP ADDED MASS ALGORITHM grid=%i t=%8.2e\n",grid,cgf.t);

      BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid); // this will create the BDA if it is not there
      std::vector<BoundaryData> & boundaryDataArray =parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
      BoundaryData & bd = boundaryDataArray[grid];
      
      // -- extract parameters from any deforming solids ---

      MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
      
      // printF("--UPE-- grid=%i has_key  deformingBodyNumber = %i\n",grid,(int)bd.dbase.has_key("deformingBodyNumber"));
      
      if( bd.dbase.has_key("deformingBodyNumber") )
      {
	int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");
        for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
	  {
            // printF("--UPE- deformingBodyNumber[side=%i][axis=%i]=%i\n",side,axis,deformingBodyNumber[side][axis]);
	    
	    if( deformingBodyNumber[side][axis]>=0 )
	    {
              int body=deformingBodyNumber[side][axis];
	      if( cgf.t <= dt )
		printF("--UPE-- AMP: grid=%i, (side,axis)=(%i,%i) belongs to deforming body %i\n",grid,side,axis,body);

	      DeformingBodyMotion & deform = movingGrids.getDeformingBody(body);
	      if( deform.isBeamModel() )
	      {
                 
		BeamModel & beamModel = deform.getBeamModel();

		real rhosHs=-1.;
		beamModel.getMassPerUnitLength( beamMassPerUnitLength[side][axis] );
  	        if( cgf.t <= dt )
		  printF("--UPE-- AMP: BeamModel: beamMassPerUnitLength = %8.2e\n",beamMassPerUnitLength[side][axis]);

	      }
	      

	    }
	  }
	}
	

      } // end if bd.dbase.has_key("deformingBodyNumber") )
      
      
      // *** FINISH ME ***

    }  // end if useAddedMassAlgorithm
    

    ForBoundary( side,axis )
    {
      boundaryConditions(side,axis,grid)=OgesParameters::neumann;  // default

      int bc = c.boundaryCondition(side,axis);

      // *wdh* 080516 ********* what is this here for ?? ****************************************
      if( bc == Parameters::interfaceBoundaryCondition &&
          !dynamic_cast<InsParameters*>(pde) )
      {
        // here is a temporary fix for multi-domain problems
        bc=dirichletInterfaceCondition;
      }
      
      switch( bc )
      {
      case Parameters::penaltyBoundaryCondition:
      case Parameters::noSlipWall:
      case Parameters::slipWall:
      {
	if( useAddedMassAlgorithm && beamMassPerUnitLength[side][axis]>=0. )
	{
	  const real & fluidDensity = parameters.dbase.get<real >("fluidDensity");
	  assert( fluidDensity>0. );
	  
	  if( cgf.t <= dt )
	  {
		
	    printF("--UPE-- grid=%i (side,axis)=(%i,%i) Apply AMP pressure BC, t=%8.2e\n",grid,side,axis,cgf.t);
	    printF("--UPE-- Boundary is a beam, beamMassPerUnitLength = %8.2e. fluidDensity=%8.2e\n",
                    beamMassPerUnitLength[side][axis],fluidDensity);
	  }
	  
	    
	  boundaryConditions(side,axis,grid)=OgesParameters::mixed;  
	  mixedNormalCoeff(pc,side,axis,grid)=beamMassPerUnitLength[side][axis]/fluidDensity;
	  mixedCoeff(pc,side,axis,grid)=1.;

	  boundaryConditionData(0,side,axis,grid)=mixedCoeff(pc,side,axis,grid);
	  boundaryConditionData(1,side,axis,grid)=mixedNormalCoeff(pc,side,axis,grid);
	  singularPressureEquation=false;
	}

        // *** Is this next option used??
        if( (parameters.gridIsMoving(grid) && (bool)parameters.dbase.get<int>("movingBodyPressureBC")) ||
             parameters.dbase.get<int>("movingBodyPressureBC")==2 )
	{
          // *wdh* 100907 -- try this for 'light' moving bodies
          // movingBodyPressureBC==2 : for testing we apply the mixed BC on all walls. 
	  const real & a0 = parameters.dbase.get<real>("movingBodyPressureCoefficient");
	  if( a0>0. )
	  {
	    if( true )
	    {
	      printF("updatePressureEquation: set mixed pressure BC, p.n+%8.2e p = ... for `light' moving body : "
		     "(grid,side,axis)=(%i,%i,%i)\n", a0,grid,side,axis);
	    }
            boundaryConditions(side,axis,grid)=OgesParameters::mixed;  
            mixedNormalCoeff(pc,side,axis,grid)=1.;
            mixedCoeff(pc,side,axis,grid)=a0;
	    boundaryConditionData(0,side,axis,grid)=mixedCoeff(pc,side,axis,grid);
	    boundaryConditionData(1,side,axis,grid)=mixedNormalCoeff(pc,side,axis,grid);
	    singularPressureEquation=false;
	  }
	}
        break;
      }
      
      case InsParameters::inflowWithVelocityGiven:
      case Parameters::symmetry:
      case Parameters::interfaceBoundaryCondition:
      case Parameters::neumannBoundaryCondition: // kkc 100812 added this, is it correct?
        break;
      case Parameters::axisymmetric:
        boundaryConditions(side,axis,grid)=OgesParameters::axisymmetric;
        break;
      case InsParameters::inflowWithPressureAndTangentialVelocityGiven:
      case Parameters::dirichletBoundaryCondition:
      case Parameters::freeSurfaceBoundaryCondition:  // *new* 2012/11/24
        singularPressureEquation=false;
	neumannBoundaryConditions=false;
        boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  
        break;
      case InsParameters::outflow:
      case InsParameters::tractionFree:
      case InsParameters::convectiveOutflow:
        // pressure equation is still singular with a mixed BC if alpha=0. (alpha*p+beta*p.n=)

        assert( mixedCoeff(pc,side,axis,grid)!=0. || mixedNormalCoeff(pc,side,axis,grid)!=0. );
	
        singularPressureEquation=singularPressureEquation && mixedCoeff(pc,side,axis,grid)==0. ;
	neumannBoundaryConditions=neumannBoundaryConditions &&
               mixedCoeff(pc,side,axis,grid)==0. && mixedNormalCoeff(pc,side,axis,grid)==1.;

        if( mixedCoeff(pc,side,axis,grid)==0. && mixedNormalCoeff(pc,side,axis,grid)==1. )
	{
          boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
        }
	else
	{
          boundaryConditions(side,axis,grid)=OgesParameters::mixed;  
          boundaryConditionData(0,side,axis,grid)=mixedCoeff(pc,side,axis,grid);
	  boundaryConditionData(1,side,axis,grid)=mixedNormalCoeff(pc,side,axis,grid);
          if( debug() & 4 )
            printF("*****updatePressureEquation: mixed BC: %f*p+%f*p.n \n",
		 mixedCoeff(pc,side,axis,grid), mixedNormalCoeff(pc,side,axis,grid));
	  
	}
//         singularPressureEquation=singularPressureEquation && 
//                                  bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid)==0. ;
// 	neumannBoundaryConditions=neumannBoundaryConditions &&
//                bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid)==0. && 
//                bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid)==1.;

//         if( bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid)==0. && 
//             bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid)==1. )
// 	{
//           boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
//         }
// 	else
// 	{
//           boundaryConditions(side,axis,grid)=OgesParameters::mixed;  
//           boundaryConditionData(0,side,axis,grid)=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid);
// 	  boundaryConditionData(1,side,axis,grid)=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid);
//           //           printF("*****updatePressureEquation: mixed BC: %f*p+%f*p.n \n",
//           //                 boundaryConditionData(0,side,axis,grid),boundaryConditionData(1,side,axis,grid));
	  
// 	}
	
        break;
      default:
	// kkc 070131      case dirichletInterfaceCondition:
	if ( bc==dirichletInterfaceCondition ) {
	  // this is a dirichlet BC for a fake region -- do not adjust the singular nature of the problem
	  boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  
	} else {
	  boundaryConditions(side,axis,grid)=c.boundaryCondition(side,axis);
	  if( c.boundaryCondition(side,axis) > 0 )
	    {
	      cout << "INS::updatePressureEquation:ERROR unknown BC value! \n";
	      printF("cg0[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i\n",grid,side,axis,
		     c.boundaryCondition(side,axis));
	      OV_ABORT("INS::updatePressureEquation ERROR unknown BC value");
	    }
	}
      }
    }
  }

  // If the initial conditions were projected then the pressure equation has already been
  // created with Neumann BC's -- there is no need to regenerate and factor the matrix
  // if we have the same BC's for the flow problem  
  if(  parameters.dbase.get<int>("initialConditionsAreBeingProjected")==0 &&  // we are not now projecting the IC's
       neumannBoundaryConditions && parameters.dbase.get<bool >("projectInitialConditions") && 
      !parameters.isMovingGridProblem()  )
  {
    printF("**** Pressure matrix NOT regenerated since it is the same as the projection matrix ****\n");
  }
  else
  {

// ***** *wdh* 2014/03/31 -- this check is no longer needed *****
    // BoundaryConditionParameters bcParams;
    // RealArray & a = bcParams.a;
    // a.redim(2);
//     for( int l=0; l<m.numberOfMultigridLevels(); l++ )
//     {
//       CompositeGrid & cg = m.numberOfMultigridLevels()==1 ? cg0 : m.multigridLevel[l];
//       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	MappedGrid & c = cg[grid];
// 	// For outflow etc. boundaries check whether the pressure BC a*p+b*p.n is neumann (b!=0) or dirichlet
//         // *** for now we either apply a mixed or dirichlet BC at all outflow boundaries on a given component grid ****
// 	for( int bcType=0; bcType<3; bcType++ )
// 	{
//           // we check the three BC's that specify a mixed condition on the pressure
// 	  int bc = 
// 	    bcType==0 ? InsParameters::outflow : 
// 	    bcType==1 ? InsParameters::convectiveOutflow :
// 	                InsParameters::tractionFree;

//   	  int typeOfBoundaryCondition=-1;  // -1=no outflow boundaries, 0=dirichlet, 1=neumann
// 	  ForBoundary( side,axis )
// 	  {
// 	    if( c.boundaryCondition(side,axis)==(int)bc )
// 	    {
// 	      if( mixedNormalCoeff(pc,side,axis,grid)!=0. )
// 	      {
// 		if( typeOfBoundaryCondition!=0 )
// 		  typeOfBoundaryCondition=1;  // neumann
// 		else
// 		  typeOfBoundaryCondition=2;  // error
// 		a(0)=mixedCoeff(pc,side,axis,grid);
// 		a(1)=mixedNormalCoeff(pc,side,axis,grid);
// 	      }
// 	      else
// 	      {
// 		if( typeOfBoundaryCondition!=1 )
// 		  typeOfBoundaryCondition=0;  // dirichlet
// 		else
// 		  typeOfBoundaryCondition=2;  // error
// 		a(0)=mixedCoeff(pc,side,axis,grid);
// 		a(1)=mixedNormalCoeff(pc,side,axis,grid);
// 	      }
// 	      if( typeOfBoundaryCondition==2 )
// 	      {
// 		printF("updatePressureEquation:ERROR: in assign boundary conditions for coeff. matrix \n"
// 		       "there are two outflow/convectiveOutflow/tractionFree boundaries on a component grid "
// 		       "with one a mixed and one a dirichlet BC\n");
// 		printF(" Ask Bill to fix this! \n");
// 		OV_ABORT("error");
// 	      }
// 	    }
// 	  }
// // 	  if( typeOfBoundaryCondition==1 )
// // 	  {
// // 	    // mixed or neumann BC
// //             if( parameters.dbase.get<int >("debug") & 2 ) printF("Apply mixed BC on pressure for bc=%i\n",(int)bc);
// // 	    poissonCoeff.applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,bc,bcParams);
// // 	  }
// // 	  else if( typeOfBoundaryCondition==0 )
// // 	  {
// //             if( parameters.dbase.get<int >("debug") & 2 ) printF("Apply dirichlet BC on pressure for bc=%i\n",(int)bc);
// // 	    poissonCoeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,bc);
// // 	    poissonCoeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,bc);
// // 	  }
// 	}
//       }
//     }// end for( l )
// ***********
    
    // *** poissonCoefficients.applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,parameters.dbase.get< >("outflow"),bcParams);

//     poissonCoefficients.finishBoundaryConditions();
//     if( FALSE )
//     {
//       grid=0;
//       display(poissonCoefficients[grid],"poissonCoefficients[0]",parameters.dbase.get<FILE* >("debugFile"));
//       display(cg0.interpolationPoint[grid],"cg.interpolationPoint[grid]",parameters.dbase.get<FILE* >("debugFile"));
//       display(cg0.interpoleeGrid[grid],"cg.interpoleeGrid[grid]",parameters.dbase.get<FILE* >("debugFile"));
//       display(cg0.interpoleeLocation[grid],"cg.interpoleeLocation[grid]",parameters.dbase.get<FILE* >("debugFile"));
//       display(cg0.interpolationCoordinates[grid],"cg.interpolationCoordinates[grid]",parameters.dbase.get<FILE* >("debugFile"));
//       displayMask(cg0[grid].mask(),"mask",parameters.dbase.get<FILE* >("debugFile"));
//     }
    


    // ----Set parameters for Poisson Solver
    assert( poisson!=0 );
    if( poisson!=0 ) 
    {

      if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::rKutta ) // *wdh*
      {
         // destroy the coefficient matrix after use
         // poissonCoefficients.destroy();
      }
      
      bool outOfDate = false;  // We indicate when the grid changes in updateForMovingGrids
      poisson->setGrid( cg0,outOfDate ); 


      if( parameters.isAxisymmetric() )
	poisson->set(OgesParameters::THEisAxisymmetric,true);

      const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
      const bool solveVariableDensityPoisson = pdeModel==InsParameters::twoPhaseFlowModel;
      
      if( solveVariableDensityPoisson )
      {
        // Solve div( (1/rho) grad ) p = ...
	const int & rc = parameters.dbase.get<int >("rc");
	assert( rc>=0 );

	Index I1,I2,I3;
        realCompositeGridFunction rhoInverse(cg);  // fix this
 	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
 	{
          realArray & u = cgf.u[grid];
          #ifdef USE_PPP
           realSerialArray uLocal;     getLocalArrayWithGhostBoundaries(u,uLocal);
           realSerialArray rhoiLocal;  getLocalArrayWithGhostBoundaries(rhoInverse[grid],rhoiLocal);
          #else
           const realSerialArray & uLocal = u;
           const realSerialArray & rhoiLocal = rhoInverse[grid];
          #endif
	  getIndex(cg[grid].dimension(),I1,I2,I3);
          bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
	  if( ok )
	  {
	    rhoiLocal(I1,I2,I3) = 1./uLocal(I1,I2,I3,rc);
	  }
	}

        printF(" PPPPPPPP Cgins::updatePressureEquation form div( (1/rho) grad(p))=F,  t=%9.3e...\n\n",cgf.t);

	OgesParameters::EquationEnum equation = OgesParameters::divScalarGradOperator;
        poisson->setEquationAndBoundaryConditions(equation,cgop,boundaryConditions, boundaryConditionData,
                                                  Overture::nullRealArray(),&rhoInverse);
      }
      else
      {
	OgesParameters::EquationEnum equation = OgesParameters::laplaceEquation;
	poisson->setEquationAndBoundaryConditions(equation,cgop,boundaryConditions, boundaryConditionData );
      }
      
      // for variable density use
      // equation=divScalarGradOperator,              // div( s(x) grad )
//       setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, 
// 					CompositeGridOperators & op,
// 					const IntegerArray & boundaryConditions_,
// 					const RealArray & bcData_, 
// 					RealArray & constantCoeff,
// 					realCompositeGridFunction *varCoeff /* =NULL */ )

      if( useAddedMassAlgorithm )
      {
        // Make any adjustments to the equations needed for the added mass algorithm
        adjustPressureCoefficients( cg0, cgf );
      }
      



      if( false )
      {
	realCompositeGridFunction & coeff = poisson->coeff;
        coeff.display("coeff");
        for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
	{
	  const intMappedGridFunction & classify = coeff[grid].sparse->classify;
	  ::display(classify,"classify");
	}
      }
      

      poisson->set(OgesParameters::THEkeepCoefficientGridFunction,FALSE); 
    }
  }
  
  // compute the weight term for the divergence in the pressure equation
  int geometryHasChanged=true;
  updateDivergenceDamping( cg0,geometryHasChanged );

  checkArrays("Cgins::updatePressureEquation: end");
  Overture::checkMemoryUsage("Cgins::updatePressureEquation: end");

}





//   ...weight divergence term : 1/dx^2 + 1/dy^2 + 1/dz^2  ******************* compute and save this *******
#define DAI1(cd,I1,I2,I3)  ( \
        cd/ max(realSmall,\
            ( POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))  \
             +POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1)) ) ) )

#define DAI2(cd,I1,I2,I3)  ( \
        cd/max(realSmall,\
            ( POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))  \
             +POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1)) ) ) \
      + cd/max(realSmall,  \
            ( POW2(xy(I1  ,I2+1,I3  ,0)-xy(I1  ,I2-1,I3  ,0))  \
             +POW2(xy(I1  ,I2+1,I3  ,1)-xy(I1  ,I2-1,I3  ,1)) ) ) )
#define DAI3(cd,I1,I2,I3)  ( \
        cd/max(realSmall,  \
            ( POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))  \
             +POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1))  \
             +POW2(xy(I1+1,I2  ,I3  ,2)-xy(I1-1,I2  ,I3  ,2)) ) ) \
      + cd/max(realSmall, \
            ( POW2(xy(I1  ,I2+1,I3  ,0)-xy(I1  ,I2-1,I3  ,0))  \
             +POW2(xy(I1  ,I2+1,I3  ,1)-xy(I1  ,I2-1,I3  ,1))  \
             +POW2(xy(I1  ,I2+1,I3  ,2)-xy(I1  ,I2-1,I3  ,2)) ) ) \
      + cd/max(realSmall, \
            ( POW2(xy(I1  ,I2  ,I3+1,0)-xy(I1  ,I2  ,I3-1,0))  \
             +POW2(xy(I1  ,I2  ,I3+1,1)-xy(I1  ,I2  ,I3-1,1))  \
             +POW2(xy(I1  ,I2  ,I3+1,2)-xy(I1  ,I2  ,I3-1,2)) ) ) )


//\begin{>>MappedGridSolverInclude.tex}{\subsection{updateDivergenceDamping}} 
void Cgins::
updateDivergenceDamping( CompositeGrid & cg0, const int & geometryHasChanged )
//==========================================================================================================
// /Description:
// parameters.dbase.get<real >("dampingDt") : time step last used when computing the divergence damping.
//
// divDampingWeight =min( cdvnu*( 1./SQR(2.*dx[0])+ 1./SQR(2.*dx[1]) ), cDtDt );
// where
//    cDtDt= cDt/dt
//    cdvmu = cdv*max(nu,hMin[grid])*4/nd;
//
// NOTE: set cDt==0 to turn off this limit
// 
//\end{MappedGridSolverInclude.tex}  
//=========================================================================================================
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return;

  if( debug() & 4 )
  {
    // dt= 2.00e-02;  // ************ TEMP
    printF(" >>> Cgins: updateDivergenceDamping cdv=%8.2e dt=%8.2e <<<\n",parameters.dbase.get<real >("cdv"),dt);
  }
  
  // +++++++++++++++++++++++++++++++++++++++++++++=
  if( cg0.numberOfComponentGrids() > dtv.size() ) 
  {
    dtv.resize(cg0.numberOfComponentGrids(),dt);
//    hMin.resize(cg0.numberOfComponentGrids(),0.);
  }
  for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
  {
    dtv[grid]=dt;
//    hMin[grid]=mappedGridSolver[grid]->hMin;
  }

// +++++++++++++++++++++++++++++++++++++++++++++=

  // compute the weight term for the divergence in the pressure equation
  // ***** no need to recompute for solid body rotation/transation *************************

  const real realSmall= REAL_MIN*1.e5;
  
  const real & nu = parameters.dbase.get<real >("nu");
  const real & cdv = parameters.dbase.get<real >("cdv");

  // ****NOTE: this should use the variable time step ************

  assert( dtv.size()>0 );
  real dt=dtv[0]; // do this for now

  real & dampingDt = parameters.dbase.get<real >("dampingDt");
  if( geometryHasChanged || dt>1.5*dampingDt || dt<dampingDt/1.5 )
  {
    // recompute the divergence damping term.
    if( debug() & 4 )
      printF(" xxxxxxxx recompute the divergence damping term, dt=%9.2e, dampingDt=%9.2e, nu=%9.2e, cdv=%9.2e \n",
              dt,dampingDt,nu,cdv);
    dampingDt=dt;
    
    const real cDt = parameters.dbase.get<real >("cDt");
    
    real cDtDt= dt>0. ? cDt/dt : REAL_MAX;
    if( cDtDt <= 0. )
    {  // turn off this limit if cDt<=0
      cDtDt=REAL_MAX*.01;
    }

    if( parameters.isAxisymmetric() && parameters.dbase.get<real >("advectionCoefficient")>0. )
    { // 040228
      cDtDt*=4;
    }
    
// * try this ?    
//     real cDtDt= REAL_MAX;
//     if( dt>0. && 
//         (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit ||
//         parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicitAllSpeed) )
//     {
//       cDtDt=parameters.dbase.get<real >("cDt")/dt;
//       printF(" ----divergence damping coefficient limited for implicit method by the value cDtDt=%e\n",cDtDt);
//     }
    
    divDampingWeight.updateToMatchGrid(cg0);

    Index I1,I2,I3;
    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      realMappedGridFunction & divergenceDampingWeight = divDampingWeight[grid];
      getIndex(cg0[grid].dimension(),I1,I2,I3,-1);

      assert( grid < hMin.size() && hMin[grid]>=0 );  // this needs to be computed
      
      const int nd=parameters.dbase.get<int >("compare3Dto2D") ? min(2,cg0.numberOfDimensions()) : cg0.numberOfDimensions();
      // *** FIX ME 2015/03/22 : Not dimensionally correct to compare nu to hMin !!
      real cdvnu=cdv*max(nu,hMin[grid])*4/nd;

      #ifdef USE_PPP
        realSerialArray ddw; getLocalArrayWithGhostBoundaries( divergenceDampingWeight,ddw);
      #else
        realSerialArray & ddw = divergenceDampingWeight;
      #endif
      #ifdef USE_PPP
        // restrict bounds to local processor, do not include ghost
        bool ok = ParallelUtility::getLocalArrayBounds(divergenceDampingWeight,ddw,I1,I2,I3,0);   
        if( !ok ) continue;  // no points on this processor
      #endif

      if( cg0[grid].isRectangular() )
      {
	real dx[3];
	cg0[grid].getDeltaX( dx );
	// printF(" ***** insp: dx for rectangular grid = [%e,%e,%e] cDtDt=%6.2e cdvnu=%6.2e\n",dx[0],dx[1],dx[2],
        //  cDtDt,cdvnu);
	  
        if( cg0[grid].numberOfDimensions()==1 )
	  ddw(I1,I2,I3)=min( cdvnu*( 1./SQR(2.*dx[0]) ), cDtDt );
        else if( cg0[grid].numberOfDimensions()==2 || parameters.dbase.get<int >("compare3Dto2D") )
	  ddw(I1,I2,I3)=min( cdvnu*( 1./SQR(2.*dx[0])+ 1./SQR(2.*dx[1]) ), cDtDt );
        else 
    	  ddw(I1,I2,I3)=min( cdvnu*( 1./SQR(2.*dx[0])+1./SQR(2.*dx[1])+1./SQR(2.*dx[2]) ), cDtDt );
      }
      else
      {
        // updated for P++ 060928 *wdh*
        // realArray & xy = cg0[grid].center();
        cg0[grid].update(MappedGrid::THEcenter);
	
        #ifdef USE_PPP
          realSerialArray xy; getLocalArrayWithGhostBoundaries(cg0[grid].center(),xy);
        #else
          realSerialArray & xy = cg0[grid].center();
        #endif
        #ifdef USE_PPP
          // restrict bounds to local processor, do not include ghost
          bool ok = ParallelUtility::getLocalArrayBounds(divergenceDampingWeight,ddw,I1,I2,I3,0);   
          if( !ok ) continue;  // no points on this processor
        #endif

	if( cg0[grid].numberOfDimensions()==1 )
	  ddw(I1,I2,I3)=min( cdvnu*DAI1(1.,I1,I2,I3), cDtDt );
	else if( cg0[grid].numberOfDimensions()==2 || 
		 parameters.dbase.get<int >("compare3Dto2D") )  // *** we use a 2D divergence for comparing 3d to 2d
	{
          // printF("updateDivergenceDamping: grid=%i, cdv=%e\n",grid, cdv);
	  // ::display(ddw,"ddw","%3.1f ");
	  // ::display(xy,"xy","%3.1f ");
	  
          RealArray temp1(I1,I2,I3),temp2(I1,I2,I3);
          temp1 = POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))+
	          POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1));
	  temp1 = 1./(max(realSmall,temp1));
          temp2 = POW2(xy(I1  ,I2+1,I3  ,0)-xy(I1  ,I2-1,I3  ,0))+
	          POW2(xy(I1  ,I2+1,I3  ,1)-xy(I1  ,I2-1,I3  ,1));
	  temp2 = 1./(max(realSmall,temp2));
	  temp1 += temp2;
	  
// #define DAI2(cd,I1,I2,I3)  ( \
//         cd/max(realSmall,\
//             ( POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))  \
//              +POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1)) ) ) \
//       + cd/max(realSmall,  \
//             ( POW2(xy(I1  ,I2+1,I3  ,0)-xy(I1  ,I2-1,I3  ,0))  \
//              +POW2(xy(I1  ,I2+1,I3  ,1)-xy(I1  ,I2-1,I3  ,1)) ) ) )	  

    	  ddw(I1,I2,I3)=min( cdvnu*temp1, cDtDt );

//  	  ddw(I1,I2,I3)=min( cdvnu*DAI2(1.,I1,I2,I3), cDtDt );

//           printF(" divergenceDampingWeight, max=%e, min=%e\n",max(divergenceDampingWeight(I1,I2,I3)),
//                min(divergenceDampingWeight(I1,I2,I3)));
	}
	else
	  ddw(I1,I2,I3)=min( cdvnu*DAI3(1.,I1,I2,I3), cDtDt );
	
      }
      // printF(" divergenceDampingWeight, max=%e, min=%e\n",max(divergenceDampingWeight(I1,I2,I3)),
      //         min(divergenceDampingWeight(I1,I2,I3)));
      
    }
  }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef TURN_OFF
  for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
  {
    assert( mappedGridSolver[grid]->pdivergenceDampingWeight!=NULL );
    
    realMappedGridFunction & divergenceDampingWeight = mappedGridSolver[grid]->divergenceDampingWeight();
    divergenceDampingWeight.updateToMatchGrid(cg0[grid]);
    divergenceDampingWeight=divDampingWeight[grid];
  }
#endif
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

