      SUBROUTINE SNROLD(NJ,NK,X,Y,Z,SNX,SNY,SNZ,JJP,JJM,KKP,KKM)
C
C   Compute unit surface normal vector for surface points.
C
C   Variables:
C    x,y,z: coordinates of surface points
C    nj,nk: dimensions of surface grid
C    snx,sny,snz: surface normal components in the x,y and z directions
C
#include "precis.h"
      DIMENSION X(NJ,NK),Y(NJ,NK),Z(NJ,NK)
      DIMENSION SNX(NJ,NK),SNY(NJ,NK),SNZ(NJ,NK)
      DIMENSION JJP(NJ),JJM(NJ),KKP(NK),KKM(NK)
C
      LOGICAL JAXSA,JAXSB,KAXSA,KAXSB
      LOGICAL JPER,KPER
C
C   Define a distance function.
C
      DIST2(AX,AY,AZ) = AX**2 + AY**2 + AZ**2
C
C
C   Initialize axis and periodic flags.
C
      JAXSA    = .FALSE.
      JAXSB    = .FALSE.
      KAXSA    = .FALSE.
      KAXSB    = .FALSE.
      JPER     = JJM(1).NE.1
      KPER     = KKM(1).NE.1
C
      DO 10 K = 1,NK
      DO 10 J = 1,NJ
         JP       = JJP(J)
         JM       = JJM(J)
         KP       = KKP(K)
         KM       = KKM(K)
C
         DX1      = X(JP,K) - X(J,K)
         DY1      = Y(JP,K) - Y(J,K)
         DZ1      = Z(JP,K) - Z(J,K)
         D1LEN    = DIST2(DX1,DY1,DZ1)
         DX2      = X(J,KP) - X(J,K)
         DY2      = Y(J,KP) - Y(J,K)
         DZ2      = Z(J,KP) - Z(J,K)
         D2LEN    = DIST2(DX2,DY2,DZ2)
         DX3      = X(JM,K) - X(J,K)
         DY3      = Y(JM,K) - Y(J,K)
         DZ3      = Z(JM,K) - Z(J,K)
         D3LEN    = DIST2(DX3,DY3,DZ3)
         DX4      = X(J,KM) - X(J,K)
         DY4      = Y(J,KM) - Y(J,K)
         DZ4      = Z(J,KM) - Z(J,K)
         D4LEN    = DIST2(DX4,DY4,DZ4)
C
C   Axis at K=1 or NK.
C
         IF      (D1LEN.EQ.0. .AND. D3LEN.EQ.0.) THEN
            IF (K.EQ.NK) THEN
               KK       = KM
               KAXSB    = .TRUE.
            ELSE
               KK       = KP
               KAXSA    = .TRUE.
            ENDIF
            DX1      = X(JP,KK) - X(JP,K)
            DY1      = Y(JP,KK) - Y(JP,K)
            DZ1      = Z(JP,KK) - Z(JP,K)
            D1LEN    = DIST2(DX1,DY1,DZ1)
            DX3      = X(JM,KK) - X(JM,K)
            DY3      = Y(JM,KK) - Y(JM,K)
            DZ3      = Z(JM,KK) - Z(JM,K)
            D3LEN    = DIST2(DX3,DY3,DZ3)
C
C   Axis at J=1 or NJ.
C
         ELSE IF (D2LEN.EQ.0. .AND. D4LEN.EQ.0.) THEN
            IF (J.EQ.NJ) THEN
               JJ       = JM
               JAXSB    = .TRUE.
            ELSE
               JJ       = JP
               JAXSA    = .TRUE.
            ENDIF
            DX2      = X(JJ,KP) - X(J,KP)
            DY2      = Y(JJ,KP) - Y(J,KP)
            DZ2      = Z(JJ,KP) - Z(J,KP)
            D2LEN    = DIST2(DX2,DY2,DZ2)
            DX4      = X(JJ,KM) - X(J,KM)
            DY4      = Y(JJ,KM) - Y(J,KM)
            DZ4      = Z(JJ,KM) - Z(J,KM)
            D4LEN    = DIST2(DX4,DY4,DZ4)
         ENDIF
C
C   Compute a normalized normal vector for each surrounding quadrant.
C
         CALL NCRSPR(DX1,DY1,DZ1,DX2,DY2,DZ2,XV1,YV1,ZV1)
         CALL NCRSPR(DX2,DY2,DZ2,DX3,DY3,DZ3,XV2,YV2,ZV2)
         CALL NCRSPR(DX3,DY3,DZ3,DX4,DY4,DZ4,XV3,YV3,ZV3)
         CALL NCRSPR(DX4,DY4,DZ4,DX1,DY1,DZ1,XV4,YV4,ZV4)
C
C   Average the results.
C
         CALL NORMAL(XV1+XV2+XV3+XV4,YV1+YV2+YV3+YV4,ZV1+ZV2+ZV3+ZV4,
     &               SNX(J,K),SNY(J,K),SNZ(J,K),VLEN)
   10    CONTINUE
C
C   Recalculate corner normals to do the right thing for degenerate
C   cases.
C
      IF (.NOT.(JPER.OR.KPER)) THEN
         IF (.NOT.(JAXSA.OR.KAXSA)) THEN
            J        = 1
            K        = 1
            JP       = JJP(J)
            KP       = KKP(K)
            DX1      = X(JP,KP) - X(J,K)
            DY1      = Y(JP,KP) - Y(J,K)
            DZ1      = Z(JP,KP) - Z(J,K)
            DX2      = X(J,KP) - X(JP,K)
            DY2      = Y(J,KP) - Y(JP,K)
            DZ2      = Z(J,KP) - Z(JP,K)
            CALL NCRSPR(DX1,DY1,DZ1,DX2,DY2,DZ2,
     &                  SNX(J,K),SNY(J,K),SNZ(J,K))
         ENDIF
C
         IF (.NOT.(JAXSB.OR.KAXSA)) THEN
            J        = NJ
            K        = 1
            JM       = JJM(J)
            KP       = KKP(K)
            DX1      = X(J,KP) - X(JM,K)
            DY1      = Y(J,KP) - Y(JM,K)
            DZ1      = Z(J,KP) - Z(JM,K)
            DX2      = X(JM,KP) - X(J,K)
            DY2      = Y(JM,KP) - Y(J,K)
            DZ2      = Z(JM,KP) - Z(J,K)
            CALL NCRSPR(DX1,DY1,DZ1,DX2,DY2,DZ2,
     &                  SNX(J,K),SNY(J,K),SNZ(J,K))
         ENDIF
C
         IF (.NOT.(JAXSA.OR.KAXSB)) THEN
            J        = 1
            K        = NK
            JP       = JJP(J)
            KM       = KKM(K)
            DX1      = X(JP,K) - X(J,KM)
            DY1      = Y(JP,K) - Y(J,KM)
            DZ1      = Z(JP,K) - Z(J,KM)
            DX2      = X(J,K) - X(JP,KM)
            DY2      = Y(J,K) - Y(JP,KM)
            DZ2      = Z(J,K) - Z(JP,KM)
            CALL NCRSPR(DX1,DY1,DZ1,DX2,DY2,DZ2,
     &                  SNX(J,K),SNY(J,K),SNZ(J,K))
         ENDIF
C
         IF (.NOT.(JAXSB.OR.KAXSB)) THEN
            J        = NJ
            K        = NK
            JM       = JJM(J)
            KM       = KKM(K)
            DX1      = X(J,K) - X(JM,KM)
            DY1      = Y(J,K) - Y(JM,KM)
            DZ1      = Z(J,K) - Z(JM,KM)
            DX2      = X(JM,K) - X(J,KM)
            DY2      = Y(JM,K) - Y(J,KM)
            DZ2      = Z(JM,K) - Z(J,KM)
            CALL NCRSPR(DX1,DY1,DZ1,DX2,DY2,DZ2,
     &                  SNX(J,K),SNY(J,K),SNZ(J,K))
         ENDIF
      ENDIF
C
C
      RETURN
      END
