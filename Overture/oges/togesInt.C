//===============================================================================
//  Test the Overlapping Grid Equation Solver Interpolant
//==============================================================================

#include "Interpolant.h"
#include "OGTrigFunction.h"  // Trigonometric function
#include "OGPolyFunction.h"  // polynomial function

main()
{
  
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Oges::debug=7;              // set debug flag for Oges
  cout << "toges2: Enter Oges::debug" << endl;
  cin >> Oges::debug;

  aString nameOfOGFile, nameOfShowFile, nameOfDirectory;
  
  cout << "toges2>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;
  cout << "toges2: Enter the directory to use:" << endl;
  cin >> nameOfDirectory;

  cout << "Create a CompositeGrid..." << endl;
  MultigridCompositeGrid mgcg(nameOfOGFile,nameOfDirectory);
  CompositeGrid & cg=mgcg[0];  // use multigrid level 0

  Interpolant interpolant( cg );    // Here is the interpolant object

  // OGTrigFunction tz(1.,1.,1.);      // create an exact solution (Twilight-Zone solution)
  OGPolyFunction tz;      // create an exact solution (Twilight-Zone solution)
  
  realCompositeGridFunction u(cg); // create grid functions:

  tz.assignGridFunction( u,cg );    // set u=Twilight-Zone flow

//  interpolant.interpolate( u );     // interpolate u
//  u.interpolate( interpolant );     // interpolate u  (another way)
  u.interpolate();                    // interpolate u 
    
  // ...Calculate the maximum error  (for Twilight-zone flow )
  Oges solver( cg );
  int printOptions = 9;  // bitflag: 1=print max errors, 8=print errors, 16=print solution
  solver.determineErrors(  u, tz, printOptions );

  return(0);

}
