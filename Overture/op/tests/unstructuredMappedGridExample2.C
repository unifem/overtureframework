#include "Overture.h" 
#include "HDF_DataBase.h" 
#include "PlotStuff.h"
#include "MappedGridOperators.h"
#include "OGPulseFunction.h"
#include "OGTrigFunction.h"

#include "UnstructuredMapping.h"
#include "UnstructuredOperators.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------------- \n");
  printf(" Solve a convection-diffusion equation                               \n");
  printf("    using prototype unstructured operators                           \n");
  printf(" Use operators to compute derivatives and apply boundary conditions  \n");
  printf(" Interactively plot results                                          \n");
  printf(" ------------------------------------------------------------------- \n");

  int plotOption=TRUE;
  aString filename="";
  if( argc == 2 || argc==3 )
    { // look at arguments for "noplot" or some other name
      aString line;
      for( int i=1; i<argc; i++ )
	{
	  line=argv[i];
	  if( line=="noplot" )
	    plotOption=FALSE;
	  else if( filename=="" )
	    filename=line;    
	}
    }
  else
    {
      cout << "Usage: "<<argv[0]<<" [noplot] file.[hdf|msh]"<<endl;
      exit(1);
    }

  PlotStuff ps(plotOption);
  GraphicsParameters psp;

  bool isAnHDF = false;
  if ( filename(filename.length()-4,filename.length()-1)==".hdf" )
    isAnHDF = true;
  
  UnstructuredMapping umap;
  if (isAnHDF)
    {
      HDF_DataBase db;
      db.mount(filename,"R");
      umap.get(db,"");
    }
  else
    {
      umap.get(filename);
    }

  int rangeDimension = umap.getRangeDimension();
  int nnodes = umap.getNumberOfNodes();
  int nelems = umap.getNumberOfElements();
  const intArray & faces = umap.getFaces();

  real bounds[2][3];
  real delt[3],mindelt;
  bounds[0][2] = bounds[1][2] = 0.0;
  
  for ( int aa=0; aa<2; aa++ )
    for ( int r=0; r<rangeDimension; r++ )
      bounds[aa][r] = (real)umap.getRangeBound(aa,r);
  
  delt[2] = 0;
  mindelt = REAL_MAX;
  for ( int r=0; r<rangeDimension; r++ )
    {
      delt[r] = (bounds[1][r]-bounds[0][r]);
      mindelt = min(mindelt,delt[r]);
    }
  cout<<"the mesh is "<<rangeDimension<<"D with "<<nnodes<<" nodes and "<<nelems<<" elements"<<endl;
	
  MappedGrid mg(umap);
  mg.update(MappedGrid::THEvertex);
  RealArray & vertices = mg.vertex();
  Range all;
  realMappedGridFunction u(mg);
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components

  Index I1,I2,I3;                                            
  // The A++ array mg.dimension()(2,3) holds index bounds on all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);               // assign I1,I2,I3 from dimension
  
  real  a=1., b=1., nu=.1; 
  //OGTrigFunction exact(1/delt[0],1/delt[1],0.0,1.0);
  OGPulseFunction exact;
  exact.setVelocity(a,b,0);
  exact.setRadius(0.1*mindelt);
  exact.setCentre(bounds[0][0]+0.2*mindelt,bounds[0][1]+0.2*mindelt);
  //  u(I1,I2,I3)=1.;// initial conditions

  for ( int n=0; n<nnodes; n++ )
    u(n,0,0,0)=exact(vertices(n,0,0,0),vertices(n,0,0,1),0.0);                                // initial conditions
    
  //MappedGridOperators op(mg);                    // operators 
  //  UnsOperators uop(mg);
  UnsOperators uop(mg);
  //u.setOperators(op);                            // associate with a grid function

  RealArray & uArray = (RealArray &)u;

  //PlotStuff ps(TRUE,"mappedGridExample2");      // create a PlotStuff object
  //PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

  real t=0, dt=.005;
  for( int step=0; step<200; step++ )
  {
    if( step % 10 == 0 )
    { // plot contours every 10 steps
      ps.erase();
      psp.set(GI_TOP_LABEL,sPrintF(buffer,"Solution at time t=%e",t));  // set title
      //psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      PlotIt::contour(ps, u,psp );
    }

    //u+=dt*( (-a)*u.x()+(-b)*u.y()+nu*(u.xx()+u.yy()) ); // ****** forward Euler time step *****

    //    u += dt * ( (-a)*uop.x(u) + (-b)*uop.y(u) + nu*(uop.xx(u)+uop.yy(u)) ); 
    u += dt * ( (-a)*uop.x(u) + (-b)*uop.y(u) + nu*uop.laplacian(u) ); // a bit more efficient

    // add back the forcing for the TW function
    for ( int n=0; n<nnodes; n++ )
      uArray(n,0,0,0) += dt*(exact.t(vertices(n,0,0,0),vertices(n,0,0,1),0.0,0,t) + 
	(a)*exact.x(vertices(n,0,0,0),vertices(n,0,0,1),0.0,0,t) +
			     (b)*exact.y(vertices(n,0,0,0),vertices(n,0,0,1),0.0,0,t)
	-nu*(exact.xx(vertices(n,0,0,0),vertices(n,0,0,1),0.0,0,t) + 
	     exact.yy(vertices(n,0,0,0),vertices(n,0,0,1),0.0,0,t)));

    t+=dt;
    // apply dirichlet Boundary conditions
    int component=0;
    for ( int fbdy=0; fbdy<umap.getNumberOfBoundaryFaces(); fbdy++ )
      {
	int fnm = umap.getBoundaryFace(fbdy);
	int nn = faces(fnm,0);
	uArray(nn,0,0,0) = exact(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0,0,t);
	nn = faces(fnm,1);
	uArray(nn,0,0,0) = exact(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0,0,t);
      }	

    //u.applyBoundaryCondition(component,BCTypes::dirichlet,BCTypes::allBoundaries,0.);    // set u=0.
    // fix up corners, periodic update:
    //u.finishBoundaryConditions();                                      
  }

  ps.erase();
  psp.set(GI_TOP_LABEL,sPrintF(buffer,"Solution at time t=%e (final)",t));  // set title
  PlotIt::contour(ps, u,psp );

  int nn;

  realMappedGridFunction uerr(mg);
  for ( nn=0; nn<nnodes; nn++ )
    {
      uerr(nn,0,0,0) = exact(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0,0,t)-uArray(nn,0,0,0);
    }

  ps.erase();
  psp.set(GI_TOP_LABEL,sPrintF(buffer,"Error at time t=%e (final)",t));  // set title
  PlotIt::contour(ps, uerr,psp );

  // compute some norms
  real linf = -REAL_MAX;
  real l2=0,l1=0,asum=0;
  for ( nn=0; nn<nnodes; nn++ )
    {
      real ex= exact(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0,0,t);
      real err = fabs(ex-uArray(nn,0,0,0));
      real area = uop.area(nn);
      
      linf = max(linf, err);
      l1 += err*area;
      l2 += err*err*area;
      asum += area;
    }

  l1 /=asum;
  l2 = sqrt(l2/asum);
  cout<<"linf "<<linf<<endl;
  cout<<"l1   "<<l1<<endl;
  cout<<"l2   "<<l2<<endl;

  Overture::finish();          
  return 0;
}

