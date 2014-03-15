c ** notes ** renamed routines to start with h to avoid conflicts with surgrd
c *** rhs, metbln, filtrx, step, initia, inigrd, inipar, tanhs, epsil, asinn, asihnn
c *** remember to change the return value in hepsil: epsil=eps -> hepsil=eps
c *** changed forttype.h to define D_PRECISION if DOUBLE is defined.

c *** adding include "precis.h" did not seem to work in place of -r8 -i4
      subroutine hypgen2(
     & IFORM )
c*wdh*
c* include "precis.h"
c     & ,IZSTRT,NZREG,NPZREG(NZREG),ZREG(NZREG),
c     & DZ0(NZREG),DZ1(NZREG),
c     & IBCJA,IBCJB,IBCKA,IBCKB,
c     & IVSPEC,EPSSS,ITSVOL,
c     & IMETH,SMU2,
c     & TIMJ,TIMK,
c     & IAXIS,EXAXIS,VOLRES   
c     & JMAX,KMAX,LDUM,
c     & JZS,KZS,LZS,
c     & X(JMAX,KMAX),Y(JMAX,KMAX),Z(JMAX,KMAX),
c     & ZETAVR(JMAX,KMAX,3),
c     & RR(JZS,KZS,LMAX),
c     & XW(M3D),YW(M3D),ZW(M3D) )





C***********************************************************************
C
C                         HYPGEN Version 2.0i
C
C               Author  : William M. Chan
C
C          Version Date : May, 96.
C
C
C    Version 1.0 of HYPGEN was derived from an earlier code HYGRID by
C    J. L. Steger
C
C  WMC
C   1.3a - Test for divide by zero in ANGMET2
C   1.3b - Variable initial spacing/zetamax (read in from file zetast.i)
C   1.3c - Put in new algorithm to do rotated metrics blending
C          (major mods in METBLN)
C   1.3d - Put in matching upper/lower cut bc by metric rotation and
C          floating at collapsed edge. Check and prevent divides by zero. 
C   2.0  - (1) Put in BC types to replace the old bc inputs.
C          (2) Put in new BC's for constant interior planes.
C          (3) Read in variable far field, initial and final spacing from
C              file zetavar.i (used for first L-region only).
C          (4) Complete re-write of subroutine ZSPACS.
C          (5) Read in axis params. only if axis bc is present,i.e. the last
C              line of the input file is not read if there is no axis bc.
C          (6) Do special smoothing at leading edge for collapsed edge bc
C          (7) Do constant interior planes bc like a 2D case but do all
C              parallel planes together. Fixed up indices/bugs for both
C              J and K.
C          (8) Do further smoothing in leading edge area of collapsed edge
C              (C and O topologies)
C          (9) Fixed bugs in BNDUNJ,K (bad divergence test and dot scaling)
C         (10) Fixed bug in indices in STORLP for double periodic case
C         (11) Put in option to read in user defined stretching function
C         (12) Fixed indices bug in STEP for corners update (floating, 2d,..)
C         (13) Removed parameter MXREGS, use M1D and TMP1 instead for
C              NPZREG, etc...
C         (14) Pass cmin,scalel instead of using common block.
C         (15) Do inquire on surf.i, zetavar.i, zetastr.i.
C         (16) Do auto detection of format for all input files. IFORM is
C              used for output file format control only.
C         (17) Trap zero initial spacing for IZSTRT=1 and make it uniform
C              spacing. Check for negative values/steps in zetavar.i and
C              zetastr.i files.
C
C  WMC
C  2.0a - Do initialization of ADXI, ADET in INITIA
C  2.0b - Allow const. pl. bc at collapsed singular point boundary
C  2.0c - Let DISSL continue to increase up to 1
C  2.0d - Initialize XW,YW,ZW to zero (pointed out by C.J. Woan)
C  2.0e - Auto output PLOT3D command files for viewing
C  2.0f - Mod. EPSIL to allow for eps near zero. Cleaned up OUTCOM.
C         Replaced HYPTAN routines with more robust new TANHS routines.
C         Fixed DISTO zero problem in INITIA.
C         Mod. command files to allow for formatted/unformatted output.
C  2.0g - Fixed bug related to extra planes needed for JZS,KZS,RR
C         for user-specified stretching.
C  2.0h - Changed IAXIS=2 option to become dimple smoothing option.
C  2.0i - Changed DISVAR to avoid computation of large numbers when
C         called by INITIA. Put explicit numbers in parameters statements
C         in all MIN and MAX's.
C
C***********************************************************************
C
C   This is a program that generates a 3D grid with hyperbolic p.d.e.'s.
C   Generalized coordinates xi, eta and zeta corresponding to grid
C   indices J, K and L are used. The basic equations consist of two
C   orthogonality relations w.r.t. zeta and one cell volume condition.
C   The marching direction is in zeta and an AF implicit scheme is used
C   on the LHS.
C
C The following boundary conditions are allowed in the J or K directions
C (1) Periodic
C (2) Reflected symmetry
C (3) Constant plane
C (4) Floating with splay option 
C (5) Axis (J-direction only, extrapolation with volume scaling,
C           or unstructured logic)
C (6) 2D grid
C
C   A hard copy of the user guide can be obtained from
C         William M. Chan
C         NASA Ames Research Center
C         Mail Stop 227-2
C         Moffett Field, CA 94035
C   (415) 604-6607
C
C   This program is evolved from a basic version (HYG3D) by J.L. Steger.
C   A collection of new algorithms and techniques to enhance the
C   robustness of the code has since been developed and implemented
C   by W.M. Chan. Earlier versions of the code include HYG3D6 and HYG3D8.
C
C***********************************************************************
C
c*wdh      PROGRAM HYPGEN
C
C***********************************************************************
C
C    Parameter list:
C
C     M3D = max number of points in 3D grid
C     M2D = max number of points in 2D slice (J-K plane)
C     M1D = max number of points in any one dimension 
C
C     Allow at least two extra points in each direction for M3D,M2D,M1D.
C     These parameters appear in the main program only.
C
C     --------------------------------------------
      PARAMETER (M3D=1781001, M2D=35001, M1D=401)
C     --------------------------------------------
C
C     IDIM and LNKDIM are used in the unstructured axis logic.
C     The default values should be good enough for most cases.
      PARAMETER (IDIM=1000, LNKDIM=4)
C
C     ==================================================================
C     Incore memory requirement approx. = 4*M3D + 80*M2D + 20*M1D 
C                                         + 20*IDIM
C     ==================================================================
C
      LOGICAL JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,J2D,
     >        KSYMA,KSYMB,KFLTA,KFLTB,K2D,VZETA
C
      DIMENSION XW(M3D),YW(M3D),ZW(M3D),RR(M3D)
C
      DIMENSION X(M2D),Y(M2D),Z(M2D),VOLM(M2D),VOL(M2D),SR(M2D,3)
      DIMENSION XX(M2D),YX(M2D),ZX(M2D),XE(M2D),YE(M2D),ZE(M2D),
     >          XZ(M2D),YZ(M2D),ZZ(M2D)
      DIMENSION XIDS(M2D),ETDS(M2D),
     >          ADXI(M2D),ADET(M2D),ADRXI(M2D),ADRET(M2D)
      DIMENSION DAREA(M2D),JKBAD(M2D,2)
      DIMENSION H(M2D,9),A(M2D,9),B(M2D,9),C(M2D,9),F(M2D,3)
      DIMENSION CAXI(M2D),CAET(M2D),CVEX(M2D),
     >          AFNXI(M2D),AFNET(M2D),BLN(M2D)
      DIMENSION TMP2(M2D,9)
C
      DIMENSION JJP(M1D),JJR(M1D),KKP(M1D),KKR(M1D)
      DIMENSION SPHI(M1D),R(M1D),LBAD(M1D),NBAD(M1D),DISSL(M1D),
     >          DLC(M1D),DAREAS(M1D)
      DIMENSION TMP1(M1D,9)
C
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3),
     >          JPLN1(3),KPLN1(3),PLNKAB(3)
C
C    Common statement for unstructured axis logic
      COMMON/UBASE/ LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM),
     >              DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),
     >              EXI(IDIM,3), EET(IDIM,3),
     >              XP(IDIM), YP(IDIM), ZP(IDIM),
     >              XS(IDIM), YS(IDIM), ZS(IDIM), NKKP(IDIM/10)
C
C    ----------------------------------------------------------------
C     Read input parameters, do checks and determine array dimensions
C    ----------------------------------------------------------------
       CALL HINIPAR(IFORM,IZSTRT,NZREG,
     >             TMP1(1,1),TMP1(1,2),TMP1(1,3),TMP1(1,4),J2D,K2D,
     >             JPER,KPER,JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,
     >             KSYMA,KSYMB,KFLTA,KFLTB,JPLNA,JPLNB,KPLNA,KPLNB,
     >             EXTJA,EXTJB,EXTKA,EXTKB,JPLN1,KPLN1,PLNKAB,
     >             IBCJA,IBCJB,IBCKA,IBCKB,IVSPEC,EPSSS,ITSVOL,
     >             IMETH,SMU2,TIMJ,TIMK,IAXIS,EXAXIS,VOLRES,
     >             M3D,M2D,M1D,IFMTSU,IFMTVA,IFMTST,VZETA,
     >             JDIM,KDIM,LDIM,JMAX,KMAX,LMAX,JZS,KZS)
C
C    ------------------------------------------------------------------
C     Initialize parameters, variables, arrays and read in surface grid
C    ------------------------------------------------------------------
       CALL HINITIA(JDIM,KDIM,M1D,M3D,JMAX,KMAX,LMAX,J1,J2,K1,K2,
     >             JA,JB,KA,KB,JJP,JJR,KKP,KKR,J2D,K2D,JPER,KPER,
     >             JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,
     >             KSYMA,KSYMB,KFLTA,KFLTB,JPLNA,JPLNB,KPLNA,KPLNB,
     >             EXTJA,EXTJB,EXTKA,EXTKB,JPLN1,KPLN1,PLNKAB,
     >             IBCJA,IBCJB,IBCKA,IBCKB,ISJA,ISJB,ISKA,ISKB,
     >             IZSTRT,NZREG,TMP1(1,1),TMP1(1,2),TMP1(1,3),TMP1(1,4),
     >             VZETA,JZS,KZS,
     >             IVSPEC,EPSSS,ITSVOL,IMETH,SMU2,
     >             IAXIS,EXAXIS,VOLRES,
     >             X,Y,Z,VOL,SR,SPHI,R,TMP2,XW,YW,ZW,
     >             XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,
     >             XIDS,ETDS,DISSL,ADXI,ADET,ADRXI,ADRET,
     >             LTRANS,LTEST,RJMAXM,RKMAXM,CMIN,SCALEL,
     >             DAREA,RR,RADIS,DTHET,DLC,DAREAS,
     >             JFLAGA,JFLAGB,KFLAGA,KFLAGB,ITLE,ITTE,LSLE,LSTE,
     >             JAOUT,JBOUT,KAOUT,KBOUT,IFMTSU,IFMTVA,IFMTST)
C
C     Store surface grid
       CALL STORLP(JDIM,KDIM,JMAX,KMAX,LMAX,J1,J2,K1,K2,1,
     >             JPER,KPER,X,Y,Z,XW,YW,ZW,PLNKAB)
C
C    ---------------------------------------------------------
C     March equations in zeta direction and store each L-plane
C    ---------------------------------------------------------
       DO 10 L=2,LMAX
C
        CALL HSTEP(JDIM,KDIM,M1D,JMAX,KMAX,LMAX,JA,JB,KA,KB,L,
     >            JJP,JJR,KKP,KKR,IBCJA,IBCJB,IBCKA,IBCKB,VZETA,
     >            J2D,K2D,JPER,KPER,ISJA,ISJB,ISKA,ISKB,
     >            JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,
     >            KSYMA,KSYMB,KFLTA,KFLTB,JPLNA,JPLNB,KPLNA,KPLNB,
     >            EXTJA,EXTJB,EXTKA,EXTKB,JPLN1,KPLN1,PLNKAB,
     >            SPHI,R,SMU2,TIMJ,TIMK,IVSPEC,EPSSS,ITSVOL,
     >            IAXIS,EXAXIS,VOLRES,X,Y,Z,VOLM,VOL,SR,
     >            XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,XW,YW,ZW,
     >            XIDS,ETDS,DISSL,ADXI,ADET,ADRXI,ADRET,
     >            IMETH,LTRANS,LTEST,RJMAXM,RKMAXM,CMIN,SCALEL,
     >            H,A,B,C,F,
     >            DAREA,RR,RADIS,DTHET,DLC,DAREAS,
     >            JFLAGA,JFLAGB,KFLAGA,KFLAGB,ITLE,ITTE,LSLE,LSTE,
     >            JAOUT,JBOUT,KAOUT,KBOUT,
     >            TMP2(1,1),TMP2(1,2),TMP2(1,3),TMP2(1,6),
     >            CAXI,CAET,CVEX,AFNXI,AFNET,BLN,
     >            TMP1(1,1),TMP1(1,2),TMP1(1,3),TMP1(1,4),TMP1(1,5),
     >            TMP1(1,6),TMP1(1,7),TMP1(1,8),TMP1(1,9) )
C
        CALL STORLP(JDIM,KDIM,JMAX,KMAX,LMAX,J1,J2,K1,K2,L,
     >              JPER,KPER,X,Y,Z,XW,YW,ZW,PLNKAB)
C
 10    CONTINUE
C
C   --------------------
C    Grid quality checks
C   --------------------
C    Tetrahedral volume decomposition
      CALL CHKVOL(M2D,JDIM,KDIM,LMAX,JAOUT,JBOUT,KAOUT,KBOUT,LBAD,NBAD,
     >            JKBAD,JJP,JJR,KKP,KKR,JPER,KPER,H,XW,YW,ZW)
C
C    Finite volume Jacobian
      CALL CHKFVJ(M2D,JDIM,KDIM,JMAX,KMAX,LMAX,JPER,KPER,
     >            JAOUT,JBOUT,KAOUT,KBOUT,JJP,JJR,KKP,KKR,LBAD,NBAD,
     >            JKBAD,XW,YW,ZW,H(1,1),H(1,2),H(1,3),H(1,4),
     >            H(1,5),H(1,6),H(1,7),TMP2(1,1),TMP2(1,4),TMP2(1,7))
C
C   --------------------------------------
C    Write out grid in PLOT3D whole format
C   --------------------------------------
      CALL OUTWGR(JDIM,KDIM,J1,J2,K1,K2,LMAX,IFORM,XW,YW,ZW)
      CALL OUTCOM(JDIM,KDIM,J1,J2,K1,K2,JMAX,KMAX,LMAX,IFORM,
     >            IBCJA,IBCJB,IBCKA,IBCKB,XW,YW,ZW)
C
      STOP
      END
C***********************************************************************
      SUBROUTINE ADDEDG(JDIM,KDIM,JMAX,KMAX,LMAX,X,Y,Z,ZETAVR,RR,
     >                  VZETA,IZSTRT,JZS,KZS,IBCJA,IBCJB,IBCKA,IBCKB,
     >                  ISJA,ISJB,JAOUT,JBOUT,KAOUT,KBOUT)
c*wdh*
c* include "precis.h"
C
      LOGICAL VZETA
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION ZETAVR(JDIM,KDIM,3), RR(JDIM,KDIM,LMAX)
C
C    Add one or more lines to grid and zetavr arrays by linear extrap.
C    at floating edges; and shift rr array if floating edges are present.
C    Nothing is done if the edge is a singular point or if the angle
C    between the xi and eta lines at the corner exceeds 135 degrees.
C
C    Compute cosine of angle at the four corners
       J = 1
       K = 1
       JP = J+1
       KP = K+1
       CA11 = COSAN( X(J,K),Y(J,K),Z(J,K),X(JP,K),Y(JP,K),Z(JP,K),
     >               X(J,KP),Y(J,KP),Z(J,KP) )
       J = 1
       K = KMAX
       JP = J+1
       KP = K-1
       CA1M = COSAN( X(J,K),Y(J,K),Z(J,K),X(JP,K),Y(JP,K),Z(JP,K),
     >               X(J,KP),Y(J,KP),Z(J,KP) )
       J = JMAX
       K = 1
       JP = J-1
       KP = K+1
       CAM1 = COSAN( X(J,K),Y(J,K),Z(J,K),X(JP,K),Y(JP,K),Z(JP,K),
     >               X(J,KP),Y(J,KP),Z(J,KP) )
       J = JMAX
       K = KMAX
       JP = J-1
       KP = K-1
       CAMM = COSAN( X(J,K),Y(J,K),Z(J,K),X(JP,K),Y(JP,K),Z(JP,K),
     >               X(J,KP),Y(J,KP),Z(J,KP) )
C
C    Set add edge test at 135 degrees
      CALIM = -0.5*SQRT(2.0)
C
C    Floating edges in J
C
      IF ( (IBCJA.LE.-1) .AND. (ISJA.EQ.0) ) THEN
C
       IF ((CA11.GE.CALIM).AND.(CA1M.GE.CALIM)) THEN
C
        WRITE(*,*)'Phantom edge method used at J=1'
        JAOUT = 2
         DO 11 J=JMAX,1,-1
         DO 11 K=1,KMAX
          X(J+1,K) = X(J,K)
          Y(J+1,K) = Y(J,K)
          Z(J+1,K) = Z(J,K)
 11      CONTINUE
         DO 12 K=1,KMAX
          X(1,K) = 2.0*X(2,K)-X(3,K)
          Y(1,K) = 2.0*Y(2,K)-Y(3,K)
          Z(1,K) = 2.0*Z(2,K)-Z(3,K)
 12      CONTINUE
C
        IF ( VZETA ) THEN
         DO 14 J=JMAX,1,-1
         DO 14 N=1,3
         DO 14 K=1,KMAX
          ZETAVR(J+1,K,N) = ZETAVR(J,K,N)
 14      CONTINUE
         DO 15 N=1,3
         DO 15 K=1,KMAX
          ZETAVR(1,K,N) = 2.0*ZETAVR(2,K,N)-ZETAVR(3,K,N)
 15      CONTINUE
        ENDIF
C
        IF ( (IZSTRT.EQ.-1) .AND. (JZS.EQ.JMAX) ) THEN
         DO 17 J=JZS,1,-1
         DO 17 K=1,KZS
         DO 17 L=1,LMAX
           RR(J+1,K,L) = RR(J,K,L)
 17      CONTINUE
         DO 18 K=1,KZS
         DO 18 L=1,LMAX
           RR(1,K,L) = RR(2,K,L)
 18      CONTINUE
         JZS = JZS+1
        ENDIF
C
        JMAX = JMAX + 1
        JBOUT = JMAX
C
       ENDIF
C
      ENDIF
C
      IF ( (IBCJB.LE.-1) .AND. (ISJB.EQ.0) ) THEN
C
       IF ((CAM1.GE.CALIM).AND.(CAMM.GE.CALIM)) THEN
C
        WRITE(*,*)'Phantom edge method used at J=JMAX'
          DO 21 K=1,KMAX
           X(JMAX+1,K) = 2.0*X(JMAX,K)-X(JMAX-1,K)
           Y(JMAX+1,K) = 2.0*Y(JMAX,K)-Y(JMAX-1,K)
           Z(JMAX+1,K) = 2.0*Z(JMAX,K)-Z(JMAX-1,K)
 21       CONTINUE
C
         IF ( VZETA ) THEN
          DO 22 N=1,3
          DO 22 K=1,KMAX
           ZETAVR(JMAX+1,K,N) = 2.0*ZETAVR(JMAX,K,N)-ZETAVR(JMAX-1,K,N)
 22       CONTINUE
         ENDIF
C
         IF ( (IZSTRT.EQ.-1) .AND. (JZS.EQ.JMAX) ) THEN
          DO 23 K=1,KZS
          DO 23 L=1,LMAX
            RR(JMAX+1,K,L) = RR(JMAX,K,L)
 23       CONTINUE
          JZS = JZS+1
         ENDIF
C
         JMAX = JMAX + 1
         JBOUT = JMAX - 1
C
       ENDIF
C
      ENDIF
C
C    Floating edges in K
C
      IF ( (IBCKA.LE.-1).AND.(CA11.GE.CALIM).AND.(CAM1.GE.CALIM) ) THEN
C
        WRITE(*,*)'Phantom edge method used at K=1'
        KAOUT = 2
         DO 31 K=KMAX,1,-1
         DO 31 J=1,JMAX
          X(J,K+1) = X(J,K)
          Y(J,K+1) = Y(J,K)
          Z(J,K+1) = Z(J,K)
 31      CONTINUE
         DO 32 J=1,JMAX
          X(J,1) = 2.0*X(J,2)-X(J,3)
          Y(J,1) = 2.0*Y(J,2)-Y(J,3)
          Z(J,1) = 2.0*Z(J,2)-Z(J,3)
 32      CONTINUE
C
        IF ( VZETA ) THEN
         DO 34 K=KMAX,1,-1
         DO 34 N=1,3
         DO 34 J=1,JMAX
          ZETAVR(J,K+1,N) = ZETAVR(J,K,N)
 34      CONTINUE
         DO 35 N=1,3
         DO 35 J=1,JMAX
          ZETAVR(J,1,N) = 2.0*ZETAVR(J,2,N)-ZETAVR(J,3,N)
 35      CONTINUE
        ENDIF
C
        IF ( (IZSTRT.EQ.-1) .AND. (KZS.EQ.KMAX) ) THEN
         DO 37 K=KZS,1,-1
         DO 37 J=1,JZS
         DO 37 L=1,LMAX
           RR(J,K+1,L) = RR(J,K,L)
 37      CONTINUE
         DO 38 J=1,JZS
         DO 38 L=1,LMAX
           RR(J,1,L) = RR(J,2,L)
 38      CONTINUE
         KZS = KZS+1
        ENDIF
C
        KMAX = KMAX + 1
        KBOUT = KMAX
C
      ENDIF
C
      IF ( (IBCKB.LE.-1).AND.(CA1M.GE.CALIM).AND.(CAMM.GE.CALIM) ) THEN
C
        WRITE(*,*)'Phantom edge method used at K=KMAX'
          DO 41 J=1,JMAX
           X(J,KMAX+1) = 2.0*X(J,KMAX)-X(J,KMAX-1)
           Y(J,KMAX+1) = 2.0*Y(J,KMAX)-Y(J,KMAX-1)
           Z(J,KMAX+1) = 2.0*Z(J,KMAX)-Z(J,KMAX-1)
 41       CONTINUE
C
         IF ( VZETA ) THEN
          DO 42 N=1,3
          DO 42 J=1,JMAX
           ZETAVR(J,KMAX+1,N) = 2.0*ZETAVR(J,KMAX,N)-ZETAVR(J,KMAX-1,N)
 42       CONTINUE
         ENDIF
C
         IF ( (IZSTRT.EQ.-1) .AND. (KZS.EQ.KMAX) ) THEN
          DO 43 J=1,JZS
          DO 43 L=1,LMAX
            RR(J,KMAX+1,L) = RR(J,KMAX,L)
 43       CONTINUE
          KZS = KZS+1
         ENDIF
C
         KMAX = KMAX + 1
         KBOUT = KMAX - 1
C
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ADDEXE(JDIM,KDIM,JMAX,KMAX,LMAX,JPLNA,KPLNA,
     >                  JPLN1,KPLN1,X,Y,Z,ZETAVR,RR,VZETA,IZSTRT,
     >                  JZS,KZS,IBCJA,IBCKA,JAOUT,JBOUT,KAOUT,KBOUT)
c*wdh*
c* include "precis.h"
C
      LOGICAL VZETA
      DIMENSION JPLNA(3),KPLNA(3),JPLN1(3),KPLN1(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION ZETAVR(JDIM,KDIM,3), RR(JDIM,KDIM,LMAX)
C
C    Add one or more lines to grid, zetavr, rr array by extrapolation
C    at boundary. This is only used for constant interior planes
C    boundary condition.
C
      IF (IBCJA.GT.20) THEN
C
        WRITE(*,*)'Phantom edge method used at J=1'
        JAOUT = 2
         DO 11 J=JMAX,1,-1
         DO 11 K=1,KMAX
          X(J+1,K) = X(J,K)
          Y(J+1,K) = Y(J,K)
          Z(J+1,K) = Z(J,K)
 11      CONTINUE
         DO 12 K=1,KMAX
c          X(1,K) = X(2,K)*JPLNA(1) + (2.*X(2,K)-X(3,K))*JPLN1(1)
c          Y(1,K) = Y(2,K)*JPLNA(2) + (2.*Y(2,K)-Y(3,K))*JPLN1(2)
c          Z(1,K) = Z(2,K)*JPLNA(3) + (2.*Z(2,K)-Z(3,K))*JPLN1(3)
          X(1,K) = 2.0*X(2,K)-X(3,K)
          Y(1,K) = 2.0*Y(2,K)-Y(3,K)
          Z(1,K) = 2.0*Z(2,K)-Z(3,K)
 12      CONTINUE
C
        IF ( VZETA ) THEN
         DO 14 J=JMAX,1,-1
         DO 14 N=1,3
         DO 14 K=1,KMAX
          ZETAVR(J+1,K,N) = ZETAVR(J,K,N)
 14      CONTINUE
         DO 15 N=1,3
         DO 15 K=1,KMAX
          ZETAVR(1,K,N) = ZETAVR(2,K,N)
 15      CONTINUE
        ENDIF
C
        IF ( (IZSTRT.EQ.-1) .AND. (JZS.EQ.JMAX) ) THEN
         DO 17 J=JZS,1,-1
         DO 17 K=1,KZS
         DO 17 L=1,LMAX
           RR(J+1,K,L) = RR(J,K,L)
 17      CONTINUE
         DO 18 K=1,KZS
         DO 18 L=1,LMAX
           RR(1,K,L) = RR(2,K,L)
 18      CONTINUE
         JZS = JZS+1
        ENDIF
C
        JMAX = JMAX + 1
        JBOUT = JMAX
C
        WRITE(*,*)'Phantom edge method used at J=JMAX'
          DO 21 K=1,KMAX
c           X(JMAX+1,K) = X(JMAX,K)*JPLNA(1) + 
c     >                   (2.0*X(JMAX,K)-X(JMAX-1,K))*JPLN1(1)
c           Y(JMAX+1,K) = Y(JMAX,K)*JPLNA(2) +
c     >                   (2.0*Y(JMAX,K)-Y(JMAX-1,K))*JPLN1(2)
c           Z(JMAX+1,K) = Z(JMAX,K)*JPLNA(3) +
c     >                   (2.0*Z(JMAX,K)-Z(JMAX-1,K))*JPLN1(3)
           X(JMAX+1,K) = 2.0*X(JMAX,K)-X(JMAX-1,K)
           Y(JMAX+1,K) = 2.0*Y(JMAX,K)-Y(JMAX-1,K)
           Z(JMAX+1,K) = 2.0*Z(JMAX,K)-Z(JMAX-1,K)
 21       CONTINUE
C
         IF ( VZETA ) THEN
          DO 22 N=1,3
          DO 22 K=1,KMAX
           ZETAVR(JMAX+1,K,N) = ZETAVR(JMAX,K,N)
 22       CONTINUE
         ENDIF
C
         IF ( (IZSTRT.EQ.-1) .AND. (JZS.EQ.JMAX) ) THEN
          DO 23 K=1,KZS
          DO 23 L=1,LMAX
            RR(JMAX+1,K,L) = RR(JMAX,K,L)
 23       CONTINUE
          JZS = JZS+1
         ENDIF
C
         JMAX = JMAX + 1
         JBOUT = JMAX - 1
C
      ENDIF
C
      IF (IBCKA.GT.20) THEN
C
        WRITE(*,*)'Phantom edge method used at K=1'
        KAOUT = 2
         DO 31 K=KMAX,1,-1
         DO 31 J=1,JMAX
          X(J,K+1) = X(J,K)
          Y(J,K+1) = Y(J,K)
          Z(J,K+1) = Z(J,K)
 31      CONTINUE
         DO 32 J=1,JMAX
c          X(J,1) = X(J,2)*KPLNA(1) + (2.*X(J,2)-X(J,3))*KPLN1(1)
c          Y(J,1) = Y(J,2)*KPLNA(2) + (2.*Y(J,2)-Y(J,3))*KPLN1(2)
c          Z(J,1) = Z(J,2)*KPLNA(3) + (2.*Z(J,2)-Z(J,3))*KPLN1(3)
          X(J,1) = 2.0*X(J,2)-X(J,3)
          Y(J,1) = 2.0*Y(J,2)-Y(J,3)
          Z(J,1) = 2.0*Z(J,2)-Z(J,3)
 32      CONTINUE
C
        IF ( VZETA ) THEN
         DO 34 K=KMAX,1,-1
         DO 34 N=1,3
         DO 34 J=1,JMAX
          ZETAVR(J,K+1,N) = ZETAVR(J,K,N)
 34      CONTINUE
         DO 35 N=1,3
         DO 35 J=1,JMAX
          ZETAVR(J,1,N) = ZETAVR(J,2,N)
 35      CONTINUE
        ENDIF
C
        IF ( (IZSTRT.EQ.-1) .AND. (KZS.EQ.KMAX) ) THEN
         DO 37 K=KZS,1,-1
         DO 37 J=1,JZS
         DO 37 L=1,LMAX
           RR(J,K+1,L) = RR(J,K,L)
 37      CONTINUE
         DO 38 J=1,JZS
         DO 38 L=1,LMAX
           RR(J,1,L) = RR(J,2,L)
 38      CONTINUE
         KZS = KZS+1
        ENDIF
C
        KMAX = KMAX + 1
        KBOUT = KMAX
C
        WRITE(*,*)'Phantom edge method used at K=KMAX'
          DO 41 J=1,JMAX
c           X(J,KMAX+1) = X(J,KMAX)*KPLNA(1) + 
c     >                   (2.0*X(J,KMAX)-X(J,KMAX-1))*KPLN1(1)
c           Y(J,KMAX+1) = Y(J,KMAX)*KPLNA(2) + 
c     >                   (2.0*Y(J,KMAX)-Y(J,KMAX-1))*KPLN1(2)
c           Z(J,KMAX+1) = Z(J,KMAX)*KPLNA(3) + 
c     >                   (2.0*Z(J,KMAX)-Z(J,KMAX-1))*KPLN1(3)
           X(J,KMAX+1) = 2.0*X(J,KMAX)-X(J,KMAX-1)
           Y(J,KMAX+1) = 2.0*Y(J,KMAX)-Y(J,KMAX-1)
           Z(J,KMAX+1) = 2.0*Z(J,KMAX)-Z(J,KMAX-1)
 41       CONTINUE
C
         IF ( VZETA ) THEN
          DO 42 N=1,3
          DO 42 J=1,JMAX
           ZETAVR(J,KMAX+1,N) = ZETAVR(J,KMAX,N)
 42       CONTINUE
         ENDIF
C
         IF ( (IZSTRT.EQ.-1) .AND. (KZS.EQ.KMAX) ) THEN
          DO 43 J=1,JZS
          DO 43 L=1,LMAX
            RR(J,KMAX+1,L) = RR(J,KMAX,L)
 43       CONTINUE
          KZS = KZS+1
         ENDIF
C
         KMAX = KMAX + 1
         KBOUT = KMAX - 1
C
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ADD3D(JDIM,KDIM,JMAX,KMAX,IBCJA,IBCJB,IBCKA,IBCKB,
     >                VZETA,X,Y,Z,PLNKAB,ZETAVR,JAOUT,JBOUT,KAOUT,KBOUT)
c*wdh*
c* include "precis.h"
C
      LOGICAL VZETA
      DIMENSION X(JDIM,KDIM), Y(JDIM,KDIM), Z(JDIM,KDIM), PLNKAB(3)
      DIMENSION ZETAVR(JDIM,KDIM,3)
C
C   --------------------------------------------------------------------
C    Add edges on each side of a 2D line to make it look like a 3D case
C    and adjust zetavr array appropriately
C   --------------------------------------------------------------------
C
      PLNKAB(1) = 0.0
      PLNKAB(2) = 0.0
      PLNKAB(3) = 0.0
C
C    Two-D plane in J
      IF (JMAX.EQ.1) THEN
C
C      Do some input checks and copy grid to 3 planes
        IF (IBCJA.NE.IBCJB) THEN
         WRITE(*,*)'Error: Inconsistent conditions on constant ',
     >             'J-boundary plane'
         STOP
        ENDIF
        IF ( (IBCJA.LE.0) .OR. (IBCJA.GE.4) ) THEN
         WRITE(*,*)'Error: Constant plane conditions not activated ',
     >             'for 2D case'
         STOP
        ENDIF
        JMAX  = 3
        JAOUT = 2
        JBOUT = 2
        DO 20 J=2,3
        DO 20 K=1,KMAX
         X(J,K) = X(1,K)
         Y(J,K) = Y(1,K)
         Z(J,K) = Z(1,K)
 20     CONTINUE
C
C      Adjust appropriate coordinates
        IF (IBCJA.EQ.1) THEN
         DEL = SQRT( (Y(1,2)-Y(1,1))**2 + (Z(1,2)-Z(1,1))**2 )
         PLNKAB(1) = X(1,1)
         DO 21 K=1,KMAX
          X(1,K) = -DEL
          X(2,K) =  0.0
          X(3,K) =  DEL
 21      CONTINUE
        ENDIF
        IF (IBCJA.EQ.2) THEN
         DEL = SQRT( (X(1,2)-X(1,1))**2 + (Z(1,2)-Z(1,1))**2 )
         PLNKAB(2) = Y(1,1)
         DO 22 K=1,KMAX
          Y(1,K) = -DEL
          Y(2,K) =  0.0
          Y(3,K) =  DEL
 22      CONTINUE
        ENDIF
        IF (IBCJA.EQ.3) THEN
         DEL = SQRT( (X(1,2)-X(1,1))**2 + (Y(1,2)-Y(1,1))**2 )
         PLNKAB(3) = Z(1,1)
         DO 23 K=1,KMAX
          Z(1,K) = -DEL
          Z(2,K) =  0.0
          Z(3,K) =  DEL
 23      CONTINUE
        ENDIF
C
C      Add extra planes to zetavr array
        IF ( VZETA ) THEN
         DO 25 J=2,3
         DO 25 N=1,3
         DO 25 K=1,KMAX
          ZETAVR(J,K,N) = ZETAVR(1,K,N)
 25      CONTINUE
        ENDIF
C
      ENDIF
C
C
C    Two-D plane in K
      IF (KMAX.EQ.1) THEN
C
C      Do some input checks and copy grid to 3 planes
        IF (IBCKA.NE.IBCKB) THEN
         WRITE(*,*)'Error: Inconsistent conditions on constant ',
     >             'K-boundary plane'
         STOP
        ENDIF
        IF ( (IBCKA.LE.0) .OR. (IBCKA.GE.4) ) THEN
         WRITE(*,*)'Error: Constant plane conditions not activated ',
     >             'for 2D case'
         STOP
        ENDIF
        KMAX  = 3
        KAOUT = 2
        KBOUT = 2
        DO 30 K=2,3
        DO 30 J=1,JMAX
         X(J,K) = X(J,1)
         Y(J,K) = Y(J,1)
         Z(J,K) = Z(J,1)
 30     CONTINUE
C
C      Adjust appropriate coordinates
        IF (IBCKA.EQ.1) THEN
         DEL = SQRT( (Y(2,1)-Y(1,1))**2 + (Z(2,1)-Z(1,1))**2 )
         PLNKAB(1) = X(1,1)
         DO 31 J=1,JMAX
          X(J,1) = -DEL
          X(J,2) =  0.0
          X(J,3) =  DEL
 31      CONTINUE
        ENDIF
        IF (IBCKA.EQ.2) THEN
         DEL = SQRT( (X(2,1)-X(1,1))**2 + (Z(2,1)-Z(1,1))**2 )
         PLNKAB(2) = Y(1,1)
         DO 32 J=1,JMAX
          Y(J,1) = -DEL
          Y(J,2) =  0.0
          Y(J,3) =  DEL
 32      CONTINUE
        ENDIF
        IF (IBCKA.EQ.3) THEN
         DEL = SQRT( (X(2,1)-X(1,1))**2 + (Y(2,1)-Y(1,1))**2 )
         PLNKAB(3) = Z(1,1)
         DO 33 J=1,JMAX
          Z(J,1) = -DEL
          Z(J,2) =  0.0
          Z(J,3) =  DEL
 33      CONTINUE
        ENDIF
C
C      Add extra planes to zetavr array
        IF ( VZETA ) THEN
         DO 35 N=1,3
         DO 35 K=2,3
         DO 35 J=1,JMAX
          ZETAVR(J,K,N) = ZETAVR(J,1,N)
 35      CONTINUE
        ENDIF
C
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ANBUNC(JDIM,KDIM,J,K,JPJ,JPK,JRJ,JRK,KPJ,KPK,KRJ,KRK,
     >                  X,Y,Z,XNOR,YNOR,ZNOR)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
C    Compute angle-bisecting unit normal at collapsed edge end point
C    for C-topology (5-point stencil)
       XJP = X(JPJ,JPK)-X(J,K)
       YJP = Y(JPJ,JPK)-Y(J,K)
       ZJP = Z(JPJ,JPK)-Z(J,K)
       XJR = X(JRJ,JRK)-X(J,K)
       YJR = Y(JRJ,JRK)-Y(J,K)
       ZJR = Z(JRJ,JRK)-Z(J,K)
       XKP = X(KPJ,KPK)-X(J,K)
       YKP = Y(KPJ,KPK)-Y(J,K)
       ZKP = Z(KPJ,KPK)-Z(J,K)
       XKR = X(KRJ,KRK)-X(J,K)
       YKR = Y(KRJ,KRK)-Y(J,K)
       ZKR = Z(KRJ,KRK)-Z(J,K)
C
C     Form unit vectors
       VJP = SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP )
       VJR = SQRT( XJR*XJR + YJR*YJR + ZJR*ZJR )
       VKP = SQRT( XKP*XKP + YKP*YKP + ZKP*ZKP )
       VKR = SQRT( XKR*XKR + YKR*YKR + ZKR*ZKR )
       XCJ = (XJP/VJP) - (XJR/VJR)
       YCJ = (YJP/VJP) - (YJR/VJR)
       ZCJ = (ZJP/VJP) - (ZJR/VJR)
       XCK = (XKP/VKP) - (XKR/VKR)
       YCK = (YKP/VKP) - (YKR/VKR)
       ZCK = (ZKP/VKP) - (ZKR/VKR)
C
C     Form angle-bisecting normal
       XNOR = YCJ*ZCK - ZCJ*YCK
       YNOR = ZCJ*XCK - XCJ*ZCK
       ZNOR = XCJ*YCK - YCJ*XCK
       DNOR = SQRT(XNOR*XNOR + YNOR*YNOR + ZNOR*ZNOR)
       XNOR = XNOR/DNOR
       YNOR = YNOR/DNOR
       ZNOR = ZNOR/DNOR
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ANBUNO(JDIM,KDIM,J,K,JPJ,JPK,KRJ,KRK,X,Y,Z,
     >                  XNOR,YNOR,ZNOR)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
C    Compute angle-bisecting unit normal at collapsed edge end point
C    for O-topology (3-point stencil)
      XJP = X(J,K) - X(JPJ,JPK)
      YJP = Y(J,K) - Y(JPJ,JPK)
      ZJP = Z(J,K) - Z(JPJ,JPK)
      XKR = X(J,K) - X(KRJ,KRK)
      YKR = Y(J,K) - Y(KRJ,KRK)
      ZKR = Z(J,K) - Z(KRJ,KRK)
      VJP = SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP )
      VKR = SQRT( XKR*XKR + YKR*YKR + ZKR*ZKR )
      XNOR = XJP/VJP + XKR/VKR
      YNOR = YJP/VJP + YKR/VKR
      ZNOR = ZJP/VJP + ZKR/VKR
      DNOR = SQRT(XNOR*XNOR + YNOR*YNOR + ZNOR*ZNOR)
      XNOR = XNOR/DNOR
      YNOR = YNOR/DNOR
      ZNOR = ZNOR/DNOR
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ANGMET2(JDIM,KDIM,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >                   JPLN1,KPLN1,VOL,X,Y,Z,XZ,YZ,ZZ,CAXI,CAET,
     >                   AFNXI,AFNET,VECM,UN,CJ,CK)
c*wdh*
c* include "precis.h"
C
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM),
     >          JPLN1(3),KPLN1(3)
      DIMENSION VOL(JDIM,KDIM),X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION CAXI(JDIM,KDIM),CAET(JDIM,KDIM),AFNXI(JDIM,KDIM),
     >          AFNET(JDIM,KDIM),VECM(JDIM,KDIM),UN(JDIM,KDIM,3),
     >          CJ(JDIM,KDIM,3),CK(JDIM,KDIM,3)
C
C   --------------------------------------------------------------------
C    Subroutine to
C    (1) Compute cosines of half grid angles
C    (2) Compute angle sensor function for spatially-varying dissipation
C    (3) Compute angle-bisecting vector and return in UN
C    (4) Compute modified zeta-derivatives and mix with normal derivatives
C
C    This subroutine is for L=2 only where special tests are required
C    to treat the singular zero-thickness plate case
C   --------------------------------------------------------------------
C
C   ------------------------------------------------
C    Dot product tolerance for zero-thickness corner
      DOTTOL = 1.0E-10
C   ------------------------------------------------

C    Compute un-normalized cosines of half angles
C
      DO 10 K=KA,KB
       KP = KKP(K)
       KR = KKR(K)
      DO 10 J=JA,JB
       JP = JJP(J)
       JR = JJR(J)
C
C      Form differences with neighbors
        XJP = (X(JP,K)-X(J,K))*JPLN1(1)
        YJP = (Y(JP,K)-Y(J,K))*JPLN1(2)
        ZJP = (Z(JP,K)-Z(J,K))*JPLN1(3)
        XJR = (X(JR,K)-X(J,K))*JPLN1(1)
        YJR = (Y(JR,K)-Y(J,K))*JPLN1(2)
        ZJR = (Z(JR,K)-Z(J,K))*JPLN1(3)
        XKP = (X(J,KP)-X(J,K))*KPLN1(1)
        YKP = (Y(J,KP)-Y(J,K))*KPLN1(2)
        ZKP = (Z(J,KP)-Z(J,K))*KPLN1(3)
        XKR = (X(J,KR)-X(J,K))*KPLN1(1)
        YKR = (Y(J,KR)-Y(J,K))*KPLN1(2)
        ZKR = (Z(J,KR)-Z(J,K))*KPLN1(3)
C
C      Form unit vectors
        VJP = MAX(SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP ),1.E-20)
        VJR = MAX(SQRT( XJR*XJR + YJR*YJR + ZJR*ZJR ),1.E-20)
        VKP = MAX(SQRT( XKP*XKP + YKP*YKP + ZKP*ZKP ),1.E-20)
        VKR = MAX(SQRT( XKR*XKR + YKR*YKR + ZKR*ZKR ),1.E-20)
        XCJ = (XJP/VJP) - (XJR/VJR)
        YCJ = (YJP/VJP) - (YJR/VJR)
        ZCJ = (ZJP/VJP) - (ZJR/VJR)
        XCK = (XKP/VKP) - (XKR/VKR)
        YCK = (YKP/VKP) - (YKR/VKR)
        ZCK = (ZKP/VKP) - (ZKR/VKR)
        CJ(J,K,1) = XCJ
        CJ(J,K,2) = YCJ
        CJ(J,K,3) = ZCJ
        CK(J,K,1) = XCK
        CK(J,K,2) = YCK
        CK(J,K,3) = ZCK
C
C      Form angle-bisecting normal
        XNOR = YCJ*ZCK - ZCJ*YCK
        YNOR = ZCJ*XCK - XCJ*ZCK
        ZNOR = XCJ*YCK - YCJ*XCK
C
C      Store magnitude of neighboring vectors
        VECM(J,K) = 0.0625*(VJP+VJR)*(VKP+VKR)
C
C      Store normalized dot product differences in AFNXI and AFNET
        AFNXI(J,K) = 1.0 - ((XJP*XJR + YJP*YJR + ZJP*ZJR)/(VJP*VJR))
        AFNET(J,K) = 1.0 - ((XKP*XKR + YKP*YKR + ZKP*ZKR)/(VKP*VKR))
C
C      Compute normalized cosines and angle-bisecting unit normal
C      for non-singular cases
        XYZNOR = XNOR*XNOR + YNOR*YNOR + ZNOR*ZNOR
        IF ( (AFNXI(J,K).GE.DOTTOL) .AND. (AFNET(J,K).GE.DOTTOL) .AND.
     >       (XYZNOR.GT.0.0) ) THEN
         CM = SQRT(XYZNOR)
         UN(J,K,1) = XNOR/CM
         UN(J,K,2) = YNOR/CM
         UN(J,K,3) = ZNOR/CM
         CAXI(J,K) = (UN(J,K,1)*XJP+UN(J,K,2)*YJP+UN(J,K,3)*ZJP)/VJP
         CAET(J,K) = (UN(J,K,1)*XKP+UN(J,K,2)*YKP+UN(J,K,3)*ZKP)/VKP
         VECM(J,K) = VOL(J,K)/(VECM(J,K)*CM)
        ENDIF
        IF (XYZNOR.EQ.0.0) THEN
         AFMIN = MIN( AFNXI(J,K), AFNET(J,K) )
         IF ( AFMIN.EQ.AFNXI(J,K) ) AFNXI(J,K) = 0.0
         IF ( AFMIN.EQ.AFNET(J,K) ) AFNET(J,K) = 0.0
        ENDIF
C
 10   CONTINUE
C
C    Normalize cosines, compute angle fns and modified zeta-deriv and store in UN
      DO 20 K=KA,KB
       KP = KKP(K)
       KR = KKR(K)
      DO 20 J=JA,JB
       JP = JJP(J)
       JR = JJR(J)
C
       IF ((AFNXI(J,K).LT.DOTTOL).OR.(AFNET(J,K).LT.DOTTOL)) THEN
C
        IF (AFNXI(J,K).LT.DOTTOL) THEN
         XCJ = -UN(JR,K,1)
         YCJ = -UN(JR,K,2)
         ZCJ = -UN(JR,K,3)
         XCK = CK(J,K,1)
         YCK = CK(J,K,2)
         ZCK = CK(J,K,3)
        ELSE IF (AFNET(J,K).LT.DOTTOL) THEN
         XCJ = CJ(J,K,1)
         YCJ = CJ(J,K,2)
         ZCJ = CJ(J,K,3)
         XCK = -UN(J,KR,1)
         YCK = -UN(J,KR,2)
         ZCK = -UN(J,KR,3)
        ENDIF
         XNOR = YCJ*ZCK - ZCJ*YCK
         YNOR = ZCJ*XCK - XCJ*ZCK
         ZNOR = XCJ*YCK - YCJ*XCK
         CM = SQRT(XNOR*XNOR + YNOR*YNOR + ZNOR*ZNOR)
         VECM(J,K) = VOL(J,K)/(VECM(J,K)*CM)
         UN(J,K,1) = XNOR/CM
         UN(J,K,2) = YNOR/CM
         UN(J,K,3) = ZNOR/CM
        IF (AFNXI(J,K).LT.DOTTOL) THEN
         XKP = X(J,KP)-X(J,K)
         YKP = Y(J,KP)-Y(J,K)
         ZKP = Z(J,KP)-Z(J,K)
         VKP = SQRT( XKP*XKP + YKP*YKP + ZKP*ZKP )
         CAXI(J,K) = -1.0
         CAET(J,K) = (UN(J,K,1)*XKP + UN(J,K,2)*YKP + 
     >                UN(J,K,3)*ZKP)/VKP
        ELSE IF (AFNET(J,K).LT.DOTTOL) THEN
         XJP = X(JP,K)-X(J,K)
         YJP = Y(JP,K)-Y(J,K)
         ZJP = Z(JP,K)-Z(J,K)
         VJP = SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP )
         CAET(J,K) = -1.0
         CAXI(J,K) = (UN(J,K,1)*XJP + UN(J,K,2)*YJP + 
     >                UN(J,K,3)*ZJP)/VJP
        ENDIF
C
       ENDIF
C
 20   CONTINUE
C
      DO 30 K=KA,KB
      DO 30 J=JA,JB
C
       IF (CAXI(J,K).LE.0.0) THEN
        AFNXI(J,K) = 1.0
       ELSE IF (CAXI(J,K).GT.0.0) THEN
        AFNXI(J,K) = 1.0/(1.0-(CAXI(J,K)*CAXI(J,K)))
       ENDIF
       IF (CAET(J,K).LE.0.0) THEN
        AFNET(J,K) = 1.0
       ELSE IF (CAET(J,K).GT.0.0) THEN
        AFNET(J,K) = 1.0/(1.0-(CAET(J,K)*CAET(J,K)))
       ENDIF
C
C      Modify zeta derivatives (use angle-bisecting normal at L=2)
        XZ(J,K) = UN(J,K,1)*VECM(J,K)
        YZ(J,K) = UN(J,K,2)*VECM(J,K)
        ZZ(J,K) = UN(J,K,3)*VECM(J,K)
C
 30   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ANGMET(JDIM,KDIM,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >                  JPLN1,KPLN1,VOL,X,Y,Z,XZ,YZ,ZZ,CAXI,CAET,
     >                  AFNXI,AFNET,UN)
c*wdh*
c* include "precis.h"
C
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM),
     >          JPLN1(3),KPLN1(3)
      DIMENSION VOL(JDIM,KDIM),X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION CAXI(JDIM,KDIM),CAET(JDIM,KDIM),AFNXI(JDIM,KDIM),
     >          AFNET(JDIM,KDIM),UN(JDIM,KDIM,3)
C
C   --------------------------------------------------------------------
C    Subroutine to
C    (1) Compute cosines of half grid angles
C    (2) Compute angle sensor function for spatially-varying dissipation
C    (3) Compute angle-bisecting vector and return in UN
C    (4) Compute modified zeta-derivatives and mix with normal derivatives
C
C    This subroutine is for L>2 where the special tests for singular
C    zero-thickness plate cases are not required
C   --------------------------------------------------------------------
C
C    Compute mixing factor for modified zeta derivatives
      IF (L.EQ.2) THEN
       ZSC = 1.0
      ELSE IF ((L.GT.2).AND.(L.LE.10)) THEN
       ZSC = 1.0/(2.0**(L-2))
      ELSE IF (L.GT.10) THEN
       ZSC = 0.0
      ENDIF
      ZSCM = 1.0-ZSC
C
      DO 10 K=KA,KB
       KP = KKP(K)
       KR = KKR(K)
      DO 10 J=JA,JB
       JP = JJP(J)
       JR = JJR(J)
C
C      Form differences with neighbors
        XJP = (X(JP,K)-X(J,K))*JPLN1(1)
        YJP = (Y(JP,K)-Y(J,K))*JPLN1(2)
        ZJP = (Z(JP,K)-Z(J,K))*JPLN1(3)
        XJR = (X(JR,K)-X(J,K))*JPLN1(1)
        YJR = (Y(JR,K)-Y(J,K))*JPLN1(2)
        ZJR = (Z(JR,K)-Z(J,K))*JPLN1(3)
        XKP = (X(J,KP)-X(J,K))*KPLN1(1)
        YKP = (Y(J,KP)-Y(J,K))*KPLN1(2)
        ZKP = (Z(J,KP)-Z(J,K))*KPLN1(3)
        XKR = (X(J,KR)-X(J,K))*KPLN1(1)
        YKR = (Y(J,KR)-Y(J,K))*KPLN1(2)
        ZKR = (Z(J,KR)-Z(J,K))*KPLN1(3)
C
C      Form unit vectors
        VJP = MAX(SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP ),1.E-20)
        VJR = MAX(SQRT( XJR*XJR + YJR*YJR + ZJR*ZJR ),1.E-20)
        VKP = MAX(SQRT( XKP*XKP + YKP*YKP + ZKP*ZKP ),1.E-20)
        VKR = MAX(SQRT( XKR*XKR + YKR*YKR + ZKR*ZKR ),1.E-20)
        XCJ = (XJP/VJP) - (XJR/VJR)
        YCJ = (YJP/VJP) - (YJR/VJR)
        ZCJ = (ZJP/VJP) - (ZJR/VJR)
        XCK = (XKP/VKP) - (XKR/VKR)
        YCK = (YKP/VKP) - (YKR/VKR)
        ZCK = (ZKP/VKP) - (ZKR/VKR)
C
C      Form angle-bisecting normal and store normalized vector in UN
        XNOR = YCJ*ZCK - ZCJ*YCK
        YNOR = ZCJ*XCK - XCJ*ZCK
        ZNOR = XCJ*YCK - YCJ*XCK
        XYZNOR = MAX( (XNOR*XNOR + YNOR*YNOR + ZNOR*ZNOR), 1.E-30 )
        CM = SQRT(XYZNOR)
        UN(J,K,1) = XNOR/CM
        UN(J,K,2) = YNOR/CM
        UN(J,K,3) = ZNOR/CM
C
C      Compute magnitude of neighboring vectors
        VECJK = 0.0625*(VJP+VJR)*(VKP+VKR)
C
C      Compute normalized cosines
        CAXI(J,K) = (UN(J,K,1)*XJP + UN(J,K,2)*YJP + UN(J,K,3)*ZJP)/VJP
        CAET(J,K) = (UN(J,K,1)*XKP + UN(J,K,2)*YKP + UN(J,K,3)*ZKP)/VKP
C
C      Compute angle functions
        IF (CAXI(J,K).LE.0.0) THEN
         AFNXI(J,K) = 1.0
        ELSE IF (CAXI(J,K).GT.0.0) THEN
         AFNXI(J,K) = 1.0/(1.0-(CAXI(J,K)*CAXI(J,K)))
        ENDIF
        IF (CAET(J,K).LE.0.0) THEN
         AFNET(J,K) = 1.0
        ELSE IF (CAET(J,K).GT.0.0) THEN
         AFNET(J,K) = 1.0/(1.0-(CAET(J,K)*CAET(J,K)))
        ENDIF
C
C      Store angle-bisecting vector in UN and modify zeta derivatives
        VRD = VOL(J,K)/(VECJK*XYZNOR)
        XZN = XNOR*VRD
        YZN = YNOR*VRD
        ZZN = ZNOR*VRD
        XZ(J,K) = ZSCM*XZ(J,K) + ZSC*XZN
        YZ(J,K) = ZSCM*YZ(J,K) + ZSC*YZN
        ZZ(J,K) = ZSCM*ZZ(J,K) + ZSC*ZZN
C
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE AXSMOO(JDIM,KDIM,KMAX,J,JI,KPLNA,KPLNB,PLNKAB,X,Y,Z)
c*wdh*
c* include "precis.h"
C
      DIMENSION KPLNA(3),KPLNB(3),PLNKAB(3)
      DIMENSION X(JDIM,KDIM), Y(JDIM,KDIM), Z(JDIM,KDIM)
C
C    Do linear extrapolation at axis to smooth out dimple
C
      JN  = J+JI
      JNN = JN+JI
      XAV = 0.0
      YAV = 0.0
      ZAV = 0.0
C
      DO 10 K=1,KMAX
       XAV = XAV + 2.0*X(JN,K) - X(JNN,K)
       YAV = YAV + 2.0*Y(JN,K) - Y(JNN,K)
       ZAV = ZAV + 2.0*Z(JN,K) - Z(JNN,K)
 10   CONTINUE
C
      XAV = XAV/FLOAT(KMAX)
      YAV = YAV/FLOAT(KMAX)
      ZAV = ZAV/FLOAT(KMAX)
      XAV = XAV*KPLNA(1) + (1.0-KPLNA(1))*PLNKAB(1)
      YAV = YAV*KPLNA(2) + (1.0-KPLNA(2))*PLNKAB(2)
      ZAV = ZAV*KPLNA(3) + (1.0-KPLNA(3))*PLNKAB(3)
C
      DO 20 K=1,KMAX
       X(J,K) = XAV
       Y(J,K) = YAV
       Z(J,K) = ZAV
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE AXSMOO2(JDIM,KDIM,JMAX,KMAX,LMAX,J,L,JI,KPLNA,KPLNB,
     >                  PLNKAB,X,Y,Z,XW,YW,ZW)
c*wdh*
c* include "precis.h"
C
      DIMENSION KPLNA(3),KPLNB(3),PLNKAB(3)
      DIMENSION X(JDIM,KDIM), Y(JDIM,KDIM), Z(JDIM,KDIM)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C   --------------------------------------------------------
C   Do smoothing at axis if dimple at axis is getting deeper
C   --------------------------------------------------------
C
      DOTTOL = -0.005
      COSCMIN = 100000.0
      COSBMIN = 100000.0
      COSBMAX = 0.0
      COSOMIN = 100000.0
C
C    Compute unit vector in axis direction
      XA = X(J,1)-XW(J,1,L-1)
      YA = Y(J,1)-YW(J,1,L-1)
      ZA = Z(J,1)-ZW(J,1,L-1)
      DA = 1.0/SQRT(XA*XA+YA*YA+ZA*ZA)
C
C    Compute dot products
      DO 10 K=1,KMAX
       XB = X(J,K) - X(J+JI,K)
       YB = Y(J,K) - Y(J+JI,K)
       ZB = Z(J,K) - Z(J+JI,K)
       DB = 1.0/SQRT(XB*XB + YB*YB + ZB*ZB)
       COSC = (XB*XA + YB*YA + ZB*ZA)*DB*DA
       XC = X(J+JI,K) - X(J+JI+JI,K)
       YC = Y(J+JI,K) - Y(J+JI+JI,K)
       ZC = Z(J+JI,K) - Z(J+JI+JI,K)
       DC = 1.0/SQRT(XC*XC + YC*YC + ZC*ZC)
       COSB = (XC*XA + YC*YA + ZC*ZA)*DC*DA
       XO = XW(J,K,L-1) - XW(J+JI,K,L-1)
       YO = YW(J,K,L-1) - YW(J+JI,K,L-1)
       ZO = ZW(J,K,L-1) - ZW(J+JI,K,L-1)
       DO = 1.0/SQRT(XO*XO + YO*YO + ZO*ZO)
       COSO = (XO*XA + YO*YA + ZO*ZA)*DO*DA
       IF (COSC.LT.COSCMIN) COSCMIN = COSC
       IF (COSB.LT.COSBMIN) COSBMIN = COSB
       IF (COSB.GT.COSBMAX) THEN
        COSBMAX = COSB
        COSAMAX = (XB*XC + YB*YC + ZB*ZC)*DB*DC
        DBMAX = 1.0/DB
       ENDIF
       IF (COSO.LT.COSOMIN) COSOMIN = COSO
 10   CONTINUE
C
C    Move point out along axis direction
      IMOVE = 0
      IF ((COSCMIN.LT.DOTTOL).AND.(COSCMIN.LT.COSOMIN)) THEN
       IMOVE = 1
       DNEW = COSCMIN/DA
       write(*,*)'case a ',l,coscmin,cosomin
      ELSE IF ((COSCMIN.GE.DOTTOL).AND.(COSBMIN.GE.-DOTTOL)) THEN
       IMOVE = 1
       QUO = (1.0-COSAMAX*COSAMAX)/(1.0-COSBMAX*COSBMAX)
       DNEW = 0.5*DBMAX*SQRT(QUO)
       write(*,*)'case b ',l,coscmin,cosbmin,dbmax
      ENDIF
      IF (IMOVE.EQ.1) THEN
       XAV = XW(J,1,L-1) + XA*DNEW
       YAV = YW(J,1,L-1) + YA*DNEW
       ZAV = ZW(J,1,L-1) + ZA*DNEW
       XAV = XAV*KPLNA(1) + (1.0-KPLNA(1))*PLNKAB(1)
       YAV = YAV*KPLNA(2) + (1.0-KPLNA(2))*PLNKAB(2)
       ZAV = ZAV*KPLNA(3) + (1.0-KPLNA(3))*PLNKAB(3)
       DO 20 K=1,KMAX
        X(J,K) = XAV
        Y(J,K) = YAV
        Z(J,K) = ZAV
 20    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE AXWAVG(JDIM,KDIM,JMAX,KMAX,J,KPER,KSYMA,KSYMB,KFLTA,
     >                  KFLTB,KPLNA,KPLNB,PLNKAB,KKP,KKR,DP,X,Y,Z)
c*wdh*
c* include "precis.h"
C
      LOGICAL KSYMA,KSYMB,KFLTA,KFLTB
      DIMENSION KPLNA(3),KPLNB(3),PLNKAB(3),KKP(KDIM),KKR(KDIM)
      DIMENSION DP(KDIM), X(JDIM,KDIM), Y(JDIM,KDIM), Z(JDIM,KDIM)
C
C   ----------------------------------------
C    Compute weighted average for axis point
C   ----------------------------------------
      XAV = 0.0
      YAV = 0.0
      ZAV = 0.0
      IF (J.EQ.1) JN=2
      IF (J.EQ.JMAX) JN=JMAX-1
C
C    Compute distance between each point in K at JN
      ARCL = 0.0
      DO 2 K=1,KMAX
       KP = KKP(K)
       DP(K) = SQRT( (X(JN,KP)-X(JN,K))**2 + (Y(JN,KP)-Y(JN,K))**2 + 
     >               (Z(JN,KP)-Z(JN,K))**2 )
       ARCL = ARCL + DP(K)
 2    CONTINUE
C
C    Compute adjusted arc length and weighted averages
      IF (KPER.EQ.1) THEN
        DO 10 K=1,KMAX
         KR = KKR(K)
         WT = 0.5*(DP(K)+DP(KR))/ARCL
         XAV = XAV + WT*X(J,K)
         YAV = YAV + WT*Y(J,K)
         ZAV = ZAV + WT*Z(J,K)
 10     CONTINUE
      ELSE IF (KPER.EQ.0) THEN
       IF (KSYMA .AND. KSYMB) THEN
        ARCL = 2.0*(ARCL - DP(1) - DP(KMAX-1) - DP(KMAX))
        KMB = 2
        KME = KMAX-1
       ELSE IF (KFLTA .AND. KFLTB) THEN
        ARCL = 2.0*(ARCL - DP(KMAX))
        KMB = 1
        KME = KMAX
       ENDIF
        DP(KME) = DP(KME-1)
        WB = DP(KMB)/ARCL
        WE = DP(KME)/ARCL
        XAV = XAV + WB*X(J,KMB) + WE*X(J,KME)
        YAV = YAV + WB*Y(J,KMB) + WE*Y(J,KME)
        ZAV = ZAV + WB*Z(J,KMB) + WE*Z(J,KME)
       DO 20 K=KMB+1,KME-1
        KR = KKR(K)
        WT = (DP(K)+DP(KR))/ARCL
        XAV = XAV + WT*X(J,K)*KPLNA(1)
        YAV = YAV + WT*Y(J,K)*KPLNA(2)
        ZAV = ZAV + WT*Z(J,K)*KPLNA(3)
 20    CONTINUE
      ENDIF
C
C    Update all points at boundary
      XAV = XAV*KPLNA(1) + (1.0-KPLNA(1))*PLNKAB(1)
      YAV = YAV*KPLNA(2) + (1.0-KPLNA(2))*PLNKAB(2)
      ZAV = ZAV*KPLNA(3) + (1.0-KPLNA(3))*PLNKAB(3)
      DO 200 K=1,KMAX
       X(J,K) = XAV
       Y(J,K) = YAV
       Z(J,K) = ZAV
 200  CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE AXFILT(JDIM,KDIM,KMAX,LMAX,JA,JB,KA,KB,L,
     >                  JAXSA,JAXSB,KPLNA,KPLNB,X,Y,Z)
c*wdh*
c* include "precis.h"
C
      LOGICAL JAXSA,JAXSB
      DIMENSION KPLNA(3),KPLNB(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
C    Filter axis values for iaxis = 2
      FILSC = 0.3*(FLOAT(L)/FLOAT(LMAX))**2
      IF (JAXSA) THEN
       XAXS = 0.
       YAXS = 0.
       ZAXS = 0.
       DO 41 K = 1,KMAX
        XAXS =  XAXS+ 1.333*X(JA ,K) -.333*X(JA+1,K)
        YAXS =  YAXS+ 1.333*Y(JA ,K) -.333*Y(JA+1,K)
        ZAXS =  ZAXS+ 1.333*Z(JA ,K) -.333*Z(JA+1,K)
 41    CONTINUE
       XAXS = XAXS/KMAX
       YAXS = YAXS/KMAX
       ZAXS = ZAXS/KMAX
       DO 42 K = 1,KMAX
        X(JA-1,K) =KPLNA(1)*(X(JA-1,K)+FILSC*(XAXS-X(JA-1,K)))
        Y(JA-1,K) =KPLNA(2)*(Y(JA-1,K)+FILSC*(YAXS-Y(JA-1,K)))
        Z(JA-1,K) =KPLNA(3)*(Z(JA-1,K)+FILSC*(ZAXS-Z(JA-1,K)))
 42    CONTINUE
      ENDIF 
      IF (JAXSB) THEN
       XAXS = 0.
       YAXS = 0.
       ZAXS = 0.
       DO 43 K = 1,KMAX
        XAXS =  XAXS+ 1.333*X(JB ,K) -.333*X(JB-1,K)
        YAXS =  YAXS+ 1.333*Y(JB ,K) -.333*Y(JB-1,K)
        ZAXS =  ZAXS+ 1.333*Z(JB ,K) -.333*Z(JB-1,K)
c        XAXS =  XAXS+ 1.333*X(JB ,K) -.333*X(JB+1,K)
c        YAXS =  YAXS+ 1.333*Y(JB ,K) -.333*Y(JB+1,K)
c        ZAXS =  ZAXS+ 1.333*Z(JB ,K) -.333*Z(JB+1,K)
 43    CONTINUE
       XAXS = XAXS/KMAX
       YAXS = YAXS/KMAX
       ZAXS = ZAXS/KMAX
       DO 44 K = 1,KMAX
        X(JB+1,K) =KPLNB(1)*(X(JB+1,K)+FILSC*(XAXS-X(JB+1,K)))
        Y(JB+1,K) =KPLNB(2)*(Y(JB+1,K)+FILSC*(YAXS-Y(JB+1,K)))
        Z(JB+1,K) =KPLNB(3)*(Z(JB+1,K)+FILSC*(ZAXS-Z(JB+1,K)))
 44    CONTINUE
      ENDIF 
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE AXIS(I1,I2,INC,XP,YP,ZP,IDIM)
c*wdh*
c* include "precis.h"
      DIMENSION XP(IDIM), YP(IDIM), ZP(IDIM)
C
C    Simple averaging of axis points
      NAVER = 0
      XSUM1 = 0.
      YSUM1 = 0.
      ZSUM1 = 0.
      DO 100 I =I1,I2,INC
       NAVER = NAVER + 1
       XSUM1 = XSUM1 + XP(I)
       YSUM1 = YSUM1 + YP(I)
       ZSUM1 = ZSUM1 + ZP(I)
 100  CONTINUE 
      XSUM1 = XSUM1/NAVER
      YSUM1 = YSUM1/NAVER
      ZSUM1 = ZSUM1/NAVER
      DO 200 I = I1,I2,INC
       XP(I) = XSUM1
       YP(I) = YSUM1
       ZP(I) = ZSUM1
 200  CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCAXJ(JDIM,KDIM,JA,JB,KA,KB,JAXSA,JAXSB,EXAXIS,A,B,C,F)
c*wdh*
c* include "precis.h"
C
      LOGICAL JAXSA,JAXSB
      DIMENSION A(JDIM,KDIM,3,3),B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),
     >          F(JDIM,KDIM,3)
C
C    Set up end point matrices in HFILTRX for axis bc in J. Extrapolation
C    is controlled by EXAXIS where 0(0th order) <= EXAXIS <= 1(1st order)
C
      EXAP = 1.0 + EXAXIS
C
      IF (JAXSA) THEN
       DO 11 N=1,3
       DO 11 M=1,3
       DO 11 K=KA,KB
        A(JA-1,K,N,M) = 0.0
        B(JA-1,K,N,M) = 0.0
        C(JA-1,K,N,M) = 0.0
 11    CONTINUE
       DO 12 N=1,3
       DO 12 K=KA,KB
        F(JA-1,K,N) = 0.0
        A(JA-1,K,N,N) = EXAXIS
        B(JA-1,K,N,N) = 1.0
        C(JA-1,K,N,N) = -EXAP
 12    CONTINUE
      ENDIF
C
      IF (JAXSB) THEN
       DO 21 N=1,3
       DO 21 M=1,3
       DO 21 K=KA,KB
        A(JB+1,K,N,M) = 0.0
        B(JB+1,K,N,M) = 0.0
        C(JB+1,K,N,M) = 0.0
 21    CONTINUE
       DO 22 N=1,3
       DO 22 K=KA,KB
        F(JB+1,K,N) = 0.0
        C(JB+1,K,N,N) = EXAXIS
        B(JB+1,K,N,N) = 1.0
        A(JB+1,K,N,N) = -EXAP
 22    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCAXJA(JDIM,KDIM,LMAX,JA,KA,KB,L,KPER,KSYMA,KSYMB,
     >                  KPLNA,DISSL,RR,X,Y,Z,XJADEL,YJADEL,ZJADEL)
c*wdh*
c* include "precis.h"
C
      PARAMETER (IDIM=1000, LNKDIM=4)
      LOGICAL KSYMA,KSYMB
      DIMENSION KPLNA(3),DISSL(LMAX),RR(JDIM,KDIM,LMAX)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
      COMMON/UBASE/ LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM),
     >              DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),
     >              EXI(IDIM,3), EET(IDIM,3),
     >              XP(IDIM), YP(IDIM), ZP(IDIM),
     >              XS(IDIM), YS(IDIM), ZS(IDIM), NKKP(IDIM/10)
C
C   -------------------------------------------------------------------
C    Set up data for axis at JA to use four link unstructured logic
C    explicit update - UHYG4L. The b.c. in the K-direction is assumed
C    to be either periodic or symmetric at both KA and KB.
C   -------------------------------------------------------------------
C
      IADD = 0
      J = JA -1
      DZETA = RR(1,1,L) - RR(1,1,L-1)
C
C    Load XU,YU,ZU from I=IS,IE and I=IS+KB-KA+1,IE+KB-KA+1
C
      IF (KPER.EQ.1) THEN
C
       IS = KA + IADD
       IE = KB-KA+1 + IADD
       DO 10 K = KA,KB
        I = K + IADD
        XU(I) = X(J,K)
        YU(I) = Y(J,K)
        ZU(I) = Z(J,K)
        I = K + (KB-KA+1) + IADD
        XU(I) = X(J+1,K)
        YU(I) = Y(J+1,K)
        ZU(I) = Z(J+1,K)
   10  CONTINUE
c
      ELSE IF ((KSYMA).AND.(KSYMB)) THEN
c
       KMAX = KB+1
       IS = 1
       IE = 2*(KMAX-4) + 2
       DO 110 K=2,KMAX-1
        I = K-1
        XU(I) = X(J,K)
        YU(I) = Y(J,K)
        ZU(I) = Z(J,K)
 110   CONTINUE
       DO 115 K=3,KMAX-2
        I = (KMAX-2)+(K-2)
        KREF = KMAX+1-K
        XU(I) = X(J,KREF)*(2.*KPLNA(1)-1.)
        YU(I) = Y(J,KREF)*(2.*KPLNA(2)-1.)
        ZU(I) = Z(J,KREF)*(2.*KPLNA(3)-1.)
 115   CONTINUE
       DO 120 K=2,KMAX-1
        I = 2*KMAX - 7 + K
        XU(I) = X(J+1,K)
        YU(I) = Y(J+1,K)
        ZU(I) = Z(J+1,K)
 120   CONTINUE
       DO 125 K=3,KMAX-2
        I = 3*KMAX - 10 + K
        KREF = KMAX+1-K
        XU(I) = X(J+1,KREF)*(2.*KPLNA(1)-1.)
        YU(I) = Y(J+1,KREF)*(2.*KPLNA(2)-1.)
        ZU(I) = Z(J+1,KREF)*(2.*KPLNA(3)-1.)
 125   CONTINUE
c
      ENDIF
c
      KTOT = IE
      DO 180 K=1,KTOT
       NKKP(K) = K+1
 180  CONTINUE
      NKKP(KTOT) = 1
c
C                           LINKS FOR PERIODIC AXIS
      KH = (KTOT+1)/2
      KQ = (KH+1)/2
      K1 = 1
      K2 = K1 + KH
      K3 = K1 + KQ
      K4 = K1 + KQ + KH
      DO 11 K =1,KTOT
      I = K +IADD
      LINK(I,1) =  KTOT + K1     
      LINK(I,2) =  KTOT + K2
C      LINK(I,2) =  KTOT + K1
C      LINK(I,1) =  KTOT + K2
      LINK(I,3) =  KTOT + K3
      LINK(I,4) =  KTOT + K4
      K1 = NKKP(K1)
      K2 = NKKP(K2)
      K3 = NKKP(K3)
      K4 = NKKP(K4)
   11 CONTINUE
C                                  SAVE AXIS VALUES OF X,Y,Z AT LEVEL L
      I = (KA+KB)/2 + IADD
      XA = XU(I) 
      YA = YU(I) 
      ZA = ZU(I) 
C                                      ADVANCE  X,Y,Z TO LEVEL L+1
      CALL UHYG4L(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >     DELXI,DELET,DELVOL,EXI,EET,DISSL(L),XS,YS,ZS,XP,YP,ZP)
C                                        AVERAGE AXIS VALUES
      CALL AXIS(IS,IE,1,XU,YU,ZU,IDIM)
C
C    Save increment for BC on inversion
      I = (KA+KB)/2 + IADD
      XJADEL = (XU(I) - XA)*KPLNA(1)
      YJADEL = (YU(I) - YA)*KPLNA(2)
      ZJADEL = (ZU(I) - ZA)*KPLNA(3)
C
C    Update X,Y,Z to level L+1
      IF (KPER.EQ.1) THEN
       ISH = 0
      ELSE IF ((KSYMA).AND.(KSYMB)) THEN
       ISH = -1
      ENDIF
      DO 12 K = KA,KB
       I = K + ISH
       X(J,K) = XU(I)*KPLNA(1)
       Y(J,K) = YU(I)*KPLNA(2)
       Z(J,K) = ZU(I)*KPLNA(3)
 12   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCAXJBN(JDIM,KDIM,LMAX,JB,KA,KB,L,KPER,KSYMA,KSYMB,
     >                   KPLNB,DISSL,RR,X,Y,Z,XJBDEL,YJBDEL,ZJBDEL)
c*wdh*
c* include "precis.h"
C
      PARAMETER (IDIM=1000, LNKDIM=4)
      LOGICAL KSYMA,KSYMB
      DIMENSION KPLNB(3),DISSL(LMAX),RR(JDIM,KDIM,LMAX)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
      COMMON/UBASE/ LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM),
     >              DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),
     >              EXI(IDIM,3), EET(IDIM,3),
     >              XP(IDIM), YP(IDIM), ZP(IDIM),
     >              XS(IDIM), YS(IDIM), ZS(IDIM), NKKP(IDIM/10)
C
C   -------------------------------------------------------------------
C    Set up data for axis at JB to use four link unstructured logic
C    explicit update - UHYG4L. The b.c. in the K-direction is assumed
C    to be either periodic or symmetric at both KA and KB.
C   -------------------------------------------------------------------
C
      IADD = 0
      J = JB + 1
      DZETA = RR(1,1,L) - RR(1,1,L-1)
C
C    Load IS,IE,XU,YU,ZU
C
      IF (KPER.EQ.1) THEN
c
       IS = KA + IADD
       IE = KB-KA+1 + IADD
       DO 10 K = KA,KB
        I = K + IADD
        KREF = KMAX + 1 - K
        XU(I) = X(J,KREF)
        YU(I) = Y(J,KREF)
        ZU(I) = Z(J,KREF)
        I = K + (KB-KA+1) + IADD
        XU(I) = X(J-1,KREF)
        YU(I) = Y(J-1,KREF)
        ZU(I) = Z(J-1,KREF)
   10  CONTINUE
c
      ELSE IF ((KSYMA).AND.(KSYMB)) THEN
c
       KMAX = KB+1
       IS = 1
       IE = 2*(KMAX-4) + 2
       DO 110 K=2,KMAX-1
        I = K-1
        KREF = KMAX + 1 - K
        XU(I) = X(J,KREF)
        YU(I) = Y(J,KREF)
        ZU(I) = Z(J,KREF)
 110   CONTINUE
       DO 115 K=3,KMAX-2
        I = (KMAX-2)+(K-2)
        XU(I) = X(J,K)*(2.*KPLNB(1)-1.)
        YU(I) = Y(J,K)*(2.*KPLNB(2)-1.)
        ZU(I) = Z(J,K)*(2.*KPLNB(3)-1.)
 115   CONTINUE
       DO 120 K=2,KMAX-1
        I = 2*KMAX - 7 + K
        KREF = KMAX + 1 - K
        XU(I) = X(J-1,KREF)
        YU(I) = Y(J-1,KREF)
        ZU(I) = Z(J-1,KREF)
 120   CONTINUE
       DO 125 K=3,KMAX-2
        I = 3*KMAX - 10 + K
        XU(I) = X(J-1,K)*(2.*KPLNB(1)-1.)
        YU(I) = Y(J-1,K)*(2.*KPLNB(2)-1.)
        ZU(I) = Z(J-1,K)*(2.*KPLNB(3)-1.)
 125   CONTINUE
c
      ENDIF
c
      KTOT = IE
      DO 180 K=1,KTOT
       NKKP(K) = K+1
 180  CONTINUE
      NKKP(KTOT) = 1
c
C                           LINKS FOR PERIODIC AXIS
      KH = (KTOT+1)/2
      KQ = (KH+1)/2
      K1 = 1
      K2 = K1 + KH
      K3 = K1 + KQ
      K4 = K1 + KQ + KH
      DO 21 K =1,KTOT
      I = K + IADD
      LINK(I,1) =  KTOT + K1
      LINK(I,2) =  KTOT + K2
      LINK(I,3) =  KTOT + K3
      LINK(I,4) =  KTOT + K4
C      LINK(I,2) =  K1
C      LINK(I,1) =  K2
C      LINK(I,4) =  K3
C      LINK(I,3) =  K4
      K1 = NKKP(K1)
      K2 = NKKP(K2)
      K3 = NKKP(K3)
      K4 = NKKP(K4)
   21 CONTINUE
C
C                                  SAVE AXIS VALUES OF X,Y,Z AT LEVEL L
      I = (KA+KB)/2 + IADD
      XA = XU(I) 
      YA = YU(I) 
      ZA = ZU(I) 
C                                      ADVANCE  X,Y,Z TO LEVEL L+1
      CALL UHYG4L(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >     DELXI,DELET,DELVOL,EXI,EET,DISSL(L),XS,YS,ZS,XP,YP,ZP)
C                                        AVERAGE AXIS VALUES
      CALL AXIS(IS,IE,1,XU,YU,ZU,IDIM)
C
C    Save increment for BC on inversion
      I = (KA+KB)/2 + IADD
      XJBDEL = (XU(I) - XA)*KPLNB(1)
      YJBDEL = (YU(I) - YA)*KPLNB(2)
      ZJBDEL = (ZU(I) - ZA)*KPLNB(3)
C
      IF (KPER.EQ.1) THEN
       ISH = 0
      ELSE IF ((KSYMA).AND.(KSYMB)) THEN
       ISH = -1
      ENDIF
      DO 22 K = KA,KB
       I = K + ISH
       KREF = KMAX + 1 - K
       X(J,KREF) = XU(I)*KPLNB(1)
       Y(J,KREF) = YU(I)*KPLNB(2)
       Z(J,KREF) = ZU(I)*KPLNB(3)
 22   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCAXJB(JDIM,KDIM,LMAX,JB,KA,KB,L,KPER,KSYMA,KSYMB,
     >                  KPLNB,DISSL,RR,X,Y,Z,XJBDEL,YJBDEL,ZJBDEL)
c*wdh*
c* include "precis.h"
C
      PARAMETER (IDIM=1000, LNKDIM=4)
      LOGICAL KSYMA,KSYMB
      DIMENSION KPLNB(3),DISSL(LMAX),RR(JDIM,KDIM,LMAX)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
      COMMON/UBASE/ LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM),
     >              DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),
     >              EXI(IDIM,3), EET(IDIM,3),
     >              XP(IDIM), YP(IDIM), ZP(IDIM),
     >              XS(IDIM), YS(IDIM), ZS(IDIM), NKKP(IDIM/10)
C
C   -------------------------------------------------------------------
C    Set up data for axis at JB to use four link unstructured logic
C    explicit update - UHYG4L. The b.c. in the K-direction is assumed
C    to be either periodic or symmetric at both KA and KB.
C   -------------------------------------------------------------------
C
      IADD = 0
      J = JB + 1
      DZETA = RR(1,1,L) - RR(1,1,L-1)
C
C    Load IS,IE,XU,YU,ZU
C
      IF (KPER.EQ.1) THEN
c
       IS = KA + KB-KA+1 +IADD
       IE = KB-KA+1 + KB-KA+1 + IADD
       DO 10 K = KA,KB
        I = K + IADD
        XU(I) = X(J-1,K)
        YU(I) = Y(J-1,K)
        ZU(I) = Z(J-1,K)
        I = K + (KB-KA+1) + IADD
        XU(I) = X(J,K)
        YU(I) = Y(J,K)
        ZU(I) = Z(J,K)
   10  CONTINUE
c
      ELSE IF ((KSYMA).AND.(KSYMB)) THEN
c
       KMAX = KB+1
       IS = 1 + KMAX
       IE = 2*(KMAX-4) + 2 + KMAX
       DO 110 K=2,KMAX-1
        I = K-1
        XU(I) = X(J-1,K)
        YU(I) = Y(J-1,K)
        ZU(I) = Z(J-1,K)
 110   CONTINUE
       DO 115 K=3,KMAX-2
        I = (KMAX-2)+(K-2)
        KREF = KMAX+1-K
        XU(I) = X(J-1,KREF)*(2.*KPLNB(1)-1.)
        YU(I) = Y(J-1,KREF)*(2.*KPLNB(2)-1.)
        ZU(I) = Z(J-1,KREF)*(2.*KPLNB(3)-1.)
 115   CONTINUE
       DO 120 K=2,KMAX-1
        I = 2*KMAX - 7 + K + KMAX
        XU(I) = X(J,K)
        YU(I) = Y(J,K)
        ZU(I) = Z(J,K)
 120   CONTINUE
       DO 125 K=3,KMAX-2
        I = 3*KMAX - 10 + K + KMAX
        KREF = KMAX+1-K
        XU(I) = X(J,KREF)*(2.*KPLNB(1)-1.)
        YU(I) = Y(J,KREF)*(2.*KPLNB(2)-1.)
        ZU(I) = Z(J,KREF)*(2.*KPLNB(3)-1.)
 125   CONTINUE
c
      ENDIF
c
c      KTOT = IE
      KTOT = IE-KMAX
      DO 180 K=1,KTOT
       NKKP(K) = K+1
 180  CONTINUE
      NKKP(KTOT) = 1
c
C                           LINKS FOR PERIODIC AXIS
      KH = (KTOT+1)/2
      KQ = (KH+1)/2
      K1 = 1
      K2 = K1 + KH
      K3 = K1 + KQ
      K4 = K1 + KQ + KH
      DO 21 K =1,KTOT
      I = K + KTOT + IADD
c      LINK(I,1) =  K1
c      LINK(I,2) =  K2
      LINK(I,3) =  K3
      LINK(I,4) =  K4
      LINK(I,2) =  K1
      LINK(I,1) =  K2
C      LINK(I,4) =  K3
C      LINK(I,3) =  K4
      K1 = NKKP(K1)
      K2 = NKKP(K2)
      K3 = NKKP(K3)
      K4 = NKKP(K4)
   21 CONTINUE
C
C                                  SAVE AXIS VALUES OF X,Y,Z AT LEVEL L
      I = (KA+KB)/2 + KTOT + IADD
      XA = XU(I) 
      YA = YU(I) 
      ZA = ZU(I) 
C                                      ADVANCE  X,Y,Z TO LEVEL L+1
      CALL UHYG4L(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >     DELXI,DELET,DELVOL,EXI,EET,DISSL(L),XS,YS,ZS,XP,YP,ZP)
C                                        AVERAGE AXIS VALUES
      CALL AXIS(IS,IE,1,XU,YU,ZU,IDIM)
C
C    Save increment for BC on inversion
      I = (KA+KB)/2 + KTOT + IADD
      XJBDEL = (XU(I) - XA)*KPLNB(1)
      YJBDEL = (YU(I) - YA)*KPLNB(2)
      ZJBDEL = (ZU(I) - ZA)*KPLNB(3)
C
      DO 22 K = KA,KB
       I = K + KTOT + IADD
       X(J,K) = XU(I)*KPLNB(1)
       Y(J,K) = YU(I)*KPLNB(2)
       Z(J,K) = ZU(I)*KPLNB(3)
 22   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCCONV(IBCJA,IBCJB,IBCKA,IBCKB,JMAX,KMAX,JPER,KPER,
     >                  JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,J2D,
     >                  KSYMA,KSYMB,KFLTA,KFLTB,K2D,JPLN1,KPLN1,
     >                  JPLNA,JPLNB,KPLNA,KPLNB,EXTJA,EXTJB,EXTKA,EXTKB)
c*wdh*
c* include "precis.h"
C
      LOGICAL JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,J2D,
     >        KSYMA,KSYMB,KFLTA,KFLTB,K2D
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3),JPLN1(3),KPLN1(3)
C
C   --------------------------------------------------
C    Convert some BC parameters into old input format
C   --------------------------------------------------
C
C    Periodic bc parameters
      IF (IBCJA.EQ.10) THEN
       JPER = 1
      ELSE
       JPER = 0
      ENDIF
      IF (IBCKA.EQ.10) THEN
       KPER = 1
      ELSE
       KPER = 0
      ENDIF
C
      DO 10 I=1,3
       JPLNA(I) = 1
       JPLNB(I) = 1
       KPLNA(I) = 1
       KPLNB(I) = 1
 10   CONTINUE
       EXTJA = 0.0
       EXTJB = 0.0
       EXTKA = 0.0
       EXTKB = 0.0
C
C    Reflected symmetry parameters
      IF ( (IBCJA.GE.11) .AND. (IBCJA.LE.13) ) THEN
       JSYMA = .TRUE.
       JPLNA(IBCJA-10) = 0
      ELSE
       JSYMA = .FALSE.
      ENDIF
      IF ( (IBCJB.GE.11) .AND. (IBCJB.LE.13) ) THEN
       JSYMB = .TRUE.
       JPLNB(IBCJB-10) = 0
      ELSE
       JSYMB = .FALSE.
      ENDIF
      IF ( (IBCKA.GE.11) .AND. (IBCKA.LE.13) ) THEN
       KSYMA = .TRUE.
       KPLNA(IBCKA-10) = 0
      ELSE
       KSYMA = .FALSE.
      ENDIF
      IF ( (IBCKB.GE.11) .AND. (IBCKB.LE.13) ) THEN
       KSYMB = .TRUE.
       KPLNB(IBCKB-10) = 0
      ELSE
       KSYMB = .FALSE.
      ENDIF
C
C    Floating boundary parameters
      JFLTA = .FALSE.
      IF (IBCJA.LT.0) THEN
       JFLTA = .TRUE.
       IF (IBCJA.LT.-1) EXTJA = -FLOAT(IBCJA)/1000.0
      ELSE IF ( ( (IBCJA.GE.1) .AND. (IBCJA.LE.7) ) .OR.
     >          ( (IBCJA.GE.21) .AND. (IBCJA.LE.23) ) ) THEN
       JFLTA = .TRUE.
       IF (IBCJA.LE.3) THEN
        JPLNA(IBCJA) = 0
       ELSE IF (IBCJA.EQ.4) THEN
        JPLNA(2) = 0
        JPLNA(3) = 0
       ELSE IF (IBCJA.EQ.5) THEN
        JPLNA(1) = 0
        JPLNA(3) = 0
       ELSE IF (IBCJA.EQ.6) THEN
        JPLNA(1) = 0
        JPLNA(2) = 0
       ELSE IF (IBCJA.GE.21) THEN
        JPLNA(IBCJA-20) = 0
       ENDIF
      ENDIF
C
      JFLTB = .FALSE.
      IF (IBCJB.LT.0) THEN
       JFLTB = .TRUE.
       IF (IBCJB.LT.-1) EXTJB = -FLOAT(IBCJB)/1000.0
      ELSE IF ( ( (IBCJB.GE.1) .AND. (IBCJB.LE.7) ) .OR.
     >          ( (IBCJB.GE.21) .AND. (IBCJB.LE.23) ) ) THEN
       JFLTB = .TRUE.
       IF (IBCJB.LE.3) THEN
        JPLNB(IBCJB) = 0
       ELSE IF (IBCJB.EQ.4) THEN
        JPLNB(2) = 0
        JPLNB(3) = 0
       ELSE IF (IBCJB.EQ.5) THEN
        JPLNB(1) = 0
        JPLNB(3) = 0
       ELSE IF (IBCJB.EQ.6) THEN
        JPLNB(1) = 0
        JPLNB(2) = 0
       ELSE IF (IBCJB.GE.21) THEN
        JPLNB(IBCJB-20) = 0
       ENDIF
      ENDIF
C
      KFLTA = .FALSE.
      IF (IBCKA.LT.0) THEN
       KFLTA = .TRUE.
       IF (IBCKA.LT.-1) EXTKA = -FLOAT(IBCKA)/1000.0
      ELSE IF ( ( (IBCKA.GE.1) .AND. (IBCKA.LE.7) ) .OR.
     >          ( (IBCKA.GE.21) .AND. (IBCKA.LE.23) ) ) THEN
       KFLTA = .TRUE.
       IF (IBCKA.LE.3) THEN
        KPLNA(IBCKA) = 0
       ELSE IF (IBCKA.EQ.4) THEN
        KPLNA(2) = 0
        KPLNA(3) = 0
       ELSE IF (IBCKA.EQ.5) THEN
        KPLNA(1) = 0
        KPLNA(3) = 0
       ELSE IF (IBCKA.EQ.6) THEN
        KPLNA(1) = 0
        KPLNA(2) = 0
       ELSE IF (IBCKA.GE.21) THEN
        KPLNA(IBCKA-20) = 0
       ENDIF
      ENDIF
C
      KFLTB = .FALSE.
      IF (IBCKB.LT.0) THEN
       KFLTB = .TRUE.
       IF (IBCKB.LT.-1) EXTKB = -FLOAT(IBCKB)/1000.0
      ELSE IF ( ( (IBCKB.GE.1) .AND. (IBCKB.LE.7) ) .OR.
     >          ( (IBCKB.GE.21) .AND. (IBCKB.LE.23) ) ) THEN
       KFLTB = .TRUE.
       IF (IBCKB.LE.3) THEN
        KPLNB(IBCKB) = 0
       ELSE IF (IBCKB.EQ.4) THEN
        KPLNB(2) = 0
        KPLNB(3) = 0
       ELSE IF (IBCKB.EQ.5) THEN
        KPLNB(1) = 0
        KPLNB(3) = 0
       ELSE IF (IBCKB.EQ.6) THEN
        KPLNB(1) = 0
        KPLNB(2) = 0
       ELSE IF (IBCKB.GE.21) THEN
        KPLNB(IBCKB-20) = 0
       ENDIF
      ENDIF
C
C    Constant interior planes parameters
      IF ( (IBCJA.GE.21) .AND. (IBCJA.LE.23) ) THEN
       JPLN1(1) = 1-JPLNA(1)
       JPLN1(2) = 1-JPLNA(2)
       JPLN1(3) = 1-JPLNA(3)
      ELSE
       JPLN1(1) = 1
       JPLN1(2) = 1
       JPLN1(3) = 1
      ENDIF
      IF ( (IBCKA.GE.21) .AND. (IBCKA.LE.23) ) THEN
       KPLN1(1) = 1-KPLNA(1)
       KPLN1(2) = 1-KPLNA(2)
       KPLN1(3) = 1-KPLNA(3)
      ELSE
       KPLN1(1) = 1
       KPLN1(2) = 1
       KPLN1(3) = 1
      ENDIF
C
C    Axis parameters
      IF (IBCJA.EQ.20) THEN
       JAXSA = .TRUE.
      ELSE
       JAXSA = .FALSE.
      ENDIF
      IF (IBCJB.EQ.20) THEN
       JAXSB = .TRUE.
      ELSE
       JAXSB = .FALSE.
      ENDIF
C
C    Two-dimensional grid parameters
      IF ( (JMAX.EQ.1) .OR. (IBCJA.GT.20) ) THEN
       J2D = .TRUE.
      ELSE
       J2D = .FALSE.
      ENDIF
      IF ( (KMAX.EQ.1) .OR. (IBCKA.GT.20) ) THEN
       K2D = .TRUE.
      ELSE
       K2D = .FALSE.
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCEDJ(JDIM,KDIM,JA,JB,KA,KB,JFLTA,JFLTB,JPLNA,JPLNB,
     >                 EXTJA,EXTJB,A,B,C,F)
c*wdh*
c* include "precis.h"
C
      LOGICAL JFLTA,JFLTB
      DIMENSION JPLNA(3),JPLNB(3)
      DIMENSION A(JDIM,KDIM,3,3),B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),
     >          F(JDIM,KDIM,3)
C
C    Set up end point matrices in HFILTRX for floating edge bc in J
C
      IF (JFLTA) THEN
       EXTJAM = -1.0 + EXTJA
       DO 11 N=1,3
       DO 11 M=1,3
       DO 11 K=KA,KB
        A(JA-1,K,N,M) = 0.0
        B(JA-1,K,N,M) = 0.0
        C(JA-1,K,N,M) = 0.0
 11    CONTINUE
       DO 12 N=1,3
       DO 12 K=KA,KB
        F(JA-1,K,N) = 0.0
        A(JA-1,K,N,N) = EXTJA
        B(JA-1,K,N,N) = 1.0
        C(JA-1,K,N,N) = EXTJAM*JPLNA(N)
 12    CONTINUE
      ENDIF
C
      IF (JFLTB) THEN
       EXTJBM = -1.0 + EXTJB
       DO 21 N=1,3
       DO 21 M=1,3
       DO 21 K=KA,KB
        A(JB+1,K,N,M) = 0.0
        B(JB+1,K,N,M) = 0.0
        C(JB+1,K,N,M) = 0.0
 21    CONTINUE
       DO 22 N=1,3
       DO 22 K=KA,KB
        F(JB+1,K,N) = 0.0
        C(JB+1,K,N,N) = EXTJB
        B(JB+1,K,N,N) = 1.0
        A(JB+1,K,N,N) = EXTJBM*JPLNB(N)
 22    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCEDK(JDIM,KDIM,JA,JB,KA,KB,KFLTA,KFLTB,KPLNA,KPLNB,
     >                 EXTKA,EXTKB,A,B,C,F)
c*wdh*
c* include "precis.h"
C
      LOGICAL KFLTA,KFLTB
      DIMENSION KPLNA(3),KPLNB(3)
      DIMENSION A(JDIM,KDIM,3,3),B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),
     >          F(JDIM,KDIM,3)
C
C    Set up end point matrices in FILTRE for floating edge bc in K
C
      IF (KFLTA) THEN
       EXTKAM = -1.0 + EXTKA
       DO 11 N=1,3
       DO 11 M=1,3
       DO 11 J=JA,JB
        A(J,KA-1,N,M) = 0.0
        B(J,KA-1,N,M) = 0.0
        C(J,KA-1,N,M) = 0.0
 11    CONTINUE
       DO 12 N=1,3
       DO 12 J=JA,JB
        F(J,KA-1,N) = 0.0
        A(J,KA-1,N,N) = EXTKA
        B(J,KA-1,N,N) = 1.0
        C(J,KA-1,N,N) = EXTKAM*KPLNA(N)
 12    CONTINUE
      ENDIF
C
      IF (KFLTB) THEN
       EXTKBM = -1.0 + EXTKB
       DO 21 N=1,3
       DO 21 M=1,3
       DO 21 J=JA,JB
        A(J,KB+1,N,M) = 0.0
        B(J,KB+1,N,M) = 0.0
        C(J,KB+1,N,M) = 0.0
 21    CONTINUE
       DO 22 N=1,3
       DO 22 J=JA,JB
        F(J,KB+1,N) = 0.0
        C(J,KB+1,N,N) = EXTKB
        B(J,KB+1,N,N) = 1.0
        A(J,KB+1,N,N) = EXTKBM*KPLNB(N)
 22    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCSYMJ(JDIM,KDIM,JA,JB,KA,KB,JSYMA,JSYMB,JPLNA,JPLNB,
     >                  A,B,C,F)
c*wdh*
c* include "precis.h"
C
      LOGICAL JSYMA,JSYMB
      DIMENSION JPLNA(3),JPLNB(3)
      DIMENSION A(JDIM,KDIM,3,3),B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),
     >          F(JDIM,KDIM,3)
C
C    Set up end point matrices in HFILTRX for reflected symmetry bc in J.
C    For JSYMA, note that the element in the third column of the first
C    row of the LHS matrix is non-zero and is stored in A(1,K,N,N).
C    A special BTRI is used for this type of matrix. Similarly for JSYMB.
C
      IF (JSYMA) THEN
       DO 11 N=1,3
       DO 11 M=1,3
       DO 11 K=KA,KB
        A(JA-1,K,N,M) = 0.0
        B(JA-1,K,N,M) = 0.0
        C(JA-1,K,N,M) = 0.0
 11    CONTINUE
       DO 12 N=1,3
       DO 12 K=KA,KB
        F(JA-1,K,N) = 0.0
        B(JA-1,K,N,N) = 1.0
        A(JA-1,K,N,N) = 1.0 - 2*JPLNA(N)
 12    CONTINUE
      ENDIF
C
      IF (JSYMB) THEN
       DO 21 N=1,3
       DO 21 M=1,3
       DO 21 K=KA,KB
        A(JB+1,K,N,M) = 0.0
        B(JB+1,K,N,M) = 0.0
        C(JB+1,K,N,M) = 0.0
 21    CONTINUE
       DO 22 N=1,3
       DO 22 K=KA,KB
        F(JB+1,K,N) = 0.0
        B(JB+1,K,N,N) = 1.0
        C(JB+1,K,N,N) = 1.0 - 2*JPLNB(N)
 22    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCSYMK(JDIM,KDIM,JA,JB,KA,KB,KSYMA,KSYMB,KPLNA,KPLNB,
     >                  A,B,C,F)
c*wdh*
c* include "precis.h"
C
      LOGICAL KSYMA,KSYMB
      DIMENSION KPLNA(3),KPLNB(3)
      DIMENSION A(JDIM,KDIM,3,3),B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),
     >          F(JDIM,KDIM,3)
C
C    Set up end point matrices in FILTRE for reflected symmetry bc in K.
C    For KSYMA, note that the element in the third column of the first
C    row of the LHS matrix is non-zero and is stored in A(J,1,N,N).
C    A special BTRI is used for this type of matrix. Similarly for KSYMB.
C
      IF (KSYMA) THEN
       DO 11 N=1,3
       DO 11 M=1,3
       DO 11 J=JA,JB
        A(J,KA-1,N,M) = 0.0
        B(J,KA-1,N,M) = 0.0
        C(J,KA-1,N,M) = 0.0
 11    CONTINUE
       DO 12 N=1,3
       DO 12 J=JA,JB
        F(J,KA-1,N) = 0.0
        B(J,KA-1,N,N) = 1.0
        A(J,KA-1,N,N) = 1.0 - 2*KPLNA(N)
 12    CONTINUE
      ENDIF
C
      IF (KSYMB) THEN
       DO 21 N=1,3
       DO 21 M=1,3
       DO 21 J=JA,JB
        A(J,KB+1,N,M) = 0.0
        B(J,KB+1,N,M) = 0.0
        C(J,KB+1,N,M) = 0.0
 21    CONTINUE
       DO 22 N=1,3
       DO 22 J=JA,JB
        F(J,KB+1,N) = 0.0
        B(J,KB+1,N,N) = 1.0
        C(J,KB+1,N,N) = 1.0 - 2*KPLNB(N)
 22    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BNDUNJ(JDIM,KDIM,JMAX,KMAX,J,JN,KA,KB,KKP,KKR,JPLN,
     >                  IBCJ,IBCK,X,Y,Z,XZ,YZ,ZZ,BNX,BNY,BNZ,
     >                  PHI,NPA,IDIV,UNI)
c*wdh*
c* include "precis.h"
C
      PARAMETER (I2=2, ONE=1.0)
      DIMENSION KKP(KDIM), KKR(KDIM), JPLN(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION BNX(KDIM), BNY(KDIM), BNZ(KDIM),
     >          PHI(KDIM), NPA(KDIM), IDIV(KDIM), UNI(KDIM)
C
C   --------------------------------------------
C    Determine unit normal vector at J boundary
C   --------------------------------------------
C
C    Dot product tolerance for zero-thickness corner
      ZTTOL = 1.0E-4
C    Tolerance for switching sign of normal
      TOL = 0.001
C
      IF ( (IBCJ.GE.4) .AND. (IBCJ.LE.6) ) THEN
C
C      Set up unit normal at boundary for IBCJ = 4,5,6
       DO 10 K=KA,KB
        BNX(K) = 0.0
        BNY(K) = 0.0
        BNZ(K) = 0.0
        IF (IBCJ.EQ.4) THEN
         BNX(K) = 1.0
        ELSE IF (IBCJ.EQ.5) THEN
         BNY(K) = 1.0
        ELSE IF (IBCJ.EQ.6) THEN
         BNZ(K) = 1.0
        ENDIF
 10    CONTINUE
C
      ELSE IF ( (IBCJ.LE.3) .OR. (IBCJ.GE.21) ) THEN
C
C      Set up unit normal for constraining boundary plane
        TNX = 1. - JPLN(1)
        TNY = 1. - JPLN(2)
        TNZ = 1. - JPLN(3)
        IF ( (IBCJ.EQ.1) .OR. (IBCJ.EQ.21) ) IP = 1
        IF ( (IBCJ.EQ.2) .OR. (IBCJ.EQ.22) ) IP = 2
        IF ( (IBCJ.EQ.3) .OR. (IBCJ.EQ.23) ) IP = 3
C
       DO 20 K=KA,KB
C
C       Compute unit normal vector at J boundary
         KP = KKP(K)
         KR = KKR(K)
         XP = X(J,KP) - X(J,K)
         YP = Y(J,KP) - Y(J,K)
         ZP = Z(J,KP) - Z(J,K)
         XM = X(J,KR) - X(J,K)
         YM = Y(J,KR) - Y(J,K)
         ZM = Z(J,KR) - Z(J,K)
         DP = 1.0/SQRT( XP*XP + YP*YP + ZP*ZP )
         DM = 1.0/SQRT( XM*XM + YM*YM + ZM*ZM )
         DELX = XP*DP - XM*DM
         DELY = YP*DP - YM*DM
         DELZ = ZP*DP - ZM*DM
C
         ICLP = 0
         IF ((DELX.EQ.0.).AND.(DELY.EQ.0.).AND.(IP.EQ.3)) ICLP = 1
         IF ((DELY.EQ.0.).AND.(DELZ.EQ.0.).AND.(IP.EQ.1)) ICLP = 1
         IF ((DELZ.EQ.0.).AND.(DELX.EQ.0.).AND.(IP.EQ.2)) ICLP = 1
         IF (ICLP.EQ.1) THEN
          DELX = X(J,KP) - X(JN,K)
          DELY = Y(J,KP) - Y(JN,K)
          DELZ = Z(J,KP) - Z(JN,K)
         ENDIF
C
         AYX = DELY*TNZ - DELZ*TNY
         AZX = DELZ*TNY - DELY*TNZ
         AXY = DELX*TNZ - DELZ*TNX
         AZY = DELZ*TNX - DELX*TNZ
         AXZ = DELX*TNY - DELY*TNX
         AYZ = DELY*TNX - DELX*TNY
C
         IF ( (AYX.NE.0.0) .AND. (AZX.NE.0.0) ) THEN
          QY = AXY/AYX
          QZ = AXZ/AZX
          BNX(K) = 1.0/SQRT( 1.0 + QY*QY + QZ*QZ )
          BNY(K) = -QY*BNX(K)
          BNZ(K) = -QZ*BNX(K)
         ELSE IF ( (AXY.NE.0.0) .AND. (AZY.NE.0.0) ) THEN
          QX = AYX/AXY
          QZ = AYZ/AZY
          BNY(K) = 1.0/SQRT( 1.0 + QX*QX + QZ*QZ )
          BNX(K) = -QX*BNY(K)
          BNZ(K) = -QZ*BNY(K)
         ELSE IF ( (AXZ.NE.0.0) .AND. (AYZ.NE.0.0) ) THEN
          QX = AZX/AXZ
          QY = AZY/AYZ
          BNZ(K) = 1.0/SQRT( 1.0 + QX*QX + QY*QY )
          BNX(K) = -QX*BNZ(K)
          BNY(K) = -QY*BNZ(K)
         ENDIF
C
 20    CONTINUE
C
      ENDIF
C
      IF (IBCJ.LT.7) THEN
C
C      Determine if sign of normal need to be switched
       DO 30 K=KA,KB
C
        DOT = XZ(JN,K)*BNX(K) + YZ(JN,K)*BNY(K) + ZZ(JN,K)*BNZ(K)
        DZ  = SQRT(XZ(JN,K)**2 + YZ(JN,K)**2 + ZZ(JN,K)**2)
        DOTN = DOT/DZ
        IF (DOTN.LT.-TOL) THEN
         BNX(K) = -BNX(K)
         BNY(K) = -BNY(K)
         BNZ(K) = -BNZ(K)
        ELSE IF (ABS(DOTN).LE.TOL) THEN
         DIN = (X(J,K)-BNX(K)-X(JN,K))**2 + (Y(J,K)-BNY(K)-Y(JN,K))**2 +
     >         (Z(J,K)-BNZ(K)-Z(JN,K))**2
         DOUT= (X(J,K)+BNX(K)-X(JN,K))**2 + (Y(J,K)+BNY(K)-Y(JN,K))**2 +
     >         (Z(J,K)+BNZ(K)-Z(JN,K))**2
         IF (DIN.GT.DOUT) THEN
          BNX(K) = -BNX(K)
          BNY(K) = -BNY(K)
          BNZ(K) = -BNZ(K)
         ENDIF
        ENDIF
C
 30    CONTINUE
C
      ELSE IF (IBCJ.EQ.7) THEN
C
C      Compute angle-bisecting normal for one side of collapsed edge
        IF (IBCK.EQ.10) THEN
         KADD = 1
        ELSE
         KADD = 0
        ENDIF
        KMM   = KMAX+1+KADD
        KMID  = KMM/2
        KMIDM = KMID-1
        ISIGN = J-JN
C
        DO 40 K=2-KADD,KMID
C
         KP = K+1
         KR = K-1
         KC = MAX( I2, MIN(K,KMIDM) )
         KK = KMM-KC
C
C        Form differences with neighbors
          XJP = (X(JN,KK)-X(J,K))*ISIGN
          YJP = (Y(JN,KK)-Y(J,K))*ISIGN
          ZJP = (Z(JN,KK)-Z(J,K))*ISIGN
          XJR = (X(JN,KC)-X(J,K))*ISIGN
          YJR = (Y(JN,KC)-Y(J,K))*ISIGN
          ZJR = (Z(JN,KC)-Z(J,K))*ISIGN
          IF (K.LT.KMID) THEN
           XKP = X(J,KP)-X(J,K)
           YKP = Y(J,KP)-Y(J,K)
           ZKP = Z(J,KP)-Z(J,K)
          ELSE IF (K.EQ.KMID) THEN
           XKP = X(JN,K)-X(J,K)
           YKP = Y(JN,K)-Y(J,K)
           ZKP = Z(JN,K)-Z(J,K)
          ENDIF
          XKR = X(J,KR)-X(J,K)
          YKR = Y(J,KR)-Y(J,K)
          ZKR = Z(J,KR)-Z(J,K)
C
C        Form unit vectors
          VJP = SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP )
          VJR = SQRT( XJR*XJR + YJR*YJR + ZJR*ZJR )
          VKP = SQRT( XKP*XKP + YKP*YKP + ZKP*ZKP )
          VKR = SQRT( XKR*XKR + YKR*YKR + ZKR*ZKR )
          XCJ = (XJP/VJP) - (XJR/VJR)
          YCJ = (YJP/VJP) - (YJR/VJR)
          ZCJ = (ZJP/VJP) - (ZJR/VJR)
          XCK = (XKP/VKP) - (XKR/VKR)
          YCK = (YKP/VKP) - (YKR/VKR)
          ZCK = (ZKP/VKP) - (ZKR/VKR)
C
C        Form angle-bisecting normal for non-degenerate cases
          XNOR = YCJ*ZCK - ZCJ*YCK
          YNOR = ZCJ*XCK - XCJ*ZCK
          ZNOR = XCJ*YCK - YCJ*XCK
          DNOR = SQRT(XNOR*XNOR + YNOR*YNOR + ZNOR*ZNOR)
C
C        Compute normalized dot product differences
          DOTXI = ABS(1.0 - ((XJP*XJR + YJP*YJR + ZJP*ZJR)/(VJP*VJR)))
          DOTET = ABS(1.0 - ((XKP*XKR + YKP*YKR + ZKP*ZKR)/(VKP*VKR)))
C
C        Compute angle-bisecting unit normal at edge
          IF ( (DOTXI.GE.ZTTOL) .AND. (DOTET.GE.ZTTOL) ) THEN
           BNX(K) = XNOR/DNOR
           BNY(K) = YNOR/DNOR
           BNZ(K) = ZNOR/DNOR
          ELSE IF (DOTXI.LT.ZTTOL) THEN
           BNX(K) = -XJR/VJR
           BNY(K) = -YJR/VJR
           BNZ(K) = -ZJR/VJR
          ELSE IF (DOTET.LT.ZTTOL) THEN
           BNX(K) = -XKR/VKR
           BNY(K) = -YKR/VKR
           BNZ(K) = -ZKR/VKR
          ENDIF
C
 40     CONTINUE
C
C      Copy unit normals to the other side
        DO 50 K=2,KMID-1
         KK = KMM-K
         BNX(KK) = BNX(K)
         BNY(KK) = BNY(K)
         BNZ(KK) = BNZ(K)
 50     CONTINUE
C
      ENDIF
C
C    Determine how many interior points to be affected
      DO 60 K=KA,KB
       UNI(K) = SQRT(XZ(JN,K)**2 + YZ(JN,K)**2 + ZZ(JN,K)**2)
       DOT = ( BNX(K)*XZ(JN,K)+BNY(K)*YZ(JN,K)+BNZ(K)*ZZ(JN,K) )/UNI(K)
       DOT = MIN( ONE, ABS(DOT) )
       NPA(K) = MIN( INT( ABS(1.0-DOT)*20.0 ), JDIM/2 )
       PHI(K) = ACOS(DOT)
 60   CONTINUE
C
C    Determine if normals are diverging or converging
      IF (IBCJ.LT.7) THEN
       DO 70 K=KA,KB
        DBASE = SQRT( (X(JN,K)-X(J,K))**2 + (Y(JN,K)-Y(J,K))**2 + 
     >                (Z(JN,K)-Z(J,K))**2 )
        DB2 = 0.5*DBASE
        DN  = DB2/UNI(K)
        DNEW = SQRT(((X(JN,K) + XZ(JN,K)*DN)-(X(J,K) + BNX(K)*DB2))**2
     >        +     ((Y(JN,K) + YZ(JN,K)*DN)-(Y(J,K) + BNY(K)*DB2))**2
     >        +     ((Z(JN,K) + ZZ(JN,K)*DN)-(Z(J,K) + BNZ(K)*DB2))**2)
        IF (DNEW.GE.DBASE) THEN
         IDIV(K) = 1
        ELSE
         IDIV(K) = 0
        ENDIF
 70    CONTINUE
      ELSE IF (IBCJ.EQ.7) THEN
       DO 80 K=KA,KB
        IDIV(K) = 1
 80    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BNDUNK(JDIM,KDIM,JMAX,KMAX,JA,JB,K,KN,JJP,JJR,KPLN,
     >                  IBCK,IBCJ,X,Y,Z,XZ,YZ,ZZ,BNX,BNY,BNZ,
     >                  PHI,NPA,IDIV,UNI)
c*wdh*
c* include "precis.h"
C
      PARAMETER (I2=2, ONE=1.0)
      DIMENSION JJP(JDIM), JJR(JDIM), KPLN(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION BNX(JDIM), BNY(JDIM), BNZ(JDIM),
     >          PHI(JDIM), NPA(JDIM), IDIV(JDIM), UNI(JDIM)
C
C   --------------------------------------------
C    Determine unit normal vector at K boundary
C   --------------------------------------------
C
C    Dot product tolerance for zero-thickness corner
      ZTTOL = 1.0E-4
C    Tolerance for switching sign of normal
      TOL = 0.001
C
      IF ( (IBCK.GE.4) .AND. (IBCK.LE.6) ) THEN
C
C      Set up unit normal at boundary for IBCK = 4,5,6
       DO 10 J=JA,JB
        BNX(J) = 0.0
        BNY(J) = 0.0
        BNZ(J) = 0.0
        IF (IBCK.EQ.4) THEN
         BNX(J) = 1.0
        ELSE IF (IBCK.EQ.5) THEN
         BNY(J) = 1.0
        ELSE IF (IBCK.EQ.6) THEN
         BNZ(J) = 1.0
        ENDIF
 10    CONTINUE
C
      ELSE IF ( (IBCK.LE.3) .OR. (IBCK.GE.21) ) THEN
C
C      Set up unit normal for constraining boundary plane
        TNX = 1. - KPLN(1)
        TNY = 1. - KPLN(2)
        TNZ = 1. - KPLN(3)
        IF ( (IBCK.EQ.1) .OR. (IBCK.EQ.21) ) IP = 1
        IF ( (IBCK.EQ.2) .OR. (IBCK.EQ.22) ) IP = 2
        IF ( (IBCK.EQ.3) .OR. (IBCK.EQ.23) ) IP = 3
C
       DO 20 J=JA,JB
C
C       Compute unit normal vector at K boundary
         JP = JJP(J)
         JR = JJR(J)
         XP = X(JP,K) - X(J,K)
         YP = Y(JP,K) - Y(J,K)
         ZP = Z(JP,K) - Z(J,K)
         XM = X(JR,K) - X(J,K)
         YM = Y(JR,K) - Y(J,K)
         ZM = Z(JR,K) - Z(J,K)
         DP = 1.0/SQRT( XP*XP + YP*YP + ZP*ZP )
         DM = 1.0/SQRT( XM*XM + YM*YM + ZM*ZM )
         DELX = XP*DP - XM*DM
         DELY = YP*DP - YM*DM
         DELZ = ZP*DP - ZM*DM
C
         ICLP = 0
         IF ((DELX.EQ.0.).AND.(DELY.EQ.0.).AND.(IP.EQ.3)) ICLP = 1
         IF ((DELY.EQ.0.).AND.(DELZ.EQ.0.).AND.(IP.EQ.1)) ICLP = 1
         IF ((DELZ.EQ.0.).AND.(DELX.EQ.0.).AND.(IP.EQ.2)) ICLP = 1
         IF (ICLP.EQ.1) THEN
          DELX = X(JP,K) - X(J,KN)
          DELY = Y(JP,K) - Y(J,KN)
          DELZ = Z(JP,K) - Z(J,KN)
         ENDIF
C
         AYX = DELY*TNZ - DELZ*TNY
         AZX = DELZ*TNY - DELY*TNZ
         AXY = DELX*TNZ - DELZ*TNX
         AZY = DELZ*TNX - DELX*TNZ
         AXZ = DELX*TNY - DELY*TNX
         AYZ = DELY*TNX - DELX*TNY
C
         IF ( (AYX.NE.0.0) .AND. (AZX.NE.0.0) ) THEN
          QY = AXY/AYX
          QZ = AXZ/AZX
          BNX(J) = 1.0/SQRT( 1.0 + QY*QY + QZ*QZ )
          BNY(J) = -QY*BNX(J)
          BNZ(J) = -QZ*BNX(J)
         ELSE IF ( (AXY.NE.0.0) .AND. (AZY.NE.0.0) ) THEN
          QX = AYX/AXY
          QZ = AYZ/AZY
          BNY(J) = 1.0/SQRT( 1.0 + QX*QX + QZ*QZ )
          BNX(J) = -QX*BNY(J)
          BNZ(J) = -QZ*BNY(J)
         ELSE IF ( (AXZ.NE.0.0) .AND. (AYZ.NE.0.0) ) THEN
          QX = AZX/AXZ
          QY = AZY/AYZ
          BNZ(J) = 1.0/SQRT( 1.0 + QX*QX + QY*QY )
          BNX(J) = -QX*BNZ(J)
          BNY(J) = -QY*BNZ(J)
         ENDIF
C
 20    CONTINUE
C
      ENDIF
C
      IF (IBCK.LT.7) THEN
C
C      Determine if sign of normal need to be switched
       DO 30 J=JA,JB
C
        DOT = XZ(J,KN)*BNX(J) + YZ(J,KN)*BNY(J) + ZZ(J,KN)*BNZ(J)
        DZ  = SQRT(XZ(J,KN)**2 + YZ(J,KN)**2 + ZZ(J,KN)**2)
        DOTN = DOT/DZ
        IF (DOTN.LT.-TOL) THEN
         BNX(J) = -BNX(J)
         BNY(J) = -BNY(J)
         BNZ(J) = -BNZ(J)
        ELSE IF (ABS(DOTN).LE.TOL) THEN
         DIN = (X(J,K)-BNX(J)-X(J,KN))**2 + (Y(J,K)-BNY(J)-Y(J,KN))**2 +
     >         (Z(J,K)-BNZ(J)-Z(J,KN))**2
         DOUT= (X(J,K)+BNX(J)-X(J,KN))**2 + (Y(J,K)+BNY(J)-Y(J,KN))**2 +
     >         (Z(J,K)+BNZ(J)-Z(J,KN))**2
         IF (DIN.GT.DOUT) THEN
          BNX(J) = -BNX(J)
          BNY(J) = -BNY(J)
          BNZ(J) = -BNZ(J)
         ENDIF
        ENDIF
C
 30    CONTINUE
C
      ELSE IF (IBCK.EQ.7) THEN
C
C      Compute angle-bisecting normal for one side of collapsed edge
        IF (IBCJ.EQ.10) THEN
         JADD = 1
        ELSE
         JADD = 0
        ENDIF
        JMM   = JMAX+1+JADD
        JMID  = JMM/2
        JMIDM = JMID-1
        ISIGN = K-KN
C
        DO 40 J=2-JADD,JMID
C
         JP = J+1
         JR = J-1
         JC = MAX( I2, MIN(J,JMIDM) )
         JJ = JMM-JC
C
C        Form differences with neighbors
          XKP = (X(JJ,KN)-X(J,K))*ISIGN
          YKP = (Y(JJ,KN)-Y(J,K))*ISIGN
          ZKP = (Z(JJ,KN)-Z(J,K))*ISIGN
          XKR = (X(JC,KN)-X(J,K))*ISIGN
          YKR = (Y(JC,KN)-Y(J,K))*ISIGN
          ZKR = (Z(JC,KN)-Z(J,K))*ISIGN
          IF (J.LT.JMID) THEN
           XJP = X(JP,K)-X(J,K)
           YJP = Y(JP,K)-Y(J,K)
           ZJP = Z(JP,K)-Z(J,K)
          ELSE IF (J.EQ.JMID) THEN
           XJP = X(J,KN)-X(J,K)
           YJP = Y(J,KN)-Y(J,K)
           ZJP = Z(J,KN)-Z(J,K)
          ENDIF
          IF (J.GT.1) THEN
           XJR = X(JR,K)-X(J,K)
           YJR = Y(JR,K)-Y(J,K)
           ZJR = Z(JR,K)-Z(J,K)
          ELSE IF (J.EQ.1) THEN
           XJR = X(J,KN)-X(J,K)
           YJR = Y(J,KN)-Y(J,K)
           ZJR = Z(J,KN)-Z(J,K)
          ENDIF
C
C        Form unit vectors
          VJP = SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP )
          VJR = SQRT( XJR*XJR + YJR*YJR + ZJR*ZJR )
          VKP = SQRT( XKP*XKP + YKP*YKP + ZKP*ZKP )
          VKR = SQRT( XKR*XKR + YKR*YKR + ZKR*ZKR )
          XCJ = (XJP/VJP) - (XJR/VJR)
          YCJ = (YJP/VJP) - (YJR/VJR)
          ZCJ = (ZJP/VJP) - (ZJR/VJR)
          XCK = (XKP/VKP) - (XKR/VKR)
          YCK = (YKP/VKP) - (YKR/VKR)
          ZCK = (ZKP/VKP) - (ZKR/VKR)
C
C        Form angle-bisecting normal for non-degenerate cases
          XNOR = YCJ*ZCK - ZCJ*YCK
          YNOR = ZCJ*XCK - XCJ*ZCK
          ZNOR = XCJ*YCK - YCJ*XCK
          DNOR = SQRT(XNOR*XNOR + YNOR*YNOR + ZNOR*ZNOR)
C
C        Compute normalized dot product differences
          DOTXI = ABS(1.0 - ((XJP*XJR + YJP*YJR + ZJP*ZJR)/(VJP*VJR)))
          DOTET = ABS(1.0 - ((XKP*XKR + YKP*YKR + ZKP*ZKR)/(VKP*VKR)))
C
C        Compute angle-bisecting unit normal at edge
          IF ( (DOTXI.GE.ZTTOL) .AND. (DOTET.GE.ZTTOL) ) THEN
           BNX(J) = XNOR/DNOR
           BNY(J) = YNOR/DNOR
           BNZ(J) = ZNOR/DNOR
          ELSE IF (DOTXI.LT.ZTTOL) THEN
           BNX(J) = -XJR/VJR
           BNY(J) = -YJR/VJR
           BNZ(J) = -ZJR/VJR
          ELSE IF (DOTET.LT.ZTTOL) THEN
           BNX(J) = -XKR/VKR
           BNY(J) = -YKR/VKR
           BNZ(J) = -ZKR/VKR
          ENDIF
C
 40     CONTINUE
C
C      Copy unit normals to the other side
        DO 50 J=2,JMID-1
         JJ = JMM-J
         BNX(JJ) = BNX(J)
         BNY(JJ) = BNY(J)
         BNZ(JJ) = BNZ(J)
 50     CONTINUE
C
      ENDIF
C
C    Determine how many interior points to be affected
      DO 60 J=JA,JB
       UNI(J) = SQRT(XZ(J,KN)**2 + YZ(J,KN)**2 + ZZ(J,KN)**2)
       DOT = ( BNX(J)*XZ(J,KN)+BNY(J)*YZ(J,KN)+BNZ(J)*ZZ(J,KN) )/UNI(J)
       DOT = MIN( ONE, ABS(DOT) )
       NPA(J) = MIN( INT( ABS(1.0-DOT)*20.0 ), KDIM/2 )
       PHI(J) = ACOS(DOT)
 60   CONTINUE
C
C    Determine if normals are diverging or converging
      IF (IBCK.LT.7) THEN
       DO 70 J=JA,JB
        DBASE = SQRT( (X(J,KN)-X(J,K))**2 + (Y(J,KN)-Y(J,K))**2 + 
     >                (Z(J,KN)-Z(J,K))**2 )
        DB2 = 0.5*DBASE
        DN  = DB2/UNI(J)
        DNEW = SQRT(((X(J,KN) + XZ(J,KN)*DN)-(X(J,K) + BNX(J)*DB2))**2
     >        +     ((Y(J,KN) + YZ(J,KN)*DN)-(Y(J,K) + BNY(J)*DB2))**2
     >        +     ((Z(J,KN) + ZZ(J,KN)*DN)-(Z(J,K) + BNZ(J)*DB2))**2)
        IF (DNEW.GE.DBASE) THEN
         IDIV(J) = 1
        ELSE
         IDIV(J) = 0
        ENDIF
 70    CONTINUE
      ELSE IF (IBCK.EQ.7) THEN
       DO 80 J=JA,JB
        IDIV(J) = 1
 80    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CHKDIM(M3D,M2D,M1D,JDIM,KDIM,LDIM)
c*wdh*
c* include "precis.h"
C
C    Check if array dimensions are large enough
      N3D = JDIM*KDIM*LDIM
      N2D = JDIM*KDIM
      N1D = MAX(JDIM,KDIM,LDIM)
      IDSTAT = 1
C
      IF (N3D.GT.M3D) THEN
       WRITE(*,*)'Number of points in 3D grid greater than M3D'
       WRITE(*,*)'Recompile program with M3D > or = to ',N3D
       IDSTAT = 0
      ENDIF
      IF (N2D.GT.M2D) THEN
       WRITE(*,*)'Number of points in J-K plane greater than M2D'
       WRITE(*,*)'Recompile program with M2D > or = to ',N2D
       IDSTAT = 0
      ENDIF
      IF (N1D.GT.M1D) THEN
       WRITE(*,*)'Largest single dimension of grid greater than M1D'
       WRITE(*,*)'Recompile program with M1D > or = to ',N1D
       IDSTAT = 0
      ENDIF
      IF (IDSTAT.EQ.0) THEN
       WRITE(*,*)
       WRITE(*,*)'Program terminated due to array dimension error'
       STOP
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CHKFVJ(M2D,JDIM,KDIM,JMAX,KMAX,LMAX,JPER,KPER,
     >                  JVS,JVE,KVS,KVE,JJP,JJR,KKP,KKR,LBAD,NBAD,JKBAD,
     >                  X,Y,Z,VOL,XX,XY,XZ,YX,YY,YZ,ZX,ZY,ZZ)
c*wdh*
c* include "precis.h"
C
C   Compute Jacobians/volumes with the same algorithm as that in 
C   the flow solver OVERFLOW.
C
C   Notes:
C
C     o Finite volume formulation as written by Shigeru Obayashi (11/87)
C       (ref. Vinokur, "An Analysis of Finite-Difference and
C       Finite-Volume Formulations of Conservation Laws," NASA CR 177416,
C       June 1986).
C     o General grid topology coded by Pieter Buning (7/89). Jacobians at
C       boundaries fixed too.
C     o Conversion of code to HYPGEN format by William Chan (1/92)
C     o Symmetric Jacobian computation by William Chan (1/93)
C
      LOGICAL JSYMA,JSYMB,KSYMA,KSYMB,J2D,K2D
      DIMENSION JJP(JDIM), JJR(JDIM), KKP(KDIM), KKR(KDIM)
      DIMENSION LBAD(LMAX), NBAD(LMAX), JKBAD(M2D,2)
      DIMENSION X(JDIM,KDIM,LMAX), Y(JDIM,KDIM,LMAX), Z(JDIM,KDIM,LMAX)
      DIMENSION VOL(JDIM,KDIM)
      DIMENSION XX(JDIM,KDIM), XY(JDIM,KDIM), XZ(JDIM,KDIM),
     >          YX(JDIM,KDIM), YY(JDIM,KDIM), YZ(JDIM,KDIM),
     >          ZX(JDIM,KDIM,3), ZY(JDIM,KDIM,3), ZZ(JDIM,KDIM,3)
C
      PARAMETER ( FAC1=1./2.,FAC2=1./6. )
      PARAMETER ( EPS=1.E-30 )
C
      WRITE(*,*)
      WRITE(*,*)'************************************************'
      WRITE(*,*)'         Finite volume Jacobian check           '
      WRITE(*,*)'************************************************'
      ICLEAR = 1
      NBTOT = 0
      NBADMAX = 50
C
C    Set size of differencing (1-point or 2-point) at endpoints.
      JS1E  = 1
      IF (JPER.EQ.1) JS1E  = 2
      KS1E  = 1
      IF (KPER.EQ.1) KS1E  = 2
      LS1E  = 1
C
C    Set indices (overload jjp,jjr,kkp,kkr since these are not needed anymore)
      IF (JPER.EQ.0) THEN
       JJR(JVS) = JVS
       JJP(JVE) = JVE
      ENDIF
      IF (KPER.EQ.0) THEN
       KKR(KVS) = KVS
       KKP(KVE) = KVE
      ENDIF
C
C    Store JS1 and KS1 temporarily in JKBAD
      DO 10 J=JVS+1,JVE-1
       JKBAD(J,1) = 2
 10   CONTINUE
      DO 20 K=KVS+1,KVE-1
       JKBAD(K,2) = 2
 20   CONTINUE
      JKBAD(JVS,1) = JS1E
      JKBAD(JVE,1) = JS1E
      JKBAD(KVS,2) = KS1E
      JKBAD(KVE,2) = KS1E
C
C    Loop through in L
C
      DO 500 L=1,LMAX
C
C      Set L indices
        LP1 = L+1
        LM1 = L-1
        IF (L.EQ.1) LM1 = 1
        IF (L.EQ.LMAX) LP1 = LMAX
        LS1 = 2
        IF ((L.EQ.1).OR.(L.EQ.LMAX)) LS1 = LS1E
C
C      J-direction areas.
        DO 120 K = KVS,KVE
         KP1   = KKP(K)
         KM1   = KKR(K)
         KS1   = JKBAD(K,2)
         FACT  = FAC1/(KS1*LS1)
         DO 110 J = JVS,JVE
            DX1       = X(J,KP1,LP1) - X(J,KM1,LM1)
            DY1       = Y(J,KP1,LP1) - Y(J,KM1,LM1)
            DZ1       = Z(J,KP1,LP1) - Z(J,KM1,LM1)
            DX2       = X(J,KM1,LP1) - X(J,KP1,LM1)
            DY2       = Y(J,KM1,LP1) - Y(J,KP1,LM1)
            DZ2       = Z(J,KM1,LP1) - Z(J,KP1,LM1)
            XX(J,K) = FACT*( DY1*DZ2 - DY2*DZ1 ) 
            XY(J,K) = FACT*( DZ1*DX2 - DZ2*DX1 ) 
            XZ(J,K) = FACT*( DX1*DY2 - DX2*DY1 ) 
 110     CONTINUE
 120    CONTINUE
C
C      K-direction areas.
        DO 220 J = JVS,JVE
         JP1   = JJP(J)
         JM1   = JJR(J)
         JS1   = JKBAD(J,1)
         FACT  = FAC1/(JS1*LS1)
         DO 210 K = KVS,KVE
            DX1       = X(JP1,K,LP1) - X(JM1,K,LM1)
            DY1       = Y(JP1,K,LP1) - Y(JM1,K,LM1)
            DZ1       = Z(JP1,K,LP1) - Z(JM1,K,LM1)
            DX2       = X(JP1,K,LM1) - X(JM1,K,LP1)
            DY2       = Y(JP1,K,LM1) - Y(JM1,K,LP1)
            DZ2       = Z(JP1,K,LM1) - Z(JM1,K,LP1)
            YX(J,K) = FACT*( DY1*DZ2 - DY2*DZ1 ) 
            YY(J,K) = FACT*( DZ1*DX2 - DZ2*DX1 ) 
            YZ(J,K) = FACT*( DX1*DY2 - DX2*DY1 ) 
 210     CONTINUE
 220    CONTINUE
C
C      L-direction areas.
        DO 320 K = KVS,KVE
         KP1   = KKP(K)
         KM1   = KKR(K)
         KS1   = JKBAD(K,2)
        DO 320 J = JVS,JVE
         JP1   = JJP(J)
         JM1   = JJR(J)
         JS1   = JKBAD(J,1)
         FACT  = FAC1/(JS1*KS1)
            DX1       = X(JP1,KP1,LM1) - X(JM1,KM1,LM1)
            DY1       = Y(JP1,KP1,LM1) - Y(JM1,KM1,LM1)
            DZ1       = Z(JP1,KP1,LM1) - Z(JM1,KM1,LM1)
            DX2       = X(JM1,KP1,LM1) - X(JP1,KM1,LM1)
            DY2       = Y(JM1,KP1,LM1) - Y(JP1,KM1,LM1)
            DZ2       = Z(JM1,KP1,LM1) - Z(JP1,KM1,LM1)
            ZX(J,K,1) = FACT*( DY1*DZ2 - DY2*DZ1 ) 
            ZY(J,K,1) = FACT*( DZ1*DX2 - DZ2*DX1 ) 
            ZZ(J,K,1) = FACT*( DX1*DY2 - DX2*DY1 ) 
            DX1       = X(JP1,KP1,LP1) - X(JM1,KM1,LP1)
            DY1       = Y(JP1,KP1,LP1) - Y(JM1,KM1,LP1)
            DZ1       = Z(JP1,KP1,LP1) - Z(JM1,KM1,LP1)
            DX2       = X(JM1,KP1,LP1) - X(JP1,KM1,LP1)
            DY2       = Y(JM1,KP1,LP1) - Y(JP1,KM1,LP1)
            DZ2       = Z(JM1,KP1,LP1) - Z(JP1,KM1,LP1)
            ZX(J,K,3) = FACT*( DY1*DZ2 - DY2*DZ1 ) 
            ZY(J,K,3) = FACT*( DZ1*DX2 - DZ2*DX1 ) 
            ZZ(J,K,3) = FACT*( DX1*DY2 - DX2*DY1 ) 
 320    CONTINUE
C
C      Compute Jacobians and find minimum in a J-K plane
        VOLMIN = 10000.0
        DO 400 K = KVS,KVE
         KP1   = KKP(K)
         KM1   = KKR(K)
         KS1   = JKBAD(K,2)
         KLS1 = KS1*LS1
        DO 400 J = JVS,JVE
         JP1   = JJP(J)
         JM1   = JJR(J)
         JS1   = JKBAD(J,1)
         JLS1 = JS1*LS1
         JKS1 = JS1*KS1
C
         SJX = (XX(JP1,K) + XX(JM1,K))*KLS1
         SJY = (XY(JP1,K) + XY(JM1,K))*KLS1
         SJZ = (XZ(JP1,K) + XZ(JM1,K))*KLS1
         SKX = (YX(J,KP1) + YX(J,KM1))*JLS1
         SKY = (YY(J,KP1) + YY(J,KM1))*JLS1
         SKZ = (YZ(J,KP1) + YZ(J,KM1))*JLS1
         SLX = (ZX(J,K,3) + ZX(J,K,1))*JKS1
         SLY = (ZY(J,K,3) + ZY(J,K,1))*JKS1
         SLZ = (ZZ(J,K,3) + ZZ(J,K,1))*JKS1
C
         V17 = (SJX+SKX+SLX)*(X(JP1,KP1,LP1)-X(JM1,KM1,LM1)) +
     >         (SJY+SKY+SLY)*(Y(JP1,KP1,LP1)-Y(JM1,KM1,LM1)) +
     >         (SJZ+SKZ+SLZ)*(Z(JP1,KP1,LP1)-Z(JM1,KM1,LM1))
         V28 = (SJX-SKX+SLX)*(X(JP1,KM1,LP1)-X(JM1,KP1,LM1)) +
     >         (SJY-SKY+SLY)*(Y(JP1,KM1,LP1)-Y(JM1,KP1,LM1)) +
     >         (SJZ-SKZ+SLZ)*(Z(JP1,KM1,LP1)-Z(JM1,KP1,LM1))
         V35 = (-SJX-SKX+SLX)*(X(JM1,KM1,LP1)-X(JP1,KP1,LM1)) +
     >         (-SJY-SKY+SLY)*(Y(JM1,KM1,LP1)-Y(JP1,KP1,LM1)) +
     >         (-SJZ-SKZ+SLZ)*(Z(JM1,KM1,LP1)-Z(JP1,KP1,LM1))
         V46 = (-SJX+SKX+SLX)*(X(JM1,KP1,LP1)-X(JP1,KM1,LM1)) +
     >         (-SJY+SKY+SLY)*(Y(JM1,KP1,LP1)-Y(JP1,KM1,LM1)) +
     >         (-SJZ+SKZ+SLZ)*(Z(JM1,KP1,LP1)-Z(JP1,KM1,LM1))
C
         FM = FAC2/(JS1*KS1*LS1)
         V17 = V17*FM
         V28 = V28*FM
         V35 = V35*FM
         V46 = V46*FM
         VOL(J,K) = 0.25*(V17+V28+V35+V46)
C
         VOLMIN= MIN(VOL(J,K),VOLMIN)
c         IF (VOL(J,K).LT.0.0) THEN
c          WRITE(12,*) J,K,L,V17,V28,V35,V46,VOL(J,K)
c         ENDIF
 400    CONTINUE
c        if (l.eq.3) then
c         do 881 j=jvs,jve
c          write(11,*) j,vol(j,8)
c 881     continue
c        endif
C
C      Find and report on negative Jacobians. First NBADMAX reported.
        IF (VOLMIN.LT.0.0) THEN
         ICLEAR = 0
         NFAIL = 0
         WRITE(*,601) L
 601     FORMAT('L =',I3,'   Negative Jacobians found at (J,K) =')
         DO 610 K = KVS,KVE
         DO 610 J = JVS,JVE
          IF (VOL(J,K).LT.0.) THEN
           NFAIL = NFAIL+1
           JKBAD(NFAIL,1) = J-JVS+1
           JKBAD(NFAIL,2) = K-KVS+1
          ENDIF
 610     CONTINUE
         NLOOP = MIN(NFAIL/5,NBADMAX/5)
         IIS = 1
         IIE = 0
         DO 620 N=1,NLOOP
          IIS = (N-1)*5 + 1
          IIE = IIS+4
          WRITE(*,621) ('(',JKBAD(II,1),',',JKBAD(II,2),') ',II=IIS,IIE)
 620     CONTINUE
         IF ((NFAIL.GT.IIE).AND.(NFAIL.LE.NBADMAX)) THEN
          WRITE(*,621) ('(',JKBAD(II,1),',',JKBAD(II,2),') ',
     >                      II=IIE+1,NFAIL)
         ENDIF
 621     FORMAT('         ',5(A,I3,A,I3,A))
         IF (NFAIL.GT.NBADMAX) THEN
          WRITE(*,622)
 622      FORMAT('... Additional locations will not be reported')
         ENDIF
         NBTOT = NBTOT + 1
         LBAD(NBTOT) = L
         NBAD(NBTOT) = NFAIL
        ENDIF
C
 500  CONTINUE
C
C    Write out summary message
      WRITE(*,*)'----------------------------------'
      WRITE(*,*)'Summary of negative Jacobian check'
      WRITE(*,*)'----------------------------------'
      IF (ICLEAR.EQ.0) THEN
       WRITE(*,*)'NB = Number of cells with negative Jacobian'
       WRITE(*,*)
       NLOOP = NBTOT/10
       LLS = 1
       LLE = 0
       DO 700 N=1,NLOOP
        LLS = (N-1)*10 + 1
        LLE = LLS+9
        WRITE(*,701) (LBAD(LL),LL=LLS,LLE)
        WRITE(*,702) (NBAD(LL),LL=LLS,LLE)
        WRITE(*,*)
 700   CONTINUE
       IF (NBTOT.GT.LLE) THEN
        WRITE(*,701) (LBAD(LL),LL=LLE+1,NBTOT)
        WRITE(*,702) (NBAD(LL),LL=LLE+1,NBTOT)
       ENDIF
 701   FORMAT(' L =',10I5)
 702   FORMAT('NB =',10I5)
      ELSE IF (ICLEAR.EQ.1) THEN
       WRITE(*,*)
       WRITE(*,*)' No negative Jacobians found in entire mesh'
       WRITE(*,*)
      ENDIF
C
      RETURN
      END      
C***********************************************************************
      SUBROUTINE CHKGRD(JDIM,KDIM,JMAX,KMAX,JPER,KPER,X,Y,Z)
c*wdh*
c* include "precis.h"
C
      PARAMETER (I1=1,I2=2)
      DIMENSION X(JDIM,KDIM), Y(JDIM,KDIM), Z(JDIM,KDIM)
C
C    Check for consecutive coincident points along an interior
C    coordinate line
C
      JS = MIN(I2,JMAX)
      JE = MAX(I1,JMAX-1)
      KS = MIN(I2,KMAX)
      KE = MAX(I1,KMAX-1)
C
       DO 10 K=KS,KE
        KP = K+1
        IF ((KPER.EQ.1).AND.(K.EQ.KE)) KP = 1
       DO 10 J=JS,JE
        JP = J+1
        IF ((JPER.EQ.1).AND.(J.EQ.JE)) JP = 1
        IF (JMAX.NE.1) THEN
         IF ((X(J,K).EQ.X(JP,K)).AND.(Y(J,K).EQ.Y(JP,K)).AND.
     >       (Z(J,K).EQ.Z(JP,K)) ) THEN
          WRITE(*,*)'Consecutive coincident points found at ',J,K,
     >              ' and ', JP,K
          WRITE(*,*)'Program terminated'
          STOP
         ENDIF
        ENDIF
        IF (KMAX.NE.1) THEN
         IF ((X(J,K).EQ.X(J,KP)).AND.(Y(J,K).EQ.Y(J,KP)).AND.
     >       (Z(J,K).EQ.Z(J,KP)) ) THEN
          WRITE(*,*)'Consecutive coincident points found at ',J,K,
     >              ' and ', J,KP
          WRITE(*,*)'Program terminated'
          STOP
         ENDIF
        ENDIF
 10    CONTINUE
C
      RETURN
      END      
C***********************************************************************
      SUBROUTINE CHKINP(IZSTRT,NZREG,NPZREG,ZREG,DZ0,DZ1,
     >                  IBCJA,IBCJB,IBCKA,IBCKB,
     >                  IVSPEC,EPSSS,ITSVOL,IMETH,SMU2,TIMJ,TIMK,
     >                  IAXIS,EXAXIS,VOLRES,VZETA,JMAX,KMAX,LMAX)
c*wdh*
c* include "precis.h"
C
C
      LOGICAL VZETA
      DIMENSION NPZREG(NZREG), ZREG(NZREG), DZ0(NZREG), DZ1(NZREG)
C
C   -----------------------------------------
C    Check validity of user input parameters
C   -----------------------------------------
C
      IOK = 1
C
C    Stretching checks
      IF ((IZSTRT.NE.-1).AND.(IZSTRT.NE.1).AND.(IZSTRT.NE.2)) THEN
       WRITE(*,*)'Warning: IZSTRT is not in valid range, reset to 1'
       IZSTRT = 1
      ENDIF
C
C    L-regions checks
      IF (NZREG.GT.LMAX/2) THEN
       WRITE(*,*)'Error: Too many L-regions relative to no. of points',
     >           ' in each region.'
       IOK = 0
      ENDIF
      DO 1 NR=1,NZREG
       IF (NPZREG(NR).LT.2) THEN
        WRITE(*,*)'Error: At least 2 points must be present in ',
     >            'an L-region'
        IOK = 0
       ENDIF
 1    CONTINUE
C
C    Check for variable zeta in outer L-regions
      IF (NZREG.GT.1) THEN
       DO 2 NR=2,NZREG
        IF ( (ZREG(NR).LE.0.) .OR. (DZ0(NR).LT.0.) .OR.
     >       (DZ1(NR).LT.0.) ) THEN
         WRITE(*,*)'Error: variable far field, initial/end spacings ',
     >             'can only be used in the first L-region.'
         IOK = 0
        ENDIF
 2     CONTINUE
      ENDIF
C
C    Mixed user defined stretching and multiple L-reg./var zeta checks
      IF (IZSTRT.EQ.-1) THEN
       IF (NZREG.GT.1) THEN
       WRITE(*,*)'Warning: User-defined stretching can only be used ',
     >           'with one L-region.'
       WRITE(*,*)'         Additional L-regions are disregarded.'
       ENDIF
       IF (VZETA) THEN
       WRITE(*,*)'Warning: User-defined stretching cannot be used with',
     >           'var. far field, initial/end spacing.'
       WRITE(*,*)'ZREG, DZ0, DZ1 are disregarded.'
       ENDIF
      ENDIF
C
C    Boundary condition checks for J-direction
      IF (IBCJA.LT.-1000) IBCJA = -1000
      IF (IBCJB.LT.-1000) IBCJB = -1000
      IF ( (IBCJA.EQ.0) .OR. (IBCJA.EQ.8) .OR. (IBCJA.EQ.9) .OR.
     >     ((IBCJA.GE.14).AND.(IBCJA.LE.19)) .OR. (IBCJA.GE.24) ) THEN
       WRITE(*,*)'Error: Invalid bc type at JA'
       IOK = 0
      ENDIF
      IF ( (IBCJB.EQ.0) .OR. (IBCJB.EQ.8) .OR. (IBCJB.EQ.9) .OR.
     >     ((IBCJB.GE.14).AND.(IBCJB.LE.19)) .OR. (IBCJB.GE.24) ) THEN
       WRITE(*,*)'Error: Invalid bc type at JB'
       IOK = 0
      ENDIF
C
C    Boundary condition checks for K-direction
      IF (IBCKA.LT.-1000) IBCKA = -1000
      IF (IBCKB.LT.-1000) IBCKB = -1000
      IF ( (IBCKA.EQ.0) .OR. (IBCKA.EQ.8) .OR. (IBCKA.EQ.9) .OR.
     >     ((IBCKA.GE.14).AND.(IBCKA.LE.19)) .OR. (IBCKA.GE.24) ) THEN
       WRITE(*,*)'Error: Invalid bc type at KA'
       IOK = 0
      ENDIF
      IF ( (IBCKB.EQ.0) .OR. (IBCKB.EQ.8) .OR. (IBCKB.EQ.9) .OR.
     >     ((IBCKB.GE.14).AND.(IBCKB.LE.19)) .OR. (IBCKB.GE.24) ) THEN
       WRITE(*,*)'Error: Invalid bc type at KB'
       IOK = 0
      ENDIF
C
C    Boundary condition checks for axis
      IF ( (IBCKA.EQ.20).OR.(IBCKB.EQ.20) ) THEN
       WRITE(*,*)'Error: Axis bc cannot be activated in K'
       IOK = 0
      ENDIF
      IF ( (IBCJA.EQ.20).OR.(IBCJB.EQ.20) ) THEN
       IF ( (IBCKA.LT.0).OR.(IBCKB.LT.0) ) THEN
        WRITE(*,*)'Error: Floating bc in K cannot be used with axis',
     >            ' bc in J'
        IOK = 0
       ENDIF
       IF ( (IBCKA.GT.20).OR.(IBCKB.GT.20) ) THEN
        WRITE(*,*)'Error: Axis bc in J cannot be used with all',
     >            ' constant planes bc in K'
        IOK = 0
       ENDIF
       IF ( (IBCKA.EQ.7).OR.(IBCKB.EQ.7) ) THEN
        WRITE(*,*)'Error: Axis bc in J cannot be used with',
     >            ' collapsed edge bc in K'
        IOK = 0
       ENDIF
      ENDIF
C
C    Check BC's that must be applied to both ends
      IF ( ( (IBCJA.EQ.10).AND.(IBCJB.NE.10) ) .OR.
     >     ( (IBCJB.EQ.10).AND.(IBCJA.NE.10) ) ) THEN  
       WRITE(*,*)'Error: Periodic bc must be applied to both JA and JB'
       IOK = 0
      ENDIF
      IF ( ( (IBCKA.EQ.10).AND.(IBCKB.NE.10) ) .OR.
     >     ( (IBCKB.EQ.10).AND.(IBCKA.NE.10) ) ) THEN  
       WRITE(*,*)'Error: Periodic bc must be applied to both KA and KB'
       IOK = 0
      ENDIF
      IF ( ( (IBCJA.EQ.21).AND.(IBCJB.NE.21) ) .OR.
     >     ( (IBCJB.EQ.21).AND.(IBCJA.NE.21) ) .OR.
     >     ( (IBCJA.EQ.22).AND.(IBCJB.NE.22) ) .OR.
     >     ( (IBCJB.EQ.22).AND.(IBCJA.NE.22) ) .OR.
     >     ( (IBCJA.EQ.23).AND.(IBCJB.NE.23) ) .OR.
     >     ( (IBCJB.EQ.23).AND.(IBCJA.NE.23) ) ) THEN
       WRITE(*,*)'Error: Constant interior planes bc must be applied',
     >           ' to both JA and JB'
       IOK = 0
      ENDIF
      IF ( ( (IBCKA.EQ.21).AND.(IBCKB.NE.21) ) .OR.
     >     ( (IBCKB.EQ.21).AND.(IBCKA.NE.21) ) .OR.
     >     ( (IBCKA.EQ.22).AND.(IBCKB.NE.22) ) .OR.
     >     ( (IBCKB.EQ.22).AND.(IBCKA.NE.22) ) .OR.
     >     ( (IBCKA.EQ.23).AND.(IBCKB.NE.23) ) .OR.
     >     ( (IBCKB.EQ.23).AND.(IBCKA.NE.23) ) ) THEN
       WRITE(*,*)'Error: Constant interior planes bc must be applied',
     >           ' to both KA and KB'
       IOK = 0
      ENDIF
C
C    Volume specs checks
      IF ((IVSPEC.NE.1).AND.(IVSPEC.NE.2)) THEN
       WRITE(*,*)'Warning: IVSPEC is not in valid range, reset to 1'
       IVSPEC = 1
      ENDIF
      IF ((IVSPEC.EQ.2).AND.((JMAX.EQ.1).OR.(KMAX.EQ.1))) THEN
       WRITE(*,*)'Error: IVSPEC=2 cannot be used for 2D cases'
       IOK = 0
      ENDIF
      IF (ITSVOL.LT.0) THEN
       WRITE(*,*)'Warning: ITSVOL is negative, reset to 0'
       ITSVOL = 0
      ENDIF
C
C    Dissipation input checks, set smu4 to 0.0
      SMU4 = 0.0
      IF ((IMETH.LT.0).OR.(IMETH.GT.3)) THEN
       WRITE(*,*)'Warning: IMETH is not in valid range, reset to 1'
       IMETH = 1
      ENDIF
      IF (SMU2.LT.0.0) THEN
       WRITE(*,*)'Error: SMU2 must be positive'
       IOK = 0
      ENDIF
C
C    TIM factors checks
      IF (TIMJ .LT. 0.0) THEN
       WRITE(*,*)'Warning: TIMJ is negative, reset to 0'
       TIMJ = 0.0
      ENDIF
      IF (TIMK .LT. 0.0) THEN
       WRITE(*,*)'Warning: TIMK is negative, reset to 0'
       TIMK = 0.0
      ENDIF
C
C    Axis logic checks
      IF ( (IBCJA.EQ.20) .OR. (IBCJB.EQ.20) ) THEN
       IF ((IAXIS.NE.1).AND.(IAXIS.NE.2)) THEN
        WRITE(*,*)'Warning: IAXIS is not in valid range, reset to 1'
        IAXIS = 1
       ENDIF
       IF (VOLRES.LE.0.0) THEN
        WRITE(*,*)'Error: VOLRES must be positive'
        IOK = 0
       ENDIF
      ENDIF
C
      IF (IOK.EQ.0) THEN
       WRITE(*,*)
       WRITE(*,*)'*** PROGRAM TERMINATED DUE TO INVALID USER INPUT ***'
       STOP
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CHKSPT(JDIM,KDIM,JMAX,KMAX,X,Y,Z,ISJA,ISJB,ISKA,ISKB)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM), Y(JDIM,KDIM), Z(JDIM,KDIM)
C
C    Check for singular points at boundaries in J and K
C
       ISJA = 0
       J = 1
       X1 = X(J,1)
       Y1 = Y(J,1)
       Z1 = Z(J,1)
       DO 10 K=2,KMAX
        IF ((X(J,K).EQ.X1).AND.(Y(J,K).EQ.Y1).AND.(Z(J,K).EQ.Z1)) THEN
         ISJA = 1
         GO TO 15
        ENDIF
 10    CONTINUE
 15    CONTINUE
C
       ISJB = 0
       J = JMAX
       X1 = X(J,1)
       Y1 = Y(J,1)
       Z1 = Z(J,1)
       DO 20 K=2,KMAX
        IF ((X(J,K).EQ.X1).AND.(Y(J,K).EQ.Y1).AND.(Z(J,K).EQ.Z1)) THEN
         ISJB = 1
         GO TO 25
        ENDIF
 20    CONTINUE
 25    CONTINUE
C
       ISKA = 0
       K = 1
       X1 = X(1,K)
       Y1 = Y(1,K)
       Z1 = Z(1,K)
       DO 30 J=2,JMAX
        IF ((X(J,K).EQ.X1).AND.(Y(J,K).EQ.Y1).AND.(Z(J,K).EQ.Z1)) THEN
         ISKA = 1
         GO TO 35
        ENDIF
 30    CONTINUE
 35    CONTINUE
C
       ISKB = 0
       K = KMAX
       X1 = X(1,K)
       Y1 = Y(1,K)
       Z1 = Z(1,K)
       DO 40 J=2,JMAX
        IF ((X(J,K).EQ.X1).AND.(Y(J,K).EQ.Y1).AND.(Z(J,K).EQ.Z1)) THEN
         ISKB = 1
         GO TO 45
        ENDIF
 40    CONTINUE
 45    CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CHKSTR(JDIM,KDIM,JZS,KZS,LMAX,RR)
c*wdh*
c* include "precis.h"
C
      DIMENSION RR(JDIM,KDIM,LMAX)
C
C    Check for negative steps in zetastr.i file
      IOK = 1
      DO 10 K=1,KZS
      DO 10 J=1,JZS
      DO 10 L=1,LMAX-1
       ZSTEP = RR(J,K,L+1)-RR(J,K,L)
       IF (ZSTEP.LE.0.0) THEN
        IOK = 0
        GO TO 100
       ENDIF
 10   CONTINUE
C
 100  CONTINUE
      IF (IOK.EQ.0) THEN
       WRITE(*,*)'Program terminated due to negative step size in ',
     >           'zetastr.i file.'
       STOP
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CHKVAR(JDIM,KDIM,JMAX,KMAX,ZD,Z0,Z1,ZETAVR)
c*wdh*
c* include "precis.h"
C
      DIMENSION ZETAVR(JDIM,KDIM,3)
C
C    Check input zetavr array to make sure things are positive
      IOK = 1
      IF (ZD.LE.0.0) THEN
       DO 10 K=1,KMAX
       DO 10 J=1,JMAX
        IF (ZETAVR(J,K,1) .LE. 0.0) THEN
         IOK = 0
         GO TO 100
        ENDIF
 10    CONTINUE
      ENDIF
      IF (Z0.LT.0.0) THEN
       DO 20 K=1,KMAX
       DO 20 J=1,JMAX
        IF (ZETAVR(J,K,2) .LE. 0.0) THEN
         IOK = 0
         GO TO 100
        ENDIF
 20    CONTINUE
      ENDIF
      IF (Z1.LT.0.0) THEN
       DO 30 K=1,KMAX
       DO 30 J=1,JMAX
        IF (ZETAVR(J,K,3) .LE. 0.0) THEN
         IOK = 0
         GO TO 100
        ENDIF
 30    CONTINUE
      ENDIF
C
 100  CONTINUE
      IF (IOK.EQ.0) THEN
       WRITE(*,*)'Program terminated due to negative values in ',
     >           'zetavar.i file.'
       STOP
      ENDIF
C
      RETURN
      END
C***********************************************************************
c       Subroutine to check if a purported 3D grid is "proper". Imagine
c       each cell decomposed into 6 tetrahedra as in Fig. 3(b) of
c       Kordulla-Vinokur, "Efficient Computation of Volume in
c       Flow Predictions".  Compute the volume of each tetrahedron
c       using the vector formula.  If any of these volumes turn out
c       to be "negative", there are problems with the grid (probably).
c       If all these volumes are positive, the grid is OK (probably).
c       (I have no theorem about this.)
c       The word negative is in quotes because of the nature of
c       finite-precision arithmetic.  A tolerance is declared as a
c       parameter in the subroutine that does the bulk of the work.
c
c       Routine modified from D. Jespersen's proper3d.f   WMC (2/92)
C***********************************************************************
      SUBROUTINE CHKVOL(M2D,JDIM,KDIM,LMAX,JVS,JVE,KVS,KVE,LBAD,NBAD,
     >                  JKBAD,JJP,JJR,KKP,KKR,JPER,KPER,VMIN,X,Y,Z)
c*wdh*
c* include "precis.h"
      PARAMETER (TOL=2.**(-23))
c        parameter (tol=2.**(-46))
      DIMENSION LBAD(LMAX), NBAD(LMAX), JKBAD(M2D,2)
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM),VMIN(JDIM,KDIM)
      DIMENSION X(JDIM,KDIM,LMAX), Y(JDIM,KDIM,LMAX), Z(JDIM,KDIM,LMAX)
c
c     This is the formula for the volume of a tetrahedron.
         VOLTET(x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4)=
     >          (x2-x1)*( (y3-y1)*(z4-z1) - (y4-y1)*(z3-z1) )
     >        + (y2-y1)*( (z3-z1)*(x4-x1) - (x3-x1)*(z4-z1) )
     >        + (z2-z1)*( (x3-x1)*(y4-y1) - (x4-x1)*(y3-y1) )
c------------------------------------------------------------------------
c
      WRITE(*,*)
      WRITE(*,*)'************************************************'
      WRITE(*,*)'     Tetrahedral volume decomposition check     '
      WRITE(*,*)'************************************************'
      ICLEAR = 1
      NBTOT = 0
      NBADMAX = 50
c
c    Set indices
      IF (JPER.EQ.0) THEN
       JEND = JVE-1
      ELSE IF (JPER.EQ.1) THEN
       JEND = JVE
      ENDIF
      IF (KPER.EQ.0) THEN
       KEND = KVE-1
      ELSE IF (KPER.EQ.1) THEN
       KEND = KVE
      ENDIF
c
       DO 200 L=1,LMAX-1
c
        LP1 = L+1
        VOLMIN = 100000.0
        DO 90 K = KVS, KEND
         KP1 = KKP(K)
         DO 80 J = JVS, JEND
          JP1 = JJP(J)
c         Compute a typical volume for this cell:
             DIM =( (X(JP1,K,L)-X(J,K,L))**2 + (Y(JP1,K,L)-Y(J,K,L))**2
     >	               + (Z(JP1,K,L)-Z(J,K,L))**2
     >	          * (X(J,KP1,L)-X(J,K,L))**2 + (Y(J,KP1,L)-Y(J,K,L))**2
     >                 + (Z(J,KP1,L)-Z(J,K,L))**2
     >            * (X(J,K,LP1)-X(J,K,L))**2 + (Y(J,K,LP1)-Y(J,K,L))**2
     >	               + (Z(J,K,LP1)-Z(J,K,L))**2 ) ** 0.5
	      TOLNOW = TOL*DIM
              V1 = VOLTET(X(J,K,L),Y(J,K,L),Z(J,K,L),
     >                    X(JP1,K,L),Y(JP1,K,L),Z(JP1,K,L),
     >                    X(JP1,KP1,L),Y(JP1,KP1,L),Z(JP1,KP1,L),
     >                    X(JP1,KP1,LP1),Y(JP1,KP1,LP1),Z(JP1,KP1,LP1))
              V2 = VOLTET(X(J,K,L),Y(J,K,L),Z(J,K,L),
     >                    X(JP1,KP1,L),Y(JP1,KP1,L),Z(JP1,KP1,L),
     >                    X(J,KP1,L),Y(J,KP1,L),Z(J,KP1,L),
     >                    X(JP1,KP1,LP1),Y(JP1,KP1,LP1),Z(JP1,KP1,LP1))
              V3 = VOLTET(X(J,K,L),Y(J,K,L),Z(J,K,L),
     >                    X(JP1,K,LP1),Y(JP1,K,LP1),Z(JP1,K,LP1),
     >                    X(JP1,K,L),Y(JP1,K,L),Z(JP1,K,L),
     >                    X(JP1,KP1,LP1),Y(JP1,KP1,LP1),Z(JP1,KP1,LP1))
              V4 = VOLTET(X(J,K,L),Y(J,K,L),Z(J,K,L),
     >                    X(J,K,LP1),Y(J,K,LP1),Z(J,K,LP1),
     >                    X(JP1,K,LP1),Y(JP1,K,LP1),Z(JP1,K,LP1),
     >                    X(JP1,KP1,LP1),Y(JP1,KP1,LP1),Z(JP1,KP1,LP1))
              V5 = VOLTET(X(J,K,L),Y(J,K,L),Z(J,K,L),
     >                    X(J,KP1,L),Y(J,KP1,L),Z(J,KP1,L),
     >                    X(J,KP1,LP1),Y(J,KP1,LP1),Z(J,KP1,LP1),
     >                    X(JP1,KP1,LP1),Y(JP1,KP1,LP1),Z(JP1,KP1,LP1))
              V6 = VOLTET(X(J,K,L),Y(J,K,L),Z(J,K,L),
     >                    X(J,KP1,LP1),Y(J,KP1,LP1),Z(J,KP1,LP1),
     >                    X(J,K,LP1),Y(J,K,LP1),Z(J,K,LP1),
     >                    X(JP1,KP1,LP1),Y(JP1,KP1,LP1),Z(JP1,KP1,LP1))
              VMIN(J,K) = (MIN(V1,V2,V3,V4,V5,V6)) + TOLNOW
              VOLMIN = MIN(VOLMIN,VMIN(J,K))
 80       CONTINUE
 90      CONTINUE
c
C      Find and report on negative volumes.
        IF (VOLMIN.LE.0.0) THEN
         ICLEAR = 0
         NFAIL = 0
         WRITE(*,601) L
 601     FORMAT('L =',I3,'   Negative volumes found at (J,K) =')
         DO 610 K = KVS,KEND
         DO 610 J = JVS,JEND
          IF (VMIN(J,K).LE.0.0) THEN
           NFAIL = NFAIL+1
           JKBAD(NFAIL,1) = J-JVS+1
           JKBAD(NFAIL,2) = K-KVS+1
          ENDIF
 610     CONTINUE
         NLOOP = MIN(NFAIL/5,NBADMAX/5)
         IIS = 1
         IIE = 0
         DO 620 N=1,NLOOP
          IIS = (N-1)*5 + 1
          IIE = IIS+4
          WRITE(*,621) ('(',JKBAD(II,1),',',JKBAD(II,2),') ',II=IIS,IIE)
 620     CONTINUE
         IF ((NFAIL.GT.IIE).AND.(NFAIL.LE.NBADMAX)) THEN
          WRITE(*,621) ('(',JKBAD(II,1),',',JKBAD(II,2),') ',
     >                      II=IIE+1,NFAIL)
         ENDIF
 621     FORMAT('         ',5(A,I3,A,I3,A))
         IF (NFAIL.GT.NBADMAX) THEN
          WRITE(*,622)
 622      FORMAT('... Additional locations will not be reported')
         ENDIF
         NBTOT = NBTOT + 1
         LBAD(NBTOT) = L
         NBAD(NBTOT) = NFAIL
        ENDIF
c
 200   CONTINUE
c
C    Write out summary message
      WRITE(*,*)'-------------------------------------'
      WRITE(*,*)'Summary of negative cell volume check'
      WRITE(*,*)'-------------------------------------'
      IF (ICLEAR.EQ.0) THEN
       WRITE(*,*)'NB = Number of cells with negative cell volume'
       WRITE(*,*)
       NLOOP = NBTOT/10
       LLS = 1
       LLE = 0
       DO 700 N=1,NLOOP
        LLS = (N-1)*10 + 1
        LLE = LLS+9
        WRITE(*,701) (LBAD(LL),LL=LLS,LLE)
        WRITE(*,702) (NBAD(LL),LL=LLS,LLE)
        WRITE(*,*)
 700   CONTINUE
       IF (NBTOT.GT.LLE) THEN
        WRITE(*,701) (LBAD(LL),LL=LLE+1,NBTOT)
        WRITE(*,702) (NBAD(LL),LL=LLE+1,NBTOT)
       ENDIF
       WRITE(*,*)
 701   FORMAT(' L =',10I5)
 702   FORMAT('NB =',10I5)
      ELSE IF (ICLEAR.EQ.1) THEN
       WRITE(*,*)
       WRITE(*,*)' No negative cell volumes found in entire mesh'
       WRITE(*,*)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CINVA(JDIM,KDIM,JS,JE,KS,KE,
     >                 XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOL,AA)
c*wdh*
c* include "precis.h"
C
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION VOL(JDIM,KDIM),AA(JDIM,KDIM,3,3)
C
C    Compute C inverse times A on LHS of governing equations,
C    noting that the result is a symmetric matrix
C
      DO 10 K=KS,KE
      DO 10 J=JS,JE
C
       Z1 = XZ(J,K)/VOL(J,K)
       Z2 = YZ(J,K)/VOL(J,K)
       Z3 = ZZ(J,K)/VOL(J,K)
       C11 = YE(J,K)*Z3 - ZE(J,K)*Z2
       C21 = ZE(J,K)*Z1 - XE(J,K)*Z3
       C31 = XE(J,K)*Z2 - YE(J,K)*Z1
C       C12 = ZX(J,K)*Z2 - YX(J,K)*Z3
C       C22 = XX(J,K)*Z3 - ZX(J,K)*Z1
C       C32 = YX(J,K)*Z1 - XX(J,K)*Z2
       C13 = Z1
       C23 = Z2
       C33 = Z3
       A11 = XZ(J,K)
       A12 = YZ(J,K)
       A13 = ZZ(J,K)
       A31 = (YE(J,K)*ZZ(J,K) - YZ(J,K)*ZE(J,K))
       A32 = (XZ(J,K)*ZE(J,K) - XE(J,K)*ZZ(J,K))
       A33 = (XE(J,K)*YZ(J,K) - XZ(J,K)*YE(J,K))
C
       AA(J,K,1,1) = C11*A11 + C13*A31
       AA(J,K,2,1) = C21*A11 + C23*A31
       AA(J,K,3,1) = C31*A11 + C33*A31
       AA(J,K,1,2) = AA(J,K,2,1)
       AA(J,K,2,2) = C21*A12 + C23*A32
       AA(J,K,3,2) = C31*A12 + C33*A32
       AA(J,K,1,3) = AA(J,K,3,1)
       AA(J,K,2,3) = AA(J,K,3,2)
       AA(J,K,3,3) = C31*A13 + C33*A33
C
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CINVB(JDIM,KDIM,JS,JE,KS,KE,
     >                 XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOL,BB)
c*wdh*
c* include "precis.h"
C
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION VOL(JDIM,KDIM),BB(JDIM,KDIM,3,3)
C
C    Compute C inverse times B on LHS of governing equations,
C    noting that the result is a symmetric matrix
C
      DO 10 K=KS,KE
      DO 10 J=JS,JE
C
       Z1 = XZ(J,K)/VOL(J,K)
       Z2 = YZ(J,K)/VOL(J,K)
       Z3 = ZZ(J,K)/VOL(J,K)
C       C11 = YE(J,K)*Z3 - ZE(J,K)*Z2
C       C21 = ZE(J,K)*Z1 - XE(J,K)*Z3
C       C31 = XE(J,K)*Z2 - YE(J,K)*Z1
       C12 = ZX(J,K)*Z2 - YX(J,K)*Z3
       C22 = XX(J,K)*Z3 - ZX(J,K)*Z1
       C32 = YX(J,K)*Z1 - XX(J,K)*Z2
       C13 = Z1
       C23 = Z2
       C33 = Z3
       B21 = XZ(J,K)
       B22 = YZ(J,K)
       B23 = ZZ(J,K)
       B31 = (YZ(J,K)*ZX(J,K) - YX(J,K)*ZZ(J,K))
       B32 = (XX(J,K)*ZZ(J,K) - XZ(J,K)*ZX(J,K))
       B33 = (XZ(J,K)*YX(J,K) - XX(J,K)*YZ(J,K))
C
       BB(J,K,1,1) = C12*B21 + C13*B31
       BB(J,K,2,1) = C22*B21 + C23*B31
       BB(J,K,3,1) = C32*B21 + C33*B31
       BB(J,K,1,2) = BB(J,K,2,1)
       BB(J,K,2,2) = C22*B22 + C23*B32
       BB(J,K,3,2) = C32*B22 + C33*B32
       BB(J,K,1,3) = BB(J,K,3,1)
       BB(J,K,2,3) = BB(J,K,3,2)
       BB(J,K,3,3) = C32*B23 + C33*B33
C
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CMPBNKO(JDIM,KDIM,J,K,KN,JJP,JJR,KPLN,IBCK,X,Y,Z,
     >                  XZ,YZ,ZZ,UNX,UNY,UNZ,PHI,NPA,IDIV)
c*wdh*
c* include "precis.h"
C
      PARAMETER (ONE=1.0)
      DIMENSION JJP(JDIM), JJR(JDIM), KPLN(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
C
C    Determine unit normal vector at K boundary
      TOL = 0.001
c      UNX = XZ(J,K)
c      UNY = YZ(J,K)
c      UNZ = ZZ(J,K)
c      IF (KPLN(1).EQ.0) UNX = 0.0
c      IF (KPLN(2).EQ.0) UNY = 0.0
c      IF (KPLN(3).EQ.0) UNZ = 0.0
c      DOTB = ( X(J,KN) - X(J,K) )*( 1 - KPLN(1) ) +
c     >       ( Y(J,KN) - Y(J,K) )*( 1 - KPLN(2) ) +
c     >       ( Z(J,KN) - Z(J,K) )*( 1 - KPLN(3) )
C      IF ( (UNX.EQ.0.0).AND.(UNY.EQ.0.0).AND.(UNZ.EQ.0.0) ) THEN
c      IF ( ABS(DOTB).LE.TOL ) THEN
c       JP = JJP(J)
c       JR = JJR(J)
c       XB = X(JP,KN) - X(JR,KN)
c       YB = Y(JP,KN) - Y(JR,KN)
c       ZB = Z(JP,KN) - Z(JR,KN)
c       UNX = ( YZ(J,K)*ZB - ZZ(J,K)*YB )*KPLN(1)
c       UNY = ( ZZ(J,K)*XB - XZ(J,K)*ZB )*KPLN(2)
c       UNZ = ( XZ(J,K)*YB - YZ(J,K)*XB )*KPLN(3)
c      ENDIF
c      UN = SQRT( UNX*UNX + UNY*UNY + UNZ*UNZ )
c      UNX = UNX/UN
c      UNY = UNY/UN
c      UNZ = UNZ/UN
C
C    Set up unit normal at boundary for IBCK = 4,5,6
      UNX = 0.0
      UNY = 0.0
      UNZ = 0.0
      IF (IBCK.EQ.4) THEN
       UNX = 1.0
      ELSE IF (IBCK.EQ.5) THEN
       UNY = 1.0
      ELSE IF (IBCK.EQ.6) THEN
       UNZ = 1.0
      ENDIF
C
      IF (IBCK.LE.3) THEN
C
C      Set up unit normal for constraining boundary plane
       BNX = 1. - KPLN(1)
       BNY = 1. - KPLN(2)
       BNZ = 1. - KPLN(3)
C
C      Compute unit normal vector at K boundary
       JP = JJP(J)
       JR = JJR(J)
       XP = X(JP,KN) - X(J,KN)
       YP = Y(JP,KN) - Y(J,KN)
       ZP = Z(JP,KN) - Z(J,KN)
       XM = X(JR,KN) - X(J,KN)
       YM = Y(JR,KN) - Y(J,KN)
       ZM = Z(JR,KN) - Z(J,KN)
       DP = 1.0/SQRT( XP*XP + YP*YP + ZP*ZP )
       DM = 1.0/SQRT( XM*XM + YM*YM + ZM*ZM )
       DELX = XP*DP - XM*DM
       DELY = YP*DP - YM*DM
       DELZ = ZP*DP - ZM*DM
C
       ICLP = 0
       IF ((DELX.EQ.0.).AND.(DELY.EQ.0.).AND.(IBCK.EQ.3)) ICLP = 1
       IF ((DELY.EQ.0.).AND.(DELZ.EQ.0.).AND.(IBCK.EQ.1)) ICLP = 1
       IF ((DELZ.EQ.0.).AND.(DELX.EQ.0.).AND.(IBCK.EQ.2)) ICLP = 1
       IF (ICLP.EQ.1) THEN
        DELX = X(JP,KN) - X(J,K)
        DELY = Y(JP,KN) - Y(J,K)
        DELZ = Z(JP,KN) - Z(J,K)
       ENDIF
C
       AYX = DELY*BNZ - DELZ*BNY
       AZX = DELZ*BNY - DELY*BNZ
       AXY = DELX*BNZ - DELZ*BNX
       AZY = DELZ*BNX - DELX*BNZ
       AXZ = DELX*BNY - DELY*BNX
       AYZ = DELY*BNX - DELX*BNY
C
       IF ( (AYX.NE.0.0) .AND. (AZX.NE.0.0) ) THEN
        QY = AXY/AYX
        QZ = AXZ/AZX
        UNX = 1.0/SQRT( 1.0 + QY*QY + QZ*QZ )
        UNY = -QY*UNX
        UNZ = -QZ*UNX
       ELSE IF ( (AXY.NE.0.0) .AND. (AZY.NE.0.0) ) THEN
        QX = AYX/AXY
        QZ = AYZ/AZY
        UNY = 1.0/SQRT( 1.0 + QX*QX + QZ*QZ )
        UNX = -QX*UNY
        UNZ = -QZ*UNY
       ELSE IF ( (AXZ.NE.0.0) .AND. (AYZ.NE.0.0) ) THEN
        QX = AZX/AXZ
        QY = AZY/AYZ
        UNZ = 1.0/SQRT( 1.0 + QX*QX + QY*QY )
        UNX = -QX*UNZ
        UNY = -QY*UNZ
       ENDIF
C
      ENDIF
C
C    Determine if sign of normal need to be switched
      DOT = XZ(J,K)*UNX + YZ(J,K)*UNY + ZZ(J,K)*UNZ
      IF (DOT.LT.-TOL) THEN
       UNX = -UNX
       UNY = -UNY
       UNZ = -UNZ
      ELSE IF (ABS(DOT).LE.TOL) THEN
       DIN  = (X(J,KN)-UNX-X(J,K))**2 + (Y(J,KN)-UNY-Y(J,K))**2 +
     >        (Z(J,KN)-UNZ-Z(J,K))**2
       DOUT = (X(J,KN)+UNX-X(J,K))**2 + (Y(J,KN)+UNY-Y(J,K))**2 +
     >        (Z(J,KN)+UNZ-Z(J,K))**2
       IF (DIN.GT.DOUT) THEN
        UNX = -UNX
        UNY = -UNY
        UNZ = -UNZ
       ENDIF
      ENDIF
C
C    Determine how many interior points to be affected
      UNI = SQRT(XZ(J,K)**2 + YZ(J,K)**2 + ZZ(J,K)**2)
      DOT = ( UNX*XZ(J,K) + UNY*YZ(J,K) + UNZ*ZZ(J,K) )/UNI
      DOT = MIN( ONE, ABS(DOT) )
      NPA = MIN( INT( ABS(1.0-DOT)*10.0 ), KDIM/2 )
      PHI = ACOS(DOT)
C
C    Determine if normals are diverging or converging
      DBASE = (X(J,K)-X(J,KN))**2 + (Y(J,K)-Y(J,KN))**2 + 
     >        (Z(J,K)-Z(J,KN))**2
      DNEW  = ( (X(J,K) + XZ(J,K)/UNI) - (X(J,KN) + UNX) )**2 +
     >        ( (Y(J,K) + YZ(J,K)/UNI) - (Y(J,KN) + UNY) )**2 +
     >        ( (Z(J,K) + ZZ(J,K)/UNI) - (Z(J,KN) + UNZ) )**2
      IF (DNEW.GE.DBASE) THEN
       IDIV = 1
      ELSE
       IDIV = 0
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CONVOL(JDIM,KDIM,M1D,LMAX,JA,JB,KA,KB,L,
     >                  JPER,KPER,JJP,JJR,KKP,KKR,JAXSA,JAXSB,VZETA,
     >                  IAXIS,EXAXIS,VOLRES,IVSPEC,EPSSS,ITSVOL,
     >                  RADIS,DTHET,SPHI,DLC,DAREAS,DAREA,VOL,RR)
c*wdh*
c* include "precis.h"
C
      LOGICAL JAXSA,JAXSB,VZETA
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM)
      DIMENSION SPHI(M1D),DLC(JDIM),DAREAS(JDIM)
      DIMENSION DAREA(JDIM,KDIM),VOL(JDIM,KDIM),RR(JDIM,KDIM,LMAX)
C
C   -------------------------
C    Compute control volumes
C   -------------------------
      IF (IVSPEC.EQ.1) THEN
C
C      Compute volumes as (arc length) times DAREA(J,K)
        DO 20 K = KA,KB
        DO 20 J = JA,JB
         VOL(J,K) = DAREA(J,K)*( RR(J,K,L) - RR(J,K,L-1) )
 20     CONTINUE
C
      ELSE IF (IVSPEC.EQ.2) THEN
C
C      Compute volumes by scaling with sphere
        SC = (1.0-EPSSS)**(2*(L-1))
        DO 30 K=KA,KB
        DO 30 J=JA,JB
         IF (JAXSA.AND.JAXSB) THEN
          VOLU = ((RR(J,K,L)+RADIS)**2)*SPHI(J)*(RR(J,K,L)-RR(J,K,L-1))
         ELSE
          VOLU = DLC(J)*(RR(J,K,L)+RADIS)*DTHET*(RR(J,K,L)-RR(J,K,L-1))
         ENDIF
         VOLNU = (DAREA(J,K)/DAREAS(J))*VOLU
         VOL(J,K) = VOLU*(1.0-SC) + VOLNU*SC
 30     CONTINUE
C
      ENDIF
C
C    Average volumes at axis if variable zetavr is used
      IF ( VZETA .AND. JAXSA ) THEN
        KTOT = KB-KA+1
        DO 46 J=JA,JB
         RJMJA = FLOAT(J-JA)
         SC = EXP(-0.02*RJMJA*RJMJA)
         VOLAV = 0.0
         DO 47 K=KA,KB
          VOLAV = VOLAV + VOL(J,K)
 47      CONTINUE
         VOLAV = VOLAV/FLOAT(KTOT)
         DO 48 K=KA,KB
          VOL(J,K) = (1.0-SC)*VOL(J,K) + SC*VOLAV
 48      CONTINUE
 46     CONTINUE
      ENDIF
      IF ( VZETA .AND. JAXSB ) THEN
        KTOT = KB-KA+1
        DO 76 J=JA,JB
         RJMJB = FLOAT(JB-J)
         SC = EXP(-0.02*RJMJB*RJMJB)
         VOLAV = 0.0
         DO 77 K=KA,KB
          VOLAV = VOLAV + VOL(J,K)
 77      CONTINUE
         VOLAV = VOLAV/FLOAT(KTOT)
         DO 78 K=KA,KB
          VOL(J,K) = (1.0-SC)*VOL(J,K) + SC*VOLAV
 78      CONTINUE
 76     CONTINUE
      ENDIF
C
c    Restrict volumes near axis to less than VOLRES of previous cell
      IF ((IAXIS.GE.1).AND.(EXAXIS.NE.0.0)) THEN
       IF (JAXSA) THEN
        J = JA
        DO 88 K=KA,KB
         VOLTOL = VOLRES*VOL(J+1,K)
         IF (VOL(J,K).GT.VOLTOL) VOL(J,K) = VOLTOL
 88     CONTINUE
       ENDIF
       IF (JAXSB) THEN
        J = JB
        DO 89 K=KA,KB
         VOLTOL = VOLRES*VOL(J-1,K)
         IF (VOL(J,K).GT.VOLTOL) VOL(J,K) = VOLTOL
 89    CONTINUE
       ENDIF
      ENDIF
C
C   ---------------------------------------
C    Smooth volumes ITSVOL number of times
C   ---------------------------------------
C     Vary ITSVOL with L
      LITS = 5
      IF (L.LT.LITS) THEN
       ITSVOLL = INT(ITSVOL*(SQRT(FLOAT(L)/FLOAT(LITS))))
      ELSE
       ITSVOLL = ITSVOL
      ENDIF
C      IF (ITSVOL .GE. 1) THEN
      IF (ITSVOLL .GE. 1) THEN
       VSMU =.16
       VSH = VSMU/4.
C       DO 39 NITS = 1,ITSVOL
       DO 39 NITS = 1,ITSVOLL
C
        IF (JPER .EQ. 0) THEN
         FA = 1.
         IF(JAXSA) FA = 0.
         FB =1.
         IF(JAXSB) FB = 0.
         DO 33 K = KA-1+KPER,KB+1-KPER
          J = JA-1
          VOL(J,K) = (1.33*VOL(J+1,K)-.33*VOL(J+2,K))*FA
          J = JB+1
          VOL(J,K) = (1.33*VOL(J-1,K)-.33*VOL(J-2,K))*FB
 33      CONTINUE
        ENDIF
        IF (KPER .EQ. 0) THEN
         DO 34 J = JA-1+JPER,JB+1-JPER
          K = KA-1
          VOL(J,K) = 1.33*VOL(J,K+1)-.33*VOL(J,K+2)
          K = KB+1
          VOL(J,K) = 1.33*VOL(J,K-1)-.33*VOL(J,K-2)
 34      CONTINUE
        ENDIF
         DO 36 K = KA,KB
          KP = KKP(K)
          KR = KKR(K)
         DO 36 J = JA,JB
          JP = JJP(J)
          JR = JJR(J)
          DAREA(J,K) =  (1.-VSMU)*VOL(J,K) 
     >               +  (VOL(JP,K) + VOL(JR,K))*VSH
     >               +  (VOL(J,KP) + VOL(J,KR))*VSH
 36      CONTINUE
         DO 38 K = KA,KB
         DO 38 J = JA,JB
          VOL(J,K) = DAREA(J,K)
 38      CONTINUE
C
 39    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CORNUP(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >                  JSYMA,JSYMB,JAXSA,JAXSB,KSYMA,KSYMB,
     >                  JPLNA,JPLNB,KPLNA,KPLNB,X,Y,Z,XW,YW,ZW)
c*wdh*
c* include "precis.h"
C
      LOGICAL JSYMA,JSYMB,JAXSA,JAXSB,KSYMA,KSYMB
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C    Update corner points of grid
       IF (.NOT.JAXSA) THEN
c       X(JA-1,KA-1) = X(JA-1,KA-1) + JPLNA(1)*KPLNA(1)*
c     &     (-X(JA-1,KA-1) + X(JA-1,KA) + X(JA,KA-1) -X(JA,KA))
c       Y(JA-1,KA-1) = Y(JA-1,KA-1) + JPLNA(2)*KPLNA(2)*
c     &     (-Y(JA-1,KA-1) + Y(JA-1,KA) + Y(JA,KA-1) -Y(JA,KA))
c       Z(JA-1,KA-1) = Z(JA-1,KA-1) + JPLNA(3)*KPLNA(3)*
c     &     (-Z(JA-1,KA-1) + Z(JA-1,KA) + Z(JA,KA-1) -Z(JA,KA))
c       X(JA-1,KB+1) = X(JA-1,KB+1) + JPLNA(1)*KPLNB(1)*
c     &     (-X(JA-1,KB+1) + X(JA-1,KB) + X(JA,KB+1) -X(JA,KB))
c       Y(JA-1,KB+1) = Y(JA-1,KB+1) + JPLNA(2)*KPLNB(2)*
c     &     (-Y(JA-1,KB+1) + Y(JA-1,KB) + Y(JA,KB+1) -Y(JA,KB))
c       Z(JA-1,KB+1) = Z(JA-1,KB+1) + JPLNA(3)*KPLNB(3)*
c     &     (-Z(JA-1,KB+1) + Z(JA-1,KB) + Z(JA,KB+1) -Z(JA,KB))
        X(JA-1,KA-1) = XW(JA-1,KA-1,L-1) + 0.5*JPLNA(1)*KPLNA(1)*
     >       ( X(JA-1,KA)-XW(JA-1,KA,L-1) + X(JA,KA-1)-XW(JA,KA-1,L-1) )
        Y(JA-1,KA-1) = YW(JA-1,KA-1,L-1) + 0.5*JPLNA(2)*KPLNA(2)*
     >       ( Y(JA-1,KA)-YW(JA-1,KA,L-1) + Y(JA,KA-1)-YW(JA,KA-1,L-1) )
        Z(JA-1,KA-1) = ZW(JA-1,KA-1,L-1) + 0.5*JPLNA(3)*KPLNA(3)*
     >       ( Z(JA-1,KA)-ZW(JA-1,KA,L-1) + Z(JA,KA-1)-ZW(JA,KA-1,L-1) )
        X(JA-1,KB+1) = XW(JA-1,KB+1,L-1) + 0.5*JPLNA(1)*KPLNB(1)*
     >       ( X(JA-1,KB)-XW(JA-1,KB,L-1) + X(JA,KB+1)-XW(JA,KB+1,L-1) )
        Y(JA-1,KB+1) = YW(JA-1,KB+1,L-1) + 0.5*JPLNA(2)*KPLNB(2)*
     >       ( Y(JA-1,KB)-YW(JA-1,KB,L-1) + Y(JA,KB+1)-YW(JA,KB+1,L-1) )
        Z(JA-1,KB+1) = ZW(JA-1,KB+1,L-1) + 0.5*JPLNA(3)*KPLNB(3)*
     >       ( Z(JA-1,KB)-ZW(JA-1,KB,L-1) + Z(JA,KB+1)-ZW(JA,KB+1,L-1) )
       ENDIF
       IF (.NOT.JAXSB) THEN
c       X(JB+1,KA-1) = X(JB+1,KA-1) + JPLNB(1)*KPLNA(1)*
c     &     (-X(JB+1,KA-1) + X(JB+1,KA) + X(JB,KA-1) -X(JB,KA))
c       Y(JB+1,KA-1) = Y(JB+1,KA-1) + JPLNB(2)*KPLNA(2)*
c     &     (-Y(JB+1,KA-1) + Y(JB+1,KA) + Y(JB,KA-1) -Y(JB,KA))
c       Z(JB+1,KA-1) = Z(JB+1,KA-1) + JPLNB(3)*KPLNA(3)*
c     &     (-Z(JB+1,KA-1) + Z(JB+1,KA) + Z(JB,KA-1) -Z(JB,KA))
c       X(JB+1,KB+1) = X(JB+1,KB+1) + JPLNB(1)*KPLNB(1)*
c     &     (-X(JB+1,KB+1) + X(JB+1,KB) + X(JB,KB+1) -X(JB,KB))
c       Y(JB+1,KB+1) = Y(JB+1,KB+1) + JPLNB(2)*KPLNB(2)*
c     &     (-Y(JB+1,KB+1) + Y(JB+1,KB) + Y(JB,KB+1) -Y(JB,KB))
c       Z(JB+1,KB+1) = Z(JB+1,KB+1) + JPLNB(3)*KPLNB(3)*
c     &     (-Z(JB+1,KB+1) + Z(JB+1,KB) + Z(JB,KB+1) -Z(JB,KB))
        X(JB+1,KA-1) = XW(JB+1,KA-1,L-1) + 0.5*JPLNB(1)*KPLNA(1)*
     >       ( X(JB+1,KA)-XW(JB+1,KA,L-1) + X(JB,KA-1)-XW(JB,KA-1,L-1) )
        Y(JB+1,KA-1) = YW(JB+1,KA-1,L-1) + 0.5*JPLNB(2)*KPLNA(2)*
     >       ( Y(JB+1,KA)-YW(JB+1,KA,L-1) + Y(JB,KA-1)-YW(JB,KA-1,L-1) )
        Z(JB+1,KA-1) = ZW(JB+1,KA-1,L-1) + 0.5*JPLNB(3)*KPLNA(3)*
     >       ( Z(JB+1,KA)-ZW(JB+1,KA,L-1) + Z(JB,KA-1)-ZW(JB,KA-1,L-1) )
        X(JB+1,KB+1) = XW(JB+1,KB+1,L-1) + 0.5*JPLNB(1)*KPLNB(1)*
     >       ( X(JB+1,KB)-XW(JB+1,KB,L-1) + X(JB,KB+1)-XW(JB,KB+1,L-1) )
        Y(JB+1,KB+1) = YW(JB+1,KB+1,L-1) + 0.5*JPLNB(2)*KPLNB(2)*
     >       ( Y(JB+1,KB)-YW(JB+1,KB,L-1) + Y(JB,KB+1)-YW(JB,KB+1,L-1) )
        Z(JB+1,KB+1) = ZW(JB+1,KB+1,L-1) + 0.5*JPLNB(3)*KPLNB(3)*
     >       ( Z(JB+1,KB)-ZW(JB+1,KB,L-1) + Z(JB,KB+1)-ZW(JB,KB+1,L-1) )
       ENDIF
C
C      Overwrite corner points with symmetry condition
        IF (KSYMA) THEN
         J = JA-1
         X(J,KA-1) = X(J,KA+1)*(-1+2*KPLNA(1))
         Y(J,KA-1) = Y(J,KA+1)*(-1+2*KPLNA(2))
         Z(J,KA-1) = Z(J,KA+1)*(-1+2*KPLNA(3))
         J = JB+1
         X(J,KA-1) = X(J,KA+1)*(-1+2*KPLNA(1))
         Y(J,KA-1) = Y(J,KA+1)*(-1+2*KPLNA(2))
         Z(J,KA-1) = Z(J,KA+1)*(-1+2*KPLNA(3))
        ENDIF
        IF (KSYMB) THEN
         J = JA-1
         X(J,KB+1) = X(J,KB-1)*(-1+2*KPLNB(1))
         Y(J,KB+1) = Y(J,KB-1)*(-1+2*KPLNB(2))
         Z(J,KB+1) = Z(J,KB-1)*(-1+2*KPLNB(3))
         J = JB+1
         X(J,KB+1) = X(J,KB-1)*(-1+2*KPLNB(1))
         Y(J,KB+1) = Y(J,KB-1)*(-1+2*KPLNB(2))
         Z(J,KB+1) = Z(J,KB-1)*(-1+2*KPLNB(3))
        ENDIF
        IF (JSYMA) THEN
         K = KA-1
         X(JA-1,K) = X(JA+1,K)*(-1+2*JPLNA(1))
         Y(JA-1,K) = Y(JA+1,K)*(-1+2*JPLNA(2))
         Z(JA-1,K) = Z(JA+1,K)*(-1+2*JPLNA(3))
         K = KB+1
         X(JA-1,K) = X(JA+1,K)*(-1+2*JPLNA(1))
         Y(JA-1,K) = Y(JA+1,K)*(-1+2*JPLNA(2))
         Z(JA-1,K) = Z(JA+1,K)*(-1+2*JPLNA(3))
        ENDIF
        IF (JSYMB) THEN
         K = KA-1
         X(JB+1,K) = X(JB-1,K)*(-1+2*JPLNB(1))
         Y(JB+1,K) = Y(JB-1,K)*(-1+2*JPLNB(2))
         Z(JB+1,K) = Z(JB-1,K)*(-1+2*JPLNB(3))
         K = KB+1
         X(JB+1,K) = X(JB-1,K)*(-1+2*JPLNB(1))
         Y(JB+1,K) = Y(JB-1,K)*(-1+2*JPLNB(2))
         Z(JB+1,K) = Z(JB-1,K)*(-1+2*JPLNB(3))
        ENDIF
C
      RETURN
      END
C***********************************************************************
      FUNCTION COSAN(X0,Y0,Z0,X1,Y1,Z1,X2,Y2,Z2)
c*wdh*
c* include "precis.h"
C
C    Function to compute cosine of angle between vectors (X1-X0)
C    and (X2-X0)
      VX1 = X1-X0
      VY1 = Y1-Y0
      VZ1 = Z1-Z0
      VX2 = X2-X0
      VY2 = Y2-Y0
      VZ2 = Z2-Z0
      D1 = SQRT(VX1*VX1 + VY1*VY1 + VZ1*VZ1)
      D2 = SQRT(VX2*VX2 + VY2*VY2 + VZ2*VZ2)
      IF ( (D1.NE.0.0).AND.(D2.NE.0.0) ) THEN
       COSAN = (VX1*VX2 + VY1*VY2 + VZ1*VZ2)/(D1*D2)
      ELSE
       COSAN = -1.0
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE DISVAR(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >                  X,Y,Z,IMETH,SMU2,LTEST,LTRANS,DISSL,INI,
     >                  RJMAXM,RKMAXM,ADXI,ADET,ADRXI,ADRET,RATJ,RATK)
c*wdh*
c* include "precis.h"
C
      PARAMETER (P1=0.1)
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM),DISSL(LMAX)
      DIMENSION ADXI(JDIM,KDIM),ADET(JDIM,KDIM),ADRXI(JDIM,KDIM),
     >          ADRET(JDIM,KDIM),RATJ(JDIM,KDIM),RATK(JDIM,KDIM)
C
C   -----------------------------------------------------------
C    Compute arrays for use with spatially-varying dissipation
C   -----------------------------------------------------------
      IF (IMETH.EQ.0) THEN
C
       DO 5 K=KA,KB
       DO 5 J=JA,JB
        ADRXI(J,K) = 1.0
        ADRET(J,K) = 1.0
 5     CONTINUE
C
      ELSE IF (IMETH.GE.1) THEN
C
C      Compute average distance between grid pts, form ratio with L-1 level
       DO 10 K=KA,KB
        KP = KKP(K)
        KR = KKR(K)
       DO 10 J=JA,JB
        JP = JJP(J)
        JR = JJR(J)
        DISXP = SQRT( (X(JP,K)-X(J,K))**2 + (Y(JP,K)-Y(J,K))**2 +
     >                (Z(JP,K)-Z(J,K))**2 )
        DISXM = SQRT( (X(JR,K)-X(J,K))**2 + (Y(JR,K)-Y(J,K))**2 +
     >                (Z(JR,K)-Z(J,K))**2 )
        DISEP = SQRT( (X(J,KP)-X(J,K))**2 + (Y(J,KP)-Y(J,K))**2 +
     >                (Z(J,KP)-Z(J,K))**2 )
        DISEM = SQRT( (X(J,KR)-X(J,K))**2 + (Y(J,KR)-Y(J,K))**2 +
     >                (Z(J,KR)-Z(J,K))**2 )
        ADXIL = DISXP+DISXM
        ADETL = DISEP+DISEM
        RATJ(J,K) = ADXI(J,K)/ADXIL
        RATK(J,K) = ADET(J,K)/ADETL
        ADXI(J,K) = ADXIL
        ADET(J,K) = ADETL
 10    CONTINUE
c
C     Reset RATJ,RATK for L=1 to avoid computing large numbers in loop 50.
       IF (INI.EQ.1) THEN
        DO 15 K=KA,KB
        DO 15 J=JA,JB
         RATJ(J,K) = 1.0
         RATK(J,K) = 1.0
 15     CONTINUE
       ENDIF
c
c     Search for max ratj and ratk
       IF ((L.GE.LTEST-1).AND.(LTRANS.EQ.LMAX)) THEN
        RJMAX = 0.0
        RKMAX = 0.0
        DO 20 K=KA,KB
        DO 20 J=JA,JB
         IF (RATJ(J,K).GT.RJMAX) RJMAX = RATJ(J,K)
         IF (RATK(J,K).GT.RKMAX) RKMAX = RATK(J,K)
 20     CONTINUE
       ENDIF
       IF (L.EQ.LTEST-1) THEN
        RJMAXM = RJMAX
        RKMAXM = RKMAX
       ENDIF
c
C wmc Allow DISSL to increase up to 1
c     Test for switch in dissl and reset to constant level
c       IF ((L.GE.LTEST).AND.(LTRANS.EQ.LMAX)) THEN
c        DRATJ = RJMAX-RJMAXM
c        DRATK = RKMAX-RKMAXM
c        IF ((DRATJ.LT.0.0).OR.(DRATK.LT.0.0)) THEN
c         LTRANS = L
c         SLFIX = DISSL(L)
c         DO 25 LL=LTRANS,LMAX
c          DISSL(LL) = SLFIX
c 25      CONTINUE
c         WRITE(*,*)' LTRANS = ',LTRANS
c        ENDIF
c        RJMAXM = RJMAX
c        RKMAXM = RKMAX
c       ENDIF
c
c     Compute scaled distance ratio functions
       IF (DISSL(L).EQ.0.0) THEN
        ADRIND = 2.0
       ELSE
        ADRIND = 2.0/DISSL(L)
       ENDIF
       DO 50 K=KA,KB
       DO 50 J=JA,JB
        ADRXI(J,K) = MAX( P1, RATJ(J,K)**ADRIND )
        ADRET(J,K) = MAX( P1, RATK(J,K)**ADRIND )
 50    CONTINUE
c
      ENDIF
      RETURN
      END
C***********************************************************************
      FUNCTION HEPSIL(FMX,DFM,NPT)
c*wdh*
c* include "precis.h"
C         
C     This function is modified from the EPSIL function by Steger.
C     New checks are now performed for near uniform spacing up front.
C     If eps gets small, divide by a small number is avoided by
C     series expansions. FMIN is assumed to be zero and NCALL is assumed
C     to be 1 (Jan. 95, WMC).
C
C        THIS SUBROUTINE APPLIES A NEWTON-RAPHSON ROOT-FINDING        
C        TECHNIQUE TO FIND A VALUE OF EPSILON FOR A PARTICULAR USE    
C        OF THE EXPONENTIAL STRECHING TRANSFORMATION.       
C         
C     FMX    = TOTAL ARC LENGTH ALONG COORDINATE      
C     DFM    = SPECIFIED INITIAL INCREMENT OF ARC LENGTH
C     NPT    = NUMBER OF POINTS ALONG COORDINATE
C
C     FPCC   = ITERATIVE ERROR BOUND, E.G.O( 0.00002)
C     ICC    = MAXIMUM NUMBER OF ITERATIONS
C     FMIN   = INITIAL VALUE OF ARC LENGTH (assumed zero)
C     NCALL  = 1  INITIAL GUESS FOR EPS IS USED (assumed)
C            > 1  PREVIOUS EPS USED AS INITIAL GUESS
C         
C     TOL    = tolerance for uniform spacing test
C     EPSMAL = series expansion used if eps smaller than EPSMAL
C
      PARAMETER ( FPCC=0.00002, ICC=20 )
      PARAMETER ( TOL=1.0E-6, EPSMAL=5.0E-5 )
C
C    Basic checks
      IF ((FMX.LE.0.0).OR.(DFM.LE.0.0)) THEN
       WRITE(*,*)'Initial spacing or total distance not positive.'
       STOP
      ENDIF
      IF (DFM.GT.FMX) THEN
       WRITE(*,*)'Initial spacing greater than total distance.'
       STOP
      ENDIF
      IF (NPT.LE.2) THEN
       WRITE(*,*)'Too few points. Reset to 3.'
       NPT = 3
      ENDIF
C
      FNPTM1 = FLOAT(NPT-1)
      FNPTM2 = FLOAT(NPT-2)
C
C    Test for uniform spacing before doing iterations
      REM = ABS( ((FMX - FNPTM1*DFM))/FMX )
      IF (REM.LT.TOL) THEN
       EPS = 0.0
       WRITE(*,*) 'UNIFORM SPACING DETERMINED IN EPSIL.'
       GO TO 10
      ENDIF
C
C    Set initial guess
      EPS = (FMX/DFM)**(1.0/FNPTM2) - 1.0
C         
C    Do Newton iterations
      DO 5 NIT=1,ICC
       EP1   = EPS + 1.0
       EP1TN = EP1**FNPTM2   
C       IF (EPS.EQ.0.0) GO TO 10
       IF (ABS(EPS) .GT. EPSMAL) THEN
        F    = FMX - (DFM/EPS)*(EP1TN*EP1 - 1.0)
        RU   = EPS*EPS*F/DFM
        RD   = 1.0 + EP1TN*( EPS*FNPTM2 - 1.0 )
        DEPS = RU/RD
       ELSE
        F    = FMX - DFM*FNPTM1*(1.0 + 0.5*FNPTM2*EPS)
        C0   = 0.5*FNPTM1*FNPTM2
        R    = (FMX/DFM) - FNPTM1
        C1   = C0 + 2.0*R*FLOAT(NPT-3)/3.0
        DEPS = (R - C1*EPS)/C0
       ENDIF
       IF ( (ABS(F) .LT. FPCC) .OR. (ABS(DEPS) .LT. TOL) ) GO TO 10
       EPS  = EPS + DEPS
 5    CONTINUE
C         
      WRITE(*,*)'EXCEEDED MAX. NO. OF ITERATIONS IN EPSIL.'
      NIT = ICC
C
 10   CONTINUE
      HEPSIL = EPS
C      WRITE(*,101) EPSIL,F,NIT
 101  FORMAT(1X,'EPSIL =',F11.5,' AND F =',F11.5,' AFTER ',I3,     
     >       ' ITERATIONS.')
C
      RETURN
      END
C***********************************************************************
      SUBROUTIN EFILTRE(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,TIMK,
     >                  XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOL,IMETH,
     >            DISSL,ADRET,CAXI,CAET,AFNET,ETDS,CVEX,BB,SR,A,B,C,F)
C
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION VOL(JDIM,KDIM),DISSL(LMAX),ADRET(JDIM,KDIM),
     >          CAXI(JDIM,KDIM),CAET(JDIM,KDIM),AFNET(JDIM,KDIM),
     >          ETDS(JDIM,KDIM),CVEX(JDIM,KDIM),
     >          BB(JDIM,KDIM,3,3),SR(JDIM,KDIM,3)
      DIMENSION A(JDIM,KDIM,3,3),B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),
     >          F(JDIM,KDIM,3)
C
C   ------------------------------------------
C    Fill LHS block tridiagonal matrix in eta
C   ------------------------------------------
      T5 = 0.5*(1.0 + TIMK)
C
      CALL CINVB(JDIM,KDIM,JA,JB,KA,KB,
     >           XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOL,BB)
C
      DO 10 N = 1,3
      DO 10 M = 1,3
      DO 10 K = KA,KB
      DO 10 J = JA,JB
       TBB = T5*BB(J,K,N,M)
       A(J,K,N,M) = -TBB
       B(J,K,N,M) = 0.0
       C(J,K,N,M) =  TBB
 10   CONTINUE
C
C    Solve averaging equations or impose solution at convex corners
      IF ((IMETH.EQ.2).OR.(IMETH.EQ.3)) THEN
       CAVEX = -0.5
       DO 15 K = KA,KB
       DO 15 J = JA,JB
C        IF ((CAXI(J,K).LT.CAVEX).OR.(CAET(J,K).LT.CAVEX)) THEN
        IF (CVEX(J,K).EQ.1.0) THEN
         DO 16 N = 1,3
         DO 16 M = 1,3
          A(J,K,M,N) = 0.0
          C(J,K,M,N) = 0.0
 16      CONTINUE
         IF (IMETH.EQ.2) THEN
          DO 17 N = 1,3
           A(J,K,N,N) = -0.25
           C(J,K,N,N) = -0.25
 17       CONTINUE
         ENDIF
        ENDIF
 15    CONTINUE
      ENDIF
C
      DO 20 K = KA,KB
      DO 20 J = JA,JB
       SMUIM = 2.0*DISSL(L)*ADRET(J,K)*AFNET(J,K)
       SMUIME = SMUIM*ETDS(J,K)
       SMUIMD = 1.0 + 2.0*SMUIME
       A(J,K,1,1) = A(J,K,1,1) - SMUIME
       A(J,K,2,2) = A(J,K,2,2) - SMUIME
       A(J,K,3,3) = A(J,K,3,3) - SMUIME
       B(J,K,1,1) = SMUIMD
       B(J,K,2,2) = SMUIMD
       B(J,K,3,3) = SMUIMD
       C(J,K,1,1) = C(J,K,1,1) - SMUIME
       C(J,K,2,2) = C(J,K,2,2) - SMUIME
       C(J,K,3,3) = C(J,K,3,3) - SMUIME
       F(J,K,1)   = SR(J,K,1)
       F(J,K,2)   = SR(J,K,2)
       F(J,K,3)   = SR(J,K,3)
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE HFILTRX(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,TIMJ,
     >                  XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOL,IMETH,
     >            DISSL,ADRXI,CAXI,CAET,AFNXI,XIDS,CVEX,AA,SR,A,B,C,F)
c*wdh*
c* include "precis.h"
C
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION VOL(JDIM,KDIM),DISSL(LMAX),ADRXI(JDIM,KDIM),
     >          CAXI(JDIM,KDIM),CAET(JDIM,KDIM),AFNXI(JDIM,KDIM),
     >          XIDS(JDIM,KDIM),CVEX(JDIM,KDIM),
     >          AA(JDIM,KDIM,3,3),SR(JDIM,KDIM,3)
      DIMENSION A(JDIM,KDIM,3,3),B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),
     >          F(JDIM,KDIM,3)
C
C   -----------------------------------------
C    Fill LHS block tridiagonal matrix in xi
C   -----------------------------------------
      T5 = 0.5*(1.0 + TIMJ)
C
      CALL CINVA(JDIM,KDIM,JA,JB,KA,KB,
     >           XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOL,AA)
C
      DO 10 N = 1,3
      DO 10 M = 1,3
      DO 10 K = KA,KB
      DO 10 J = JA,JB
       TAA = T5*AA(J,K,N,M)
       A(J,K,N,M) = -TAA
       B(J,K,N,M) = 0.0
       C(J,K,N,M) =  TAA
 10   CONTINUE
C
C    Solve averaging equations or impose solution at convex corners
      IF ((IMETH.EQ.2).OR.(IMETH.EQ.3)) THEN
       CAVEX = -0.5
       DO 15 K = KA,KB
       DO 15 J = JA,JB
C        IF ((CAXI(J,K).LT.CAVEX).OR.(CAET(J,K).LT.CAVEX)) THEN
        IF (CVEX(J,K).EQ.1.0) THEN
         DO 16 N = 1,3
         DO 16 M = 1,3
          A(J,K,M,N) = 0.0
          C(J,K,M,N) = 0.0
 16      CONTINUE
         IF (IMETH.EQ.2) THEN
          DO 17 N = 1,3
           A(J,K,N,N) = -0.25
           C(J,K,N,N) = -0.25
 17       CONTINUE
         ENDIF
        ENDIF
 15    CONTINUE
      ENDIF
C
      DO 20 K = KA,KB
      DO 20 J = JA,JB
       SMUIM = 2.0*DISSL(L)*ADRXI(J,K)*AFNXI(J,K)
       SMUIMX = SMUIM*XIDS(J,K)
       SMUIMD = 1.0 + 2.0*SMUIMX
       A(J,K,1,1) = A(J,K,1,1) - SMUIMX
       A(J,K,2,2) = A(J,K,2,2) - SMUIMX
       A(J,K,3,3) = A(J,K,3,3) - SMUIMX
       B(J,K,1,1) = SMUIMD
       B(J,K,2,2) = SMUIMD
       B(J,K,3,3) = SMUIMD
       C(J,K,1,1) = C(J,K,1,1) - SMUIMX
       C(J,K,2,2) = C(J,K,2,2) - SMUIMX
       C(J,K,3,3) = C(J,K,3,3) - SMUIMX
       F(J,K,1)   = SR(J,K,1)
       F(J,K,2)   = SR(J,K,2)
       F(J,K,3)   = SR(J,K,3)
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INIGRD2(JDIM,KDIM,JMAX,KMAX,LMAX,JZS,KZS,VZETA,IZSTRT,
     >                  IFMTSU,IFMTVA,IFMTST,X,Y,Z,ZETAVR,RR)
c*wdh*
c* include "precis.h"
C
      LOGICAL VZETA
      DIMENSION X(JDIM,KDIM), Y(JDIM,KDIM), Z(JDIM,KDIM),
     >          ZETAVR(JDIM,KDIM,3), RR(JDIM,KDIM,LMAX)
C
C   --------------------------------
C    Read in surface grid from disk
C   --------------------------------
C
      IF (IFMTSU.EQ.0) THEN
       READ(2) ((X(J,K),J=1,JMAX),K=1,KMAX),
     >         ((Y(J,K),J=1,JMAX),K=1,KMAX),
     >         ((Z(J,K),J=1,JMAX),K=1,KMAX)
      ELSE IF (IFMTSU.EQ.1) THEN
       READ(2,*) ((X(J,K),J=1,JMAX),K=1,KMAX),
     >           ((Y(J,K),J=1,JMAX),K=1,KMAX),
     >           ((Z(J,K),J=1,JMAX),K=1,KMAX)
      ENDIF
      CLOSE(2)
C
C   ------------------------------------------------------------------
C    Read in variable far field, initial/end spacing file
C   ------------------------------------------------------------------
      IF ( (VZETA) .AND. (IZSTRT.NE.-1) ) THEN
       IF (IFMTVA.EQ.0) THEN
        READ(7) (((ZETAVR(J,K,N),J=1,JMAX),K=1,KMAX),N=1,3)
       ELSE IF (IFMTVA.EQ.1) THEN
        READ(7,*) (((ZETAVR(J,K,N),J=1,JMAX),K=1,KMAX),N=1,3)
       ENDIF
       CLOSE(7)
      ENDIF
C
C   --------------------------------------------
C    Read in user-specified stretching function
C   --------------------------------------------
      IF (IZSTRT.EQ.-1) THEN
       IF (IFMTST.EQ.0) THEN
        READ(8) (((RR(J,K,L),J=1,JZS),K=1,KZS),L=1,LMAX)
       ELSE IF (IFMTST.EQ.1) THEN
        READ(8,*) (((RR(J,K,L),J=1,JZS),K=1,KZS),L=1,LMAX)
       ENDIF
       CLOSE(8)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INIPAR2(IFORM,IZSTRT,NZREG,NPZREG,ZREG,DZ0,DZ1,J2D,K2D,
     >                  JPER,KPER,JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,
     >                  KSYMA,KSYMB,KFLTA,KFLTB,JPLNA,JPLNB,KPLNA,KPLNB,
     >                  EXTJA,EXTJB,EXTKA,EXTKB,JPLN1,KPLN1,PLNKAB,
     >                  IBCJA,IBCJB,IBCKA,IBCKB,IVSPEC,EPSSS,ITSVOL,
     >                  IMETH,SMU2,TIMJ,TIMK,IAXIS,EXAXIS,VOLRES,
     >                  M3D,M2D,M1D,IFMTSU,IFMTVA,IFMTST,VZETA,
     >                  JDIM,KDIM,LDIM,JMAX,KMAX,LMAX,JZS,KZS)
c*wdh*
c* include "precis.h"
C
      LOGICAL JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,J2D,
     >        KSYMA,KSYMB,KFLTA,KFLTB,K2D
      LOGICAL VZETA,FILEX
C
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3),
     >          JPLN1(3),KPLN1(3),PLNKAB(3)
      DIMENSION NPZREG(M1D),ZREG(M1D),DZ0(M1D),DZ1(M1D)
C
C-----------------------------------------------------------------------
C
C   Input Notes:
C
C-----------------------------------------------------------------------
C   IFORM = 0  unformatted output of PLOT3D volume grid file
C         = 1  formatted . . . . . . . . . . . . . . . . . .
C
C   IZSTRT = 1  exponential stretching in L
C          = 2  hyperbolic tangent stretching in L
C          = -1 stretching funtion specified in file zetastr.i (@)
C   NZREG  = number of L-regions
C   (@) Only 1 L-region can be used with this option and ZREG,DZO,DZ1
C       will be disregarded.
C
C   NPZREG = number of points (including ends) in each L-region
C   ZREG   > 0  distance to march out for this L-region
C          <=0  variable far field distance specified in file zetavar.i (#)
C   DZ0    = 0  initial spacing is not fixed for this L-region
C          > 0  initial spacing for this L-region
C          < 0  variable initial spacing specified in file zetavar.i (#)
C   DZ1    = 0  end spacing is not fixed for this L-region
C          > 0  end spacing for this L-region
C          < 0  variable end spacing specified in file zetavar.i (#)
C   (#) Applied to first L-region only
C
C   The boundary conditions types at J=1, J=JMAX, K=1, K=KMAX are indicated 
C   by IBCJA, IBCJB, IBCKA, IBCKB, respectively
C
C   IBCJA  = -1  float X, Y and Z - zero order extrapolation (free floating)
C          < -1  outward-splaying free floating boundary condition which bends
C                the edge away from the interior. Use small $|$IBCJA$|$ for
C                small bending - mixed zeroth and first order extrapolation
C                with EXTJA = -IBCJA/1000.0 where EXTJA must satisfy 0 <EXTJA< 1
C          =  1  fix X, float Y and Z (constant X plane)
C          =  2  fix Y, float X and Z (constant Y plane)
C          =  3  fix Z, float X and Y (constant Z plane)
C          =  4  float X, fix Y and Z
C          =  5  float Y, fix X and Z
C          =  6  float Z, fix X and Y
C          =  7  floating collapsed edge with matching upper and lower sides
C                (points along K=1,(KMAX+1)/2 are matched with those on K=KMAX,(KMAX+1)/2)
C          = 10  periodic condition (*)
C          = 11  reflected symmetry condition with X=constant plane
C          = 12  reflected symmetry condition with Y=constant plane
C          = 13  reflected symmetry condition with Z=constant plane
C          = 20  singular axis point
C          = 21  constant X planes for interior and boundaries slices (*)
C          = 22  constant Y planes for interior and boundaries slices (*)
C          = 23  constant Z planes for interior and boundaries slices (*)
C   (*) Must also apply at the other end condition in J
C
C   IBCJB, IBCKA, IBCKB likewise
C
C   IVSPEC = 1  volume spec. by cell area times arc length
C          = 2  volume spec. by mixed spherical volumes scaling
C   EPSSS  = parameter that controls how fast spherical volumes are mixed in
C            (used with IVSPEC=2 only)
C   ITSVOL = number of times volumes are averaged
C
C   IMETH  = 0  constant coef. dissipation
C          = 1  spatially-varying coef. dissipation
C          = 2  severe convex corners treated by solving averaging eqns.
C          = 3  severe convex corners treated by angle-bisecting predictor
C   SMU2   = second order dissipation coef.
C
C   TIMJ   = Barth implicitness factor in J
C   TIMK   = Barth implicitness factor in K
C   
C   The following 3 parameters are read in only if axis bc is activated
C
C   IAXIS = 1  extrapolation and volume scaling logic
C         = 2  same as 1 but with dimple smoothing
C   EXAXIS = 0  zeroth order extrapolation at axis
C          > 0 and < 1  control local pointedness at axis (~0.3)
C          = 1  first order extrapolation at axis
C   VOLRES      restrict volume at one point from axis. This parameter is
C               only switched on if exaxis is non-zero. Good values are
C               ~0.1 to ~0.5
C_______________________________________________________________________
C
C    Write version number to file
      WRITE(*,*)'*****************************************************'
      WRITE(*,*)'           Output from HYPGEN version 2.0i.          '
      WRITE(*,*)'*****************************************************'
      WRITE(*,*)
C
C    Read and write out input parameters
c*wdh      READ(*,*) IFORM
      WRITE(*,601) IFORM
C
      READ(*,*) IZSTRT,NZREG
      WRITE(*,602) IZSTRT,NZREG
C
      DO 10 NR=1,NZREG
       READ(*,*) NPZREG(NR),ZREG(NR),DZ0(NR),DZ1(NR)
       WRITE(*,603) NPZREG(NR),ZREG(NR),DZ0(NR),DZ1(NR)
 10   CONTINUE
C
      READ(*,*) IBCJA,IBCJB,IBCKA,IBCKB
      WRITE(*,604) IBCJA,IBCJB,IBCKA,IBCKB
C
      READ(*,*) IVSPEC,EPSSS,ITSVOL
      WRITE(*,605) IVSPEC,EPSSS,ITSVOL
C
      READ(*,*) IMETH,SMU2
      WRITE(*,606) IMETH,SMU2
C
      READ(*,*) TIMJ,TIMK
      WRITE(*,607) TIMJ,TIMK
C
      IF ( (IBCJA.EQ.20) .OR. (IBCJB.EQ.20) ) THEN
       READ(*,*) IAXIS,EXAXIS,VOLRES
       WRITE(*,608) IAXIS,EXAXIS,VOLRES
      ELSE
       IAXIS = 1
       EXAXIS = 0.0
       VOLRES = 0.5
      ENDIF
C
 601  FORMAT(' IFORM =',I2)
 602  FORMAT(' IZSTRT, NZREG =',2I3)
 603  FORMAT(' NPZREG(*), ZREG(*), DZ0(*), DZ1(*) =',I5,3F12.5)
 604  FORMAT(' IBCJA, IBCJB, IBCKA, IBCKB =',4I5)
 605  FORMAT(' IVSPEC, EPSSS, ITSVOL =',I5,F13.5,I5)
 606  FORMAT(' IMETH, SMU2 =',I5,F13.5)
 607  FORMAT(' TIMJ, TIMK =',2F13.5)
 608  FORMAT(' IAXIS, EXAXIS, VOLRES =',I5,2F13.5)
C
C    Initialize variable zeta switch
      IF ((ZREG(1).LE.0.) .OR. (DZ0(1).LT.0.) .OR. (DZ1(1).LT.0.)) THEN
       VZETA = .TRUE.
      ELSE
       VZETA = .FALSE.
      ENDIF
C
C    Sum up number of points in each L-region
      NPTOT = 0
      DO 15 NR=1,NZREG
       NPTOT = NPTOT + NPZREG(NR)
 15   CONTINUE
      NPTOT = NPTOT - NZREG + 1
      LMAX = NPTOT
C
C    Check surface grid file existence and format and read dimensions
      INQUIRE(FILE='surf.i', EXIST=FILEX)
       IF (.NOT. FILEX) THEN
        WRITE(*,*)'Cannot find surf.i file. Program terminated.'
        STOP
       ENDIF
      OPEN (2,FILE='surf.i',STATUS='UNKNOWN',form='formatted')
       READ(2,*,ERR=20) JMAX,KMAX,LDUM
       IFMTSU = 1
       GO TO 30
 20   CLOSE(2)
      OPEN (2,FILE='surf.i',STATUS='UNKNOWN',form='unformatted')
       READ(2) JMAX,KMAX,LDUM
       IFMTSU = 0
 30   CONTINUE
C
C    Check zetavar.i file existence and format and read dimensions
      IF ( (VZETA) .AND. (IZSTRT.NE.-1) ) THEN
       INQUIRE(FILE='zetavar.i', EXIST=FILEX)
        IF (.NOT. FILEX) THEN
         WRITE(*,*)'Cannot find zetavar.i file. Program terminated.'
         STOP
        ENDIF
       OPEN (7,FILE='zetavar.i',STATUS='UNKNOWN',form='formatted')
        READ(7,*,ERR=40) JDUM,KDUM,LDUM
        IFMTVA = 1
        GO TO 50
 40    CLOSE(7)
       OPEN (7,FILE='zetavar.i',STATUS='UNKNOWN',form='unformatted')
        READ(7) JDUM,KDUM,LDUM
        IFMTVA = 0
 50    CONTINUE
       IF ( (JDUM.NE.JMAX) .OR. (KDUM.NE.KMAX) ) THEN
        WRITE(*,*)'Error: surface grid dimensions inconsistent with',
     >            ' dimensions in zetavar.i file.'
        WRITE(*,*)'Program terminated.'
        STOP
       ENDIF
      ENDIF
C
C    Check zetastr.i file existence and format and read dimensions
      IF (IZSTRT.EQ.-1) THEN
       INQUIRE(FILE='zetastr.i', EXIST=FILEX)
       IF (.NOT. FILEX) THEN
        WRITE(*,*)'Cannot find zetastr.i file. Program terminated.'
        STOP
       ENDIF
       OPEN (8,FILE='zetastr.i',STATUS='UNKNOWN',form='formatted')
        READ(8,*,ERR=60) JZS,KZS,LZS
        IFMTST = 1
        GO TO 70
 60    CLOSE(8)
        OPEN (8,FILE='zetastr.i',STATUS='UNKNOWN',form='unformatted')
        READ(8) JZS,KZS,LZS
        IFMTST = 0
 70    CONTINUE
       IF (LZS.NE.LMAX) THEN
        WRITE(*,*)'Warning: no. of points in L in input parameters ',
     >            'file is inconsistent with that in zetastr.i'
        WRITE(*,*)'The LMAX in zetastr.i will be used.'
        LMAX = LZS
       ENDIF
       IF ( ((JZS.NE.1) .AND. (JZS.NE.JMAX)) .OR.
     >      ((KZS.NE.1) .AND. (KZS.NE.KMAX)) ) THEN
        WRITE(*,*)'Error: no. of points in J and/or K in surface grid',
     >            ' is inconsistent with that in zetastr.i'
        WRITE(*,*)'Program terminated.'
        STOP
       ENDIF
      ENDIF
C
      WRITE(*,*) 'JMAX, KMAX, LMAX = ',JMAX,KMAX,LMAX
      WRITE(*,*)
C
C    Determine array dimensions
      JDIM = JMAX+2
      KDIM = KMAX+2
      LDIM = LMAX
C
C    Check if array dimensions are large enough
      CALL CHKDIM(M3D,M2D,M1D,JDIM,KDIM,LDIM)
C
C    Check user input
      CALL CHKINP(IZSTRT,NZREG,NPZREG,ZREG,DZ0,DZ1,
     >            IBCJA,IBCJB,IBCKA,IBCKB,
     >            IVSPEC,EPSSS,ITSVOL,IMETH,SMU2,TIMJ,TIMK,
     >            IAXIS,EXAXIS,VOLRES,VZETA,JMAX,KMAX,LMAX)
C
C    Convert some BC parameters into old input format
      CALL BCCONV(IBCJA,IBCJB,IBCKA,IBCKB,JMAX,KMAX,JPER,KPER,
     >            JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,J2D,
     >            KSYMA,KSYMB,KFLTA,KFLTB,K2D,JPLN1,KPLN1,
     >            JPLNA,JPLNB,KPLNA,KPLNB,EXTJA,EXTJB,EXTKA,EXTKB)
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INITIA2(JDIM,KDIM,M1D,M3D,JMAX,KMAX,LMAX,J1,J2,K1,K2,
     >                  JA,JB,KA,KB,JJP,JJR,KKP,KKR,J2D,K2D,JPER,KPER,
     >                  JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,
     >                  KSYMA,KSYMB,KFLTA,KFLTB,JPLNA,JPLNB,KPLNA,KPLNB,
     >                  EXTJA,EXTJB,EXTKA,EXTKB,JPLN1,KPLN1,PLNKAB,
     >                  IBCJA,IBCJB,IBCKA,IBCKB,ISJA,ISJB,ISKA,ISKB,
     >                  IZSTRT,NZREG,NPZREG,ZREG,DZ0,DZ1,VZETA,JZS,KZS,
     >                  IVSPEC,EPSSS,ITSVOL,IMETH,SMU2,
     >                  IAXIS,EXAXIS,VOLRES,
     >                  X,Y,Z,VOL,SR,SPHI,R,TMP2,XW,YW,ZW,
     >                  XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,
     >                  XIDS,ETDS,DISSL,ADXI,ADET,ADRXI,ADRET,
     >                  LTRANS,LTEST,RJMAXM,RKMAXM,CMIN,SCALEL,
     >                  DAREA,RR,RADIS,DTHET,DLC,DAREAS,
     >                  JFLAGA,JFLAGB,KFLAGA,KFLAGB,ITLE,ITTE,LSLE,LSTE,
     >                  JAOUT,JBOUT,KAOUT,KBOUT,IFMTSU,IFMTVA,IFMTST)
c*wdh*
c* include "precis.h"
C
      PARAMETER (I1=1, I2=2)
      LOGICAL JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,J2D,
     >        KSYMA,KSYMB,KFLTA,KFLTB,K2D,VZETA
C
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM)
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3),
     >          JPLN1(3),KPLN1(3),PLNKAB(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM),
     >          VOL(JDIM,KDIM),SR(JDIM,KDIM,3),SPHI(M1D),R(M1D)
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION DAREA(JDIM,KDIM),RR(JDIM,KDIM,LMAX),
     >          DLC(JDIM),DAREAS(JDIM)
      DIMENSION XIDS(JDIM,KDIM),ETDS(JDIM,KDIM),DISSL(LMAX),
     >          ADXI(JDIM,KDIM),ADET(JDIM,KDIM),
     >          ADRXI(JDIM,KDIM),ADRET(JDIM,KDIM)
      DIMENSION NPZREG(NZREG), ZREG(NZREG), DZ0(NZREG), DZ1(NZREG)
      DIMENSION TMP2(JDIM,KDIM,6), XW(M3D), YW(M3D), ZW(M3D)
C
C    Initialize constants
      PI = 4.*ATAN(1.)
C
C    Read in body surface grid. Make sure the grid is read in with
C    the current JMAX,KMAX before extra planes are added. Also read
C    in variable zetavr array if needed (store temporarily in TMP2).
C    Also read in zetastr.i file if needed.
      CALL HINIGRD(JDIM,KDIM,JMAX,KMAX,LMAX,JZS,KZS,VZETA,IZSTRT,
     >            IFMTSU,IFMTVA,IFMTST,X,Y,Z,TMP2(1,1,4),RR)
C
C    Check for consecutive coincident points along an interior
C    coordinate line on surface grid
      CALL CHKGRD(JDIM,KDIM,JMAX,KMAX,JPER,KPER,X,Y,Z)
C
C    Check for negative values in zetavar.i file
      IF (VZETA) THEN
      CALL CHKVAR(JDIM,KDIM,JMAX,KMAX,ZREG(1),DZ0(1),DZ1(1),TMP2(1,1,4))
      ENDIF
C
C    Check for negative step size in zetastr.i file
      IF (IZSTRT.EQ.-1) THEN
       CALL CHKSTR(JDIM,KDIM,JZS,KZS,LMAX,RR)
      ENDIF
C
C    Reset JMAX,KMAX if periodic
      IF (JPER.EQ.1) JMAX = JMAX-1
      IF (KPER.EQ.1) KMAX = KMAX-1
C
C    Pre-processing if there are free-floating edges or it is a 2D case.
C    Also check for singular points at boundaries.
      JAOUT = 1
      JBOUT = JMAX
      KAOUT = 1
      KBOUT = KMAX
      IF ( (JMAX.EQ.1) .OR. (KMAX.EQ.1) ) THEN
       CALL ADD3D(JDIM,KDIM,JMAX,KMAX,IBCJA,IBCJB,IBCKA,IBCKB,
     >           VZETA,X,Y,Z,PLNKAB,TMP2(1,1,4),JAOUT,JBOUT,KAOUT,KBOUT)
      ENDIF
      CALL CHKSPT(JDIM,KDIM,JMAX,KMAX,X,Y,Z,ISJA,ISJB,ISKA,ISKB)
      IF ( (IBCJA.LE.-1) .OR. (IBCJB.LE.-1) .OR.
     >     (IBCKA.LE.-1) .OR. (IBCKB.LE.-1) ) THEN
       CALL ADDEDG(JDIM,KDIM,JMAX,KMAX,LMAX,X,Y,Z,TMP2(1,1,4),RR,
     >             VZETA,IZSTRT,JZS,KZS,IBCJA,IBCJB,IBCKA,IBCKB,
     >             ISJA,ISJB,JAOUT,JBOUT,KAOUT,KBOUT)
      ENDIF
      IF ( (IBCJA.GT.20) .OR. (IBCKA.GT.20) ) THEN
       CALL ADDEXE(JDIM,KDIM,JMAX,KMAX,LMAX,JPLNA,KPLNA,JPLN1,KPLN1,
     >             X,Y,Z,TMP2(1,1,4),RR,VZETA,IZSTRT,
     >             JZS,KZS,IBCJA,IBCKA,JAOUT,JBOUT,KAOUT,KBOUT)
      ENDIF
C
C    Set begin and end indices
      JM = JMAX -1
      KM = KMAX -1
      IF (JPER .EQ. 1) THEN
       JA = 1
       JB = JMAX
      ELSE       
       JA = 2
       JB = JM
      ENDIF
      IF (KPER .EQ. 1) THEN
       KA = 1
       KB = KMAX
      ELSE       
       KA = 2
       KB = KM
      ENDIF
C
C    Set up periodic indices
      DO 12 J = 1,JMAX
       JJP(J) = J+1
       JJR(J) = J-1
 12   CONTINUE
      IF (JPER.EQ.0) THEN
       JJP(JMAX) = JMAX
       JJR(1) = 1
      ELSE IF (JPER.EQ.1) THEN
       JJP(JMAX) = 1
       JJR(1) = JMAX
      ENDIF
      DO 14 K = 1,KMAX
       KKP(K) = K+1
       KKR(K) = K-1
 14   CONTINUE
      IF (KPER.EQ.0) THEN
       KKP(KMAX) = KMAX
       KKR(1) = 1
      ELSE IF (KPER.EQ.1) THEN
       KKP(KMAX) = 1
       KKR(1) = KMAX
      ENDIF
C
C    Initialize some variables
      DO 20 K=1,KMAX
      DO 20 J=1,JMAX
       VOL(J,K) = 0.0
       XIDS(J,K) = 1.0
       ETDS(J,K) = 1.0
       ADXI(J,K) = 1.0
       ADET(J,K) = 1.0
 20   CONTINUE
      DO 22 N=1,3
      DO 22 K=1,KMAX
      DO 22 J=1,JMAX
       SR(J,K,N) = 0.0
 22   CONTINUE
C
C    Reset appropriate symmetry plane coordinates for exact reflection on surface
      CALL SETSYM(JDIM,KDIM,X,Y,Z,JA,JB,KA,KB,JMAX,KMAX,
     >            JSYMA,JSYMB,KSYMA,KSYMB,JPLNA,JPLNB,KPLNA,KPLNB)
C
C    Reset collapsed edge on surface to be compatible with bc
      CALL SETEDG(JDIM,KDIM,JMAX,KMAX,IBCJA,IBCJB,IBCKA,IBCKB,X,Y,Z)
C
C    Set up flags for bc and block tri-diagonal solvers routines
      JFLAGA = 0
      JFLAGB = 0
      IF (JSYMA) JFLAGA = 1
      IF (JSYMB) JFLAGB = 1
      IF ((JFLTA) .AND. (EXTJA.NE.0.0)) JFLAGA = 1
      IF ((JFLTB) .AND. (EXTJB.NE.0.0)) JFLAGB = 1
      IF ((IAXIS.GE.1).AND.(EXAXIS.NE.0).AND.JAXSA) JFLAGA = 1
      IF ((IAXIS.GE.1).AND.(EXAXIS.NE.0).AND.JAXSB) JFLAGB = 1
      KFLAGA = 0
      KFLAGB = 0
      IF (KSYMA) KFLAGA = 1
      IF (KSYMB) KFLAGB = 1
      IF ((KFLTA) .AND. (EXTKA.NE.0.0)) KFLAGA = 1
      IF ((KFLTB) .AND. (EXTKB.NE.0.0)) KFLAGB = 1
C
C    Set up flags for collapsed edge bc at leading and trailing edges
      ITLE = 1
      ITTE = 1
      LSLE = 1
      LSTE = 1
C
C    Store coordinate of constant K-plane if any
      IF ((JAXSA .OR. JAXSB).AND.(KPER.EQ.0)) THEN
       IF (KSYMA) KINDEX = 2
       IF (KFLTA) KINDEX = 1
       PLNKAB(1) = (1.0-KPLNA(1))*X(1,KINDEX)
       PLNKAB(2) = (1.0-KPLNA(2))*Y(1,KINDEX)
       PLNKAB(3) = (1.0-KPLNA(3))*Z(1,KINDEX)
      ENDIF
C
C    Set up 1D stretching function in zeta
      CALL ZSPACS(JDIM,KDIM,M1D,JMAX,KMAX,LMAX,VZETA,
     >            IZSTRT,NZREG,NPZREG,ZREG,DZ0,DZ1,JZS,KZS,
     >            SPHI,TMP2(1,1,4),RR)
C
C    Compute metrics and area elements
      CALL METRAR(JDIM,KDIM,JA,JB,KA,KB,JPER,KPER,JJP,JJR,KKP,KKR,
     >            IBCJA,IBCKA,JPLN1,KPLN1,X,Y,Z,VOL,
     >            XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,DAREA)
C
      IF (IVSPEC.EQ.2) THEN
c
c      First, compute total area of surface
        BAREA = 0.0
        DO 128 K = KA-1+KPER,KB+1-KPER
        DO 128 J = JA-1+JPER,JB+1-JPER
         BAREA = BAREA + DAREA(J,K)
 128    CONTINUE
c
c      Compute characteristic radius of sphere and other parameters
        DPH = (0.5*PI*(1.0+JPER))/(JM+JPER)
        DTHET = (1.0+KPER)*PI/(KM+KPER)
        IF (JAXSA.AND.JAXSB) THEN
         RADIS = SQRT(BAREA/(4.0*PI))
         DO 129 J=JA,JB
          PHI = DPH*FLOAT(J-1)
          SPHI(J) = SIN(PHI)*DTHET*DPH
          DAREAS(J) = RADIS*RADIS*SPHI(J)
 129     CONTINUE
        ELSE
c        Compute distance between grid points in J-direction
         DO 110 J=2,JMAX
          DISTJ = SQRT ( (X(J,1)-X(J-1,1))**2 + (Y(J,1)-Y(J-1,1))**2
     &                 + (Z(J,1)-Z(J-1,1))**2 )
          DLC(J) = DISTJ
 110     CONTINUE
         DLC(1) = DLC(2)
         JH    = (JMAX+1)/2
         DIST1 = SQRT ( (X(JMAX,1)-X(1,1))**2 + (Y(JMAX,1)-Y(1,1))**2
     &                + (Z(JMAX,1)-Z(1,1))**2 )
         DIST2 = SQRT ( (X(JH,1)-X(1,1))**2 + (Y(JH,1)-Y(1,1))**2
     &                + (Z(JH,1)-Z(1,1))**2 )
         DISTO = MAX(DIST1,DIST2)
         RADIS = 0.5*BAREA/(2.0*PI*DISTO)
         DO 130 J=JA,JB
          PHI = DPH*FLOAT(J-1)
          SPHI(J) = SIN(PHI)*DTHET*DPH
          DAREAS(J) = DLC(J)*DTHET*RADIS
 130     CONTINUE
        ENDIF
C
      ENDIF
C
C    Compute control volumes
      CALL CONVOL(JDIM,KDIM,M1D,LMAX,JA,JB,KA,KB,2,
     >            JPER,KPER,JJP,JJR,KKP,KKR,JAXSA,JAXSB,VZETA,
     >            IAXIS,EXAXIS,VOLRES,IVSPEC,EPSSS,ITSVOL,
     >            RADIS,DTHET,SPHI,DLC,DAREAS,DAREA,VOL,RR)
C
C    Compute dissipation coef scale factor in L
       IF (IMETH.EQ.0) THEN
        DO 144 L=1,LMAX
         DISSL(L) = SMU2
 144    CONTINUE
       ELSE IF (IMETH.GE.1) THEN
        LTEST = 0.75*LMAX
        LTRANS = LMAX
        DO 145 L=2,LMAX
         DISSL(L) = SMU2*SQRT( FLOAT(L-1)/FLOAT(LMAX-1) )
 145    CONTINUE
       ENDIF
       CALL DISVAR(JDIM,KDIM,LMAX,JA,JB,KA,KB,I2,JJP,JJR,KKP,KKR,
     >             X,Y,Z,IMETH,SMU2,LTEST,LTRANS,DISSL,I1,
     >             RJMAXM,RKMAXM,ADXI,ADET,ADRXI,ADRET,
     >             TMP2(1,1,1),TMP2(1,1,2))
C
C    Initialize some parameters for imeth=3
       CMIN   = 1.0
       SCALEL = 1.0
C
C    Set output indices
       IF (JPER.EQ.0) THEN
        j1 = jaout
        j2 = jbout
       ELSE IF (JPER.EQ.1) THEN
        J1 = 1
        J2 = JMAX+1
       ENDIF
       IF (KPER.EQ.0) THEN
        k1 = kaout
        k2 = kbout
       ELSE IF (KPER.EQ.1) THEN
        K1 = 1
        K2 = KMAX+1
       ENDIF
C
c     Steger's indices
c       J1 = JA -1 +JPER
c       J2 = JB +1 -JPER
c       K1 = KA -1 +KPER
c       K2 = KB +1 -KPER
C
C    Initialize XW,YW,ZW
      DO 160 I=1,M3D
       XW(I) = 0.0
       YW(I) = 0.0
       ZW(I) = 0.0
 160  CONTINUE
c
      RETURN
      END
C***********************************************************************
      SUBROUTINE HMETBLN(M1D,JDIM,KDIM,JMAX,KMAX,JA,JB,KA,KB,
     >                  IBCJA,IBCJB,IBCKA,IBCKB,ISJA,ISJB,ISKA,ISKB,
     >                  JJP,JJR,KKP,KKR,JPLNA,JPLNB,KPLNA,KPLNB,
     >                  BNX,BNY,BNZ,PHI,NPA,IDIV,UNI,X,Y,Z,XZ,YZ,ZZ)
c*wdh*
c* include "precis.h"
C
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM)
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3)
      DIMENSION BNX(M1D), BNY(M1D), BNZ(M1D),
     >          PHI(M1D), NPA(M1D), IDIV(M1D), UNI(M1D)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
C
      IF ( (IBCJA.GE.1) .AND. (IBCJA.LE.7) ) THEN
C
        JBN = JA-1
        IF ((ISJA.EQ.1).AND.(IBCJA.NE.7)) JBN = JA
C
C       Determine unit normal vector at boundary, no. of pts. to modify
         CALL BNDUNJ(JDIM,KDIM,JMAX,KMAX,JBN,JA,KA,KB,KKP,KKR,JPLNA,
     >               IBCJA,IBCKA,X,Y,Z,XZ,YZ,ZZ,BNX,BNY,BNZ,
     >               PHI,NPA,IDIV,UNI)
C
C       Rotate interior normals to blend with boundary normal
         DO 10 K=KA,KB
          RJ = 1.0
          JAF = JA + NPA(K) - 1
          IF (IDIV(K).EQ.0) THEN
            DO 11 J=JA,JAF-1
             RJ = ( FLOAT(JAF-J)/FLOAT(NPA(K)) )**0.2
             CALL METROT(JDIM,KDIM,J,K,BNX(K),BNY(K),BNZ(K),PHI(K),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 11          CONTINUE
          ELSE IF (IDIV(K).EQ.1) THEN
            DO 12 J=JA,JAF-1
             RJ = 0.5*RJ
             CALL METROT(JDIM,KDIM,J,K,BNX(K),BNY(K),BNZ(K),PHI(K),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 12         CONTINUE
          ENDIF
 10      CONTINUE
C
      ENDIF
C
      IF ( (IBCJB.GE.1) .AND. (IBCJB.LE.7) ) THEN
C
        JBN = JB+1
        IF ((ISJB.EQ.1).AND.(IBCJB.NE.7)) JBN = JB
C
C       Determine unit normal vector at boundary, no. of pts. to modify
         CALL BNDUNJ(JDIM,KDIM,JMAX,KMAX,JBN,JB,KA,KB,KKP,KKR,JPLNB,
     >               IBCJB,IBCKB,X,Y,Z,XZ,YZ,ZZ,BNX,BNY,BNZ,
     >               PHI,NPA,IDIV,UNI)
C
C       Rotate interior normals to blend with boundary normal
         DO 20 K=KA,KB
          RJ = 1.0
          JAF = JB - NPA(K) + 1
          IF (IDIV(K).EQ.0) THEN
            DO 21 J=JB,JAF+1,-1
             RJ = ( FLOAT(J-JAF)/FLOAT(NPA(K)) )**0.2
             CALL METROT(JDIM,KDIM,J,K,BNX(K),BNY(K),BNZ(K),PHI(K),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 21         CONTINUE
          ELSE IF (IDIV(K).EQ.1) THEN
            DO 22 J=JB,JAF+1,-1
             RJ = 0.5*RJ
             CALL METROT(JDIM,KDIM,J,K,BNX(K),BNY(K),BNZ(K),PHI(K),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 22         CONTINUE
          ENDIF
 20      CONTINUE
C
      ENDIF
C
      IF ( (IBCKA.GE.1) .AND. (IBCKA.LE.7) ) THEN
C
        KBN = KA-1
        IF ((ISKA.EQ.1).AND.(IBCKA.NE.7)) KBN = KA
C
C       Determine unit normal vector at boundary, no. of pts. to modify
         CALL BNDUNK(JDIM,KDIM,JMAX,KMAX,JA,JB,KBN,KA,JJP,JJR,KPLNA,
     >               IBCKA,IBCJA,X,Y,Z,XZ,YZ,ZZ,BNX,BNY,BNZ,
     >               PHI,NPA,IDIV,UNI)
C
C       Rotate interior normals to blend with boundary normal
         DO 30 J=JA,JB
          RJ = 1.0
          KAF = KA + NPA(J) - 1
          IF (IDIV(J).EQ.0) THEN
            DO 31 K=KA,KAF-1
             RJ = ( FLOAT(KAF-K)/FLOAT(NPA(J)) )**0.2
             CALL METROT(JDIM,KDIM,J,K,BNX(J),BNY(J),BNZ(J),PHI(J),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 31         CONTINUE
          ELSE IF (IDIV(J).EQ.1) THEN
            DO 32 K=KA,KAF-1
             RJ = 0.5*RJ
             CALL METROT(JDIM,KDIM,J,K,BNX(J),BNY(J),BNZ(J),PHI(J),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 32         CONTINUE
          ENDIF
 30      CONTINUE
C
      ENDIF
C
      IF ( (IBCKB.GE.1) .AND. (IBCKB.LE.7) ) THEN
C
        KBN = KB+1
        IF ((ISKB.EQ.1).AND.(IBCKB.NE.7)) KBN = KB
C
C       Determine unit normal vector at boundary, no. of pts. to modify
         CALL BNDUNK(JDIM,KDIM,JMAX,KMAX,JA,JB,KBN,KB,JJP,JJR,KPLNB,
     >               IBCKB,IBCJB,X,Y,Z,XZ,YZ,ZZ,BNX,BNY,BNZ,
     >               PHI,NPA,IDIV,UNI)
C
C       Rotate interior normals to blend with boundary normal
         DO 40 J=JA,JB
          RJ = 1.0
          KAF = KB - NPA(J) + 1
          IF (IDIV(J).EQ.0) THEN
            DO 41 K=KB,KAF+1,-1
             RJ = ( FLOAT(K-KAF)/FLOAT(NPA(J)) )**0.2
             CALL METROT(JDIM,KDIM,J,K,BNX(J),BNY(J),BNZ(J),PHI(J),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 41         CONTINUE
          ELSE IF (IDIV(J).EQ.1) THEN
            DO 42 K=KB,KAF+1,-1
             RJ = 0.5*RJ
             CALL METROT(JDIM,KDIM,J,K,BNX(J),BNY(J),BNZ(J),PHI(J),RJ,
     >                   X,Y,Z,XZ,YZ,ZZ)
 42         CONTINUE
          ENDIF
 40      CONTINUE
C
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE METRAR(JDIM,KDIM,JA,JB,KA,KB,JPER,KPER,JJP,JJR,KKP,KKR,
     >                  IBCJA,IBCKA,JPLN1,KPLN1,X,Y,Z,VOL,
     >                  XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,DAREA)
c*wdh*
c* include "precis.h"
C
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM),
     >          JPLN1(3),KPLN1(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM),VOL(JDIM,KDIM)
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION DAREA(JDIM,KDIM)
C
C   ------------------------------------------------------------------
C    Compute metrics and area elements. Metrics xxi,yxi,zxi,xeta,yeta,
C    zeta are found by differencing. Metrics xzeta,yzeta,zzeta are
C    found from nonlinear governing equations.
C   ------------------------------------------------------------------
C
C    Zeroth (iord=0) or first (iord=1) order extrapolation at ends
      IORD = 1
C
      DO 10 J = JA-1+JPER,JB+1-JPER
       DO 15 K=KA,KB
        KP = KKP(K)
        KR = KKR(K)
        XE(J,K) = ( X(J,KP) - X(J,KR))*.5
        YE(J,K) = ( Y(J,KP) - Y(J,KR))*.5
        ZE(J,K) = ( Z(J,KP) - Z(J,KR))*.5
 15    CONTINUE
       IF (KPER .EQ. 0) THEN
        K = KA-1
c        IF (IORD.EQ.1) THEN
         XE(J,K) = -1.5*X(J,K) + 2.*X(J,K+1) -.5*X(J,K+2)
         YE(J,K) = -1.5*Y(J,K) + 2.*Y(J,K+1) -.5*Y(J,K+2)
         ZE(J,K) = -1.5*Z(J,K) + 2.*Z(J,K+1) -.5*Z(J,K+2)
c        ELSE IF (IORD.EQ.0) THEN
c         XE(J,K) = X(J,K+1)-X(J,K)
c         YE(J,K) = Y(J,K+1)-Y(J,K)
c         ZE(J,K) = Z(J,K+1)-Z(J,K)
c        ENDIF
       K = KB+1
c       IF (IORD.EQ.1) THEN
        XE(J,K) = 1.5*X(J,K) -2.*X(J,K-1) + .5*X(J,K-2)
        YE(J,K) = 1.5*Y(J,K) -2.*Y(J,K-1) + .5*Y(J,K-2)
        ZE(J,K) = 1.5*Z(J,K) -2.*Z(J,K-1) + .5*Z(J,K-2)
c       ELSE IF (IORD.EQ.0) THEN
c        XE(J,K) = X(J,K)-X(J,K-1)
c        YE(J,K) = Y(J,K)-Y(J,K-1)
c        ZE(J,K) = Z(J,K)-Z(J,K-1)
c       ENDIF
       ENDIF
 10   CONTINUE
C
      DO 20 K = KA-1+KPER,KB+1-KPER
       DO 25 J=JA,JB
        JP = JJP(J)
        JR = JJR(J)
        XX(J,K) = ( X(JP,K) - X(JR,K))*.5
        YX(J,K) = ( Y(JP,K) - Y(JR,K))*.5
        ZX(J,K) = ( Z(JP,K) - Z(JR,K))*.5
 25    CONTINUE
       IF (JPER .EQ. 0) THEN
        J = JA-1
c        IF (IORD.EQ.1) THEN
         XX(J,K) = -1.5*X(J,K) + 2.*X(J+1,K) -.5*X(J+2,K)
         YX(J,K) = -1.5*Y(J,K) + 2.*Y(J+1,K) -.5*Y(J+2,K)
         ZX(J,K) = -1.5*Z(J,K) + 2.*Z(J+1,K) -.5*Z(J+2,K)
c        ELSE IF (IORD.EQ.0) THEN
c         XX(J,K) = X(J+1,K)-X(J,K)
c         YX(J,K) = Y(J+1,K)-Y(J,K)
c         ZX(J,K) = Z(J+1,K)-Z(J,K)
c        ENDIF
       J = JB+1
c       IF (IORD.EQ.1) THEN
        XX(J,K) = 1.5*X(J,K) -2.*X(J-1,K) + .5*X(J-2,K)
        YX(J,K) = 1.5*Y(J,K) -2.*Y(J-1,K) + .5*Y(J-2,K)
        ZX(J,K) = 1.5*Z(J,K) -2.*Z(J-1,K) + .5*Z(J-2,K)
c       ELSE IF (IORD.EQ.0) THEN
c        XX(J,K) = X(J,K)-X(J-1,K)
c        YX(J,K) = Y(J,K)-Y(J-1,K)
c        ZX(J,K) = Z(J,K)-Z(J-1,K)
c       ENDIF
       ENDIF
 20   CONTINUE
C
C    Check for singular metrics and do one-sided differencing
      DO 30 K = KA-1+KPER,KB+1-KPER
       KR = KKR(K)
      DO 30 J = JA-1+JPER,JB+1-JPER
       JR = JJR(J)
       IF ( (XE(J,K).EQ.0.0) .AND. (YE(J,K).EQ.0.0) .AND.
     >      (ZE(J,K).EQ.0.0) ) THEN
              XE(J,K) = X(J,K) - X(J,KR)
              YE(J,K) = Y(J,K) - Y(J,KR)
              ZE(J,K) = Z(J,K) - Z(J,KR)
       ENDIF
       IF ( (XX(J,K).EQ.0.0) .AND. (YX(J,K).EQ.0.0) .AND.
     >      (ZX(J,K).EQ.0.0) ) THEN
              XX(J,K) = X(J,K) - X(JR,K)
              YX(J,K) = Y(J,K) - Y(JR,K)
              ZX(J,K) = Z(J,K) - Z(JR,K)
       ENDIF
 30   CONTINUE
C
C    Zero out appropriate metrics for constant interior planes option
      IF ( (IBCJA.GE.21) .AND. (IBCJA.LE.23) ) THEN
       DO 40 K = KA-1+KPER,KB+1-KPER
       DO 40 J = JA-1+JPER,JB+1-JPER
        XX(J,K) = XX(J,K)*JPLN1(1)
        YX(J,K) = YX(J,K)*JPLN1(2)
        ZX(J,K) = ZX(J,K)*JPLN1(3)
 40    CONTINUE
      ENDIF
      IF ( (IBCKA.GE.21) .AND. (IBCKA.LE.23) ) THEN
       DO 50 K = KA-1+KPER,KB+1-KPER
       DO 50 J = JA-1+JPER,JB+1-JPER
        XE(J,K) = XE(J,K)*KPLN1(1)
        YE(J,K) = YE(J,K)*KPLN1(2)
        ZE(J,K) = ZE(J,K)*KPLN1(3)
 50    CONTINUE
      ENDIF
C
C    Metrics in zeta are formed from governing p.d.e's by inverting
C    the C coef. matrix into the vector (0,0,vol). Surface area elements
C    are computed using metrics at surface grid points.
       DO 60 K=KA,KB
       DO 60 J=JA,JB
        C1 = YX(J,K)*ZE(J,K) - YE(J,K)*ZX(J,K)
        C2 = XE(J,K)*ZX(J,K) - XX(J,K)*ZE(J,K)
        C3 = XX(J,K)*YE(J,K) - XE(J,K)*YX(J,K)
        DET = C1*C1 + C2*C2 + C3*C3
        VRD = VOL(J,K)/DET
        XZ(J,K) = C1*VRD
        YZ(J,K) = C2*VRD
        ZZ(J,K) = C3*VRD
        DAREA(J,K) = SQRT(DET)
 60    CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE METROT(JDIM,KDIM,J,K,UNX,UNY,UNZ,PHI,RJ,X,Y,Z,XZ,YZ,ZZ)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
C
C    Rotate interior zeta metrics by specified amount
      AX = YZ(J,K)*UNZ - ZZ(J,K)*UNY
      AY = ZZ(J,K)*UNX - XZ(J,K)*UNZ
      AZ = XZ(J,K)*UNY - YZ(J,K)*UNX
      AN = 1.0/SQRT( AX*AX + AY*AY + AZ*AZ )
      AX = AX*AN
      AY = AY*AN
      AZ = AZ*AN
      PX = XZ(J,K)
      PY = YZ(J,K)
      PZ = ZZ(J,K)
      RPHI = RJ*PHI
      CDEG = COS(RPHI)
      SDEG = SIN(RPHI)
      CDEG1 = 1.0-CDEG
      CPDOTA = CDEG1*(PX*AX + PY*AY + PZ*AZ)
      XZ(J,K) = CPDOTA*AX + CDEG*PX + SDEG*(AY*PZ-AZ*PY)
      YZ(J,K) = CPDOTA*AY + CDEG*PY + SDEG*(AZ*PX-AX*PZ)
      ZZ(J,K) = CPDOTA*AZ + CDEG*PZ + SDEG*(AX*PY-AY*PX)
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE OUTLZ(JDIM,KDIM,JMAX,KMAX,J1,J2,K1,K2,
     >                 JPER,KPER,XZ,YZ,ZZ,VOL)
c*wdh*
c* include "precis.h"
C
      DIMENSION XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION VOL(JDIM,KDIM)
C
C    Load periodic plane for output
      IF (JPER.EQ.1) THEN
       DO 30 K=1,KMAX
        XZ(JMAX+1,K) = XZ(1,K)
        YZ(JMAX+1,K) = YZ(1,K)
        ZZ(JMAX+1,K) = ZZ(1,K)
 30    CONTINUE
      ENDIF
      IF (KPER.EQ.1) THEN
       DO 40 J=1,JMAX
        XZ(J,KMAX+1) = XZ(J,1)
        YZ(J,KMAX+1) = YZ(J,1)
        ZZ(J,KMAX+1) = ZZ(J,1)
 40    CONTINUE
      ENDIF
      WRITE(21) ((VOL(J,K),J=J1,J2),K=K1,K2),
     >          ((XZ(J,K),J=J1,J2),K=K1,K2),
     >          ((YZ(J,K),J=J1,J2),K=K1,K2),
     >          ((ZZ(J,K),J=J1,J2),K=K1,K2),
     >          ((VOL(J,K),J=J1,J2),K=K1,K2)
      RETURN
      END
C***********************************************************************
      SUBROUTINE OUTCOM(JDIM,KDIM,J1,J2,K1,K2,JMAX,KMAX,LMAX,IFORM,
     >                  IBCJA,IBCJB,IBCKA,IBCKB,XW,YW,ZW)
c*wdh*
c* include "precis.h"
C
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C   ----------------------------------------
C    Write PLOT3D command files for viewing
C   ----------------------------------------
C
C    Write minmax command file
C
      OPEN(UNIT=3,FILE='minmax.com',STATUS='UNKNOWN',FORM='FORMATTED')
       XMIN = XW(J1,K1,1)
       XMAX = XW(J1,K1,1)
       YMIN = YW(J1,K1,1)
       YMAX = YW(J1,K1,1)
       ZMIN = ZW(J1,K1,1)
       ZMAX = ZW(J1,K1,1)
       DO 10 K=K1,K2
       DO 10 J=J1,J2
        XMIN = MIN( XMIN, XW(J,K,1) )
        XMAX = MAX( XMAX, XW(J,K,1) )
        YMIN = MIN( YMIN, YW(J,K,1) )
        YMAX = MAX( YMAX, YW(J,K,1) )
        ZMIN = MIN( ZMIN, ZW(J,K,1) )
        ZMAX = MAX( ZMAX, ZW(J,K,1) )
 10    CONTINUE
       DIAG = SQRT( (XMAX-XMIN)**2 + (YMAX-YMIN)**2 + (ZMAX-ZMIN)**2 )
       DIST = 2.5*DIAG
       WRITE(3,101) XMIN,XMAX,YMIN,YMAX,ZMIN,ZMAX
       WRITE(3,102) DIST
      CLOSE(3)
C
C    Write surface command file
C
      OPEN(UNIT=3,FILE='surf.com',STATUS='UNKNOWN',FORM='FORMATTED')
       IF (IFORM.EQ.0) THEN
        WRITE(3,103)
       ELSE
        WRITE(3,113)
       ENDIF
      CLOSE(3)
C
C    Write edges command file
C
      JS = 1
      JE = JMAX
      KS = 1
      KE = KMAX
      IF (IBCJA.EQ.10) JE = (JMAX+2)/2
      IF (IBCKA.EQ.10) KE = (KMAX+2)/2
      IF ( (IBCJA.GE.11).AND.(IBCJA.LE.13) ) JS = 2
      IF ( (IBCJB.GE.11).AND.(IBCJB.LE.13) ) JE = JMAX-1
      IF ( (IBCKA.GE.11).AND.(IBCKA.LE.13) ) KS = 2
      IF ( (IBCKB.GE.11).AND.(IBCKB.LE.13) ) KE = KMAX-1
      OPEN(UNIT=3,FILE='edges.com',STATUS='UNKNOWN',FORM='FORMATTED')
       IF (IFORM.EQ.0) THEN
        WRITE(3,104)
       ELSE
        WRITE(3,114)
       ENDIF
       WRITE(3,105) js,'red'
       WRITE(3,105) je,'cyan'
       WRITE(3,106) ks,'yellow'
       WRITE(3,106) ke,'green'
       WRITE(3,107)
      CLOSE(3)
C
C    Write far field command file
C
      OPEN(UNIT=3,FILE='far.com',STATUS='UNKNOWN',FORM='FORMATTED')
       IF (IFORM.EQ.0) THEN
        WRITE(3,108)
       ELSE
        WRITE(3,118)
       ENDIF
      CLOSE(3)
C
 101  FORMAT('min ',6E11.3)
 102  FORMAT('vp/a 150 20 ',E11.3)
 103  FORMAT('@minmax',/,'re/u/x=surf.i',/,'w',/,
     >       'a',//,'a',//,'1',///,'white',//////,
     >       'p/noax/noti/noba/noad')
 113  FORMAT('@minmax',/,'re/for/x=surf.i',/,'w',/,
     >       'a',//,'a',//,'1',///,'white',//////,
     >       'p/noax/noti/noba/noad')
 104  FORMAT('@minmax',/,'re/u/x=plot3d.dat',/,'w',/,
     >       'a',//,'a',//,'1',///,'white',////)
 114  FORMAT('@minmax',/,'re/for/x=plot3d.dat',/,'w',/,
     >       'a',//,'a',//,'1',///,'white',////)
 105  FORMAT(I5,//,'a',//,'a'///,A12,////)
 106  FORMAT('a',//,I5,//,'a',///,A12,////)
 107  FORMAT(/,'p/noax/noti/noba/noad')
 108  FORMAT('@minmax',/,'re/u/x=plot3d.dat',/,'w',/,
     >       'a',//,'a',//,'1',///,'rgb .5 .5 .5',/////,
     >       'a',//,'a',//,'l',///,'rgb 1. .5 0.',//////,
     >       'p/noax/noti/noba/noad')
 118  FORMAT('@minmax',/,'re/for/x=plot3d.dat',/,'w',/,
     >       'a',//,'a',//,'1',///,'rgb .5 .5 .5',/////,
     >       'a',//,'a',//,'l',///,'rgb 1. .5 0.',//////,
     >       'p/noax/noti/noba/noad')
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE OUTWGR(JDIM,KDIM,J1,J2,K1,K2,LMAX,IFORM,XW,YW,ZW)
c*wdh*
c* include "precis.h"
C
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C    Write out grid in plot3d whole format
      JPRNT = J2-J1+1
      KPRNT = K2-K1+1
C
      IF (IFORM.EQ.-1) THEN
       OPEN (20,FILE='plot3d.dat',STATUS='UNKNOWN')
       WRITE(20) JPRNT,KPRNT,LMAX
      ELSE IF (IFORM.EQ.0) THEN
       OPEN (20,FILE='plot3d.dat',STATUS='UNKNOWN',FORM='UNFORMATTED')
       WRITE(20) JPRNT,KPRNT,LMAX
      ELSE IF (IFORM.EQ.1) THEN
       OPEN (20,FILE='plot3d.dat',STATUS='UNKNOWN',FORM='FORMATTED')
       WRITE(20,*) JPRNT,KPRNT,LMAX
      ENDIF
C
      IF (IFORM.LE.0) THEN
       WRITE(20)(((XW(J,K,L),J=J1,J2),K=K1,K2),L=1,LMAX),
     >          (((YW(J,K,L),J=J1,J2),K=K1,K2),L=1,LMAX),
     >          (((ZW(J,K,L),J=J1,J2),K=K1,K2),L=1,LMAX)
      ELSE IF (IFORM.EQ.1) THEN
       WRITE(20,*)(((XW(J,K,L),J=J1,J2),K=K1,K2),L=1,LMAX),
     >            (((YW(J,K,L),J=J1,J2),K=K1,K2),L=1,LMAX),
     >            (((ZW(J,K,L),J=J1,J2),K=K1,K2),L=1,LMAX)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE HRHS(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >               JPLN1,KPLN1,IBCJA,IBCKA,IMETH,SMU2,IAXIS,JAXSA,
     >               JAXSB,CMIN,SCALEL,X,Y,Z,XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,
     >               VOLM,VOL,SR,RR,DISSL,XIDS,ETDS,XIDF,ETDF,
     >               ADRXI,ADRET,CAXI,CAET,AFNXI,AFNET,BLN,CVEX)
c*wdh*
c* include "precis.h"
C
      PARAMETER (P1=0.1)
      LOGICAL JAXSA,JAXSB
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM)
      DIMENSION JPLN1(3),KPLN1(3)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION VOLM(JDIM,KDIM),VOL(JDIM,KDIM),SR(JDIM,KDIM,3),
     >          RR(JDIM,KDIM,LMAX),DISSL(LMAX)
      DIMENSION XIDS(JDIM,KDIM),ETDS(JDIM,KDIM),XIDF(JDIM,KDIM,3),
     >          ETDF(JDIM,KDIM,3),ADRXI(JDIM,KDIM),ADRET(JDIM,KDIM),
     >          CAXI(JDIM,KDIM),CAET(JDIM,KDIM),
     >          AFNXI(JDIM,KDIM),AFNET(JDIM,KDIM),BLN(JDIM,KDIM),
     >          CVEX(JDIM,KDIM)
C
C   -------------------------------------------------------------------
C   Warning : XIDF is being used to store the angle-bisecting unit normal
C             computed from subroutine ANGMET. XIDF must be used to
C             restore the unit normal before it is used as a scratch array
C   -------------------------------------------------------------------
C
C   -----------------------
C    Form RHS forcing term
C   -----------------------
      DO 10 K = KA,KB
      DO 10 J = JA,JB
       VRAT = VOL(J,K)/VOLM(J,K)
       SR(J,K,1) = XZ(J,K)*VRAT
       SR(J,K,2) = YZ(J,K)*VRAT
       SR(J,K,3) = ZZ(J,K)*VRAT
 10   CONTINUE
C
C   --------------------------------------------------------------------
C    Impose predicted coordinates at convex corners and adjust smoothing
C   --------------------------------------------------------------------
      CACAV = 0.5
      CAVEX = -0.5
      CAVEXS = -0.4
      SAVEXS = 1.0/(SQRT( 1.0-(CAVEXS*CAVEXS) ))
      IF (L.EQ.2) THEN
       DO 12 K=KA,KB
       DO 12 J=JA,JB
        IF ((CAXI(J,K).LT.CAVEX).OR.(CAET(J,K).LT.CAVEX)) THEN
         CVEX(J,K) = 1.0
        ELSE
         CVEX(J,K) = 0.0
        ENDIF
 12    CONTINUE
      ELSE IF (L.GT.2) THEN
       DO 13 K=KA,KB
       DO 13 J=JA,JB
        IF (CVEX(J,K).EQ.1.0) THEN
         CAMIN = MIN(CAXI(J,K),CAET(J,K))
         IF (CAMIN.GT.CAVEXS) CVEX(J,K) = 0.0
        ENDIF
 13    CONTINUE
      ENDIF
C
      IF (IMETH.EQ.2) THEN
       DO 15 K = KA,KB
       DO 15 J = JA,JB
        IF (CVEX(J,K).EQ.1.0) THEN
          SR(J,K,1) = 0.0
          SR(J,K,2) = 0.0
          SR(J,K,3) = 0.0
          AFNXI(J,K) = 0.0
          AFNET(J,K) = 0.0
        ENDIF
 15    CONTINUE
      ENDIF
      IF (IMETH.EQ.3) THEN
       if (cmin.gt.-0.7) then
        scalel = scalel*0.9
       else
        scalel = 1.0
       endif
       if (l.eq.2) scalel = 1.0
       cmint = 1.0
       DO 16 K = KA,KB
       DO 16 J = JA,JB
        IF (CVEX(J,K).EQ.1.0) THEN
          COSMIN = MIN(CAXI(J,K),CAET(J,K))
          cmint = min(cosmin,cmint)
c          if ((j.eq.23).and.(k.eq.24)) then
c           write(*,*) 'imeth=3 at J,K,L=',j,k,l
c           write(*,*) 'caxi,caet=',caxi(j,k),caet(j,k)
c           write(*,*) 'cmin, scalel =',cmin,scalel
c          endif
          ARCSC = ( MAX( SQRT(1.0-COSMIN**2) , P1 ) ) *SAVEXS*scalel
          ZMAGN = (RR(J,K,L)-RR(J,K,L-1))*ARCSC
          SR(J,K,1) = XIDF(J,K,1)*ZMAGN
          SR(J,K,2) = XIDF(J,K,2)*ZMAGN
          SR(J,K,3) = XIDF(J,K,3)*ZMAGN
          AFNXI(J,K) = 0.0
          AFNET(J,K) = 0.0
c          IF (CAXI(J,K).LT.CAVEX) AFNXI(J,K) = 0.0
c          IF (CAET(J,K).LT.CAVEX) AFNET(J,K) = 0.0
        ENDIF
 16    CONTINUE
       cmin = cmint
      ENDIF
C
C   --------------------------------------------------------------------
C    Set iblank array for smoothing at L=2
C   --------------------------------------------------------------------
      IF (L.EQ.2) THEN
       DO 17 K = KA,KB
       DO 17 J = JA,JB
        BLN(J,K) = 0.0
 17    CONTINUE
       DO 20 K = KA,KB
        KP1 = KKP(K)
        KM1 = KKR(K)
        KP2 = KKP(KP1)
        KM2 = KKR(KM1)
       DO 20 J = JA,JB
        JP1 = JJP(J)
        JM1 = JJR(J)
        JP2 = JJP(JP1)
        JM2 = JJR(JM1)
        IF ((CAXI(J,K).LT.CAVEX).OR.(CAXI(J,K).GT.CACAV)) THEN
         BLN(JP1,K) = 0.5
         BLN(JM1,K) = 0.5
         BLN(JP2,K) = 0.25
         BLN(JM2,K) = 0.25
        ENDIF
        IF ((CAET(J,K).LT.CAVEX).OR.(CAET(J,K).GT.CACAV)) THEN
         BLN(J,KP1) = 0.5
         BLN(J,KM1) = 0.5
         BLN(J,KP2) = 0.25
         BLN(J,KM2) = 0.25
        ENDIF
        IF (CAXI(J,K).GT.CACAV) BLN(J,K) = 1.0
        IF (CAET(J,K).GT.CACAV) BLN(J,K) = 1.0
 20    CONTINUE
       DO 25 K = KA,KB
       DO 25 J = JA,JB
        AFNXI(J,K) = AFNXI(J,K)*BLN(J,K)
        AFNET(J,K) = AFNET(J,K)*BLN(J,K)
 25    CONTINUE
      ENDIF
C
C   ----------------------------------
C    Add numerical dissipation to RHS
C   ----------------------------------
      DO 50 K = KA,KB
       KP = KKP(K)
       KR = KKR(K)
      DO 50 J = JA,JB
       JP = JJP(J)
       JR = JJR(J)
       XIDF(J,K,1) = X(JP,K) - 2.*X(J,K) + X(JR,K)
       XIDF(J,K,2) = Y(JP,K) - 2.*Y(J,K) + Y(JR,K)
       XIDF(J,K,3) = Z(JP,K) - 2.*Z(J,K) + Z(JR,K)
       ETDF(J,K,1) = X(J,KP) - 2.*X(J,K) + X(J,KR)
       ETDF(J,K,2) = Y(J,KP) - 2.*Y(J,K) + Y(J,KR)
       ETDF(J,K,3) = Z(J,KP) - 2.*Z(J,K) + Z(J,KR)
       DX = XX(J,K)**2 + YX(J,K)**2 +ZX(J,K)**2
       DE = XE(J,K)**2 + YE(J,K)**2 +ZE(J,K)**2
       DZ = XZ(J,K)**2 + YZ(J,K)**2 +ZZ(J,K)**2
       CIA = SQRT(DZ/DX)
       CIB = SQRT(DZ/DE)   
       XIDS(J,K) = CIA
       ETDS(J,K) = CIB
 50   CONTINUE
C
C    Do adjustments for constant interior planes
      IF (IBCJA.GT.20) THEN
       DO 60 N=1,3
       DO 60 K=KA,KB
       DO 60 J=JA,JB
        XIDF(J,K,N) = XIDF(J,K,N)*JPLN1(N)
 60    CONTINUE
      ENDIF
      IF (IBCKA.GT.20) THEN
       DO 65 N=1,3
       DO 65 K=KA,KB
       DO 65 J=JA,JB
        ETDF(J,K,N) = ETDF(J,K,N)*KPLN1(N)
 65    CONTINUE
      ENDIF
C
C    Scale circumferential smoothing at axis
      IF ((JAXSA).AND.(IAXIS.EQ.-2)) THEN
       DO 75 K=KA,KB
        ETDS(JA,K) = 0.1*ETDS(JA,K)
        ETDS(JA+1,K) = 0.65*ETDS(JA+1,K)
 75    CONTINUE
      ENDIF
      IF ((JAXSB).AND.(IAXIS.EQ.-2)) THEN
       DO 76 K=KA,KB
        ETDS(JB,K) = 0.1*ETDS(JB,K)
        ETDS(JB-1,K) = 0.65*ETDS(JB-1,K)
 76    CONTINUE
      ENDIF
C
      IF (SMU2.GT.0.0) THEN
       EPSE = DISSL(L)
       DO 80 K=KA,KB
       DO 80 J=JA,JB
        XICO = ADRXI(J,K)*XIDS(J,K)*AFNXI(J,K)
        ETCO = ADRET(J,K)*ETDS(J,K)*AFNET(J,K)
        SR(J,K,1) = SR(J,K,1) + EPSE*(XICO*XIDF(J,K,1)+ETCO*ETDF(J,K,1))
        SR(J,K,2) = SR(J,K,2) + EPSE*(XICO*XIDF(J,K,2)+ETCO*ETDF(J,K,2))
        SR(J,K,3) = SR(J,K,3) + EPSE*(XICO*XIDF(J,K,3)+ETCO*ETDF(J,K,3))
 80    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SETEDG(JDIM,KDIM,JMAX,KMAX,IBCJA,IBCJB,IBCKA,IBCKB,
     >                  X,Y,Z)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
C    Reset collapsed edge on surface grid to be compatible with bc
      IF (IBCJA.EQ.10) THEN
       JADD = 1
      ELSE
       JADD = 0
      ENDIF
      IF (IBCKA.EQ.10) THEN
       KADD = 1
      ELSE
       KADD = 0
      ENDIF
      JS = 1 + JADD
      KS = 1 + KADD
      JMID = (JMAX + JS)/2
      KMID = (KMAX + KS)/2
C
      IF (IBCJA.EQ.7) THEN
       J = 1
       CALL SETJED(JDIM,KDIM,J,KMAX,KMID,KS,X,Y,Z)
      ENDIF
C
      IF (IBCJB.EQ.7) THEN
       J = JMAX
       CALL SETJED(JDIM,KDIM,J,KMAX,KMID,KS,X,Y,Z)
      ENDIF
C
      IF (IBCKA.EQ.7) THEN
       K = 1
       CALL SETKED(JDIM,KDIM,JMAX,JMID,JS,K,X,Y,Z)
      ENDIF
C
      IF (IBCKB.EQ.7) THEN
       K = KMAX
       CALL SETKED(JDIM,KDIM,JMAX,JMID,JS,K,X,Y,Z)
      ENDIF
C                    
      RETURN
      END
C***********************************************************************
      SUBROUTINE SETJED(JDIM,KDIM,J,KMAX,KMID,KS,X,Y,Z)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
C    Reset collapsed J edge
      DO 10 K=KS,KMID-1
       KK = KMAX-K+KS
       XEDG = 0.5*( X(J,K) + X(J,KK) )
       YEDG = 0.5*( Y(J,K) + Y(J,KK) )
       ZEDG = 0.5*( Z(J,K) + Z(J,KK) )
       X(J,K) = XEDG
       Y(J,K) = YEDG
       Z(J,K) = ZEDG
       X(J,KK) = XEDG
       Y(J,KK) = YEDG
       Z(J,KK) = ZEDG
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SETKED(JDIM,KDIM,JMAX,JMID,JS,K,X,Y,Z)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
C
C    Reset collapsed K edge
      DO 10 J=JS,JMID-1
       JJ = JMAX-J+JS
       XEDG = 0.5*( X(J,K) + X(JJ,K) )
       YEDG = 0.5*( Y(J,K) + Y(JJ,K) )
       ZEDG = 0.5*( Z(J,K) + Z(JJ,K) )
       X(J,K) = XEDG
       Y(J,K) = YEDG
       Z(J,K) = ZEDG
       X(JJ,K) = XEDG
       Y(JJ,K) = YEDG
       Z(JJ,K) = ZEDG
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SETSYM(JDIM,KDIM,X,Y,Z,JA,JB,KA,KB,JMAX,KMAX,
     >                  JSYMA,JSYMB,KSYMA,KSYMB,JPLNA,JPLNB,KPLNA,KPLNB)
c*wdh*
c* include "precis.h"
C
      LOGICAL JSYMA,JSYMB,KSYMA,KSYMB
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3)
C
C    Reset appropriate symmetry plane coordinates for exact reflection
      IF (JSYMA) THEN
       F1 = 2.*JPLNA(1) - 1.
       F2 = 2.*JPLNA(2) - 1.
       F3 = 2.*JPLNA(3) - 1.
       DO 45 K=1,KMAX
        X(JA-1,K) = X(JA+1,K)*F1
        Y(JA-1,K) = Y(JA+1,K)*F2
        Z(JA-1,K) = Z(JA+1,K)*F3
        X(JA,K) = X(JA,K)*JPLNA(1)
        Y(JA,K) = Y(JA,K)*JPLNA(2)
        Z(JA,K) = Z(JA,K)*JPLNA(3)
 45    CONTINUE
      ENDIF
      IF (JSYMB) THEN
       F1 = 2.*JPLNB(1) - 1.
       F2 = 2.*JPLNB(2) - 1.
       F3 = 2.*JPLNB(3) - 1.
       DO 46 K=1,KMAX
        X(JB+1,K) = X(JB-1,K)*F1
        Y(JB+1,K) = Y(JB-1,K)*F2
        Z(JB+1,K) = Z(JB-1,K)*F3
        X(JB,K) = X(JB,K)*JPLNB(1)
        Y(JB,K) = Y(JB,K)*JPLNB(2)
        Z(JB,K) = Z(JB,K)*JPLNB(3)
 46    CONTINUE
      ENDIF
      IF (KSYMA) THEN
       F1 = 2.*KPLNA(1) - 1.
       F2 = 2.*KPLNA(2) - 1.
       F3 = 2.*KPLNA(3) - 1.
       DO 47 J=1,JMAX
        X(J,KA-1) = X(J,KA+1)*F1
        Y(J,KA-1) = Y(J,KA+1)*F2
        Z(J,KA-1) = Z(J,KA+1)*F3
        X(J,KA) = X(J,KA)*KPLNA(1)
        Y(J,KA) = Y(J,KA)*KPLNA(2)
        Z(J,KA) = Z(J,KA)*KPLNA(3)
 47    CONTINUE
      ENDIF
      IF (KSYMB) THEN
       F1 = 2.*KPLNB(1) - 1.
       F2 = 2.*KPLNB(2) - 1.
       F3 = 2.*KPLNB(3) - 1.
       DO 48 J=1,JMAX
        X(J,KB+1) = X(J,KB-1)*F1
        Y(J,KB+1) = Y(J,KB-1)*F2
        Z(J,KB+1) = Z(J,KB-1)*F3
        X(J,KB) = X(J,KB)*KPLNB(1)
        Y(J,KB) = Y(J,KB)*KPLNB(2)
        Z(J,KB) = Z(J,KB)*KPLNB(3)
 48    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SMOEDG(JDIM,KDIM,JMAX,KMAX,LMAX,L,IBCJA,IBCJB,IBCKA,
     >                  IBCKB,ITLE,ITTE,LSLE,LSTE,CVEX,X,Y,Z,XW,YW,ZW,
     >                  M1D,XS,YS,ZS)
c*wdh*
c* include "precis.h"
C
      DIMENSION CVEX(JDIM,KDIM),X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
      DIMENSION XS(M1D), YS(M1D), ZS(M1D)
C
C    Smooth out collapsed edge near leading/trailing edge point.
C    (1) Do angle bisection near body surface.
C    (2) When the grid naturally follows the angle bisection direction,
C        switch to (a) smoothing the neighboring point, (b) smoothing
C        the end point with blending in L.
C
      IF (IBCJA.EQ.10) THEN
       JADD = 1
      ELSE
       JADD = 0
      ENDIF
      IF (IBCKA.EQ.10) THEN
       KADD = 1
      ELSE
       KADD = 0
      ENDIF
      JS = 1 + JADD
      KS = 1 + KADD
      JMID = (JMAX + JS)/2
      KMID = (KMAX + KS)/2
C
       IF ( IBCJA.EQ.7 ) THEN
        J = 1
        K = KMID
        IF ( ITLE.EQ.1 ) THEN
         CALL ANBUNC(JDIM,KDIM,J,K,J+1,K,J,K-1,J+1,K+1,J+1,K-1,
     >               X,Y,Z,XNOR,YNOR,ZNOR)
         CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J+1,K+1,XNOR,YNOR,ZNOR,ITLE,
     >               LSLE,X,Y,Z,XW,YW,ZW)
        ENDIF
        CALL SMONBJ(JDIM,KDIM,LMAX,J,K,L,J+1,J+2,ITLE,LSLE,
     >              X,Y,Z,XW,YW,ZW)
        IF ( IBCKA.EQ.10 ) THEN
         K = 1
         KP = 2
         KR = KMAX
         IF ( ITTE.EQ.1 ) THEN
          IF (CVEX(J+1,K).EQ.1.0) THEN
           CALL ANBUNO(JDIM,KDIM,J,K,J+1,K,J,KP,X,Y,Z,
     >                 XNOR,YNOR,ZNOR)
          ELSE
           CALL ANBUNC(JDIM,KDIM,J,K,J+1,K,J,KP,J+1,KP,J+1,KR,
     >                 X,Y,Z,XNOR,YNOR,ZNOR)
          ENDIF
          CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J+1,K+1,XNOR,YNOR,ZNOR,ITTE,
     >                LSTE,X,Y,Z,XW,YW,ZW)
         ENDIF
         CALL SMONBJ(JDIM,KDIM,LMAX,J,K,L,J+1,J+2,ITTE,LSTE,
     >               X,Y,Z,XW,YW,ZW)
        ENDIF
       ENDIF
C
       IF ( IBCJB.EQ.7 ) THEN
        J = JMAX
        K = KMID
        IF ( ITLE.EQ.1 ) THEN
         CALL ANBUNC(JDIM,KDIM,J,K,J-1,K,J,K-1,J-1,K-1,J-1,K+1,
     >               X,Y,Z,XNOR,YNOR,ZNOR)
         CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J-1,K+1,XNOR,YNOR,ZNOR,ITLE,
     >               LSLE,X,Y,Z,XW,YW,ZW)
        ENDIF
        CALL SMONBJ(JDIM,KDIM,LMAX,J,K,L,J-1,J-2,ITLE,LSLE,
     >              X,Y,Z,XW,YW,ZW)
        IF ( IBCKA.EQ.10 ) THEN
         K = 1
         KP = 2
         KR = KMAX
         IF ( ITTE.EQ.1 ) THEN
          IF (CVEX(J-1,K).EQ.1.0) THEN
           CALL ANBUNO(JDIM,KDIM,J,K,J-1,K,J,KP,X,Y,Z,
     >                 XNOR,YNOR,ZNOR)
          ELSE
           CALL ANBUNC(JDIM,KDIM,J,K,J-1,K,J,KP,J-1,KR,J-1,KP,
     >                 X,Y,Z,XNOR,YNOR,ZNOR)
          ENDIF
          CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J-1,K+1,XNOR,YNOR,ZNOR,ITTE,
     >                LSTE,X,Y,Z,XW,YW,ZW)
         ENDIF
         CALL SMONBJ(JDIM,KDIM,LMAX,J,K,L,J-1,J-2,ITTE,LSTE,
     >               X,Y,Z,XW,YW,ZW)
        ENDIF
       ENDIF
C
       IF ( IBCKA.EQ.7 ) THEN
        K = 1
        J = JMID
        IF ( ITLE.EQ.1 ) THEN
         CALL ANBUNC(JDIM,KDIM,J,K,J,K+1,J-1,K,J-1,K+1,J+1,K+1,
     >               X,Y,Z,XNOR,YNOR,ZNOR)
         CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J+1,K+1,XNOR,YNOR,ZNOR,ITLE,
     >               LSLE,X,Y,Z,XW,YW,ZW)
        ENDIF
        CALL SMONBK(JDIM,KDIM,LMAX,J,K,L,K+1,K+2,ITLE,LSLE,
     >              X,Y,Z,XW,YW,ZW)
        IF ( IBCJA.EQ.10 ) THEN
         J = 1
         JP = 2
         JR = JMAX
         IF ( ITTE.EQ.1 ) THEN
          IF (CVEX(J,K+1).EQ.1.0) THEN
           CALL ANBUNO(JDIM,KDIM,J,K,JP,K,J,K+1,X,Y,Z,
     >                 XNOR,YNOR,ZNOR)
          ELSE
           CALL ANBUNC(JDIM,KDIM,J,K,J,K+1,JP,K,JR,K+1,JP,K+1,
     >                 X,Y,Z,XNOR,YNOR,ZNOR)
          ENDIF
          CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J+1,K+1,XNOR,YNOR,ZNOR,ITTE,
     >                LSTE,X,Y,Z,XW,YW,ZW)
         ENDIF
         CALL SMONBK(JDIM,KDIM,LMAX,J,K,L,K+1,K+2,ITTE,LSTE,
     >               X,Y,Z,XW,YW,ZW)
        ENDIF
       ENDIF
C
       IF ( IBCKB.EQ.7 ) THEN
        K = KMAX
        J = JMID
        IF ( ITLE.EQ.1 ) THEN
         CALL ANBUNC(JDIM,KDIM,J,K,J,K-1,J-1,K,J+1,K-1,J-1,K-1,
     >               X,Y,Z,XNOR,YNOR,ZNOR)
         CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J+1,K-1,XNOR,YNOR,ZNOR,ITLE,
     >               LSLE,X,Y,Z,XW,YW,ZW)
        ENDIF
        CALL SMONBK(JDIM,KDIM,LMAX,J,K,L,K-1,K-2,ITLE,LSLE,
     >              X,Y,Z,XW,YW,ZW)
        IF ( IBCJA.EQ.10 ) THEN
         J = 1
         JP = 2
         JR = JMAX
         IF ( ITTE.EQ.1 ) THEN
          IF (CVEX(J,K-1).EQ.1.0) THEN
           CALL ANBUNO(JDIM,KDIM,J,K,JP,K,J,K-1,X,Y,Z,
     >                 XNOR,YNOR,ZNOR)
          ELSE
           CALL ANBUNC(JDIM,KDIM,J,K,J,K-1,JP,K,JP,K-1,JR,K-1,
     >                 X,Y,Z,XNOR,YNOR,ZNOR)
          ENDIF
          CALL SMOEND(JDIM,KDIM,LMAX,J,K,L,J+1,K-1,XNOR,YNOR,ZNOR,ITTE,
     >                LSTE,X,Y,Z,XW,YW,ZW)
         ENDIF
         CALL SMONBK(JDIM,KDIM,LMAX,J,K,L,K-1,K-2,ITTE,LSTE,
     >               X,Y,Z,XW,YW,ZW)
        ENDIF
       ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SMOEND(JDIM,KDIM,LMAX,J,K,L,JN,KN,XNOR,YNOR,ZNOR,ITEST,
     >                  LSWT,X,Y,Z,XW,YW,ZW)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C    Do angle bisection at leading/trailing edge point.
C    Switch back to normal scheme if dot product is greater than dotsw
      DOTSW = 0.95
      LTEST = LMAX/5
C
      XN = X(J,K) - XW(J,K,L-1)
      YN = Y(J,K) - YW(J,K,L-1)
      ZN = Z(J,K) - ZW(J,K,L-1)
      DD = SQRT( XN*XN + YN*YN + ZN*ZN )
      DOT = ABS( (XNOR*XN + YNOR*YN + ZNOR*ZN)/DD )
C
      IF ( ( DOT.LT.DOTSW ) .OR. ( L.LE.LTEST ) ) THEN
        DNJ = SQRT( (X(JN,K) - XW(JN,K,L-1))**2 +
     >              (Y(JN,K) - YW(JN,K,L-1))**2 +
     >              (Z(JN,K) - ZW(JN,K,L-1))**2 )
        DNK = SQRT( (X(J,KN) - XW(J,KN,L-1))**2 +
     >              (Y(J,KN) - YW(J,KN,L-1))**2 +
     >              (Z(J,KN) - ZW(J,KN,L-1))**2 )
        DN = 0.5*( DNJ + DNK )
        X(J,K) = XNOR*DN + XW(J,K,L-1)
        Y(J,K) = YNOR*DN + YW(J,K,L-1)
        Z(J,K) = ZNOR*DN + ZW(J,K,L-1)
      ELSE
        ITEST = 0
        LSWT  = L
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SMONBJ(JDIM,KDIM,LMAX,J,K,L,JN,JNN,ITEST,LSWT,
     >                  X,Y,Z,XW,YW,ZW)
c*wdh*
c* include "precis.h"
C
      PARAMETER (I1=1)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C    Smooth point next to leading or trailing edge (J collapsed edge)
      X(JN,K) = 0.5*(X(J,K) - XW(J,K,L-1) + X(JNN,K) - XW(JNN,K,L-1))
     >        + XW(JN,K,L-1)
      Y(JN,K) = 0.5*(Y(J,K) - YW(J,K,L-1) + Y(JNN,K) - YW(JNN,K,L-1))
     >        + YW(JN,K,L-1)
      Z(JN,K) = 0.5*(Z(J,K) - ZW(J,K,L-1) + Z(JNN,K) - ZW(JNN,K,L-1))
     >        + ZW(JN,K,L-1)
C
C    Smooth leading or trailing edge point
      IF (ITEST.EQ.0) THEN
       LLEV = INT(0.7*(LMAX-LSWT)) + LSWT
       IF (L.LE.LLEV) THEN
        COEF = ( FLOAT(L-LSWT) / FLOAT( MAX(LLEV-LSWT,I1) ) )**2
       ELSE
        COEF = 1.0
       ENDIF
       COEFM = 1.0-COEF
       XSM = 0.25*( X(JN,K) + 2.0*X(J,K) + X(J,K+1) )
       YSM = 0.25*( Y(JN,K) + 2.0*Y(J,K) + Y(J,K+1) )
       ZSM = 0.25*( Z(JN,K) + 2.0*Z(J,K) + Z(J,K+1) )
       X(J,K) = COEF*XSM + COEFM*X(J,K)
       Y(J,K) = COEF*YSM + COEFM*Y(J,K)
       Z(J,K) = COEF*ZSM + COEFM*Z(J,K)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SMONBK(JDIM,KDIM,LMAX,J,K,L,KN,KNN,ITEST,LSWT,
     >                  X,Y,Z,XW,YW,ZW)
c*wdh*
c* include "precis.h"
C
      PARAMETER (I1=1)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C    Smooth point next to leading or trailing edge (K collapsed edge)
      X(J,KN) = 0.5*(X(J,K) - XW(J,K,L-1) + X(J,KNN) - XW(J,KNN,L-1))
     >        + XW(J,KN,L-1)
      Y(J,KN) = 0.5*(Y(J,K) - YW(J,K,L-1) + Y(J,KNN) - YW(J,KNN,L-1))
     >        + YW(J,KN,L-1)
      Z(J,KN) = 0.5*(Z(J,K) - ZW(J,K,L-1) + Z(J,KNN) - ZW(J,KNN,L-1))
     >        + ZW(J,KN,L-1)
C
C    Smooth leading or trailing edge point
      IF (ITEST.EQ.0) THEN
       LLEV = INT(0.7*(LMAX-LSWT)) + LSWT
       IF (L.LE.LLEV) THEN
        COEF = ( FLOAT(L-LSWT) / FLOAT( MAX(LLEV-LSWT,I1) ) )**2
       ELSE
        COEF = 1.0
       ENDIF
       COEFM = 1.0-COEF
       XSM = 0.25*( X(J,KN) + 2.0*X(J,K) + X(J+1,K) )
       YSM = 0.25*( Y(J,KN) + 2.0*Y(J,K) + Y(J+1,K) )
       ZSM = 0.25*( Z(J,KN) + 2.0*Z(J,K) + Z(J+1,K) )
       X(J,K) = COEF*XSM + COEFM*X(J,K)
       Y(J,K) = COEF*YSM + COEFM*Y(J,K)
       Z(J,K) = COEF*ZSM + COEFM*Z(J,K)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE HSTEP(JDIM,KDIM,M1D,JMAX,KMAX,LMAX,JA,JB,KA,KB,L,
     >                JJP,JJR,KKP,KKR,IBCJA,IBCJB,IBCKA,IBCKB,VZETA,
     >                J2D,K2D,JPER,KPER,ISJA,ISJB,ISKA,ISKB,
     >                JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,
     >                KSYMA,KSYMB,KFLTA,KFLTB,JPLNA,JPLNB,KPLNA,KPLNB,
     >                EXTJA,EXTJB,EXTKA,EXTKB,JPLN1,KPLN1,PLNKAB,
     >                SPHI,R,SMU2,TIMJ,TIMK,IVSPEC,EPSSS,ITSVOL,
     >                IAXIS,EXAXIS,VOLRES,X,Y,Z,VOLM,VOL,SR,
     >                XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,XW,YW,ZW,
     >                XIDS,ETDS,DISSL,ADXI,ADET,ADRXI,ADRET,
     >                IMETH,LTRANS,LTEST,RJMAXM,RKMAXM,CMIN,SCALEL,
     >                H,A,B,C,F,
     >                DAREA,RR,RADIS,DTHET,DLC,DAREAS,
     >                JFLAGA,JFLAGB,KFLAGA,KFLAGB,ITLE,ITTE,LSLE,LSTE,
     >                JAOUT,JBOUT,KAOUT,KBOUT,SCR21,SCR22,SCR23,SCR26,
     >                CAXI,CAET,CVEX,AFNXI,AFNET,BLN,
     >                B11,B21,B22,B31,B32,B33,U12,U13,U23)
c*wdh*
c* include "precis.h"
C
      PARAMETER (I0=0)
      LOGICAL JSYMA,JSYMB,JFLTA,JFLTB,JAXSA,JAXSB,J2D,
     >        KSYMA,KSYMB,KFLTA,KFLTB,K2D,VZETA
C
      DIMENSION JJP(JDIM),JJR(JDIM),KKP(KDIM),KKR(KDIM)
      DIMENSION JPLNA(3),JPLNB(3),KPLNA(3),KPLNB(3),
     >          JPLN1(3),KPLN1(3),PLNKAB(3)
      DIMENSION SPHI(M1D),R(M1D)
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM),
     >          VOLM(JDIM,KDIM), VOL(JDIM,KDIM), SR(JDIM,KDIM,3)
      DIMENSION XX(JDIM,KDIM),YX(JDIM,KDIM),ZX(JDIM,KDIM),
     >          XE(JDIM,KDIM),YE(JDIM,KDIM),ZE(JDIM,KDIM),
     >          XZ(JDIM,KDIM),YZ(JDIM,KDIM),ZZ(JDIM,KDIM)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
      DIMENSION XIDS(JDIM,KDIM),ETDS(JDIM,KDIM),DISSL(LMAX),
     >          ADXI(JDIM,KDIM),ADET(JDIM,KDIM),
     >          ADRXI(JDIM,KDIM),ADRET(JDIM,KDIM)
      DIMENSION H(JDIM,KDIM,9),A(JDIM,KDIM,3,3),
     >          B(JDIM,KDIM,3,3),C(JDIM,KDIM,3,3),F(JDIM,KDIM,3)
      DIMENSION DAREA(JDIM,KDIM),RR(JDIM,KDIM,LMAX),
     >          DLC(JDIM),DAREAS(JDIM)
      DIMENSION SCR21(JDIM,KDIM),SCR22(JDIM,KDIM),
     >          SCR23(JDIM,KDIM,3),SCR26(JDIM,KDIM,3)
      DIMENSION CAXI(JDIM,KDIM),CAET(JDIM,KDIM),CVEX(JDIM,KDIM),
     >          AFNXI(JDIM,KDIM),AFNET(JDIM,KDIM),BLN(JDIM,KDIM)
      DIMENSION B11(M1D),B21(M1D),B22(M1D),B31(M1D),B32(M1D),
     >          B33(M1D),U12(M1D),U13(M1D),U23(M1D)
C
C-----------------------------------------------------------------------
C
C    Routine to implicitly march grid generation equations one step in
C    the normal direction (L-direction) away from the body surface.
C    Boundary conditions include periodic, float, float with splay,
C    float/fix x, y or z, collapsed edge, reflected symmetry,
C    axis, 2D plane (1 slice) and 2D interior and boundary planes.
C
C-----------------------------------------------------------------------
C
C   --------------------------------------------------------
C    Compute metrics, volumes, dissipation and RHS operator
C   --------------------------------------------------------
      DO 10 K=KA,KB
      DO 10 J=JA,JB
       VOLM(J,K) = VOL(J,K)
 10   CONTINUE
c
C    Compute metrics using grid and volumes at level L-1
      CALL METRAR(JDIM,KDIM,JA,JB,KA,KB,JPER,KPER,JJP,JJR,KKP,KKR,
     >            IBCJA,IBCKA,JPLN1,KPLN1,X,Y,Z,VOLM,
     >            XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,DAREA)
C
C    Compute volumes at level L
      CALL CONVOL(JDIM,KDIM,M1D,LMAX,JA,JB,KA,KB,L,
     >            JPER,KPER,JJP,JJR,KKP,KKR,JAXSA,JAXSB,VZETA,
     >            IAXIS,EXAXIS,VOLRES,IVSPEC,EPSSS,ITSVOL,
     >            RADIS,DTHET,SPHI,DLC,DAREAS,DAREA,VOL,RR)
C
C    Compute arrays used for spatially-variable dissipation
      CALL DISVAR(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >            X,Y,Z,IMETH,SMU2,LTEST,LTRANS,DISSL,I0,
     >            RJMAXM,RKMAXM,ADXI,ADET,ADRXI,ADRET,SCR21,SCR22)
C
C    Compute grid angles functions and modified zeta-derivatives
      IF (L.EQ.2) THEN
       CALL ANGMET2(JDIM,KDIM,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >              JPLN1,KPLN1,VOLM,X,Y,Z,XZ,YZ,ZZ,CAXI,CAET,
     >              AFNXI,AFNET,SCR26,SCR23,H(1,1,1),H(1,1,4))
      ELSE IF (L.GT.2) THEN
       CALL ANGMET(JDIM,KDIM,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >             JPLN1,KPLN1,VOLM,X,Y,Z,XZ,YZ,ZZ,CAXI,CAET,
     >             AFNXI,AFNET,SCR23)
      ENDIF
C
C    Blend zeta metrics near boundaries to be consistent with constant
C    plane or collapsed edge boundaries
      CALL HMETBLN(M1D,JDIM,KDIM,JMAX,KMAX,JA,JB,KA,KB,
     >            IBCJA,IBCJB,IBCKA,IBCKB,ISJA,ISJB,ISKA,ISKB,
     >            JJP,JJR,KKP,KKR,JPLNA,JPLNB,KPLNA,KPLNB,
     >            B11,B21,B22,B31,B32,B33,U12,X,Y,Z,XZ,YZ,ZZ)
C
C    Compute forcing using volumes at L and compute dissipation on RHS
      CALL HRHS(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >         JPLN1,KPLN1,IBCJA,IBCKA,IMETH,SMU2,IAXIS,JAXSA,
     >         JAXSB,CMIN,SCALEL,X,Y,Z,XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,
     >         VOLM,VOL,SR,RR,DISSL,XIDS,ETDS,SCR23,SCR26,
     >         ADRXI,ADRET,CAXI,CAET,AFNXI,AFNET,BLN,CVEX)
C
C   -------------------------------------------
C    Invert LHS operator in eta if not 2D in K
C   -------------------------------------------
C
      IF ( .NOT. K2D ) THEN
C
       CALL FILTRE(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,TIMK,
     >             XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOLM,IMETH,
     >             DISSL,ADRET,CAXI,CAET,AFNET,ETDS,CVEX,H,SR,A,B,C,F)
C
       IF ( KPER .EQ. 0 ) THEN
C
        K1 = KA-1
        K2 = KB+1
C
        IF ( KSYMA .OR. KSYMB )
     >    CALL BCSYMK(JDIM,KDIM,JA,JB,KA,KB,KSYMA,KSYMB,KPLNA,KPLNB,
     >                A,B,C,F)
C
        IF ( KFLTA .OR. KFLTB )
     >    CALL BCEDK(JDIM,KDIM,JA,JB,KA,KB,KFLTA,KFLTB,KPLNA,KPLNB,
     >               EXTKA,EXTKB,A,B,C,F)
C
        CALL V1BTRI(JDIM,KDIM,JA,JB,KA-1,KB+1,KFLAGA,KFLAGB,A,B,C,F,
     >              U12,U13,U23,B11,B21,B31,B22,B32,B33)
C
       ELSE IF ( KPER .EQ. 1 ) THEN
C
        K1 = KA
        K2 = KB
        CALL V1BTRIP(JDIM,KDIM,JA,JB,KA,KB,A,B,C,F,
     >               U12,U13,U23,B11,B21,B31,B22,B32,B33)
C
       ENDIF 
C
       DO 20 N=1,3
       DO 20 K=K1,K2
       DO 20 J=JA,JB
        SR(J,K,N) = F(J,K,N)
 20    CONTINUE
C
      ENDIF
C
       J1 = JA-1
       J2 = JB+1
C
C   ------------------------------------------
C    Invert LHS operator in xi if not 2D in J
C   ------------------------------------------
      IF ( .NOT. J2D ) THEN
C
       CALL HFILTRX(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,TIMJ,
     >             XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,VOLM,IMETH,
     >             DISSL,ADRXI,CAXI,CAET,AFNXI,XIDS,CVEX,H,SR,A,B,C,F)
C
       IF ( JPER .EQ. 0 ) THEN
C
        IF ( JAXSA .OR. JAXSB ) THEN
         IF (IAXIS.GE.1) THEN
          CALL BCAXJ(JDIM,KDIM,JA,JB,KA,KB,JAXSA,JAXSB,EXAXIS,A,B,C,F)
         ELSE IF (IAXIS.EQ.-2) THEN
          IF (JAXSA) THEN
           J1 = JA
           CALL BCAXJA(JDIM,KDIM,LMAX,JA,KA,KB,L,KPER,KSYMA,KSYMB,
     >                 KPLNA,DISSL,RR,X,Y,Z,XJADEL,YJADEL,ZJADEL)
           DO 31 N=1,3
           DO 31 K=KA,KB
            F(JA,K,N) = F(JA,K,N) -A(JA,K,N,1)*XJADEL
     >                 -A(JA,K,N,2)*YJADEL -A(JA,K,N,3)*ZJADEL
 31        CONTINUE
          ENDIF
          IF (JAXSB) THEN
           J2 = JB
           CALL BCAXJBN(JDIM,KDIM,LMAX,JB,KA,KB,L,KPER,KSYMA,KSYMB,
     >                  KPLNB,DISSL,RR,X,Y,Z,XJBDEL,YJBDEL,ZJBDEL)
           DO 32 N=1,3
           DO 32 K=KA,KB
            F(JB,K,N) = F(JB,K,N) -C(JB,K,N,1)*XJBDEL
     >                 -C(JB,K,N,2)*YJBDEL -C(JB,K,N,3)*ZJBDEL
 32        CONTINUE
          ENDIF
         ENDIF
        ENDIF
C
        IF ( JSYMA .OR. JSYMB )
     >    CALL BCSYMJ(JDIM,KDIM,JA,JB,KA,KB,JSYMA,JSYMB,JPLNA,JPLNB,
     >                A,B,C,F)
C
        IF ( JFLTA .OR. JFLTB )
     >    CALL BCEDJ(JDIM,KDIM,JA,JB,KA,KB,JFLTA,JFLTB,JPLNA,JPLNB,
     >               EXTJA,EXTJB,A,B,C,F)
C
        CALL V2BTRI(JDIM,KDIM,J1,J2,KA,KB,JFLAGA,JFLAGB,A,B,C,F,
     >              U12,U13,U23,B11,B21,B31,B22,B32,B33)
C
       ELSE IF ( JPER .EQ. 1 ) THEN
C
        J1 = JA
        J2 = JB
        CALL V2BTRIP(JDIM,KDIM,JA,JB,KA,KB,A,B,C,F,
     >               U12,U13,U23,B11,B21,B31,B22,B32,B33)
C
       ENDIF
C
      ENDIF
C
C    Update X,Y,Z
      IF ( (IBCJA.LE.20) .AND. (IBCKA.LE.20) ) THEN
C
C      Arbitrary interior planes
        DO 40 K=KA,KB
        DO 40 J=J1,J2
         X(J,K) = F(J,K,1) + X(J,K)
         Y(J,K) = F(J,K,2) + Y(J,K)
         Z(J,K) = F(J,K,3) + Z(J,K)
 40     CONTINUE
C
      ENDIF
      IF ( (IBCJA.GE.21) .AND. (IBCJA.LE.23) ) THEN
C
C      Constant interior planes in J
        DO 45 K=K1,K2
        DO 45 J=JA,JB
         X(J,K) = F(J,K,1)*JPLNA(1) + X(J,K)
         Y(J,K) = F(J,K,2)*JPLNA(2) + Y(J,K)
         Z(J,K) = F(J,K,3)*JPLNA(3) + Z(J,K)
 45     CONTINUE
C
      ENDIF
      IF ( (IBCKA.GE.21) .AND. (IBCKA.LE.23) ) THEN
C
C      Constant interior planes in K
        DO 46 K=KA,KB
        DO 46 J=J1,J2
         X(J,K) = F(J,K,1)*KPLNA(1) + X(J,K)
         Y(J,K) = F(J,K,2)*KPLNA(2) + Y(J,K)
         Z(J,K) = F(J,K,3)*KPLNA(3) + Z(J,K)
 46     CONTINUE
C
      ENDIF
C
C
C    Update floating edges at K=KA-1 and K=KB+1
       IF ( (.NOT. J2D) .AND. (.NOT. K2D) ) THEN
        IF (IBCKA.LE.7) THEN
         DO 50 J=JA,JB
          X(J,KA-1) = F(J,KA,1)*KPLNA(1)*(1.-EXTKA) + X(J,KA-1)
          Y(J,KA-1) = F(J,KA,2)*KPLNA(2)*(1.-EXTKA) + Y(J,KA-1)
          Z(J,KA-1) = F(J,KA,3)*KPLNA(3)*(1.-EXTKA) + Z(J,KA-1)
 50      CONTINUE
        ENDIF
        IF (IBCKB.LE.7) THEN
         DO 60 J=JA,JB
          X(J,KB+1) = F(J,KB,1)*KPLNB(1)*(1.-EXTKB) + X(J,KB+1)
          Y(J,KB+1) = F(J,KB,2)*KPLNB(2)*(1.-EXTKB) + Y(J,KB+1)
          Z(J,KB+1) = F(J,KB,3)*KPLNB(3)*(1.-EXTKB) + Z(J,KB+1)
 60      CONTINUE
        ENDIF
       ENDIF
C
C    Update boundary edges in K for 2D option in K
       IF ( K2D ) THEN
         DO 70 J=J1,J2
          X(J,1) = X(J,1) + F(J,2,1)*KPLNA(1)
          Y(J,1) = Y(J,1) + F(J,2,2)*KPLNA(2)
          Z(J,1) = Z(J,1) + F(J,2,3)*KPLNA(3)
          X(J,KMAX) = X(J,KMAX) + F(J,KMAX-1,1)*KPLNB(1)
          Y(J,KMAX) = Y(J,KMAX) + F(J,KMAX-1,2)*KPLNB(2)
          Z(J,KMAX) = Z(J,KMAX) + F(J,KMAX-1,3)*KPLNB(3)
 70      CONTINUE
       ENDIF
c
C    Update boundary edges in J for 2D option in J
       IF ( J2D ) THEN
         DO 80 K=K1,K2
          X(1,K) = X(1,K) + F(2,K,1)*JPLNA(1)
          Y(1,K) = Y(1,K) + F(2,K,2)*JPLNA(2)
          Z(1,K) = Z(1,K) + F(2,K,3)*JPLNA(3)
          X(JMAX,K) = X(JMAX,K) + F(JMAX-1,K,1)*JPLNB(1)
          Y(JMAX,K) = Y(JMAX,K) + F(JMAX-1,K,2)*JPLNB(2)
          Z(JMAX,K) = Z(JMAX,K) + F(JMAX-1,K,3)*JPLNB(3)
 80      CONTINUE
       ENDIF
c
C------------------------------------------------------------------
C
C    Average and smooth axis values
      IF ((JAXSA).AND.(IAXIS.GE.1)) THEN
       CALL AXWAVG(JDIM,KDIM,JMAX,KMAX,J1,KPER,KSYMA,KSYMB,KFLTA,
     >             KFLTB,KPLNA,KPLNB,PLNKAB,KKP,KKR,B11,X,Y,Z)
       IF (IAXIS.EQ.2) THEN
        CALL AXSMOO(JDIM,KDIM,KMAX,J1,1,KPLNA,KPLNB,PLNKAB,X,Y,Z)
       ENDIF
      ENDIF
      IF ((JAXSB).AND.(IAXIS.GE.1)) THEN
       CALL AXWAVG(JDIM,KDIM,JMAX,KMAX,J2,KPER,KSYMA,KSYMB,KFLTA,
     >             KFLTB,KPLNA,KPLNB,PLNKAB,KKP,KKR,B11,X,Y,Z)
       IF (IAXIS.EQ.2) THEN
        CALL AXSMOO(JDIM,KDIM,KMAX,J2,-1,KPLNA,KPLNB,PLNKAB,X,Y,Z)
       ENDIF
      ENDIF
C
C    Update J and K symmetry points
      CALL SETSYM(JDIM,KDIM,X,Y,Z,JA,JB,KA,KB,JMAX,KMAX,
     >            JSYMA,JSYMB,KSYMA,KSYMB,JPLNA,JPLNB,KPLNA,KPLNB)
C
C    Update corners if neither J nor K is periodic
      IF ( (JPER .EQ. 0) .AND. (KPER .EQ. 0) .AND.
     >     (.NOT. J2D) .AND. (.NOT. K2D) ) THEN
       IF ( ((IBCJA.LE.20) .AND. (IBCKA.LE.20)) .OR.
     >      ((IBCJA.GT.20) .AND. (IBCKA.GT.20)) ) THEN
        CALL CORNUP(JDIM,KDIM,LMAX,JA,JB,KA,KB,L,JJP,JJR,KKP,KKR,
     >              JSYMA,JSYMB,JAXSA,JAXSB,KSYMA,KSYMB,
     >              JPLNA,JPLNB,KPLNA,KPLNB,X,Y,Z,XW,YW,ZW)
       ENDIF
      ENDIF
C
C    Update collapsed edges and smooth ends
      IF ( (IBCJA.EQ.7) .OR. (IBCJB.EQ.7) .OR. 
     >     (IBCKA.EQ.7) .OR. (IBCKB.EQ.7) ) THEN
C
       CALL SETEDG(JDIM,KDIM,JMAX,KMAX,IBCJA,IBCJB,IBCKA,IBCKB,X,Y,Z)
C
       CALL SMOEDG(JDIM,KDIM,JMAX,KMAX,LMAX,L,IBCJA,IBCJB,IBCKA,
     >             IBCKB,ITLE,ITTE,LSLE,LSTE,CVEX,X,Y,Z,XW,YW,ZW,
     >             M1D,B11,B22,B33)
C
      ENDIF
C
C    Filter axis values for iaxis=2 only
      IF (IAXIS .EQ. -2) THEN
       CALL AXFILT(JDIM,KDIM,KMAX,LMAX,JA,JB,KA,KB,L,
     >             JAXSA,JAXSB,KPLNA,KPLNB,X,Y,Z)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE STORLP(JDIM,KDIM,JMAX,KMAX,LMAX,J1,J2,K1,K2,L,
     >                  JPER,KPER,X,Y,Z,XW,YW,ZW,PLNKAB)
c*wdh*
c* include "precis.h"
C
      DIMENSION X(JDIM,KDIM),Y(JDIM,KDIM),Z(JDIM,KDIM),PLNKAB(3)
      DIMENSION XW(JDIM,KDIM,LMAX),YW(JDIM,KDIM,LMAX),ZW(JDIM,KDIM,LMAX)
C
C    Reset constant plane coordinate for 2D cases
      IF ((J1.EQ.2).AND.(J2.EQ.2)) THEN
       J = 2
       DO 10 K=1,KMAX
        X(J,K) = X(J,K) + PLNKAB(1)
        Y(J,K) = Y(J,K) + PLNKAB(2)
        Z(J,K) = Z(J,K) + PLNKAB(3)
 10    CONTINUE
      ENDIF
      IF ((K1.EQ.2).AND.(K2.EQ.2)) THEN
       K = 2
       DO 20 J=1,JMAX
        X(J,K) = X(J,K) + PLNKAB(1)
        Y(J,K) = Y(J,K) + PLNKAB(2)
        Z(J,K) = Z(J,K) + PLNKAB(3)
 20    CONTINUE
      ENDIF
C
C    Load periodic plane for output
      IF (JPER.EQ.1) THEN
       DO 30 K=1,KMAX+KPER
        X(JMAX+1,K) = X(1,K)
        Y(JMAX+1,K) = Y(1,K)
        Z(JMAX+1,K) = Z(1,K)
 30    CONTINUE
      ENDIF
      IF (KPER.EQ.1) THEN
       DO 40 J=1,JMAX+JPER
        X(J,KMAX+1) = X(J,1)
        Y(J,KMAX+1) = Y(J,1)
        Z(J,KMAX+1) = Z(J,1)
 40    CONTINUE
      ENDIF
C
C    Store J-K plane of grid at level L
      DO 45 K=K1,K2
      DO 45 J=J1,J2
       XW(J,K,L) = X(J,K)
       YW(J,K,L) = Y(J,K)
       ZW(J,K,L) = Z(J,K)
 45   CONTINUE
C
C    Reset constant plane coordinate to zero for 2D cases
      IF ((J1.EQ.2).AND.(J2.EQ.2)) THEN
       J = 2
       DO 50 K=1,KMAX
        X(J,K) = X(J,K) - PLNKAB(1)
        Y(J,K) = Y(J,K) - PLNKAB(2)
        Z(J,K) = Z(J,K) - PLNKAB(3)
 50    CONTINUE
      ENDIF
      IF ((K1.EQ.2).AND.(K2.EQ.2)) THEN
       K = 2
       DO 60 J=1,JMAX
        X(J,K) = X(J,K) - PLNKAB(1)
        Y(J,K) = Y(J,K) - PLNKAB(2)
        Z(J,K) = Z(J,K) - PLNKAB(3)
 60    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE HTANHS(X0,X1,N,DX0,DX1,DX0A,DX1A,X)
c*wdh*
c* include "precis.h"
C
C
C    Refs: J.F. Thompson, Z.U.A. Warsi, and C.W. Mastin, NUMERICAL GRID
C          GENERATION, North-Holland, New York, pp. 307-308 (1985).
C
C          M. Vinokur, On One-Dimensional Stretching Functions for
C          Finite Difference Calculations, J. of Comp. Phys., Vol. 50,
C          pp. 215-234 (1983).
C
C  WMC (1/95)
C
C    This routine computes a hyperbolic tangent stretching in the range
C    [X0,X1] with either or both the initial and final spacings
C    specified. When an end point spacing is specified, the analytic
C    spacing is the specified spacing. When an end point spacing is
C    not specified, the analytic spacing is computed. In both cases,
C    the end point analytic spacings are returned.
C
C    This routine is derived from a combination of HYPTAN and VCLUST3. 
C    Series expansions from VCLUST3 are used to form initial guesses
C    for Newton iterations used to invert y=sinh(x)/x and y=sin(x)/x.
C    Treatments for B<1, B>1 and B close to 1 are taken from Vinokur's
C    paper.
C
C    Input variables
C
C     X0   =  Value of distance function at end point X0
C     X1   =  Value of distance function at end point X1
C     N    =  Number of grid points including ends
C     DX0  =  Grid spacing at end point X0
C     DX1  =  Grid spacing at end point X1
C
C    There are four acceptable combinations of DX0 and DX1
C
C       DX0       DX1                 Description
C     ==============================================================
C     non-zero  non-zero    Grid spacing fixed at both ends
C     non-zero    zero      Grid spacing fixed at X0, floated at X1
C       zero    non-zero    Grid spacing floated at X0, fixed at X1
C       zero      zero      Uniform grid spacing in interval [X0,X1]
C
C    Returned variables
C
C     DX0A =  Analytic grid spacing at end point X0
C     DX1A =  Analytic grid spacing at end point X1
C     X    =  Array returned with coordinates of points in the
C             interval [X0,X1]
C
C    Parameters
C
C     EPSB =  If B is within EPSB of 1.0, a special series expansion
C             that is to first order in (B-1) is used.
C
C
      PARAMETER ( EPSB=0.001 )
      PARAMETER ( ZERO=0.0, ONE=1.0, TWO=2.0, HALF=0.5 )
C
      DIMENSION X(*)
C
      ONEM  = ONE - EPSB
      ONEP  = ONE + EPSB
      NM    = N - 1
      XLEN  = X1 - X0
      IF (DX0.NE.ZERO) DX0A = DX0
      IF (DX1.NE.ZERO) DX1A = DX1
C
C   Check input.
C
C   X0=X1.
C
      IF (XLEN.EQ.ZERO) THEN
         DO 10 I=1,N
            X(I) = X0
 10      CONTINUE
         GOTO 100
      ENDIF
C
C   Can't have DX0 or DX1 with the wrong sign.
C
      IF ( ( (DX0.NE.ZERO).AND.(DX0*XLEN.LT.ZERO) ) .OR.
     &     ( (DX1.NE.ZERO).AND.(DX1*XLEN.LT.ZERO) ) ) THEN
         WRITE(*,*) 'Error: initial/final spacing cannot be of ',
     &              'the wrong sign.'
         GOTO 100
      ENDIF
C
C   Degenerate cases.
C
      IF      (N.EQ.0) THEN
         GOTO 100
      ELSE IF (N.EQ.1) THEN
         X(1)  = X0
         GOTO 100
      ELSE IF (N.EQ.2) THEN
         X(1)  = X0
         X(2)  = X1
         IF (DX0.NE.ZERO) DX0A   = XLEN
         IF (DX1.NE.ZERO) DX1A   = XLEN
         GOTO 100
      ENDIF
C
C   Scale grid spacings and set end values.
C
         DS    = ONE/NM
         DS0   = DX0/XLEN
         DS1   = DX1/XLEN
         X(1)  = X0
         X(N)  = X1
C
C   Generate stretching function.
C
      IF ( (DX0.EQ.ZERO) .AND. (DX1.EQ.ZERO) ) THEN
C
C      Uniform spacing
C
        DO 20 I=2,N-1
          X(I) = X0 + XLEN*((I-1)*DS)
 20     CONTINUE
        DX0A   = XLEN*DS
        DX1A   = XLEN*DS
C
      ELSE
C
C      Hyperbolic tangent stretching with one/two fixed end points
C
C      Set parameters
C
        IF      ( (DX0.NE.ZERO) .AND. (DX1.EQ.ZERO) ) THEN
C
         A     = ONE
         B     = DS/DS0
         U1    = ONE
         U2    = ONE
         U3    = HALF
         U4    = TWO
C
        ELSE IF ( (DX0.EQ.ZERO) .AND. (DX1.NE.ZERO) ) THEN
C
         A     = ONE
         B     = DS/DS1
         U1    = ONE
         U2    = ZERO
         U3    = HALF
         U4    = -ONE
C
        ELSE IF ( (DX0.NE.ZERO) .AND. (DX1.NE.ZERO) ) THEN
C
         A     = SQRT(DS1/DS0)
         B     = DS/SQRT(DS0*DS1)
         U1    = HALF
         U2    = ONE
         U3    = TWO
         U4    = HALF
C
        ENDIF
C
C      Different functions used depending on value of B
C
        IF      (B .LE. ONEM) THEN
C
          CALL HASINN(B,DELTA)
          HDELTA = HALF*DELTA
          TNH2   = TAN(HDELTA)
          DO 30 I=2,N-1
             XII     = (I-1)*DS
             X(I)    = U1*( U2 + TAN( HDELTA*(XII/U1-U2) )/TNH2 )
 30       CONTINUE
C
        ELSE IF (B .GE. ONEP) THEN
C
          CALL HASINHN(B,DELTA)
          HDELTA = HALF*DELTA
          TNH2   = TANH(HDELTA)
          DO 40 I=2,N-1
             XII     = (I-1)*DS
             X(I)    = U1*( U2 + TANH( HDELTA*(XII/U1-U2) )/TNH2 )
 40       CONTINUE
C
        ELSE
C
          UBM = U3*(ONE-B)
          DO 50 I=2,N-1
             XII     = (I-1)*DS
             X(I)    = XII*( ONE + UBM*(XII-U4)*(XII-ONE) )
 50       CONTINUE
C
        ENDIF
C
C      Rescale coordinates
C
        AM = ONE - A
        DO 60 I=2,N-1
           X(I) = X0 + XLEN*( X(I)/(A + AM*X(I)) )
 60     CONTINUE
C
C      Compute end point analytic spacing
C
        IF ( (B .LE. ONEM) .OR. (B.GE.ONEP) ) THEN
          DXA = XLEN*DS*HDELTA/TNH2
          IF (DX1.EQ.ZERO) DX1A = DXA
          IF (DX0.EQ.ZERO) DX0A = DXA
        ELSE
          IF (DX1.EQ.ZERO) DX1A = XLEN*DS*(ONE + UBM*(ONE-U4))
          IF (DX0.EQ.ZERO) DX0A = XLEN*DS*(ONE + UBM*U4)
        ENDIF
C
      ENDIF
C
C   Check for crossovers.
C
      DO 70 I=2,N
         DS    = (X(I)-X(I-1))/XLEN
         IF (DS.LE.0.) THEN
            WRITE(*,*) 'Warning: zero or negative spacing starting at ',
     &                 'point ',I
            GOTO 90
         ENDIF
 70   CONTINUE
C
 90   CONTINUE
C
 100  CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE HASINHN(B,DELTA)
c*wdh*
c* include "precis.h"
C
      PARAMETER (A1= -0.15,
     &           A2=  0.0573214285714,
     &           A3= -0.024907294878,
     &           A4=  0.0077424460899,
     &           A5= -0.0010794122691)
      PARAMETER (C0= -0.0204176930892,
     &           C1=  0.2490272170591,
     &           C2=  1.9496443322775,
     &           C3= -2.629454725241,
     &           C4=  8.5679591096315)
      PARAMETER (B1=  2.7829681178603,
     &           B2= 35.0539798452776)
      PARAMETER ( ONE=1.0, TWO=2.0, SIX=6.0 )
      PARAMETER ( MITER=20, DELMIN=5.0E-5, TOL=1.0E-6 )
C
C    Use series expansions to get initial guess for delta where 
C    sinh(delta)/delta = B
C    Then use Newton iterations to converge delta. Since B is never
C    below (1+epsb) in the calling sequence, delta should remain
C    well above DELMIN.
C
      IF (B.LE.B1) THEN
         BB    = B - ONE
         DELTA = SQRT(SIX*BB)
     &           *(((((A5*BB+A4)*BB+A3)*BB+A2)*BB+A1)*BB+ONE)
      ELSE
         V     = LOG(B)
         W     = ONE/B - ONE/B2
         DELTA = V + LOG(TWO*V)
     &           *(ONE+ONE/V)+(((C4*W+C3)*W+C2)*W+C1)*W+C0
      ENDIF
C
C    Newton iterations
C
      DO 10 I=1,MITER
       IF (ABS(DELTA).GE.DELMIN) THEN
        SDELTA = SINH(DELTA)
        CDELTA = COSH(DELTA)
        F  = SDELTA/DELTA - B
        FP = (DELTA*CDELTA - SDELTA)/(DELTA*DELTA)
        DD = -F/FP
       ELSE
        WRITE(*,*)'Delta fell below DELMIN.'
        STOP
       ENDIF
       IF (ABS(F).LT.TOL) GO TO 20
       DELTA = DELTA + DD
 10   CONTINUE
C
c      WRITE(*,*)'Exceeded max number of iterations.'
c      WRITE(*,*)'DELTA=',DELTA,'   F=',F,'   DDELTA=',DD
C
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE HASINN(B,DELTA)
c*wdh*
c* include "precis.h"
C
      PARAMETER (A1=  0.15,
     &           A2=  0.0573214285714,
     &           A3=  0.0489742834696,
     &           A4= -0.053337753213,
     &           A5=  0.0758451335824)
      PARAMETER (C3= -2.6449340668482,
     &           C4=  6.7947319658321,
     &           C5=-13.2055008110734,
     &           C6= 11.7260952338351)
      PARAMETER (B1=  0.2693897165164)
      PARAMETER (PI=  3.14159265358981)
      PARAMETER ( ONE=1.0, SIX=6.0 )
      PARAMETER ( MITER=20, DELMIN=5.0E-5, TOL=1.0E-6 )
C
C    Use series expansions to get initial guess for delta where 
C    sin(delta)/delta = B
C    Then use Newton iterations to converge delta. Since B is never
C    above (1-epsb) in the calling sequence, delta should remain
C    above DELMIN.
C
      IF (B.LE.B1) THEN
         DELTA = PI*((((((C6*B+C5)*B+C4)*B+C3)*B+ONE)*B-ONE)*B+ONE)
      ELSE
         BB    = ONE - B
         DELTA = SQRT(SIX*BB)
     &         *(((((A5*BB+A4)*BB+A3)*BB+A2)*BB+A1)*BB+ONE)
      ENDIF
C
C    Newton iterations
C
      DO 10 I=1,MITER
       IF (ABS(DELTA).GE.DELMIN) THEN
        SDELTA = SIN(DELTA)
        CDELTA = COS(DELTA)
        F  = SDELTA/DELTA - B
        FP = (DELTA*CDELTA - SDELTA)/(DELTA*DELTA)
        DD = -F/FP
       ELSE
        WRITE(*,*)'Delta fell below DELMIN.'
        STOP
       ENDIF
       IF (ABS(F).LT.TOL) GO TO 20
       DELTA = DELTA + DD
 10   CONTINUE
C
c      WRITE(*,*)'Exceeded max number of iterations.'
c      WRITE(*,*)'DELTA=',DELTA,'   F=',F,'   DDELTA=',DD
C
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE UCONVOL(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >                   DELXI,DELET,DELVOL,EXI,EET)
c*wdh*
c* include "precis.h"
C-----------------------------------------------------------------------
C     ROUTINE TO FORM CONTROL VOLUMES
C
C     VOLUMES FORMED BY MULTIPLING CELL BASE AREA VIA A USER SPECIFIED
C     ARC LENGTH. VOLUMES ARE THEN SMOOTHED ALONG THE SURFACE.
C-----------------------------------------------------------------------
      DIMENSION LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM)
      DIMENSION DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),D1(2,2)
      DIMENSION EXI(IDIM,3), EET(IDIM,3)
C
      VAREA = 0.
      DO 10 I = IS,IE
C
C                            COMPUTE DELTA XI AND DELTA ETA INCREMENTS
      I11 = LINK(I,1)
      I21 = LINK(I,2)
      I12 = LINK(I,3)
      I22 = LINK(I,4)
c      DELXI(1 ) = ( XU(I11) - XU(I) )*EXI(I,1)
c     &          + ( YU(I11) - YU(I) )*EXI(I,2)
c     &          + ( ZU(I11) - ZU(I) )*EXI(I,3)
c      DELXI(2 ) = ( XU(I21) - XU(I) )*EXI(I,1)
c     &          + ( YU(I21) - YU(I) )*EXI(I,2)
c     &          + ( ZU(I21) - ZU(I) )*EXI(I,3)
c      DELET(1 ) = ( XU(I12) - XU(I) )*EET(I,1)
c     &          + ( YU(I12) - YU(I) )*EET(I,2)
c     &          + ( ZU(I12) - ZU(I) )*EET(I,3)
c      DELET(2 ) = ( XU(I22) - XU(I) )*EET(I,1)
c     &          + ( YU(I22) - YU(I) )*EET(I,2)
c     &          + ( ZU(I22) - ZU(I) )*EET(I,3)
C                            COMPUTE DIFFERENCE COEFFICIENTS FOR XI AND ETA
c      D1(1,1) = 1./( DELXI(1) - DELXI(2) )
      d1(1,1) = .5
      D1(2,1) = - D1(1,1)
c      D1(1,2) = 1./( DELET(1) - DELET(2) )
      d1(1,2) = .5
      D1(2,2) = - D1(1,2)
C
      XXI = D1(1,1)*XU(I11) + D1(2,1)*XU(I21)
      YXI = D1(1,1)*YU(I11) + D1(2,1)*YU(I21)
      ZXI = D1(1,1)*ZU(I11) + D1(2,1)*ZU(I21)
      XET = D1(1,2)*XU(I12) + D1(2,2)*XU(I22)
      YET = D1(1,2)*YU(I12) + D1(2,2)*YU(I22)
      ZET = D1(1,2)*ZU(I12) + D1(2,2)*ZU(I22)
C
      XZE = YXI*ZET - YET*ZXI
      YZE = XET*ZXI - XXI*ZET
      ZZE = XXI*YET - XET*YXI
C                                        CELL LOWER BASE AREA
      DETC= XZE**2 + YZE**2 + ZZE**2
      SQDETC = SQRT(DETC)
C                                        CELL VOLUME
      DELVOL(I) = DZETA*SQDETC 
C         CORRECTION OF AXIS CELL VOLUME (UNIT CIRCLE TO UNIT SQUARE RATIO)
      DELVOL(I) = .785 * DELVOL(I)
   10 CONTINUE
C
C
C                               SMOOTH VOLUMES ALONG SURFACE
C      VSMU = 0.25
C      DO 55 NNN = 1,ITSVOL
CC
C      DO 50 I = IS,IE
C      I11 = LINK(I,1)
C      I21 = LINK(I,2)
C      I12 = LINK(I,3)
C      I22 = LINK(I,4)
C      U(I) = .25*( DELVOL(I11) + DELVOL(I21) +DELVOL(I12) +DELVOL(I22))
C   50 CONTINUE
CC
C      DO 60 I =IS,IE
C      DELVOL(I) = DELVOL(I) + VSMU*(U(I) - DELVOL(I))
C   60 CONTINUE
C   55 CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE UEVECS(IDIM,LNKDIM,IS,IE,LINK,XU,YU,ZU,EXI,EET)
c*wdh*
c* include "precis.h"
C-----------------------------------------------------------------------
C   ROUTINE TO FORM THE LOCAL UNIT VECTORS IN XI AND IN ETA AT EACH POINT
C-----------------------------------------------------------------------
      DIMENSION LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM)
      DIMENSION EXI(IDIM,3), EET(IDIM,3)
C
      DO 10 I = IS,IE
C
      I11 = LINK(I,1)
      I21 = LINK(I,2)
      DX = XU(I11) - XU(I21) 
      DY = YU(I11) - YU(I21) 
      DZ = ZU(I11) - ZU(I21) 
      ABSE = SQRT(DX*DX + DY*DY + DZ*DZ)
      EXI(I,1) = DX/ABSE
      EXI(I,2) = DY/ABSE
      EXI(I,3) = DZ/ABSE
C
      I12 = LINK(I,3)
      I22 = LINK(I,4)
      DX = XU(I12) - XU(I22) 
      DY = YU(I12) - YU(I22) 
      DZ = ZU(I12) - ZU(I22) 
      ABSE = SQRT(DX*DX + DY*DY + DZ*DZ)
      EET(I,1) = DX/ABSE
      EET(I,2) = DY/ABSE
      EET(I,3) = DZ/ABSE
C
   10 CONTINUE
      RETURN 
      END
C***********************************************************************
      SUBROUTINE UHYG4L(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >           DELXI,DELET,DELVOL,EXI,EET,USMU,XS,YS,ZS,XP,YP,ZP)
c*wdh*
c* include "precis.h"
C
      DIMENSION LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM)
      DIMENSION DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),D1(2,2)
      DIMENSION EXI(IDIM,3), EET(IDIM,3)
      DIMENSION XP(IDIM), YP(IDIM), ZP(IDIM)
      DIMENSION XS(IDIM), YS(IDIM), ZS(IDIM)
C
C-----------------------------------------------------------------------
C   UHYG4   ... 4 LINK Unstructured Grid 3D HYPERBOLIC PDE GRID SOLVER
C           THIS VERSION INTENDED TO BE CALLED FROM OTHER CODES
C           IT ALWAYS USES 4 LINKS, 2 IN XI AND 2 IN ETA 
C
C   REQUIRED INPUTS: DZETA, IS, IE, LINKS, AND X,Y,Z
C   DZETA IS USUAL STRETCHED ZETA DELTA INCREMENT
C   IS AND IE ARE STARTING AND ENDING INDICES 
C   LINKS LOADED INTO LINK(IDIM,4), 
C   X,Y,Z LOADED INTO XU(IDIM), YU(IDIM), ZU(IDIM)
C   NOTE: CODE CAN LITERALLY TREAT A SINGLE POINT AT A TIME, OR A SMALL 
C         PART OF ANOTHER GRID 
C   BUT!  XU,YU,ZU MUST CONTAIN ALL NECESSARY FRINGE DATA NEEDED BY LINK(I,4) 
C-----------------------------------------------------------------------
C     CODE REQUIRES THAT FIRST DERIVATIVE COEFFICIENTS IN XI AND ETA
C             DETERMINED WITH THE CENTERED I WEIGHT NOT USED.
C            LINK = 1  MUST CORRESPOND TO UNIT VECTOR IN XI (JP) (1,1)
C            LINK = 2   MUST BE OTHER XI (JR)                    (2,1)
C            LINK = 3  MUST CORRESPOND TO UNIT VECTOR IN ETA     (1,2)
C            LINK = 4   MUST BE OTHER ETA (KR)                   (2,2)
C-----------------------------------------------------------------------
C
c      CALL UEVECS(IDIM,LNKDIM,IS,IE,LINK,XU,YU,ZU,EXI,EET)
      CALL UCONVOL(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >             DELXI,DELET,DELVOL,EXI,EET)
      CALL USTEP(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >           DELXI,DELET,DELVOL,EXI,EET,USMU,XS,YS,ZS,XP,YP,ZP)
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE URHS(IDIM,LNKDIM,DZETA,I,LINK,XU,YU,ZU,DELXI,DELET,
     >                DELVOL,EXI,EET,RESIDX,RESIDY,RESIDZ,USMU,XP,YP,ZP)
c*wdh*
c* include "precis.h"
C-----------------------------------------------------------------------
C     ROUTINE ASSUMES THE PDE IS FIRST MULTIPLIED BY C INVERSE 
C     XP,YP,ZP ARE PREDICTED VALUES OF X,Y,Z... INITIALLY XP = X ETC.
C     CENTERED FIRST DIFFERENCES USED
C-----------------------------------------------------------------------
      DIMENSION LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM)
      DIMENSION DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),D1(2,2)
      DIMENSION EXI(IDIM,3), EET(IDIM,3)
      DIMENSION XP(IDIM), YP(IDIM), ZP(IDIM)
      DIMENSION CIA(3,3), CIB(3,3), CI(3,3), AA(3,3), BB(3,3) 
C
      NMNBR = 4
C 
C                             FORM METRICS XXI ETC.
      I11 = LINK(I,1)
      I21 = LINK(I,2)
      I12 = LINK(I,3)
      I22 = LINK(I,4)
c      DELXI(1 ) = ( XU(I11) - XU(I) )*EXI(I,1)
c     &          + ( YU(I11) - YU(I) )*EXI(I,2)
c     &          + ( ZU(I11) - ZU(I) )*EXI(I,3)
c      DELXI(2 ) = ( XU(I21) - XU(I) )*EXI(I,1)
c     &          + ( YU(I21) - YU(I) )*EXI(I,2)
c     &          + ( ZU(I21) - ZU(I) )*EXI(I,3)
c      DELET(1 ) = ( XU(I12) - XU(I) )*EET(I,1)
c     &          + ( YU(I12) - YU(I) )*EET(I,2)
c     &          + ( ZU(I12) - ZU(I) )*EET(I,3)
c      DELET(2 ) = ( XU(I22) - XU(I) )*EET(I,1)
c     &          + ( YU(I22) - YU(I) )*EET(I,2)
c     &          + ( ZU(I22) - ZU(I) )*EET(I,3)
C                            COMPUTE DIFFERENCE COEFFICIENTS FOR XI AND ETA
c      D1(1,1) = 1./( DELXI(1) - DELXI(2) )
      d1(1,1) = .5
      D1(2,1) = - D1(1,1)
c      D1(1,2) = 1./( DELET(1) - DELET(2) )
      d1(1,2) = .5
      D1(2,2) = - D1(1,2)
C
      XXI = D1(1,1)*XU(I11) + D1(2,1)*XU(I21)
      YXI = D1(1,1)*YU(I11) + D1(2,1)*YU(I21)
      ZXI = D1(1,1)*ZU(I11) + D1(2,1)*ZU(I21)
      XET = D1(1,2)*XU(I12) + D1(2,2)*XU(I22)
      YET = D1(1,2)*YU(I12) + D1(2,2)*YU(I22)
      ZET = D1(1,2)*ZU(I12) + D1(2,2)*ZU(I22)
C      XPXI = D1(1,1)*XP(I11) + D1(2,1)*XP(I21)
C      YPXI = D1(1,1)*YP(I11) + D1(2,1)*YP(I21)
C      ZPXI = D1(1,1)*ZP(I11) + D1(2,1)*ZP(I21)
C      XPET = D1(1,2)*XP(I12) + D1(2,2)*XP(I22)
C      YPET = D1(1,2)*YP(I12) + D1(2,2)*YP(I22)
C      ZPET = D1(1,2)*ZP(I12) + D1(2,2)*ZP(I22)
      XPXI = XXI
      YPXI = YXI
      ZPXI = ZXI
      XPET = XET
      YPET = YET
      ZPET = ZET
C
      XZE = YXI*ZET - YET*ZXI
      YZE = XET*ZXI - XXI*ZET
      ZZE = XXI*YET - XET*YXI
      DETC= XZE**2 + YZE**2 + ZZE**2
      DVOL = DELVOL(I)/DETC
      XZE = DVOL*XZE
      YZE = DVOL*YZE
      ZZE = DVOL*ZZE
C
C                          FORM A AND B COEFF. MATRICES
      AA(1,1) = XZE
      AA(1,2) = YZE
      AA(1,3) = ZZE
      AA(2,1) = 0.
      AA(2,2) = 0.
      AA(2,3) = 0.
      AA(3,1) = YET*ZZE - YZE*ZET
      AA(3,2) = XZE*ZET - XET*ZZE
      AA(3,3) = XET*YZE - XZE*YET
      BB(1,1) = 0.
      BB(1,2) = 0.
      BB(1,3) = 0.
      BB(2,1) = XZE
      BB(2,2) = YZE
      BB(2,3) = ZZE
      BB(3,1) = YZE*ZXI - YXI*ZZE
      BB(3,2) = XXI*ZZE - XZE*ZXI
      BB(3,3) = XZE*YXI - XXI*YZE
C
C                               FORM C INVERSE COEFF MATRIX
      DJ = 1./DELVOL(I)
      Z1 = XZE*DJ
      Z2 = YZE*DJ
      Z3 = ZZE*DJ
      CI(1,1) =  YET*Z3 - ZET*Z2
      CI(1,2) = -( YXI*Z3 - ZXI*Z2)
      CI(1,3) = Z1
      CI(2,1) = -( XET*Z3 - ZET*Z1)
      CI(2,2) = XXI*Z3 - ZXI*Z1
      CI(2,3) = Z2
      CI(3,1) = XET*Z2 - YET*Z1
      CI(3,2) = -( XXI*Z2 - YXI*Z1)
      CI(3,3) = Z3
C
C                      FORM C INVERSE TIMES A AND C INVERSE TIMES B
C                      FORM SIMPLE NORMS OF ABOVE FOR RELAXATION SCALING
      DO 11 N=1,3
      DO 12 M=1,3
      SUMA = 0.
      SUMB = 0.
      DO 13 L=1,3
      SUMA = CI(N,L)*AA(L,M) + SUMA
      SUMB = CI(N,L)*BB(L,M) + SUMB
   13 CONTINUE
      CIA(N,M) = SUMA
      CIB(N,M) = SUMB
   12 CONTINUE
   11 CONTINUE
C
C                                          FORM RESIDUAL, HYPERBOLIC PART
      RESIDX = CI(1,3)*3.*DELVOL(I)
     &       -CIA(1,1)*XPXI -CIA(1,2)*YPXI -CIA(1,3)*ZPXI
     &       -CIB(1,1)*XPET -CIB(1,2)*YPET -CIB(1,3)*ZPET
      RESIDY = CI(2,3)*3.*DELVOL(I)
     &       -CIA(2,1)*XPXI -CIA(2,2)*YPXI -CIA(2,3)*ZPXI
     &       -CIB(2,1)*XPET -CIB(2,2)*YPET -CIB(2,3)*ZPET
      RESIDZ = CI(3,3)*3.*DELVOL(I)
     &       -CIA(3,1)*XPXI -CIA(3,2)*YPXI -CIA(3,3)*ZPXI
     &       -CIB(3,1)*XPET -CIB(3,2)*YPET -CIB(3,3)*ZPET
C
C                                         ADD SMOOTHING
      XXIXI = XU(I11) -2.*XU(I) + XU(I21)
      YXIXI = YU(I11) -2.*YU(I) + YU(I21)
      ZXIXI = ZU(I11) -2.*ZU(I) + ZU(I21)
      XETET = XU(I12) -2.*XU(I) + XU(I22)
      YETET = YU(I12) -2.*YU(I) + YU(I22)
      ZETET = ZU(I12) -2.*ZU(I) + ZU(I22)
      DX = XXI**2 + YXI**2 +ZXI**2
      DE = XET**2 + YET**2 +ZET**2
      DZ = DETC
      CXI = SQRT(DZ/DX)
      CET = SQRT(DZ/DE)   
      RESIDX = RESIDX + USMU*(CXI*XXIXI +CET*XETET)
      RESIDY = RESIDY + USMU*(CXI*YXIXI +CET*YETET)
      RESIDZ = RESIDZ + USMU*(CXI*ZXIXI +CET*ZETET)
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE USTEP(IDIM,LNKDIM,DZETA,IS,IE,LINK,XU,YU,ZU,
     >                DELXI,DELET,DELVOL,EXI,EET,USMU,XS,YS,ZS,XP,YP,ZP)
c*wdh*
c* include "precis.h"
C
C-----------------------------------------------------------------------
C    COMPUTE GRID BY ADVANCING OUT ONE SHELL (STEP) AT A TIME IN ZETA
C    USING EULER PREDICTOR FOLLOWED BY TWO NEW LEVEL EULLER CORRECTORS
C-----------------------------------------------------------------------
      DIMENSION LINK(IDIM,LNKDIM), XU(IDIM), YU(IDIM), ZU(IDIM)
      DIMENSION DELXI(LNKDIM),DELET(LNKDIM),DELVOL(IDIM),D1(2,2)
      DIMENSION EXI(IDIM,3), EET(IDIM,3)
      DIMENSION XP(IDIM), YP(IDIM), ZP(IDIM)
      DIMENSION XS(IDIM), YS(IDIM), ZS(IDIM)
C
      RESIDX = 0.
      RESIDY = 0.
      RESIDZ = 0.
C
C
      NNNP= 1
c
c                           caution corrector stage temporarilly off
c                               in general need in ... unstable otherwise
c
      DO 10 I = IS,IE
      CALL URHS(IDIM,LNKDIM,DZETA,I,LINK,XU,YU,ZU,DELXI,DELET,
     >          DELVOL,EXI,EET,RESIDX,RESIDY,RESIDZ,USMU,XP,YP,ZP)
      XS(I) = XU(I) + RESIDX
      YS(I) = YU(I) + RESIDY
      ZS(I) = ZU(I) + RESIDZ
   10 CONTINUE
      DO 11 I = IS,IE
      XU(I) = XS(I) 
      YU(I) = YS(I) 
      ZU(I) = ZS(I) 
   11 CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE V1BTRI(JD,KD,JS,JE,KS,KE,IFLGS,IFLGE,A,B,C,F,
     >                  U12,U13,U23,B11,B21,B31,B22,B32,B33)
c*wdh*
c* include "precis.h"
C
C-----------------------------------------------------------------------
C
C     This is a special version of a self contained vectorized 3x3 block
C     tridiagonal routine that contains a non-zero element in the third
C     column of the first row and in the third last column of the last
C     row of the LHS matrix. These extra elements are stored in A(KS)
C     and C(KE) respectively. If IFLGS=0 then the extra element at KS
C     is assumed to be zero and no extra operations are performed.
C     Similarly for IFLGE at KE.
C
C     (A,B,C)F = F, F and C are overloaded, solution in F
C
C     Vectorized in J where J is the first index
C
C     This version also works for single block, KE = KS,
C     as well as KE = KS+1.
C
C     Operation counts are reduced by making use of the following
C     special structures of A, B and C in the first and last rows:
C     A and C are diagonal matrices and B is the identity matrix.
C
C-----------------------------------------------------------------------
C     WARNING: DO NOT use this routine if full block matrices are
C              present at K=KS and/or K=KE.
C-----------------------------------------------------------------------
C
      DIMENSION A(JD,KD,3,3), B(JD,KD,3,3), C(JD,KD,3,3), F(JD,KD,3)
      DIMENSION U12(JD),U13(JD),U23(JD),
     >          B11(JD),B21(JD),B31(JD),B22(JD),B32(JD),B33(JD)
C
C=======================================================================
C       PART 1.  FORWARD BLOCK SWEEP
C=======================================================================
C
C    --------
      K = KS
C    --------
C
C     Replace with more code if full matrices are present at KS
       IF (IFLGS.NE.0) THEN
        DO 5 M = 1,3
        DO 5 N = 1,3
        DO 5 J = JS,JE
          C(J,K+1,M,N) = ((C(J,K+1,M,N) -A(J,K+1,M,1)*A(J,K,1,N))
     >        -A(J,K+1,M,2)*A(J,K,2,N)) -A(J,K+1,M,3)*A(J,K,3,N)
 5      CONTINUE
       ENDIF
C
C    ----------------
C     K = KS+1 to KE
C    ----------------
C
      DO 20 K = KS+1,KE
C
        IF ((IFLGE.NE.0).AND.(K.EQ.KE)) THEN
         DO 11 M = 1,3
         DO 11 N = 1,3
         DO 11 J = JS,JE
C         Replace with these lines if C(KE) is a full block matrix
C          A(J,K,M,N)  = ((A(J,K,M,N) -C(J,K,M,1)*C(J,K-2,1,N))
C     >           -C(J,K,M,2)*C(J,K-2,2,N)) -C(J,K,M,3)*C(J,K-2,3,N)
          A(J,K,M,N)  = A(J,K,M,N) -C(J,K,M,M)*C(J,K-2,M,N)
 11      CONTINUE
         DO 12 M = 1,3
         DO 12 J = JS,JE
          F(J,K,M) = ((F(J,K,M) -C(J,K,M,1)*F(J,K-2,1))
     >             -C(J,K,M,2)*F(J,K-2,2)) -C(J,K,M,3)*F(J,K-2,3)
 12      CONTINUE
        ENDIF
C
C-----------------------------------------------------------------------
C       Step 1.  Construct L(I) in B
C-----------------------------------------------------------------------
C
         DO 13 M = 1,3
         DO 13 N = 1,3
         DO 13 J = JS,JE
          B(J,K,M,N)  = ((B(J,K,M,N) -A(J,K,M,1)*C(J,K-1,1,N))
     >           -A(J,K,M,2)*C(J,K-1,2,N)) -A(J,K,M,3)*C(J,K-1,3,N)
 13      CONTINUE
C
C-----------------------------------------------------------------------
C       Step 2.  Decompose B(I) into L and U
C-----------------------------------------------------------------------
C
         DO 14 J = JS,JE
          B11(J)     = 1./B(J,K,1,1)
          U12(J)     = B(J,K,1,2)*B11(J)
          U13(J)     = B(J,K,1,3)*B11(J)
          B21(J)     = B(J,K,2,1)
          B22(J)     = 1./( ((B(J,K,2,2)) -B21(J)*U12(J)) )
          U23(J)     = ( ((B(J,K,2,3)) -B21(J)*U13(J)) )*B22(J)
          B31(J)     = B(J,K,3,1)
          B32(J)     = (B(J,K,3,2)) -B31(J)*U12(J)
          B33(J)     = 1./( (((B(J,K,3,3)) -B31(J)*U13(J))
     >                     -B32(J)*U23(J)))
 14      CONTINUE
C
C-----------------------------------------------------------------------
C       Step 3.  Solve for intermediate vector
C-----------------------------------------------------------------------
C
C        A. Construct RHS
C
         DO 15 M = 1,3
         DO 15 J = JS,JE
          F(J,K,M) = ((F(J,K,M) -A(J,K,M,1)*F(J,K-1,1))
     >             -A(J,K,M,2)*F(J,K-1,2)) -A(J,K,M,3)*F(J,K-1,3)
 15      CONTINUE
C
C        B. Intermediate vector
C
         DO 16 J = JS,JE
C         Forward substitution
           D1         = F(J,K,1)*B11(J)
           D2         = ( (F(J,K,2)) -B21(J)*D1 )*B22(J)
           D3         = ( ((F(J,K,3)) -B31(J)*D1) -B32(J)*D2 )*B33(J)
C         Backward substitution
           F(J,K,3)  = D3
           F(J,K,2)  = D2 -U23(J)*F(J,K,3)
           F(J,K,1)  = (D1 -U12(J)*F(J,K,2)) -U13(J)*F(J,K,3)
 16      CONTINUE
C
C-----------------------------------------------------------------------
C       Step 4.  Construct U(I) = L(I)**(-1)*C(I+1)
C                by columns and store in C
C-----------------------------------------------------------------------
C
        IF (K.NE.KE) THEN
         DO 17 N = 1,3
         DO 17 J = JS,JE
C         Forward substitution
           C1         = C(J,K,1,N)*B11(J)
           C2         = ((C(J,K,2,N)) -B21(J)*C1)*B22(J)
           C3         = (((C(J,K,3,N)) -B31(J)*C1) -B32(J)*C2)*B33(J)
C         Backward substitution
           C(J,K,3,N)  = C3
           C(J,K,2,N)  = C2 -U23(J)*C(J,K,3,N)
           C(J,K,1,N)  = (C1 -U12(J)*C(J,K,2,N)) -U13(J)*C(J,K,3,N)
 17      CONTINUE
        ENDIF
C
 20    CONTINUE
C
C=======================================================================
C       PART 2.  BACKWARD BLOCK SWEEP
C=======================================================================
C
      IF (KE .EQ. KS) RETURN
C
       DO 21 K = KE-1,KS,-1
       DO 21 M = 1,3
       DO 21 J = JS,JE
        F(J,K,M) = ((F(J,K,M) -C(J,K,M,1)*F(J,K+1,1))
     >               -C(J,K,M,2)*F(J,K+1,2)) -C(J,K,M,3)*F(J,K+1,3)
 21    CONTINUE
C
      IF (IFLGS.NE.0) THEN
       K = KS
       DO 22 M = 1,3
       DO 22 J = JS,JE
C        Replace with these lines if A(KS) is a full block matrix
C        F(J,K,M) = ((F(J,K,M) -A(J,KS,M,1)*F(J,K+2,1))
C     >               -A(J,KS,M,2)*F(J,K+2,2)) -A(J,KS,M,3)*F(J,K+2,3)
        F(J,K,M) = F(J,K,M) -A(J,KS,M,M)*F(J,K+2,M)
 22    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE V2BTRI(JD,KD,JS,JE,KS,KE,IFLGS,IFLGE,A,B,C,F,
     >                  U12,U13,U23,B11,B21,B31,B22,B32,B33)
c*wdh*
c* include "precis.h"
C
C-----------------------------------------------------------------------
C
C     This is a special version of a self contained vectorized 3x3 block
C     tridiagonal routine that contains a non-zero element in the third
C     column of the first row and in the third last column of the last
C     row of the LHS matrix. These extra elements are stored in A(JS)
C     and C(JE) respectively. If IFLGS=0 then the extra element at JS
C     is assumed to be zero and no extra operations are performed.
C     Similarly for IFLGE at JE.
C
C     (A,B,C)F = F, F and C are overloaded, solution in F
C
C     Vectorized in K where K is the second index
C
C     This version also works for single block, JE = JS,
C     as well as JE = JS+1.
C
C     Operation counts are reduced by making use of the following
C     special structures of A, B and C in the first and last rows:
C     A and C are diagonal matrices and B is the identity matrix.
C
C-----------------------------------------------------------------------
C     WARNING: DO NOT use this routine if full block matrices are
C              present at J=JS and/or J=JE.
C-----------------------------------------------------------------------
C
      DIMENSION A(JD,KD,3,3), B(JD,KD,3,3), C(JD,KD,3,3), F(JD,KD,3)
      DIMENSION U12(KD),U13(KD),U23(KD),
     >          B11(KD),B21(KD),B31(KD),B22(KD),B32(KD),B33(KD)
C
C=======================================================================
C       PART 1.  FORWARD BLOCK SWEEP
C=======================================================================
C
C    --------
      J = JS
C    --------
C
C     Replace with more code if full matrices are present at JS
       IF (IFLGS.NE.0) THEN
        DO 5 M = 1,3
        DO 5 N = 1,3
        DO 5 K = KS,KE
          C(J+1,K,M,N) = ((C(J+1,K,M,N) -A(J+1,K,M,1)*A(J,K,1,N))
     >        -A(J+1,K,M,2)*A(J,K,2,N)) -A(J+1,K,M,3)*A(J,K,3,N)
 5      CONTINUE
       ENDIF
C
C    ----------------
C     J = JS+1 to JE
C    ----------------
C
      DO 20 J = JS+1,JE
C
        IF ((IFLGE.NE.0).AND.(J.EQ.JE)) THEN
         DO 11 M = 1,3
         DO 11 N = 1,3
         DO 11 K = KS,KE
C         Replace with these lines if C(JE) is a full block matrix
C          A(J,K,M,N)  = ((A(J,K,M,N) -C(J,K,M,1)*C(J-2,K,1,N))
C     >           -C(J,K,M,2)*C(J-2,K,2,N)) -C(J,K,M,3)*C(J-2,K,3,N)
          A(J,K,M,N)  = A(J,K,M,N) -C(J,K,M,M)*C(J-2,K,M,N)
 11      CONTINUE
         DO 12 M = 1,3
         DO 12 K = KS,KE
          F(J,K,M) = ((F(J,K,M) -C(J,K,M,1)*F(J-2,K,1))
     >             -C(J,K,M,2)*F(J-2,K,2)) -C(J,K,M,3)*F(J-2,K,3)
 12      CONTINUE
        ENDIF
C
C-----------------------------------------------------------------------
C       Step 1.  Construct L(I) in B
C-----------------------------------------------------------------------
C
         DO 13 M = 1,3
         DO 13 N = 1,3
         DO 13 K = KS,KE
          B(J,K,M,N)  = ((B(J,K,M,N) -A(J,K,M,1)*C(J-1,K,1,N))
     >           -A(J,K,M,2)*C(J-1,K,2,N)) -A(J,K,M,3)*C(J-1,K,3,N)
 13      CONTINUE
C
C-----------------------------------------------------------------------
C       Step 2.  Decompose B(I) into L and U
C-----------------------------------------------------------------------
C
         DO 14 K = KS,KE
          B11(K)     = 1./B(J,K,1,1)
          U12(K)     = B(J,K,1,2)*B11(K)
          U13(K)     = B(J,K,1,3)*B11(K)
          B21(K)     = B(J,K,2,1)
          B22(K)     = 1./( ((B(J,K,2,2)) -B21(K)*U12(K)) )
          U23(K)     = ( ((B(J,K,2,3)) -B21(K)*U13(K)) )*B22(K)
          B31(K)     = B(J,K,3,1)
          B32(K)     = (B(J,K,3,2)) -B31(K)*U12(K)
          B33(K)     = 1./( (((B(J,K,3,3)) -B31(K)*U13(K))
     >                     -B32(K)*U23(K)))
 14      CONTINUE
C
C-----------------------------------------------------------------------
C       Step 3.  Solve for intermediate vector
C-----------------------------------------------------------------------
C
C        A. Construct RHS
C
         DO 15 M = 1,3
         DO 15 K = KS,KE
          F(J,K,M) = ((F(J,K,M) -A(J,K,M,1)*F(J-1,K,1))
     >             -A(J,K,M,2)*F(J-1,K,2)) -A(J,K,M,3)*F(J-1,K,3)
 15      CONTINUE
C
C        B. Intermediate vector
C
         DO 16 K = KS,KE
C         Forward substitution
           D1         = F(J,K,1)*B11(K)
           D2         = ( (F(J,K,2)) -B21(K)*D1 )*B22(K)
           D3         = ( ((F(J,K,3)) -B31(K)*D1) -B32(K)*D2 )*B33(K)
C         Backward substitution
           F(J,K,3)  = D3
           F(J,K,2)  = D2 -U23(K)*F(J,K,3)
           F(J,K,1)  = (D1 -U12(K)*F(J,K,2)) -U13(K)*F(J,K,3)
 16      CONTINUE
C
C-----------------------------------------------------------------------
C       Step 4.  Construct U(I) = L(I)**(-1)*C(I+1)
C                by columns and store in C
C-----------------------------------------------------------------------
C
        IF (J.NE.JE) THEN
         DO 17 N = 1,3
         DO 17 K = KS,KE
C         Forward substitution
           C1         = C(J,K,1,N)*B11(K)
           C2         = ((C(J,K,2,N)) -B21(K)*C1)*B22(K)
           C3         = (((C(J,K,3,N)) -B31(K)*C1) -B32(K)*C2)*B33(K)
C         Backward substitution
           C(J,K,3,N)  = C3
           C(J,K,2,N)  = C2 -U23(K)*C(J,K,3,N)
           C(J,K,1,N)  = (C1 -U12(K)*C(J,K,2,N)) -U13(K)*C(J,K,3,N)
 17      CONTINUE
        ENDIF
C
 20    CONTINUE
C
C=======================================================================
C       PART 2.  BACKWARD BLOCK SWEEP
C=======================================================================
C
      IF (JE .EQ. JS) RETURN
C
       DO 21 J = JE-1,JS,-1
       DO 21 M = 1,3
       DO 21 K = KS,KE
        F(J,K,M) = ((F(J,K,M) -C(J,K,M,1)*F(J+1,K,1))
     >               -C(J,K,M,2)*F(J+1,K,2)) -C(J,K,M,3)*F(J+1,K,3)
 21    CONTINUE
C
      IF (IFLGS.NE.0) THEN
       J = JS
       DO 22 M = 1,3
       DO 22 K = KS,KE
C        Replace with these lines if A(JS) is a full block matrix
C        F(J,K,M) = ((F(J,K,M) -A(JS,K,M,1)*F(J+2,K,1))
C     >               -A(JS,K,M,2)*F(J+2,K,2)) -A(JS,K,M,3)*F(J+2,K,3)
        F(J,K,M) = F(J,K,M) -A(JS,K,M,M)*F(J+2,K,M)
 22    CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE V1BTRIP( JD,KD,JS,JE,KS,KE,A,B,C,F,
     &                    U12,U13,U23,B11,B21,B31,B22,B32,B33 )
c*wdh*
c* include "precis.h"
C
C-----------------------------------------------------------------------
C
C
C     (A,B,C)F = F,  A,B AND F ARE OVERLOADED, SOLUTION IN F
C
C     VECTORIZED ON J, J IS FIRST INDEX
C     Crout reduction used for LU decomposition.
C       Ref: Introductions to Matrix Computations, G.W. Stewart, pp. 134
C
C
C-----------------------------------------------------------------------
C
      DIMENSION A(JD,KD,3,3), B(JD,KD,3,3), C(JD,KD,3,3), F(JD,KD,3)
      DIMENSION U12(JD),U13(JD),U23(JD),
     &          B11(JD),B21(JD),B31(JD),B22(JD),B32(JD),B33(JD)
C
C-----------------------------------------------------------------------
C
C...FORWARD SWEEP
      K = KS
C
C  L-U DECOMPOSITIION OF B at KS
C
      DO 10 J = JS,JE
         B11(J) = 1./B(J,K,1,1)
         U12(J) = B(J,K,1,2)*B11(J)
         U13(J) = B(J,K,1,3)*B11(J)
         B21(J) = B(J,K,2,1)
         B22(J) = 1./( ((B(J,K,2,2)) -B21(J)*U12(J)) )
         U23(J) = ( ((B(J,K,2,3)) -B21(J)*U13(J)) )*B22(J)
         B31(J) = B(J,K,3,1)
         B32(J) = B(J,K,3,2) -B31(J)*U12(J)
         B33(J) = 1./( ((B(J,K,3,3) -B31(J)*U13(J))
     &            -B32(J)*U23(J)))
 10   CONTINUE
C
C            -1
C  Multiply B  to B, C, A and F at KS.  Note that now the diagonal, B,
C  at KS is an identity matrix.
C
C       -1
C  F = B  *F
C
      DO 12 J = JS,JE
         D1=F(J,K,1)*B11(J)
         D2=(F(J,K,2)-B21(J)*D1)*B22(J)
         D3=((F(J,K,3)-B31(J)*D1)-B32(J)*D2)*B33(J)
         F(J,K,3)=D3
         F(J,K,2)=D2 -U23(J)*F(J,K,3)
         F(J,K,1)=(D1-U13(J)*F(J,K,3)) -U12(J)*F(J,K,2)
   12 CONTINUE
C
C       -1
C  C = B  *C
C
      DO 13 M=1,3
         DO 13 J = JS,JE
            D1=C(J,K,1,M)*B11(J)
            D2=(C(J,K,2,M)-B21(J)*D1)*B22(J)
            D3=((C(J,K,3,M)-B31(J)*D1)-B32(J)*D2)*B33(J)
            C(J,K,3,M)=D3
            C(J,K,2,M)=D2-U23(J)*C(J,K,3,M)
            C(J,K,1,M)=(D1-U13(J)*C(J,K,3,M)) -U12(J)*C(J,K,2,M)
   13 CONTINUE
C
C       -1
C  A = B  *A
C
      DO 14 M=1,3
         DO 14 J = JS,JE
            D1=A(J,K,1,M)*B11(J)
            D2=(A(J,K,2,M)-B21(J)*D1)*B22(J)
            D3=((A(J,K,3,M)-B31(J)*D1)-B32(J)*D2)*B33(J)
            A(J,K,3,M)=D3
            A(J,K,2,M)=D2-U23(J)*A(J,K,3,M)
            A(J,K,1,M)=(D1-U13(J)*A(J,K,3,M)) -U12(J)*A(J,K,2,M)
   14 CONTINUE
C
C      WRITE(6,601)  (F(1,K,N),N=1,3)
C  601 FORMAT(' ' , 8F13.5)
C
      DO 94 K = KS+1,KE-1
C
C  Eliminate lower diagonal A and recalculate B, F and A' (A' is
C  created due to the elimination of A and will be overloaded by
C  A).
C     |I C        A|      |I C         A |  Note: I is identity
C     |A B C       |      |  B' C      A'|        matrix.
C     |  A B C     |  ==> |  A  B C      |
C     |    : : :   |      |     : : :    |
C
         I = K-1
         DO 36 N=1,3
            DO 35 J = JS,JE
C
C  F = F - A *F
C   k   k   k  k-1
C
               F(J,K,N)=((F(J,K,N)-A(J,K,N,1)*F(J,I,1))
     &              -A(J,K,N,2)*F(J,I,2)) -A(J,K,N,3)*F(J,I,3)
 35         CONTINUE
C
            DO 25 M=1,3
               DO 25 J = JS,JE
C
C  B = B - A *C
C   k   k   k  k-1
C
                  B(J,K,N,M)=((B(J,K,N,M)-A(J,K,N,1)*C(J,I,1,M))
     &                 -A(J,K,N,2)*C(J,I,2,M)) -A(J,K,N,3)*C(J,I,3,M)
C
C  B(J,KS,N,M) used as temporary storage for A *A
C                                             k  k-1
C
                  B(J,KS,N,M)=((-A(J,K,N,1)*A(J,I,1,M))
     &                 -A(J,K,N,2)*A(J,I,2,M)) -A(J,K,N,3)*A(J,I,3,M)
 25         CONTINUE
 36      CONTINUE
C
C  Overload A with B(J,KS,N,M), i.e. A = -A *A
C                                     k    k  k-1
C
         DO 7 N=1,3
            DO 7 M=1,3
               DO 7 J = JS,JE
                  A(J,K,N,M)=B(J,KS,N,M)
 7       CONTINUE
C
C  L-U DECOMPOSITIION OF B at K
C
         DO 30 J = JS,JE
            B11(J) = 1./B(J,K,1,1)
            U12(J) = B(J,K,1,2)*B11(J)
            U13(J) = B(J,K,1,3)*B11(J)
            B21(J) = B(J,K,2,1)
            B22(J) = 1./( ((B(J,K,2,2)) -B21(J)*U12(J)) )
            U23(J) = ( ((B(J,K,2,3)) -B21(J)*U13(J)) )*B22(J)
            B31(J) = B(J,K,3,1)
            B32(J) = (B(J,K,3,2)) -B31(J)*U12(J)
            B33(J) = 1./( (((B(J,K,3,3)) -B31(J)*U13(J))
     &               -B32(J)*U23(J)))
 30      CONTINUE
C
C       -1
C  F = B  *F
C
         DO 32 J = JS,JE
            D1=F(J,K,1)*B11(J)
            D2=(F(J,K,2)-B21(J)*D1)*B22(J)
            D3=((F(J,K,3)-B31(J)*D1)-B32(J)*D2)*B33(J)
            F(J,K,3)=D3
            F(J,K,2)=D2 -U23(J)*F(J,K,3)
            F(J,K,1)=(D1-U13(J)*F(J,K,3))-U12(J)*F(J,K,2)
 32      CONTINUE
C
C       -1
C  C = B  *C
C
         DO 33 M=1,3
            DO 33 J = JS,JE
               D1=C(J,K,1,M)*B11(J)
               D2=(C(J,K,2,M)-B21(J)*D1)*B22(J)
               D3=((C(J,K,3,M)-B31(J)*D1)-B32(J)*D2)*B33(J)
               C(J,K,3,M)=D3
               C(J,K,2,M)=D2 -U23(J)*C(J,K,3,M)
               C(J,K,1,M)=(D1-U13(J)*C(J,K,3,M)) -U12(J)*C(J,K,2,M)
 33      CONTINUE
C
C       -1
C  A = B  *A
C
         DO 34 M=1,3
            DO 34 J = JS,JE
               D1=A(J,K,1,M)*B11(J)
               D2=(A(J,K,2,M)-B21(J)*D1)*B22(J)
               D3=((A(J,K,3,M)-B31(J)*D1)-B32(J)*D2)*B33(J)
               A(J,K,3,M)=D3
               A(J,K,2,M)=D2-U23(J)*A(J,K,3,M)
               A(J,K,1,M)=(D1-U13(J)*A(J,K,3,M)) -U12(J)*A(J,K,2,M)
 34      CONTINUE
C
C-----------------------------------------------------------------------
CVV
C      WRITE(*,601)  (F(1,K,N),N=1,3)
 94   CONTINUE
C
C-----------------------------------------------------------------------
CBS                         SET UP BACK SWEEP
C
C  To elinminate A at KE-1, A    = C    - A    *A
C                            ke-1   ke-1   ke-1  ke-2
C     |I C       A|                     |____________|                   
C     |  I C     A|                      This portion is already computed
C     |    : :   :|                      above.  Now it only needs to be
C     |      I C A|                      added to C at KE-1.
C     |      A B C| <== K=KE-1
C     |C       A B|
C
      K = KE-1
      DO 800 N=1,3
         DO 800 M=1,3
            DO 800 J = JS,JE
               A(J,K,N,M) = A(J,K,N,M) + C(J,K,N,M)
 800  CONTINUE
C
C  Now, the remaining matrix is in the form:
C
C     |I C       A| <== K=KS
C     |  I C     A|
C     |    : :   :|
C     |      I C A|
C     |        I A| <== K=KE-1
C     |C       A B|
C
C  we then do a backward sweep to eliminate upper diagonal C from KE-2
C  to KS.
C
      DO 850 K=KE-2,KS,-1
         KP = K+1
         DO 830 N=1,3
C
C  F = F - C *F
C   k   k   k  k+1
C
            DO 810 J = JS,JE
               F(J,K,N)=((F(J,K,N)-C(J,K,N,1)*F(J,KP,1))
     &              -C(J,K,N,2)*F(J,KP,2)) -C(J,K,N,3)*F(J,KP,3)
 810        CONTINUE
C
C  A = A - C *A
C   k   k   k  k+1
C
            DO 820 M=1,3
               DO 820 J = JS,JE
                  A(J,K,N,M)=((A(J,K,N,M)-C(J,K,N,1)*A(J,KP,1,M))
     &                 -C(J,K,N,2)*A(J,KP,2,M)) -C(J,K,N,3)*A(J,KP,3,M)
 820        CONTINUE
 830     CONTINUE
 850  CONTINUE
C
C  We have reduced the matrix to this form now,
C
C     |I         A| <== K=KS
C     |  I       A|
C     |    :     :|
C     |      I   A|
C     |        I A| <== K=KE-1
C     |C       A B| <== K=KE
C
C  Eliminate lower diagonal C and A of last equation, KE.
C
      K  = KE
      KM = K-1
      DO 880 N=1,3
         DO 860 J = JS,JE
            F(J,K,N)=(((((F(J,K,N)-C(J,K,N,1)*F(J,KS,1))
     &           -C(J,K,N,2)*F(J,KS,2)) -C(J,K,N,3)*F(J,KS,3))
     &           -A(J,K,N,1)*F(J,KM,1)) -A(J,K,N,2)*F(J,KM,2))
     &           -A(J,K,N,3)*F(J,KM,3)
 860     CONTINUE
         DO 870 M=1,3
            DO 870 J = JS,JE
               B(J,K,N,M)=(((((B(J,K,N,M)-C(J,K,N,1)*A(J,KS,1,M))
     &              -C(J,K,N,2)*A(J,KS,2,M)) -C(J,K,N,3)*A(J,KS,3,M))
     &              -A(J,K,N,1)*A(J,KM,1,M)) -A(J,K,N,2)*A(J,KM,2,M))
     &              -A(J,K,N,3)*A(J,KM,3,M)
 870     CONTINUE
 880  CONTINUE
C
C  L-U DECOMPOSITIION OF B at KE
C
      DO 51 J = JS,JE
         B11(J) = 1./B(J,K,1,1)
         U12(J) = B(J,K,1,2)*B11(J)
         U13(J) = B(J,K,1,3)*B11(J)
         B21(J) = B(J,K,2,1)
         B22(J) = 1./( ((B(J,K,2,2)) -B21(J)*U12(J)) )
         U23(J) = ( ((B(J,K,2,3)) -B21(J)*U13(J)) )*B22(J)
         B31(J) = B(J,K,3,1)
         B32(J) = (B(J,K,3,2)) -B31(J)*U12(J)
         B33(J) = 1./( (((B(J,K,3,3)) -B31(J)*U13(J))
     &            -B32(J)*U23(J)))
 51   CONTINUE
C
C       -1
C  F = B  *F  (we have obtained solution at K=KE in this step)
C
      DO 54 J = JS,JE
         D1=F(J,K,1)*B11(J)
         D2=(F(J,K,2)-B21(J)*D1)*B22(J)
         D3=((F(J,K,3)-B31(J)*D1)-B32(J)*D2)*B33(J)
         F(J,K,3)=D3
         F(J,K,2)=D2 -U23(J)*F(J,K,3)
         F(J,K,1)=(D1-U13(J)*F(J,K,3)) -U12(J)*F(J,K,2)
 54   CONTINUE
C      WRITE(*,601)  (F(1,K,N),N=1,3)
C
C-----------------------------------------------------------------------
C  Now, we have this simple matrix.  We need only backward sweep to
C  obtain solutions.
C
C     |I         A| <== K=KS
C     |  I       A|
C     |    :     :|
C     |      I   A|
C     |        I A| <== K=KE-1
C     |          I| <== K=KE
C
      DO 20 K=KM,KS,-1
         DO 20 N=1,3
            DO 20 J = JS,JE
               F(J,K,N)=((F(J,K,N)-A(J,K,N,1)*F(J,KE,1))
     &              -A(J,K,N,2)*F(J,KE,2)) -A(J,K,N,3)*F(J,KE,3)
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE V2BTRIP( JD,KD,JS,JE,KS,KE,A,B,C,F,
     &                    U12,U13,U23,B11,B21,B31,B22,B32,B33 )
c*wdh*
c* include "precis.h"
C
C-----------------------------------------------------------------------
C
C
C     (A,B,C)F = F,  A,B AND F ARE OVERLOADED, SOLUTION IN F
C
C     VECTORIZED ON K, K IS SECOND INDEX
C     Crout reduction used for LU decomposition.
C       Ref: Introductions to Matrix Computations, G.W. Stewart, pp. 134
C
C
C-----------------------------------------------------------------------
C
      DIMENSION A(JD,KD,3,3), B(JD,KD,3,3), C(JD,KD,3,3), F(JD,KD,3)
      DIMENSION U12(KD),U13(KD),U23(KD),
     &          B11(KD),B21(KD),B31(KD),B22(KD),B32(KD),B33(KD)
C
C-----------------------------------------------------------------------
C
C...FORWARD SWEEP
      J = JS
C
C  L-U DECOMPOSITIION OF B at JS
C
      DO 10 K = KS,KE
         B11(K) = 1./B(J,K,1,1)
         U12(K) = B(J,K,1,2)*B11(K)
         U13(K) = B(J,K,1,3)*B11(K)
         B21(K) = B(J,K,2,1)
         B22(K) = 1./( ((B(J,K,2,2)) -B21(K)*U12(K)) )
         U23(K) = ( ((B(J,K,2,3)) -B21(K)*U13(K)) )*B22(K)
         B31(K) = B(J,K,3,1)
         B32(K) = B(J,K,3,2) -B31(K)*U12(K)
         B33(K) = 1./( ((B(J,K,3,3) -B31(K)*U13(K))
     &            -B32(K)*U23(K)))
 10   CONTINUE
C
C            -1
C  Multiply B  to B, C, A and F at JS.  Note that now the diagonal, B,
C  at JS is an identity matrix.
C
C       -1
C  F = B  *F
C
      DO 12 K = KS,KE
         D1=F(J,K,1)*B11(K)
         D2=(F(J,K,2)-B21(K)*D1)*B22(K)
         D3=((F(J,K,3)-B31(K)*D1)-B32(K)*D2)*B33(K)
         F(J,K,3)=D3
         F(J,K,2)=D2 -U23(K)*F(J,K,3)
         F(J,K,1)=(D1-U13(K)*F(J,K,3)) -U12(K)*F(J,K,2)
 12   CONTINUE
C
C       -1
C  C = B  *C
C
      DO 13 M=1,3
         DO 13 K = KS,KE
            D1=C(J,K,1,M)*B11(K)
            D2=(C(J,K,2,M)-B21(K)*D1)*B22(K)
            D3=((C(J,K,3,M)-B31(K)*D1)-B32(K)*D2)*B33(K)
            C(J,K,3,M)=D3
            C(J,K,2,M)=D2 -U23(K)*C(J,K,3,M)
            C(J,K,1,M)=(D1 -U13(K)*C(J,K,3,M)) -U12(K)*C(J,K,2,M)
 13   CONTINUE
C
C       -1
C  A = B  *A
C
      DO 14 M=1,3
         DO 14 K = KS,KE
            D1=A(J,K,1,M)*B11(K)
            D2=(A(J,K,2,M)-B21(K)*D1)*B22(K)
            D3=((A(J,K,3,M)-B31(K)*D1)-B32(K)*D2)*B33(K)
            A(J,K,3,M)=D3
            A(J,K,2,M)=D2 -U23(K)*A(J,K,3,M)
            A(J,K,1,M)=(D1 -U13(K)*A(J,K,3,M)) -U12(K)*A(J,K,2,M)
 14   CONTINUE
C
C      WRITE(6,601)  (F(J,1,N),N=1,3)
C  601 FORMAT(' ' , 8F13.5)
C
      DO 94 J = JS+1,JE-1
C
C  Eliminate lower diagonal A and recalculate B, F and A' (A' is
C  created due to the elimination of A and will be overloaded by
C  A).
C     |I C        A|      |I C         A |  Note: I is identity
C     |A B C       |      |  B' C      A'|        matrix.
C     |  A B C     |  ==> |  A  B C      |
C     |    : : :   |      |     : : :    |
C
         I = J-1
         DO 36 N=1,3
            DO 35 K = KS,KE
C
C  F = F - A *F
C   j   j   j  j-1
C
               F(J,K,N)=((F(J,K,N)-A(J,K,N,1)*F(I,K,1))
     &              -A(J,K,N,2)*F(I,K,2)) -A(J,K,N,3)*F(I,K,3)
 35         CONTINUE
C
            DO 25 M=1,3
               DO 25 K = KS,KE
C
C  B = B - A *C
C   j   j   j  j-1
C
                  B(J,K,N,M)=((B(J,K,N,M)-A(J,K,N,1)*C(I,K,1,M))
     &                 -A(J,K,N,2)*C(I,K,2,M)) -A(J,K,N,3)*C(I,K,3,M)
C
C  B(JS,K,N,M) used as temporary storage for A *A
C                                             j  j-1
C
                  B(JS,K,N,M)=((-A(J,K,N,1)*A(I,K,1,M))
     &                 -A(J,K,N,2)*A(I,K,2,M)) -A(J,K,N,3)*A(I,K,3,M)
 25         CONTINUE
 36      CONTINUE
C
C  Overload A with B(JS,K,N,M), i.e. A = -A *A
C                                     j    j  j-1
C
         DO 7 N=1,3
            DO 7 M=1,3
               DO 7 K = KS,KE
                  A(J,K,N,M)=B(JS,K,N,M)
 7       CONTINUE
C
C  L-U DECOMPOSITIION OF B at J
C
         DO 30 K = KS,KE
            B11(K) = 1./B(J,K,1,1)
            U12(K) = B(J,K,1,2)*B11(K)
            U13(K) = B(J,K,1,3)*B11(K)
            B21(K) = B(J,K,2,1)
            B22(K) = 1./( ((B(J,K,2,2)) -B21(K)*U12(K)) )
            U23(K) = ( ((B(J,K,2,3)) -B21(K)*U13(K)) )*B22(K)
            B31(K) = B(J,K,3,1)
            B32(K) = B(J,K,3,2) -B31(K)*U12(K)
            B33(K) = 1./( ((B(J,K,3,3) -B31(K)*U13(K))
     &               -B32(K)*U23(K)))
 30      CONTINUE
C
C       -1
C  F = B  *F
C
         DO 32 K = KS,KE
            D1=F(J,K,1)*B11(K)
            D2=(F(J,K,2)-B21(K)*D1)*B22(K)
            D3=((F(J,K,3)-B31(K)*D1)-B32(K)*D2)*B33(K)
            F(J,K,3)=D3
            F(J,K,2)=D2 -U23(K)*F(J,K,3)
            F(J,K,1)=(D1 -U13(K)*F(J,K,3)) -U12(K)*F(J,K,2)
 32      CONTINUE
C
C       -1
C  C = B  *C
C
         DO 33 M=1,3
            DO 33 K = KS,KE
               D1=C(J,K,1,M)*B11(K)
               D2=(C(J,K,2,M)-B21(K)*D1)*B22(K)
               D3=((C(J,K,3,M)-B31(K)*D1)-B32(K)*D2)*B33(K)
               C(J,K,3,M)=D3
               C(J,K,2,M)=D2 -U23(K)*C(J,K,3,M)
               C(J,K,1,M)=(D1 -U13(K)*C(J,K,3,M)) -U12(K)*C(J,K,2,M)
 33      CONTINUE
C
C       -1
C  A = B  *A
C
         DO 34 M=1,3
            DO 34 K = KS,KE
               D1=A(J,K,1,M)*B11(K)
               D2=(A(J,K,2,M)-B21(K)*D1)*B22(K)
               D3=((A(J,K,3,M)-B31(K)*D1)-B32(K)*D2)*B33(K)
               A(J,K,3,M)=D3
               A(J,K,2,M)=D2 -U23(K)*A(J,K,3,M)
               A(J,K,1,M)=(D1 -U13(K)*A(J,K,3,M)) -U12(K)*A(J,K,2,M)
 34      CONTINUE
C
C-----------------------------------------------------------------------
CVV
C      WRITE(*,601)  (F(J,1,N),N=1,3)
 94   CONTINUE
C
C-----------------------------------------------------------------------
CBS                         SET UP BACK SWEEP
C
C  To elinminate A at JE-1, A    = C    - A    *A
C                            je-1   je-1   je-1  je-2
C     |I C       A|                     |____________|                   
C     |  I C     A|                      This portion is already computed
C     |    : :   :|                      above.  Now it only needs to be
C     |      I C A|                      added to C at JE-1.
C     |      A B C| <== J=JE-1
C     |C       A B|
C
      J = JE-1
      DO 800 N=1,3
         DO 800 M=1,3
            DO 800 K = KS,KE
               A(J,K,N,M) = A(J,K,N,M) + C(J,K,N,M)
 800  CONTINUE
C
C  Now, the remaining matrix is in the form:
C
C     |I C       A| <== J=JS
C     |  I C     A|
C     |    : :   :|
C     |      I C A|
C     |        I A| <== J=JE-1
C     |C       A B|
C
C  we then do a backward sweep to eliminate upper diagonal C from JE-2
C  to JS.
C
      DO 850 J=JE-2,JS,-1
         JP = J+1
         DO 830 N=1,3
C
C  F = F - C *F
C   j   j   j  j+1
C
            DO 810 K = KS,KE
               F(J,K,N)=((F(J,K,N)-C(J,K,N,1)*F(JP,K,1))
     &              -C(J,K,N,2)*F(JP,K,2)) -C(J,K,N,3)*F(JP,K,3)
 810        CONTINUE
C
C  A = A - C *A
C   j   j   j  j+1
C
            DO 820 M=1,3
               DO 820 K = KS,KE
                  A(J,K,N,M)=((A(J,K,N,M)-C(J,K,N,1)*A(JP,K,1,M))
     &                 -C(J,K,N,2)*A(JP,K,2,M)) -C(J,K,N,3)*A(JP,K,3,M)
 820        CONTINUE
 830     CONTINUE
 850  CONTINUE
C
C  We have reduced the matrix to this form now,
C
C     |I         A| <== J=JS
C     |  I       A|
C     |    :     :|
C     |      I   A|
C     |        I A| <== J=JE-1
C     |C       A B| <== J=JE
C
C  Eliminate lower diagonal C and A of last equation, JE.
C
      J  = JE
      JM = J-1
      DO 880 N=1,3
         DO 860 K = KS,KE
            F(J,K,N)=(((((F(J,K,N)-C(J,K,N,1)*F(JS,K,1))
     &           -C(J,K,N,2)*F(JS,K,2)) -C(J,K,N,3)*F(JS,K,3))
     &           -A(J,K,N,1)*F(JM,K,1)) -A(J,K,N,2)*F(JM,K,2))
     &           -A(J,K,N,3)*F(JM,K,3)
 860     CONTINUE
         DO 870 M=1,3
            DO 870 K = KS,KE
               B(J,K,N,M)=(((((B(J,K,N,M)-C(J,K,N,1)*A(JS,K,1,M))
     &              -C(J,K,N,2)*A(JS,K,2,M)) -C(J,K,N,3)*A(JS,K,3,M))
     &              -A(J,K,N,1)*A(JM,K,1,M)) -A(J,K,N,2)*A(JM,K,2,M))
     &              -A(J,K,N,3)*A(JM,K,3,M)
 870     CONTINUE
 880  CONTINUE
C
C  L-U DECOMPOSITIION OF B at JE
C
      DO 51 K = KS,KE
         B11(K) = 1./B(J,K,1,1)
         U12(K) = B(J,K,1,2)*B11(K)
         U13(K) = B(J,K,1,3)*B11(K)
         B21(K) = B(J,K,2,1)
         B22(K) = 1./( ((B(J,K,2,2)) -B21(K)*U12(K)) )
         U23(K) = ( ((B(J,K,2,3)) -B21(K)*U13(K)) )*B22(K)
         B31(K) = B(J,K,3,1)
         B32(K) = B(J,K,3,2) -B31(K)*U12(K)
         B33(K) = 1./( ((B(J,K,3,3) -B31(K)*U13(K))
     &            -B32(K)*U23(K)))
 51   CONTINUE
C
C       -1
C  F = B  *F  (we have obtained solution at J=JE in this step)
C
      DO 54 K = KS,KE
         D1=F(J,K,1)*B11(K)
         D2=(F(J,K,2)-B21(K)*D1)*B22(K)
         D3=((F(J,K,3)-B31(K)*D1)-B32(K)*D2)*B33(K)
         F(J,K,3)=D3
         F(J,K,2)=D2 -U23(K)*F(J,K,3)
         F(J,K,1)=(D1 -U13(K)*F(J,K,3)) -U12(K)*F(J,K,2)
 54   CONTINUE
C      WRITE(*,601)  (F(J,1,N),N=1,3)
C
C-----------------------------------------------------------------------
C  Now, we have this simple matrix.  We need only backward sweep to
C  obtain solutions.
C
C     |I         A| <== J=JS
C     |  I       A|
C     |    :     :|
C     |      I   A|
C     |        I A| <== J=JE-1
C     |          I| <== J=JE
C
      DO 20 J=JM,JS,-1
         DO 20 N=1,3
            DO 20 K = KS,KE
               F(J,K,N)=((F(J,K,N)-A(J,K,N,1)*F(JE,K,1))
     &              -A(J,K,N,2)*F(JE,K,2)) -A(J,K,N,3)*F(JE,K,3)
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ZSPACS(JDIM,KDIM,M1D,JMAX,KMAX,LMAX,VZETA,
     >                  IZSTRT,NZREG,NPZREG,ZREG,DZ0,DZ1,JZS,KZS,
     >                  SPHI,ZETAVR,RR)
c*wdh*
c* include "precis.h"
C
      LOGICAL VZETA
      DIMENSION NPZREG(NZREG), ZREG(NZREG), DZ0(NZREG), DZ1(NZREG)
      DIMENSION SPHI(M1D), ZETAVR(JDIM,KDIM,3), RR(JDIM,KDIM,LMAX)
      DIMENSION IBZ(3), IBZM(3)
C
C   -----------------------------------------------------------------
C    Set up 1D stretching function for marching in zeta direction.
C    Multiple L-regions with different stretching ratios are allowed.
C    Variable far field, initial and end spacings are allowed for
C    the first L-region only. User specified stretching function
C    can be specified in 4 ways:
C    JZS=1,    KZS=1       - same stretching function everywhere
C    JZS=JMAX, KZS=1       - copy variation in J for K=1 to all K
C    JZS=1,    KZS=KMAX    - copy variation in K for J=1 to all J
C    JZS=JMAX, KZS=KMAX    - different stretching for each J and K
C   -----------------------------------------------------------------
C
C    Recommended maximum stretching ratio
      STRMAX = 1.3
C
      RATMAX = 0.0
      LC = 1
C
      IF ( IZSTRT.GT.0 ) THEN
C
C     ------------------------------------
C     Compute stretching for each L-region
C     ------------------------------------
C
      DO 10 NR = 1, NZREG
C
       IF ( .NOT. VZETA ) THEN
C
        IF ( NR .EQ. 1 ) THEN
         RR(1,1,1) = 0.0
C        Trap for zero initial spacing for exponential stretching
C        and make it uniform stretching
          IF ( (IZSTRT.EQ.1).AND.(DZ0(1).EQ.0.0) ) THEN
           DZ0(1) = ZREG(1)/FLOAT(NPZREG(1)-1)
          ENDIF
        ELSE
C        If initial spacing is not specified, reset it such that the
C        stretching ratio is the same as last cell of previous region
          IF (DZ0(NR).EQ.0.0) THEN
            DELM = RR(1,1,LC) - RR(1,1,LC-1)
            DELM1 = RR(1,1,LC-1) - RR(1,1,LC-2)
            DZ0(NR) = DELM*DELM/DELM1
          ENDIF
        ENDIF
C
        ZETAMX = ZREG(NR)
        DZINI  = DZ0(NR)
        DZEND  = DZ1(NR)
C
        IF (IZSTRT.EQ.1) THEN
C
C         Exponential stretching
           EPS = HEPSIL( ZETAMX,DZINI,NPZREG(NR) )
           EPSP1 = 1.0 + EPS
           IF ( EPSP1 .GT. RATMAX ) THEN
            RATMAX = EPSP1
            NRRMAX = NR
           ENDIF
           DO 20 L=2,NPZREG(NR)
            LL = LC + L - 1
            RR(1,1,LL) = RR(1,1,LL-1) + DZINI*(EPSP1**(L-2))
 20        CONTINUE
C
        ELSE IF (IZSTRT.EQ.2) THEN
C
C         Hyperbolic functions stretching
           CALL HTANHS(0.0,ZETAMX,NPZREG(NR),DZINI,DZEND,DZ0A,DZ1A,SPHI)
           DO 30 L=2,NPZREG(NR)
            LL = LC + L - 1
            RR(1,1,LL) = SPHI(L) + RR(1,1,LC)
 30        CONTINUE
C
        ENDIF
C
       ELSE IF ( VZETA ) THEN
C
        IF ( NR .EQ. 1 ) THEN
C
C        Set up blanking arrays for variable zeta
          IBZ(1) = 0
          IBZ(2) = 0
          IBZ(3) = 0
          IF (ZREG(1).LE.0.) IBZ(1) = 1
          IF (DZ0(1).LT.0.)  IBZ(2) = 1
          IF (DZ1(1).LT.0.)  IBZ(3) = 1
          IBZM(1) = 1 - IBZ(1)
          IBZM(2) = 1 - IBZ(2)
          IBZM(3) = 1 - IBZ(3)
C
        ENDIF
C
C       Compute stretching for each point
         DO 40 K=1,KMAX
         DO 40 J=1,JMAX
C
          IF ( NR .EQ. 1 ) THEN
           ZETAMX = IBZ(1)*ZETAVR(J,K,1) + IBZM(1)*ZREG(1)
           DZINI  = IBZ(2)*ZETAVR(J,K,2) + IBZM(2)*DZ0(1)
           DZEND  = IBZ(3)*ZETAVR(J,K,3) + IBZM(3)*DZ1(1)
           RR(J,K,1) = 0.0
           IF ( (IZSTRT.EQ.1).AND.(DZ0(1).EQ.0.0) ) THEN
            DZINI = ZETAMX/FLOAT(NPZREG(1)-1)
           ENDIF
          ELSE IF ( NR .GT. 1 ) THEN
           ZETAMX = ZREG(NR)
           DZINI  = DZ0(NR)
           DZEND  = DZ1(NR)
           IF (DZ0(NR).EQ.0.0) THEN
            DELM = RR(J,K,LC) - RR(J,K,LC-1)
            DELM1 = RR(J,K,LC-1) - RR(J,K,LC-2)
            DZINI = DELM*DELM/DELM1
           ENDIF
          ENDIF
C
C         Make final far field smooother by adjusting to initial variation
          IF ( ( NZREG .GT. 1 ) .AND. ( NR .EQ. NZREG ) )
     >      ZETAMX = ZREG(NR) - ZETAVR(J,K,1)
C
          IF (IZSTRT.EQ.1) THEN
C
C          Exponential stretching
            EPS = HEPSIL( ZETAMX,DZINI,NPZREG(NR) )
            EPSP1 = 1.0 + EPS
            DO 50 L=2,NPZREG(NR)
             LL = LC + L - 1
             RR(J,K,LL) = RR(J,K,LL-1) + DZINI*(EPSP1**(L-2))
 50         CONTINUE
C
          ELSE IF (IZSTRT.EQ.2) THEN
C
C          Hyperbolic functions stretching
           CALL HTANHS(0.0,ZETAMX,NPZREG(NR),DZINI,DZEND,DZ0A,DZ1A,SPHI)
            DO 60 L=2,NPZREG(NR)
             LL = LC + L - 1
             RR(J,K,LL) = SPHI(L) + RR(J,K,LC)
 60         CONTINUE
C
          ENDIF
C
 40      CONTINUE
C
       ENDIF
C
       LC = LC + NPZREG(NR) - 1
C
 10   CONTINUE
C
C      -----------------------------------------------
C      Do other points in J and K if not variable zeta
C      -----------------------------------------------
       IF ( .NOT. VZETA ) THEN
        JEND = 1
        KEND = 1
        DO 80 K=1,KMAX
        DO 80 J=1,JMAX
        DO 80 L=1,LMAX
         RR(J,K,L) = RR(1,1,L)
 80     CONTINUE
       ELSE
        JEND = JMAX
        KEND = KMAX
       ENDIF
C
      ELSE IF ( IZSTRT.EQ.-1 ) THEN
C
C      -----------------------------------------------
C      Complete RR array for user-specified stretching
C      -----------------------------------------------
       JEND = JMAX
       KEND = KMAX
       IF ((JZS.EQ.1).AND.(KZS.EQ.1)) THEN
        JEND = 1
        KEND = 1
        DO 70 K=1,KMAX
        DO 70 J=1,JMAX
        DO 70 L=1,LMAX
         RR(J,K,L) = RR(1,1,L)
 70     CONTINUE
       ELSE IF ((JZS.EQ.JMAX).AND.(KZS.EQ.1)) THEN
        KEND = 1
        DO 72 K=2,KMAX
        DO 72 J=1,JMAX
        DO 72 L=1,LMAX
         RR(J,K,L) = RR(J,1,L)
 72     CONTINUE
       ELSE IF ((JZS.EQ.1).AND.(KZS.EQ.KMAX)) THEN
        JEND = 1
        DO 75 J=2,JMAX
        DO 75 K=1,KMAX
        DO 75 L=1,LMAX
         RR(J,K,L) = RR(1,K,L)
 75     CONTINUE
       ENDIF
C
      ENDIF
C
C    Write out zeta spacing for checks
      WRITE(*,*)' STRETCHING FUNCTION IN NORMAL DIRECTION'
      WRITE(*,101) (RR(1,1,L) ,L=1,LMAX)
 101  FORMAT(1X,6E12.5)
C
C    ------------------------------------
C    Find max stretching ratio and report
C    ------------------------------------
C
      IF ( VZETA .OR. (IZSTRT.EQ.2) .OR. (IZSTRT.EQ.-1) ) THEN
       DO 90 K=1,KEND
       DO 90 J=1,JEND
       DO 90 L=3,LMAX
        D1 = RR(J,K,L-1)-RR(J,K,L-2)
        D2 = RR(J,K,L)-RR(J,K,L-1)
        RD = D2/D1
        IF (RD.GT.RATMAX) THEN
         RATMAX = RD
         JRDMAX = J
         KRDMAX = K
         LRDMAX = L
        ENDIF
 90    CONTINUE
       WRITE(*,107) RATMAX,JRDMAX,KRDMAX,LRDMAX
      ELSE
       WRITE(*,106) RATMAX,NRRMAX
      ENDIF
      IF (RATMAX.GT.STRMAX) WRITE(*,108)
C
 106  FORMAT(' Maximum stretching ratio = ',F7.3,' in L-region ',I2)
 107  FORMAT(' Maximum stretching ratio = ',F7.3,' at J,K,L=',3I4)
 108  FORMAT(' WARNING: Stretching ratio seems high. Recommend trying'
     >       ' more pts. in L')
C
      RETURN
      END
