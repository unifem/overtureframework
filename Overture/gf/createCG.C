// ----------------------------------------------------------------------------------------
//     Create your own Composite Grid
//
//  This routine can be used to create your own CompositeGrid by filling in all
//  the necessary information.
//
//  The grid is output to a data-base file
//
// Usage:
//    createCG [plot] 
// ----------------------------------------------------------------------------------------
#include "Overture.h"
#include "DataPointMapping.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"

void initializeMappingList();   // this allows Mappings to be read from a data base

int
main(int argc, char *argv[])
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  int plotOption=0;   // >0 : plot
  if( argc>1 )
    plotOption=1;
  else
    cout << "To plot the grid type `createCG plot' \n";
  
  int numberOfDimensions=2;
  int numberOfGrids=2;

  CompositeGrid cg(numberOfDimensions,numberOfGrids);

  PlotStuff ps(plotOption>0,"create");

  // First create the MappedGrid's 
  for( int grid=0; grid<numberOfGrids; grid++ )
  {
    // read in vertices here
    int n=11;
    realArray vertices(n,n,1,2);
    for( int i=0; i<n; i++ )
    {
      for( int j=0; j<n; j++ )
      {
        vertices(i,j,0,0)=(grid+1)*(i-n/2.)/(n-1.);
        vertices(i,j,0,1)=(grid+1)*(j-n/2.)/(n-1.);
      }
    }
    // vertices.display("vertices");
    // create a mapping from data points
    DataPointMapping & dataPoint = *( new DataPointMapping() );
    dataPoint.setDataPoints(vertices,3,numberOfDimensions);  // 3=position of coordinates
    //  PlotIt::plot(ps,dataPoint);
    
    if( grid==0 )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
        // dataPoint.setIsPeriodic(axis,Mapping::functionPeriodic);
        dataPoint.setBoundaryCondition(Start,axis,-1);
        dataPoint.setBoundaryCondition(End  ,axis,-1);
      }
    }
    
    MappedGrid mg(dataPoint);
    mg.updateReferences();
    mg.update();
    //  PlotIt::plot(ps,mg);
    
    cg[grid].reference(mg);
    cg[grid].updateReferences();
    //  cg[grid].update();

    // PlotIt::plot(ps,cg[grid]);
    // cg.numberOfInterpolationPoints.display("cg.numberOfInterpolationPoints");
    int numberOfInterpolationPoints=5;
    cg.numberOfInterpolationPoints(grid)=numberOfInterpolationPoints;

  }
  cg.updateReferences();
  cg.update();
  
  // Now fill in various CompositeGrid arrays
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    Index I1,I2,I3;
    getIndex(cg[grid].gridIndexRange,I1,I2,I3);
    cg[grid].mask=0;
    cg[grid].mask(I1,I2,I3)=1;
    // cg[grid].mask.display("mask");
    // cg[grid].vertex.display("vertex");
    
    for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
    {
      cg.interpolationPoint[grid](i,0)=i;
      cg.interpolationPoint[grid](i,1)=i;
      cg.interpoleeGrid[grid](i)=(grid+1) % numberOfGrids;

      cg.interpoleeLocation[grid](i,0)=i+2;
      cg.interpoleeLocation[grid](i,1)=i+3;

      cg.interpolationCoordinates[grid](i,0)=.1;
      cg.interpolationCoordinates[grid](i,1)=.1;
      
    }
    cg.interpolationPoint[grid].display("Here is cg.interpolationPoint ");
  }
  // Tell the CompositeGrid the interpolation info has been defined:
  cg.computedGeometry() |=  CompositeGrid::THEinterpolationCoordinates 
                          | CompositeGrid::THEinterpolationPoint 
                          | CompositeGrid::THEinterpoleeLocation
                          | CompositeGrid::THEinterpoleeGrid;
  if( plotOption & 1 )
    PlotIt::plot(ps,cg);

  aString fileName = "bogus.hdf", gridName="bogus";
  
  HDF_DataBase dataFile;
  dataFile.mount(fileName,"I");

  // first destroy any big geometry arrays: (but not the mask) (This saves space)
  cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
  cg.put(dataFile,gridName);

  dataFile.unmount();

  // ------ this next section is for debugging -----------------------------------

  // Read the CompositeGrid back in so we can check if stuff is there
  initializeMappingList();   // this allows Mappings to be read from a data base
  dataFile.mount(fileName,"R");
  CompositeGrid cg2;
  cout << "------------------- read back the grid ---------------------\n";
  cg2.get(dataFile,gridName);
  cg2.update();
  
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    cg2.interpolationPoint[grid].display("Here is cg2.interpolationPoint ");
  }

  if( plotOption & 1 )
    PlotIt::plot(ps,cg2);
}

