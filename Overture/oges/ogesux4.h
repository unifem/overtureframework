//
// CGUX statement functions for fourth-order difference approximations
//
//  needs
//      Dr4(m1) = 1 -8 8 1
//      d14=1./(12.*c.gridSpacing);
//      d24=1./(12.*SQR(c.gridSpacing));

#if !defined(RX)
  #define RX(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,0)
  #define SX(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,1)
  #define TX(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,2)
  #define RY(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,0+numberOfDimensions)
  #define SY(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,1+numberOfDimensions)
  #define TY(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,5)
  #define RZ(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,6)
  #define SZ(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,7)
  #define TZ(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,8)
#endif

#define UR4(I1,I2,I3,m1,m2,m3) (Dr4(m1)*delta(m2)*delta(m3))
#define US4(I1,I2,I3,m1,m2,m3) (Ds4(m2)*delta(m1)*delta(m3))
#define UT4(I1,I2,I3,m1,m2,m3) (Dt4(m3)*delta(m1)*delta(m2))

#define URR4(I1,I2,I3,m1,m2,m3) (Drr4(m1)*delta(m2)*delta(m3))
#define USS4(I1,I2,I3,m1,m2,m3) (Dss4(m2)*delta(m1)*delta(m3))
#define URS4(I1,I2,I3,m1,m2,m3) (Drs4(m1,m2)*delta(m3))
#define UTT4(I1,I2,I3,m1,m2,m3) (Dtt4(m3)*delta(m1)*delta(m2))
#define URT4(I1,I2,I3,m1,m2,m3) (Drt4(m1,m3)*delta(m2))
#define UST4(I1,I2,I3,m1,m2,m3) (Dst4(m2,m3)*delta(m1))


#define RXR4(I1,I2,I3) ( (8.*(RX(I1+1,I2,I3)-RX(I1-1,I2,I3))    \
                            -(RX(I1+2,I2,I3)-RX(I1-2,I2,I3)))*d14(axis1) )
#define RYR4(I1,I2,I3) ( (8.*(RY(I1+1,I2,I3)-RY(I1-1,I2,I3))    \
                            -(RY(I1+2,I2,I3)-RY(I1-2,I2,I3)))*d14(axis1) )
#define RZR4(I1,I2,I3) ( (8.*(RZ(I1+1,I2,I3)-RZ(I1-1,I2,I3))    \
                            -(RZ(I1+2,I2,I3)-RZ(I1-2,I2,I3)))*d14(axis1) )
#define RXS4(I1,I2,I3) ( (8.*(RX(I1,I2+1,I3)-RX(I1,I2-1,I3))    \
                            -(RX(I1,I2+2,I3)-RX(I1,I2-2,I3)))*d14(axis2) )
#define RYS4(I1,I2,I3) ( (8.*(RY(I1,I2+1,I3)-RY(I1,I2-1,I3))    \
                            -(RY(I1,I2+2,I3)-RY(I1,I2-2,I3)))*d14(axis2) )
#define RZS4(I1,I2,I3) ( (8.*(RZ(I1,I2+1,I3)-RZ(I1,I2-1,I3))    \
                            -(RZ(I1,I2+2,I3)-RZ(I1,I2-2,I3)))*d14(axis2) )
#define RXT4(I1,I2,I3) ( (8.*(RX(I1,I2,I3+1)-RX(I1,I2,I3-1))    \
                            -(RX(I1,I2,I3+2)-RX(I1,I2,I3-2)))*d14(axis3) )
#define RYT4(I1,I2,I3) ( (8.*(RY(I1,I2,I3+1)-RY(I1,I2,I3-1))    \
                            -(RY(I1,I2,I3+2)-RY(I1,I2,I3-2)))*d14(axis3) )
#define RZT4(I1,I2,I3) ( (8.*(RZ(I1,I2,I3+1)-RZ(I1,I2,I3-1))    \
                            -(RZ(I1,I2,I3+2)-RZ(I1,I2,I3-2)))*d14(axis3) )
#define SXR4(I1,I2,I3) ( (8.*(SX(I1+1,I2,I3)-SX(I1-1,I2,I3))    \
                            -(SX(I1+2,I2,I3)-SX(I1-2,I2,I3)))*d14(axis1) )
#define SYR4(I1,I2,I3) ( (8.*(SY(I1+1,I2,I3)-SY(I1-1,I2,I3))    \
                            -(SY(I1+2,I2,I3)-SY(I1-2,I2,I3)))*d14(axis1) )
#define SZR4(I1,I2,I3) ( (8.*(SZ(I1+1,I2,I3)-SZ(I1-1,I2,I3))    \
                            -(SZ(I1+2,I2,I3)-SZ(I1-2,I2,I3)))*d14(axis1) )
#define SXS4(I1,I2,I3) ( (8.*(SX(I1,I2+1,I3)-SX(I1,I2-1,I3))    \
                            -(SX(I1,I2+2,I3)-SX(I1,I2-2,I3)))*d14(axis2) )
#define SYS4(I1,I2,I3) ( (8.*(SY(I1,I2+1,I3)-SY(I1,I2-1,I3))    \
                            -(SY(I1,I2+2,I3)-SY(I1,I2-2,I3)))*d14(axis2) )
#define SZS4(I1,I2,I3) ( (8.*(SZ(I1,I2+1,I3)-SZ(I1,I2-1,I3))    \
                            -(SZ(I1,I2+2,I3)-SZ(I1,I2-2,I3)))*d14(axis2) )
#define SXT4(I1,I2,I3) ( (8.*(SX(I1,I2,I3+1)-SX(I1,I2,I3-1))    \
                            -(SX(I1,I2,I3+2)-SX(I1,I2,I3-2)))*d14(axis3) )
#define SYT4(I1,I2,I3) ( (8.*(SY(I1,I2,I3+1)-SY(I1,I2,I3-1))    \
                            -(SY(I1,I2,I3+2)-SY(I1,I2,I3-2)))*d14(axis3) )
#define SZT4(I1,I2,I3) ( (8.*(SZ(I1,I2,I3+1)-SZ(I1,I2,I3-1))    \
                            -(SZ(I1,I2,I3+2)-SZ(I1,I2,I3-2)))*d14(axis3) )
#define TXR4(I1,I2,I3) ( (8.*(TX(I1+1,I2,I3)-TX(I1-1,I2,I3))    \
                            -(TX(I1+2,I2,I3)-TX(I1-2,I2,I3)))*d14(axis1) )
#define TYR4(I1,I2,I3) ( (8.*(TY(I1+1,I2,I3)-TY(I1-1,I2,I3))    \
                            -(TY(I1+2,I2,I3)-TY(I1-2,I2,I3)))*d14(axis1) )
#define TZR4(I1,I2,I3) ( (8.*(TZ(I1+1,I2,I3)-TZ(I1-1,I2,I3))    \
                            -(TZ(I1+2,I2,I3)-TZ(I1-2,I2,I3)))*d14(axis1) )
#define TXS4(I1,I2,I3) ( (8.*(TX(I1,I2+1,I3)-TX(I1,I2-1,I3))    \
                            -(TX(I1,I2+2,I3)-TX(I1,I2-2,I3)))*d14(axis2) )
#define TYS4(I1,I2,I3) ( (8.*(TY(I1,I2+1,I3)-TY(I1,I2-1,I3))    \
                            -(TY(I1,I2+2,I3)-TY(I1,I2-2,I3)))*d14(axis2) )
#define TZS4(I1,I2,I3) ( (8.*(TZ(I1,I2+1,I3)-TZ(I1,I2-1,I3))    \
                            -(TZ(I1,I2+2,I3)-TZ(I1,I2-2,I3)))*d14(axis2) )
#define TXT4(I1,I2,I3) ( (8.*(TX(I1,I2,I3+1)-TX(I1,I2,I3-1))    \
                            -(TX(I1,I2,I3+2)-TX(I1,I2,I3-2)))*d14(axis3) )
#define TYT4(I1,I2,I3) ( (8.*(TY(I1,I2,I3+1)-TY(I1,I2,I3-1))    \
                            -(TY(I1,I2,I3+2)-TY(I1,I2,I3-2)))*d14(axis3) )
#define TZT4(I1,I2,I3) ( (8.*(TZ(I1,I2,I3+1)-TZ(I1,I2,I3-1))    \
                            -(TZ(I1,I2,I3+2)-TZ(I1,I2,I3-2)))*d14(axis3) )

#define UX4(I1,I2,I3,m1,m2,m3)  ( RX(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)   \
                           +SX(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3) )
#define UY4(I1,I2,I3,m1,m2,m3)  ( RY(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)   \
                           +SY(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3) )
#define UX43(I1,I2,I3,m1,m2,m3) ( RX(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)   \
                           +SX(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)   \
                           +TX(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3) )
#define UY43(I1,I2,I3,m1,m2,m3) ( RY(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)  \
                           +SY(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)  \
                           +TY(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3) )
#define UZ43(I1,I2,I3,m1,m2,m3) ( RZ(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)  \
                           +SZ(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)  \
                           +TZ(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3) )

#define RXX4(I1,I2,I3) ( RX(I1,I2,I3)*RXR4(I1,I2,I3)     \
                        +SX(I1,I2,I3)*RXS4(I1,I2,I3) )
#define RXY4(I1,I2,I3) ( RX(I1,I2,I3)*RYR4(I1,I2,I3)     \
                        +SX(I1,I2,I3)*RYS4(I1,I2,I3) )
#define RYY4(I1,I2,I3) ( RY(I1,I2,I3)*RYR4(I1,I2,I3)     \
                        +SY(I1,I2,I3)*RYS4(I1,I2,I3) )
#define SXX4(I1,I2,I3) ( RX(I1,I2,I3)*SXR4(I1,I2,I3)     \
                        +SX(I1,I2,I3)*SXS4(I1,I2,I3) )
#define SXY4(I1,I2,I3) ( RX(I1,I2,I3)*SYR4(I1,I2,I3)     \
                        +SX(I1,I2,I3)*SYS4(I1,I2,I3) )
#define SYY4(I1,I2,I3) ( RY(I1,I2,I3)*SYR4(I1,I2,I3)     \
                        +SY(I1,I2,I3)*SYS4(I1,I2,I3) )

#define RXX43(I1,I2,I3) ( RX(I1,I2,I3)*RXR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*RXS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*RXT4(I1,I2,I3) )
#define RXY43(I1,I2,I3) ( RX(I1,I2,I3)*RYR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*RYS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*RYT4(I1,I2,I3) )
#define RXZ43(I1,I2,I3) ( RX(I1,I2,I3)*RZR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*RZS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*RZT4(I1,I2,I3) )
#define RYY43(I1,I2,I3) ( RY(I1,I2,I3)*RYR4(I1,I2,I3)     \
                         +SY(I1,I2,I3)*RYS4(I1,I2,I3)     \
                         +TY(I1,I2,I3)*RYT4(I1,I2,I3) )
#define RYZ43(I1,I2,I3) ( RY(I1,I2,I3)*RZR4(I1,I2,I3)     \
                         +SY(I1,I2,I3)*RZS4(I1,I2,I3)     \
                         +TY(I1,I2,I3)*RZT4(I1,I2,I3) )
#define RZZ43(I1,I2,I3) ( RZ(I1,I2,I3)*RZR4(I1,I2,I3)     \
                         +SZ(I1,I2,I3)*RZS4(I1,I2,I3)     \
                         +TZ(I1,I2,I3)*RZT4(I1,I2,I3) )
#define SXX43(I1,I2,I3) ( RX(I1,I2,I3)*SXR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*SXS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*SXT4(I1,I2,I3) )
#define SXY43(I1,I2,I3) ( RX(I1,I2,I3)*SYR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*SYS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*SYT4(I1,I2,I3) )
#define SXZ43(I1,I2,I3) ( RX(I1,I2,I3)*SZR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*SZS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*SZT4(I1,I2,I3) )
#define SYY43(I1,I2,I3) ( RY(I1,I2,I3)*SYR4(I1,I2,I3)     \
                         +SY(I1,I2,I3)*SYS4(I1,I2,I3)     \
                         +TY(I1,I2,I3)*SYT4(I1,I2,I3) )
#define SYZ43(I1,I2,I3) ( RY(I1,I2,I3)*SZR4(I1,I2,I3)     \
                         +SY(I1,I2,I3)*SZS4(I1,I2,I3)     \
                         +TY(I1,I2,I3)*SZT4(I1,I2,I3) )
#define SZZ43(I1,I2,I3) ( RZ(I1,I2,I3)*SZR4(I1,I2,I3)     \
                         +SZ(I1,I2,I3)*SZS4(I1,I2,I3)     \
                         +TZ(I1,I2,I3)*SZT4(I1,I2,I3) )
#define TXX43(I1,I2,I3) ( RX(I1,I2,I3)*TXR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*TXS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*TXT4(I1,I2,I3) )
#define TXY43(I1,I2,I3) ( RX(I1,I2,I3)*TYR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*TYS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*TYT4(I1,I2,I3) )
#define TXZ43(I1,I2,I3) ( RX(I1,I2,I3)*TZR4(I1,I2,I3)     \
                         +SX(I1,I2,I3)*TZS4(I1,I2,I3)     \
                         +TX(I1,I2,I3)*TZT4(I1,I2,I3) )
#define TYY43(I1,I2,I3) ( RY(I1,I2,I3)*TYR4(I1,I2,I3)     \
                         +SY(I1,I2,I3)*TYS4(I1,I2,I3)     \
                         +TY(I1,I2,I3)*TYT4(I1,I2,I3) )
#define TYZ43(I1,I2,I3) ( RY(I1,I2,I3)*TZR4(I1,I2,I3)     \
                         +SY(I1,I2,I3)*TZS4(I1,I2,I3)     \
                         +TY(I1,I2,I3)*TZT4(I1,I2,I3) )
#define TZZ43(I1,I2,I3) ( RZ(I1,I2,I3)*TZR4(I1,I2,I3)     \
                         +SZ(I1,I2,I3)*TZS4(I1,I2,I3)     \
                         +TZ(I1,I2,I3)*TZT4(I1,I2,I3) )

#define UXX4(I1,I2,I3,m1,m2,m3)   (   \
       (SQR(RX(I1,I2,I3))                )*URR4(I1,I2,I3,m1,m2,m3)     \
      +2.*(RX(I1,I2,I3)*SX(I1,I2,I3)                           )     \
                                        *URS4(I1,I2,I3,m1,m2,m3)     \
      +(SQR(SX(I1,I2,I3))                )*USS4(I1,I2,I3,m1,m2,m3)     \
           +(RXX4(I1,I2,I3)              )*UR4(I1,I2,I3,m1,m2,m3)     \
           +(SXX4(I1,I2,I3)              )*US4(I1,I2,I3,m1,m2,m3)   \
                            )
#define UYY4(I1,I2,I3,m1,m2,m3)   (   \
       (              SQR(RY(I1,I2,I3)))*URR4(I1,I2,I3,m1,m2,m3)     \
      +2.*(                           RY(I1,I2,I3)*SY(I1,I2,I3))     \
                                        *URS4(I1,I2,I3,m1,m2,m3)     \
      +(              SQR(SY(I1,I2,I3)))*USS4(I1,I2,I3,m1,m2,m3)     \
           +(              RYY4(I1,I2,I3))*UR4(I1,I2,I3,m1,m2,m3)     \
           +(              SYY4(I1,I2,I3))*US4(I1,I2,I3,m1,m2,m3)  \
                            )
#define UXY4(I1,I2,I3,m1,m2,m3)   (   \
          RX(I1,I2,I3)*RY(I1,I2,I3)*URR4(I1,I2,I3,m1,m2,m3)     \
      +(RX(I1,I2,I3)*SY(I1,I2,I3)+RY(I1,I2,I3)*SX(I1,I2,I3))     \
                                   *URS4(I1,I2,I3,m1,m2,m3)     \
      +   SX(I1,I2,I3)*SY(I1,I2,I3)*USS4(I1,I2,I3,m1,m2,m3)     \
        +RXY4(I1,I2,I3)              *UR4(I1,I2,I3,m1,m2,m3)     \
        +SXY4(I1,I2,I3)              *US4(I1,I2,I3,m1,m2,m3)  \
                            )
#define UXX43(I1,I2,I3,m1,m2,m3)  (    \
       SQR(RX(I1,I2,I3)) *URR4(I1,I2,I3,m1,m2,m3)     \
      +SQR(SX(I1,I2,I3)) *USS4(I1,I2,I3,m1,m2,m3)     \
      +SQR(TX(I1,I2,I3)) *UTT4(I1,I2,I3,m1,m2,m3)     \
      +2.*RX(I1,I2,I3)*SX(I1,I2,I3)*URS4(I1,I2,I3,m1,m2,m3)     \
      +2.*RX(I1,I2,I3)*TX(I1,I2,I3)*URT4(I1,I2,I3,m1,m2,m3)     \
      +2.*SX(I1,I2,I3)*TX(I1,I2,I3)*UST4(I1,I2,I3,m1,m2,m3)     \
      +RXX43(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)     \
      +SXX43(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)     \
      +TXX43(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3)     \
                            )
#define UYY43(I1,I2,I3,m1,m2,m3)  (    \
       SQR(RY(I1,I2,I3)) *URR4(I1,I2,I3,m1,m2,m3)     \
      +SQR(SY(I1,I2,I3)) *USS4(I1,I2,I3,m1,m2,m3)     \
      +SQR(TY(I1,I2,I3)) *UTT4(I1,I2,I3,m1,m2,m3)     \
      +2.*RY(I1,I2,I3)*SY(I1,I2,I3)*URS4(I1,I2,I3,m1,m2,m3)     \
      +2.*RY(I1,I2,I3)*TY(I1,I2,I3)*URT4(I1,I2,I3,m1,m2,m3)     \
      +2.*SY(I1,I2,I3)*TY(I1,I2,I3)*UST4(I1,I2,I3,m1,m2,m3)     \
      +RYY43(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)     \
      +SYY43(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)     \
      +TYY43(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3)  \
                            )
#define UZZ43(I1,I2,I3,m1,m2,m3)  (    \
       SQR(RZ(I1,I2,I3)) *URR4(I1,I2,I3,m1,m2,m3)     \
      +SQR(SZ(I1,I2,I3)) *USS4(I1,I2,I3,m1,m2,m3)     \
      +SQR(TZ(I1,I2,I3)) *UTT4(I1,I2,I3,m1,m2,m3)     \
      +2.*RZ(I1,I2,I3)*SZ(I1,I2,I3)*URS4(I1,I2,I3,m1,m2,m3)     \
      +2.*RZ(I1,I2,I3)*TZ(I1,I2,I3)*URT4(I1,I2,I3,m1,m2,m3)     \
      +2.*SZ(I1,I2,I3)*TZ(I1,I2,I3)*UST4(I1,I2,I3,m1,m2,m3)     \
      +RZZ43(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)     \
      +SZZ43(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)     \
      +TZZ43(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3)     \
                            )
#define UXY43(I1,I2,I3,m1,m2,m3)  (    \
        RX(I1,I2,I3)*RY(I1,I2,I3)*URR4(I1,I2,I3,m1,m2,m3)     \
       +SX(I1,I2,I3)*SY(I1,I2,I3)*USS4(I1,I2,I3,m1,m2,m3)     \
       +TX(I1,I2,I3)*TY(I1,I2,I3)*UTT4(I1,I2,I3,m1,m2,m3)     \
       +(RX(I1,I2,I3)*SY(I1,I2,I3)+RY(I1,I2,I3)*SX(I1,I2,I3))     \
                                        *URS4(I1,I2,I3,m1,m2,m3)     \
       +(RX(I1,I2,I3)*TY(I1,I2,I3)+RY(I1,I2,I3)*TX(I1,I2,I3))     \
                                        *URT4(I1,I2,I3,m1,m2,m3)     \
       +(SX(I1,I2,I3)*TY(I1,I2,I3)+SY(I1,I2,I3)*TX(I1,I2,I3))     \
                                        *UST4(I1,I2,I3,m1,m2,m3)     \
       +RXY43(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)     \
       +SXY43(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)     \
       +TXY43(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3)     \
                            )
#define UXZ43(I1,I2,I3,m1,m2,m3)  (    \
        RX(I1,I2,I3)*RZ(I1,I2,I3)*URR4(I1,I2,I3,m1,m2,m3)     \
       +SX(I1,I2,I3)*SZ(I1,I2,I3)*USS4(I1,I2,I3,m1,m2,m3)     \
       +TX(I1,I2,I3)*TZ(I1,I2,I3)*UTT4(I1,I2,I3,m1,m2,m3)     \
       +(RX(I1,I2,I3)*SZ(I1,I2,I3)+RZ(I1,I2,I3)*SX(I1,I2,I3))     \
                                        *URS4(I1,I2,I3,m1,m2,m3)     \
       +(RX(I1,I2,I3)*TZ(I1,I2,I3)+RZ(I1,I2,I3)*TX(I1,I2,I3))     \
                                        *URT4(I1,I2,I3,m1,m2,m3)     \
       +(SX(I1,I2,I3)*TZ(I1,I2,I3)+SZ(I1,I2,I3)*TX(I1,I2,I3))     \
                                        *UST4(I1,I2,I3,m1,m2,m3)     \
       +RXZ43(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)     \
       +SXZ43(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)     \
       +TXZ43(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3)     \
                            )
#define UYZ43(I1,I2,I3,m1,m2,m3)  (    \
        RY(I1,I2,I3)*RZ(I1,I2,I3)*URR4(I1,I2,I3,m1,m2,m3)     \
       +SY(I1,I2,I3)*SZ(I1,I2,I3)*USS4(I1,I2,I3,m1,m2,m3)     \
       +TY(I1,I2,I3)*TZ(I1,I2,I3)*UTT4(I1,I2,I3,m1,m2,m3)     \
       +(RY(I1,I2,I3)*SZ(I1,I2,I3)+RZ(I1,I2,I3)*SY(I1,I2,I3))     \
                                        *URS4(I1,I2,I3,m1,m2,m3)     \
       +(RY(I1,I2,I3)*TZ(I1,I2,I3)+RZ(I1,I2,I3)*TY(I1,I2,I3))     \
                                        *URT4(I1,I2,I3,m1,m2,m3)     \
       +(SY(I1,I2,I3)*TZ(I1,I2,I3)+SZ(I1,I2,I3)*TY(I1,I2,I3))     \
                                        *UST4(I1,I2,I3,m1,m2,m3)     \
       +RYZ43(I1,I2,I3)*UR4(I1,I2,I3,m1,m2,m3)     \
       +SYZ43(I1,I2,I3)*US4(I1,I2,I3,m1,m2,m3)     \
       +TYZ43(I1,I2,I3)*UT4(I1,I2,I3,m1,m2,m3)     
