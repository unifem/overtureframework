      SUBROUTINE OFFSUR(X,Y,Z,NJ,NK,SNX,SNY,SNZ)
C
C   Generate an offset surface to surface grid (X,Y,Z) using surface
C   normals (SNX,SNY,SNZ).
C
#include "precis.h"
      DIMENSION X(NJ,NK,2),Y(NJ,NK,2),Z(NJ,NK,2)
      DIMENSION SNX(NJ,NK),SNY(NJ,NK),SNZ(NJ,NK)
C
C   Define a distance function.
C
      DIST(AX,AY,AZ) = SQRT(AX**2 + AY**2 + AZ**2)
C
C
C   Find some average cell length to scale the surface normals by.
C
      DAVG   = 0.
      ND     = 0
      DO 10 K = 1,NK
      DO 10 J = 1,NJ
         JP     = J+1
         KP     = K+1
         IF (JP.LE.NJ) THEN
            DJ     = DIST(X(JP,K,1)-X(J,K,1),Y(JP,K,1)-Y(J,K,1),
     &                                       Z(JP,K,1)-Z(J,K,1))
            DAVG   = DAVG + DJ
            ND     = ND+1
         ENDIF
         IF (KP.LE.NK) THEN
            DK     = DIST(X(J,KP,1)-X(J,K,1),Y(J,KP,1)-Y(J,K,1),
     &                                       Z(J,KP,1)-Z(J,K,1))
            DAVG   = DAVG + DK
            ND     = ND+1
         ENDIF
   10    CONTINUE
      IF (ND.NE.0) THEN
         DAVG   = DAVG/ND
      ELSE
         DAVG   = 1.
      ENDIF
      DOFF   = 0.1*DAVG
C
C   Set up the offset grid surface.
C
      DO 20 K = 1,NK
      DO 20 J = 1,NJ
         X(J,K,2)  = X(J,K,1) + DOFF*SNX(J,K)
         Y(J,K,2)  = Y(J,K,1) + DOFF*SNY(J,K)
         Z(J,K,2)  = Z(J,K,1) + DOFF*SNZ(J,K)
   20    CONTINUE
C
C
      RETURN
      END
