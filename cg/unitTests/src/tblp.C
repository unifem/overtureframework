// ====================================================================================
///  \file tblp.C
///  \brief test program for the Boundary Layer Profile class
// ===================================================================================


#include "Overture.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "display.h"
// #include "NurbsMapping.h"

#define CSGEN  EXTERN_C_NAME(csgen)
#define CSEVAL EXTERN_C_NAME(cseval)

// These are the old spline routines from FMM
extern "C"
{
  void CSGEN (int & n, real & x, real & y, real & bcd, int & iopt  );
  void CSEVAL(int & n, real & x, real & y, real & bcd, real & u, real & s, real & sp );
}


#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// ========================================================================================================
/// \brief Class to evaluate a boundary layer profile
// ========================================================================================================

class BoundaryLayerProfile
{

public:

BoundaryLayerProfile();
~BoundaryLayerProfile();

int setParameters( real nu, real U );

// Evaluate the solution u(x,y) and v(x,y)
int eval( const real x, const real y, real & u, real & v );


// Evaluate the Blasius flat-plate profile
int evalBlasius( const real eta, real & f , real & fp );

protected:

int initializeProfile();

int blasiusFunc( real *fv, real *fvp );

real nu;  // kinematic viscosity
real U;   // free-stream velocity

bool initialized;
int nEta;  // number of eta points in spline
RealArray eta,f,fp,bcd;  // for spline fit 
real etaMin, etaMax;     // evaluate Blasius function over this range


};

BoundaryLayerProfile::
BoundaryLayerProfile()
{
  initialized=false;
  nu=1.e-3;
  U=1.;
  nEta=-1;
  etaMin=0.; etaMax=10.; // evaluate Blasius function over this range
  
}

BoundaryLayerProfile::
~BoundaryLayerProfile()
{
}
    
// ===================================================================================
/// \brief Assign the parameters in the solution
/// \param nu (input) : kinematic viscosity
/// \param U (input) : free-stream velocity
// ===================================================================================
int BoundaryLayerProfile::
setParameters( real nu_, real U_ )
{
  nu=nu_;
  U=U_;

  return 0;
}


// ===================================================================================
/// \brief Evaluate the solution u(x,y) and v(x,y)
/// \note  For x<=0 the solution is the free stream
// ===================================================================================
int BoundaryLayerProfile::
eval( const real x, const real y, real & u, real & v )
{

  if( !initialized )
    initializeProfile(); 

  if( x<=0 )
  {
    if( x<0 )
      printF("BoundaryLayerProfile:WARNING: x<0.\n");
    u=U;
    v=0.;
    return 0;
  }
  
  real eta0 = y*sqrt(U/(nu*x));

  if( eta0<etaMax )
  {
    real f0,fp0;
    evalBlasius( eta0, f0 ,fp0 );
  

    u = U*fp0;
    v = .5*sqrt(nu*U/x)*(eta0*fp0-f0);
  }
  else
  {
    // asymptotic form
    //  eta=10 :  eta*fp-f=1.72078750
    // Blasius: eta=  9.9500 f=  8.22921 fp=  1.00000  ( eta*fp-f=1.72078762)                                                                                                  // Blasius: eta= 10.0000 f=  8.27921 fp=  1.00000  ( eta*fp-f=1.72078763)        
    u=U;
    real Rex = U*x/nu;
    v = .5*1.7207876*U/sqrt(Rex);   // we could use more digits here 
  }
  
  return 0;
}


// ===================================================================================
/// \brief Evaluate the Blasius flat-plate profile f and the derivative f'
/// \param eta0 (input) : similarity variable eta0 = y*sqrt(U/(nu*x))
/// \param f0, fp0 (output) : f and f'
// ===================================================================================
int BoundaryLayerProfile::
evalBlasius( const real eta0, real & f0 , real & fp0 )
{
  if( !initialized )
    initializeProfile(); 

  if( false )
  {
    // --- Here is a place-holder for the Blasius solution ---
    
    // f'(1.73)=.5 
    // tanh(.55)=.5005
    real z=eta0*.55/1.73;
    fp = tanh(z);
  
    f= log(cosh(z));
  }
  
  // --- evaluate the splines --- 
  assert( nEta>0 );

  real fp1,fpp;
  real eta1=eta0;
  CSEVAL(nEta,eta(0), f(0),bcd(0,0,0), eta1,f0,fp1 ); 

  CSEVAL(nEta,eta(0),fp(0),bcd(0,0,1), eta1,fp0,fpp ); 

  // printF("evalBlasius: eta=%8.4f, f=%9.5f, fp=%9.5f (fp1=%9.5f)\n",eta0,f0,fp0,fp1);
  
  return 0;
}

// ===================================================================================
// "slope" function for the Blasius equation written as a first-order system
//   f''' = -.5*f*f'' 
// ===================================================================================
int BoundaryLayerProfile::
blasiusFunc( real *fv, real *fvp )
{
  fvp[0] = fv[1];    // f' 
  fvp[1] = fv[2];    // f'' 
  fvp[2] = -.5*fv[0]*fv[2];  // f''' 
}


// ===================================================================================
/// \brief This function will initialize the solution.
// ===================================================================================
int BoundaryLayerProfile::
initializeProfile()
{
  printF("BoundaryLayerProfile::initializeProfile: nu=%9.3e, U=%8.2e\n",nu,U);
  initialized=true;
  

  // Solve Blasius ODE with RK4:

  const real fpp0 = 0.3320573362151946;   // value of f''(0) that leads to f'(infinity)=1


  nEta=201;   // number of eta values 
  real deta=(etaMax-etaMin)/(nEta-1);  // time-step

  eta.redim(nEta); f.redim(nEta); fp.redim(nEta);  // save f and f' here 

  real fv[3], df[3], k1[3], k2[3], k3[3], k4[3]; 

  fv[0]=0.; fv[1]=0.; fv[2]=fpp0; // initial values 
  eta(0)=etaMin;
  f(0)=fv[0];  // save f 
  fp(0)=fv[1]; // save fp 
  for( int i=1; i<nEta; i++ )
  {

    blasiusFunc(fv,k1);

    for( int j=0; j<3; j++ ){ df[j] = fv[j] + .5*deta*k1[j]; }
    blasiusFunc(df,k2);

    for( int j=0; j<3; j++ ){ df[j] = fv[j] + .5*deta*k2[j]; }
    blasiusFunc(df,k3);

    for( int j=0; j<3; j++ ){ df[j] = fv[j] +   deta*k3[j]; }
    blasiusFunc(df,k4);

    for( int j=0; j<3; j++ ){ fv[j] +=  (deta/6.)*( k1[j] + 2.*(k2[j]+k3[j]) + k4[j] );  } // RK4 
       
    eta(i) = etaMin + i*deta;
    f(i)=fv[0];  // save f 
    fp(i)=fv[1]; // save fp 

    //  eta0*fp0-f0
    printF("Blasius: eta=%8.4f f=%9.5f fp=%9.5f  ( eta*fp-f=%10.8f) \n",eta(i),f(i),fp(i),eta(i)*fp(i)-f(i));
  }
  

  // --- fit splines ---
  int option= 0; // 0 = not-periodic
  int numberOfSplinePoints=nEta;
  bcd.redim(3,numberOfSplinePoints,2);
  CSGEN (numberOfSplinePoints, eta(0),  f(0), bcd(0,0,0), option ); // spline for f 
  CSGEN (numberOfSplinePoints, eta(0), fp(0), bcd(0,0,1), option ); // spline for f'


  return 0;
}



int 
main(int argc, char *argv[]) 
{
  Mapping::debug=0;

  Overture::start(argc,argv);  // initialize Overture and A++/P++

  printF("Usage: tbl -nu=<> -debug=<> -cmd=<> ... \n" );


  BoundaryLayerProfile profile;

  int debug = 1, plotOption=1;

  real nu=1.e-3;
  real U=1.;
   
  aString commandFileName="";

  char buff[180];
  int len=0;
  if( argc > 1 )
  { // look at arguments for "-noplot" or "-cfl=<value>"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
        plotOption=false;
      else if( len=line.matches("-nu=") )
      {
        sScanF(line(len,line.length()-1),"%e",&nu);
	printF("nu = %6.2f\n",nu);
      }
      else if( len=line.matches("-U=") )
      {
        sScanF(line(len,line.length()-1),"%e",&U);
	printF("U = %6.2f\n",U);
      }
      else if( len=line.matches("-debug=") )
      {
        sScanF(line(len,line.length()-1),"%i",&debug);
	printF("debug = %i\n",debug);
        // RigidBodyMotion::debug=debug;
      }
      else if( len=line.matches("-cmd=") )
      {
        commandFileName=line(len,line.length()-1);
        printF("tbl: reading commands from file [%s]\n",(const char*)commandFileName);
      }
    }
  }

  PlotStuff gi(plotOption,"Boundary Layer Profile Tester");
  PlotStuffParameters psp;
  
  // By default start saving the command file called "tbl.cmd"
  aString logFile="tbl.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }

  FILE *checkFile = fopen("tbl.check","w" );   // Here is the check file for regression tests

  aString answer;
  GUIState dialog;

  dialog.setWindowTitle("Boundary Layer Profile Tester");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"plot Blasius profile",
                    "compute solution",
                    "contour",
                    "stream lines",
                    "exit",
		    ""};

  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textLabels[nt] = "nu:"; 
  sPrintF(textStrings[nt],"%g",nu);  nt++; 

  textLabels[nt] = "U:"; 
  sPrintF(textStrings[nt],"%g",U);  nt++; 

  textLabels[nt] = "debug:"; 
  sPrintF(textStrings[nt],"%i",debug);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gi.pushGUI(dialog);


  real xa=1., xb=5., ya=0., yb=1.;
  SquareMapping mapping(xa,xb,ya,yb);
  mapping.setGridDimensions(axis1,501);  
  mapping.setGridDimensions(axis2,101);  
  MappedGrid mg(mapping);              
  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);                          

  OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),xLocal);

  Range all;
  realMappedGridFunction u;
  u.updateToMatchGrid(mg,all,all,all,2);          // define after declaration (like resize)
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components
  u.setName("v",1);                               // ...and components

  for(;;)
  {
    
    gi.getAnswer(answer,"");  //  testProblem = (TestProblemEnum)ps.getMenuItem(menu,answer,"Choose a test");

    if( answer=="exit" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"nu:","%e",nu) ){} //
    else if( dialog.getTextValue(answer,"U:","%e",U) ){} //
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //
    else if( answer=="plot Blasius profile" )
    {
      // Plot the Blasius function f and its derivative
      // u/U = f'
      // eta = y*sqrt(U/(nu*x))

      profile.setParameters( nu,U );
      
      int n=101;
      RealArray eta(n), w(n,3);
      real etaStart=0., etaEnd=10.;
      for( int i=0; i<n; i++ )
      {
	eta(i)=etaStart + (etaEnd-etaStart)*i/(n-1);
        real f,fp;
	profile.evalBlasius( eta(i),w(i,0),w(i,1) );
        w(i,2)=eta(i)*w(i,1)-w(i,0);  // eta*f' - f 
      }
 
      aString cNames[3]={"f","fp","fv"};  // 
      psp.componentsToPlot.redim(3);
      psp.componentsToPlot(0)=0;
      psp.componentsToPlot(1)=1;
      psp.componentsToPlot(2)=2;
      // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true); 

      gi.erase();
      PlotIt::plot(gi,eta,w,sPrintF("Blasius profile, nu=%9.3e",nu),"eta",cNames,psp);


    }

    else if( answer=="compute solution" )
    {
      profile.setParameters( nu,U );
      

      Index I1,I2,I3;
      getIndex(mg.gridIndexRange(),I1,I2,I3);               // assign I1,I2,I3 from dimension

      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	profile.eval( xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1), u(i1,i2,i3,0), u(i1,i2,i3,1) );
      }
   
    }
    
    else if( answer=="contour" )
    {
      gi.erase();
      PlotIt::contour(gi,u,psp);
    }
    else if( answer=="stream lines" )
    {
      gi.erase();
      PlotIt::streamLines(gi,u,psp);
    }

    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

  } // for(;;)
  
  gi.popGUI(); // restore the previous GUI

  fclose(checkFile);
  Overture::finish(); 
  return 0;
}
