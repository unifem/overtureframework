//
// Test the fillet mapping
//

#include "MappingInformation.h"
#include "FilletMapping.h"
#include "LineMapping.h"
#include "CircleMapping.h"
#include "PlaneMapping.h"
#include "Sphere.h"
#include "CylinderMapping.h"
#include "MatrixTransform.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"

int initializeMappingList();

int
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test FilletMapping =====" << endl;

  PlotStuff ps;
  GraphicsParameters params;

  LineMapping line1(0.,0., 1.,1.);
  LineMapping line2(0.,1., 1.,0.);

  CircleMapping circle1(0.,0.,1.,1.);
  CircleMapping circle2(.5,0.,1.,1.);
  

  PlaneMapping plane1(0.,0.,0., 0.,0.,1., 1.,1.,1. );
  PlaneMapping plane2(0.,1.,0., 0.,1.,1., 1.,0.,1. );
  
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


  MappingInformation mapInfo;
  mapInfo.graphXInterface=&ps;
  

  FilletMapping fillet;
  
  aString answer,answer2;
  aString menu[] = { "line-line",
                    "circle-circle",
                    "plane-plane",
                    "sphere-sphere",
                    "cylinder-cylinder (smaller in larger)",
                    "cylinder-cylinder (same size)",
		    "sphere-cylinder",
                    "erase",
		    "exit",
                    "" };
  for(;;)
  {
    ps.getMenuItem(menu,answer);


    if( answer=="line-line" )
    {
      fillet.setCurves(line1,line2);
      fillet.update(mapInfo);
      
    }
    else if( answer=="circle-circle" )
    {
      fillet.setCurves(circle1,circle2);
      fillet.update(mapInfo);
    }
    else if( answer=="plane-plane" )
    {
      fillet.setCurves(plane1,plane2);
      fillet.update(mapInfo);
    }
    else if( answer=="sphere-sphere" )
    {
      fillet.setCurves(s1,s2);
      fillet.update(mapInfo);
    }
    else if( answer=="cylinder-cylinder (smaller in larger)" )
    {
      fillet.setCurves(c1,c2);
      fillet.update(mapInfo);
    }
    else if( answer=="cylinder-cylinder (same size)" )
    {
      fillet.setCurves(c1,c3);
      fillet.update(mapInfo);
    }
    else if( answer=="sphere-cylinder" )
    {
      fillet.setCurves(s1,c1);
      fillet.update(mapInfo);
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
