//
// Test TrimmedMapping
//

#include "Mapping.h"
#include "NurbsMapping.h"
// #include "TrimmedMapping.h"
#include "TrimmedMapping.h"
#include "CircleMapping.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"
#include "MappingInformation.h"

int initializeMappingList();

int
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test TrimmedSurface =====" << endl;

  PlotStuff ps;
  GraphicsParameters params;
  Mapping::debug=1;

  if( FALSE )
  { // read a trimmed mapping from a data base file
    HDF_DataBase db;
    db.mount("trim.hdf","R");
    initializeMappingList();
    TrimmedMapping trim;
    cout << "read from a data base \n";
    
    trim.get(db,"trim");
    // trim.setGridDimensions(axis1,31);
    // trim.setGridDimensions(axis2,31);
    // trim.reinitialize();

//    trim.numberOfInnerCurves=0; // ********
//    trim.outerCurve=NULL;
//    trim.createTrimmedSurface();

    params.set(GI_TOP_LABEL,"surface in TrimmedMapping");
    PlotIt::plot(ps,*(trim.surface),params);

    if( trim.outerCurve!=NULL )
    {
      params.set(GI_TOP_LABEL,"outer curve in TrimmedMapping");

      if( FALSE )
      {
	NurbsMapping & map = (NurbsMapping &) *trim.outerCurve;
	realArray r(1,1),x(1,2),y(1,2),xr(1,2,1);

	map.display();
	Mapping::debug=63;
	for( int j=0; j<5; j++ )
	{
	  cout << "Enter x0,x1 for inverse \n";
	  cin >> x(0,0) >> x(0,1);
	  map.inverseMap(x,r);
	  map.map(r,y,xr);
	  printf(" r=%e, y=(%e,%e), xr=(%e,%e) \n",r(0,0),y(0,0),y(0,1),xr(0,0,0),xr(0,1,0));

	}
	Mapping::debug=0;
      }

      PlotIt::plot(ps,*(trim.outerCurve),params);
    }
    for( int i=0; i<trim.numberOfInnerCurves; i++ )
    {
      params.set(GI_TOP_LABEL,"inner trimming curve");
      cout << "trim.innerCurve[i].getIsPeriodic = " << trim.innerCurve[i]->getIsPeriodic(axis1) << endl;

      if( FALSE )
      {
	Mapping & map = *trim.innerCurve[i];
	realArray r(1,1),x(1,2);

	Mapping::debug=63;
	for( int j=0; j<5; j++ )
	{
	  cout << "Enter x0,x1 for inverse \n";
	  cin >> x(0,0) >> x(0,1);
	  map.inverseMap(x,r);
	  r.display("Here is r");
	}
	Mapping::debug=0;
      }
      

      PlotIt::plot(ps,*(trim.innerCurve[i]),params);
    }
    

    params.set(GI_TOP_LABEL,"TrimmedMapping from the data base");
    PlotIt::plot(ps,trim,params);

    return 0;
  }
  



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
//TrimmedMapping trim(nurbs,NULL,numberOfInnerCurves,inner);

  MappingInformation mapInfo;
  mapInfo.graphXInterface=&ps;

  trim.update(mapInfo);


  ps.erase();
  params.set(GI_TOP_LABEL,"plot trimmed mapping");
  PlotIt::plot(ps,trim,params);

  
  initializeMappingList();
  
  HDF_DataBase db;
  db.mount("trimTemp.hdf","I");
  
  trim.put(db,"trim");
  db.unmount();
  db.mount("trimTemp.hdf","R");
  
  TrimmedMapping trim2;
  trim2.get(db,"trim");
  ps.erase();
  params.set(GI_TOP_LABEL,"TrimmedMapping from the data base");
  PlotIt::plot(ps,trim2,params);
  
  return 0;
}
