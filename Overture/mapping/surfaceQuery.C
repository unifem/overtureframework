//
// Query a surface to determine if points are inside or outside
//

#include "Mapping.h"
#include "CompositeSurface.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"
#include "MappingRC.h"
#include "MappingProjectionParameters.h"
#include "MappingInformation.h"
#include "CompositeTopology.h"
#include "UnstructuredMapping.h"

int initializeMappingList();
int destructMappingList();

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  cout << "====== surfaceQuery =====" << endl;

  PlotStuff ps;
  GraphicsParameters params;

  aString csName="slac1.hdf";
  
  // read a CompositeSurface from a data base file
  cout << "read from a data base \n";

  HDF_DataBase db;
  db.mount(csName,"R");
  initializeMappingList();  // This allows us to read and build different types of Mapping's
    
  MappingRC maprc;  // The Mapping is saved in the file as a reference counted Mapping
  maprc.get(db,"compositeSurface");  // get the Mapping named "compositeSurface"
  db.unmount();

  // We could cast to a CompositeSurface but there is no need for the things we do here
  //    assert( maprc.getClassName()=="CompositeSurface");
  //    CompositeSurface & cs = (CompositeSurface &)maprc.getMapping();

  // Just get a Generic Mapping
  Mapping & map = maprc.getMapping();
  
  params.set(GI_TOP_LABEL,"Surface Query");
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  aString menu[] = { "project a point",
                     "edit surface",
		     "exit",
                    "" };
  aString answer,line;

  // This next object is used to pass and return additional parameters to the "project" function.
  MappingProjectionParameters mpParams;
  typedef MappingProjectionParameters MPP;
  realArray & surfaceNormal  = mpParams.getRealArray(MPP::normal);
  intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  realArray & xOld           = mpParams.getRealArray(MPP::x);  // this could be used as an initial guess

  // Allocate space for the normal to indicate that it should be computed
  int numberOfPointsToProject=1;  // we only project 1 point at a time here
  surfaceNormal.redim(numberOfPointsToProject,3);
  surfaceNormal=0.;       

  realArray x(1,3),xP(1,3); // arrays to hold initial and projected points
  x=0.;
  
  for(;;)
  {

    PlotIt::plot(ps,map,params);      // plot the surface

    ps.getMenuItem(menu,answer);

    if( answer=="project a point" )
    {
      ps.inputString(line,"Enter (x,y,z) to project");
      if( line!="" ) sScanF(line,"%e %e %e ",&x(0,0),&x(0,1),&x(0,2));

      if( subSurfaceIndex.getLength(0)>0 )
        subSurfaceIndex=-1; // this will force a global search

      xP=x;
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

      if( map.getClassName()=="CompositeSurface" )
      {
	CompositeSurface & cs = (CompositeSurface &)map;
        IntegerArray inside(1); inside=-1;
	cs.insideOrOutside(x,inside);
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

    }
    else if( answer=="edit surface" )
    {
      // edit the surface (to change the plotting, for example).
      MappingInformation mapInfo;
      mapInfo.graphXInterface=&ps;
      mapInfo.gp_=&params;
      map.update(mapInfo);
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  // clean up:
  destructMappingList();
  Overture::finish();
  return 0;
}
