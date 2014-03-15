// 2013/01/26 WDH
// 
// Query an unstructured surface (or Mapping curve or surface ) to determine if points are inside or outside
//
// Examples:
//   testInsideOutside insideOutside.cmd
//   testInsideOutside insideOutsideCurve.cmd

#include "Mapping.h"
#include "NurbsMapping.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"
#include "MappingRC.h"
#include "MappingProjectionParameters.h"
#include "MappingInformation.h"
// #include "CompositeTopology.h"
#include "UnstructuredMapping.h"
#include "Inverse.h"

// #include "CompositeSurface.h"

int initializeMappingList();
int destructMappingList();

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  printF("====== testInsideOutside =====\n");

  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="noplot" || line=="nopause" || line=="abortOnEnd" )
        continue;
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
  {
    printF("Usage: `testInsideOutside [file.cmd]' \n"
           "          file.cmd: read this command file \n");
  }
  

  PlotStuff ps;
  GraphicsParameters params;

  params.set(GI_PLOT_UNS_EDGES,true);
  params.set(GI_PLOT_UNS_BOUNDARY_EDGES,true);
  params.set(GI_PLOT_BLOCK_BOUNDARIES,false);

  // By default start saving the command file called "testInsideOutside.cmd"
  aString logFile="testInsideOutside.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);


  UnstructuredMapping uMap;
  NurbsMapping nurbs;

  Mapping *pMap;

  pMap= &uMap;
  
  
  params.set(GI_TOP_LABEL,"Test Inside Outside");
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  aString menu[] = { "build unstructured surface",
                     "build curve",
                     "project a point",
		     "exit",
                    "" };
  aString answer,line;

  // This next object is used to pass and return additional parameters to the "project" function.
  MappingProjectionParameters mpParams;
  typedef MappingProjectionParameters MPP;
  realArray & surfaceNormal  = mpParams.getRealArray(MPP::normal);
  intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  intArray & elementIndex    = mpParams.getIntArray(MPP::elementIndex);
  realArray & xOld           = mpParams.getRealArray(MPP::x);  // this could be used as an initial guess

  // Allocate space for the normal to indicate that it should be computed
  int numberOfPointsToProject=1;  // we only project 1 point at a time here
  surfaceNormal.redim(numberOfPointsToProject,3);
  surfaceNormal=0.;       

  RealArray x(1,3),xP(1,3); // arrays to hold initial and projected points
  x=0.;
  
  for(;;)
  {
    Mapping & map = *pMap;

    PlotIt::plot(ps,map,params);      // plot the surface

    ps.getMenuItem(menu,answer);

    if( answer=="build unstructured surface" )
    {
      pMap = & uMap;
      MappingInformation mapInfo;
      mapInfo.graphXInterface=&ps;
      mapInfo.gp_=&params;
      pMap->update(mapInfo);

      params.set(GI_PLOT_UNS_EDGES,true);
      params.set(GI_PLOT_UNS_BOUNDARY_EDGES,true);
    }
    else if( answer=="build curve" )
    {
      pMap = &nurbs;  // make active mapping a Nurbs

      MappingInformation mapInfo;
      mapInfo.graphXInterface=&ps;
      mapInfo.gp_=&params;
      pMap->update(mapInfo);

      params.set(GI_PLOT_UNS_EDGES,false);
      params.set(GI_PLOT_UNS_BOUNDARY_EDGES,false);
    }
    else if( answer=="project a point" )
    {
      if( map.getClassName()=="UnstructuredMapping" )
      {
	ps.inputString(line,"Enter (x,y,z) to project");
	if( line!="" ) sScanF(line,"%e %e %e ",&x(0,0),&x(0,1),&x(0,2));

	xP=x;
	elementIndex=-1;     // this means we have no guess at the previous element.

	map.project( xP,mpParams );  // project xP to the closest point on the surface

	// compute the distance to the surface 
	real dist = sqrt( SQR(xP(0,0)-x(0,0))+SQR(xP(0,1)-x(0,1))+SQR(xP(0,2)-x(0,2)) );
	// the dot-product will indicate whether we are inside or outside
	real dot = (xP(0,0)-x(0,0))*surfaceNormal(0,0) +
	  (xP(0,1)-x(0,1))*surfaceNormal(0,1) +
	  (xP(0,2)-x(0,2))*surfaceNormal(0,2);
	dot = dot/max(REAL_MIN*100.,dist);
       
	printf("\n >>> point=(%8.2e,%8.2e,%8.2e), projected point=(%8.2e,%8.2e,%8.2e), \n"
	       "       surfaceNormal=(%8.2e,%8.2e,%8.2e), dist=%8.2e, dot=%8.2e subSurface=%i \n",
	       x(0,0),x(0,1),x(0,2),xP(0,0),xP(0,1),xP(0,2),
	       surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2),
	       dist,dot, subSurfaceIndex(0));

        IntegerArray inside(1); inside=-1;
	uMap.insideOrOutside(x,inside);
        if( inside(0)==1 )
	{
	  printf(" ***The point is inside the triangulation***\n");
	}
	else if( inside(0)==0 )
	{
	  printf(" ***The point is outside the triangulation***\n");
	}
        else
	{
          printf(" ***ERROR determining whether the point is inside or outside the triangulation***\n");
	}
      }
      else
      {
	// -- 2D curve: 

	ps.inputString(line,"Enter (x,y) to project");
	if( line!="" ) sScanF(line,"%e %e",&x(0,0),&x(0,1));

	RealArray r(1,3);
	r=-1.;
	map.inverseMap(x,r);
	if( r(0,0)!=Mapping::bogus )
	{
	  map.map(r,xP);
	}
	
	printF("Closest point to x=(%9.3e,%9.3e) is xP=(%9.3e,%9.3e) at r=(%9.3e,%9.3e)\n",x(0,0),x(0,1),
	       xP(0,0),xP(0,1),r(0,0),r(0,1));
	
	assert( map.approximateGlobalInverse !=NULL );
	IntegerArray cross(1);  // ****** fix this ******
	cross=0;
	map.approximateGlobalInverse->countCrossingsWithPolygon( x,cross );
        int inside = (cross(0) % 2 == 0) ? -1 : +1;
        if( inside==1 )
	{
	  printF(" ***The point is inside the curve***\n");
	}
	else 
	{
	  printF(" ***The point is outside the curve***\n");
	}

      }
      

      // Now plot the two points and a line joining them
      ps.erase();

      params.set(GI_MAPPING_COLOUR,"black");
      params.set(GraphicsParameters::curveLineWidth,3.);
      realArray segments(1,3,2);
      Range R3=3;
      segments(0,R3,0)=x(0,R3);  // start point of line segment
      segments(0,R3,1)=xP(0,R3);  // end pt of line segment
      ps.plotLines(segments, params);

      params.set(GI_POINT_SIZE, (real) 6.0);
      params.set(GI_POINT_COLOUR, "green");
      ps.plotPoints(x,params);
      params.set(GI_POINT_COLOUR, "red");
      ps.plotPoints(xP,params);

      // -- reset
      params.set(GraphicsParameters::curveLineWidth,1.);
      params.set(GI_POINT_SIZE, (real) 3.0);

    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  // clean up:
  Overture::finish();
  return 0;
}
