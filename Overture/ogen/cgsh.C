#include "Cgsh.h"
#include "Square.h"
#include "HDF_DataBase.h"

//
// Generate overlapping grids.
//
int main(int argc, char* argv[]) {
    ios::sync_with_stdio();
    Index::setBoundsCheck(On);
//
//  Create an overlapping grid generator object.
//
    PlotStuff plotStuff; Cgsh cgsh(plotStuff);
//
//  Create some Mappings.
//
    Mapping::staticMapList().add(new SquareMapping);

    SquareMapping backgr(-1., 1., -1., 1.);
    backgr.setName(Mapping::mappingName, "Background");
    backgr.setBc(0,0,1); backgr.setBc(1,0,1);
    backgr.setBc(0,1,1); backgr.setBc(1,1,1);
    backgr.setIsPeriodic(0,Mapping::notPeriodic);
    backgr.setIsPeriodic(1,Mapping::notPeriodic);
    backgr.setGridDimensions(0,25); backgr.setGridDimensions(1,25);

    SquareMapping square(-.5, .5, -.5, .5);
    square.setName(Mapping::mappingName, "Moving");
    square.setBc(0,0,0); square.setBc(1,0,0);
    square.setBc(0,1,0); square.setBc(1,1,0);
    square.setIsPeriodic(0,Mapping::notPeriodic);
    square.setIsPeriodic(1,Mapping::notPeriodic);
    square.setGridDimensions(0,13); square.setGridDimensions(1,13);
//
//  Put Mappings into the list of mappings.
//
    cgsh.mappingInformation.mappingList.addElement(backgr);
    cgsh.mappingInformation.mappingList.addElement(square);
//
//  Interactively generate an overlapping grid.
//
    MultigridCompositeGrid m;
    cgsh.specifyOverlap(m);
//
//  Test get() and put().
//
    HDF_DataBase dataFile;
    PlotStuffParameters plotStuffParameters;
//
//  Test MappedGrid get() and put().
//
    MappedGrid &g1 = m[0][0], g2;
    dataFile.mount("cgsh.hdf", "I");
    g1.put(dataFile, "g");
    dataFile.unmount();
    dataFile.mount("cgsh.hdf", "R");
    g2.get(dataFile, "g");
    dataFile.unmount();
    g2.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,g2, plotStuffParameters);
//
//  Test CompositeGrid get() and put().
//
    CompositeGrid &c1 = m[0], c2;
    dataFile.mount("cgsh.hdf", "I");
    c1.put(dataFile, "g");
    dataFile.unmount();
    dataFile.mount("cgsh.hdf", "R");
    c2.get(dataFile, "g");
    dataFile.unmount();
    c2.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,c2, plotStuffParameters);
//
//  Test MultigridCompositeGrid get() and put().
//
//***************
    m[0].interpoleeGrid[0].display("m[0].interpoleeGrid[0] before destroy()");
    m.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask);
    m[0].interpoleeGrid[0].display("m[0].interpoleeGrid[0] after destroy()");
//***************
    MultigridCompositeGrid &m1 = m, m2;
    dataFile.mount("cgsh.hdf", "I");
    m1.put(dataFile, "g");
    dataFile.unmount();
    dataFile.mount("cgsh.hdf", "R");
    m2.get(dataFile, "g");
    dataFile.unmount();
//***************
    m2[0].interpoleeGrid[0].display("m2[0].interpoleeGrid[0] after get()");
//***************
    m2.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,m2[0], plotStuffParameters);

    return 0;
}
