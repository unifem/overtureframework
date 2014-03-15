#include <malloc.h>

#include "fc.h"

Int dskcal_(disk0,disk1,size)
 Int *disk0, *disk1, *size; {
 Int *p = (Int*)malloc(*size * (disk1 - disk0) * sizeof(Int));
  return p ? ((p - disk0) / (disk1 - disk0) + 1) : 0;
}

void dskcfr_(disk0,disk1,block)
 Int *disk0, *disk1, *block; {
  free((char*)(disk0 + (*block - 1) * (disk1 - disk0)));
}
