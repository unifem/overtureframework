//
// Test the intersection mapping
//
#include "IntersectionMapping.h"
#include "SphereMapping.h"
#include "CylinderMapping.h"
#include "MatrixTransform.h"
#include "PlaneMapping.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"

int initializeMappingList();

int
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture


  cout << "====== Test IntersectionMapping =====" << endl;

  int plotOption=true;
  if( argc > 1 )
  { // look at arguments for "noplot" 
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="noplot" )
        plotOption=false;
    }
  }

  PlotStuff ps(plotOption);
  GraphicsParameters params;

  SphereMapping s1(.5,1.,-.25,-.25,-.25);
  s1.setDomainDimension(2);
  SphereMapping s2(.5,1.,.25,.25,.25);
  s2.setDomainDimension(2);
  
  CylinderMapping c1(0.,1., 0.,1., .25,.75, -.25,-.25,-.25, 2);    // r0=.25. r1=.75

  // c2: smaller cylinder rotated
  CylinderMapping c20(0.,1., 0.,1., .20,.75, -.25,-.25,-.25, 2);  
  MatrixTransform c2(c20);
  c2.rotate(axis2,90.*Pi/180.);

  // c3: same size cylinder rotated
  CylinderMapping c30(0.,1., 0.,1., .25, .75, -.25,-.25,-.25, 2);    // .249 ok
  MatrixTransform c3(c30);
  c3.rotate(axis2,90.*Pi/180.);
  
  PlaneMapping p1(-1.,-1.,0., 1.,-1,0., -1.,1.,0. );

  PlaneMapping plane1(0.,0.,0., 0.,0.,1., 1.,1.,0. );
  PlaneMapping plane2(0.,1.,0., 0.,1.,1., 1.,0.,0. );

  Mapping *map1 = &s1;
  Mapping *map2 = &s2;
  

  RealArray bounds(2,3);
  bounds(Start,0)=-1.;
  bounds(End  ,0)= 1.;
  bounds(Start,1)=-1.;
  bounds(End  ,1)= 1.;
  bounds(Start,2)=-1.;
  bounds(End  ,2)= 1.;
  
  params.set(GI_PLOT_BOUNDS,bounds); // initialize the plot bounds
  params.set(GI_USE_PLOT_BOUNDS,TRUE); 
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  PlotIt::plot(ps,*map1,params);
  params.set(GI_MAPPING_COLOUR,"blue");
  PlotIt::plot(ps,*map2,params);
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

  IntersectionMapping inter;
  
  aString answer,answer2;
  aString menu[] = { "sphere-sphere",
                    "cylinder-cylinder (smaller in larger)",
                    "cylinder-cylinder (same size)",
                    "sphere-cylinder",
                    "sphere-plane (one)",
                    "plane-plane",
                    "intersect",
		    "plot intersection",
                    "reparameterize",
                    "erase",
		    "exit",
                    "" };
  for(;;)
  {
    ps.getMenuItem(menu,answer);

    if( answer=="intersect" )
    {
      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 

      inter.intersect(*map1,*map2,&ps,params);
      params.set(GI_MAPPING_COLOUR,"green");
      // params.set(GraphicsParameters::lineWidth,3.);
      // ps.erase();
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      PlotIt::plot(ps, inter,params );
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      // params.set(GraphicsParameters::lineWidth,1.);
    }
    else if( answer=="plot intersection" )
    {
      ps.erase();
      params.set(GI_MAPPING_COLOUR,"green");
      params.set(GI_TOP_LABEL,"actual intersection curve in space");
      PlotIt::plot(ps, inter,params );
      params.set(GI_USE_PLOT_BOUNDS,FALSE); 
      if( inter.rCurve1!=NULL )
      {
        params.set(GI_TOP_LABEL,"parametric curve rCurve1");
        PlotIt::plot(ps, *inter.rCurve1,params );
      }
      if( inter.rCurve2!=NULL )
      {
        params.set(GI_TOP_LABEL,"parametric curve rCurve2");
        PlotIt::plot(ps, *inter.rCurve2,params );
      }
      params.set(GI_TOP_LABEL,"");
      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 
      
    }
    else if( answer=="reparameterize" )
    {
      real arcLengthWeight=1., curvatureWeight=1.;
      inter.reparameterize(arcLengthWeight,curvatureWeight);

      ps.erase();
      params.set(GI_PLOT_BOUNDS,bounds); // initialize the plot bounds
      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

      params.set(GI_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,*map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,*map2,params);

      params.set(GI_MAPPING_COLOUR,"green");
      PlotIt::plot(ps, inter,params );
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);


    }
    else if( answer=="sphere-sphere" )
    {
      map1=&s1;
      map2=&s2;

      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,*map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,*map2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="cylinder-cylinder (smaller in larger)" )
    {
      map1=&c1;
      map2=&c2;

      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,*map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,*map2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="cylinder-cylinder (same size)" )
    {
      map1=&c1;
      map2=&c3;

      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,*map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,*map2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="sphere-cylinder" )
    {
      map1=&s1;
      map2=&c1;

      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,*map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,*map2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="sphere-plane (one)" )
    {
      map1=&s1;
      map2=&p1;

      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,*map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,*map2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="plane-plane" )
    {
      map1=&plane1;
      map2=&plane2;

      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,bounds); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,*map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,*map2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      cout << "unknown response = " << answer << endl;
    }
  }

  Overture::finish();
  Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );  
  return 0;
}
  

