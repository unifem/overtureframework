#include "Overture.h"

const int printArrayDefaultValue=INT_MAX;

//\begin{>>otherStuffInclude.tex}{\subsection{printing out arrays, gridFunctions and Strings from dbx}}
////\no function header:
//  If you put these next lines in your .dbxrc file:
//   \index{debugging!printing A++ arrays}\index{debugging!printing grid functions}\index{debugging!printing Strings}
// \begin{verbatim}
//     dalias pa 'call printArray';
//     dalias pv 'call printArrayValue';
//     dalias ps 'call printString';
// \end{verbatim}
// then inside dbx you can say, assuming 
// \begin{verbatim}
//    realArray u(...)
//    realMappedGridFunction v(...)
//    realGridCollectionFunction w(...) or realCompositeGridFunction w(...)
//    aString s="hello world"; 
// \end{verbatim}
// \begin{verbatim}
//    (dbx) pa(u)                : print the entire array u
//    (dbx) pa(u,2,3,4,5)        : print u(2:3,4:5)
//    (dbx) pv(u)                : print the first value in u
//    (dbx) pv(u,2,3)            : print u(2,3)     
//    (dbx) pa(w,0)              : print w[0] -- all values on grid 0
//    (dbx) pa(w,1,2,3,4,5)      : print w[1](2:3,4:5,all) -- a range of values
//    (dbx) pv(w,1,2,3,0)          : print w[1](2,3,0) -- a single value
//    (dbx) ps(s)                : print the aString s.  
// \end{verbatim}
//   
//\end{otherStuffInclude.tex}

//\begin{>>otherStuffInclude.tex}{\subsection{printArray}}
// void  
// printArray(doubleArray & u,   
//            int i1a=printArrayDefaultValue, int i1b=printArrayDefaultValue, 
//            int i2a=printArrayDefaultValue, int i2b=printArrayDefaultValue, 
//            int i3a=printArrayDefaultValue, int i3b=printArrayDefaultValue, 
//            int i4a=printArrayDefaultValue, int i4b=printArrayDefaultValue, 
//            int i5a=printArrayDefaultValue, int i5b=printArrayDefaultValue, 
//            int i6a=printArrayDefaultValue, int i6b=printArrayDefaultValue );
// =======================================================================================================
// /Description:
//   Print array values. Set unspecified args to be the entire range of values.
//
//\end{otherStuffInclude.tex}
// =======================================================================================================

//\begin{>>otherStuffInclude.tex}{\subsection{printArrayValue}}
// void  
// printArrayValue(doubleArray & u,   
// 		int i1a=printArrayDefaultValue, 
// 		int i2a=printArrayDefaultValue, 
// 		int i3a=printArrayDefaultValue, 
// 		int i4a=printArrayDefaultValue, 
// 		int i5a=printArrayDefaultValue, 
// 		int i6a=printArrayDefaultValue);
// ======================================================================================================= 
// /Description: 
//   Print a value. Set unspecified args to the base value of the array. 
//  
//\end{otherStuffInclude.tex}
// ======================================================================================================= 

//\begin{>>otherStuffInclude.tex}{\subsection{printArrayValue(GridCollectionFunction,CompositeGridFunction)}}
// void  
// printArrayValue(doubleGridCollectionFunction & u,   
//              int grid,
// 		int i1a=printArrayDefaultValue, 
// 		int i2a=printArrayDefaultValue, 
// 		int i3a=printArrayDefaultValue, 
// 		int i4a=printArrayDefaultValue, 
// 		int i5a=printArrayDefaultValue, 
// 		int i6a=printArrayDefaultValue);
// ======================================================================================================= 
// /Description: 
//   Print a value. Set unspecified args to the base value of the array. 
//  
//\end{otherStuffInclude.tex}
// ======================================================================================================= 


#undef PRINT_ARRAY
#define PRINT_ARRAY(doubleArray,format) \
void  \
printArray(const doubleArray & u,   \
           int i1a=printArrayDefaultValue, int i1b=printArrayDefaultValue,   \
           int i2a=printArrayDefaultValue, int i2b=printArrayDefaultValue,   \
           int i3a=printArrayDefaultValue, int i3b=printArrayDefaultValue,   \
           int i4a=printArrayDefaultValue, int i4b=printArrayDefaultValue,   \
           int i5a=printArrayDefaultValue, int i5b=printArrayDefaultValue,   \
           int i6a=printArrayDefaultValue, int i6b=printArrayDefaultValue )  \
{  \
  \
  const int numberOfDimensions=6;  \
  int iva[numberOfDimensions]= {i1a,i2a,i3a,i4a,i5a,i6a};   \
  int ivb[numberOfDimensions]= {i1b,i2b,i3b,i4b,i5b,i6b};   \
  \
  for( int axis=0; axis<numberOfDimensions; axis++ )  \
  {  \
    iva[axis]= iva[axis]==printArrayDefaultValue ? u.getBase(axis)  :   \
      max(u.getBase(axis),min(u.getBound(axis),iva[axis]));  \
    ivb[axis]= ivb[axis]==printArrayDefaultValue ? u.getBound(axis) :   \
      max(u.getBase(axis),min(u.getBound(axis),ivb[axis]));  \
  \
  }  \
  \
  for( int i5=iva[5]; i5<=ivb[5]; i5++ )  \
  for( int i4=iva[4]; i4<=ivb[4]; i4++ )  \
  for( int i3=iva[3]; i3<=ivb[3]; i3++ )  \
  for( int i2=iva[2]; i2<=ivb[2]; i2++ )  \
  for( int i1=iva[1]; i1<=ivb[1]; i1++ )  \
  for( int i0=iva[0]; i0<=ivb[0]; i0++ )  \
  {  \
    if( u.numberOfDimensions()==1 )  \
      printf("u(%i)=" #format "\n",i0,u(i0,i1,i2,i3,i4,i5));  \
    else if( u.numberOfDimensions()==2 )  \
      printf("u(%i,%i)=" #format "\n",i0,i1,u(i0,i1,i2,i3,i4,i5));  \
    else if( u.numberOfDimensions()==3 )  \
      printf("u(%i,%i,%i)=" #format "\n",i0,i1,i2,u(i0,i1,i2,i3,i4,i5));  \
    else if( u.numberOfDimensions()==4 )  \
      printf("u(%i,%i,%i,%i)=" #format "\n",i0,i1,i2,i3,u(i0,i1,i2,i3,i4,i5));  \
    else if( u.numberOfDimensions()==5 )  \
      printf("u(%i,%i,%i,%i,%i)=" #format "\n",i0,i1,i2,i3,i4,u(i0,i1,i2,i3,i4,i5));  \
    else   \
      printf("u(%i,%i,%i,%i,%i,%i)=" #format "\n",i0,i1,i2,i3,i4,i5,u(i0,i1,i2,i3,i4,i5));  \
  \
  } \
}  \
  \
void  \
printArrayValue(const doubleArray & u,   \
		int i1a=printArrayDefaultValue,   \
		int i2a=printArrayDefaultValue,   \
		int i3a=printArrayDefaultValue,   \
		int i4a=printArrayDefaultValue,   \
		int i5a=printArrayDefaultValue,   \
		int i6a=printArrayDefaultValue )  \
{  \
  const int numberOfDimensions=6;  \
  int iva[numberOfDimensions]= {i1a,i2a,i3a,i4a,i5a,i6a};   \
  \
  for( int axis=0; axis<numberOfDimensions; axis++ )  \
  {  \
    iva[axis]= iva[axis]==printArrayDefaultValue ? u.getBase(axis)  :   \
      max(u.getBase(axis),min(u.getBound(axis),iva[axis]));  \
  \
  }  \
  printArray(u,iva[0],iva[0],iva[1],iva[1],iva[2],iva[2],iva[3],iva[3],iva[4],iva[4],iva[5],iva[5]);  \
}  \


PRINT_ARRAY(intArray,%i)
PRINT_ARRAY(floatArray,%e)
PRINT_ARRAY(doubleArray,%e)

#ifdef USE_PPP

PRINT_ARRAY(intSerialArray,%i)
PRINT_ARRAY(floatSerialArray,%e)
PRINT_ARRAY(doubleSerialArray,%e)

#endif

#undef PRINT_ARRAY

#define PRINT_GF(doubleArray,doubleMappedGridFunction,doubleGridCollectionFunction,doubleCompositeGridFunction,format) \
void  \
printArray(const doubleMappedGridFunction & u,   \
           int i1a=printArrayDefaultValue, int i1b=printArrayDefaultValue,   \
           int i2a=printArrayDefaultValue, int i2b=printArrayDefaultValue,   \
           int i3a=printArrayDefaultValue, int i3b=printArrayDefaultValue,   \
           int i4a=printArrayDefaultValue, int i4b=printArrayDefaultValue,   \
           int i5a=printArrayDefaultValue, int i5b=printArrayDefaultValue,   \
           int i6a=printArrayDefaultValue, int i6b=printArrayDefaultValue )  \
{  \
  printArray((const doubleArray&)u,i1a,i1b,i2a,i2b,i3a,i3b,i4a,i4b,i5a,i5b,i6a,i6b );  \
}  \
  \
void  \
printArrayValue(const doubleMappedGridFunction & u,   \
		int i1a=printArrayDefaultValue,   \
		int i2a=printArrayDefaultValue,   \
		int i3a=printArrayDefaultValue,   \
		int i4a=printArrayDefaultValue,   \
		int i5a=printArrayDefaultValue,   \
		int i6a=printArrayDefaultValue )  \
{  \
  printArrayValue((const doubleArray&)u,i1a,i2a,i3a,i4a,i5a,i6a);  \
}  \
void  \
printArray(const doubleGridCollectionFunction & u, int grid,   \
           int i1a=printArrayDefaultValue, int i1b=printArrayDefaultValue,   \
           int i2a=printArrayDefaultValue, int i2b=printArrayDefaultValue,   \
           int i3a=printArrayDefaultValue, int i3b=printArrayDefaultValue,   \
           int i4a=printArrayDefaultValue, int i4b=printArrayDefaultValue,   \
           int i5a=printArrayDefaultValue, int i5b=printArrayDefaultValue,   \
           int i6a=printArrayDefaultValue, int i6b=printArrayDefaultValue )  \
{  \
  printArray((doubleArray&)u[grid],i1a,i1b,i2a,i2b,i3a,i3b,i4a,i4b,i5a,i5b,i6a,i6b );  \
}  \
  \
void  \
printArrayValue(const doubleGridCollectionFunction & u, int grid,   \
		int i1a=printArrayDefaultValue,   \
		int i2a=printArrayDefaultValue,   \
		int i3a=printArrayDefaultValue,   \
		int i4a=printArrayDefaultValue,   \
		int i5a=printArrayDefaultValue,   \
		int i6a=printArrayDefaultValue )  \
{  \
  printArrayValue((const doubleArray&)u[grid],i1a,i2a,i3a,i4a,i5a,i6a);  \
}  \
void  \
printArray(const doubleCompositeGridFunction & u, int grid,   \
           int i1a=printArrayDefaultValue, int i1b=printArrayDefaultValue,   \
           int i2a=printArrayDefaultValue, int i2b=printArrayDefaultValue,   \
           int i3a=printArrayDefaultValue, int i3b=printArrayDefaultValue,   \
           int i4a=printArrayDefaultValue, int i4b=printArrayDefaultValue,   \
           int i5a=printArrayDefaultValue, int i5b=printArrayDefaultValue,   \
           int i6a=printArrayDefaultValue, int i6b=printArrayDefaultValue )  \
{  \
  printArray((const doubleArray&)u[grid],i1a,i1b,i2a,i2b,i3a,i3b,i4a,i4b,i5a,i5b,i6a,i6b );  \
}  \
  \
void  \
printArrayValue(const doubleCompositeGridFunction & u, int grid,   \
		int i1a=printArrayDefaultValue,   \
		int i2a=printArrayDefaultValue,   \
		int i3a=printArrayDefaultValue,   \
		int i4a=printArrayDefaultValue,   \
		int i5a=printArrayDefaultValue,   \
		int i6a=printArrayDefaultValue )  \
{  \
  printArrayValue((const doubleArray&)u[grid],i1a,i2a,i3a,i4a,i5a,i6a);  \
}  


PRINT_GF(intArray,intMappedGridFunction,intGridCollectionFunction,intCompositeGridFunction,%i)
PRINT_GF(floatArray,floatMappedGridFunction,floatGridCollectionFunction,floatCompositeGridFunction,%e)
PRINT_GF(doubleArray,doubleMappedGridFunction,doubleGridCollectionFunction,doubleCompositeGridFunction,%e)

#undef PRINT_GF


void
printString( aString & s )
{
  cout << "aString=[" << s << "]\n";
}
