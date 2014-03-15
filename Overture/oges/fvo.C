//===============================================================================
//   Test out David's Finite Volume Operators
//==============================================================================
#include "Overture.h"  
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "CompositeGridFiniteVolumeOperators.h"

main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  cout <<"Enter a value for Oges::debug \n";
  cin >> Oges::debug;              // set debug flag for Oges

  aString nameOfOGFile, nameOfDirectory=".";
  cout << "Enter the name of the composite grid file (in the cguser directory)" << endl;
  cin >> nameOfOGFile;   nameOfOGFile="/n/c3servet/henshaw/res/cgsh/" + nameOfOGFile;

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  // make a grid function to hold the coefficients
  int stencilSize=3*3+1;    // add 1 since Oges stores interpolation equations here too
  Range all;
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
    
  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg);

  CompositeGridFiniteVolumeOperators op(cg);                // create some differential operators
  op.setNumberOfBoundaryConditions(1);                      // 1 boundary condition
  op.setBoundaryCondition(MappedGridFiniteVolumeOperators::dirichlet);  // Dirichlet

  // ** some tests ****
  realCompositeGridFunction v;
  v=op.laplacian();  
  v.display("Here is the solution");
  for( grid=0; grid<v.numberOfComponentGrids(); grid++ )
    v[grid].isCellCentered().display("------Here is v.isCellCentered()--------");



  coeff=op.Laplacian();                   // get the coefficients for the Laplace operator
  op.ApplyBoundaryConditions(coeff);       // fill in the coefficients for the boundary conditions
  coeff.display("Here is the coeff array:");
  
  Oges solver( cg );                     // create a solver
  solver.setEquationType( Oges::userSuppliedArray ); 
  solver.setCoefficientArray( coeff );   // supply coefficients
  solver.setOrderOfAccuracy(2);
  solver.setNumberOfGhostLines(1);  
  solver.setGhostLineOption(1,Oges::useGhostLineExceptCorner);
  solver.initialize( );                  // initialize oges (assigns classify array used below)


  // assign the rhs: u.xx+u.yy=1, u=0 on the boundary
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    f[grid]=0.;
    where( solver.classify[grid]>0 )
      f[grid]=1.;
    where( solver.classify[grid]==Oges::ghost1 ) // Dirichlet BC's are assigned on first ghost line
      f[grid]=0.;
  }    

  solver.solve( u,f );   // solve the equations

  u.display("Here is the solution to u.xx+u.yy=1");

  return(0);

}
