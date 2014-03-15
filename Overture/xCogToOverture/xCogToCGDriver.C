//
//  Create a CompositeGrid hdf file from xcog hdf file.
//
//  Usage:  xCogToCG xcogFile OvertureFile
//

#include "Square.h"
#include "DataPointMapping.h"
#include "CompositeGrid.h"
#include "HDF_DataBase.h"
#include "hdf_stuff.h"

int 
xCogToOverture(const aString & xCogFileName, CompositeGrid & cg, const bool & checkTheGrid=TRUE);

int 
checkOverlappingGrid( const CompositeGrid & cg, const int & option=0 );

int 
main(int argc, char *argv[]) 
{
  if (argc != 3) {
    cerr << "Usage:  " << argv[0] << " xCogFile OvertureFile" << endl;
    exit(1);
  } // end if

  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  char *fileName=argv[1], *newFileName=argv[2], *gridName="overlapping grid";
  CompositeGrid cg;
  xCogToOverture(fileName,cg);

//  Recompute interpolation coordinates and check interpolation stencils.
//  This should not really be necessary, but it is a good debugging tool.
  if( TRUE )
  {
    cout << "Now recompute the interpolation coordinates for checking...\n";
    if (cg.update(
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpoleeLocation,
      CompositeGrid::COMPUTEgeometry) &
	CompositeGrid::COMPUTEfailed)
      cout << "Warning:  Update interpolation failed!" << endl;
    cout << "done recomputing the interpolation coordinates.\n";
      
  }
  

//  Write the CompositeGrid out to a data file.
  HDF_DataBase dataFile; dataFile.mount(newFileName, "I");
//  Destroy all big geometry arrays except for the mask.  This saves space.
  cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask);
  cg.put(dataFile, gridName);
  dataFile.unmount();

  return 0;
}
