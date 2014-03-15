      SUBROUTINE SUBMMS(NJ,NK,NL,X,Y,Z,XMIN,XMAX,YMIN,YMAX,ZMIN,ZMAX)
C
C   Compute the minmax for a grid.
C
#include "precis.h"
      DIMENSION X(NJ*NK*NL), Y(NJ*NK*NL), Z(NJ*NK*NL)
C
C
      N      = NJ*NK*NL
      XMIN   = X(1)
      XMAX   = X(1)
      YMIN   = Y(1)
      YMAX   = Y(1)
      ZMIN   = Z(1)
      ZMAX   = Z(1)
      DO 10 I = 2,N
         XMIN   = MIN( XMIN, X(I) )
         XMAX   = MAX( XMAX, X(I) )
         YMIN   = MIN( YMIN, Y(I) )
         YMAX   = MAX( YMAX, Y(I) )
         ZMIN   = MIN( ZMIN, Z(I) )
         ZMAX   = MAX( ZMAX, Z(I) )
   10    CONTINUE
C
C
      RETURN
      END
