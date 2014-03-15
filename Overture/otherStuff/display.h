#ifndef DISPLAY_FUNCIONS_H
#define DISPLAY_FUNCIONS_H

#include "DisplayParameters.h"
#include "GridFunction.h"

extern const aString nullString;

int display( const    intArray & x, const char *label= NULL, FILE *file = NULL, const char *format_=NULL, 
             const Index *Iv=NULL );
int display( const  floatArray & x, const char *label= NULL, FILE *file = NULL, const char *format_=NULL, 
             const Index *Iv=NULL );
int display( const doubleArray & x, const char *label= NULL, FILE *file = NULL, const char *format_=NULL, 
             const Index *Iv=NULL );

int display( const    intArray & x, const char *label, const char *format_, const Index *Iv=NULL );
int display( const  floatArray & x, const char *label, const char *format_, const Index *Iv=NULL );
int display( const doubleArray & x, const char *label, const char *format_, const Index *Iv=NULL );

// pass option arguments in the DisplayParameters object
int display( const    intArray & x, const char *label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL);
int display( const  floatArray & x, const char *label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL); 
int display( const doubleArray & x, const char *label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL); 

// display the mask from a MappedGrid:
int displayMask( const intArray & mask, const aString & label=nullString, FILE *file = NULL, 
             const Index *Iv=NULL);
int displayMask( const intArray & mask, const aString & label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL);

// display the equations in a coefficient matrix
int displayCoeff(realMappedGridFunction & coeff,
		 const aString & label,
		 FILE *file=stdout,
		 const aString format =nullString );

#ifdef USE_PPP
int display( const    intSerialArray & x, const char *label= NULL, FILE *file = NULL, const char *format_=NULL, 
             const Index *Iv=NULL );
int display( const  floatSerialArray & x, const char *label= NULL, FILE *file = NULL, const char *format_=NULL, 
             const Index *Iv=NULL );
int display( const doubleSerialArray & x, const char *label= NULL, FILE *file = NULL, const char *format_=NULL, 
             const Index *Iv=NULL );

int display( const    intSerialArray & x, const char *label, const char *format_, const Index *Iv=NULL );
int display( const  floatSerialArray & x, const char *label, const char *format_, const Index *Iv=NULL );
int display( const doubleSerialArray & x, const char *label, const char *format_, const Index *Iv=NULL );

int display( const    intSerialArray & x, const char *label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL);
int display( const  floatSerialArray & x, const char *label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL); 
int display( const doubleSerialArray & x, const char *label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL); 

// display the mask from a MappedGrid:
int displayMask( const intSerialArray & mask, const aString & label=nullString, FILE *file = NULL, 
             const Index *Iv=NULL);
int displayMask( const intSerialArray & mask, const aString & label, const DisplayParameters & displayParameters, 
             const Index *Iv=NULL);
#endif

#endif
