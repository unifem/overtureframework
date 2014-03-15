#include "display.h"
#include "ParallelUtility.h"
#include "MappedGridOperators.h"
#include "SparseRep.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
    int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
    int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
    for(i3=I3Base; i3<=I3Bound; i3++) \
    for(i2=I2Base; i2<=I2Bound; i2++) \
    for(i1=I1Base; i1<=I1Bound; i1++)

// ===================================================================================================================
/// \brief Output the equations contained in a coefficient matrix
/// \param coeff: output the equations for this matrix.
/// \param label: write this label as the header. 
/// \param file: write results to this file. In parallel, only the local array is written to this file and thus there
///              should be a different file for each processor. 
/// \param format: output format
///
// ===================================================================================================================
int
displayCoeff(realMappedGridFunction &coeff,
	     const aString & label,
	     FILE *file /* =stdout */,
	     const aString format /* =nullString */ )
{
  MappedGrid & mg = *coeff.getMappedGrid();
  const int numberOfDimensions = mg.numberOfDimensions();

  assert( coeff.sparse!=NULL );
  SparseRepForMGF & sparse = *coeff.sparse;
  int numberOfComponentsForCoefficients = sparse.numberOfComponents;  // size of the system of equations
  int numberOfGhostLines = sparse.numberOfGhostLines;
  int stencilSize = sparse.stencilSize;
  int stencilDim=stencilSize*numberOfComponentsForCoefficients; // number of coefficients per equation

  if( label.length()>0 )
    fPrintF(file,"%s\n",(const char*)label);

#ifdef USE_PPP
  // For distributed arrays we copy the array to a serial array on processor zero and then print
  // (this code taken from display.C, with minor changes)
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int processorForDisplay=0;
  Partitioning_Type partition; 
  partition.SpecifyProcessorRange(Range(processorForDisplay,processorForDisplay));
  realArray & x_ = coeff;
  if( x_.getInternalPartitionPointer()!=NULL )
  {
    Partitioning_Type xPartition=x_.getPartition();
    for( int axis=0; axis<MAX_ARRAY_DIMENSION; axis++ )
    {
      int ghost=xPartition.getGhostBoundaryWidth(axis);
      if( ghost>0 )
	partition.partitionAlongAxis(axis, true , ghost);
      else
	partition.partitionAlongAxis(axis, false, 0);
    }
  }

  realArray xd;  xd.partition(partition); 
  xd.redim(x_.dimension(0),x_.dimension(1),x_.dimension(2),
           x_.dimension(3),x_.dimension(4),x_.dimension(5));
  const int nd=4;
  Index D[4]={x_.dimension(0),x_.dimension(1),x_.dimension(2),x_.dimension(3)}; // 
  Index S[4]={x_.dimension(0),x_.dimension(1),x_.dimension(2),x_.dimension(3)}; // 
  CopyArray::copyArray( xd, D, x_, S, nd );
  
  const realSerialArray & coeffLocal= xd.getLocalArray(); 
  // realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff,coeffLocal);

  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);

  if( myid!=processorForDisplay ) return 0;
#else
  realSerialArray & coeffLocal = coeff;
  intSerialArray & maskLocal = mg.mask();
#endif


  //    mce2(m1,m2,m3,c,e) = ma2(m1,m2,m3) + stencilSize*(c+ncc*e)
  //    mce3(m1,m2,m3,c,e) = ma3(m1,m2,m3) + stencilSize*(c+ncc*e)
  #define ce(c,e) stencilSize*((c)+numberOfComponentsForCoefficients*(e))

  Index I1,I2,I3;
  getIndex(mg.gridIndexRange(),I1,I2,I3,numberOfGhostLines);

  aString fmt="%9.2e ";
  if( format !="" )
    fmt=format;

  int i1,i2,i3;
  FOR_3D(i1,i2,i3,I1,I2,I3)
  {
    for( int e=0; e<numberOfComponentsForCoefficients; e++ )
    {
      for( int c=0; c<numberOfComponentsForCoefficients; c++ )
      {
	if( numberOfDimensions==2 )
	{
	  if( numberOfComponentsForCoefficients==1 )
	  {
	    fPrintF(file," (%i,%i) coeff=",i1,i2);
	  }
	  else if( c==0 )
	  {
	    fPrintF(file," (%i,%i) e=%i c=%i coeff=",i1,i2,e,c);
	  }
	  else
	    fPrintF(file,"                 c=%i coeff=",c);
	}
	else
	{
	  if( numberOfComponentsForCoefficients==1 )
	  {
	    fPrintF(file," (%i,%i,%i) coeff=",i1,i2,i3,e,c);
	  }
	  else if( c==0 )
	  {
	    fPrintF(file," (%i,%i,%i) e=%i c=%i coeff=",i1,i2,i3,e,c);
	  }
	  else
	    fPrintF(file,"                     c=%i coeff=",c);
	}

	for( int m=ce(c,e); m<ce(c+1,e); m++ )
	{
	  fPrintF(file,(const char*)fmt,coeffLocal(m,i1,i2,i3));
	}
	fPrintF(file,"\n");
      }
    }
  }

  return 0;
}
