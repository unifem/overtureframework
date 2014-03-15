// **************************************************************************************************
// ******************** Test the getLocalInterpolationData function ********************************************
// **************************************************************************************************

// Examples
//
// srun -N1 -n2 -ppdebug tglid -g=sise1.order2.hdf
// srun -N1 -n4 -ppdebug tglid -g=cice2.order2.hdf
// srun -N1 -n8 -ppdebug tglid -g=sibe1.order2.hdf
//
//
//

#include "Overture.h"
#include "display.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "InterpolationData.h"

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




//================================================================================
//
//  Test the getLocalInterpolationData function
//
//================================================================================
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();

  aString inFile="sise1.order2.hdf";

  printF("Usage: tglid -g=<name> -debug=<> \n");

  int debug=3;

  aString line;
  int len=0;
  for( int i=1; i<argc; i++ )
  {
    line=argv[i];
    if( len=line.matches("-g=") )
    {
      inFile=line(len,line.length()-1);
    }
    else if( len=line.matches("-debug=") )
    {
      sScanF(line(len,line.length()-1),"%i",&debug);
    }
  }


  FILE *debugFile=NULL;
  
  aString fileName=sPrintF("tglidNP%ip%i.debug",np,myid);
  debugFile=fopen((const char*)fileName,"w"); // open a different file on each proc.
  printF("tglid:: output written to debug file %s\n",(const char*)fileName);

  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    const int numParallelGhost=2;
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numParallelGhost);
  #endif


  CompositeGrid cg;
  getFromADataBase(cg,inFile);
  cg.update( MappedGrid::THEmask );
  
  const int numberOfComponentGrids=cg.numberOfComponentGrids();
  const int numberOfDimensions=cg.numberOfDimensions();


  InterpolationData *interpData=NULL;
  ParallelGridUtility::getLocalInterpolationData( cg, interpData );
  
  int iv[3]={0,0,0}; //
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    InterpolationData & ipd = interpData[grid];

    intSerialArray & ip = ipd.interpolationPoint;
    intSerialArray & il = ipd.interpoleeLocation;
    intSerialArray & ig = ipd.interpoleeGrid;
    intSerialArray & viw= ipd.variableInterpolationWidth;
    realSerialArray & ci= ipd.interpolationCoordinates;

    if( debug & 2 )
    {
      const intArray & mask = cg[grid].mask();
      
      for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
      {
	fprintf(debugFile," grid=%i : i=%5i ip=(%4i,%4i,%4i) il=(%4i,%4i,%4i) ig=%4i viw=%2i ci=(%5.2f,%5.2f,%5.2f)\n",
                grid,i,
                ip(i,0),ip(i,1),(numberOfDimensions==2 ? 0 : ip(i,2)),
                il(i,0),il(i,1),(numberOfDimensions==2 ? 0 : il(i,2)),ig(i),viw(i),
                ci(i,0),ci(i,1),(numberOfDimensions==2 ? 0 : ci(i,2))
                );

        if( ig(i)<0 || ig(i)>=numberOfComponentGrids )
	{
	  printF("tglid:ERROR:grid=%i, pt i=%i INVALID interpoleeGrid=%i\n",grid,i,ig(i));
	}
        if( viw(i)<0 || viw(i)>11 )
	{
	  printF("tglid:ERROR:grid=%i, pt i=%i INVALID variableInterpolationWidth=%i\n",grid,i,viw(i));
	}

	// check that the interp pt is really on this proc.
	for( int axis=0; axis<numberOfDimensions; axis++ )
	  iv[axis]=ip(i,axis);
	int p= mask.Array_Descriptor.findProcNum( iv );  // interp. pt. lives on this processor

	if( p!=myid )
	{
	  printf("tglid:ERROR: pt %i, grid=%i does not exist on myid=%i!\n",i,grid,myid);
	}
      }
    }
    
    int totalNumInterp = ParallelUtility::getSum(ipd.numberOfInterpolationPoints);
    if( totalNumInterp!=cg.numberOfInterpolationPoints(grid) )
    {
      printF("tglid:ERROR:grid=%i : cg.numberOfInterpolationPoints=%i is NOT equal to numInterp=%i\n",
           grid,cg.numberOfInterpolationPoints(grid), totalNumInterp); 
    }
    else
    {
      printF(" grid=%i : cg.numberOfInterpolationPoints=%i, numInterp=%i\n",grid,cg.numberOfInterpolationPoints(grid),
	     totalNumInterp);
    }
    
    
  }
  fclose(debugFile);

  Overture::finish();          
  return 0;
}

