
//
// Define 2nd-order difference approximations
//
//    needs d12(kd) = 1/(2*h(kd))
//          d22(kd) = 1/(h(kd)**2)
//



#define UR2(I1,I2,I3,m1,m2,m3) (Dr(m1)*delta(m2)*delta(m3))
#define US2(I1,I2,I3,m1,m2,m3) (Ds(m2)*delta(m1)*delta(m3))
#define UT2(I1,I2,I3,m1,m2,m3) (Dt(m3)*delta(m1)*delta(m2))

#undef  RX
#define RX(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,0)
#define SX(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,1)
#define TX(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,2)
#define RY(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,0+numberOfDimensions)
#define SY(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,1+numberOfDimensions)
#define TY(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,2+numberOfDimensions)
#define RZ(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,6)
#define SZ(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,7)
#define TZ(I1,I2,I3) c.inverseVertexDerivative(I1,I2,I3,8)


#define RXR2(I1,I2,I3) ( (RX(I1+1,I2,I3)-RX(I1-1,I2,I3))*d12(axis1) )
#define RYR2(I1,I2,I3) ( (RY(I1+1,I2,I3)-RY(I1-1,I2,I3))*d12(axis1) )
#define RZR2(I1,I2,I3) ( (RZ(I1+1,I2,I3)-RZ(I1-1,I2,I3))*d12(axis1) )
#define RXS2(I1,I2,I3) ( (RX(I1,I2+1,I3)-RX(I1,I2-1,I3))*d12(axis2) )
#define RYS2(I1,I2,I3) ( (RY(I1,I2+1,I3)-RY(I1,I2-1,I3))*d12(axis2) )
#define RZS2(I1,I2,I3) ( (RZ(I1,I2+1,I3)-RZ(I1,I2-1,I3))*d12(axis2) )
#define RXT2(I1,I2,I3) ( (RX(I1,I2,I3+1)-RX(I1,I2,I3-1))*d12(axis3) )
#define RYT2(I1,I2,I3) ( (RY(I1,I2,I3+1)-RY(I1,I2,I3-1))*d12(axis3) )
#define RZT2(I1,I2,I3) ( (RZ(I1,I2,I3+1)-RZ(I1,I2,I3-1))*d12(axis3) )
#define SXR2(I1,I2,I3) ( (SX(I1+1,I2,I3)-SX(I1-1,I2,I3))*d12(axis1) )
#define SYR2(I1,I2,I3) ( (SY(I1+1,I2,I3)-SY(I1-1,I2,I3))*d12(axis1) )
#define SZR2(I1,I2,I3) ( (SZ(I1+1,I2,I3)-SZ(I1-1,I2,I3))*d12(axis1) )
#define SXS2(I1,I2,I3) ( (SX(I1,I2+1,I3)-SX(I1,I2-1,I3))*d12(axis2) )
#define SYS2(I1,I2,I3) ( (SY(I1,I2+1,I3)-SY(I1,I2-1,I3))*d12(axis2) )
#define SZS2(I1,I2,I3) ( (SZ(I1,I2+1,I3)-SZ(I1,I2-1,I3))*d12(axis2) )
#define SXT2(I1,I2,I3) ( (SX(I1,I2,I3+1)-SX(I1,I2,I3-1))*d12(axis3) )
#define SYT2(I1,I2,I3) ( (SY(I1,I2,I3+1)-SY(I1,I2,I3-1))*d12(axis3) )
#define SZT2(I1,I2,I3) ( (SZ(I1,I2,I3+1)-SZ(I1,I2,I3-1))*d12(axis3) )
#define TXR2(I1,I2,I3) ( (TX(I1+1,I2,I3)-TX(I1-1,I2,I3))*d12(axis1) )
#define TYR2(I1,I2,I3) ( (TY(I1+1,I2,I3)-TY(I1-1,I2,I3))*d12(axis1) )
#define TZR2(I1,I2,I3) ( (TZ(I1+1,I2,I3)-TZ(I1-1,I2,I3))*d12(axis1) )
#define TXS2(I1,I2,I3) ( (TX(I1,I2+1,I3)-TX(I1,I2-1,I3))*d12(axis2) )
#define TYS2(I1,I2,I3) ( (TY(I1,I2+1,I3)-TY(I1,I2-1,I3))*d12(axis2) )
#define TZS2(I1,I2,I3) ( (TZ(I1,I2+1,I3)-TZ(I1,I2-1,I3))*d12(axis2) )
#define TXT2(I1,I2,I3) ( (TX(I1,I2,I3+1)-TX(I1,I2,I3-1))*d12(axis3) )
#define TYT2(I1,I2,I3) ( (TY(I1,I2,I3+1)-TY(I1,I2,I3-1))*d12(axis3) )
#define TZT2(I1,I2,I3) ( (TZ(I1,I2,I3+1)-TZ(I1,I2,I3-1))*d12(axis3) )

#define UX2(I1,I2,I3,m1,m2,m3)  ( RX(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)  \
                           +SX(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3) )
#define UY2(I1,I2,I3,m1,m2,m3)  ( RY(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)  \
                           +SY(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3) )
#define UX23(I1,I2,I3,m1,m2,m3) ( RX(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)  \
                           +SX(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)  \
                           +TX(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3) )
#define UY23(I1,I2,I3,m1,m2,m3) ( RY(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)  \
                           +SY(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)  \
                           +TY(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3) )
#define UZ23(I1,I2,I3,m1,m2,m3) ( RZ(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)  \
                           +SZ(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)  \
                           +TZ(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3) )

#define RXX2(I1,I2,I3) ( RX(I1,I2,I3)*RXR2(I1,I2,I3)  \
                        +SX(I1,I2,I3)*RXS2(I1,I2,I3) )
#define RXY2(I1,I2,I3) ( RX(I1,I2,I3)*RYR2(I1,I2,I3)  \
                        +SX(I1,I2,I3)*RYS2(I1,I2,I3) )
#define RYY2(I1,I2,I3) ( RY(I1,I2,I3)*RYR2(I1,I2,I3)  \
                        +SY(I1,I2,I3)*RYS2(I1,I2,I3) )
#define SXX2(I1,I2,I3) ( RX(I1,I2,I3)*SXR2(I1,I2,I3)  \
                        +SX(I1,I2,I3)*SXS2(I1,I2,I3) )
#define SXY2(I1,I2,I3) ( RX(I1,I2,I3)*SYR2(I1,I2,I3)  \
                        +SX(I1,I2,I3)*SYS2(I1,I2,I3) )
#define SYY2(I1,I2,I3) ( RY(I1,I2,I3)*SYR2(I1,I2,I3)  \
                        +SY(I1,I2,I3)*SYS2(I1,I2,I3) )

#define RXX23(I1,I2,I3) ( RX(I1,I2,I3)*RXR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*RXS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*RXT2(I1,I2,I3) )
#define RXY23(I1,I2,I3) ( RX(I1,I2,I3)*RYR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*RYS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*RYT2(I1,I2,I3) )
#define RXZ23(I1,I2,I3) ( RX(I1,I2,I3)*RZR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*RZS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*RZT2(I1,I2,I3) )
#define RYY23(I1,I2,I3) ( RY(I1,I2,I3)*RYR2(I1,I2,I3)  \
                         +SY(I1,I2,I3)*RYS2(I1,I2,I3)  \
                         +TY(I1,I2,I3)*RYT2(I1,I2,I3) )
#define RYZ23(I1,I2,I3) ( RY(I1,I2,I3)*RZR2(I1,I2,I3)  \
                         +SY(I1,I2,I3)*RZS2(I1,I2,I3)  \
                         +TY(I1,I2,I3)*RZT2(I1,I2,I3) )
#define RZZ23(I1,I2,I3) ( RZ(I1,I2,I3)*RZR2(I1,I2,I3)  \
                         +SZ(I1,I2,I3)*RZS2(I1,I2,I3)  \
                         +TZ(I1,I2,I3)*RZT2(I1,I2,I3) )
#define SXX23(I1,I2,I3) ( RX(I1,I2,I3)*SXR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*SXS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*SXT2(I1,I2,I3) )
#define SXY23(I1,I2,I3) ( RX(I1,I2,I3)*SYR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*SYS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*SYT2(I1,I2,I3) )
#define SXZ23(I1,I2,I3) ( RX(I1,I2,I3)*SZR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*SZS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*SZT2(I1,I2,I3) )
#define SYY23(I1,I2,I3) ( RY(I1,I2,I3)*SYR2(I1,I2,I3)  \
                         +SY(I1,I2,I3)*SYS2(I1,I2,I3)  \
                         +TY(I1,I2,I3)*SYT2(I1,I2,I3) )
#define SYZ23(I1,I2,I3) ( RY(I1,I2,I3)*SZR2(I1,I2,I3)  \
                         +SY(I1,I2,I3)*SZS2(I1,I2,I3)  \
                         +TY(I1,I2,I3)*SZT2(I1,I2,I3) )
#define SZZ23(I1,I2,I3) ( RZ(I1,I2,I3)*SZR2(I1,I2,I3)  \
                         +SZ(I1,I2,I3)*SZS2(I1,I2,I3)  \
                         +TZ(I1,I2,I3)*SZT2(I1,I2,I3) )
#define TXX23(I1,I2,I3) ( RX(I1,I2,I3)*TXR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*TXS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*TXT2(I1,I2,I3) )
#define TXY23(I1,I2,I3) ( RX(I1,I2,I3)*TYR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*TYS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*TYT2(I1,I2,I3) )
#define TXZ23(I1,I2,I3) ( RX(I1,I2,I3)*TZR2(I1,I2,I3)  \
                         +SX(I1,I2,I3)*TZS2(I1,I2,I3)  \
                         +TX(I1,I2,I3)*TZT2(I1,I2,I3) )
#define TYY23(I1,I2,I3) ( RY(I1,I2,I3)*TYR2(I1,I2,I3)  \
                         +SY(I1,I2,I3)*TYS2(I1,I2,I3)  \
                         +TY(I1,I2,I3)*TYT2(I1,I2,I3) )
#define TYZ23(I1,I2,I3) ( RY(I1,I2,I3)*TZR2(I1,I2,I3)  \
                         +SY(I1,I2,I3)*TZS2(I1,I2,I3)  \
                         +TY(I1,I2,I3)*TZT2(I1,I2,I3) )
#define TZZ23(I1,I2,I3) ( RZ(I1,I2,I3)*TZR2(I1,I2,I3)  \
                         +SZ(I1,I2,I3)*TZS2(I1,I2,I3)  \
                         +TZ(I1,I2,I3)*TZT2(I1,I2,I3) )

#define URR2(I1,I2,I3,m1,m2,m3) (Drr(m1)*delta(m2)*delta(m3))
#define USS2(I1,I2,I3,m1,m2,m3) (Dss(m2)*delta(m1)*delta(m3))
#define URS2(I1,I2,I3,m1,m2,m3) (Drs(m1,m2)*delta(m3))
#define UTT2(I1,I2,I3,m1,m2,m3) (Dtt(m3)*delta(m1)*delta(m2))
#define URT2(I1,I2,I3,m1,m2,m3) (Drt(m1,m3)*delta(m2))
#define UST2(I1,I2,I3,m1,m2,m3) (Dst(m2,m3)*delta(m1))


#define UXX2(I1,I2,I3,m1,m2,m3)                                           \
       ( (SQR(RX(I1,I2,I3))              )*URR2(I1,I2,I3,m1,m2,m3)        \
        +2.*(RX(I1,I2,I3)*SX(I1,I2,I3)                           )  \
                                          *URS2(I1,I2,I3,m1,m2,m3)        \
        +(SQR(SX(I1,I2,I3))                )*USS2(I1,I2,I3,m1,m2,m3)        \
             +(RXX2(I1,I2,I3)              )*UR2(I1,I2,I3,m1,m2,m3)       \
             +(SXX2(I1,I2,I3)              )*US2(I1,I2,I3,m1,m2,m3) )
#define UYY2(I1,I2,I3,m1,m2,m3)                                           \
       ( (              SQR(RY(I1,I2,I3)))*URR2(I1,I2,I3,m1,m2,m3)        \
        +2.*(                           RY(I1,I2,I3)*SY(I1,I2,I3))  \
                                          *URS2(I1,I2,I3,m1,m2,m3)        \
        +(              SQR(SY(I1,I2,I3)))*USS2(I1,I2,I3,m1,m2,m3)        \
             +(              RYY2(I1,I2,I3))*UR2(I1,I2,I3,m1,m2,m3)       \
             +(              SYY2(I1,I2,I3))*US2(I1,I2,I3,m1,m2,m3)  )
#define UXY2(I1,I2,I3,m1,m2,m3)                                           \
       (    RX(I1,I2,I3)*RY(I1,I2,I3)*URR2(I1,I2,I3,m1,m2,m3)             \
        +(RX(I1,I2,I3)*SY(I1,I2,I3)+RY(I1,I2,I3)*SX(I1,I2,I3))      \
                                     *URS2(I1,I2,I3,m1,m2,m3)             \
        +   SX(I1,I2,I3)*SY(I1,I2,I3)*USS2(I1,I2,I3,m1,m2,m3)             \
          +RXY2(I1,I2,I3)              *UR2(I1,I2,I3,m1,m2,m3)            \
          +SXY2(I1,I2,I3)              *US2(I1,I2,I3,m1,m2,m3)  )

#define UXX23(I1,I2,I3,m1,m2,m3)                                         \
       ( SQR(RX(I1,I2,I3)) *URR2(I1,I2,I3,m1,m2,m3)                        \
        +SQR(SX(I1,I2,I3)) *USS2(I1,I2,I3,m1,m2,m3)                        \
        +SQR(TX(I1,I2,I3)) *UTT2(I1,I2,I3,m1,m2,m3)                        \
        +2.*RX(I1,I2,I3)*SX(I1,I2,I3)*URS2(I1,I2,I3,m1,m2,m3)            \
        +2.*RX(I1,I2,I3)*TX(I1,I2,I3)*URT2(I1,I2,I3,m1,m2,m3)            \
        +2.*SX(I1,I2,I3)*TX(I1,I2,I3)*UST2(I1,I2,I3,m1,m2,m3)            \
        +RXX23(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)                          \
        +SXX23(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)                          \
        +TXX23(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3) )

#define UYY23(I1,I2,I3,m1,m2,m3)   \
       ( SQR(RY(I1,I2,I3)) *URR2(I1,I2,I3,m1,m2,m3)   \
        +SQR(SY(I1,I2,I3)) *USS2(I1,I2,I3,m1,m2,m3)   \
        +SQR(TY(I1,I2,I3)) *UTT2(I1,I2,I3,m1,m2,m3)   \
        +2.*RY(I1,I2,I3)*SY(I1,I2,I3)*URS2(I1,I2,I3,m1,m2,m3)   \
        +2.*RY(I1,I2,I3)*TY(I1,I2,I3)*URT2(I1,I2,I3,m1,m2,m3)   \
        +2.*SY(I1,I2,I3)*TY(I1,I2,I3)*UST2(I1,I2,I3,m1,m2,m3)   \
        +RYY23(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)   \
        +SYY23(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)   \
        +TYY23(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3) )

#define UZZ23(I1,I2,I3,m1,m2,m3)   \
       ( SQR(RZ(I1,I2,I3)) *URR2(I1,I2,I3,m1,m2,m3)   \
        +SQR(SZ(I1,I2,I3)) *USS2(I1,I2,I3,m1,m2,m3)   \
        +SQR(TZ(I1,I2,I3)) *UTT2(I1,I2,I3,m1,m2,m3)   \
        +2.*RZ(I1,I2,I3)*SZ(I1,I2,I3)*URS2(I1,I2,I3,m1,m2,m3)   \
        +2.*RZ(I1,I2,I3)*TZ(I1,I2,I3)*URT2(I1,I2,I3,m1,m2,m3)   \
        +2.*SZ(I1,I2,I3)*TZ(I1,I2,I3)*UST2(I1,I2,I3,m1,m2,m3)   \
        +RZZ23(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)   \
        +SZZ23(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)   \
        +TZZ23(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3) )

#define UXY23(I1,I2,I3,m1,m2,m3)      \
     ( RX(I1,I2,I3)*RY(I1,I2,I3)*URR2(I1,I2,I3,m1,m2,m3)    \
      +SX(I1,I2,I3)*SY(I1,I2,I3)*USS2(I1,I2,I3,m1,m2,m3)    \
      +TX(I1,I2,I3)*TY(I1,I2,I3)*UTT2(I1,I2,I3,m1,m2,m3)    \
      +(RX(I1,I2,I3)*SY(I1,I2,I3)+RY(I1,I2,I3)*SX(I1,I2,I3))    \
                                       *URS2(I1,I2,I3,m1,m2,m3)    \
      +(RX(I1,I2,I3)*TY(I1,I2,I3)+RY(I1,I2,I3)*TX(I1,I2,I3))    \
                                       *URT2(I1,I2,I3,m1,m2,m3)    \
      +(SX(I1,I2,I3)*TY(I1,I2,I3)+SY(I1,I2,I3)*TX(I1,I2,I3))    \
                                       *UST2(I1,I2,I3,m1,m2,m3)    \
      +RXY23(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)    \
      +SXY23(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)    \
      +TXY23(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3) )                  


#define UXZ23(I1,I2,I3,m1,m2,m3)   \
       (  RX(I1,I2,I3)*RZ(I1,I2,I3)*URR2(I1,I2,I3,m1,m2,m3)   \
         +SX(I1,I2,I3)*SZ(I1,I2,I3)*USS2(I1,I2,I3,m1,m2,m3)   \
         +TX(I1,I2,I3)*TZ(I1,I2,I3)*UTT2(I1,I2,I3,m1,m2,m3)   \
         +(RX(I1,I2,I3)*SZ(I1,I2,I3)+RZ(I1,I2,I3)*SX(I1,I2,I3))   \
                                          *URS2(I1,I2,I3,m1,m2,m3)   \
         +(RX(I1,I2,I3)*TZ(I1,I2,I3)+RZ(I1,I2,I3)*TX(I1,I2,I3))   \
                                          *URT2(I1,I2,I3,m1,m2,m3)   \
         +(SX(I1,I2,I3)*TZ(I1,I2,I3)+SZ(I1,I2,I3)*TX(I1,I2,I3))   \
                                          *UST2(I1,I2,I3,m1,m2,m3)   \
         +RXZ23(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)   \
         +SXZ23(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)   \
         +TXZ23(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3)  )

#define UYZ23(I1,I2,I3,m1,m2,m3)   \
       (  RY(I1,I2,I3)*RZ(I1,I2,I3)*URR2(I1,I2,I3,m1,m2,m3)   \
         +SY(I1,I2,I3)*SZ(I1,I2,I3)*USS2(I1,I2,I3,m1,m2,m3)   \
         +TY(I1,I2,I3)*TZ(I1,I2,I3)*UTT2(I1,I2,I3,m1,m2,m3)   \
         +(RY(I1,I2,I3)*SZ(I1,I2,I3)+RZ(I1,I2,I3)*SY(I1,I2,I3))   \
                                          *URS2(I1,I2,I3,m1,m2,m3)   \
         +(RY(I1,I2,I3)*TZ(I1,I2,I3)+RZ(I1,I2,I3)*TY(I1,I2,I3))   \
                                          *URT2(I1,I2,I3,m1,m2,m3)   \
         +(SY(I1,I2,I3)*TZ(I1,I2,I3)+SZ(I1,I2,I3)*TY(I1,I2,I3))   \
                                          *UST2(I1,I2,I3,m1,m2,m3)   \
         +RYZ23(I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)   \
         +SYZ23(I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)   \
         +TYZ23(I1,I2,I3)*UT2(I1,I2,I3,m1,m2,m3)  )

