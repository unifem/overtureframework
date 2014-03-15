// Define statement functions for difference approximations of order 4 
// To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder4Components0(u,OPTION)

#define #If #OPTION  = "RX"
#define d14(kd)   1./(12.*dr(kd))
#define d24(kd)   1./(12.*SQR(dr(kd)))
#End

#define u ## r4(i1,i2,i3) (8.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-(u(i1+2,i2,i3)-u(i1-2,i2,i3)))*d14(0)
#define u ## s4(i1,i2,i3) (8.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-(u(i1,i2+2,i3)-u(i1,i2-2,i3)))*d14(1)
#define u ## t4(i1,i2,i3) (8.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-(u(i1,i2,i3+2)-u(i1,i2,i3-2)))*d14(2)

#define u ## rr4(i1,i2,i3) (-30.*u(i1,i2,i3)+16.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-(u(i1+2,i2,i3)+u(i1-2,i2,i3)) )*d24(0)
#define u ## ss4(i1,i2,i3) (-30.*u(i1,i2,i3)+16.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-(u(i1,i2+2,i3)+u(i1,i2-2,i3)) )*d24(1)
#define u ## tt4(i1,i2,i3) (-30.*u(i1,i2,i3)+16.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-(u(i1,i2,i3+2)+u(i1,i2,i3-2)) )*d24(2)
#define u ## rs4(i1,i2,i3) (8.*(u ## r4(i1,i2+1,i3)-u ## r4(i1,i2-1,i3))-(u ## r4(i1,i2+2,i3)-u ## r4(i1,i2-2,i3)))*d14(1)
#define u ## rt4(i1,i2,i3) (8.*(u ## r4(i1,i2,i3+1)-u ## r4(i1,i2,i3-1))-(u ## r4(i1,i2,i3+2)-u ## r4(i1,i2,i3-2)))*d14(2)
#define u ## st4(i1,i2,i3) (8.*(u ## s4(i1,i2,i3+1)-u ## s4(i1,i2,i3-1))-(u ## s4(i1,i2,i3+2)-u ## s4(i1,i2,i3-2)))*d14(2)

#define #If #OPTION  = "RX"
#define rxr4(i1,i2,i3) (8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-(rx(i1+2,i2,i3)-rx(i1-2,i2,i3)))*d14(0)
#define rxs4(i1,i2,i3) (8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-(rx(i1,i2+2,i3)-rx(i1,i2-2,i3)))*d14(1)
#define rxt4(i1,i2,i3) (8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,i3+2)-rx(i1,i2,i3-2)))*d14(2)
#define ryr4(i1,i2,i3) (8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,i2,i3)-ry(i1-2,i2,i3)))*d14(0)
#define rys4(i1,i2,i3) (8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+2,i3)-ry(i1,i2-2,i3)))*d14(1)
#define ryt4(i1,i2,i3) (8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,i3+2)-ry(i1,i2,i3-2)))*d14(2)
#define rzr4(i1,i2,i3) (8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,i2,i3)-rz(i1-2,i2,i3)))*d14(0)
#define rzs4(i1,i2,i3) (8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+2,i3)-rz(i1,i2-2,i3)))*d14(1)
#define rzt4(i1,i2,i3) (8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,i3+2)-rz(i1,i2,i3-2)))*d14(2)
#define sxr4(i1,i2,i3) (8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,i2,i3)-sx(i1-2,i2,i3)))*d14(0)
#define sxs4(i1,i2,i3) (8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+2,i3)-sx(i1,i2-2,i3)))*d14(1)
#define sxt4(i1,i2,i3) (8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,i3+2)-sx(i1,i2,i3-2)))*d14(2)
#define syr4(i1,i2,i3) (8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,i2,i3)-sy(i1-2,i2,i3)))*d14(0)
#define sys4(i1,i2,i3) (8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+2,i3)-sy(i1,i2-2,i3)))*d14(1)
#define syt4(i1,i2,i3) (8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,i3+2)-sy(i1,i2,i3-2)))*d14(2)
#define szr4(i1,i2,i3) (8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,i2,i3)-sz(i1-2,i2,i3)))*d14(0)
#define szs4(i1,i2,i3) (8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+2,i3)-sz(i1,i2-2,i3)))*d14(1)
#define szt4(i1,i2,i3) (8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,i3+2)-sz(i1,i2,i3-2)))*d14(2)
#define txr4(i1,i2,i3) (8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,i2,i3)-tx(i1-2,i2,i3)))*d14(0)
#define txs4(i1,i2,i3) (8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+2,i3)-tx(i1,i2-2,i3)))*d14(1)
#define txt4(i1,i2,i3) (8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,i3+2)-tx(i1,i2,i3-2)))*d14(2)
#define tyr4(i1,i2,i3) (8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,i2,i3)-ty(i1-2,i2,i3)))*d14(0)
#define tys4(i1,i2,i3) (8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+2,i3)-ty(i1,i2-2,i3)))*d14(1)
#define tyt4(i1,i2,i3) (8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,i3+2)-ty(i1,i2,i3-2)))*d14(2)
#define tzr4(i1,i2,i3) (8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,i2,i3)-tz(i1-2,i2,i3)))*d14(0)
#define tzs4(i1,i2,i3) (8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+2,i3)-tz(i1,i2-2,i3)))*d14(1)
#define tzt4(i1,i2,i3) (8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-(tz(i1,i2,i3+2)-tz(i1,i2,i3-2)))*d14(2)
#End

#define u ## x41(i1,i2,i3)  rx(i1,i2,i3)*u ## r4(i1,i2,i3)
#define u ## y41(i1,i2,i3) 0
#define u ## z41(i1,i2,i3) 0

#define u ## x42(i1,i2,i3)  rx(i1,i2,i3)*u ## r4(i1,i2,i3)+sx(i1,i2,i3)*u ## s4(i1,i2,i3)
#define u ## y42(i1,i2,i3)  ry(i1,i2,i3)*u ## r4(i1,i2,i3)+sy(i1,i2,i3)*u ## s4(i1,i2,i3)
#define u ## z42(i1,i2,i3) 0
#define u ## x43(i1,i2,i3) rx(i1,i2,i3)*u ## r4(i1,i2,i3)+sx(i1,i2,i3)*u ## s4(i1,i2,i3)+tx(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## y43(i1,i2,i3) ry(i1,i2,i3)*u ## r4(i1,i2,i3)+sy(i1,i2,i3)*u ## s4(i1,i2,i3)+ty(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## z43(i1,i2,i3) rz(i1,i2,i3)*u ## r4(i1,i2,i3)+sz(i1,i2,i3)*u ## s4(i1,i2,i3)+tz(i1,i2,i3)*u ## t4(i1,i2,i3)

#define #If #OPTION  = "RX"
#define rxx41(i1,i2,i3)  rx(i1,i2,i3)*rxr4(i1,i2,i3)
#define rxx42(i1,i2,i3)  rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,i2,i3)
#define rxy42(i1,i2,i3)  ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,i2,i3)
#define rxx43(i1,i2,i3) rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
#define rxy43(i1,i2,i3) ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
#define rxz43(i1,i2,i3) rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(i1,i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
#define ryx42(i1,i2,i3)  rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,i2,i3)
#define ryy42(i1,i2,i3)  ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,i2,i3)
#define ryx43(i1,i2,i3) rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
#define ryy43(i1,i2,i3) ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
#define ryz43(i1,i2,i3) rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(i1,i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
#define rzx42(i1,i2,i3)  rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,i2,i3)
#define rzy42(i1,i2,i3)  ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,i2,i3)
#define rzx43(i1,i2,i3) rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
#define rzy43(i1,i2,i3) ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
#define rzz43(i1,i2,i3) rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(i1,i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
#define sxx42(i1,i2,i3)  rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,i2,i3)
#define sxy42(i1,i2,i3)  ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,i2,i3)
#define sxx43(i1,i2,i3) rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
#define sxy43(i1,i2,i3) ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
#define sxz43(i1,i2,i3) rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(i1,i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
#define syx42(i1,i2,i3)  rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,i2,i3)
#define syy42(i1,i2,i3)  ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,i2,i3)
#define syx43(i1,i2,i3) rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
#define syy43(i1,i2,i3) ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
#define syz43(i1,i2,i3) rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(i1,i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
#define szx42(i1,i2,i3)  rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,i2,i3)
#define szy42(i1,i2,i3)  ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,i2,i3)
#define szx43(i1,i2,i3) rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
#define szy43(i1,i2,i3) ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
#define szz43(i1,i2,i3) rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(i1,i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
#define txx42(i1,i2,i3)  rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,i2,i3)
#define txy42(i1,i2,i3)  ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,i2,i3)
#define txx43(i1,i2,i3) rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
#define txy43(i1,i2,i3) ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
#define txz43(i1,i2,i3) rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(i1,i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
#define tyx42(i1,i2,i3)  rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,i2,i3)
#define tyy42(i1,i2,i3)  ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,i2,i3)
#define tyx43(i1,i2,i3) rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
#define tyy43(i1,i2,i3) ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
#define tyz43(i1,i2,i3) rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(i1,i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
#define tzx42(i1,i2,i3)  rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,i2,i3)
#define tzy42(i1,i2,i3)  ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,i2,i3)
#define tzx43(i1,i2,i3) rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,i2,i3)+tx(i1,i2,i3)*tzt4(i1,i2,i3)
#define tzy43(i1,i2,i3) ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,i2,i3)+ty(i1,i2,i3)*tzt4(i1,i2,i3)
#define tzz43(i1,i2,i3) rz(i1,i2,i3)*tzr4(i1,i2,i3)+sz(i1,i2,i3)*tzs4(i1,i2,i3)+tz(i1,i2,i3)*tzt4(i1,i2,i3)
#End

#define u ## xx41(i1,i2,i3) (SQR(rx(i1,i2,i3)))*u ## rr4(i1,i2,i3)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3)
#define u ## yy41(i1,i2,i3) 0
#define u ## xy41(i1,i2,i3) 0
#define u ## xz41(i1,i2,i3) 0
#define u ## yz41(i1,i2,i3) 0
#define u ## zz41(i1,i2,i3) 0
#define u ## laplacian41(i1,i2,i3) u ## xx41(i1,i2,i3)
#define u ## xx42(i1,i2,i3) (SQR(rx(i1,i2,i3)))*u ## rr4(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+(SQR(sx(i1,i2,i3)))*u ## ss4(i1,i2,i3)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3)+(sxx42(i1,i2,i3))*u ## s4(i1,i2,i3)
#define u ## yy42(i1,i2,i3) (SQR(ry(i1,i2,i3)))*u ## rr4(i1,i2,i3)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3)+(SQR(sy(i1,i2,i3)))*u ## ss4(i1,i2,i3)+(ryy42(i1,i2,i3))*u ## r4(i1,i2,i3)+(syy42(i1,i2,i3))*u ## s4(i1,i2,i3)
#define u ## xy42(i1,i2,i3) rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3)+rxy42(i1,i2,i3)*u ## r4(i1,i2,i3)+sxy42(i1,i2,i3)*u ## s4(i1,i2,i3)
#define u ## xz42(i1,i2,i3) 0
#define u ## yz42(i1,i2,i3) 0
#define u ## zz42(i1,i2,i3) 0
#define u ## laplacian42(i1,i2,i3) (SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3)))*u ## rr4(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3)))*u ## ss4(i1,i2,i3)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*u ## r4(i1,i2,i3)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*u ## s4(i1,i2,i3)
#define u ## xx43(i1,i2,i3) SQR(rx(i1,i2,i3))*u ## rr4(i1,i2,i3)+SQR(sx(i1,i2,i3))*u ## ss4(i1,i2,i3)+SQR(tx(i1,i2,i3))*u ## tt4(i1,i2,i3)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs4(i1,i2,i3)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt4(i1,i2,i3)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st4(i1,i2,i3)+rxx43(i1,i2,i3)*u ## r4(i1,i2,i3)+sxx43(i1,i2,i3)*u ## s4(i1,i2,i3)+txx43(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## yy43(i1,i2,i3) SQR(ry(i1,i2,i3))*u ## rr4(i1,i2,i3)+SQR(sy(i1,i2,i3))*u ## ss4(i1,i2,i3)+SQR(ty(i1,i2,i3))*u ## tt4(i1,i2,i3)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs4(i1,i2,i3)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt4(i1,i2,i3)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st4(i1,i2,i3)+ryy43(i1,i2,i3)*u ## r4(i1,i2,i3)+syy43(i1,i2,i3)*u ## s4(i1,i2,i3)+tyy43(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## zz43(i1,i2,i3) SQR(rz(i1,i2,i3))*u ## rr4(i1,i2,i3)+SQR(sz(i1,i2,i3))*u ## ss4(i1,i2,i3)+SQR(tz(i1,i2,i3))*u ## tt4(i1,i2,i3)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs4(i1,i2,i3)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt4(i1,i2,i3)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st4(i1,i2,i3)+rzz43(i1,i2,i3)*u ## r4(i1,i2,i3)+szz43(i1,i2,i3)*u ## s4(i1,i2,i3)+tzz43(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## xy43(i1,i2,i3) rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt4(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3)+rxy43(i1,i2,i3)*u ## r4(i1,i2,i3)+sxy43(i1,i2,i3)*u ## s4(i1,i2,i3)+txy43(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## xz43(i1,i2,i3) rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3)+rxz43(i1,i2,i3)*u ## r4(i1,i2,i3)+sxz43(i1,i2,i3)*u ## s4(i1,i2,i3)+txz43(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## yz43(i1,i2,i3) ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt4(i1,i2,i3)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st4(i1,i2,i3)+ryz43(i1,i2,i3)*u ## r4(i1,i2,i3)+syz43(i1,i2,i3)*u ## s4(i1,i2,i3)+tyz43(i1,i2,i3)*u ## t4(i1,i2,i3)
#define u ## laplacian43(i1,i2,i3) (SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3))+SQR(rz(i1,i2,i3)))*u ## rr4(i1,i2,i3)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3))+SQR(sz(i1,i2,i3)))*u ## ss4(i1,i2,i3)+(SQR(tx(i1,i2,i3))+SQR(ty(i1,i2,i3))+SQR(tz(i1,i2,i3)))*u ## tt4(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs4(i1,i2,i3)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt4(i1,i2,i3)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st4(i1,i2,i3)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*u ## r4(i1,i2,i3)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*u ## s4(i1,i2,i3)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*u ## t4(i1,i2,i3)
//============================================================================================
// Define derivatives for a rectangular grid
//
//============================================================================================
#define #If #OPTION  = "RX"
#define h41(kd)   1./(12.*dx(kd))
#define h42(kd)   1./(12.*SQR(dx(kd)))
#End
#define u ## x43r(i1,i2,i3) (8.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-(u(i1+2,i2,i3)-u(i1-2,i2,i3)))*h41(0)
#define u ## y43r(i1,i2,i3) (8.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-(u(i1,i2+2,i3)-u(i1,i2-2,i3)))*h41(1)
#define u ## z43r(i1,i2,i3) (8.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-(u(i1,i2,i3+2)-u(i1,i2,i3-2)))*h41(2)
#define u ## xx43r(i1,i2,i3) ( -30.*u(i1,i2,i3)+16.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-(u(i1+2,i2,i3)+u(i1-2,i2,i3)) )*h42(0) 
#define u ## yy43r(i1,i2,i3) ( -30.*u(i1,i2,i3)+16.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-(u(i1,i2+2,i3)+u(i1,i2-2,i3)) )*h42(1) 
#define u ## zz43r(i1,i2,i3) ( -30.*u(i1,i2,i3)+16.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-(u(i1,i2,i3+2)+u(i1,i2,i3-2)) )*h42(2)
#define u ## xy43r(i1,i2,i3) ( (u(i1+2,i2+2,i3)-u(i1-2,i2+2,i3)- u(i1+2,i2-2,i3)+u(i1-2,i2-2,i3)) +8.*(u(i1-1,i2+2,i3)-u(i1-1,i2-2,i3)-u(i1+1,i2+2,i3)+u(i1+1,i2-2,i3) +u(i1+2,i2-1,i3)-u(i1-2,i2-1,i3)-u(i1+2,i2+1,i3)+u(i1-2,i2+1,i3))+64.*(u(i1+1,i2+1,i3)-u(i1-1,i2+1,i3)- u(i1+1,i2-1,i3)+u(i1-1,i2-1,i3)))*(h41(0)*h41(1))
#define u ## xz43r(i1,i2,i3) ( (u(i1+2,i2,i3+2)-u(i1-2,i2,i3+2)-u(i1+2,i2,i3-2)+u(i1-2,i2,i3-2)) +8.*(u(i1-1,i2,i3+2)-u(i1-1,i2,i3-2)-u(i1+1,i2,i3+2)+u(i1+1,i2,i3-2) +u(i1+2,i2,i3-1)-u(i1-2,i2,i3-1)- u(i1+2,i2,i3+1)+u(i1-2,i2,i3+1)) +64.*(u(i1+1,i2,i3+1)-u(i1-1,i2,i3+1)-u(i1+1,i2,i3-1)+u(i1-1,i2,i3-1)) )*(h41(0)*h41(2))
#define u ## yz43r(i1,i2,i3) ( (u(i1,i2+2,i3+2)-u(i1,i2-2,i3+2)-u(i1,i2+2,i3-2)+u(i1,i2-2,i3-2)) +8.*(u(i1,i2-1,i3+2)-u(i1,i2-1,i3-2)-u(i1,i2+1,i3+2)+u(i1,i2+1,i3-2) +u(i1,i2+2,i3-1)-u(i1,i2-2,i3-1)-u(i1,i2+2,i3+1)+u(i1,i2-2,i3+1)) +64.*(u(i1,i2+1,i3+1)-u(i1,i2-1,i3+1)-u(i1,i2+1,i3-1)+u(i1,i2-1,i3-1)) )*(h41(1)*h41(2))
#define u ## x41r(i1,i2,i3)  u ## x43r(i1,i2,i3)
#define u ## y41r(i1,i2,i3)  u ## y43r(i1,i2,i3)
#define u ## z41r(i1,i2,i3)  u ## z43r(i1,i2,i3)
#define u ## xx41r(i1,i2,i3)  u ## xx43r(i1,i2,i3)
#define u ## yy41r(i1,i2,i3)  u ## yy43r(i1,i2,i3)
#define u ## zz41r(i1,i2,i3)  u ## zz43r(i1,i2,i3)
#define u ## xy41r(i1,i2,i3)  u ## xy43r(i1,i2,i3)
#define u ## xz41r(i1,i2,i3)  u ## xz43r(i1,i2,i3)
#define u ## yz41r(i1,i2,i3)  u ## yz43r(i1,i2,i3)
#define u ## laplacian41r(i1,i2,i3) u ## xx43r(i1,i2,i3)
#define u ## x42r(i1,i2,i3)  u ## x43r(i1,i2,i3)
#define u ## y42r(i1,i2,i3)  u ## y43r(i1,i2,i3)
#define u ## z42r(i1,i2,i3)  u ## z43r(i1,i2,i3)
#define u ## xx42r(i1,i2,i3)  u ## xx43r(i1,i2,i3)
#define u ## yy42r(i1,i2,i3)  u ## yy43r(i1,i2,i3)
#define u ## zz42r(i1,i2,i3)  u ## zz43r(i1,i2,i3)
#define u ## xy42r(i1,i2,i3)  u ## xy43r(i1,i2,i3)
#define u ## xz42r(i1,i2,i3)  u ## xz43r(i1,i2,i3)
#define u ## yz42r(i1,i2,i3)  u ## yz43r(i1,i2,i3)
#define u ## laplacian42r(i1,i2,i3) u ## xx43r(i1,i2,i3)+u ## yy43r(i1,i2,i3)
#define u ## laplacian43r(i1,i2,i3) u ## xx43r(i1,i2,i3)+u ## yy43r(i1,i2,i3)+u ## zz43r(i1,i2,i3)
#endMacro
// To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder4Components1(u,OPTION)

#If #OPTION == "RX"
d14(kd) = 1./(12.*dr(kd))
d24(kd) = 1./(12.*SQR(dr(kd)))
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

u ## xx41(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3)))*u ## rr4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)
u ## yy41(i1,i2,i3,kd)=0
u ## xy41(i1,i2,i3,kd)=0
u ## xz41(i1,i2,i3,kd)=0
u ## yz41(i1,i2,i3,kd)=0
u ## zz41(i1,i2,i3,kd)=0
u ## laplacian41(i1,i2,i3,kd)=u ## xx41(i1,i2,i3,kd)
u ## xx42(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3)))*u ## rr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(SQR(sx(i1,i2,i3)))*u ## ss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(sxx42(i1,i2,i3))*u ## s4(i1,i2,i3,kd)
u ## yy42(i1,i2,i3,kd)=(SQR(ry(i1,i2,i3)))*u ## rr4(i1,i2,i3,kd)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(SQR(sy(i1,i2,i3)))*u ## ss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(syy42(i1,i2,i3))*u ## s4(i1,i2,i3,kd)
u ## xy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+rxy42(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*u ## s4(i1,i2,i3,kd)
u ## xz42(i1,i2,i3,kd)=0
u ## yz42(i1,i2,i3,kd)=0
u ## zz42(i1,i2,i3,kd)=0
u ## laplacian42(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3)))*u ## rr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3)))*u ## ss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*u ## s4(i1,i2,i3,kd)
u ## xx43(i1,i2,i3,kd)=SQR(rx(i1,i2,i3))*u ## rr4(i1,i2,i3,kd)+SQR(sx(i1,i2,i3))*u ## ss4(i1,i2,i3,kd)+SQR(tx(i1,i2,i3))*u ## tt4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxx43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+txx43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## yy43(i1,i2,i3,kd)=SQR(ry(i1,i2,i3))*u ## rr4(i1,i2,i3,kd)+SQR(sy(i1,i2,i3))*u ## ss4(i1,i2,i3,kd)+SQR(ty(i1,i2,i3))*u ## tt4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+syy43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## zz43(i1,i2,i3,kd)=SQR(rz(i1,i2,i3))*u ## rr4(i1,i2,i3,kd)+SQR(sz(i1,i2,i3))*u ## ss4(i1,i2,i3,kd)+SQR(tz(i1,i2,i3))*u ## tt4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+szz43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## xy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+rxy43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+txy43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## xz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+rxz43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+txz43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## yz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr4(i1,i2,i3,kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+ryz43(i1,i2,i3)*u ## r4(i1,i2,i3,kd)+syz43(i1,i2,i3)*u ## s4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*u ## t4(i1,i2,i3,kd)
u ## laplacian43(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3))+SQR(rz(i1,i2,i3)))*u ## rr4(i1,i2,i3,kd)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3))+SQR(sz(i1,i2,i3)))*u ## ss4(i1,i2,i3,kd)+(SQR(tx(i1,i2,i3))+SQR(ty(i1,i2,i3))+SQR(tz(i1,i2,i3)))*u ## tt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*u ## r4(i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*u ## s4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*u ## t4(i1,i2,i3,kd)
//============================================================================================
// Define derivatives for a rectangular grid
//
//============================================================================================
#If #OPTION == "RX"
h41(kd) = 1./(12.*dx(kd))
h42(kd) = 1./(12.*SQR(dx(kd)))
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
