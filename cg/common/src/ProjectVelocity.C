#include "ProjectVelocity.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "Parameters.h"
#include "ParallelUtility.h"

ProjectVelocity::
ProjectVelocity()
{
  uc=0;
  axisymmetric=FALSE;
  numberOfSmoothsPerProjectionIteration=10;
  minimumNumberOfProjectionIterations=1;
  maximumNumberOfProjectionIterations=10;
  debug=2;
  compare3Dto2D=FALSE;
  convergenceTolerance=.95;
  poissonSolver=NULL;
}

ProjectVelocity::
~ProjectVelocity()
{
}

int ProjectVelocity::
setCompare3Dto2D( int value )
{
  compare3Dto2D=value;
  return 0;
}


int ProjectVelocity::
setIsAxisymmetric( bool trueOrFalse /* =TRUE */ )
{
  axisymmetric=trueOrFalse;
  return 0;
}


int ProjectVelocity::
setNumberOfSmoothsPerProjectionIteration(int number )
{
  numberOfSmoothsPerProjectionIteration=number;
  return 0;
}

int ProjectVelocity::
setMinimumNumberOfProjectionIterations(int number )
{
  minimumNumberOfProjectionIterations=number;
  return 0;
}

int ProjectVelocity::
setMaximumNumberOfProjectionIterations(int number )
{
  maximumNumberOfProjectionIterations=number;
  return 0;
}

int ProjectVelocity::
setDebug(int number )
{
  debug = number;
  return 0;
}

int ProjectVelocity::
setConvergenceTolerance(real value)
{
  convergenceTolerance=value;
  return 0;
}

int ProjectVelocity::
setVelocityComponent(int uc_)
{
  uc=uc_;
  return 0;
}


int ProjectVelocity::
setPoissonSolver(Oges *solver)
{
  poissonSolver=solver;
  return 0;
}




#define ForBoundary(side,axis)   for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
real ProjectVelocity::
computeDivergence(const realCompositeGridFunction & u, 
                  realCompositeGridFunction & divergence )
{
  real divl2Norm;
  return computeDivergence(u,divergence,divl2Norm);
}

#define getDivAndNorms EXTERN_C_NAME(getdivandnorms)
extern "C"
{

 void getDivAndNorms(const int &nd,
      const int &n1a,const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b,
      const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,
      const int &nd4a,const int &nd4b,
      const int &mask,const real &xy,const real &rsxy, const real &u, 
      const real &div,  const int &ipar, real &rpar, const int &ierr );
}

real ProjectVelocity::
computeDivergence(const realCompositeGridFunction & u, 
                  realCompositeGridFunction & divergence,
                  real & divl2Norm )
//==============================================================================================
// /Description:
//    Compute the divergence of a composite grid function and also determine the maximum
//    divergence.
// /uc (input) : The component position of "u" 
// /divergence (output) the divergence
// /divl2Norm (output) : the l2 norm of the divergence  = sqrt[ sum( div(u)^2 )/( Number of points )  ]
// /Return value: the maximum divergence
//==============================================================================================
{
  real divMax=0.;
  CompositeGrid & cg0 = *u.getCompositeGrid(); // (CompositeGrid&) (*u.gridCollection);
  Index I1,I2,I3;
  divl2Norm=0.;
  real vorMax=0.;
  
  int numberOfPoints=0;
  if( false && axisymmetric )
  {
    // old way -- still do this for axisymmetric ---
    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      realArray & div = divergence[grid];

      // *wdh* 030313 getIndex(cg0[grid].extendedIndexRange(),I1,I2,I3); 
      getIndex(cg0[grid].gridIndexRange(),I1,I2,I3); 
      if( cg0.numberOfDimensions()==1 )
	div(I1,I2,I3)=u[grid].x(I1,I2,I3,uc)(I1,I2,I3,uc);
      else if( cg0.numberOfDimensions()==2 ) // || compare3Dto2D )
      {

	div(I1,I2,I3)=u[grid].x(I1,I2,I3,uc)(I1,I2,I3,uc)+u[grid].y(I1,I2,I3,uc+1)(I1,I2,I3,uc+1);

	if( cg0.numberOfDimensions()==2 && axisymmetric )
	{
	  // div(u) = u.x + v.y + v/y for y>0   or u.x + 2 v.y at y=0
	  realArray radiusInverse = 1./max(REAL_MIN,cg0[grid].vertex()(I1,I2,I3,axis2));
	  Index Ib1,Ib2,Ib3;
	  for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
	  {
	    for( int side=0; side<=1; side++ )
	    {
	      if( cg0[grid].boundaryCondition(side,axis)==Parameters::axisymmetric )
	      {
		getBoundaryIndex(cg0[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		radiusInverse(Ib1,Ib2,Ib3)=0.;
		div(Ib1,Ib2,Ib3)+=u[grid].y(Ib1,Ib2,Ib3,uc+1)(Ib1,Ib2,Ib3,uc+1);
	      }
	    }
	  }
	  div(I1,I2,I3)+=u[grid](I1,I2,I3,uc+1)*radiusInverse;
	}
      }
      else
	div(I1,I2,I3)=(u[grid].x(I1,I2,I3,uc  )(I1,I2,I3,uc  )+
		       u[grid].y(I1,I2,I3,uc+1)(I1,I2,I3,uc+1)+
		       u[grid].z(I1,I2,I3,uc+2)(I1,I2,I3,uc+2));

      // *wdh* 990714 getIndex(cg0[grid].extendedIndexRange(),I1,I2,I3,-1);   // trouble if periodic and only 3 pts.
      // *wdh* getIndex(extendedGridIndexRange(cg0[grid]),I1,I2,I3,-1);  // *wdh* 030313 wrong for 4th order
      const intArray & mask = cg0[grid].mask();
    
      getIndex(cg0[grid].gridIndexRange(),I1,I2,I3); 
      numberOfPoints+=sum(mask(I1,I2,I3)>0 );
      where( mask(I1,I2,I3)>0 )
      {
	divMax= max(divMax,max(fabs(div(I1,I2,I3))));
	divl2Norm+=sum(div(I1,I2,I3)*div(I1,I2,I3));
      }
    }
  }
  else
  {
    // *new* optimised way
    // printf("*** ProjectVelocity::computeDivergence axisymmetric=%i ***\n",axisymmetric);
    

    divMax=0.;
    vorMax=0.;
    divl2Norm=0.;
    int numberOfPoints=0;
      
    CompositeGridOperators & cgop = *u.getOperators();
    const int orderOfAccuracy = cgop.getOrderOfAccuracy();

    CompositeGrid & cg0 = *u.getCompositeGrid();
    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      const realArray & v = u[grid];
      realArray & div = divergence[grid];

      MappedGrid & mg = cg0[grid];
      const IntegerArray & gid = mg.gridIndexRange();
	
      bool isRectangular = mg.isRectangular();
      const int gridType = isRectangular ? 0 : 1;
      real dx[3]={1.,1.,1.};
      real xab[2][3]={0.,1.,0.,1.,0.,1.};
      if( isRectangular )
	mg.getRectangularGridParameters( dx, xab );

      int i1a=mg.gridIndexRange(0,0);
      int i2a=mg.gridIndexRange(0,1);
      int i3a=mg.gridIndexRange(0,2);

      int ierr=0;
      const int option=1; // compute norms AND save save div(i1,i2,i3)
      int ipar[20] ={ uc-1,uc,uc+1,uc+2,grid,orderOfAccuracy,
                      (int)axisymmetric,gridType,option,
		      i1a,i2a,i3a,0,0,0,0,0,0,0,0	};  //
	
      const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
      real rpar[20]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2), dx[0],dx[1],dx[2],
		     xab[0][0],xab[0][1],xab[0][2],yEps,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};  //

      if( !isRectangular ) 
        mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex);  // *wdh* 2013/08/28

      #ifdef USE_PPP
	realSerialArray uLocal; getLocalArrayWithGhostBoundaries(v,uLocal);
        const real *pu = uLocal.getDataPointer();
        const real *prsxy = isRectangular ? pu : mg.inverseVertexDerivative().getLocalArray().getDataPointer();
        const real *pxy = isRectangular ? pu : mg.vertex().getLocalArray().getDataPointer();
        const int *pmask = mg.mask().getLocalArray().getDataPointer();
        const real *pdiv = div.getLocalArray().getDataPointer(); 
      #else
	const realSerialArray & uLocal=v; 
        const real *pu = uLocal.getDataPointer();
        const real *prsxy = isRectangular ? pu : mg.inverseVertexDerivative().getDataPointer();
        const real *pxy = isRectangular ? pu : mg.vertex().getDataPointer();
        const int *pmask = mg.mask().getDataPointer();
        const real *pdiv = div.getDataPointer(); 
      #endif

//       const real *pu = v.getDataPointer();
//       const real *prsxy = isRectangular ? pu : mg.inverseVertexDerivative().getDataPointer();
//       const real *pxy = isRectangular ? pu : mg.vertex().getDataPointer();

        getIndex(gid,I1,I2,I3);

	int n1a,n1b,n2a,n2b,n3a,n3b;
	bool ok = ParallelUtility::getLocalArrayBounds(v,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b); 
        if( !ok ) continue;

	getDivAndNorms(mg.numberOfDimensions(),
		       n1a,n1b,n2a,n2b,n3a,n3b,
		       uLocal.getBase(0),uLocal.getBound(0),
		       uLocal.getBase(1),uLocal.getBound(1),
		       uLocal.getBase(2),uLocal.getBound(2),
		       uLocal.getBase(3),uLocal.getBound(3),
		       *pmask, *pxy, *prsxy, *pu, *pdiv,  ipar[0], rpar[0], ierr );

	divMax=max(divMax,rpar[10]);
	vorMax=max(vorMax,rpar[11]);
	divl2Norm+=rpar[12];
	numberOfPoints+=ipar[10];
      } // end for grid

      divMax=ParallelUtility::getMaxValue(divMax);
      vorMax=ParallelUtility::getMaxValue(vorMax);
      numberOfPoints=ParallelUtility::getSum(numberOfPoints);
      divl2Norm=ParallelUtility::getSum(divl2Norm);

  }
  
  divl2Norm=sqrt(divl2Norm/max(1,numberOfPoints));
  
  return divMax;
}


void  ProjectVelocity::
smoothVelocity(realCompositeGridFunction & u, 
               const int numberOfSmooths )
// ============================================================================================
// /Description:
//  Smooth the velocity
// ============================================================================================
{
  CompositeGrid & cg = *u.getCompositeGrid();
  
  //  ---Use a Jacobi smoother, under-relaxed
  real omega0=.9;
  real omo=1.-omega0, ob4=omega0/4., ob6=omega0/6.;
  
  Index I1,I2,I3;
  int extra[3] = { 0,0,0 };  
  Index N(uc,cg.numberOfDimensions());
  for( int it=0; it<numberOfSmooths; it++ )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      realArray & v = u[grid];
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
        extra[axis] = cg[grid].isPeriodic()(axis) ? 0 : -1;
//      getIndex(cg[grid].gridIndexRange(),I1,I2,I3,extra[0],extra[1],extra[2]);   // *wdh* 980408
      getIndex(extendedGridIndexRange(cg[grid]),I1,I2,I3,extra[0],extra[1],extra[2]); 
      
      if( cg.numberOfDimensions()==2 || compare3Dto2D )
      {
        // where( cg[grid).mask()(I1,I2,I3) >0 ) ** add this ?
	v(I1,I2,I3,N)=omo*v(I1,I2,I3,N)
	  +ob4*( v(I1+1,I2,I3,N)+v(I1-1,I2,I3,N)
		+v(I1,I2+1,I3,N)+v(I1,I2-1,I3,N));
      }
      else
      {
	v(I1,I2,I3,N)=omo*v(I1,I2,I3,N)
	  +ob6*( v(I1+1,I2,I3,N)+v(I1-1,I2,I3,N)
		+v(I1,I2+1,I3,N)+v(I1,I2-1,I3,N)
		+v(I1,I2,I3+1,N)+v(I1,I2,I3-1,N) );
      }
    }
  }  
}

int ProjectVelocity::
projectVelocity(realCompositeGridFunction & u, 
                GenericCompositeGridOperators & op )
//====================================================================================
//
// /Description:
//   Smooth and Project the Solution to Reduce the Divergence
//   **WARNING** this routine is not optimized to be called multiple times!  
//
// /u (input/output):  The velocity components are assumed to be numbered 0,1,[2]. u should
//   have an initial value in the interior and the correct boundary conditions. On output
//   the interior values will be changed but not the boundary values.
// /op (input) : Use these operators
// /poissonSolver : Use this Oges solver (allows one to set Oges parameters to be used by this routine)
// /uc (input) : The component position of "u" (v is assumed to follow u)
// /numberOfSmoothsPerProjectionIteration: (input):
// /minimumNumberOfProjectionsIterations (input):
// /maximumNumberOfProjectionsIterations (input):
// /convergenceTolerance (input): iterate until the ratio of the new divergence to the old divergence
//    is larger than this value:
//                div(n)/div(n-1) > convergenceTolerance
//
// /Detailed Description:
//  This routine is used to massage initial conditions that are
//  not divegence free. In order to reduce the divergence we
//  smooth and project the solution:
//         do it=1,nit
//            smooth a few times
//            project
//         end do
//         smooth a final time
//
//  smooth: Take a few explicit time steps in solving
//              u.t = anu Delta u
//  project: Compute a projected solution u_p by subtracting the gradient
//    of a function phi (except on the boundary)
//           u_p = u - grad phi   x in Omega
//           u_p = u              x in the boundary
//    where phi satisfies (take divergence of above equation and
//    set div u_p = 0)
//           Delta phi = div u   x in Omega
//               phi.n = 0  or phi=0      x in the Boundary
//    (phi is solved in the same way as the pressure and has the
//    same boundary conditions as the pressure)
//    By projecting in this way we cannot satisfy the full boundary
//    conditions on u - only the normal velocity is consistent.
//
//
//    Since we use the original
//    u on the boundary and not (u+grad phi) it follows we cannot
//    reduce the divergence to zero in general.
//      In practice it seems that this projection is good enough
//    so that after integrating in time for a while the
//    divergence goes to zero.
//
//
//
//======================================================================================
{

  // ***********  NOTE: cgins uses it's own project function ***********

  int returnValue=0;

  if( debug & 2 )
    printf(" projectVelocity>>> Reducing divergence of the velocity\n");

  if( FALSE )
    printf("\n**** projectVelocity1: Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);

  CompositeGrid & cg = *u.getCompositeGrid();

  real divMax;
  // int nitmax=9; // maximum number of smooth/project iterations
  int nitsm=10;  // number of smoothing steps
  int numberOfSmoothingSubIterations=1;  // only do 1 because of BC's and interp pts
  real divMaxOld=1.e6;

  Range all;
  realCompositeGridFunction phi(cg,all,all,all); // projection function
  phi=0.;
  phi.setOperators(op);
  realCompositeGridFunction divergence(cg,all,all,all);  // holds divergence
  divergence=0.;


  int grid,side,axis;
  int stencilSize=int( pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
  op.setStencilSize(stencilSize);
  coeff.setOperators(op);

  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  if( axisymmetric )
  {
    // add on corrections for a axisymmetric problem
    // Delta p = p.xx + p.yy + (1/y) p.y
    // note that p.y=0 on y=0 and p.y/y = p.yy at r=0
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      Index I1,I2,I3;
      getIndex(c.dimension(),I1,I2,I3);
      realArray radiusInverse;
      radiusInverse=1./max(REAL_MIN,c.vertex()(I1,I2,I3,axis2));

      ForBoundary(side,axis)
      {
	if( c.boundaryCondition(side,axis)==Parameters::axisymmetric )
	{
	  Index Ib1,Ib2,Ib3;
	  Range M=stencilSize;
	  getBoundaryIndex( c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); 
	  coeff[grid](M,Ib1,Ib2,Ib3)+=op[grid].yyCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3); // add p.yy on axis
	  radiusInverse(Ib1,Ib2,Ib3)=0.;  // this will remove p.y/y term below from boundary
	}
      }
      // add p.y/y term
      coeff[grid]+=multiply(radiusInverse,op[grid].yCoefficients());
    }
  }
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::allBoundaries);
  coeff.finishBoundaryConditions();

  Oges *localPoissonSolver=NULL;
  if( poissonSolver==NULL )
    localPoissonSolver= new Oges(cg);
  else
    poissonSolver->updateToMatchGrid(cg);   // this will force a refactor

  Oges & poisson = poissonSolver!=NULL ? *poissonSolver : *localPoissonSolver;

  poisson.setCoefficientArray( coeff );   // supply coefficients
  poisson.set(OgesParameters::THEcompatibilityConstraint,TRUE);
  poisson.initialize();

  if( FALSE )
    printf("\n**** projectVelocity2: Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);

  Index I1,I2,I3;
  int extra[3] = { 0,0,0 };  
  Index V(uc,cg.numberOfDimensions());

  for( int it=0; it<maximumNumberOfProjectionIterations; it++ )
  {
    // ---smoothing iterations, smooth more the first few times
    int nitsm0 = (it<5) ? max(nitsm,numberOfSmoothsPerProjectionIteration) : nitsm; 
    for( int itsm=0; itsm<nitsm0; itsm++ )
    {
      if( debug & 4 )
        printf(" projectVelocity>>> iteration %i, smoothing iteration %i\n",it,itsm);
      smoothVelocity( u,numberOfSmoothingSubIterations ); 
      // interpolate first, needed for extrapolate-neighbours
      u.interpolate();
       // need divergence on the boundary, so get values on ghost line
      u.applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries); 
      u.finishBoundaryConditions();
    }

    real divMaxAfterSmooth = computeDivergence( u,divergence );   // compute divergence for rhs 
    if( it==0 ) divMaxOld=divMaxAfterSmooth;
    if( debug & 4 )
      printf(" projectVelocity>>> iteration=%i, divergence after smooth    =%f\n",it,divMaxAfterSmooth);

    //  --- we put the rhs for the Pressure equation into divergence ---
    //  rhs = divergence in the interior 
    // Boundary conditions are homogeneous:
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {

      // if( debug & 4 )
      // {
      //   u[grid](all,all,all,uc).display("u");
      //   u[grid](all,all,all,uc+1).display("v");
      // }      
      ForBoundary(side,axis)
      {
	if( cg[grid].boundaryCondition()(side,axis)>0 )
	{
	  switch (cg[grid].boundaryCondition()(side,axis))
	  {
	  default:
	    getGhostIndex( cg[grid].extendedIndexRange(),side,axis,I1,I2,I3,1);  // first ghost line
	    divergence[grid](I1,I2,I3)=0.; 
	  }
	}
      }
    }
    // set mean value for pressure: **** do only if singular ****
    if( poisson.getCompatibilityConstraint() )
    {
      int ne,i1e,i2e,i3e,gride;
      poisson.equationToIndex( poisson.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
      divergence[gride](i1e,i2e,i3e)=0.;
    }

    // if( debug & 4 )
    // {
    //   for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    //     divergence[grid](all,all,all).display("divergence");
    // }
    
  if( FALSE )
    printf("\n**** projectVelocity4: Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);

    if( poisson.isSolverIterative() )
      phi=0.;  // for iterative solvers
    poisson.solve( phi,divergence );    // ---- solve for "phi" ---- 

  if( FALSE )
    printf("\n**** projectVelocity5: Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);
  
    // if( debug & 4 )
    // {
    //   for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    //     phi[grid](all,all,all).display("phi");
    // }

    // u-=phi.grad();   //   ---project the solution 
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      // update interior points (and periodic boundaries)
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
        extra[axis] = cg[grid].isPeriodic()(axis) ? 0 : -1;
//      getIndex(cg[grid].gridIndexRange(),I1,I2,I3,extra[0],extra[1],extra[2]); 
      getIndex(extendedGridIndexRange(cg[grid]),I1,I2,I3,extra[0],extra[1],extra[2]); 

      // if( debug & 4 )
      // {
      //   phi[grid].x(I1,I2,I3)(I1,I2,I3).display("phi.x");
      //   phi[grid].y(I1,I2,I3)(I1,I2,I3).display("phi.y");
      // }

      u[grid](I1,I2,I3,uc  )-=phi[grid].x(I1,I2,I3)(I1,I2,I3);
      u[grid](I1,I2,I3,uc+1)-=phi[grid].y(I1,I2,I3)(I1,I2,I3);
      if( cg.numberOfDimensions()==3 )
        u[grid](I1,I2,I3,uc+2)-=phi[grid].z(I1,I2,I3)(I1,I2,I3);

      for( int n=0; n<cg.numberOfDimensions(); n++ ) // **wdh** 980201
      {
	where( cg[grid].mask()(I1,I2,I3) == 0 )  
	{
	  u[grid](I1,I2,I3,uc+n)=0.;  // zero unused points
	}
      }
    }
    // u.display("project velocity u, before interpolate");
    
    u.interpolate(V);  // interpolate first!
    u.applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries); 
    u.finishBoundaryConditions();
    
    divMax = computeDivergence( u,divergence );  // compute new divergence to test for convergence

    if( debug & 2 )
    {
      printf(" projectVelocity>>> iteration=%i, (new div)/(old div)=%f, divergence after projection=%f \n", 
        it,divMax/max(REAL_MIN,divMaxOld),divMax);
    }
    
    //  --- stop when the divergence is nolonger decreasing
    if(divMax==0. || (it>minimumNumberOfProjectionIterations && divMax/divMaxOld > convergenceTolerance))
      break;
    divMaxOld=divMax;


  }
  delete localPoissonSolver;
  return returnValue;
}

