#include "Overture.h"
#include "PlotStuff.h"  
#include "CompositeGridOperators.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ---------------------------------------------------------------------------- \n");
  printf("Solve a PDE using the efficient derivative evaluation                         \n");
  printf("      u_t + u u_x + v u_y = nu( u.xx + u.yy )                                   \n");
  printf("      v_t + u v_x + v v_y = nu( v.xx + v.yy )                                   \n");
  printf(" ---------------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "example>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  // Interpolant interpolant(cg);                                 // Make an interpolant
  Interpolant & interpolant = *new Interpolant(cg);               // do this instead for now. 

  PlotStuff ps(TRUE,"example10");
  PlotStuffParameters psp;
    
  CompositeGridOperators operators(cg);                        // operators for a CompositeGrid

  Range all;
  realCompositeGridFunction uv(cg,all,all,all,2);               // create a grid function with 2 components
  uv.setOperators(operators);                                 
  uv.setName("uv");                                             // name the grid function
  uv.setName("u",0);                                            // name the component
  uv.setName("v",1);                                            // name the component
  realCompositeGridFunction u,v;                                // make links to the 2 components
  u.link(uv,Range(0,0));
  v.link(uv,Range(1,1));

  // The arrays uvx, uvy, uvxx and uvyy are used to save the results in. These arrays are re-used for all
  // the different component grids (thus saving space)
  RealArray uvx,uvy,uvxx,uvyy;                          
  // --- make a list of derivatives to evaluate on each component grid
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    operators[grid].setNumberOfDerivativesToEvaluate( 4 );        
    operators[grid].setDerivativeType( 0, MappedGridOperators::xDerivative,  uvx );
    operators[grid].setDerivativeType( 1, MappedGridOperators::yDerivative,  uvy );
    operators[grid].setDerivativeType( 2, MappedGridOperators::xxDerivative, uvxx);
    operators[grid].setDerivativeType( 3, MappedGridOperators::yyDerivative, uvyy);
  }
  
  u=+1.;                             // initial condition  u=+1
  v=-1.;                             // initial condition  v=-1
  
  real t=0, dt=.01;                                            // initialize time and time step
  real viscosity=.1;                                           // initialize parameters
    
  Index I1,I2,I3;
  Index N(0,2);                                                // Index for components, N=0,1
  char buffer[80];                                             // buffer for sprintf
  int numberOfTimeSteps=250;
  for( int i=0; i<=numberOfTimeSteps; i++ )                    // take some time steps
  {
    if( i % 5 ==0 )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buffer,"Solution at t=%e",t));
      ps.erase();
      PlotIt::contour(ps,uv,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);     // set this to run in "movie" mode (after first plot)
      ps.redraw(TRUE);
    }
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].gridIndexRange(),I1,I2,I3);             // define Index's for interior+boundary pts
      uv[grid].getDerivatives(I1,I2,I3,N);                    // evaluate all derivatives at once

      RealArray & uu = u[grid];                               // make some aliases for readability
      RealArray & vv = v[grid];                               // and efficiency
      const RealArray & ux = uvx (I1,I2,I3,0);                // these values were computed by getDerivatives.
      const RealArray & uy = uvy (I1,I2,I3,0);                // Note that these arrays will be redimensioned
      const RealArray & uxx= uvxx(I1,I2,I3,0);                // by getDerivatives only if there is not enough
      const RealArray & uyy= uvyy(I1,I2,I3,0);                // space. Thus after one time step the arrays
      const RealArray & vx = uvx (I1,I2,I3,1);                // will be as large as the largest grid and then
      const RealArray & vy = uvy (I1,I2,I3,1);                // will remain that size.
      const RealArray & vxx= uvxx(I1,I2,I3,1);
      const RealArray & vyy= uvyy(I1,I2,I3,1);

      uu(I1,I2,I3)+=dt*( -uu(I1,I2,I3)*ux -vv(I1,I2,I3)*uy +viscosity*( uxx+uyy ));            // Euler time step
      vv(I1,I2,I3)+=dt*( -uu(I1,I2,I3)*vx -vv(I1,I2,I3)*vy +viscosity*( vxx+vyy ));
      
    }
    
    uv.interpolate();                                           // interpolate
    // apply a dirichlet BC on all boundaries:
    uv.applyBoundaryCondition(N,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
    uv.finishBoundaryConditions();
    t+=dt;
  }
  
  Overture::finish();          
  return 0;
}
