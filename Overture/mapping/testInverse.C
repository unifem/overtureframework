// ==============================================================================
// 
//    Test the inverseMap function of the Mapping class
//    -------------------------------------------------
//
// parallel examples:
//    mpirun -np 1  testInverse -name=square -noInverse
// 
//    srun -N1 -n2 -ppdebug testInverse -name=square -noInverse
//    srun -N1 -n2 -ppdebug testInverse -name=smooth
//    srun -N1 -n2 -ppdebug testInverse -name=nurbs -debug=15
// 
//  Test find nearest grid point:
//    mpirun -np 2 -all-local testInverse -name=smooth -noInverse -testFindNearest
//    mpirun -np 2 -all-local testInverse -name=smooth -noInverse -testFindNearest -p0=1 -p1=1
//    mpirun -np 4 -all-local testInverse -name=smooth -noInverse -testFindNearest -p0=1 -p1=2
//    mpirun -np 4 -all-local testInverse -name=annulus -noInverse -testFindNearest -p0=1 -p1=2
//    mpirun -np 4 -all-local testInverse -name=dpm -noInverse -testFindNearest -p0=0 -p1=1
// 
//    srun -N1 -n2 -ppdebug memcheck_all ./testInverse -name=smooth -noInverse -testFindNearest
// Test parallel inverse
//    mpirun -np 2 -all-local testInverse -name=square -noInverse -testInverseMap
//    mpirun -np 2 -all-local testInverse -name=smooth -noInverse -testInverseMap
//    mpirun -np 2 -all-local testInverse -name=smooth -noInverse -testInverseMap -p0=1 -p1=1
//    mpirun -np 4 -all-local testInverse -name=smooth -noInverse -testInverseMap -p0=1 -p1=2
//    mpirun -np 4 -all-local testInverse -name=annulus -noInverse -testInverseMap -p0=1 -p1=2
//  -- dpm: 
//    mpirun -np 1 -all-local testInverse -name=dpm -noInverse -testInverseMap 
//    mpirun -np 4 -all-local testInverse -name=dpm -noInverse -testInverseMap -p0=1 -p1=3
// ==============================================================================
#include "MappingRC.h"
#include "BoxMapping.h"
#include "MatrixMapping.h"
#include "ComposeMapping.h"
#include "DataPointMapping.h"
#include "StretchMapping.h"       // stetching routines
#include "SquareMapping.h"        // square
#include "SphereMapping.h"        // sphere
#include "CircleMapping.h"
#include "AnnulusMapping.h"
#include "CylinderMapping.h"
#include "LineMapping.h"
#include "RevolutionMapping.h"
#include "OrthographicTransform.h"
#include "ReparameterizationTransform.h"
#include "SmoothedPolygon.h"
#include "CrossSectionMapping.h"
#include "RestrictionMapping.h"
#include "MatrixTransform.h"
#include "StretchedSquare.h"  
#include "NurbsMapping.h" 

#include "MappedGrid.h"
#include "PlotStuff.h"

// cause seg fault MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  
  aString name= "square";
  Mapping::debug=3;
  int numberOfPoints=16;
  bool noInverse=false;
  
  int testFindNearest=false;
  int testInverseMap=false;
  int testMap=false;
  
  int p0=0,p1=-1;  // distribute grids over this processor range 

  printF("====== testInverse: test the inverseMap function of the mapping class =====\n");
  printF("Usage: testInverse [-name=<>] [-debug=<num>] [-numPoints=<>] [-noInverse] \n"
         " -testInverseMap -testFindNearest -testMap -p0=<> -p1=<>\n");
  printF(" name : one of square, sphere, circle, cube, annulus, cylinder,  \n"
	   "   rotatedSquare, line, revolve, ortho, orthoSphere, xSection, orthoSection, restrict \n"
	   "  smoothedPolygon rotatedSmoothedPolygon, stretchedSquare, sphereSurface, nurbs, dpm \n"
	   " (append NI to not use basicInverse)  \n");

  int len=0;
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( len=arg.matches("-name=") )
      {
	name = arg(len,arg.length()-1);
	printF("Setting name=[%s]\n",(const char*)name);
      }
      else if( len=arg.matches("-debug=") )
      {
	sScanF(arg(len,arg.length()-1),"%i",&Mapping::debug);
	printF("Setting Mapping::debug=%i\n",Mapping::debug);
      }
      else if( len=arg.matches("-numPoints=") )
      {
	sScanF(arg(len,arg.length()-1),"%i",&numberOfPoints);
	printF("Setting numberOfPoints=%i\n",numberOfPoints);
      }
      else if( len=arg.matches("-p0=") )
      {
	sScanF(arg(len,arg.length()-1),"%i",&p0);
	printF("Setting p0=%i. processor range=[p0,p1]=[%i,%i]\n",p0,p0,p1);
      }
      else if( len=arg.matches("-p1=") )
      {
	sScanF(arg(len,arg.length()-1),"%i",&p1);
	printF("Setting p1=%i. processor range=[p0,p1]=[%i,%i]\n",p1,p0,p1);
      }
      else if( len=arg.matches("-testFindNearest") )
      {
	testFindNearest=true;
	printF("Setting testFindNearest=%i\n",testFindNearest);
      }
      else if( len=arg.matches("-testInverseMap") )
      {
	testInverseMap=true;
	printF("Setting testInverseMap=%i\n",testInverseMap);
      }
      else if( len=arg.matches("-testMap") )
      {
	testMap=true;
	printF("Setting testMap=%i\n",testMap);
      }
      else if( len=arg.matches("-noInverse") )
      {
	noInverse=true;
      }
      else
      {
	printF("Unknown option = [%s]\n",(const char*)arg);
      }
    }
  }


//   Range R = 0;
//   R.display("R");
//   printF(" R=[%i,%i]\n",R.getBase(),R.getBound());

  const MPI_Comm & OV_COMM = Overture::OV_COMM;
#ifdef USE_PPP
  fflush(0);
  Communication_Manager::Sync();
  MPI_Barrier(OV_COMM);
#endif

  int i;
    
  RealArray r(1,3);
  RealArray x(1,3);
  RealArray xr(1,3,3);
  RealArray rx(1,3,3);
  RealArray t(1,3);
  RealArray tx(1,3,3);

  
  RealArray r1,r2,x1,x2,xr1,xr2,rx2;

  Mapping *mapPointer;

  
  // -- Define a Mapping

  SquareMapping square(-1.,1.,-1.,1.);     // Define a square
  square.setName(Mapping::mappingName,"square");
  square.setGridDimensions( axis1,21 );
  square.setGridDimensions( axis2,21 );
  r=0.; square.mapS( r,x,xr );

  SphereMapping sphere(.5,1.,0.,0.,0.);     // Define a sphere, inner radius .5, outer radius 1, 
  sphere.setGridDimensions( axis1,11 );     // phi
  sphere.setGridDimensions( axis2,21 );     // theta
  sphere.setGridDimensions( axis3,7 );      // r

  SphereMapping sphereSurface(.5,1.,0.,0.,0.);  
  sphereSurface.setGridDimensions( axis1,11 );     // phi
  sphereSurface.setGridDimensions( axis2,21 );     // theta
  sphereSurface.setDomainDimension(2);

  CircleMapping circle(0.,0.,1.,1.);     // Define a circle, centre=(0,0), (a,b)=(1.,1.)
  circle.setName(Mapping::mappingName,"circle");
  circle.setGridDimensions( axis1,31 );     

  BoxMapping cube(-1.,1.,-1.,1.,-1.,1.)  ;  // [-1,1]x[-1,1]x[-1,1]
  cube.setName(Mapping::mappingName,"cube");
  cube.setGridDimensions( axis1,11 );
  cube.setGridDimensions( axis2,11 );

  AnnulusMapping annulus;
  annulus.setName(Mapping::mappingName,"annulus");

  CylinderMapping cylinder;     // Define a cylinder
  cylinder.setOrientation(2,0,1);
  cylinder.setName(Mapping::mappingName,"cylinder");

  // Here is a rotated square created with compose mapping
  MatrixMapping rotate(2,2)  ;     // Define a matrix mapping, R^2 -> R^2 
  rotate.setName(Mapping::mappingName,"rotate");
  rotate.rotate( zAxis, Pi/4. );  // rotate about z axis
  // ** ComposeMapping rotatedSquare( square,rotate );    // define a mapping by composition

  MatrixTransform rotatedSquare(square);
  rotatedSquare.rotate(axis3,45.);

  rotatedSquare.setName(Mapping::mappingName,"rotatedSquare");

  

  LineMapping line(0.,1.11);                  // mapping for the unit interval
  line.setName(Mapping::mappingName,"line");

  SquareMapping & unitSquare = *new SquareMapping; unitSquare.incrementReferenceCount();
  // fix this for parallel
  RevolutionMapping revolution(unitSquare);  // revolve a square
  // revolution.setParameterAxes(axis1,axis3,axis2);
  

  OrthographicTransform orthographic(1.,1.,OrthographicTransform::southPole); 

  ReparameterizationTransform orthoSphere(sphere);

  CrossSectionMapping xSection(.1,.9);  // cross-section, leave off singular ends

  CrossSectionMapping section;          // cross-section, include singular ends
  
  ReparameterizationTransform orthoSection(section);

  RestrictionMapping restrict(.1,.9,.2,.8);

  SmoothedPolygon smoothedPolygon;

  MatrixTransform rotatedSmoothedPolygon(smoothedPolygon);
  rotatedSmoothedPolygon.rotate(axis3,45.);
  rotatedSmoothedPolygon.setName(Mapping::mappingName,"rotatedSmoothedPolygon");

  StretchedSquare stretchedSquare;
  
//  NurbsMapping nurbs(2,3); 
  NurbsMapping nurbs(2,2); 

  DataPointMapping dpm;
  dpm.setMapping(annulus);

  
  
  if( name(0,5)=="square" )
    mapPointer=&square;
  else if( name(0,5)=="sphere" )
    mapPointer=&sphere;
  else if( name(0,5)=="circle" )
    mapPointer=&circle;
  else if( name(0,3)=="cube" )
    mapPointer=&cube;
  else if( name(0,6)=="annulus" )
    mapPointer=&annulus;
  else if( name(0,7)=="cylinder" )
    mapPointer=&cylinder;
  else if( name(0,12)=="rotatedSquare" )
    mapPointer=&rotatedSquare;
  else if( name(0,3)=="line" )
    mapPointer=&line;
  else if( name(0,6)=="revolve" )
  {
    mapPointer=&revolution;
  }
  else if( name(0,10)=="orthoSphere" )
    mapPointer=&orthoSphere;
  else if( name(0,11)=="orthoSection" )
    mapPointer=&orthoSection;
  else if( name(0,4)=="ortho" )
    mapPointer=&orthographic;
  else if( name(0,7)=="xSection" )
    mapPointer=&xSection;
  else if( name(0,7)=="restrict" )
    mapPointer=&restrict;
  else if( name(0,5)=="smooth" )
    mapPointer=&smoothedPolygon;
  else if( name(0,12)=="rotatedSmooth" )
    mapPointer=&rotatedSmoothedPolygon;
  else if( name(0,14)=="stretchedSquare" )
    mapPointer=&stretchedSquare;
  else if( name(0,4)=="nurbs" )
  {
    mapPointer=&nurbs;
  }
  else if( name(0,12)=="sphereSurface" )
    mapPointer=&sphere;
  else if( name.matches("dpm") )
    mapPointer=&dpm;
  else
  {
    cout << "Unknown mapping, name=" << name << endl;
    Overture::abort("error");
  }
  
  MappingRC map( *mapPointer );	

  printF("** domainDimension=%i, rangeDimension=%i \n",map.getDomainDimension(),map.getRangeDimension());

  if( noInverse )
  {
    map.setBasicInverseOption( Mapping::canDoNothing );  // turn off inverse if it exists
  }

  if( testFindNearest || testInverseMap || testMap ) 
  {
    // --- test parallel inverse or find nearest grid point ---


    PlotStuff gi;
    GraphicsParameters gip;
      
      
    if( p1<0 ) p1=np-1;
    p0=max( 0,min(p0,np-1));
    p1=max(p0,min(p1,np-1));
    
    // Range P(0,np-1);
    Range P(p0,p1);
    Partitioning_Type partition;
    partition.SpecifyProcessorRange(P); 
    map.getMapping().setPartition(partition);

    // for plotting we create a MappedGrid since plotStructured(Mapping) does not work for some mapping's (e.g. DPM)
    MappedGrid mg(map);
    // MappedGrid mg(square);
    mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask );

    gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,true); 
    // PlotIt::plot(gi, map.getMapping(), gip );
    PlotIt::plot(gi, mg, gip );
    gip.set(GI_USE_PLOT_BOUNDS,true);



    aString line;

    int numPoints=1;
    gi.inputString(line,"Enter number of points to check\n");
    sScanF(line,"%i",&numPoints);
    
    x.redim(numPoints,3);   x=0.;
    r.redim(numPoints,3);   r=0.;
    t.redim(numPoints,3);   t=0.;
    rx.redim(numPoints,3,3);
    
    RealArray dista(numPoints), xa(numPoints,3), ra(numPoints,map.getDomainDimension());
    xa=0.;
    for(;;) 
    {
      for( int i=0; i<numPoints; i++ )
      {
	gi.inputString(line,sPrintF("Enter point %i : x,y,z (to find the nearest grid point to, `done' to finish)) \n",i));
	if( line=="done" || line=="exit" ) break;
	
	sScanF(line,"%e %e %e",&x(i,0),&x(i,1),&x(i,2));
	if( map.getRangeDimension()==2 ) x(i,2)=0.;
      }
      if( line=="done" || line=="exit" ) break;

      // shift the points around on different processors: 
      real delta=max(fabs(x))*.1;
      for( int i=0; i<numPoints; i++ )
      {
	x(i,0) += myid*delta*(i+1);
	x(i,1) += myid*delta*2.*(i+1);
      }
      

//       printF("Enter x(1),..,x(3) for find nearest grid point (0,0,0)=stop\n");
//       cin >> x(0,axis1) >> x(0,axis2) >> x(0,axis3) ;
//       if( x(0,axis1)==0. && x(0,axis2)==0. && x(0,axis3)==0. ) 
// 	break;

      if( testFindNearest )
      {
	dista=REAL_MAX;
	map.getMapping().findNearestGridPoint( x,r, dista,xa );
	map.mapS( r,t );
	for( int i=0; i<numPoints; i++ )
	{
	  printf("Find results: myid=%i : x = (%6.3f,%6.3f,%6.3f), r = (%f,%f,%f), "
                 "map(r) = (%6.3f,%6.3f,%6.3f) dista=%8.2e xa=(%6.3f,%6.3f,%6.3f)\n",
		 myid,x(i,axis1),x(i,axis2),x(i,axis3),r(i,axis1),r(i,axis2),r(i,axis3),t(i,axis1),t(i,axis2),t(i,axis3),
		 dista(i),xa(i,axis1),xa(i,axis2),xa(i,axis3));
	}
	
      }
      else if( testInverseMap )
      {
        r=-1.;
	map.inverseMapS( x,r,rx );

        // map back for checking:
	map.mapS( r,t );

        // call without rx: 
	ra=-1.;
        map.inverseMapS( x,ra );  

	Range Rd=map.getDomainDimension();
	Range Rx=map.getRangeDimension();
	for( int i=0; i<numPoints; i++ )
	{
	  real diff=max(fabs(x(i,Rx)-t(i,Rx)));
	  printf("Inverse results: myid=%i : x = (%6.3f,%6.3f,%6.3f), r = (%f,%f,%f), "
		 "map(r) = (%6.3f,%6.3f,%6.3f), diff=%8.2e\n",
		 myid,x(i,axis1),x(i,axis2),x(i,axis3),r(i,axis1),r(i,axis2),r(i,axis3),
		 t(i,axis1),t(i,axis2),t(i,axis3),diff);
	  real diffr=max(fabs(r(i,Rd)-ra(i,Rd)));
	  if( diffr>REAL_EPSILON*100. )
	  {
	    printf("Inverse ERROR: myid=%i results for inverse(x,r) do not match inverse(x,r,rx) : diffr=%8.2e\n",
		   myid,diffr);
	  }
	  
	}
	xa=t; // for plotting
      }
      else 
      {
	// test map function
        r=x;
        r=min(1.05,max(-.05,r));
	x=0.;
        map.mapS( r,x,xr );

	for( int i=0; i<numPoints; i++ )
	{
	  printf("Map results: myid=%i : r = (%6.3f,%6.3f,%6.3f), x = (%f,%f,%f)\n",
		 myid,r(i,axis1),r(i,axis2),r(i,axis3),x(i,axis1),x(i,axis2),x(i,axis3));
	}
        xa=x;
      }
      
      
      fflush(0);
      
      gi.erase();

      int ptSize=5;
      gip.set(GI_POINT_SIZE,ptSize*gi.getLineWidthScaleFactor());
      gip.set(GI_POINT_COLOUR, "blue");
      gi.plotPoints(x,gip);
      gip.set(GI_POINT_COLOUR, "black");
      gi.plotPoints(xa,gip);
      // PlotIt::plot(gi, map.getMapping(), gip );
      PlotIt::plot(gi, mg, gip );


    }

    Overture::finish();          
    return 0;

  }
  



  for( ; Mapping::debug & 8; )
  {
    cout << "Enter x(1),..,x(3) for inverse (0,0,0)=stop" << endl;
    cin >> x(0,axis1) >> x(0,axis2) >> x(0,axis3) ;
    if( x(0,axis1)==0. && x(0,axis2)==0. && x(0,axis3)==0. ) 
      break;
    map.inverseMapS( x,r,rx );
    map.mapS( r,t );
    printF( "Results: x = (%6.3f,%6.3f,%6.3f), r = (%f,%f,%f), map(r) = (%6.3f,%6.3f,%6.3f)\n",
	   x(0,axis1),x(0,axis2),x(0,axis3),r(0,axis1),r(0,axis2),r(0,axis3),t(0,axis1),t(0,axis2),t(0,axis3));
  }


  x(0,axis1)=0.;  x(0,axis2)=0.; x(0,axis3)=0.;
  // This call will initialize the inverse
  real time0=getCPU();
  #ifndef USE_PPP
  map.inverseMapS( x,r,rx );  
  #endif
  cout << "Time to initialize inverseMap = " << getCPU()-time0 << endl;

  if( false && Mapping::debug < 8 )
    map.checkMapping();  // check the mapping and derivatives
    
  printF(" ---Call map with an array of values:\n");
  // diagonal line:
  int i1,i2,i3;
  enum CallTypes
  {
    diagonalLine,
    squarePatch
    } callType;
  // callType=diagonalLine;
  callType=squarePatch;

  int nx=1,ny=1,nz=1;
  if( callType==squarePatch )
  {
    if( map.getDomainDimension()==2 )
    {
      nx=(int)sqrt(real(np*numberOfPoints));
      ny=nx;
    }
    else if( map.getDomainDimension()==3 )
    {
      nx=int( pow(real(np*numberOfPoints),1./3.)+.5);
      ny=nx;
      nz=nx;
    }
    
  }
  
  r1.redim(numberOfPoints,3); r2.redim(numberOfPoints,3);
  x1.redim(numberOfPoints,3); x2.redim(numberOfPoints,3);
  xr1.redim(numberOfPoints,3,3); xr2.redim(numberOfPoints,3,3); rx2.redim(numberOfPoints,3,3);
  r1=0.;  r2=0.; x1=0.; x2=0.;

  if( callType==diagonalLine )
  {
    real dr = 1./real(np*(numberOfPoints-1));
    for( i=0; i<numberOfPoints; i++ )
    {
      int j= i + myid*(numberOfPoints-1);
      
      r1(i,axis1)=j*dr;
      r1(i,axis2)=j*dr;
      r1(i,axis3)=j*dr;
    }
  }
  else if( callType==squarePatch )
  {
    if( map.getDomainDimension()==1 )
    {
      real dr = 1./real(np*(numberOfPoints-1));
      for( i=0; i<numberOfPoints; i++ )
      {
        int j= i + myid*(numberOfPoints-1);
	r1(i,axis1)=j*dr;
	r1(i,axis2)=0.;
	r1(i,axis3)=0.;
      }
    }
    else if( map.getDomainDimension()==2 )
    {
      for( i=0; i<numberOfPoints; i++ )
      {
        int j= i + myid*(numberOfPoints-1);

	i1=j % nx;
	i2=(j/nx) % ny;
	r1(i,axis1)=i1/max(1.,nx-1);
	r1(i,axis2)=i2/max(1.,ny-1);
	r1(i,axis3)=0.;
      }
    }
    else
    {
      for( i=0; i<numberOfPoints; i++ )
      {
	int j= i + myid*(numberOfPoints-1);
	
	i1=j % nx;
	i2=(j/nx) % ny;
        i3=(j/(nx*ny)) % nz;
	r1(i,axis1)=i1/real(max(1,nx-1)); 
	r1(i,axis2)=i2/real(max(1,ny-1)); 
	r1(i,axis3)=i3/real(max(1,nz-1));
      }
    }
  }
  
  time0=getCPU();
  map.mapS( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  real timeForMap = getCPU()-time0;

  if(  Mapping::debug & 2 )
  {
    for( i=0; i<numberOfPoints; i++ )
    {
      if( map.getDomainDimension()==1 )
        printf(" myid=%i map: r= %6.3f",myid,r1(i,axis1));
      else if( map.getDomainDimension()==2 )
        printf(" myid=%i map: r= (%6.3f,%6.3f)",myid,r1(i,axis1),r1(i,axis2));
      else
        printf(" myid=%i map: r= (%6.3f,%6.3f,%6.3f)",myid,r1(i,axis1),r1(i,axis2),r1(i,axis3));

      if( map.getRangeDimension()==1 )
        printf(" x = %7.4f\n",x1(i,axis1));
      else if( map.getRangeDimension()==2 )
        printf(" x = (%7.4f,%7.4f)\n",x1(i,axis1),x1(i,axis2));
      else
        printf(" x = (%7.4f,%7.4f,%7.4f) \n",x1(i,axis1),x1(i,axis2),x1(i,axis3));
    }
  }
  
  printF(" ---Call inverseMap with an array of values:\n");

  // x1*=2.;
  
  for( i=0; i<numberOfPoints; i++ )
  {
    x2(i,axis1)=x1(i,axis1); 
    x2(i,axis2)=x1(i,axis2); 
    x2(i,axis3)=x1(i,axis3); 
  }
  time0=getCPU();
  r2=-1.;

  if( false )
    map.inverseMapS( x2,r2,rx2 );  
  else
    map.inverseMapS( x2,r2 );  

  real timeForInverseMap = getCPU()-time0;

/* ----
  r1.display("r1");
  r2.display("r2");

  map.map( r2,x2 );
  (x2-x1).display("x2-x1");
--- */  

  Mapping::openDebugFiles();
  FILE *& pDebugFile = Mapping::pDebugFile;

  if(  Mapping::debug & 2 )
  {
    real maxErr=0.;
    const int domainDimension=map.getDomainDimension();
    for( i=0; i<numberOfPoints; i++ )
    {
      if( map.getRangeDimension()==1 )
        fprintf(pDebugFile," inverseMap: myid=%i x = %7.4f",myid,x2(i,axis1));
      else if( map.getRangeDimension()==2 )
        fprintf(pDebugFile," inverseMap: myid=%i x = (%7.4f,%7.4f)",myid,x2(i,axis1),x2(i,axis2));
      else
        fprintf(pDebugFile," inverseMap: myid=%i x = (%7.4f,%7.4f,%7.4f)",myid,x2(i,axis1),x2(i,axis2),x2(i,axis3));

      if( map.getDomainDimension()==1 )
        fprintf(pDebugFile," r= %6.3f, ",r2(i,axis1));
      else if( map.getDomainDimension()==2 )
        fprintf(pDebugFile," r= (%6.3f,%6.3f), ",r2(i,axis1),r2(i,axis2));
      else
        fprintf(pDebugFile," r= (%6.3f,%6.3f,%6.3f), ",r2(i,axis1),r2(i,axis2),r2(i,axis3));

      if( map.getDomainDimension()==1 )
        fprintf(pDebugFile," err-r= %8.2e \n",fabs(r2(i,axis1)-r1(i,0)));
      else if( map.getDomainDimension()==2 )
        fprintf(pDebugFile," err-r= (%8.2e,%8.2e) \n",fabs(r2(i,axis1)-r1(i,0)),fabs(r2(i,axis2)-r1(i,1)));
      else
        fprintf(pDebugFile," err-r= (%8.2e,%8.2e,%8.2e) \n",fabs(r2(i,axis1)-r1(i,0)),fabs(r2(i,axis2)-r1(i,1)),
                          fabs(r2(i,axis3)-r1(i,2)));

      for( int axis=0; axis<domainDimension; axis++ )
      {
	real err = fabs(r2(i,axis)-r1(i,axis));
	if( err>.5 && (bool)map.getIsPeriodic(axis) )
	{
	  err = min(err, fabs(r2(i,axis)-r1(i,axis))-1.);
	}
        maxErr=max(maxErr,err);
      }
    }
    printf("\n *** myid=%i: max-err in inverseMap=%8.2e *** \n\n",myid,maxErr);
  }
  
  printf(" myid=%i Time for map = %6.2e, for inverse = %6.2e, ratio = %6.2e \n",
	 myid,timeForMap,timeForInverseMap,timeForInverseMap/max(REAL_MIN*100.,timeForMap));

  Index Axes(0,map.getRangeDimension());
  #ifndef USE_PPP
  time0=getCPU();
  for( i=0; i<numberOfPoints; i++ )
  {
    x(0,Axes)=x2(i,Axes);
    map.inverseMapS( x,r,rx );  
  }
  real timeForScalarInverseMap = getCPU()-time0;
  ApproximateGlobalInverse::printStatistics();

  printF(" Time for scalar inverseMap = %e, ratio to vector inverse = %e \n",
	 timeForScalarInverseMap,timeForScalarInverseMap/max(REAL_MIN*100.,timeForInverseMap));


  // call again (this check if we make use of the initial guess)
  time0=getCPU();
  map.inverseMapS( x2,r2,rx2 );  
  real timeForInverseMap2 = getCPU()-time0;

  printF(" Time for 2nd inverse (using initial guess) =%6.2e \n",timeForInverseMap2);
  ApproximateGlobalInverse::printStatistics();
  #endif

  if( unitSquare.decrementReferenceCount()==0 )
    delete &unitSquare;
  Overture::finish();

  return 0;
}
