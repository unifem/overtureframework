#include "Overture.h"
#include "GeometricADT3dInt.h"
#include "list.h"
#include "UnstructuredMapping.h"
#include "PlotStuff.h"
#include "display.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);

   aString fileName = "triShip3.ingrid";
//   aString fileName = "nurbs.ingrid";
   UnstructuredMapping map;
   map.get(fileName);

//   PlotStuff ps;
//   PlotIt::plot(ps, map );
  
  int rangeDimension=map.getRangeDimension();

  realArray bb(2,rangeDimension),bb0;
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    bb(Start,axis)=map.getRangeBound(Start,axis);
    bb(End  ,axis)=map.getRangeBound(End  ,axis);
  }
  display(bb,"global bounding box");
  bb0=bb;
  
  const intArray & elements = map.getElements();
  const realArray & nodes = map.getNodes();
  const int numberOfNodes = map.getNumberOfNodes();
  const int numberOfElements = map.getNumberOfElements();

  real time0,time;
  int e;
  realArray target(2,rangeDimension);
  int count;

  if( true )
  {
    for( int it=0; it<=1; it++ )
    {

      GeometricADT3dInt adt2(rangeDimension,bb0.getDataPointer());


      time0=getCPU();
      for( e=0; e<numberOfElements; e++ )
      {
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0,dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  bb(1,dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}
	// bb.reshape(2*rangeDimension);
    
	adt2.addElement(bb.getDataPointer(),e);
    
	// bb.reshape(2,rangeDimension);
      }
  
      time=getCPU()-time0;
      printf("(2) Time to insert %i nodes = %8.2e, time per node = %8.2e \n",
	     numberOfElements,time,time/numberOfElements);
  
      GeometricADTTraversor3dInt traversor(adt2);

      count=0;
      int hits=0;
      for( e=0; e<numberOfElements; e+=10 )
      {
	count++;
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);

	// target.reshape(2,rangeDimension);
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  target(0,dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  target(1,dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}
	// target.reshape(2*rangeDimension);
    
	// GeometricADT2<int>::traversor traversor(adt2,target.getDataPointer());
	traversor.setTarget(target.getDataPointer());
	
	bool found=false;
	while( !traversor.isFinished() )
	{
          hits++;
	  GeomADTTuple3dInt & leaf = *traversor;
	  // printf(" (%8.2e,%8.2e,%8.2e) is inside box %i \n",x,y,z,leaf.data);
	  if( leaf.data==e )
	    found=true;
	  traversor++;
	}
	if( !found )
	{
	  printf("Error finding element %i\n",e);
	}
    
      }
  
      time=getCPU()-time0;
      printf("(2) Time for %i queries = %8.2e, time/query = %8.2e, hits/query=%3.0f \n",count,time,time/count,
         hits/real(count));
    }
    

  }

  return 0;
}
