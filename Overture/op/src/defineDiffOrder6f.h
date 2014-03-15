c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX
#beginMacro declareDifferenceOrder6(u,OPTION)
#If #OPTION == "RX"
 real d16
 real d26
 real h16
 real h26

 real rxr6
 real rxs6
 real rxt6
 real ryr6
 real rys6
 real ryt6
 real rzr6
 real rzs6
 real rzt6
 real sxr6
 real sxs6
 real sxt6
 real syr6
 real sys6
 real syt6
 real szr6
 real szs6
 real szt6
 real txr6
 real txs6
 real txt6
 real tyr6
 real tys6
 real tyt6
 real tzr6
 real tzs6
 real tzt6
 real rxx61
 real rxx62
 real rxy62
 real rxx63
 real rxy63
 real rxz63
 real ryx62
 real ryy62
 real ryx63
 real ryy63
 real ryz63
 real rzx62
 real rzy62
 real rzx63
 real rzy63
 real rzz63
 real sxx62
 real sxy62
 real sxx63
 real sxy63
 real sxz63
 real syx62
 real syy62
 real syx63
 real syy63
 real syz63
 real szx62
 real szy62
 real szx63
 real szy63
 real szz63
 real txx62
 real txy62
 real txx63
 real txy63
 real txz63
 real tyx62
 real tyy62
 real tyx63
 real tyy63
 real tyz63
 real tzx62
 real tzy62
 real tzx63
 real tzy63
 real tzz63
#End
 real u ## r6
 real u ## s6
 real u ## t6
 real u ## rr6
 real u ## ss6
 real u ## tt6
 real u ## rs6
 real u ## rt6
 real u ## st6
 real u ## x61
 real u ## y61
 real u ## z61
 real u ## x62
 real u ## y62
 real u ## z62
 real u ## x63
 real u ## y63
 real u ## z63
 real u ## xx61
 real u ## yy61
 real u ## xy61
 real u ## xz61
 real u ## yz61
 real u ## zz61
 real u ## laplacian61
 real u ## xx62
 real u ## yy62
 real u ## xy62
 real u ## xz62
 real u ## yz62
 real u ## zz62
 real u ## laplacian62
 real u ## xx63
 real u ## yy63
 real u ## zz63
 real u ## xy63
 real u ## xz63
 real u ## yz63
 real u ## laplacian63
 real u ## x63r
 real u ## y63r
 real u ## z63r
 real u ## xx63r
 real u ## yy63r
 real u ## zz63r
 real u ## xy63r
 real u ## xz63r
 real u ## yz63r
 real u ## x61r
 real u ## y61r
 real u ## z61r
 real u ## xx61r
 real u ## yy61r
 real u ## zz61r
 real u ## xy61r
 real u ## xz61r
 real u ## yz61r
 real u ## laplacian61r
 real u ## x62r
 real u ## y62r
 real u ## z62r
 real u ## xx62r
 real u ## yy62r
 real u ## zz62r
 real u ## xy62r
 real u ## xz62r
 real u ## yz62r
 real u ## laplacian62r
 real u ## laplacian63r
#endMacro


c Define statement functions for difference approximations of order 6 
c To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder6Components0(u,OPTION)

#If #OPTION == "RX"
d16(kd) = 1./(60.*dr(kd))
d26(kd) = 1./(180.*dr(kd)**2)
#End

u ## r6(i1,i2,i3)=(45.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-9.*(u(i1+2,i2,i3)-u(i1-2,i2,i3))+(u(i1+3,i2,i3)-u(i1-3,i2,i3)))*d16(0)
u ## s6(i1,i2,i3)=(45.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-9.*(u(i1,i2+2,i3)-u(i1,i2-2,i3))+(u(i1,i2+3,i3)-u(i1,i2-3,i3)))*d16(1)
u ## t6(i1,i2,i3)=(45.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-9.*(u(i1,i2,i3+2)-u(i1,i2,i3-2))+(u(i1,i2,i3+3)-u(i1,i2,i3-3)))*d16(2)

u ## rr6(i1,i2,i3)=(-490.*u(i1,i2,i3)+270.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-27.*(u(i1+2,i2,i3)+u(i1-2,i2,i3))+2.*(u(i1+3,i2,i3)+u(i1-3,i2,i3)) )*d26(0)
u ## ss6(i1,i2,i3)=(-490.*u(i1,i2,i3)+270.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-27.*(u(i1,i2+2,i3)+u(i1,i2-2,i3))+2.*(u(i1,i2+3,i3)+u(i1,i2-3,i3)) )*d26(1)
u ## tt6(i1,i2,i3)=(-490.*u(i1,i2,i3)+270.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-27.*(u(i1,i2,i3+2)+u(i1,i2,i3-2))+2.*(u(i1,i2,i3+3)+u(i1,i2,i3-3)) )*d26(2)
u ## rs6(i1,i2,i3)=(45.*(u ## r6(i1,i2+1,i3)-u ## r6(i1,i2-1,i3))-9.*(u ## r6(i1,i2+2,i3)-u ## r6(i1,i2-2,i3))+(u ## r6(i1,i2+3,i3)-u ## r6(i1,i2-3,i3)))*d16(1)
u ## rt6(i1,i2,i3)=(45.*(u ## r6(i1,i2,i3+1)-u ## r6(i1,i2,i3-1))-9.*(u ## r6(i1,i2,i3+2)-u ## r6(i1,i2,i3-2))+(u ## r6(i1,i2,i3+3)-u ## r6(i1,i2,i3-3)))*d16(2)
u ## st6(i1,i2,i3)=(45.*(u ## s6(i1,i2,i3+1)-u ## s6(i1,i2,i3-1))-9.*(u ## s6(i1,i2,i3+2)-u ## s6(i1,i2,i3-2))+(u ## s6(i1,i2,i3+3)-u ## s6(i1,i2,i3-3)))*d16(2)

#If #OPTION == "RX"
rxr6(i1,i2,i3)=(45.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-9.*(rx(i1+2,i2,i3)-rx(i1-2,i2,i3))+(rx(i1+3,i2,i3)-rx(i1-3,i2,i3)))*d16(0)
rxs6(i1,i2,i3)=(45.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-9.*(rx(i1,i2+2,i3)-rx(i1,i2-2,i3))+(rx(i1,i2+3,i3)-rx(i1,i2-3,i3)))*d16(1)
rxt6(i1,i2,i3)=(45.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-9.*(rx(i1,i2,i3+2)-rx(i1,i2,i3-2))+(rx(i1,i2,i3+3)-rx(i1,i2,i3-3)))*d16(2)
ryr6(i1,i2,i3)=(45.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-9.*(ry(i1+2,i2,i3)-ry(i1-2,i2,i3))+(ry(i1+3,i2,i3)-ry(i1-3,i2,i3)))*d16(0)
rys6(i1,i2,i3)=(45.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-9.*(ry(i1,i2+2,i3)-ry(i1,i2-2,i3))+(ry(i1,i2+3,i3)-ry(i1,i2-3,i3)))*d16(1)
ryt6(i1,i2,i3)=(45.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-9.*(ry(i1,i2,i3+2)-ry(i1,i2,i3-2))+(ry(i1,i2,i3+3)-ry(i1,i2,i3-3)))*d16(2)
rzr6(i1,i2,i3)=(45.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-9.*(rz(i1+2,i2,i3)-rz(i1-2,i2,i3))+(rz(i1+3,i2,i3)-rz(i1-3,i2,i3)))*d16(0)
rzs6(i1,i2,i3)=(45.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-9.*(rz(i1,i2+2,i3)-rz(i1,i2-2,i3))+(rz(i1,i2+3,i3)-rz(i1,i2-3,i3)))*d16(1)
rzt6(i1,i2,i3)=(45.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-9.*(rz(i1,i2,i3+2)-rz(i1,i2,i3-2))+(rz(i1,i2,i3+3)-rz(i1,i2,i3-3)))*d16(2)
sxr6(i1,i2,i3)=(45.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-9.*(sx(i1+2,i2,i3)-sx(i1-2,i2,i3))+(sx(i1+3,i2,i3)-sx(i1-3,i2,i3)))*d16(0)
sxs6(i1,i2,i3)=(45.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-9.*(sx(i1,i2+2,i3)-sx(i1,i2-2,i3))+(sx(i1,i2+3,i3)-sx(i1,i2-3,i3)))*d16(1)
sxt6(i1,i2,i3)=(45.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-9.*(sx(i1,i2,i3+2)-sx(i1,i2,i3-2))+(sx(i1,i2,i3+3)-sx(i1,i2,i3-3)))*d16(2)
syr6(i1,i2,i3)=(45.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-9.*(sy(i1+2,i2,i3)-sy(i1-2,i2,i3))+(sy(i1+3,i2,i3)-sy(i1-3,i2,i3)))*d16(0)
sys6(i1,i2,i3)=(45.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-9.*(sy(i1,i2+2,i3)-sy(i1,i2-2,i3))+(sy(i1,i2+3,i3)-sy(i1,i2-3,i3)))*d16(1)
syt6(i1,i2,i3)=(45.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-9.*(sy(i1,i2,i3+2)-sy(i1,i2,i3-2))+(sy(i1,i2,i3+3)-sy(i1,i2,i3-3)))*d16(2)
szr6(i1,i2,i3)=(45.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-9.*(sz(i1+2,i2,i3)-sz(i1-2,i2,i3))+(sz(i1+3,i2,i3)-sz(i1-3,i2,i3)))*d16(0)
szs6(i1,i2,i3)=(45.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-9.*(sz(i1,i2+2,i3)-sz(i1,i2-2,i3))+(sz(i1,i2+3,i3)-sz(i1,i2-3,i3)))*d16(1)
szt6(i1,i2,i3)=(45.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-9.*(sz(i1,i2,i3+2)-sz(i1,i2,i3-2))+(sz(i1,i2,i3+3)-sz(i1,i2,i3-3)))*d16(2)
txr6(i1,i2,i3)=(45.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-9.*(tx(i1+2,i2,i3)-tx(i1-2,i2,i3))+(tx(i1+3,i2,i3)-tx(i1-3,i2,i3)))*d16(0)
txs6(i1,i2,i3)=(45.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-9.*(tx(i1,i2+2,i3)-tx(i1,i2-2,i3))+(tx(i1,i2+3,i3)-tx(i1,i2-3,i3)))*d16(1)
txt6(i1,i2,i3)=(45.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-9.*(tx(i1,i2,i3+2)-tx(i1,i2,i3-2))+(tx(i1,i2,i3+3)-tx(i1,i2,i3-3)))*d16(2)
tyr6(i1,i2,i3)=(45.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-9.*(ty(i1+2,i2,i3)-ty(i1-2,i2,i3))+(ty(i1+3,i2,i3)-ty(i1-3,i2,i3)))*d16(0)
tys6(i1,i2,i3)=(45.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-9.*(ty(i1,i2+2,i3)-ty(i1,i2-2,i3))+(ty(i1,i2+3,i3)-ty(i1,i2-3,i3)))*d16(1)
tyt6(i1,i2,i3)=(45.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-9.*(ty(i1,i2,i3+2)-ty(i1,i2,i3-2))+(ty(i1,i2,i3+3)-ty(i1,i2,i3-3)))*d16(2)
tzr6(i1,i2,i3)=(45.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-9.*(tz(i1+2,i2,i3)-tz(i1-2,i2,i3))+(tz(i1+3,i2,i3)-tz(i1-3,i2,i3)))*d16(0)
tzs6(i1,i2,i3)=(45.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-9.*(tz(i1,i2+2,i3)-tz(i1,i2-2,i3))+(tz(i1,i2+3,i3)-tz(i1,i2-3,i3)))*d16(1)
tzt6(i1,i2,i3)=(45.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-9.*(tz(i1,i2,i3+2)-tz(i1,i2,i3-2))+(tz(i1,i2,i3+3)-tz(i1,i2,i3-3)))*d16(2)
#End

u ## x61(i1,i2,i3)= rx(i1,i2,i3)*u ## r6(i1,i2,i3)
u ## y61(i1,i2,i3)=0
u ## z61(i1,i2,i3)=0

u ## x62(i1,i2,i3)= rx(i1,i2,i3)*u ## r6(i1,i2,i3)+sx(i1,i2,i3)*u ## s6(i1,i2,i3)
u ## y62(i1,i2,i3)= ry(i1,i2,i3)*u ## r6(i1,i2,i3)+sy(i1,i2,i3)*u ## s6(i1,i2,i3)
u ## z62(i1,i2,i3)=0
u ## x63(i1,i2,i3)=rx(i1,i2,i3)*u ## r6(i1,i2,i3)+sx(i1,i2,i3)*u ## s6(i1,i2,i3)+tx(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## y63(i1,i2,i3)=ry(i1,i2,i3)*u ## r6(i1,i2,i3)+sy(i1,i2,i3)*u ## s6(i1,i2,i3)+ty(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## z63(i1,i2,i3)=rz(i1,i2,i3)*u ## r6(i1,i2,i3)+sz(i1,i2,i3)*u ## s6(i1,i2,i3)+tz(i1,i2,i3)*u ## t6(i1,i2,i3)

#If #OPTION == "RX"
rxx61(i1,i2,i3)= rx(i1,i2,i3)*rxr6(i1,i2,i3)
rxx62(i1,i2,i3)= rx(i1,i2,i3)*rxr6(i1,i2,i3)+sx(i1,i2,i3)*rxs6(i1,i2,i3)
rxy62(i1,i2,i3)= ry(i1,i2,i3)*rxr6(i1,i2,i3)+sy(i1,i2,i3)*rxs6(i1,i2,i3)
rxx63(i1,i2,i3)=rx(i1,i2,i3)*rxr6(i1,i2,i3)+sx(i1,i2,i3)*rxs6(i1,i2,i3)+tx(i1,i2,i3)*rxt6(i1,i2,i3)
rxy63(i1,i2,i3)=ry(i1,i2,i3)*rxr6(i1,i2,i3)+sy(i1,i2,i3)*rxs6(i1,i2,i3)+ty(i1,i2,i3)*rxt6(i1,i2,i3)
rxz63(i1,i2,i3)=rz(i1,i2,i3)*rxr6(i1,i2,i3)+sz(i1,i2,i3)*rxs6(i1,i2,i3)+tz(i1,i2,i3)*rxt6(i1,i2,i3)
ryx62(i1,i2,i3)= rx(i1,i2,i3)*ryr6(i1,i2,i3)+sx(i1,i2,i3)*rys6(i1,i2,i3)
ryy62(i1,i2,i3)= ry(i1,i2,i3)*ryr6(i1,i2,i3)+sy(i1,i2,i3)*rys6(i1,i2,i3)
ryx63(i1,i2,i3)=rx(i1,i2,i3)*ryr6(i1,i2,i3)+sx(i1,i2,i3)*rys6(i1,i2,i3)+tx(i1,i2,i3)*ryt6(i1,i2,i3)
ryy63(i1,i2,i3)=ry(i1,i2,i3)*ryr6(i1,i2,i3)+sy(i1,i2,i3)*rys6(i1,i2,i3)+ty(i1,i2,i3)*ryt6(i1,i2,i3)
ryz63(i1,i2,i3)=rz(i1,i2,i3)*ryr6(i1,i2,i3)+sz(i1,i2,i3)*rys6(i1,i2,i3)+tz(i1,i2,i3)*ryt6(i1,i2,i3)
rzx62(i1,i2,i3)= rx(i1,i2,i3)*rzr6(i1,i2,i3)+sx(i1,i2,i3)*rzs6(i1,i2,i3)
rzy62(i1,i2,i3)= ry(i1,i2,i3)*rzr6(i1,i2,i3)+sy(i1,i2,i3)*rzs6(i1,i2,i3)
rzx63(i1,i2,i3)=rx(i1,i2,i3)*rzr6(i1,i2,i3)+sx(i1,i2,i3)*rzs6(i1,i2,i3)+tx(i1,i2,i3)*rzt6(i1,i2,i3)
rzy63(i1,i2,i3)=ry(i1,i2,i3)*rzr6(i1,i2,i3)+sy(i1,i2,i3)*rzs6(i1,i2,i3)+ty(i1,i2,i3)*rzt6(i1,i2,i3)
rzz63(i1,i2,i3)=rz(i1,i2,i3)*rzr6(i1,i2,i3)+sz(i1,i2,i3)*rzs6(i1,i2,i3)+tz(i1,i2,i3)*rzt6(i1,i2,i3)
sxx62(i1,i2,i3)= rx(i1,i2,i3)*sxr6(i1,i2,i3)+sx(i1,i2,i3)*sxs6(i1,i2,i3)
sxy62(i1,i2,i3)= ry(i1,i2,i3)*sxr6(i1,i2,i3)+sy(i1,i2,i3)*sxs6(i1,i2,i3)
sxx63(i1,i2,i3)=rx(i1,i2,i3)*sxr6(i1,i2,i3)+sx(i1,i2,i3)*sxs6(i1,i2,i3)+tx(i1,i2,i3)*sxt6(i1,i2,i3)
sxy63(i1,i2,i3)=ry(i1,i2,i3)*sxr6(i1,i2,i3)+sy(i1,i2,i3)*sxs6(i1,i2,i3)+ty(i1,i2,i3)*sxt6(i1,i2,i3)
sxz63(i1,i2,i3)=rz(i1,i2,i3)*sxr6(i1,i2,i3)+sz(i1,i2,i3)*sxs6(i1,i2,i3)+tz(i1,i2,i3)*sxt6(i1,i2,i3)
syx62(i1,i2,i3)= rx(i1,i2,i3)*syr6(i1,i2,i3)+sx(i1,i2,i3)*sys6(i1,i2,i3)
syy62(i1,i2,i3)= ry(i1,i2,i3)*syr6(i1,i2,i3)+sy(i1,i2,i3)*sys6(i1,i2,i3)
syx63(i1,i2,i3)=rx(i1,i2,i3)*syr6(i1,i2,i3)+sx(i1,i2,i3)*sys6(i1,i2,i3)+tx(i1,i2,i3)*syt6(i1,i2,i3)
syy63(i1,i2,i3)=ry(i1,i2,i3)*syr6(i1,i2,i3)+sy(i1,i2,i3)*sys6(i1,i2,i3)+ty(i1,i2,i3)*syt6(i1,i2,i3)
syz63(i1,i2,i3)=rz(i1,i2,i3)*syr6(i1,i2,i3)+sz(i1,i2,i3)*sys6(i1,i2,i3)+tz(i1,i2,i3)*syt6(i1,i2,i3)
szx62(i1,i2,i3)= rx(i1,i2,i3)*szr6(i1,i2,i3)+sx(i1,i2,i3)*szs6(i1,i2,i3)
szy62(i1,i2,i3)= ry(i1,i2,i3)*szr6(i1,i2,i3)+sy(i1,i2,i3)*szs6(i1,i2,i3)
szx63(i1,i2,i3)=rx(i1,i2,i3)*szr6(i1,i2,i3)+sx(i1,i2,i3)*szs6(i1,i2,i3)+tx(i1,i2,i3)*szt6(i1,i2,i3)
szy63(i1,i2,i3)=ry(i1,i2,i3)*szr6(i1,i2,i3)+sy(i1,i2,i3)*szs6(i1,i2,i3)+ty(i1,i2,i3)*szt6(i1,i2,i3)
szz63(i1,i2,i3)=rz(i1,i2,i3)*szr6(i1,i2,i3)+sz(i1,i2,i3)*szs6(i1,i2,i3)+tz(i1,i2,i3)*szt6(i1,i2,i3)
txx62(i1,i2,i3)= rx(i1,i2,i3)*txr6(i1,i2,i3)+sx(i1,i2,i3)*txs6(i1,i2,i3)
txy62(i1,i2,i3)= ry(i1,i2,i3)*txr6(i1,i2,i3)+sy(i1,i2,i3)*txs6(i1,i2,i3)
txx63(i1,i2,i3)=rx(i1,i2,i3)*txr6(i1,i2,i3)+sx(i1,i2,i3)*txs6(i1,i2,i3)+tx(i1,i2,i3)*txt6(i1,i2,i3)
txy63(i1,i2,i3)=ry(i1,i2,i3)*txr6(i1,i2,i3)+sy(i1,i2,i3)*txs6(i1,i2,i3)+ty(i1,i2,i3)*txt6(i1,i2,i3)
txz63(i1,i2,i3)=rz(i1,i2,i3)*txr6(i1,i2,i3)+sz(i1,i2,i3)*txs6(i1,i2,i3)+tz(i1,i2,i3)*txt6(i1,i2,i3)
tyx62(i1,i2,i3)= rx(i1,i2,i3)*tyr6(i1,i2,i3)+sx(i1,i2,i3)*tys6(i1,i2,i3)
tyy62(i1,i2,i3)= ry(i1,i2,i3)*tyr6(i1,i2,i3)+sy(i1,i2,i3)*tys6(i1,i2,i3)
tyx63(i1,i2,i3)=rx(i1,i2,i3)*tyr6(i1,i2,i3)+sx(i1,i2,i3)*tys6(i1,i2,i3)+tx(i1,i2,i3)*tyt6(i1,i2,i3)
tyy63(i1,i2,i3)=ry(i1,i2,i3)*tyr6(i1,i2,i3)+sy(i1,i2,i3)*tys6(i1,i2,i3)+ty(i1,i2,i3)*tyt6(i1,i2,i3)
tyz63(i1,i2,i3)=rz(i1,i2,i3)*tyr6(i1,i2,i3)+sz(i1,i2,i3)*tys6(i1,i2,i3)+tz(i1,i2,i3)*tyt6(i1,i2,i3)
tzx62(i1,i2,i3)= rx(i1,i2,i3)*tzr6(i1,i2,i3)+sx(i1,i2,i3)*tzs6(i1,i2,i3)
tzy62(i1,i2,i3)= ry(i1,i2,i3)*tzr6(i1,i2,i3)+sy(i1,i2,i3)*tzs6(i1,i2,i3)
tzx63(i1,i2,i3)=rx(i1,i2,i3)*tzr6(i1,i2,i3)+sx(i1,i2,i3)*tzs6(i1,i2,i3)+tx(i1,i2,i3)*tzt6(i1,i2,i3)
tzy63(i1,i2,i3)=ry(i1,i2,i3)*tzr6(i1,i2,i3)+sy(i1,i2,i3)*tzs6(i1,i2,i3)+ty(i1,i2,i3)*tzt6(i1,i2,i3)
tzz63(i1,i2,i3)=rz(i1,i2,i3)*tzr6(i1,i2,i3)+sz(i1,i2,i3)*tzs6(i1,i2,i3)+tz(i1,i2,i3)*tzt6(i1,i2,i3)
#End

u ## xx61(i1,i2,i3)=(rx(i1,i2,i3)**2)*u ## rr6(i1,i2,i3)+(rxx62(i1,i2,i3))*u ## r6(i1,i2,i3)
u ## yy61(i1,i2,i3)=0
u ## xy61(i1,i2,i3)=0
u ## xz61(i1,i2,i3)=0
u ## yz61(i1,i2,i3)=0
u ## zz61(i1,i2,i3)=0
u ## laplacian61(i1,i2,i3)=u ## xx61(i1,i2,i3)
u ## xx62(i1,i2,i3)=(rx(i1,i2,i3)**2)*u ## rr6(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3)+(sx(i1,i2,i3)**2)*u ## ss6(i1,i2,i3)+(rxx62(i1,i2,i3))*u ## r6(i1,i2,i3)+(sxx62(i1,i2,i3))*u ## s6(i1,i2,i3)
u ## yy62(i1,i2,i3)=(ry(i1,i2,i3)**2)*u ## rr6(i1,i2,i3)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs6(i1,i2,i3)+(sy(i1,i2,i3)**2)*u ## ss6(i1,i2,i3)+(ryy62(i1,i2,i3))*u ## r6(i1,i2,i3)+(syy62(i1,i2,i3))*u ## s6(i1,i2,i3)
u ## xy62(i1,i2,i3)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr6(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss6(i1,i2,i3)+rxy62(i1,i2,i3)*u ## r6(i1,i2,i3)+sxy62(i1,i2,i3)*u ## s6(i1,i2,i3)
u ## xz62(i1,i2,i3)=0
u ## yz62(i1,i2,i3)=0
u ## zz62(i1,i2,i3)=0
u ## laplacian62(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*u ## rr6(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs6(i1,i2,i3)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*u ## ss6(i1,i2,i3)+(rxx62(i1,i2,i3)+ryy62(i1,i2,i3))*u ## r6(i1,i2,i3)+(sxx62(i1,i2,i3)+syy62(i1,i2,i3))*u ## s6(i1,i2,i3)
u ## xx63(i1,i2,i3)=rx(i1,i2,i3)**2*u ## rr6(i1,i2,i3)+sx(i1,i2,i3)**2*u ## ss6(i1,i2,i3)+tx(i1,i2,i3)**2*u ## tt6(i1,i2,i3)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs6(i1,i2,i3)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt6(i1,i2,i3)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st6(i1,i2,i3)+rxx63(i1,i2,i3)*u ## r6(i1,i2,i3)+sxx63(i1,i2,i3)*u ## s6(i1,i2,i3)+txx63(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## yy63(i1,i2,i3)=ry(i1,i2,i3)**2*u ## rr6(i1,i2,i3)+sy(i1,i2,i3)**2*u ## ss6(i1,i2,i3)+ty(i1,i2,i3)**2*u ## tt6(i1,i2,i3)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs6(i1,i2,i3)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt6(i1,i2,i3)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st6(i1,i2,i3)+ryy63(i1,i2,i3)*u ## r6(i1,i2,i3)+syy63(i1,i2,i3)*u ## s6(i1,i2,i3)+tyy63(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## zz63(i1,i2,i3)=rz(i1,i2,i3)**2*u ## rr6(i1,i2,i3)+sz(i1,i2,i3)**2*u ## ss6(i1,i2,i3)+tz(i1,i2,i3)**2*u ## tt6(i1,i2,i3)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs6(i1,i2,i3)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt6(i1,i2,i3)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st6(i1,i2,i3)+rzz63(i1,i2,i3)*u ## r6(i1,i2,i3)+szz63(i1,i2,i3)*u ## s6(i1,i2,i3)+tzz63(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## xy63(i1,i2,i3)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr6(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss6(i1,i2,i3)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt6(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt6(i1,i2,i3)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st6(i1,i2,i3)+rxy63(i1,i2,i3)*u ## r6(i1,i2,i3)+sxy63(i1,i2,i3)*u ## s6(i1,i2,i3)+txy63(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## xz63(i1,i2,i3)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr6(i1,i2,i3)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss6(i1,i2,i3)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt6(i1,i2,i3)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt6(i1,i2,i3)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st6(i1,i2,i3)+rxz63(i1,i2,i3)*u ## r6(i1,i2,i3)+sxz63(i1,i2,i3)*u ## s6(i1,i2,i3)+txz63(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## yz63(i1,i2,i3)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr6(i1,i2,i3)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss6(i1,i2,i3)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt6(i1,i2,i3)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs6(i1,i2,i3)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt6(i1,i2,i3)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st6(i1,i2,i3)+ryz63(i1,i2,i3)*u ## r6(i1,i2,i3)+syz63(i1,i2,i3)*u ## s6(i1,i2,i3)+tyz63(i1,i2,i3)*u ## t6(i1,i2,i3)
u ## laplacian63(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*u ## rr6(i1,i2,i3)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*u ## ss6(i1,i2,i3)+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*u ## tt6(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs6(i1,i2,i3)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt6(i1,i2,i3)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st6(i1,i2,i3)+(rxx63(i1,i2,i3)+ryy63(i1,i2,i3)+rzz63(i1,i2,i3))*u ## r6(i1,i2,i3)+(sxx63(i1,i2,i3)+syy63(i1,i2,i3)+szz63(i1,i2,i3))*u ## s6(i1,i2,i3)+(txx63(i1,i2,i3)+tyy63(i1,i2,i3)+tzz63(i1,i2,i3))*u ## t6(i1,i2,i3)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
#If #OPTION == "RX"
h16(kd) = 1./(60.*dx(kd))
h26(kd) = 1./(180.*dx(kd)**2)
#End

u ## x63r(i1,i2,i3)=(45.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-9.*(u(i1+2,i2,i3)-u(i1-2,i2,i3))+(u(i1+3,i2,i3)-u(i1-3,i2,i3)))*h16(0)
u ## y63r(i1,i2,i3)=(45.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-9.*(u(i1,i2+2,i3)-u(i1,i2-2,i3))+(u(i1,i2+3,i3)-u(i1,i2-3,i3)))*h16(1)
u ## z63r(i1,i2,i3)=(45.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-9.*(u(i1,i2,i3+2)-u(i1,i2,i3-2))+(u(i1,i2,i3+3)-u(i1,i2,i3-3)))*h16(2)

u ## xx63r(i1,i2,i3)=(-490.*u(i1,i2,i3)+270.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-27.*(u(i1+2,i2,i3)+u(i1-2,i2,i3))+2.*(u(i1+3,i2,i3)+u(i1-3,i2,i3)) )*h26(0)
u ## yy63r(i1,i2,i3)=(-490.*u(i1,i2,i3)+270.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-27.*(u(i1,i2+2,i3)+u(i1,i2-2,i3))+2.*(u(i1,i2+3,i3)+u(i1,i2-3,i3)) )*h26(1)
u ## zz63r(i1,i2,i3)=(-490.*u(i1,i2,i3)+270.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-27.*(u(i1,i2,i3+2)+u(i1,i2,i3-2))+2.*(u(i1,i2,i3+3)+u(i1,i2,i3-3)) )*h26(2)
u ## xy63r(i1,i2,i3)=(45.*(u ## x63r(i1,i2+1,i3)-u ## x63r(i1,i2-1,i3))-9.*(u ## x63r(i1,i2+2,i3)-u ## x63r(i1,i2-2,i3))+(u ## x63r(i1,i2+3,i3)-u ## x63r(i1,i2-3,i3)))*h16(1)
u ## xz63r(i1,i2,i3)=(45.*(u ## x63r(i1,i2,i3+1)-u ## x63r(i1,i2,i3-1))-9.*(u ## x63r(i1,i2,i3+2)-u ## x63r(i1,i2,i3-2))+(u ## x63r(i1,i2,i3+3)-u ## x63r(i1,i2,i3-3)))*h16(2)
u ## yz63r(i1,i2,i3)=(45.*(u ## y63r(i1,i2,i3+1)-u ## y63r(i1,i2,i3-1))-9.*(u ## y63r(i1,i2,i3+2)-u ## y63r(i1,i2,i3-2))+(u ## y63r(i1,i2,i3+3)-u ## y63r(i1,i2,i3-3)))*h16(2)

u ## x61r(i1,i2,i3)= u ## x63r(i1,i2,i3)
u ## y61r(i1,i2,i3)= u ## y63r(i1,i2,i3)
u ## z61r(i1,i2,i3)= u ## z63r(i1,i2,i3)
u ## xx61r(i1,i2,i3)= u ## xx63r(i1,i2,i3)
u ## yy61r(i1,i2,i3)= u ## yy63r(i1,i2,i3)
u ## zz61r(i1,i2,i3)= u ## zz63r(i1,i2,i3)
u ## xy61r(i1,i2,i3)= u ## xy63r(i1,i2,i3)
u ## xz61r(i1,i2,i3)= u ## xz63r(i1,i2,i3)
u ## yz61r(i1,i2,i3)= u ## yz63r(i1,i2,i3)
u ## laplacian61r(i1,i2,i3)=u ## xx63r(i1,i2,i3)
u ## x62r(i1,i2,i3)= u ## x63r(i1,i2,i3)
u ## y62r(i1,i2,i3)= u ## y63r(i1,i2,i3)
u ## z62r(i1,i2,i3)= u ## z63r(i1,i2,i3)
u ## xx62r(i1,i2,i3)= u ## xx63r(i1,i2,i3)
u ## yy62r(i1,i2,i3)= u ## yy63r(i1,i2,i3)
u ## zz62r(i1,i2,i3)= u ## zz63r(i1,i2,i3)
u ## xy62r(i1,i2,i3)= u ## xy63r(i1,i2,i3)
u ## xz62r(i1,i2,i3)= u ## xz63r(i1,i2,i3)
u ## yz62r(i1,i2,i3)= u ## yz63r(i1,i2,i3)
u ## laplacian62r(i1,i2,i3)=u ## xx63r(i1,i2,i3)+u ## yy63r(i1,i2,i3)
u ## laplacian63r(i1,i2,i3)=u ## xx63r(i1,i2,i3)+u ## yy63r(i1,i2,i3)+u ## zz63r(i1,i2,i3)
#endMacro
c To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder6Components1(u,OPTION)

#If #OPTION == "RX"
d16(kd) = 1./(60.*dr(kd))
d26(kd) = 1./(180.*dr(kd)**2)
#End

u ## r6(i1,i2,i3,kd)=(45.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-9.*(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+(u(i1+3,i2,i3,kd)-u(i1-3,i2,i3,kd)))*d16(0)
u ## s6(i1,i2,i3,kd)=(45.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-9.*(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+(u(i1,i2+3,i3,kd)-u(i1,i2-3,i3,kd)))*d16(1)
u ## t6(i1,i2,i3,kd)=(45.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-9.*(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+(u(i1,i2,i3+3,kd)-u(i1,i2,i3-3,kd)))*d16(2)

u ## rr6(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))-27.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd))+2.*(u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd)) )*d26(0)
u ## ss6(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))-27.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd))+2.*(u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd)) )*d26(1)
u ## tt6(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))-27.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd))+2.*(u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd)) )*d26(2)
u ## rs6(i1,i2,i3,kd)=(45.*(u ## r6(i1,i2+1,i3,kd)-u ## r6(i1,i2-1,i3,kd))-9.*(u ## r6(i1,i2+2,i3,kd)-u ## r6(i1,i2-2,i3,kd))+(u ## r6(i1,i2+3,i3,kd)-u ## r6(i1,i2-3,i3,kd)))*d16(1)
u ## rt6(i1,i2,i3,kd)=(45.*(u ## r6(i1,i2,i3+1,kd)-u ## r6(i1,i2,i3-1,kd))-9.*(u ## r6(i1,i2,i3+2,kd)-u ## r6(i1,i2,i3-2,kd))+(u ## r6(i1,i2,i3+3,kd)-u ## r6(i1,i2,i3-3,kd)))*d16(2)
u ## st6(i1,i2,i3,kd)=(45.*(u ## s6(i1,i2,i3+1,kd)-u ## s6(i1,i2,i3-1,kd))-9.*(u ## s6(i1,i2,i3+2,kd)-u ## s6(i1,i2,i3-2,kd))+(u ## s6(i1,i2,i3+3,kd)-u ## s6(i1,i2,i3-3,kd)))*d16(2)

#If #OPTION == "RX"
rxr6(i1,i2,i3)=(45.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-9.*(rx(i1+2,i2,i3)-rx(i1-2,i2,i3))+(rx(i1+3,i2,i3)-rx(i1-3,i2,i3)))*d16(0)
rxs6(i1,i2,i3)=(45.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-9.*(rx(i1,i2+2,i3)-rx(i1,i2-2,i3))+(rx(i1,i2+3,i3)-rx(i1,i2-3,i3)))*d16(1)
rxt6(i1,i2,i3)=(45.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-9.*(rx(i1,i2,i3+2)-rx(i1,i2,i3-2))+(rx(i1,i2,i3+3)-rx(i1,i2,i3-3)))*d16(2)
ryr6(i1,i2,i3)=(45.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-9.*(ry(i1+2,i2,i3)-ry(i1-2,i2,i3))+(ry(i1+3,i2,i3)-ry(i1-3,i2,i3)))*d16(0)
rys6(i1,i2,i3)=(45.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-9.*(ry(i1,i2+2,i3)-ry(i1,i2-2,i3))+(ry(i1,i2+3,i3)-ry(i1,i2-3,i3)))*d16(1)
ryt6(i1,i2,i3)=(45.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-9.*(ry(i1,i2,i3+2)-ry(i1,i2,i3-2))+(ry(i1,i2,i3+3)-ry(i1,i2,i3-3)))*d16(2)
rzr6(i1,i2,i3)=(45.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-9.*(rz(i1+2,i2,i3)-rz(i1-2,i2,i3))+(rz(i1+3,i2,i3)-rz(i1-3,i2,i3)))*d16(0)
rzs6(i1,i2,i3)=(45.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-9.*(rz(i1,i2+2,i3)-rz(i1,i2-2,i3))+(rz(i1,i2+3,i3)-rz(i1,i2-3,i3)))*d16(1)
rzt6(i1,i2,i3)=(45.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-9.*(rz(i1,i2,i3+2)-rz(i1,i2,i3-2))+(rz(i1,i2,i3+3)-rz(i1,i2,i3-3)))*d16(2)
sxr6(i1,i2,i3)=(45.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-9.*(sx(i1+2,i2,i3)-sx(i1-2,i2,i3))+(sx(i1+3,i2,i3)-sx(i1-3,i2,i3)))*d16(0)
sxs6(i1,i2,i3)=(45.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-9.*(sx(i1,i2+2,i3)-sx(i1,i2-2,i3))+(sx(i1,i2+3,i3)-sx(i1,i2-3,i3)))*d16(1)
sxt6(i1,i2,i3)=(45.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-9.*(sx(i1,i2,i3+2)-sx(i1,i2,i3-2))+(sx(i1,i2,i3+3)-sx(i1,i2,i3-3)))*d16(2)
syr6(i1,i2,i3)=(45.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-9.*(sy(i1+2,i2,i3)-sy(i1-2,i2,i3))+(sy(i1+3,i2,i3)-sy(i1-3,i2,i3)))*d16(0)
sys6(i1,i2,i3)=(45.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-9.*(sy(i1,i2+2,i3)-sy(i1,i2-2,i3))+(sy(i1,i2+3,i3)-sy(i1,i2-3,i3)))*d16(1)
syt6(i1,i2,i3)=(45.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-9.*(sy(i1,i2,i3+2)-sy(i1,i2,i3-2))+(sy(i1,i2,i3+3)-sy(i1,i2,i3-3)))*d16(2)
szr6(i1,i2,i3)=(45.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-9.*(sz(i1+2,i2,i3)-sz(i1-2,i2,i3))+(sz(i1+3,i2,i3)-sz(i1-3,i2,i3)))*d16(0)
szs6(i1,i2,i3)=(45.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-9.*(sz(i1,i2+2,i3)-sz(i1,i2-2,i3))+(sz(i1,i2+3,i3)-sz(i1,i2-3,i3)))*d16(1)
szt6(i1,i2,i3)=(45.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-9.*(sz(i1,i2,i3+2)-sz(i1,i2,i3-2))+(sz(i1,i2,i3+3)-sz(i1,i2,i3-3)))*d16(2)
txr6(i1,i2,i3)=(45.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-9.*(tx(i1+2,i2,i3)-tx(i1-2,i2,i3))+(tx(i1+3,i2,i3)-tx(i1-3,i2,i3)))*d16(0)
txs6(i1,i2,i3)=(45.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-9.*(tx(i1,i2+2,i3)-tx(i1,i2-2,i3))+(tx(i1,i2+3,i3)-tx(i1,i2-3,i3)))*d16(1)
txt6(i1,i2,i3)=(45.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-9.*(tx(i1,i2,i3+2)-tx(i1,i2,i3-2))+(tx(i1,i2,i3+3)-tx(i1,i2,i3-3)))*d16(2)
tyr6(i1,i2,i3)=(45.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-9.*(ty(i1+2,i2,i3)-ty(i1-2,i2,i3))+(ty(i1+3,i2,i3)-ty(i1-3,i2,i3)))*d16(0)
tys6(i1,i2,i3)=(45.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-9.*(ty(i1,i2+2,i3)-ty(i1,i2-2,i3))+(ty(i1,i2+3,i3)-ty(i1,i2-3,i3)))*d16(1)
tyt6(i1,i2,i3)=(45.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-9.*(ty(i1,i2,i3+2)-ty(i1,i2,i3-2))+(ty(i1,i2,i3+3)-ty(i1,i2,i3-3)))*d16(2)
tzr6(i1,i2,i3)=(45.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-9.*(tz(i1+2,i2,i3)-tz(i1-2,i2,i3))+(tz(i1+3,i2,i3)-tz(i1-3,i2,i3)))*d16(0)
tzs6(i1,i2,i3)=(45.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-9.*(tz(i1,i2+2,i3)-tz(i1,i2-2,i3))+(tz(i1,i2+3,i3)-tz(i1,i2-3,i3)))*d16(1)
tzt6(i1,i2,i3)=(45.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-9.*(tz(i1,i2,i3+2)-tz(i1,i2,i3-2))+(tz(i1,i2,i3+3)-tz(i1,i2,i3-3)))*d16(2)
#End

u ## x61(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r6(i1,i2,i3,kd)
u ## y61(i1,i2,i3,kd)=0
u ## z61(i1,i2,i3,kd)=0

u ## x62(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s6(i1,i2,i3,kd)
u ## y62(i1,i2,i3,kd)= ry(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s6(i1,i2,i3,kd)
u ## z62(i1,i2,i3,kd)=0
u ## x63(i1,i2,i3,kd)=rx(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+tx(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## y63(i1,i2,i3,kd)=ry(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+ty(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## z63(i1,i2,i3,kd)=rz(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sz(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+tz(i1,i2,i3)*u ## t6(i1,i2,i3,kd)

#If #OPTION == "RX"
rxx61(i1,i2,i3)= rx(i1,i2,i3)*rxr6(i1,i2,i3)
rxx62(i1,i2,i3)= rx(i1,i2,i3)*rxr6(i1,i2,i3)+sx(i1,i2,i3)*rxs6(i1,i2,i3)
rxy62(i1,i2,i3)= ry(i1,i2,i3)*rxr6(i1,i2,i3)+sy(i1,i2,i3)*rxs6(i1,i2,i3)
rxx63(i1,i2,i3)=rx(i1,i2,i3)*rxr6(i1,i2,i3)+sx(i1,i2,i3)*rxs6(i1,i2,i3)+tx(i1,i2,i3)*rxt6(i1,i2,i3)
rxy63(i1,i2,i3)=ry(i1,i2,i3)*rxr6(i1,i2,i3)+sy(i1,i2,i3)*rxs6(i1,i2,i3)+ty(i1,i2,i3)*rxt6(i1,i2,i3)
rxz63(i1,i2,i3)=rz(i1,i2,i3)*rxr6(i1,i2,i3)+sz(i1,i2,i3)*rxs6(i1,i2,i3)+tz(i1,i2,i3)*rxt6(i1,i2,i3)
ryx62(i1,i2,i3)= rx(i1,i2,i3)*ryr6(i1,i2,i3)+sx(i1,i2,i3)*rys6(i1,i2,i3)
ryy62(i1,i2,i3)= ry(i1,i2,i3)*ryr6(i1,i2,i3)+sy(i1,i2,i3)*rys6(i1,i2,i3)
ryx63(i1,i2,i3)=rx(i1,i2,i3)*ryr6(i1,i2,i3)+sx(i1,i2,i3)*rys6(i1,i2,i3)+tx(i1,i2,i3)*ryt6(i1,i2,i3)
ryy63(i1,i2,i3)=ry(i1,i2,i3)*ryr6(i1,i2,i3)+sy(i1,i2,i3)*rys6(i1,i2,i3)+ty(i1,i2,i3)*ryt6(i1,i2,i3)
ryz63(i1,i2,i3)=rz(i1,i2,i3)*ryr6(i1,i2,i3)+sz(i1,i2,i3)*rys6(i1,i2,i3)+tz(i1,i2,i3)*ryt6(i1,i2,i3)
rzx62(i1,i2,i3)= rx(i1,i2,i3)*rzr6(i1,i2,i3)+sx(i1,i2,i3)*rzs6(i1,i2,i3)
rzy62(i1,i2,i3)= ry(i1,i2,i3)*rzr6(i1,i2,i3)+sy(i1,i2,i3)*rzs6(i1,i2,i3)
rzx63(i1,i2,i3)=rx(i1,i2,i3)*rzr6(i1,i2,i3)+sx(i1,i2,i3)*rzs6(i1,i2,i3)+tx(i1,i2,i3)*rzt6(i1,i2,i3)
rzy63(i1,i2,i3)=ry(i1,i2,i3)*rzr6(i1,i2,i3)+sy(i1,i2,i3)*rzs6(i1,i2,i3)+ty(i1,i2,i3)*rzt6(i1,i2,i3)
rzz63(i1,i2,i3)=rz(i1,i2,i3)*rzr6(i1,i2,i3)+sz(i1,i2,i3)*rzs6(i1,i2,i3)+tz(i1,i2,i3)*rzt6(i1,i2,i3)
sxx62(i1,i2,i3)= rx(i1,i2,i3)*sxr6(i1,i2,i3)+sx(i1,i2,i3)*sxs6(i1,i2,i3)
sxy62(i1,i2,i3)= ry(i1,i2,i3)*sxr6(i1,i2,i3)+sy(i1,i2,i3)*sxs6(i1,i2,i3)
sxx63(i1,i2,i3)=rx(i1,i2,i3)*sxr6(i1,i2,i3)+sx(i1,i2,i3)*sxs6(i1,i2,i3)+tx(i1,i2,i3)*sxt6(i1,i2,i3)
sxy63(i1,i2,i3)=ry(i1,i2,i3)*sxr6(i1,i2,i3)+sy(i1,i2,i3)*sxs6(i1,i2,i3)+ty(i1,i2,i3)*sxt6(i1,i2,i3)
sxz63(i1,i2,i3)=rz(i1,i2,i3)*sxr6(i1,i2,i3)+sz(i1,i2,i3)*sxs6(i1,i2,i3)+tz(i1,i2,i3)*sxt6(i1,i2,i3)
syx62(i1,i2,i3)= rx(i1,i2,i3)*syr6(i1,i2,i3)+sx(i1,i2,i3)*sys6(i1,i2,i3)
syy62(i1,i2,i3)= ry(i1,i2,i3)*syr6(i1,i2,i3)+sy(i1,i2,i3)*sys6(i1,i2,i3)
syx63(i1,i2,i3)=rx(i1,i2,i3)*syr6(i1,i2,i3)+sx(i1,i2,i3)*sys6(i1,i2,i3)+tx(i1,i2,i3)*syt6(i1,i2,i3)
syy63(i1,i2,i3)=ry(i1,i2,i3)*syr6(i1,i2,i3)+sy(i1,i2,i3)*sys6(i1,i2,i3)+ty(i1,i2,i3)*syt6(i1,i2,i3)
syz63(i1,i2,i3)=rz(i1,i2,i3)*syr6(i1,i2,i3)+sz(i1,i2,i3)*sys6(i1,i2,i3)+tz(i1,i2,i3)*syt6(i1,i2,i3)
szx62(i1,i2,i3)= rx(i1,i2,i3)*szr6(i1,i2,i3)+sx(i1,i2,i3)*szs6(i1,i2,i3)
szy62(i1,i2,i3)= ry(i1,i2,i3)*szr6(i1,i2,i3)+sy(i1,i2,i3)*szs6(i1,i2,i3)
szx63(i1,i2,i3)=rx(i1,i2,i3)*szr6(i1,i2,i3)+sx(i1,i2,i3)*szs6(i1,i2,i3)+tx(i1,i2,i3)*szt6(i1,i2,i3)
szy63(i1,i2,i3)=ry(i1,i2,i3)*szr6(i1,i2,i3)+sy(i1,i2,i3)*szs6(i1,i2,i3)+ty(i1,i2,i3)*szt6(i1,i2,i3)
szz63(i1,i2,i3)=rz(i1,i2,i3)*szr6(i1,i2,i3)+sz(i1,i2,i3)*szs6(i1,i2,i3)+tz(i1,i2,i3)*szt6(i1,i2,i3)
txx62(i1,i2,i3)= rx(i1,i2,i3)*txr6(i1,i2,i3)+sx(i1,i2,i3)*txs6(i1,i2,i3)
txy62(i1,i2,i3)= ry(i1,i2,i3)*txr6(i1,i2,i3)+sy(i1,i2,i3)*txs6(i1,i2,i3)
txx63(i1,i2,i3)=rx(i1,i2,i3)*txr6(i1,i2,i3)+sx(i1,i2,i3)*txs6(i1,i2,i3)+tx(i1,i2,i3)*txt6(i1,i2,i3)
txy63(i1,i2,i3)=ry(i1,i2,i3)*txr6(i1,i2,i3)+sy(i1,i2,i3)*txs6(i1,i2,i3)+ty(i1,i2,i3)*txt6(i1,i2,i3)
txz63(i1,i2,i3)=rz(i1,i2,i3)*txr6(i1,i2,i3)+sz(i1,i2,i3)*txs6(i1,i2,i3)+tz(i1,i2,i3)*txt6(i1,i2,i3)
tyx62(i1,i2,i3)= rx(i1,i2,i3)*tyr6(i1,i2,i3)+sx(i1,i2,i3)*tys6(i1,i2,i3)
tyy62(i1,i2,i3)= ry(i1,i2,i3)*tyr6(i1,i2,i3)+sy(i1,i2,i3)*tys6(i1,i2,i3)
tyx63(i1,i2,i3)=rx(i1,i2,i3)*tyr6(i1,i2,i3)+sx(i1,i2,i3)*tys6(i1,i2,i3)+tx(i1,i2,i3)*tyt6(i1,i2,i3)
tyy63(i1,i2,i3)=ry(i1,i2,i3)*tyr6(i1,i2,i3)+sy(i1,i2,i3)*tys6(i1,i2,i3)+ty(i1,i2,i3)*tyt6(i1,i2,i3)
tyz63(i1,i2,i3)=rz(i1,i2,i3)*tyr6(i1,i2,i3)+sz(i1,i2,i3)*tys6(i1,i2,i3)+tz(i1,i2,i3)*tyt6(i1,i2,i3)
tzx62(i1,i2,i3)= rx(i1,i2,i3)*tzr6(i1,i2,i3)+sx(i1,i2,i3)*tzs6(i1,i2,i3)
tzy62(i1,i2,i3)= ry(i1,i2,i3)*tzr6(i1,i2,i3)+sy(i1,i2,i3)*tzs6(i1,i2,i3)
tzx63(i1,i2,i3)=rx(i1,i2,i3)*tzr6(i1,i2,i3)+sx(i1,i2,i3)*tzs6(i1,i2,i3)+tx(i1,i2,i3)*tzt6(i1,i2,i3)
tzy63(i1,i2,i3)=ry(i1,i2,i3)*tzr6(i1,i2,i3)+sy(i1,i2,i3)*tzs6(i1,i2,i3)+ty(i1,i2,i3)*tzt6(i1,i2,i3)
tzz63(i1,i2,i3)=rz(i1,i2,i3)*tzr6(i1,i2,i3)+sz(i1,i2,i3)*tzs6(i1,i2,i3)+tz(i1,i2,i3)*tzt6(i1,i2,i3)
#End

u ## xx61(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u ## rr6(i1,i2,i3,kd)+(rxx62(i1,i2,i3))*u ## r6(i1,i2,i3,kd)
u ## yy61(i1,i2,i3,kd)=0
u ## xy61(i1,i2,i3,kd)=0
u ## xz61(i1,i2,i3,kd)=0
u ## yz61(i1,i2,i3,kd)=0
u ## zz61(i1,i2,i3,kd)=0
u ## laplacian61(i1,i2,i3,kd)=u ## xx61(i1,i2,i3,kd)
u ## xx62(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u ## rr6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*u ## ss6(i1,i2,i3,kd)+(rxx62(i1,i2,i3))*u ## r6(i1,i2,i3,kd)+(sxx62(i1,i2,i3))*u ## s6(i1,i2,i3,kd)
u ## yy62(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*u ## rr6(i1,i2,i3,kd)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*u ## ss6(i1,i2,i3,kd)+(ryy62(i1,i2,i3))*u ## r6(i1,i2,i3,kd)+(syy62(i1,i2,i3))*u ## s6(i1,i2,i3,kd)
u ## xy62(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr6(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss6(i1,i2,i3,kd)+rxy62(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sxy62(i1,i2,i3)*u ## s6(i1,i2,i3,kd)
u ## xz62(i1,i2,i3,kd)=0
u ## yz62(i1,i2,i3,kd)=0
u ## zz62(i1,i2,i3,kd)=0
u ## laplacian62(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*u ## rr6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*u ## ss6(i1,i2,i3,kd)+(rxx62(i1,i2,i3)+ryy62(i1,i2,i3))*u ## r6(i1,i2,i3,kd)+(sxx62(i1,i2,i3)+syy62(i1,i2,i3))*u ## s6(i1,i2,i3,kd)
u ## xx63(i1,i2,i3,kd)=rx(i1,i2,i3)**2*u ## rr6(i1,i2,i3,kd)+sx(i1,i2,i3)**2*u ## ss6(i1,i2,i3,kd)+tx(i1,i2,i3)**2*u ## tt6(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs6(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt6(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st6(i1,i2,i3,kd)+rxx63(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sxx63(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+txx63(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## yy63(i1,i2,i3,kd)=ry(i1,i2,i3)**2*u ## rr6(i1,i2,i3,kd)+sy(i1,i2,i3)**2*u ## ss6(i1,i2,i3,kd)+ty(i1,i2,i3)**2*u ## tt6(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs6(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt6(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st6(i1,i2,i3,kd)+ryy63(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+syy63(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+tyy63(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## zz63(i1,i2,i3,kd)=rz(i1,i2,i3)**2*u ## rr6(i1,i2,i3,kd)+sz(i1,i2,i3)**2*u ## ss6(i1,i2,i3,kd)+tz(i1,i2,i3)**2*u ## tt6(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs6(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt6(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st6(i1,i2,i3,kd)+rzz63(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+szz63(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+tzz63(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## xy63(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr6(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss6(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt6(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt6(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st6(i1,i2,i3,kd)+rxy63(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sxy63(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+txy63(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## xz63(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr6(i1,i2,i3,kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss6(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt6(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt6(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st6(i1,i2,i3,kd)+rxz63(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+sxz63(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+txz63(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## yz63(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr6(i1,i2,i3,kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss6(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt6(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt6(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st6(i1,i2,i3,kd)+ryz63(i1,i2,i3)*u ## r6(i1,i2,i3,kd)+syz63(i1,i2,i3)*u ## s6(i1,i2,i3,kd)+tyz63(i1,i2,i3)*u ## t6(i1,i2,i3,kd)
u ## laplacian63(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*u ## rr6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*u ## ss6(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*u ## tt6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt6(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st6(i1,i2,i3,kd)+(rxx63(i1,i2,i3)+ryy63(i1,i2,i3)+rzz63(i1,i2,i3))*u ## r6(i1,i2,i3,kd)+(sxx63(i1,i2,i3)+syy63(i1,i2,i3)+szz63(i1,i2,i3))*u ## s6(i1,i2,i3,kd)+(txx63(i1,i2,i3)+tyy63(i1,i2,i3)+tzz63(i1,i2,i3))*u ## t6(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
#If #OPTION == "RX"
h16(kd) = 1./(60.*dx(kd))
h26(kd) = 1./(180.*dx(kd)**2)
#End

u ## x63r(i1,i2,i3,kd)=(45.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-9.*(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+(u(i1+3,i2,i3,kd)-u(i1-3,i2,i3,kd)))*h16(0)
u ## y63r(i1,i2,i3,kd)=(45.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-9.*(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+(u(i1,i2+3,i3,kd)-u(i1,i2-3,i3,kd)))*h16(1)
u ## z63r(i1,i2,i3,kd)=(45.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-9.*(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+(u(i1,i2,i3+3,kd)-u(i1,i2,i3-3,kd)))*h16(2)

u ## xx63r(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))-27.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd))+2.*(u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd)) )*h26(0)
u ## yy63r(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))-27.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd))+2.*(u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd)) )*h26(1)
u ## zz63r(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))-27.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd))+2.*(u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd)) )*h26(2)
u ## xy63r(i1,i2,i3,kd)=(45.*(u ## x63r(i1,i2+1,i3,kd)-u ## x63r(i1,i2-1,i3,kd))-9.*(u ## x63r(i1,i2+2,i3,kd)-u ## x63r(i1,i2-2,i3,kd))+(u ## x63r(i1,i2+3,i3,kd)-u ## x63r(i1,i2-3,i3,kd)))*h16(1)
u ## xz63r(i1,i2,i3,kd)=(45.*(u ## x63r(i1,i2,i3+1,kd)-u ## x63r(i1,i2,i3-1,kd))-9.*(u ## x63r(i1,i2,i3+2,kd)-u ## x63r(i1,i2,i3-2,kd))+(u ## x63r(i1,i2,i3+3,kd)-u ## x63r(i1,i2,i3-3,kd)))*h16(2)
u ## yz63r(i1,i2,i3,kd)=(45.*(u ## y63r(i1,i2,i3+1,kd)-u ## y63r(i1,i2,i3-1,kd))-9.*(u ## y63r(i1,i2,i3+2,kd)-u ## y63r(i1,i2,i3-2,kd))+(u ## y63r(i1,i2,i3+3,kd)-u ## y63r(i1,i2,i3-3,kd)))*h16(2)

u ## x61r(i1,i2,i3,kd)= u ## x63r(i1,i2,i3,kd)
u ## y61r(i1,i2,i3,kd)= u ## y63r(i1,i2,i3,kd)
u ## z61r(i1,i2,i3,kd)= u ## z63r(i1,i2,i3,kd)
u ## xx61r(i1,i2,i3,kd)= u ## xx63r(i1,i2,i3,kd)
u ## yy61r(i1,i2,i3,kd)= u ## yy63r(i1,i2,i3,kd)
u ## zz61r(i1,i2,i3,kd)= u ## zz63r(i1,i2,i3,kd)
u ## xy61r(i1,i2,i3,kd)= u ## xy63r(i1,i2,i3,kd)
u ## xz61r(i1,i2,i3,kd)= u ## xz63r(i1,i2,i3,kd)
u ## yz61r(i1,i2,i3,kd)= u ## yz63r(i1,i2,i3,kd)
u ## laplacian61r(i1,i2,i3,kd)=u ## xx63r(i1,i2,i3,kd)
u ## x62r(i1,i2,i3,kd)= u ## x63r(i1,i2,i3,kd)
u ## y62r(i1,i2,i3,kd)= u ## y63r(i1,i2,i3,kd)
u ## z62r(i1,i2,i3,kd)= u ## z63r(i1,i2,i3,kd)
u ## xx62r(i1,i2,i3,kd)= u ## xx63r(i1,i2,i3,kd)
u ## yy62r(i1,i2,i3,kd)= u ## yy63r(i1,i2,i3,kd)
u ## zz62r(i1,i2,i3,kd)= u ## zz63r(i1,i2,i3,kd)
u ## xy62r(i1,i2,i3,kd)= u ## xy63r(i1,i2,i3,kd)
u ## xz62r(i1,i2,i3,kd)= u ## xz63r(i1,i2,i3,kd)
u ## yz62r(i1,i2,i3,kd)= u ## yz63r(i1,i2,i3,kd)
u ## laplacian62r(i1,i2,i3,kd)=u ## xx63r(i1,i2,i3,kd)+u ## yy63r(i1,i2,i3,kd)
u ## laplacian63r(i1,i2,i3,kd)=u ## xx63r(i1,i2,i3,kd)+u ## yy63r(i1,i2,i3,kd)+u ## zz63r(i1,i2,i3,kd)
#endMacro
