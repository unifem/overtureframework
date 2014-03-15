// ---------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------

#include "Overture.h"
#include "Ogshow.h"  
#include "CompositeGridOperators.h"
#include "PlotStuff.h"

real
getDt(const real & cfl, 
      const real & a, 
      const real & b, 
      const real & c, 
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
    real dtGrid=getDt(cfl,c,c,c,nu,cg[grid],operators[grid],-1.,1.);
    dt=min(dt,dtGrid);
    printf(" dt for grid %i is %e\n",grid,dtGrid);

  }
  return dt;
}


// Assign the initial conditions
void
getInitialConditions( InitialConditionOptionEnum option, realCompositeGridFunction *u, real t, real dt,
                      real c )
{
  printf("get initial conditions\n");
  
  CompositeGrid & cg = *( u[0].getCompositeGrid() );

  // Pulse parameters:
  
  Index I1,I2,I3;

// define U0(x,y,t) exp( - alpha*( SQR((x)-(xPulse-c*dt)) + SQR((y)-yPulse) ) )
// #define U0(x,y,t) exp( - alpha*( SQR((x)-(xPulse+c*(t))) ) )
// define U0(x,y,t) exp( - alpha*( pow( a0*( (x)-(xPulse+c*(t)) ),20.) ) )
#define U0(x,y,t) exp( - alpha*( pow( a0*( (x)-(xPulse+c*(t)) ),pulsePow) ) )
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    // initial condition is a pulse, we make an approximate guess for u(-dt) 
    // u[1] = u(x,-dt) 
    // u[0] = u(x,t)
    getIndex(cg[grid].dimension(),I1,I2,I3); // assign all points including ghost points.
    if( option==smoothPulse )
    {
      if( cg[grid].isRectangular() )
      {
        // for a rectangular grid we avoid building the array of verticies.
        // we assign the initial conditions with C-style loops
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
        // (this would not work in parallel)
        real *upm= u[1][grid].Array_Descriptor.Array_View_Pointer3;
        real *up = u[0][grid].Array_Descriptor.Array_View_Pointer3;
        const int uDim0=u[0][grid].getRawDataSize(0);
        const int uDim1=u[0][grid].getRawDataSize(1);
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
	u[1][grid]=U0(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),-dt);
	u[0][grid]=U0(vertex(I1,I2,I3,0),vertex(I1,I2,I3,1),0.);
      }
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
    cg[grid].destroy(MappedGrid::THEvertex);  // vertices are no nolonger needed.

  }
  printf("done initial conditions\n");

}



int 
main(int argc, char *argv[])
{
  cout << "Usage: `wave [noplot][file.cmd]' \n";

  Overture::start(argc,argv);  // initialize Overture

  aString nameOfOGFile="cic.4.hdf";
  aString buff;

  aString commandFileName="";
  bool plotOption=true;  // by default we plot interactively
  if( argc > 1 )
  {
    for( int i=1; i<argc; i++ )
    {
      aString line=argv[i];
      if( line=="noplot" )
        plotOption=FALSE; 
      else
	commandFileName=line;
    }
  }
  
  PlotStuff ps(plotOption,"wave equation");
  PlotStuffParameters psp;

  // By default start saving the command file called "wave.cmd"
  aString logFile="wave.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  ps.outputString(sPrintF(buff,"wave>> Enter the name of the (old) overlapping grid file: (example; %s)",
                  (const char*)nameOfOGFile));
  ps.inputString(nameOfOGFile);

  aString nameOfShowFile="wave.show";

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update(MappedGrid::THEmask);

  int orderOfAccuracy=4;
  bool secondOrderGrid = max(cg[0].discretizationWidth())==3;
  printf(" secondOrderGrid=%i\n",secondOrderGrid);
  if( secondOrderGrid )
  { 
    orderOfAccuracy=2;
    printf("This grid has been made for 2nd-order accuracy. I am switching to 2nd-order accuracy.\n");
    printf("You can explicitly change the order back to 4 to try it.\n");
  }

  // Interpolant interpolant(cg);                                 // Make an interpolant
  Interpolant & interpolant = *new Interpolant(cg);               // do this instead for now. 
  Ogshow show;
    

  CompositeGridOperators operators(cg);                        // operators for a CompositeGrid
  operators.setOrderOfAccuracy(orderOfAccuracy);   
  BoundaryConditionParameters bcParams;

  Range all;
  realCompositeGridFunction u[2];
  u[0].updateToMatchGrid(cg);
  u[0].setOperators(operators);                                 
  u[0].setName("u");                                              // name the grid function
  u[1]=u[0];
  realCompositeGridFunction laplacian(cg);  // holds laplacian
  
  real tFinal=2.1;
  real tPlot=.05;  // plot this often
  real tShow=.25;  // save to show file this often
  bool saveShowFile=false;
  
  real t=0;
  real c=1., cSquared=c*c;
    
  real ad4=1.;  // coeff of the artificial dissipation.
  
  Index I1,I2,I3;
  // estimate the time step -- for now approximate by a first order wave equation.
  real dt;
  real cfl=.25, nu=0.;
  
  dt=getTimeStep( cg, operators, cfl, c, nu );
  real dtSquared=dt*dt;
  
  
  // Assign the initial conditions
  InitialConditionOptionEnum option=smoothPulse;

  getInitialConditions( option, u, t, dt, c );
 

  // Build a dialog menu for changing parameters
  GUIState gui;
  DialogData & dialog=gui;

  dialog.setWindowTitle("Wave Equation");
  dialog.setExitCommand("continue", "continue");

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
  sPrintF(textStrings[nt], "%g %g %g (alpha,a0,pulsePow)",alpha,a0,pulsePow);  nt++; 
  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  ps.pushGUI(gui);
  aString answer, line;
  int grid;
  int len=0;
  int current=0;
  for(;;) 
  {
    ps.erase();
    psp.set(GI_TOP_LABEL,sPrintF(buff,"Wave equation, t=%5.3f",t));
    PlotIt::contour(ps,u[current%2],psp);

    ps.getAnswer(answer,"");      
    if( answer=="exit" || answer=="continue" )
    {
      break;
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
    }
    else if( len=answer.matches("tFinal") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&tFinal);
      cout << " tFinal=" << tFinal << endl;
      dialog.setTextLabel("tFinal",sPrintF(line, "%g",tFinal));
    }
    else if( len=answer.matches("tPlot") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&tPlot);
      cout << " tPlot=" << tPlot << endl;
      dialog.setTextLabel("tPlot",sPrintF(line, "%g",tPlot));
    }
    else if( len=answer.matches("tShow") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&tShow);
      cout << " tShow=" << tShow << endl;
      dialog.setTextLabel("tShow",sPrintF(line, "%g",tShow));
    }
    else if( len=answer.matches("artificial dissipation") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&ad4);
      cout << " artificial diffusion=" << ad4 << endl;
      dialog.setTextLabel("artificial dissipation",sPrintF(line, "%g",ad4));
    }
    else if( len=answer.matches("save show file") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); saveShowFile=value;
      dialog.setToggleState("save show file",saveShowFile==true);
      if( saveShowFile )
        printf("Save show file %s \n",(const char*)nameOfShowFile);
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
      getInitialConditions( option, u, t, dt, c );
      dialog.getOptionMenu("Initial Condition:").setCurrentChoice((int)option);
    }
    else if( len=answer.matches("pulse params") )
    {
      printf("Smooth pulse:  U0 = exp( - alpha*( pow( a0*( (x)-(xPulse+c*(t)) ),pulsePow) ) )\n");
      sScanF(answer(len,answer.length()-1),"%e %e %e",&alpha,&a0,&pulsePow);
      dialog.setTextLabel("pulse params",sPrintF(line, "%g %g %g (alpha,a0,pulsePow)",alpha,a0,pulsePow));
      printf(" alpha=%g, a0=%g, pulsePow=%g \n",alpha,a0,pulsePow);
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
      if( saveShowFile )
      {
        show.open( nameOfShowFile );                               // create a show file
        show.saveGeneralComment("Wave equation");
        show.setFlushFrequency(2);                                 
      }
      
      t=0.;
      dt=getTimeStep( cg, operators, cfl, c, nu );
      getInitialConditions( option, u, t, dt, c );

      const int numberOfTimeSteps=int( tFinal/dt+.5);
      const int plotSteps = (int)max(1.,tPlot/dt+.5);
      const int showSteps = (int)max(1.,tShow/dt+.5);
      real time0=getCPU();
      for( int i=0; i<numberOfTimeSteps; i++ )                    // take some time steps
      {
	realCompositeGridFunction & u1 = u[i %2];
	realCompositeGridFunction & u2 = u[(i+1) %2];
    
	if( (i % plotSteps) == 0 )  // plot solution every 'plotSteps' steps
	{
          printf("completed step %i, t=%8.2e (cpu =%8.2e)\n",i,t,getCPU()-time0);
	  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  psp.set(GI_TOP_LABEL,sPrintF(buff,"Wave equation, t=%5.3f (order=%i)",t,orderOfAccuracy));
	  ps.erase();
	  PlotIt::contour(ps,u1,psp);
	  ps.redraw(TRUE);
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

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
	  realArray & u1g = u1[grid];
	  realArray & u2g = u2[grid];
	  realArray & lap = laplacian[grid];
          // evaluate laplacian(u1) and save in lap
	  operators[grid].derivative(MappedGridOperators::laplacianOperator,u1g,lap);
      
          // The next line could also be used for part of the loop below but is slightly
          // less efficient and uses a temporary array the size of 1 grid function.
	  // u2g=2.*u1g-u2g  + (dtSquared*cSquared)*lap; // this version uses A++ statements

          // Here we grab a pointer to the data of the array so we can index it as a C-array
          // (this would not work in parallel)
	  real *u1gp= u1g.Array_Descriptor.Array_View_Pointer3;
	  real       *u2gp= u2g.Array_Descriptor.Array_View_Pointer3;
	  real *lapp= lap.Array_Descriptor.Array_View_Pointer3;
	  const int uDim0=u[0][grid].getRawDataSize(0);
	  const int uDim1=u[0][grid].getRawDataSize(1);
	  const int d1=uDim0, d2=d1*uDim1; 
#define U1G(i1,i2,i3) u1gp[(i1)+(i2)*d1+(i3)*d2]
#define U2G(i1,i2,i3) u2gp[(i1)+(i2)*d1+(i3)*d2]

#define LAP(i1,i2,i3) lapp[(i1)+(i2)*d1+(i3)*d2]

          real cdtsq=dtSquared*cSquared;
	  int i1,i2,i3;
	  real ad4dt=ad4*dt;
          getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
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
	  
	  FOR_3(i1,i2,i3,I1,I2,I3) 
	  {
	    U2G(i1,i2,i3)=2.*U1G(i1,i2,i3)-U2G(i1,i2,i3)  + LAP(i1,i2,i3);
            
	  }
	}
    

	t+=dt;
	u2.interpolate();                                           // interpolate

	BCTypes::BCNames boundaryCondition=BCTypes::evenSymmetry; 

	if( boundaryCondition==BCTypes::evenSymmetry )  
	{
          // apply a symmetry BC
	  u2.applyBoundaryCondition(0,BCTypes::evenSymmetry,BCTypes::allBoundaries,0.);
          if( orderOfAccuracy==4 || ad4!=0. )
          {
	    bcParams.ghostLineToAssign=2;
            u2.applyBoundaryCondition(0,BCTypes::evenSymmetry,BCTypes::allBoundaries,0.,t,bcParams);
	  }
	}
	else if( boundaryCondition==BCTypes::dirichlet )
	{
	  u2.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
	  if( orderOfAccuracy==4 || ad4!=0. )
	    u2.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.); // for 4th order
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
    
	u2.finishBoundaryConditions();
      }
      current=(numberOfTimeSteps-1)%2;

    }
    else
    {
      cout << "Unknown command = [" << answer << "]\n";
      ps.stopReadingCommandFile();
       
    }
  }

  if( false )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }
  ps.popGUI();  // pop dialog

  Overture::finish();          
  return 0;
}
