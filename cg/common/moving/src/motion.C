// ====================================================================================
//  Test out the MatrixMotion class that can be used to create rigid body motions
//
//  
//  Examples:
//     motion motion1.cmd
//     motion motion2.cmd
// ===================================================================================

#include "Overture.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "CrossSectionMapping.h"
#include "CylinderMapping.h"
#include "display.h"
#include "MappingInformation.h"
#include "MappedGrid.h"


#include "TimeFunction.h"
#include "MatrixMotion.h"


// ===================================================================================
/// \brief Compute the grid position, velocity or acceleration
/// 
/// \param u (output) : holds the grid points (option=0), grid velocity (option=1) or 
///               grid acceleration (option=2) at time t
/// \param option : compute grid points (option=0), grid velocity (option=1) or 
///               grid acceleration (option=2) 
/// ==================================================================================
int
getGridMotion( real t, MatrixTransform & transform, MatrixMotion & matrixMotion, 
	       realArray & u, int option=0 )
{


  RealArray rMatrix(4,4), rt(4,4);
  rMatrix=0.;
  
  // compute matrix R, and R' or R''
  int derivative= option;  
  matrixMotion.getMotion( t, rMatrix, rt, derivative);
  transform.reset();
  transform.rotate( rMatrix );  // this is NOT incremental by default
  transform.shift( rMatrix(0,3),rMatrix(1,3),rMatrix(2,3));

  MappedGrid mg(transform);
  mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );

  realArray & vertex = mg.vertex();
  
  Index I1,I2,I3;
  getIndex(mg.dimension(),I1,I2,I3);
  
  const int numberOfDimensions = mg.numberOfDimensions();

  u.redim(I1,I2,I3,numberOfDimensions);
  if( option==0 )
  {
    u=vertex;  // save grid points 
    return 0;
  }

  RealArray ri(4,4), rpri(4,4);
  // ri = inverse of rMatrix
  MatrixMapping::matrixInversion( ri,rMatrix );
  // rpri = rt*ri : 
  MatrixMapping::matrixMatrixProduct( rpri, rt, ri );
 
  // x = R(t)*x(0) + g(t)
  // v = R'(t)*x(0) + g'(t)
  //   = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)
  if( numberOfDimensions==3 )
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      // velocity    = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)
      // acceleration= R''(t)*R^{-1}( x(t)-g(t) ) + g''(t)

      // Here is the velocity (option==1) or acceleration (option=2)
      u(I1,I2,I3,axis) = ( rpri(axis,0)*(vertex(I1,I2,I3,0) - rMatrix(0,3))+
			   rpri(axis,1)*(vertex(I1,I2,I3,1) - rMatrix(1,3))+
			   rpri(axis,2)*(vertex(I1,I2,I3,2) - rMatrix(2,3))) + rt(axis,3);
    }
  }
  else
  {

    OV_ABORT("finish me");
  }
  
  return 0;

}

// ===========================================================================
// Choose appropriate fixed plot bounds so that the bounds don't change
// as the body rotates.
// ===========================================================================
int 
updatePlotBounds(RealArray & xBound, Mapping & map , GraphicsParameters & gip )
{
  
  Range all;
  real xMin=min(xBound(0,all)), xMax=max(xBound(1,all));
  for( int axis=0; axis<map.getRangeDimension(); axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      Bound b = map.getRangeBound(side,axis);
      if( b.isFinite() )
      {
	xMin=min(xMin,(real)b);
	xMax=max(xMax,(real)b);
      }
    }
  }
  printf("Set plot bounds using : xMin=%e xMax=%e \n",xMin,xMax);
  
  xBound(Start,Range(0,2))=xMin;
  xBound(End  ,Range(0,2))=xMax;
  
  gip.set(GI_USE_PLOT_BOUNDS,true);  // use the region defined by the plot bounds
  gip.set(GI_PLOT_BOUNDS,xBound); // set plot bounds

  return 0;
}


int 
main(int argc, char *argv[]) 
{
  Mapping::debug=0;

  Overture::start(argc,argv);  // initialize Overture and A++/P++

  char buff[180];
  int plotOption=true;
  
  printF("Usage:  motion [file.cmd] \n");

  aString commandFileName="";

  int len=0;
  if( argc > 1 )
  { // look at arguments for "-noplot"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
        plotOption=false;
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printf("motion: reading commands from file [%s]\n",(const char*)commandFileName);
      }

    }
  }

  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface("motion",plotOption,argc,argv);

  // By default start saving the command file called "motion.cmd"
  aString logFile="motion.cmd";
  gi.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  gi.appendToTheDefaultPrompt("motion>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }


  PlotStuffParameters gip;
    
  gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  gip.set(GI_TOP_LABEL,"Original mapping");  // set title
  
  CrossSectionMapping map;
  if( plotOption )
    PlotIt::plot(gi,map,gip);

  Range all;
  RealArray xBound(2,3);
  xBound(0,all)=REAL_MAX;
  xBound(1,all)=REAL_MIN;

  updatePlotBounds( xBound,map, gip );


  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform transform(map);

  MatrixMotion & matrixMotion = *new MatrixMotion; matrixMotion.incrementReferenceCount();
  
  const int maxNumberOfBodies=100;
  int numberOfBodies=1;
  Mapping **mapList = new Mapping * [maxNumberOfBodies];
  mapList[0]=&transform;
  MatrixMotion **matrixMotionList = new MatrixMotion * [maxNumberOfBodies];
  for( int i=0; i<maxNumberOfBodies; i++ )
    matrixMotionList[i]=NULL;
  matrixMotionList[0]=&matrixMotion;

  
  // We put the Mapping's for the bodies into a MappingInformation
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  mapInfo.mappingList.addElement(*mapList[0]);


  real tFinal=1., dt=.1; 

  RealArray rMatrix(4,4);
  rMatrix=0.;

//   matrixMotion.getMotion( t, rMatrix );
//   ::display(rMatrix,"Rotation matrix at t=0");
//   t=.25;
//   matrixMotion.getMotion( t, rMatrix );
//   ::display(rMatrix,"Rotation matrix at t=.25");


  int numberOfDimensions=map.getRangeDimension();

  bool checkDerivatives=false;
  bool checkGridVelocity=false;
  
  GUIState dialog;
  bool buildDialog=true;
  if( buildDialog )
  {
    dialog.setWindowTitle("Motion");
    dialog.setExitCommand("exit", "exit");

    aString cmds[] = {"move",
                      "move and stop",
                      "add cross-section",
                      "add cylinder",
		      ""};

    int numberOfPushButtons=3;  // number of entries in cmds
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    aString tbCommands[] = {"check derivatives",
                            "check grid velocity",
			    ""};
    int tbState[10];
    tbState[0] = checkDerivatives;
    tbState[1] = checkGridVelocity;
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

    const int numberOfTextStrings=7;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "final time:";  sPrintF(textStrings[nt],"%g",tFinal);  nt++; 
    textLabels[nt] = "time step:";  sPrintF(textStrings[nt],"%g",dt);  nt++; 
    textLabels[nt] = "edit motion for body:";  sPrintF(textStrings[nt],"%i",-1);  nt++; 
    textLabels[nt] = "compose motion for body:";  sPrintF(textStrings[nt],"%i",-1);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

    // dialog.buildPopup(menu);
    gi.pushGUI(dialog);
  }
  

  aString answer;

  int bodyToEdit=-1;
  for(;;)
  {
    bool move=false;
    
    gi.getAnswer(answer,"");
    if( answer=="exit" )
      break;
  
    if( dialog.getTextValue(answer,"edit motion for body:","%i",bodyToEdit) )
    {
      if( bodyToEdit>=0 && bodyToEdit<numberOfBodies )
      {
	printF("Edit motion for body %i\n",bodyToEdit);
        // interactively change parameters:
	matrixMotionList[bodyToEdit]->update(gi);
      }
      else
      {
	printF("ERROR: invalid body=%i, there are %i bodies\n",bodyToEdit,numberOfBodies);
      }
      continue;
    }
    else if( dialog.getTextValue(answer,"compose motion for body:","%i",bodyToEdit) )
    {
      if( bodyToEdit>=0 && bodyToEdit<numberOfBodies )
      {
	printF("Compose the motion for body %i\n",bodyToEdit);
        // interactively change parameters:
        MatrixMotion & matrixMotion = *new MatrixMotion; matrixMotion.incrementReferenceCount();
	matrixMotion.compose(matrixMotionList[bodyToEdit]);

        // replace with the new MatrixMotion:
	matrixMotionList[bodyToEdit] = &matrixMotion;
	matrixMotionList[bodyToEdit]->update(gi);
      }
      else
      {
	printF("ERROR: invalid body=%i, there are %i bodies\n",bodyToEdit,numberOfBodies);
      }
      continue;
    }
    else if( dialog.getToggleValue(answer,"check derivatives",checkDerivatives) ){}  // 
    else if( dialog.getToggleValue(answer,"check grid velocity",checkGridVelocity) ){}  // 
    else if( dialog.getTextValue(answer,"final time:","%e",tFinal) ){ continue;} // 
    else if( dialog.getTextValue(answer,"time step:","%e",dt) ){ continue;}
    else if( answer=="move and stop" )
    {
      gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      move=true;
    }
    else if( answer=="move" )
    {
      gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      move=true;
    }
    else if( answer=="add cylinder" )
    {
      CylinderMapping & map = *new CylinderMapping; // we don't worry about memory leaks in this code
      map.update(mapInfo);

      MatrixTransform & transform = *new MatrixTransform(map);
      
      mapList[numberOfBodies] = &transform; 
      matrixMotionList[numberOfBodies] = new MatrixMotion;
      
      mapInfo.mappingList.addElement(*mapList[numberOfBodies]);

      numberOfBodies++;
      assert( numberOfBodies<maxNumberOfBodies );
      updatePlotBounds( xBound,map, gip );
    }
    else if( answer=="add cross-section" )
    {
      CrossSectionMapping & map = *new CrossSectionMapping; // we don't worry about memory leaks in this code
      map.update(mapInfo);

      MatrixTransform & transform = *new MatrixTransform(map);
      
      mapList[numberOfBodies] = &transform; 
      matrixMotionList[numberOfBodies] = new MatrixMotion;
      
      mapInfo.mappingList.addElement(*mapList[numberOfBodies]);

      numberOfBodies++;
      assert( numberOfBodies<maxNumberOfBodies );
      updatePlotBounds( xBound,map, gip );
    }
    else
    {
      printf("Unknown answer=[%s]\n",(const char*)answer);
      continue;
    }
    
    
    // body.setInitialConditions(t,xCM,vCM,w,e0);
    transform.reset();
    
    int numberOfSteps=int(tFinal/dt+.5); 
    if( !move ) numberOfSteps=0;
    
    real t=0.;
    for (int i=0; i<=numberOfSteps; i++) 
    {
      
      gip.set(GI_TOP_LABEL,sPrintF(buff,"Mapping at step %i, t=%9.3e",i+1,t));
      gi.erase();

      // --- move all bodies ---
      for( int b=0; b<numberOfBodies; b++ )
      {
	
        assert( mapList[b]!=NULL );
	MatrixTransform & transform = *((MatrixTransform*)mapList[b]);
        MatrixMotion & matrixMotion = *matrixMotionList[b];
	

	matrixMotion.getMotion( t, rMatrix );
      

//      transform.shift(-xCM(0),-xCM(1),-xCM(2));

	transform.reset();
	transform.rotate( rMatrix );  // this is NOT incremental by default

	transform.shift( rMatrix(0,3),rMatrix(1,3),rMatrix(2,3));
        // ::display(rMatrix,sPrintF("Rotation matrix at t=%9.3e",t));

	if( checkDerivatives )
	{
	  RealArray r(3,4), rt(3,4);

          int derivative=1;
          matrixMotion.getMotion( t, r, rt, derivative );

          // compute the first derivative by differences
	  RealArray r1(3,4), r2(3,4), rd(3,4);
          // optimal h from trucation and round off:   eps/h = h^2 (first derivative) or e/h^2 = h^2 for second
          const real h = pow(REAL_EPSILON,1./3.);  
	  real t1=t-h, t2=t+h;
          matrixMotion.getMotion( t1, r1 );
          matrixMotion.getMotion( t2, r2 );
	  rd=(r2-r1)/(2.*h);
          real norm1 = max( REAL_MIN*100., max(fabs(rt)), max(fabs(rd)) );
	  real maxErr1 = max(fabs(rt-rd))/norm1;

          derivative=2;
          matrixMotion.getMotion( t, r, rt, derivative );
          rd=(r2-2.*r+r1)/(h*h);
          real norm2 = max( REAL_MIN*100., max(fabs(rt)), max(fabs(rd)) );
	  real maxErr2 = max(fabs(rt-rd))/norm2;

	  printF(" body b=%i, t=%9.3e, max relative err in derivatives of motion = %8.2e (1st) and =%8.2e (second)\n",
             b,t,maxErr1,maxErr2);

	  if( maxErr2>.1 )
	  {
	    ::display(rt,"2nd derivative from matrixMotion.getMotion");
	    ::display(rd,"2nd derivative from difference");
	  }
	  
	}
	if( checkGridVelocity )
	{
          realArray gridVelocity,acceleration;
	  getGridMotion( t, transform, matrixMotion, gridVelocity, 1 );
	  getGridMotion( t, transform, matrixMotion, acceleration, 2 );

          // compute the grid velocity and acceleration by differences
          const real h = pow(REAL_EPSILON,1./3.);  
	  real t1=t-h, t2=t+h;

          realArray x0,x1,x2, gv,ac;
          getGridMotion( t , transform, matrixMotion, x0, 0 );
          getGridMotion( t1, transform, matrixMotion, x1, 0 );
          getGridMotion( t2, transform, matrixMotion, x2, 0 );

	  gv=(x2-x1)/(2.*h);
          real norm1 = max( REAL_MIN*100., max(fabs(gridVelocity)), max(fabs(gv)) );
	  real maxErr1 = max(fabs(gv-gridVelocity))/norm1;

          ac=(x2-2.*x0+x1)/(h*h);
          real norm2 = max( REAL_MIN*100., max(fabs(ac)), max(fabs(acceleration)) );
	  real maxErr2 = max(fabs(ac-acceleration))/norm2;

	  printF(" body b=%i, t=%9.3e, max relative err = %8.2e (grid-velocity) and =%8.2e (grid-acceleration)\n",
		 b,t,maxErr1,maxErr2);


	}
	
   


	gip.set(GI_MAPPING_COLOUR,gi.getColourName(b));
	PlotIt::plot(gi,transform,gip);

      }
      gi.redraw(true);   // force a redraw

      t+=dt;

      // body.getAcceleration(t,aCM);
      

    } // end for (i )  (time steps)


  } // for(;;)
  
  gi.unAppendTheDefaultPrompt();  // reset prompt

  // clean up
  for( int i=0; i<maxNumberOfBodies; i++ )
  {
    if( matrixMotionList[i]!=NULL && matrixMotionList[i]->decrementReferenceCount()==0 )
    {
      delete  matrixMotionList[i];
    }
  }
  delete [] matrixMotionList;


  Overture::finish(); 
  return 0;
}
