
//===============================================================================
//   Driver Program for Ogmg: Overlapping Grid Multigrid Solver
//   ----------------------------------------------------------
// 
//==============================================================================

#include "Overture.h"  
#include "CompositeGridOperators.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "PlotStuff.h"

int 
main()
{
  
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    
  printf(" ------------------------------------------------------------ \n");
  printf(" Test the multigridLevel's of a grid function \n");
  printf(" ------------------------------------------------------------ \n");

  aString nameOfOGFile;
  cout << "Enter the name of the overlapping grid data base file " << endl;
  cin >> nameOfOGFile;
  nameOfOGFile="/users/henshaw/res/cgsh/"+nameOfOGFile;
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  cout << "cg.numberOfGrids = " << cg.numberOfGrids() << endl;
  cout << "cg.numberOfComponentGrids = " << cg.numberOfComponentGrids() << endl;
  
  char buff[80];
  for( int level=0; level<cg.numberOfMultigridLevels(); level++ )
  {
    cout << "cg.multigridLevel[level].numberOfGrids() = " << cg.multigridLevel[level].numberOfGrids() << endl;
    cout << "cg.multigridLevel[level].numberOfComponentGrids() = " << cg.multigridLevel[level].numberOfComponentGrids() << endl;
    // cg.multigridLevel[level][0].indexRange().display("cg.multigridLevel[level][0].indexRange()");
    //cg.multigridLevel[level][1].indexRange().display("cg.multigridLevel[level][1].indexRange()");
    
    // cg.multigridLevel[level].numberOfComponentGrids()=cg.multigridLevel[level].numberOfGrids();
/* ---
    if( cg.numberOfComponentGrids()>0 )
    {
      cg.multigridLevel[level].interpolationPoint[0].display(sPrintF(buff,"interpolationPoint, level=%i \n",level));
      cg.interpoleeGrid[0].display("cg.interpoleeGrid[0]");
      cg.multigridLevel[level].interpoleeGrid[0].display("cg.multigridLevel[level].interpoleeGrid[0]");
      cg.interpoleeGrid[1].display("cg.interpoleeGrid[1]");
      cg.multigridLevel[level].interpoleeGrid[1].display("cg.multigridLevel[level].interpoleeGrid[1]");

      cg.interpoleeLocation[0].display("cg.interpoleeLocation[0]");
      cg.multigridLevel[level].interpoleeLocation[0].display("cg.multigridLevel[level].interpoleeLocation[0]");
      
    }
--- */
  }
  
  cout << "cg.computedGeometry() & GridCollection::THEmultigridLevel = " <<
    (cg.computedGeometry() & GridCollection::THEmultigridLevel) << endl;

  PlotStuff ps(TRUE,"MG test");
  PlotStuffParameters psp;
  

  if( FALSE )
  {
    psp.set(GI_TOP_LABEL,"cg");
    PlotIt::plot(ps,cg,psp);

    for( level=0; level<cg.numberOfMultigridLevels(); level++ )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buff,"cg.multigridLevel[%i]",level));
      PlotIt::plot(ps,cg.multigridLevel[level],psp);
    }
  }
  

//  realCompositeGridFunction u;
//  u.updateToMatchGrid(cg);

  realCompositeGridFunction u(cg);

  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfGrids(); grid++ )
  {
    getIndex(cg[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(cg[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(cg[grid].vertex()(I1,I2,I3,axis2)*Pi);
  }
  
  psp.set(GI_TOP_LABEL,"u");
  PlotIt::contour(ps,u,psp);    

  for( level=0; level<cg.numberOfMultigridLevels(); level++ )
  {
    cout << "u.multigridLevel[level].numberOfComponentGrids() = " << 
             u.multigridLevel[level].numberOfComponentGrids() << endl;
    cout << "(floatGridCollectionFunction&)u.multigridLevel[level].numberOfGrids() = " << 
             ((floatGridCollectionFunction&)u).multigridLevel[level].numberOfGrids() << endl;
    
    psp.set(GI_TOP_LABEL,sPrintF(buff,"u.multigridLevel[%i]",level));
    PlotIt::contour(ps,u.multigridLevel[level],psp);
  }
  
  cout << "test a deep copy, v=u \n";
  realCompositeGridFunction v;
  v=u;
  
  psp.set(GI_TOP_LABEL,"v");
  PlotIt::contour(ps,v,psp);    
  for( level=0; level<cg.numberOfMultigridLevels(); level++ )
  {
    cout << "v.multigridLevel[level].numberOfComponentGrids() = " << 
             v.multigridLevel[level].numberOfComponentGrids() << endl;
    psp.set(GI_TOP_LABEL,sPrintF(buff,"v.multigridLevel[%i]",level));
    PlotIt::contour(ps,v.multigridLevel[level],psp);
  }

  cout << "test a referece  w.reference(u) \n";
  realCompositeGridFunction w;
  w.reference(u);
  
  psp.set(GI_TOP_LABEL,"w");
  PlotIt::contour(ps,w,psp);    
  for( level=0; level<cg.numberOfMultigridLevels(); level++ )
  {
    cout << "w.multigridLevel[level].numberOfComponentGrids() = " << 
             w.multigridLevel[level].numberOfComponentGrids() << endl;
    psp.set(GI_TOP_LABEL,sPrintF(buff,"w.multigridLevel[%i]",level));
    PlotIt::contour(ps,w.multigridLevel[level],psp);
  }

  return 0;
}
