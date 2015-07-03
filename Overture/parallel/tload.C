// 
//  Test code for the LoadBalancer
//
//  tload -noplot load
// 
//  mpirun -np 2 -all-local tload
//  mpirun-wdh -np 2 -all-local tload


#include "LoadBalancer.h"
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"
#include "OGTrigFunction.h"
#include "App.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


void
displayDistribution(CompositeGrid & cg, const aString & label)
{
  const int myid = Communication_Manager::My_Process_Number;
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  printF("\n"
         " ======== Parallel Distribution for %s ============\n",(const char*)label);
  printF(" numberOfGrids=%i, gridDistributionList.size()=%i \n",cg.numberOfGrids(),
         cg->gridDistributionList.size());

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    realArray & x = mg.center();
//    const realSerialArray & xLocal = x.getLocalArray();
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
	
    Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
    const intSerialArray & processorSet = partition.getProcessorSet();

    printF("grid=%i: actual-processors=[%i,%i]\n",
	   grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));

    if( true )
    {
      printf(" grid=%i: actual-processors=[%i,%i] myid=%i: x: bounds=[%i,%i][%i,%i] local bounds=[%i,%i][%i,%i]\n",
	     grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)),myid,
	     x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),
	     xLocal.getBase(0),xLocal.getBound(0),xLocal.getBase(1),xLocal.getBound(1));
    }
    if( true )
    {
       intArray & mask = mg.mask();
       // const intSerialArray & maskLocal = mask.getLocalArray();
       intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

       printf(" grid=%i: actual-proc=[%i,%i] myid=%i: mask: bounds=[%i,%i][%i,%i] local bounds=[%i,%i][%i,%i]\n",
 	     grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)),myid,
 	     mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1),
	      maskLocal.getBase(0),maskLocal.getBound(0),maskLocal.getBase(1),maskLocal.getBound(1));
    }
    

  }
  fflush(0);

}

void
displayDistribution(realArray & u, const aString & label)
{
  const Partitioning_Type & partition = u.getPartition();
  const intSerialArray & processorSet = ((Partitioning_Type &)partition).getProcessorSet();

  printF("Parallel Distribution: %s: actual-processors=[%i,%i]\n",(const char*)label,
	 processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));

  fflush(0);
}

// bool hasSameDistribution(const Partitioning_Type & uPartition, const Partitioning_Type & vPartition )
// // ===================================================================================
// // /Description:
// //   Return true if the two Partitioning_Type's have the same parallel distribution.
// //  This function should really be in the Partitioning_Type class.
// // ===================================================================================
// {
//   bool returnValue=true;

//   // We could compare the pointer to the internal partitioning object:
//   // Internal_Partitioning_Type* getInternalPartitioningObject()
//   if( uPartition.getInternalPartitioningObject() == vPartition.getInternalPartitioningObject() )
//   {
//     printF("hasSameDistribution: Partitioning_Type's have the same Internal_Partitioning_Type\n");
    
//     return true;
//   }

//   const intSerialArray & uProcessors  = ((Partitioning_Type &)uPartition).getProcessorSet();
//   const intSerialArray & vProcessors  = ((Partitioning_Type &)vPartition).getProcessorSet();

//   // For now we just check the set of processors
//   returnValue = uProcessors.getLength(0)==vProcessors.getLength(0) && max(abs(uProcessors-vProcessors))==0;
//   if( returnValue )
//   {
//     printF("hasSameDistribution: Internal_Partitioning_Type's are NOT the same but processors agree!\n");
//   }
  
//   return returnValue;

// }

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);

  int debug=0;
  bool plot=true;
  

  if( true )
  {
    realArray a,b;
    Partitioning_Type partition;
    partition.SpecifyProcessorRange(Range(0,0));
    int nd=2;
    partition.SpecifyDecompositionAxes(nd);

    a.partition(partition);
    a.redim(4,4);
    a=1.;
    displayDistribution(a,"a");

    b.partition(partition);
    b.redim(6,6);
    b=2.;
    displayDistribution(b,"b");
  
    partition.SpecifyProcessorRange(Range(np-1,np-1));
    partition.SpecifyDecompositionAxes(nd);

    displayDistribution(a,"a (2)");
    displayDistribution(b,"b (2)");

    a.partition(partition);
    a.redim(8,8);
    a=1.;
    displayDistribution(a,"a (3)");


    if( hasSameDistribution(a.getPartition(),b.getPartition()) )
    {
      printF("*** a and b have the same partition\n");
    }
    else
    {
      printF("*** a and b do NOT have the same partition\n");
    }
    fflush(0);
    Communication_Manager::Sync();

    realArray c; c.partition(partition);
    c.redim(5,5);
    if( hasSameDistribution(a.getPartition(),c.getPartition()) )
    {
      printF("*** a and c have the same partition\n");
    }
    else
    {
      printF("*** a and c do NOT have the same partition\n");
    }
    fflush(0);
    Communication_Manager::Sync(); 
    


    Partitioning_Type partition2;
    partition2.SpecifyProcessorRange(Range(np-1,np-1));
    partition2.SpecifyDecompositionAxes(nd);

    if( hasSameDistribution(partition,partition2) )
    {
      printF(" partition and partition2 have the same distribution\n");
    }
    else
    {
      printF(" partition and partition2 do NOT have the same distribution\n");
    }
    

    Overture::finish();          
    return 0;
  }
  

  aString answer,answer2;
  
  CompositeGrid cg;
  realCompositeGridFunction u;
  
//   aString nameOfOGFile="shapes2.order4.hdf";
   aString nameOfOGFile="sise.hdf";
//  aString nameOfOGFile="tcice.hdf";
  
//   bool loadBalance=true;
//   getFromADataBase(cg,nameOfOGFile,loadBalance);

  LoadBalancer loadBalancer;
  loadBalancer.setLoadBalancer(LoadBalancer::sequentialAssignment);
//  getFromADataBase(cg,nameOfOGFile,loadBalancer);

  printf("********** myid=%i : Read in the initial grid **************\n",myid);
  getFromADataBase(cg,nameOfOGFile);
  cg.update( MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );

  fflush(0);
  Communication_Manager::Sync();

  displayDistribution(cg,"cg (initial read)");

  fflush(0);
  Communication_Manager::Sync();
  

  // Load-balance this grid
  GridDistributionList & gridDistributionList = cg->gridDistributionList;

  // work-loads per grid are based on the number of grid points by default:
  loadBalancer.assignWorkLoads( cg,gridDistributionList );
  loadBalancer.determineLoadBalance( gridDistributionList );

  // now destroy the grid and read it in again (using the load balance computed above)
  if( true )
  {
    cg.destroy(CompositeGrid::EVERYTHING);  // this may be necessary
    printF("\n $$$$$$$$$$$ After cg.destroy() : cg.numberOfComponentGrids=%i $$$$$$$$$$$\n",
	   cg.numberOfComponentGrids());
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      cg[grid].rcData->partitionInitialized=false;
      cg[grid].rcData->matrixPartitionInitialized=false;
    }
  }
  else
  {
    cg.destroy(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      cg[grid].rcData->partitionInitialized=false;
      cg[grid].rcData->matrixPartitionInitialized=false;
    }
  }
  

  printf("********** myid=%i : Read in the LoadBalanced  grid **************\n",myid); 
  getFromADataBase(cg,nameOfOGFile);

  cg.update( MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );

  displayDistribution(cg,"cg (load balanced)");
  
  fflush(0);
  Communication_Manager::Sync();

  if( false )
  {
    Overture::finish();          
    return 0;
  }

  printF("\n ============ cga=cg ================\n\n");
  fflush(0);
  CompositeGrid cga;
  cga=cg;
  
  displayDistribution(cga,"cga (load balanced)");
  
  fflush(0);
  Communication_Manager::Sync();


  if( false )
  {
    Overture::finish();          
    return 0;
  }
  

  PlotStuff ps(plot,"tload");               // for plotting
  GraphicsParameters psp;


  if( plot )
  {
    ps.erase();
    PlotIt::plot(ps,cg,psp);
  }
  
  realCompositeGridFunction v(cg);
  
  if( true )
  { // test changing the partition of one grid
    int grid=1;

    // we must destroy the existing geometry before we can change the processors
    cg[grid].destroy(MappedGrid::EVERYTHING);
    v[grid].destroy();
    
    cg[grid].specifyProcesses(Range(0,0));
    cg[grid].update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );
    displayDistribution(cg,"cg (after re-partition)");
  }
  
  if( plot )
  {
    ps.erase();
    PlotIt::plot(ps,cg,psp);
  } 



  u.updateToMatchGrid(cg);

  OGTrigFunction trig;
  trig.assignGridFunction(u);
//  u=1.;
  
  if( plot )
  {
    ps.erase();
    PlotIt::contour(ps,u,psp);
  }
  

  CompositeGrid cg2;
  cg2=cg;

  displayDistribution(cg,"cg2 (=cg)");

  if( plot )
  {
    ps.erase();
    PlotIt::plot(ps,cg2,psp); 
  }
  
  realCompositeGridFunction u2(cg);
  u2=u;

  if( plot )
  {
    ps.erase();
    PlotIt::contour(ps,u2,psp); 
  }
  

  Overture::finish();          
  return 0;
}
