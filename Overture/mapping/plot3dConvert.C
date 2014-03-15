#include "Mapping.h"

#define OPPLT3D  EXTERN_C_NAME(opplt3d)
#define CONVERTP3D EXTERN_C_NAME(convertp3d)

extern "C"
{

  void OPPLT3D(char  filename[], int & iunit,int & fileFormat,int & ngd, int & ng,
               int & nx,int & ny,int & nz, const int len_filename);

  void CONVERTP3D(
     char inFile[], const int & fileFormat, const int & iunit,
     char outFile[],const int & outFileFormat,const int & ounit,
     const int & ngrid,const int & nx,const int & ny,const int & nz, 
     const int & ndr,const int & nds,const int & ndt, real & xy,
     const int len_inFile, const int len_outFile );
}

int
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  aString inFile,outFile;
  
  cout << "Enter the name of the initial plot3d file" << endl;
  cin >> inFile;

  int ng;
  int ngd=1000;  // maximum number of grids
  intArray nx(ngd,3);
  int iunit=23;
  int fileFormat=0;
  OPPLT3D( (char *)((const char*)inFile),iunit,fileFormat,ngd, ng,nx(0,0),nx(0,1),nx(0,2),strlen(inFile) );

  printf("After OPPLT3D: nx=%i, ny=%i, nz=%i \n",nx(0,0),nx(0,1),nx(0,2));
      
  if( fileFormat==0 )
    printf("input file is single grid, formatted \n");
  else if( fileFormat==1 )
    printf("input file is single grid, unformatted \n");
  else if( fileFormat==2 )
    printf("input file is multiple grid, formatted \n");
  else if( fileFormat==3 )
    printf("input file is multiple grid, unformatted \n");
  else
  {
    printf("ERROR: unknown file format! \n");
    return 1;
  }
  
  int outFileFormat;
  cout << "Enter the name of the output file \n";
  cin >> outFile;
  cout << "Enter the file type for the output file \n";
  printf("0 : single grid, formatted \n"
         "1 : single grid, unformatted \n"
         "2 : multiple grid, formatted \n"
         "3 : multiple grid, unformatted\n");
  cin >> outFileFormat;
  
  Range R(0,ng-1);
  int ndx = max(1,max(nx(R,0)));
  int ndy = max(1,max(nx(R,1)));
  int ndz = max(1,max(nx(R,2)));

  RealArray xy(ndx,ndy,ndz,3);

  int ounit=iunit+1; // fortran unit for output
  CONVERTP3D( (char *)((const char*)inFile),fileFormat,iunit,
              (char *)((const char*)outFile),outFileFormat,ounit,
              ng,nx(0,0),nx(0,1),nx(0,2),
              ndx,ndy,ndz,xy(0,0,0,0),strlen(inFile),strlen(outFile));

  return 0;  

}
