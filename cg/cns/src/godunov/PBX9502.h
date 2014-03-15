c
c EOS Parameters for PBX 9502:
c
c solid phase
      omeg(1)=0.8938d0
      ajwl(1,1)= 577.5d0
      ajwl(2,1)=-.04086d0
      rjwl(1,1)=11.3d0
      rjwl(2,1)=1.13d0
c
c gas phase
      omeg(2)=0.5d0
      ajwl(1,2)= 12.29d0
      ajwl(2,2)= .6146d0
      rjwl(1,2)=6.2d0
      rjwl(2,2)=2.2d0
c
c solid phase reference state
      vs0=1.d0
      ts0=1.d0
      zsvs0=vs0*(ajwl(1,1)*dexp(-rjwl(1,1)*vs0)
     *          +ajwl(2,1)*dexp(-rjwl(2,1)*vs0))/omeg(1)
      fsvs0=zsvs0-ajwl(1,1)*dexp(-rjwl(1,1)*vs0)/rjwl(1,1)
     *           -ajwl(2,1)*dexp(-rjwl(2,1)*vs0)/rjwl(2,1)
c     zsvs0=0.d0
c     fsvs0=0.d0
c
c gas phase reference state
!      vg0=4.d0
      vg0=2.5d0
      zgvg0=vg0*(ajwl(1,2)*dexp(-rjwl(1,2)*vg0)
     *          +ajwl(2,2)*dexp(-rjwl(2,2)*vg0))/omeg(2)
      fgvg0=zgvg0-ajwl(1,2)*dexp(-rjwl(1,2)*vg0)/rjwl(1,2)
     *           -ajwl(2,2)*dexp(-rjwl(2,2)*vg0)/rjwl(2,2)
c     zgvg0=0.d0
c     fgvg0=0.d0
c
c ratio Cg/Cs and heat release
      cgcs=.4021d0
      heat=.0630d0
c
c Ignition and growth parameters for PBX 9502:
c
      ra=.214d0
      eb=2.d0/3.d0
      ex=7.d0
      ec=2.d0/3.d0
      ed=1.d0/9.d0
      ey=1.d0
      ee=1.d0/3.d0
      eg=1.d0
      ez=3.d0
      al0=0.5d0
      al1=0.5d0
      al2=0.d0 
c
      tref=1.d-6
      pref=1.094489d11
      ai=tref*4.4d11
      ag1=tref*.6d6*(pref*1.d-11)**ey
      ag2=tref*400.d6*(pref*1.d-11)**ez
c
c CJ detonation speed
c     DCJ=7.6078d6
c     xref=DCJ*tref
