#include "Cgsh.h"
#include "Square.h"

int main(int argc, char* argv[]) {
    Index::setBoundsCheck(On);
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
//  Put mappings into the list of mappings.
//
    aString answer;
    PlotStuff plotStuff; Cgsh cgsh(plotStuff);
    cgsh.mappingInformation.mappingList.addElement(backgr);
    cgsh.mappingInformation.mappingList.addElement(square);
//
//  Set up the initial overlapping grid.
//
    CompositeGrid g[2];
    cgsh.specifyOverlap(g[0]);
//
//  Plot the grid.
//
    PlotStuffParameters psp; psp.plotObjectAndExit = TRUE;
    PlotIt::plot(plotStuff,g[0], psp); plotStuff.redraw(TRUE);
//
//  Set up a copy of the overlapping grid.
//
    g[1] = g[0];
//
//  Remove all of the big data.  These will be shared if possible.
//
    g[1].destroy(CompositeGrid::EVERYTHING);
//
//  Move the grid a bunch of times.
//
    LogicalArray hasMoved(2);
    hasMoved    = LogicalFalse;
    hasMoved(1) = LogicalTrue;  // Only this grid will move.

    const Integer MOVES = 48;
    const Real PIM = (Real)4. * atan(1.) / MOVES;
    for (Integer i=1; i<=MOVES; i++) {
        CompositeGrid &g0 = g[i%2], &g1 = g[(i+1)%2];
//
//      Use a new (moved) grid.
//
        const Real size = (Real).5 + (Real).2 * sin(PIM * i);
        SquareMapping mapping(-size, size, -size, size);
        g0[1].reference(mapping);
//
//      Force the overlap algorithm to be used occasionally.
//
        if (i % 8 == 0)
          g0.geometryHasChanged(CompositeGrid::THEinterpolationPoint);
//
//      Update the overlapping grid g0, starting from and sharing data with g1.
//
        cgsh.updateOverlap(g0, g1, hasMoved);
//
//      Plot the grid.
//
        plotStuff.erase(); PlotIt::plot(plotStuff,g0, psp); plotStuff.redraw(TRUE);
    } // end for

    sleep(5);
    return 0;
}
