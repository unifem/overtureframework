//
// Test the intersection of curves
//

#include "IntersectionMapping.h"
#include "LineMapping.h"
#include "CircleMapping.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"

int initializeMappingList();

int
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test IntersectionMapping =====" << endl;

  PlotStuff ps;
  GraphicsParameters params;

  LineMapping line1(0.,0., 1.,1.);
  LineMapping line2(0.,1., 1.,0.);

  CircleMapping c1(0.,0.,1.,1.);
  CircleMapping c2(.5,0.,1.,1.);
  

  int numberOfIntersectionPoints;
  RealArray r1,r2,x;
  

  RealArray bounds(2,3);
  bounds(Start,0)=-1.;
  bounds(End  ,0)= 1.;
  bounds(Start,1)=-1.;
  bounds(End  ,1)= 1.;
  bounds(Start,2)=-1.;
  bounds(End  ,2)= 1.;
  
  params.set(GI_SET_PLOT_BOUNDS,bounds); // initialize the plot bounds
  params.set(GI_USE_PLOT_BOUNDS,TRUE); 
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  params.set(GI_USE_PLOT_BOUNDS,TRUE); 
  params.set(GI_SET_PLOT_BOUNDS,bounds); 

  IntersectionMapping inter;
  
  aString answer,answer2;
  aString menu[] = { "line-line",
                    "circle-circle",
                    "erase",
		    "exit",
                    "" };
  for(;;)
  {
    ps.getMenuItem(menu,answer);


    if( answer=="line-line" )
    {
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_SET_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,line1,params);
      params.set(GI_SET_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,line2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

      inter.intersectCurves(line1,line2,numberOfIntersectionPoints,r1,r2,x);
      for( int i=0; i<numberOfIntersectionPoints; i++ )
      {
        printf("intersection point %i : r1=%e, r2=%e, x=(%e,%e) \n",i,r1(i),r2(i),x(0,i),x(1,i));
      }
    }
    else if( answer=="circle-circle" )
    {
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      params.set(GI_SET_MAPPING_COLOUR,"red");
      PlotIt::plot(ps,c1,params);
      params.set(GI_SET_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,c2,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

      inter.intersectCurves(c1,c2,numberOfIntersectionPoints,r1,r2,x);
      for( int i=0; i<numberOfIntersectionPoints; i++ )
      {
        printf("intersection point %i : r1=%e, r2=%e, x=(%e,%e) \n",i,r1(i),r2(i),x(0,i),x(1,i));
      }
    }
    else if( answer=="erase")
    {
      ps.erase();
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

  return 0;
}
