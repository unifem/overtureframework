//================================================================================
//  Test the Integrate class for integrating grid functions.
//
// Examples:
//
//   testIntegrate -grid=cic -checkDataBase
// 
//   testIntegrate -grid=sibe.hdf -hybrid -surfOnly
//       surfaceArea for sphere = 3.094677e+00, (true=3.14159) error=4.691584e-02
// 
//   testIntegrate -grid=sibe4.order2.hdf -hybrid -surfOnly -interactive 
//       surfaceArea for sphere = 3.095142e+00, (true=3.14159) error=4.645061e-02 (null-vector)     
//       surfaceArea for sphere = 3.116109e+00, (true=3.14159) error=2.548364e-02 (hybrid)
//       surfaceArea for sphere = 3.139267e+00, (true=3.14159) error=2.325592e-03 (hybrid *new*)
//   testIntegrate -grid=sibe8.order2.hdf -hybrid -surfOnly -interactive
//       surfaceArea for sphere = 3.140966e+00, (true=3.14159) error=6.266765e-04 (hybrid *new*)
//   testIntegrate -grid=sibe16.order2.hdf -hybrid -surfOnly -interactive
// 
//   testIntegrate -grid=ellipsoid2.hdf -hybrid -surfOnly
//       surfaceArea for ellipsoid = 2.143764e+01, (true=21.4784) error=4.079462e-02 (null-vector)
//       surfaceArea for ellipsoid = 2.091132e+01, (true=21.4784) error=5.671125e-01 (hybrid)
//       surfaceArea for ellipsoid = 2.145757e+01, (true=21.4784) error=2.086925e-02 (hybrid *new*)
// 
//   testIntegrate -grid=boxsbs1.hdf -hybrid -surfOnly -interactive 
// 
// AMR:
//   testIntegrate -grid=cic2.hdf -amr (will test AMR for surface integrals)
// 
//   testIntegrate -grid=sibe.hdf -hybrid -surfOnly -amr 
//
// Parallel:
//  srun -N1 -n2 -ppdebug testIntegrate -grid=square20
//================================================================================

#include "Overture.h"
#include "Integrate.h"
#include "OGTrigFunction.h"  // Trigonometric function
#include "OGPolyFunction.h"  // polynomial function
#include "display.h"
#include "Oges.h"
#include "PlotStuff.h"
#include "SurfaceStitcher.h"
#include "Ogen.h"
#include "ParallelUtility.h"
#include "HDF_DataBase.h"

int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  printF("Usage: `testIntegrate -grid=<gridName> -tz=trig -hybrid -surfOnly -interactive -amr -debug=<> -checkDataBase -surfaceArea=<f> -volume=<f>' \n");

  const int maxNumberOfGridsToTest=4;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "sic", "cic", "sib" };
    
  bool useHybrid=false;
  bool checkSurfaces=true;
  bool checkVolumes=true;
  bool interactiveStitching=false; // true;
  bool amr=false;
  bool checkDataBase=false;        // if true then saving and reading Integrate to a file.
  
  // The user may supply the exact values for comparison
  real exactSurfaceArea=-1.;
  real exactVolume=-1.;
  
  
  Integrate::debug=0;

  int tz=0, len=0;
  if( argc > 1 )
  { 
    // numberOfGridsToTest=1;
    for( int i=1; i<argc; i++ )
    {
      aString line;
      line=argv[i];
      if( line(0,6)=="tz=trig" )
	tz=1;
      else if( line.matches("-hybrid") )
      {
	useHybrid=true;
      }
      else if( line.matches("-surfOnly") )
      {
	checkVolumes=false;
      }
      else if( line.matches("-interactive") )
      {
	interactiveStitching=true;
      }
      else if( line.matches("-checkDataBase") )
      {
	checkDataBase=true;
      }
      else if( line.matches("-amr") )
      {
	amr=true;
      }
      else if( len=line.matches("-debug=") )
      {
	sScanF(line(len,line.length()-1),"%i",&Integrate::debug);
	printF("Setting Integrate::debug=%i\n",Integrate::debug);
      }
      else if( len=line.matches("-surfaceArea=") )
      {
	sScanF(line(len,line.length()-1),"%e",&exactSurfaceArea);
	printF("Setting exactSurfaceArea=%16.8e\n",exactSurfaceArea);
      }
      else if( len=line.matches("-volume=") )
      {
	sScanF(line(len,line.length()-1),"%e",&exactVolume);
	printF("Setting exactVolume=%16.8e\n",exactVolume);
      }
      else if( len=line.matches("-grid=") )
      {
	numberOfGridsToTest=1;
        gridName[0]=line(len,line.length()-1);
      }
    }
  }

  int plotOption=useHybrid;
  PlotStuff ps(plotOption,"testIntegrate");    // add this for now -- fix stitcher
  PlotStuffParameters psp;

  int debug=0;

  // Oges::debug=1;
  // Integrate::debug=3;
    
  real worstError=0;
  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];

    printF("\n *****************************************************************\n"
           " ******** Checking grid: %s ************ \n"
           " *****************************************************************\n\n",(const char*)nameOfOGFile);
    
    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );
    
    Integrate integrate(cg);
    integrate.useHybridGrids(useHybrid);
    integrate.setInteractiveStitching(interactiveStitching);
    
    real volume, surfaceArea;
    realCompositeGridFunction u(cg);
    u=1.;

    // Save [x,y,z] [x^2,y^2,z^2] in v 
    Range all;
    Range Rx = cg.numberOfDimensions(), Rv=2*cg.numberOfDimensions();
    realCompositeGridFunction v(cg,all,all,all,Rv);
    Index I1,I2,I3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++)
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      realArray & x = cg[grid].center();
      
      #ifdef USE_PPP
        realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v[grid],vLocal);
      #else
        realSerialArray & xLocal = x;
        realSerialArray & vLocal = v[grid]; 
      #endif
      int includeGhost=1; // include parallel ghost pts in fLocal:
      bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost);
      if( !ok ) continue; // there are no points on this processor.

      vLocal(I1,I2,I3,Rx)=xLocal(I1,I2,I3,Rx);
      vLocal(I1,I2,I3,Rx+cg.numberOfDimensions())=SQR(xLocal(I1,I2,I3,Rx));
    } 


    if( gridName[it].matches("cic") &&  !gridName[it].matches("cic2") )
    {
      int surface=0;
      int numberOfFaces=1;
      IntegerArray boundary(3,numberOfFaces);
      int side=0, axis=axis2, grid=1;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      integrate.defineSurface( 0,numberOfFaces,boundary ); 
      
      surfaceArea = integrate.surfaceIntegral(u,surface);
      printF("Grid cic: surfaceArea for cylinder = %e, error=%e \n",surfaceArea,fabs(surfaceArea-Pi));
    }
    else if( gridName[it].matches("sib") || gridName[it].matches("drop3d") )
    {
      int surface=0;
      int numberOfFaces=2;
      IntegerArray boundary(3,numberOfFaces);
      int side=0, axis=axis3, grid=1;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      grid=2;
      boundary(0,1)=side;
      boundary(1,1)=axis;
      boundary(2,1)=grid;
      integrate.defineSurface( 0,numberOfFaces,boundary ); 
      
      surfaceArea = integrate.surfaceIntegral(u,surface);
      real radius=.5;
      real areaTrue=4.*Pi*SQR(radius);
      
      printF("Grid %s: surfaceArea for sphere = %e, (true=%g) error=%7.1e \n",(const char*)gridName[it],
               surfaceArea,areaTrue,fabs(surfaceArea-areaTrue));

      if( amr )
      {
	// in this case we add refinement grids 
	cg.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

	IntegerArray range(2,3), factor(3);
	int ratio=2;  // refinement ratio
	factor = ratio;                            // refinement factor 
	int grid=0,level=1;
	
	grid = 1;                              // refine this base grid
	range(0,0) = 2; range(1,0) = 6;
	range(0,1) = 2; range(1,1) = 4;
	range(0,2) = 0; range(1,2) = 2;
	cg.addRefinement(range, factor, level, grid);

	grid = 2;                              // refine this base grid
	range(0,0) = 0; range(1,0) = 6;
	range(0,1) = 0; range(1,1) = 4;
	range(0,2) = 0; range(1,2) = 2;
	cg.addRefinement(range, factor, level, grid);


	// here is a second refinement at level=1
// 	range(0,0) = 6; range(1,0) = 8;
// 	range(0,1) = 1; range(1,1) = 4;
// 	cg.addRefinement(range, factor, level, grid);

	// here is a first refinement at level=2
	// level=2;
	// range(0,0) = 5*ratio; range(1,0) = 7*ratio;
	// range(0,1) = 3*ratio; range(1,1) = 4*ratio;
	// cg.addRefinement(range, factor, level, grid);

      
	cg.update(GridCollection::THErefinementLevel);  
	//  cg.setMaskAtRefinements();

	Ogen ogen;
        ogen.updateRefinement(cg);

	if( true )
	{
	  psp.set(GI_TOP_LABEL,"Grid with refinements"); 
	  PlotIt::plot(ps,cg,psp);
	}
      
	u.updateToMatchGrid(cg);
	u=1.;
      }

      const int nd=cg.numberOfDimensions();
      RealArray integral(Rv);
      integrate.surfaceIntegral(v,Rv,integral,surface);
      real xIntegral=0., xSquaredIntegral=(1./3.)*pow(radius,2.)*areaTrue;
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	printF(" Integral(x%i)   = %11.4e, true=%11.4e, err=%7.1e\n",
	       axis,integral(axis),xIntegral,fabs(integral(axis)-xIntegral));
      }
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	printF(" Integral(x%i^2) = %11.4e, true=%11.4e, err=%7.1e\n",
               axis,integral(axis+nd),xSquaredIntegral,fabs(integral(axis+nd)-xSquaredIntegral));
      }
    }
    else if( gridName[it](0,6)=="twoDrop" )
    {
      int surface=0;
      int numberOfFaces=1;
      IntegerArray boundary(3,numberOfFaces);
      int side=0, axis=1, grid=1;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      integrate.defineSurface( surface,numberOfFaces,boundary ); 
      
      surface=1;
      side=0;
      axis=1;
      grid=2;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      integrate.defineSurface( surface,numberOfFaces,boundary ); 
      
      for( surface=0; surface<2; surface++ )
      {
	surfaceArea = integrate.surfaceIntegral(u,surface);
	printF("Grid twoDrop: surfaceArea for drop %i = %e, error=%e \n",surface,surfaceArea,
	       fabs(surfaceArea-2.*Pi*.3));
      }
      
    }
    else if( gridName[it].matches("square10") )
    {
      // *** computeSurfaceWeights fails after the grid is changed because the cg with Integrate has changed ***
      surfaceArea = integrate.surfaceIntegral(u);
      printF("surfaceArea =%e before any volume integrals\n",surfaceArea);
      volume = integrate.volumeIntegral(u);
      printF("volume = %e \n",volume);

      // in this case we add refinement grids 
      cg.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

      IntegerArray range(2,3), factor(3);
      int ratio=2;  // refinement ratio

      range(0,0) = 2; range(1,0) = 6;
      range(0,1) = 0; range(1,1) = 4;
      range(0,2) = 0; range(1,2) =  0;
      factor = ratio;                            // refinement factor 
      Integer level = 1;
      int grid = 0;                              // refine this base grid
      cg.addRefinement(range, factor, level, grid);

      // here is a second refinement at level=1
      range(0,0) = 6; range(1,0) = 8;
      range(0,1) = 0; range(1,1) = 6;
      cg.addRefinement(range, factor, level, grid);

      // here is a first refinement at level=2
      level=2;
      range(0,0) = 5*ratio; range(1,0) = 7*ratio;
      range(0,1) = 0;       range(1,1) = 2*ratio;
      cg.addRefinement(range, factor, level, grid);

      
      cg.update(GridCollection::THErefinementLevel);  
      cg.setMaskAtRefinements();

      if( false )
      {
	psp.set(GI_TOP_LABEL,"Grid with refinements"); 
	PlotIt::plot(ps,cg,psp);
      }
      
      u.updateToMatchGrid(cg);
      u=1.;
      
      int surface=0;
      int numberOfFaces=1;
      IntegerArray boundary(3,numberOfFaces);
      int side=0, axis=axis2;
      grid=0;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      integrate.defineSurface( 0,numberOfFaces,boundary ); 
      
      integrate.useAdaptiveMeshRefinementGrids(false);
      real surfaceAreaNoAMR = integrate.surfaceIntegral(u,surface);
      printF("Grid square10: surfaceArea for bottom = %e, error=%e (no AMR)\n",surfaceAreaNoAMR,
                   fabs(surfaceAreaNoAMR-1.));
      
      integrate.useAdaptiveMeshRefinementGrids(true);
      surfaceArea = integrate.surfaceIntegral(u,surface);
      printF("Grid square10: surfaceArea for bottom = %e, error=%e\n",surfaceArea,fabs(surfaceArea-1.));

    }
    else if( gridName[it].matches("cic2") )
    {
      // *** computeSurfaceWeights fails after the grid is changed because the cg with Integrate has changed ***
      surfaceArea = integrate.surfaceIntegral(u);
      printF("surfaceArea =%e before any volume integrals\n",surfaceArea);
      volume = integrate.volumeIntegral(u);
      printF("volume = %e \n",volume);

      if( amr )
      {
	// in this case we add refinement grids 
	cg.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

	IntegerArray range(2,3), factor(3);
	int ratio=2;  // refinement ratio

	range(0,0) = 2; range(1,0) = 6;
	range(0,1) = 2; range(1,1) = 4;
	range(0,2) = 0; range(1,2) =  0;
	factor = ratio;                            // refinement factor 
	Integer level = 1;
	int grid = 1;                              // refine this base grid
	cg.addRefinement(range, factor, level, grid);

	// here is a second refinement at level=1
	range(0,0) = 6; range(1,0) = 8;
	range(0,1) = 1; range(1,1) = 4;
	cg.addRefinement(range, factor, level, grid);

	// here is a first refinement at level=2
	level=2;
	range(0,0) = 5*ratio; range(1,0) = 7*ratio;
	range(0,1) = 3*ratio; range(1,1) = 4*ratio;
	cg.addRefinement(range, factor, level, grid);

      
	cg.update(GridCollection::THErefinementLevel);  
	cg.setMaskAtRefinements();

	if( true )
	{
	  psp.set(GI_TOP_LABEL,"Grid with refinements"); 
	  PlotIt::plot(ps,cg,psp);
	}
      
	u.updateToMatchGrid(cg);
	u=1.;
      }
      
      int surface=0;
      int numberOfFaces=1;
      IntegerArray boundary(3,numberOfFaces);
      int side=1, axis=axis2;   // note: cic2 has a boundary on side=1
      int grid=1;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      integrate.defineSurface( 0,numberOfFaces,boundary ); 
      
      real trueArea = Pi;

      integrate.useAdaptiveMeshRefinementGrids(false);
      real surfaceAreaNoAMR = integrate.surfaceIntegral(u,surface);
      printF("Grid cic2: surfaceArea for circle = %e, error=%e (no AMR)\n",surfaceAreaNoAMR,
	     fabs(surfaceAreaNoAMR-trueArea));
      if( amr )
      {
	integrate.useAdaptiveMeshRefinementGrids(true);
	surfaceArea = integrate.surfaceIntegral(u,surface);
	printF("Grid cic2: surfaceArea for circle = %e, error=%e (with AMR)\n",surfaceArea,fabs(surfaceArea-trueArea));
      
	int numFaces =integrate.numberOfFacesOnASurface(surface); 
	printF("INFO: There are a total of %i faces on surface=%i (including AMR grids)\n",numFaces,surface);
      
	int fside, faxis, fgrid;
	for( int face=0; face<numFaces; face++ )
	{
	  integrate.getFace(surface,face, fside,faxis,fgrid);
	  printF("INFO: surface=%i: face=%i (grid,side,axis)=(%i,%i,%i)\n",surface,face,fgrid,fside,faxis);
	}
      }
      
    }
    else if( gridName[it].matches("ellipsoid") )
    {
      int surface=0;
      int numberOfFaces=3;
      IntegerArray boundary(3,numberOfFaces);
      int side=0, axis=axis3, grid=1;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      grid=2;
      boundary(0,1)=side;
      boundary(1,1)=axis;
      boundary(2,1)=grid;
      grid=3;
      boundary(0,2)=side;
      boundary(1,2)=axis;
      boundary(2,2)=grid;
      integrate.defineSurface( 0,numberOfFaces,boundary ); 
      
      surfaceArea = integrate.surfaceIntegral(u,surface);
      // surface area of a prolate ellipsoid (axis of rotation = major axis)
      real a=2., b=1.;
      real e = sqrt(1.-SQR(b/a));  // eccentricity
      real areaTrue=2.*Pi*b*b*( 1+ (a/b)*asin(e)/e );
      
      printF("Grid %s: surfaceArea for ellipsoid = %e, (true=%g) error=%e \n",(const char*)gridName[it],
               surfaceArea,areaTrue,fabs(surfaceArea-areaTrue));
    }
    else if( gridName[it].matches("boxsbs") )
    { // two boxes side by side -- we should get the surface area exactly in this case

      int surface=0;
      int numberOfFaces=2;
      IntegerArray boundary(3,numberOfFaces);
      int side=0, axis=axis3, grid=0;
      boundary(0,0)=side;
      boundary(1,0)=axis;
      boundary(2,0)=grid;
      grid=1;
      boundary(0,1)=side;
      boundary(1,1)=axis;
      boundary(2,1)=grid;

      integrate.defineSurface( 0,numberOfFaces,boundary ); 

      surfaceArea = integrate.surfaceIntegral(u,surface);
      // surface area of a prolate ellipsoid (axis of rotation = major axis)

      real areaTrue=(numberOfFaces/2)*SQR(2.);   // 6 faces, each [-1,1]^2
      printF("Grid %s: surfaceArea for boxsbs = %e, (true=%g) error=%e \n",(const char*)gridName[it],
               surfaceArea,areaTrue,fabs(surfaceArea-areaTrue));
    }
    
    surfaceArea = integrate.surfaceIntegral(u);
    printF("\n"
           " -------------- testIntegrate results for grid %s ---------------\n",
           (const char*)nameOfOGFile);

    if( exactSurfaceArea>0. )
      printF("surfaceArea =%e before any volume integrals, error=%12.4e\n",surfaceArea,surfaceArea-exactSurfaceArea);
    else
      printF("surfaceArea =%e before any volume integrals\n",surfaceArea);
    
    if( checkVolumes )
    {
      volume = integrate.volumeIntegral(u);

      if( exactVolume>0. )
	printF("volume = %e, error=%12.4e \n",volume,volume-exactVolume);
      else
	printF("volume = %e \n",volume);

      surfaceArea = integrate.surfaceIntegral(u);

      printF("surfaceArea after volume integral = %e \n",surfaceArea);
    
      if( gridName[it](0,2)=="cic" )
      {
	printF("Error in volume = %e \n", fabs(volume-( 4.*4.-Pi*SQR(.5) )) );
	printF("Error in surface area = %e \n",fabs(surfaceArea-( 4.*4 + Pi)));
      }
      else if( gridName[it](0,2)=="sib" )
      {
	printF("Error in volume = %e \n", fabs(volume-( 4.*4.*4.-4./3.*Pi*.5*.5*.5 )) );
	printF("Error in surface area = %e \n",fabs(surfaceArea-( 4.*4.*6 + 4.*Pi*SQR(.5) )));
      }
    }
    
    // integrate.leftNullVector();
    
    if( useHybrid && cg.numberOfDimensions()==3 )
    {
      // plot the grid so we can see the hybrid surface grid
      if( integrate.getSurfaceStitcher()!=NULL )
      {

	psp.set(GI_PLOT_UNS_EDGES,true);
	psp.set(GI_PLOT_UNS_FACES,true);

	SurfaceStitcher & stitcher = *integrate.getSurfaceStitcher();
	if( stitcher.getSurfaceCompositeGrid()!=NULL )
	{
	  PlotIt::plot(ps,*stitcher.getSurfaceCompositeGrid(),psp);
	}
      }
      else
      {
	PlotIt::plot(ps,cg,psp);
      }
    } // end if useHybrid


    if( checkDataBase )
    {
      // Save the Integrate class to a data base file, then read back in and check the results

      aString fileName="IntegrateTestFile.hdf";
      if( true )
      {
	HDF_DataBase db;
	printF("Saving the Integrate class in file=[%s]\n",(const char*)fileName);
	db.mount(fileName,"I");

	integrate.put( db, "Integrate" );
      
	db.unmount();
      }
      
      Integrate integrate(cg);  // here is a *new* version 

      HDF_DataBase db;
      printF("Read the Integrate class from file=[%s]\n",(const char*)fileName);
      db.mount(fileName,"R");

      integrate.get( db, "Integrate" );
      
      db.unmount();
      
      // realCompositeGridFunction & weights = integrate.integrationWeights();
      // weights.display("integrationWeights from file","%6.2f ");

      Integrate::debug=3;

      volume = integrate.volumeIntegral(u);
      printF("volume = %e (computed using DataBase version)\n",volume);

      surfaceArea = integrate.surfaceIntegral(u);

      printF("surfaceArea after volume integral = %e (computed using DataBase version)\n",surfaceArea);

      if( gridName[it].matches("cic") &&  !gridName[it].matches("cic2") )
      {
        // check that the surface=0 is still known
	int surface=0;
	surfaceArea = integrate.surfaceIntegral(u,surface);
	printF("Grid cic: surfaceArea for cylinder = %e, error=%e (computed using DataBase version)\n",surfaceArea,fabs(surfaceArea-Pi));

      }
    } // end checkDataBase
    

  }
  


  Overture::finish();          
  return 0;
}
