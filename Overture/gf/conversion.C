#include "conversion.h"

// define "=" operators for mixed mode A++ operations
//  Usage:
//     floatArray x(5);
//     intArray y(5);     
//     y=3;
//     equals(x,y);   // sets x=y
// 
#undef APP_EQUALS
#define APP_EQUALS(type) \
void \
equals(const type & y_, const type & x) \
{ \
  type & y = (type &)y_; \
  y=x; \
}
APP_EQUALS(intArray)
APP_EQUALS(floatArray)
APP_EQUALS(doubleArray)
#ifdef USE_PPP
APP_EQUALS(intSerialArray)
APP_EQUALS(floatSerialArray)
APP_EQUALS(doubleSerialArray)
#endif


#undef APP_EQUALS
#define APP_EQUALS(type1,type2,castType) \
void  \
equals(const type1 & y_, const type2 & x)  \
{  \
  type1 & y = (type1 &) y_; /* cast away const */ \
  const int nd = x.numberOfDimensions();  \
  int j0,j1,j2,j3, i0,i1,i2,i3;  \
  const int xb0=x.getBound(0), xb1=x.getBound(1);  \
  switch( nd )  \
  {  \
  case 1:  \
    for( i0=x.getBase(0),j0=y.getBase(0) ; i0<=xb0; i0++,j0++ )    \
    {    \
      y(j0)=(castType)x(i0);    \
    }    \
    break;  \
  case 2:  \
    for( i1=x.getBase(1), j1=y.getBase(1); i1<=xb1; i1++,j1++ )    \
    for( i0=x.getBase(0), j0=y.getBase(0); i0<=xb0; i0++,j0++ )    \
    {    \
      y(j0,j1)=(castType)x(i0,i1);    \
    }    \
    break;  \
  case 3:  \
    for( i2=x.getBase(2), j2=y.getBase(2); i2<=x.getBound(2); i2++,j2++ )    \
    for( i1=x.getBase(1), j1=y.getBase(1); i1<=xb1; i1++,j1++ )    \
    for( i0=x.getBase(0), j0=y.getBase(0); i0<=xb0; i0++,j0++ )    \
    {    \
      y(j0,j1,j2)=(castType)x(i0,i1,i2);    \
    }    \
    break;  \
  case 4:  \
    for( i3=x.getBase(3), j3=y.getBase(3); i3<=x.getBound(3); i3++,j3++ )    \
    for( i2=x.getBase(2), j2=y.getBase(2); i2<=x.getBound(2); i2++,j2++ )    \
    for( i1=x.getBase(1), j1=y.getBase(1); i1<=xb1; i1++,j1++ )    \
    for( i0=x.getBase(0), j0=y.getBase(0); i0<=xb0; i0++,j0++ )    \
    {    \
      y(j0,j1,j2,j3)=(castType)x(i0,i1,i2,i3);    \
    }    \
    break;  \
  default:  \
    cout << "equals conversion operator: ERROR: number of array dimensions =" << nd << endl;  \
    abort();  \
  }  \
}

APP_EQUALS(intArray,   floatArray  ,int)
APP_EQUALS(intArray,   doubleArray ,int)
APP_EQUALS(floatArray, intArray    ,float)
APP_EQUALS(floatArray, doubleArray ,float)
APP_EQUALS(doubleArray,intArray    ,double)
APP_EQUALS(doubleArray,floatArray  ,double)
#ifdef USE_PPP
APP_EQUALS(intSerialArray,   floatSerialArray  ,int)
APP_EQUALS(intSerialArray,   doubleSerialArray ,int)
APP_EQUALS(floatSerialArray, intSerialArray    ,float)
APP_EQUALS(floatSerialArray, doubleSerialArray ,float)
APP_EQUALS(doubleSerialArray,intSerialArray    ,double)
APP_EQUALS(doubleSerialArray,floatSerialArray  ,double)
#endif


#undef APP_EQUALS


#define type1 intArray
#define type2 floatArray
#define castType int
void  
equals(type1 & y, const type2 & x)  
{  
  const int nd = x.numberOfDimensions();  
  int j0,j1,j2,j3, i0,i1,i2,i3;  
  const int xb0=x.getBound(0), xb1=x.getBound(1);  
  switch( nd )  
  {  
  case 1:  
    for( i0=x.getBase(0),j0=y.getBase(0) ; i0<=xb0; i0++,j0++ )    
    {    
      y(j0)=(castType)x(i0);    
    }    
    break;  
  case 2:  
    for( i1=x.getBase(1), j1=y.getBase(1); i1<=xb1; i1++,j1++ )    
    for( i0=x.getBase(0), j0=y.getBase(0); i0<=xb0; i0++,j0++ )    
    {    
      y(j0,j1)=(castType)x(i0,i1);    
    }    
    break;  
  case 3:  
    for( i2=x.getBase(2), j2=y.getBase(2); i2<=x.getBound(2); i2++,j2++ )    
    for( i1=x.getBase(1), j1=y.getBase(1); i1<=xb1; i1++,j1++ )    
    for( i0=x.getBase(0), j0=y.getBase(0); i0<=xb0; i0++,j0++ )    
    {    
      y(j0,j1,j2)=(castType)x(i0,i1,i2);    
    }    
    break;  
  case 4:  
    for( i3=x.getBase(3), j3=y.getBase(3); i3<=x.getBound(3); i3++,j3++ )    
    for( i2=x.getBase(2), j2=y.getBase(2); i2<=x.getBound(2); i2++,j2++ )    
    for( i1=x.getBase(1), j1=y.getBase(1); i1<=xb1; i1++,j1++ )    
    for( i0=x.getBase(0), j0=y.getBase(0); i0<=xb0; i0++,j0++ )    
    {    
      y(j0,j1,j2,j3)=(castType)x(i0,i1,i2,i3);    
    }    
    break;  
  default:  
    cout << "equals conversion operator: ERROR: number of array dimensions =" << nd << endl;  
    abort();  
  }  
}

#undef type1
#undef type2
#undef castType






# define some bit-wise array operations

#define BITOP(op,intArray) \
intArray   \
operator op ( const intArray & x, const int & value )  \
{  \
  const int nd = x.numberOfDimensions();  \
  int i0,i1,i2,i3;  \
  const int xa0=x.getBase(0),  xa1=x.getBase(1);  \
  const int xb0=x.getBound(0), xb1=x.getBound(1);  \
  \
  intArray y; y.redim(x);  \
    \
  switch( nd )  \
  {  \
  case 1:  \
    for( i0=xa0; i0<=xb0; i0++ )    \
    {    \
      y(i0)=x(i0) op value;    \
    }    \
    break;  \
  case 2:  \
    for( i1=xa1; i1<=xb1; i1++ )    \
      for( i0=xa0; i0<=xb0; i0++ )    \
      {    \
	y(i0,i1)=x(i0,i1) op value;      \
      }    \
    break;  \
  case 3:  \
    for( i2=x.getBase(2); i2<=x.getBound(2); i2++ )    \
    for( i1=xa1; i1<=xb1; i1++ )    \
      for( i0=xa0; i0<=xb0; i0++ )    \
      {    \
	y(i0,i1,i2)=x(i0,i1,i2) op value;    \
      }    \
    break;  \
  case 4:  \
    for( i3=x.getBase(3); i3<=x.getBound(3); i3++ )    \
    for( i2=x.getBase(2); i2<=x.getBound(2); i2++ )    \
    for( i1=xa1; i1<=xb1; i1++ )    \
      for( i0=xa0; i0<=xb0; i0++ )    \
      {    \
	y(i0,i1,i2,i3)=x(i0,i1,i2,i3) op value;    \
      }    \
    break;  \
  default:  \
    cout << "bitwise " #op "operator: ERROR: number of array dimensions =" << nd << endl;  \
    abort();  \
  }  \
  return y;  \
}  \

#define BITOP2(op,intArray) \
intArray &  \
operator op ( intArray & x, const int & value )  \
{  \
  const int nd = x.numberOfDimensions();  \
  int i0,i1,i2,i3;  \
  const int xa0=x.getBase(0),  xa1=x.getBase(1);  \
  const int xb0=x.getBound(0), xb1=x.getBound(1);  \
  \
  switch( nd )  \
  {  \
  case 1:  \
    for( i0=xa0; i0<=xb0; i0++ )    \
    {    \
      x(i0) op value;    \
    }    \
    break;  \
  case 2:  \
    for( i1=xa1; i1<=xb1; i1++ )    \
      for( i0=xa0; i0<=xb0; i0++ )    \
      {    \
	x(i0,i1) op value;      \
      }    \
    break;  \
  case 3:  \
    for( i2=x.getBase(2); i2<=x.getBound(2); i2++ )    \
    for( i1=xa1; i1<=xb1; i1++ )    \
      for( i0=xa0; i0<=xb0; i0++ )    \
      {    \
	x(i0,i1,i2) op value;    \
      }    \
    break;  \
  case 4:  \
    for( i3=x.getBase(3); i3<=x.getBound(3); i3++ )    \
    for( i2=x.getBase(2); i2<=x.getBound(2); i2++ )    \
    for( i1=xa1; i1<=xb1; i1++ )    \
      for( i0=xa0; i0<=xb0; i0++ )    \
      {    \
	x(i0,i1,i2,i3) op value;    \
      }    \
    break;  \
  default:  \
    cout << "bitwise " #op "operator: ERROR: number of array dimensions =" << nd << endl;  \
    abort();  \
  }  \
  return x;  \
}

#ifdef USE_OLD_APP 
BITOP(|,intArray)
BITOP(&,intArray)
BITOP2(|=,intArray)
BITOP2(&=,intArray)
#ifdef USE_PPP 
BITOP(|,intSerialArray)
BITOP(&,intSerialArray)
BITOP2(|=,intSerialArray)
BITOP2(&=,intSerialArray)
#endif
#endif

#undef BITOP
#undef BITOP2
