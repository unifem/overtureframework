#ifndef __VECTOR_SIMPLE_INT_H__
#define __VECTOR_SIMPLE_INT_H__

#include "ArraySimpleCommon.h"

// // //
/// A class for one dimensional dynamically allocated arrays
/**
 * VectorSimpleInt provides templatized, dynamically allocated 1D arrays.  It takes
 * only one template parameter specifying the type contained in the array.
 */

class VectorSimpleInt
{

public:
  /// default constructor
  /** creates an instance suitable only for copying into
   */
  inline VectorSimpleInt() : data(0), nsize(0) { }

  /// basic constructor with size information
  /** create an array of a specified size
   * \param s the requested size of the array
   */
  inline EXPLICIT VectorSimpleInt( const int &s ) 
  {
    nsize = s;
    data = new int[s];
  }

  /// copy constructor
  /** perform a deep copy of one VectorSimpleInt into another, only allocate
   *  new space if the size of the sink array is different from the source.
   * \param a the source array
   */
  inline VectorSimpleInt( const VectorSimpleInt &a ) : data(0), nsize(0)
  {
    if ( nsize != a.size() )
      {
	if ( data!=0 ) delete [] data;
	data = new int[a.size()];
      }
    
    nsize = a.nsize;
    for ( int s=0; s<nsize; s++ )
      data[s] = a[s];
  }

  /// destructor
  inline ~VectorSimpleInt() 
  {
    if ( data!=0 ) delete [] data;
    
    data = 0;
    nsize = 0;
  }

  /// return the length of the array
  /** \return the length of the array
   */
  inline int size() const 
  { 
    return nsize;
  }

  /// get the data pointer
  /** this method is usefull for communicating the data to other languages/libraries
   *  \return pointer to the data
   */
  inline int * ptr() 
  {
    return data;
  }

  /// assignment operator
  /** Perform an assignment of the sink array into the current instance.
   *  A deep copy is conducted, resizing the current array to the source if neccessary
   *  \return the copied array
   */
  inline VectorSimpleInt & operator= ( const VectorSimpleInt &a )
  {
    if ( nsize != a.size() )
      {
	if ( data!=0 ) delete [] data;
	data = new int[a.size()];
      }
    
    nsize = a.nsize;
    for ( int s=0; s<nsize; s++ )
      data[s] = a[s];

    return *this;
  }
  inline VectorSimpleInt & operator= ( const int &a )
  {
    for ( int s=0; s<nsize; s++ )
      data[s] = a;

    return *this;
  }

  /// index operator
  /** \param i the requested index
   * \return data item at index i
   */
  inline int & operator() ( const int &i ) 
  { 
    assert(RANGE_CHK(i>=0 && i<nsize));
    return data[i];
  }

  /// const index operator
  /** \param i the requested index
   * \return const reference to the item at index i
   */
  inline const int & operator() ( const int &i ) const 
  { 
    assert(RANGE_CHK(i>=0 && i<nsize));
    return data[i];
  }

  /// yet another index operator
  /** offered for consistency with the other ArraySimpleInt classes
   *  \param i the requested index
   *  \return reference to the item at index i
   */
  inline int & operator[] ( const int &i )
  { 
    assert(RANGE_CHK(i>=0 && i<nsize));
    return data[i];
  }

  /// yet another const index operator
  /** offered for consistency with the other ArraySimpleInt classes
   *  \param i the requested index
   *  \return const reference to the item at index i
   */
  inline const int & operator[] ( const int &i ) const 
  { 
    assert(RANGE_CHK(i>=0 && i<nsize));
    return data[i];
  }

private:
  int *data;
  int nsize;
};


ostream & operator<< (ostream &os, VectorSimpleInt &a)
{
  // /Description : overloading of ostream to make printing VectorSimples easier
 
  os<<"********** VectorSimpleInt[ "<<a.size()<<" ]\n";

  for ( int i=0; i<a.size(); i++ )
    os<<"a( "<<i<<" ) = "<<a[i]<<endl;

  return os;
}

#endif
