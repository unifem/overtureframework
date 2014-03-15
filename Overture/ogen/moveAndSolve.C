// ====================================================================================
// moveAndSolve.C:
//
//   Routine to test the speed of solving an elliptic equation many times on a moving grid.
// ===================================================================================

#include "Ogen.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "mogl.h"
#include "MatrixTransform.h"
#include "OGPolyFunction.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "display.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  aString nameOfOGFile;
  nameOfOGFile="stir";  // "sis.hdf";
  aString solverTypeName="yale";
  int solverType=OgesParameters::yale;

  if( argc > 1 )
    nameOfOGFile=argv[1];
  if( argc > 2 )
  {
    solverTypeName = argv[2];
    if( solverTypeName=="yale" )
      solverType=OgesParameters::yale;
    else if( solverTypeName=="slap" )
      solverType=OgesParameters::SLAP;
    else if( solverTypeName=="petsc" )
      solverType=OgesParameters::PETSc;
    else
    {
      printf("Error: unknown solverType = %s\n",(const char *)solverTypeName);
      return 0;
    }
  }

  if( argc<=1 )
  {
    cout << "Usage: `moveAndSolve [grid] [solverType] ' \n";
    cout << "     solverType=yale,slap,petsc    ' \n";
  }

  
  cout << "Create a CompositeGrid = " << nameOfOGFile << endl;
  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);

  CompositeGrid cg[2];                              // use these two grids for moving
  cg0.update(); 
  cg[0].reference(cg0);
  cg[1]=cg0; cg[1].update(); 

  int numberOfGridPoints=0;
  for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
  {
    const IntegerArray & d = cg0[grid].dimension();
    numberOfGridPoints+=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);
  }

  PlotStuff ps;
  PlotStuffParameters psp;
    
  // Here is the grid generator
  Ogen gridGenerator(ps);


  psp.set(GI_TOP_LABEL,"Original grid, cg0");  // set title
  PlotIt::plot(ps,cg0,psp);

  // ---- Move the grid a bunch of times.----
  
  
  // Change the mapping on this grid:
  int gridToMove=0;
  Mapping *mapPointer = cg0[gridToMove].mapping().mapPointer;

  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform *transform[2];
  transform[0]= new MatrixTransform(*mapPointer);
  transform[1]= new MatrixTransform(*mapPointer);

  // Change the mappings that the grids point to: (***This will invalidate the grid data, the mask etc.)
  // ***WARNING**** This will invalidate the grid data, the mask etc. *****
  cg[0][gridToMove].reference(*transform[0]);  transform[0]->incrementReferenceCount();
  cg[1][gridToMove].reference(*transform[1]);  transform[1]->incrementReferenceCount();

  // for( int g=0; g<cg[0].numberOfComponentGrids(); g++ )
  //  displayMask(cg[0][g].mask(),"g.mask()");

  cg[0][gridToMove].update();  // the previous reference seems to destroy the data
  cg[1][gridToMove].update();  // the previous reference seems to destroy the data
  
  int numberOfSteps=20;   // ********* number of steps **********
  real deltaAngle=.25;
  deltaAngle*=Pi/180.;
  real xShift=.01;
  int debug=2;
  enum MoveOptions
  {
    rotate=0,
    shift
  } moveOption;

  int move;
//  cout << "Enter moveOption: 0=rotate, 1=shift\n";
//   cin >> move;
  move=0;
  
  moveOption = (MoveOptions)move;
  

  Range all;
  // use this twilight-zone function so we can compute errors in interpolating exposed points
  int degreeX=1;
  OGPolyFunction exact(degreeX,cg0.numberOfDimensions(),2,1);   

  // make a grid function to hold the coefficients
  int stencilSize=int(pow(3,cg[0].numberOfDimensions())+1.5);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg[0],stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
  coeff=0.;
    
  realCompositeGridFunction w(cg[0]),f(cg[0]);
  w=0.; // for iterative solvers

  CompositeGridOperators op(cg[0]);                            // create some differential operators
  op.setStencilSize(stencilSize);


  Oges solver( cg[0] );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients
  const real tolerance=max(1.e-8,REAL_EPSILON*10.);
  solver.set(OgesParameters::THEsolverType,solverType); 
  if( solverType==OgesParameters::SLAP ||  solverType==OgesParameters::PETSc )
  {
    solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
    solver.set(OgesParameters::THEtolerance,max(1.e-8,REAL_EPSILON*10.));
  }    

  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  LogicalArray hasMoved(2);
  hasMoved    = LogicalFalse;
  hasMoved(gridToMove) = LogicalTrue;  // Only this grid will move.

  char buff[80];
  aString showFileTitle[2];
  real currentAngle=0.;
    
  real gridGenerationTime=0.;
  real matrixSetUpTime=0.;
  real matrixSolveTime=0., matrixSolveTime2=0.;

  int numberOfArrays=GET_NUMBER_OF_ARRAYS;
  int numberOfIterations=0;

  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    // Draw the overlapping grid

    if( moveOption==rotate )
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i, angle=%6.2e",i,currentAngle*180./Pi));  // set title
    else
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i",i));  // set title
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    ps.erase();
    PlotIt::plot(ps,cg[oldCG],psp);
    ps.redraw(TRUE);   // force a redraw

    //  Rotate the grid...
    // After the first step we must double the angle since we start from the old grid
    if( moveOption==rotate )
    {
      real angle = i==1 ? deltaAngle : deltaAngle*2.; 
      transform[newCG]->rotate(axis3,angle);
      currentAngle+=deltaAngle;
    }
    else
    {
      real delta = i==1 ? xShift : xShift*2.; 
      transform[newCG]->shift(delta,delta,0.);
    }
    
    //  Update the overlapping newCG, starting with and sharing data with oldCG.    
    Ogen::MovingGridOption option = i==1 ? Ogen::useFullAlgorithm : Ogen::useOptimalAlgorithm;
    // if( (i%10) == 4  )
    if( i==1 || (i%10) == 40 )
    {
      cout << "\n +++++++++++ use full algorithm in updateOverlap +++++++++++++++ \n";
      option=Ogen::useFullAlgorithm;
    }

    if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
    {
      numberOfArrays=GET_NUMBER_OF_ARRAYS;
      printf("**** Before gridGenerator:WARNING: number of A++ arrays has increased to = %i \n",GET_NUMBER_OF_ARRAYS);
    }

    real time0=getCPU();
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved, option);
    gridGenerationTime+=getCPU()-time0;
    
    if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
    {
      numberOfArrays=GET_NUMBER_OF_ARRAYS;
      printf("**** After gridGenerator:WARNING: number of A++ arrays has increased to = %i \n",GET_NUMBER_OF_ARRAYS);
    }

    // solve Laplace's equation on the new grid

    time0=getCPU();
      
    op.updateToMatchGrid(cg[newCG]);
    coeff.updateToMatchGrid(cg[newCG]);
    coeff.setIsACoefficientMatrix(TRUE,stencilSize);  // this is needed to reset the classify array****
    coeff.setOperators(op);
    coeff=0.;
      
    coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,  BCTypes::allBoundaries);
    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries);

    coeff.finishBoundaryConditions();

    matrixSetUpTime+=getCPU()-time0;
    time0=getCPU();
    if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
    {
      numberOfArrays=GET_NUMBER_OF_ARRAYS;
      printf("**** After build coeff:WARNING: number of A++ arrays has increased to = %i \n",GET_NUMBER_OF_ARRAYS);
    }
      
      // coeff.display("Here is coeff after finishBoundaryConditions");
    solver.setCoefficientArray( coeff );   // supply coefficients

    // This next call will cause the matrix to be recreated and refactored
    solver.updateToMatchGrid(cg[newCG]);
//    if( i>1 )
//      solver.setReorder(FALSE);

    f.updateToMatchGrid(cg[newCG]);
    w.updateToMatchGrid(cg[newCG]);
    // assign the rhs: Laplacian(u)=f, u=exact on the boundary
    Index I1,I2,I3, Ia1,Ia2,Ia3;
    int side,axis;
    Index Ib1,Ib2,Ib3;
    int grid;
    for( grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[newCG][grid];
      getIndex(mg.indexRange(),I1,I2,I3);  

      if( mg.numberOfDimensions()==1 )
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
      else if( mg.numberOfDimensions()==2 )
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
      else
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
	}
      }
    }
  
    time0=getCPU();
    w=0.;
    solver.solve( w,f );   // solve the equations
    numberOfIterations+=solver.numberOfIterations;
    matrixSolveTime+=getCPU()-time0;
    // solve again
    time0=getCPU();
    w=0.;
    solver.solve( w,f );   // solve the equations
    numberOfIterations+=solver.numberOfIterations;
    
    matrixSolveTime2+=getCPU()-time0;

      // ...Calculate the maximum error  (for Twilight-zone flow )
    real error=0.;
    for( grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[newCG][grid];
      getIndex(mg.indexRange(),I1,I2,I3,1);  
      where( mg.mask()(I1,I2,I3)!=0 )
	error=max(error, max(abs(w[grid](I1,I2,I3)-exact(mg,I1,I2,I3,0)))/
		  max(abs(exact(mg,I1,I2,I3,0))) );
      if( Oges::debug & 8 )
      {
	realArray err(I1,I2,I3);
	err(I1,I2,I3)=abs(w[grid](I1,I2,I3)-exact(mg,I1,I2,I3,0))/max(abs(exact(mg,I1,I2,I3,0)));
	where( mg.mask()(I1,I2,I3)==0 )
	  err(I1,I2,I3)=0.;
	err.display("abs(error on indexRange +1)");
	// abs(w[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
      }
    }
    printf("Maximum relative error with dirichlet bc's= %e\n",error);  

    if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
    {
      numberOfArrays=GET_NUMBER_OF_ARRAYS;
      printf("**** WARNING: number of A++ arrays has increased to = %i \n",GET_NUMBER_OF_ARRAYS);
    }

  } // end for
  printf("\n -----------------------------------------------------------------------------\n");
  printf(" Total number of grid points = %i \n",numberOfGridPoints);
  printf(" Solver type = %s, \n",(const char *)solver.parameters.getSolverName());
  printf(" Average time for matrix setup..................%7.2e \n"
         " Average time for matrix solve..................%7.2e(first) %7.2e(second) \n"
         " Average number of iterations...................%i (tolerance=%6.2e)\n"
         " Average time for grid generation...............%7.2e \n",
          matrixSetUpTime/numberOfSteps,
          matrixSolveTime/numberOfSteps,matrixSolveTime2/numberOfSteps,
	 numberOfIterations/(2*numberOfSteps),tolerance,
          gridGenerationTime/numberOfSteps);
  
  for( int m=0; m<=1; m++ )
  {
    if( transform[m]->decrementReferenceCount()==0 )
      delete transform[m];
  }
  cout << "Done! ...\n";

  Overture::finish();          
  return 0;
}

