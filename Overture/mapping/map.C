//
// Test out the Mapping Class Library
//
// 960215: 0 bytes lost MIU and MLK  (with some system stuff turned off in .purify)

#include <A++.h>
#include <string.h>
// #include <tlist.h>  // include list Template from jss

#include "realPrecision.h"  // define real to be float or double

#include "Mapping.h"   // Base Class
#include "MappingRC.h" // Mapping Reference Counted Class
#include "MappingInformation.h" // Mapping Reference Counted Class
#include "maputil.h"   // utilities, matrix, compose, box
#include "StretchMapping.h"   // stretching functions
#include "CircleMapping.h"    // Here is a derived class to define a circle in 2D
#include "CylinderMapping.h"  // Here is a derived class to define a Cylindrical Surface in 3D
#include "BoxMapping.h"

#include "HDF_DataBase.h"

MemoryManagerType memoryManager;  // This will delete allocated memory at the end

//=================================================================
// Class to hold a pointer to a mapping
//================================================================
class Container
{ 
public:
  Mapping *mapPointer;
  int getMap;          // True if mapPointer set through get

  Container()
  {
    getMap=FALSE;
  }
  ~Container()
   { if( (Mapping::debug/4) % 2 )
     cout << " Container::Destructor called" << endl;
     if( getMap )
       delete mapPointer;  // calls appropriate (virtual) destructor
   }

  void get( const GenericDataBase & dir, const aString & name )
   {
     GenericDataBase & subDir = *dir.virtualConstructor();
     dir.find(subDir,name,"Container");
     
     // Look for the className of the Mapping:
     GenericDataBase *mappingDir = dir.virtualConstructor();
     subDir.find(*mappingDir,"containedMapping","Mapping");

     aString mappingClassName;
     mappingDir->get( mappingClassName,"className" );
     cout << "Container: mappingClassName from file = " << mappingClassName << endl;

     // Make an instance of the appropriate derived Mapping class
     mapPointer = Mapping::makeMapping( mappingClassName );
     getMap=TRUE;
     mapPointer->get( subDir,"containedMapping" );   // get the mapping

     delete & subDir;
   }
  void put( GenericDataBase & dir, const aString & name ) const
   {  
     GenericDataBase & subDir = *dir.virtualConstructor();
     dir.create(subDir,name,"Container");

     mapPointer->put( subDir,"containedMapping" );  // save the mapping

     delete & subDir;
   }

};

// --- pass a Mapping by value ----
void
passByValue( CircleMapping map )
{
  cout << "*** pass a Mapping by value \n";

  RealArray r(3);
  RealArray x(3);
  RealArray xr(3,3);
  RealArray rx(3,3);

  r=0.;
  r(axis1)=.5;
  map.map( r,x,xr );
  cout << " For map(circle): Here is x(.5) : " << x(axis1) << " , " << x(axis2) << endl; 
  r=0.;
  map.inverseMap(x,r,rx);
  cout << " For inverseMap(circle): r=" << r(0) << endl;
}  



int main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

  cout << "Enter debug\n";
  cin >> Mapping::debug;         // set the debug parameter for mappings
//  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test of the mapping class =====" << endl;
  cout << "Pi =" << Pi << endl;
  cout << "twoPi =" << twoPi << endl;


  RealArray r(3);
  RealArray x(3);
  RealArray xr(3,3);
  RealArray rx(3,3);
  RealArray t(3);
  RealArray tx(3,3);

  RealArray r1(3,10);
  RealArray x1(3,10), x2(3,10);
  RealArray xr1(3,3,10), xr2(3,3,10);

// -- Define a circle

  CircleMapping circle;     // Define a circle, radius=1, centre=(0,0)
  circle.setName(Mapping::mappingName,"circle");

  cout << " circle.getDomainDimension() = " 
         << circle.getDomainDimension() << endl;
  cout << " circle.getRangeDimension()  = " 
       <<   circle.getRangeDimension()  << endl;
  cout << " circle:getName(Mapping::mappingName) = " 
         << circle.getName(Mapping::mappingName)  << endl;

  r(axis1)=.5;
  circle.map( r,x,xr );
  cout << " For circle: Here is x(.5) : " << x(axis1) << " , " << x(axis2) << endl; 

  cout << " ---Call circle map with an array of values:" << endl;
  for( int i=0; i<10; i++ )
    r1(axis1,i)=i/9.; 
  circle.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Circle: r= %6.3f , x = %7.4f , xr = %8.5f\n",
      r1(axis1,i),x1(axis1,i),xr1(axis1,axis1,i));

  cout << " ---Call circle map with an array of values (compute x only):" << endl;
  circle.map( r1,x2 );   // only get x1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Circle: r= %6.3f , x = %7.4f \n",r1(axis1,i),x2(axis1,i));

  cout << " ---Call circle map with an array of values (compute xr only):" << endl;
  circle.map( r1,Overture::nullRealArray(),xr2 );  // only get xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Circle: r= %6.3f , xr = %8.5f\n",r1(axis1,i),xr2(axis1,axis1,i));

  for( ; Mapping::debug & 8 ; )
  {
    cout << "Enter x(1),x(2) for circle inverse (0,0)=stop" << endl;
    cin >> x(axis1) >> x(axis2);
    if( x(axis1)==0. && x(axis2)==0. ) 
      break;
    circle.inverseMap( x,r,rx );
    circle.map( r,t );
    printf( "Circle: x = (%6.3f,%6.3f), r = %6.3f, map(r) = (%6.3f,%6.3f)\n",
	   x(axis1),x(axis2),r(axis1),t(axis1),t(axis2));
  }

  // ---- pass a Mapping by value to test the copy constructor ---
  circle.setBasicInverseOption( Mapping::canDoNothing );  // turn off inverse if it exists
  passByValue( circle );
  circle.setBasicInverseOption( Mapping::canInvert );


  CylinderMapping cylinder(1.,0.,0.,1.);     // Define a circle, radius=1, centre=(0,0)
  for( ; Mapping::debug & 8; )
  {
    cout << "Enter x(1),..,x(3) for cylinder inverse (0,0,0)=stop" << endl;
    cin >> x(axis1) >> x(axis2) >> x(axis3) ;
    if( x(axis1)==0. && x(axis2)==0. && x(axis3)==0. ) 
      break;
    cylinder.inverseMap( x,r,rx );
    cylinder.map( r,t );
    printf( "Cylinder: x = (%6.3f,%6.3f,%6.3f), r = (%6.3f,%6.3f), map(r) = (%6.3f,%6.3f,%6.3f)\n",
	   x(axis1),x(axis2),x(axis3),r(axis1),r(axis2),t(axis1),t(axis2),t(axis3));
  }
  
    

// -- Test the stretch mapping:

/*
  StretchMapping stretch( 2, 0 );               // two layers, zero intervals
  stretch.setLayerParameters( 0, 1., 10., .25 ); // set layer 0, a,b,c
  stretch.setLayerParameters( 1, 1., 10., .75 ); // set layer 1, a,b,c
  stretch.setIsPeriodic(FALSE);               // default is FALSE
*/
  StretchMapping stretch( 0, 1 );               // zero layers, one intervals
  stretch.setName(Mapping::mappingName,"stretch");
  stretch.setIntervalParameters( 0, 5., 20., .25 ); // spacing is smaller
  stretch.setIntervalParameters( 1, 0.,  0., .75 ); // between .25 and .75
  stretch.setIsPeriodic(FALSE);               // 

  cout << " ---Test the StretchMapping stretch:" << endl;
  for( i=0; i<=8 ; i++ )
    { r(axis1)=i*.125;
      stretch.map( r,x,xr );
      printf("stretch: r= %6.3f , x = %7.4f , xr = %8.5f\n",
              r(axis1),x(axis1),xr(axis1,axis1) );
    }

// -- Define a box in 3D

  cout << " ---Create a BoxMapping cube: " << endl;

  BoxMapping cube(1.,2.,1.,2.,1.,2.)  ;  // [1,2]x[1,2]x[1,2]
  cube.setName(Mapping::mappingName,"cube");

  r(axis1)=.25; r(axis2)=.5; r(axis3)=.75;

  cube.map( r,x,xr );
  cout << " Evaluate cube, r=(" 
       << r(axis1) << ", " << r(axis2) << ", " << r(axis3) << ")" << endl;
  cout << " Evaluate cube, x=(" 
       << x(axis1) << ", " << x(axis2) << ", " << x(axis3) << ")" << endl;

  cube.map( r,x,xr );
  cout << " Evaluate cube, xr=(" 
   <<xr(axis1,axis1)<<", "<<xr(axis1,axis2)<<", "<<xr(axis1,axis3) <<")"<< endl
       << "                   (" 
   <<xr(axis2,axis1)<<", "<<xr(axis2,axis2)<<", "<<xr(axis2,axis3) <<")"<< endl
       << "                   (" 
   <<xr(axis3,axis1)<<", "<<xr(axis3,axis2)<<", "<<xr(axis3,axis3) <<")"<< endl;


  if( cube.getInvertible() )
    cout << "The BoxMapping cube is invertible" << endl;

  r=0;
  cube.inverseMap( x,r,rx );
  cout << " Evaluate the Inverse Box Mapping, r=(" 
       << r(axis1) << ", " << r(axis2) << ", " << r(axis3) << ")" << endl;

  cube.display(" Here is cube.display:");

  BoxMapping grida(0.,.5,0.,.5,0.,.5)  ;  // Define grid to be a cube
  grida.setName(Mapping::mappingName,"grida");

  MatrixMapping rotScaleShift  ;     // Define a matrix mapping 
  rotScaleShift.setName(Mapping::mappingName,"rotScaleShift");

//  rotScaleShift.rotate( xAxis, Pi/2. );  // rotate about x axis
//  rotScaleShift.rotate( yAxis, Pi/2. );  // rotate about y axis
  rotScaleShift.rotate( zAxis, Pi/2. );  // rotate about z axis
  rotScaleShift.scale( 2.,1.,1. );    // scale by 2 in x-direction
  rotScaleShift.shift( 0.,1.,0. );   // shift by 1 in y direction
  
// Now compose two mappings 

  ComposeMapping gridc( grida,rotScaleShift );    // define a mapping by composition
  gridc.setName(Mapping::mappingName,"gridc");
  r(axis1)=.5; r(axis2)=.5; r(axis3)=.5;

  gridc.map( r,x,xr );
  cout << " Evaluate the ComposeMapping, r=(" 
       << r(axis1) << ", " << r(axis2) << ", " << r(axis3) << ")" << endl;
  cout << " Evaluate the ComposeMapping, x=(" 
       << x(axis1) << ", " << x(axis2) << ", " << x(axis3) << ")" << endl;

  gridc.inverseMap( x,t,tx );
  cout << " Evaluate the inverse ComposeMapping, t=(" 
       << t(axis1) << ", " << t(axis2) << ", " << t(axis3) << ")" << endl;

  cout << "grida.getName( Mapping::domainAxis1Name ) =" 
       <<  grida.getName( Mapping::domainAxis1Name ) << endl;
  cout << "grida.getName( Mapping::rangeAxis1Name )  =" 
       <<  grida.getName( Mapping::rangeAxis1Name ) << endl;


  cout << "---Mount a DataBase----" << endl;
  HDF_DataBase root;
  root.mount("map.dat","I");  // Initialize a database file

  circle.display(" put this circle mapping...");
  circle.put( root,"circle" );   // save the circle mapping
    
  stretch.put( root,"stretch" ); // save a stretch mapping

  cout << " Add various mappings to the Mapping::staticMapList().. " << endl;

  Mapping::staticMapList().add( &circle );
  Mapping::staticMapList().add( &stretch );
  Mapping::staticMapList().add( &gridc );
  Mapping::staticMapList().add( &cube );

  cout << " Mapping::staticMapList().start->val->getname(Mapping::mappingName) = " << 
   Mapping::staticMapList().start->val->getName(Mapping::mappingName) << endl;

  cout << " Mapping::staticMapList().end->val->getname(Mapping::mappingName) = " << 
   Mapping::staticMapList().end->val->getName(Mapping::mappingName) << endl;

  Mapping *ptr =Mapping::makeMapping("CircleMapping");
  cout << " ptr->getName(Mapping::mappingName) = " << 
            ptr->getName(Mapping::mappingName) << endl;
  delete ptr;

  ptr=Mapping::makeMapping("StretchMapping");
  cout << " ptr->getName(Mapping::mappingName) = " << 
            ptr->getName(Mapping::mappingName) << endl;
  delete ptr;

  CircleMapping circle2;       // make a new circle mapping

  circle2.get( root,"circle"); // read in from the dsk
  circle2.setName(Mapping::mappingName,"circle2");
  circle.display(" circle2 mapping after get(circle)...");
  circle2.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" circle2: r= %6.3f , x = %7.4f , xr = %8.5f\n",
      r1(axis1,i),x1(axis1,i),xr1(axis1,axis1,i));

  cout << " circle2.getDomainDimension() = " 
       <<   circle2.getDomainDimension() << endl;
  cout << " circle2.getName( Mapping::mappingName ) = " 
       <<   circle2.getName( Mapping::mappingName ) << endl;

  StretchMapping stretch2;
  stretch2.setName(Mapping::mappingName,"stretch2");
  stretch2.get( root,"stretch");
  cout << "strecth2.getClassName = " << (const char*) stretch2.getClassName() << endl;
  cout << endl << "Here is stretch2 after get(stretch) \n";
  // stretch.display("Here is stretch");
  // stretch2.display("Here is stretch2 = get(stretch)");
  for( i=0; i<=8 ; i++ )
    { r(axis1)=i*.125;
      stretch2.map( r,x,xr );
      printf("stretch2: r= %6.3f , x = %7.4f , xr = %8.5f\n",
              r(axis1),x(axis1),xr(axis1,axis1) );
    }

  cout << "  ---Now make a container that holds a ptr to a mappings..." << endl;
  Container container;
  container.mapPointer = & circle2; 
  container.put( root, "container" );

  cout << "  ---get the container from the data base " << endl;
  cout << "     (the container must figure out how to construct the mappings) " << endl;

  Container container2;
  container2.get( root, "container" );
  cout << " container2: " 
       << "domainDimension = " << container2.mapPointer->getDomainDimension()
       << ", Mapping::mappingName = " 
       << container2.mapPointer->getName(Mapping::mappingName)
       << endl;

  cout << "  ---Now make a MappingRC from circle2" << endl;
  MappingRC maprc( circle2 );
  maprc.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf("maprc(circle2): r= %6.3f , x = %7.4f , xr = %8.5f\n",
      r1(axis1,i),x1(axis1,i),xr1(axis1,axis1,i));
  

  maprc.put( root, "maprc" );

  cout << "  ---get the MappingRC from the data base " << endl;
  cout << "     (the container must figure out how to construct the mappings) " << endl;

  MappingRC maprc2;
  maprc2.get( root, "maprc" );
  cout << " maprc2: " 
       << "domainDimension = " << maprc2.getDomainDimension()
       << ", Mapping::mappingName = " << maprc2.getName(Mapping::mappingName)
       << endl;
  maprc2.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf("maprc2: r= %6.3f , x = %7.4f , xr = %8.5f\n",
      r1(axis1,i),x1(axis1,i),xr1(axis1,axis1,i));

  cout << "Create maprc3(circle2.getClassName()) " << endl;
  MappingRC maprc3( circle2.getClassName() );
  maprc3.reference( maprc );
  maprc3.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf("maprc3(circle2.getClassName): r= %6.3f , x = %7.4f , xr = %8.5f\n",
      r1(axis1,i),x1(axis1,i),xr1(axis1,axis1,i));
    
  cout << "maprc3=stretch2..." << endl;
  maprc3=stretch2;  // deep copy
  // maprc3.display("Here is maprc3 (=stretch2)");
  for( i=0; i<=8 ; i++ )
  {
    r(axis1)=i*.125;
    maprc3.map( r,x,xr );
    printf("maprc3: r= %6.3f , x = %7.4f , xr = %8.5f\n",r(axis1),x(axis1),xr(axis1,axis1) );
  }
    

  MappingInformation mapInfo;
  mapInfo.mappingList.addElement( maprc2 );
  mapInfo.mappingList.addElement( grida );
    
  cout << "mapInfo.mappingList[0].getName(Mapping::mappingName) =" 
       << mapInfo.mappingList[0].getName(Mapping::mappingName) << endl;
  cout << "mapInfo.mappingList[1].getName(Mapping::mappingName) =" 
       << mapInfo.mappingList[1].getName(Mapping::mappingName) << endl;
    


  root.unmount();              // Flush data and close the database file
  cout << "---DataBase file unmounted----" << endl;

  printf("Finished test...\n");

  return 0;
}
