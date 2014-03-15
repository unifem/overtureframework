//
// Test CompositeSurface
//

#include "Mapping.h"
#include "CompositeSurface.h"
#include "TrimmedMapping.h"
#include "NurbsMapping.h"
#include "CircleMapping.h"
#include "SphereMapping.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"
#include "LineMapping.h"
#include "MappingRC.h"

int initializeMappingList();


int
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test CompositeSurface =====" << endl;

  PlotStuff ps;
  GraphicsParameters params;

  if( FALSE )
  {
    CompositeSurface cs;
    NurbsMapping nurbs(2,3);
    // params.set(GI_TOP_LABEL,"nurbs");
    // PlotIt::plot(ps,nurbs,params);

    CircleMapping outerBoundary(.5,.5,.45); // radius=.45
    CircleMapping innerBoundary1(.40,.40,.13);
    CircleMapping innerBoundary2(.60,.60,.13,-.13);  // reverse tangent
    Mapping *inner[3];
    int numberOfInnerCurves=2;
    inner[0]=&innerBoundary1;
    inner[1]=&innerBoundary2;

    TrimmedMapping trim(nurbs,&outerBoundary,numberOfInnerCurves,inner);

    // params.set(GI_TOP_LABEL,"trimmed mapping");
    // PlotIt::plot(ps,trim,params);
  
//    SphereMapping sphere;
//    sphere.setDomainDimension(2);
    
//    cs.add(sphere);
    cs.add(trim);
    
    params.set(GI_TOP_LABEL,"CompositeSurface");
    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    printf("plot the cs...\n");
    PlotIt::plot(ps,cs,params);

    initializeMappingList();
  
    printf("mount a file and save the cs...\n");
    HDF_DataBase db;
    db.mount("csTemp.hdf","I");
//    db.setMode(GenericDataBase::noStreamMode);
  
    cs.put(db,"cs");
    db.unmount();

    HDF_DataBase db2;
    db2.mount("csTemp.hdf","R");
  
    CompositeSurface cs2;
    cs2.get(db2,"cs");
    params.set(GI_TOP_LABEL,"CompositeSurface from the data base");
    PlotIt::plot(ps,cs,params);
    


    return 0;
  }

  aString csName="slac1.hdf";
  
  // read a CompositeSurface from a data base file
  HDF_DataBase db;
  db.mount(csName,"R");
  initializeMappingList();
  cout << "read from a data base \n";
    
  MappingRC maprc;
  maprc.get(db,"compositeSurface");
  db.unmount();

  assert( maprc.getClassName()=="CompositeSurface");
  
  CompositeSurface & cs = (CompositeSurface &)maprc.getMapping();
  

/* ---
  cout << "save surface in a new data base file \n";
  HDF_DataBase db2;
  db2.mount("newCS.hdf","I");
  db2.setMode(GenericDataBase::streamOutputMode);
  cs.put(db2,"compositeSurface");
  db2.setMode(GenericDataBase::normalMode);
  db2.unmount();
--- */  
  

  params.set(GI_TOP_LABEL,"composite surface");

  PlotIt::plot(ps,cs,params);
  params.set(GI_TOP_LABEL,"");
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  params.set(GI_USE_PLOT_BOUNDS,TRUE);  // use at least the region defined by the plot bounds

//  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
//    ps.plot(cs[s],params);
  
  aString menu[] = { "project a point",
                    "draw line on surface",
		    "exit",
                    "" };
  aString answer,line;
  realArray x(1,3),rP(1,2),xP(1,3),xrP(1,3,2);
  IntegerArray subSurface(1);
  subSurface=-1;
  for(;;)
  {
    ps.getMenuItem(menu,answer);

    if( answer=="project a point" )
    {
      ps.inputString(line,"Enter (x,y,z) to project");
      if( line!="" ) sScanF(line,"%e %e %e ",&x(0,0),&x(0,1),&x(0,2));
      printf(" (x,y,z) = (%e,%e,%e) \n",x(0,0),x(0,1),x(0,2));
      cs.project( subSurface,x,rP,xP,xrP );
      printf(" projected point: (x,y,z) = (%e,%e,%e), subSurface=%i \n",xP(0,0),xP(0,1),xP(0,2),subSurface(0));
      LineMapping line(x(0,0),x(0,1),x(0,2),xP(0,0),xP(0,1),xP(0,2),3);

      params.set(GI_MAPPING_COLOUR,"black");
      params.set(GraphicsParameters::lineWidth,3.);
      PlotIt::plot(ps,line,params);
      
    }
    else if( answer=="draw line on surface" )
    {
      Range Axes(0,1), xAxes(0,2);
      realArray normal(3),x0(1,3);
      real eps=.0;
      
      params.set(GI_MAPPING_COLOUR,"black");
      params.set(GraphicsParameters::lineWidth,3.);

      RealArray plane(3), dir(3);
      plane(0)=1.;
      plane(1)=0.;
      plane(2)=.5;

      ps.inputString(line,"Enter the starting point (x,y,z) and numberOfSteps");
      int numberOfSteps=10;
      if( line!="" ) sScanF(line,"%e %e %e %i",&x(0,0),&x(0,1),&x(0,2),&numberOfSteps);
      real dr=.01;
      x0=x;  // save old value
      real t1Dotp,t2Dotp;
      for( int step=0; step<numberOfSteps; step++ )
      {
        printf(" ---------\n step=%i, (x,y,z) = (%e,%e,%e) \n",step,x(0,0),x(0,1),x(0,2));
        cs.project( subSurface,x,rP,xP,xrP );

        // **** need to keep normal consistent across patches ********
        normal(axis1)=xrP(0,1,0)*xrP(0,2,1)-xrP(0,2,0)*xrP(0,1,1);
        normal(axis2)=xrP(0,2,0)*xrP(0,0,1)-xrP(0,0,0)*xrP(0,2,1);
        normal(axis3)=xrP(0,0,0)*xrP(0,1,1)-xrP(0,1,0)*xrP(0,0,1);

        real norm=SQRT( SQR(normal(0))+SQR(normal(1))+SQR(normal(2)));
	normal/=norm;
        printf(" projected point: (x,y,z) = (%e,%e,%e), subSurface=%i, normal=(%e,%e,%e) \n",
          xP(0,0),xP(0,1),xP(0,2),subSurface(0),normal(0),normal(1),normal(2));

        LineMapping line(x0(0,0)+eps*normal(0),x0(0,1)+eps*normal(1),x0(0,2)+eps*normal(2),
                         xP(0,0)+eps*normal(0),xP(0,1)+eps*normal(1),xP(0,2)+eps*normal(2),3);

        PlotIt::plot(ps,line,params);
        ps.redraw(TRUE);
        // step along average of tangents:
        x0=xP;
        xrP.reshape(3,2);

	// direction, dir, is in the tangent plane and on the plane plane(0:2)
	// dir(0)=-1.; dir(1)=0.; dir(2)=2.;
        t1Dotp=xrP(0,0)*plane(0)+xrP(1,0)*plane(1)+xrP(2,0)*plane(2);
        t2Dotp=xrP(0,1)*plane(0)+xrP(1,1)*plane(1)+xrP(2,1)*plane(2);
        dir(xAxes)= +t2Dotp*xrP(xAxes,0)-t1Dotp*xrP(xAxes,1);

        if( subSurface(0)==5 )
          dir=-dir;

/* ----
  	  // project current direction onto tangent plane
          t1Dotp=xrP(0,0)*dir(0)+xrP(1,0)*dir(1)+xrP(2,0)*dir(2);
          t2Dotp=xrP(0,1)*dir(0)+xrP(1,1)*dir(1)+xrP(2,1)*dir(2);

        real norm1= SQR(xrP(0,0))+SQR(xrP(1,0))+SQR(xrP(2,0));
        real norm2= SQR(xrP(0,1))+SQR(xrP(1,1))+SQR(xrP(2,1));
	
	dir(xAxes)= t1Dotp*xrP(xAxes,0)/norm1+t2Dotp*xrP(xAxes,1)/norm2;
	norm=SQRT( SQR(dir(0))+SQR(dir(1))+SQR(dir(2)));
        if( norm!=0. )
          dir/=norm;
        else
          dir=1.;
--- */
        printf(" dir=(%e,%e,%e) \n",dir(0),dir(1),dir(2));

        x(0,0)=xP(0,0)+dir(0)*dr;
        x(0,1)=xP(0,1)+dir(1)*dr;
        x(0,2)=xP(0,2)+dir(2)*dr;

        xrP.reshape(1,3,2);
	
      }
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  return 0;
}
