#include <math.h>

// ** To-do *** Put this function somewhere ***

// forward declaration
int ovmod (int a, int b);


// ==============================================================================
/// \brief Define a cyclic index. This can can be used to index another vector
///     as a cyclic (circular) vector.
///
/// \details Example use:
/// 
///     int n=5, base=0;
///     CylicIndex ic(n,base);  // define an index of length 5: [0,1,2,3,4]
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
class CyclicIndex 
{
public:

CyclicIndex( int length, int shift=0, bool increasing=true );

CyclicIndex(const CyclicIndex & x);

~CyclicIndex();

CyclicIndex& operator=(const CyclicIndex & x);

// The cycle can be indexed with any positive, negative or zero integer
int operator[]( int i ) const{ return data[ovmod(i+base,n)]; }  
// int & operator[]( int i ){ return data[ovmod(i+base,n)]; }  


// Shift the origin of the index by count
void shift( int count=1 ){ base+=count; } // 


private:
int n, base;
int *data;

};


