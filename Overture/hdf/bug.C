#include "HDF_DataBase.h"

extern "C"
{
#include "hdf.h"   

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
    GenericDataBase & subDir = *db.virtualConstructor();      // create a derived data-base object
    db.create(subDir,name,"MyClass");                        // create a sub-directory 
    int status;
    status=subDir.put(a1,"a1");
    if( status!=0 )
    {
      FILE *file = fopen("bug.out","w");
      HEprint(file,0); 
      fclose(file);
      throw "error";
    }
    
    status=subDir.put(a2,"a2");
    if( status!=0 )
    {
      FILE *file = fopen("bug.out","w");
      HEprint(file,0);
      fclose(file);
      throw "error";
    }
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
  dataBase.mount("myfile.hdf","I");

  MyClass stuff;

  char buff[200];
  double time0,time;
  for( int i=0; i<9361+10; i++ )
  // for( int i=0; i<1701; i++ )
  {
    sprintf(buff,"myStuff%i",i);
    time0=wallClockTime();
    stuff.put(dataBase,buff);
    time=wallClockTime()-time0;
    if( i % 100 == 0 || i>9360 )
    {
      printf(" i=%i:cpu=%6.3e ",i,time);
      dataBase.printStatistics();
    }
  }
  printf("unmount the file\n");
  time0=wallClockTime();
  dataBase.unmount();
  printf("time to unmount = %e \n",wallClockTime()-time0);

  return 0;
}
