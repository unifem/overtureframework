#include <algorithm>

#include "LoadBalancer.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"

using namespace std;

int LoadBalancer::debug=0;
int LoadBalancer::numberOfLoadBalances=0;
real LoadBalancer::maximumImbalance=0., LoadBalancer::averageImbalance=0., 
     LoadBalancer::averageNumberOfGrids=0., LoadBalancer::averageNumberOfBlocks=0., 
     LoadBalancer::averageWorkPerBlock=0.;


LoadBalancer::LoadBalancer()
// ========================================================================
/// \brief Constructor for the load balancing class
/// 
// ========================================================================
{
 np=max(1,Communication_Manager::Number_Of_Processors);
 processorID= new int [np];
 for( int i=0; i<np; i++ )
   processorID[i]=i;

 loadBalancer=defaultLoadBalancer;
 targetMaximumLoadImbalance=.1;  // attempt to achieve at most 10% load imbalance

}

LoadBalancer::
~LoadBalancer()
{
  delete [] processorID;
}


int LoadBalancer::
setProcessors(int pStart, int pEnd)
// ===============================================================================
/// 
/// \brief Specify a contiguous set of processors over which to load balance.
// 
// ===============================================================================
{
  if( pStart>pEnd )
  {
    printF(" LoadBalancer::setProcessors:ERROR: pFirst>pLast\n");
    return 1;
  }
  
  np=pEnd-pStart+1;
  delete [] processorID;
  processorID= new int [np];
  for( int i=0; i<np; i++ )
    processorID[i]=pStart+i;

  return 0;
}


int LoadBalancer::
setLoadBalancer(LoadBalancerTypeEnum loadBalancer_ )
// ======================================================================================
/// \brief Choose the type of load balancing algorithm to use.
///
///  enum LoadBalancerTypeEnum
///  {
///    defaultLoadBalancer=0,
///    KernighanLin,
///    sequentialAssignment, // grid g is placed on processor p= g % np;
///    randomAssignment,     // grid g is placed in a random processor -- this is used for testing
///    allToAll,             // grid g is given all processors
///    userDefined
///  }
// 
// ======================================================================================
{
  loadBalancer=loadBalancer_;
  return 0;
}

LoadBalancer::LoadBalancerTypeEnum LoadBalancer::
getLoadBalancerType() const
// ========================================================================================
/// \brief Return the active load balancing algorithm.
// ========================================================================================
{
  return loadBalancer;
}

aString LoadBalancer::
getLoadBalancerTypeName() const
// ========================================================================================
/// \brief Return the name of the active load balancing algorithm.
// ========================================================================================
{
  if( loadBalancer==defaultLoadBalancer ) return "KernighanLin";
  else if( loadBalancer==KernighanLin ) return "KernighanLin";
  else if( loadBalancer==sequentialAssignment ) return "sequentialAssignment";
  else if( loadBalancer==randomAssignment ) return "randomAssignment";
  else if( loadBalancer==allToAll ) return "allToAll";
  else if( loadBalancer==userDefined ) return "userDefined";
  else return "unknownLoadBalancertype";
  
}



int LoadBalancer::
setTargetMaximumLoadImbalance( real target )
// ========================================================================================
/// \brief Set the target maximum relative load imbalance, 0< target < 1.
///
/// \details
///    Example, if target=.1 then attempt to achieve a maximum relative load imbalance of .1 (10%). 
/// 
///       relative load imbalance = (load - aveLoad)/aveLoad. 
///
// ========================================================================================
{
  targetMaximumLoadImbalance=target;
  return 0;
}

// print statistics from the load balancing
int LoadBalancer::
printStatistics(FILE *file /* =NULL */)
// ========================================================================================
/// \brief  Print statistics from the load balancer.
///
/// \param file (input) : write statistics to this file.
/// 
// ========================================================================================
{
  const int nlb=max(1,numberOfLoadBalances);  // avoid division by 0
  fPrintF(file,"\n"
          "------------------ LoadBalancer Statistics -----------------\n"
          "   load-balancer=%s, np=%i, number-of-load-balances=%i, target-max-imbalance=%g\n"
          "   maxImbalance=%4.1f %%, aveImbalance=%4.1f %%\n"
          "   aveNumberOfGrids=%i, aveNumberOfBlocks=%i, aveWorkPerBlock=%g\n"
          "------------------------------------------------------------\n",
	  (const char*)getLoadBalancerTypeName(),np,numberOfLoadBalances,targetMaximumLoadImbalance,
	  100.*maximumImbalance,100.*averageImbalance/nlb, 
	  int(averageNumberOfGrids/nlb+.5),int(averageNumberOfBlocks/nlb+.5),
          averageWorkPerBlock/nlb);
  return 0;
}




namespace // this makes the next class local to this file
{

class WorkLoad
{
public:
  WorkLoad( int grid_, real workLoad_ ){ grid=grid_; workLoad=workLoad_;}
  // For sorting by work-load:
  bool operator< ( const WorkLoad & x )const{ return workLoad<x.workLoad; }

  real workLoad;
  int grid;

};

 
};

int LoadBalancer::
assignLoadBalance( GridCollection & gc, GridDistributionList & gridDistributionList,
                   int refinementLevel /* = 0  */ ) const
// ========================================================================================
/// \brief Assign the load balance to the grids in the GridCollection
///
/// \param gc (input/output) : determine a load balance for this GridCollection 
/// \param gridDistributionList (input) : holds the load balance information
/// \param refinementLevel (input) : assign load balance to levels greater than or equal to this value.
///
// ========================================================================================
{
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = gc[grid];
    if( gc.refinementLevelNumber(grid)>=refinementLevel )
    {
      int pStart=-1,pEnd=0;
      gridDistributionList[grid].getProcessorRange(pStart,pEnd);

      printF("assignLoadBalance: assign grid %i to processors=[%i,%i]\n",grid,pStart,pEnd);
    
      mg.specifyProcesses(Range(pStart,pEnd));
    }
  }
  return 0;
}

int LoadBalancer::
assignWorkLoads( GridCollection & gc, GridDistributionList & gridDistributionList,
                 int refinementLevel /* = 0  */ ) const
// ========================================================================================
/// \brief Assign work loads for each grid based on the number of grid points and save these
/// results in the gridDistributionList.
///
/// \param gc (input) : determine work-loads for this grid
/// \param gridDistributionList (output) : holds information about the work loads
/// \param refinementLevel (input) : assign work loads to levels greater than or equal to this value.
///
// ========================================================================================
{
  return assignWorkLoads( *gc.rcData,gridDistributionList,refinementLevel );
}

int LoadBalancer::
assignWorkLoads( GridCollectionData & gc, GridDistributionList & gridDistributionList,
                 int refinementLevel /* = 0  */ ) const
// ========================================================================================
/// \brief Assign work loads for each grid based on the number of grid points and save these
/// results in the gridDistributionList.
/// *This* version takes a GridCollectionData -- needed for when we get/put a GridCollection.
///
/// \param gc (input) : determine work-loads for this GridCollectionData
/// \param gridDistributionList (output) : holds information about the work loads
/// \param refinementLevel (input) : assign work loads to levels greater than or equal to this value.
///
// ========================================================================================
{
  // First make sure that the gridDistributionList is the correct size
  GridDistribution gd;
  while( gridDistributionList.size()<gc.numberOfComponentGrids )
  {
    gridDistributionList.push_back(gd);
  }
  while( gridDistributionList.size()>gc.numberOfComponentGrids )   // fix this -----------------
  {
    gridDistributionList.erase(gridDistributionList.end());
  }
  
  int gridPoints[3]={1,1,1}; //
  for( int grid=0; grid<gc.numberOfComponentGrids; grid++ )
  {
    MappedGrid & mg = gc[grid];
    if( gc.refinementLevelNumber(grid)>=refinementLevel )
    {
      const IntegerArray & d = mg.dimension();
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	gridPoints[axis]=d(1,axis)-d(0,axis)+1;
      
      real workLoad = (d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);
    
      gridDistributionList[grid].setGridAndRefinementLevel(grid,gc.refinementLevelNumber(grid));
      gridDistributionList[grid].setWorkLoadAndGridPoints(workLoad,gridPoints);

      if( debug & 2 )
	printF("LoadBalancer::assignWorkLoads: grid=%i, workLoad=%8.2e\n",grid,workLoad);
    }
  }

  return 0;
}


int LoadBalancer::
determineLoadBalance( GridCollection & gc, GridDistributionList & gridDistributionList,
                      int refinementLevel /* = 0  */,
                      int mgStart /* = 0 */, int mgEnd /* = INT_MAX */  ) const
// ========================================================================================
/// \brief Determine a load balance for a grid collection. This function will first compute
/// the work loads (based on the number of grid points) and then save them in the gridDistributionList.
///
/// \param gc (input) : determine a load balance for this GridCollection 
/// \param gridDistributionList (output) : holds information about the load balance
/// \param refinementLevel (input) : determine load balance for grids with levels greater than or equal to this value.
/// \param mgStart, mgEnd (input) : load balance multigrid levels level=mgStart,...,mgEnd (by default load balance
///                                 all MG levels).
///
// ========================================================================================
{
  assignWorkLoads(gc,gridDistributionList);
  return determineLoadBalance(gridDistributionList,refinementLevel,mgStart,mgEnd);
}



int LoadBalancer::
determineLoadBalance(GridDistributionList & gridDistributionList,
                     int refinementLevel /* = 0  */,
                     int mgStart /* = 0 */, int mgEnd /* = INT_MAX */ ) const
// ========================================================================================
/// \brief Determine a load balance for a gridDistributionList. The work-loads in the 
/// gridDistributionList should already have been assigned.
///
/// \param gridDistributionList (output) : holds information about the load balance
/// \param refinementLevel (input) : determine load balance for grids with levels greater than or 
///    equal to this value. The grids on lower levels are not changed. 
/// \param mgStart, mgEnd (input) : load balance multigrid levels level=mgStart,...,mgEnd (by default load balance
///                                 all MG levels).
///
/// ========================================================================================
{
  int returnValue=0;

  LoadBalancerTypeEnum loadBalancerActual= loadBalancer;
  if( gridDistributionList.size()<=1 && loadBalancer==defaultLoadBalancer )
  { 
    // If there is only one grid (and we are using the default LB), just use the LB allToAll. 
    // This will force the grid to be distrbuted across all processors. The bin-packing algorithm may not do this 
    loadBalancerActual=allToAll;
  }
  

  if( loadBalancerActual==KernighanLin || loadBalancerActual==defaultLoadBalancer)
  {
    returnValue=determineLoadBalanceKernighanLin( gridDistributionList,refinementLevel,mgStart,mgEnd );
  }
  else if( loadBalancerActual==sequentialAssignment )
  {
    // put grid 0 on p=0, grid 1 on p=1, ...
    //      p = grid % numberOfProcessors
    const int np = max(1,Communication_Manager::numberOfProcessors());
    for( int grid=0; grid<gridDistributionList.size(); grid++ )
    {
      const int mgLevel=gridDistributionList[grid].getMultigridLevel();
      if( gridDistributionList[grid].getRefinementLevel()>=refinementLevel && mgLevel>=mgStart && mgLevel<=mgEnd )
      {
	int pStart= grid % np; int pEnd=pStart;
	gridDistributionList[grid].setProcessors(pStart,pEnd);
      }
    }
  }
  else if( loadBalancerActual==randomAssignment )
  {
    // Assign a grid to a random number of processors and a random starting rocessor 
    //    -- this is used for testing
    const int np = max(1,Communication_Manager::numberOfProcessors());
    for( int grid=0; grid<gridDistributionList.size(); grid++ )
    {
      const int mgLevel=gridDistributionList[grid].getMultigridLevel();
      if( gridDistributionList[grid].getRefinementLevel()>=refinementLevel && mgLevel>=mgStart && mgLevel<=mgEnd )
      {
        real rand1=rand()/(RAND_MAX+1.0), rand2=rand()/(RAND_MAX+1.0);

        int numProc= 1 + (int)( np*rand1+.5 );  // a random int in [1,np]
        numProc=max(1,min(np,numProc));  // make sure the value is valid

	int pStart= (int)( (np-numProc+1)*rand2+.5 );  // a random int in [0,np-numProc]
        pStart=max(0,min(np-numProc,pStart));  // make sure the value is valid
        int pEnd=pStart+numProc-1;

	if( false )
	{
	  const int myid=max(0,Communication_Manager::My_Process_Number);
	  fflush(0);
	  Communication_Manager::Sync();
	  printf("LoadBalance:random: myid=%i np=%i numProc=%i pStart=%i pEnd=%i rand1=%5.2f rand2=%5.2f\n",
                myid,np,numProc,pStart,pEnd,rand1,rand2);
	  fflush(0);
	  Communication_Manager::Sync();
	}
	
	gridDistributionList[grid].setProcessors(pStart,pEnd);
      }
    }
  }
  else if( loadBalancerActual==allToAll )
  {
    // All grids are assigned to all processors
    // *wdh* 2015/06/30 const int np = max(1,Communication_Manager::numberOfProcessors());
    for( int grid=0; grid<gridDistributionList.size(); grid++ )
    {
      const int mgLevel=gridDistributionList[grid].getMultigridLevel();
      if( gridDistributionList[grid].getRefinementLevel()>=refinementLevel && mgLevel>=mgStart && mgLevel<=mgEnd )
      {
	// int pStart= 0; int pEnd=np-1;
        int pStart=processorID[0], pEnd=processorID[np-1]; // *wdh* 2015/07/01
	gridDistributionList[grid].setProcessors(pStart,pEnd);
      }
    }
  }
  else if( loadBalancerActual==userDefined )
  {
    returnValue=determineLoadBalanceUserDefined( gridDistributionList,refinementLevel,mgStart,mgEnd );
  }
  else
  {
    printF("LoadBalancer::loadBalance:ERROR:unknown loadBalancer=%i\n",loadBalancerActual);
    OV_ABORT("error");
  }

  saveStatistics( gridDistributionList );
  
  return returnValue;
}



int LoadBalancer::
determineLoadBalanceKernighanLin( GridDistributionList & gridDistributionList,
                                  int refinementLevel /* = 0  */,
                                  int mgStart /* = 0 */, int mgEnd /* = INT_MAX */ ) const
// ========================================================================================
/// \brief
///     Determine a load balance using a variant of the Kernighan-Lin algorithm
/// \note
///   This load balancer does not take into account any communication costs.
///   Adjustments to the Kernighan-Lin algorithm are added to take into account the fact that
///   grids can be split across a contiguous range of processors. 
///
/// \param gridDistributionList (input/output) : On input this should hold the work-loads for
///    each grid. On outout this will hold the information about the load balance.
/// \param refinementLevel (input) : determine load balance for grids with levels greater than or equal to this value.
/// \param mgStart, mgEnd (input) : load balance multigrid levels level=mgStart,...,mgEnd (by default load balance
///                                 all MG levels).
///
// ========================================================================================
{
  // printF("determineLoadBalanceKernighanLin: mgStart=%i, mgEnd=%i\n",mgStart,mgEnd);

  const int numberOfGrids = gridDistributionList.size();
  
  // The workLoadList will hold the sorted lists of work loads
  std::vector<WorkLoad> workLoadList;

  // First determine the total load
  RealArray localWork(numberOfGrids);
  real totalWork=0.;
  for( int grid=0; grid<numberOfGrids; grid++ )
  {
    real workLoad=0.;
    const int mgLevel=gridDistributionList[grid].getMultigridLevel();
    if( mgLevel>=mgStart && mgLevel<=mgEnd )
    {
      workLoad = gridDistributionList[grid].getWorkLoad();
    }
    
    localWork(grid) = workLoad;
    workLoadList.push_back(WorkLoad(grid,workLoad));
    totalWork+=localWork(grid);

  }

  real aveWork=totalWork/max(1,np);
  if( debug & 2 )
    printF(" LB: totalWork=%8.2e, ave-work/proc=%8.2e \n",totalWork,aveWork);
  
  // *** sort the grids from smallest to largest workload ****

  sort(workLoadList.begin(), workLoadList.end());
  if( debug & 2 )
  {
    printF("Grids sorted by work-loads:\n");
    for( int g=0; g<numberOfGrids; g++ )
    {
      printF("grid=%i, workLoad=%4.1f%%\n",workLoadList[g].grid,100.*workLoadList[g].workLoad/totalWork);
    }
  }
  

  // splitFraction: split a grid whose work load is bigger than splitFraction*aveWork
  //           -- the worst load balance will then be less than splitFraction <- NO : because
  //  split grids must be contiguous
  
  // ** could start with splitFraction fairly big (.5 ?) check the balance, if not good
  //  enough then decrease splitFraction. 
  real splitFraction=.5; 

  real maxImbalance, aveImbalance;
  RealArray work(np);          // work bin: holds work allocated per processor : these are the bin's we are filling
  IntegerArray numGrids(np);   // holds number of "grids" placed on each processor

  const int maximumNumberOfTries=5;
  for( int it=0; it<maximumNumberOfTries; it++ )  // iterate to improve load balance
  {
    work=0;
    numGrids=0;

    // Grids with level<refinementLevel have already been load balanced and we do not want to change them
    if( refinementLevel>0 )
    {
      // Fill in the work bin's with the work-loads from grids with level<refinementLevel
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
        const int mgLevel=gridDistributionList[grid].getMultigridLevel();
        if( gridDistributionList[grid].getRefinementLevel()<refinementLevel  && mgLevel>=mgStart && mgLevel<=mgEnd )
	{
	  int pStart=-1,pEnd=0;
	  gridDistributionList[grid].getProcessorRange(pStart,pEnd);
          Range P(pStart,pEnd);
          work(P)+=gridDistributionList[grid].getWorkLoad()/max(1,pEnd-pStart+1);
          numGrids(P)+=1;

          if( debug & 4 )
            printF("+++ (1) grid=%i work(0)=%8.2e, level=%i \n",grid,work(0),
                  gridDistributionList[grid].getRefinementLevel());

	}
      }
    }
    
    // **** Start load balance algorithm ***
    for( int g=0; g<numberOfGrids; g++ )
    {
      // Go through the grids from largest to smallest work-load:
      int grid= workLoadList[numberOfGrids-g-1].grid;

      const int mgLevel=gridDistributionList[grid].getMultigridLevel();
      if( gridDistributionList[grid].getRefinementLevel()<refinementLevel || !( mgLevel>=mgStart && mgLevel<=mgEnd ) )
        continue;  // skip this grid since it has already been included

      int numProc=1;  // split this grid on this many processors
      if( localWork(grid)>aveWork*splitFraction )
      {
        // Here is a first guess at how many processors to use:
	numProc= min( np, int( localWork(grid)/(aveWork*splitFraction) +.5) );

        // Determine a nearby value for numProc that gives a good array distribution 
        const int minProc=1, maxProc=np;
        gridDistributionList[grid].determineAGoodNumberOfProcessors(numProc,minProc,maxProc);

	if( false ) //  sanity check 
	{
	  // Determine a nearby value for numProc that gives a good array distribution 
	  assert( numProc>=1 && numProc<=np );
	  int numProcMax= ParallelUtility::getMaxValue(numProc);

	  if( numProcMax!=numProc )
	  {
            const int myid=max(0,Communication_Manager::My_Process_Number);
   	    fflush(0);
	    printf("LoadBalancer::loadBalanceKL:ERROR: myid=%i numProc=%i BUT numProcMax=%i!!\n",
                   myid,numProc,numProcMax);
            fflush(0);
	    Overture::abort("error");
	  }
	  
	}
	
      }
    
      // find the contiguous range of numProc processors with the least work so far
      int pMin=0;
      real minWork=REAL_MAX;
      for( int p=0; p<np-numProc+1; p++ )
      {
	real workInSet=sum(work(Range(p,p+numProc-1))); // sum work for p..p+numProc-1

	// printf("grid=%i p=%i workInSet=%8.2e minWork=%8.2e\n",grid,p,workInSet,minWork);
      
	if( workInSet<minWork )
	{
	  pMin=p;
	  minWork=workInSet;
	}
      }
      int pStart=pMin;
      int pEnd=numProc+pStart-1;
    
      gridDistributionList[grid].setProcessors(pStart,pEnd);

      // update the work-bin (work in each processor)
      for( int p=pStart; p<=pEnd; p++ )
      {
	work(p)+= localWork(grid)/max(1,pEnd-pStart+1);

        if( debug & 4 )
	  printF("+++ (2) work(%i)=%8.2e (grid=%i,level=%i)\n",p,work(0),grid,
		 gridDistributionList[grid].getRefinementLevel());

	numGrids(p)+=1;
      }
    }
    // *** finished the load balance algorithm ***

    // Determine properties of the computed load balance
    maxImbalance=0., aveImbalance=0.;
    for( int p=0; p<np; p++ )
    {
      if( debug & 4 ) printF("+++ p=%i work(p)=%8.2e \n",p,work(p));
      
      real imbalance = fabs(work(p)-aveWork)/aveWork;
      maxImbalance=max(maxImbalance,imbalance);
      aveImbalance+=imbalance;
    }
    aveImbalance/=np;
    if(  debug & 4 )
    {
      printF("...splitFraction=%5.2f: maximum imbalance = %4.1f%%, Average imBalance=%4.1f %%\n",
               splitFraction,100.*maxImbalance,100.*aveImbalance);
    }
    if( maxImbalance<=targetMaximumLoadImbalance )
    { // we accept this load balance

      break;
    }
    else
    { // choose a smaller splitFraction and try again
      splitFraction=splitFraction/2.;
    }
  }
  
//   // save statistics:
//   numberOfLoadBalances++;
//   maximumImbalance=max(maximumImbalance,maxImbalance);
//   averageImbalance+=aveImbalance;
//   averageNumberOfGrids+=numberOfGrids;
//   int numberOfBlocks=0;
//   for( int grid=0; grid<numberOfGrids; grid++ )
//   {
//     int pStart=-1,pEnd=0;
//     gridDistributionList[grid].getProcessorRange(pStart,pEnd);
//     numberOfBlocks+=pEnd-pStart+1;
//   }
//   averageNumberOfBlocks+=numberOfBlocks;
//   averageWorkPerBlock += totalWork/numberOfBlocks;
  
  if(  debug >0 )
  {
    int numberOfBlocks=sum(numGrids);
    printF("--- KL-load-balance: np=%i grids=%i blocks=%i splitFraction=%7.4f max-imbalance = %4.1f%%, "
           "ave-imBalance=%4.1f %%\n",
	   np,numberOfGrids,numberOfBlocks,splitFraction,100.*maxImbalance,100.*aveImbalance);
    
    if( debug & 2 )
    {
      printF("--- Load balance results (np=%i) ---\n",np);
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	int pStart=-1,pEnd=0;
	gridDistributionList[grid].getProcessorRange(pStart,pEnd);
	     
	printF(" grid=%i : work=%4.1f%% processors=[%i,%i]\n",grid,100.*localWork(grid)/totalWork,pStart,pEnd);
      }

      printF("--- Work per processor (np=%i) ---\n",np);
      for( int p=0; p<np; p++ )
      {
	printF(" p=%i : grids=%i, work/ave=%5.2f (imbalance=%5.1f%%) \n",
	       p,numGrids(p),work(p)/aveWork,100.*(work(p)-aveWork)/aveWork);
      }
    }
  }
  
  return 0;

}

int LoadBalancer::
determineLoadBalanceUserDefined( GridDistributionList & gridDistributionList,
                                 int refinementLevel /* = 0  */,
                                 int mgStart /* = 0 */, int mgEnd /* = INT_MAX */ ) const
// ========================================================================================
/// \brief User defined load-balancer -- here you can write your own load balancer.
///
/// \param gridDistributionList (input/output) : On input this should hold the work-loads for
///    each grid. On outout this will hold the information about the load balance.
/// \param refinementLevel (input) : determine load balance for grids with levels greater than or equal to this value.
/// \param mgStart, mgEnd (input) : load balance multigrid levels level=mgStart,...,mgEnd (by default load balance
///                                 all MG levels).
///
// ========================================================================================
{
  OV_ABORT("LoadBalancer:loadBalanceUserDefined:ERROR: this function not implemented\n");

  return 0;
}


int LoadBalancer::
saveStatistics(GridDistributionList & gridDistributionList ) const
// ========================================================================================
/// \brief  Protected routine: Save statistics on the load balancer. This routine is
///  called after load balancing
/// 
/// \param gridDistributionList (output) : holds information about the load balance
///
// ========================================================================================
{
  const int numberOfGrids = gridDistributionList.size();
  
  // Determine properties of the computed load balance

  real totalWork=0.;
  int numberOfBlocks=0;
  RealArray work(np);          // processor bin: holds work allocated per processor
  work=0.;
  for( int grid=0; grid<numberOfGrids; grid++ )
  {
    real workLoad = gridDistributionList[grid].getWorkLoad();
    totalWork+=workLoad;

    int pStart=-1, pEnd=0;
    gridDistributionList[grid].getProcessorRange(pStart,pEnd);

    numberOfBlocks+=pEnd-pStart+1;
    // update the work-bin (work in each processor)
    for( int p=pStart; p<=pEnd; p++ )
      work(p)+= workLoad/max(1,pEnd-pStart+1);
  }
  real aveWork=totalWork/max(1,np);
   
  real maxImbalance=0., aveImbalance=0.;
  for( int p=0; p<np; p++ )
  {
    real imbalance = fabs(work(p)-aveWork)/aveWork;
    maxImbalance=max(maxImbalance,imbalance);
    aveImbalance+=imbalance;
  }
  aveImbalance/=np;
 
  // save statistics:
  numberOfLoadBalances++;
  maximumImbalance=max(maximumImbalance,maxImbalance);
  averageImbalance+=aveImbalance;
  averageNumberOfGrids+=numberOfGrids;
  averageNumberOfBlocks+=numberOfBlocks;
  averageWorkPerBlock += totalWork/numberOfBlocks;

  return 0;
}


int LoadBalancer::
update( GenericGraphicsInterface & gi )
// ===========================================================================
/// \brief Change parameters interactively.
/// 
/// \param gi (input) : 
///
// ==========================================================================
{
  aString menu[]=
  {
    "!LoadBalancer parameters",
    "display parameters",
    "default load balancer",
    "KernighanLin",
    "sequential assignment",
    "random assignment",
    "all to all",
    "user defined",
    "exit",
    ""
  };

  aString answer,answer2;

  gi.appendToTheDefaultPrompt("LoadBalancer>");  
  for(;;)
  {
    gi.getMenuItem(menu,answer,"choose a menu item");
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="display parameters" )
    {
      printF(" The default load balancer is KernighanLin\n"
             "   KernighanLin : a knapsack algorithm\n"
             "   sequentialAssignment : grid g is placed on processor g, g=0,1,...\n"
             "   random assignment : places a random number of processors on each grid\n"
             "   all to all : all grids use all processors\n"
             "   userDefined : use a load balancer defined by a user.\n");
      printF(" loadBalancer=%s\n",(loadBalancer==defaultLoadBalancer ? "defaultLoadBalancer" :
				   loadBalancer==KernighanLin ? "KernighanLin" : 
                                   loadBalancer==sequentialAssignment ? "sequentialAssignment" :
                                   loadBalancer==userDefined ? "userDefined" : "unknown"));
    }
    else if( answer=="default load balancer" )
    {
      loadBalancer=defaultLoadBalancer;
    }
    else if( answer=="KernighanLin" )
    {
      loadBalancer=KernighanLin;
    }
    else if( answer=="sequential assignment" )
    {
      loadBalancer=sequentialAssignment;
    }
    else if( answer=="random assignment" )
    {
      loadBalancer=randomAssignment;
    }
    else if( answer=="user defined" )
    {
      loadBalancer=userDefined;
    }
    else if( answer=="all to all" )
    {
      loadBalancer=allToAll;
    }
    else if( answer=="target maximum load imbalance" )
    {
      printF("The target maximum load imbalance is a value in (0,1). "
             " A value of .1 would mean a maximum 10% load imbalance\n");
      
      gi.inputString(answer,sPrintF("Enter the target maximum load imbalance, current=%f",targetMaximumLoadImbalance));
      sScanF(answer,"%e",&targetMaximumLoadImbalance);
      printF("Setting targetMaximumLoadImbalance=%f\n",targetMaximumLoadImbalance);
    }
    else
    {
      cout << "Unknown response: [" << answer2 << "]\n";
      gi.stopReadingCommandFile();
    }
    
  }

  gi.unAppendTheDefaultPrompt();



  return 0;
}
