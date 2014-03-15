// ====================================================================================
//      Test moving grids
// ===================================================================================

#include "Ogen.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "display.h"
#include "Ugen.h"

int 
main( int argc, char *argv[] ) 
{
  Overture::start(argc,argv);  // initialize Overture
  // Mapping::debug=7;

  aString nameOfOGFile;
  nameOfOGFile="";
  

  printf("Usage: move [grid [commandFile]]\n");

  PlotStuff ps;
  PlotStuffParameters psp;
  aString answer;

  if( argc>1 )
  {
    nameOfOGFile=argv[1];
  }
  else
  {
    ps.inputString(nameOfOGFile,"Enter the name of the (old) overlapping grid file:");
  }

    
  aString commandFileName="";
  if( argc>2 )
  {
    commandFileName=argv[2];
  }
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);
  
  aString logFile="move.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  cout << "Create a CompositeGrid..." << endl;
  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);

  aString nameOfShowFile = "move2.show";
  Ogshow show( nameOfShowFile );
  show.saveGeneralComment("Moving grid example");
  show.setMovingGridProblem(TRUE);
  show.setFlushFrequency(2);

  cg0.update(); // m0.update

  aString menu[]={
    "move grids",
    "choose grids to move",
    "rotate",
    "shift",
    "oscillate",
    "number of steps",
    "change the plot",
    "debug",
    "full frequency",
    "build an overlapping grid",
    "build a hybrid grid",
    "exit",
    "" 
  };
  
  enum GridOption
  {
    overlapping,
    hybrid
  } gridOption=overlapping;


  // Here is the grid generator
  Ogen gridGenerator(ps);
  Ugen *ugenPointer=NULL;   // this is the hybrid grid generator

  // gridGenerator.debug=3;
  // gridGenerator.useNewMovingUpdate=TRUE;
  
  int fullAlgorithmFrequency=1000;  // apply full algorithm every this many steps

  assert( cg0.numberOfComponentGrids()<100 );
  MatrixTransform *transform[2][100];
  
  IntegerArray isMoving(cg0.numberOfComponentGrids());
  isMoving=FALSE;
  isMoving(cg0.numberOfComponentGrids()-1)=TRUE; // by default move the last grid
  int grid;
  for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
  {
    transform[0][grid]=transform[1][grid]=NULL;
  }
  

  int numberOfSteps=11;
  real deltaAngle= 5.*Pi/180.;
  real xShift=.01, yShift=0., zShift=0.;
  Range G=cg0.numberOfComponentGrids();
  RealArray par(Range(0,20),G);
  par=0.;
  
  int debug=0;
  enum MoveOptions
  {
    rotate=0,
    shift,
    oscillate
  } moveOption;

  // by default we rotate about the origin
  moveOption = rotate;
  par(0,G)=0;  // x0  (x0,y0,z0) = point to rotate about
  par(1,G)=0;  // y0
  par(2,G)=0;  // z0
  par(3,G)=0;  // t0   (t0,t1,t2) = tangent of line to rotate about (3D)
  par(4,G)=0;  // t1
  par(5,G)=1;  // t2
  par(6,G)=.25;  // angular velocity

  psp.set(GI_TOP_LABEL,"Original grid, cg0");  // set title
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  if( cg0.numberOfDimensions()<3 )
    psp.set(GI_PLOT_INTERPOLATION_POINTS,TRUE);
  PlotIt::plot(ps,cg0,psp);

  for( ;; )
  {

    ps.getMenuItem(menu,answer,"choose an option");
    if( answer=="choose grids to move" )
    {
      aString *gridMenu = new aString [cg0.numberOfComponentGrids()+2];
      for( ;; )
      {
	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
	{
	  if( isMoving(grid) )
	    gridMenu[grid]=cg0[grid].getName()+" (on)";
	  else
	    gridMenu[grid]=cg0[grid].getName()+" (off)";
	}
	gridMenu[cg0.numberOfComponentGrids()]="done";
	gridMenu[cg0.numberOfComponentGrids()+1]="";
      
	grid=ps.getMenuItem(gridMenu,answer,"choose an option");
	if( grid>=0 && grid<cg0.numberOfComponentGrids() )
	{
	  isMoving(grid)=!isMoving(grid);
	}
	else
	{
	  break;
	}
      }
    }
    else if( answer=="shift" )
    {
      moveOption=shift;
      cout << "Enter numberOfSteps, xShift,yShift,zShift\n";
      cin >> numberOfSteps >> xShift >> yShift >> zShift;
    }
    else if( answer=="rotate" )
    {
      moveOption=rotate;
      if( cg0.numberOfDimensions()==2 )
      {
	ps.inputString(answer,"Enter the point to rotate about");
	real x0=0., y0=0.;
	if( answer!="" )
	  sScanF(answer,"%e %e",&x0,&y0);
	par(0,G)=x0;
	par(1,G)=y0;
      }
      else
      {
	real x0=0., y0=0., z0=0.;
	ps.inputString(answer,"Enter a point on the line to rotate about");
	if( answer!="" )  
	  sScanF(answer,"%e %e %e",&x0,&y0,&z0);
	par(0,G)=x0;
	par(1,G)=y0;
	par(2,G)=z0;

	x0=0.; y0=0.; z0=1.;
	ps.inputString(answer,"Enter the tangent of the line to rotate about");
	if( answer!="" )
	  sScanF(answer,"%e %e %e",&x0,&y0,&z0);
	par(3,G)=x0;
	par(4,G)=y0;
	par(5,G)=z0;
	      
      }
      real omega=1.;
      ps.inputString(answer,"Enter the number of rotations per second");
      if( answer!="" )
	sScanF(answer,"%e ",&omega);
      par(6,G)=omega;
	      
    }
    else if( answer=="oscillate" )
    {
      printf("Oscillation: x(t) = x(0) + tangent { [ 1-cos( (t-t0)*(omega *2*pi) ) ]*amplitude }\n");
      moveOption=oscillate;
      real x0=0., y0=0., z0=0.;
      ps.inputString(answer,"Enter the tangent of the line of oscillation");
      if( answer!="" )
      {
	if( cg0.numberOfDimensions()==2 )
	  sScanF(answer,"%e %e",&x0,&y0);
	else
	  sScanF(answer,"%e %e %e",&x0,&y0,&z0);
      }
      par(0,G)=x0;
      par(1,G)=y0;
      par(2,G)=z0;
	      
      real omega=1.;
      ps.inputString(answer,"Enter omega, the number of oscillations per second");
      if( answer!="" )
	sScanF(answer,"%e",&omega);
      par(3,G)=omega;

      real amplitude=.5;
      ps.inputString(answer,"Enter amplitude, the amplitude of the oscillation");
      if( answer!="" )
	sScanF(answer,"%e",&amplitude);
      par(4,G)=amplitude;

      real t0=0.;
      ps.inputString(answer,"Enter t0, the origin of the oscillation");
      if( answer!="" )
	sScanF(answer,"%e",&t0);
      par(5,G)=t0;
    }
    else if( answer=="number of steps" )
    {
      ps.inputString(answer,"Enter the number of steps");
      sScanF(answer,"%i",&numberOfSteps);
      printf(" numberOfSteps=%i\n",numberOfSteps);
    }
    else if( answer=="change the plot" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      PlotIt::plot(ps,cg0,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="debug" )
    {
      ps.inputString(answer,"Enter debug");
      sScanF(answer,"%i",&gridGenerator.debug);
      gridGenerator.info=gridGenerator.debug;
      printf(" debug=%i (also set info=debug)\n",gridGenerator.debug);
    }
    else if( answer=="full frequency" )
    {
      ps.inputString(answer,"Enter the frequency to apply the full algorithm");
      sScanF(answer,"%i",&fullAlgorithmFrequency);
      printf(" fullAlgorithmFrequency=%i\n",fullAlgorithmFrequency);     
    }
    else if( answer=="build an overlapping grid" )
    {
      gridOption=overlapping;
    }
    else if( answer=="build a hybrid grid" )
    {
      gridOption=hybrid;
    }
    else if( answer=="exit" )
    {
      break;
    }

    if( answer!="move grids" )
    {
      continue;
    }

    printf(" numberOfSteps=%i debug=%i \n",numberOfSteps,debug);

    CompositeGrid cg[2];                              // use these two grids for moving

    //  cg[0]=cg0; cg[0].update(); 
//    cg[0].reference(cg0);
    cg[0]=cg0;  // don't destroy the mask
    cg[1]=cg0; cg[1].update(); 

    
    if( gridOption==hybrid && ugenPointer==NULL )
    {
      ugenPointer=new Ugen;
    }
    

    // ---- Move the grid a bunch of times.----

    for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      if( !isMoving(grid) )
        continue;
      
      Mapping *mapPointer = cg0[grid].mapping().mapPointer;

      // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
      // can rotate/scale and shift any Mapping
      transform[0][grid]= new MatrixTransform(*mapPointer); transform[0][grid]->incrementReferenceCount();
      transform[1][grid]= new MatrixTransform(*mapPointer); transform[1][grid]->incrementReferenceCount();

      // Change the mappings that the grids point to:
      // Change the mapping of component grid 1:
      cg[0][grid].reference(*transform[0][grid]); 
      cg[1][grid].reference(*transform[1][grid]); 

      cg[0][grid].update();  // the previous reference seems to destroy the data
      cg[0][grid].mask()=cg0[grid].mask();  // copy the mask as it gets lost too
      
      cg[1][grid].update();  // the previous reference seems to destroy the data
    }
    
    //  cg[0][1].dimension().display("cg[0][1].dimension()");
    //  cg[1][1].dimension().display("cg[1][1].dimension()");
  
    // now we destroy all the data on the new grid -- it will be shared with the old grid
    // this is not necessary to do
    cg[1].destroy(CompositeGrid::EVERYTHING);  

    char buff[80];
    aString showFileTitle[2];
    real currentAngle=0.;
    
    int numberOfArrays=GET_NUMBER_OF_ARRAYS;

    for (int i=1; i<=numberOfSteps; i++) 
    {
      int newCG = i % 2;        // new grid
      int oldCG = (i+1) % 2;    // old grid

      real dt=1./(numberOfSteps+1);
      real t=i*dt;
      
      // Draw the overlapping grid

      if( moveOption==rotate )
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i, angle=%6.2e",i,currentAngle*180./Pi));  // set title
      else
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i",i));  // set title
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      ps.erase();
      PlotIt::plot(ps,cg[oldCG],psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

      ps.redraw(TRUE);   // force a redraw

      //  Rotate the grid...
      // After the first step we must double the angle since we start from the old grid
      bool firstRotatingGrid=TRUE;
      for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      {
	if( isMoving(grid) )
	{
	  if( moveOption==rotate )
	  {
	    // real angle = i==1 ? deltaAngle : deltaAngle*2.; 
	    real deltaT=i==1 ? dt : dt*2.; 
	
	    const realArray & x0 =par(Range(0,2),grid); // centre of rotation
	    const realArray & tn =par(Range(3,5),grid); // tangent to rotation axis
	    // static real deltaTheta=180.* (4.*atan(1.)/180); 

	    const real angularSpeed = deltaT*par(6,grid)*2.*Pi;
	    printf("step %i : angularSpeed=%e\n",i,angularSpeed);
	
	    // shift to centre, rotate and shift back
	    transform[newCG][grid]->shift(-x0(0),-x0(1),-x0(2));
	    if( cg0.numberOfDimensions()==2 || (tn(0)==0. && tn(1)==0. ) )
	      transform[newCG][grid]->rotate(axis3,angularSpeed);
	    else
	    {
	      throw "error";
	    }
	    transform[newCG][grid]->shift( x0(0), x0(1), x0(2));
	    if( firstRotatingGrid )
	    {
              firstRotatingGrid=FALSE;
              deltaAngle=dt*par(6,grid)*2.*Pi;
	      currentAngle+=deltaAngle;
	    }
	  }
	  else if( moveOption==shift )
	  {
	    real delta1 = i==1 ? xShift : xShift*2.; 
	    real delta2 = i==1 ? yShift : yShift*2.; 
	    real delta3 = i==1 ? zShift : zShift*2.; 
	    transform[newCG][grid]->shift(delta1,delta2,delta3 );
	  }
	  else if( moveOption==oscillate )
	  {
	    // x(t) = (1-cos([t-tOrigin]*omega/(2 pi)))*amplitude
	    const realArray & vector = par(Range(0,2),grid); // tangent
	    const real omega         = par(3,grid)*2.*Pi;    // oscillation rate
	    const real amplitude     = par(4,grid);          // amplitude
	    const real tOrigin       = par(5,grid);                  

        // compute the shift from time t=0 to avoid accumulation of round-off.
	    const real t0=0.; // this may be wrong!
	    const real deltaX=amplitude*(cos(omega*(t0-tOrigin))-cos(omega*(t-tOrigin)));

	    transform[newCG][grid]->reset();
	    transform[newCG][grid]->shift(vector(0)*deltaX,vector(1)*deltaX,vector(2)*deltaX);
	  }

	} 
      }  // for( grid
      
      
      //      Update the overlapping newCG, starting with and sharing data with oldCG.    
      if( gridOption==overlapping )
      {
	Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
	if( i % fullAlgorithmFrequency == (fullAlgorithmFrequency-1)  )
	{
	  cout << "\n +++++++++++ use full algorithm in updateOverlap +++++++++++++++ \n";
	  option=Ogen::useFullAlgorithm;
	}
	// gridGenerator.debug=7;
	gridGenerator.updateOverlap(cg[newCG], cg[oldCG], isMoving, option);
      }
      else
      {
	// build a hybrid grid
        gridGenerator.turnOnHybridHoleCutting();
        Ogen::MovingGridOption option =Ogen::useFullAlgorithm;

        // If the last component grid is an unstructured grid, we remove it
        int ncg=cg[newCG].numberOfComponentGrids();
	if( cg[newCG][ncg-1].getGridType()==MappedGrid::unstructuredGrid )
          cg[newCG].deleteGrid(ncg-1);

        gridGenerator.updateOverlap(cg[newCG], cg[oldCG], isMoving, option); 
        ugenPointer->updateHybrid(cg[newCG]);

        cg[newCG].numberOfInterpolationPoints=0;  // fix this ** interp points arrays may remain
      }
      

      // save results in a show file:
      if( debug & 4 )
      {
        RealCompositeGridFunction u(cg[newCG]);
	u=i;
	show.startFrame();
	sPrintF(buff,"Moving Example, step=%i",i);
	show.saveComment(0,buff);
	show.saveSolution(u);
 
      }

      if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
      {
	numberOfArrays=GET_NUMBER_OF_ARRAYS;
	printf("**** WARNING: number of A++ arrays has increased to = %i \n",GET_NUMBER_OF_ARRAYS);
      }

    } // end for
  
    cout << "Done! ...\n";

  } // for( ;; )

  delete ugenPointer;
  // delete [] transform;
  destructMappingList();  // cleanup Mappings allocated by getFromADataBase

  Overture::finish();          
  return 0;
}

