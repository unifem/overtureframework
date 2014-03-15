      SUBROUTINE CELLPR(X,Y,Z,NJ,NK,XP,YP,ZP,IPRO,J,K,A,B,STATUS)
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
      PARAMETER (MITER=10, TOL=0.0002, TOL2=1.E-5, TOL3=0.005)
      DIMENSION AMAT(3,3),BMAT(3,3)
      LOGICAL SEXT
      PARAMETER (I1=1)
      PARAMETER (ZERO=0.,ONE=1.)
C
C   Some variables NEED to be REAL*8 for accurate transformations.
C
      REAL*8 X0,XA,XB,XG,XAB,XAG,XBG,XABG,
     &       Y0,YA,YB,YG,YAB,YAG,YBG,YABG,
     &       Z0,ZA,ZB,ZG,ZAB,ZAG,ZBG,ZABG,
     &       XH,YH,ZH
C
C   Flag to print diagnostic information.
C
      LOGICAL DIALOG
#ifdef DEBUG
      PARAMETER (DIALOG=.TRUE.)
#else
      PARAMETER (DIALOG=.FALSE.)
#endif
C
C
C   For IPRO=1,2,3, use a different scheme.
C
      IF (IPRO.GE.1 .AND. IPRO.LE.3) THEN
         CALL CELLPX(X,Y,Z,NJ,NK,XP,YP,ZP,IPRO,J,K,A,B,STATUS)
         GOTO 100
      ENDIF
C
C   Just project normal to the surface.
C
      J      = MAX(MIN(J,NJ-1),I1)
      K      = MAX(MIN(K,NK-1),I1)
      JP     = J+1
      KP     = K+1
      JSAV1  = J
      KSAV1  = K
      JSAV2  = 0
      KSAV2  = 0
      IDJ    = 1
      IDK    = 1
C
C   Initial guess for A,B,G (alpha, beta, gamma) is the middle of the
C   cell.
C
      A      = 0.5
      B      = 0.5
      G      = 0.5
      IF (DIALOG) WRITE(*,*) 'Starting cell index ',J,K
      IF (DIALOG) WRITE(*,*) 'Starting A,B,G=',A,B,G
C
C   Maximum number of steps before giving up.
C
      MSTEP  = MAX(NJ,NK)
      NSTEP  = 0
   20 CONTINUE
      NSTEP  = NSTEP+1
      ISTAT  = 0
      JP     = J+1
      KP     = K+1
C
      X0     = X(J,K,1)
      XA     = X(JP,K,1) - X(J,K,1)
      XB     = X(J,KP,1) - X(J,K,1)
      XG     = X(J,K,2) - X(J,K,1)
      XAB    = X(JP,KP,1) - X(J,KP,1) - XA
      XAG    = X(JP,K,2) - X(J,K,2) - XA
      XBG    = X(J,KP,2) - X(J,K,2) - XB
      XABG   = X(JP,KP,2) - X(J,KP,2) - X(JP,K,2) + X(J,K,2) - XAB
C
      Y0     = Y(J,K,1)
      YA     = Y(JP,K,1) - Y(J,K,1)
      YB     = Y(J,KP,1) - Y(J,K,1)
      YG     = Y(J,K,2) - Y(J,K,1)
      YAB    = Y(JP,KP,1) - Y(J,KP,1) - YA
      YAG    = Y(JP,K,2) - Y(J,K,2) - YA
      YBG    = Y(J,KP,2) - Y(J,K,2) - YB
      YABG   = Y(JP,KP,2) - Y(J,KP,2) - Y(JP,K,2) + Y(J,K,2) - YAB
C
      Z0     = Z(J,K,1)
      ZA     = Z(JP,K,1) - Z(J,K,1)
      ZB     = Z(J,KP,1) - Z(J,K,1)
      ZG     = Z(J,K,2) - Z(J,K,1)
      ZAB    = Z(JP,KP,1) - Z(J,KP,1) - ZA
      ZAG    = Z(JP,K,2) - Z(J,K,2) - ZA
      ZBG    = Z(J,KP,2) - Z(J,K,2) - ZB
      ZABG   = Z(JP,KP,2) - Z(J,KP,2) - Z(JP,K,2) + Z(J,K,2) - ZAB
C
      ITER   = 0
   30    CONTINUE
         ITER   = ITER+1
         XH     = X0 + XA*A + XB*B + XG*G
     &          + XAB*(A*B) + XAG*(A*G) + XBG*(B*G) + XABG*(A*(B*G))
         YH     = Y0 + YA*A + YB*B + YG*G
     &          + YAB*(A*B) + YAG*(A*G) + YBG*(B*G) + YABG*(A*(B*G))
         ZH     = Z0 + ZA*A + ZB*B + ZG*G
     &          + ZAB*(A*B) + ZAG*(A*G) + ZBG*(B*G) + ZABG*(A*(B*G))
C
         AMAT(1,1)= XA + XAB*B + XAG*G + XABG*(B*G)
         AMAT(2,1)= YA + YAB*B + YAG*G + YABG*(B*G)
         AMAT(3,1)= ZA + ZAB*B + ZAG*G + ZABG*(B*G)
C
         AMAT(1,2)= XB + XAB*A + XBG*G + XABG*(A*G)
         AMAT(2,2)= YB + YAB*A + YBG*G + YABG*(A*G)
         AMAT(3,2)= ZB + ZAB*A + ZBG*G + ZABG*(A*G)
C
         AMAT(1,3)= XG + XAG*A + XBG*B + XABG*(A*B)
         AMAT(2,3)= YG + YAG*A + YBG*B + YABG*(A*B)
         AMAT(3,3)= ZG + ZAG*A + ZBG*B + ZABG*(A*B)
C
         CALL INV3X3(AMAT,BMAT,ISTATI)
C
C   Check for a degenerate cell.
C
         IF (ISTATI.NE.0) THEN
            IF (DIALOG) WRITE(*,*) 'Degenerate cell at index ',J,K,L
C
C   See if we're at the edge of the cell.  If so, move away from the
C   (possibly degenerate) edge and recompute the matrix.
C
            AS = A
            BS = B
            GS = G
            IF (A.EQ.0.) A      = 0.01
            IF (A.EQ.1.) A      = 0.99
            IF (B.EQ.0.) B      = 0.01
            IF (B.EQ.1.) B      = 0.99
            IF (G.EQ.0.) G      = 0.01
            IF (G.EQ.1.) G      = 0.99
            IF (A.NE.AS .OR. B.NE.BS .OR. G.NE.GS) THEN
               GOTO 30
C
C   We're inside a cell and the transformation matrix is singular.
C   Move to the next cell and try again.
C
            ELSE
               A      = IDJ + 0.5
               B      = IDK + 0.5
               GOTO 40
            ENDIF
         ENDIF
C
         DX     = XP - XH
         DY     = YP - YH
         DZ     = ZP - ZH
         DA     = DX*BMAT(1,1) + DY*BMAT(1,2) + DZ*BMAT(1,3)
         DB     = DX*BMAT(2,1) + DY*BMAT(2,2) + DZ*BMAT(2,3)
         DG     = DX*BMAT(3,1) + DY*BMAT(3,2) + DZ*BMAT(3,3)
         A      = A + DA
         B      = B + DB
         G      = G + DG
         IF (ITER.EQ.1) THEN
            A1     = A
            B1     = B
            G1     = G
         ENDIF
         IF (DIALOG) WRITE(*,*) '  Iteration ',ITER,' A,B,G=',A,B,G
C
C   Check iteration error and branch out if it's small enough.
C
         ERRA   = ABS(DA)/MAX(ABS(A),ONE)
         ERRB   = ABS(DB)/MAX(ABS(B),ONE)
         ERRG   = ABS(DG)/MAX(ABS(G),ONE)
         ERRMAX = MAX(ERRA,ERRB,ERRG)
         IF (ERRMAX.LE.TOL2) GOTO 40
         IF (ITER.LT.MITER) GOTO 30
C
C   If the corrections are small, we will assume that we are
C   oscillating about a reasonable answer.  We'll even be
C   generous and call it converged.
C
         IF (ERRMAX.LE.TOL3) THEN
            IF (DIALOG) WRITE(*,*) 'Newton iteration is oscillating'
C
C   If we didn't converge, our first guess is our best (we're out of the
C   cell, to be sure).
C
         ELSE
            IF (DIALOG) WRITE(*,*) 'Newton iteration did not converge'
            ISTAT  = 1
            A      = A1
            B      = B1
            G      = G1
         ENDIF
C
   40 CONTINUE
C
C   The point is in this cell.
C
      IF (      (A.GE.-TOL .AND. A.LE.1.+TOL)
     &    .AND. (B.GE.-TOL .AND. B.LE.1.+TOL) ) THEN
         IF (DIALOG) WRITE(*,*) 'Match'
C
C   We've taken more steps then we're willing to wait...
C
      ELSE IF (NSTEP.GT.MSTEP) THEN
         ISTAT  = 1
         IF (DIALOG) WRITE(*,*) 'More than ',MSTEP,' steps'
C
C   Update our (J,K) guess, keeping it inbounds.
C
      ELSE
         JNEW   = J
         KNEW   = K
         IF (A.LT.0.) JNEW   = MAX(JNEW-1,I1)
         IF (A.GT.1.) JNEW   = MIN(JNEW+1,NJ-1)
         IF (B.LT.0.) KNEW   = MAX(KNEW-1,I1)
         IF (B.GT.1.) KNEW   = MIN(KNEW+1,NK-1)
         IF (DIALOG) WRITE(*,*) 'Try cell index ',JNEW,KNEW
C
C   Not repeating a previous point.  Use the new (J,K,L) and try
C   again.
C
         IF (      (JNEW.NE.JSAV1 .OR. KNEW.NE.KSAV1)
     &       .AND. (JNEW.NE.JSAV2 .OR. KNEW.NE.KSAV2) ) THEN
            JSAV2  = JSAV1
            KSAV2  = KSAV1
            JSAV1  = JNEW
            KSAV1  = KNEW
            IDJ    = SIGN(I1,JNEW-J)
            IDK    = SIGN(I1,KNEW-K)
            A      = MIN(MAX(A - (JNEW-J),ZERO),ONE)
            B      = MIN(MAX(B - (KNEW-K),ZERO),ONE)
            G      = MIN(MAX(G,ZERO),ONE)
            J      = JNEW
            K      = KNEW
            GOTO 20
C
C   We've been here before...
C
         ELSE
C
C   It seems to be outside the domain.  We would have to extrapolate to
C   find it.
C
            IF (DIALOG) WRITE(*,*) 'Extrapolate'
         ENDIF
      ENDIF
C
C   At this point, ISTAT=1 flags non-convergence. 
C   Check cell indices and (A,B) to set STATUS properly.
C
      SEXT   = .FALSE.
      IF (     (J.EQ.   1 .AND. A.LT.  -TOL)
     &    .OR. (J.EQ.NJ-1 .AND. A.GT.1.+TOL)
     &    .OR. (K.EQ.   1 .AND. B.LT.  -TOL)
     &    .OR. (K.EQ.NK-1 .AND. B.GT.1.+TOL) ) SEXT   = .TRUE.
C
C   There are cases where (J,K) is still in-bounds but (A,B) is outside
C   [0,1].  This may be the case if the point is not close to the
C   surface.
C
      IF (     (A.LT.-TOL .OR. A.GT.1.+TOL)
     &    .OR. (B.LT.-TOL .OR. B.GT.1.+TOL) ) SEXT   = .TRUE.
C
      STATUS = 0
      IF (ISTAT.NE.0) STATUS = 2
      IF (SEXT)       STATUS = 1
C
  100 CONTINUE
C
C
      RETURN
      END
