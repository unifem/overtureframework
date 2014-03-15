#ifndef __ARRAY_SIMPLE_FIXED_H__
#define __ARRAY_SIMPLE_FIXED_H__

#include "ArraySimpleCommon.h"

// // //
/// simple fixed size arrays of up to rank 4
/** Provides an efficient wrapper for statically declared arrays (eg a[2][3].)
 *  The class also provides column major ordering and access to the data pointer. It should
 *  support ranks up to MAXRANK.
 */
#ifndef OV_NO_DEFAULT_TEMPL_ARGS
template <class T, int d1, int d2=1, int d3=1, int d4=1, int d5=1>
#else
template <class T, int d1, int d2, int d3, int d4, int d5>
#endif
class ArraySimpleFixed
{
public:
  /// default constructor, does nothing
  inline ArraySimpleFixed() { }

  /// copy constructor, copies contents of source into sink
  inline ArraySimpleFixed(const ArraySimpleFixed<T,d1,d2,d3,d4,d5> &a)
  { for ( int i=0; i<size(); i++ ) data[i] = a.data[i]; }

  /// destructor
  inline ~ArraySimpleFixed() { }

  /// get the total size of the array
  /** \return the size of the array
   */
  inline int size() const { return d1*d2*d3*d4*d5; }

  /// get the size of the array along one dimension
  /** \param i the dimension whose size is requested
   *  \return the size of the array along dimension i
   */
  inline int size(const int &i) const 
  { 
    assert(i>=0 && i<5);
    return i==0 ? d1 : (i==1 ? d2 : (i==2 ? d3 : ( i==3 ? d4 : d5) ) ); 
  }

  /// get the rank of the array
  inline int rank() const { return 5; }

  /// return a pointer to the data
  inline T * ptr() { return &data[0]; }

  /// assignment operator, copy data from source array into the current instance
  /** \param a the source array
   *  \return the sink array
   */
  inline ArraySimpleFixed<T,d1,d2,d3,d4,d5> & operator= ( const ArraySimpleFixed<T,d1,d2,d3,d4,d5> &a )
  { 
    for ( int i=0; i<size(); i++ ) data[i] = a.data[i]; 
    return *this;
  }
    
  inline ArraySimpleFixed<T,d1,d2,d3,d4,d5> & operator= ( const T &a )
  {
    // /Description : set all array values equal to a const value
    for ( int i=0; i<size(); i++ ) data[i] = a;
    return *this;
  }

  /// one dimensional index operator
  /** index into the array in a one dimensional manner, ignoring the dimensions of the array
   * \param i index into the data
   * \return a reference to data item i
   */
  inline T & operator[] ( const int &i ) 
  { 
    assert( RANGE_CHK(i>=0 && i<size()) );
    return data[i]; 
  }

  /// const one dimensional index operator
  /** index into the array in a one dimensional manner, ignoring the dimensions of the array
   * \param i index into the data
   * \return a const reference to data item i
   */
  inline const T & operator[] ( const int &i ) const 
  { 
    assert( RANGE_CHK(i>=0 && i<size()) );
    return data[i]; 
  }

  /// multidimensional index operator
  /** 
   * \param i1 rank 1 index
   * \param i2 rank 2 index, range check fails if rank is 1 and i2>0
   * \param i3 rank 3 index, range check fails if rank is 2 and i3>0
   * \param i4 rank 4 index, range check fails if rank is 3 and i4>0
   * \param i5 rank 5 index, range check fails if rank is 4 and i5>0
   * \return a reference to data item at location (i1,i2,i3,i4)
   */
  inline T & operator() ( const int & i1, const int & i2=0, const int & i3=0, const int & i4=0,const int &i5=0 ) 
  {
    assert( RANGE_CHK(i1>=0 && i1<d1 &&
		      i2>=0 && i2<d2 &&
		      i3>=0 && i3<d3 &&
		      i4>=0 && i4<d4 &&
		      i5>=0 && i5<d5 ) );
    return data[i1 + d1*(i2 + d2*(i3 + d3*(i4+d4*i5)))]; 
  }

  /// const multidimensional index operator
  /** 
   * \param i1 rank 1 index
   * \param i2 rank 2 index, range check fails if rank is 1 and i2>0
   * \param i3 rank 3 index, range check fails if rank is 2 and i3>0
   * \param i4 rank 4 index, range check fails if rank is 3 and i4>0
   * \return a reference to data item at location (i1,i2,i3,i4)
   */
  inline const T & operator() ( const int & i1, const int & i2=0, 
				const int & i3=0, const int & i4=0,
				const int & i5=0) const 
  {
    assert( RANGE_CHK(i1>=0 && i1<d1 &&
		      i2>=0 && i2<d2 &&
		      i3>=0 && i3<d3 &&
		      i4>=0 && i4<d4 &&
		      i5>=0 && i5<d5 ) );
    return data[i1 + d1*(i2 + d2*(i3 + d3*(i4+d4*i5)))]; 
//    return data[i1 + d1*(i2 + d2*(i3 + i4*d3))]; 
  }

private:
  T data[d1*d2*d3*d4*d5];
};

//
// End of ArraySimpleFixed
// // //



#ifndef OV_NO_DEFAULT_TEMPL_ARGS
template <class T, int d1, int d2, int d3, int d4, int d5>
ostream & operator<< (ostream &os, ArraySimpleFixed<T,d1,d2,d3,d4,d5> &a)
{
  // /Description : overloading of ostream to make printing ArraySimpleFixeds easier
  os<<"********** ArraySimpleFixed( "<<d1<<", "<<d2<<", "<<d3<<", "<<d4<<" ) **********\n";

  for ( int i1=0; i1<d1; i1++ )
    for ( int i2=0; i2<d2; i2++ )
      for ( int i3=0; i3<d3; i3++ )
	for ( int i4=0; i4<d4; i4++ )
	  for ( int i5=0; i5<d5; i5++ )
	  os<<"a( "<<i1<<", "<<i2<<", "<<i3<<", "<<i4<<", "<<i5<<" ) = "<<a(i1,i2,i3,i4,i5)<<endl;

  return os;
}
#endif

#endif
