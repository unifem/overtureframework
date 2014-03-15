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
//  Create a SquareMapping.
//
  PlotStuff plotStuff;

    SquareMapping square(-1., 1., -1., 1.);
    square.setGridDimensions(0,26); square.setGridDimensions(1,26);
    Mapping::staticMapList().add(&square);

    MappedGrid mg(square);
    mg.update();
    cout << "Plot MappedGrid(square) \n";
    PlotIt::plot(plotStuff,mg);

    
    ReparameterizationTransform mapping(square,ReparameterizationTransform::restriction);
    mg.reference(mapping);
    cout << "main():  mapping.setBounds(0.,1.,0.,1.,0.,0.);" << endl;
    mapping.setBounds(0.,1.,0.,1.,0.,0.); 

    cout << "Plot ReparameterizationTransform \n";
    PlotIt::plot(plotStuff,mapping);
    mg.destroy(MappedGrid::EVERYTHING);
    mg.update(MappedGrid::EVERYTHING);
    cout << "Plot MappedGrid(ReparameterizationTransform) \n";
    PlotIt::plot(plotStuff,mg);

    cout << "main():  mapping.setBounds(.25,.75,.25,.75,0.,0.);" << endl;
    mapping.setBounds(.25,.75,.25,.75,0.,0.); 
    cout << "Plot ReparameterizationTransform \n";
    PlotIt::plot(plotStuff,mapping);
    mg.destroy(MappedGrid::EVERYTHING);
    mg.update(MappedGrid::EVERYTHING);
    cout << "Plot MappedGrid(ReparameterizationTransform) \n";
    PlotIt::plot(plotStuff,mg);


    return 0;
}
