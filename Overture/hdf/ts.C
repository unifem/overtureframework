#include "HDF_DataBase.h"

#include "hdf.h"   
extern "C"
{
  double wallClockTime()
  {
    // this one only works for unix workstation networks
    // make sure that sys/time.h is included somewhere
    unsigned long ustime;
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    ustime = (unsigned long) tp.tv_sec;
    ustime = (ustime * 1000000) + (unsigned long) tp.tv_usec;

    return (((double) ustime) * 1e-6);
  }
};

class MyClass
{
public:
  floatArray a1,a2;
  float b1,b2;
  MyClass(){ a1.redim(5,5); a1=1.; a2.redim(2,2); a2=2.; b1=1.; b2=2.; } 
  ~MyClass(){} 
  int put( GenericDataBase & db, const aString & name ) const
  {  // save this object to a sub-directory called "name"
    GenericDataBase & subDir = (*db.virtualConstructor());      // create a derived data-base object
    db.create(subDir,name,"MyClass");                        // create a sub-directory 
    int status;
    subDir.put(a1,"a1");
    subDir.put(a2,"a2");
    subDir.put(b1,"b1");
    subDir.put(b2,"b2");
    delete &subDir;
    return 0;
  }
  int get( const GenericDataBase & db, const aString & name ) 
  { // get this object from a sub-directory called "name"
    GenericDataBase & subDir = *db.virtualConstructor();
    db.find(subDir,name,"MyClass");
    subDir.get(a1,"a1");
    subDir.get(a2,"a2");
    subDir.get(b1,"b1");
    subDir.get(b2,"b2");
    delete &subDir;
    return 0;
  }
};

int
main( ) 
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  // HDF_DataBase::debug=3;
    
  HDF_DataBase dataBase;
  dataBase.mount("stream.hdf","I");

  floatArray a(5,5), b(3,3);
  a=1.;
  b=2.;
  float x1=5.;
  aString name, name2;
  name="stream";
  aString menu[2] =
  {
    "menu item 1", "menu item 2" 
  };
  aString menu2[2];

  dataBase.setMode(GenericDataBase::streamOutputMode);
  dataBase.put(a,"a");
  dataBase.put(x1,"x1");
  dataBase.put(b,"b");
  dataBase.put(name,"name");
  dataBase.put(menu,"menu",2);
  
  dataBase.printStatistics();
  
  dataBase.setMode(GenericDataBase::normalMode);
  dataBase.printStatistics();
  printf("unmount the data-base \n");
  dataBase.unmount();
  
  a.redim(0);
  b.redim(0);

  HDF_DataBase db;
  printf("mount a new file.. \n");
  db.mount("stream.hdf","R");

  db.setMode(GenericDataBase::streamInputMode);
  db.get(a,"a");
  x1=-1.;
  db.get(x1,"x1");
  db.get(b,"b");
  db.get(name2,"name");
  db.get(menu2,"menu",2);
  
  a.display("Here is a (=1?)");
  printf(" *** x1= %e (=5.) \n",x1);
  b.display("Here is b (=2?)");
  cout << "name2 = [" << name << "], =? [" << name << "]" << endl;
  cout << "menu2[0] = [" << menu2[0]<< "], =? [" << menu[0] << "]" << endl;
  cout << "menu2[1] = [" << menu2[1]<< "], =? [" << menu[1] << "]" << endl;
  
  db.setMode(GenericDataBase::normalMode);
  db.unmount();

//if( TRUE )
//  return 0;

  // *******
  HDF_DataBase db2;
  db2.mount("stream.hdf","I");
  db2.setMode(GenericDataBase::streamOutputMode);

  MyClass stuff;

  char buff[200];
  double time0,time;
  for( int i=0; i<5000; i++ )
  {
    sprintf(buff,"myStuff%i",i);
    time0=wallClockTime();
    stuff.put(db2,buff);
    time=wallClockTime()-time0;
    if( i % 100 == 0 || i>9360 )
    {
      printf(" i=%i:cpu=%6.3e ",i,time);
      db2.printStatistics();
    }
  }
  printf("unmount the file\n");
  time0=wallClockTime();
  db2.setMode(GenericDataBase::normalMode);
  db2.unmount();
  printf("time to unmount = %e \n",wallClockTime()-time0);

  return 0;
}
