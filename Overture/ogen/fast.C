//  gridSEGV --- causes an abort in A++
//
//  $Id: fast.C,v 1.4 2002/01/23 01:32:03 andersp Exp $

#include "Ogen.h"
#include "PlotStuff.h"
#include "Ogshow.h"
#include "CircleMapping.h"
#include "Annulus.h"
#include "HyperbolicMapping.h"
#include "TFIMapping.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int keepOrigOutbub=0;
  cout << "--- Options 1, 2 will crash A++:" << endl;
  cout << "Keep original outerbubble(0), keep orig. inner(1), or use the annulus(2)?" << endl;
  cin >> keepOrigOutbub;

  int ndim=32, across=11, debug=0;
  int mode=5;
  real modeAmpl=.3;
  Range all;

  enum BubbleBCTypes {
    interpBC=0, innerBC, outerBC
  }; 

  PlotStuff ps(TRUE,"Moving Local Bubble Example");
  PlotStuffParameters psp;
  char buff[80];

  MappingInformation mappingInfo;              // parameters used by map.update
  mappingInfo.graphXInterface=&ps;             // pass graphics interface


  //GRID stuff ==============================================
  CompositeGrid cg[2];
 
  CircleMapping outerBdry(0,0,5,5);
  getFromADataBase(cg[0],"stir.hdf");
  cg[1]=cg[0];


  // ..Create the mappings
  cout<< "Creating innerbdry...\n" ;
  AnnulusMapping anInBub0;
  anInBub0.setRadii(.1,.3);
  anInBub0.setBoundaryCondition(1,1,interpBC); 
  anInBub0.setBoundaryCondition(0,1,innerBC);  
  anInBub0.setGridDimensions(axis1,ndim);
  anInBub0.setGridDimensions(axis2,across);
  AnnulusMapping anInBub1=anInBub0;


  cout<< "Creating outerbdry...\n" ;

  AnnulusMapping anOutBub;
  anOutBub.setRadii(.2,2.);
  anOutBub.setName(Mapping::mappingName, "outerBoundaryGrid");
  anOutBub.setBoundaryCondition(0,1,interpBC); // inner bdry (=interp)
  anOutBub.setBoundaryCondition(1,1,outerBC);  // outer bdry
  anOutBub.setGridDimensions(axis1,ndim);
  anOutBub.setGridDimensions(axis2,across);
  

  cout<< "Changing the mappings in the composite grid...\n";

  if (keepOrigOutbub == 2) {   // change outer bubble to an annulus

    cg[0][1].reference(anInBub0);  // change inner bubble 
    cg[1][1].reference(anInBub1);

    cg[0][0].reference(anOutBub);  // change outer bubble
    cg[1][0].reference(anOutBub);

    cg[0].update();
    cg[1].update();

    ps.erase();
    psp.set(GI_TOP_LABEL,sPrintF(buff,"The grid before updating Overlap"));
    PlotIt::plot(ps,cg[1],psp);                          // plot the grid

  } else if (keepOrigOutbub == 1) {

    cg[0][0].reference(anInBub0);  // change inner bubble & update cg's
    cg[1][0].reference(anInBub1); 

    cg[0].update();
    cg[1].update();

  } else if (keepOrigOutbub == 0) {

    cg[0][1].reference(anInBub0);  // change inner bubble & update cg's
    cg[1][1].reference(anInBub1); 

    cg[0].update();
    cg[1].update();

  }  


  cg[1].destroy(CompositeGrid::EVERYTHING);

  LogicalArray hasMoved(2);
  hasMoved = LogicalTrue;

  // ----------The Overlapping Grid Generator, & a show file
  Ogen gridGenerator(ps);

  cout << "About to `updateOverlap'..." << endl;
  gridGenerator.updateOverlap(cg[1],cg[0],hasMoved);
//  gridGenerator.updateOverlap(cg[1],cg[0],hasMoved,Ogen::useFullAlgorithm);


  cout << "OK! --- overlap computed" << endl;

  ps.erase();
  psp.set(GI_TOP_LABEL,sPrintF(buff,"-- Generated the Overlapping Grid --"));
  PlotIt::plot(ps,cg[1],psp);                          // plot the grid

  return 0;
}
