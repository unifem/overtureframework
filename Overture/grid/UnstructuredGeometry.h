#ifndef __OV_UNSTRUCTURED_GEOMETRY_H__
#define __OV_UNSTRUCTURED_GEOMETRY_H__

#include "OvertureTypes.h"

class UnstructuredMapping;

namespace UnstructuredGeometry {
  void computeCellCenters(UnstructuredMapping &umap, realArray &c);
  void computeGeometry( UnstructuredMapping &umap, 
			realArray *cellNorm, realArray *edgeNorm, realArray *vertVol, realArray *cellVol, 
			realArray *subCellNorms, realArray *subCellVols );
}
  
  
#endif
