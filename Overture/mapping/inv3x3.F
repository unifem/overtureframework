      SUBROUTINE INV3X3(A,AINV,STATUS)
C
C   Invert the 3x3 matrix A.  If A is singular, do our best to find the
C   pseudo-inverse.
C
C      STATUS=1 - A has one dependent column.
C      STATUS=2 - A has two dependent columns.
C      STATUS=3 - A is zero.
C
#include "precis.h"
      DIMENSION A(3,3),AINV(3,3)
      __INTEGER STATUS
C
      DIMENSION TMP(3,3),WORK(3),S(3),E(3),U(3,3),V(3,3),SIU(3,3)
C
      STATUS= 0
C
      AINV(1,1)= A(2,2)*A(3,3) - A(3,2)*A(2,3)
      AINV(1,2)= A(3,2)*A(1,3) - A(1,2)*A(3,3)
      AINV(1,3)= A(1,2)*A(2,3) - A(2,2)*A(1,3)
C
      AINV(2,1)= A(3,1)*A(2,3) - A(2,1)*A(3,3)
      AINV(2,2)= A(1,1)*A(3,3) - A(3,1)*A(1,3)
      AINV(2,3)= A(2,1)*A(1,3) - A(1,1)*A(2,3)
C
      AINV(3,1)= A(2,1)*A(3,2) - A(3,1)*A(2,2)
      AINV(3,2)= A(3,1)*A(1,2) - A(1,1)*A(3,2)
      AINV(3,3)= A(1,1)*A(2,2) - A(2,1)*A(1,2)
C
      DET= A(1,1)*AINV(1,1) + A(2,1)*AINV(1,2) + A(3,1)*AINV(1,3)
C
C   Matrix is nonsingular.  Finish up AINV.
C
      IF (DET.NE.0.) THEN
         DET= 1./DET
         AINV(1,1)= AINV(1,1)*DET
         AINV(2,1)= AINV(2,1)*DET
         AINV(3,1)= AINV(3,1)*DET
         AINV(1,2)= AINV(1,2)*DET
         AINV(2,2)= AINV(2,2)*DET
         AINV(3,2)= AINV(3,2)*DET
         AINV(1,3)= AINV(1,3)*DET
         AINV(2,3)= AINV(2,3)*DET
         AINV(3,3)= AINV(3,3)*DET
C
C   Matrix is singular.  Do a singular value decomposition to construct the
C   pseudo-inverse.  Use LINPACK routine SSVDC.
C
      ELSE
         CALL COPY(9,A,TMP)
         CALL SSVDC(TMP,3,3,3,S,E,U,3,V,3,WORK,11,INFO)
         IF (S(1).EQ.0.) THEN
            STATUS= 3
            CALL ZERO(9,AINV)
            GOTO 10
         ENDIF
C
C              -1 T
C   Compute V S  U .
C
         S(1)= 1./S(1)
         IF (S(3)*S(1).LT.1.E-5) THEN
            STATUS= 1
            S(3)  = 0.
         ELSE
            S(3)  = 1./S(3)
         ENDIF
         IF (S(2)*S(1).LT.1.E-5) THEN
            STATUS= 2
            S(2)  = 0.
         ELSE
            S(2)  = 1./S(2)
         ENDIF
C
C   Start out assuming S is a diagonal matrix.
C
         SIU(1,1) = S(1)*U(1,1)
         SIU(2,1) = S(2)*U(1,2)
         SIU(3,1) = S(3)*U(1,3)
C
         SIU(1,2) = S(1)*U(2,1)
         SIU(2,2) = S(2)*U(2,2)
         SIU(3,2) = S(3)*U(2,3)
C
         SIU(1,3) = S(1)*U(3,1)
         SIU(2,3) = S(2)*U(3,2)
         SIU(3,3) = S(3)*U(3,3)
C
C   S is upper bidiagonal, with E as the super diagonal.
C
         IF (INFO.GE.1) THEN
            SIU(1,1) = SIU(1,1) - (E(1)*S(1)*S(2))*U(2,1)
            SIU(1,2) = SIU(1,2) - (E(1)*S(1)*S(2))*U(2,2)
            SIU(1,3) = SIU(1,3) - (E(1)*S(1)*S(2))*U(2,3)
         ENDIF
         IF (INFO.GE.2) THEN
            SIU(1,1) = SIU(1,1) + (E(1)*E(2)*S(1)*S(2)**2*S(3))*U(3,1)
            SIU(1,2) = SIU(1,2) + (E(1)*E(2)*S(1)*S(2)**2*S(3))*U(3,2)
            SIU(1,3) = SIU(1,3) + (E(1)*E(2)*S(1)*S(2)**2*S(3))*U(3,3)
C
            SIU(2,1) = SIU(2,1) - (E(2)*S(2)*S(3))*U(3,1)
            SIU(2,2) = SIU(2,2) - (E(2)*S(2)*S(3))*U(3,2)
            SIU(2,3) = SIU(2,3) - (E(2)*S(2)*S(3))*U(3,3)
         ENDIF
C
C               +       -1 T
C   Finish up  A   = V S  U .
C
         AINV(1,1)= V(1,1)*SIU(1,1) + V(1,2)*SIU(2,1) + V(1,3)*SIU(3,1)
         AINV(2,1)= V(2,1)*SIU(1,1) + V(2,2)*SIU(2,1) + V(2,3)*SIU(3,1)
         AINV(3,1)= V(3,1)*SIU(1,1) + V(3,2)*SIU(2,1) + V(3,3)*SIU(3,1)
C
         AINV(1,2)= V(1,1)*SIU(1,2) + V(1,2)*SIU(2,2) + V(1,3)*SIU(3,2)
         AINV(2,2)= V(2,1)*SIU(1,2) + V(2,2)*SIU(2,2) + V(2,3)*SIU(3,2)
         AINV(3,2)= V(3,1)*SIU(1,2) + V(3,2)*SIU(2,2) + V(3,3)*SIU(3,2)
C
         AINV(1,3)= V(1,1)*SIU(1,3) + V(1,2)*SIU(2,3) + V(1,3)*SIU(3,3)
         AINV(2,3)= V(2,1)*SIU(1,3) + V(2,2)*SIU(2,3) + V(2,3)*SIU(3,3)
         AINV(3,3)= V(3,1)*SIU(1,3) + V(3,2)*SIU(2,3) + V(3,3)*SIU(3,3)
      ENDIF
C
   10 CONTINUE
      RETURN
      END
