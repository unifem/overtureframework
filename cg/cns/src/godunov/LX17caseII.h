c
c EOS Parameters for LX-17:II.   Revised May 7, 03
c We use EOS parameters for PBX-9502, and rate parameters 
c for PBX-9502, except I, which is changed from 4.4e11
c to 4e12
c solid phase
      omeg(1)=0.8938d0
c     ajwl(1,1)= 710.925d0
c     ajwl(2,1)=-.0459667d0
      ajwl(1,1)= 577.5d0
      ajwl(2,1)=-.04086d0
      rjwl(1,1)=11.3d0
      rjwl(2,1)=1.13d0
c
c gas phase
      omeg(2)=0.5d0
      ajwl(1,2)= 12.29249d0
      ajwl(2,2)= .6146247d0
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
      vg0=5.d0
      zgvg0=vg0*(ajwl(1,2)*dexp(-rjwl(1,2)*vg0)
     *          +ajwl(2,2)*dexp(-rjwl(2,2)*vg0))/omeg(2)
      fgvg0=zgvg0-ajwl(1,2)*dexp(-rjwl(1,2)*vg0)/rjwl(1,2)
     *           -ajwl(2,2)*dexp(-rjwl(2,2)*vg0)/rjwl(2,2)
c     zgvg0=0.d0
c     fgvg0=0.d0
c
c ratio Cg/Cs and heat release
      cgcs=1.d0/2.487d0
      heat=.063043d0
c
c Ignition and growth parameters for PBX-9502:
c
c     ra=0.22d0
      ra = 0.214d0
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
      al2=0.0d0 
c
      tref=1d-6
      pref=1.094489d0
      ai=tref*4.d12
      ag1=tref*0.6d6*(pref)**ey
      ag2=tref*400.0d6*(pref)**ez
c
c Lagrangian marker locations
      DCJ=7.6078d6
      xref=DCJ*tref
