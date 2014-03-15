#include "HDF_DataBase.h"

// MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end


int
main( ) 
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  HDF_DataBase::debug=3;
    
  HDF_DataBase dataBase;
  dataBase.mount("myfile.hdf","I");

  dataBase.setMode(HDF_DataBase::noStreamMode);

  floatArray x(4,4),y,z, z0;
  x=1;
  dataBase.put(x,"x");

  cout << "\n  put 'x' again with new values (x=7)" << endl;
  x=7.;
  dataBase.put(x,"x");
  dataBase.get(x,"x");
  x.display("Here is x (SDS) (x=7)");



  intArray ia(2,3);
  ia=23;
  dataBase.put(ia,"ia");
  
  cout << "put a null floatArray, z0, dataPointer = " << x.getDataPointer() << endl;
  dataBase.put(z0,"z0");
  dataBase.get(z0,"z0");
  z0.display("here is z0 after a get");

  int i1,i2;
  i1=1; i2=2;
  dataBase.put(i1,"i1");
  dataBase.put(i2,"i2");
   
  

  // create a sub directory
  HDF_DataBase dir;
  dataBase.create(dir,"myDir","grid");
  x=2.;
  dir.put(x,"y");
  x=3.;
  dir.put(x,"z");

  dir.get(y,"y");
  y.display("here is y");

  float c1=2., c2;
  dir.put(c1,"c1");
  dir.get(c2,"c1");
  cout << "get c1=2 from the database, c2=" << c2 << endl;

  HDF_DataBase dir2;
  dataBase.find(dir2,"myDir","grid");

  x.reshape(Range(-1,2),Range(1,4));
  x=5.;
  dir.put(x,"a");

  dir.get(z,"a");
  z.display("Here is z (SDS) (a(-1:2,1:4)=5)");

  aString label;
  label="This is a label";
  dataBase.put(label,"my label");
  cout << "label=[" << (const char *) label << "], length=" << label.length() << endl;

  cout << "flush the file\n";
  dataBase.flush();

  aString list[2] = { "first list entry", "second entry"  }; 
  dataBase.put(list,"my list",2);    
    


  HDF_DataBase dbGrid[10];
  aString names[10];
  int maxNumber=10, actualNumber, numberSaved;
  cout << "dbGrid[0].className = " << (const char *)dbGrid[0].className << endl;
  cout << "dbGrid[1].className = " << (const char *)dbGrid[1].className << endl;
  cout << "dbGrid[2].className = " << (const char *)dbGrid[2].className << endl;
  
  // make another grid
  HDF_DataBase dir3;
  dataBase.create(dir3,"grid 2","grid");

  // find all grid's
  printf("\n--------\n");
  numberSaved = dataBase.find(dbGrid,names,"grid",maxNumber,actualNumber );
  printf("Find all grid's : actualNumber = %i, numberSaved = %i \n",actualNumber,numberSaved);
  for( int i=0; i<numberSaved; i++ )
    printf("name of grid %i = %s \n",i,(const char *)names[i]);

  // now find all int's
  printf("\n--------\n");
  numberSaved = dataBase.find(names,"int",maxNumber,actualNumber );
  printf("Find all int's : actualNumber = %i, numberSaved = %i \n",actualNumber,numberSaved);
  for( i=0; i<numberSaved; i++ )
    printf("name of int %i = %s \n",i,(const char *)names[i]);

  // now find all floatArray's
  printf("\n--------\n");
  numberSaved = dataBase.find(names,"floatArray",maxNumber,actualNumber );
  printf("Find all floatArray's : actualNumber = %i, numberSaved = %i \n",actualNumber,numberSaved);
  for( i=0; i<numberSaved; i++ )
    printf("name of floatArray %i = %s \n",i,(const char *)names[i]);

  // now find all string's
  printf("\n--------\n");
  numberSaved = dataBase.find(names,"string",maxNumber,actualNumber );
  printf("Find all string's : actualNumber = %i, numberSaved = %i \n",actualNumber,numberSaved);
  for( i=0; i<numberSaved; i++ )
    printf("name of string %i = %s \n",i,(const char *)names[i]);

  dataBase.printStatistics();

  dataBase.unmount();

  cout << "\n ++++Mount the file again, read-only ++++++ \n";

  dataBase.mount("myfile.hdf","R");
  x.redim(0);
  dataBase.get(x,"x");
  x.display("Here is x");

  // This should generate an error because the file is read-only
  dataBase.put(y,"yy");
    
  ia.redim(0);
  dataBase.get(ia,"ia");
  ia.display("here is ia(0:1,0:2)=23");

  aString label2;
  dataBase.get(label2,"my label");
  cout << "my label=[" << (const char *) label2 << "], length=" << label2.length() << endl;

  aString list2[2];
  dataBase.get(list2,"my list",2);    
  cout << "list2[0]=[" << (const char *) list2[0] << "] \n";
  cout << "list2[1]=[" << (const char *) list2[1] << "] \n";

  dataBase.printStatistics();
  dataBase.unmount();

  return 0;
}

