#include "Overture.h"
#include "GeometricADT.h"
#include "GeometricADT2.h"
#include "GeometricADT3.h"
// include "intGeometricADT3D.h"
#include "list.h"
#include "UnstructuredMapping.h"
#include "PlotStuff.h"
#include "display.h"

// test 3d or 2d:
#if 1
#define DIMENSION 3
#define DIMENSION2 6
#else
#define DIMENSION 2
#define DIMENSION2 4
#endif

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  aString fileName = "triShip3.ingrid";
// aString fileName = "nurbs.ingrid";
   UnstructuredMapping map;
   map.get(fileName);

//   PlotStuff ps;
//   PlotIt::plot(ps, map );
  
  int rangeDimension=min(map.getRangeDimension(),DIMENSION);

  realArray bb(2,rangeDimension), bb0;
  realArray adtbb(2,2,rangeDimension);
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    bb(Start,axis)=map.getRangeBound(Start,axis);
    bb(End  ,axis)=map.getRangeBound(End  ,axis);
    adtbb(0,Start,axis)=bb(Start,axis); // min for xMin
    adtbb(1,Start,axis)=bb(End  ,axis); // max for xMin
    adtbb(0,End  ,axis)=bb(Start,axis); // min for xMax
    adtbb(1,End  ,axis)=bb(End  ,axis);
    
  }
  display(bb,"global bounding box");
  bb0=bb;
  
  adtbb.reshape(4*rangeDimension);


  const intArray & elements = map.getElements();
  const realArray & nodes = map.getNodes();
  const int numberOfNodes = map.getNumberOfNodes();
  const int numberOfElements = map.getNumberOfElements();

  real time0,time;
  int e;
  realArray target(2,rangeDimension);
  int count;

  // GeometricADT<int> adt3(*adtbb.getDataPointer());

  // ================================================================================================
  if( true )
  {
    ArraySimple<real> globalbb(4*rangeDimension), bb(2*rangeDimension), target(2*rangeDimension);

    int dir;
    for( int i=0; i<2*rangeDimension; i++ )
    {
      globalbb(i)=bb0(i);
    }
    
    for( int it=0; it<=1; it++ )
    {
      GeometricADT<int> adt(rangeDimension,globalbb);

      time0=getCPU();
      for( e=0; e<numberOfElements; e++ )
      {
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);
	for( dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0+2*dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  bb(1+2*dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}
	adt.addElement(bb,e);
    
      }
  
      time=getCPU()-time0;
      printf("(ADT) Time to insert %i nodes = %8.2e, time per node = %8.2e \n",
              numberOfElements,time,time/numberOfElements);
  
      count=0;
      for( e=0; e<numberOfElements; e+=10 )
      {
	count++;
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);

	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  target(0+2*dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  target(1+2*dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}

    
	GeometricADT<int>::traversor traversor(adt,target);

	bool found=false;
	while( !traversor.isFinished() )
	{
	  GeomADTTuple<int> & leaf = *traversor;
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
      printf("(ADT) Time for %i queries = %8.2e, time/query = %8.2e \n",count,time,time/count);
    }
    
  }
   // ========================================================================================
  if( true )
  {
    for( int it=0; it<=1; it++ )
    {

      GeometricADT2<int,DIMENSION2> adt2(rangeDimension,bb0.getDataPointer());
      ArraySimple<real> bb(2*rangeDimension), target(2*rangeDimension);

      time0=getCPU();
      for( e=0; e<numberOfElements; e++ )
      {
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0+2*dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  bb(1+2*dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}
	// bb.reshape(2*rangeDimension);
    
	adt2.addElement(bb.ptr(),e);
    
	// bb.reshape(2,rangeDimension);
      }
  
      time=getCPU()-time0;
      printf("(ADT2) Time to insert %i nodes = %8.2e, time per node = %8.2e \n",
	     numberOfElements,time,time/numberOfElements);
  
      GeometricADT2<int,DIMENSION2>::traversor traversor(adt2);

      count=0;
      for( e=0; e<numberOfElements; e+=10 )
      {
	count++;
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);

	// target.reshape(2,rangeDimension);
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  target(0+2*dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  target(1+2*dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}
	// target.reshape(2*rangeDimension);
    
	// GeometricADT2<int>::traversor traversor(adt2,target.getDataPointer());
	traversor.setTarget(target.ptr());
	
	bool found=false;
	while( !traversor.isFinished() )
	{
	  GeomADTTuple2<int,DIMENSION2> & leaf = *traversor;
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
      printf("(ADT2) Time for %i queries = %8.2e, time/query = %8.2e \n",count,time,time/count);
    }
    

  }
  // ========================================================================
  if( true )
  {
    for( int it=0; it<=1; it++ )
    {

      GeometricADT3<int> adt3(rangeDimension,bb0.getDataPointer());
      ArraySimple<real> bb(2*rangeDimension), target(2*rangeDimension);

      time0=getCPU();
      for( e=0; e<numberOfElements; e++ )
      {
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0+2*dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  bb(1+2*dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}
	// bb.reshape(2*rangeDimension);
    
	adt3.addElement(bb.ptr(),e);
    
	// bb.reshape(2,rangeDimension);
      }
  
      time=getCPU()-time0;
      printf("(ADT3) Time to insert %i nodes = %8.2e, time per node = %8.2e \n",
	     numberOfElements,time,time/numberOfElements);
  
      GeometricADT3<int>::traversor traversor(rangeDimension*2,adt3);

      count=0;
      for( e=0; e<numberOfElements; e+=10 )
      {
	count++;
	int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);

	// target.reshape(2,rangeDimension);
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  target(0+2*dir)=min(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	  target(1+2*dir)=max(nodes(n0,dir),nodes(n1,dir),nodes(n2,dir));
	}
	// target.reshape(2*rangeDimension);
    
	// GeometricADT2<int>::traversor traversor(adt2,target.getDataPointer());
	traversor.setTarget(target.ptr());
	
	bool found=false;
	while( !traversor.isFinished() )
	{
	  GeomADTTuple3<int> & leaf = *traversor;
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
      printf("(ADT3) Time for %i queries = %8.2e, time/query = %8.2e \n",count,time,time/count);
    }
    

  }



/* ----
  if( true )
  {
    for( int it=0; it<=1; it++ )
    {

      intGeometricADT3D adt2(rangeDimension,adtbb.getDataPointer());


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
  
      intGeometricADTtraversor3D traversor(adt2);

      count=0;
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
	  intGeomADTTuple3D & leaf = *traversor;
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
      printf("(2) Time for %i queries = %8.2e, time/query = %8.2e \n",count,time,time/count);
    }
    

  }
 ---- */ 
//   for( ;; )
//   {
//     real x=0., y=0., z=0.;
//     cout << "Enter a point x,y,z \n";
//     cin >> x >> y >> z;
  
//     target.reshape(2,rangeDimension);
//     target(0,0)=x; target(1,0)=x;
//     target(0,1)=y; target(1,1)=y;
//     if( rangeDimension>2 )
//     {
//       target(0,2)=z; target(1,2)=z;
//     }
    
//     target.reshape(2*rangeDimension);
    
//     GeometricADT<int>::traversor traversor(adt,target);
  
//     while( !traversor.isFinished() )
//     {
//       GeomADTTuple<int> & leaf = *traversor;
//       printf(" (%8.2e,%8.2e,%8.2e) is inside box %i \n",x,y,z,leaf.data);
//       traversor++;
//     }
//   }
  

  return 0;
}
