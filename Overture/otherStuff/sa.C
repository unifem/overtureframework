// Test of the sparse array class
//     -- only store values that are assigned, other entries get a default

// Maybe we want to be able to switch between a sparse and full representation
// once the array becomes sufficiently full


#include "Overture.h"  
#include "SparseArray.h"



int 
main(int argc, char** argv)
{
  Overture::start(argc,argv);  // initialize Overture


  typedef SparseArray<int> intSparseArray;
  intSparseArray a(10);
  
  a.set(2 , 4);
  
  for( int i=0; i<10; i++ )
  {
    printf("a(%i) = %i\n",i,a(i)); 
  }


  typedef SparseArray<real> realSparseArray;
  realSparseArray b(5,5);
  b.setDefaultValue(7.);
  b.set( 24., 2,4 );
  b.set( 31., 3,1 );

  b.get( 1,2)=12.;
  
  for( int i0=0; i0<5; i0++ )
  {
    for( int i1=0; i1<5; i1++ )
    {
      printf("b(%i,%i)=%g ",i0,i1,b(i0,i1)); 
    }
    printf("\n");
  }
  printf(" ---> The number of entries in the sparse array b is b.sparseSize()=%i\n",b.sparseSize());
  

/* ---

  a[0]=5;
  a[3]=2;
  a.set(4, 2);
  a[7]=6;
  
  // Here we assign a value 
  a(8,intSparseArray::createEntryIfNeeded)=8;
  

  intSparseArray::iterator it;
  it = a.find(3);
  (*it).second=4;          // is this legal to set a value ?

//  const intSparseArray & ac =a;
  for( int i=0; i<10; i++ )
  {

    printf("a(%i) = %i\n",i,a(i)); 
//    printf("a(%i) = %i\n",i,ac(i)); 

//     i = a.find(j);
//     if( i != a.end() )
//     {
//       printf("a[%i] = %i\n",(*i).first,(*i).second);
//     }
//     else
//     {
//       printf("a[%i] = default\n",j);
//     }
  }
  

//   const intSparseArray b;
//   b[0]=7;
//   int i=0;
//   printf("b(%i) = %i\n",i,b(i)); 


--- */

  Overture::finish();

  return 0;
}

