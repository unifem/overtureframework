#include "HyperbolicMapping.h"
#include "DataPointMapping.h"
// #include "TridiagonalSolver.h"
#include "display.h"
// #include "arrayGetIndex.h"
// #include "MatchingCurve.h"
#include "ParallelUtility.h"

int HyperbolicMapping::
generateParallel(const int & numberOfAdditionalSteps /* = 0 */)
//===========================================================================
/// \brief  
///     Generate the hyperbolic grid. 
/// \param Notes:
///     Without any smoothing the hyperbolic equations just advance in the normal
///   direction, a constant distance per step.
///     The distance marched is adjusted by smoothing the "volumes" and by smoothing
///   the grid.   
//===========================================================================
{
  // generate the grid in serial (on all ranks)
  generateSerial(numberOfAdditionalSteps);

  // -- assign the distributed array in the DataPointMapping ---

  // Here are ALL the grid points in a serial array:
  RealArray & x = xHyper;
  Range xAxes=rangeDimension;
  
  if( true )
  {
    IntegerArray gid(2,3), dim(2,3);
    gid=gridIndexRange;
    dim=dimension;
    
    if( domainDimension==2 )
    {
      // -- adjust the array, gridIndexRange and dimension to make the array 2D

      x.reshape(x.dimension(0),x.dimension(2),1,xAxes);
      for( int side=0; side<=1; side++ )
      {
	gid(side,0)=gridIndexRange(side,0);
	gid(side,1)=gridIndexRange(side,2);  // note 2
	gid(side,2)=0;

	dim(side,0)=dimension(side,0);
	dim(side,1)=dimension(side,2);  // note 2
	dim(side,2)=0;
	
      }
    }
    
    if( debug & 4 )
    {
      ::display(x,"HyperbolicMapping::generateParallel: array x (serial)",pDebugFile,"%5.2f ");
      fflush(pDebugFile);
    }

    dpm->setDataPoints(x,domainDimension,rangeDimension,dim,gid);

    if( domainDimension==2 )
      x.reshape(x.dimension(0),1,x.dimension(1),xAxes);

    if( false )
    {
      // **** TEST *****
      const int np= max(1,Communication_Manager::numberOfProcessors());
      const int myid=max(0,Communication_Manager::My_Process_Number);
      RealArray rc(1,2), xc(1,2);
      rc=0.;
      rc(0,0)=.5;
      dpm->mapS(rc,xc);
      printf("\n --HY-- np=%i, myid=%i rc=(%e,%e) (xc=%e,%e)\n",np,myid,rc(0,0),rc(0,1),xc(0,0),xc(0,1));
    }
    
  }
  else
  {
    // *OLD WAY* First try:


    dpm->setDomainDimension(domainDimension);  // for partition
    dpm->setRangeDimension(rangeDimension);
    dpm->initializePartition();
    Partitioning_Type & partition = dpm->partition;  // partition for DPM grid

    realArray xd;
    xd.partition(partition);




    Index D1,D2,D3;
    ::getIndex(dimension,D1,D2,D3);
    printF(" D1=[%i,%i], D2=[%i,%i], D3=[%i,%i]\n",
	   D1.getBase(),D1.getBound(),D2.getBase(),D2.getBound(),D3.getBase(),D3.getBound());
  
  
    Index I1,I2,I3;
    ::getIndex(gridIndexRange,I1,I2,I3);
  
    printF(" I1=[%i,%i], I2=[%i,%i], I3=[%i,%i]\n",
	   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());

    // IntegerArray gid(2,3);
    // OV_ABORT("finish me");

    if( domainDimension==2 )
      xd.redim(D1,D3,1,xAxes);
    else
      xd.redim(D1,D2,D3,xAxes);
  

    OV_GET_SERIAL_ARRAY(real,xd,xdLocal);

    Index J1=D1, J2=D2, J3=D3;
    if( domainDimension==2 )
    {
      J2=D3; J3=Range(0,0);
    }
  
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(xd,xdLocal,J1,J2,J3,includeGhost);

    fprintf(pDebugFile," J1=[%i,%i], J2=[%i,%i], J3=[%i,%i]\n",
	    J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound());

    int numGhost=0;
    if( domainDimension==2 )
    {
      x.reshape(x.dimension(0),x.dimension(2),1,xAxes);
      ::display(x,"HyperbolicMapping::generateParallel: array x (serial)",pDebugFile,"%5.2f ");

      xdLocal(J1,J2,0,xAxes)=x(J1,J2,0,xAxes);

      ::display(xdLocal,"HyperbolicMapping::generateParallel: array xdLocal",pDebugFile,"%5.2f ");


      ::display(xd,"HyperbolicMapping::generateParallel: array xd","%5.2f ");

      IntegerArray gid(2,3);
      for( int side=0; side<=1; side++ )
      {
	gid(side,0)=gridIndexRange(side,0);
	gid(side,1)=gridIndexRange(side,2);
	gid(side,2)=0;
      }
    
      dpm->setDataPoints(xd,3,domainDimension,numGhost,gid);

      x.reshape(x.dimension(0),1,x.dimension(1),xAxes);
    }
    else
    {
      xdLocal(D1,D2,D3,xAxes)=x(D1,D2,D3,xAxes);
      dpm->setDataPoints(xd,3,domainDimension,numGhost,gridIndexRange);
    }
  
  }
  


  setBasicInverseOption(dpm->getBasicInverseOption());
  reinitialize();  
      
  mappingHasChanged();
  
  return 0;
}

