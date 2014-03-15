      SUBROUTINE NRSTPT(XP,YP,ZP,JNRST,KNRST,NNRST,MNRST,DNRST,
     &                  X,Y,Z,NJ,NK)
C
C   Exhaustive search for the nearest point on a surface grid (X,Y,Z)
C   to a point (XP,YP,ZP).  Return the distance to the surface in DNRST.
C
C   Variables:
C    xp,yp,zp: "the point"
C    jnrst,knrst: indices of the nearest point on the surface
C    dnrst: distance between the target point and the nearest point
C    x,y,z: coordinates of surface points
C    nj,nk: dimensions of surface grid
C
#include "precis.h"
      DIMENSION X(NJ,NK),Y(NJ,NK),Z(NJ,NK)
      DIMENSION JNRST(MNRST),KNRST(MNRST)
C
C   Define a distance function.
C
      DIST2(AX,AY,AZ) = AX**2 + AY**2 + AZ**2
C
C
C   Set JNRST,KNRST here, otherwise if (XP,YP,ZP) =
C   (X(1,1),Y(1,1),Z(1,1)), JNRST,KNRST will be undefined.
C
      DX2MIN   = DIST2(XP-X(1,1),YP-Y(1,1),ZP-Z(1,1))
      JNRST(1) = 1
      KNRST(1) = 1
      NNRST    = 0
      DO 10 K = 1,NK
      DO 10 J = 1,NJ
         DX2      = DIST2(XP-X(J,K),YP-Y(J,K),ZP-Z(J,K))
         IF (DX2.LT.DX2MIN) THEN
            DX2MIN       = DX2
            NNRST        = 1
            JNRST(NNRST) = J
            KNRST(NNRST) = K
         ELSE IF (DX2.EQ.DX2MIN .AND. NNRST.LT.MNRST) THEN
            NNRST        = NNRST+1
            JNRST(NNRST) = J
            KNRST(NNRST) = K
         ENDIF
   10    CONTINUE
C
C   Return the minimum distance.
C
      DNRST    = SQRT(DX2MIN)
C
C
      RETURN
      END
