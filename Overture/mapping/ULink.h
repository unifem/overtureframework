#ifndef OV_ULINK_H
#define OV_ULINK_H


#include "UnstructuredMapping.h"
#include "ArraySimple.h"

int unstructuredLink( UnstructuredMapping &umap,
		      ArraySimple< UnstructuredMappingAdjacencyIterator > &links,
		      const UnstructuredMappingIterator &linkFrom,
		      int nHops,
		      int minRefs,
		      int pathDim=-1, 
		      bool useGhost=true );
#endif
