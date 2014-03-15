! This file automatically generated from interfaceMacros.bf with bpp.
       ! ******************* 6th order ******************
       uu1 = u1(i1,i2,i3,ex)
       uu1r = (-u1(i1-3,i2,i3,ex)+9.*u1(i1-2,i2,i3,ex)-45.*u1(i1-1,i2,
     & i3,ex)+45.*u1(i1+1,i2,i3,ex)-9.*u1(i1+2,i2,i3,ex)+u1(i1+3,i2,
     & i3,ex))/(60.*dr1(0))
       uu1s = (-u1(i1,i2-3,i3,ex)+9.*u1(i1,i2-2,i3,ex)-45.*u1(i1,i2-1,
     & i3,ex)+45.*u1(i1,i2+1,i3,ex)-9.*u1(i1,i2+2,i3,ex)+u1(i1,i2+3,
     & i3,ex))/(60.*dr1(1))
       uu1rr = (2.*u1(i1-3,i2,i3,ex)-27.*u1(i1-2,i2,i3,ex)+270.*u1(i1-
     & 1,i2,i3,ex)-490.*u1(i1,i2,i3,ex)+270.*u1(i1+1,i2,i3,ex)-27.*u1(
     & i1+2,i2,i3,ex)+2.*u1(i1+3,i2,i3,ex))/(180.*dr1(0)**2)
       uu1rs = (-(-u1(i1-3,i2-3,i3,ex)+9.*u1(i1-3,i2-2,i3,ex)-45.*u1(
     & i1-3,i2-1,i3,ex)+45.*u1(i1-3,i2+1,i3,ex)-9.*u1(i1-3,i2+2,i3,ex)
     & +u1(i1-3,i2+3,i3,ex))/(60.*dr1(1))+9.*(-u1(i1-2,i2-3,i3,ex)+9.*
     & u1(i1-2,i2-2,i3,ex)-45.*u1(i1-2,i2-1,i3,ex)+45.*u1(i1-2,i2+1,
     & i3,ex)-9.*u1(i1-2,i2+2,i3,ex)+u1(i1-2,i2+3,i3,ex))/(60.*dr1(1))
     & -45.*(-u1(i1-1,i2-3,i3,ex)+9.*u1(i1-1,i2-2,i3,ex)-45.*u1(i1-1,
     & i2-1,i3,ex)+45.*u1(i1-1,i2+1,i3,ex)-9.*u1(i1-1,i2+2,i3,ex)+u1(
     & i1-1,i2+3,i3,ex))/(60.*dr1(1))+45.*(-u1(i1+1,i2-3,i3,ex)+9.*u1(
     & i1+1,i2-2,i3,ex)-45.*u1(i1+1,i2-1,i3,ex)+45.*u1(i1+1,i2+1,i3,
     & ex)-9.*u1(i1+1,i2+2,i3,ex)+u1(i1+1,i2+3,i3,ex))/(60.*dr1(1))-
     & 9.*(-u1(i1+2,i2-3,i3,ex)+9.*u1(i1+2,i2-2,i3,ex)-45.*u1(i1+2,i2-
     & 1,i3,ex)+45.*u1(i1+2,i2+1,i3,ex)-9.*u1(i1+2,i2+2,i3,ex)+u1(i1+
     & 2,i2+3,i3,ex))/(60.*dr1(1))+(-u1(i1+3,i2-3,i3,ex)+9.*u1(i1+3,
     & i2-2,i3,ex)-45.*u1(i1+3,i2-1,i3,ex)+45.*u1(i1+3,i2+1,i3,ex)-9.*
     & u1(i1+3,i2+2,i3,ex)+u1(i1+3,i2+3,i3,ex))/(60.*dr1(1)))/(60.*
     & dr1(0))
       uu1ss = (2.*u1(i1,i2-3,i3,ex)-27.*u1(i1,i2-2,i3,ex)+270.*u1(i1,
     & i2-1,i3,ex)-490.*u1(i1,i2,i3,ex)+270.*u1(i1,i2+1,i3,ex)-27.*u1(
     & i1,i2+2,i3,ex)+2.*u1(i1,i2+3,i3,ex))/(180.*dr1(1)**2)
       vv1 = u1(i1,i2,i3,ey)
       vv1r = (-u1(i1-3,i2,i3,ey)+9.*u1(i1-2,i2,i3,ey)-45.*u1(i1-1,i2,
     & i3,ey)+45.*u1(i1+1,i2,i3,ey)-9.*u1(i1+2,i2,i3,ey)+u1(i1+3,i2,
     & i3,ey))/(60.*dr1(0))
       vv1s = (-u1(i1,i2-3,i3,ey)+9.*u1(i1,i2-2,i3,ey)-45.*u1(i1,i2-1,
     & i3,ey)+45.*u1(i1,i2+1,i3,ey)-9.*u1(i1,i2+2,i3,ey)+u1(i1,i2+3,
     & i3,ey))/(60.*dr1(1))
       vv1rr = (2.*u1(i1-3,i2,i3,ey)-27.*u1(i1-2,i2,i3,ey)+270.*u1(i1-
     & 1,i2,i3,ey)-490.*u1(i1,i2,i3,ey)+270.*u1(i1+1,i2,i3,ey)-27.*u1(
     & i1+2,i2,i3,ey)+2.*u1(i1+3,i2,i3,ey))/(180.*dr1(0)**2)
       vv1rs = (-(-u1(i1-3,i2-3,i3,ey)+9.*u1(i1-3,i2-2,i3,ey)-45.*u1(
     & i1-3,i2-1,i3,ey)+45.*u1(i1-3,i2+1,i3,ey)-9.*u1(i1-3,i2+2,i3,ey)
     & +u1(i1-3,i2+3,i3,ey))/(60.*dr1(1))+9.*(-u1(i1-2,i2-3,i3,ey)+9.*
     & u1(i1-2,i2-2,i3,ey)-45.*u1(i1-2,i2-1,i3,ey)+45.*u1(i1-2,i2+1,
     & i3,ey)-9.*u1(i1-2,i2+2,i3,ey)+u1(i1-2,i2+3,i3,ey))/(60.*dr1(1))
     & -45.*(-u1(i1-1,i2-3,i3,ey)+9.*u1(i1-1,i2-2,i3,ey)-45.*u1(i1-1,
     & i2-1,i3,ey)+45.*u1(i1-1,i2+1,i3,ey)-9.*u1(i1-1,i2+2,i3,ey)+u1(
     & i1-1,i2+3,i3,ey))/(60.*dr1(1))+45.*(-u1(i1+1,i2-3,i3,ey)+9.*u1(
     & i1+1,i2-2,i3,ey)-45.*u1(i1+1,i2-1,i3,ey)+45.*u1(i1+1,i2+1,i3,
     & ey)-9.*u1(i1+1,i2+2,i3,ey)+u1(i1+1,i2+3,i3,ey))/(60.*dr1(1))-
     & 9.*(-u1(i1+2,i2-3,i3,ey)+9.*u1(i1+2,i2-2,i3,ey)-45.*u1(i1+2,i2-
     & 1,i3,ey)+45.*u1(i1+2,i2+1,i3,ey)-9.*u1(i1+2,i2+2,i3,ey)+u1(i1+
     & 2,i2+3,i3,ey))/(60.*dr1(1))+(-u1(i1+3,i2-3,i3,ey)+9.*u1(i1+3,
     & i2-2,i3,ey)-45.*u1(i1+3,i2-1,i3,ey)+45.*u1(i1+3,i2+1,i3,ey)-9.*
     & u1(i1+3,i2+2,i3,ey)+u1(i1+3,i2+3,i3,ey))/(60.*dr1(1)))/(60.*
     & dr1(0))
       vv1ss = (2.*u1(i1,i2-3,i3,ey)-27.*u1(i1,i2-2,i3,ey)+270.*u1(i1,
     & i2-1,i3,ey)-490.*u1(i1,i2,i3,ey)+270.*u1(i1,i2+1,i3,ey)-27.*u1(
     & i1,i2+2,i3,ey)+2.*u1(i1,i2+3,i3,ey))/(180.*dr1(1)**2)
       uu2 = u2(j1,j2,j3,ex)
       uu2r = (-u2(j1-3,j2,j3,ex)+9.*u2(j1-2,j2,j3,ex)-45.*u2(j1-1,j2,
     & j3,ex)+45.*u2(j1+1,j2,j3,ex)-9.*u2(j1+2,j2,j3,ex)+u2(j1+3,j2,
     & j3,ex))/(60.*dr2(0))
       uu2s = (-u2(j1,j2-3,j3,ex)+9.*u2(j1,j2-2,j3,ex)-45.*u2(j1,j2-1,
     & j3,ex)+45.*u2(j1,j2+1,j3,ex)-9.*u2(j1,j2+2,j3,ex)+u2(j1,j2+3,
     & j3,ex))/(60.*dr2(1))
       uu2rr = (2.*u2(j1-3,j2,j3,ex)-27.*u2(j1-2,j2,j3,ex)+270.*u2(j1-
     & 1,j2,j3,ex)-490.*u2(j1,j2,j3,ex)+270.*u2(j1+1,j2,j3,ex)-27.*u2(
     & j1+2,j2,j3,ex)+2.*u2(j1+3,j2,j3,ex))/(180.*dr2(0)**2)
       uu2rs = (-(-u2(j1-3,j2-3,j3,ex)+9.*u2(j1-3,j2-2,j3,ex)-45.*u2(
     & j1-3,j2-1,j3,ex)+45.*u2(j1-3,j2+1,j3,ex)-9.*u2(j1-3,j2+2,j3,ex)
     & +u2(j1-3,j2+3,j3,ex))/(60.*dr2(1))+9.*(-u2(j1-2,j2-3,j3,ex)+9.*
     & u2(j1-2,j2-2,j3,ex)-45.*u2(j1-2,j2-1,j3,ex)+45.*u2(j1-2,j2+1,
     & j3,ex)-9.*u2(j1-2,j2+2,j3,ex)+u2(j1-2,j2+3,j3,ex))/(60.*dr2(1))
     & -45.*(-u2(j1-1,j2-3,j3,ex)+9.*u2(j1-1,j2-2,j3,ex)-45.*u2(j1-1,
     & j2-1,j3,ex)+45.*u2(j1-1,j2+1,j3,ex)-9.*u2(j1-1,j2+2,j3,ex)+u2(
     & j1-1,j2+3,j3,ex))/(60.*dr2(1))+45.*(-u2(j1+1,j2-3,j3,ex)+9.*u2(
     & j1+1,j2-2,j3,ex)-45.*u2(j1+1,j2-1,j3,ex)+45.*u2(j1+1,j2+1,j3,
     & ex)-9.*u2(j1+1,j2+2,j3,ex)+u2(j1+1,j2+3,j3,ex))/(60.*dr2(1))-
     & 9.*(-u2(j1+2,j2-3,j3,ex)+9.*u2(j1+2,j2-2,j3,ex)-45.*u2(j1+2,j2-
     & 1,j3,ex)+45.*u2(j1+2,j2+1,j3,ex)-9.*u2(j1+2,j2+2,j3,ex)+u2(j1+
     & 2,j2+3,j3,ex))/(60.*dr2(1))+(-u2(j1+3,j2-3,j3,ex)+9.*u2(j1+3,
     & j2-2,j3,ex)-45.*u2(j1+3,j2-1,j3,ex)+45.*u2(j1+3,j2+1,j3,ex)-9.*
     & u2(j1+3,j2+2,j3,ex)+u2(j1+3,j2+3,j3,ex))/(60.*dr2(1)))/(60.*
     & dr2(0))
       uu2ss = (2.*u2(j1,j2-3,j3,ex)-27.*u2(j1,j2-2,j3,ex)+270.*u2(j1,
     & j2-1,j3,ex)-490.*u2(j1,j2,j3,ex)+270.*u2(j1,j2+1,j3,ex)-27.*u2(
     & j1,j2+2,j3,ex)+2.*u2(j1,j2+3,j3,ex))/(180.*dr2(1)**2)
       vv2 = u2(j1,j2,j3,ey)
       vv2r = (-u2(j1-3,j2,j3,ey)+9.*u2(j1-2,j2,j3,ey)-45.*u2(j1-1,j2,
     & j3,ey)+45.*u2(j1+1,j2,j3,ey)-9.*u2(j1+2,j2,j3,ey)+u2(j1+3,j2,
     & j3,ey))/(60.*dr2(0))
       vv2s = (-u2(j1,j2-3,j3,ey)+9.*u2(j1,j2-2,j3,ey)-45.*u2(j1,j2-1,
     & j3,ey)+45.*u2(j1,j2+1,j3,ey)-9.*u2(j1,j2+2,j3,ey)+u2(j1,j2+3,
     & j3,ey))/(60.*dr2(1))
       vv2rr = (2.*u2(j1-3,j2,j3,ey)-27.*u2(j1-2,j2,j3,ey)+270.*u2(j1-
     & 1,j2,j3,ey)-490.*u2(j1,j2,j3,ey)+270.*u2(j1+1,j2,j3,ey)-27.*u2(
     & j1+2,j2,j3,ey)+2.*u2(j1+3,j2,j3,ey))/(180.*dr2(0)**2)
       vv2rs = (-(-u2(j1-3,j2-3,j3,ey)+9.*u2(j1-3,j2-2,j3,ey)-45.*u2(
     & j1-3,j2-1,j3,ey)+45.*u2(j1-3,j2+1,j3,ey)-9.*u2(j1-3,j2+2,j3,ey)
     & +u2(j1-3,j2+3,j3,ey))/(60.*dr2(1))+9.*(-u2(j1-2,j2-3,j3,ey)+9.*
     & u2(j1-2,j2-2,j3,ey)-45.*u2(j1-2,j2-1,j3,ey)+45.*u2(j1-2,j2+1,
     & j3,ey)-9.*u2(j1-2,j2+2,j3,ey)+u2(j1-2,j2+3,j3,ey))/(60.*dr2(1))
     & -45.*(-u2(j1-1,j2-3,j3,ey)+9.*u2(j1-1,j2-2,j3,ey)-45.*u2(j1-1,
     & j2-1,j3,ey)+45.*u2(j1-1,j2+1,j3,ey)-9.*u2(j1-1,j2+2,j3,ey)+u2(
     & j1-1,j2+3,j3,ey))/(60.*dr2(1))+45.*(-u2(j1+1,j2-3,j3,ey)+9.*u2(
     & j1+1,j2-2,j3,ey)-45.*u2(j1+1,j2-1,j3,ey)+45.*u2(j1+1,j2+1,j3,
     & ey)-9.*u2(j1+1,j2+2,j3,ey)+u2(j1+1,j2+3,j3,ey))/(60.*dr2(1))-
     & 9.*(-u2(j1+2,j2-3,j3,ey)+9.*u2(j1+2,j2-2,j3,ey)-45.*u2(j1+2,j2-
     & 1,j3,ey)+45.*u2(j1+2,j2+1,j3,ey)-9.*u2(j1+2,j2+2,j3,ey)+u2(j1+
     & 2,j2+3,j3,ey))/(60.*dr2(1))+(-u2(j1+3,j2-3,j3,ey)+9.*u2(j1+3,
     & j2-2,j3,ey)-45.*u2(j1+3,j2-1,j3,ey)+45.*u2(j1+3,j2+1,j3,ey)-9.*
     & u2(j1+3,j2+2,j3,ey)+u2(j1+3,j2+3,j3,ey))/(60.*dr2(1)))/(60.*
     & dr2(0))
       vv2ss = (2.*u2(j1,j2-3,j3,ey)-27.*u2(j1,j2-2,j3,ey)+270.*u2(j1,
     & j2-1,j3,ey)-490.*u2(j1,j2,j3,ey)+270.*u2(j1,j2+1,j3,ey)-27.*u2(
     & j1,j2+2,j3,ey)+2.*u2(j1,j2+3,j3,ey))/(180.*dr2(1)**2)
       ! 1st derivatives, 6th order
       uu1x6 = a1j6rx*uu1r+a1j6sx*uu1s
       uu1y6 = a1j6ry*uu1r+a1j6sy*uu1s
       vv1x6 = a1j6rx*vv1r+a1j6sx*vv1s
       vv1y6 = a1j6ry*vv1r+a1j6sy*vv1s
       uu2x6 = a2j6rx*uu2r+a2j6sx*uu2s
       uu2y6 = a2j6ry*uu2r+a2j6sy*uu2s
       vv2x6 = a2j6rx*vv2r+a2j6sx*vv2s
       vv2y6 = a2j6ry*vv2r+a2j6sy*vv2s
       ! 2nd derivatives, 6th order
       t1 = a1j6rx**2
       t6 = a1j6sx**2
       uu1xx6 = t1*uu1rr+2*a1j6rx*a1j6sx*uu1rs+t6*uu1ss+a1j6rxx*uu1r+
     & a1j6sxx*uu1s
       t1 = a1j6ry**2
       t6 = a1j6sy**2
       uu1yy6 = t1*uu1rr+2*a1j6ry*a1j6sy*uu1rs+t6*uu1ss+a1j6ryy*uu1r+
     & a1j6syy*uu1s
       t1 = a1j6rx**2
       t6 = a1j6sx**2
       vv1xx6 = t1*vv1rr+2*a1j6rx*a1j6sx*vv1rs+t6*vv1ss+a1j6rxx*vv1r+
     & a1j6sxx*vv1s
       t1 = a1j6ry**2
       t6 = a1j6sy**2
       vv1yy6 = t1*vv1rr+2*a1j6ry*a1j6sy*vv1rs+t6*vv1ss+a1j6ryy*vv1r+
     & a1j6syy*vv1s
       t1 = a2j6rx**2
       t6 = a2j6sx**2
       uu2xx6 = t1*uu2rr+2*a2j6rx*a2j6sx*uu2rs+t6*uu2ss+a2j6rxx*uu2r+
     & a2j6sxx*uu2s
       t1 = a2j6ry**2
       t6 = a2j6sy**2
       uu2yy6 = t1*uu2rr+2*a2j6ry*a2j6sy*uu2rs+t6*uu2ss+a2j6ryy*uu2r+
     & a2j6syy*uu2s
       t1 = a2j6rx**2
       t6 = a2j6sx**2
       vv2xx6 = t1*vv2rr+2*a2j6rx*a2j6sx*vv2rs+t6*vv2ss+a2j6rxx*vv2r+
     & a2j6sxx*vv2s
       t1 = a2j6ry**2
       t6 = a2j6sy**2
       vv2yy6 = t1*vv2rr+2*a2j6ry*a2j6sy*vv2rs+t6*vv2ss+a2j6ryy*vv2r+
     & a2j6syy*vv2s
       ulap1=uu1xx6+uu1yy6
       vlap1=vv1xx6+vv1yy6
       ulap2=uu2xx6+uu2yy6
       vlap2=vv2xx6+vv2yy6
       ! ****** fourth order ******
       uu1 = u1(i1,i2,i3,ex)
       uu1r = (u1(i1-2,i2,i3,ex)-8.*u1(i1-1,i2,i3,ex)+8.*u1(i1+1,i2,i3,
     & ex)-u1(i1+2,i2,i3,ex))/(12.*dr1(0))
       uu1s = (u1(i1,i2-2,i3,ex)-8.*u1(i1,i2-1,i3,ex)+8.*u1(i1,i2+1,i3,
     & ex)-u1(i1,i2+2,i3,ex))/(12.*dr1(1))
       uu1rr = (-u1(i1-2,i2,i3,ex)+16.*u1(i1-1,i2,i3,ex)-30.*u1(i1,i2,
     & i3,ex)+16.*u1(i1+1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dr1(0)**2)
       uu1rs = ((u1(i1-2,i2-2,i3,ex)-8.*u1(i1-2,i2-1,i3,ex)+8.*u1(i1-2,
     & i2+1,i3,ex)-u1(i1-2,i2+2,i3,ex))/(12.*dr1(1))-8.*(u1(i1-1,i2-2,
     & i3,ex)-8.*u1(i1-1,i2-1,i3,ex)+8.*u1(i1-1,i2+1,i3,ex)-u1(i1-1,
     & i2+2,i3,ex))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,ex)-8.*u1(i1+1,
     & i2-1,i3,ex)+8.*u1(i1+1,i2+1,i3,ex)-u1(i1+1,i2+2,i3,ex))/(12.*
     & dr1(1))-(u1(i1+2,i2-2,i3,ex)-8.*u1(i1+2,i2-1,i3,ex)+8.*u1(i1+2,
     & i2+1,i3,ex)-u1(i1+2,i2+2,i3,ex))/(12.*dr1(1)))/(12.*dr1(0))
       uu1ss = (-u1(i1,i2-2,i3,ex)+16.*u1(i1,i2-1,i3,ex)-30.*u1(i1,i2,
     & i3,ex)+16.*u1(i1,i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dr1(1)**2)
       uu1rrr = (u1(i1-3,i2,i3,ex)-8.*u1(i1-2,i2,i3,ex)+13.*u1(i1-1,i2,
     & i3,ex)-13.*u1(i1+1,i2,i3,ex)+8.*u1(i1+2,i2,i3,ex)-u1(i1+3,i2,
     & i3,ex))/(8.*dr1(0)**3)
       uu1rrs = (-(u1(i1-2,i2-2,i3,ex)-8.*u1(i1-2,i2-1,i3,ex)+8.*u1(i1-
     & 2,i2+1,i3,ex)-u1(i1-2,i2+2,i3,ex))/(12.*dr1(1))+16.*(u1(i1-1,
     & i2-2,i3,ex)-8.*u1(i1-1,i2-1,i3,ex)+8.*u1(i1-1,i2+1,i3,ex)-u1(
     & i1-1,i2+2,i3,ex))/(12.*dr1(1))-30.*(u1(i1,i2-2,i3,ex)-8.*u1(i1,
     & i2-1,i3,ex)+8.*u1(i1,i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dr1(1)
     & )+16.*(u1(i1+1,i2-2,i3,ex)-8.*u1(i1+1,i2-1,i3,ex)+8.*u1(i1+1,
     & i2+1,i3,ex)-u1(i1+1,i2+2,i3,ex))/(12.*dr1(1))-(u1(i1+2,i2-2,i3,
     & ex)-8.*u1(i1+2,i2-1,i3,ex)+8.*u1(i1+2,i2+1,i3,ex)-u1(i1+2,i2+2,
     & i3,ex))/(12.*dr1(1)))/(12.*dr1(0)**2)
       uu1rss = ((-u1(i1-2,i2-2,i3,ex)+16.*u1(i1-2,i2-1,i3,ex)-30.*u1(
     & i1-2,i2,i3,ex)+16.*u1(i1-2,i2+1,i3,ex)-u1(i1-2,i2+2,i3,ex))/(
     & 12.*dr1(1)**2)-8.*(-u1(i1-1,i2-2,i3,ex)+16.*u1(i1-1,i2-1,i3,ex)
     & -30.*u1(i1-1,i2,i3,ex)+16.*u1(i1-1,i2+1,i3,ex)-u1(i1-1,i2+2,i3,
     & ex))/(12.*dr1(1)**2)+8.*(-u1(i1+1,i2-2,i3,ex)+16.*u1(i1+1,i2-1,
     & i3,ex)-30.*u1(i1+1,i2,i3,ex)+16.*u1(i1+1,i2+1,i3,ex)-u1(i1+1,
     & i2+2,i3,ex))/(12.*dr1(1)**2)-(-u1(i1+2,i2-2,i3,ex)+16.*u1(i1+2,
     & i2-1,i3,ex)-30.*u1(i1+2,i2,i3,ex)+16.*u1(i1+2,i2+1,i3,ex)-u1(
     & i1+2,i2+2,i3,ex))/(12.*dr1(1)**2))/(12.*dr1(0))
       uu1sss = (u1(i1,i2-3,i3,ex)-8.*u1(i1,i2-2,i3,ex)+13.*u1(i1,i2-1,
     & i3,ex)-13.*u1(i1,i2+1,i3,ex)+8.*u1(i1,i2+2,i3,ex)-u1(i1,i2+3,
     & i3,ex))/(8.*dr1(1)**3)
       uu1rrrr = (-u1(i1-3,i2,i3,ex)+12.*u1(i1-2,i2,i3,ex)-39.*u1(i1-1,
     & i2,i3,ex)+56.*u1(i1,i2,i3,ex)-39.*u1(i1+1,i2,i3,ex)+12.*u1(i1+
     & 2,i2,i3,ex)-u1(i1+3,i2,i3,ex))/(6.*dr1(0)**4)
       uu1rrrs = ((u1(i1-3,i2-2,i3,ex)-8.*u1(i1-3,i2-1,i3,ex)+8.*u1(i1-
     & 3,i2+1,i3,ex)-u1(i1-3,i2+2,i3,ex))/(12.*dr1(1))-8.*(u1(i1-2,i2-
     & 2,i3,ex)-8.*u1(i1-2,i2-1,i3,ex)+8.*u1(i1-2,i2+1,i3,ex)-u1(i1-2,
     & i2+2,i3,ex))/(12.*dr1(1))+13.*(u1(i1-1,i2-2,i3,ex)-8.*u1(i1-1,
     & i2-1,i3,ex)+8.*u1(i1-1,i2+1,i3,ex)-u1(i1-1,i2+2,i3,ex))/(12.*
     & dr1(1))-13.*(u1(i1+1,i2-2,i3,ex)-8.*u1(i1+1,i2-1,i3,ex)+8.*u1(
     & i1+1,i2+1,i3,ex)-u1(i1+1,i2+2,i3,ex))/(12.*dr1(1))+8.*(u1(i1+2,
     & i2-2,i3,ex)-8.*u1(i1+2,i2-1,i3,ex)+8.*u1(i1+2,i2+1,i3,ex)-u1(
     & i1+2,i2+2,i3,ex))/(12.*dr1(1))-(u1(i1+3,i2-2,i3,ex)-8.*u1(i1+3,
     & i2-1,i3,ex)+8.*u1(i1+3,i2+1,i3,ex)-u1(i1+3,i2+2,i3,ex))/(12.*
     & dr1(1)))/(8.*dr1(0)**3)
       uu1rrss = (-(-u1(i1-2,i2-2,i3,ex)+16.*u1(i1-2,i2-1,i3,ex)-30.*
     & u1(i1-2,i2,i3,ex)+16.*u1(i1-2,i2+1,i3,ex)-u1(i1-2,i2+2,i3,ex))
     & /(12.*dr1(1)**2)+16.*(-u1(i1-1,i2-2,i3,ex)+16.*u1(i1-1,i2-1,i3,
     & ex)-30.*u1(i1-1,i2,i3,ex)+16.*u1(i1-1,i2+1,i3,ex)-u1(i1-1,i2+2,
     & i3,ex))/(12.*dr1(1)**2)-30.*(-u1(i1,i2-2,i3,ex)+16.*u1(i1,i2-1,
     & i3,ex)-30.*u1(i1,i2,i3,ex)+16.*u1(i1,i2+1,i3,ex)-u1(i1,i2+2,i3,
     & ex))/(12.*dr1(1)**2)+16.*(-u1(i1+1,i2-2,i3,ex)+16.*u1(i1+1,i2-
     & 1,i3,ex)-30.*u1(i1+1,i2,i3,ex)+16.*u1(i1+1,i2+1,i3,ex)-u1(i1+1,
     & i2+2,i3,ex))/(12.*dr1(1)**2)-(-u1(i1+2,i2-2,i3,ex)+16.*u1(i1+2,
     & i2-1,i3,ex)-30.*u1(i1+2,i2,i3,ex)+16.*u1(i1+2,i2+1,i3,ex)-u1(
     & i1+2,i2+2,i3,ex))/(12.*dr1(1)**2))/(12.*dr1(0)**2)
       uu1rsss = ((u1(i1-2,i2-3,i3,ex)-8.*u1(i1-2,i2-2,i3,ex)+13.*u1(
     & i1-2,i2-1,i3,ex)-13.*u1(i1-2,i2+1,i3,ex)+8.*u1(i1-2,i2+2,i3,ex)
     & -u1(i1-2,i2+3,i3,ex))/(8.*dr1(1)**3)-8.*(u1(i1-1,i2-3,i3,ex)-
     & 8.*u1(i1-1,i2-2,i3,ex)+13.*u1(i1-1,i2-1,i3,ex)-13.*u1(i1-1,i2+
     & 1,i3,ex)+8.*u1(i1-1,i2+2,i3,ex)-u1(i1-1,i2+3,i3,ex))/(8.*dr1(1)
     & **3)+8.*(u1(i1+1,i2-3,i3,ex)-8.*u1(i1+1,i2-2,i3,ex)+13.*u1(i1+
     & 1,i2-1,i3,ex)-13.*u1(i1+1,i2+1,i3,ex)+8.*u1(i1+1,i2+2,i3,ex)-
     & u1(i1+1,i2+3,i3,ex))/(8.*dr1(1)**3)-(u1(i1+2,i2-3,i3,ex)-8.*u1(
     & i1+2,i2-2,i3,ex)+13.*u1(i1+2,i2-1,i3,ex)-13.*u1(i1+2,i2+1,i3,
     & ex)+8.*u1(i1+2,i2+2,i3,ex)-u1(i1+2,i2+3,i3,ex))/(8.*dr1(1)**3))
     & /(12.*dr1(0))
       uu1ssss = (-u1(i1,i2-3,i3,ex)+12.*u1(i1,i2-2,i3,ex)-39.*u1(i1,
     & i2-1,i3,ex)+56.*u1(i1,i2,i3,ex)-39.*u1(i1,i2+1,i3,ex)+12.*u1(
     & i1,i2+2,i3,ex)-u1(i1,i2+3,i3,ex))/(6.*dr1(1)**4)
       vv1 = u1(i1,i2,i3,ey)
       vv1r = (u1(i1-2,i2,i3,ey)-8.*u1(i1-1,i2,i3,ey)+8.*u1(i1+1,i2,i3,
     & ey)-u1(i1+2,i2,i3,ey))/(12.*dr1(0))
       vv1s = (u1(i1,i2-2,i3,ey)-8.*u1(i1,i2-1,i3,ey)+8.*u1(i1,i2+1,i3,
     & ey)-u1(i1,i2+2,i3,ey))/(12.*dr1(1))
       vv1rr = (-u1(i1-2,i2,i3,ey)+16.*u1(i1-1,i2,i3,ey)-30.*u1(i1,i2,
     & i3,ey)+16.*u1(i1+1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dr1(0)**2)
       vv1rs = ((u1(i1-2,i2-2,i3,ey)-8.*u1(i1-2,i2-1,i3,ey)+8.*u1(i1-2,
     & i2+1,i3,ey)-u1(i1-2,i2+2,i3,ey))/(12.*dr1(1))-8.*(u1(i1-1,i2-2,
     & i3,ey)-8.*u1(i1-1,i2-1,i3,ey)+8.*u1(i1-1,i2+1,i3,ey)-u1(i1-1,
     & i2+2,i3,ey))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,ey)-8.*u1(i1+1,
     & i2-1,i3,ey)+8.*u1(i1+1,i2+1,i3,ey)-u1(i1+1,i2+2,i3,ey))/(12.*
     & dr1(1))-(u1(i1+2,i2-2,i3,ey)-8.*u1(i1+2,i2-1,i3,ey)+8.*u1(i1+2,
     & i2+1,i3,ey)-u1(i1+2,i2+2,i3,ey))/(12.*dr1(1)))/(12.*dr1(0))
       vv1ss = (-u1(i1,i2-2,i3,ey)+16.*u1(i1,i2-1,i3,ey)-30.*u1(i1,i2,
     & i3,ey)+16.*u1(i1,i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dr1(1)**2)
       vv1rrr = (u1(i1-3,i2,i3,ey)-8.*u1(i1-2,i2,i3,ey)+13.*u1(i1-1,i2,
     & i3,ey)-13.*u1(i1+1,i2,i3,ey)+8.*u1(i1+2,i2,i3,ey)-u1(i1+3,i2,
     & i3,ey))/(8.*dr1(0)**3)
       vv1rrs = (-(u1(i1-2,i2-2,i3,ey)-8.*u1(i1-2,i2-1,i3,ey)+8.*u1(i1-
     & 2,i2+1,i3,ey)-u1(i1-2,i2+2,i3,ey))/(12.*dr1(1))+16.*(u1(i1-1,
     & i2-2,i3,ey)-8.*u1(i1-1,i2-1,i3,ey)+8.*u1(i1-1,i2+1,i3,ey)-u1(
     & i1-1,i2+2,i3,ey))/(12.*dr1(1))-30.*(u1(i1,i2-2,i3,ey)-8.*u1(i1,
     & i2-1,i3,ey)+8.*u1(i1,i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dr1(1)
     & )+16.*(u1(i1+1,i2-2,i3,ey)-8.*u1(i1+1,i2-1,i3,ey)+8.*u1(i1+1,
     & i2+1,i3,ey)-u1(i1+1,i2+2,i3,ey))/(12.*dr1(1))-(u1(i1+2,i2-2,i3,
     & ey)-8.*u1(i1+2,i2-1,i3,ey)+8.*u1(i1+2,i2+1,i3,ey)-u1(i1+2,i2+2,
     & i3,ey))/(12.*dr1(1)))/(12.*dr1(0)**2)
       vv1rss = ((-u1(i1-2,i2-2,i3,ey)+16.*u1(i1-2,i2-1,i3,ey)-30.*u1(
     & i1-2,i2,i3,ey)+16.*u1(i1-2,i2+1,i3,ey)-u1(i1-2,i2+2,i3,ey))/(
     & 12.*dr1(1)**2)-8.*(-u1(i1-1,i2-2,i3,ey)+16.*u1(i1-1,i2-1,i3,ey)
     & -30.*u1(i1-1,i2,i3,ey)+16.*u1(i1-1,i2+1,i3,ey)-u1(i1-1,i2+2,i3,
     & ey))/(12.*dr1(1)**2)+8.*(-u1(i1+1,i2-2,i3,ey)+16.*u1(i1+1,i2-1,
     & i3,ey)-30.*u1(i1+1,i2,i3,ey)+16.*u1(i1+1,i2+1,i3,ey)-u1(i1+1,
     & i2+2,i3,ey))/(12.*dr1(1)**2)-(-u1(i1+2,i2-2,i3,ey)+16.*u1(i1+2,
     & i2-1,i3,ey)-30.*u1(i1+2,i2,i3,ey)+16.*u1(i1+2,i2+1,i3,ey)-u1(
     & i1+2,i2+2,i3,ey))/(12.*dr1(1)**2))/(12.*dr1(0))
       vv1sss = (u1(i1,i2-3,i3,ey)-8.*u1(i1,i2-2,i3,ey)+13.*u1(i1,i2-1,
     & i3,ey)-13.*u1(i1,i2+1,i3,ey)+8.*u1(i1,i2+2,i3,ey)-u1(i1,i2+3,
     & i3,ey))/(8.*dr1(1)**3)
       vv1rrrr = (-u1(i1-3,i2,i3,ey)+12.*u1(i1-2,i2,i3,ey)-39.*u1(i1-1,
     & i2,i3,ey)+56.*u1(i1,i2,i3,ey)-39.*u1(i1+1,i2,i3,ey)+12.*u1(i1+
     & 2,i2,i3,ey)-u1(i1+3,i2,i3,ey))/(6.*dr1(0)**4)
       vv1rrrs = ((u1(i1-3,i2-2,i3,ey)-8.*u1(i1-3,i2-1,i3,ey)+8.*u1(i1-
     & 3,i2+1,i3,ey)-u1(i1-3,i2+2,i3,ey))/(12.*dr1(1))-8.*(u1(i1-2,i2-
     & 2,i3,ey)-8.*u1(i1-2,i2-1,i3,ey)+8.*u1(i1-2,i2+1,i3,ey)-u1(i1-2,
     & i2+2,i3,ey))/(12.*dr1(1))+13.*(u1(i1-1,i2-2,i3,ey)-8.*u1(i1-1,
     & i2-1,i3,ey)+8.*u1(i1-1,i2+1,i3,ey)-u1(i1-1,i2+2,i3,ey))/(12.*
     & dr1(1))-13.*(u1(i1+1,i2-2,i3,ey)-8.*u1(i1+1,i2-1,i3,ey)+8.*u1(
     & i1+1,i2+1,i3,ey)-u1(i1+1,i2+2,i3,ey))/(12.*dr1(1))+8.*(u1(i1+2,
     & i2-2,i3,ey)-8.*u1(i1+2,i2-1,i3,ey)+8.*u1(i1+2,i2+1,i3,ey)-u1(
     & i1+2,i2+2,i3,ey))/(12.*dr1(1))-(u1(i1+3,i2-2,i3,ey)-8.*u1(i1+3,
     & i2-1,i3,ey)+8.*u1(i1+3,i2+1,i3,ey)-u1(i1+3,i2+2,i3,ey))/(12.*
     & dr1(1)))/(8.*dr1(0)**3)
       vv1rrss = (-(-u1(i1-2,i2-2,i3,ey)+16.*u1(i1-2,i2-1,i3,ey)-30.*
     & u1(i1-2,i2,i3,ey)+16.*u1(i1-2,i2+1,i3,ey)-u1(i1-2,i2+2,i3,ey))
     & /(12.*dr1(1)**2)+16.*(-u1(i1-1,i2-2,i3,ey)+16.*u1(i1-1,i2-1,i3,
     & ey)-30.*u1(i1-1,i2,i3,ey)+16.*u1(i1-1,i2+1,i3,ey)-u1(i1-1,i2+2,
     & i3,ey))/(12.*dr1(1)**2)-30.*(-u1(i1,i2-2,i3,ey)+16.*u1(i1,i2-1,
     & i3,ey)-30.*u1(i1,i2,i3,ey)+16.*u1(i1,i2+1,i3,ey)-u1(i1,i2+2,i3,
     & ey))/(12.*dr1(1)**2)+16.*(-u1(i1+1,i2-2,i3,ey)+16.*u1(i1+1,i2-
     & 1,i3,ey)-30.*u1(i1+1,i2,i3,ey)+16.*u1(i1+1,i2+1,i3,ey)-u1(i1+1,
     & i2+2,i3,ey))/(12.*dr1(1)**2)-(-u1(i1+2,i2-2,i3,ey)+16.*u1(i1+2,
     & i2-1,i3,ey)-30.*u1(i1+2,i2,i3,ey)+16.*u1(i1+2,i2+1,i3,ey)-u1(
     & i1+2,i2+2,i3,ey))/(12.*dr1(1)**2))/(12.*dr1(0)**2)
       vv1rsss = ((u1(i1-2,i2-3,i3,ey)-8.*u1(i1-2,i2-2,i3,ey)+13.*u1(
     & i1-2,i2-1,i3,ey)-13.*u1(i1-2,i2+1,i3,ey)+8.*u1(i1-2,i2+2,i3,ey)
     & -u1(i1-2,i2+3,i3,ey))/(8.*dr1(1)**3)-8.*(u1(i1-1,i2-3,i3,ey)-
     & 8.*u1(i1-1,i2-2,i3,ey)+13.*u1(i1-1,i2-1,i3,ey)-13.*u1(i1-1,i2+
     & 1,i3,ey)+8.*u1(i1-1,i2+2,i3,ey)-u1(i1-1,i2+3,i3,ey))/(8.*dr1(1)
     & **3)+8.*(u1(i1+1,i2-3,i3,ey)-8.*u1(i1+1,i2-2,i3,ey)+13.*u1(i1+
     & 1,i2-1,i3,ey)-13.*u1(i1+1,i2+1,i3,ey)+8.*u1(i1+1,i2+2,i3,ey)-
     & u1(i1+1,i2+3,i3,ey))/(8.*dr1(1)**3)-(u1(i1+2,i2-3,i3,ey)-8.*u1(
     & i1+2,i2-2,i3,ey)+13.*u1(i1+2,i2-1,i3,ey)-13.*u1(i1+2,i2+1,i3,
     & ey)+8.*u1(i1+2,i2+2,i3,ey)-u1(i1+2,i2+3,i3,ey))/(8.*dr1(1)**3))
     & /(12.*dr1(0))
       vv1ssss = (-u1(i1,i2-3,i3,ey)+12.*u1(i1,i2-2,i3,ey)-39.*u1(i1,
     & i2-1,i3,ey)+56.*u1(i1,i2,i3,ey)-39.*u1(i1,i2+1,i3,ey)+12.*u1(
     & i1,i2+2,i3,ey)-u1(i1,i2+3,i3,ey))/(6.*dr1(1)**4)
       uu2 = u2(j1,j2,j3,ex)
       uu2r = (u2(j1-2,j2,j3,ex)-8.*u2(j1-1,j2,j3,ex)+8.*u2(j1+1,j2,j3,
     & ex)-u2(j1+2,j2,j3,ex))/(12.*dr2(0))
       uu2s = (u2(j1,j2-2,j3,ex)-8.*u2(j1,j2-1,j3,ex)+8.*u2(j1,j2+1,j3,
     & ex)-u2(j1,j2+2,j3,ex))/(12.*dr2(1))
       uu2rr = (-u2(j1-2,j2,j3,ex)+16.*u2(j1-1,j2,j3,ex)-30.*u2(j1,j2,
     & j3,ex)+16.*u2(j1+1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dr2(0)**2)
       uu2rs = ((u2(j1-2,j2-2,j3,ex)-8.*u2(j1-2,j2-1,j3,ex)+8.*u2(j1-2,
     & j2+1,j3,ex)-u2(j1-2,j2+2,j3,ex))/(12.*dr2(1))-8.*(u2(j1-1,j2-2,
     & j3,ex)-8.*u2(j1-1,j2-1,j3,ex)+8.*u2(j1-1,j2+1,j3,ex)-u2(j1-1,
     & j2+2,j3,ex))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,ex)-8.*u2(j1+1,
     & j2-1,j3,ex)+8.*u2(j1+1,j2+1,j3,ex)-u2(j1+1,j2+2,j3,ex))/(12.*
     & dr2(1))-(u2(j1+2,j2-2,j3,ex)-8.*u2(j1+2,j2-1,j3,ex)+8.*u2(j1+2,
     & j2+1,j3,ex)-u2(j1+2,j2+2,j3,ex))/(12.*dr2(1)))/(12.*dr2(0))
       uu2ss = (-u2(j1,j2-2,j3,ex)+16.*u2(j1,j2-1,j3,ex)-30.*u2(j1,j2,
     & j3,ex)+16.*u2(j1,j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dr2(1)**2)
       uu2rrr = (u2(j1-3,j2,j3,ex)-8.*u2(j1-2,j2,j3,ex)+13.*u2(j1-1,j2,
     & j3,ex)-13.*u2(j1+1,j2,j3,ex)+8.*u2(j1+2,j2,j3,ex)-u2(j1+3,j2,
     & j3,ex))/(8.*dr2(0)**3)
       uu2rrs = (-(u2(j1-2,j2-2,j3,ex)-8.*u2(j1-2,j2-1,j3,ex)+8.*u2(j1-
     & 2,j2+1,j3,ex)-u2(j1-2,j2+2,j3,ex))/(12.*dr2(1))+16.*(u2(j1-1,
     & j2-2,j3,ex)-8.*u2(j1-1,j2-1,j3,ex)+8.*u2(j1-1,j2+1,j3,ex)-u2(
     & j1-1,j2+2,j3,ex))/(12.*dr2(1))-30.*(u2(j1,j2-2,j3,ex)-8.*u2(j1,
     & j2-1,j3,ex)+8.*u2(j1,j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dr2(1)
     & )+16.*(u2(j1+1,j2-2,j3,ex)-8.*u2(j1+1,j2-1,j3,ex)+8.*u2(j1+1,
     & j2+1,j3,ex)-u2(j1+1,j2+2,j3,ex))/(12.*dr2(1))-(u2(j1+2,j2-2,j3,
     & ex)-8.*u2(j1+2,j2-1,j3,ex)+8.*u2(j1+2,j2+1,j3,ex)-u2(j1+2,j2+2,
     & j3,ex))/(12.*dr2(1)))/(12.*dr2(0)**2)
       uu2rss = ((-u2(j1-2,j2-2,j3,ex)+16.*u2(j1-2,j2-1,j3,ex)-30.*u2(
     & j1-2,j2,j3,ex)+16.*u2(j1-2,j2+1,j3,ex)-u2(j1-2,j2+2,j3,ex))/(
     & 12.*dr2(1)**2)-8.*(-u2(j1-1,j2-2,j3,ex)+16.*u2(j1-1,j2-1,j3,ex)
     & -30.*u2(j1-1,j2,j3,ex)+16.*u2(j1-1,j2+1,j3,ex)-u2(j1-1,j2+2,j3,
     & ex))/(12.*dr2(1)**2)+8.*(-u2(j1+1,j2-2,j3,ex)+16.*u2(j1+1,j2-1,
     & j3,ex)-30.*u2(j1+1,j2,j3,ex)+16.*u2(j1+1,j2+1,j3,ex)-u2(j1+1,
     & j2+2,j3,ex))/(12.*dr2(1)**2)-(-u2(j1+2,j2-2,j3,ex)+16.*u2(j1+2,
     & j2-1,j3,ex)-30.*u2(j1+2,j2,j3,ex)+16.*u2(j1+2,j2+1,j3,ex)-u2(
     & j1+2,j2+2,j3,ex))/(12.*dr2(1)**2))/(12.*dr2(0))
       uu2sss = (u2(j1,j2-3,j3,ex)-8.*u2(j1,j2-2,j3,ex)+13.*u2(j1,j2-1,
     & j3,ex)-13.*u2(j1,j2+1,j3,ex)+8.*u2(j1,j2+2,j3,ex)-u2(j1,j2+3,
     & j3,ex))/(8.*dr2(1)**3)
       uu2rrrr = (-u2(j1-3,j2,j3,ex)+12.*u2(j1-2,j2,j3,ex)-39.*u2(j1-1,
     & j2,j3,ex)+56.*u2(j1,j2,j3,ex)-39.*u2(j1+1,j2,j3,ex)+12.*u2(j1+
     & 2,j2,j3,ex)-u2(j1+3,j2,j3,ex))/(6.*dr2(0)**4)
       uu2rrrs = ((u2(j1-3,j2-2,j3,ex)-8.*u2(j1-3,j2-1,j3,ex)+8.*u2(j1-
     & 3,j2+1,j3,ex)-u2(j1-3,j2+2,j3,ex))/(12.*dr2(1))-8.*(u2(j1-2,j2-
     & 2,j3,ex)-8.*u2(j1-2,j2-1,j3,ex)+8.*u2(j1-2,j2+1,j3,ex)-u2(j1-2,
     & j2+2,j3,ex))/(12.*dr2(1))+13.*(u2(j1-1,j2-2,j3,ex)-8.*u2(j1-1,
     & j2-1,j3,ex)+8.*u2(j1-1,j2+1,j3,ex)-u2(j1-1,j2+2,j3,ex))/(12.*
     & dr2(1))-13.*(u2(j1+1,j2-2,j3,ex)-8.*u2(j1+1,j2-1,j3,ex)+8.*u2(
     & j1+1,j2+1,j3,ex)-u2(j1+1,j2+2,j3,ex))/(12.*dr2(1))+8.*(u2(j1+2,
     & j2-2,j3,ex)-8.*u2(j1+2,j2-1,j3,ex)+8.*u2(j1+2,j2+1,j3,ex)-u2(
     & j1+2,j2+2,j3,ex))/(12.*dr2(1))-(u2(j1+3,j2-2,j3,ex)-8.*u2(j1+3,
     & j2-1,j3,ex)+8.*u2(j1+3,j2+1,j3,ex)-u2(j1+3,j2+2,j3,ex))/(12.*
     & dr2(1)))/(8.*dr2(0)**3)
       uu2rrss = (-(-u2(j1-2,j2-2,j3,ex)+16.*u2(j1-2,j2-1,j3,ex)-30.*
     & u2(j1-2,j2,j3,ex)+16.*u2(j1-2,j2+1,j3,ex)-u2(j1-2,j2+2,j3,ex))
     & /(12.*dr2(1)**2)+16.*(-u2(j1-1,j2-2,j3,ex)+16.*u2(j1-1,j2-1,j3,
     & ex)-30.*u2(j1-1,j2,j3,ex)+16.*u2(j1-1,j2+1,j3,ex)-u2(j1-1,j2+2,
     & j3,ex))/(12.*dr2(1)**2)-30.*(-u2(j1,j2-2,j3,ex)+16.*u2(j1,j2-1,
     & j3,ex)-30.*u2(j1,j2,j3,ex)+16.*u2(j1,j2+1,j3,ex)-u2(j1,j2+2,j3,
     & ex))/(12.*dr2(1)**2)+16.*(-u2(j1+1,j2-2,j3,ex)+16.*u2(j1+1,j2-
     & 1,j3,ex)-30.*u2(j1+1,j2,j3,ex)+16.*u2(j1+1,j2+1,j3,ex)-u2(j1+1,
     & j2+2,j3,ex))/(12.*dr2(1)**2)-(-u2(j1+2,j2-2,j3,ex)+16.*u2(j1+2,
     & j2-1,j3,ex)-30.*u2(j1+2,j2,j3,ex)+16.*u2(j1+2,j2+1,j3,ex)-u2(
     & j1+2,j2+2,j3,ex))/(12.*dr2(1)**2))/(12.*dr2(0)**2)
       uu2rsss = ((u2(j1-2,j2-3,j3,ex)-8.*u2(j1-2,j2-2,j3,ex)+13.*u2(
     & j1-2,j2-1,j3,ex)-13.*u2(j1-2,j2+1,j3,ex)+8.*u2(j1-2,j2+2,j3,ex)
     & -u2(j1-2,j2+3,j3,ex))/(8.*dr2(1)**3)-8.*(u2(j1-1,j2-3,j3,ex)-
     & 8.*u2(j1-1,j2-2,j3,ex)+13.*u2(j1-1,j2-1,j3,ex)-13.*u2(j1-1,j2+
     & 1,j3,ex)+8.*u2(j1-1,j2+2,j3,ex)-u2(j1-1,j2+3,j3,ex))/(8.*dr2(1)
     & **3)+8.*(u2(j1+1,j2-3,j3,ex)-8.*u2(j1+1,j2-2,j3,ex)+13.*u2(j1+
     & 1,j2-1,j3,ex)-13.*u2(j1+1,j2+1,j3,ex)+8.*u2(j1+1,j2+2,j3,ex)-
     & u2(j1+1,j2+3,j3,ex))/(8.*dr2(1)**3)-(u2(j1+2,j2-3,j3,ex)-8.*u2(
     & j1+2,j2-2,j3,ex)+13.*u2(j1+2,j2-1,j3,ex)-13.*u2(j1+2,j2+1,j3,
     & ex)+8.*u2(j1+2,j2+2,j3,ex)-u2(j1+2,j2+3,j3,ex))/(8.*dr2(1)**3))
     & /(12.*dr2(0))
       uu2ssss = (-u2(j1,j2-3,j3,ex)+12.*u2(j1,j2-2,j3,ex)-39.*u2(j1,
     & j2-1,j3,ex)+56.*u2(j1,j2,j3,ex)-39.*u2(j1,j2+1,j3,ex)+12.*u2(
     & j1,j2+2,j3,ex)-u2(j1,j2+3,j3,ex))/(6.*dr2(1)**4)
       vv2 = u2(j1,j2,j3,ey)
       vv2r = (u2(j1-2,j2,j3,ey)-8.*u2(j1-1,j2,j3,ey)+8.*u2(j1+1,j2,j3,
     & ey)-u2(j1+2,j2,j3,ey))/(12.*dr2(0))
       vv2s = (u2(j1,j2-2,j3,ey)-8.*u2(j1,j2-1,j3,ey)+8.*u2(j1,j2+1,j3,
     & ey)-u2(j1,j2+2,j3,ey))/(12.*dr2(1))
       vv2rr = (-u2(j1-2,j2,j3,ey)+16.*u2(j1-1,j2,j3,ey)-30.*u2(j1,j2,
     & j3,ey)+16.*u2(j1+1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dr2(0)**2)
       vv2rs = ((u2(j1-2,j2-2,j3,ey)-8.*u2(j1-2,j2-1,j3,ey)+8.*u2(j1-2,
     & j2+1,j3,ey)-u2(j1-2,j2+2,j3,ey))/(12.*dr2(1))-8.*(u2(j1-1,j2-2,
     & j3,ey)-8.*u2(j1-1,j2-1,j3,ey)+8.*u2(j1-1,j2+1,j3,ey)-u2(j1-1,
     & j2+2,j3,ey))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,ey)-8.*u2(j1+1,
     & j2-1,j3,ey)+8.*u2(j1+1,j2+1,j3,ey)-u2(j1+1,j2+2,j3,ey))/(12.*
     & dr2(1))-(u2(j1+2,j2-2,j3,ey)-8.*u2(j1+2,j2-1,j3,ey)+8.*u2(j1+2,
     & j2+1,j3,ey)-u2(j1+2,j2+2,j3,ey))/(12.*dr2(1)))/(12.*dr2(0))
       vv2ss = (-u2(j1,j2-2,j3,ey)+16.*u2(j1,j2-1,j3,ey)-30.*u2(j1,j2,
     & j3,ey)+16.*u2(j1,j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dr2(1)**2)
       vv2rrr = (u2(j1-3,j2,j3,ey)-8.*u2(j1-2,j2,j3,ey)+13.*u2(j1-1,j2,
     & j3,ey)-13.*u2(j1+1,j2,j3,ey)+8.*u2(j1+2,j2,j3,ey)-u2(j1+3,j2,
     & j3,ey))/(8.*dr2(0)**3)
       vv2rrs = (-(u2(j1-2,j2-2,j3,ey)-8.*u2(j1-2,j2-1,j3,ey)+8.*u2(j1-
     & 2,j2+1,j3,ey)-u2(j1-2,j2+2,j3,ey))/(12.*dr2(1))+16.*(u2(j1-1,
     & j2-2,j3,ey)-8.*u2(j1-1,j2-1,j3,ey)+8.*u2(j1-1,j2+1,j3,ey)-u2(
     & j1-1,j2+2,j3,ey))/(12.*dr2(1))-30.*(u2(j1,j2-2,j3,ey)-8.*u2(j1,
     & j2-1,j3,ey)+8.*u2(j1,j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dr2(1)
     & )+16.*(u2(j1+1,j2-2,j3,ey)-8.*u2(j1+1,j2-1,j3,ey)+8.*u2(j1+1,
     & j2+1,j3,ey)-u2(j1+1,j2+2,j3,ey))/(12.*dr2(1))-(u2(j1+2,j2-2,j3,
     & ey)-8.*u2(j1+2,j2-1,j3,ey)+8.*u2(j1+2,j2+1,j3,ey)-u2(j1+2,j2+2,
     & j3,ey))/(12.*dr2(1)))/(12.*dr2(0)**2)
       vv2rss = ((-u2(j1-2,j2-2,j3,ey)+16.*u2(j1-2,j2-1,j3,ey)-30.*u2(
     & j1-2,j2,j3,ey)+16.*u2(j1-2,j2+1,j3,ey)-u2(j1-2,j2+2,j3,ey))/(
     & 12.*dr2(1)**2)-8.*(-u2(j1-1,j2-2,j3,ey)+16.*u2(j1-1,j2-1,j3,ey)
     & -30.*u2(j1-1,j2,j3,ey)+16.*u2(j1-1,j2+1,j3,ey)-u2(j1-1,j2+2,j3,
     & ey))/(12.*dr2(1)**2)+8.*(-u2(j1+1,j2-2,j3,ey)+16.*u2(j1+1,j2-1,
     & j3,ey)-30.*u2(j1+1,j2,j3,ey)+16.*u2(j1+1,j2+1,j3,ey)-u2(j1+1,
     & j2+2,j3,ey))/(12.*dr2(1)**2)-(-u2(j1+2,j2-2,j3,ey)+16.*u2(j1+2,
     & j2-1,j3,ey)-30.*u2(j1+2,j2,j3,ey)+16.*u2(j1+2,j2+1,j3,ey)-u2(
     & j1+2,j2+2,j3,ey))/(12.*dr2(1)**2))/(12.*dr2(0))
       vv2sss = (u2(j1,j2-3,j3,ey)-8.*u2(j1,j2-2,j3,ey)+13.*u2(j1,j2-1,
     & j3,ey)-13.*u2(j1,j2+1,j3,ey)+8.*u2(j1,j2+2,j3,ey)-u2(j1,j2+3,
     & j3,ey))/(8.*dr2(1)**3)
       vv2rrrr = (-u2(j1-3,j2,j3,ey)+12.*u2(j1-2,j2,j3,ey)-39.*u2(j1-1,
     & j2,j3,ey)+56.*u2(j1,j2,j3,ey)-39.*u2(j1+1,j2,j3,ey)+12.*u2(j1+
     & 2,j2,j3,ey)-u2(j1+3,j2,j3,ey))/(6.*dr2(0)**4)
       vv2rrrs = ((u2(j1-3,j2-2,j3,ey)-8.*u2(j1-3,j2-1,j3,ey)+8.*u2(j1-
     & 3,j2+1,j3,ey)-u2(j1-3,j2+2,j3,ey))/(12.*dr2(1))-8.*(u2(j1-2,j2-
     & 2,j3,ey)-8.*u2(j1-2,j2-1,j3,ey)+8.*u2(j1-2,j2+1,j3,ey)-u2(j1-2,
     & j2+2,j3,ey))/(12.*dr2(1))+13.*(u2(j1-1,j2-2,j3,ey)-8.*u2(j1-1,
     & j2-1,j3,ey)+8.*u2(j1-1,j2+1,j3,ey)-u2(j1-1,j2+2,j3,ey))/(12.*
     & dr2(1))-13.*(u2(j1+1,j2-2,j3,ey)-8.*u2(j1+1,j2-1,j3,ey)+8.*u2(
     & j1+1,j2+1,j3,ey)-u2(j1+1,j2+2,j3,ey))/(12.*dr2(1))+8.*(u2(j1+2,
     & j2-2,j3,ey)-8.*u2(j1+2,j2-1,j3,ey)+8.*u2(j1+2,j2+1,j3,ey)-u2(
     & j1+2,j2+2,j3,ey))/(12.*dr2(1))-(u2(j1+3,j2-2,j3,ey)-8.*u2(j1+3,
     & j2-1,j3,ey)+8.*u2(j1+3,j2+1,j3,ey)-u2(j1+3,j2+2,j3,ey))/(12.*
     & dr2(1)))/(8.*dr2(0)**3)
       vv2rrss = (-(-u2(j1-2,j2-2,j3,ey)+16.*u2(j1-2,j2-1,j3,ey)-30.*
     & u2(j1-2,j2,j3,ey)+16.*u2(j1-2,j2+1,j3,ey)-u2(j1-2,j2+2,j3,ey))
     & /(12.*dr2(1)**2)+16.*(-u2(j1-1,j2-2,j3,ey)+16.*u2(j1-1,j2-1,j3,
     & ey)-30.*u2(j1-1,j2,j3,ey)+16.*u2(j1-1,j2+1,j3,ey)-u2(j1-1,j2+2,
     & j3,ey))/(12.*dr2(1)**2)-30.*(-u2(j1,j2-2,j3,ey)+16.*u2(j1,j2-1,
     & j3,ey)-30.*u2(j1,j2,j3,ey)+16.*u2(j1,j2+1,j3,ey)-u2(j1,j2+2,j3,
     & ey))/(12.*dr2(1)**2)+16.*(-u2(j1+1,j2-2,j3,ey)+16.*u2(j1+1,j2-
     & 1,j3,ey)-30.*u2(j1+1,j2,j3,ey)+16.*u2(j1+1,j2+1,j3,ey)-u2(j1+1,
     & j2+2,j3,ey))/(12.*dr2(1)**2)-(-u2(j1+2,j2-2,j3,ey)+16.*u2(j1+2,
     & j2-1,j3,ey)-30.*u2(j1+2,j2,j3,ey)+16.*u2(j1+2,j2+1,j3,ey)-u2(
     & j1+2,j2+2,j3,ey))/(12.*dr2(1)**2))/(12.*dr2(0)**2)
       vv2rsss = ((u2(j1-2,j2-3,j3,ey)-8.*u2(j1-2,j2-2,j3,ey)+13.*u2(
     & j1-2,j2-1,j3,ey)-13.*u2(j1-2,j2+1,j3,ey)+8.*u2(j1-2,j2+2,j3,ey)
     & -u2(j1-2,j2+3,j3,ey))/(8.*dr2(1)**3)-8.*(u2(j1-1,j2-3,j3,ey)-
     & 8.*u2(j1-1,j2-2,j3,ey)+13.*u2(j1-1,j2-1,j3,ey)-13.*u2(j1-1,j2+
     & 1,j3,ey)+8.*u2(j1-1,j2+2,j3,ey)-u2(j1-1,j2+3,j3,ey))/(8.*dr2(1)
     & **3)+8.*(u2(j1+1,j2-3,j3,ey)-8.*u2(j1+1,j2-2,j3,ey)+13.*u2(j1+
     & 1,j2-1,j3,ey)-13.*u2(j1+1,j2+1,j3,ey)+8.*u2(j1+1,j2+2,j3,ey)-
     & u2(j1+1,j2+3,j3,ey))/(8.*dr2(1)**3)-(u2(j1+2,j2-3,j3,ey)-8.*u2(
     & j1+2,j2-2,j3,ey)+13.*u2(j1+2,j2-1,j3,ey)-13.*u2(j1+2,j2+1,j3,
     & ey)+8.*u2(j1+2,j2+2,j3,ey)-u2(j1+2,j2+3,j3,ey))/(8.*dr2(1)**3))
     & /(12.*dr2(0))
       vv2ssss = (-u2(j1,j2-3,j3,ey)+12.*u2(j1,j2-2,j3,ey)-39.*u2(j1,
     & j2-1,j3,ey)+56.*u2(j1,j2,j3,ey)-39.*u2(j1,j2+1,j3,ey)+12.*u2(
     & j1,j2+2,j3,ey)-u2(j1,j2+3,j3,ey))/(6.*dr2(1)**4)
       ! 3rd derivatives, 4th order
       t1 = a1j4rx**2
       t7 = a1j4sx**2
       uu1xxx4 = t1*a1j4rx*uu1rrr+3*t1*a1j4sx*uu1rrs+3*a1j4rx*t7*
     & uu1rss+t7*a1j4sx*uu1sss+3*a1j4rx*a1j4rxx*uu1rr+(3*a1j4sxx*
     & a1j4rx+3*a1j4sx*a1j4rxx)*uu1rs+3*a1j4sxx*a1j4sx*uu1ss+a1j4rxxx*
     & uu1r+a1j4sxxx*uu1s
       t1 = a1j4rx**2
       t10 = a1j4sx**2
       uu1xxy4 = a1j4ry*t1*uu1rrr+(a1j4sy*t1+2*a1j4ry*a1j4sx*a1j4rx)*
     & uu1rrs+(a1j4ry*t10+2*a1j4sy*a1j4sx*a1j4rx)*uu1rss+a1j4sy*t10*
     & uu1sss+(2*a1j4rxy*a1j4rx+a1j4ry*a1j4rxx)*uu1rr+(a1j4ry*a1j4sxx+
     & 2*a1j4sx*a1j4rxy+2*a1j4sxy*a1j4rx+a1j4sy*a1j4rxx)*uu1rs+(
     & a1j4sy*a1j4sxx+2*a1j4sxy*a1j4sx)*uu1ss+a1j4rxxy*uu1r+a1j4sxxy*
     & uu1s
       t1 = a1j4ry**2
       t4 = a1j4sy*a1j4ry
       t8 = a1j4sy*a1j4rx+a1j4ry*a1j4sx
       t16 = a1j4sy**2
       uu1xyy4 = t1*a1j4rx*uu1rrr+(t4*a1j4rx+a1j4ry*t8)*uu1rrs+(t4*
     & a1j4sx+a1j4sy*t8)*uu1rss+t16*a1j4sx*uu1sss+(a1j4ryy*a1j4rx+2*
     & a1j4ry*a1j4rxy)*uu1rr+(2*a1j4ry*a1j4sxy+2*a1j4sy*a1j4rxy+
     & a1j4ryy*a1j4sx+a1j4syy*a1j4rx)*uu1rs+(a1j4syy*a1j4sx+2*a1j4sy*
     & a1j4sxy)*uu1ss+a1j4rxyy*uu1r+a1j4sxyy*uu1s
       t1 = a1j4ry**2
       t7 = a1j4sy**2
       uu1yyy4 = a1j4ry*t1*uu1rrr+3*t1*a1j4sy*uu1rrs+3*a1j4ry*t7*
     & uu1rss+t7*a1j4sy*uu1sss+3*a1j4ry*a1j4ryy*uu1rr+(3*a1j4syy*
     & a1j4ry+3*a1j4sy*a1j4ryy)*uu1rs+3*a1j4syy*a1j4sy*uu1ss+a1j4ryyy*
     & uu1r+a1j4syyy*uu1s
       t1 = a1j4rx**2
       t7 = a1j4sx**2
       vv1xxx4 = t1*a1j4rx*vv1rrr+3*t1*a1j4sx*vv1rrs+3*a1j4rx*t7*
     & vv1rss+t7*a1j4sx*vv1sss+3*a1j4rx*a1j4rxx*vv1rr+(3*a1j4sxx*
     & a1j4rx+3*a1j4sx*a1j4rxx)*vv1rs+3*a1j4sxx*a1j4sx*vv1ss+a1j4rxxx*
     & vv1r+a1j4sxxx*vv1s
       t1 = a1j4rx**2
       t10 = a1j4sx**2
       vv1xxy4 = a1j4ry*t1*vv1rrr+(a1j4sy*t1+2*a1j4ry*a1j4sx*a1j4rx)*
     & vv1rrs+(a1j4ry*t10+2*a1j4sy*a1j4sx*a1j4rx)*vv1rss+a1j4sy*t10*
     & vv1sss+(2*a1j4rxy*a1j4rx+a1j4ry*a1j4rxx)*vv1rr+(a1j4ry*a1j4sxx+
     & 2*a1j4sx*a1j4rxy+2*a1j4sxy*a1j4rx+a1j4sy*a1j4rxx)*vv1rs+(
     & a1j4sy*a1j4sxx+2*a1j4sxy*a1j4sx)*vv1ss+a1j4rxxy*vv1r+a1j4sxxy*
     & vv1s
       t1 = a1j4ry**2
       t4 = a1j4sy*a1j4ry
       t8 = a1j4sy*a1j4rx+a1j4ry*a1j4sx
       t16 = a1j4sy**2
       vv1xyy4 = t1*a1j4rx*vv1rrr+(t4*a1j4rx+a1j4ry*t8)*vv1rrs+(t4*
     & a1j4sx+a1j4sy*t8)*vv1rss+t16*a1j4sx*vv1sss+(a1j4ryy*a1j4rx+2*
     & a1j4ry*a1j4rxy)*vv1rr+(2*a1j4ry*a1j4sxy+2*a1j4sy*a1j4rxy+
     & a1j4ryy*a1j4sx+a1j4syy*a1j4rx)*vv1rs+(a1j4syy*a1j4sx+2*a1j4sy*
     & a1j4sxy)*vv1ss+a1j4rxyy*vv1r+a1j4sxyy*vv1s
       t1 = a1j4ry**2
       t7 = a1j4sy**2
       vv1yyy4 = a1j4ry*t1*vv1rrr+3*t1*a1j4sy*vv1rrs+3*a1j4ry*t7*
     & vv1rss+t7*a1j4sy*vv1sss+3*a1j4ry*a1j4ryy*vv1rr+(3*a1j4syy*
     & a1j4ry+3*a1j4sy*a1j4ryy)*vv1rs+3*a1j4syy*a1j4sy*vv1ss+a1j4ryyy*
     & vv1r+a1j4syyy*vv1s
       t1 = a2j4rx**2
       t7 = a2j4sx**2
       uu2xxx4 = t1*a2j4rx*uu2rrr+3*t1*a2j4sx*uu2rrs+3*a2j4rx*t7*
     & uu2rss+t7*a2j4sx*uu2sss+3*a2j4rx*a2j4rxx*uu2rr+(3*a2j4sxx*
     & a2j4rx+3*a2j4sx*a2j4rxx)*uu2rs+3*a2j4sxx*a2j4sx*uu2ss+a2j4rxxx*
     & uu2r+a2j4sxxx*uu2s
       t1 = a2j4rx**2
       t10 = a2j4sx**2
       uu2xxy4 = a2j4ry*t1*uu2rrr+(a2j4sy*t1+2*a2j4ry*a2j4sx*a2j4rx)*
     & uu2rrs+(a2j4ry*t10+2*a2j4sy*a2j4sx*a2j4rx)*uu2rss+a2j4sy*t10*
     & uu2sss+(2*a2j4rxy*a2j4rx+a2j4ry*a2j4rxx)*uu2rr+(a2j4ry*a2j4sxx+
     & 2*a2j4sx*a2j4rxy+2*a2j4sxy*a2j4rx+a2j4sy*a2j4rxx)*uu2rs+(
     & a2j4sy*a2j4sxx+2*a2j4sxy*a2j4sx)*uu2ss+a2j4rxxy*uu2r+a2j4sxxy*
     & uu2s
       t1 = a2j4ry**2
       t4 = a2j4sy*a2j4ry
       t8 = a2j4sy*a2j4rx+a2j4ry*a2j4sx
       t16 = a2j4sy**2
       uu2xyy4 = t1*a2j4rx*uu2rrr+(t4*a2j4rx+a2j4ry*t8)*uu2rrs+(t4*
     & a2j4sx+a2j4sy*t8)*uu2rss+t16*a2j4sx*uu2sss+(a2j4ryy*a2j4rx+2*
     & a2j4ry*a2j4rxy)*uu2rr+(2*a2j4ry*a2j4sxy+2*a2j4sy*a2j4rxy+
     & a2j4ryy*a2j4sx+a2j4syy*a2j4rx)*uu2rs+(a2j4syy*a2j4sx+2*a2j4sy*
     & a2j4sxy)*uu2ss+a2j4rxyy*uu2r+a2j4sxyy*uu2s
       t1 = a2j4ry**2
       t7 = a2j4sy**2
       uu2yyy4 = a2j4ry*t1*uu2rrr+3*t1*a2j4sy*uu2rrs+3*a2j4ry*t7*
     & uu2rss+t7*a2j4sy*uu2sss+3*a2j4ry*a2j4ryy*uu2rr+(3*a2j4syy*
     & a2j4ry+3*a2j4sy*a2j4ryy)*uu2rs+3*a2j4syy*a2j4sy*uu2ss+a2j4ryyy*
     & uu2r+a2j4syyy*uu2s
       t1 = a2j4rx**2
       t7 = a2j4sx**2
       vv2xxx4 = t1*a2j4rx*vv2rrr+3*t1*a2j4sx*vv2rrs+3*a2j4rx*t7*
     & vv2rss+t7*a2j4sx*vv2sss+3*a2j4rx*a2j4rxx*vv2rr+(3*a2j4sxx*
     & a2j4rx+3*a2j4sx*a2j4rxx)*vv2rs+3*a2j4sxx*a2j4sx*vv2ss+a2j4rxxx*
     & vv2r+a2j4sxxx*vv2s
       t1 = a2j4rx**2
       t10 = a2j4sx**2
       vv2xxy4 = a2j4ry*t1*vv2rrr+(a2j4sy*t1+2*a2j4ry*a2j4sx*a2j4rx)*
     & vv2rrs+(a2j4ry*t10+2*a2j4sy*a2j4sx*a2j4rx)*vv2rss+a2j4sy*t10*
     & vv2sss+(2*a2j4rxy*a2j4rx+a2j4ry*a2j4rxx)*vv2rr+(a2j4ry*a2j4sxx+
     & 2*a2j4sx*a2j4rxy+2*a2j4sxy*a2j4rx+a2j4sy*a2j4rxx)*vv2rs+(
     & a2j4sy*a2j4sxx+2*a2j4sxy*a2j4sx)*vv2ss+a2j4rxxy*vv2r+a2j4sxxy*
     & vv2s
       t1 = a2j4ry**2
       t4 = a2j4sy*a2j4ry
       t8 = a2j4sy*a2j4rx+a2j4ry*a2j4sx
       t16 = a2j4sy**2
       vv2xyy4 = t1*a2j4rx*vv2rrr+(t4*a2j4rx+a2j4ry*t8)*vv2rrs+(t4*
     & a2j4sx+a2j4sy*t8)*vv2rss+t16*a2j4sx*vv2sss+(a2j4ryy*a2j4rx+2*
     & a2j4ry*a2j4rxy)*vv2rr+(2*a2j4ry*a2j4sxy+2*a2j4sy*a2j4rxy+
     & a2j4ryy*a2j4sx+a2j4syy*a2j4rx)*vv2rs+(a2j4syy*a2j4sx+2*a2j4sy*
     & a2j4sxy)*vv2ss+a2j4rxyy*vv2r+a2j4sxyy*vv2s
       t1 = a2j4ry**2
       t7 = a2j4sy**2
       vv2yyy4 = a2j4ry*t1*vv2rrr+3*t1*a2j4sy*vv2rrs+3*a2j4ry*t7*
     & vv2rss+t7*a2j4sy*vv2sss+3*a2j4ry*a2j4ryy*vv2rr+(3*a2j4syy*
     & a2j4ry+3*a2j4sy*a2j4ryy)*vv2rs+3*a2j4syy*a2j4sy*vv2ss+a2j4ryyy*
     & vv2r+a2j4syyy*vv2s
       ! 4th derivatives, 4th order
       t1 = a1j4rx**2
       t2 = t1**2
       t8 = a1j4sx**2
       t16 = t8**2
       t25 = a1j4sxx*a1j4rx
       t27 = t25+a1j4sx*a1j4rxx
       t28 = 3*t27
       t30 = 2*t27
       t46 = a1j4rxx**2
       t60 = a1j4sxx**2
       uu1xxxx4 = t2*uu1rrrr+4*t1*a1j4rx*a1j4sx*uu1rrrs+6*t1*t8*
     & uu1rrss+4*a1j4rx*t8*a1j4sx*uu1rsss+t16*uu1ssss+6*t1*a1j4rxx*
     & uu1rrr+(7*a1j4sx*a1j4rx*a1j4rxx+a1j4sxx*t1+a1j4rx*t28+a1j4rx*
     & t30)*uu1rrs+(a1j4sx*t28+7*t25*a1j4sx+a1j4rxx*t8+a1j4sx*t30)*
     & uu1rss+6*t8*a1j4sxx*uu1sss+(4*a1j4rx*a1j4rxxx+3*t46)*uu1rr+(4*
     & a1j4sxxx*a1j4rx+4*a1j4sx*a1j4rxxx+6*a1j4sxx*a1j4rxx)*uu1rs+(4*
     & a1j4sxxx*a1j4sx+3*t60)*uu1ss+a1j4rxxxx*uu1r+a1j4sxxxx*uu1s
       t1 = a1j4ry**2
       t2 = a1j4rx**2
       t5 = a1j4sy*a1j4ry
       t11 = a1j4sy*t2+2*a1j4ry*a1j4sx*a1j4rx
       t16 = a1j4sx**2
       t21 = a1j4ry*t16+2*a1j4sy*a1j4sx*a1j4rx
       t29 = a1j4sy**2
       t38 = 2*a1j4rxy*a1j4rx+a1j4ry*a1j4rxx
       t52 = a1j4sx*a1j4rxy
       t54 = a1j4sxy*a1j4rx
       t57 = a1j4ry*a1j4sxx+2*t52+2*t54+a1j4sy*a1j4rxx
       t60 = 2*t52+2*t54
       t68 = a1j4sy*a1j4sxx+2*a1j4sxy*a1j4sx
       t92 = a1j4rxy**2
       t110 = a1j4sxy**2
       uu1xxyy4 = t1*t2*uu1rrrr+(t5*t2+a1j4ry*t11)*uu1rrrs+(a1j4sy*t11+
     & a1j4ry*t21)*uu1rrss+(a1j4sy*t21+t5*t16)*uu1rsss+t29*t16*
     & uu1ssss+(2*a1j4ry*a1j4rxy*a1j4rx+a1j4ry*t38+a1j4ryy*t2)*uu1rrr+
     & (a1j4sy*t38+2*a1j4sy*a1j4rxy*a1j4rx+2*a1j4ryy*a1j4sx*a1j4rx+
     & a1j4syy*t2+a1j4ry*t57+a1j4ry*t60)*uu1rrs+(a1j4sy*t57+a1j4ry*
     & t68+a1j4ryy*t16+2*a1j4ry*a1j4sxy*a1j4sx+2*a1j4syy*a1j4sx*
     & a1j4rx+a1j4sy*t60)*uu1rss+(2*a1j4sy*a1j4sxy*a1j4sx+a1j4sy*t68+
     & a1j4syy*t16)*uu1sss+(2*a1j4rx*a1j4rxyy+a1j4ryy*a1j4rxx+2*
     & a1j4ry*a1j4rxxy+2*t92)*uu1rr+(4*a1j4sxy*a1j4rxy+2*a1j4ry*
     & a1j4sxxy+a1j4ryy*a1j4sxx+2*a1j4sy*a1j4rxxy+2*a1j4sxyy*a1j4rx+
     & a1j4syy*a1j4rxx+2*a1j4sx*a1j4rxyy)*uu1rs+(2*t110+2*a1j4sy*
     & a1j4sxxy+a1j4syy*a1j4sxx+2*a1j4sx*a1j4sxyy)*uu1ss+a1j4rxxyy*
     & uu1r+a1j4sxxyy*uu1s
       t1 = a1j4ry**2
       t2 = t1**2
       t8 = a1j4sy**2
       t16 = t8**2
       t25 = a1j4syy*a1j4ry
       t27 = t25+a1j4sy*a1j4ryy
       t28 = 3*t27
       t30 = 2*t27
       t46 = a1j4ryy**2
       t60 = a1j4syy**2
       uu1yyyy4 = t2*uu1rrrr+4*t1*a1j4ry*a1j4sy*uu1rrrs+6*t1*t8*
     & uu1rrss+4*a1j4ry*t8*a1j4sy*uu1rsss+t16*uu1ssss+6*t1*a1j4ryy*
     & uu1rrr+(7*a1j4sy*a1j4ry*a1j4ryy+a1j4syy*t1+a1j4ry*t28+a1j4ry*
     & t30)*uu1rrs+(a1j4sy*t28+7*t25*a1j4sy+a1j4ryy*t8+a1j4sy*t30)*
     & uu1rss+6*t8*a1j4syy*uu1sss+(4*a1j4ry*a1j4ryyy+3*t46)*uu1rr+(4*
     & a1j4syyy*a1j4ry+4*a1j4sy*a1j4ryyy+6*a1j4syy*a1j4ryy)*uu1rs+(4*
     & a1j4syyy*a1j4sy+3*t60)*uu1ss+a1j4ryyyy*uu1r+a1j4syyyy*uu1s
       t1 = a1j4rx**2
       t2 = t1**2
       t8 = a1j4sx**2
       t16 = t8**2
       t25 = a1j4sxx*a1j4rx
       t27 = t25+a1j4sx*a1j4rxx
       t28 = 3*t27
       t30 = 2*t27
       t46 = a1j4rxx**2
       t60 = a1j4sxx**2
       vv1xxxx4 = t2*vv1rrrr+4*t1*a1j4rx*a1j4sx*vv1rrrs+6*t1*t8*
     & vv1rrss+4*a1j4rx*t8*a1j4sx*vv1rsss+t16*vv1ssss+6*t1*a1j4rxx*
     & vv1rrr+(7*a1j4sx*a1j4rx*a1j4rxx+a1j4sxx*t1+a1j4rx*t28+a1j4rx*
     & t30)*vv1rrs+(a1j4sx*t28+7*t25*a1j4sx+a1j4rxx*t8+a1j4sx*t30)*
     & vv1rss+6*t8*a1j4sxx*vv1sss+(4*a1j4rx*a1j4rxxx+3*t46)*vv1rr+(4*
     & a1j4sxxx*a1j4rx+4*a1j4sx*a1j4rxxx+6*a1j4sxx*a1j4rxx)*vv1rs+(4*
     & a1j4sxxx*a1j4sx+3*t60)*vv1ss+a1j4rxxxx*vv1r+a1j4sxxxx*vv1s
       t1 = a1j4ry**2
       t2 = a1j4rx**2
       t5 = a1j4sy*a1j4ry
       t11 = a1j4sy*t2+2*a1j4ry*a1j4sx*a1j4rx
       t16 = a1j4sx**2
       t21 = a1j4ry*t16+2*a1j4sy*a1j4sx*a1j4rx
       t29 = a1j4sy**2
       t38 = 2*a1j4rxy*a1j4rx+a1j4ry*a1j4rxx
       t52 = a1j4sx*a1j4rxy
       t54 = a1j4sxy*a1j4rx
       t57 = a1j4ry*a1j4sxx+2*t52+2*t54+a1j4sy*a1j4rxx
       t60 = 2*t52+2*t54
       t68 = a1j4sy*a1j4sxx+2*a1j4sxy*a1j4sx
       t92 = a1j4rxy**2
       t110 = a1j4sxy**2
       vv1xxyy4 = t1*t2*vv1rrrr+(t5*t2+a1j4ry*t11)*vv1rrrs+(a1j4sy*t11+
     & a1j4ry*t21)*vv1rrss+(a1j4sy*t21+t5*t16)*vv1rsss+t29*t16*
     & vv1ssss+(2*a1j4ry*a1j4rxy*a1j4rx+a1j4ry*t38+a1j4ryy*t2)*vv1rrr+
     & (a1j4sy*t38+2*a1j4sy*a1j4rxy*a1j4rx+2*a1j4ryy*a1j4sx*a1j4rx+
     & a1j4syy*t2+a1j4ry*t57+a1j4ry*t60)*vv1rrs+(a1j4sy*t57+a1j4ry*
     & t68+a1j4ryy*t16+2*a1j4ry*a1j4sxy*a1j4sx+2*a1j4syy*a1j4sx*
     & a1j4rx+a1j4sy*t60)*vv1rss+(2*a1j4sy*a1j4sxy*a1j4sx+a1j4sy*t68+
     & a1j4syy*t16)*vv1sss+(2*a1j4rx*a1j4rxyy+a1j4ryy*a1j4rxx+2*
     & a1j4ry*a1j4rxxy+2*t92)*vv1rr+(4*a1j4sxy*a1j4rxy+2*a1j4ry*
     & a1j4sxxy+a1j4ryy*a1j4sxx+2*a1j4sy*a1j4rxxy+2*a1j4sxyy*a1j4rx+
     & a1j4syy*a1j4rxx+2*a1j4sx*a1j4rxyy)*vv1rs+(2*t110+2*a1j4sy*
     & a1j4sxxy+a1j4syy*a1j4sxx+2*a1j4sx*a1j4sxyy)*vv1ss+a1j4rxxyy*
     & vv1r+a1j4sxxyy*vv1s
       t1 = a1j4ry**2
       t2 = t1**2
       t8 = a1j4sy**2
       t16 = t8**2
       t25 = a1j4syy*a1j4ry
       t27 = t25+a1j4sy*a1j4ryy
       t28 = 3*t27
       t30 = 2*t27
       t46 = a1j4ryy**2
       t60 = a1j4syy**2
       vv1yyyy4 = t2*vv1rrrr+4*t1*a1j4ry*a1j4sy*vv1rrrs+6*t1*t8*
     & vv1rrss+4*a1j4ry*t8*a1j4sy*vv1rsss+t16*vv1ssss+6*t1*a1j4ryy*
     & vv1rrr+(7*a1j4sy*a1j4ry*a1j4ryy+a1j4syy*t1+a1j4ry*t28+a1j4ry*
     & t30)*vv1rrs+(a1j4sy*t28+7*t25*a1j4sy+a1j4ryy*t8+a1j4sy*t30)*
     & vv1rss+6*t8*a1j4syy*vv1sss+(4*a1j4ry*a1j4ryyy+3*t46)*vv1rr+(4*
     & a1j4syyy*a1j4ry+4*a1j4sy*a1j4ryyy+6*a1j4syy*a1j4ryy)*vv1rs+(4*
     & a1j4syyy*a1j4sy+3*t60)*vv1ss+a1j4ryyyy*vv1r+a1j4syyyy*vv1s
       t1 = a2j4rx**2
       t2 = t1**2
       t8 = a2j4sx**2
       t16 = t8**2
       t25 = a2j4sxx*a2j4rx
       t27 = t25+a2j4sx*a2j4rxx
       t28 = 3*t27
       t30 = 2*t27
       t46 = a2j4rxx**2
       t60 = a2j4sxx**2
       uu2xxxx4 = t2*uu2rrrr+4*t1*a2j4rx*a2j4sx*uu2rrrs+6*t1*t8*
     & uu2rrss+4*a2j4rx*t8*a2j4sx*uu2rsss+t16*uu2ssss+6*t1*a2j4rxx*
     & uu2rrr+(7*a2j4sx*a2j4rx*a2j4rxx+a2j4sxx*t1+a2j4rx*t28+a2j4rx*
     & t30)*uu2rrs+(a2j4sx*t28+7*t25*a2j4sx+a2j4rxx*t8+a2j4sx*t30)*
     & uu2rss+6*t8*a2j4sxx*uu2sss+(4*a2j4rx*a2j4rxxx+3*t46)*uu2rr+(4*
     & a2j4sxxx*a2j4rx+4*a2j4sx*a2j4rxxx+6*a2j4sxx*a2j4rxx)*uu2rs+(4*
     & a2j4sxxx*a2j4sx+3*t60)*uu2ss+a2j4rxxxx*uu2r+a2j4sxxxx*uu2s
       t1 = a2j4ry**2
       t2 = a2j4rx**2
       t5 = a2j4sy*a2j4ry
       t11 = a2j4sy*t2+2*a2j4ry*a2j4sx*a2j4rx
       t16 = a2j4sx**2
       t21 = a2j4ry*t16+2*a2j4sy*a2j4sx*a2j4rx
       t29 = a2j4sy**2
       t38 = 2*a2j4rxy*a2j4rx+a2j4ry*a2j4rxx
       t52 = a2j4sx*a2j4rxy
       t54 = a2j4sxy*a2j4rx
       t57 = a2j4ry*a2j4sxx+2*t52+2*t54+a2j4sy*a2j4rxx
       t60 = 2*t52+2*t54
       t68 = a2j4sy*a2j4sxx+2*a2j4sxy*a2j4sx
       t92 = a2j4rxy**2
       t110 = a2j4sxy**2
       uu2xxyy4 = t1*t2*uu2rrrr+(t5*t2+a2j4ry*t11)*uu2rrrs+(a2j4sy*t11+
     & a2j4ry*t21)*uu2rrss+(a2j4sy*t21+t5*t16)*uu2rsss+t29*t16*
     & uu2ssss+(2*a2j4ry*a2j4rxy*a2j4rx+a2j4ry*t38+a2j4ryy*t2)*uu2rrr+
     & (a2j4sy*t38+2*a2j4sy*a2j4rxy*a2j4rx+2*a2j4ryy*a2j4sx*a2j4rx+
     & a2j4syy*t2+a2j4ry*t57+a2j4ry*t60)*uu2rrs+(a2j4sy*t57+a2j4ry*
     & t68+a2j4ryy*t16+2*a2j4ry*a2j4sxy*a2j4sx+2*a2j4syy*a2j4sx*
     & a2j4rx+a2j4sy*t60)*uu2rss+(2*a2j4sy*a2j4sxy*a2j4sx+a2j4sy*t68+
     & a2j4syy*t16)*uu2sss+(2*a2j4rx*a2j4rxyy+a2j4ryy*a2j4rxx+2*
     & a2j4ry*a2j4rxxy+2*t92)*uu2rr+(4*a2j4sxy*a2j4rxy+2*a2j4ry*
     & a2j4sxxy+a2j4ryy*a2j4sxx+2*a2j4sy*a2j4rxxy+2*a2j4sxyy*a2j4rx+
     & a2j4syy*a2j4rxx+2*a2j4sx*a2j4rxyy)*uu2rs+(2*t110+2*a2j4sy*
     & a2j4sxxy+a2j4syy*a2j4sxx+2*a2j4sx*a2j4sxyy)*uu2ss+a2j4rxxyy*
     & uu2r+a2j4sxxyy*uu2s
       t1 = a2j4ry**2
       t2 = t1**2
       t8 = a2j4sy**2
       t16 = t8**2
       t25 = a2j4syy*a2j4ry
       t27 = t25+a2j4sy*a2j4ryy
       t28 = 3*t27
       t30 = 2*t27
       t46 = a2j4ryy**2
       t60 = a2j4syy**2
       uu2yyyy4 = t2*uu2rrrr+4*t1*a2j4ry*a2j4sy*uu2rrrs+6*t1*t8*
     & uu2rrss+4*a2j4ry*t8*a2j4sy*uu2rsss+t16*uu2ssss+6*t1*a2j4ryy*
     & uu2rrr+(7*a2j4sy*a2j4ry*a2j4ryy+a2j4syy*t1+a2j4ry*t28+a2j4ry*
     & t30)*uu2rrs+(a2j4sy*t28+7*t25*a2j4sy+a2j4ryy*t8+a2j4sy*t30)*
     & uu2rss+6*t8*a2j4syy*uu2sss+(4*a2j4ry*a2j4ryyy+3*t46)*uu2rr+(4*
     & a2j4syyy*a2j4ry+4*a2j4sy*a2j4ryyy+6*a2j4syy*a2j4ryy)*uu2rs+(4*
     & a2j4syyy*a2j4sy+3*t60)*uu2ss+a2j4ryyyy*uu2r+a2j4syyyy*uu2s
       t1 = a2j4rx**2
       t2 = t1**2
       t8 = a2j4sx**2
       t16 = t8**2
       t25 = a2j4sxx*a2j4rx
       t27 = t25+a2j4sx*a2j4rxx
       t28 = 3*t27
       t30 = 2*t27
       t46 = a2j4rxx**2
       t60 = a2j4sxx**2
       vv2xxxx4 = t2*vv2rrrr+4*t1*a2j4rx*a2j4sx*vv2rrrs+6*t1*t8*
     & vv2rrss+4*a2j4rx*t8*a2j4sx*vv2rsss+t16*vv2ssss+6*t1*a2j4rxx*
     & vv2rrr+(7*a2j4sx*a2j4rx*a2j4rxx+a2j4sxx*t1+a2j4rx*t28+a2j4rx*
     & t30)*vv2rrs+(a2j4sx*t28+7*t25*a2j4sx+a2j4rxx*t8+a2j4sx*t30)*
     & vv2rss+6*t8*a2j4sxx*vv2sss+(4*a2j4rx*a2j4rxxx+3*t46)*vv2rr+(4*
     & a2j4sxxx*a2j4rx+4*a2j4sx*a2j4rxxx+6*a2j4sxx*a2j4rxx)*vv2rs+(4*
     & a2j4sxxx*a2j4sx+3*t60)*vv2ss+a2j4rxxxx*vv2r+a2j4sxxxx*vv2s
       t1 = a2j4ry**2
       t2 = a2j4rx**2
       t5 = a2j4sy*a2j4ry
       t11 = a2j4sy*t2+2*a2j4ry*a2j4sx*a2j4rx
       t16 = a2j4sx**2
       t21 = a2j4ry*t16+2*a2j4sy*a2j4sx*a2j4rx
       t29 = a2j4sy**2
       t38 = 2*a2j4rxy*a2j4rx+a2j4ry*a2j4rxx
       t52 = a2j4sx*a2j4rxy
       t54 = a2j4sxy*a2j4rx
       t57 = a2j4ry*a2j4sxx+2*t52+2*t54+a2j4sy*a2j4rxx
       t60 = 2*t52+2*t54
       t68 = a2j4sy*a2j4sxx+2*a2j4sxy*a2j4sx
       t92 = a2j4rxy**2
       t110 = a2j4sxy**2
       vv2xxyy4 = t1*t2*vv2rrrr+(t5*t2+a2j4ry*t11)*vv2rrrs+(a2j4sy*t11+
     & a2j4ry*t21)*vv2rrss+(a2j4sy*t21+t5*t16)*vv2rsss+t29*t16*
     & vv2ssss+(2*a2j4ry*a2j4rxy*a2j4rx+a2j4ry*t38+a2j4ryy*t2)*vv2rrr+
     & (a2j4sy*t38+2*a2j4sy*a2j4rxy*a2j4rx+2*a2j4ryy*a2j4sx*a2j4rx+
     & a2j4syy*t2+a2j4ry*t57+a2j4ry*t60)*vv2rrs+(a2j4sy*t57+a2j4ry*
     & t68+a2j4ryy*t16+2*a2j4ry*a2j4sxy*a2j4sx+2*a2j4syy*a2j4sx*
     & a2j4rx+a2j4sy*t60)*vv2rss+(2*a2j4sy*a2j4sxy*a2j4sx+a2j4sy*t68+
     & a2j4syy*t16)*vv2sss+(2*a2j4rx*a2j4rxyy+a2j4ryy*a2j4rxx+2*
     & a2j4ry*a2j4rxxy+2*t92)*vv2rr+(4*a2j4sxy*a2j4rxy+2*a2j4ry*
     & a2j4sxxy+a2j4ryy*a2j4sxx+2*a2j4sy*a2j4rxxy+2*a2j4sxyy*a2j4rx+
     & a2j4syy*a2j4rxx+2*a2j4sx*a2j4rxyy)*vv2rs+(2*t110+2*a2j4sy*
     & a2j4sxxy+a2j4syy*a2j4sxx+2*a2j4sx*a2j4sxyy)*vv2ss+a2j4rxxyy*
     & vv2r+a2j4sxxyy*vv2s
       t1 = a2j4ry**2
       t2 = t1**2
       t8 = a2j4sy**2
       t16 = t8**2
       t25 = a2j4syy*a2j4ry
       t27 = t25+a2j4sy*a2j4ryy
       t28 = 3*t27
       t30 = 2*t27
       t46 = a2j4ryy**2
       t60 = a2j4syy**2
       vv2yyyy4 = t2*vv2rrrr+4*t1*a2j4ry*a2j4sy*vv2rrrs+6*t1*t8*
     & vv2rrss+4*a2j4ry*t8*a2j4sy*vv2rsss+t16*vv2ssss+6*t1*a2j4ryy*
     & vv2rrr+(7*a2j4sy*a2j4ry*a2j4ryy+a2j4syy*t1+a2j4ry*t28+a2j4ry*
     & t30)*vv2rrs+(a2j4sy*t28+7*t25*a2j4sy+a2j4ryy*t8+a2j4sy*t30)*
     & vv2rss+6*t8*a2j4syy*vv2sss+(4*a2j4ry*a2j4ryyy+3*t46)*vv2rr+(4*
     & a2j4syyy*a2j4ry+4*a2j4sy*a2j4ryyy+6*a2j4syy*a2j4ryy)*vv2rs+(4*
     & a2j4syyy*a2j4sy+3*t60)*vv2ss+a2j4ryyyy*vv2r+a2j4syyyy*vv2s
       ulapSq1=uu1xxxx4+2.*uu1xxyy4+uu1yyyy4
       vlapSq1=vv1xxxx4+2.*vv1xxyy4+vv1yyyy4
       ulapSq2=uu2xxxx4+2.*uu2xxyy4+uu2yyyy4
       vlapSq2=vv2xxxx4+2.*vv2xxyy4+vv2yyyy4
       ! ****** 2nd order ******
       uu1 = u1(i1,i2,i3,ex)
       uu1r = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dr1(0))
       uu1s = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))
       uu1rr = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,i2,i3,ex))
     & /(dr1(0)**2)
       uu1rs = (-(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*dr1(1))
     & +(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))/(2.*
     & dr1(0))
       uu1ss = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,i3,ex))
     & /(dr1(1)**2)
       uu1rrr = (-u1(i1-2,i2,i3,ex)+2.*u1(i1-1,i2,i3,ex)-2.*u1(i1+1,i2,
     & i3,ex)+u1(i1+2,i2,i3,ex))/(2.*dr1(0)**3)
       uu1rrs = ((-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*dr1(1))
     & -2.*(-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))+(-u1(i1+
     & 1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))/(dr1(0)**2)
       uu1rss = (-(u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(i1-1,i2+
     & 1,i3,ex))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,i2,i3,ex)
     & +u1(i1+1,i2+1,i3,ex))/(dr1(1)**2))/(2.*dr1(0))
       uu1sss = (-u1(i1,i2-2,i3,ex)+2.*u1(i1,i2-1,i3,ex)-2.*u1(i1,i2+1,
     & i3,ex)+u1(i1,i2+2,i3,ex))/(2.*dr1(1)**3)
       uu1rrrr = (u1(i1-2,i2,i3,ex)-4.*u1(i1-1,i2,i3,ex)+6.*u1(i1,i2,
     & i3,ex)-4.*u1(i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(dr1(0)**4)
       uu1rrrs = (-(-u1(i1-2,i2-1,i3,ex)+u1(i1-2,i2+1,i3,ex))/(2.*dr1(
     & 1))+2.*(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*dr1(1))-
     & 2.*(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1))+(-u1(
     & i1+2,i2-1,i3,ex)+u1(i1+2,i2+1,i3,ex))/(2.*dr1(1)))/(2.*dr1(0)**
     & 3)
       uu1rrss = ((u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(i1-1,i2+
     & 1,i3,ex))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+
     & u1(i1,i2+1,i3,ex))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,
     & i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dr1(1)**2))/(dr1(0)**2)
       uu1rsss = (-(-u1(i1-1,i2-2,i3,ex)+2.*u1(i1-1,i2-1,i3,ex)-2.*u1(
     & i1-1,i2+1,i3,ex)+u1(i1-1,i2+2,i3,ex))/(2.*dr1(1)**3)+(-u1(i1+1,
     & i2-2,i3,ex)+2.*u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,i2+1,i3,ex)+u1(
     & i1+1,i2+2,i3,ex))/(2.*dr1(1)**3))/(2.*dr1(0))
       uu1ssss = (u1(i1,i2-2,i3,ex)-4.*u1(i1,i2-1,i3,ex)+6.*u1(i1,i2,
     & i3,ex)-4.*u1(i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(dr1(1)**4)
       uu1rrrrr = (-u1(i1-3,i2,i3,ex)+4.*u1(i1-2,i2,i3,ex)-5.*u1(i1-1,
     & i2,i3,ex)+5.*u1(i1+1,i2,i3,ex)-4.*u1(i1+2,i2,i3,ex)+u1(i1+3,i2,
     & i3,ex))/(2.*dr1(0)**5)
       uu1rrrrs = ((-u1(i1-2,i2-1,i3,ex)+u1(i1-2,i2+1,i3,ex))/(2.*dr1(
     & 1))-4.*(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*dr1(1))+
     & 6.*(-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))-4.*(-u1(
     & i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1))+(-u1(i1+2,i2-
     & 1,i3,ex)+u1(i1+2,i2+1,i3,ex))/(2.*dr1(1)))/(dr1(0)**4)
       uu1rrrss = (-(u1(i1-2,i2-1,i3,ex)-2.*u1(i1-2,i2,i3,ex)+u1(i1-2,
     & i2+1,i3,ex))/(dr1(1)**2)+2.*(u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,
     & i3,ex)+u1(i1-1,i2+1,i3,ex))/(dr1(1)**2)-2.*(u1(i1+1,i2-1,i3,ex)
     & -2.*u1(i1+1,i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dr1(1)**2)+(u1(i1+
     & 2,i2-1,i3,ex)-2.*u1(i1+2,i2,i3,ex)+u1(i1+2,i2+1,i3,ex))/(dr1(1)
     & **2))/(2.*dr1(0)**3)
       uu1rrsss = ((-u1(i1-1,i2-2,i3,ex)+2.*u1(i1-1,i2-1,i3,ex)-2.*u1(
     & i1-1,i2+1,i3,ex)+u1(i1-1,i2+2,i3,ex))/(2.*dr1(1)**3)-2.*(-u1(
     & i1,i2-2,i3,ex)+2.*u1(i1,i2-1,i3,ex)-2.*u1(i1,i2+1,i3,ex)+u1(i1,
     & i2+2,i3,ex))/(2.*dr1(1)**3)+(-u1(i1+1,i2-2,i3,ex)+2.*u1(i1+1,
     & i2-1,i3,ex)-2.*u1(i1+1,i2+1,i3,ex)+u1(i1+1,i2+2,i3,ex))/(2.*
     & dr1(1)**3))/(dr1(0)**2)
       uu1rssss = (-(u1(i1-1,i2-2,i3,ex)-4.*u1(i1-1,i2-1,i3,ex)+6.*u1(
     & i1-1,i2,i3,ex)-4.*u1(i1-1,i2+1,i3,ex)+u1(i1-1,i2+2,i3,ex))/(
     & dr1(1)**4)+(u1(i1+1,i2-2,i3,ex)-4.*u1(i1+1,i2-1,i3,ex)+6.*u1(
     & i1+1,i2,i3,ex)-4.*u1(i1+1,i2+1,i3,ex)+u1(i1+1,i2+2,i3,ex))/(
     & dr1(1)**4))/(2.*dr1(0))
       uu1sssss = (-u1(i1,i2-3,i3,ex)+4.*u1(i1,i2-2,i3,ex)-5.*u1(i1,i2-
     & 1,i3,ex)+5.*u1(i1,i2+1,i3,ex)-4.*u1(i1,i2+2,i3,ex)+u1(i1,i2+3,
     & i3,ex))/(2.*dr1(1)**5)
       uu1rrrrrr = (u1(i1-3,i2,i3,ex)-6.*u1(i1-2,i2,i3,ex)+15.*u1(i1-1,
     & i2,i3,ex)-20.*u1(i1,i2,i3,ex)+15.*u1(i1+1,i2,i3,ex)-6.*u1(i1+2,
     & i2,i3,ex)+u1(i1+3,i2,i3,ex))/(dr1(0)**6)
       uu1rrrrrs = (-(-u1(i1-3,i2-1,i3,ex)+u1(i1-3,i2+1,i3,ex))/(2.*
     & dr1(1))+4.*(-u1(i1-2,i2-1,i3,ex)+u1(i1-2,i2+1,i3,ex))/(2.*dr1(
     & 1))-5.*(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*dr1(1))+
     & 5.*(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1))-4.*(-
     & u1(i1+2,i2-1,i3,ex)+u1(i1+2,i2+1,i3,ex))/(2.*dr1(1))+(-u1(i1+3,
     & i2-1,i3,ex)+u1(i1+3,i2+1,i3,ex))/(2.*dr1(1)))/(2.*dr1(0)**5)
       uu1rrrrss = ((u1(i1-2,i2-1,i3,ex)-2.*u1(i1-2,i2,i3,ex)+u1(i1-2,
     & i2+1,i3,ex))/(dr1(1)**2)-4.*(u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,
     & i3,ex)+u1(i1-1,i2+1,i3,ex))/(dr1(1)**2)+6.*(u1(i1,i2-1,i3,ex)-
     & 2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,i3,ex))/(dr1(1)**2)-4.*(u1(i1+1,
     & i2-1,i3,ex)-2.*u1(i1+1,i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dr1(1)**
     & 2)+(u1(i1+2,i2-1,i3,ex)-2.*u1(i1+2,i2,i3,ex)+u1(i1+2,i2+1,i3,
     & ex))/(dr1(1)**2))/(dr1(0)**4)
       uu1rrrsss = (-(-u1(i1-2,i2-2,i3,ex)+2.*u1(i1-2,i2-1,i3,ex)-2.*
     & u1(i1-2,i2+1,i3,ex)+u1(i1-2,i2+2,i3,ex))/(2.*dr1(1)**3)+2.*(-
     & u1(i1-1,i2-2,i3,ex)+2.*u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2+1,i3,
     & ex)+u1(i1-1,i2+2,i3,ex))/(2.*dr1(1)**3)-2.*(-u1(i1+1,i2-2,i3,
     & ex)+2.*u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,i2+1,i3,ex)+u1(i1+1,i2+2,
     & i3,ex))/(2.*dr1(1)**3)+(-u1(i1+2,i2-2,i3,ex)+2.*u1(i1+2,i2-1,
     & i3,ex)-2.*u1(i1+2,i2+1,i3,ex)+u1(i1+2,i2+2,i3,ex))/(2.*dr1(1)**
     & 3))/(2.*dr1(0)**3)
       uu1rrssss = ((u1(i1-1,i2-2,i3,ex)-4.*u1(i1-1,i2-1,i3,ex)+6.*u1(
     & i1-1,i2,i3,ex)-4.*u1(i1-1,i2+1,i3,ex)+u1(i1-1,i2+2,i3,ex))/(
     & dr1(1)**4)-2.*(u1(i1,i2-2,i3,ex)-4.*u1(i1,i2-1,i3,ex)+6.*u1(i1,
     & i2,i3,ex)-4.*u1(i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(dr1(1)**4)+(
     & u1(i1+1,i2-2,i3,ex)-4.*u1(i1+1,i2-1,i3,ex)+6.*u1(i1+1,i2,i3,ex)
     & -4.*u1(i1+1,i2+1,i3,ex)+u1(i1+1,i2+2,i3,ex))/(dr1(1)**4))/(dr1(
     & 0)**2)
       uu1rsssss = (-(-u1(i1-1,i2-3,i3,ex)+4.*u1(i1-1,i2-2,i3,ex)-5.*
     & u1(i1-1,i2-1,i3,ex)+5.*u1(i1-1,i2+1,i3,ex)-4.*u1(i1-1,i2+2,i3,
     & ex)+u1(i1-1,i2+3,i3,ex))/(2.*dr1(1)**5)+(-u1(i1+1,i2-3,i3,ex)+
     & 4.*u1(i1+1,i2-2,i3,ex)-5.*u1(i1+1,i2-1,i3,ex)+5.*u1(i1+1,i2+1,
     & i3,ex)-4.*u1(i1+1,i2+2,i3,ex)+u1(i1+1,i2+3,i3,ex))/(2.*dr1(1)**
     & 5))/(2.*dr1(0))
       uu1ssssss = (u1(i1,i2-3,i3,ex)-6.*u1(i1,i2-2,i3,ex)+15.*u1(i1,
     & i2-1,i3,ex)-20.*u1(i1,i2,i3,ex)+15.*u1(i1,i2+1,i3,ex)-6.*u1(i1,
     & i2+2,i3,ex)+u1(i1,i2+3,i3,ex))/(dr1(1)**6)
       vv1 = u1(i1,i2,i3,ey)
       vv1r = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dr1(0))
       vv1s = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))
       vv1rr = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,i2,i3,ey))
     & /(dr1(0)**2)
       vv1rs = (-(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*dr1(1))
     & +(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))/(2.*
     & dr1(0))
       vv1ss = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,i3,ey))
     & /(dr1(1)**2)
       vv1rrr = (-u1(i1-2,i2,i3,ey)+2.*u1(i1-1,i2,i3,ey)-2.*u1(i1+1,i2,
     & i3,ey)+u1(i1+2,i2,i3,ey))/(2.*dr1(0)**3)
       vv1rrs = ((-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*dr1(1))
     & -2.*(-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))+(-u1(i1+
     & 1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))/(dr1(0)**2)
       vv1rss = (-(u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(i1-1,i2+
     & 1,i3,ey))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,i2,i3,ey)
     & +u1(i1+1,i2+1,i3,ey))/(dr1(1)**2))/(2.*dr1(0))
       vv1sss = (-u1(i1,i2-2,i3,ey)+2.*u1(i1,i2-1,i3,ey)-2.*u1(i1,i2+1,
     & i3,ey)+u1(i1,i2+2,i3,ey))/(2.*dr1(1)**3)
       vv1rrrr = (u1(i1-2,i2,i3,ey)-4.*u1(i1-1,i2,i3,ey)+6.*u1(i1,i2,
     & i3,ey)-4.*u1(i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(dr1(0)**4)
       vv1rrrs = (-(-u1(i1-2,i2-1,i3,ey)+u1(i1-2,i2+1,i3,ey))/(2.*dr1(
     & 1))+2.*(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*dr1(1))-
     & 2.*(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1))+(-u1(
     & i1+2,i2-1,i3,ey)+u1(i1+2,i2+1,i3,ey))/(2.*dr1(1)))/(2.*dr1(0)**
     & 3)
       vv1rrss = ((u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(i1-1,i2+
     & 1,i3,ey))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+
     & u1(i1,i2+1,i3,ey))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,
     & i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dr1(1)**2))/(dr1(0)**2)
       vv1rsss = (-(-u1(i1-1,i2-2,i3,ey)+2.*u1(i1-1,i2-1,i3,ey)-2.*u1(
     & i1-1,i2+1,i3,ey)+u1(i1-1,i2+2,i3,ey))/(2.*dr1(1)**3)+(-u1(i1+1,
     & i2-2,i3,ey)+2.*u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,i2+1,i3,ey)+u1(
     & i1+1,i2+2,i3,ey))/(2.*dr1(1)**3))/(2.*dr1(0))
       vv1ssss = (u1(i1,i2-2,i3,ey)-4.*u1(i1,i2-1,i3,ey)+6.*u1(i1,i2,
     & i3,ey)-4.*u1(i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(dr1(1)**4)
       vv1rrrrr = (-u1(i1-3,i2,i3,ey)+4.*u1(i1-2,i2,i3,ey)-5.*u1(i1-1,
     & i2,i3,ey)+5.*u1(i1+1,i2,i3,ey)-4.*u1(i1+2,i2,i3,ey)+u1(i1+3,i2,
     & i3,ey))/(2.*dr1(0)**5)
       vv1rrrrs = ((-u1(i1-2,i2-1,i3,ey)+u1(i1-2,i2+1,i3,ey))/(2.*dr1(
     & 1))-4.*(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*dr1(1))+
     & 6.*(-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))-4.*(-u1(
     & i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1))+(-u1(i1+2,i2-
     & 1,i3,ey)+u1(i1+2,i2+1,i3,ey))/(2.*dr1(1)))/(dr1(0)**4)
       vv1rrrss = (-(u1(i1-2,i2-1,i3,ey)-2.*u1(i1-2,i2,i3,ey)+u1(i1-2,
     & i2+1,i3,ey))/(dr1(1)**2)+2.*(u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,
     & i3,ey)+u1(i1-1,i2+1,i3,ey))/(dr1(1)**2)-2.*(u1(i1+1,i2-1,i3,ey)
     & -2.*u1(i1+1,i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dr1(1)**2)+(u1(i1+
     & 2,i2-1,i3,ey)-2.*u1(i1+2,i2,i3,ey)+u1(i1+2,i2+1,i3,ey))/(dr1(1)
     & **2))/(2.*dr1(0)**3)
       vv1rrsss = ((-u1(i1-1,i2-2,i3,ey)+2.*u1(i1-1,i2-1,i3,ey)-2.*u1(
     & i1-1,i2+1,i3,ey)+u1(i1-1,i2+2,i3,ey))/(2.*dr1(1)**3)-2.*(-u1(
     & i1,i2-2,i3,ey)+2.*u1(i1,i2-1,i3,ey)-2.*u1(i1,i2+1,i3,ey)+u1(i1,
     & i2+2,i3,ey))/(2.*dr1(1)**3)+(-u1(i1+1,i2-2,i3,ey)+2.*u1(i1+1,
     & i2-1,i3,ey)-2.*u1(i1+1,i2+1,i3,ey)+u1(i1+1,i2+2,i3,ey))/(2.*
     & dr1(1)**3))/(dr1(0)**2)
       vv1rssss = (-(u1(i1-1,i2-2,i3,ey)-4.*u1(i1-1,i2-1,i3,ey)+6.*u1(
     & i1-1,i2,i3,ey)-4.*u1(i1-1,i2+1,i3,ey)+u1(i1-1,i2+2,i3,ey))/(
     & dr1(1)**4)+(u1(i1+1,i2-2,i3,ey)-4.*u1(i1+1,i2-1,i3,ey)+6.*u1(
     & i1+1,i2,i3,ey)-4.*u1(i1+1,i2+1,i3,ey)+u1(i1+1,i2+2,i3,ey))/(
     & dr1(1)**4))/(2.*dr1(0))
       vv1sssss = (-u1(i1,i2-3,i3,ey)+4.*u1(i1,i2-2,i3,ey)-5.*u1(i1,i2-
     & 1,i3,ey)+5.*u1(i1,i2+1,i3,ey)-4.*u1(i1,i2+2,i3,ey)+u1(i1,i2+3,
     & i3,ey))/(2.*dr1(1)**5)
       vv1rrrrrr = (u1(i1-3,i2,i3,ey)-6.*u1(i1-2,i2,i3,ey)+15.*u1(i1-1,
     & i2,i3,ey)-20.*u1(i1,i2,i3,ey)+15.*u1(i1+1,i2,i3,ey)-6.*u1(i1+2,
     & i2,i3,ey)+u1(i1+3,i2,i3,ey))/(dr1(0)**6)
       vv1rrrrrs = (-(-u1(i1-3,i2-1,i3,ey)+u1(i1-3,i2+1,i3,ey))/(2.*
     & dr1(1))+4.*(-u1(i1-2,i2-1,i3,ey)+u1(i1-2,i2+1,i3,ey))/(2.*dr1(
     & 1))-5.*(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*dr1(1))+
     & 5.*(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1))-4.*(-
     & u1(i1+2,i2-1,i3,ey)+u1(i1+2,i2+1,i3,ey))/(2.*dr1(1))+(-u1(i1+3,
     & i2-1,i3,ey)+u1(i1+3,i2+1,i3,ey))/(2.*dr1(1)))/(2.*dr1(0)**5)
       vv1rrrrss = ((u1(i1-2,i2-1,i3,ey)-2.*u1(i1-2,i2,i3,ey)+u1(i1-2,
     & i2+1,i3,ey))/(dr1(1)**2)-4.*(u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,
     & i3,ey)+u1(i1-1,i2+1,i3,ey))/(dr1(1)**2)+6.*(u1(i1,i2-1,i3,ey)-
     & 2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,i3,ey))/(dr1(1)**2)-4.*(u1(i1+1,
     & i2-1,i3,ey)-2.*u1(i1+1,i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dr1(1)**
     & 2)+(u1(i1+2,i2-1,i3,ey)-2.*u1(i1+2,i2,i3,ey)+u1(i1+2,i2+1,i3,
     & ey))/(dr1(1)**2))/(dr1(0)**4)
       vv1rrrsss = (-(-u1(i1-2,i2-2,i3,ey)+2.*u1(i1-2,i2-1,i3,ey)-2.*
     & u1(i1-2,i2+1,i3,ey)+u1(i1-2,i2+2,i3,ey))/(2.*dr1(1)**3)+2.*(-
     & u1(i1-1,i2-2,i3,ey)+2.*u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2+1,i3,
     & ey)+u1(i1-1,i2+2,i3,ey))/(2.*dr1(1)**3)-2.*(-u1(i1+1,i2-2,i3,
     & ey)+2.*u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,i2+1,i3,ey)+u1(i1+1,i2+2,
     & i3,ey))/(2.*dr1(1)**3)+(-u1(i1+2,i2-2,i3,ey)+2.*u1(i1+2,i2-1,
     & i3,ey)-2.*u1(i1+2,i2+1,i3,ey)+u1(i1+2,i2+2,i3,ey))/(2.*dr1(1)**
     & 3))/(2.*dr1(0)**3)
       vv1rrssss = ((u1(i1-1,i2-2,i3,ey)-4.*u1(i1-1,i2-1,i3,ey)+6.*u1(
     & i1-1,i2,i3,ey)-4.*u1(i1-1,i2+1,i3,ey)+u1(i1-1,i2+2,i3,ey))/(
     & dr1(1)**4)-2.*(u1(i1,i2-2,i3,ey)-4.*u1(i1,i2-1,i3,ey)+6.*u1(i1,
     & i2,i3,ey)-4.*u1(i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(dr1(1)**4)+(
     & u1(i1+1,i2-2,i3,ey)-4.*u1(i1+1,i2-1,i3,ey)+6.*u1(i1+1,i2,i3,ey)
     & -4.*u1(i1+1,i2+1,i3,ey)+u1(i1+1,i2+2,i3,ey))/(dr1(1)**4))/(dr1(
     & 0)**2)
       vv1rsssss = (-(-u1(i1-1,i2-3,i3,ey)+4.*u1(i1-1,i2-2,i3,ey)-5.*
     & u1(i1-1,i2-1,i3,ey)+5.*u1(i1-1,i2+1,i3,ey)-4.*u1(i1-1,i2+2,i3,
     & ey)+u1(i1-1,i2+3,i3,ey))/(2.*dr1(1)**5)+(-u1(i1+1,i2-3,i3,ey)+
     & 4.*u1(i1+1,i2-2,i3,ey)-5.*u1(i1+1,i2-1,i3,ey)+5.*u1(i1+1,i2+1,
     & i3,ey)-4.*u1(i1+1,i2+2,i3,ey)+u1(i1+1,i2+3,i3,ey))/(2.*dr1(1)**
     & 5))/(2.*dr1(0))
       vv1ssssss = (u1(i1,i2-3,i3,ey)-6.*u1(i1,i2-2,i3,ey)+15.*u1(i1,
     & i2-1,i3,ey)-20.*u1(i1,i2,i3,ey)+15.*u1(i1,i2+1,i3,ey)-6.*u1(i1,
     & i2+2,i3,ey)+u1(i1,i2+3,i3,ey))/(dr1(1)**6)
       uu2 = u2(j1,j2,j3,ex)
       uu2r = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dr2(0))
       uu2s = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))
       uu2rr = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,j2,j3,ex))
     & /(dr2(0)**2)
       uu2rs = (-(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*dr2(1))
     & +(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))/(2.*
     & dr2(0))
       uu2ss = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,j3,ex))
     & /(dr2(1)**2)
       uu2rrr = (-u2(j1-2,j2,j3,ex)+2.*u2(j1-1,j2,j3,ex)-2.*u2(j1+1,j2,
     & j3,ex)+u2(j1+2,j2,j3,ex))/(2.*dr2(0)**3)
       uu2rrs = ((-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*dr2(1))
     & -2.*(-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))+(-u2(j1+
     & 1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))/(dr2(0)**2)
       uu2rss = (-(u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(j1-1,j2+
     & 1,j3,ex))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,j2,j3,ex)
     & +u2(j1+1,j2+1,j3,ex))/(dr2(1)**2))/(2.*dr2(0))
       uu2sss = (-u2(j1,j2-2,j3,ex)+2.*u2(j1,j2-1,j3,ex)-2.*u2(j1,j2+1,
     & j3,ex)+u2(j1,j2+2,j3,ex))/(2.*dr2(1)**3)
       uu2rrrr = (u2(j1-2,j2,j3,ex)-4.*u2(j1-1,j2,j3,ex)+6.*u2(j1,j2,
     & j3,ex)-4.*u2(j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(dr2(0)**4)
       uu2rrrs = (-(-u2(j1-2,j2-1,j3,ex)+u2(j1-2,j2+1,j3,ex))/(2.*dr2(
     & 1))+2.*(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*dr2(1))-
     & 2.*(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1))+(-u2(
     & j1+2,j2-1,j3,ex)+u2(j1+2,j2+1,j3,ex))/(2.*dr2(1)))/(2.*dr2(0)**
     & 3)
       uu2rrss = ((u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(j1-1,j2+
     & 1,j3,ex))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+
     & u2(j1,j2+1,j3,ex))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,
     & j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dr2(1)**2))/(dr2(0)**2)
       uu2rsss = (-(-u2(j1-1,j2-2,j3,ex)+2.*u2(j1-1,j2-1,j3,ex)-2.*u2(
     & j1-1,j2+1,j3,ex)+u2(j1-1,j2+2,j3,ex))/(2.*dr2(1)**3)+(-u2(j1+1,
     & j2-2,j3,ex)+2.*u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,j2+1,j3,ex)+u2(
     & j1+1,j2+2,j3,ex))/(2.*dr2(1)**3))/(2.*dr2(0))
       uu2ssss = (u2(j1,j2-2,j3,ex)-4.*u2(j1,j2-1,j3,ex)+6.*u2(j1,j2,
     & j3,ex)-4.*u2(j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(dr2(1)**4)
       uu2rrrrr = (-u2(j1-3,j2,j3,ex)+4.*u2(j1-2,j2,j3,ex)-5.*u2(j1-1,
     & j2,j3,ex)+5.*u2(j1+1,j2,j3,ex)-4.*u2(j1+2,j2,j3,ex)+u2(j1+3,j2,
     & j3,ex))/(2.*dr2(0)**5)
       uu2rrrrs = ((-u2(j1-2,j2-1,j3,ex)+u2(j1-2,j2+1,j3,ex))/(2.*dr2(
     & 1))-4.*(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*dr2(1))+
     & 6.*(-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))-4.*(-u2(
     & j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1))+(-u2(j1+2,j2-
     & 1,j3,ex)+u2(j1+2,j2+1,j3,ex))/(2.*dr2(1)))/(dr2(0)**4)
       uu2rrrss = (-(u2(j1-2,j2-1,j3,ex)-2.*u2(j1-2,j2,j3,ex)+u2(j1-2,
     & j2+1,j3,ex))/(dr2(1)**2)+2.*(u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,
     & j3,ex)+u2(j1-1,j2+1,j3,ex))/(dr2(1)**2)-2.*(u2(j1+1,j2-1,j3,ex)
     & -2.*u2(j1+1,j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dr2(1)**2)+(u2(j1+
     & 2,j2-1,j3,ex)-2.*u2(j1+2,j2,j3,ex)+u2(j1+2,j2+1,j3,ex))/(dr2(1)
     & **2))/(2.*dr2(0)**3)
       uu2rrsss = ((-u2(j1-1,j2-2,j3,ex)+2.*u2(j1-1,j2-1,j3,ex)-2.*u2(
     & j1-1,j2+1,j3,ex)+u2(j1-1,j2+2,j3,ex))/(2.*dr2(1)**3)-2.*(-u2(
     & j1,j2-2,j3,ex)+2.*u2(j1,j2-1,j3,ex)-2.*u2(j1,j2+1,j3,ex)+u2(j1,
     & j2+2,j3,ex))/(2.*dr2(1)**3)+(-u2(j1+1,j2-2,j3,ex)+2.*u2(j1+1,
     & j2-1,j3,ex)-2.*u2(j1+1,j2+1,j3,ex)+u2(j1+1,j2+2,j3,ex))/(2.*
     & dr2(1)**3))/(dr2(0)**2)
       uu2rssss = (-(u2(j1-1,j2-2,j3,ex)-4.*u2(j1-1,j2-1,j3,ex)+6.*u2(
     & j1-1,j2,j3,ex)-4.*u2(j1-1,j2+1,j3,ex)+u2(j1-1,j2+2,j3,ex))/(
     & dr2(1)**4)+(u2(j1+1,j2-2,j3,ex)-4.*u2(j1+1,j2-1,j3,ex)+6.*u2(
     & j1+1,j2,j3,ex)-4.*u2(j1+1,j2+1,j3,ex)+u2(j1+1,j2+2,j3,ex))/(
     & dr2(1)**4))/(2.*dr2(0))
       uu2sssss = (-u2(j1,j2-3,j3,ex)+4.*u2(j1,j2-2,j3,ex)-5.*u2(j1,j2-
     & 1,j3,ex)+5.*u2(j1,j2+1,j3,ex)-4.*u2(j1,j2+2,j3,ex)+u2(j1,j2+3,
     & j3,ex))/(2.*dr2(1)**5)
       uu2rrrrrr = (u2(j1-3,j2,j3,ex)-6.*u2(j1-2,j2,j3,ex)+15.*u2(j1-1,
     & j2,j3,ex)-20.*u2(j1,j2,j3,ex)+15.*u2(j1+1,j2,j3,ex)-6.*u2(j1+2,
     & j2,j3,ex)+u2(j1+3,j2,j3,ex))/(dr2(0)**6)
       uu2rrrrrs = (-(-u2(j1-3,j2-1,j3,ex)+u2(j1-3,j2+1,j3,ex))/(2.*
     & dr2(1))+4.*(-u2(j1-2,j2-1,j3,ex)+u2(j1-2,j2+1,j3,ex))/(2.*dr2(
     & 1))-5.*(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*dr2(1))+
     & 5.*(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1))-4.*(-
     & u2(j1+2,j2-1,j3,ex)+u2(j1+2,j2+1,j3,ex))/(2.*dr2(1))+(-u2(j1+3,
     & j2-1,j3,ex)+u2(j1+3,j2+1,j3,ex))/(2.*dr2(1)))/(2.*dr2(0)**5)
       uu2rrrrss = ((u2(j1-2,j2-1,j3,ex)-2.*u2(j1-2,j2,j3,ex)+u2(j1-2,
     & j2+1,j3,ex))/(dr2(1)**2)-4.*(u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,
     & j3,ex)+u2(j1-1,j2+1,j3,ex))/(dr2(1)**2)+6.*(u2(j1,j2-1,j3,ex)-
     & 2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,j3,ex))/(dr2(1)**2)-4.*(u2(j1+1,
     & j2-1,j3,ex)-2.*u2(j1+1,j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dr2(1)**
     & 2)+(u2(j1+2,j2-1,j3,ex)-2.*u2(j1+2,j2,j3,ex)+u2(j1+2,j2+1,j3,
     & ex))/(dr2(1)**2))/(dr2(0)**4)
       uu2rrrsss = (-(-u2(j1-2,j2-2,j3,ex)+2.*u2(j1-2,j2-1,j3,ex)-2.*
     & u2(j1-2,j2+1,j3,ex)+u2(j1-2,j2+2,j3,ex))/(2.*dr2(1)**3)+2.*(-
     & u2(j1-1,j2-2,j3,ex)+2.*u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2+1,j3,
     & ex)+u2(j1-1,j2+2,j3,ex))/(2.*dr2(1)**3)-2.*(-u2(j1+1,j2-2,j3,
     & ex)+2.*u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,j2+1,j3,ex)+u2(j1+1,j2+2,
     & j3,ex))/(2.*dr2(1)**3)+(-u2(j1+2,j2-2,j3,ex)+2.*u2(j1+2,j2-1,
     & j3,ex)-2.*u2(j1+2,j2+1,j3,ex)+u2(j1+2,j2+2,j3,ex))/(2.*dr2(1)**
     & 3))/(2.*dr2(0)**3)
       uu2rrssss = ((u2(j1-1,j2-2,j3,ex)-4.*u2(j1-1,j2-1,j3,ex)+6.*u2(
     & j1-1,j2,j3,ex)-4.*u2(j1-1,j2+1,j3,ex)+u2(j1-1,j2+2,j3,ex))/(
     & dr2(1)**4)-2.*(u2(j1,j2-2,j3,ex)-4.*u2(j1,j2-1,j3,ex)+6.*u2(j1,
     & j2,j3,ex)-4.*u2(j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(dr2(1)**4)+(
     & u2(j1+1,j2-2,j3,ex)-4.*u2(j1+1,j2-1,j3,ex)+6.*u2(j1+1,j2,j3,ex)
     & -4.*u2(j1+1,j2+1,j3,ex)+u2(j1+1,j2+2,j3,ex))/(dr2(1)**4))/(dr2(
     & 0)**2)
       uu2rsssss = (-(-u2(j1-1,j2-3,j3,ex)+4.*u2(j1-1,j2-2,j3,ex)-5.*
     & u2(j1-1,j2-1,j3,ex)+5.*u2(j1-1,j2+1,j3,ex)-4.*u2(j1-1,j2+2,j3,
     & ex)+u2(j1-1,j2+3,j3,ex))/(2.*dr2(1)**5)+(-u2(j1+1,j2-3,j3,ex)+
     & 4.*u2(j1+1,j2-2,j3,ex)-5.*u2(j1+1,j2-1,j3,ex)+5.*u2(j1+1,j2+1,
     & j3,ex)-4.*u2(j1+1,j2+2,j3,ex)+u2(j1+1,j2+3,j3,ex))/(2.*dr2(1)**
     & 5))/(2.*dr2(0))
       uu2ssssss = (u2(j1,j2-3,j3,ex)-6.*u2(j1,j2-2,j3,ex)+15.*u2(j1,
     & j2-1,j3,ex)-20.*u2(j1,j2,j3,ex)+15.*u2(j1,j2+1,j3,ex)-6.*u2(j1,
     & j2+2,j3,ex)+u2(j1,j2+3,j3,ex))/(dr2(1)**6)
       vv2 = u2(j1,j2,j3,ey)
       vv2r = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dr2(0))
       vv2s = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))
       vv2rr = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,j2,j3,ey))
     & /(dr2(0)**2)
       vv2rs = (-(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*dr2(1))
     & +(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))/(2.*
     & dr2(0))
       vv2ss = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,j3,ey))
     & /(dr2(1)**2)
       vv2rrr = (-u2(j1-2,j2,j3,ey)+2.*u2(j1-1,j2,j3,ey)-2.*u2(j1+1,j2,
     & j3,ey)+u2(j1+2,j2,j3,ey))/(2.*dr2(0)**3)
       vv2rrs = ((-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*dr2(1))
     & -2.*(-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))+(-u2(j1+
     & 1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))/(dr2(0)**2)
       vv2rss = (-(u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(j1-1,j2+
     & 1,j3,ey))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,j2,j3,ey)
     & +u2(j1+1,j2+1,j3,ey))/(dr2(1)**2))/(2.*dr2(0))
       vv2sss = (-u2(j1,j2-2,j3,ey)+2.*u2(j1,j2-1,j3,ey)-2.*u2(j1,j2+1,
     & j3,ey)+u2(j1,j2+2,j3,ey))/(2.*dr2(1)**3)
       vv2rrrr = (u2(j1-2,j2,j3,ey)-4.*u2(j1-1,j2,j3,ey)+6.*u2(j1,j2,
     & j3,ey)-4.*u2(j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(dr2(0)**4)
       vv2rrrs = (-(-u2(j1-2,j2-1,j3,ey)+u2(j1-2,j2+1,j3,ey))/(2.*dr2(
     & 1))+2.*(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*dr2(1))-
     & 2.*(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1))+(-u2(
     & j1+2,j2-1,j3,ey)+u2(j1+2,j2+1,j3,ey))/(2.*dr2(1)))/(2.*dr2(0)**
     & 3)
       vv2rrss = ((u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(j1-1,j2+
     & 1,j3,ey))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+
     & u2(j1,j2+1,j3,ey))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,
     & j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dr2(1)**2))/(dr2(0)**2)
       vv2rsss = (-(-u2(j1-1,j2-2,j3,ey)+2.*u2(j1-1,j2-1,j3,ey)-2.*u2(
     & j1-1,j2+1,j3,ey)+u2(j1-1,j2+2,j3,ey))/(2.*dr2(1)**3)+(-u2(j1+1,
     & j2-2,j3,ey)+2.*u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,j2+1,j3,ey)+u2(
     & j1+1,j2+2,j3,ey))/(2.*dr2(1)**3))/(2.*dr2(0))
       vv2ssss = (u2(j1,j2-2,j3,ey)-4.*u2(j1,j2-1,j3,ey)+6.*u2(j1,j2,
     & j3,ey)-4.*u2(j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(dr2(1)**4)
       vv2rrrrr = (-u2(j1-3,j2,j3,ey)+4.*u2(j1-2,j2,j3,ey)-5.*u2(j1-1,
     & j2,j3,ey)+5.*u2(j1+1,j2,j3,ey)-4.*u2(j1+2,j2,j3,ey)+u2(j1+3,j2,
     & j3,ey))/(2.*dr2(0)**5)
       vv2rrrrs = ((-u2(j1-2,j2-1,j3,ey)+u2(j1-2,j2+1,j3,ey))/(2.*dr2(
     & 1))-4.*(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*dr2(1))+
     & 6.*(-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))-4.*(-u2(
     & j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1))+(-u2(j1+2,j2-
     & 1,j3,ey)+u2(j1+2,j2+1,j3,ey))/(2.*dr2(1)))/(dr2(0)**4)
       vv2rrrss = (-(u2(j1-2,j2-1,j3,ey)-2.*u2(j1-2,j2,j3,ey)+u2(j1-2,
     & j2+1,j3,ey))/(dr2(1)**2)+2.*(u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,
     & j3,ey)+u2(j1-1,j2+1,j3,ey))/(dr2(1)**2)-2.*(u2(j1+1,j2-1,j3,ey)
     & -2.*u2(j1+1,j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dr2(1)**2)+(u2(j1+
     & 2,j2-1,j3,ey)-2.*u2(j1+2,j2,j3,ey)+u2(j1+2,j2+1,j3,ey))/(dr2(1)
     & **2))/(2.*dr2(0)**3)
       vv2rrsss = ((-u2(j1-1,j2-2,j3,ey)+2.*u2(j1-1,j2-1,j3,ey)-2.*u2(
     & j1-1,j2+1,j3,ey)+u2(j1-1,j2+2,j3,ey))/(2.*dr2(1)**3)-2.*(-u2(
     & j1,j2-2,j3,ey)+2.*u2(j1,j2-1,j3,ey)-2.*u2(j1,j2+1,j3,ey)+u2(j1,
     & j2+2,j3,ey))/(2.*dr2(1)**3)+(-u2(j1+1,j2-2,j3,ey)+2.*u2(j1+1,
     & j2-1,j3,ey)-2.*u2(j1+1,j2+1,j3,ey)+u2(j1+1,j2+2,j3,ey))/(2.*
     & dr2(1)**3))/(dr2(0)**2)
       vv2rssss = (-(u2(j1-1,j2-2,j3,ey)-4.*u2(j1-1,j2-1,j3,ey)+6.*u2(
     & j1-1,j2,j3,ey)-4.*u2(j1-1,j2+1,j3,ey)+u2(j1-1,j2+2,j3,ey))/(
     & dr2(1)**4)+(u2(j1+1,j2-2,j3,ey)-4.*u2(j1+1,j2-1,j3,ey)+6.*u2(
     & j1+1,j2,j3,ey)-4.*u2(j1+1,j2+1,j3,ey)+u2(j1+1,j2+2,j3,ey))/(
     & dr2(1)**4))/(2.*dr2(0))
       vv2sssss = (-u2(j1,j2-3,j3,ey)+4.*u2(j1,j2-2,j3,ey)-5.*u2(j1,j2-
     & 1,j3,ey)+5.*u2(j1,j2+1,j3,ey)-4.*u2(j1,j2+2,j3,ey)+u2(j1,j2+3,
     & j3,ey))/(2.*dr2(1)**5)
       vv2rrrrrr = (u2(j1-3,j2,j3,ey)-6.*u2(j1-2,j2,j3,ey)+15.*u2(j1-1,
     & j2,j3,ey)-20.*u2(j1,j2,j3,ey)+15.*u2(j1+1,j2,j3,ey)-6.*u2(j1+2,
     & j2,j3,ey)+u2(j1+3,j2,j3,ey))/(dr2(0)**6)
       vv2rrrrrs = (-(-u2(j1-3,j2-1,j3,ey)+u2(j1-3,j2+1,j3,ey))/(2.*
     & dr2(1))+4.*(-u2(j1-2,j2-1,j3,ey)+u2(j1-2,j2+1,j3,ey))/(2.*dr2(
     & 1))-5.*(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*dr2(1))+
     & 5.*(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1))-4.*(-
     & u2(j1+2,j2-1,j3,ey)+u2(j1+2,j2+1,j3,ey))/(2.*dr2(1))+(-u2(j1+3,
     & j2-1,j3,ey)+u2(j1+3,j2+1,j3,ey))/(2.*dr2(1)))/(2.*dr2(0)**5)
       vv2rrrrss = ((u2(j1-2,j2-1,j3,ey)-2.*u2(j1-2,j2,j3,ey)+u2(j1-2,
     & j2+1,j3,ey))/(dr2(1)**2)-4.*(u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,
     & j3,ey)+u2(j1-1,j2+1,j3,ey))/(dr2(1)**2)+6.*(u2(j1,j2-1,j3,ey)-
     & 2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,j3,ey))/(dr2(1)**2)-4.*(u2(j1+1,
     & j2-1,j3,ey)-2.*u2(j1+1,j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dr2(1)**
     & 2)+(u2(j1+2,j2-1,j3,ey)-2.*u2(j1+2,j2,j3,ey)+u2(j1+2,j2+1,j3,
     & ey))/(dr2(1)**2))/(dr2(0)**4)
       vv2rrrsss = (-(-u2(j1-2,j2-2,j3,ey)+2.*u2(j1-2,j2-1,j3,ey)-2.*
     & u2(j1-2,j2+1,j3,ey)+u2(j1-2,j2+2,j3,ey))/(2.*dr2(1)**3)+2.*(-
     & u2(j1-1,j2-2,j3,ey)+2.*u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2+1,j3,
     & ey)+u2(j1-1,j2+2,j3,ey))/(2.*dr2(1)**3)-2.*(-u2(j1+1,j2-2,j3,
     & ey)+2.*u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,j2+1,j3,ey)+u2(j1+1,j2+2,
     & j3,ey))/(2.*dr2(1)**3)+(-u2(j1+2,j2-2,j3,ey)+2.*u2(j1+2,j2-1,
     & j3,ey)-2.*u2(j1+2,j2+1,j3,ey)+u2(j1+2,j2+2,j3,ey))/(2.*dr2(1)**
     & 3))/(2.*dr2(0)**3)
       vv2rrssss = ((u2(j1-1,j2-2,j3,ey)-4.*u2(j1-1,j2-1,j3,ey)+6.*u2(
     & j1-1,j2,j3,ey)-4.*u2(j1-1,j2+1,j3,ey)+u2(j1-1,j2+2,j3,ey))/(
     & dr2(1)**4)-2.*(u2(j1,j2-2,j3,ey)-4.*u2(j1,j2-1,j3,ey)+6.*u2(j1,
     & j2,j3,ey)-4.*u2(j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(dr2(1)**4)+(
     & u2(j1+1,j2-2,j3,ey)-4.*u2(j1+1,j2-1,j3,ey)+6.*u2(j1+1,j2,j3,ey)
     & -4.*u2(j1+1,j2+1,j3,ey)+u2(j1+1,j2+2,j3,ey))/(dr2(1)**4))/(dr2(
     & 0)**2)
       vv2rsssss = (-(-u2(j1-1,j2-3,j3,ey)+4.*u2(j1-1,j2-2,j3,ey)-5.*
     & u2(j1-1,j2-1,j3,ey)+5.*u2(j1-1,j2+1,j3,ey)-4.*u2(j1-1,j2+2,j3,
     & ey)+u2(j1-1,j2+3,j3,ey))/(2.*dr2(1)**5)+(-u2(j1+1,j2-3,j3,ey)+
     & 4.*u2(j1+1,j2-2,j3,ey)-5.*u2(j1+1,j2-1,j3,ey)+5.*u2(j1+1,j2+1,
     & j3,ey)-4.*u2(j1+1,j2+2,j3,ey)+u2(j1+1,j2+3,j3,ey))/(2.*dr2(1)**
     & 5))/(2.*dr2(0))
       vv2ssssss = (u2(j1,j2-3,j3,ey)-6.*u2(j1,j2-2,j3,ey)+15.*u2(j1,
     & j2-1,j3,ey)-20.*u2(j1,j2,j3,ey)+15.*u2(j1,j2+1,j3,ey)-6.*u2(j1,
     & j2+2,j3,ey)+u2(j1,j2+3,j3,ey))/(dr2(1)**6)
       ! 5th derivatives, 2nd order
       t1 = a1j2rx**2
       t2 = t1**2
       t8 = a1j2sx**2
       t9 = t1*a1j2rx
       t13 = t8*a1j2sx
       t17 = t8**2
       t29 = a1j2sxx*t1
       t30 = a1j2sx*a1j2rxx
       t31 = a1j2sxx*a1j2rx
       t32 = t30+t31
       t33 = 2*t32
       t34 = a1j2rx*t33
       t36 = a1j2sx*a1j2rx*a1j2rxx
       t38 = t29+t34+4*t36
       t42 = 3*t32
       t44 = 7*t36+t29+a1j2rx*t42+t34
       t51 = a1j2sx*t33
       t52 = t31*a1j2sx
       t54 = a1j2rxx*t8
       t55 = t51+4*t52+t54
       t59 = a1j2sx*t42+7*t52+t54+t51
       t77 = a1j2rx*a1j2rxxx
       t79 = a1j2rxx**2
       t81 = 4*t77+3*t79
       t85 = t77+t79
       t86 = 2*t85
       t88 = 3*t85
       t94 = a1j2sx*a1j2rxxx
       t99 = a1j2sxxx*a1j2rx
       t101 = a1j2sxx*a1j2rxx
       t103 = 2*t94+2*t99+4*t101
       t107 = 6*t101
       t108 = 4*t99+4*t94+t107
       t117 = t107+3*t94+3*t99
       t121 = a1j2sxxx*t1+2*t94*a1j2rx+a1j2sx*t88+a1j2rx*t103+a1j2rx*
     & t108+a1j2sx*t81+7*t31*a1j2rxx+a1j2rxx*t42+a1j2sx*t86+a1j2rx*
     & t117+2*a1j2rxx*t33
       t125 = a1j2sxxx*a1j2sx
       t127 = a1j2sxx**2
       t129 = 4*t125+3*t127
       t132 = t125+t127
       t133 = 3*t132
       t136 = 2*t132
       t145 = 7*t30*a1j2sxx+a1j2rx*t129+a1j2sx*t108+a1j2rx*t133+
     & a1j2rxxx*t8+a1j2rx*t136+2*a1j2sxx*t33+a1j2sx*t103+a1j2sxx*t42+
     & a1j2sx*t117+2*t125*a1j2rx
       uu1xxxxx2 = t2*a1j2rx*uu1rrrrr+5*a1j2sx*t2*uu1rrrrs+10*t8*t9*
     & uu1rrrss+10*t13*t1*uu1rrsss+5*t17*a1j2rx*uu1rssss+t17*a1j2sx*
     & uu1sssss+10*a1j2rxx*t9*uu1rrrr+(12*a1j2rxx*t1*a1j2sx+a1j2rx*
     & t38+a1j2sxx*t9+a1j2rx*t44)*uu1rrrs+(3*a1j2rxx*a1j2rx*t8+a1j2rx*
     & t55+a1j2rx*t59+a1j2sx*t44+3*t29*a1j2sx+a1j2sx*t38)*uu1rrss+(
     & a1j2rxx*t13+12*t31*t8+a1j2sx*t55+a1j2sx*t59)*uu1rsss+10*
     & a1j2sxx*t13*uu1ssss+(a1j2rx*t81+7*t79*a1j2rx+a1j2rx*t86+a1j2rx*
     & t88+a1j2rxxx*t1)*uu1rrr+t121*uu1rrs+t145*uu1rss+(a1j2sx*t129+7*
     & t127*a1j2sx+a1j2sx*t136+a1j2sx*t133+a1j2sxxx*t8)*uu1sss+(10*
     & a1j2rxx*a1j2rxxx+5*a1j2rx*a1j2rxxxx)*uu1rr+(5*a1j2sxxxx*a1j2rx+
     & 10*a1j2sxx*a1j2rxxx+5*a1j2sx*a1j2rxxxx+10*a1j2sxxx*a1j2rxx)*
     & uu1rs+(5*a1j2sxxxx*a1j2sx+10*a1j2sxx*a1j2sxxx)*uu1ss
       t1 = a1j2rx**2
       t2 = t1**2
       t5 = t1*a1j2rx
       t15 = a1j2ry*t1
       t16 = a1j2sx**2
       t22 = t16*a1j2sx
       t25 = a1j2sy*t1
       t30 = t16**2
       t47 = a1j2sx*a1j2rxy
       t50 = a1j2sxy*t1
       t51 = a1j2sxy*a1j2rx
       t53 = 2*t47+2*t51
       t55 = 4*t47*a1j2rx+t50+a1j2rx*t53
       t64 = a1j2sxx*a1j2rx
       t66 = t64+a1j2sx*a1j2rxx
       t67 = 3*t66
       t69 = 2*t66
       t71 = 7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t1+a1j2rx*t67+a1j2rx*t69
       t81 = 4*t51*a1j2sx+a1j2rxy*t16+a1j2sx*t53
       t89 = a1j2sx*t67+7*t64*a1j2sx+a1j2rxx*t16+a1j2sx*t69
       t115 = a1j2rxy*a1j2rxx
       t119 = a1j2rx*a1j2rxxy+t115
       t120 = 3*t119
       t122 = 2*t119
       t126 = a1j2rxx**2
       t128 = 4*a1j2rx*a1j2rxxx+3*t126
       t133 = a1j2sxy*a1j2rxx
       t137 = a1j2sx*a1j2rxxy
       t140 = a1j2sxx*a1j2rxy
       t142 = t137+t140+a1j2sxxy*a1j2rx+t133
       t143 = 3*t142
       t148 = 2*t142
       t160 = 4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+6*a1j2sxx*a1j2rxx
       t162 = a1j2sxxy*t1+5*a1j2rx*t133+a1j2sy*t128+2*t137*a1j2rx+
     & a1j2rx*t143+a1j2rxy*t69+a1j2rxx*t53+a1j2sx*t120+a1j2rx*t148+2*
     & t140*a1j2rx+a1j2rxy*t67+a1j2sx*t122+a1j2ry*t160
       t169 = a1j2sxxy*a1j2sx
       t173 = a1j2sxy*a1j2sxx+t169
       t174 = 3*t173
       t179 = 2*t173
       t183 = a1j2sxx**2
       t185 = 4*a1j2sxxx*a1j2sx+3*t183
       t190 = a1j2sxx*t53+5*t47*a1j2sxx+a1j2sxy*t67+a1j2sx*t143+2*t169*
     & a1j2rx+a1j2rx*t174+a1j2sxy*t69+a1j2sy*t160+a1j2sx*t148+a1j2rx*
     & t179+a1j2ry*t185+2*a1j2sx*t133+a1j2rxxy*t16
       uu1xxxxy2 = a1j2ry*t2*uu1rrrrr+(4*a1j2ry*t5*a1j2sx+a1j2sy*t2)*
     & uu1rrrrs+(4*a1j2sy*t5*a1j2sx+6*t15*t16)*uu1rrrss+(4*a1j2ry*
     & a1j2rx*t22+6*t25*t16)*uu1rrsss+(a1j2ry*t30+4*a1j2sy*a1j2rx*t22)
     & *uu1rssss+a1j2sy*t30*uu1sssss+(4*t5*a1j2rxy+6*t15*a1j2rxx)*
     & uu1rrrr+(6*t25*a1j2rxx+a1j2rx*t55+6*t47*t1+a1j2sxy*t5+a1j2ry*
     & t71)*uu1rrrs+(3*t50*a1j2sx+a1j2rx*t81+a1j2sy*t71+a1j2ry*t89+3*
     & a1j2rxy*a1j2rx*t16+a1j2sx*t55)*uu1rrss+(a1j2sy*t89+6*t51*t16+
     & a1j2sx*t81+a1j2rxy*t22+6*a1j2ry*t16*a1j2sxx)*uu1rsss+(4*t22*
     & a1j2sxy+6*a1j2sy*t16*a1j2sxx)*uu1ssss+(a1j2rxxy*t1+7*t115*
     & a1j2rx+a1j2rx*t120+a1j2rx*t122+a1j2ry*t128)*uu1rrr+t162*uu1rrs+
     & t190*uu1rss+(a1j2sx*t174+a1j2sxxy*t16+7*a1j2sxy*a1j2sx*a1j2sxx+
     & a1j2sx*t179+a1j2sy*t185)*uu1sss+(a1j2ry*a1j2rxxxx+4*a1j2rxxx*
     & a1j2rxy+6*a1j2rxxy*a1j2rxx+4*a1j2rxxxy*a1j2rx)*uu1rr+(4*
     & a1j2sxxx*a1j2rxy+a1j2ry*a1j2sxxxx+4*a1j2sxxxy*a1j2rx+4*a1j2sxy*
     & a1j2rxxx+4*a1j2sx*a1j2rxxxy+6*a1j2sxx*a1j2rxxy+a1j2sy*
     & a1j2rxxxx+6*a1j2sxxy*a1j2rxx)*uu1rs+(4*a1j2sxxx*a1j2sxy+4*
     & a1j2sxxxy*a1j2sx+6*a1j2sxx*a1j2sxxy+a1j2sy*a1j2sxxxx)*uu1ss
       t1 = a1j2ry**2
       t2 = a1j2rx**2
       t3 = t2*a1j2rx
       t6 = a1j2sy*a1j2ry
       t12 = 3*a1j2ry*t2*a1j2sx+a1j2sy*t3
       t16 = a1j2ry*a1j2rx
       t17 = a1j2sx**2
       t22 = 3*t16*t17+3*a1j2sy*t2*a1j2sx
       t27 = a1j2sy*a1j2rx
       t30 = t17*a1j2sx
       t32 = 3*t27*t17+a1j2ry*t30
       t41 = a1j2sy**2
       t47 = 3*a1j2rxy*t2+3*a1j2rxx*t16
       t55 = a1j2sx*a1j2rxy
       t57 = 4*a1j2rx*t55
       t58 = a1j2sxy*t2
       t59 = a1j2sxy*a1j2rx
       t61 = 2*t55+2*t59
       t62 = a1j2rx*t61
       t63 = t57+t58+t62
       t71 = 3*a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx
       t75 = t57+t58+t62+a1j2ry*t71+3*t27*a1j2rxx
       t86 = 4*a1j2sx*t59
       t87 = a1j2rxy*t17
       t88 = a1j2sx*t61
       t89 = t86+t87+t88
       t95 = t87+a1j2sy*t71+3*a1j2ry*a1j2sxx*a1j2sx+t86+t88
       t101 = a1j2ryy*a1j2rx
       t106 = a1j2syy*a1j2rx
       t118 = 3*a1j2sy*a1j2sxx*a1j2sx+3*t17*a1j2sxy
       t130 = a1j2rx*a1j2rxxy
       t131 = a1j2rxy*a1j2rxx
       t133 = 3*t130+3*t131
       t139 = 3*t131+3*t130+a1j2ry*a1j2rxxx
       t141 = a1j2rxy**2
       t146 = 2*a1j2rx*a1j2rxyy+2*t141
       t152 = a1j2sxy*a1j2rxy
       t155 = a1j2sxx*a1j2rxy
       t158 = a1j2sxy*a1j2rxx
       t160 = a1j2sx*a1j2rxxy
       t162 = a1j2sxxy*a1j2rx
       t165 = 3*t155+a1j2ry*a1j2sxxx+3*t158+3*t160+3*t162+a1j2sy*
     & a1j2rxxx
       t170 = a1j2sx*a1j2rxyy
       t175 = 2*t170+2*a1j2sxyy*a1j2rx+4*t152
       t184 = 3*t160+3*t155+3*t162+3*t158
       t188 = 4*t152*a1j2rx+a1j2ry*t165+a1j2ryy*t71+2*a1j2rxy*t61+
     & a1j2rx*t175+a1j2sx*t146+2*t170*a1j2rx+a1j2sy*t139+a1j2sy*t133+
     & a1j2sxyy*t2+a1j2ry*t184+3*t106*a1j2rxx
       t195 = a1j2sxy*a1j2sxx
       t196 = a1j2sxxy*a1j2sx
       t198 = 3*t195+3*t196
       t206 = 3*t196+a1j2sy*a1j2sxxx+3*t195
       t208 = a1j2sx*a1j2sxyy
       t209 = a1j2sxy**2
       t211 = 2*t208+2*t209
       t219 = a1j2sy*t184+a1j2sx*t175+a1j2rxyy*t17+2*a1j2sxy*t61+
     & a1j2ry*t198+3*a1j2ryy*a1j2sxx*a1j2sx+a1j2ry*t206+a1j2rx*t211+2*
     & t208*a1j2rx+a1j2sy*t165+4*t152*a1j2sx+a1j2syy*t71
       uu1xxxyy2 = t1*t3*uu1rrrrr+(t6*t3+a1j2ry*t12)*uu1rrrrs+(a1j2ry*
     & t22+a1j2sy*t12)*uu1rrrss+(a1j2ry*t32+a1j2sy*t22)*uu1rrsss+(t6*
     & t30+a1j2sy*t32)*uu1rssss+t41*t30*uu1sssss+(a1j2ry*t47+3*a1j2ry*
     & a1j2rxy*t2+a1j2ryy*t3)*uu1rrrr+(a1j2ry*t63+3*a1j2sy*a1j2rxy*t2+
     & a1j2ry*t75+a1j2syy*t3+a1j2sy*t47+3*a1j2ryy*t2*a1j2sx)*uu1rrrs+(
     & a1j2sy*t75+a1j2ry*t89+a1j2ry*t95+a1j2sy*t63+3*a1j2syy*t2*
     & a1j2sx+3*t101*t17)*uu1rrss+(3*t106*t17+a1j2sy*t89+3*a1j2ry*t17*
     & a1j2sxy+a1j2sy*t95+a1j2ry*t118+a1j2ryy*t30)*uu1rsss+(3*a1j2sy*
     & t17*a1j2sxy+a1j2syy*t30+a1j2sy*t118)*uu1ssss+(a1j2ry*t133+
     & a1j2rxyy*t2+a1j2ry*t139+4*t141*a1j2rx+a1j2rx*t146+3*t101*
     & a1j2rxx)*uu1rrr+t188*uu1rrs+t219*uu1rss+(4*t209*a1j2sx+a1j2sy*
     & t206+a1j2sx*t211+3*a1j2syy*a1j2sxx*a1j2sx+a1j2sy*t198+a1j2sxyy*
     & t17)*uu1sss+(3*a1j2rx*a1j2rxxyy+6*a1j2rxy*a1j2rxxy+3*a1j2rxx*
     & a1j2rxyy+a1j2ryy*a1j2rxxx+2*a1j2ry*a1j2rxxxy)*uu1rr+(3*
     & a1j2sxxyy*a1j2rx+6*a1j2sxxy*a1j2rxy+3*a1j2sxyy*a1j2rxx+3*
     & a1j2sx*a1j2rxxyy+3*a1j2sxx*a1j2rxyy+2*a1j2ry*a1j2sxxxy+6*
     & a1j2sxy*a1j2rxxy+a1j2ryy*a1j2sxxx+2*a1j2sy*a1j2rxxxy+a1j2syy*
     & a1j2rxxx)*uu1rs+(3*a1j2sxxyy*a1j2sx+3*a1j2sxyy*a1j2sxx+2*
     & a1j2sy*a1j2sxxxy+a1j2syy*a1j2sxxx+6*a1j2sxy*a1j2sxxy)*uu1ss
       t1 = a1j2ry**2
       t3 = a1j2rx**2
       t6 = a1j2sy*a1j2ry
       t12 = a1j2sy*t3+2*a1j2ry*a1j2sx*a1j2rx
       t14 = t6*t3+a1j2ry*t12
       t21 = a1j2sx**2
       t26 = a1j2ry*t21+2*a1j2sy*a1j2sx*a1j2rx
       t28 = a1j2sy*t12+a1j2ry*t26
       t36 = a1j2sy*t26+t6*t21
       t41 = a1j2sy**2
       t51 = 2*a1j2ry*a1j2rxy*a1j2rx
       t55 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
       t57 = a1j2ryy*t3
       t58 = t51+a1j2ry*t55+t57
       t62 = t51+t57
       t70 = 2*a1j2sy*a1j2rxy*a1j2rx
       t73 = 2*a1j2ryy*a1j2sx*a1j2rx
       t74 = a1j2syy*t3
       t76 = a1j2sx*a1j2rxy
       t78 = a1j2sxy*a1j2rx
       t81 = a1j2ry*a1j2sxx+2*t76+2*t78+a1j2sy*a1j2rxx
       t84 = 2*t76+2*t78
       t85 = a1j2ry*t84
       t86 = a1j2sy*t55+t70+t73+t74+a1j2ry*t81+t85
       t88 = t74+t85+t73+t70
       t96 = a1j2sy*t84
       t99 = 2*a1j2syy*a1j2sx*a1j2rx
       t102 = 2*a1j2ry*a1j2sxy*a1j2sx
       t103 = a1j2ryy*t21
       t104 = t96+t99+t102+t103
       t111 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
       t113 = a1j2sy*t81+a1j2ry*t111+t103+t102+t99+t96
       t120 = a1j2syy*t21
       t123 = 2*a1j2sy*a1j2sxy*a1j2sx
       t124 = t120+t123
       t132 = t123+a1j2sy*t111+t120
       t145 = a1j2ryy*a1j2rxx
       t146 = a1j2rx*a1j2rxyy
       t147 = 2*t146
       t148 = a1j2ry*a1j2rxxy
       t149 = a1j2rxy**2
       t150 = 2*t149
       t151 = t145+t147+t148+t150
       t155 = t147+t145+2*t148+t150
       t159 = 2*t146+2*t149
       t172 = 4*a1j2sxy*a1j2rxy
       t173 = a1j2ry*a1j2sxxy
       t175 = a1j2ryy*a1j2sxx
       t176 = a1j2sy*a1j2rxxy
       t179 = 2*a1j2sxyy*a1j2rx
       t180 = a1j2syy*a1j2rxx
       t182 = 2*a1j2sx*a1j2rxyy
       t183 = t172+2*t173+t175+2*t176+t179+t180+t182
       t191 = t176+t173+t175+t172+t179+t180+t182
       t193 = t182+t179+t172
       t195 = a1j2sy*t155+2*a1j2ryyy*a1j2sx*a1j2rx+a1j2syy*t55+2*
     & a1j2ryy*t84+a1j2syyy*t3+a1j2ry*t183+4*a1j2syy*a1j2rxy*a1j2rx+
     & a1j2sy*t151+a1j2ryy*t81+a1j2sy*t159+a1j2ry*t191+a1j2ry*t193
       t198 = a1j2sxy**2
       t199 = 2*t198
       t200 = a1j2sy*a1j2sxxy
       t202 = a1j2syy*a1j2sxx
       t203 = a1j2sx*a1j2sxyy
       t204 = 2*t203
       t205 = t199+2*t200+t202+t204
       t214 = t200+t204+t202+t199
       t223 = 2*t203+2*t198
       t225 = a1j2syy*t81+a1j2ry*t205+a1j2sy*t183+a1j2ryyy*t21+a1j2sy*
     & t191+4*a1j2ryy*a1j2sxy*a1j2sx+a1j2sy*t193+a1j2ry*t214+2*
     & a1j2syyy*a1j2sx*a1j2rx+2*a1j2syy*t84+a1j2ryy*t111+a1j2ry*t223
       uu1xxyyy2 = t1*a1j2ry*t3*uu1rrrrr+(a1j2ry*t14+a1j2sy*t1*t3)*
     & uu1rrrrs+(a1j2ry*t28+a1j2sy*t14)*uu1rrrss+(a1j2sy*t28+a1j2ry*
     & t36)*uu1rrsss+(a1j2sy*t36+a1j2ry*t41*t21)*uu1rssss+t41*a1j2sy*
     & t21*uu1sssss+(a1j2ry*t58+a1j2ryy*a1j2ry*t3+a1j2ry*t62)*uu1rrrr+
     & (a1j2sy*t58+a1j2ry*t86+a1j2ry*t88+a1j2syy*a1j2ry*t3+a1j2ryy*
     & t12+a1j2sy*t62)*uu1rrrs+(a1j2ry*t104+a1j2syy*t12+a1j2ry*t113+
     & a1j2sy*t88+a1j2sy*t86+a1j2ryy*t26)*uu1rrss+(a1j2ry*t124+a1j2sy*
     & t104+a1j2syy*t26+a1j2ryy*a1j2sy*t21+a1j2sy*t113+a1j2ry*t132)*
     & uu1rsss+(a1j2sy*t132+a1j2syy*a1j2sy*t21+a1j2sy*t124)*uu1ssss+(
     & 4*a1j2ryy*a1j2rxy*a1j2rx+a1j2ry*t151+a1j2ryyy*t3+a1j2ry*t155+
     & a1j2ryy*t55+a1j2ry*t159)*uu1rrr+t195*uu1rrs+t225*uu1rss+(
     & a1j2sy*t214+a1j2syyy*t21+4*a1j2syy*a1j2sxy*a1j2sx+a1j2syy*t111+
     & a1j2sy*t223+a1j2sy*t205)*uu1sss+(a1j2ryyy*a1j2rxx+3*a1j2ry*
     & a1j2rxxyy+6*a1j2rxy*a1j2rxyy+2*a1j2rx*a1j2rxyyy+3*a1j2ryy*
     & a1j2rxxy)*uu1rr+(a1j2ryyy*a1j2sxx+3*a1j2ry*a1j2sxxyy+3*a1j2syy*
     & a1j2rxxy+6*a1j2sxyy*a1j2rxy+a1j2syyy*a1j2rxx+2*a1j2sx*
     & a1j2rxyyy+3*a1j2sy*a1j2rxxyy+3*a1j2ryy*a1j2sxxy+6*a1j2sxy*
     & a1j2rxyy+2*a1j2sxyyy*a1j2rx)*uu1rs+(3*a1j2syy*a1j2sxxy+2*
     & a1j2sx*a1j2sxyyy+a1j2syyy*a1j2sxx+3*a1j2sy*a1j2sxxyy+6*
     & a1j2sxyy*a1j2sxy)*uu1ss
       t1 = a1j2ry**2
       t2 = t1**2
       t10 = a1j2sy*a1j2ry
       t14 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
       t16 = t10*a1j2rx+a1j2ry*t14
       t18 = a1j2sy*t1*a1j2rx+a1j2ry*t16
       t25 = t10*a1j2sx+a1j2sy*t14
       t27 = a1j2sy*t16+a1j2ry*t25
       t33 = a1j2sy**2
       t36 = a1j2sy*t25+a1j2ry*t33*a1j2sx
       t47 = t33**2
       t52 = a1j2ryy*a1j2rx
       t53 = a1j2ry*a1j2rxy
       t54 = t52+t53
       t55 = a1j2ry*t54
       t57 = a1j2ryy*a1j2ry*a1j2rx
       t58 = t55+t57
       t61 = t52+2*t53
       t63 = t55+a1j2ry*t61+t57
       t68 = a1j2syy*a1j2ry*a1j2rx
       t69 = a1j2sy*a1j2rxy
       t70 = a1j2ryy*a1j2sx
       t71 = a1j2syy*a1j2rx
       t72 = a1j2ry*a1j2sxy
       t73 = t69+t70+t71+t72
       t74 = a1j2ry*t73
       t75 = a1j2ryy*t14
       t76 = a1j2sy*t54
       t77 = t68+t74+t75+t76
       t85 = 2*t72+2*t69+t70+t71
       t87 = a1j2sy*t61+t76+t75+t68+a1j2ry*t85+t74
       t95 = a1j2syy*a1j2sx
       t96 = a1j2sy*a1j2sxy
       t98 = t95+2*t96
       t101 = a1j2ryy*a1j2sy*a1j2sx
       t102 = t96+t95
       t103 = a1j2ry*t102
       t104 = a1j2syy*t14
       t105 = a1j2sy*t73
       t106 = a1j2sy*t85+a1j2ry*t98+t101+t103+t104+t105
       t108 = t105+t104+t103+t101
       t117 = a1j2sy*t102
       t119 = a1j2syy*a1j2sy*a1j2sx
       t120 = t117+t119
       t123 = t117+a1j2sy*t98+t119
       t136 = a1j2ryyy*a1j2rx
       t137 = a1j2ry*a1j2rxyy
       t139 = a1j2ryy*a1j2rxy
       t140 = 3*t139
       t141 = t136+2*t137+t140
       t146 = t136+2*t139+t137
       t151 = 3*t137+t136+t140
       t159 = a1j2ryyy*a1j2sx
       t160 = a1j2ry*a1j2sxyy
       t161 = a1j2syyy*a1j2rx
       t162 = a1j2syy*a1j2rxy
       t164 = a1j2sy*a1j2rxyy
       t165 = a1j2ryy*a1j2sxy
       t167 = t159+t160+t161+2*t162+t164+2*t165
       t174 = 3*t165
       t177 = 3*t162
       t178 = t174+2*t164+t159+2*t160+t161+t177
       t184 = 3*t160+3*t164+t161+t177+t159+t174
       t188 = a1j2syyy*a1j2ry*a1j2rx+a1j2ryyy*t14+a1j2ry*t167+2*
     & a1j2ryy*t73+2*a1j2syy*t54+a1j2ryy*t85+a1j2ry*t178+a1j2syy*t61+
     & a1j2sy*t151+a1j2ry*t184+a1j2sy*t146+a1j2sy*t141
       t195 = a1j2syyy*a1j2sx
       t196 = a1j2sy*a1j2sxyy
       t198 = a1j2syy*a1j2sxy
       t199 = 3*t198
       t200 = t195+2*t196+t199
       t203 = t195+3*t196+t199
       t208 = t195+2*t198+t196
       t215 = 2*a1j2syy*t73+a1j2sy*t178+a1j2ryyy*a1j2sy*a1j2sx+a1j2ry*
     & t200+a1j2ry*t203+a1j2syy*t85+a1j2syyy*t14+a1j2ry*t208+2*
     & a1j2ryy*t102+a1j2sy*t184+a1j2sy*t167+a1j2ryy*t98
       uu1xyyyy2 = t2*a1j2rx*uu1rrrrr+(a1j2sy*t1*a1j2ry*a1j2rx+a1j2ry*
     & t18)*uu1rrrrs+(a1j2ry*t27+a1j2sy*t18)*uu1rrrss+(a1j2ry*t36+
     & a1j2sy*t27)*uu1rrsss+(a1j2sy*t36+a1j2ry*t33*a1j2sy*a1j2sx)*
     & uu1rssss+t47*a1j2sx*uu1sssss+(a1j2ryy*t1*a1j2rx+a1j2ry*t58+
     & a1j2ry*t63)*uu1rrrr+(a1j2ry*t77+a1j2sy*t58+a1j2syy*t1*a1j2rx+
     & a1j2ry*t87+a1j2sy*t63+a1j2ryy*t16)*uu1rrrs+(a1j2syy*t16+a1j2ry*
     & t106+a1j2ry*t108+a1j2sy*t77+a1j2sy*t87+a1j2ryy*t25)*uu1rrss+(
     & a1j2syy*t25+a1j2sy*t108+a1j2ry*t120+a1j2ry*t123+a1j2sy*t106+
     & a1j2ryy*t33*a1j2sx)*uu1rsss+(a1j2syy*t33*a1j2sx+a1j2sy*t120+
     & a1j2sy*t123)*uu1ssss+(a1j2ry*t141+2*a1j2ryy*t54+a1j2ry*t146+
     & a1j2ryyy*a1j2ry*a1j2rx+a1j2ry*t151+a1j2ryy*t61)*uu1rrr+t188*
     & uu1rrs+t215*uu1rss+(a1j2syyy*a1j2sy*a1j2sx+2*a1j2syy*t102+
     & a1j2sy*t200+a1j2sy*t203+a1j2syy*t98+a1j2sy*t208)*uu1sss+(
     & a1j2ryyyy*a1j2rx+4*a1j2ry*a1j2rxyyy+4*a1j2ryyy*a1j2rxy+6*
     & a1j2ryy*a1j2rxyy)*uu1rr+(4*a1j2ryyy*a1j2sxy+a1j2syyyy*a1j2rx+4*
     & a1j2sy*a1j2rxyyy+6*a1j2syy*a1j2rxyy+4*a1j2ry*a1j2sxyyy+4*
     & a1j2syyy*a1j2rxy+6*a1j2ryy*a1j2sxyy+a1j2ryyyy*a1j2sx)*uu1rs+(4*
     & a1j2sy*a1j2sxyyy+a1j2syyyy*a1j2sx+4*a1j2syyy*a1j2sxy+6*a1j2syy*
     & a1j2sxyy)*uu1ss
       t1 = a1j2ry**2
       t2 = t1**2
       t8 = a1j2sy**2
       t9 = t1*a1j2ry
       t13 = t8*a1j2sy
       t17 = t8**2
       t26 = a1j2sy*a1j2ryy
       t29 = a1j2syy*t1
       t30 = a1j2syy*a1j2ry
       t31 = t26+t30
       t32 = 2*t31
       t33 = a1j2ry*t32
       t35 = a1j2sy*a1j2ry*a1j2ryy
       t37 = t29+t33+4*t35
       t41 = 3*t31
       t43 = 7*t35+t29+a1j2ry*t41+t33
       t52 = a1j2sy*t32
       t53 = t30*a1j2sy
       t55 = a1j2ryy*t8
       t56 = t52+4*t53+t55
       t60 = a1j2sy*t41+7*t53+t55+t52
       t78 = a1j2ry*a1j2ryyy
       t79 = a1j2ryy**2
       t80 = t78+t79
       t81 = 3*t80
       t87 = 4*t78+3*t79
       t89 = 2*t80
       t96 = a1j2sy*a1j2ryyy
       t98 = a1j2syyy*a1j2ry
       t100 = a1j2syy*a1j2ryy
       t102 = 2*t96+2*t98+4*t100
       t106 = 6*t100
       t109 = t106+3*t96+3*t98
       t117 = 4*t98+4*t96+t106
       t121 = 7*t30*a1j2ryy+a1j2sy*t89+a1j2ry*t102+a1j2ryy*t41+
     & a1j2syyy*t1+a1j2ry*t109+a1j2sy*t87+a1j2sy*t81+2*t96*a1j2ry+
     & a1j2ry*t117+2*a1j2ryy*t32
       t129 = a1j2syyy*a1j2sy
       t131 = a1j2syy**2
       t133 = 4*t129+3*t131
       t135 = t129+t131
       t136 = 2*t135
       t138 = 3*t135
       t145 = 7*t100*a1j2sy+a1j2sy*t109+2*a1j2syy*t32+a1j2sy*t117+
     & a1j2ry*t133+a1j2ry*t136+a1j2ry*t138+a1j2syy*t41+a1j2ryyy*t8+2*
     & t129*a1j2ry+a1j2sy*t102
       uu1yyyyy2 = t2*a1j2ry*uu1rrrrr+5*a1j2sy*t2*uu1rrrrs+10*t8*t9*
     & uu1rrrss+10*t13*t1*uu1rrsss+5*t17*a1j2ry*uu1rssss+t17*a1j2sy*
     & uu1sssss+10*t9*a1j2ryy*uu1rrrr+(12*t26*t1+a1j2ry*t37+a1j2syy*
     & t9+a1j2ry*t43)*uu1rrrs+(3*a1j2ryy*a1j2ry*t8+a1j2sy*t43+a1j2sy*
     & t37+a1j2ry*t56+a1j2ry*t60+3*t29*a1j2sy)*uu1rrss+(12*a1j2ry*t8*
     & a1j2syy+a1j2sy*t56+a1j2sy*t60+a1j2ryy*t13)*uu1rsss+10*t13*
     & a1j2syy*uu1ssss+(a1j2ryyy*t1+a1j2ry*t81+7*t79*a1j2ry+a1j2ry*
     & t87+a1j2ry*t89)*uu1rrr+t121*uu1rrs+t145*uu1rss+(a1j2sy*t138+
     & a1j2sy*t133+7*t131*a1j2sy+a1j2syyy*t8+a1j2sy*t136)*uu1sss+(5*
     & a1j2ry*a1j2ryyyy+10*a1j2ryyy*a1j2ryy)*uu1rr+(10*a1j2syy*
     & a1j2ryyy+10*a1j2syyy*a1j2ryy+5*a1j2sy*a1j2ryyyy+5*a1j2syyyy*
     & a1j2ry)*uu1rs+(10*a1j2syy*a1j2syyy+5*a1j2sy*a1j2syyyy)*uu1ss
       t1 = a1j2rx**2
       t2 = t1**2
       t8 = a1j2sx**2
       t9 = t1*a1j2rx
       t13 = t8*a1j2sx
       t17 = t8**2
       t29 = a1j2sxx*t1
       t30 = a1j2sx*a1j2rxx
       t31 = a1j2sxx*a1j2rx
       t32 = t30+t31
       t33 = 2*t32
       t34 = a1j2rx*t33
       t36 = a1j2sx*a1j2rx*a1j2rxx
       t38 = t29+t34+4*t36
       t42 = 3*t32
       t44 = 7*t36+t29+a1j2rx*t42+t34
       t51 = a1j2sx*t33
       t52 = t31*a1j2sx
       t54 = a1j2rxx*t8
       t55 = t51+4*t52+t54
       t59 = a1j2sx*t42+7*t52+t54+t51
       t77 = a1j2rx*a1j2rxxx
       t79 = a1j2rxx**2
       t81 = 4*t77+3*t79
       t85 = t77+t79
       t86 = 2*t85
       t88 = 3*t85
       t94 = a1j2sx*a1j2rxxx
       t99 = a1j2sxxx*a1j2rx
       t101 = a1j2sxx*a1j2rxx
       t103 = 2*t94+2*t99+4*t101
       t107 = 6*t101
       t108 = 4*t99+4*t94+t107
       t117 = t107+3*t94+3*t99
       t121 = a1j2sxxx*t1+2*t94*a1j2rx+a1j2sx*t88+a1j2rx*t103+a1j2rx*
     & t108+a1j2sx*t81+7*t31*a1j2rxx+a1j2rxx*t42+a1j2sx*t86+a1j2rx*
     & t117+2*a1j2rxx*t33
       t125 = a1j2sxxx*a1j2sx
       t127 = a1j2sxx**2
       t129 = 4*t125+3*t127
       t132 = t125+t127
       t133 = 3*t132
       t136 = 2*t132
       t145 = 7*t30*a1j2sxx+a1j2rx*t129+a1j2sx*t108+a1j2rx*t133+
     & a1j2rxxx*t8+a1j2rx*t136+2*a1j2sxx*t33+a1j2sx*t103+a1j2sxx*t42+
     & a1j2sx*t117+2*t125*a1j2rx
       vv1xxxxx2 = t2*a1j2rx*vv1rrrrr+5*a1j2sx*t2*vv1rrrrs+10*t8*t9*
     & vv1rrrss+10*t13*t1*vv1rrsss+5*t17*a1j2rx*vv1rssss+t17*a1j2sx*
     & vv1sssss+10*a1j2rxx*t9*vv1rrrr+(12*a1j2rxx*t1*a1j2sx+a1j2rx*
     & t38+a1j2sxx*t9+a1j2rx*t44)*vv1rrrs+(3*a1j2rxx*a1j2rx*t8+a1j2rx*
     & t55+a1j2rx*t59+a1j2sx*t44+3*t29*a1j2sx+a1j2sx*t38)*vv1rrss+(
     & a1j2rxx*t13+12*t31*t8+a1j2sx*t55+a1j2sx*t59)*vv1rsss+10*
     & a1j2sxx*t13*vv1ssss+(a1j2rx*t81+7*t79*a1j2rx+a1j2rx*t86+a1j2rx*
     & t88+a1j2rxxx*t1)*vv1rrr+t121*vv1rrs+t145*vv1rss+(a1j2sx*t129+7*
     & t127*a1j2sx+a1j2sx*t136+a1j2sx*t133+a1j2sxxx*t8)*vv1sss+(10*
     & a1j2rxx*a1j2rxxx+5*a1j2rx*a1j2rxxxx)*vv1rr+(5*a1j2sxxxx*a1j2rx+
     & 10*a1j2sxx*a1j2rxxx+5*a1j2sx*a1j2rxxxx+10*a1j2sxxx*a1j2rxx)*
     & vv1rs+(5*a1j2sxxxx*a1j2sx+10*a1j2sxx*a1j2sxxx)*vv1ss
       t1 = a1j2rx**2
       t2 = t1**2
       t5 = t1*a1j2rx
       t15 = a1j2ry*t1
       t16 = a1j2sx**2
       t22 = t16*a1j2sx
       t25 = a1j2sy*t1
       t30 = t16**2
       t47 = a1j2sx*a1j2rxy
       t50 = a1j2sxy*t1
       t51 = a1j2sxy*a1j2rx
       t53 = 2*t47+2*t51
       t55 = 4*t47*a1j2rx+t50+a1j2rx*t53
       t64 = a1j2sxx*a1j2rx
       t66 = t64+a1j2sx*a1j2rxx
       t67 = 3*t66
       t69 = 2*t66
       t71 = 7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t1+a1j2rx*t67+a1j2rx*t69
       t81 = 4*t51*a1j2sx+a1j2rxy*t16+a1j2sx*t53
       t89 = a1j2sx*t67+7*t64*a1j2sx+a1j2rxx*t16+a1j2sx*t69
       t115 = a1j2rxy*a1j2rxx
       t119 = a1j2rx*a1j2rxxy+t115
       t120 = 3*t119
       t122 = 2*t119
       t126 = a1j2rxx**2
       t128 = 4*a1j2rx*a1j2rxxx+3*t126
       t133 = a1j2sxy*a1j2rxx
       t137 = a1j2sx*a1j2rxxy
       t140 = a1j2sxx*a1j2rxy
       t142 = t137+t140+a1j2sxxy*a1j2rx+t133
       t143 = 3*t142
       t148 = 2*t142
       t160 = 4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+6*a1j2sxx*a1j2rxx
       t162 = a1j2sxxy*t1+5*a1j2rx*t133+a1j2sy*t128+2*t137*a1j2rx+
     & a1j2rx*t143+a1j2rxy*t69+a1j2rxx*t53+a1j2sx*t120+a1j2rx*t148+2*
     & t140*a1j2rx+a1j2rxy*t67+a1j2sx*t122+a1j2ry*t160
       t169 = a1j2sxxy*a1j2sx
       t173 = a1j2sxy*a1j2sxx+t169
       t174 = 3*t173
       t179 = 2*t173
       t183 = a1j2sxx**2
       t185 = 4*a1j2sxxx*a1j2sx+3*t183
       t190 = a1j2sxx*t53+5*t47*a1j2sxx+a1j2sxy*t67+a1j2sx*t143+2*t169*
     & a1j2rx+a1j2rx*t174+a1j2sxy*t69+a1j2sy*t160+a1j2sx*t148+a1j2rx*
     & t179+a1j2ry*t185+2*a1j2sx*t133+a1j2rxxy*t16
       vv1xxxxy2 = a1j2ry*t2*vv1rrrrr+(4*a1j2ry*t5*a1j2sx+a1j2sy*t2)*
     & vv1rrrrs+(4*a1j2sy*t5*a1j2sx+6*t15*t16)*vv1rrrss+(4*a1j2ry*
     & a1j2rx*t22+6*t25*t16)*vv1rrsss+(a1j2ry*t30+4*a1j2sy*a1j2rx*t22)
     & *vv1rssss+a1j2sy*t30*vv1sssss+(4*t5*a1j2rxy+6*t15*a1j2rxx)*
     & vv1rrrr+(6*t25*a1j2rxx+a1j2rx*t55+6*t47*t1+a1j2sxy*t5+a1j2ry*
     & t71)*vv1rrrs+(3*t50*a1j2sx+a1j2rx*t81+a1j2sy*t71+a1j2ry*t89+3*
     & a1j2rxy*a1j2rx*t16+a1j2sx*t55)*vv1rrss+(a1j2sy*t89+6*t51*t16+
     & a1j2sx*t81+a1j2rxy*t22+6*a1j2ry*t16*a1j2sxx)*vv1rsss+(4*t22*
     & a1j2sxy+6*a1j2sy*t16*a1j2sxx)*vv1ssss+(a1j2rxxy*t1+7*t115*
     & a1j2rx+a1j2rx*t120+a1j2rx*t122+a1j2ry*t128)*vv1rrr+t162*vv1rrs+
     & t190*vv1rss+(a1j2sx*t174+a1j2sxxy*t16+7*a1j2sxy*a1j2sx*a1j2sxx+
     & a1j2sx*t179+a1j2sy*t185)*vv1sss+(a1j2ry*a1j2rxxxx+4*a1j2rxxx*
     & a1j2rxy+6*a1j2rxxy*a1j2rxx+4*a1j2rxxxy*a1j2rx)*vv1rr+(4*
     & a1j2sxxx*a1j2rxy+a1j2ry*a1j2sxxxx+4*a1j2sxxxy*a1j2rx+4*a1j2sxy*
     & a1j2rxxx+4*a1j2sx*a1j2rxxxy+6*a1j2sxx*a1j2rxxy+a1j2sy*
     & a1j2rxxxx+6*a1j2sxxy*a1j2rxx)*vv1rs+(4*a1j2sxxx*a1j2sxy+4*
     & a1j2sxxxy*a1j2sx+6*a1j2sxx*a1j2sxxy+a1j2sy*a1j2sxxxx)*vv1ss
       t1 = a1j2ry**2
       t2 = a1j2rx**2
       t3 = t2*a1j2rx
       t6 = a1j2sy*a1j2ry
       t12 = 3*a1j2ry*t2*a1j2sx+a1j2sy*t3
       t16 = a1j2ry*a1j2rx
       t17 = a1j2sx**2
       t22 = 3*t16*t17+3*a1j2sy*t2*a1j2sx
       t27 = a1j2sy*a1j2rx
       t30 = t17*a1j2sx
       t32 = 3*t27*t17+a1j2ry*t30
       t41 = a1j2sy**2
       t47 = 3*a1j2rxy*t2+3*a1j2rxx*t16
       t55 = a1j2sx*a1j2rxy
       t57 = 4*a1j2rx*t55
       t58 = a1j2sxy*t2
       t59 = a1j2sxy*a1j2rx
       t61 = 2*t55+2*t59
       t62 = a1j2rx*t61
       t63 = t57+t58+t62
       t71 = 3*a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx
       t75 = t57+t58+t62+a1j2ry*t71+3*t27*a1j2rxx
       t86 = 4*a1j2sx*t59
       t87 = a1j2rxy*t17
       t88 = a1j2sx*t61
       t89 = t86+t87+t88
       t95 = t87+a1j2sy*t71+3*a1j2ry*a1j2sxx*a1j2sx+t86+t88
       t101 = a1j2ryy*a1j2rx
       t106 = a1j2syy*a1j2rx
       t118 = 3*a1j2sy*a1j2sxx*a1j2sx+3*t17*a1j2sxy
       t130 = a1j2rx*a1j2rxxy
       t131 = a1j2rxy*a1j2rxx
       t133 = 3*t130+3*t131
       t139 = 3*t131+3*t130+a1j2ry*a1j2rxxx
       t141 = a1j2rxy**2
       t146 = 2*a1j2rx*a1j2rxyy+2*t141
       t152 = a1j2sxy*a1j2rxy
       t155 = a1j2sxx*a1j2rxy
       t158 = a1j2sxy*a1j2rxx
       t160 = a1j2sx*a1j2rxxy
       t162 = a1j2sxxy*a1j2rx
       t165 = 3*t155+a1j2ry*a1j2sxxx+3*t158+3*t160+3*t162+a1j2sy*
     & a1j2rxxx
       t170 = a1j2sx*a1j2rxyy
       t175 = 2*t170+2*a1j2sxyy*a1j2rx+4*t152
       t184 = 3*t160+3*t155+3*t162+3*t158
       t188 = 4*t152*a1j2rx+a1j2ry*t165+a1j2ryy*t71+2*a1j2rxy*t61+
     & a1j2rx*t175+a1j2sx*t146+2*t170*a1j2rx+a1j2sy*t139+a1j2sy*t133+
     & a1j2sxyy*t2+a1j2ry*t184+3*t106*a1j2rxx
       t195 = a1j2sxy*a1j2sxx
       t196 = a1j2sxxy*a1j2sx
       t198 = 3*t195+3*t196
       t206 = 3*t196+a1j2sy*a1j2sxxx+3*t195
       t208 = a1j2sx*a1j2sxyy
       t209 = a1j2sxy**2
       t211 = 2*t208+2*t209
       t219 = a1j2sy*t184+a1j2sx*t175+a1j2rxyy*t17+2*a1j2sxy*t61+
     & a1j2ry*t198+3*a1j2ryy*a1j2sxx*a1j2sx+a1j2ry*t206+a1j2rx*t211+2*
     & t208*a1j2rx+a1j2sy*t165+4*t152*a1j2sx+a1j2syy*t71
       vv1xxxyy2 = t1*t3*vv1rrrrr+(t6*t3+a1j2ry*t12)*vv1rrrrs+(a1j2ry*
     & t22+a1j2sy*t12)*vv1rrrss+(a1j2ry*t32+a1j2sy*t22)*vv1rrsss+(t6*
     & t30+a1j2sy*t32)*vv1rssss+t41*t30*vv1sssss+(a1j2ry*t47+3*a1j2ry*
     & a1j2rxy*t2+a1j2ryy*t3)*vv1rrrr+(a1j2ry*t63+3*a1j2sy*a1j2rxy*t2+
     & a1j2ry*t75+a1j2syy*t3+a1j2sy*t47+3*a1j2ryy*t2*a1j2sx)*vv1rrrs+(
     & a1j2sy*t75+a1j2ry*t89+a1j2ry*t95+a1j2sy*t63+3*a1j2syy*t2*
     & a1j2sx+3*t101*t17)*vv1rrss+(3*t106*t17+a1j2sy*t89+3*a1j2ry*t17*
     & a1j2sxy+a1j2sy*t95+a1j2ry*t118+a1j2ryy*t30)*vv1rsss+(3*a1j2sy*
     & t17*a1j2sxy+a1j2syy*t30+a1j2sy*t118)*vv1ssss+(a1j2ry*t133+
     & a1j2rxyy*t2+a1j2ry*t139+4*t141*a1j2rx+a1j2rx*t146+3*t101*
     & a1j2rxx)*vv1rrr+t188*vv1rrs+t219*vv1rss+(4*t209*a1j2sx+a1j2sy*
     & t206+a1j2sx*t211+3*a1j2syy*a1j2sxx*a1j2sx+a1j2sy*t198+a1j2sxyy*
     & t17)*vv1sss+(3*a1j2rx*a1j2rxxyy+6*a1j2rxy*a1j2rxxy+3*a1j2rxx*
     & a1j2rxyy+a1j2ryy*a1j2rxxx+2*a1j2ry*a1j2rxxxy)*vv1rr+(3*
     & a1j2sxxyy*a1j2rx+6*a1j2sxxy*a1j2rxy+3*a1j2sxyy*a1j2rxx+3*
     & a1j2sx*a1j2rxxyy+3*a1j2sxx*a1j2rxyy+2*a1j2ry*a1j2sxxxy+6*
     & a1j2sxy*a1j2rxxy+a1j2ryy*a1j2sxxx+2*a1j2sy*a1j2rxxxy+a1j2syy*
     & a1j2rxxx)*vv1rs+(3*a1j2sxxyy*a1j2sx+3*a1j2sxyy*a1j2sxx+2*
     & a1j2sy*a1j2sxxxy+a1j2syy*a1j2sxxx+6*a1j2sxy*a1j2sxxy)*vv1ss
       t1 = a1j2ry**2
       t3 = a1j2rx**2
       t6 = a1j2sy*a1j2ry
       t12 = a1j2sy*t3+2*a1j2ry*a1j2sx*a1j2rx
       t14 = t6*t3+a1j2ry*t12
       t21 = a1j2sx**2
       t26 = a1j2ry*t21+2*a1j2sy*a1j2sx*a1j2rx
       t28 = a1j2sy*t12+a1j2ry*t26
       t36 = a1j2sy*t26+t6*t21
       t41 = a1j2sy**2
       t51 = 2*a1j2ry*a1j2rxy*a1j2rx
       t55 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
       t57 = a1j2ryy*t3
       t58 = t51+a1j2ry*t55+t57
       t62 = t51+t57
       t70 = 2*a1j2sy*a1j2rxy*a1j2rx
       t73 = 2*a1j2ryy*a1j2sx*a1j2rx
       t74 = a1j2syy*t3
       t76 = a1j2sx*a1j2rxy
       t78 = a1j2sxy*a1j2rx
       t81 = a1j2ry*a1j2sxx+2*t76+2*t78+a1j2sy*a1j2rxx
       t84 = 2*t76+2*t78
       t85 = a1j2ry*t84
       t86 = a1j2sy*t55+t70+t73+t74+a1j2ry*t81+t85
       t88 = t74+t85+t73+t70
       t96 = a1j2sy*t84
       t99 = 2*a1j2syy*a1j2sx*a1j2rx
       t102 = 2*a1j2ry*a1j2sxy*a1j2sx
       t103 = a1j2ryy*t21
       t104 = t96+t99+t102+t103
       t111 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
       t113 = a1j2sy*t81+a1j2ry*t111+t103+t102+t99+t96
       t120 = a1j2syy*t21
       t123 = 2*a1j2sy*a1j2sxy*a1j2sx
       t124 = t120+t123
       t132 = t123+a1j2sy*t111+t120
       t145 = a1j2ryy*a1j2rxx
       t146 = a1j2rx*a1j2rxyy
       t147 = 2*t146
       t148 = a1j2ry*a1j2rxxy
       t149 = a1j2rxy**2
       t150 = 2*t149
       t151 = t145+t147+t148+t150
       t155 = t147+t145+2*t148+t150
       t159 = 2*t146+2*t149
       t172 = 4*a1j2sxy*a1j2rxy
       t173 = a1j2ry*a1j2sxxy
       t175 = a1j2ryy*a1j2sxx
       t176 = a1j2sy*a1j2rxxy
       t179 = 2*a1j2sxyy*a1j2rx
       t180 = a1j2syy*a1j2rxx
       t182 = 2*a1j2sx*a1j2rxyy
       t183 = t172+2*t173+t175+2*t176+t179+t180+t182
       t191 = t176+t173+t175+t172+t179+t180+t182
       t193 = t182+t179+t172
       t195 = a1j2sy*t155+2*a1j2ryyy*a1j2sx*a1j2rx+a1j2syy*t55+2*
     & a1j2ryy*t84+a1j2syyy*t3+a1j2ry*t183+4*a1j2syy*a1j2rxy*a1j2rx+
     & a1j2sy*t151+a1j2ryy*t81+a1j2sy*t159+a1j2ry*t191+a1j2ry*t193
       t198 = a1j2sxy**2
       t199 = 2*t198
       t200 = a1j2sy*a1j2sxxy
       t202 = a1j2syy*a1j2sxx
       t203 = a1j2sx*a1j2sxyy
       t204 = 2*t203
       t205 = t199+2*t200+t202+t204
       t214 = t200+t204+t202+t199
       t223 = 2*t203+2*t198
       t225 = a1j2syy*t81+a1j2ry*t205+a1j2sy*t183+a1j2ryyy*t21+a1j2sy*
     & t191+4*a1j2ryy*a1j2sxy*a1j2sx+a1j2sy*t193+a1j2ry*t214+2*
     & a1j2syyy*a1j2sx*a1j2rx+2*a1j2syy*t84+a1j2ryy*t111+a1j2ry*t223
       vv1xxyyy2 = t1*a1j2ry*t3*vv1rrrrr+(a1j2ry*t14+a1j2sy*t1*t3)*
     & vv1rrrrs+(a1j2ry*t28+a1j2sy*t14)*vv1rrrss+(a1j2sy*t28+a1j2ry*
     & t36)*vv1rrsss+(a1j2sy*t36+a1j2ry*t41*t21)*vv1rssss+t41*a1j2sy*
     & t21*vv1sssss+(a1j2ry*t58+a1j2ryy*a1j2ry*t3+a1j2ry*t62)*vv1rrrr+
     & (a1j2sy*t58+a1j2ry*t86+a1j2ry*t88+a1j2syy*a1j2ry*t3+a1j2ryy*
     & t12+a1j2sy*t62)*vv1rrrs+(a1j2ry*t104+a1j2syy*t12+a1j2ry*t113+
     & a1j2sy*t88+a1j2sy*t86+a1j2ryy*t26)*vv1rrss+(a1j2ry*t124+a1j2sy*
     & t104+a1j2syy*t26+a1j2ryy*a1j2sy*t21+a1j2sy*t113+a1j2ry*t132)*
     & vv1rsss+(a1j2sy*t132+a1j2syy*a1j2sy*t21+a1j2sy*t124)*vv1ssss+(
     & 4*a1j2ryy*a1j2rxy*a1j2rx+a1j2ry*t151+a1j2ryyy*t3+a1j2ry*t155+
     & a1j2ryy*t55+a1j2ry*t159)*vv1rrr+t195*vv1rrs+t225*vv1rss+(
     & a1j2sy*t214+a1j2syyy*t21+4*a1j2syy*a1j2sxy*a1j2sx+a1j2syy*t111+
     & a1j2sy*t223+a1j2sy*t205)*vv1sss+(a1j2ryyy*a1j2rxx+3*a1j2ry*
     & a1j2rxxyy+6*a1j2rxy*a1j2rxyy+2*a1j2rx*a1j2rxyyy+3*a1j2ryy*
     & a1j2rxxy)*vv1rr+(a1j2ryyy*a1j2sxx+3*a1j2ry*a1j2sxxyy+3*a1j2syy*
     & a1j2rxxy+6*a1j2sxyy*a1j2rxy+a1j2syyy*a1j2rxx+2*a1j2sx*
     & a1j2rxyyy+3*a1j2sy*a1j2rxxyy+3*a1j2ryy*a1j2sxxy+6*a1j2sxy*
     & a1j2rxyy+2*a1j2sxyyy*a1j2rx)*vv1rs+(3*a1j2syy*a1j2sxxy+2*
     & a1j2sx*a1j2sxyyy+a1j2syyy*a1j2sxx+3*a1j2sy*a1j2sxxyy+6*
     & a1j2sxyy*a1j2sxy)*vv1ss
       t1 = a1j2ry**2
       t2 = t1**2
       t10 = a1j2sy*a1j2ry
       t14 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
       t16 = t10*a1j2rx+a1j2ry*t14
       t18 = a1j2sy*t1*a1j2rx+a1j2ry*t16
       t25 = t10*a1j2sx+a1j2sy*t14
       t27 = a1j2sy*t16+a1j2ry*t25
       t33 = a1j2sy**2
       t36 = a1j2sy*t25+a1j2ry*t33*a1j2sx
       t47 = t33**2
       t52 = a1j2ryy*a1j2rx
       t53 = a1j2ry*a1j2rxy
       t54 = t52+t53
       t55 = a1j2ry*t54
       t57 = a1j2ryy*a1j2ry*a1j2rx
       t58 = t55+t57
       t61 = t52+2*t53
       t63 = t55+a1j2ry*t61+t57
       t68 = a1j2syy*a1j2ry*a1j2rx
       t69 = a1j2sy*a1j2rxy
       t70 = a1j2ryy*a1j2sx
       t71 = a1j2syy*a1j2rx
       t72 = a1j2ry*a1j2sxy
       t73 = t69+t70+t71+t72
       t74 = a1j2ry*t73
       t75 = a1j2ryy*t14
       t76 = a1j2sy*t54
       t77 = t68+t74+t75+t76
       t85 = 2*t72+2*t69+t70+t71
       t87 = a1j2sy*t61+t76+t75+t68+a1j2ry*t85+t74
       t95 = a1j2syy*a1j2sx
       t96 = a1j2sy*a1j2sxy
       t98 = t95+2*t96
       t101 = a1j2ryy*a1j2sy*a1j2sx
       t102 = t96+t95
       t103 = a1j2ry*t102
       t104 = a1j2syy*t14
       t105 = a1j2sy*t73
       t106 = a1j2sy*t85+a1j2ry*t98+t101+t103+t104+t105
       t108 = t105+t104+t103+t101
       t117 = a1j2sy*t102
       t119 = a1j2syy*a1j2sy*a1j2sx
       t120 = t117+t119
       t123 = t117+a1j2sy*t98+t119
       t136 = a1j2ryyy*a1j2rx
       t137 = a1j2ry*a1j2rxyy
       t139 = a1j2ryy*a1j2rxy
       t140 = 3*t139
       t141 = t136+2*t137+t140
       t146 = t136+2*t139+t137
       t151 = 3*t137+t136+t140
       t159 = a1j2ryyy*a1j2sx
       t160 = a1j2ry*a1j2sxyy
       t161 = a1j2syyy*a1j2rx
       t162 = a1j2syy*a1j2rxy
       t164 = a1j2sy*a1j2rxyy
       t165 = a1j2ryy*a1j2sxy
       t167 = t159+t160+t161+2*t162+t164+2*t165
       t174 = 3*t165
       t177 = 3*t162
       t178 = t174+2*t164+t159+2*t160+t161+t177
       t184 = 3*t160+3*t164+t161+t177+t159+t174
       t188 = a1j2syyy*a1j2ry*a1j2rx+a1j2ryyy*t14+a1j2ry*t167+2*
     & a1j2ryy*t73+2*a1j2syy*t54+a1j2ryy*t85+a1j2ry*t178+a1j2syy*t61+
     & a1j2sy*t151+a1j2ry*t184+a1j2sy*t146+a1j2sy*t141
       t195 = a1j2syyy*a1j2sx
       t196 = a1j2sy*a1j2sxyy
       t198 = a1j2syy*a1j2sxy
       t199 = 3*t198
       t200 = t195+2*t196+t199
       t203 = t195+3*t196+t199
       t208 = t195+2*t198+t196
       t215 = 2*a1j2syy*t73+a1j2sy*t178+a1j2ryyy*a1j2sy*a1j2sx+a1j2ry*
     & t200+a1j2ry*t203+a1j2syy*t85+a1j2syyy*t14+a1j2ry*t208+2*
     & a1j2ryy*t102+a1j2sy*t184+a1j2sy*t167+a1j2ryy*t98
       vv1xyyyy2 = t2*a1j2rx*vv1rrrrr+(a1j2sy*t1*a1j2ry*a1j2rx+a1j2ry*
     & t18)*vv1rrrrs+(a1j2ry*t27+a1j2sy*t18)*vv1rrrss+(a1j2ry*t36+
     & a1j2sy*t27)*vv1rrsss+(a1j2sy*t36+a1j2ry*t33*a1j2sy*a1j2sx)*
     & vv1rssss+t47*a1j2sx*vv1sssss+(a1j2ryy*t1*a1j2rx+a1j2ry*t58+
     & a1j2ry*t63)*vv1rrrr+(a1j2ry*t77+a1j2sy*t58+a1j2syy*t1*a1j2rx+
     & a1j2ry*t87+a1j2sy*t63+a1j2ryy*t16)*vv1rrrs+(a1j2syy*t16+a1j2ry*
     & t106+a1j2ry*t108+a1j2sy*t77+a1j2sy*t87+a1j2ryy*t25)*vv1rrss+(
     & a1j2syy*t25+a1j2sy*t108+a1j2ry*t120+a1j2ry*t123+a1j2sy*t106+
     & a1j2ryy*t33*a1j2sx)*vv1rsss+(a1j2syy*t33*a1j2sx+a1j2sy*t120+
     & a1j2sy*t123)*vv1ssss+(a1j2ry*t141+2*a1j2ryy*t54+a1j2ry*t146+
     & a1j2ryyy*a1j2ry*a1j2rx+a1j2ry*t151+a1j2ryy*t61)*vv1rrr+t188*
     & vv1rrs+t215*vv1rss+(a1j2syyy*a1j2sy*a1j2sx+2*a1j2syy*t102+
     & a1j2sy*t200+a1j2sy*t203+a1j2syy*t98+a1j2sy*t208)*vv1sss+(
     & a1j2ryyyy*a1j2rx+4*a1j2ry*a1j2rxyyy+4*a1j2ryyy*a1j2rxy+6*
     & a1j2ryy*a1j2rxyy)*vv1rr+(4*a1j2ryyy*a1j2sxy+a1j2syyyy*a1j2rx+4*
     & a1j2sy*a1j2rxyyy+6*a1j2syy*a1j2rxyy+4*a1j2ry*a1j2sxyyy+4*
     & a1j2syyy*a1j2rxy+6*a1j2ryy*a1j2sxyy+a1j2ryyyy*a1j2sx)*vv1rs+(4*
     & a1j2sy*a1j2sxyyy+a1j2syyyy*a1j2sx+4*a1j2syyy*a1j2sxy+6*a1j2syy*
     & a1j2sxyy)*vv1ss
       t1 = a1j2ry**2
       t2 = t1**2
       t8 = a1j2sy**2
       t9 = t1*a1j2ry
       t13 = t8*a1j2sy
       t17 = t8**2
       t26 = a1j2sy*a1j2ryy
       t29 = a1j2syy*t1
       t30 = a1j2syy*a1j2ry
       t31 = t26+t30
       t32 = 2*t31
       t33 = a1j2ry*t32
       t35 = a1j2sy*a1j2ry*a1j2ryy
       t37 = t29+t33+4*t35
       t41 = 3*t31
       t43 = 7*t35+t29+a1j2ry*t41+t33
       t52 = a1j2sy*t32
       t53 = t30*a1j2sy
       t55 = a1j2ryy*t8
       t56 = t52+4*t53+t55
       t60 = a1j2sy*t41+7*t53+t55+t52
       t78 = a1j2ry*a1j2ryyy
       t79 = a1j2ryy**2
       t80 = t78+t79
       t81 = 3*t80
       t87 = 4*t78+3*t79
       t89 = 2*t80
       t96 = a1j2sy*a1j2ryyy
       t98 = a1j2syyy*a1j2ry
       t100 = a1j2syy*a1j2ryy
       t102 = 2*t96+2*t98+4*t100
       t106 = 6*t100
       t109 = t106+3*t96+3*t98
       t117 = 4*t98+4*t96+t106
       t121 = 7*t30*a1j2ryy+a1j2sy*t89+a1j2ry*t102+a1j2ryy*t41+
     & a1j2syyy*t1+a1j2ry*t109+a1j2sy*t87+a1j2sy*t81+2*t96*a1j2ry+
     & a1j2ry*t117+2*a1j2ryy*t32
       t129 = a1j2syyy*a1j2sy
       t131 = a1j2syy**2
       t133 = 4*t129+3*t131
       t135 = t129+t131
       t136 = 2*t135
       t138 = 3*t135
       t145 = 7*t100*a1j2sy+a1j2sy*t109+2*a1j2syy*t32+a1j2sy*t117+
     & a1j2ry*t133+a1j2ry*t136+a1j2ry*t138+a1j2syy*t41+a1j2ryyy*t8+2*
     & t129*a1j2ry+a1j2sy*t102
       vv1yyyyy2 = t2*a1j2ry*vv1rrrrr+5*a1j2sy*t2*vv1rrrrs+10*t8*t9*
     & vv1rrrss+10*t13*t1*vv1rrsss+5*t17*a1j2ry*vv1rssss+t17*a1j2sy*
     & vv1sssss+10*t9*a1j2ryy*vv1rrrr+(12*t26*t1+a1j2ry*t37+a1j2syy*
     & t9+a1j2ry*t43)*vv1rrrs+(3*a1j2ryy*a1j2ry*t8+a1j2sy*t43+a1j2sy*
     & t37+a1j2ry*t56+a1j2ry*t60+3*t29*a1j2sy)*vv1rrss+(12*a1j2ry*t8*
     & a1j2syy+a1j2sy*t56+a1j2sy*t60+a1j2ryy*t13)*vv1rsss+10*t13*
     & a1j2syy*vv1ssss+(a1j2ryyy*t1+a1j2ry*t81+7*t79*a1j2ry+a1j2ry*
     & t87+a1j2ry*t89)*vv1rrr+t121*vv1rrs+t145*vv1rss+(a1j2sy*t138+
     & a1j2sy*t133+7*t131*a1j2sy+a1j2syyy*t8+a1j2sy*t136)*vv1sss+(5*
     & a1j2ry*a1j2ryyyy+10*a1j2ryyy*a1j2ryy)*vv1rr+(10*a1j2syy*
     & a1j2ryyy+10*a1j2syyy*a1j2ryy+5*a1j2sy*a1j2ryyyy+5*a1j2syyyy*
     & a1j2ry)*vv1rs+(10*a1j2syy*a1j2syyy+5*a1j2sy*a1j2syyyy)*vv1ss
       t1 = a2j2rx**2
       t2 = t1**2
       t8 = a2j2sx**2
       t9 = t1*a2j2rx
       t13 = t8*a2j2sx
       t17 = t8**2
       t29 = a2j2sxx*t1
       t30 = a2j2sx*a2j2rxx
       t31 = a2j2sxx*a2j2rx
       t32 = t30+t31
       t33 = 2*t32
       t34 = a2j2rx*t33
       t36 = a2j2sx*a2j2rx*a2j2rxx
       t38 = t29+t34+4*t36
       t42 = 3*t32
       t44 = 7*t36+t29+a2j2rx*t42+t34
       t51 = a2j2sx*t33
       t52 = t31*a2j2sx
       t54 = a2j2rxx*t8
       t55 = t51+4*t52+t54
       t59 = a2j2sx*t42+7*t52+t54+t51
       t77 = a2j2rx*a2j2rxxx
       t79 = a2j2rxx**2
       t81 = 4*t77+3*t79
       t85 = t77+t79
       t86 = 2*t85
       t88 = 3*t85
       t94 = a2j2sx*a2j2rxxx
       t99 = a2j2sxxx*a2j2rx
       t101 = a2j2sxx*a2j2rxx
       t103 = 2*t94+2*t99+4*t101
       t107 = 6*t101
       t108 = 4*t99+4*t94+t107
       t117 = t107+3*t94+3*t99
       t121 = a2j2sxxx*t1+2*t94*a2j2rx+a2j2sx*t88+a2j2rx*t103+a2j2rx*
     & t108+a2j2sx*t81+7*t31*a2j2rxx+a2j2rxx*t42+a2j2sx*t86+a2j2rx*
     & t117+2*a2j2rxx*t33
       t125 = a2j2sxxx*a2j2sx
       t127 = a2j2sxx**2
       t129 = 4*t125+3*t127
       t132 = t125+t127
       t133 = 3*t132
       t136 = 2*t132
       t145 = 7*t30*a2j2sxx+a2j2rx*t129+a2j2sx*t108+a2j2rx*t133+
     & a2j2rxxx*t8+a2j2rx*t136+2*a2j2sxx*t33+a2j2sx*t103+a2j2sxx*t42+
     & a2j2sx*t117+2*t125*a2j2rx
       uu2xxxxx2 = t2*a2j2rx*uu2rrrrr+5*a2j2sx*t2*uu2rrrrs+10*t8*t9*
     & uu2rrrss+10*t13*t1*uu2rrsss+5*t17*a2j2rx*uu2rssss+t17*a2j2sx*
     & uu2sssss+10*a2j2rxx*t9*uu2rrrr+(12*a2j2rxx*t1*a2j2sx+a2j2rx*
     & t38+a2j2sxx*t9+a2j2rx*t44)*uu2rrrs+(3*a2j2rxx*a2j2rx*t8+a2j2rx*
     & t55+a2j2rx*t59+a2j2sx*t44+3*t29*a2j2sx+a2j2sx*t38)*uu2rrss+(
     & a2j2rxx*t13+12*t31*t8+a2j2sx*t55+a2j2sx*t59)*uu2rsss+10*
     & a2j2sxx*t13*uu2ssss+(a2j2rx*t81+7*t79*a2j2rx+a2j2rx*t86+a2j2rx*
     & t88+a2j2rxxx*t1)*uu2rrr+t121*uu2rrs+t145*uu2rss+(a2j2sx*t129+7*
     & t127*a2j2sx+a2j2sx*t136+a2j2sx*t133+a2j2sxxx*t8)*uu2sss+(10*
     & a2j2rxx*a2j2rxxx+5*a2j2rx*a2j2rxxxx)*uu2rr+(5*a2j2sxxxx*a2j2rx+
     & 10*a2j2sxx*a2j2rxxx+5*a2j2sx*a2j2rxxxx+10*a2j2sxxx*a2j2rxx)*
     & uu2rs+(5*a2j2sxxxx*a2j2sx+10*a2j2sxx*a2j2sxxx)*uu2ss
       t1 = a2j2rx**2
       t2 = t1**2
       t5 = t1*a2j2rx
       t15 = a2j2ry*t1
       t16 = a2j2sx**2
       t22 = t16*a2j2sx
       t25 = a2j2sy*t1
       t30 = t16**2
       t47 = a2j2sx*a2j2rxy
       t50 = a2j2sxy*t1
       t51 = a2j2sxy*a2j2rx
       t53 = 2*t47+2*t51
       t55 = 4*t47*a2j2rx+t50+a2j2rx*t53
       t64 = a2j2sxx*a2j2rx
       t66 = t64+a2j2sx*a2j2rxx
       t67 = 3*t66
       t69 = 2*t66
       t71 = 7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t1+a2j2rx*t67+a2j2rx*t69
       t81 = 4*t51*a2j2sx+a2j2rxy*t16+a2j2sx*t53
       t89 = a2j2sx*t67+7*t64*a2j2sx+a2j2rxx*t16+a2j2sx*t69
       t115 = a2j2rxy*a2j2rxx
       t119 = a2j2rx*a2j2rxxy+t115
       t120 = 3*t119
       t122 = 2*t119
       t126 = a2j2rxx**2
       t128 = 4*a2j2rx*a2j2rxxx+3*t126
       t133 = a2j2sxy*a2j2rxx
       t137 = a2j2sx*a2j2rxxy
       t140 = a2j2sxx*a2j2rxy
       t142 = t137+t140+a2j2sxxy*a2j2rx+t133
       t143 = 3*t142
       t148 = 2*t142
       t160 = 4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+6*a2j2sxx*a2j2rxx
       t162 = a2j2sxxy*t1+5*a2j2rx*t133+a2j2sy*t128+2*t137*a2j2rx+
     & a2j2rx*t143+a2j2rxy*t69+a2j2rxx*t53+a2j2sx*t120+a2j2rx*t148+2*
     & t140*a2j2rx+a2j2rxy*t67+a2j2sx*t122+a2j2ry*t160
       t169 = a2j2sxxy*a2j2sx
       t173 = a2j2sxy*a2j2sxx+t169
       t174 = 3*t173
       t179 = 2*t173
       t183 = a2j2sxx**2
       t185 = 4*a2j2sxxx*a2j2sx+3*t183
       t190 = a2j2sxx*t53+5*t47*a2j2sxx+a2j2sxy*t67+a2j2sx*t143+2*t169*
     & a2j2rx+a2j2rx*t174+a2j2sxy*t69+a2j2sy*t160+a2j2sx*t148+a2j2rx*
     & t179+a2j2ry*t185+2*a2j2sx*t133+a2j2rxxy*t16
       uu2xxxxy2 = a2j2ry*t2*uu2rrrrr+(4*a2j2ry*t5*a2j2sx+a2j2sy*t2)*
     & uu2rrrrs+(4*a2j2sy*t5*a2j2sx+6*t15*t16)*uu2rrrss+(4*a2j2ry*
     & a2j2rx*t22+6*t25*t16)*uu2rrsss+(a2j2ry*t30+4*a2j2sy*a2j2rx*t22)
     & *uu2rssss+a2j2sy*t30*uu2sssss+(4*t5*a2j2rxy+6*t15*a2j2rxx)*
     & uu2rrrr+(6*t25*a2j2rxx+a2j2rx*t55+6*t47*t1+a2j2sxy*t5+a2j2ry*
     & t71)*uu2rrrs+(3*t50*a2j2sx+a2j2rx*t81+a2j2sy*t71+a2j2ry*t89+3*
     & a2j2rxy*a2j2rx*t16+a2j2sx*t55)*uu2rrss+(a2j2sy*t89+6*t51*t16+
     & a2j2sx*t81+a2j2rxy*t22+6*a2j2ry*t16*a2j2sxx)*uu2rsss+(4*t22*
     & a2j2sxy+6*a2j2sy*t16*a2j2sxx)*uu2ssss+(a2j2rxxy*t1+7*t115*
     & a2j2rx+a2j2rx*t120+a2j2rx*t122+a2j2ry*t128)*uu2rrr+t162*uu2rrs+
     & t190*uu2rss+(a2j2sx*t174+a2j2sxxy*t16+7*a2j2sxy*a2j2sx*a2j2sxx+
     & a2j2sx*t179+a2j2sy*t185)*uu2sss+(a2j2ry*a2j2rxxxx+4*a2j2rxxx*
     & a2j2rxy+6*a2j2rxxy*a2j2rxx+4*a2j2rxxxy*a2j2rx)*uu2rr+(4*
     & a2j2sxxx*a2j2rxy+a2j2ry*a2j2sxxxx+4*a2j2sxxxy*a2j2rx+4*a2j2sxy*
     & a2j2rxxx+4*a2j2sx*a2j2rxxxy+6*a2j2sxx*a2j2rxxy+a2j2sy*
     & a2j2rxxxx+6*a2j2sxxy*a2j2rxx)*uu2rs+(4*a2j2sxxx*a2j2sxy+4*
     & a2j2sxxxy*a2j2sx+6*a2j2sxx*a2j2sxxy+a2j2sy*a2j2sxxxx)*uu2ss
       t1 = a2j2ry**2
       t2 = a2j2rx**2
       t3 = t2*a2j2rx
       t6 = a2j2sy*a2j2ry
       t12 = 3*a2j2ry*t2*a2j2sx+a2j2sy*t3
       t16 = a2j2ry*a2j2rx
       t17 = a2j2sx**2
       t22 = 3*t16*t17+3*a2j2sy*t2*a2j2sx
       t27 = a2j2sy*a2j2rx
       t30 = t17*a2j2sx
       t32 = 3*t27*t17+a2j2ry*t30
       t41 = a2j2sy**2
       t47 = 3*a2j2rxy*t2+3*a2j2rxx*t16
       t55 = a2j2sx*a2j2rxy
       t57 = 4*a2j2rx*t55
       t58 = a2j2sxy*t2
       t59 = a2j2sxy*a2j2rx
       t61 = 2*t55+2*t59
       t62 = a2j2rx*t61
       t63 = t57+t58+t62
       t71 = 3*a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx
       t75 = t57+t58+t62+a2j2ry*t71+3*t27*a2j2rxx
       t86 = 4*a2j2sx*t59
       t87 = a2j2rxy*t17
       t88 = a2j2sx*t61
       t89 = t86+t87+t88
       t95 = t87+a2j2sy*t71+3*a2j2ry*a2j2sxx*a2j2sx+t86+t88
       t101 = a2j2ryy*a2j2rx
       t106 = a2j2syy*a2j2rx
       t118 = 3*a2j2sy*a2j2sxx*a2j2sx+3*t17*a2j2sxy
       t130 = a2j2rx*a2j2rxxy
       t131 = a2j2rxy*a2j2rxx
       t133 = 3*t130+3*t131
       t139 = 3*t131+3*t130+a2j2ry*a2j2rxxx
       t141 = a2j2rxy**2
       t146 = 2*a2j2rx*a2j2rxyy+2*t141
       t152 = a2j2sxy*a2j2rxy
       t155 = a2j2sxx*a2j2rxy
       t158 = a2j2sxy*a2j2rxx
       t160 = a2j2sx*a2j2rxxy
       t162 = a2j2sxxy*a2j2rx
       t165 = 3*t155+a2j2ry*a2j2sxxx+3*t158+3*t160+3*t162+a2j2sy*
     & a2j2rxxx
       t170 = a2j2sx*a2j2rxyy
       t175 = 2*t170+2*a2j2sxyy*a2j2rx+4*t152
       t184 = 3*t160+3*t155+3*t162+3*t158
       t188 = 4*t152*a2j2rx+a2j2ry*t165+a2j2ryy*t71+2*a2j2rxy*t61+
     & a2j2rx*t175+a2j2sx*t146+2*t170*a2j2rx+a2j2sy*t139+a2j2sy*t133+
     & a2j2sxyy*t2+a2j2ry*t184+3*t106*a2j2rxx
       t195 = a2j2sxy*a2j2sxx
       t196 = a2j2sxxy*a2j2sx
       t198 = 3*t195+3*t196
       t206 = 3*t196+a2j2sy*a2j2sxxx+3*t195
       t208 = a2j2sx*a2j2sxyy
       t209 = a2j2sxy**2
       t211 = 2*t208+2*t209
       t219 = a2j2sy*t184+a2j2sx*t175+a2j2rxyy*t17+2*a2j2sxy*t61+
     & a2j2ry*t198+3*a2j2ryy*a2j2sxx*a2j2sx+a2j2ry*t206+a2j2rx*t211+2*
     & t208*a2j2rx+a2j2sy*t165+4*t152*a2j2sx+a2j2syy*t71
       uu2xxxyy2 = t1*t3*uu2rrrrr+(t6*t3+a2j2ry*t12)*uu2rrrrs+(a2j2ry*
     & t22+a2j2sy*t12)*uu2rrrss+(a2j2ry*t32+a2j2sy*t22)*uu2rrsss+(t6*
     & t30+a2j2sy*t32)*uu2rssss+t41*t30*uu2sssss+(a2j2ry*t47+3*a2j2ry*
     & a2j2rxy*t2+a2j2ryy*t3)*uu2rrrr+(a2j2ry*t63+3*a2j2sy*a2j2rxy*t2+
     & a2j2ry*t75+a2j2syy*t3+a2j2sy*t47+3*a2j2ryy*t2*a2j2sx)*uu2rrrs+(
     & a2j2sy*t75+a2j2ry*t89+a2j2ry*t95+a2j2sy*t63+3*a2j2syy*t2*
     & a2j2sx+3*t101*t17)*uu2rrss+(3*t106*t17+a2j2sy*t89+3*a2j2ry*t17*
     & a2j2sxy+a2j2sy*t95+a2j2ry*t118+a2j2ryy*t30)*uu2rsss+(3*a2j2sy*
     & t17*a2j2sxy+a2j2syy*t30+a2j2sy*t118)*uu2ssss+(a2j2ry*t133+
     & a2j2rxyy*t2+a2j2ry*t139+4*t141*a2j2rx+a2j2rx*t146+3*t101*
     & a2j2rxx)*uu2rrr+t188*uu2rrs+t219*uu2rss+(4*t209*a2j2sx+a2j2sy*
     & t206+a2j2sx*t211+3*a2j2syy*a2j2sxx*a2j2sx+a2j2sy*t198+a2j2sxyy*
     & t17)*uu2sss+(3*a2j2rx*a2j2rxxyy+6*a2j2rxy*a2j2rxxy+3*a2j2rxx*
     & a2j2rxyy+a2j2ryy*a2j2rxxx+2*a2j2ry*a2j2rxxxy)*uu2rr+(3*
     & a2j2sxxyy*a2j2rx+6*a2j2sxxy*a2j2rxy+3*a2j2sxyy*a2j2rxx+3*
     & a2j2sx*a2j2rxxyy+3*a2j2sxx*a2j2rxyy+2*a2j2ry*a2j2sxxxy+6*
     & a2j2sxy*a2j2rxxy+a2j2ryy*a2j2sxxx+2*a2j2sy*a2j2rxxxy+a2j2syy*
     & a2j2rxxx)*uu2rs+(3*a2j2sxxyy*a2j2sx+3*a2j2sxyy*a2j2sxx+2*
     & a2j2sy*a2j2sxxxy+a2j2syy*a2j2sxxx+6*a2j2sxy*a2j2sxxy)*uu2ss
       t1 = a2j2ry**2
       t3 = a2j2rx**2
       t6 = a2j2sy*a2j2ry
       t12 = a2j2sy*t3+2*a2j2ry*a2j2sx*a2j2rx
       t14 = t6*t3+a2j2ry*t12
       t21 = a2j2sx**2
       t26 = a2j2ry*t21+2*a2j2sy*a2j2sx*a2j2rx
       t28 = a2j2sy*t12+a2j2ry*t26
       t36 = a2j2sy*t26+t6*t21
       t41 = a2j2sy**2
       t51 = 2*a2j2ry*a2j2rxy*a2j2rx
       t55 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
       t57 = a2j2ryy*t3
       t58 = t51+a2j2ry*t55+t57
       t62 = t51+t57
       t70 = 2*a2j2sy*a2j2rxy*a2j2rx
       t73 = 2*a2j2ryy*a2j2sx*a2j2rx
       t74 = a2j2syy*t3
       t76 = a2j2sx*a2j2rxy
       t78 = a2j2sxy*a2j2rx
       t81 = a2j2ry*a2j2sxx+2*t76+2*t78+a2j2sy*a2j2rxx
       t84 = 2*t76+2*t78
       t85 = a2j2ry*t84
       t86 = a2j2sy*t55+t70+t73+t74+a2j2ry*t81+t85
       t88 = t74+t85+t73+t70
       t96 = a2j2sy*t84
       t99 = 2*a2j2syy*a2j2sx*a2j2rx
       t102 = 2*a2j2ry*a2j2sxy*a2j2sx
       t103 = a2j2ryy*t21
       t104 = t96+t99+t102+t103
       t111 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
       t113 = a2j2sy*t81+a2j2ry*t111+t103+t102+t99+t96
       t120 = a2j2syy*t21
       t123 = 2*a2j2sy*a2j2sxy*a2j2sx
       t124 = t120+t123
       t132 = t123+a2j2sy*t111+t120
       t145 = a2j2ryy*a2j2rxx
       t146 = a2j2rx*a2j2rxyy
       t147 = 2*t146
       t148 = a2j2ry*a2j2rxxy
       t149 = a2j2rxy**2
       t150 = 2*t149
       t151 = t145+t147+t148+t150
       t155 = t147+t145+2*t148+t150
       t159 = 2*t146+2*t149
       t172 = 4*a2j2sxy*a2j2rxy
       t173 = a2j2ry*a2j2sxxy
       t175 = a2j2ryy*a2j2sxx
       t176 = a2j2sy*a2j2rxxy
       t179 = 2*a2j2sxyy*a2j2rx
       t180 = a2j2syy*a2j2rxx
       t182 = 2*a2j2sx*a2j2rxyy
       t183 = t172+2*t173+t175+2*t176+t179+t180+t182
       t191 = t176+t173+t175+t172+t179+t180+t182
       t193 = t182+t179+t172
       t195 = a2j2sy*t155+2*a2j2ryyy*a2j2sx*a2j2rx+a2j2syy*t55+2*
     & a2j2ryy*t84+a2j2syyy*t3+a2j2ry*t183+4*a2j2syy*a2j2rxy*a2j2rx+
     & a2j2sy*t151+a2j2ryy*t81+a2j2sy*t159+a2j2ry*t191+a2j2ry*t193
       t198 = a2j2sxy**2
       t199 = 2*t198
       t200 = a2j2sy*a2j2sxxy
       t202 = a2j2syy*a2j2sxx
       t203 = a2j2sx*a2j2sxyy
       t204 = 2*t203
       t205 = t199+2*t200+t202+t204
       t214 = t200+t204+t202+t199
       t223 = 2*t203+2*t198
       t225 = a2j2syy*t81+a2j2ry*t205+a2j2sy*t183+a2j2ryyy*t21+a2j2sy*
     & t191+4*a2j2ryy*a2j2sxy*a2j2sx+a2j2sy*t193+a2j2ry*t214+2*
     & a2j2syyy*a2j2sx*a2j2rx+2*a2j2syy*t84+a2j2ryy*t111+a2j2ry*t223
       uu2xxyyy2 = t1*a2j2ry*t3*uu2rrrrr+(a2j2ry*t14+a2j2sy*t1*t3)*
     & uu2rrrrs+(a2j2ry*t28+a2j2sy*t14)*uu2rrrss+(a2j2sy*t28+a2j2ry*
     & t36)*uu2rrsss+(a2j2sy*t36+a2j2ry*t41*t21)*uu2rssss+t41*a2j2sy*
     & t21*uu2sssss+(a2j2ry*t58+a2j2ryy*a2j2ry*t3+a2j2ry*t62)*uu2rrrr+
     & (a2j2sy*t58+a2j2ry*t86+a2j2ry*t88+a2j2syy*a2j2ry*t3+a2j2ryy*
     & t12+a2j2sy*t62)*uu2rrrs+(a2j2ry*t104+a2j2syy*t12+a2j2ry*t113+
     & a2j2sy*t88+a2j2sy*t86+a2j2ryy*t26)*uu2rrss+(a2j2ry*t124+a2j2sy*
     & t104+a2j2syy*t26+a2j2ryy*a2j2sy*t21+a2j2sy*t113+a2j2ry*t132)*
     & uu2rsss+(a2j2sy*t132+a2j2syy*a2j2sy*t21+a2j2sy*t124)*uu2ssss+(
     & 4*a2j2ryy*a2j2rxy*a2j2rx+a2j2ry*t151+a2j2ryyy*t3+a2j2ry*t155+
     & a2j2ryy*t55+a2j2ry*t159)*uu2rrr+t195*uu2rrs+t225*uu2rss+(
     & a2j2sy*t214+a2j2syyy*t21+4*a2j2syy*a2j2sxy*a2j2sx+a2j2syy*t111+
     & a2j2sy*t223+a2j2sy*t205)*uu2sss+(a2j2ryyy*a2j2rxx+3*a2j2ry*
     & a2j2rxxyy+6*a2j2rxy*a2j2rxyy+2*a2j2rx*a2j2rxyyy+3*a2j2ryy*
     & a2j2rxxy)*uu2rr+(a2j2ryyy*a2j2sxx+3*a2j2ry*a2j2sxxyy+3*a2j2syy*
     & a2j2rxxy+6*a2j2sxyy*a2j2rxy+a2j2syyy*a2j2rxx+2*a2j2sx*
     & a2j2rxyyy+3*a2j2sy*a2j2rxxyy+3*a2j2ryy*a2j2sxxy+6*a2j2sxy*
     & a2j2rxyy+2*a2j2sxyyy*a2j2rx)*uu2rs+(3*a2j2syy*a2j2sxxy+2*
     & a2j2sx*a2j2sxyyy+a2j2syyy*a2j2sxx+3*a2j2sy*a2j2sxxyy+6*
     & a2j2sxyy*a2j2sxy)*uu2ss
       t1 = a2j2ry**2
       t2 = t1**2
       t10 = a2j2sy*a2j2ry
       t14 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
       t16 = t10*a2j2rx+a2j2ry*t14
       t18 = a2j2sy*t1*a2j2rx+a2j2ry*t16
       t25 = t10*a2j2sx+a2j2sy*t14
       t27 = a2j2sy*t16+a2j2ry*t25
       t33 = a2j2sy**2
       t36 = a2j2sy*t25+a2j2ry*t33*a2j2sx
       t47 = t33**2
       t52 = a2j2ryy*a2j2rx
       t53 = a2j2ry*a2j2rxy
       t54 = t52+t53
       t55 = a2j2ry*t54
       t57 = a2j2ryy*a2j2ry*a2j2rx
       t58 = t55+t57
       t61 = t52+2*t53
       t63 = t55+a2j2ry*t61+t57
       t68 = a2j2syy*a2j2ry*a2j2rx
       t69 = a2j2sy*a2j2rxy
       t70 = a2j2ryy*a2j2sx
       t71 = a2j2syy*a2j2rx
       t72 = a2j2ry*a2j2sxy
       t73 = t69+t70+t71+t72
       t74 = a2j2ry*t73
       t75 = a2j2ryy*t14
       t76 = a2j2sy*t54
       t77 = t68+t74+t75+t76
       t85 = 2*t72+2*t69+t70+t71
       t87 = a2j2sy*t61+t76+t75+t68+a2j2ry*t85+t74
       t95 = a2j2syy*a2j2sx
       t96 = a2j2sy*a2j2sxy
       t98 = t95+2*t96
       t101 = a2j2ryy*a2j2sy*a2j2sx
       t102 = t96+t95
       t103 = a2j2ry*t102
       t104 = a2j2syy*t14
       t105 = a2j2sy*t73
       t106 = a2j2sy*t85+a2j2ry*t98+t101+t103+t104+t105
       t108 = t105+t104+t103+t101
       t117 = a2j2sy*t102
       t119 = a2j2syy*a2j2sy*a2j2sx
       t120 = t117+t119
       t123 = t117+a2j2sy*t98+t119
       t136 = a2j2ryyy*a2j2rx
       t137 = a2j2ry*a2j2rxyy
       t139 = a2j2ryy*a2j2rxy
       t140 = 3*t139
       t141 = t136+2*t137+t140
       t146 = t136+2*t139+t137
       t151 = 3*t137+t136+t140
       t159 = a2j2ryyy*a2j2sx
       t160 = a2j2ry*a2j2sxyy
       t161 = a2j2syyy*a2j2rx
       t162 = a2j2syy*a2j2rxy
       t164 = a2j2sy*a2j2rxyy
       t165 = a2j2ryy*a2j2sxy
       t167 = t159+t160+t161+2*t162+t164+2*t165
       t174 = 3*t165
       t177 = 3*t162
       t178 = t174+2*t164+t159+2*t160+t161+t177
       t184 = 3*t160+3*t164+t161+t177+t159+t174
       t188 = a2j2syyy*a2j2ry*a2j2rx+a2j2ryyy*t14+a2j2ry*t167+2*
     & a2j2ryy*t73+2*a2j2syy*t54+a2j2ryy*t85+a2j2ry*t178+a2j2syy*t61+
     & a2j2sy*t151+a2j2ry*t184+a2j2sy*t146+a2j2sy*t141
       t195 = a2j2syyy*a2j2sx
       t196 = a2j2sy*a2j2sxyy
       t198 = a2j2syy*a2j2sxy
       t199 = 3*t198
       t200 = t195+2*t196+t199
       t203 = t195+3*t196+t199
       t208 = t195+2*t198+t196
       t215 = 2*a2j2syy*t73+a2j2sy*t178+a2j2ryyy*a2j2sy*a2j2sx+a2j2ry*
     & t200+a2j2ry*t203+a2j2syy*t85+a2j2syyy*t14+a2j2ry*t208+2*
     & a2j2ryy*t102+a2j2sy*t184+a2j2sy*t167+a2j2ryy*t98
       uu2xyyyy2 = t2*a2j2rx*uu2rrrrr+(a2j2sy*t1*a2j2ry*a2j2rx+a2j2ry*
     & t18)*uu2rrrrs+(a2j2ry*t27+a2j2sy*t18)*uu2rrrss+(a2j2ry*t36+
     & a2j2sy*t27)*uu2rrsss+(a2j2sy*t36+a2j2ry*t33*a2j2sy*a2j2sx)*
     & uu2rssss+t47*a2j2sx*uu2sssss+(a2j2ryy*t1*a2j2rx+a2j2ry*t58+
     & a2j2ry*t63)*uu2rrrr+(a2j2ry*t77+a2j2sy*t58+a2j2syy*t1*a2j2rx+
     & a2j2ry*t87+a2j2sy*t63+a2j2ryy*t16)*uu2rrrs+(a2j2syy*t16+a2j2ry*
     & t106+a2j2ry*t108+a2j2sy*t77+a2j2sy*t87+a2j2ryy*t25)*uu2rrss+(
     & a2j2syy*t25+a2j2sy*t108+a2j2ry*t120+a2j2ry*t123+a2j2sy*t106+
     & a2j2ryy*t33*a2j2sx)*uu2rsss+(a2j2syy*t33*a2j2sx+a2j2sy*t120+
     & a2j2sy*t123)*uu2ssss+(a2j2ry*t141+2*a2j2ryy*t54+a2j2ry*t146+
     & a2j2ryyy*a2j2ry*a2j2rx+a2j2ry*t151+a2j2ryy*t61)*uu2rrr+t188*
     & uu2rrs+t215*uu2rss+(a2j2syyy*a2j2sy*a2j2sx+2*a2j2syy*t102+
     & a2j2sy*t200+a2j2sy*t203+a2j2syy*t98+a2j2sy*t208)*uu2sss+(
     & a2j2ryyyy*a2j2rx+4*a2j2ry*a2j2rxyyy+4*a2j2ryyy*a2j2rxy+6*
     & a2j2ryy*a2j2rxyy)*uu2rr+(4*a2j2ryyy*a2j2sxy+a2j2syyyy*a2j2rx+4*
     & a2j2sy*a2j2rxyyy+6*a2j2syy*a2j2rxyy+4*a2j2ry*a2j2sxyyy+4*
     & a2j2syyy*a2j2rxy+6*a2j2ryy*a2j2sxyy+a2j2ryyyy*a2j2sx)*uu2rs+(4*
     & a2j2sy*a2j2sxyyy+a2j2syyyy*a2j2sx+4*a2j2syyy*a2j2sxy+6*a2j2syy*
     & a2j2sxyy)*uu2ss
       t1 = a2j2ry**2
       t2 = t1**2
       t8 = a2j2sy**2
       t9 = t1*a2j2ry
       t13 = t8*a2j2sy
       t17 = t8**2
       t26 = a2j2sy*a2j2ryy
       t29 = a2j2syy*t1
       t30 = a2j2syy*a2j2ry
       t31 = t26+t30
       t32 = 2*t31
       t33 = a2j2ry*t32
       t35 = a2j2sy*a2j2ry*a2j2ryy
       t37 = t29+t33+4*t35
       t41 = 3*t31
       t43 = 7*t35+t29+a2j2ry*t41+t33
       t52 = a2j2sy*t32
       t53 = t30*a2j2sy
       t55 = a2j2ryy*t8
       t56 = t52+4*t53+t55
       t60 = a2j2sy*t41+7*t53+t55+t52
       t78 = a2j2ry*a2j2ryyy
       t79 = a2j2ryy**2
       t80 = t78+t79
       t81 = 3*t80
       t87 = 4*t78+3*t79
       t89 = 2*t80
       t96 = a2j2sy*a2j2ryyy
       t98 = a2j2syyy*a2j2ry
       t100 = a2j2syy*a2j2ryy
       t102 = 2*t96+2*t98+4*t100
       t106 = 6*t100
       t109 = t106+3*t96+3*t98
       t117 = 4*t98+4*t96+t106
       t121 = 7*t30*a2j2ryy+a2j2sy*t89+a2j2ry*t102+a2j2ryy*t41+
     & a2j2syyy*t1+a2j2ry*t109+a2j2sy*t87+a2j2sy*t81+2*t96*a2j2ry+
     & a2j2ry*t117+2*a2j2ryy*t32
       t129 = a2j2syyy*a2j2sy
       t131 = a2j2syy**2
       t133 = 4*t129+3*t131
       t135 = t129+t131
       t136 = 2*t135
       t138 = 3*t135
       t145 = 7*t100*a2j2sy+a2j2sy*t109+2*a2j2syy*t32+a2j2sy*t117+
     & a2j2ry*t133+a2j2ry*t136+a2j2ry*t138+a2j2syy*t41+a2j2ryyy*t8+2*
     & t129*a2j2ry+a2j2sy*t102
       uu2yyyyy2 = t2*a2j2ry*uu2rrrrr+5*a2j2sy*t2*uu2rrrrs+10*t8*t9*
     & uu2rrrss+10*t13*t1*uu2rrsss+5*t17*a2j2ry*uu2rssss+t17*a2j2sy*
     & uu2sssss+10*t9*a2j2ryy*uu2rrrr+(12*t26*t1+a2j2ry*t37+a2j2syy*
     & t9+a2j2ry*t43)*uu2rrrs+(3*a2j2ryy*a2j2ry*t8+a2j2sy*t43+a2j2sy*
     & t37+a2j2ry*t56+a2j2ry*t60+3*t29*a2j2sy)*uu2rrss+(12*a2j2ry*t8*
     & a2j2syy+a2j2sy*t56+a2j2sy*t60+a2j2ryy*t13)*uu2rsss+10*t13*
     & a2j2syy*uu2ssss+(a2j2ryyy*t1+a2j2ry*t81+7*t79*a2j2ry+a2j2ry*
     & t87+a2j2ry*t89)*uu2rrr+t121*uu2rrs+t145*uu2rss+(a2j2sy*t138+
     & a2j2sy*t133+7*t131*a2j2sy+a2j2syyy*t8+a2j2sy*t136)*uu2sss+(5*
     & a2j2ry*a2j2ryyyy+10*a2j2ryyy*a2j2ryy)*uu2rr+(10*a2j2syy*
     & a2j2ryyy+10*a2j2syyy*a2j2ryy+5*a2j2sy*a2j2ryyyy+5*a2j2syyyy*
     & a2j2ry)*uu2rs+(10*a2j2syy*a2j2syyy+5*a2j2sy*a2j2syyyy)*uu2ss
       t1 = a2j2rx**2
       t2 = t1**2
       t8 = a2j2sx**2
       t9 = t1*a2j2rx
       t13 = t8*a2j2sx
       t17 = t8**2
       t29 = a2j2sxx*t1
       t30 = a2j2sx*a2j2rxx
       t31 = a2j2sxx*a2j2rx
       t32 = t30+t31
       t33 = 2*t32
       t34 = a2j2rx*t33
       t36 = a2j2sx*a2j2rx*a2j2rxx
       t38 = t29+t34+4*t36
       t42 = 3*t32
       t44 = 7*t36+t29+a2j2rx*t42+t34
       t51 = a2j2sx*t33
       t52 = t31*a2j2sx
       t54 = a2j2rxx*t8
       t55 = t51+4*t52+t54
       t59 = a2j2sx*t42+7*t52+t54+t51
       t77 = a2j2rx*a2j2rxxx
       t79 = a2j2rxx**2
       t81 = 4*t77+3*t79
       t85 = t77+t79
       t86 = 2*t85
       t88 = 3*t85
       t94 = a2j2sx*a2j2rxxx
       t99 = a2j2sxxx*a2j2rx
       t101 = a2j2sxx*a2j2rxx
       t103 = 2*t94+2*t99+4*t101
       t107 = 6*t101
       t108 = 4*t99+4*t94+t107
       t117 = t107+3*t94+3*t99
       t121 = a2j2sxxx*t1+2*t94*a2j2rx+a2j2sx*t88+a2j2rx*t103+a2j2rx*
     & t108+a2j2sx*t81+7*t31*a2j2rxx+a2j2rxx*t42+a2j2sx*t86+a2j2rx*
     & t117+2*a2j2rxx*t33
       t125 = a2j2sxxx*a2j2sx
       t127 = a2j2sxx**2
       t129 = 4*t125+3*t127
       t132 = t125+t127
       t133 = 3*t132
       t136 = 2*t132
       t145 = 7*t30*a2j2sxx+a2j2rx*t129+a2j2sx*t108+a2j2rx*t133+
     & a2j2rxxx*t8+a2j2rx*t136+2*a2j2sxx*t33+a2j2sx*t103+a2j2sxx*t42+
     & a2j2sx*t117+2*t125*a2j2rx
       vv2xxxxx2 = t2*a2j2rx*vv2rrrrr+5*a2j2sx*t2*vv2rrrrs+10*t8*t9*
     & vv2rrrss+10*t13*t1*vv2rrsss+5*t17*a2j2rx*vv2rssss+t17*a2j2sx*
     & vv2sssss+10*a2j2rxx*t9*vv2rrrr+(12*a2j2rxx*t1*a2j2sx+a2j2rx*
     & t38+a2j2sxx*t9+a2j2rx*t44)*vv2rrrs+(3*a2j2rxx*a2j2rx*t8+a2j2rx*
     & t55+a2j2rx*t59+a2j2sx*t44+3*t29*a2j2sx+a2j2sx*t38)*vv2rrss+(
     & a2j2rxx*t13+12*t31*t8+a2j2sx*t55+a2j2sx*t59)*vv2rsss+10*
     & a2j2sxx*t13*vv2ssss+(a2j2rx*t81+7*t79*a2j2rx+a2j2rx*t86+a2j2rx*
     & t88+a2j2rxxx*t1)*vv2rrr+t121*vv2rrs+t145*vv2rss+(a2j2sx*t129+7*
     & t127*a2j2sx+a2j2sx*t136+a2j2sx*t133+a2j2sxxx*t8)*vv2sss+(10*
     & a2j2rxx*a2j2rxxx+5*a2j2rx*a2j2rxxxx)*vv2rr+(5*a2j2sxxxx*a2j2rx+
     & 10*a2j2sxx*a2j2rxxx+5*a2j2sx*a2j2rxxxx+10*a2j2sxxx*a2j2rxx)*
     & vv2rs+(5*a2j2sxxxx*a2j2sx+10*a2j2sxx*a2j2sxxx)*vv2ss
       t1 = a2j2rx**2
       t2 = t1**2
       t5 = t1*a2j2rx
       t15 = a2j2ry*t1
       t16 = a2j2sx**2
       t22 = t16*a2j2sx
       t25 = a2j2sy*t1
       t30 = t16**2
       t47 = a2j2sx*a2j2rxy
       t50 = a2j2sxy*t1
       t51 = a2j2sxy*a2j2rx
       t53 = 2*t47+2*t51
       t55 = 4*t47*a2j2rx+t50+a2j2rx*t53
       t64 = a2j2sxx*a2j2rx
       t66 = t64+a2j2sx*a2j2rxx
       t67 = 3*t66
       t69 = 2*t66
       t71 = 7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t1+a2j2rx*t67+a2j2rx*t69
       t81 = 4*t51*a2j2sx+a2j2rxy*t16+a2j2sx*t53
       t89 = a2j2sx*t67+7*t64*a2j2sx+a2j2rxx*t16+a2j2sx*t69
       t115 = a2j2rxy*a2j2rxx
       t119 = a2j2rx*a2j2rxxy+t115
       t120 = 3*t119
       t122 = 2*t119
       t126 = a2j2rxx**2
       t128 = 4*a2j2rx*a2j2rxxx+3*t126
       t133 = a2j2sxy*a2j2rxx
       t137 = a2j2sx*a2j2rxxy
       t140 = a2j2sxx*a2j2rxy
       t142 = t137+t140+a2j2sxxy*a2j2rx+t133
       t143 = 3*t142
       t148 = 2*t142
       t160 = 4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+6*a2j2sxx*a2j2rxx
       t162 = a2j2sxxy*t1+5*a2j2rx*t133+a2j2sy*t128+2*t137*a2j2rx+
     & a2j2rx*t143+a2j2rxy*t69+a2j2rxx*t53+a2j2sx*t120+a2j2rx*t148+2*
     & t140*a2j2rx+a2j2rxy*t67+a2j2sx*t122+a2j2ry*t160
       t169 = a2j2sxxy*a2j2sx
       t173 = a2j2sxy*a2j2sxx+t169
       t174 = 3*t173
       t179 = 2*t173
       t183 = a2j2sxx**2
       t185 = 4*a2j2sxxx*a2j2sx+3*t183
       t190 = a2j2sxx*t53+5*t47*a2j2sxx+a2j2sxy*t67+a2j2sx*t143+2*t169*
     & a2j2rx+a2j2rx*t174+a2j2sxy*t69+a2j2sy*t160+a2j2sx*t148+a2j2rx*
     & t179+a2j2ry*t185+2*a2j2sx*t133+a2j2rxxy*t16
       vv2xxxxy2 = a2j2ry*t2*vv2rrrrr+(4*a2j2ry*t5*a2j2sx+a2j2sy*t2)*
     & vv2rrrrs+(4*a2j2sy*t5*a2j2sx+6*t15*t16)*vv2rrrss+(4*a2j2ry*
     & a2j2rx*t22+6*t25*t16)*vv2rrsss+(a2j2ry*t30+4*a2j2sy*a2j2rx*t22)
     & *vv2rssss+a2j2sy*t30*vv2sssss+(4*t5*a2j2rxy+6*t15*a2j2rxx)*
     & vv2rrrr+(6*t25*a2j2rxx+a2j2rx*t55+6*t47*t1+a2j2sxy*t5+a2j2ry*
     & t71)*vv2rrrs+(3*t50*a2j2sx+a2j2rx*t81+a2j2sy*t71+a2j2ry*t89+3*
     & a2j2rxy*a2j2rx*t16+a2j2sx*t55)*vv2rrss+(a2j2sy*t89+6*t51*t16+
     & a2j2sx*t81+a2j2rxy*t22+6*a2j2ry*t16*a2j2sxx)*vv2rsss+(4*t22*
     & a2j2sxy+6*a2j2sy*t16*a2j2sxx)*vv2ssss+(a2j2rxxy*t1+7*t115*
     & a2j2rx+a2j2rx*t120+a2j2rx*t122+a2j2ry*t128)*vv2rrr+t162*vv2rrs+
     & t190*vv2rss+(a2j2sx*t174+a2j2sxxy*t16+7*a2j2sxy*a2j2sx*a2j2sxx+
     & a2j2sx*t179+a2j2sy*t185)*vv2sss+(a2j2ry*a2j2rxxxx+4*a2j2rxxx*
     & a2j2rxy+6*a2j2rxxy*a2j2rxx+4*a2j2rxxxy*a2j2rx)*vv2rr+(4*
     & a2j2sxxx*a2j2rxy+a2j2ry*a2j2sxxxx+4*a2j2sxxxy*a2j2rx+4*a2j2sxy*
     & a2j2rxxx+4*a2j2sx*a2j2rxxxy+6*a2j2sxx*a2j2rxxy+a2j2sy*
     & a2j2rxxxx+6*a2j2sxxy*a2j2rxx)*vv2rs+(4*a2j2sxxx*a2j2sxy+4*
     & a2j2sxxxy*a2j2sx+6*a2j2sxx*a2j2sxxy+a2j2sy*a2j2sxxxx)*vv2ss
       t1 = a2j2ry**2
       t2 = a2j2rx**2
       t3 = t2*a2j2rx
       t6 = a2j2sy*a2j2ry
       t12 = 3*a2j2ry*t2*a2j2sx+a2j2sy*t3
       t16 = a2j2ry*a2j2rx
       t17 = a2j2sx**2
       t22 = 3*t16*t17+3*a2j2sy*t2*a2j2sx
       t27 = a2j2sy*a2j2rx
       t30 = t17*a2j2sx
       t32 = 3*t27*t17+a2j2ry*t30
       t41 = a2j2sy**2
       t47 = 3*a2j2rxy*t2+3*a2j2rxx*t16
       t55 = a2j2sx*a2j2rxy
       t57 = 4*a2j2rx*t55
       t58 = a2j2sxy*t2
       t59 = a2j2sxy*a2j2rx
       t61 = 2*t55+2*t59
       t62 = a2j2rx*t61
       t63 = t57+t58+t62
       t71 = 3*a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx
       t75 = t57+t58+t62+a2j2ry*t71+3*t27*a2j2rxx
       t86 = 4*a2j2sx*t59
       t87 = a2j2rxy*t17
       t88 = a2j2sx*t61
       t89 = t86+t87+t88
       t95 = t87+a2j2sy*t71+3*a2j2ry*a2j2sxx*a2j2sx+t86+t88
       t101 = a2j2ryy*a2j2rx
       t106 = a2j2syy*a2j2rx
       t118 = 3*a2j2sy*a2j2sxx*a2j2sx+3*t17*a2j2sxy
       t130 = a2j2rx*a2j2rxxy
       t131 = a2j2rxy*a2j2rxx
       t133 = 3*t130+3*t131
       t139 = 3*t131+3*t130+a2j2ry*a2j2rxxx
       t141 = a2j2rxy**2
       t146 = 2*a2j2rx*a2j2rxyy+2*t141
       t152 = a2j2sxy*a2j2rxy
       t155 = a2j2sxx*a2j2rxy
       t158 = a2j2sxy*a2j2rxx
       t160 = a2j2sx*a2j2rxxy
       t162 = a2j2sxxy*a2j2rx
       t165 = 3*t155+a2j2ry*a2j2sxxx+3*t158+3*t160+3*t162+a2j2sy*
     & a2j2rxxx
       t170 = a2j2sx*a2j2rxyy
       t175 = 2*t170+2*a2j2sxyy*a2j2rx+4*t152
       t184 = 3*t160+3*t155+3*t162+3*t158
       t188 = 4*t152*a2j2rx+a2j2ry*t165+a2j2ryy*t71+2*a2j2rxy*t61+
     & a2j2rx*t175+a2j2sx*t146+2*t170*a2j2rx+a2j2sy*t139+a2j2sy*t133+
     & a2j2sxyy*t2+a2j2ry*t184+3*t106*a2j2rxx
       t195 = a2j2sxy*a2j2sxx
       t196 = a2j2sxxy*a2j2sx
       t198 = 3*t195+3*t196
       t206 = 3*t196+a2j2sy*a2j2sxxx+3*t195
       t208 = a2j2sx*a2j2sxyy
       t209 = a2j2sxy**2
       t211 = 2*t208+2*t209
       t219 = a2j2sy*t184+a2j2sx*t175+a2j2rxyy*t17+2*a2j2sxy*t61+
     & a2j2ry*t198+3*a2j2ryy*a2j2sxx*a2j2sx+a2j2ry*t206+a2j2rx*t211+2*
     & t208*a2j2rx+a2j2sy*t165+4*t152*a2j2sx+a2j2syy*t71
       vv2xxxyy2 = t1*t3*vv2rrrrr+(t6*t3+a2j2ry*t12)*vv2rrrrs+(a2j2ry*
     & t22+a2j2sy*t12)*vv2rrrss+(a2j2ry*t32+a2j2sy*t22)*vv2rrsss+(t6*
     & t30+a2j2sy*t32)*vv2rssss+t41*t30*vv2sssss+(a2j2ry*t47+3*a2j2ry*
     & a2j2rxy*t2+a2j2ryy*t3)*vv2rrrr+(a2j2ry*t63+3*a2j2sy*a2j2rxy*t2+
     & a2j2ry*t75+a2j2syy*t3+a2j2sy*t47+3*a2j2ryy*t2*a2j2sx)*vv2rrrs+(
     & a2j2sy*t75+a2j2ry*t89+a2j2ry*t95+a2j2sy*t63+3*a2j2syy*t2*
     & a2j2sx+3*t101*t17)*vv2rrss+(3*t106*t17+a2j2sy*t89+3*a2j2ry*t17*
     & a2j2sxy+a2j2sy*t95+a2j2ry*t118+a2j2ryy*t30)*vv2rsss+(3*a2j2sy*
     & t17*a2j2sxy+a2j2syy*t30+a2j2sy*t118)*vv2ssss+(a2j2ry*t133+
     & a2j2rxyy*t2+a2j2ry*t139+4*t141*a2j2rx+a2j2rx*t146+3*t101*
     & a2j2rxx)*vv2rrr+t188*vv2rrs+t219*vv2rss+(4*t209*a2j2sx+a2j2sy*
     & t206+a2j2sx*t211+3*a2j2syy*a2j2sxx*a2j2sx+a2j2sy*t198+a2j2sxyy*
     & t17)*vv2sss+(3*a2j2rx*a2j2rxxyy+6*a2j2rxy*a2j2rxxy+3*a2j2rxx*
     & a2j2rxyy+a2j2ryy*a2j2rxxx+2*a2j2ry*a2j2rxxxy)*vv2rr+(3*
     & a2j2sxxyy*a2j2rx+6*a2j2sxxy*a2j2rxy+3*a2j2sxyy*a2j2rxx+3*
     & a2j2sx*a2j2rxxyy+3*a2j2sxx*a2j2rxyy+2*a2j2ry*a2j2sxxxy+6*
     & a2j2sxy*a2j2rxxy+a2j2ryy*a2j2sxxx+2*a2j2sy*a2j2rxxxy+a2j2syy*
     & a2j2rxxx)*vv2rs+(3*a2j2sxxyy*a2j2sx+3*a2j2sxyy*a2j2sxx+2*
     & a2j2sy*a2j2sxxxy+a2j2syy*a2j2sxxx+6*a2j2sxy*a2j2sxxy)*vv2ss
       t1 = a2j2ry**2
       t3 = a2j2rx**2
       t6 = a2j2sy*a2j2ry
       t12 = a2j2sy*t3+2*a2j2ry*a2j2sx*a2j2rx
       t14 = t6*t3+a2j2ry*t12
       t21 = a2j2sx**2
       t26 = a2j2ry*t21+2*a2j2sy*a2j2sx*a2j2rx
       t28 = a2j2sy*t12+a2j2ry*t26
       t36 = a2j2sy*t26+t6*t21
       t41 = a2j2sy**2
       t51 = 2*a2j2ry*a2j2rxy*a2j2rx
       t55 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
       t57 = a2j2ryy*t3
       t58 = t51+a2j2ry*t55+t57
       t62 = t51+t57
       t70 = 2*a2j2sy*a2j2rxy*a2j2rx
       t73 = 2*a2j2ryy*a2j2sx*a2j2rx
       t74 = a2j2syy*t3
       t76 = a2j2sx*a2j2rxy
       t78 = a2j2sxy*a2j2rx
       t81 = a2j2ry*a2j2sxx+2*t76+2*t78+a2j2sy*a2j2rxx
       t84 = 2*t76+2*t78
       t85 = a2j2ry*t84
       t86 = a2j2sy*t55+t70+t73+t74+a2j2ry*t81+t85
       t88 = t74+t85+t73+t70
       t96 = a2j2sy*t84
       t99 = 2*a2j2syy*a2j2sx*a2j2rx
       t102 = 2*a2j2ry*a2j2sxy*a2j2sx
       t103 = a2j2ryy*t21
       t104 = t96+t99+t102+t103
       t111 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
       t113 = a2j2sy*t81+a2j2ry*t111+t103+t102+t99+t96
       t120 = a2j2syy*t21
       t123 = 2*a2j2sy*a2j2sxy*a2j2sx
       t124 = t120+t123
       t132 = t123+a2j2sy*t111+t120
       t145 = a2j2ryy*a2j2rxx
       t146 = a2j2rx*a2j2rxyy
       t147 = 2*t146
       t148 = a2j2ry*a2j2rxxy
       t149 = a2j2rxy**2
       t150 = 2*t149
       t151 = t145+t147+t148+t150
       t155 = t147+t145+2*t148+t150
       t159 = 2*t146+2*t149
       t172 = 4*a2j2sxy*a2j2rxy
       t173 = a2j2ry*a2j2sxxy
       t175 = a2j2ryy*a2j2sxx
       t176 = a2j2sy*a2j2rxxy
       t179 = 2*a2j2sxyy*a2j2rx
       t180 = a2j2syy*a2j2rxx
       t182 = 2*a2j2sx*a2j2rxyy
       t183 = t172+2*t173+t175+2*t176+t179+t180+t182
       t191 = t176+t173+t175+t172+t179+t180+t182
       t193 = t182+t179+t172
       t195 = a2j2sy*t155+2*a2j2ryyy*a2j2sx*a2j2rx+a2j2syy*t55+2*
     & a2j2ryy*t84+a2j2syyy*t3+a2j2ry*t183+4*a2j2syy*a2j2rxy*a2j2rx+
     & a2j2sy*t151+a2j2ryy*t81+a2j2sy*t159+a2j2ry*t191+a2j2ry*t193
       t198 = a2j2sxy**2
       t199 = 2*t198
       t200 = a2j2sy*a2j2sxxy
       t202 = a2j2syy*a2j2sxx
       t203 = a2j2sx*a2j2sxyy
       t204 = 2*t203
       t205 = t199+2*t200+t202+t204
       t214 = t200+t204+t202+t199
       t223 = 2*t203+2*t198
       t225 = a2j2syy*t81+a2j2ry*t205+a2j2sy*t183+a2j2ryyy*t21+a2j2sy*
     & t191+4*a2j2ryy*a2j2sxy*a2j2sx+a2j2sy*t193+a2j2ry*t214+2*
     & a2j2syyy*a2j2sx*a2j2rx+2*a2j2syy*t84+a2j2ryy*t111+a2j2ry*t223
       vv2xxyyy2 = t1*a2j2ry*t3*vv2rrrrr+(a2j2ry*t14+a2j2sy*t1*t3)*
     & vv2rrrrs+(a2j2ry*t28+a2j2sy*t14)*vv2rrrss+(a2j2sy*t28+a2j2ry*
     & t36)*vv2rrsss+(a2j2sy*t36+a2j2ry*t41*t21)*vv2rssss+t41*a2j2sy*
     & t21*vv2sssss+(a2j2ry*t58+a2j2ryy*a2j2ry*t3+a2j2ry*t62)*vv2rrrr+
     & (a2j2sy*t58+a2j2ry*t86+a2j2ry*t88+a2j2syy*a2j2ry*t3+a2j2ryy*
     & t12+a2j2sy*t62)*vv2rrrs+(a2j2ry*t104+a2j2syy*t12+a2j2ry*t113+
     & a2j2sy*t88+a2j2sy*t86+a2j2ryy*t26)*vv2rrss+(a2j2ry*t124+a2j2sy*
     & t104+a2j2syy*t26+a2j2ryy*a2j2sy*t21+a2j2sy*t113+a2j2ry*t132)*
     & vv2rsss+(a2j2sy*t132+a2j2syy*a2j2sy*t21+a2j2sy*t124)*vv2ssss+(
     & 4*a2j2ryy*a2j2rxy*a2j2rx+a2j2ry*t151+a2j2ryyy*t3+a2j2ry*t155+
     & a2j2ryy*t55+a2j2ry*t159)*vv2rrr+t195*vv2rrs+t225*vv2rss+(
     & a2j2sy*t214+a2j2syyy*t21+4*a2j2syy*a2j2sxy*a2j2sx+a2j2syy*t111+
     & a2j2sy*t223+a2j2sy*t205)*vv2sss+(a2j2ryyy*a2j2rxx+3*a2j2ry*
     & a2j2rxxyy+6*a2j2rxy*a2j2rxyy+2*a2j2rx*a2j2rxyyy+3*a2j2ryy*
     & a2j2rxxy)*vv2rr+(a2j2ryyy*a2j2sxx+3*a2j2ry*a2j2sxxyy+3*a2j2syy*
     & a2j2rxxy+6*a2j2sxyy*a2j2rxy+a2j2syyy*a2j2rxx+2*a2j2sx*
     & a2j2rxyyy+3*a2j2sy*a2j2rxxyy+3*a2j2ryy*a2j2sxxy+6*a2j2sxy*
     & a2j2rxyy+2*a2j2sxyyy*a2j2rx)*vv2rs+(3*a2j2syy*a2j2sxxy+2*
     & a2j2sx*a2j2sxyyy+a2j2syyy*a2j2sxx+3*a2j2sy*a2j2sxxyy+6*
     & a2j2sxyy*a2j2sxy)*vv2ss
       t1 = a2j2ry**2
       t2 = t1**2
       t10 = a2j2sy*a2j2ry
       t14 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
       t16 = t10*a2j2rx+a2j2ry*t14
       t18 = a2j2sy*t1*a2j2rx+a2j2ry*t16
       t25 = t10*a2j2sx+a2j2sy*t14
       t27 = a2j2sy*t16+a2j2ry*t25
       t33 = a2j2sy**2
       t36 = a2j2sy*t25+a2j2ry*t33*a2j2sx
       t47 = t33**2
       t52 = a2j2ryy*a2j2rx
       t53 = a2j2ry*a2j2rxy
       t54 = t52+t53
       t55 = a2j2ry*t54
       t57 = a2j2ryy*a2j2ry*a2j2rx
       t58 = t55+t57
       t61 = t52+2*t53
       t63 = t55+a2j2ry*t61+t57
       t68 = a2j2syy*a2j2ry*a2j2rx
       t69 = a2j2sy*a2j2rxy
       t70 = a2j2ryy*a2j2sx
       t71 = a2j2syy*a2j2rx
       t72 = a2j2ry*a2j2sxy
       t73 = t69+t70+t71+t72
       t74 = a2j2ry*t73
       t75 = a2j2ryy*t14
       t76 = a2j2sy*t54
       t77 = t68+t74+t75+t76
       t85 = 2*t72+2*t69+t70+t71
       t87 = a2j2sy*t61+t76+t75+t68+a2j2ry*t85+t74
       t95 = a2j2syy*a2j2sx
       t96 = a2j2sy*a2j2sxy
       t98 = t95+2*t96
       t101 = a2j2ryy*a2j2sy*a2j2sx
       t102 = t96+t95
       t103 = a2j2ry*t102
       t104 = a2j2syy*t14
       t105 = a2j2sy*t73
       t106 = a2j2sy*t85+a2j2ry*t98+t101+t103+t104+t105
       t108 = t105+t104+t103+t101
       t117 = a2j2sy*t102
       t119 = a2j2syy*a2j2sy*a2j2sx
       t120 = t117+t119
       t123 = t117+a2j2sy*t98+t119
       t136 = a2j2ryyy*a2j2rx
       t137 = a2j2ry*a2j2rxyy
       t139 = a2j2ryy*a2j2rxy
       t140 = 3*t139
       t141 = t136+2*t137+t140
       t146 = t136+2*t139+t137
       t151 = 3*t137+t136+t140
       t159 = a2j2ryyy*a2j2sx
       t160 = a2j2ry*a2j2sxyy
       t161 = a2j2syyy*a2j2rx
       t162 = a2j2syy*a2j2rxy
       t164 = a2j2sy*a2j2rxyy
       t165 = a2j2ryy*a2j2sxy
       t167 = t159+t160+t161+2*t162+t164+2*t165
       t174 = 3*t165
       t177 = 3*t162
       t178 = t174+2*t164+t159+2*t160+t161+t177
       t184 = 3*t160+3*t164+t161+t177+t159+t174
       t188 = a2j2syyy*a2j2ry*a2j2rx+a2j2ryyy*t14+a2j2ry*t167+2*
     & a2j2ryy*t73+2*a2j2syy*t54+a2j2ryy*t85+a2j2ry*t178+a2j2syy*t61+
     & a2j2sy*t151+a2j2ry*t184+a2j2sy*t146+a2j2sy*t141
       t195 = a2j2syyy*a2j2sx
       t196 = a2j2sy*a2j2sxyy
       t198 = a2j2syy*a2j2sxy
       t199 = 3*t198
       t200 = t195+2*t196+t199
       t203 = t195+3*t196+t199
       t208 = t195+2*t198+t196
       t215 = 2*a2j2syy*t73+a2j2sy*t178+a2j2ryyy*a2j2sy*a2j2sx+a2j2ry*
     & t200+a2j2ry*t203+a2j2syy*t85+a2j2syyy*t14+a2j2ry*t208+2*
     & a2j2ryy*t102+a2j2sy*t184+a2j2sy*t167+a2j2ryy*t98
       vv2xyyyy2 = t2*a2j2rx*vv2rrrrr+(a2j2sy*t1*a2j2ry*a2j2rx+a2j2ry*
     & t18)*vv2rrrrs+(a2j2ry*t27+a2j2sy*t18)*vv2rrrss+(a2j2ry*t36+
     & a2j2sy*t27)*vv2rrsss+(a2j2sy*t36+a2j2ry*t33*a2j2sy*a2j2sx)*
     & vv2rssss+t47*a2j2sx*vv2sssss+(a2j2ryy*t1*a2j2rx+a2j2ry*t58+
     & a2j2ry*t63)*vv2rrrr+(a2j2ry*t77+a2j2sy*t58+a2j2syy*t1*a2j2rx+
     & a2j2ry*t87+a2j2sy*t63+a2j2ryy*t16)*vv2rrrs+(a2j2syy*t16+a2j2ry*
     & t106+a2j2ry*t108+a2j2sy*t77+a2j2sy*t87+a2j2ryy*t25)*vv2rrss+(
     & a2j2syy*t25+a2j2sy*t108+a2j2ry*t120+a2j2ry*t123+a2j2sy*t106+
     & a2j2ryy*t33*a2j2sx)*vv2rsss+(a2j2syy*t33*a2j2sx+a2j2sy*t120+
     & a2j2sy*t123)*vv2ssss+(a2j2ry*t141+2*a2j2ryy*t54+a2j2ry*t146+
     & a2j2ryyy*a2j2ry*a2j2rx+a2j2ry*t151+a2j2ryy*t61)*vv2rrr+t188*
     & vv2rrs+t215*vv2rss+(a2j2syyy*a2j2sy*a2j2sx+2*a2j2syy*t102+
     & a2j2sy*t200+a2j2sy*t203+a2j2syy*t98+a2j2sy*t208)*vv2sss+(
     & a2j2ryyyy*a2j2rx+4*a2j2ry*a2j2rxyyy+4*a2j2ryyy*a2j2rxy+6*
     & a2j2ryy*a2j2rxyy)*vv2rr+(4*a2j2ryyy*a2j2sxy+a2j2syyyy*a2j2rx+4*
     & a2j2sy*a2j2rxyyy+6*a2j2syy*a2j2rxyy+4*a2j2ry*a2j2sxyyy+4*
     & a2j2syyy*a2j2rxy+6*a2j2ryy*a2j2sxyy+a2j2ryyyy*a2j2sx)*vv2rs+(4*
     & a2j2sy*a2j2sxyyy+a2j2syyyy*a2j2sx+4*a2j2syyy*a2j2sxy+6*a2j2syy*
     & a2j2sxyy)*vv2ss
       t1 = a2j2ry**2
       t2 = t1**2
       t8 = a2j2sy**2
       t9 = t1*a2j2ry
       t13 = t8*a2j2sy
       t17 = t8**2
       t26 = a2j2sy*a2j2ryy
       t29 = a2j2syy*t1
       t30 = a2j2syy*a2j2ry
       t31 = t26+t30
       t32 = 2*t31
       t33 = a2j2ry*t32
       t35 = a2j2sy*a2j2ry*a2j2ryy
       t37 = t29+t33+4*t35
       t41 = 3*t31
       t43 = 7*t35+t29+a2j2ry*t41+t33
       t52 = a2j2sy*t32
       t53 = t30*a2j2sy
       t55 = a2j2ryy*t8
       t56 = t52+4*t53+t55
       t60 = a2j2sy*t41+7*t53+t55+t52
       t78 = a2j2ry*a2j2ryyy
       t79 = a2j2ryy**2
       t80 = t78+t79
       t81 = 3*t80
       t87 = 4*t78+3*t79
       t89 = 2*t80
       t96 = a2j2sy*a2j2ryyy
       t98 = a2j2syyy*a2j2ry
       t100 = a2j2syy*a2j2ryy
       t102 = 2*t96+2*t98+4*t100
       t106 = 6*t100
       t109 = t106+3*t96+3*t98
       t117 = 4*t98+4*t96+t106
       t121 = 7*t30*a2j2ryy+a2j2sy*t89+a2j2ry*t102+a2j2ryy*t41+
     & a2j2syyy*t1+a2j2ry*t109+a2j2sy*t87+a2j2sy*t81+2*t96*a2j2ry+
     & a2j2ry*t117+2*a2j2ryy*t32
       t129 = a2j2syyy*a2j2sy
       t131 = a2j2syy**2
       t133 = 4*t129+3*t131
       t135 = t129+t131
       t136 = 2*t135
       t138 = 3*t135
       t145 = 7*t100*a2j2sy+a2j2sy*t109+2*a2j2syy*t32+a2j2sy*t117+
     & a2j2ry*t133+a2j2ry*t136+a2j2ry*t138+a2j2syy*t41+a2j2ryyy*t8+2*
     & t129*a2j2ry+a2j2sy*t102
       vv2yyyyy2 = t2*a2j2ry*vv2rrrrr+5*a2j2sy*t2*vv2rrrrs+10*t8*t9*
     & vv2rrrss+10*t13*t1*vv2rrsss+5*t17*a2j2ry*vv2rssss+t17*a2j2sy*
     & vv2sssss+10*t9*a2j2ryy*vv2rrrr+(12*t26*t1+a2j2ry*t37+a2j2syy*
     & t9+a2j2ry*t43)*vv2rrrs+(3*a2j2ryy*a2j2ry*t8+a2j2sy*t43+a2j2sy*
     & t37+a2j2ry*t56+a2j2ry*t60+3*t29*a2j2sy)*vv2rrss+(12*a2j2ry*t8*
     & a2j2syy+a2j2sy*t56+a2j2sy*t60+a2j2ryy*t13)*vv2rsss+10*t13*
     & a2j2syy*vv2ssss+(a2j2ryyy*t1+a2j2ry*t81+7*t79*a2j2ry+a2j2ry*
     & t87+a2j2ry*t89)*vv2rrr+t121*vv2rrs+t145*vv2rss+(a2j2sy*t138+
     & a2j2sy*t133+7*t131*a2j2sy+a2j2syyy*t8+a2j2sy*t136)*vv2sss+(5*
     & a2j2ry*a2j2ryyyy+10*a2j2ryyy*a2j2ryy)*vv2rr+(10*a2j2syy*
     & a2j2ryyy+10*a2j2syyy*a2j2ryy+5*a2j2sy*a2j2ryyyy+5*a2j2syyyy*
     & a2j2ry)*vv2rs+(10*a2j2syy*a2j2syyy+5*a2j2sy*a2j2syyyy)*vv2ss
       ! 6th derivatives, 2nd order
       t1 = a1j2sx**2
       t3 = a1j2sxxx*a1j2sx
       t4 = a1j2sxx**2
       t5 = t3+t4
       t6 = 3*t5
       t9 = a1j2sxxx*a1j2rx
       t11 = a1j2sx*a1j2rxxx
       t13 = a1j2sxx*a1j2rxx
       t14 = 6*t13
       t15 = 4*t9+4*t11+t14
       t20 = 2*t11+2*t9+4*t13
       t23 = a1j2sxx*a1j2rxxx
       t24 = 10*t23
       t25 = a1j2sxxx*a1j2rxx
       t26 = 10*t25
       t27 = a1j2sxxxx*a1j2rx
       t29 = a1j2sx*a1j2rxxxx
       t31 = t24+t26+4*t27+4*t29
       t33 = a1j2sx*a1j2rxx
       t34 = a1j2sxx*a1j2rx
       t35 = t33+t34
       t36 = 2*t35
       t43 = 6*t23+2*t29+6*t25+2*t27
       t47 = 5*t27+t24+5*t29+t26
       t51 = a1j2sxxxx*a1j2sx
       t53 = a1j2sxx*a1j2sxxx
       t55 = 2*t51+6*t53
       t59 = 2*t5
       t63 = 10*t53
       t64 = 5*t51+t63
       t66 = 3*t35
       t69 = 4*t51+t63
       t73 = t14+3*t11+3*t9
       t80 = 3*t29+9*t23+9*t25+3*t27
       t84 = 4*t3+3*t4
       t88 = 9*t53+3*t51
       t90 = a1j2rxxxx*t1+2*a1j2rxx*t6+a1j2sxx*t15+3*a1j2sxx*t20+
     & a1j2sx*t31+3*a1j2sxxx*t36+a1j2sx*t43+a1j2sx*t47+9*t23*a1j2sx+
     & a1j2rx*t55+2*t51*a1j2rx+3*a1j2rxx*t59+a1j2rx*t64+a1j2sxxx*t66+
     & a1j2rx*t69+2*a1j2sxx*t73+a1j2sx*t80+a1j2rxx*t84+a1j2rx*t88
       t92 = t1*a1j2sx
       t96 = a1j2rxx*t92
       t97 = a1j2sx*t36
       t98 = t34*a1j2sx
       t100 = a1j2rxx*t1
       t101 = t97+4*t98+t100
       t102 = a1j2sx*t101
       t103 = t34*t1
       t105 = t96+t102+6*t103
       t107 = t1**2
       t112 = a1j2sx*t66+7*t98+t100+t97
       t114 = t96+12*t103+t102+a1j2sx*t112
       t118 = a1j2rx**2
       t121 = a1j2rxxx*t118
       t122 = a1j2rxx**2
       t123 = t122*a1j2rx
       t125 = a1j2rx*a1j2rxxx
       t126 = t125+t122
       t127 = 2*t126
       t128 = a1j2rx*t127
       t129 = t121+4*t123+t128
       t131 = 7*t123
       t132 = 3*t126
       t133 = a1j2rx*t132
       t134 = t131+t128+t121+t133
       t136 = a1j2sxx*t118
       t137 = a1j2rx*t36
       t139 = a1j2sx*a1j2rx*a1j2rxx
       t141 = t136+t137+4*t139
       t148 = 7*t139+t136+a1j2rx*t66+t137
       t151 = 2*a1j2rxx*t36
       t152 = t34*a1j2rxx
       t154 = a1j2rx*t20
       t155 = a1j2sxxx*t118
       t156 = a1j2sx*t127
       t158 = 2*t11*a1j2rx
       t159 = t151+4*t152+t154+t155+t156+t158
       t161 = t118*a1j2rx
       t163 = a1j2rxx*t66
       t164 = a1j2sx*t132
       t165 = 7*t152
       t166 = a1j2rx*t73
       t167 = t163+t156+t151+t154+t158+t164+t165+t155+t166
       t172 = 4*t125+3*t122
       t174 = t155+t158+t164+t154+a1j2rx*t15+a1j2sx*t172+t165+t163+
     & t156+t166+t151
       t177 = a1j2rx*t172+t131+t128+t133+t121
       t179 = 12*t13*t118+a1j2sx*t129+a1j2sx*t134+2*a1j2rxx*t141+3*
     & t121*a1j2sx+a1j2rxx*t148+a1j2rx*t159+a1j2sxxx*t161+a1j2rx*t167+
     & a1j2rx*t174+a1j2sx*t177
       t181 = t118**2
       t190 = a1j2rxxx**2
       t204 = a1j2sxxx**2
       t215 = a1j2rxx*t118
       t216 = t215*a1j2sx
       t218 = a1j2sxx*t161
       t219 = a1j2rx*t141
       t220 = 6*t216+t218+t219
       t224 = 12*t216+t219+t218+a1j2rx*t148
       t231 = t90*uu1rss+(18*a1j2rx*t92*a1j2sxx+a1j2sx*t105+a1j2rxx*
     & t107+a1j2sx*t114)*uu1rssss+t179*uu1rrrs+15*a1j2rxx*t181*
     & uu1rrrrr+15*t181*t1*uu1rrrrss+(15*a1j2rxx*a1j2rxxxx+10*t190)*
     & uu1rr+(15*a1j2sxxxx*a1j2rxx+20*a1j2sxxx*a1j2rxxx+15*a1j2sxx*
     & a1j2rxxxx)*uu1rs+(15*a1j2sxxxx*a1j2sxx+10*t204)*uu1ss+t181*
     & t118*uu1rrrrrr+t107*t1*uu1ssssss+(18*t33*t161+a1j2sxx*t181+
     & a1j2rx*t220+a1j2rx*t224)*uu1rrrrs+15*t118*t107*uu1rrssss
       t246 = t4*a1j2sx
       t247 = 7*t246
       t248 = a1j2sx*t59
       t249 = a1j2sx*t6
       t250 = a1j2sxxx*t1
       t251 = a1j2sx*t84+t247+t248+t249+t250
       t254 = t250+4*t246+t248
       t256 = t247+t248+t249+t250
       t263 = a1j2rx*a1j2rxxxx
       t265 = a1j2rxx*a1j2rxxx
       t267 = 2*t263+6*t265
       t272 = 10*t265
       t274 = t272+5*t263
       t277 = t272+4*t263
       t293 = 9*t265+3*t263
       t299 = 3*a1j2sxx*t127+a1j2sxx*t172+a1j2sx*t267+9*t25*a1j2rx+
     & a1j2rx*t31+a1j2sx*t274+a1j2sx*t277+3*a1j2rxx*t20+3*a1j2rxxx*
     & t36+2*a1j2sxx*t132+a1j2sxxxx*t118+a1j2rxxx*t66+a1j2rx*t43+
     & a1j2rxx*t15+2*a1j2rxx*t73+a1j2sx*t293+2*t29*a1j2rx+a1j2rx*t80+
     & a1j2rx*t47
       t304 = t33*a1j2sxx
       t306 = a1j2rxxx*t1
       t307 = a1j2rx*t59
       t309 = 2*a1j2sxx*t36
       t311 = 2*t3*a1j2rx
       t312 = a1j2sx*t20
       t313 = 4*t304+t306+t307+t309+t311+t312
       t316 = 7*t304
       t319 = a1j2rx*t6
       t320 = a1j2sxx*t66
       t321 = a1j2sx*t73
       t322 = t316+a1j2rx*t84+a1j2sx*t15+t319+t306+t307+t309+t312+t320+
     & t321+t311
       t326 = t312+t316+t311+t320+t306+t319+t307+t321+t309
       t333 = a1j2rx*t251+12*t100*a1j2sxx+a1j2sx*t313+a1j2rxxx*t92+
     & a1j2sx*t322+3*t9*t1+a1j2sx*t326+2*a1j2sxx*t101+a1j2rx*t256+
     & a1j2sxx*t112+a1j2rx*t254
       t337 = a1j2rxx*a1j2rx
       t341 = 3*t337*t1
       t342 = a1j2sx*t141
       t344 = 3*t136*a1j2sx
       t345 = a1j2rx*t101
       t346 = t341+t342+t344+t345
       t352 = t341+t345+a1j2rx*t112+a1j2sx*t148+t344+t342
       t422 = a1j2sx*t167+a1j2rxx*t112+a1j2sx*t174+a1j2rx*t313+a1j2sxx*
     & t148+2*a1j2sxx*t141+a1j2rx*t322+3*t155*a1j2sx+a1j2sx*t159+
     & a1j2rx*t326+3*t125*t1+2*a1j2rxx*t101
       t424 = 20*t161*t92*uu1rrrsss+15*t107*a1j2sxx*uu1sssss+6*a1j2rx*
     & t107*a1j2sx*uu1rsssss+(a1j2sxxx*t92+12*t4*t1+a1j2sx*t251+
     & a1j2sx*t254+a1j2sx*t256)*uu1ssss+t299*uu1rrs+t333*uu1rsss+(
     & a1j2rx*t105+a1j2rx*t114+4*t337*t92+a1j2sx*t346+6*t136*t1+
     & a1j2sx*t352)*uu1rrsss+(a1j2sx*t224+6*t215*t1+a1j2sx*t220+4*
     & t218*a1j2sx+a1j2rx*t346+a1j2rx*t352)*uu1rrrss+(a1j2sxx*t84+9*
     & t3*a1j2sxx+2*a1j2sxx*t6+a1j2sx*t64+a1j2sxxxx*t1+a1j2sx*t55+
     & a1j2sx*t69+3*a1j2sxx*t59+a1j2sx*t88)*uu1sss+(a1j2rxx*t172+
     & a1j2rx*t267+a1j2rx*t274+9*t265*a1j2rx+3*a1j2rxx*t127+2*a1j2rxx*
     & t132+a1j2rx*t277+a1j2rxxxx*t118+a1j2rx*t293)*uu1rrr+6*a1j2sx*
     & t181*a1j2rx*uu1rrrrrs+(a1j2rx*t129+a1j2rx*t177+12*t122*t118+
     & a1j2rxxx*t161+a1j2rx*t134)*uu1rrrr+t422*uu1rrss
       uu1xxxxxx2 = t231+t424
       t1 = a1j2sx*a1j2rxy
       t4 = a1j2rx**2
       t5 = a1j2sxy*t4
       t6 = a1j2sxy*a1j2rx
       t8 = 2*t1+2*t6
       t10 = 4*t1*a1j2rx+t5+a1j2rx*t8
       t11 = a1j2sx*t10
       t13 = 3*t5*a1j2sx
       t15 = a1j2sx**2
       t17 = 3*a1j2rxy*a1j2rx*t15
       t20 = a1j2rxy*t15
       t22 = 4*a1j2sx*t6+t20+a1j2sx*t8
       t23 = a1j2rx*t22
       t24 = t11+t13+t17+t23
       t27 = 6*t6*t15
       t28 = a1j2sx*t15
       t29 = a1j2rxy*t28
       t30 = a1j2sx*t22
       t31 = t27+t29+t30
       t33 = a1j2syy*t4
       t36 = a1j2sxx*a1j2rx
       t38 = t36+a1j2sx*a1j2rxx
       t39 = 3*t38
       t44 = 2*t38
       t46 = a1j2sx*t39+7*a1j2sx*t36+a1j2rxx*t15+a1j2sx*t44
       t51 = a1j2sy*t46+t27+t30+t29+6*a1j2ry*t15*a1j2sxx
       t62 = 7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t4+a1j2rx*t39+t44*a1j2rx
       t65 = t13+t23+a1j2sy*t62+a1j2ry*t46+t17+t11
       t74 = 4*t28*a1j2sxy+6*a1j2sy*t15*a1j2sxx
       t84 = t15**2
       t88 = a1j2rxy**2
       t91 = a1j2rxxy*t4
       t92 = a1j2rxy*a1j2rxx
       t94 = 7*t92*a1j2rx
       t96 = a1j2rx*a1j2rxxy+t92
       t97 = 3*t96
       t98 = a1j2rx*t97
       t99 = 2*t96
       t100 = a1j2rx*t99
       t103 = a1j2rxx**2
       t105 = 4*a1j2rx*a1j2rxxx+3*t103
       t107 = t91+t94+t98+t100+a1j2ry*t105
       t109 = t94+t100+t98+t91
       t111 = t4*a1j2rx
       t113 = a1j2ryy*t4
       t116 = a1j2rxyy*t4
       t117 = a1j2rx*a1j2rxyy
       t119 = 2*t117+2*t88
       t123 = t116+a1j2rx*t119+4*a1j2rx*t88
       t134 = a1j2sxx*t8
       t136 = 5*t1*a1j2sxx
       t137 = a1j2sxy*t39
       t138 = a1j2sx*a1j2rxxy
       t139 = a1j2sxx*a1j2rxy
       t141 = a1j2sxy*a1j2rxx
       t142 = t138+t139+a1j2sxxy*a1j2rx+t141
       t143 = 3*t142
       t144 = a1j2sx*t143
       t145 = a1j2sxxy*a1j2sx
       t147 = 2*t145*a1j2rx
       t149 = a1j2sxy*a1j2sxx+t145
       t150 = 3*t149
       t151 = a1j2rx*t150
       t152 = a1j2sxy*t44
       t159 = 4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+6*a1j2sxx*a1j2rxx
       t161 = 2*t142
       t162 = a1j2sx*t161
       t163 = 2*t149
       t164 = a1j2rx*t163
       t167 = a1j2sxx**2
       t169 = 4*a1j2sxxx*a1j2sx+3*t167
       t172 = 2*t141*a1j2sx
       t173 = a1j2rxxy*t15
       t174 = t134+t136+t137+t144+t147+t151+t152+a1j2sy*t159+t162+t164+
     & a1j2ry*t169+t172+t173
       t176 = a1j2sxy*a1j2rxy
       t179 = a1j2sxyy*t4
       t181 = a1j2sx*a1j2rxyy
       t183 = a1j2sxyy*a1j2rx
       t186 = 2*t181+2*t183+4*t176
       t192 = 4*a1j2rx*t176+t179+a1j2sx*t119+a1j2rx*t186+2*a1j2rx*t181+
     & 2*a1j2rxy*t8
       t194 = a1j2sxxy*t4
       t196 = 5*t141*a1j2rx
       t199 = 2*t138*a1j2rx
       t200 = a1j2rx*t143
       t201 = a1j2rxy*t44
       t202 = a1j2rxx*t8
       t203 = t97*a1j2sx
       t204 = a1j2rx*t161
       t206 = 2*t139*a1j2rx
       t207 = a1j2rxy*t39
       t208 = a1j2sx*t99
       t210 = t194+t196+a1j2sy*t105+t199+t200+t201+t202+t203+t204+t206+
     & t207+t208+a1j2ry*t159
       t214 = a1j2sx*a1j2sxyy
       t215 = a1j2sxy**2
       t217 = 2*t214+2*t215
       t227 = a1j2rx*t217+a1j2sx*t186+4*a1j2sx*t176+a1j2rxyy*t15+2*
     & t214*a1j2rx+2*a1j2sxy*t8
       t233 = t202+t204+t206+t203+t194+t200+t199+t208+t196+t207+t201
       t236 = t151+t162+t137+t164+t147+t136+t172+t134+t144+t152+t173
       t241 = a1j2ry*t174+a1j2sx*t192+a1j2sy*t210+2*a1j2rxy*t22+a1j2rx*
     & t227+3*t179*a1j2sx+3*t117*t15+a1j2sy*t233+a1j2ryy*t46+a1j2ry*
     & t236+a1j2syy*t62+2*a1j2sxy*t10
       t259 = 2*a1j2rxy*t10+a1j2ry*t233+a1j2ry*t210+6*t33*a1j2rxx+6*
     & t176*t4+a1j2sy*t107+a1j2sx*t123+3*t116*a1j2sx+a1j2sy*t109+
     & a1j2rx*t192+a1j2sxyy*t111+a1j2ryy*t62
       t261 = a1j2ry**2
       t262 = t4**2
       t265 = a1j2sy**2
       t268 = a1j2sx*t150
       t269 = a1j2sxxy*t15
       t272 = 7*a1j2sxy*a1j2sx*a1j2sxx
       t273 = a1j2sx*t163
       t275 = t268+t269+t272+t273+a1j2sy*t169
       t287 = a1j2sxyy*t15+4*t215*a1j2sx+a1j2sx*t217
       t289 = t272+t269+t268+t273
       t295 = a1j2sxy*a1j2sxxy
       t298 = a1j2sxyy*a1j2sxx
       t300 = a1j2sxxyy*a1j2sx
       t303 = 2*t298+2*t300+4*t295
       t309 = 3*t298+3*t300+6*t295
       t312 = 4*a1j2sxxx*a1j2sxy
       t314 = 4*a1j2sxxxy*a1j2sx
       t316 = 6*a1j2sxx*a1j2sxxy
       t318 = t312+t314+t316+a1j2sy*a1j2sxxxx
       t320 = t314+t312+t316
       t328 = 5*t214*a1j2sxx+4*t295*a1j2sx+a1j2sx*t303+a1j2sxx*t217+
     & a1j2sx*t309+a1j2sy*t318+a1j2sy*t320+2*a1j2sxy*t150+a1j2sxxyy*
     & t15+2*a1j2sxy*t163+a1j2syy*t169
       t330 = a1j2ry*a1j2sy
       t336 = a1j2ry*t84+4*a1j2sy*a1j2rx*t28
       t340 = a1j2sxyy*a1j2rxx
       t342 = a1j2sxxy*a1j2rxy
       t344 = a1j2sxy*a1j2rxxy
       t346 = a1j2sx*a1j2rxxyy
       t348 = a1j2sxxyy*a1j2rx
       t350 = a1j2sxx*a1j2rxyy
       t352 = 3*t340+6*t342+6*t344+3*t346+3*t348+3*t350
       t366 = 2*t346+2*t340+4*t344+4*t342+2*t350+2*t348
       t373 = 4*a1j2sxxx*a1j2rxy
       t376 = 4*a1j2sxxxy*a1j2rx
       t378 = 4*a1j2sxy*a1j2rxxx
       t380 = 4*a1j2sx*a1j2rxxxy
       t382 = 6*a1j2sxx*a1j2rxxy
       t385 = 6*a1j2sxxy*a1j2rxx
       t386 = t373+a1j2ry*a1j2sxxxx+t376+t378+t380+t382+a1j2sy*
     & a1j2rxxxx+t385
       t389 = a1j2sx*t352+a1j2syy*t159+a1j2ry*t318+2*a1j2sxy*t161+2*
     & a1j2rxy*t150+a1j2sx*t366+a1j2ry*t320+a1j2sxyy*t44+2*t300*
     & a1j2rx+a1j2sy*t386+a1j2rxx*t217
       t392 = t376+t373+t380+t382+t385+t378
       t408 = 2*a1j2sxy*t143+a1j2sy*t392+a1j2rx*t309+5*t350*a1j2sx+
     & a1j2sxx*t186+a1j2sxyy*t39+2*a1j2rxy*t163+a1j2ryy*t169+a1j2rx*
     & t303+2*a1j2sxxy*t8+a1j2rxxyy*t15+4*t344*a1j2sx
       t411 = (a1j2sy*t24+a1j2ry*t31+6*t33*t15+a1j2ry*t51+4*a1j2ryy*
     & a1j2rx*t28+a1j2sy*t65)*uu1rrsss+(a1j2ry*t74+a1j2sy*t51+a1j2sy*
     & t31+4*a1j2ry*t28*a1j2sxy+4*a1j2syy*a1j2rx*t28+a1j2ryy*t84)*
     & uu1rssss+(6*t88*t4+a1j2ry*t107+a1j2ry*t109+a1j2rxyy*t111+6*
     & t113*a1j2rxx+a1j2rx*t123)*uu1rrrr+(a1j2sy*t74+a1j2syy*t84+4*
     & a1j2sy*t28*a1j2sxy)*uu1sssss+t241*uu1rrss+t259*uu1rrrs+t261*
     & t262*uu1rrrrrr+t265*t84*uu1ssssss+(a1j2sy*t275+6*a1j2syy*t15*
     & a1j2sxx+6*t215*t15+a1j2sxyy*t28+a1j2sx*t287+a1j2sy*t289)*
     & uu1ssss+t328*uu1sss+(t330*t84+a1j2sy*t336)*uu1rsssss+(t389+
     & t408)*uu1rss
       t413 = a1j2ry*t111
       t418 = a1j2ry*t4
       t421 = 4*t111*a1j2rxy+6*t418*a1j2rxx
       t425 = a1j2sxxy**2
       t439 = 4*t413*a1j2sx+a1j2sy*t262
       t445 = a1j2sy*t111
       t450 = 4*t445*a1j2sx+6*t418*t15
       t457 = a1j2sy*t4
       t460 = 4*a1j2ry*a1j2rx*t28+6*t457*t15
       t470 = 4*a1j2rxxx*a1j2rxy
       t472 = 6*a1j2rxxy*a1j2rxx
       t474 = 4*a1j2rxxxy*a1j2rx
       t475 = a1j2ry*a1j2rxxxx+t470+t472+t474
       t487 = t474+t470+t472
       t489 = a1j2ry*t392+5*t340*a1j2rx+a1j2sy*t475+a1j2rx*t352+2*
     & a1j2rxxy*t8+a1j2rxyy*t44+a1j2rxx*t186+a1j2sxx*t119+2*a1j2sxy*
     & t97+2*a1j2rxy*t161+a1j2sy*t487
       t495 = a1j2rx*a1j2rxxyy
       t497 = a1j2rxx*a1j2rxyy
       t499 = a1j2rxy*a1j2rxxy
       t501 = 3*t495+3*t497+6*t499
       t510 = 2*t495+4*t499+2*t497
       t517 = a1j2ryy*t159+a1j2syy*t105+a1j2ry*t386+2*a1j2sxy*t99+
     & a1j2sx*t501+a1j2sxxyy*t4+4*t342*a1j2rx+a1j2rx*t366+a1j2sx*t510+
     & 2*t346*a1j2rx+2*a1j2rxy*t143+a1j2rxyy*t39
       t553 = 5*t497*a1j2rx+a1j2ry*t475+a1j2rx*t510+a1j2ry*t487+a1j2rx*
     & t501+a1j2rxxyy*t4+2*a1j2rxy*t97+4*t499*a1j2rx+a1j2ryy*t105+2*
     & a1j2rxy*t99+a1j2rxx*t119
       t564 = a1j2rx*t10
       t566 = 6*t1*t4
       t567 = a1j2sxy*t111
       t569 = 6*t457*a1j2rxx+t564+t566+t567+a1j2ry*t62
       t571 = t567+t566+t564
       t607 = a1j2rxyy*t28+3*t183*t15+a1j2ry*t289+a1j2sy*t174+a1j2rx*
     & t287+a1j2sy*t236+6*a1j2ryy*t15*a1j2sxx+2*a1j2sxy*t22+a1j2syy*
     & t46+a1j2sx*t227+6*t20*a1j2sxy+a1j2ry*t275
       t616 = a1j2rxxy**2
       t620 = (a1j2ryy*t262+4*t413*a1j2rxy+a1j2ry*t421)*uu1rrrrr+(6*
     & t425+8*a1j2sxxxy*a1j2sxy+a1j2syy*a1j2sxxxx+4*a1j2sxyy*a1j2sxxx+
     & 6*a1j2sxxyy*a1j2sxx)*uu1ss+(a1j2ry*t439+t330*t262)*uu1rrrrrs+(
     & a1j2sy*t439+a1j2ry*t450)*uu1rrrrss+(a1j2ry*t460+a1j2sy*t450)*
     & uu1rrrsss+(t489+t517)*uu1rrs+(8*a1j2sxxxy*a1j2rxy+4*a1j2sxxx*
     & a1j2rxyy+4*a1j2sxyy*a1j2rxxx+6*a1j2sxxyy*a1j2rxx+12*a1j2sxxy*
     & a1j2rxxy+a1j2syy*a1j2rxxxx+6*a1j2sxx*a1j2rxxyy+a1j2ryy*
     & a1j2sxxxx+8*a1j2sxy*a1j2rxxxy)*uu1rs+t553*uu1rrr+(a1j2ry*t336+
     & a1j2sy*t460)*uu1rrssss+(4*t445*a1j2rxy+a1j2syy*t262+a1j2ry*
     & t569+a1j2ry*t571+a1j2sy*t421+4*a1j2ryy*t111*a1j2sx)*uu1rrrrs+(
     & a1j2sy*t569+a1j2ry*t65+4*a1j2syy*t111*a1j2sx+a1j2ry*t24+a1j2sy*
     & t571+6*t113*t15)*uu1rrrss+t607*uu1rsss+(8*a1j2rxy*a1j2rxxxy+4*
     & a1j2rxyy*a1j2rxxx+6*a1j2rxx*a1j2rxxyy+a1j2ryy*a1j2rxxxx+6*t616)
     & *uu1rr
       uu1xxxxyy2 = t411+t620
       t1 = a1j2sy**2
       t2 = t1**2
       t3 = a1j2sx**2
       t6 = a1j2ry**2
       t7 = t6**2
       t8 = a1j2rx**2
       t13 = 2*a1j2sy*a1j2sxy*a1j2sx
       t17 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
       t19 = a1j2syy*t3
       t20 = t13+a1j2sy*t17+t19
       t23 = a1j2syy*a1j2sy*t3
       t24 = t19+t13
       t25 = a1j2sy*t24
       t26 = a1j2sy*t20+t23+t25
       t30 = t23+t25
       t34 = a1j2sx*a1j2rxy
       t35 = a1j2sxy*a1j2rx
       t37 = 2*t34+2*t35
       t38 = a1j2sy*t37
       t41 = 2*a1j2syy*a1j2sx*a1j2rx
       t44 = 2*a1j2ry*a1j2sxy*a1j2sx
       t45 = a1j2ryy*t3
       t46 = t38+t41+t44+t45
       t49 = a1j2syyy*t3
       t52 = 4*a1j2syy*a1j2sxy*a1j2sx
       t53 = a1j2sx*a1j2sxyy
       t54 = a1j2sxy**2
       t56 = 2*t53+2*t54
       t57 = a1j2sy*t56
       t58 = t49+t52+t57
       t64 = a1j2ry*t3+2*a1j2sy*a1j2sx*a1j2rx
       t66 = a1j2sy*a1j2sxxy
       t67 = 2*t53
       t68 = a1j2syy*a1j2sxx
       t69 = 2*t54
       t70 = t66+t67+t68+t69
       t71 = a1j2sy*t70
       t72 = a1j2syy*t17
       t73 = t52+t71+t72+t57+t49
       t76 = t69+2*t66+t68+t67
       t78 = t71+t49+t52+t72+t57+a1j2sy*t76
       t82 = a1j2ry*t56
       t85 = 2*a1j2syyy*a1j2sx*a1j2rx
       t86 = a1j2ryyy*t3
       t88 = 2*a1j2syy*t37
       t91 = 4*a1j2ryy*a1j2sxy*a1j2sx
       t93 = 2*a1j2sx*a1j2rxyy
       t95 = 2*a1j2sxyy*a1j2rx
       t97 = 4*a1j2sxy*a1j2rxy
       t98 = t93+t95+t97
       t99 = a1j2sy*t98
       t100 = t82+t85+t86+t88+t91+t99
       t106 = a1j2ry*a1j2sxx+2*t34+2*t35+a1j2sy*a1j2rxx
       t109 = a1j2sy*t106+a1j2ry*t17+t45+t44+t41+t38
       t114 = a1j2sy*a1j2rxxy
       t115 = a1j2ry*a1j2sxxy
       t116 = a1j2ryy*a1j2sxx
       t117 = a1j2syy*a1j2rxx
       t118 = t114+t115+t116+t97+t95+t117+t93
       t119 = a1j2sy*t118
       t120 = a1j2ryy*t17
       t121 = a1j2syy*t106
       t122 = a1j2ry*t70
       t123 = t119+t91+t120+t85+t121+t82+t88+t99+t122+t86
       t128 = t97+2*t115+t116+2*t114+t95+t117+t93
       t130 = t121+a1j2ry*t76+a1j2sy*t128+t86+t119+t91+t99+t122+t85+
     & t88+t120+t82
       t132 = 2*a1j2syy*t46+a1j2ry*t58+a1j2syyy*t64+a1j2ry*t73+a1j2ry*
     & t78+2*a1j2ryy*t24+a1j2sy*t100+a1j2syy*t109+a1j2ryy*t20+
     & a1j2ryyy*a1j2sy*t3+a1j2sy*t123+a1j2sy*t130
       t134 = a1j2ryyy*a1j2rxx
       t135 = a1j2ry*a1j2rxxyy
       t138 = 6*a1j2rxy*a1j2rxyy
       t140 = 2*a1j2rx*a1j2rxyyy
       t141 = a1j2ryy*a1j2rxxy
       t142 = 3*t141
       t143 = t134+3*t135+t138+t140+t142
       t146 = t140+t138+2*t135+t134+t142
       t149 = t140+t138
       t151 = a1j2rx*a1j2rxyy
       t152 = 2*t151
       t153 = a1j2ryy*a1j2rxx
       t154 = a1j2ry*a1j2rxxy
       t156 = a1j2rxy**2
       t157 = 2*t156
       t158 = t152+t153+2*t154+t157
       t161 = 2*t151+2*t156
       t164 = t153+t152+t154+t157
       t171 = t134+t138+t135+t140+2*t141
       t176 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
       t184 = 2*a1j2ry*a1j2rxy*a1j2rx
       t186 = a1j2ryy*t8
       t187 = t184+a1j2ry*t176+t186
       t189 = a1j2ry*t161
       t190 = a1j2ryyy*t8
       t193 = 4*a1j2ryy*a1j2rxy*a1j2rx
       t194 = t189+t190+t193
       t196 = a1j2ry*t164
       t197 = a1j2ryy*t176
       t198 = t193+t189+t196+t190+t197
       t200 = t184+t186
       t204 = t193+t196+t190+a1j2ry*t158+t197+t189
       t209 = a1j2ry*a1j2sy
       t211 = a1j2sy*t64+t209*t3
       t215 = a1j2sy*t211+a1j2ry*t1*t3
       t226 = a1j2sy*t8+2*a1j2ry*a1j2sx*a1j2rx
       t228 = a1j2syy*t8
       t229 = a1j2ry*t37
       t232 = 2*a1j2ryy*a1j2sx*a1j2rx
       t235 = 2*a1j2sy*a1j2rxy*a1j2rx
       t236 = t228+t229+t232+t235
       t241 = 2*a1j2ryy*t37
       t242 = a1j2syy*t176
       t245 = 4*a1j2syy*a1j2rxy*a1j2rx
       t246 = a1j2ry*t118
       t247 = a1j2sy*t161
       t248 = a1j2sy*t164
       t251 = 2*a1j2ryyy*a1j2sx*a1j2rx
       t252 = a1j2ry*t98
       t253 = a1j2ryy*t106
       t254 = a1j2syyy*t8
       t255 = t241+t242+t245+t246+t247+t248+t251+t252+t253+t254
       t261 = a1j2sy*t158+t251+t242+t241+t254+a1j2ry*t128+t245+t248+
     & t253+t247+t246+t252
       t263 = t251+t254+t245+t241+t247+t252
       t269 = a1j2sy*t176+t235+t232+t228+a1j2ry*t106+t229
       t274 = a1j2ryyy*t226+2*a1j2ryy*t236+a1j2sy*t204+a1j2ry*t255+2*
     & a1j2syy*t200+a1j2ry*t261+a1j2ry*t263+a1j2sy*t194+a1j2sy*t198+
     & a1j2ryy*t269+a1j2syyy*a1j2ry*t8+a1j2syy*t187
       t288 = 2*a1j2sxyyy*a1j2rx
       t289 = a1j2syy*a1j2rxxy
       t290 = 3*t289
       t292 = 6*a1j2sxy*a1j2rxyy
       t293 = a1j2ry*a1j2sxxyy
       t295 = a1j2ryyy*a1j2sxx
       t296 = a1j2ryy*a1j2sxxy
       t297 = 3*t296
       t299 = 6*a1j2sxyy*a1j2rxy
       t301 = 2*a1j2sx*a1j2rxyyy
       t302 = a1j2syyy*a1j2rxx
       t303 = a1j2sy*a1j2rxxyy
       t305 = t288+t290+t292+2*t293+t295+t297+t299+t301+t302+2*t303
       t310 = t295+t303+t301+t288+t299+t292+2*t289+t293+2*t296+t302
       t321 = t295+3*t293+t290+t299+t302+t301+3*t303+t297+t292+t288
       t329 = t292+t301+t299+t288
       t331 = a1j2sy*t149+3*a1j2syy*t161+a1j2sy*t146+a1j2syyyy*t8+6*
     & a1j2syyy*a1j2rxy*a1j2rx+2*a1j2syy*t164+a1j2sy*t171+a1j2ry*t305+
     & a1j2sy*t143+a1j2ry*t310+a1j2syyy*t176+2*a1j2ryyyy*a1j2sx*
     & a1j2rx+a1j2syy*t158+3*a1j2ryy*t98+a1j2ry*t321+3*a1j2ryyy*t37+
     & a1j2ryy*t128+a1j2ryyy*t106+2*a1j2ryy*t118+a1j2ry*t329
       t335 = a1j2syy*t64
       t336 = a1j2sy*t46
       t337 = a1j2ry*t24
       t339 = a1j2ryy*a1j2sy*t3
       t340 = t335+t336+t337+t339
       t344 = t337+t336+t335+t339+a1j2sy*t109+a1j2ry*t20
       t383 = a1j2syyy*t226+a1j2sy*t261+a1j2ry*t100+a1j2ry*t123+a1j2sy*
     & t255+a1j2syy*t269+2*a1j2syy*t236+a1j2ryy*t109+a1j2sy*t263+
     & a1j2ryyy*t64+a1j2ry*t130+2*a1j2ryy*t46
       t385 = t2*t3*uu1ssssss+t7*t8*uu1rrrrrr+(a1j2sy*t26+a1j2syy*t1*
     & t3+a1j2sy*t30)*uu1sssss+t132*uu1rsss+(a1j2ry*t143+a1j2ry*t146+
     & a1j2ryyyy*t8+a1j2ry*t149+a1j2ryy*t158+3*a1j2ryy*t161+2*a1j2ryy*
     & t164+6*a1j2ryyy*a1j2rxy*a1j2rx+a1j2ry*t171+a1j2ryyy*t176)*
     & uu1rrr+(a1j2ryyy*a1j2ry*t8+a1j2ryy*t187+a1j2ry*t194+a1j2ry*
     & t198+2*a1j2ryy*t200+a1j2ry*t204)*uu1rrrr+(a1j2sy*t215+a1j2ry*
     & t1*a1j2sy*t3)*uu1rsssss+t274*uu1rrrs+t331*uu1rrs+(a1j2ryy*t1*
     & t3+a1j2sy*t340+a1j2sy*t344+a1j2syy*t211+a1j2ry*t30+a1j2ry*t26)*
     & uu1rssss+(8*a1j2sxy*a1j2rxyyy+12*a1j2sxyy*a1j2rxyy+a1j2syyyy*
     & a1j2rxx+8*a1j2sxyyy*a1j2rxy+6*a1j2syy*a1j2rxxyy+4*a1j2syyy*
     & a1j2rxxy+a1j2ryyyy*a1j2sxx+6*a1j2ryy*a1j2sxxyy+4*a1j2ryyy*
     & a1j2sxxy)*uu1rs+t383*uu1rrss
       t390 = a1j2ryy*a1j2ry*t8
       t391 = a1j2ry*t200
       t392 = a1j2ry*t187+t390+t391
       t394 = t391+t390
       t398 = a1j2sxyy**2
       t410 = a1j2ry*t46
       t411 = a1j2syy*t226
       t413 = a1j2sy*t236
       t415 = a1j2ryy*t64
       t416 = t410+t411+a1j2ry*t109+t413+a1j2sy*t269+t415
       t420 = a1j2sy*t226+a1j2ry*t64
       t423 = t413+t411+t415+t410
       t433 = t209*t8+a1j2ry*t226
       t437 = a1j2ry*t433+a1j2sy*t6*t8
       t461 = a1j2syy*a1j2sxxy
       t462 = 3*t461
       t463 = a1j2sy*a1j2sxxyy
       t466 = 2*a1j2sx*a1j2sxyyy
       t468 = 6*a1j2sxyy*a1j2sxy
       t469 = a1j2syyy*a1j2sxx
       t470 = t462+2*t463+t466+t468+t469
       t477 = t466+t468
       t482 = t468+2*t461+t463+t469+t466
       t491 = t462+t466+t469+3*t463+t468
       t495 = 3*a1j2syy*t98+3*a1j2syyy*t37+a1j2sy*t305+2*a1j2syyyy*
     & a1j2sx*a1j2rx+a1j2syyy*t106+a1j2sy*t310+a1j2ry*t470+2*a1j2syy*
     & t118+6*a1j2ryyy*a1j2sxy*a1j2sx+a1j2ry*t477+a1j2ryyyy*t3+
     & a1j2syy*t128+a1j2ry*t482+a1j2sy*t329+2*a1j2ryy*t70+a1j2sy*t321+
     & 3*a1j2ryy*t56+a1j2ry*t491+a1j2ryyy*t17+a1j2ryy*t76
       t499 = a1j2ry*t236
       t501 = a1j2ry*t8*a1j2syy
       t502 = a1j2ryy*t226
       t503 = a1j2sy*t200
       t504 = a1j2sy*t187+a1j2ry*t269+t499+t501+t502+t503
       t506 = t501+t499+t503+t502
       t517 = a1j2ry*t420+a1j2sy*t433
       t533 = a1j2rxyy**2
       t560 = a1j2sy*t420+a1j2ry*t211
       t568 = (a1j2ryy*t6*t8+a1j2ry*t392+a1j2ry*t394)*uu1rrrrr+(6*t398+
     & a1j2syyyy*a1j2sxx+8*a1j2sxy*a1j2sxyyy+6*a1j2syy*a1j2sxxyy+4*
     & a1j2syyy*a1j2sxxy)*uu1ss+(a1j2ry*t340+a1j2sy*t416+a1j2syy*t420+
     & a1j2ry*t344+a1j2sy*t423+a1j2ryy*t211)*uu1rrsss+(a1j2sy*t6*
     & a1j2ry*t8+a1j2ry*t437)*uu1rrrrrs+(a1j2sy*t58+a1j2syy*t20+
     & a1j2sy*t78+a1j2syyy*a1j2sy*t3+a1j2sy*t73+2*a1j2syy*t24)*
     & uu1ssss+t495*uu1rss+(a1j2ry*t504+a1j2ry*t506+a1j2ryy*t433+
     & a1j2sy*t392+a1j2syy*t6*t8+a1j2sy*t394)*uu1rrrrs+(a1j2ry*t517+
     & a1j2sy*t437)*uu1rrrrss+(a1j2ry*t423+a1j2ryy*t420+a1j2ry*t416+
     & a1j2sy*t506+a1j2sy*t504+a1j2syy*t433)*uu1rrrss+(a1j2ryyyy*
     & a1j2rxx+4*a1j2ryyy*a1j2rxxy+6*t533+8*a1j2rxyyy*a1j2rxy+6*
     & a1j2ryy*a1j2rxxyy)*uu1rr+(a1j2sy*t491+a1j2sy*t470+a1j2sy*t482+
     & a1j2sy*t477+3*a1j2syy*t56+6*a1j2syyy*a1j2sxy*a1j2sx+a1j2syyyy*
     & t3+a1j2syy*t76+a1j2syyy*t17+2*a1j2syy*t70)*uu1sss+(a1j2ry*t215+
     & a1j2sy*t560)*uu1rrssss+(a1j2sy*t517+a1j2ry*t560)*uu1rrrsss
       uu1xxyyyy2 = t385+t568
       t1 = a1j2sy**2
       t2 = t1**2
       t7 = a1j2syyy*a1j2sy
       t8 = a1j2syy**2
       t9 = t7+t8
       t10 = 2*t9
       t17 = 4*t7+3*t8
       t19 = a1j2syy*a1j2syyy
       t20 = 10*t19
       t21 = a1j2sy*a1j2syyyy
       t23 = t20+4*t21
       t27 = 2*t21+6*t19
       t30 = t20+5*t21
       t34 = 9*t19+3*t21
       t37 = 3*t9
       t42 = a1j2ry**2
       t43 = t42**2
       t51 = t42*a1j2ry
       t56 = a1j2syy*t42
       t57 = a1j2sy*a1j2ryy
       t58 = a1j2syy*a1j2ry
       t59 = t57+t58
       t60 = 2*t59
       t61 = a1j2ry*t60
       t63 = a1j2sy*a1j2ry*a1j2ryy
       t65 = t56+t61+4*t63
       t66 = a1j2ry*t65
       t67 = a1j2syy*t51
       t68 = t57*t42
       t70 = t66+t67+6*t68
       t74 = 3*t59
       t76 = 7*t63+t56+a1j2ry*t74+t61
       t78 = 12*t68+t66+t67+a1j2ry*t76
       t82 = a1j2syy*a1j2ryyy
       t83 = 10*t82
       t84 = a1j2syyy*a1j2ryy
       t85 = 10*t84
       t86 = a1j2sy*a1j2ryyyy
       t88 = a1j2syyyy*a1j2ry
       t90 = t83+t85+5*t86+5*t88
       t94 = t83+4*t86+t85+4*t88
       t96 = a1j2ry*a1j2ryyy
       t97 = a1j2ryy**2
       t98 = t96+t97
       t99 = 2*t98
       t104 = a1j2syyy*a1j2ry
       t106 = a1j2sy*a1j2ryyy
       t108 = a1j2syy*a1j2ryy
       t109 = 6*t108
       t110 = 4*t104+4*t106+t109
       t116 = 6*t82+2*t86+6*t84+2*t88
       t122 = 9*t84+3*t86+9*t82+3*t88
       t124 = a1j2ry*a1j2ryyyy
       t126 = a1j2ryyy*a1j2ryy
       t128 = 2*t124+6*t126
       t133 = 4*t96+3*t97
       t137 = t109+3*t106+3*t104
       t141 = 10*t126
       t142 = 5*t124+t141
       t148 = t141+4*t124
       t152 = 3*t98
       t158 = 2*t106+2*t104+4*t108
       t163 = 9*t126+3*t124
       t165 = a1j2ry*t90+a1j2ry*t94+3*a1j2syy*t99+9*t84*a1j2ry+a1j2ryy*
     & t110+a1j2ry*t116+a1j2ry*t122+a1j2sy*t128+a1j2ryyy*t74+a1j2syy*
     & t133+2*a1j2ryy*t137+a1j2sy*t142+3*a1j2ryyy*t60+a1j2syyyy*t42+
     & a1j2sy*t148+2*a1j2ry*t86+2*a1j2syy*t152+3*a1j2ryy*t158+a1j2sy*
     & t163
       t167 = t1*a1j2sy
       t178 = a1j2sy*t60
       t179 = t58*a1j2sy
       t181 = a1j2ryy*t1
       t182 = t178+4*t179+t181
       t183 = a1j2ry*t182
       t184 = a1j2ryy*a1j2ry
       t186 = 3*t184*t1
       t187 = a1j2sy*t65
       t189 = 3*a1j2sy*t56
       t190 = t183+t186+t187+t189
       t195 = a1j2sy*t74+7*t179+t181+t178
       t197 = t186+a1j2sy*t76+t187+t183+a1j2ry*t195+t189
       t203 = a1j2sy*t10
       t204 = t8*a1j2sy
       t206 = a1j2syyy*t1
       t207 = t203+4*t204+t206
       t210 = a1j2sy*t37
       t212 = 7*t204
       t213 = t210+a1j2sy*t17+t212+t206+t203
       t215 = t212+t206+t203+t210
       t245 = 2*a1j2ryy*t37+a1j2sy*t94+3*a1j2ryy*t10+a1j2ryyyy*t1+2*
     & t21*a1j2ry+a1j2ryy*t17+9*t106*a1j2syy+a1j2ry*t27+a1j2sy*t116+3*
     & a1j2syy*t158+a1j2syy*t110+a1j2ry*t23+a1j2sy*t90+3*a1j2syyy*t60+
     & a1j2syyy*t74+2*a1j2syy*t137+a1j2ry*t34+a1j2sy*t122+a1j2ry*t30
       t249 = a1j2ry*t1*a1j2syy
       t251 = a1j2sy*t182
       t253 = a1j2ryy*t167
       t254 = 12*t249+t251+a1j2sy*t195+t253
       t262 = 6*t249+t253+t251
       t266 = 6*t2*a1j2sy*a1j2ry*uu1rsssss+(3*a1j2syy*t10+9*t7*a1j2syy+
     & a1j2syy*t17+a1j2sy*t23+a1j2sy*t27+a1j2sy*t30+a1j2sy*t34+
     & a1j2syyyy*t1+2*a1j2syy*t37)*uu1sss+t43*t42*uu1rrrrrr+15*
     & a1j2syy*t2*uu1sssss+t2*t1*uu1ssssss+(18*a1j2ryy*t51*a1j2sy+
     & a1j2syy*t43+a1j2ry*t70+a1j2ry*t78)*uu1rrrrs+t165*uu1rrs+20*
     & t167*t51*uu1rrrsss+(6*a1j2ryy*t42*t1+a1j2sy*t78+a1j2sy*t70+4*
     & t67*a1j2sy+a1j2ry*t190+a1j2ry*t197)*uu1rrrss+(12*t8*t1+a1j2sy*
     & t207+a1j2syyy*t167+a1j2sy*t213+a1j2sy*t215)*uu1ssss+t245*
     & uu1rss+(a1j2sy*t190+a1j2ry*t254+a1j2sy*t197+6*t56*t1+4*t184*
     & t167+a1j2ry*t262)*uu1rrsss
       t269 = a1j2ryyy*t42
       t270 = a1j2ry*t152
       t271 = t97*a1j2ry
       t272 = 7*t271
       t274 = a1j2ry*t99
       t275 = t269+t270+t272+a1j2ry*t133+t274
       t280 = 2*t106*a1j2ry
       t281 = a1j2syyy*t42
       t282 = t58*a1j2ryy
       t285 = 2*a1j2ryy*t60
       t286 = a1j2sy*t99
       t287 = a1j2ry*t158
       t288 = t280+t281+4*t282+t285+t286+t287
       t291 = t274+t269+4*t271
       t293 = 7*t282
       t294 = a1j2ryy*t74
       t295 = a1j2ry*t137
       t297 = a1j2sy*t152
       t299 = t293+t286+t287+t294+t281+t295+a1j2sy*t133+t297+t280+
     & a1j2ry*t110+t285
       t302 = t280+t287+t295+t293+t286+t281+t285+t294+t297
       t306 = t272+t269+t270+t274
       t309 = 12*t108*t42+a1j2sy*t275+2*a1j2ryy*t65+a1j2ry*t288+a1j2sy*
     & t291+a1j2ry*t299+a1j2syyy*t51+a1j2ry*t302+3*t269*a1j2sy+a1j2sy*
     & t306+a1j2ryy*t76
       t313 = a1j2ryyy**2
       t319 = a1j2syyy**2
       t333 = a1j2ry*t10
       t335 = 2*t7*a1j2ry
       t336 = a1j2ryyy*t1
       t338 = 2*a1j2syy*t60
       t339 = a1j2sy*t108
       t341 = a1j2sy*t158
       t342 = t333+t335+t336+t338+4*t339+t341
       t344 = a1j2ry*t37
       t345 = a1j2syy*t74
       t346 = 7*t339
       t347 = a1j2sy*t137
       t348 = t335+t344+t345+t333+t338+t341+t346+t336+t347
       t362 = t346+t347+t338+a1j2sy*t110+a1j2ry*t17+t333+t344+t345+
     & t336+t335+t341
       t365 = 3*t281*a1j2sy+a1j2ry*t342+a1j2ry*t348+a1j2ryy*t195+
     & a1j2sy*t302+a1j2sy*t288+2*a1j2syy*t65+2*a1j2ryy*t182+a1j2sy*
     & t299+3*t96*t1+a1j2ry*t362+a1j2syy*t76
       t389 = a1j2sy*t342+a1j2ry*t207+12*t181*a1j2syy+a1j2ryyy*t167+3*
     & t104*t1+a1j2sy*t362+a1j2ry*t213+a1j2syy*t195+a1j2sy*t348+
     & a1j2ry*t215+2*a1j2syy*t182
       t426 = t309*uu1rrrs+(15*a1j2ryy*a1j2ryyyy+10*t313)*uu1rr+(15*
     & a1j2syy*a1j2syyyy+10*t319)*uu1ss+(15*a1j2syy*a1j2ryyyy+20*
     & a1j2syyy*a1j2ryyy+15*a1j2syyyy*a1j2ryy)*uu1rs+t365*uu1rrss+(
     & a1j2sy*t254+18*a1j2ry*t167*a1j2syy+a1j2sy*t262+a1j2ryy*t2)*
     & uu1rssss+t389*uu1rsss+(9*t126*a1j2ry+a1j2ry*t148+a1j2ry*t128+
     & a1j2ry*t142+a1j2ryyyy*t42+2*a1j2ryy*t152+3*a1j2ryy*t99+a1j2ryy*
     & t133+a1j2ry*t163)*uu1rrr+15*a1j2ryy*t43*uu1rrrrr+15*t2*t42*
     & uu1rrssss+(a1j2ryyy*t51+a1j2ry*t291+a1j2ry*t275+12*t97*t42+
     & a1j2ry*t306)*uu1rrrr+6*a1j2sy*t43*a1j2ry*uu1rrrrrs+15*t1*t43*
     & uu1rrrrss
       uu1yyyyyy2 = t266+t426
       t1 = a1j2sx**2
       t3 = a1j2sxxx*a1j2sx
       t4 = a1j2sxx**2
       t5 = t3+t4
       t6 = 3*t5
       t9 = a1j2sxxx*a1j2rx
       t11 = a1j2sx*a1j2rxxx
       t13 = a1j2sxx*a1j2rxx
       t14 = 6*t13
       t15 = 4*t9+4*t11+t14
       t20 = 2*t11+2*t9+4*t13
       t23 = a1j2sxx*a1j2rxxx
       t24 = 10*t23
       t25 = a1j2sxxx*a1j2rxx
       t26 = 10*t25
       t27 = a1j2sxxxx*a1j2rx
       t29 = a1j2sx*a1j2rxxxx
       t31 = t24+t26+4*t27+4*t29
       t33 = a1j2sx*a1j2rxx
       t34 = a1j2sxx*a1j2rx
       t35 = t33+t34
       t36 = 2*t35
       t43 = 6*t23+2*t29+6*t25+2*t27
       t47 = 5*t27+t24+5*t29+t26
       t51 = a1j2sxxxx*a1j2sx
       t53 = a1j2sxx*a1j2sxxx
       t55 = 2*t51+6*t53
       t59 = 2*t5
       t63 = 10*t53
       t64 = 5*t51+t63
       t66 = 3*t35
       t69 = 4*t51+t63
       t73 = t14+3*t11+3*t9
       t80 = 3*t29+9*t23+9*t25+3*t27
       t84 = 4*t3+3*t4
       t88 = 9*t53+3*t51
       t90 = a1j2rxxxx*t1+2*a1j2rxx*t6+a1j2sxx*t15+3*a1j2sxx*t20+
     & a1j2sx*t31+3*a1j2sxxx*t36+a1j2sx*t43+a1j2sx*t47+9*t23*a1j2sx+
     & a1j2rx*t55+2*t51*a1j2rx+3*a1j2rxx*t59+a1j2rx*t64+a1j2sxxx*t66+
     & a1j2rx*t69+2*a1j2sxx*t73+a1j2sx*t80+a1j2rxx*t84+a1j2rx*t88
       t92 = t1*a1j2sx
       t96 = a1j2rxx*t92
       t97 = a1j2sx*t36
       t98 = t34*a1j2sx
       t100 = a1j2rxx*t1
       t101 = t97+4*t98+t100
       t102 = a1j2sx*t101
       t103 = t34*t1
       t105 = t96+t102+6*t103
       t107 = t1**2
       t112 = a1j2sx*t66+7*t98+t100+t97
       t114 = t96+12*t103+t102+a1j2sx*t112
       t118 = a1j2rx**2
       t121 = a1j2rxxx*t118
       t122 = a1j2rxx**2
       t123 = t122*a1j2rx
       t125 = a1j2rx*a1j2rxxx
       t126 = t125+t122
       t127 = 2*t126
       t128 = a1j2rx*t127
       t129 = t121+4*t123+t128
       t131 = 7*t123
       t132 = 3*t126
       t133 = a1j2rx*t132
       t134 = t131+t128+t121+t133
       t136 = a1j2sxx*t118
       t137 = a1j2rx*t36
       t139 = a1j2sx*a1j2rx*a1j2rxx
       t141 = t136+t137+4*t139
       t148 = 7*t139+t136+a1j2rx*t66+t137
       t151 = 2*a1j2rxx*t36
       t152 = t34*a1j2rxx
       t154 = a1j2rx*t20
       t155 = a1j2sxxx*t118
       t156 = a1j2sx*t127
       t158 = 2*t11*a1j2rx
       t159 = t151+4*t152+t154+t155+t156+t158
       t161 = t118*a1j2rx
       t163 = a1j2rxx*t66
       t164 = a1j2sx*t132
       t165 = 7*t152
       t166 = a1j2rx*t73
       t167 = t163+t156+t151+t154+t158+t164+t165+t155+t166
       t172 = 4*t125+3*t122
       t174 = t155+t158+t164+t154+a1j2rx*t15+a1j2sx*t172+t165+t163+
     & t156+t166+t151
       t177 = a1j2rx*t172+t131+t128+t133+t121
       t179 = 12*t13*t118+a1j2sx*t129+a1j2sx*t134+2*a1j2rxx*t141+3*
     & t121*a1j2sx+a1j2rxx*t148+a1j2rx*t159+a1j2sxxx*t161+a1j2rx*t167+
     & a1j2rx*t174+a1j2sx*t177
       t181 = t118**2
       t190 = a1j2rxxx**2
       t204 = a1j2sxxx**2
       t215 = a1j2rxx*t118
       t216 = t215*a1j2sx
       t218 = a1j2sxx*t161
       t219 = a1j2rx*t141
       t220 = 6*t216+t218+t219
       t224 = 12*t216+t219+t218+a1j2rx*t148
       t231 = t90*vv1rss+(18*a1j2rx*t92*a1j2sxx+a1j2sx*t105+a1j2rxx*
     & t107+a1j2sx*t114)*vv1rssss+t179*vv1rrrs+15*a1j2rxx*t181*
     & vv1rrrrr+15*t181*t1*vv1rrrrss+(15*a1j2rxx*a1j2rxxxx+10*t190)*
     & vv1rr+(15*a1j2sxxxx*a1j2rxx+20*a1j2sxxx*a1j2rxxx+15*a1j2sxx*
     & a1j2rxxxx)*vv1rs+(15*a1j2sxxxx*a1j2sxx+10*t204)*vv1ss+t181*
     & t118*vv1rrrrrr+t107*t1*vv1ssssss+(18*t33*t161+a1j2sxx*t181+
     & a1j2rx*t220+a1j2rx*t224)*vv1rrrrs+15*t118*t107*vv1rrssss
       t246 = t4*a1j2sx
       t247 = 7*t246
       t248 = a1j2sx*t59
       t249 = a1j2sx*t6
       t250 = a1j2sxxx*t1
       t251 = a1j2sx*t84+t247+t248+t249+t250
       t254 = t250+4*t246+t248
       t256 = t247+t248+t249+t250
       t263 = a1j2rx*a1j2rxxxx
       t265 = a1j2rxx*a1j2rxxx
       t267 = 2*t263+6*t265
       t272 = 10*t265
       t274 = t272+5*t263
       t277 = t272+4*t263
       t293 = 9*t265+3*t263
       t299 = 3*a1j2sxx*t127+a1j2sxx*t172+a1j2sx*t267+9*t25*a1j2rx+
     & a1j2rx*t31+a1j2sx*t274+a1j2sx*t277+3*a1j2rxx*t20+3*a1j2rxxx*
     & t36+2*a1j2sxx*t132+a1j2sxxxx*t118+a1j2rxxx*t66+a1j2rx*t43+
     & a1j2rxx*t15+2*a1j2rxx*t73+a1j2sx*t293+2*t29*a1j2rx+a1j2rx*t80+
     & a1j2rx*t47
       t304 = t33*a1j2sxx
       t306 = a1j2rxxx*t1
       t307 = a1j2rx*t59
       t309 = 2*a1j2sxx*t36
       t311 = 2*t3*a1j2rx
       t312 = a1j2sx*t20
       t313 = 4*t304+t306+t307+t309+t311+t312
       t316 = 7*t304
       t319 = a1j2rx*t6
       t320 = a1j2sxx*t66
       t321 = a1j2sx*t73
       t322 = t316+a1j2rx*t84+a1j2sx*t15+t319+t306+t307+t309+t312+t320+
     & t321+t311
       t326 = t312+t316+t311+t320+t306+t319+t307+t321+t309
       t333 = a1j2rx*t251+12*t100*a1j2sxx+a1j2sx*t313+a1j2rxxx*t92+
     & a1j2sx*t322+3*t9*t1+a1j2sx*t326+2*a1j2sxx*t101+a1j2rx*t256+
     & a1j2sxx*t112+a1j2rx*t254
       t337 = a1j2rxx*a1j2rx
       t341 = 3*t337*t1
       t342 = a1j2sx*t141
       t344 = 3*t136*a1j2sx
       t345 = a1j2rx*t101
       t346 = t341+t342+t344+t345
       t352 = t341+t345+a1j2rx*t112+a1j2sx*t148+t344+t342
       t422 = a1j2sx*t167+a1j2rxx*t112+a1j2sx*t174+a1j2rx*t313+a1j2sxx*
     & t148+2*a1j2sxx*t141+a1j2rx*t322+3*t155*a1j2sx+a1j2sx*t159+
     & a1j2rx*t326+3*t125*t1+2*a1j2rxx*t101
       t424 = 20*t161*t92*vv1rrrsss+15*t107*a1j2sxx*vv1sssss+6*a1j2rx*
     & t107*a1j2sx*vv1rsssss+(a1j2sxxx*t92+12*t4*t1+a1j2sx*t251+
     & a1j2sx*t254+a1j2sx*t256)*vv1ssss+t299*vv1rrs+t333*vv1rsss+(
     & a1j2rx*t105+a1j2rx*t114+4*t337*t92+a1j2sx*t346+6*t136*t1+
     & a1j2sx*t352)*vv1rrsss+(a1j2sx*t224+6*t215*t1+a1j2sx*t220+4*
     & t218*a1j2sx+a1j2rx*t346+a1j2rx*t352)*vv1rrrss+(a1j2sxx*t84+9*
     & t3*a1j2sxx+2*a1j2sxx*t6+a1j2sx*t64+a1j2sxxxx*t1+a1j2sx*t55+
     & a1j2sx*t69+3*a1j2sxx*t59+a1j2sx*t88)*vv1sss+(a1j2rxx*t172+
     & a1j2rx*t267+a1j2rx*t274+9*t265*a1j2rx+3*a1j2rxx*t127+2*a1j2rxx*
     & t132+a1j2rx*t277+a1j2rxxxx*t118+a1j2rx*t293)*vv1rrr+6*a1j2sx*
     & t181*a1j2rx*vv1rrrrrs+(a1j2rx*t129+a1j2rx*t177+12*t122*t118+
     & a1j2rxxx*t161+a1j2rx*t134)*vv1rrrr+t422*vv1rrss
       vv1xxxxxx2 = t231+t424
       t1 = a1j2sx*a1j2rxy
       t4 = a1j2rx**2
       t5 = a1j2sxy*t4
       t6 = a1j2sxy*a1j2rx
       t8 = 2*t1+2*t6
       t10 = 4*t1*a1j2rx+t5+a1j2rx*t8
       t11 = a1j2sx*t10
       t13 = 3*t5*a1j2sx
       t15 = a1j2sx**2
       t17 = 3*a1j2rxy*a1j2rx*t15
       t20 = a1j2rxy*t15
       t22 = 4*a1j2sx*t6+t20+a1j2sx*t8
       t23 = a1j2rx*t22
       t24 = t11+t13+t17+t23
       t27 = 6*t6*t15
       t28 = a1j2sx*t15
       t29 = a1j2rxy*t28
       t30 = a1j2sx*t22
       t31 = t27+t29+t30
       t33 = a1j2syy*t4
       t36 = a1j2sxx*a1j2rx
       t38 = t36+a1j2sx*a1j2rxx
       t39 = 3*t38
       t44 = 2*t38
       t46 = a1j2sx*t39+7*a1j2sx*t36+a1j2rxx*t15+a1j2sx*t44
       t51 = a1j2sy*t46+t27+t30+t29+6*a1j2ry*t15*a1j2sxx
       t62 = 7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t4+a1j2rx*t39+t44*a1j2rx
       t65 = t13+t23+a1j2sy*t62+a1j2ry*t46+t17+t11
       t74 = 4*t28*a1j2sxy+6*a1j2sy*t15*a1j2sxx
       t84 = t15**2
       t88 = a1j2rxy**2
       t91 = a1j2rxxy*t4
       t92 = a1j2rxy*a1j2rxx
       t94 = 7*t92*a1j2rx
       t96 = a1j2rx*a1j2rxxy+t92
       t97 = 3*t96
       t98 = a1j2rx*t97
       t99 = 2*t96
       t100 = a1j2rx*t99
       t103 = a1j2rxx**2
       t105 = 4*a1j2rx*a1j2rxxx+3*t103
       t107 = t91+t94+t98+t100+a1j2ry*t105
       t109 = t94+t100+t98+t91
       t111 = t4*a1j2rx
       t113 = a1j2ryy*t4
       t116 = a1j2rxyy*t4
       t117 = a1j2rx*a1j2rxyy
       t119 = 2*t117+2*t88
       t123 = t116+a1j2rx*t119+4*a1j2rx*t88
       t134 = a1j2sxx*t8
       t136 = 5*t1*a1j2sxx
       t137 = a1j2sxy*t39
       t138 = a1j2sx*a1j2rxxy
       t139 = a1j2sxx*a1j2rxy
       t141 = a1j2sxy*a1j2rxx
       t142 = t138+t139+a1j2sxxy*a1j2rx+t141
       t143 = 3*t142
       t144 = a1j2sx*t143
       t145 = a1j2sxxy*a1j2sx
       t147 = 2*t145*a1j2rx
       t149 = a1j2sxy*a1j2sxx+t145
       t150 = 3*t149
       t151 = a1j2rx*t150
       t152 = a1j2sxy*t44
       t159 = 4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+6*a1j2sxx*a1j2rxx
       t161 = 2*t142
       t162 = a1j2sx*t161
       t163 = 2*t149
       t164 = a1j2rx*t163
       t167 = a1j2sxx**2
       t169 = 4*a1j2sxxx*a1j2sx+3*t167
       t172 = 2*t141*a1j2sx
       t173 = a1j2rxxy*t15
       t174 = t134+t136+t137+t144+t147+t151+t152+a1j2sy*t159+t162+t164+
     & a1j2ry*t169+t172+t173
       t176 = a1j2sxy*a1j2rxy
       t179 = a1j2sxyy*t4
       t181 = a1j2sx*a1j2rxyy
       t183 = a1j2sxyy*a1j2rx
       t186 = 2*t181+2*t183+4*t176
       t192 = 4*a1j2rx*t176+t179+a1j2sx*t119+a1j2rx*t186+2*a1j2rx*t181+
     & 2*a1j2rxy*t8
       t194 = a1j2sxxy*t4
       t196 = 5*t141*a1j2rx
       t199 = 2*t138*a1j2rx
       t200 = a1j2rx*t143
       t201 = a1j2rxy*t44
       t202 = a1j2rxx*t8
       t203 = t97*a1j2sx
       t204 = a1j2rx*t161
       t206 = 2*t139*a1j2rx
       t207 = a1j2rxy*t39
       t208 = a1j2sx*t99
       t210 = t194+t196+a1j2sy*t105+t199+t200+t201+t202+t203+t204+t206+
     & t207+t208+a1j2ry*t159
       t214 = a1j2sx*a1j2sxyy
       t215 = a1j2sxy**2
       t217 = 2*t214+2*t215
       t227 = a1j2rx*t217+a1j2sx*t186+4*a1j2sx*t176+a1j2rxyy*t15+2*
     & t214*a1j2rx+2*a1j2sxy*t8
       t233 = t202+t204+t206+t203+t194+t200+t199+t208+t196+t207+t201
       t236 = t151+t162+t137+t164+t147+t136+t172+t134+t144+t152+t173
       t241 = a1j2ry*t174+a1j2sx*t192+a1j2sy*t210+2*a1j2rxy*t22+a1j2rx*
     & t227+3*t179*a1j2sx+3*t117*t15+a1j2sy*t233+a1j2ryy*t46+a1j2ry*
     & t236+a1j2syy*t62+2*a1j2sxy*t10
       t259 = 2*a1j2rxy*t10+a1j2ry*t233+a1j2ry*t210+6*t33*a1j2rxx+6*
     & t176*t4+a1j2sy*t107+a1j2sx*t123+3*t116*a1j2sx+a1j2sy*t109+
     & a1j2rx*t192+a1j2sxyy*t111+a1j2ryy*t62
       t261 = a1j2ry**2
       t262 = t4**2
       t265 = a1j2sy**2
       t268 = a1j2sx*t150
       t269 = a1j2sxxy*t15
       t272 = 7*a1j2sxy*a1j2sx*a1j2sxx
       t273 = a1j2sx*t163
       t275 = t268+t269+t272+t273+a1j2sy*t169
       t287 = a1j2sxyy*t15+4*t215*a1j2sx+a1j2sx*t217
       t289 = t272+t269+t268+t273
       t295 = a1j2sxy*a1j2sxxy
       t298 = a1j2sxyy*a1j2sxx
       t300 = a1j2sxxyy*a1j2sx
       t303 = 2*t298+2*t300+4*t295
       t309 = 3*t298+3*t300+6*t295
       t312 = 4*a1j2sxxx*a1j2sxy
       t314 = 4*a1j2sxxxy*a1j2sx
       t316 = 6*a1j2sxx*a1j2sxxy
       t318 = t312+t314+t316+a1j2sy*a1j2sxxxx
       t320 = t314+t312+t316
       t328 = 5*t214*a1j2sxx+4*t295*a1j2sx+a1j2sx*t303+a1j2sxx*t217+
     & a1j2sx*t309+a1j2sy*t318+a1j2sy*t320+2*a1j2sxy*t150+a1j2sxxyy*
     & t15+2*a1j2sxy*t163+a1j2syy*t169
       t330 = a1j2ry*a1j2sy
       t336 = a1j2ry*t84+4*a1j2sy*a1j2rx*t28
       t340 = a1j2sxyy*a1j2rxx
       t342 = a1j2sxxy*a1j2rxy
       t344 = a1j2sxy*a1j2rxxy
       t346 = a1j2sx*a1j2rxxyy
       t348 = a1j2sxxyy*a1j2rx
       t350 = a1j2sxx*a1j2rxyy
       t352 = 3*t340+6*t342+6*t344+3*t346+3*t348+3*t350
       t366 = 2*t346+2*t340+4*t344+4*t342+2*t350+2*t348
       t373 = 4*a1j2sxxx*a1j2rxy
       t376 = 4*a1j2sxxxy*a1j2rx
       t378 = 4*a1j2sxy*a1j2rxxx
       t380 = 4*a1j2sx*a1j2rxxxy
       t382 = 6*a1j2sxx*a1j2rxxy
       t385 = 6*a1j2sxxy*a1j2rxx
       t386 = t373+a1j2ry*a1j2sxxxx+t376+t378+t380+t382+a1j2sy*
     & a1j2rxxxx+t385
       t389 = a1j2sx*t352+a1j2syy*t159+a1j2ry*t318+2*a1j2sxy*t161+2*
     & a1j2rxy*t150+a1j2sx*t366+a1j2ry*t320+a1j2sxyy*t44+2*t300*
     & a1j2rx+a1j2sy*t386+a1j2rxx*t217
       t392 = t376+t373+t380+t382+t385+t378
       t408 = 2*a1j2sxy*t143+a1j2sy*t392+a1j2rx*t309+5*t350*a1j2sx+
     & a1j2sxx*t186+a1j2sxyy*t39+2*a1j2rxy*t163+a1j2ryy*t169+a1j2rx*
     & t303+2*a1j2sxxy*t8+a1j2rxxyy*t15+4*t344*a1j2sx
       t411 = (a1j2sy*t24+a1j2ry*t31+6*t33*t15+a1j2ry*t51+4*a1j2ryy*
     & a1j2rx*t28+a1j2sy*t65)*vv1rrsss+(a1j2ry*t74+a1j2sy*t51+a1j2sy*
     & t31+4*a1j2ry*t28*a1j2sxy+4*a1j2syy*a1j2rx*t28+a1j2ryy*t84)*
     & vv1rssss+(6*t88*t4+a1j2ry*t107+a1j2ry*t109+a1j2rxyy*t111+6*
     & t113*a1j2rxx+a1j2rx*t123)*vv1rrrr+(a1j2sy*t74+a1j2syy*t84+4*
     & a1j2sy*t28*a1j2sxy)*vv1sssss+t241*vv1rrss+t259*vv1rrrs+t261*
     & t262*vv1rrrrrr+t265*t84*vv1ssssss+(a1j2sy*t275+6*a1j2syy*t15*
     & a1j2sxx+6*t215*t15+a1j2sxyy*t28+a1j2sx*t287+a1j2sy*t289)*
     & vv1ssss+t328*vv1sss+(t330*t84+a1j2sy*t336)*vv1rsssss+(t389+
     & t408)*vv1rss
       t413 = a1j2ry*t111
       t418 = a1j2ry*t4
       t421 = 4*t111*a1j2rxy+6*t418*a1j2rxx
       t425 = a1j2sxxy**2
       t439 = 4*t413*a1j2sx+a1j2sy*t262
       t445 = a1j2sy*t111
       t450 = 4*t445*a1j2sx+6*t418*t15
       t457 = a1j2sy*t4
       t460 = 4*a1j2ry*a1j2rx*t28+6*t457*t15
       t470 = 4*a1j2rxxx*a1j2rxy
       t472 = 6*a1j2rxxy*a1j2rxx
       t474 = 4*a1j2rxxxy*a1j2rx
       t475 = a1j2ry*a1j2rxxxx+t470+t472+t474
       t487 = t474+t470+t472
       t489 = a1j2ry*t392+5*t340*a1j2rx+a1j2sy*t475+a1j2rx*t352+2*
     & a1j2rxxy*t8+a1j2rxyy*t44+a1j2rxx*t186+a1j2sxx*t119+2*a1j2sxy*
     & t97+2*a1j2rxy*t161+a1j2sy*t487
       t495 = a1j2rx*a1j2rxxyy
       t497 = a1j2rxx*a1j2rxyy
       t499 = a1j2rxy*a1j2rxxy
       t501 = 3*t495+3*t497+6*t499
       t510 = 2*t495+4*t499+2*t497
       t517 = a1j2ryy*t159+a1j2syy*t105+a1j2ry*t386+2*a1j2sxy*t99+
     & a1j2sx*t501+a1j2sxxyy*t4+4*t342*a1j2rx+a1j2rx*t366+a1j2sx*t510+
     & 2*t346*a1j2rx+2*a1j2rxy*t143+a1j2rxyy*t39
       t553 = 5*t497*a1j2rx+a1j2ry*t475+a1j2rx*t510+a1j2ry*t487+a1j2rx*
     & t501+a1j2rxxyy*t4+2*a1j2rxy*t97+4*t499*a1j2rx+a1j2ryy*t105+2*
     & a1j2rxy*t99+a1j2rxx*t119
       t564 = a1j2rx*t10
       t566 = 6*t1*t4
       t567 = a1j2sxy*t111
       t569 = 6*t457*a1j2rxx+t564+t566+t567+a1j2ry*t62
       t571 = t567+t566+t564
       t607 = a1j2rxyy*t28+3*t183*t15+a1j2ry*t289+a1j2sy*t174+a1j2rx*
     & t287+a1j2sy*t236+6*a1j2ryy*t15*a1j2sxx+2*a1j2sxy*t22+a1j2syy*
     & t46+a1j2sx*t227+6*t20*a1j2sxy+a1j2ry*t275
       t616 = a1j2rxxy**2
       t620 = (a1j2ryy*t262+4*t413*a1j2rxy+a1j2ry*t421)*vv1rrrrr+(6*
     & t425+8*a1j2sxxxy*a1j2sxy+a1j2syy*a1j2sxxxx+4*a1j2sxyy*a1j2sxxx+
     & 6*a1j2sxxyy*a1j2sxx)*vv1ss+(a1j2ry*t439+t330*t262)*vv1rrrrrs+(
     & a1j2sy*t439+a1j2ry*t450)*vv1rrrrss+(a1j2ry*t460+a1j2sy*t450)*
     & vv1rrrsss+(t489+t517)*vv1rrs+(8*a1j2sxxxy*a1j2rxy+4*a1j2sxxx*
     & a1j2rxyy+4*a1j2sxyy*a1j2rxxx+6*a1j2sxxyy*a1j2rxx+12*a1j2sxxy*
     & a1j2rxxy+a1j2syy*a1j2rxxxx+6*a1j2sxx*a1j2rxxyy+a1j2ryy*
     & a1j2sxxxx+8*a1j2sxy*a1j2rxxxy)*vv1rs+t553*vv1rrr+(a1j2ry*t336+
     & a1j2sy*t460)*vv1rrssss+(4*t445*a1j2rxy+a1j2syy*t262+a1j2ry*
     & t569+a1j2ry*t571+a1j2sy*t421+4*a1j2ryy*t111*a1j2sx)*vv1rrrrs+(
     & a1j2sy*t569+a1j2ry*t65+4*a1j2syy*t111*a1j2sx+a1j2ry*t24+a1j2sy*
     & t571+6*t113*t15)*vv1rrrss+t607*vv1rsss+(8*a1j2rxy*a1j2rxxxy+4*
     & a1j2rxyy*a1j2rxxx+6*a1j2rxx*a1j2rxxyy+a1j2ryy*a1j2rxxxx+6*t616)
     & *vv1rr
       vv1xxxxyy2 = t411+t620
       t1 = a1j2sy**2
       t2 = t1**2
       t3 = a1j2sx**2
       t6 = a1j2ry**2
       t7 = t6**2
       t8 = a1j2rx**2
       t13 = 2*a1j2sy*a1j2sxy*a1j2sx
       t17 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
       t19 = a1j2syy*t3
       t20 = t13+a1j2sy*t17+t19
       t23 = a1j2syy*a1j2sy*t3
       t24 = t19+t13
       t25 = a1j2sy*t24
       t26 = a1j2sy*t20+t23+t25
       t30 = t23+t25
       t34 = a1j2sx*a1j2rxy
       t35 = a1j2sxy*a1j2rx
       t37 = 2*t34+2*t35
       t38 = a1j2sy*t37
       t41 = 2*a1j2syy*a1j2sx*a1j2rx
       t44 = 2*a1j2ry*a1j2sxy*a1j2sx
       t45 = a1j2ryy*t3
       t46 = t38+t41+t44+t45
       t49 = a1j2syyy*t3
       t52 = 4*a1j2syy*a1j2sxy*a1j2sx
       t53 = a1j2sx*a1j2sxyy
       t54 = a1j2sxy**2
       t56 = 2*t53+2*t54
       t57 = a1j2sy*t56
       t58 = t49+t52+t57
       t64 = a1j2ry*t3+2*a1j2sy*a1j2sx*a1j2rx
       t66 = a1j2sy*a1j2sxxy
       t67 = 2*t53
       t68 = a1j2syy*a1j2sxx
       t69 = 2*t54
       t70 = t66+t67+t68+t69
       t71 = a1j2sy*t70
       t72 = a1j2syy*t17
       t73 = t52+t71+t72+t57+t49
       t76 = t69+2*t66+t68+t67
       t78 = t71+t49+t52+t72+t57+a1j2sy*t76
       t82 = a1j2ry*t56
       t85 = 2*a1j2syyy*a1j2sx*a1j2rx
       t86 = a1j2ryyy*t3
       t88 = 2*a1j2syy*t37
       t91 = 4*a1j2ryy*a1j2sxy*a1j2sx
       t93 = 2*a1j2sx*a1j2rxyy
       t95 = 2*a1j2sxyy*a1j2rx
       t97 = 4*a1j2sxy*a1j2rxy
       t98 = t93+t95+t97
       t99 = a1j2sy*t98
       t100 = t82+t85+t86+t88+t91+t99
       t106 = a1j2ry*a1j2sxx+2*t34+2*t35+a1j2sy*a1j2rxx
       t109 = a1j2sy*t106+a1j2ry*t17+t45+t44+t41+t38
       t114 = a1j2sy*a1j2rxxy
       t115 = a1j2ry*a1j2sxxy
       t116 = a1j2ryy*a1j2sxx
       t117 = a1j2syy*a1j2rxx
       t118 = t114+t115+t116+t97+t95+t117+t93
       t119 = a1j2sy*t118
       t120 = a1j2ryy*t17
       t121 = a1j2syy*t106
       t122 = a1j2ry*t70
       t123 = t119+t91+t120+t85+t121+t82+t88+t99+t122+t86
       t128 = t97+2*t115+t116+2*t114+t95+t117+t93
       t130 = t121+a1j2ry*t76+a1j2sy*t128+t86+t119+t91+t99+t122+t85+
     & t88+t120+t82
       t132 = 2*a1j2syy*t46+a1j2ry*t58+a1j2syyy*t64+a1j2ry*t73+a1j2ry*
     & t78+2*a1j2ryy*t24+a1j2sy*t100+a1j2syy*t109+a1j2ryy*t20+
     & a1j2ryyy*a1j2sy*t3+a1j2sy*t123+a1j2sy*t130
       t134 = a1j2ryyy*a1j2rxx
       t135 = a1j2ry*a1j2rxxyy
       t138 = 6*a1j2rxy*a1j2rxyy
       t140 = 2*a1j2rx*a1j2rxyyy
       t141 = a1j2ryy*a1j2rxxy
       t142 = 3*t141
       t143 = t134+3*t135+t138+t140+t142
       t146 = t140+t138+2*t135+t134+t142
       t149 = t140+t138
       t151 = a1j2rx*a1j2rxyy
       t152 = 2*t151
       t153 = a1j2ryy*a1j2rxx
       t154 = a1j2ry*a1j2rxxy
       t156 = a1j2rxy**2
       t157 = 2*t156
       t158 = t152+t153+2*t154+t157
       t161 = 2*t151+2*t156
       t164 = t153+t152+t154+t157
       t171 = t134+t138+t135+t140+2*t141
       t176 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
       t184 = 2*a1j2ry*a1j2rxy*a1j2rx
       t186 = a1j2ryy*t8
       t187 = t184+a1j2ry*t176+t186
       t189 = a1j2ry*t161
       t190 = a1j2ryyy*t8
       t193 = 4*a1j2ryy*a1j2rxy*a1j2rx
       t194 = t189+t190+t193
       t196 = a1j2ry*t164
       t197 = a1j2ryy*t176
       t198 = t193+t189+t196+t190+t197
       t200 = t184+t186
       t204 = t193+t196+t190+a1j2ry*t158+t197+t189
       t209 = a1j2ry*a1j2sy
       t211 = a1j2sy*t64+t209*t3
       t215 = a1j2sy*t211+a1j2ry*t1*t3
       t226 = a1j2sy*t8+2*a1j2ry*a1j2sx*a1j2rx
       t228 = a1j2syy*t8
       t229 = a1j2ry*t37
       t232 = 2*a1j2ryy*a1j2sx*a1j2rx
       t235 = 2*a1j2sy*a1j2rxy*a1j2rx
       t236 = t228+t229+t232+t235
       t241 = 2*a1j2ryy*t37
       t242 = a1j2syy*t176
       t245 = 4*a1j2syy*a1j2rxy*a1j2rx
       t246 = a1j2ry*t118
       t247 = a1j2sy*t161
       t248 = a1j2sy*t164
       t251 = 2*a1j2ryyy*a1j2sx*a1j2rx
       t252 = a1j2ry*t98
       t253 = a1j2ryy*t106
       t254 = a1j2syyy*t8
       t255 = t241+t242+t245+t246+t247+t248+t251+t252+t253+t254
       t261 = a1j2sy*t158+t251+t242+t241+t254+a1j2ry*t128+t245+t248+
     & t253+t247+t246+t252
       t263 = t251+t254+t245+t241+t247+t252
       t269 = a1j2sy*t176+t235+t232+t228+a1j2ry*t106+t229
       t274 = a1j2ryyy*t226+2*a1j2ryy*t236+a1j2sy*t204+a1j2ry*t255+2*
     & a1j2syy*t200+a1j2ry*t261+a1j2ry*t263+a1j2sy*t194+a1j2sy*t198+
     & a1j2ryy*t269+a1j2syyy*a1j2ry*t8+a1j2syy*t187
       t288 = 2*a1j2sxyyy*a1j2rx
       t289 = a1j2syy*a1j2rxxy
       t290 = 3*t289
       t292 = 6*a1j2sxy*a1j2rxyy
       t293 = a1j2ry*a1j2sxxyy
       t295 = a1j2ryyy*a1j2sxx
       t296 = a1j2ryy*a1j2sxxy
       t297 = 3*t296
       t299 = 6*a1j2sxyy*a1j2rxy
       t301 = 2*a1j2sx*a1j2rxyyy
       t302 = a1j2syyy*a1j2rxx
       t303 = a1j2sy*a1j2rxxyy
       t305 = t288+t290+t292+2*t293+t295+t297+t299+t301+t302+2*t303
       t310 = t295+t303+t301+t288+t299+t292+2*t289+t293+2*t296+t302
       t321 = t295+3*t293+t290+t299+t302+t301+3*t303+t297+t292+t288
       t329 = t292+t301+t299+t288
       t331 = a1j2sy*t149+3*a1j2syy*t161+a1j2sy*t146+a1j2syyyy*t8+6*
     & a1j2syyy*a1j2rxy*a1j2rx+2*a1j2syy*t164+a1j2sy*t171+a1j2ry*t305+
     & a1j2sy*t143+a1j2ry*t310+a1j2syyy*t176+2*a1j2ryyyy*a1j2sx*
     & a1j2rx+a1j2syy*t158+3*a1j2ryy*t98+a1j2ry*t321+3*a1j2ryyy*t37+
     & a1j2ryy*t128+a1j2ryyy*t106+2*a1j2ryy*t118+a1j2ry*t329
       t335 = a1j2syy*t64
       t336 = a1j2sy*t46
       t337 = a1j2ry*t24
       t339 = a1j2ryy*a1j2sy*t3
       t340 = t335+t336+t337+t339
       t344 = t337+t336+t335+t339+a1j2sy*t109+a1j2ry*t20
       t383 = a1j2syyy*t226+a1j2sy*t261+a1j2ry*t100+a1j2ry*t123+a1j2sy*
     & t255+a1j2syy*t269+2*a1j2syy*t236+a1j2ryy*t109+a1j2sy*t263+
     & a1j2ryyy*t64+a1j2ry*t130+2*a1j2ryy*t46
       t385 = t2*t3*vv1ssssss+t7*t8*vv1rrrrrr+(a1j2sy*t26+a1j2syy*t1*
     & t3+a1j2sy*t30)*vv1sssss+t132*vv1rsss+(a1j2ry*t143+a1j2ry*t146+
     & a1j2ryyyy*t8+a1j2ry*t149+a1j2ryy*t158+3*a1j2ryy*t161+2*a1j2ryy*
     & t164+6*a1j2ryyy*a1j2rxy*a1j2rx+a1j2ry*t171+a1j2ryyy*t176)*
     & vv1rrr+(a1j2ryyy*a1j2ry*t8+a1j2ryy*t187+a1j2ry*t194+a1j2ry*
     & t198+2*a1j2ryy*t200+a1j2ry*t204)*vv1rrrr+(a1j2sy*t215+a1j2ry*
     & t1*a1j2sy*t3)*vv1rsssss+t274*vv1rrrs+t331*vv1rrs+(a1j2ryy*t1*
     & t3+a1j2sy*t340+a1j2sy*t344+a1j2syy*t211+a1j2ry*t30+a1j2ry*t26)*
     & vv1rssss+(8*a1j2sxy*a1j2rxyyy+12*a1j2sxyy*a1j2rxyy+a1j2syyyy*
     & a1j2rxx+8*a1j2sxyyy*a1j2rxy+6*a1j2syy*a1j2rxxyy+4*a1j2syyy*
     & a1j2rxxy+a1j2ryyyy*a1j2sxx+6*a1j2ryy*a1j2sxxyy+4*a1j2ryyy*
     & a1j2sxxy)*vv1rs+t383*vv1rrss
       t390 = a1j2ryy*a1j2ry*t8
       t391 = a1j2ry*t200
       t392 = a1j2ry*t187+t390+t391
       t394 = t391+t390
       t398 = a1j2sxyy**2
       t410 = a1j2ry*t46
       t411 = a1j2syy*t226
       t413 = a1j2sy*t236
       t415 = a1j2ryy*t64
       t416 = t410+t411+a1j2ry*t109+t413+a1j2sy*t269+t415
       t420 = a1j2sy*t226+a1j2ry*t64
       t423 = t413+t411+t415+t410
       t433 = t209*t8+a1j2ry*t226
       t437 = a1j2ry*t433+a1j2sy*t6*t8
       t461 = a1j2syy*a1j2sxxy
       t462 = 3*t461
       t463 = a1j2sy*a1j2sxxyy
       t466 = 2*a1j2sx*a1j2sxyyy
       t468 = 6*a1j2sxyy*a1j2sxy
       t469 = a1j2syyy*a1j2sxx
       t470 = t462+2*t463+t466+t468+t469
       t477 = t466+t468
       t482 = t468+2*t461+t463+t469+t466
       t491 = t462+t466+t469+3*t463+t468
       t495 = 3*a1j2syy*t98+3*a1j2syyy*t37+a1j2sy*t305+2*a1j2syyyy*
     & a1j2sx*a1j2rx+a1j2syyy*t106+a1j2sy*t310+a1j2ry*t470+2*a1j2syy*
     & t118+6*a1j2ryyy*a1j2sxy*a1j2sx+a1j2ry*t477+a1j2ryyyy*t3+
     & a1j2syy*t128+a1j2ry*t482+a1j2sy*t329+2*a1j2ryy*t70+a1j2sy*t321+
     & 3*a1j2ryy*t56+a1j2ry*t491+a1j2ryyy*t17+a1j2ryy*t76
       t499 = a1j2ry*t236
       t501 = a1j2ry*t8*a1j2syy
       t502 = a1j2ryy*t226
       t503 = a1j2sy*t200
       t504 = a1j2sy*t187+a1j2ry*t269+t499+t501+t502+t503
       t506 = t501+t499+t503+t502
       t517 = a1j2ry*t420+a1j2sy*t433
       t533 = a1j2rxyy**2
       t560 = a1j2sy*t420+a1j2ry*t211
       t568 = (a1j2ryy*t6*t8+a1j2ry*t392+a1j2ry*t394)*vv1rrrrr+(6*t398+
     & a1j2syyyy*a1j2sxx+8*a1j2sxy*a1j2sxyyy+6*a1j2syy*a1j2sxxyy+4*
     & a1j2syyy*a1j2sxxy)*vv1ss+(a1j2ry*t340+a1j2sy*t416+a1j2syy*t420+
     & a1j2ry*t344+a1j2sy*t423+a1j2ryy*t211)*vv1rrsss+(a1j2sy*t6*
     & a1j2ry*t8+a1j2ry*t437)*vv1rrrrrs+(a1j2sy*t58+a1j2syy*t20+
     & a1j2sy*t78+a1j2syyy*a1j2sy*t3+a1j2sy*t73+2*a1j2syy*t24)*
     & vv1ssss+t495*vv1rss+(a1j2ry*t504+a1j2ry*t506+a1j2ryy*t433+
     & a1j2sy*t392+a1j2syy*t6*t8+a1j2sy*t394)*vv1rrrrs+(a1j2ry*t517+
     & a1j2sy*t437)*vv1rrrrss+(a1j2ry*t423+a1j2ryy*t420+a1j2ry*t416+
     & a1j2sy*t506+a1j2sy*t504+a1j2syy*t433)*vv1rrrss+(a1j2ryyyy*
     & a1j2rxx+4*a1j2ryyy*a1j2rxxy+6*t533+8*a1j2rxyyy*a1j2rxy+6*
     & a1j2ryy*a1j2rxxyy)*vv1rr+(a1j2sy*t491+a1j2sy*t470+a1j2sy*t482+
     & a1j2sy*t477+3*a1j2syy*t56+6*a1j2syyy*a1j2sxy*a1j2sx+a1j2syyyy*
     & t3+a1j2syy*t76+a1j2syyy*t17+2*a1j2syy*t70)*vv1sss+(a1j2ry*t215+
     & a1j2sy*t560)*vv1rrssss+(a1j2sy*t517+a1j2ry*t560)*vv1rrrsss
       vv1xxyyyy2 = t385+t568
       t1 = a1j2sy**2
       t2 = t1**2
       t7 = a1j2syyy*a1j2sy
       t8 = a1j2syy**2
       t9 = t7+t8
       t10 = 2*t9
       t17 = 4*t7+3*t8
       t19 = a1j2syy*a1j2syyy
       t20 = 10*t19
       t21 = a1j2sy*a1j2syyyy
       t23 = t20+4*t21
       t27 = 2*t21+6*t19
       t30 = t20+5*t21
       t34 = 9*t19+3*t21
       t37 = 3*t9
       t42 = a1j2ry**2
       t43 = t42**2
       t51 = t42*a1j2ry
       t56 = a1j2syy*t42
       t57 = a1j2sy*a1j2ryy
       t58 = a1j2syy*a1j2ry
       t59 = t57+t58
       t60 = 2*t59
       t61 = a1j2ry*t60
       t63 = a1j2sy*a1j2ry*a1j2ryy
       t65 = t56+t61+4*t63
       t66 = a1j2ry*t65
       t67 = a1j2syy*t51
       t68 = t57*t42
       t70 = t66+t67+6*t68
       t74 = 3*t59
       t76 = 7*t63+t56+a1j2ry*t74+t61
       t78 = 12*t68+t66+t67+a1j2ry*t76
       t82 = a1j2syy*a1j2ryyy
       t83 = 10*t82
       t84 = a1j2syyy*a1j2ryy
       t85 = 10*t84
       t86 = a1j2sy*a1j2ryyyy
       t88 = a1j2syyyy*a1j2ry
       t90 = t83+t85+5*t86+5*t88
       t94 = t83+4*t86+t85+4*t88
       t96 = a1j2ry*a1j2ryyy
       t97 = a1j2ryy**2
       t98 = t96+t97
       t99 = 2*t98
       t104 = a1j2syyy*a1j2ry
       t106 = a1j2sy*a1j2ryyy
       t108 = a1j2syy*a1j2ryy
       t109 = 6*t108
       t110 = 4*t104+4*t106+t109
       t116 = 6*t82+2*t86+6*t84+2*t88
       t122 = 9*t84+3*t86+9*t82+3*t88
       t124 = a1j2ry*a1j2ryyyy
       t126 = a1j2ryyy*a1j2ryy
       t128 = 2*t124+6*t126
       t133 = 4*t96+3*t97
       t137 = t109+3*t106+3*t104
       t141 = 10*t126
       t142 = 5*t124+t141
       t148 = t141+4*t124
       t152 = 3*t98
       t158 = 2*t106+2*t104+4*t108
       t163 = 9*t126+3*t124
       t165 = a1j2ry*t90+a1j2ry*t94+3*a1j2syy*t99+9*t84*a1j2ry+a1j2ryy*
     & t110+a1j2ry*t116+a1j2ry*t122+a1j2sy*t128+a1j2ryyy*t74+a1j2syy*
     & t133+2*a1j2ryy*t137+a1j2sy*t142+3*a1j2ryyy*t60+a1j2syyyy*t42+
     & a1j2sy*t148+2*a1j2ry*t86+2*a1j2syy*t152+3*a1j2ryy*t158+a1j2sy*
     & t163
       t167 = t1*a1j2sy
       t178 = a1j2sy*t60
       t179 = t58*a1j2sy
       t181 = a1j2ryy*t1
       t182 = t178+4*t179+t181
       t183 = a1j2ry*t182
       t184 = a1j2ryy*a1j2ry
       t186 = 3*t184*t1
       t187 = a1j2sy*t65
       t189 = 3*a1j2sy*t56
       t190 = t183+t186+t187+t189
       t195 = a1j2sy*t74+7*t179+t181+t178
       t197 = t186+a1j2sy*t76+t187+t183+a1j2ry*t195+t189
       t203 = a1j2sy*t10
       t204 = t8*a1j2sy
       t206 = a1j2syyy*t1
       t207 = t203+4*t204+t206
       t210 = a1j2sy*t37
       t212 = 7*t204
       t213 = t210+a1j2sy*t17+t212+t206+t203
       t215 = t212+t206+t203+t210
       t245 = 2*a1j2ryy*t37+a1j2sy*t94+3*a1j2ryy*t10+a1j2ryyyy*t1+2*
     & t21*a1j2ry+a1j2ryy*t17+9*t106*a1j2syy+a1j2ry*t27+a1j2sy*t116+3*
     & a1j2syy*t158+a1j2syy*t110+a1j2ry*t23+a1j2sy*t90+3*a1j2syyy*t60+
     & a1j2syyy*t74+2*a1j2syy*t137+a1j2ry*t34+a1j2sy*t122+a1j2ry*t30
       t249 = a1j2ry*t1*a1j2syy
       t251 = a1j2sy*t182
       t253 = a1j2ryy*t167
       t254 = 12*t249+t251+a1j2sy*t195+t253
       t262 = 6*t249+t253+t251
       t266 = 6*t2*a1j2sy*a1j2ry*vv1rsssss+(3*a1j2syy*t10+9*t7*a1j2syy+
     & a1j2syy*t17+a1j2sy*t23+a1j2sy*t27+a1j2sy*t30+a1j2sy*t34+
     & a1j2syyyy*t1+2*a1j2syy*t37)*vv1sss+t43*t42*vv1rrrrrr+15*
     & a1j2syy*t2*vv1sssss+t2*t1*vv1ssssss+(18*a1j2ryy*t51*a1j2sy+
     & a1j2syy*t43+a1j2ry*t70+a1j2ry*t78)*vv1rrrrs+t165*vv1rrs+20*
     & t167*t51*vv1rrrsss+(6*a1j2ryy*t42*t1+a1j2sy*t78+a1j2sy*t70+4*
     & t67*a1j2sy+a1j2ry*t190+a1j2ry*t197)*vv1rrrss+(12*t8*t1+a1j2sy*
     & t207+a1j2syyy*t167+a1j2sy*t213+a1j2sy*t215)*vv1ssss+t245*
     & vv1rss+(a1j2sy*t190+a1j2ry*t254+a1j2sy*t197+6*t56*t1+4*t184*
     & t167+a1j2ry*t262)*vv1rrsss
       t269 = a1j2ryyy*t42
       t270 = a1j2ry*t152
       t271 = t97*a1j2ry
       t272 = 7*t271
       t274 = a1j2ry*t99
       t275 = t269+t270+t272+a1j2ry*t133+t274
       t280 = 2*t106*a1j2ry
       t281 = a1j2syyy*t42
       t282 = t58*a1j2ryy
       t285 = 2*a1j2ryy*t60
       t286 = a1j2sy*t99
       t287 = a1j2ry*t158
       t288 = t280+t281+4*t282+t285+t286+t287
       t291 = t274+t269+4*t271
       t293 = 7*t282
       t294 = a1j2ryy*t74
       t295 = a1j2ry*t137
       t297 = a1j2sy*t152
       t299 = t293+t286+t287+t294+t281+t295+a1j2sy*t133+t297+t280+
     & a1j2ry*t110+t285
       t302 = t280+t287+t295+t293+t286+t281+t285+t294+t297
       t306 = t272+t269+t270+t274
       t309 = 12*t108*t42+a1j2sy*t275+2*a1j2ryy*t65+a1j2ry*t288+a1j2sy*
     & t291+a1j2ry*t299+a1j2syyy*t51+a1j2ry*t302+3*t269*a1j2sy+a1j2sy*
     & t306+a1j2ryy*t76
       t313 = a1j2ryyy**2
       t319 = a1j2syyy**2
       t333 = a1j2ry*t10
       t335 = 2*t7*a1j2ry
       t336 = a1j2ryyy*t1
       t338 = 2*a1j2syy*t60
       t339 = a1j2sy*t108
       t341 = a1j2sy*t158
       t342 = t333+t335+t336+t338+4*t339+t341
       t344 = a1j2ry*t37
       t345 = a1j2syy*t74
       t346 = 7*t339
       t347 = a1j2sy*t137
       t348 = t335+t344+t345+t333+t338+t341+t346+t336+t347
       t362 = t346+t347+t338+a1j2sy*t110+a1j2ry*t17+t333+t344+t345+
     & t336+t335+t341
       t365 = 3*t281*a1j2sy+a1j2ry*t342+a1j2ry*t348+a1j2ryy*t195+
     & a1j2sy*t302+a1j2sy*t288+2*a1j2syy*t65+2*a1j2ryy*t182+a1j2sy*
     & t299+3*t96*t1+a1j2ry*t362+a1j2syy*t76
       t389 = a1j2sy*t342+a1j2ry*t207+12*t181*a1j2syy+a1j2ryyy*t167+3*
     & t104*t1+a1j2sy*t362+a1j2ry*t213+a1j2syy*t195+a1j2sy*t348+
     & a1j2ry*t215+2*a1j2syy*t182
       t426 = t309*vv1rrrs+(15*a1j2ryy*a1j2ryyyy+10*t313)*vv1rr+(15*
     & a1j2syy*a1j2syyyy+10*t319)*vv1ss+(15*a1j2syy*a1j2ryyyy+20*
     & a1j2syyy*a1j2ryyy+15*a1j2syyyy*a1j2ryy)*vv1rs+t365*vv1rrss+(
     & a1j2sy*t254+18*a1j2ry*t167*a1j2syy+a1j2sy*t262+a1j2ryy*t2)*
     & vv1rssss+t389*vv1rsss+(9*t126*a1j2ry+a1j2ry*t148+a1j2ry*t128+
     & a1j2ry*t142+a1j2ryyyy*t42+2*a1j2ryy*t152+3*a1j2ryy*t99+a1j2ryy*
     & t133+a1j2ry*t163)*vv1rrr+15*a1j2ryy*t43*vv1rrrrr+15*t2*t42*
     & vv1rrssss+(a1j2ryyy*t51+a1j2ry*t291+a1j2ry*t275+12*t97*t42+
     & a1j2ry*t306)*vv1rrrr+6*a1j2sy*t43*a1j2ry*vv1rrrrrs+15*t1*t43*
     & vv1rrrrss
       vv1yyyyyy2 = t266+t426
       t1 = a2j2sx**2
       t3 = a2j2sxxx*a2j2sx
       t4 = a2j2sxx**2
       t5 = t3+t4
       t6 = 3*t5
       t9 = a2j2sxxx*a2j2rx
       t11 = a2j2sx*a2j2rxxx
       t13 = a2j2sxx*a2j2rxx
       t14 = 6*t13
       t15 = 4*t9+4*t11+t14
       t20 = 2*t11+2*t9+4*t13
       t23 = a2j2sxx*a2j2rxxx
       t24 = 10*t23
       t25 = a2j2sxxx*a2j2rxx
       t26 = 10*t25
       t27 = a2j2sxxxx*a2j2rx
       t29 = a2j2sx*a2j2rxxxx
       t31 = t24+t26+4*t27+4*t29
       t33 = a2j2sx*a2j2rxx
       t34 = a2j2sxx*a2j2rx
       t35 = t33+t34
       t36 = 2*t35
       t43 = 6*t23+2*t29+6*t25+2*t27
       t47 = 5*t27+t24+5*t29+t26
       t51 = a2j2sxxxx*a2j2sx
       t53 = a2j2sxx*a2j2sxxx
       t55 = 2*t51+6*t53
       t59 = 2*t5
       t63 = 10*t53
       t64 = 5*t51+t63
       t66 = 3*t35
       t69 = 4*t51+t63
       t73 = t14+3*t11+3*t9
       t80 = 3*t29+9*t23+9*t25+3*t27
       t84 = 4*t3+3*t4
       t88 = 9*t53+3*t51
       t90 = a2j2rxxxx*t1+2*a2j2rxx*t6+a2j2sxx*t15+3*a2j2sxx*t20+
     & a2j2sx*t31+3*a2j2sxxx*t36+a2j2sx*t43+a2j2sx*t47+9*t23*a2j2sx+
     & a2j2rx*t55+2*t51*a2j2rx+3*a2j2rxx*t59+a2j2rx*t64+a2j2sxxx*t66+
     & a2j2rx*t69+2*a2j2sxx*t73+a2j2sx*t80+a2j2rxx*t84+a2j2rx*t88
       t92 = t1*a2j2sx
       t96 = a2j2rxx*t92
       t97 = a2j2sx*t36
       t98 = t34*a2j2sx
       t100 = a2j2rxx*t1
       t101 = t97+4*t98+t100
       t102 = a2j2sx*t101
       t103 = t34*t1
       t105 = t96+t102+6*t103
       t107 = t1**2
       t112 = a2j2sx*t66+7*t98+t100+t97
       t114 = t96+12*t103+t102+a2j2sx*t112
       t118 = a2j2rx**2
       t121 = a2j2rxxx*t118
       t122 = a2j2rxx**2
       t123 = t122*a2j2rx
       t125 = a2j2rx*a2j2rxxx
       t126 = t125+t122
       t127 = 2*t126
       t128 = a2j2rx*t127
       t129 = t121+4*t123+t128
       t131 = 7*t123
       t132 = 3*t126
       t133 = a2j2rx*t132
       t134 = t131+t128+t121+t133
       t136 = a2j2sxx*t118
       t137 = a2j2rx*t36
       t139 = a2j2sx*a2j2rx*a2j2rxx
       t141 = t136+t137+4*t139
       t148 = 7*t139+t136+a2j2rx*t66+t137
       t151 = 2*a2j2rxx*t36
       t152 = t34*a2j2rxx
       t154 = a2j2rx*t20
       t155 = a2j2sxxx*t118
       t156 = a2j2sx*t127
       t158 = 2*t11*a2j2rx
       t159 = t151+4*t152+t154+t155+t156+t158
       t161 = t118*a2j2rx
       t163 = a2j2rxx*t66
       t164 = a2j2sx*t132
       t165 = 7*t152
       t166 = a2j2rx*t73
       t167 = t163+t156+t151+t154+t158+t164+t165+t155+t166
       t172 = 4*t125+3*t122
       t174 = t155+t158+t164+t154+a2j2rx*t15+a2j2sx*t172+t165+t163+
     & t156+t166+t151
       t177 = a2j2rx*t172+t131+t128+t133+t121
       t179 = 12*t13*t118+a2j2sx*t129+a2j2sx*t134+2*a2j2rxx*t141+3*
     & t121*a2j2sx+a2j2rxx*t148+a2j2rx*t159+a2j2sxxx*t161+a2j2rx*t167+
     & a2j2rx*t174+a2j2sx*t177
       t181 = t118**2
       t190 = a2j2rxxx**2
       t204 = a2j2sxxx**2
       t215 = a2j2rxx*t118
       t216 = t215*a2j2sx
       t218 = a2j2sxx*t161
       t219 = a2j2rx*t141
       t220 = 6*t216+t218+t219
       t224 = 12*t216+t219+t218+a2j2rx*t148
       t231 = t90*uu2rss+(18*a2j2rx*t92*a2j2sxx+a2j2sx*t105+a2j2rxx*
     & t107+a2j2sx*t114)*uu2rssss+t179*uu2rrrs+15*a2j2rxx*t181*
     & uu2rrrrr+15*t181*t1*uu2rrrrss+(15*a2j2rxx*a2j2rxxxx+10*t190)*
     & uu2rr+(15*a2j2sxxxx*a2j2rxx+20*a2j2sxxx*a2j2rxxx+15*a2j2sxx*
     & a2j2rxxxx)*uu2rs+(15*a2j2sxxxx*a2j2sxx+10*t204)*uu2ss+t181*
     & t118*uu2rrrrrr+t107*t1*uu2ssssss+(18*t33*t161+a2j2sxx*t181+
     & a2j2rx*t220+a2j2rx*t224)*uu2rrrrs+15*t118*t107*uu2rrssss
       t246 = t4*a2j2sx
       t247 = 7*t246
       t248 = a2j2sx*t59
       t249 = a2j2sx*t6
       t250 = a2j2sxxx*t1
       t251 = a2j2sx*t84+t247+t248+t249+t250
       t254 = t250+4*t246+t248
       t256 = t247+t248+t249+t250
       t263 = a2j2rx*a2j2rxxxx
       t265 = a2j2rxx*a2j2rxxx
       t267 = 2*t263+6*t265
       t272 = 10*t265
       t274 = t272+5*t263
       t277 = t272+4*t263
       t293 = 9*t265+3*t263
       t299 = 3*a2j2sxx*t127+a2j2sxx*t172+a2j2sx*t267+9*t25*a2j2rx+
     & a2j2rx*t31+a2j2sx*t274+a2j2sx*t277+3*a2j2rxx*t20+3*a2j2rxxx*
     & t36+2*a2j2sxx*t132+a2j2sxxxx*t118+a2j2rxxx*t66+a2j2rx*t43+
     & a2j2rxx*t15+2*a2j2rxx*t73+a2j2sx*t293+2*t29*a2j2rx+a2j2rx*t80+
     & a2j2rx*t47
       t304 = t33*a2j2sxx
       t306 = a2j2rxxx*t1
       t307 = a2j2rx*t59
       t309 = 2*a2j2sxx*t36
       t311 = 2*t3*a2j2rx
       t312 = a2j2sx*t20
       t313 = 4*t304+t306+t307+t309+t311+t312
       t316 = 7*t304
       t319 = a2j2rx*t6
       t320 = a2j2sxx*t66
       t321 = a2j2sx*t73
       t322 = t316+a2j2rx*t84+a2j2sx*t15+t319+t306+t307+t309+t312+t320+
     & t321+t311
       t326 = t312+t316+t311+t320+t306+t319+t307+t321+t309
       t333 = a2j2rx*t251+12*t100*a2j2sxx+a2j2sx*t313+a2j2rxxx*t92+
     & a2j2sx*t322+3*t9*t1+a2j2sx*t326+2*a2j2sxx*t101+a2j2rx*t256+
     & a2j2sxx*t112+a2j2rx*t254
       t337 = a2j2rxx*a2j2rx
       t341 = 3*t337*t1
       t342 = a2j2sx*t141
       t344 = 3*t136*a2j2sx
       t345 = a2j2rx*t101
       t346 = t341+t342+t344+t345
       t352 = t341+t345+a2j2rx*t112+a2j2sx*t148+t344+t342
       t422 = a2j2sx*t167+a2j2rxx*t112+a2j2sx*t174+a2j2rx*t313+a2j2sxx*
     & t148+2*a2j2sxx*t141+a2j2rx*t322+3*t155*a2j2sx+a2j2sx*t159+
     & a2j2rx*t326+3*t125*t1+2*a2j2rxx*t101
       t424 = 20*t161*t92*uu2rrrsss+15*t107*a2j2sxx*uu2sssss+6*a2j2rx*
     & t107*a2j2sx*uu2rsssss+(a2j2sxxx*t92+12*t4*t1+a2j2sx*t251+
     & a2j2sx*t254+a2j2sx*t256)*uu2ssss+t299*uu2rrs+t333*uu2rsss+(
     & a2j2rx*t105+a2j2rx*t114+4*t337*t92+a2j2sx*t346+6*t136*t1+
     & a2j2sx*t352)*uu2rrsss+(a2j2sx*t224+6*t215*t1+a2j2sx*t220+4*
     & t218*a2j2sx+a2j2rx*t346+a2j2rx*t352)*uu2rrrss+(a2j2sxx*t84+9*
     & t3*a2j2sxx+2*a2j2sxx*t6+a2j2sx*t64+a2j2sxxxx*t1+a2j2sx*t55+
     & a2j2sx*t69+3*a2j2sxx*t59+a2j2sx*t88)*uu2sss+(a2j2rxx*t172+
     & a2j2rx*t267+a2j2rx*t274+9*t265*a2j2rx+3*a2j2rxx*t127+2*a2j2rxx*
     & t132+a2j2rx*t277+a2j2rxxxx*t118+a2j2rx*t293)*uu2rrr+6*a2j2sx*
     & t181*a2j2rx*uu2rrrrrs+(a2j2rx*t129+a2j2rx*t177+12*t122*t118+
     & a2j2rxxx*t161+a2j2rx*t134)*uu2rrrr+t422*uu2rrss
       uu2xxxxxx2 = t231+t424
       t1 = a2j2sx*a2j2rxy
       t4 = a2j2rx**2
       t5 = a2j2sxy*t4
       t6 = a2j2sxy*a2j2rx
       t8 = 2*t1+2*t6
       t10 = 4*t1*a2j2rx+t5+a2j2rx*t8
       t11 = a2j2sx*t10
       t13 = 3*t5*a2j2sx
       t15 = a2j2sx**2
       t17 = 3*a2j2rxy*a2j2rx*t15
       t20 = a2j2rxy*t15
       t22 = 4*a2j2sx*t6+t20+a2j2sx*t8
       t23 = a2j2rx*t22
       t24 = t11+t13+t17+t23
       t27 = 6*t6*t15
       t28 = a2j2sx*t15
       t29 = a2j2rxy*t28
       t30 = a2j2sx*t22
       t31 = t27+t29+t30
       t33 = a2j2syy*t4
       t36 = a2j2sxx*a2j2rx
       t38 = t36+a2j2sx*a2j2rxx
       t39 = 3*t38
       t44 = 2*t38
       t46 = a2j2sx*t39+7*a2j2sx*t36+a2j2rxx*t15+a2j2sx*t44
       t51 = a2j2sy*t46+t27+t30+t29+6*a2j2ry*t15*a2j2sxx
       t62 = 7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t4+a2j2rx*t39+t44*a2j2rx
       t65 = t13+t23+a2j2sy*t62+a2j2ry*t46+t17+t11
       t74 = 4*t28*a2j2sxy+6*a2j2sy*t15*a2j2sxx
       t84 = t15**2
       t88 = a2j2rxy**2
       t91 = a2j2rxxy*t4
       t92 = a2j2rxy*a2j2rxx
       t94 = 7*t92*a2j2rx
       t96 = a2j2rx*a2j2rxxy+t92
       t97 = 3*t96
       t98 = a2j2rx*t97
       t99 = 2*t96
       t100 = a2j2rx*t99
       t103 = a2j2rxx**2
       t105 = 4*a2j2rx*a2j2rxxx+3*t103
       t107 = t91+t94+t98+t100+a2j2ry*t105
       t109 = t94+t100+t98+t91
       t111 = t4*a2j2rx
       t113 = a2j2ryy*t4
       t116 = a2j2rxyy*t4
       t117 = a2j2rx*a2j2rxyy
       t119 = 2*t117+2*t88
       t123 = t116+a2j2rx*t119+4*a2j2rx*t88
       t134 = a2j2sxx*t8
       t136 = 5*t1*a2j2sxx
       t137 = a2j2sxy*t39
       t138 = a2j2sx*a2j2rxxy
       t139 = a2j2sxx*a2j2rxy
       t141 = a2j2sxy*a2j2rxx
       t142 = t138+t139+a2j2sxxy*a2j2rx+t141
       t143 = 3*t142
       t144 = a2j2sx*t143
       t145 = a2j2sxxy*a2j2sx
       t147 = 2*t145*a2j2rx
       t149 = a2j2sxy*a2j2sxx+t145
       t150 = 3*t149
       t151 = a2j2rx*t150
       t152 = a2j2sxy*t44
       t159 = 4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+6*a2j2sxx*a2j2rxx
       t161 = 2*t142
       t162 = a2j2sx*t161
       t163 = 2*t149
       t164 = a2j2rx*t163
       t167 = a2j2sxx**2
       t169 = 4*a2j2sxxx*a2j2sx+3*t167
       t172 = 2*t141*a2j2sx
       t173 = a2j2rxxy*t15
       t174 = t134+t136+t137+t144+t147+t151+t152+a2j2sy*t159+t162+t164+
     & a2j2ry*t169+t172+t173
       t176 = a2j2sxy*a2j2rxy
       t179 = a2j2sxyy*t4
       t181 = a2j2sx*a2j2rxyy
       t183 = a2j2sxyy*a2j2rx
       t186 = 2*t181+2*t183+4*t176
       t192 = 4*a2j2rx*t176+t179+a2j2sx*t119+a2j2rx*t186+2*a2j2rx*t181+
     & 2*a2j2rxy*t8
       t194 = a2j2sxxy*t4
       t196 = 5*t141*a2j2rx
       t199 = 2*t138*a2j2rx
       t200 = a2j2rx*t143
       t201 = a2j2rxy*t44
       t202 = a2j2rxx*t8
       t203 = t97*a2j2sx
       t204 = a2j2rx*t161
       t206 = 2*t139*a2j2rx
       t207 = a2j2rxy*t39
       t208 = a2j2sx*t99
       t210 = t194+t196+a2j2sy*t105+t199+t200+t201+t202+t203+t204+t206+
     & t207+t208+a2j2ry*t159
       t214 = a2j2sx*a2j2sxyy
       t215 = a2j2sxy**2
       t217 = 2*t214+2*t215
       t227 = a2j2rx*t217+a2j2sx*t186+4*a2j2sx*t176+a2j2rxyy*t15+2*
     & t214*a2j2rx+2*a2j2sxy*t8
       t233 = t202+t204+t206+t203+t194+t200+t199+t208+t196+t207+t201
       t236 = t151+t162+t137+t164+t147+t136+t172+t134+t144+t152+t173
       t241 = a2j2ry*t174+a2j2sx*t192+a2j2sy*t210+2*a2j2rxy*t22+a2j2rx*
     & t227+3*t179*a2j2sx+3*t117*t15+a2j2sy*t233+a2j2ryy*t46+a2j2ry*
     & t236+a2j2syy*t62+2*a2j2sxy*t10
       t259 = 2*a2j2rxy*t10+a2j2ry*t233+a2j2ry*t210+6*t33*a2j2rxx+6*
     & t176*t4+a2j2sy*t107+a2j2sx*t123+3*t116*a2j2sx+a2j2sy*t109+
     & a2j2rx*t192+a2j2sxyy*t111+a2j2ryy*t62
       t261 = a2j2ry**2
       t262 = t4**2
       t265 = a2j2sy**2
       t268 = a2j2sx*t150
       t269 = a2j2sxxy*t15
       t272 = 7*a2j2sxy*a2j2sx*a2j2sxx
       t273 = a2j2sx*t163
       t275 = t268+t269+t272+t273+a2j2sy*t169
       t287 = a2j2sxyy*t15+4*t215*a2j2sx+a2j2sx*t217
       t289 = t272+t269+t268+t273
       t295 = a2j2sxy*a2j2sxxy
       t298 = a2j2sxyy*a2j2sxx
       t300 = a2j2sxxyy*a2j2sx
       t303 = 2*t298+2*t300+4*t295
       t309 = 3*t298+3*t300+6*t295
       t312 = 4*a2j2sxxx*a2j2sxy
       t314 = 4*a2j2sxxxy*a2j2sx
       t316 = 6*a2j2sxx*a2j2sxxy
       t318 = t312+t314+t316+a2j2sy*a2j2sxxxx
       t320 = t314+t312+t316
       t328 = 5*t214*a2j2sxx+4*t295*a2j2sx+a2j2sx*t303+a2j2sxx*t217+
     & a2j2sx*t309+a2j2sy*t318+a2j2sy*t320+2*a2j2sxy*t150+a2j2sxxyy*
     & t15+2*a2j2sxy*t163+a2j2syy*t169
       t330 = a2j2ry*a2j2sy
       t336 = a2j2ry*t84+4*a2j2sy*a2j2rx*t28
       t340 = a2j2sxyy*a2j2rxx
       t342 = a2j2sxxy*a2j2rxy
       t344 = a2j2sxy*a2j2rxxy
       t346 = a2j2sx*a2j2rxxyy
       t348 = a2j2sxxyy*a2j2rx
       t350 = a2j2sxx*a2j2rxyy
       t352 = 3*t340+6*t342+6*t344+3*t346+3*t348+3*t350
       t366 = 2*t346+2*t340+4*t344+4*t342+2*t350+2*t348
       t373 = 4*a2j2sxxx*a2j2rxy
       t376 = 4*a2j2sxxxy*a2j2rx
       t378 = 4*a2j2sxy*a2j2rxxx
       t380 = 4*a2j2sx*a2j2rxxxy
       t382 = 6*a2j2sxx*a2j2rxxy
       t385 = 6*a2j2sxxy*a2j2rxx
       t386 = t373+a2j2ry*a2j2sxxxx+t376+t378+t380+t382+a2j2sy*
     & a2j2rxxxx+t385
       t389 = a2j2sx*t352+a2j2syy*t159+a2j2ry*t318+2*a2j2sxy*t161+2*
     & a2j2rxy*t150+a2j2sx*t366+a2j2ry*t320+a2j2sxyy*t44+2*t300*
     & a2j2rx+a2j2sy*t386+a2j2rxx*t217
       t392 = t376+t373+t380+t382+t385+t378
       t408 = 2*a2j2sxy*t143+a2j2sy*t392+a2j2rx*t309+5*t350*a2j2sx+
     & a2j2sxx*t186+a2j2sxyy*t39+2*a2j2rxy*t163+a2j2ryy*t169+a2j2rx*
     & t303+2*a2j2sxxy*t8+a2j2rxxyy*t15+4*t344*a2j2sx
       t411 = (a2j2sy*t24+a2j2ry*t31+6*t33*t15+a2j2ry*t51+4*a2j2ryy*
     & a2j2rx*t28+a2j2sy*t65)*uu2rrsss+(a2j2ry*t74+a2j2sy*t51+a2j2sy*
     & t31+4*a2j2ry*t28*a2j2sxy+4*a2j2syy*a2j2rx*t28+a2j2ryy*t84)*
     & uu2rssss+(6*t88*t4+a2j2ry*t107+a2j2ry*t109+a2j2rxyy*t111+6*
     & t113*a2j2rxx+a2j2rx*t123)*uu2rrrr+(a2j2sy*t74+a2j2syy*t84+4*
     & a2j2sy*t28*a2j2sxy)*uu2sssss+t241*uu2rrss+t259*uu2rrrs+t261*
     & t262*uu2rrrrrr+t265*t84*uu2ssssss+(a2j2sy*t275+6*a2j2syy*t15*
     & a2j2sxx+6*t215*t15+a2j2sxyy*t28+a2j2sx*t287+a2j2sy*t289)*
     & uu2ssss+t328*uu2sss+(t330*t84+a2j2sy*t336)*uu2rsssss+(t389+
     & t408)*uu2rss
       t413 = a2j2ry*t111
       t418 = a2j2ry*t4
       t421 = 4*t111*a2j2rxy+6*t418*a2j2rxx
       t425 = a2j2sxxy**2
       t439 = 4*t413*a2j2sx+a2j2sy*t262
       t445 = a2j2sy*t111
       t450 = 4*t445*a2j2sx+6*t418*t15
       t457 = a2j2sy*t4
       t460 = 4*a2j2ry*a2j2rx*t28+6*t457*t15
       t470 = 4*a2j2rxxx*a2j2rxy
       t472 = 6*a2j2rxxy*a2j2rxx
       t474 = 4*a2j2rxxxy*a2j2rx
       t475 = a2j2ry*a2j2rxxxx+t470+t472+t474
       t487 = t474+t470+t472
       t489 = a2j2ry*t392+5*t340*a2j2rx+a2j2sy*t475+a2j2rx*t352+2*
     & a2j2rxxy*t8+a2j2rxyy*t44+a2j2rxx*t186+a2j2sxx*t119+2*a2j2sxy*
     & t97+2*a2j2rxy*t161+a2j2sy*t487
       t495 = a2j2rx*a2j2rxxyy
       t497 = a2j2rxx*a2j2rxyy
       t499 = a2j2rxy*a2j2rxxy
       t501 = 3*t495+3*t497+6*t499
       t510 = 2*t495+4*t499+2*t497
       t517 = a2j2ryy*t159+a2j2syy*t105+a2j2ry*t386+2*a2j2sxy*t99+
     & a2j2sx*t501+a2j2sxxyy*t4+4*t342*a2j2rx+a2j2rx*t366+a2j2sx*t510+
     & 2*t346*a2j2rx+2*a2j2rxy*t143+a2j2rxyy*t39
       t553 = 5*t497*a2j2rx+a2j2ry*t475+a2j2rx*t510+a2j2ry*t487+a2j2rx*
     & t501+a2j2rxxyy*t4+2*a2j2rxy*t97+4*t499*a2j2rx+a2j2ryy*t105+2*
     & a2j2rxy*t99+a2j2rxx*t119
       t564 = a2j2rx*t10
       t566 = 6*t1*t4
       t567 = a2j2sxy*t111
       t569 = 6*t457*a2j2rxx+t564+t566+t567+a2j2ry*t62
       t571 = t567+t566+t564
       t607 = a2j2rxyy*t28+3*t183*t15+a2j2ry*t289+a2j2sy*t174+a2j2rx*
     & t287+a2j2sy*t236+6*a2j2ryy*t15*a2j2sxx+2*a2j2sxy*t22+a2j2syy*
     & t46+a2j2sx*t227+6*t20*a2j2sxy+a2j2ry*t275
       t616 = a2j2rxxy**2
       t620 = (a2j2ryy*t262+4*t413*a2j2rxy+a2j2ry*t421)*uu2rrrrr+(6*
     & t425+8*a2j2sxxxy*a2j2sxy+a2j2syy*a2j2sxxxx+4*a2j2sxyy*a2j2sxxx+
     & 6*a2j2sxxyy*a2j2sxx)*uu2ss+(a2j2ry*t439+t330*t262)*uu2rrrrrs+(
     & a2j2sy*t439+a2j2ry*t450)*uu2rrrrss+(a2j2ry*t460+a2j2sy*t450)*
     & uu2rrrsss+(t489+t517)*uu2rrs+(8*a2j2sxxxy*a2j2rxy+4*a2j2sxxx*
     & a2j2rxyy+4*a2j2sxyy*a2j2rxxx+6*a2j2sxxyy*a2j2rxx+12*a2j2sxxy*
     & a2j2rxxy+a2j2syy*a2j2rxxxx+6*a2j2sxx*a2j2rxxyy+a2j2ryy*
     & a2j2sxxxx+8*a2j2sxy*a2j2rxxxy)*uu2rs+t553*uu2rrr+(a2j2ry*t336+
     & a2j2sy*t460)*uu2rrssss+(4*t445*a2j2rxy+a2j2syy*t262+a2j2ry*
     & t569+a2j2ry*t571+a2j2sy*t421+4*a2j2ryy*t111*a2j2sx)*uu2rrrrs+(
     & a2j2sy*t569+a2j2ry*t65+4*a2j2syy*t111*a2j2sx+a2j2ry*t24+a2j2sy*
     & t571+6*t113*t15)*uu2rrrss+t607*uu2rsss+(8*a2j2rxy*a2j2rxxxy+4*
     & a2j2rxyy*a2j2rxxx+6*a2j2rxx*a2j2rxxyy+a2j2ryy*a2j2rxxxx+6*t616)
     & *uu2rr
       uu2xxxxyy2 = t411+t620
       t1 = a2j2sy**2
       t2 = t1**2
       t3 = a2j2sx**2
       t6 = a2j2ry**2
       t7 = t6**2
       t8 = a2j2rx**2
       t13 = 2*a2j2sy*a2j2sxy*a2j2sx
       t17 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
       t19 = a2j2syy*t3
       t20 = t13+a2j2sy*t17+t19
       t23 = a2j2syy*a2j2sy*t3
       t24 = t19+t13
       t25 = a2j2sy*t24
       t26 = a2j2sy*t20+t23+t25
       t30 = t23+t25
       t34 = a2j2sx*a2j2rxy
       t35 = a2j2sxy*a2j2rx
       t37 = 2*t34+2*t35
       t38 = a2j2sy*t37
       t41 = 2*a2j2syy*a2j2sx*a2j2rx
       t44 = 2*a2j2ry*a2j2sxy*a2j2sx
       t45 = a2j2ryy*t3
       t46 = t38+t41+t44+t45
       t49 = a2j2syyy*t3
       t52 = 4*a2j2syy*a2j2sxy*a2j2sx
       t53 = a2j2sx*a2j2sxyy
       t54 = a2j2sxy**2
       t56 = 2*t53+2*t54
       t57 = a2j2sy*t56
       t58 = t49+t52+t57
       t64 = a2j2ry*t3+2*a2j2sy*a2j2sx*a2j2rx
       t66 = a2j2sy*a2j2sxxy
       t67 = 2*t53
       t68 = a2j2syy*a2j2sxx
       t69 = 2*t54
       t70 = t66+t67+t68+t69
       t71 = a2j2sy*t70
       t72 = a2j2syy*t17
       t73 = t52+t71+t72+t57+t49
       t76 = t69+2*t66+t68+t67
       t78 = t71+t49+t52+t72+t57+a2j2sy*t76
       t82 = a2j2ry*t56
       t85 = 2*a2j2syyy*a2j2sx*a2j2rx
       t86 = a2j2ryyy*t3
       t88 = 2*a2j2syy*t37
       t91 = 4*a2j2ryy*a2j2sxy*a2j2sx
       t93 = 2*a2j2sx*a2j2rxyy
       t95 = 2*a2j2sxyy*a2j2rx
       t97 = 4*a2j2sxy*a2j2rxy
       t98 = t93+t95+t97
       t99 = a2j2sy*t98
       t100 = t82+t85+t86+t88+t91+t99
       t106 = a2j2ry*a2j2sxx+2*t34+2*t35+a2j2sy*a2j2rxx
       t109 = a2j2sy*t106+a2j2ry*t17+t45+t44+t41+t38
       t114 = a2j2sy*a2j2rxxy
       t115 = a2j2ry*a2j2sxxy
       t116 = a2j2ryy*a2j2sxx
       t117 = a2j2syy*a2j2rxx
       t118 = t114+t115+t116+t97+t95+t117+t93
       t119 = a2j2sy*t118
       t120 = a2j2ryy*t17
       t121 = a2j2syy*t106
       t122 = a2j2ry*t70
       t123 = t119+t91+t120+t85+t121+t82+t88+t99+t122+t86
       t128 = t97+2*t115+t116+2*t114+t95+t117+t93
       t130 = t121+a2j2ry*t76+a2j2sy*t128+t86+t119+t91+t99+t122+t85+
     & t88+t120+t82
       t132 = 2*a2j2syy*t46+a2j2ry*t58+a2j2syyy*t64+a2j2ry*t73+a2j2ry*
     & t78+2*a2j2ryy*t24+a2j2sy*t100+a2j2syy*t109+a2j2ryy*t20+
     & a2j2ryyy*a2j2sy*t3+a2j2sy*t123+a2j2sy*t130
       t134 = a2j2ryyy*a2j2rxx
       t135 = a2j2ry*a2j2rxxyy
       t138 = 6*a2j2rxy*a2j2rxyy
       t140 = 2*a2j2rx*a2j2rxyyy
       t141 = a2j2ryy*a2j2rxxy
       t142 = 3*t141
       t143 = t134+3*t135+t138+t140+t142
       t146 = t140+t138+2*t135+t134+t142
       t149 = t140+t138
       t151 = a2j2rx*a2j2rxyy
       t152 = 2*t151
       t153 = a2j2ryy*a2j2rxx
       t154 = a2j2ry*a2j2rxxy
       t156 = a2j2rxy**2
       t157 = 2*t156
       t158 = t152+t153+2*t154+t157
       t161 = 2*t151+2*t156
       t164 = t153+t152+t154+t157
       t171 = t134+t138+t135+t140+2*t141
       t176 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
       t184 = 2*a2j2ry*a2j2rxy*a2j2rx
       t186 = a2j2ryy*t8
       t187 = t184+a2j2ry*t176+t186
       t189 = a2j2ry*t161
       t190 = a2j2ryyy*t8
       t193 = 4*a2j2ryy*a2j2rxy*a2j2rx
       t194 = t189+t190+t193
       t196 = a2j2ry*t164
       t197 = a2j2ryy*t176
       t198 = t193+t189+t196+t190+t197
       t200 = t184+t186
       t204 = t193+t196+t190+a2j2ry*t158+t197+t189
       t209 = a2j2ry*a2j2sy
       t211 = a2j2sy*t64+t209*t3
       t215 = a2j2sy*t211+a2j2ry*t1*t3
       t226 = a2j2sy*t8+2*a2j2ry*a2j2sx*a2j2rx
       t228 = a2j2syy*t8
       t229 = a2j2ry*t37
       t232 = 2*a2j2ryy*a2j2sx*a2j2rx
       t235 = 2*a2j2sy*a2j2rxy*a2j2rx
       t236 = t228+t229+t232+t235
       t241 = 2*a2j2ryy*t37
       t242 = a2j2syy*t176
       t245 = 4*a2j2syy*a2j2rxy*a2j2rx
       t246 = a2j2ry*t118
       t247 = a2j2sy*t161
       t248 = a2j2sy*t164
       t251 = 2*a2j2ryyy*a2j2sx*a2j2rx
       t252 = a2j2ry*t98
       t253 = a2j2ryy*t106
       t254 = a2j2syyy*t8
       t255 = t241+t242+t245+t246+t247+t248+t251+t252+t253+t254
       t261 = a2j2sy*t158+t251+t242+t241+t254+a2j2ry*t128+t245+t248+
     & t253+t247+t246+t252
       t263 = t251+t254+t245+t241+t247+t252
       t269 = a2j2sy*t176+t235+t232+t228+a2j2ry*t106+t229
       t274 = a2j2ryyy*t226+2*a2j2ryy*t236+a2j2sy*t204+a2j2ry*t255+2*
     & a2j2syy*t200+a2j2ry*t261+a2j2ry*t263+a2j2sy*t194+a2j2sy*t198+
     & a2j2ryy*t269+a2j2syyy*a2j2ry*t8+a2j2syy*t187
       t288 = 2*a2j2sxyyy*a2j2rx
       t289 = a2j2syy*a2j2rxxy
       t290 = 3*t289
       t292 = 6*a2j2sxy*a2j2rxyy
       t293 = a2j2ry*a2j2sxxyy
       t295 = a2j2ryyy*a2j2sxx
       t296 = a2j2ryy*a2j2sxxy
       t297 = 3*t296
       t299 = 6*a2j2sxyy*a2j2rxy
       t301 = 2*a2j2sx*a2j2rxyyy
       t302 = a2j2syyy*a2j2rxx
       t303 = a2j2sy*a2j2rxxyy
       t305 = t288+t290+t292+2*t293+t295+t297+t299+t301+t302+2*t303
       t310 = t295+t303+t301+t288+t299+t292+2*t289+t293+2*t296+t302
       t321 = t295+3*t293+t290+t299+t302+t301+3*t303+t297+t292+t288
       t329 = t292+t301+t299+t288
       t331 = a2j2sy*t149+3*a2j2syy*t161+a2j2sy*t146+a2j2syyyy*t8+6*
     & a2j2syyy*a2j2rxy*a2j2rx+2*a2j2syy*t164+a2j2sy*t171+a2j2ry*t305+
     & a2j2sy*t143+a2j2ry*t310+a2j2syyy*t176+2*a2j2ryyyy*a2j2sx*
     & a2j2rx+a2j2syy*t158+3*a2j2ryy*t98+a2j2ry*t321+3*a2j2ryyy*t37+
     & a2j2ryy*t128+a2j2ryyy*t106+2*a2j2ryy*t118+a2j2ry*t329
       t335 = a2j2syy*t64
       t336 = a2j2sy*t46
       t337 = a2j2ry*t24
       t339 = a2j2ryy*a2j2sy*t3
       t340 = t335+t336+t337+t339
       t344 = t337+t336+t335+t339+a2j2sy*t109+a2j2ry*t20
       t383 = a2j2syyy*t226+a2j2sy*t261+a2j2ry*t100+a2j2ry*t123+a2j2sy*
     & t255+a2j2syy*t269+2*a2j2syy*t236+a2j2ryy*t109+a2j2sy*t263+
     & a2j2ryyy*t64+a2j2ry*t130+2*a2j2ryy*t46
       t385 = t2*t3*uu2ssssss+t7*t8*uu2rrrrrr+(a2j2sy*t26+a2j2syy*t1*
     & t3+a2j2sy*t30)*uu2sssss+t132*uu2rsss+(a2j2ry*t143+a2j2ry*t146+
     & a2j2ryyyy*t8+a2j2ry*t149+a2j2ryy*t158+3*a2j2ryy*t161+2*a2j2ryy*
     & t164+6*a2j2ryyy*a2j2rxy*a2j2rx+a2j2ry*t171+a2j2ryyy*t176)*
     & uu2rrr+(a2j2ryyy*a2j2ry*t8+a2j2ryy*t187+a2j2ry*t194+a2j2ry*
     & t198+2*a2j2ryy*t200+a2j2ry*t204)*uu2rrrr+(a2j2sy*t215+a2j2ry*
     & t1*a2j2sy*t3)*uu2rsssss+t274*uu2rrrs+t331*uu2rrs+(a2j2ryy*t1*
     & t3+a2j2sy*t340+a2j2sy*t344+a2j2syy*t211+a2j2ry*t30+a2j2ry*t26)*
     & uu2rssss+(8*a2j2sxy*a2j2rxyyy+12*a2j2sxyy*a2j2rxyy+a2j2syyyy*
     & a2j2rxx+8*a2j2sxyyy*a2j2rxy+6*a2j2syy*a2j2rxxyy+4*a2j2syyy*
     & a2j2rxxy+a2j2ryyyy*a2j2sxx+6*a2j2ryy*a2j2sxxyy+4*a2j2ryyy*
     & a2j2sxxy)*uu2rs+t383*uu2rrss
       t390 = a2j2ryy*a2j2ry*t8
       t391 = a2j2ry*t200
       t392 = a2j2ry*t187+t390+t391
       t394 = t391+t390
       t398 = a2j2sxyy**2
       t410 = a2j2ry*t46
       t411 = a2j2syy*t226
       t413 = a2j2sy*t236
       t415 = a2j2ryy*t64
       t416 = t410+t411+a2j2ry*t109+t413+a2j2sy*t269+t415
       t420 = a2j2sy*t226+a2j2ry*t64
       t423 = t413+t411+t415+t410
       t433 = t209*t8+a2j2ry*t226
       t437 = a2j2ry*t433+a2j2sy*t6*t8
       t461 = a2j2syy*a2j2sxxy
       t462 = 3*t461
       t463 = a2j2sy*a2j2sxxyy
       t466 = 2*a2j2sx*a2j2sxyyy
       t468 = 6*a2j2sxyy*a2j2sxy
       t469 = a2j2syyy*a2j2sxx
       t470 = t462+2*t463+t466+t468+t469
       t477 = t466+t468
       t482 = t468+2*t461+t463+t469+t466
       t491 = t462+t466+t469+3*t463+t468
       t495 = 3*a2j2syy*t98+3*a2j2syyy*t37+a2j2sy*t305+2*a2j2syyyy*
     & a2j2sx*a2j2rx+a2j2syyy*t106+a2j2sy*t310+a2j2ry*t470+2*a2j2syy*
     & t118+6*a2j2ryyy*a2j2sxy*a2j2sx+a2j2ry*t477+a2j2ryyyy*t3+
     & a2j2syy*t128+a2j2ry*t482+a2j2sy*t329+2*a2j2ryy*t70+a2j2sy*t321+
     & 3*a2j2ryy*t56+a2j2ry*t491+a2j2ryyy*t17+a2j2ryy*t76
       t499 = a2j2ry*t236
       t501 = a2j2ry*t8*a2j2syy
       t502 = a2j2ryy*t226
       t503 = a2j2sy*t200
       t504 = a2j2sy*t187+a2j2ry*t269+t499+t501+t502+t503
       t506 = t501+t499+t503+t502
       t517 = a2j2ry*t420+a2j2sy*t433
       t533 = a2j2rxyy**2
       t560 = a2j2sy*t420+a2j2ry*t211
       t568 = (a2j2ryy*t6*t8+a2j2ry*t392+a2j2ry*t394)*uu2rrrrr+(6*t398+
     & a2j2syyyy*a2j2sxx+8*a2j2sxy*a2j2sxyyy+6*a2j2syy*a2j2sxxyy+4*
     & a2j2syyy*a2j2sxxy)*uu2ss+(a2j2ry*t340+a2j2sy*t416+a2j2syy*t420+
     & a2j2ry*t344+a2j2sy*t423+a2j2ryy*t211)*uu2rrsss+(a2j2sy*t6*
     & a2j2ry*t8+a2j2ry*t437)*uu2rrrrrs+(a2j2sy*t58+a2j2syy*t20+
     & a2j2sy*t78+a2j2syyy*a2j2sy*t3+a2j2sy*t73+2*a2j2syy*t24)*
     & uu2ssss+t495*uu2rss+(a2j2ry*t504+a2j2ry*t506+a2j2ryy*t433+
     & a2j2sy*t392+a2j2syy*t6*t8+a2j2sy*t394)*uu2rrrrs+(a2j2ry*t517+
     & a2j2sy*t437)*uu2rrrrss+(a2j2ry*t423+a2j2ryy*t420+a2j2ry*t416+
     & a2j2sy*t506+a2j2sy*t504+a2j2syy*t433)*uu2rrrss+(a2j2ryyyy*
     & a2j2rxx+4*a2j2ryyy*a2j2rxxy+6*t533+8*a2j2rxyyy*a2j2rxy+6*
     & a2j2ryy*a2j2rxxyy)*uu2rr+(a2j2sy*t491+a2j2sy*t470+a2j2sy*t482+
     & a2j2sy*t477+3*a2j2syy*t56+6*a2j2syyy*a2j2sxy*a2j2sx+a2j2syyyy*
     & t3+a2j2syy*t76+a2j2syyy*t17+2*a2j2syy*t70)*uu2sss+(a2j2ry*t215+
     & a2j2sy*t560)*uu2rrssss+(a2j2sy*t517+a2j2ry*t560)*uu2rrrsss
       uu2xxyyyy2 = t385+t568
       t1 = a2j2sy**2
       t2 = t1**2
       t7 = a2j2syyy*a2j2sy
       t8 = a2j2syy**2
       t9 = t7+t8
       t10 = 2*t9
       t17 = 4*t7+3*t8
       t19 = a2j2syy*a2j2syyy
       t20 = 10*t19
       t21 = a2j2sy*a2j2syyyy
       t23 = t20+4*t21
       t27 = 2*t21+6*t19
       t30 = t20+5*t21
       t34 = 9*t19+3*t21
       t37 = 3*t9
       t42 = a2j2ry**2
       t43 = t42**2
       t51 = t42*a2j2ry
       t56 = a2j2syy*t42
       t57 = a2j2sy*a2j2ryy
       t58 = a2j2syy*a2j2ry
       t59 = t57+t58
       t60 = 2*t59
       t61 = a2j2ry*t60
       t63 = a2j2sy*a2j2ry*a2j2ryy
       t65 = t56+t61+4*t63
       t66 = a2j2ry*t65
       t67 = a2j2syy*t51
       t68 = t57*t42
       t70 = t66+t67+6*t68
       t74 = 3*t59
       t76 = 7*t63+t56+a2j2ry*t74+t61
       t78 = 12*t68+t66+t67+a2j2ry*t76
       t82 = a2j2syy*a2j2ryyy
       t83 = 10*t82
       t84 = a2j2syyy*a2j2ryy
       t85 = 10*t84
       t86 = a2j2sy*a2j2ryyyy
       t88 = a2j2syyyy*a2j2ry
       t90 = t83+t85+5*t86+5*t88
       t94 = t83+4*t86+t85+4*t88
       t96 = a2j2ry*a2j2ryyy
       t97 = a2j2ryy**2
       t98 = t96+t97
       t99 = 2*t98
       t104 = a2j2syyy*a2j2ry
       t106 = a2j2sy*a2j2ryyy
       t108 = a2j2syy*a2j2ryy
       t109 = 6*t108
       t110 = 4*t104+4*t106+t109
       t116 = 6*t82+2*t86+6*t84+2*t88
       t122 = 9*t84+3*t86+9*t82+3*t88
       t124 = a2j2ry*a2j2ryyyy
       t126 = a2j2ryyy*a2j2ryy
       t128 = 2*t124+6*t126
       t133 = 4*t96+3*t97
       t137 = t109+3*t106+3*t104
       t141 = 10*t126
       t142 = 5*t124+t141
       t148 = t141+4*t124
       t152 = 3*t98
       t158 = 2*t106+2*t104+4*t108
       t163 = 9*t126+3*t124
       t165 = a2j2ry*t90+a2j2ry*t94+3*a2j2syy*t99+9*t84*a2j2ry+a2j2ryy*
     & t110+a2j2ry*t116+a2j2ry*t122+a2j2sy*t128+a2j2ryyy*t74+a2j2syy*
     & t133+2*a2j2ryy*t137+a2j2sy*t142+3*a2j2ryyy*t60+a2j2syyyy*t42+
     & a2j2sy*t148+2*a2j2ry*t86+2*a2j2syy*t152+3*a2j2ryy*t158+a2j2sy*
     & t163
       t167 = t1*a2j2sy
       t178 = a2j2sy*t60
       t179 = t58*a2j2sy
       t181 = a2j2ryy*t1
       t182 = t178+4*t179+t181
       t183 = a2j2ry*t182
       t184 = a2j2ryy*a2j2ry
       t186 = 3*t184*t1
       t187 = a2j2sy*t65
       t189 = 3*a2j2sy*t56
       t190 = t183+t186+t187+t189
       t195 = a2j2sy*t74+7*t179+t181+t178
       t197 = t186+a2j2sy*t76+t187+t183+a2j2ry*t195+t189
       t203 = a2j2sy*t10
       t204 = t8*a2j2sy
       t206 = a2j2syyy*t1
       t207 = t203+4*t204+t206
       t210 = a2j2sy*t37
       t212 = 7*t204
       t213 = t210+a2j2sy*t17+t212+t206+t203
       t215 = t212+t206+t203+t210
       t245 = 2*a2j2ryy*t37+a2j2sy*t94+3*a2j2ryy*t10+a2j2ryyyy*t1+2*
     & t21*a2j2ry+a2j2ryy*t17+9*t106*a2j2syy+a2j2ry*t27+a2j2sy*t116+3*
     & a2j2syy*t158+a2j2syy*t110+a2j2ry*t23+a2j2sy*t90+3*a2j2syyy*t60+
     & a2j2syyy*t74+2*a2j2syy*t137+a2j2ry*t34+a2j2sy*t122+a2j2ry*t30
       t249 = a2j2ry*t1*a2j2syy
       t251 = a2j2sy*t182
       t253 = a2j2ryy*t167
       t254 = 12*t249+t251+a2j2sy*t195+t253
       t262 = 6*t249+t253+t251
       t266 = 6*t2*a2j2sy*a2j2ry*uu2rsssss+(3*a2j2syy*t10+9*t7*a2j2syy+
     & a2j2syy*t17+a2j2sy*t23+a2j2sy*t27+a2j2sy*t30+a2j2sy*t34+
     & a2j2syyyy*t1+2*a2j2syy*t37)*uu2sss+t43*t42*uu2rrrrrr+15*
     & a2j2syy*t2*uu2sssss+t2*t1*uu2ssssss+(18*a2j2ryy*t51*a2j2sy+
     & a2j2syy*t43+a2j2ry*t70+a2j2ry*t78)*uu2rrrrs+t165*uu2rrs+20*
     & t167*t51*uu2rrrsss+(6*a2j2ryy*t42*t1+a2j2sy*t78+a2j2sy*t70+4*
     & t67*a2j2sy+a2j2ry*t190+a2j2ry*t197)*uu2rrrss+(12*t8*t1+a2j2sy*
     & t207+a2j2syyy*t167+a2j2sy*t213+a2j2sy*t215)*uu2ssss+t245*
     & uu2rss+(a2j2sy*t190+a2j2ry*t254+a2j2sy*t197+6*t56*t1+4*t184*
     & t167+a2j2ry*t262)*uu2rrsss
       t269 = a2j2ryyy*t42
       t270 = a2j2ry*t152
       t271 = t97*a2j2ry
       t272 = 7*t271
       t274 = a2j2ry*t99
       t275 = t269+t270+t272+a2j2ry*t133+t274
       t280 = 2*t106*a2j2ry
       t281 = a2j2syyy*t42
       t282 = t58*a2j2ryy
       t285 = 2*a2j2ryy*t60
       t286 = a2j2sy*t99
       t287 = a2j2ry*t158
       t288 = t280+t281+4*t282+t285+t286+t287
       t291 = t274+t269+4*t271
       t293 = 7*t282
       t294 = a2j2ryy*t74
       t295 = a2j2ry*t137
       t297 = a2j2sy*t152
       t299 = t293+t286+t287+t294+t281+t295+a2j2sy*t133+t297+t280+
     & a2j2ry*t110+t285
       t302 = t280+t287+t295+t293+t286+t281+t285+t294+t297
       t306 = t272+t269+t270+t274
       t309 = 12*t108*t42+a2j2sy*t275+2*a2j2ryy*t65+a2j2ry*t288+a2j2sy*
     & t291+a2j2ry*t299+a2j2syyy*t51+a2j2ry*t302+3*t269*a2j2sy+a2j2sy*
     & t306+a2j2ryy*t76
       t313 = a2j2ryyy**2
       t319 = a2j2syyy**2
       t333 = a2j2ry*t10
       t335 = 2*t7*a2j2ry
       t336 = a2j2ryyy*t1
       t338 = 2*a2j2syy*t60
       t339 = a2j2sy*t108
       t341 = a2j2sy*t158
       t342 = t333+t335+t336+t338+4*t339+t341
       t344 = a2j2ry*t37
       t345 = a2j2syy*t74
       t346 = 7*t339
       t347 = a2j2sy*t137
       t348 = t335+t344+t345+t333+t338+t341+t346+t336+t347
       t362 = t346+t347+t338+a2j2sy*t110+a2j2ry*t17+t333+t344+t345+
     & t336+t335+t341
       t365 = 3*t281*a2j2sy+a2j2ry*t342+a2j2ry*t348+a2j2ryy*t195+
     & a2j2sy*t302+a2j2sy*t288+2*a2j2syy*t65+2*a2j2ryy*t182+a2j2sy*
     & t299+3*t96*t1+a2j2ry*t362+a2j2syy*t76
       t389 = a2j2sy*t342+a2j2ry*t207+12*t181*a2j2syy+a2j2ryyy*t167+3*
     & t104*t1+a2j2sy*t362+a2j2ry*t213+a2j2syy*t195+a2j2sy*t348+
     & a2j2ry*t215+2*a2j2syy*t182
       t426 = t309*uu2rrrs+(15*a2j2ryy*a2j2ryyyy+10*t313)*uu2rr+(15*
     & a2j2syy*a2j2syyyy+10*t319)*uu2ss+(15*a2j2syy*a2j2ryyyy+20*
     & a2j2syyy*a2j2ryyy+15*a2j2syyyy*a2j2ryy)*uu2rs+t365*uu2rrss+(
     & a2j2sy*t254+18*a2j2ry*t167*a2j2syy+a2j2sy*t262+a2j2ryy*t2)*
     & uu2rssss+t389*uu2rsss+(9*t126*a2j2ry+a2j2ry*t148+a2j2ry*t128+
     & a2j2ry*t142+a2j2ryyyy*t42+2*a2j2ryy*t152+3*a2j2ryy*t99+a2j2ryy*
     & t133+a2j2ry*t163)*uu2rrr+15*a2j2ryy*t43*uu2rrrrr+15*t2*t42*
     & uu2rrssss+(a2j2ryyy*t51+a2j2ry*t291+a2j2ry*t275+12*t97*t42+
     & a2j2ry*t306)*uu2rrrr+6*a2j2sy*t43*a2j2ry*uu2rrrrrs+15*t1*t43*
     & uu2rrrrss
       uu2yyyyyy2 = t266+t426
       t1 = a2j2sx**2
       t3 = a2j2sxxx*a2j2sx
       t4 = a2j2sxx**2
       t5 = t3+t4
       t6 = 3*t5
       t9 = a2j2sxxx*a2j2rx
       t11 = a2j2sx*a2j2rxxx
       t13 = a2j2sxx*a2j2rxx
       t14 = 6*t13
       t15 = 4*t9+4*t11+t14
       t20 = 2*t11+2*t9+4*t13
       t23 = a2j2sxx*a2j2rxxx
       t24 = 10*t23
       t25 = a2j2sxxx*a2j2rxx
       t26 = 10*t25
       t27 = a2j2sxxxx*a2j2rx
       t29 = a2j2sx*a2j2rxxxx
       t31 = t24+t26+4*t27+4*t29
       t33 = a2j2sx*a2j2rxx
       t34 = a2j2sxx*a2j2rx
       t35 = t33+t34
       t36 = 2*t35
       t43 = 6*t23+2*t29+6*t25+2*t27
       t47 = 5*t27+t24+5*t29+t26
       t51 = a2j2sxxxx*a2j2sx
       t53 = a2j2sxx*a2j2sxxx
       t55 = 2*t51+6*t53
       t59 = 2*t5
       t63 = 10*t53
       t64 = 5*t51+t63
       t66 = 3*t35
       t69 = 4*t51+t63
       t73 = t14+3*t11+3*t9
       t80 = 3*t29+9*t23+9*t25+3*t27
       t84 = 4*t3+3*t4
       t88 = 9*t53+3*t51
       t90 = a2j2rxxxx*t1+2*a2j2rxx*t6+a2j2sxx*t15+3*a2j2sxx*t20+
     & a2j2sx*t31+3*a2j2sxxx*t36+a2j2sx*t43+a2j2sx*t47+9*t23*a2j2sx+
     & a2j2rx*t55+2*t51*a2j2rx+3*a2j2rxx*t59+a2j2rx*t64+a2j2sxxx*t66+
     & a2j2rx*t69+2*a2j2sxx*t73+a2j2sx*t80+a2j2rxx*t84+a2j2rx*t88
       t92 = t1*a2j2sx
       t96 = a2j2rxx*t92
       t97 = a2j2sx*t36
       t98 = t34*a2j2sx
       t100 = a2j2rxx*t1
       t101 = t97+4*t98+t100
       t102 = a2j2sx*t101
       t103 = t34*t1
       t105 = t96+t102+6*t103
       t107 = t1**2
       t112 = a2j2sx*t66+7*t98+t100+t97
       t114 = t96+12*t103+t102+a2j2sx*t112
       t118 = a2j2rx**2
       t121 = a2j2rxxx*t118
       t122 = a2j2rxx**2
       t123 = t122*a2j2rx
       t125 = a2j2rx*a2j2rxxx
       t126 = t125+t122
       t127 = 2*t126
       t128 = a2j2rx*t127
       t129 = t121+4*t123+t128
       t131 = 7*t123
       t132 = 3*t126
       t133 = a2j2rx*t132
       t134 = t131+t128+t121+t133
       t136 = a2j2sxx*t118
       t137 = a2j2rx*t36
       t139 = a2j2sx*a2j2rx*a2j2rxx
       t141 = t136+t137+4*t139
       t148 = 7*t139+t136+a2j2rx*t66+t137
       t151 = 2*a2j2rxx*t36
       t152 = t34*a2j2rxx
       t154 = a2j2rx*t20
       t155 = a2j2sxxx*t118
       t156 = a2j2sx*t127
       t158 = 2*t11*a2j2rx
       t159 = t151+4*t152+t154+t155+t156+t158
       t161 = t118*a2j2rx
       t163 = a2j2rxx*t66
       t164 = a2j2sx*t132
       t165 = 7*t152
       t166 = a2j2rx*t73
       t167 = t163+t156+t151+t154+t158+t164+t165+t155+t166
       t172 = 4*t125+3*t122
       t174 = t155+t158+t164+t154+a2j2rx*t15+a2j2sx*t172+t165+t163+
     & t156+t166+t151
       t177 = a2j2rx*t172+t131+t128+t133+t121
       t179 = 12*t13*t118+a2j2sx*t129+a2j2sx*t134+2*a2j2rxx*t141+3*
     & t121*a2j2sx+a2j2rxx*t148+a2j2rx*t159+a2j2sxxx*t161+a2j2rx*t167+
     & a2j2rx*t174+a2j2sx*t177
       t181 = t118**2
       t190 = a2j2rxxx**2
       t204 = a2j2sxxx**2
       t215 = a2j2rxx*t118
       t216 = t215*a2j2sx
       t218 = a2j2sxx*t161
       t219 = a2j2rx*t141
       t220 = 6*t216+t218+t219
       t224 = 12*t216+t219+t218+a2j2rx*t148
       t231 = t90*vv2rss+(18*a2j2rx*t92*a2j2sxx+a2j2sx*t105+a2j2rxx*
     & t107+a2j2sx*t114)*vv2rssss+t179*vv2rrrs+15*a2j2rxx*t181*
     & vv2rrrrr+15*t181*t1*vv2rrrrss+(15*a2j2rxx*a2j2rxxxx+10*t190)*
     & vv2rr+(15*a2j2sxxxx*a2j2rxx+20*a2j2sxxx*a2j2rxxx+15*a2j2sxx*
     & a2j2rxxxx)*vv2rs+(15*a2j2sxxxx*a2j2sxx+10*t204)*vv2ss+t181*
     & t118*vv2rrrrrr+t107*t1*vv2ssssss+(18*t33*t161+a2j2sxx*t181+
     & a2j2rx*t220+a2j2rx*t224)*vv2rrrrs+15*t118*t107*vv2rrssss
       t246 = t4*a2j2sx
       t247 = 7*t246
       t248 = a2j2sx*t59
       t249 = a2j2sx*t6
       t250 = a2j2sxxx*t1
       t251 = a2j2sx*t84+t247+t248+t249+t250
       t254 = t250+4*t246+t248
       t256 = t247+t248+t249+t250
       t263 = a2j2rx*a2j2rxxxx
       t265 = a2j2rxx*a2j2rxxx
       t267 = 2*t263+6*t265
       t272 = 10*t265
       t274 = t272+5*t263
       t277 = t272+4*t263
       t293 = 9*t265+3*t263
       t299 = 3*a2j2sxx*t127+a2j2sxx*t172+a2j2sx*t267+9*t25*a2j2rx+
     & a2j2rx*t31+a2j2sx*t274+a2j2sx*t277+3*a2j2rxx*t20+3*a2j2rxxx*
     & t36+2*a2j2sxx*t132+a2j2sxxxx*t118+a2j2rxxx*t66+a2j2rx*t43+
     & a2j2rxx*t15+2*a2j2rxx*t73+a2j2sx*t293+2*t29*a2j2rx+a2j2rx*t80+
     & a2j2rx*t47
       t304 = t33*a2j2sxx
       t306 = a2j2rxxx*t1
       t307 = a2j2rx*t59
       t309 = 2*a2j2sxx*t36
       t311 = 2*t3*a2j2rx
       t312 = a2j2sx*t20
       t313 = 4*t304+t306+t307+t309+t311+t312
       t316 = 7*t304
       t319 = a2j2rx*t6
       t320 = a2j2sxx*t66
       t321 = a2j2sx*t73
       t322 = t316+a2j2rx*t84+a2j2sx*t15+t319+t306+t307+t309+t312+t320+
     & t321+t311
       t326 = t312+t316+t311+t320+t306+t319+t307+t321+t309
       t333 = a2j2rx*t251+12*t100*a2j2sxx+a2j2sx*t313+a2j2rxxx*t92+
     & a2j2sx*t322+3*t9*t1+a2j2sx*t326+2*a2j2sxx*t101+a2j2rx*t256+
     & a2j2sxx*t112+a2j2rx*t254
       t337 = a2j2rxx*a2j2rx
       t341 = 3*t337*t1
       t342 = a2j2sx*t141
       t344 = 3*t136*a2j2sx
       t345 = a2j2rx*t101
       t346 = t341+t342+t344+t345
       t352 = t341+t345+a2j2rx*t112+a2j2sx*t148+t344+t342
       t422 = a2j2sx*t167+a2j2rxx*t112+a2j2sx*t174+a2j2rx*t313+a2j2sxx*
     & t148+2*a2j2sxx*t141+a2j2rx*t322+3*t155*a2j2sx+a2j2sx*t159+
     & a2j2rx*t326+3*t125*t1+2*a2j2rxx*t101
       t424 = 20*t161*t92*vv2rrrsss+15*t107*a2j2sxx*vv2sssss+6*a2j2rx*
     & t107*a2j2sx*vv2rsssss+(a2j2sxxx*t92+12*t4*t1+a2j2sx*t251+
     & a2j2sx*t254+a2j2sx*t256)*vv2ssss+t299*vv2rrs+t333*vv2rsss+(
     & a2j2rx*t105+a2j2rx*t114+4*t337*t92+a2j2sx*t346+6*t136*t1+
     & a2j2sx*t352)*vv2rrsss+(a2j2sx*t224+6*t215*t1+a2j2sx*t220+4*
     & t218*a2j2sx+a2j2rx*t346+a2j2rx*t352)*vv2rrrss+(a2j2sxx*t84+9*
     & t3*a2j2sxx+2*a2j2sxx*t6+a2j2sx*t64+a2j2sxxxx*t1+a2j2sx*t55+
     & a2j2sx*t69+3*a2j2sxx*t59+a2j2sx*t88)*vv2sss+(a2j2rxx*t172+
     & a2j2rx*t267+a2j2rx*t274+9*t265*a2j2rx+3*a2j2rxx*t127+2*a2j2rxx*
     & t132+a2j2rx*t277+a2j2rxxxx*t118+a2j2rx*t293)*vv2rrr+6*a2j2sx*
     & t181*a2j2rx*vv2rrrrrs+(a2j2rx*t129+a2j2rx*t177+12*t122*t118+
     & a2j2rxxx*t161+a2j2rx*t134)*vv2rrrr+t422*vv2rrss
       vv2xxxxxx2 = t231+t424
       t1 = a2j2sx*a2j2rxy
       t4 = a2j2rx**2
       t5 = a2j2sxy*t4
       t6 = a2j2sxy*a2j2rx
       t8 = 2*t1+2*t6
       t10 = 4*t1*a2j2rx+t5+a2j2rx*t8
       t11 = a2j2sx*t10
       t13 = 3*t5*a2j2sx
       t15 = a2j2sx**2
       t17 = 3*a2j2rxy*a2j2rx*t15
       t20 = a2j2rxy*t15
       t22 = 4*a2j2sx*t6+t20+a2j2sx*t8
       t23 = a2j2rx*t22
       t24 = t11+t13+t17+t23
       t27 = 6*t6*t15
       t28 = a2j2sx*t15
       t29 = a2j2rxy*t28
       t30 = a2j2sx*t22
       t31 = t27+t29+t30
       t33 = a2j2syy*t4
       t36 = a2j2sxx*a2j2rx
       t38 = t36+a2j2sx*a2j2rxx
       t39 = 3*t38
       t44 = 2*t38
       t46 = a2j2sx*t39+7*a2j2sx*t36+a2j2rxx*t15+a2j2sx*t44
       t51 = a2j2sy*t46+t27+t30+t29+6*a2j2ry*t15*a2j2sxx
       t62 = 7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t4+a2j2rx*t39+t44*a2j2rx
       t65 = t13+t23+a2j2sy*t62+a2j2ry*t46+t17+t11
       t74 = 4*t28*a2j2sxy+6*a2j2sy*t15*a2j2sxx
       t84 = t15**2
       t88 = a2j2rxy**2
       t91 = a2j2rxxy*t4
       t92 = a2j2rxy*a2j2rxx
       t94 = 7*t92*a2j2rx
       t96 = a2j2rx*a2j2rxxy+t92
       t97 = 3*t96
       t98 = a2j2rx*t97
       t99 = 2*t96
       t100 = a2j2rx*t99
       t103 = a2j2rxx**2
       t105 = 4*a2j2rx*a2j2rxxx+3*t103
       t107 = t91+t94+t98+t100+a2j2ry*t105
       t109 = t94+t100+t98+t91
       t111 = t4*a2j2rx
       t113 = a2j2ryy*t4
       t116 = a2j2rxyy*t4
       t117 = a2j2rx*a2j2rxyy
       t119 = 2*t117+2*t88
       t123 = t116+a2j2rx*t119+4*a2j2rx*t88
       t134 = a2j2sxx*t8
       t136 = 5*t1*a2j2sxx
       t137 = a2j2sxy*t39
       t138 = a2j2sx*a2j2rxxy
       t139 = a2j2sxx*a2j2rxy
       t141 = a2j2sxy*a2j2rxx
       t142 = t138+t139+a2j2sxxy*a2j2rx+t141
       t143 = 3*t142
       t144 = a2j2sx*t143
       t145 = a2j2sxxy*a2j2sx
       t147 = 2*t145*a2j2rx
       t149 = a2j2sxy*a2j2sxx+t145
       t150 = 3*t149
       t151 = a2j2rx*t150
       t152 = a2j2sxy*t44
       t159 = 4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+6*a2j2sxx*a2j2rxx
       t161 = 2*t142
       t162 = a2j2sx*t161
       t163 = 2*t149
       t164 = a2j2rx*t163
       t167 = a2j2sxx**2
       t169 = 4*a2j2sxxx*a2j2sx+3*t167
       t172 = 2*t141*a2j2sx
       t173 = a2j2rxxy*t15
       t174 = t134+t136+t137+t144+t147+t151+t152+a2j2sy*t159+t162+t164+
     & a2j2ry*t169+t172+t173
       t176 = a2j2sxy*a2j2rxy
       t179 = a2j2sxyy*t4
       t181 = a2j2sx*a2j2rxyy
       t183 = a2j2sxyy*a2j2rx
       t186 = 2*t181+2*t183+4*t176
       t192 = 4*a2j2rx*t176+t179+a2j2sx*t119+a2j2rx*t186+2*a2j2rx*t181+
     & 2*a2j2rxy*t8
       t194 = a2j2sxxy*t4
       t196 = 5*t141*a2j2rx
       t199 = 2*t138*a2j2rx
       t200 = a2j2rx*t143
       t201 = a2j2rxy*t44
       t202 = a2j2rxx*t8
       t203 = t97*a2j2sx
       t204 = a2j2rx*t161
       t206 = 2*t139*a2j2rx
       t207 = a2j2rxy*t39
       t208 = a2j2sx*t99
       t210 = t194+t196+a2j2sy*t105+t199+t200+t201+t202+t203+t204+t206+
     & t207+t208+a2j2ry*t159
       t214 = a2j2sx*a2j2sxyy
       t215 = a2j2sxy**2
       t217 = 2*t214+2*t215
       t227 = a2j2rx*t217+a2j2sx*t186+4*a2j2sx*t176+a2j2rxyy*t15+2*
     & t214*a2j2rx+2*a2j2sxy*t8
       t233 = t202+t204+t206+t203+t194+t200+t199+t208+t196+t207+t201
       t236 = t151+t162+t137+t164+t147+t136+t172+t134+t144+t152+t173
       t241 = a2j2ry*t174+a2j2sx*t192+a2j2sy*t210+2*a2j2rxy*t22+a2j2rx*
     & t227+3*t179*a2j2sx+3*t117*t15+a2j2sy*t233+a2j2ryy*t46+a2j2ry*
     & t236+a2j2syy*t62+2*a2j2sxy*t10
       t259 = 2*a2j2rxy*t10+a2j2ry*t233+a2j2ry*t210+6*t33*a2j2rxx+6*
     & t176*t4+a2j2sy*t107+a2j2sx*t123+3*t116*a2j2sx+a2j2sy*t109+
     & a2j2rx*t192+a2j2sxyy*t111+a2j2ryy*t62
       t261 = a2j2ry**2
       t262 = t4**2
       t265 = a2j2sy**2
       t268 = a2j2sx*t150
       t269 = a2j2sxxy*t15
       t272 = 7*a2j2sxy*a2j2sx*a2j2sxx
       t273 = a2j2sx*t163
       t275 = t268+t269+t272+t273+a2j2sy*t169
       t287 = a2j2sxyy*t15+4*t215*a2j2sx+a2j2sx*t217
       t289 = t272+t269+t268+t273
       t295 = a2j2sxy*a2j2sxxy
       t298 = a2j2sxyy*a2j2sxx
       t300 = a2j2sxxyy*a2j2sx
       t303 = 2*t298+2*t300+4*t295
       t309 = 3*t298+3*t300+6*t295
       t312 = 4*a2j2sxxx*a2j2sxy
       t314 = 4*a2j2sxxxy*a2j2sx
       t316 = 6*a2j2sxx*a2j2sxxy
       t318 = t312+t314+t316+a2j2sy*a2j2sxxxx
       t320 = t314+t312+t316
       t328 = 5*t214*a2j2sxx+4*t295*a2j2sx+a2j2sx*t303+a2j2sxx*t217+
     & a2j2sx*t309+a2j2sy*t318+a2j2sy*t320+2*a2j2sxy*t150+a2j2sxxyy*
     & t15+2*a2j2sxy*t163+a2j2syy*t169
       t330 = a2j2ry*a2j2sy
       t336 = a2j2ry*t84+4*a2j2sy*a2j2rx*t28
       t340 = a2j2sxyy*a2j2rxx
       t342 = a2j2sxxy*a2j2rxy
       t344 = a2j2sxy*a2j2rxxy
       t346 = a2j2sx*a2j2rxxyy
       t348 = a2j2sxxyy*a2j2rx
       t350 = a2j2sxx*a2j2rxyy
       t352 = 3*t340+6*t342+6*t344+3*t346+3*t348+3*t350
       t366 = 2*t346+2*t340+4*t344+4*t342+2*t350+2*t348
       t373 = 4*a2j2sxxx*a2j2rxy
       t376 = 4*a2j2sxxxy*a2j2rx
       t378 = 4*a2j2sxy*a2j2rxxx
       t380 = 4*a2j2sx*a2j2rxxxy
       t382 = 6*a2j2sxx*a2j2rxxy
       t385 = 6*a2j2sxxy*a2j2rxx
       t386 = t373+a2j2ry*a2j2sxxxx+t376+t378+t380+t382+a2j2sy*
     & a2j2rxxxx+t385
       t389 = a2j2sx*t352+a2j2syy*t159+a2j2ry*t318+2*a2j2sxy*t161+2*
     & a2j2rxy*t150+a2j2sx*t366+a2j2ry*t320+a2j2sxyy*t44+2*t300*
     & a2j2rx+a2j2sy*t386+a2j2rxx*t217
       t392 = t376+t373+t380+t382+t385+t378
       t408 = 2*a2j2sxy*t143+a2j2sy*t392+a2j2rx*t309+5*t350*a2j2sx+
     & a2j2sxx*t186+a2j2sxyy*t39+2*a2j2rxy*t163+a2j2ryy*t169+a2j2rx*
     & t303+2*a2j2sxxy*t8+a2j2rxxyy*t15+4*t344*a2j2sx
       t411 = (a2j2sy*t24+a2j2ry*t31+6*t33*t15+a2j2ry*t51+4*a2j2ryy*
     & a2j2rx*t28+a2j2sy*t65)*vv2rrsss+(a2j2ry*t74+a2j2sy*t51+a2j2sy*
     & t31+4*a2j2ry*t28*a2j2sxy+4*a2j2syy*a2j2rx*t28+a2j2ryy*t84)*
     & vv2rssss+(6*t88*t4+a2j2ry*t107+a2j2ry*t109+a2j2rxyy*t111+6*
     & t113*a2j2rxx+a2j2rx*t123)*vv2rrrr+(a2j2sy*t74+a2j2syy*t84+4*
     & a2j2sy*t28*a2j2sxy)*vv2sssss+t241*vv2rrss+t259*vv2rrrs+t261*
     & t262*vv2rrrrrr+t265*t84*vv2ssssss+(a2j2sy*t275+6*a2j2syy*t15*
     & a2j2sxx+6*t215*t15+a2j2sxyy*t28+a2j2sx*t287+a2j2sy*t289)*
     & vv2ssss+t328*vv2sss+(t330*t84+a2j2sy*t336)*vv2rsssss+(t389+
     & t408)*vv2rss
       t413 = a2j2ry*t111
       t418 = a2j2ry*t4
       t421 = 4*t111*a2j2rxy+6*t418*a2j2rxx
       t425 = a2j2sxxy**2
       t439 = 4*t413*a2j2sx+a2j2sy*t262
       t445 = a2j2sy*t111
       t450 = 4*t445*a2j2sx+6*t418*t15
       t457 = a2j2sy*t4
       t460 = 4*a2j2ry*a2j2rx*t28+6*t457*t15
       t470 = 4*a2j2rxxx*a2j2rxy
       t472 = 6*a2j2rxxy*a2j2rxx
       t474 = 4*a2j2rxxxy*a2j2rx
       t475 = a2j2ry*a2j2rxxxx+t470+t472+t474
       t487 = t474+t470+t472
       t489 = a2j2ry*t392+5*t340*a2j2rx+a2j2sy*t475+a2j2rx*t352+2*
     & a2j2rxxy*t8+a2j2rxyy*t44+a2j2rxx*t186+a2j2sxx*t119+2*a2j2sxy*
     & t97+2*a2j2rxy*t161+a2j2sy*t487
       t495 = a2j2rx*a2j2rxxyy
       t497 = a2j2rxx*a2j2rxyy
       t499 = a2j2rxy*a2j2rxxy
       t501 = 3*t495+3*t497+6*t499
       t510 = 2*t495+4*t499+2*t497
       t517 = a2j2ryy*t159+a2j2syy*t105+a2j2ry*t386+2*a2j2sxy*t99+
     & a2j2sx*t501+a2j2sxxyy*t4+4*t342*a2j2rx+a2j2rx*t366+a2j2sx*t510+
     & 2*t346*a2j2rx+2*a2j2rxy*t143+a2j2rxyy*t39
       t553 = 5*t497*a2j2rx+a2j2ry*t475+a2j2rx*t510+a2j2ry*t487+a2j2rx*
     & t501+a2j2rxxyy*t4+2*a2j2rxy*t97+4*t499*a2j2rx+a2j2ryy*t105+2*
     & a2j2rxy*t99+a2j2rxx*t119
       t564 = a2j2rx*t10
       t566 = 6*t1*t4
       t567 = a2j2sxy*t111
       t569 = 6*t457*a2j2rxx+t564+t566+t567+a2j2ry*t62
       t571 = t567+t566+t564
       t607 = a2j2rxyy*t28+3*t183*t15+a2j2ry*t289+a2j2sy*t174+a2j2rx*
     & t287+a2j2sy*t236+6*a2j2ryy*t15*a2j2sxx+2*a2j2sxy*t22+a2j2syy*
     & t46+a2j2sx*t227+6*t20*a2j2sxy+a2j2ry*t275
       t616 = a2j2rxxy**2
       t620 = (a2j2ryy*t262+4*t413*a2j2rxy+a2j2ry*t421)*vv2rrrrr+(6*
     & t425+8*a2j2sxxxy*a2j2sxy+a2j2syy*a2j2sxxxx+4*a2j2sxyy*a2j2sxxx+
     & 6*a2j2sxxyy*a2j2sxx)*vv2ss+(a2j2ry*t439+t330*t262)*vv2rrrrrs+(
     & a2j2sy*t439+a2j2ry*t450)*vv2rrrrss+(a2j2ry*t460+a2j2sy*t450)*
     & vv2rrrsss+(t489+t517)*vv2rrs+(8*a2j2sxxxy*a2j2rxy+4*a2j2sxxx*
     & a2j2rxyy+4*a2j2sxyy*a2j2rxxx+6*a2j2sxxyy*a2j2rxx+12*a2j2sxxy*
     & a2j2rxxy+a2j2syy*a2j2rxxxx+6*a2j2sxx*a2j2rxxyy+a2j2ryy*
     & a2j2sxxxx+8*a2j2sxy*a2j2rxxxy)*vv2rs+t553*vv2rrr+(a2j2ry*t336+
     & a2j2sy*t460)*vv2rrssss+(4*t445*a2j2rxy+a2j2syy*t262+a2j2ry*
     & t569+a2j2ry*t571+a2j2sy*t421+4*a2j2ryy*t111*a2j2sx)*vv2rrrrs+(
     & a2j2sy*t569+a2j2ry*t65+4*a2j2syy*t111*a2j2sx+a2j2ry*t24+a2j2sy*
     & t571+6*t113*t15)*vv2rrrss+t607*vv2rsss+(8*a2j2rxy*a2j2rxxxy+4*
     & a2j2rxyy*a2j2rxxx+6*a2j2rxx*a2j2rxxyy+a2j2ryy*a2j2rxxxx+6*t616)
     & *vv2rr
       vv2xxxxyy2 = t411+t620
       t1 = a2j2sy**2
       t2 = t1**2
       t3 = a2j2sx**2
       t6 = a2j2ry**2
       t7 = t6**2
       t8 = a2j2rx**2
       t13 = 2*a2j2sy*a2j2sxy*a2j2sx
       t17 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
       t19 = a2j2syy*t3
       t20 = t13+a2j2sy*t17+t19
       t23 = a2j2syy*a2j2sy*t3
       t24 = t19+t13
       t25 = a2j2sy*t24
       t26 = a2j2sy*t20+t23+t25
       t30 = t23+t25
       t34 = a2j2sx*a2j2rxy
       t35 = a2j2sxy*a2j2rx
       t37 = 2*t34+2*t35
       t38 = a2j2sy*t37
       t41 = 2*a2j2syy*a2j2sx*a2j2rx
       t44 = 2*a2j2ry*a2j2sxy*a2j2sx
       t45 = a2j2ryy*t3
       t46 = t38+t41+t44+t45
       t49 = a2j2syyy*t3
       t52 = 4*a2j2syy*a2j2sxy*a2j2sx
       t53 = a2j2sx*a2j2sxyy
       t54 = a2j2sxy**2
       t56 = 2*t53+2*t54
       t57 = a2j2sy*t56
       t58 = t49+t52+t57
       t64 = a2j2ry*t3+2*a2j2sy*a2j2sx*a2j2rx
       t66 = a2j2sy*a2j2sxxy
       t67 = 2*t53
       t68 = a2j2syy*a2j2sxx
       t69 = 2*t54
       t70 = t66+t67+t68+t69
       t71 = a2j2sy*t70
       t72 = a2j2syy*t17
       t73 = t52+t71+t72+t57+t49
       t76 = t69+2*t66+t68+t67
       t78 = t71+t49+t52+t72+t57+a2j2sy*t76
       t82 = a2j2ry*t56
       t85 = 2*a2j2syyy*a2j2sx*a2j2rx
       t86 = a2j2ryyy*t3
       t88 = 2*a2j2syy*t37
       t91 = 4*a2j2ryy*a2j2sxy*a2j2sx
       t93 = 2*a2j2sx*a2j2rxyy
       t95 = 2*a2j2sxyy*a2j2rx
       t97 = 4*a2j2sxy*a2j2rxy
       t98 = t93+t95+t97
       t99 = a2j2sy*t98
       t100 = t82+t85+t86+t88+t91+t99
       t106 = a2j2ry*a2j2sxx+2*t34+2*t35+a2j2sy*a2j2rxx
       t109 = a2j2sy*t106+a2j2ry*t17+t45+t44+t41+t38
       t114 = a2j2sy*a2j2rxxy
       t115 = a2j2ry*a2j2sxxy
       t116 = a2j2ryy*a2j2sxx
       t117 = a2j2syy*a2j2rxx
       t118 = t114+t115+t116+t97+t95+t117+t93
       t119 = a2j2sy*t118
       t120 = a2j2ryy*t17
       t121 = a2j2syy*t106
       t122 = a2j2ry*t70
       t123 = t119+t91+t120+t85+t121+t82+t88+t99+t122+t86
       t128 = t97+2*t115+t116+2*t114+t95+t117+t93
       t130 = t121+a2j2ry*t76+a2j2sy*t128+t86+t119+t91+t99+t122+t85+
     & t88+t120+t82
       t132 = 2*a2j2syy*t46+a2j2ry*t58+a2j2syyy*t64+a2j2ry*t73+a2j2ry*
     & t78+2*a2j2ryy*t24+a2j2sy*t100+a2j2syy*t109+a2j2ryy*t20+
     & a2j2ryyy*a2j2sy*t3+a2j2sy*t123+a2j2sy*t130
       t134 = a2j2ryyy*a2j2rxx
       t135 = a2j2ry*a2j2rxxyy
       t138 = 6*a2j2rxy*a2j2rxyy
       t140 = 2*a2j2rx*a2j2rxyyy
       t141 = a2j2ryy*a2j2rxxy
       t142 = 3*t141
       t143 = t134+3*t135+t138+t140+t142
       t146 = t140+t138+2*t135+t134+t142
       t149 = t140+t138
       t151 = a2j2rx*a2j2rxyy
       t152 = 2*t151
       t153 = a2j2ryy*a2j2rxx
       t154 = a2j2ry*a2j2rxxy
       t156 = a2j2rxy**2
       t157 = 2*t156
       t158 = t152+t153+2*t154+t157
       t161 = 2*t151+2*t156
       t164 = t153+t152+t154+t157
       t171 = t134+t138+t135+t140+2*t141
       t176 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
       t184 = 2*a2j2ry*a2j2rxy*a2j2rx
       t186 = a2j2ryy*t8
       t187 = t184+a2j2ry*t176+t186
       t189 = a2j2ry*t161
       t190 = a2j2ryyy*t8
       t193 = 4*a2j2ryy*a2j2rxy*a2j2rx
       t194 = t189+t190+t193
       t196 = a2j2ry*t164
       t197 = a2j2ryy*t176
       t198 = t193+t189+t196+t190+t197
       t200 = t184+t186
       t204 = t193+t196+t190+a2j2ry*t158+t197+t189
       t209 = a2j2ry*a2j2sy
       t211 = a2j2sy*t64+t209*t3
       t215 = a2j2sy*t211+a2j2ry*t1*t3
       t226 = a2j2sy*t8+2*a2j2ry*a2j2sx*a2j2rx
       t228 = a2j2syy*t8
       t229 = a2j2ry*t37
       t232 = 2*a2j2ryy*a2j2sx*a2j2rx
       t235 = 2*a2j2sy*a2j2rxy*a2j2rx
       t236 = t228+t229+t232+t235
       t241 = 2*a2j2ryy*t37
       t242 = a2j2syy*t176
       t245 = 4*a2j2syy*a2j2rxy*a2j2rx
       t246 = a2j2ry*t118
       t247 = a2j2sy*t161
       t248 = a2j2sy*t164
       t251 = 2*a2j2ryyy*a2j2sx*a2j2rx
       t252 = a2j2ry*t98
       t253 = a2j2ryy*t106
       t254 = a2j2syyy*t8
       t255 = t241+t242+t245+t246+t247+t248+t251+t252+t253+t254
       t261 = a2j2sy*t158+t251+t242+t241+t254+a2j2ry*t128+t245+t248+
     & t253+t247+t246+t252
       t263 = t251+t254+t245+t241+t247+t252
       t269 = a2j2sy*t176+t235+t232+t228+a2j2ry*t106+t229
       t274 = a2j2ryyy*t226+2*a2j2ryy*t236+a2j2sy*t204+a2j2ry*t255+2*
     & a2j2syy*t200+a2j2ry*t261+a2j2ry*t263+a2j2sy*t194+a2j2sy*t198+
     & a2j2ryy*t269+a2j2syyy*a2j2ry*t8+a2j2syy*t187
       t288 = 2*a2j2sxyyy*a2j2rx
       t289 = a2j2syy*a2j2rxxy
       t290 = 3*t289
       t292 = 6*a2j2sxy*a2j2rxyy
       t293 = a2j2ry*a2j2sxxyy
       t295 = a2j2ryyy*a2j2sxx
       t296 = a2j2ryy*a2j2sxxy
       t297 = 3*t296
       t299 = 6*a2j2sxyy*a2j2rxy
       t301 = 2*a2j2sx*a2j2rxyyy
       t302 = a2j2syyy*a2j2rxx
       t303 = a2j2sy*a2j2rxxyy
       t305 = t288+t290+t292+2*t293+t295+t297+t299+t301+t302+2*t303
       t310 = t295+t303+t301+t288+t299+t292+2*t289+t293+2*t296+t302
       t321 = t295+3*t293+t290+t299+t302+t301+3*t303+t297+t292+t288
       t329 = t292+t301+t299+t288
       t331 = a2j2sy*t149+3*a2j2syy*t161+a2j2sy*t146+a2j2syyyy*t8+6*
     & a2j2syyy*a2j2rxy*a2j2rx+2*a2j2syy*t164+a2j2sy*t171+a2j2ry*t305+
     & a2j2sy*t143+a2j2ry*t310+a2j2syyy*t176+2*a2j2ryyyy*a2j2sx*
     & a2j2rx+a2j2syy*t158+3*a2j2ryy*t98+a2j2ry*t321+3*a2j2ryyy*t37+
     & a2j2ryy*t128+a2j2ryyy*t106+2*a2j2ryy*t118+a2j2ry*t329
       t335 = a2j2syy*t64
       t336 = a2j2sy*t46
       t337 = a2j2ry*t24
       t339 = a2j2ryy*a2j2sy*t3
       t340 = t335+t336+t337+t339
       t344 = t337+t336+t335+t339+a2j2sy*t109+a2j2ry*t20
       t383 = a2j2syyy*t226+a2j2sy*t261+a2j2ry*t100+a2j2ry*t123+a2j2sy*
     & t255+a2j2syy*t269+2*a2j2syy*t236+a2j2ryy*t109+a2j2sy*t263+
     & a2j2ryyy*t64+a2j2ry*t130+2*a2j2ryy*t46
       t385 = t2*t3*vv2ssssss+t7*t8*vv2rrrrrr+(a2j2sy*t26+a2j2syy*t1*
     & t3+a2j2sy*t30)*vv2sssss+t132*vv2rsss+(a2j2ry*t143+a2j2ry*t146+
     & a2j2ryyyy*t8+a2j2ry*t149+a2j2ryy*t158+3*a2j2ryy*t161+2*a2j2ryy*
     & t164+6*a2j2ryyy*a2j2rxy*a2j2rx+a2j2ry*t171+a2j2ryyy*t176)*
     & vv2rrr+(a2j2ryyy*a2j2ry*t8+a2j2ryy*t187+a2j2ry*t194+a2j2ry*
     & t198+2*a2j2ryy*t200+a2j2ry*t204)*vv2rrrr+(a2j2sy*t215+a2j2ry*
     & t1*a2j2sy*t3)*vv2rsssss+t274*vv2rrrs+t331*vv2rrs+(a2j2ryy*t1*
     & t3+a2j2sy*t340+a2j2sy*t344+a2j2syy*t211+a2j2ry*t30+a2j2ry*t26)*
     & vv2rssss+(8*a2j2sxy*a2j2rxyyy+12*a2j2sxyy*a2j2rxyy+a2j2syyyy*
     & a2j2rxx+8*a2j2sxyyy*a2j2rxy+6*a2j2syy*a2j2rxxyy+4*a2j2syyy*
     & a2j2rxxy+a2j2ryyyy*a2j2sxx+6*a2j2ryy*a2j2sxxyy+4*a2j2ryyy*
     & a2j2sxxy)*vv2rs+t383*vv2rrss
       t390 = a2j2ryy*a2j2ry*t8
       t391 = a2j2ry*t200
       t392 = a2j2ry*t187+t390+t391
       t394 = t391+t390
       t398 = a2j2sxyy**2
       t410 = a2j2ry*t46
       t411 = a2j2syy*t226
       t413 = a2j2sy*t236
       t415 = a2j2ryy*t64
       t416 = t410+t411+a2j2ry*t109+t413+a2j2sy*t269+t415
       t420 = a2j2sy*t226+a2j2ry*t64
       t423 = t413+t411+t415+t410
       t433 = t209*t8+a2j2ry*t226
       t437 = a2j2ry*t433+a2j2sy*t6*t8
       t461 = a2j2syy*a2j2sxxy
       t462 = 3*t461
       t463 = a2j2sy*a2j2sxxyy
       t466 = 2*a2j2sx*a2j2sxyyy
       t468 = 6*a2j2sxyy*a2j2sxy
       t469 = a2j2syyy*a2j2sxx
       t470 = t462+2*t463+t466+t468+t469
       t477 = t466+t468
       t482 = t468+2*t461+t463+t469+t466
       t491 = t462+t466+t469+3*t463+t468
       t495 = 3*a2j2syy*t98+3*a2j2syyy*t37+a2j2sy*t305+2*a2j2syyyy*
     & a2j2sx*a2j2rx+a2j2syyy*t106+a2j2sy*t310+a2j2ry*t470+2*a2j2syy*
     & t118+6*a2j2ryyy*a2j2sxy*a2j2sx+a2j2ry*t477+a2j2ryyyy*t3+
     & a2j2syy*t128+a2j2ry*t482+a2j2sy*t329+2*a2j2ryy*t70+a2j2sy*t321+
     & 3*a2j2ryy*t56+a2j2ry*t491+a2j2ryyy*t17+a2j2ryy*t76
       t499 = a2j2ry*t236
       t501 = a2j2ry*t8*a2j2syy
       t502 = a2j2ryy*t226
       t503 = a2j2sy*t200
       t504 = a2j2sy*t187+a2j2ry*t269+t499+t501+t502+t503
       t506 = t501+t499+t503+t502
       t517 = a2j2ry*t420+a2j2sy*t433
       t533 = a2j2rxyy**2
       t560 = a2j2sy*t420+a2j2ry*t211
       t568 = (a2j2ryy*t6*t8+a2j2ry*t392+a2j2ry*t394)*vv2rrrrr+(6*t398+
     & a2j2syyyy*a2j2sxx+8*a2j2sxy*a2j2sxyyy+6*a2j2syy*a2j2sxxyy+4*
     & a2j2syyy*a2j2sxxy)*vv2ss+(a2j2ry*t340+a2j2sy*t416+a2j2syy*t420+
     & a2j2ry*t344+a2j2sy*t423+a2j2ryy*t211)*vv2rrsss+(a2j2sy*t6*
     & a2j2ry*t8+a2j2ry*t437)*vv2rrrrrs+(a2j2sy*t58+a2j2syy*t20+
     & a2j2sy*t78+a2j2syyy*a2j2sy*t3+a2j2sy*t73+2*a2j2syy*t24)*
     & vv2ssss+t495*vv2rss+(a2j2ry*t504+a2j2ry*t506+a2j2ryy*t433+
     & a2j2sy*t392+a2j2syy*t6*t8+a2j2sy*t394)*vv2rrrrs+(a2j2ry*t517+
     & a2j2sy*t437)*vv2rrrrss+(a2j2ry*t423+a2j2ryy*t420+a2j2ry*t416+
     & a2j2sy*t506+a2j2sy*t504+a2j2syy*t433)*vv2rrrss+(a2j2ryyyy*
     & a2j2rxx+4*a2j2ryyy*a2j2rxxy+6*t533+8*a2j2rxyyy*a2j2rxy+6*
     & a2j2ryy*a2j2rxxyy)*vv2rr+(a2j2sy*t491+a2j2sy*t470+a2j2sy*t482+
     & a2j2sy*t477+3*a2j2syy*t56+6*a2j2syyy*a2j2sxy*a2j2sx+a2j2syyyy*
     & t3+a2j2syy*t76+a2j2syyy*t17+2*a2j2syy*t70)*vv2sss+(a2j2ry*t215+
     & a2j2sy*t560)*vv2rrssss+(a2j2sy*t517+a2j2ry*t560)*vv2rrrsss
       vv2xxyyyy2 = t385+t568
       t1 = a2j2sy**2
       t2 = t1**2
       t7 = a2j2syyy*a2j2sy
       t8 = a2j2syy**2
       t9 = t7+t8
       t10 = 2*t9
       t17 = 4*t7+3*t8
       t19 = a2j2syy*a2j2syyy
       t20 = 10*t19
       t21 = a2j2sy*a2j2syyyy
       t23 = t20+4*t21
       t27 = 2*t21+6*t19
       t30 = t20+5*t21
       t34 = 9*t19+3*t21
       t37 = 3*t9
       t42 = a2j2ry**2
       t43 = t42**2
       t51 = t42*a2j2ry
       t56 = a2j2syy*t42
       t57 = a2j2sy*a2j2ryy
       t58 = a2j2syy*a2j2ry
       t59 = t57+t58
       t60 = 2*t59
       t61 = a2j2ry*t60
       t63 = a2j2sy*a2j2ry*a2j2ryy
       t65 = t56+t61+4*t63
       t66 = a2j2ry*t65
       t67 = a2j2syy*t51
       t68 = t57*t42
       t70 = t66+t67+6*t68
       t74 = 3*t59
       t76 = 7*t63+t56+a2j2ry*t74+t61
       t78 = 12*t68+t66+t67+a2j2ry*t76
       t82 = a2j2syy*a2j2ryyy
       t83 = 10*t82
       t84 = a2j2syyy*a2j2ryy
       t85 = 10*t84
       t86 = a2j2sy*a2j2ryyyy
       t88 = a2j2syyyy*a2j2ry
       t90 = t83+t85+5*t86+5*t88
       t94 = t83+4*t86+t85+4*t88
       t96 = a2j2ry*a2j2ryyy
       t97 = a2j2ryy**2
       t98 = t96+t97
       t99 = 2*t98
       t104 = a2j2syyy*a2j2ry
       t106 = a2j2sy*a2j2ryyy
       t108 = a2j2syy*a2j2ryy
       t109 = 6*t108
       t110 = 4*t104+4*t106+t109
       t116 = 6*t82+2*t86+6*t84+2*t88
       t122 = 9*t84+3*t86+9*t82+3*t88
       t124 = a2j2ry*a2j2ryyyy
       t126 = a2j2ryyy*a2j2ryy
       t128 = 2*t124+6*t126
       t133 = 4*t96+3*t97
       t137 = t109+3*t106+3*t104
       t141 = 10*t126
       t142 = 5*t124+t141
       t148 = t141+4*t124
       t152 = 3*t98
       t158 = 2*t106+2*t104+4*t108
       t163 = 9*t126+3*t124
       t165 = a2j2ry*t90+a2j2ry*t94+3*a2j2syy*t99+9*t84*a2j2ry+a2j2ryy*
     & t110+a2j2ry*t116+a2j2ry*t122+a2j2sy*t128+a2j2ryyy*t74+a2j2syy*
     & t133+2*a2j2ryy*t137+a2j2sy*t142+3*a2j2ryyy*t60+a2j2syyyy*t42+
     & a2j2sy*t148+2*a2j2ry*t86+2*a2j2syy*t152+3*a2j2ryy*t158+a2j2sy*
     & t163
       t167 = t1*a2j2sy
       t178 = a2j2sy*t60
       t179 = t58*a2j2sy
       t181 = a2j2ryy*t1
       t182 = t178+4*t179+t181
       t183 = a2j2ry*t182
       t184 = a2j2ryy*a2j2ry
       t186 = 3*t184*t1
       t187 = a2j2sy*t65
       t189 = 3*a2j2sy*t56
       t190 = t183+t186+t187+t189
       t195 = a2j2sy*t74+7*t179+t181+t178
       t197 = t186+a2j2sy*t76+t187+t183+a2j2ry*t195+t189
       t203 = a2j2sy*t10
       t204 = t8*a2j2sy
       t206 = a2j2syyy*t1
       t207 = t203+4*t204+t206
       t210 = a2j2sy*t37
       t212 = 7*t204
       t213 = t210+a2j2sy*t17+t212+t206+t203
       t215 = t212+t206+t203+t210
       t245 = 2*a2j2ryy*t37+a2j2sy*t94+3*a2j2ryy*t10+a2j2ryyyy*t1+2*
     & t21*a2j2ry+a2j2ryy*t17+9*t106*a2j2syy+a2j2ry*t27+a2j2sy*t116+3*
     & a2j2syy*t158+a2j2syy*t110+a2j2ry*t23+a2j2sy*t90+3*a2j2syyy*t60+
     & a2j2syyy*t74+2*a2j2syy*t137+a2j2ry*t34+a2j2sy*t122+a2j2ry*t30
       t249 = a2j2ry*t1*a2j2syy
       t251 = a2j2sy*t182
       t253 = a2j2ryy*t167
       t254 = 12*t249+t251+a2j2sy*t195+t253
       t262 = 6*t249+t253+t251
       t266 = 6*t2*a2j2sy*a2j2ry*vv2rsssss+(3*a2j2syy*t10+9*t7*a2j2syy+
     & a2j2syy*t17+a2j2sy*t23+a2j2sy*t27+a2j2sy*t30+a2j2sy*t34+
     & a2j2syyyy*t1+2*a2j2syy*t37)*vv2sss+t43*t42*vv2rrrrrr+15*
     & a2j2syy*t2*vv2sssss+t2*t1*vv2ssssss+(18*a2j2ryy*t51*a2j2sy+
     & a2j2syy*t43+a2j2ry*t70+a2j2ry*t78)*vv2rrrrs+t165*vv2rrs+20*
     & t167*t51*vv2rrrsss+(6*a2j2ryy*t42*t1+a2j2sy*t78+a2j2sy*t70+4*
     & t67*a2j2sy+a2j2ry*t190+a2j2ry*t197)*vv2rrrss+(12*t8*t1+a2j2sy*
     & t207+a2j2syyy*t167+a2j2sy*t213+a2j2sy*t215)*vv2ssss+t245*
     & vv2rss+(a2j2sy*t190+a2j2ry*t254+a2j2sy*t197+6*t56*t1+4*t184*
     & t167+a2j2ry*t262)*vv2rrsss
       t269 = a2j2ryyy*t42
       t270 = a2j2ry*t152
       t271 = t97*a2j2ry
       t272 = 7*t271
       t274 = a2j2ry*t99
       t275 = t269+t270+t272+a2j2ry*t133+t274
       t280 = 2*t106*a2j2ry
       t281 = a2j2syyy*t42
       t282 = t58*a2j2ryy
       t285 = 2*a2j2ryy*t60
       t286 = a2j2sy*t99
       t287 = a2j2ry*t158
       t288 = t280+t281+4*t282+t285+t286+t287
       t291 = t274+t269+4*t271
       t293 = 7*t282
       t294 = a2j2ryy*t74
       t295 = a2j2ry*t137
       t297 = a2j2sy*t152
       t299 = t293+t286+t287+t294+t281+t295+a2j2sy*t133+t297+t280+
     & a2j2ry*t110+t285
       t302 = t280+t287+t295+t293+t286+t281+t285+t294+t297
       t306 = t272+t269+t270+t274
       t309 = 12*t108*t42+a2j2sy*t275+2*a2j2ryy*t65+a2j2ry*t288+a2j2sy*
     & t291+a2j2ry*t299+a2j2syyy*t51+a2j2ry*t302+3*t269*a2j2sy+a2j2sy*
     & t306+a2j2ryy*t76
       t313 = a2j2ryyy**2
       t319 = a2j2syyy**2
       t333 = a2j2ry*t10
       t335 = 2*t7*a2j2ry
       t336 = a2j2ryyy*t1
       t338 = 2*a2j2syy*t60
       t339 = a2j2sy*t108
       t341 = a2j2sy*t158
       t342 = t333+t335+t336+t338+4*t339+t341
       t344 = a2j2ry*t37
       t345 = a2j2syy*t74
       t346 = 7*t339
       t347 = a2j2sy*t137
       t348 = t335+t344+t345+t333+t338+t341+t346+t336+t347
       t362 = t346+t347+t338+a2j2sy*t110+a2j2ry*t17+t333+t344+t345+
     & t336+t335+t341
       t365 = 3*t281*a2j2sy+a2j2ry*t342+a2j2ry*t348+a2j2ryy*t195+
     & a2j2sy*t302+a2j2sy*t288+2*a2j2syy*t65+2*a2j2ryy*t182+a2j2sy*
     & t299+3*t96*t1+a2j2ry*t362+a2j2syy*t76
       t389 = a2j2sy*t342+a2j2ry*t207+12*t181*a2j2syy+a2j2ryyy*t167+3*
     & t104*t1+a2j2sy*t362+a2j2ry*t213+a2j2syy*t195+a2j2sy*t348+
     & a2j2ry*t215+2*a2j2syy*t182
       t426 = t309*vv2rrrs+(15*a2j2ryy*a2j2ryyyy+10*t313)*vv2rr+(15*
     & a2j2syy*a2j2syyyy+10*t319)*vv2ss+(15*a2j2syy*a2j2ryyyy+20*
     & a2j2syyy*a2j2ryyy+15*a2j2syyyy*a2j2ryy)*vv2rs+t365*vv2rrss+(
     & a2j2sy*t254+18*a2j2ry*t167*a2j2syy+a2j2sy*t262+a2j2ryy*t2)*
     & vv2rssss+t389*vv2rsss+(9*t126*a2j2ry+a2j2ry*t148+a2j2ry*t128+
     & a2j2ry*t142+a2j2ryyyy*t42+2*a2j2ryy*t152+3*a2j2ryy*t99+a2j2ryy*
     & t133+a2j2ry*t163)*vv2rrr+15*a2j2ryy*t43*vv2rrrrr+15*t2*t42*
     & vv2rrssss+(a2j2ryyy*t51+a2j2ry*t291+a2j2ry*t275+12*t97*t42+
     & a2j2ry*t306)*vv2rrrr+6*a2j2sy*t43*a2j2ry*vv2rrrrrs+15*t1*t43*
     & vv2rrrrss
       vv2yyyyyy2 = t266+t426
       ulapCubed1=uu1xxxxxx2+3.*(uu1xxxxyy2+uu1xxyyyy2)+uu1yyyyyy2
       vlapCubed1=vv1xxxxxx2+3.*(vv1xxxxyy2+vv1xxyyyy2)+vv1yyyyyy2
       ulapCubed2=uu2xxxxxx2+3.*(uu2xxxxyy2+uu2xxyyyy2)+uu2yyyyyy2
       vlapCubed2=vv2xxxxxx2+3.*(vv2xxxxyy2+vv2xxyyyy2)+vv2yyyyyy2
       ! first evaluate the equations we want to solve with the wrong values at the ghost points:
       f(0)=(uu1x6+vv1y6)  - (uu2x6+vv2y6)
       f(1)=(an1*ulap1+an2*vlap1) - (an1*ulap2+an2*vlap2)
       f(2)=(vv1x6-uu1y6) - (vv2x6-uu2y6)
       f(3)=(tau1*ulap1+tau2*vlap1)/eps1 - (tau1*ulap2+tau2*vlap2)/eps2
       ! These next we can do to 4th order 
       !     also subtract off f(3)_tau = (tau.Lap(uv))_tau/eps to eliminate vxxy term
       f(4)=( (uu1xxx4         + vv1xxy4        ) - ( tau1a*(uu1xxy4+
     & uu1yyy4)+tau2a*(vv1xxy4+vv1yyy4) ) )/eps1 - ( (uu2xxx4         
     & + vv2xxy4        ) - ( tau1a*(uu2xxy4+uu2yyy4)+tau2a*(vv2xxy4+
     & vv2yyy4) ) )/eps2
       !f(4)=( (uu1xxx4+uu1xyy4 + vv1xxy4+vv1yyy4) - ( tau1a*(uu1xxy4+uu1yyy4)+tau2a*(vv1xxy4+vv1yyy4) ) )/eps1 - !     ( (uu2xxx4+uu2xyy4 + vv2xxy4+vv2yyy4) - ( tau1a*(uu2xxy4+uu2yyy4)+tau2a*(vv2xxy4+vv2yyy4) ) )/eps2
       !f(4)=( (uu1xxx4+uu1xyy4 + vv1xxy4+vv1yyy4) )/eps1 - !     ( (uu2xxx4+uu2xyy4 + vv2xxy4+vv2yyy4) )/eps2
       f(5)=(an1*ulapSq1 + an2*vlapSq1)/eps1 - (an1*ulapSq2 + an2*
     & vlapSq2)/eps2
       ! also subtract ...
       f(6)=( ((vv1xxx4+vv1xyy4)-(uu1xxy4+uu1yyy4)) +(uu1xxy4+vv1xyy4) 
     & )/eps1 - ( ((vv2xxx4+vv2xyy4)-(uu2xxy4+uu2yyy4)) +(uu2xxy4+
     & vv2xyy4) )/eps2
       ! f(6)=( ((vv1xxx4+vv1xyy4)-(uu1xxy4+uu1yyy4)) )/eps1 - !      ( ((vv2xxx4+vv2xyy4)-(uu2xxy4+uu2yyy4)) )/eps2
       f(7)=(tau1*ulapSq1 + tau2*vlapSq1)/eps1**2 - (tau1*ulapSq2 + 
     & tau2*vlapSq2)/eps2**2
       ! These last we do to 2nd order
       f(8)=((uu1xxxxx2+2.*uu1xxxyy2+uu1xyyyy2)+(vv1xxxxy2+2.*
     & vv1xxyyy2+vv1yyyyy2) - (tau1a*(uu1xxxxy2+2.*uu1xxyyy2+
     & uu1yyyyy2)+tau2a*(vv1xxxxy2+2.*vv1xxyyy2+ vv1yyyyy2)) )/eps1**
     & 2 - ((uu2xxxxx2+2.*uu2xxxyy2+uu2xyyyy2)+(vv2xxxxy2+2.*
     & vv2xxyyy2+vv2yyyyy2)- (tau1a*(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2)
     & +tau2a*(vv2xxxxy2+2.*vv2xxyyy2+ vv2yyyyy2)) )/eps2**2
       ! f(8)=((uu1xxxxx2+2.*uu1xxxyy2+uu1xyyyy2)+(vv1xxxxy2+2.*vv1xxyyy2+vv1yyyyy2) !                                       )/eps1**2 - !      ((uu2xxxxx2+2.*uu2xxxyy2+uu2xyyyy2)+(vv2xxxxy2+2.*vv2xxyyy2+vv2yyyyy2) !                                       )/eps2**2
       f(9) =(an1*ulapCubed1+an2*vlapCubed1)/eps1**2 - (an1*ulapCubed2+
     & an2*vlapCubed2)/eps2**2
       ! add on extra terms to cancel odd y-derivative terms
       f(10)=( ((vv1xxxxx2+2.*vv1xxxyy2+vv1xyyyy2)-(uu1xxxxy2+2.*
     & uu1xxyyy2+uu1yyyyy2)) + ((uu1xxxxy2+vv1xxxyy2) +2.*(uu1xxyyy2+
     & vv1xyyyy2))               )/eps1**2 - ( ((vv2xxxxx2+2.*
     & vv2xxxyy2+vv2xyyyy2)-(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2)) + ((
     & uu2xxxxy2+vv2xxxyy2) +2.*(uu2xxyyy2+vv2xyyyy2))               )
     & /eps2**2
       ! f(10)=( ((vv1xxxxx2+2.*vv1xxxyy2+vv1xyyyy2)-(uu1xxxxy2+2.*uu1xxyyy2+uu1yyyyy2))  !                             )/eps1**2 - !       ( ((vv2xxxxx2+2.*vv2xxxyy2+vv2xyyyy2)-(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2))  !                            )/eps2**2
       f(11)=(tau1*ulapCubed1+tau2*vlapCubed1)/eps1**3 - (tau1*
     & ulapCubed2+tau2*vlapCubed2)/eps2**3
