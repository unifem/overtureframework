//===============================================================================
//  Test the Overlapping Grid Show file class Ogshow
//
//   -----Test the moving grids version-------
//
//==============================================================================
#include "Overture.h"

main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile(80), nameOfShowFile(80), nameOfDirectory(80);
  
  cout << "togshow>> Enter the name of the (old) overlapping grid file (in cguser):" << endl;
  cin >> nameOfOGFile;  
  nameOfOGFile="/n/c3servet/henshaw/cgap/cguser/"+nameOfOGFile;
  cout << "togshow: Enter the directory to use:" << endl;
  cin >> nameOfDirectory;

//  Dir OGFile(3000000);
  Dir OGFile;
  cout << "Ogshow: ---Mount an Overlapping Grid file: " << nameOfOGFile << endl;
  OGFile.mount( nameOfOGFile, " R L4096");

  nameOfShowFile="s.show";
  cout << "Ogshow: ---Mount a showFile:" << nameOfShowFile << endl;
//  Dir showFile(3000000);
  Dir showFile;
  showFile.mount(nameOfShowFile," I L4096 N16");  // Initialize a database file
  showFile.copy( nameOfDirectory, OGFile,nameOfDirectory," R");

  cout << "Ogshow: ---create a multigridCompositeGrid for the show file...\n";
  MultigridCompositeGrid mgcg( showFile,nameOfDirectory );

//  MultigridCompositeGrid multigridCompositeGrid;
//  multigridCompositeGrid.reference(mgcg);
//  CompositeGrid compositeGrid;
//  compositeGrid.reference(multigridCompositeGrid[0]);

  int ct;
  cout << "Enter copy type: 1=R, 0=R L \n";
  cin >> ct;
  if( ct==0 )    
    showFile.copy( "new", *(mgcg[0].rcData),".", " R L" ); 
  else
    showFile.copy( "new", *(mgcg[0].rcData),".", " R" ); 

  cout << "Call check at end...\n";
  showFile.check(" Ofgmst");

}

