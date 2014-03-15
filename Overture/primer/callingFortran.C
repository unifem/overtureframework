#include "Overture.h"
#include "Ogshow.h"  
#include "CompositeGridOperators.h"
#include "PlotStuff.h"
#include "display.h"

// fortran names may have an appended underscore which is added by the EXTERN_C_NAME macro: 
#define mySolver EXTERN_C_NAME(mysolver)

extern "C"
{
  // fortran variables are passed by reference:
  void mySolver( const real &t, const real &dt,const real &a,const real &b,const real &nu, const int&nd,
     const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,
     const int &n1a,const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b,
	       const real &x,const real &u, real &dudt );
}

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ---------------------------------------------------------------------------- \n");
  printf(" Solve a PDE u.t=f(u,x,t). Call a fortran routine to compute f(u,x,t) for     \n");
  printf(" each component grid. Plot the results.                                       \n");
  printf(" ---------------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "callingFortran>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update(MappedGrid::THEvertex | MappedGrid::THEmask);      // build vertices and mask

  // Interpolant interpolant(cg);                                 // Make an interpolant
  Interpolant & interpolant = *new Interpolant(cg);               // do this instead for now. 

  CompositeGridOperators operators(cg);                        // operators for a CompositeGrid

  Range all;
  realCompositeGridFunction u(cg,all,all,all,1);               // create grid functions
  realCompositeGridFunction dudt(cg,all,all,all,1);            
  u.setOperators(operators);                                 
  u.setName("u");                                              // name the grid function

  u=1.;                                                        // initial condition
  dudt=0.;
  
  real t=0, dt=.01;                                           // initialize time and time step
  real a=1., b=1., nu=.1;                                     // initialize parameters

  bool openGraphicsWindow=TRUE;
  PlotStuff ps(openGraphicsWindow,"callingFortran");
  PlotStuffParameters psp;
  
  
  aString buff;                                              // buffer for sPrintF
  int numberOfTimeSteps=200;
  for( int i=0; i<numberOfTimeSteps; i++ )                    // take some time steps
  {
    if( (i % 10)==0 )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution t=%f",t));  // set title
      ps.erase();
      PlotIt::contour(ps,u,psp);
      ps.redraw(true);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);     // set this to run in "movie" mode (after first plot)
    }
    
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ ) // loop over grids
    {
      MappedGrid & mg = cg[grid];
      realArray & ug = u[grid];
      realArray & dudtg = dudt[grid];
      realArray & x = mg.vertex();  // array of vertices

      const IntegerArray & d = mg.dimension();
      const IntegerArray & gir= mg.gridIndexRange();
      const int nd=cg.numberOfDimensions();

      // call a fortran function to compute du/dt
      // (This function does not currently solve the convection diffusion equation)
      mySolver( t,dt,a,b,nu,nd, d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2), 
              gir(0,0),gir(1,0),gir(0,1),gir(1,1),gir(0,2),gir(1,2), 
              *x.getDataPointer(),*ug.getDataPointer(), *dudtg.getDataPointer() );
      
      // display(ug,"ug","%6.3f");
      // display(dudtg,"dudtg","%6.3f");
      
      ug+=dt*dudtg;
    }
    t+=dt;
    u.interpolate();                                           // interpolate
    // u.display("u after interpolate");
    
    // apply a dirichlet BC on all boundaries:
    u.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
    u.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.);
    u.finishBoundaryConditions();
  }

  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);  
  psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution t=%f",t));  // set title
  ps.erase();
  PlotIt::contour(ps,u,psp);

  Overture::finish();          
  return 0;
    
}
