      SUBROUTINE TOPLGY(NJ,NK,X,Y,Z,JJP,JJM,KKP,KKM,
     &                  JPER,JAXSA,JAXSB,JSYMA,JSYMB,
     &                  KPER,KAXSA,KAXSB,KSYMA,KSYMB)
C
C   Check topology of surface grid and set +1/-1 index arrays
C   JJP,JJM,KKP,KKM.
C
#include "precis.h"
      LOGICAL JPER,JAXSA,JAXSB
      LOGICAL KPER,KAXSA,KAXSB
      DIMENSION X(NJ,NK),Y(NJ,NK),Z(NJ,NK)
      DIMENSION JJP(NJ),JJM(NJ),KKP(NK),KKM(NK)
C
      LOGICAL JSYMX,JSYMY,JSYMZ,KSYMX,KSYMY,KSYMZ
C
C
C   Check for periodic in J.
C
      JPER   = .TRUE.
      DO 10 K = 1,NK
         IF (X(1,K).NE.X(NJ,K) .OR. Y(1,K).NE.Y(NJ,K)
     &                         .OR. Z(1,K).NE.Z(NJ,K)) THEN
            JPER   = .FALSE.
            GOTO 20
         ENDIF
   10    CONTINUE
   20 CONTINUE
C
C   Check for periodic in K.
C
      KPER   = .TRUE.
      DO 30 J = 1,NJ
         IF (X(J,1).NE.X(J,NK) .OR. Y(J,1).NE.Y(J,NK)
     &                         .OR. Z(J,1).NE.Z(J,NK)) THEN
            KPER   = .FALSE.
            GOTO 40
         ENDIF
   30    CONTINUE
   40 CONTINUE
C
C   Check for axes in J.
C
      JAXSA  = .TRUE.
      J      = 1
      DO 110 K = 1,NK-1
         KP     = K+1
         IF (X(J,K).NE.X(J,KP) .OR. Y(J,K).NE.Y(J,KP)
     &                         .OR. Z(J,K).NE.Z(J,KP)) THEN
            JAXSA  = .FALSE.
            GOTO 120
         ENDIF
  110    CONTINUE
  120 CONTINUE
C
      JAXSB  = .TRUE.
      J      = NJ
      DO 130 K = 1,NK-1
         KP     = K+1
         IF (X(J,K).NE.X(J,KP) .OR. Y(J,K).NE.Y(J,KP)
     &                         .OR. Z(J,K).NE.Z(J,KP)) THEN
            JAXSB  = .FALSE.
            GOTO 140
         ENDIF
  130    CONTINUE
  140 CONTINUE
C
C   Check for axes in K.
C
      KAXSA  = .TRUE.
      K      = 1
      DO 150 J = 1,NJ-1
         JP     = J+1
         IF (X(J,K).NE.X(JP,K) .OR. Y(J,K).NE.Y(JP,K)
     &                         .OR. Z(J,K).NE.Z(JP,K)) THEN
            KAXSA  = .FALSE.
            GOTO 160
         ENDIF
  150    CONTINUE
  160 CONTINUE
C
      KAXSB  = .TRUE.
      K      = NK
      DO 170 J = 1,NJ-1
         JP     = J+1
         IF (X(J,K).NE.X(JP,K) .OR. Y(J,K).NE.Y(JP,K)
     &                         .OR. Z(J,K).NE.Z(JP,K)) THEN
            KAXSB  = .FALSE.
            GOTO 180
         ENDIF
  170    CONTINUE
  180 CONTINUE
C
C   Check for symmetry on J.
C
      JSYMX  = .TRUE.
      JSYMY  = .TRUE.
      JSYMZ  = .TRUE.
      J      = 1
      JR     = J+2
      DO 210 K = 1,NK
         IF (X(J,K).NE.X(JR,K)) JSYMX  = .FALSE.
         IF (Y(J,K).NE.Y(JR,K)) JSYMY  = .FALSE.
         IF (Z(J,K).NE.Z(JR,K)) JSYMZ  = .FALSE.
         IF (.NOT.((JSYMX.OR.JSYMY).OR.JSYMZ)) THEN
            JSYMA  = 0
            GOTO 220
         ENDIF
  210    CONTINUE
      IF      (JSYMX) THEN
         JSYMA  = 1
      ELSE IF (JSYMY) THEN
         JSYMA  = 2
      ELSE IF (JSYMZ) THEN
         JSYMA  = 3
      ENDIF
  220 CONTINUE
C
      JSYMX  = .TRUE.
      JSYMY  = .TRUE.
      JSYMZ  = .TRUE.
      J      = NJ
      JR     = J-2
      DO 230 K = 1,NK
         IF (X(J,K).NE.X(JR,K)) JSYMX  = .FALSE.
         IF (Y(J,K).NE.Y(JR,K)) JSYMY  = .FALSE.
         IF (Z(J,K).NE.Z(JR,K)) JSYMZ  = .FALSE.
         IF (.NOT.((JSYMX.OR.JSYMY).OR.JSYMZ)) THEN
            JSYMB  = 0
            GOTO 240
         ENDIF
  230    CONTINUE
      IF      (JSYMX) THEN
         JSYMB  = 1
      ELSE IF (JSYMY) THEN
         JSYMB  = 2
      ELSE IF (JSYMZ) THEN
         JSYMB  = 3
      ENDIF
  240 CONTINUE
C
C   Check for symmetry on K.
C
      KSYMX  = .TRUE.
      KSYMY  = .TRUE.
      KSYMZ  = .TRUE.
      K      = 1
      KR     = K+2
      DO 250 J = 1,NJ
         IF (X(J,K).NE.X(J,KR)) KSYMX  = .FALSE.
         IF (Y(J,K).NE.Y(J,KR)) KSYMY  = .FALSE.
         IF (Z(J,K).NE.Z(J,KR)) KSYMZ  = .FALSE.
         IF (.NOT.((KSYMX.OR.KSYMY).OR.KSYMZ)) THEN
            KSYMA  = 0
            GOTO 260
         ENDIF
  250    CONTINUE
      IF      (KSYMX) THEN
         KSYMA  = 1
      ELSE IF (KSYMY) THEN
         KSYMA  = 2
      ELSE IF (KSYMZ) THEN
         KSYMA  = 3
      ENDIF
  260 CONTINUE
C
      KSYMX  = .TRUE.
      KSYMY  = .TRUE.
      KSYMZ  = .TRUE.
      K      = NK
      KR     = K-2
      DO 270 J = 1,NJ
         IF (X(J,K).NE.X(J,KR)) KSYMX  = .FALSE.
         IF (Y(J,K).NE.Y(J,KR)) KSYMY  = .FALSE.
         IF (Z(J,K).NE.Z(J,KR)) KSYMZ  = .FALSE.
         IF (.NOT.((KSYMX.OR.KSYMY).OR.KSYMZ)) THEN
            KSYMB  = 0
            GOTO 280
         ENDIF
  270    CONTINUE
      IF      (KSYMX) THEN
         KSYMB  = 1
      ELSE IF (KSYMY) THEN
         KSYMB  = 2
      ELSE IF (KSYMZ) THEN
         KSYMB  = 3
      ENDIF
  280 CONTINUE
C
C   Set +1/-1 indexing arrays.
C
      DO 510 J = 1,NJ
         JJP(J)  = J+1
         JJM(J)  = J-1
  510    CONTINUE
      IF (JPER) THEN
         JJP(NJ) = 2
         JJM(1)  = NJ-1
      ELSE
         JJP(NJ) = NJ
         JJM(1)  = 1
      ENDIF
C
      DO 520 K = 1,NK
         KKP(K) = K+1
         KKM(K) = K-1
  520    CONTINUE
      IF (KPER) THEN
         KKP(NK) = 2
         KKM(1)  = NK-1
      ELSE
         KKP(NK) = NK
         KKM(1)  = 1
      ENDIF
C
C
      RETURN
      END
