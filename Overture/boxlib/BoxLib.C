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
// $Id: BoxLib.C,v 1.5 2004/03/25 20:02:15 henshaw Exp $
//

// #include <strstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Added for Overture, 12.28.01 kkc
#include <OvertureDefine.h>
#ifdef __sgi
#include <iostream.h>
#else
#ifdef OV_USE_OLD_STL_HEADERS
#include <iostream.h>
#else
#include <iostream>
OV_USINGNAMESPACE(std);
#endif
#endif

#include <BoxLib.H>
#include <BLVERSION.H>

//
// The definition of our NULL string used as default argument.
//
const char* BoxLib::nullString = "";

#define bl_str(s)  # s
#define bl_xstr(s) bl_str(s)

//
// The definition of our version string.
//
const char * const BoxLib::version =

"boxlib version "
bl_xstr(BL_VERSION_MAJOR)
"."
bl_xstr(BL_VERSION_MINOR)
" built "
__DATE__
" at "
__TIME__;

#undef bl_str
#undef bl_xstr

//
// This is used by BoxLib::Error(), BoxLib::Abort(), and BoxLib::Assert()
// to ensure that when writing the message to stderr, that no additional
// heap-based memory is allocated.
//

static
void
write_to_stderr_without_buffering (const char* str)
{
    if (str)
    {
        //
        // Flush all buffers.
        //
        fflush(NULL);
        //
        // Add some `!'s and a newline to the string.
        //
        const char * const end = " !!!\n";
        fwrite(str, strlen(str), 1, stderr);
        fwrite(end, strlen(end), 1, stderr);
    }
}

void
BoxLib::Error (const char* msg)
{
    write_to_stderr_without_buffering(msg);
    abort();
}

void
BoxLib::Abort (const char* msg)
{
    write_to_stderr_without_buffering(msg);
    abort();
}

void
BoxLib::Warning (const char* msg)
{
    if (msg)
        cerr << msg << '!' << endl;
}

//
// A pre-allocated buffer used by BoxLib::Assert().
//
const int DIMENSION = 512;
static char buf[DIMENSION];

void
BoxLib::Assert (const char* EX,
                const char* file,
                int         line)
{
    //
    // Why declare this static?  Well, the expectation is that by declaring
    // it static, the space for it'll be pre-allocated, so that when this
    // function is called, only the appropriate constructor will need to
    // be called.  This way BoxLib::Assert() should work fine, even when
    // complaining about running out of memory, or some such nasty situation.
    //
/* ---- wdh changed for linux machine that choked.
    static ostrstream os(buf, DIMENSION);
    ostrstream os(buf, DIMENSION);

    os << "Assertion `"
       << EX
       << "' failed, "
       << "file \""
       << file
       << "\", "
       << "line "
       << line
       << ends;
       ---- */
    cout << "Assertion `"
       << EX
       << "' failed, "
       << "file \""
       << file
       << "\", "
       << "line "
       << line
       << ends;

    write_to_stderr_without_buffering(buf);

    abort();
}

void
BoxLib::OutOfMemory (const char* file,
                     int         line)
{
    BoxLib::Assert("operator new", file, line);
}
