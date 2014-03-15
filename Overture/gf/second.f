      subroutine second(time)
c
c       $Header: /usr/gapps/overture/OvertureRepoCVS/overture/Overture/gf/second.f,v 1.4 2007/01/02 16:03:38 henshaw Exp $
c
c       Compute CPU time on systems where dtime is available.
c
        logical init
c* wdh        double precision xtime
c* wdh         real tarray(2)
        real*8 xtime
        real*4 tarray(2)
        save init,xtime
        data init/.true./

        call dtime(tarray)
        if(init)then
          xtime=tarray(1)+tarray(2)
          init=.false.
         else
          xtime=xtime+tarray(1)+tarray(2)
         endif
        time=xtime
       end
      subroutine secondf(time)
      call second(time)
      end
      subroutine ovtime(time)
      call second(time)
      end
