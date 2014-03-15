#include "Overture.h"
#include "Square.h"

realMappedGridFunction
f( MappedGrid & mg, realMappedGridFunction & u)
{
  realMappedGridFunction v;
  v=u;
  return v;
}

  

int main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,11);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,11);                  // axis2==1, set no. of grid points
  MappedGrid mg(square);                               // MappedGrid for a square
  mg.update();
  
  Range all;
  realMappedGridFunction u(mg,all,all,all); 

  u=2.;
  
  realMappedGridFunction v;
  
  v=u;
  
  v.display("Here is v");
  cout << " u.getMappedGrid=" << u.getMappedGrid() << endl;
  cout << " v.getMappedGrid=" << v.getMappedGrid() << endl;

  realMappedGridFunction w;
  
  w=f(mg,u);
  
  w.display("Here is w");
  cout << " u.getMappedGrid=" << u.getMappedGrid() << endl;
  cout << " w.getMappedGrid=" << w.getMappedGrid() << endl;

  return 0;  

}
