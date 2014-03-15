#include "Overture.h"
#include "PlotStuff.h"

int
main(int argc, char *argv[])
{
  
  aString nameOfOGFile;
  cout << ">> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  PlotStuff ps;
  PlotStuffParameters psp;
  

  psp.set(GI_TOP_LABEL,"Initial grid");
  PlotIt::plot(ps,cg,psp);
  

  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    IntegerArray & mask = c.mask();

    where( mask<0 )
    {
      mask=0;
    }
  }
  psp.set(GI_TOP_LABEL,"After removing overlap");
  PlotIt::plot(ps,cg,psp);

  // make a list of vertices on the boundaries
  IntegerArray *vertexIndex = new IntegerArray[ cg.numberOfComponentGrids() ];
  IntegerArray numberOfVertices(cg.numberOfComponentGrids());
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    IntegerArray & mask = c.mask();
    getIndex(c.gridIndexRange(),I1,I2,I3);

    IntegerArray & ia = vertexIndex[grid];
    ia.redim(I1.getLength()*I2.getLength()*I2.getLength(),3);

    int i=0;
    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  if( mask(i1,i2,i3)!=0 && 
	      ( mask(i1-1,i2-1,i3)==0 || mask(i1  ,i2-1,i3)==0 || mask(i1+1,i2-1,i3)==0 ||
	        mask(i1-1,i2  ,i3)==0 ||                          mask(i1+1,i2  ,i3)==0 ||
	        mask(i1-1,i2+1,i3)==0 || mask(i1  ,i2+1,i3)==0 || mask(i1+1,i2+1,i3)==0 ) )
	  {
	    ia(i,0)=i1;
	    ia(i,1)=i2;
	    ia(i,2)=i3;
	    i++;
	  }
	}
      }
    }
    numberOfVertices(grid)=i;
  }
  
  // Now plot the grid and the vertices
  psp.set(GI_TOP_LABEL,"Vertices on grid boundaries");
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  PlotIt::plot(ps,cg,psp);

  psp.set(GI_USE_PLOT_BOUNDS,TRUE); 
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    RealArray & vertex = c.vertex();
    IntegerArray & ia = vertexIndex[grid];
    if( numberOfVertices(grid)>0 )
    {
      Range R(0,numberOfVertices(grid)-1);
      RealArray x(R,cg.numberOfDimensions());
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
        x(R,axis) = vertex(ia(R,0),ia(R,1),ia(R,2),axis);
    
      ps.plotPoints(x,psp);
    }
  }
  psp.set(GI_USE_PLOT_BOUNDS,FALSE); 
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

  aString menu[] =
  {
    "exit",
    ""
  };
  aString answer;
  ps.getMenuItem(menu,answer,"Choose an option");
  
  delete [] vertexIndex;
  return 0;
}
