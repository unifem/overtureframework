// ******* the file display.C is processed by display.p to create the files 
// ******* intDisplay.C, floatDisplay.C and doubleDisplay.C

// The next line may be turned on by the preprocessor display.p 
#ifdef USE_PPP
#include "display.h"
#include <string.h>
#include "mathutil.h"
#include "ParallelUtility.h"

// forward declaration: 
int
display( const floatSerialArray & x, 
	 const char *label,
	 FILE *file,
	 const char *format_ ,
         const DisplayParameters *displayParameters, 
         const Index *Iv );


//\begin{>displayInclude.tex}{\subsection{display: display an A++ array}}
int
display( const floatSerialArray & x, const char *label, const char *format_, const Index *Iv /* =NULL*/ )
// =============================================================================
//   /Description:
//      Another version of display -- pass a format but no FILE
// /x (input) : array to display. There are also versions of this routine for int and double arrays.
// /label (input): optional header label
// /format (input) : an optional format such as "\%6.1e " or "\%11.4e " (note blank at end) 
//  that will be used to display each element in the array. The default is "\%11.4e "
// /Iv[d] : If Iv is not NULL then print the values x(Iv[0],Iv[1],Iv[2],...) You must supply at
//   least nd entries in the array Iv[d] where nd=x.numberOfDimensions();
//\end{displayInclude.tex}
// =============================================================================
{
  return display(x,label,NULL,format_,NULL,Iv);
}

//\begin{>>displayInclude.tex}{\subsection{display: save an A++ array in a file}}
int
display( const floatSerialArray & x, 
	 const char *label   /* = NULL */, 
	 FILE *file          /* = NULL */, 
	 const char *format_ /* = NULL */, 
         const Index *Iv /* =NULL*/ )
// =======================================================================================
// /Description:
//    Display an A++ array
// /x (input) : array to display. There are also versions of this routine for int and double arrays.
// /label (input): optional header label
// /file (input) : optionally supply a file to print to.
// /format (input) : an optional format such as "\%6.1e " or "\%11.4e " (note blank at end) 
//  that will be used to display each element in the array. The default is "\%11.4e "
// /Iv[d] : If Iv is not NULL then print the values x(Iv[0],Iv[1],Iv[2],...)
//\end{displayInclude.tex}
// =======================================================================================
{
  return display(x,label,file,format_,NULL,Iv);
}


//\begin{>>displayInclude.tex}{\subsection{display an A++ array with DisplayParameters}}
int
display( const floatSerialArray & x, const char *label, const DisplayParameters & displayParameters, 
         const Index *Iv /* =NULL*/ )
// =============================================================================
//   /Description:
//      Another version of display -- pass a format but no FILE
// /x (input) : array to display. There are also versions of this routine for int and double arrays.
// /label (input): optional header label
// /format (input) : an optional format such as "\%6.1e " or "\%11.4e " (note blank at end) 
//  that will be used to display each element in the array. The default is "\%11.4e "
// /displayParameters (input) : provide parameters for display. 
// /Iv[d] : If Iv is not NULL then print the values x(Iv[0],Iv[1],Iv[2],...)
//\end{displayInclude.tex}
// =============================================================================
{
  return display(x,label,NULL,NULL,&displayParameters,Iv);
}


int
display( const floatSerialArray & x_, 
	 const char *label,
	 FILE *file,
	 const char *format_ ,
         const DisplayParameters *displayParameters /* = NULL */, 
         const Index *Iv /* =NULL*/ )
// =======================================================================================
// /Description:
//    **** THIS routine is private and should not be called other than by the above functions**** 
//
// /label (input): optional header label
// /file (input) : optionally supply a file to print to.
// /format (input) : an optional format such as "\%6.1e " or "\%11.4e " (note blank at end) 
//  that will be used to display each element in the array. The default is "\%11.4e "
// /Iv[d] : If Iv is not NULL then print the values x(Iv[0],Iv[1],Iv[2],...)
// =======================================================================================
{
  // For distributed arrays we copy the array to a serial array on processor zero and then print
  const int processorForDisplay=0;

#ifndef USE_PPP
  const floatSerialArray & x = x_;
#else
#undef PARALLEL_DISPLAY
#ifndef PARALLEL_DISPLAY
  const floatSerialArray & x = x_;
#else

  Partitioning_Type partition; 
  partition.SpecifyProcessorRange(Range(processorForDisplay,processorForDisplay));

  if( x_.getInternalPartitionPointer()!=NULL )
  {
    Partitioning_Type xPartition=x_.getPartition();
    Internal_Partitioning_Type *iPartition =partition.getInternalPartitioningObject();
    assert( iPartition!=NULL );
    for( int axis=0; axis<MAX_ARRAY_DIMENSION; axis++ )
    {
      int ghost=xPartition.getGhostBoundaryWidth(axis);
      // if( ghost>0 )
      // *wdh* new way 2013/08/31
      if( iPartition->Distribution_String[axis]=='B' ) // this axis is distributed
	partition.partitionAlongAxis(axis, true , ghost);
      else
	partition.partitionAlongAxis(axis, false, 0);
    }
  }
  
  floatSerialArray xd;  xd.partition(partition); 
  Range R0=x_.dimension(0);
  Range R1=x_.dimension(1);
  Range R2=x_.dimension(2);
  Range R1a=R1;
  if( false && R1.length()==1 ) // whay was this here ?? *wdh* 2013/08/31 
    R1a=Range(R1.getBase(),R1.getBound()+1);
  xd.redim(x_.dimension(0),R1a,x_.dimension(2),
           x_.dimension(3),x_.dimension(4),x_.dimension(5));
  Range all;
  if( false )
  {
    xd(all,R1,all,all)=x_;
  }
  else
  {
    const int nd=4;
    Index D[4]={x_.dimension(0),x_.dimension(1),x_.dimension(2),x_.dimension(3)}; // 
    Index S[4]={x_.dimension(0),x_.dimension(1),x_.dimension(2),x_.dimension(3)}; // 
    CopyArray::copyArray( xd, D, x_, S, nd );
  }
  
  const floatSerialArray & x = xd.getLocalArray(); // WithGhostBoundaries();

#endif
#endif


#ifdef PARALLEL_DISPLAY
  Communication_Manager::Sync(); // *wdh* 060202 -- only sync parallel arrays!
  if( Communication_Manager::My_Process_Number==processorForDisplay )
#endif
  {

    DisplayParameters defaultDisplayParameters;
    const DisplayParameters & dp = displayParameters==NULL ? defaultDisplayParameters : *displayParameters;
    const int & ordering = dp.ordering;

    FILE *f = file!=NULL ? file : dp.file; // dp.file defaults to stdout

    if( label!=NULL && strlen(label)>0 )
      fprintf(f,"%s\n",label);
  
    if( x.getDataPointer()==0 )
      return 0;
  
// dp.fFormat = dp.iFormat or dp.fFormat or dp.dformat
    const char *format = (format_!=NULL && strlen(format_)>0) ? format_ : (const char *)dp.fFormat;
    // determine the width of the format
    int width;
    const int buffSize=250; // Sometimes %5.2f format for e.g. can produce a result with MANY numbers (?)
    char buff[buffSize];
    for( int k=0; k<buffSize; k++ ) buff[k]=0;
    sprintf(buff,format,x(x.getBase(0),x.getBase(1),x.getBase(2),x.getBase(3)));
    width=strlen(buff);
    if( width<=0 || width>15 )
      width=12;

    char labelFormat[20];
    sprintf(labelFormat,"(%%%ii) ",width-3);
    // printf("labelFormat=[%s] \n",labelFormat);

    int nd=x.numberOfDimensions();
    int numberOfDigits=int( log10(x.getBound(1)-x.getBase(1)+1.)+1 );
    numberOfDigits+= x.getBase(1)<0 ? 1 : 0;   // add one for negative sign
  
    char leftLabel[20];
    sprintf(leftLabel,"(%%%ii) ",numberOfDigits);
    // printf("leftLabel=[%s] \n",leftLabel);
    char spaces[25] ="                        ";
    spaces[numberOfDigits+3]=0;
    // printf("spaces=[%s] \n",spaces);

    int base[MAX_ARRAY_DIMENSION], bound[MAX_ARRAY_DIMENSION]; //  order[MAX_ARRAY_DIMENSION];
    for( int i=0; i<MAX_ARRAY_DIMENSION; i++ )
    {
      if( Iv==NULL || i>=nd )
      {
	base[i]  = dp.stride[i] > 0 ? x.getBase(i)  : x.getBound(i);
	bound[i] = dp.stride[i] > 0 ? x.getBound(i) : x.getBase(i);
      }
      else
      { // get bounds from Iv[i]
	base[i]  = dp.stride[i] > 0 ? Iv[i].getBase()  : Iv[i].getBound();
	bound[i] = dp.stride[i] > 0 ? Iv[i].getBound() : Iv[i].getBase();
      }
      
      // order[i] = ordering ? i : 7-i;     // this is used to reverse the output ordering
    }

    const int & s0 = dp.stride[0];
    const int & s1 = dp.stride[1];
    const int & s2 = dp.stride[2];
    const int & s3 = dp.stride[3];
    const int & s4 = dp.stride[4];
    const int & s5 = dp.stride[5];

    for( int i5=base[5]; i5<=bound[5]; i5+=s5 )
    {
      if( nd>5 && dp.indexLabel[5] )
	fprintf(f,"===================axis 5 = %i======================== \n",i5);
      for( int i4=base[4]; i4<=bound[4]; i4+=s4 )
      {
	if( nd>4 && dp.indexLabel[4] )
	  fprintf(f,"===================axis 4 = %i======================== \n",i4);
	for( int i3=base[3]; i3<=bound[3]; i3+=s3 )
	{
	  if( nd>3 && dp.indexLabel[3] )
	    fprintf(f,"===================axis 3 = %i======================== \n",i3);
	  for( int i2=base[2]; i2<=bound[2]; i2+=s2 )
	  {
	    if( nd>2  && dp.indexLabel[2] )
	      fprintf(f,"-------------------axis 2 = %i------------------------ \n",i2);

	    if( dp.indexLabel[0] )
	    {
	      fprintf(f,"%s",spaces);
	      for( int i0=base[0]; i0<=bound[0]; i0+=s0 )
		fprintf(f,labelFormat,i0);
	      fprintf(f,"\n");
	    }
	    for( int i1=base[1]; i1<=bound[1]; i1+=s1 )
	    {
	      if( dp.indexLabel[1] )
		fprintf(f,leftLabel,i1);
	      for( int i0=base[0]; i0<=bound[0]; i0+=s0 )
	      {
                #ifdef USE_PPP
		  fprintf(f,format,x(i0,i1,i2,i3));
                #else
	  	  fprintf(f,format,x(i0,i1,i2,i3,i4,i5));  // **** only 6 there ?
                #endif
	      }
	      fprintf(f,"\n");
	    }
	  }
	}
	  
      }
    }
  }

#ifdef PARALLEL_DISPLAY
  Communication_Manager::Sync(); // *wdh* 060202 -- only sync parallel arrays!
#endif

  return 0;
}

#endif
