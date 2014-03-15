      SUBROUTINE RSTTOP(NJ,NK,NL,X,Y,Z,JJP,JJM,KKP,KKM,
     &                  JPER,JAXSA,JAXSB,JSYMA,JSYMB,
     &                  KPER,KAXSA,KAXSB,KSYMA,KSYMB)
C
C   Reset grid topology attributes and set +1/-1 index arrays
C   JJP,JJM,KKP,KKM.
C   (Axis conditions are not yet handled in a general way, but then,
C   neither are the other conditions.  Implied assumption is that L is
C   normal to the body surface.)
C
#include "precis.h"
      DIMENSION X(NJ,NK,NL),Y(NJ,NK,NL),Z(NJ,NK,NL)
      DIMENSION JJP(NJ),JJM(NJ),KKP(NK),KKM(NK)
      LOGICAL JPER,JAXSA,JAXSB
      LOGICAL KPER,KAXSA,KAXSB
C
      DIMENSION IREFL(3),IZERO(3)
C
C
C   Reset periodic conditions.
C
      IF (JPER) THEN
         DO 10 L = 1,NL
         DO 10 K = 1,NK
            X(NJ,K,L) = X(1,K,L)
            Y(NJ,K,L) = Y(1,K,L)
            Z(NJ,K,L) = Z(1,K,L)
   10       CONTINUE
      ENDIF
C
      IF (KPER) THEN
         DO 20 L = 1,NL
         DO 20 J = 1,NJ
            X(J,NK,L) = X(J,1,L)
            Y(J,NK,L) = Y(J,1,L)
            Z(J,NK,L) = Z(J,1,L)
   20       CONTINUE
      ENDIF
C
C   Reset symmetry conditions.
C
      IF (JSYMA.NE.0) THEN
         J         = 1
         JS        = J+1
         JR        = J+2
         IREFL(1)  = 1.
         IREFL(2)  = 1.
         IREFL(3)  = 1.
         IREFL(JSYMA)  = -1.
         IZERO(1)  = 1.
         IZERO(2)  = 1.
         IZERO(3)  = 1.
         IZERO(JSYMA)  = 0.
         DO 110 L = 1,NL
         DO 110 K = 1,NK
            X(J,K,L)  = X(JR,K,L)*IREFL(1)
            Y(J,K,L)  = Y(JR,K,L)*IREFL(2)
            Z(J,K,L)  = Z(JR,K,L)*IREFL(3)
            X(JS,K,L) = X(JS,K,L)*IZERO(1)
            Y(JS,K,L) = Y(JS,K,L)*IZERO(2)
            Z(JS,K,L) = Z(JS,K,L)*IZERO(3)
  110       CONTINUE
      ENDIF
C
      IF (JSYMB.NE.0) THEN
         J         = NJ
         JS        = J-1
         JR        = J-2
         IREFL(1)  = 1.
         IREFL(2)  = 1.
         IREFL(3)  = 1.
         IREFL(JSYMB)  = -1.
         IZERO(1)  = 1.
         IZERO(2)  = 1.
         IZERO(3)  = 1.
         IZERO(JSYMB)  = 0.
         DO 120 L = 1,NL
         DO 120 K = 1,NK
            X(J,K,L)  = X(JR,K,L)*IREFL(1)
            Y(J,K,L)  = Y(JR,K,L)*IREFL(2)
            Z(J,K,L)  = Z(JR,K,L)*IREFL(3)
            X(JS,K,L) = X(JS,K,L)*IZERO(1)
            Y(JS,K,L) = Y(JS,K,L)*IZERO(2)
            Z(JS,K,L) = Z(JS,K,L)*IZERO(3)
  120       CONTINUE
      ENDIF
C
      IF (KSYMA.NE.0) THEN
         K         = 1
         KS        = K+1
         KR        = K+2
         IREFL(1)  = 1.
         IREFL(2)  = 1.
         IREFL(3)  = 1.
         IREFL(KSYMA)  = -1.
         IZERO(1)  = 1.
         IZERO(2)  = 1.
         IZERO(3)  = 1.
         IZERO(KSYMA)  = 0.
         DO 130 L = 1,NL
         DO 130 J = 1,NJ
            X(J,K,L)  = X(J,KR,L)*IREFL(1)
            Y(J,K,L)  = Y(J,KR,L)*IREFL(2)
            Z(J,K,L)  = Z(J,KR,L)*IREFL(3)
            X(J,KS,L) = X(J,KS,L)*IZERO(1)
            Y(J,KS,L) = Y(J,KS,L)*IZERO(2)
            Z(J,KS,L) = Z(J,KS,L)*IZERO(3)
  130       CONTINUE
      ENDIF
C
      IF (KSYMB.NE.0) THEN
         K         = NK
         KS        = K-1
         KR        = K-2
         IREFL(1)  = 1.
         IREFL(2)  = 1.
         IREFL(3)  = 1.
         IREFL(KSYMB)  = -1.
         IZERO(1)  = 1.
         IZERO(2)  = 1.
         IZERO(3)  = 1.
         IZERO(KSYMB)  = 0.
         DO 140 L = 1,NL
         DO 140 J = 1,NJ
            X(J,K,L)  = X(J,KR,L)*IREFL(1)
            Y(J,K,L)  = Y(J,KR,L)*IREFL(2)
            Z(J,K,L)  = Z(J,KR,L)*IREFL(3)
            X(J,KS,L) = X(J,KS,L)*IZERO(1)
            Y(J,KS,L) = Y(J,KS,L)*IZERO(2)
            Z(J,KS,L) = Z(J,KS,L)*IZERO(3)
  140       CONTINUE
      ENDIF
C
C   Reset axis conditions.
C
      IF (JAXSA) THEN
         J         = 1
         DO 220 L = 1,NL
            XAXIS     = X(J,1,L)
            YAXIS     = Y(J,1,L)
            ZAXIS     = Z(J,1,L)
            IF (KSYMA.EQ.1) XAXIS     = 0.
            IF (KSYMA.EQ.2) YAXIS     = 0.
            IF (KSYMA.EQ.3) ZAXIS     = 0.
            IF (KSYMB.EQ.1) XAXIS     = 0.
            IF (KSYMB.EQ.2) YAXIS     = 0.
            IF (KSYMB.EQ.3) ZAXIS     = 0.
            DO 210 K = 1,NK
               X(J,K,L)  = XAXIS
               Y(J,K,L)  = YAXIS
               Z(J,K,L)  = ZAXIS
  210          CONTINUE
  220       CONTINUE
      ENDIF
C
      IF (JAXSB) THEN
         J         = NJ
         DO 240 L = 1,NL
            XAXIS     = X(J,1,L)
            YAXIS     = Y(J,1,L)
            ZAXIS     = Z(J,1,L)
            IF (KSYMA.EQ.1) XAXIS     = 0.
            IF (KSYMA.EQ.2) YAXIS     = 0.
            IF (KSYMA.EQ.3) ZAXIS     = 0.
            IF (KSYMB.EQ.1) XAXIS     = 0.
            IF (KSYMB.EQ.2) YAXIS     = 0.
            IF (KSYMB.EQ.3) ZAXIS     = 0.
            DO 230 K = 1,NK
               X(J,K,L)  = XAXIS
               Y(J,K,L)  = YAXIS
               Z(J,K,L)  = ZAXIS
  230          CONTINUE
  240       CONTINUE
      ENDIF
C
      IF (KAXSA) THEN
         K         = 1
         DO 260 L = 1,NL
            XAXIS     = X(1,K,L)
            YAXIS     = Y(1,K,L)
            ZAXIS     = Z(1,K,L)
            IF (JSYMA.EQ.1) XAXIS     = 0.
            IF (JSYMA.EQ.2) YAXIS     = 0.
            IF (JSYMA.EQ.3) ZAXIS     = 0.
            IF (JSYMB.EQ.1) XAXIS     = 0.
            IF (JSYMB.EQ.2) YAXIS     = 0.
            IF (JSYMB.EQ.3) ZAXIS     = 0.
            DO 250 J = 1,NJ
               X(J,K,L)  = XAXIS
               Y(J,K,L)  = YAXIS
               Z(J,K,L)  = ZAXIS
  250          CONTINUE
  260       CONTINUE
      ENDIF
C
      IF (KAXSB) THEN
         K         = NK
         DO 280 L = 1,NL
            XAXIS     = X(1,K,L)
            YAXIS     = Y(1,K,L)
            ZAXIS     = Z(1,K,L)
            IF (JSYMA.EQ.1) XAXIS     = 0.
            IF (JSYMA.EQ.2) YAXIS     = 0.
            IF (JSYMA.EQ.3) ZAXIS     = 0.
            IF (JSYMB.EQ.1) XAXIS     = 0.
            IF (JSYMB.EQ.2) YAXIS     = 0.
            IF (JSYMB.EQ.3) ZAXIS     = 0.
            DO 270 J = 1,NJ
               X(J,K,L)  = XAXIS
               Y(J,K,L)  = YAXIS
               Z(J,K,L)  = ZAXIS
  270          CONTINUE
  280       CONTINUE
      ENDIF
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
