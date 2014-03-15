#include "Overture.h"
#include "MappedGridOperators.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "display.h"
#include "Annulus.h"


//================================================================================
//  **** Test the boundary conditions *****
//================================================================================

int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture
 
  AnnulusMapping map;
  map.setGridDimensions(axis1,11);              // axis1==0, set no. of grid points
  map.setGridDimensions(axis2,5 );              // axis2==1, set no. of grid points
  MappedGrid mg(map);                           // MappedGrid for a square
  mg.update();                                       // create default variables

  Range all;
  realMappedGridFunction u;
  u.updateToMatchGrid(mg,all,all,all,2);          // define after declaration (like resize)
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components
  u.setName("v",1);                               // ...and components

  Index I1,I2,I3, Ib1,Ib2,Ib3;

  u(all,all,all,0)=1.;
  u(all,all,all,1)=2.;
  


  // set BC's
  const int outerBoundary=5;
  const int outerSide=1, outerAxis=axis2;
  
  assert( mg.boundaryCondition()(outerSide,outerAxis)>0 );
  mg.boundaryCondition()(outerSide,outerAxis)=outerBoundary;
  

  MappedGridOperators op(mg);
  

  u.setOperators(op);
  

  getBoundaryIndex(mg.gridIndexRange(),outerSide,outerAxis,Ib1,Ib2,Ib3);
  
  realArray fluxBC(Ib1,Ib2,Ib3,Range(1,1)); // ***** note last component is 1:1 
  
  display(u,"u before applyBC","%9.2e");

  fluxBC.seqAdd(0.,1.);
  u.applyBoundaryCondition(1,BCTypes::neumann, outerBoundary, fluxBC);
  

  display(u,"u after applyBC","%9.2e");



  Overture::finish();          
  cout << "Program Terminated Normally! \n";
  return 0;

}
