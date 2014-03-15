#ifndef GCMath_

//
// Who to blame:  Geoff Chesshire
//

#include "Types.h"

inline Int min0 (const Int& i, const Int& j)       { return i < j ? i : j; }
inline Int max0 (const Int& i, const Int& j)       { return i > j ? i : j; }
inline Float amin1(const Float& x, const Float& y) { return x < y ? x : y; }
inline Float amax1(const Float& x, const Float& y) { return x > y ? x : y; }
inline Int Iabs (const Int& i )                    { return i < 0 ? -i : i; }
inline Float Fabs (const Float& x )                { return x < 0 ? -x : x; }
inline Int Isign(const Int& i, const Int& j)
  { return j < 0 ? -Iabs(i) : Iabs(i); }
inline Float Fsign(const Float& x, const Float& y)
  { return y < 0 ? -Fabs(x) : Fabs(x); }

#define GCMath_
#endif // GCMath_
