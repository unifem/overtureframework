// Define statement functions for difference approximations of order 8 
// To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder8Components0(u,OPTION)

#define #If #OPTION  = "RX"
#define d18(kd)   1./(840.*dr(kd))
#define d28(kd)   1./(5040.*SQR(dr(kd)))
#End

#define u ## r8(i1,i2,i3) (672.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-168.*(u(i1+2,i2,i3)-u(i1-2,i2,i3))+32.*(u(i1+3,i2,i3)-u(i1-3,i2,i3))-3.*(u(i1+4,i2,i3)-u(i1-4,i2,i3)))*d18(0)
#define u ## s8(i1,i2,i3) (672.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-168.*(u(i1,i2+2,i3)-u(i1,i2-2,i3))+32.*(u(i1,i2+3,i3)-u(i1,i2-3,i3))-3.*(u(i1,i2+4,i3)-u(i1,i2-4,i3)))*d18(1)
#define u ## t8(i1,i2,i3) (672.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-168.*(u(i1,i2,i3+2)-u(i1,i2,i3-2))+32.*(u(i1,i2,i3+3)-u(i1,i2,i3-3))-3.*(u(i1,i2,i3+4)-u(i1,i2,i3-4)))*d18(2)

#define u ## rr8(i1,i2,i3) (-14350.*u(i1,i2,i3)+8064.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-1008.*(u(i1+2,i2,i3)+u(i1-2,i2,i3))+128.*(u(i1+3,i2,i3)+u(i1-3,i2,i3))-9.*(u(i1+4,i2,i3)+u(i1-4,i2,i3)) )*d28(0)
#define u ## ss8(i1,i2,i3) (-14350.*u(i1,i2,i3)+8064.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-1008.*(u(i1,i2+2,i3)+u(i1,i2-2,i3))+128.*(u(i1,i2+3,i3)+u(i1,i2-3,i3))-9.*(u(i1,i2+4,i3)+u(i1,i2-4,i3)) )*d28(1)
#define u ## tt8(i1,i2,i3) (-14350.*u(i1,i2,i3)+8064.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-1008.*(u(i1,i2,i3+2)+u(i1,i2,i3-2))+128.*(u(i1,i2,i3+3)+u(i1,i2,i3-3))-9.*(u(i1,i2,i3+4)+u(i1,i2,i3-4)) )*d28(2)
#define u ## rs8(i1,i2,i3) (672.*(u ## r8(i1,i2+1,i3)-u ## r8(i1,i2-1,i3))-168.*(u ## r8(i1,i2+2,i3)-u ## r8(i1,i2-2,i3))+32.*(u ## r8(i1,i2+3,i3)-u ## r8(i1,i2-3,i3))-3.*(u ## r8(i1,i2+4,i3)-u ## r8(i1,i2-4,i3)))*d18(1)
#define u ## rt8(i1,i2,i3) (672.*(u ## r8(i1,i2,i3+1)-u ## r8(i1,i2,i3-1))-168.*(u ## r8(i1,i2,i3+2)-u ## r8(i1,i2,i3-2))+32.*(u ## r8(i1,i2,i3+3)-u ## r8(i1,i2,i3-3))-3.*(u ## r8(i1,i2,i3+4)-u ## r8(i1,i2,i3-4)))*d18(2)
#define u ## st8(i1,i2,i3) (672.*(u ## s8(i1,i2,i3+1)-u ## s8(i1,i2,i3-1))-168.*(u ## s8(i1,i2,i3+2)-u ## s8(i1,i2,i3-2))+32.*(u ## s8(i1,i2,i3+3)-u ## s8(i1,i2,i3-3))-3.*(u ## s8(i1,i2,i3+4)-u ## s8(i1,i2,i3-4)))*d18(2)

#define #If #OPTION  = "RX"
#define rxr8(i1,i2,i3) (672.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-168.*(rx(i1+2,i2,i3)-rx(i1-2,i2,i3))+32.*(rx(i1+3,i2,i3)-rx(i1-3,i2,i3))-3.*(rx(i1+4,i2,i3)-rx(i1-4,i2,i3)))*d18(0)
#define rxs8(i1,i2,i3) (672.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-168.*(rx(i1,i2+2,i3)-rx(i1,i2-2,i3))+32.*(rx(i1,i2+3,i3)-rx(i1,i2-3,i3))-3.*(rx(i1,i2+4,i3)-rx(i1,i2-4,i3)))*d18(1)
#define rxt8(i1,i2,i3) (672.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-168.*(rx(i1,i2,i3+2)-rx(i1,i2,i3-2))+32.*(rx(i1,i2,i3+3)-rx(i1,i2,i3-3))-3.*(rx(i1,i2,i3+4)-rx(i1,i2,i3-4)))*d18(2)
#define ryr8(i1,i2,i3) (672.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-168.*(ry(i1+2,i2,i3)-ry(i1-2,i2,i3))+32.*(ry(i1+3,i2,i3)-ry(i1-3,i2,i3))-3.*(ry(i1+4,i2,i3)-ry(i1-4,i2,i3)))*d18(0)
#define rys8(i1,i2,i3) (672.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-168.*(ry(i1,i2+2,i3)-ry(i1,i2-2,i3))+32.*(ry(i1,i2+3,i3)-ry(i1,i2-3,i3))-3.*(ry(i1,i2+4,i3)-ry(i1,i2-4,i3)))*d18(1)
#define ryt8(i1,i2,i3) (672.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-168.*(ry(i1,i2,i3+2)-ry(i1,i2,i3-2))+32.*(ry(i1,i2,i3+3)-ry(i1,i2,i3-3))-3.*(ry(i1,i2,i3+4)-ry(i1,i2,i3-4)))*d18(2)
#define rzr8(i1,i2,i3) (672.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-168.*(rz(i1+2,i2,i3)-rz(i1-2,i2,i3))+32.*(rz(i1+3,i2,i3)-rz(i1-3,i2,i3))-3.*(rz(i1+4,i2,i3)-rz(i1-4,i2,i3)))*d18(0)
#define rzs8(i1,i2,i3) (672.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-168.*(rz(i1,i2+2,i3)-rz(i1,i2-2,i3))+32.*(rz(i1,i2+3,i3)-rz(i1,i2-3,i3))-3.*(rz(i1,i2+4,i3)-rz(i1,i2-4,i3)))*d18(1)
#define rzt8(i1,i2,i3) (672.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-168.*(rz(i1,i2,i3+2)-rz(i1,i2,i3-2))+32.*(rz(i1,i2,i3+3)-rz(i1,i2,i3-3))-3.*(rz(i1,i2,i3+4)-rz(i1,i2,i3-4)))*d18(2)
#define sxr8(i1,i2,i3) (672.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-168.*(sx(i1+2,i2,i3)-sx(i1-2,i2,i3))+32.*(sx(i1+3,i2,i3)-sx(i1-3,i2,i3))-3.*(sx(i1+4,i2,i3)-sx(i1-4,i2,i3)))*d18(0)
#define sxs8(i1,i2,i3) (672.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-168.*(sx(i1,i2+2,i3)-sx(i1,i2-2,i3))+32.*(sx(i1,i2+3,i3)-sx(i1,i2-3,i3))-3.*(sx(i1,i2+4,i3)-sx(i1,i2-4,i3)))*d18(1)
#define sxt8(i1,i2,i3) (672.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-168.*(sx(i1,i2,i3+2)-sx(i1,i2,i3-2))+32.*(sx(i1,i2,i3+3)-sx(i1,i2,i3-3))-3.*(sx(i1,i2,i3+4)-sx(i1,i2,i3-4)))*d18(2)
#define syr8(i1,i2,i3) (672.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-168.*(sy(i1+2,i2,i3)-sy(i1-2,i2,i3))+32.*(sy(i1+3,i2,i3)-sy(i1-3,i2,i3))-3.*(sy(i1+4,i2,i3)-sy(i1-4,i2,i3)))*d18(0)
#define sys8(i1,i2,i3) (672.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-168.*(sy(i1,i2+2,i3)-sy(i1,i2-2,i3))+32.*(sy(i1,i2+3,i3)-sy(i1,i2-3,i3))-3.*(sy(i1,i2+4,i3)-sy(i1,i2-4,i3)))*d18(1)
#define syt8(i1,i2,i3) (672.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-168.*(sy(i1,i2,i3+2)-sy(i1,i2,i3-2))+32.*(sy(i1,i2,i3+3)-sy(i1,i2,i3-3))-3.*(sy(i1,i2,i3+4)-sy(i1,i2,i3-4)))*d18(2)
#define szr8(i1,i2,i3) (672.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-168.*(sz(i1+2,i2,i3)-sz(i1-2,i2,i3))+32.*(sz(i1+3,i2,i3)-sz(i1-3,i2,i3))-3.*(sz(i1+4,i2,i3)-sz(i1-4,i2,i3)))*d18(0)
#define szs8(i1,i2,i3) (672.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-168.*(sz(i1,i2+2,i3)-sz(i1,i2-2,i3))+32.*(sz(i1,i2+3,i3)-sz(i1,i2-3,i3))-3.*(sz(i1,i2+4,i3)-sz(i1,i2-4,i3)))*d18(1)
#define szt8(i1,i2,i3) (672.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-168.*(sz(i1,i2,i3+2)-sz(i1,i2,i3-2))+32.*(sz(i1,i2,i3+3)-sz(i1,i2,i3-3))-3.*(sz(i1,i2,i3+4)-sz(i1,i2,i3-4)))*d18(2)
#define txr8(i1,i2,i3) (672.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-168.*(tx(i1+2,i2,i3)-tx(i1-2,i2,i3))+32.*(tx(i1+3,i2,i3)-tx(i1-3,i2,i3))-3.*(tx(i1+4,i2,i3)-tx(i1-4,i2,i3)))*d18(0)
#define txs8(i1,i2,i3) (672.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-168.*(tx(i1,i2+2,i3)-tx(i1,i2-2,i3))+32.*(tx(i1,i2+3,i3)-tx(i1,i2-3,i3))-3.*(tx(i1,i2+4,i3)-tx(i1,i2-4,i3)))*d18(1)
#define txt8(i1,i2,i3) (672.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-168.*(tx(i1,i2,i3+2)-tx(i1,i2,i3-2))+32.*(tx(i1,i2,i3+3)-tx(i1,i2,i3-3))-3.*(tx(i1,i2,i3+4)-tx(i1,i2,i3-4)))*d18(2)
#define tyr8(i1,i2,i3) (672.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-168.*(ty(i1+2,i2,i3)-ty(i1-2,i2,i3))+32.*(ty(i1+3,i2,i3)-ty(i1-3,i2,i3))-3.*(ty(i1+4,i2,i3)-ty(i1-4,i2,i3)))*d18(0)
#define tys8(i1,i2,i3) (672.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-168.*(ty(i1,i2+2,i3)-ty(i1,i2-2,i3))+32.*(ty(i1,i2+3,i3)-ty(i1,i2-3,i3))-3.*(ty(i1,i2+4,i3)-ty(i1,i2-4,i3)))*d18(1)
#define tyt8(i1,i2,i3) (672.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-168.*(ty(i1,i2,i3+2)-ty(i1,i2,i3-2))+32.*(ty(i1,i2,i3+3)-ty(i1,i2,i3-3))-3.*(ty(i1,i2,i3+4)-ty(i1,i2,i3-4)))*d18(2)
#define tzr8(i1,i2,i3) (672.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-168.*(tz(i1+2,i2,i3)-tz(i1-2,i2,i3))+32.*(tz(i1+3,i2,i3)-tz(i1-3,i2,i3))-3.*(tz(i1+4,i2,i3)-tz(i1-4,i2,i3)))*d18(0)
#define tzs8(i1,i2,i3) (672.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-168.*(tz(i1,i2+2,i3)-tz(i1,i2-2,i3))+32.*(tz(i1,i2+3,i3)-tz(i1,i2-3,i3))-3.*(tz(i1,i2+4,i3)-tz(i1,i2-4,i3)))*d18(1)
#define tzt8(i1,i2,i3) (672.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-168.*(tz(i1,i2,i3+2)-tz(i1,i2,i3-2))+32.*(tz(i1,i2,i3+3)-tz(i1,i2,i3-3))-3.*(tz(i1,i2,i3+4)-tz(i1,i2,i3-4)))*d18(2)
#End

#define u ## x81(i1,i2,i3)  rx(i1,i2,i3)*u ## r8(i1,i2,i3)
#define u ## y81(i1,i2,i3) 0
#define u ## z81(i1,i2,i3) 0

#define u ## x82(i1,i2,i3)  rx(i1,i2,i3)*u ## r8(i1,i2,i3)+sx(i1,i2,i3)*u ## s8(i1,i2,i3)
#define u ## y82(i1,i2,i3)  ry(i1,i2,i3)*u ## r8(i1,i2,i3)+sy(i1,i2,i3)*u ## s8(i1,i2,i3)
#define u ## z82(i1,i2,i3) 0
#define u ## x83(i1,i2,i3) rx(i1,i2,i3)*u ## r8(i1,i2,i3)+sx(i1,i2,i3)*u ## s8(i1,i2,i3)+tx(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## y83(i1,i2,i3) ry(i1,i2,i3)*u ## r8(i1,i2,i3)+sy(i1,i2,i3)*u ## s8(i1,i2,i3)+ty(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## z83(i1,i2,i3) rz(i1,i2,i3)*u ## r8(i1,i2,i3)+sz(i1,i2,i3)*u ## s8(i1,i2,i3)+tz(i1,i2,i3)*u ## t8(i1,i2,i3)

#define #If #OPTION  = "RX"
#define rxx81(i1,i2,i3)  rx(i1,i2,i3)*rxr8(i1,i2,i3)
#define rxx82(i1,i2,i3)  rx(i1,i2,i3)*rxr8(i1,i2,i3)+sx(i1,i2,i3)*rxs8(i1,i2,i3)
#define rxy82(i1,i2,i3)  ry(i1,i2,i3)*rxr8(i1,i2,i3)+sy(i1,i2,i3)*rxs8(i1,i2,i3)
#define rxx83(i1,i2,i3) rx(i1,i2,i3)*rxr8(i1,i2,i3)+sx(i1,i2,i3)*rxs8(i1,i2,i3)+tx(i1,i2,i3)*rxt8(i1,i2,i3)
#define rxy83(i1,i2,i3) ry(i1,i2,i3)*rxr8(i1,i2,i3)+sy(i1,i2,i3)*rxs8(i1,i2,i3)+ty(i1,i2,i3)*rxt8(i1,i2,i3)
#define rxz83(i1,i2,i3) rz(i1,i2,i3)*rxr8(i1,i2,i3)+sz(i1,i2,i3)*rxs8(i1,i2,i3)+tz(i1,i2,i3)*rxt8(i1,i2,i3)
#define ryx82(i1,i2,i3)  rx(i1,i2,i3)*ryr8(i1,i2,i3)+sx(i1,i2,i3)*rys8(i1,i2,i3)
#define ryy82(i1,i2,i3)  ry(i1,i2,i3)*ryr8(i1,i2,i3)+sy(i1,i2,i3)*rys8(i1,i2,i3)
#define ryx83(i1,i2,i3) rx(i1,i2,i3)*ryr8(i1,i2,i3)+sx(i1,i2,i3)*rys8(i1,i2,i3)+tx(i1,i2,i3)*ryt8(i1,i2,i3)
#define ryy83(i1,i2,i3) ry(i1,i2,i3)*ryr8(i1,i2,i3)+sy(i1,i2,i3)*rys8(i1,i2,i3)+ty(i1,i2,i3)*ryt8(i1,i2,i3)
#define ryz83(i1,i2,i3) rz(i1,i2,i3)*ryr8(i1,i2,i3)+sz(i1,i2,i3)*rys8(i1,i2,i3)+tz(i1,i2,i3)*ryt8(i1,i2,i3)
#define rzx82(i1,i2,i3)  rx(i1,i2,i3)*rzr8(i1,i2,i3)+sx(i1,i2,i3)*rzs8(i1,i2,i3)
#define rzy82(i1,i2,i3)  ry(i1,i2,i3)*rzr8(i1,i2,i3)+sy(i1,i2,i3)*rzs8(i1,i2,i3)
#define rzx83(i1,i2,i3) rx(i1,i2,i3)*rzr8(i1,i2,i3)+sx(i1,i2,i3)*rzs8(i1,i2,i3)+tx(i1,i2,i3)*rzt8(i1,i2,i3)
#define rzy83(i1,i2,i3) ry(i1,i2,i3)*rzr8(i1,i2,i3)+sy(i1,i2,i3)*rzs8(i1,i2,i3)+ty(i1,i2,i3)*rzt8(i1,i2,i3)
#define rzz83(i1,i2,i3) rz(i1,i2,i3)*rzr8(i1,i2,i3)+sz(i1,i2,i3)*rzs8(i1,i2,i3)+tz(i1,i2,i3)*rzt8(i1,i2,i3)
#define sxx82(i1,i2,i3)  rx(i1,i2,i3)*sxr8(i1,i2,i3)+sx(i1,i2,i3)*sxs8(i1,i2,i3)
#define sxy82(i1,i2,i3)  ry(i1,i2,i3)*sxr8(i1,i2,i3)+sy(i1,i2,i3)*sxs8(i1,i2,i3)
#define sxx83(i1,i2,i3) rx(i1,i2,i3)*sxr8(i1,i2,i3)+sx(i1,i2,i3)*sxs8(i1,i2,i3)+tx(i1,i2,i3)*sxt8(i1,i2,i3)
#define sxy83(i1,i2,i3) ry(i1,i2,i3)*sxr8(i1,i2,i3)+sy(i1,i2,i3)*sxs8(i1,i2,i3)+ty(i1,i2,i3)*sxt8(i1,i2,i3)
#define sxz83(i1,i2,i3) rz(i1,i2,i3)*sxr8(i1,i2,i3)+sz(i1,i2,i3)*sxs8(i1,i2,i3)+tz(i1,i2,i3)*sxt8(i1,i2,i3)
#define syx82(i1,i2,i3)  rx(i1,i2,i3)*syr8(i1,i2,i3)+sx(i1,i2,i3)*sys8(i1,i2,i3)
#define syy82(i1,i2,i3)  ry(i1,i2,i3)*syr8(i1,i2,i3)+sy(i1,i2,i3)*sys8(i1,i2,i3)
#define syx83(i1,i2,i3) rx(i1,i2,i3)*syr8(i1,i2,i3)+sx(i1,i2,i3)*sys8(i1,i2,i3)+tx(i1,i2,i3)*syt8(i1,i2,i3)
#define syy83(i1,i2,i3) ry(i1,i2,i3)*syr8(i1,i2,i3)+sy(i1,i2,i3)*sys8(i1,i2,i3)+ty(i1,i2,i3)*syt8(i1,i2,i3)
#define syz83(i1,i2,i3) rz(i1,i2,i3)*syr8(i1,i2,i3)+sz(i1,i2,i3)*sys8(i1,i2,i3)+tz(i1,i2,i3)*syt8(i1,i2,i3)
#define szx82(i1,i2,i3)  rx(i1,i2,i3)*szr8(i1,i2,i3)+sx(i1,i2,i3)*szs8(i1,i2,i3)
#define szy82(i1,i2,i3)  ry(i1,i2,i3)*szr8(i1,i2,i3)+sy(i1,i2,i3)*szs8(i1,i2,i3)
#define szx83(i1,i2,i3) rx(i1,i2,i3)*szr8(i1,i2,i3)+sx(i1,i2,i3)*szs8(i1,i2,i3)+tx(i1,i2,i3)*szt8(i1,i2,i3)
#define szy83(i1,i2,i3) ry(i1,i2,i3)*szr8(i1,i2,i3)+sy(i1,i2,i3)*szs8(i1,i2,i3)+ty(i1,i2,i3)*szt8(i1,i2,i3)
#define szz83(i1,i2,i3) rz(i1,i2,i3)*szr8(i1,i2,i3)+sz(i1,i2,i3)*szs8(i1,i2,i3)+tz(i1,i2,i3)*szt8(i1,i2,i3)
#define txx82(i1,i2,i3)  rx(i1,i2,i3)*txr8(i1,i2,i3)+sx(i1,i2,i3)*txs8(i1,i2,i3)
#define txy82(i1,i2,i3)  ry(i1,i2,i3)*txr8(i1,i2,i3)+sy(i1,i2,i3)*txs8(i1,i2,i3)
#define txx83(i1,i2,i3) rx(i1,i2,i3)*txr8(i1,i2,i3)+sx(i1,i2,i3)*txs8(i1,i2,i3)+tx(i1,i2,i3)*txt8(i1,i2,i3)
#define txy83(i1,i2,i3) ry(i1,i2,i3)*txr8(i1,i2,i3)+sy(i1,i2,i3)*txs8(i1,i2,i3)+ty(i1,i2,i3)*txt8(i1,i2,i3)
#define txz83(i1,i2,i3) rz(i1,i2,i3)*txr8(i1,i2,i3)+sz(i1,i2,i3)*txs8(i1,i2,i3)+tz(i1,i2,i3)*txt8(i1,i2,i3)
#define tyx82(i1,i2,i3)  rx(i1,i2,i3)*tyr8(i1,i2,i3)+sx(i1,i2,i3)*tys8(i1,i2,i3)
#define tyy82(i1,i2,i3)  ry(i1,i2,i3)*tyr8(i1,i2,i3)+sy(i1,i2,i3)*tys8(i1,i2,i3)
#define tyx83(i1,i2,i3) rx(i1,i2,i3)*tyr8(i1,i2,i3)+sx(i1,i2,i3)*tys8(i1,i2,i3)+tx(i1,i2,i3)*tyt8(i1,i2,i3)
#define tyy83(i1,i2,i3) ry(i1,i2,i3)*tyr8(i1,i2,i3)+sy(i1,i2,i3)*tys8(i1,i2,i3)+ty(i1,i2,i3)*tyt8(i1,i2,i3)
#define tyz83(i1,i2,i3) rz(i1,i2,i3)*tyr8(i1,i2,i3)+sz(i1,i2,i3)*tys8(i1,i2,i3)+tz(i1,i2,i3)*tyt8(i1,i2,i3)
#define tzx82(i1,i2,i3)  rx(i1,i2,i3)*tzr8(i1,i2,i3)+sx(i1,i2,i3)*tzs8(i1,i2,i3)
#define tzy82(i1,i2,i3)  ry(i1,i2,i3)*tzr8(i1,i2,i3)+sy(i1,i2,i3)*tzs8(i1,i2,i3)
#define tzx83(i1,i2,i3) rx(i1,i2,i3)*tzr8(i1,i2,i3)+sx(i1,i2,i3)*tzs8(i1,i2,i3)+tx(i1,i2,i3)*tzt8(i1,i2,i3)
#define tzy83(i1,i2,i3) ry(i1,i2,i3)*tzr8(i1,i2,i3)+sy(i1,i2,i3)*tzs8(i1,i2,i3)+ty(i1,i2,i3)*tzt8(i1,i2,i3)
#define tzz83(i1,i2,i3) rz(i1,i2,i3)*tzr8(i1,i2,i3)+sz(i1,i2,i3)*tzs8(i1,i2,i3)+tz(i1,i2,i3)*tzt8(i1,i2,i3)
#End

#define u ## xx81(i1,i2,i3) (SQR(rx(i1,i2,i3)))*u ## rr8(i1,i2,i3)+(rxx82(i1,i2,i3))*u ## r8(i1,i2,i3)
#define u ## yy81(i1,i2,i3) 0
#define u ## xy81(i1,i2,i3) 0
#define u ## xz81(i1,i2,i3) 0
#define u ## yz81(i1,i2,i3) 0
#define u ## zz81(i1,i2,i3) 0
#define u ## laplacian81(i1,i2,i3) u ## xx81(i1,i2,i3)
#define u ## xx82(i1,i2,i3) (SQR(rx(i1,i2,i3)))*u ## rr8(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3)+(SQR(sx(i1,i2,i3)))*u ## ss8(i1,i2,i3)+(rxx82(i1,i2,i3))*u ## r8(i1,i2,i3)+(sxx82(i1,i2,i3))*u ## s8(i1,i2,i3)
#define u ## yy82(i1,i2,i3) (SQR(ry(i1,i2,i3)))*u ## rr8(i1,i2,i3)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs8(i1,i2,i3)+(SQR(sy(i1,i2,i3)))*u ## ss8(i1,i2,i3)+(ryy82(i1,i2,i3))*u ## r8(i1,i2,i3)+(syy82(i1,i2,i3))*u ## s8(i1,i2,i3)
#define u ## xy82(i1,i2,i3) rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr8(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss8(i1,i2,i3)+rxy82(i1,i2,i3)*u ## r8(i1,i2,i3)+sxy82(i1,i2,i3)*u ## s8(i1,i2,i3)
#define u ## xz82(i1,i2,i3) 0
#define u ## yz82(i1,i2,i3) 0
#define u ## zz82(i1,i2,i3) 0
#define u ## laplacian82(i1,i2,i3) (SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3)))*u ## rr8(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs8(i1,i2,i3)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3)))*u ## ss8(i1,i2,i3)+(rxx82(i1,i2,i3)+ryy82(i1,i2,i3))*u ## r8(i1,i2,i3)+(sxx82(i1,i2,i3)+syy82(i1,i2,i3))*u ## s8(i1,i2,i3)
#define u ## xx83(i1,i2,i3) SQR(rx(i1,i2,i3))*u ## rr8(i1,i2,i3)+SQR(sx(i1,i2,i3))*u ## ss8(i1,i2,i3)+SQR(tx(i1,i2,i3))*u ## tt8(i1,i2,i3)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs8(i1,i2,i3)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt8(i1,i2,i3)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st8(i1,i2,i3)+rxx83(i1,i2,i3)*u ## r8(i1,i2,i3)+sxx83(i1,i2,i3)*u ## s8(i1,i2,i3)+txx83(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## yy83(i1,i2,i3) SQR(ry(i1,i2,i3))*u ## rr8(i1,i2,i3)+SQR(sy(i1,i2,i3))*u ## ss8(i1,i2,i3)+SQR(ty(i1,i2,i3))*u ## tt8(i1,i2,i3)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs8(i1,i2,i3)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt8(i1,i2,i3)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st8(i1,i2,i3)+ryy83(i1,i2,i3)*u ## r8(i1,i2,i3)+syy83(i1,i2,i3)*u ## s8(i1,i2,i3)+tyy83(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## zz83(i1,i2,i3) SQR(rz(i1,i2,i3))*u ## rr8(i1,i2,i3)+SQR(sz(i1,i2,i3))*u ## ss8(i1,i2,i3)+SQR(tz(i1,i2,i3))*u ## tt8(i1,i2,i3)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs8(i1,i2,i3)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt8(i1,i2,i3)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st8(i1,i2,i3)+rzz83(i1,i2,i3)*u ## r8(i1,i2,i3)+szz83(i1,i2,i3)*u ## s8(i1,i2,i3)+tzz83(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## xy83(i1,i2,i3) rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr8(i1,i2,i3)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss8(i1,i2,i3)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt8(i1,i2,i3)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt8(i1,i2,i3)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st8(i1,i2,i3)+rxy83(i1,i2,i3)*u ## r8(i1,i2,i3)+sxy83(i1,i2,i3)*u ## s8(i1,i2,i3)+txy83(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## xz83(i1,i2,i3) rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr8(i1,i2,i3)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss8(i1,i2,i3)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt8(i1,i2,i3)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt8(i1,i2,i3)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st8(i1,i2,i3)+rxz83(i1,i2,i3)*u ## r8(i1,i2,i3)+sxz83(i1,i2,i3)*u ## s8(i1,i2,i3)+txz83(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## yz83(i1,i2,i3) ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr8(i1,i2,i3)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss8(i1,i2,i3)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt8(i1,i2,i3)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs8(i1,i2,i3)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt8(i1,i2,i3)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st8(i1,i2,i3)+ryz83(i1,i2,i3)*u ## r8(i1,i2,i3)+syz83(i1,i2,i3)*u ## s8(i1,i2,i3)+tyz83(i1,i2,i3)*u ## t8(i1,i2,i3)
#define u ## laplacian83(i1,i2,i3) (SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3))+SQR(rz(i1,i2,i3)))*u ## rr8(i1,i2,i3)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3))+SQR(sz(i1,i2,i3)))*u ## ss8(i1,i2,i3)+(SQR(tx(i1,i2,i3))+SQR(ty(i1,i2,i3))+SQR(tz(i1,i2,i3)))*u ## tt8(i1,i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs8(i1,i2,i3)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt8(i1,i2,i3)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st8(i1,i2,i3)+(rxx83(i1,i2,i3)+ryy83(i1,i2,i3)+rzz83(i1,i2,i3))*u ## r8(i1,i2,i3)+(sxx83(i1,i2,i3)+syy83(i1,i2,i3)+szz83(i1,i2,i3))*u ## s8(i1,i2,i3)+(txx83(i1,i2,i3)+tyy83(i1,i2,i3)+tzz83(i1,i2,i3))*u ## t8(i1,i2,i3)
//============================================================================================
// Define derivatives for a rectangular grid
//
//============================================================================================
#define #If #OPTION  = "RX"
#define h18(kd)   1./(840.*dx(kd))
#define h28(kd)   1./(5040.*SQR(dx(kd)))
#End

#define u ## x83r(i1,i2,i3) (672.*(u(i1+1,i2,i3)-u(i1-1,i2,i3))-168.*(u(i1+2,i2,i3)-u(i1-2,i2,i3))+32.*(u(i1+3,i2,i3)-u(i1-3,i2,i3))-3.*(u(i1+4,i2,i3)-u(i1-4,i2,i3)))*h18(0)
#define u ## y83r(i1,i2,i3) (672.*(u(i1,i2+1,i3)-u(i1,i2-1,i3))-168.*(u(i1,i2+2,i3)-u(i1,i2-2,i3))+32.*(u(i1,i2+3,i3)-u(i1,i2-3,i3))-3.*(u(i1,i2+4,i3)-u(i1,i2-4,i3)))*h18(1)
#define u ## z83r(i1,i2,i3) (672.*(u(i1,i2,i3+1)-u(i1,i2,i3-1))-168.*(u(i1,i2,i3+2)-u(i1,i2,i3-2))+32.*(u(i1,i2,i3+3)-u(i1,i2,i3-3))-3.*(u(i1,i2,i3+4)-u(i1,i2,i3-4)))*h18(2)

#define u ## xx83r(i1,i2,i3) (-14350.*u(i1,i2,i3)+8064.*(u(i1+1,i2,i3)+u(i1-1,i2,i3))-1008.*(u(i1+2,i2,i3)+u(i1-2,i2,i3))+128.*(u(i1+3,i2,i3)+u(i1-3,i2,i3))-9.*(u(i1+4,i2,i3)+u(i1-4,i2,i3)) )*h28(0)
#define u ## yy83r(i1,i2,i3) (-14350.*u(i1,i2,i3)+8064.*(u(i1,i2+1,i3)+u(i1,i2-1,i3))-1008.*(u(i1,i2+2,i3)+u(i1,i2-2,i3))+128.*(u(i1,i2+3,i3)+u(i1,i2-3,i3))-9.*(u(i1,i2+4,i3)+u(i1,i2-4,i3)) )*h28(1)
#define u ## zz83r(i1,i2,i3) (-14350.*u(i1,i2,i3)+8064.*(u(i1,i2,i3+1)+u(i1,i2,i3-1))-1008.*(u(i1,i2,i3+2)+u(i1,i2,i3-2))+128.*(u(i1,i2,i3+3)+u(i1,i2,i3-3))-9.*(u(i1,i2,i3+4)+u(i1,i2,i3-4)) )*h28(2)
#define u ## xy83r(i1,i2,i3) (672.*(u ## x83r(i1,i2+1,i3)-u ## x83r(i1,i2-1,i3))-168.*(u ## x83r(i1,i2+2,i3)-u ## x83r(i1,i2-2,i3))+32.*(u ## x83r(i1,i2+3,i3)-u ## x83r(i1,i2-3,i3))-3.*(u ## x83r(i1,i2+4,i3)-u ## x83r(i1,i2-4,i3)))*h18(1)
#define u ## xz83r(i1,i2,i3) (672.*(u ## x83r(i1,i2,i3+1)-u ## x83r(i1,i2,i3-1))-168.*(u ## x83r(i1,i2,i3+2)-u ## x83r(i1,i2,i3-2))+32.*(u ## x83r(i1,i2,i3+3)-u ## x83r(i1,i2,i3-3))-3.*(u ## x83r(i1,i2,i3+4)-u ## x83r(i1,i2,i3-4)))*h18(2)
#define u ## yz83r(i1,i2,i3) (672.*(u ## y83r(i1,i2,i3+1)-u ## y83r(i1,i2,i3-1))-168.*(u ## y83r(i1,i2,i3+2)-u ## y83r(i1,i2,i3-2))+32.*(u ## y83r(i1,i2,i3+3)-u ## y83r(i1,i2,i3-3))-3.*(u ## y83r(i1,i2,i3+4)-u ## y83r(i1,i2,i3-4)))*h18(2)

#define u ## x81r(i1,i2,i3)  u ## x83r(i1,i2,i3)
#define u ## y81r(i1,i2,i3)  u ## y83r(i1,i2,i3)
#define u ## z81r(i1,i2,i3)  u ## z83r(i1,i2,i3)
#define u ## xx81r(i1,i2,i3)  u ## xx83r(i1,i2,i3)
#define u ## yy81r(i1,i2,i3)  u ## yy83r(i1,i2,i3)
#define u ## zz81r(i1,i2,i3)  u ## zz83r(i1,i2,i3)
#define u ## xy81r(i1,i2,i3)  u ## xy83r(i1,i2,i3)
#define u ## xz81r(i1,i2,i3)  u ## xz83r(i1,i2,i3)
#define u ## yz81r(i1,i2,i3)  u ## yz83r(i1,i2,i3)
#define u ## laplacian81r(i1,i2,i3) u ## xx83r(i1,i2,i3)
#define u ## x82r(i1,i2,i3)  u ## x83r(i1,i2,i3)
#define u ## y82r(i1,i2,i3)  u ## y83r(i1,i2,i3)
#define u ## z82r(i1,i2,i3)  u ## z83r(i1,i2,i3)
#define u ## xx82r(i1,i2,i3)  u ## xx83r(i1,i2,i3)
#define u ## yy82r(i1,i2,i3)  u ## yy83r(i1,i2,i3)
#define u ## zz82r(i1,i2,i3)  u ## zz83r(i1,i2,i3)
#define u ## xy82r(i1,i2,i3)  u ## xy83r(i1,i2,i3)
#define u ## xz82r(i1,i2,i3)  u ## xz83r(i1,i2,i3)
#define u ## yz82r(i1,i2,i3)  u ## yz83r(i1,i2,i3)
#define u ## laplacian82r(i1,i2,i3) u ## xx83r(i1,i2,i3)+u ## yy83r(i1,i2,i3)
#define u ## laplacian83r(i1,i2,i3) u ## xx83r(i1,i2,i3)+u ## yy83r(i1,i2,i3)+u ## zz83r(i1,i2,i3)
#endMacro
// To include derivatives of rx use OPTION=RX
#beginMacro defineDifferenceOrder8Components1(u,OPTION)

#If #OPTION == "RX"
d18(kd) = 1./(840.*dr(kd))
d28(kd) = 1./(5040.*SQR(dr(kd)))
#End

u ## r8(i1,i2,i3,kd)=(672.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-168.*(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+32.*(u(i1+3,i2,i3,kd)-u(i1-3,i2,i3,kd))-3.*(u(i1+4,i2,i3,kd)-u(i1-4,i2,i3,kd)))*d18(0)
u ## s8(i1,i2,i3,kd)=(672.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-168.*(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+32.*(u(i1,i2+3,i3,kd)-u(i1,i2-3,i3,kd))-3.*(u(i1,i2+4,i3,kd)-u(i1,i2-4,i3,kd)))*d18(1)
u ## t8(i1,i2,i3,kd)=(672.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-168.*(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+32.*(u(i1,i2,i3+3,kd)-u(i1,i2,i3-3,kd))-3.*(u(i1,i2,i3+4,kd)-u(i1,i2,i3-4,kd)))*d18(2)

u ## rr8(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))-1008.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd))+128.*(u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd))-9.*(u(i1+4,i2,i3,kd)+u(i1-4,i2,i3,kd)) )*d28(0)
u ## ss8(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))-1008.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd))+128.*(u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd))-9.*(u(i1,i2+4,i3,kd)+u(i1,i2-4,i3,kd)) )*d28(1)
u ## tt8(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))-1008.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd))+128.*(u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd))-9.*(u(i1,i2,i3+4,kd)+u(i1,i2,i3-4,kd)) )*d28(2)
u ## rs8(i1,i2,i3,kd)=(672.*(u ## r8(i1,i2+1,i3,kd)-u ## r8(i1,i2-1,i3,kd))-168.*(u ## r8(i1,i2+2,i3,kd)-u ## r8(i1,i2-2,i3,kd))+32.*(u ## r8(i1,i2+3,i3,kd)-u ## r8(i1,i2-3,i3,kd))-3.*(u ## r8(i1,i2+4,i3,kd)-u ## r8(i1,i2-4,i3,kd)))*d18(1)
u ## rt8(i1,i2,i3,kd)=(672.*(u ## r8(i1,i2,i3+1,kd)-u ## r8(i1,i2,i3-1,kd))-168.*(u ## r8(i1,i2,i3+2,kd)-u ## r8(i1,i2,i3-2,kd))+32.*(u ## r8(i1,i2,i3+3,kd)-u ## r8(i1,i2,i3-3,kd))-3.*(u ## r8(i1,i2,i3+4,kd)-u ## r8(i1,i2,i3-4,kd)))*d18(2)
u ## st8(i1,i2,i3,kd)=(672.*(u ## s8(i1,i2,i3+1,kd)-u ## s8(i1,i2,i3-1,kd))-168.*(u ## s8(i1,i2,i3+2,kd)-u ## s8(i1,i2,i3-2,kd))+32.*(u ## s8(i1,i2,i3+3,kd)-u ## s8(i1,i2,i3-3,kd))-3.*(u ## s8(i1,i2,i3+4,kd)-u ## s8(i1,i2,i3-4,kd)))*d18(2)

#If #OPTION == "RX"
rxr8(i1,i2,i3)=(672.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-168.*(rx(i1+2,i2,i3)-rx(i1-2,i2,i3))+32.*(rx(i1+3,i2,i3)-rx(i1-3,i2,i3))-3.*(rx(i1+4,i2,i3)-rx(i1-4,i2,i3)))*d18(0)
rxs8(i1,i2,i3)=(672.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-168.*(rx(i1,i2+2,i3)-rx(i1,i2-2,i3))+32.*(rx(i1,i2+3,i3)-rx(i1,i2-3,i3))-3.*(rx(i1,i2+4,i3)-rx(i1,i2-4,i3)))*d18(1)
rxt8(i1,i2,i3)=(672.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-168.*(rx(i1,i2,i3+2)-rx(i1,i2,i3-2))+32.*(rx(i1,i2,i3+3)-rx(i1,i2,i3-3))-3.*(rx(i1,i2,i3+4)-rx(i1,i2,i3-4)))*d18(2)
ryr8(i1,i2,i3)=(672.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-168.*(ry(i1+2,i2,i3)-ry(i1-2,i2,i3))+32.*(ry(i1+3,i2,i3)-ry(i1-3,i2,i3))-3.*(ry(i1+4,i2,i3)-ry(i1-4,i2,i3)))*d18(0)
rys8(i1,i2,i3)=(672.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-168.*(ry(i1,i2+2,i3)-ry(i1,i2-2,i3))+32.*(ry(i1,i2+3,i3)-ry(i1,i2-3,i3))-3.*(ry(i1,i2+4,i3)-ry(i1,i2-4,i3)))*d18(1)
ryt8(i1,i2,i3)=(672.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-168.*(ry(i1,i2,i3+2)-ry(i1,i2,i3-2))+32.*(ry(i1,i2,i3+3)-ry(i1,i2,i3-3))-3.*(ry(i1,i2,i3+4)-ry(i1,i2,i3-4)))*d18(2)
rzr8(i1,i2,i3)=(672.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-168.*(rz(i1+2,i2,i3)-rz(i1-2,i2,i3))+32.*(rz(i1+3,i2,i3)-rz(i1-3,i2,i3))-3.*(rz(i1+4,i2,i3)-rz(i1-4,i2,i3)))*d18(0)
rzs8(i1,i2,i3)=(672.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-168.*(rz(i1,i2+2,i3)-rz(i1,i2-2,i3))+32.*(rz(i1,i2+3,i3)-rz(i1,i2-3,i3))-3.*(rz(i1,i2+4,i3)-rz(i1,i2-4,i3)))*d18(1)
rzt8(i1,i2,i3)=(672.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-168.*(rz(i1,i2,i3+2)-rz(i1,i2,i3-2))+32.*(rz(i1,i2,i3+3)-rz(i1,i2,i3-3))-3.*(rz(i1,i2,i3+4)-rz(i1,i2,i3-4)))*d18(2)
sxr8(i1,i2,i3)=(672.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-168.*(sx(i1+2,i2,i3)-sx(i1-2,i2,i3))+32.*(sx(i1+3,i2,i3)-sx(i1-3,i2,i3))-3.*(sx(i1+4,i2,i3)-sx(i1-4,i2,i3)))*d18(0)
sxs8(i1,i2,i3)=(672.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-168.*(sx(i1,i2+2,i3)-sx(i1,i2-2,i3))+32.*(sx(i1,i2+3,i3)-sx(i1,i2-3,i3))-3.*(sx(i1,i2+4,i3)-sx(i1,i2-4,i3)))*d18(1)
sxt8(i1,i2,i3)=(672.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-168.*(sx(i1,i2,i3+2)-sx(i1,i2,i3-2))+32.*(sx(i1,i2,i3+3)-sx(i1,i2,i3-3))-3.*(sx(i1,i2,i3+4)-sx(i1,i2,i3-4)))*d18(2)
syr8(i1,i2,i3)=(672.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-168.*(sy(i1+2,i2,i3)-sy(i1-2,i2,i3))+32.*(sy(i1+3,i2,i3)-sy(i1-3,i2,i3))-3.*(sy(i1+4,i2,i3)-sy(i1-4,i2,i3)))*d18(0)
sys8(i1,i2,i3)=(672.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-168.*(sy(i1,i2+2,i3)-sy(i1,i2-2,i3))+32.*(sy(i1,i2+3,i3)-sy(i1,i2-3,i3))-3.*(sy(i1,i2+4,i3)-sy(i1,i2-4,i3)))*d18(1)
syt8(i1,i2,i3)=(672.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-168.*(sy(i1,i2,i3+2)-sy(i1,i2,i3-2))+32.*(sy(i1,i2,i3+3)-sy(i1,i2,i3-3))-3.*(sy(i1,i2,i3+4)-sy(i1,i2,i3-4)))*d18(2)
szr8(i1,i2,i3)=(672.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-168.*(sz(i1+2,i2,i3)-sz(i1-2,i2,i3))+32.*(sz(i1+3,i2,i3)-sz(i1-3,i2,i3))-3.*(sz(i1+4,i2,i3)-sz(i1-4,i2,i3)))*d18(0)
szs8(i1,i2,i3)=(672.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-168.*(sz(i1,i2+2,i3)-sz(i1,i2-2,i3))+32.*(sz(i1,i2+3,i3)-sz(i1,i2-3,i3))-3.*(sz(i1,i2+4,i3)-sz(i1,i2-4,i3)))*d18(1)
szt8(i1,i2,i3)=(672.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-168.*(sz(i1,i2,i3+2)-sz(i1,i2,i3-2))+32.*(sz(i1,i2,i3+3)-sz(i1,i2,i3-3))-3.*(sz(i1,i2,i3+4)-sz(i1,i2,i3-4)))*d18(2)
txr8(i1,i2,i3)=(672.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-168.*(tx(i1+2,i2,i3)-tx(i1-2,i2,i3))+32.*(tx(i1+3,i2,i3)-tx(i1-3,i2,i3))-3.*(tx(i1+4,i2,i3)-tx(i1-4,i2,i3)))*d18(0)
txs8(i1,i2,i3)=(672.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-168.*(tx(i1,i2+2,i3)-tx(i1,i2-2,i3))+32.*(tx(i1,i2+3,i3)-tx(i1,i2-3,i3))-3.*(tx(i1,i2+4,i3)-tx(i1,i2-4,i3)))*d18(1)
txt8(i1,i2,i3)=(672.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-168.*(tx(i1,i2,i3+2)-tx(i1,i2,i3-2))+32.*(tx(i1,i2,i3+3)-tx(i1,i2,i3-3))-3.*(tx(i1,i2,i3+4)-tx(i1,i2,i3-4)))*d18(2)
tyr8(i1,i2,i3)=(672.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-168.*(ty(i1+2,i2,i3)-ty(i1-2,i2,i3))+32.*(ty(i1+3,i2,i3)-ty(i1-3,i2,i3))-3.*(ty(i1+4,i2,i3)-ty(i1-4,i2,i3)))*d18(0)
tys8(i1,i2,i3)=(672.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-168.*(ty(i1,i2+2,i3)-ty(i1,i2-2,i3))+32.*(ty(i1,i2+3,i3)-ty(i1,i2-3,i3))-3.*(ty(i1,i2+4,i3)-ty(i1,i2-4,i3)))*d18(1)
tyt8(i1,i2,i3)=(672.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-168.*(ty(i1,i2,i3+2)-ty(i1,i2,i3-2))+32.*(ty(i1,i2,i3+3)-ty(i1,i2,i3-3))-3.*(ty(i1,i2,i3+4)-ty(i1,i2,i3-4)))*d18(2)
tzr8(i1,i2,i3)=(672.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-168.*(tz(i1+2,i2,i3)-tz(i1-2,i2,i3))+32.*(tz(i1+3,i2,i3)-tz(i1-3,i2,i3))-3.*(tz(i1+4,i2,i3)-tz(i1-4,i2,i3)))*d18(0)
tzs8(i1,i2,i3)=(672.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-168.*(tz(i1,i2+2,i3)-tz(i1,i2-2,i3))+32.*(tz(i1,i2+3,i3)-tz(i1,i2-3,i3))-3.*(tz(i1,i2+4,i3)-tz(i1,i2-4,i3)))*d18(1)
tzt8(i1,i2,i3)=(672.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-168.*(tz(i1,i2,i3+2)-tz(i1,i2,i3-2))+32.*(tz(i1,i2,i3+3)-tz(i1,i2,i3-3))-3.*(tz(i1,i2,i3+4)-tz(i1,i2,i3-4)))*d18(2)
#End

u ## x81(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r8(i1,i2,i3,kd)
u ## y81(i1,i2,i3,kd)=0
u ## z81(i1,i2,i3,kd)=0

u ## x82(i1,i2,i3,kd)= rx(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s8(i1,i2,i3,kd)
u ## y82(i1,i2,i3,kd)= ry(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s8(i1,i2,i3,kd)
u ## z82(i1,i2,i3,kd)=0
u ## x83(i1,i2,i3,kd)=rx(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sx(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+tx(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## y83(i1,i2,i3,kd)=ry(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sy(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+ty(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## z83(i1,i2,i3,kd)=rz(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sz(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+tz(i1,i2,i3)*u ## t8(i1,i2,i3,kd)

#If #OPTION == "RX"
rxx81(i1,i2,i3)= rx(i1,i2,i3)*rxr8(i1,i2,i3)
rxx82(i1,i2,i3)= rx(i1,i2,i3)*rxr8(i1,i2,i3)+sx(i1,i2,i3)*rxs8(i1,i2,i3)
rxy82(i1,i2,i3)= ry(i1,i2,i3)*rxr8(i1,i2,i3)+sy(i1,i2,i3)*rxs8(i1,i2,i3)
rxx83(i1,i2,i3)=rx(i1,i2,i3)*rxr8(i1,i2,i3)+sx(i1,i2,i3)*rxs8(i1,i2,i3)+tx(i1,i2,i3)*rxt8(i1,i2,i3)
rxy83(i1,i2,i3)=ry(i1,i2,i3)*rxr8(i1,i2,i3)+sy(i1,i2,i3)*rxs8(i1,i2,i3)+ty(i1,i2,i3)*rxt8(i1,i2,i3)
rxz83(i1,i2,i3)=rz(i1,i2,i3)*rxr8(i1,i2,i3)+sz(i1,i2,i3)*rxs8(i1,i2,i3)+tz(i1,i2,i3)*rxt8(i1,i2,i3)
ryx82(i1,i2,i3)= rx(i1,i2,i3)*ryr8(i1,i2,i3)+sx(i1,i2,i3)*rys8(i1,i2,i3)
ryy82(i1,i2,i3)= ry(i1,i2,i3)*ryr8(i1,i2,i3)+sy(i1,i2,i3)*rys8(i1,i2,i3)
ryx83(i1,i2,i3)=rx(i1,i2,i3)*ryr8(i1,i2,i3)+sx(i1,i2,i3)*rys8(i1,i2,i3)+tx(i1,i2,i3)*ryt8(i1,i2,i3)
ryy83(i1,i2,i3)=ry(i1,i2,i3)*ryr8(i1,i2,i3)+sy(i1,i2,i3)*rys8(i1,i2,i3)+ty(i1,i2,i3)*ryt8(i1,i2,i3)
ryz83(i1,i2,i3)=rz(i1,i2,i3)*ryr8(i1,i2,i3)+sz(i1,i2,i3)*rys8(i1,i2,i3)+tz(i1,i2,i3)*ryt8(i1,i2,i3)
rzx82(i1,i2,i3)= rx(i1,i2,i3)*rzr8(i1,i2,i3)+sx(i1,i2,i3)*rzs8(i1,i2,i3)
rzy82(i1,i2,i3)= ry(i1,i2,i3)*rzr8(i1,i2,i3)+sy(i1,i2,i3)*rzs8(i1,i2,i3)
rzx83(i1,i2,i3)=rx(i1,i2,i3)*rzr8(i1,i2,i3)+sx(i1,i2,i3)*rzs8(i1,i2,i3)+tx(i1,i2,i3)*rzt8(i1,i2,i3)
rzy83(i1,i2,i3)=ry(i1,i2,i3)*rzr8(i1,i2,i3)+sy(i1,i2,i3)*rzs8(i1,i2,i3)+ty(i1,i2,i3)*rzt8(i1,i2,i3)
rzz83(i1,i2,i3)=rz(i1,i2,i3)*rzr8(i1,i2,i3)+sz(i1,i2,i3)*rzs8(i1,i2,i3)+tz(i1,i2,i3)*rzt8(i1,i2,i3)
sxx82(i1,i2,i3)= rx(i1,i2,i3)*sxr8(i1,i2,i3)+sx(i1,i2,i3)*sxs8(i1,i2,i3)
sxy82(i1,i2,i3)= ry(i1,i2,i3)*sxr8(i1,i2,i3)+sy(i1,i2,i3)*sxs8(i1,i2,i3)
sxx83(i1,i2,i3)=rx(i1,i2,i3)*sxr8(i1,i2,i3)+sx(i1,i2,i3)*sxs8(i1,i2,i3)+tx(i1,i2,i3)*sxt8(i1,i2,i3)
sxy83(i1,i2,i3)=ry(i1,i2,i3)*sxr8(i1,i2,i3)+sy(i1,i2,i3)*sxs8(i1,i2,i3)+ty(i1,i2,i3)*sxt8(i1,i2,i3)
sxz83(i1,i2,i3)=rz(i1,i2,i3)*sxr8(i1,i2,i3)+sz(i1,i2,i3)*sxs8(i1,i2,i3)+tz(i1,i2,i3)*sxt8(i1,i2,i3)
syx82(i1,i2,i3)= rx(i1,i2,i3)*syr8(i1,i2,i3)+sx(i1,i2,i3)*sys8(i1,i2,i3)
syy82(i1,i2,i3)= ry(i1,i2,i3)*syr8(i1,i2,i3)+sy(i1,i2,i3)*sys8(i1,i2,i3)
syx83(i1,i2,i3)=rx(i1,i2,i3)*syr8(i1,i2,i3)+sx(i1,i2,i3)*sys8(i1,i2,i3)+tx(i1,i2,i3)*syt8(i1,i2,i3)
syy83(i1,i2,i3)=ry(i1,i2,i3)*syr8(i1,i2,i3)+sy(i1,i2,i3)*sys8(i1,i2,i3)+ty(i1,i2,i3)*syt8(i1,i2,i3)
syz83(i1,i2,i3)=rz(i1,i2,i3)*syr8(i1,i2,i3)+sz(i1,i2,i3)*sys8(i1,i2,i3)+tz(i1,i2,i3)*syt8(i1,i2,i3)
szx82(i1,i2,i3)= rx(i1,i2,i3)*szr8(i1,i2,i3)+sx(i1,i2,i3)*szs8(i1,i2,i3)
szy82(i1,i2,i3)= ry(i1,i2,i3)*szr8(i1,i2,i3)+sy(i1,i2,i3)*szs8(i1,i2,i3)
szx83(i1,i2,i3)=rx(i1,i2,i3)*szr8(i1,i2,i3)+sx(i1,i2,i3)*szs8(i1,i2,i3)+tx(i1,i2,i3)*szt8(i1,i2,i3)
szy83(i1,i2,i3)=ry(i1,i2,i3)*szr8(i1,i2,i3)+sy(i1,i2,i3)*szs8(i1,i2,i3)+ty(i1,i2,i3)*szt8(i1,i2,i3)
szz83(i1,i2,i3)=rz(i1,i2,i3)*szr8(i1,i2,i3)+sz(i1,i2,i3)*szs8(i1,i2,i3)+tz(i1,i2,i3)*szt8(i1,i2,i3)
txx82(i1,i2,i3)= rx(i1,i2,i3)*txr8(i1,i2,i3)+sx(i1,i2,i3)*txs8(i1,i2,i3)
txy82(i1,i2,i3)= ry(i1,i2,i3)*txr8(i1,i2,i3)+sy(i1,i2,i3)*txs8(i1,i2,i3)
txx83(i1,i2,i3)=rx(i1,i2,i3)*txr8(i1,i2,i3)+sx(i1,i2,i3)*txs8(i1,i2,i3)+tx(i1,i2,i3)*txt8(i1,i2,i3)
txy83(i1,i2,i3)=ry(i1,i2,i3)*txr8(i1,i2,i3)+sy(i1,i2,i3)*txs8(i1,i2,i3)+ty(i1,i2,i3)*txt8(i1,i2,i3)
txz83(i1,i2,i3)=rz(i1,i2,i3)*txr8(i1,i2,i3)+sz(i1,i2,i3)*txs8(i1,i2,i3)+tz(i1,i2,i3)*txt8(i1,i2,i3)
tyx82(i1,i2,i3)= rx(i1,i2,i3)*tyr8(i1,i2,i3)+sx(i1,i2,i3)*tys8(i1,i2,i3)
tyy82(i1,i2,i3)= ry(i1,i2,i3)*tyr8(i1,i2,i3)+sy(i1,i2,i3)*tys8(i1,i2,i3)
tyx83(i1,i2,i3)=rx(i1,i2,i3)*tyr8(i1,i2,i3)+sx(i1,i2,i3)*tys8(i1,i2,i3)+tx(i1,i2,i3)*tyt8(i1,i2,i3)
tyy83(i1,i2,i3)=ry(i1,i2,i3)*tyr8(i1,i2,i3)+sy(i1,i2,i3)*tys8(i1,i2,i3)+ty(i1,i2,i3)*tyt8(i1,i2,i3)
tyz83(i1,i2,i3)=rz(i1,i2,i3)*tyr8(i1,i2,i3)+sz(i1,i2,i3)*tys8(i1,i2,i3)+tz(i1,i2,i3)*tyt8(i1,i2,i3)
tzx82(i1,i2,i3)= rx(i1,i2,i3)*tzr8(i1,i2,i3)+sx(i1,i2,i3)*tzs8(i1,i2,i3)
tzy82(i1,i2,i3)= ry(i1,i2,i3)*tzr8(i1,i2,i3)+sy(i1,i2,i3)*tzs8(i1,i2,i3)
tzx83(i1,i2,i3)=rx(i1,i2,i3)*tzr8(i1,i2,i3)+sx(i1,i2,i3)*tzs8(i1,i2,i3)+tx(i1,i2,i3)*tzt8(i1,i2,i3)
tzy83(i1,i2,i3)=ry(i1,i2,i3)*tzr8(i1,i2,i3)+sy(i1,i2,i3)*tzs8(i1,i2,i3)+ty(i1,i2,i3)*tzt8(i1,i2,i3)
tzz83(i1,i2,i3)=rz(i1,i2,i3)*tzr8(i1,i2,i3)+sz(i1,i2,i3)*tzs8(i1,i2,i3)+tz(i1,i2,i3)*tzt8(i1,i2,i3)
#End

u ## xx81(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3)))*u ## rr8(i1,i2,i3,kd)+(rxx82(i1,i2,i3))*u ## r8(i1,i2,i3,kd)
u ## yy81(i1,i2,i3,kd)=0
u ## xy81(i1,i2,i3,kd)=0
u ## xz81(i1,i2,i3,kd)=0
u ## yz81(i1,i2,i3,kd)=0
u ## zz81(i1,i2,i3,kd)=0
u ## laplacian81(i1,i2,i3,kd)=u ## xx81(i1,i2,i3,kd)
u ## xx82(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3)))*u ## rr8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+(SQR(sx(i1,i2,i3)))*u ## ss8(i1,i2,i3,kd)+(rxx82(i1,i2,i3))*u ## r8(i1,i2,i3,kd)+(sxx82(i1,i2,i3))*u ## s8(i1,i2,i3,kd)
u ## yy82(i1,i2,i3,kd)=(SQR(ry(i1,i2,i3)))*u ## rr8(i1,i2,i3,kd)+2.*(ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+(SQR(sy(i1,i2,i3)))*u ## ss8(i1,i2,i3,kd)+(ryy82(i1,i2,i3))*u ## r8(i1,i2,i3,kd)+(syy82(i1,i2,i3))*u ## s8(i1,i2,i3,kd)
u ## xy82(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr8(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss8(i1,i2,i3,kd)+rxy82(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sxy82(i1,i2,i3)*u ## s8(i1,i2,i3,kd)
u ## xz82(i1,i2,i3,kd)=0
u ## yz82(i1,i2,i3,kd)=0
u ## zz82(i1,i2,i3,kd)=0
u ## laplacian82(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3)))*u ## rr8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3)))*u ## ss8(i1,i2,i3,kd)+(rxx82(i1,i2,i3)+ryy82(i1,i2,i3))*u ## r8(i1,i2,i3,kd)+(sxx82(i1,i2,i3)+syy82(i1,i2,i3))*u ## s8(i1,i2,i3,kd)
u ## xx83(i1,i2,i3,kd)=SQR(rx(i1,i2,i3))*u ## rr8(i1,i2,i3,kd)+SQR(sx(i1,i2,i3))*u ## ss8(i1,i2,i3,kd)+SQR(tx(i1,i2,i3))*u ## tt8(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u ## rs8(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*u ## rt8(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*u ## st8(i1,i2,i3,kd)+rxx83(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sxx83(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+txx83(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## yy83(i1,i2,i3,kd)=SQR(ry(i1,i2,i3))*u ## rr8(i1,i2,i3,kd)+SQR(sy(i1,i2,i3))*u ## ss8(i1,i2,i3,kd)+SQR(ty(i1,i2,i3))*u ## tt8(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u ## rs8(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*u ## rt8(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*u ## st8(i1,i2,i3,kd)+ryy83(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+syy83(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+tyy83(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## zz83(i1,i2,i3,kd)=SQR(rz(i1,i2,i3))*u ## rr8(i1,i2,i3,kd)+SQR(sz(i1,i2,i3))*u ## ss8(i1,i2,i3,kd)+SQR(tz(i1,i2,i3))*u ## tt8(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u ## rs8(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*u ## rt8(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*u ## st8(i1,i2,i3,kd)+rzz83(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+szz83(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+tzz83(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## xy83(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u ## rr8(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u ## ss8(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,i2,i3)*u ## tt8(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u ## rt8(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u ## st8(i1,i2,i3,kd)+rxy83(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sxy83(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+txy83(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## xz83(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*u ## rr8(i1,i2,i3,kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*u ## ss8(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,i2,i3)*u ## tt8(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u ## rt8(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u ## st8(i1,i2,i3,kd)+rxz83(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+sxz83(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+txz83(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## yz83(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*u ## rr8(i1,i2,i3,kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*u ## ss8(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,i2,i3)*u ## tt8(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u ## rt8(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u ## st8(i1,i2,i3,kd)+ryz83(i1,i2,i3)*u ## r8(i1,i2,i3,kd)+syz83(i1,i2,i3)*u ## s8(i1,i2,i3,kd)+tyz83(i1,i2,i3)*u ## t8(i1,i2,i3,kd)
u ## laplacian83(i1,i2,i3,kd)=(SQR(rx(i1,i2,i3))+SQR(ry(i1,i2,i3))+SQR(rz(i1,i2,i3)))*u ## rr8(i1,i2,i3,kd)+(SQR(sx(i1,i2,i3))+SQR(sy(i1,i2,i3))+SQR(sz(i1,i2,i3)))*u ## ss8(i1,i2,i3,kd)+(SQR(tx(i1,i2,i3))+SQR(ty(i1,i2,i3))+SQR(tz(i1,i2,i3)))*u ## tt8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))*u ## rs8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u ## rt8(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*u ## st8(i1,i2,i3,kd)+(rxx83(i1,i2,i3)+ryy83(i1,i2,i3)+rzz83(i1,i2,i3))*u ## r8(i1,i2,i3,kd)+(sxx83(i1,i2,i3)+syy83(i1,i2,i3)+szz83(i1,i2,i3))*u ## s8(i1,i2,i3,kd)+(txx83(i1,i2,i3)+tyy83(i1,i2,i3)+tzz83(i1,i2,i3))*u ## t8(i1,i2,i3,kd)
//============================================================================================
// Define derivatives for a rectangular grid
//
//============================================================================================
#If #OPTION == "RX"
h18(kd) = 1./(840.*dx(kd))
h28(kd) = 1./(5040.*SQR(dx(kd)))
#End

u ## x83r(i1,i2,i3,kd)=(672.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-168.*(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+32.*(u(i1+3,i2,i3,kd)-u(i1-3,i2,i3,kd))-3.*(u(i1+4,i2,i3,kd)-u(i1-4,i2,i3,kd)))*h18(0)
u ## y83r(i1,i2,i3,kd)=(672.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-168.*(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+32.*(u(i1,i2+3,i3,kd)-u(i1,i2-3,i3,kd))-3.*(u(i1,i2+4,i3,kd)-u(i1,i2-4,i3,kd)))*h18(1)
u ## z83r(i1,i2,i3,kd)=(672.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-168.*(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+32.*(u(i1,i2,i3+3,kd)-u(i1,i2,i3-3,kd))-3.*(u(i1,i2,i3+4,kd)-u(i1,i2,i3-4,kd)))*h18(2)

u ## xx83r(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))-1008.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd))+128.*(u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd))-9.*(u(i1+4,i2,i3,kd)+u(i1-4,i2,i3,kd)) )*h28(0)
u ## yy83r(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))-1008.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd))+128.*(u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd))-9.*(u(i1,i2+4,i3,kd)+u(i1,i2-4,i3,kd)) )*h28(1)
u ## zz83r(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))-1008.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd))+128.*(u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd))-9.*(u(i1,i2,i3+4,kd)+u(i1,i2,i3-4,kd)) )*h28(2)
u ## xy83r(i1,i2,i3,kd)=(672.*(u ## x83r(i1,i2+1,i3,kd)-u ## x83r(i1,i2-1,i3,kd))-168.*(u ## x83r(i1,i2+2,i3,kd)-u ## x83r(i1,i2-2,i3,kd))+32.*(u ## x83r(i1,i2+3,i3,kd)-u ## x83r(i1,i2-3,i3,kd))-3.*(u ## x83r(i1,i2+4,i3,kd)-u ## x83r(i1,i2-4,i3,kd)))*h18(1)
u ## xz83r(i1,i2,i3,kd)=(672.*(u ## x83r(i1,i2,i3+1,kd)-u ## x83r(i1,i2,i3-1,kd))-168.*(u ## x83r(i1,i2,i3+2,kd)-u ## x83r(i1,i2,i3-2,kd))+32.*(u ## x83r(i1,i2,i3+3,kd)-u ## x83r(i1,i2,i3-3,kd))-3.*(u ## x83r(i1,i2,i3+4,kd)-u ## x83r(i1,i2,i3-4,kd)))*h18(2)
u ## yz83r(i1,i2,i3,kd)=(672.*(u ## y83r(i1,i2,i3+1,kd)-u ## y83r(i1,i2,i3-1,kd))-168.*(u ## y83r(i1,i2,i3+2,kd)-u ## y83r(i1,i2,i3-2,kd))+32.*(u ## y83r(i1,i2,i3+3,kd)-u ## y83r(i1,i2,i3-3,kd))-3.*(u ## y83r(i1,i2,i3+4,kd)-u ## y83r(i1,i2,i3-4,kd)))*h18(2)

u ## x81r(i1,i2,i3,kd)= u ## x83r(i1,i2,i3,kd)
u ## y81r(i1,i2,i3,kd)= u ## y83r(i1,i2,i3,kd)
u ## z81r(i1,i2,i3,kd)= u ## z83r(i1,i2,i3,kd)
u ## xx81r(i1,i2,i3,kd)= u ## xx83r(i1,i2,i3,kd)
u ## yy81r(i1,i2,i3,kd)= u ## yy83r(i1,i2,i3,kd)
u ## zz81r(i1,i2,i3,kd)= u ## zz83r(i1,i2,i3,kd)
u ## xy81r(i1,i2,i3,kd)= u ## xy83r(i1,i2,i3,kd)
u ## xz81r(i1,i2,i3,kd)= u ## xz83r(i1,i2,i3,kd)
u ## yz81r(i1,i2,i3,kd)= u ## yz83r(i1,i2,i3,kd)
u ## laplacian81r(i1,i2,i3,kd)=u ## xx83r(i1,i2,i3,kd)
u ## x82r(i1,i2,i3,kd)= u ## x83r(i1,i2,i3,kd)
u ## y82r(i1,i2,i3,kd)= u ## y83r(i1,i2,i3,kd)
u ## z82r(i1,i2,i3,kd)= u ## z83r(i1,i2,i3,kd)
u ## xx82r(i1,i2,i3,kd)= u ## xx83r(i1,i2,i3,kd)
u ## yy82r(i1,i2,i3,kd)= u ## yy83r(i1,i2,i3,kd)
u ## zz82r(i1,i2,i3,kd)= u ## zz83r(i1,i2,i3,kd)
u ## xy82r(i1,i2,i3,kd)= u ## xy83r(i1,i2,i3,kd)
u ## xz82r(i1,i2,i3,kd)= u ## xz83r(i1,i2,i3,kd)
u ## yz82r(i1,i2,i3,kd)= u ## yz83r(i1,i2,i3,kd)
u ## laplacian82r(i1,i2,i3,kd)=u ## xx83r(i1,i2,i3,kd)+u ## yy83r(i1,i2,i3,kd)
u ## laplacian83r(i1,i2,i3,kd)=u ## xx83r(i1,i2,i3,kd)+u ## yy83r(i1,i2,i3,kd)+u ## zz83r(i1,i2,i3,kd)
#endMacro
