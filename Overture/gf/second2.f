      subroutine second(time)
c
c       $Header: /usr/gapps/overture/OvertureRepoCVS/overture/Overture/gf/second2.f,v 1.3 1999/11/19 23:10:41 henshaw Exp $
c
c       Compute CPU time on systems where dtime is available.
c
        logical init
        real*8 xtime
        integer tarray(4)
        save init,xtime
        data init/.true./

        call ftimes(tarray)
        if(init)then
          xtime=tarray(1)+tarray(2)
          init=.false.
         else
          xtime=xtime+tarray(1)+tarray(2)
         endif
        time=xtime
       end
