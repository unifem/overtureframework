#include "FortranIO.h"

#define FFOPEN    EXTERN_C_NAME(ffopen)
#define FFCLOSE   EXTERN_C_NAME(ffclose)

#define FFPRINTI  EXTERN_C_NAME(ffprinti)
#define FFPRINTF  EXTERN_C_NAME(ffprintf)
#define FFPRINTD  EXTERN_C_NAME(ffprintd)
#define FFPRINTC  EXTERN_C_NAME(ffprintc)
#define FFPRINTIA EXTERN_C_NAME(ffprintia)
#define FFPRINTFA EXTERN_C_NAME(ffprintfa)
#define FFPRINTDA EXTERN_C_NAME(ffprintda)
#define FFPRINTIFA EXTERN_C_NAME(ffprintifa)
#define FFPRINTIDA EXTERN_C_NAME(ffprintida)

#define FFREADI  EXTERN_C_NAME(ffreadi)
#define FFREADF  EXTERN_C_NAME(ffreadf)
#define FFREADD  EXTERN_C_NAME(ffreadd)
#define FFREADC  EXTERN_C_NAME(ffreadc)
#define FFREADIA EXTERN_C_NAME(ffreadia)
#define FFREADFA EXTERN_C_NAME(ffreadfa)
#define FFREADDA EXTERN_C_NAME(ffreadda)

extern "C"
{
  void FFOPEN(const int & io, const char *fileName, const char *fileForm, const char *fileStatus,
              const int len1, const int len2, const int len3);
  void FFCLOSE(const int & io);

  void FFPRINTI(const int & io, const int & i, const int & count);
  void FFPRINTF(const int & io, const float & i, const int & count);
  void FFPRINTD(const int & io, const double & i, const int & count);
  void FFPRINTC(const int & io, const char *c, const int len);

  void FFPRINTIA(const int & io, const int & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b);
  void FFPRINTFA(const int & io, const float & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b);
  void FFPRINTDA(const int & io, const double & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b);

  void FFPRINTIFA(const int & io, 
                 const int & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b,
                 const float & v, 
                 const int & m1a, const int & m1b,
                 const int & m2a, const int & m2b,
                 const int & m3a, const int & m3b,
                 const int & m4a, const int & m4b,
                 const int & md1a, const int & md1b,
                 const int & md2a, const int & md2b,
                 const int & md3a, const int & md3b,
                 const int & md4a, const int & md4b);
  
  void FFPRINTIDA(const int & io, 
                 const int & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b,
                 const double & v, 
                 const int & m1a, const int & m1b,
                 const int & m2a, const int & m2b,
                 const int & m3a, const int & m3b,
                 const int & m4a, const int & m4b,
                 const int & md1a, const int & md1b,
                 const int & md2a, const int & md2b,
                 const int & md3a, const int & md3b,
                 const int & md4a, const int & md4b);
  

  void FFREADI(const int & io, const int & i, const int & count);
  void FFREADF(const int & io, const float & i, const int & count);
  void FFREADD(const int & io, const double & i, const int & count);
  void FFREADC(const int & io, const char *c, const int len);

  void FFREADIA(const int & io, const int & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b);
  void FFREADFA(const int & io, const float & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b);
  void FFREADDA(const int & io, const double & u, 
                 const int & n1a, const int & n1b,
                 const int & n2a, const int & n2b,
                 const int & n3a, const int & n3b,
                 const int & n4a, const int & n4b,
                 const int & nd1a, const int & nd1b,
                 const int & nd2a, const int & nd2b,
                 const int & nd3a, const int & nd3b,
                 const int & nd4a, const int & nd4b);
  
};


//\begin{>FortranIOInclude.tex}{\subsection{constructor}}
FortranIO::
FortranIO()
// ================================================================================
// /Description:
//   Build a FortranIO object. 
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  io=-1;  // use this fortran unit, -1==file closed
}  

FortranIO::
~FortranIO()
{
  close();
}

//\begin{>>FortranIOInclude.tex}{\subsection{open}}
int FortranIO:: 
open(const aString & fileName, 
     const aString & fileForm, 
     const aString & fileStatus,
     const int & fortranUnitNumber /* =25 */ )
// ================================================================================
// /Description:
//   Open a fortran file with a fortran statement of the form:
// \begin{verbatim}
//    open (unit=io, file=fileName,form=fileForm,status=fileStatus)
// \end{verbatim}
// /fileName (input) : name of the file.
// /fileForm (input) : a valid fortran file format such as "formatted" or "unformatted". *** only "unformatted"
//  is currently supported.
// /fileStatus (input) : a valid file status such as "old", "new", "unknown"
// /fortranUnitNumber (input) : a positive integer.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  close();   // first close any open file
  io=fortranUnitNumber;    // use this fortran unit
  FFOPEN(io,(const char *)fileName,(const char *)fileForm,(const char *)fileStatus,
         strlen((const char *)fileName),strlen((const char *)fileForm),strlen((const char *)fileStatus));
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{close}}
int FortranIO:: 
close()
// ================================================================================
// /Description:
//   Close a fortran file.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  if( io> 0 )
    FFCLOSE(io);
  io=-1;
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( int )}}
int FortranIO::
print(const int & i)
// ================================================================================
// /Description:
//   Save an int in the file.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTI(io,i,1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( float )}}
int FortranIO::
print(const float & f)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTF(io,f,1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( double )}}
int FortranIO::
print(const double & d)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTD(io,d,1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( int* )}}
int FortranIO::
print(const int *a, const int & count)
// ================================================================================
// /Description:
//   Save an array of values.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTI(io,a[0],count);
  return 0;
}
//\begin{>>FortranIOInclude.tex}{\subsection{print( float* )}}
int FortranIO::
print(const float *a, const int & count)
// ================================================================================
// /Description:
//   Save an array of values.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTF(io,a[0],count);
  return 0;
}
//\begin{>>FortranIOInclude.tex}{\subsection{print( double* )}}
int FortranIO::
print(const double *a, const int & count)
// ================================================================================
// /Description:
//   Save an array of values.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTD(io,a[0],count);
  return 0;
}


//\begin{>>FortranIOInclude.tex}{\subsection{print( aString )}}
int FortranIO::
print(const aString & label )
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTC(io,(const char *)label,strlen((const char *)label));
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( intArray )}}
int FortranIO::
print(const intArray & u)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTIA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
  
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( floatArray )}}
int FortranIO::
print(const floatArray & u)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTFA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( doubleArray )}}
int FortranIO::
print(const doubleArray & u)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
/* ----
  printf("FortranIO::print(double): getDataPointer=%i, (%i,%i), (%i,%i), (%i,%i), (%i,%i); (%i,%i), (%i,%i), (%i,%i), (%i,%i)\n",
        u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    u.getBase(0)-u.getDataOffset(0),u.getRawDataSize(0)+u.getBase(0)-u.getDataOffset(0)-1,
	    u.getBase(1)-u.getDataOffset(1),u.getRawDataSize(1)+u.getBase(1)-u.getDataOffset(1)-1,
	    u.getBase(2)-u.getDataOffset(2),u.getRawDataSize(2)+u.getBase(2)-u.getDataOffset(2)-1,
	    u.getBase(3)-u.getDataOffset(3),u.getRawDataSize(3)+u.getBase(3)-u.getDataOffset(3)-1);
---- */

// --- *wdh* 000615:   u.getBase(0)-u.getDataOffset(0) == 0 I think
/* ---
  FFPRINTDA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    u.getBase(0)-u.getDataOffset(0),u.getRawDataSize(0)+u.getBase(0)-u.getDataOffset(0)-1,
	    u.getBase(1)-u.getDataOffset(1),u.getRawDataSize(1)+u.getBase(1)-u.getDataOffset(1)-1,
	    u.getBase(2)-u.getDataOffset(2),u.getRawDataSize(2)+u.getBase(2)-u.getDataOffset(2)-1,
	    u.getBase(3)-u.getDataOffset(3),u.getRawDataSize(3)+u.getBase(3)-u.getDataOffset(3)-1);
--- */
  FFPRINTDA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);

  return 0;
}




//\begin{>>FortranIOInclude.tex}{\subsection{print( intArray,floatArray )}}
int FortranIO::
print(const intArray & u, const floatArray & v)
// ================================================================================
// /Description:
//   Output an int and float array.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTIFA(io,*u.getDataPointer(),
	     u.getBase(0),u.getBound(0),
	     u.getBase(1),u.getBound(1),
	     u.getBase(2),u.getBound(2),
	     u.getBase(3),u.getBound(3),
	     0,u.getRawDataSize(0)-1,
	     0,u.getRawDataSize(1)-1,
	     0,u.getRawDataSize(2)-1,
	     0,u.getRawDataSize(3)-1,
	     *v.getDataPointer(),
	     v.getBase(0),v.getBound(0),
	     v.getBase(1),v.getBound(1),
	     v.getBase(2),v.getBound(2),
	     v.getBase(3),v.getBound(3),
	     0,v.getRawDataSize(0)-1,
	     0,v.getRawDataSize(1)-1,
	     0,v.getRawDataSize(2)-1,
	     0,v.getRawDataSize(3)-1 );
  return 0;
  
}

//\begin{>>FortranIOInclude.tex}{\subsection{print( intArray,doubleArray )}}
int FortranIO::
print(const intArray & u, const doubleArray & v)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFPRINTIDA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	     0,u.getRawDataSize(0)-1,
	     0,u.getRawDataSize(1)-1,
	     0,u.getRawDataSize(2)-1,
	     0,u.getRawDataSize(3)-1,
	     *v.getDataPointer(),
	     v.getBase(0),v.getBound(0),
	     v.getBase(1),v.getBound(1),
	     v.getBase(2),v.getBound(2),
	     v.getBase(3),v.getBound(3),
	     0,v.getRawDataSize(0)-1,
	     0,v.getRawDataSize(1)-1,
	     0,v.getRawDataSize(2)-1,
	     0,v.getRawDataSize(3)-1 );
  return 0;
  
}



//\begin{>>FortranIOInclude.tex}{\subsection{read( int )}}
int FortranIO::
read(const int & i)
// ================================================================================
// /Description:
//   Save an int in the file.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADI(io,i,1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{read( float )}}
int FortranIO::
read(const float & f)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADF(io,f,1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{read( double )}}
int FortranIO::
read(const double & d)
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADD(io,d,1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{read( int* )}}
int FortranIO::
read(const int *a, const int & count)
// ================================================================================
// /Description:
//   Save an array of values.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADI(io,a[0],count);
  return 0;
}
//\begin{>>FortranIOInclude.tex}{\subsection{read( float* )}}
int FortranIO::
read(const float *a, const int & count)
// ================================================================================
// /Description:
//   Save an array of values.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADF(io,a[0],count);
  return 0;
}
//\begin{>>FortranIOInclude.tex}{\subsection{read( double* )}}
int FortranIO::
read(const double *a, const int & count)
// ================================================================================
// /Description:
//   Save an array of values.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADD(io,a[0],count);
  return 0;
}


//\begin{>>FortranIOInclude.tex}{\subsection{read( aString )}}
int FortranIO::
read(const aString & label )
// ================================================================================
// /Description:
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADC(io,(const char *)label,strlen((const char *)label));
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{read( intArray )}}
int FortranIO::
read(const intArray & u)
// ================================================================================
// /Description:
//   Read in an array -- the array must be dimensioned to the correct size.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADIA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);

  return 0;
  
}

//\begin{>>FortranIOInclude.tex}{\subsection{read( floatArray )}}
int FortranIO::
read(const floatArray & u)
// ================================================================================
// /Description:
//   Read in an array -- the array must be dimensioned to the correct size.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADFA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
}

//\begin{>>FortranIOInclude.tex}{\subsection{read( doubleArray )}}
int FortranIO::
read(const doubleArray & u)
// ================================================================================
// /Description:
//   Read in an array -- the array must be dimensioned to the correct size.
//\end{FortranIOInclude.tex} 
// ================================================================================
{
  assert( io>0 );
  FFREADDA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
}



// **************************************************************************************



#ifdef USE_PPP
int FortranIO::
print(const intSerialArray & u)
// ================================================================================
// /Description:
// ================================================================================
{
  assert( io>0 );
  FFPRINTIA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
  
}

int FortranIO::
print(const floatSerialArray & u)
// ================================================================================
// /Description:
// ================================================================================
{
  assert( io>0 );
  FFPRINTFA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
}

int FortranIO::
print(const doubleSerialArray & u)
// ================================================================================
// /Description:
// ================================================================================
{
  assert( io>0 );
  FFPRINTDA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);

  return 0;
}




int FortranIO::
print(const intSerialArray & u, const floatSerialArray & v)
// ================================================================================
// /Description:
//   Output an int and float array.
// ================================================================================
{
  assert( io>0 );
  FFPRINTIFA(io,*u.getDataPointer(),
	     u.getBase(0),u.getBound(0),
	     u.getBase(1),u.getBound(1),
	     u.getBase(2),u.getBound(2),
	     u.getBase(3),u.getBound(3),
	     0,u.getRawDataSize(0)-1,
	     0,u.getRawDataSize(1)-1,
	     0,u.getRawDataSize(2)-1,
	     0,u.getRawDataSize(3)-1,
	     *v.getDataPointer(),
	     v.getBase(0),v.getBound(0),
	     v.getBase(1),v.getBound(1),
	     v.getBase(2),v.getBound(2),
	     v.getBase(3),v.getBound(3),
	     0,v.getRawDataSize(0)-1,
	     0,v.getRawDataSize(1)-1,
	     0,v.getRawDataSize(2)-1,
	     0,v.getRawDataSize(3)-1 );
  return 0;
  
}

int FortranIO::
print(const intSerialArray & u, const doubleSerialArray & v)
// ================================================================================
// /Description:
// ================================================================================
{
  assert( io>0 );
  FFPRINTIDA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	     0,u.getRawDataSize(0)-1,
	     0,u.getRawDataSize(1)-1,
	     0,u.getRawDataSize(2)-1,
	     0,u.getRawDataSize(3)-1,
	     *v.getDataPointer(),
	     v.getBase(0),v.getBound(0),
	     v.getBase(1),v.getBound(1),
	     v.getBase(2),v.getBound(2),
	     v.getBase(3),v.getBound(3),
	     0,v.getRawDataSize(0)-1,
	     0,v.getRawDataSize(1)-1,
	     0,v.getRawDataSize(2)-1,
	     0,v.getRawDataSize(3)-1 );
  return 0;
  
}

int FortranIO::
read(const intSerialArray & u)
// ================================================================================
// /Description:
//   Read in an array -- the array must be dimensioned to the correct size.
// ================================================================================
{
  assert( io>0 );
  FFREADIA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);

  return 0;
  
}

int FortranIO::
read(const floatSerialArray & u)
// ================================================================================
// /Description:
//   Read in an array -- the array must be dimensioned to the correct size.
// ================================================================================
{
  assert( io>0 );
  FFREADFA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
}

int FortranIO::
read(const doubleSerialArray & u)
// ================================================================================
// /Description:
//   Read in an array -- the array must be dimensioned to the correct size.
// ================================================================================
{
  assert( io>0 );
  FFREADDA(io,*u.getDataPointer(),
	    u.getBase(0),u.getBound(0),
	    u.getBase(1),u.getBound(1),
	    u.getBase(2),u.getBound(2),
	    u.getBase(3),u.getBound(3),
	    0,u.getRawDataSize(0)-1,
	    0,u.getRawDataSize(1)-1,
	    0,u.getRawDataSize(2)-1,
	    0,u.getRawDataSize(3)-1);
  return 0;
}

#endif
