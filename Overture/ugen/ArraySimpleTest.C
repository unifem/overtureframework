#define assert(x)

#include "OvertureDefine.h"

#include <iostream.h>
#include <time.h>
#include <math.h>
#include "ArraySimple.h"
#include "ArraySimpleInt2.h"

OV_USINGNAMESPACE(std);

int main(void)
{

  //int testsize = 10;
  int k;

  int n1=10,n2=10;
  ArraySimpleInt ia(n1,n2);
//  intArraySimple ia(n1,n2);
  intArraySimple ia3(n1,n2,n2);
  intArraySimple ia4(n1,n2,n2,n2);
  int *i = new int[n1*n2];
  int *bb = new int[n1];

  int nn[3];
  nn[0]=n1;
  
#ifndef OV_NO_DEFAULT_TEMPL_ARGS
  ArraySimpleFixed<int,10,10> isa;
  ArraySimpleFixed<int,2,2,2> isa3;
  ArraySimpleFixed<int,2,2,2,2> isa4;
  ArraySimple<ArraySimpleFixed<int,10> > iasa(10);
#else
  ArraySimpleFixed<int,10,10,1,1> isa;
  ArraySimpleFixed<int,2,2,2,1> isa3;
  ArraySimpleFixed<int,2,2,2,2> isa4;
  ArraySimple<ArraySimpleFixed<int,10,1,1,1> > iasa(10);
#endif


  VectorSimple<int> vs(n2*n1);
  ArraySimpleInt b(n1);
//  intArraySimple b(n1);

  intArraySimple isacopy;
  intArraySimple vscopy;

  ArraySimple<int> a5(5);
  ArraySimple<int> a55(5,5);
  ArraySimple<int> a555(5,5,5);
  ArraySimple<int> a5555(5,5,5,5);

  //int *b = new int[n1];
  //for ( k=0; k<testsize; k++ )
  for ( k=0; k<n2; k++ )
    for ( int l=0; l<n1; l++ )
    {
      ia(l,k) = 10;
      i[l + n1*k] = 10;
      b[l] = l;
      bb[l] = l;
// -- wdh comment out for now -- takes too long
      for ( int m=0; m<n2; m++ )
	{
	  ia3(l,k,m) = b[l]*m;
	  for ( int n=0; n<n2; n++ )
	    ia4(l,k,m,n) = b[l]*m*n;
	}
// 
    }

  int itersize = 100000;

  int n;
  clock_t t0 = clock();
  for ( n=0; n<itersize; n++ )
    for ( k=0; k<n2; k++ )
      for ( int l=0; l<n1; l++ )
	{
	  ia(l,k) = b(l)*b(l); // b[l]*b[l];//sqrt(float(b[l]));
	}

  clock_t t1 = clock();

  int tmp;
  for ( n=0; n<itersize; n++ )
    for ( k=0; k<n2; k++ )
#if 0
      for ( int l=0; l<n1; l++ )
	{
	  ia[l+n1*k] = b[l]*b[l];//sqrt(float(b[l]));
	}
#else
     { int l1=0;
       if ( l1<n1 )
	 { 
	   tmp = n1*k;
	   for ( ; l1<n1; l1+=1 )
	     ia(l1+tmp) = b[l1]*b[l1];
	 }
     }
#endif
  clock_t t2 = clock();

  for ( n=0; n<itersize; n++ )
    for ( k=0; k<n2; k++ )
      for ( int l=0; l<n1; l++ )
	{
	  i[l + n1*k] = b[l]*b[l];//sqrt(float(b[l]));
	}

  clock_t t3 = clock();

  for ( n=0; n<itersize; n++ )
    for ( k=0; k<n2; k++ )
      for ( int l=0; l<n1; l++ )
	{
	  isa(l,k) = b[l]*b[l];
	}
  
  clock_t t4 = clock();

#if 1
  for ( n=0; n<itersize; n++ )
    for ( k=0; k<n2; k++ )
      for ( int l=0; l<n1; l++ )
	{
	  iasa[l](k) = b[l]*b[l];
	}
#endif

  clock_t t5 = clock();

  for ( n=0; n<itersize; n++ )
    for ( k=0; k<n2; k++ )
      for ( int l=0; l<n1; l++ )
	{
	  vs[l+k*n1] = b[l]*b[l];
	}

  clock_t t6 = clock();

  
  for ( n=0; n<itersize; n++ )
    for ( k=0; k<n2; k++ )
      for ( int l=0; l<n1; l++ )
	{
	  i[l + nn[0]*k] = bb[l]*bb[l]; //sqrt(float(b[l]));
	}

  clock_t t7 = clock();

  cout<<"ArraySimpleInt: ia(l,k) = b(l)*b(l) = "<<float(t1-t0)/float(CLOCKS_PER_SEC)<<endl;
  cout <<"c loop i[l + nn[0]*k] = bb[l]*bb[l] = " << float(t7-t6)/float(CLOCKS_PER_SEC)<<endl;

  cout<<"ia1d  = "<<float(t2-t1)/float(CLOCKS_PER_SEC)<<endl;
  cout<<"isa = "<<float(t4-t3)/float(CLOCKS_PER_SEC)<<endl;
  cout<<"iasa = "<<float(t5-t4)/float(CLOCKS_PER_SEC)<<endl;
  cout<<"vs = "<<float(t6-t5)/float(CLOCKS_PER_SEC)<<endl;
  cout<<"i  = "<<float(t3-t2)/float(CLOCKS_PER_SEC)<<endl;
  for ( k=0; k<n2; k++ )
    for ( int l=0; l<n1; l++ )
      {
	if ( ia(l,k)!=i[l+k*n2] ) throw "error";
	if ( isa(l,k)!=i[l+k*n2] ) throw "error";
	if ( iasa[l][k]!=i[l+k*n2] ) throw "error";
      }

#ifndef OV_NO_DEFAULT_TEMPL_ARGS
  isacopy = isa;
#endif

  cout << "vscopy.ptr()=" << vscopy.ptr() << endl;
  
  vscopy = vs;

#if 0
  cout<<ia<<endl;
  cout<<ia3<<endl;
  cout<<ia4<<endl;
  intArraySimple ianull;
  cout<<ianull<<endl;

  cout<<isa<<endl;
  cout<<isacopy<<endl;
#endif

  for ( int i1=0; i1<5; i1++ )
    {
      a5(i1) = 1000*i1;
      for ( int i2=0; i2<5; i2++ )
	{
	  a55(i1,i2) = a5(i1) + 100*i2;
	  for ( int i3=0; i3<5; i3++ )
	    {
	      a555(i1,i2,i3) = a55(i1,i2) + 10*i3;
	      for ( int i4=0; i4<5; i4++ )
		a5555(i1,i2,i3,i4) = a555(i1,i2,i3) + i4;
	    }
	}
    }

#if 0
  cout<<a5<<endl;
  cout<<a55<<endl;
  cout<<a555<<endl;
  cout<<a5555<<endl;
#endif

  delete [] i;
  //delete [] b;

  return 0;
}

