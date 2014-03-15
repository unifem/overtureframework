#include "HDF_DataBase.h"

//
//  HDF_DataBase: example1
//

int
main( ) 
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O 

  int nA,nS,nD,nL;
  cout << "Enter nA, nS, nD, nL : the number of arrays, scalars, directories and labels to save\n";
  cin >> nA >> nS >> nD >> nL;

  HDF_DataBase root;
  root.mount("ex3.hdf","I");     // mount a new file (I=Initialize)

  floatArray x(Range(-1,2),Range(3,4));
  x=1;
  char buff[80];
  for( int i=0; i<nA; i++)
  {
    sprintf(buff,"x%i",i);
    root.put(x,buff);               // save an A++ array in the "root" directory
  }

  int num=5;
  for( int i=0; i<nS; i++)
  {
    sprintf(buff,"num%i",i);
    root.put(num,buff);           // save an int in the "root" directory
  }
  
  for( int i=0; i<nD; i++)
  {
    HDF_DataBase subDir1;      
    sprintf(buff,"stuff%i",i);
    root.create(subDir1,buff,"directory");   // create a sub-directory, class="directory"
  }

  aString label; 
  label="my label";
  for( int i=0; i<nL; i++)
  {
    sprintf(buff,"label%i",i);
    root.put(buff,"label1");   // save a aString in the sub-directory  
  }
  
  root.unmount();                // flush the data and close the file
    

  return 0;
}
