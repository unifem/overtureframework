#include "DataFormats.h"

#include "conversion.h"

#include "MappingInformation.h"
#include "DataPointMapping.h"
#include "UnstructuredMapping.h"
#include "arrayGetIndex.h"
#include <string.h>
#include "plyFileInterface.h"
#include "display.h"

#define OPPLT3D    EXTERN_C_NAME(opplt3d)
#define RDPLT3D    EXTERN_C_NAME(rdplt3d)
#define RDPLT3DS   EXTERN_C_NAME(rdplt3ds)
#define RDPLT3DD   EXTERN_C_NAME(rdplt3dd)
#define RDPLT3DQS  EXTERN_C_NAME(rdplt3dqs)
#define RDPLT3DQD  EXTERN_C_NAME(rdplt3dqd)
#define OPINGRID   EXTERN_C_NAME(opingrid)
#define RDINGRID   EXTERN_C_NAME(rdingrid)
#define CLINGRID   EXTERN_C_NAME(clingrid)
#define WRPLT3DS   EXTERN_C_NAME(wrplt3ds)
#define WRPLT3DD   EXTERN_C_NAME(wrplt3dd)
#define WRINGRID   EXTERN_C_NAME(wringrid)
#define CLOSEPLT3D EXTERN_C_NAME(closeplt3d)

int 
getLineFromFile( FILE *file, char s[], int lim);

#define SC (char *)(const char *)
extern "C"
{

  void OPPLT3D(char  filename[], int & iunit,int & fileFormat,int & ngd, int & ng,
               int & nx,int & ny,int & nz, int & nq, int & nqc, const int len_filename);

  void RDPLT3D(int & fileFormat,int & iunit, const int & grid, int & nx, int & ny, int & nz,
	       int & nd, int & ndra, int & ndrb, int & ndsa, int & ndsb, int & ndta, int & ndtb, real & xy,
               const int & readIblank, int & iblank, int & ierr );

  // single precision:
  void RDPLT3DS(int & fileFormat,int & iunit, const int & grid, int & nx, int & ny, int & nz,
	       int & nd, int & ndra, int & ndrb, int & ndsa, int & ndsb, int & ndta, int & ndtb, float & xy,
               const int & readIblank, int & iblank, int & ierr );
  // double precision:
  void RDPLT3DD(int & fileFormat,int & iunit, const int & grid, int & nx, int & ny, int & nz,
	       int & nd, int & ndra, int & ndrb, int & ndsa, int & ndsb, int & ndta, int & ndtb, double & xy,
               const int & readIblank, int & iblank, int & ierr );

  // q file
  void RDPLT3DQS(int & fileFormat,int & iunit, const int & grid, int & nx, int & ny, int & nz,
	       const int & nq, 
               const int & ndra, const int & ndrb, 
               const int & ndsa, const int & ndsb, 
               const int & ndta, const int & ndtb, 
               float & q,
               int & nqc, float & fsmach, float & alpha, float & re, float & time, float & gaminf, float & rgas, 
               int & ierr );

  void RDPLT3DQD(int & fileFormat,int & iunit, const int & grid, int & nx, int & ny, int & nz,
	       const int & nq, 
               const int & ndra, const int & ndrb, 
               const int & ndsa, const int & ndsb, 
               const int & ndta, const int & ndtb, 
               double & q,
               int & nqc, double & fsmach, double & alpha, double & re, double & time, double & gaminf, double & rgas, 
               int & ierr );

  void OPINGRID(char filename[], char mtype[], int & iunit, int & rdim, int & ddim, int & nnode, int & nelem, int & nemax, const int len_filename, const int len_mtype);
 
  void CLINGRID(int &iunit);

  void RDINGRID(int & iunit, char mtype[], int & rdim, int & ddim, int & nnode, int & nelem, int &nemax, real &xyz, int &elements, int &tags, const int len_mtype);

  void WRINGRID(char filename[], int & iunit, int & rdim, int & ddim, int & nnode, int & nelem, int & nemax, real & xyz, int & elements, int & tags, const int len_filename);

   void WRPLT3DS(char filename[], const int & fileFormat, const int & iunit,const int & ng,const int & grid,
                 const int & nx, const int & ny, const int & nz,
                 const int & nd, const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb,
                 const int & ndta, const int & ndtb, const float & xy, 
                 const int & writeIblank, const int & iblank, int & ierr, const int len_filename );

   void WRPLT3DD(char filename[], const int & fileFormat, const int & iunit,const int & ng,const int & grid,
                 const int & nx, const int & ny, const int & nz,
                 const int & nd, const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb,
                 const int & ndta, const int & ndtb, const double & xy, 
                 const int & writeIblank, const int & iblank, int & ierr, const int len_filename );

  void CLOSEPLT3D(const int & iunit);
}



///   DataFormats::
///   DataFormats(GenericGraphicsInterface *ggiPointer /* =NULL */ )
//  // =====================================================================================
/// \param / /Description:  Constructor.
///   //   This class can be used to read and write files in various data formats. Currently
///   //  supported formats are Plot3d and Ingrid.
///   // 
/// \param / /ggiPointer (input) : supply a graphics interface to use when querying for info.
//  // =====================================================================================
//  {
//    giPointer=ggiPointer;
//  }

//  DataFormats::
//  ~DataFormats()
//  {
//  }

///   int DataFormats::
///   setGraphicsInterface( GenericGraphicsInterface *ggiPointer /* =NULL */ )
//  // =====================================================================================
/// \param / /Description:
///   //   Supply a graphics interface to use when querying for info. 
/// \param / /ggiPointer (input) : supply a graphics interface to use when querying for info.
//  // =====================================================================================
//  {
//    giPointer=ggiPointer;
//    return 0;
//  }


int DataFormats::
readPlot3d(DataPointMapping & dpm,
	   int gridToRead /* =-1 */,
	   const aString & gridFileName /* =nullString */,
	   intArray *maskPointer /* =NULL */,
	   const bool expectIblank /* =false */ )
// ==============================================================================
/// \details 
///    Read in a single grid from a file, optionally read a mask (iblank) array
// =====================================================================================
{
  MappingInformation mapInfo;
  realArray *qp=NULL;
  return readPlot3d(  mapInfo,-1,gridFileName,&dpm,qp,Overture::nullRealArray(),maskPointer,expectIblank );
}



int DataFormats::
readPlot3d(MappingInformation & mapInfo, 
           const aString & gridFileName,
           intArray *&maskPointer,
	   const bool expectIblank /* =false */ )
// ==============================================================================
/// \details 
///  Read in all grids from a file, optionally read the mask (iblank) arrays
// =====================================================================================
{
  realArray *qp=NULL;
  return readPlot3d(  mapInfo,-1,gridFileName,NULL,qp,Overture::nullRealArray(),maskPointer,expectIblank );
}

int DataFormats::
readPlot3d(realArray & q, 
           RealArray & par, 
           const aString & qFileName /* =nullString */)
// ==============================================================================
/// \details 
///   Read in a solution array and parameters from a file
/// 
///  par (input/output) : must be dimensioned to receive values on output.
// =====================================================================================
{
  MappingInformation mapInfo;
  intArray *mp;
  realArray *qp = &q;
  return readPlot3d(  mapInfo,-1,qFileName,NULL,qp,par,mp,false );
}

// read in all solutions and parameters from a file
int DataFormats::
readPlot3d(realArray *& q, RealArray & par, 
	   const aString & qFileName/*=nullString*/)
// ==============================================================================
/// \details 
///   Read in all solution and parameters from a multiple grid q file
///  q (input/output)   : arrays containing the solution for each grid
///  par (input/output) : arrays containing the parameters for each grid
/// 
// =====================================================================================
{
  MappingInformation mapInfo;
  intArray *mp;
  return readPlot3d(  mapInfo,-1,qFileName,NULL,q,par,mp,false );
}

int DataFormats::
readPlot3d(MappingInformation & mapInfo, 
           int gridToRead,
           const aString & plot3dFileName,
           DataPointMapping *dpmPointer,
           realArray *&qPointer ,
           RealArray & par  /* = Overture::nullRealArray() */,
           intArray *&maskPointer /* =NULL */,
	   const bool expectIblank /* =false */ )
// =========================================================================================
/// \param Access level: protected.
/// \details 
///     Read in grids from a plot3d file. If dpmPointer is supplied then try to create one
///  grid; otherwise read in all grids, creating a dataPointMapping for each one and add these
///  to the mapInfo.mappingList.
///  
/// \param mapInfo (input):
/// \param plot3dFileName (input) : optional name of the file
/// \param dpmPointer (input) : if not NULL, create this Mapping. 
/// \return  0=success, 1=failure.
// =========================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert(giPointer!=NULL);
  GenericGraphicsInterface & gi = *giPointer; 
    

  FILE *fp=NULL;
  aString fileName;

  if( plot3dFileName!="" && plot3dFileName!=" " )
    fileName=plot3dFileName;
  else
    gi.inputString(fileName,"Enter the name of the plot3d file");

  fp=fopen((const char*)fileName,"r");

  int attempts=0;
  while( fp==NULL && attempts<5 )
  {
    gi.inputString(fileName,"File not found, enter another name for the plot3d file");
    fp=fopen((const char*)fileName,"r");
    attempts++;
  }
  if( attempts>=5 )
  {
    gi.outputString("Too many tries");
    return 1;
  }
  int numberOfBytes=0;
  while( fgetc(fp)!=EOF )
    numberOfBytes++;
  printf("Number of bytes in the file =%i\n",numberOfBytes);

  fclose(fp);

  int ng=1;
  int ngd=1000;  // maximum number of grids
  int nq=0, nqc=0;
  
  IntegerArray nx(ngd,3);
  int iunit=23;
  int fileFormat=0;
  OPPLT3D( (char *)((const char*)fileName),iunit,fileFormat,ngd, ng,nx(0,0),nx(0,1),nx(0,2),nq,nqc,strlen(fileName) );

  if( fileFormat==0 )
    printf("input file is single grid, formatted nx=%i ny=%i nz=%i \n",nx(0,0),nx(0,1),nx(0,2));
  else if( fileFormat==1 )
    printf("input file is single grid, unformatted  nx=%i ny=%i nz=%i \n",nx(0,0),nx(0,1),nx(0,2));
  else if( fileFormat==2 )
    printf("input file is multiple grid, formatted \n");
  else if( fileFormat==3 )
    printf("input file is multiple grid, unformatted \n");
  else if( fileFormat==4 )
    printf("input file is single grid, 2D, formatted  nx=%i ny=%i \n",nx(0,0),nx(0,1));
  else if( fileFormat==5 )
    printf("input file is single grid, 2D, unformatted nx=%i ny=%i \n",nx(0,0),nx(0,1));
  else if( fileFormat==6 )
    printf("input file is a 'q file', single grid, formatted nx=%i ny=%i nz=%i nq=%i nqc=%i \n",
              nx(0,0),nx(0,1),nx(0,2),nq,nqc);
  else if( fileFormat==7 )
    printf("input file is a 'q file', single grid, unformatted nx=%i ny=%i nz=%i nq=%i nqc=%i \n",
              nx(0,0),nx(0,1),nx(0,2),nq,nqc);
  else if( fileFormat==8 )
    printf("input file is a 'q file', multiple grids, formatted nx=%i ny=%i nz=%i nq=%i nqc=%i \n",
              nx(0,0),nx(0,1),nx(0,2),nq,nqc);
  else if( fileFormat==9 )
    printf("input file is a 'q file', multiple grids, unformatted nx=%i ny=%i nz=%i nq=%i nqc=%i \n",
              nx(0,0),nx(0,1),nx(0,2),nq,nqc);
  else
  {
    printf("ERROR: unknown file format! \n");
    return 1;
  }
  enum Precision
  {
    defaultPrecision=0,
    singlePrecision,
    doublePrecision
  } filePrecision = defaultPrecision;
  
  bool unformatted=fileFormat==1 || fileFormat==3 || fileFormat==5 || fileFormat==7 || fileFormat==9;

  if( fileFormat>=6 && fileFormat<=9 )
  {
    if( qPointer!=0 )
    {
      printF("This is apparently not a grid file but a solution file\n");
      if( unformatted && numberOfBytes < sizeof(double)*nq*nx(0,0)*nx(0,1)*nx(0,2) )
      {
	printF("The file is probably single precision since it is not long enough to be double precision\n");
	filePrecision = singlePrecision;
      }

      int gridStart=0, gridEnd=ng-1;
      if( gridToRead!=-1 )
      {
        gridStart=gridToRead;
	gridEnd=gridToRead;
      }
      

      for( int grid=gridStart; grid<=gridEnd; grid++ )
      {
	Range I1(1,nx(grid,0)), I2(1,nx(grid,1)), I3(1,nx(grid,2)), N(0,nq-1);

	realArray & q = qPointer[grid-gridStart];
	q.redim(I1,I2,I3,N);

	printF("readPlot3d: read q file for grid=%i. (nx,ny,nz)=(%i,%i,%i) nq=%i\n",
	       grid,nx(grid,0),nx(grid,1),nx(grid,2),nq);

	real fsmach,alpha,re,time,gaminf;
	RealArray rgas(max(1,nqc));
	int ierr=0;
	if( filePrecision == singlePrecision )
	{
	  floatArray q0(I1,I2,I3,N);
	  float fsmach0,alpha0,re0,time0,gaminf0;
	  floatSerialArray rgas0(max(1,nqc));
	  RDPLT3DQS(fileFormat,iunit, grid+1, nx(0,0), nx(0,1), nx(0,2),
		    nq,
		    q0.getBase(0),q0.getBound(0),
		    q0.getBase(1),q0.getBound(1),
		    q0.getBase(2),q0.getBound(2),
		    *q0.getDataPointer(),nqc,fsmach0,alpha0,re0,time0,gaminf0,*rgas0.getDataPointer(),
		    ierr);
	  equals(q,q0);
	  equals(rgas,rgas0);
	  fsmach=fsmach0; alpha=alpha0; re=re0; time=time0; gaminf=gaminf0; 
	
	}
	else
	{
	  doubleArray q0(I1,I2,I3,N);
	  double fsmach0,alpha0,re0,time0,gaminf0;
	  doubleSerialArray rgas0(max(1,nqc));
	  RDPLT3DQD(fileFormat,iunit, grid+1, nx(0,0), nx(0,1), nx(0,2),
		    nq,
		    q0.getBase(0),q0.getBound(0),
		    q0.getBase(1),q0.getBound(1),
		    q0.getBase(2),q0.getBound(2),
		    *q0.getDataPointer(),nqc,fsmach0,alpha0,re0,time0,gaminf0,*rgas0.getDataPointer(),
		    ierr);

	  // display(q0(I1,I2,I3,0),"DataFormats: q0(I1,I2,I3,0)","%5.1f ");

	  equals(q,q0);

	  // display(q(I1,I2,I3,0),"DataFormats: q(I1,I2,I3,0)","%5.1f ");

	  equals(rgas,rgas0);
	  fsmach=fsmach0; alpha=alpha0; re=re0; time=time0; gaminf=gaminf0; 
	}
	if( par.getLength(0)>0 )
	{
	  par.redim(6);
	  par=0.;
	  par(0)=fsmach; par(1)=alpha; par(2)=re; par(3)=time; par(4)=gaminf;
	}
	for( int n=0; n<nq; n++ )
	{
	  printf(" readPlot3d: component %i min=%e max=%e \n",n,min(q(I1,I2,I3,n)),max(q(I1,I2,I3,n)));
	}
      
	if( nx(grid,2)==3 ) // this is really a 2d computation.
	{
	  Range N2(0,nq-2);
	  realArray q0(I1,I2,1,N2);

	  q0(I1,I2,0,0)=q(I1,I2,1,0);
	  q0(I1,I2,0,1)=q(I1,I2,1,1);
	  q0(I1,I2,0,2)=q(I1,I2,1,3);  // throw away rho*v
	  for( int n=3; n<nq-1; n++ )
	    q0(I1,I2,0,n)=q(I1,I2,1,n+1);

	  q.redim(I1,I2,1,N2);
	  q=q0;
	}
	
      }
      
      return 0;
    }
    else // qPointer==0
    {
      printf("This is apparently not a grid file but a solution file\n");
      return 2;
    }
  }
  

  int numberOfDimensionsInFile=3;
  if( fileFormat==4 || fileFormat==5 )
    numberOfDimensionsInFile=2;
  
  int grid;
  int totalNumberOfGridPoints=0;
  bool gridIsTwoDimensional=true;
  for( int grid=0; grid<ng; grid++ )
  {
    totalNumberOfGridPoints+=nx(grid,0)*nx(grid,1)*nx(grid,2);
    gridIsTwoDimensional=gridIsTwoDimensional && nx(grid,2)==1;
    
  }
  if( gridIsTwoDimensional ) // we miss the case of multiple grids 2D when we open the file -- fix this --
  {
    numberOfDimensionsInFile=2;
    if( fileFormat==0 )
    {
      fileFormat=4;
      printf("...changed to: input file is single grid, 2D, formatted  nx=%i ny=%i \n",nx(0,0),nx(0,1)); 
    }
    else if( fileFormat==1 )
    {
      fileFormat=5;
      printf("...changed to: input file is single grid, 2D, unformatted nx=%i ny=%i \n",nx(0,0),nx(0,1));
    }
    
  }
  
  int length=fileName.length();
  // printf(" fileName(length-5,length-1)=[%s]\n", (const char*)fileName(length-5,length-1));
  if( length>5 && fileName.substr(length-5,length-1)==".surf" )
  {
    // --- check for .surf files --- *wdh* 2016/11/16 
    printF("This appears to be a surface grid file since fileName ends in [.surf], setting rangeDimension==3.\n");
    numberOfDimensionsInFile=3;
  }
  


  char buff[80];
  IntegerArray dimensions(2,3);
//  if( unformatted && numberOfBytes < sizeof(double)*ng*nx(0,0)*nx(0,1)*nx(0,2)*numberOfDimensionsInFile )
  if( unformatted && numberOfBytes < sizeof(double)*totalNumberOfGridPoints*numberOfDimensionsInFile )
  {
    printf("The file is probably single precision since it is not long enough to be double precision\n");
    filePrecision = singlePrecision;
  }
  bool iblankIsThere=expectIblank; // *wdh* 110514 FALSE;
  if( unformatted )
  {
    int bytesPerNum = filePrecision == singlePrecision ? sizeof(float) : sizeof(double) ;
//    if( numberOfBytes >= ng*nx(0,0)*nx(0,1)*nx(0,2)*(numberOfDimensionsInFile*bytesPerNum+sizeof(int)) )
    if( numberOfBytes >= totalNumberOfGridPoints*(numberOfDimensionsInFile*bytesPerNum+sizeof(int)) )
    {
      printf("This grid file probably has the iblank mask array in it since there is room for it\n");
      iblankIsThere=TRUE;
    }
  }


  int gridToUse=-1;
  if( ng>1 && dpmPointer!=0 )
  {
    char buff[80];
    aString answer;
	     
    gi.inputString(answer,
				  sPrintF(buff,"There are %i grids in this file. Enter one to use 0,...,%i",ng,ng-1));
    gridToUse=0;
    sScanF(answer,"%i",&gridToUse);
    if( gridToUse<0 || gridToUse>ng )
    {
      printf("Invalid grid to use: %i, will read grid 0\n",gridToUse);
      gridToUse=0;
    }
  }
  const int gridStart= gridToUse>=0 ? gridToUse : 0;
  const int gridEnd  = gridToUse>=0 ? gridToUse : ng-1;

  if ( gridToUse<0 && !maskPointer && iblankIsThere )
    maskPointer = new intArray[ng];

  for( grid=gridStart; grid<=gridEnd; grid++ )
  {

    // allocate space to hold the data points:  
    int extra=0;
    int domainDimension=3;
    dimensions.redim(2,3);
    dimensions(Start,0)=1-extra;
    dimensions(End  ,0)=nx(grid,0)+extra;
    if( nx(grid,1)==1 )
    {
      domainDimension--;
      dimensions(Start,1)=1;
      dimensions(End  ,1)=nx(grid,1);
    }
    else
    {
      dimensions(Start,1)=1-extra;
      dimensions(End  ,1)=nx(grid,1)+extra;
    }
    if( nx(grid,2)==1 )
    {
      extra=0;
      domainDimension--;
    }
    dimensions(Start,2)=1-extra;
    dimensions(End  ,2)=nx(grid,2)+extra;
    int rangeDimension=numberOfDimensionsInFile; // =3 ** 990701 **
    // if( nx(grid,2)==1 )  // *wdh* 010413
    //   rangeDimension=2;
    
    realArray xyz;
    xyz.redim(Range(dimensions(Start,0),dimensions(End,0)),
	      Range(dimensions(Start,1),dimensions(End,1)),
	      Range(dimensions(Start,2),dimensions(End,2)), rangeDimension );
    xyz=REAL_MAX;
    int gridToRead=gridToUse>=0 ? gridStart+1 : 1;   
    int grid0 = gridToUse>=0 ? 0 : grid;
    int ierr;

    int readIblank=0;
    intArray nullMask;
    intArray *mask=&nullMask;
    
    if( iblankIsThere && maskPointer!=0 )
    {
      readIblank=1;
      mask=&maskPointer[grid0];
      
      maskPointer[grid0].redim(Range(dimensions(Start,0),dimensions(End,0)),
			       Range(dimensions(Start,1),dimensions(End,1)),
			       Range(dimensions(Start,2),dimensions(End,2)));
      maskPointer[grid0]=0;
    }
    

    Range I1(1,nx(grid,0)), I2(1,nx(grid,1)), I3(1,nx(grid,2)), Rx(0,rangeDimension-1);

    if( filePrecision==defaultPrecision )
    {
      // read as the default precision

      RDPLT3D(fileFormat,iunit, gridToRead, nx(grid0,0), nx(grid0,1), nx(grid0,2),
	      rangeDimension, 
	      dimensions(Start,0),dimensions(End,0), 
	      dimensions(Start,1),dimensions(End,1), 
	      dimensions(Start,2),dimensions(End,2), 
	      *xyz.getDataPointer(), readIblank, *mask->getDataPointer(), ierr);

      real xMin[3]={REAL_MAX,REAL_MAX,REAL_MAX}, xMax[3]={REAL_MAX,REAL_MAX,REAL_MAX};
      
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	xMax[axis]=max(xyz(I1,I2,I3,axis));
	xMin[axis]=min(xyz(I1,I2,I3,axis));
        printf(" axis=%i : xMin=%e  xMax=%e \n",axis,xMin[axis],xMax[axis]);
      }
    }
    if( filePrecision==singlePrecision || (ierr!=0 && FLT_EPSILON > REAL_EPSILON) )
    {
      // we are in double precision -- try reading the file as single precision
      if( grid==gridStart )
      {
        printf("*** try reading the file in single precision ***\n");
        CLOSEPLT3D(iunit);      
        fileFormat=0;
        OPPLT3D( (char *)((const char*)fileName),iunit,fileFormat,ngd, ng,nx(0,0),nx(0,1),nx(0,2),nq,nqc,strlen(fileName) );
	if( fileFormat==0 )
	  printf("input file is single grid, formatted nx=%i ny=%i nz=%i \n",nx(0,0),nx(0,1),nx(0,2));
	else if( fileFormat==1 )
	  printf("input file is single grid, unformatted  nx=%i ny=%i nz=%i \n",nx(0,0),nx(0,1),nx(0,2));
	else if( fileFormat==2 )
	  printf("input file is multiple grid, formatted \n");
	else if( fileFormat==3 )
	  printf("input file is multiple grid, unformatted \n");
	else if( fileFormat==4 )
	  printf("input file is single grid, 2D, formatted  nx=%i ny=%i \n",nx(0,0),nx(0,1));
	else if( fileFormat==5 )
	  printf("input file is single grid, 2D, unformatted nx=%i ny=%i \n",nx(0,0),nx(0,1));
	else
	{
	  printf("ERROR: unknown file format! \n");
	  return 1;
	}
      }
      floatArray xyzs;
      xyzs.redim(Range(dimensions(Start,0),dimensions(End,0)),
		Range(dimensions(Start,1),dimensions(End,1)),
		Range(dimensions(Start,2),dimensions(End,2)), rangeDimension );
      xyzs=0.;
      int ierr;
      RDPLT3DS(fileFormat,iunit, gridToRead, nx(grid0,0), nx(grid0,1), nx(grid0,2),
	       rangeDimension, 
	       dimensions(Start,0),dimensions(End,0), 
	       dimensions(Start,1),dimensions(End,1), 
	       dimensions(Start,2),dimensions(End,2), 
	       *xyzs.getDataPointer(), readIblank, *mask->getDataPointer(), ierr);

      for( int axis=0; axis<rangeDimension; axis++ )
      {
	real xMax=max(xyzs(I1,I2,I3,axis));
	real xMin=min(xyzs(I1,I2,I3,axis));
        printf(" axis=%i : xMin=%e  xMax=%e \n",axis,xMin,xMax);
      }
      


      if( ierr==0 )
      {
        if( grid==0 ) 
          printf("Success! The plot3d file is single precision\n");
        filePrecision=singlePrecision;
        for( int i4=xyz.getBase(3); i4<=xyz.getBound(3); i4++ )
        for( int i3=xyz.getBase(2); i3<=xyz.getBound(2); i3++ )
        for( int i2=xyz.getBase(1); i2<=xyz.getBound(1); i2++ )
        for( int i1=xyz.getBase(0); i1<=xyz.getBound(0); i1++ )
          xyz(i1,i2,i3,i4)=xyzs(i1,i2,i3,i4);
      }
      else
      {
	printf("Error reading in single precision\n");
	break;
      }
    }
    else if( filePrecision==doublePrecision || (ierr!=0 && DBL_EPSILON < REAL_EPSILON) )
    {
      // we are in single precision -- try reading the file as double precision
      if( grid==0 )
      {
        printf("*** try reading the file in double precision ***\n");
        CLOSEPLT3D(iunit);      
        OPPLT3D( (char *)((const char*)fileName),iunit,fileFormat,ngd, ng,nx(0,0),nx(0,1),nx(0,2),nq,nqc,strlen(fileName) );
      }
      doubleArray xyzs;
      xyzs.redim(Range(dimensions(Start,0),dimensions(End,0)),
		 Range(dimensions(Start,1),dimensions(End,1)),
		 Range(dimensions(Start,2),dimensions(End,2)), rangeDimension );
      xyzs=0.;
      int grid0=1;
      int ierr;
      RDPLT3DD(fileFormat,iunit, grid0, nx(grid,0), nx(grid,1), nx(grid,2),
	       rangeDimension, 
	       dimensions(Start,0),dimensions(End,0), 
	       dimensions(Start,1),dimensions(End,1), 
	       dimensions(Start,2),dimensions(End,2), 
	       *xyzs.getDataPointer(), readIblank, *mask->getDataPointer(), ierr);

      if( ierr==0 )
      {
        if( grid==0 ) 
          printf("Success! The plot3d file is double precision\n");
        filePrecision=doublePrecision;
        for( int i4=xyz.getBase(3); i4<=xyz.getBound(3); i4++ )
        for( int i3=xyz.getBase(2); i3<=xyz.getBound(2); i3++ )
        for( int i2=xyz.getBase(1); i2<=xyz.getBound(1); i2++ )
        for( int i1=xyz.getBase(0); i1<=xyz.getBound(0); i1++ )
          xyz(i1,i2,i3,i4)=xyzs(i1,i2,i3,i4);
      }
      else
	break;
    }

//        xyz.display("Here are the grid points xyz after RDPLT3D");
    // *** check if this is really a 2d grid ***
    if( domainDimension<rangeDimension )
    {
      // determine whether the range dimension is smaller
      Index I1(1,nx(grid,0)), I2(1,nx(grid,1)), I3(1,nx(grid,2));
          
      if( max(fabs(xyz(I1,I2,I3,1)))== 0. )
      {  // y values are zero, set y=z.
	rangeDimension=2;
	xyz(I1,I2,I3,1)=xyz(I1,I2,I3,2);
	// xyz.display("Here are the grid points xyz after y=z");
      }
      else if(  max(fabs(xyz(I1,I2,I3,2)))== 0. )
      { // discard z
	rangeDimension=2;
      }
      if( rangeDimension==2 )
      { // we need to resize the arrays
	if( nx(grid,1)==1 )
	{
	  xyz.reshape(Range(dimensions(Start,0),dimensions(End,0)),
		      Range(dimensions(Start,2),dimensions(End,2)),
		      Range(dimensions(Start,1),dimensions(End,1)), 3 );

	  xyz.resize(Range(dimensions(Start,0),dimensions(End,0)),
		     Range(dimensions(Start,2),dimensions(End,2)),   // **** note 2's
		     Range(dimensions(Start,1),dimensions(End,1)), rangeDimension );
	}
	else if( nx(grid,2)==1 )
	{
	  xyz.reshape(Range(dimensions(Start,0),dimensions(End,0)),
		      Range(dimensions(Start,1),dimensions(End,1)),
		      Range(dimensions(Start,2),dimensions(End,2)), 3 );

	  xyz.resize(Range(dimensions(Start,0),dimensions(End,0)),
		     Range(dimensions(Start,1),dimensions(End,1)),
		     Range(dimensions(Start,2),dimensions(End,2)), rangeDimension );
	}
	else if( nx(grid,0)==1 )
	{
	  xyz.reshape(Range(dimensions(Start,1),dimensions(End,1)),
		      Range(dimensions(Start,2),dimensions(End,2)),
		      Range(dimensions(Start,0),dimensions(End,0)), 3 );
	  xyz.resize(Range(dimensions(Start,1),dimensions(End,1)),
		     Range(dimensions(Start,2),dimensions(End,2)),
		     Range(dimensions(Start,0),dimensions(End,0)), rangeDimension );
	}
      }
          
    }
    else if( domainDimension==3 )
    {
      // *** more checks for 2d grids ***
      if( nx(grid,0)==2 )
      { // only 2 points in X, --> should be a 2d grid
	Index I1(1,nx(grid,0)), I2(1,nx(grid,1)), I3(1,nx(grid,2));
	realArray xyz2(1,
		       Range(dimensions(Start,1),dimensions(End,1)),
		       Range(dimensions(Start,2),dimensions(End,2)), 2 );
	xyz2(0,I2,I3,0)=xyz(1,I2,I3,0);
	xyz2(0,I2,I3,1)=xyz(1,I2,I3,2);  // throw away y (yes!)
	xyz2.reshape(Range(dimensions(Start,1),dimensions(End,1)),
		     Range(dimensions(Start,2),dimensions(End,2)), 
		     1,2 );
	xyz.redim(0);
	xyz.reference(xyz2);
	domainDimension=2;
	rangeDimension=2;
      }
      else if( nx(grid,2)==3 )  // overflow can output 3 points in the z-direction for 2D
      { // only 3 points in z, --> should be a 2d grid
        printf("readPlot3d: 3 points in y-direction -- converting to a 2d grid\n");
	
	Range I1(1,nx(grid,0)), I2(1,nx(grid,1)), I3(1,nx(grid,2));
	realArray xyz2(I1,I2,1,2);

	xyz2(I1,I2,0,0)=xyz(I1,I2,2,0);
	xyz2(I1,I2,0,1)=xyz(I1,I2,2,2);  // ! throw away y 

	xyz.redim(0);
	xyz.reference(xyz2);

	if( iblankIsThere && maskPointer!=0 )
	{
          intArray & mask = maskPointer[grid0];
          intArray mask2(I1,I2);
          mask2(I1,I2)=mask(I1,I2,2);
	  
          mask.reference(mask2);
	}
	

	domainDimension=2;
	rangeDimension=2;
      }
    }

    if( domainDimension<3 )
    {
      xyz.setBase(0,2);
      if( iblankIsThere && maskPointer!=0 )
      {
        intArray & mask = maskPointer[grid0];
	mask.setBase(0,2);
      }
    }
    if( domainDimension<2 )
      xyz.setBase(0,1);
	
    // xyz.display("Here are the grid points xyz after resize");


    printf("readPlot3d: domain dimension =%i, range dimension=%i \n",domainDimension,rangeDimension);

    DataPointMapping *dpm = dpmPointer==0 ? new DataPointMapping() : dpmPointer;
    dpm->setDataPoints(xyz,3,domainDimension);

    // strip off any directory path stuff from the name. (look for a '/' working from the end).
    aString name=fileName;
    int length=name.length()-1;
    int i=length;
    while( i>=0 && name[i]!='/' )
      i--;
    i++;
    sPrintF(buff,"%s-grid%i",(const char *)name(i,length),grid);
    printf("creating mapping %s\n",buff);
    dpm->setName(Mapping::mappingName,buff);
    if( dpmPointer==0 )
      mapInfo.mappingList.addElement(*dpm);

  }
  CLOSEPLT3D(iunit);
  
  return 0;  

}

int DataFormats::
readIngrid(UnstructuredMapping &map, const aString &gridFileName /* =  nullString */)

// =========================================================================================
/// \param Access level: protected.
/// \details 
///     Read in grids from an ingrid file. // 
/// \param map (input):
/// \param gridFileName(input) : optional name of the file
/// \return  0=success, 1=failure.
// =========================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface("",false);
  assert(giPointer!=NULL || gridFileName!=nullString);
  GenericGraphicsInterface & gi = *giPointer;
    

  FILE *fp=NULL;
  aString fileName;

  if( gridFileName!="" && gridFileName!=" " )
    fileName=gridFileName;
  else
    gi.inputString(fileName,"Enter the name of the ingrid file");

  fp=fopen((const char*)fileName,"r");

  int attempts=0;
  while( fp==NULL && attempts<5 )
  {
    gi.inputString(fileName,"File not found, enter another name for the ingrid file");
    fp=fopen((const char*)fileName,"r");
    attempts++;
  }
  if( attempts>=5 )
  {
    gi.outputString("Too many tries");
    return 1;
  }
  int numberOfBytes=0;
  while( fgetc(fp)!=EOF )
    numberOfBytes++;
  printf("Number of bytes in the file =%i\n",numberOfBytes);

  fclose(fp);

  int iunit=23;
  int rdim;
  int ddim;
  int nnode, nelem;
  int nemax;
  char mtype[1025];

  OPINGRID((char *)((const char *)fileName), mtype, iunit, rdim, ddim, nnode, nelem, nemax, strlen(fileName), 1025);

  intArray elems;
  elems.redim(nelem, nemax);
  intArray tags;
  tags.redim(nelem);

  realArray xyz;
  xyz.redim(nnode, rdim);
  elems = 0;
  xyz = 0.0;

  RDINGRID(iunit, mtype, rdim, ddim, nnode, nelem, nemax, *xyz.getDataPointer(), *elems.getDataPointer(), *tags.getDataPointer(), 1025);

  // adjust from fortran indexing
  elems-=1;
  tags -=1;

  map.setNodesAndConnectivity(xyz, elems, ddim,false);
  map.setTags(tags);

  CLINGRID(iunit);

  return 0;
}

int DataFormats::
readTecplot(ListOfMappingRC &mList, const aString &gridFileName /* =  nullString */)

// =========================================================================================
/// \param Access level: protected.
/// \details 
///     Read in grids from an ingrid file. // 
/// \param mList (input):
/// \param gridFileName(input) : optional name of the file
/// \return  0=success, 1=failure.
// =========================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert(giPointer!=NULL || gridFileName!=nullString);
  GenericGraphicsInterface & gi = *giPointer;
    
  FILE *fp=NULL;
  aString fileName;

  if( gridFileName!="" && gridFileName!=" " )
    fileName=gridFileName;
  else
    gi.inputFileName(fileName, "Enter the name of the tecplot file", "dat");

  fp=fopen((const char*)fileName,"r");

  int attempts=0;
  while( fp==NULL && attempts<5 )
  {
    gi.inputFileName(fileName,"File not found, enter another name for the tecplot file", 
				    "dat");
    fp=fopen((const char*)fileName,"r");
    attempts++;
  }
  if( attempts>=5 )
  {
    gi.outputString("Too many tries");
    return 1;
  }

  int rdim=3;
  int ddim;
  int nnode, nelem, totalNodes=0, totalElems=0;
  int nemax;
  
  realArray xyz;
  intArray elems;

  char comment[1025], etype[30];
  char *subs;
  
  fgets(comment, 1025, fp);
//  printf("1st line: `%s'\n", comment);
  fgets(comment, 1025, fp);
//  printf("2nd line: `%s'\n", comment); // could figure out rdim from the number of fields...

// read all components...
  while (fgets(comment, 1025, fp))
  {
//    printf("Next line: `%s'", comment);
    subs = strchr(comment, '"'); subs++;
//    printf("subs: `%s'\n", subs);
    char name[50];
    int le=strcspn(subs,"\"");
    strncpy(name, subs, le);
    name[le]='\0';
    printf("name=`%s'\n", name);
    
    subs = strchr(subs, '='); subs++; // After second =
    printf("subs: `%s'\n", subs);
    sScanF(subs, "%i", &nnode);
    subs = strchr(subs, '='); subs++; // After third =
    printf("subs: `%s'\n", subs);
    sScanF(subs, "%i", &nelem);
    subs = strchr(subs, '='); subs++; // After fourth =
    printf("subs: `%s'\n", subs);
    sScanF(subs, "%s", etype);

    if (strncmp(etype,"FEPOINT",7))
    {
      printf("ERROR: Unable to parse tecplot file of type `%s'\n", etype);
      fclose(fp);
      return 1;
    }
  
// check if there is more info after FEPOINT
    char esub[20];
    
    if (strlen(etype) > 7)
    {
      if ((subs = strchr(subs, '=')))
      {
	subs++; // After fifth =
	sScanF(subs, "%s", esub);
	if (!strcmp(esub,"BRICK"))
	{
	  ddim=3;
	  nemax=8;
	  printf("Assuming hex elements\n");
	}
	else
	{
	  printf("Unknown element type: %s\n", esub);
	  return 1;
	}
      }
    }
    else
    {
      ddim=2;
      nemax=4; //assume quads for now
    }
    
      
    printf("nnode: %i, nelem: %i, etype: %s\n", nnode, nelem, etype);

// make a new unstructured mapping and insert it into the list
    UnstructuredMapping * uMap_ = new UnstructuredMapping;
    uMap_->incrementReferenceCount();
    MappingRC uMapRC(*uMap_);
    mList.addElement(uMapRC);
    
    xyz.resize(nnode, rdim);

// read all nodes
    int i;
    for (i=0; i<nnode; i++)
    {
      fgets(comment, 1025, fp);
      sScanF(comment,"%e %e %e", &xyz(i, 0), &xyz(i, 1), &xyz(i, 2));
    }
    
// tmp
//    printf("Last line before reading elements:`%s'", comment);

    elems.resize(nelem, nemax);

// read all elements
    if (nemax == 4)
    {
      for (i=0; i<nelem; i++)
      {
	fgets(comment, 1025, fp);
	sScanF(comment,"%i %i %i %i", &elems(i,0), &elems(i,1), &elems(i,2), &elems(i,3));
      }
    }
    else if (nemax==8)
    {
      for (i=0; i<nelem; i++)
      {
	fgets(comment, 1025, fp);
	sScanF(comment,"%i %i %i %i %i %i %i %i", &elems(i,0), &elems(i,1), &elems(i,2), &elems(i,3), 
	       &elems(i,4), &elems(i,5), &elems(i,6), &elems(i,7));
      }
    }
    
    
// tmp
//    printf("Last line after reading elements:`%s'", comment);

// adjust for fortran numbering of nodes (starting at 1 rather than 0)
    elems -= 1;

// still need to buil connectivity, since the plotter uses the face and edge arrays?
    printf("Building conectivity...");
    uMap_->setNodesAndConnectivity(xyz, elems, ddim, false); 
    printf("Done\n");

    intArray tags;
    tags.redim(nelem);

    uMap_->setTags(tags);

// assign name after setNodesAndConnectivity!
    uMap_->setName(Mapping::mappingName, name);

    if (uMap_->decrementReferenceCount() == 0)
      delete uMap_;


  }
  
  fclose(fp);

  return 0;
}

int DataFormats::
readCart3dTri(ListOfMappingRC &mList, const aString &gridFileName /* =  nullString */)

// =========================================================================================
/// \param Access level: protected.
/// \details 
///     Read in grids from an ingrid file. // 
/// \param mList (input):
/// \param gridFileName(input) : optional name of the file
/// \return  0=success, 1=failure.
// =========================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert(giPointer!=NULL || gridFileName!=nullString);
  GenericGraphicsInterface & gi = *giPointer;
    
  FILE *fp=NULL;
  aString fileName;

  if( gridFileName!="" && gridFileName!=" " )
    fileName=gridFileName;
  else
    gi.inputFileName(fileName, "Enter the name of the Cart3d tri-file", "tri");

  fp=fopen((const char*)fileName,"r");

  int attempts=0;
  while( fp==NULL && attempts<5 )
  {
    gi.inputFileName(fileName,"File not found, enter another name for the Cart3d tri-file", 
				    "tri");
    fp=fopen((const char*)fileName,"r");
    attempts++;
  }
  if( attempts>=5 )
  {
    gi.outputString("Too many tries");
    return 1;
  }

  int rdim=3;
  int ddim=2;
  int nnode, nelem;
  int nemax=4; //we just do tris for now
  realArray xyz;
  intArray elems;
  
// read all components...
  char comment[1025];

  while (fgets(comment, 1025, fp))
  {
    sScanF(comment, "%i %i", &nnode, &nelem);
    printf("nnode: %i, nelem: %i\n", nnode, nelem);

// make a new unstructured mapping and insert it into the list
    UnstructuredMapping * uMap_ = new UnstructuredMapping;
    uMap_->incrementReferenceCount();
    MappingRC uMapRC(*uMap_);
    mList.addElement(uMapRC);
    
    xyz.resize(nnode, rdim);

// read all nodes
    int i;
    for (i=0; i<nnode; i++)
    {
      fgets(comment, 1025, fp);
      sScanF(comment,"%e %e %e", &xyz(i, 0), &xyz(i, 1), &xyz(i, 2));
    }
    
// tmp
//    printf("Last line before reading elements:`%s'", comment);

    elems.resize(nelem, nemax);
    elems = -1;

// read all elements
    for (i=0; i<nelem; i++)
    {
      fgets(comment, 1025, fp);
      sScanF(comment,"%i %i %i %i", &elems(i,0), &elems(i,1), &elems(i,2), &elems(i,3));
    }
    
// adjust for fortran numbering of nodes (starting at 1 rather than 0)
//    elems -= 1;
    // kkc some do dome don't start with one, check first and adjust if needed
    Range R(nelem),E(3);
    if ( min(elems(R,E))==1 )
      elems(R,E) -= 1;

// read all tags
    intArray tags;
    tags.redim(nelem);

    for (i=0; i<nelem; i++)
    {
      fgets(comment, 1025, fp);
      sScanF(comment,"%i", &tags(i));
    }

// still need to build connectivity, since the plotter uses the face and edge arrays?
    printf("Building conectivity...");
    uMap_->setNodesAndConnectivity(xyz, elems, ddim);//kkc for tgrid ??, false); 
    printf("Done\n");

    uMap_->setTags(tags);

    if (uMap_->decrementReferenceCount() == 0)
      delete uMap_;


  }
  
  fclose(fp);
  return 0;
}

int DataFormats::
readPly(UnstructuredMapping &map, const aString &gridFileName /* =  nullString */)

// =========================================================================================
/// \param Access level: protected.
/// \details 
///     Read in unstructured surface from a PLY Polygonal file. // 
/// \param map (input):
/// \param gridFileName(input) : optional name of the file
/// \return  0=success, 1=failure.
// =========================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert(giPointer!=NULL || gridFileName!=nullString);
  GenericGraphicsInterface & gi = *giPointer;
    
  FILE *fp=NULL;
  aString fileName;

  if( gridFileName!="" && gridFileName!=" " )
    fileName=gridFileName;
  else
    gi.inputString(fileName,"Enter the name of the PLY file");

  fp=fopen((const char*)fileName,"r");

  int attempts=0;
  while( fp==NULL && attempts<5 )
  {
    gi.inputString(fileName,"File not found, enter another name for the PLY file");
    fp=fopen((const char*)fileName,"r");
    attempts++;
  }
  if( attempts>=5 )
  {
    gi.outputString("Too many tries");
    return 1;
  }
  int numberOfBytes=0;
  while( fgetc(fp)!=EOF )
    numberOfBytes++;
  printf("Number of bytes in the file =%i\n",numberOfBytes);
  fclose(fp);

  int rdim;
  int ddim;
  int nnode, nelem;
  int nemax;
  //char mtype[1025];

  intArray elems;
  intArray tags;
  realArray xyz;
  elems = 0;
  xyz = 0.0;

  PlyFileInterface ply;
  ply.openFile( fileName );
  ply.readFile( elems, tags, xyz, nnode, nelem, ddim, rdim);
  
  map.setNodesAndConnectivity(xyz, elems, ddim);
  map.setTags(tags);

  ply.closeFile();

  return 0;
}


int DataFormats::
writePlot3d(Mapping & map,
	    const aString & gridFileName /* =nullString */ )
// ==============================================================================
/// \details 
// =====================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert( giPointer!=NULL );

  GenericGraphicsInterface & gi = *giPointer;
  
  aString fileName;
  if( gridFileName!="" && gridFileName!=" " )
    fileName=gridFileName;
  else
    gi.inputString(fileName,"Enter the name of the plot3d file");

  aString menu[]=
  {
    "exit",
    "save file",
    "save file for overflow",
    ">format",
      "formatted",
      "unformatted",
    "<>precision",
      "single precision",
      "double precision",
    "<>iblank",
      "include iblank",
      "do not include iblank",
    "<",
    ""
  };
  
  aString answer;
  enum PrecisionEnum
  {
    singlePrecision,
    doublePrecision
  } precision;
  precision = sizeof(real)==sizeof(double) ? doublePrecision : singlePrecision;
  
   
  enum FormatEnum
  {
    formatted=0,
    unformatted
  } formatType=formatted;
  
  int writeIblank=0;

  printf("The default file format is an unformatted file in %s precision with no iblank array\n",
	 (precision==singlePrecision ? "single" : "double"));

  gi.appendToTheDefaultPrompt("writePlot3d>"); // set the default prompt
  for(;;)
  {
    gi.getMenuItem(menu,answer,"choose an option");
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="formatted" )
    {
      formatType=formatted;
    }
    else if(  answer=="unformatted" )
    {
      formatType=unformatted;
    }
    else if( answer=="single precision" )
    {
      precision=singlePrecision;
    }
    else if( answer=="double precision" )
    {
      precision=doublePrecision;
    }
    else if( answer=="include iblank" )
    {
      writeIblank=1;
    }
    else if( answer=="do not include iblank" )
    {
      writeIblank=0;
    }
    else if( answer=="save file" || answer=="save file for overflow" )
    {
      
      
      const int iunit=24;
      int ng=1, grid=1, ierr=0;
      IntegerArray nx(3,ng),iblank;
      nx=1;
      
      RealArray *xyPointer = (RealArray*)(&map.getGrid());
      RealArray xyz;
      
      int rangeDimension = map.getRangeDimension();
      for( int axis=0; axis<map.getDomainDimension(); axis++ )
      {
        nx(axis,0)=map.getGridDimensions(axis);
      }
      
      const int saveForOverflow = answer=="save file for overflow";
      if( saveForOverflow )
      {
	formatType=unformatted;
        // for overflow we save a 2D file as 3D with 3 lines, make y direction constant
        if( rangeDimension==2 )
	{
          printf("INFO: save a 2d grid as a 3D grid for overflow. 3 points in i3, y=constant\n");
	  
          RealArray & xy = *xyPointer;
	  rangeDimension=3;
	  Range I1=xy.dimension(0), I2=xy.dimension(1);
	  xyz.redim(I1,I2,3,3);

          real dy=1.;
	  nx(2,0)=3;
	  for( int i3=0; i3<3; i3++ )
	  {
	    xyz(I1,I2,i3,0)=xy(I1,I2,0,0);
	    xyz(I1,I2,i3,1)=dy*(1-i3);
	    xyz(I1,I2,i3,2)=xy(I1,I2,0,1);
	  }
          xyPointer=&xyz;
	}
      }

      if( !writeIblank )
      {
        printf("Saving the file in `plot3d' format (fortran %s file, %s precision):\n"
               "nx, ny, ny \n"
               "x(0) x(1) .... x(ny-1) \n"
               "y(0) y(1) .... y(ny-1) \n",
	       (formatType==formatted ? "formatted" : "unformatted"),
               (precision==singlePrecision ? "single" : "double"));
	if( rangeDimension==3 )
	  printf("z(0) z(1) .... z(nz-1)\n");
      }
      else
      {
        printf("Saving the file in `plot3d' format with iblank (fortran %s file):\n"
               "nx, ny, ny \n"
               "x(0) x(1) .... x(ny-1) \n"
               "y(0) y(1) .... y(ny-1) \n",
	       (formatType==formatted ? "formatted" : "unformatted"));
	if( rangeDimension==3 )
	  printf("z(0) z(1) .... z(nz-1)\n");
        printf("iblank(0) iblank(1) .... iblank(nz-1) \n");
      }
      
      RealArray & xy = *xyPointer;

      if( precision==singlePrecision )
      {
        floatSerialArray xs;
        floatSerialArray *xp;
        if( sizeof(real)!=sizeof(float) )
	{ // make a single precision copy
          xp=&xs;
	  xs.redim(xy.dimension(0),xy.dimension(1),xy.dimension(2),xy.dimension(3));
	  equals(xs,xy);
	}
        else
          xp=(floatSerialArray*)(&xy);

	floatSerialArray & x = *xp;
//  	WRPLT3DS((char *)((const char*)fileName), (int)formatType,iunit, ng,grid,nx(0,0),nx(1,0),nx(2,0),
//  		 rangeDimension,
//                   x.getBase(0)+1,x.getBound(0)+1,  // shift so 1=boundary instead of 0
//                   x.getBase(1)+1,x.getBound(1)+1,
//                   x.getBase(2)+1,x.getBound(2)+1,
//                   *x.getDataPointer(),writeIblank,*iblank.getDataPointer(),ierr,strlen(fileName) );
        // we need to do this for views (such as returned by a DPM mapping)
	WRPLT3DS((char *)((const char*)fileName), (int)formatType,iunit, ng,grid,nx(0,0),nx(1,0),nx(2,0),
		 rangeDimension,
                 1,x.getRawDataSize(0),  // shift so 1=boundary instead of 0
                 1,x.getRawDataSize(1), 
                 1,x.getRawDataSize(2),
                 *getDataPointer(x),writeIblank,*iblank.getDataPointer(),ierr,strlen(fileName) );
        // we need to do this for views (such as returned by a DPM mapping)

      }
      else
      {
        doubleSerialArray xd;
        doubleSerialArray *xp;
        if( sizeof(real)!=sizeof(double) )
	{ // make a double precision copy
          xp=&xd;
	  xd.redim(xy.dimension(0),xy.dimension(1),xy.dimension(2),xy.dimension(3));
	  equals(xd,xy);
	}
        else
          xp=(doubleSerialArray*)(&xy);
	doubleSerialArray & x = *xp;

//          printf(" xBase(0)=%i xBound(0)=%i \n",x.getBase(0),x.getBound(0));
//          printf(" xRawBase(0)=%i xRawBound(0)=%i \n",x.getRawBase(0),x.getRawBound(0));
//          printf(" x.getRawDataSize(0)=%i \n",x.getRawDataSize(0));
//          x.display("x");
	
	
//  	WRPLT3DD((char *)((const char*)fileName), (int)formatType,iunit, ng,grid,nx(0,0),nx(1,0),nx(2,0),
//  		 rangeDimension,
//                   x.getBase(0)+1,x.getBound(0)+1,  // shift so 1=boundary instead of 0
//                   x.getBase(1)+1,x.getBound(1)+1,
//                   x.getBase(2)+1,x.getBound(2)+1,
//                   *x.getDataPointer(),writeIblank,*iblank.getDataPointer(),ierr,strlen(fileName) );
        // we need to do this for views (such as returned by a DPM mapping)
	WRPLT3DD((char *)((const char*)fileName), (int)formatType,iunit, ng,grid,nx(0,0),nx(1,0),nx(2,0),
		 rangeDimension,
                 1,x.getRawDataSize(0),  // shift so 1=boundary instead of 0
                 1,x.getRawDataSize(1), 
                 1,x.getRawDataSize(2),
                 *getDataPointer(x),writeIblank,*iblank.getDataPointer(),ierr,strlen(fileName) );
      }
      CLOSEPLT3D(iunit);
    }
    else
    {
      cout << "Unknown response=" << answer << endl;
      gi.stopReadingCommandFile();
    }
  }
  gi.unAppendTheDefaultPrompt();  // reset

  return 0;
}


int DataFormats::
writeIngrid(Mapping & map,
	    const aString & gridFileName /* =nullString */ )
// ==============================================================================
/// \details  write a mapping to an ascii file in an unstructured format
// =====================================================================================
{  
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert(giPointer!=NULL || gridFileName!=nullString);
  GenericGraphicsInterface & gi = *giPointer;

  aString fileName=nullString;

  if( gridFileName!="" && gridFileName!=" " )
    fileName=gridFileName;
  else
    gi.inputString(fileName,"Enter the name of the ingrid file");

  UnstructuredMapping *umap;

  if (map.getClassName() !="UnstructuredMapping") {
    umap = new UnstructuredMapping;
    // this is a costly way to write an ingrid mesh from a structured Mapping
    // it will probably change when GridCollections are written to Ingrid
    umap->buildFromAMapping(map);
  } else {
    umap = (UnstructuredMapping *)&map;
  }

  int iunit = 24;

  const realArray & nodes = umap->getNodes();
  const intArray & elems = umap->getElements();
  const intArray & tags = umap->getTags();

  int nnode = umap->getNumberOfNodes();
  int nelem = umap->getNumberOfElements();
  int rdim  = umap->getRangeDimension();
  int ddim  = umap->getDomainDimension();
  int nemax = umap->getMaxNumberOfNodesPerElement();
  cout<<fileName<<endl;
  WRINGRID((char *)((const char*)fileName), iunit, rdim, ddim, nnode, nelem, nemax, *nodes.getDataPointer(), *elems.getDataPointer(), *tags.getDataPointer(), strlen(fileName));

  if (map.getClassName() !="UnstructuredMapping") {
    delete umap;
  }

  return 0;
}



int DataFormats::
readSTL(UnstructuredMapping &map, 
	const aString & stlFileName /* =nullString */ )
// ==============================================================================
/// \details 
///    Read a surface triangulation from STL file into an unstructured Mapping.
/// \param map (input/output) : build this unstructured mapping
/// \param stlFileName (input) : optionally supply the name of the stl file. If not supplied,
///     prompt for a name.
/// \param Return values : 0=success.
// =====================================================================================
{
  GenericGraphicsInterface *giPointer = Overture::getGraphicsInterface();
  assert(giPointer!=NULL || stlFileName!=nullString);
  GenericGraphicsInterface & gi = *giPointer;

  aString fileName;
  if( stlFileName!="" && stlFileName!=" " )
    fileName=stlFileName;
  else
    gi.inputString(fileName,"Enter the name of the stl file");


  FILE *fp=NULL;

  fp=fopen((const char*)fileName,"r");
  if( fp==NULL )
  {
    printF("DataFormats::readSTL:ERROR: unable to open the STL file =[%s]\n",(const char*)fileName);
    return 1;
  }
  
  // Here is the STL ascii file format 
  // solid name
  // facet normal n1 n2 n3
  //   outer loop
  //     vertex v11 v12 v13
  //     vertex v21 v22 v23
  //     vertex v31 v32 v33
  //  endloop
  // endfacet
  // facet normal n1 n2 n3
  //  ...
  // 
  // endsolid name
    
  int numberOfNodes=0;
  int numberOfElements=0;
  const int maxNumberOfNodesPerElement=3;

  const int domainDimension=2, rangeDimension=3;

  intArray elements;
  realArray xyz;

  const int buffSize=100;
  char buff[buffSize], name[100];
  getLineFromFile( fp,buff,buffSize );

  printF("DataFormats::readSTL:header = [%s]\n",buff);

  int maxElements=10000; // guess number of elements. This is increased below as needed
  intArray tri(maxElements,maxNumberOfNodesPerElement);

  int maxNodes=maxElements*3;  // guess number of nodes
  realArray nodes(maxNodes,rangeDimension);

  real x,y,z;
  bool done=false;
  while( !done )
  {
    // look for facet normal
    int numChars= getLineFromFile( fp,buff,buffSize );
    // printF(" line=[%s] (facet normal?)\n",buff);
    if( strstr(buff,"endsolid") )
    {
      done=true;
      break;
    }
    
    // look for outer loop
    numChars= getLineFromFile( fp,buff,buffSize );
    // printF(" line=[%s] (outer loop?)\n",buff);

    
    // look for 3 vertices
    for( int i=0; i<3; i++ )
    {
      numChars= getLineFromFile( fp,buff,buffSize );
      // printF(" line=[%s] \n",buff);

      sScanF(buff,"%s %e %e %e",name,&x,&y,&z);
      // printF(" triangle %i : node %i : (%g,%g,%g) \n",numberOfElements,i,x,y,z);
    
      // check if (x,y,z) is already there --- this is SLOW: fix me ---
      bool found=false;
      for( int n=0; n<numberOfNodes; n++ )
      {
	if( x==nodes(n,0) && y==nodes(n,1) && z==nodes(n,2) )
	{
          tri(numberOfElements,i)=n;
	  found=true;
          break;
	}
      }
      if( !found )
      {
        // we have found a new node
	nodes(numberOfNodes,0)=x;
	nodes(numberOfNodes,1)=y;
	nodes(numberOfNodes,2)=z;
	tri(numberOfElements,i)=numberOfNodes;
        printF(" node %i = (%g,%g,%g)\n",numberOfNodes,x,y,z);
	numberOfNodes++;
      }
    }
    
    printF(" element %i : consists of nodes %i,%i,%i\n",numberOfElements,tri(numberOfElements,0),
	   tri(numberOfElements,1),tri(numberOfElements,2));
    
      
    numberOfElements++;
    if( numberOfElements >= maxElements )
    {
      maxElements *=2;
      printF(" readSTL: increase maxElements to %i\n",maxElements);
      tri.resize(maxElements,rangeDimension);
    }
    if( numberOfNodes >= maxNodes-3 )
    {
      maxNodes *=2;
      printF(" readSTL: increase maxNodes to %i\n",maxNodes);
      tri.resize(maxNodes,maxNumberOfNodesPerElement);

    }
    
    // look for endloop
    numChars= getLineFromFile( fp,buff,buffSize );
    // printF(" line=[%s] (endloop?)\n",buff);

    // look for endfacet
    numChars= getLineFromFile( fp,buff,buffSize );
    // printF(" line=[%s] (endfacet?)\n",buff);

    if( numChars==0 )
    {
      break;
    }
//    int dum1,dum2,dum3;
//    sScanF(buff,"%i %i %i %i %i",&numberOfNodes,&numberOfElements,&dum1,&dum2,&dum3);
     
  }
  printF("number of triangles in the stl file =%i\n",numberOfElements);

  if( !done )
  {
    printF("DataFormats::readSTL:ERROR reading the stl file. The file did not end with 'endSolid' ?!\n");
  }

  //OV_ABORT("finish me");

  
  xyz.redim(numberOfNodes,rangeDimension);
  elements.redim(numberOfElements,maxNumberOfNodesPerElement);

  Range E=numberOfElements, Rx=3;
  elements=tri(E,Rx);

  xyz=nodes(Range(numberOfNodes),Range(rangeDimension));

  map.setNodesAndConnectivity(xyz, elements, domainDimension);


  fclose(fp);

  return 0;
}
