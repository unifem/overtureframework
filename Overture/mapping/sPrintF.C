#ifndef NO_APP
#include "GenericDataBase.h" 
#else
#include "GUITypes.h"
#endif

#include "wdhdefs.h"           // some useful defines and constants
// include <stdio.h>
#include <stdarg.h>
// include "aString.H"

char *
sPrintF(char *s, const char *format, ...)
// ======================================================================
/// \details 
///  Implementation of an "sprintf" like function that returns the formatted 
///   string s and NOT the number of chars assigned.\index{sPrintF}
///   
/// \param s (input) : fill in this string.
/// \param format (input) : use this printf style format.
/// \param argument `$\ldots$' (input): variable length argument list.
///  
/// \param author: wdh
// ======================================================================
{
  va_list args;
  va_start(args,format);
  vsprintf(s,format,args);
  va_end(args);
  return s;
}


aString &
sPrintF(aString & s, const char *format, ...)
// ======================================================================
/// \details 
///  Implementation of an "sprintf" like function that returns the formatted 
///   string s and NOT the number of chars assigned.\index{sPrintF}
///   
/// \param NOTE: this function assumes a maximum of 300 chars in the format string.
/// 
/// \param s (input) : fill in this string.
/// \param format (input) : use this printf style format.
/// \param argument `$\ldots$' (input): variable length argument list.
///  
/// \param author: wdh
// ======================================================================
{
  const int maxBuff=300;
  char buff[maxBuff];

  va_list args;
  va_start(args,format);
  int num=vsprintf(buff,format,args);
  va_end(args);

  // printf("sPrintF: num = %i \n",strlen(buff));
  
  if( num>maxBuff-2 )
  {
    printf("sPrintF:Possible ERROR: the buffer may not be long enough. Memory could be corrupted.\n");
    printf("sPrintF:format=%s\n",format);
  }
  

  s=buff;
  return s;
}


aString 
sPrintF(const char *format, ...)
// ======================================================================
/// \details 
///  Implementation of an "sprintf" like function that returns the formatted 
///   string NOT the number of chars assigned.\index{sPrintF}
///   
/// \param NOTE: this function assumes a maximum of 300 chars in the format string.
/// 
/// \param format (input) : use this printf style format.
/// \param argument `$\ldots$' (input): variable length argument list.
/// \return  formatted string
///  
/// \param author: wdh
// ======================================================================
{
  const int maxBuff=300;
  char buff[maxBuff];

  va_list args;
  va_start(args,format);
  int num=vsprintf(buff,format,args);
  va_end(args);

  // printf("sPrintF: num = %i \n",strlen(buff));
  
  if( num>maxBuff-2 )
  {
    printf("sPrintF:Possible ERROR: the buffer may not be long enough. Memory could be corrupted.\n");
    printf("sPrintF:format=%s\n",format);
  }
  
  aString s;
  s=buff;
  return s;
}



int 
sScanF(const aString & s, const char *format, 
       void *p0,
       void *p1 /* =NULL */, 
       void *p2 /* =NULL */, 
       void *p3 /* =NULL */,
       void *p4 /* =NULL */,
       void *p5 /* =NULL */,
       void *p6 /* =NULL */,
       void *p7 /* =NULL */,
       void *p8 /* =NULL */,
       void *p9 /* =NULL */,
       void *p10 /* =NULL */,
       void *p11 /* =NULL */,
       void *p12 /* =NULL */,
       void *p13 /* =NULL */,
       void *p14 /* =NULL */,
       void *p15 /* =NULL */,
       void *p16 /* =NULL */,
       void *p17 /* =NULL */,
       void *p18 /* =NULL */,
       void *p19 /* =NULL */,
       void *p20 /* =NULL */,
       void *p21 /* =NULL */,
       void *p22 /* =NULL */,
       void *p23 /* =NULL */,
       void *p24 /* =NULL */,
       void *p25 /* =NULL */,
       void *p26 /* =NULL */,
       void *p27 /* =NULL */,
       void *p28 /* =NULL */,
       void *p29 /* =NULL */
       )
// =====================================================================
/// \details 
///    A special version of sscanf that strips out any ',' characters
///  and replaces them with ' ' and converts the format string with ftor so that
///  \%e and \%f formats are converted properly for double precision.\index{sScanF}
/// \param p0,p1,... (input) : supply addresses of variables to save the results in.
// =======================================================================
{
  // first replace "," by a " "
  aString n;
  n = s;
  int i=0;
  for( i=0; i<n.length(); i++ )
  {
    if( n[i]==',' )
      n[i]=' ';
  }
  // printf("sScanF: s=%s \n"
  //        "        n=%s \n",(const char*)s,(const char*)n);
  return sscanf((const char*)n.c_str(),(const char*)(ftor(format)).c_str(),
		p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,
		p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,
                p20,p21,p22,p23,p24,p25,p26,p27,p28,p29);
}

int 
fScanF(FILE *file, const char *format, 
       void *p0,
       void *p1 /* =NULL */, 
       void *p2 /* =NULL */, 
       void *p3 /* =NULL */,
       void *p4 /* =NULL */,
       void *p5 /* =NULL */,
       void *p6 /* =NULL */,
       void *p7 /* =NULL */,
       void *p8 /* =NULL */,
       void *p9 /* =NULL */,
       void *p10 /* =NULL */,
       void *p11 /* =NULL */,
       void *p12 /* =NULL */,
       void *p13 /* =NULL */,
       void *p14 /* =NULL */,
       void *p15 /* =NULL */,
       void *p16 /* =NULL */,
       void *p17 /* =NULL */,
       void *p18 /* =NULL */,
       void *p19 /* =NULL */,
       void *p20 /* =NULL */,
       void *p21 /* =NULL */,
       void *p22 /* =NULL */,
       void *p23 /* =NULL */,
       void *p24 /* =NULL */,
       void *p25 /* =NULL */,
       void *p26 /* =NULL */,
       void *p27 /* =NULL */,
       void *p28 /* =NULL */,
       void *p29 /* =NULL */
       )
// =====================================================================
/// \details 
///    A special version of fscanf that strips out any ',' characters
///  and replaces them with ' ' and converts the format string with ftor so that
///  \%e and \%f formats are converted properly for double precision. \index{fScanF}
/// \param file (input) : scan this file.
/// \param format (input) : use this printf style format.
/// \param p0,p1,... (input) : supply addresses of variables to save the results in.
// =======================================================================
{
  return fscanf(file,(const char*)(ftor(format)).c_str(),
		p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,
		p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,
                p20,p21,p22,p23,p24,p25,p26,p27,p28,p29);
}



int 
getLine( char s[], int lim)
/// --------------------------------------------------------------------------------------
/// \details 
///   Read a line from standard input. \index{getLine}
/// \param s (input) : char array in which to store the line
/// \param lim (input) : maximum number of chars that can be saved in s
//-------------------------------------------------------------------------------------
{
  int c,i;
  for(i=0; i<lim-1 && (c=fgetc(stdin))!=EOF && c!='\n'; ++i)  // use fgetc instead of getchar for linux!
    s[i]=c;
  s[i]='\0';
    
  // printF("getLine: i=%i lim=%i s=[%s] c=[%c]\n",i,lim,s,c);
  // if( i==0 || ferror(stdin) !=0 )

  // *wdh* July 21, 2016 
  // When running in the background (using the system command from perl) and trying to read from the terminal it could
  // be that no chars are read -- this leads to an infinite loop -- we can check for this
  // as follows:  ( ferror(stdin) returns no error)
  if( i==0 )
  {  
    printF("getLine:ERROR return from fgetc(stdin) -- no characters read!\n");
    printF("This could happen when running in the background and trying to read from the terminal.\n");
    printf("Error occured in file %s, function %s, line %d.\n",__FILE__,__func__,__LINE__);
    abort();
  }
  
  return i;
}

int
getLine( aString &answer )
/// --------------------------------------------------------------------------------------
/// \details 
///   Read a line from standard input.\index{getLine}
/// \param s (input) : char array in which to store the line
/// \param lim (input) : maximum number of chars that can be saved in s
//-------------------------------------------------------------------------------------
{
  char buff[180];
  int returnValue = getLine(buff,sizeof(buff));
  answer=buff;
  return returnValue;
}

// *** NOTE *** the prototype is defined in wdhdefs.h
aString
ftor(const char *ss)
// ======================================================================
/// \details 
///  "float to real aString conversion" \index{ftor}
///  This function is used to convert arguments to sscanf and fscanf so they work
///  when OV\_USE\_DOUBLE is or is not defined. It will convert \%e to \%le and \%f to \%lf 
///  when OV\_USE\_DOUBLE is defined.
///   Usually one should use sScanF and fScanF
///    to have this done automatically so
///  there is no need to call ftor directly.
/// \param ss (input) : convert this string.
/// \param author: wdh
// ======================================================================
{
#ifndef OV_USE_DOUBLE
  // single precision case:
  return ss;
#else
  // convert e to le and f to lf
  aString n= ss;
  int k=0;
  while( k<n.length()-1 )
  {
    if( substring(n,k,k+1)=="%e" )
    {
      n=substring(n,0,k)+"l"+substring(n,k+1,n.length()); // kkc should this be length()-1 ??
      k+=2;
    }
    else if( substring(n,k,k+1)=="%f" )
    {
      n=substring(n,0,k)+"l"+substring(n,k+1,n.length()); // kkc should this be length()-1 ??
      k+=2;
    }
    else if( substring(n,k,k+1)=="%g" )
    {
      n=substring(n,0,k)+"l"+substring(n,k+1,n.length()); // kkc should this be length()-1 ??
      k+=2;
    }
    k++;
  }
  return n;
  
#endif
}


void
printF(const char *format, ...)
// ======================================================================
/// \details 
///     Implementation of an "printf" like function that only prints on processor 0
///  
/// \param s (input) : fill in this string.
/// \param format (input) : use this printf style format.
/// \param argument `$\ldots$' (input): variable length argument list.
///  
/// \param author: wdh
// ======================================================================
{
  #ifdef USE_PPP
    if( Communication_Manager::My_Process_Number > 0 )
      return;
  #endif

  va_list args;
  va_start(args,format);
  vprintf(format,args);
  va_end(args);
}

void
fPrintF(FILE *file, const char *format, ...)
// ======================================================================
/// \details 
///     Implementation of an "fprintf" like function that only prints on processor 0
///  
/// \param s (input) : fill in this string.
/// \param format (input) : use this printf style format.
/// \param argument `$\ldots$' (input): variable length argument list.
///  
/// \param author: wdh
// ======================================================================
{
  #ifdef USE_PPP
    if( Communication_Manager::My_Process_Number > 0 )
      return;
  #endif

  va_list args;
  va_start(args,format);
  vfprintf(file,format,args);
  va_end(args);
}


