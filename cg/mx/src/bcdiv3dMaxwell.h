! These should already be computed from the tangential components:
! DsTau1DotUvr = ( tau11*ur+tau12*vr+tau13*wr).s 
! DsTau2DotUvr = ( tau21*ur+tau22*vr+tau23*wr).s 

      detnt=tau21*tau13*a12-tau23*a12*tau11+a13*tau22*tau11+tau23*a11*tau12-tau22*tau13*a11-a13*tau21*tau12
      a1DotUvrsRHS=-a33st*uez-a23ss*uez-a13rs*uez-a11rs*uex-a31st*uex-a21ss*uex-a12rs*uey-a22ss*uey-a32st*uey-a11r*us-a33s*wt-a33t*ws-a12r*vs-2*a23s*ws-a32*vst-a22*vss-a13r*ws-a31t*us-a31s*ut-2*a21s*us-a33*wst-a23*wss-a21*uss-a32t*vs-a32s*vt-a31*ust-2*a22s*vs
      a1DotUvrtRHS=-a23t*ws-a23s*wt-2*a31t*ut-a21*ust-a13r*wt-a31*utt-a21t*us-a21s*ut-2*a32t*vt-a32*vtt-a22*vst-2*a33t*wt-a22s*vt-a22st*uey-a23st*uez-a13rt*uez-a21st*uex-a12rt*uey-a32tt*uey-a11rt*uex-a33tt*uez-a31tt*uex-a22t*vs-a23*wst-a33*wtt-a11r*ut-a12r*vt
      a1DotUvrssRHS=-3*a21s*uss-3*a21ss*us-2*a13rs*ws-a12r*vss-a31t*uss-2*a31s*ust-a31*usst-a21*usss-2*a31st*us-a31ss*ut-3*a22s*vss-3*a22ss*vs-a13rss*uez-a23sss*uez-a33sst*uez-a21sss*uex-a31sst*uex-a11rss*uex-a12rss*uey-a22sss*uey-a32sst*uey-a32*vsst-a22*vsss-2*a32s*vst-a32t*vss-2*a32st*vs-a32ss*vt-2*a12rs*vs-a13r*wss-3*a23s*wss-3*a23ss*ws-a33ss*wt-2*a33st*ws-a33t*wss-2*a33s*wst-a33*wsst-a23*wsss-a11r*uss-2*a11rs*us
      a1DotUvrttRHS=-2*a13rt*wt-a13r*wtt-a21tt*us-2*a21st*ut-2*a21t*ust-a21s*utt-a11r*utt-a22tt*vs-2*a22st*vt-2*a22t*vst-a22s*vtt-2*a11rt*ut-a21stt*uex-a11rtt*uex-a31ttt*uex-2*a12rt*vt-a32ttt*uey-a23stt*uez-a33ttt*uez-a13rtt*uez-a22stt*uey-a12rtt*uey-2*a23t*wst-3*a31tt*ut-3*a31t*utt-a23s*wtt-a12r*vtt-a23tt*ws-2*a23st*wt-3*a32tt*vt-3*a32t*vtt-a21*ustt-a31*uttt-3*a33tt*wt-3*a33t*wtt-a32*vttt-a22*vstt-a33*wttt-a23*wstt

! urs = cursu*ur + cursv*vr + cursw*wr + furs
! vrs = cvrsu*ur + cvrsv*vr + cvrsw*wr + fvrs
! wrs = cwrsu*ur + cwrsv*vr + cwrsw*wr + fwrs
      cursu=(-a13*tau11s*tau22+a13*tau12*tau21s+tau11s*tau23*a12+tau13*a11s*tau22-tau13*a12*tau21s-tau12*tau23*a11s)/detnt
      cursv=(-a13*tau12s*tau22+a13*tau12*tau22s-tau12*tau23*a12s+tau12s*tau23*a12+tau13*a12s*tau22-tau13*a12*tau22s)/detnt
      cursw=(-a13*tau13s*tau22+a13*tau12*tau23s+tau13s*tau23*a12+tau13*a13s*tau22-tau13*a12*tau23s-tau12*tau23*a13s)/detnt

      cvrsu=(-tau21*tau13*a11s-tau23*a11*tau11s+tau23*a11s*tau11+tau21s*tau13*a11-a13*tau21s*tau11+a13*tau21*tau11s)/detnt
      cvrsv=(a13*tau21*tau12s-tau23*a11*tau12s-tau21*tau13*a12s+tau23*a12s*tau11+tau22s*tau13*a11-a13*tau22s*tau11)/detnt
      cvrsw=(tau23s*tau13*a11-tau21*tau13*a13s-tau23*a11*tau13s+tau23*a13s*tau11+a13*tau21*tau13s-a13*tau23s*tau11)/detnt

      cwrsu=(-a11*tau12*tau21s+a11*tau11s*tau22+a11s*tau21*tau12-a11s*tau22*tau11-a12*tau21*tau11s+a12*tau21s*tau11)/detnt
      cwrsv=(-a11*tau12*tau22s+a11*tau12s*tau22-a12*tau21*tau12s-a12s*tau22*tau11+a12*tau22s*tau11+a12s*tau21*tau12)/detnt
      cwrsw=(-a11*tau12*tau23s+a11*tau13s*tau22-a12*tau21*tau13s+a12*tau23s*tau11+a13s*tau21*tau12-a13s*tau22*tau11)/detnt

! urt = curtu*ur + curtv*vr + curtw*wr + furt
! vrt = cvrtu*ur + cvrtv*vr + cvrtw*wr + fvrt
! wrt = cwrtu*ur + cwrtv*vr + cwrtw*wr + fwrt
      curtu=(a13*tau12*tau21t-a13*tau11t*tau22-tau13*a12*tau21t-tau12*tau23*a11t+tau13*a11t*tau22+tau11t*tau23*a12)/detnt
      curtv=(-a13*tau12t*tau22+a13*tau12*tau22t-tau12*tau23*a12t+tau12t*tau23*a12-tau13*a12*tau22t+tau13*a12t*tau22)/detnt
      curtw=(a13*tau12*tau23t-tau12*tau23*a13t-tau13*a12*tau23t-a13*tau13t*tau22+tau13*a13t*tau22+tau13t*tau23*a12)/detnt

      cvrtu=(-a13*tau21t*tau11+a13*tau21*tau11t-tau21*tau13*a11t+tau21t*tau13*a11+tau23*a11t*tau11-tau23*a11*tau11t)/detnt
      cvrtv=(-a13*tau22t*tau11+a13*tau21*tau12t+tau22t*tau13*a11+tau23*a12t*tau11-tau21*tau13*a12t-tau23*a11*tau12t)/detnt
      cvrtw=(-a13*tau23t*tau11+a13*tau21*tau13t+tau23*a13t*tau11-tau23*a11*tau13t-tau21*tau13*a13t+tau23t*tau13*a11)/detnt

      cwrtu=(-a12*tau21*tau11t+a11t*tau21*tau12-a11t*tau22*tau11+a12*tau21t*tau11+a11*tau11t*tau22-a11*tau12*tau21t)/detnt
      cwrtv=(-a12*tau21*tau12t+a12*tau22t*tau11+a12t*tau21*tau12-a12t*tau22*tau11+a11*tau12t*tau22-a11*tau12*tau22t)/detnt
      cwrtw=(a12*tau23t*tau11+a11*tau13t*tau22-a12*tau21*tau13t-a11*tau12*tau23t+a13t*tau21*tau12-a13t*tau22*tau11)/detnt

      furs=(-a13*tau12*DsTau2DotUvr+a13*DsTau1DotUvr*tau22+tau12*tau23*a1DotUvrsRHS-tau13*a1DotUvrsRHS*tau22+tau13*a12*DsTau2DotUvr-DsTau1DotUvr*tau23*a12)/detnt
      fvrs=-(a13*tau21*DsTau1DotUvr-a13*DsTau2DotUvr*tau11-tau23*a11*DsTau1DotUvr-tau21*tau13*a1DotUvrsRHS+tau23*a1DotUvrsRHS*tau11+DsTau2DotUvr*tau13*a11)/detnt
      fwrs=(-a11*DsTau1DotUvr*tau22+a11*tau12*DsTau2DotUvr-a12*DsTau2DotUvr*tau11+a12*tau21*DsTau1DotUvr-a1DotUvrsRHS*tau21*tau12+a1DotUvrsRHS*tau22*tau11)/detnt

      furt=-(DtTau1DotUvr*tau23*a12+tau13*a1DotUvrtRHS*tau22-tau12*tau23*a1DotUvrtRHS+a13*tau12*DtTau2DotUvr-a13*DtTau1DotUvr*tau22-tau13*a12*DtTau2DotUvr)/detnt
      fvrt=(-a13*tau21*DtTau1DotUvr+a13*DtTau2DotUvr*tau11-tau23*a1DotUvrtRHS*tau11-DtTau2DotUvr*tau13*a11+tau21*tau13*a1DotUvrtRHS+tau23*a11*DtTau1DotUvr)/detnt
      fwrt=-(a11*DtTau1DotUvr*tau22-a11*tau12*DtTau2DotUvr+a12*DtTau2DotUvr*tau11-a12*tau21*DtTau1DotUvr-a1DotUvrtRHS*tau22*tau11+a1DotUvrtRHS*tau21*tau12)/detnt

! These may not be needed:
!       urs =cursu*ur+cursv*vr+cursw*wr+furs
!       vrs =cvrsu*ur+cvrsv*vr+cvrsw*wr+fvrs
!       wrs =cwrsu*ur+cwrsv*vr+cwrsw*wr+fwrs

!       urt =curtu*ur+curtv*vr+curtw*wr+furt
!       vrt =cvrtu*ur+cvrtv*vr+cvrtw*wr+fvrt
!       wrt =cwrtu*ur+cwrtv*vr+cwrtw*wr+fwrt

      b3u=a11*c11
      b3v=a12*c11
      b3w=a13*c11
      b2u=a11r*c11+a11*c1+a11*c11r
      b2v=a12*c1+a12*c11r+a12r*c11
      b2w=a13*c11r+a13*c1+a13r*c11
      b1u=-2*c22*a11s*cursu-2*c33*a13t*cwrtu-c2*a11s-2*c33*a12t*cvrtu-2*c33*a11t*curtu-2*c22*a13s*cwrsu-2*c22*a12s*cvrsu-c3*a11t-c22*a11ss-c33*a11tt+a11*c1r+a11r*c1
      b1v=-2*c22*a11s*cursv-2*c33*a13t*cwrtv-2*c33*a12t*cvrtv-c2*a12s-2*c33*a11t*curtv-2*c22*a13s*cwrsv-2*c22*a12s*cvrsv-c3*a12t-c22*a12ss-c33*a12tt+a12*c1r+a12r*c1
      b1w=-2*c22*a11s*cursw-2*c33*a13t*cwrtw-c2*a13s-2*c33*a12t*cvrtw-2*c33*a11t*curtw-2*c22*a13s*cwrsw-2*c22*a12s*cvrsw-c3*a13t+a13*c1r-c22*a13ss+a13r*c1-c33*a13tt
      bf =-a11r*uTmTm-a13r*wTmTm-a13*wTmTmr-a11*uTmTmr-a12r*vTmTm-a12*vTmTmr+c2*a1DotUvrsRHS+c3*a1DotUvrtRHS+c22*a1DotUvrssRHS+a13*c22r*wss+a13*c33r*wtt+a13*c2r*ws+a13*c3r*wt+a13r*c22*wss+a13r*c33*wtt+a13r*c2*ws+a13r*c3*wt+c33*a1DotUvrttRHS-2*c22*a12s*fvrs-2*c22*a13s*fwrs-2*c33*a11t*furt-2*c33*a12t*fvrt-2*c33*a13t*fwrt-2*c22*a11s*furs+a12*c2r*vs+a12*c3r*vt+a12r*c22*vss+a12r*c33*vtt+a12r*c2*vs+a12r*c3*vt+a12*c22r*vss+a12*c33r*vtt+a11*c2r*us+a11*c3r*ut+a11*c22r*uss+a11*c33r*utt+a11r*c3*ut+a11r*c33*utt+a11r*c22*uss+a11r*c2*us
