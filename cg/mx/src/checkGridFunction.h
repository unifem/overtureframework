#ifndef CHECK_GRID_FUNCION_H
#define CHECK_GRID_FUNCION_H

#include "Overture.h"

int
checkGridFunction(realGridCollectionFunction & u, const aString & title, bool printResults=true );

int
checkGridFunction(realMappedGridFunction & u, const aString & title, bool printResults=true, int grid=0 );

#endif
