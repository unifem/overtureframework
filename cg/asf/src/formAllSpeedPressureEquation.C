#include "Cgasf.h"
#include "MappedGridOperators.h"
#include "interpPoints.h"
#include "Reactions.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "ParallelUtility.h"
#include "SparseRep.h"
#include "AsfParameters.h"

// This macro is used to loop over the boundaries
#define ForBoundary(side,axis)   for( axis=0; axis<c.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))

#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilLength0*(n))

#define FOR_4D(m,i1,i2,i3,M,I1,I2,I3) \
int mBase=M.getBase(), mBound=M.getBound(); \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++) \
for(m=mBase; m<=mBound; m++)

#define FOR_4(m,i1,i2,i3,M,I1,I2,I3) \
mBase=M.getBase(), mBound=M.getBound(); \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++) \
for(m=mBase; m<=mBound; m++)

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

extern bool refactorImplicitMatrix;
  
//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

void Cgasf::
formAllSpeedPressureEquation( GridFunction & gf0, real t, real deltaT,
                             const bool & formSteadyEquation /* =false */ )
// ==================================================================================================
// ==================================================================================================
{
  real time = getCPU();
  
  const int & rc = parameters.dbase.get<int >("rc");
  // const int & uc = parameters.dbase.get<int >("uc");
  // const int & vc = parameters.dbase.get<int >("vc");
  // const int & wc = parameters.dbase.get<int >("wc");
  const int & pc = parameters.dbase.get<int >("pc");
  const int & computeReactions = parameters.dbase.get<bool >("computeReactions");

  // const real & mu = parameters.dbase.get<real >("mu");
  const real & gamma = parameters.dbase.get<real >("gamma");
  // const real & kThermal = parameters.dbase.get<real >("kThermal");
  // const real & Rg = parameters.dbase.get<real >("Rg");
  // const real & avr = parameters.dbase.get<real >("avr");
  const real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
  // const real & nuRho = parameters.dbase.get<real >("nuRho");
  // const real & nu = parameters.dbase.get<real >("nu");
  // const real & anu = parameters.dbase.get<real >("anu");
  const real & a0 = parameters.dbase.get<real >("a0");
  // const real & a1 = parameters.dbase.get<real >("a1");
  // const real & a2 = parameters.dbase.get<real >("a2");
  // const real & b0 = parameters.dbase.get<real >("b0");
  // const real & b1 = parameters.dbase.get<real >("b1");
  // const real & b2 = parameters.dbase.get<real >("b2");

  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");

  const int & linearizeImplicitMethod = parameters.dbase.get<int >("linearizeImplicitMethod");
  const Parameters::ImplicitMethod & implicitMethod = parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");

  realCompositeGridFunction & u1 = gf0.u;
  CompositeGrid & cg = gf0.cg;
  CompositeGridOperators & operators = *(u1.getOperators());   // ****
  const int numberOfDimensions = cg.numberOfDimensions();

  cout << "formAllSpeedPressureEquation: refactor the matrix............... \n";
  refactorImplicitMatrix=FALSE;
    


// ---  turn this off for now 060926 *wdh*
//   if( linearizeImplicitMethod && prL==NULL )
//   {
//     if( prL==NULL )
//       prL=new realCompositeGridFunction;
//     rL().updateToMatchGrid(cg);                         // ************ fix this  *****
//     if( ppL==NULL )
//       ppL=new realCompositeGridFunction;
//     pL().updateToMatchGrid(cg);
//     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     {
//       rL()[grid].dataCopy(mappedGridSolver[grid]->rL());    // ************ fix this  *****
//       pL()[grid].dataCopy(mappedGridSolver[grid]->pL());
//     }
//   }
//   if( computeReactions && pgam==NULL )
//   {
//     if( pgam==NULL )
//       pgam=new realCompositeGridFunction;
//     gam().updateToMatchGrid(cg);
//     if( parameters.dbase.get<Reactions* >("reactions")!=NULL )
//     {
//       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// 	gam()[grid].dataCopy(mappedGridSolver[grid]->gam());    // ************ fix this  *****
//     }
//     else
//     {
//       // do this for now   ***** fix this ****
//       printf("ASF: setting gamma=1.4\n");
//       gam()=1.4;
//     }
    
//   }
// ****************************







  // pM : use this pressure in the matrix 
  realCompositeGridFunction & pM = linearizeImplicitMethod ? pL() : p();

  int stencilSize=int( pow(3,cg.numberOfDimensions())+1 );  
  if( movingGridProblem() )
  {
    coeff.updateToMatchGrid(cg,stencilSize);  
    int numberOfGhostLines=1; // *wdh* 2015/04/19
    coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
    operators.setStencilSize(stencilSize);
  }
  coeff.setOperators(operators);

  bool variableGamma = computeReactions;
  
  real alpha=-gamma*pow(double(a0*deltaT),2.);
  if( variableGamma ) 
    alpha = -pow(double(a0*deltaT),2.);

  bool useOpt=true;
  if( formSteadyEquation )
  {
    // This operator is used when computing an initial pressure for low mach number
    Index I1,I2,I3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg =cg[grid];

#ifdef USE_PPP
      realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff[grid],coeffLocal);
      realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
#else
      realSerialArray & coeffLocal = coeff[grid];
      realSerialArray & u1Local = u1[grid];
#endif

      getIndex(mg.dimension(),I1,I2,I3);
    
      int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
      if( !ok ) continue;
      coeffLocal=0.;

      getIndex(mg.gridIndexRange(),I1,I2,I3);  // assign coefficients here *wdh* 2015/04/19 
      operators[grid].assignCoefficients(MappedGridOperators::laplacianOperator,coeffLocal,I1,I2,I3,0,0);
    }
    
    // coeff=operators.laplacianCoefficients();
  }
  else
  {
    // define the operator I + alpha*[ (p/r)\Delta - (p/r^2)( r.xD_x + r_yD_y) ]
    if( useOpt )
    {
      Index I1,I2,I3;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        MappedGrid & mg =cg[grid];

	realArray & rL0  = linearizeImplicitMethod ? rL()[grid] : u1[grid];
        #ifdef USE_PPP
  	  realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff[grid],coeffLocal);
  	  realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
  	  realSerialArray pMLocal; getLocalArrayWithGhostBoundaries(pM[grid],pMLocal);
  	  realSerialArray rLLocal; getLocalArrayWithGhostBoundaries(rL0,rLLocal);
        #else
          realSerialArray & coeffLocal = coeff[grid];
          realSerialArray & u1Local = u1[grid];
          realSerialArray & pMLocal = pM[grid];
          realSerialArray & rLLocal = rL0;
	#endif
        
	// compute 1./rho
        getIndex(mg.dimension(),I1,I2,I3);
        int includeGhost=1;
	bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
        if( !ok ) continue;
	RealArray rhoInverse(I1,I2,I3);
        if( linearizeImplicitMethod )
	  rhoInverse(I1,I2,I3)=1./rLLocal(I1,I2,I3);
	else
          rhoInverse(I1,I2,I3)=1./u1Local(I1,I2,I3,rc);

        getIndex(mg.gridIndexRange(),I1,I2,I3);  // assign coefficients here
	ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
        if( !ok ) continue;

	// first form coeff = div( 1/rho grad )

        coeffLocal=0.;  
	
	operators[grid].assignCoefficients(MappedGridOperators::divergenceScalarGradient,coeffLocal,rhoInverse,I1,I2,I3,0,0);
 
	const int numberOfComponentsForCoefficients=1; 
	const int stencilSize=coeff[grid].sparse->stencilSize;
	const int width = parameters.dbase.get<int >("orderOfAccuracy")+1; 
	const int halfWidth1 = (width-1)/2;
	const int halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
	const int halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;

        Index M(0,stencilSize);        
        int i1,i2,i3,m;
        if( !variableGamma )
	{
	  FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	  { // form alpha*(p+pressureLevel)*div(1/rho grad)
	    coeffLocal(m,i1,i2,i3)*=alpha*(pMLocal(i1,i2,i3)+pressureLevel);        // optimize this loop
	  }
	}
	else
	{
          #ifdef USE_PPP
    	    realSerialArray gamLocal; getLocalArrayWithGhostBoundaries(gam()[grid],gamLocal);
          #else
            realSerialArray & gamLocal = gam()[grid];
	  #endif
	  FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	  { // form alpha*gamma*(p+pressureLevel)*div(1/rho grad)
	    coeffLocal(m,i1,i2,i3)*=alpha*gamLocal(i1,i2,i3)*(pMLocal(i1,i2,i3)+pressureLevel); // optimize this loop
	  }
	}
	
        // add on the identity:
        int md=M123(0,0,0); // index of the diagonal term
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  coeffLocal(md,i1,i2,i3)+=1.;        // optimize this loop
	}
	
	// ::display(coeff[grid],"coeff after setting interior pts","%5.2f ");

      } // end for grid
      
    }
    else
    {
      rho().link(u1,Range(rc,rc));  // this probably doesn't work in parallel (?)
      realCompositeGridFunction & rM = linearizeImplicitMethod ? rL() : rho();  
      rM.setOperators(operators);
      if( cg.numberOfDimensions()==1 )
      {
	if( !computeReactions )
	{
          // ********************* does  (pM+pressureLevel)/rM generate a temp that never is deleted???? *************
	  coeff=operators.identityCoefficients() 
	    + alpha*( 
	      multiply( evaluate((pM+pressureLevel)/rM) ,operators.laplacianCoefficients())   
	      +multiply( evaluate(-(pM+pressureLevel)/(rM*rM)),
			 multiply(rM.x(),operators.xCoefficients()) )
	      );
	}
	else
	{
	  coeff=operators.identityCoefficients() 
	    + (-pow(double(a0*deltaT),2.))*( 
	      multiply( evaluate(gam()*(pM+pressureLevel)/rM) ,operators.laplacianCoefficients())
	      +multiply( evaluate(-gam()*(pM+pressureLevel)/(rM*rM)),
			 multiply(rM.x(),operators.xCoefficients()) )
	      );
	}
      }
      else if( cg.numberOfDimensions()==2 )
      {
	coeff=operators.identityCoefficients() 
	  + alpha*( 
	    multiply( evaluate((pM+pressureLevel)/rM) ,operators.laplacianCoefficients())
	    +multiply( evaluate(-(pM+pressureLevel)/(rM*rM)),
		       multiply(rM.x(),operators.xCoefficients())
		       +multiply(rM.y(),operators.yCoefficients()))
	    );
      }
      else
      {
	coeff=operators.identityCoefficients() 
	  + alpha*( 
	    multiply( evaluate((pM+pressureLevel)/rM) ,operators.laplacianCoefficients())
	    +multiply( evaluate(-(pM+pressureLevel)/(rM*rM)),
		       multiply(rM.x(),operators.xCoefficients())
		       +multiply(rM.y(),operators.yCoefficients())
		       +multiply(rM.z(),operators.zCoefficients()))
	    );
      }
    }
  }
  
//    realCompositeGridFunction t1,t2,t3,t4;
//    t1=-(pM+pressureLevel)/(rM*rM);
//    t2=multiply(rM.x(),operators.xCoefficients())+multiply(rM.y(),operators.yCoefficients());
//    if( debug & 64 )
//    {
//      rho.display("Here is rho");
//      rM.x().display("r.x");
//      operators.xCoefficients().display("Here is D_x");
//      multiply(rM.x(),operators.xCoefficients()).display("r.x*D_x");
//      rM.y().display("r.y");
//      
//      t2.display("Here is r.x*D_x + r.y*D_y");
//    }
//    t3=multiply(t1,t2);
//    if( debug() & 64 )
//      t3.display(" -(p/r^2) *(r.x*D_x + r.y*D_y) ");
//    
//    t1=(pM+pressureLevel)/rM;
//    coeff=operators.identityCoefficients() 
//      + alpha*( multiply(t1,operators.laplacianCoefficients())+t3);
    
  // +++++++++++++++++++++++++++++++++++++++++++
  // Boundary Conditions

  cg.update(MappedGrid::THEvertexBoundaryNormal); // *wdh* 070113  -- need for a BC below -- fix the BC ---

  //kkc 070201  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,  Parameters::subSonicInflow2);
  //kkc 070201 coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,Parameters::subSonicInflow2);

  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,AsfParameters::subSonicInflow);
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,Parameters::slipWall);
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,Parameters::noSlipWall);

  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,  Parameters::dirichletBoundaryCondition);
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,Parameters::dirichletBoundaryCondition);


  int side,axis, grid;
  BoundaryConditionParameters bcParams;
  RealArray & a = bcParams.a;
  a.redim(2);
  
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    // For outflow boundaries check whether the pressure BC a*p+b*p.n is neumann (b!=0) or dirichlet
    int typeOfBoundaryCondition=-1;  // -1=no outflow boundaries, 0=dirichlet, 1=neumann
    ForBoundary( side,axis )
    {
      if( c.boundaryCondition(side,axis)==AsfParameters::subSonicOutflow ||
          c.boundaryCondition(side,axis)==AsfParameters::convectiveOutflow ||
          c.boundaryCondition(side,axis)==AsfParameters::tractionFree  )
      {
	const real & alpha = mixedCoeff(pc,side,axis,grid);
	const real & beta  = mixedNormalCoeff(pc,side,axis,grid);
      
        if( debug() & 1 )
          printF(">>>> formASF: Set BC's for the pressure eqn: **** alpha=%e, beta=%e ******\n",alpha,beta);
	
	if( beta!=0. ) // coeff of p.n 
	{
	  if( typeOfBoundaryCondition!=0 )
	    typeOfBoundaryCondition=1;  // neumann
          else
	    typeOfBoundaryCondition=2;  // error
	  a(0)=alpha;
	  a(1)=beta;
	}
	else
	{
	  if( typeOfBoundaryCondition!=1 )
	    typeOfBoundaryCondition=0;  // dirichlet
          else
	    typeOfBoundaryCondition=2;  // error
	  a(0)=alpha;
	  a(1)=beta;
	}
	if( typeOfBoundaryCondition==2 )
	{
	  printf("formAllSpeedPressureEquation:ERROR: in assign boundary conditions for coeff. matrix \n"
		 "there are two outflow boundaries on a component grid with one a mixed and one a dirichlet BC\n");
	  Overture::abort("error -- fix me Bill!");
	}
      }
      
    }
    if( typeOfBoundaryCondition==1 )
    {
      // mixed or neumann BC
      if( debug() & 2 )
        printF(" formAllSpeedPressure:Mixed BC coeff's for grid=%i :  %g p + %g p.n =  \n",grid,a(0),a(1));
      
      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,AsfParameters::subSonicOutflow,bcParams);
      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,AsfParameters::convectiveOutflow,bcParams);
      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,AsfParameters::tractionFree,bcParams);
    }
    else if( typeOfBoundaryCondition==0 )
    {
      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,AsfParameters::subSonicOutflow);
      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,AsfParameters::subSonicOutflow);

      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,AsfParameters::convectiveOutflow);
      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,AsfParameters::convectiveOutflow);

      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,AsfParameters::tractionFree);
      coeff[grid].applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,AsfParameters::tractionFree);
    }
  }
  coeff.finishBoundaryConditions();


    //  Check to see if the pressure equation is singular (low Mach number and neumann BC's)
  bool singularPressureEquation=TRUE;  // change this depending on the boundary conditions

  bool neumannBoundaryConditions=TRUE;  // true if all BC's are neumann
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    
    ForBoundary( side,axis )
    {
      switch( cg[grid].boundaryCondition(side,axis) )
      {
      case Parameters::noSlipWall:
      case Parameters::slipWall:
      case AsfParameters::subSonicInflow:
	break;
	//kkc 070201      case Parameters::inflowWithPressureAndTangentialVelocityGiven:
      case Parameters::dirichletBoundaryCondition:
	singularPressureEquation=false;
	neumannBoundaryConditions=false;
	break;
      case AsfParameters::subSonicOutflow:
      case AsfParameters::convectiveOutflow:
      case AsfParameters::tractionFree:
	// pressure equation is still singular with a mixed BC if alpha=0. (alpha*p+beta*p.n=)
	singularPressureEquation=singularPressureEquation && mixedCoeff(pc,side,axis,grid)==0. ;
	neumannBoundaryConditions=neumannBoundaryConditions &&
	     mixedCoeff(pc,side,axis,grid)==0. && 
             mixedNormalCoeff(pc,side,axis,grid)==1.; 
	break;
      default:
	if( cg[grid].boundaryCondition(side,axis) > 0 )
	{
	  cout << "ASF::formAllSpeedPressureEquation:ERROR unknown BC value! \n";
	  printf("cg[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i (%s)\n",grid,side,axis,
		 cg[grid].boundaryCondition()(side,axis), 
		 (const char*)parameters.bcNames[cg[grid].boundaryCondition()(side,axis)]);
	  throw "ASF::formAllSpeedPressureEquation ERROR unknown BC value";
	}
      }
    }
  }

  if( false &&   // turn this off for now 070114
      singularPressureEquation && pressureLevel>100. )
  {
    printf(" solveForAllSpeedPressure:: ********** pressure equation is singular ********* \n");
    // implicitSolver.setCoefficientArray( coeff );
    implicitSolver[0].set(OgesParameters::THEnullVectorScaling,real(1.+pressureLevel));   // should be dt^2 p0
      
    implicitSolver[0].set(OgesParameters::THEcompatibilityConstraint,TRUE);
  }

  implicitSolver[0].updateToMatchGrid(cg);    // ******************** needed every time ????

  if( debug() & 64 )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      display(coeff[grid],"solveForPressure: Here is coeff",parameters.dbase.get<FILE* >("debugFile"));
  }

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdatePressureEquation"))+=getCPU()-time;
  
}
