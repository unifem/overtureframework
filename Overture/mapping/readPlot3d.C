#include "ReadPlot3d.h"
#include "conversion.h"

#include "MappingInformation.h"
#include "DataPointMapping.h"
#include "OGgetIndex.h"
#include <string.h>

#define OPPLT3D  EXTERN_C_NAME(opplt3d)
#define RDPLT3D  EXTERN_C_NAME(rdplt3d)
#define RDPLT3DS  EXTERN_C_NAME(rdplt3ds)
#define RDPLT3DD  EXTERN_C_NAME(rdplt3dd)
#define RDPLT3DQS  EXTERN_C_NAME(rdplt3dqs)
#define RDPLT3DQD  EXTERN_C_NAME(rdplt3dqd)
#define CLOSEPLT3D EXTERN_C_NAME(closeplt3d)

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

  void CLOSEPLT3D(const int & iunit);
}

int
readPlot3d(MappingInformation & mapInfo, 
           const aString & gridFileName /* =nullString */,
           DataPointMapping *dpmPointer /* =NULL */ )
{
  return readPlot3d(  mapInfo,gridFileName,dpmPointer,NULL );
}

int
readPlot3d(MappingInformation & mapInfo, 
           const aString & gridFileName,
           intArray *maskPointer )
{
  return readPlot3d(  mapInfo,gridFileName,NULL,NULL,Overture::nullRealArray(),maskPointer );
}

int
readPlot3d(GenericGraphicsInterface & gi,
           RealArray & q, RealArray & par, 
           const aString & qFileName /* =nullString */)
// par (input/output) : must be dimensioned to receive values on output.
{
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  
  return readPlot3d(  mapInfo,qFileName,NULL,&q,par );
}


int
readPlot3d(MappingInformation & mapInfo, 
           const aString & plot3dFileName,
           DataPointMapping *dpmPointer,
           RealArray *qPointer ,
           RealArray & par  /* = Overture::nullRealArray() */,
           intArray *maskPointer /* =NULL */ )
// =========================================================================================
// /Description:
//    Read in grids from a plot3d file. If dpmPointer is supplied then try to create one
// grid; otherwise read in all grids, creating a dataPointMapping for each one and add these
// to the mapInfo.mappingList.
// 
// /mapInfo (input):
// /plot3dFileName (input) : optional name of the file
// /dpmPointer (input) : if not NULL, create this Mapping. 
// /Return value: 0=success, 1=failure.
// =========================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & graphicsInterface = *mapInfo.graphXInterface;
    

  FILE *fp=NULL;
  aString fileName;

  if( plot3dFileName!="" && plot3dFileName!=" " )
    fileName=plot3dFileName;
  else
    graphicsInterface.inputString(fileName,"Enter the name of the plot3d file");

  fp=fopen((const char*)fileName,"r");

  int attempts=0;
  while( fp==NULL && attempts<5 )
  {
    graphicsInterface.inputString(fileName,"File not found, enter another name for the plot3d file");
    fp=fopen((const char*)fileName,"r");
    attempts++;
  }
  if( attempts>=5 )
  {
    graphicsInterface.outputString("Too many tries");
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
  
  intArray nx(ngd,3);
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
      printf("This is apparently not a grid file but a solution file\n");
      if( unformatted && numberOfBytes < sizeof(double)*nq*nx(0,0)*nx(0,1)*nx(0,2) )
      {
	printf("The file is probably single precision since it is not long enough to be double precision\n");
	filePrecision = singlePrecision;
      }

      Range I1(1,nx(0,0)), I2(1,nx(0,1)), I3(1,nx(0,2)), N(0,nq-1);

      RealArray & q = *qPointer;
      q.redim(I1,I2,I3,N);

      real fsmach,alpha,re,time,gaminf;
      RealArray rgas(max(1,nqc));
      int ierr=0, gridToRead=1;
      if( filePrecision == singlePrecision )
      {
	floatArray q0(I1,I2,I3,N);
	float fsmach0,alpha0,re0,time0,gaminf0;
        floatArray rgas0(max(1,nqc));
	RDPLT3DQS(fileFormat,iunit, gridToRead, nx(0,0), nx(0,1), nx(0,2),
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
	RDPLT3DQD(fileFormat,iunit, gridToRead, nx(0,0), nx(0,1), nx(0,2),
		 nq,
		 q.getBase(0),q.getBound(0),
		 q.getBase(1),q.getBound(1),
		 q.getBase(2),q.getBound(2),
		 *q.getDataPointer(),nqc,fsmach,alpha,re,time,gaminf,*rgas.getDataPointer(),
		 ierr);
      }
      if( par.getLength(0)>0 )
      {
	par.redim(6);
	par=0.;
	par(0)=fsmach; par(1)=alpha; par(2)=re; par(3)=time; par(4)=gaminf;
      }
      if( nx(0,2)==3 ) // this is really a 2d computation.
      {
        Range N2(0,nq-2);
        RealArray q0(I1,I2,1,N2);

	q0(I1,I2,0,0)=q(I1,I2,2,0);
	q0(I1,I2,0,1)=q(I1,I2,2,1);
	q0(I1,I2,0,2)=q(I1,I2,2,3);  // throw away rho*v
        for( int n=3; n<nq-1; n++ )
          q0(I1,I2,0,n)=q(I1,I2,2,n+1);

	q.reference(q0);
      }
      

      return ierr;
    }
    else
    {
      printf("This is apparently not a grid file but a solution file\n");
      return 2;
    }
  }
  

  int numberOfDimensionsInFile=3;
  if( fileFormat==4 || fileFormat==5 )
    numberOfDimensionsInFile=2;
  
  
  char buff[80];
  IntegerArray dimensions(2,3);
  if( unformatted && numberOfBytes < sizeof(double)*ng*nx(0,0)*nx(0,1)*nx(0,2)*numberOfDimensionsInFile )
  {
    printf("The file is probably single precision since it is not long enough to be double precision\n");
    filePrecision = singlePrecision;
  }
  bool iblankIsThere=FALSE;
  if( unformatted )
  {
    int bytesPerNum = filePrecision == singlePrecision ? sizeof(float) : sizeof(double) ;
    if( numberOfBytes >= ng*nx(0,0)*nx(0,1)*nx(0,2)*(numberOfDimensionsInFile*bytesPerNum+sizeof(int)) )
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
	     
    graphicsInterface.inputString(answer,
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
  
  for( int grid=gridStart; grid<=gridEnd; grid++ )
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
    realArray xyz;
    xyz.redim(Range(dimensions(Start,0),dimensions(End,0)),
	      Range(dimensions(Start,1),dimensions(End,1)),
	      Range(dimensions(Start,2),dimensions(End,2)), rangeDimension );
    xyz=0.;
    int gridToRead=gridToUse>=0 ? gridStart+1 : 1;   
    int grid0 = gridToUse>=0 ? 0 : grid;
    int ierr;

    int readIblank=0;
    IntegerArray nullMask;
    IntegerArray *mask=&nullMask;
    
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


      for( int axis=0; axis<rangeDimension; axis++ )
      {
	real xMax=max(xyz(I1,I2,I3,axis));
	real xMin=min(xyz(I1,I2,I3,axis));
        printf(" axis=%i : xMin=%e  xMax=%e \n",axis,xMin,xMax);
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
          IntegerArray & mask = maskPointer[grid0];
          IntegerArray mask2(I1,I2);
          mask2(I1,I2)=mask(I1,I2,2);
	  
          mask.reference(mask2);
	}
	

	domainDimension=2;
	rangeDimension=2;
      }
    }

    if( domainDimension<3 )
      xyz.setBase(0,2);
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
