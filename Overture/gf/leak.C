//===============================================================================
//  Test for leaks
//
// mpirun -np 1 leak
//==============================================================================
#include "A++.h"
/* -- this didn't work
static MemoryManagerType *memoryManager;

void 
startUp()
{
  cout << "In startUp\n";
  memoryManager = new MemoryManagerType;  // This will delete A++ allocated memory at the end
}
void 
endDown()
{
  cout << "In endDown\n";
  delete memoryManager;
}
#pragma init (startUp)
#pragma fini (endDown)

----- */

#include "Overture.h"  
#include "SquareMapping.h"
#include "Oges.h"
#include "HDF_DataBase.h"
#include "Ogshow.h"

#include "App.h"
#include "ParallelUtility.h"

realArray
leaky()
{
  realArray u(10); 
  u=5.;
  return u;
}

realMappedGridFunction
leaky(MappedGrid & mg)
{
  realMappedGridFunction u(mg);
  return u;
}

realCompositeGridFunction
leaky(CompositeGrid & mg)
{
  realCompositeGridFunction u(mg);
  return u;
}


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture


  int j;
  char buff[180];

  bool checkArrays=false;
  
  bool checkRedim=false;
  bool checkResize=true;
  bool checkMapping=false;
  
  bool checkMappedGrid = false;
  bool checkComposteGrids=false;
  bool checkCompositeGridEqual = false;

  if( false )
  {
    aString nameOfOGFile="quarterSphere2e.hdf";
  
    // create and read in a CompositeGrid
    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update(MappedGrid::THEmask);


    Range all;
    realCompositeGridFunction u(cg,all,all,all,5);
    u=1.;



    Interpolant interpolant(cg);
    for( j=0; j<100; j++ )
    {
      real mem=Overture::getCurrentMemoryUsage();
      printF("interp: step j=%i, mem=%g (Mb)\n",j,mem);
      interpolant.updateToMatchGrid(cg);
      u.interpolate();
      checkArrayIDs(sPrintF("*** interpolate.updateToMatchGrid(cg) step=%i ***",j));
    }

    for( j=0; j<1000; j++ )
    {

      // cg.breakReference();  // test for Philip Blakely
      real *a = new real [10];

      cg.interpolationOverlap()=3.;   // this leaks ************************* 080410 ****

      if( j % 10 == 0 )
      {
	real mem=Overture::getCurrentMemoryUsage();
	printF("break: step j=%i, mem=%16.12e (Mb)\n",j,mem);
        checkArrayIDs(sPrintF("*** cg.breakReference step=%i ***",j));
      }
      
    }
    return 0;
    
  }
  

  if( checkArrays )
  {
  
    Partitioning_Type partition;
    doubleArray a; 
    a.partition(partition);



//    a.redim(10); 
//    a=1.;
    checkArrayIDs("*** array at start ***",true);
    for( j=0; j<100; j++ )
    {
      doubleArray b,c;
      b.partition(partition);
      c.partition(partition);
      

      a.redim(10);
      a.resize(20);
      
      b=a;
      c=b+a;
      
      b.reference(a);
      
      checkArrayIDs(sPrintF(buff,"*** array j=%i ",j));

    }
    checkArrayIDs("*** array at finish ***",true);

  }

  if( checkRedim )
  {
  
    Partitioning_Type partition;
    doubleArray a;
    a.partition(partition);
//    a.redim(10); 
//    a=1.;
    checkArrayIDs("*** redim at start ***",true);
    for( j=0; j<100; j++ )
    {
      // a.redim(0);
      a.redim(10);
      
      checkArrayIDs(sPrintF(buff,"*** redim j=%i ",j));

    }
    checkArrayIDs("*** redim at finish ***",true);

  }

  if( checkResize )
  {
  
    Partitioning_Type partition;
    doubleArray a;
    a.partition(partition);
    a.redim(5,4);
    
    checkArrayIDs("*** resize at start ***",true);
    for( j=0; j<100; j++ )
    {
      // a.resize(0);
      // a.resize(10);
      a.resize(20);  // *wdh* 060504 fixed resize leak
      
      a.resize(2,10);

      doubleArray b;
      b.partition(partition);
      b.redim(5,4);
      b.resize(20);

      // leak here: 
      // doubleSerialArray bLocal; getLocalArrayWithGhostBoundaries(b,bLocal);
      // bLocal.resize(5,4);   // leak generated here in parallel
      // // bLocal.reshape(5,4);   // leak generated here too
      // bLocal.resize(20); // leak

      // no leak doing this: 
      doubleSerialArray bLocal; getLocalArrayWithGhostBoundaries(b,bLocal);
      doubleSerialArray b2Local;
      b2Local.reference(bLocal);
      b2Local.resize(5,4); 
      b2Local.reshape(20);

      // no leak here: 
      // doubleSerialArray c(20), d;
      // d.reference(c);
      // d.resize(4,5);

      checkArrayIDs(sPrintF(buff,"*** resize j=%i ",j));

    }
    checkArrayIDs("*** resize at finish ***",true);

  }

  if( FALSE )
  {
  
    floatArray a(1,1),b;
    intArray c(2);
    c=5;
    c.display("");
    c.redim(0);
  
    a=1.;
    for( j=0; j<3000; j++ )
    {
      b.redim(0);
      b.adopt(&a(0,0),1);


      if( j % 100 == 0 )
	printf("**** adopt: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }
  
  if( FALSE )
  {
  
    floatArray a(5),b,c(5),d,e(8),f;
    c=1.;
    for( j=0; j<3000; j++ )
    {
/* ---
      b.reference(a);
      b.reference(c);
      b=c;
      floatArray *p;
      p=new floatArray(5);
      (*p)=3.;
      b.reference(*p);
      a.reference(*p);
      d=b;
      
      a.redim(0);
      delete p;
--- */
//      e.reshape(2,4);
      f.resize(15);
      
      if( j % 100 == 0 )
	printf("**** ref:   number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }
  
  if( FALSE )
  {
    for( j=0; j<3000; j++ )
    {
      leaky();
      if( j % 100 == 0 )
	printf("**** leaky(): number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }
  

  if( checkMapping )
  {
    checkArrayIDs(">>>>>>>>Mapping START",true);
    for( j=0; j<100; j++ )
    {
      SquareMapping square;

      checkArrayIDs(sPrintF(buff,"*** mapping j=%i ",j));
    }
    checkArrayIDs("<<<<<<<<<Mapping END",true);
  }

  if( checkMappedGrid )
  {
    // int na = GET_NUMBER_OF_ARRAYS; // this is wrong for P++
    checkArrayIDs(">>>>>>>>MappedGrid START",true);
    SquareMapping square;
    for( j=0; j<100; j++ )
    {
      MappedGrid g(square);
      g.update(MappedGrid::THEvertex);

      checkArrayIDs(sPrintF(buff,"*** mappedGrid j=%i ",j));
    }

    checkArrayIDs("<<<<<<<<<MappedGrid END",true);
  }


  if( !checkComposteGrids ) return 0;
  

  aString nameOfOGFile="sis.hdf";
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cout << "cg.computedGeometry() & THErefinementLevel = " 
       << (cg.computedGeometry() & GridCollection::THErefinementLevel) << endl;

  cg.update(MappedGrid::THEmask);

  cout << "cg.computedGeometry() & THErefinementLevel = " 
       << (cg.computedGeometry() & GridCollection::THErefinementLevel) << endl;
  

  if( false )
  {
    for( j=0; j<100; j++ )
    {
      cg.update(
	CompositeGrid::THEinterpolationPoint       |
	CompositeGrid::THEinterpoleeGrid           |
	CompositeGrid::THEinterpoleeLocation       |
	CompositeGrid::THEinterpolationCoordinates, 
	CompositeGrid::COMPUTEnothing);

      if( j % 5 == 0 )
	printf("**** cg.update: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }

  if( false )
  {
    printf("**** Checking g=cg[0] ***********\n");
    for( j=0; j<100; j++ )
    {
      MappedGrid g;
      g = cg[0]; 
      if( j % 5 == 0 )
	checkArrayIDs(sPrintF(buff,"*** g=cg[0]: j=%i ",j));

    }
  }

  if( checkCompositeGridEqual )
  {
    printf("**** Checking cg2=cg ***********\n");
    
    cg.update();
    for( j=0; j<100; j++ )
    {
      CompositeGrid cg2; 
      cg2 = cg; 
      if( j % 5 == 0 )
        checkArrayIDs(sPrintF(buff,"*** cg2=cg: j=%i ",j));
    }

    //  return 0;  // ****

  }


  if( false )
  {
    // this is from Ogshow:
    realCompositeGridFunction u(cg);
    u=1.;
    int totalNumberOfArrays = GET_NUMBER_OF_ARRAYS;
    for( int i=0; i<100; i++ )
    {
      GridCollection & gc0 = *u.getGridCollection();
      if( gc0.getClassName()=="CompositeGrid" )
      {  // the GridCollection is really a CompositeGrid
        
//	CompositeGrid cg = (CompositeGrid&) gc0;  // make a copy
//	CompositeGrid cg = *u.getCompositeGrid();  // make a copy
	CompositeGrid cg; cg = *u.getCompositeGrid();  // make a copy
	// first destroy any big geometry arrays: (but not the mask)
	// cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
	// cg.put(*frame,"CompositeGrid");
      }
      else
      {
	throw "error";
        // GridCollection cg = (CompositeGrid&) gc0;
	// first destroy any big geometry arrays: (but not the mask)
	// cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
	// cg.put(*frame,"CompositeGrid");
      }
      if( GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
      {
	totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
	printf("**** Ogshow example: Warning the number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
      }

    }
  }
  



  if( FALSE )
  {
    MappedGrid & mg = cg[0];
    Range C(0,1);
    realMappedGridFunction u(mg);
    for( j=0; j<900; j++ )
    {
//      u.updateToMatchGrid(GridFunctionParameters::cellCentered, C);
      u.updateToMatchGrid(mg,GridFunctionParameters::cellCentered, C);
      if( j % 100 == 0 )
	printf("**** updateToMatchGrid: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }
  
  if( FALSE )
  {
    MappedGrid & mg = cg[0];
    realMappedGridFunction u1(mg),u2(mg);
    u1=1.;
    for( j=0; j<900; j++ )
    {
      u2=u1+u1*u1;
      if( j % 100 == 0 )
	printf("**** MGF u2=u1+...    : number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }
  
  if( false )
  {
    RealArray a(2),b(2);
    ListOfRealArray list, list2,list3;
    list.addElement(a);
    list.addElement(b);
    
    for( j=0; j<100; j++ )
    {
      // CompositeGrid cg2;    // this is ok, no leak
      list2=list;
      list3.reference(list);
      if( j % 5 == 0 )
	printf("**** list2=list: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }
  

  if( false )
  {
    realCompositeGridFunction u1(cg),u2(cg);
    u1=1.;
    for( j=0; j<900; j++ )
    {
      u2=u1+u1;
      if( j % 100 == 0 )
	printf("**** CGF u2=u1+...    : number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
    }
  }
  
  if( TRUE )
    return 0;

  Range all;
  realCompositeGridFunction u(cg,all,all,all,2),v;
  u=1.;
  
  Interpolant interpolant(cg);
  for( j=0; j<500; j++ )
  {
//    interpolant.updateToMatchGrid(cg);
    u.interpolate();
    if( j % 50 == 0 )
      printf("**** interpolant: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

  for( j=0; j<1000; j++ )
  {
    v.link(u,Range(0,0));
    if( j % 100 == 0 )
      printf("**** link: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

  for( j=0; j<1000; j++ )
  {
    u[0].updateToMatchGrid(cg[0],all,all,all,2);
    if( j % 100 == 0 )
      printf("**** updateToMatchGrid(mg): number of A++ arrays = %i, \n",GET_NUMBER_OF_ARRAYS);
  }

  for( j=0; j<1000; j++ )
  {
    u.updateToMatchGrid(cg,all,all,all,2);
    if( j % 100 == 0 )
      printf("**** updateToMatchGrid(cg): number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }



/* ----

  aString nameOfOGFile="/users/henshaw/res/cgsh/sis.hdf";
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  int j;
  realCompositeGridFunction u(cg);
  u=1.;
  
  Interpolant interpolant(cg);
  for( j=0; j<100; j++ )
  {
    interpolant.updateToMatchGrid(cg);
    u.interpolate();
    if( j % 5 == 0 )
      printf("**** interpolant: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }
  for( j=0; j<100; j++ )
  {
    floatCompositeGridFunction u;
    u=leaky(cg);
    realArray a;
    a=leaky();
    if( j % 5 == 0 )
      printf("**** return by value CGF number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }



  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   normalComponent       = BCTypes::normalComponent,
                   aDotU                 = BCTypes::aDotU,
                   generalizedDivergence = BCTypes::generalizedDivergence,
                   tangentialComponent   = BCTypes::tangentialComponent,
                   allBoundaries         = BCTypes::allBoundaries; 

  CompositeGridOperators op(cg);
  Range all;
  
  floatCompositeGridFunction u(cg,all,all,all,2), gridVelocity(cg,all,all,all,2);
  u.setOperators(op);
  
  u=0.; gridVelocity=1.;
  realArray inflowWithVelocityGivenData(10);
  inflowWithVelocityGivenData=5.;
  
  real t0=0.;
  Range V(0,1);
  for( j=0; j<1000; j++ )
  {
    BoundaryConditionParameters extrapParams;

    u.applyBoundaryCondition(V,extrapolate,    allBoundaries,0.,t0);
    u.applyBoundaryCondition(V,normalComponent,allBoundaries,gridVelocity,t0);
    u.applyBoundaryCondition(V,extrapolate,    allBoundaries,0.,t0);
    u.applyBoundaryCondition(V,extrapolate,allBoundaries,0.,t0);
    u.applyBoundaryCondition(V,dirichlet,  allBoundaries,inflowWithVelocityGivenData,t0);
    u.applyBoundaryCondition(V,extrapolate,allBoundaries,0.,t0);
    if( j % 10 == 0 )
      printf("**** aBC: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

  char buffer[80];
  
  Ogshow show("leak.show");
  show.setMovingGridProblem(TRUE);
  show.setFlushFrequency(2);
  
  for( j=0; j<100; j++ )
  {
    floatCompositeGridFunction u(cg);
    show.startFrame();
    show.saveComment(0,sPrintF(buffer,"  t=%i ",j));               // comment 1 (shown on plot)
    show.saveSolution(u);
    if( j % 5 == 0 )
      printf("**** cg.3: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }


  HDF_DataBase db;
  db.mount("leak.hdf","I");

  for( j=0; j<100; j++ )
  {
    CompositeGrid cg2 = cg;  
    cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
    cg.put(db,"CompositeGrid");
    if( j % 10 == 0 )
      printf("**** cg.2: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }
  db.unmount();

  


  MappingRC g1;
  for (j=0; j<100; j++) {
    MappingRC g2 = g1;
    if (j % 10 == 0 ) cout << "MappingRC: number of A++ arrays = "
      << GET_NUMBER_OF_ARRAYS << endl;
  }

  for(j=0; j<300; j++ )
  {
    // GenericMappedGridOperators op(cg[0]); // ok
    MappedGridOperators op(cg[0]);  // ok
    // CompositeGridOperators op(cg);
    if( j % 30 == 0 )
      printf("**** Operators: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }


  for(j=0; j<2000; j++ )
  {
    RealArray x(10),y;
    x=1.;
    x.resize(10,10);  // ok
//    x.redim(100);
//    y=1./(12.*x);
//   d24=1./(12.*SQR(mappedGrid.gridSpacing));

    if( j % 200 == 0 )
      printf("**** resize: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }


  for(j=0; j<300; j++ )
  {
    MappedGrid mg2;         // this is ok
    mg2.reference(cg[0]);   // this is ok
    mg2.update();         // ok
    
    mg2.update(MappedGrid::THEvertexBoundaryNormal | MappedGrid::THEinverseVertexDerivative); // ok
    // CompositeGridOperators op(cg);
    if( j % 30 == 0 )
      printf("**** MappedGrid: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }


  for(j=0; j<100; j++ )
  {
    Oges solver(cg);
    if( j % 30 == 0 )
      printf("**** Oges: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

  for(j=0; j<1000; j++ )
  {
    SquareMapping square;         // this line is ok by itself
    MappingRC map(square),map2;   // this is ok
    map2.reference(map);          // ok
    if( j % 100 == 0 )
      printf("**** Mapping: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

  for(j=0; j<1000; j++ )
  {
    realCompositeGridFunction u(cg),v; // no leak here
    v=u;                               // no leak here
    if( j % 100 == 0 )
      printf("**** u: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }


  for( j=0; j<100; j++ )
  {
    // CompositeGrid cg2;    // this is ok, no leak
    CompositeGrid cg2 = cg;  // no leak
    if( j % 5 == 0 )
      printf("**** cg2: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

----- */

/* ----

    // cout << "Ogshow: put the GridCollection in frame = " << frameNumber << endl;
    CompositeGrid cg = *u.getCompositeGrid();
    // first destroy any big geometry arrays: (but not the mask)
    cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
    cg.put(*frame,"CompositeGrid");
----- */

  Overture::finish();          
  printf ("Program Terminated Normally! \n");
  return 0;
}

