! This file automatically generated from interfaceMacros.bf with bpp.

      a1j6rx = rsxy1(i1,i2,i3,0,0)
      a1j6rxr = (-rsxy1(i1-3,i2,i3,0,0)+9.*rsxy1(i1-2,i2,i3,0,0)-45.*
     & rsxy1(i1-1,i2,i3,0,0)+45.*rsxy1(i1+1,i2,i3,0,0)-9.*rsxy1(i1+2,
     & i2,i3,0,0)+rsxy1(i1+3,i2,i3,0,0))/(60.*dr1(0))
      a1j6rxs = (-rsxy1(i1,i2-3,i3,0,0)+9.*rsxy1(i1,i2-2,i3,0,0)-45.*
     & rsxy1(i1,i2-1,i3,0,0)+45.*rsxy1(i1,i2+1,i3,0,0)-9.*rsxy1(i1,i2+
     & 2,i3,0,0)+rsxy1(i1,i2+3,i3,0,0))/(60.*dr1(1))
      a1j6sx = rsxy1(i1,i2,i3,1,0)
      a1j6sxr = (-rsxy1(i1-3,i2,i3,1,0)+9.*rsxy1(i1-2,i2,i3,1,0)-45.*
     & rsxy1(i1-1,i2,i3,1,0)+45.*rsxy1(i1+1,i2,i3,1,0)-9.*rsxy1(i1+2,
     & i2,i3,1,0)+rsxy1(i1+3,i2,i3,1,0))/(60.*dr1(0))
      a1j6sxs = (-rsxy1(i1,i2-3,i3,1,0)+9.*rsxy1(i1,i2-2,i3,1,0)-45.*
     & rsxy1(i1,i2-1,i3,1,0)+45.*rsxy1(i1,i2+1,i3,1,0)-9.*rsxy1(i1,i2+
     & 2,i3,1,0)+rsxy1(i1,i2+3,i3,1,0))/(60.*dr1(1))
      a1j6ry = rsxy1(i1,i2,i3,0,1)
      a1j6ryr = (-rsxy1(i1-3,i2,i3,0,1)+9.*rsxy1(i1-2,i2,i3,0,1)-45.*
     & rsxy1(i1-1,i2,i3,0,1)+45.*rsxy1(i1+1,i2,i3,0,1)-9.*rsxy1(i1+2,
     & i2,i3,0,1)+rsxy1(i1+3,i2,i3,0,1))/(60.*dr1(0))
      a1j6rys = (-rsxy1(i1,i2-3,i3,0,1)+9.*rsxy1(i1,i2-2,i3,0,1)-45.*
     & rsxy1(i1,i2-1,i3,0,1)+45.*rsxy1(i1,i2+1,i3,0,1)-9.*rsxy1(i1,i2+
     & 2,i3,0,1)+rsxy1(i1,i2+3,i3,0,1))/(60.*dr1(1))
      a1j6sy = rsxy1(i1,i2,i3,1,1)
      a1j6syr = (-rsxy1(i1-3,i2,i3,1,1)+9.*rsxy1(i1-2,i2,i3,1,1)-45.*
     & rsxy1(i1-1,i2,i3,1,1)+45.*rsxy1(i1+1,i2,i3,1,1)-9.*rsxy1(i1+2,
     & i2,i3,1,1)+rsxy1(i1+3,i2,i3,1,1))/(60.*dr1(0))
      a1j6sys = (-rsxy1(i1,i2-3,i3,1,1)+9.*rsxy1(i1,i2-2,i3,1,1)-45.*
     & rsxy1(i1,i2-1,i3,1,1)+45.*rsxy1(i1,i2+1,i3,1,1)-9.*rsxy1(i1,i2+
     & 2,i3,1,1)+rsxy1(i1,i2+3,i3,1,1))/(60.*dr1(1))
      a1j6rxx = a1j6rx*a1j6rxr+a1j6sx*a1j6rxs
      a1j6rxy = a1j6ry*a1j6rxr+a1j6sy*a1j6rxs
      a1j6sxx = a1j6rx*a1j6sxr+a1j6sx*a1j6sxs
      a1j6sxy = a1j6ry*a1j6sxr+a1j6sy*a1j6sxs
      a1j6ryx = a1j6rx*a1j6ryr+a1j6sx*a1j6rys
      a1j6ryy = a1j6ry*a1j6ryr+a1j6sy*a1j6rys
      a1j6syx = a1j6rx*a1j6syr+a1j6sx*a1j6sys
      a1j6syy = a1j6ry*a1j6syr+a1j6sy*a1j6sys
      a1j4rx = rsxy1(i1,i2,i3,0,0)
      a1j4rxr = (rsxy1(i1-2,i2,i3,0,0)-8.*rsxy1(i1-1,i2,i3,0,0)+8.*
     & rsxy1(i1+1,i2,i3,0,0)-rsxy1(i1+2,i2,i3,0,0))/(12.*dr1(0))
      a1j4rxs = (rsxy1(i1,i2-2,i3,0,0)-8.*rsxy1(i1,i2-1,i3,0,0)+8.*
     & rsxy1(i1,i2+1,i3,0,0)-rsxy1(i1,i2+2,i3,0,0))/(12.*dr1(1))
      a1j4rxrr = (-rsxy1(i1-2,i2,i3,0,0)+16.*rsxy1(i1-1,i2,i3,0,0)-30.*
     & rsxy1(i1,i2,i3,0,0)+16.*rsxy1(i1+1,i2,i3,0,0)-rsxy1(i1+2,i2,i3,
     & 0,0))/(12.*dr1(0)**2)
      a1j4rxrs = ((rsxy1(i1-2,i2-2,i3,0,0)-8.*rsxy1(i1-2,i2-1,i3,0,0)+
     & 8.*rsxy1(i1-2,i2+1,i3,0,0)-rsxy1(i1-2,i2+2,i3,0,0))/(12.*dr1(1)
     & )-8.*(rsxy1(i1-1,i2-2,i3,0,0)-8.*rsxy1(i1-1,i2-1,i3,0,0)+8.*
     & rsxy1(i1-1,i2+1,i3,0,0)-rsxy1(i1-1,i2+2,i3,0,0))/(12.*dr1(1))+
     & 8.*(rsxy1(i1+1,i2-2,i3,0,0)-8.*rsxy1(i1+1,i2-1,i3,0,0)+8.*
     & rsxy1(i1+1,i2+1,i3,0,0)-rsxy1(i1+1,i2+2,i3,0,0))/(12.*dr1(1))-(
     & rsxy1(i1+2,i2-2,i3,0,0)-8.*rsxy1(i1+2,i2-1,i3,0,0)+8.*rsxy1(i1+
     & 2,i2+1,i3,0,0)-rsxy1(i1+2,i2+2,i3,0,0))/(12.*dr1(1)))/(12.*dr1(
     & 0))
      a1j4rxss = (-rsxy1(i1,i2-2,i3,0,0)+16.*rsxy1(i1,i2-1,i3,0,0)-30.*
     & rsxy1(i1,i2,i3,0,0)+16.*rsxy1(i1,i2+1,i3,0,0)-rsxy1(i1,i2+2,i3,
     & 0,0))/(12.*dr1(1)**2)
      a1j4rxrrr = (rsxy1(i1-3,i2,i3,0,0)-8.*rsxy1(i1-2,i2,i3,0,0)+13.*
     & rsxy1(i1-1,i2,i3,0,0)-13.*rsxy1(i1+1,i2,i3,0,0)+8.*rsxy1(i1+2,
     & i2,i3,0,0)-rsxy1(i1+3,i2,i3,0,0))/(8.*dr1(0)**3)
      a1j4rxrrs = (-(rsxy1(i1-2,i2-2,i3,0,0)-8.*rsxy1(i1-2,i2-1,i3,0,0)
     & +8.*rsxy1(i1-2,i2+1,i3,0,0)-rsxy1(i1-2,i2+2,i3,0,0))/(12.*dr1(
     & 1))+16.*(rsxy1(i1-1,i2-2,i3,0,0)-8.*rsxy1(i1-1,i2-1,i3,0,0)+8.*
     & rsxy1(i1-1,i2+1,i3,0,0)-rsxy1(i1-1,i2+2,i3,0,0))/(12.*dr1(1))-
     & 30.*(rsxy1(i1,i2-2,i3,0,0)-8.*rsxy1(i1,i2-1,i3,0,0)+8.*rsxy1(
     & i1,i2+1,i3,0,0)-rsxy1(i1,i2+2,i3,0,0))/(12.*dr1(1))+16.*(rsxy1(
     & i1+1,i2-2,i3,0,0)-8.*rsxy1(i1+1,i2-1,i3,0,0)+8.*rsxy1(i1+1,i2+
     & 1,i3,0,0)-rsxy1(i1+1,i2+2,i3,0,0))/(12.*dr1(1))-(rsxy1(i1+2,i2-
     & 2,i3,0,0)-8.*rsxy1(i1+2,i2-1,i3,0,0)+8.*rsxy1(i1+2,i2+1,i3,0,0)
     & -rsxy1(i1+2,i2+2,i3,0,0))/(12.*dr1(1)))/(12.*dr1(0)**2)
      a1j4rxrss = ((-rsxy1(i1-2,i2-2,i3,0,0)+16.*rsxy1(i1-2,i2-1,i3,0,
     & 0)-30.*rsxy1(i1-2,i2,i3,0,0)+16.*rsxy1(i1-2,i2+1,i3,0,0)-rsxy1(
     & i1-2,i2+2,i3,0,0))/(12.*dr1(1)**2)-8.*(-rsxy1(i1-1,i2-2,i3,0,0)
     & +16.*rsxy1(i1-1,i2-1,i3,0,0)-30.*rsxy1(i1-1,i2,i3,0,0)+16.*
     & rsxy1(i1-1,i2+1,i3,0,0)-rsxy1(i1-1,i2+2,i3,0,0))/(12.*dr1(1)**
     & 2)+8.*(-rsxy1(i1+1,i2-2,i3,0,0)+16.*rsxy1(i1+1,i2-1,i3,0,0)-
     & 30.*rsxy1(i1+1,i2,i3,0,0)+16.*rsxy1(i1+1,i2+1,i3,0,0)-rsxy1(i1+
     & 1,i2+2,i3,0,0))/(12.*dr1(1)**2)-(-rsxy1(i1+2,i2-2,i3,0,0)+16.*
     & rsxy1(i1+2,i2-1,i3,0,0)-30.*rsxy1(i1+2,i2,i3,0,0)+16.*rsxy1(i1+
     & 2,i2+1,i3,0,0)-rsxy1(i1+2,i2+2,i3,0,0))/(12.*dr1(1)**2))/(12.*
     & dr1(0))
      a1j4rxsss = (rsxy1(i1,i2-3,i3,0,0)-8.*rsxy1(i1,i2-2,i3,0,0)+13.*
     & rsxy1(i1,i2-1,i3,0,0)-13.*rsxy1(i1,i2+1,i3,0,0)+8.*rsxy1(i1,i2+
     & 2,i3,0,0)-rsxy1(i1,i2+3,i3,0,0))/(8.*dr1(1)**3)
      a1j4sx = rsxy1(i1,i2,i3,1,0)
      a1j4sxr = (rsxy1(i1-2,i2,i3,1,0)-8.*rsxy1(i1-1,i2,i3,1,0)+8.*
     & rsxy1(i1+1,i2,i3,1,0)-rsxy1(i1+2,i2,i3,1,0))/(12.*dr1(0))
      a1j4sxs = (rsxy1(i1,i2-2,i3,1,0)-8.*rsxy1(i1,i2-1,i3,1,0)+8.*
     & rsxy1(i1,i2+1,i3,1,0)-rsxy1(i1,i2+2,i3,1,0))/(12.*dr1(1))
      a1j4sxrr = (-rsxy1(i1-2,i2,i3,1,0)+16.*rsxy1(i1-1,i2,i3,1,0)-30.*
     & rsxy1(i1,i2,i3,1,0)+16.*rsxy1(i1+1,i2,i3,1,0)-rsxy1(i1+2,i2,i3,
     & 1,0))/(12.*dr1(0)**2)
      a1j4sxrs = ((rsxy1(i1-2,i2-2,i3,1,0)-8.*rsxy1(i1-2,i2-1,i3,1,0)+
     & 8.*rsxy1(i1-2,i2+1,i3,1,0)-rsxy1(i1-2,i2+2,i3,1,0))/(12.*dr1(1)
     & )-8.*(rsxy1(i1-1,i2-2,i3,1,0)-8.*rsxy1(i1-1,i2-1,i3,1,0)+8.*
     & rsxy1(i1-1,i2+1,i3,1,0)-rsxy1(i1-1,i2+2,i3,1,0))/(12.*dr1(1))+
     & 8.*(rsxy1(i1+1,i2-2,i3,1,0)-8.*rsxy1(i1+1,i2-1,i3,1,0)+8.*
     & rsxy1(i1+1,i2+1,i3,1,0)-rsxy1(i1+1,i2+2,i3,1,0))/(12.*dr1(1))-(
     & rsxy1(i1+2,i2-2,i3,1,0)-8.*rsxy1(i1+2,i2-1,i3,1,0)+8.*rsxy1(i1+
     & 2,i2+1,i3,1,0)-rsxy1(i1+2,i2+2,i3,1,0))/(12.*dr1(1)))/(12.*dr1(
     & 0))
      a1j4sxss = (-rsxy1(i1,i2-2,i3,1,0)+16.*rsxy1(i1,i2-1,i3,1,0)-30.*
     & rsxy1(i1,i2,i3,1,0)+16.*rsxy1(i1,i2+1,i3,1,0)-rsxy1(i1,i2+2,i3,
     & 1,0))/(12.*dr1(1)**2)
      a1j4sxrrr = (rsxy1(i1-3,i2,i3,1,0)-8.*rsxy1(i1-2,i2,i3,1,0)+13.*
     & rsxy1(i1-1,i2,i3,1,0)-13.*rsxy1(i1+1,i2,i3,1,0)+8.*rsxy1(i1+2,
     & i2,i3,1,0)-rsxy1(i1+3,i2,i3,1,0))/(8.*dr1(0)**3)
      a1j4sxrrs = (-(rsxy1(i1-2,i2-2,i3,1,0)-8.*rsxy1(i1-2,i2-1,i3,1,0)
     & +8.*rsxy1(i1-2,i2+1,i3,1,0)-rsxy1(i1-2,i2+2,i3,1,0))/(12.*dr1(
     & 1))+16.*(rsxy1(i1-1,i2-2,i3,1,0)-8.*rsxy1(i1-1,i2-1,i3,1,0)+8.*
     & rsxy1(i1-1,i2+1,i3,1,0)-rsxy1(i1-1,i2+2,i3,1,0))/(12.*dr1(1))-
     & 30.*(rsxy1(i1,i2-2,i3,1,0)-8.*rsxy1(i1,i2-1,i3,1,0)+8.*rsxy1(
     & i1,i2+1,i3,1,0)-rsxy1(i1,i2+2,i3,1,0))/(12.*dr1(1))+16.*(rsxy1(
     & i1+1,i2-2,i3,1,0)-8.*rsxy1(i1+1,i2-1,i3,1,0)+8.*rsxy1(i1+1,i2+
     & 1,i3,1,0)-rsxy1(i1+1,i2+2,i3,1,0))/(12.*dr1(1))-(rsxy1(i1+2,i2-
     & 2,i3,1,0)-8.*rsxy1(i1+2,i2-1,i3,1,0)+8.*rsxy1(i1+2,i2+1,i3,1,0)
     & -rsxy1(i1+2,i2+2,i3,1,0))/(12.*dr1(1)))/(12.*dr1(0)**2)
      a1j4sxrss = ((-rsxy1(i1-2,i2-2,i3,1,0)+16.*rsxy1(i1-2,i2-1,i3,1,
     & 0)-30.*rsxy1(i1-2,i2,i3,1,0)+16.*rsxy1(i1-2,i2+1,i3,1,0)-rsxy1(
     & i1-2,i2+2,i3,1,0))/(12.*dr1(1)**2)-8.*(-rsxy1(i1-1,i2-2,i3,1,0)
     & +16.*rsxy1(i1-1,i2-1,i3,1,0)-30.*rsxy1(i1-1,i2,i3,1,0)+16.*
     & rsxy1(i1-1,i2+1,i3,1,0)-rsxy1(i1-1,i2+2,i3,1,0))/(12.*dr1(1)**
     & 2)+8.*(-rsxy1(i1+1,i2-2,i3,1,0)+16.*rsxy1(i1+1,i2-1,i3,1,0)-
     & 30.*rsxy1(i1+1,i2,i3,1,0)+16.*rsxy1(i1+1,i2+1,i3,1,0)-rsxy1(i1+
     & 1,i2+2,i3,1,0))/(12.*dr1(1)**2)-(-rsxy1(i1+2,i2-2,i3,1,0)+16.*
     & rsxy1(i1+2,i2-1,i3,1,0)-30.*rsxy1(i1+2,i2,i3,1,0)+16.*rsxy1(i1+
     & 2,i2+1,i3,1,0)-rsxy1(i1+2,i2+2,i3,1,0))/(12.*dr1(1)**2))/(12.*
     & dr1(0))
      a1j4sxsss = (rsxy1(i1,i2-3,i3,1,0)-8.*rsxy1(i1,i2-2,i3,1,0)+13.*
     & rsxy1(i1,i2-1,i3,1,0)-13.*rsxy1(i1,i2+1,i3,1,0)+8.*rsxy1(i1,i2+
     & 2,i3,1,0)-rsxy1(i1,i2+3,i3,1,0))/(8.*dr1(1)**3)
      a1j4ry = rsxy1(i1,i2,i3,0,1)
      a1j4ryr = (rsxy1(i1-2,i2,i3,0,1)-8.*rsxy1(i1-1,i2,i3,0,1)+8.*
     & rsxy1(i1+1,i2,i3,0,1)-rsxy1(i1+2,i2,i3,0,1))/(12.*dr1(0))
      a1j4rys = (rsxy1(i1,i2-2,i3,0,1)-8.*rsxy1(i1,i2-1,i3,0,1)+8.*
     & rsxy1(i1,i2+1,i3,0,1)-rsxy1(i1,i2+2,i3,0,1))/(12.*dr1(1))
      a1j4ryrr = (-rsxy1(i1-2,i2,i3,0,1)+16.*rsxy1(i1-1,i2,i3,0,1)-30.*
     & rsxy1(i1,i2,i3,0,1)+16.*rsxy1(i1+1,i2,i3,0,1)-rsxy1(i1+2,i2,i3,
     & 0,1))/(12.*dr1(0)**2)
      a1j4ryrs = ((rsxy1(i1-2,i2-2,i3,0,1)-8.*rsxy1(i1-2,i2-1,i3,0,1)+
     & 8.*rsxy1(i1-2,i2+1,i3,0,1)-rsxy1(i1-2,i2+2,i3,0,1))/(12.*dr1(1)
     & )-8.*(rsxy1(i1-1,i2-2,i3,0,1)-8.*rsxy1(i1-1,i2-1,i3,0,1)+8.*
     & rsxy1(i1-1,i2+1,i3,0,1)-rsxy1(i1-1,i2+2,i3,0,1))/(12.*dr1(1))+
     & 8.*(rsxy1(i1+1,i2-2,i3,0,1)-8.*rsxy1(i1+1,i2-1,i3,0,1)+8.*
     & rsxy1(i1+1,i2+1,i3,0,1)-rsxy1(i1+1,i2+2,i3,0,1))/(12.*dr1(1))-(
     & rsxy1(i1+2,i2-2,i3,0,1)-8.*rsxy1(i1+2,i2-1,i3,0,1)+8.*rsxy1(i1+
     & 2,i2+1,i3,0,1)-rsxy1(i1+2,i2+2,i3,0,1))/(12.*dr1(1)))/(12.*dr1(
     & 0))
      a1j4ryss = (-rsxy1(i1,i2-2,i3,0,1)+16.*rsxy1(i1,i2-1,i3,0,1)-30.*
     & rsxy1(i1,i2,i3,0,1)+16.*rsxy1(i1,i2+1,i3,0,1)-rsxy1(i1,i2+2,i3,
     & 0,1))/(12.*dr1(1)**2)
      a1j4ryrrr = (rsxy1(i1-3,i2,i3,0,1)-8.*rsxy1(i1-2,i2,i3,0,1)+13.*
     & rsxy1(i1-1,i2,i3,0,1)-13.*rsxy1(i1+1,i2,i3,0,1)+8.*rsxy1(i1+2,
     & i2,i3,0,1)-rsxy1(i1+3,i2,i3,0,1))/(8.*dr1(0)**3)
      a1j4ryrrs = (-(rsxy1(i1-2,i2-2,i3,0,1)-8.*rsxy1(i1-2,i2-1,i3,0,1)
     & +8.*rsxy1(i1-2,i2+1,i3,0,1)-rsxy1(i1-2,i2+2,i3,0,1))/(12.*dr1(
     & 1))+16.*(rsxy1(i1-1,i2-2,i3,0,1)-8.*rsxy1(i1-1,i2-1,i3,0,1)+8.*
     & rsxy1(i1-1,i2+1,i3,0,1)-rsxy1(i1-1,i2+2,i3,0,1))/(12.*dr1(1))-
     & 30.*(rsxy1(i1,i2-2,i3,0,1)-8.*rsxy1(i1,i2-1,i3,0,1)+8.*rsxy1(
     & i1,i2+1,i3,0,1)-rsxy1(i1,i2+2,i3,0,1))/(12.*dr1(1))+16.*(rsxy1(
     & i1+1,i2-2,i3,0,1)-8.*rsxy1(i1+1,i2-1,i3,0,1)+8.*rsxy1(i1+1,i2+
     & 1,i3,0,1)-rsxy1(i1+1,i2+2,i3,0,1))/(12.*dr1(1))-(rsxy1(i1+2,i2-
     & 2,i3,0,1)-8.*rsxy1(i1+2,i2-1,i3,0,1)+8.*rsxy1(i1+2,i2+1,i3,0,1)
     & -rsxy1(i1+2,i2+2,i3,0,1))/(12.*dr1(1)))/(12.*dr1(0)**2)
      a1j4ryrss = ((-rsxy1(i1-2,i2-2,i3,0,1)+16.*rsxy1(i1-2,i2-1,i3,0,
     & 1)-30.*rsxy1(i1-2,i2,i3,0,1)+16.*rsxy1(i1-2,i2+1,i3,0,1)-rsxy1(
     & i1-2,i2+2,i3,0,1))/(12.*dr1(1)**2)-8.*(-rsxy1(i1-1,i2-2,i3,0,1)
     & +16.*rsxy1(i1-1,i2-1,i3,0,1)-30.*rsxy1(i1-1,i2,i3,0,1)+16.*
     & rsxy1(i1-1,i2+1,i3,0,1)-rsxy1(i1-1,i2+2,i3,0,1))/(12.*dr1(1)**
     & 2)+8.*(-rsxy1(i1+1,i2-2,i3,0,1)+16.*rsxy1(i1+1,i2-1,i3,0,1)-
     & 30.*rsxy1(i1+1,i2,i3,0,1)+16.*rsxy1(i1+1,i2+1,i3,0,1)-rsxy1(i1+
     & 1,i2+2,i3,0,1))/(12.*dr1(1)**2)-(-rsxy1(i1+2,i2-2,i3,0,1)+16.*
     & rsxy1(i1+2,i2-1,i3,0,1)-30.*rsxy1(i1+2,i2,i3,0,1)+16.*rsxy1(i1+
     & 2,i2+1,i3,0,1)-rsxy1(i1+2,i2+2,i3,0,1))/(12.*dr1(1)**2))/(12.*
     & dr1(0))
      a1j4rysss = (rsxy1(i1,i2-3,i3,0,1)-8.*rsxy1(i1,i2-2,i3,0,1)+13.*
     & rsxy1(i1,i2-1,i3,0,1)-13.*rsxy1(i1,i2+1,i3,0,1)+8.*rsxy1(i1,i2+
     & 2,i3,0,1)-rsxy1(i1,i2+3,i3,0,1))/(8.*dr1(1)**3)
      a1j4sy = rsxy1(i1,i2,i3,1,1)
      a1j4syr = (rsxy1(i1-2,i2,i3,1,1)-8.*rsxy1(i1-1,i2,i3,1,1)+8.*
     & rsxy1(i1+1,i2,i3,1,1)-rsxy1(i1+2,i2,i3,1,1))/(12.*dr1(0))
      a1j4sys = (rsxy1(i1,i2-2,i3,1,1)-8.*rsxy1(i1,i2-1,i3,1,1)+8.*
     & rsxy1(i1,i2+1,i3,1,1)-rsxy1(i1,i2+2,i3,1,1))/(12.*dr1(1))
      a1j4syrr = (-rsxy1(i1-2,i2,i3,1,1)+16.*rsxy1(i1-1,i2,i3,1,1)-30.*
     & rsxy1(i1,i2,i3,1,1)+16.*rsxy1(i1+1,i2,i3,1,1)-rsxy1(i1+2,i2,i3,
     & 1,1))/(12.*dr1(0)**2)
      a1j4syrs = ((rsxy1(i1-2,i2-2,i3,1,1)-8.*rsxy1(i1-2,i2-1,i3,1,1)+
     & 8.*rsxy1(i1-2,i2+1,i3,1,1)-rsxy1(i1-2,i2+2,i3,1,1))/(12.*dr1(1)
     & )-8.*(rsxy1(i1-1,i2-2,i3,1,1)-8.*rsxy1(i1-1,i2-1,i3,1,1)+8.*
     & rsxy1(i1-1,i2+1,i3,1,1)-rsxy1(i1-1,i2+2,i3,1,1))/(12.*dr1(1))+
     & 8.*(rsxy1(i1+1,i2-2,i3,1,1)-8.*rsxy1(i1+1,i2-1,i3,1,1)+8.*
     & rsxy1(i1+1,i2+1,i3,1,1)-rsxy1(i1+1,i2+2,i3,1,1))/(12.*dr1(1))-(
     & rsxy1(i1+2,i2-2,i3,1,1)-8.*rsxy1(i1+2,i2-1,i3,1,1)+8.*rsxy1(i1+
     & 2,i2+1,i3,1,1)-rsxy1(i1+2,i2+2,i3,1,1))/(12.*dr1(1)))/(12.*dr1(
     & 0))
      a1j4syss = (-rsxy1(i1,i2-2,i3,1,1)+16.*rsxy1(i1,i2-1,i3,1,1)-30.*
     & rsxy1(i1,i2,i3,1,1)+16.*rsxy1(i1,i2+1,i3,1,1)-rsxy1(i1,i2+2,i3,
     & 1,1))/(12.*dr1(1)**2)
      a1j4syrrr = (rsxy1(i1-3,i2,i3,1,1)-8.*rsxy1(i1-2,i2,i3,1,1)+13.*
     & rsxy1(i1-1,i2,i3,1,1)-13.*rsxy1(i1+1,i2,i3,1,1)+8.*rsxy1(i1+2,
     & i2,i3,1,1)-rsxy1(i1+3,i2,i3,1,1))/(8.*dr1(0)**3)
      a1j4syrrs = (-(rsxy1(i1-2,i2-2,i3,1,1)-8.*rsxy1(i1-2,i2-1,i3,1,1)
     & +8.*rsxy1(i1-2,i2+1,i3,1,1)-rsxy1(i1-2,i2+2,i3,1,1))/(12.*dr1(
     & 1))+16.*(rsxy1(i1-1,i2-2,i3,1,1)-8.*rsxy1(i1-1,i2-1,i3,1,1)+8.*
     & rsxy1(i1-1,i2+1,i3,1,1)-rsxy1(i1-1,i2+2,i3,1,1))/(12.*dr1(1))-
     & 30.*(rsxy1(i1,i2-2,i3,1,1)-8.*rsxy1(i1,i2-1,i3,1,1)+8.*rsxy1(
     & i1,i2+1,i3,1,1)-rsxy1(i1,i2+2,i3,1,1))/(12.*dr1(1))+16.*(rsxy1(
     & i1+1,i2-2,i3,1,1)-8.*rsxy1(i1+1,i2-1,i3,1,1)+8.*rsxy1(i1+1,i2+
     & 1,i3,1,1)-rsxy1(i1+1,i2+2,i3,1,1))/(12.*dr1(1))-(rsxy1(i1+2,i2-
     & 2,i3,1,1)-8.*rsxy1(i1+2,i2-1,i3,1,1)+8.*rsxy1(i1+2,i2+1,i3,1,1)
     & -rsxy1(i1+2,i2+2,i3,1,1))/(12.*dr1(1)))/(12.*dr1(0)**2)
      a1j4syrss = ((-rsxy1(i1-2,i2-2,i3,1,1)+16.*rsxy1(i1-2,i2-1,i3,1,
     & 1)-30.*rsxy1(i1-2,i2,i3,1,1)+16.*rsxy1(i1-2,i2+1,i3,1,1)-rsxy1(
     & i1-2,i2+2,i3,1,1))/(12.*dr1(1)**2)-8.*(-rsxy1(i1-1,i2-2,i3,1,1)
     & +16.*rsxy1(i1-1,i2-1,i3,1,1)-30.*rsxy1(i1-1,i2,i3,1,1)+16.*
     & rsxy1(i1-1,i2+1,i3,1,1)-rsxy1(i1-1,i2+2,i3,1,1))/(12.*dr1(1)**
     & 2)+8.*(-rsxy1(i1+1,i2-2,i3,1,1)+16.*rsxy1(i1+1,i2-1,i3,1,1)-
     & 30.*rsxy1(i1+1,i2,i3,1,1)+16.*rsxy1(i1+1,i2+1,i3,1,1)-rsxy1(i1+
     & 1,i2+2,i3,1,1))/(12.*dr1(1)**2)-(-rsxy1(i1+2,i2-2,i3,1,1)+16.*
     & rsxy1(i1+2,i2-1,i3,1,1)-30.*rsxy1(i1+2,i2,i3,1,1)+16.*rsxy1(i1+
     & 2,i2+1,i3,1,1)-rsxy1(i1+2,i2+2,i3,1,1))/(12.*dr1(1)**2))/(12.*
     & dr1(0))
      a1j4sysss = (rsxy1(i1,i2-3,i3,1,1)-8.*rsxy1(i1,i2-2,i3,1,1)+13.*
     & rsxy1(i1,i2-1,i3,1,1)-13.*rsxy1(i1,i2+1,i3,1,1)+8.*rsxy1(i1,i2+
     & 2,i3,1,1)-rsxy1(i1,i2+3,i3,1,1))/(8.*dr1(1)**3)
      a1j4rxx = a1j4rx*a1j4rxr+a1j4sx*a1j4rxs
      a1j4rxy = a1j4ry*a1j4rxr+a1j4sy*a1j4rxs
      a1j4sxx = a1j4rx*a1j4sxr+a1j4sx*a1j4sxs
      a1j4sxy = a1j4ry*a1j4sxr+a1j4sy*a1j4sxs
      a1j4ryx = a1j4rx*a1j4ryr+a1j4sx*a1j4rys
      a1j4ryy = a1j4ry*a1j4ryr+a1j4sy*a1j4rys
      a1j4syx = a1j4rx*a1j4syr+a1j4sx*a1j4sys
      a1j4syy = a1j4ry*a1j4syr+a1j4sy*a1j4sys
      t1 = a1j4rx**2
      t6 = a1j4sx**2
      a1j4rxxx = t1*a1j4rxrr+2*a1j4rx*a1j4sx*a1j4rxrs+t6*a1j4rxss+
     & a1j4rxx*a1j4rxr+a1j4sxx*a1j4rxs
      a1j4rxxy = a1j4ry*a1j4rx*a1j4rxrr+(a1j4sy*a1j4rx+a1j4ry*a1j4sx)*
     & a1j4rxrs+a1j4sy*a1j4sx*a1j4rxss+a1j4rxy*a1j4rxr+a1j4sxy*a1j4rxs
      t1 = a1j4ry**2
      t6 = a1j4sy**2
      a1j4rxyy = t1*a1j4rxrr+2*a1j4ry*a1j4sy*a1j4rxrs+t6*a1j4rxss+
     & a1j4ryy*a1j4rxr+a1j4syy*a1j4rxs
      t1 = a1j4rx**2
      t6 = a1j4sx**2
      a1j4sxxx = t1*a1j4sxrr+2*a1j4rx*a1j4sx*a1j4sxrs+t6*a1j4sxss+
     & a1j4rxx*a1j4sxr+a1j4sxx*a1j4sxs
      a1j4sxxy = a1j4ry*a1j4rx*a1j4sxrr+(a1j4sy*a1j4rx+a1j4ry*a1j4sx)*
     & a1j4sxrs+a1j4sy*a1j4sx*a1j4sxss+a1j4rxy*a1j4sxr+a1j4sxy*a1j4sxs
      t1 = a1j4ry**2
      t6 = a1j4sy**2
      a1j4sxyy = t1*a1j4sxrr+2*a1j4ry*a1j4sy*a1j4sxrs+t6*a1j4sxss+
     & a1j4ryy*a1j4sxr+a1j4syy*a1j4sxs
      t1 = a1j4rx**2
      t6 = a1j4sx**2
      a1j4ryxx = t1*a1j4ryrr+2*a1j4rx*a1j4sx*a1j4ryrs+t6*a1j4ryss+
     & a1j4rxx*a1j4ryr+a1j4sxx*a1j4rys
      a1j4ryxy = a1j4ry*a1j4rx*a1j4ryrr+(a1j4sy*a1j4rx+a1j4ry*a1j4sx)*
     & a1j4ryrs+a1j4sy*a1j4sx*a1j4ryss+a1j4rxy*a1j4ryr+a1j4sxy*a1j4rys
      t1 = a1j4ry**2
      t6 = a1j4sy**2
      a1j4ryyy = t1*a1j4ryrr+2*a1j4ry*a1j4sy*a1j4ryrs+t6*a1j4ryss+
     & a1j4ryy*a1j4ryr+a1j4syy*a1j4rys
      t1 = a1j4rx**2
      t6 = a1j4sx**2
      a1j4syxx = t1*a1j4syrr+2*a1j4rx*a1j4sx*a1j4syrs+t6*a1j4syss+
     & a1j4rxx*a1j4syr+a1j4sxx*a1j4sys
      a1j4syxy = a1j4ry*a1j4rx*a1j4syrr+(a1j4sy*a1j4rx+a1j4ry*a1j4sx)*
     & a1j4syrs+a1j4sy*a1j4sx*a1j4syss+a1j4rxy*a1j4syr+a1j4sxy*a1j4sys
      t1 = a1j4ry**2
      t6 = a1j4sy**2
      a1j4syyy = t1*a1j4syrr+2*a1j4ry*a1j4sy*a1j4syrs+t6*a1j4syss+
     & a1j4ryy*a1j4syr+a1j4syy*a1j4sys
      t1 = a1j4rx**2
      t7 = a1j4sx**2
      a1j4rxxxx = t1*a1j4rx*a1j4rxrrr+3*t1*a1j4sx*a1j4rxrrs+3*a1j4rx*
     & t7*a1j4rxrss+t7*a1j4sx*a1j4rxsss+3*a1j4rx*a1j4rxx*a1j4rxrr+(3*
     & a1j4sxx*a1j4rx+3*a1j4sx*a1j4rxx)*a1j4rxrs+3*a1j4sxx*a1j4sx*
     & a1j4rxss+a1j4rxxx*a1j4rxr+a1j4sxxx*a1j4rxs
      t1 = a1j4rx**2
      t10 = a1j4sx**2
      a1j4rxxxy = a1j4ry*t1*a1j4rxrrr+(a1j4sy*t1+2*a1j4ry*a1j4sx*
     & a1j4rx)*a1j4rxrrs+(a1j4ry*t10+2*a1j4sy*a1j4sx*a1j4rx)*
     & a1j4rxrss+a1j4sy*t10*a1j4rxsss+(2*a1j4rxy*a1j4rx+a1j4ry*
     & a1j4rxx)*a1j4rxrr+(a1j4ry*a1j4sxx+2*a1j4sx*a1j4rxy+2*a1j4sxy*
     & a1j4rx+a1j4sy*a1j4rxx)*a1j4rxrs+(a1j4sy*a1j4sxx+2*a1j4sxy*
     & a1j4sx)*a1j4rxss+a1j4rxxy*a1j4rxr+a1j4sxxy*a1j4rxs
      t1 = a1j4ry**2
      t4 = a1j4sy*a1j4ry
      t8 = a1j4sy*a1j4rx+a1j4ry*a1j4sx
      t16 = a1j4sy**2
      a1j4rxxyy = t1*a1j4rx*a1j4rxrrr+(t4*a1j4rx+a1j4ry*t8)*a1j4rxrrs+(
     & t4*a1j4sx+a1j4sy*t8)*a1j4rxrss+t16*a1j4sx*a1j4rxsss+(a1j4ryy*
     & a1j4rx+2*a1j4ry*a1j4rxy)*a1j4rxrr+(2*a1j4ry*a1j4sxy+2*a1j4sy*
     & a1j4rxy+a1j4ryy*a1j4sx+a1j4syy*a1j4rx)*a1j4rxrs+(a1j4syy*
     & a1j4sx+2*a1j4sy*a1j4sxy)*a1j4rxss+a1j4rxyy*a1j4rxr+a1j4sxyy*
     & a1j4rxs
      t1 = a1j4ry**2
      t7 = a1j4sy**2
      a1j4rxyyy = a1j4ry*t1*a1j4rxrrr+3*t1*a1j4sy*a1j4rxrrs+3*a1j4ry*
     & t7*a1j4rxrss+t7*a1j4sy*a1j4rxsss+3*a1j4ry*a1j4ryy*a1j4rxrr+(3*
     & a1j4syy*a1j4ry+3*a1j4sy*a1j4ryy)*a1j4rxrs+3*a1j4syy*a1j4sy*
     & a1j4rxss+a1j4ryyy*a1j4rxr+a1j4syyy*a1j4rxs
      t1 = a1j4rx**2
      t7 = a1j4sx**2
      a1j4sxxxx = t1*a1j4rx*a1j4sxrrr+3*t1*a1j4sx*a1j4sxrrs+3*a1j4rx*
     & t7*a1j4sxrss+t7*a1j4sx*a1j4sxsss+3*a1j4rx*a1j4rxx*a1j4sxrr+(3*
     & a1j4sxx*a1j4rx+3*a1j4sx*a1j4rxx)*a1j4sxrs+3*a1j4sxx*a1j4sx*
     & a1j4sxss+a1j4rxxx*a1j4sxr+a1j4sxxx*a1j4sxs
      t1 = a1j4rx**2
      t10 = a1j4sx**2
      a1j4sxxxy = a1j4ry*t1*a1j4sxrrr+(a1j4sy*t1+2*a1j4ry*a1j4sx*
     & a1j4rx)*a1j4sxrrs+(a1j4ry*t10+2*a1j4sy*a1j4sx*a1j4rx)*
     & a1j4sxrss+a1j4sy*t10*a1j4sxsss+(2*a1j4rxy*a1j4rx+a1j4ry*
     & a1j4rxx)*a1j4sxrr+(a1j4ry*a1j4sxx+2*a1j4sx*a1j4rxy+2*a1j4sxy*
     & a1j4rx+a1j4sy*a1j4rxx)*a1j4sxrs+(a1j4sy*a1j4sxx+2*a1j4sxy*
     & a1j4sx)*a1j4sxss+a1j4rxxy*a1j4sxr+a1j4sxxy*a1j4sxs
      t1 = a1j4ry**2
      t4 = a1j4sy*a1j4ry
      t8 = a1j4sy*a1j4rx+a1j4ry*a1j4sx
      t16 = a1j4sy**2
      a1j4sxxyy = t1*a1j4rx*a1j4sxrrr+(t4*a1j4rx+a1j4ry*t8)*a1j4sxrrs+(
     & t4*a1j4sx+a1j4sy*t8)*a1j4sxrss+t16*a1j4sx*a1j4sxsss+(a1j4ryy*
     & a1j4rx+2*a1j4ry*a1j4rxy)*a1j4sxrr+(2*a1j4ry*a1j4sxy+2*a1j4sy*
     & a1j4rxy+a1j4ryy*a1j4sx+a1j4syy*a1j4rx)*a1j4sxrs+(a1j4syy*
     & a1j4sx+2*a1j4sy*a1j4sxy)*a1j4sxss+a1j4rxyy*a1j4sxr+a1j4sxyy*
     & a1j4sxs
      t1 = a1j4ry**2
      t7 = a1j4sy**2
      a1j4sxyyy = a1j4ry*t1*a1j4sxrrr+3*t1*a1j4sy*a1j4sxrrs+3*a1j4ry*
     & t7*a1j4sxrss+t7*a1j4sy*a1j4sxsss+3*a1j4ry*a1j4ryy*a1j4sxrr+(3*
     & a1j4syy*a1j4ry+3*a1j4sy*a1j4ryy)*a1j4sxrs+3*a1j4syy*a1j4sy*
     & a1j4sxss+a1j4ryyy*a1j4sxr+a1j4syyy*a1j4sxs
      t1 = a1j4rx**2
      t7 = a1j4sx**2
      a1j4ryxxx = t1*a1j4rx*a1j4ryrrr+3*t1*a1j4sx*a1j4ryrrs+3*a1j4rx*
     & t7*a1j4ryrss+t7*a1j4sx*a1j4rysss+3*a1j4rx*a1j4rxx*a1j4ryrr+(3*
     & a1j4sxx*a1j4rx+3*a1j4sx*a1j4rxx)*a1j4ryrs+3*a1j4sxx*a1j4sx*
     & a1j4ryss+a1j4rxxx*a1j4ryr+a1j4sxxx*a1j4rys
      t1 = a1j4rx**2
      t10 = a1j4sx**2
      a1j4ryxxy = a1j4ry*t1*a1j4ryrrr+(a1j4sy*t1+2*a1j4ry*a1j4sx*
     & a1j4rx)*a1j4ryrrs+(a1j4ry*t10+2*a1j4sy*a1j4sx*a1j4rx)*
     & a1j4ryrss+a1j4sy*t10*a1j4rysss+(2*a1j4rxy*a1j4rx+a1j4ry*
     & a1j4rxx)*a1j4ryrr+(a1j4ry*a1j4sxx+2*a1j4sx*a1j4rxy+2*a1j4sxy*
     & a1j4rx+a1j4sy*a1j4rxx)*a1j4ryrs+(a1j4sy*a1j4sxx+2*a1j4sxy*
     & a1j4sx)*a1j4ryss+a1j4rxxy*a1j4ryr+a1j4sxxy*a1j4rys
      t1 = a1j4ry**2
      t4 = a1j4sy*a1j4ry
      t8 = a1j4sy*a1j4rx+a1j4ry*a1j4sx
      t16 = a1j4sy**2
      a1j4ryxyy = t1*a1j4rx*a1j4ryrrr+(t4*a1j4rx+a1j4ry*t8)*a1j4ryrrs+(
     & t4*a1j4sx+a1j4sy*t8)*a1j4ryrss+t16*a1j4sx*a1j4rysss+(a1j4ryy*
     & a1j4rx+2*a1j4ry*a1j4rxy)*a1j4ryrr+(2*a1j4ry*a1j4sxy+2*a1j4sy*
     & a1j4rxy+a1j4ryy*a1j4sx+a1j4syy*a1j4rx)*a1j4ryrs+(a1j4syy*
     & a1j4sx+2*a1j4sy*a1j4sxy)*a1j4ryss+a1j4rxyy*a1j4ryr+a1j4sxyy*
     & a1j4rys
      t1 = a1j4ry**2
      t7 = a1j4sy**2
      a1j4ryyyy = a1j4ry*t1*a1j4ryrrr+3*t1*a1j4sy*a1j4ryrrs+3*a1j4ry*
     & t7*a1j4ryrss+t7*a1j4sy*a1j4rysss+3*a1j4ry*a1j4ryy*a1j4ryrr+(3*
     & a1j4syy*a1j4ry+3*a1j4sy*a1j4ryy)*a1j4ryrs+3*a1j4syy*a1j4sy*
     & a1j4ryss+a1j4ryyy*a1j4ryr+a1j4syyy*a1j4rys
      t1 = a1j4rx**2
      t7 = a1j4sx**2
      a1j4syxxx = t1*a1j4rx*a1j4syrrr+3*t1*a1j4sx*a1j4syrrs+3*a1j4rx*
     & t7*a1j4syrss+t7*a1j4sx*a1j4sysss+3*a1j4rx*a1j4rxx*a1j4syrr+(3*
     & a1j4sxx*a1j4rx+3*a1j4sx*a1j4rxx)*a1j4syrs+3*a1j4sxx*a1j4sx*
     & a1j4syss+a1j4rxxx*a1j4syr+a1j4sxxx*a1j4sys
      t1 = a1j4rx**2
      t10 = a1j4sx**2
      a1j4syxxy = a1j4ry*t1*a1j4syrrr+(a1j4sy*t1+2*a1j4ry*a1j4sx*
     & a1j4rx)*a1j4syrrs+(a1j4ry*t10+2*a1j4sy*a1j4sx*a1j4rx)*
     & a1j4syrss+a1j4sy*t10*a1j4sysss+(2*a1j4rxy*a1j4rx+a1j4ry*
     & a1j4rxx)*a1j4syrr+(a1j4ry*a1j4sxx+2*a1j4sx*a1j4rxy+2*a1j4sxy*
     & a1j4rx+a1j4sy*a1j4rxx)*a1j4syrs+(a1j4sy*a1j4sxx+2*a1j4sxy*
     & a1j4sx)*a1j4syss+a1j4rxxy*a1j4syr+a1j4sxxy*a1j4sys
      t1 = a1j4ry**2
      t4 = a1j4sy*a1j4ry
      t8 = a1j4sy*a1j4rx+a1j4ry*a1j4sx
      t16 = a1j4sy**2
      a1j4syxyy = t1*a1j4rx*a1j4syrrr+(t4*a1j4rx+a1j4ry*t8)*a1j4syrrs+(
     & t4*a1j4sx+a1j4sy*t8)*a1j4syrss+t16*a1j4sx*a1j4sysss+(a1j4ryy*
     & a1j4rx+2*a1j4ry*a1j4rxy)*a1j4syrr+(2*a1j4ry*a1j4sxy+2*a1j4sy*
     & a1j4rxy+a1j4ryy*a1j4sx+a1j4syy*a1j4rx)*a1j4syrs+(a1j4syy*
     & a1j4sx+2*a1j4sy*a1j4sxy)*a1j4syss+a1j4rxyy*a1j4syr+a1j4sxyy*
     & a1j4sys
      t1 = a1j4ry**2
      t7 = a1j4sy**2
      a1j4syyyy = a1j4ry*t1*a1j4syrrr+3*t1*a1j4sy*a1j4syrrs+3*a1j4ry*
     & t7*a1j4syrss+t7*a1j4sy*a1j4sysss+3*a1j4ry*a1j4ryy*a1j4syrr+(3*
     & a1j4syy*a1j4ry+3*a1j4sy*a1j4ryy)*a1j4syrs+3*a1j4syy*a1j4sy*
     & a1j4syss+a1j4ryyy*a1j4syr+a1j4syyy*a1j4sys
      a1j2rx = rsxy1(i1,i2,i3,0,0)
      a1j2rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(2.*dr1(
     & 0))
      a1j2rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(2.*dr1(
     & 1))
      a1j2rxrr = (rsxy1(i1-1,i2,i3,0,0)-2.*rsxy1(i1,i2,i3,0,0)+rsxy1(
     & i1+1,i2,i3,0,0))/(dr1(0)**2)
      a1j2rxrs = (-(-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,0,0))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,i3,0,0))/(
     & 2.*dr1(1)))/(2.*dr1(0))
      a1j2rxss = (rsxy1(i1,i2-1,i3,0,0)-2.*rsxy1(i1,i2,i3,0,0)+rsxy1(
     & i1,i2+1,i3,0,0))/(dr1(1)**2)
      a1j2rxrrr = (-rsxy1(i1-2,i2,i3,0,0)+2.*rsxy1(i1-1,i2,i3,0,0)-2.*
     & rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+2,i2,i3,0,0))/(2.*dr1(0)**3)
      a1j2rxrrs = ((-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,0,0))/(
     & 2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,i3,0,0))/(
     & 2.*dr1(1)))/(dr1(0)**2)
      a1j2rxrss = (-(rsxy1(i1-1,i2-1,i3,0,0)-2.*rsxy1(i1-1,i2,i3,0,0)+
     & rsxy1(i1-1,i2+1,i3,0,0))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,0,0)-
     & 2.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+1,i2+1,i3,0,0))/(dr1(1)**2))
     & /(2.*dr1(0))
      a1j2rxsss = (-rsxy1(i1,i2-2,i3,0,0)+2.*rsxy1(i1,i2-1,i3,0,0)-2.*
     & rsxy1(i1,i2+1,i3,0,0)+rsxy1(i1,i2+2,i3,0,0))/(2.*dr1(1)**3)
      a1j2rxrrrr = (rsxy1(i1-2,i2,i3,0,0)-4.*rsxy1(i1-1,i2,i3,0,0)+6.*
     & rsxy1(i1,i2,i3,0,0)-4.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+2,i2,i3,
     & 0,0))/(dr1(0)**4)
      a1j2rxrrrs = (-(-rsxy1(i1-2,i2-1,i3,0,0)+rsxy1(i1-2,i2+1,i3,0,0))
     & /(2.*dr1(1))+2.*(-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,0,
     & 0))/(2.*dr1(1))-2.*(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,
     & i3,0,0))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,0,0)+rsxy1(i1+2,i2+1,
     & i3,0,0))/(2.*dr1(1)))/(2.*dr1(0)**3)
      a1j2rxrrss = ((rsxy1(i1-1,i2-1,i3,0,0)-2.*rsxy1(i1-1,i2,i3,0,0)+
     & rsxy1(i1-1,i2+1,i3,0,0))/(dr1(1)**2)-2.*(rsxy1(i1,i2-1,i3,0,0)-
     & 2.*rsxy1(i1,i2,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(dr1(1)**2)+(
     & rsxy1(i1+1,i2-1,i3,0,0)-2.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+1,i2+
     & 1,i3,0,0))/(dr1(1)**2))/(dr1(0)**2)
      a1j2rxrsss = (-(-rsxy1(i1-1,i2-2,i3,0,0)+2.*rsxy1(i1-1,i2-1,i3,0,
     & 0)-2.*rsxy1(i1-1,i2+1,i3,0,0)+rsxy1(i1-1,i2+2,i3,0,0))/(2.*dr1(
     & 1)**3)+(-rsxy1(i1+1,i2-2,i3,0,0)+2.*rsxy1(i1+1,i2-1,i3,0,0)-2.*
     & rsxy1(i1+1,i2+1,i3,0,0)+rsxy1(i1+1,i2+2,i3,0,0))/(2.*dr1(1)**3)
     & )/(2.*dr1(0))
      a1j2rxssss = (rsxy1(i1,i2-2,i3,0,0)-4.*rsxy1(i1,i2-1,i3,0,0)+6.*
     & rsxy1(i1,i2,i3,0,0)-4.*rsxy1(i1,i2+1,i3,0,0)+rsxy1(i1,i2+2,i3,
     & 0,0))/(dr1(1)**4)
      a1j2rxrrrrr = (-rsxy1(i1-3,i2,i3,0,0)+4.*rsxy1(i1-2,i2,i3,0,0)-
     & 5.*rsxy1(i1-1,i2,i3,0,0)+5.*rsxy1(i1+1,i2,i3,0,0)-4.*rsxy1(i1+
     & 2,i2,i3,0,0)+rsxy1(i1+3,i2,i3,0,0))/(2.*dr1(0)**5)
      a1j2rxrrrrs = ((-rsxy1(i1-2,i2-1,i3,0,0)+rsxy1(i1-2,i2+1,i3,0,0))
     & /(2.*dr1(1))-4.*(-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,0,
     & 0))/(2.*dr1(1))+6.*(-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,
     & 0))/(2.*dr1(1))-4.*(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,
     & i3,0,0))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,0,0)+rsxy1(i1+2,i2+1,
     & i3,0,0))/(2.*dr1(1)))/(dr1(0)**4)
      a1j2rxrrrss = (-(rsxy1(i1-2,i2-1,i3,0,0)-2.*rsxy1(i1-2,i2,i3,0,0)
     & +rsxy1(i1-2,i2+1,i3,0,0))/(dr1(1)**2)+2.*(rsxy1(i1-1,i2-1,i3,0,
     & 0)-2.*rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1-1,i2+1,i3,0,0))/(dr1(1)**
     & 2)-2.*(rsxy1(i1+1,i2-1,i3,0,0)-2.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(
     & i1+1,i2+1,i3,0,0))/(dr1(1)**2)+(rsxy1(i1+2,i2-1,i3,0,0)-2.*
     & rsxy1(i1+2,i2,i3,0,0)+rsxy1(i1+2,i2+1,i3,0,0))/(dr1(1)**2))/(
     & 2.*dr1(0)**3)
      a1j2rxrrsss = ((-rsxy1(i1-1,i2-2,i3,0,0)+2.*rsxy1(i1-1,i2-1,i3,0,
     & 0)-2.*rsxy1(i1-1,i2+1,i3,0,0)+rsxy1(i1-1,i2+2,i3,0,0))/(2.*dr1(
     & 1)**3)-2.*(-rsxy1(i1,i2-2,i3,0,0)+2.*rsxy1(i1,i2-1,i3,0,0)-2.*
     & rsxy1(i1,i2+1,i3,0,0)+rsxy1(i1,i2+2,i3,0,0))/(2.*dr1(1)**3)+(-
     & rsxy1(i1+1,i2-2,i3,0,0)+2.*rsxy1(i1+1,i2-1,i3,0,0)-2.*rsxy1(i1+
     & 1,i2+1,i3,0,0)+rsxy1(i1+1,i2+2,i3,0,0))/(2.*dr1(1)**3))/(dr1(0)
     & **2)
      a1j2rxrssss = (-(rsxy1(i1-1,i2-2,i3,0,0)-4.*rsxy1(i1-1,i2-1,i3,0,
     & 0)+6.*rsxy1(i1-1,i2,i3,0,0)-4.*rsxy1(i1-1,i2+1,i3,0,0)+rsxy1(
     & i1-1,i2+2,i3,0,0))/(dr1(1)**4)+(rsxy1(i1+1,i2-2,i3,0,0)-4.*
     & rsxy1(i1+1,i2-1,i3,0,0)+6.*rsxy1(i1+1,i2,i3,0,0)-4.*rsxy1(i1+1,
     & i2+1,i3,0,0)+rsxy1(i1+1,i2+2,i3,0,0))/(dr1(1)**4))/(2.*dr1(0))
      a1j2rxsssss = (-rsxy1(i1,i2-3,i3,0,0)+4.*rsxy1(i1,i2-2,i3,0,0)-
     & 5.*rsxy1(i1,i2-1,i3,0,0)+5.*rsxy1(i1,i2+1,i3,0,0)-4.*rsxy1(i1,
     & i2+2,i3,0,0)+rsxy1(i1,i2+3,i3,0,0))/(2.*dr1(1)**5)
      a1j2sx = rsxy1(i1,i2,i3,1,0)
      a1j2sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(2.*dr1(
     & 0))
      a1j2sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(2.*dr1(
     & 1))
      a1j2sxrr = (rsxy1(i1-1,i2,i3,1,0)-2.*rsxy1(i1,i2,i3,1,0)+rsxy1(
     & i1+1,i2,i3,1,0))/(dr1(0)**2)
      a1j2sxrs = (-(-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,1,0))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,i3,1,0))/(
     & 2.*dr1(1)))/(2.*dr1(0))
      a1j2sxss = (rsxy1(i1,i2-1,i3,1,0)-2.*rsxy1(i1,i2,i3,1,0)+rsxy1(
     & i1,i2+1,i3,1,0))/(dr1(1)**2)
      a1j2sxrrr = (-rsxy1(i1-2,i2,i3,1,0)+2.*rsxy1(i1-1,i2,i3,1,0)-2.*
     & rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+2,i2,i3,1,0))/(2.*dr1(0)**3)
      a1j2sxrrs = ((-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,1,0))/(
     & 2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,i3,1,0))/(
     & 2.*dr1(1)))/(dr1(0)**2)
      a1j2sxrss = (-(rsxy1(i1-1,i2-1,i3,1,0)-2.*rsxy1(i1-1,i2,i3,1,0)+
     & rsxy1(i1-1,i2+1,i3,1,0))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,1,0)-
     & 2.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+1,i2+1,i3,1,0))/(dr1(1)**2))
     & /(2.*dr1(0))
      a1j2sxsss = (-rsxy1(i1,i2-2,i3,1,0)+2.*rsxy1(i1,i2-1,i3,1,0)-2.*
     & rsxy1(i1,i2+1,i3,1,0)+rsxy1(i1,i2+2,i3,1,0))/(2.*dr1(1)**3)
      a1j2sxrrrr = (rsxy1(i1-2,i2,i3,1,0)-4.*rsxy1(i1-1,i2,i3,1,0)+6.*
     & rsxy1(i1,i2,i3,1,0)-4.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+2,i2,i3,
     & 1,0))/(dr1(0)**4)
      a1j2sxrrrs = (-(-rsxy1(i1-2,i2-1,i3,1,0)+rsxy1(i1-2,i2+1,i3,1,0))
     & /(2.*dr1(1))+2.*(-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,1,
     & 0))/(2.*dr1(1))-2.*(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,
     & i3,1,0))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,1,0)+rsxy1(i1+2,i2+1,
     & i3,1,0))/(2.*dr1(1)))/(2.*dr1(0)**3)
      a1j2sxrrss = ((rsxy1(i1-1,i2-1,i3,1,0)-2.*rsxy1(i1-1,i2,i3,1,0)+
     & rsxy1(i1-1,i2+1,i3,1,0))/(dr1(1)**2)-2.*(rsxy1(i1,i2-1,i3,1,0)-
     & 2.*rsxy1(i1,i2,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(dr1(1)**2)+(
     & rsxy1(i1+1,i2-1,i3,1,0)-2.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+1,i2+
     & 1,i3,1,0))/(dr1(1)**2))/(dr1(0)**2)
      a1j2sxrsss = (-(-rsxy1(i1-1,i2-2,i3,1,0)+2.*rsxy1(i1-1,i2-1,i3,1,
     & 0)-2.*rsxy1(i1-1,i2+1,i3,1,0)+rsxy1(i1-1,i2+2,i3,1,0))/(2.*dr1(
     & 1)**3)+(-rsxy1(i1+1,i2-2,i3,1,0)+2.*rsxy1(i1+1,i2-1,i3,1,0)-2.*
     & rsxy1(i1+1,i2+1,i3,1,0)+rsxy1(i1+1,i2+2,i3,1,0))/(2.*dr1(1)**3)
     & )/(2.*dr1(0))
      a1j2sxssss = (rsxy1(i1,i2-2,i3,1,0)-4.*rsxy1(i1,i2-1,i3,1,0)+6.*
     & rsxy1(i1,i2,i3,1,0)-4.*rsxy1(i1,i2+1,i3,1,0)+rsxy1(i1,i2+2,i3,
     & 1,0))/(dr1(1)**4)
      a1j2sxrrrrr = (-rsxy1(i1-3,i2,i3,1,0)+4.*rsxy1(i1-2,i2,i3,1,0)-
     & 5.*rsxy1(i1-1,i2,i3,1,0)+5.*rsxy1(i1+1,i2,i3,1,0)-4.*rsxy1(i1+
     & 2,i2,i3,1,0)+rsxy1(i1+3,i2,i3,1,0))/(2.*dr1(0)**5)
      a1j2sxrrrrs = ((-rsxy1(i1-2,i2-1,i3,1,0)+rsxy1(i1-2,i2+1,i3,1,0))
     & /(2.*dr1(1))-4.*(-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,1,
     & 0))/(2.*dr1(1))+6.*(-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,
     & 0))/(2.*dr1(1))-4.*(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,
     & i3,1,0))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,1,0)+rsxy1(i1+2,i2+1,
     & i3,1,0))/(2.*dr1(1)))/(dr1(0)**4)
      a1j2sxrrrss = (-(rsxy1(i1-2,i2-1,i3,1,0)-2.*rsxy1(i1-2,i2,i3,1,0)
     & +rsxy1(i1-2,i2+1,i3,1,0))/(dr1(1)**2)+2.*(rsxy1(i1-1,i2-1,i3,1,
     & 0)-2.*rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1-1,i2+1,i3,1,0))/(dr1(1)**
     & 2)-2.*(rsxy1(i1+1,i2-1,i3,1,0)-2.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(
     & i1+1,i2+1,i3,1,0))/(dr1(1)**2)+(rsxy1(i1+2,i2-1,i3,1,0)-2.*
     & rsxy1(i1+2,i2,i3,1,0)+rsxy1(i1+2,i2+1,i3,1,0))/(dr1(1)**2))/(
     & 2.*dr1(0)**3)
      a1j2sxrrsss = ((-rsxy1(i1-1,i2-2,i3,1,0)+2.*rsxy1(i1-1,i2-1,i3,1,
     & 0)-2.*rsxy1(i1-1,i2+1,i3,1,0)+rsxy1(i1-1,i2+2,i3,1,0))/(2.*dr1(
     & 1)**3)-2.*(-rsxy1(i1,i2-2,i3,1,0)+2.*rsxy1(i1,i2-1,i3,1,0)-2.*
     & rsxy1(i1,i2+1,i3,1,0)+rsxy1(i1,i2+2,i3,1,0))/(2.*dr1(1)**3)+(-
     & rsxy1(i1+1,i2-2,i3,1,0)+2.*rsxy1(i1+1,i2-1,i3,1,0)-2.*rsxy1(i1+
     & 1,i2+1,i3,1,0)+rsxy1(i1+1,i2+2,i3,1,0))/(2.*dr1(1)**3))/(dr1(0)
     & **2)
      a1j2sxrssss = (-(rsxy1(i1-1,i2-2,i3,1,0)-4.*rsxy1(i1-1,i2-1,i3,1,
     & 0)+6.*rsxy1(i1-1,i2,i3,1,0)-4.*rsxy1(i1-1,i2+1,i3,1,0)+rsxy1(
     & i1-1,i2+2,i3,1,0))/(dr1(1)**4)+(rsxy1(i1+1,i2-2,i3,1,0)-4.*
     & rsxy1(i1+1,i2-1,i3,1,0)+6.*rsxy1(i1+1,i2,i3,1,0)-4.*rsxy1(i1+1,
     & i2+1,i3,1,0)+rsxy1(i1+1,i2+2,i3,1,0))/(dr1(1)**4))/(2.*dr1(0))
      a1j2sxsssss = (-rsxy1(i1,i2-3,i3,1,0)+4.*rsxy1(i1,i2-2,i3,1,0)-
     & 5.*rsxy1(i1,i2-1,i3,1,0)+5.*rsxy1(i1,i2+1,i3,1,0)-4.*rsxy1(i1,
     & i2+2,i3,1,0)+rsxy1(i1,i2+3,i3,1,0))/(2.*dr1(1)**5)
      a1j2ry = rsxy1(i1,i2,i3,0,1)
      a1j2ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(2.*dr1(
     & 0))
      a1j2rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(2.*dr1(
     & 1))
      a1j2ryrr = (rsxy1(i1-1,i2,i3,0,1)-2.*rsxy1(i1,i2,i3,0,1)+rsxy1(
     & i1+1,i2,i3,0,1))/(dr1(0)**2)
      a1j2ryrs = (-(-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,0,1))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,i3,0,1))/(
     & 2.*dr1(1)))/(2.*dr1(0))
      a1j2ryss = (rsxy1(i1,i2-1,i3,0,1)-2.*rsxy1(i1,i2,i3,0,1)+rsxy1(
     & i1,i2+1,i3,0,1))/(dr1(1)**2)
      a1j2ryrrr = (-rsxy1(i1-2,i2,i3,0,1)+2.*rsxy1(i1-1,i2,i3,0,1)-2.*
     & rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+2,i2,i3,0,1))/(2.*dr1(0)**3)
      a1j2ryrrs = ((-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,0,1))/(
     & 2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,i3,0,1))/(
     & 2.*dr1(1)))/(dr1(0)**2)
      a1j2ryrss = (-(rsxy1(i1-1,i2-1,i3,0,1)-2.*rsxy1(i1-1,i2,i3,0,1)+
     & rsxy1(i1-1,i2+1,i3,0,1))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,0,1)-
     & 2.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+1,i2+1,i3,0,1))/(dr1(1)**2))
     & /(2.*dr1(0))
      a1j2rysss = (-rsxy1(i1,i2-2,i3,0,1)+2.*rsxy1(i1,i2-1,i3,0,1)-2.*
     & rsxy1(i1,i2+1,i3,0,1)+rsxy1(i1,i2+2,i3,0,1))/(2.*dr1(1)**3)
      a1j2ryrrrr = (rsxy1(i1-2,i2,i3,0,1)-4.*rsxy1(i1-1,i2,i3,0,1)+6.*
     & rsxy1(i1,i2,i3,0,1)-4.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+2,i2,i3,
     & 0,1))/(dr1(0)**4)
      a1j2ryrrrs = (-(-rsxy1(i1-2,i2-1,i3,0,1)+rsxy1(i1-2,i2+1,i3,0,1))
     & /(2.*dr1(1))+2.*(-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,0,
     & 1))/(2.*dr1(1))-2.*(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,
     & i3,0,1))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,0,1)+rsxy1(i1+2,i2+1,
     & i3,0,1))/(2.*dr1(1)))/(2.*dr1(0)**3)
      a1j2ryrrss = ((rsxy1(i1-1,i2-1,i3,0,1)-2.*rsxy1(i1-1,i2,i3,0,1)+
     & rsxy1(i1-1,i2+1,i3,0,1))/(dr1(1)**2)-2.*(rsxy1(i1,i2-1,i3,0,1)-
     & 2.*rsxy1(i1,i2,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(dr1(1)**2)+(
     & rsxy1(i1+1,i2-1,i3,0,1)-2.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+1,i2+
     & 1,i3,0,1))/(dr1(1)**2))/(dr1(0)**2)
      a1j2ryrsss = (-(-rsxy1(i1-1,i2-2,i3,0,1)+2.*rsxy1(i1-1,i2-1,i3,0,
     & 1)-2.*rsxy1(i1-1,i2+1,i3,0,1)+rsxy1(i1-1,i2+2,i3,0,1))/(2.*dr1(
     & 1)**3)+(-rsxy1(i1+1,i2-2,i3,0,1)+2.*rsxy1(i1+1,i2-1,i3,0,1)-2.*
     & rsxy1(i1+1,i2+1,i3,0,1)+rsxy1(i1+1,i2+2,i3,0,1))/(2.*dr1(1)**3)
     & )/(2.*dr1(0))
      a1j2ryssss = (rsxy1(i1,i2-2,i3,0,1)-4.*rsxy1(i1,i2-1,i3,0,1)+6.*
     & rsxy1(i1,i2,i3,0,1)-4.*rsxy1(i1,i2+1,i3,0,1)+rsxy1(i1,i2+2,i3,
     & 0,1))/(dr1(1)**4)
      a1j2ryrrrrr = (-rsxy1(i1-3,i2,i3,0,1)+4.*rsxy1(i1-2,i2,i3,0,1)-
     & 5.*rsxy1(i1-1,i2,i3,0,1)+5.*rsxy1(i1+1,i2,i3,0,1)-4.*rsxy1(i1+
     & 2,i2,i3,0,1)+rsxy1(i1+3,i2,i3,0,1))/(2.*dr1(0)**5)
      a1j2ryrrrrs = ((-rsxy1(i1-2,i2-1,i3,0,1)+rsxy1(i1-2,i2+1,i3,0,1))
     & /(2.*dr1(1))-4.*(-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,0,
     & 1))/(2.*dr1(1))+6.*(-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,
     & 1))/(2.*dr1(1))-4.*(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,
     & i3,0,1))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,0,1)+rsxy1(i1+2,i2+1,
     & i3,0,1))/(2.*dr1(1)))/(dr1(0)**4)
      a1j2ryrrrss = (-(rsxy1(i1-2,i2-1,i3,0,1)-2.*rsxy1(i1-2,i2,i3,0,1)
     & +rsxy1(i1-2,i2+1,i3,0,1))/(dr1(1)**2)+2.*(rsxy1(i1-1,i2-1,i3,0,
     & 1)-2.*rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1-1,i2+1,i3,0,1))/(dr1(1)**
     & 2)-2.*(rsxy1(i1+1,i2-1,i3,0,1)-2.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(
     & i1+1,i2+1,i3,0,1))/(dr1(1)**2)+(rsxy1(i1+2,i2-1,i3,0,1)-2.*
     & rsxy1(i1+2,i2,i3,0,1)+rsxy1(i1+2,i2+1,i3,0,1))/(dr1(1)**2))/(
     & 2.*dr1(0)**3)
      a1j2ryrrsss = ((-rsxy1(i1-1,i2-2,i3,0,1)+2.*rsxy1(i1-1,i2-1,i3,0,
     & 1)-2.*rsxy1(i1-1,i2+1,i3,0,1)+rsxy1(i1-1,i2+2,i3,0,1))/(2.*dr1(
     & 1)**3)-2.*(-rsxy1(i1,i2-2,i3,0,1)+2.*rsxy1(i1,i2-1,i3,0,1)-2.*
     & rsxy1(i1,i2+1,i3,0,1)+rsxy1(i1,i2+2,i3,0,1))/(2.*dr1(1)**3)+(-
     & rsxy1(i1+1,i2-2,i3,0,1)+2.*rsxy1(i1+1,i2-1,i3,0,1)-2.*rsxy1(i1+
     & 1,i2+1,i3,0,1)+rsxy1(i1+1,i2+2,i3,0,1))/(2.*dr1(1)**3))/(dr1(0)
     & **2)
      a1j2ryrssss = (-(rsxy1(i1-1,i2-2,i3,0,1)-4.*rsxy1(i1-1,i2-1,i3,0,
     & 1)+6.*rsxy1(i1-1,i2,i3,0,1)-4.*rsxy1(i1-1,i2+1,i3,0,1)+rsxy1(
     & i1-1,i2+2,i3,0,1))/(dr1(1)**4)+(rsxy1(i1+1,i2-2,i3,0,1)-4.*
     & rsxy1(i1+1,i2-1,i3,0,1)+6.*rsxy1(i1+1,i2,i3,0,1)-4.*rsxy1(i1+1,
     & i2+1,i3,0,1)+rsxy1(i1+1,i2+2,i3,0,1))/(dr1(1)**4))/(2.*dr1(0))
      a1j2rysssss = (-rsxy1(i1,i2-3,i3,0,1)+4.*rsxy1(i1,i2-2,i3,0,1)-
     & 5.*rsxy1(i1,i2-1,i3,0,1)+5.*rsxy1(i1,i2+1,i3,0,1)-4.*rsxy1(i1,
     & i2+2,i3,0,1)+rsxy1(i1,i2+3,i3,0,1))/(2.*dr1(1)**5)
      a1j2sy = rsxy1(i1,i2,i3,1,1)
      a1j2syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(2.*dr1(
     & 0))
      a1j2sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(2.*dr1(
     & 1))
      a1j2syrr = (rsxy1(i1-1,i2,i3,1,1)-2.*rsxy1(i1,i2,i3,1,1)+rsxy1(
     & i1+1,i2,i3,1,1))/(dr1(0)**2)
      a1j2syrs = (-(-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,1,1))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,i3,1,1))/(
     & 2.*dr1(1)))/(2.*dr1(0))
      a1j2syss = (rsxy1(i1,i2-1,i3,1,1)-2.*rsxy1(i1,i2,i3,1,1)+rsxy1(
     & i1,i2+1,i3,1,1))/(dr1(1)**2)
      a1j2syrrr = (-rsxy1(i1-2,i2,i3,1,1)+2.*rsxy1(i1-1,i2,i3,1,1)-2.*
     & rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+2,i2,i3,1,1))/(2.*dr1(0)**3)
      a1j2syrrs = ((-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,1,1))/(
     & 2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,i3,1,1))/(
     & 2.*dr1(1)))/(dr1(0)**2)
      a1j2syrss = (-(rsxy1(i1-1,i2-1,i3,1,1)-2.*rsxy1(i1-1,i2,i3,1,1)+
     & rsxy1(i1-1,i2+1,i3,1,1))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,1,1)-
     & 2.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+1,i2+1,i3,1,1))/(dr1(1)**2))
     & /(2.*dr1(0))
      a1j2sysss = (-rsxy1(i1,i2-2,i3,1,1)+2.*rsxy1(i1,i2-1,i3,1,1)-2.*
     & rsxy1(i1,i2+1,i3,1,1)+rsxy1(i1,i2+2,i3,1,1))/(2.*dr1(1)**3)
      a1j2syrrrr = (rsxy1(i1-2,i2,i3,1,1)-4.*rsxy1(i1-1,i2,i3,1,1)+6.*
     & rsxy1(i1,i2,i3,1,1)-4.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+2,i2,i3,
     & 1,1))/(dr1(0)**4)
      a1j2syrrrs = (-(-rsxy1(i1-2,i2-1,i3,1,1)+rsxy1(i1-2,i2+1,i3,1,1))
     & /(2.*dr1(1))+2.*(-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,1,
     & 1))/(2.*dr1(1))-2.*(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,
     & i3,1,1))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,1,1)+rsxy1(i1+2,i2+1,
     & i3,1,1))/(2.*dr1(1)))/(2.*dr1(0)**3)
      a1j2syrrss = ((rsxy1(i1-1,i2-1,i3,1,1)-2.*rsxy1(i1-1,i2,i3,1,1)+
     & rsxy1(i1-1,i2+1,i3,1,1))/(dr1(1)**2)-2.*(rsxy1(i1,i2-1,i3,1,1)-
     & 2.*rsxy1(i1,i2,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(dr1(1)**2)+(
     & rsxy1(i1+1,i2-1,i3,1,1)-2.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+1,i2+
     & 1,i3,1,1))/(dr1(1)**2))/(dr1(0)**2)
      a1j2syrsss = (-(-rsxy1(i1-1,i2-2,i3,1,1)+2.*rsxy1(i1-1,i2-1,i3,1,
     & 1)-2.*rsxy1(i1-1,i2+1,i3,1,1)+rsxy1(i1-1,i2+2,i3,1,1))/(2.*dr1(
     & 1)**3)+(-rsxy1(i1+1,i2-2,i3,1,1)+2.*rsxy1(i1+1,i2-1,i3,1,1)-2.*
     & rsxy1(i1+1,i2+1,i3,1,1)+rsxy1(i1+1,i2+2,i3,1,1))/(2.*dr1(1)**3)
     & )/(2.*dr1(0))
      a1j2syssss = (rsxy1(i1,i2-2,i3,1,1)-4.*rsxy1(i1,i2-1,i3,1,1)+6.*
     & rsxy1(i1,i2,i3,1,1)-4.*rsxy1(i1,i2+1,i3,1,1)+rsxy1(i1,i2+2,i3,
     & 1,1))/(dr1(1)**4)
      a1j2syrrrrr = (-rsxy1(i1-3,i2,i3,1,1)+4.*rsxy1(i1-2,i2,i3,1,1)-
     & 5.*rsxy1(i1-1,i2,i3,1,1)+5.*rsxy1(i1+1,i2,i3,1,1)-4.*rsxy1(i1+
     & 2,i2,i3,1,1)+rsxy1(i1+3,i2,i3,1,1))/(2.*dr1(0)**5)
      a1j2syrrrrs = ((-rsxy1(i1-2,i2-1,i3,1,1)+rsxy1(i1-2,i2+1,i3,1,1))
     & /(2.*dr1(1))-4.*(-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,1,
     & 1))/(2.*dr1(1))+6.*(-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,
     & 1))/(2.*dr1(1))-4.*(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,
     & i3,1,1))/(2.*dr1(1))+(-rsxy1(i1+2,i2-1,i3,1,1)+rsxy1(i1+2,i2+1,
     & i3,1,1))/(2.*dr1(1)))/(dr1(0)**4)
      a1j2syrrrss = (-(rsxy1(i1-2,i2-1,i3,1,1)-2.*rsxy1(i1-2,i2,i3,1,1)
     & +rsxy1(i1-2,i2+1,i3,1,1))/(dr1(1)**2)+2.*(rsxy1(i1-1,i2-1,i3,1,
     & 1)-2.*rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1-1,i2+1,i3,1,1))/(dr1(1)**
     & 2)-2.*(rsxy1(i1+1,i2-1,i3,1,1)-2.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(
     & i1+1,i2+1,i3,1,1))/(dr1(1)**2)+(rsxy1(i1+2,i2-1,i3,1,1)-2.*
     & rsxy1(i1+2,i2,i3,1,1)+rsxy1(i1+2,i2+1,i3,1,1))/(dr1(1)**2))/(
     & 2.*dr1(0)**3)
      a1j2syrrsss = ((-rsxy1(i1-1,i2-2,i3,1,1)+2.*rsxy1(i1-1,i2-1,i3,1,
     & 1)-2.*rsxy1(i1-1,i2+1,i3,1,1)+rsxy1(i1-1,i2+2,i3,1,1))/(2.*dr1(
     & 1)**3)-2.*(-rsxy1(i1,i2-2,i3,1,1)+2.*rsxy1(i1,i2-1,i3,1,1)-2.*
     & rsxy1(i1,i2+1,i3,1,1)+rsxy1(i1,i2+2,i3,1,1))/(2.*dr1(1)**3)+(-
     & rsxy1(i1+1,i2-2,i3,1,1)+2.*rsxy1(i1+1,i2-1,i3,1,1)-2.*rsxy1(i1+
     & 1,i2+1,i3,1,1)+rsxy1(i1+1,i2+2,i3,1,1))/(2.*dr1(1)**3))/(dr1(0)
     & **2)
      a1j2syrssss = (-(rsxy1(i1-1,i2-2,i3,1,1)-4.*rsxy1(i1-1,i2-1,i3,1,
     & 1)+6.*rsxy1(i1-1,i2,i3,1,1)-4.*rsxy1(i1-1,i2+1,i3,1,1)+rsxy1(
     & i1-1,i2+2,i3,1,1))/(dr1(1)**4)+(rsxy1(i1+1,i2-2,i3,1,1)-4.*
     & rsxy1(i1+1,i2-1,i3,1,1)+6.*rsxy1(i1+1,i2,i3,1,1)-4.*rsxy1(i1+1,
     & i2+1,i3,1,1)+rsxy1(i1+1,i2+2,i3,1,1))/(dr1(1)**4))/(2.*dr1(0))
      a1j2sysssss = (-rsxy1(i1,i2-3,i3,1,1)+4.*rsxy1(i1,i2-2,i3,1,1)-
     & 5.*rsxy1(i1,i2-1,i3,1,1)+5.*rsxy1(i1,i2+1,i3,1,1)-4.*rsxy1(i1,
     & i2+2,i3,1,1)+rsxy1(i1,i2+3,i3,1,1))/(2.*dr1(1)**5)
      a1j2rxx = a1j2rx*a1j2rxr+a1j2sx*a1j2rxs
      a1j2rxy = a1j2ry*a1j2rxr+a1j2sy*a1j2rxs
      a1j2sxx = a1j2rx*a1j2sxr+a1j2sx*a1j2sxs
      a1j2sxy = a1j2ry*a1j2sxr+a1j2sy*a1j2sxs
      a1j2ryx = a1j2rx*a1j2ryr+a1j2sx*a1j2rys
      a1j2ryy = a1j2ry*a1j2ryr+a1j2sy*a1j2rys
      a1j2syx = a1j2rx*a1j2syr+a1j2sx*a1j2sys
      a1j2syy = a1j2ry*a1j2syr+a1j2sy*a1j2sys
      t1 = a1j2rx**2
      t6 = a1j2sx**2
      a1j2rxxx = t1*a1j2rxrr+2*a1j2rx*a1j2sx*a1j2rxrs+t6*a1j2rxss+
     & a1j2rxx*a1j2rxr+a1j2sxx*a1j2rxs
      a1j2rxxy = a1j2ry*a1j2rx*a1j2rxrr+(a1j2sy*a1j2rx+a1j2ry*a1j2sx)*
     & a1j2rxrs+a1j2sy*a1j2sx*a1j2rxss+a1j2rxy*a1j2rxr+a1j2sxy*a1j2rxs
      t1 = a1j2ry**2
      t6 = a1j2sy**2
      a1j2rxyy = t1*a1j2rxrr+2*a1j2ry*a1j2sy*a1j2rxrs+t6*a1j2rxss+
     & a1j2ryy*a1j2rxr+a1j2syy*a1j2rxs
      t1 = a1j2rx**2
      t6 = a1j2sx**2
      a1j2sxxx = t1*a1j2sxrr+2*a1j2rx*a1j2sx*a1j2sxrs+t6*a1j2sxss+
     & a1j2rxx*a1j2sxr+a1j2sxx*a1j2sxs
      a1j2sxxy = a1j2ry*a1j2rx*a1j2sxrr+(a1j2sy*a1j2rx+a1j2ry*a1j2sx)*
     & a1j2sxrs+a1j2sy*a1j2sx*a1j2sxss+a1j2rxy*a1j2sxr+a1j2sxy*a1j2sxs
      t1 = a1j2ry**2
      t6 = a1j2sy**2
      a1j2sxyy = t1*a1j2sxrr+2*a1j2ry*a1j2sy*a1j2sxrs+t6*a1j2sxss+
     & a1j2ryy*a1j2sxr+a1j2syy*a1j2sxs
      t1 = a1j2rx**2
      t6 = a1j2sx**2
      a1j2ryxx = t1*a1j2ryrr+2*a1j2rx*a1j2sx*a1j2ryrs+t6*a1j2ryss+
     & a1j2rxx*a1j2ryr+a1j2sxx*a1j2rys
      a1j2ryxy = a1j2ry*a1j2rx*a1j2ryrr+(a1j2sy*a1j2rx+a1j2ry*a1j2sx)*
     & a1j2ryrs+a1j2sy*a1j2sx*a1j2ryss+a1j2rxy*a1j2ryr+a1j2sxy*a1j2rys
      t1 = a1j2ry**2
      t6 = a1j2sy**2
      a1j2ryyy = t1*a1j2ryrr+2*a1j2ry*a1j2sy*a1j2ryrs+t6*a1j2ryss+
     & a1j2ryy*a1j2ryr+a1j2syy*a1j2rys
      t1 = a1j2rx**2
      t6 = a1j2sx**2
      a1j2syxx = t1*a1j2syrr+2*a1j2rx*a1j2sx*a1j2syrs+t6*a1j2syss+
     & a1j2rxx*a1j2syr+a1j2sxx*a1j2sys
      a1j2syxy = a1j2ry*a1j2rx*a1j2syrr+(a1j2sy*a1j2rx+a1j2ry*a1j2sx)*
     & a1j2syrs+a1j2sy*a1j2sx*a1j2syss+a1j2rxy*a1j2syr+a1j2sxy*a1j2sys
      t1 = a1j2ry**2
      t6 = a1j2sy**2
      a1j2syyy = t1*a1j2syrr+2*a1j2ry*a1j2sy*a1j2syrs+t6*a1j2syss+
     & a1j2ryy*a1j2syr+a1j2syy*a1j2sys
      t1 = a1j2rx**2
      t7 = a1j2sx**2
      a1j2rxxxx = t1*a1j2rx*a1j2rxrrr+3*t1*a1j2sx*a1j2rxrrs+3*a1j2rx*
     & t7*a1j2rxrss+t7*a1j2sx*a1j2rxsss+3*a1j2rx*a1j2rxx*a1j2rxrr+(3*
     & a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx)*a1j2rxrs+3*a1j2sxx*a1j2sx*
     & a1j2rxss+a1j2rxxx*a1j2rxr+a1j2sxxx*a1j2rxs
      t1 = a1j2rx**2
      t10 = a1j2sx**2
      a1j2rxxxy = a1j2ry*t1*a1j2rxrrr+(a1j2sy*t1+2*a1j2ry*a1j2sx*
     & a1j2rx)*a1j2rxrrs+(a1j2ry*t10+2*a1j2sy*a1j2sx*a1j2rx)*
     & a1j2rxrss+a1j2sy*t10*a1j2rxsss+(2*a1j2rxy*a1j2rx+a1j2ry*
     & a1j2rxx)*a1j2rxrr+(a1j2ry*a1j2sxx+2*a1j2sx*a1j2rxy+2*a1j2sxy*
     & a1j2rx+a1j2sy*a1j2rxx)*a1j2rxrs+(a1j2sy*a1j2sxx+2*a1j2sxy*
     & a1j2sx)*a1j2rxss+a1j2rxxy*a1j2rxr+a1j2sxxy*a1j2rxs
      t1 = a1j2ry**2
      t4 = a1j2sy*a1j2ry
      t8 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t16 = a1j2sy**2
      a1j2rxxyy = t1*a1j2rx*a1j2rxrrr+(t4*a1j2rx+a1j2ry*t8)*a1j2rxrrs+(
     & t4*a1j2sx+a1j2sy*t8)*a1j2rxrss+t16*a1j2sx*a1j2rxsss+(a1j2ryy*
     & a1j2rx+2*a1j2ry*a1j2rxy)*a1j2rxrr+(2*a1j2ry*a1j2sxy+2*a1j2sy*
     & a1j2rxy+a1j2ryy*a1j2sx+a1j2syy*a1j2rx)*a1j2rxrs+(a1j2syy*
     & a1j2sx+2*a1j2sy*a1j2sxy)*a1j2rxss+a1j2rxyy*a1j2rxr+a1j2sxyy*
     & a1j2rxs
      t1 = a1j2ry**2
      t7 = a1j2sy**2
      a1j2rxyyy = a1j2ry*t1*a1j2rxrrr+3*t1*a1j2sy*a1j2rxrrs+3*a1j2ry*
     & t7*a1j2rxrss+t7*a1j2sy*a1j2rxsss+3*a1j2ry*a1j2ryy*a1j2rxrr+(3*
     & a1j2syy*a1j2ry+3*a1j2sy*a1j2ryy)*a1j2rxrs+3*a1j2syy*a1j2sy*
     & a1j2rxss+a1j2ryyy*a1j2rxr+a1j2syyy*a1j2rxs
      t1 = a1j2rx**2
      t7 = a1j2sx**2
      a1j2sxxxx = t1*a1j2rx*a1j2sxrrr+3*t1*a1j2sx*a1j2sxrrs+3*a1j2rx*
     & t7*a1j2sxrss+t7*a1j2sx*a1j2sxsss+3*a1j2rx*a1j2rxx*a1j2sxrr+(3*
     & a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx)*a1j2sxrs+3*a1j2sxx*a1j2sx*
     & a1j2sxss+a1j2rxxx*a1j2sxr+a1j2sxxx*a1j2sxs
      t1 = a1j2rx**2
      t10 = a1j2sx**2
      a1j2sxxxy = a1j2ry*t1*a1j2sxrrr+(a1j2sy*t1+2*a1j2ry*a1j2sx*
     & a1j2rx)*a1j2sxrrs+(a1j2ry*t10+2*a1j2sy*a1j2sx*a1j2rx)*
     & a1j2sxrss+a1j2sy*t10*a1j2sxsss+(2*a1j2rxy*a1j2rx+a1j2ry*
     & a1j2rxx)*a1j2sxrr+(a1j2ry*a1j2sxx+2*a1j2sx*a1j2rxy+2*a1j2sxy*
     & a1j2rx+a1j2sy*a1j2rxx)*a1j2sxrs+(a1j2sy*a1j2sxx+2*a1j2sxy*
     & a1j2sx)*a1j2sxss+a1j2rxxy*a1j2sxr+a1j2sxxy*a1j2sxs
      t1 = a1j2ry**2
      t4 = a1j2sy*a1j2ry
      t8 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t16 = a1j2sy**2
      a1j2sxxyy = t1*a1j2rx*a1j2sxrrr+(t4*a1j2rx+a1j2ry*t8)*a1j2sxrrs+(
     & t4*a1j2sx+a1j2sy*t8)*a1j2sxrss+t16*a1j2sx*a1j2sxsss+(a1j2ryy*
     & a1j2rx+2*a1j2ry*a1j2rxy)*a1j2sxrr+(2*a1j2ry*a1j2sxy+2*a1j2sy*
     & a1j2rxy+a1j2ryy*a1j2sx+a1j2syy*a1j2rx)*a1j2sxrs+(a1j2syy*
     & a1j2sx+2*a1j2sy*a1j2sxy)*a1j2sxss+a1j2rxyy*a1j2sxr+a1j2sxyy*
     & a1j2sxs
      t1 = a1j2ry**2
      t7 = a1j2sy**2
      a1j2sxyyy = a1j2ry*t1*a1j2sxrrr+3*t1*a1j2sy*a1j2sxrrs+3*a1j2ry*
     & t7*a1j2sxrss+t7*a1j2sy*a1j2sxsss+3*a1j2ry*a1j2ryy*a1j2sxrr+(3*
     & a1j2syy*a1j2ry+3*a1j2sy*a1j2ryy)*a1j2sxrs+3*a1j2syy*a1j2sy*
     & a1j2sxss+a1j2ryyy*a1j2sxr+a1j2syyy*a1j2sxs
      t1 = a1j2rx**2
      t7 = a1j2sx**2
      a1j2ryxxx = t1*a1j2rx*a1j2ryrrr+3*t1*a1j2sx*a1j2ryrrs+3*a1j2rx*
     & t7*a1j2ryrss+t7*a1j2sx*a1j2rysss+3*a1j2rx*a1j2rxx*a1j2ryrr+(3*
     & a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx)*a1j2ryrs+3*a1j2sxx*a1j2sx*
     & a1j2ryss+a1j2rxxx*a1j2ryr+a1j2sxxx*a1j2rys
      t1 = a1j2rx**2
      t10 = a1j2sx**2
      a1j2ryxxy = a1j2ry*t1*a1j2ryrrr+(a1j2sy*t1+2*a1j2ry*a1j2sx*
     & a1j2rx)*a1j2ryrrs+(a1j2ry*t10+2*a1j2sy*a1j2sx*a1j2rx)*
     & a1j2ryrss+a1j2sy*t10*a1j2rysss+(2*a1j2rxy*a1j2rx+a1j2ry*
     & a1j2rxx)*a1j2ryrr+(a1j2ry*a1j2sxx+2*a1j2sx*a1j2rxy+2*a1j2sxy*
     & a1j2rx+a1j2sy*a1j2rxx)*a1j2ryrs+(a1j2sy*a1j2sxx+2*a1j2sxy*
     & a1j2sx)*a1j2ryss+a1j2rxxy*a1j2ryr+a1j2sxxy*a1j2rys
      t1 = a1j2ry**2
      t4 = a1j2sy*a1j2ry
      t8 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t16 = a1j2sy**2
      a1j2ryxyy = t1*a1j2rx*a1j2ryrrr+(t4*a1j2rx+a1j2ry*t8)*a1j2ryrrs+(
     & t4*a1j2sx+a1j2sy*t8)*a1j2ryrss+t16*a1j2sx*a1j2rysss+(a1j2ryy*
     & a1j2rx+2*a1j2ry*a1j2rxy)*a1j2ryrr+(2*a1j2ry*a1j2sxy+2*a1j2sy*
     & a1j2rxy+a1j2ryy*a1j2sx+a1j2syy*a1j2rx)*a1j2ryrs+(a1j2syy*
     & a1j2sx+2*a1j2sy*a1j2sxy)*a1j2ryss+a1j2rxyy*a1j2ryr+a1j2sxyy*
     & a1j2rys
      t1 = a1j2ry**2
      t7 = a1j2sy**2
      a1j2ryyyy = a1j2ry*t1*a1j2ryrrr+3*t1*a1j2sy*a1j2ryrrs+3*a1j2ry*
     & t7*a1j2ryrss+t7*a1j2sy*a1j2rysss+3*a1j2ry*a1j2ryy*a1j2ryrr+(3*
     & a1j2syy*a1j2ry+3*a1j2sy*a1j2ryy)*a1j2ryrs+3*a1j2syy*a1j2sy*
     & a1j2ryss+a1j2ryyy*a1j2ryr+a1j2syyy*a1j2rys
      t1 = a1j2rx**2
      t7 = a1j2sx**2
      a1j2syxxx = t1*a1j2rx*a1j2syrrr+3*t1*a1j2sx*a1j2syrrs+3*a1j2rx*
     & t7*a1j2syrss+t7*a1j2sx*a1j2sysss+3*a1j2rx*a1j2rxx*a1j2syrr+(3*
     & a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx)*a1j2syrs+3*a1j2sxx*a1j2sx*
     & a1j2syss+a1j2rxxx*a1j2syr+a1j2sxxx*a1j2sys
      t1 = a1j2rx**2
      t10 = a1j2sx**2
      a1j2syxxy = a1j2ry*t1*a1j2syrrr+(a1j2sy*t1+2*a1j2ry*a1j2sx*
     & a1j2rx)*a1j2syrrs+(a1j2ry*t10+2*a1j2sy*a1j2sx*a1j2rx)*
     & a1j2syrss+a1j2sy*t10*a1j2sysss+(2*a1j2rxy*a1j2rx+a1j2ry*
     & a1j2rxx)*a1j2syrr+(a1j2ry*a1j2sxx+2*a1j2sx*a1j2rxy+2*a1j2sxy*
     & a1j2rx+a1j2sy*a1j2rxx)*a1j2syrs+(a1j2sy*a1j2sxx+2*a1j2sxy*
     & a1j2sx)*a1j2syss+a1j2rxxy*a1j2syr+a1j2sxxy*a1j2sys
      t1 = a1j2ry**2
      t4 = a1j2sy*a1j2ry
      t8 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t16 = a1j2sy**2
      a1j2syxyy = t1*a1j2rx*a1j2syrrr+(t4*a1j2rx+a1j2ry*t8)*a1j2syrrs+(
     & t4*a1j2sx+a1j2sy*t8)*a1j2syrss+t16*a1j2sx*a1j2sysss+(a1j2ryy*
     & a1j2rx+2*a1j2ry*a1j2rxy)*a1j2syrr+(2*a1j2ry*a1j2sxy+2*a1j2sy*
     & a1j2rxy+a1j2ryy*a1j2sx+a1j2syy*a1j2rx)*a1j2syrs+(a1j2syy*
     & a1j2sx+2*a1j2sy*a1j2sxy)*a1j2syss+a1j2rxyy*a1j2syr+a1j2sxyy*
     & a1j2sys
      t1 = a1j2ry**2
      t7 = a1j2sy**2
      a1j2syyyy = a1j2ry*t1*a1j2syrrr+3*t1*a1j2sy*a1j2syrrs+3*a1j2ry*
     & t7*a1j2syrss+t7*a1j2sy*a1j2sysss+3*a1j2ry*a1j2ryy*a1j2syrr+(3*
     & a1j2syy*a1j2ry+3*a1j2sy*a1j2ryy)*a1j2syrs+3*a1j2syy*a1j2sy*
     & a1j2syss+a1j2ryyy*a1j2syr+a1j2syyy*a1j2sys
      t1 = a1j2rx**2
      t2 = t1**2
      t8 = a1j2sx**2
      t16 = t8**2
      t25 = a1j2sxx*a1j2rx
      t27 = t25+a1j2sx*a1j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2rxx**2
      t60 = a1j2sxx**2
      a1j2rxxxxx = t2*a1j2rxrrrr+4*t1*a1j2rx*a1j2sx*a1j2rxrrrs+6*t1*t8*
     & a1j2rxrrss+4*a1j2rx*t8*a1j2sx*a1j2rxrsss+t16*a1j2rxssss+6*t1*
     & a1j2rxx*a1j2rxrrr+(7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t1+a1j2rx*
     & t28+a1j2rx*t30)*a1j2rxrrs+(a1j2sx*t28+7*t25*a1j2sx+a1j2rxx*t8+
     & a1j2sx*t30)*a1j2rxrss+6*t8*a1j2sxx*a1j2rxsss+(4*a1j2rx*
     & a1j2rxxx+3*t46)*a1j2rxrr+(4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+
     & 6*a1j2sxx*a1j2rxx)*a1j2rxrs+(4*a1j2sxxx*a1j2sx+3*t60)*a1j2rxss+
     & a1j2rxxxx*a1j2rxr+a1j2sxxxx*a1j2rxs
      t1 = a1j2rx**2
      t2 = t1*a1j2rx
      t11 = a1j2ry*a1j2rx
      t12 = a1j2sx**2
      t19 = a1j2sy*a1j2rx
      t22 = t12*a1j2sx
      t33 = a1j2sx*a1j2rxy
      t37 = a1j2sxy*a1j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx
      a1j2rxxxxy = a1j2ry*t2*a1j2rxrrrr+(3*a1j2ry*t1*a1j2sx+a1j2sy*t2)*
     & a1j2rxrrrs+(3*t11*t12+3*a1j2sy*t1*a1j2sx)*a1j2rxrrss+(3*t19*
     & t12+a1j2ry*t22)*a1j2rxrsss+a1j2sy*t22*a1j2rxssss+(3*a1j2rxy*t1+
     & 3*t11*a1j2rxx)*a1j2rxrrr+(4*t33*a1j2rx+a1j2sxy*t1+a1j2rx*t39+
     & a1j2ry*t44+3*t19*a1j2rxx)*a1j2rxrrs+(a1j2rxy*t12+a1j2sy*t44+3*
     & a1j2ry*a1j2sxx*a1j2sx+4*t37*a1j2sx+a1j2sx*t39)*a1j2rxrss+(3*
     & a1j2sy*a1j2sxx*a1j2sx+3*t12*a1j2sxy)*a1j2rxsss+(3*a1j2rxy*
     & a1j2rxx+3*a1j2rx*a1j2rxxy+a1j2ry*a1j2rxxx)*a1j2rxrr+(3*a1j2sxx*
     & a1j2rxy+a1j2ry*a1j2sxxx+3*a1j2sxy*a1j2rxx+3*a1j2sx*a1j2rxxy+3*
     & a1j2sxxy*a1j2rx+a1j2sy*a1j2rxxx)*a1j2rxrs+(3*a1j2sxxy*a1j2sx+
     & a1j2sy*a1j2sxxx+3*a1j2sxy*a1j2sxx)*a1j2rxss+a1j2rxxxy*a1j2rxr+
     & a1j2sxxxy*a1j2rxs
      t1 = a1j2ry**2
      t2 = a1j2rx**2
      t5 = a1j2sy*a1j2ry
      t11 = a1j2sy*t2+2*a1j2ry*a1j2sx*a1j2rx
      t16 = a1j2sx**2
      t21 = a1j2ry*t16+2*a1j2sy*a1j2sx*a1j2rx
      t29 = a1j2sy**2
      t38 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
      t52 = a1j2sx*a1j2rxy
      t54 = a1j2sxy*a1j2rx
      t57 = a1j2ry*a1j2sxx+2*t52+2*t54+a1j2sy*a1j2rxx
      t60 = 2*t52+2*t54
      t68 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
      t92 = a1j2rxy**2
      t110 = a1j2sxy**2
      a1j2rxxxyy = t1*t2*a1j2rxrrrr+(t5*t2+a1j2ry*t11)*a1j2rxrrrs+(
     & a1j2sy*t11+a1j2ry*t21)*a1j2rxrrss+(a1j2sy*t21+t5*t16)*
     & a1j2rxrsss+t29*t16*a1j2rxssss+(2*a1j2ry*a1j2rxy*a1j2rx+a1j2ry*
     & t38+a1j2ryy*t2)*a1j2rxrrr+(a1j2sy*t38+2*a1j2sy*a1j2rxy*a1j2rx+
     & 2*a1j2ryy*a1j2sx*a1j2rx+a1j2syy*t2+a1j2ry*t57+a1j2ry*t60)*
     & a1j2rxrrs+(a1j2sy*t57+a1j2ry*t68+a1j2ryy*t16+2*a1j2ry*a1j2sxy*
     & a1j2sx+2*a1j2syy*a1j2sx*a1j2rx+a1j2sy*t60)*a1j2rxrss+(2*a1j2sy*
     & a1j2sxy*a1j2sx+a1j2sy*t68+a1j2syy*t16)*a1j2rxsss+(2*a1j2rx*
     & a1j2rxyy+a1j2ryy*a1j2rxx+2*a1j2ry*a1j2rxxy+2*t92)*a1j2rxrr+(4*
     & a1j2sxy*a1j2rxy+2*a1j2ry*a1j2sxxy+a1j2ryy*a1j2sxx+2*a1j2sy*
     & a1j2rxxy+2*a1j2sxyy*a1j2rx+a1j2syy*a1j2rxx+2*a1j2sx*a1j2rxyy)*
     & a1j2rxrs+(2*t110+2*a1j2sy*a1j2sxxy+a1j2syy*a1j2sxx+2*a1j2sx*
     & a1j2sxyy)*a1j2rxss+a1j2rxxyy*a1j2rxr+a1j2sxxyy*a1j2rxs
      t1 = a1j2ry**2
      t7 = a1j2sy*a1j2ry
      t11 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t13 = t7*a1j2rx+a1j2ry*t11
      t20 = t7*a1j2sx+a1j2sy*t11
      t25 = a1j2sy**2
      t33 = a1j2ryy*a1j2rx
      t34 = a1j2ry*a1j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a1j2ry*a1j2sxy
      t51 = a1j2sy*a1j2rxy
      t53 = a1j2ryy*a1j2sx
      t54 = a1j2syy*a1j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a1j2syy*a1j2sx
      t63 = a1j2sy*a1j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a1j2rxxyyy = t1*a1j2ry*a1j2rx*a1j2rxrrrr+(a1j2sy*t1*a1j2rx+
     & a1j2ry*t13)*a1j2rxrrrs+(a1j2sy*t13+a1j2ry*t20)*a1j2rxrrss+(
     & a1j2sy*t20+a1j2ry*t25*a1j2sx)*a1j2rxrsss+t25*a1j2sy*a1j2sx*
     & a1j2rxssss+(a1j2ry*t35+a1j2ry*t38+a1j2ryy*a1j2ry*a1j2rx)*
     & a1j2rxrrr+(a1j2sy*t38+a1j2sy*t35+a1j2ryy*t11+a1j2syy*a1j2ry*
     & a1j2rx+a1j2ry*t55+a1j2ry*t57)*a1j2rxrrs+(a1j2sy*t55+a1j2ry*t65+
     & a1j2ryy*a1j2sy*a1j2sx+a1j2ry*t69+a1j2syy*t11+a1j2sy*t57)*
     & a1j2rxrss+(a1j2sy*t69+a1j2sy*t65+a1j2syy*a1j2sy*a1j2sx)*
     & a1j2rxsss+(3*a1j2ry*a1j2rxyy+a1j2ryyy*a1j2rx+3*a1j2ryy*a1j2rxy)
     & *a1j2rxrr+(3*a1j2ry*a1j2sxyy+3*a1j2sy*a1j2rxyy+a1j2syyy*a1j2rx+
     & 3*a1j2syy*a1j2rxy+a1j2ryyy*a1j2sx+3*a1j2ryy*a1j2sxy)*a1j2rxrs+(
     & a1j2syyy*a1j2sx+3*a1j2sy*a1j2sxyy+3*a1j2syy*a1j2sxy)*a1j2rxss+
     & a1j2rxyyy*a1j2rxr+a1j2sxyyy*a1j2rxs
      t1 = a1j2ry**2
      t2 = t1**2
      t8 = a1j2sy**2
      t16 = t8**2
      t25 = a1j2syy*a1j2ry
      t27 = t25+a1j2sy*a1j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2ryy**2
      t60 = a1j2syy**2
      a1j2rxyyyy = t2*a1j2rxrrrr+4*t1*a1j2ry*a1j2sy*a1j2rxrrrs+6*t1*t8*
     & a1j2rxrrss+4*a1j2ry*t8*a1j2sy*a1j2rxrsss+t16*a1j2rxssss+6*t1*
     & a1j2ryy*a1j2rxrrr+(7*a1j2sy*a1j2ry*a1j2ryy+a1j2syy*t1+a1j2ry*
     & t28+a1j2ry*t30)*a1j2rxrrs+(a1j2sy*t28+7*t25*a1j2sy+a1j2ryy*t8+
     & a1j2sy*t30)*a1j2rxrss+6*t8*a1j2syy*a1j2rxsss+(4*a1j2ry*
     & a1j2ryyy+3*t46)*a1j2rxrr+(4*a1j2syyy*a1j2ry+4*a1j2sy*a1j2ryyy+
     & 6*a1j2syy*a1j2ryy)*a1j2rxrs+(4*a1j2syyy*a1j2sy+3*t60)*a1j2rxss+
     & a1j2ryyyy*a1j2rxr+a1j2syyyy*a1j2rxs
      t1 = a1j2rx**2
      t2 = t1**2
      t8 = a1j2sx**2
      t16 = t8**2
      t25 = a1j2sxx*a1j2rx
      t27 = t25+a1j2sx*a1j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2rxx**2
      t60 = a1j2sxx**2
      a1j2sxxxxx = t2*a1j2sxrrrr+4*t1*a1j2rx*a1j2sx*a1j2sxrrrs+6*t1*t8*
     & a1j2sxrrss+4*a1j2rx*t8*a1j2sx*a1j2sxrsss+t16*a1j2sxssss+6*t1*
     & a1j2rxx*a1j2sxrrr+(7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t1+a1j2rx*
     & t28+a1j2rx*t30)*a1j2sxrrs+(a1j2sx*t28+7*t25*a1j2sx+a1j2rxx*t8+
     & a1j2sx*t30)*a1j2sxrss+6*t8*a1j2sxx*a1j2sxsss+(4*a1j2rx*
     & a1j2rxxx+3*t46)*a1j2sxrr+(4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+
     & 6*a1j2sxx*a1j2rxx)*a1j2sxrs+(4*a1j2sxxx*a1j2sx+3*t60)*a1j2sxss+
     & a1j2rxxxx*a1j2sxr+a1j2sxxxx*a1j2sxs
      t1 = a1j2rx**2
      t2 = t1*a1j2rx
      t11 = a1j2ry*a1j2rx
      t12 = a1j2sx**2
      t19 = a1j2sy*a1j2rx
      t22 = t12*a1j2sx
      t33 = a1j2sx*a1j2rxy
      t37 = a1j2sxy*a1j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx
      a1j2sxxxxy = a1j2ry*t2*a1j2sxrrrr+(3*a1j2ry*t1*a1j2sx+a1j2sy*t2)*
     & a1j2sxrrrs+(3*t11*t12+3*a1j2sy*t1*a1j2sx)*a1j2sxrrss+(3*t19*
     & t12+a1j2ry*t22)*a1j2sxrsss+a1j2sy*t22*a1j2sxssss+(3*a1j2rxy*t1+
     & 3*t11*a1j2rxx)*a1j2sxrrr+(4*t33*a1j2rx+a1j2sxy*t1+a1j2rx*t39+
     & a1j2ry*t44+3*t19*a1j2rxx)*a1j2sxrrs+(a1j2rxy*t12+a1j2sy*t44+3*
     & a1j2ry*a1j2sxx*a1j2sx+4*t37*a1j2sx+a1j2sx*t39)*a1j2sxrss+(3*
     & a1j2sy*a1j2sxx*a1j2sx+3*t12*a1j2sxy)*a1j2sxsss+(3*a1j2rxy*
     & a1j2rxx+3*a1j2rx*a1j2rxxy+a1j2ry*a1j2rxxx)*a1j2sxrr+(3*a1j2sxx*
     & a1j2rxy+a1j2ry*a1j2sxxx+3*a1j2sxy*a1j2rxx+3*a1j2sx*a1j2rxxy+3*
     & a1j2sxxy*a1j2rx+a1j2sy*a1j2rxxx)*a1j2sxrs+(3*a1j2sxxy*a1j2sx+
     & a1j2sy*a1j2sxxx+3*a1j2sxy*a1j2sxx)*a1j2sxss+a1j2rxxxy*a1j2sxr+
     & a1j2sxxxy*a1j2sxs
      t1 = a1j2ry**2
      t2 = a1j2rx**2
      t5 = a1j2sy*a1j2ry
      t11 = a1j2sy*t2+2*a1j2ry*a1j2sx*a1j2rx
      t16 = a1j2sx**2
      t21 = a1j2ry*t16+2*a1j2sy*a1j2sx*a1j2rx
      t29 = a1j2sy**2
      t38 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
      t52 = a1j2sx*a1j2rxy
      t54 = a1j2sxy*a1j2rx
      t57 = a1j2ry*a1j2sxx+2*t52+2*t54+a1j2sy*a1j2rxx
      t60 = 2*t52+2*t54
      t68 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
      t92 = a1j2rxy**2
      t110 = a1j2sxy**2
      a1j2sxxxyy = t1*t2*a1j2sxrrrr+(t5*t2+a1j2ry*t11)*a1j2sxrrrs+(
     & a1j2sy*t11+a1j2ry*t21)*a1j2sxrrss+(a1j2sy*t21+t5*t16)*
     & a1j2sxrsss+t29*t16*a1j2sxssss+(2*a1j2ry*a1j2rxy*a1j2rx+a1j2ry*
     & t38+a1j2ryy*t2)*a1j2sxrrr+(a1j2sy*t38+2*a1j2sy*a1j2rxy*a1j2rx+
     & 2*a1j2ryy*a1j2sx*a1j2rx+a1j2syy*t2+a1j2ry*t57+a1j2ry*t60)*
     & a1j2sxrrs+(a1j2sy*t57+a1j2ry*t68+a1j2ryy*t16+2*a1j2ry*a1j2sxy*
     & a1j2sx+2*a1j2syy*a1j2sx*a1j2rx+a1j2sy*t60)*a1j2sxrss+(2*a1j2sy*
     & a1j2sxy*a1j2sx+a1j2sy*t68+a1j2syy*t16)*a1j2sxsss+(2*a1j2rx*
     & a1j2rxyy+a1j2ryy*a1j2rxx+2*a1j2ry*a1j2rxxy+2*t92)*a1j2sxrr+(4*
     & a1j2sxy*a1j2rxy+2*a1j2ry*a1j2sxxy+a1j2ryy*a1j2sxx+2*a1j2sy*
     & a1j2rxxy+2*a1j2sxyy*a1j2rx+a1j2syy*a1j2rxx+2*a1j2sx*a1j2rxyy)*
     & a1j2sxrs+(2*t110+2*a1j2sy*a1j2sxxy+a1j2syy*a1j2sxx+2*a1j2sx*
     & a1j2sxyy)*a1j2sxss+a1j2rxxyy*a1j2sxr+a1j2sxxyy*a1j2sxs
      t1 = a1j2ry**2
      t7 = a1j2sy*a1j2ry
      t11 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t13 = t7*a1j2rx+a1j2ry*t11
      t20 = t7*a1j2sx+a1j2sy*t11
      t25 = a1j2sy**2
      t33 = a1j2ryy*a1j2rx
      t34 = a1j2ry*a1j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a1j2ry*a1j2sxy
      t51 = a1j2sy*a1j2rxy
      t53 = a1j2ryy*a1j2sx
      t54 = a1j2syy*a1j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a1j2syy*a1j2sx
      t63 = a1j2sy*a1j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a1j2sxxyyy = t1*a1j2ry*a1j2rx*a1j2sxrrrr+(a1j2sy*t1*a1j2rx+
     & a1j2ry*t13)*a1j2sxrrrs+(a1j2sy*t13+a1j2ry*t20)*a1j2sxrrss+(
     & a1j2sy*t20+a1j2ry*t25*a1j2sx)*a1j2sxrsss+t25*a1j2sy*a1j2sx*
     & a1j2sxssss+(a1j2ry*t35+a1j2ry*t38+a1j2ryy*a1j2ry*a1j2rx)*
     & a1j2sxrrr+(a1j2sy*t38+a1j2sy*t35+a1j2ryy*t11+a1j2syy*a1j2ry*
     & a1j2rx+a1j2ry*t55+a1j2ry*t57)*a1j2sxrrs+(a1j2sy*t55+a1j2ry*t65+
     & a1j2ryy*a1j2sy*a1j2sx+a1j2ry*t69+a1j2syy*t11+a1j2sy*t57)*
     & a1j2sxrss+(a1j2sy*t69+a1j2sy*t65+a1j2syy*a1j2sy*a1j2sx)*
     & a1j2sxsss+(3*a1j2ry*a1j2rxyy+a1j2ryyy*a1j2rx+3*a1j2ryy*a1j2rxy)
     & *a1j2sxrr+(3*a1j2ry*a1j2sxyy+3*a1j2sy*a1j2rxyy+a1j2syyy*a1j2rx+
     & 3*a1j2syy*a1j2rxy+a1j2ryyy*a1j2sx+3*a1j2ryy*a1j2sxy)*a1j2sxrs+(
     & a1j2syyy*a1j2sx+3*a1j2sy*a1j2sxyy+3*a1j2syy*a1j2sxy)*a1j2sxss+
     & a1j2rxyyy*a1j2sxr+a1j2sxyyy*a1j2sxs
      t1 = a1j2ry**2
      t2 = t1**2
      t8 = a1j2sy**2
      t16 = t8**2
      t25 = a1j2syy*a1j2ry
      t27 = t25+a1j2sy*a1j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2ryy**2
      t60 = a1j2syy**2
      a1j2sxyyyy = t2*a1j2sxrrrr+4*t1*a1j2ry*a1j2sy*a1j2sxrrrs+6*t1*t8*
     & a1j2sxrrss+4*a1j2ry*t8*a1j2sy*a1j2sxrsss+t16*a1j2sxssss+6*t1*
     & a1j2ryy*a1j2sxrrr+(7*a1j2sy*a1j2ry*a1j2ryy+a1j2syy*t1+a1j2ry*
     & t28+a1j2ry*t30)*a1j2sxrrs+(a1j2sy*t28+7*t25*a1j2sy+a1j2ryy*t8+
     & a1j2sy*t30)*a1j2sxrss+6*t8*a1j2syy*a1j2sxsss+(4*a1j2ry*
     & a1j2ryyy+3*t46)*a1j2sxrr+(4*a1j2syyy*a1j2ry+4*a1j2sy*a1j2ryyy+
     & 6*a1j2syy*a1j2ryy)*a1j2sxrs+(4*a1j2syyy*a1j2sy+3*t60)*a1j2sxss+
     & a1j2ryyyy*a1j2sxr+a1j2syyyy*a1j2sxs
      t1 = a1j2rx**2
      t2 = t1**2
      t8 = a1j2sx**2
      t16 = t8**2
      t25 = a1j2sxx*a1j2rx
      t27 = t25+a1j2sx*a1j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2rxx**2
      t60 = a1j2sxx**2
      a1j2ryxxxx = t2*a1j2ryrrrr+4*t1*a1j2rx*a1j2sx*a1j2ryrrrs+6*t1*t8*
     & a1j2ryrrss+4*a1j2rx*t8*a1j2sx*a1j2ryrsss+t16*a1j2ryssss+6*t1*
     & a1j2rxx*a1j2ryrrr+(7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t1+a1j2rx*
     & t28+a1j2rx*t30)*a1j2ryrrs+(a1j2sx*t28+7*t25*a1j2sx+a1j2rxx*t8+
     & a1j2sx*t30)*a1j2ryrss+6*t8*a1j2sxx*a1j2rysss+(4*a1j2rx*
     & a1j2rxxx+3*t46)*a1j2ryrr+(4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+
     & 6*a1j2sxx*a1j2rxx)*a1j2ryrs+(4*a1j2sxxx*a1j2sx+3*t60)*a1j2ryss+
     & a1j2rxxxx*a1j2ryr+a1j2sxxxx*a1j2rys
      t1 = a1j2rx**2
      t2 = t1*a1j2rx
      t11 = a1j2ry*a1j2rx
      t12 = a1j2sx**2
      t19 = a1j2sy*a1j2rx
      t22 = t12*a1j2sx
      t33 = a1j2sx*a1j2rxy
      t37 = a1j2sxy*a1j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx
      a1j2ryxxxy = a1j2ry*t2*a1j2ryrrrr+(3*a1j2ry*t1*a1j2sx+a1j2sy*t2)*
     & a1j2ryrrrs+(3*t11*t12+3*a1j2sy*t1*a1j2sx)*a1j2ryrrss+(3*t19*
     & t12+a1j2ry*t22)*a1j2ryrsss+a1j2sy*t22*a1j2ryssss+(3*a1j2rxy*t1+
     & 3*t11*a1j2rxx)*a1j2ryrrr+(4*t33*a1j2rx+a1j2sxy*t1+a1j2rx*t39+
     & a1j2ry*t44+3*t19*a1j2rxx)*a1j2ryrrs+(a1j2rxy*t12+a1j2sy*t44+3*
     & a1j2ry*a1j2sxx*a1j2sx+4*t37*a1j2sx+a1j2sx*t39)*a1j2ryrss+(3*
     & a1j2sy*a1j2sxx*a1j2sx+3*t12*a1j2sxy)*a1j2rysss+(3*a1j2rxy*
     & a1j2rxx+3*a1j2rx*a1j2rxxy+a1j2ry*a1j2rxxx)*a1j2ryrr+(3*a1j2sxx*
     & a1j2rxy+a1j2ry*a1j2sxxx+3*a1j2sxy*a1j2rxx+3*a1j2sx*a1j2rxxy+3*
     & a1j2sxxy*a1j2rx+a1j2sy*a1j2rxxx)*a1j2ryrs+(3*a1j2sxxy*a1j2sx+
     & a1j2sy*a1j2sxxx+3*a1j2sxy*a1j2sxx)*a1j2ryss+a1j2rxxxy*a1j2ryr+
     & a1j2sxxxy*a1j2rys
      t1 = a1j2ry**2
      t2 = a1j2rx**2
      t5 = a1j2sy*a1j2ry
      t11 = a1j2sy*t2+2*a1j2ry*a1j2sx*a1j2rx
      t16 = a1j2sx**2
      t21 = a1j2ry*t16+2*a1j2sy*a1j2sx*a1j2rx
      t29 = a1j2sy**2
      t38 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
      t52 = a1j2sx*a1j2rxy
      t54 = a1j2sxy*a1j2rx
      t57 = a1j2ry*a1j2sxx+2*t52+2*t54+a1j2sy*a1j2rxx
      t60 = 2*t52+2*t54
      t68 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
      t92 = a1j2rxy**2
      t110 = a1j2sxy**2
      a1j2ryxxyy = t1*t2*a1j2ryrrrr+(t5*t2+a1j2ry*t11)*a1j2ryrrrs+(
     & a1j2sy*t11+a1j2ry*t21)*a1j2ryrrss+(a1j2sy*t21+t5*t16)*
     & a1j2ryrsss+t29*t16*a1j2ryssss+(2*a1j2ry*a1j2rxy*a1j2rx+a1j2ry*
     & t38+a1j2ryy*t2)*a1j2ryrrr+(a1j2sy*t38+2*a1j2sy*a1j2rxy*a1j2rx+
     & 2*a1j2ryy*a1j2sx*a1j2rx+a1j2syy*t2+a1j2ry*t57+a1j2ry*t60)*
     & a1j2ryrrs+(a1j2sy*t57+a1j2ry*t68+a1j2ryy*t16+2*a1j2ry*a1j2sxy*
     & a1j2sx+2*a1j2syy*a1j2sx*a1j2rx+a1j2sy*t60)*a1j2ryrss+(2*a1j2sy*
     & a1j2sxy*a1j2sx+a1j2sy*t68+a1j2syy*t16)*a1j2rysss+(2*a1j2rx*
     & a1j2rxyy+a1j2ryy*a1j2rxx+2*a1j2ry*a1j2rxxy+2*t92)*a1j2ryrr+(4*
     & a1j2sxy*a1j2rxy+2*a1j2ry*a1j2sxxy+a1j2ryy*a1j2sxx+2*a1j2sy*
     & a1j2rxxy+2*a1j2sxyy*a1j2rx+a1j2syy*a1j2rxx+2*a1j2sx*a1j2rxyy)*
     & a1j2ryrs+(2*t110+2*a1j2sy*a1j2sxxy+a1j2syy*a1j2sxx+2*a1j2sx*
     & a1j2sxyy)*a1j2ryss+a1j2rxxyy*a1j2ryr+a1j2sxxyy*a1j2rys
      t1 = a1j2ry**2
      t7 = a1j2sy*a1j2ry
      t11 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t13 = t7*a1j2rx+a1j2ry*t11
      t20 = t7*a1j2sx+a1j2sy*t11
      t25 = a1j2sy**2
      t33 = a1j2ryy*a1j2rx
      t34 = a1j2ry*a1j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a1j2ry*a1j2sxy
      t51 = a1j2sy*a1j2rxy
      t53 = a1j2ryy*a1j2sx
      t54 = a1j2syy*a1j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a1j2syy*a1j2sx
      t63 = a1j2sy*a1j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a1j2ryxyyy = t1*a1j2ry*a1j2rx*a1j2ryrrrr+(a1j2sy*t1*a1j2rx+
     & a1j2ry*t13)*a1j2ryrrrs+(a1j2sy*t13+a1j2ry*t20)*a1j2ryrrss+(
     & a1j2sy*t20+a1j2ry*t25*a1j2sx)*a1j2ryrsss+t25*a1j2sy*a1j2sx*
     & a1j2ryssss+(a1j2ry*t35+a1j2ry*t38+a1j2ryy*a1j2ry*a1j2rx)*
     & a1j2ryrrr+(a1j2sy*t38+a1j2sy*t35+a1j2ryy*t11+a1j2syy*a1j2ry*
     & a1j2rx+a1j2ry*t55+a1j2ry*t57)*a1j2ryrrs+(a1j2sy*t55+a1j2ry*t65+
     & a1j2ryy*a1j2sy*a1j2sx+a1j2ry*t69+a1j2syy*t11+a1j2sy*t57)*
     & a1j2ryrss+(a1j2sy*t69+a1j2sy*t65+a1j2syy*a1j2sy*a1j2sx)*
     & a1j2rysss+(3*a1j2ry*a1j2rxyy+a1j2ryyy*a1j2rx+3*a1j2ryy*a1j2rxy)
     & *a1j2ryrr+(3*a1j2ry*a1j2sxyy+3*a1j2sy*a1j2rxyy+a1j2syyy*a1j2rx+
     & 3*a1j2syy*a1j2rxy+a1j2ryyy*a1j2sx+3*a1j2ryy*a1j2sxy)*a1j2ryrs+(
     & a1j2syyy*a1j2sx+3*a1j2sy*a1j2sxyy+3*a1j2syy*a1j2sxy)*a1j2ryss+
     & a1j2rxyyy*a1j2ryr+a1j2sxyyy*a1j2rys
      t1 = a1j2ry**2
      t2 = t1**2
      t8 = a1j2sy**2
      t16 = t8**2
      t25 = a1j2syy*a1j2ry
      t27 = t25+a1j2sy*a1j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2ryy**2
      t60 = a1j2syy**2
      a1j2ryyyyy = t2*a1j2ryrrrr+4*t1*a1j2ry*a1j2sy*a1j2ryrrrs+6*t1*t8*
     & a1j2ryrrss+4*a1j2ry*t8*a1j2sy*a1j2ryrsss+t16*a1j2ryssss+6*t1*
     & a1j2ryy*a1j2ryrrr+(7*a1j2sy*a1j2ry*a1j2ryy+a1j2syy*t1+a1j2ry*
     & t28+a1j2ry*t30)*a1j2ryrrs+(a1j2sy*t28+7*t25*a1j2sy+a1j2ryy*t8+
     & a1j2sy*t30)*a1j2ryrss+6*t8*a1j2syy*a1j2rysss+(4*a1j2ry*
     & a1j2ryyy+3*t46)*a1j2ryrr+(4*a1j2syyy*a1j2ry+4*a1j2sy*a1j2ryyy+
     & 6*a1j2syy*a1j2ryy)*a1j2ryrs+(4*a1j2syyy*a1j2sy+3*t60)*a1j2ryss+
     & a1j2ryyyy*a1j2ryr+a1j2syyyy*a1j2rys
      t1 = a1j2rx**2
      t2 = t1**2
      t8 = a1j2sx**2
      t16 = t8**2
      t25 = a1j2sxx*a1j2rx
      t27 = t25+a1j2sx*a1j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2rxx**2
      t60 = a1j2sxx**2
      a1j2syxxxx = t2*a1j2syrrrr+4*t1*a1j2rx*a1j2sx*a1j2syrrrs+6*t1*t8*
     & a1j2syrrss+4*a1j2rx*t8*a1j2sx*a1j2syrsss+t16*a1j2syssss+6*t1*
     & a1j2rxx*a1j2syrrr+(7*a1j2sx*a1j2rx*a1j2rxx+a1j2sxx*t1+a1j2rx*
     & t28+a1j2rx*t30)*a1j2syrrs+(a1j2sx*t28+7*t25*a1j2sx+a1j2rxx*t8+
     & a1j2sx*t30)*a1j2syrss+6*t8*a1j2sxx*a1j2sysss+(4*a1j2rx*
     & a1j2rxxx+3*t46)*a1j2syrr+(4*a1j2sxxx*a1j2rx+4*a1j2sx*a1j2rxxx+
     & 6*a1j2sxx*a1j2rxx)*a1j2syrs+(4*a1j2sxxx*a1j2sx+3*t60)*a1j2syss+
     & a1j2rxxxx*a1j2syr+a1j2sxxxx*a1j2sys
      t1 = a1j2rx**2
      t2 = t1*a1j2rx
      t11 = a1j2ry*a1j2rx
      t12 = a1j2sx**2
      t19 = a1j2sy*a1j2rx
      t22 = t12*a1j2sx
      t33 = a1j2sx*a1j2rxy
      t37 = a1j2sxy*a1j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a1j2sxx*a1j2rx+3*a1j2sx*a1j2rxx
      a1j2syxxxy = a1j2ry*t2*a1j2syrrrr+(3*a1j2ry*t1*a1j2sx+a1j2sy*t2)*
     & a1j2syrrrs+(3*t11*t12+3*a1j2sy*t1*a1j2sx)*a1j2syrrss+(3*t19*
     & t12+a1j2ry*t22)*a1j2syrsss+a1j2sy*t22*a1j2syssss+(3*a1j2rxy*t1+
     & 3*t11*a1j2rxx)*a1j2syrrr+(4*t33*a1j2rx+a1j2sxy*t1+a1j2rx*t39+
     & a1j2ry*t44+3*t19*a1j2rxx)*a1j2syrrs+(a1j2rxy*t12+a1j2sy*t44+3*
     & a1j2ry*a1j2sxx*a1j2sx+4*t37*a1j2sx+a1j2sx*t39)*a1j2syrss+(3*
     & a1j2sy*a1j2sxx*a1j2sx+3*t12*a1j2sxy)*a1j2sysss+(3*a1j2rxy*
     & a1j2rxx+3*a1j2rx*a1j2rxxy+a1j2ry*a1j2rxxx)*a1j2syrr+(3*a1j2sxx*
     & a1j2rxy+a1j2ry*a1j2sxxx+3*a1j2sxy*a1j2rxx+3*a1j2sx*a1j2rxxy+3*
     & a1j2sxxy*a1j2rx+a1j2sy*a1j2rxxx)*a1j2syrs+(3*a1j2sxxy*a1j2sx+
     & a1j2sy*a1j2sxxx+3*a1j2sxy*a1j2sxx)*a1j2syss+a1j2rxxxy*a1j2syr+
     & a1j2sxxxy*a1j2sys
      t1 = a1j2ry**2
      t2 = a1j2rx**2
      t5 = a1j2sy*a1j2ry
      t11 = a1j2sy*t2+2*a1j2ry*a1j2sx*a1j2rx
      t16 = a1j2sx**2
      t21 = a1j2ry*t16+2*a1j2sy*a1j2sx*a1j2rx
      t29 = a1j2sy**2
      t38 = 2*a1j2rxy*a1j2rx+a1j2ry*a1j2rxx
      t52 = a1j2sx*a1j2rxy
      t54 = a1j2sxy*a1j2rx
      t57 = a1j2ry*a1j2sxx+2*t52+2*t54+a1j2sy*a1j2rxx
      t60 = 2*t52+2*t54
      t68 = a1j2sy*a1j2sxx+2*a1j2sxy*a1j2sx
      t92 = a1j2rxy**2
      t110 = a1j2sxy**2
      a1j2syxxyy = t1*t2*a1j2syrrrr+(t5*t2+a1j2ry*t11)*a1j2syrrrs+(
     & a1j2sy*t11+a1j2ry*t21)*a1j2syrrss+(a1j2sy*t21+t5*t16)*
     & a1j2syrsss+t29*t16*a1j2syssss+(2*a1j2ry*a1j2rxy*a1j2rx+a1j2ry*
     & t38+a1j2ryy*t2)*a1j2syrrr+(a1j2sy*t38+2*a1j2sy*a1j2rxy*a1j2rx+
     & 2*a1j2ryy*a1j2sx*a1j2rx+a1j2syy*t2+a1j2ry*t57+a1j2ry*t60)*
     & a1j2syrrs+(a1j2sy*t57+a1j2ry*t68+a1j2ryy*t16+2*a1j2ry*a1j2sxy*
     & a1j2sx+2*a1j2syy*a1j2sx*a1j2rx+a1j2sy*t60)*a1j2syrss+(2*a1j2sy*
     & a1j2sxy*a1j2sx+a1j2sy*t68+a1j2syy*t16)*a1j2sysss+(2*a1j2rx*
     & a1j2rxyy+a1j2ryy*a1j2rxx+2*a1j2ry*a1j2rxxy+2*t92)*a1j2syrr+(4*
     & a1j2sxy*a1j2rxy+2*a1j2ry*a1j2sxxy+a1j2ryy*a1j2sxx+2*a1j2sy*
     & a1j2rxxy+2*a1j2sxyy*a1j2rx+a1j2syy*a1j2rxx+2*a1j2sx*a1j2rxyy)*
     & a1j2syrs+(2*t110+2*a1j2sy*a1j2sxxy+a1j2syy*a1j2sxx+2*a1j2sx*
     & a1j2sxyy)*a1j2syss+a1j2rxxyy*a1j2syr+a1j2sxxyy*a1j2sys
      t1 = a1j2ry**2
      t7 = a1j2sy*a1j2ry
      t11 = a1j2sy*a1j2rx+a1j2ry*a1j2sx
      t13 = t7*a1j2rx+a1j2ry*t11
      t20 = t7*a1j2sx+a1j2sy*t11
      t25 = a1j2sy**2
      t33 = a1j2ryy*a1j2rx
      t34 = a1j2ry*a1j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a1j2ry*a1j2sxy
      t51 = a1j2sy*a1j2rxy
      t53 = a1j2ryy*a1j2sx
      t54 = a1j2syy*a1j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a1j2syy*a1j2sx
      t63 = a1j2sy*a1j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a1j2syxyyy = t1*a1j2ry*a1j2rx*a1j2syrrrr+(a1j2sy*t1*a1j2rx+
     & a1j2ry*t13)*a1j2syrrrs+(a1j2sy*t13+a1j2ry*t20)*a1j2syrrss+(
     & a1j2sy*t20+a1j2ry*t25*a1j2sx)*a1j2syrsss+t25*a1j2sy*a1j2sx*
     & a1j2syssss+(a1j2ry*t35+a1j2ry*t38+a1j2ryy*a1j2ry*a1j2rx)*
     & a1j2syrrr+(a1j2sy*t38+a1j2sy*t35+a1j2ryy*t11+a1j2syy*a1j2ry*
     & a1j2rx+a1j2ry*t55+a1j2ry*t57)*a1j2syrrs+(a1j2sy*t55+a1j2ry*t65+
     & a1j2ryy*a1j2sy*a1j2sx+a1j2ry*t69+a1j2syy*t11+a1j2sy*t57)*
     & a1j2syrss+(a1j2sy*t69+a1j2sy*t65+a1j2syy*a1j2sy*a1j2sx)*
     & a1j2sysss+(3*a1j2ry*a1j2rxyy+a1j2ryyy*a1j2rx+3*a1j2ryy*a1j2rxy)
     & *a1j2syrr+(3*a1j2ry*a1j2sxyy+3*a1j2sy*a1j2rxyy+a1j2syyy*a1j2rx+
     & 3*a1j2syy*a1j2rxy+a1j2ryyy*a1j2sx+3*a1j2ryy*a1j2sxy)*a1j2syrs+(
     & a1j2syyy*a1j2sx+3*a1j2sy*a1j2sxyy+3*a1j2syy*a1j2sxy)*a1j2syss+
     & a1j2rxyyy*a1j2syr+a1j2sxyyy*a1j2sys
      t1 = a1j2ry**2
      t2 = t1**2
      t8 = a1j2sy**2
      t16 = t8**2
      t25 = a1j2syy*a1j2ry
      t27 = t25+a1j2sy*a1j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a1j2ryy**2
      t60 = a1j2syy**2
      a1j2syyyyy = t2*a1j2syrrrr+4*t1*a1j2ry*a1j2sy*a1j2syrrrs+6*t1*t8*
     & a1j2syrrss+4*a1j2ry*t8*a1j2sy*a1j2syrsss+t16*a1j2syssss+6*t1*
     & a1j2ryy*a1j2syrrr+(7*a1j2sy*a1j2ry*a1j2ryy+a1j2syy*t1+a1j2ry*
     & t28+a1j2ry*t30)*a1j2syrrs+(a1j2sy*t28+7*t25*a1j2sy+a1j2ryy*t8+
     & a1j2sy*t30)*a1j2syrss+6*t8*a1j2syy*a1j2sysss+(4*a1j2ry*
     & a1j2ryyy+3*t46)*a1j2syrr+(4*a1j2syyy*a1j2ry+4*a1j2sy*a1j2ryyy+
     & 6*a1j2syy*a1j2ryy)*a1j2syrs+(4*a1j2syyy*a1j2sy+3*t60)*a1j2syss+
     & a1j2ryyyy*a1j2syr+a1j2syyyy*a1j2sys
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
      a1j2rxxxxxx = t2*a1j2rx*a1j2rxrrrrr+5*a1j2sx*t2*a1j2rxrrrrs+10*
     & t8*t9*a1j2rxrrrss+10*t13*t1*a1j2rxrrsss+5*t17*a1j2rx*
     & a1j2rxrssss+t17*a1j2sx*a1j2rxsssss+10*a1j2rxx*t9*a1j2rxrrrr+(
     & 12*a1j2rxx*t1*a1j2sx+a1j2rx*t38+a1j2sxx*t9+a1j2rx*t44)*
     & a1j2rxrrrs+(3*a1j2rxx*a1j2rx*t8+a1j2rx*t55+a1j2rx*t59+a1j2sx*
     & t44+3*t29*a1j2sx+a1j2sx*t38)*a1j2rxrrss+(a1j2rxx*t13+12*t31*t8+
     & a1j2sx*t55+a1j2sx*t59)*a1j2rxrsss+10*a1j2sxx*t13*a1j2rxssss+(
     & a1j2rx*t81+7*t79*a1j2rx+a1j2rx*t86+a1j2rx*t88+a1j2rxxx*t1)*
     & a1j2rxrrr+t121*a1j2rxrrs+t145*a1j2rxrss+(a1j2sx*t129+7*t127*
     & a1j2sx+a1j2sx*t136+a1j2sx*t133+a1j2sxxx*t8)*a1j2rxsss+(10*
     & a1j2rxx*a1j2rxxx+5*a1j2rx*a1j2rxxxx)*a1j2rxrr+(5*a1j2sxxxx*
     & a1j2rx+10*a1j2sxx*a1j2rxxx+5*a1j2sx*a1j2rxxxx+10*a1j2sxxx*
     & a1j2rxx)*a1j2rxrs+(5*a1j2sxxxx*a1j2sx+10*a1j2sxx*a1j2sxxx)*
     & a1j2rxss
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
      a1j2rxxxxxy = a1j2ry*t2*a1j2rxrrrrr+(4*a1j2ry*t5*a1j2sx+a1j2sy*
     & t2)*a1j2rxrrrrs+(4*a1j2sy*t5*a1j2sx+6*t15*t16)*a1j2rxrrrss+(4*
     & a1j2ry*a1j2rx*t22+6*t25*t16)*a1j2rxrrsss+(a1j2ry*t30+4*a1j2sy*
     & a1j2rx*t22)*a1j2rxrssss+a1j2sy*t30*a1j2rxsssss+(4*t5*a1j2rxy+6*
     & t15*a1j2rxx)*a1j2rxrrrr+(6*t25*a1j2rxx+a1j2rx*t55+6*t47*t1+
     & a1j2sxy*t5+a1j2ry*t71)*a1j2rxrrrs+(3*t50*a1j2sx+a1j2rx*t81+
     & a1j2sy*t71+a1j2ry*t89+3*a1j2rxy*a1j2rx*t16+a1j2sx*t55)*
     & a1j2rxrrss+(a1j2sy*t89+6*t51*t16+a1j2sx*t81+a1j2rxy*t22+6*
     & a1j2ry*t16*a1j2sxx)*a1j2rxrsss+(4*t22*a1j2sxy+6*a1j2sy*t16*
     & a1j2sxx)*a1j2rxssss+(a1j2rxxy*t1+7*t115*a1j2rx+a1j2rx*t120+
     & a1j2rx*t122+a1j2ry*t128)*a1j2rxrrr+t162*a1j2rxrrs+t190*
     & a1j2rxrss+(a1j2sx*t174+a1j2sxxy*t16+7*a1j2sxy*a1j2sx*a1j2sxx+
     & a1j2sx*t179+a1j2sy*t185)*a1j2rxsss+(a1j2ry*a1j2rxxxx+4*
     & a1j2rxxx*a1j2rxy+6*a1j2rxxy*a1j2rxx+4*a1j2rxxxy*a1j2rx)*
     & a1j2rxrr+(4*a1j2sxxx*a1j2rxy+a1j2ry*a1j2sxxxx+4*a1j2sxxxy*
     & a1j2rx+4*a1j2sxy*a1j2rxxx+4*a1j2sx*a1j2rxxxy+6*a1j2sxx*
     & a1j2rxxy+a1j2sy*a1j2rxxxx+6*a1j2sxxy*a1j2rxx)*a1j2rxrs+(4*
     & a1j2sxxx*a1j2sxy+4*a1j2sxxxy*a1j2sx+6*a1j2sxx*a1j2sxxy+a1j2sy*
     & a1j2sxxxx)*a1j2rxss
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
      t219 = a1j2sy*t184+a1j2sx*t175+a1j2rxyy*t17+2*a1j2sxy*t61+a1j2ry*
     & t198+3*a1j2ryy*a1j2sxx*a1j2sx+a1j2ry*t206+a1j2rx*t211+2*t208*
     & a1j2rx+a1j2sy*t165+4*t152*a1j2sx+a1j2syy*t71
      a1j2rxxxxyy = t1*t3*a1j2rxrrrrr+(t6*t3+a1j2ry*t12)*a1j2rxrrrrs+(
     & a1j2ry*t22+a1j2sy*t12)*a1j2rxrrrss+(a1j2ry*t32+a1j2sy*t22)*
     & a1j2rxrrsss+(t6*t30+a1j2sy*t32)*a1j2rxrssss+t41*t30*
     & a1j2rxsssss+(a1j2ry*t47+3*a1j2ry*a1j2rxy*t2+a1j2ryy*t3)*
     & a1j2rxrrrr+(a1j2ry*t63+3*a1j2sy*a1j2rxy*t2+a1j2ry*t75+a1j2syy*
     & t3+a1j2sy*t47+3*a1j2ryy*t2*a1j2sx)*a1j2rxrrrs+(a1j2sy*t75+
     & a1j2ry*t89+a1j2ry*t95+a1j2sy*t63+3*a1j2syy*t2*a1j2sx+3*t101*
     & t17)*a1j2rxrrss+(3*t106*t17+a1j2sy*t89+3*a1j2ry*t17*a1j2sxy+
     & a1j2sy*t95+a1j2ry*t118+a1j2ryy*t30)*a1j2rxrsss+(3*a1j2sy*t17*
     & a1j2sxy+a1j2syy*t30+a1j2sy*t118)*a1j2rxssss+(a1j2ry*t133+
     & a1j2rxyy*t2+a1j2ry*t139+4*t141*a1j2rx+a1j2rx*t146+3*t101*
     & a1j2rxx)*a1j2rxrrr+t188*a1j2rxrrs+t219*a1j2rxrss+(4*t209*
     & a1j2sx+a1j2sy*t206+a1j2sx*t211+3*a1j2syy*a1j2sxx*a1j2sx+a1j2sy*
     & t198+a1j2sxyy*t17)*a1j2rxsss+(3*a1j2rx*a1j2rxxyy+6*a1j2rxy*
     & a1j2rxxy+3*a1j2rxx*a1j2rxyy+a1j2ryy*a1j2rxxx+2*a1j2ry*
     & a1j2rxxxy)*a1j2rxrr+(3*a1j2sxxyy*a1j2rx+6*a1j2sxxy*a1j2rxy+3*
     & a1j2sxyy*a1j2rxx+3*a1j2sx*a1j2rxxyy+3*a1j2sxx*a1j2rxyy+2*
     & a1j2ry*a1j2sxxxy+6*a1j2sxy*a1j2rxxy+a1j2ryy*a1j2sxxx+2*a1j2sy*
     & a1j2rxxxy+a1j2syy*a1j2rxxx)*a1j2rxrs+(3*a1j2sxxyy*a1j2sx+3*
     & a1j2sxyy*a1j2sxx+2*a1j2sy*a1j2sxxxy+a1j2syy*a1j2sxxx+6*a1j2sxy*
     & a1j2sxxy)*a1j2rxss
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
      a1j2rxxxyyy = t1*a1j2ry*t3*a1j2rxrrrrr+(a1j2ry*t14+a1j2sy*t1*t3)*
     & a1j2rxrrrrs+(a1j2ry*t28+a1j2sy*t14)*a1j2rxrrrss+(a1j2sy*t28+
     & a1j2ry*t36)*a1j2rxrrsss+(a1j2sy*t36+a1j2ry*t41*t21)*
     & a1j2rxrssss+t41*a1j2sy*t21*a1j2rxsssss+(a1j2ry*t58+a1j2ryy*
     & a1j2ry*t3+a1j2ry*t62)*a1j2rxrrrr+(a1j2sy*t58+a1j2ry*t86+a1j2ry*
     & t88+a1j2syy*a1j2ry*t3+a1j2ryy*t12+a1j2sy*t62)*a1j2rxrrrs+(
     & a1j2ry*t104+a1j2syy*t12+a1j2ry*t113+a1j2sy*t88+a1j2sy*t86+
     & a1j2ryy*t26)*a1j2rxrrss+(a1j2ry*t124+a1j2sy*t104+a1j2syy*t26+
     & a1j2ryy*a1j2sy*t21+a1j2sy*t113+a1j2ry*t132)*a1j2rxrsss+(a1j2sy*
     & t132+a1j2syy*a1j2sy*t21+a1j2sy*t124)*a1j2rxssss+(4*a1j2ryy*
     & a1j2rxy*a1j2rx+a1j2ry*t151+a1j2ryyy*t3+a1j2ry*t155+a1j2ryy*t55+
     & a1j2ry*t159)*a1j2rxrrr+t195*a1j2rxrrs+t225*a1j2rxrss+(a1j2sy*
     & t214+a1j2syyy*t21+4*a1j2syy*a1j2sxy*a1j2sx+a1j2syy*t111+a1j2sy*
     & t223+a1j2sy*t205)*a1j2rxsss+(a1j2ryyy*a1j2rxx+3*a1j2ry*
     & a1j2rxxyy+6*a1j2rxy*a1j2rxyy+2*a1j2rx*a1j2rxyyy+3*a1j2ryy*
     & a1j2rxxy)*a1j2rxrr+(a1j2ryyy*a1j2sxx+3*a1j2ry*a1j2sxxyy+3*
     & a1j2syy*a1j2rxxy+6*a1j2sxyy*a1j2rxy+a1j2syyy*a1j2rxx+2*a1j2sx*
     & a1j2rxyyy+3*a1j2sy*a1j2rxxyy+3*a1j2ryy*a1j2sxxy+6*a1j2sxy*
     & a1j2rxyy+2*a1j2sxyyy*a1j2rx)*a1j2rxrs+(3*a1j2syy*a1j2sxxy+2*
     & a1j2sx*a1j2sxyyy+a1j2syyy*a1j2sxx+3*a1j2sy*a1j2sxxyy+6*
     & a1j2sxyy*a1j2sxy)*a1j2rxss
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
      t188 = a1j2syyy*a1j2ry*a1j2rx+a1j2ryyy*t14+a1j2ry*t167+2*a1j2ryy*
     & t73+2*a1j2syy*t54+a1j2ryy*t85+a1j2ry*t178+a1j2syy*t61+a1j2sy*
     & t151+a1j2ry*t184+a1j2sy*t146+a1j2sy*t141
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
      a1j2rxxyyyy = t2*a1j2rx*a1j2rxrrrrr+(a1j2sy*t1*a1j2ry*a1j2rx+
     & a1j2ry*t18)*a1j2rxrrrrs+(a1j2ry*t27+a1j2sy*t18)*a1j2rxrrrss+(
     & a1j2ry*t36+a1j2sy*t27)*a1j2rxrrsss+(a1j2sy*t36+a1j2ry*t33*
     & a1j2sy*a1j2sx)*a1j2rxrssss+t47*a1j2sx*a1j2rxsssss+(a1j2ryy*t1*
     & a1j2rx+a1j2ry*t58+a1j2ry*t63)*a1j2rxrrrr+(a1j2ry*t77+a1j2sy*
     & t58+a1j2syy*t1*a1j2rx+a1j2ry*t87+a1j2sy*t63+a1j2ryy*t16)*
     & a1j2rxrrrs+(a1j2syy*t16+a1j2ry*t106+a1j2ry*t108+a1j2sy*t77+
     & a1j2sy*t87+a1j2ryy*t25)*a1j2rxrrss+(a1j2syy*t25+a1j2sy*t108+
     & a1j2ry*t120+a1j2ry*t123+a1j2sy*t106+a1j2ryy*t33*a1j2sx)*
     & a1j2rxrsss+(a1j2syy*t33*a1j2sx+a1j2sy*t120+a1j2sy*t123)*
     & a1j2rxssss+(a1j2ry*t141+2*a1j2ryy*t54+a1j2ry*t146+a1j2ryyy*
     & a1j2ry*a1j2rx+a1j2ry*t151+a1j2ryy*t61)*a1j2rxrrr+t188*
     & a1j2rxrrs+t215*a1j2rxrss+(a1j2syyy*a1j2sy*a1j2sx+2*a1j2syy*
     & t102+a1j2sy*t200+a1j2sy*t203+a1j2syy*t98+a1j2sy*t208)*
     & a1j2rxsss+(a1j2ryyyy*a1j2rx+4*a1j2ry*a1j2rxyyy+4*a1j2ryyy*
     & a1j2rxy+6*a1j2ryy*a1j2rxyy)*a1j2rxrr+(4*a1j2ryyy*a1j2sxy+
     & a1j2syyyy*a1j2rx+4*a1j2sy*a1j2rxyyy+6*a1j2syy*a1j2rxyy+4*
     & a1j2ry*a1j2sxyyy+4*a1j2syyy*a1j2rxy+6*a1j2ryy*a1j2sxyy+
     & a1j2ryyyy*a1j2sx)*a1j2rxrs+(4*a1j2sy*a1j2sxyyy+a1j2syyyy*
     & a1j2sx+4*a1j2syyy*a1j2sxy+6*a1j2syy*a1j2sxyy)*a1j2rxss
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
      t121 = 7*t30*a1j2ryy+a1j2sy*t89+a1j2ry*t102+a1j2ryy*t41+a1j2syyy*
     & t1+a1j2ry*t109+a1j2sy*t87+a1j2sy*t81+2*t96*a1j2ry+a1j2ry*t117+
     & 2*a1j2ryy*t32
      t129 = a1j2syyy*a1j2sy
      t131 = a1j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a1j2sy+a1j2sy*t109+2*a1j2syy*t32+a1j2sy*t117+
     & a1j2ry*t133+a1j2ry*t136+a1j2ry*t138+a1j2syy*t41+a1j2ryyy*t8+2*
     & t129*a1j2ry+a1j2sy*t102
      a1j2rxyyyyy = t2*a1j2ry*a1j2rxrrrrr+5*a1j2sy*t2*a1j2rxrrrrs+10*
     & t8*t9*a1j2rxrrrss+10*t13*t1*a1j2rxrrsss+5*t17*a1j2ry*
     & a1j2rxrssss+t17*a1j2sy*a1j2rxsssss+10*t9*a1j2ryy*a1j2rxrrrr+(
     & 12*t26*t1+a1j2ry*t37+a1j2syy*t9+a1j2ry*t43)*a1j2rxrrrs+(3*
     & a1j2ryy*a1j2ry*t8+a1j2sy*t43+a1j2sy*t37+a1j2ry*t56+a1j2ry*t60+
     & 3*t29*a1j2sy)*a1j2rxrrss+(12*a1j2ry*t8*a1j2syy+a1j2sy*t56+
     & a1j2sy*t60+a1j2ryy*t13)*a1j2rxrsss+10*t13*a1j2syy*a1j2rxssss+(
     & a1j2ryyy*t1+a1j2ry*t81+7*t79*a1j2ry+a1j2ry*t87+a1j2ry*t89)*
     & a1j2rxrrr+t121*a1j2rxrrs+t145*a1j2rxrss+(a1j2sy*t138+a1j2sy*
     & t133+7*t131*a1j2sy+a1j2syyy*t8+a1j2sy*t136)*a1j2rxsss+(5*
     & a1j2ry*a1j2ryyyy+10*a1j2ryyy*a1j2ryy)*a1j2rxrr+(10*a1j2syy*
     & a1j2ryyy+10*a1j2syyy*a1j2ryy+5*a1j2sy*a1j2ryyyy+5*a1j2syyyy*
     & a1j2ry)*a1j2rxrs+(10*a1j2syy*a1j2syyy+5*a1j2sy*a1j2syyyy)*
     & a1j2rxss
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
      a1j2sxxxxxx = t2*a1j2rx*a1j2sxrrrrr+5*a1j2sx*t2*a1j2sxrrrrs+10*
     & t8*t9*a1j2sxrrrss+10*t13*t1*a1j2sxrrsss+5*t17*a1j2rx*
     & a1j2sxrssss+t17*a1j2sx*a1j2sxsssss+10*a1j2rxx*t9*a1j2sxrrrr+(
     & 12*a1j2rxx*t1*a1j2sx+a1j2rx*t38+a1j2sxx*t9+a1j2rx*t44)*
     & a1j2sxrrrs+(3*a1j2rxx*a1j2rx*t8+a1j2rx*t55+a1j2rx*t59+a1j2sx*
     & t44+3*t29*a1j2sx+a1j2sx*t38)*a1j2sxrrss+(a1j2rxx*t13+12*t31*t8+
     & a1j2sx*t55+a1j2sx*t59)*a1j2sxrsss+10*a1j2sxx*t13*a1j2sxssss+(
     & a1j2rx*t81+7*t79*a1j2rx+a1j2rx*t86+a1j2rx*t88+a1j2rxxx*t1)*
     & a1j2sxrrr+t121*a1j2sxrrs+t145*a1j2sxrss+(a1j2sx*t129+7*t127*
     & a1j2sx+a1j2sx*t136+a1j2sx*t133+a1j2sxxx*t8)*a1j2sxsss+(10*
     & a1j2rxx*a1j2rxxx+5*a1j2rx*a1j2rxxxx)*a1j2sxrr+(5*a1j2sxxxx*
     & a1j2rx+10*a1j2sxx*a1j2rxxx+5*a1j2sx*a1j2rxxxx+10*a1j2sxxx*
     & a1j2rxx)*a1j2sxrs+(5*a1j2sxxxx*a1j2sx+10*a1j2sxx*a1j2sxxx)*
     & a1j2sxss
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
      a1j2sxxxxxy = a1j2ry*t2*a1j2sxrrrrr+(4*a1j2ry*t5*a1j2sx+a1j2sy*
     & t2)*a1j2sxrrrrs+(4*a1j2sy*t5*a1j2sx+6*t15*t16)*a1j2sxrrrss+(4*
     & a1j2ry*a1j2rx*t22+6*t25*t16)*a1j2sxrrsss+(a1j2ry*t30+4*a1j2sy*
     & a1j2rx*t22)*a1j2sxrssss+a1j2sy*t30*a1j2sxsssss+(4*t5*a1j2rxy+6*
     & t15*a1j2rxx)*a1j2sxrrrr+(6*t25*a1j2rxx+a1j2rx*t55+6*t47*t1+
     & a1j2sxy*t5+a1j2ry*t71)*a1j2sxrrrs+(3*t50*a1j2sx+a1j2rx*t81+
     & a1j2sy*t71+a1j2ry*t89+3*a1j2rxy*a1j2rx*t16+a1j2sx*t55)*
     & a1j2sxrrss+(a1j2sy*t89+6*t51*t16+a1j2sx*t81+a1j2rxy*t22+6*
     & a1j2ry*t16*a1j2sxx)*a1j2sxrsss+(4*t22*a1j2sxy+6*a1j2sy*t16*
     & a1j2sxx)*a1j2sxssss+(a1j2rxxy*t1+7*t115*a1j2rx+a1j2rx*t120+
     & a1j2rx*t122+a1j2ry*t128)*a1j2sxrrr+t162*a1j2sxrrs+t190*
     & a1j2sxrss+(a1j2sx*t174+a1j2sxxy*t16+7*a1j2sxy*a1j2sx*a1j2sxx+
     & a1j2sx*t179+a1j2sy*t185)*a1j2sxsss+(a1j2ry*a1j2rxxxx+4*
     & a1j2rxxx*a1j2rxy+6*a1j2rxxy*a1j2rxx+4*a1j2rxxxy*a1j2rx)*
     & a1j2sxrr+(4*a1j2sxxx*a1j2rxy+a1j2ry*a1j2sxxxx+4*a1j2sxxxy*
     & a1j2rx+4*a1j2sxy*a1j2rxxx+4*a1j2sx*a1j2rxxxy+6*a1j2sxx*
     & a1j2rxxy+a1j2sy*a1j2rxxxx+6*a1j2sxxy*a1j2rxx)*a1j2sxrs+(4*
     & a1j2sxxx*a1j2sxy+4*a1j2sxxxy*a1j2sx+6*a1j2sxx*a1j2sxxy+a1j2sy*
     & a1j2sxxxx)*a1j2sxss
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
      t219 = a1j2sy*t184+a1j2sx*t175+a1j2rxyy*t17+2*a1j2sxy*t61+a1j2ry*
     & t198+3*a1j2ryy*a1j2sxx*a1j2sx+a1j2ry*t206+a1j2rx*t211+2*t208*
     & a1j2rx+a1j2sy*t165+4*t152*a1j2sx+a1j2syy*t71
      a1j2sxxxxyy = t1*t3*a1j2sxrrrrr+(t6*t3+a1j2ry*t12)*a1j2sxrrrrs+(
     & a1j2ry*t22+a1j2sy*t12)*a1j2sxrrrss+(a1j2ry*t32+a1j2sy*t22)*
     & a1j2sxrrsss+(t6*t30+a1j2sy*t32)*a1j2sxrssss+t41*t30*
     & a1j2sxsssss+(a1j2ry*t47+3*a1j2ry*a1j2rxy*t2+a1j2ryy*t3)*
     & a1j2sxrrrr+(a1j2ry*t63+3*a1j2sy*a1j2rxy*t2+a1j2ry*t75+a1j2syy*
     & t3+a1j2sy*t47+3*a1j2ryy*t2*a1j2sx)*a1j2sxrrrs+(a1j2sy*t75+
     & a1j2ry*t89+a1j2ry*t95+a1j2sy*t63+3*a1j2syy*t2*a1j2sx+3*t101*
     & t17)*a1j2sxrrss+(3*t106*t17+a1j2sy*t89+3*a1j2ry*t17*a1j2sxy+
     & a1j2sy*t95+a1j2ry*t118+a1j2ryy*t30)*a1j2sxrsss+(3*a1j2sy*t17*
     & a1j2sxy+a1j2syy*t30+a1j2sy*t118)*a1j2sxssss+(a1j2ry*t133+
     & a1j2rxyy*t2+a1j2ry*t139+4*t141*a1j2rx+a1j2rx*t146+3*t101*
     & a1j2rxx)*a1j2sxrrr+t188*a1j2sxrrs+t219*a1j2sxrss+(4*t209*
     & a1j2sx+a1j2sy*t206+a1j2sx*t211+3*a1j2syy*a1j2sxx*a1j2sx+a1j2sy*
     & t198+a1j2sxyy*t17)*a1j2sxsss+(3*a1j2rx*a1j2rxxyy+6*a1j2rxy*
     & a1j2rxxy+3*a1j2rxx*a1j2rxyy+a1j2ryy*a1j2rxxx+2*a1j2ry*
     & a1j2rxxxy)*a1j2sxrr+(3*a1j2sxxyy*a1j2rx+6*a1j2sxxy*a1j2rxy+3*
     & a1j2sxyy*a1j2rxx+3*a1j2sx*a1j2rxxyy+3*a1j2sxx*a1j2rxyy+2*
     & a1j2ry*a1j2sxxxy+6*a1j2sxy*a1j2rxxy+a1j2ryy*a1j2sxxx+2*a1j2sy*
     & a1j2rxxxy+a1j2syy*a1j2rxxx)*a1j2sxrs+(3*a1j2sxxyy*a1j2sx+3*
     & a1j2sxyy*a1j2sxx+2*a1j2sy*a1j2sxxxy+a1j2syy*a1j2sxxx+6*a1j2sxy*
     & a1j2sxxy)*a1j2sxss
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
      a1j2sxxxyyy = t1*a1j2ry*t3*a1j2sxrrrrr+(a1j2ry*t14+a1j2sy*t1*t3)*
     & a1j2sxrrrrs+(a1j2ry*t28+a1j2sy*t14)*a1j2sxrrrss+(a1j2sy*t28+
     & a1j2ry*t36)*a1j2sxrrsss+(a1j2sy*t36+a1j2ry*t41*t21)*
     & a1j2sxrssss+t41*a1j2sy*t21*a1j2sxsssss+(a1j2ry*t58+a1j2ryy*
     & a1j2ry*t3+a1j2ry*t62)*a1j2sxrrrr+(a1j2sy*t58+a1j2ry*t86+a1j2ry*
     & t88+a1j2syy*a1j2ry*t3+a1j2ryy*t12+a1j2sy*t62)*a1j2sxrrrs+(
     & a1j2ry*t104+a1j2syy*t12+a1j2ry*t113+a1j2sy*t88+a1j2sy*t86+
     & a1j2ryy*t26)*a1j2sxrrss+(a1j2ry*t124+a1j2sy*t104+a1j2syy*t26+
     & a1j2ryy*a1j2sy*t21+a1j2sy*t113+a1j2ry*t132)*a1j2sxrsss+(a1j2sy*
     & t132+a1j2syy*a1j2sy*t21+a1j2sy*t124)*a1j2sxssss+(4*a1j2ryy*
     & a1j2rxy*a1j2rx+a1j2ry*t151+a1j2ryyy*t3+a1j2ry*t155+a1j2ryy*t55+
     & a1j2ry*t159)*a1j2sxrrr+t195*a1j2sxrrs+t225*a1j2sxrss+(a1j2sy*
     & t214+a1j2syyy*t21+4*a1j2syy*a1j2sxy*a1j2sx+a1j2syy*t111+a1j2sy*
     & t223+a1j2sy*t205)*a1j2sxsss+(a1j2ryyy*a1j2rxx+3*a1j2ry*
     & a1j2rxxyy+6*a1j2rxy*a1j2rxyy+2*a1j2rx*a1j2rxyyy+3*a1j2ryy*
     & a1j2rxxy)*a1j2sxrr+(a1j2ryyy*a1j2sxx+3*a1j2ry*a1j2sxxyy+3*
     & a1j2syy*a1j2rxxy+6*a1j2sxyy*a1j2rxy+a1j2syyy*a1j2rxx+2*a1j2sx*
     & a1j2rxyyy+3*a1j2sy*a1j2rxxyy+3*a1j2ryy*a1j2sxxy+6*a1j2sxy*
     & a1j2rxyy+2*a1j2sxyyy*a1j2rx)*a1j2sxrs+(3*a1j2syy*a1j2sxxy+2*
     & a1j2sx*a1j2sxyyy+a1j2syyy*a1j2sxx+3*a1j2sy*a1j2sxxyy+6*
     & a1j2sxyy*a1j2sxy)*a1j2sxss
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
      t188 = a1j2syyy*a1j2ry*a1j2rx+a1j2ryyy*t14+a1j2ry*t167+2*a1j2ryy*
     & t73+2*a1j2syy*t54+a1j2ryy*t85+a1j2ry*t178+a1j2syy*t61+a1j2sy*
     & t151+a1j2ry*t184+a1j2sy*t146+a1j2sy*t141
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
      a1j2sxxyyyy = t2*a1j2rx*a1j2sxrrrrr+(a1j2sy*t1*a1j2ry*a1j2rx+
     & a1j2ry*t18)*a1j2sxrrrrs+(a1j2ry*t27+a1j2sy*t18)*a1j2sxrrrss+(
     & a1j2ry*t36+a1j2sy*t27)*a1j2sxrrsss+(a1j2sy*t36+a1j2ry*t33*
     & a1j2sy*a1j2sx)*a1j2sxrssss+t47*a1j2sx*a1j2sxsssss+(a1j2ryy*t1*
     & a1j2rx+a1j2ry*t58+a1j2ry*t63)*a1j2sxrrrr+(a1j2ry*t77+a1j2sy*
     & t58+a1j2syy*t1*a1j2rx+a1j2ry*t87+a1j2sy*t63+a1j2ryy*t16)*
     & a1j2sxrrrs+(a1j2syy*t16+a1j2ry*t106+a1j2ry*t108+a1j2sy*t77+
     & a1j2sy*t87+a1j2ryy*t25)*a1j2sxrrss+(a1j2syy*t25+a1j2sy*t108+
     & a1j2ry*t120+a1j2ry*t123+a1j2sy*t106+a1j2ryy*t33*a1j2sx)*
     & a1j2sxrsss+(a1j2syy*t33*a1j2sx+a1j2sy*t120+a1j2sy*t123)*
     & a1j2sxssss+(a1j2ry*t141+2*a1j2ryy*t54+a1j2ry*t146+a1j2ryyy*
     & a1j2ry*a1j2rx+a1j2ry*t151+a1j2ryy*t61)*a1j2sxrrr+t188*
     & a1j2sxrrs+t215*a1j2sxrss+(a1j2syyy*a1j2sy*a1j2sx+2*a1j2syy*
     & t102+a1j2sy*t200+a1j2sy*t203+a1j2syy*t98+a1j2sy*t208)*
     & a1j2sxsss+(a1j2ryyyy*a1j2rx+4*a1j2ry*a1j2rxyyy+4*a1j2ryyy*
     & a1j2rxy+6*a1j2ryy*a1j2rxyy)*a1j2sxrr+(4*a1j2ryyy*a1j2sxy+
     & a1j2syyyy*a1j2rx+4*a1j2sy*a1j2rxyyy+6*a1j2syy*a1j2rxyy+4*
     & a1j2ry*a1j2sxyyy+4*a1j2syyy*a1j2rxy+6*a1j2ryy*a1j2sxyy+
     & a1j2ryyyy*a1j2sx)*a1j2sxrs+(4*a1j2sy*a1j2sxyyy+a1j2syyyy*
     & a1j2sx+4*a1j2syyy*a1j2sxy+6*a1j2syy*a1j2sxyy)*a1j2sxss
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
      t121 = 7*t30*a1j2ryy+a1j2sy*t89+a1j2ry*t102+a1j2ryy*t41+a1j2syyy*
     & t1+a1j2ry*t109+a1j2sy*t87+a1j2sy*t81+2*t96*a1j2ry+a1j2ry*t117+
     & 2*a1j2ryy*t32
      t129 = a1j2syyy*a1j2sy
      t131 = a1j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a1j2sy+a1j2sy*t109+2*a1j2syy*t32+a1j2sy*t117+
     & a1j2ry*t133+a1j2ry*t136+a1j2ry*t138+a1j2syy*t41+a1j2ryyy*t8+2*
     & t129*a1j2ry+a1j2sy*t102
      a1j2sxyyyyy = t2*a1j2ry*a1j2sxrrrrr+5*a1j2sy*t2*a1j2sxrrrrs+10*
     & t8*t9*a1j2sxrrrss+10*t13*t1*a1j2sxrrsss+5*t17*a1j2ry*
     & a1j2sxrssss+t17*a1j2sy*a1j2sxsssss+10*t9*a1j2ryy*a1j2sxrrrr+(
     & 12*t26*t1+a1j2ry*t37+a1j2syy*t9+a1j2ry*t43)*a1j2sxrrrs+(3*
     & a1j2ryy*a1j2ry*t8+a1j2sy*t43+a1j2sy*t37+a1j2ry*t56+a1j2ry*t60+
     & 3*t29*a1j2sy)*a1j2sxrrss+(12*a1j2ry*t8*a1j2syy+a1j2sy*t56+
     & a1j2sy*t60+a1j2ryy*t13)*a1j2sxrsss+10*t13*a1j2syy*a1j2sxssss+(
     & a1j2ryyy*t1+a1j2ry*t81+7*t79*a1j2ry+a1j2ry*t87+a1j2ry*t89)*
     & a1j2sxrrr+t121*a1j2sxrrs+t145*a1j2sxrss+(a1j2sy*t138+a1j2sy*
     & t133+7*t131*a1j2sy+a1j2syyy*t8+a1j2sy*t136)*a1j2sxsss+(5*
     & a1j2ry*a1j2ryyyy+10*a1j2ryyy*a1j2ryy)*a1j2sxrr+(10*a1j2syy*
     & a1j2ryyy+10*a1j2syyy*a1j2ryy+5*a1j2sy*a1j2ryyyy+5*a1j2syyyy*
     & a1j2ry)*a1j2sxrs+(10*a1j2syy*a1j2syyy+5*a1j2sy*a1j2syyyy)*
     & a1j2sxss
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
      a1j2ryxxxxx = t2*a1j2rx*a1j2ryrrrrr+5*a1j2sx*t2*a1j2ryrrrrs+10*
     & t8*t9*a1j2ryrrrss+10*t13*t1*a1j2ryrrsss+5*t17*a1j2rx*
     & a1j2ryrssss+t17*a1j2sx*a1j2rysssss+10*a1j2rxx*t9*a1j2ryrrrr+(
     & 12*a1j2rxx*t1*a1j2sx+a1j2rx*t38+a1j2sxx*t9+a1j2rx*t44)*
     & a1j2ryrrrs+(3*a1j2rxx*a1j2rx*t8+a1j2rx*t55+a1j2rx*t59+a1j2sx*
     & t44+3*t29*a1j2sx+a1j2sx*t38)*a1j2ryrrss+(a1j2rxx*t13+12*t31*t8+
     & a1j2sx*t55+a1j2sx*t59)*a1j2ryrsss+10*a1j2sxx*t13*a1j2ryssss+(
     & a1j2rx*t81+7*t79*a1j2rx+a1j2rx*t86+a1j2rx*t88+a1j2rxxx*t1)*
     & a1j2ryrrr+t121*a1j2ryrrs+t145*a1j2ryrss+(a1j2sx*t129+7*t127*
     & a1j2sx+a1j2sx*t136+a1j2sx*t133+a1j2sxxx*t8)*a1j2rysss+(10*
     & a1j2rxx*a1j2rxxx+5*a1j2rx*a1j2rxxxx)*a1j2ryrr+(5*a1j2sxxxx*
     & a1j2rx+10*a1j2sxx*a1j2rxxx+5*a1j2sx*a1j2rxxxx+10*a1j2sxxx*
     & a1j2rxx)*a1j2ryrs+(5*a1j2sxxxx*a1j2sx+10*a1j2sxx*a1j2sxxx)*
     & a1j2ryss
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
      a1j2ryxxxxy = a1j2ry*t2*a1j2ryrrrrr+(4*a1j2ry*t5*a1j2sx+a1j2sy*
     & t2)*a1j2ryrrrrs+(4*a1j2sy*t5*a1j2sx+6*t15*t16)*a1j2ryrrrss+(4*
     & a1j2ry*a1j2rx*t22+6*t25*t16)*a1j2ryrrsss+(a1j2ry*t30+4*a1j2sy*
     & a1j2rx*t22)*a1j2ryrssss+a1j2sy*t30*a1j2rysssss+(4*t5*a1j2rxy+6*
     & t15*a1j2rxx)*a1j2ryrrrr+(6*t25*a1j2rxx+a1j2rx*t55+6*t47*t1+
     & a1j2sxy*t5+a1j2ry*t71)*a1j2ryrrrs+(3*t50*a1j2sx+a1j2rx*t81+
     & a1j2sy*t71+a1j2ry*t89+3*a1j2rxy*a1j2rx*t16+a1j2sx*t55)*
     & a1j2ryrrss+(a1j2sy*t89+6*t51*t16+a1j2sx*t81+a1j2rxy*t22+6*
     & a1j2ry*t16*a1j2sxx)*a1j2ryrsss+(4*t22*a1j2sxy+6*a1j2sy*t16*
     & a1j2sxx)*a1j2ryssss+(a1j2rxxy*t1+7*t115*a1j2rx+a1j2rx*t120+
     & a1j2rx*t122+a1j2ry*t128)*a1j2ryrrr+t162*a1j2ryrrs+t190*
     & a1j2ryrss+(a1j2sx*t174+a1j2sxxy*t16+7*a1j2sxy*a1j2sx*a1j2sxx+
     & a1j2sx*t179+a1j2sy*t185)*a1j2rysss+(a1j2ry*a1j2rxxxx+4*
     & a1j2rxxx*a1j2rxy+6*a1j2rxxy*a1j2rxx+4*a1j2rxxxy*a1j2rx)*
     & a1j2ryrr+(4*a1j2sxxx*a1j2rxy+a1j2ry*a1j2sxxxx+4*a1j2sxxxy*
     & a1j2rx+4*a1j2sxy*a1j2rxxx+4*a1j2sx*a1j2rxxxy+6*a1j2sxx*
     & a1j2rxxy+a1j2sy*a1j2rxxxx+6*a1j2sxxy*a1j2rxx)*a1j2ryrs+(4*
     & a1j2sxxx*a1j2sxy+4*a1j2sxxxy*a1j2sx+6*a1j2sxx*a1j2sxxy+a1j2sy*
     & a1j2sxxxx)*a1j2ryss
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
      t219 = a1j2sy*t184+a1j2sx*t175+a1j2rxyy*t17+2*a1j2sxy*t61+a1j2ry*
     & t198+3*a1j2ryy*a1j2sxx*a1j2sx+a1j2ry*t206+a1j2rx*t211+2*t208*
     & a1j2rx+a1j2sy*t165+4*t152*a1j2sx+a1j2syy*t71
      a1j2ryxxxyy = t1*t3*a1j2ryrrrrr+(t6*t3+a1j2ry*t12)*a1j2ryrrrrs+(
     & a1j2ry*t22+a1j2sy*t12)*a1j2ryrrrss+(a1j2ry*t32+a1j2sy*t22)*
     & a1j2ryrrsss+(t6*t30+a1j2sy*t32)*a1j2ryrssss+t41*t30*
     & a1j2rysssss+(a1j2ry*t47+3*a1j2ry*a1j2rxy*t2+a1j2ryy*t3)*
     & a1j2ryrrrr+(a1j2ry*t63+3*a1j2sy*a1j2rxy*t2+a1j2ry*t75+a1j2syy*
     & t3+a1j2sy*t47+3*a1j2ryy*t2*a1j2sx)*a1j2ryrrrs+(a1j2sy*t75+
     & a1j2ry*t89+a1j2ry*t95+a1j2sy*t63+3*a1j2syy*t2*a1j2sx+3*t101*
     & t17)*a1j2ryrrss+(3*t106*t17+a1j2sy*t89+3*a1j2ry*t17*a1j2sxy+
     & a1j2sy*t95+a1j2ry*t118+a1j2ryy*t30)*a1j2ryrsss+(3*a1j2sy*t17*
     & a1j2sxy+a1j2syy*t30+a1j2sy*t118)*a1j2ryssss+(a1j2ry*t133+
     & a1j2rxyy*t2+a1j2ry*t139+4*t141*a1j2rx+a1j2rx*t146+3*t101*
     & a1j2rxx)*a1j2ryrrr+t188*a1j2ryrrs+t219*a1j2ryrss+(4*t209*
     & a1j2sx+a1j2sy*t206+a1j2sx*t211+3*a1j2syy*a1j2sxx*a1j2sx+a1j2sy*
     & t198+a1j2sxyy*t17)*a1j2rysss+(3*a1j2rx*a1j2rxxyy+6*a1j2rxy*
     & a1j2rxxy+3*a1j2rxx*a1j2rxyy+a1j2ryy*a1j2rxxx+2*a1j2ry*
     & a1j2rxxxy)*a1j2ryrr+(3*a1j2sxxyy*a1j2rx+6*a1j2sxxy*a1j2rxy+3*
     & a1j2sxyy*a1j2rxx+3*a1j2sx*a1j2rxxyy+3*a1j2sxx*a1j2rxyy+2*
     & a1j2ry*a1j2sxxxy+6*a1j2sxy*a1j2rxxy+a1j2ryy*a1j2sxxx+2*a1j2sy*
     & a1j2rxxxy+a1j2syy*a1j2rxxx)*a1j2ryrs+(3*a1j2sxxyy*a1j2sx+3*
     & a1j2sxyy*a1j2sxx+2*a1j2sy*a1j2sxxxy+a1j2syy*a1j2sxxx+6*a1j2sxy*
     & a1j2sxxy)*a1j2ryss
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
      a1j2ryxxyyy = t1*a1j2ry*t3*a1j2ryrrrrr+(a1j2ry*t14+a1j2sy*t1*t3)*
     & a1j2ryrrrrs+(a1j2ry*t28+a1j2sy*t14)*a1j2ryrrrss+(a1j2sy*t28+
     & a1j2ry*t36)*a1j2ryrrsss+(a1j2sy*t36+a1j2ry*t41*t21)*
     & a1j2ryrssss+t41*a1j2sy*t21*a1j2rysssss+(a1j2ry*t58+a1j2ryy*
     & a1j2ry*t3+a1j2ry*t62)*a1j2ryrrrr+(a1j2sy*t58+a1j2ry*t86+a1j2ry*
     & t88+a1j2syy*a1j2ry*t3+a1j2ryy*t12+a1j2sy*t62)*a1j2ryrrrs+(
     & a1j2ry*t104+a1j2syy*t12+a1j2ry*t113+a1j2sy*t88+a1j2sy*t86+
     & a1j2ryy*t26)*a1j2ryrrss+(a1j2ry*t124+a1j2sy*t104+a1j2syy*t26+
     & a1j2ryy*a1j2sy*t21+a1j2sy*t113+a1j2ry*t132)*a1j2ryrsss+(a1j2sy*
     & t132+a1j2syy*a1j2sy*t21+a1j2sy*t124)*a1j2ryssss+(4*a1j2ryy*
     & a1j2rxy*a1j2rx+a1j2ry*t151+a1j2ryyy*t3+a1j2ry*t155+a1j2ryy*t55+
     & a1j2ry*t159)*a1j2ryrrr+t195*a1j2ryrrs+t225*a1j2ryrss+(a1j2sy*
     & t214+a1j2syyy*t21+4*a1j2syy*a1j2sxy*a1j2sx+a1j2syy*t111+a1j2sy*
     & t223+a1j2sy*t205)*a1j2rysss+(a1j2ryyy*a1j2rxx+3*a1j2ry*
     & a1j2rxxyy+6*a1j2rxy*a1j2rxyy+2*a1j2rx*a1j2rxyyy+3*a1j2ryy*
     & a1j2rxxy)*a1j2ryrr+(a1j2ryyy*a1j2sxx+3*a1j2ry*a1j2sxxyy+3*
     & a1j2syy*a1j2rxxy+6*a1j2sxyy*a1j2rxy+a1j2syyy*a1j2rxx+2*a1j2sx*
     & a1j2rxyyy+3*a1j2sy*a1j2rxxyy+3*a1j2ryy*a1j2sxxy+6*a1j2sxy*
     & a1j2rxyy+2*a1j2sxyyy*a1j2rx)*a1j2ryrs+(3*a1j2syy*a1j2sxxy+2*
     & a1j2sx*a1j2sxyyy+a1j2syyy*a1j2sxx+3*a1j2sy*a1j2sxxyy+6*
     & a1j2sxyy*a1j2sxy)*a1j2ryss
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
      t188 = a1j2syyy*a1j2ry*a1j2rx+a1j2ryyy*t14+a1j2ry*t167+2*a1j2ryy*
     & t73+2*a1j2syy*t54+a1j2ryy*t85+a1j2ry*t178+a1j2syy*t61+a1j2sy*
     & t151+a1j2ry*t184+a1j2sy*t146+a1j2sy*t141
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
      a1j2ryxyyyy = t2*a1j2rx*a1j2ryrrrrr+(a1j2sy*t1*a1j2ry*a1j2rx+
     & a1j2ry*t18)*a1j2ryrrrrs+(a1j2ry*t27+a1j2sy*t18)*a1j2ryrrrss+(
     & a1j2ry*t36+a1j2sy*t27)*a1j2ryrrsss+(a1j2sy*t36+a1j2ry*t33*
     & a1j2sy*a1j2sx)*a1j2ryrssss+t47*a1j2sx*a1j2rysssss+(a1j2ryy*t1*
     & a1j2rx+a1j2ry*t58+a1j2ry*t63)*a1j2ryrrrr+(a1j2ry*t77+a1j2sy*
     & t58+a1j2syy*t1*a1j2rx+a1j2ry*t87+a1j2sy*t63+a1j2ryy*t16)*
     & a1j2ryrrrs+(a1j2syy*t16+a1j2ry*t106+a1j2ry*t108+a1j2sy*t77+
     & a1j2sy*t87+a1j2ryy*t25)*a1j2ryrrss+(a1j2syy*t25+a1j2sy*t108+
     & a1j2ry*t120+a1j2ry*t123+a1j2sy*t106+a1j2ryy*t33*a1j2sx)*
     & a1j2ryrsss+(a1j2syy*t33*a1j2sx+a1j2sy*t120+a1j2sy*t123)*
     & a1j2ryssss+(a1j2ry*t141+2*a1j2ryy*t54+a1j2ry*t146+a1j2ryyy*
     & a1j2ry*a1j2rx+a1j2ry*t151+a1j2ryy*t61)*a1j2ryrrr+t188*
     & a1j2ryrrs+t215*a1j2ryrss+(a1j2syyy*a1j2sy*a1j2sx+2*a1j2syy*
     & t102+a1j2sy*t200+a1j2sy*t203+a1j2syy*t98+a1j2sy*t208)*
     & a1j2rysss+(a1j2ryyyy*a1j2rx+4*a1j2ry*a1j2rxyyy+4*a1j2ryyy*
     & a1j2rxy+6*a1j2ryy*a1j2rxyy)*a1j2ryrr+(4*a1j2ryyy*a1j2sxy+
     & a1j2syyyy*a1j2rx+4*a1j2sy*a1j2rxyyy+6*a1j2syy*a1j2rxyy+4*
     & a1j2ry*a1j2sxyyy+4*a1j2syyy*a1j2rxy+6*a1j2ryy*a1j2sxyy+
     & a1j2ryyyy*a1j2sx)*a1j2ryrs+(4*a1j2sy*a1j2sxyyy+a1j2syyyy*
     & a1j2sx+4*a1j2syyy*a1j2sxy+6*a1j2syy*a1j2sxyy)*a1j2ryss
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
      t121 = 7*t30*a1j2ryy+a1j2sy*t89+a1j2ry*t102+a1j2ryy*t41+a1j2syyy*
     & t1+a1j2ry*t109+a1j2sy*t87+a1j2sy*t81+2*t96*a1j2ry+a1j2ry*t117+
     & 2*a1j2ryy*t32
      t129 = a1j2syyy*a1j2sy
      t131 = a1j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a1j2sy+a1j2sy*t109+2*a1j2syy*t32+a1j2sy*t117+
     & a1j2ry*t133+a1j2ry*t136+a1j2ry*t138+a1j2syy*t41+a1j2ryyy*t8+2*
     & t129*a1j2ry+a1j2sy*t102
      a1j2ryyyyyy = t2*a1j2ry*a1j2ryrrrrr+5*a1j2sy*t2*a1j2ryrrrrs+10*
     & t8*t9*a1j2ryrrrss+10*t13*t1*a1j2ryrrsss+5*t17*a1j2ry*
     & a1j2ryrssss+t17*a1j2sy*a1j2rysssss+10*t9*a1j2ryy*a1j2ryrrrr+(
     & 12*t26*t1+a1j2ry*t37+a1j2syy*t9+a1j2ry*t43)*a1j2ryrrrs+(3*
     & a1j2ryy*a1j2ry*t8+a1j2sy*t43+a1j2sy*t37+a1j2ry*t56+a1j2ry*t60+
     & 3*t29*a1j2sy)*a1j2ryrrss+(12*a1j2ry*t8*a1j2syy+a1j2sy*t56+
     & a1j2sy*t60+a1j2ryy*t13)*a1j2ryrsss+10*t13*a1j2syy*a1j2ryssss+(
     & a1j2ryyy*t1+a1j2ry*t81+7*t79*a1j2ry+a1j2ry*t87+a1j2ry*t89)*
     & a1j2ryrrr+t121*a1j2ryrrs+t145*a1j2ryrss+(a1j2sy*t138+a1j2sy*
     & t133+7*t131*a1j2sy+a1j2syyy*t8+a1j2sy*t136)*a1j2rysss+(5*
     & a1j2ry*a1j2ryyyy+10*a1j2ryyy*a1j2ryy)*a1j2ryrr+(10*a1j2syy*
     & a1j2ryyy+10*a1j2syyy*a1j2ryy+5*a1j2sy*a1j2ryyyy+5*a1j2syyyy*
     & a1j2ry)*a1j2ryrs+(10*a1j2syy*a1j2syyy+5*a1j2sy*a1j2syyyy)*
     & a1j2ryss
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
      a1j2syxxxxx = t2*a1j2rx*a1j2syrrrrr+5*a1j2sx*t2*a1j2syrrrrs+10*
     & t8*t9*a1j2syrrrss+10*t13*t1*a1j2syrrsss+5*t17*a1j2rx*
     & a1j2syrssss+t17*a1j2sx*a1j2sysssss+10*a1j2rxx*t9*a1j2syrrrr+(
     & 12*a1j2rxx*t1*a1j2sx+a1j2rx*t38+a1j2sxx*t9+a1j2rx*t44)*
     & a1j2syrrrs+(3*a1j2rxx*a1j2rx*t8+a1j2rx*t55+a1j2rx*t59+a1j2sx*
     & t44+3*t29*a1j2sx+a1j2sx*t38)*a1j2syrrss+(a1j2rxx*t13+12*t31*t8+
     & a1j2sx*t55+a1j2sx*t59)*a1j2syrsss+10*a1j2sxx*t13*a1j2syssss+(
     & a1j2rx*t81+7*t79*a1j2rx+a1j2rx*t86+a1j2rx*t88+a1j2rxxx*t1)*
     & a1j2syrrr+t121*a1j2syrrs+t145*a1j2syrss+(a1j2sx*t129+7*t127*
     & a1j2sx+a1j2sx*t136+a1j2sx*t133+a1j2sxxx*t8)*a1j2sysss+(10*
     & a1j2rxx*a1j2rxxx+5*a1j2rx*a1j2rxxxx)*a1j2syrr+(5*a1j2sxxxx*
     & a1j2rx+10*a1j2sxx*a1j2rxxx+5*a1j2sx*a1j2rxxxx+10*a1j2sxxx*
     & a1j2rxx)*a1j2syrs+(5*a1j2sxxxx*a1j2sx+10*a1j2sxx*a1j2sxxx)*
     & a1j2syss
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
      a1j2syxxxxy = a1j2ry*t2*a1j2syrrrrr+(4*a1j2ry*t5*a1j2sx+a1j2sy*
     & t2)*a1j2syrrrrs+(4*a1j2sy*t5*a1j2sx+6*t15*t16)*a1j2syrrrss+(4*
     & a1j2ry*a1j2rx*t22+6*t25*t16)*a1j2syrrsss+(a1j2ry*t30+4*a1j2sy*
     & a1j2rx*t22)*a1j2syrssss+a1j2sy*t30*a1j2sysssss+(4*t5*a1j2rxy+6*
     & t15*a1j2rxx)*a1j2syrrrr+(6*t25*a1j2rxx+a1j2rx*t55+6*t47*t1+
     & a1j2sxy*t5+a1j2ry*t71)*a1j2syrrrs+(3*t50*a1j2sx+a1j2rx*t81+
     & a1j2sy*t71+a1j2ry*t89+3*a1j2rxy*a1j2rx*t16+a1j2sx*t55)*
     & a1j2syrrss+(a1j2sy*t89+6*t51*t16+a1j2sx*t81+a1j2rxy*t22+6*
     & a1j2ry*t16*a1j2sxx)*a1j2syrsss+(4*t22*a1j2sxy+6*a1j2sy*t16*
     & a1j2sxx)*a1j2syssss+(a1j2rxxy*t1+7*t115*a1j2rx+a1j2rx*t120+
     & a1j2rx*t122+a1j2ry*t128)*a1j2syrrr+t162*a1j2syrrs+t190*
     & a1j2syrss+(a1j2sx*t174+a1j2sxxy*t16+7*a1j2sxy*a1j2sx*a1j2sxx+
     & a1j2sx*t179+a1j2sy*t185)*a1j2sysss+(a1j2ry*a1j2rxxxx+4*
     & a1j2rxxx*a1j2rxy+6*a1j2rxxy*a1j2rxx+4*a1j2rxxxy*a1j2rx)*
     & a1j2syrr+(4*a1j2sxxx*a1j2rxy+a1j2ry*a1j2sxxxx+4*a1j2sxxxy*
     & a1j2rx+4*a1j2sxy*a1j2rxxx+4*a1j2sx*a1j2rxxxy+6*a1j2sxx*
     & a1j2rxxy+a1j2sy*a1j2rxxxx+6*a1j2sxxy*a1j2rxx)*a1j2syrs+(4*
     & a1j2sxxx*a1j2sxy+4*a1j2sxxxy*a1j2sx+6*a1j2sxx*a1j2sxxy+a1j2sy*
     & a1j2sxxxx)*a1j2syss
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
      t219 = a1j2sy*t184+a1j2sx*t175+a1j2rxyy*t17+2*a1j2sxy*t61+a1j2ry*
     & t198+3*a1j2ryy*a1j2sxx*a1j2sx+a1j2ry*t206+a1j2rx*t211+2*t208*
     & a1j2rx+a1j2sy*t165+4*t152*a1j2sx+a1j2syy*t71
      a1j2syxxxyy = t1*t3*a1j2syrrrrr+(t6*t3+a1j2ry*t12)*a1j2syrrrrs+(
     & a1j2ry*t22+a1j2sy*t12)*a1j2syrrrss+(a1j2ry*t32+a1j2sy*t22)*
     & a1j2syrrsss+(t6*t30+a1j2sy*t32)*a1j2syrssss+t41*t30*
     & a1j2sysssss+(a1j2ry*t47+3*a1j2ry*a1j2rxy*t2+a1j2ryy*t3)*
     & a1j2syrrrr+(a1j2ry*t63+3*a1j2sy*a1j2rxy*t2+a1j2ry*t75+a1j2syy*
     & t3+a1j2sy*t47+3*a1j2ryy*t2*a1j2sx)*a1j2syrrrs+(a1j2sy*t75+
     & a1j2ry*t89+a1j2ry*t95+a1j2sy*t63+3*a1j2syy*t2*a1j2sx+3*t101*
     & t17)*a1j2syrrss+(3*t106*t17+a1j2sy*t89+3*a1j2ry*t17*a1j2sxy+
     & a1j2sy*t95+a1j2ry*t118+a1j2ryy*t30)*a1j2syrsss+(3*a1j2sy*t17*
     & a1j2sxy+a1j2syy*t30+a1j2sy*t118)*a1j2syssss+(a1j2ry*t133+
     & a1j2rxyy*t2+a1j2ry*t139+4*t141*a1j2rx+a1j2rx*t146+3*t101*
     & a1j2rxx)*a1j2syrrr+t188*a1j2syrrs+t219*a1j2syrss+(4*t209*
     & a1j2sx+a1j2sy*t206+a1j2sx*t211+3*a1j2syy*a1j2sxx*a1j2sx+a1j2sy*
     & t198+a1j2sxyy*t17)*a1j2sysss+(3*a1j2rx*a1j2rxxyy+6*a1j2rxy*
     & a1j2rxxy+3*a1j2rxx*a1j2rxyy+a1j2ryy*a1j2rxxx+2*a1j2ry*
     & a1j2rxxxy)*a1j2syrr+(3*a1j2sxxyy*a1j2rx+6*a1j2sxxy*a1j2rxy+3*
     & a1j2sxyy*a1j2rxx+3*a1j2sx*a1j2rxxyy+3*a1j2sxx*a1j2rxyy+2*
     & a1j2ry*a1j2sxxxy+6*a1j2sxy*a1j2rxxy+a1j2ryy*a1j2sxxx+2*a1j2sy*
     & a1j2rxxxy+a1j2syy*a1j2rxxx)*a1j2syrs+(3*a1j2sxxyy*a1j2sx+3*
     & a1j2sxyy*a1j2sxx+2*a1j2sy*a1j2sxxxy+a1j2syy*a1j2sxxx+6*a1j2sxy*
     & a1j2sxxy)*a1j2syss
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
      a1j2syxxyyy = t1*a1j2ry*t3*a1j2syrrrrr+(a1j2ry*t14+a1j2sy*t1*t3)*
     & a1j2syrrrrs+(a1j2ry*t28+a1j2sy*t14)*a1j2syrrrss+(a1j2sy*t28+
     & a1j2ry*t36)*a1j2syrrsss+(a1j2sy*t36+a1j2ry*t41*t21)*
     & a1j2syrssss+t41*a1j2sy*t21*a1j2sysssss+(a1j2ry*t58+a1j2ryy*
     & a1j2ry*t3+a1j2ry*t62)*a1j2syrrrr+(a1j2sy*t58+a1j2ry*t86+a1j2ry*
     & t88+a1j2syy*a1j2ry*t3+a1j2ryy*t12+a1j2sy*t62)*a1j2syrrrs+(
     & a1j2ry*t104+a1j2syy*t12+a1j2ry*t113+a1j2sy*t88+a1j2sy*t86+
     & a1j2ryy*t26)*a1j2syrrss+(a1j2ry*t124+a1j2sy*t104+a1j2syy*t26+
     & a1j2ryy*a1j2sy*t21+a1j2sy*t113+a1j2ry*t132)*a1j2syrsss+(a1j2sy*
     & t132+a1j2syy*a1j2sy*t21+a1j2sy*t124)*a1j2syssss+(4*a1j2ryy*
     & a1j2rxy*a1j2rx+a1j2ry*t151+a1j2ryyy*t3+a1j2ry*t155+a1j2ryy*t55+
     & a1j2ry*t159)*a1j2syrrr+t195*a1j2syrrs+t225*a1j2syrss+(a1j2sy*
     & t214+a1j2syyy*t21+4*a1j2syy*a1j2sxy*a1j2sx+a1j2syy*t111+a1j2sy*
     & t223+a1j2sy*t205)*a1j2sysss+(a1j2ryyy*a1j2rxx+3*a1j2ry*
     & a1j2rxxyy+6*a1j2rxy*a1j2rxyy+2*a1j2rx*a1j2rxyyy+3*a1j2ryy*
     & a1j2rxxy)*a1j2syrr+(a1j2ryyy*a1j2sxx+3*a1j2ry*a1j2sxxyy+3*
     & a1j2syy*a1j2rxxy+6*a1j2sxyy*a1j2rxy+a1j2syyy*a1j2rxx+2*a1j2sx*
     & a1j2rxyyy+3*a1j2sy*a1j2rxxyy+3*a1j2ryy*a1j2sxxy+6*a1j2sxy*
     & a1j2rxyy+2*a1j2sxyyy*a1j2rx)*a1j2syrs+(3*a1j2syy*a1j2sxxy+2*
     & a1j2sx*a1j2sxyyy+a1j2syyy*a1j2sxx+3*a1j2sy*a1j2sxxyy+6*
     & a1j2sxyy*a1j2sxy)*a1j2syss
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
      t188 = a1j2syyy*a1j2ry*a1j2rx+a1j2ryyy*t14+a1j2ry*t167+2*a1j2ryy*
     & t73+2*a1j2syy*t54+a1j2ryy*t85+a1j2ry*t178+a1j2syy*t61+a1j2sy*
     & t151+a1j2ry*t184+a1j2sy*t146+a1j2sy*t141
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
      a1j2syxyyyy = t2*a1j2rx*a1j2syrrrrr+(a1j2sy*t1*a1j2ry*a1j2rx+
     & a1j2ry*t18)*a1j2syrrrrs+(a1j2ry*t27+a1j2sy*t18)*a1j2syrrrss+(
     & a1j2ry*t36+a1j2sy*t27)*a1j2syrrsss+(a1j2sy*t36+a1j2ry*t33*
     & a1j2sy*a1j2sx)*a1j2syrssss+t47*a1j2sx*a1j2sysssss+(a1j2ryy*t1*
     & a1j2rx+a1j2ry*t58+a1j2ry*t63)*a1j2syrrrr+(a1j2ry*t77+a1j2sy*
     & t58+a1j2syy*t1*a1j2rx+a1j2ry*t87+a1j2sy*t63+a1j2ryy*t16)*
     & a1j2syrrrs+(a1j2syy*t16+a1j2ry*t106+a1j2ry*t108+a1j2sy*t77+
     & a1j2sy*t87+a1j2ryy*t25)*a1j2syrrss+(a1j2syy*t25+a1j2sy*t108+
     & a1j2ry*t120+a1j2ry*t123+a1j2sy*t106+a1j2ryy*t33*a1j2sx)*
     & a1j2syrsss+(a1j2syy*t33*a1j2sx+a1j2sy*t120+a1j2sy*t123)*
     & a1j2syssss+(a1j2ry*t141+2*a1j2ryy*t54+a1j2ry*t146+a1j2ryyy*
     & a1j2ry*a1j2rx+a1j2ry*t151+a1j2ryy*t61)*a1j2syrrr+t188*
     & a1j2syrrs+t215*a1j2syrss+(a1j2syyy*a1j2sy*a1j2sx+2*a1j2syy*
     & t102+a1j2sy*t200+a1j2sy*t203+a1j2syy*t98+a1j2sy*t208)*
     & a1j2sysss+(a1j2ryyyy*a1j2rx+4*a1j2ry*a1j2rxyyy+4*a1j2ryyy*
     & a1j2rxy+6*a1j2ryy*a1j2rxyy)*a1j2syrr+(4*a1j2ryyy*a1j2sxy+
     & a1j2syyyy*a1j2rx+4*a1j2sy*a1j2rxyyy+6*a1j2syy*a1j2rxyy+4*
     & a1j2ry*a1j2sxyyy+4*a1j2syyy*a1j2rxy+6*a1j2ryy*a1j2sxyy+
     & a1j2ryyyy*a1j2sx)*a1j2syrs+(4*a1j2sy*a1j2sxyyy+a1j2syyyy*
     & a1j2sx+4*a1j2syyy*a1j2sxy+6*a1j2syy*a1j2sxyy)*a1j2syss
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
      t121 = 7*t30*a1j2ryy+a1j2sy*t89+a1j2ry*t102+a1j2ryy*t41+a1j2syyy*
     & t1+a1j2ry*t109+a1j2sy*t87+a1j2sy*t81+2*t96*a1j2ry+a1j2ry*t117+
     & 2*a1j2ryy*t32
      t129 = a1j2syyy*a1j2sy
      t131 = a1j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a1j2sy+a1j2sy*t109+2*a1j2syy*t32+a1j2sy*t117+
     & a1j2ry*t133+a1j2ry*t136+a1j2ry*t138+a1j2syy*t41+a1j2ryyy*t8+2*
     & t129*a1j2ry+a1j2sy*t102
      a1j2syyyyyy = t2*a1j2ry*a1j2syrrrrr+5*a1j2sy*t2*a1j2syrrrrs+10*
     & t8*t9*a1j2syrrrss+10*t13*t1*a1j2syrrsss+5*t17*a1j2ry*
     & a1j2syrssss+t17*a1j2sy*a1j2sysssss+10*t9*a1j2ryy*a1j2syrrrr+(
     & 12*t26*t1+a1j2ry*t37+a1j2syy*t9+a1j2ry*t43)*a1j2syrrrs+(3*
     & a1j2ryy*a1j2ry*t8+a1j2sy*t43+a1j2sy*t37+a1j2ry*t56+a1j2ry*t60+
     & 3*t29*a1j2sy)*a1j2syrrss+(12*a1j2ry*t8*a1j2syy+a1j2sy*t56+
     & a1j2sy*t60+a1j2ryy*t13)*a1j2syrsss+10*t13*a1j2syy*a1j2syssss+(
     & a1j2ryyy*t1+a1j2ry*t81+7*t79*a1j2ry+a1j2ry*t87+a1j2ry*t89)*
     & a1j2syrrr+t121*a1j2syrrs+t145*a1j2syrss+(a1j2sy*t138+a1j2sy*
     & t133+7*t131*a1j2sy+a1j2syyy*t8+a1j2sy*t136)*a1j2sysss+(5*
     & a1j2ry*a1j2ryyyy+10*a1j2ryyy*a1j2ryy)*a1j2syrr+(10*a1j2syy*
     & a1j2ryyy+10*a1j2syyy*a1j2ryy+5*a1j2sy*a1j2ryyyy+5*a1j2syyyy*
     & a1j2ry)*a1j2syrs+(10*a1j2syy*a1j2syyy+5*a1j2sy*a1j2syyyy)*
     & a1j2syss

      a2j6rx = rsxy2(j1,j2,j3,0,0)
      a2j6rxr = (-rsxy2(j1-3,j2,j3,0,0)+9.*rsxy2(j1-2,j2,j3,0,0)-45.*
     & rsxy2(j1-1,j2,j3,0,0)+45.*rsxy2(j1+1,j2,j3,0,0)-9.*rsxy2(j1+2,
     & j2,j3,0,0)+rsxy2(j1+3,j2,j3,0,0))/(60.*dr2(0))
      a2j6rxs = (-rsxy2(j1,j2-3,j3,0,0)+9.*rsxy2(j1,j2-2,j3,0,0)-45.*
     & rsxy2(j1,j2-1,j3,0,0)+45.*rsxy2(j1,j2+1,j3,0,0)-9.*rsxy2(j1,j2+
     & 2,j3,0,0)+rsxy2(j1,j2+3,j3,0,0))/(60.*dr2(1))
      a2j6sx = rsxy2(j1,j2,j3,1,0)
      a2j6sxr = (-rsxy2(j1-3,j2,j3,1,0)+9.*rsxy2(j1-2,j2,j3,1,0)-45.*
     & rsxy2(j1-1,j2,j3,1,0)+45.*rsxy2(j1+1,j2,j3,1,0)-9.*rsxy2(j1+2,
     & j2,j3,1,0)+rsxy2(j1+3,j2,j3,1,0))/(60.*dr2(0))
      a2j6sxs = (-rsxy2(j1,j2-3,j3,1,0)+9.*rsxy2(j1,j2-2,j3,1,0)-45.*
     & rsxy2(j1,j2-1,j3,1,0)+45.*rsxy2(j1,j2+1,j3,1,0)-9.*rsxy2(j1,j2+
     & 2,j3,1,0)+rsxy2(j1,j2+3,j3,1,0))/(60.*dr2(1))
      a2j6ry = rsxy2(j1,j2,j3,0,1)
      a2j6ryr = (-rsxy2(j1-3,j2,j3,0,1)+9.*rsxy2(j1-2,j2,j3,0,1)-45.*
     & rsxy2(j1-1,j2,j3,0,1)+45.*rsxy2(j1+1,j2,j3,0,1)-9.*rsxy2(j1+2,
     & j2,j3,0,1)+rsxy2(j1+3,j2,j3,0,1))/(60.*dr2(0))
      a2j6rys = (-rsxy2(j1,j2-3,j3,0,1)+9.*rsxy2(j1,j2-2,j3,0,1)-45.*
     & rsxy2(j1,j2-1,j3,0,1)+45.*rsxy2(j1,j2+1,j3,0,1)-9.*rsxy2(j1,j2+
     & 2,j3,0,1)+rsxy2(j1,j2+3,j3,0,1))/(60.*dr2(1))
      a2j6sy = rsxy2(j1,j2,j3,1,1)
      a2j6syr = (-rsxy2(j1-3,j2,j3,1,1)+9.*rsxy2(j1-2,j2,j3,1,1)-45.*
     & rsxy2(j1-1,j2,j3,1,1)+45.*rsxy2(j1+1,j2,j3,1,1)-9.*rsxy2(j1+2,
     & j2,j3,1,1)+rsxy2(j1+3,j2,j3,1,1))/(60.*dr2(0))
      a2j6sys = (-rsxy2(j1,j2-3,j3,1,1)+9.*rsxy2(j1,j2-2,j3,1,1)-45.*
     & rsxy2(j1,j2-1,j3,1,1)+45.*rsxy2(j1,j2+1,j3,1,1)-9.*rsxy2(j1,j2+
     & 2,j3,1,1)+rsxy2(j1,j2+3,j3,1,1))/(60.*dr2(1))
      a2j6rxx = a2j6rx*a2j6rxr+a2j6sx*a2j6rxs
      a2j6rxy = a2j6ry*a2j6rxr+a2j6sy*a2j6rxs
      a2j6sxx = a2j6rx*a2j6sxr+a2j6sx*a2j6sxs
      a2j6sxy = a2j6ry*a2j6sxr+a2j6sy*a2j6sxs
      a2j6ryx = a2j6rx*a2j6ryr+a2j6sx*a2j6rys
      a2j6ryy = a2j6ry*a2j6ryr+a2j6sy*a2j6rys
      a2j6syx = a2j6rx*a2j6syr+a2j6sx*a2j6sys
      a2j6syy = a2j6ry*a2j6syr+a2j6sy*a2j6sys
      a2j4rx = rsxy2(j1,j2,j3,0,0)
      a2j4rxr = (rsxy2(j1-2,j2,j3,0,0)-8.*rsxy2(j1-1,j2,j3,0,0)+8.*
     & rsxy2(j1+1,j2,j3,0,0)-rsxy2(j1+2,j2,j3,0,0))/(12.*dr2(0))
      a2j4rxs = (rsxy2(j1,j2-2,j3,0,0)-8.*rsxy2(j1,j2-1,j3,0,0)+8.*
     & rsxy2(j1,j2+1,j3,0,0)-rsxy2(j1,j2+2,j3,0,0))/(12.*dr2(1))
      a2j4rxrr = (-rsxy2(j1-2,j2,j3,0,0)+16.*rsxy2(j1-1,j2,j3,0,0)-30.*
     & rsxy2(j1,j2,j3,0,0)+16.*rsxy2(j1+1,j2,j3,0,0)-rsxy2(j1+2,j2,j3,
     & 0,0))/(12.*dr2(0)**2)
      a2j4rxrs = ((rsxy2(j1-2,j2-2,j3,0,0)-8.*rsxy2(j1-2,j2-1,j3,0,0)+
     & 8.*rsxy2(j1-2,j2+1,j3,0,0)-rsxy2(j1-2,j2+2,j3,0,0))/(12.*dr2(1)
     & )-8.*(rsxy2(j1-1,j2-2,j3,0,0)-8.*rsxy2(j1-1,j2-1,j3,0,0)+8.*
     & rsxy2(j1-1,j2+1,j3,0,0)-rsxy2(j1-1,j2+2,j3,0,0))/(12.*dr2(1))+
     & 8.*(rsxy2(j1+1,j2-2,j3,0,0)-8.*rsxy2(j1+1,j2-1,j3,0,0)+8.*
     & rsxy2(j1+1,j2+1,j3,0,0)-rsxy2(j1+1,j2+2,j3,0,0))/(12.*dr2(1))-(
     & rsxy2(j1+2,j2-2,j3,0,0)-8.*rsxy2(j1+2,j2-1,j3,0,0)+8.*rsxy2(j1+
     & 2,j2+1,j3,0,0)-rsxy2(j1+2,j2+2,j3,0,0))/(12.*dr2(1)))/(12.*dr2(
     & 0))
      a2j4rxss = (-rsxy2(j1,j2-2,j3,0,0)+16.*rsxy2(j1,j2-1,j3,0,0)-30.*
     & rsxy2(j1,j2,j3,0,0)+16.*rsxy2(j1,j2+1,j3,0,0)-rsxy2(j1,j2+2,j3,
     & 0,0))/(12.*dr2(1)**2)
      a2j4rxrrr = (rsxy2(j1-3,j2,j3,0,0)-8.*rsxy2(j1-2,j2,j3,0,0)+13.*
     & rsxy2(j1-1,j2,j3,0,0)-13.*rsxy2(j1+1,j2,j3,0,0)+8.*rsxy2(j1+2,
     & j2,j3,0,0)-rsxy2(j1+3,j2,j3,0,0))/(8.*dr2(0)**3)
      a2j4rxrrs = (-(rsxy2(j1-2,j2-2,j3,0,0)-8.*rsxy2(j1-2,j2-1,j3,0,0)
     & +8.*rsxy2(j1-2,j2+1,j3,0,0)-rsxy2(j1-2,j2+2,j3,0,0))/(12.*dr2(
     & 1))+16.*(rsxy2(j1-1,j2-2,j3,0,0)-8.*rsxy2(j1-1,j2-1,j3,0,0)+8.*
     & rsxy2(j1-1,j2+1,j3,0,0)-rsxy2(j1-1,j2+2,j3,0,0))/(12.*dr2(1))-
     & 30.*(rsxy2(j1,j2-2,j3,0,0)-8.*rsxy2(j1,j2-1,j3,0,0)+8.*rsxy2(
     & j1,j2+1,j3,0,0)-rsxy2(j1,j2+2,j3,0,0))/(12.*dr2(1))+16.*(rsxy2(
     & j1+1,j2-2,j3,0,0)-8.*rsxy2(j1+1,j2-1,j3,0,0)+8.*rsxy2(j1+1,j2+
     & 1,j3,0,0)-rsxy2(j1+1,j2+2,j3,0,0))/(12.*dr2(1))-(rsxy2(j1+2,j2-
     & 2,j3,0,0)-8.*rsxy2(j1+2,j2-1,j3,0,0)+8.*rsxy2(j1+2,j2+1,j3,0,0)
     & -rsxy2(j1+2,j2+2,j3,0,0))/(12.*dr2(1)))/(12.*dr2(0)**2)
      a2j4rxrss = ((-rsxy2(j1-2,j2-2,j3,0,0)+16.*rsxy2(j1-2,j2-1,j3,0,
     & 0)-30.*rsxy2(j1-2,j2,j3,0,0)+16.*rsxy2(j1-2,j2+1,j3,0,0)-rsxy2(
     & j1-2,j2+2,j3,0,0))/(12.*dr2(1)**2)-8.*(-rsxy2(j1-1,j2-2,j3,0,0)
     & +16.*rsxy2(j1-1,j2-1,j3,0,0)-30.*rsxy2(j1-1,j2,j3,0,0)+16.*
     & rsxy2(j1-1,j2+1,j3,0,0)-rsxy2(j1-1,j2+2,j3,0,0))/(12.*dr2(1)**
     & 2)+8.*(-rsxy2(j1+1,j2-2,j3,0,0)+16.*rsxy2(j1+1,j2-1,j3,0,0)-
     & 30.*rsxy2(j1+1,j2,j3,0,0)+16.*rsxy2(j1+1,j2+1,j3,0,0)-rsxy2(j1+
     & 1,j2+2,j3,0,0))/(12.*dr2(1)**2)-(-rsxy2(j1+2,j2-2,j3,0,0)+16.*
     & rsxy2(j1+2,j2-1,j3,0,0)-30.*rsxy2(j1+2,j2,j3,0,0)+16.*rsxy2(j1+
     & 2,j2+1,j3,0,0)-rsxy2(j1+2,j2+2,j3,0,0))/(12.*dr2(1)**2))/(12.*
     & dr2(0))
      a2j4rxsss = (rsxy2(j1,j2-3,j3,0,0)-8.*rsxy2(j1,j2-2,j3,0,0)+13.*
     & rsxy2(j1,j2-1,j3,0,0)-13.*rsxy2(j1,j2+1,j3,0,0)+8.*rsxy2(j1,j2+
     & 2,j3,0,0)-rsxy2(j1,j2+3,j3,0,0))/(8.*dr2(1)**3)
      a2j4sx = rsxy2(j1,j2,j3,1,0)
      a2j4sxr = (rsxy2(j1-2,j2,j3,1,0)-8.*rsxy2(j1-1,j2,j3,1,0)+8.*
     & rsxy2(j1+1,j2,j3,1,0)-rsxy2(j1+2,j2,j3,1,0))/(12.*dr2(0))
      a2j4sxs = (rsxy2(j1,j2-2,j3,1,0)-8.*rsxy2(j1,j2-1,j3,1,0)+8.*
     & rsxy2(j1,j2+1,j3,1,0)-rsxy2(j1,j2+2,j3,1,0))/(12.*dr2(1))
      a2j4sxrr = (-rsxy2(j1-2,j2,j3,1,0)+16.*rsxy2(j1-1,j2,j3,1,0)-30.*
     & rsxy2(j1,j2,j3,1,0)+16.*rsxy2(j1+1,j2,j3,1,0)-rsxy2(j1+2,j2,j3,
     & 1,0))/(12.*dr2(0)**2)
      a2j4sxrs = ((rsxy2(j1-2,j2-2,j3,1,0)-8.*rsxy2(j1-2,j2-1,j3,1,0)+
     & 8.*rsxy2(j1-2,j2+1,j3,1,0)-rsxy2(j1-2,j2+2,j3,1,0))/(12.*dr2(1)
     & )-8.*(rsxy2(j1-1,j2-2,j3,1,0)-8.*rsxy2(j1-1,j2-1,j3,1,0)+8.*
     & rsxy2(j1-1,j2+1,j3,1,0)-rsxy2(j1-1,j2+2,j3,1,0))/(12.*dr2(1))+
     & 8.*(rsxy2(j1+1,j2-2,j3,1,0)-8.*rsxy2(j1+1,j2-1,j3,1,0)+8.*
     & rsxy2(j1+1,j2+1,j3,1,0)-rsxy2(j1+1,j2+2,j3,1,0))/(12.*dr2(1))-(
     & rsxy2(j1+2,j2-2,j3,1,0)-8.*rsxy2(j1+2,j2-1,j3,1,0)+8.*rsxy2(j1+
     & 2,j2+1,j3,1,0)-rsxy2(j1+2,j2+2,j3,1,0))/(12.*dr2(1)))/(12.*dr2(
     & 0))
      a2j4sxss = (-rsxy2(j1,j2-2,j3,1,0)+16.*rsxy2(j1,j2-1,j3,1,0)-30.*
     & rsxy2(j1,j2,j3,1,0)+16.*rsxy2(j1,j2+1,j3,1,0)-rsxy2(j1,j2+2,j3,
     & 1,0))/(12.*dr2(1)**2)
      a2j4sxrrr = (rsxy2(j1-3,j2,j3,1,0)-8.*rsxy2(j1-2,j2,j3,1,0)+13.*
     & rsxy2(j1-1,j2,j3,1,0)-13.*rsxy2(j1+1,j2,j3,1,0)+8.*rsxy2(j1+2,
     & j2,j3,1,0)-rsxy2(j1+3,j2,j3,1,0))/(8.*dr2(0)**3)
      a2j4sxrrs = (-(rsxy2(j1-2,j2-2,j3,1,0)-8.*rsxy2(j1-2,j2-1,j3,1,0)
     & +8.*rsxy2(j1-2,j2+1,j3,1,0)-rsxy2(j1-2,j2+2,j3,1,0))/(12.*dr2(
     & 1))+16.*(rsxy2(j1-1,j2-2,j3,1,0)-8.*rsxy2(j1-1,j2-1,j3,1,0)+8.*
     & rsxy2(j1-1,j2+1,j3,1,0)-rsxy2(j1-1,j2+2,j3,1,0))/(12.*dr2(1))-
     & 30.*(rsxy2(j1,j2-2,j3,1,0)-8.*rsxy2(j1,j2-1,j3,1,0)+8.*rsxy2(
     & j1,j2+1,j3,1,0)-rsxy2(j1,j2+2,j3,1,0))/(12.*dr2(1))+16.*(rsxy2(
     & j1+1,j2-2,j3,1,0)-8.*rsxy2(j1+1,j2-1,j3,1,0)+8.*rsxy2(j1+1,j2+
     & 1,j3,1,0)-rsxy2(j1+1,j2+2,j3,1,0))/(12.*dr2(1))-(rsxy2(j1+2,j2-
     & 2,j3,1,0)-8.*rsxy2(j1+2,j2-1,j3,1,0)+8.*rsxy2(j1+2,j2+1,j3,1,0)
     & -rsxy2(j1+2,j2+2,j3,1,0))/(12.*dr2(1)))/(12.*dr2(0)**2)
      a2j4sxrss = ((-rsxy2(j1-2,j2-2,j3,1,0)+16.*rsxy2(j1-2,j2-1,j3,1,
     & 0)-30.*rsxy2(j1-2,j2,j3,1,0)+16.*rsxy2(j1-2,j2+1,j3,1,0)-rsxy2(
     & j1-2,j2+2,j3,1,0))/(12.*dr2(1)**2)-8.*(-rsxy2(j1-1,j2-2,j3,1,0)
     & +16.*rsxy2(j1-1,j2-1,j3,1,0)-30.*rsxy2(j1-1,j2,j3,1,0)+16.*
     & rsxy2(j1-1,j2+1,j3,1,0)-rsxy2(j1-1,j2+2,j3,1,0))/(12.*dr2(1)**
     & 2)+8.*(-rsxy2(j1+1,j2-2,j3,1,0)+16.*rsxy2(j1+1,j2-1,j3,1,0)-
     & 30.*rsxy2(j1+1,j2,j3,1,0)+16.*rsxy2(j1+1,j2+1,j3,1,0)-rsxy2(j1+
     & 1,j2+2,j3,1,0))/(12.*dr2(1)**2)-(-rsxy2(j1+2,j2-2,j3,1,0)+16.*
     & rsxy2(j1+2,j2-1,j3,1,0)-30.*rsxy2(j1+2,j2,j3,1,0)+16.*rsxy2(j1+
     & 2,j2+1,j3,1,0)-rsxy2(j1+2,j2+2,j3,1,0))/(12.*dr2(1)**2))/(12.*
     & dr2(0))
      a2j4sxsss = (rsxy2(j1,j2-3,j3,1,0)-8.*rsxy2(j1,j2-2,j3,1,0)+13.*
     & rsxy2(j1,j2-1,j3,1,0)-13.*rsxy2(j1,j2+1,j3,1,0)+8.*rsxy2(j1,j2+
     & 2,j3,1,0)-rsxy2(j1,j2+3,j3,1,0))/(8.*dr2(1)**3)
      a2j4ry = rsxy2(j1,j2,j3,0,1)
      a2j4ryr = (rsxy2(j1-2,j2,j3,0,1)-8.*rsxy2(j1-1,j2,j3,0,1)+8.*
     & rsxy2(j1+1,j2,j3,0,1)-rsxy2(j1+2,j2,j3,0,1))/(12.*dr2(0))
      a2j4rys = (rsxy2(j1,j2-2,j3,0,1)-8.*rsxy2(j1,j2-1,j3,0,1)+8.*
     & rsxy2(j1,j2+1,j3,0,1)-rsxy2(j1,j2+2,j3,0,1))/(12.*dr2(1))
      a2j4ryrr = (-rsxy2(j1-2,j2,j3,0,1)+16.*rsxy2(j1-1,j2,j3,0,1)-30.*
     & rsxy2(j1,j2,j3,0,1)+16.*rsxy2(j1+1,j2,j3,0,1)-rsxy2(j1+2,j2,j3,
     & 0,1))/(12.*dr2(0)**2)
      a2j4ryrs = ((rsxy2(j1-2,j2-2,j3,0,1)-8.*rsxy2(j1-2,j2-1,j3,0,1)+
     & 8.*rsxy2(j1-2,j2+1,j3,0,1)-rsxy2(j1-2,j2+2,j3,0,1))/(12.*dr2(1)
     & )-8.*(rsxy2(j1-1,j2-2,j3,0,1)-8.*rsxy2(j1-1,j2-1,j3,0,1)+8.*
     & rsxy2(j1-1,j2+1,j3,0,1)-rsxy2(j1-1,j2+2,j3,0,1))/(12.*dr2(1))+
     & 8.*(rsxy2(j1+1,j2-2,j3,0,1)-8.*rsxy2(j1+1,j2-1,j3,0,1)+8.*
     & rsxy2(j1+1,j2+1,j3,0,1)-rsxy2(j1+1,j2+2,j3,0,1))/(12.*dr2(1))-(
     & rsxy2(j1+2,j2-2,j3,0,1)-8.*rsxy2(j1+2,j2-1,j3,0,1)+8.*rsxy2(j1+
     & 2,j2+1,j3,0,1)-rsxy2(j1+2,j2+2,j3,0,1))/(12.*dr2(1)))/(12.*dr2(
     & 0))
      a2j4ryss = (-rsxy2(j1,j2-2,j3,0,1)+16.*rsxy2(j1,j2-1,j3,0,1)-30.*
     & rsxy2(j1,j2,j3,0,1)+16.*rsxy2(j1,j2+1,j3,0,1)-rsxy2(j1,j2+2,j3,
     & 0,1))/(12.*dr2(1)**2)
      a2j4ryrrr = (rsxy2(j1-3,j2,j3,0,1)-8.*rsxy2(j1-2,j2,j3,0,1)+13.*
     & rsxy2(j1-1,j2,j3,0,1)-13.*rsxy2(j1+1,j2,j3,0,1)+8.*rsxy2(j1+2,
     & j2,j3,0,1)-rsxy2(j1+3,j2,j3,0,1))/(8.*dr2(0)**3)
      a2j4ryrrs = (-(rsxy2(j1-2,j2-2,j3,0,1)-8.*rsxy2(j1-2,j2-1,j3,0,1)
     & +8.*rsxy2(j1-2,j2+1,j3,0,1)-rsxy2(j1-2,j2+2,j3,0,1))/(12.*dr2(
     & 1))+16.*(rsxy2(j1-1,j2-2,j3,0,1)-8.*rsxy2(j1-1,j2-1,j3,0,1)+8.*
     & rsxy2(j1-1,j2+1,j3,0,1)-rsxy2(j1-1,j2+2,j3,0,1))/(12.*dr2(1))-
     & 30.*(rsxy2(j1,j2-2,j3,0,1)-8.*rsxy2(j1,j2-1,j3,0,1)+8.*rsxy2(
     & j1,j2+1,j3,0,1)-rsxy2(j1,j2+2,j3,0,1))/(12.*dr2(1))+16.*(rsxy2(
     & j1+1,j2-2,j3,0,1)-8.*rsxy2(j1+1,j2-1,j3,0,1)+8.*rsxy2(j1+1,j2+
     & 1,j3,0,1)-rsxy2(j1+1,j2+2,j3,0,1))/(12.*dr2(1))-(rsxy2(j1+2,j2-
     & 2,j3,0,1)-8.*rsxy2(j1+2,j2-1,j3,0,1)+8.*rsxy2(j1+2,j2+1,j3,0,1)
     & -rsxy2(j1+2,j2+2,j3,0,1))/(12.*dr2(1)))/(12.*dr2(0)**2)
      a2j4ryrss = ((-rsxy2(j1-2,j2-2,j3,0,1)+16.*rsxy2(j1-2,j2-1,j3,0,
     & 1)-30.*rsxy2(j1-2,j2,j3,0,1)+16.*rsxy2(j1-2,j2+1,j3,0,1)-rsxy2(
     & j1-2,j2+2,j3,0,1))/(12.*dr2(1)**2)-8.*(-rsxy2(j1-1,j2-2,j3,0,1)
     & +16.*rsxy2(j1-1,j2-1,j3,0,1)-30.*rsxy2(j1-1,j2,j3,0,1)+16.*
     & rsxy2(j1-1,j2+1,j3,0,1)-rsxy2(j1-1,j2+2,j3,0,1))/(12.*dr2(1)**
     & 2)+8.*(-rsxy2(j1+1,j2-2,j3,0,1)+16.*rsxy2(j1+1,j2-1,j3,0,1)-
     & 30.*rsxy2(j1+1,j2,j3,0,1)+16.*rsxy2(j1+1,j2+1,j3,0,1)-rsxy2(j1+
     & 1,j2+2,j3,0,1))/(12.*dr2(1)**2)-(-rsxy2(j1+2,j2-2,j3,0,1)+16.*
     & rsxy2(j1+2,j2-1,j3,0,1)-30.*rsxy2(j1+2,j2,j3,0,1)+16.*rsxy2(j1+
     & 2,j2+1,j3,0,1)-rsxy2(j1+2,j2+2,j3,0,1))/(12.*dr2(1)**2))/(12.*
     & dr2(0))
      a2j4rysss = (rsxy2(j1,j2-3,j3,0,1)-8.*rsxy2(j1,j2-2,j3,0,1)+13.*
     & rsxy2(j1,j2-1,j3,0,1)-13.*rsxy2(j1,j2+1,j3,0,1)+8.*rsxy2(j1,j2+
     & 2,j3,0,1)-rsxy2(j1,j2+3,j3,0,1))/(8.*dr2(1)**3)
      a2j4sy = rsxy2(j1,j2,j3,1,1)
      a2j4syr = (rsxy2(j1-2,j2,j3,1,1)-8.*rsxy2(j1-1,j2,j3,1,1)+8.*
     & rsxy2(j1+1,j2,j3,1,1)-rsxy2(j1+2,j2,j3,1,1))/(12.*dr2(0))
      a2j4sys = (rsxy2(j1,j2-2,j3,1,1)-8.*rsxy2(j1,j2-1,j3,1,1)+8.*
     & rsxy2(j1,j2+1,j3,1,1)-rsxy2(j1,j2+2,j3,1,1))/(12.*dr2(1))
      a2j4syrr = (-rsxy2(j1-2,j2,j3,1,1)+16.*rsxy2(j1-1,j2,j3,1,1)-30.*
     & rsxy2(j1,j2,j3,1,1)+16.*rsxy2(j1+1,j2,j3,1,1)-rsxy2(j1+2,j2,j3,
     & 1,1))/(12.*dr2(0)**2)
      a2j4syrs = ((rsxy2(j1-2,j2-2,j3,1,1)-8.*rsxy2(j1-2,j2-1,j3,1,1)+
     & 8.*rsxy2(j1-2,j2+1,j3,1,1)-rsxy2(j1-2,j2+2,j3,1,1))/(12.*dr2(1)
     & )-8.*(rsxy2(j1-1,j2-2,j3,1,1)-8.*rsxy2(j1-1,j2-1,j3,1,1)+8.*
     & rsxy2(j1-1,j2+1,j3,1,1)-rsxy2(j1-1,j2+2,j3,1,1))/(12.*dr2(1))+
     & 8.*(rsxy2(j1+1,j2-2,j3,1,1)-8.*rsxy2(j1+1,j2-1,j3,1,1)+8.*
     & rsxy2(j1+1,j2+1,j3,1,1)-rsxy2(j1+1,j2+2,j3,1,1))/(12.*dr2(1))-(
     & rsxy2(j1+2,j2-2,j3,1,1)-8.*rsxy2(j1+2,j2-1,j3,1,1)+8.*rsxy2(j1+
     & 2,j2+1,j3,1,1)-rsxy2(j1+2,j2+2,j3,1,1))/(12.*dr2(1)))/(12.*dr2(
     & 0))
      a2j4syss = (-rsxy2(j1,j2-2,j3,1,1)+16.*rsxy2(j1,j2-1,j3,1,1)-30.*
     & rsxy2(j1,j2,j3,1,1)+16.*rsxy2(j1,j2+1,j3,1,1)-rsxy2(j1,j2+2,j3,
     & 1,1))/(12.*dr2(1)**2)
      a2j4syrrr = (rsxy2(j1-3,j2,j3,1,1)-8.*rsxy2(j1-2,j2,j3,1,1)+13.*
     & rsxy2(j1-1,j2,j3,1,1)-13.*rsxy2(j1+1,j2,j3,1,1)+8.*rsxy2(j1+2,
     & j2,j3,1,1)-rsxy2(j1+3,j2,j3,1,1))/(8.*dr2(0)**3)
      a2j4syrrs = (-(rsxy2(j1-2,j2-2,j3,1,1)-8.*rsxy2(j1-2,j2-1,j3,1,1)
     & +8.*rsxy2(j1-2,j2+1,j3,1,1)-rsxy2(j1-2,j2+2,j3,1,1))/(12.*dr2(
     & 1))+16.*(rsxy2(j1-1,j2-2,j3,1,1)-8.*rsxy2(j1-1,j2-1,j3,1,1)+8.*
     & rsxy2(j1-1,j2+1,j3,1,1)-rsxy2(j1-1,j2+2,j3,1,1))/(12.*dr2(1))-
     & 30.*(rsxy2(j1,j2-2,j3,1,1)-8.*rsxy2(j1,j2-1,j3,1,1)+8.*rsxy2(
     & j1,j2+1,j3,1,1)-rsxy2(j1,j2+2,j3,1,1))/(12.*dr2(1))+16.*(rsxy2(
     & j1+1,j2-2,j3,1,1)-8.*rsxy2(j1+1,j2-1,j3,1,1)+8.*rsxy2(j1+1,j2+
     & 1,j3,1,1)-rsxy2(j1+1,j2+2,j3,1,1))/(12.*dr2(1))-(rsxy2(j1+2,j2-
     & 2,j3,1,1)-8.*rsxy2(j1+2,j2-1,j3,1,1)+8.*rsxy2(j1+2,j2+1,j3,1,1)
     & -rsxy2(j1+2,j2+2,j3,1,1))/(12.*dr2(1)))/(12.*dr2(0)**2)
      a2j4syrss = ((-rsxy2(j1-2,j2-2,j3,1,1)+16.*rsxy2(j1-2,j2-1,j3,1,
     & 1)-30.*rsxy2(j1-2,j2,j3,1,1)+16.*rsxy2(j1-2,j2+1,j3,1,1)-rsxy2(
     & j1-2,j2+2,j3,1,1))/(12.*dr2(1)**2)-8.*(-rsxy2(j1-1,j2-2,j3,1,1)
     & +16.*rsxy2(j1-1,j2-1,j3,1,1)-30.*rsxy2(j1-1,j2,j3,1,1)+16.*
     & rsxy2(j1-1,j2+1,j3,1,1)-rsxy2(j1-1,j2+2,j3,1,1))/(12.*dr2(1)**
     & 2)+8.*(-rsxy2(j1+1,j2-2,j3,1,1)+16.*rsxy2(j1+1,j2-1,j3,1,1)-
     & 30.*rsxy2(j1+1,j2,j3,1,1)+16.*rsxy2(j1+1,j2+1,j3,1,1)-rsxy2(j1+
     & 1,j2+2,j3,1,1))/(12.*dr2(1)**2)-(-rsxy2(j1+2,j2-2,j3,1,1)+16.*
     & rsxy2(j1+2,j2-1,j3,1,1)-30.*rsxy2(j1+2,j2,j3,1,1)+16.*rsxy2(j1+
     & 2,j2+1,j3,1,1)-rsxy2(j1+2,j2+2,j3,1,1))/(12.*dr2(1)**2))/(12.*
     & dr2(0))
      a2j4sysss = (rsxy2(j1,j2-3,j3,1,1)-8.*rsxy2(j1,j2-2,j3,1,1)+13.*
     & rsxy2(j1,j2-1,j3,1,1)-13.*rsxy2(j1,j2+1,j3,1,1)+8.*rsxy2(j1,j2+
     & 2,j3,1,1)-rsxy2(j1,j2+3,j3,1,1))/(8.*dr2(1)**3)
      a2j4rxx = a2j4rx*a2j4rxr+a2j4sx*a2j4rxs
      a2j4rxy = a2j4ry*a2j4rxr+a2j4sy*a2j4rxs
      a2j4sxx = a2j4rx*a2j4sxr+a2j4sx*a2j4sxs
      a2j4sxy = a2j4ry*a2j4sxr+a2j4sy*a2j4sxs
      a2j4ryx = a2j4rx*a2j4ryr+a2j4sx*a2j4rys
      a2j4ryy = a2j4ry*a2j4ryr+a2j4sy*a2j4rys
      a2j4syx = a2j4rx*a2j4syr+a2j4sx*a2j4sys
      a2j4syy = a2j4ry*a2j4syr+a2j4sy*a2j4sys
      t1 = a2j4rx**2
      t6 = a2j4sx**2
      a2j4rxxx = t1*a2j4rxrr+2*a2j4rx*a2j4sx*a2j4rxrs+t6*a2j4rxss+
     & a2j4rxx*a2j4rxr+a2j4sxx*a2j4rxs
      a2j4rxxy = a2j4ry*a2j4rx*a2j4rxrr+(a2j4sy*a2j4rx+a2j4ry*a2j4sx)*
     & a2j4rxrs+a2j4sy*a2j4sx*a2j4rxss+a2j4rxy*a2j4rxr+a2j4sxy*a2j4rxs
      t1 = a2j4ry**2
      t6 = a2j4sy**2
      a2j4rxyy = t1*a2j4rxrr+2*a2j4ry*a2j4sy*a2j4rxrs+t6*a2j4rxss+
     & a2j4ryy*a2j4rxr+a2j4syy*a2j4rxs
      t1 = a2j4rx**2
      t6 = a2j4sx**2
      a2j4sxxx = t1*a2j4sxrr+2*a2j4rx*a2j4sx*a2j4sxrs+t6*a2j4sxss+
     & a2j4rxx*a2j4sxr+a2j4sxx*a2j4sxs
      a2j4sxxy = a2j4ry*a2j4rx*a2j4sxrr+(a2j4sy*a2j4rx+a2j4ry*a2j4sx)*
     & a2j4sxrs+a2j4sy*a2j4sx*a2j4sxss+a2j4rxy*a2j4sxr+a2j4sxy*a2j4sxs
      t1 = a2j4ry**2
      t6 = a2j4sy**2
      a2j4sxyy = t1*a2j4sxrr+2*a2j4ry*a2j4sy*a2j4sxrs+t6*a2j4sxss+
     & a2j4ryy*a2j4sxr+a2j4syy*a2j4sxs
      t1 = a2j4rx**2
      t6 = a2j4sx**2
      a2j4ryxx = t1*a2j4ryrr+2*a2j4rx*a2j4sx*a2j4ryrs+t6*a2j4ryss+
     & a2j4rxx*a2j4ryr+a2j4sxx*a2j4rys
      a2j4ryxy = a2j4ry*a2j4rx*a2j4ryrr+(a2j4sy*a2j4rx+a2j4ry*a2j4sx)*
     & a2j4ryrs+a2j4sy*a2j4sx*a2j4ryss+a2j4rxy*a2j4ryr+a2j4sxy*a2j4rys
      t1 = a2j4ry**2
      t6 = a2j4sy**2
      a2j4ryyy = t1*a2j4ryrr+2*a2j4ry*a2j4sy*a2j4ryrs+t6*a2j4ryss+
     & a2j4ryy*a2j4ryr+a2j4syy*a2j4rys
      t1 = a2j4rx**2
      t6 = a2j4sx**2
      a2j4syxx = t1*a2j4syrr+2*a2j4rx*a2j4sx*a2j4syrs+t6*a2j4syss+
     & a2j4rxx*a2j4syr+a2j4sxx*a2j4sys
      a2j4syxy = a2j4ry*a2j4rx*a2j4syrr+(a2j4sy*a2j4rx+a2j4ry*a2j4sx)*
     & a2j4syrs+a2j4sy*a2j4sx*a2j4syss+a2j4rxy*a2j4syr+a2j4sxy*a2j4sys
      t1 = a2j4ry**2
      t6 = a2j4sy**2
      a2j4syyy = t1*a2j4syrr+2*a2j4ry*a2j4sy*a2j4syrs+t6*a2j4syss+
     & a2j4ryy*a2j4syr+a2j4syy*a2j4sys
      t1 = a2j4rx**2
      t7 = a2j4sx**2
      a2j4rxxxx = t1*a2j4rx*a2j4rxrrr+3*t1*a2j4sx*a2j4rxrrs+3*a2j4rx*
     & t7*a2j4rxrss+t7*a2j4sx*a2j4rxsss+3*a2j4rx*a2j4rxx*a2j4rxrr+(3*
     & a2j4sxx*a2j4rx+3*a2j4sx*a2j4rxx)*a2j4rxrs+3*a2j4sxx*a2j4sx*
     & a2j4rxss+a2j4rxxx*a2j4rxr+a2j4sxxx*a2j4rxs
      t1 = a2j4rx**2
      t10 = a2j4sx**2
      a2j4rxxxy = a2j4ry*t1*a2j4rxrrr+(a2j4sy*t1+2*a2j4ry*a2j4sx*
     & a2j4rx)*a2j4rxrrs+(a2j4ry*t10+2*a2j4sy*a2j4sx*a2j4rx)*
     & a2j4rxrss+a2j4sy*t10*a2j4rxsss+(2*a2j4rxy*a2j4rx+a2j4ry*
     & a2j4rxx)*a2j4rxrr+(a2j4ry*a2j4sxx+2*a2j4sx*a2j4rxy+2*a2j4sxy*
     & a2j4rx+a2j4sy*a2j4rxx)*a2j4rxrs+(a2j4sy*a2j4sxx+2*a2j4sxy*
     & a2j4sx)*a2j4rxss+a2j4rxxy*a2j4rxr+a2j4sxxy*a2j4rxs
      t1 = a2j4ry**2
      t4 = a2j4sy*a2j4ry
      t8 = a2j4sy*a2j4rx+a2j4ry*a2j4sx
      t16 = a2j4sy**2
      a2j4rxxyy = t1*a2j4rx*a2j4rxrrr+(t4*a2j4rx+a2j4ry*t8)*a2j4rxrrs+(
     & t4*a2j4sx+a2j4sy*t8)*a2j4rxrss+t16*a2j4sx*a2j4rxsss+(a2j4ryy*
     & a2j4rx+2*a2j4ry*a2j4rxy)*a2j4rxrr+(2*a2j4ry*a2j4sxy+2*a2j4sy*
     & a2j4rxy+a2j4ryy*a2j4sx+a2j4syy*a2j4rx)*a2j4rxrs+(a2j4syy*
     & a2j4sx+2*a2j4sy*a2j4sxy)*a2j4rxss+a2j4rxyy*a2j4rxr+a2j4sxyy*
     & a2j4rxs
      t1 = a2j4ry**2
      t7 = a2j4sy**2
      a2j4rxyyy = a2j4ry*t1*a2j4rxrrr+3*t1*a2j4sy*a2j4rxrrs+3*a2j4ry*
     & t7*a2j4rxrss+t7*a2j4sy*a2j4rxsss+3*a2j4ry*a2j4ryy*a2j4rxrr+(3*
     & a2j4syy*a2j4ry+3*a2j4sy*a2j4ryy)*a2j4rxrs+3*a2j4syy*a2j4sy*
     & a2j4rxss+a2j4ryyy*a2j4rxr+a2j4syyy*a2j4rxs
      t1 = a2j4rx**2
      t7 = a2j4sx**2
      a2j4sxxxx = t1*a2j4rx*a2j4sxrrr+3*t1*a2j4sx*a2j4sxrrs+3*a2j4rx*
     & t7*a2j4sxrss+t7*a2j4sx*a2j4sxsss+3*a2j4rx*a2j4rxx*a2j4sxrr+(3*
     & a2j4sxx*a2j4rx+3*a2j4sx*a2j4rxx)*a2j4sxrs+3*a2j4sxx*a2j4sx*
     & a2j4sxss+a2j4rxxx*a2j4sxr+a2j4sxxx*a2j4sxs
      t1 = a2j4rx**2
      t10 = a2j4sx**2
      a2j4sxxxy = a2j4ry*t1*a2j4sxrrr+(a2j4sy*t1+2*a2j4ry*a2j4sx*
     & a2j4rx)*a2j4sxrrs+(a2j4ry*t10+2*a2j4sy*a2j4sx*a2j4rx)*
     & a2j4sxrss+a2j4sy*t10*a2j4sxsss+(2*a2j4rxy*a2j4rx+a2j4ry*
     & a2j4rxx)*a2j4sxrr+(a2j4ry*a2j4sxx+2*a2j4sx*a2j4rxy+2*a2j4sxy*
     & a2j4rx+a2j4sy*a2j4rxx)*a2j4sxrs+(a2j4sy*a2j4sxx+2*a2j4sxy*
     & a2j4sx)*a2j4sxss+a2j4rxxy*a2j4sxr+a2j4sxxy*a2j4sxs
      t1 = a2j4ry**2
      t4 = a2j4sy*a2j4ry
      t8 = a2j4sy*a2j4rx+a2j4ry*a2j4sx
      t16 = a2j4sy**2
      a2j4sxxyy = t1*a2j4rx*a2j4sxrrr+(t4*a2j4rx+a2j4ry*t8)*a2j4sxrrs+(
     & t4*a2j4sx+a2j4sy*t8)*a2j4sxrss+t16*a2j4sx*a2j4sxsss+(a2j4ryy*
     & a2j4rx+2*a2j4ry*a2j4rxy)*a2j4sxrr+(2*a2j4ry*a2j4sxy+2*a2j4sy*
     & a2j4rxy+a2j4ryy*a2j4sx+a2j4syy*a2j4rx)*a2j4sxrs+(a2j4syy*
     & a2j4sx+2*a2j4sy*a2j4sxy)*a2j4sxss+a2j4rxyy*a2j4sxr+a2j4sxyy*
     & a2j4sxs
      t1 = a2j4ry**2
      t7 = a2j4sy**2
      a2j4sxyyy = a2j4ry*t1*a2j4sxrrr+3*t1*a2j4sy*a2j4sxrrs+3*a2j4ry*
     & t7*a2j4sxrss+t7*a2j4sy*a2j4sxsss+3*a2j4ry*a2j4ryy*a2j4sxrr+(3*
     & a2j4syy*a2j4ry+3*a2j4sy*a2j4ryy)*a2j4sxrs+3*a2j4syy*a2j4sy*
     & a2j4sxss+a2j4ryyy*a2j4sxr+a2j4syyy*a2j4sxs
      t1 = a2j4rx**2
      t7 = a2j4sx**2
      a2j4ryxxx = t1*a2j4rx*a2j4ryrrr+3*t1*a2j4sx*a2j4ryrrs+3*a2j4rx*
     & t7*a2j4ryrss+t7*a2j4sx*a2j4rysss+3*a2j4rx*a2j4rxx*a2j4ryrr+(3*
     & a2j4sxx*a2j4rx+3*a2j4sx*a2j4rxx)*a2j4ryrs+3*a2j4sxx*a2j4sx*
     & a2j4ryss+a2j4rxxx*a2j4ryr+a2j4sxxx*a2j4rys
      t1 = a2j4rx**2
      t10 = a2j4sx**2
      a2j4ryxxy = a2j4ry*t1*a2j4ryrrr+(a2j4sy*t1+2*a2j4ry*a2j4sx*
     & a2j4rx)*a2j4ryrrs+(a2j4ry*t10+2*a2j4sy*a2j4sx*a2j4rx)*
     & a2j4ryrss+a2j4sy*t10*a2j4rysss+(2*a2j4rxy*a2j4rx+a2j4ry*
     & a2j4rxx)*a2j4ryrr+(a2j4ry*a2j4sxx+2*a2j4sx*a2j4rxy+2*a2j4sxy*
     & a2j4rx+a2j4sy*a2j4rxx)*a2j4ryrs+(a2j4sy*a2j4sxx+2*a2j4sxy*
     & a2j4sx)*a2j4ryss+a2j4rxxy*a2j4ryr+a2j4sxxy*a2j4rys
      t1 = a2j4ry**2
      t4 = a2j4sy*a2j4ry
      t8 = a2j4sy*a2j4rx+a2j4ry*a2j4sx
      t16 = a2j4sy**2
      a2j4ryxyy = t1*a2j4rx*a2j4ryrrr+(t4*a2j4rx+a2j4ry*t8)*a2j4ryrrs+(
     & t4*a2j4sx+a2j4sy*t8)*a2j4ryrss+t16*a2j4sx*a2j4rysss+(a2j4ryy*
     & a2j4rx+2*a2j4ry*a2j4rxy)*a2j4ryrr+(2*a2j4ry*a2j4sxy+2*a2j4sy*
     & a2j4rxy+a2j4ryy*a2j4sx+a2j4syy*a2j4rx)*a2j4ryrs+(a2j4syy*
     & a2j4sx+2*a2j4sy*a2j4sxy)*a2j4ryss+a2j4rxyy*a2j4ryr+a2j4sxyy*
     & a2j4rys
      t1 = a2j4ry**2
      t7 = a2j4sy**2
      a2j4ryyyy = a2j4ry*t1*a2j4ryrrr+3*t1*a2j4sy*a2j4ryrrs+3*a2j4ry*
     & t7*a2j4ryrss+t7*a2j4sy*a2j4rysss+3*a2j4ry*a2j4ryy*a2j4ryrr+(3*
     & a2j4syy*a2j4ry+3*a2j4sy*a2j4ryy)*a2j4ryrs+3*a2j4syy*a2j4sy*
     & a2j4ryss+a2j4ryyy*a2j4ryr+a2j4syyy*a2j4rys
      t1 = a2j4rx**2
      t7 = a2j4sx**2
      a2j4syxxx = t1*a2j4rx*a2j4syrrr+3*t1*a2j4sx*a2j4syrrs+3*a2j4rx*
     & t7*a2j4syrss+t7*a2j4sx*a2j4sysss+3*a2j4rx*a2j4rxx*a2j4syrr+(3*
     & a2j4sxx*a2j4rx+3*a2j4sx*a2j4rxx)*a2j4syrs+3*a2j4sxx*a2j4sx*
     & a2j4syss+a2j4rxxx*a2j4syr+a2j4sxxx*a2j4sys
      t1 = a2j4rx**2
      t10 = a2j4sx**2
      a2j4syxxy = a2j4ry*t1*a2j4syrrr+(a2j4sy*t1+2*a2j4ry*a2j4sx*
     & a2j4rx)*a2j4syrrs+(a2j4ry*t10+2*a2j4sy*a2j4sx*a2j4rx)*
     & a2j4syrss+a2j4sy*t10*a2j4sysss+(2*a2j4rxy*a2j4rx+a2j4ry*
     & a2j4rxx)*a2j4syrr+(a2j4ry*a2j4sxx+2*a2j4sx*a2j4rxy+2*a2j4sxy*
     & a2j4rx+a2j4sy*a2j4rxx)*a2j4syrs+(a2j4sy*a2j4sxx+2*a2j4sxy*
     & a2j4sx)*a2j4syss+a2j4rxxy*a2j4syr+a2j4sxxy*a2j4sys
      t1 = a2j4ry**2
      t4 = a2j4sy*a2j4ry
      t8 = a2j4sy*a2j4rx+a2j4ry*a2j4sx
      t16 = a2j4sy**2
      a2j4syxyy = t1*a2j4rx*a2j4syrrr+(t4*a2j4rx+a2j4ry*t8)*a2j4syrrs+(
     & t4*a2j4sx+a2j4sy*t8)*a2j4syrss+t16*a2j4sx*a2j4sysss+(a2j4ryy*
     & a2j4rx+2*a2j4ry*a2j4rxy)*a2j4syrr+(2*a2j4ry*a2j4sxy+2*a2j4sy*
     & a2j4rxy+a2j4ryy*a2j4sx+a2j4syy*a2j4rx)*a2j4syrs+(a2j4syy*
     & a2j4sx+2*a2j4sy*a2j4sxy)*a2j4syss+a2j4rxyy*a2j4syr+a2j4sxyy*
     & a2j4sys
      t1 = a2j4ry**2
      t7 = a2j4sy**2
      a2j4syyyy = a2j4ry*t1*a2j4syrrr+3*t1*a2j4sy*a2j4syrrs+3*a2j4ry*
     & t7*a2j4syrss+t7*a2j4sy*a2j4sysss+3*a2j4ry*a2j4ryy*a2j4syrr+(3*
     & a2j4syy*a2j4ry+3*a2j4sy*a2j4ryy)*a2j4syrs+3*a2j4syy*a2j4sy*
     & a2j4syss+a2j4ryyy*a2j4syr+a2j4syyy*a2j4sys
      a2j2rx = rsxy2(j1,j2,j3,0,0)
      a2j2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(2.*dr2(
     & 0))
      a2j2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(2.*dr2(
     & 1))
      a2j2rxrr = (rsxy2(j1-1,j2,j3,0,0)-2.*rsxy2(j1,j2,j3,0,0)+rsxy2(
     & j1+1,j2,j3,0,0))/(dr2(0)**2)
      a2j2rxrs = (-(-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,0,0))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,j3,0,0))/(
     & 2.*dr2(1)))/(2.*dr2(0))
      a2j2rxss = (rsxy2(j1,j2-1,j3,0,0)-2.*rsxy2(j1,j2,j3,0,0)+rsxy2(
     & j1,j2+1,j3,0,0))/(dr2(1)**2)
      a2j2rxrrr = (-rsxy2(j1-2,j2,j3,0,0)+2.*rsxy2(j1-1,j2,j3,0,0)-2.*
     & rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+2,j2,j3,0,0))/(2.*dr2(0)**3)
      a2j2rxrrs = ((-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,0,0))/(
     & 2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,j3,0,0))/(
     & 2.*dr2(1)))/(dr2(0)**2)
      a2j2rxrss = (-(rsxy2(j1-1,j2-1,j3,0,0)-2.*rsxy2(j1-1,j2,j3,0,0)+
     & rsxy2(j1-1,j2+1,j3,0,0))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,0,0)-
     & 2.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+1,j2+1,j3,0,0))/(dr2(1)**2))
     & /(2.*dr2(0))
      a2j2rxsss = (-rsxy2(j1,j2-2,j3,0,0)+2.*rsxy2(j1,j2-1,j3,0,0)-2.*
     & rsxy2(j1,j2+1,j3,0,0)+rsxy2(j1,j2+2,j3,0,0))/(2.*dr2(1)**3)
      a2j2rxrrrr = (rsxy2(j1-2,j2,j3,0,0)-4.*rsxy2(j1-1,j2,j3,0,0)+6.*
     & rsxy2(j1,j2,j3,0,0)-4.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+2,j2,j3,
     & 0,0))/(dr2(0)**4)
      a2j2rxrrrs = (-(-rsxy2(j1-2,j2-1,j3,0,0)+rsxy2(j1-2,j2+1,j3,0,0))
     & /(2.*dr2(1))+2.*(-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,0,
     & 0))/(2.*dr2(1))-2.*(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,
     & j3,0,0))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,0,0)+rsxy2(j1+2,j2+1,
     & j3,0,0))/(2.*dr2(1)))/(2.*dr2(0)**3)
      a2j2rxrrss = ((rsxy2(j1-1,j2-1,j3,0,0)-2.*rsxy2(j1-1,j2,j3,0,0)+
     & rsxy2(j1-1,j2+1,j3,0,0))/(dr2(1)**2)-2.*(rsxy2(j1,j2-1,j3,0,0)-
     & 2.*rsxy2(j1,j2,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(dr2(1)**2)+(
     & rsxy2(j1+1,j2-1,j3,0,0)-2.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+1,j2+
     & 1,j3,0,0))/(dr2(1)**2))/(dr2(0)**2)
      a2j2rxrsss = (-(-rsxy2(j1-1,j2-2,j3,0,0)+2.*rsxy2(j1-1,j2-1,j3,0,
     & 0)-2.*rsxy2(j1-1,j2+1,j3,0,0)+rsxy2(j1-1,j2+2,j3,0,0))/(2.*dr2(
     & 1)**3)+(-rsxy2(j1+1,j2-2,j3,0,0)+2.*rsxy2(j1+1,j2-1,j3,0,0)-2.*
     & rsxy2(j1+1,j2+1,j3,0,0)+rsxy2(j1+1,j2+2,j3,0,0))/(2.*dr2(1)**3)
     & )/(2.*dr2(0))
      a2j2rxssss = (rsxy2(j1,j2-2,j3,0,0)-4.*rsxy2(j1,j2-1,j3,0,0)+6.*
     & rsxy2(j1,j2,j3,0,0)-4.*rsxy2(j1,j2+1,j3,0,0)+rsxy2(j1,j2+2,j3,
     & 0,0))/(dr2(1)**4)
      a2j2rxrrrrr = (-rsxy2(j1-3,j2,j3,0,0)+4.*rsxy2(j1-2,j2,j3,0,0)-
     & 5.*rsxy2(j1-1,j2,j3,0,0)+5.*rsxy2(j1+1,j2,j3,0,0)-4.*rsxy2(j1+
     & 2,j2,j3,0,0)+rsxy2(j1+3,j2,j3,0,0))/(2.*dr2(0)**5)
      a2j2rxrrrrs = ((-rsxy2(j1-2,j2-1,j3,0,0)+rsxy2(j1-2,j2+1,j3,0,0))
     & /(2.*dr2(1))-4.*(-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,0,
     & 0))/(2.*dr2(1))+6.*(-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,
     & 0))/(2.*dr2(1))-4.*(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,
     & j3,0,0))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,0,0)+rsxy2(j1+2,j2+1,
     & j3,0,0))/(2.*dr2(1)))/(dr2(0)**4)
      a2j2rxrrrss = (-(rsxy2(j1-2,j2-1,j3,0,0)-2.*rsxy2(j1-2,j2,j3,0,0)
     & +rsxy2(j1-2,j2+1,j3,0,0))/(dr2(1)**2)+2.*(rsxy2(j1-1,j2-1,j3,0,
     & 0)-2.*rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1-1,j2+1,j3,0,0))/(dr2(1)**
     & 2)-2.*(rsxy2(j1+1,j2-1,j3,0,0)-2.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(
     & j1+1,j2+1,j3,0,0))/(dr2(1)**2)+(rsxy2(j1+2,j2-1,j3,0,0)-2.*
     & rsxy2(j1+2,j2,j3,0,0)+rsxy2(j1+2,j2+1,j3,0,0))/(dr2(1)**2))/(
     & 2.*dr2(0)**3)
      a2j2rxrrsss = ((-rsxy2(j1-1,j2-2,j3,0,0)+2.*rsxy2(j1-1,j2-1,j3,0,
     & 0)-2.*rsxy2(j1-1,j2+1,j3,0,0)+rsxy2(j1-1,j2+2,j3,0,0))/(2.*dr2(
     & 1)**3)-2.*(-rsxy2(j1,j2-2,j3,0,0)+2.*rsxy2(j1,j2-1,j3,0,0)-2.*
     & rsxy2(j1,j2+1,j3,0,0)+rsxy2(j1,j2+2,j3,0,0))/(2.*dr2(1)**3)+(-
     & rsxy2(j1+1,j2-2,j3,0,0)+2.*rsxy2(j1+1,j2-1,j3,0,0)-2.*rsxy2(j1+
     & 1,j2+1,j3,0,0)+rsxy2(j1+1,j2+2,j3,0,0))/(2.*dr2(1)**3))/(dr2(0)
     & **2)
      a2j2rxrssss = (-(rsxy2(j1-1,j2-2,j3,0,0)-4.*rsxy2(j1-1,j2-1,j3,0,
     & 0)+6.*rsxy2(j1-1,j2,j3,0,0)-4.*rsxy2(j1-1,j2+1,j3,0,0)+rsxy2(
     & j1-1,j2+2,j3,0,0))/(dr2(1)**4)+(rsxy2(j1+1,j2-2,j3,0,0)-4.*
     & rsxy2(j1+1,j2-1,j3,0,0)+6.*rsxy2(j1+1,j2,j3,0,0)-4.*rsxy2(j1+1,
     & j2+1,j3,0,0)+rsxy2(j1+1,j2+2,j3,0,0))/(dr2(1)**4))/(2.*dr2(0))
      a2j2rxsssss = (-rsxy2(j1,j2-3,j3,0,0)+4.*rsxy2(j1,j2-2,j3,0,0)-
     & 5.*rsxy2(j1,j2-1,j3,0,0)+5.*rsxy2(j1,j2+1,j3,0,0)-4.*rsxy2(j1,
     & j2+2,j3,0,0)+rsxy2(j1,j2+3,j3,0,0))/(2.*dr2(1)**5)
      a2j2sx = rsxy2(j1,j2,j3,1,0)
      a2j2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(2.*dr2(
     & 0))
      a2j2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(2.*dr2(
     & 1))
      a2j2sxrr = (rsxy2(j1-1,j2,j3,1,0)-2.*rsxy2(j1,j2,j3,1,0)+rsxy2(
     & j1+1,j2,j3,1,0))/(dr2(0)**2)
      a2j2sxrs = (-(-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,1,0))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,j3,1,0))/(
     & 2.*dr2(1)))/(2.*dr2(0))
      a2j2sxss = (rsxy2(j1,j2-1,j3,1,0)-2.*rsxy2(j1,j2,j3,1,0)+rsxy2(
     & j1,j2+1,j3,1,0))/(dr2(1)**2)
      a2j2sxrrr = (-rsxy2(j1-2,j2,j3,1,0)+2.*rsxy2(j1-1,j2,j3,1,0)-2.*
     & rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+2,j2,j3,1,0))/(2.*dr2(0)**3)
      a2j2sxrrs = ((-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,1,0))/(
     & 2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,j3,1,0))/(
     & 2.*dr2(1)))/(dr2(0)**2)
      a2j2sxrss = (-(rsxy2(j1-1,j2-1,j3,1,0)-2.*rsxy2(j1-1,j2,j3,1,0)+
     & rsxy2(j1-1,j2+1,j3,1,0))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,1,0)-
     & 2.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+1,j2+1,j3,1,0))/(dr2(1)**2))
     & /(2.*dr2(0))
      a2j2sxsss = (-rsxy2(j1,j2-2,j3,1,0)+2.*rsxy2(j1,j2-1,j3,1,0)-2.*
     & rsxy2(j1,j2+1,j3,1,0)+rsxy2(j1,j2+2,j3,1,0))/(2.*dr2(1)**3)
      a2j2sxrrrr = (rsxy2(j1-2,j2,j3,1,0)-4.*rsxy2(j1-1,j2,j3,1,0)+6.*
     & rsxy2(j1,j2,j3,1,0)-4.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+2,j2,j3,
     & 1,0))/(dr2(0)**4)
      a2j2sxrrrs = (-(-rsxy2(j1-2,j2-1,j3,1,0)+rsxy2(j1-2,j2+1,j3,1,0))
     & /(2.*dr2(1))+2.*(-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,1,
     & 0))/(2.*dr2(1))-2.*(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,
     & j3,1,0))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,1,0)+rsxy2(j1+2,j2+1,
     & j3,1,0))/(2.*dr2(1)))/(2.*dr2(0)**3)
      a2j2sxrrss = ((rsxy2(j1-1,j2-1,j3,1,0)-2.*rsxy2(j1-1,j2,j3,1,0)+
     & rsxy2(j1-1,j2+1,j3,1,0))/(dr2(1)**2)-2.*(rsxy2(j1,j2-1,j3,1,0)-
     & 2.*rsxy2(j1,j2,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(dr2(1)**2)+(
     & rsxy2(j1+1,j2-1,j3,1,0)-2.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+1,j2+
     & 1,j3,1,0))/(dr2(1)**2))/(dr2(0)**2)
      a2j2sxrsss = (-(-rsxy2(j1-1,j2-2,j3,1,0)+2.*rsxy2(j1-1,j2-1,j3,1,
     & 0)-2.*rsxy2(j1-1,j2+1,j3,1,0)+rsxy2(j1-1,j2+2,j3,1,0))/(2.*dr2(
     & 1)**3)+(-rsxy2(j1+1,j2-2,j3,1,0)+2.*rsxy2(j1+1,j2-1,j3,1,0)-2.*
     & rsxy2(j1+1,j2+1,j3,1,0)+rsxy2(j1+1,j2+2,j3,1,0))/(2.*dr2(1)**3)
     & )/(2.*dr2(0))
      a2j2sxssss = (rsxy2(j1,j2-2,j3,1,0)-4.*rsxy2(j1,j2-1,j3,1,0)+6.*
     & rsxy2(j1,j2,j3,1,0)-4.*rsxy2(j1,j2+1,j3,1,0)+rsxy2(j1,j2+2,j3,
     & 1,0))/(dr2(1)**4)
      a2j2sxrrrrr = (-rsxy2(j1-3,j2,j3,1,0)+4.*rsxy2(j1-2,j2,j3,1,0)-
     & 5.*rsxy2(j1-1,j2,j3,1,0)+5.*rsxy2(j1+1,j2,j3,1,0)-4.*rsxy2(j1+
     & 2,j2,j3,1,0)+rsxy2(j1+3,j2,j3,1,0))/(2.*dr2(0)**5)
      a2j2sxrrrrs = ((-rsxy2(j1-2,j2-1,j3,1,0)+rsxy2(j1-2,j2+1,j3,1,0))
     & /(2.*dr2(1))-4.*(-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,1,
     & 0))/(2.*dr2(1))+6.*(-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,
     & 0))/(2.*dr2(1))-4.*(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,
     & j3,1,0))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,1,0)+rsxy2(j1+2,j2+1,
     & j3,1,0))/(2.*dr2(1)))/(dr2(0)**4)
      a2j2sxrrrss = (-(rsxy2(j1-2,j2-1,j3,1,0)-2.*rsxy2(j1-2,j2,j3,1,0)
     & +rsxy2(j1-2,j2+1,j3,1,0))/(dr2(1)**2)+2.*(rsxy2(j1-1,j2-1,j3,1,
     & 0)-2.*rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1-1,j2+1,j3,1,0))/(dr2(1)**
     & 2)-2.*(rsxy2(j1+1,j2-1,j3,1,0)-2.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(
     & j1+1,j2+1,j3,1,0))/(dr2(1)**2)+(rsxy2(j1+2,j2-1,j3,1,0)-2.*
     & rsxy2(j1+2,j2,j3,1,0)+rsxy2(j1+2,j2+1,j3,1,0))/(dr2(1)**2))/(
     & 2.*dr2(0)**3)
      a2j2sxrrsss = ((-rsxy2(j1-1,j2-2,j3,1,0)+2.*rsxy2(j1-1,j2-1,j3,1,
     & 0)-2.*rsxy2(j1-1,j2+1,j3,1,0)+rsxy2(j1-1,j2+2,j3,1,0))/(2.*dr2(
     & 1)**3)-2.*(-rsxy2(j1,j2-2,j3,1,0)+2.*rsxy2(j1,j2-1,j3,1,0)-2.*
     & rsxy2(j1,j2+1,j3,1,0)+rsxy2(j1,j2+2,j3,1,0))/(2.*dr2(1)**3)+(-
     & rsxy2(j1+1,j2-2,j3,1,0)+2.*rsxy2(j1+1,j2-1,j3,1,0)-2.*rsxy2(j1+
     & 1,j2+1,j3,1,0)+rsxy2(j1+1,j2+2,j3,1,0))/(2.*dr2(1)**3))/(dr2(0)
     & **2)
      a2j2sxrssss = (-(rsxy2(j1-1,j2-2,j3,1,0)-4.*rsxy2(j1-1,j2-1,j3,1,
     & 0)+6.*rsxy2(j1-1,j2,j3,1,0)-4.*rsxy2(j1-1,j2+1,j3,1,0)+rsxy2(
     & j1-1,j2+2,j3,1,0))/(dr2(1)**4)+(rsxy2(j1+1,j2-2,j3,1,0)-4.*
     & rsxy2(j1+1,j2-1,j3,1,0)+6.*rsxy2(j1+1,j2,j3,1,0)-4.*rsxy2(j1+1,
     & j2+1,j3,1,0)+rsxy2(j1+1,j2+2,j3,1,0))/(dr2(1)**4))/(2.*dr2(0))
      a2j2sxsssss = (-rsxy2(j1,j2-3,j3,1,0)+4.*rsxy2(j1,j2-2,j3,1,0)-
     & 5.*rsxy2(j1,j2-1,j3,1,0)+5.*rsxy2(j1,j2+1,j3,1,0)-4.*rsxy2(j1,
     & j2+2,j3,1,0)+rsxy2(j1,j2+3,j3,1,0))/(2.*dr2(1)**5)
      a2j2ry = rsxy2(j1,j2,j3,0,1)
      a2j2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(2.*dr2(
     & 0))
      a2j2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(2.*dr2(
     & 1))
      a2j2ryrr = (rsxy2(j1-1,j2,j3,0,1)-2.*rsxy2(j1,j2,j3,0,1)+rsxy2(
     & j1+1,j2,j3,0,1))/(dr2(0)**2)
      a2j2ryrs = (-(-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,0,1))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,j3,0,1))/(
     & 2.*dr2(1)))/(2.*dr2(0))
      a2j2ryss = (rsxy2(j1,j2-1,j3,0,1)-2.*rsxy2(j1,j2,j3,0,1)+rsxy2(
     & j1,j2+1,j3,0,1))/(dr2(1)**2)
      a2j2ryrrr = (-rsxy2(j1-2,j2,j3,0,1)+2.*rsxy2(j1-1,j2,j3,0,1)-2.*
     & rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+2,j2,j3,0,1))/(2.*dr2(0)**3)
      a2j2ryrrs = ((-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,0,1))/(
     & 2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,j3,0,1))/(
     & 2.*dr2(1)))/(dr2(0)**2)
      a2j2ryrss = (-(rsxy2(j1-1,j2-1,j3,0,1)-2.*rsxy2(j1-1,j2,j3,0,1)+
     & rsxy2(j1-1,j2+1,j3,0,1))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,0,1)-
     & 2.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+1,j2+1,j3,0,1))/(dr2(1)**2))
     & /(2.*dr2(0))
      a2j2rysss = (-rsxy2(j1,j2-2,j3,0,1)+2.*rsxy2(j1,j2-1,j3,0,1)-2.*
     & rsxy2(j1,j2+1,j3,0,1)+rsxy2(j1,j2+2,j3,0,1))/(2.*dr2(1)**3)
      a2j2ryrrrr = (rsxy2(j1-2,j2,j3,0,1)-4.*rsxy2(j1-1,j2,j3,0,1)+6.*
     & rsxy2(j1,j2,j3,0,1)-4.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+2,j2,j3,
     & 0,1))/(dr2(0)**4)
      a2j2ryrrrs = (-(-rsxy2(j1-2,j2-1,j3,0,1)+rsxy2(j1-2,j2+1,j3,0,1))
     & /(2.*dr2(1))+2.*(-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,0,
     & 1))/(2.*dr2(1))-2.*(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,
     & j3,0,1))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,0,1)+rsxy2(j1+2,j2+1,
     & j3,0,1))/(2.*dr2(1)))/(2.*dr2(0)**3)
      a2j2ryrrss = ((rsxy2(j1-1,j2-1,j3,0,1)-2.*rsxy2(j1-1,j2,j3,0,1)+
     & rsxy2(j1-1,j2+1,j3,0,1))/(dr2(1)**2)-2.*(rsxy2(j1,j2-1,j3,0,1)-
     & 2.*rsxy2(j1,j2,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(dr2(1)**2)+(
     & rsxy2(j1+1,j2-1,j3,0,1)-2.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+1,j2+
     & 1,j3,0,1))/(dr2(1)**2))/(dr2(0)**2)
      a2j2ryrsss = (-(-rsxy2(j1-1,j2-2,j3,0,1)+2.*rsxy2(j1-1,j2-1,j3,0,
     & 1)-2.*rsxy2(j1-1,j2+1,j3,0,1)+rsxy2(j1-1,j2+2,j3,0,1))/(2.*dr2(
     & 1)**3)+(-rsxy2(j1+1,j2-2,j3,0,1)+2.*rsxy2(j1+1,j2-1,j3,0,1)-2.*
     & rsxy2(j1+1,j2+1,j3,0,1)+rsxy2(j1+1,j2+2,j3,0,1))/(2.*dr2(1)**3)
     & )/(2.*dr2(0))
      a2j2ryssss = (rsxy2(j1,j2-2,j3,0,1)-4.*rsxy2(j1,j2-1,j3,0,1)+6.*
     & rsxy2(j1,j2,j3,0,1)-4.*rsxy2(j1,j2+1,j3,0,1)+rsxy2(j1,j2+2,j3,
     & 0,1))/(dr2(1)**4)
      a2j2ryrrrrr = (-rsxy2(j1-3,j2,j3,0,1)+4.*rsxy2(j1-2,j2,j3,0,1)-
     & 5.*rsxy2(j1-1,j2,j3,0,1)+5.*rsxy2(j1+1,j2,j3,0,1)-4.*rsxy2(j1+
     & 2,j2,j3,0,1)+rsxy2(j1+3,j2,j3,0,1))/(2.*dr2(0)**5)
      a2j2ryrrrrs = ((-rsxy2(j1-2,j2-1,j3,0,1)+rsxy2(j1-2,j2+1,j3,0,1))
     & /(2.*dr2(1))-4.*(-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,0,
     & 1))/(2.*dr2(1))+6.*(-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,
     & 1))/(2.*dr2(1))-4.*(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,
     & j3,0,1))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,0,1)+rsxy2(j1+2,j2+1,
     & j3,0,1))/(2.*dr2(1)))/(dr2(0)**4)
      a2j2ryrrrss = (-(rsxy2(j1-2,j2-1,j3,0,1)-2.*rsxy2(j1-2,j2,j3,0,1)
     & +rsxy2(j1-2,j2+1,j3,0,1))/(dr2(1)**2)+2.*(rsxy2(j1-1,j2-1,j3,0,
     & 1)-2.*rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1-1,j2+1,j3,0,1))/(dr2(1)**
     & 2)-2.*(rsxy2(j1+1,j2-1,j3,0,1)-2.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(
     & j1+1,j2+1,j3,0,1))/(dr2(1)**2)+(rsxy2(j1+2,j2-1,j3,0,1)-2.*
     & rsxy2(j1+2,j2,j3,0,1)+rsxy2(j1+2,j2+1,j3,0,1))/(dr2(1)**2))/(
     & 2.*dr2(0)**3)
      a2j2ryrrsss = ((-rsxy2(j1-1,j2-2,j3,0,1)+2.*rsxy2(j1-1,j2-1,j3,0,
     & 1)-2.*rsxy2(j1-1,j2+1,j3,0,1)+rsxy2(j1-1,j2+2,j3,0,1))/(2.*dr2(
     & 1)**3)-2.*(-rsxy2(j1,j2-2,j3,0,1)+2.*rsxy2(j1,j2-1,j3,0,1)-2.*
     & rsxy2(j1,j2+1,j3,0,1)+rsxy2(j1,j2+2,j3,0,1))/(2.*dr2(1)**3)+(-
     & rsxy2(j1+1,j2-2,j3,0,1)+2.*rsxy2(j1+1,j2-1,j3,0,1)-2.*rsxy2(j1+
     & 1,j2+1,j3,0,1)+rsxy2(j1+1,j2+2,j3,0,1))/(2.*dr2(1)**3))/(dr2(0)
     & **2)
      a2j2ryrssss = (-(rsxy2(j1-1,j2-2,j3,0,1)-4.*rsxy2(j1-1,j2-1,j3,0,
     & 1)+6.*rsxy2(j1-1,j2,j3,0,1)-4.*rsxy2(j1-1,j2+1,j3,0,1)+rsxy2(
     & j1-1,j2+2,j3,0,1))/(dr2(1)**4)+(rsxy2(j1+1,j2-2,j3,0,1)-4.*
     & rsxy2(j1+1,j2-1,j3,0,1)+6.*rsxy2(j1+1,j2,j3,0,1)-4.*rsxy2(j1+1,
     & j2+1,j3,0,1)+rsxy2(j1+1,j2+2,j3,0,1))/(dr2(1)**4))/(2.*dr2(0))
      a2j2rysssss = (-rsxy2(j1,j2-3,j3,0,1)+4.*rsxy2(j1,j2-2,j3,0,1)-
     & 5.*rsxy2(j1,j2-1,j3,0,1)+5.*rsxy2(j1,j2+1,j3,0,1)-4.*rsxy2(j1,
     & j2+2,j3,0,1)+rsxy2(j1,j2+3,j3,0,1))/(2.*dr2(1)**5)
      a2j2sy = rsxy2(j1,j2,j3,1,1)
      a2j2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(2.*dr2(
     & 0))
      a2j2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(2.*dr2(
     & 1))
      a2j2syrr = (rsxy2(j1-1,j2,j3,1,1)-2.*rsxy2(j1,j2,j3,1,1)+rsxy2(
     & j1+1,j2,j3,1,1))/(dr2(0)**2)
      a2j2syrs = (-(-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,1,1))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,j3,1,1))/(
     & 2.*dr2(1)))/(2.*dr2(0))
      a2j2syss = (rsxy2(j1,j2-1,j3,1,1)-2.*rsxy2(j1,j2,j3,1,1)+rsxy2(
     & j1,j2+1,j3,1,1))/(dr2(1)**2)
      a2j2syrrr = (-rsxy2(j1-2,j2,j3,1,1)+2.*rsxy2(j1-1,j2,j3,1,1)-2.*
     & rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+2,j2,j3,1,1))/(2.*dr2(0)**3)
      a2j2syrrs = ((-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,1,1))/(
     & 2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,j3,1,1))/(
     & 2.*dr2(1)))/(dr2(0)**2)
      a2j2syrss = (-(rsxy2(j1-1,j2-1,j3,1,1)-2.*rsxy2(j1-1,j2,j3,1,1)+
     & rsxy2(j1-1,j2+1,j3,1,1))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,1,1)-
     & 2.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+1,j2+1,j3,1,1))/(dr2(1)**2))
     & /(2.*dr2(0))
      a2j2sysss = (-rsxy2(j1,j2-2,j3,1,1)+2.*rsxy2(j1,j2-1,j3,1,1)-2.*
     & rsxy2(j1,j2+1,j3,1,1)+rsxy2(j1,j2+2,j3,1,1))/(2.*dr2(1)**3)
      a2j2syrrrr = (rsxy2(j1-2,j2,j3,1,1)-4.*rsxy2(j1-1,j2,j3,1,1)+6.*
     & rsxy2(j1,j2,j3,1,1)-4.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+2,j2,j3,
     & 1,1))/(dr2(0)**4)
      a2j2syrrrs = (-(-rsxy2(j1-2,j2-1,j3,1,1)+rsxy2(j1-2,j2+1,j3,1,1))
     & /(2.*dr2(1))+2.*(-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,1,
     & 1))/(2.*dr2(1))-2.*(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,
     & j3,1,1))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,1,1)+rsxy2(j1+2,j2+1,
     & j3,1,1))/(2.*dr2(1)))/(2.*dr2(0)**3)
      a2j2syrrss = ((rsxy2(j1-1,j2-1,j3,1,1)-2.*rsxy2(j1-1,j2,j3,1,1)+
     & rsxy2(j1-1,j2+1,j3,1,1))/(dr2(1)**2)-2.*(rsxy2(j1,j2-1,j3,1,1)-
     & 2.*rsxy2(j1,j2,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(dr2(1)**2)+(
     & rsxy2(j1+1,j2-1,j3,1,1)-2.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+1,j2+
     & 1,j3,1,1))/(dr2(1)**2))/(dr2(0)**2)
      a2j2syrsss = (-(-rsxy2(j1-1,j2-2,j3,1,1)+2.*rsxy2(j1-1,j2-1,j3,1,
     & 1)-2.*rsxy2(j1-1,j2+1,j3,1,1)+rsxy2(j1-1,j2+2,j3,1,1))/(2.*dr2(
     & 1)**3)+(-rsxy2(j1+1,j2-2,j3,1,1)+2.*rsxy2(j1+1,j2-1,j3,1,1)-2.*
     & rsxy2(j1+1,j2+1,j3,1,1)+rsxy2(j1+1,j2+2,j3,1,1))/(2.*dr2(1)**3)
     & )/(2.*dr2(0))
      a2j2syssss = (rsxy2(j1,j2-2,j3,1,1)-4.*rsxy2(j1,j2-1,j3,1,1)+6.*
     & rsxy2(j1,j2,j3,1,1)-4.*rsxy2(j1,j2+1,j3,1,1)+rsxy2(j1,j2+2,j3,
     & 1,1))/(dr2(1)**4)
      a2j2syrrrrr = (-rsxy2(j1-3,j2,j3,1,1)+4.*rsxy2(j1-2,j2,j3,1,1)-
     & 5.*rsxy2(j1-1,j2,j3,1,1)+5.*rsxy2(j1+1,j2,j3,1,1)-4.*rsxy2(j1+
     & 2,j2,j3,1,1)+rsxy2(j1+3,j2,j3,1,1))/(2.*dr2(0)**5)
      a2j2syrrrrs = ((-rsxy2(j1-2,j2-1,j3,1,1)+rsxy2(j1-2,j2+1,j3,1,1))
     & /(2.*dr2(1))-4.*(-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,1,
     & 1))/(2.*dr2(1))+6.*(-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,
     & 1))/(2.*dr2(1))-4.*(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,
     & j3,1,1))/(2.*dr2(1))+(-rsxy2(j1+2,j2-1,j3,1,1)+rsxy2(j1+2,j2+1,
     & j3,1,1))/(2.*dr2(1)))/(dr2(0)**4)
      a2j2syrrrss = (-(rsxy2(j1-2,j2-1,j3,1,1)-2.*rsxy2(j1-2,j2,j3,1,1)
     & +rsxy2(j1-2,j2+1,j3,1,1))/(dr2(1)**2)+2.*(rsxy2(j1-1,j2-1,j3,1,
     & 1)-2.*rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1-1,j2+1,j3,1,1))/(dr2(1)**
     & 2)-2.*(rsxy2(j1+1,j2-1,j3,1,1)-2.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(
     & j1+1,j2+1,j3,1,1))/(dr2(1)**2)+(rsxy2(j1+2,j2-1,j3,1,1)-2.*
     & rsxy2(j1+2,j2,j3,1,1)+rsxy2(j1+2,j2+1,j3,1,1))/(dr2(1)**2))/(
     & 2.*dr2(0)**3)
      a2j2syrrsss = ((-rsxy2(j1-1,j2-2,j3,1,1)+2.*rsxy2(j1-1,j2-1,j3,1,
     & 1)-2.*rsxy2(j1-1,j2+1,j3,1,1)+rsxy2(j1-1,j2+2,j3,1,1))/(2.*dr2(
     & 1)**3)-2.*(-rsxy2(j1,j2-2,j3,1,1)+2.*rsxy2(j1,j2-1,j3,1,1)-2.*
     & rsxy2(j1,j2+1,j3,1,1)+rsxy2(j1,j2+2,j3,1,1))/(2.*dr2(1)**3)+(-
     & rsxy2(j1+1,j2-2,j3,1,1)+2.*rsxy2(j1+1,j2-1,j3,1,1)-2.*rsxy2(j1+
     & 1,j2+1,j3,1,1)+rsxy2(j1+1,j2+2,j3,1,1))/(2.*dr2(1)**3))/(dr2(0)
     & **2)
      a2j2syrssss = (-(rsxy2(j1-1,j2-2,j3,1,1)-4.*rsxy2(j1-1,j2-1,j3,1,
     & 1)+6.*rsxy2(j1-1,j2,j3,1,1)-4.*rsxy2(j1-1,j2+1,j3,1,1)+rsxy2(
     & j1-1,j2+2,j3,1,1))/(dr2(1)**4)+(rsxy2(j1+1,j2-2,j3,1,1)-4.*
     & rsxy2(j1+1,j2-1,j3,1,1)+6.*rsxy2(j1+1,j2,j3,1,1)-4.*rsxy2(j1+1,
     & j2+1,j3,1,1)+rsxy2(j1+1,j2+2,j3,1,1))/(dr2(1)**4))/(2.*dr2(0))
      a2j2sysssss = (-rsxy2(j1,j2-3,j3,1,1)+4.*rsxy2(j1,j2-2,j3,1,1)-
     & 5.*rsxy2(j1,j2-1,j3,1,1)+5.*rsxy2(j1,j2+1,j3,1,1)-4.*rsxy2(j1,
     & j2+2,j3,1,1)+rsxy2(j1,j2+3,j3,1,1))/(2.*dr2(1)**5)
      a2j2rxx = a2j2rx*a2j2rxr+a2j2sx*a2j2rxs
      a2j2rxy = a2j2ry*a2j2rxr+a2j2sy*a2j2rxs
      a2j2sxx = a2j2rx*a2j2sxr+a2j2sx*a2j2sxs
      a2j2sxy = a2j2ry*a2j2sxr+a2j2sy*a2j2sxs
      a2j2ryx = a2j2rx*a2j2ryr+a2j2sx*a2j2rys
      a2j2ryy = a2j2ry*a2j2ryr+a2j2sy*a2j2rys
      a2j2syx = a2j2rx*a2j2syr+a2j2sx*a2j2sys
      a2j2syy = a2j2ry*a2j2syr+a2j2sy*a2j2sys
      t1 = a2j2rx**2
      t6 = a2j2sx**2
      a2j2rxxx = t1*a2j2rxrr+2*a2j2rx*a2j2sx*a2j2rxrs+t6*a2j2rxss+
     & a2j2rxx*a2j2rxr+a2j2sxx*a2j2rxs
      a2j2rxxy = a2j2ry*a2j2rx*a2j2rxrr+(a2j2sy*a2j2rx+a2j2ry*a2j2sx)*
     & a2j2rxrs+a2j2sy*a2j2sx*a2j2rxss+a2j2rxy*a2j2rxr+a2j2sxy*a2j2rxs
      t1 = a2j2ry**2
      t6 = a2j2sy**2
      a2j2rxyy = t1*a2j2rxrr+2*a2j2ry*a2j2sy*a2j2rxrs+t6*a2j2rxss+
     & a2j2ryy*a2j2rxr+a2j2syy*a2j2rxs
      t1 = a2j2rx**2
      t6 = a2j2sx**2
      a2j2sxxx = t1*a2j2sxrr+2*a2j2rx*a2j2sx*a2j2sxrs+t6*a2j2sxss+
     & a2j2rxx*a2j2sxr+a2j2sxx*a2j2sxs
      a2j2sxxy = a2j2ry*a2j2rx*a2j2sxrr+(a2j2sy*a2j2rx+a2j2ry*a2j2sx)*
     & a2j2sxrs+a2j2sy*a2j2sx*a2j2sxss+a2j2rxy*a2j2sxr+a2j2sxy*a2j2sxs
      t1 = a2j2ry**2
      t6 = a2j2sy**2
      a2j2sxyy = t1*a2j2sxrr+2*a2j2ry*a2j2sy*a2j2sxrs+t6*a2j2sxss+
     & a2j2ryy*a2j2sxr+a2j2syy*a2j2sxs
      t1 = a2j2rx**2
      t6 = a2j2sx**2
      a2j2ryxx = t1*a2j2ryrr+2*a2j2rx*a2j2sx*a2j2ryrs+t6*a2j2ryss+
     & a2j2rxx*a2j2ryr+a2j2sxx*a2j2rys
      a2j2ryxy = a2j2ry*a2j2rx*a2j2ryrr+(a2j2sy*a2j2rx+a2j2ry*a2j2sx)*
     & a2j2ryrs+a2j2sy*a2j2sx*a2j2ryss+a2j2rxy*a2j2ryr+a2j2sxy*a2j2rys
      t1 = a2j2ry**2
      t6 = a2j2sy**2
      a2j2ryyy = t1*a2j2ryrr+2*a2j2ry*a2j2sy*a2j2ryrs+t6*a2j2ryss+
     & a2j2ryy*a2j2ryr+a2j2syy*a2j2rys
      t1 = a2j2rx**2
      t6 = a2j2sx**2
      a2j2syxx = t1*a2j2syrr+2*a2j2rx*a2j2sx*a2j2syrs+t6*a2j2syss+
     & a2j2rxx*a2j2syr+a2j2sxx*a2j2sys
      a2j2syxy = a2j2ry*a2j2rx*a2j2syrr+(a2j2sy*a2j2rx+a2j2ry*a2j2sx)*
     & a2j2syrs+a2j2sy*a2j2sx*a2j2syss+a2j2rxy*a2j2syr+a2j2sxy*a2j2sys
      t1 = a2j2ry**2
      t6 = a2j2sy**2
      a2j2syyy = t1*a2j2syrr+2*a2j2ry*a2j2sy*a2j2syrs+t6*a2j2syss+
     & a2j2ryy*a2j2syr+a2j2syy*a2j2sys
      t1 = a2j2rx**2
      t7 = a2j2sx**2
      a2j2rxxxx = t1*a2j2rx*a2j2rxrrr+3*t1*a2j2sx*a2j2rxrrs+3*a2j2rx*
     & t7*a2j2rxrss+t7*a2j2sx*a2j2rxsss+3*a2j2rx*a2j2rxx*a2j2rxrr+(3*
     & a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx)*a2j2rxrs+3*a2j2sxx*a2j2sx*
     & a2j2rxss+a2j2rxxx*a2j2rxr+a2j2sxxx*a2j2rxs
      t1 = a2j2rx**2
      t10 = a2j2sx**2
      a2j2rxxxy = a2j2ry*t1*a2j2rxrrr+(a2j2sy*t1+2*a2j2ry*a2j2sx*
     & a2j2rx)*a2j2rxrrs+(a2j2ry*t10+2*a2j2sy*a2j2sx*a2j2rx)*
     & a2j2rxrss+a2j2sy*t10*a2j2rxsss+(2*a2j2rxy*a2j2rx+a2j2ry*
     & a2j2rxx)*a2j2rxrr+(a2j2ry*a2j2sxx+2*a2j2sx*a2j2rxy+2*a2j2sxy*
     & a2j2rx+a2j2sy*a2j2rxx)*a2j2rxrs+(a2j2sy*a2j2sxx+2*a2j2sxy*
     & a2j2sx)*a2j2rxss+a2j2rxxy*a2j2rxr+a2j2sxxy*a2j2rxs
      t1 = a2j2ry**2
      t4 = a2j2sy*a2j2ry
      t8 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t16 = a2j2sy**2
      a2j2rxxyy = t1*a2j2rx*a2j2rxrrr+(t4*a2j2rx+a2j2ry*t8)*a2j2rxrrs+(
     & t4*a2j2sx+a2j2sy*t8)*a2j2rxrss+t16*a2j2sx*a2j2rxsss+(a2j2ryy*
     & a2j2rx+2*a2j2ry*a2j2rxy)*a2j2rxrr+(2*a2j2ry*a2j2sxy+2*a2j2sy*
     & a2j2rxy+a2j2ryy*a2j2sx+a2j2syy*a2j2rx)*a2j2rxrs+(a2j2syy*
     & a2j2sx+2*a2j2sy*a2j2sxy)*a2j2rxss+a2j2rxyy*a2j2rxr+a2j2sxyy*
     & a2j2rxs
      t1 = a2j2ry**2
      t7 = a2j2sy**2
      a2j2rxyyy = a2j2ry*t1*a2j2rxrrr+3*t1*a2j2sy*a2j2rxrrs+3*a2j2ry*
     & t7*a2j2rxrss+t7*a2j2sy*a2j2rxsss+3*a2j2ry*a2j2ryy*a2j2rxrr+(3*
     & a2j2syy*a2j2ry+3*a2j2sy*a2j2ryy)*a2j2rxrs+3*a2j2syy*a2j2sy*
     & a2j2rxss+a2j2ryyy*a2j2rxr+a2j2syyy*a2j2rxs
      t1 = a2j2rx**2
      t7 = a2j2sx**2
      a2j2sxxxx = t1*a2j2rx*a2j2sxrrr+3*t1*a2j2sx*a2j2sxrrs+3*a2j2rx*
     & t7*a2j2sxrss+t7*a2j2sx*a2j2sxsss+3*a2j2rx*a2j2rxx*a2j2sxrr+(3*
     & a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx)*a2j2sxrs+3*a2j2sxx*a2j2sx*
     & a2j2sxss+a2j2rxxx*a2j2sxr+a2j2sxxx*a2j2sxs
      t1 = a2j2rx**2
      t10 = a2j2sx**2
      a2j2sxxxy = a2j2ry*t1*a2j2sxrrr+(a2j2sy*t1+2*a2j2ry*a2j2sx*
     & a2j2rx)*a2j2sxrrs+(a2j2ry*t10+2*a2j2sy*a2j2sx*a2j2rx)*
     & a2j2sxrss+a2j2sy*t10*a2j2sxsss+(2*a2j2rxy*a2j2rx+a2j2ry*
     & a2j2rxx)*a2j2sxrr+(a2j2ry*a2j2sxx+2*a2j2sx*a2j2rxy+2*a2j2sxy*
     & a2j2rx+a2j2sy*a2j2rxx)*a2j2sxrs+(a2j2sy*a2j2sxx+2*a2j2sxy*
     & a2j2sx)*a2j2sxss+a2j2rxxy*a2j2sxr+a2j2sxxy*a2j2sxs
      t1 = a2j2ry**2
      t4 = a2j2sy*a2j2ry
      t8 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t16 = a2j2sy**2
      a2j2sxxyy = t1*a2j2rx*a2j2sxrrr+(t4*a2j2rx+a2j2ry*t8)*a2j2sxrrs+(
     & t4*a2j2sx+a2j2sy*t8)*a2j2sxrss+t16*a2j2sx*a2j2sxsss+(a2j2ryy*
     & a2j2rx+2*a2j2ry*a2j2rxy)*a2j2sxrr+(2*a2j2ry*a2j2sxy+2*a2j2sy*
     & a2j2rxy+a2j2ryy*a2j2sx+a2j2syy*a2j2rx)*a2j2sxrs+(a2j2syy*
     & a2j2sx+2*a2j2sy*a2j2sxy)*a2j2sxss+a2j2rxyy*a2j2sxr+a2j2sxyy*
     & a2j2sxs
      t1 = a2j2ry**2
      t7 = a2j2sy**2
      a2j2sxyyy = a2j2ry*t1*a2j2sxrrr+3*t1*a2j2sy*a2j2sxrrs+3*a2j2ry*
     & t7*a2j2sxrss+t7*a2j2sy*a2j2sxsss+3*a2j2ry*a2j2ryy*a2j2sxrr+(3*
     & a2j2syy*a2j2ry+3*a2j2sy*a2j2ryy)*a2j2sxrs+3*a2j2syy*a2j2sy*
     & a2j2sxss+a2j2ryyy*a2j2sxr+a2j2syyy*a2j2sxs
      t1 = a2j2rx**2
      t7 = a2j2sx**2
      a2j2ryxxx = t1*a2j2rx*a2j2ryrrr+3*t1*a2j2sx*a2j2ryrrs+3*a2j2rx*
     & t7*a2j2ryrss+t7*a2j2sx*a2j2rysss+3*a2j2rx*a2j2rxx*a2j2ryrr+(3*
     & a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx)*a2j2ryrs+3*a2j2sxx*a2j2sx*
     & a2j2ryss+a2j2rxxx*a2j2ryr+a2j2sxxx*a2j2rys
      t1 = a2j2rx**2
      t10 = a2j2sx**2
      a2j2ryxxy = a2j2ry*t1*a2j2ryrrr+(a2j2sy*t1+2*a2j2ry*a2j2sx*
     & a2j2rx)*a2j2ryrrs+(a2j2ry*t10+2*a2j2sy*a2j2sx*a2j2rx)*
     & a2j2ryrss+a2j2sy*t10*a2j2rysss+(2*a2j2rxy*a2j2rx+a2j2ry*
     & a2j2rxx)*a2j2ryrr+(a2j2ry*a2j2sxx+2*a2j2sx*a2j2rxy+2*a2j2sxy*
     & a2j2rx+a2j2sy*a2j2rxx)*a2j2ryrs+(a2j2sy*a2j2sxx+2*a2j2sxy*
     & a2j2sx)*a2j2ryss+a2j2rxxy*a2j2ryr+a2j2sxxy*a2j2rys
      t1 = a2j2ry**2
      t4 = a2j2sy*a2j2ry
      t8 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t16 = a2j2sy**2
      a2j2ryxyy = t1*a2j2rx*a2j2ryrrr+(t4*a2j2rx+a2j2ry*t8)*a2j2ryrrs+(
     & t4*a2j2sx+a2j2sy*t8)*a2j2ryrss+t16*a2j2sx*a2j2rysss+(a2j2ryy*
     & a2j2rx+2*a2j2ry*a2j2rxy)*a2j2ryrr+(2*a2j2ry*a2j2sxy+2*a2j2sy*
     & a2j2rxy+a2j2ryy*a2j2sx+a2j2syy*a2j2rx)*a2j2ryrs+(a2j2syy*
     & a2j2sx+2*a2j2sy*a2j2sxy)*a2j2ryss+a2j2rxyy*a2j2ryr+a2j2sxyy*
     & a2j2rys
      t1 = a2j2ry**2
      t7 = a2j2sy**2
      a2j2ryyyy = a2j2ry*t1*a2j2ryrrr+3*t1*a2j2sy*a2j2ryrrs+3*a2j2ry*
     & t7*a2j2ryrss+t7*a2j2sy*a2j2rysss+3*a2j2ry*a2j2ryy*a2j2ryrr+(3*
     & a2j2syy*a2j2ry+3*a2j2sy*a2j2ryy)*a2j2ryrs+3*a2j2syy*a2j2sy*
     & a2j2ryss+a2j2ryyy*a2j2ryr+a2j2syyy*a2j2rys
      t1 = a2j2rx**2
      t7 = a2j2sx**2
      a2j2syxxx = t1*a2j2rx*a2j2syrrr+3*t1*a2j2sx*a2j2syrrs+3*a2j2rx*
     & t7*a2j2syrss+t7*a2j2sx*a2j2sysss+3*a2j2rx*a2j2rxx*a2j2syrr+(3*
     & a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx)*a2j2syrs+3*a2j2sxx*a2j2sx*
     & a2j2syss+a2j2rxxx*a2j2syr+a2j2sxxx*a2j2sys
      t1 = a2j2rx**2
      t10 = a2j2sx**2
      a2j2syxxy = a2j2ry*t1*a2j2syrrr+(a2j2sy*t1+2*a2j2ry*a2j2sx*
     & a2j2rx)*a2j2syrrs+(a2j2ry*t10+2*a2j2sy*a2j2sx*a2j2rx)*
     & a2j2syrss+a2j2sy*t10*a2j2sysss+(2*a2j2rxy*a2j2rx+a2j2ry*
     & a2j2rxx)*a2j2syrr+(a2j2ry*a2j2sxx+2*a2j2sx*a2j2rxy+2*a2j2sxy*
     & a2j2rx+a2j2sy*a2j2rxx)*a2j2syrs+(a2j2sy*a2j2sxx+2*a2j2sxy*
     & a2j2sx)*a2j2syss+a2j2rxxy*a2j2syr+a2j2sxxy*a2j2sys
      t1 = a2j2ry**2
      t4 = a2j2sy*a2j2ry
      t8 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t16 = a2j2sy**2
      a2j2syxyy = t1*a2j2rx*a2j2syrrr+(t4*a2j2rx+a2j2ry*t8)*a2j2syrrs+(
     & t4*a2j2sx+a2j2sy*t8)*a2j2syrss+t16*a2j2sx*a2j2sysss+(a2j2ryy*
     & a2j2rx+2*a2j2ry*a2j2rxy)*a2j2syrr+(2*a2j2ry*a2j2sxy+2*a2j2sy*
     & a2j2rxy+a2j2ryy*a2j2sx+a2j2syy*a2j2rx)*a2j2syrs+(a2j2syy*
     & a2j2sx+2*a2j2sy*a2j2sxy)*a2j2syss+a2j2rxyy*a2j2syr+a2j2sxyy*
     & a2j2sys
      t1 = a2j2ry**2
      t7 = a2j2sy**2
      a2j2syyyy = a2j2ry*t1*a2j2syrrr+3*t1*a2j2sy*a2j2syrrs+3*a2j2ry*
     & t7*a2j2syrss+t7*a2j2sy*a2j2sysss+3*a2j2ry*a2j2ryy*a2j2syrr+(3*
     & a2j2syy*a2j2ry+3*a2j2sy*a2j2ryy)*a2j2syrs+3*a2j2syy*a2j2sy*
     & a2j2syss+a2j2ryyy*a2j2syr+a2j2syyy*a2j2sys
      t1 = a2j2rx**2
      t2 = t1**2
      t8 = a2j2sx**2
      t16 = t8**2
      t25 = a2j2sxx*a2j2rx
      t27 = t25+a2j2sx*a2j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2rxx**2
      t60 = a2j2sxx**2
      a2j2rxxxxx = t2*a2j2rxrrrr+4*t1*a2j2rx*a2j2sx*a2j2rxrrrs+6*t1*t8*
     & a2j2rxrrss+4*a2j2rx*t8*a2j2sx*a2j2rxrsss+t16*a2j2rxssss+6*t1*
     & a2j2rxx*a2j2rxrrr+(7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t1+a2j2rx*
     & t28+a2j2rx*t30)*a2j2rxrrs+(a2j2sx*t28+7*t25*a2j2sx+a2j2rxx*t8+
     & a2j2sx*t30)*a2j2rxrss+6*t8*a2j2sxx*a2j2rxsss+(4*a2j2rx*
     & a2j2rxxx+3*t46)*a2j2rxrr+(4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+
     & 6*a2j2sxx*a2j2rxx)*a2j2rxrs+(4*a2j2sxxx*a2j2sx+3*t60)*a2j2rxss+
     & a2j2rxxxx*a2j2rxr+a2j2sxxxx*a2j2rxs
      t1 = a2j2rx**2
      t2 = t1*a2j2rx
      t11 = a2j2ry*a2j2rx
      t12 = a2j2sx**2
      t19 = a2j2sy*a2j2rx
      t22 = t12*a2j2sx
      t33 = a2j2sx*a2j2rxy
      t37 = a2j2sxy*a2j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx
      a2j2rxxxxy = a2j2ry*t2*a2j2rxrrrr+(3*a2j2ry*t1*a2j2sx+a2j2sy*t2)*
     & a2j2rxrrrs+(3*t11*t12+3*a2j2sy*t1*a2j2sx)*a2j2rxrrss+(3*t19*
     & t12+a2j2ry*t22)*a2j2rxrsss+a2j2sy*t22*a2j2rxssss+(3*a2j2rxy*t1+
     & 3*t11*a2j2rxx)*a2j2rxrrr+(4*t33*a2j2rx+a2j2sxy*t1+a2j2rx*t39+
     & a2j2ry*t44+3*t19*a2j2rxx)*a2j2rxrrs+(a2j2rxy*t12+a2j2sy*t44+3*
     & a2j2ry*a2j2sxx*a2j2sx+4*t37*a2j2sx+a2j2sx*t39)*a2j2rxrss+(3*
     & a2j2sy*a2j2sxx*a2j2sx+3*t12*a2j2sxy)*a2j2rxsss+(3*a2j2rxy*
     & a2j2rxx+3*a2j2rx*a2j2rxxy+a2j2ry*a2j2rxxx)*a2j2rxrr+(3*a2j2sxx*
     & a2j2rxy+a2j2ry*a2j2sxxx+3*a2j2sxy*a2j2rxx+3*a2j2sx*a2j2rxxy+3*
     & a2j2sxxy*a2j2rx+a2j2sy*a2j2rxxx)*a2j2rxrs+(3*a2j2sxxy*a2j2sx+
     & a2j2sy*a2j2sxxx+3*a2j2sxy*a2j2sxx)*a2j2rxss+a2j2rxxxy*a2j2rxr+
     & a2j2sxxxy*a2j2rxs
      t1 = a2j2ry**2
      t2 = a2j2rx**2
      t5 = a2j2sy*a2j2ry
      t11 = a2j2sy*t2+2*a2j2ry*a2j2sx*a2j2rx
      t16 = a2j2sx**2
      t21 = a2j2ry*t16+2*a2j2sy*a2j2sx*a2j2rx
      t29 = a2j2sy**2
      t38 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
      t52 = a2j2sx*a2j2rxy
      t54 = a2j2sxy*a2j2rx
      t57 = a2j2ry*a2j2sxx+2*t52+2*t54+a2j2sy*a2j2rxx
      t60 = 2*t52+2*t54
      t68 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
      t92 = a2j2rxy**2
      t110 = a2j2sxy**2
      a2j2rxxxyy = t1*t2*a2j2rxrrrr+(t5*t2+a2j2ry*t11)*a2j2rxrrrs+(
     & a2j2sy*t11+a2j2ry*t21)*a2j2rxrrss+(a2j2sy*t21+t5*t16)*
     & a2j2rxrsss+t29*t16*a2j2rxssss+(2*a2j2ry*a2j2rxy*a2j2rx+a2j2ry*
     & t38+a2j2ryy*t2)*a2j2rxrrr+(a2j2sy*t38+2*a2j2sy*a2j2rxy*a2j2rx+
     & 2*a2j2ryy*a2j2sx*a2j2rx+a2j2syy*t2+a2j2ry*t57+a2j2ry*t60)*
     & a2j2rxrrs+(a2j2sy*t57+a2j2ry*t68+a2j2ryy*t16+2*a2j2ry*a2j2sxy*
     & a2j2sx+2*a2j2syy*a2j2sx*a2j2rx+a2j2sy*t60)*a2j2rxrss+(2*a2j2sy*
     & a2j2sxy*a2j2sx+a2j2sy*t68+a2j2syy*t16)*a2j2rxsss+(2*a2j2rx*
     & a2j2rxyy+a2j2ryy*a2j2rxx+2*a2j2ry*a2j2rxxy+2*t92)*a2j2rxrr+(4*
     & a2j2sxy*a2j2rxy+2*a2j2ry*a2j2sxxy+a2j2ryy*a2j2sxx+2*a2j2sy*
     & a2j2rxxy+2*a2j2sxyy*a2j2rx+a2j2syy*a2j2rxx+2*a2j2sx*a2j2rxyy)*
     & a2j2rxrs+(2*t110+2*a2j2sy*a2j2sxxy+a2j2syy*a2j2sxx+2*a2j2sx*
     & a2j2sxyy)*a2j2rxss+a2j2rxxyy*a2j2rxr+a2j2sxxyy*a2j2rxs
      t1 = a2j2ry**2
      t7 = a2j2sy*a2j2ry
      t11 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t13 = t7*a2j2rx+a2j2ry*t11
      t20 = t7*a2j2sx+a2j2sy*t11
      t25 = a2j2sy**2
      t33 = a2j2ryy*a2j2rx
      t34 = a2j2ry*a2j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a2j2ry*a2j2sxy
      t51 = a2j2sy*a2j2rxy
      t53 = a2j2ryy*a2j2sx
      t54 = a2j2syy*a2j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a2j2syy*a2j2sx
      t63 = a2j2sy*a2j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a2j2rxxyyy = t1*a2j2ry*a2j2rx*a2j2rxrrrr+(a2j2sy*t1*a2j2rx+
     & a2j2ry*t13)*a2j2rxrrrs+(a2j2sy*t13+a2j2ry*t20)*a2j2rxrrss+(
     & a2j2sy*t20+a2j2ry*t25*a2j2sx)*a2j2rxrsss+t25*a2j2sy*a2j2sx*
     & a2j2rxssss+(a2j2ry*t35+a2j2ry*t38+a2j2ryy*a2j2ry*a2j2rx)*
     & a2j2rxrrr+(a2j2sy*t38+a2j2sy*t35+a2j2ryy*t11+a2j2syy*a2j2ry*
     & a2j2rx+a2j2ry*t55+a2j2ry*t57)*a2j2rxrrs+(a2j2sy*t55+a2j2ry*t65+
     & a2j2ryy*a2j2sy*a2j2sx+a2j2ry*t69+a2j2syy*t11+a2j2sy*t57)*
     & a2j2rxrss+(a2j2sy*t69+a2j2sy*t65+a2j2syy*a2j2sy*a2j2sx)*
     & a2j2rxsss+(3*a2j2ry*a2j2rxyy+a2j2ryyy*a2j2rx+3*a2j2ryy*a2j2rxy)
     & *a2j2rxrr+(3*a2j2ry*a2j2sxyy+3*a2j2sy*a2j2rxyy+a2j2syyy*a2j2rx+
     & 3*a2j2syy*a2j2rxy+a2j2ryyy*a2j2sx+3*a2j2ryy*a2j2sxy)*a2j2rxrs+(
     & a2j2syyy*a2j2sx+3*a2j2sy*a2j2sxyy+3*a2j2syy*a2j2sxy)*a2j2rxss+
     & a2j2rxyyy*a2j2rxr+a2j2sxyyy*a2j2rxs
      t1 = a2j2ry**2
      t2 = t1**2
      t8 = a2j2sy**2
      t16 = t8**2
      t25 = a2j2syy*a2j2ry
      t27 = t25+a2j2sy*a2j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2ryy**2
      t60 = a2j2syy**2
      a2j2rxyyyy = t2*a2j2rxrrrr+4*t1*a2j2ry*a2j2sy*a2j2rxrrrs+6*t1*t8*
     & a2j2rxrrss+4*a2j2ry*t8*a2j2sy*a2j2rxrsss+t16*a2j2rxssss+6*t1*
     & a2j2ryy*a2j2rxrrr+(7*a2j2sy*a2j2ry*a2j2ryy+a2j2syy*t1+a2j2ry*
     & t28+a2j2ry*t30)*a2j2rxrrs+(a2j2sy*t28+7*t25*a2j2sy+a2j2ryy*t8+
     & a2j2sy*t30)*a2j2rxrss+6*t8*a2j2syy*a2j2rxsss+(4*a2j2ry*
     & a2j2ryyy+3*t46)*a2j2rxrr+(4*a2j2syyy*a2j2ry+4*a2j2sy*a2j2ryyy+
     & 6*a2j2syy*a2j2ryy)*a2j2rxrs+(4*a2j2syyy*a2j2sy+3*t60)*a2j2rxss+
     & a2j2ryyyy*a2j2rxr+a2j2syyyy*a2j2rxs
      t1 = a2j2rx**2
      t2 = t1**2
      t8 = a2j2sx**2
      t16 = t8**2
      t25 = a2j2sxx*a2j2rx
      t27 = t25+a2j2sx*a2j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2rxx**2
      t60 = a2j2sxx**2
      a2j2sxxxxx = t2*a2j2sxrrrr+4*t1*a2j2rx*a2j2sx*a2j2sxrrrs+6*t1*t8*
     & a2j2sxrrss+4*a2j2rx*t8*a2j2sx*a2j2sxrsss+t16*a2j2sxssss+6*t1*
     & a2j2rxx*a2j2sxrrr+(7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t1+a2j2rx*
     & t28+a2j2rx*t30)*a2j2sxrrs+(a2j2sx*t28+7*t25*a2j2sx+a2j2rxx*t8+
     & a2j2sx*t30)*a2j2sxrss+6*t8*a2j2sxx*a2j2sxsss+(4*a2j2rx*
     & a2j2rxxx+3*t46)*a2j2sxrr+(4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+
     & 6*a2j2sxx*a2j2rxx)*a2j2sxrs+(4*a2j2sxxx*a2j2sx+3*t60)*a2j2sxss+
     & a2j2rxxxx*a2j2sxr+a2j2sxxxx*a2j2sxs
      t1 = a2j2rx**2
      t2 = t1*a2j2rx
      t11 = a2j2ry*a2j2rx
      t12 = a2j2sx**2
      t19 = a2j2sy*a2j2rx
      t22 = t12*a2j2sx
      t33 = a2j2sx*a2j2rxy
      t37 = a2j2sxy*a2j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx
      a2j2sxxxxy = a2j2ry*t2*a2j2sxrrrr+(3*a2j2ry*t1*a2j2sx+a2j2sy*t2)*
     & a2j2sxrrrs+(3*t11*t12+3*a2j2sy*t1*a2j2sx)*a2j2sxrrss+(3*t19*
     & t12+a2j2ry*t22)*a2j2sxrsss+a2j2sy*t22*a2j2sxssss+(3*a2j2rxy*t1+
     & 3*t11*a2j2rxx)*a2j2sxrrr+(4*t33*a2j2rx+a2j2sxy*t1+a2j2rx*t39+
     & a2j2ry*t44+3*t19*a2j2rxx)*a2j2sxrrs+(a2j2rxy*t12+a2j2sy*t44+3*
     & a2j2ry*a2j2sxx*a2j2sx+4*t37*a2j2sx+a2j2sx*t39)*a2j2sxrss+(3*
     & a2j2sy*a2j2sxx*a2j2sx+3*t12*a2j2sxy)*a2j2sxsss+(3*a2j2rxy*
     & a2j2rxx+3*a2j2rx*a2j2rxxy+a2j2ry*a2j2rxxx)*a2j2sxrr+(3*a2j2sxx*
     & a2j2rxy+a2j2ry*a2j2sxxx+3*a2j2sxy*a2j2rxx+3*a2j2sx*a2j2rxxy+3*
     & a2j2sxxy*a2j2rx+a2j2sy*a2j2rxxx)*a2j2sxrs+(3*a2j2sxxy*a2j2sx+
     & a2j2sy*a2j2sxxx+3*a2j2sxy*a2j2sxx)*a2j2sxss+a2j2rxxxy*a2j2sxr+
     & a2j2sxxxy*a2j2sxs
      t1 = a2j2ry**2
      t2 = a2j2rx**2
      t5 = a2j2sy*a2j2ry
      t11 = a2j2sy*t2+2*a2j2ry*a2j2sx*a2j2rx
      t16 = a2j2sx**2
      t21 = a2j2ry*t16+2*a2j2sy*a2j2sx*a2j2rx
      t29 = a2j2sy**2
      t38 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
      t52 = a2j2sx*a2j2rxy
      t54 = a2j2sxy*a2j2rx
      t57 = a2j2ry*a2j2sxx+2*t52+2*t54+a2j2sy*a2j2rxx
      t60 = 2*t52+2*t54
      t68 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
      t92 = a2j2rxy**2
      t110 = a2j2sxy**2
      a2j2sxxxyy = t1*t2*a2j2sxrrrr+(t5*t2+a2j2ry*t11)*a2j2sxrrrs+(
     & a2j2sy*t11+a2j2ry*t21)*a2j2sxrrss+(a2j2sy*t21+t5*t16)*
     & a2j2sxrsss+t29*t16*a2j2sxssss+(2*a2j2ry*a2j2rxy*a2j2rx+a2j2ry*
     & t38+a2j2ryy*t2)*a2j2sxrrr+(a2j2sy*t38+2*a2j2sy*a2j2rxy*a2j2rx+
     & 2*a2j2ryy*a2j2sx*a2j2rx+a2j2syy*t2+a2j2ry*t57+a2j2ry*t60)*
     & a2j2sxrrs+(a2j2sy*t57+a2j2ry*t68+a2j2ryy*t16+2*a2j2ry*a2j2sxy*
     & a2j2sx+2*a2j2syy*a2j2sx*a2j2rx+a2j2sy*t60)*a2j2sxrss+(2*a2j2sy*
     & a2j2sxy*a2j2sx+a2j2sy*t68+a2j2syy*t16)*a2j2sxsss+(2*a2j2rx*
     & a2j2rxyy+a2j2ryy*a2j2rxx+2*a2j2ry*a2j2rxxy+2*t92)*a2j2sxrr+(4*
     & a2j2sxy*a2j2rxy+2*a2j2ry*a2j2sxxy+a2j2ryy*a2j2sxx+2*a2j2sy*
     & a2j2rxxy+2*a2j2sxyy*a2j2rx+a2j2syy*a2j2rxx+2*a2j2sx*a2j2rxyy)*
     & a2j2sxrs+(2*t110+2*a2j2sy*a2j2sxxy+a2j2syy*a2j2sxx+2*a2j2sx*
     & a2j2sxyy)*a2j2sxss+a2j2rxxyy*a2j2sxr+a2j2sxxyy*a2j2sxs
      t1 = a2j2ry**2
      t7 = a2j2sy*a2j2ry
      t11 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t13 = t7*a2j2rx+a2j2ry*t11
      t20 = t7*a2j2sx+a2j2sy*t11
      t25 = a2j2sy**2
      t33 = a2j2ryy*a2j2rx
      t34 = a2j2ry*a2j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a2j2ry*a2j2sxy
      t51 = a2j2sy*a2j2rxy
      t53 = a2j2ryy*a2j2sx
      t54 = a2j2syy*a2j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a2j2syy*a2j2sx
      t63 = a2j2sy*a2j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a2j2sxxyyy = t1*a2j2ry*a2j2rx*a2j2sxrrrr+(a2j2sy*t1*a2j2rx+
     & a2j2ry*t13)*a2j2sxrrrs+(a2j2sy*t13+a2j2ry*t20)*a2j2sxrrss+(
     & a2j2sy*t20+a2j2ry*t25*a2j2sx)*a2j2sxrsss+t25*a2j2sy*a2j2sx*
     & a2j2sxssss+(a2j2ry*t35+a2j2ry*t38+a2j2ryy*a2j2ry*a2j2rx)*
     & a2j2sxrrr+(a2j2sy*t38+a2j2sy*t35+a2j2ryy*t11+a2j2syy*a2j2ry*
     & a2j2rx+a2j2ry*t55+a2j2ry*t57)*a2j2sxrrs+(a2j2sy*t55+a2j2ry*t65+
     & a2j2ryy*a2j2sy*a2j2sx+a2j2ry*t69+a2j2syy*t11+a2j2sy*t57)*
     & a2j2sxrss+(a2j2sy*t69+a2j2sy*t65+a2j2syy*a2j2sy*a2j2sx)*
     & a2j2sxsss+(3*a2j2ry*a2j2rxyy+a2j2ryyy*a2j2rx+3*a2j2ryy*a2j2rxy)
     & *a2j2sxrr+(3*a2j2ry*a2j2sxyy+3*a2j2sy*a2j2rxyy+a2j2syyy*a2j2rx+
     & 3*a2j2syy*a2j2rxy+a2j2ryyy*a2j2sx+3*a2j2ryy*a2j2sxy)*a2j2sxrs+(
     & a2j2syyy*a2j2sx+3*a2j2sy*a2j2sxyy+3*a2j2syy*a2j2sxy)*a2j2sxss+
     & a2j2rxyyy*a2j2sxr+a2j2sxyyy*a2j2sxs
      t1 = a2j2ry**2
      t2 = t1**2
      t8 = a2j2sy**2
      t16 = t8**2
      t25 = a2j2syy*a2j2ry
      t27 = t25+a2j2sy*a2j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2ryy**2
      t60 = a2j2syy**2
      a2j2sxyyyy = t2*a2j2sxrrrr+4*t1*a2j2ry*a2j2sy*a2j2sxrrrs+6*t1*t8*
     & a2j2sxrrss+4*a2j2ry*t8*a2j2sy*a2j2sxrsss+t16*a2j2sxssss+6*t1*
     & a2j2ryy*a2j2sxrrr+(7*a2j2sy*a2j2ry*a2j2ryy+a2j2syy*t1+a2j2ry*
     & t28+a2j2ry*t30)*a2j2sxrrs+(a2j2sy*t28+7*t25*a2j2sy+a2j2ryy*t8+
     & a2j2sy*t30)*a2j2sxrss+6*t8*a2j2syy*a2j2sxsss+(4*a2j2ry*
     & a2j2ryyy+3*t46)*a2j2sxrr+(4*a2j2syyy*a2j2ry+4*a2j2sy*a2j2ryyy+
     & 6*a2j2syy*a2j2ryy)*a2j2sxrs+(4*a2j2syyy*a2j2sy+3*t60)*a2j2sxss+
     & a2j2ryyyy*a2j2sxr+a2j2syyyy*a2j2sxs
      t1 = a2j2rx**2
      t2 = t1**2
      t8 = a2j2sx**2
      t16 = t8**2
      t25 = a2j2sxx*a2j2rx
      t27 = t25+a2j2sx*a2j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2rxx**2
      t60 = a2j2sxx**2
      a2j2ryxxxx = t2*a2j2ryrrrr+4*t1*a2j2rx*a2j2sx*a2j2ryrrrs+6*t1*t8*
     & a2j2ryrrss+4*a2j2rx*t8*a2j2sx*a2j2ryrsss+t16*a2j2ryssss+6*t1*
     & a2j2rxx*a2j2ryrrr+(7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t1+a2j2rx*
     & t28+a2j2rx*t30)*a2j2ryrrs+(a2j2sx*t28+7*t25*a2j2sx+a2j2rxx*t8+
     & a2j2sx*t30)*a2j2ryrss+6*t8*a2j2sxx*a2j2rysss+(4*a2j2rx*
     & a2j2rxxx+3*t46)*a2j2ryrr+(4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+
     & 6*a2j2sxx*a2j2rxx)*a2j2ryrs+(4*a2j2sxxx*a2j2sx+3*t60)*a2j2ryss+
     & a2j2rxxxx*a2j2ryr+a2j2sxxxx*a2j2rys
      t1 = a2j2rx**2
      t2 = t1*a2j2rx
      t11 = a2j2ry*a2j2rx
      t12 = a2j2sx**2
      t19 = a2j2sy*a2j2rx
      t22 = t12*a2j2sx
      t33 = a2j2sx*a2j2rxy
      t37 = a2j2sxy*a2j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx
      a2j2ryxxxy = a2j2ry*t2*a2j2ryrrrr+(3*a2j2ry*t1*a2j2sx+a2j2sy*t2)*
     & a2j2ryrrrs+(3*t11*t12+3*a2j2sy*t1*a2j2sx)*a2j2ryrrss+(3*t19*
     & t12+a2j2ry*t22)*a2j2ryrsss+a2j2sy*t22*a2j2ryssss+(3*a2j2rxy*t1+
     & 3*t11*a2j2rxx)*a2j2ryrrr+(4*t33*a2j2rx+a2j2sxy*t1+a2j2rx*t39+
     & a2j2ry*t44+3*t19*a2j2rxx)*a2j2ryrrs+(a2j2rxy*t12+a2j2sy*t44+3*
     & a2j2ry*a2j2sxx*a2j2sx+4*t37*a2j2sx+a2j2sx*t39)*a2j2ryrss+(3*
     & a2j2sy*a2j2sxx*a2j2sx+3*t12*a2j2sxy)*a2j2rysss+(3*a2j2rxy*
     & a2j2rxx+3*a2j2rx*a2j2rxxy+a2j2ry*a2j2rxxx)*a2j2ryrr+(3*a2j2sxx*
     & a2j2rxy+a2j2ry*a2j2sxxx+3*a2j2sxy*a2j2rxx+3*a2j2sx*a2j2rxxy+3*
     & a2j2sxxy*a2j2rx+a2j2sy*a2j2rxxx)*a2j2ryrs+(3*a2j2sxxy*a2j2sx+
     & a2j2sy*a2j2sxxx+3*a2j2sxy*a2j2sxx)*a2j2ryss+a2j2rxxxy*a2j2ryr+
     & a2j2sxxxy*a2j2rys
      t1 = a2j2ry**2
      t2 = a2j2rx**2
      t5 = a2j2sy*a2j2ry
      t11 = a2j2sy*t2+2*a2j2ry*a2j2sx*a2j2rx
      t16 = a2j2sx**2
      t21 = a2j2ry*t16+2*a2j2sy*a2j2sx*a2j2rx
      t29 = a2j2sy**2
      t38 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
      t52 = a2j2sx*a2j2rxy
      t54 = a2j2sxy*a2j2rx
      t57 = a2j2ry*a2j2sxx+2*t52+2*t54+a2j2sy*a2j2rxx
      t60 = 2*t52+2*t54
      t68 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
      t92 = a2j2rxy**2
      t110 = a2j2sxy**2
      a2j2ryxxyy = t1*t2*a2j2ryrrrr+(t5*t2+a2j2ry*t11)*a2j2ryrrrs+(
     & a2j2sy*t11+a2j2ry*t21)*a2j2ryrrss+(a2j2sy*t21+t5*t16)*
     & a2j2ryrsss+t29*t16*a2j2ryssss+(2*a2j2ry*a2j2rxy*a2j2rx+a2j2ry*
     & t38+a2j2ryy*t2)*a2j2ryrrr+(a2j2sy*t38+2*a2j2sy*a2j2rxy*a2j2rx+
     & 2*a2j2ryy*a2j2sx*a2j2rx+a2j2syy*t2+a2j2ry*t57+a2j2ry*t60)*
     & a2j2ryrrs+(a2j2sy*t57+a2j2ry*t68+a2j2ryy*t16+2*a2j2ry*a2j2sxy*
     & a2j2sx+2*a2j2syy*a2j2sx*a2j2rx+a2j2sy*t60)*a2j2ryrss+(2*a2j2sy*
     & a2j2sxy*a2j2sx+a2j2sy*t68+a2j2syy*t16)*a2j2rysss+(2*a2j2rx*
     & a2j2rxyy+a2j2ryy*a2j2rxx+2*a2j2ry*a2j2rxxy+2*t92)*a2j2ryrr+(4*
     & a2j2sxy*a2j2rxy+2*a2j2ry*a2j2sxxy+a2j2ryy*a2j2sxx+2*a2j2sy*
     & a2j2rxxy+2*a2j2sxyy*a2j2rx+a2j2syy*a2j2rxx+2*a2j2sx*a2j2rxyy)*
     & a2j2ryrs+(2*t110+2*a2j2sy*a2j2sxxy+a2j2syy*a2j2sxx+2*a2j2sx*
     & a2j2sxyy)*a2j2ryss+a2j2rxxyy*a2j2ryr+a2j2sxxyy*a2j2rys
      t1 = a2j2ry**2
      t7 = a2j2sy*a2j2ry
      t11 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t13 = t7*a2j2rx+a2j2ry*t11
      t20 = t7*a2j2sx+a2j2sy*t11
      t25 = a2j2sy**2
      t33 = a2j2ryy*a2j2rx
      t34 = a2j2ry*a2j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a2j2ry*a2j2sxy
      t51 = a2j2sy*a2j2rxy
      t53 = a2j2ryy*a2j2sx
      t54 = a2j2syy*a2j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a2j2syy*a2j2sx
      t63 = a2j2sy*a2j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a2j2ryxyyy = t1*a2j2ry*a2j2rx*a2j2ryrrrr+(a2j2sy*t1*a2j2rx+
     & a2j2ry*t13)*a2j2ryrrrs+(a2j2sy*t13+a2j2ry*t20)*a2j2ryrrss+(
     & a2j2sy*t20+a2j2ry*t25*a2j2sx)*a2j2ryrsss+t25*a2j2sy*a2j2sx*
     & a2j2ryssss+(a2j2ry*t35+a2j2ry*t38+a2j2ryy*a2j2ry*a2j2rx)*
     & a2j2ryrrr+(a2j2sy*t38+a2j2sy*t35+a2j2ryy*t11+a2j2syy*a2j2ry*
     & a2j2rx+a2j2ry*t55+a2j2ry*t57)*a2j2ryrrs+(a2j2sy*t55+a2j2ry*t65+
     & a2j2ryy*a2j2sy*a2j2sx+a2j2ry*t69+a2j2syy*t11+a2j2sy*t57)*
     & a2j2ryrss+(a2j2sy*t69+a2j2sy*t65+a2j2syy*a2j2sy*a2j2sx)*
     & a2j2rysss+(3*a2j2ry*a2j2rxyy+a2j2ryyy*a2j2rx+3*a2j2ryy*a2j2rxy)
     & *a2j2ryrr+(3*a2j2ry*a2j2sxyy+3*a2j2sy*a2j2rxyy+a2j2syyy*a2j2rx+
     & 3*a2j2syy*a2j2rxy+a2j2ryyy*a2j2sx+3*a2j2ryy*a2j2sxy)*a2j2ryrs+(
     & a2j2syyy*a2j2sx+3*a2j2sy*a2j2sxyy+3*a2j2syy*a2j2sxy)*a2j2ryss+
     & a2j2rxyyy*a2j2ryr+a2j2sxyyy*a2j2rys
      t1 = a2j2ry**2
      t2 = t1**2
      t8 = a2j2sy**2
      t16 = t8**2
      t25 = a2j2syy*a2j2ry
      t27 = t25+a2j2sy*a2j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2ryy**2
      t60 = a2j2syy**2
      a2j2ryyyyy = t2*a2j2ryrrrr+4*t1*a2j2ry*a2j2sy*a2j2ryrrrs+6*t1*t8*
     & a2j2ryrrss+4*a2j2ry*t8*a2j2sy*a2j2ryrsss+t16*a2j2ryssss+6*t1*
     & a2j2ryy*a2j2ryrrr+(7*a2j2sy*a2j2ry*a2j2ryy+a2j2syy*t1+a2j2ry*
     & t28+a2j2ry*t30)*a2j2ryrrs+(a2j2sy*t28+7*t25*a2j2sy+a2j2ryy*t8+
     & a2j2sy*t30)*a2j2ryrss+6*t8*a2j2syy*a2j2rysss+(4*a2j2ry*
     & a2j2ryyy+3*t46)*a2j2ryrr+(4*a2j2syyy*a2j2ry+4*a2j2sy*a2j2ryyy+
     & 6*a2j2syy*a2j2ryy)*a2j2ryrs+(4*a2j2syyy*a2j2sy+3*t60)*a2j2ryss+
     & a2j2ryyyy*a2j2ryr+a2j2syyyy*a2j2rys
      t1 = a2j2rx**2
      t2 = t1**2
      t8 = a2j2sx**2
      t16 = t8**2
      t25 = a2j2sxx*a2j2rx
      t27 = t25+a2j2sx*a2j2rxx
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2rxx**2
      t60 = a2j2sxx**2
      a2j2syxxxx = t2*a2j2syrrrr+4*t1*a2j2rx*a2j2sx*a2j2syrrrs+6*t1*t8*
     & a2j2syrrss+4*a2j2rx*t8*a2j2sx*a2j2syrsss+t16*a2j2syssss+6*t1*
     & a2j2rxx*a2j2syrrr+(7*a2j2sx*a2j2rx*a2j2rxx+a2j2sxx*t1+a2j2rx*
     & t28+a2j2rx*t30)*a2j2syrrs+(a2j2sx*t28+7*t25*a2j2sx+a2j2rxx*t8+
     & a2j2sx*t30)*a2j2syrss+6*t8*a2j2sxx*a2j2sysss+(4*a2j2rx*
     & a2j2rxxx+3*t46)*a2j2syrr+(4*a2j2sxxx*a2j2rx+4*a2j2sx*a2j2rxxx+
     & 6*a2j2sxx*a2j2rxx)*a2j2syrs+(4*a2j2sxxx*a2j2sx+3*t60)*a2j2syss+
     & a2j2rxxxx*a2j2syr+a2j2sxxxx*a2j2sys
      t1 = a2j2rx**2
      t2 = t1*a2j2rx
      t11 = a2j2ry*a2j2rx
      t12 = a2j2sx**2
      t19 = a2j2sy*a2j2rx
      t22 = t12*a2j2sx
      t33 = a2j2sx*a2j2rxy
      t37 = a2j2sxy*a2j2rx
      t39 = 2*t33+2*t37
      t44 = 3*a2j2sxx*a2j2rx+3*a2j2sx*a2j2rxx
      a2j2syxxxy = a2j2ry*t2*a2j2syrrrr+(3*a2j2ry*t1*a2j2sx+a2j2sy*t2)*
     & a2j2syrrrs+(3*t11*t12+3*a2j2sy*t1*a2j2sx)*a2j2syrrss+(3*t19*
     & t12+a2j2ry*t22)*a2j2syrsss+a2j2sy*t22*a2j2syssss+(3*a2j2rxy*t1+
     & 3*t11*a2j2rxx)*a2j2syrrr+(4*t33*a2j2rx+a2j2sxy*t1+a2j2rx*t39+
     & a2j2ry*t44+3*t19*a2j2rxx)*a2j2syrrs+(a2j2rxy*t12+a2j2sy*t44+3*
     & a2j2ry*a2j2sxx*a2j2sx+4*t37*a2j2sx+a2j2sx*t39)*a2j2syrss+(3*
     & a2j2sy*a2j2sxx*a2j2sx+3*t12*a2j2sxy)*a2j2sysss+(3*a2j2rxy*
     & a2j2rxx+3*a2j2rx*a2j2rxxy+a2j2ry*a2j2rxxx)*a2j2syrr+(3*a2j2sxx*
     & a2j2rxy+a2j2ry*a2j2sxxx+3*a2j2sxy*a2j2rxx+3*a2j2sx*a2j2rxxy+3*
     & a2j2sxxy*a2j2rx+a2j2sy*a2j2rxxx)*a2j2syrs+(3*a2j2sxxy*a2j2sx+
     & a2j2sy*a2j2sxxx+3*a2j2sxy*a2j2sxx)*a2j2syss+a2j2rxxxy*a2j2syr+
     & a2j2sxxxy*a2j2sys
      t1 = a2j2ry**2
      t2 = a2j2rx**2
      t5 = a2j2sy*a2j2ry
      t11 = a2j2sy*t2+2*a2j2ry*a2j2sx*a2j2rx
      t16 = a2j2sx**2
      t21 = a2j2ry*t16+2*a2j2sy*a2j2sx*a2j2rx
      t29 = a2j2sy**2
      t38 = 2*a2j2rxy*a2j2rx+a2j2ry*a2j2rxx
      t52 = a2j2sx*a2j2rxy
      t54 = a2j2sxy*a2j2rx
      t57 = a2j2ry*a2j2sxx+2*t52+2*t54+a2j2sy*a2j2rxx
      t60 = 2*t52+2*t54
      t68 = a2j2sy*a2j2sxx+2*a2j2sxy*a2j2sx
      t92 = a2j2rxy**2
      t110 = a2j2sxy**2
      a2j2syxxyy = t1*t2*a2j2syrrrr+(t5*t2+a2j2ry*t11)*a2j2syrrrs+(
     & a2j2sy*t11+a2j2ry*t21)*a2j2syrrss+(a2j2sy*t21+t5*t16)*
     & a2j2syrsss+t29*t16*a2j2syssss+(2*a2j2ry*a2j2rxy*a2j2rx+a2j2ry*
     & t38+a2j2ryy*t2)*a2j2syrrr+(a2j2sy*t38+2*a2j2sy*a2j2rxy*a2j2rx+
     & 2*a2j2ryy*a2j2sx*a2j2rx+a2j2syy*t2+a2j2ry*t57+a2j2ry*t60)*
     & a2j2syrrs+(a2j2sy*t57+a2j2ry*t68+a2j2ryy*t16+2*a2j2ry*a2j2sxy*
     & a2j2sx+2*a2j2syy*a2j2sx*a2j2rx+a2j2sy*t60)*a2j2syrss+(2*a2j2sy*
     & a2j2sxy*a2j2sx+a2j2sy*t68+a2j2syy*t16)*a2j2sysss+(2*a2j2rx*
     & a2j2rxyy+a2j2ryy*a2j2rxx+2*a2j2ry*a2j2rxxy+2*t92)*a2j2syrr+(4*
     & a2j2sxy*a2j2rxy+2*a2j2ry*a2j2sxxy+a2j2ryy*a2j2sxx+2*a2j2sy*
     & a2j2rxxy+2*a2j2sxyy*a2j2rx+a2j2syy*a2j2rxx+2*a2j2sx*a2j2rxyy)*
     & a2j2syrs+(2*t110+2*a2j2sy*a2j2sxxy+a2j2syy*a2j2sxx+2*a2j2sx*
     & a2j2sxyy)*a2j2syss+a2j2rxxyy*a2j2syr+a2j2sxxyy*a2j2sys
      t1 = a2j2ry**2
      t7 = a2j2sy*a2j2ry
      t11 = a2j2sy*a2j2rx+a2j2ry*a2j2sx
      t13 = t7*a2j2rx+a2j2ry*t11
      t20 = t7*a2j2sx+a2j2sy*t11
      t25 = a2j2sy**2
      t33 = a2j2ryy*a2j2rx
      t34 = a2j2ry*a2j2rxy
      t35 = t33+t34
      t38 = t33+2*t34
      t49 = a2j2ry*a2j2sxy
      t51 = a2j2sy*a2j2rxy
      t53 = a2j2ryy*a2j2sx
      t54 = a2j2syy*a2j2rx
      t55 = 2*t49+2*t51+t53+t54
      t57 = t51+t53+t54+t49
      t62 = a2j2syy*a2j2sx
      t63 = a2j2sy*a2j2sxy
      t65 = t62+2*t63
      t69 = t63+t62
      a2j2syxyyy = t1*a2j2ry*a2j2rx*a2j2syrrrr+(a2j2sy*t1*a2j2rx+
     & a2j2ry*t13)*a2j2syrrrs+(a2j2sy*t13+a2j2ry*t20)*a2j2syrrss+(
     & a2j2sy*t20+a2j2ry*t25*a2j2sx)*a2j2syrsss+t25*a2j2sy*a2j2sx*
     & a2j2syssss+(a2j2ry*t35+a2j2ry*t38+a2j2ryy*a2j2ry*a2j2rx)*
     & a2j2syrrr+(a2j2sy*t38+a2j2sy*t35+a2j2ryy*t11+a2j2syy*a2j2ry*
     & a2j2rx+a2j2ry*t55+a2j2ry*t57)*a2j2syrrs+(a2j2sy*t55+a2j2ry*t65+
     & a2j2ryy*a2j2sy*a2j2sx+a2j2ry*t69+a2j2syy*t11+a2j2sy*t57)*
     & a2j2syrss+(a2j2sy*t69+a2j2sy*t65+a2j2syy*a2j2sy*a2j2sx)*
     & a2j2sysss+(3*a2j2ry*a2j2rxyy+a2j2ryyy*a2j2rx+3*a2j2ryy*a2j2rxy)
     & *a2j2syrr+(3*a2j2ry*a2j2sxyy+3*a2j2sy*a2j2rxyy+a2j2syyy*a2j2rx+
     & 3*a2j2syy*a2j2rxy+a2j2ryyy*a2j2sx+3*a2j2ryy*a2j2sxy)*a2j2syrs+(
     & a2j2syyy*a2j2sx+3*a2j2sy*a2j2sxyy+3*a2j2syy*a2j2sxy)*a2j2syss+
     & a2j2rxyyy*a2j2syr+a2j2sxyyy*a2j2sys
      t1 = a2j2ry**2
      t2 = t1**2
      t8 = a2j2sy**2
      t16 = t8**2
      t25 = a2j2syy*a2j2ry
      t27 = t25+a2j2sy*a2j2ryy
      t28 = 3*t27
      t30 = 2*t27
      t46 = a2j2ryy**2
      t60 = a2j2syy**2
      a2j2syyyyy = t2*a2j2syrrrr+4*t1*a2j2ry*a2j2sy*a2j2syrrrs+6*t1*t8*
     & a2j2syrrss+4*a2j2ry*t8*a2j2sy*a2j2syrsss+t16*a2j2syssss+6*t1*
     & a2j2ryy*a2j2syrrr+(7*a2j2sy*a2j2ry*a2j2ryy+a2j2syy*t1+a2j2ry*
     & t28+a2j2ry*t30)*a2j2syrrs+(a2j2sy*t28+7*t25*a2j2sy+a2j2ryy*t8+
     & a2j2sy*t30)*a2j2syrss+6*t8*a2j2syy*a2j2sysss+(4*a2j2ry*
     & a2j2ryyy+3*t46)*a2j2syrr+(4*a2j2syyy*a2j2ry+4*a2j2sy*a2j2ryyy+
     & 6*a2j2syy*a2j2ryy)*a2j2syrs+(4*a2j2syyy*a2j2sy+3*t60)*a2j2syss+
     & a2j2ryyyy*a2j2syr+a2j2syyyy*a2j2sys
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
      a2j2rxxxxxx = t2*a2j2rx*a2j2rxrrrrr+5*a2j2sx*t2*a2j2rxrrrrs+10*
     & t8*t9*a2j2rxrrrss+10*t13*t1*a2j2rxrrsss+5*t17*a2j2rx*
     & a2j2rxrssss+t17*a2j2sx*a2j2rxsssss+10*a2j2rxx*t9*a2j2rxrrrr+(
     & 12*a2j2rxx*t1*a2j2sx+a2j2rx*t38+a2j2sxx*t9+a2j2rx*t44)*
     & a2j2rxrrrs+(3*a2j2rxx*a2j2rx*t8+a2j2rx*t55+a2j2rx*t59+a2j2sx*
     & t44+3*t29*a2j2sx+a2j2sx*t38)*a2j2rxrrss+(a2j2rxx*t13+12*t31*t8+
     & a2j2sx*t55+a2j2sx*t59)*a2j2rxrsss+10*a2j2sxx*t13*a2j2rxssss+(
     & a2j2rx*t81+7*t79*a2j2rx+a2j2rx*t86+a2j2rx*t88+a2j2rxxx*t1)*
     & a2j2rxrrr+t121*a2j2rxrrs+t145*a2j2rxrss+(a2j2sx*t129+7*t127*
     & a2j2sx+a2j2sx*t136+a2j2sx*t133+a2j2sxxx*t8)*a2j2rxsss+(10*
     & a2j2rxx*a2j2rxxx+5*a2j2rx*a2j2rxxxx)*a2j2rxrr+(5*a2j2sxxxx*
     & a2j2rx+10*a2j2sxx*a2j2rxxx+5*a2j2sx*a2j2rxxxx+10*a2j2sxxx*
     & a2j2rxx)*a2j2rxrs+(5*a2j2sxxxx*a2j2sx+10*a2j2sxx*a2j2sxxx)*
     & a2j2rxss
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
      a2j2rxxxxxy = a2j2ry*t2*a2j2rxrrrrr+(4*a2j2ry*t5*a2j2sx+a2j2sy*
     & t2)*a2j2rxrrrrs+(4*a2j2sy*t5*a2j2sx+6*t15*t16)*a2j2rxrrrss+(4*
     & a2j2ry*a2j2rx*t22+6*t25*t16)*a2j2rxrrsss+(a2j2ry*t30+4*a2j2sy*
     & a2j2rx*t22)*a2j2rxrssss+a2j2sy*t30*a2j2rxsssss+(4*t5*a2j2rxy+6*
     & t15*a2j2rxx)*a2j2rxrrrr+(6*t25*a2j2rxx+a2j2rx*t55+6*t47*t1+
     & a2j2sxy*t5+a2j2ry*t71)*a2j2rxrrrs+(3*t50*a2j2sx+a2j2rx*t81+
     & a2j2sy*t71+a2j2ry*t89+3*a2j2rxy*a2j2rx*t16+a2j2sx*t55)*
     & a2j2rxrrss+(a2j2sy*t89+6*t51*t16+a2j2sx*t81+a2j2rxy*t22+6*
     & a2j2ry*t16*a2j2sxx)*a2j2rxrsss+(4*t22*a2j2sxy+6*a2j2sy*t16*
     & a2j2sxx)*a2j2rxssss+(a2j2rxxy*t1+7*t115*a2j2rx+a2j2rx*t120+
     & a2j2rx*t122+a2j2ry*t128)*a2j2rxrrr+t162*a2j2rxrrs+t190*
     & a2j2rxrss+(a2j2sx*t174+a2j2sxxy*t16+7*a2j2sxy*a2j2sx*a2j2sxx+
     & a2j2sx*t179+a2j2sy*t185)*a2j2rxsss+(a2j2ry*a2j2rxxxx+4*
     & a2j2rxxx*a2j2rxy+6*a2j2rxxy*a2j2rxx+4*a2j2rxxxy*a2j2rx)*
     & a2j2rxrr+(4*a2j2sxxx*a2j2rxy+a2j2ry*a2j2sxxxx+4*a2j2sxxxy*
     & a2j2rx+4*a2j2sxy*a2j2rxxx+4*a2j2sx*a2j2rxxxy+6*a2j2sxx*
     & a2j2rxxy+a2j2sy*a2j2rxxxx+6*a2j2sxxy*a2j2rxx)*a2j2rxrs+(4*
     & a2j2sxxx*a2j2sxy+4*a2j2sxxxy*a2j2sx+6*a2j2sxx*a2j2sxxy+a2j2sy*
     & a2j2sxxxx)*a2j2rxss
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
      t219 = a2j2sy*t184+a2j2sx*t175+a2j2rxyy*t17+2*a2j2sxy*t61+a2j2ry*
     & t198+3*a2j2ryy*a2j2sxx*a2j2sx+a2j2ry*t206+a2j2rx*t211+2*t208*
     & a2j2rx+a2j2sy*t165+4*t152*a2j2sx+a2j2syy*t71
      a2j2rxxxxyy = t1*t3*a2j2rxrrrrr+(t6*t3+a2j2ry*t12)*a2j2rxrrrrs+(
     & a2j2ry*t22+a2j2sy*t12)*a2j2rxrrrss+(a2j2ry*t32+a2j2sy*t22)*
     & a2j2rxrrsss+(t6*t30+a2j2sy*t32)*a2j2rxrssss+t41*t30*
     & a2j2rxsssss+(a2j2ry*t47+3*a2j2ry*a2j2rxy*t2+a2j2ryy*t3)*
     & a2j2rxrrrr+(a2j2ry*t63+3*a2j2sy*a2j2rxy*t2+a2j2ry*t75+a2j2syy*
     & t3+a2j2sy*t47+3*a2j2ryy*t2*a2j2sx)*a2j2rxrrrs+(a2j2sy*t75+
     & a2j2ry*t89+a2j2ry*t95+a2j2sy*t63+3*a2j2syy*t2*a2j2sx+3*t101*
     & t17)*a2j2rxrrss+(3*t106*t17+a2j2sy*t89+3*a2j2ry*t17*a2j2sxy+
     & a2j2sy*t95+a2j2ry*t118+a2j2ryy*t30)*a2j2rxrsss+(3*a2j2sy*t17*
     & a2j2sxy+a2j2syy*t30+a2j2sy*t118)*a2j2rxssss+(a2j2ry*t133+
     & a2j2rxyy*t2+a2j2ry*t139+4*t141*a2j2rx+a2j2rx*t146+3*t101*
     & a2j2rxx)*a2j2rxrrr+t188*a2j2rxrrs+t219*a2j2rxrss+(4*t209*
     & a2j2sx+a2j2sy*t206+a2j2sx*t211+3*a2j2syy*a2j2sxx*a2j2sx+a2j2sy*
     & t198+a2j2sxyy*t17)*a2j2rxsss+(3*a2j2rx*a2j2rxxyy+6*a2j2rxy*
     & a2j2rxxy+3*a2j2rxx*a2j2rxyy+a2j2ryy*a2j2rxxx+2*a2j2ry*
     & a2j2rxxxy)*a2j2rxrr+(3*a2j2sxxyy*a2j2rx+6*a2j2sxxy*a2j2rxy+3*
     & a2j2sxyy*a2j2rxx+3*a2j2sx*a2j2rxxyy+3*a2j2sxx*a2j2rxyy+2*
     & a2j2ry*a2j2sxxxy+6*a2j2sxy*a2j2rxxy+a2j2ryy*a2j2sxxx+2*a2j2sy*
     & a2j2rxxxy+a2j2syy*a2j2rxxx)*a2j2rxrs+(3*a2j2sxxyy*a2j2sx+3*
     & a2j2sxyy*a2j2sxx+2*a2j2sy*a2j2sxxxy+a2j2syy*a2j2sxxx+6*a2j2sxy*
     & a2j2sxxy)*a2j2rxss
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
      a2j2rxxxyyy = t1*a2j2ry*t3*a2j2rxrrrrr+(a2j2ry*t14+a2j2sy*t1*t3)*
     & a2j2rxrrrrs+(a2j2ry*t28+a2j2sy*t14)*a2j2rxrrrss+(a2j2sy*t28+
     & a2j2ry*t36)*a2j2rxrrsss+(a2j2sy*t36+a2j2ry*t41*t21)*
     & a2j2rxrssss+t41*a2j2sy*t21*a2j2rxsssss+(a2j2ry*t58+a2j2ryy*
     & a2j2ry*t3+a2j2ry*t62)*a2j2rxrrrr+(a2j2sy*t58+a2j2ry*t86+a2j2ry*
     & t88+a2j2syy*a2j2ry*t3+a2j2ryy*t12+a2j2sy*t62)*a2j2rxrrrs+(
     & a2j2ry*t104+a2j2syy*t12+a2j2ry*t113+a2j2sy*t88+a2j2sy*t86+
     & a2j2ryy*t26)*a2j2rxrrss+(a2j2ry*t124+a2j2sy*t104+a2j2syy*t26+
     & a2j2ryy*a2j2sy*t21+a2j2sy*t113+a2j2ry*t132)*a2j2rxrsss+(a2j2sy*
     & t132+a2j2syy*a2j2sy*t21+a2j2sy*t124)*a2j2rxssss+(4*a2j2ryy*
     & a2j2rxy*a2j2rx+a2j2ry*t151+a2j2ryyy*t3+a2j2ry*t155+a2j2ryy*t55+
     & a2j2ry*t159)*a2j2rxrrr+t195*a2j2rxrrs+t225*a2j2rxrss+(a2j2sy*
     & t214+a2j2syyy*t21+4*a2j2syy*a2j2sxy*a2j2sx+a2j2syy*t111+a2j2sy*
     & t223+a2j2sy*t205)*a2j2rxsss+(a2j2ryyy*a2j2rxx+3*a2j2ry*
     & a2j2rxxyy+6*a2j2rxy*a2j2rxyy+2*a2j2rx*a2j2rxyyy+3*a2j2ryy*
     & a2j2rxxy)*a2j2rxrr+(a2j2ryyy*a2j2sxx+3*a2j2ry*a2j2sxxyy+3*
     & a2j2syy*a2j2rxxy+6*a2j2sxyy*a2j2rxy+a2j2syyy*a2j2rxx+2*a2j2sx*
     & a2j2rxyyy+3*a2j2sy*a2j2rxxyy+3*a2j2ryy*a2j2sxxy+6*a2j2sxy*
     & a2j2rxyy+2*a2j2sxyyy*a2j2rx)*a2j2rxrs+(3*a2j2syy*a2j2sxxy+2*
     & a2j2sx*a2j2sxyyy+a2j2syyy*a2j2sxx+3*a2j2sy*a2j2sxxyy+6*
     & a2j2sxyy*a2j2sxy)*a2j2rxss
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
      t188 = a2j2syyy*a2j2ry*a2j2rx+a2j2ryyy*t14+a2j2ry*t167+2*a2j2ryy*
     & t73+2*a2j2syy*t54+a2j2ryy*t85+a2j2ry*t178+a2j2syy*t61+a2j2sy*
     & t151+a2j2ry*t184+a2j2sy*t146+a2j2sy*t141
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
      a2j2rxxyyyy = t2*a2j2rx*a2j2rxrrrrr+(a2j2sy*t1*a2j2ry*a2j2rx+
     & a2j2ry*t18)*a2j2rxrrrrs+(a2j2ry*t27+a2j2sy*t18)*a2j2rxrrrss+(
     & a2j2ry*t36+a2j2sy*t27)*a2j2rxrrsss+(a2j2sy*t36+a2j2ry*t33*
     & a2j2sy*a2j2sx)*a2j2rxrssss+t47*a2j2sx*a2j2rxsssss+(a2j2ryy*t1*
     & a2j2rx+a2j2ry*t58+a2j2ry*t63)*a2j2rxrrrr+(a2j2ry*t77+a2j2sy*
     & t58+a2j2syy*t1*a2j2rx+a2j2ry*t87+a2j2sy*t63+a2j2ryy*t16)*
     & a2j2rxrrrs+(a2j2syy*t16+a2j2ry*t106+a2j2ry*t108+a2j2sy*t77+
     & a2j2sy*t87+a2j2ryy*t25)*a2j2rxrrss+(a2j2syy*t25+a2j2sy*t108+
     & a2j2ry*t120+a2j2ry*t123+a2j2sy*t106+a2j2ryy*t33*a2j2sx)*
     & a2j2rxrsss+(a2j2syy*t33*a2j2sx+a2j2sy*t120+a2j2sy*t123)*
     & a2j2rxssss+(a2j2ry*t141+2*a2j2ryy*t54+a2j2ry*t146+a2j2ryyy*
     & a2j2ry*a2j2rx+a2j2ry*t151+a2j2ryy*t61)*a2j2rxrrr+t188*
     & a2j2rxrrs+t215*a2j2rxrss+(a2j2syyy*a2j2sy*a2j2sx+2*a2j2syy*
     & t102+a2j2sy*t200+a2j2sy*t203+a2j2syy*t98+a2j2sy*t208)*
     & a2j2rxsss+(a2j2ryyyy*a2j2rx+4*a2j2ry*a2j2rxyyy+4*a2j2ryyy*
     & a2j2rxy+6*a2j2ryy*a2j2rxyy)*a2j2rxrr+(4*a2j2ryyy*a2j2sxy+
     & a2j2syyyy*a2j2rx+4*a2j2sy*a2j2rxyyy+6*a2j2syy*a2j2rxyy+4*
     & a2j2ry*a2j2sxyyy+4*a2j2syyy*a2j2rxy+6*a2j2ryy*a2j2sxyy+
     & a2j2ryyyy*a2j2sx)*a2j2rxrs+(4*a2j2sy*a2j2sxyyy+a2j2syyyy*
     & a2j2sx+4*a2j2syyy*a2j2sxy+6*a2j2syy*a2j2sxyy)*a2j2rxss
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
      t121 = 7*t30*a2j2ryy+a2j2sy*t89+a2j2ry*t102+a2j2ryy*t41+a2j2syyy*
     & t1+a2j2ry*t109+a2j2sy*t87+a2j2sy*t81+2*t96*a2j2ry+a2j2ry*t117+
     & 2*a2j2ryy*t32
      t129 = a2j2syyy*a2j2sy
      t131 = a2j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a2j2sy+a2j2sy*t109+2*a2j2syy*t32+a2j2sy*t117+
     & a2j2ry*t133+a2j2ry*t136+a2j2ry*t138+a2j2syy*t41+a2j2ryyy*t8+2*
     & t129*a2j2ry+a2j2sy*t102
      a2j2rxyyyyy = t2*a2j2ry*a2j2rxrrrrr+5*a2j2sy*t2*a2j2rxrrrrs+10*
     & t8*t9*a2j2rxrrrss+10*t13*t1*a2j2rxrrsss+5*t17*a2j2ry*
     & a2j2rxrssss+t17*a2j2sy*a2j2rxsssss+10*t9*a2j2ryy*a2j2rxrrrr+(
     & 12*t26*t1+a2j2ry*t37+a2j2syy*t9+a2j2ry*t43)*a2j2rxrrrs+(3*
     & a2j2ryy*a2j2ry*t8+a2j2sy*t43+a2j2sy*t37+a2j2ry*t56+a2j2ry*t60+
     & 3*t29*a2j2sy)*a2j2rxrrss+(12*a2j2ry*t8*a2j2syy+a2j2sy*t56+
     & a2j2sy*t60+a2j2ryy*t13)*a2j2rxrsss+10*t13*a2j2syy*a2j2rxssss+(
     & a2j2ryyy*t1+a2j2ry*t81+7*t79*a2j2ry+a2j2ry*t87+a2j2ry*t89)*
     & a2j2rxrrr+t121*a2j2rxrrs+t145*a2j2rxrss+(a2j2sy*t138+a2j2sy*
     & t133+7*t131*a2j2sy+a2j2syyy*t8+a2j2sy*t136)*a2j2rxsss+(5*
     & a2j2ry*a2j2ryyyy+10*a2j2ryyy*a2j2ryy)*a2j2rxrr+(10*a2j2syy*
     & a2j2ryyy+10*a2j2syyy*a2j2ryy+5*a2j2sy*a2j2ryyyy+5*a2j2syyyy*
     & a2j2ry)*a2j2rxrs+(10*a2j2syy*a2j2syyy+5*a2j2sy*a2j2syyyy)*
     & a2j2rxss
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
      a2j2sxxxxxx = t2*a2j2rx*a2j2sxrrrrr+5*a2j2sx*t2*a2j2sxrrrrs+10*
     & t8*t9*a2j2sxrrrss+10*t13*t1*a2j2sxrrsss+5*t17*a2j2rx*
     & a2j2sxrssss+t17*a2j2sx*a2j2sxsssss+10*a2j2rxx*t9*a2j2sxrrrr+(
     & 12*a2j2rxx*t1*a2j2sx+a2j2rx*t38+a2j2sxx*t9+a2j2rx*t44)*
     & a2j2sxrrrs+(3*a2j2rxx*a2j2rx*t8+a2j2rx*t55+a2j2rx*t59+a2j2sx*
     & t44+3*t29*a2j2sx+a2j2sx*t38)*a2j2sxrrss+(a2j2rxx*t13+12*t31*t8+
     & a2j2sx*t55+a2j2sx*t59)*a2j2sxrsss+10*a2j2sxx*t13*a2j2sxssss+(
     & a2j2rx*t81+7*t79*a2j2rx+a2j2rx*t86+a2j2rx*t88+a2j2rxxx*t1)*
     & a2j2sxrrr+t121*a2j2sxrrs+t145*a2j2sxrss+(a2j2sx*t129+7*t127*
     & a2j2sx+a2j2sx*t136+a2j2sx*t133+a2j2sxxx*t8)*a2j2sxsss+(10*
     & a2j2rxx*a2j2rxxx+5*a2j2rx*a2j2rxxxx)*a2j2sxrr+(5*a2j2sxxxx*
     & a2j2rx+10*a2j2sxx*a2j2rxxx+5*a2j2sx*a2j2rxxxx+10*a2j2sxxx*
     & a2j2rxx)*a2j2sxrs+(5*a2j2sxxxx*a2j2sx+10*a2j2sxx*a2j2sxxx)*
     & a2j2sxss
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
      a2j2sxxxxxy = a2j2ry*t2*a2j2sxrrrrr+(4*a2j2ry*t5*a2j2sx+a2j2sy*
     & t2)*a2j2sxrrrrs+(4*a2j2sy*t5*a2j2sx+6*t15*t16)*a2j2sxrrrss+(4*
     & a2j2ry*a2j2rx*t22+6*t25*t16)*a2j2sxrrsss+(a2j2ry*t30+4*a2j2sy*
     & a2j2rx*t22)*a2j2sxrssss+a2j2sy*t30*a2j2sxsssss+(4*t5*a2j2rxy+6*
     & t15*a2j2rxx)*a2j2sxrrrr+(6*t25*a2j2rxx+a2j2rx*t55+6*t47*t1+
     & a2j2sxy*t5+a2j2ry*t71)*a2j2sxrrrs+(3*t50*a2j2sx+a2j2rx*t81+
     & a2j2sy*t71+a2j2ry*t89+3*a2j2rxy*a2j2rx*t16+a2j2sx*t55)*
     & a2j2sxrrss+(a2j2sy*t89+6*t51*t16+a2j2sx*t81+a2j2rxy*t22+6*
     & a2j2ry*t16*a2j2sxx)*a2j2sxrsss+(4*t22*a2j2sxy+6*a2j2sy*t16*
     & a2j2sxx)*a2j2sxssss+(a2j2rxxy*t1+7*t115*a2j2rx+a2j2rx*t120+
     & a2j2rx*t122+a2j2ry*t128)*a2j2sxrrr+t162*a2j2sxrrs+t190*
     & a2j2sxrss+(a2j2sx*t174+a2j2sxxy*t16+7*a2j2sxy*a2j2sx*a2j2sxx+
     & a2j2sx*t179+a2j2sy*t185)*a2j2sxsss+(a2j2ry*a2j2rxxxx+4*
     & a2j2rxxx*a2j2rxy+6*a2j2rxxy*a2j2rxx+4*a2j2rxxxy*a2j2rx)*
     & a2j2sxrr+(4*a2j2sxxx*a2j2rxy+a2j2ry*a2j2sxxxx+4*a2j2sxxxy*
     & a2j2rx+4*a2j2sxy*a2j2rxxx+4*a2j2sx*a2j2rxxxy+6*a2j2sxx*
     & a2j2rxxy+a2j2sy*a2j2rxxxx+6*a2j2sxxy*a2j2rxx)*a2j2sxrs+(4*
     & a2j2sxxx*a2j2sxy+4*a2j2sxxxy*a2j2sx+6*a2j2sxx*a2j2sxxy+a2j2sy*
     & a2j2sxxxx)*a2j2sxss
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
      t219 = a2j2sy*t184+a2j2sx*t175+a2j2rxyy*t17+2*a2j2sxy*t61+a2j2ry*
     & t198+3*a2j2ryy*a2j2sxx*a2j2sx+a2j2ry*t206+a2j2rx*t211+2*t208*
     & a2j2rx+a2j2sy*t165+4*t152*a2j2sx+a2j2syy*t71
      a2j2sxxxxyy = t1*t3*a2j2sxrrrrr+(t6*t3+a2j2ry*t12)*a2j2sxrrrrs+(
     & a2j2ry*t22+a2j2sy*t12)*a2j2sxrrrss+(a2j2ry*t32+a2j2sy*t22)*
     & a2j2sxrrsss+(t6*t30+a2j2sy*t32)*a2j2sxrssss+t41*t30*
     & a2j2sxsssss+(a2j2ry*t47+3*a2j2ry*a2j2rxy*t2+a2j2ryy*t3)*
     & a2j2sxrrrr+(a2j2ry*t63+3*a2j2sy*a2j2rxy*t2+a2j2ry*t75+a2j2syy*
     & t3+a2j2sy*t47+3*a2j2ryy*t2*a2j2sx)*a2j2sxrrrs+(a2j2sy*t75+
     & a2j2ry*t89+a2j2ry*t95+a2j2sy*t63+3*a2j2syy*t2*a2j2sx+3*t101*
     & t17)*a2j2sxrrss+(3*t106*t17+a2j2sy*t89+3*a2j2ry*t17*a2j2sxy+
     & a2j2sy*t95+a2j2ry*t118+a2j2ryy*t30)*a2j2sxrsss+(3*a2j2sy*t17*
     & a2j2sxy+a2j2syy*t30+a2j2sy*t118)*a2j2sxssss+(a2j2ry*t133+
     & a2j2rxyy*t2+a2j2ry*t139+4*t141*a2j2rx+a2j2rx*t146+3*t101*
     & a2j2rxx)*a2j2sxrrr+t188*a2j2sxrrs+t219*a2j2sxrss+(4*t209*
     & a2j2sx+a2j2sy*t206+a2j2sx*t211+3*a2j2syy*a2j2sxx*a2j2sx+a2j2sy*
     & t198+a2j2sxyy*t17)*a2j2sxsss+(3*a2j2rx*a2j2rxxyy+6*a2j2rxy*
     & a2j2rxxy+3*a2j2rxx*a2j2rxyy+a2j2ryy*a2j2rxxx+2*a2j2ry*
     & a2j2rxxxy)*a2j2sxrr+(3*a2j2sxxyy*a2j2rx+6*a2j2sxxy*a2j2rxy+3*
     & a2j2sxyy*a2j2rxx+3*a2j2sx*a2j2rxxyy+3*a2j2sxx*a2j2rxyy+2*
     & a2j2ry*a2j2sxxxy+6*a2j2sxy*a2j2rxxy+a2j2ryy*a2j2sxxx+2*a2j2sy*
     & a2j2rxxxy+a2j2syy*a2j2rxxx)*a2j2sxrs+(3*a2j2sxxyy*a2j2sx+3*
     & a2j2sxyy*a2j2sxx+2*a2j2sy*a2j2sxxxy+a2j2syy*a2j2sxxx+6*a2j2sxy*
     & a2j2sxxy)*a2j2sxss
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
      a2j2sxxxyyy = t1*a2j2ry*t3*a2j2sxrrrrr+(a2j2ry*t14+a2j2sy*t1*t3)*
     & a2j2sxrrrrs+(a2j2ry*t28+a2j2sy*t14)*a2j2sxrrrss+(a2j2sy*t28+
     & a2j2ry*t36)*a2j2sxrrsss+(a2j2sy*t36+a2j2ry*t41*t21)*
     & a2j2sxrssss+t41*a2j2sy*t21*a2j2sxsssss+(a2j2ry*t58+a2j2ryy*
     & a2j2ry*t3+a2j2ry*t62)*a2j2sxrrrr+(a2j2sy*t58+a2j2ry*t86+a2j2ry*
     & t88+a2j2syy*a2j2ry*t3+a2j2ryy*t12+a2j2sy*t62)*a2j2sxrrrs+(
     & a2j2ry*t104+a2j2syy*t12+a2j2ry*t113+a2j2sy*t88+a2j2sy*t86+
     & a2j2ryy*t26)*a2j2sxrrss+(a2j2ry*t124+a2j2sy*t104+a2j2syy*t26+
     & a2j2ryy*a2j2sy*t21+a2j2sy*t113+a2j2ry*t132)*a2j2sxrsss+(a2j2sy*
     & t132+a2j2syy*a2j2sy*t21+a2j2sy*t124)*a2j2sxssss+(4*a2j2ryy*
     & a2j2rxy*a2j2rx+a2j2ry*t151+a2j2ryyy*t3+a2j2ry*t155+a2j2ryy*t55+
     & a2j2ry*t159)*a2j2sxrrr+t195*a2j2sxrrs+t225*a2j2sxrss+(a2j2sy*
     & t214+a2j2syyy*t21+4*a2j2syy*a2j2sxy*a2j2sx+a2j2syy*t111+a2j2sy*
     & t223+a2j2sy*t205)*a2j2sxsss+(a2j2ryyy*a2j2rxx+3*a2j2ry*
     & a2j2rxxyy+6*a2j2rxy*a2j2rxyy+2*a2j2rx*a2j2rxyyy+3*a2j2ryy*
     & a2j2rxxy)*a2j2sxrr+(a2j2ryyy*a2j2sxx+3*a2j2ry*a2j2sxxyy+3*
     & a2j2syy*a2j2rxxy+6*a2j2sxyy*a2j2rxy+a2j2syyy*a2j2rxx+2*a2j2sx*
     & a2j2rxyyy+3*a2j2sy*a2j2rxxyy+3*a2j2ryy*a2j2sxxy+6*a2j2sxy*
     & a2j2rxyy+2*a2j2sxyyy*a2j2rx)*a2j2sxrs+(3*a2j2syy*a2j2sxxy+2*
     & a2j2sx*a2j2sxyyy+a2j2syyy*a2j2sxx+3*a2j2sy*a2j2sxxyy+6*
     & a2j2sxyy*a2j2sxy)*a2j2sxss
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
      t188 = a2j2syyy*a2j2ry*a2j2rx+a2j2ryyy*t14+a2j2ry*t167+2*a2j2ryy*
     & t73+2*a2j2syy*t54+a2j2ryy*t85+a2j2ry*t178+a2j2syy*t61+a2j2sy*
     & t151+a2j2ry*t184+a2j2sy*t146+a2j2sy*t141
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
      a2j2sxxyyyy = t2*a2j2rx*a2j2sxrrrrr+(a2j2sy*t1*a2j2ry*a2j2rx+
     & a2j2ry*t18)*a2j2sxrrrrs+(a2j2ry*t27+a2j2sy*t18)*a2j2sxrrrss+(
     & a2j2ry*t36+a2j2sy*t27)*a2j2sxrrsss+(a2j2sy*t36+a2j2ry*t33*
     & a2j2sy*a2j2sx)*a2j2sxrssss+t47*a2j2sx*a2j2sxsssss+(a2j2ryy*t1*
     & a2j2rx+a2j2ry*t58+a2j2ry*t63)*a2j2sxrrrr+(a2j2ry*t77+a2j2sy*
     & t58+a2j2syy*t1*a2j2rx+a2j2ry*t87+a2j2sy*t63+a2j2ryy*t16)*
     & a2j2sxrrrs+(a2j2syy*t16+a2j2ry*t106+a2j2ry*t108+a2j2sy*t77+
     & a2j2sy*t87+a2j2ryy*t25)*a2j2sxrrss+(a2j2syy*t25+a2j2sy*t108+
     & a2j2ry*t120+a2j2ry*t123+a2j2sy*t106+a2j2ryy*t33*a2j2sx)*
     & a2j2sxrsss+(a2j2syy*t33*a2j2sx+a2j2sy*t120+a2j2sy*t123)*
     & a2j2sxssss+(a2j2ry*t141+2*a2j2ryy*t54+a2j2ry*t146+a2j2ryyy*
     & a2j2ry*a2j2rx+a2j2ry*t151+a2j2ryy*t61)*a2j2sxrrr+t188*
     & a2j2sxrrs+t215*a2j2sxrss+(a2j2syyy*a2j2sy*a2j2sx+2*a2j2syy*
     & t102+a2j2sy*t200+a2j2sy*t203+a2j2syy*t98+a2j2sy*t208)*
     & a2j2sxsss+(a2j2ryyyy*a2j2rx+4*a2j2ry*a2j2rxyyy+4*a2j2ryyy*
     & a2j2rxy+6*a2j2ryy*a2j2rxyy)*a2j2sxrr+(4*a2j2ryyy*a2j2sxy+
     & a2j2syyyy*a2j2rx+4*a2j2sy*a2j2rxyyy+6*a2j2syy*a2j2rxyy+4*
     & a2j2ry*a2j2sxyyy+4*a2j2syyy*a2j2rxy+6*a2j2ryy*a2j2sxyy+
     & a2j2ryyyy*a2j2sx)*a2j2sxrs+(4*a2j2sy*a2j2sxyyy+a2j2syyyy*
     & a2j2sx+4*a2j2syyy*a2j2sxy+6*a2j2syy*a2j2sxyy)*a2j2sxss
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
      t121 = 7*t30*a2j2ryy+a2j2sy*t89+a2j2ry*t102+a2j2ryy*t41+a2j2syyy*
     & t1+a2j2ry*t109+a2j2sy*t87+a2j2sy*t81+2*t96*a2j2ry+a2j2ry*t117+
     & 2*a2j2ryy*t32
      t129 = a2j2syyy*a2j2sy
      t131 = a2j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a2j2sy+a2j2sy*t109+2*a2j2syy*t32+a2j2sy*t117+
     & a2j2ry*t133+a2j2ry*t136+a2j2ry*t138+a2j2syy*t41+a2j2ryyy*t8+2*
     & t129*a2j2ry+a2j2sy*t102
      a2j2sxyyyyy = t2*a2j2ry*a2j2sxrrrrr+5*a2j2sy*t2*a2j2sxrrrrs+10*
     & t8*t9*a2j2sxrrrss+10*t13*t1*a2j2sxrrsss+5*t17*a2j2ry*
     & a2j2sxrssss+t17*a2j2sy*a2j2sxsssss+10*t9*a2j2ryy*a2j2sxrrrr+(
     & 12*t26*t1+a2j2ry*t37+a2j2syy*t9+a2j2ry*t43)*a2j2sxrrrs+(3*
     & a2j2ryy*a2j2ry*t8+a2j2sy*t43+a2j2sy*t37+a2j2ry*t56+a2j2ry*t60+
     & 3*t29*a2j2sy)*a2j2sxrrss+(12*a2j2ry*t8*a2j2syy+a2j2sy*t56+
     & a2j2sy*t60+a2j2ryy*t13)*a2j2sxrsss+10*t13*a2j2syy*a2j2sxssss+(
     & a2j2ryyy*t1+a2j2ry*t81+7*t79*a2j2ry+a2j2ry*t87+a2j2ry*t89)*
     & a2j2sxrrr+t121*a2j2sxrrs+t145*a2j2sxrss+(a2j2sy*t138+a2j2sy*
     & t133+7*t131*a2j2sy+a2j2syyy*t8+a2j2sy*t136)*a2j2sxsss+(5*
     & a2j2ry*a2j2ryyyy+10*a2j2ryyy*a2j2ryy)*a2j2sxrr+(10*a2j2syy*
     & a2j2ryyy+10*a2j2syyy*a2j2ryy+5*a2j2sy*a2j2ryyyy+5*a2j2syyyy*
     & a2j2ry)*a2j2sxrs+(10*a2j2syy*a2j2syyy+5*a2j2sy*a2j2syyyy)*
     & a2j2sxss
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
      a2j2ryxxxxx = t2*a2j2rx*a2j2ryrrrrr+5*a2j2sx*t2*a2j2ryrrrrs+10*
     & t8*t9*a2j2ryrrrss+10*t13*t1*a2j2ryrrsss+5*t17*a2j2rx*
     & a2j2ryrssss+t17*a2j2sx*a2j2rysssss+10*a2j2rxx*t9*a2j2ryrrrr+(
     & 12*a2j2rxx*t1*a2j2sx+a2j2rx*t38+a2j2sxx*t9+a2j2rx*t44)*
     & a2j2ryrrrs+(3*a2j2rxx*a2j2rx*t8+a2j2rx*t55+a2j2rx*t59+a2j2sx*
     & t44+3*t29*a2j2sx+a2j2sx*t38)*a2j2ryrrss+(a2j2rxx*t13+12*t31*t8+
     & a2j2sx*t55+a2j2sx*t59)*a2j2ryrsss+10*a2j2sxx*t13*a2j2ryssss+(
     & a2j2rx*t81+7*t79*a2j2rx+a2j2rx*t86+a2j2rx*t88+a2j2rxxx*t1)*
     & a2j2ryrrr+t121*a2j2ryrrs+t145*a2j2ryrss+(a2j2sx*t129+7*t127*
     & a2j2sx+a2j2sx*t136+a2j2sx*t133+a2j2sxxx*t8)*a2j2rysss+(10*
     & a2j2rxx*a2j2rxxx+5*a2j2rx*a2j2rxxxx)*a2j2ryrr+(5*a2j2sxxxx*
     & a2j2rx+10*a2j2sxx*a2j2rxxx+5*a2j2sx*a2j2rxxxx+10*a2j2sxxx*
     & a2j2rxx)*a2j2ryrs+(5*a2j2sxxxx*a2j2sx+10*a2j2sxx*a2j2sxxx)*
     & a2j2ryss
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
      a2j2ryxxxxy = a2j2ry*t2*a2j2ryrrrrr+(4*a2j2ry*t5*a2j2sx+a2j2sy*
     & t2)*a2j2ryrrrrs+(4*a2j2sy*t5*a2j2sx+6*t15*t16)*a2j2ryrrrss+(4*
     & a2j2ry*a2j2rx*t22+6*t25*t16)*a2j2ryrrsss+(a2j2ry*t30+4*a2j2sy*
     & a2j2rx*t22)*a2j2ryrssss+a2j2sy*t30*a2j2rysssss+(4*t5*a2j2rxy+6*
     & t15*a2j2rxx)*a2j2ryrrrr+(6*t25*a2j2rxx+a2j2rx*t55+6*t47*t1+
     & a2j2sxy*t5+a2j2ry*t71)*a2j2ryrrrs+(3*t50*a2j2sx+a2j2rx*t81+
     & a2j2sy*t71+a2j2ry*t89+3*a2j2rxy*a2j2rx*t16+a2j2sx*t55)*
     & a2j2ryrrss+(a2j2sy*t89+6*t51*t16+a2j2sx*t81+a2j2rxy*t22+6*
     & a2j2ry*t16*a2j2sxx)*a2j2ryrsss+(4*t22*a2j2sxy+6*a2j2sy*t16*
     & a2j2sxx)*a2j2ryssss+(a2j2rxxy*t1+7*t115*a2j2rx+a2j2rx*t120+
     & a2j2rx*t122+a2j2ry*t128)*a2j2ryrrr+t162*a2j2ryrrs+t190*
     & a2j2ryrss+(a2j2sx*t174+a2j2sxxy*t16+7*a2j2sxy*a2j2sx*a2j2sxx+
     & a2j2sx*t179+a2j2sy*t185)*a2j2rysss+(a2j2ry*a2j2rxxxx+4*
     & a2j2rxxx*a2j2rxy+6*a2j2rxxy*a2j2rxx+4*a2j2rxxxy*a2j2rx)*
     & a2j2ryrr+(4*a2j2sxxx*a2j2rxy+a2j2ry*a2j2sxxxx+4*a2j2sxxxy*
     & a2j2rx+4*a2j2sxy*a2j2rxxx+4*a2j2sx*a2j2rxxxy+6*a2j2sxx*
     & a2j2rxxy+a2j2sy*a2j2rxxxx+6*a2j2sxxy*a2j2rxx)*a2j2ryrs+(4*
     & a2j2sxxx*a2j2sxy+4*a2j2sxxxy*a2j2sx+6*a2j2sxx*a2j2sxxy+a2j2sy*
     & a2j2sxxxx)*a2j2ryss
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
      t219 = a2j2sy*t184+a2j2sx*t175+a2j2rxyy*t17+2*a2j2sxy*t61+a2j2ry*
     & t198+3*a2j2ryy*a2j2sxx*a2j2sx+a2j2ry*t206+a2j2rx*t211+2*t208*
     & a2j2rx+a2j2sy*t165+4*t152*a2j2sx+a2j2syy*t71
      a2j2ryxxxyy = t1*t3*a2j2ryrrrrr+(t6*t3+a2j2ry*t12)*a2j2ryrrrrs+(
     & a2j2ry*t22+a2j2sy*t12)*a2j2ryrrrss+(a2j2ry*t32+a2j2sy*t22)*
     & a2j2ryrrsss+(t6*t30+a2j2sy*t32)*a2j2ryrssss+t41*t30*
     & a2j2rysssss+(a2j2ry*t47+3*a2j2ry*a2j2rxy*t2+a2j2ryy*t3)*
     & a2j2ryrrrr+(a2j2ry*t63+3*a2j2sy*a2j2rxy*t2+a2j2ry*t75+a2j2syy*
     & t3+a2j2sy*t47+3*a2j2ryy*t2*a2j2sx)*a2j2ryrrrs+(a2j2sy*t75+
     & a2j2ry*t89+a2j2ry*t95+a2j2sy*t63+3*a2j2syy*t2*a2j2sx+3*t101*
     & t17)*a2j2ryrrss+(3*t106*t17+a2j2sy*t89+3*a2j2ry*t17*a2j2sxy+
     & a2j2sy*t95+a2j2ry*t118+a2j2ryy*t30)*a2j2ryrsss+(3*a2j2sy*t17*
     & a2j2sxy+a2j2syy*t30+a2j2sy*t118)*a2j2ryssss+(a2j2ry*t133+
     & a2j2rxyy*t2+a2j2ry*t139+4*t141*a2j2rx+a2j2rx*t146+3*t101*
     & a2j2rxx)*a2j2ryrrr+t188*a2j2ryrrs+t219*a2j2ryrss+(4*t209*
     & a2j2sx+a2j2sy*t206+a2j2sx*t211+3*a2j2syy*a2j2sxx*a2j2sx+a2j2sy*
     & t198+a2j2sxyy*t17)*a2j2rysss+(3*a2j2rx*a2j2rxxyy+6*a2j2rxy*
     & a2j2rxxy+3*a2j2rxx*a2j2rxyy+a2j2ryy*a2j2rxxx+2*a2j2ry*
     & a2j2rxxxy)*a2j2ryrr+(3*a2j2sxxyy*a2j2rx+6*a2j2sxxy*a2j2rxy+3*
     & a2j2sxyy*a2j2rxx+3*a2j2sx*a2j2rxxyy+3*a2j2sxx*a2j2rxyy+2*
     & a2j2ry*a2j2sxxxy+6*a2j2sxy*a2j2rxxy+a2j2ryy*a2j2sxxx+2*a2j2sy*
     & a2j2rxxxy+a2j2syy*a2j2rxxx)*a2j2ryrs+(3*a2j2sxxyy*a2j2sx+3*
     & a2j2sxyy*a2j2sxx+2*a2j2sy*a2j2sxxxy+a2j2syy*a2j2sxxx+6*a2j2sxy*
     & a2j2sxxy)*a2j2ryss
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
      a2j2ryxxyyy = t1*a2j2ry*t3*a2j2ryrrrrr+(a2j2ry*t14+a2j2sy*t1*t3)*
     & a2j2ryrrrrs+(a2j2ry*t28+a2j2sy*t14)*a2j2ryrrrss+(a2j2sy*t28+
     & a2j2ry*t36)*a2j2ryrrsss+(a2j2sy*t36+a2j2ry*t41*t21)*
     & a2j2ryrssss+t41*a2j2sy*t21*a2j2rysssss+(a2j2ry*t58+a2j2ryy*
     & a2j2ry*t3+a2j2ry*t62)*a2j2ryrrrr+(a2j2sy*t58+a2j2ry*t86+a2j2ry*
     & t88+a2j2syy*a2j2ry*t3+a2j2ryy*t12+a2j2sy*t62)*a2j2ryrrrs+(
     & a2j2ry*t104+a2j2syy*t12+a2j2ry*t113+a2j2sy*t88+a2j2sy*t86+
     & a2j2ryy*t26)*a2j2ryrrss+(a2j2ry*t124+a2j2sy*t104+a2j2syy*t26+
     & a2j2ryy*a2j2sy*t21+a2j2sy*t113+a2j2ry*t132)*a2j2ryrsss+(a2j2sy*
     & t132+a2j2syy*a2j2sy*t21+a2j2sy*t124)*a2j2ryssss+(4*a2j2ryy*
     & a2j2rxy*a2j2rx+a2j2ry*t151+a2j2ryyy*t3+a2j2ry*t155+a2j2ryy*t55+
     & a2j2ry*t159)*a2j2ryrrr+t195*a2j2ryrrs+t225*a2j2ryrss+(a2j2sy*
     & t214+a2j2syyy*t21+4*a2j2syy*a2j2sxy*a2j2sx+a2j2syy*t111+a2j2sy*
     & t223+a2j2sy*t205)*a2j2rysss+(a2j2ryyy*a2j2rxx+3*a2j2ry*
     & a2j2rxxyy+6*a2j2rxy*a2j2rxyy+2*a2j2rx*a2j2rxyyy+3*a2j2ryy*
     & a2j2rxxy)*a2j2ryrr+(a2j2ryyy*a2j2sxx+3*a2j2ry*a2j2sxxyy+3*
     & a2j2syy*a2j2rxxy+6*a2j2sxyy*a2j2rxy+a2j2syyy*a2j2rxx+2*a2j2sx*
     & a2j2rxyyy+3*a2j2sy*a2j2rxxyy+3*a2j2ryy*a2j2sxxy+6*a2j2sxy*
     & a2j2rxyy+2*a2j2sxyyy*a2j2rx)*a2j2ryrs+(3*a2j2syy*a2j2sxxy+2*
     & a2j2sx*a2j2sxyyy+a2j2syyy*a2j2sxx+3*a2j2sy*a2j2sxxyy+6*
     & a2j2sxyy*a2j2sxy)*a2j2ryss
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
      t188 = a2j2syyy*a2j2ry*a2j2rx+a2j2ryyy*t14+a2j2ry*t167+2*a2j2ryy*
     & t73+2*a2j2syy*t54+a2j2ryy*t85+a2j2ry*t178+a2j2syy*t61+a2j2sy*
     & t151+a2j2ry*t184+a2j2sy*t146+a2j2sy*t141
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
      a2j2ryxyyyy = t2*a2j2rx*a2j2ryrrrrr+(a2j2sy*t1*a2j2ry*a2j2rx+
     & a2j2ry*t18)*a2j2ryrrrrs+(a2j2ry*t27+a2j2sy*t18)*a2j2ryrrrss+(
     & a2j2ry*t36+a2j2sy*t27)*a2j2ryrrsss+(a2j2sy*t36+a2j2ry*t33*
     & a2j2sy*a2j2sx)*a2j2ryrssss+t47*a2j2sx*a2j2rysssss+(a2j2ryy*t1*
     & a2j2rx+a2j2ry*t58+a2j2ry*t63)*a2j2ryrrrr+(a2j2ry*t77+a2j2sy*
     & t58+a2j2syy*t1*a2j2rx+a2j2ry*t87+a2j2sy*t63+a2j2ryy*t16)*
     & a2j2ryrrrs+(a2j2syy*t16+a2j2ry*t106+a2j2ry*t108+a2j2sy*t77+
     & a2j2sy*t87+a2j2ryy*t25)*a2j2ryrrss+(a2j2syy*t25+a2j2sy*t108+
     & a2j2ry*t120+a2j2ry*t123+a2j2sy*t106+a2j2ryy*t33*a2j2sx)*
     & a2j2ryrsss+(a2j2syy*t33*a2j2sx+a2j2sy*t120+a2j2sy*t123)*
     & a2j2ryssss+(a2j2ry*t141+2*a2j2ryy*t54+a2j2ry*t146+a2j2ryyy*
     & a2j2ry*a2j2rx+a2j2ry*t151+a2j2ryy*t61)*a2j2ryrrr+t188*
     & a2j2ryrrs+t215*a2j2ryrss+(a2j2syyy*a2j2sy*a2j2sx+2*a2j2syy*
     & t102+a2j2sy*t200+a2j2sy*t203+a2j2syy*t98+a2j2sy*t208)*
     & a2j2rysss+(a2j2ryyyy*a2j2rx+4*a2j2ry*a2j2rxyyy+4*a2j2ryyy*
     & a2j2rxy+6*a2j2ryy*a2j2rxyy)*a2j2ryrr+(4*a2j2ryyy*a2j2sxy+
     & a2j2syyyy*a2j2rx+4*a2j2sy*a2j2rxyyy+6*a2j2syy*a2j2rxyy+4*
     & a2j2ry*a2j2sxyyy+4*a2j2syyy*a2j2rxy+6*a2j2ryy*a2j2sxyy+
     & a2j2ryyyy*a2j2sx)*a2j2ryrs+(4*a2j2sy*a2j2sxyyy+a2j2syyyy*
     & a2j2sx+4*a2j2syyy*a2j2sxy+6*a2j2syy*a2j2sxyy)*a2j2ryss
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
      t121 = 7*t30*a2j2ryy+a2j2sy*t89+a2j2ry*t102+a2j2ryy*t41+a2j2syyy*
     & t1+a2j2ry*t109+a2j2sy*t87+a2j2sy*t81+2*t96*a2j2ry+a2j2ry*t117+
     & 2*a2j2ryy*t32
      t129 = a2j2syyy*a2j2sy
      t131 = a2j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a2j2sy+a2j2sy*t109+2*a2j2syy*t32+a2j2sy*t117+
     & a2j2ry*t133+a2j2ry*t136+a2j2ry*t138+a2j2syy*t41+a2j2ryyy*t8+2*
     & t129*a2j2ry+a2j2sy*t102
      a2j2ryyyyyy = t2*a2j2ry*a2j2ryrrrrr+5*a2j2sy*t2*a2j2ryrrrrs+10*
     & t8*t9*a2j2ryrrrss+10*t13*t1*a2j2ryrrsss+5*t17*a2j2ry*
     & a2j2ryrssss+t17*a2j2sy*a2j2rysssss+10*t9*a2j2ryy*a2j2ryrrrr+(
     & 12*t26*t1+a2j2ry*t37+a2j2syy*t9+a2j2ry*t43)*a2j2ryrrrs+(3*
     & a2j2ryy*a2j2ry*t8+a2j2sy*t43+a2j2sy*t37+a2j2ry*t56+a2j2ry*t60+
     & 3*t29*a2j2sy)*a2j2ryrrss+(12*a2j2ry*t8*a2j2syy+a2j2sy*t56+
     & a2j2sy*t60+a2j2ryy*t13)*a2j2ryrsss+10*t13*a2j2syy*a2j2ryssss+(
     & a2j2ryyy*t1+a2j2ry*t81+7*t79*a2j2ry+a2j2ry*t87+a2j2ry*t89)*
     & a2j2ryrrr+t121*a2j2ryrrs+t145*a2j2ryrss+(a2j2sy*t138+a2j2sy*
     & t133+7*t131*a2j2sy+a2j2syyy*t8+a2j2sy*t136)*a2j2rysss+(5*
     & a2j2ry*a2j2ryyyy+10*a2j2ryyy*a2j2ryy)*a2j2ryrr+(10*a2j2syy*
     & a2j2ryyy+10*a2j2syyy*a2j2ryy+5*a2j2sy*a2j2ryyyy+5*a2j2syyyy*
     & a2j2ry)*a2j2ryrs+(10*a2j2syy*a2j2syyy+5*a2j2sy*a2j2syyyy)*
     & a2j2ryss
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
      a2j2syxxxxx = t2*a2j2rx*a2j2syrrrrr+5*a2j2sx*t2*a2j2syrrrrs+10*
     & t8*t9*a2j2syrrrss+10*t13*t1*a2j2syrrsss+5*t17*a2j2rx*
     & a2j2syrssss+t17*a2j2sx*a2j2sysssss+10*a2j2rxx*t9*a2j2syrrrr+(
     & 12*a2j2rxx*t1*a2j2sx+a2j2rx*t38+a2j2sxx*t9+a2j2rx*t44)*
     & a2j2syrrrs+(3*a2j2rxx*a2j2rx*t8+a2j2rx*t55+a2j2rx*t59+a2j2sx*
     & t44+3*t29*a2j2sx+a2j2sx*t38)*a2j2syrrss+(a2j2rxx*t13+12*t31*t8+
     & a2j2sx*t55+a2j2sx*t59)*a2j2syrsss+10*a2j2sxx*t13*a2j2syssss+(
     & a2j2rx*t81+7*t79*a2j2rx+a2j2rx*t86+a2j2rx*t88+a2j2rxxx*t1)*
     & a2j2syrrr+t121*a2j2syrrs+t145*a2j2syrss+(a2j2sx*t129+7*t127*
     & a2j2sx+a2j2sx*t136+a2j2sx*t133+a2j2sxxx*t8)*a2j2sysss+(10*
     & a2j2rxx*a2j2rxxx+5*a2j2rx*a2j2rxxxx)*a2j2syrr+(5*a2j2sxxxx*
     & a2j2rx+10*a2j2sxx*a2j2rxxx+5*a2j2sx*a2j2rxxxx+10*a2j2sxxx*
     & a2j2rxx)*a2j2syrs+(5*a2j2sxxxx*a2j2sx+10*a2j2sxx*a2j2sxxx)*
     & a2j2syss
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
      a2j2syxxxxy = a2j2ry*t2*a2j2syrrrrr+(4*a2j2ry*t5*a2j2sx+a2j2sy*
     & t2)*a2j2syrrrrs+(4*a2j2sy*t5*a2j2sx+6*t15*t16)*a2j2syrrrss+(4*
     & a2j2ry*a2j2rx*t22+6*t25*t16)*a2j2syrrsss+(a2j2ry*t30+4*a2j2sy*
     & a2j2rx*t22)*a2j2syrssss+a2j2sy*t30*a2j2sysssss+(4*t5*a2j2rxy+6*
     & t15*a2j2rxx)*a2j2syrrrr+(6*t25*a2j2rxx+a2j2rx*t55+6*t47*t1+
     & a2j2sxy*t5+a2j2ry*t71)*a2j2syrrrs+(3*t50*a2j2sx+a2j2rx*t81+
     & a2j2sy*t71+a2j2ry*t89+3*a2j2rxy*a2j2rx*t16+a2j2sx*t55)*
     & a2j2syrrss+(a2j2sy*t89+6*t51*t16+a2j2sx*t81+a2j2rxy*t22+6*
     & a2j2ry*t16*a2j2sxx)*a2j2syrsss+(4*t22*a2j2sxy+6*a2j2sy*t16*
     & a2j2sxx)*a2j2syssss+(a2j2rxxy*t1+7*t115*a2j2rx+a2j2rx*t120+
     & a2j2rx*t122+a2j2ry*t128)*a2j2syrrr+t162*a2j2syrrs+t190*
     & a2j2syrss+(a2j2sx*t174+a2j2sxxy*t16+7*a2j2sxy*a2j2sx*a2j2sxx+
     & a2j2sx*t179+a2j2sy*t185)*a2j2sysss+(a2j2ry*a2j2rxxxx+4*
     & a2j2rxxx*a2j2rxy+6*a2j2rxxy*a2j2rxx+4*a2j2rxxxy*a2j2rx)*
     & a2j2syrr+(4*a2j2sxxx*a2j2rxy+a2j2ry*a2j2sxxxx+4*a2j2sxxxy*
     & a2j2rx+4*a2j2sxy*a2j2rxxx+4*a2j2sx*a2j2rxxxy+6*a2j2sxx*
     & a2j2rxxy+a2j2sy*a2j2rxxxx+6*a2j2sxxy*a2j2rxx)*a2j2syrs+(4*
     & a2j2sxxx*a2j2sxy+4*a2j2sxxxy*a2j2sx+6*a2j2sxx*a2j2sxxy+a2j2sy*
     & a2j2sxxxx)*a2j2syss
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
      t219 = a2j2sy*t184+a2j2sx*t175+a2j2rxyy*t17+2*a2j2sxy*t61+a2j2ry*
     & t198+3*a2j2ryy*a2j2sxx*a2j2sx+a2j2ry*t206+a2j2rx*t211+2*t208*
     & a2j2rx+a2j2sy*t165+4*t152*a2j2sx+a2j2syy*t71
      a2j2syxxxyy = t1*t3*a2j2syrrrrr+(t6*t3+a2j2ry*t12)*a2j2syrrrrs+(
     & a2j2ry*t22+a2j2sy*t12)*a2j2syrrrss+(a2j2ry*t32+a2j2sy*t22)*
     & a2j2syrrsss+(t6*t30+a2j2sy*t32)*a2j2syrssss+t41*t30*
     & a2j2sysssss+(a2j2ry*t47+3*a2j2ry*a2j2rxy*t2+a2j2ryy*t3)*
     & a2j2syrrrr+(a2j2ry*t63+3*a2j2sy*a2j2rxy*t2+a2j2ry*t75+a2j2syy*
     & t3+a2j2sy*t47+3*a2j2ryy*t2*a2j2sx)*a2j2syrrrs+(a2j2sy*t75+
     & a2j2ry*t89+a2j2ry*t95+a2j2sy*t63+3*a2j2syy*t2*a2j2sx+3*t101*
     & t17)*a2j2syrrss+(3*t106*t17+a2j2sy*t89+3*a2j2ry*t17*a2j2sxy+
     & a2j2sy*t95+a2j2ry*t118+a2j2ryy*t30)*a2j2syrsss+(3*a2j2sy*t17*
     & a2j2sxy+a2j2syy*t30+a2j2sy*t118)*a2j2syssss+(a2j2ry*t133+
     & a2j2rxyy*t2+a2j2ry*t139+4*t141*a2j2rx+a2j2rx*t146+3*t101*
     & a2j2rxx)*a2j2syrrr+t188*a2j2syrrs+t219*a2j2syrss+(4*t209*
     & a2j2sx+a2j2sy*t206+a2j2sx*t211+3*a2j2syy*a2j2sxx*a2j2sx+a2j2sy*
     & t198+a2j2sxyy*t17)*a2j2sysss+(3*a2j2rx*a2j2rxxyy+6*a2j2rxy*
     & a2j2rxxy+3*a2j2rxx*a2j2rxyy+a2j2ryy*a2j2rxxx+2*a2j2ry*
     & a2j2rxxxy)*a2j2syrr+(3*a2j2sxxyy*a2j2rx+6*a2j2sxxy*a2j2rxy+3*
     & a2j2sxyy*a2j2rxx+3*a2j2sx*a2j2rxxyy+3*a2j2sxx*a2j2rxyy+2*
     & a2j2ry*a2j2sxxxy+6*a2j2sxy*a2j2rxxy+a2j2ryy*a2j2sxxx+2*a2j2sy*
     & a2j2rxxxy+a2j2syy*a2j2rxxx)*a2j2syrs+(3*a2j2sxxyy*a2j2sx+3*
     & a2j2sxyy*a2j2sxx+2*a2j2sy*a2j2sxxxy+a2j2syy*a2j2sxxx+6*a2j2sxy*
     & a2j2sxxy)*a2j2syss
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
      a2j2syxxyyy = t1*a2j2ry*t3*a2j2syrrrrr+(a2j2ry*t14+a2j2sy*t1*t3)*
     & a2j2syrrrrs+(a2j2ry*t28+a2j2sy*t14)*a2j2syrrrss+(a2j2sy*t28+
     & a2j2ry*t36)*a2j2syrrsss+(a2j2sy*t36+a2j2ry*t41*t21)*
     & a2j2syrssss+t41*a2j2sy*t21*a2j2sysssss+(a2j2ry*t58+a2j2ryy*
     & a2j2ry*t3+a2j2ry*t62)*a2j2syrrrr+(a2j2sy*t58+a2j2ry*t86+a2j2ry*
     & t88+a2j2syy*a2j2ry*t3+a2j2ryy*t12+a2j2sy*t62)*a2j2syrrrs+(
     & a2j2ry*t104+a2j2syy*t12+a2j2ry*t113+a2j2sy*t88+a2j2sy*t86+
     & a2j2ryy*t26)*a2j2syrrss+(a2j2ry*t124+a2j2sy*t104+a2j2syy*t26+
     & a2j2ryy*a2j2sy*t21+a2j2sy*t113+a2j2ry*t132)*a2j2syrsss+(a2j2sy*
     & t132+a2j2syy*a2j2sy*t21+a2j2sy*t124)*a2j2syssss+(4*a2j2ryy*
     & a2j2rxy*a2j2rx+a2j2ry*t151+a2j2ryyy*t3+a2j2ry*t155+a2j2ryy*t55+
     & a2j2ry*t159)*a2j2syrrr+t195*a2j2syrrs+t225*a2j2syrss+(a2j2sy*
     & t214+a2j2syyy*t21+4*a2j2syy*a2j2sxy*a2j2sx+a2j2syy*t111+a2j2sy*
     & t223+a2j2sy*t205)*a2j2sysss+(a2j2ryyy*a2j2rxx+3*a2j2ry*
     & a2j2rxxyy+6*a2j2rxy*a2j2rxyy+2*a2j2rx*a2j2rxyyy+3*a2j2ryy*
     & a2j2rxxy)*a2j2syrr+(a2j2ryyy*a2j2sxx+3*a2j2ry*a2j2sxxyy+3*
     & a2j2syy*a2j2rxxy+6*a2j2sxyy*a2j2rxy+a2j2syyy*a2j2rxx+2*a2j2sx*
     & a2j2rxyyy+3*a2j2sy*a2j2rxxyy+3*a2j2ryy*a2j2sxxy+6*a2j2sxy*
     & a2j2rxyy+2*a2j2sxyyy*a2j2rx)*a2j2syrs+(3*a2j2syy*a2j2sxxy+2*
     & a2j2sx*a2j2sxyyy+a2j2syyy*a2j2sxx+3*a2j2sy*a2j2sxxyy+6*
     & a2j2sxyy*a2j2sxy)*a2j2syss
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
      t188 = a2j2syyy*a2j2ry*a2j2rx+a2j2ryyy*t14+a2j2ry*t167+2*a2j2ryy*
     & t73+2*a2j2syy*t54+a2j2ryy*t85+a2j2ry*t178+a2j2syy*t61+a2j2sy*
     & t151+a2j2ry*t184+a2j2sy*t146+a2j2sy*t141
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
      a2j2syxyyyy = t2*a2j2rx*a2j2syrrrrr+(a2j2sy*t1*a2j2ry*a2j2rx+
     & a2j2ry*t18)*a2j2syrrrrs+(a2j2ry*t27+a2j2sy*t18)*a2j2syrrrss+(
     & a2j2ry*t36+a2j2sy*t27)*a2j2syrrsss+(a2j2sy*t36+a2j2ry*t33*
     & a2j2sy*a2j2sx)*a2j2syrssss+t47*a2j2sx*a2j2sysssss+(a2j2ryy*t1*
     & a2j2rx+a2j2ry*t58+a2j2ry*t63)*a2j2syrrrr+(a2j2ry*t77+a2j2sy*
     & t58+a2j2syy*t1*a2j2rx+a2j2ry*t87+a2j2sy*t63+a2j2ryy*t16)*
     & a2j2syrrrs+(a2j2syy*t16+a2j2ry*t106+a2j2ry*t108+a2j2sy*t77+
     & a2j2sy*t87+a2j2ryy*t25)*a2j2syrrss+(a2j2syy*t25+a2j2sy*t108+
     & a2j2ry*t120+a2j2ry*t123+a2j2sy*t106+a2j2ryy*t33*a2j2sx)*
     & a2j2syrsss+(a2j2syy*t33*a2j2sx+a2j2sy*t120+a2j2sy*t123)*
     & a2j2syssss+(a2j2ry*t141+2*a2j2ryy*t54+a2j2ry*t146+a2j2ryyy*
     & a2j2ry*a2j2rx+a2j2ry*t151+a2j2ryy*t61)*a2j2syrrr+t188*
     & a2j2syrrs+t215*a2j2syrss+(a2j2syyy*a2j2sy*a2j2sx+2*a2j2syy*
     & t102+a2j2sy*t200+a2j2sy*t203+a2j2syy*t98+a2j2sy*t208)*
     & a2j2sysss+(a2j2ryyyy*a2j2rx+4*a2j2ry*a2j2rxyyy+4*a2j2ryyy*
     & a2j2rxy+6*a2j2ryy*a2j2rxyy)*a2j2syrr+(4*a2j2ryyy*a2j2sxy+
     & a2j2syyyy*a2j2rx+4*a2j2sy*a2j2rxyyy+6*a2j2syy*a2j2rxyy+4*
     & a2j2ry*a2j2sxyyy+4*a2j2syyy*a2j2rxy+6*a2j2ryy*a2j2sxyy+
     & a2j2ryyyy*a2j2sx)*a2j2syrs+(4*a2j2sy*a2j2sxyyy+a2j2syyyy*
     & a2j2sx+4*a2j2syyy*a2j2sxy+6*a2j2syy*a2j2sxyy)*a2j2syss
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
      t121 = 7*t30*a2j2ryy+a2j2sy*t89+a2j2ry*t102+a2j2ryy*t41+a2j2syyy*
     & t1+a2j2ry*t109+a2j2sy*t87+a2j2sy*t81+2*t96*a2j2ry+a2j2ry*t117+
     & 2*a2j2ryy*t32
      t129 = a2j2syyy*a2j2sy
      t131 = a2j2syy**2
      t133 = 4*t129+3*t131
      t135 = t129+t131
      t136 = 2*t135
      t138 = 3*t135
      t145 = 7*t100*a2j2sy+a2j2sy*t109+2*a2j2syy*t32+a2j2sy*t117+
     & a2j2ry*t133+a2j2ry*t136+a2j2ry*t138+a2j2syy*t41+a2j2ryyy*t8+2*
     & t129*a2j2ry+a2j2sy*t102
      a2j2syyyyyy = t2*a2j2ry*a2j2syrrrrr+5*a2j2sy*t2*a2j2syrrrrs+10*
     & t8*t9*a2j2syrrrss+10*t13*t1*a2j2syrrsss+5*t17*a2j2ry*
     & a2j2syrssss+t17*a2j2sy*a2j2sysssss+10*t9*a2j2ryy*a2j2syrrrr+(
     & 12*t26*t1+a2j2ry*t37+a2j2syy*t9+a2j2ry*t43)*a2j2syrrrs+(3*
     & a2j2ryy*a2j2ry*t8+a2j2sy*t43+a2j2sy*t37+a2j2ry*t56+a2j2ry*t60+
     & 3*t29*a2j2sy)*a2j2syrrss+(12*a2j2ry*t8*a2j2syy+a2j2sy*t56+
     & a2j2sy*t60+a2j2ryy*t13)*a2j2syrsss+10*t13*a2j2syy*a2j2syssss+(
     & a2j2ryyy*t1+a2j2ry*t81+7*t79*a2j2ry+a2j2ry*t87+a2j2ry*t89)*
     & a2j2syrrr+t121*a2j2syrrs+t145*a2j2syrss+(a2j2sy*t138+a2j2sy*
     & t133+7*t131*a2j2sy+a2j2syyy*t8+a2j2sy*t136)*a2j2sysss+(5*
     & a2j2ry*a2j2ryyyy+10*a2j2ryyy*a2j2ryy)*a2j2syrr+(10*a2j2syy*
     & a2j2ryyy+10*a2j2syyy*a2j2ryy+5*a2j2sy*a2j2ryyyy+5*a2j2syyyy*
     & a2j2ry)*a2j2syrs+(10*a2j2syy*a2j2syyy+5*a2j2sy*a2j2syyyy)*
     & a2j2syss
