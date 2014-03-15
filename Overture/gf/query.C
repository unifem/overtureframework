//===============================================================================
//
//  Display selected info about the grid 
//
//==============================================================================
#include "Overture.h"  
#include "display.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile;
  cout << "Enter the name of the composite grid file (in the cgsh directory)" << endl;
  cin >> nameOfOGFile;
  if( nameOfOGFile[0]!='.' )
    nameOfOGFile="/users/henshaw/res/cgsh/" + nameOfOGFile;
  aString nameOfDirectory = ".";
  cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  const int inflow=1, outflow=2, wall=3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    cg[grid].boundaryCondition.display("boundaryCondition");
    cg[grid].isPeriodic.display("isPeriodic");
    printf(" cg.numberOfInterpolationPoints(grid)=%i \n",cg.numberOfInterpolationPoints(grid));
    cg.interpolationPoint[grid].display("interpolationPoint[grid]");
    cg.interpoleeLocation[grid].display("cg.interpoleeLocation[grid]");
    cg.interpoleeGrid[grid].display("cg.interpoleeGrid[grid]");
    cg.interpolationCoordinates[grid].display("cg.interpolationCoords");

    if( cg[grid].boundaryCondition(Start,axis1) > 0 )
      cg[grid].boundaryCondition(Start,axis1)=inflow;
    if( cg[grid].boundaryCondition(End  ,axis1) > 0 )
      cg[grid].boundaryCondition(End  ,axis1)=outflow;
    for( int axis=axis2; axis<cg.numberOfDimensions(); axis++ )
    {
      if( cg[grid].boundaryCondition(Start,axis) > 0 )
	cg[grid].boundaryCondition(Start,axis)=wall;
      if( cg[grid].boundaryCondition(End  ,axis) > 0 )
	cg[grid].boundaryCondition(End  ,axis)=wall;
    }

    Index I1,I2,I3;
    getIndex(cg[grid].dimension,I1,I2,I3);
    IntegerArray m(I1,I2,I3);

    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
          m(i1,i2,i3) = cg[grid].mask(i1,i2,i3) & MappedGrid::GRIDnumberBits;
	}
    display(m,"g[grid].mask & GRIDnumberBits");
    

  }    

  return 0;
}
