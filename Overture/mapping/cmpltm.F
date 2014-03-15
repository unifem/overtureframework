      SUBROUTINE CMPLTM()
C
C   Print out the date this code was compiled.
C   Do this by including a header file which will be generated
C   by the Makefile.
C
#include "precis.h"
      CHARACTER*64 DATE
C
      DATE   = ' '
#include "cmpltm.h"
      WRITE(*,'(A/)') DATE
C
      END
