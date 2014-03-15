c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX
#beginMacro declareDifferenceOrder2(u,OPTION)
#If #OPTION == "RX"
 real d12
 real d22
 real h12
 real h22

 real rxr2
 real rxs2
 real rxt2
 real rxrr2
 real rxss2
 real rxrs2
 real ryr2
 real rys2
 real ryt2
 real ryrr2
 real ryss2
 real ryrs2
 real rzr2
 real rzs2
 real rzt2
 real rzrr2
 real rzss2
 real rzrs2
 real sxr2
 real sxs2
 real sxt2
 real sxrr2
 real sxss2
 real sxrs2
 real syr2
 real sys2
 real syt2
 real syrr2
 real syss2
 real syrs2
 real szr2
 real szs2
 real szt2
 real szrr2
 real szss2
 real szrs2
 real txr2
 real txs2
 real txt2
 real txrr2
 real txss2
 real txrs2
 real tyr2
 real tys2
 real tyt2
 real tyrr2
 real tyss2
 real tyrs2
 real tzr2
 real tzs2
 real tzt2
 real tzrr2
 real tzss2
 real tzrs2
 real rxx21
 real rxx22
 real rxy22
 real rxx23
 real rxy23
 real rxz23
 real ryx22
 real ryy22
 real ryx23
 real ryy23
 real ryz23
 real rzx22
 real rzy22
 real rzx23
 real rzy23
 real rzz23
 real sxx22
 real sxy22
 real sxx23
 real sxy23
 real sxz23
 real syx22
 real syy22
 real syx23
 real syy23
 real syz23
 real szx22
 real szy22
 real szx23
 real szy23
 real szz23
 real txx22
 real txy22
 real txx23
 real txy23
 real txz23
 real tyx22
 real tyy22
 real tyx23
 real tyy23
 real tyz23
 real tzx22
 real tzy22
 real tzx23
 real tzy23
 real tzz23
#End
 real u ## r2
 real u ## s2
 real u ## t2
 real u ## rr2
 real u ## ss2
 real u ## rs2
 real u ## tt2
 real u ## rt2
 real u ## st2
 real u ## rrr2
 real u ## sss2
 real u ## ttt2
 real u ## x21
 real u ## y21
 real u ## z21
 real u ## x22
 real u ## y22
 real u ## z22
 real u ## x23
 real u ## y23
 real u ## z23
 real u ## xx21
 real u ## yy21
 real u ## xy21
 real u ## xz21
 real u ## yz21
 real u ## zz21
 real u ## laplacian21
 real u ## xx22
 real u ## yy22
 real u ## xy22
 real u ## xz22
 real u ## yz22
 real u ## zz22
 real u ## laplacian22
 real u ## xx23
 real u ## yy23
 real u ## zz23
 real u ## xy23
 real u ## xz23
 real u ## yz23
 real u ## laplacian23
 real u ## x23r
 real u ## y23r
 real u ## z23r
 real u ## xx23r
 real u ## yy23r
 real u ## xy23r
 real u ## zz23r
 real u ## xz23r
 real u ## yz23r
 real u ## x21r
 real u ## y21r
 real u ## z21r
 real u ## xx21r
 real u ## yy21r
 real u ## zz21r
 real u ## xy21r
 real u ## xz21r
 real u ## yz21r
 real u ## laplacian21r
 real u ## x22r
 real u ## y22r
 real u ## z22r
 real u ## xx22r
 real u ## yy22r
 real u ## zz22r
 real u ## xy22r
 real u ## xz22r
 real u ## yz22r
 real u ## laplacian22r
 real u ## laplacian23r

 real u ## xxx22r
 real u ## yyy22r
 real u ## xxy22r
 real u ## xyy22r
 real u ## xxxx22r
 real u ## yyyy22r
 real u ## xxyy22r

 real u ## xxx23r
 real u ## yyy23r
 real u ## zzz23r
 real u ## xxy23r
 real u ## xxz23r
 real u ## xyy23r
 real u ## yyz23r
 real u ## xzz23r
 real u ## yzz23r

 real u ## xxxx23r
 real u ## yyyy23r
 real u ## zzzz23r
 real u ## xxyy23r
 real u ## xxzz23r
 real u ## yyzz23r

 real u ## LapSq22r
 real u ## LapSq23r

#endMacro


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder2Components0(u,OPTION)

#If #OPTION == "RX"
d12(kd) = 1./(2.*dr(kd))
d22(kd) = 1./(dr(kd)**2)
#End

u ## r2(i1,i2,i3)=(u(i1+1,i2,i3)-u(i1-1,i2,i3))*d12(0)
u ## s2(i1,i2,i3)=(u(i1,i2+1,i3)-u(i1,i2-1,i3))*d12(1)
u ## t2(i1,i2,i3)=(u(i1,i2,i3+1)-u(i1,i2,i3-1))*d12(2)

u ## rr2(i1,i2,i3)=(-2.*u(i1,i2,i3)+(u(i1+1,i2,i3)+u(i1-1,i2,i3)) )*d22(0)
u ## ss2(i1,i2,i3)=(-2.*u(i1,i2,i3)+(u(i1,i2+1,i3)+u(i1,i2-1,i3)) )*d22(1)
u ## rs2(i1,i2,i3)=(u ## r2(i1,i2+1,i3)-u ## r2(i1,i2-1,i3))*d12(1)
u ## tt2(i1,i2,i3)=(-2.*u(i1,i2,i3)+(u(i1,i2,i3+1)+u(i1,i2,i3-1)) )*d22(2)
u ## rt2(i1,i2,i3)=(u ## r2(i1,i2,i3+1)-u ## r2(i1,i2,i3-1))*d12(2)
u ## st2(i1,i2,i3)=(u ## s2(i1,i2,i3+1)-u ## s2(i1,i2,i3-1))*d12(2)
u ## rrr2(i1,i2,i3)=(-2.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))+(u(i1+2,i2,i3)-u(i1-2,i2,i3)) )*d22(0)*d12(0)
u ## sss2(i1,i2,i3)=(-2.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))+(u(i1,i2+2,i3)-u(i1,i2-2,i3)) )*d22(1)*d12(1)
u ## ttt2(i1,i2,i3)=(-2.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))+(u(i1,i2,i3+2)-u(i1,i2,i3-2)) )*d22(2)*d12(2)

#If #OPTION == "RX"
rxr2(i1,i2,i3)=(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))*d12(0)
rxs2(i1,i2,i3)=(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))*d12(1)
rxt2(i1,i2,i3)=(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))*d12(2)
rxrr2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1+1,i2,i3)+rx(i1-1,i2,i3)) )*d22(0)
rxss2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1,i2+1,i3)+rx(i1,i2-1,i3)) )*d22(1)
rxrs2(i1,i2,i3)=(rx ## r2(i1,i2+1,i3)-rx ## r2(i1,i2-1,i3))*d12(1)
ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(0)
rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(1)
ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(2)
ryrr2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1+1,i2,i3)+ry(i1-1,i2,i3)) )*d22(0)
ryss2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1,i2+1,i3)+ry(i1,i2-1,i3)) )*d22(1)
ryrs2(i1,i2,i3)=(ry ## r2(i1,i2+1,i3)-ry ## r2(i1,i2-1,i3))*d12(1)
rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(0)
rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(1)
rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(2)
rzrr2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1+1,i2,i3)+rz(i1-1,i2,i3)) )*d22(0)
rzss2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1,i2+1,i3)+rz(i1,i2-1,i3)) )*d22(1)
rzrs2(i1,i2,i3)=(rz ## r2(i1,i2+1,i3)-rz ## r2(i1,i2-1,i3))*d12(1)
sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(0)
sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(1)
sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(2)
sxrr2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1+1,i2,i3)+sx(i1-1,i2,i3)) )*d22(0)
sxss2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1,i2+1,i3)+sx(i1,i2-1,i3)) )*d22(1)
sxrs2(i1,i2,i3)=(sx ## r2(i1,i2+1,i3)-sx ## r2(i1,i2-1,i3))*d12(1)
syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(0)
sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(1)
syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(2)
syrr2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1+1,i2,i3)+sy(i1-1,i2,i3)) )*d22(0)
syss2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1,i2+1,i3)+sy(i1,i2-1,i3)) )*d22(1)
syrs2(i1,i2,i3)=(sy ## r2(i1,i2+1,i3)-sy ## r2(i1,i2-1,i3))*d12(1)
szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(0)
szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(1)
szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(2)
szrr2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1+1,i2,i3)+sz(i1-1,i2,i3)) )*d22(0)
szss2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1,i2+1,i3)+sz(i1,i2-1,i3)) )*d22(1)
szrs2(i1,i2,i3)=(sz ## r2(i1,i2+1,i3)-sz ## r2(i1,i2-1,i3))*d12(1)
txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(0)
txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(1)
txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(2)
txrr2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1+1,i2,i3)+tx(i1-1,i2,i3)) )*d22(0)
txss2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1,i2+1,i3)+tx(i1,i2-1,i3)) )*d22(1)
txrs2(i1,i2,i3)=(tx ## r2(i1,i2+1,i3)-tx ## r2(i1,i2-1,i3))*d12(1)
tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(0)
tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(1)
tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(2)
tyrr2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1+1,i2,i3)+ty(i1-1,i2,i3)) )*d22(0)
tyss2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1,i2+1,i3)+ty(i1,i2-1,i3)) )*d22(1)
tyrs2(i1,i2,i3)=(ty ## r2(i1,i2+1,i3)-ty ## r2(i1,i2-1,i3))*d12(1)
tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(0)
tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(1)
tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(2)
tzrr2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1+1,i2,i3)+tz(i1-1,i2,i3)) )*d22(0)
tzss2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1,i2+1,i3)+tz(i1,i2-1,i3)) )*d22(1)
tzrs2(i1,i2,i3)=(tz ## r2(i1,i2+1,i3)-tz ## r2(i1,i2-1,i3))*d12(1)
#End

u ## x21(i1,i2,i3)= rx(i1,i2,i3)*u ## r2(i1,i2,i3)
u ## y21(i1,i2,i3)=0
u ## z21(i1,i2,i3)=0

u ## x22(i1,i2,i3)= rx(i1,i2,i3)*u ## r2(i1,i2,i3)+sx(i1,i2,i3)*u ## s2(i1,i2,i3)
u ## y22(i1,i2,i3)= ry(i1,i2,i3)*u ## r2(i1,i2,i3)+sy(i1,i2,i3)*u ## s2(i1,i2,i3)
u ## z22(i1,i2,i3)=0
u ## x23(i1,i2,i3)=rx(i1,i2,i3)*u ## r2(i1,i2,i3)+sx(i1,i2,i3)*u ## s2(i1,i2,i3)+tx(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## y23(i1,i2,i3)=ry(i1,i2,i3)*u ## r2(i1,i2,i3)+sy(i1,i2,i3)*u ## s2(i1,i2,i3)+ty(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## z23(i1,i2,i3)=rz(i1,i2,i3)*u ## r2(i1,i2,i3)+sz(i1,i2,i3)*u ## s2(i1,i2,i3)+tz(i1,i2,i3)*u ## t2(i1,i2,i3)

#If #OPTION == "RX"
rxx21(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)
rxx22(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(i1,i2,i3)
rxy22(i1,i2,i3)= ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(i1,i2,i3)
rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(i1,i2,i3)+tx(i1,i2,i3)*rxt2(i1,i2,i3)
rxy23(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(i1,i2,i3)+ty(i1,i2,i3)*rxt2(i1,i2,i3)
rxz23(i1,i2,i3)=rz(i1,i2,i3)*rxr2(i1,i2,i3)+sz(i1,i2,i3)*rxs2(i1,i2,i3)+tz(i1,i2,i3)*rxt2(i1,i2,i3)
ryx22(i1,i2,i3)= rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(i1,i2,i3)
ryy22(i1,i2,i3)= ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(i1,i2,i3)
ryx23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(i1,i2,i3)+tx(i1,i2,i3)*ryt2(i1,i2,i3)
ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(i1,i2,i3)+ty(i1,i2,i3)*ryt2(i1,i2,i3)
ryz23(i1,i2,i3)=rz(i1,i2,i3)*ryr2(i1,i2,i3)+sz(i1,i2,i3)*rys2(i1,i2,i3)+tz(i1,i2,i3)*ryt2(i1,i2,i3)
rzx22(i1,i2,i3)= rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(i1,i2,i3)
rzy22(i1,i2,i3)= ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(i1,i2,i3)
rzx23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(i1,i2,i3)+tx(i1,i2,i3)*rzt2(i1,i2,i3)
rzy23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(i1,i2,i3)+ty(i1,i2,i3)*rzt2(i1,i2,i3)
rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)+sz(i1,i2,i3)*rzs2(i1,i2,i3)+tz(i1,i2,i3)*rzt2(i1,i2,i3)
sxx22(i1,i2,i3)= rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(i1,i2,i3)
sxy22(i1,i2,i3)= ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(i1,i2,i3)
sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(i1,i2,i3)+tx(i1,i2,i3)*sxt2(i1,i2,i3)
sxy23(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(i1,i2,i3)+ty(i1,i2,i3)*sxt2(i1,i2,i3)
sxz23(i1,i2,i3)=rz(i1,i2,i3)*sxr2(i1,i2,i3)+sz(i1,i2,i3)*sxs2(i1,i2,i3)+tz(i1,i2,i3)*sxt2(i1,i2,i3)
syx22(i1,i2,i3)= rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(i1,i2,i3)
syy22(i1,i2,i3)= ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(i1,i2,i3)
syx23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(i1,i2,i3)+tx(i1,i2,i3)*syt2(i1,i2,i3)
syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(i1,i2,i3)+ty(i1,i2,i3)*syt2(i1,i2,i3)
syz23(i1,i2,i3)=rz(i1,i2,i3)*syr2(i1,i2,i3)+sz(i1,i2,i3)*sys2(i1,i2,i3)+tz(i1,i2,i3)*syt2(i1,i2,i3)
szx22(i1,i2,i3)= rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(i1,i2,i3)
szy22(i1,i2,i3)= ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(i1,i2,i3)
szx23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(i1,i2,i3)+tx(i1,i2,i3)*szt2(i1,i2,i3)
szy23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(i1,i2,i3)+ty(i1,i2,i3)*szt2(i1,i2,i3)
szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)+sz(i1,i2,i3)*szs2(i1,i2,i3)+tz(i1,i2,i3)*szt2(i1,i2,i3)
txx22(i1,i2,i3)= rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(i1,i2,i3)
txy22(i1,i2,i3)= ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(i1,i2,i3)
txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(i1,i2,i3)+tx(i1,i2,i3)*txt2(i1,i2,i3)
txy23(i1,i2,i3)=ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(i1,i2,i3)+ty(i1,i2,i3)*txt2(i1,i2,i3)
txz23(i1,i2,i3)=rz(i1,i2,i3)*txr2(i1,i2,i3)+sz(i1,i2,i3)*txs2(i1,i2,i3)+tz(i1,i2,i3)*txt2(i1,i2,i3)
tyx22(i1,i2,i3)= rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(i1,i2,i3)
tyy22(i1,i2,i3)= ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(i1,i2,i3)
tyx23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(i1,i2,i3)+tx(i1,i2,i3)*tyt2(i1,i2,i3)
tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(i1,i2,i3)+ty(i1,i2,i3)*tyt2(i1,i2,i3)
tyz23(i1,i2,i3)=rz(i1,i2,i3)*tyr2(i1,i2,i3)+sz(i1,i2,i3)*tys2(i1,i2,i3)+tz(i1,i2,i3)*tyt2(i1,i2,i3)
tzx22(i1,i2,i3)= rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(i1,i2,i3)
tzy22(i1,i2,i3)= ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(i1,i2,i3)
tzx23(i1,i2,i3)=rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(i1,i2,i3)+tx(i1,i2,i3)*tzt2(i1,i2,i3)
tzy23(i1,i2,i3)=ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(i1,i2,i3)+ty(i1,i2,i3)*tzt2(i1,i2,i3)
tzz23(i1,i2,i3)=rz(i1,i2,i3)*tzr2(i1,i2,i3)+sz(i1,i2,i3)*tzs2(i1,i2,i3)+tz(i1,i2,i3)*tzt2(i1,i2,i3)
#End

u ## xx21(i1,i2,i3)=(rx(i1,i2,i3)**2)*u ## rr2(i1,i2,i3)+(rxx22(i1,i2,i3))*u ## r2(i1,i2,i3)
u ## yy21(i1,i2,i3)=0
u ## xy21(i1,i2,i3)=0
u ## xz21(i1,i2,i3)=0
u ## yz21(i1,i2,i3)=0
u ## zz21(i1,i2,i3)=0
u ## laplacian21(i1,i2,i3)=u ## xx21(i1,i2,i3)
u ## xx22(i1,i2,i3)=(rx(i1,i2,i3)**2)*u ## rr2(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3)+(sx(i1,i2,i3)**2)*u ## ss2(i1,i2,i3)+(rxx22(i1,i2,i3))*u ## r2(i1,i2,i3)+(sxx22(i1,i2,i3))*u ## s2(i1,i2,i3)
u ## yy22(i1,i2,i3)=(ry(i1,i2,i3)**2)*u ## rr2(i1,i2,i3)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs2(i1,i2,i3)+(sy(i1,i2,i3)**2)*u ## ss2(i1,i2,i3)+(ryy22(i1,i2,i3))*u ## r2(i1,i2,i3)+(syy22(i1,i2,i3))*u ## s2(i1,i2,i3)
u ## xy22(i1,i2,i3)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr2(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss2(i1,i2,i3)+rxy22(i1,i2,i3)*u ## r2(i1,i2,i3)+sxy22(i1,i2,i3)*u ## s2(i1,i2,i3)
u ## xz22(i1,i2,i3)=0
u ## yz22(i1,i2,i3)=0
u ## zz22(i1,i2,i3)=0
u ## laplacian22(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*u ## rr2(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs2(i1,i2,i3)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*u ## ss2(i1,i2,i3)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*u ## r2(i1,i2,i3)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*u ## s2(i1,i2,i3)
u ## xx23(i1,i2,i3)=rx(i1,i2,i3)**2*u ## rr2(i1,i2,i3)+sx(i1,i2,i3)**2*u ## ss2(i1,i2,i3)+tx(i1,i2,i3)**2*u ## tt2(i1,i2,i3)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs2(i1,i2,i3)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt2(i1,i2,i3)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st2(i1,i2,i3)+rxx23(i1,i2,i3)*u ## r2(i1,i2,i3)+sxx23(i1,i2,i3)*u ## s2(i1,i2,i3)+txx23(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## yy23(i1,i2,i3)=ry(i1,i2,i3)**2*u ## rr2(i1,i2,i3)+sy(i1,i2,i3)**2*u ## ss2(i1,i2,i3)+ty(i1,i2,i3)**2*u ## tt2(i1,i2,i3)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs2(i1,i2,i3)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt2(i1,i2,i3)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st2(i1,i2,i3)+ryy23(i1,i2,i3)*u ## r2(i1,i2,i3)+syy23(i1,i2,i3)*u ## s2(i1,i2,i3)+tyy23(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## zz23(i1,i2,i3)=rz(i1,i2,i3)**2*u ## rr2(i1,i2,i3)+sz(i1,i2,i3)**2*u ## ss2(i1,i2,i3)+tz(i1,i2,i3)**2*u ## tt2(i1,i2,i3)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs2(i1,i2,i3)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt2(i1,i2,i3)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st2(i1,i2,i3)+rzz23(i1,i2,i3)*u ## r2(i1,i2,i3)+szz23(i1,i2,i3)*u ## s2(i1,i2,i3)+tzz23(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## xy23(i1,i2,i3)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr2(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss2(i1,i2,i3)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt2(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt2(i1,i2,i3)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st2(i1,i2,i3)+rxy23(i1,i2,i3)*u ## r2(i1,i2,i3)+sxy23(i1,i2,i3)*u ## s2(i1,i2,i3)+txy23(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## xz23(i1,i2,i3)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr2(i1,i2,i3)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss2(i1,i2,i3)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt2(i1,i2,i3)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt2(i1,i2,i3)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st2(i1,i2,i3)+rxz23(i1,i2,i3)*u ## r2(i1,i2,i3)+sxz23(i1,i2,i3)*u ## s2(i1,i2,i3)+txz23(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## yz23(i1,i2,i3)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr2(i1,i2,i3)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss2(i1,i2,i3)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt2(i1,i2,i3)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs2(i1,i2,i3)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt2(i1,i2,i3)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st2(i1,i2,i3)+ryz23(i1,i2,i3)*u ## r2(i1,i2,i3)+syz23(i1,i2,i3)*u ## s2(i1,i2,i3)+tyz23(i1,i2,i3)*u ## t2(i1,i2,i3)
u ## laplacian23(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*u ## rr2(i1,i2,i3)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*u ## ss2(i1,i2,i3)+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*u ## tt2(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs2(i1,i2,i3)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt2(i1,i2,i3)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st2(i1,i2,i3)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*u ## r2(i1,i2,i3)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*u ## s2(i1,i2,i3)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*u ## t2(i1,i2,i3)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
#If #OPTION == "RX"
h12(kd) = 1./(2.*dx(kd))
h22(kd) = 1./(dx(kd)**2)
#End

u ## x23r(i1,i2,i3)=(u(i1+1,i2,i3)-u(i1-1,i2,i3))*h12(0)
u ## y23r(i1,i2,i3)=(u(i1,i2+1,i3)-u(i1,i2-1,i3))*h12(1)
u ## z23r(i1,i2,i3)=(u(i1,i2,i3+1)-u(i1,i2,i3-1))*h12(2)

u ## xx23r(i1,i2,i3)=(-2.*u(i1,i2,i3)+(u(i1+1,i2,i3)+u(i1-1,i2,i3)) )*h22(0)
u ## yy23r(i1,i2,i3)=(-2.*u(i1,i2,i3)+(u(i1,i2+1,i3)+u(i1,i2-1,i3)) )*h22(1)
u ## xy23r(i1,i2,i3)=(u ## x23r(i1,i2+1,i3)-u ## x23r(i1,i2-1,i3))*h12(1)
u ## zz23r(i1,i2,i3)=(-2.*u(i1,i2,i3)+(u(i1,i2,i3+1)+u(i1,i2,i3-1)) )*h22(2)
u ## xz23r(i1,i2,i3)=(u ## x23r(i1,i2,i3+1)-u ## x23r(i1,i2,i3-1))*h12(2)
u ## yz23r(i1,i2,i3)=(u ## y23r(i1,i2,i3+1)-u ## y23r(i1,i2,i3-1))*h12(2)

u ## x21r(i1,i2,i3)= u ## x23r(i1,i2,i3)
u ## y21r(i1,i2,i3)= u ## y23r(i1,i2,i3)
u ## z21r(i1,i2,i3)= u ## z23r(i1,i2,i3)
u ## xx21r(i1,i2,i3)= u ## xx23r(i1,i2,i3)
u ## yy21r(i1,i2,i3)= u ## yy23r(i1,i2,i3)
u ## zz21r(i1,i2,i3)= u ## zz23r(i1,i2,i3)
u ## xy21r(i1,i2,i3)= u ## xy23r(i1,i2,i3)
u ## xz21r(i1,i2,i3)= u ## xz23r(i1,i2,i3)
u ## yz21r(i1,i2,i3)= u ## yz23r(i1,i2,i3)
u ## laplacian21r(i1,i2,i3)=u ## xx23r(i1,i2,i3)
u ## x22r(i1,i2,i3)= u ## x23r(i1,i2,i3)
u ## y22r(i1,i2,i3)= u ## y23r(i1,i2,i3)
u ## z22r(i1,i2,i3)= u ## z23r(i1,i2,i3)
u ## xx22r(i1,i2,i3)= u ## xx23r(i1,i2,i3)
u ## yy22r(i1,i2,i3)= u ## yy23r(i1,i2,i3)
u ## zz22r(i1,i2,i3)= u ## zz23r(i1,i2,i3)
u ## xy22r(i1,i2,i3)= u ## xy23r(i1,i2,i3)
u ## xz22r(i1,i2,i3)= u ## xz23r(i1,i2,i3)
u ## yz22r(i1,i2,i3)= u ## yz23r(i1,i2,i3)
u ## laplacian22r(i1,i2,i3)=u ## xx23r(i1,i2,i3)+u ## yy23r(i1,i2,i3)
u ## laplacian23r(i1,i2,i3)=u ## xx23r(i1,i2,i3)+u ## yy23r(i1,i2,i3)+u ## zz23r(i1,i2,i3)

u ## xxx22r(i1,i2,i3)=(-2.*(u ## (i1+1,i2,i3)-u ## (i1-1,i2,i3))+(u ## (i1+2,i2,i3)-u ## (i1-2,i2,i3)) )*h22(0)*h12(0)
u ## yyy22r(i1,i2,i3)=(-2.*(u ## (i1,i2+1,i3)-u ## (i1,i2-1,i3))+(u ## (i1,i2+2,i3)-u ## (i1,i2-2,i3)) )*h22(1)*h12(1)

u ## xxy22r(i1,i2,i3,kd)=( u ## xx22r(i1,i2+1,i3)-u ## xx22r(i1,i2-1,i3))/(2.*dx(1))
u ## xyy22r(i1,i2,i3,kd)=( u ## yy22r(i1+1,i2,i3)-u ## yy22r(i1-1,i2,i3))/(2.*dx(0))

u ## xxxx22r(i1,i2,i3)=(6.*u ## (i1,i2,i3)-4.*(u ## (i1+1,i2,i3)+u ## (i1-1,i2,i3))\
                        +(u ## (i1+2,i2,i3)+u ## (i1-2,i2,i3)) )/(dx(0)**4)

u ## yyyy22r(i1,i2,i3)=(6.*u ## (i1,i2,i3)-4.*(u ## (i1,i2+1,i3)+u ## (i1,i2-1,i3))\
                        +(u ## (i1,i2+2,i3)+u ## (i1,i2-2,i3)) )/(dx(1)**4)

u ## xxyy22r(i1,i2,i3)=( 4.*u ## (i1,i2,i3)     \
   -2.*(u ## (i1+1,i2,i3)+u ## (i1-1,i2,i3)+u ## (i1,i2+1,i3)+u ## (i1,i2-1,i3))   \
   +   (u ## (i1+1,i2+1,i3)+u ## (i1-1,i2+1,i3)+u ## (i1+1,i2-1,i3)+u ## (i1-1,i2-1,i3)) )/(dx(0)**2*dx(1)**2)

! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
u ## LapSq22r(i1,i2,i3)= ( 6.*u ## (i1,i2,i3)   \
  - 4.*(u ## (i1+1,i2,i3)+u ## (i1-1,i2,i3))    \
      +(u ## (i1+2,i2,i3)+u ## (i1-2,i2,i3)) )/(dx(0)**4) \
  +( 6.*u ## (i1,i2,i3)    \
   -4.*(u ## (i1,i2+1,i3)+u ## (i1,i2-1,i3))    \
      +(u ## (i1,i2+2,i3)+u ## (i1,i2-2,i3)) )/(dx(1)**4)  \
  +( 8.*u ## (i1,i2,i3)     \
   -4.*(u ## (i1+1,i2,i3)+u ## (i1-1,i2,i3)+u ## (i1,i2+1,i3)+u ## (i1,i2-1,i3))   \
   +2.*(u ## (i1+1,i2+1,i3)+u ## (i1-1,i2+1,i3)+u ## (i1+1,i2-1,i3)+u ## (i1-1,i2-1,i3)) )/(dx(0)**2*dx(1)**2)

u ## xxx23r(i1,i2,i3)=(-2.*(u ## (i1+1,i2,i3)-u ## (i1-1,i2,i3))+(u ## (i1+2,i2,i3)-u ## (i1-2,i2,i3)) )*h22(0)*h12(0)
u ## yyy23r(i1,i2,i3)=(-2.*(u ## (i1,i2+1,i3)-u ## (i1,i2-1,i3))+(u ## (i1,i2+2,i3)-u ## (i1,i2-2,i3)) )*h22(1)*h12(1)
u ## zzz23r(i1,i2,i3)=(-2.*(u ## (i1,i2,i3+1)-u ## (i1,i2,i3-1))+(u ## (i1,i2,i3+2)-u ## (i1,i2,i3-2)) )*h22(1)*h12(2)

u ## xxy23r(i1,i2,i3)=( u ## xx22r(i1,i2+1,i3)-u ## xx22r(i1,i2-1,i3))/(2.*dx(1))
u ## xyy23r(i1,i2,i3)=( u ## yy22r(i1+1,i2,i3)-u ## yy22r(i1-1,i2,i3))/(2.*dx(0))
u ## xxz23r(i1,i2,i3)=( u ## xx22r(i1,i2,i3+1)-u ## xx22r(i1,i2,i3-1))/(2.*dx(2))
u ## yyz23r(i1,i2,i3)=( u ## yy22r(i1,i2,i3+1)-u ## yy22r(i1,i2,i3-1))/(2.*dx(2))

u ## xzz23r(i1,i2,i3)=( u ## zz22r(i1+1,i2,i3)-u ## zz22r(i1-1,i2,i3))/(2.*dx(0))
u ## yzz23r(i1,i2,i3)=( u ## zz22r(i1,i2+1,i3)-u ## zz22r(i1,i2-1,i3))/(2.*dx(1))

u ## xxxx23r(i1,i2,i3)=(6.*u ## (i1,i2,i3)-4.*(u ## (i1+1,i2,i3)+u ## (i1-1,i2,i3))\
                        +(u ## (i1+2,i2,i3)+u ## (i1-2,i2,i3)) )/(dx(0)**4)

u ## yyyy23r(i1,i2,i3)=(6.*u ## (i1,i2,i3)-4.*(u ## (i1,i2+1,i3)+u ## (i1,i2-1,i3))\
                        +(u ## (i1,i2+2,i3)+u ## (i1,i2-2,i3)) )/(dx(1)**4)
u ## zzzz23r(i1,i2,i3)=(6.*u ## (i1,i2,i3)-4.*(u ## (i1,i2,i3+1)+u ## (i1,i2,i3-1))\
                        +(u ## (i1,i2,i3+2)+u ## (i1,i2,i3-2)) )/(dx(2)**4)

u ## xxyy23r(i1,i2,i3)=( 4.*u ## (i1,i2,i3)     \
   -2.*(u ## (i1+1,i2,i3)+u ## (i1-1,i2,i3)+u ## (i1,i2+1,i3)+u ## (i1,i2-1,i3))   \
   +   (u ## (i1+1,i2+1,i3)+u ## (i1-1,i2+1,i3)+u ## (i1+1,i2-1,i3)+u ## (i1-1,i2-1,i3)) )/(dx(0)**2*dx(1)**2)

u ## xxzz23r(i1,i2,i3)=( 4.*u ## (i1,i2,i3)     \
   -2.*(u ## (i1+1,i2,i3)+u ## (i1-1,i2,i3)+u ## (i1,i2,i3+1)+u ## (i1,i2,i3-1))   \
   +   (u ## (i1+1,i2,i3+1)+u ## (i1-1,i2,i3+1)+u ## (i1+1,i2,i3-1)+u ## (i1-1,i2,i3-1)) )/(dx(0)**2*dx(2)**2)

u ## yyzz23r(i1,i2,i3)=( 4.*u ## (i1,i2,i3)     \
   -2.*(u ## (i1,i2+1,i3)  +u ## (i1,i2-1,i3)+  u ## (i1,i2  ,i3+1)+u ## (i1,i2  ,i3-1))   \
   +   (u ## (i1,i2+1,i3+1)+u ## (i1,i2-1,i3+1)+u ## (i1,i2+1,i3-1)+u ## (i1,i2-1,i3-1)) )/(dx(1)**2*dx(2)**2)

#endMacro
c To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder2Components1(u,OPTION)

#If #OPTION == "RX"
d12(kd) = 1./(2.*dr(kd))
d22(kd) = 1./(dr(kd)**2)
#End

u ## r2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*d12(0)
u ## s2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*d12(1)
u ## t2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*d12(2)

u ## rr2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd)) )*d22(0)
u ## ss2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd)) )*d22(1)
u ## rs2(i1,i2,i3,kd)=(u ## r2(i1,i2+1,i3,kd)-u ## r2(i1,i2-1,i3,kd))*d12(1)
u ## tt2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd)) )*d22(2)
u ## rt2(i1,i2,i3,kd)=(u ## r2(i1,i2,i3+1,kd)-u ## r2(i1,i2,i3-1,kd))*d12(2)
u ## st2(i1,i2,i3,kd)=(u ## s2(i1,i2,i3+1,kd)-u ## s2(i1,i2,i3-1,kd))*d12(2)
u ## rrr2(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
u ## sss2(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
u ## ttt2(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*d22(2)*d12(2)

#If #OPTION == "RX"
rxr2(i1,i2,i3)=(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))*d12(0)
rxs2(i1,i2,i3)=(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))*d12(1)
rxt2(i1,i2,i3)=(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))*d12(2)
rxrr2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1+1,i2,i3)+rx(i1-1,i2,i3)) )*d22(0)
rxss2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1,i2+1,i3)+rx(i1,i2-1,i3)) )*d22(1)
rxrs2(i1,i2,i3)=(rx ## r2(i1,i2+1,i3)-rx ## r2(i1,i2-1,i3))*d12(1)
ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(0)
rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(1)
ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(2)
ryrr2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1+1,i2,i3)+ry(i1-1,i2,i3)) )*d22(0)
ryss2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1,i2+1,i3)+ry(i1,i2-1,i3)) )*d22(1)
ryrs2(i1,i2,i3)=(ry ## r2(i1,i2+1,i3)-ry ## r2(i1,i2-1,i3))*d12(1)
rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(0)
rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(1)
rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(2)
rzrr2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1+1,i2,i3)+rz(i1-1,i2,i3)) )*d22(0)
rzss2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1,i2+1,i3)+rz(i1,i2-1,i3)) )*d22(1)
rzrs2(i1,i2,i3)=(rz ## r2(i1,i2+1,i3)-rz ## r2(i1,i2-1,i3))*d12(1)
sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(0)
sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(1)
sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(2)
sxrr2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1+1,i2,i3)+sx(i1-1,i2,i3)) )*d22(0)
sxss2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1,i2+1,i3)+sx(i1,i2-1,i3)) )*d22(1)
sxrs2(i1,i2,i3)=(sx ## r2(i1,i2+1,i3)-sx ## r2(i1,i2-1,i3))*d12(1)
syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(0)
sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(1)
syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(2)
syrr2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1+1,i2,i3)+sy(i1-1,i2,i3)) )*d22(0)
syss2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1,i2+1,i3)+sy(i1,i2-1,i3)) )*d22(1)
syrs2(i1,i2,i3)=(sy ## r2(i1,i2+1,i3)-sy ## r2(i1,i2-1,i3))*d12(1)
szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(0)
szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(1)
szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(2)
szrr2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1+1,i2,i3)+sz(i1-1,i2,i3)) )*d22(0)
szss2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1,i2+1,i3)+sz(i1,i2-1,i3)) )*d22(1)
szrs2(i1,i2,i3)=(sz ## r2(i1,i2+1,i3)-sz ## r2(i1,i2-1,i3))*d12(1)
txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(0)
txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(1)
txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(2)
txrr2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1+1,i2,i3)+tx(i1-1,i2,i3)) )*d22(0)
txss2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1,i2+1,i3)+tx(i1,i2-1,i3)) )*d22(1)
txrs2(i1,i2,i3)=(tx ## r2(i1,i2+1,i3)-tx ## r2(i1,i2-1,i3))*d12(1)
tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(0)
tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(1)
tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(2)
tyrr2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1+1,i2,i3)+ty(i1-1,i2,i3)) )*d22(0)
tyss2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1,i2+1,i3)+ty(i1,i2-1,i3)) )*d22(1)
tyrs2(i1,i2,i3)=(ty ## r2(i1,i2+1,i3)-ty ## r2(i1,i2-1,i3))*d12(1)
tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(0)
tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(1)
tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(2)
tzrr2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1+1,i2,i3)+tz(i1-1,i2,i3)) )*d22(0)
tzss2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1,i2+1,i3)+tz(i1,i2-1,i3)) )*d22(1)
tzrs2(i1,i2,i3)=(tz ## r2(i1,i2+1,i3)-tz ## r2(i1,i2-1,i3))*d12(1)
#End

u ## x21(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r2(i1,i2,i3,kd)
u ## y21(i1,i2,i3,kd)=0
u ## z21(i1,i2,i3,kd)=0

u ## x22(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s2(i1,i2,i3,kd)
u ## y22(i1,i2,i3,kd)= ry(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s2(i1,i2,i3,kd)
u ## z22(i1,i2,i3,kd)=0
u ## x23(i1,i2,i3,kd)=rx(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+tx(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## y23(i1,i2,i3,kd)=ry(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+ty(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## z23(i1,i2,i3,kd)=rz(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sz(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+tz(i1,i2,i3)*u ## t2(i1,i2,i3,kd)

#If #OPTION == "RX"
rxx21(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)
rxx22(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(i1,i2,i3)
rxy22(i1,i2,i3)= ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(i1,i2,i3)
rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(i1,i2,i3)+tx(i1,i2,i3)*rxt2(i1,i2,i3)
rxy23(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(i1,i2,i3)+ty(i1,i2,i3)*rxt2(i1,i2,i3)
rxz23(i1,i2,i3)=rz(i1,i2,i3)*rxr2(i1,i2,i3)+sz(i1,i2,i3)*rxs2(i1,i2,i3)+tz(i1,i2,i3)*rxt2(i1,i2,i3)
ryx22(i1,i2,i3)= rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(i1,i2,i3)
ryy22(i1,i2,i3)= ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(i1,i2,i3)
ryx23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(i1,i2,i3)+tx(i1,i2,i3)*ryt2(i1,i2,i3)
ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(i1,i2,i3)+ty(i1,i2,i3)*ryt2(i1,i2,i3)
ryz23(i1,i2,i3)=rz(i1,i2,i3)*ryr2(i1,i2,i3)+sz(i1,i2,i3)*rys2(i1,i2,i3)+tz(i1,i2,i3)*ryt2(i1,i2,i3)
rzx22(i1,i2,i3)= rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(i1,i2,i3)
rzy22(i1,i2,i3)= ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(i1,i2,i3)
rzx23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(i1,i2,i3)+tx(i1,i2,i3)*rzt2(i1,i2,i3)
rzy23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(i1,i2,i3)+ty(i1,i2,i3)*rzt2(i1,i2,i3)
rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)+sz(i1,i2,i3)*rzs2(i1,i2,i3)+tz(i1,i2,i3)*rzt2(i1,i2,i3)
sxx22(i1,i2,i3)= rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(i1,i2,i3)
sxy22(i1,i2,i3)= ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(i1,i2,i3)
sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(i1,i2,i3)+tx(i1,i2,i3)*sxt2(i1,i2,i3)
sxy23(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(i1,i2,i3)+ty(i1,i2,i3)*sxt2(i1,i2,i3)
sxz23(i1,i2,i3)=rz(i1,i2,i3)*sxr2(i1,i2,i3)+sz(i1,i2,i3)*sxs2(i1,i2,i3)+tz(i1,i2,i3)*sxt2(i1,i2,i3)
syx22(i1,i2,i3)= rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(i1,i2,i3)
syy22(i1,i2,i3)= ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(i1,i2,i3)
syx23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(i1,i2,i3)+tx(i1,i2,i3)*syt2(i1,i2,i3)
syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(i1,i2,i3)+ty(i1,i2,i3)*syt2(i1,i2,i3)
syz23(i1,i2,i3)=rz(i1,i2,i3)*syr2(i1,i2,i3)+sz(i1,i2,i3)*sys2(i1,i2,i3)+tz(i1,i2,i3)*syt2(i1,i2,i3)
szx22(i1,i2,i3)= rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(i1,i2,i3)
szy22(i1,i2,i3)= ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(i1,i2,i3)
szx23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(i1,i2,i3)+tx(i1,i2,i3)*szt2(i1,i2,i3)
szy23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(i1,i2,i3)+ty(i1,i2,i3)*szt2(i1,i2,i3)
szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)+sz(i1,i2,i3)*szs2(i1,i2,i3)+tz(i1,i2,i3)*szt2(i1,i2,i3)
txx22(i1,i2,i3)= rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(i1,i2,i3)
txy22(i1,i2,i3)= ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(i1,i2,i3)
txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(i1,i2,i3)+tx(i1,i2,i3)*txt2(i1,i2,i3)
txy23(i1,i2,i3)=ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(i1,i2,i3)+ty(i1,i2,i3)*txt2(i1,i2,i3)
txz23(i1,i2,i3)=rz(i1,i2,i3)*txr2(i1,i2,i3)+sz(i1,i2,i3)*txs2(i1,i2,i3)+tz(i1,i2,i3)*txt2(i1,i2,i3)
tyx22(i1,i2,i3)= rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(i1,i2,i3)
tyy22(i1,i2,i3)= ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(i1,i2,i3)
tyx23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(i1,i2,i3)+tx(i1,i2,i3)*tyt2(i1,i2,i3)
tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(i1,i2,i3)+ty(i1,i2,i3)*tyt2(i1,i2,i3)
tyz23(i1,i2,i3)=rz(i1,i2,i3)*tyr2(i1,i2,i3)+sz(i1,i2,i3)*tys2(i1,i2,i3)+tz(i1,i2,i3)*tyt2(i1,i2,i3)
tzx22(i1,i2,i3)= rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(i1,i2,i3)
tzy22(i1,i2,i3)= ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(i1,i2,i3)
tzx23(i1,i2,i3)=rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(i1,i2,i3)+tx(i1,i2,i3)*tzt2(i1,i2,i3)
tzy23(i1,i2,i3)=ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(i1,i2,i3)+ty(i1,i2,i3)*tzt2(i1,i2,i3)
tzz23(i1,i2,i3)=rz(i1,i2,i3)*tzr2(i1,i2,i3)+sz(i1,i2,i3)*tzs2(i1,i2,i3)+tz(i1,i2,i3)*tzt2(i1,i2,i3)
#End

u ## xx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u ## rr2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*u ## r2(i1,i2,i3,kd)
u ## yy21(i1,i2,i3,kd)=0
u ## xy21(i1,i2,i3,kd)=0
u ## xz21(i1,i2,i3,kd)=0
u ## yz21(i1,i2,i3,kd)=0
u ## zz21(i1,i2,i3,kd)=0
u ## laplacian21(i1,i2,i3,kd)=u ## xx21(i1,i2,i3,kd)
u ## xx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u ## rr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*u ## ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*u ## r2(i1,i2,i3,kd)+(sxx22(i1,i2,i3))*u ## s2(i1,i2,i3,kd)
u ## yy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*u ## rr2(i1,i2,i3,kd)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*u ## ss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*u ## r2(i1,i2,i3,kd)+(syy22(i1,i2,i3))*u ## s2(i1,i2,i3,kd)
u ## xy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss2(i1,i2,i3,kd)+rxy22(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*u ## s2(i1,i2,i3,kd)
u ## xz22(i1,i2,i3,kd)=0
u ## yz22(i1,i2,i3,kd)=0
u ## zz22(i1,i2,i3,kd)=0
u ## laplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*u ## rr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*u ## ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*u ## r2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*u ## s2(i1,i2,i3,kd)
u ## xx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*u ## rr2(i1,i2,i3,kd)+sx(i1,i2,i3)**2*u ## ss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*u ## tt2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sxx23(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+txx23(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## yy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*u ## rr2(i1,i2,i3,kd)+sy(i1,i2,i3)**2*u ## ss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*u ## tt2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+syy23(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## zz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*u ## rr2(i1,i2,i3,kd)+sz(i1,i2,i3)**2*u ## ss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*u ## tt2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+szz23(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## xy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st2(i1,i2,i3,kd)+rxy23(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+txy23(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## xz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr2(i1,i2,i3,kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st2(i1,i2,i3,kd)+rxz23(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+txz23(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## yz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr2(i1,i2,i3,kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt2(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st2(i1,i2,i3,kd)+ryz23(i1,i2,i3)*u ## r2(i1,i2,i3,kd)+syz23(i1,i2,i3)*u ## s2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*u ## t2(i1,i2,i3,kd)
u ## laplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*u ## rr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*u ## ss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*u ## tt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*u ## r2(i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*u ## s2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*u ## t2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
#If #OPTION == "RX"
h12(kd) = 1./(2.*dx(kd))
h22(kd) = 1./(dx(kd)**2)
#End

u ## x23r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*h12(0)
u ## y23r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*h12(1)
u ## z23r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*h12(2)

u ## xx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd)) )*h22(0)
u ## yy23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd)) )*h22(1)
u ## xy23r(i1,i2,i3,kd)=(u ## x23r(i1,i2+1,i3,kd)-u ## x23r(i1,i2-1,i3,kd))*h12(1)
u ## zz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd)) )*h22(2)
u ## xz23r(i1,i2,i3,kd)=(u ## x23r(i1,i2,i3+1,kd)-u ## x23r(i1,i2,i3-1,kd))*h12(2)
u ## yz23r(i1,i2,i3,kd)=(u ## y23r(i1,i2,i3+1,kd)-u ## y23r(i1,i2,i3-1,kd))*h12(2)

u ## x21r(i1,i2,i3,kd)= u ## x23r(i1,i2,i3,kd)
u ## y21r(i1,i2,i3,kd)= u ## y23r(i1,i2,i3,kd)
u ## z21r(i1,i2,i3,kd)= u ## z23r(i1,i2,i3,kd)
u ## xx21r(i1,i2,i3,kd)= u ## xx23r(i1,i2,i3,kd)
u ## yy21r(i1,i2,i3,kd)= u ## yy23r(i1,i2,i3,kd)
u ## zz21r(i1,i2,i3,kd)= u ## zz23r(i1,i2,i3,kd)
u ## xy21r(i1,i2,i3,kd)= u ## xy23r(i1,i2,i3,kd)
u ## xz21r(i1,i2,i3,kd)= u ## xz23r(i1,i2,i3,kd)
u ## yz21r(i1,i2,i3,kd)= u ## yz23r(i1,i2,i3,kd)
u ## laplacian21r(i1,i2,i3,kd)=u ## xx23r(i1,i2,i3,kd)
u ## x22r(i1,i2,i3,kd)= u ## x23r(i1,i2,i3,kd)
u ## y22r(i1,i2,i3,kd)= u ## y23r(i1,i2,i3,kd)
u ## z22r(i1,i2,i3,kd)= u ## z23r(i1,i2,i3,kd)
u ## xx22r(i1,i2,i3,kd)= u ## xx23r(i1,i2,i3,kd)
u ## yy22r(i1,i2,i3,kd)= u ## yy23r(i1,i2,i3,kd)
u ## zz22r(i1,i2,i3,kd)= u ## zz23r(i1,i2,i3,kd)
u ## xy22r(i1,i2,i3,kd)= u ## xy23r(i1,i2,i3,kd)
u ## xz22r(i1,i2,i3,kd)= u ## xz23r(i1,i2,i3,kd)
u ## yz22r(i1,i2,i3,kd)= u ## yz23r(i1,i2,i3,kd)
u ## laplacian22r(i1,i2,i3,kd)=u ## xx23r(i1,i2,i3,kd)+u ## yy23r(i1,i2,i3,kd)
u ## laplacian23r(i1,i2,i3,kd)=u ## xx23r(i1,i2,i3,kd)+u ## yy23r(i1,i2,i3,kd)+u ## zz23r(i1,i2,i3,kd)

u ## xxx22r(i1,i2,i3,kd)=(-2.*(u ## (i1+1,i2,i3,kd)-u ## (i1-1,i2,i3,kd))+(u ## (i1+2,i2,i3,kd)-u ## (i1-2,i2,i3,kd)) )*h22(0)*h12(0)
u ## yyy22r(i1,i2,i3,kd)=(-2.*(u ## (i1,i2+1,i3,kd)-u ## (i1,i2-1,i3,kd))+(u ## (i1,i2+2,i3,kd)-u ## (i1,i2-2,i3,kd)) )*h22(1)*h12(1)

u ## xxy22r(i1,i2,i3,kd)=( u ## xx22r(i1,i2+1,i3,kd)-u ## xx22r(i1,i2-1,i3,kd))/(2.*dx(1))
u ## xyy22r(i1,i2,i3,kd)=( u ## yy22r(i1+1,i2,i3,kd)-u ## yy22r(i1-1,i2,i3,kd))/(2.*dx(0))

u ## xxxx22r(i1,i2,i3,kd)=(6.*u ## (i1,i2,i3,kd)-4.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd))\
                        +(u ## (i1+2,i2,i3,kd)+u ## (i1-2,i2,i3,kd)) )/(dx(0)**4)

u ## yyyy22r(i1,i2,i3,kd)=(6.*u ## (i1,i2,i3,kd)-4.*(u ## (i1,i2+1,i3,kd)+u ## (i1,i2-1,i3,kd))\
                        +(u ## (i1,i2+2,i3,kd)+u ## (i1,i2-2,i3,kd)) )/(dx(1)**4)

u ## xxyy22r(i1,i2,i3,kd)=( 4.*u ## (i1,i2,i3,kd)     \
   -2.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd)+u ## (i1,i2+1,i3,kd)+u ## (i1,i2-1,i3,kd))   \
   +   (u ## (i1+1,i2+1,i3,kd)+u ## (i1-1,i2+1,i3,kd)+u ## (i1+1,i2-1,i3,kd)+u ## (i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)

! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
u ## LapSq22r(i1,i2,i3,kd)= ( 6.*u ## (i1,i2,i3,kd)   \
  - 4.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd))    \
      +(u ## (i1+2,i2,i3,kd)+u ## (i1-2,i2,i3,kd)) )/(dx(0)**4) \
  +( 6.*u ## (i1,i2,i3,kd)    \
   -4.*(u ## (i1,i2+1,i3,kd)+u ## (i1,i2-1,i3,kd))    \
      +(u ## (i1,i2+2,i3,kd)+u ## (i1,i2-2,i3,kd)) )/(dx(1)**4)  \
  +( 8.*u ## (i1,i2,i3,kd)     \
   -4.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd)+u ## (i1,i2+1,i3,kd)+u ## (i1,i2-1,i3,kd))   \
   +2.*(u ## (i1+1,i2+1,i3,kd)+u ## (i1-1,i2+1,i3,kd)+u ## (i1+1,i2-1,i3,kd)+u ## (i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)

u ## xxx23r(i1,i2,i3,kd)=(-2.*(u ## (i1+1,i2,i3,kd)-u ## (i1-1,i2,i3,kd))+(u ## (i1+2,i2,i3,kd)-u ## (i1-2,i2,i3,kd)) )*h22(0)*h12(0)
u ## yyy23r(i1,i2,i3,kd)=(-2.*(u ## (i1,i2+1,i3,kd)-u ## (i1,i2-1,i3,kd))+(u ## (i1,i2+2,i3,kd)-u ## (i1,i2-2,i3,kd)) )*h22(1)*h12(1)
u ## zzz23r(i1,i2,i3,kd)=(-2.*(u ## (i1,i2,i3+1,kd)-u ## (i1,i2,i3-1,kd))+(u ## (i1,i2,i3+2,kd)-u ## (i1,i2,i3-2,kd)) )*h22(1)*h12(2)

u ## xxy23r(i1,i2,i3,kd)=( u ## xx22r(i1,i2+1,i3,kd)-u ## xx22r(i1,i2-1,i3,kd))/(2.*dx(1))
u ## xyy23r(i1,i2,i3,kd)=( u ## yy22r(i1+1,i2,i3,kd)-u ## yy22r(i1-1,i2,i3,kd))/(2.*dx(0))
u ## xxz23r(i1,i2,i3,kd)=( u ## xx22r(i1,i2,i3+1,kd)-u ## xx22r(i1,i2,i3-1,kd))/(2.*dx(2))
u ## yyz23r(i1,i2,i3,kd)=( u ## yy22r(i1,i2,i3+1,kd)-u ## yy22r(i1,i2,i3-1,kd))/(2.*dx(2))

u ## xzz23r(i1,i2,i3,kd)=( u ## zz22r(i1+1,i2,i3,kd)-u ## zz22r(i1-1,i2,i3,kd))/(2.*dx(0))
u ## yzz23r(i1,i2,i3,kd)=( u ## zz22r(i1,i2+1,i3,kd)-u ## zz22r(i1,i2-1,i3,kd))/(2.*dx(1))

u ## xxxx23r(i1,i2,i3,kd)=(6.*u ## (i1,i2,i3,kd)-4.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd))\
                        +(u ## (i1+2,i2,i3,kd)+u ## (i1-2,i2,i3,kd)) )/(dx(0)**4)

u ## yyyy23r(i1,i2,i3,kd)=(6.*u ## (i1,i2,i3,kd)-4.*(u ## (i1,i2+1,i3,kd)+u ## (i1,i2-1,i3,kd))\
                        +(u ## (i1,i2+2,i3,kd)+u ## (i1,i2-2,i3,kd)) )/(dx(1)**4)
u ## zzzz23r(i1,i2,i3,kd)=(6.*u ## (i1,i2,i3,kd)-4.*(u ## (i1,i2,i3+1,kd)+u ## (i1,i2,i3-1,kd))\
                        +(u ## (i1,i2,i3+2,kd)+u ## (i1,i2,i3-2,kd)) )/(dx(2)**4)

u ## xxyy23r(i1,i2,i3,kd)=( 4.*u ## (i1,i2,i3,kd)     \
   -2.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd)+u ## (i1,i2+1,i3,kd)+u ## (i1,i2-1,i3,kd))   \
   +   (u ## (i1+1,i2+1,i3,kd)+u ## (i1-1,i2+1,i3,kd)+u ## (i1+1,i2-1,i3,kd)+u ## (i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)

u ## xxzz23r(i1,i2,i3,kd)=( 4.*u ## (i1,i2,i3,kd)     \
   -2.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd)+u ## (i1,i2,i3+1,kd)+u ## (i1,i2,i3-1,kd))   \
   +   (u ## (i1+1,i2,i3+1,kd)+u ## (i1-1,i2,i3+1,kd)+u ## (i1+1,i2,i3-1,kd)+u ## (i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)

u ## yyzz23r(i1,i2,i3,kd)=( 4.*u ## (i1,i2,i3,kd)     \
   -2.*(u ## (i1,i2+1,i3,kd)  +u ## (i1,i2-1,i3,kd)+  u ## (i1,i2  ,i3+1,kd)+u ## (i1,i2  ,i3-1,kd))   \
   +   (u ## (i1,i2+1,i3+1,kd)+u ## (i1,i2-1,i3+1,kd)+u ## (i1,i2+1,i3-1,kd)+u ## (i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)

! 3D laplacian squared = u.xxxx + u.yyyy + u.zzzz + 2 (u.xxyy + u.xxzz + u.yyzz )
u ## LapSq23r(i1,i2,i3,kd)= ( 6.*u ## (i1,i2,i3,kd)   \
  - 4.*(u ## (i1+1,i2,i3,kd)+u ## (i1-1,i2,i3,kd))    \
      +(u ## (i1+2,i2,i3,kd)+u ## (i1-2,i2,i3,kd)) )/(dx(0)**4) \
  +( 6.*u ## (i1,i2,i3,kd)    \
   -4.*(u ## (i1,i2+1,i3,kd)+u ## (i1,i2-1,i3,kd))    \
      +(u ## (i1,i2+2,i3,kd)+u ## (i1,i2-2,i3,kd)) )/(dx(1)**4)  \
  +( 6.*u ## (i1,i2,i3,kd)    \
   -4.*(u ## (i1,i2,i3+1,kd)+u ## (i1,i2,i3-1,kd))    \
      +(u ## (i1,i2,i3+2,kd)+u ## (i1,i2,i3-2,kd)) )/(dx(2)**4)  \
  +( 8.*u ## (i1,i2,i3,kd)     \
   -4.*(u ## (i1+1,i2,i3,kd)  +u ## (i1-1,i2,i3,kd)  +u ## (i1  ,i2+1,i3,kd)+u ## (i1  ,i2-1,i3,kd))   \
   +2.*(u ## (i1+1,i2+1,i3,kd)+u ## (i1-1,i2+1,i3,kd)+u ## (i1+1,i2-1,i3,kd)+u ## (i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)\
  +( 8.*u ## (i1,i2,i3,kd)     \
   -4.*(u ## (i1+1,i2,i3,kd)  +u ## (i1-1,i2,i3,kd)  +u ## (i1  ,i2,i3+1,kd)+u ## (i1  ,i2,i3-1,kd))   \
   +2.*(u ## (i1+1,i2,i3+1,kd)+u ## (i1-1,i2,i3+1,kd)+u ## (i1+1,i2,i3-1,kd)+u ## (i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)\
  +( 8.*u ## (i1,i2,i3,kd)     \
   -4.*(u ## (i1,i2+1,i3,kd)  +u ## (i1,i2-1,i3,kd)  +u ## (i1,i2  ,i3+1,kd)+u ## (i1,i2  ,i3-1,kd))   \
   +2.*(u ## (i1,i2+1,i3+1,kd)+u ## (i1,i2-1,i3+1,kd)+u ## (i1,i2+1,i3-1,kd)+u ## (i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)


#endMacro



