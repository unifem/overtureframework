// ---------------------------------------------------------------------------
//
//      **** Parallel version ****
//
// Solve the second order wave equation
//       u_tt - c^2 ( u_xx + u_yy ) =0 
// Notes:
//    (1) Solve to 2nd or fourth order. If the overlapping grid is made with the 
//        option "order of accuracy" set to "fourth order" then this will be a true
//        fourth order scheme. This program will also work with the fourth order
//        approximation even if the grid is made for second order accuracy.
//    (2) Add a fourth order artificial dissipation of the form 
//                     ad4 h^4 dt (u.xxxx).t
//        This artificial diffusion may not even be needed in most cases.
//    (3) We try to be a little careful with memory usage here. Rectangular grids will
//        be efficiently treated -- only the mask array will be built for a rectangular
//        grid, even the array of verticies will not need to be built.
//   
//
// Examples for running in parallel
//
//  mpirun -np 4 pwave -grid=cice16.order4.hdf
//  mpirun -np 4 pwave -noplot -g=cice64.order4.hdf -cmd=pwave1.cmd
//
//  mpirun -np 2 -all-local pwave
//
// ---------------------------------------------------------------------------

#include "Overture.h"
#include "Ogshow.h"  
#include "CompositeGridOperators.h"
#include "PlotStuff.h"
#include "display.h"
#include "ParallelOverlappingGridInterpolator.h"
#include "ParallelUtility.h"
#include "LoadBalancer.h"

int 
getLineFromFile( FILE *file, char s[], int lim);


bool measureCPU=TRUE;
real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU(); // return MPI_Wtime();
  else
    return 0;
}

real
getMaxTime(real time)
{
  real maxTime=0;
  MPI_Reduce(&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  return maxTime;
}


real
getDt(const real & cfl, 
      const real & a, 
      const real & b, 
      const real & nu, 
      MappedGrid & mg, 
      MappedGridOperators & op,
      const real alpha0 = -2.,
      const real beta0  = 1. );

// fourth order dissipation 2D:
#define FD4_2D(u,i1,i2,i3) \
      (    -( u(i1-2,i2,i3)+u(i1+2,i2,i3)+u(i1,i2-2,i3)+u(i1,i2+2,i3) )   \
        +4.*( u(i1-1,i2,i3)+u(i1+1,i2,i3)+u(i1,i2-1,i3)+u(i1,i2+1,i3) ) \
       -12.*u(i1,i2,i3) )

// fourth order dissipation 3D:
#define FD4_3D(u,i1,i2,i3) \
      (    -( u(i1-2,i2,i3)+u(i1+2,i2,i3)+u(i1,i2-2,i3)+u(i1,i2+2,i3)+u(i1,i2,i3-2)+u(i1,i2,i3+2) )   \
        +4.*( u(i1-1,i2,i3)+u(i1+1,i2,i3)+u(i1,i2-1,i3)+u(i1,i2+1,i3)+u(i1,i2,i3-1)+u(i1,i2,i3+1) ) \
       -18.*u(i1,i2,i3) )

#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  


  // Pulse parameters:
static real alpha=50.; // 200.;
static real pulsePow=10.; // 20
static real a0=3.;
static real xPulse=-1.2;
static real yPulse=0.;

enum InitialConditionOptionEnum
{
  smoothPulse,
  pulse
};

// Determine the time step
real
getTimeStep( CompositeGrid & cg, CompositeGridOperators & operators, real cfl, real c, real nu )
{
  real dt=REAL_MAX;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    // **this is a fake but should work ok ** 
    // we choose the time step for the equation u_t + c u_x + c u_y
    real dtGrid=getDt(cfl,c,c,nu,cg[grid],operators[grid],-1.,1.);
    dt=min(dt,dtGrid);

    dt=ParallelUtility::getMinValue(dt);  // get min value over all processors (probably not necessary)
    // printf(" dt for grid %i is %e\n",grid,dtGrid);

  }
  return dt;
}


// Assign the initial conditions
void
getInitialConditions( InitialConditionOptionEnum option, realCompositeGridFunction *u, real t, real dt,
                      real c, const bool plotOption )
{
  const int myid=Communication_Manager::My_Process_Number;
  printF("get initial conditions\n");
  
  CompositeGrid & cg = *( u[0].getCompositeGrid() );

  // Pulse parameters:
  
  Index I1,I2,I3;

// define U0(x,y,t) exp( - alpha*( SQR((x)-(xPulse-c*dt)) + SQR((y)-yPulse) ) )
// #define U0(x,y,t) exp( - alpha*( SQR((x)-(xPulse+c*(t))) ) )
// define U0(x,y,t) exp( - alpha*( pow( a0*( (x)-(xPulse+c*(t)) ),20.) ) )
#define U0(x,y,t) exp( - alpha*( pow( a0*( (x)-(xPulse+c*(t)) ),pulsePow) ) )

  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    // initial condition is a pulse, we make an approximate guess for u(-dt) 
    // u[1] = u(x,-dt) 
    // u[0] = u(x,t)

    // get the local serial arrays
    OV_GET_SERIAL_ARRAY(real,u[0][grid],u0Local);
    OV_GET_SERIAL_ARRAY(real,u[1][grid],u1Local);

    getIndex(cg[grid].dimension(),I1,I2,I3); // assign all points including ghost points.
    const bool isRectangular=cg[grid].isRectangular();

    if( option==smoothPulse )
    {

      // restrict the bounds (I1,I2,i3) to the local array bounds (including parallel ghost pts):
      const int includeGhost=1;
      bool ok=ParallelUtility::getLocalArrayBounds(u[0][grid],u0Local,I1,I2,I3,includeGhost);

      if( isRectangular )
      {
        // for a rectangular grid we avoid building the array of verticies.
        // we assign the initial conditions with C-style loops

        if( !ok ) continue;  // nothing to do on this processor

	real dx[3]={0.,0.,0.}, xab[2][3]={0.,0.,0.,0.,0.,0.};
	if( cg[grid].isRectangular() )
	  cg[grid].getRectangularGridParameters( dx, xab );

	const real xa=xab[0][0], dx0=dx[0];
	const real ya=xab[0][1], dy0=dx[1];
	const real za=xab[0][2], dz0=dx[2];

	const int i0a=cg[grid].gridIndexRange(0,0);
	const int i1a=cg[grid].gridIndexRange(0,1);
	const int i2a=cg[grid].gridIndexRange(0,2);

#define VERTEX0(i0,i1,i2) xa+dx0*(i0-i0a)
#define VERTEX1(i0,i1,i2) ya+dy0*(i1-i1a)
#define VERTEX2(i0,i1,i2) za+dz0*(i2-i2a)

        // Here we grab a pointer to the data of the array so we can index it as a C-array
        real *upm= u1Local.Array_Descriptor.Array_View_Pointer3;
        real *up = u0Local.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=u0Local.getRawDataSize(0);
        const int uDim1=u0Local.getRawDataSize(1);
        const int d1=uDim0, d2=d1*uDim1; 
#define U(i0,i1,i2) up[(i0)+(i1)*d1+(i2)*d2]
#define UM(i0,i1,i2) upm[(i0)+(i1)*d1+(i2)*d2]

        int i1,i2,i3;
	FOR_3(i1,i2,i3,I1,I2,I3) // loop over all points
	{
          UM(i1,i2,i3)=U0(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),-dt);
          U(i1,i2,i3) =U0(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),0.);
	}
	
#undef VERTEX0
#undef VERTEX1
#undef VERTEX2
#undef U
#undef UM
      }
      else
      {
	cg[grid].update(MappedGrid::THEvertex);  // build the array of vertices
	const realArray & vertex = cg[grid].vertex();

        if( !ok ) continue;  // nothing to do on this processor

        // const realSerialArray & xLocal = vertex.getLocalArrayWithGhostBoundaries();
        // display(vertex,"vertex",NULL,"%4.1f ");
        // display(xLocal,"xLocal",NULL,"%4.1f ");

// 	u[1][grid]=U0(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),-dt);
// 	u[0][grid]=U0(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),0.);

        real *upm= u1Local.Array_Descriptor.Array_View_Pointer3;
        real *up = u0Local.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=u0Local.getRawDataSize(0);
        const int uDim1=u0Local.getRawDataSize(1);
        const int d1=uDim0, d2=d1*uDim1; 
#define U(i0,i1,i2) up[(i0)+(i1)*d1+(i2)*d2]
#define UM(i0,i1,i2) upm[(i0)+(i1)*d1+(i2)*d2]

        OV_GET_SERIAL_ARRAY(real,vertex,vertexLocal);
        const real *vertexp = vertexLocal.Array_Descriptor.Array_View_Pointer3;
        const int vertexDim0=vertexLocal.getRawDataSize(0);
        const int vertexDim1=vertexLocal.getRawDataSize(1);
        const int vertexDim2=vertexLocal.getRawDataSize(2);
#define VERTEX(i0,i1,i2,i3) vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(i3)))]

        int i1,i2,i3;
	FOR_3(i1,i2,i3,I1,I2,I3) // loop over all points
	{
          UM(i1,i2,i3)=U0(VERTEX(i1,i2,i3,0),VERTEX(i1,i2,i3,1),-dt);
          U(i1,i2,i3) =U0(VERTEX(i1,i2,i3,0),VERTEX(i1,i2,i3,1),0.);
	}

      }
#undef U
#undef UM
#undef VERTEX

    }
    else
    {
      // discontinuous pulse -- this doesn't work as planned since c*dt < dx and then
      // u[1] is just the same as u[0] 
      cg[grid].update(MappedGrid::THEvertex);  // build the array of vertices
      const realArray & vertex = cg[grid].vertex();
      u[1][grid]=0.;
      where( fabs(vertex(I1,I2,I3,0)-(xPulse-c*dt))<.2 )
      {
	u[1][grid]=1.;
      }
      u[0][grid]=0.;
      where( fabs(vertex(I1,I2,I3,0)-xPulse)<.2 )
      {
	u[0][grid]=1.;
      }
    }
    if( !plotOption ) 
      cg[grid].destroy(MappedGrid::THEvertex);  // vertices are no nolonger needed.

  }

  printF("done initial conditions\n");

}



int 
main(int argc, char *argv[])
{
  int debug=0;

  Overture::start(argc,argv);  // initialize Overture
  const int myid=Communication_Manager::My_Process_Number;
  const int np = Communication_Manager::numberOfProcessors();
  
  printF("Usage: `mpirun -np N pwave [-noplot][-grid=<gridName>][-cmd=<file.cmd>]'\n");
  
  // Use this to avoid un-necessary communication: 
  Optimization_Manager::setForceVSG_Update(Off);


  // "cic.4.hdf"; // "sis2.p.order4.hdf"; // "sis2.p.hdf"; // "square10.hdf" ; // "cic.4.hdf"; //
  // "cic2.4.hdf"  // 1.1M points
  aString nameOfOGFile= "cic.4.hdf"; // "cic4.4.hdf"; // "cic3.4.hdf"; // "cic2.4.hdf"; // "sis2.p.order4.hdf"; // "cic.4.hdf"; // "cic2.4.hdf"; 

  aString commandFileName="";
  aString line;
  int len=0;
  bool plotOption=true;  // by default we plot interactively
  if( argc > 1 )
  {
    int i;
    for( i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="noplot" )
        plotOption=false; 
      else if( len=line.matches("-grid=") )
      {
	nameOfOGFile=line(len,line.length()-1);
        // printf("\n$$$$ node %i : use grid=[%s]\n",myid,(const char*)nameOfOGFile);
      }
      else if( len=line.matches("-cmd=") )
      {
	commandFileName=line(len,line.length()-1);
        // printf("\n$$$$ node %i : read command file %s\n",myid,(const char*)commandFileName);
      }
    }
  }

  
  PlotStuff ps(plotOption,"wave equation");
  PlotStuffParameters psp;

  // By default start saving the command file called "wave.cmd"
  aString logFile="pwave.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  int orderOfAccuracy=4;  // **** need two ghost lines in parallel ****

  aString nameOfShowFile="wave.show";

  // create and read in a CompositeGrid
  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    const int numGhost=2;
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numGhost);
  #endif
  CompositeGrid cg;
  bool loadBalance=true; // turn on or off the load balancer
  getFromADataBase(cg,nameOfOGFile,loadBalance);

  cg.update(MappedGrid::THEmask);
  const int numberOfDimensions = cg.numberOfDimensions();

  bool secondOrderGrid = max(cg[0].discretizationWidth())==3;
  if( secondOrderGrid )
  { 
    orderOfAccuracy=2;
    printF("This grid has been made for 2nd-order accuracy. I am switching to 2nd-order accuracy.\n");
    printF("You can explicitly change the order back to 4 to try it.\n");
     
    //     // In parallel there will be only one ghost line at parallel boundaries and thus we cannot
    //     // use fourth-order dissipation etc.
    //     orderOfAccuracy=2;  // do this for now
    //     ad4=0.;
  }
  

  // Interpolant interpolant(cg);                                 // Make an interpolant
  Interpolant & interpolant = *new Interpolant(cg);               // do this instead for now. 

  Ogshow show;
    
  printF("create CompositeGridOperators...\n");
  real timea=CPU();
  CompositeGridOperators operators(cg);                        // operators for a CompositeGrid
  real timeForCGOP=CPU()-timea;
  timeForCGOP=getMaxTime(timeForCGOP);  
  printF("Time to build CompositeGridOperators=%8.2e (s)\n",timeForCGOP);

  printF("...done create CompositeGridOperators\n");

  // const int orderOfAccuracy=2;  

  operators.setOrderOfAccuracy(orderOfAccuracy);
  BoundaryConditionParameters bcParams;

  printF("create grid functions u[] and laplacian...\n");
  Range all;
  realCompositeGridFunction u[2];
  for( int iu=0; iu<2; iu++ )
  {
    u[iu].updateToMatchGrid(cg);
    u[iu].setOperators(operators);                                 
    u[iu].setName("u");                                              // name the grid function
  }
  realCompositeGridFunction laplacian(cg);  // holds laplacian
  printF("...done create grid functions u[] and laplacian\n");
  
  real tFinal=.1; // .0125; // .1; // .5; // 2.1;
  real tPlot=.05;  // plot this often
  real tShow=.25;  // save to show file this often
  bool saveShowFile=false;
  
  real t=0;
  real c=1., cSquared=c*c;
    
  real ad4=1.;  // coeff of the artificial dissipation.
  // ad4=0.;
  

  
  Index I1,I2,I3;
  // estimate the time step -- for now approximate by a first order wave equation.
  real dt;
  real cfl=.25, nu=0.;
  
  dt=getTimeStep( cg, operators, cfl, c, nu );
  real dtSquared=dt*dt;

  printF("After getTimeStep: dt=%8.2e\n",dt);
  
  
  // Assign the initial conditions
  InitialConditionOptionEnum option=smoothPulse;
  int step=-1;
  getInitialConditions( option, u, t, dt, c, plotOption );
  bool recomputeInitialConditions=false;

  int grid;


  // Build a dialog menu for changing parameters
  GUIState gui;
  DialogData & dialog=gui;

  dialog.setWindowTitle("Wave Equation");
  dialog.setExitCommand("exit", "exit");

  dialog.setOptionMenuColumns(1);

  aString accuracyLabel[] = {"second order", "fourth order", "" };
  dialog.addOptionMenu("accuracy:", accuracyLabel, accuracyLabel, (orderOfAccuracy==2 ? 0 : 1) );

  aString initialConditionLabel[] = {"smooth pulse", "pulse", "" };
  dialog.addOptionMenu("Initial Condition:",initialConditionLabel,initialConditionLabel,(int)option );

  enum TypeOfApproximation
  {
    nonConservative=0,
    conservative=1
  } typeOfApproximation=nonConservative;
    
  aString approximationLabel[] = {"nonconservative", "conservative", "" };
  dialog.addOptionMenu("Approximation:",approximationLabel,approximationLabel,(int)typeOfApproximation );

  aString pbLabels[] = {"compute",
                        "contour",
                        "grid",
                        "erase",
                        "reset",
                        "exit",
			""};
  int numRows=2;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  aString tbCommands[] = {"save show file",
                           ""};
  int tbState[10];
  tbState[0] = saveShowFile==true;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "cfl";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",cfl);  nt++; 
  textCommands[nt] = "tFinal";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",tFinal);  nt++; 
  textCommands[nt] = "tPlot";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",tPlot);  nt++; 
  textCommands[nt] = "tShow";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",tShow);  nt++; 
  textCommands[nt] = "artificial dissipation";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",ad4);  nt++; 
  textCommands[nt] = "pulse params";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g %g %g %g %g (alpha,a0,pulsePow,x0,y0)",alpha,a0,pulsePow,xPulse,yPulse);  nt++; 
  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  

  ps.pushGUI(gui);
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  aString answer;
  char buff[200];
  int current=0;
  for(;;) 
  {
    if( recomputeInitialConditions )
    {
      dt=getTimeStep( cg, operators, cfl, c, nu );
      getInitialConditions( option, u, t, dt, c, plotOption );
      recomputeInitialConditions=false;
    }

    if( plotOption )
    {
      ps.erase();
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Wave equation, t=%5.3f",t));
      PlotIt::contour(ps,u[current%2],psp);
    }
    
    ps.getAnswer(answer,"");      
    if( answer=="exit" || answer=="continue" )
    {
      break;
    }
    else if( answer.matches("erase") )
    {
      ps.erase();
    }
    else if( answer.matches("reset") )
    {
      t=0.;
      step=-1;
      recomputeInitialConditions=true;
    }
    else if( answer.matches("contour") )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Wave equation, t=%5.3f",t));
      PlotIt::contour(ps,u[current%2],psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer.matches("grid") )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(ps,cg,psp);                          // plot the grid
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( len=answer.matches("cfl") )
    {
      real cflOld=cfl;

      sScanF(answer(len,answer.length()-1),"%e",&cfl);
      cout << " cfl=" << cfl << endl;
      dialog.setTextLabel("cfl",sPrintF(line, "%g",cfl));

      dt=dt*cfl/cflOld;
      recomputeInitialConditions=true;
    }
    else if( len=answer.matches("tFinal") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&tFinal);
      if( myid==0 ){ cout << " tFinal=" << tFinal << endl; }
      
      dialog.setTextLabel("tFinal",sPrintF(line, "%g",tFinal));
    }
    else if( len=answer.matches("tPlot") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&tPlot);
      if( myid==0 ){ cout << " tPlot=" << tPlot << endl; }
      dialog.setTextLabel("tPlot",sPrintF(line, "%g",tPlot));
    }
    else if( len=answer.matches("tShow") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&tShow);
      if( myid==0 ){ cout << " tShow=" << tShow << endl; }
      dialog.setTextLabel("tShow",sPrintF(line, "%g",tShow));
    }
    else if( len=answer.matches("artificial dissipation") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&ad4);
      if( myid==0 ){ cout << " artificial diffusion=" << ad4 << endl; }
      dialog.setTextLabel("artificial dissipation",sPrintF(line, "%g",ad4));
    }
    else if( len=answer.matches("save show file") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); saveShowFile=value;
      dialog.setToggleState("save show file",saveShowFile==true);
      if( myid==0 )
      {
      if( saveShowFile )
        printf("Save show file %s \n",(const char*)nameOfShowFile);
      else
        printf("No show file will be saved.\n");
      }
    }
    else if( answer=="second order" || answer=="fourth order" )
    {
      orderOfAccuracy= answer=="second order" ? 2 : 4;
      dialog.getOptionMenu("accuracy:").setCurrentChoice((orderOfAccuracy==2 ? 0 : 1));
      operators.setOrderOfAccuracy(orderOfAccuracy);  
    }
    else if( answer=="smooth pulse" || answer=="pulse" )
    {
      option = answer=="smooth pulse" ? smoothPulse : pulse;
      getInitialConditions( option, u, t, dt, c, plotOption );
      dialog.getOptionMenu("Initial Condition:").setCurrentChoice((int)option);
      recomputeInitialConditions=true;
    }
    else if( len=answer.matches("pulse params") )
    {
      if( myid==0) printf("Smooth pulse:  U0 = exp( - alpha*( pow( a0*( (x)-(xPulse+c*(t)) ),pulsePow) ) )\n");
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e",&alpha,&a0,&pulsePow,&xPulse,&yPulse);
      dialog.setTextLabel("pulse params",sPrintF(line, "%g %g %g %g %g (alpha,a0,pulsePow)",
                          alpha,a0,pulsePow,xPulse,yPulse));
      if( myid==0) printf(" alpha=%g a0=%g pulsePow=%g xPulse=%g yPulse=%g \n",alpha,a0,pulsePow,xPulse,yPulse);
      recomputeInitialConditions=true;
    }
    else if( answer=="conservative" || answer=="nonConservative" )
    {
      typeOfApproximation=answer=="conservative" ? conservative : nonConservative;      
      operators.useConservativeApproximations(typeOfApproximation==conservative);
      dialog.getOptionMenu("Approximation:").setCurrentChoice((int)typeOfApproximation);
      if( typeOfApproximation==conservative ) 
        printf("Using a conservative difference approximation\n");
      else
        printf("Using a non-conservative difference approximation\n");
    }
    else if( answer=="compute" )
    {
      // *************************************************************
      // *************** Solve the Equations *************************
      // *************************************************************


      if( saveShowFile )
      {
        bool useStreamMode=true;  // true means save a compressed file (smaller but less portable)
        show.open( nameOfShowFile,".",useStreamMode );                               // create a show file
        show.saveGeneralComment("Wave equation");
        show.setFlushFrequency(2);                                 
      }
      
      int numberOfTimeSteps=int( (tFinal-t)/dt+.5);
      if( myid==0 )
       printf(" numberOfTimeSteps=%i\n",numberOfTimeSteps);
      if( numberOfTimeSteps<=0 )
      {
	if( myid==0 )
          printf("*** Increase tFinal if you want to take more steps, or `reset' to time 0\n");
        continue;
      }

      const int plotSteps = (int)max(1.,tPlot/dt+.5);
      const int showSteps = (int)max(1.,tShow/dt+.5);
      real time0=CPU(), timeb;

      real timeForLaplace=0, timeForBoundaryConditions=0., timeForUpdateGhostBoundaries=0.,
	timeForInterpolate=0., timeForAdvance=0., timeForGetLocalArray=0.,
        timeForFinishBoundaryConditions=0.;
      
      int i1,i2,i3;

      for( int i=0; i<numberOfTimeSteps; i++ )                    // take some time steps
      {
        step++;
	realCompositeGridFunction & u1 = u[step % 2];
	realCompositeGridFunction & u2 = u[(step+1) %2];
    
	if( i!=0 && (i % plotSteps) == 0 )  // plot solution every 'plotSteps' steps
	{
          printF("completed step %i, t=%8.2e (cpu =%8.2e)\n",i,t,CPU()-time0);
	  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
  	  psp.set(GI_TOP_LABEL,sPrintF(buff,"Wave equation, t=%5.3f (order=%i)",t,orderOfAccuracy));
  	  ps.erase();
  	  PlotIt::contour(ps,u1,psp);
  	  ps.redraw(true);
          // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

	}
	if( saveShowFile && (i % showSteps == 0) )  // save solution every 'showSteps' steps
	{
	  show.startFrame();                                         // start a new frame
	  show.saveComment(0,sPrintF(buff,"Wave equation"));
	  show.saveComment(1,sPrintF(buff,"t=%5.2f c=%3.1f ad4=%3.1f",t,c,ad4));
	  show.saveSolution( u1 );                                        // save the current grid function
	}


	// advance the solution   u_tt = c^2 laplacian(u) + artificial dissipation
	// This next line could be used instead for part of the loop below (high level but slower)
	// u2=2.*u1-u2  + (dtSquared*cSquared)*u1.laplacian(); 
    
	// Here is a more efficient method
	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
          if( debug & 2 ) printf("Advance grid %i, step=%i...\n",grid,step);
	  

	  realArray & u1g = u1[grid];
	  realArray & u2g = u2[grid];
	  realArray & lap = laplacian[grid];
          // evaluate laplacian(u1) and save in lap

          if( debug & 2 ) printf("pwave: compute laplacian...\n");
          timea=CPU();
	  operators[grid].derivative(MappedGridOperators::laplacianOperator,u1g,lap);
          timeForLaplace+=CPU()-timea;
	  
          if( debug & 2 ) printf("pwave: ...done laplacian\n");

      
          // The next line could also be used for part of the loop below but is slightly
          // less efficient and uses a temporary array the size of 1 grid function.
	  // u2g=2.*u1g-u2g  + (dtSquared*cSquared)*lap; // this version uses A++ statements


          timea=CPU();
          // These next macros get the local serial array on this processor
	  OV_GET_SERIAL_ARRAY(real,u1g,u1gLocal);
	  OV_GET_SERIAL_ARRAY(real,u2g,u2gLocal);
	  OV_GET_SERIAL_ARRAY(real,lap,lapLocal);
	  

          timeForGetLocalArray+=CPU()-timea;
	  timea=CPU();

          // Here we grab a pointer to the data of the array so we can index it as a C-array
	  real *u1gp= u1gLocal.Array_Descriptor.Array_View_Pointer3;
	  real *u2gp= u2gLocal.Array_Descriptor.Array_View_Pointer3;
	  real *lapp= lapLocal.Array_Descriptor.Array_View_Pointer3;
	  const int uDim0=u1gLocal.getRawDataSize(0);
	  const int uDim1=u1gLocal.getRawDataSize(1);
	  const int d1=uDim0, d2=d1*uDim1; 
#define U1G(i1,i2,i3) u1gp[(i1)+(i2)*d1+(i3)*d2]
#define U2G(i1,i2,i3) u2gp[(i1)+(i2)*d1+(i3)*d2]

#define LAP(i1,i2,i3) lapp[(i1)+(i2)*d1+(i3)*d2]

          real cdtsq=dtSquared*cSquared;
	  real ad4dt=ad4*dt;
          getIndex(cg[grid].gridIndexRange(),I1,I2,I3);

          // restrict the bounds (I1,I2,i3) to the local array bounds:
          bool ok=ParallelUtility::getLocalArrayBounds(u1g,u1gLocal,I1,I2,I3);
	  if( ok ) // there are points on this processor
	  {
	    if( ad4>0. )
	    {
	      if( cg.numberOfDimensions()==2 )
	      {
		FOR_3(i1,i2,i3,I1,I2,I3) // loop over all points
		{
		  // add a 'fourth' order dissipation  ad4 h^4 dt (u.xxxx).t to (c*dt)^2*laplacian(u)
		  LAP(i1,i2,i3)=cdtsq*LAP(i1,i2,i3)+ad4dt*( FD4_2D(U1G,i1,i2,i3)-FD4_2D(U2G,i1,i2,i3) );
		}
	      }
	      else
	      {
		FOR_3(i1,i2,i3,I1,I2,I3) // loop over all points
		{
		  // add a 'fourth' order dissipation  ad4 h^4 dt (u.xxxx).t to (c*dt)^2*laplacian(u)
		  LAP(i1,i2,i3)=cdtsq*LAP(i1,i2,i3)+ad4dt*( FD4_3D(U1G,i1,i2,i3)-FD4_3D(U2G,i1,i2,i3) );
		}
	      }
	    }
	    else
	    {
	      FOR_3(i1,i2,i3,I1,I2,I3) // loop over all points
	      {
		LAP(i1,i2,i3)=cdtsq*LAP(i1,i2,i3);
	      }
	    }
	  
	    FOR_3(i1,i2,i3,I1,I2,I3) 
	    {
	      U2G(i1,i2,i3)=2.*U1G(i1,i2,i3)-U2G(i1,i2,i3)  + LAP(i1,i2,i3);
            
	    }
	  }
   	  timeForAdvance+=CPU()-timea;


          timea=CPU();
          u2[grid].updateGhostBoundaries();
          timeForUpdateGhostBoundaries+=CPU()-timea;
	}  // end for grid
	
	t+=dt;

        if( debug & 2 ) printf("...done advance, now interpolate...\n");

	
        // *** Note: interpolate also does a periodic update **** 
	timea=CPU();

	u2.interpolate();

	timeForInterpolate+=CPU()-timea;
	if( debug & 2 ) printf("...done interpolate, now apply BC's...\n");

	BCTypes::BCNames boundaryCondition=BCTypes::evenSymmetry; 
	// BCTypes::BCNames boundaryCondition=BCTypes::dirichlet;

        timea=CPU();
	if( boundaryCondition==BCTypes::evenSymmetry )  
	{
          // apply a symmetry BC
          if( orderOfAccuracy==4 || ad4!=0. )
	  {
            for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	    {
	      MappedGrid & mg = cg[grid];
              const IntegerArray & dimension = mg.dimension();
	      
	      int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
	    
              realArray & u2g = u2[grid];
              OV_GET_SERIAL_ARRAY(real,u2g,u2gLocal);
	      real *u2gp= u2gLocal.Array_Descriptor.Array_View_Pointer3;
	      const int uDim0=u2gLocal.getRawDataSize(0);
	      const int uDim1=u2gLocal.getRawDataSize(1);
	      const int d1=uDim0, d2=d1*uDim1; 

	      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	      {
		for( int side=0; side<=1; side++ )
		{
		  if( mg.boundaryCondition(side,axis)>0 && (
                     (side==0 && u2gLocal.getBase(axis) <=dimension(0,axis) ) ||
                     (side==1 && u2gLocal.getBound(axis)>=dimension(1,axis) ) ) )
		  {
		    getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
		    I1 = Range(max(I1.getBase(),u2gLocal.getBase(0)),min(I1.getBound(),u2gLocal.getBound(0)));
		    I2 = Range(max(I2.getBase(),u2gLocal.getBase(1)),min(I2.getBound(),u2gLocal.getBound(1)));
		    I3 = Range(max(I3.getBase(),u2gLocal.getBase(2)),min(I3.getBound(),u2gLocal.getBound(2)));

		    is1=is2=is3=0;
		    isv[axis]=1-2*side;
		    FOR_3(i1,i2,i3,I1,I2,I3) 
		    {
		      U2G(i1-  is1,i2-  is2,i3-  is3)=U2G(i1+  is1,i2+  is2,i3+  is3);
		      U2G(i1-2*is1,i2-2*is2,i3-2*is3)=U2G(i1+2*is1,i2+2*is2,i3+2*is3);
            
		    }

		  }
		}
	      }
	    }
	    
	  }
	  else // non-opt verions
	  {
	    u2.applyBoundaryCondition(0,BCTypes::evenSymmetry,BCTypes::allBoundaries,0.);

	    if( orderOfAccuracy==4 || ad4!=0. )
	    {
	      bcParams.ghostLineToAssign=2;
	      u2.applyBoundaryCondition(0,BCTypes::evenSymmetry,BCTypes::allBoundaries,0.,t,bcParams);
	    }
	  }
	  
          if( debug & 2 ) printf("...done evenSymmetry\n");
	}
	else if( boundaryCondition==BCTypes::dirichlet )
	{
          if( debug & 2 ) printf("start dirichlet...\n");
	  u2.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
          if( debug & 2 ) printf("...done dirichlet\n");
	  if( orderOfAccuracy==4 || ad4!=0. )
	  {
            if( debug & 2 ) printf("start extrapolate...\n");
	    u2.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.); // for 4th order
            if( debug & 2 ) printf("...done extrapolate\n");
	  }
	}
	else if( boundaryCondition==BCTypes::neumann )     
	{
	  assert( orderOfAccuracy==2 );
	  u2.applyBoundaryCondition(0,BCTypes::neumann,BCTypes::allBoundaries,0.);  // not implemented for 4th order
	}

	if( boundaryCondition!=BCTypes::evenSymmetry && (orderOfAccuracy==4 || ad4!=0.) )
	{ // extrapolate 2nd ghostline for 4th order when the grid only supports second order
	  bcParams.ghostLineToAssign=2;
	  bcParams.orderOfExtrapolation=4;
	  u2.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,bcParams);
          if( secondOrderGrid )
	  {
	    // also extrapolate unused points next to interpolation points -- this allows us to
	    // avoid making a grid with 2 lines of interpolation.
	    u2.applyBoundaryCondition(0,BCTypes::extrapolateInterpolationNeighbours);
	  }
	}

        if( debug & 2 ) printf("...BC's, now finishBoundaryConditions...\n");


        timeForBoundaryConditions+=CPU()-timea;
        timea=CPU();

	u2.finishBoundaryConditions();
	timeForFinishBoundaryConditions+=CPU()-timea;

        timea=CPU();
        for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  u2[grid].updateGhostBoundaries();
        timeForUpdateGhostBoundaries+=CPU()-timea;


      } // for i .. number of steps


      real totalTime=CPU()-time0;
      totalTime=getMaxTime(totalTime);
      timeForAdvance=getMaxTime(timeForAdvance);
      timeForGetLocalArray=getMaxTime(timeForGetLocalArray);
      timeForLaplace=getMaxTime(timeForLaplace);
      timeForBoundaryConditions=getMaxTime(timeForBoundaryConditions);
      timeForFinishBoundaryConditions=getMaxTime(timeForFinishBoundaryConditions);
      timeForUpdateGhostBoundaries=getMaxTime(timeForUpdateGhostBoundaries);
      timeForInterpolate=getMaxTime(timeForInterpolate);
	

      real sum=timeForAdvance+timeForGetLocalArray+timeForLaplace+
	timeForBoundaryConditions+timeForUpdateGhostBoundaries+timeForInterpolate;

      printF("\n"
	     " ================== number of processors =%i =======================================\n"
	     "     t=%8.2e, %i steps, %i grid-points \n"
	     " advance..........................%7.2f (%6.2f%%)\n"
	     " laplace..........................%7.2f (%6.2f%%)\n"
	     " get local array..................%7.2f (%6.2f%%)\n"
	     " boundary conditions..............%7.2f (%6.2f%%)\n"
	     " boundary finish BC...............%7.2f (%6.2f%%)\n"
	     " update ghost boundaries..........%7.2f (%6.2f%%)\n"
	     " interpolate......................%7.2f (%6.2f%%)\n"
	     " sum of above.....................%7.2f (%6.2f%%)\n"
	     " total............................%7.2f (%6.2f%%)\n"
	     ,np,t,numberOfTimeSteps,cg.numberOfGridPoints(),
	     timeForAdvance,timeForAdvance/totalTime*100.,
	     timeForLaplace,timeForLaplace/totalTime*100.,
	     timeForGetLocalArray,timeForGetLocalArray/totalTime*100.,
	     timeForBoundaryConditions,timeForBoundaryConditions/totalTime*100.,
	     timeForFinishBoundaryConditions,timeForFinishBoundaryConditions/totalTime*100.,
	     timeForUpdateGhostBoundaries,timeForUpdateGhostBoundaries/totalTime*100.,
	     timeForInterpolate,timeForInterpolate/totalTime*100.,
	     sum,sum/totalTime*100.,
	     totalTime,totalTime/totalTime*100.);
      
      current=(numberOfTimeSteps-1)%2;
    }
    else
    {
      printF("Unknown command = [%s]\n",(const char*)answer);
      ps.stopReadingCommandFile();
       
    }
  }

  if( debug & 4 )
  {
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      printF("\n\n=====================grid %i====================================\n",grid);
      const intArray & mask = cg[grid].mask();
      const intSerialArray & maskLocal = mask.getLocalArray();
      Communication_Manager::Sync();

      printf("node %i: mask dimensions: local=[%4i,%4i][%4i,%4i][%4i,%4i] global=[%4i,%4i][%4i,%4i][%4i,%4i]\n",myid,
//            maskLocal.getBase(0),maskLocal.getBound(0),
//            maskLocal.getBase(1),maskLocal.getBound(1),
//            maskLocal.getBase(2),maskLocal.getBound(2),
	     mask.getLocalBase(0),mask.getLocalBound(0),
	     mask.getLocalBase(1),mask.getLocalBound(1),
	     mask.getLocalBase(2),mask.getLocalBound(2),
	     mask.getBase(0),mask.getBound(0),
	     mask.getBase(1),mask.getBound(1),
	     mask.getBase(2),mask.getBound(2));

      Communication_Manager::Sync();
    
      if( debug & 4 )
      {
	printF("\n\n***************partition grid %i*********************************\n",grid);
      }
    
      Communication_Manager::Sync();

      if( debug & 4 )
	cg[grid].getPartition().display(sPrintF("grid %i : partition",grid));
    }
  }
  
//    if( false )
//    {
//      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//        cg[grid].displayComputedGeometry();
//    }
  ps.popGUI();  // pop dialog

  if( Communication_Manager::My_Process_Number==0 )
  {
    printf("pwave: finish:\n");
    printf(" P++: number of messages sent=%i\n",Diagnostic_Manager::getNumberOfMessagesSent());
    printf(" P++: number of messages received=%i\n",Diagnostic_Manager::getNumberOfMessagesReceived());

    // ** u.displayPartitioning();
  }

  Overture::finish();          
  return 0;
}
