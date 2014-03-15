#include "HDF_DataBase.h"

//
//  HDF_DataBase: example1
//
int
main(int argc, char *argv[] ) 
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O 

  HDF_DataBase root;
  root.mount("ex1.hdf","I");     // mount a new file (I=Initialize)

  floatArray x(Range(-1,2),Range(3,4));
  x=1;
  root.putDistributed(x,"x");               // save an A++ array in the "root" directory

  int num=5;
  root.put(num,"num");           // save an int in the "root" directory
  
  HDF_DataBase subDir1;      
  root.create(subDir1,"stuff","directory");   // create a sub-directory, class="directory"

  aString label; 
  label="my label";
  subDir1.put(label,"label1");   // save a aString in the sub-directory  

  root.unmount();                // flush the data and close the file
    
  cout << "\n ++++Mount the file again, read-only ++++++ \n";

  root.mount("ex1.hdf","R");   // mount read-only

  floatArray x2;
  root.getDistributed(x2,"x");            // get "x"
  x2.display("Here is x2 (should be x2(-1:2,3:4)=1)");
    
  HDF_DataBase subDir2;
  root.find(subDir2,"stuff","directory");
    
  aString label2;
  subDir2.get(label2,"label1"); // get label1
  cout << "label2 from file =[" << (const char *) label2 << "]" << endl;

  root.unmount();

  return 0;
}

