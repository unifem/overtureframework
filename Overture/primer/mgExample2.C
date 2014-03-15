#include "Overture.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "OGPolyFunction.h"


// These macros define how to access the elements in a coefficient matrix. See the example below
#undef C
#undef M123
#define M123(m1,m2,m3) (m1+halfWidth1+width1*(m2+halfWidth2+width2*(m3+halfWidth3)))
#define COEFF(m1,m2,m3,I1,I2,I3) c(M123(m1,m2,m3),I1,I2,I3)

int 
main(int argc, char* argv[]) 
{
  ios::sync_with_stdio();
  Index::setBoundsCheck(On);

  printf(" -------------------------------------------------------------------------- \n");
  printf(" Demonstrate how to solve an elliptic problem on different multigrid levels.\n");
  printf(" The overlapping grid should be created with more than 1 multigrid level,   \n");
  printf(" see the cicmg.cmd command file as an example.                              \n");
  printf(" -------------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "mgExample2>> Enter the name of the (old) overlapping grid file: (cicmg for example)" << endl;
  cin >> nameOfOGFile;

  // create and read in a (multigrid) CompositeGrid
  CompositeGrid cgmg;
  getFromADataBase(cgmg,nameOfOGFile);
  cgmg.update();

  // allocate operators and sparse solvers for all levels
  CompositeGridOperators *opMG = new CompositeGridOperators [cgmg.numberOfMultigridLevels()];
  Oges *solverMG = new Oges [cgmg.numberOfMultigridLevels()]; 
  // Now build a coefficient matrix
  Range all;
  const int stencilSize=int( pow(3,cgmg.numberOfDimensions())+1.5 );
  realCompositeGridFunction coeffMG(cgmg,stencilSize,all,all,all);

  // build grid functions for the solution and rhs
  realCompositeGridFunction uMG(cgmg), fMG(cgmg);
  // create a twilight-zone function for checking the errors
  int degreeOfSpacePolynomial = 2;
  int degreeOfTimePolynomial = 1;
  int numberOfComponents = cgmg.numberOfDimensions();
  OGPolyFunction exact(degreeOfSpacePolynomial,cgmg.numberOfDimensions(),numberOfComponents,
		      degreeOfTimePolynomial);

  // Now loop over each level. Solve a Poisson problem on each level
  for( int level=0; level<cgmg.numberOfMultigridLevels(); level++ )
  {
    // first make some references for ease of use
    CompositeGrid & cg = cgmg.multigridLevel[level];
    if( FALSE && level>0 )
    {
      cg.interpoleeGrid[0].display("cg.interpoleeGrid");
      cg.interpolationPoint[0].display("cg.interpolationPoint.display");
    }
    

    realCompositeGridFunction & coeff = coeffMG.multigridLevel[level];
    realCompositeGridFunction & u = uMG.multigridLevel[level];
    realCompositeGridFunction & f = fMG.multigridLevel[level];
    CompositeGridOperators & op = opMG[level];

    op.updateToMatchGrid(cg);  // the operators on this level must be associated with a grid (once only)
    op.setStencilSize(stencilSize);   // set stencil size for operators

    coeff.setIsACoefficientMatrix(TRUE, stencilSize);
    coeff.setOperators(op);
    coeff = op.laplacianCoefficients();
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,  BCTypes::allBoundaries);
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries);
    coeff.finishBoundaryConditions();

    Oges & solver = solverMG[level];
    solver.setCoefficientArray( coeff );   // supply coefficients
    solver.updateToMatchGrid( cg );        // create a solver, and update to the grid (once only)
    
    // assign the rhs: Laplacian(u)=f, u=exact on the boundary
    Index I1,I2,I3, Ia1,Ia2,Ia3;
    int side,axis,grid;
    Index Ib1,Ib2,Ib3;
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
      // loop over the boundaries
      for( axis=0; axis<mg.numberOfDimensions(); axis++ )
      for( side=0; side<=1; side++ )
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
	}
      }
    }
  
    u=0.;  // initial guess for iterative solvers
    real time0=getCPU();
    solver.solve( u,f );   // solve the equations
    real time=getCPU()-time0;
    cout << "level=" << level << ", time for solve of the Dirichlet problem = " << time << endl;
  
    real error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].indexRange(),I1,I2,I3);  
      where( cg[grid].mask()(I1,I2,I3)!=0 )
	error=max(error,max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))));    
    }
    printf("level=%i, Maximum error with dirichlet bc's= %e\n",level,error);  
   
    // Now compute the maximum residual
    real maximumResidual=0.;
    realCompositeGridFunction residual(cg);
    // These stencil widths are used by the COEFF macro
    const int width1=3, halfWidth1=width1/2, width2=3, halfWidth2=width2/2;
    const int width3= cg.numberOfDimensions()==2 ? 1 : 3, halfWidth3=width3/2;
    
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      realArray & c = coeff[grid];
      realArray & uu= u[grid];
      realArray & ff= f[grid];
      realArray & res = residual[grid];
      // We must first reshape the arrays so that we can multiply by the coefficient matrix
      uu.reshape(1,uu.dimension(0),uu.dimension(1),uu.dimension(2));
      ff.reshape(1,ff.dimension(0),ff.dimension(1),ff.dimension(2));
      res.reshape(1,res.dimension(0),res.dimension(1),res.dimension(2));
      
      getIndex(cg[grid].indexRange(),I1,I2,I3);  

      if( cg.numberOfDimensions()==2 )
      {
        // The COEFF macro makes the coeff array look like a 6 dimensional array.
	res(0,I1,I2,I3)=ff(0,I1,I2,I3)-(
	   COEFF( 0, 0,0,I1,I2,I3)*uu(0,I1  ,I2  ,I3)
	  +COEFF( 1, 0,0,I1,I2,I3)*uu(0,I1+1,I2  ,I3)
	  +COEFF( 0, 1,0,I1,I2,I3)*uu(0,I1  ,I2+1,I3)
	  +COEFF(-1, 0,0,I1,I2,I3)*uu(0,I1-1,I2  ,I3)
	  +COEFF( 0,-1,0,I1,I2,I3)*uu(0,I1  ,I2-1,I3)
	  +COEFF( 1, 1,0,I1,I2,I3)*uu(0,I1+1,I2+1,I3)
	  +COEFF( 1,-1,0,I1,I2,I3)*uu(0,I1+1,I2-1,I3)
	  +COEFF(-1, 1,0,I1,I2,I3)*uu(0,I1-1,I2+1,I3)
	  +COEFF(-1,-1,0,I1,I2,I3)*uu(0,I1-1,I2-1,I3)
	  );
      }
      else
      {
	res(0,I1,I2,I3)=ff(0,I1,I2,I3)-(
	   COEFF(-1,-1,-1,I1,I2,I3)*uu(0,I1-1,I2-1,I3-1)
	  +COEFF( 0,-1,-1,I1,I2,I3)*uu(0,I1  ,I2-1,I3-1)
	  +COEFF( 1,-1,-1,I1,I2,I3)*uu(0,I1+1,I2-1,I3-1)
	  +COEFF(-1, 0,-1,I1,I2,I3)*uu(0,I1-1,I2  ,I3-1)
	  +COEFF( 0, 0,-1,I1,I2,I3)*uu(0,I1  ,I2  ,I3-1)
	  +COEFF( 1, 0,-1,I1,I2,I3)*uu(0,I1+1,I2  ,I3-1)
	  +COEFF(-1, 1,-1,I1,I2,I3)*uu(0,I1-1,I2+1,I3-1)
	  +COEFF( 0, 1,-1,I1,I2,I3)*uu(0,I1  ,I2+1,I3-1)
	  +COEFF( 1, 1,-1,I1,I2,I3)*uu(0,I1+1,I2+1,I3-1)
				       	    	 
	  +COEFF(-1,-1, 0,I1,I2,I3)*uu(0,I1-1,I2-1,I3  )
	  +COEFF( 0,-1, 0,I1,I2,I3)*uu(0,I1  ,I2-1,I3  )
	  +COEFF( 1,-1, 0,I1,I2,I3)*uu(0,I1+1,I2-1,I3  )
	  +COEFF(-1, 0, 0,I1,I2,I3)*uu(0,I1-1,I2  ,I3  )
	  +COEFF( 0, 0, 0,I1,I2,I3)*uu(0,I1  ,I2  ,I3  )
	  +COEFF( 1, 0, 0,I1,I2,I3)*uu(0,I1+1,I2  ,I3  )
	  +COEFF(-1, 1, 0,I1,I2,I3)*uu(0,I1-1,I2+1,I3  )
	  +COEFF( 0, 1, 0,I1,I2,I3)*uu(0,I1  ,I2+1,I3  )
	  +COEFF( 1, 1, 0,I1,I2,I3)*uu(0,I1+1,I2+1,I3  )
				       	    	 
	  +COEFF(-1,-1, 1,I1,I2,I3)*uu(0,I1-1,I2-1,I3+1)
	  +COEFF( 0,-1, 1,I1,I2,I3)*uu(0,I1  ,I2-1,I3+1)
	  +COEFF( 1,-1, 1,I1,I2,I3)*uu(0,I1+1,I2-1,I3+1)
	  +COEFF(-1, 0, 1,I1,I2,I3)*uu(0,I1-1,I2  ,I3+1)
	  +COEFF( 0, 0, 1,I1,I2,I3)*uu(0,I1  ,I2  ,I3+1)
	  +COEFF( 1, 0, 1,I1,I2,I3)*uu(0,I1+1,I2  ,I3+1)
	  +COEFF(-1, 1, 1,I1,I2,I3)*uu(0,I1-1,I2+1,I3+1)
	  +COEFF( 0, 1, 1,I1,I2,I3)*uu(0,I1  ,I2+1,I3+1)
	  +COEFF( 1, 1, 1,I1,I2,I3)*uu(0,I1+1,I2+1,I3+1)
	  );
      }
      // reshape the arrays back to their original shape -- this would
      // be essential if the GridFunctions u,f or res were used again
      uu.reshape(uu.dimension(1),uu.dimension(2),uu.dimension(3));
      ff.reshape(ff.dimension(1),ff.dimension(2),ff.dimension(3));
      res.reshape(res.dimension(1),res.dimension(2),res.dimension(3));

      // compute the residual at all discretization points
      where( cg[grid].mask()(I1,I2,I3) > 0 )
        maximumResidual=max(maximumResidual,max(fabs(res(I1,I2,I3))));
    }
    printf("level=%i, Maximum residual with dirichlet bc's= %e\n",level,maximumResidual);  

  }
  return 0;
}
