#ifndef __KKC_GEOM__
#define __KKC_GEOM__

// AP: Is this really necessary? #include "Overture.h"
#include "aString.H" // defines true (!)
#include "mathutil.h" // defines min, max, etc.

#include "ArraySimple.h"
#include "Face.h"
#include "ShewchukPredicates.h"


#define USE_SARRAY

#ifndef USE_SARRAY
bool intersect2D(realArray const &a, realArray const &b, realArray const &c, realArray const &d, bool &isParallel);

#else

bool intersect2D(const ArraySimple<real> &a, const ArraySimple<real> &b, 
		 const ArraySimple<real> &c, const ArraySimple<real> &d, bool &isParallel);

bool intersect3D(const ArraySimpleFixed<real,3,3,1,1> &triVertices, 
		 const ArraySimpleFixed<real,3,1,1,1> &p1, 
		 const ArraySimpleFixed<real,3,1,1,1> &p2, 
		 bool &isParallel, real &angle, real ftol=0.0);

int
get_circle_center_on_plane(ArraySimple<real> const &p1, 
			   ArraySimple<real> const &p2, 
			   ArraySimple<real> const &p3, ArraySimple<real> &center);

#endif

#include "Geom_inline.C"

#endif
