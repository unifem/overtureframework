      subroutine openck( inck,fileName )
      integer inck
      character fileName*(*)

      open (inck, form='unformatted', status='unknown', 
     1              file=fileName) 

      return 
      end

      subroutine closeck( inck )
      close(inck)
      end

