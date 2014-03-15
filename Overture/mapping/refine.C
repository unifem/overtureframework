#include "Overture.h"  
#include "Annulus.h"
#include "ReparameterizationTransform.h"
//#include "PlotStuff.h"
#include "GenericGraphicsInterface.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  Mapping::debug=7; 

//  PlotStuff ps;

  AnnulusMapping map;

  MappedGrid mg(map);
  mg.update();

//  PlotIt::plot(ps,mg);

  ReparameterizationTransform refineMap( map,ReparameterizationTransform::restriction );
  refineMap.scaleBounds( .25,.75, .25,.75 );  

//  PlotIt::plot(ps,refineMap);

  ReparameterizationTransform refineMap2( refineMap,ReparameterizationTransform::restriction );
  refineMap2.scaleBounds( .25,.75, .25,.75 );  

//  PlotIt::plot(ps,refineMap2);

  Mapping *map2 = &refineMap2;
    
  ReparameterizationTransform refineMap3( *map2,ReparameterizationTransform::restriction );
  refineMap3.scaleBounds( .25,.75, .25,.75 );  

//  PlotIt::plot(ps,refineMap3);

  MappedGrid mg3(refineMap3);
  mg3.update();
//  PlotIt::plot(ps,mg3);


  // test deep copy:
  MappedGrid mg4;
  mg4=mg3;


  return(0);

}

