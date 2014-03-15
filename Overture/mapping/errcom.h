C
C   COMMON blocks for error logging.
C
C     NUMERR - Number of errors logged.
C     IOERR  - FORTAN unit number for error logging.
C     LERRFI - Length of filename.
C     ERRLOG - Logical flag for error logging.
C     ERRFIL - Filename for writing error messages.
C
      COMMON /ERRCOM/ NUMERR,IOERR,LERRFI,ERRLOG
      LOGICAL ERRLOG
      COMMON /ERRCOC/ ERRFIL
      CHARACTER*80 ERRFIL
