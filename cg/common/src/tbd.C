#include "BoundaryData.h"

BoundaryData::BoundaryDataArray& 
getBoundaryData(std::vector<BoundaryData> & boundaryData, int grid )
// ===================================================================================
// /Description:
// return the boundary data for a grid
// ===================================================================================
{
  if( grid >= boundaryData.size() )
    boundaryData.resize(grid+1,BoundaryData());

  return boundaryData[grid].boundaryData;

}

realArray &
getBoundaryData(std::vector<BoundaryData> & boundaryData, int side, int axis, int grid, MappedGrid & mg )
// ===================================================================================
// /Description:
//   Allocate the boundary data for a given side of a grid.
//   Some boundary data may have so be saved since it is too expensive to recompute. e.g. parabolicInfow
// Return an array to use on a given face (allocate it if necessary)
// /side,axis,grid (input) : face
// /mg (input) : the MappedGrid
//\end{MappedGridSolverInclude.tex}  
// ===================================================================================
{
  assert( grid>=0 );
  if( grid >= boundaryData.size() )
    boundaryData.resize(grid+1,BoundaryData());
  
  if( false )
  {
    for( int grid2=0; grid2<boundaryData.size(); grid2++ )
    {
      BoundaryData::BoundaryDataArray & pBoundaryData = getBoundaryData(boundaryData,grid2);
      if( pBoundaryData!=NULL )
      {
        printf("@@@getBoundaryData:start: grid2=%i pBoundaryData=%i\n",grid2,pBoundaryData);
	for( int side=0; side<=1; side++ )for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  if( pBoundaryData[side][axis]!=NULL )
	  {
	    realArray & bd = *pBoundaryData[side][axis];
	    printF("@@@getBoundaryData:start (grid,side,axis)=(%i,%i,%i) bd : [%i,%i][%i,%i] \n"
                   "    ---> pBoundaryData[side][axis]=%i\n",grid2,side,axis,
		   bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1),pBoundaryData[side][axis]);
	  }
	}
      }
      else
      {
	printF("@@@getBoundaryData: grid=%i pBoundaryData=NULL\n",grid2);
      }
    }
  }

  realArray *&pBoundaryData = boundaryData[grid].boundaryData[side][axis];
  if( pBoundaryData==NULL )
  {
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    int extra=1;
    getBoundaryIndex(mg.extendedIndexRange(),side,axis,I1,I2,I3,extra);
    Range C(1,2);
    realArray *pbd =new realArray(I1,I2,I3,C);
    cout << "\n ==== allocate a new array pdg= "<< pbd << endl;
    pBoundaryData = pbd; // new realArray(I1,I2,I3,C);

    realArray & bd = *pBoundaryData;
    printf(">>getBoundaryData: (side,axis,grid)=(%i,%i,%i) allocate new boundary array [%i,%i][%i,%i][%i,%i][%i,%i]\n"
           "   ---> pBoundaryData=%i <----- \n",
	   side,axis,grid,bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1),bd.getBase(2),bd.getBound(2),
	   bd.getBase(3),bd.getBound(3),pBoundaryData);
    
  }
  if( true )
  {
    for( int grid2=0; grid2<boundaryData.size(); grid2++ )
    {
      BoundaryData::BoundaryDataArray & pBoundaryData = getBoundaryData(boundaryData,grid2);
      if( pBoundaryData!=NULL )
      {
	for( int side=0; side<=1; side++ )for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  if( pBoundaryData[side][axis]!=NULL )
	  {
	    realArray & bd = *pBoundaryData[side][axis];
	    printF("@@@getBoundaryData: (grid,side,axis)=(%i,%i,%i) bd : [%i,%i][%i,%i]\n",grid2,side,axis,
		   bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1));
	  }
	}
      }
      else
      {
	printF("@@@getBoundaryData: grid=%i pBoundaryData=NULL\n",grid2);
      }
    }
  }

  return *pBoundaryData;
}



int main( int argc, char *argv[] )
{
  Overture::start(argc,argv);

  aString nameOfOGFile="backStepSmooth.hdf";
  
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);

  // boundaryData holds the RHS for BC's 
  std::vector<BoundaryData> boundaryData;

  if( false )
  {

    boundaryData.push_back(BoundaryData());
    boundaryData.push_back(BoundaryData());


    for( int grid=0; grid<boundaryData.size(); grid++ )
    {
      printF("grid=%i : boundaryData[grid]=%i \n",grid,&boundaryData[grid]);
    }
  
    for( int grid=0; grid<boundaryData.size(); grid++ )
    {
      printF("grid=%i : boundaryData[grid]=%i \n",grid,&boundaryData[grid]);
    }
  }
  
  int grid,side,axis;
  
  if( true )
  {
    printf("\n **** call getBoundaryData with grid=1\n");
    grid=1, side=0, axis=0;
    realArray & bd = getBoundaryData(boundaryData, side, axis, grid, cg[grid] );
 
    printf("\n **** call getBoundaryData with grid=2\n");
    grid=2, side=0, axis=0;
    realArray & bd2 = getBoundaryData(boundaryData, side, axis, grid, cg[grid] );
  }
  else
  {
    grid=2, side=0, axis=0;
    realArray & bd2 = getBoundaryData(boundaryData, side, axis, grid, cg[grid] );

    grid=1, side=0, axis=0;
    realArray & bd = getBoundaryData(boundaryData, side, axis, grid, cg[grid] );
  }
  
  if( true )
  {
    for( int grid2=0; grid2<boundaryData.size(); grid2++ )
    {
      BoundaryData::BoundaryDataArray & pBoundaryData = getBoundaryData(boundaryData,grid2);
      if( pBoundaryData!=NULL )
      {
	printf("@@@@@ grid2=%i pBoundaryData=%i\n",grid2,pBoundaryData);
	for( int side=0; side<=1; side++ )for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  if( pBoundaryData[side][axis]!=NULL )
	  {
	    realArray & bd = *pBoundaryData[side][axis];
	    printF("@@@@@ (grid,side,axis)=(%i,%i,%i) bd : [%i,%i][%i,%i] \n"
		   "    ---> pBoundaryData[side][axis]=%i\n",grid2,side,axis,
		   bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1),pBoundaryData[side][axis]);
	  }
	}
      }
      else
      {
	printF("@@@@@ grid=%i pBoundaryData=NULL\n",grid2);
      }
    }
  }
  
  return 0;
}
