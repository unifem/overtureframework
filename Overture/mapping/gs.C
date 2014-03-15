//
// Test out the A++ Class Library
//


#include <A++.h>

// Here we use double precision
// typedef double real;
// typedef doubleArray RealArray;  

// Here we use single precision
typedef float real;
typedef floatArray RealArray;  


void output(  char *label, RealArray & X)
{
// Output a Label and an A++ Real Array

  cout << label << endl;
  for( int i=X.getBase(0); i <=X.getBound(0); i++ )
    {
    cout << " i = " << i << " X(i) =" << X(i) << endl;
    }
} 

void output(  char *label, IntegerArray & X)
{
// Output a Label and an A++ int Array

  cout << label << endl;
  for( int i=X.getBase(0); i <=X.getBound(0); i++ )
    {
    cout << " i = " << i << " X(i) =" << X(i) << endl;
    }
} 

inline RealArray Dx( RealArray & X, Index I )
{
  return( X(I+1)-X(I-1) );
}

real d2xi = 10.;
real d2ri = 10.;



inline real Drr( RealArray A, int i)
  {
   return( (A(i+1)-2.*A(i)+A(i-1))*d2ri );
  }    

inline real Dss( RealArray A, int i)
  {
   return( (A(i+1)-2.*A(i)+A(i-1))*d2ri );
  }    

inline real Dxx( RealArray A, int i)
  {
   extern RealArray rx;
   return( rx(i)*Drr(A,i)+rx(i)*Dss(A,i) );
  }    


RealArray rx(10); 


#define drr(x,i) ((x(i+1)-2.*x(i)+x(i-1))*d2ri)
#define dss(x,i) ((x(i+1)-2.*x(i)+x(i-1))*d2ri)
#define dxx(x,i) (rx(i)*drr(x,i)+rx(i)*dss(x,i)) 

void main()
{

  rx=1;

  int arraySize =10;

  cout << "====== Test of A++ =====" << endl;

  RealArray A1(arraySize);
  RealArray A2(arraySize);

  A1 = 1.;
  A2 = A1;

  A1.display("This is A1:");

  output( "Here is A1 Again--->", A1 );

  IntegerArray B(5);
  B.setBase(1);
  B=7;
  output( "Here is an int array, B:", B); 

  A2.display("This is A2:");

  Index I(2,7,1); // Index from 2,3,...,7

  A1(I)=2.;
  A1.display("Here is A1 now:");

  sin(A1).display("Here is sin(A1)"); 

  RealArray A3(5,5);
  A3.setBase(1);  // set base to 1 in all directions
  A3=3;
  A3.display("Here is A3:");
//  A3.view("Here is a view of A3:");


//  A2 = Dx(A1,I);
//  A2.display("Here is Dx(A1) ");


  cout << " Drr(A1,5) =" << Drr(A1,5) << endl;

  for( int i=1; i< 6; i++)
    {
      cout << " Dxx(A1," << i << ") =" << Dxx(A1,i) << endl;
      cout << " dxx(A1," << i << ") =" << dxx(A1,i) << endl;
    }

  printf("Finished test...\n");

}
