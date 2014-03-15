#ifndef __OV_ArraySimple_H__
#define __OV_ArraySimple_H__

#define WAS_NOT_DETEMPLIFIED

// include overturetypes to make sure we get bool!
#include "OvertureTypes.h"

#include "ArraySimpleCommon.h"
//#include "VectorSimple.h"
#ifndef WAS_DETEMPLIFIED
#include "ArraySimpleFixed.h"
#endif

// // //
/// Reference counting/copying info for arrays
class AS_ReferenceCounting {

public:
  AS_ReferenceCounting( bool startCounting=TRUE ) : r(NULL) 
  { 

#ifdef TEST_AS_REFCOUNTING
    cout<<"building referenced object at "<<long(this)<<endl;
#endif

    if (startCounting) increment(); 
  }

  AS_ReferenceCounting( const AS_ReferenceCounting &ref ) : r(NULL)
  { 
#ifdef TEST_AS_REFCOUNTING
    cout<<"copy constructor called for object at "<<long(this)<<endl;
#endif

    *this=ref;
  }

  ~AS_ReferenceCounting() {
    decrement();
#ifdef TEST_AS_REFCOUNTING
    cout<<"destructor called for object at "<<long(this)<<endl;
    cout<<"   ncreated = "<<ncreated<<" ndeleted = "<<ndeleted<<endl;
#endif
  }

  int numberOfReferences() const { return r ? *r : 0; }

  AS_ReferenceCounting & operator= ( const AS_ReferenceCounting &ref ) {

#ifdef TEST_AS_REFCOUNTING
    cout<<"operator= called for object at "<<long(this)<<endl;
#endif

    if ( ref.r!=r )
      {
	// we are changing the reference or it is a brand new reference
	if ( r )
	  if ( !decrement() ) delete r;
	r = ref.r;

	// if counting is on on both instances, increment the reference count
	if ( r ) increment();

      }

    return *this;
  }

  int breakReference()
  {
    int n = numberOfReferences();
    if (n)
      {
	decrement();
	r=0;
	increment();
      }
    return n;
  }

protected:

  int decrement() { 
#ifdef TEST_AS_REFCOUNTING
    if ( r )
      cout<<"decrementing object at "<<long(this)<<" to "<<*r-1<<" counts"<<endl;
    else
      cout<<"decrement called on non-counted object at "<<long(this)<<endl;
#endif
    if ( r )
      if ( ! --*r )
	{
	  delete r;
	  r=0;
#ifdef TEST_AS_REFCOUNTING
	ndeleted++;
#endif
	}

    return r ? *r : 0; 
  }

  int increment() { 

    if ( r )
      ++*r;
    else
      {
	// begin reference counting the object
	r = new int;
	*r = 1;

#ifdef TEST_AS_REFCOUNTING
	ncreated++;
#endif
      }

#ifdef TEST_AS_REFCOUNTING
    cout<<"incrementing object at "<<long(this)<<" to "<<*r<<" counts"<<endl;
#endif

    return *r; 
  }

  
#ifdef TEST_AS_REFCOUNTING
public:
  static int ncreated, ndeleted;
#endif

private:
  int *r;

};

// // //
/// simple multidimensional arrays
/** ArraySimple provides a simple array class for dealing with small arrays ( ~10x10 or so)
 *  Range checking is available by defining OV_DEBUG. The constructor sets the rank of an
 *  array which becomes immutable until a copy constructor is called or the array is destroyed.
 *  operator(), overloaded for the various ranks available (up to 4),
 *  provides Fortran like indexing.  For arrays with a rank>1, it will be more efficient to
 *  compute the index inside the loop and use operator[] to access the element (no range
 *  checking is done, essentially the rank is ignored for []).  
 *  This approach provides the compilers with a more easily optimizable loop.
 */
template <class T>
class ArraySimple : protected AS_ReferenceCounting {
public:

  /// default constructor
  /** create a blank array, suitable only for copying to
   */
  inline ArraySimple() : AS_ReferenceCounting(FALSE), data(0), nrank(0) 
  {
    for ( int r=0; r<MAXRANK; r++ ) n[r] = 0;
  }

  /// copy constructor
  inline ArraySimple( const ArraySimple<T> &a )  : AS_ReferenceCounting(FALSE), data(0), nrank(0)
  {

#if 0
    // shallow copy
    *this = a;

#else
    // deep copy
    nrank = a.nrank;
    for ( int r=0; r<MAXRANK; r++ ) n[r] = a.n[r];
    allocate();

    for ( int i=0; i<size(); i++ ) data[i] = a.data[i];
#endif

  }

#ifndef WAS_DETEMPLIFIED
#ifndef OV_NO_DEFAULT_TEMPL_ARGS
  /// copy from an ArraySimpleFixed
  template<int d1, int d2, int d3, int d4,int d5>
  inline ArraySimple( const ArraySimpleFixed<T, d1,d2,d3,d4,d5> &a ) : AS_ReferenceCounting(FALSE), data(0), nrank(0)
  {

    n[0] = d1;
    n[1] = d2; 
    n[2] = d3;
    n[3] = d4;
    n[4] = d5;

    if ( n[1]==1 && n[2]==1 && n[3]==1 && n[4]==1 ) nrank = 1;
    else if ( n[2]==1 && n[3]==1 && n[4]==1 ) nrank = 2;
    else if ( n[3]==1 && n[4]==1 ) nrank = 3;
    else if ( n[4]==1 ) nrank = 4;
    else nrank = 5;

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
  EXPLICIT ArraySimple(int n1) : AS_ReferenceCounting(FALSE), data(0), nrank(0) 
  {
    nrank = 1;
    n[0] = n1;
    for ( int r=1; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  }

  EXPLICIT ArraySimple(int n1, int n2) : AS_ReferenceCounting(FALSE), data(0), nrank(0) 
  {
    nrank = 2;
    n[0] = n1;
    n[1] = n2;
    for ( int r=2; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  }  

  EXPLICIT ArraySimple(int n1, int n2, int n3) : AS_ReferenceCounting(FALSE), data(0), nrank(0) 
  {
    nrank = 3;
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    for ( int r=3; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  } 

  EXPLICIT ArraySimple(int n1, int n2, int n3, int n4) : AS_ReferenceCounting(FALSE), data(0), nrank(0) 
  {
    nrank = 4;
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
    for ( int r=4; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  } 
  
  EXPLICIT ArraySimple(int n1, int n2, int n3, int n4, int n5) : AS_ReferenceCounting(FALSE), data(0), nrank(0) 
  {
    nrank = 5;
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
    n[4] = n5;
    for ( int r=5; r<MAXRANK; r++ ) n[r] = 1;
    allocate();
  } 
  
  // destructor
  inline ~ArraySimple() { if (numberOfReferences()<2) destroy(); }
  
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
  inline int size( const int &r_ ) const 
  {
    // /Description : return the size of one dimension
    assert(RANGE_CHK(r_>=0 && r_<nrank));
    
    return n[r_];
  }

  // rank
  inline int rank() const
  {
    // /Description : return the rank of the array
    return nrank; 
  }

  // deep copy
  inline void copy(const ArraySimple<T> &a)
  {
    // /Description : create a deep copy of array a
    
    int s = size();
    int as = a.size();

    if ( breakReference()<2 && s!=as ) 
      destroy();

    nrank = a.nrank;
    for (int r=0; r<MAXRANK; r++ ) n[r] = a.n[r];
    
    if ( s!=as )
      {
	data = 0;
	allocate();
      }

    for ( int i=0; i<s; i++ ) data[i] = a.data[i];

  }

  // shallow copy
  inline void reference(ArraySimple<T> &a)
  {
#ifdef TEST_AS_REFCOUNTING
    cout<<"instance at "<<long(this)<<" is referencing instance at "<<long(&a)<<endl;
#endif

    // /Description : create a reference to the data in a (shallow copy)
    if ( a.numberOfReferences()==0 ) a.increment(); // start the counting if needed

    (AS_ReferenceCounting &)(*this) = a;
    nrank = a.nrank;
    for ( int r=0; r<MAXRANK; r++ ) n[r] = a.n[r];
    data = a.data;

  }

  // ptr
  inline T * ptr() 
  {
    // /Description : returns the data pointer
    return data; 
  }

  inline ArraySimple<T> & operator= ( const ArraySimple<T> &a )
  {

#if 0
    // /Description : perform a shallow copy

    (AS_ReferenceCounting &)(*this) = a;
    nrank = a.nrank;
    for ( int r=0; r<MAXRANK; r++ ) n[r] = a.n[r];
    data = a.data;

    return *this;
    
#else
    // /Description : perform a deep copy
    if ( numberOfReferences()>1 ) 
      {
	breakReference();
	decrement(); // turn off the reference counting
	data = 0;
      }

    int s=size();
    int sa=a.size();

    if ( s != sa )
      destroy();	
	
    nrank = a.nrank;
    for ( int r=0; r<MAXRANK; r++ ) n[r] = a.n[r];

    if ( s!=sa )
      allocate();
  
    for ( int i=0; i<size(); i++ ) data[i] = a.data[i];
    return *this;
#endif

  }

  inline ArraySimple<T> & operator= ( const T &a )
  {
    // /Description : set all array values equal to a const value
    for ( int i=0; i<size(); i++ ) data[i] = a;
    return *this;
  }

  // operator[]
  inline T & operator[] (const int &i)
  { 
    // /Description : indexes the data using 1d indexing (no range checks along each axis)
    assert(RANGE_CHK( i>=0 && i<size() ) );
    return data[i];
  }

  // operator[]
  inline const T & operator[] (const int &i) const
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
  inline T & operator() (const int &i0) 
  {
    assert(RANGE_CHK( nrank==1 && i0>-1 && i0<n[0] )); 
    return data[ i0 ];
  }

  inline const T & operator() (const int &i0) const
  {
    assert(RANGE_CHK( nrank==1 && i0>-1 && i0<n[0] )); 
    return data[ i0 ];
  }

  inline T & operator() (const int &i0, const int &i1) 
  {
    assert(RANGE_CHK( nrank==2 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] ));
    return data[ i0 + i1*n[0] ];
  }

  inline const T & operator() (const int &i0, const int &i1) const
  {
    assert(RANGE_CHK( nrank==2 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] ));
    return data[ i0 + i1*n[0] ];
  }

  inline T & operator() (int i0, int i1, int i2) 
  { 
    assert(RANGE_CHK( nrank==3 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] ));
    return data[ i0 + n[0]*(i1 + i2*n[1]) ];

  }

  inline const T & operator() (int i0, int i1, int i2) const
  { 
    assert(RANGE_CHK( nrank==3 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] ));
    return data[ i0 + n[0]*(i1 + i2*n[1]) ];

  }

  inline T & operator() (int i0, int i1, int i2, int i3) 
  {
    assert(RANGE_CHK( nrank==4 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] &&
		      i3>-1 && i3<n[3] ));
    return data[ i0 + n[0]*(i1 + n[1]*(i2 + i3*n[2])) ];

  }

  inline const T & operator() (int i0, int i1, int i2, int i3) const
  {
    assert(RANGE_CHK( nrank==4 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] &&
		      i3>-1 && i3<n[3] ));
    return data[ i0 + n[0]*(i1 + n[1]*(i2 + i3*n[2])) ];

  }

  inline T & operator() (int i0, int i1, int i2, int i3, int i4) 
  {
    assert(RANGE_CHK( nrank==4 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] &&
		      i3>-1 && i3<n[3] &&
		      i4>-1 && i4<n[4] ));
    return data[ i0 + n[0]*(i1 + n[1]*(i2 + n[2]*(i3 + n[3]*i4))) ];
//    return data[ i0 + n[0]*(i1 + n[1]*(i2 + i3*n[2])) ];

  }

  inline const T & operator() (int i0, int i1, int i2, int i3, int i4) const
  {
    assert(RANGE_CHK( nrank==4 &&
		      i0>-1 && i0<n[0] &&
		      i1>-1 && i1<n[1] &&
		      i2>-1 && i2<n[2] &&
		      i3>-1 && i3<n[3] &&
		      i4>-1 && i4<n[4] ));
    return data[ i0 + n[0]*(i1 + n[1]*(i2 + n[2]*(i3 + n[3]*i4))) ];

  }

  /// resize the array, copying the data
  void resize(int n1, int n2=-1, int n3=-1, int n4=-1, int n5=-1) 
  {
    int oldRefs = numberOfReferences();
    if ( oldRefs )
      {
	breakReference();
	decrement(); // turn off ref counting
      }

    T *tmp_data = data;
    int tmp_size = size();
    
    int i=0;
    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
    n[4] = n5;
    while ( n[i]>0 && i<MAXRANK ) i++;
    nrank = i;

    for ( i=nrank; i<MAXRANK; i++ ) n[i] = 1;

    data = 0;
    allocate();
    
    for ( int i=0; i<min(tmp_size,size()); i++ )
      data[i] = tmp_data[i];
    
    if ( oldRefs<2 && tmp_data ) 
      {
	delete [] tmp_data;
#ifdef TEST_AS_REFCOUNTING 
	cout<<"DELETING ARRAY DATA DURING RESIZE FOR INSTANCE "<<long(this)<<endl;
	ndel++;
	cout<<"   nalloc = "<<nalloc<<" ndel "<<ndel<<endl;
#endif
      }
  }

  void redim(int n1, int n2=-1, int n3=-1, int n4=-1, int n5=-1) 
  {
    int oldRefs = numberOfReferences();
    if ( oldRefs )
      {
	breakReference();
	decrement(); // turn off ref counting
      }

    destroy();

    n[0] = n1;
    n[1] = n2;
    n[2] = n3;
    n[3] = n4;
    n[4] = n5;

    int i=0;
    while ( n[i]>0 && i<MAXRANK ) i++;
    nrank = i;

    for ( i=nrank; i<MAXRANK; i++ ) n[i] = 1;
    
    data = 0;
    allocate();
    
  }
  
#ifdef TEST_AS_REFCOUNTING
  static int nalloc, ndel;
#endif

protected:

  inline void allocate() 
  { 
    int s = size();

    assert(data==0);
    assert(RANGE_CHK(s>=0));

#ifdef TEST_AS_REFCOUNTING 
    if ( s ) 
      {
	cout<<"ALLOCATING ARRAY DATA FOR INSTANCE "<<long(this)<<endl;
	nalloc++;
	cout<<"   nalloc = "<<nalloc<<" ndel "<<ndel<<endl;
      }
#endif

    if ( s>0 )
      {
	data = new T[s];
	assert(data != 0);
      }
    else
      data = 0;
  }

  inline void destroy()
  {
#ifdef TEST_AS_REFCOUNTING 
    if ( data )
      {
	cout<<"DESTROYING ARRAY DATA FOR INSTANCE "<<long(this)<<endl;
	ndel++;
	cout<<"   nalloc = "<<nalloc<<" ndel "<<ndel<<endl;
      }
#endif

    if ( data!=0 ) delete [] data; 
    for ( int r=0; r<MAXRANK; r++ ) n[r] = 0;
    data = 0;
    nrank = 0;
  }

private:
  T *data;
  int n[MAXRANK];
  short nrank;

};
//
// end of ArraySimple
// // //

#ifndef WAS_DETEMPLIFIED
typedef ArraySimple<int> intArraySimple;
typedef ArraySimple<float> floatArraySimple;
typedef ArraySimple<double> doubleArraySimple;
#endif

template<class T>
ostream & operator<< (ostream &os, ArraySimple<T> &sa)
{
  // /Description : overloading for ostream to make printing ArraySimples easier

  int rank = sa.rank();

  os<<"********** ArraySimple( ";

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
	case 5:
	  {
	    for ( int i1=0; i1<sa.size(0); i1++ )
	      for ( int i2=0; i2<sa.size(1); i2++ )
		for ( int i3=0; i3<sa.size(2); i3++ )
		  for ( int i4=0; i4<sa.size(3); i4++ )
		    for ( int i5=0; i5<sa.size(4); i5++ )
		      os<<"a( "<<i1<<", "<<i2<<", "<<i3<<", "<<i4<<", "<<i5<<" ) = "<<sa(i1,i2,i3,i4,i5)<<endl;
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
