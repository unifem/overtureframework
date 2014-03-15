#include <malloc.h>

int
bytesInUse()
{
  /* struct mallinfo mi;
  mi=mallinfo(); */
  return mallinfo().arena;
}
