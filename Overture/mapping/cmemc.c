/*-----------------------------------------------------------------------------

      CMEMC.C  --  C memory management routines for use with FORTRAN.

                      Pieter G. Buning     13 December 1988


FORTRAN entry points:

  SUBROUTINE GETARR(LENGTH,ARRAY,INDEX)
  __REAL ARRAY(*)
    Return an index into ARRAY representing LENGTH words of available
    memory.  Calls Unix MALLOC(3C).
      LENGTH - Number of words of memory requested.
      ARRAY  - Base address such that ARRAY(INDEX) represents the first
               element of the new space.
      INDEX  - Returned index into ARRAY for the first element of the new
               space.  If space could not be allocated, zero is returned
               for INDEX.

  SUBROUTINE GETIAR(LENGTH,ARRAY,INDEX)
  __INTEGER ARRAY(*)
    Return an index into ARRAY representing LENGTH words of available
    memory.  Calls Unix MALLOC(3C).
      LENGTH - Number of words of memory requested.
      ARRAY  - Base address such that ARRAY(INDEX) represents the first
               element of the new space.
      INDEX  - Returned index into ARRAY for the first element of the new
               space.  If space could not be allocated, zero is returned
               for INDEX.

  SUBROUTINE FREARR(LENGTH,ARRAY,INDEX)
  __REAL ARRAY(*)
    Free space starting at ARRAY(INDEX), previously allocated by GETARR.
    Calls Unix FREE(3C).
      LENGTH - Number of words of memory to be freed.
      ARRAY  - Base address such that ARRAY(INDEX) represents the first
               element of the space to be freed.
      INDEX  - Index into ARRAY for the first element of the space to be
               freed.

  SUBROUTINE FREIAR(LENGTH,ARRAY,INDEX)
  __INTEGER ARRAY(*)
    Free space starting at ARRAY(INDEX), previously allocated by GETARR.
    Calls Unix FREE(3C).
      LENGTH - Number of words of memory to be freed.
      ARRAY  - Base address such that ARRAY(INDEX) represents the first
               element of the space to be freed.
      INDEX  - Index into ARRAY for the first element of the space to be
               freed.

  SUBROUTINE TYPCHK
    Checks length of a C pointer (void *) with the corresponding FORTRAN
    length __NCHPTR from forttype.h.  Also checks C types __int and
    __float with FORTRAN lengths __NCHPWD and __NCHINT, all defined in
    forttype.h.

-----------------------------------------------------------------------------*/


#include <stdlib.h>
#include "forttype.h"
#include "fortcall.h"



/*  GETARR - Return an index into REAL ARRAY representing LENGTH words of
             available memory.                                               */

#ifdef FORTCALL_TR_US
void getarr_(length, array, index)
#endif
#ifdef FORTCALL_UC
void GETARR(length, array, index)
#endif
#ifdef FORTCALL_LC
void getarr(length, array, index)
#endif
#ifdef FORTCALL_DECL_REVARG
fortran getarr(index, array, length)
#endif

__int *length;
void **index;
__float *array;

{
    size_t len;
    void *ptr, *malloc();
    long int iaddr_unit;
    __float test[2];

/*  Byte-addressable, word-addressable, or what?  */

    iaddr_unit = (long int)&test[1] - (long int)&test[0];

/*  Whatever the addressing unit, MALLOC should take len in bytes.  */

    len = (size_t)(*length * sizeof(__float));

/*  Call MALLOC.  */

    ptr = malloc(len);
    if (ptr == 0)
	*index = 0;
    else
	*index = (void *)(((long int)ptr - (long int)array)/iaddr_unit + 1);

    return;
}


/*  GETIAR - Return an index into INTEGER ARRAY representing LENGTH words of
             available memory.                                               */

#ifdef FORTCALL_TR_US
void getiar_(length, array, index)
#endif
#ifdef FORTCALL_UC
void GETIAR(length, array, index)
#endif
#ifdef FORTCALL_LC
void getiar(length, array, index)
#endif
#ifdef FORTCALL_DECL_REVARG
fortran getiar(index, array, length)
#endif

__int *length;
void **index;
__int *array;

{
    size_t len;
    void *ptr, *malloc();
    long int iaddr_unit;
    __int test[2];

/*  Byte-addressable, word-addressable, or what?  */

    iaddr_unit = (long int)&test[1] - (long int)&test[0];

/*  Whatever the addressing unit, MALLOC should take len in bytes.  */

    len = (size_t)(*length * sizeof(__int));

/*  Call MALLOC.  */

    ptr = malloc(len);
    if (ptr == 0)
	*index = 0;
    else
	*index = (void *)(((long int)ptr - (long int)array)/iaddr_unit + 1);

    return;
}


/*  FREARR - Free space starting at REAL ARRAY(INDEX), previously
             allocated by GETARR.                                                         */

#ifdef FORTCALL_TR_US
void frearr_(length, array, index)
#endif
#ifdef FORTCALL_UC
void FREARR(length, array, index)
#endif
#ifdef FORTCALL_LC
void frearr(length, array, index)
#endif
#ifdef FORTCALL_DECL_REVARG
fortran frearr(index, array, length)
#endif

__int *length;
void **index;
__float *array;

{
    void *ptr;
    long int iaddr_unit;
    __float test[2];

/*  Byte-addressable, word-addressable, or what?  */

    iaddr_unit = (long int)&test[1] - (long int)&test[0];

    ptr = (void *)(((long int)*index - 1)*iaddr_unit + (long int)array);

/*  Call FREE.  */

    free(ptr);

    return;
}


/*  FREIAR - Free space starting at INTEGER ARRAY(INDEX), previously
             allocated by GETARR.                                                         */

#ifdef FORTCALL_TR_US
void freiar_(length, array, index)
#endif
#ifdef FORTCALL_UC
void FREIAR(length, array, index)
#endif
#ifdef FORTCALL_LC
void freiar(length, array, index)
#endif
#ifdef FORTCALL_DECL_REVARG
fortran freiar(index, array, length)
#endif

__int *length;
void **index;
__int *array;

{
    void *ptr;
    long int iaddr_unit;
    __int test[2];

/*  Byte-addressable, word-addressable, or what?  */

    iaddr_unit = (long int)&test[1] - (long int)&test[0];

    ptr = (void *)(((long int)*index - 1)*iaddr_unit + (long int)array);

/*  Call FREE.  */

    free(ptr);

    return;
}


/*  TYPCHK - Check the length of a C pointer (void *) with the
corresponding FORTRAN length __NCHPTR from forttype.h.                       */

#ifdef FORTCALL_TR_US
void typchk_()
#endif
#ifdef FORTCALL_UC
void TYPCHK()
#endif
#ifdef FORTCALL_LC
void typchk()
#endif
#ifdef FORTCALL_DECL_REVARG
fortran typchk()
#endif

{
    int exit_flag;
    int c_pointer_length,fortran_pointer_length;
    int c_int_length,fortran_int_length;
    int c_float_length,fortran_float_length;

    exit_flag = 0;

    c_pointer_length       = sizeof(void *);
    fortran_pointer_length = __NCHPTR;

    c_int_length           = sizeof(__int);
    fortran_int_length     = __NCHINT;

    c_float_length         = sizeof(__float);
    fortran_float_length   = __NCHPWD;

/*  Check the pointer lengths.  */

    if (c_pointer_length != fortran_pointer_length) {
	printf("\n  ** ERROR FROM CMEMC ROUTINE TYPCHK **\n");
	printf(  "     FORTRAN AND C POINTER TYPES DO NOT MATCH\n");
	printf("\n     C POINTER LENGTH (sizeof(void *)) IS %d\n",
	       c_pointer_length);
	printf("\n     FORTRAN POINTER LENGTH (__NCHPTR, length of type ");
	printf(         "__POINTER from forttype.h) IS %d\n",
	       fortran_pointer_length);
	exit_flag = 1;
    }

/*  Check the int lengths.  */

    if (c_int_length != fortran_int_length) {
	printf("\n  ** ERROR FROM CMEMC ROUTINE TYPCHK **\n");
	printf(  "     FORTRAN AND C INTEGER TYPES DO NOT MATCH\n");
	printf("\n     C INTEGER LENGTH (sizeof(__int)) IS %d\n",
	       c_int_length);
	printf("\n     FORTRAN INTEGER LENGTH (__NCHINT, length of type ");
	printf(         "__INTEGER from forttype.h) IS %d\n",
	       fortran_int_length);
	exit_flag = 1;
    }

/*  Check the float lengths.  */

    if (c_float_length != fortran_float_length) {
	printf("\n  ** ERROR FROM CMEMC ROUTINE TYPCHK **\n");
	printf(  "     FORTRAN AND C FLOAT TYPES DO NOT MATCH\n");
	printf("\n     C FLOAT LENGTH (sizeof(__float)) IS %d\n",
	       c_float_length);
	printf("\n     FORTRAN REAL LENGTH (__NCHPWD, length of type ");
	printf(         "__REAL from forttype.h) IS %d\n",
	       fortran_float_length);
	exit_flag = 1;
    }

/*  If things do not match, we have to quit.  */

    if (exit_flag != 0) exit(exit_flag);

    return;
}

