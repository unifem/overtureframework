#include "HDF_DataBase.h"
//
//  HDF_DataBase: example 2
//
class MyClass
{
public:
  float a1,a2;
  MyClass(){ a1=0.; a2=0.; } 
  ~MyClass(){} 
  int put( GenericDataBase & db, const aString & name ) const
  {  // save this object to a sub-directory called "name"
    GenericDataBase & subDir = *db.virtualConstructor();      // create a derived data-base object
    db.create(subDir,name,"MyClass");                        // create a sub-directory 
    subDir.put(a1,"a1");
    subDir.put(a2,"a2");
    delete &subDir;
    return 0;
  }
  int get( const GenericDataBase & db, const aString & name ) 
  { // get this object from a sub-directory called "name"
    GenericDataBase & subDir = *db.virtualConstructor();
    db.find(subDir,name,"MyClass");
    subDir.get(a1,"a1");
    subDir.get(a2,"a2");
    delete &subDir;
    return 0;
  }
};

int
main( ) 
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O 

  HDF_DataBase root;
  root.mount("ex2.hdf","I");     // mount a new file (I=Initialize)

  MyClass m1;
  m1.a1=1.;  m1.a2=2.;
  m1.put(root,"m1");
  root.unmount();                // flush the data and close the file
    
  cout << "\n ++++Mount the file again, read-only ++++++ \n";

  HDF_DataBase root2;
  root2.mount("ex2.hdf","R");   // mount read-only

  MyClass m2;
  m2.get(root2,"m1");
  cout << "m2.a1 =" << m2.a1 << ", m2.a2=" << m2.a2 << endl;
  root2.unmount();

  return 0;
}

