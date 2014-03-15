// ********** temporary fixup file **************


#include "OB_CompositeGridSolver.h"
#include "OB_MappedGridSolver.h"
#include "Parameters.h"
#include "MappedGridOperators.h"
#include "Ogmg.h"
#include "ParallelUtility.h"
#include "SparseRep.h"

#include <float.h>

#include "turbulenceParameters.h"
#include "viscoPlasticMacrosCpp.h"

#include "EquationDomain.h"
extern ListOfEquationDomains equationDomainList; // This is in the global name space for now.
#include "SurfaceEquation.h"
extern SurfaceEquation surfaceEquation;  // This is in the global name space for now.

// Put this here for now:
#include "Cgins.h"
extern DomainSolver *pDomainSolver;


void
getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                     IntegerArray & gidLocal, 
                                     IntegerArray & dimensionLocal, 
                                     IntegerArray & bcLocal );


#define POW2(x) pow((x),2)

#define ForBoundary(side,axis)   for( int axis=0; axis<c.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )


#define U(c)     u(I1,I2,I3,c)   
#define UU(c)   uu(I1,I2,I3,c)
#define UX(c)   ux(I1,I2,I3,c)
#define UY(c)   uy(I1,I2,I3,c)
#define UZ(c)   uz(I1,I2,I3,c)
#define UXX(c) uxx(I1,I2,I3,c)
#define UXY(c) uxy(I1,I2,I3,c)
#define UXZ(c) uxz(I1,I2,I3,c)
#define UYY(c) uyy(I1,I2,I3,c)
#define UYZ(c) uyz(I1,I2,I3,c)
#define UZZ(c) uzz(I1,I2,I3,c)

// ***** fix this for real case ****
// define utOnBoundary(c,side,axis,I1,I2,I3,component,t) e.t(c,I1,I2,I3,component,t)
  
//     normal derivative of p (outward normal)
#define PN1(I1,I2,I3)  ( (2*side-1)*(  nu*uxx(I1,I2,I3,uc) \
                         + advectionCoefficient*uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) ) )

#define PXB2(I1,I2,I3) ( nu*(uxx(I1,I2,I3,uc)+uyy(I1,I2,I3,uc)) \
                         +advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc)) )

#define PYB2(I1,I2,I3) ( nu*(uxx(I1,I2,I3,vc)+uyy(I1,I2,I3,vc))   \
                         +advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc)) )

//     normal derivative of p (outward normal)
#define PN2(I1,I2,I3)  ( normal(I1,I2,I3,0)*PXB2(I1,I2,I3)  \
                        +normal(I1,I2,I3,1)*PYB2(I1,I2,I3) )



//  ...momentum eqn's in 3d without grad p term
#define DELTAU(I1,I2,I3,dir) (uxx(I1,I2,I3,dir)+uyy(I1,I2,I3,dir)+uzz(I1,I2,I3,dir))

#define P3B(I1,I2,I3,dir) ( nu*DELTAU(I1,I2,I3,dir) \
                          +advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,dir) \
                                                + uu(I1,I2,I3,vc)*uy(I1,I2,I3,dir) \
                                                + uu(I1,I2,I3,wc)*uz(I1,I2,I3,dir)) )

//    ...normal derivative of p in 3d (outward normal)
#define PN3(I1,I2,I3) ( normal(I1,I2,I3,0)*P3B(I1,I2,I3,uc)  \
                       +normal(I1,I2,I3,1)*P3B(I1,I2,I3,vc)  \
                       +normal(I1,I2,I3,2)*P3B(I1,I2,I3,wc) )


#define realSmall (REAL_MIN*1.e5)

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

#define D2UW2(I1,I2,I3) (UW(I1+1,I2,I3)+UW(I1-1,I2,I3)  \
                        +UW(I1,I2+1,I3)+UW(I1,I2-1,I3)-4.*UW(I1,I2,I3))
                                                                     


// --------- define the artificial diffusions ------------
//
//       [ad21+ad22* |grad\uv|] ( D+rD-r(u) dr**2 + D+sD-s(u) ds**2 )
//           cd22=ad22/nd**2
//    ---2D:

#define AD2(kd) (  \
        (ad21 + cd22*    \
         ( fabs(UX(uc))+fabs(UY(uc))    \
          +fabs(UX(vc))+fabs(UY(vc)) ) )    \
         *(u(I1+1,I2,I3,kd)-4.*u(I1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
          +u(I1,I2+1,I3,kd)                  +u(I1,I2-1,I3,kd))    \
                         )    \
                 
//  ---3D:
#define  AD23(kd)  (    \
        (ad21 + cd22*    \
         ( fabs(UX(uc))+fabs(UY(uc))+fabs(UZ(uc))    \
          +fabs(UX(vc))+fabs(UY(vc))+fabs(UZ(vc))    \
          +fabs(UX(wc))+fabs(UY(wc))+fabs(UZ(wc)) ) )    \
         *(u(I1+1,I2,I3,kd)-6.*u(I1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
          +u(I1,I2+1,I3,kd)                   +u(I1,I2-1,I3,kd)    \
          +u(I1,I2,I3+1,kd)                   +u(I1,I2,I3-1,kd))    \
                            )
                       
//  ---fourth-order artficial diffusion in 2D
#define AD4(kd) (    \
        (ad41 + cd42*    \
         ( fabs(UX(uc))+fabs(UY(uc))    \
          +fabs(UX(vc))+fabs(UY(vc)) ) )    \
         *(   -u(I1+2,I2,I3,kd)-u(I1-2,I2,I3,kd)    \
              -u(I1,I2+2,I3,kd)-u(I1,I2-2,I3,kd)    \
          +4.*(u(I1+1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
              +u(I1,I2+1,I3,kd)+u(I1,I2-1,I3,kd))    \
           -12.*u(I1,I2,I3,kd) )    \
                         )
//   ---fourth-order artficial diffusion in 3D
#define AD43(kd) (    \
        (ad41 + cd42*    \
         ( fabs(UX(uc))+fabs(UY(uc))+fabs(UZ(uc))    \
          +fabs(UX(vc))+fabs(UY(vc))+fabs(UZ(vc))    \
          +fabs(UX(wc))+fabs(UY(wc))+fabs(UZ(wc)) ) )    \
         *(   -u(I1+2,I2,I3,kd)-u(I1-2,I2,I3,kd)    \
              -u(I1,I2+2,I3,kd)-u(I1,I2-2,I3,kd)    \
              -u(I1,I2,I3+2,kd)-u(I1,I2,I3-2,kd)    \
          +4.*(u(I1+1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
              +u(I1,I2+1,I3,kd)+u(I1,I2-1,I3,kd)    \
              +u(I1,I2,I3+1,kd)+u(I1,I2,I3-1,kd))    \
           -18.*u(I1,I2,I3,kd) )    \
                          )


//\begin{>>MappedGridSolverInclude.tex}{\subsection{assignPressureRHS}} 
void OB_CompositeGridSolver::
updatePressureEquation(CompositeGrid & cg0, CompositeGridOperators & cgop)
//======================================================================
// /Description:
//\end{MappedGridSolverInclude.tex}  
//======================================================================
{
  if( false && pDomainSolver!=NULL )
  {
    // call the version in Cgins too
    Cgins & cgins = (Cgins&)(*pDomainSolver);
    if( cg0.numberOfComponentGrids() > cgins.dtv.size() ) 
    {
      cgins.dtv.resize(cg0.numberOfComponentGrids(),dt);
      cgins.hMin.resize(cg0.numberOfComponentGrids(),0.);
    }
    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      cgins.dtv[grid]=dt;
      cgins.hMin[grid]=mappedGridSolver[grid]->hMin;
    }
    
    cgins.updatePressureEquation(cg0,cgop);
  }

  if( debug() & 2 )
    printf("OB_CompositeGridSolver:updatePressureEquation...\n");

  checkArrays(" updatePressureEquation: start");

  const int & pc = parameters.dbase.get<int >("pc");

  CompositeGrid & m = cg0;

  pressureRightHandSide.updateToMatchGrid(cg0);  // could be the same as pressure for non-iterative solvers
  
  IntegerArray boundaryConditions(2,3,cg0.numberOfComponentGrids());  // for Oges
  boundaryConditions=0;
  RealArray boundaryConditionData(2,2,3,cg0.numberOfComponentGrids());               // for Oges
  boundaryConditionData=0.;
  
  bool singularPressureEquation=true;  // change this depending on the boundary conditions
  bool neumannBoundaryConditions=true;  // true if all BC's are neumann

  const int dirichletInterfaceCondition=Parameters::numberOfBCNames+100;  // choose an unused value

  int grid,side,axis;
  for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg0[grid];
    
    const int numberOfEquationDomains=equationDomainList.size();
    const int equationDomainNumber=equationDomainList.gridDomainNumberList[grid];
    assert( equationDomainNumber>=0 && equationDomainNumber<numberOfEquationDomains );
    EquationDomain & equationDomain = equationDomainList[equationDomainNumber];

    const Parameters::PDE pde = equationDomain.getPDE();


    ForBoundary( side,axis )
    {
      boundaryConditions(side,axis,grid)=OgesParameters::neumann;  // default

      int bc = c.boundaryCondition(side,axis);
      if( bc == Parameters::interfaceBoundaryCondition &&
          pde!=Parameters::incompressibleNavierStokes )
      {
        // here is a temporary fix for multi-domain problems
        bc=dirichletInterfaceCondition;
      }
      
      switch( bc )
      {
      case Parameters::noSlipWall:
      case Parameters::inflowWithVelocityGiven:
      case Parameters::slipWall:
      case Parameters::symmetry:
      case Parameters::interfaceBoundaryCondition:
        break;
      case Parameters::axisymmetric:
        boundaryConditions(side,axis,grid)=OgesParameters::axisymmetric;
        break;
      case Parameters::inflowWithPressureAndTangentialVelocityGiven:
      case Parameters::dirichletBoundaryCondition:
        singularPressureEquation=false;
	neumannBoundaryConditions=false;
        boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  
        break;
      case dirichletInterfaceCondition:
        // this is a dirichlet BC for a fake region -- do not adjust the singular nature of the problem
        boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  
        break;
      case Parameters::outflow:
      case Parameters::tractionFree:
      case Parameters::convectiveOutflow:
        // pressure equation is still singular with a mixed BC if alpha=0. (alpha*p+beta*p.n=)
        singularPressureEquation=singularPressureEquation && 
                                 parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid)==0. ;
	neumannBoundaryConditions=neumannBoundaryConditions &&
               parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid)==0. && 
               parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid)==1.;

        if( parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid)==0. && 
            parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid)==1. )
	{
          boundaryConditions(side,axis,grid)=OgesParameters::neumann;  
        }
	else
	{
          boundaryConditions(side,axis,grid)=OgesParameters::mixed;  
          boundaryConditionData(0,side,axis,grid)=parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid);
	  boundaryConditionData(1,side,axis,grid)=parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid);
//           printf("*****updatePressureEquation: mixed BC: %f*p+%f*p.n \n",
//                 boundaryConditionData(0,side,axis,grid),boundaryConditionData(1,side,axis,grid));
	  
	}
	
        break;
      default:
        boundaryConditions(side,axis,grid)=c.boundaryCondition(side,axis);
        if( c.boundaryCondition(side,axis) > 0 )
	{
  	  cout << "INS::updatePressureEquation:ERROR unknown BC value! \n";
          printf("cg0[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i\n",grid,side,axis,
		 c.boundaryCondition(side,axis));
          throw "INS::updatePressureEquation ERROR unknown BC value";
	}
      }
    }
  }

  // If the initial conditions were projected then the pressure equation has already been
  // created with Neumann BC's -- there is no need to regenerate and factor the matrix
  // if we have the same BC's for the flow problem  
  if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::incompressibleNavierStokes &&   // ** new way **
      neumannBoundaryConditions && parameters.dbase.get<bool >("projectInitialConditions") && !movingGridProblem()  )
  {
    cout << "**** Pressure matrix NOT regenerated since it is the same as the projection matrix ****\n";
  }
  else
  {

    RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
    BoundaryConditionParameters bcParams;
    RealArray & a = bcParams.a;
    a.redim(2);

    for( int l=0; l<m.numberOfMultigridLevels(); l++ )
    {
      CompositeGrid & cg = m.numberOfMultigridLevels()==1 ? cg0 : m.multigridLevel[l];
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];
	// For outflow etc. boundaries check whether the pressure BC a*p+b*p.n is neumann (b!=0) or dirichlet
        // *** for now we either apply a mixed or dirichlet BC at all outflow boundaries on a given component grid ****
	for( int bcType=0; bcType<3; bcType++ )
	{
          // we check the three BC's that specify a mixed condition on the pressure
	  Parameters::BoundaryCondition bc = 
	    bcType==0 ? Parameters::outflow : 
	    bcType==1 ? Parameters::convectiveOutflow :
	                Parameters::tractionFree;

  	  int typeOfBoundaryCondition=-1;  // -1=no outflow boundaries, 0=dirichlet, 1=neumann
	  ForBoundary( side,axis )
	  {
	    if( c.boundaryCondition(side,axis)==(int)bc )
	    {
	      if( bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid)!=0. )
	      {
		if( typeOfBoundaryCondition!=0 )
		  typeOfBoundaryCondition=1;  // neumann
		else
		  typeOfBoundaryCondition=2;  // error
		a(0)=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid);
		a(1)=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid);
	      }
	      else
	      {
		if( typeOfBoundaryCondition!=1 )
		  typeOfBoundaryCondition=0;  // dirichlet
		else
		  typeOfBoundaryCondition=2;  // error
		a(0)=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid);
		a(1)=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid);
	      }
	      if( typeOfBoundaryCondition==2 )
	      {
		printf("updatePressureEquation:ERROR: in assign boundary conditions for coeff. matrix \n"
		       "there are two outflow/convectiveOutflow/tractionFree boundaries on a component grid "
		       "with one a mixed and one a dirichlet BC\n");
		printf(" Ask Bill to fix this! \n");
		throw "error";
	      }
	    }
	  }
// 	  if( typeOfBoundaryCondition==1 )
// 	  {
// 	    // mixed or neumann BC
//             if( parameters.dbase.get<int >("debug") & 2 ) printf("Apply mixed BC on pressure for bc=%i\n",(int)bc);
// 	    poissonCoeff.applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,bc,bcParams);
// 	  }
// 	  else if( typeOfBoundaryCondition==0 )
// 	  {
//             if( parameters.dbase.get<int >("debug") & 2 ) printf("Apply dirichlet BC on pressure for bc=%i\n",(int)bc);
// 	    poissonCoeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,bc);
// 	    poissonCoeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,bc);
// 	  }
	}
      }
    }
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
//          poissonCoefficients.destroy();
      }
      

      poisson->setGrid( cg0 ); 

      OgesParameters::EquationEnum equation = parameters.isAxisymmetric() ? 
                 OgesParameters::axisymmetricLaplaceEquation :
                 OgesParameters::laplaceEquation;

      // printf(" *** insp: cgop:orderOfAccuracy=%i\n",cgop[0].orderOfAccuracy);
      
      poisson->setEquationAndBoundaryConditions(equation,cgop,boundaryConditions, boundaryConditionData );

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
  int geometryHasChanged=TRUE;
  updateDivergenceDamping( cg0,geometryHasChanged );

  checkArrays(" updatePressureEquation: end");

}

//\begin{>>MappedGridSolverInclude.tex}{\subsection{updateDivergenceDamping}} 
void OB_CompositeGridSolver::
updateDivergenceDamping( CompositeGrid & cg0, const int & geometryHasChanged )
//======================================================================
// /Description:
// parameters.dbase.get<real >("dampingDt") : time step last used when computing the divergence damping.
//
//\end{MappedGridSolverInclude.tex}  
//======================================================================
{
  if( true && pDomainSolver!=NULL )
  {
    // call the version in Cgins too
    Cgins & cgins = (Cgins&)(*pDomainSolver);
    if( cg0.numberOfComponentGrids() > cgins.dtv.size() ) 
    {
      cgins.dtv.resize(cg0.numberOfComponentGrids(),dt);
      cgins.hMin.resize(cg0.numberOfComponentGrids(),0.);
    }
    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      cgins.dtv[grid]=dt;
      cgins.hMin[grid]=mappedGridSolver[grid]->hMin;
    }

    cgins.updateDivergenceDamping(cg0,geometryHasChanged);

    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      realMappedGridFunction & divergenceDampingWeight = mappedGridSolver[grid]->divergenceDampingWeight();
      divergenceDampingWeight.updateToMatchGrid(cg0[grid]);
      divergenceDampingWeight=cgins.divDampingWeight[grid];
    }
    
  }
  
  // compute the weight term for the divergence in the pressure equation
  // ***** no need to recompute for solid body rotation/transation *************************

  
  real & dampingDt = parameters.dbase.get<real >("dampingDt");
  if( geometryHasChanged || dt>1.5*dampingDt || dt<dampingDt/1.5 )
  {
    // recompute the divergence damping term.
    if( debug() & 2 )
      printf(" xxxxxxxx recompute the divergence damping term, dt=%9.2e, dampingDt=%9.2e, nu=%9.2e, cdv=%9.2e \n",
              dt,dampingDt,parameters.dbase.get<real >("nu"),parameters.dbase.get<real >("cdv"));
    dampingDt=dt;
    
    real cDtDt= dt>0. ? parameters.dbase.get<real >("cDt")/dt : REAL_MAX;

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
//       printf(" ----divergence damping coefficient limited for implicit method by the value cDtDt=%e\n",cDtDt);
//     }
    
    Index I1,I2,I3;
    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      realMappedGridFunction & divergenceDampingWeight = mappedGridSolver[grid]->divergenceDampingWeight();
      divergenceDampingWeight.updateToMatchGrid(cg0[grid]);
      realArray & xy = cg0[grid].center();
      getIndex(cg0[grid].dimension(),I1,I2,I3,-1);

      const int nd=parameters.dbase.get<int >("compare3Dto2D") ? min(2,cg0.numberOfDimensions()) : cg0.numberOfDimensions();
      real cdvnu=parameters.dbase.get<real >("cdv")*max(parameters.dbase.get<real >("nu"),mappedGridSolver[grid]->hMin)*4/nd;

      if( cg0[grid].isRectangular() )
      {
	real dx[3];
	cg0[grid].getDeltaX( dx );
	// printf(" ***** insp: dx for rectangular grid = [%e,%e,%e] cDtDt=%6.2e cdvnu=%6.2e\n",dx[0],dx[1],dx[2],
        //  cDtDt,cdvnu);
	  
        if( cg0[grid].numberOfDimensions()==1 )
	  divergenceDampingWeight(I1,I2,I3)=min( cdvnu*( 1./SQR(2.*dx[0]) ), cDtDt );
        else if( cg0[grid].numberOfDimensions()==2 || parameters.dbase.get<int >("compare3Dto2D") )
	  divergenceDampingWeight(I1,I2,I3)=min( cdvnu*( 1./SQR(2.*dx[0])+ 1./SQR(2.*dx[1]) ), cDtDt );
        else 
    	  divergenceDampingWeight(I1,I2,I3)=min( cdvnu*( 1./SQR(2.*dx[0])+1./SQR(2.*dx[1])+1./SQR(2.*dx[2]) ), cDtDt );
      }
      else
      {
	if( cg0[grid].numberOfDimensions()==1 )
	  divergenceDampingWeight(I1,I2,I3)=min( cdvnu*DAI1(1.,I1,I2,I3), cDtDt );
	else if( cg0[grid].numberOfDimensions()==2 || 
		 parameters.dbase.get<int >("compare3Dto2D") )  // *** we use a 2D divergence for comparing 3d to 2d
	{
    	  divergenceDampingWeight(I1,I2,I3)=min( cdvnu*DAI2(1.,I1,I2,I3), cDtDt );
//           printf(" divergenceDampingWeight, max=%e, min=%e\n",max(divergenceDampingWeight(I1,I2,I3)),
//                min(divergenceDampingWeight(I1,I2,I3)));
	}
	else
	  divergenceDampingWeight(I1,I2,I3)=min( cdvnu*DAI3(1.,I1,I2,I3), cDtDt );
	
      }
      // printf(" divergenceDampingWeight, max=%e, min=%e\n",max(divergenceDampingWeight(I1,I2,I3)),
      //         min(divergenceDampingWeight(I1,I2,I3)));
      
    }
  }
}


//\begin{>>MappedGridSolverInclude.tex}{\subsection{assignPressureRHS}} 
void OB_CompositeGridSolver::
assignPressureRHS( GridFunction & gf0, realCompositeGridFunction & f )
//======================================================================
// /Description:
//
//\end{MappedGridSolverInclude.tex}  
//======================================================================
{
  if( debug() & 8 )
    printf("OB_CompositeGridSolver:assignPressureRHS...\n");
//  real time0=getCPU();
  
// ******
  if( FALSE )
    f=0.;
  

  real t0 = gf0.t;
  CompositeGrid & cg = gf0.cg;

  if( !poisson->getCompatibilityConstraint() )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      mappedGridSolver[grid]->assignPressureRHS( grid,cg[grid],gf0.u[grid],f[grid],gf0.getGridVelocity(grid),t0 );
    }
  }
  else
  { // set the rhs for the compatibility equation for the pressure

    const int & pc = parameters.dbase.get<int >("pc");
    // First get the indices of the (unused) point on the grid where the compat. eqn is put
    int ne,i1e,i2e,i3e,gride;
    poisson->equationToIndex( poisson->extraEquationNumber(0),ne,i1e,i2e,i3e,gride);

    // printf(" $$$$$$$ Poisson solve: extra equation: f[%i](%i,%i,%i) $$$$$$\n",gride,i1e,i2e,i3e);
    if( parameters.dbase.get<int >("orderOfAccuracy")==4 && gride!=cg.numberOfComponentGrids()-1 )
    {
      printf("ERROR: Poisson solve: the extra equation for the singular pressure equation is at f[%i](%i,%i,%i) \n"
             "  BUT this point NOT on the last grid -- this causes problems.  \n"
             "  Add an extra ghost line to the last grid to overcome this problem.\n",gride,i1e,i2e,i3e);
      Overture::abort("error");
    }
    

    f[gride](i1e,i2e,i3e)=0.;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      mappedGridSolver[grid]->assignPressureRHS( grid, gf0.cg[grid], gf0.u[grid], f[grid],
						 gf0.getGridVelocity(grid), t0 );

      if( twilightZoneFlow() ) 
      {
	//     ...add in the constraint equation     
	//        (This is the equation that sets the mean value of p)
	MappedGrid & c = gf0.cg[grid];
        Index I1,I2,I3;
	getIndex(c.dimension(),I1,I2,I3);
	f[gride](i1e,i2e,i3e)+=sum(poisson->rightNullVector[grid](I1,I2,I3)*
                                   (*parameters.dbase.get<OGFunction* >("exactSolution"))(c,I1,I2,I3,pc,t0));

        if( debug() & 32 )
	  display(poisson->rightNullVector[grid],"-- right null vector",parameters.dbase.get<FILE* >("debugFile"),"%10.4e ");
      }
    }
    if( debug() & 4 )
      fprintf(parameters.dbase.get<FILE* >("debugFile"),"compatibility rhs f[%i](%i,%i,%i)= %14.10e \n",
           gride,i1e,i2e,i3e,f[gride](i1e,i2e,i3e));
  }
//    if( debug() & 32 )
//      f.display("assignPressureRHS and solve : here is the rhs for the pressure equation",
//  				  parameters.dbase.get<FILE* >("debugFile"),"%8.1e "); // "%8.1e ");

}

int OB_MappedGridSolver::
computeAxisymmetricDivergence(realArray & divergence, 
                              Index & I1, Index & I2, Index & I3, MappedGrid & c,
			      const realArray & u0,
			      const realArray & u0x, 
			      const realArray & v0y )
// =============================================================================================
//  /Desctription:
//    Add corrections to the standard formual for divergence for axisymmetric flows.
//   The correction is $v/y$ which turns into $v_y$ at $y=0$.
// =============================================================================================
{
  const int vc = parameters.dbase.get<int >("vc");
  const realArray & vertex = c.vertex();
  
  realArray radiusInverse = 1./max(REAL_MIN,vertex(I1,I2,I3,axis2));
  Index Ib1,Ib2,Ib3;
  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      if( c.boundaryCondition(side,axis)==Parameters::axisymmetric )
      {
	getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	radiusInverse(Ib1,Ib2,Ib3)=0.;
	divergence(Ib1,Ib2,Ib3)+=v0y(Ib1,Ib2,Ib3);
      }
    }
  }
  divergence(I1,I2,I3)+=u0(I1,I2,I3,vc)*radiusInverse;
  return 0;
}




#define assignPressureRHSOpt assignpressurerhsopt_
extern "C"
{
void assignPressureRHSOpt(const int&nd,
		       const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
		       const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                       const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
		       const int&mask,  const real&rx, 
                       const real&u, const real&uu, real&f,const real&gv,const real&divDamping,  
                       const int&bc, const int&indexRange, const int&ndp, const real&pressureValue, 
                       const int&nr1a,const int&nr1b,const int&nr2a,const int&nr2b,const int&nr3a,const int&nr3b,
                       const real&normal00,const real&normal10,
                       const real&normal01,const real&normal11,
                       const real&normal02,const real&normal12,
		       const int&ipar, const real&rpar, const int&ierr );
}



static int countPressureSolves=0;

//\begin{>>MappedGridSolverInclude.tex}{\subsection{assignPressureRHS}} 
void OB_MappedGridSolver::
assignPressureRHS(const int & grid, 
		  MappedGrid & c,
		  realMappedGridFunction & u0, 
		  realMappedGridFunction & f,
		  realMappedGridFunction & gridVelocity, 
		  const real & t0 ) 
//======================================================================
// /Description:
//  Assign the right hand side for the pressure equation
// Notes
//   o The momentum equations are used here
//
//\end{MappedGridSolverInclude.tex}  
//======================================================================
{
  if( debug() & 32 )
    cout << "***Entering assignPressureRHS *** \n";
  if( debug() & 64 )
    display(u0,"u at start of assignPressureRHS",parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");

  if( grid==0 )
    countPressureSolves++;

  const int numberOfEquationDomains=equationDomainList.size();
  const int equationDomainNumber=equationDomainList.gridDomainNumberList[grid];
  assert( equationDomainNumber>=0 && equationDomainNumber<numberOfEquationDomains );
  EquationDomain & equationDomain = equationDomainList[equationDomainNumber];

  const Parameters::PDE pde = equationDomain.getPDE();


  realArray & u = u0;
  realArray & divergenceDamping = divergenceDampingWeight();
  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & pc = parameters.dbase.get<int >("pc");

  const real & ad21 = parameters.dbase.get<real >("ad21");
  const real & ad22 = parameters.dbase.get<real >("ad22");
  const real & ad41 = parameters.dbase.get<real >("ad41");
  const real & ad42 = parameters.dbase.get<real >("ad42");

  const int numberOfDimensions = c.numberOfDimensions();

  Index I1,I2,I3;
  getIndex( extendedGridIndexRange(c),I1,I2,I3 ); // use this large region so ux,uy,uz are big enough for BC's

  const int isRectangular=u0.getOperators()->isRectangular();
  const bool gridIsMoving = parameters.gridIsMoving(grid);

  #ifdef USE_PPP
    realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
  #else
    realSerialArray & uLocal = u;
  #endif
  const real *pu = u.getDataPointer();

  real *punc = (real*)pu;
  real *pNormal[2][3]={punc,punc,punc,punc,punc,punc}; // 
  if( !isRectangular )
  {
    ForBoundary(side,axis)
    {
      #ifdef USE_PPP
        realSerialArray *pn =c.rcData->pVertexBoundaryNormal[axis][side];
        assert( pn!=NULL );
	pNormal[side][axis] =pn->getDataPointer();
      #else
        pNormal[side][axis] =c.vertexBoundaryNormal(side,axis).getDataPointer();
      #endif
      if( pNormal[side][axis]==NULL )
      { // the boundary may not exist on this processor -- could double check this 
        pNormal[side][axis]=(real*)pu;
      }
    }
  }


  bool optimizedVersionWasUsed=false;

  bool useOpt=true; // false; // true;
  if( useOpt && 
      // c.numberOfDimensions()==2 && 
      // isRectangular &&  // we can also do curvilinear
      // !parameters.gridIsMoving(grid) && // we can do moving now too
      !parameters.isAxisymmetric() )
  {
    optimizedVersionWasUsed=true;
    
    int useWhereMask=!gridIsMoving; // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 
    real dx[3];
    c.getDeltaX(dx);
   
    const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");

    const realArray & xy = isRectangular ? u :  c.center();
    const realArray & rsxy = isRectangular ? u :  c.inverseVertexDerivative();

    #ifdef USE_PPP
      real *pf = f.getLocalArray().getDataPointer(); 
//      const real *pxy = xy.getLocalArray().getDataPointer(); 
      const real *prsxy = rsxy.getLocalArray().getDataPointer(); 
      const real *pgv = gridIsMoving ? gridVelocity.getLocalArray().getDataPointer() : pu;
      const real *pdd = divergenceDamping.getLocalArray().getDataPointer();
      const int *pmask = c.mask().getLocalArray().getDataPointer();
    #else
      real *pf = f.getDataPointer(); 
//      const real *pxy = xy.getDataPointer(); 
      const real *prsxy = rsxy.getDataPointer(); 
      const real *pgv = gridIsMoving ? gridVelocity.getDataPointer() : pu;
      const real *pdd = divergenceDamping.getDataPointer();
      const int *pmask = c.mask().getDataPointer();
    #endif

    // const realArray *rxp = isRectangular ? &u :  &c.inverseVertexDerivative();

      
    // For non-moving grids u==uu, otherwise uu is a temp space to hold (u-gv)
    real *puu =  (real*)pu;

    if( gridIsMoving )
    {
      // *** added 040825 ****  uu was being allocated
      // *note* uu doesn't have as many components as u but fortran routines currently assume the
      // dimensions of uu are the same as for u (note: uc=1, vc=2 for INS)
      realArray & uu = get(WorkSpace::uu);
      MappedGridSolverWorkSpace::resize(uu,u0.dimension(0),u0.dimension(1),u0.dimension(2),
                                           u0.dimension(3)); 
      #ifdef USE_PPP
        puu= uu.getLocalArray().getDataPointer();
      #else
        puu= uu.getDataPointer();
      #endif
    }

    if( debug() & 8 )
    {
      display(f,sPrintF("assignPressureRHS: rhs f, grid=%i, before assignOPT",grid),parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
    }

    const IntegerArray & gid = c.gridIndexRange();
    getIndex(gid,I1,I2,I3);
    int n1a,n1b,n2a,n2b,n3a,n3b;
    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b); 

    if( ok )
    {

      // When we are projecting the initial conditions the advectionCoefficient=0 **fix this**
      const int includeADinPressure=parameters.dbase.get<bool >("includeArtificialDiffusionInPressureEquation") &&
	parameters.dbase.get<real >("advectionCoefficient")!=0.;

      real adcBoussinesq=0.; // coefficient of artificial diffusion for Boussinesq T equation 
      real thermalExpansivity=1.;
      parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
      parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("adcBoussinesq",adcBoussinesq);

      // declare and lookup visco-plastic parameters (macro)
      declareViscoPlasticParameters;


      int ipar[] ={parameters.dbase.get<int >("pc"),
		   parameters.dbase.get<int >("uc"),
		   parameters.dbase.get<int >("vc"),
		   parameters.dbase.get<int >("wc"),
		   parameters.dbase.get<int >("tc"),
		   parameters.dbase.get<int >("kc"),  // for a turbulence model
		   grid,
		   orderOfAccuracy,
		   (int)parameters.gridIsMoving(grid),
		   useWhereMask,
		   (int)parameters.isAxisymmetric(),
		   (int)parameters.dbase.get<int >("pressureBoundaryCondition"),
		   parameters.dbase.get<int >("numberOfComponents"),
		   (isRectangular? 0 : 1),
		   parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),
		   (int)parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion"),
		   (int)parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion"),
		   (int)parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion"),
		   includeADinPressure,
                   (int)parameters.dbase.get<Parameters::PDEModel >("pdeModel") }; //

      real rpar[]={c.gridSpacing(0),c.gridSpacing(1),c.gridSpacing(2),
		   dx[0],dx[1],dx[2],
		   parameters.dbase.get<real >("nu"),
		   parameters.dbase.get<real >("advectionCoefficient"),
		   parameters.dbase.get<real >("inflowPressure"),
		   parameters.dbase.get<real >("ad21"),
		   parameters.dbase.get<real >("ad22"),
		   parameters.dbase.get<real >("ad41"),
		   parameters.dbase.get<real >("ad42") ,
		   parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],
		   parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1],
		   parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2],   // 15
		   thermalExpansivity,
                   adcBoussinesq,
                   nuViscoPlastic,          // 18
                   etaViscoPlastic,
                   yieldStressViscoPlastic,
                   exponentViscoPlastic,
                   epsViscoPlastic };

      int ierr=0;
    
      real pressureValue;

      IntegerArray gidLocal,dimensionLocal,bcLocal;
      getLocalBoundsAndBoundaryConditions( u0,gidLocal,dimensionLocal,bcLocal );

      // this is a fudge to treat the interface BC:
      ForBoundary(side,axis)
      {
	if( bcLocal(side,axis)==Parameters::interfaceBoundaryCondition && 
            pde!=Parameters::incompressibleNavierStokes )
	{
	  bcLocal(side,axis)==Parameters::dirichletBoundaryCondition;
	}
      }
      

      assignPressureRHSOpt(c.numberOfDimensions(),
			   I1.getBase(),I1.getBound(),
			   I2.getBase(),I2.getBound(),
			   I3.getBase(),I3.getBound(),
			   uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			   uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			   *pmask,*prsxy, *pu,*puu,  *pf, *pgv, *pdd, 
			   // c.boundaryCondition(0,0), c.extendedIndexRange(0,0), 
			   bcLocal(0,0), gidLocal(0,0), 
			   parameters.dbase.get<RealArray >("bcData").getLength(0),parameters.bcData(0,0,0,grid), 
			   n1a,n1b,n2a,n2b,n3a,n3b, 
			   *pNormal[0][0],*pNormal[1][0],*pNormal[0][1],*pNormal[1][1],*pNormal[0][2],*pNormal[1][2],
			   ipar[0], rpar[0], ierr );

      
    } 
    
    if( debug() & 8 )
    {
//       if( c.rcData->pVertexBoundaryNormal[1][0] != NULL )
//       {
// 	::display(*c.rcData->pVertexBoundaryNormal[axis2][Start],
//                   sPrintF("assignPressureRHS: normal side=0 axis=1, grid=%i",grid),
//                   parameters.dbase.get<FILE* >("pDebugFile"),"%8.5f ");
//       }
      display(f,sPrintF("assignPressureRHS: rhs f, grid=%i, after assignOPT",grid),parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
    }
   
    if( parameters.gridIsMoving(grid) &&
        !parameters.dbase.get<bool >("twilightZoneFlow")  )  // *wdh* 040914 
    {
      // add grid acceleration 
      // *wdh* 040914 -- for TZ no sense adding this on since we would have to subtract it off later  -- it
      // is just another external forcing 
      int side,axis;
      Index I1g,I2g,I3g;
      ForBoundary(side,axis)
      {
	if( c.boundaryCondition(side,axis)>0 )
	{
	  getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line
	  getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line
	  switch (c.boundaryCondition(side,axis))
	  {
	  case Parameters::outflow:
	  case Parameters::convectiveOutflow:
	  case Parameters::tractionFree:
	  case Parameters::inflowWithPressureAndTangentialVelocityGiven:
	  case Parameters::dirichletBoundaryCondition:
	  case Parameters::symmetry:
	  case Parameters::axisymmetric:
	    break;
	  default:
	    // add n.( -u.t) for moving grids or time dependent BC's
	    gridAccelerationBC(grid, t0, c, u0, f, gridVelocity,c.vertexBoundaryNormal(side,axis),
			       I1,I2,I3, I1g,I2g,I3g,side,axis  );
	  }
	}
      } // end forBoundary
    }
    
  }


  if( !optimizedVersionWasUsed 
      // *wdh* 030729   || !isRectangular     // For curvilinear grids we do not apply BC's yet in the optimized code.
    )
  {

    // ************** curvilinear grids *************************

    const real & nu  = parameters.dbase.get<real >("nu");
    const real & cdv = parameters.dbase.get<real >("cdv");
    const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");

    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    Range N = c.numberOfDimensions()==2 ? Range(uc,vc) : c.numberOfDimensions()==3 ? Range(uc,wc) : Range(uc,uc);

    realArray & uu = get(WorkSpace::uu);
    realArray & ux = get(WorkSpace::ux);
    realArray & uy = get(WorkSpace::uy);
    realArray & uz = get(WorkSpace::uz);


    MappedGridOperators & op = *(u0.getOperators());
    realArray & xy = c.vertex();

    realArray radiusInverse;  // needed for axisymmetric case
    if( !optimizedVersionWasUsed ) // *** 1 ***
    {
      // **************************************************
      // ***** determine the interior RHS *****************
      // **************************************************


      Index D1,D2,D3;
      getIndex( c.dimension(),D1,D2,D3 );
      MappedGridSolverWorkSpace::resize(uu,D1,D2,D3,Range(uc,uc+c.numberOfDimensions()-1));

      MappedGridSolverWorkSpace::resize(ux,I1,I2,I3,N);
    
      op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,N);
      if( c.numberOfDimensions()>=2 )
      {
	MappedGridSolverWorkSpace::resize(uy,I1,I2,I3,N);
	op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,N);
      }
      if( c.numberOfDimensions()>=3 )
      {
	MappedGridSolverWorkSpace::resize(uz,I1,I2,I3,N);
	op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,N);
      }
  
      const int vc0 = c.numberOfDimensions()>1 ? vc : uc;
      const int wc0 = c.numberOfDimensions()>2 ? wc : uc;
  
      const realArray & u0x = ux(I1,I2,I3,uc);
      const realArray & v0y = uy(I1,I2,I3,vc0);
      const realArray & w0z = uz(I1,I2,I3,wc0);

      realArray dilatation(I1,I2,I3);  // **************** fix this ***********
      
      if( c.numberOfDimensions()==1 )
      {
	dilatation=u0x;
	f(I1,I2,I3)=( (-advectionCoefficient)*( SQR(UX(uc)) )
		      + divergenceDamping(I1,I2,I3)*dilatation );     
      }
      else if( c.numberOfDimensions()==2 )
      {
	dilatation=u0x+v0y;
	if( parameters.isAxisymmetric() ) // ** wdh ** 040228 && !parameters.dbase.get<bool >("twilightZoneFlow") )
	{
	  radiusInverse = 1./max(REAL_MIN,xy(I1,I2,I3,axis2));
	  Index Ib1,Ib2,Ib3;
	  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
	  {
	    for( int side=0; side<=1; side++ )
	    {
	      if( c.boundaryCondition(side,axis)==Parameters::axisymmetric )
	      {
		getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		radiusInverse(Ib1,Ib2,Ib3)=0.;
		dilatation(Ib1,Ib2,Ib3)+=v0y(Ib1,Ib2,Ib3);
	      }
	    }
	  }
	  dilatation+=u(I1,I2,I3,vc)*radiusInverse;
	}
    
        if( false )
	{
	  real maxDiv=0.;
	  where( c.mask()(I1,I2,I3)>0 )
	  {
	    maxDiv=max(fabs(dilatation(I1,I2,I3)));
	  }
	  printf(" *** assignPressureRHS: grid=%i maxDiv=%8.2e\n",grid,maxDiv);
	}
	
	f(I1,I2,I3)=( (-advectionCoefficient)*( u0x*u0x+2.*UY(uc)*UX(vc)+v0y*v0y )
		      + divergenceDamping(I1,I2,I3)*dilatation );     
      }
      else  // 3D
      {
	dilatation=u0x+v0y+w0z;
	f(I1,I2,I3)=(
	  (-advectionCoefficient)*( u0x*u0x+v0y*v0y+w0z*w0z
				    + 2.*(UY(uc)*UX(vc) + UZ(uc)*UX(wc) + UZ(vc)*UY(wc)) )
	  + divergenceDamping(I1,I2,I3)*dilatation
	  );                         
      } 

  
      if( countPressureSolves % 10 == 1 )  // check divergence every few steps
      {
        const intArray & mask = c.mask();

        getIndex( c.gridIndexRange(),I1,I2,I3 ); // *wdh* 030313

	real maximumDivergence, divOverGrad;
	where( mask(I1,I2,I3) > 0 )
	  maximumDivergence=max(fabs(dilatation(I1,I2,I3)));

	realArray uMin(parameters.dbase.get<int >("numberOfComponents")), uMax(parameters.dbase.get<int >("numberOfComponents"));
	real uvMax;
	getSolutionBounds(u0,uMin,uMax,uvMax);

	const Range & Ru = parameters.dbase.get<Range >("Ru"); // velocity components
	real uScale=max(fabs(uMax(Ru))+fabs(uMin(Ru)));
	if( uScale==0. )
	  uScale=1.;
    
	if( maximumDivergence > .1*uScale )
	{
	  if( c.numberOfDimensions()==1 )
	  {
	    where( mask(I1,I2,I3) > 0 )
	      divOverGrad=max(fabs(dilatation(I1,I2,I3))/
			      (  fabs(u0x(I1,I2,I3)) )+.1*maximumDivergence );
	  }
	  else if( c.numberOfDimensions()==2 )
	  {
	    where( mask(I1,I2,I3) > 0 )
	      divOverGrad=2.*max(fabs(dilatation(I1,I2,I3))/
				 (  fabs(u0x(I1,I2,I3)   )+fabs(UX(vc))
				    +fabs(UY(uc))+fabs(v0y(I1,I2,I3)   )+.1*maximumDivergence ));
	  }
	  else
	  {
	    where( mask(I1,I2,I3) > 0 )
	      divOverGrad=3.*max(fabs(dilatation(I1,I2,I3))/
				 (  fabs(u0x(I1,I2,I3)   )+fabs(UX(vc))+fabs(UX(wc))
				    +fabs(UY(uc))+fabs(v0y(I1,I2,I3)   )+fabs(UY(wc))
				    +fabs(UZ(uc))+fabs(UZ(vc))+fabs(w0z(I1,I2,I3)   )+.1*maximumDivergence ));
	  }
	  if( divOverGrad>.1*uScale )
	  {
            if( parameters.dbase.get<int >("info") & 2 )
	      printf("Warning: max|div|=%8.1e, max|div/grad|=%8.1e, t=%e, cdv=%8.1e, grid= %s\n",
		     maximumDivergence,divOverGrad,t0,cdv,(const char*)c.getName());
	  }
	}
      }
      
    }  // end if( !optimizedVersionWasUsed )  *** 1 ***
    

    //     ************************************
    //     ----apply the boundary condition----
    //     ************************************

    
    realArray & uxx= get(WorkSpace::uxx);
    realArray & uyy= get(WorkSpace::uyy);
    realArray & uzz= get(WorkSpace::uzz);

    int side,axis;
    Index I1g,I2g,I3g;
    Range R[3];

    ForBoundary(side,axis)
    {
      if( c.boundaryCondition(side,axis)>0 )
      {
	getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line
	getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line

	// getGhostIndex( extendedGridIndexRange(c),side,axis,I1g,I2g,I3g,1);  // first ghost line
	// getGhostIndex( extendedGridIndexRange(c),side,axis,I1 ,I2 ,I3 ,0);     // boundary line

        // ::display(f(I1g,I2g,I3g),"f before C++ BC","%19.10e "); // ****************************

        #ifdef USE_PPP
 	  realArray & normal = c.inverseVertexDerivative(); // *** fix this ***
        #else
    	  realArray & normal = c.vertexBoundaryNormal(side,axis);
        #endif

	switch (c.boundaryCondition(side,axis))
	{
	case Parameters::outflow:
	case Parameters::convectiveOutflow:
	case Parameters::tractionFree:
	{
	  bool applyNeumannBC = parameters.bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid)!=0.;

	  if( applyNeumannBC )
	  {
	    // printf("**apply mixed BC on pressure rhs...\n");
	    f(I1g,I2g,I3g)=parameters.bcData(pc,side,axis,grid);
	  }
	  else
	  {
	    f(I1,I2,I3)=parameters.bcData(pc,side,axis,grid);
	    f(I1g,I2g,I3g)=0.;  // for extrapolation
	  }
	  break;
	}
	case Parameters::inflowWithPressureAndTangentialVelocityGiven:
	case Parameters::dirichletBoundaryCondition:
	  f(I1,I2,I3)=parameters.dbase.get<real >("inflowPressure");
	  break;	
	case Parameters::symmetry:
	case Parameters::axisymmetric:
	  f(I1g,I2g,I3g)=0.;             // p.n=0
	  break;
	default:
	  if( parameters.dbase.get<int >("pressureBoundaryCondition")==2 )
	  {
	    // give p.n==0 
	    f(I1g,I2g,I3g)=0.;
	    break;
	  }

	  // we need more derivatives here than above (2nd)
	  if( c.numberOfDimensions()==1 )
	  {
	    // u0.getDerivatives(I1,I2,I3,N);      // **************************** is this efficient??
	    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,N);
	    op.derivative(MappedGridOperators::xxDerivative,u0,uxx,I1,I2,I3,N);
	    if( parameters.gridIsMoving(grid) )
	      uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-advectionCoefficient*U(uc);
	    else
	      uu(I1,I2,I3,uc)=(-advectionCoefficient)*U(uc);

	    f(I1g,I2g,I3g)=PN1(I1,I2,I3);  // give normal component of momentum equations (without u.t term)

	    // add n.( -u.t) for moving grids or time dependent BC's
	    gridAccelerationBC(grid, t0, c, u0, f, gridVelocity,normal,
			       I1,I2,I3, I1g,I2g,I3g,side,axis  );

	    // **********************************************************************************
	    // ** f(I1g,I2g,I3g)=0.;  // for testing

	  }
	  else if( c.numberOfDimensions()==2 )
	  {
	    if( parameters.gridIsMoving(grid) )
	    {
	      uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-advectionCoefficient*U(uc);
	      uu(I1,I2,I3,vc)=gridVelocity(I1,I2,I3,1)-advectionCoefficient*U(vc);
	    }
	    else
	    {
	      uu(I1,I2,I3,uc)=(-advectionCoefficient)*U(uc);
	      uu(I1,I2,I3,vc)=(-advectionCoefficient)*U(vc);
	    }

	    // ***** could optimize for rectangular ****

	    // fprintf(parameters.dbase.get<FILE* >("debugFile"),"++++ side=%i, axis=%i, bc=%i \n",side,axis,c.boundaryCondition()(side,axis));
	    // Range V(uc,uc+c.numberOfDimensions()-1);
	    // display(uu(I1,I2,I3,V),"uu",parameters.dbase.get<FILE* >("debugFile"),"%4.1f ");
	    // display(ux(I1,I2,I3,V),"ux",parameters.dbase.get<FILE* >("debugFile"),"%4.1f ");
	    // display(uy(I1,I2,I3,V),"ux",parameters.dbase.get<FILE* >("debugFile"),"%4.1f ");
	    // display(uxx(I1,I2,I3,V),"uxx",parameters.dbase.get<FILE* >("debugFile"),"%4.1f ");
	    // display(uyy(I1,I2,I3,V),"uyy",parameters.dbase.get<FILE* >("debugFile"),"%4.1f ");

	    if( parameters.dbase.get<int >("pressureBoundaryCondition")==1 )
	    { // old way
	      u0.getDerivatives(I1,I2,I3,N);      // **************************** is this efficient??
	      f(I1g,I2g,I3g)=PN2(I1,I2,I3);  // give normal component of momentum equations (without u.t term)
	    }
	    else if( parameters.dbase.get<int >("pressureBoundaryCondition")==2 )
	    { // give p.n=0
	      printf("ERROR: this case should not happen.\n");
	      throw "error";
	    }
	    else 
	    {
	      // ******* curl form of nu*Delta(u) --> nu*( -vxy+uyy , vxx-uxy )

	      // * f(I1g,I2g,I3g)=PN2(I1,I2,I3);  // give normal component of momentum equations (without u.t term)
	      // * real regBC =max(fabs(f(I1g,I2g,I3g)));


	      // ***** fix: use opt operators
	      // const realArray & u0xyOld = u0.xy(I1,I2,I3,Range(uc,vc))(I1,I2,I3,Range(uc,vc));
	      if( op.isRectangular() )
	      {
		// printf(" *** Error in u0xy = %e\n",max(fabs(u0xy-u0xyOld)));
		if( axis==axis1 )
		{
		  realArray udd(I1,I2,I3,Range(uc,vc));
		  op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,uc);
		  op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,uc);
		  op.derivative(MappedGridOperators::yyDerivative,u0,udd,I1,I2,I3,uc);
		  op.derivative(MappedGridOperators::xyDerivative,u0,udd,I1,I2,I3,vc);

		  f(I1g,I2g,I3g)=(2*side-1)*(
		    nu*(-udd(I1,I2,I3,vc)+udd(I1,I2,I3,uc))+
		    advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) +
					   uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc)) );

                  if( parameters.isAxisymmetric() )
		  { 
                    // In this case u.x + v.y + v/y = 0 -> u.xx = -v.xy -v.x/y
                    // add on nu*( -v.x/y + u.y/y ) 
		    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,vc); // compute v.x
                    where( radiusInverse(I1,I2,I3)!= 0. )
		    {
		      f(I1g,I2g,I3g)+=(2*side-1)*nu*radiusInverse(I1,I2,I3)*( -ux(I1,I2,I3,vc)+uy(I1,I2,I3,uc) );
		    }
		    otherwise()
		    {
                      // -v.xy + u.yy 
		      f(I1g,I2g,I3g)+=(2*side-1)*nu*( -udd(I1,I2,I3,vc) + udd(I1,I2,I3,uc) );
		    }
		  }

  		  if( false && parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
  		  { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
		    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,vc);
		    op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,vc);

  		    real cd22=ad22/SQR(c.numberOfDimensions());
                    f(I1g,I2g,I3g)+=(2*side-1)*AD2(uc);
		  }
		}
		else 
		{
		  realArray udd(I1,I2,I3,Range(uc,vc));
		  op.derivative(MappedGridOperators::xDerivative,u0,ux ,I1,I2,I3,vc);
		  op.derivative(MappedGridOperators::yDerivative,u0,uy ,I1,I2,I3,vc);
		  op.derivative(MappedGridOperators::xxDerivative,u0,udd,I1,I2,I3,vc);
		  op.derivative(MappedGridOperators::xyDerivative,u0,udd,I1,I2,I3,uc);
	     
		  f(I1g,I2g,I3g)=(2*side-1)*(
		    nu*(udd(I1,I2,I3,vc)-udd(I1,I2,I3,uc))+
		    advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc)) 
		    );


                  if( parameters.isAxisymmetric() )
		  { 
                    // In this case u.x + v.y + v/y = 0 -> u.xy + v.yy + v.y/y - v/y^2 = 0
                    //          => v.yy + v.y/y - v/y^2 = - u.xy (as before)

                    // no need to add anything extra in this case
		  }

  		  if( false && parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
  		  { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
		    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,uc);
		    op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,uc);

  		    real cd22=ad22/SQR(c.numberOfDimensions());
                    f(I1g,I2g,I3g)+=(2*side-1)*AD2(vc);
		  }


		}

	      }
	      else // curvilinear case
	      {
		// u0.getDerivatives(I1,I2,I3,N);      // **************************** is this efficient?? 
		if( optimizedVersionWasUsed )
		{
		  MappedGridSolverWorkSpace::resize(ux,I1,I2,I3,N);
		  MappedGridSolverWorkSpace::resize(uy,I1,I2,I3,N);
		}
		
		MappedGridSolverWorkSpace::resize(uxx,I1,I2,I3,N);
		MappedGridSolverWorkSpace::resize(uyy,I1,I2,I3,N);

		op.derivative(MappedGridOperators::xDerivative,u0,ux ,I1,I2,I3,N);
		op.derivative(MappedGridOperators::yDerivative,u0,uy ,I1,I2,I3,N);
		op.derivative(MappedGridOperators::xxDerivative,u0,uxx ,I1,I2,I3,vc);
		op.derivative(MappedGridOperators::yyDerivative,u0,uyy ,I1,I2,I3,uc);

		Range V(uc,vc);
		realArray u0xy(I1,I2,I3,V);
		op.derivative(MappedGridOperators::xyDerivative,u0,u0xy,I1,I2,I3,V);

//  		printf(" *** Error in u0xy = %e\n",max(fabs(u0xy-u0.xy()(I1,I2,I3,V))));
//  		realArray temp;
//  		temp =nu*(-u0xy(I1,I2,I3,vc)+uyy(I1,I2,I3,uc))+
//  		  advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) +
//  					 uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc));
//                  printf(" *** nu=%e\n",nu);
//  		::display(u0xy(I1,I2,I3,uc),"uxy from C++ BC","%19.10e "); 
//  		::display(u0xy(I1,I2,I3,vc),"vxy from C++ BC","%19.10e "); 
//  		::display(uyy(I1,I2,I3,uc),"uyy from C++ BC","%19.10e "); 
//  		::display(ux(I1,I2,I3,uc),"ux from C++ BC","%19.10e "); 
//  		::display(uy(I1,I2,I3,uc),"uy from C++ BC","%19.10e "); 
//  		::display(temp,"pbu from C++ BC","%19.10e "); 
//  		temp=nu*(uxx(I1,I2,I3,vc)-u0xy(I1,I2,I3,uc))+
//  		  advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc));
//  		::display(temp,"pbv from C++ BC","%19.10e "); 

		f(I1g,I2g,I3g)=normal(I1,I2,I3,0)*(
		  nu*(-u0xy(I1,I2,I3,vc)+uyy(I1,I2,I3,uc))+
		  advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) +
					 uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc))
		  )+
		  normal(I1,I2,I3,1)*(
		    nu*(uxx(I1,I2,I3,vc)-u0xy(I1,I2,I3,uc))+
		    advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc)) 
		    );

                  if( parameters.isAxisymmetric() )
		  { 
                    // -->  add on nu*( u.y/y ) 
                    // NOTE: there are no extra terms from the v equation.
                    where( radiusInverse(I1,I2,I3)!= 0. )
		    {
		      f(I1g,I2g,I3g)+=normal(I1,I2,I3,0)*
                         nu*radiusInverse(I1,I2,I3)*( uy(I1,I2,I3,uc) - ux(I1,I2,I3,vc) );
		    }
		    otherwise()
		    {
		      f(I1g,I2g,I3g)+=normal(I1,I2,I3,0)*nu*( uyy(I1,I2,I3,uc) -u0xy(I1,I2,I3,vc) );
		    }
		  }
	      }
	    
//             real curlBC=max(fabs(f(I1g,I2g,I3g)));
//             real regBC=max(PN2(I1,I2,I3));
// 	       printf("apply curl(curl) BC for p (side,axis,grid)=(%i,%i,%i): max(rhs)=%8.2e"
//                     "(regular BC max(rhs)=%8.2e)\n",side,axis,grid,curlBC,regBC);
	    }  // else
	  

	    // display(f(I1g,I2g,I3g),"PN2",parameters.dbase.get<FILE* >("debugFile"),"%4.1f ");

	    // add n.( -u.t) for moving grids or time dependent BC's
	    gridAccelerationBC(grid, t0, c, u0, f, gridVelocity,normal,
			       I1,I2,I3, I1g,I2g,I3g,side,axis   );

	    // **********************************************************************************
	    // ** f(I1g,I2g,I3g)=0.;  // for testing

	  }
	  else
	  {
	    // 3D
	    if( parameters.gridIsMoving(grid) )
	    {
	      uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-advectionCoefficient*U(uc);
	      uu(I1,I2,I3,vc)=gridVelocity(I1,I2,I3,1)-advectionCoefficient*U(vc);
	      uu(I1,I2,I3,wc)=gridVelocity(I1,I2,I3,2)-advectionCoefficient*U(wc);
	    }
	    else
	    {
	      uu(I1,I2,I3,uc)=(-advectionCoefficient)*U(uc);
	      uu(I1,I2,I3,vc)=(-advectionCoefficient)*U(vc);
	      uu(I1,I2,I3,wc)=(-advectionCoefficient)*U(wc);
	    }

	    if( parameters.dbase.get<int >("pressureBoundaryCondition")==1 )
	    { // old way
	      u0.getDerivatives(I1,I2,I3,N);  
	      f(I1g,I2g,I3g)=PN3(I1,I2,I3);  // give normal component of momentum equations (without u.t term)
	    }
	    else if( parameters.dbase.get<int >("pressureBoundaryCondition")==2 )
	    { // give zero normal component
	      f(I1g,I2g,I3g)=0.;  
	    }
	    else
	    {
	      // curl form:  Delta(u) = ( (-v.xy-w.xz) +u.yy+u.zz, v.xx-u.xy-w.yz+v.zz, w.xx+w.yy-u.xz-v.yz)
	      // (just use u.x+v.y+w.z=0 to replace diagonal terms)

	      if( op.isRectangular() )
	      {
		// printf(" *** Error in u0xy = %e\n",max(fabs(u0xy-u0xyOld)));
		if( axis==axis1 )
		{
		  op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,uc);  // is this already done?
		  op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,uc);
		  op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,uc);
		  f(I1g,I2g,I3g)=((2*side-1)*advectionCoefficient)*(
		    uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc)+
		    uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc)+
		    uu(I1,I2,I3,wc)*uz(I1,I2,I3,uc));

  		  if( false && parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
  		  { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
		    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,Range(vc,wc));
		    op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,Range(vc,wc));
		    op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,Range(vc,wc));

  		    real cd22=ad22/SQR(c.numberOfDimensions());
                    f(I1g,I2g,I3g)+=(2*side-1)*AD2(uc);
		  }


		  op.derivative(MappedGridOperators::yyDerivative,u0,ux,I1,I2,I3,uc);
		  op.derivative(MappedGridOperators::xyDerivative,u0,ux,I1,I2,I3,vc);
		  op.derivative(MappedGridOperators::xzDerivative,u0,ux,I1,I2,I3,wc);
		  op.derivative(MappedGridOperators::zzDerivative,u0,uz,I1,I2,I3,uc);

		  f(I1g,I2g,I3g)+=((2*side-1)*nu)*(
		    (ux(I1,I2,I3,uc)-ux(I1,I2,I3,vc)-ux(I1,I2,I3,wc)+uz(I1,I2,I3,uc)) );


		}
		else if( axis==axis2 )
		{
		  op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,vc);  // is this already done?
		  op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,vc);
		  op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,vc);
		  f(I1g,I2g,I3g)=((2*side-1)*advectionCoefficient)*(
		    uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc)+
		    uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc)+
		    uu(I1,I2,I3,wc)*uz(I1,I2,I3,vc));

  		  if( false && parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
  		  { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
		    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,uc);
		    op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,uc);
		    op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,uc);

		    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,wc);
		    op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,wc);
		    op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,wc);

  		    real cd22=ad22/SQR(c.numberOfDimensions());
                    f(I1g,I2g,I3g)+=(2*side-1)*AD2(vc);
		  }

		  op.derivative(MappedGridOperators::xxDerivative,u0,ux,I1,I2,I3,vc);
		  op.derivative(MappedGridOperators::xyDerivative,u0,ux,I1,I2,I3,uc);
		  op.derivative(MappedGridOperators::yzDerivative,u0,ux,I1,I2,I3,wc);
		  op.derivative(MappedGridOperators::zzDerivative,u0,uz,I1,I2,I3,vc);

		  f(I1g,I2g,I3g)+=((2*side-1)*nu)*(
		    (ux(I1,I2,I3,vc)-ux(I1,I2,I3,uc)-ux(I1,I2,I3,wc)+uz(I1,I2,I3,vc)) );

		}
		else 
		{
		  op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,wc);  // is this already done?
		  op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,wc);
		  op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,wc);
		  f(I1g,I2g,I3g)=((2*side-1)*advectionCoefficient)*(
		    uu(I1,I2,I3,uc)*ux(I1,I2,I3,wc)+
		    uu(I1,I2,I3,vc)*uy(I1,I2,I3,wc)+
		    uu(I1,I2,I3,wc)*uz(I1,I2,I3,wc));

  		  if( false && parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
  		  { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
		    op.derivative(MappedGridOperators::xDerivative,u0,ux,I1,I2,I3,Range(uc,vc));
		    op.derivative(MappedGridOperators::yDerivative,u0,uy,I1,I2,I3,Range(uc,vc));
		    op.derivative(MappedGridOperators::zDerivative,u0,uz,I1,I2,I3,Range(uc,vc));

  		    real cd22=ad22/SQR(c.numberOfDimensions());
                    f(I1g,I2g,I3g)+=(2*side-1)*AD2(wc);
		  }

		  op.derivative(MappedGridOperators::xxDerivative,u0,ux,I1,I2,I3,wc);
		  op.derivative(MappedGridOperators::xzDerivative,u0,ux,I1,I2,I3,uc);
		  op.derivative(MappedGridOperators::yzDerivative,u0,ux,I1,I2,I3,vc);
		  op.derivative(MappedGridOperators::yyDerivative,u0,uz,I1,I2,I3,wc);

		  f(I1g,I2g,I3g)+=((2*side-1)*nu)*(
		    (ux(I1,I2,I3,wc)-ux(I1,I2,I3,uc)-ux(I1,I2,I3,vc)+uz(I1,I2,I3,wc)) );


		}

	      }
	      else
	      {
		// u0.getDerivatives(I1,I2,I3,N);      // **************************** is this efficient?? 
		if( optimizedVersionWasUsed )
		{
		  MappedGridSolverWorkSpace::resize(ux,I1,I2,I3,N);
		  MappedGridSolverWorkSpace::resize(uy,I1,I2,I3,N);
		  MappedGridSolverWorkSpace::resize(uz,I1,I2,I3,N);
		}

		MappedGridSolverWorkSpace::resize(uxx,I1,I2,I3,N);
		MappedGridSolverWorkSpace::resize(uyy,I1,I2,I3,N);
		MappedGridSolverWorkSpace::resize(uzz,I1,I2,I3,N);

		op.derivative(MappedGridOperators::xDerivative,u0,ux ,I1,I2,I3,N);
		op.derivative(MappedGridOperators::yDerivative,u0,uy ,I1,I2,I3,N);
		op.derivative(MappedGridOperators::zDerivative,u0,uz ,I1,I2,I3,N);
		op.derivative(MappedGridOperators::xxDerivative,u0,uxx ,I1,I2,I3,Range(vc,wc));
		op.derivative(MappedGridOperators::yyDerivative,u0,uyy ,I1,I2,I3,uc);
		op.derivative(MappedGridOperators::yyDerivative,u0,uyy ,I1,I2,I3,wc);
		op.derivative(MappedGridOperators::zzDerivative,u0,uzz ,I1,I2,I3,Range(uc,vc));

		Range V1(uc,vc);
		realArray u0xy(I1,I2,I3,V1);  
		op.derivative(MappedGridOperators::xyDerivative,u0,u0xy,I1,I2,I3,V1);
		Range V2(vc,wc);
		realArray u0yz(I1,I2,I3,V2);
		op.derivative(MappedGridOperators::yzDerivative,u0,u0yz,I1,I2,I3,V2);

		Range V3(uc,wc);
		realArray u0xz(I1,I2,I3,V3);
		op.derivative(MappedGridOperators::xzDerivative,u0,u0xz,I1,I2,I3,uc);
		op.derivative(MappedGridOperators::xzDerivative,u0,u0xz,I1,I2,I3,wc);
		// printf(" *** Error in u0xy = %e\n",max(fabs(u0xy-u0xyOld)));

		u0.getDerivatives(I1,I2,I3,N);  
		f(I1g,I2g,I3g)=normal(I1,I2,I3,0)*(
		  nu*(-u0xy(I1,I2,I3,vc)-u0xz(I1,I2,I3,wc)+uyy(I1,I2,I3,uc)+uzz(I1,I2,I3,uc))+
		  advectionCoefficient*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc)+
					 uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc)+
					 uu(I1,I2,I3,wc)*uz(I1,I2,I3,uc))
		  )+
		  normal(I1,I2,I3,1)*(
		    nu*(uxx(I1,I2,I3,vc)-u0xy(I1,I2,I3,uc)-u0yz(I1,I2,I3,wc)+uzz(I1,I2,I3,vc))+
		    advectionCoefficient*(uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc)+ 
					  uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc)+ 
					  uu(I1,I2,I3,wc)*uz(I1,I2,I3,vc)) 
		    )+
		  normal(I1,I2,I3,2)*(
		    nu*(uxx(I1,I2,I3,wc)+uyy(I1,I2,I3,wc)-u0xz(I1,I2,I3,uc)-u0yz(I1,I2,I3,vc))+
		    advectionCoefficient*(uu(I1,I2,I3,uc)*ux(I1,I2,I3,wc)+ 
					  uu(I1,I2,I3,vc)*uy(I1,I2,I3,wc)+ 
					  uu(I1,I2,I3,wc)*uz(I1,I2,I3,wc)) 
		    );

		if( debug() & 4 )
		{
		  realArray temp;
		  temp=(uzz(I1,I2,I3,wc)+u0.xz(I1,I2,I3,uc)(I1,I2,I3,uc)+u0.yz(I1,I2,I3,vc)(I1,I2,I3,vc));
		  display(temp,"w_zz + u_xz + v_yz on boundary:",parameters.dbase.get<FILE* >("debugFile"),"%9.2e");
	      
		  realArray temp2;
		  temp2=u0.x(I1,I2,I3,uc)(I1,I2,I3,uc)+u0.y(I1,I2,I3,vc)(I1,I2,I3,vc)+u0.z(I1,I2,I3,wc)(I1,I2,I3,wc);
		  display(temp2,"div(u):",parameters.dbase.get<FILE* >("debugFile"),"%9.2e");
	      
		  real regBC =max(fabs(PN3(I1,I2,I3)));  // old way for checking

		  real curlBC=max(fabs(f(I1g,I2g,I3g)));

		  printf("apply curl(curl) BC for p (side,axis,grid)=(%i,%i,%i): max(rhs)=%8.2e"
			 "(regular BC max(rhs)=%8.2e)\n",side,axis,grid,curlBC,regBC);
		}
	    
	      }
	    }
	  
	    // add n.( -u.t) for moving grids or time dependent BC's
	    gridAccelerationBC(grid, t0, c, u0, f, gridVelocity,normal,  // *****
                               I1,I2,I3, I1g,I2g,I3g,side,axis   );
	  }


	}
        // ::display(f(I1g,I2g,I3g),"f after C++ BC","%19.10e "); // ****************************

      }
    } // end forBoundary
    
    
  } // end if !optimized version
  
  

  if( parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    // In the TZ case we subtract off n.u_t -- but then we add it back on later -- could just skip both 

    // ***** add forcing for twilightZoneFlow *******
    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    #ifdef USE_PPP
      realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
    #else
      const realSerialArray & fLocal = f;
    #endif  
    realArray & x= c.center();
    #ifdef USE_PPP
      realSerialArray xLocal; 
      if( true || !isRectangular ) 
        getLocalArrayWithGhostBoundaries(x,xLocal);
    #else
      const realSerialArray & xLocal = x;
    #endif

    int side,axis;
    Index I1g,I2g,I3g;
    ForBoundary(side,axis)
    {
      if( c.boundaryCondition(side,axis)>0 )
      {
	switch (c.boundaryCondition(side,axis))
	{
	case Parameters::outflow:
	case Parameters::convectiveOutflow:
	case Parameters::tractionFree:
	{
	  break;
	}
	case Parameters::inflowWithPressureAndTangentialVelocityGiven:
	case Parameters::dirichletBoundaryCondition:
	  break;	
	case Parameters::symmetry:
	case Parameters::axisymmetric:
	  break;
	default:
	  if( parameters.dbase.get<int >("pressureBoundaryCondition")==2 )
	  {
	    break;
	  }

	  getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line
	  getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line


          #ifdef USE_PPP
	   realSerialArray & normalLocal = *(c.rcData->pVertexBoundaryNormal[axis][side]); 
          #else
    	   realArray & normalLocal = c.vertexBoundaryNormal(side,axis);
          #endif

          bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3);
          if( !ok ) continue;
          ParallelUtility::getLocalArrayBounds(f,fLocal,I1g,I2g,I3g); // is this right?

	  if( c.numberOfDimensions()==1 )
	  {
	    realSerialArray u0t(I1,I2,I3); 
	    e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t0);
	    fLocal(I1g,I2g,I3g)-=(2*side-1)*u0t(I1,I2,I3);
	  }
	  else if( c.numberOfDimensions()==2 )
	  {
	    realSerialArray u0t(I1,I2,I3),v0t(I1,I2,I3);
	    e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t0);
	    e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t0);

	    fLocal(I1g,I2g,I3g)-=normalLocal(I1,I2,I3,0)*u0t(I1,I2,I3)
		                +normalLocal(I1,I2,I3,1)*v0t(I1,I2,I3);
	  }
	  else
	  {
	    // 3D
	    realSerialArray u0t(I1,I2,I3),v0t(I1,I2,I3),w0t(I1,I2,I3);
	    e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t0);
	    e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t0);
	    e.gd( w0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,wc,t0);

	    fLocal(I1g,I2g,I3g)-=normalLocal(I1,I2,I3,0)*u0t(I1,I2,I3)
		                +normalLocal(I1,I2,I3,1)*v0t(I1,I2,I3)
		                +normalLocal(I1,I2,I3,2)*w0t(I1,I2,I3);

	  }
	}
      }
    }
  
    addForcingToPressureEquation( grid,c,f,gridVelocity,t0 );  // add forcing to rhs for twilight-zone flow
    
  }
  
}

  
#define P0(c,I1,I2,I3,t)  e(c,I1,I2,I3,pc,t)

#define P02N(c,I1,I2,I3,t) ( normal(I1,I2,I3,0)*p0x(I1,I2,I3) \
			     +normal(I1,I2,I3,1)*p0y(I1,I2,I3) )

#define P03N(c,I1,I2,I3,t) ( normal(I1,I2,I3,0)*p0x(I1,I2,I3) \
			    +normal(I1,I2,I3,1)*p0y(I1,I2,I3) \
			    +normal(I1,I2,I3,2)*p0z(I1,I2,I3) )

//   ---Here is the pressure equation in 2D
#define PF2(c,I1,I2,I3,t)  (  \
advectionCoefficient*( u0x*u0x+2.*u0y*v0x+v0y*v0y ) + p0xx + p0yy )

//    ---Here is the pressure equation in 3D
#define PF3(c,I1,I2,I3,t)  (  \
           advectionCoefficient*( u0x*u0x+v0y*v0y+w0z*w0z+2.*(u0y*v0x+u0z*w0x+v0z*w0y) )+p0xx+p0yy+p0zz \
                           )

//     ---Here are the momentum equations in 2D
#define FB21(c,I1,I2,I3,t) (  \
         u0t(I1,I2,I3)                            \
        +uuLocal(I1,I2,I3,uc)*u0x(I1,I2,I3) \
        +uuLocal(I1,I2,I3,vc)*u0y(I1,I2,I3)    \
        +p0x(I1,I2,I3)                                            \
        -nu*(u0xx+u0yy)        \
                   )
#define FB22(c,I1,I2,I3,t)  (   \
        v0t(I1,I2,I3)             \
       +uuLocal(I1,I2,I3,uc)*v0x(I1,I2,I3)     \
       +uuLocal(I1,I2,I3,vc)*v0y(I1,I2,I3)     \
       +p0y(I1,I2,I3) \
       -nu*(v0xx+v0yy)        \
                           )  
//     ...normal component of the momentum equations
#define FB2N(c,I1,I2,I3,t) ( normal(I1,I2,I3,0)*FB21(c,I1,I2,I3,t)   \
                            +normal(I1,I2,I3,1)*FB22(c,I1,I2,I3,t) )

//    ---Here are the momentum equations in 3D
#define FB31(c,I1,I2,I3,t) (  \
         u0t(I1,I2,I3)                          \
        +uuLocal(I1,I2,I3,uc)*u0x(I1,I2,I3)     \
        +uuLocal(I1,I2,I3,vc)*u0y(I1,I2,I3)     \
        +uuLocal(I1,I2,I3,wc)*u0z(I1,I2,I3)     \
        +p0x(I1,I2,I3)                                            \
        -nu*(u0xx+u0yy+u0zz)        \
                   )
#define FB32(c,I1,I2,I3,t)  (   \
         v0t(I1,I2,I3)            \
        +uuLocal(I1,I2,I3,uc)*v0x(I1,I2,I3)     \
        +uuLocal(I1,I2,I3,vc)*v0y(I1,I2,I3)     \
        +uuLocal(I1,I2,I3,wc)*v0z(I1,I2,I3)     \
        +p0y(I1,I2,I3) \
        -nu*(v0xx+v0yy+v0zz)        \
                           )  
#define FB33(c,I1,I2,I3,t)  (   \
         w0t(I1,I2,I3)                        \
        +uuLocal(I1,I2,I3,uc)*w0x(I1,I2,I3)    \
        +uuLocal(I1,I2,I3,vc)*w0y(I1,I2,I3)     \
        +uuLocal(I1,I2,I3,wc)*w0z(I1,I2,I3)     \
        +p0z(I1,I2,I3) \
        -nu*(w0xx+w0yy+w0zz)        \
                           )  
//     ...normal component of the momentum equations
#define FB3N(c,I1,I2,I3,t) ( normal(I1,I2,I3,0)*FB31(c,I1,I2,I3,t)   \
                            +normal(I1,I2,I3,1)*FB32(c,I1,I2,I3,t)   \
                            +normal(I1,I2,I3,2)*FB33(c,I1,I2,I3,t) )

//======================================================================
//   Add the forcing to the pressure equation for
//        Twilightzone flow for modeltb=0
//
// NOTE
//  o  mometum equations are used here for the pressure BC
//
//  Input -
//    f  : rhs for pressure equation before forcing is added
//  Output -
//    f  : rhs for pressure equation after forcing is added
//======================================================================

void OB_MappedGridSolver::
addForcingToPressureEquation( const int & grid,
			      MappedGrid & c, 
			      realMappedGridFunction & f,  
			      realMappedGridFunction & gridVelocity, 
			      const real & t )
{
  if( !parameters.dbase.get<bool >("twilightZoneFlow") )
    return;

  MappedGrid & mg =c;

  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & pc = parameters.dbase.get<int >("pc");
  const int & tc = parameters.dbase.get<int >("tc");
  const int nc=parameters.dbase.get<int >("kc");
  const int kc=parameters.dbase.get<int >("kc");
  const int ec=kc+1;
  const int numberOfDimensions = c.numberOfDimensions();
  
  realArray & uu = get(WorkSpace::uu);

  const real & nu  = parameters.dbase.get<real >("nu");
  const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");
  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
  const bool gridIsMoving = parameters.gridIsMoving(grid);
  
  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

  if( debug() & 32 )
    cout << " ***Entering addForcingToPressureEquation *** \n";

  if( debug() & 8 )
  {
    display(f,sPrintF("addForcingToPressureEquation: rhs f, grid=%i, before adding TZ",grid),
             parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
  }


  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index I1g,I2g,I3g;
  Range R[3];

  //     ..assign all interior points
 
  int side,axis;
  realSerialArray radiusInverse;

  real thermalExpansivity=1.;
  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
  const real *gravity=parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

  #ifdef USE_PPP
    realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
    // intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
  #else
    realSerialArray & fLocal = f;
    // const intSerialArray & mask = mg.mask();
  #endif  
  realArray & x= c.center();
  #ifdef USE_PPP
    realSerialArray xLocal; 
    if( !isRectangular ) 
      getLocalArrayWithGhostBoundaries(x,xLocal);
  #else
    const realSerialArray & xLocal = x;
  #endif


  bool evaluteTZ=tzTimeStart2==REAL_MAX;  // set to true if we need to evaluate the TZ functions

  // ****NOTE: the tzForcing arrays arrays are shared with addForcingINS
  const int numberOfTZArrays=c.numberOfDimensions()==1 ? 1 : c.numberOfDimensions()==2 ? 10 : 14;
  if( tzForcing==NULL )
  {
    evaluteTZ=true;
    
    tzForcing = new realSerialArray [numberOfTZArrays];
    int extra=1;
    getIndex(extendedGridIndexRange(c),I1,I2,I3,extra); // allocate space to hold  BC forcing in ghost points
    bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3); 
    if( ok )
    {
      tzForcing[0].redim(I1,I2,I3);
      tzForcing[1].redim(I1,I2,I3);
      tzForcing[2].redim(I1,I2,I3);
    }
  }
  // we cannot use the opt evaluation for moving grids since the grid points change
  if( gridIsMoving )
    evaluteTZ=true;  // we are forced to re-evaluate the TZ functions every time step

  real scaleFactor=1., scaleFactorT=1.;
  if( evaluteTZ )
  {
    tzTimeStart2=t;  // save the time at which the TZ functions were evaluated
  }
  else 
  {
    // This is not the first time through -- compute scale factors for stored TZ values

    // Here we assume that the TZ function is a tensor product of a spatial function
    // times a function of time. In this case we just need to scale the TZ function
    // by the new value of the time function
    real xa=.123,ya=.456,za= c.numberOfDimensions()==2 ? .789 : 0.;
    real ta=tzTimeStart2;
	
    scaleFactor = e(xa,ya,za,pc,t)/e(xa,ya,za,pc,ta); // we assume all time functions are the same

    real sfta=e.t(xa,ya,za,uc,ta);
    if( fabs(sfta)>REAL_EPSILON*100. )
      scaleFactorT=e.t(xa,ya,za,uc,t)/sfta;
    else
      scaleFactorT=1.;

//      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
//      {
//        real scaleFactorU = e(xa,ya,za,uc,t)/e(xa,ya,za,uc,ta);
//        real scaleFactorK = e(xa,ya,za,kc,t)/e(xa,ya,za,kc,ta);
//        real scaleFactorE = e(xa,ya,za,ec,t)/e(xa,ya,za,ec,ta);
//        printf(" t=%8.2e, scaleFactor=%12.10f, scaleFactorU/sf=%12.10f, scaleFactorK/sf=%12.10f, scaleFactorE/sf=%12.10f\n",
//  	     t,scaleFactor,scaleFactorU/scaleFactor,scaleFactorK/scaleFactor,scaleFactorE/scaleFactor);
//      }
    
  }
  assert( fabs(scaleFactor)<1.e10 &&  fabs(scaleFactorT)<1.e10 );

  real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0;
  real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI;



  #ifdef USE_PPP
    bool useOpt=true;
  #else
    bool useOpt=false || parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel 
                      || parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel;
  #endif
  if( useOpt ) // new version for parallel -- needs to be finished for other cases
  {
    MappedGrid & mg = c;
    
    getIndex(extendedGridIndexRange(c),I1,I2,I3);


    if( !isRectangular )
      mg.update(MappedGrid::THEcenter);

    // loop bounds for this boundary:
    bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3);  

    if( ok )
    {

      if( c.numberOfDimensions()==2 )
      {
        // ***************************************************
        // **************** Two-Dimensions *******************
        // ***************************************************
	realSerialArray u0x(I1,I2,I3),u0y(I1,I2,I3);
	realSerialArray v0x(I1,I2,I3),v0y(I1,I2,I3);
	realSerialArray p0xx(I1,I2,I3),p0yy(I1,I2,I3);
	
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);

	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);

	e.gd( p0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,pc,t);
	e.gd( p0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,pc,t);


	fLocal(I1,I2,I3)+=advectionCoefficient*( u0x*u0x+2.*u0y*v0x+v0y*v0y ) + p0xx + p0yy;


	if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{
	  // visco-plastic model

          // declare and lookup visco-plastic parameters (macro)
          declareViscoPlasticParameters;

          printf(" **insp:addForcing: nuViscoPlastic=%10.2e\n",nuViscoPlastic);

	  realSerialArray nuT(I1,I2,I3),nuTx(I1,I2,I3),nuTy(I1,I2,I3),
                          nuTxx(I1,I2,I3),nuTxy(I1,I2,I3),nuTyy(I1,I2,I3),nuTd(I1,I2,I3),nuTdd(I1,I2,I3);
	  realSerialArray eDotNorm(I1,I2,I3),exp0(I1,I2,I3),
                          n0x(I1,I2,I3),n0y(I1,I2,I3),n0xx(I1,I2,I3),n0xy(I1,I2,I3),n0yy(I1,I2,I3); 

          realSerialArray u0xx(I1,I2,I3),u0xy(I1,I2,I3),u0yy(I1,I2,I3); 
          realSerialArray u0xxx(I1,I2,I3),u0xxy(I1,I2,I3),u0xyy(I1,I2,I3),u0yyy(I1,I2,I3); 

          realSerialArray v0xx(I1,I2,I3),v0xy(I1,I2,I3),v0yy(I1,I2,I3); 
          realSerialArray v0xxx(I1,I2,I3),v0xxy(I1,I2,I3),v0xyy(I1,I2,I3),v0yyy(I1,I2,I3); 


	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	  e.gd( u0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,uc,t);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	  e.gd( u0xxx,xLocal,numberOfDimensions,isRectangular,0,3,0,0,I1,I2,I3,uc,t);
	  e.gd( u0xxy,xLocal,numberOfDimensions,isRectangular,0,2,1,0,I1,I2,I3,uc,t);
	  e.gd( u0xyy,xLocal,numberOfDimensions,isRectangular,0,1,2,0,I1,I2,I3,uc,t);
	  e.gd( u0yyy,xLocal,numberOfDimensions,isRectangular,0,0,3,0,I1,I2,I3,uc,t);


	  e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	  e.gd( v0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,vc,t);
	  e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

	  e.gd( v0xxx,xLocal,numberOfDimensions,isRectangular,0,3,0,0,I1,I2,I3,vc,t);
	  e.gd( v0xxy,xLocal,numberOfDimensions,isRectangular,0,2,1,0,I1,I2,I3,vc,t);
	  e.gd( v0xyy,xLocal,numberOfDimensions,isRectangular,0,1,2,0,I1,I2,I3,vc,t);
	  e.gd( v0yyy,xLocal,numberOfDimensions,isRectangular,0,0,3,0,I1,I2,I3,vc,t);


	  // eDotNorm = sqrt( u0x*u0x + u0y*u0y + v0x*v0x + v0y*v0y )+ epsViscoPlastic;
          eDotNorm = strainRate2d();

          defineViscoPlasticCoefficientsAndTwoDerivatives(eDotNorm);

// 	  nuT = nu+ nuViscoPlastic*eDotNormSq;   // fake form 
// 	  nuTd=nuViscoPlastic;   // d(nuT)/d(eDotNormSq)
// 	  nuTdd=0; // d^2(nuT)/d(eDotNormSq^2)

	  // n0x = 2.*( u0x*u0xx + u0y*u0xy + v0x*v0xx + v0y*v0xy );
	  // n0y = 2.*( u0x*u0xy + u0y*u0yy + v0x*v0xy + v0y*v0yy );
	  n0x = strainRate2dSqx(); 
	  n0y = strainRate2dSqy(); 

	  nuTx=nuTd*n0x; 
	  nuTy=nuTd*n0y; 

//	  n0xx = 2.*(u0x*u0xxx + u0y*u0xxy + v0x*v0xxx + v0y*v0xxy + SQR(u0xx) + SQR(u0xy) + SQR(v0xx) + SQR(v0xy) );
//	  n0xy = 2.*(u0x*u0xxy + u0y*u0xyy + v0x*v0xxy + v0y*v0xyy + u0xy*u0xx + u0yy*u0xy + v0xy*v0xx + v0yy*v0xy );
//	  n0yy = 2.*(u0x*u0xyy + u0y*u0yyy + v0x*v0xyy + v0y*v0yyy + SQR(u0xy) + SQR(u0yy) + SQR(v0xy) + SQR(v0yy) );
	  n0xx = strainRate2dSqxx();
	  n0xy = strainRate2dSqxy();
	  n0yy = strainRate2dSqyy();
	  nuTxx=(n0xx*nuTd+n0x*n0x*nuTdd);
	  nuTxy=(n0xy*nuTd+n0x*n0y*nuTdd);
	  nuTyy=(n0yy*nuTd+n0y*n0y*nuTdd);

	  fLocal(I1,I2,I3)-=2.*( nuTx*(u0xx+u0yy)+nuTxx*u0x+nuTxy*u0y+ nuTy*(v0xx+v0yy)+nuTxy*v0x+nuTyy*v0y);

//                    nuT=nu + nuVP*eDotNorm**2
//                    nuTx=nuVP*2.*( u0x*u0xx + u0y*u0xy + v0x*v0xx + v0y*
//      & v0xy )
//                    nuTy=nuVP*2.*( u0x*u0xy + u0y*u0yy + v0x*v0xy + v0y*
//      & v0yy )
//                    nuTxx=nuVP*2.*( u0x*u0xxx + u0y*u0xxy + v0x*v0xxx + 
//      & v0y*v0xxy + u0xx**2   + u0xy**2   + v0xx**2   + v0xy**2 )
//                    nuTxy=nuVP*2.*( u0x*u0xxy + u0y*u0xyy + v0x*v0xxy + 
//      & v0y*v0xyy + u0xy*u0xx + u0yy*u0xy + v0xy*v0xx + v0yy*v0xy )
//                    nuTyy=nuVP*2.*( u0x*u0xyy + u0y*u0yyy + v0x*v0xyy + 
//      & v0y*v0yyy + u0xy**2   + u0yy**2   + v0xy**2   + v0yy**2 )
//                 f(i1,i2,i3)=(-advectionCoefficient)*(u0x**2+2.*u0y*v0x+
//      & v0y**2)+divDamping(i1,i2,i3)*(u0x+v0y)+2.*(nuTx*u0Lap+nuTxx*
//      & u0x+nuTxy*u0y+nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y)


	}
	
        // *** this must be done last since we over-write u0x, ... ***
        if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel || 
            parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{ // add terms for Boussinesq approximation
          // Evaluate T_x, T_y and save in u0x, u0y
	  e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
          fLocal(I1,I2,I3)+=thermalExpansivity*(gravity[0]*u0x+gravity[1]*u0y);
	}
	

      }
      else if( c.numberOfDimensions()==3 )
      {

	realSerialArray u0x(I1,I2,I3),u0y(I1,I2,I3),u0z(I1,I2,I3);
	realSerialArray v0x(I1,I2,I3),v0y(I1,I2,I3),v0z(I1,I2,I3);
	realSerialArray w0x(I1,I2,I3),w0y(I1,I2,I3),w0z(I1,I2,I3);
	realSerialArray p0xx(I1,I2,I3),p0yy(I1,I2,I3),p0zz(I1,I2,I3);
	
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,uc,t);

	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	e.gd( v0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,vc,t);

	e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
	e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
	e.gd( w0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,wc,t);

	e.gd( p0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,pc,t);
	e.gd( p0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,pc,t);
	e.gd( p0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,pc,t);

	fLocal(I1,I2,I3)+=advectionCoefficient*
	  ( u0x*u0x+v0y*v0y+w0z*w0z+2.*(u0y*v0x+u0z*w0x+v0z*w0y) )  +p0xx+p0yy+p0zz;

	if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{     
          Overture::abort("viscoPlasticModel in 3D: Option no implemented yet");
	}
	

        // *** this must be done last since we over-write u0x, ... ***
        if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel || 
            parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{ // add terms for Boussinesq approximation
          // Evaluate T_x, T_y, T_z and save in u0x, u0y and u0z
	  e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
  	  e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,tc,t);
          fLocal(I1,I2,I3)+=thermalExpansivity*(gravity[0]*u0x+gravity[1]*u0y+gravity[2]*u0z);
	}


      }	
      else
      {
	Overture::abort("error");
      }
    }
  }  
  else // if( useOpt )
  {
  
    getIndex(extendedGridIndexRange(c),I1,I2,I3);
    if( !isRectangular )
      mg.update(MappedGrid::THEcenter);

    // loop bounds for this boundary:
    bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3);  

    if( ok && evaluteTZ )
    {
      realSerialArray u0x(I1,I2,I3),u0y(I1,I2,I3);
      realSerialArray v0x(I1,I2,I3),v0y(I1,I2,I3);
      realSerialArray p0xx(I1,I2,I3),p0yy(I1,I2,I3);
	
      e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
      e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);

      e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
      e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);

      e.gd( p0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,pc,t);
      e.gd( p0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,pc,t);


      if( c.numberOfDimensions()==2 )
      {
	tzForcing[8] = advectionCoefficient*( u0x*u0x+2.*u0y*v0x+v0y*v0y );
	tzForcing[9] = p0xx+p0yy;
      }
      else
      {
	realSerialArray u0z(I1,I2,I3),v0z(I1,I2,I3),w0x(I1,I2,I3),w0y(I1,I2,I3),w0z(I1,I2,I3),p0zz(I1,I2,I3);
	e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,uc,t);
	e.gd( v0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,vc,t);

	e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
	e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
	e.gd( w0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,wc,t);

	e.gd( p0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,pc,t);

	tzForcing[12] =advectionCoefficient*( u0x*u0x+v0y*v0y+w0z*w0z+2.*(u0y*v0x+u0z*w0x+v0z*w0y) );
	tzForcing[13] =p0xx+p0yy+p0zz;

      }
    
    }


#ifndef USE_PPP
    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
    {
      getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0);

      assert( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL );
      const realArray & d = (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];
	
      const realArray & u0  = e   (c,I1,I2,I3,uc,t);
      const realArray & u0x = e.x (c,I1,I2,I3,uc,t);
      const realArray & u0y = e.y (c,I1,I2,I3,uc,t);
      const realArray & u0Lap= e.laplacian(c,I1,I2,I3,uc,t);

      const realArray & v0  = e   (c,I1,I2,I3,vc,t);
      const realArray & v0x = e.x (c,I1,I2,I3,vc,t);
      const realArray & v0y = e.y (c,I1,I2,I3,vc,t);
      const realArray & v0Lap= e.laplacian(c,I1,I2,I3,vc,t);

      const realArray & n0   = e  (c,I1,I2,I3,nc,t);
      const realArray & n0x  = e.x(c,I1,I2,I3,nc,t);
      const realArray & n0y  = e.y(c,I1,I2,I3,nc,t);
      const realArray & n0xx = e.xx(c,I1,I2,I3,nc,t);
      const realArray & n0xy = e.xy(c,I1,I2,I3,nc,t);
      const realArray & n0yy = e.yy(c,I1,I2,I3,nc,t);
        

      realArray nuT,chi,chi3,nuTx,nuTy,nuTxx,nuTxy,nuTd,nuTyy,nuTdd;
      chi=n0/nu;
      chi3 = pow(chi,3.);

      nuT = nu+n0*(chi3/(chi3+cv1e3)); // *** this is a funny scaling *** 
      nuTd=chi3*(chi3+4.*cv1e3)/pow(chi3+cv1e3,2.);
      nuTx= n0x*nuTd;// ******************
      nuTy= n0y*nuTd;// ******************

      nuTdd= (6./nu)*chi*chi*cv1e3*(-chi3+2.*cv1e3)/pow(chi3+cv1e3,3.); // this is really nuTdd/nu : from spal.maple

      nuTxx=n0xx*nuTd+n0x*n0x*nuTdd;
      nuTxy=n0xy*nuTd+n0x*n0y*nuTdd;
      nuTyy=n0yy*nuTd+n0y*n0y*nuTdd;
    
      if( c.numberOfDimensions()==2 )
      {
	// [8]=quadratic part, [9]=linear
	f(I1,I2,I3)-=2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+ nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y);
	// These do not scale in an easy way
	// tzForcing[9]-= 2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+ nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y);
      }
      else
      {

	const realArray & u0z = e.z (c,I1,I2,I3,uc,t);
	const realArray & v0z = e.z (c,I1,I2,I3,vc,t);

	const realArray & w0  = e   (c,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (c,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (c,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (c,I1,I2,I3,wc,t);
	const realArray & w0Lap= e.laplacian(c,I1,I2,I3,wc,t);

	const realArray & n0z  = e.z (c,I1,I2,I3,nc,t);
	const realArray & n0xz = e.xz(c,I1,I2,I3,nc,t);
	const realArray & n0yz = e.yz(c,I1,I2,I3,nc,t);
	const realArray & n0zz = e.zz(c,I1,I2,I3,nc,t);

	realArray nuTz,nuTxz,nuTyz,nuTzz;
      
	nuTz=n0z*nuTd;
	nuTxz=n0xz*nuTd+n0x*n0z*nuTdd;
	nuTyz=n0yz*nuTd+n0y*n0z*nuTdd;
	nuTzz=n0zz*nuTd+n0z*n0z*nuTdd;
      
	f(I1,I2,I3)-=2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+
			  nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+
			  nuTz*w0Lap+nuTxz*w0x+nuTyz*w0y+nuTzz*w0z );
      }
    

    }
    else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
    {
      getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI );

      const realArray & u0  = e   (c,I1,I2,I3,uc,t);
      const realArray & u0x = e.x (c,I1,I2,I3,uc,t);
      const realArray & u0y = e.y (c,I1,I2,I3,uc,t);
      const realArray & u0Lap= e.laplacian(c,I1,I2,I3,uc,t);

      const realArray & v0  = e   (c,I1,I2,I3,vc,t);
      const realArray & v0x = e.x (c,I1,I2,I3,vc,t);
      const realArray & v0y = e.y (c,I1,I2,I3,vc,t);
      const realArray & v0Lap= e.laplacian(c,I1,I2,I3,vc,t);

      const realArray & k0   = e   (c,I1,I2,I3,kc,t);
      const realArray & k0x  = e.x (c,I1,I2,I3,kc,t);
      const realArray & k0y  = e.y (c,I1,I2,I3,kc,t);
      const realArray & k0xx = e.xx(c,I1,I2,I3,kc,t);
      const realArray & k0xy = e.xy(c,I1,I2,I3,kc,t);
      const realArray & k0yy = e.yy(c,I1,I2,I3,kc,t);

      const realArray & e0   = e   (c,I1,I2,I3,ec,t);
      const realArray & e0x  = e.x (c,I1,I2,I3,ec,t);
      const realArray & e0y  = e.y (c,I1,I2,I3,ec,t);
      const realArray & e0xx = e.xx(c,I1,I2,I3,ec,t);
      const realArray & e0xy = e.xy(c,I1,I2,I3,ec,t);
      const realArray & e0yy = e.yy(c,I1,I2,I3,ec,t);
        

      realArray nuT,nuTx,nuTy,nuTxx,nuTxy,nuTyy,nuTdd,e02,e03,k02;

      e02=e0*e0;
      e03=e02*e0;
      k02=k0*k0;
    
      nuT = nu + cMu*k02/e0;
      nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e02;
      nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e02;

      nuTxx=cMu*(2.*k0xx*e02-4.*k0*k0x*e0x*e0+2*k0*k0xx*e02+2*k02*e0x*e0x-k02*e0xx*e0)/e03;
      nuTxy=cMu*(2*k0y*k0x*e02-2*k0*k0x*e0y*e0+2*k0*k0xy*e02-2*k0*e0x*k0y*e0+2*k02*e0x*e0y-k02*e0xy*e0)/e03;
      nuTyy=cMu*(2.*k0yy*e02-4.*k0*k0y*e0y*e0+2*k0*k0yy*e02+2*k02*e0y*e0y-k02*e0yy*e0)/e03;

      if( c.numberOfDimensions()==2 )
      {
	f(I1,I2,I3)-= 2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+ nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y);
      }
      else
      {

	const realArray & u0z = e.z (c,I1,I2,I3,uc,t);
	const realArray & v0z = e.z (c,I1,I2,I3,vc,t);

	const realArray & w0  = e   (c,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (c,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (c,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (c,I1,I2,I3,wc,t);
	const realArray & w0Lap= e.laplacian(c,I1,I2,I3,wc,t);

	const realArray & k0z  = e.z (c,I1,I2,I3,kc,t);
	const realArray & k0xz = e.xz(c,I1,I2,I3,kc,t);
	const realArray & k0yz = e.yz(c,I1,I2,I3,kc,t);
	const realArray & k0zz = e.zz(c,I1,I2,I3,kc,t);

	const realArray & e0z  = e.z (c,I1,I2,I3,ec,t);
	const realArray & e0xz = e.xz(c,I1,I2,I3,ec,t);
	const realArray & e0yz = e.yz(c,I1,I2,I3,ec,t);
	const realArray & e0zz = e.zz(c,I1,I2,I3,ec,t);

	realArray nuTz,nuTxz,nuTyz,nuTzz;
      
	nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e02;
	nuTxz=cMu*(2*k0z*k0x*e02-2*k0*k0x*e0z*e0+2*k0*k0xz*e02-2*k0*e0x*k0z*e0+2*k02*e0x*e0z-k02*e0xz*e0)/e03;
	nuTyz=cMu*(2*k0z*k0y*e02-2*k0*k0y*e0z*e0+2*k0*k0yz*e02-2*k0*e0y*k0z*e0+2*k02*e0y*e0z-k02*e0yz*e0)/e03;
	nuTzz=cMu*(2.*k0zz*e02-4.*k0*k0z*e0z*e0+2*k0*k0zz*e02+2*k02*e0z*e0z-k02*e0zz*e0)/e03;
    

	f(I1,I2,I3)-= 2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+
			   nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+
			   nuTz*w0Lap+nuTxz*w0x+nuTyz*w0y+nuTzz*w0z );
      }
    }
    else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel )
    {
      Overture::abort("insp: turbulence model not implemented");
    }
#endif
  
    // ******************************************************
    // ***** add in the contributions times a scale factor **
    // ******************************************************
    if( ok )
    {
      if( c.numberOfDimensions()==2 )
      {
	// f(I1,I2,I3)+=advectionCoefficient*( u0x*u0x+2.*u0y*v0x+v0y*v0y ) + p0xx + p0yy;
	fLocal(I1,I2,I3)+=tzForcing[8]*SQR(scaleFactor)+tzForcing[9]*scaleFactor;

	if( parameters.isAxisymmetric() )
	  radiusInverse=1./max(REAL_MIN,xLocal(I1,I2,I3,axis2)); // this is used in fixup below 
      }
      else
      {

	// f(I1,I2,I3)+=advectionCoefficient*( u0x*u0x+v0y*v0y+w0z*w0z+2.*(u0y*v0x+u0z*w0x+v0z*w0y) )+p0xx+p0yy+p0zz;
	fLocal(I1,I2,I3)+=tzForcing[12]*SQR(scaleFactor)+tzForcing[13]*scaleFactor;
    
      }
    
      //  ---- Fixup axisymmetric ---
      if( c.numberOfDimensions()==2 && parameters.isAxisymmetric() )
      {
	// Make this fix first so it doesn't interfere with any adjacent dirichlet conditions
	ForBoundary(side,axis)
	{
	  if( c.boundaryCondition(side,axis)==Parameters::axisymmetric )
	  {
	    getGhostIndex( c.extendedIndexRange(),side,axis,I1,I2,I3,0);     // boundary line
	    bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3);
	    if( ok )
	    {
	      realSerialArray p0yy(I1,I2,I3);
	      e.gd( p0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,pc,t);
	    
	      fLocal(I1,I2,I3)+=p0yy;   // p.y/y = p.yy on y==0
	      radiusInverse(I1,I2,I3)=0.;      // this will zero out p.y/y term on axis boundary in statement below
	    }
	    
	  }
	}

	// add p.y/y term (except on the axis where radiusInverse has been set to zero)
	getIndex(extendedGridIndexRange(c),I1,I2,I3);
	bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3);
	if( ok )
	{
	  realSerialArray p0y; 
	  e.gd( p0y,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);

	  fLocal(I1,I2,I3)+=p0y*radiusInverse(I1,I2,I3);
	}
	
      }
    }
    
  }

  //     ----apply the boundary condition----

  if( debug() & 8 )
  {
    display(f,"addForcingToPressureEquation: pressure RHS before BC",parameters.dbase.get<FILE* >("debugFile"),"%9.2e");
    #ifndef USE_PPP
      display(f(I1,I2,I3)-e.laplacian(c,I1,I2,I3,pc,t),"pressure RHS - p.laplacian",parameters.dbase.get<FILE* >("debugFile"),"%9.2e");
    #endif
  }
    

  #ifndef USE_PPP
    c.update(MappedGrid::THEvertexBoundaryNormal); // *wdh* 040824
  #endif
  
  #ifdef USE_PPP
    realSerialArray gridVelocityLocal,uuLocal;
    if( gridIsMoving )
    {
      getLocalArrayWithGhostBoundaries(gridVelocity,gridVelocityLocal);
      getLocalArrayWithGhostBoundaries(uu,uuLocal);
    }
  #else
    realSerialArray & gridVelocityLocal = gridVelocity;
    realSerialArray & uuLocal = uu;
  #endif    
  ForBoundary(side,axis)
  {
    if( c.boundaryCondition(side,axis) > 0 )
    {
      getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line
      getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line

      // getGhostIndex( extendedGridIndexRange(c),side,axis,I1g,I2g,I3g,1);  // first ghost line
      // getGhostIndex( extendedGridIndexRange(c),side,axis,I1 ,I2 ,I3 ,0);     // boundary line


      bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3);
      if( !ok ) continue;
      ParallelUtility::getLocalArrayBounds(f,fLocal,I1g,I2g,I3g); // is this right?

      #ifdef USE_PPP
       realSerialArray & normal = *(c.rcData->pVertexBoundaryNormal[axis][side]); 
      #else
       realArray & normal = c.vertexBoundaryNormal(side,axis);
      #endif

      switch (c.boundaryCondition(side,axis))
      {
      case Parameters::outflow:
      case Parameters::convectiveOutflow:
      case Parameters::tractionFree:
      {
	realSerialArray p0(I1,I2,I3),p0x(I1,I2,I3),p0y(I1,I2,I3);
	e.gd( p0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,pc,t);
	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);

        bool applyNeumannBC = bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid)!=0.;

        const real a0=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*1,side,axis,grid);
	const real a1=bcData(pc+parameters.dbase.get<int >("numberOfComponents")*2,side,axis,grid);

	if( c.numberOfDimensions()==2 )
	{
	  if( applyNeumannBC )
	  {
	    fLocal(I1g,I2g,I3g)=a0*p0(I1,I2,I3)+a1*P02N(c,I1,I2,I3,t);
	  }
	  else
	  {
	    fLocal(I1,I2,I3)=a0*p0(I1,I2,I3);
            fLocal(I1g,I2g,I3g)=0.;  // for extrapolation
	  }
	}
        else
	{
          if( applyNeumannBC )
	  {
	    realSerialArray p0z(I1,I2,I3);
	    e.gd( p0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,pc,t);

	    fLocal(I1g,I2g,I3g)=a0*p0(I1,I2,I3)+a1*P03N(c,I1,I2,I3,t);
	  }
	  else
	  {
	    fLocal(I1,I2,I3)=a0*p0(I1,I2,I3);
            fLocal(I1g,I2g,I3g)=0.;  // for extrapolation
	  }
	}

        break;
      }
      case Parameters::inflowWithPressureAndTangentialVelocityGiven:
      case Parameters::dirichletBoundaryCondition:
      {
	// const realArray & p0  = e   (c,I1,I2,I3,pc,t);
        
	// f(I1,I2,I3)=e(c,I1,I2,I3,pc,t);
        e.gd( fLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,pc,t);
        break;
      }
      case Parameters::axisymmetric:
      {
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);

        fLocal(I1g,I2g,I3g)=P02N(c,I1,I2,I3,t);    // for p.n BC
        break;
      }
      case Parameters::symmetry:
      {
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);
        if( c.numberOfDimensions()==2 )
  	  fLocal(I1g,I2g,I3g)=P02N(c,I1,I2,I3,t);  // give normal component pressure
        else if( c.numberOfDimensions()==3 )
	{
	  realSerialArray p0z(I1,I2,I3);
	  e.gd( p0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,pc,t);
  	  fLocal(I1g,I2g,I3g)=P03N(c,I1,I2,I3,t);  // give normal component pressure
	}
        break;
      }
      default:
      {

	if( parameters.dbase.get<int >("pressureBoundaryCondition")==2 )
	{ // give  give p.n=P.n
          // printf("give p.n=P.n \n");
	  
	  realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	  e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	  e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);

	  if( c.numberOfDimensions()==2 )
	  {
	    fLocal(I1g,I2g,I3g)=(normal(I1,I2,I3,0)*p0x(I1,I2,I3)+
				 normal(I1,I2,I3,1)*p0y(I1,I2,I3));
	  }
	  else
	  {
	    realSerialArray p0z(I1,I2,I3);
	    e.gd( p0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,pc,t);

	    fLocal(I1g,I2g,I3g)=(normal(I1,I2,I3,0)*p0x(I1,I2,I3)+
				 normal(I1,I2,I3,1)*p0y(I1,I2,I3)+
				 normal(I1,I2,I3,2)*p0z(I1,I2,I3));
	  }
	}
	else
	{
          // ******************************************
          // *************p.n = n.( NS )***************
          // ******************************************
        

	  if( c.numberOfDimensions()==2 )
	  {
	    if( parameters.gridIsMoving(grid) )
	    {
              // ******************************************
              // *******Moving Grid************************
              // ******************************************

              assert( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel );
	      
	      realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3);
	      realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
	      realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	
	      realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3);
	      realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3);

	      e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	      e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	      e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	      e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	      e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	      e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	      e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	      e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	      e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	      e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	      e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	      e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

	      e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	      e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);


	      uuLocal(I1,I2,I3,uc)=advectionCoefficient*u0(I1,I2,I3) -gridVelocityLocal(I1,I2,I3,0);
	      uuLocal(I1,I2,I3,vc)=advectionCoefficient*v0(I1,I2,I3) -gridVelocityLocal(I1,I2,I3,1);


              if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
	      {
	        fLocal(I1g,I2g,I3g)+=FB2N(c,I1,I2,I3,t);  // give normal component of momentum equations
              }
	      else
	      {
                Overture::abort("error: case not implemented");
	      }
  
	    }
	    else
	    {
//  	      uu(I1,I2,I3,uc)=advectionCoefficient*u0(I1,I2,I3);
//  	      uu(I1,I2,I3,vc)=advectionCoefficient*v0(I1,I2,I3);

	      if( evaluteTZ )
	      {
                // Save forcing at the start time
		realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3);
		realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
		realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	
		realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3);
		realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3);

		e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
		e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
		e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
		e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
		e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
		e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

		e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
		e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
		e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
		e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
		e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
		e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

		e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
		e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);


                // Save boundary forcing in the ghost points of the forcing arrays:
                // t-part goes in [0], quadratic part in [1]

		tzForcing[0](I1g,I2g,I3g) = normal(I1,I2,I3,0)*u0t(I1,I2,I3)+
		                            normal(I1,I2,I3,1)*v0t(I1,I2,I3);
		tzForcing[1](I1g,I2g,I3g) = advectionCoefficient*(
                                            normal(I1,I2,I3,0)*(u0*u0x+v0*u0y)+
		                            normal(I1,I2,I3,1)*(u0*v0x+v0*v0y));

		if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel &&
                    parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::standardModel )
		{
		  tzForcing[2](I1g,I2g,I3g) = normal(I1,I2,I3,0)*(p0x-nu*(u0xx+u0yy))+
		                              normal(I1,I2,I3,1)*(p0y-nu*(v0xx+v0yy));

		  if( parameters.isAxisymmetric() )  // *wdh* 040228
		  {
                    where( radiusInverse(I1,I2,I3)!=0. )
		    {
		      tzForcing[2](I1g,I2g,I3g)+=
			normal(I1,I2,I3,0)*(-nu*u0y*radiusInverse(I1,I2,I3))+
			normal(I1,I2,I3,1)*(-nu*radiusInverse(I1,I2,I3)*(
			               v0y(I1,I2,I3)-v0(I1,I2,I3)*radiusInverse(I1,I2,I3)));
		    }
		    otherwise()
		    {
                      // v.y/y -> v.yy   and -v/y^2 -> -(1/2)*v.yy
                      // v.y/y - v/y^2 -> .5*vyy(0)
		      tzForcing[2](I1g,I2g,I3g)+=
			normal(I1,I2,I3,0)*( (-nu)*   u0yy(I1,I2,I3) )
		       +normal(I1,I2,I3,1)*( (-.5*nu)*v0yy(I1,I2,I3) );

		    }
		  }

		}
                else
		  tzForcing[2](I1g,I2g,I3g)=0.;
	      }
              if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	      {
                // declare and lookup visco-plastic parameters (macro)
                declareViscoPlasticParameters;

		realSerialArray nuT(I1,I2,I3),nuTx(I1,I2,I3),nuTy(I1,I2,I3),nuTd(I1,I2,I3); 
		realSerialArray eDotNorm(I1,I2,I3), exp0(I1,I2,I3); 

		realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);

		realSerialArray u0x(I1,I2,I3),u0y(I1,I2,I3);
		realSerialArray v0x(I1,I2,I3),v0y(I1,I2,I3);
		realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3);
		realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3);
		realSerialArray u0xy(I1,I2,I3),v0xy(I1,I2,I3); 

		e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
		e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
		e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
		e.gd( u0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,uc,t);
		e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

		e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
		e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
		e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
		e.gd( v0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,vc,t);
		e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

		e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
		e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);

		printf(" **insp:bc:addForcing: nuViscoPlastic=%10.2e\n",nuViscoPlastic);
	  

		// eDotNorm = sqrt( u0x*u0x + u0y*u0y + v0x*v0x + v0y*v0y )+ epsViscoPlastic;
                eDotNorm = strainRate2d();
                // define nuT, nuTd: 
		defineViscoPlasticCoefficients(eDotNorm);
	  
		// nuTx=nuTd*2.*( u0x*u0xx + u0y*u0xy + v0x*v0xx + v0y*v0xy ); 
		// nuTy=nuTd*2.*( u0x*u0xy + u0y*u0yy + v0x*v0xy + v0y*v0yy ); 
    	        nuTx=nuTd*strainRate2dSqx(); 
 	        nuTy=nuTd*strainRate2dSqy();

		fLocal(I1g,I2g,I3g)+=
		  normal(I1,I2,I3,0)*(p0x-
				      (nuT*(u0xx+u0yy)-2.*nuTx*v0y+nuTy*(u0y+v0x)) ) +
		  normal(I1,I2,I3,1)*(p0y-
				      (nuT*(v0xx+v0yy)-2.*nuTy*u0x+nuTx*(v0x+u0y)) );
	      }
	      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
	      { // do nothing in this case
	      }
#ifndef USE_PPP
	      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
	      {
		const realArray & p0x = e.x (c,I1,I2,I3,pc,t);
		const realArray & p0y = e.y (c,I1,I2,I3,pc,t);
	
		const realArray & u0x = e.x (c,I1,I2,I3,uc,t);
		const realArray & u0y = e.y (c,I1,I2,I3,uc,t);
		const realArray & v0x = e.x (c,I1,I2,I3,vc,t);
		const realArray & v0y = e.y (c,I1,I2,I3,vc,t);

		const realArray & n0   = e  (c,I1,I2,I3,nc,t);
		const realArray & n0x  = e.x(c,I1,I2,I3,nc,t);
		const realArray & n0y  = e.y(c,I1,I2,I3,nc,t);

		realArray nuT,chi,chi3,nuTx,nuTy,nuTd;
		chi=n0/nu;
		chi3 = pow(chi,3.);

		nuT = nu+n0*(chi3/(chi3+cv1e3)); 
		nuTd=chi3*(chi3+4.*cv1e3)/pow(chi3+cv1e3,2.);
		nuTx= n0x*nuTd;
		nuTy= n0y*nuTd;

		// linear part goes in [2], quadratic part in [1]
//  		tzForcing[2](I1g,I2g,I3g)=normal(I1,I2,I3,0)*(p0x)+normal(I1,I2,I3,1)*(p0y);
//  		tzForcing[1](I1g,I2g,I3g)-=normal(I1,I2,I3,0)*(
//  		  (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*v0y+nuTy*(u0y+v0x)) ) +
//  		  normal(I1,I2,I3,1)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*u0x+nuTx*(v0x+u0y)) );

		f(I1g,I2g,I3g)+=
		  normal(I1,I2,I3,0)*(p0x-
				      (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*v0y+nuTy*(u0y+v0x)) ) +
		  normal(I1,I2,I3,1)*(p0y-
				      (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*u0x+nuTx*(v0x+u0y)) );

	      }
	      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
	      {

		const realArray & p0x = e.x (c,I1,I2,I3,pc,t);
		const realArray & p0y = e.y (c,I1,I2,I3,pc,t);
	
		const realArray & u0x = e.x (c,I1,I2,I3,uc,t);
		const realArray & u0y = e.y (c,I1,I2,I3,uc,t);
		const realArray & v0x = e.x (c,I1,I2,I3,vc,t);
		const realArray & v0y = e.y (c,I1,I2,I3,vc,t);

		const realArray & k0   = e  (c,I1,I2,I3,kc,t);
		const realArray & k0x  = e.x(c,I1,I2,I3,kc,t);
		const realArray & k0y  = e.y(c,I1,I2,I3,kc,t);

		const realArray & e0   = e  (c,I1,I2,I3,ec,t);
		const realArray & e0x  = e.x(c,I1,I2,I3,ec,t);
		const realArray & e0y  = e.y(c,I1,I2,I3,ec,t);

		realArray nuT,nuTx,nuTy;
		  
		nuT = nu+ cMu*k0*k0/e0;
		nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/(e0*e0);
		nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/(e0*e0);

		// linear part goes in [2], quadratic part in [1]
//  		tzForcing[2](I1g,I2g,I3g)=normal(I1,I2,I3,0)*(p0x)+normal(I1,I2,I3,1)*(p0y);
//  		tzForcing[1](I1g,I2g,I3g)-=normal(I1,I2,I3,0)*(
//  		  (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*v0y+nuTy*(u0y+v0x)) ) +
//  		  normal(I1,I2,I3,1)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*u0x+nuTx*(v0x+u0y)) );

		f(I1g,I2g,I3g)+=
		  normal(I1,I2,I3,0)*(p0x-
				      (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*v0y+nuTy*(u0y+v0x)) ) +
		  normal(I1,I2,I3,1)*(p0y-
				      (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*u0x+nuTx*(v0x+u0y)) );


	      }
#endif
	      else
	      {
		Overture::abort("insp:error: case not implemented"); 
	      }
		

	      // f(I1g,I2g,I3g)+=FB2N(c,I1,I2,I3,t);  // give normal component of momentum equations

              if( false && parameters.isAxisymmetric() )
	      {
                // ** for testing *** 
// 		const realArray & p0x = e.x (c,I1,I2,I3,pc,t);
// 		const realArray & p0y = e.y (c,I1,I2,I3,pc,t);
// 		// f(I1g,I2g,I3g)=P02N(c,I1,I2,I3,t);
//                 f(I1g,I2g,I3g)=(normal(I1,I2,I3,0)*(p0x-nu*e.laplacian(c,I1,I2,I3,uc,t))+
// 				normal(I1,I2,I3,1)*(p0y-nu*e.laplacian(c,I1,I2,I3,vc,t)));
	      }
	      else
	      {
		fLocal(I1g,I2g,I3g)+=scaleFactorT*tzForcing[0](I1g,I2g,I3g)+
		  (SQR(scaleFactor))*tzForcing[1](I1g,I2g,I3g)+ scaleFactor*tzForcing[2](I1g,I2g,I3g);
	      }
	      
	    }
	    if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel || 
                parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel)
	    { // add terms for Boussinesq approximation
              
              realSerialArray te0(I1,I2,I3);
              e.gd( te0 ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);

              fLocal(I1g,I2g,I3g)+=thermalExpansivity*te0*(
		gravity[0]*normal(I1,I2,I3,0)+gravity[1]*normal(I1,I2,I3,1));

              // f(I1g,I2g,I3g)+=thermalExpansivity*e(c,I1,I2,I3,tc,t)*(
	      //  gravity[0]*normal(I1,I2,I3,0)+gravity[1]*normal(I1,I2,I3,1));
	    }
	  }
	  else  // ***** 3D *****
	  {
	    if( parameters.gridIsMoving(grid) )
	    {
              // ******************************************
              // *******Moving Grid************************
              // ******************************************
	      realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3),u0z(I1,I2,I3);
	      realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3),v0z(I1,I2,I3);
	      realSerialArray w0(I1,I2,I3),w0t(I1,I2,I3),w0x(I1,I2,I3),w0y(I1,I2,I3),w0z(I1,I2,I3);
	      realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3),p0z(I1,I2,I3);
	
	      realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0zz(I1,I2,I3);
	      realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0zz(I1,I2,I3);
	      realSerialArray w0xx(I1,I2,I3),w0yy(I1,I2,I3),w0zz(I1,I2,I3);

	      e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	      e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	      e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	      e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	      e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,uc,t);
	      e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	      e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);
	      e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,uc,t);

	      e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	      e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	      e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	      e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	      e.gd( v0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,vc,t);
	      e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	      e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);
	      e.gd( v0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,vc,t);

	      e.gd( w0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,wc,t);
	      e.gd( w0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,wc,t);
	      e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
	      e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
	      e.gd( w0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,wc,t);
	      e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,t);
	      e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,t);
	      e.gd( w0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,wc,t);


	      e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	      e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);
	      e.gd( p0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,pc,t);


	      uuLocal(I1,I2,I3,uc)=advectionCoefficient*u0(I1,I2,I3)-gridVelocityLocal(I1,I2,I3,0);
	      uuLocal(I1,I2,I3,vc)=advectionCoefficient*v0(I1,I2,I3)-gridVelocityLocal(I1,I2,I3,1);
	      uuLocal(I1,I2,I3,wc)=advectionCoefficient*w0(I1,I2,I3)-gridVelocityLocal(I1,I2,I3,2);


	      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
	      {
		fLocal(I1g,I2g,I3g)+=FB3N(c,I1,I2,I3,t);  // give normal component of momentum equations
              }
	      else
	      {
                Overture::abort("error: case not implemented");
	      }

	    }
            else
            {

	      if( evaluteTZ )
	      {
                // Save forcing at the start time

		realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3),u0z(I1,I2,I3);
		realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3),v0z(I1,I2,I3);
		realSerialArray w0(I1,I2,I3),w0t(I1,I2,I3),w0x(I1,I2,I3),w0y(I1,I2,I3),w0z(I1,I2,I3);
		realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3),p0z(I1,I2,I3);
	
		realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0zz(I1,I2,I3);
		realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0zz(I1,I2,I3);
		realSerialArray w0xx(I1,I2,I3),w0yy(I1,I2,I3),w0zz(I1,I2,I3);

		e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
		e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
		e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
		e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
		e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,uc,t);
		e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
		e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);
		e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,uc,t);

		e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
		e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
		e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
		e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
		e.gd( v0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,vc,t);
		e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
		e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);
		e.gd( v0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,vc,t);

		e.gd( w0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,wc,t);
		e.gd( w0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,wc,t);
		e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
		e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
		e.gd( w0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,wc,t);
		e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,t);
		e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,t);
		e.gd( w0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,wc,t);


		e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
		e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);
		e.gd( p0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,pc,t);

				  
                // Save boundary forcing in the ghost points of the forcing arrays:
		tzForcing[0](I1g,I2g,I3g) = normal(I1,I2,I3,0)*u0t+
		                            normal(I1,I2,I3,1)*v0t+
		                            normal(I1,I2,I3,2)*w0t; 
		tzForcing[1](I1g,I2g,I3g) = advectionCoefficient*(
                                            normal(I1,I2,I3,0)*(u0*u0x+v0*u0y+w0*u0z)+
		                            normal(I1,I2,I3,1)*(u0*v0x+v0*v0y+w0*v0z)+
		                            normal(I1,I2,I3,2)*(u0*w0x+v0*w0y+w0*w0z));



		if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
		{
		  tzForcing[2](I1g,I2g,I3g) = (normal(I1,I2,I3,0)*(p0x-nu*(u0xx+u0yy+u0zz))+
					       normal(I1,I2,I3,1)*(p0y-nu*(v0xx+v0yy+v0zz))+
					       normal(I1,I2,I3,2)*(p0z-nu*(w0xx+w0yy+w0zz)) );
		}
                else
		{
                  tzForcing[2](I1g,I2g,I3g) =0.;
		}
	      }

              if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
	      {
	      }
#ifndef USE_PPP
	      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
	      {
		const realArray & p0x = e.x (c,I1,I2,I3,pc,t);
		const realArray & p0y = e.y (c,I1,I2,I3,pc,t);
		const realArray & p0z = e.z (c,I1,I2,I3,pc,t);
	
		const realArray & u0x = e.x (c,I1,I2,I3,uc,t);
		const realArray & u0y = e.y (c,I1,I2,I3,uc,t);
		const realArray & v0x = e.x (c,I1,I2,I3,vc,t);
		const realArray & v0y = e.y (c,I1,I2,I3,vc,t);
		const realArray & u0z = e.z(c,I1,I2,I3,uc,t);
		const realArray & v0z = e.z(c,I1,I2,I3,vc,t);
		const realArray & w0x = e.x(c,I1,I2,I3,wc,t);
		const realArray & w0y = e.y(c,I1,I2,I3,wc,t);
		const realArray & w0z = e.z (c,I1,I2,I3,wc,t);

		const realArray & n0   = e  (c,I1,I2,I3,nc,t);
		const realArray & n0x  = e.x(c,I1,I2,I3,nc,t);
		const realArray & n0y  = e.y(c,I1,I2,I3,nc,t);
		const realArray & n0z  = e.z(c,I1,I2,I3,nc,t);

		realArray nuT,chi,chi3,nuTx,nuTy,nuTz,nuTd;
		chi=n0/nu;
		chi3 = pow(chi,3.);

		nuT = nu+n0*(chi3/(chi3+cv1e3)); 
		nuTd=chi3*(chi3+4.*cv1e3)/pow(chi3+cv1e3,2.);
		nuTx= n0x*nuTd;
		nuTy= n0y*nuTd;
		nuTz= n0z*nuTd;

		// linear part goes in [2], quadratic part in [1]
//  		tzForcing[2](I1g,I2g,I3g) = (normal(I1,I2,I3,0)*(p0x)+
//  					     normal(I1,I2,I3,1)*(p0y)+
//  					     normal(I1,I2,I3,2)*(p0z));
//  		tzForcing[1](I1g,I2g,I3g) -=
//  		  normal(I1,I2,I3,0)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*(v0y+w0z)+nuTy*(u0y+v0x)+nuTz*(u0z+w0x)) ) +
//  		  normal(I1,I2,I3,1)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*(w0z+u0x)+nuTz*(v0z+w0y)+nuTx*(v0x+u0y)) ) +
//  		  normal(I1,I2,I3,2)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,wc,t)-2.*nuTz*(u0x+v0y)+nuTx*(w0x+u0z)+nuTy*(w0y+v0z)) );

  		 fLocal(I1g,I2g,I3g) +=
  		      normal(I1,I2,I3,0)*(p0x-
  			     (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*(v0y+w0z)+nuTy*(u0y+v0x)+nuTz*(u0z+w0x)) ) +
  		      normal(I1,I2,I3,1)*(p0y-
  			     (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*(w0z+u0x)+nuTz*(v0z+w0y)+nuTx*(v0x+u0y)) ) +
  		      normal(I1,I2,I3,2)*(p0z-
  			     (nuT*e.laplacian(c,I1,I2,I3,wc,t)-2.*nuTz*(u0x+v0y)+nuTx*(w0x+u0z)+nuTy*(w0y+v0z)) );

	      }
	      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
	      {

		const realArray & p0x = e.x (c,I1,I2,I3,pc,t);
		const realArray & p0y = e.y (c,I1,I2,I3,pc,t);
		const realArray & p0z = e.z (c,I1,I2,I3,pc,t);
	
		const realArray & u0x = e.x (c,I1,I2,I3,uc,t);
		const realArray & u0y = e.y (c,I1,I2,I3,uc,t);
		const realArray & v0x = e.x (c,I1,I2,I3,vc,t);
		const realArray & v0y = e.y (c,I1,I2,I3,vc,t);
		const realArray & u0z = e.z(c,I1,I2,I3,uc,t);
		const realArray & v0z = e.z(c,I1,I2,I3,vc,t);
		const realArray & w0x = e.x(c,I1,I2,I3,wc,t);
		const realArray & w0y = e.y(c,I1,I2,I3,wc,t);
		const realArray & w0z = e.z (c,I1,I2,I3,wc,t);

		const realArray & k0   = e  (c,I1,I2,I3,kc,t);
		const realArray & k0x  = e.x(c,I1,I2,I3,kc,t);
		const realArray & k0y  = e.y(c,I1,I2,I3,kc,t);
		const realArray & k0z  = e.z(c,I1,I2,I3,kc,t);

		const realArray & e0   = e  (c,I1,I2,I3,ec,t);
		const realArray & e0x  = e.x(c,I1,I2,I3,ec,t);
		const realArray & e0y  = e.y(c,I1,I2,I3,ec,t);
		const realArray & e0z  = e.z(c,I1,I2,I3,ec,t);

		realArray nuT,nuTx,nuTy,nuTz;
		  
		nuT = nu+ cMu*k0*k0/e0;
		nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/(e0*e0);
		nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/(e0*e0);
		nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/(e0*e0);

		// linear part goes in [2], quadratic part in [1]
//  		tzForcing[2](I1g,I2g,I3g) = (normal(I1,I2,I3,0)*(p0x)+
//  					     normal(I1,I2,I3,1)*(p0y)+
//  					     normal(I1,I2,I3,2)*(p0z));
//  		tzForcing[1](I1g,I2g,I3g) -=
//  		  normal(I1,I2,I3,0)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*(v0y+w0z)+nuTy*(u0y+v0x)+nuTz*(u0z+w0x)) ) +
//  		  normal(I1,I2,I3,1)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*(w0z+u0x)+nuTz*(v0z+w0y)+nuTx*(v0x+u0y)) ) +
//  		  normal(I1,I2,I3,2)*(
//  		    (nuT*e.laplacian(c,I1,I2,I3,wc,t)-2.*nuTz*(u0x+v0y)+nuTx*(w0x+u0z)+nuTy*(w0y+v0z)) );

  		 f(I1g,I2g,I3g) +=
  		      normal(I1,I2,I3,0)*(p0x-
  			     (nuT*e.laplacian(c,I1,I2,I3,uc,t)-2.*nuTx*(v0y+w0z)+nuTy*(u0y+v0x)+nuTz*(u0z+w0x)) ) +
  		      normal(I1,I2,I3,1)*(p0y-
  			     (nuT*e.laplacian(c,I1,I2,I3,vc,t)-2.*nuTy*(w0z+u0x)+nuTz*(v0z+w0y)+nuTx*(v0x+u0y)) ) +
  		      normal(I1,I2,I3,2)*(p0z-
  			     (nuT*e.laplacian(c,I1,I2,I3,wc,t)-2.*nuTz*(u0x+v0y)+nuTx*(w0x+u0z)+nuTy*(w0y+v0z)) );


	      }
#endif
	      else
	      {
		Overture::abort("error: case not implemented"); 
	      }

	      fLocal(I1g,I2g,I3g)+=scaleFactorT*tzForcing[0](I1g,I2g,I3g)+
		(SQR(scaleFactor))*tzForcing[1](I1g,I2g,I3g)+scaleFactor*tzForcing[2](I1g,I2g,I3g);




	    }

	    // f(I1g,I2g,I3g)=P03N(c,I1,I2,I3,t);  // give normal component of momentum equations
	    if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel ||
                parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel)
	    { // add terms for Boussinesq approximation
              realSerialArray te0(I1,I2,I3);
              e.gd( te0 ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);

              fLocal(I1g,I2g,I3g)+=thermalExpansivity*te0*(
		gravity[0]*normal(I1,I2,I3,0)+gravity[1]*normal(I1,I2,I3,1)+gravity[2]*normal(I1,I2,I3,2));
	    }
	    
	  }
	  
	} // else
      } // default:
      } // switch
    }
  } // ForBoundary
  

  if( debug() & 8 )
  {
    getIndex(c.gridIndexRange(),I1,I2,I3);

    display(f,"addForcingToPressureEquation: pressure RHS AFTER BC",parameters.dbase.get<FILE* >("debugFile"),"%9.2e");

    if( c.numberOfDimensions()==2 && parameters.isAxisymmetric() )
    {
      realArray err(I1,I2,I3);
      const realArray & y = c.vertex()(I1,I2,I3,axis2);
        
      const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
      where( fabs(y)>yEps )
      {
        err=f(I1,I2,I3)-e.laplacian(c,I1,I2,I3,pc,t)-e.y(c,I1,I2,I3,pc,t)/y;
      }
      otherwise()
      {
        err=f(I1,I2,I3)-e.laplacian(c,I1,I2,I3,pc,t)-e.yy(c,I1,I2,I3,pc,t);
      }
      display(err,"pressure RHS - p.laplacian AXISYMMETRIC",parameters.dbase.get<FILE* >("debugFile"),"%9.2e");
    }
    
  }
 
}			
			
			


    
    
