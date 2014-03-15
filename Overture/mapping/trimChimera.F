      SUBROUTINE TRIM  ( STRING,LSTRIN )
C
C   Return the length of STRING after trailing blanks, nulls, and tabs have 
C   been removed.
C
#include "precis.h"
      CHARACTER*(*) STRING
C
      CHARACTER NULL,TAB
C
C
C   Initialize the null and tab characters.
C
      NULL   = CHAR(0)
      TAB    = CHAR(9)
C
C   Loop backwards through the character string and find the last nonblank,
C   nonnull character.
C
      LSTRIN = LEN(STRING)
      DO 10 L = LSTRIN,1,-1
         IF (STRING(L:L).NE.' ' .AND. STRING(L:L).NE.NULL
     C                          .AND. STRING(L:L).NE.TAB) THEN
            LSTRIN = L
            GOTO 20
         ENDIF
   10    CONTINUE
C
C   ALL blank or null or tabs!
C
      LSTRIN = 0
C
C
   20 CONTINUE
      RETURN
      END
