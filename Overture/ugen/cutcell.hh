#ifndef __CUTCELL__
#define __CUTCELL__

#include "OvertureTypes.h"
#include "SquareMapping.h"
#include "ArraySimple.h"

enum CutCellNodeClassification {
  unclassifiedNode=-3,
  blankedNode,
  //  activeButTooCloseNode, 
  activeNode
};

void cutcell(SquareMapping &square, real dx, real dy,
	     intArray &faces, realArray &xyz,
	     intArray &mask);
#endif
