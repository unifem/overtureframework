
#include <string.h>
#include "Fraction.h"
#include "Bound.h"





int
main()
{
  cout << "==========Test of the Fraction Class and Bound Class===========" << endl;

  const int nFraction=5;
  Fraction fa(1,2),fb(3,4),fc(1,0),fd(0,1),fe(-2,0);
  Fraction f1,f2,F1[nFraction], F2[nFraction];

  F1[0]=fa; F2[0]=fb;
  F1[1]=fa; F2[1]=fc;
  F1[2]=fa; F2[2]=fd;
  F1[3]=fb; F2[3]=fb;
  F1[4]=fc; F2[4]=fe;

  for( int i=0; i<nFraction; i++ )
  { f1=F1[i]; f2=F2[i];
    cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
    cout << " f1 = " << f1 << ", f2 =" << f2 << endl;
   
    cout << " f1+f2 =" << f1+f2 << ", error = " << (f1+f2)-(f1*1.+f2*1.) << endl;
    cout << " f1-f2 =" << f1-f2 << ", error = " << (f1-f2)-(f1*1.-f2*1.) << endl;
    cout << " f1*f2 =" << f1*f2 << ", error = " << (f1*f2)-(f1*1.)*(f2*1.) << endl;
    cout << " f1/f2 =" << f1/f2 << ", error = " << (f1/f2)-(f1*1.)/(f2*1.) << endl;
    cout << " f1<f2 =" << (f1<f2) << endl;
    cout << " f1<=f2 =" << (f1<=f2) << endl;
    cout << " f1>f2 =" << (f1>f2) << endl;
    cout << " f1>=f2 =" << (f1>=f2) << endl;
    cout << " f1==f2 =" << (f1==f2) << endl;

  }

  const int nBound=4;
  Bound ba(Fraction(1,2)),bb(Fraction(1,4)),bc(real(1./3.)),bd(Fraction(1,0));
  Bound b1,b2,B1[nBound], B2[nBound];   

  B1[0]=ba; B2[0]=bb;
  B1[1]=ba; B2[1]=bc;
  B1[2]=ba; B2[2]=bd;
  B1[3]=bb; B2[3]=bb;

  real one=1.;

  for( i=0; i<nBound; i++ )
  {
     b1=B1[i]; b2=B2[i];
     cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
     cout << " b1 = " << b1 << ", b2 =" << b2 << endl;
   
     cout << " b1+b2 =" << b1+b2 << ", error = " << (b1+b2)-(b1*one+b2*one) << endl;
     cout << " b1-b2 =" << b1-b2 << ", error = " << (b1-b2)-(b1*one-b2*one) << endl;
     cout << " b1*b2 =" << b1*b2 << ", error = " << (b1*b2)-(b1*one)*(b2*one) << endl;
     cout << " b1/b2 =" << b1/b2 << ", error = " << (b1/b2)-(b1*one)/(b2*one) << endl;
     cout << " b1<b2 =" << (b1<b2) << endl;
     cout << " b1<=b2 =" << (b1<=b2) << endl;
     cout << " b1>b2 =" << (b1>b2) << endl;
     cout << " b1>=b2 =" << (b1>=b2) << endl;
     cout << " b1==b2 =" << (b1==b2) << endl;
   
   }

  cout << " ba = " << ba << ", ba.isFinite= " << ba.isFinite() << ", (real)ba = " << (real)ba << endl;
  cout << " bd = " << bd << ", bd.isFinite= " << bd.isFinite() << ", (real)bd = " << (real)bd << endl;
  
  return 0;
}
