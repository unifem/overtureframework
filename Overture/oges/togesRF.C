//===============================================================================
//  Test the Overlapping Grid Equation Solver
//    Demonstrate the Refactor Option
//==============================================================================

#include "Oges.h"  
#include "CompositeGridOperators.h"

// -- here are some useful macros:
#define ForBoundary(side,axis)   for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  
  Oges::debug=0;              // set debug flag for Oges

  aString nameOfOGFile, nameOfShowFile, nameOfDirectory;
  
  cout << "Enter the name of the composite grid file " << endl;
  cin >> nameOfOGFile; 

  cout << "Create a CompositeGrid..." << endl;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  // make a grid function to hold the coefficients
  int stencilSize=3*3+1;    // add 1 since Oges stores interpolation equations here too
  realCompositeGridFunction coeff(cg,Range(0,stencilSize-1));
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);

  coeff.setOperators(op);

  // assign the coefficients
  Index I1,I2,I3,Ib1,Ib2,Ib3,R[3];  
  int side,axis;
  coeff=0.;  // initialize all values to 0
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3,-1);  // Indices for interior points
    coeff[grid](0,I1,I2) =   0.;      //  coefficient of i1-1,i2-1 (no need to set as already zero)
    coeff[grid](1,I1,I2) =   1.;      //  coefficient of i1  ,i2-1
    coeff[grid](2,I1,I2) =   0.;      //  coefficient of i1+1,i2-1 (no need to set as already zero)
    coeff[grid](3,I1,I2) =   1.;      //  coefficient of i1-1,i2  
    coeff[grid](4,I1,I2) =  -4.;      //  coefficient of i1  ,i2  
    coeff[grid](5,I1,I2) =   1.;      //  coefficient of i1+1,i2  
    coeff[grid](6,I1,I2) =   0.;      //  coefficient of i1-1,i2+1 (no need to set as already zero) 
    coeff[grid](7,I1,I2) =   1.;      //  coefficient of i1  ,i2+1
    coeff[grid](8,I1,I2) =   0.;      //  coefficient of i1+1,i2+1 (no need to set as already zero)
    ForBoundary(side,axis)  // for each boundary (see macro above)
    {
      if( cg[grid].boundaryCondition()(side,axis) > 0 )
      {
        getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3); 
        coeff[grid](4,Ib1,Ib2) = 1.;               // apply a Dirichlet BC on the boundary
      }
    }
  }
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries);
  coeff.finishBoundaryConditions();

  Oges solver( cg );                     // create a solver

  // create grid functions:
  Range all;
  realCompositeGridFunction u(cg,all,all,all),f(cg,all,all,all);

  // assign the rhs: u.xx+u.yy=1, u=0 on the boundary
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    f[grid]=1.;
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	f[grid](Ib1,Ib2,Ib3)=0.;
      }
    }
  }    

  int numberOfIterations = 10;
  for( int it=0; it<numberOfIterations; it++ )
  {
    bool refactor= (it % 2) == 0; // refactor the matrix every second step
    solver.setRefactor(refactor);
    if( (it % 4)==0 )                        // change coefficients every fourth step
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].indexRange(),I1,I2,I3,-1);  // Indices for interior points
	coeff[grid](0,I1,I2) =   .0;       coeff[grid](1,I1,I2) =   1.; 
	coeff[grid](2,I1,I2) =   .0;       coeff[grid](3,I1,I2) =   1.; 
	coeff[grid](4,I1,I2) =  -4.; 
	coeff[grid](5,I1,I2) =   1.;       coeff[grid](6,I1,I2) =   .0; 
	coeff[grid](7,I1,I2) =   1.;       coeff[grid](8,I1,I2) =   .0; 
      }
    }
    else
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].indexRange(),I1,I2,I3,-1);  // Indices for interior points
	coeff[grid](0,I1,I2) =   .5;       coeff[grid](1,I1,I2) =   .5; 
	coeff[grid](2,I1,I2) =   .5;       coeff[grid](3,I1,I2) =   .5; 
	coeff[grid](4,I1,I2) =  -4.; 
	coeff[grid](5,I1,I2) =   .5;       coeff[grid](6,I1,I2) =   .5; 
	coeff[grid](7,I1,I2) =   .5;       coeff[grid](8,I1,I2) =   .5; 
      }
    }

    solver.setCoefficientArray( coeff );   // supply coefficients

    u=0.;
    real time=getCPU();
    solver.solve( u,f );                   // solve the equations
    time=getCPU()-time;
    printf(" iteration %i : refactor=%i, time for solve=%9.3e\n",it,refactor,time);
    
    if( Oges::debug & 4 ) 
      u.display("Here is the solution");

  }
  return(0);

}
