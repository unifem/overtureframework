c@process directive('*vdir:')
      SUBROUTINE SSMV( N, X, Y, NELT, IA, JA, A, ISYM )
C***BEGIN PROLOGUE  SSMV
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSMV-S),
C             Matrix Vector Multiply, Sparse
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP Column Format Sparse Matrix Vector Product.
C            Routine to calculate the sparse matrix vector product:
C            Y = A*X.
C***DESCRIPTION
C *Usage:
C     INTEGER  N, NELT, IA(NELT), JA(N+1), ISYM
C     REAL     X(N), Y(N), A(NELT)
C
C     CALL SSMV(N, X, Y, NELT, IA, JA, A, ISYM )
C
C *Arguments:
C N      :IN       Integer.
C         Order of the Matrix.
C X      :IN       Real X(N).
C         The vector that should be multiplied by the matrix.
C Y      :OUT      Real Y(N).
C         The product of the matrix and the vector.
C NELT   :IN       Integer.
C         Number of Non-Zeros stored in A.
C IA     :IN       Integer IA(NELT).
C JA     :IN       Integer JA(N+1).
C A      :IN       Integer A(NELT).
C         These arrays should hold the matrix A in the SLAP Column
C         format.  See "Description", below.
C ISYM   :IN       Integer.
C         Flag to indicate symmetric storage format.
C         If ISYM=0, all nonzero entries of the matrix are stored.
C         If ISYM=1, the matrix is symmetric, and only the upper
C         or lower triangle of the matrix is stored.
C
C *Description
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
C       With  the SLAP  format  the "inner  loops" of  this  routine
C       should vectorize   on machines with   hardware  support  for
C       vector gather/scatter operations.  Your compiler may require
C       a  compiler directive  to  convince   it that there  are  no
C       implicit vector  dependencies.  Compiler directives  for the
C       Alliant FX/Fortran and CRI CFT/CFT77 compilers  are supplied
C       with the standard SLAP distribution.
C
C *Precision:           Single Precision
C *Cautions:
C     This   routine   assumes  that  the matrix A is stored in SLAP
C     Column format.  It does not check  for  this (for  speed)  and
C     evil, ugly, ornery and nasty things  will happen if the matrix
C     data  structure  is,  in fact, not SLAP Column.  Beware of the
C     wrong data structure!!!
C
C *See Also:
C       SSMTV
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SSMV
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM
      REAL    A(NELT), X(N), Y(N)
C
C         Zero out the result vector.
C***FIRST EXECUTABLE STATEMENT  SSMV
      DO 10 I = 1, N
         Y(I) = 0.0
 10   CONTINUE
C
C         Multiply by A.
C
CVD$R NOCONCUR
      DO 30 ICOL = 1, N
         IBGN = JA(ICOL)
         IEND = JA(ICOL+1)-1
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NODEPCHK
C*VDIR:  IGNORE RECRDEPS(Y)
         DO 20 I = IBGN, IEND
            Y(IA(I)) = Y(IA(I)) + A(I)*X(ICOL)
 20      CONTINUE
 30   CONTINUE
C
      IF( ISYM.EQ.1 ) THEN
C
C         The matrix is non-symmetric.  Need to get the other half in...
C         This loops assumes that the diagonal is the first entry in
C         each column.
C
         DO 50 IROW = 1, N
            JBGN = JA(IROW)+1
            JEND = JA(IROW+1)-1
            IF( JBGN.GT.JEND ) GOTO 50
            DO 40 J = JBGN, JEND
               Y(IROW) = Y(IROW) + A(J)*X(IA(J))
 40         CONTINUE
 50      CONTINUE
      ENDIF
      RETURN
C------------- LAST LINE OF SSMV FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSMTV( N, X, Y, NELT, IA, JA, A, ISYM )
C***BEGIN PROLOGUE  SSMTV
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSMTV-S),
C             Matrix transpose Vector Multiply, Sparse
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP Column Format Sparse Matrix (transpose) Vector Prdt.
C            Routine to calculate the sparse matrix vector product:
C            Y = A'*X, where ' denotes transpose.
C***DESCRIPTION
C *Usage:
C     INTEGER  N, NELT, IA(NELT), JA(N+1), ISYM
C     REAL     X(N), Y(N), A(NELT)
C
C     CALL SSMTV(N, X, Y, NELT, IA, JA, A, ISYM )
C
C *Arguments:
C N      :IN       Integer.
C         Order of the Matrix.
C X      :IN       Real X(N).
C         The vector that should be multiplied by the transpose of
C         the matrix.
C Y      :OUT      Real Y(N).
C         The product of the transpose of the matrix and the vector.
C NELT   :IN       Integer.
C         Number of Non-Zeros stored in A.
C IA     :IN       Integer IA(NELT).
C JA     :IN       Integer JA(N+1).
C A      :IN       Integer A(NELT).
C         These arrays should hold the matrix A in the SLAP Column
C         format.  See "Description", below.
C ISYM   :IN       Integer.
C         Flag to indicate symmetric storage format.
C         If ISYM=0, all nonzero entries of the matrix are stored.
C         If ISYM=1, the matrix is symmetric, and only the upper
C         or lower triangle of the matrix is stored.
C
C *Description
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
C       With  the SLAP  format  the "inner  loops" of  this  routine
C       should vectorize   on machines with   hardware  support  for
C       vector gather/scatter operations.  Your compiler may require
C       a  compiler directive  to  convince   it that there  are  no
C       implicit vector  dependencies.  Compiler directives  for the
C       Alliant FX/Fortran and CRI CFT/CFT77 compilers  are supplied
C       with the standard SLAP distribution.
C
C *Precision:           Single Precision
C *Cautions:
C     This   routine   assumes  that  the matrix A is stored in SLAP
C     Column format.  It does not check  for  this (for  speed)  and
C     evil, ugly, ornery and nasty things  will happen if the matrix
C     data  structure  is,  in fact, not SLAP Column.  Beware of the
C     wrong data structure!!!
C
C *See Also:
C       SSMV
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SSMTV
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM
      REAL    X(N), Y(N), A(NELT)
C
C         Zero out the result vector.
C***FIRST EXECUTABLE STATEMENT  SSMTV
      DO 10 I = 1, N
         Y(I) = 0.0
 10   CONTINUE
C
C         Multiply by A-Transpose.
C         A-Transpose is stored by rows...
CVD$R NOCONCUR
      DO 30 IROW = 1, N
         IBGN = JA(IROW)
         IEND = JA(IROW+1)-1
CVD$ ASSOC
c*vdir: ignore recrdeps(y)
         DO 20 I = IBGN, IEND
            Y(IROW) = Y(IROW) + A(I)*X(IA(I))
 20      CONTINUE
 30   CONTINUE
C
      IF( ISYM.EQ.1 ) THEN
C
C         The matrix is non-symmetric.  Need to get the other half in...
C         This loops assumes that the diagonal is the first entry in
C         each column.
C
         DO 50 ICOL = 1, N
            JBGN = JA(ICOL)+1
            JEND = JA(ICOL+1)-1
            IF( JBGN.GT.JEND ) GOTO 50
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NODEPCHK
c*vdir: ignore recrdeps(y)
            DO 40 J = JBGN, JEND
               Y(IA(J)) = Y(IA(J)) + A(J)*X(ICOL)
 40         CONTINUE
 50      CONTINUE
      ENDIF
      RETURN
C------------- LAST LINE OF SSMTV FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSDI(N, B, X, NELT, IA, JA, A, ISYM, RWORK, IWORK)
C***BEGIN PROLOGUE  SSDI
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213  (YYMMDD)
C***CATEGORY NO.  D2A4, D2B4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSDI-S),
C             Linear system solve, Sparse, Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  Diagonal Matrix Vector Multiply.
C            Routine to calculate the product  X = DIAG*B,
C            where DIAG is a diagonal matrix.
C***DESCRIPTION
C *Usage:
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C B      :IN       Real B(N).
C         Vector to multiply the diagonal by.
C X      :OUT      Real X(N).
C         Result of DIAG*B.
C NELT   :DUMMY    Integer.
C         Retained for compatibility with SLAP MSOLVE calling sequence.
C IA     :DUMMY    Integer IA(NELT).
C         Retained for compatibility with SLAP MSOLVE calling sequence.
C JA     :DUMMY    Integer JA(N+1).
C         Retained for compatibility with SLAP MSOLVE calling sequence.
C  A     :DUMMY    Real A(NELT).
C         Retained for compatibility with SLAP MSOLVE calling sequence.
C ISYM   :DUMMY    Integer.
C         Retained for compatibility with SLAP MSOLVE calling sequence.
C RWORK  :IN       Real RWORK(USER DEFINABLE).
C         Work array holding the diagonal of some matrix to scale
C         B by.  This array must be set by the user or by a call
C         to the slap routine SSDS or SSD2S.  The length of RWORK
C         must be > IWORK(4)+N.
C IWORK  :IN       Integer IWORK(10).
C         IWORK(4) holds the offset into RWORK for the diagonal matrix
C         to scale B by.  This is usually set up by the SLAP pre-
C         conditioner setup routines SSDS or SSD2S.
C
C *Description:
C         This routine is supplied with the SLAP package to perform
C         the  MSOLVE  operation for iterative drivers that require
C         diagonal  Scaling  (e.g., SSDCG, SSDBCG).   It  conforms
C         to the SLAP MSOLVE CALLING CONVENTION  and hence does not
C         require an interface routine as do some of the other pre-
C         conditioners supplied with SLAP.
C
C *Precision:           Single Precision
C *See Also:
C       SSDS, SSD2S
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SSDI
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IWORK(10)
      REAL    B(N), X(N), A(NELT), RWORK(1)
C
C         Determine where the inverse of the diagonal
C         is in the work array and then scale by it.
C***FIRST EXECUTABLE STATEMENT  SSDI
      LOCD = IWORK(4) - 1
      DO 10 I = 1, N
         X(I) = RWORK(LOCD+I)*B(I)
 10   CONTINUE
      RETURN
C------------- LAST LINE OF SSDI FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSLI(N, B, X, NELT, IA, JA, A, ISYM, RWORK, IWORK )
C***BEGIN PROLOGUE  SSLI
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSLI-S),
C             Linear system solve, Sparse, Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP MSOLVE for Lower Triangle Matrix.
C            This routine acts as an interface between the SLAP generic
C            MSOLVE calling convention and the routine that actually
C                      -1
C            computes L  B = X.
C
C *Description
C       See the Description of SLLI2 for the gory details.
C***ROUTINES CALLED  SLLI2
C***END PROLOGUE  SSLI
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IWORK(10)
      REAL    B(N), X(N), A(NELT), RWORK(1)
C***FIRST EXECUTABLE STATEMENT  SSLI
C
      NEL = IWORK(1)
      LOCIEL = IWORK(2)
      LOCJEL = IWORK(3)
      LOCEL = IWORK(4)
      CALL SSLI2(N, B, X, NEL, IWORK(LOCIEL), IWORK(LOCJEL),
     $     RWORK(LOCEL))
C
      RETURN
C------------- LAST LINE OF SSLI FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSLI2(N, B, X, NEL, IEL, JEL, EL)
C***BEGIN PROLOGUE  SSLI2
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSLI2-S),
C             Linear system solve, Sparse, Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP for Lower Triangle Matrix Backsolve.
C            Routine to solve a system of the form  Lx = b , where
C            L is a lower triangular matrix.
C***DESCRIPTION
C *Usage:
C     INTEGER N,  NEL, IEL(N+1), JEL(NEL)
C     REAL    B(N), X(N), EL(NEL)
C
C     CALL SSLI2( N, B, X, NEL, IEL, JEL, EL )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C B      :IN       Real B(N).
C         Right hand side vector.
C X      :OUT      Real X(N).
C         Solution to Lx = b.
C NEL    :IN       Integer.
C         Number of non-zeros in the EL array.
C IEL    :IN       Integer IEL(N+1).
C JEL    :IN       Integer JEL(NEL).
C EL     :IN       Real     EL(NEL).
C         IEL, JEL, EL contain the unit lower triangular factor   of
C         the incomplete decomposition   of the A  matrix  stored in
C         SLAP Row format.  The diagonal of  ones *IS* stored.  This
C         structure can be set up by the  SS2LT  routine.  See "LONG
C         DESCRIPTION", below for more details about  the  SLAP  Row
C         format.
C
C *Description:
C       This routine is supplied with the SLAP package  as a routine
C       to  perform the  MSOLVE operation in  the SIR for the driver
C       routine SSGS.  It must be called via the SLAP MSOLVE calling
C       sequence convention interface routine SSLI.
C         **** THIS ROUTINE ITSELF DOES NOT CONFORM TO THE ****
C               **** SLAP MSOLVE CALLING CONVENTION ****
C
C       ==================== S L A P Row format ====================
C       This routine requires  that the matrix A  be  stored  in the
C       SLAP  Row format.   In this format  the non-zeros are stored
C       counting across  rows (except for the diagonal  entry, which
C       must appear first in each "row") and  are stored in the real
C       array A.  In other words, for each row in the matrix put the
C       diagonal entry in  A.   Then   put  in the   other  non-zero
C       elements   going  across the  row (except   the diagonal) in
C       order.   The  JA array  holds   the column   index for  each
C       non-zero.   The IA  array holds the  offsets into  the JA, A
C       arrays  for   the   beginning  of   each  row.   That    is,
C       JA(IA(IROW)),  A(IA(IROW)) points  to  the beginning  of the
C       IROW-th row in JA and A.   JA(IA(IROW+1)-1), A(IA(IROW+1)-1)
C       points to the  end of the  IROW-th row.  Note that we always
C       have IA(N+1) =  NELT+1, where  N  is  the number of rows  in
C       the matrix  and NELT  is the  number   of  non-zeros in  the
C       matrix.
C
C       Here is an example of the SLAP Row storage format for a  5x5
C       Matrix (in the A and JA arrays '|' denotes the end of a row):
C
C           5x5 Matrix         SLAP Row format for 5x5 matrix on left.
C                              1  2  3    4  5    6  7    8    9 10 11
C       |11 12  0  0 15|   A: 11 12 15 | 22 21 | 33 35 | 44 | 55 51 53
C       |21 22  0  0  0|  JA:  1  2  5 |  2  1 |  3  5 |  4 |  5  1  3
C       | 0  0 33  0 35|  IA:  1  4  6    8  9   12
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C       With  the SLAP  Row format  the "inner loop" of this routine
C       should vectorize   on machines with   hardware  support  for
C       vector gather/scatter operations.  Your compiler may require
C       a  compiler directive  to  convince   it that there  are  no
C       implicit vector  dependencies.  Compiler directives  for the
C       Alliant FX/Fortran and CRI CFT/CFT77 compilers  are supplied
C       with the standard SLAP distribution.
C
C *Precision: Single Precision
C *See Also:
C         SSLI
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SSLI2
      INTEGER N, NEL, IEL(NEL), JEL(NEL)
      REAL    B(N), X(N), EL(NEL)
C
C         Initialize the solution by copying the right hands side
C         into it.
C***FIRST EXECUTABLE STATEMENT  SSLI2
      DO 10 I=1,N
         X(I) = B(I)
 10   CONTINUE
C
CVD$ NOCONCUR
      DO 30 ICOL = 1, N
         X(ICOL) = X(ICOL)/EL(JEL(ICOL))
         JBGN = JEL(ICOL) + 1
         JEND = JEL(ICOL+1) - 1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NOCONCUR
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 20 J = JBGN, JEND
               X(IEL(J)) = X(IEL(J)) - EL(J)*X(ICOL)
 20         CONTINUE
         ENDIF
 30   CONTINUE
C
      RETURN
C------------- LAST LINE OF SSLI2 FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSLLTI(N, B, X, NELT, IA, JA, A, ISYM, RWORK, IWORK)
C***BEGIN PROLOGUE  SSLLTI
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSLLTI-S),
C             Linear system solve, Sparse, Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP MSOLVE for LDL' (IC) Factorization.
C            This routine acts as an interface between the SLAP generic
C            MSOLVE calling convention and the routine that actually
C                           -1
C            computes (LDL')  B = X.
C***DESCRIPTION
C       See the DESCRIPTION of SLLTI2 for the gory details.
C***ROUTINES CALLED SLLTI2
C
C***END PROLOGUE  SSLLTI
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IWORK(*)
      REAL    B(1), X(1), A(NELT), RWORK(1)
C
C***FIRST EXECUTABLE STATEMENT  SSLLTI
      NEL = IWORK(1)
      LOCIEL = IWORK(3)
      LOCJEL = IWORK(2)
      LOCEL  = IWORK(4)
      LOCDIN = IWORK(5)
      CALL SLLTI2(N, B, X, NEL, IWORK(LOCIEL), IWORK(LOCJEL),
     $     RWORK(LOCEL), RWORK(LOCDIN))
C
      RETURN
C------------- LAST LINE OF SSLLTI FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SLLTI2(N, B, X, NEL, IEL, JEL, EL, DINV)
C***BEGIN PROLOGUE  SLLTI2
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SLLTI2-S),
C             Symmetric Linear system solve, Sparse,
C             Iterative Precondition, Incomplete Factorization
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP back solve routine for LDL' Factorization.
C            Routine to solve a system of the  form  L*D*L' X  =  B,
C            where L is a unit lower triangular  matrix  and  D is a
C            diagonal matrix and ' means transpose.
C***DESCRIPTION
C *Usage:
C     INTEGER N,  NEL, IEL(N+1), JEL(NEL)
C     REAL    B(N), X(N), EL(NEL), DINV(N)
C
C     CALL SLLTI2( N, B, X, NEL, IEL, JEL, EL, DINV )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C B      :IN       Real B(N).
C         Right hand side vector.
C X      :OUT      Real X(N).
C         Solution to L*D*L' x = b.
C NEL    :IN       Integer.
C         Number of non-zeros in the EL array.
C IEL    :IN       Integer IEL(N+1).
C JEL    :IN       Integer JEL(NEL).
C EL     :IN       Real     EL(NEL).
C         IEL, JEL, EL contain the unit lower triangular factor   of
C         the incomplete decomposition   of the A  matrix  stored in
C         SLAP Row format.   The diagonal of ones *IS* stored.  This
C         structure can be set  up  by  the SS2LT routine. See
C         "Description", below for more details about the   SLAP Row
C         format.
C DINV   :IN       Real DINV(N).
C         Inverse of the diagonal matrix D.
C
C *Description:
C       This routine is supplied with  the SLAP package as a routine
C       to perform the MSOLVE operation in the SCG iteration routine
C       for  the driver  routine SSICCG.   It must be called via the
C       SLAP  MSOLVE calling sequence  convention  interface routine
C       SSLLI.
C         **** THIS ROUTINE ITSELF DOES NOT CONFORM TO THE ****
C               **** SLAP MSOLVE CALLING CONVENTION ****
C
C       IEL, JEL, EL should contain the unit lower triangular factor
C       of  the incomplete decomposition of  the A matrix  stored in
C       SLAP Row format.   This IC factorization  can be computed by
C       the  SSICS routine.  The  diagonal  (which is all one's) is
C       stored.
C
C       ==================== S L A P Row format ====================
C       This routine requires  that the matrix A  be  stored  in the
C       SLAP  Row format.   In this format  the non-zeros are stored
C       counting across  rows (except for the diagonal  entry, which
C       must appear first in each "row") and  are stored in the real
C       array A.  In other words, for each row in the matrix put the
C       diagonal entry in  A.   Then   put  in the   other  non-zero
C       elements   going  across the  row (except   the diagonal) in
C       order.   The  JA array  holds   the column   index for  each
C       non-zero.   The IA  array holds the  offsets into  the JA, A
C       arrays  for   the   beginning  of   each  row.   That    is,
C       JA(IA(IROW)),  A(IA(IROW)) points  to  the beginning  of the
C       IROW-th row in JA and A.   JA(IA(IROW+1)-1), A(IA(IROW+1)-1)
C       points to the  end of the  IROW-th row.  Note that we always
C       have IA(N+1) =  NELT+1, where  N  is  the number of rows  in
C       the matrix  and NELT  is the  number   of  non-zeros in  the
C       matrix.
C
C       Here is an example of the SLAP Row storage format for a  5x5
C       Matrix (in the A and JA arrays '|' denotes the end of a row):
C
C           5x5 Matrix         SLAP Row format for 5x5 matrix on left.
C                              1  2  3    4  5    6  7    8    9 10 11
C       |11 12  0  0 15|   A: 11 12 15 | 22 21 | 33 35 | 44 | 55 51 53
C       |21 22  0  0  0|  JA:  1  2  5 |  2  1 |  3  5 |  4 |  5  1  3
C       | 0  0 33  0 35|  IA:  1  4  6    8  9   12
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C       With  the SLAP  Row format  the "inner loop" of this routine
C       should vectorize   on machines with   hardware  support  for
C       vector gather/scatter operations.  Your compiler may require
C       a  compiler directive  to  convince   it that there  are  no
C       implicit vector  dependencies.  Compiler directives  for the
C       Alliant FX/Fortran and CRI CFT/CFT77 compilers  are supplied
C       with the standard SLAP distribution.
C
C *Precision:           Single Precision
C *See Also:
C       SSICCG, SSICS
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SLLTI2
      INTEGER N, NEL, IEL(NEL), JEL(1)
      REAL    B(N), X(N), EL(NEL), DINV(N)
C
C         solve  l*y = b,  storing result in x.
C***FIRST EXECUTABLE STATEMENT  SLLTI2
      DO 10 I=1,N
         X(I) = B(I)
 10   CONTINUE
      DO 30 IROW = 1, N
         IBGN = IEL(IROW) + 1
         IEND = IEL(IROW+1) - 1
         IF( IBGN.LE.IEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NOCONCUR
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 20 I = IBGN, IEND
               X(IROW) = X(IROW) - EL(I)*X(JEL(I))
 20         CONTINUE
         ENDIF
 30   CONTINUE
C
C         Solve  D*Z = Y,  storing result in X.
C
      DO 40 I=1,N
         X(I) = X(I)*DINV(I)
 40   CONTINUE
C
C         Solve  L-trans*X = Z.
C
      DO 60 IROW = N, 2, -1
         IBGN = IEL(IROW) + 1
         IEND = IEL(IROW+1) - 1
         IF( IBGN.LE.IEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NOCONCUR
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 50 I = IBGN, IEND
               X(JEL(I)) = X(JEL(I)) - EL(I)*X(IROW)
 50         CONTINUE
         ENDIF
 60   CONTINUE
C
      RETURN
C------------- LAST LINE OF SLTI2 FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSLUI(N, B, X, NELT, IA, JA, A, ISYM, RWORK, IWORK)
C***BEGIN PROLOGUE  SSLUI
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSLUI-S),
C             Non-Symmetric Linear system solve, Sparse,
C             Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP MSOLVE for LDU Factorization.
C            This routine  acts as an  interface between  the   SLAP
C            generic MSLOVE calling convention and the routine  that
C            actually computes:     -1
C                              (LDU)  B = X.
C***DESCRIPTION
C       See the "DESCRIPTION" of SSLUI2 for the gory details.
C***ROUTINES CALLED  SSLUI2
C***END PROLOGUE  SSLUI
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IWORK(10)
      REAL    B(N), X(N), A(NELT), RWORK(1)
C
C         Pull out the locations of the arrays holding the ILU
C         factorization.
C***FIRST EXECUTABLE STATEMENT  SSLUI
      LOCIL = IWORK(1)
      LOCJL = IWORK(2)
      LOCIU = IWORK(3)
      LOCJU = IWORK(4)
      LOCL = IWORK(5)
      LOCDIN = IWORK(6)
      LOCU = IWORK(7)
C
C         Solve the system LUx = b
      CALL SSLUI2(N, B, X, IWORK(LOCIL), IWORK(LOCJL), RWORK(LOCL),
     $     RWORK(LOCDIN), IWORK(LOCIU), IWORK(LOCJU), RWORK(LOCU) )
C
      RETURN
C------------- LAST LINE OF SSLUI FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSLUI2(N, B, X, IL, JL, L, DINV, IU, JU, U )
C***BEGIN PROLOGUE  SSLUI2
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSLUI2-S),
C             Non-Symmetric Linear system solve, Sparse,
C             Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP Back solve for LDU Factorization.
C            Routine  to  solve a system of the form  L*D*U X  =  B,
C            where L is a unit  lower  triangular  matrix,  D  is  a
C            diagonal matrix, and U is a unit upper triangular matrix.
C***DESCRIPTION
C *Usage:
C     INTEGER N, IL(N+1), JL(NL), IU(NU), JU(N+1)
C     REAL    B(N), X(N), L(NL), DINV(N), U(NU)
C
C     CALL SSLUI2( N, B, X, IL, JL, L, DINV, IU, JU, U )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C B      :IN       Real B(N).
C         Right hand side.
C X      :OUT      Real X(N).
C         Solution of L*D*U x = b.
C NEL    :IN       Integer.
C         Number of non-zeros in the EL array.
C IL     :IN       Integer IL(N+1).
C JL     :IN       Integer JL(NL).
C  L     :IN       Real     L(NL).
C         IL, JL, L contain the unit  lower triangular factor of the
C         incomplete decomposition of some matrix stored in SLAP Row
C         format.  The diagonal of ones *IS* stored.  This structure
C         can   be   set up  by   the  SSILUS routine.   See
C         "DESCRIPTION", below  for more   details about   the  SLAP
C         format.
C DINV   :IN       Real DINV(N).
C         Inverse of the diagonal matrix D.
C NU     :IN       Integer.
C         Number of non-zeros in the U array.
C IU     :IN       Integer IU(N+1).
C JU     :IN       Integer JU(NU).
C U      :IN       Real     U(NU).
C         IU, JU, U contain the unit upper triangular factor  of the
C         incomplete decomposition  of  some  matrix stored in  SLAP
C         Column format.   The diagonal of ones  *IS* stored.   This
C         structure can be set up  by the SSILUS routine.  See
C         "DESCRIPTION", below   for  more   details about  the SLAP
C         format.
C
C *Description:
C       This routine is supplied with  the SLAP package as a routine
C       to  perform  the  MSOLVE operation  in   the  SIR and   SBCG
C       iteration routines for  the  drivers SSILUR and SSLUBC.   It
C       must  be called  via   the  SLAP  MSOLVE  calling   sequence
C       convention interface routine SSLUI.
C         **** THIS ROUTINE ITSELF DOES NOT CONFORM TO THE ****
C               **** SLAP MSOLVE CALLING CONVENTION ****
C
C       IL, JL, L should contain the unit lower triangular factor of
C       the incomplete decomposition of the A matrix  stored in SLAP
C       Row format.  IU, JU, U should contain  the unit upper factor
C       of the  incomplete decomposition of  the A matrix  stored in
C       SLAP Column format This ILU factorization can be computed by
C       the SSILUS routine.  The diagonals (which is all one's) are
C       stored.
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
C       ==================== S L A P Row format ====================
C       This routine requires  that the matrix A  be  stored  in the
C       SLAP  Row format.   In this format  the non-zeros are stored
C       counting across  rows (except for the diagonal  entry, which
C       must appear first in each "row") and  are stored in the real
C       array A.  In other words, for each row in the matrix put the
C       diagonal entry in  A.   Then   put  in the   other  non-zero
C       elements   going  across the  row (except   the diagonal) in
C       order.   The  JA array  holds   the column   index for  each
C       non-zero.   The IA  array holds the  offsets into  the JA, A
C       arrays  for   the   beginning  of   each  row.   That    is,
C       JA(IA(IROW)),  A(IA(IROW)) points  to  the beginning  of the
C       IROW-th row in JA and A.   JA(IA(IROW+1)-1), A(IA(IROW+1)-1)
C       points to the  end of the  IROW-th row.  Note that we always
C       have IA(N+1) =  NELT+1, where  N  is  the number of rows  in
C       the matrix  and NELT  is the  number   of  non-zeros in  the
C       matrix.
C
C       Here is an example of the SLAP Row storage format for a  5x5
C       Matrix (in the A and JA arrays '|' denotes the end of a row):
C
C           5x5 Matrix         SLAP Row format for 5x5 matrix on left.
C                              1  2  3    4  5    6  7    8    9 10 11
C       |11 12  0  0 15|   A: 11 12 15 | 22 21 | 33 35 | 44 | 55 51 53
C       |21 22  0  0  0|  JA:  1  2  5 |  2  1 |  3  5 |  4 |  5  1  3
C       | 0  0 33  0 35|  IA:  1  4  6    8  9   12
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C       With  the SLAP  format  the "inner  loops" of  this  routine
C       should vectorize   on machines with   hardware  support  for
C       vector gather/scatter operations.  Your compiler may require
C       a  compiler directive  to  convince   it that there  are  no
C       implicit vector  dependencies.  Compiler directives  for the
C       Alliant FX/Fortran and CRI CFT/CFT77 compilers  are supplied
C       with the standard SLAP distribution.
C
C *Precision:           Single Precision
C *See Also:
C       SSILUS
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SSLUI2
      INTEGER N, IL(1), JL(1), IU(1), JU(1)
      REAL    B(N), X(N), L(1), DINV(N), U(1)
C
C         Solve  L*Y = B,  storing result in X, L stored by rows.
C***FIRST EXECUTABLE STATEMENT  SSLUI2
      DO 10 I = 1, N
         X(I) = B(I)
 10   CONTINUE
      DO 30 IROW = 2, N
         JBGN = IL(IROW)
         JEND = IL(IROW+1)-1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ ASSOC
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 20 J = JBGN, JEND
               X(IROW) = X(IROW) - L(J)*X(JL(J))
 20         CONTINUE
         ENDIF
 30   CONTINUE
C
C         Solve  D*Z = Y,  storing result in X.
      DO 40 I=1,N
         X(I) = X(I)*DINV(I)
 40   CONTINUE
C
C         Solve  U*X = Z, U stored by columns.
      DO 60 ICOL = N, 2, -1
         JBGN = JU(ICOL)
         JEND = JU(ICOL+1)-1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 50 J = JBGN, JEND
               X(IU(J)) = X(IU(J)) - U(J)*X(ICOL)
 50         CONTINUE
         ENDIF
 60   CONTINUE
C
      RETURN
C------------- LAST LINE OF SSLUI2 FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSLUTI(N, B, X, NELT, IA, JA, A, ISYM, RWORK, IWORK)
C***BEGIN PROLOGUE  SSLUTI
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSLUTI-S),
C             Linear system solve, Sparse, Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP MTSOLV for LDU Factorization.
C            This routine acts as  an  interface  between  the  SLAP
C            generic MTSOLV calling convention and  the routine that
C            actually computes:       -T
C                                (LDU)  B = X.
C***DESCRIPTION
C       See the "DESCRIPTION" of SSLUI4 for the gory details.
C***ROUTINES CALLED  SSLUI4
C***END PROLOGUE  SSLUTI
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IWORK(10)
      REAL    B(N), X(N), A(N), RWORK(1)
C
C         Pull out the pointers to the L, D and U matricies and call
C         the workhorse routine.
C***FIRST EXECUTABLE STATEMENT  SSLUTI
      LOCIL = IWORK(1)
      LOCJL = IWORK(2)
      LOCIU = IWORK(3)
      LOCJU = IWORK(4)
      LOCL = IWORK(5)
      LOCDIN = IWORK(6)
      LOCU = IWORK(7)
C
      CALL SSLUI4(N, B, X, IWORK(LOCIL), IWORK(LOCJL), RWORK(LOCL),
     $     RWORK(LOCDIN), IWORK(LOCIU), IWORK(LOCJU), RWORK(LOCU))
C
      RETURN
C------------- LAST LINE OF SSLUTI FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSLUI4(N, B, X, IL, JL, L, DINV, IU, JU, U )
C***BEGIN PROLOGUE  SSLUI4
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSLUI4-S),
C             Non-Symmetric Linear system solve, Sparse,
C             Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP back solve for LDU Factorization.
C            Routine to solve a system of the form  (L*D*U)' X =  B,
C            where L is a unit  lower  triangular  matrix,  D  is  a
C            diagonal matrix, and  U  is  a  unit  upper  triangular
C            matrix and ' denotes transpose.
C***DESCRIPTION
C *Usage:
C     INTEGER N, NL, IL(N+1), JL(NL), NU, IU(N+1), JU(NU)
C     REAL    B(N), X(N), L(NEL), DINV(N), U(NU)
C
C     CALL SSLUI4( N, B, X, IL, JL, L, DINV, IU, JU, U )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C B      :IN       Real B(N).
C         Right hand side.
C X      :OUT      Real X(N).
C         Solution of (L*D*U)trans x = b.
C IL     :IN       Integer IL(N+1).
C JL     :IN       Integer JL(NL).
C  L     :IN       Real     L(NL).
C         IL, JL, L contain the unit lower triangular  factor of the
C         incomplete decomposition of some matrix stored in SLAP Row
C         format.  The diagonal of ones *IS* stored.  This structure
C         can    be set  up  by   the  SSILUS routine.   See
C         "DESCRIPTION",  below for  more  details about  the   SLAP
C         format.
C DINV   :IN       Real DINV(N).
C         Inverse of the diagonal matrix D.
C IU     :IN       Integer IU(N+1).
C JU     :IN       Integer JU(NU).
C U      :IN       Real     U(NU).
C         IU, JU, U contain the  unit upper triangular factor of the
C         incomplete  decomposition of some  matrix stored  in  SLAP
C         Column  format.   The diagonal of  ones *IS* stored.  This
C         structure can be set up by the  SSILUS routine.  See
C         "DESCRIPTION",  below for  more  details  about  the  SLAP
C         format.
C
C *Description:
C       This routine is supplied with the SLAP package as  a routine
C       to  perform  the  MTSOLV  operation  in  the SBCG  iteration
C       routine for the  driver SSLUBC.   It must  be called via the
C       SLAP  MTSOLV calling  sequence convention interface  routine
C       SSLUTI.
C         **** THIS ROUTINE ITSELF DOES NOT CONFORM TO THE ****
C               **** SLAP MSOLVE CALLING CONVENTION ****
C
C       IL, JL, L should contain the unit lower triangular factor of
C       the incomplete decomposition of the A matrix  stored in SLAP
C       Row format.  IU, JU, U should contain  the unit upper factor
C       of the  incomplete decomposition of  the A matrix  stored in
C       SLAP Column format This ILU factorization can be computed by
C       the SSILUS routine.  The diagonals (which is all one's) are
C       stored.
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
C       ==================== S L A P Row format ====================
C       This routine requires  that the matrix A  be  stored  in the
C       SLAP  Row format.   In this format  the non-zeros are stored
C       counting across  rows (except for the diagonal  entry, which
C       must appear first in each "row") and  are stored in the real
C       array A.  In other words, for each row in the matrix put the
C       diagonal entry in  A.   Then   put  in the   other  non-zero
C       elements   going  across the  row (except   the diagonal) in
C       order.   The  JA array  holds   the column   index for  each
C       non-zero.   The IA  array holds the  offsets into  the JA, A
C       arrays  for   the   beginning  of   each  row.   That    is,
C       JA(IA(IROW)),  A(IA(IROW)) points  to  the beginning  of the
C       IROW-th row in JA and A.   JA(IA(IROW+1)-1), A(IA(IROW+1)-1)
C       points to the  end of the  IROW-th row.  Note that we always
C       have IA(N+1) =  NELT+1, where  N  is  the number of rows  in
C       the matrix  and NELT  is the  number   of  non-zeros in  the
C       matrix.
C
C       Here is an example of the SLAP Row storage format for a  5x5
C       Matrix (in the A and JA arrays '|' denotes the end of a row):
C
C           5x5 Matrix         SLAP Row format for 5x5 matrix on left.
C                              1  2  3    4  5    6  7    8    9 10 11
C       |11 12  0  0 15|   A: 11 12 15 | 22 21 | 33 35 | 44 | 55 51 53
C       |21 22  0  0  0|  JA:  1  2  5 |  2  1 |  3  5 |  4 |  5  1  3
C       | 0  0 33  0 35|  IA:  1  4  6    8  9   12
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C       With  the SLAP  format  the "inner  loops" of  this  routine
C       should vectorize   on machines with   hardware  support  for
C       vector gather/scatter operations.  Your compiler may require
C       a  compiler directive  to  convince   it that there  are  no
C       implicit vector  dependencies.  Compiler directives  for the
C       Alliant FX/Fortran and CRI CFT/CFT77 compilers  are supplied
C       with the standard SLAP distribution.
C
C *Precision:           Single Precision
C *See Also:
C       SSILUS
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SSLUI4
      INTEGER N, IL(*), JL(*), IU(*), JU(*)
      REAL    B(N), X(N), L(*), DINV(N), U(*)
C
C***FIRST EXECUTABLE STATEMENT  SSLUI4
      DO 10 I=1,N
         X(I) = B(I)
 10   CONTINUE
C
C         Solve  U'*Y = X,  storing result in X, U stored by columns.
      DO 80 IROW = 2, N
         JBGN = JU(IROW)
         JEND = JU(IROW+1) - 1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ ASSOC
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 70 J = JBGN, JEND
               X(IROW) = X(IROW) - U(J)*X(IU(J))
 70         CONTINUE
         ENDIF
 80   CONTINUE
C
C         Solve  D*Z = Y,  storing result in X.
      DO 90 I = 1, N
         X(I) = X(I)*DINV(I)
 90   CONTINUE
C
C         Solve  L'*X = Z, L stored by rows.
      DO 110 ICOL = N, 2, -1
         JBGN = IL(ICOL)
         JEND = IL(ICOL+1) - 1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 100 J = JBGN, JEND
               X(JL(J)) = X(JL(J)) - L(J)*X(ICOL)
 100        CONTINUE
         ENDIF
 110  CONTINUE
      RETURN
C------------- LAST LINE OF SSLUI4 FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSMMTI(N, B, X, NELT, IA, JA, A, ISYM, RWORK, IWORK )
C***BEGIN PROLOGUE  SSMMTI
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSMMTI-S),
C             Linear system solve, Sparse, Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP MSOLVE for LDU Factorization of Normal Equations.
C            This routine acts as  an  interface  between  the  SLAP
C            generic MMTSLV calling convention and the routine  that
C            actually computes:            -1
C                            [(LDU)*(LDU)']  B = X.
C***DESCRIPTION
C       See the "DESCRIPTION" of SSMMI2 for the gory details.
C***ROUTINES CALLED  SSMMI2
C***END PROLOGUE  SSMMTI
      INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, IWORK(10)
      REAL    B(N), X(N), A(NELT), RWORK(1)
C
C         Pull out the locations of the arrays holding the ILU
C         factorization.
C***FIRST EXECUTABLE STATEMENT  SSMMTI
      LOCIL = IWORK(1)
      LOCJL = IWORK(2)
      LOCIU = IWORK(3)
      LOCJU = IWORK(4)
      LOCL = IWORK(5)
      LOCDIN = IWORK(6)
      LOCU = IWORK(7)
C
      CALL SSMMI2(N, B, X, IWORK(LOCIL), IWORK(LOCJL),
     $     RWORK(LOCL), RWORK(LOCDIN), IWORK(LOCIU),
     $     IWORK(LOCJU), RWORK(LOCU))
C
      RETURN
C------------- LAST LINE OF SSMMTI FOLLOWS ----------------------------
      END
c@process directive('*vdir:')
      SUBROUTINE SSMMI2( N, B, X, IL, JL, L, DINV, IU, JU, U )
C***BEGIN PROLOGUE  SSMMI2
C***DATE WRITTEN   871119   (YYMMDD)
C***REVISION DATE  881213   (YYMMDD)
C***CATEGORY NO.  D2A4
C***KEYWORDS  LIBRARY=SLATEC(SLAP),
C             TYPE=SINGLE PRECISION(SSMMI2-S),
C             Linear system, Sparse, Iterative Precondition
C***AUTHOR  Greenbaum, Anne, Courant Institute
C           Seager, Mark K., (LLNL)
C             Lawrence Livermore National Laboratory
C             PO BOX 808, L-300
C             Livermore, CA 94550 (415) 423-3141
C             seager@lll-crg.llnl.gov
C***PURPOSE  SLAP Back solve for LDU Factorization of Normal Equations.
C            To solve a system of the form (L*D*U)*(L*D*U)' X  =  B,
C            where  L  is a unit lower triangular matrix,  D   is  a
C            diagonal matrix, and  U  is  a  unit  upper  triangular
C            matrix and ' denotes transpose.
C***DESCRIPTION
C *Usage:
C     INTEGER N, IL(N+1), JL(NL), IU(N+1), JU(NU)
C     REAL    B(N), X(N), L(NL), DINV(N), U(NU)
C
C     CALL SSMMI2( N, B, X, IL, JE, L, DINV, IU, JU, U )
C
C *Arguments:
C N      :IN       Integer
C         Order of the Matrix.
C B      :IN       Real B(N).
C         Right hand side.
C X      :OUT      Real X(N).
C         Solution of (L*D*U)(L*D*U)trans x = b.
C IL     :IN       Integer IL(N+1).
C JL     :IN       Integer JL(NL).
C  L     :IN       Real     L(NL).
C         IL, JL, L contain the unit lower  triangular factor of the
C         incomplete decomposition of some matrix stored in SLAP Row
C         format.  The diagonal of ones *IS* stored.  This structure
C         can  be  set up by   the  SSILUS   routine.    See
C         "DESCRIPTION", below for  more   details   about  the SLAP
C         format.
C DINV   :IN       Real DINV(N).
C         Inverse of the diagonal matrix D.
C IU     :IN       Integer IU(N+1).
C JU     :IN       Integer JU(NU).
C U      :IN       Real     U(NU).
C         IU, JU, U contain the unit upper  triangular factor of the
C         incomplete decomposition  of   some matrix stored in  SLAP
C         Column  format.  The diagonal  of  ones *IS* stored.  This
C         structure can be set up  by the SSILUS routine.  See
C         "DESCRIPTION",  below  for  more  details  about  the SLAP
C         format.
C
C *Description:
C       This routine is supplied with the SLAP package as  a routine
C       to  perform  the  MSOLVE  operation  in  the SBCGN iteration
C       routine for the  driver SSLUCN.   It must  be called via the
C       SLAP  MSOLVE calling  sequence convention interface  routine
C       SSMMTI.
C         **** THIS ROUTINE ITSELF DOES NOT CONFORM TO THE ****
C               **** SLAP MSOLVE CALLING CONVENTION ****
C
C       IL, JL, L should contain the unit lower triangular factor of
C       the incomplete decomposition of the A matrix  stored in SLAP
C       Row format.  IU, JU, U should contain  the unit upper factor
C       of the  incomplete decomposition of  the A matrix  stored in
C       SLAP Column format This ILU factorization can be computed by
C       the SSILUS routine.  The diagonals (which is all one's) are
C       stored.
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
C       ==================== S L A P Row format ====================
C       This routine requires  that the matrix A  be  stored  in the
C       SLAP  Row format.   In this format  the non-zeros are stored
C       counting across  rows (except for the diagonal  entry, which
C       must appear first in each "row") and  are stored in the real
C       array A.  In other words, for each row in the matrix put the
C       diagonal entry in  A.   Then   put  in the   other  non-zero
C       elements   going  across the  row (except   the diagonal) in
C       order.   The  JA array  holds   the column   index for  each
C       non-zero.   The IA  array holds the  offsets into  the JA, A
C       arrays  for   the   beginning  of   each  row.   That    is,
C       JA(IA(IROW)),  A(IA(IROW)) points  to  the beginning  of the
C       IROW-th row in JA and A.   JA(IA(IROW+1)-1), A(IA(IROW+1)-1)
C       points to the  end of the  IROW-th row.  Note that we always
C       have IA(N+1) =  NELT+1, where  N  is  the number of rows  in
C       the matrix  and NELT  is the  number   of  non-zeros in  the
C       matrix.
C
C       Here is an example of the SLAP Row storage format for a  5x5
C       Matrix (in the A and JA arrays '|' denotes the end of a row):
C
C           5x5 Matrix         SLAP Row format for 5x5 matrix on left.
C                              1  2  3    4  5    6  7    8    9 10 11
C       |11 12  0  0 15|   A: 11 12 15 | 22 21 | 33 35 | 44 | 55 51 53
C       |21 22  0  0  0|  JA:  1  2  5 |  2  1 |  3  5 |  4 |  5  1  3
C       | 0  0 33  0 35|  IA:  1  4  6    8  9   12
C       | 0  0  0 44  0|
C       |51  0 53  0 55|
C
C       With  the SLAP  format  the "inner  loops" of  this  routine
C       should vectorize   on machines with   hardware  support  for
C       vector gather/scatter operations.  Your compiler may require
C       a  compiler directive  to  convince   it that there  are  no
C       implicit vector  dependencies.  Compiler directives  for the
C       Alliant FX/Fortran and CRI CFT/CFT77 compilers  are supplied
C       with the standard SLAP distribution.
C
C *Precision:           Single Precision
C *See Also:
C       SSILUS
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  SSMMI2
      INTEGER N, IL(1), JL(1), IU(1), JU(1)
      REAL    B(N), X(N), L(1), DINV(N), U(N)
C
C         Solve  L*Y = B,  storing result in X, L stored by rows.
C***FIRST EXECUTABLE STATEMENT  SSMMI2
      DO 10 I = 1, N
         X(I) = B(I)
 10   CONTINUE
      DO 30 IROW = 2, N
         JBGN = IL(IROW)
         JEND = IL(IROW+1)-1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ ASSOC
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 20 J = JBGN, JEND
               X(IROW) = X(IROW) - L(J)*X(JL(J))
 20         CONTINUE
         ENDIF
 30   CONTINUE
C
C         Solve  D*Z = Y,  storing result in X.
      DO 40 I=1,N
         X(I) = X(I)*DINV(I)
 40   CONTINUE
C
C         Solve  U*X = Z, U stored by columns.
      DO 60 ICOL = N, 2, -1
         JBGN = JU(ICOL)
         JEND = JU(ICOL+1)-1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 50 J = JBGN, JEND
               X(IU(J)) = X(IU(J)) - U(J)*X(ICOL)
 50         CONTINUE
         ENDIF
 60   CONTINUE
C
C         Solve  U'*Y = X,  storing result in X, U stored by columns.
      DO 80 IROW = 2, N
         JBGN = JU(IROW)
         JEND = JU(IROW+1) - 1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ ASSOC
CVD$ NODEPCHK
c*vdir: ignore recrdeps(x)
            DO 70 J = JBGN, JEND
               X(IROW) = X(IROW) - U(J)*X(IU(J))
 70         CONTINUE
         ENDIF
 80   CONTINUE
C
C         Solve  D*Z = Y,  storing result in X.
      DO 90 I = 1, N
         X(I) = X(I)*DINV(I)
 90   CONTINUE
C
C         Solve  L'*X = Z, L stored by rows.
c*vdir: ignore recrdeps(x)
      DO 110 ICOL = N, 2, -1
         JBGN = IL(ICOL)
         JEND = IL(ICOL+1) - 1
         IF( JBGN.LE.JEND ) THEN
CLLL. OPTION ASSERT (NOHAZARD)
CDIR$ IVDEP
CVD$ NODEPCHK
            DO 100 J = JBGN, JEND
               X(JL(J)) = X(JL(J)) - L(J)*X(ICOL)
 100        CONTINUE
         ENDIF
 110  CONTINUE
C
      RETURN
C------------- LAST LINE OF SSMMI2 FOLLOWS ----------------------------
      END
