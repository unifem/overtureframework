#ifndef MAPPING_GEOMETRY_H
#define MAPPING_GEOMETRY_H

#include "Mapping.h"


// ===================================================================================================
/// \brief Compute geomtrical properties (e.g. volume, surface area, moments of inertia) of Mappings.
// ===================================================================================================
class MappingGeometry
{
public:

  static void getGeometricProperties( Mapping & map, RealArray & rvalues, IntegerArray & ivalues );

  static void computeVolumeIntegrals(UnstructuredMapping & uns, RealArray & values);

};


#endif
