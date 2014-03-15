//===============================================================================
//  Write Coefficient Matrix to Disk
//
// Usage: `writeSparseMatrix [<gridName>] [<outputfile>]  '
//
//  ** Form overset grid discretization of the Poisson eq.
//  ** Save compressed sparse row formatted matrix to files:
//      using simply triplet format -- we save i,j, a(i,j)
//  ** Matrix is stored in <outputfile>_matrix.dat, and
//     the grid information is stored in <outputfile>_grid.dat
//  ** For file formats, see the documentation for the routines
//     writeMatrixToFile & writeMatrixGridInformationToFile in Oges.C
//
//==============================================================================
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


  const int maxNumberOfGridsToTest=1;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  //  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
  aString nameOfOGFile="cic";

  aString outputMatrixFilename;
  aString outputGridInfoFilename;
  aString baseFilename="";

  if( argc > 2 )
  { 
    numberOfGridsToTest=1;
    //gridName[0]=argv[1];
    nameOfOGFile=argv[1];
    baseFilename=argv[2];
  }
  else
  {
    cout << "Usage: `" << argv[0]
	 <<  " [<grid>] [<baseFilename>] \n";
    cout << "....the matrix is saved in <baseFilename>_matrix.dat & "
	 << " <baseFilename>_grid.dat\n";
    exit(-1);
  }


  outputMatrixFilename=    baseFilename+"_matrix.dat";
  outputGridInfoFilename=  baseFilename+"_grid.dat";

  if( Oges::debug > 3 )
    SparseRepForMGF::debug=3;  

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  //aString nameOfOGFile=gridName[it];
  
  cout << "\n *****************************************************************\n";
  cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
  cout << " *****************************************************************\n\n";
  
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();
  
  if( Oges::debug >1 )
  {
    cout << "------SHOWING THE MASK!!--------\n";
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      displayMask(cg[grid].mask(),"mask");
  }
  
  const int inflow=1, outflow=2, wall=3;
  
  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=int(pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
  coeff=0.;
  
  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg);
  f=0.; // for iterative solvers
  
  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
  
  coeff.finishBoundaryConditions();
  // coeff.display("Here is coeff after finishBoundaryConditions");

  Oges oges( cg );                     // create a solver
  oges.setCoefficientArray( coeff );   // supply coefficients

  bool allocateSpace = TRUE;
  bool factorMatrixInPlace = FALSE; // allocate space for matrix only
  int neq=oges.numberOfEquations;
  int nnz=oges.numberOfNonzeros;

  //..Build Overture matrix & save it
  oges.formMatrix(neq,nnz,
		  Oges::compressedRow,
		  allocateSpace,
		  factorMatrixInPlace);

  cout << "..Saving the matrix in '"+baseFilename +"_matrix.dat'"
       << " and the grid-info in '"+baseFilename +"_grid.dat'\n";


  oges.writeMatrixToFile( outputMatrixFilename );
  oges.writeMatrixGridInformationToFile( outputGridInfoFilename );

}


