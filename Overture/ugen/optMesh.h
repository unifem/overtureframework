#ifndef OV_OPTMESH_H
#define OV_OPTMESH_H

#include "MeshQuality.h"

class MetricEvaluator;
class UnstructuredMapping;

void optimize(UnstructuredMapping &umap, MetricEvaluator &metricEval);
void optimize_one(UnstructuredMapping &umap, UnstructuredMappingIterator &vert, MetricEvaluator &cf);

#endif
