#include <stdio.h>
#include <stdarg.h>
#include "aString.H"

char *
sPrintF(char *s, const char *format, ...)
// Implementation of an "sprintf" like function that returns the formatted string s 
// author: wdh
{
  va_list args;
  va_start(args,format);
  vsprintf(s,format,args);
  va_end(args);
  return s;
}


const char *
ftor(const char *s)
// ======================================================================
// "float to real aString conversion"
// This function is used to convert arguments to sscanf so they work
// when DOUBLE is or is not defined.
// It will %e to %le and %f to %lf when DOUBLE is defined.
// author: wdh
// ======================================================================
{
#ifndef OV_USE_DOUBLE
  // single precision case:
  return s;
#else
  // convert e to le and f to lf
  char *ss, *p;
  aString n = s;
  int k=0;
  while( k<n.length()-1 )
  {
    if( n(k,k+1)=="%e" )
    {
      n=n(0,k)+"l"+n(k+1,n.length());
      k+=2;
    }
    else if( n(k,k+1)=="%f" )
    {
      n=n(0,k)+"l"+n(k+1,n.length());
      k+=2;
    }
    k++;
  }
  return (const char*) n;
#endif
}


//--------------------------------------------------------------------------------------
//  Read a line from standard input
//   char s[]   : char array in which to store the line
//   lim        : maximum number of chars that can be saved in s
//-------------------------------------------------------------------------------------
int 
getLine( char s[], int lim)
{
  int c,i;
  for(i=0; i<lim-1 && (c=fgetc(stdin))!=EOF && c!='\n'; ++i)  // use fgetc instead of getchar for linux!
    s[i]=c;
  s[i]='\0';
  return i;
}

int
getLine( aString &answer )
{
  char buff[180];
  int returnValue = getLine(buff,sizeof(buff));
  answer=buff;
  return returnValue;
}


int 
sScanF(aString & s, 
       const char *format,
       void *p0, 
       void *p1=NULL, 
       void *p2=NULL, 
       void *p3=NULL,
       void *p4=NULL,
       void *p5=NULL,
       void *p6=NULL,
       void *p7=NULL,
       void *p8=NULL,
       void *p9=NULL,
)
{
  
  int numberOfValuesRequired=0;

  if( p0!=NULL ) numberOfValuesRequired++;
  if( p1!=NULL ) numberOfValuesRequired++;
  if( p2!=NULL ) numberOfValuesRequired++;
  if( p3!=NULL ) numberOfValuesRequired++;
  
 int numberOfValuesRead = sscanf((const char*)s,format,p0,p1,p2,p3);
 printf(" numberOfValuesRead=%i \n",numberOfValuesRead);

 
  // convert e to le and f to lf
  char *ss, *p;
  aString n = s;
  int k=0;
  int i=0, iStart;
  while( k<format.length()-1 )
  {
    iStart=i;
    if( format(k,k+1)=="%e" )
    {
      // look for a float or double
      while( s[i]!=' ' && s[i]!=',' )
        i++      ;
      
      sscanf(s(iStart,i-1),"%e",p);
      
      k+=2;
    }
    else if( n(k,k+1)=="%f" )
    {
      // look for a float
      k+=2;
    }
    else if( n(k,k+1)=="%i" )
    {
      // look for a float
      k+=2;
    }

    k++;
  }
  return (const char*) n;


 return  numberOfValuesRead;
}



int main()
{
  

  aString answer;
  float x0,x1,x2;
  
  x0=x1=x2=0.;
  
  answer ="1. 2. 3.";
  sScanF(answer,"%e %e %e",&x0,&x1,&x2);
  
  printf(" x0=%e, x1=%e, x2=%e \n",x0,x1,x2);

  x0=x1=x2=0.;
  answer ="1., 2.,3.";
  sScanF(answer,"%e %e %e",&x0,&x1,&x2);

  printf(" x0=%e, x1=%e, x2=%e \n",x0,x1,x2);

  return 0;
}

