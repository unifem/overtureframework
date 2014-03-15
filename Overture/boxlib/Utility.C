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
// $Id: Utility.C,v 1.2 2008/12/03 17:54:46 chand Exp $
//

#include <stdlib.h>
//kkc 081124 #include <iostream.h>
#include <iostream>
#include <string.h>
//kkc 081124 #include <fstream.h>
#include <fstream>
#include <ctype.h>
#include <unistd.h>

#include <REAL.H>
#include <Misc.H>
#include <BoxLib.H>
#include <Utility.H>

using namespace std;

#if !defined(BL_ARCH_CRAY)
//
// ------------------------------------------------------------
// Return current run-time.
// ------------------------------------------------------------
//
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/param.h>

//
// This doesn't seem to be defined on SunOS when using g++.
//
#if defined(__GNUG__) && defined(__sun) && defined(BL_SunOS)
extern "C" int gettimeofday (struct timeval*, struct timezone*);
#endif

double
Utility::second (double* t)
{
    struct tms buffer;

    times(&buffer);
    static long CyclesPerSecond = 0;
    if (CyclesPerSecond == 0)
    {
#if defined(_SC_CLK_TCK)
        CyclesPerSecond = sysconf(_SC_CLK_TCK);
        if (CyclesPerSecond == -1)
            BoxLib::Error("second(double*): sysconf() failed");
#elif defined(HZ)
        CyclesPerSecond = HZ;
#else
        CyclesPerSecond = 100;
        BoxLib::Warning("second(): sysconf(): default value of 100 for hz, worry about timings");
#endif
    }
    double dt = (buffer.tms_utime + buffer.tms_stime)/(1.0*CyclesPerSecond);
    if (t != 0)
        *t = dt;
    return dt;
}

double
Utility::wsecond (double* t)
{
    static double epoch = -1.0;
    struct timeval tp;
    if (epoch < 0.0)
    {
        if (gettimeofday(&tp, 0) != 0)
            BoxLib::Abort("wsecond(): gettimeofday() failed");
        epoch = tp.tv_sec + tp.tv_usec/1000000.0;
    }
    gettimeofday(&tp,0);
    double dt = tp.tv_sec + tp.tv_usec/1000000.0-epoch;
    if(t != 0)
        *t = dt;
    return dt;
}

#else

extern "C" double SECOND();
extern "C" double RTC();

double
Utility::second (double* t_)
{
    double t = SECOND();
    if (t_)
        *t_ = t;
    return t;
}

double
Utility::wsecond (double* t_)
{
    static double epoch = -1.0;
    if (epoch < 0.0)
    {
        epoch = RTC();
    }
    double t = RTC() - epoch;
    if (t_)
        *t_ = t;
    return t;
}
#endif /*!defined(BL_ARCH_CRAY)*/

//
// ------------------------------------------------------------
// Return true if argument is a non-zero length string of digits.
// ------------------------------------------------------------
//

bool
Utility::is_integer (const char* str)
{
    if (str == 0)
        return false;

    int len = strlen(str);

    if (len == 0)
        return false;
    else
    {
        for (int i = 0; i < len; i++)
            if (!isdigit(str[i]))
                return false;
        return true;
    }
}
