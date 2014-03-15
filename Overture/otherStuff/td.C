#include "display.h"

//
// Test the display routines for A++ arrays
//

int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  floatArray x(3,3,3);
  x=1.;
  
  display(x,"x");
  display(x,"x (%6.1e )",NULL,"%6.1e ");    

  intArray a(3,2,3);
  a=123;
  a(1,Range(0,1),2)=-321;
  display(a,"a");
  display(a,"a",NULL,"%4i ");
  
  FILE *file = fopen("td.out","w");
  DisplayParameters dp;
  dp.set(file);
  dp.set(DisplayParameters::labelNoIndicies);
  fprintf(file,"%i %i %i %i\n",x.getBound(0)+1,x.getBound(1)+1,x.getBound(2)+1,x.getBound(3)+1);

  display(x,NULL,dp);

  intArray ig;
  Partitioning_Type partition;
  
  int numberOfDimensionsToPartition=1;
  partition.SpecifyDecompositionAxes(numberOfDimensionsToPartition);
  const int numGhost=0;  
  for( int kd=0; kd<numberOfDimensionsToPartition; kd++ )
    partition.partitionAlongAxis(kd, true, numGhost );
  for( int kd=numberOfDimensionsToPartition; kd<MAX_ARRAY_DIMENSION; kd++ )
    partition.partitionAlongAxis(kd, false, 0); 

  Internal_Partitioning_Type *ipt =partition.getInternalPartitioningObject();
  assert( ipt!=NULL );
  printF("partition = [%c,%c]\n",  ipt->Distribution_String[0],ipt->Distribution_String[1]);

  ig.partition(partition);
  ig.redim(5); ig=0;
  ig(1)=1; 
  ig(4)=4;

  display(ig,"ig","%3i");

  Overture::finish();          
  return 0;
}
