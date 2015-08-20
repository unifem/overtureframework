#include "Maxwell.h"
#include "display.h"
#include "Oges.h"
#include "SparseRep.h"
#include "CompositeGridOperators.h"
#include "checkGridFunction.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "ArrayUtil.h"

#ifdef USE_PPP
#include "ParallelOverlappingGridInterpolator.h"
#endif

#define mxProjectInterp EXTERN_C_NAME(mxprojectinterp)

extern "C"
{

 void mxProjectInterp(const int&nd,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int & gridIndexRange, real & u,  const int&mask, const real & rsxy,  const real & xy,
      const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );
}


#define ForBoundary(side,axis)   for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

//\begin{>>MappedGridSolverInclude.tex}{\subsection{assignPressureRHS}} 
int Maxwell::
updateProjectionEquation()
//======================================================================
// /Description:
//\end{MappedGridSolverInclude.tex}  
//======================================================================
{
  if( debug & 2 )
    printF("Maxwell::updateProjectionEquation...\n");

  checkArrays(" Maxwell::updateProjectionEquation: start");

  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;

  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  
  IntegerArray boundaryConditions(2,3,cg.numberOfComponentGrids());  // for Oges
  boundaryConditions=0;
  RealArray boundaryConditionData(2,2,3,cg.numberOfComponentGrids());               // for Oges
  boundaryConditionData=0.;
  
  int grid,side,axis;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    
    ForBoundary( side,axis )
    {
      boundaryConditions(side,axis,grid)=OgesParameters::neumann;  // default
      
      switch( c.boundaryCondition(side,axis) )
      {
      case perfectElectricalConductor:
        // boundaryConditions(side,axis,grid)=OgesParameters::dirichletAndOddSymmetry;  
        boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  
        break;
      default:
        // boundaryConditions(side,axis,grid)=OgesParameters::dirichletAndOddSymmetry;  
        boundaryConditions(side,axis,grid)=OgesParameters::dirichlet;  
        break;
      }
    }
  }


  // ----Set parameters for Poisson Solver
  assert( poisson!=0 );

  cgop->useConservativeApproximations(false);  // do not use conservative ops

  poisson->setGrid( cg ); 

  OgesParameters::EquationEnum equation = OgesParameters::laplaceEquation;

  // printF(" *** insp: cgop:orderOfAccuracy=%i\n",cgop[0].orderOfAccuracy);

  if( orderOfAccuracyInSpace!=2 )
  {
    printF("--MX-- project: WARNING - FIX ME -- order of extrapolation=2 for projection \n");
  }
  
  // **FIX ME* 2015/08/16 ---
  poisson->parameters.set(OgesParameters::THEorderOfExtrapolation,2);
  // we should use this I guess: 
  // poisson->parameters.set(OgesParameters::THEorderOfExtrapolation,orderOfAccuracyInSpace+1);
  

  assert( cgop!=NULL );
  poisson->setEquationAndBoundaryConditions(equation,*cgop,boundaryConditions, boundaryConditionData );

  if( false )
  {
    realCompositeGridFunction & coeff = poisson->coeff;
    coeff.display("coeff");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const intMappedGridFunction & classify = coeff[grid].sparse->classify;
      ::display(classify,"classify");
    }
  }
      

  poisson->set(OgesParameters::THEkeepCoefficientGridFunction,false); 

  cgop->useConservativeApproximations(useConservative);  // reset

  return 0;
}


int Maxwell::
project( int numberOfStepsTaken, int current, real t, real dt )
// ====================================================================================
// /Description:
//    Project the fields to satisfy the correct divergence condition, 
//            div(eps*E)=rho
// 
//     
// ====================================================================================
{
  real time0=getCPU();

  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  if( poisson==NULL )
  {
    poisson = new Oges;
  }
  if( initializeProjection )
  {
    updateProjectionEquation();
    initializeProjection=false;
  }
  

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  // realCompositeGridFunction phi(cg), f(cg);  // fix this
  if( pPhi==NULL )
  {
    pPhi = new realCompositeGridFunction(cg);
    // *pPhi=0.;
    ArrayUtil::assign( *pPhi,0. );  // this will set parallel ghost boundaries too
  }
  if( pF==NULL )
  {
    pF = new realCompositeGridFunction(cg);    // could we share space for this ??
    // *pF=0.;
    ArrayUtil::assign( *pF,0. );  // this will set parallel ghost boundaries too
  }
  realCompositeGridFunction & phi = *pPhi;
  realCompositeGridFunction & f = *pF;
  

  // Compute f=div(eps*E)-rho, optionally compute maxNorms for diagonostics
  const bool computeMaxNorms =  true || numberOfStepsTaken<10  || (debug & 2); 

  // f=0.;  // *wdh* 051112 -- try this for parallel 
  
  getMaxDivergence(current,t,&f,0, NULL,0, computeMaxNorms);  // div(eps*E)-rho 

  if( computeMaxNorms )
  {
    if( debug & 1 )
      printF("===>> project: Before project: |div(E)-rho|=%8.2e, |div(E)-rho|/|grad(E)|=%8.2e, step=%i, t=%9.3e\n",
	     divEMax,divEMax/max(divEMax*.1+REAL_MIN*100.,gradEMax),numberOfStepsTaken,t);
  }

  
  // assign the RHS f = div(E) - rho
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const intArray & mask = mg.mask();

    const bool isRectangular=mg.isRectangular();
    MappedGridOperators & op = (*cgop)[grid];

    realArray & fg = f[grid];
    realMappedGridFunction & phig = phi[grid];

    realMappedGridFunction & fieldCurrent = cgfields[current][grid];
    realArray & u  = fieldCurrent;

//     getIndex(mg.dimension(),I1,I2,I3);
//     where( mask(I1,I2,I3)<=0 )
//       fg(I1,I2,I3)=0.;

//     fg=0.;
    

//     if( false )
//     {
//       // only assign interior points:
//       int extra=-1;
//       getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

	
//       // ** do this for now: ***
//       if( useConservativeDivergence )
//       { 
// 	op.useConservativeApproximations(useConservative);  // reset
// 	printf("getMaxDivergence: get conservative divergence t=%9.3e, useConservative=%i\n",t,useConservative);
// 	op.derivative(MappedGridOperators::divergence,fieldCurrent,fg,I1,I2,I3);
//       }
//       else
//       {
    if( false ) // ********************************
    {
      int extra=-1;
      getIndex(mg.gridIndexRange(),I1,I2,I3,extra);
      fg=0.;
      op.useConservativeApproximations(false);  // turn off since there are no conservative
      op.derivative(MappedGridOperators::xDerivative,fieldCurrent,phig,I1,I2,I3,ex);
      fg(I1,I2,I3)=phig(I1,I2,I3);

      op.derivative(MappedGridOperators::yDerivative,fieldCurrent,phig,I1,I2,I3,ey);
      fg(I1,I2,I3)+=phig(I1,I2,I3);

      if( numberOfDimensions==3 )
      {
	op.derivative(MappedGridOperators::zDerivative,fieldCurrent,phig,I1,I2,I3,ez);
	fg(I1,I2,I3)+=phig(I1,I2,I3);
      }
      op.useConservativeApproximations(useConservative);  // reset
    }
    
    // *** no need to assign if points are already zero: (?)
    int side,axis;
    ForBoundary(side,axis)
    {
      if( cg[grid].boundaryCondition(side,axis)>0 )
      {
	switch (cg[grid].boundaryCondition(side,axis))
	{
	default:
	  getBoundaryIndex( cg[grid].gridIndexRange(),side,axis,I1,I2,I3);
          int ia=side==0 ? mg.dimension(side,axis) : mg.gridIndexRange(side,axis);
          int ib=side==1 ? mg.dimension(side,axis) : mg.gridIndexRange(side,axis);
	  
          Iv[axis]= Range(ia,ib); 
          
	  // ** fg(I1,I2,I3)=0.;   // zero out the boundary and ghost lines
          ArrayUtil::assign( fg,0., I1,I2,I3);
	  
	}
      }
    }

    op.useConservativeApproximations(useConservative);  // reset    
  }

  // this is seems to work better for iterative solvers 
  ArrayUtil::assign( phi,0. );  // this will set parallel ghost boundaries too

  // optionally smooth the rhs
  smoothDivergence(f,numberOfDivergenceSmooths);

  poisson->solve(phi,f);
  if( poisson->isSolverIterative() )
  {
    if( myid==0 )
      printF("Projection:solve: %i iterations (step=%i, t=%9.3e)\n",poisson->getNumberOfIterations(),
	     numberOfStepsTaken,t);

    if( debug & 4 ) checkGridFunction(phi,"check phi after solve",true);
    
  }
  
  // Assign: 
  //         E(new) = E - grad(phi)
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const intArray & mask = mg.mask();

    MappedGridOperators & op = (*cgop)[grid];
    op.useConservativeApproximations(false);  // turn off since there are no conservative

    const bool isRectangular=mg.isRectangular();

    realMappedGridFunction & phig = phi[grid];
    realMappedGridFunction & phix = f[grid];        // temp space -- fix this 

    realMappedGridFunction & fieldCurrent = cgfields[current][grid];
    realArray & u  = fieldCurrent;

    // int extra=-1;  // only assign interior points:
    int extra=0;      // assign boundary points too
    getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray phixLocal;  getLocalArrayWithGhostBoundaries(phix,phixLocal);
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

    ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);

    op.derivative(MappedGridOperators::xDerivative,phig,phix,I1,I2,I3);
    where( maskLocal(I1,I2,I3)>0 )
      uLocal(I1,I2,I3,ex)-=phixLocal(I1,I2,I3);

    op.derivative(MappedGridOperators::yDerivative,phig,phix,I1,I2,I3);
    where( maskLocal(I1,I2,I3)>0 )
      uLocal(I1,I2,I3,ey)-=phixLocal(I1,I2,I3);

    if( numberOfDimensions==3 )
    {
      op.derivative(MappedGridOperators::zDerivative,phig,phix,I1,I2,I3);
      where( maskLocal(I1,I2,I3)>0 )
        uLocal(I1,I2,I3,ez)-=phixLocal(I1,I2,I3);
    }
    
    op.useConservativeApproximations(useConservative);  // reset
  }
  
  real timei=getCPU();
  cgfields[current].interpolate();
  timei=getCPU()-timei;
  timing(timeForInterpolate)+=timei;
  timing(timeForProject)-=timei; // do not include this time in project


  if( false && computeMaxNorms ) // do this in advanceStructured after BC's
  {
    getMaxDivergence(current,t);

    printF("===>> project: After project: |div(E)-rho|=%8.2e, |div(E)-rho|/|grad(E)|=%8.2e, step=%i, t=%9.3e\n",
	   divEMax,divEMax/gradEMax,numberOfStepsTaken,t);
    
  }
  
  timing(timeForProject)+=getCPU()-time0;

/* ----------------
  // first solve 
  //         Delta( phi ) = - div( E )
  // Then correct
  //         E(new) = E + grad(phi)
  // 

  for( int it=0; it<numberOfProjectionIterations; it++ )
  {

    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      real time0=getCPU();

      MappedGrid & mg = cg[grid];
      assert( mgp==NULL || op!=NULL );

      const bool isRectangular=mg.isRectangular();
      
      real dx[3]={1.,1.,1.};
      if( isRectangular )
	mg.getDeltaX(dx);

      getIndex(mg.gridIndexRange(),I1,I2,I3);
  


    } // end for grid
    
  } // end for it
  
  --------------- */

  return 0;
}

void Maxwell:: 
smoothDivergence(realCompositeGridFunction & u, const int numberOfSmooths )
// ============================================================================================
// /Description:
//    Smooth the divergence before projecting 
//
//  NOTES:
//     Only smooth the interior points. Assume div(E)=0 on the boundary. 
// ============================================================================================
{
  if( numberOfSmooths<=0 ) return;

  CompositeGrid & cg = *u.getCompositeGrid();
  
//  BoundaryConditionParameters bcParams;
//  bcParams.ghostLineToAssign= parameters.orderOfAccuracy/2;  // *note*

  //  ---Use a Jacobi smoother, under-relaxed
  real omega0=.9;
  real omo=1.-omega0, ob4=omega0/4., ob6=omega0/6.;
  
  Index I1,I2,I3;
  int extra[3] = { 0,0,0 };  
  Index N=u[0].dimension(4);  // all components -- should only be 1
  for( int it=0; it<numberOfSmooths; it++ )
  {
    if( true || debug & 4 )
      printF(" smoothDivergence>>> iteration=%i, max(fabs(div(E)))=%8.2e\n",it,max(fabs(u)));

    u.interpolate();

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      realArray & v = u[grid];

      // Only smooth interior points
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
        extra[axis] = cg[grid].isPeriodic(axis) ? 0 : -1;
      getIndex(cg[grid].gridIndexRange(),I1,I2,I3,extra[0],extra[1],extra[2]); 
      
      #ifdef USE_PPP
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
      #else
        realSerialArray & vLocal = v;
      #endif    

      bool ok = ParallelUtility::getLocalArrayBounds(v,vLocal,I1,I2,I3);
      if( ok )
      {
	if( cg.numberOfDimensions()==2 )
	{
	  // where( cg[grid).mask()(I1,I2,I3) >0 ) ** add this ?
	  vLocal(I1,I2,I3,N)=omo*vLocal(I1,I2,I3,N)
	    +ob4*( vLocal(I1+1,I2,I3,N)+vLocal(I1-1,I2,I3,N)
		   +vLocal(I1,I2+1,I3,N)+vLocal(I1,I2-1,I3,N));
	}
	else
	{
	  vLocal(I1,I2,I3,N)=omo*vLocal(I1,I2,I3,N)
	    +ob6*( vLocal(I1+1,I2,I3,N)+vLocal(I1-1,I2,I3,N)
		   +vLocal(I1,I2+1,I3,N)+vLocal(I1,I2-1,I3,N)
		   +vLocal(I1,I2,I3+1,N)+vLocal(I1,I2,I3-1,N) );
	}
      }
    }

//     // assign bc's to get ghost point values
//     if( parameters.orderOfAccuracy==2 )
//       applyBoundaryConditions(cgf); // **** 990106
//     if( parameters.orderOfAccuracy==4 ) // trouble at outflow with standard BC's
//     {
//       u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries);
//       u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,bcParams); // **** 990106
//       u.finishBoundaryConditions();
//     }

//     // *wdh* this extrap condition causes trouble at slipWall/outflow corners 000124
//     // *** undo the div(u)=0 BC so the projection will work better on the boundary.
//     // *wdh*  u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries); // **** 990106

//     if( debug() & 32 )
//       cgf.u.display("smooth velocity solution after sub-smooth, before interpolate",parameters.debugFile,"%9.6f ");
//     // interpolate first, needed for extrapolate-neighbours


//     if( debug() & 32 )
//       cgf.u.display("smooth velocity solution after sub-smooth, after interpolate",parameters.debugFile,"%9.6f ");
//     // assign bc's to get ghost point values
//     if( parameters.orderOfAccuracy==2 )
//       applyBoundaryConditions(cgf);

//     // *wdh* this extrap condition causes trouble at slipWall/outflow corners 000124
//     // *** undo the div(u)=0 BC so the projection will work better on the boundary.
//     // *wdh* u.applyBoundaryCondition(N,BCTypes::extrapolate,BCTypes::allBoundaries); // **** 990106

//     if( debug() & 32 )
//       cgf.u.display("smooth velocity solution after sub-smooth, after applyBC",parameters.debugFile,"%9.6f ");

  }  
}

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


int Maxwell::
projectInterpolationPoints( int numberOfStepsTaken, int current, real t, real dt )
// ====================================================================================
// /Description:
//    Project the interpolation points to satisfy the correct divergence condition, 
//            div(eps*E)=rho
// 
//     
// ====================================================================================
{
  printF("*** projectInterpolationPoints: t=%9.3e\n",t);

  CompositeGrid & cg = *cgfields[current].getCompositeGrid();


  int option=0;  // not used yet
  real rpar[10];

  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];

    const intArray & mask = mg.mask();

    realMappedGridFunction & fieldCurrent = cgfields[current][grid];
    realArray & u  = fieldCurrent;

    const bool isRectangular=mg.isRectangular();
    real dx[3]={1.,1.,1.};
    if( isRectangular )
      mg.getDeltaX(dx);

    #ifdef USE_PPP
      realSerialArray uLocal;   getLocalArrayWithGhostBoundaries(u,uLocal);
      intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
    #else
      const realSerialArray & uLocal  =  u;
      const intSerialArray & maskLocal = mask; 
    #endif

    real *prx,*pxy;
    if( isRectangular )
    {
      prx=uLocal.getDataPointer(); // not used in this case
    }
    else
    {
#ifdef USE_PPP
      realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rxLocal);
#else
      const realSerialArray & rxLocal=mg.inverseVertexDerivative();
#endif  
      prx = rxLocal.getDataPointer();
    }
    pxy=prx; // not currently used

    // int extra=orderOfAccuracy/2;  // include interp points
    // getIndex(mg.gridIndexRange(),I1,I2,I3,extra);
    
    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fieldCurrent,gidLocal,dimLocal,bcLocal );

    const int gridType = isRectangular ? 0 : 1;

    int ierr=0;
    int ipar[] = {option, ex,ey,ez,hx,hy,hz,debug,myid,orderOfAccuracyInSpace,grid,gridType };


    real omega=0.;  // not used
    rpar[ 0]=omega;
    rpar[ 1]=dt;
    rpar[ 2]=t;
    rpar[ 3]=dx[0];
    rpar[ 4]=dx[1];
    rpar[ 5]=dx[2];
    rpar[ 6]=mg.gridSpacing(0);
    rpar[ 7]=mg.gridSpacing(1);
    rpar[ 8]=mg.gridSpacing(2);

    mxProjectInterp( mg.numberOfDimensions(),
		     uLocal.getBase(0),uLocal.getBound(0),
		     uLocal.getBase(1),uLocal.getBound(1),
		     uLocal.getBase(2),uLocal.getBound(2),
		     gidLocal(0,0),*uLocal.getDataPointer(),*maskLocal.getDataPointer(),
                     *prx,*pxy,
		     bcLocal(0,0), ipar[0],rpar[0],ierr );


  }
  
  return 0;
}
