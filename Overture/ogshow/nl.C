#include "NameList.h"

int
main()
{
  ios::sync_with_stdio();
 
  NameList nl;  // create a NameList object
  // define some parameters:
  int itest;
  real a;
  int array[10], matrix[5][5][5];
  int value,i0,i1,i2,i3;
  intArray c(3,3,3,3);
  realArray d(3,3,3,3);
  // the array num will take values from the enum "Numbers" defined next
  intArray num(2,2); num=-1;
  enum Numbers{ zero, one, two } number;
  aString enumName[] = { "zero", "one", "two", "" }; // here are the names of the enum
  intArray n(3,3,3,3);

  printf(
   "Make changes to the following variables: \n"
   "itest           (int)\n"
   "a               (real)\n"
   "array[10]       (int)\n"
   "matrix[5][5][5] (int)\n"
   "c(3,3,3,3)      (intArray)\n"
   "d(3,3,3,3)      (realArray)\n"
   "num(2,2) :      (enumArray, `zero', `one', `two')\n"
   "n(enum)=value   (enum one of `zero', `one', `two')\n"
   );
  aString answer,name;
  for( ;; )
  {
    cout << "Enter changes to variables, exit to continue" << endl;
    cin >> answer;
    if( answer=="exit" ) break;

    nl.getVariableName( answer, name );   // parse the answer
    if( name=="itest" )
    {
      itest=nl.intValue(answer);  
      cout << "itest=" << itest << endl;
    }
    else if( name=="a" )
    {
      a=nl.realValue(answer);
      cout << "a=" << a << endl;
    }
    else if( name=="array" )
    {
      nl.intArrayValue(answer,value,i0);
      cout << "value = " << value << ", i0 = " << i0 << endl;
      array[i0]=value;
    }
    else if( name=="matrix" )
    {
      if( nl.intArrayValue(answer,value,i0,i1,i2) )
      {
        matrix[i0][i1][i2]=value;
        printf("matrix[%i][%i][%i]=%i \n",i0,i1,i2, matrix[i0][i1][i2]);
      }
    }
    else if( name=="c" )
      nl.getIntArray( answer,c );
    else if( name=="d" )
      nl.getRealArray( answer,d ); 
    else if( name=="num" )
    {
      nl.arrayEqualsName( answer,enumName,num,i0,i1,i2,i3 );
      printf(" num(%i,%i) = %i (=%s) \n",i0,i1,num(i0,i1),(const char*)enumName[num(i0,i1)]);
    }
    else if( name=="n" )
    {
      nl.arrayOfNameEqualsValue( answer,enumName,n,i0 );
      printf(" n(%i) = %i (n(%s)=%i) \n",i0,n(i0),(const char*)enumName[i0],n(i0));
    }
    else
      cout << "unknown response: [" << name << "]" << endl;
  }
  return 0;
}
