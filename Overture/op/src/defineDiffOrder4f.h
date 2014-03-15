c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX
#beginMacro declareDifferenceOrder4(u,OPTION)
#If #OPTION == "RX"
 real d14
 real d24
 real h41
 real h42

 real rxr4
 real rxs4
 real rxt4
 real ryr4
 real rys4
 real ryt4
 real rzr4
 real rzs4
 real rzt4
 real sxr4
 real sxs4
 real sxt4
 real syr4
 real sys4
 real syt4
 real szr4
 real szs4
 real szt4
 real txr4
 real txs4
 real txt4
 real tyr4
 real tys4
 real tyt4
 real tzr4
 real tzs4
 real tzt4
 real rxx41
 real rxx42
 real rxy42
 real rxx43
 real rxy43
 real rxz43
 real ryx42
 real ryy42
 real ryx43
 real ryy43
 real ryz43
 real rzx42
 real rzy42
 real rzx43
 real rzy43
 real rzz43
 real sxx42
 real sxy42
 real sxx43
 real sxy43
 real sxz43
 real syx42
 real syy42
 real syx43
 real syy43
 real syz43
 real szx42
 real szy42
 real szx43
 real szy43
 real szz43
 real txx42
 real txy42
 real txx43
 real txy43
 real txz43
 real tyx42
 real tyy42
 real tyx43
 real tyy43
 real tyz43
 real tzx42
 real tzy42
 real tzx43
 real tzy43
 real tzz43
#End
 real u ## r4
 real u ## s4
 real u ## t4
 real u ## rr4
 real u ## ss4
 real u ## tt4
 real u ## rs4
 real u ## rt4
 real u ## st4
 real u ## x41
 real u ## y41
 real u ## z41
 real u ## x42
 real u ## y42
 real u ## z42
 real u ## x43
 real u ## y43
 real u ## z43
 real u ## xx41
 real u ## yy41
 real u ## xy41
 real u ## xz41
 real u ## yz41
 real u ## zz41
 real u ## laplacian41
 real u ## xx42
 real u ## yy42
 real u ## xy42
 real u ## xz42
 real u ## yz42
 real u ## zz42
 real u ## laplacian42
 real u ## xx43
 real u ## yy43
 real u ## zz43
 real u ## xy43
 real u ## xz43
 real u ## yz43
 real u ## laplacian43
 real u ## x43r
 real u ## y43r
 real u ## z43r
 real u ## xx43r
 real u ## yy43r
 real u ## zz43r
 real u ## xy43r
 real u ## xz43r
 real u ## yz43r
 real u ## x41r
 real u ## y41r
 real u ## z41r
 real u ## xx41r
 real u ## yy41r
 real u ## zz41r
 real u ## xy41r
 real u ## xz41r
 real u ## yz41r
 real u ## laplacian41r
 real u ## x42r
 real u ## y42r
 real u ## z42r
 real u ## xx42r
 real u ## yy42r
 real u ## zz42r
 real u ## xy42r
 real u ## xz42r
 real u ## yz42r
 real u ## laplacian42r
 real u ## laplacian43r
#endMacro


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder4Components0(u,OPTION)

#If #OPTION == "RX"
d14(kd) = 1./(12.*dr(kd))
d24(kd) = 1./(12.*dr(kd)**2)
#End

u ## r4(i1,i2,i3)=(8.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-(u(i1+2,i2,i3)-u(i1-2,i2,i3)))*d14(0)
u ## s4(i1,i2,i3)=(8.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-(u(i1,i2+2,i3)-u(i1,i2-2,i3)))*d14(1)
u ## t4(i1,i2,i3)=(8.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-(u(i1,i2,i3+2)-u(i1,i2,i3-2)))*d14(2)

u ## rr4(i1,i2,i3)=(-30.*u(i1,i2,i3)+16.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-(u(i1+2,i2,i3)+u(i1-2,i2,i3)) )*d24(0)
u ## ss4(i1,i2,i3)=(-30.*u(i1,i2,i3)+16.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-(u(i1,i2+2,i3)+u(i1,i2-2,i3)) )*d24(1)
u ## tt4(i1,i2,i3)=(-30.*u(i1,i2,i3)+16.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-(u(i1,i2,i3+2)+u(i1,i2,i3-2)) )*d24(2)
u ## rs4(i1,i2,i3)=(8.*(u ## r4(i1,i2+1,i3)-u ## r4(i1,i2-1,i3))-(u ## r4(i1,i2+2,i3)-u ## r4(i1,i2-2,i3)))*d14(1)
u ## rt4(i1,i2,i3)=(8.*(u ## r4(i1,i2,i3+1)-u ## r4(i1,i2,i3-1))-(u ## r4(i1,i2,i3+2)-u ## r4(i1,i2,i3-2)))*d14(2)
u ## st4(i1,i2,i3)=(8.*(u ## s4(i1,i2,i3+1)-u ## s4(i1,i2,i3-1))-(u ## s4(i1,i2,i3+2)-u ## s4(i1,i2,i3-2)))*d14(2)

#If #OPTION == "RX"
rxr4(i1,i2,i3)=(8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-(rx(i1+2,i2,i3)-rx(i1-2,i2,i3)))*d14(0)
rxs4(i1,i2,i3)=(8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-(rx(i1,i2+2,i3)-rx(i1,i2-2,i3)))*d14(1)
rxt4(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,i3+2)-rx(i1,i2,i3-2)))*d14(2)
ryr4(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,i2,i3)-ry(i1-2,i2,i3)))*d14(0)
rys4(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+2,i3)-ry(i1,i2-2,i3)))*d14(1)
ryt4(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,i3+2)-ry(i1,i2,i3-2)))*d14(2)
rzr4(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,i2,i3)-rz(i1-2,i2,i3)))*d14(0)
rzs4(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+2,i3)-rz(i1,i2-2,i3)))*d14(1)
rzt4(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,i3+2)-rz(i1,i2,i3-2)))*d14(2)
sxr4(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,i2,i3)-sx(i1-2,i2,i3)))*d14(0)
sxs4(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+2,i3)-sx(i1,i2-2,i3)))*d14(1)
sxt4(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,i3+2)-sx(i1,i2,i3-2)))*d14(2)
syr4(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,i2,i3)-sy(i1-2,i2,i3)))*d14(0)
sys4(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+2,i3)-sy(i1,i2-2,i3)))*d14(1)
syt4(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,i3+2)-sy(i1,i2,i3-2)))*d14(2)
szr4(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,i2,i3)-sz(i1-2,i2,i3)))*d14(0)
szs4(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+2,i3)-sz(i1,i2-2,i3)))*d14(1)
szt4(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,i3+2)-sz(i1,i2,i3-2)))*d14(2)
txr4(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,i2,i3)-tx(i1-2,i2,i3)))*d14(0)
txs4(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+2,i3)-tx(i1,i2-2,i3)))*d14(1)
txt4(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,i3+2)-tx(i1,i2,i3-2)))*d14(2)
tyr4(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,i2,i3)-ty(i1-2,i2,i3)))*d14(0)
tys4(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+2,i3)-ty(i1,i2-2,i3)))*d14(1)
tyt4(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,i3+2)-ty(i1,i2,i3-2)))*d14(2)
tzr4(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,i2,i3)-tz(i1-2,i2,i3)))*d14(0)
tzs4(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+2,i3)-tz(i1,i2-2,i3)))*d14(1)
tzt4(i1,i2,i3)=(8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-(tz(i1,i2,i3+2)-tz(i1,i2,i3-2)))*d14(2)
#End

u ## x41(i1,i2,i3)= rx(i1,i2,i3)*u ## r4(i1,i2,i3)
u ## y41(i1,i2,i3)=0
u ## z41(i1,i2,i3)=0

u ## x42(i1,i2,i3)= rx(i1,i2,i3)*u ## r4(i1,i2,i3)+sx(i1,i2,i3)*u ## s4(i1,i2,i3)
u ## y42(i1,i2,i3)= ry(i1,i2,i3)*u ## r4(i1,i2,i3)+sy(i1,i2,i3)*u ## s4(i1,i2,i3)
u ## z42(i1,i2,i3)=0
u ## x43(i1,i2,i3)=rx(i1,i2,i3)*u ## r4(i1,i2,i3)+sx(i1,i2,i3)*u ## s4(i1,i2,i3)+tx(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## y43(i1,i2,i3)=ry(i1,i2,i3)*u ## r4(i1,i2,i3)+sy(i1,i2,i3)*u ## s4(i1,i2,i3)+ty(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## z43(i1,i2,i3)=rz(i1,i2,i3)*u ## r4(i1,i2,i3)+sz(i1,i2,i3)*u ## s4(i1,i2,i3)+tz(i1,i2,i3)*u ## t4(i1,i2,i3)

#If #OPTION == "RX"
rxx41(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)
rxx42(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,i2,i3)
rxy42(i1,i2,i3)= ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,i2,i3)
rxx43(i1,i2,i3)=rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
rxy43(i1,i2,i3)=ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
rxz43(i1,i2,i3)=rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(i1,i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
ryx42(i1,i2,i3)= rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,i2,i3)
ryy42(i1,i2,i3)= ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,i2,i3)
ryx43(i1,i2,i3)=rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
ryy43(i1,i2,i3)=ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
ryz43(i1,i2,i3)=rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(i1,i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
rzx42(i1,i2,i3)= rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,i2,i3)
rzy42(i1,i2,i3)= ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,i2,i3)
rzx43(i1,i2,i3)=rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
rzy43(i1,i2,i3)=ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
rzz43(i1,i2,i3)=rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(i1,i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
sxx42(i1,i2,i3)= rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,i2,i3)
sxy42(i1,i2,i3)= ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,i2,i3)
sxx43(i1,i2,i3)=rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
sxy43(i1,i2,i3)=ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
sxz43(i1,i2,i3)=rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(i1,i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
syx42(i1,i2,i3)= rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,i2,i3)
syy42(i1,i2,i3)= ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,i2,i3)
syx43(i1,i2,i3)=rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
syy43(i1,i2,i3)=ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
syz43(i1,i2,i3)=rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(i1,i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
szx42(i1,i2,i3)= rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,i2,i3)
szy42(i1,i2,i3)= ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,i2,i3)
szx43(i1,i2,i3)=rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
szy43(i1,i2,i3)=ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
szz43(i1,i2,i3)=rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(i1,i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
txx42(i1,i2,i3)= rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,i2,i3)
txy42(i1,i2,i3)= ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,i2,i3)
txx43(i1,i2,i3)=rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
txy43(i1,i2,i3)=ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
txz43(i1,i2,i3)=rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(i1,i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
tyx42(i1,i2,i3)= rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,i2,i3)
tyy42(i1,i2,i3)= ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,i2,i3)
tyx43(i1,i2,i3)=rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
tyy43(i1,i2,i3)=ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
tyz43(i1,i2,i3)=rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(i1,i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
tzx42(i1,i2,i3)= rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,i2,i3)
tzy42(i1,i2,i3)= ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,i2,i3)
tzx43(i1,i2,i3)=rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,i2,i3)+tx(i1,i2,i3)*tzt4(i1,i2,i3)
tzy43(i1,i2,i3)=ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,i2,i3)+ty(i1,i2,i3)*tzt4(i1,i2,i3)
tzz43(i1,i2,i3)=rz(i1,i2,i3)*tzr4(i1,i2,i3)+sz(i1,i2,i3)*tzs4(i1,i2,i3)+tz(i1,i2,i3)*tzt4(i1,i2,i3)
#End

u ## xx41(i1,i2,i3)=(rx(i1,i2,i3)**2)*u ## rr4(i1,i2,i3)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3)
u ## yy41(i1,i2,i3)=0
u ## xy41(i1,i2,i3)=0
u ## xz41(i1,i2,i3)=0
u ## yz41(i1,i2,i3)=0
u ## zz41(i1,i2,i3)=0
u ## laplacian41(i1,i2,i3)=u ## xx41(i1,i2,i3)
u ## xx42(i1,i2,i3)=(rx(i1,i2,i3)**2)*u ## rr4(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+(sx(i1,i2,i3)**2)*u ## ss4(i1,i2,i3)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3)+(sxx42(i1,i2,i3))*u ## s4(i1,i2,i3)
u ## yy42(i1,i2,i3)=(ry(i1,i2,i3)**2)*u ## rr4(i1,i2,i3)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3)+(sy(i1,i2,i3)**2)*u ## ss4(i1,i2,i3)+(ryy42(i1,i2,i3))*u ## r4(i1,i2,i3)+(syy42(i1,i2,i3))*u ## s4(i1,i2,i3)
u ## xy42(i1,i2,i3)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3)+rxy42(i1,i2,i3)*u ## r4(i1,i2,i3)+sxy42(i1,i2,i3)*u ## s4(i1,i2,i3)
u ## xz42(i1,i2,i3)=0
u ## yz42(i1,i2,i3)=0
u ## zz42(i1,i2,i3)=0
u ## laplacian42(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*u ## rr4(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*u ## ss4(i1,i2,i3)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*u ## r4(i1,i2,i3)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*u ## s4(i1,i2,i3)
u ## xx43(i1,i2,i3)=rx(i1,i2,i3)**2*u ## rr4(i1,i2,i3)+sx(i1,i2,i3)**2*u ## ss4(i1,i2,i3)+tx(i1,i2,i3)**2*u ## tt4(i1,i2,i3)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs4(i1,i2,i3)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt4(i1,i2,i3)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st4(i1,i2,i3)+rxx43(i1,i2,i3)*u ## r4(i1,i2,i3)+sxx43(i1,i2,i3)*u ## s4(i1,i2,i3)+txx43(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## yy43(i1,i2,i3)=ry(i1,i2,i3)**2*u ## rr4(i1,i2,i3)+sy(i1,i2,i3)**2*u ## ss4(i1,i2,i3)+ty(i1,i2,i3)**2*u ## tt4(i1,i2,i3)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs4(i1,i2,i3)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt4(i1,i2,i3)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st4(i1,i2,i3)+ryy43(i1,i2,i3)*u ## r4(i1,i2,i3)+syy43(i1,i2,i3)*u ## s4(i1,i2,i3)+tyy43(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## zz43(i1,i2,i3)=rz(i1,i2,i3)**2*u ## rr4(i1,i2,i3)+sz(i1,i2,i3)**2*u ## ss4(i1,i2,i3)+tz(i1,i2,i3)**2*u ## tt4(i1,i2,i3)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs4(i1,i2,i3)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt4(i1,i2,i3)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st4(i1,i2,i3)+rzz43(i1,i2,i3)*u ## r4(i1,i2,i3)+szz43(i1,i2,i3)*u ## s4(i1,i2,i3)+tzz43(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## xy43(i1,i2,i3)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt4(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3)+rxy43(i1,i2,i3)*u ## r4(i1,i2,i3)+sxy43(i1,i2,i3)*u ## s4(i1,i2,i3)+txy43(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## xz43(i1,i2,i3)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3)+rxz43(i1,i2,i3)*u ## r4(i1,i2,i3)+sxz43(i1,i2,i3)*u ## s4(i1,i2,i3)+txz43(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## yz43(i1,i2,i3)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt4(i1,i2,i3)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st4(i1,i2,i3)+ryz43(i1,i2,i3)*u ## r4(i1,i2,i3)+syz43(i1,i2,i3)*u ## s4(i1,i2,i3)+tyz43(i1,i2,i3)*u ## t4(i1,i2,i3)
u ## laplacian43(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*u ## rr4(i1,i2,i3)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*u ## ss4(i1,i2,i3)+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*u ## tt4(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs4(i1,i2,i3)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt4(i1,i2,i3)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st4(i1,i2,i3)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*u ## r4(i1,i2,i3)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*u ## s4(i1,i2,i3)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*u ## t4(i1,i2,i3)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
#If #OPTION == "RX"
h41(kd) = 1./(12.*dx(kd))
h42(kd) = 1./(12.*dx(kd)**2)
#End
u ## x43r(i1,i2,i3)=(8.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-(u(i1+2,i2,i3)-u(i1-2,i2,i3)))*h41(0)
u ## y43r(i1,i2,i3)=(8.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-(u(i1,i2+2,i3)-u(i1,i2-2,i3)))*h41(1)
u ## z43r(i1,i2,i3)=(8.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-(u(i1,i2,i3+2)-u(i1,i2,i3-2)))*h41(2)
u ## xx43r(i1,i2,i3)=( -30.*u(i1,i2,i3)+16.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-(u(i1+2,i2,i3)+u(i1-2,i2,i3)) )*h42(0) 
u ## yy43r(i1,i2,i3)=( -30.*u(i1,i2,i3)+16.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-(u(i1,i2+2,i3)+u(i1,i2-2,i3)) )*h42(1) 
u ## zz43r(i1,i2,i3)=( -30.*u(i1,i2,i3)+16.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-(u(i1,i2,i3+2)+u(i1,i2,i3-2)) )*h42(2)
u ## xy43r(i1,i2,i3)=( (u(i1+2,i2+2,i3)-u(i1-2,i2+2,i3)- u(i1+2,i2-2,i3)+u(i1-2,i2-2,i3)) +8.*(u(i1-1,i2+2,i3)-u(i1-1,i2-2,i3)-u(i1+1,i2+2,i3)+u(i1+1,i2-2,i3) +u(i1+2,i2-1,i3)-u(i1-2,i2-1,i3)-u(i1+2,i2+1,i3)+u(i1-2,i2+1,i3))+64.*(u(i1+1,i2+1,i3)-u(i1-1,i2+1,i3)- u(i1+1,i2-1,i3)+u(i1-1,i2-1,i3)))*(h41(0)*h41(1))
u ## xz43r(i1,i2,i3)=( (u(i1+2,i2,i3+2)-u(i1-2,i2,i3+2)-u(i1+2,i2,i3-2)+u(i1-2,i2,i3-2)) +8.*(u(i1-1,i2,i3+2)-u(i1-1,i2,i3-2)-u(i1+1,i2,i3+2)+u(i1+1,i2,i3-2) +u(i1+2,i2,i3-1)-u(i1-2,i2,i3-1)- u(i1+2,i2,i3+1)+u(i1-2,i2,i3+1)) +64.*(u(i1+1,i2,i3+1)-u(i1-1,i2,i3+1)-u(i1+1,i2,i3-1)+u(i1-1,i2,i3-1)) )*(h41(0)*h41(2))
u ## yz43r(i1,i2,i3)=( (u(i1,i2+2,i3+2)-u(i1,i2-2,i3+2)-u(i1,i2+2,i3-2)+u(i1,i2-2,i3-2)) +8.*(u(i1,i2-1,i3+2)-u(i1,i2-1,i3-2)-u(i1,i2+1,i3+2)+u(i1,i2+1,i3-2) +u(i1,i2+2,i3-1)-u(i1,i2-2,i3-1)-u(i1,i2+2,i3+1)+u(i1,i2-2,i3+1)) +64.*(u(i1,i2+1,i3+1)-u(i1,i2-1,i3+1)-u(i1,i2+1,i3-1)+u(i1,i2-1,i3-1)) )*(h41(1)*h41(2))
u ## x41r(i1,i2,i3)= u ## x43r(i1,i2,i3)
u ## y41r(i1,i2,i3)= u ## y43r(i1,i2,i3)
u ## z41r(i1,i2,i3)= u ## z43r(i1,i2,i3)
u ## xx41r(i1,i2,i3)= u ## xx43r(i1,i2,i3)
u ## yy41r(i1,i2,i3)= u ## yy43r(i1,i2,i3)
u ## zz41r(i1,i2,i3)= u ## zz43r(i1,i2,i3)
u ## xy41r(i1,i2,i3)= u ## xy43r(i1,i2,i3)
u ## xz41r(i1,i2,i3)= u ## xz43r(i1,i2,i3)
u ## yz41r(i1,i2,i3)= u ## yz43r(i1,i2,i3)
u ## laplacian41r(i1,i2,i3)=u ## xx43r(i1,i2,i3)
u ## x42r(i1,i2,i3)= u ## x43r(i1,i2,i3)
u ## y42r(i1,i2,i3)= u ## y43r(i1,i2,i3)
u ## z42r(i1,i2,i3)= u ## z43r(i1,i2,i3)
u ## xx42r(i1,i2,i3)= u ## xx43r(i1,i2,i3)
u ## yy42r(i1,i2,i3)= u ## yy43r(i1,i2,i3)
u ## zz42r(i1,i2,i3)= u ## zz43r(i1,i2,i3)
u ## xy42r(i1,i2,i3)= u ## xy43r(i1,i2,i3)
u ## xz42r(i1,i2,i3)= u ## xz43r(i1,i2,i3)
u ## yz42r(i1,i2,i3)= u ## yz43r(i1,i2,i3)
u ## laplacian42r(i1,i2,i3)=u ## xx43r(i1,i2,i3)+u ## yy43r(i1,i2,i3)
u ## laplacian43r(i1,i2,i3)=u ## xx43r(i1,i2,i3)+u ## yy43r(i1,i2,i3)+u ## zz43r(i1,i2,i3)
#endMacro
c To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder4Components1(u,OPTION)

#If #OPTION == "RX"
d14(kd) = 1./(12.*dr(kd))
d24(kd) = 1./(12.*dr(kd)**2)
#End

u ## r4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)
u ## s4(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(1)
u ## t4(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(2)

u ## rr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(0)
u ## ss4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(1)
u ## tt4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(2)
u ## rs4(i1,i2,i3,kd)=(8.*(u ## r4(i1,i2+1,i3,kd)-u ## r4(i1,i2-1,i3,kd))-(u ## r4(i1,i2+2,i3,kd)-u ## r4(i1,i2-2,i3,kd)))*d14(1)
u ## rt4(i1,i2,i3,kd)=(8.*(u ## r4(i1,i2,i3+1,kd)-u ## r4(i1,i2,i3-1,kd))-(u ## r4(i1,i2,i3+2,kd)-u ## r4(i1,i2,i3-2,kd)))*d14(2)
u ## st4(i1,i2,i3,kd)=(8.*(u ## s4(i1,i2,i3+1,kd)-u ## s4(i1,i2,i3-1,kd))-(u ## s4(i1,i2,i3+2,kd)-u ## s4(i1,i2,i3-2,kd)))*d14(2)

#If #OPTION == "RX"
rxr4(i1,i2,i3)=(8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-(rx(i1+2,i2,i3)-rx(i1-2,i2,i3)))*d14(0)
rxs4(i1,i2,i3)=(8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-(rx(i1,i2+2,i3)-rx(i1,i2-2,i3)))*d14(1)
rxt4(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,i3+2)-rx(i1,i2,i3-2)))*d14(2)
ryr4(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,i2,i3)-ry(i1-2,i2,i3)))*d14(0)
rys4(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+2,i3)-ry(i1,i2-2,i3)))*d14(1)
ryt4(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,i3+2)-ry(i1,i2,i3-2)))*d14(2)
rzr4(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,i2,i3)-rz(i1-2,i2,i3)))*d14(0)
rzs4(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+2,i3)-rz(i1,i2-2,i3)))*d14(1)
rzt4(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,i3+2)-rz(i1,i2,i3-2)))*d14(2)
sxr4(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,i2,i3)-sx(i1-2,i2,i3)))*d14(0)
sxs4(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+2,i3)-sx(i1,i2-2,i3)))*d14(1)
sxt4(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,i3+2)-sx(i1,i2,i3-2)))*d14(2)
syr4(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,i2,i3)-sy(i1-2,i2,i3)))*d14(0)
sys4(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+2,i3)-sy(i1,i2-2,i3)))*d14(1)
syt4(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,i3+2)-sy(i1,i2,i3-2)))*d14(2)
szr4(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,i2,i3)-sz(i1-2,i2,i3)))*d14(0)
szs4(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+2,i3)-sz(i1,i2-2,i3)))*d14(1)
szt4(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,i3+2)-sz(i1,i2,i3-2)))*d14(2)
txr4(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,i2,i3)-tx(i1-2,i2,i3)))*d14(0)
txs4(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+2,i3)-tx(i1,i2-2,i3)))*d14(1)
txt4(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,i3+2)-tx(i1,i2,i3-2)))*d14(2)
tyr4(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,i2,i3)-ty(i1-2,i2,i3)))*d14(0)
tys4(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+2,i3)-ty(i1,i2-2,i3)))*d14(1)
tyt4(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,i3+2)-ty(i1,i2,i3-2)))*d14(2)
tzr4(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,i2,i3)-tz(i1-2,i2,i3)))*d14(0)
tzs4(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+2,i3)-tz(i1,i2-2,i3)))*d14(1)
tzt4(i1,i2,i3)=(8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-(tz(i1,i2,i3+2)-tz(i1,i2,i3-2)))*d14(2)
#End

u ## x41(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r4(i1,i2,i3,kd)
u ## y41(i1,i2,i3,kd)=0
u ## z41(i1,i2,i3,kd)=0

u ## x42(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s4(i1,i2,i3,kd)
u ## y42(i1,i2,i3,kd)= ry(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s4(i1,i2,i3,kd)
u ## z42(i1,i2,i3,kd)=0
u ## x43(i1,i2,i3,kd)=rx(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tx(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## y43(i1,i2,i3,kd)=ry(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+ty(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## z43(i1,i2,i3,kd)=rz(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sz(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tz(i1,i2,i3)*u ## t4(i1,i2,i3,kd)

#If #OPTION == "RX"
rxx41(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)
rxx42(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,i2,i3)
rxy42(i1,i2,i3)= ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,i2,i3)
rxx43(i1,i2,i3)=rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
rxy43(i1,i2,i3)=ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
rxz43(i1,i2,i3)=rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(i1,i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
ryx42(i1,i2,i3)= rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,i2,i3)
ryy42(i1,i2,i3)= ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,i2,i3)
ryx43(i1,i2,i3)=rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
ryy43(i1,i2,i3)=ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
ryz43(i1,i2,i3)=rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(i1,i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
rzx42(i1,i2,i3)= rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,i2,i3)
rzy42(i1,i2,i3)= ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,i2,i3)
rzx43(i1,i2,i3)=rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
rzy43(i1,i2,i3)=ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
rzz43(i1,i2,i3)=rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(i1,i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
sxx42(i1,i2,i3)= rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,i2,i3)
sxy42(i1,i2,i3)= ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,i2,i3)
sxx43(i1,i2,i3)=rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
sxy43(i1,i2,i3)=ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
sxz43(i1,i2,i3)=rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(i1,i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
syx42(i1,i2,i3)= rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,i2,i3)
syy42(i1,i2,i3)= ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,i2,i3)
syx43(i1,i2,i3)=rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
syy43(i1,i2,i3)=ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
syz43(i1,i2,i3)=rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(i1,i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
szx42(i1,i2,i3)= rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,i2,i3)
szy42(i1,i2,i3)= ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,i2,i3)
szx43(i1,i2,i3)=rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
szy43(i1,i2,i3)=ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
szz43(i1,i2,i3)=rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(i1,i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
txx42(i1,i2,i3)= rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,i2,i3)
txy42(i1,i2,i3)= ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,i2,i3)
txx43(i1,i2,i3)=rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
txy43(i1,i2,i3)=ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
txz43(i1,i2,i3)=rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(i1,i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
tyx42(i1,i2,i3)= rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,i2,i3)
tyy42(i1,i2,i3)= ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,i2,i3)
tyx43(i1,i2,i3)=rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
tyy43(i1,i2,i3)=ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
tyz43(i1,i2,i3)=rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(i1,i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
tzx42(i1,i2,i3)= rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,i2,i3)
tzy42(i1,i2,i3)= ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,i2,i3)
tzx43(i1,i2,i3)=rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,i2,i3)+tx(i1,i2,i3)*tzt4(i1,i2,i3)
tzy43(i1,i2,i3)=ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,i2,i3)+ty(i1,i2,i3)*tzt4(i1,i2,i3)
tzz43(i1,i2,i3)=rz(i1,i2,i3)*tzr4(i1,i2,i3)+sz(i1,i2,i3)*tzs4(i1,i2,i3)+tz(i1,i2,i3)*tzt4(i1,i2,i3)
#End

u ## xx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u ## rr4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)
u ## yy41(i1,i2,i3,kd)=0
u ## xy41(i1,i2,i3,kd)=0
u ## xz41(i1,i2,i3,kd)=0
u ## yz41(i1,i2,i3,kd)=0
u ## zz41(i1,i2,i3,kd)=0
u ## laplacian41(i1,i2,i3,kd)=u ## xx41(i1,i2,i3,kd)
u ## xx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u ## rr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*u ## ss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(sxx42(i1,i2,i3))*u ## s4(i1,i2,i3,kd)
u ## yy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*u ## rr4(i1,i2,i3,kd)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*u ## ss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(syy42(i1,i2,i3))*u ## s4(i1,i2,i3,kd)
u ## xy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+rxy42(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*u ## s4(i1,i2,i3,kd)
u ## xz42(i1,i2,i3,kd)=0
u ## yz42(i1,i2,i3,kd)=0
u ## zz42(i1,i2,i3,kd)=0
u ## laplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*u ## rr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*u ## ss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*u ## s4(i1,i2,i3,kd)
u ## xx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*u ## rr4(i1,i2,i3,kd)+sx(i1,i2,i3)**2*u ## ss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*u ## tt4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxx43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+txx43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## yy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*u ## rr4(i1,i2,i3,kd)+sy(i1,i2,i3)**2*u ## ss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*u ## tt4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+syy43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## zz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*u ## rr4(i1,i2,i3,kd)+sz(i1,i2,i3)**2*u ## ss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*u ## tt4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+szz43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## xy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+rxy43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+txy43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## xz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+rxz43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+txz43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## yz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+ryz43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+syz43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## laplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*u ## rr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*u ## ss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*u ## tt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*u ## s4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*u ## t4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
#If #OPTION == "RX"
h41(kd) = 1./(12.*dx(kd))
h42(kd) = 1./(12.*dx(kd)**2)
#End
u ## x43r(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*h41(0)
u ## y43r(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*h41(1)
u ## z43r(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*h41(2)
u ## xx43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*h42(0) 
u ## yy43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*h42(1) 
u ## zz43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*h42(2)
u ## xy43r(i1,i2,i3,kd)=( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)- u(i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) +8.*(u(i1-1,i2+2,i3,kd)-u(i1-1,i2-2,i3,kd)-u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd) +u(i1+2,i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-u(i1+2,i2+1,i3,kd)+u(i1-2,i2+1,i3,kd))+64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)- u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
u ## xz43r(i1,i2,i3,kd)=( (u(i1+2,i2,i3+2,kd)-u(i1-2,i2,i3+2,kd)-u(i1+2,i2,i3-2,kd)+u(i1-2,i2,i3-2,kd)) +8.*(u(i1-1,i2,i3+2,kd)-u(i1-1,i2,i3-2,kd)-u(i1+1,i2,i3+2,kd)+u(i1+1,i2,i3-2,kd) +u(i1+2,i2,i3-1,kd)-u(i1-2,i2,i3-1,kd)- u(i1+2,i2,i3+1,kd)+u(i1-2,i2,i3+1,kd)) +64.*(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)-u(i1+1,i2,i3-1,kd)+u(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
u ## yz43r(i1,i2,i3,kd)=( (u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)-u(i1,i2+2,i3-2,kd)+u(i1,i2-2,i3-2,kd)) +8.*(u(i1,i2-1,i3+2,kd)-u(i1,i2-1,i3-2,kd)-u(i1,i2+1,i3+2,kd)+u(i1,i2+1,i3-2,kd) +u(i1,i2+2,i3-1,kd)-u(i1,i2-2,i3-1,kd)-u(i1,i2+2,i3+1,kd)+u(i1,i2-2,i3+1,kd)) +64.*(u(i1,i2+1,i3+1,kd)-u(i1,i2-1,i3+1,kd)-u(i1,i2+1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
u ## x41r(i1,i2,i3,kd)= u ## x43r(i1,i2,i3,kd)
u ## y41r(i1,i2,i3,kd)= u ## y43r(i1,i2,i3,kd)
u ## z41r(i1,i2,i3,kd)= u ## z43r(i1,i2,i3,kd)
u ## xx41r(i1,i2,i3,kd)= u ## xx43r(i1,i2,i3,kd)
u ## yy41r(i1,i2,i3,kd)= u ## yy43r(i1,i2,i3,kd)
u ## zz41r(i1,i2,i3,kd)= u ## zz43r(i1,i2,i3,kd)
u ## xy41r(i1,i2,i3,kd)= u ## xy43r(i1,i2,i3,kd)
u ## xz41r(i1,i2,i3,kd)= u ## xz43r(i1,i2,i3,kd)
u ## yz41r(i1,i2,i3,kd)= u ## yz43r(i1,i2,i3,kd)
u ## laplacian41r(i1,i2,i3,kd)=u ## xx43r(i1,i2,i3,kd)
u ## x42r(i1,i2,i3,kd)= u ## x43r(i1,i2,i3,kd)
u ## y42r(i1,i2,i3,kd)= u ## y43r(i1,i2,i3,kd)
u ## z42r(i1,i2,i3,kd)= u ## z43r(i1,i2,i3,kd)
u ## xx42r(i1,i2,i3,kd)= u ## xx43r(i1,i2,i3,kd)
u ## yy42r(i1,i2,i3,kd)= u ## yy43r(i1,i2,i3,kd)
u ## zz42r(i1,i2,i3,kd)= u ## zz43r(i1,i2,i3,kd)
u ## xy42r(i1,i2,i3,kd)= u ## xy43r(i1,i2,i3,kd)
u ## xz42r(i1,i2,i3,kd)= u ## xz43r(i1,i2,i3,kd)
u ## yz42r(i1,i2,i3,kd)= u ## yz43r(i1,i2,i3,kd)
u ## laplacian42r(i1,i2,i3,kd)=u ## xx43r(i1,i2,i3,kd)+u ## yy43r(i1,i2,i3,kd)
u ## laplacian43r(i1,i2,i3,kd)=u ## xx43r(i1,i2,i3,kd)+u ## yy43r(i1,i2,i3,kd)+u ## zz43r(i1,i2,i3,kd)
#endMacro
