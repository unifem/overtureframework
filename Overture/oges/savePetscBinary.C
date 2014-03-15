//=========================================================================
//
//  Coefficient Matrix Example
//  ..SAVE Petsc Binary matrix    --- doesn't work yet
//
//=========================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "Square.h"
#include "Annulus.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"
#include "display.h"
#include "Ogmg.h"

//#include "EquationSolver.h"
//#include "PETScEquationSolver.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

bool measureCPU=TRUE;
real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU();
  else
    return 0;
}

int 
main(int argc, char **argv)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  bool saveDirichlet=true;
  bool saveNeumann=true;
  
  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
    
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg(0,6)=="-debug=" )
      {
        sScanF(arg(7,arg.length()-1),"%i",&Oges::debug);
	printf("Setting Oges::debug=%i\n",Oges::debug);
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[1];
      }
    }
  }
  else
    cout << "Usage: `savePetscBinary [<gridname>] [-debug=<value>] [-nodirichlet] [-noneumann]\n";

  if( Oges::debug > 3 )
    SparseRepForMGF::debug=3;  

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  aString nameOfOGFile=gridName[0];
  aString outnameDirichlet  = nameOfOGFile + "_d"+".petsc";
  aString outnameNeumann    = nameOfOGFile + "_n"+".petsc";

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();
  
  if( Oges::debug >1 )
  {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  displayMask(cg[grid].mask(),"mask");
  }
  
  const int inflow=1, outflow=2, wall=3;
  
  // create a twilight-zone function for checking the errors
  OGFunction *exactPointer;
  if( min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
  {
      // this grid is probably periodic in space, use a trig function
      printf("TwilightZone: trigonometric polynomial\n");
      exactPointer = new OGTrigFunction(2.,2.);  // 2*Pi periodic
  }
  else
  {
      printf("TwilightZone: algebraic polynomial\n");
      int degreeOfSpacePolynomial = 2;
      int degreeOfTimePolynomial = 1;
      int numberOfComponents = cg.numberOfDimensions();
      exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					degreeOfTimePolynomial);    
  }
  OGFunction & exact = *exactPointer;
  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=int(pow(3,cg.numberOfDimensions())+1);  // add 1 for interp
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  

  Oges solver( cg );                     // create a solver
  int solverType=OgesParameters::PETSc; 
  solver.set(OgesParameters::THEsolverType,solverType); 

  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg);
  f=0.;
  
  CompositeGridOperators op(cg);     
  op.setStencilSize(stencilSize);
  
  f.setOperators(op); // for apply the BC
  coeff.setOperators(op);

  //
  // --- DIRICHLET ---
  //

  if (saveDirichlet)
  {

      coeff=0.;
      coeff=op.laplacianCoefficients();  
      // fill in the coefficients for the boundary conditions
      coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
      coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
      coeff.finishBoundaryConditions();
      
      solver.setCoefficientArray( coeff );   // supply coefficients
      
      // assign the rhs: Laplacian(u)=f, u=exact on the boundary
      Index I1,I2,I3, Ia1,Ia2,Ia3;
      int side,axis;
      Index Ib1,Ib2,Ib3;
      int grid;
      
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	  MappedGrid & mg = cg[grid];
	  getIndex(mg.indexRange(),I1,I2,I3);  
	  
	  if( cg.numberOfDimensions()==1 )
	      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
	  else if( cg.numberOfDimensions()==2 )
	      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
	  else
	      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);
	  
	  ForBoundary(side,axis)
	      {
		  if( mg.boundaryCondition()(side,axis) > 0 )
		  {
		      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		      // f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
		      f[grid].applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::boundary(side,axis),exact(mg,Ib1,Ib2,Ib3,0));
		  }
	      }
      }
      u=0.;  // initial guess for iterative solvers
      solver.writePetscMatrixToFile( outnameDirichlet, u, f );  
  }
  
  
  // ----- Neumann BC's ----

  if (saveNeumann) 
  {
      coeff=0.;
      coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
      // fill in the coefficients for the boundary conditions
      coeff.applyBoundaryConditionCoefficients(0,0,neumann,allBoundaries);
      coeff.finishBoundaryConditions();
      
      Index Ig1,Ig2,Ig3;
      bool singularProblem=TRUE;  

      Index I1,I2,I3, Ia1,Ia2,Ia3;
      int side,axis;
      Index Ib1,Ib2,Ib3;
      int grid;
           
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	  MappedGrid & mg = cg[grid];
	  getIndex(mg.indexRange(),I1,I2,I3);  
	  if( mg.numberOfDimensions()==1 )
	      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
	  else if(  mg.numberOfDimensions()==2 )
	      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
	  else 
	      f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);
	  ForBoundary(side,axis)
	  {
	      if( mg.boundaryCondition()(side,axis) > 0  )
	      { // for Neumann BC's -- fill in f on first ghostline
		  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
		  RealArray & normal = mg.vertexBoundaryNormal(side,axis);
		  if( mg.numberOfDimensions()==1 )
		      f[grid](Ig1,Ig2,Ig3)=(2*side-1)*exact.x(mg,Ib1,Ib2,Ib3,0);  
		  else if( mg.numberOfDimensions()==2 )
		      f[grid](Ig1,Ig2,Ig3)=
			  normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
			  +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0);
		  else
		      f[grid](Ig1,Ig2,Ig3)=
			  normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
			  +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
			  +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0);
	      }
	      else if( mg.boundaryCondition()(side,axis) ==inflow ||  mg.boundaryCondition()(side,axis) ==outflow )
	      {
		  singularProblem=FALSE;
		  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
	      }
	  }
      }
      // if the problem is singular Oges will add an extra constraint equation to make the system nonsingular
      if( singularProblem )
	  solver.set(OgesParameters::THEcompatibilityConstraint,TRUE);
      // Tell the solver to refactor the matrix since the coefficients have changed
      solver.setRefactor(TRUE);
      // we need to reorder too because the matrix changes a lot for the singular case
      solver.setReorder(TRUE);
      
      if( singularProblem )
      {
	  // we need to first initialize the solver before we can fill in the rhs for the compatibility equation
	  solver.initialize();
	  int ne,i1e,i2e,i3e,gride;
	  solver.equationToIndex( solver.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
	  f[gride](i1e,i2e,i3e)=0.;
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	      getIndex(cg[grid].dimension(),I1,I2,I3);
	      f[gride](i1e,i2e,i3e)+=sum(solver.rightNullVector[grid](I1,I2,I3)*exact(cg[grid],I1,I2,I3,0,0.));
	  }
      }
      u=0.;  // initial guess for iterative solvers
      solver.writePetscMatrixToFile( outnameNeumann, u, f );  
  }

  return(0);
}


