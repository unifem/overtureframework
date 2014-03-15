//
//  C++ interface to hypgen: allocate space for work arrays
//
#include "Mapping.h"

#define HYPGEN EXTERN_C_NAME(hypgen)

extern "C"
{
  void HYPGEN(const int & iform, const int & izstrt, const int & nzreg,
              int & npzreg, real & zreg, real & dz0, real & dz1, 
              int & ibcja, int & ibcjb, int & ibcka, int & ibckb,
              int & ivspec, real & epsss, int & itsvol,
              int & imeth, real & smu2,
              real & timj, real & timk,
              int & iaxis, real & exaxis, real & volres,
              int & jmax, int & kmax,
	      int & jdim, int & kdim, int & lmax,
              real & x, real & y, real & z,
              real & xw, real & yw, real & zw,
              const int & m3d, const int & m2d, const int & m1d,
	      real & RR,            
	      real & VOLM,real & VOL,real & SR,
	      real & XX,real & YX,real & ZX,real & XE,real & YE,real & ZE,real & XZ,real & YZ,real & ZZ,
	      real & XIDS,real & ETDS,real & ADXI,real & ADET,real & ADRXI,real & ADRET,
	      real & DAREA,real & JKBAD,
	      real & H,real & A,real & B,real & C,real & F,
	      real & CAXI,real & CAET,real & CVEX,
	      real & AFNXI,real & AFNET,real & BLN,
	      real & TMP2,
	      real & JJP,real & JJR,real & KKP,real & KKR,
	      real & SPHI,real & R,real & LBAD,real & NBAD,real & DISSL,real & DLC,real & DAREAS,
	      real & TMP1, real & ITMP1 );
}

int 
hyper(
      int & IFORM, int & IZSTRT, int & NZREG,
      int & NPZREG, real & ZREG,  real & DZ0, real &  DZ1,
      int & IBCJA,int & IBCJB,int & IBCKA,int & IBCKB,
      int & IVSPEC, real & EPSSS, int & ITSVOL,
      int & IMETH, real & SMU2,
      real & TIMJ, real & TIMK,
      int & IAXIS, real & EXAXIS, real & VOLRES,
      int & JMAX, int & KMAX,
      int & JDIM,int & KDIM,int & LMAX,
      real & X, real & Y, real & Z,
      realArray & XW, realArray & YW, realArray & ZW )
{
  
//      dimension NPZREG(NZREG), ZREG(NZREG), DZ0(NZREG), DZ1(NZREG)
//      dimension X(*),Y(*),Z(*)
//      dimension XW(*),YW(*),ZW(*)

	//     M3D = max number of points in 3D grid
//C     M2D = max number of points in 2D slice (J-K plane)
//C     M1D = max number of points in any one dimension 
//C
//C     Allow at least two extra points in each direction for M3D,M2D,M1D.
//C     These parameters appear in the main program only.
//C
//C     --------------------------------------------
//c*wdh      PARAMETER (M3D=1781001, M2D=35001, M1D=401)
  const int m3d=JDIM*KDIM*(LMAX+2);
  const int m2d=JDIM*KDIM;
  const int m1d=max(JDIM,max(KDIM,LMAX));
//     --------------------------------------------
//

  if( XW.elementCount() != m3d )
    XW.redim(m3d);
  if( YW.elementCount() != m3d )
    YW.redim(m3d);
  if( ZW.elementCount() != m3d )
    ZW.redim(m3d);

  int RR=0,
    VOLM = RR + m3d, 
    VOL  = VOLM+m2d,
    SR   = VOL+ m2d,
    XX   = SR + m2d*3,
    YX   = XX + m2d,
    ZX   = YX + m2d,
    XE   = ZX + m2d,
    YE   = XE + m2d,
    ZE   = YE + m2d,
    XZ   = ZE + m2d,
    YZ   = XZ + m2d,
    ZZ   = YZ + m2d,
    XIDS = ZZ + m2d,
    ETDS =XIDS+ m2d,
    ADXI =ETDS+ m2d,
    ADET =ADXI+ m2d,
    ADRXI=ADET+m2d,
    ADRET=ADRXI+m2d,
    DAREA=ADRET+m2d,
    JKBAD=DAREA+m2d,
    H    =JKBAD+2*m2d,
    A    =H+9*m2d,
    B    =A+9*m2d,
    C    =B+9*m2d,
    F    =C+9*m2d,
    CAXI =F+3*m2d,
    CAET=CAXI+m2d,
    CVEX=CAET+m2d,
    AFNXI=CVEX+m2d,
    AFNET=AFNXI+m2d,
    BLN=AFNET+m2d,
    TMP2=BLN+m2d,
    JJP=TMP2+m2d*9,
    JJR=JJP+m1d,
    KKP=JJR+m1d,
    KKR=KKP+m1d,
    SPHI=KKR+m1d,
    R=SPHI+m1d,
    LBAD=R+m1d,
    NBAD=LBAD+m1d,
    DISSL=NBAD+m1d,
    DLC=DISSL+m1d,
    DAREAS=DLC+m1d,
    TMP1=DAREAS+m1d,
    ITMP1=TMP1,
    total=ITMP1+m1d*9;
  
  real *w  = new real [total];

  HYPGEN(   
      IFORM, IZSTRT, NZREG,
      NPZREG, ZREG, DZ0, DZ1,
      IBCJA,IBCJB,IBCKA,IBCKB,
      IVSPEC,EPSSS,ITSVOL,
      IMETH,SMU2,
      TIMJ,TIMK,
      IAXIS,EXAXIS,VOLRES,
      JMAX, KMAX,
      JDIM,KDIM,LMAX,
      X,Y,Z,
      XW(0),YW(0),ZW(0),
      m3d, m2d, m1d,
      w[RR],            
      w[VOLM],w[VOL],w[SR],
      w[XX],w[YX],w[ZX],w[XE],w[YE],w[ZE],w[XZ],w[YZ],w[ZZ],
      w[XIDS],w[ETDS],w[ADXI],w[ADET],w[ADRXI],w[ADRET],
      w[DAREA],w[JKBAD],
      w[H],w[A],w[B],w[C],w[F],
      w[CAXI],w[CAET],w[CVEX],
      w[AFNXI],w[AFNET],w[BLN],
      w[TMP2],
      w[JJP],w[JJR],w[KKP],w[KKR],
      w[SPHI],w[R],w[LBAD],w[NBAD],w[DISSL],w[DLC],w[DAREAS],
      w[TMP1],w[ITMP1] );

  delete [] w;  // *wdh* 981203
  return 0;
}
