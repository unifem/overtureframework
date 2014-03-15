C-------------------------------------------------------------------------------
C
C   ERROR -- Error logging utility for small applications or tools.
C
C                Pieter G. Buning      5 January 1995
C
C
C   FORTRAN entry points:
C
C     SUBROUTINE ERRINI( NAME,IUNIT )
C     CHARACTER*(*) NAME
C       Initialize error logging by opening a new text file of the name
C       "NAME.err".
C         NAME   - Base name of the application.
C         IUNIT  - FORTRAN unit number to use for error logging.
C
C     SUBROUTINE ERRMSG( TEXT,NLINE )
C     CHARACTER*(*) TEXT(NLINE)
C       Print an error message to the error logging file and echo to
C       standard out.
C         TEXT   - Text to print out as an error message.
C         NLINE  - Number of lines of text to print.
C
C     SUBROUTINE ERREND
C       Terminate error logging.  Delete the error log file if no errors
C       were logged.
C
C-------------------------------------------------------------------------------
C   
C
      SUBROUTINE ERRINI( NAME,IUNIT )
C
C   Initialize error logging for application "NAME".  This means delete
C   any existing file named NAME.err and open a new file.  Use unit
C   IUNIT for error logging.
C
#include "precis.h"
#include "errcom.h"
C
      CHARACTER*(*) NAME
C
C
C   Zero the number of errors, save the filename and unit number.
C
      NUMERR = 0
      ERRLOG = .TRUE.
      ERRFIL = NAME//'.err'
      LERRFI = LEN(NAME)+4
      IOERR  = IUNIT
C
C   Open the error logging file as a new file.
C
      OPEN(UNIT=IOERR,FILE=ERRFIL(1:LERRFI),STATUS='UNKNOWN',
     C     FORM='FORMATTED',ERR=1010)
C
C
   10 CONTINUE
      RETURN
C
C   Unable to open file.  Disable error logging.
C
 1010 CONTINUE
      ERRLOG = .FALSE.
      WRITE(*,1011) ERRFIL(1:LERRFI)
 1011 FORMAT(/' ** WARNING ** Unable to open error logging file ',A/
     &        '               Error messages will ONLY be written ',
     &        'to standard out.'/)
      GOTO 10
C
      END
      SUBROUTINE ERRMSG( TEXT,NLINES )
C
C   Log an error message by writing it out to the error log file.
C   Also echo it to standard out.
C
#include "precis.h"
#include "errcom.h"
C
      CHARACTER*(*) TEXT(*)
C
C
C   Write the message to the error log and standard out, and increment
C   the counter.
C
      WRITE(*,*)
      IF (ERRLOG) WRITE(IOERR,*)
      DO 10 I = 1,NLINES
         CALL TRIM  ( TEXT(I),LTEXT )
         WRITE(*,*) TEXT(I)(1:LTEXT)
         IF (ERRLOG) WRITE(IOERR,*) TEXT(I)(1:LTEXT)
   10    CONTINUE
      NUMERR = NUMERR+1
C
C
      RETURN
      END
      SUBROUTINE ERREND
C
C   End error logging.  Delete the error logging file if no errors were
C   logged.
C
#include "precis.h"
#include "errcom.h"
C
C
C   If there were errors, close the error logging file.
C
      IF (NUMERR.NE.0) THEN
         IF (ERRLOG) CLOSE(UNIT=IOERR)
C
C   If not, delete it.
C
      ELSE
         IF (ERRLOG) CLOSE(UNIT=IOERR,STATUS='DELETE',ERR=1010)
      ENDIF
C
C
   10 CONTINUE
      RETURN
C
 1010 CONTINUE
      WRITE(*,1011) ERRFIL(1:LERRFI)
 1011 FORMAT(' ** WARNING ** Unable to delete error logging file ',A)
      GOTO 10
C
      END
