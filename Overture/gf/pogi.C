// =======================================================================================================
//   Test the parallel overlapping grid interpolator
//
// Examples:
//   mpirun -np 2 pogi -grid=sise2.order2.hdf -debug=0
//   mpirun -np 2 pogi -grid=sisi2.order2.hdf -debug=0
// 
//  srun -N1 -n1 -ppdebug pogi -grid=sise.hdf
// =======================================================================================================
#include "ParallelOverlappingGridInterpolator.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "display.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

int 
getLineFromFile( FILE *file, char s[], int lim);

real 
getMaxValue(real value, int processor /* = -1 */)
{
  real maxValue=value;
  #ifdef USE_PPP 
  if( processor==-1 )
    MPI_Allreduce(&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  else
    MPI_Reduce        (&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, processor, MPI_COMM_WORLD);
  #endif
  return maxValue;
}

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture


  ParallelOverlappingGridInterpolator::debug=3; // 3;
  
  // Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );  // doesn't work?

//  ios::sync_with_stdio();

//  int Number_Of_Processors = 0;
//  Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);
  // Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );

  // Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

  // Optimization_Manager::setForceVSG_Update(Off);
  
  int myid = max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::Number_Of_Processors;

  // create and read in a CompositeGrid
  // aString nameOfOGFile="cice3.hdf";
  // aString nameOfOGFile="sise.hdf";
  // aString nameOfOGFile="bis2e.order4.hdf";
  // aString nameOfOGFile="bis3e.order4.hdf";
  // aString nameOfOGFile="qsib1a.hdf";

  // aString nameOfOGFile="rodArray1x1ye1.order2.hdf";
  aString nameOfOGFile="rodArray2x2ye1.order2.hdf";

//  FILE *pogiInputFile = fopen("pogiInputFile","r" );

//  const int maxArgs=50;
//  argv = new  char* [maxArgs];  // could delete
    
  ParallelOverlappingGridInterpolator::ExplicitInterpolationStorageOptionEnum 
                  option=ParallelOverlappingGridInterpolator::precomputeNoCoefficients;

  bool usePolyTZ = true; // false;
  bool saveCheckFile=true;
  bool compareToCheckFile=true;

  aString line;
  const int maxBuff=300;
  char buff[maxBuff];
  int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
     if( len=line.matches("-grid=") )
     {
       nameOfOGFile=line(len,line.length()-1);
       printF(" Using grid file = [%s]\n",(const char*)nameOfOGFile);
     }
     if( len=line.matches("-debug=") )
     {
       sScanF(line(len,line.length()-1),"%i",&ParallelOverlappingGridInterpolator::debug);
       printF(" ParallelOverlappingGridInterpolator::debug=%i\n",ParallelOverlappingGridInterpolator::debug);
     }
     else if( line.matches("-full") )
     {
       option=ParallelOverlappingGridInterpolator::precomputeAllCoefficients;
     }
    }

//   while( getLineFromFile(pogiInputFile,buff,maxBuff) )
//   {
//     line=buff;
//     printF(" line=[%s]\n",(const char*)line);
//     if( line[0]=='*' ) continue;  // skip comments
      
//     if( len=line.matches("-grid=") )
//     {
//       nameOfOGFile=line(len,line.length()-1);
//       if( myid==0 ) printf(" Using grid file = [%s]\n",(const char*)nameOfOGFile);
//     }
//     else if( line.matches("-full") )
//     {
//       option=ParallelOverlappingGridInterpolator::precomputeAllCoefficients;
//     }
    
//   }
//   fclose(pogiInputFile);
  

//    if( argc>1 )
//    {
//      char *command = argv[1];
//      if( strncmp(command,"debug",5)==0 )
//      {
//        int debug=0;
//        sscanf(command,"debug=%i",&debug);
//        ParallelOverlappingGridInterpolator::debug=debug;
//        printf("**** Setting debug=%i *****\n",debug);
//      }
//    }
  
  
//   realSerialArray aa(Range(3,7)), bb;
//   aa=1.;
//   bb=aa;
//   bb.display("bb");
  

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update(MappedGrid::THEmask | MappedGrid::THEcenter );
  const int numberOfDimensions = cg.numberOfDimensions();

  Range all;

  const int nc=3;
  const int numberOfComponents=nc;
  realCompositeGridFunction u(cg,all,all,all,nc);
  u=1;
 
  int dw = max(cg[0].discretizationWidth());
  int orderOfAccuracyInSpace=dw-1;
  

  // create a twilight-zone function for checking the errors
  OGFunction *exactPointer;
  if( !usePolyTZ ||
      min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
  {
    // (Use a trig function if this grid is probably periodic in space)
    printF("TwilightZone: trigonometric polynomial\n");
    exactPointer = new OGTrigFunction(2.,2.);  // 2*Pi periodic
  }
  else
  {
    printF("TwilightZone: algebraic polynomial\n");
      
    int degreeOfSpacePolynomial = min(6,dw-1); // 2; // 6; // 2; // 2;
    // degreeOfSpacePolynomial=1;
    
    printF("  >>>dw=%i, Setting TZ degreeOfSpacePolynomial=%i<<<\n",dw,degreeOfSpacePolynomial);

    int degreeOfTimePolynomial = 1;
    exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
				      degreeOfTimePolynomial);
    
  }
  OGFunction & e = *exactPointer;


  ParallelOverlappingGridInterpolator pi;
  pi.setExplicitInterpolationStorageOption(option);
  
  Interpolant interpolant;
  #ifndef USE_PPP
    interpolant.updateToMatchGrid(cg);
  #endif

  IntegerArray gridsToInterpolate(cg.numberOfComponentGrids());
  gridsToInterpolate=1;
  IntegerArray gridsToInterpolateFrom(cg.numberOfComponentGrids());
  gridsToInterpolateFrom=1;
  
  bool onlyInterpolateSomeGrids=false; // true;
  if( onlyInterpolateSomeGrids )
  { // only interpolate pts on grid 0:
    gridsToInterpolate=0;
    gridsToInterpolate(0)=1;
  }
  
  const int nit=2;
  for( int it=0; it<nit; it++ ) // test updateToMatchGrid on 2nd iteration
  {
    #ifdef USE_PPP
    if( it==0 )
      pi.setup(u);
    else
      pi.updateToMatchGrid(u);
    #endif

    real t=0.;
    Index I1,I2,I3;
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & ug = u[grid];
      const intArray & mask = mg.mask();

      const realSerialArray & uLocal  =  ug.getLocalArrayWithGhostBoundaries();
      const realSerialArray & xLocal  =  mg.center().getLocalArrayWithGhostBoundaries();
      const intSerialArray & maskLocal = mask.getLocalArrayWithGhostBoundaries();

      real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
      const int uDim0=uLocal.getRawDataSize(0);
      const int uDim1=uLocal.getRawDataSize(1);
      const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

      real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
      const int xDim0=xLocal.getRawDataSize(0);
      const int xDim1=xLocal.getRawDataSize(1);
      const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

      const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
      const int maskDim0=maskLocal.getRawDataSize(0);
      const int maskDim1=maskLocal.getRawDataSize(1);
      const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

      // getIndex(mg.gridIndexRange(),I1,I2,I3);
      getIndex(mg.dimension(),I1,I2,I3);
    
      Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)),min(I1.getBound(),uLocal.getBound(0)));
      Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)),min(I2.getBound(),uLocal.getBound(1)));
      Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)),min(I3.getBound(),uLocal.getBound(2)));
      int i1,i2,i3;
      if( mg.numberOfDimensions()==2 )
      {
	FOR_3D(i1,i2,i3,J1,J2,J3)
	{
	  if( MASK(i1,i2,i3)>0. )
	  {
	    real x0 = X(i1,i2,i3,0);
	    real y0 = X(i1,i2,i3,1);
	    for( int c=0; c<nc; c++ )
	      U(i1,i2,i3,c) =e(x0,y0,0.,c,t);
	  }
	  else
	  {
	    for( int c=0; c<nc; c++ )
	      U(i1,i2,i3,c) =0.;
	  }
	
	}
      }
      else
      {
	FOR_3D(i1,i2,i3,J1,J2,J3)
	{
	  if( MASK(i1,i2,i3)>0. )
	  {
	    real x0 = X(i1,i2,i3,0);
	    real y0 = X(i1,i2,i3,1);
	    real z0 = X(i1,i2,i3,2);
	    for( int c=0; c<nc; c++ )
	      U(i1,i2,i3,c) =e(x0,y0,z0,c,t);
	  }
	  else
	  {
	    for( int c=0; c<nc; c++ )
	      U(i1,i2,i3,c) =0.;
	  }
	}
      }
      
    } // end for grid
  

    // u.display("************u before interpolate");


    #ifdef USE_PPP
      pi.interpolate(u,gridsToInterpolate);
    #else
      interpolant.interpolate(u);
    #endif

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      u[grid].updateGhostBoundaries(); // *wdh* 090507 -- so interp pts on parallel ghost are set
      u[grid].periodicUpdate(); // this IS needed  *wdh* 060306
    }
  
    if( it==0 && saveCheckFile )
    {
      aString checkFileName = nameOfOGFile(0,nameOfOGFile.length()-5) +
                              sPrintF(buff,".pogiNP%i.check",np);
      FILE *check = fopen((const char*)checkFileName,"w");
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	::display(u[grid],sPrintF(buff,"Solution on grid %i",grid),check,"%20.14e ");
      }
      fclose(check);
      printF("Solution saved to the check file: %s\n",(const char*)checkFileName);
    }
    
    // u.display("************* u after interpolate");

    real err=0.;

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( !gridsToInterpolate(grid) ) continue;
      
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & ug = u[grid];
      const intArray & mask = mg.mask();

      const realSerialArray & uLocal  =  ug.getLocalArrayWithGhostBoundaries();
      const realSerialArray & xLocal  =  mg.center().getLocalArrayWithGhostBoundaries();
      const intSerialArray & maskLocal = mask.getLocalArrayWithGhostBoundaries();

      real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
      const int uDim0=uLocal.getRawDataSize(0);
      const int uDim1=uLocal.getRawDataSize(1);
      const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

      real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
      const int xDim0=xLocal.getRawDataSize(0);
      const int xDim1=xLocal.getRawDataSize(1);
      const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

      const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
      const int maskDim0=maskLocal.getRawDataSize(0);
      const int maskDim1=maskLocal.getRawDataSize(1);
      const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

      getIndex(mg.gridIndexRange(),I1,I2,I3,1);

      const int ng=orderOfAccuracyInSpace/2;
      const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
      
//       Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)+ng ),min(I1.getBound(),uLocal.getBound(0)-ng ));
//       Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)+ng ),min(I2.getBound(),uLocal.getBound(1)-ng ));
//       Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)+ng3),min(I3.getBound(),uLocal.getBound(2)-ng3));

      int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);

      int i1,i2,i3;
      if( ok && mg.numberOfDimensions()==2 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( MASK(i1,i2,i3)<0. )
	  {
	    real x0 = X(i1,i2,i3,0);
	    real y0 = X(i1,i2,i3,1);
	    for( int c=0; c<nc; c++ )
	    {
	      if( false )
	      {
		printf("myid=%i: i=(%i,%i) u=%9.2e exact=%9.2e err=%8.2e\n",myid,i1,i2,U(i1,i2,i3,c),e(x0,y0,0.,c,t),
		       fabs(U(i1,i2,i3,c)-e(x0,y0,0.,c,t)));
	      }
	      err=max(err,fabs(U(i1,i2,i3,c)-e(x0,y0,0.,c,t)));
	    }
	  }
	
	}
      }
      else if( ok )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( MASK(i1,i2,i3)<0. )
	  {
	    real x0 = X(i1,i2,i3,0);
	    real y0 = X(i1,i2,i3,1);
	    real z0 = X(i1,i2,i3,2);
	    for( int c=0; c<nc; c++ )
	      err=max(err,fabs(U(i1,i2,i3,c)-e(x0,y0,z0,c,t)));
	  }
	}
      }
      
    } // end for grid
  
    err=getMaxValue(err,0);

    real size=pi.sizeOf()/(1024.*1024);
    real minSize=ParallelUtility::getMinValue(size);
    real maxSize=ParallelUtility::getMaxValue(size);
    printF(" ============= it=%i maximum error = %8.2e (%i components, dw=%i, %s, %s) "
           "np=%i size[min,max]=(%g,%g) (Mb)===========\n",
	   it,err,nc,dw,
	   option==ParallelOverlappingGridInterpolator::precomputeNoCoefficients ? "sparse" : "full",
	   (const char*)nameOfOGFile,np,minSize,maxSize);
    
  }
  
  Overture::finish();     
//  Optimization_Manager::Exit_Virtual_Machine();
  return 0;
}
