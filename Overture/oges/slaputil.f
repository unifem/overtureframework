*DECK SBHIN
      SUBROUTINE SBHIN( N, NELT, IA, JA, A, ISYM, SOLN, RHS,
     $     IUNIT, JOB )
C***BEGIN PROLOGUE  SBHIN
C***DATE WRITTEN   881107   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SBHIN-S),
C             Linear system, SLAP Sparse, Diagnostics
C***AUTHOR  Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  Read a Sparse Linear System in the Boeing/Harwell Format.
C            The matrix is read in and if the right hand side is also
C            present in the input file then it too is read in.
C            The matrix is then modified to be in the SLAP Column
C            format.
C***DESCRIPTION
C *Usage:
C     INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IUNIT, JOB
C     REAL    A(NELT), SOLN(N), RHS(N)
C
C     CALL SBHIN( N, NELT, IA, JA, A, ISYM, SOLN, RHS, IUNIT, JOB )
C
C *Arguments:
C N      :OUT      Integer
C         Order of the Matrix.
C NELT   :INOUT    Integer.
C         On input NELT is the maximum number of non-zeros that
C         can be stored in the IA, JA, A arrays.
C         On output NELT is the number of non-zeros stored in A.
C IA     :OUT      Integer IA(NELT).
C JA     :OUT      Integer JA(NELT).
C A      :OUT      Real A(NELT).
C         On output these arrays hold the matrix A in the SLAP
C         Triad format.  See "LONG DESCRIPTION", below.
C ISYM   :OUT      Integer.
C         Flag to indicate symmetric storage format.
C         If ISYM=0, all nonzero entries of the matrix are stored.
C         If ISYM=1, the matrix is symmetric, and only the lower
C         triangle of the matrix is stored.
C SOLN   :OUT      Real SOLN(N).
C         The solution to the linear system, if present.  This array
C         is accessed if and only if JOB to read it in, see below.
C         If the user requests that SOLN be read in, but it is not in
C         the file, then it is simply zeroed out.
C RHS    :OUT      Real RHS(N).
C         The right hand side vector.  This array is accessed if and
C         only if JOB is set to read it in, see below.
C         If the user requests that RHS be read in, but it is not in
C         the file, then it is simply zeroed out.
C IUNIT  :IN       Integer.
C         Fortran logical I/O device unit number to write the matrix
C         to.  This unit must be connected in a system dependent fashion
C         to a file or the console or you will get a nasty message
C         from the Fortran I/O libraries.
C JOB    :INOUT    Integer.
C         Flag indicating what I/O operations to perform.
C         On input JOB indicates what Input operations to try to
C         perform.
C         JOB = 0 => Read only the matrix.
C             = 1 => Read matrix and RHS (if present).
C             = 2 => Read matrix and SOLN (if present).
C             = 3 => Read matrix, RHS and SOLN (if present).
C         On output JOB indicates what operations were actually
C         performed.
C               -3 => Unable to parse matrix "CODE" from input file
C                     to determine if only the lower triangle of matrix
C                     is stored.
C               -2 => Number of non-zeros (NELT) too large.
C               -1 => System size (N) too large.
C         JOB =  0 => Read in only the matrix.
C             =  1 => Read in the matrix and RHS.
C             =  2 => Read in the matrix and SOLN.
C             =  3 => Read in the matrix, RHS and SOLN.
C             = 10 => Read in only the matrix *STRUCTURE*, but no
C                     non-zero entries.  Hence, A(*) is not referenced
C                     and has the return values the same as the input.
C             = 11 => Read in the matrix *STRUCTURE* and RHS.
C             = 12 => Read in the matrix *STRUCTURE* and SOLN.
C             = 13 => Read in the matrix *STRUCTURE*, RHS and SOLN.
C
C *Precision:           Single Precision
C *Portability:
C         You must make sure that IUNIT is a valid Fortran logical
C         I/O device unit number and that the unit number has been
C         associated with a file or the console.  This is a system
C         dependent function.
C
C***LONG DESCRIPTION
C       The format for the output is as follows.  On  the first line
C       are counters and flags: N, NELT, ISYM, IRHS, ISOLN.  N, NELT
C       and ISYM are described above.  IRHS is  a flag indicating if
C       the RHS was  written out (1 is  yes, 0 is  no).  ISOLN  is a
C       flag indicating if the SOLN was written out  (1 is yes, 0 is
C       no).  The format for the fist line is: 5i10.  Then comes the
C       NELT Triad's IA(I), JA(I) and A(I), I = 1, NELT.  The format
C       for  these lines is   :  1X,I5,1X,I5,1X,E16.7.   Then  comes
C       RHS(I), I = 1, N, if IRHS = 1.  Then  comes SOLN(I), I  = 1,
C       N, if ISOLN = 1.  The format for these lines is: 1X,E16.7.
C
C       =================== S L A P Triad format ===================
C       This routine requires that the  matrix A be   stored in  the
C       SLAP  Triad format.  In  this format only the non-zeros  are
C       stored.  They may appear in  *ANY* order.  The user supplies
C       three arrays of  length NELT, where  NELT is  the number  of
C       non-zeros in the matrix: (IA(NELT), JA(NELT), A(NELT)).  For
C       each non-zero the user puts the row and column index of that
C       matrix element  in the IA and  JA arrays.  The  value of the
C       non-zero  matrix  element is  placed   in  the corresponding
C       location of the A array.   This is  an  extremely  easy data
C       structure to generate.  On  the  other hand it   is  not too
C       efficient on vector computers for  the iterative solution of
C       linear systems.  Hence,   SLAP changes   this  input    data
C       structure to the SLAP Column format  for  the iteration (but
C       does not change it back).
C
C       Here is an example of the  SLAP Triad   storage format for a
C       5x5 Matrix.  Recall that the entries may appear in any order.
C
C           5x5 Matrix       SLAP Triad format for 5x5 matrix on left.
C                              1  2  3  4  5  6  7  8  9 10 11
C       |11 12  0  0 15|   A: 51 12 11 33 15 53 55 22 35 44 21
C       |21 22  0  0  0|  IA:  5  1  1  3  1  5  5  2  3  4  2
C       | 0  0 33  0 35|  JA:  1  2  1  3  5  3  5  2  5  4  1
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SBHIN
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, JOB
      REAL    A(NELT), RHS(N), SOLN(N)
C
C         Local Variables
C
      CHARACTER*80  TITLE
      CHARACTER*3   CODE
      CHARACTER*16  PNTFMT, RINFMT
      CHARACTER*20  NVLFMT, RHSFMT
C
      INTEGER NLINE, NPLS, NRILS, NNVLS, NRHSLS, NROW, NCOL, NIND, NELE
C
C         Read Matrices In BOEING-HARWELL format.
C
C NLINE  Number of Data (after the header) lines in the file.
C NPLS   Number of lines for the Column Pointer data in the file.
C NRILS  Number of lines for the Row indicies in the data file.
C NNVLS  Number of lines for the Matrix elements in the data file.
C NRHSLS Number of lines for the RHS in the data file.
C
C***FIRST EXECUTABLE STATEMENT  SBHIN
      READ(IUNIT,9000) TITLE
      READ(IUNIT,9010) NLINE, NPLS, NRILS, NNVLS, NRHSLS
      READ(IUNIT,9020) CODE, NROW, NCOL, NIND, NELE
      READ(IUNIT,9030) PNTFMT, RINFMT, NVLFMT, RHSFMT
C
      IF( NROW.GT.N ) THEN
         N = NROW
         JOBRET = -1
         GOTO 999
      ENDIF
      IF( NIND.GT.NELT ) THEN
         NELT = NIND
         JOBRET = -2
         GOTO 999
      ENDIF
C
C         Set the parameters.
C
      N    = NROW
      NELT = NIND
      IF( CODE.EQ.'RUA' ) THEN
         ISYM = 0
      ELSE IF( CODE.EQ.'RSA' ) THEN
         ISYM = 1
      ELSE
         JOBRET = -3
         GOTO 999
      ENDIF
      READ(IUNIT,PNTFMT) (JA(I), I = 1, N+1)
      READ(IUNIT,RINFMT) (IA(I), I = 1, NELT)
      JOBRET = 10
      IF( NNVLS.GT.0 ) THEN
         READ(IUNIT,NVLFMT) (A(I),  I = 1, NELT)
         JOBRET = 0
      ENDIF
      IF( NRHSLS.GT.0 .AND. MOD(JOB,2).EQ.1 ) THEN
         READ(5,RHSFMT) (RHS(I), I = 1, N)
         JOBRET = JOBRET + 1
      ENDIF
C
C         Now loop thru the IA(i) array making sure that the Diagonal
C         matrix element appears first in the column.  Then sort the
C         rest of the column in ascending order.
C
CVD$R NOCONCUR
CVD$R NOVECTOR
      DO 70 ICOL = 1, N
         IBGN = JA(ICOL)
         IEND = JA(ICOL+1)-1
         DO 30 I = IBGN, IEND
            IF( IA(I).EQ.ICOL ) THEN
C         Swap the diag element with the first element in the column.
               ITEMP = IA(I)
               IA(I) = IA(IBGN)
               IA(IBGN) = ITEMP
               TEMP = A(I)
               A(I) = A(IBGN)
               A(IBGN) = TEMP
               GOTO 40
            ENDIF
 30      CONTINUE
 40      IBGN = IBGN + 1
         IF( IBGN.LT.IEND ) THEN
            DO 60 I = IBGN, IEND
               DO 50 J = I+1, IEND
                  IF( IA(I).GT.IA(J) ) THEN
                     ITEMP = IA(I)
                     IA(I) = IA(J)
                     IA(J) = ITEMP
                     TEMP = A(I)
                     A(I) = A(J)
                     A(J) = TEMP
                  ENDIF
 50            CONTINUE
 60         CONTINUE
         ENDIF
 70   CONTINUE
C
C         Set return flag.
 999  JOB = JOBRET
      RETURN
 9000 FORMAT( A80 )
 9010 FORMAT( 5I14 )
 9020 FORMAT( A3, 11X, 4I14 )
 9030 FORMAT( 2A16, 2A20 )
C------------- LAST LINE OF SBHIN FOLLOWS ------------------------------
      END
*DECK SCHKW
      SUBROUTINE SCHKW( NAME, LOCIW, LENIW, LOCW, LENW,
     $     IERR, ITER, ERR )
C***BEGIN PROLOGUE  SCHKW
C***DATE WRITTEN   880225   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  R2
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SCHKW-S),
C             SLAP, Error Checking, Workspace Checking
C***AUTHOR  Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP WORK/IWORK Array Bounds Checker.
C            This routine checks the work array lengths  and  inter-
C            faces to the SLATEC  error  handler  if  a  problem  is
C            found.
C***DESCRIPTION
C *Usage:
C     CHARACTER*(*) NAME
C     INTEGER LOCIW, LENIW, LOCW, LENW, IERR, ITER
C     REAL    ERR
C
C     CALL SCHKW( NAME, LOCIW, LENIW, LOCW, LENW, IERR, ITER, ERR )
C
C *Arguments:
C NAME   :IN       Character*(*).
C         Name of the calling routine.  This is used in the output
C         message, if an error is detected.
C LOCIW  :IN       Integer.
C         Location of the first free element in the integer workspace
C         array.
C LENIW  :IN       Integer.
C         Length of the integer workspace array.
C LOCW   :IN       Integer.
C         Location of the first free element in the real workspace
C         array.
C LENRW  :IN       Integer.
C         Length of the real workspace array.
C IERR   :OUT      Integer.
C         Return error flag.
C               IERR = 0 => All went well.
C               IERR = 1 => Insufficient storage allocated for
C                           WORK or IWORK.
C ITER   :OUT      Integer.
C         Set to 0 if an error is detected.
C ERR    :OUT      Real.
C         Set to a very large number if an error is detected.
C
C *Precision:           Single Precision
C
C***REFERENCES  (NONE)
C***ROUTINES CALLED  R1MACH, XERRWV
C***END PROLOGUE  SCHKW
      CHARACTER*(*) NAME
      CHARACTER*72 MESG
      INTEGER LOCIW, LENIW, LOCW, LENW, IERR, ITER
c*wdh REAL    ERR, R1MACH
c*wdh EXTERNAL R1MACH, XERRWV
      REAL    ERR, DRMACH
      EXTERNAL DRMACH, XERRWV
C
C         Check the Integer workspace situation.
C***FIRST EXECUTABLE STATEMENT  SCHKW
      IERR = 0
      IF( LOCIW.GT.LENIW ) THEN
         IERR = 1
         ITER = 0
c*wdh    ERR = R1MACH(2)
         ERR = DRMACH(2)
         MESG = NAME // ': INTEGER work array too short. '//
     $        ' IWORK needs i1: have allocated i2.'
         CALL XERRWV( MESG, LEN(MESG), 1, 1, 2, LOCIW, LENIW,
     $        0, 0.0, 0.0 )
      ENDIF
C
C         Check the Real workspace situation.
      IF( LOCW.GT.LENW ) THEN
         IERR = 1
         ITER = 0
c*wdh    ERR = R1MACH(2)
         ERR = DRMACH(2)
         MESG = NAME // ': REAL work array too short. '//
     $        ' RWORK needs i1: have allocated i2.'
         CALL XERRWV( MESG, LEN(MESG), 1, 1, 2, LOCW, LENW,
     $        0, 0.0, 0.0 )
      ENDIF
      RETURN
C------------- LAST LINE OF SCHKW FOLLOWS ----------------------------
      END
*DECK QS2I1R
      SUBROUTINE QS2I1R( IA, JA, A, N, KFLAG )
C***BEGIN PROLOGUE  QS2I1R
C***DATE WRITTEN   761118   (YYMMDD)
C***REVISION DATE  890125   (YYMMDD)
C***CATEGORY NO.  N6A2A
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=INTEGER(QS2I1R-I),
C             QUICKSORT,SINGLETON QUICKSORT,SORT,SORTING
C***AUTHOR  Jones, R. E., (SNLA)
C           Kahaner, D. K., (NBS)
C           Seager, M. K., (LLNL) seager@lll-crg.llnl.gov
C           Wisniewski, J. A., (SNLA)
C***PURPOSE  Sort an integer array also moving an integer and real array
C            This routine sorts the integer array IA and makes the same
C            interchanges in the integer array JA and the real array A.
C            The array IA may be sorted in increasing order or decreas-
C            ing order. A slightly modified QUICKSORT algorithm is used.
C
C***DESCRIPTION
C     Written by Rondall E Jones
C     Modified by John A. Wisniewski to use the Singleton QUICKSORT
C     algorithm. date 18 November 1976.
C
C     Further modified by David K. Kahaner
C     National Bureau of Standards
C     August, 1981
C
C     Even further modification made to bring the code up to the
C     Fortran 77 level and make it more readable and to carry
C     along one integer array and one real array during the sort by
C     Mark K. Seager
C     Lawrence Livermore National Laboratory
C     November, 1987
C     This routine was adapted from the ISORT routine.
C
C     ABSTRACT
C         This routine sorts an integer array IA and makes the same
C         interchanges in the integer array JA and the real array A.
C         The array a may be sorted in increasing order or decreasing
C         order.  A slightly modified quicksort algorithm is used.
C
C     DESCRIPTION OF PARAMETERS
C        IA - Integer array of values to be sorted.
C        JA - Integer array to be carried along.
C         A - Real array to be carried along.
C         N - Number of values in integer array IA to be sorted.
C     KFLAG - Control parameter
C           = 1 means sort IA in INCREASING order.
C           =-1 means sort IA in DECREASING order.
C
C***REFERENCES
C     Singleton, R. C., Algorithm 347, "An Efficient Algorithm for
C     Sorting with Minimal Storage", cacm, Vol. 12, No. 3, 1969,
C     Pp. 185-187.
C***ROUTINES CALLED  XERROR
C***END PROLOGUE  QS2I1R
CVD$R NOVECTOR
CVD$R NOCONCUR
      DIMENSION IL(21),IU(21)
      INTEGER   IA(N),JA(N),IT,IIT,JT,JJT
      REAL      A(N), TA, TTA
C
C***FIRST EXECUTABLE STATEMENT  QS2I1R
      NN = N
      IF (NN.LT.1) THEN
         CALL XERROR ( 'QS2I1R- the number of values to be sorted was no
     $t POSITIVE.',59,1,1)
         RETURN
      ENDIF
      IF( N.EQ.1 ) RETURN
      KK = IABS(KFLAG)
      IF ( KK.NE.1 ) THEN
         CALL XERROR ( 'QS2I1R- the sort control parameter, k, was not 1
     $ OR -1.',55,2,1)
         RETURN
      ENDIF
C
C     Alter array IA to get decreasing order if needed.
C
      IF( KFLAG.LT.1 ) THEN
         DO 20 I=1,NN
            IA(I) = -IA(I)
 20      CONTINUE
      ENDIF
C
C     Sort IA and carry JA and A along.
C     And now...Just a little black magic...
      M = 1
      I = 1
      J = NN
      R = .375
 210  IF( R.LE.0.5898437 ) THEN
         R = R + 3.90625E-2
      ELSE
         R = R-.21875
      ENDIF
 225  K = I
C
C     Select a central element of the array and save it in location
C     it, jt, at.
C
      IJ = I + IFIX (FLOAT (J-I) *R)
      IT = IA(IJ)
      JT = JA(IJ)
      TA = A(IJ)
C
C     If first element of array is greater than it, interchange with it.
C
      IF( IA(I).GT.IT ) THEN
         IA(IJ) = IA(I)
         IA(I)  = IT
         IT     = IA(IJ)
         JA(IJ) = JA(I)
         JA(I)  = JT
         JT     = JA(IJ)
         A(IJ)  = A(I)
         A(I)   = TA
         TA     = A(IJ)
      ENDIF
      L=J
C
C     If last element of array is less than it, swap with it.
C
      IF( IA(J).LT.IT ) THEN
         IA(IJ) = IA(J)
         IA(J)  = IT
         IT     = IA(IJ)
         JA(IJ) = JA(J)
         JA(J)  = JT
         JT     = JA(IJ)
         A(IJ)  = A(J)
         A(J)   = TA
         TA     = A(IJ)
C
C     If first element of array is greater than it, swap with it.
C
         IF ( IA(I).GT.IT ) THEN
            IA(IJ) = IA(I)
            IA(I)  = IT
            IT     = IA(IJ)
            JA(IJ) = JA(I)
            JA(I)  = JT
            JT     = JA(IJ)
            A(IJ)  = A(I)
            A(I)   = TA
            TA     = A(IJ)
         ENDIF
      ENDIF
C
C     Find an element in the second half of the array which is
C     smaller than it.
C
  240 L=L-1
      IF( IA(L).GT.IT ) GO TO 240
C
C     Find an element in the first half of the array which is
C     greater than it.
C
  245 K=K+1
      IF( IA(K).LT.IT ) GO TO 245
C
C     Interchange these elements.
C
      IF( K.LE.L ) THEN
         IIT   = IA(L)
         IA(L) = IA(K)
         IA(K) = IIT
         JJT   = JA(L)
         JA(L) = JA(K)
         JA(K) = JJT
         TTA   = A(L)
         A(L)  = A(K)
         A(K)  = TTA
         GOTO 240
      ENDIF
C
C     Save upper and lower subscripts of the array yet to be sorted.
C
      IF( L-I.GT.J-K ) THEN
         IL(M) = I
         IU(M) = L
         I = K
         M = M+1
      ELSE
         IL(M) = K
         IU(M) = J
         J = L
         M = M+1
      ENDIF
      GO TO 260
C
C     Begin again on another portion of the unsorted array.
C
  255 M = M-1
      IF( M.EQ.0 ) GO TO 300
      I = IL(M)
      J = IU(M)
  260 IF( J-I.GE.1 ) GO TO 225
      IF( I.EQ.J ) GO TO 255
      IF( I.EQ.1 ) GO TO 210
      I = I-1
  265 I = I+1
      IF( I.EQ.J ) GO TO 255
      IT = IA(I+1)
      JT = JA(I+1)
      TA =  A(I+1)
      IF( IA(I).LE.IT ) GO TO 265
      K=I
  270 IA(K+1) = IA(K)
      JA(K+1) = JA(K)
      A(K+1)  =  A(K)
      K = K-1
      IF( IT.LT.IA(K) ) GO TO 270
      IA(K+1) = IT
      JA(K+1) = JT
      A(K+1)  = TA
      GO TO 265
C
C     Clean up, if necessary.
C
  300 IF( KFLAG.LT.1 ) THEN
         DO 310 I=1,NN
            IA(I) = -IA(I)
 310     CONTINUE
      ENDIF
      RETURN
C------------- LAST LINE OF QS2I1R FOLLOWS ----------------------------
      END
*DECK SS2Y
      SUBROUTINE SS2Y(N, NELT, IA, JA, A, ISYM )
C***BEGIN PROLOGUE  SS2Y
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SS2Y-S),
C             Linear system, SLAP Sparse
C***AUTHOR  Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP Triad to SLAP Column Format Converter.
C            Routine to convert from the SLAP Triad to SLAP Column
C            format.
C***DESCRIPTION
C *Usage:
C     INTEGER N, NELT, IA(NELT), JA(NELT), ISYM
C     REAL    A(NELT)
C
C     CALL SS2Y( N, NELT, IA, JA, A, ISYM )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C NELT   :IN       Integer.
C         Number of non-zeros stored in A.
C IA     :INOUT    Integer IA(NELT).
C JA     :INOUT    Integer JA(NELT).
C A      :INOUT    Real A(NELT).
C         These arrays should hold the matrix A in either the SLAP
C         Triad format or the SLAP Column format.  See "LONG
C         DESCRIPTION", below.  If the SLAP Triad format is used
C         this format is translated to the SLAP Column format by
C         this routine.
C ISYM   :IN       Integer.
C         Flag to indicate symmetric storage format.
C         If ISYM=0, all nonzero entries of the matrix are stored.
C         If ISYM=1, the matrix is symmetric, and only the lower
C         triangle of the matrix is stored.
C
C *Precision:           Single Precision
C
C***LONG DESCRIPTION
C       The Sparse Linear Algebra Package (SLAP) utilizes two matrix
C       data structures: 1) the  SLAP Triad  format or  2)  the SLAP
C       Column format.  The user can hand this routine either of the
C       of these data structures.  If the SLAP Triad format is give
C       as input then this routine transforms it into SLAP Column
C       format.  The way this routine tells which format is given as
C       input is to look at JA(N+1).  If JA(N+1) = NELT+1 then we
C       have the SLAP Column format.  If that equality does not hold
C       then it is assumed that the IA, JA, A arrays contain the
C       SLAP Triad format.
C
C       =================== S L A P Triad format ===================
C       This routine requires that the  matrix A be   stored in  the
C       SLAP  Triad format.  In  this format only the non-zeros  are
C       stored.  They may appear in  *ANY* order.  The user supplies
C       three arrays of  length NELT, where  NELT is  the number  of
C       non-zeros in the matrix: (IA(NELT), JA(NELT), A(NELT)).  For
C       each non-zero the user puts the row and column index of that
C       matrix element  in the IA and  JA arrays.  The  value of the
C       non-zero   matrix  element is  placed  in  the corresponding
C       location of the A array.   This is  an  extremely  easy data
C       structure to generate.  On  the  other hand it   is  not too
C       efficient on vector computers for  the iterative solution of
C       linear systems.  Hence,   SLAP changes   this  input    data
C       structure to the SLAP Column format  for  the iteration (but
C       does not change it back).
C
C       Here is an example of the  SLAP Triad   storage format for a
C       5x5 Matrix.  Recall that the entries may appear in any order.
C
C           5x5 Matrix       SLAP Triad format for 5x5 matrix on left.
C                              1  2  3  4  5  6  7  8  9 10 11
C       |11 12  0  0 15|   A: 51 12 11 33 15 53 55 22 35 44 21
C       |21 22  0  0  0|  IA:  5  1  1  3  1  5  5  2  3  4  2
C       | 0  0 33  0 35|  JA:  1  2  1  3  5  3  5  2  5  4  1
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C       =================== S L A P Column format ==================
C       This routine  requires that  the matrix A  be stored in  the
C       SLAP Column format.  In this format the non-zeros are stored
C       counting down columns (except  for the diagonal entry, which
C       must appear first in each  "column") and  are stored  in the
C       real array A.  In other words, for each column in the matrix
C       put the diagonal entry in A.  Then put in the other non-zero
C       elements going down   the  column (except  the diagonal)  in
C       order.  The IA array holds the row  index for each non-zero.
C       The JA array holds the offsets into the IA, A arrays for the
C       beginning of   each    column.    That  is,    IA(JA(ICOL)),
C       A(JA(ICOL)) points to the beginning of the ICOL-th column in
C       IA and  A.  IA(JA(ICOL+1)-1),  A(JA(ICOL+1)-1) points to the
C       end  of   the ICOL-th  column.  Note   that  we  always have
C       JA(N+1) = NELT+1, where  N  is the number of columns in  the
C       matrix and  NELT   is the number of non-zeros in the matrix.
C
C       Here is an example of the  SLAP Column  storage format for a
C       5x5 Matrix (in the A and IA arrays '|'  denotes the end of a
C       column):
C
C           5x5 Matrix      SLAP Column format for 5x5 matrix on left.
C                              1  2  3    4  5    6  7    8    9 10 11
C       |11 12  0  0 15|   A: 11 21 51 | 22 12 | 33 53 | 44 | 55 15 35
C       |21 22  0  0  0|  IA:  1  2  5 |  2  1 |  3  5 |  4 |  5  1  3
C       | 0  0 33  0 35|  JA:  1  4  6    8  9   12
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C***REFERENCES  (NONE)
C***ROUTINES CALLED  QS2I1R
C***END PROLOGUE  SS2Y
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM
      REAL    A(NELT)
C
C         Check to see if the (IA,JA,A) arrays are in SLAP Column
C         format.  If it's not then transform from SLAP Triad.
C***FIRST EXECUTABLE STATEMENT  SS2LT
      IF( JA(N+1).EQ.NELT+1 ) RETURN
C
C         Sort into ascending order by COLUMN (on the ja array).
C         This will line up the columns.
C
      CALL QS2I1R( JA, IA, A, NELT, 1 )
C
C         Loop over each column to see where the column indicies change
C         in the column index array ja.  This marks the beginning of the
C         next column.
C
CVD$R NOVECTOR
      JA(1) = 1
      DO 20 ICOL = 1, N-1
         DO 10 J = JA(ICOL)+1, NELT
            IF( JA(J).NE.ICOL ) THEN
               JA(ICOL+1) = J
               GOTO 20
            ENDIF
 10      CONTINUE
 20   CONTINUE
      JA(N+1) = NELT+1
C
C         Mark the n+2 element so that future calls to a SLAP routine
C         utilizing the YSMP-Column storage format will be able to tell.
C
      JA(N+2) = 0
C
C         Now loop thru the ia(i) array making sure that the Diagonal
C         matrix element appears first in the column.  Then sort the
C         rest of the column in ascending order.
C
      DO 70 ICOL = 1, N
         IBGN = JA(ICOL)
         IEND = JA(ICOL+1)-1
         DO 30 I = IBGN, IEND
            IF( IA(I).EQ.ICOL ) THEN
C         Swap the diag element with the first element in the column.
               ITEMP = IA(I)
               IA(I) = IA(IBGN)
               IA(IBGN) = ITEMP
               TEMP = A(I)
               A(I) = A(IBGN)
               A(IBGN) = TEMP
               GOTO 40
            ENDIF
 30      CONTINUE
 40      IBGN = IBGN + 1
         IF( IBGN.LT.IEND ) THEN
            DO 60 I = IBGN, IEND
               DO 50 J = I+1, IEND
                  IF( IA(I).GT.IA(J) ) THEN
                     ITEMP = IA(I)
                     IA(I) = IA(J)
                     IA(J) = ITEMP
                     TEMP = A(I)
                     A(I) = A(J)
                     A(J) = TEMP
                  ENDIF
 50            CONTINUE
 60         CONTINUE
         ENDIF
 70   CONTINUE
      RETURN
C------------- LAST LINE OF SS2Y FOLLOWS ----------------------------
      END
*DECK SCPPLT
      SUBROUTINE SCPPLT( N, NELT, IA, JA, A, ISYM, IUNIT )
C***BEGIN PROLOGUE  SCPPLT
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SCPPLT-S),
C             Linear system, SLAP Sparse, Diagnostics
C***AUTHOR  Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  Printer Plot of SLAP Column Format Matrix.
C            Routine to print out a SLAP Column format matrix in
C            a "printer plot" graphical representation.
C***DESCRIPTION
C *Usage:
C     INTEGER N, NELT, IA(NELT), JA(N+1), ISYM, IUNIT
C     REAL    A(NELT)
C
C     CALL SCPPLT( N, NELT, IA, JA, A, ISYM, IUNIT )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C NELT   :IN       Integer.
C         Number of non-zeros stored in A.
C IA     :INOUT    Integer IA(NELT).
C JA     :INOUT    Integer JA(N+1).
C A      :INOUT    Real A(NELT).
C         These arrays should hold the matrix A in the SLAP
C         Column format.  See "LONG DESCRIPTION", below.
C ISYM   :IN       Integer.
C         Flag to indicate symmetric storage format.
C         If ISYM=0, all nonzero entries of the matrix are stored.
C         If ISYM=1, the matrix is symmetric, and only the lower
C         triangle of the matrix is stored.
C IUNIT  :IN       Integer.
C         Fortran logical I/O device unit number to write the matrix
C         to.  This unit must be connected in a system dependent fashion
C         to a file or the console or you will get a nasty message
C         from the Fortran I/O libraries.
C
C *Precision:           Single Precision
C *Portability:
C         You must make sure that IUNIT is a valid Fortran logical
C         I/O device unit number and that the unit number has been
C         associated with a file or the console.  This is a system
C         dependent function.
C
C***LONG DESCRIPTION
C       This routine prints out a SLAP  Column format matrix  to the
C       Fortran logical I/O unit   number  IUNIT.  The  numbers them
C       selves  are not printed  out, but   rather  a one  character
C       representation of the numbers.   Elements of the matrix that
C       are not represented in the (IA,JA,A)  arrays are  denoted by
C       ' ' character (a blank).  Elements of A that are *ZERO* (and
C       hence  should  really not be  stored) are  denoted  by a '0'
C       character.  Elements of A that are *POSITIVE* are denoted by
C       'D' if they are Diagonal elements  and '#' if  they are off
C       Diagonal  elements.  Elements of  A that are *NEGATIVE* are
C       denoted by 'N'  if they  are Diagonal  elements and  '*' if
C       they are off Diagonal elements.
C
C       =================== S L A P Column format ==================
C       This routine  requires that  the matrix A  be stored in  the
C       SLAP Column format.  In this format the non-zeros are stored
C       counting down columns (except  for the diagonal entry, which
C       must appear first in each  "column") and  are stored  in the
C       real array A.  In other words, for each column in the matrix
C       put the diagonal entry in A.  Then put in the other non-zero
C       elements going down   the  column (except  the diagonal)  in
C       order.  The IA array holds the row  index for each non-zero.
C       The JA array holds the offsets into the IA, A arrays for the
C       beginning of   each    column.    That  is,    IA(JA(ICOL)),
C       A(JA(ICOL)) points to the beginning of the ICOL-th column in
C       IA and  A.  IA(JA(ICOL+1)-1),  A(JA(ICOL+1)-1) points to the
C       end  of   the ICOL-th  column.  Note   that  we  always have
C       JA(N+1) = NELT+1, where  N  is the number of columns in  the
C       matrix and  NELT   is the number of non-zeros in the matrix.
C
C       Here is an example of the  SLAP Column  storage format for a
C       5x5 Matrix (in the A and IA arrays '|'  denotes the end of a
C       column):
C
C           5x5 Matrix      SLAP Column format for 5x5 matrix on left.
C                              1  2  3    4  5    6  7    8    9 10 11
C       |11 12  0  0 15|   A: 11 21 51 | 22 12 | 33 53 | 44 | 55 15 35
C       |21 22  0  0  0|  IA:  1  2  5 |  2  1 |  3  5 |  4 |  5  1  3
C       | 0  0 33  0 35|  JA:  1  4  6    8  9   12
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SCPPLT
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM
      REAL    A(NELT)
      CHARACTER*225 CHMAT(225)
C
C         Set up the character matrix...
C***FIRST EXECUTABLE STATEMENT  SCPPLT
      NMAX = MIN( 225, N)
      DO 10 I = 1, NMAX
         CHMAT(I)(1:NMAX) = ' '
 10   CONTINUE
      DO 30 ICOL = 1, NMAX
         JBGN = JA(ICOL)
         JEND = JA(ICOL+1)-1
         DO 20 J = JBGN, JEND
            IROW = IA(J)
            IF( IROW.LE.NMAX ) THEN
               IF( ISYM.NE.0 ) THEN
C         Put in non-sym part as well...
                  IF( A(J).EQ.0.0 ) THEN
                     CHMAT(IROW)(ICOL:ICOL) = '0'
                  ELSEIF( A(J).GT.0.0 ) THEN
                     CHMAT(IROW)(ICOL:ICOL) = '#'
                  ELSE
                     CHMAT(IROW)(ICOL:ICOL) = '*'
                  ENDIF
               ENDIF
               IF( IROW.EQ.ICOL ) THEN
C         Diagonal entry.
                  IF( A(J).EQ.0.0 ) THEN
                     CHMAT(IROW)(ICOL:ICOL) = '0'
                  ELSEIF( A(J).GT.0.0 ) THEN
                     CHMAT(IROW)(ICOL:ICOL) = 'D'
                  ELSE
                     CHMAT(IROW)(ICOL:ICOL) = 'N'
                  ENDIF
               ELSE
C         Off-Diagonal entry
                  IF( A(J).EQ.0.0 ) THEN
                     CHMAT(IROW)(ICOL:ICOL) = '0'
                  ELSEIF( A(J).GT.0.0 ) THEN
                     CHMAT(IROW)(ICOL:ICOL) = '#'
                  ELSE
                     CHMAT(IROW)(ICOL:ICOL) = '*'
                  ENDIF
               ENDIF
            ENDIF
 20      CONTINUE
 30   CONTINUE
C
C         Write out the heading.
      WRITE(IUNIT,1000) N, NELT, FLOAT(NELT)/FLOAT(N*N)
      WRITE(IUNIT,1010) (MOD(I,10),I=1,NMAX)
C
C         Write out the character representations matrix elements.
      DO 40 IROW = 1, NMAX
         WRITE(IUNIT,1020) IROW, CHMAT(IROW)(1:NMAX)
 40   CONTINUE
      RETURN
 1000 FORMAT(/'**** Picture of Column SLAP matrix follows ****'/
     $     ' N, NELT and Density = ',2I10,E16.7)
 1010 FORMAT(4X,255(I1))
 1020 FORMAT(1X,I3,A)
C------------- LAST LINE OF SCPPLT FOLLOWS ----------------------------
      END
*DECK STOUT
      SUBROUTINE STOUT( N, NELT, IA, JA, A, ISYM, SOLN, RHS,
     $     IUNIT, JOB )
C***BEGIN PROLOGUE  STOUT
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(STOUT-S),
C             Linear system, SLAP Sparse, Diagnostics
C***AUTHOR  Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  Write out SLAP Triad Format Linear System.
C            Routine to write out a SLAP Triad format matrix and
C            right hand side and solution to the system, if known.
C***DESCRIPTION
C *Usage:
C     INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IUNIT, JOB
C     REAL    A(NELT), SOLN(N), RHS(N)
C
C     CALL STOUT( N, NELT, IA, JA, A, ISYM, SOLN, RHS, IUNIT, JOB )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C NELT   :IN       Integer.
C         Number of non-zeros stored in A.
C IA     :INOUT    Integer IA(NELT).
C JA     :INOUT    Integer JA(NELT).
C A      :INOUT    Real A(NELT).
C         These arrays should hold the matrix A in the SLAP
C         Triad format.  See "LONG DESCRIPTION", below.
C ISYM   :IN       Integer.
C         Flag to indicate symmetric storage format.
C         If ISYM=0, all nonzero entries of the matrix are stored.
C         If ISYM=1, the matrix is symmetric, and only the lower
C         triangle of the matrix is stored.
C SOLN   :IN       Real SOLN(N).
C         The solution to the linear system, if known.  This array
C         is accessed if and only if JOB is set to print it out,
C         see below.
C RHS    :IN       Real RHS(N).
C         The right hand side vector.  This array is accessed if and
C         only if JOB is set to print it out, see below.
C IUNIT  :IN       Integer.
C         Fortran logical I/O device unit number to write the matrix
C         to.  This unit must be connected in a system dependent fashion
C         to a file or the console or you will get a nasty message
C         from the Fortran I/O libraries.
C JOB    :IN       Integer.
C         Flag indicating what I/O operations to perform.
C         JOB = 0 => Print only the matrix.
C             = 1 => Print matrix and RHS.
C             = 2 => Print matrix and SOLN.
C             = 3 => Print matrix, RHS and SOLN.
C
C *Precision:           Single Precision
C *Portability:
C         You must make sure that IUNIT is a valid Fortran logical
C         I/O device unit number and that the unit number has been
C         associated with a file or the console.  This is a system
C         dependent function.
C
C***LONG DESCRIPTION
C       The format for the output is as follows.  On  the first line
C       are counters and flags: N, NELT, ISYM, IRHS, ISOLN.  N, NELT
C       and ISYM are described above.  IRHS is  a flag indicating if
C       the RHS was  written out (1 is  yes, 0 is  no).  ISOLN  is a
C       flag indicating if the SOLN was written out  (1 is yes, 0 is
C       no).  The format for the fist line is: 5i10.  Then comes the
C       NELT Triad's IA(I), JA(I) and A(I), I = 1, NELT.  The format
C       for  these lines is   :  1X,I5,1X,I5,1X,E16.7.   Then  comes
C       RHS(I), I = 1, N, if IRHS = 1.  Then  comes SOLN(I), I  = 1,
C       N, if ISOLN = 1.  The format for these lines is: 1X,E16.7.
C
C       =================== S L A P Triad format ===================
C       This routine requires that the  matrix A be   stored in  the
C       SLAP  Triad format.  In  this format only the non-zeros  are
C       stored.  They may appear in  *ANY* order.  The user supplies
C       three arrays of  length NELT, where  NELT is  the number  of
C       non-zeros in the matrix: (IA(NELT), JA(NELT), A(NELT)).  For
C       each non-zero the user puts the row and column index of that
C       matrix element  in the IA and  JA arrays.  The  value of the
C       non-zero  matrix  element is  placed   in  the corresponding
C       location of the A array.   This is  an  extremely  easy data
C       structure to generate.  On  the  other hand it   is  not too
C       efficient on vector computers for  the iterative solution of
C       linear systems.  Hence,   SLAP changes   this  input    data
C       structure to the SLAP Column format  for  the iteration (but
C       does not change it back).
C
C       Here is an example of the  SLAP Triad   storage format for a
C       5x5 Matrix.  Recall that the entries may appear in any order.
C
C           5x5 Matrix       SLAP Triad format for 5x5 matrix on left.
C                              1  2  3  4  5  6  7  8  9 10 11
C       |11 12  0  0 15|   A: 51 12 11 33 15 53 55 22 35 44 21
C       |21 22  0  0  0|  IA:  5  1  1  3  1  5  5  2  3  4  2
C       | 0  0 33  0 35|  JA:  1  2  1  3  5  3  5  2  5  4  1
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  STOUT
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, JOB
      REAL    A(NELT), RHS(N), SOLN(N)
C
C         Local variables.
C
      INTEGER IRHS, ISOLN, I
C
C         If RHS and SOLN are to be printed also.
C         Write out the information heading.
C***FIRST EXECUTABLE STATEMENT  STOUT
      IRHS = 0
      ISOLN = 0
      IF( JOB.EQ.1 .OR. JOB.EQ.3 ) IRHS = 1
      IF( JOB.GT.1 ) ISOLN = 1
      WRITE(IUNIT,1000) N, NELT, ISYM, IRHS, ISOLN
C
C         Write out the matrix non-zeros in Triad format.
      DO 10 I = 1, NELT
         WRITE(IUNIT,1010) IA(I), JA(I), A(I)
 10   CONTINUE
C
C         If requested, write out the rhs.
      IF( IRHS.EQ.1 ) THEN
         WRITE(IUNIT,1020) (RHS(I),I=1,N)
      ENDIF
C
C         If requested, write out the soln.
      IF( ISOLN.EQ.1 ) THEN
         WRITE(IUNIT,1020) (SOLN(I),I=1,N)
      ENDIF
      RETURN
 1000 FORMAT(5I10)
 1010 FORMAT(1X,I5,1X,I5,1X,E16.7)
 1020 FORMAT(1X,E16.7)
C------------- LAST LINE OF STOUT FOLLOWS ----------------------------
      END
*DECK STIN
      SUBROUTINE STIN( N, NELT, IA, JA, A, ISYM, SOLN, RHS,
     $     IUNIT, JOB )
C***BEGIN PROLOGUE  STIN
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(STIN-S),
C             Linear system, SLAP Sparse, Diagnostics
C***AUTHOR  Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  Read in SLAP Triad Format Linear System.
C            Routine to read in a SLAP Triad format matrix and
C            right hand side and solution to the system, if known.
C***DESCRIPTION
C *Usage:
C     INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IUNIT, JOB
C     REAL    A(NELT), SOLN(N), RHS(N)
C
C     CALL STIN( N, NELT, IA, JA, A, ISYM, SOLN, RHS, IUNIT, JOB )
C
C *Arguments:
C N      :OUT      Integer
C         Order of the Matrix.
C NELT   :INOUT    Integer.
C         On input NELT is the maximum number of non-zeros that
C         can be stored in the IA, JA, A arrays.
C         On output NELT is the number of non-zeros stored in A.
C IA     :OUT      Integer IA(NELT).
C JA     :OUT      Integer JA(NELT).
C A      :OUT      Real A(NELT).
C         On output these arrays hold the matrix A in the SLAP
C         Triad format.  See "LONG DESCRIPTION", below.
C ISYM   :OUT      Integer.
C         Flag to indicate symmetric storage format.
C         If ISYM=0, all nonzero entries of the matrix are stored.
C         If ISYM=1, the matrix is symmetric, and only the lower
C         triangle of the matrix is stored.
C SOLN   :OUT      Real SOLN(N).
C         The solution to the linear system, if present.  This array
C         is accessed if and only if JOB to read it in, see below.
C         If the user requests that SOLN be read in, but it is not in
C         the file, then it is simply zeroed out.
C RHS    :OUT      Real RHS(N).
C         The right hand side vector.  This array is accessed if and
C         only if JOB is set to read it in, see below.
C         If the user requests that RHS be read in, but it is not in
C         the file, then it is simply zeroed out.
C IUNIT  :IN       Integer.
C         Fortran logical I/O device unit number to write the matrix
C         to.  This unit must be connected in a system dependent fashion
C         to a file or the console or you will get a nasty message
C         from the Fortran I/O libraries.
C JOB    :INOUT    Integer.
C         Flag indicating what I/O operations to perform.
C         On input JOB indicates what Input operations to try to
C         perform.
C         JOB = 0 => Read only the matrix.
C             = 1 => Read matrix and RHS (if present).
C             = 2 => Read matrix and SOLN (if present).
C             = 3 => Read matrix, RHS and SOLN (if present).
C         On output JOB indicates what operations were actually
C         performed.
C         JOB = 0 => Read in only the matrix.
C             = 1 => Read in the matrix and RHS.
C             = 2 => Read in the matrix and SOLN.
C             = 3 => Read in the matrix, RHS and SOLN.
C
C *Precision:           Single Precision
C *Portability:
C         You must make sure that IUNIT is a valid Fortran logical
C         I/O device unit number and that the unit number has been
C         associated with a file or the console.  This is a system
C         dependent function.
C
C***LONG DESCRIPTION
C       The format for the output is as follows.  On  the first line
C       are counters and flags: N, NELT, ISYM, IRHS, ISOLN.  N, NELT
C       and ISYM are described above.  IRHS is  a flag indicating if
C       the RHS was  written out (1 is  yes, 0 is  no).  ISOLN  is a
C       flag indicating if the SOLN was written out  (1 is yes, 0 is
C       no).  The format for the fist line is: 5i10.  Then comes the
C       NELT Triad's IA(I), JA(I) and A(I), I = 1, NELT.  The format
C       for  these lines is   :  1X,I5,1X,I5,1X,E16.7.   Then  comes
C       RHS(I), I = 1, N, if IRHS = 1.  Then  comes SOLN(I), I  = 1,
C       N, if ISOLN = 1.  The format for these lines is: 1X,E16.7.
C
C       =================== S L A P Triad format ===================
C       This routine requires that the  matrix A be   stored in  the
C       SLAP  Triad format.  In  this format only the non-zeros  are
C       stored.  They may appear in  *ANY* order.  The user supplies
C       three arrays of  length NELT, where  NELT is  the number  of
C       non-zeros in the matrix: (IA(NELT), JA(NELT), A(NELT)).  For
C       each non-zero the user puts the row and column index of that
C       matrix element  in the IA and  JA arrays.  The  value of the
C       non-zero  matrix  element is  placed   in  the corresponding
C       location of the A array.   This is  an  extremely  easy data
C       structure to generate.  On  the  other hand it   is  not too
C       efficient on vector computers for  the iterative solution of
C       linear systems.  Hence,   SLAP changes   this  input    data
C       structure to the SLAP Column format  for  the iteration (but
C       does not change it back).
C
C       Here is an example of the  SLAP Triad   storage format for a
C       5x5 Matrix.  Recall that the entries may appear in any order.
C
C           5x5 Matrix       SLAP Triad format for 5x5 matrix on left.
C                              1  2  3  4  5  6  7  8  9 10 11
C       |11 12  0  0 15|   A: 51 12 11 33 15 53 55 22 35 44 21
C       |21 22  0  0  0|  IA:  5  1  1  3  1  5  5  2  3  4  2
C       | 0  0 33  0 35|  JA:  1  2  1  3  5  3  5  2  5  4  1
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  STIN
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, JOB
      REAL    A(NELT), RHS(N), SOLN(N)
C
C         Local variables.
C
      INTEGER IRHS, ISOLN, I, NELTMAX
C
C         Read in the information heading.
C***FIRST EXECUTABLE STATEMENT  STIN
      NELTMAX = NELT
      READ(IUNIT,1000) N, NELT, ISYM, IRHS, ISOLN
      NELT = MIN( NELT, NELTMAX )
C
C         Read in the matrix non-zeros in Triad format.
      DO 10 I = 1, NELT
         READ(IUNIT,1010) IA(I), JA(I), A(I)
 10   CONTINUE
C
C         If requested, read in the rhs.
      JOBRET = 0
      IF( JOB.EQ.1 .OR. JOB.EQ.3 ) THEN
C
C         Check to see if rhs is in the file.
         IF( IRHS.EQ.1 ) THEN
            JOBRET = 1
            READ(IUNIT,1020) (RHS(I),I=1,N)
         ELSE
            DO 20 I = 1, N
               RHS(I) = 0.0
 20         CONTINUE
         ENDIF
      ENDIF
C
C         If requested, read in the soln.
      IF( JOB.GT.1 ) THEN
C
C         Check to see if soln is in the file.
         IF( ISOLN.EQ.1 ) THEN
            JOBRET = JOBRET + 2
            READ(IUNIT,1020) (SOLN(I),I=1,N)
         ELSE
            DO 30 I = 1, N
               SOLN(I) = 0.0
 30         CONTINUE
         ENDIF
      ENDIF
C
      JOB = JOBRET
      RETURN
 1000 FORMAT(5I10)
 1010 FORMAT(1X,I5,1X,I5,1X,E16.7)
 1020 FORMAT(1X,E16.7)
C------------- LAST LINE OF STIN FOLLOWS ----------------------------
      END
