#include "Overture.h"
#include "HDF_DataBase.h"
#include "GL_GraphicsInterface.h"

int 
getLineFromFile( FILE *file, char s[], int lim);

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  aString inFileName,fileName;

  if( argc ==3 )
  {
    inFileName=argv[1];
    fileName=argv[1];
  }
  else
  {
    cout << "Type: `decompress inFile.hdf outFile.hdf to decompress an Overture .hdf file\n";
    cout << "A decompressed file can usually be read by a newer version of Overture.\n";
    cout << "This function currently only works for grid files, not show files.\n";
    return 1;
  }


  // create and read in a CompositeGrid
  CompositeGrid cg,cg2;
  aString gridName;
  getFromADataBase(cg,inFileName);

  // fileName = "out.hdf";
  
  HDF_DataBase db;
  db.mount(fileName,"I");

  db.setMode(GenericDataBase::noStreamMode);
  
  gridName="decompressed"; // *** fix this ****

  printf("Saving the decompressed file %s\n",(const char*)fileName);

  int streamMode=0;
  db.put(streamMode,"streamMode");
  
  
  cg.put(db,gridName);

  db.unmount();

  // ***** test that we can read the new grid back in ****
  // db.mount(fileName,"R");
  // cg2.get(db,gridName);
  
  // PlotStuff ps;
  // ps.plot(cg2);

  Overture::finish();          
  return 0;
}


