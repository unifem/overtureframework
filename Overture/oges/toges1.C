//===============================================================================
//  Test the Overlapping Grid Equation Solver
//==============================================================================

#include "Oges.h"  
#include "OGTrigFunction.h"  // Trigonometric function
#include "OGPolyFunction.h"  // polynomial function
void assignRightHandSide( Oges & oges, realCompositeGridFunction & f, OGFunction & exactSolution );

int
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Oges::debug=7;              // set debug flag for Oges

  aString nameOfOGFile, nameOfShowFile, nameOfDirectory;
  
  cout << "toges1>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;
  cout << "toges1: Enter the directory to use:" << endl;
  cin >> nameOfDirectory;

  cout << "Create a CompositeGrid..." << endl;
  MultigridCompositeGrid mgcg(nameOfOGFile,nameOfDirectory);
  CompositeGrid & cg=mgcg[0];  // use multigrid level 0
  cg.update();
  createInverseVertexDerivative(cg);    // this should go away!

  Oges solver( cg );                    // create a solver

  // Assign parameters 
  solver.setEquationType( Oges::LaplaceDirichlet ); // Use one of the predefined equations 
  solver.initialize( );   // initialize oges (assigns classify array used below)

  // create grid functions: (numberOfComponents=1, positionOfComponent=3)
  Range all;
  realCompositeGridFunction u(cg,all,all,all),f(cg,all,all,all);

  // assign the rhs: u.xx+u.yy=1, u=0 on the boundary
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    f[grid]=0.;
    where( solver.classify[grid]>0 )
      f[grid]=1.;
    where( solver.classify[grid]==Oges::boundary )
      f[grid]=0.;
  }    

  solver.solve( u,f );   // solve the equations

  // Now solve again, comparing to a true solution
  OGTrigFunction tz(1.,1.,1.);  // create an exact solution (Twilight-Zone solution)
  assignRightHandSide( solver,f,tz );  // assign f so that the true solution is known

  solver.solve( u,f );   // solve the equations

  // ...Calculate the maximum error  (for Twilight-zone flow )
  int printOptions = 1;  // bitflag: 1=print max errors, 8=print errors, 16=print solution

  solver.determineErrors(  u, tz, printOptions );

  return(0);

}
