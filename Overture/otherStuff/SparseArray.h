#ifndef SPARSE_ARRAY
#define SPARSE_ARRAY

// =====================================================================================
//     Templated Sparse Array class
//
//   o multi-dimensional array (zero base)
//   o default value is given to all entries
//   o only entries with non-default values are stored
//
// Example:
//    SparseArray<real> b(5,5);
//    b.setDefaultValue(7.);
//    b.set( 24., 2,4 );   // b(2,4)=24.
//    b.get(1,2)=12.;      // b(1,2)=12.
//    for( int i0=0; i0<5; i0++ )
//    {
//      for( int i1=0; i1<5; i1++ )
//      {
//        printf("b(%i,%i)=%g ",i0,i1,b(i0,i1)); 
//      }
//      printf("\n");
//    }
// =====================================================================================



#define SPARSE_MAP std::map
#include <map>

// The hash_map has a constant time lookup (modulo initialization costs)
// #define SPARSE_MAP hash_map
// using namespace std;

// #include <ext/hash_map>

#define MAX_SPARSE_ARRAY_DIMS 6 

template<class T>
class SparseArray 
{
public:

typedef SPARSE_MAP<int, T, std::less<int> > SmapType;

SparseArray(int n0=0, int n1=1, int n2=1, int n3=1, int n4=1, int n5=1);
~SparseArray();

// access an array entry (no new entry is created)
const T & operator()(int i0, int i1=0, int i2=0, int i3=0, int i4=0, int i5=0) const;

// delete all sparse entries:
void clear();

// void compress( vector<int> & indexInfo, vector<T> & sparseData );
// void decompress( const vector<int> & indexInfo, const vector<T> & sparseData );

// remove all entries and redimension the array to size 0
void destroy();

// get a reference to an array entry (an new entry will be created if needed)
T & get(int i0=0, int i1=0, int i2=0, int i3=0, int i4=0, int i5=0 );

// redimension the array. 
void redim(int n0, int n1=1, int n2=1, int n3=1, int n4=1, int n5=1);

// set an array value. Create an entry if needed. Erase the entry if value=defaultValue
void set( const T & value, int i0, int i1=0, int i2=0, int i3=0, int i4=0, int i5=0 );

void setDefaultValue( const T & value );

int size() const;  // return the total number of (possible entries)

int size(int d ) const;  // return the size of dimension d of the array

// return the number of sparse entries stored
int sparseSize() const;

// // ---> We could also have a state that is set to create an entry if needed ---
// enum AssignOptionEnum
// {
//   createEntryIfNeeded=0
// };
// 
// 
// // use this operator to create an entry if it is accessed but does not exist
// T & operator()( const int i, AssignOptionEnum opt );

protected:

int boundsCheck(int i0, int i1, int i2, int i3, int i4, int i5 ) const;

SPARSE_MAP<int, T, std::less<int> > smap;

int dims[MAX_SPARSE_ARRAY_DIMS];
T defaultValue;

};

#define SPARSE_INDEX(i0,i1,i2,i3,i4,i5) ((i0)+dims[0]*((i1)+dims[1]*((i2)+dims[2]*((i3)+dims[3]*((i4)+dims[4]*((i5)))))))

template<class T> SparseArray<T>::
SparseArray(int n0 /* =0 */, int n1 /* =1 */, int n2 /* =1 */, int n3 /* =1 */, int n4 /* =1 */, int n5 /* =1 */)
// ===============================================================================================
//  /Description:
//    Create a sparse array with dimensions (n0,n1,...) and default value of zero.
// 
// /n0,n1,... (input) : dimensions 
// ===============================================================================================
{ 
  dims[0]=n0;
  dims[1]=n1;
  dims[2]=n2;
  dims[3]=n3;
  dims[4]=n4;
  dims[5]=n5;
  defaultValue=0;
}

template<class T> SparseArray<T>::
~SparseArray()
{
}

template<class T> int SparseArray<T>::
boundsCheck(int i0, int i1, int i2, int i3, int i4, int i5 ) const
{
  int iv[MAX_SPARSE_ARRAY_DIMS]={i0,i1,i2,i3,i4,i5}; // 
  bool ok=true;
  for( int d=0; d<MAX_SPARSE_ARRAY_DIMS; d++ )
  {
    if( iv[d]<0 || iv[d]>=dims[d] )
    {
      ok=false;
      printf("SparseArray::boundsCheck: index i%i=%i is out of bounds : [%i,%i]\n",d,iv[d],0,dims[d]-1);
    }
  }
  if( !ok ) Overture::abort("error");
  return 0;
}


template<class T> void SparseArray<T>::
clear()
// ===============================================================================================
//  /Description:
//    Remove all entries from the array.
// ===============================================================================================
{
  smap.clear();
}

template<class T> void SparseArray<T>::
destroy()
// ===============================================================================================
//  /Description:
//    Remove all entries from the array.
// ===============================================================================================
{
  clear();
  redim(0,0,0,0,0,0);
}


template<class T> T & SparseArray<T>::
get(int i0/* =0 */, int i1/* =0 */, int i2/* =0 */, int i3/* =0 */, int i4/* =0 */, int i5/* =0 */)
// ===============================================================================================
//  /Description:
//     Return a reference to value of the array at index (i0,i1,...)
// /i0,i1,... (input) : index to evaluate the array at
// ===============================================================================================
{
  boundsCheck(i0,i1,i2,i3,i4,i5);
  const int i = SPARSE_INDEX(i0,i1,i2,i3,i4,i5);

  typename SmapType::iterator sait = smap.find(i);

  if( sait != smap.end() )
  {
    return (*sait).second;
  }
  else 
  {
    // printf(" SparseArray: insert a new value into the array at [%i]\n",i);
    smap[i]=defaultValue;
    return smap[i];	
  }
}


template<class T> void SparseArray<T>::
redim(int n0 , int n1 /* =1 */, int n2 /* =1 */, int n3 /* =1 */, int n4 /* =1 */, int n5 /* =1 */)
// ===============================================================================================
//  /Description:
//    Redimension the array
// /n0,n1,... (input) : dimensions 
// ===============================================================================================
{
  dims[0]=n0;
  dims[1]=n1;
  dims[2]=n2;
  dims[3]=n3;
  dims[4]=n4;
  dims[5]=n5;
}


template<class T> const T & SparseArray<T>::
operator()(int i0, int i1/* =0 */, int i2/* =0 */, int i3/* =0 */, int i4/* =0 */, int i5/* =0 */ ) const
// ===============================================================================================
//  /Description:
//     Return the value of the array at index (i0,i1,...)
// /i0,i1,... (input) : index to evaluate the array at
// ===============================================================================================
{
  boundsCheck(i0,i1,i2,i3,i4,i5);
  const int i = SPARSE_INDEX(i0,i1,i2,i3,i4,i5);

  typename SmapType::const_iterator sait= smap.find(i);

  sait = smap.find(i);
  if( sait != smap.end() )
    return (*sait).second;
  else
    return defaultValue;
}

template<class T> void SparseArray<T>::
set(const T & value, int i0, int i1/* =0 */, int i2/* =0 */, int i3/* =0 */, int i4/* =0 */, int i5/* =0 */   )
// ===============================================================================================
//  /Description:
//     Assign a value:
//        array(i0,i1,i2,...)=value
//     Create a new entry if one is not alread y there.
// 
// /i0,i1,... (input) : index to evaluate the array at
// ===============================================================================================
{
  boundsCheck(i0,i1,i2,i3,i4,i5);
  const int i = SPARSE_INDEX(i0,i1,i2,i3,i4,i5);

  typename SmapType::iterator sait = smap.find(i);

  if( sait != smap.end() )
  {
    if( value!=defaultValue )
      (*sait).second=value;
    else
    {
      // Remove the entry if value==defaultValue
      smap.erase(sait);
    }
    
  }
  else if( value!=defaultValue )
  {
    // printf(" SparseArray: insert a new value into the array at [%i]\n",i);
    smap[i]=value;	
  }
      
}

template<class T> void SparseArray<T>::
setDefaultValue( const T & value )
// ===============================================================================================
//  /Description:
//     Assign a the default value for array entries that have not been set.
// 
// /value (input) : default value for array entries that have not been set.
// ===============================================================================================
{
  defaultValue=value;
}

template<class T> int SparseArray<T>::
size() const
// ===============================================================================================
//  /Description:
//     Return the total number of (possible) entries
// 
// ===============================================================================================
{
  int total=1;
  for( int d=0; d<MAX_SPARSE_ARRAY_DIMS; d++ )
    total*=dims[d];
  return total;
}

template<class T> int SparseArray<T>::
size(int d ) const
// ===============================================================================================
//  /Description:
//     Return the size of dimension d of the array.
// 
// ===============================================================================================
{
  if( d>=0 && d<MAX_SPARSE_ARRAY_DIMS )
    return dims[d];
  else
  {
    printf("SparseArray:ERROR:size: invalid array dimension d=%i\n",d);
    return -1;
  }
  
}


template<class T> int SparseArray<T>::
sparseSize() const
// ===============================================================================================
//  /Description:
//     Return the number of non-default entries in the sparse array
// 
// ===============================================================================================
{
  return smap.size();
}



// // use this operator to create an entry if it is accessed but does not exist
// template<class T> T & SparseArray<T>::
// operator()( const int i, AssignOptionEnum opt ) 
// {
//   typedef map<int, T, less<int> > SmapT;
//   typename SmapT::const_iterator sait = smap.find(i);
// 
//   sait = smap.find(i); 
//   if( sait != smap.end() )
//   {
//     return (*sait).second;
//   }
//   else
//   {
//     printf(" SparseArray: insert a new value into the array at [%i]\n",i);
//     // return (*this)[i](defaultValue);
//     return smap[i];
//   }
// }     

#undef SPARSE_INDEX
#undef MAX_SPARSE_ARRAY_DIMS
#undef SPARSE_MAP

#endif
