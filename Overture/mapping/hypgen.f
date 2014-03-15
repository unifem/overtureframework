      subroutine hypgen(
     & IFORM, IZSTRT, NZREG,
     & NPZREG, ZREG, DZ0, DZ1,
     & IBCJA,IBCJB,IBCKA,IBCKB,
     & IVSPEC,EPSSS,ITSVOL,
     & IMETH,SMU2,
     & TIMJ,TIMK,
     & IAXIS,EXAXIS,VOLRES,
     & JMAX, KMAX,
     & JDIM,KDIM,LMAX,
     & X,Y,Z,
     & XW,YW,ZW,
     & M3D, M2D, M1D,
     & RR,     VOLM,VOL,SR,   XX,YX,ZX,XE,YE,ZE,XZ,YZ,ZZ,
     & XIDS,ETDS,ADXI,ADET,ADRXI,ADRET,
     & DAREA,JKBAD,  H,A,B,C,F, CAXI,CAET,CVEX,
     & AFNXI,AFNET,BLN, TMP2,
     & JJP,JJR,KKP,KKR, SPHI,R,LBAD,NBAD,DISSL,DLC,DAREAS,
     & TMP1,ITMP1 )

c*wdh*
c* include "precis.h"

      dimension NPZREG(NZREG), ZREG(NZREG), DZ0(NZREG), DZ1(NZREG)
c*    dimension X(*),Y(*),Z(*)
c*      dimension XW(*),YW(*),ZW(*)
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
c*wdh      PARAMETER (M3D=1781001, M2D=35001, M1D=401)
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
c
      DIMENSION JJP(M1D),JJR(M1D),KKP(M1D),KKR(M1D)
      DIMENSION SPHI(M1D),R(M1D),LBAD(M1D),NBAD(M1D),DISSL(M1D),
     >          DLC(M1D),DAREAS(M1D)
      DIMENSION TMP1(M1D,9)

      dimension ITMP1(M1D,9)
c*wdh      equivalence( TMP1,ITMP1)

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

c*wdh
      do NR=1,NZREG
       ITMP1(NR,1)=NPZREG(NR)
       TMP1(NR,2)=ZREG(NR)
       TMP1(NR,3)=DZ0(NR)
       TMP1(NR,4)=DZ1(NR)
      end do
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
      return
      END

C***********************************************************************
      SUBROUTINE HINIGRD(JDIM,KDIM,JMAX,KMAX,LMAX,JZS,KZS,VZETA,IZSTRT,
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
c*wdh       READ(2) ((X(J,K),J=1,JMAX),K=1,KMAX),
c     >         ((Y(J,K),J=1,JMAX),K=1,KMAX),
c     >         ((Z(J,K),J=1,JMAX),K=1,KMAX)
      ELSE IF (IFMTSU.EQ.1) THEN
c*wdh       READ(2,*) ((X(J,K),J=1,JMAX),K=1,KMAX),
c     >           ((Y(J,K),J=1,JMAX),K=1,KMAX),
c     >           ((Z(J,K),J=1,JMAX),K=1,KMAX)
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
      SUBROUTINE HINIPAR(IFORM,IZSTRT,NZREG,NPZREG,ZREG,DZ0,DZ1,J2D,K2D,
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
c*wdh      READ(*,*) IZSTRT,NZREG
      WRITE(*,602) IZSTRT,NZREG
C
      DO 10 NR=1,NZREG
c*wdh       READ(*,*) NPZREG(NR),ZREG(NR),DZ0(NR),DZ1(NR)
       WRITE(*,603) NPZREG(NR),ZREG(NR),DZ0(NR),DZ1(NR)
 10   CONTINUE
C
c*wdh      READ(*,*) IBCJA,IBCJB,IBCKA,IBCKB
      WRITE(*,604) IBCJA,IBCJB,IBCKA,IBCKB
C
c*wdh      READ(*,*) IVSPEC,EPSSS,ITSVOL
      WRITE(*,605) IVSPEC,EPSSS,ITSVOL
C
c*wdh      READ(*,*) IMETH,SMU2
      WRITE(*,606) IMETH,SMU2
C
c*wdh      READ(*,*) TIMJ,TIMK
      WRITE(*,607) TIMJ,TIMK
C
      IF ( (IBCJA.EQ.20) .OR. (IBCJB.EQ.20) ) THEN
c*wdh       READ(*,*) IAXIS,EXAXIS,VOLRES
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
c*wdh      INQUIRE(FILE='surf.i', EXIST=FILEX)
c*wdh       IF (.NOT. FILEX) THEN
c*wdh        WRITE(*,*)'Cannot find surf.i file. Program terminated.'
c*wdh        STOP
c*wdh       ENDIF
c*wdh      OPEN (2,FILE='surf.i',STATUS='UNKNOWN',form='formatted')
c*wdh       READ(2,*,ERR=20) JMAX,KMAX,LDUM
c*wdh       IFMTSU = 1
       GO TO 30
 20   CLOSE(2)
c*wdh      OPEN (2,FILE='surf.i',STATUS='UNKNOWN',form='unformatted')
c*wdh       READ(2) JMAX,KMAX,LDUM
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
      SUBROUTINE HINITIA(JDIM,KDIM,M1D,M3D,JMAX,KMAX,LMAX,J1,J2,K1,K2,
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
