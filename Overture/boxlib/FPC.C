/*
** This software is copyright (C) by the Lawrence Berkeley National
** Laboratory.  Permission is granted to reproduce this software for
** non-commercial purposes provided that this notice is left intact.
**  
** It is acknowledged that the U.S. Government has rights to this
** software under Contract DE-AC03-765F00098 between the U.S.  Department
** of Energy and the University of California.
**  
** This software is provided as a professional and academic contribution
** for joint exchange. Thus it is experimental, is provided ``as is'',
** with no warranties of any kind whatsoever, no support, no promise of
** updates, or printed documentation. By using this software, you
** acknowledge that the Lawrence Berkeley National Laboratory and Regents
** of the University of California shall have no liability with respect
** to the infringement of other copyrights by any part of this software.
**  
** For further information about this software, contact:
** 
**         Dr. John Bell
**         Bldg. 50D, Rm. 129,
**         Lawrence Berkeley National Laboratory
**         Berkeley, CA, 94720
**         jbbell@lbl.gov
*/

//
// $Id: FPC.C,v 1.3 2005/10/29 17:20:06 henshaw Exp $
//

#include <FPC.H>

//
// FP orders.
//
const int FPC::normal_float_order[]     = { 1, 2, 3, 4 };
const int FPC::reverse_float_order[]    = { 4, 3, 2, 1 };
const int FPC::reverse_float_order_2[]  = { 2, 1, 4, 3 };
const int FPC::normal_double_order[]    = { 1, 2, 3, 4, 5, 6, 7, 8 };
const int FPC::reverse_double_order[]   = { 8, 7, 6, 5, 4, 3, 2, 1 };
const int FPC::reverse_double_order_2[] = { 2, 1, 4, 3, 6, 5, 8, 7 };
const int FPC::cray_float_order[]       = { 1, 2, 3, 4, 5, 6, 7, 8 };

//
// Floating point formats.
//
const long FPC::ieee_float[]  = { 32L,  8L, 23L, 0L, 1L,  9L, 0L,   0x7FL };
const long FPC::ieee_double[] = { 64L, 11L, 52L, 0L, 1L, 12L, 0L,  0x3FFL };
const long FPC::cray_float[]  = { 64L, 15L, 48L, 0L, 1L, 16L, 1L, 0x4000L };


//
// Every copy of the library will have exactly one
// `nativeLongDescriptor' and `nativeRealDescriptor' compiled into it.
// Each machine on which BoxLib runs MUST have them defined below.
//


const
IntDescriptor&
FPC::NativeLongDescriptor ()
{
// *wdh* if defined(__alpha) || defined(__i486__) || defined(__i386__)
#if defined(__alpha) || defined(__i486__) || defined(__i386__) || defined(__x86_64__)
    static const IntDescriptor nld(sizeof(long), IntDescriptor::ReverseOrder);
#endif

#ifdef _CRAY1
    static const IntDescriptor nld(sizeof(long), IntDescriptor::NormalOrder);
#endif

#if defined(__sgi) || \
    defined(__sun) || \
    defined(_AIX)  || \
    defined(__hpux) || \
    defined(__ppc__)
    static const IntDescriptor  nld(sizeof(long), IntDescriptor::NormalOrder);
#endif

    return nld;
}

const
RealDescriptor&
FPC::NativeRealDescriptor ()
{
#if defined(__alpha) || defined(__i486__) || defined(__i386__) || defined(__x86_64__)
#ifdef BL_USE_FLOAT
    static const RealDescriptor nrd(ieee_float, reverse_float_order, 4);
#else
    static const RealDescriptor nrd(ieee_double, reverse_double_order, 8);
#endif
#endif

#ifdef _CRAY1
    static const RealDescriptor nrd(cray_float, cray_float_order, 8);
#endif

#if defined(__sgi) || \
    defined(__sun) || \
    defined(_AIX)  || \
    defined(__hpux) || \
    defined(__ppc__)
#ifdef BL_USE_FLOAT
    static const RealDescriptor nrd(ieee_float, normal_float_order, 4);
#else
    static const RealDescriptor nrd(ieee_double, normal_double_order, 8);
#endif
#endif

    return nrd;
}

const
RealDescriptor&
FPC::CrayRealDescriptor ()
{
    static const RealDescriptor crd(cray_float, cray_float_order, 8);
    return crd;
}

const
RealDescriptor&
FPC::Ieee32NormalRealDescriptor ()
{
    static const RealDescriptor i32rd(ieee_float, normal_float_order, 4);
    return i32rd;
}

const
RealDescriptor&
FPC::Ieee64NormalRealDescriptor ()
{
    static const RealDescriptor i64rd(ieee_double, normal_double_order, 8);
    return i64rd;
}


//
// TODO -- add more machine descriptions.
//
#if !(defined(__alpha)  || \
      defined(_CRAY1)   || \
      defined(__sgi)    || \
      defined(__sun)    || \
      defined(__i386__) || /* wdh */ \
      defined(__x86_64__) || /* wdh */ \
      defined(__i486__) || \
      defined(__hpux)   || \
      defined(_AIX)     || \
      defined(__ppc__))
#error We do not yet support FAB I/O on this machine
#endif

