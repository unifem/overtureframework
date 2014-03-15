//  bugGrid --- produces invalid interpolation points at the
//              periodic boundary of the outer annulus
//
// NOTE: needs 'stir.hdf'

#include "Ogen.h"
#include "PlotStuff.h"
#include "Ogshow.h"
#include "CircleMapping.h"
#include "Annulus.h"
#include "HyperbolicMapping.h"
#include "TFIMapping.h"

int 
checkOverlappingGrid( const CompositeGrid & cg, const int & option=0);

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  int ndim=32,  across=8;    // outer
  int inDim=30, inAcross=25; // inner

  int showBug;
  cout << "\n\nSelect number of bugs; 0, 1 or 2 : ";
  cin >> showBug;

  cout << endl;
  if (showBug==1) {
    cout << "........Creating a DENSE inner grid --> 1 invalid interp. point.\n";
    inDim=30;   inAcross=8;
  } else if (showBug==2) {
    cout << "........DENSEST inner grid ---> 2 invalid interp. points.\n";
    inDim=30;   inAcross=16;
  } else {
    cout << "........Choosing inner & outer grid spacing similar ---> no problem.\n";
    inDim=30;   inAcross=5;

  }

  cout << endl;

  Range all;

  enum BubbleBCTypes {
    interpBC=0, innerBC, outerBC
  }; 

  PlotStuff ps(TRUE,"Testing interpolation weights");
  PlotStuffParameters psp;
  char buff[80];

  MappingInformation mappingInfo; 
  mappingInfo.graphXInterface=&ps;

  //GRID stuff ==============================================
  CompositeGrid cg[2];
 
  CircleMapping outerBdry(0,0,5,5);
  getFromADataBase(cg[0],"stir.hdf");
  cg[1]=cg[0];


  // ..Create the mappings
  cout<< "Creating innerbdry...\n" ;
  AnnulusMapping anInBub0;
  anInBub0.setRadii(.5,1.);
  anInBub0.setBoundaryCondition(1,1,interpBC); 
  anInBub0.setBoundaryCondition(0,1,innerBC);  
  anInBub0.setOrigin(0.2,0.2);
  anInBub0.setGridDimensions(axis1,inDim);
  anInBub0.setGridDimensions(axis2,inAcross);

  cout<< "Creating outerbdry...\n" ;

  AnnulusMapping anOutBub;
  anOutBub.setRadii(.8,2.);
  anOutBub.setName(Mapping::mappingName, "outerBoundaryGrid");
  anOutBub.setBoundaryCondition(0,1,interpBC); // inner bdry (=interp)
  anOutBub.setBoundaryCondition(1,1,outerBC);  // outer bdry
  anOutBub.setGridDimensions(axis1,ndim);
  anOutBub.setGridDimensions(axis2,across);
  
  cout<< "Changing the mappings in the composite grid...\n";

  // ----------The Overlapping Grid Generator, & a show file
  Ogen gridGenerator(ps);

  // update mappings
  cg[0][0].reference(anOutBub);  // Outer bubble(=map 0)   
  cg[1][0].reference(anOutBub);
  cg[0][1].reference(anInBub0);  // Inner bubble(=map 1) gives nice grid
  cg[1][1].reference(anInBub0);

  for( int side=Start; side<=End; side++ )
  {
    cg[0][1].numberOfGhostPoints()(side,axis1)=2;
    cg[0][1].dimension()(side,axis1)+=2*side-1;

    cg[1][1].numberOfGhostPoints()(side,axis1)=2;
    cg[1][1].dimension()(side,axis1)+=2*side-1;
  }
  cg[0].update();
  cg[1].update();

  // create grid
  LogicalArray hasMoved(2);
  hasMoved = LogicalTrue;

  cout << "About to `updateOverlap'..." << endl;
  cg[0].destroy(CompositeGrid::EVERYTHING);
  gridGenerator.updateOverlap(cg[0],cg[1],hasMoved,Ogen::useFullAlgorithm);

  cout << "OK! --- overlap computed" << endl;
  cg[1]=cg[0];

  int numErs=checkOverlappingGrid( cg[1], 0 );

  cout << endl 
       << "...HELLO,    number of errors = " << numErs << endl;

  ps.erase();
  psp.set(GI_TOP_LABEL,sPrintF(buff,"-- Generated the Overlapping Grid --"));
  PlotIt::plot(ps,cg[1],psp);                          // plot the grid

  return 0;
}
