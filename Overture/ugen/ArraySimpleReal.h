#ifndef __OV_ArraySimpleReal_H__
#define __OV_ArraySimpleReal_H__

#define WAS_DETEMPLIFIED

#include "ArraySimpleCommon.h"
#include "VectorSimpleReal.h"
#ifndef WAS_DETEMPLIFIED
#include "ArraySimpleFixed.h"
#endif
// // //
/// simple multidimensional arrays
/** ArraySimpleReal provides a simple array class for dealing with small arrays ( ~10x10 or so)
 *  Range checking is available by defining OV_DEBUG. The constructor sets the rank of an
 *  array which becomes immutable until a copy constructor is called or the array is destroyed.
 *  operator(), overloaded for the various ranks available (up to 4),
 *  provides Fortran like indexing.  For arrays with a rank>1, it will be more efficient to
 *  compute the index inside the loop and use operator[] to access the element (no range
 *  checking is done, essentially the rank is ignored for []).  
 *  This approach provides the compilers with a more easily optimizable loop.
 */

class ArraySimpleReal {
public:

  /// default constructor
  /** create a blank array, suitable only for copying to
   */
  inline ArraySimpleReal() : data(0), nrank(0) 
  {
    for ( int r=0; r<MAXRANK; r++ ) n[r] = 0;
  }


  /// copy constructor
  inline ArraySimpleReal( const ArraySimpleReal &a ) : data(0), nrank(0)
  {
    nrank = a.nrank;
    for ( int r=0; r<MAXRANK; r++ ) n[r] = a.n[r];
    allocate();

    for ( int i=0; i<size(); i++ ) data[i] = a.data[i];
  }

  /// copy from a VectorSimpleReal
  inline ArraySimpleReal( const VectorSimpleReal &a ) : data(0), nrank(0)
  {
    destroy();
    nrank = 1;
    n[0] = a.size();
    for ( int r=1; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
    for ( int i=0; i<size(); i++ ) data[i] = a[i];
  }

#ifndef WAS_DETEMPLIFIED
#ifndef OV_NO_DEFAULT_TEMPL_ARGS
  /// copy from an ArraySimpleFixed
  
  inline ArraySimpleReal( const ArraySimpleFixed<real, d1,d2,d3,d4> &a ) : data(0), nrank(0)
  {

    n[0] = d1;
    n[1] = d2; 
    n[2] = d3;
    n[3] = d4;

    if ( n[1]==1 && n[2]==1 && n[3]==1 ) nrank = 1;
    else if ( n[2]==1 && n[3]==1 ) nrank = 2;
    else if ( n[3]==1 ) nrank = 3;
    else nrank = 4;

    allocate();
    for ( int s=0; s<size(); s++ )
      data[s] = a[s];

  }
#endif
#endif
  //
  // Basic constructors for the various rank arrays
  // The arguments are the sizes of each dimension
  //
  EXPLICIT ArraySimpleReal(int n1)
  {
    nrank = 1;
    n[0] = n1;
    for ( int r=1; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  }

  EXPLICIT ArraySimpleReal(int n1, int n2)
  {
    nrank = 2;
    n[0] = n1;
    n[1] = n2;
    for ( int r=2; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  }  

  EXPLICIT ArraySimpleReal(int n1, int n2, int n3)
  {
    nrank = 3;
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    for ( int r=3; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  } 

  EXPLICIT ArraySimpleReal(int n1, int n2, int n3, int n4)
  {
    nrank = 4;
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
    for ( int r=4; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  } 
  
  // destructor
  inline ~ArraySimpleReal() { destroy(); }
  
  // size
  inline int size() const 
  {
    // /Description : return the total size of the array
    int s = 1;
    for ( int r=0; r<nrank; r++ )
      s *= n[r];

    return nrank==0 ? 0 : s;
  }

  // size (int)
  inline int size( const int &r ) const 
  {
    // /Description : return the size of one dimension
    assert(RANGE_CHK(r>=0 && r<nrank));
    
    return n[r];
  }

  // rank
  inline int rank() const
  {
    // /Description : return the rank of the array
    return nrank; 
  }

  // ptr
  inline real * ptr() 
  {
    // /Description : returns the data pointer
    return data; 
  }

  inline ArraySimpleReal & operator= ( const ArraySimpleReal &a )
  {
    // /Description : perform a deep copy
    if ( size() != a.size() )
      { 
	destroy();	
	nrank = a.nrank;
	for ( int r=0; r<MAXRANK; r++ ) n[r] = a.n[r];
	allocate();
      }

    for ( int i=0; i<size(); i++ ) data[i] = a.data[i];
    return *this;
  }

  inline ArraySimpleReal & operator= ( const real &a )
  {
    // /Description : set all array values equal to a const value
    for ( int i=0; i<size(); i++ ) data[i] = a;
    return *this;
  }

  // operator[]
  inline real & operator[] (const int &i)
  { 
    // /Description : indexes the data using 1d indexing (no range checks along each axis)
    assert(RANGE_CHK( i>=0 && i<size() ) );
    return data[i];
  }

  // operator[]
  inline const real & operator[] (const int &i) const
  { 
    // /Description : indexes the data using 1d indexing (no range checks along each axis)
    assert(RANGE_CHK( i>=0 && i<size() ) );
    return data[i];
  }

  //
  // index operators
  // for each rank, an index operator is specified. Range checking is performed if
  // OV_DEBUG is defined (using RANGE_CHK)
  //
  inline real & operator() (const int &i0) 
  {
    assert(RANGE_CHK( nrank==1 && i0>-1 && i0<n[0] )); 
    return data[ i0 ];
  }

  inline const real & operator() (const int &i0) const
  {
    assert(RANGE_CHK( nrank==1 && i0>-1 && i0<n[0] )); 
    return data[ i0 ];
  }

  inline real & operator() (const int &i0, const int &i1) 
  {
    assert(RANGE_CHK( nrank==2 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] ));
    return data[ i0 + i1*n[0] ];
  }

  inline const real & operator() (const int &i0, const int &i1) const
  {
    assert(RANGE_CHK( nrank==2 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] ));
    return data[ i0 + i1*n[0] ];
  }

  inline real & operator() (int i0, int i1, int i2) 
  { 
    assert(RANGE_CHK( nrank==3 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] ));
    return data[ i0 + i1*n[0] + i2*n[0]*n[1] ];

  }

  inline const real & operator() (int i0, int i1, int i2) const
  { 
    assert(RANGE_CHK( nrank==3 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] ));
    return data[ i0 + i1*n[0] + i2*n[0]*n[1] ];

  }

  inline real & operator() (int i0, int i1, int i2, int i3) 
  {
    assert(RANGE_CHK( nrank==4 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] &&
		      i3>-1 && i2<n[3] ));
    return data[ i0 + i1*n[0] + i2*n[0]*n[1] + i3*n[0]*n[1]*n[3] ];

  }

  inline const real & operator() (int i0, int i1, int i2, int i3) const
  {
    assert(RANGE_CHK( nrank==4 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] &&
		      i3>-1 && i2<n[3] ));
    return data[ i0 + i1*n[0] + i2*n[0]*n[1] + i3*n[0]*n[1]*n[3] ];

  }

protected:

  inline void allocate() 
  { 
    int s = size();

    assert(RANGE_CHK(s>0));

    data = new real[s];
    assert(data != 0);
  }

  inline void destroy()
  {
    if ( data!=0 ) delete [] data; 
    for ( int r=0; r<MAXRANK; r++ ) n[r] = 0;
    nrank = 0;
  }

private:
  real *data;
  int n[MAXRANK];
  short nrank;

};
//
// end of ArraySimpleReal
// // //

#ifndef WAS_DETEMPLIFIED
typedef ArraySimpleReal intArraySimple;
typedef ArraySimpleReal floatArraySimple;
typedef ArraySimpleReal doubleArraySimple;
#endif


ostream & operator<< (ostream &os, ArraySimpleReal &sa)
{
  // /Description : overloading for ostream to make printing ArraySimples easier

  int rank = sa.rank();

  os<<"********** ArraySimpleReal( ";

  for ( int r=0; r<rank-1; r++ )
    os<<sa.size(r)<<", ";
  if ( rank!=0 )
    os<<sa.size(rank-1);
  else
    os<<"UNINITIALIZED";

  os<<" ) **********\n";

  if ( rank != 0 ) 
    {
      switch (rank) 
	{
	case 1:
	  {
	    for ( int i=0; i<sa.size(); i++ )
	      os<<"a( "<<i<<" ) = "<<sa(i)<<endl;
	    break;
	  }
	case 2:
	  {
	    for ( int i1=0; i1<sa.size(0); i1++ )
	      for ( int i2=0; i2<sa.size(1); i2++ )
		os<<"a( "<<i1<<", "<<i2<<" ) = "<<sa(i1,i2)<<endl;
	    break;
	  }
	case 3:
	  {
	    for ( int i1=0; i1<sa.size(0); i1++ )
	      for ( int i2=0; i2<sa.size(1); i2++ )
		for ( int i3=0; i3<sa.size(2); i3++ )
		  os<<"a( "<<i1<<", "<<i2<<", "<<i3<<" ) = "<<sa(i1,i2,i3)<<endl;
	    break;
	  }
	case 4:
	  {
	    for ( int i1=0; i1<sa.size(0); i1++ )
	      for ( int i2=0; i2<sa.size(1); i2++ )
		for ( int i3=0; i3<sa.size(2); i3++ )
		  for ( int i4=0; i4<sa.size(3); i4++ )
		    os<<"a( "<<i1<<", "<<i2<<", "<<i3<<", "<<i4<<" ) = "<<sa(i1,i2,i3,i4)<<endl;
	    break;
	  }
	default:
	  {
	    os<<"RANK "<<rank<<" ARRAYS ARE NOT SUPPORTED"<<endl;
	  }
	}
    }
      
  return os;
}

#endif
