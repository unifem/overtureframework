#include "Cgsh.h"
#include "Square.h"
#include "HDF_DataBase.h"

#define GRID_COLLECTION CompositeGrid

//
// Generate overlapping grids.
//
int main(int argc, char* argv[]) {
    ios::sync_with_stdio();
    Index::setBoundsCheck(On);
//
//  Create a SquareMapping.
//
    SquareMapping square(-1., 1., -1., 1.);
    square.setGridDimensions(0,26); square.setGridDimensions(1,26);
    Mapping::staticMapList().add(&square);
//
//  Create a two-dimensional GRID_COLLECTION with one square grid.
//
    PlotStuff plotStuff; PlotStuffParameters plotStuffParameters;
    plotStuff.outputString(
      "Operation                                        time (step)  time (cumulative)");
    Real time0, time1, time; char _[1000]; second_(time0); time1 = time0;

    GRID_COLLECTION c1(2,1);
    c1[0].reference(MappedGrid(square));

    second_(time); plotStuff.outputString((sprintf(_,
      "Construct c1 (initial)                    %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    c1.update(GRID_COLLECTION::THErefinementLevel);

    second_(time); plotStuff.outputString((sprintf(_,
      "Update the refinementLevel of c1          %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    cout << "amr:  c1.refinementLevel.getLength() = "
         <<        c1.refinementLevel.getLength() << ", "
         <<       "c1.refinementLevel[0].grid.getLength() = "
         <<        c1.refinementLevel[0].grid.getLength()
         << endl;

    GRID_COLLECTION c2 = c1;

    second_(time); plotStuff.outputString((sprintf(_,
      "Construct c2 (deep copy)                  %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    c2.update(GRID_COLLECTION::THErefinementLevel);

    second_(time); plotStuff.outputString((sprintf(_,
      "Update the refinementLevel of c2          %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    cout << "amr:  c2.refinementLevel.getLength() = "
         <<        c2.refinementLevel.getLength() << ", "
         <<       "c2.refinementLevel[0].grid.getLength() = "
         <<        c2.refinementLevel[0].grid.getLength()
         << endl;

    IntegerArray range(2,3), factor(3);
    range(0,0) = 26; range(1,0) = 76;
    range(0,1) = 26; range(1,1) = 76;
    range(0,2) = 0; range(1,2) = 0;
    factor = 4;
    Integer level = 1, grid = 0;
    c1.addRefinement(range, factor, level, grid);

    second_(time); plotStuff.outputString((sprintf(_,
      "Add refinement to c1                      %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    c1.update(GRID_COLLECTION::THErefinementLevel);

    cout << "amr:  c1.refinementLevel.getLength() = "
         <<        c1.refinementLevel.getLength() << ", "
         <<       "c1.refinementLevel[0].grid.getLength() = "
         <<        c1.refinementLevel[0].grid.getLength() << ", "
         <<       "c1.refinementLevel[1].grid.getLength() = "
         <<        c1.refinementLevel[1].grid.getLength()
         << endl;

    MappedGrid &g_b = c1.refinementLevel[0][0],
               &g_r = c1.refinementLevel[1][0];
    cout <<  g_b.indexRange()(0,1) << ":"
         <<  g_b.indexRange()(1,1) << ","
         <<  g_b.indexRange()(0,2) << ":"
         <<  g_b.indexRange()(1,2) << "), "
         << "g_b.gridIndexRange() = ("
         <<  g_b.gridIndexRange()(0,0) << ":"
         <<  g_b.gridIndexRange()(1,0) << ","
         <<  g_b.gridIndexRange()(0,1) << ":"
         <<  g_b.gridIndexRange()(1,1) << ","
         <<  g_b.gridIndexRange()(0,2) << ":"
         <<  g_b.gridIndexRange()(1,2) << "), "
         << endl
         << "g_r.indexRange() = ("
         <<  g_r.indexRange()(0,0) << ":"
         <<  g_r.indexRange()(1,0) << ","
         <<  g_r.indexRange()(0,1) << ":"
         <<  g_r.indexRange()(1,1) << ","
         <<  g_b.indexRange()(0,2) << ":"
         <<  g_r.indexRange()(1,2) << "), "
         << "g_r.gridIndexRange() = ("
         <<  g_r.gridIndexRange()(0,0) << ":"
         <<  g_r.gridIndexRange()(1,0) << ","
         <<  g_r.gridIndexRange()(0,1) << ":"
         <<  g_r.gridIndexRange()(1,1) << ","
         <<  g_r.gridIndexRange()(0,2) << ":"
         <<  g_r.gridIndexRange()(1,2) << "), "
         << endl;

    cout << "main():  plot(c1)" << endl;
    c1.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,c1, plotStuffParameters);

    cout << "main():  plot(c1[0])" << endl;
    c1[0].update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,c1[0], plotStuffParameters);

    cout << "main():  plot(c1[1])" << endl;
    c1[1].update(); // BOGUS:  plot() should call update() for the data it needs.

    c1[1].vertex().display("Here is the vertex array");
    c1[1].gridSpacing().display("\n ****Here is grid spacing****");
    c1[1].dimension().display(" *** dimension *** ");

    PlotIt::plot(plotStuff,c1[1], plotStuffParameters);

    cout << "main():  plot(g_r)" << endl;
    g_r.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,g_r, plotStuffParameters);

    MappedGrid g1(g_r, SHALLOW);
    cout << "main():  plot(MappedGrid(g_r, SHALLOW))" << endl;
    g1.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,g1, plotStuffParameters);

    MappedGrid g2(g_r.mapping());
    cout << "main():  plot(MappedGrid(g_r.mapping()))" << endl;
    g2.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,g2, plotStuffParameters);

    MappedGrid g3(*g_r.mapping().mapPointer);
    cout << "main():  plot(MappedGrid(*g_r.mapping().mapPointer))" << endl;
    g3.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,g3, plotStuffParameters);

    ReparameterizationTransform &mapping =
      *(ReparameterizationTransform*)g_r.mapping().mapPointer;

    MappedGrid g4(mapping);
    cout << "main():  plot(MappedGrid(mapping))" << endl;
    g4.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,g4, plotStuffParameters);

    cout << "main():  plot(c1)" << endl;
    c1.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,c1, plotStuffParameters);

    cout << "main():  mapping.setBounds(0.,1.,0.,1.,0.,1.); plot(c1)" << endl;
    mapping.setBounds(0.,1.,0.,1.,0.,1.); c1.destroy(MappedGrid::EVERYTHING);
    c1.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,c1, plotStuffParameters);

    cout << "main():  mapping.setBounds(.25,.75,.25,.75,0.,1.); plot(c1)" << endl;
    mapping.setBounds(.25,.75,.25,.75,0.,1.); c1.destroy(MappedGrid::EVERYTHING);
    c1.update(); // BOGUS:  plot() should call update() for the data it needs.
    PlotIt::plot(plotStuff,c1, plotStuffParameters);

    second_(time); plotStuff.outputString((sprintf(_,
      "Update the refinementLevel of c1          %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    c2.referenceRefinementLevels(c1, level);

    second_(time); plotStuff.outputString((sprintf(_,
      "Reference refinement levels c2 <= c1      %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    c2.update(GRID_COLLECTION::THErefinementLevel);

    second_(time); plotStuff.outputString((sprintf(_,
      "Update the refinementLevel of c2          %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    cout << "amr:  c2.refinementLevel.getLength() = "
         <<        c2.refinementLevel.getLength() << ", "
         <<       "c2.refinementLevel[0].grid.getLength() = "
         <<        c2.refinementLevel[0].grid.getLength() << ", "
         <<       "c2.refinementLevel[1].grid.getLength() = "
         <<        c2.refinementLevel[1].grid.getLength()
         << endl;

    c2.deleteRefinementLevels(level);

    second_(time); plotStuff.outputString((sprintf(_,
      "Delete refinement levels of c2            %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    c2.update(GRID_COLLECTION::THErefinementLevel);

    second_(time); plotStuff.outputString((sprintf(_,
      "Update the refinementLevel of c2          %15.3f%15.3f",
      time - time1, time - time0), _)); time1 = time;

    cout << "amr:  c2.refinementLevel.getLength() = "
         <<        c2.refinementLevel.getLength() << ", "
         <<       "c2.refinementLevel[0].grid.getLength() = "
         <<        c2.refinementLevel[0].grid.getLength()
         << endl;

    return 0;
}
