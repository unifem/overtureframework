// ====================================================================================
//      Test moving grids
// ===================================================================================

#include "Ogen.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "mogl.h"
#include "MatrixTransform.h"
#include "OGPolyFunction.h"
#include "Oges.h"
#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "interpPoints.h"
#include "CompositeGridOperators.h"
#include "display.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  // Mapping::debug=7;

//   ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
//   Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile;
  nameOfOGFile="sis2.hdf";
  
  
  cout << "Create a CompositeGrid..." << endl;
  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);

  aString nameOfShowFile = "move2.show";
  Ogshow show( nameOfShowFile );
  show.saveGeneralComment("Moving grid example");
  show.setMovingGridProblem(TRUE);
  show.setFlushFrequency(1);


  CompositeGrid cg[2];                              // use these two grids for moving

  cg0.update(); // m0.update

//  cg[0]=cg0; cg[0].update(); 
  cg[0].reference(cg0);
  cg[1]=cg0; cg[1].update(); 

//    cg0[1].dimension().display("cg0[1].dimension()");
//    cg[1][1].dimension().display("cg[1][1].dimension()");
  
  bool plotOption=false; // true;
  PlotStuff ps(plotOption,"move2");
  PlotStuffParameters psp;
    
  // Here is the grid generator
  Ogen gridGenerator(ps);




  psp.set(GI_TOP_LABEL,"Original grid, cg0");  // set title
//  PlotIt::plot(ps,cg0,psp);

  // ---- Move the grid a bunch of times.----
  
  
  int gridToMove=cg0.numberOfComponentGrids()-1; // =1;

  // Change the mapping on gridToMove:
  Mapping *mapPointer = cg0[gridToMove].mapping().mapPointer;

  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform *transform[2];
  transform[0]= new MatrixTransform(*mapPointer); transform[0]->incrementReferenceCount();
  transform[1]= new MatrixTransform(*mapPointer); transform[1]->incrementReferenceCount();

  // Change the mappings that the grids point to:
  // Change the mapping of component grid 1:
  cg[0][gridToMove].reference(*transform[0]); // this will invalidate the mask ! 
  cg[1][gridToMove].reference(*transform[1]); 

  cg[0][gridToMove].update();  // the previous reference seems to destroy the data
  cg[1][gridToMove].update();  // the previous reference seems to destroy the data

//  cg[0][1].dimension().display("cg[0][1].dimension()");
//  cg[1][1].dimension().display("cg[1][1].dimension()");

  // update the initial grid, since the above reference destroys the mask
  gridGenerator.updateOverlap(cg[0]);
  

  int numberOfSteps=21; // ***********************************
  real deltaAngle;
  real xShift=.01;
  int debug=0;
  enum MoveOptions
  {
    rotate=0,
    shift
  } moveOption;

  int move;
  cout << "Enter moveOption: 0=rotate, 1=shift\n";
//   cin >> move;
  move=rotate;
//  move=shift;
  
  moveOption = (MoveOptions)move;
  
  if( moveOption==shift )
  {
    cout << "Enter numberOfSteps, shift amount, debug (1=interpolant, 2=Oges, 4=Ogshow) \n";
    // cin >> numberOfSteps >> xShift >> debug;
    xShift=.03; // .01;

    numberOfSteps=3;
    xShift=.1;
    debug=0;
  }
  else
  {
    cout << "Enter numberOfSteps, rotation angle (degrees), debug (1=interpolant, 2=Oges, 4=Ogshow 8=exposed) \n";
    // cin >> numberOfSteps >> deltaAngle >> debug;
    deltaAngle=15.;
    // ** debug =2;
    
    deltaAngle*=Pi/180.;
  }
  printf(" numberOfSteps=%i debug=%i \n",numberOfSteps,debug);

  // Here are some grid functions that we will use to interpolate exposed points
  realCompositeGridFunction u[2];
    
  Range all;
  u[0].updateToMatchGrid(cg[0],all,all,all,2); 
  u[1].updateToMatchGrid(cg[1],all,all,all,2); 
  u[0].setName("u");
  u[0].setName("u0",0);
  u[0].setName("u1",1);
  u[1].setName("u");
  u[1].setName("u0",0);
  u[1].setName("u1",1);
  // use this twilight-zone function so we can compute errors in interpolating exposed points
  int degreeX=1;
  OGPolyFunction exact(degreeX,cg0.numberOfDimensions(),2,1);   
  exact.assignGridFunction(u[0]);
  exact.assignGridFunction(u[1]);

  Interpolant interpolant;
  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);

  // make a grid function to hold the coefficients
  int stencilSize=int( pow(3,cg[0].numberOfDimensions())+1.5);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg[0],stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
  coeff=0.;
    
  realCompositeGridFunction w(cg[0]),f(cg[0]);
  w=0.; // for iterative solvers

  CompositeGridOperators op(cg[0]);                            // create some differential operators
  op.setStencilSize(stencilSize);


  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  LogicalArray hasMoved(2);
  hasMoved    = LogicalFalse;
  hasMoved(1) = LogicalTrue;  // Only this grid will move.

  char buff[80];
  aString showFileTitle[2];
  real currentAngle=0.;
  int grid;

  real matrixSetUpTime=0.;
  real matrixSolveTime=0.;

  int numberOfArrays=GET_NUMBER_OF_ARRAYS;

  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    // Draw the overlapping grid

//     if( moveOption==rotate )
//       psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i, angle=%6.2e",i,currentAngle*180./Pi));  // set title
//     else
//       psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i",i));  // set title

//     if( i==1 )
//       psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
//     ps.erase();
//     PlotIt::plot(ps,cg[oldCG],psp);
//     psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

//     ps.redraw(true);   // force a redraw

    //  Rotate the grid...
    // After the first step we must double the angle since we start from the old grid
    if( true )
    {
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
    }
    
    //      Update the overlapping newCG, starting with and sharing data with oldCG.    
    Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
    
    int useFullAlgorithmInterval=1; // 10000;
    if( i% useFullAlgorithmInterval == useFullAlgorithmInterval-1  )
    {
      // cout << "\n +++++++++++ use full algorithm in updateOverlap +++++++++++++++ \n";
      option=Ogen::useFullAlgorithm;
    }
    // gridGenerator.debug=7;
    printf("gridGenerator.updateOverlap step %i\n",i);
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved, option);
    
    // ****
    // cg[newCG].update(MappedGrid::THEinverseVertexDerivative);

    // cg[newCG][1].inverseVertexDerivative().display("Here is the inverseVertexDerivative");
    
    // *****

    // cout << "cg[newCG].numberOfInterpolationPoints(1) = " << cg[newCG].numberOfInterpolationPoints(1) << endl;
    
    if( min(cg[newCG].interpoleeGrid[0])<0 || max(cg[newCG].interpoleeGrid[0])>cg[newCG].numberOfComponentGrids() )
    {
      cout << "****** error ***** \n";
      cg[newCG].interpoleeGrid[0].display("Here is cg[newCG].interpoleeGrid[0]");
    }
    

    if( Mapping::debug > 0 ) 
      ApproximateGlobalInverse::printStatistics();

    // cg[newCG][1].inverseVertexDerivative().display("Here is the inverseVertexDerivative");
    // cg[newCG][1].vertexDerivative().display("Here is the vertexDerivative on the new grid");
    
    if( i % 100 == 1 )
    {
      u[newCG].updateToMatchGrid(cg[newCG]);
      // Interpolate any exposed points on the old grid function
      // (pass a TwilightZone function and the routine will compute errors)
      if( debug & 8 )
      {
	cout << "Interpolate exposed points...\n";
	interpolateExposedPoints(cg[oldCG],cg[newCG],u[oldCG],&exact);
      }
      // re-evaluate the grid function on the moved grid
      exact.assignGridFunction(u[newCG]);

      show.startFrame();
      sPrintF(buff,"Moving Example, step=%i",i);
      show.saveComment(0,buff);
      show.saveSolution(u[newCG]);
    }
    
    if( debug & 1 )
    {
      // cg[newCG][1].mask().display("Here is cg[newCG][1]");
      cg[newCG][1].update();
      // cg[newCG][1].mask().display("Here is cg[newCG][1] after cg[newCG][1].update");
      

      // interpolate the new grid function
      // first put bogus values in the interpolation and unused points
      Index I1,I2,I3;
      for( grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
      {
        MappedGrid & mg = cg[newCG][grid];
        getIndex(mg.indexRange(),I1,I2,I3); 
        // mg.mask().display("Here is the mask array");
        where( mg.mask()(I1,I2,I3)<=0 )
          u[newCG][grid](I1,I2,I3,0)=1.e5;
      }

      if( debug & 1 )
      {
        interpolant.updateToMatchGrid(cg[newCG]);
        // u[newCG].display("u before interpolate");
        u[newCG].interpolate();
        // u[newCG].display("u after interpolate");
      }
      
      real error=0.;
      for( grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
      {
        MappedGrid & mg = cg[newCG][grid];
        getIndex(mg.indexRange(),I1,I2,I3); 

        // abs(u[newCG][grid](I1,I2,I3,0)-exact.u(mg,I1,I2,I3,0,0.)).display("error");

        where( mg.mask()(I1,I2,I3)!=0 )
          error=max(error,max(abs(u[newCG][grid](I1,I2,I3,0)-exact(mg,I1,I2,I3,0,0.))));
      }
      printf("\n >>>>Maximum error in interpolating = %e <<<<<<\n\n",error);  
    }
    
    real time0;

    if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
    {
      numberOfArrays=GET_NUMBER_OF_ARRAYS;
      printf("**** WARNING: number of A++ arrays has increased to = %i \n",GET_NUMBER_OF_ARRAYS);
    }

  } // end for
  printf("**** END: number of A++ is = %i \n",GET_NUMBER_OF_ARRAYS);
  
  cout << "Done! ...\n";
  for( int m=0; m<=1; m++ )
  {
    if( transform[m]->decrementReferenceCount()==0 )
      delete transform[m];
  }

  Overture::finish();          
  return 0;
}

