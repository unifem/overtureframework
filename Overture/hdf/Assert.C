#define _ASSERT_C_ "$Id: Assert.C,v 1.3 1999/12/11 00:21:30 henshaw Exp $"

#include <iostream.h>
#include <Assert.H>
#include <stdlib.h>

void
_Assert(const char *EX, const char *file, int line)
{
    cerr << "Assertion " << EX << "failed: "
	 << "file \"" << file << "\", "
	 << "line " << line << endl;
    abort();
}
