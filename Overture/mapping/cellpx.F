      SUBROUTINE CELLPX(X,Y,Z,NJ,NK,XP,YP,ZP,IPRO,J,K,A,B,STATUS)
C
C   Find the projection of the point (XP,YP,ZP) on the grid (X,Y,Z) and
C   return its (J,K) cell number and (A,B) offset within the cell.
C   A two-layer grid is used to establish surface normals independent of
C   grid stretching.  For IPRO=1,2,3 project in the X,Y,Z-plane,
C   respectively.
C
C     STATUS=1 - Point lies outside the grid.
C     STATUS=2 - For some reason, the stencil walk and/or Newton
C                iteration did not converge.
C
#include "precis.h"
      DIMENSION X(NJ,NK,2),Y(NJ,NK,2),Z(NJ,NK,2)
      __INTEGER STATUS
C
      PARAMETER (NJI=2,NKI=1)
      DIMENSION XL(NJI),YL(NJI),ZL(NJI)
C
C
C   Create a line in the appropriate direction.
C
      XL(1)  = XP
      YL(1)  = YP
      ZL(1)  = ZP
      XL(2)  = XP
      YL(2)  = YP
      ZL(2)  = ZP
      IF      (IPRO.EQ.1) THEN
         XL(2)  = XL(2) + 1.
      ELSE IF (IPRO.EQ.2) THEN
         YL(2)  = YL(2) + 1.
      ELSE IF (IPRO.EQ.3) THEN
         ZL(2)  = ZL(2) + 1.
      ENDIF
C
C   Call INTSEC.
C
      JL     = 1
      KL     = 1
      CALL INTSEC(X,Y,Z,NJ,NK,XL,YL,ZL,NJI,NKI,
     &            J,K,JL,KL,A,B,G,XP,YP,ZP,ISTAT)
C
C   Translate INTSEC ISTAT into CELLPX STATUS.  We don't care if the
C   intersection was actually on the line.
C
      STATUS = 0
      IF (ISTAT.EQ.1 .OR. ISTAT.EQ.3) THEN
         STATUS = 1
      ELSE IF (ISTAT.EQ.4) THEN
         STATUS = 2
      ENDIF
C
C
      RETURN
      END
