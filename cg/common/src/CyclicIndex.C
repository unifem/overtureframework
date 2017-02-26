#include "CyclicIndex.h"
#include <assert.h>

// // =================================================================
// /// \brief Overture modulus function: returns mod(a,b) with the 
// ///   result between 0 and b-1 (i.e. always positive unlike a % b)
// /// \details This function assumes b>0 
// // ================================================================
// int ovmod (int a, int b)
// {
//    int ret = a % b;
//    if(ret < 0)
//      ret+=b;
//    return ret;
// }


// ==============================================================================
/// \brief Define a cyclic index. This can can be used to index another vector
///     as a cyclic (circular) vector.
///
/// \param length (input) : length of the integer sequence, 0,1,2,...length-1
/// \param shift (input) : initial shift
/// \param increasing (input) : true if sequence is increasing: [0,1,2,3,...,n-1] ,
///                            false if the sequence is decreasing: [n-1,n-2,...,2,1,0].
///
/// \details Example use:
/// 
///     int n=5;
///     CylicIndex ic(n);  // define an index of length 5: [0,1,2,3,4]
///     ic[i] : mod(i+base,n) 
///     MyClass object[5];
///
///     object(ic[ 1]) = object(1) = next object
///     object(ic[ 0]) = object(0) = current object 
///     object(ic[-1]) = object(4) = previous object
///     object(ic[-2]) = object(3) = previous-previous object
///
///     ic.shift(1);  // shift base of sequence, now index is [1,2,3,4,0]
/// 
///     object(ic[ 1]) = object(2) = next object
///     object(ic[ 0]) = object(1) = current object 
///     object(ic[-1]) = object(0) = previous object
///     object(ic[-2]) = object(4) = previous-previous object
///
/// \param increasing (input) : 
///      increasing=true implies cycle values increase: [0,1,2,3,...,n-1]
///      increasing=false implies cycle values decrease: [n-1,n-2,...,2,1,0]
// ============================================================================


// ==============================================================================
/// \brief Construct a CyclicIndex object.
// ==============================================================================
CyclicIndex::CyclicIndex( int length, int shift /* =0 */, bool increasing /* =true */ )
{ 
  assert( length>0 );
  const int direction = increasing ? 1 : -1;
  
  n=length; 
  data=new int[n]; base=shift;
  for( int i=0; i<n; i++ ){ data[ovmod(i*direction,n)]=i; }
}

// ==============================================================================
/// \brief Copy constructor (deep copy)
// ==============================================================================
CyclicIndex::CyclicIndex(const CyclicIndex & x)
{
  n=0;
  base=0;
  data=0;
  *this=x;
}



// ==============================================================================
/// \brief Destroy a CyclicIndex object.
// ==============================================================================
CyclicIndex::~CyclicIndex()
{ 
  delete [] data;
} 


// ==============================================================================
/// \brief Equals operator
// ==============================================================================
CyclicIndex& CyclicIndex::operator=(const CyclicIndex & x)
{ 
  if( n!=x.n )
  {
    delete [] data;
    data = new int [x.n];
  }
  n=x.n;
  base=x.base;
  for( int i=0; i<n; i++ )
    data[i]=x.data[i];

  return *this;
} 

