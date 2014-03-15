#include "Overture.h"
#include "CompositeGrid.h"

int main(void)
{
  
  CompositeGrid cg;

  getFromADataBase(cg,"hybrid.hdf");
  cg.update();

  const CompositeGridHybridConnectivity & hybConn = cg.getHybridConnectivity();

  cout<<"unstructured grid is "<<hybConn.getUnstructuredGridIndex()<<endl;
  assert(cg[hybConn.getUnstructuredGridIndex()].mapping().getMapping().getClassName()=="UnstructuredMapping");

  hybConn.getUVertex2GridIndex().display("uVertex2GridIndex");
  hybConn.getBoundaryFaceMapping().display("BoundaryFaceMapping");

  for ( int g=0; g<cg.numberOfGrids()-1; g++ )
    {
      cout<<"number of interface vertices on "<<g<<" : "<<hybConn.getNumberOfInterfaceVertices(g)<<endl;
      hybConn.getGridIndex2UVertex(g).display("GridIndex2UVertex");
      hybConn.getGridVertex2UVertex(g).display("GridVertex2UVertex");
    }

  return 0;
}
