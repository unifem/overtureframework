C***********************************************************************
C   Program to generate surface grid with hyperbolic pde's over
C   arbitrary surface.
C
C   The grid generation process consists of two steps:
C
C   (1) Hyperbolic marching scheme to generate grid with the following
C       system of equations:
C    (a) orthogonality of xi w.r.t. eta,
C    (b) orthogonality eta w.r.t. surface normals
C    (c) mesh cell area constraint,
C
C   (2) Projection of grid points on to surface definition which can
C       be of two types
C    (i) PLOT3D multiple surface grid format (multiple panel networks)
C    (ii) NIGES format (NURBS surfaces only)
C
C   The following types of boundary conditions are available (see
C   header info in RDINPA subroutine for complete list)
C   (1) periodic
C   (2) constant plane
C   (3) reflected symmetry
C   (4) generalized floating with constraints
C   (5) fixed boundary (not fully tested yet, use at your own risk!)
C
C   This version of surgrd.f is modified from
C   wk23:~buning/grids/collar/nsurgrd.f          (wmc 15th Mar,90)
C
C----------------------------------------------------------------------
C wmc May, 92.
C New features in this version of surgrd include
C (1) New boundary conditions: constant plane, symmetry plane
C (2) Better projection/quad search algorithm
C (3) Second order smoothing
C (4) TIM factor
C (5) Generalized floating boundary BC (float along constant xi or eta
C     which does not have to be integer values, or user supplied set
C     of points, see INIPAR for options)
C (6) All common statements have been removed. Dimensions need to be
C     specified once only at the top
C (7) Angle-bisecting metrics
C (8) Auto-detection of sharp convex corners and treatment with
C     implicit averaging
C (9) All boundary condition parameters have been replaced by a single
C     BC type parameter at each J-boundary
C
C Features that need to be included in the future
C (1) Add extra point for floating bc
C (2) Spatially varying smoothing based on distance
C----------------------------------------------------------------------
C
C     Dimensions
C     ==========
C
C     M2D = max number of points for input or output 2D grid files
C     M1D = max dimension in J or K
C     MRGRD = max (no. of reference grids, no. of active grids)
C
C     MNOD = max number of nodes for piece-wise specifiable variable
C            far field distance
C
C     Static memory Requirement = (16*M1D + 4*MNOD + 22)*MRGRD
C                                 + 7*M2D + 6*M1D**2 + smaller stuff...
C
C***********************************************************************
C

      subroutine surgrd( 
     &          INIC, JMAX, JCMAX, IRFAM, 
     &          JRAXSA,JRAXSB,JRPER,KRPER,
     &          IBCJA, IBCJB, IAFAM,
     &          NGBCA, NGBCB, KMAX, NNOD,
     &          JNOD, ETAMX, DETA,
     &          DFAR, SMU, TIM, ITSVOL, 
     &          JRMAX,KRMAX,
     & xsurf,ndra,ndrb,ndsa,ndsb,ndta,ndtb,nra,nrb,nsa,nsb,nta,ntb,
     & xcurve,mdra,mdrb,mdsa,mdsb,mdta,mdtb,mra,mrb,msa,msb,mta,mtb,
     & xhype,ldra,ldrb,ldsa,ldsb,ldta,ldtb,lra,lrb,lsa,lsb,lta,ltb )


#include "precis.h"
#include "chimera_dimens.h"

      dimension xsurf(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*)
      dimension xcurve(mdra:mdrb,mdsa:mdsb,mdta:mdtb,*)
      dimension xhype(ldra:ldrb,ldsa:ldsb,ldta:ldtb,*)

      DIMENSION NGPR(MRGRD), NGNR(MRGRD), NGP1(MRGRD), 
     &          NGP(MRGRD), NGN(MRGRD)
      DIMENSION JRMAX(MRGRD), KRMAX(MRGRD)
      DIMENSION JRPER(MRGRD), KRPER(MRGRD), JRAXSA(MRGRD), JRAXSB(MRGRD)
      DIMENSION JRP(M1D*MRGRD),JRR(M1D*MRGRD),
     &          KRP(M1D*MRGRD),KRR(M1D*MRGRD)
      DIMENSION BMM(MRGRD,6)
      DIMENSION IGPB(M1D*12*MRGRD), IRFAM(MRGRD), IAFAM(MRGRD)
C
      DIMENSION JMAX(MRGRD), KMAX(MRGRD), NGPC(MRGRD), JCMAX(MRGRD),
     &          INIC(MRGRD), IBCJA(MRGRD), IBCJB(MRGRD),
     &          NGBCA(MRGRD), NGBCB(MRGRD),
     &          NNOD(MRGRD), JNOD(MNOD,MRGRD), ETAMX(MNOD,MRGRD),
     &          DETA(MNOD,MRGRD), DFAR(MNOD,MRGRD),
     &          SMU(MRGRD), TIM(MRGRD), ITSVOL(MRGRD)
C
      DIMENSION JPLNA(3),JPLNB(3)
      DIMENSION JJP(M1D),JJR(M1D)
      DIMENSION R(M2D), RH(M1D*3), DAREA(M1D)
      DIMENSION XX(M1D),YX(M1D),ZX(M1D),XE(M1D),YE(M1D),ZE(M1D)
C
      DIMENSION A(M1D*9),B(M1D*9),C(M1D*9),F(M1D*3),H(M1D*9),U(M1D*9)
      DIMENSION CSN1(M2D), CSN2(M2D), CSN3(M2D)
      DIMENSION GSN1(M1D), GSN2(M1D), GSN3(M1D),
     &          XSNR(M1D), YSNR(M1D), ZSNR(M1D)
      DIMENSION XB(M1D*2), YB(M1D*2), ZB(M1D*2)
      DIMENSION CAXI(M1D), XID(M1D*3), AVOL(M1D), JBMAX(M1D), ICUR(M1D)
      DIMENSION JKIH(M1D,3), DELS(M1D*3), DPT(M1D), ISEG(2)
      DIMENSION TMP1(M1D,3)
C
C   Note that the dimension of XI,YI,ZI is specifically M1D*M1D, which
C   could be larger than M2D.
C
      DIMENSION XI(M1D*M1D),YI(M1D*M1D),ZI(M1D,M1D)
C
      DIMENSION EV(M2D*3), GNV(M2D*3)

      call surgrd2( MRGRD, M1D, M2D, MNOD,
     & NGPR, NGNR, NGP1, NGP,NGN,JRMAX,KRMAX,
     & JRPER, KRPER, JRAXSA, JRAXSB,JRP,JRR,KRP,KRR,BMM,
     & IGPB,IRFAM,IAFAM,JMAX, KMAX, NGPC, JCMAX,INIC, IBCJA, IBCJB,
     & NGBCA, NGBCB, NNOD, JNOD, ETAMX, DETA, DFAR, SMU,TIM, ITSVOL,
     & JPLNA,JPLNB,JJP,JJR,R, RH, DAREA,XX,YX,ZX,XE,YE,ZE,A,B,C,F,H,U,
     & CSN1, CSN2, CSN3, GSN1, GSN2, GSN3, XSNR, YSNR, ZSNR,XB,YB,ZB,
     & CAXI, XID, AVOL, JBMAX, ICUR, JKIH, DELS, DPT, ISEG,TMP1,
     & XI,YI,ZI, EV,GNV,
     & xsurf,ndra,ndrb,ndsa,ndsb,ndta,ndtb,nra,nrb,nsa,nsb,nta,ntb,
     & xcurve,mdra,mdrb,mdsa,mdsb,mdta,mdtb,mra,mrb,msa,msb,mta,mtb,
     & xhype,ldra,ldrb,ldsa,ldsb,ldta,ldtb,lra,lrb,lsa,lsb,lta,ltb )

      return
      end






      subroutine surgrd2( MRGRD, M1D, M2D, MNOD,
     & NGPR, NGNR, NGP1, NGP,NGN,JRMAX,KRMAX,
     & JRPER, KRPER, JRAXSA, JRAXSB,JRP,JRR,KRP,KRR,BMM,
     & IGPB,IRFAM,IAFAM,JMAX, KMAX, NGPC, JCMAX,INIC, IBCJA, IBCJB,
     & NGBCA, NGBCB, NNOD, JNOD, ETAMX, DETA, DFAR, SMU,TIM, ITSVOL,
     & JPLNA,JPLNB,JJP,JJR,R, RH, DAREA,XX,YX,ZX,XE,YE,ZE,A,B,C,F,H,U,
     & CSN1, CSN2, CSN3, GSN1, GSN2, GSN3, XSNR, YSNR, ZSNR,XB,YB,ZB,
     & CAXI, XID, AVOL, JBMAX, ICUR, JKIH, DELS, DPT, ISEG,TMP1,
     & XI,YI,ZI, EV,GNV, 
     & xsurf,ndra,ndrb,ndsa,ndsb,ndta,ndtb,nra,nrb,nsa,nsb,nta,ntb,
     & xcurve,mdra,mdrb,mdsa,mdsb,mdta,mdtb,mra,mrb,msa,msb,mta,mtb,
     & xhype,ldra,ldrb,ldsa,ldsb,ldta,ldtb,lra,lrb,lsa,lsb,lta,ltb )

#include "precis.h"
C
c*wdh
      dimension xsurf(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*)
      dimension xcurve(mdra:mdrb,mdsa:mdsb,mdta:mdtb,*)
      dimension xhype(ldra:ldrb,ldsa:ldsb,ldta:ldtb,*)


      LOGICAL HYSTEP
      CHARACTER*80 REFSFN, REFCFN, OUTGFN
      __POINTER IPXR,IPSN,IPX,IPRC
      __POINTER ISAX,ISAY,ISAZ
C
      DIMENSION NGPR(MRGRD), NGNR(MRGRD), NGP1(MRGRD), 
     &          NGP(MRGRD), NGN(MRGRD)
      DIMENSION JRMAX(MRGRD), KRMAX(MRGRD)
      DIMENSION JRPER(MRGRD), KRPER(MRGRD), JRAXSA(MRGRD), JRAXSB(MRGRD)
      DIMENSION JRP(M1D*MRGRD),JRR(M1D*MRGRD),
     &          KRP(M1D*MRGRD),KRR(M1D*MRGRD)
      DIMENSION BMM(MRGRD,6)
      DIMENSION IGPB(M1D*12*MRGRD), IRFAM(MRGRD), IAFAM(MRGRD)
C
      DIMENSION JMAX(MRGRD), KMAX(MRGRD), NGPC(MRGRD), JCMAX(MRGRD),
     &          INIC(MRGRD), IBCJA(MRGRD), IBCJB(MRGRD),
     &          NGBCA(MRGRD), NGBCB(MRGRD),
     &          NNOD(MRGRD), JNOD(MNOD,MRGRD), ETAMX(MNOD,MRGRD),
     &          DETA(MNOD,MRGRD), DFAR(MNOD,MRGRD),
     &          SMU(MRGRD), TIM(MRGRD), ITSVOL(MRGRD)
C
      DIMENSION JPLNA(3),JPLNB(3)
      DIMENSION JJP(M1D),JJR(M1D)
      DIMENSION R(M2D), RH(M1D*3), DAREA(M1D)
      DIMENSION XX(M1D),YX(M1D),ZX(M1D),XE(M1D),YE(M1D),ZE(M1D)
C
      DIMENSION A(M1D*9),B(M1D*9),C(M1D*9),F(M1D*3),H(M1D*9),U(M1D*9)
      DIMENSION CSN1(M2D), CSN2(M2D), CSN3(M2D)
      DIMENSION GSN1(M1D), GSN2(M1D), GSN3(M1D),
     &          XSNR(M1D), YSNR(M1D), ZSNR(M1D)
      DIMENSION XB(M1D*2), YB(M1D*2), ZB(M1D*2)
      DIMENSION CAXI(M1D), XID(M1D*3), AVOL(M1D), JBMAX(M1D), ICUR(M1D)
      DIMENSION JKIH(M1D,3), DELS(M1D*3), DPT(M1D), ISEG(2)
      DIMENSION TMP1(M1D,3)
C
C   Note that the dimension of XI,YI,ZI is specifically M1D*M1D, which
C   could be larger than M2D.
C
      DIMENSION XI(M1D*M1D),YI(M1D*M1D),ZI(M1D,M1D)
C
      DIMENSION EV(M2D*3), GNV(M2D*3)
C
      DIMENSION WORK(1),IWORK(1)
C

      PARAMETER (IUNIT=19)
      PARAMETER (I2=2)
C
C***********************************************************************
C
C   Initialize error logging.
C
      CALL ERRINI('surgrd',IUNIT)
C
C   Read in parameters, allocate space, and read grids.
C
      CALL INIPAR(REFSFN,REFCFN,OUTGFN,M1D,M2D,MRGRD,MNOD,
     &            NRGRD,NRCUR,NAGRD,NTOTR,NTOTC,NTOT,
     &            NGPR,NGNR,NGP1,NGP,NGN,JRMAX,KRMAX,IRFAM,
     &            JRPER,KRPER,JRAXSA,JRAXSB,
     &            JMAX,KMAX,NGPC,JCMAX,INIC,JKD,IRSTYP,IMODEL,
     &            IBCJA,IBCJB,IAFAM,NGBCA,NGBCB,
     &            NNOD,SMU,TIM,ITSVOL,
     &            JNOD,ETAMX,DETA,DFAR,IAUC,ISGRI,ISGRB)
C
      CALL INITSZ(NTOTR,NTOTC,NTOT,NRGRD,M2D,M1D,
     &            IPXR,IPSN,IPX,IPRC,WORK,IWORK)

c*wdh
c   Fill in the reference surface
      call putSurf(JRMAX(1),KRMAX(1),WORK(IPXR+NGPR(1)-1),
     & WORK(IPXR+2*NTOTR+NGPR(1)-1),WORK(IPXR+4*NTOTR+NGPR(1)-1), 
     & xsurf,ndra,ndrb,ndsa,ndsb,ndta,ndtb)
c   Fill in the reference curve
      call putCurve(JCMAX(1),WORK(IPRC+NGPC(1)-1),
     & WORK(IPRC+NTOTC+NGPC(1)-1),WORK(IPRC+2*NTOTC+NGPC(1)-1), 
     & xcurve,mdra,mdrb,mdsa,mdsb,mdta,mdtb)
c*wdh

C
c* wdh ... just load initial curve, x(j,1) = xc(j)
      CALL INIGRD(NRGRD,NRCUR,NAGRD,NTOTR,NTOTC,NTOT,IRSTYP,
     &            NGPR,NGNR,NGPC,JCMAX,NGP,JMAX,INIC,
     &            WORK(IPXR),WORK(IPXR+2*NTOTR),WORK(IPXR+4*NTOTR),
     &            WORK(IPRC),WORK(IPRC+NTOTC),WORK(IPRC+2*NTOTC),
     &            WORK(IPX),WORK(IPX+NTOT),WORK(IPX+2*NTOT))


c
c         write(27,*) jmax(1),1,1
c         write(27,*) (work(i),i=ipx,ipx+jmax(1)-1),
c     &               (work(i),i=ipx+ntot,ipx+ntot+jmax(1)-1),
c     &               (work(i),i=ipx+2*ntot,ipx+2*ntot+jmax(1)-1)
c
c         write(37,*) 2
c         write(37,*) (jrmax(n),krmax(n),1,n=1,nrgrd)
c         do 201 n=1,nrgrd
c          is1 = ipxr+ngpr(n)-1
c          is2 = is1+2*ntotr
c          is3 = is1+4*ntotr
c          write(37,*) (work(i),i=is1,is1+ngnr(n)-1),
c     >                (work(i),i=is2,is2+ngnr(n)-1),
c     >                (work(i),i=is3,is3+ngnr(n)-1)
c 201     continue
c
C
C   Initialize arrays for reference surface and do some bc checks.
C
c*wdh   only checks BC's if irstyp==2      
      CALL INITIA(M1D,NRGRD,NAGRD,NTOTR,NGPR,NGNR,NGP1,
     &            JRMAX,KRMAX,IBCJA,IBCJB,NGBCA,NGBCB,
     &            JRPER,KRPER,JRAXSA,JRAXSB,IRSTYP,
     &            JRP,JRR,KRP,KRR,
     &            WORK(IPXR),WORK(IPXR+2*NTOTR),WORK(IPXR+4*NTOTR),
     &            WORK(IPSN),WORK(IPSN+NTOTR),WORK(IPSN+2*NTOTR),
     &            WORK(IPSN+3*NTOTR),WORK(IPSN+4*NTOTR),
     &            WORK(IPSN+5*NTOTR),
     &            IGPB,IRFAM,BMM)
C
C   March equations for each active grid.
C
      DO 100 N = 1,NAGRD
C
C   Initialize arrays for each active grid.
C
         ISAM = NGP(N) - 1
         ISAX = IPX + ISAM
         ISAY = ISAX + NTOT
         ISAZ = ISAX + 2*NTOT
C
         CALL INITS (M1D,JKD,NRGRD,NRCUR,NTOTR,NTOTC,N,JMAX(N),KMAX(N),
     &               NGPR,NGNR,NGP1,NGPC,JRMAX,KRMAX,JCMAX,
     &               J1,J2,JPER,HYSTEP,IRSTYP,IMODEL,
     &               IBCJA(N),IBCJB(N),IAFAM(N),NGBCA(N),NGBCB(N),
     &               JRPER,KRPER,JRAXSA,JRAXSB,
     &               JPLNA,JPLNB,JJP,JJR,JRP,JRR,KRP,KRR,NNOD(N),
     &               JNOD(1,N),ETAMX(1,N),DETA(1,N),DFAR(1,N),DAREA,
     &               XI,YI,ZI,WORK(ISAX),WORK(ISAY),WORK(ISAZ),
     &               WORK(IPXR),WORK(IPXR+2*NTOTR),WORK(IPXR+4*NTOTR),
     &               WORK(IPRC),WORK(IPRC+NTOTC),WORK(IPRC+2*NTOTC),
     &               R,RH,TMP1,WORK(IPSN+3*NTOTR),
     &               WORK(IPSN+4*NTOTR),WORK(IPSN+5*NTOTR),
     &               IGPB,IRFAM,XB,YB,ZB,ISEG,
     &               GNV,GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,
     &               JKIH,DELS,JBMAX,ICUR)
C
C   March equations in eta direction for each grid.
C
         DO 10 K = 2,KMAX(N)
C
C   Hyperbolic marching step.
C
            IF (HYSTEP) THEN
               ISSTAT = 1
               CALL STEP  (M1D,JKD,JMAX(N),KMAX(N),JPER,
     &                     IBCJA(N),IBCJB(N),
     &                     JPLNA,JPLNB,JJP,JJR,J1,J2,K,
     &                     SMU(N),TIM(N),ITSVOL(N),
     &                     WORK(ISAX),WORK(ISAY),WORK(ISAZ),R,RH,DAREA,
     &                     XX,YX,ZX,XE,YE,ZE,A,B,C,F,H,U,
     &                     CAXI,XID,AVOL,TMP1,CSN1,CSN2,CSN3,
     &                     GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,XB,YB,ZB,
     &                     ISEG,EV)
C
C   Project onto surface.
C
               IF (IAUC.NE.-1) THEN
C
                  CALL SURFE (M1D,NRGRD,NTOTR,JMAX(N),KMAX(N),
     &                        NGPR,NGNR,NGP1,JRMAX,KRMAX,J1,J2,K,
     &                        IRSTYP,IMODEL,JRPER,KRPER,JRAXSA,JRAXSB,
     &                        IBCJA(N),IBCJB(N),
     &                        IAFAM(N),JPER,JRP,JRR,KRP,KRR,
     &                        WORK(ISAX),WORK(ISAY),WORK(ISAZ),R,
     &                        WORK(IPXR),WORK(IPXR+2*NTOTR),
     &                        WORK(IPXR+4*NTOTR),WORK(IPSN),
     &                        WORK(IPSN+NTOTR),WORK(IPSN+2*NTOTR),
     &                        IGPB,IRFAM,GSN1,GSN2,GSN3,GNV,
     &                        XSNR,YSNR,ZSNR,JKIH,DELS,ISSTAT)
C
C   Make sure boundary points satisfy BC and are on the surface,
C   and do corrector step for fixed boundaries.
C
                  CALL POSTBC(M1D,JMAX(N),KMAX(N),JKD,NRGRD,NTOTR,
     &                        NGPR,NGNR,NGP1,JRMAX,KRMAX,
     &                        IRSTYP,IBCJA(N),IBCJB(N),JRPER,KRPER,
     &                        JRAXSA,JRAXSB,
     &                        JPLNA,JPLNB,JRP,JRR,KRP,KRR,J1,J2,K,
     &                        WORK(ISAX),WORK(ISAY),WORK(ISAZ),
     &                        WORK(IPXR),WORK(IPXR+2*NTOTR),
     &                        WORK(IPXR+4*NTOTR),WORK(IPSN),
     &                        WORK(IPSN+NTOTR),WORK(IPSN+2*NTOTR),
     &                        R,IGPB,JKIH,IRFAM,IAFAM(N),
     &                        GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,
     &                        XB,YB,ZB,ISEG)
               ENDIF
C
C   Algebraic marching step.
C
            ELSE
               CALL STEPI (JMAX(N),KMAX(N),JKD,IRSTYP,K,R,
     &                     WORK(ISAX),WORK(ISAY),WORK(ISAZ),
     &                     JBMAX,ICUR,XI,YI,ZI)
            ENDIF
   10       CONTINUE
#ifdef DEBUG
C
C   Write fake Q files of (1) active grid eta metrics and (2) active
C   grid (interpolated) surface normals.
C
         CALL WRINOR(NAGRD,N,JMAX,KMAX,JMAX(N),KMAX(N),EV,GNV,
     &               'mets.dat','norms.dat')
#endif
  100    CONTINUE
#ifdef DEBUG
C
C   Write out the two-layer version of the reference grids for debugging
C   purposes.
C
      IF (IRSTYP.EQ.1) THEN
       CALL WRIREF(NRGRD,NTOTR,NGPR,NGNR,JRMAX,KRMAX,I2,'ref2.dat',
     &             WORK(IPXR),WORK(IPXR+2*NTOTR),WORK(IPXR+4*NTOTR))
      ENDIF
#endif
C
C   Write grid in PLOT3D format and PLOT3D command files for viewing.
C
c*wdh      CALL WRGRID(IAUC,NAGRD,NAGRDN,NTOT,NGP,NGN,JMAX,KMAX,
c*wdh     &            JPER,TMP1(1,1),TMP1(1,2),ISGRI,ICUR,
c*wdh     &            WORK(IPX),WORK(IPX+NTOT),WORK(IPX+2*NTOT))

c*wdh
c   Save the surface grid in the array xyz
      call saveHype(JMAX(1),KMAX(1),WORK(IPX+NGP(1)-1),
     & WORK(IPX+NTOT+NGP(1)-1),WORK(IPX+2*NTOT+NGP(1)-1), 
     & xhype,ldra,ldrb,ldsa,ldsb,ldta,ldtb)
c*wdh

c*wdh      CALL WRPCOM(REFSFN,REFCFN,OUTGFN,NRGRD,NRCUR,NAGRD,NAGRDN,
c*wdh     &            NTOT,NGP,JMAX,KMAX,ISGRI,ISGRB,IRSTYP,
c*wdh     &            WORK(IPX),WORK(IPX+NTOT),WORK(IPX+2*NTOT))
C
C   End error loging.
C
      CALL ERREND
C
C
      return
      END

      subroutine putSurf(jrmax,krmax,xr,yr,zr,
     & xsurf,ndra,ndrb,ndsa,ndsb,ndta,ndtb)
#include "precis.h"
      
      dimension xr(jrmax,krmax),yr(jrmax,krmax),zr(jrmax,krmax)
      dimension xsurf(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*)
  
      write(*,*) 'putSurf: jrmax = ',jrmax,' krmax=',krmax
      do j=1,jrmax  
        do k=1,krmax
          xr(j,k)=xsurf(j-1+ndra,k-1+ndsa,ndta,1)
          yr(j,k)=xsurf(j-1+ndra,k-1+ndsa,ndta,2)
          zr(j,k)=xsurf(j-1+ndra,k-1+ndsa,ndta,3)
        end do
      end do
      return
      end

      subroutine putCurve(jcmax,xc,yc,zc,
     & xcurve,ndra,ndrb,ndsa,ndsb,ndta,ndtb)
c   Fill in the reference curve
#include "precis.h"
      
      dimension xc(jcmax),yc(jcmax),zc(jcmax)
      dimension xcurve(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*)
  
      write(*,*) 'putCurve: jcmax = ',jcmax
      do j=1,jcmax  
        xc(j)=xcurve(j-1+ndra,ndsa,ndta,1)
        yc(j)=xcurve(j-1+ndra,ndsa,ndta,2)
        zc(j)=xcurve(j-1+ndra,ndsa,ndta,3)
      end do
      return
      end

      subroutine saveHype(jmax,kmax,x,y,z, 
     &        xhype,ldra,ldrb,ldsa,ldsb,ldta,ldtb )
#include "precis.h"
      
      dimension x(jmax,kmax),y(jmax,kmax),z(jmax,kmax)
      dimension xhype(ldra:ldrb,ldsa:ldsb,ldta:ldtb,*)
  
      write(*,*) 'saveSurf: jmax = ',jmax,' kmax=',kmax
      do j=1,jmax  
        do k=1,kmax
          xhype(j-1+ldra,k-1+ldsa,ldta,1)=x(j,k)
          xhype(j-1+ldra,k-1+ldsa,ldta,2)=y(j,k)
          xhype(j-1+ldra,k-1+ldsa,ldta,3)=z(j,k)
        end do
      end do
      return
      end

C***********************************************************************
      SUBROUTINE AMATRX(JMAX,JKD,JS,JE,GSN1,GSN2,GSN3,XE,YE,ZE,A)
C
#include "precis.h"
C
      PARAMETER (ZERO=0.0)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX)
      DIMENSION XE(JMAX), YE(JMAX), ZE(JMAX), A(JKD,3,3)
C
C    Coefficient matrix to xi derivative
C
      DO 10 J = JS,JE
       A(J,1,1) = XE(J)
       A(J,1,2) = YE(J)
       A(J,1,3) = ZE(J)
       A(J,2,1) = (GSN3(J)*YE(J) - GSN2(J)*ZE(J))
       A(J,2,2) = (GSN1(J)*ZE(J) - GSN3(J)*XE(J))
       A(J,2,3) = (GSN2(J)*XE(J) - GSN1(J)*YE(J))
       A(J,3,1) = ZERO
       A(J,3,2) = ZERO
       A(J,3,3) = ZERO
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE ANGLES(JMAX,KMAX,JS,JE,K,JJP,JJR,GSN1,GSN2,GSN3,
     &                  X,Y,Z,CAXI)
C
#include "precis.h"
C
      PARAMETER (ONE=1.0)
      DIMENSION JJP(JMAX),JJR(JMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), CAXI(JMAX)
C
C    Compute cosine of half angle between two neighbors
C
      DO 10 J=JS,JE
       JP = JJP(J)
       JR = JJR(J)
       XJP = X(JP,K)-X(J,K)
       YJP = Y(JP,K)-Y(J,K)
       ZJP = Z(JP,K)-Z(J,K)
       XJR = X(JR,K)-X(J,K)
       YJR = Y(JR,K)-Y(J,K)
       ZJR = Z(JR,K)-Z(J,K)
       VJP = ONE/SQRT( XJP*XJP + YJP*YJP + ZJP*ZJP )
       VJR = ONE/SQRT( XJR*XJR + YJR*YJR + ZJR*ZJR )
       XB  = XJP*VJP - XJR*VJR
       YB  = YJP*VJP - YJR*VJR
       ZB  = ZJP*VJP - ZJR*VJR
       XA  = GSN2(J)*ZB - GSN3(J)*YB
       YA  = GSN3(J)*XB - GSN1(J)*ZB
       ZA  = GSN1(J)*YB - GSN2(J)*XB
       DA  = VJP/SQRT(XA*XA + YA*YA + ZA*ZA)
       CAXI(J) = (XJP*XA + YJP*YA + ZJP*ZA)*DA
 10   CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE AVGNOR(NJ,NK,J,JN,XNR,YNR,ZNR)
C
C   Average normals at axis points.
C
#include "precis.h"
C
      PARAMETER (ZERO=0.0, ONE=1.0)
      DIMENSION XNR(NJ,NK), YNR(NJ,NK), ZNR(NJ,NK)
C
C
       XN        = ZERO
       YN        = ZERO
       ZN        = ZERO
       DO 10 K = 1,NK
          XN        = XN + XNR(JN,K)
          YN        = YN + YNR(JN,K)
          ZN        = ZN + ZNR(JN,K)
   10     CONTINUE
       AVK       = ONE/FLOAT(NK)
       XN        = XN*AVK
       YN        = YN*AVK
       ZN        = ZN*AVK
       DO 20 K = 1,NK
          XNR(J,K)  = XN
          YNR(J,K)  = YN
          ZNR(J,K)  = ZN
   20     CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BCMAT(JMAX,KMAX,JKD,IBCJA,IBCJB,J1,J2,K,JPLNA,JPLNB,
     &                 JFLAGA,JFLAGB,EXTJA,EXTJB,X,Y,Z,A,B,C,F,
     &                 XBP,YBP,ZBP)
C
#include "precis.h"
C
      PARAMETER (ZERO=0.0, ONE=1.0)
      DIMENSION JPLNA(3), JPLNB(3)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION A(JKD,3,3), B(JKD,3,3), C(JKD,3,3), F(JKD,3)
      DIMENSION XBP(2), YBP(2), ZBP(2)
C
C    Set up end point matrices for implicit BC in J
C    Symmetry plane is treated as const plane.
C    Old code needs special BTRI that has three contiguous
C    elements in first and last rows.
C
      J = J1
C
C      JFLAGA = 0
C
      JFLAGA = 1
      EXTJAM = -ONE + EXTJA
      DO 20 N =1,3
      DO 21 M =1,3
       A(J,N,M) = ZERO
       B(J,N,M) = ZERO
       C(J,N,M) = ZERO
 21   CONTINUE
       F(J,N) = ZERO
       B(J,N,N) = ONE
C
C       C(J,N,N) = -ONE*JPLNA(N)
C
       C(J,N,N) = EXTJAM*JPLNA(N)
       A(J,N,N) = EXTJA*JPLNA(N)
 20   CONTINUE
      IF ((IBCJA.GE.11).AND.(IBCJA.LE.20)) THEN
       C(J,1,1) = ZERO
       C(J,2,2) = ZERO
       C(J,3,3) = ZERO
       F(J,1) = XBP(1) - X(J,K-1)
       F(J,2) = YBP(1) - Y(J,K-1)
       F(J,3) = ZBP(1) - Z(J,K-1)
      ENDIF
C
      J = J2
C
C      JFLAGB = 0
C
      JFLAGB = 1
      EXTJBM = -ONE + EXTJB
      DO 22 N =1,3
      DO 23 M =1,3
       A(J,N,M) = ZERO
       B(J,N,M) = ZERO
       C(J,N,M) = ZERO
 23   CONTINUE
       F(J,N) = ZERO
       B(J,N,N) = ONE
C
C       A(J,N,N) = -ONE*JPLNB(N)
C
       A(J,N,N) = EXTJBM*JPLNB(N)
       C(J,N,N) = EXTJB*JPLNB(N)
 22   CONTINUE
      IF ((IBCJB.GE.11).AND.(IBCJB.LE.20)) THEN
       A(J,1,1) = ZERO
       A(J,2,2) = ZERO
       A(J,3,3) = ZERO
       F(J,1) = XBP(2) - X(J,K-1)
       F(J,2) = YBP(2) - Y(J,K-1)
       F(J,3) = ZBP(2) - Z(J,K-1)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BINV(JMAX,JKD,JS,JE,XX,YX,ZX,GSN1,GSN2,GSN3,U)
C
#include "precis.h"
C
      DIMENSION XX(JMAX), YX(JMAX), ZX(JMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX), U(JKD,3,3)
C
C    Compute inverse of B coef matrix
C
      DO 10 J = JS,JE
       RXRX = XX(J)**2 + YX(J)**2 + ZX(J)**2
       SNRX = GSN1(J)*XX(J) + GSN2(J)*YX(J) + GSN3(J)*ZX(J)
       RD = 1./( RXRX - SNRX**2 )
       U(J,1,1) =  (XX(J) - GSN1(J)*SNRX)*RD
       U(J,2,1) =  (YX(J) - GSN2(J)*SNRX)*RD
       U(J,3,1) =  (ZX(J) - GSN3(J)*SNRX)*RD
       U(J,1,2) =  (GSN2(J)*ZX(J) - GSN3(J)*YX(J))*RD
       U(J,2,2) =  (GSN3(J)*XX(J) - GSN1(J)*ZX(J))*RD
       U(J,3,2) =  (GSN1(J)*YX(J) - GSN2(J)*XX(J))*RD
       U(J,1,3) =  (GSN1(J)*RXRX -XX(J)*SNRX)*RD
       U(J,2,3) =  (GSN2(J)*RXRX -YX(J)*SNRX)*RD
       U(J,3,3) =  (GSN3(J)*RXRX -ZX(J)*SNRX)*RD
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BINVA(JKD,JS,JE,A,U,H)
C
#include "precis.h"
C
      DIMENSION A(JKD,3,3), U(JKD,3,3), H(JKD,3,3)
C
C    Form B inverse times A coef matrix
C
      DO 12 N=1,3
      DO 12 M=1,3
       DO 10 J=JS,JE
        H(J,N,M)=U(J,N,1)*A(J,1,M)+U(J,N,2)*A(J,2,M)+U(J,N,3)*A(J,3,M)
 10    CONTINUE
 12   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BTRI(JKD,IL,IU,NFLG1,NFLG2,A,B,C,F,H,U)
C
#include "precis.h"
C
      PARAMETER (I1=1)
      DIMENSION A(JKD,3,3),B(JKD,3,3),C(JKD,3,3),F(JKD,3)
      DIMENSION H(JKD,3,3),U(JKD,3,3)
      DIMENSION G(3,3),G1(3,3),G2(3,3),GH(3,3),GF(3,3),CSAVE(3,3)
      COMMON/LUD/ L11,L21,L22,L31,L32,L33,V1,V2,V3,U12,U13,U23
      __REAL       L11,L21,L22,L31,L32,L33
C
C  INVERSION OF BLOCK TRIDIAGONAL... A,B,C ARE 3*3 BLOCKS
C  F IS FORCING FUNCTION AND SOLUTION IS OUTPUT IN F
C  BLOCK INVERSIONS USE NONPIVOTED LU DECOMPOSITION
C  IL AND IU ARE STARTING AND FINISHING INDICES
C  THIS VERSION CAN USE NONZERO A(IL,N,M) OR C(IU,N,M) IN THE
C  (IS,3) AND (IU,IU-2) BLOCK LOCATIONS IFF NFLG1 OR NFLG2 = 1
C
      IS = IL +1
      IF( NFLG1 .NE. I1) NFLG1 = 0
      IF( NFLG2 .NE. I1) NFLG2 = 0
      DO 9 M =1,3
      DO 9 N =1,3
      G1(N,M) = NFLG1*A(IL,N,M)
      G2(N,M) = NFLG2*C(IU,N,M)
    9 CONTINUE
C
C  PRELIMINARY FILLS
C
      I = IL
      DO 11 M = 1,3
      DO 11 N = 1,3
   11 G(N,M)=B(I,N,M)
      CALL LUDEC(G)
C
      D1 = V1*F(I,1)
      D2 = V2*( F(I,2) - L21*D1)
      D3 = V3*( F(I,3) - L31*D1 - L32*D2)
      F(I,3) = D3
      F(I,2) = D2          - U23*F(I,3)
      F(I,1) = D1          - U13*F(I,3) - U12*F(I,2)
C
      DO 12 M = 1,3
      D1 = V1*C(I,1,M)
      D2 = V2*( C(I,2,M) - L21*D1)
      D3 = V3*( C(I,3,M) - L31*D1 - L32*D2)
      U(I,3,M) = D3
      U(I,2,M) = D2          - U23*U(I,3,M)
      U(I,1,M) = D1          - U13*U(I,3,M) - U12*U(I,2,M)
      D1 = V1* G1(1,M)
      D2 = V2*(G1(2,M) - L21*D1)
      D3 = V3*(G1(3,M) - L31*D1 - L32*D2)
      GH(3,M) = D3
      GH(2,M) = D2          - U23*GH(3,M)
      GH(1,M) = D1          - U13*GH(3,M) - U12*GH(2,M)
   12 CONTINUE
C
C   OVERLOAD C AT IL +1 TO MAINTAIN GENERAL FORM, SAVE IN CSAVE
C
      I = IS
      DO 10 N = 1,3
      DO 10 M = 1,3
      CSAVE(N,M) = C(I,N,M)
      C(I,N,M) = C(I,N,M) -A(I,N,1)*GH(1,M) -A(I,N,2)*GH(2,M)
     &    -A(I,N,3)*GH(3,M)
   10 CONTINUE
C
C   FORWARD RECURSIVE SWEEP
C
      IUR = IU - 1
      DO 13 I=IS,IUR
      IR = I -1
      DO 14 N = 1,3
      F(I,N) = F(I,N) -A(I,N,1)*F(IR,1) -A(I,N,2)*F(IR,2)
     &     -A(I,N,3)*F(IR,3)
      DO 14 M = 1,3
      G(N,M) = B(I,N,M) -A(I,N,1)*U(IR,1,M) -A(I,N,2)*U(IR,2,M)
     &    -A(I,N,3)*U(IR,3,M)
   14 CONTINUE
C
      CALL LUDEC(G)
C
      D1 = V1*F(I,1)
      D2 = V2*( F(I,2) - L21*D1)
      D3 = V3*( F(I,3) - L31*D1 - L32*D2)
      F(I,3) = D3
      F(I,2) = D2          - U23*F(I,3)
      F(I,1) = D1          - U13*F(I,3) - U12*F(I,2)
C
      DO 15 M = 1,3
      D1 = V1*C(I,1,M)
      D2 = V2*( C(I,2,M) - L21*D1)
      D3 = V3*( C(I,3,M) - L31*D1 - L32*D2)
      U(I,3,M) = D3
      U(I,2,M) = D2          - U23*U(I,3,M)
      U(I,1,M) = D1          - U13*U(I,3,M) - U12*U(I,2,M)
   15 CONTINUE
   13 CONTINUE
C
C  FORM SPECIAL MATRICES AT IU AND RETURN C OF IL+1
C
      I = IU
      DO 18 N = 1,3
      DO 18 M = 1,3
      GF(N,M) = A(I,N,M) -G2(N,1)*U(I-2,1,M)-G2(N,2)*U(I-2,2,M)
     &    -G2(N,3)*U(I-2,3,M)
      C(IS,N,M) = CSAVE(N,M)
   18 CONTINUE
C
C   FORM G AND SPECIAL F AT I = IU
C
      DO 19 N = 1,3
      F(I,N) = F(I,N) - GF(N,1)*F(I-1,1) -GF(N,2)*F(I-1,2)
     &                 -GF(N,3)*F(I-1,3) -G2(N,1)*F(I-2,1)
     &                 -G2(N,2)*F(I-2,2) -G2(N,3)*F(I-2,3)
      DO 19 M = 1,3
      G(N,M) = B(I,N,M) -GF(N,1)*U(I-1,1,M)-GF(N,2)*U(I-1,2,M)
     &    -GF(N,3)*U(I-1,3,M)
   19 CONTINUE
C
      CALL LUDEC(G)
C
      D1 = V1*F(I,1)
      D2 = V2*( F(I,2) - L21*D1)
      D3 = V3*( F(I,3) - L31*D1 - L32*D2)
      F(I,3) = D3
      F(I,2) = D2          - U23*F(I,3)
      F(I,1) = D1          - U13*F(I,3) - U12*F(I,2)
C
C   BACK SUBSTITUTION
C
      IT = IL + IU
      IUR = IU - 1
      DO 21 II = IS,IUR
      I = IT - II
      IP = I+1
      DO 22 N = 1,3
      F(I,N) = F(I,N) - U(I ,N,1)*F(IP,1) - U(I ,N,2)*F(IP,2)
     &     -U(I ,N,3)*F(IP,3)
   22 CONTINUE
   21 CONTINUE
C
      I =IL
      IP = I +1
      IPP = I +2
      DO 24 N = 1,3
      F(I,N) = F(I,N) - U(I ,N,1)*F(IP,1) - U(I ,N,2)*F(IP,2)
     &     -U(I ,N,3)*F(IP,3) -GH(N,1)*F(IPP,1)
     &     -GH(N,2)*F(IPP,2) -GH(N,3)*F(IPP,3)
   24 CONTINUE
      RETURN
      END
C***********************************************************************
      SUBROUTINE BTRIP(JKD,IL,IU,A,B,C,F,H,U)
C
#include "precis.h"
C
      DIMENSION A(JKD,3,3),B(JKD,3,3),C(JKD,3,3),F(JKD,3)
      DIMENSION H(JKD,3,3),U(JKD,3,3)
      DIMENSION G(3,3),E(3,3),Q(3,3),FN(3),S(3,3)
      COMMON/LUD/ L11,L21,L22,L31,L32,L33,V1,V2,V3,U12,U13,U23
      __REAL       L11,L21,L22,L31,L32,L33
C
C  INVERSION ALGORITHM FOR BLOCK PERIODIC TRIDIAGONAL
C  THIS VERSION DOES NOT OVERLOAD THE BLOCK MATRICES
C  A,B,C, ARE 3*3 BLOCKS... F IS THE FORCING FUNCTION AND THE
C  SOLUTION IS OUTPUT IN F... IL AND IU ARE STARTING AND FINISHING
C  INDICES... BLOCK INVERSIONS USE NON-PIVOTED LU DECOMPOSITION
C
      IS = IL +1
      IM = IU -1
      IN = IU -2
      MN = 3
C
C  FORWARD SWEEP
C
      I = IL
      DO 10 N=1,MN
      DO 10 M=1,MN
      E(N,M) = C(IU,N,M)
   10 G(N,M) = B(I,N,M)
      CALL LUDEC(G)
      D1 = V1*F(I,1)
      D2 = V2*( F(I,2) - L21*D1)
      D3 = V3*( F(I,3) - L31*D1 - L32*D2)
      F(I,3) = D3
      F(I,2) = D2          - U23*F(I,3)
      F(I,1) = D1          - U13*F(I,3) - U12*F(I,2)
      DO 11 M=1,MN
      D1 = V1*C(I,1,M)
      D2 = V2*( C(I,2,M) - L21*D1)
      D3 = V3*( C(I,3,M) - L31*D1 - L32*D2)
      U(I,3,M) = D3
      U(I,2,M) = D2          - U23*U(I,3,M)
      U(I,1,M) = D1          - U13*U(I,3,M) - U12*U(I,2,M)
C
      D1 = V1*A(I,1,M)
      D2 = V2*( A(I,2,M) - L21*D1)
      D3 = V3*( A(I,3,M) - L31*D1 - L32*D2)
      H(I,3,M) = D3
      H(I,2,M) = D2          - U23*H(I,3,M)
      H(I,1,M) = D1          - U13*H(I,3,M) - U12*H(I,2,M)
   11 CONTINUE
      DO 13 N=1,MN
      FN(N) = F(IU,N) -E(N,1)*F(I,1) -E(N,2)*F(I,2) -E(N,3)*F(I,3)
      DO 13 M=1,MN
      Q(N,M) = B(IU,N,M) -E(N,1)*H(I,1,M) -E(N,2)*H(I,2,M)
     &  -E(N,3)*H(I,3,M)
   13 CONTINUE
C
C
      DO 14 I=IS,IN
      L = I-1
      DO 15 N=1,MN
      F(I,N) = F(I,N) - A(I,N,1)*F(L,1) -A(I,N,2)*F(L,2)
     &  -A(I,N,3)*F(L,3)
      DO 15 M=1,MN
      G(N,M) = B(I,N,M) -A(I,N,1)*U(L,1,M) -A(I,N,2)*U(L,2,M)
     &  -A(I,N,3)*U(L,3,M)
      S(N,M) = -E(N,1)*U(L,1,M) -E(N,2)*U(L,2,M) -E(N,3)*U(L,3,M)
      H(I,N,M) = -A(I,N,1)*H(L,1,M) -A(I,N,2)*H(L,2,M)
     &  -A(I,N,3)*H(L,3,M)
   15 CONTINUE
      DO 7 N=1,MN
      DO 7 M=1,MN
    7 E(N,M) = S(N,M)
      CALL LUDEC(G)
      D1 = V1*F(I,1)
      D2 = V2*( F(I,2) - L21*D1)
      D3 = V3*( F(I,3) - L31*D1 - L32*D2)
      F(I,3) = D3
      F(I,2) = D2          - U23*F(I,3)
      F(I,1) = D1          - U13*F(I,3) - U12*F(I,2)
      DO 16 M=1,MN
      D1 = V1*C(I,1,M)
      D2 = V2*( C(I,2,M) - L21*D1)
      D3 = V3*( C(I,3,M) - L31*D1 - L32*D2)
      U(I,3,M) = D3
      U(I,2,M) = D2          - U23*U(I,3,M)
      U(I,1,M) = D1          - U13*U(I,3,M) - U12*U(I,2,M)
C
      D1 = V1*H(I,1,M)
      D2 = V2*( H(I,2,M) - L21*D1)
      D3 = V3*( H(I,3,M) - L31*D1 - L32*D2)
      H(I,3,M) = D3
      H(I,2,M) = D2          - U23*H(I,3,M)
      H(I,1,M) = D1          - U13*H(I,3,M) - U12*H(I,2,M)
   16 CONTINUE
      DO 12 N=1,MN
      FN(N) = FN(N)   -E(N,1)*F(I,1) -E(N,2)*F(I,2) -E(N,3)*F(I,3)
      DO 12 M=1,MN
      Q(N,M) = Q(N,M)    -E(N,1)*H(I,1,M) -E(N,2)*H(I,2,M)
     &  -E(N,3)*H(I,3,M)
   12 CONTINUE
   14 CONTINUE
C
C
      I = IM
      L = I-1
      DO 17 N=1,MN
      F(I,N) = F(I,N) - A(I,N,1)*F(L,1) -A(I,N,2)*F(L,2)
     &  -A(I,N,3)*F(L,3)
      DO 17 M=1,MN
      G(N,M) = B(I,N,M) -A(I,N,1)*U(L,1,M) -A(I,N,2)*U(L,2,M)
     &  -A(I,N,3)*U(L,3,M)
      S(N,M) = -E(N,1)*U(L,1,M) -E(N,2)*U(L,2,M) -E(N,3)*U(L,3,M)
     &                    + A(IU,N,M)
      H(I,N,M) = -A(I,N,1)*H(L,1,M) -A(I,N,2)*H(L,2,M)
     &  -A(I,N,3)*H(L,3,M)                       +C(I,N,M)
   17 CONTINUE
      DO 8 N=1,MN
      DO 8 M=1,MN
    8 E(N,M) = S(N,M)
      CALL LUDEC(G)
      D1 = V1*F(I,1)
      D2 = V2*( F(I,2) - L21*D1)
      D3 = V3*( F(I,3) - L31*D1 - L32*D2)
      F(I,3) = D3
      F(I,2) = D2          - U23*F(I,3)
      F(I,1) = D1          - U13*F(I,3) - U12*F(I,2)
      DO 18 M=1,MN
      D1 = V1*H(I,1,M)
      D2 = V2*( H(I,2,M) - L21*D1)
      D3 = V3*( H(I,3,M) - L31*D1 - L32*D2)
      H(I,3,M) = D3
      H(I,2,M) = D2          - U23*H(I,3,M)
      H(I,1,M) = D1          - U13*H(I,3,M) - U12*H(I,2,M)
   18 CONTINUE
      DO 19 N=1,MN
      FN(N) = FN(N)   -E(N,1)*F(I,1) -E(N,2)*F(I,2) -E(N,3)*F(I,3)
      DO 19 M=1,MN
      Q(N,M) = Q(N,M)    -E(N,1)*H(I,1,M) -E(N,2)*H(I,2,M)
     &  -E(N,3)*H(I,3,M)
   19 CONTINUE
C
      I = IU
      CALL LUDEC(Q)
      D1 = V1*FN( 1)
      D2 = V2*( FN( 2) - L21*D1)
      D3 = V3*( FN( 3) - L31*D1 - L32*D2)
      F(I,3) = D3
      F(I,2) = D2          - U23*F(I,3)
      F(I,1) = D1          - U13*F(I,3) - U12*F(I,2)
C
C  BACKWARD SWEEP
C
      K = IU
      I = IM
      DO 20 N=1,MN
      F(I,N) = F(I,N) -H(I,N,1)*F(K,1) -H(I,N,2)*F(K,2)
     &  -H(I,N,3)*F(K,3)
   20 CONTINUE
      IT = IL + IN
      DO 22 J=IL,IN
      I = IT - J
      L = I+1
      DO 21 N=1,MN
      F(I,N) = F(I,N) -U(I,N,1)*F(L,1) -U(I,N,2)*F(L,2)
     &  -U(I,N,3)*F(L,3)                  -H(I,N,1)*F(K,1)
     & -H(I,N,2)*F(K,2)-H(I,N,3)*F(K,3)
   21 CONTINUE
   22 CONTINUE
      RETURN
      END
C***********************************************************************
      SUBROUTINE CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &                  XP,YP,ZP,NS,JS,KS,A,B,
     &                  IGPB,IRES,ICSTAT)
C
#include "precis.h"
C
C   WMC Jan 93
C   This subroutine is derived from the CELL3 subroutine in the
C   PARTICLES module by P.G. Buning. Modifications were made to
C   (1) treat mesh with only one layer of cells in the L direction,
C   (2) search on surface defined by multiple grids.
C
C   Find grid number NS, cell indices JS,KS, delxi (=A) and delet (=B)
C   for point (xp,yp,zp).  If the point is off this grid, use IGPB array
C   to try a neighboring grid.  If we are unable to find the point,
C   return an estimate and set ICSTAT appropriately.
C
C     IRES   = 0  Do not restrict A,B to range [0,1]
C            = 1  Restrict A,B to range [0,1]
C     ICSTAT = 0  Point converged
C            = 1  Unable to find the point without going out of the
C                 computational domain or active subset.  The computational
C                 point returned indicates the direction to look...
C
      DIMENSION NGPR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
C
C   Number of close points we're willing to consider.
C
      PARAMETER (MSAVE=1)
      DIMENSION JSAVE(MSAVE),KSAVE(MSAVE),NGSAVE(MSAVE)
      LOGICAL FIND
      PARAMETER (IPRO=0)
      PARAMETER (I0=0,I1=1)
      PARAMETER (ZERO=0.,ONE=1.)
C
C
      NFIRST = 0
      FIND   = .FALSE.
C
   10 CONTINUE
C
C   Mark grids we have tried with a negative IRFAM.  (Reset these before
C   we leave.)
C
      IRFAM(NS) = -IRFAM(NS)
C
C   Try to find the computational cell starting from the current
C   (NS,JS,KS).
C
#ifdef DEBUG
      WRITE(*,*) 'Starting cell index ',JS,KS,NS
#endif
      IS = NGPR(NS)
      CALL CELLPR(XR(IS),YR(IS),ZR(IS),JRMAX(NS),KRMAX(NS),
     &            XP,YP,ZP,IPRO,JS,KS,A,B,ISTAT)
C
C   Quick check for hitting a periodic or axis boundary
C   (not handled in CELLPR).
C
      IF (ISTAT.EQ.1) THEN
         JNEW   = JS
         KNEW   = KS
C
C   Hit the periodic boundary in J.
C
         IF (JRPER(NS).EQ.1) THEN
            IF      (JS.EQ.1           .AND. A.LT.0.) THEN
               JNEW   = JRMAX(NS)-1
            ELSE IF (JS.EQ.JRMAX(NS)-1 .AND. A.GT.1.) THEN
               JNEW   = 1
            ENDIF
         ENDIF
C
C   Hit the periodic boundary in K.
C
         IF (KRPER(NS).EQ.1) THEN
            IF      (KS.EQ.1           .AND. B.LT.0.) THEN
               KNEW   = KRMAX(NS)-1
            ELSE IF (KS.EQ.KRMAX(NS)-1 .AND. B.GT.1.) THEN
               KNEW   = 1
            ENDIF
         ENDIF
C
C   Hit the axis at J=1.
C
         IF (JRAXSA(NS).EQ.1) THEN
            IF      (JS.EQ.1           .AND. A.LT.0.) THEN
               K2     = (KRMAX(NS)+1)/2
               IF (KS.LT.K2) THEN
                  KNEW   = MIN(KS+K2,KRMAX(NS)-I1)
               ELSE
                  KNEW   = MAX(KS-K2,I1)
               ENDIF
            ENDIF
         ENDIF
C
C   Hit the axis at JMAX.
C
         IF (JRAXSB(NS).EQ.1) THEN
            IF      (JS.EQ.JRMAX(NS)-1 .AND. A.GT.1.) THEN
               K2     = (KRMAX(NS)+1)/2
               IF (KS.LT.K2) THEN
                  KNEW   = MIN(KS+K2,KRMAX(NS)-I1)
               ELSE
                  KNEW   = MAX(KS-K2,I1)
               ENDIF
            ENDIF
         ENDIF
C
C   If we have a new point, go back and try again (once).
C
         IF (JNEW.NE.JS .OR. KNEW.NE.KS) THEN
            JS     = JNEW
            KS     = KNEW
#ifdef DEBUG
      WRITE(*,*) 'Try new point based on topology'
      WRITE(*,*) 'Starting cell index ',JS,KS,NS
#endif
            CALL CELLPR(XR(IS),YR(IS),ZR(IS),JRMAX(NS),KRMAX(NS),
     &                  XP,YP,ZP,IPRO,JS,KS,A,B,ISTAT)
         ENDIF
      ENDIF
C
C   Save the results of the first attempt.  We will return these values
C   in case we find no satisfactory grid.
C
      IF (NFIRST.EQ.0) THEN
         NFIRST = NS
         JFIRST = JS
         KFIRST = KS
         AFIRST = A
         BFIRST = B
      ENDIF
C
C   Point lies outside this grid.
C
      IF (ISTAT.EQ.1) THEN
C
C   Check to see if we have a pointer to another grid.
C
         IBJ    = 0
         IBK    = 0
         IF      (JS.EQ.1           .AND. A.LT.0.) THEN
            IF (IGPB(KS,1,1,NS).NE.0) IBJ    = 1
         ELSE IF (JS.EQ.JRMAX(NS)-1 .AND. A.GT.1.) THEN
            IF (IGPB(KS,2,1,NS).NE.0) IBJ    = 2
         ENDIF
         IF      (KS.EQ.1           .AND. B.LT.0.) THEN
            IF (IGPB(JS,3,1,NS).NE.0) IBK    = 3
         ELSE IF (KS.EQ.KRMAX(NS)-1 .AND. B.GT.1.) THEN
            IF (IGPB(JS,4,1,NS).NE.0) IBK    = 4
         ENDIF
C
C   Get the grid number.
C
         NTJ    = 0
         NTK    = 0
         IF (IBJ.NE.0) NTJ    = IGPB(KS,IBJ,1,NS)
         IF (IBK.NE.0) NTK    = IGPB(JS,IBK,1,NS)
         NT     = 0
         IF      (NTJ.NE.0) THEN
            NT     = NTJ
            IB     = IBJ
            II     = KS
         ELSE IF (NTK.NE.0) THEN
            NT     = NTK
            IB     = IBK
            II     = JS
         ENDIF
C
C   We have an adjoining grid...
C
         IF (NT.NE.0) THEN
C
C   ...that we haven't visited before.
C
            IF (IRFAM(NT).GT.0) THEN
C
C   Get the cell index in the adjoining grid, go back and try to find
C   the point.
C
               JS     = IGPB(II,IB,2,NS)
               KS     = IGPB(II,IB,3,NS)
               NS     = NT
#ifdef DEBUG
      WRITE(*,*) 'Try neighboring grid'
#endif
               GOTO 10
C
C   ...but we've already tried it (and failed).
C
            ELSE
               ICSTAT = 1
            ENDIF
C
C   No adjoining grid.
C
         ELSE
            ICSTAT = 1
         ENDIF
C
C   Iteration did not converge.  Ignore and hope.
C
      ELSE IF (ISTAT.EQ.2) THEN
         ICSTAT = 0
C
C   Everything went fine.
C
      ELSE
         ICSTAT = 0
      ENDIF
C
C   If we didn't find a satisfactory grid cell starting with the initial
C   (JS,KS,NS) suggestion, we can find the closest point and search from
C   there (assuming this is a grid we haven't tried before).
C
      IF (ICSTAT.EQ.1 .AND. .NOT.FIND) THEN
         FIND   = .TRUE.
         CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &               JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
     &               I0,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               XR,YR,ZR,XP,YP,ZP)
         IF (IRFAM(NGSAVE(1)).GT.0) THEN
            JS     = JSAVE(1)
            KS     = KSAVE(1)
            NS     = NGSAVE(1)
#ifdef DEBUG
      WRITE(*,*) 'Try searching from global nearest point'
#endif
            GOTO 10
         ENDIF
      ENDIF
C
C   We didn't find a satisfactory grid.  Return the (extrapolated)
C   values from the first search attempt.
C
      IF (ICSTAT.EQ.1) THEN
         NS     = NFIRST
         JS     = JFIRST
         KS     = KFIRST
         A      = AFIRST
         B      = BFIRST
C
C   Restrict the computational bounds.
C
      ELSE IF (IRES.NE.0) THEN
         A      = MAX(MIN(A,ONE),ZERO)
         B      = MAX(MIN(B,ONE),ZERO)
      ENDIF
C
C   Reset IRFAM to non-negative numbers.
C
      DO 20 N = 1,NRGRD
         IRFAM(N)  = ABS(IRFAM(N))
   20    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CHKAXS(JRMAX,KRMAX,JMAX,KMAX,JS,J,K,
     &                  XR,YR,ZR,X,Y,Z,GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR)
C
C   Check if (j,k) point falls on an axis point and modify
C   interpolation.
C
#include "precis.h"
C
      PARAMETER (ZERO=0.0)
      DIMENSION XR(JRMAX,KRMAX), YR(JRMAX,KRMAX), ZR(JRMAX,KRMAX)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX),
     &          XSNR(JMAX), YSNR(JMAX), ZSNR(JMAX)
C
C
      IF ( (XR(JS,1).EQ.X(J,1)) .AND. (YR(JS,1).EQ.Y(J,1))
     &                          .AND. (ZR(JS,1).EQ.Z(J,1)) ) THEN
#ifdef DEBUG
         WRITE(*,1)
    1    FORMAT(' Point on initial curve coincides with an axis point.'/
     &          ' Special interpolation performed.')
#endif
         XSNR(J)   = XR(JS,1)
         YSNR(J)   = YR(JS,1)
         ZSNR(J)   = ZR(JS,1)
         IF (J.EQ.1) THEN
            GSN1(J)   = GSN1(J+1)
            GSN2(J)   = GSN2(J+1)
            GSN3(J)   = GSN3(J+1)
         ELSE IF (J.EQ.JMAX) THEN
            GSN1(J)   = GSN1(J-1)
            GSN2(J)   = GSN2(J-1)
            GSN3(J)   = GSN3(J-1)
         ELSE
            XN        = 0.5*(GSN1(J-1) + GSN1(J+1))
            YN        = 0.5*(GSN2(J-1) + GSN2(J+1))
            ZN        = 0.5*(GSN3(J-1) + GSN3(J+1))
            DN        = SQRT(XN**2 + YN**2 + ZN**2)
            IF (DN.NE.ZERO) THEN
               GSN1(J)   = XN/DN
               GSN2(J)   = YN/DN
               GSN3(J)   = ZN/DN
            ENDIF
         ENDIF
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CMPBMM(NRGRD,NTOTR,NGPR,NGNR,JRMAX,KRMAX,XR,YR,ZR,
     &                  XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX)
C
C   Compute big minmax box for each grid.
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XBMIN(NRGRD), YBMIN(NRGRD), ZBMIN(NRGRD)
      DIMENSION XBMAX(NRGRD), YBMAX(NRGRD), ZBMAX(NRGRD)
C
C   Tolerance for big minmax box is TOL times (wmax-wmin) in the
C   w direction where w=x,y,z.
C
      PARAMETER (TOL=0.04, HTOL=0.5*TOL)
C
C
      DO 20 N = 1,NRGRD
C
         IS     = NGPR(N)
         IE     = IS + NGNR(N) - 1
C
         XMIN   = XR(IS)
         XMAX   = XR(IS)
         YMIN   = YR(IS)
         YMAX   = YR(IS)
         ZMIN   = ZR(IS)
         ZMAX   = ZR(IS)
C
         DO 10 I = IS+1,IE
            XMIN   = MIN( XMIN, XR(I) )
            XMAX   = MAX( XMAX, XR(I) )
            YMIN   = MIN( YMIN, YR(I) )
            YMAX   = MAX( YMAX, YR(I) )
            ZMIN   = MIN( ZMIN, ZR(I) )
            ZMAX   = MAX( ZMAX, ZR(I) )
   10       CONTINUE
C
C   Extend all boundaries of box by a little.
C
         XSIZE  = XMAX - XMIN
         YSIZE  = YMAX - YMIN
         ZSIZE  = ZMAX - ZMIN
         TOLX   = TOL*XSIZE
         TOLY   = TOL*YSIZE
         TOLZ   = TOL*ZSIZE
C
C   Add a little more to the thin side.
C
         XSML   = HTOL*(YSIZE + ZSIZE)
         YSML   = HTOL*(XSIZE + ZSIZE)
         ZSML   = HTOL*(XSIZE + YSIZE)
         IF (XSIZE.LT.XSML) TOLX   = XSML
         IF (YSIZE.LT.YSML) TOLY   = YSML
         IF (ZSIZE.LT.ZSML) TOLZ   = ZSML
C
         XBMIN(N) = XMIN - TOLX
         XBMAX(N) = XMAX + TOLX
         YBMIN(N) = YMIN - TOLY
         YBMAX(N) = YMAX + TOLY
         ZBMIN(N) = ZMIN - TOLZ
         ZBMAX(N) = ZMAX + TOLZ
   20    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CMPFIX(JMAX,KMAX,M1D,J,K,IB,X,Y,Z,
     &                  XB,YB,ZB,XBP,YBP,ZBP,R,ISEG,IMODE)
C
#include "precis.h"
C
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION XB(M1D,2), YB(M1D,2), ZB(M1D,2), R(JMAX,KMAX)
      DIMENSION XBP(2), YBP(2), ZBP(2), ISEG(2)
C
C    This subroutine computes the coordinates of a grid point along
C    the fixed boundary at j. The current point and the marching
C    increment are given. The coordinates of the new point are obtained
C    by marching along the given line of points in XB,YB,ZB. The current
C    point is assumed to lie on the segment (ISEG,ISEG+1) of the line of
C    points defined by XB,YB,ZB.
C
C    IMODE = 1  predictor mode where the marching increment is given
C               by the 1D stretching function R.
C    IMODE = 2  corrector mode where the marching increment is given
C               by the increment of the neighboring point in j.
C
      XCUR = X(J,K-1)
      YCUR = Y(J,K-1)
      ZCUR = Z(J,K-1)
      I = ISEG(IB)
C
      IF (IMODE.EQ.1) THEN
       DELETA = R(J,K)-R(J,K-1)
      ELSE IF (IMODE.EQ.2) THEN
       IF (IB.EQ.1) THEN
        JN = J+1
C
C        JN = J+2
C
       ELSE IF (IB.EQ.2) THEN
        JN = J-1
C
C        JN = J-2
C
       ENDIF
       DELETA = SQRT( (X(JN,K)-X(JN,K-1))**2 + (Y(JN,K)-Y(JN,K-1))**2 +
     &                (Z(JN,K)-Z(JN,K-1))**2 )
      ENDIF
C
 25    CONTINUE
       DXN = XB(I+1,IB) - XCUR
       DYN = YB(I+1,IB) - YCUR
       DZN = ZB(I+1,IB) - ZCUR
       DNODE = SQRT(DXN*DXN + DYN*DYN + DZN*DZN)
C
       IF (DELETA.GT.DNODE) THEN
        DELETA = DELETA - DNODE
        XCUR = XB(I+1,IB)
        YCUR = YB(I+1,IB)
        ZCUR = ZB(I+1,IB)
        I = I+1
        GOTO 25
       ENDIF
C
      DM = DELETA/DNODE
      XBP(IB) = XCUR + DM*DXN
      YBP(IB) = YCUR + DM*DYN
      ZBP(IB) = ZCUR + DM*DZN
      IF (IMODE.EQ.2) ISEG(IB) = I
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CMPSMM(NRGRD,NTOTR,NGPR,NGP1,JRMAX,KRMAX,XR,YR,ZR,
     &                  XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX)
C
C   Compute small minmax box for each cell of each grid.
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGP1(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XSMIN(NTOTR), YSMIN(NTOTR), ZSMIN(NTOTR)
      DIMENSION XSMAX(NTOTR), YSMAX(NTOTR), ZSMAX(NTOTR)
C
C   Tolerance for small minmax box is TOL times (wmax-wmin) in the
C   w direction where w=x,y,z.
C
      PARAMETER (TOL=0.1, HTOL=0.5*TOL)
C
C
      DO 20 N = 1,NRGRD
C
         ISM  = NGPR(N) - 1
         IS1M = NGP1(N) - 1 
C
         DO 10 K = 1,KRMAX(N)-1
         DO 10 J = 1,JRMAX(N)-1
C
            JK0    = (K-1)*JRMAX(N) + J
            JK1    = JK0 + ISM
            JK2    = JK1 + 1
            JK3    = JK1 + JRMAX(N)
            JK4    = JK3 + 1
            JK11   = JK0 + IS1M
C
            XMIN   = MIN( XR(JK1), XR(JK2), XR(JK3), XR(JK4) )
            XMAX   = MAX( XR(JK1), XR(JK2), XR(JK3), XR(JK4) )
            YMIN   = MIN( YR(JK1), YR(JK2), YR(JK3), YR(JK4) )
            YMAX   = MAX( YR(JK1), YR(JK2), YR(JK3), YR(JK4) )
            ZMIN   = MIN( ZR(JK1), ZR(JK2), ZR(JK3), ZR(JK4) )
            ZMAX   = MAX( ZR(JK1), ZR(JK2), ZR(JK3), ZR(JK4) )
C
C   Extend all boundaries of box by a little.
C
            XSIZE  = XMAX - XMIN
            YSIZE  = YMAX - YMIN
            ZSIZE  = ZMAX - ZMIN
            TOLX   = TOL*XSIZE
            TOLY   = TOL*YSIZE
            TOLZ   = TOL*ZSIZE
C
C   Add a little more to the thin side.
C
            XSML   = HTOL*(YSIZE + ZSIZE)
            YSML   = HTOL*(XSIZE + ZSIZE)
            ZSML   = HTOL*(XSIZE + YSIZE)
            IF (XSIZE.LT.XSML) TOLX   = XSML
            IF (YSIZE.LT.YSML) TOLY   = YSML
            IF (ZSIZE.LT.ZSML) TOLZ   = ZSML
C
            XSMIN(JK11) = XMIN - TOLX
            XSMAX(JK11) = XMAX + TOLX
            YSMIN(JK11) = YMIN - TOLY
            YSMAX(JK11) = YMAX + TOLY
            ZSMIN(JK11) = ZMIN - TOLZ
            ZSMAX(JK11) = ZMAX + TOLZ
   10       CONTINUE
   20    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE CONARE(JMAX,KMAX,K,JPER,ITSVOL,JJP,JJR,
     &                  XX,YX,ZX,R,DAREA,AVOL)
C
#include "precis.h"
C
      DIMENSION JJP(JMAX),JJR(JMAX)
      DIMENSION XX(JMAX), YX(JMAX), ZX(JMAX), R(JMAX,KMAX), DAREA(JMAX)
      DIMENSION AVOL(JMAX)
C
C    Compute control area which governs hyperbolic grid growth.
C    Area is computed from specified arc length R(j,k) times previous
C    delta xi arc length
C
      DO 30 J =1,JMAX
       DAREA(J) =(R(J,K)-R(J,K-1))*SQRT(XX(J)**2+YX(J)**2+ZX(J)**2)
 30   CONTINUE
C
      IF (ITSVOL .GT. 0) THEN
      DO 40 NITS = 1,ITSVOL
C
      DO 41 J = 2,JMAX-1
       JP = JJP(J)
       JR = JJR(J)
       AVOL(J) = .875*DAREA(J) +.0625*(DAREA(JP)+DAREA(JR))
  41  CONTINUE
      IF (JPER.EQ.1) THEN
       AVOL(1) = .875*DAREA(1) +.0625*(DAREA(2)+DAREA(JMAX-1))
       AVOL(JMAX) = AVOL(1)
C
C       AVOL(1) = .875*DAREA(1) +.0625*(DAREA(2)+DAREA(JMAX))
C       AVOL(JMAX) = .875*DAREA(JMAX) +.0625*(DAREA(JMAX-1)+DAREA(1))
C
      ELSE
       AVOL(1) = DAREA(1)
       AVOL(JMAX) = DAREA(JMAX)
      ENDIF
      DO 42 J = 1,JMAX
       DAREA(J) = AVOL(J)
  42  CONTINUE
C
  40  CONTINUE
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE FILTRX(JMAX,JKD,JS,JE,SMU,TIM,CAXI,H,RH,DFN,A,B,C,F)
C
#include "precis.h"
C
      PARAMETER (ZERO=0.0, HALF=0.5, ONE=1.0, TWO=2.0)
      DIMENSION CAXI(JMAX), H(JKD,3,3), RH(JMAX,3), DFN(JMAX)
      DIMENSION A(JKD,3,3), B(JKD,3,3), C(JKD,3,3), F(JKD,3)
C
C    Fill implicit block tridiagonal matrix
C
      SMUIMC = TWO
      TM = 0.5*(ONE + TIM)
C
      DO 10 J = JS,JE
      DO 12 N=1,3
      DO 13 M=1,3
       A(J,N,M) = -TM*H(J,N,M)
       B(J,N,M) = ZERO
       C(J,N,M) =  TM*H(J,N,M)
 13   CONTINUE
      SMUIM = SMUIMC*DFN(J)
      A(J,N,N) = A(J,N,N) - SMUIM
      B(J,N,N) = ONE + TWO*SMUIM
      C(J,N,N) = C(J,N,N) - SMUIM
      F(J,N) = RH(J,N)
 12   CONTINUE
 10   CONTINUE
C
C    Implicit averaging for convex corners
C
      CVEX = -HALF
      DO 20 J = JS,JE
       IF (CAXI(J).LT.CVEX) THEN
        DO 22 N=1,3
        DO 22 M=1,3
         A(J,N,M) = ZERO
         C(J,N,M) = ZERO
 22     CONTINUE
        DO 23 N=1,3
         A(J,N,N) = -HALF
         B(J,N,N) =  ONE
         C(J,N,N) = -HALF
 23     CONTINUE
       ENDIF
 20   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE FNDGJK(M1D,NRGRD,NTOTR,NGP1,JRMAX,KRMAX,
     &                  IP1,IP2,IJK,IB,N,XR,YR,ZR,
     &                  XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX,
     &                  XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX,IGPB,IRFAM)
C
C   Given mid-pt of edge defined by IP1 and IP2 on grid N, find
C   grid number and J,K cell index of neighbor.
C
#include "precis.h"
C
      PARAMETER (HALF=0.5)
      DIMENSION NGP1(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XBMIN(NRGRD), YBMIN(NRGRD), ZBMIN(NRGRD)
      DIMENSION XBMAX(NRGRD), YBMAX(NRGRD), ZBMAX(NRGRD)
      DIMENSION XSMIN(NTOTR), YSMIN(NTOTR), ZSMIN(NTOTR)
      DIMENSION XSMAX(NTOTR), YSMAX(NTOTR), ZSMAX(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
C
C
      XP     = HALF*( XR(IP1) + XR(IP2) )
      YP     = HALF*( YR(IP1) + YR(IP2) )
      ZP     = HALF*( ZR(IP1) + ZR(IP2) )
C
      DO 20 NT = 1,NRGRD
         IF (N.NE.NT .AND. IRFAM(N).EQ.IRFAM(NT)) THEN
C
C   Test if point is inside big minmax box of grid NT.
C
            IF (   (XP.GE.XBMIN(NT) .AND. XP.LE.XBMAX(NT))
     &       .AND. (YP.GE.YBMIN(NT) .AND. YP.LE.YBMAX(NT))
     &       .AND. (ZP.GE.ZBMIN(NT) .AND. ZP.LE.ZBMAX(NT)) ) THEN
C
C   Test which small minmax box the point belongs to.
C
               ISM = NGP1(NT) - 1
C
               DO 10 K = 1,KRMAX(NT)-1
               DO 10 J = 1,JRMAX(NT)-1
                  I      = (K-1)*JRMAX(NT) + J + ISM
                  IF (   (XP.GE.XSMIN(I) .AND. XP.LE.XSMAX(I))
     &             .AND. (YP.GE.YSMIN(I) .AND. YP.LE.YSMAX(I))
     &             .AND. (ZP.GE.ZSMIN(I) .AND. ZP.LE.ZSMAX(I)) )
     &                                                              THEN
                     IGPB(IJK,IB,1,N) = NT
                     IGPB(IJK,IB,2,N) = J
                     IGPB(IJK,IB,3,N) = K
                     GO TO 100
                  ENDIF
   10             CONTINUE
            ENDIF
         ENDIF
   20    CONTINUE
C
  100 CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE FNDIQD(M1D,NRGRD,NTOTR,JMAX,KMAX,NGPR,JRMAX,KRMAX,
     &                  J1,J2,K,NS,JS,KS,JPER,JRPER,KRPER,JRAXSA,JRAXSB,
     &                  IG,JRP,JRR,KRP,KRR,XR,YR,ZR,IGPB,
     &                  IRFAM,IAFAM,JKIH,X,Y,Z,R)
C
C    Determine grid number and quad indices for points on initial line
C    that will provide correct marching direction. Note that the grid
C    and quad indices found may not necessarily contain the point on
C    the original line.
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD), JKIH(JMAX,3)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), R(JMAX,KMAX)
C
C   Number of close points we're willing to consider.
C
      PARAMETER (MSAVE=1)
      DIMENSION JSAVE(MSAVE),KSAVE(MSAVE),NGSAVE(MSAVE)
      PARAMETER (I0=0,I1=1)
      PARAMETER (TOLS=1.E-3)
C
      DO 10 J=1,JMAX-1
C
C      Find cell for current point.
C
         CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &               JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
     &               IG,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               XR,YR,ZR,X(J,K),Y(J,K),Z(J,K))
         JS        = JSAVE(1)
         KS        = KSAVE(1)
         NS        = NGSAVE(1)
C
#ifdef DEBUG
         WRITE(*,*)
         WRITE(*,*) 'calling cells from fndiqd'
         WRITE(*,*) ' initial point (j,k)        ',J,K
         WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
C
         CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &               JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &               IGPB,I0,IQSTAT)
C
#ifdef DEBUG
         WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
         WRITE(*,*)
#endif
C
         NS1       = NS
         JS1       = JS
         KS1       = KS
         DELXI1    = DELXI
         DELET1    = DELET
C
         ADXA1     = ABS(DELXI)
         ADXB1     = ABS(1.0-DELXI)
         ADEA1     = ABS(DELET)
         ADEB1     = ABS(1.0-DELET)
         IF ( (ADXB1.LT.TOLS) .AND. 
     &        ((JRPER(NS1).EQ.1).OR.(JS1.LT.JRMAX(NS1)-1)) )
     &        JS1 = JRP(JS1,NS1)
         IF ( (ADEB1.LT.TOLS) .AND.
     &        ((KRPER(NS1).EQ.1).OR.(KS1.LT.KRMAX(NS1)-1)) )
     &        KS1 = KRP(KS1,NS1)
         JKIH(J,1) = JS1
         JKIH(J,2) = KS1
         JKIH(J,3) = NS1
C
C      If current point lands near a cell boundary, find cell for next point.
C
         IF (     (ADXA1.LT.TOLS .OR. ADXB1.LT.TOLS)
     &       .OR. (ADEA1.LT.TOLS .OR. ADEB1.LT.TOLS) ) THEN
C
C          Current point is near a cell boundary.
C
C            CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
C     &                  JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
C     &                  IG,JRAXSA,JRAXSB,IRFAM,IAFAM,
C     &                  XR,YR,ZR,X(J+1,K),Y(J+1,K),Z(J+1,K))
C            JS        = JSAVE(1)
C            KS        = KSAVE(1)
C            NS        = NGSAVE(1)
C
C          Take guess from previous cell.
C
             JS = JS1
             KS = KS1
             NS = NS1
C
#ifdef DEBUG
            WRITE(*,*)
            WRITE(*,*) 'calling cells from fndiqd x2'
            WRITE(*,*) ' initial point (j,k)        ',J,K
            WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
C
            CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &                  X(J+1,K),Y(J+1,K),Z(J+1,K),NS,JS,KS,DELXI,DELET,
     &                  IGPB,I0,IQSTAT)
C
#ifdef DEBUG
            WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
            WRITE(*,*)
#endif
C
            NS2       = NS
            JS2       = JS
            KS2       = KS
            DELXI2    = DELXI
            DELET2    = DELET
C
            ADXA2     = ABS(DELXI)
            ADXB2     = ABS(1.0-DELXI)
            ADEA2     = ABS(DELET)
            ADEB2     = ABS(1.0-DELET)
            IF ( (ADXB2.LT.TOLS) .AND. 
     &           ((JRPER(NS2).EQ.1).OR.(JS2.LT.JRMAX(NS2)-1)) )
     &           JS2 = JRP(JS2,NS2)
            IF ( (ADEB2.LT.TOLS) .AND.
     &           ((KRPER(NS2).EQ.1).OR.(KS2.LT.KRMAX(NS2)-1)) )
     &           KS2 = KRP(KS2,NS2)
C
            NREDGE = 0
            IF (     (ADXA2.LT.TOLS .OR. ADXB2.LT.TOLS)
     &          .OR. (ADEA2.LT.TOLS .OR. ADEB2.LT.TOLS) ) NREDGE = 1
            ISEDGE = 0
            IF ( (NS1.EQ.NS2) .AND. 
     &           ((JS1.EQ.JS2).OR.(KS1.EQ.KS2)) ) ISEDGE = 1
C
            IF ( (NREDGE.EQ.1) .AND. (ISEDGE.EQ.1) ) THEN
C
C            Next point is also near a cell boundary along the same edge
C            of the same panel network. Have to do more tests.
C
              CALL BNCHEK(M1D,NRGRD,JMAX,JRMAX,KRMAX,
     &                    JRPER,KRPER,JRAXSA,JRAXSB,JRP,JRR,KRP,KRR,
     &                    IGPB,JKIH,J,JS1,KS1,NS1,JS2,KS2,NS2,
     &                  ADXA1,ADXB1,ADEA1,ADEB1,ADXA2,ADXB2,ADEA2,ADEB2)
C
            ELSE
C
C            Next point is not near a cell boundary along the same edge
C            of the same panel network. Copy cell info for next point on
C            to current point.
C
              JKIH(J,1) = JS2
              JKIH(J,2) = KS2
              JKIH(J,3) = NS2
C
            ENDIF
C
         ENDIF
C
C        Check JKIH for outside domain indices.
C
          IF (JKIH(J,3).EQ.0) THEN
             WRITE(*,*)'Initial curve index direction may be wrong.'
             WRITE(*,*)'Try reversing index direction and re-run.'
             STOP 'FNDIQD'
          ENDIF
C
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE BNCHEK(M1D,NRGRD,JMAX,JRMAX,KRMAX,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,JRP,JRR,KRP,KRR,
     &                  IGPB,JKIH,J,JS1,KS1,NS1,JS2,KS2,NS2,
     &                  ADXA1,ADXB1,ADEA1,ADEB1,ADXA2,ADXB2,ADEA2,ADEB2)
C
#include "precis.h"
C
C    Given cell indices and interpolation coefs for current point and
C    next point that land near a cell edge on the same panel network,
C    determine whether the cell for the current point is acceptable
C    based on right hand rule for cell normal and initial curve index
C    direction. If not acceptable, find the neighboring cell that is
C    (may need to go to neighboring reference panel network).
C
C     TOLS: Point is considered near an edge if it is within TOLS of
C     that edge in parameter space of cell.
C
      PARAMETER (TOLS=1.E-3)
      DIMENSION JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION IGPB(M1D,4,3,NRGRD), JKIH(JMAX,3)
C
      IF      (JS1.EQ.JS2 .AND. KS1.NE.KS2) THEN
C
C         On J=constant edge with different K indices.
C
            IF      (KS2.GT.KS1) THEN
C
C             Have to pick cell on the other side of edge.
C
               IF (JRR(JS1,NS1).NE.JS1) THEN
                  JKIH(J,1)= JRR(JS1,NS1)
                  JKIH(J,2)= KS1
                  JKIH(J,3)= NS1
               ELSE
                 IF (KRPER(NS1).EQ.0) THEN
                  JKIH(J,1)= IGPB(KS1,1,2,NS1)
                  JKIH(J,2)= IGPB(KS1,1,3,NS1)
                  JKIH(J,3)= IGPB(KS1,1,1,NS1)
                 ENDIF
               ENDIF
C
            ELSE IF (KS2.LT.KS1) THEN
C
            ENDIF
C
      ELSE IF (JS1.NE.JS2 .AND. KS1.EQ.KS2) THEN
C
C         On K=constant edge with different J indices.
C
            IF      (JS2.LT.JS1) THEN
C
C             Have to pick cell on the other side of edge.
C
               IF (KRR(KS1,NS1).NE.KS1) THEN
                  JKIH(J,1)= JS1
                  JKIH(J,2)= KRR(KS1,NS1)
                  JKIH(J,3)= NS1
               ELSE
                 IF (JRPER(NS1).EQ.0) THEN
                  JKIH(J,1)= IGPB(JS1,3,2,NS1)
                  JKIH(J,2)= IGPB(JS1,3,3,NS1)
                  JKIH(J,3)= IGPB(JS1,3,1,NS1)
                 ENDIF
               ENDIF
C
            ELSE IF (JS2.GT.JS1) THEN
C
            ENDIF
C
      ELSE IF (JS1.EQ.JS2 .AND. KS1.EQ.KS2) THEN
C
C         In the same cell.
C
            IF (ADXA1.LT.TOLS .AND. ADXA2.LT.TOLS) IED       = 1
            IF (ADXB1.LT.TOLS .AND. ADXB2.LT.TOLS) IED       = 2
            IF (ADEA1.LT.TOLS .AND. ADEA2.LT.TOLS) IED       = 3
            IF (ADEB1.LT.TOLS .AND. ADEB2.LT.TOLS) IED       = 4
C
            IF      (IED.EQ.1) THEN
C
               IF      (ADEA1.LT.ADEA2) THEN
                  IF (JRR(JS1,NS1).NE.JS1) THEN
                     JKIH(J,1)= JRR(JS1,NS1)
                     JKIH(J,2)= KS1
                     JKIH(J,3)= NS1
                  ELSE
                     JKIH(J,1)= IGPB(KS1,1,2,NS1)
                     JKIH(J,2)= IGPB(KS1,1,3,NS1)
                     JKIH(J,3)= IGPB(KS1,1,1,NS1)
                  ENDIF
               ENDIF
C
            ELSE IF (IED.EQ.2) THEN
C
               IF (ADEA1.GT.ADEA2) THEN
                  IF (JS1.EQ.JRMAX(NS1)) THEN
                     JKIH(J,1)= IGPB(KS1,2,2,NS1)
                     JKIH(J,2)= IGPB(KS1,2,3,NS1)
                     JKIH(J,3)= IGPB(KS1,2,1,NS1)
                  ENDIF
               ENDIF
C
            ELSE IF (IED.EQ.3) THEN
C
               IF (ADXA2.LT.ADXA1) THEN
                  IF (KRR(KS1,NS1).NE.KS1) THEN
                     JKIH(J,1)= JS1
                     JKIH(J,2)= KRR(KS1,NS1)
                     JKIH(J,3)= NS1
                  ELSE
                     JKIH(J,1)= IGPB(JS1,3,2,NS1)
                     JKIH(J,2)= IGPB(JS1,3,3,NS1)
                     JKIH(J,3)= IGPB(JS1,3,1,NS1)
                  ENDIF
               ENDIF
C
            ELSE IF (IED.EQ.4) THEN
C
               IF (ADXA2.GT.ADXA1) THEN
                  IF (KS1.EQ.KRMAX(NS1)) THEN
                     JKIH(J,1)= IGPB(JS1,4,2,NS1)
                     JKIH(J,2)= IGPB(JS1,4,3,NS1)
                     JKIH(J,3)= IGPB(JS1,4,1,NS1)
                  ENDIF
               ENDIF
C
            ENDIF
C
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE FNDIQDO(M1D,NRGRD,NTOTR,JMAX,KMAX,NGPR,JRMAX,KRMAX,
     &                  J1,J2,K,NS,JS,KS,JPER,JRPER,KRPER,JRAXSA,JRAXSB,
     &                  IG,JRP,JRR,KRP,KRR,XR,YR,ZR,IGPB,
     &                  IRFAM,IAFAM,JKIH,X,Y,Z,R)
C
C    Determine grid number and quad indices for points on initial line
C    that will provide correct marching direction. Note that the grid
C    and quad indices found may not necessarily contain the point on
C    the original line.
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD), JKIH(JMAX,3)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), R(JMAX,KMAX)
C
C   Number of close points we're willing to consider.
C
      PARAMETER (MSAVE=1)
      DIMENSION JSAVE(MSAVE),KSAVE(MSAVE),NGSAVE(MSAVE)
      PARAMETER (I0=0,I1=1)
      PARAMETER (TOLS=1.E-3)
C
C
C   Find cell for first point on initial curve.
C
      J         = J1
      CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &            JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
     &            IG,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &            XR,YR,ZR,X(J,K),Y(J,K),Z(J,K))
      JS        = JSAVE(1)
      KS        = KSAVE(1)
      NS        = NGSAVE(1)
C
#ifdef DEBUG
      WRITE(*,*)
      WRITE(*,*) 'calling cells from fndiqd'
      WRITE(*,*) ' initial point (j,k)        ',J,K
      WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
C
      CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &            JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &            X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &            IGPB,I0,IQSTAT)
C
#ifdef DEBUG
      WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
      WRITE(*,*)
#endif
      NS1       = NS
      JS1       = JS
      KS1       = KS
      DELXI1    = DELXI
      DELET1    = DELET
C
      ADXA1     = ABS(DELXI)
      ADXB1     = ABS(1.0-DELXI)
      ADEA1     = ABS(DELET)
      ADEB1     = ABS(1.0-DELET)
      IF (ADXB1.LT.TOLS) JS1       = JRP(JS1,NS1)
      IF (ADEB1.LT.TOLS) KS1       = KRP(KS1,NS1)
      JKIH(J1,1)= JS1
      JKIH(J1,2)= KS1
      JKIH(J1,3)= NS1
C
C   Find cell for second point on initial curve if first point lands near
C   a cell boundary.
C
      IF (     (ADXA1.LT.TOLS .OR. ADXB1.LT.TOLS)
     &    .OR. (ADEA1.LT.TOLS .OR. ADEB1.LT.TOLS) ) THEN
C
         J         = J+1
         CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &               JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
     &               IG,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               XR,YR,ZR,X(J,K),Y(J,K),Z(J,K))
         JS        = JSAVE(1)
         KS        = KSAVE(1)
         NS        = NGSAVE(1)
C
#ifdef DEBUG
         WRITE(*,*)
         WRITE(*,*) 'calling cells from fndiqd x2'
         WRITE(*,*) ' initial point (j,k)        ',J,K
         WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
         CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &               JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &               IGPB,I0,IQSTAT)
#ifdef DEBUG
         WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
         WRITE(*,*)
#endif
C
         NS2       = NS
         JS2       = JS
         KS2       = KS
         DELXI2    = DELXI
         DELET2    = DELET
C
         ADXA2     = ABS(DELXI)
         ADXB2     = ABS(1.0-DELXI)
         ADEA2     = ABS(DELET)
         ADEB2     = ABS(1.0-DELET)
         IF (ADXB2.LT.TOLS) JS2       = JRP(JS2,NS2)
         IF (ADEB2.LT.TOLS) KS2       = KRP(KS2,NS2)
C
         IF      (JS1.EQ.JS2 .AND. KS1.NE.KS2) THEN
C
            IF      (KS2.GT.KS1) THEN
               IF (JRR(JS1,NS1).NE.JS1) THEN
                  JKIH(J1,1)= JRR(JS1,NS1)
                  JKIH(J1,2)= KS1
                  JKIH(J1,3)= NS1
               ELSE
                 IF (KRPER(NS1).EQ.0) THEN
                  JKIH(J1,1)= IGPB(KS1,1,2,NS1)
                  JKIH(J1,2)= IGPB(KS1,1,3,NS1)
                  JKIH(J1,3)= IGPB(KS1,1,1,NS1)
                 ENDIF
               ENDIF
            ELSE IF (KS2.LT.KS1) THEN
               IF (JS1.EQ.JRMAX(NS1)) THEN
                  JKIH(J1,1)= IGPB(KS1,2,2,NS1)
                  JKIH(J1,2)= IGPB(KS1,2,3,NS1)
                  JKIH(J1,3)= IGPB(KS1,2,1,NS1)
               ENDIF
            ENDIF
C
         ELSE IF (JS1.NE.JS2 .AND. KS1.EQ.KS2) THEN
C
            IF      (JS2.LT.JS1) THEN
               IF (KRR(KS1,NS1).NE.KS1) THEN
                  JKIH(J1,1)= JS1
                  JKIH(J1,2)= KRR(KS1,NS1)
                  JKIH(J1,3)= NS1
               ELSE
                 IF (JRPER(NS1).EQ.0) THEN
                  JKIH(J1,1)= IGPB(JS1,3,2,NS1)
                  JKIH(J1,2)= IGPB(JS1,3,3,NS1)
                  JKIH(J1,3)= IGPB(JS1,3,1,NS1)
                 ENDIF
               ENDIF
            ELSE IF (JS2.GT.JS1) THEN
               IF (KS1.EQ.KRMAX(NS1)) THEN
                  JKIH(J1,1)= IGPB(JS1,4,2,NS1)
                  JKIH(J1,2)= IGPB(JS1,4,3,NS1)
                  JKIH(J1,3)= IGPB(JS1,4,1,NS1)
               ENDIF
            ENDIF
C
         ELSE IF (JS1.EQ.JS2 .AND. KS1.EQ.KS2) THEN
C
            IF (ADXA1.LT.TOLS .AND. ADXA2.LT.TOLS) IED       = 1
            IF (ADXB1.LT.TOLS .AND. ADXB2.LT.TOLS) IED       = 2
            IF (ADEA1.LT.TOLS .AND. ADEA2.LT.TOLS) IED       = 3
            IF (ADEB1.LT.TOLS .AND. ADEB2.LT.TOLS) IED       = 4
C
            IF      (IED.EQ.1) THEN
C
               IF      (ADEA1.LT.ADEA2) THEN
                  IF (JRR(JS1,NS1).NE.JS1) THEN
                     JKIH(J1,1)= JRR(JS1,NS1)
                     JKIH(J1,2)= KS1
                     JKIH(J1,3)= NS1
                  ELSE
                     JKIH(J1,1)= IGPB(KS1,1,2,NS1)
                     JKIH(J1,2)= IGPB(KS1,1,3,NS1)
                     JKIH(J1,3)= IGPB(KS1,1,1,NS1)
                  ENDIF
               ENDIF
C
            ELSE IF (IED.EQ.2) THEN
C
               IF (ADEA1.GT.ADEA2) THEN
                  IF (JS1.EQ.JRMAX(NS1)) THEN
                     JKIH(J1,1)= IGPB(KS1,2,2,NS1)
                     JKIH(J1,2)= IGPB(KS1,2,3,NS1)
                     JKIH(J1,3)= IGPB(KS1,2,1,NS1)
                  ENDIF
               ENDIF
C
            ELSE IF (IED.EQ.3) THEN
C
               IF (ADXA2.LT.ADXA1) THEN
                  IF (KRR(KS1,NS1).NE.KS1) THEN
                     JKIH(J1,1)= JS1
                     JKIH(J1,2)= KRR(KS1,NS1)
                     JKIH(J1,3)= NS1
                  ELSE
                     JKIH(J1,1)= IGPB(JS1,3,2,NS1)
                     JKIH(J1,2)= IGPB(JS1,3,3,NS1)
                     JKIH(J1,3)= IGPB(JS1,3,1,NS1)
                  ENDIF
               ENDIF
C
            ELSE IF (IED.EQ.4) THEN
C
               IF (ADXA2.GT.ADXA1) THEN
                  IF (KS1.EQ.KRMAX(NS1)) THEN
                     JKIH(J1,1)= IGPB(JS1,4,2,NS1)
                     JKIH(J1,2)= IGPB(JS1,4,3,NS1)
                     JKIH(J1,3)= IGPB(JS1,4,1,NS1)
                  ENDIF
               ENDIF
C
            ENDIF
C
         ENDIF
C
      ENDIF
C
C    Check JKIH for outside domain indices.
C
      IF (JKIH(J1,3).EQ.0) THEN
         WRITE(*,*)'Initial curve index direction may be wrong.'
         WRITE(*,*)'Try reversing index direction and re-run.'
         STOP 'FNDIQD'
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &                  JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
     &                  NGS,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &                  XR,YR,ZR,XP,YP,ZP)
C
C   Find the nearest point(s) to the target point (XP,YP,ZP) from among
C   the surface grids in the family IAFAM.  If NGS is greater than zero,
C   just check that grid.  Return a list of up to MSAVE points (all with
C   the same minimum distance).  NSAVE is the actual number of points
C   returned, with grid numbers NGSAVE and indices (JSAVE,KSAVE).
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRAXSA(NRGRD), JRAXSB(NRGRD), IRFAM(NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION JSAVE(MSAVE),KSAVE(MSAVE),NGSAVE(MSAVE)
C
C   Number of close points we're willing to consider.
C
      PARAMETER (MNRST=5)
      DIMENSION JNRST(MNRST),KNRST(MNRST)
      PARAMETER (I1=1,I2=2)
      LOGICAL FIRST
C
C   Define a distance function.
C
      DIST2(AX,AY,AZ) = AX**2 + AY**2 + AZ**2
C
C
C   If NGS>0, just test grid NGS.
C
      IF (NGS.GT.0) THEN
         NS     = NGS
         NE     = NGS
C
C   Otherwise loop through all grids.
C
      ELSE
         NS     = 1
         NE     = NRGRD
      ENDIF
C
C   Find the grid number and index of the closest point.
C   Only search grids in family IAFAM or grid number NGS.
C
      NSAVE     = 0
      JSAVE(1)  = 1
      KSAVE(1)  = 1
      NGSAVE(1) = NS
      FIRST     = .TRUE.
      DO 100 N = NS,NE
         IF (ABS(IRFAM(N)).EQ.IAFAM .OR. NGS.GT.0) THEN
            IS = NGPR(N)
            CALL NRSTPT(XP,YP,ZP,JNRST,KNRST,NNRST,MNRST,DNRST,
     &                  XR(IS),YR(IS),ZR(IS),JRMAX(N),KRMAX(N))
C
c            write(*,*)'Nearest point at ',JNRST(1),KNRST(1),N,DNRST
C
C   For the first grid we check, these are the points to store.
C
            IF (FIRST) THEN
               FIRST  = .FALSE.
               DSAVE  = DNRST
               NSAVE  = MIN(NNRST,MSAVE)
               DO 10 I = 1,NSAVE
                  JSAVE(I)  = JNRST(I)
                  KSAVE(I)  = KNRST(I)
                  NGSAVE(I) = N
   10             CONTINUE
C
C   For follow-on grids, store the points with minimum distance.
C
            ELSE IF (DNRST.LT.DSAVE) THEN
               DSAVE  = DNRST
               NSAVE  = MIN(NNRST,MSAVE)
               DO 20 I = 1,NSAVE
                  JSAVE(I)  = JNRST(I)
                  KSAVE(I)  = KNRST(I)
                  NGSAVE(I) = N
   20             CONTINUE
C
C   If the distances are the same, try to append the list of points.
C
            ELSE IF (DNRST.EQ.DSAVE) THEN
               NOLD   = NSAVE
               NSAVE  = MIN(NSAVE+NNRST,MSAVE)
               IN     = 0
               DO 30 I = NOLD+1,NSAVE
                  IN        = IN+1
                  JSAVE(I)  = JNRST(IN)
                  KSAVE(I)  = KNRST(IN)
                  NGSAVE(I) = N
   30             CONTINUE
            ENDIF
         ENDIF
  100    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE FNDSPN(M1D,NRGRD,NTOTR,JMAX,KMAX,NGPR,NGP1,JRMAX,KRMAX,
     &                  J1,J2,K,JPER,JRPER,KRPER,JRAXSA,JRAXSB,
     &                  JRP,JRR,KRP,KRR,X,Y,Z,R,XR,YR,ZR,
     &                  XNR,YNR,ZNR,IGPB,IRFAM,IAFAM,GNV,
     &                  GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,JKIH)
C
C    Find point XS,YS,ZS and unit surface normal on reference surface.
C    Use CELLS to find surface quadrant JS,KS that points (J,K) are
C    contained in, then use bilinear interpolation to find surface normal
C    and point associated with grid point (J,K).
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGP1(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XNR(NTOTR), YNR(NTOTR), ZNR(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
C
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), R(JMAX,KMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX),
     &          XSNR(JMAX), YSNR(JMAX), ZSNR(JMAX)
      DIMENSION JKIH(JMAX,3), GNV(JMAX,KMAX,3)
C
      PARAMETER (I0=0,I1=1)
C
C
      IF (K.EQ.1) THEN
         IRES   = 0
      ELSE
         IRES   = 1
      ENDIF
C
      DO 100 J = J1,J2
C
         IF (K.EQ.1) THEN
            IF (J.EQ.J1) THEN
               JS     = JKIH(J,1)
               KS     = JKIH(J,2)
               NS     = JKIH(J,3)
            ELSE
               JS     = JKIH(J-1,1)
               KS     = JKIH(J-1,2)
               NS     = JKIH(J-1,3)
            ENDIF
         ELSE
            JS     = JKIH(J,1)
            KS     = JKIH(J,2)
            NS     = JKIH(J,3)
         ENDIF
C
#ifdef DEBUG
         WRITE(*,*)
         WRITE(*,*) 'calling cells from fndspn'
         WRITE(*,*) ' initial point (j,k)        ',J,K
         WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
         CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &               JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &               IGPB,IRES,IQSTAT)
#ifdef DEBUG
         WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
         WRITE(*,*)
#endif
C
C   Take initial guess from previous J point and try again.
C
         IF (IQSTAT.EQ.1 .AND. J.GT.J1) THEN
C
            IF (K.GT.1) THEN
              JS        = JKIH(J-1,1)
              KS        = JKIH(J-1,2)
              NS        = JKIH(J-1,3)
            ELSE IF (K.EQ.1) THEN
              CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &                    JSAVE,KSAVE,NGSAVE,NSAVE,I1,
     &                    I0,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &                    XR,YR,ZR,X(J,K),Y(J,K),Z(J,K))
              JS = JSAVE
              KS = KSAVE
              NS = NGSAVE
            ENDIF
C
#ifdef DEBUG
            WRITE(*,*)
            WRITE(*,*) 'calling cells from fndspn x2'
            WRITE(*,*) ' initial point (j,k)        ',J,K
            WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
            CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &                  X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &                  IGPB,IRES,IQSTAT)
#ifdef DEBUG
            WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
            WRITE(*,*)
#endif
C
         ENDIF
C
         JKIH(J,1) = JS
         JKIH(J,2) = KS
         JKIH(J,3) = NS
C
         IS  = NGPR(NS)
         IS1 = NGP1(NS)
         CALL INTERP(JRMAX(NS),KRMAX(NS),JS,KS,
     &               DELXI,DELET,XR(IS),YR(IS),ZR(IS),
     &               XNR(IS1),YNR(IS1),ZNR(IS1),
     &               GSN1(J),GSN2(J),GSN3(J),XSNR(J),YSNR(J),ZSNR(J))
C
  100    CONTINUE
C
C   Do special interpolation for points on the initial curve that fall
C   on an axis point.
C
      IF (K.EQ.1) THEN
         CALL INTAXS(NRGRD,NTOTR,NGPR,JMAX,KMAX,JRMAX,KRMAX,J1,J2,K,
     &               JRAXSA,JRAXSB,X,Y,Z,XR,YR,ZR,
     &               GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,JKIH)
      ENDIF
C
      DO 110 J = J1,J2
         GNV(J,K,1) = GSN1(J)
         GNV(J,K,2) = GSN2(J)
         GNV(J,K,3) = GSN3(J)
  110    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE GETIP (JSMAX,KSMAX,M1D,JD,JSPER,KSPER,IBCJ,IB,
     &                  JBMAX,JCUR,KCUR,DELXI,DELET,
     &                  JSP,JSR,KSP,KSR,XS,YS,ZS,XI,YI,ZI)
C
C    Store interpolation points in xi,yi,zi arrays.
C
#include "precis.h"
C
      DIMENSION JSP(JSMAX), JSR(JSMAX), KSP(KSMAX), KSR(KSMAX)
      DIMENSION XS(JSMAX,KSMAX), YS(JSMAX,KSMAX), ZS(JSMAX,KSMAX)
      DIMENSION JBMAX(JD), XI(M1D,JD), YI(M1D,JD), ZI(M1D,JD)
C
C
      DXIM      = 1.0 - DELXI
      DETAM     = 1.0 - DELET
      JCURP     = JSP(JCUR)
      KCURP     = KSP(KCUR)
C
      IF ( (IBCJ.EQ.13) .OR. (IBCJ.EQ.14) .OR.
     &     (IBCJ.EQ.33) .OR. (IBCJ.EQ.34) ) THEN
C
         IF (KSPER.EQ.0) THEN
            I         = 0
            IF      ( (IBCJ.EQ.13) .OR. (IBCJ.EQ.33) ) THEN
               KBEG      = KCUR
               KEND      = KSMAX
               KINC      = 1
            ELSE IF ( (IBCJ.EQ.14) .OR. (IBCJ.EQ.34) ) THEN
               KBEG      = KCURP
               KEND      = 1
               KINC      = -1
            ENDIF
            DO 10 KS = KBEG,KEND,KINC
               I         = I+1
               JBMAX(IB) = JBMAX(IB) + 1
               XI(I,IB)  = DXIM*XS(JCUR,KS) + DELXI*XS(JCURP,KS)
               YI(I,IB)  = DXIM*YS(JCUR,KS) + DELXI*YS(JCURP,KS)
               ZI(I,IB)  = DXIM*ZS(JCUR,KS) + DELXI*ZS(JCURP,KS)
   10          CONTINUE
C
         ELSE IF (KSPER.EQ.1) THEN
            JBMAX(IB) = KSMAX
            IF      ( (IBCJ.EQ.13) .OR. (IBCJ.EQ.33) ) THEN
               KBEG      = KCUR
               KS        = KSR(KBEG)
               DO 20 I = 1,KSMAX
                  KS        = KSP(KS)
                  XI(I,IB)  = DXIM*XS(JCUR,KS) + DELXI*XS(JCURP,KS)
                  YI(I,IB)  = DXIM*YS(JCUR,KS) + DELXI*YS(JCURP,KS)
                  ZI(I,IB)  = DXIM*ZS(JCUR,KS) + DELXI*ZS(JCURP,KS)
   20             CONTINUE
            ELSE IF ( (IBCJ.EQ.14) .OR. (IBCJ.EQ.34) ) THEN
               KBEG      = KCURP
               KS        = KSP(KBEG)
               DO 30 I = 1,KSMAX
                  KS        = KSR(KS)
                  XI(I,IB)  = DXIM*XS(JCUR,KS) + DELXI*XS(JCURP,KS)
                  YI(I,IB)  = DXIM*YS(JCUR,KS) + DELXI*YS(JCURP,KS)
                  ZI(I,IB)  = DXIM*ZS(JCUR,KS) + DELXI*ZS(JCURP,KS)
   30             CONTINUE
            ENDIF
         ENDIF
C
      ELSE IF ( (IBCJ.EQ.11) .OR. (IBCJ.EQ.12) .OR.
     &          (IBCJ.EQ.31) .OR. (IBCJ.EQ.32) ) THEN
C
         IF (JSPER.EQ.0) THEN
            I         = 0
            IF ( (IBCJ.EQ.11) .OR. (IBCJ.EQ.31) ) THEN
               JBEG      = JCUR
               JEND      = JSMAX
               JINC      = 1
            ELSE IF ( (IBCJ.EQ.12) .OR. (IBCJ.EQ.32) ) THEN
               JBEG      = JCURP
               JEND      = 1
               JINC      = -1
            ENDIF
            DO 40 JS = JBEG,JEND,JINC
               I         = I+1
               JBMAX(IB) = JBMAX(IB) + 1
               XI(I,IB)  = DETAM*XS(JS,KCUR) + DELET*XS(JS,KCURP)
               YI(I,IB)  = DETAM*YS(JS,KCUR) + DELET*YS(JS,KCURP)
               ZI(I,IB)  = DETAM*ZS(JS,KCUR) + DELET*ZS(JS,KCURP)
   40          CONTINUE
C
         ELSE IF (JSPER.EQ.1) THEN
            JBMAX(IB) = JSMAX
            IF      ( (IBCJ.EQ.11) .OR. (IBCJ.EQ.31) ) THEN
               JBEG      = JCUR
               JS        = JSR(JBEG)
               DO 50 I = 1,JSMAX
                  JS        = JSP(JS)
                  XI(I,IB)  = DETAM*XS(JS,KCUR) + DELET*XS(JS,KCURP)
                  YI(I,IB)  = DETAM*YS(JS,KCUR) + DELET*YS(JS,KCURP)
                  ZI(I,IB)  = DETAM*ZS(JS,KCUR) + DELET*ZS(JS,KCURP)
   50             CONTINUE
            ELSE IF ( (IBCJ.EQ.12) .OR. (IBCJ.EQ.32) ) THEN
               JBEG      = JCURP
               JS        = JSP(JBEG)
               DO 60 I = 1,JSMAX
                  JS        = JSR(JS)
                  XI(I,IB)  = DETAM*XS(JS,KCUR) + DELET*XS(JS,KCURP)
                  YI(I,IB)  = DETAM*YS(JS,KCUR) + DELET*YS(JS,KCURP)
                  ZI(I,IB)  = DETAM*ZS(JS,KCUR) + DELET*ZS(JS,KCURP)
   60             CONTINUE
            ENDIF
         ENDIF
C
      ENDIF
C
C    Add extra distance at end of curve to march on in case we go off
C    the end.
C
      IF ( (JSPER.EQ.0) .OR. (KSPER.EQ.0) ) THEN
         IF (JBMAX(IB).GE.2) THEN
            EXT          = 100.0
            JEND         = JBMAX(IB)
            JENDP        = JEND + 1
            XI(JENDP,IB) = XI(JEND,IB) + EXT*(XI(JEND,IB)-XI(JEND-1,IB))
            YI(JENDP,IB) = YI(JEND,IB) + EXT*(YI(JEND,IB)-YI(JEND-1,IB))
            ZI(JENDP,IB) = ZI(JEND,IB) + EXT*(ZI(JEND,IB)-ZI(JEND-1,IB))
            JBMAX(IB)    = JBMAX(IB) + 1
         ENDIF
      ENDIF
C
C
      RETURN
      END

#ifdef SUPERLIB
C***********************************************************************
      SUBROUTINE IGSPRN(JMAX,K,X,Y,Z,GSN1,GSN2,GSN3,UI,VI,IMODEL)
C
#include "precis.h"
C
      DOUBLE PRECISION MSPT(3), SRFPT(3), UV(2), R3DIST, VEC(9)
C
      DIMENSION X(JMAX), Y(JMAX), Z(JMAX),
     &          GSN1(JMAX), GSN2(JMAX), GSN3(JMAX), UI(JMAX), VI(JMAX)
C
C    Project a K-line of points on to NIGES surface and find local normals
C
      IGSTYP = 1
      NNURB  = 1
      ISURF = IGS_MODEL_NTH (IMODEL,IGSTYP,NNURB)
C      WRITE(*,*) 'ISURF ',ISURF
C
      IF (K.EQ.1) THEN
C
       DO 10 J=1,JMAX
C
        IF (J.EQ.1) THEN
         IEFLAG = 0
        ELSE
         IEFLAG = 1
        ENDIF
        MSPT(1) = X(J)
        MSPT(2) = Y(J)
        MSPT(3) = Z(J)
c        write(*,*)'j,k,u,v,ieflag ',j,k,uv(1),uv(2),ieflag
        CALL IGS_SURF_PROJ (ISURF, MSPT, SRFPT, UV, R3DIST, IEFLAG)
        X(J)    = SRFPT(1)
        Y(J)    = SRFPT(2)
        Z(J)    = SRFPT(3)
        UI(J)   = UV(1)
        VI(J)   = UV(2)
C
        CALL IGS_SURF_NOR (ISURF, UV(1), UV(2), VEC, IEFLAG)
        GSN1(J) = VEC(7)
        GSN2(J) = VEC(8)
        GSN3(J) = VEC(9)
C        WRITE(*,*) J,X(J),Y(J),Z(J),GSN1(J),GSN2(J),GSN3(J)
C
 10    CONTINUE
C
      ELSE IF (K.GT.1) THEN
C
       DO 20 J=1,JMAX
C
        IEFLAG = 1
        MSPT(1) = X(J)
        MSPT(2) = Y(J)
        MSPT(3) = Z(J)
        UV(1)   = UI(J)
        UV(2)   = VI(J)
c        write(*,*)'j,k,u,v,ieflag ',j,k,uv(1),uv(2),ieflag
        CALL IGS_SURF_PROJ (ISURF, MSPT, SRFPT, UV, R3DIST, IEFLAG)
        X(J)    = SRFPT(1)
        Y(J)    = SRFPT(2)
        Z(J)    = SRFPT(3)
        UI(J)   = UV(1)
        VI(J)   = UV(2)
C
        CALL IGS_SURF_NOR (ISURF, UV(1), UV(2), VEC, IEFLAG)
        GSN1(J) = VEC(7)
        GSN2(J) = VEC(8)
        GSN3(J) = VEC(9)
C        WRITE(*,*) J,X(J),Y(J),Z(J),GSN1(J),GSN2(J),GSN3(J)
C
 20    CONTINUE
C
      ENDIF
C
      RETURN
      END
#endif

C***********************************************************************
      SUBROUTINE INIFIX(M1D,NRGRD,NTOTR,JMAX,KMAX,JKD,NGPR,JRMAX,KRMAX,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,J,IBCJ,IAFAM,NGBC,
     &                  IB,JBMAX,JRP,JRR,KRP,KRR,
     &                  X,Y,Z,XR,YR,ZR,
     &                  IGPB,IRFAM,XB,YB,ZB)
C
C   Set up interpolation points for fixed boundary BC.
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION JBMAX(2), XB(M1D,2), YB(M1D,2), ZB(M1D,2)
C
C   Number of close points we're willing to consider.
C
      PARAMETER (MSAVE=1)
      DIMENSION JSAVE(MSAVE),KSAVE(MSAVE),NGSAVE(MSAVE)
      PARAMETER (I0=0,I1=1,I2=2)
C
C
      K         = 1
      JBMAX(1)  = 0
      JBMAX(2)  = 0
C
C   Find quad in which the boundary point lies.
C
      CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &            JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
     &            NGBC,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &            XR,YR,ZR,X(J,K),Y(J,K),Z(J,K))
      NS        = NGSAVE(1)
      JS        = JSAVE(1)
      KS        = KSAVE(1)
C
#ifdef DEBUG
      WRITE(*,*)
      WRITE(*,*) 'calling cells from inifix'
      WRITE(*,*) ' initial point (j,k)        ',J,K
      WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
      CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &            JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &            X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &            IGPB,I1,IQSTAT)
#ifdef DEBUG
      WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
      WRITE(*,*)
#endif
C
C   Store points in xb,yb,zb arrays for boundary to get interpolations.
C
      IS = NGPR(NS)
      CALL GETIP (JRMAX(NS),KRMAX(NS),M1D,I2,JRPER(NS),KRPER(NS),
     &            IBCJ,IB,JBMAX,JS,KS,DELXI,DELET,
     &            JRP(1,NS),JRR(1,NS),KRP(1,NS),KRR(1,NS),
     &            XR(IS),YR(IS),ZR(IS),XB,YB,ZB)
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INIGRD(NRGRD,NRCUR,NAGRD,NTOTR,NTOTC,NTOT,IRSTYP,
     &                  NGPR,NGNR,NGPC,JCMAX,NGP,JMAX,INIC,
     &                  XR,YR,ZR,XC,YC,ZC,X,Y,Z)
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), NGPC(NRCUR), JCMAX(NRCUR),
     &          NGP(NAGRD), JMAX(NAGRD), INIC(NAGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XC(NTOTC), YC(NTOTC), ZC(NTOTC)
      DIMENSION X(NTOT), Y(NTOT), Z(NTOT)
C
C    Read user defined body surface
C
      IF (IRSTYP.EQ.1) THEN
C
c*wdh       DO 10 N=1,NRGRD
c*wdh        IS = NGPR(N)
c*wdh        IE = IS + NGNR(N) - 1
c*wdh        READ(2) (XR(I),I=IS,IE),(YR(I),I=IS,IE),(ZR(I),I=IS,IE)
c*wdh 10    CONTINUE
C
      ENDIF
C
C    Read reference curves.
C
c*wdh      DO 20 N=1,NRCUR
c*wdh       IS = NGPC(N)
c*wdh       IE = IS + JCMAX(N) - 1
c*wdh       READ(4) (XC(I),I=IS,IE),(YC(I),I=IS,IE),(ZC(I),I=IS,IE)
c*wdh 20   CONTINUE
C
C    Load initial curves.
C
      DO 30 N=1,NAGRD
         ICNUM = ABS(INIC(N))
         IS    = NGP(N)
         IE    = IS + JMAX(N) - 1
         ISRC  = NGPC(ICNUM)
         IF (INIC(N).GT.0) THEN
            IADD = ISRC - IS
            DO 40 I=IS,IE
               II   = IADD + I
               X(I) = XC(II)
               Y(I) = YC(II)
               Z(I) = ZC(II)
 40         CONTINUE
         ELSE
            IADD = ISRC + JMAX(N) - 1 + IS
            DO 50 I=IS,IE
               II   = IADD - I
               X(I) = XC(II)
               Y(I) = YC(II)
               Z(I) = ZC(II)
 50         CONTINUE
         ENDIF
 30   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INIINT(M1D,NRGRD,NTOTR,JKD,JMAX,KMAX,NGPR,JRMAX,KRMAX,
     &                  IBCJA,IAFAM,NGBC,JRPER,KRPER,JRAXSA,JRAXSB,
     &                  JRP,JRR,KRP,KRR,XR,YR,ZR,X,Y,Z,IGPB,
     &                  IRFAM,JBMAX,ICUR,XI,YI,ZI)
C
C   Determine xi_s and eta_s of each point on initial line and
C   store string of points for constant xi_s or eta_s line BC's.
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION JBMAX(JMAX), ICUR(JMAX)
      DIMENSION XI(JKD,JMAX), YI(JKD,JMAX), ZI(JKD,JMAX)
C
C   Number of close points we're willing to consider.
C
      PARAMETER (MSAVE=1)
      DIMENSION JSAVE(MSAVE),KSAVE(MSAVE),NGSAVE(MSAVE)
      PARAMETER (I0=0,I1=1)
      PARAMETER (TOLS=1.E-3)
C
C
      K      = 1
C
C   Initialize arrays.
C
      DO 10 J = 1,JMAX
         JBMAX(J)  = 0
         ICUR(J)   = 1
   10    CONTINUE
C
C   Find quad in which each point of the initial line lies.
C
      DO 20 J = 1,JMAX
         CALL FNDNRP(NRGRD,NTOTR,NGPR,JRMAX,KRMAX,
     &               JSAVE,KSAVE,NGSAVE,NSAVE,MSAVE,
     &               NGBC,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               XR,YR,ZR,X(J,K),Y(J,K),Z(J,K))
         NS     = NGSAVE(1)
         JS     = JSAVE(1)
         KS     = KSAVE(1)
         IC     = 1
C
#ifdef DEBUG
         WRITE(*,*)
         WRITE(*,*) 'calling cells from iniint'
         WRITE(*,*) ' initial point (j,k)        ',J,K
         WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
         CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &               JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &               X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &               IGPB,I1,IQSTAT)
#ifdef DEBUG
         WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
         WRITE(*,*)
#endif
C
         IF (IBCJA.EQ.31 .AND. ABS(DELXI-1.0).LT.TOLS) THEN
            IF (JRPER(NS).EQ.1 .OR. JS.LT.JRMAX(NS)-1) THEN
               DELXI  = DELXI - 1.0
               JS     = JRP(JS,NS)
            ENDIF
         ENDIF
         IF (IBCJA.EQ.32 .AND. ABS(DELXI).LT.TOLS) THEN
            IF (JRPER(NS).EQ.1 .OR. JS.GT.1) THEN
               DELXI  = DELXI + 1.0
               JS     = JRR(JS,NS)
            ENDIF
         ENDIF
         IF (IBCJA.EQ.33 .AND. ABS(DELET-1.0).LT.TOLS) THEN
            IF (KRPER(NS).EQ.1 .OR. KS.LT.KRMAX(NS)-1) THEN
               DELET  = DELET - 1.0
               KS     = KRP(KS,NS)
            ENDIF
         ENDIF
         IF (IBCJA.EQ.34 .AND. ABS(DELET).LT.TOLS) THEN
            IF (KRPER(NS).EQ.1 .OR. KS.GT.1) THEN
               DELET  = DELET + 1.0
               KS     = KRR(KS,NS)
            ENDIF
         ENDIF
C
         IS = NGPR(NS)
         CALL GETIP (JRMAX(NS),KRMAX(NS),JKD,JMAX,JRPER(NS),KRPER(NS),
     &               IBCJA,J,JBMAX,JS,KS,DELXI,DELET,
     &               JRP(1,NS),JRR(1,NS),KRP(1,NS),KRR(1,NS),
     &               XR(IS),YR(IS),ZR(IS),XI,YI,ZI)
   20    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INIPAR(REFSFN,REFCFN,OUTGFN,M1D,M2D,MRGRD,MNOD,
     &                  NRGRD,NRCUR,NAGRD,NTOTR,NTOTC,NTOT,
     &                  NGPR,NGNR,NGP1,NGP,NGN,JRMAX,KRMAX,IRFAM,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,
     &                  JMAX,KMAX,NGPC,JCMAX,INIC,JKD,IRSTYP,IMODEL,
     &                  IBCJA,IBCJB,IAFAM,NGBCA,NGBCB,
     &                  NNOD,SMU,TIM,ITSVOL,
     &                  JNOD,ETAMX,DETA,DFAR,IAUC,ISGRI,ISGRB)
C
#include "precis.h"
C
C    Read headers from reference surface file, reference curves file,
C    read input parameters file.
C
      LOGICAL FIRSTL
      CHARACTER*80 REFSFN, REFCFN, OUTGFN, TMPSTR
      CHARACTER*80 TEXT(3)
      CHARACTER*120 LINE
C
      DIMENSION NGPR(MRGRD), NGNR(MRGRD), NGP1(MRGRD), 
     &          NGP(MRGRD), NGN(MRGRD)
      DIMENSION JRMAX(MRGRD), KRMAX(MRGRD), IRFAM(MRGRD),
     &          JRPER(MRGRD), KRPER(MRGRD), JRAXSA(MRGRD), JRAXSB(MRGRD)
      DIMENSION JMAX(MRGRD), KMAX(MRGRD), NGPC(MRGRD), JCMAX(MRGRD),
     &          INIC(MRGRD), IBCJA(MRGRD), IBCJB(MRGRD),  IAFAM(MRGRD),
     &          NGBCA(MRGRD), NGBCB(MRGRD), NNOD(MRGRD),
     &          SMU(MRGRD), TIM(MRGRD), ITSVOL(MRGRD)
      DIMENSION JNOD(MNOD,MRGRD), ETAMX(MNOD,MRGRD), DETA(MNOD,MRGRD),
     &          DFAR(MNOD,MRGRD)
C
C-----------------------------------------------------------------------
C
C   Write header
C
C-----------------------------------------------------------------------
C
      WRITE(*,2)
      WRITE(*,3)
c*wdh print date compiled:
      CALL CMPLTM
      WRITE(*,2)
 2    FORMAT('********************************************************')
 3    FORMAT('           Output from SURGRD version 7.0d              ')
C
C-----------------------------------------------------------------------
C
C   Read reference surface, reference curves, output grid filenames
C
C-----------------------------------------------------------------------
C
c*wdh      CALL RDFNAM(REFSFN,REFCFN,OUTGFN,FIRSTL,LINE)
        firstl=.TRUE.   
C
C-----------------------------------------------------------------------
C
C   Open reference surface file and determine reference surface type
C
C-----------------------------------------------------------------------
C
c*wdh      OPEN (UNIT=2,FILE=REFSFN,STATUS='UNKNOWN',FORM='FORMATTED')
c*wdh      READ(2,1,ERR=5,END=5) TMPSTR
c*wdh 1    FORMAT(A80)
c*wdh      IF (TMPSTR(73:73).NE.'S') GO TO 5
c*wdh      IRSTYP = 2
c*wdh      GO TO 6
c*wdh 5    CONTINUE
c*wdh      IRSTYP = 1
c*wdh 6    CONTINUE
c*wdh      CLOSE(2)

c*wdh : 2 : "nurb surface"
      irstyp = 2

C
C-----------------------------------------------------------------------
C    Open files
C-----------------------------------------------------------------------
C
C    Reference surface file
C
      IF (IRSTYP.EQ.1) THEN
C
       WRITE(*,8)
 8     FORMAT('Reference surface format:  Multiple Panel Networks.',/)
c*wdh       OPEN (UNIT=2,FILE=REFSFN,STATUS='UNKNOWN',FORM='UNFORMATTED')
C
      ELSE IF (IRSTYP.EQ.2) THEN
C
       WRITE(*,9)
 9     FORMAT('Reference surface format:  NIGES.',/)
c*wdh       IMDLOK = 0
       IMDLOK = 1
C
#ifdef SUPERLIB
               IMODEL = IGS_MODEL_READ (REFSFN, LEN(REFSFN))
               IMDLOK = IMODEL
#endif
C
       IF (IMDLOK .EQ. 0) THEN
        WRITE (*,*) 'Cannot use iges model. It is invalid.'
        STOP
       ENDIF
C
      ENDIF
C
C    Initial curves in PLOT3D format
C
c*wdh      OPEN (UNIT=4,FILE=REFCFN,STATUS='UNKNOWN',FORM='UNFORMATTED')
C
C    Output PLOT3D grid file
C
c*wdh      OPEN (UNIT=3,FILE=OUTGFN,STATUS='UNKNOWN',FORM='UNFORMATTED')
C
C-----------------------------------------------------------------------
C   Read dimensions of reference curves.
C-----------------------------------------------------------------------
C
C   Test for single or multiple grid format.
C
      ISGRI  = 1
c*wdh      READ(4,ERR=10,END=10) JDUM,KDUM,LDUM
      GO TO 20
C
   10 CONTINUE
      ISGRI  = 0
C
   20 CONTINUE
c*wdh      REWIND(4)
c*wdh      IF (ISGRI.EQ.0) THEN
c*wdh         READ(4) NRCUR
c*wdh      ELSE
c*wdh         NRCUR = 1
c*wdh      ENDIF
C
      nrcur = 1

      IF (NRCUR.GT.MRGRD) THEN
         WRITE(TEXT,21) NRCUR
   21    FORMAT(' Max number of reference curves exceeded.'/
     &          ' Recompile with MRGRD = ',I10)
         NTEXT  = 2
         CALL ERRMSG(TEXT,NTEXT)
         GOTO 1050
      ENDIF
c*wdh      READ(4) (JCMAX(N),KDUM,LDUM,N=1,NRCUR)
C
C-----------------------------------------------------------------------
C   Read dimensions of input surface grid for projection.
C-----------------------------------------------------------------------
C
      IF (IRSTYP.EQ.1) THEN
C
C      Test for single or multiple grid format.
C
        ISGRB  = 1
c*wdh        READ(2,ERR=30,END=30) JDUM,KDUM,LDUM
        GO TO 40
C
   30   CONTINUE
        ISGRB  = 0
C
   40   CONTINUE
c*wdh        REWIND(2)
        IF (ISGRB.EQ.0) THEN
c*wdh           READ(2) NRGRD
           nrgrd=1
        ELSE
           NRGRD  = 1
        ENDIF
C
        IF (NRGRD.GT.MRGRD) THEN
           WRITE(TEXT,41) NRGRD
   41      FORMAT(' Max number of reference grids exceeded.'/
     &            ' Recompile with MRGRD = ',I10)
           NTEXT  = 2
           CALL ERRMSG(TEXT,NTEXT)
           GOTO 1050
        ENDIF
c*wdh        READ(2) (JRMAX(N),KRMAX(N),LRMAX,N=1,NRGRD)
C
      ELSE IF (IRSTYP.EQ.2) THEN
C
C      Read reference surface from niges file using external library
C
        NRGRD = 1
        JRMAX(1) = 1
        KRMAX(1) = 1
C
      ENDIF
C
C-----------------------------------------------------------------------
C   Read input parameters for each grid.
C-----------------------------------------------------------------------
C
      WRITE(*,48) NRCUR
 48   FORMAT(' Number of reference curves       = ',I5,/)
C
      CALL RDINPA(FIRSTL,LINE,MRGRD,MNOD,NRGRD,NRCUR,NAGRD,
     &            INIC,JMAX,JCMAX,IRFAM,JRAXSA,JRAXSB,JRPER,KRPER,
     &            IBCJA,IBCJB,IAFAM,NGBCA,NGBCB,
     &            KMAX,NNOD,JNOD,ETAMX,DETA,DFAR,SMU,TIM,ITSVOL,
     &            NSPBC,IAUC,NFAM)
C
C-----------------------------------------------------------------------
C   Determine dimensions of arrays.
C-----------------------------------------------------------------------
C
      JKD    = 0
      M2DG   = 0
      NTOTR  = 0
      NTOTC  = 0
      NTOT   = 0
      DO 240 N = 1,NRGRD
         NP     = JRMAX(N)*KRMAX(N)
         NGNR(N)= NP
         NGPR(N)= 2*NTOTR + 1
         NGP1(N)= NTOTR + 1
         NTOTR  = NTOTR + NP
         JKD    = MAX( JKD, JRMAX(N), KRMAX(N) )
         M2DG   = MAX( M2DG, NP )
  240    CONTINUE
      DO 250 N = 1,NAGRD
         N2D    = JMAX(N)*KMAX(N)
         NGN(N) = N2D
         NGP(N) = NTOT + 1
         NTOT   = NTOT + N2D
         JKD    = MAX( JKD, JMAX(N), KMAX(N) )
         M2DG   = MAX( M2DG, N2D )
  250    CONTINUE
      DO 245 N = 1,NRCUR
         NGPC(N) = NTOTC + 1
         NTOTC   = NTOTC + JCMAX(N)
         JKD     = MAX( JKD, JCMAX(N) )
 245  CONTINUE
      WRITE(*,251) NTOT
  251 FORMAT(' Total number of grid points = ',I6,/)
C
C   Print error message if max grid dimension exceeded.
C
      IF (JKD.GT.M1D) THEN
         WRITE(TEXT,252) JKD
  252    FORMAT(' Max dimension M1D exceeded.'/
     &          ' M1D has to be at least ',I10,' for current case.'/
     &          ' Program terminated.')
         NTEXT  = 3
         CALL ERRMSG(TEXT,NTEXT)
         GOTO 1050
      ENDIF
      IF (M2DG.GT.M2D) THEN
         WRITE(TEXT,253) M2DG
  253    FORMAT(' Max dimension M2D exceeded.'/
     &          ' M2D has to be at least ',I10,' for current case.'/
     &          ' Program terminated.')
         NTEXT  = 3
         CALL ERRMSG(TEXT,NTEXT)
         GOTO 1050
      ENDIF
C
C   Convert bc types to switches.
C
      DO 260 N = 1,NRGRD
         IF (JRAXSA(N).EQ.1) THEN
            JBC    = JRPER(N)
            IF ( (JBC.LT.0 .OR. JBC.GT.3) .AND. (JBC.NE.10) ) THEN
               WRITE(*,254) N
  254          FORMAT(' Warning: Invalid bc type in J for grid ',I5/
     &                '          Reset to type 0 (non-periodic).')
               JBC    = 0
            ENDIF
C
            IF (JBC.NE.10) THEN
               JRPER(N)  = 0
            ELSE
               JRPER(N)  = 1
            ENDIF
C
            IF (JBC.EQ.1 .OR. JBC.EQ.3) THEN
               JRAXSA(N) = 1
            ELSE
               JRAXSA(N) = 0
            ENDIF
C
            IF (JBC.EQ.2 .OR. JBC.EQ.3) THEN
               JRAXSB(N) = 1
            ELSE
               JRAXSB(N) = 0
            ENDIF
C
            IF (KRPER(N).NE.0 .AND. KRPER(N).NE.10) THEN
               WRITE(*,255) N
  255          FORMAT(' Warning: Invalid bc type in K for grid ',I5/
     &                '          Reset to type 0 (non-periodic).')
               IF (KRPER(N).NE.10) THEN
                  KRPER(N)  = 0
               ELSE
                  KRPER(N)  = 1
               ENDIF
            ENDIF
C
         ELSE
            JRPER(N)  = -1
            KRPER(N)  = -1
         ENDIF
  260    CONTINUE
C
C
      RETURN
C
C   Error on input.
C
 1010 CONTINUE
      TEXT(1)= 'Error on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Premature end-of-file on input.
C
 1020 CONTINUE
      TEXT(1)= 'Premature end-of-file on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Stop due to error.
C
 1050 CONTINUE
      CALL ERREND
      STOP 'INIPAR'
C
      END
C***********************************************************************
      SUBROUTINE INITIA(M1D,NRGRD,NAGRD,NTOTR,NGPR,NGNR,NGP1,
     &                  JRMAX,KRMAX,IBCJA,IBCJB,NGBCA,NGBCB,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,IRSTYP,
     &                  JRP,JRR,KRP,KRR,XR,YR,ZR,
     &                  XNR,YNR,ZNR,XNRU,YNRU,ZNRU,IGPB,IRFAM,BMM)
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), NGP1(NRGRD),
     &          JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION IBCJA(NAGRD), IBCJB(NAGRD), NGBCA(NAGRD), NGBCB(NAGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD), JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD), KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XNR(NTOTR), YNR(NTOTR), ZNR(NTOTR)
      DIMENSION XNRU(NTOTR), YNRU(NTOTR), ZNRU(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
      DIMENSION BMM(NRGRD,6)
C
      CHARACTER*80 TEXT(2)
C
C
C   Do some checks on BC type and grid number.
C
      DO 10 N = 1,NAGRD
         IF (IBCJA(N).LT.-1000 .OR. IBCJA(N).EQ.0 .OR. IBCJA(N).GT.34)
     &                                                              THEN
            WRITE(TEXT,1) N
    1       FORMAT('Error in input, invalid BC type for IBCJA, grid ',
     &             I5)
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ELSE IF (IBCJA(N).GE.7 .AND. IBCJA(N).LE.9) THEN
            WRITE(TEXT,1) N
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ELSE IF (IBCJA(N).GE.16 .AND. IBCJA(N).LE.19) THEN
            WRITE(TEXT,1) N
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ELSE IF (IBCJA(N).GE.24 .AND. IBCJA(N).LE.30) THEN
            WRITE(TEXT,1) N
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ENDIF
C
         IF (IBCJB(N).LT.-1000 .OR. IBCJB(N).EQ.0 .OR. IBCJB(N).GT.34)
     &                                                              THEN
            WRITE(TEXT,2) N
    2       FORMAT('Error in input, invalid BC type for IBCJB, grid ',
     &             I5)
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ELSE IF (IBCJB(N).GE.7 .AND. IBCJB(N).LE.9) THEN
            WRITE(TEXT,2) N
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ELSE IF (IBCJB(N).GE.16 .AND. IBCJB(N).LE.19) THEN
            WRITE(TEXT,2) N
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ELSE IF (IBCJB(N).GE.24 .AND. IBCJB(N).LE.30) THEN
            WRITE(TEXT,2) N
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ENDIF
C
         IF (     (IBCJA(N).EQ.10 .AND. IBCJB(N).NE.10)
     &       .OR. (IBCJB(N).EQ.10 .AND. IBCJA(N).NE.10) ) THEN
            WRITE(TEXT,3) N
    3       FORMAT('Inconsistent BC type: periodic BC must be applied'/
     &             '     to both boundaries or neither for grid ',I5)
            NTEXT  = 2
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ENDIF
C
         IBADBC = 0
         IF (     (IBCJA(N).EQ.21 .AND. IBCJB(N).NE.21)
     &       .OR. (IBCJB(N).EQ.21 .AND. IBCJA(N).NE.21) ) IBADBC = 1
         IF (     (IBCJA(N).EQ.22 .AND. IBCJB(N).NE.22)
     &       .OR. (IBCJB(N).EQ.22 .AND. IBCJA(N).NE.22) ) IBADBC = 1
         IF (     (IBCJA(N).EQ.23 .AND. IBCJB(N).NE.23)
     &      .OR.  (IBCJB(N).EQ.23 .AND. IBCJA(N).NE.23) ) IBADBC = 1
         IF (     (IBCJA(N).EQ.31 .AND. IBCJB(N).NE.31)
     &       .OR. (IBCJB(N).EQ.31 .AND. IBCJA(N).NE.31) ) IBADBC = 1
         IF (     (IBCJA(N).EQ.32 .AND. IBCJB(N).NE.32)
     &       .OR. (IBCJB(N).EQ.32 .AND. IBCJA(N).NE.32) ) IBADBC = 1
         IF (     (IBCJA(N).EQ.33 .AND. IBCJB(N).NE.33)
     &       .OR. (IBCJB(N).EQ.33 .AND. IBCJA(N).NE.33) ) IBADBC = 1
         IF (     (IBCJA(N).EQ.34 .AND. IBCJB(N).NE.34)
     &       .OR. (IBCJB(N).EQ.34 .AND. IBCJA(N).NE.34) ) IBADBC = 1
         IF (IBADBC.EQ.1) THEN
            WRITE(TEXT,4) N
    4       FORMAT('Inconsistent BC type: const. plane or xi/eta for ',
     &             'all J must be applied'/
     &             '     to both boundaries or neither for grid ',I5)
            NTEXT  = 2
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1010
         ENDIF
C
   10    CONTINUE
C
C
C-----------------------------------------------------------------------
C   Determine bc's for reference surface grids and adjust indices if
C   needed.
C
      IF (IRSTYP.EQ.1) THEN
C
C   Use the second level of arrays XR,YR,ZR as temporary space in case
C   we need to swap J and K here.  
C
      CALL SURBCS(M1D,NRGRD,NTOTR,NGPR,NGNR,JRMAX,KRMAX,
     &            JRPER,KRPER,JRAXSA,JRAXSB,JRP,JRR,KRP,KRR,IRFAM,
     &            XR,YR,ZR)
C
C   Set up IGPB pointer array that shows the grid number and the J,K
C   index of the neighboring grid for each point on the boundary of
C   each reference grid.
C
C   Use the arrays XNRU,YNRU,ZNRU,XNR,YNR (which will hold reference
C   grid surface normals) temporarily to hold minmax box values.
C
      CALL SURNHB(M1D,NRGRD,NTOTR,NGPR,NGNR,NGP1,
     &            JRMAX,KRMAX,JRPER,KRPER,JRAXSA,JRAXSB,XR,YR,ZR,
     &            BMM(1,1),BMM(1,2),BMM(1,3),BMM(1,4),BMM(1,5),BMM(1,6),
     &            XNRU,YNRU,ZNRU,XNR,YNR,ZNR,IGPB,IRFAM)
C
C   Compute surface normals at cell vertices.
C
      DO 40 N = 1,NRGRD
         IS  = NGPR(N)
         IS1 = NGP1(N)
         CALL SNROLD(JRMAX(N),KRMAX(N),XR(IS),YR(IS),ZR(IS),
     &               XNRU(IS1),YNRU(IS1),ZNRU(IS1),
     &               JRP(1,N),JRR(1,N),KRP(1,N),KRR(1,N))
   40    CONTINUE
C
C   Copy surface normals to (XNR,YNR,ZNR).
C
      DO 45 N = 1,NRGRD
         IS1 = NGP1(N)
         CALL SURCPY(JRMAX(N),KRMAX(N),XNRU(IS1),YNRU(IS1),ZNRU(IS1),
     &               XNR(IS1),YNR(IS1),ZNR(IS1))
   45    CONTINUE
C
C   Modify surface normals at grid boundaries to match with neighbors.
C
      IF (NRGRD.GT.1) THEN
       DO 50 N = 1,NRGRD
          CALL SURNMM(NRGRD,M1D,NTOTR,NGP1,JRMAX,KRMAX,
     &                JRPER(N),KRPER(N),JRAXSA(N),JRAXSB(N),N,
     &                XNRU,YNRU,ZNRU,XNR,YNR,ZNR,IGPB,IRFAM)
   50  CONTINUE
      ENDIF
C
C   Generate a second (offset) layer of grid points so that we have a
C   layer of grid cells.
C
      DO 60 N = 1,NRGRD
         IS  = NGPR(N)
         IS1 = NGP1(N)
         CALL OFFSUR(XR(IS),YR(IS),ZR(IS),JRMAX(N),KRMAX(N),
     &               XNR(IS1),YNR(IS1),ZNR(IS1))
   60    CONTINUE
C
      ENDIF
C
      RETURN
C
C   Stop after an error.
C
 1010 CONTINUE
      CALL ERREND
      STOP 'INITIA'
C
C
      END
C***********************************************************************
      SUBROUTINE INITS (M1D,JKD,NRGRD,NRCUR,NTOTR,NTOTC,IG,JMAX,KMAX,
     &                  NGPR,NGNR,NGP1,NGPC,JRMAX,KRMAX,JCMAX,
     &                  J1,J2,JPER,HYSTEP,IRSTYP,IMODEL,
     &                  IBCJA,IBCJB,IAFAM,NGBCA,NGBCB,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,
     &                  JPLNA,JPLNB,JJP,JJR,JRP,JRR,KRP,KRR,
     &                  NNOD,JNOD,ETAMX,DETA,DFAR,TMP1,XI,YI,ZI,
     &                  X,Y,Z,XR,YR,ZR,XC,YC,ZC,R,RH,ETAVAR,
     &                  XNRU,YNRU,ZNRU,IGPB,IRFAM,XB,YB,ZB,ISEG,
     &                  GNV,GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,
     &                  JKIH,DELS,JBMAX,ICUR)
C
#include "precis.h"
C
      LOGICAL HYSTEP
      DIMENSION JPLNA(3), JPLNB(3), JJP(JMAX), JJR(JMAX)
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), NGP1(NRGRD),
     &          JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XNRU(NTOTR), YNRU(NTOTR), ZNRU(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
C
      DIMENSION NGPC(NRCUR),JCMAX(NRCUR),XC(NTOTC),YC(NTOTC),ZC(NTOTC)
C
      DIMENSION JNOD(NNOD), ETAMX(NNOD), DETA(NNOD), DFAR(NNOD)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION TMP1(M1D), XI(JKD*JKD), YI(JKD*JKD), ZI(JKD*JKD)
      DIMENSION R(JMAX,KMAX), RH(JMAX,3), ETAVAR(JMAX,3)
      DIMENSION XB(M1D,2), YB(M1D,2), ZB(M1D,2), ISEG(2)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX),
     &          XSNR(JMAX), YSNR(JMAX), ZSNR(JMAX)
      DIMENSION JKIH(JMAX,3), DELS(JMAX,3), JBMAX(JMAX), ICUR(JMAX)
      DIMENSION GNV(JMAX,KMAX,3)
C
      PARAMETER (ISF=2,RMAX=0.)
      PARAMETER (I1=1)
      PARAMETER (ZERO=0.)
c*wdh
      external mappro !$pragma C( mappro )
c*wdh
C
      WRITE(*,1) IG
    1 FORMAT(' Generating grid number ',I4)
C
C   Load points from reference curve into boundary points array.
C
      IF ( (IBCJA.EQ.15) .OR. (IBCJA.EQ.20) ) THEN
         NRC      = ABS(NGBCA)
         JBMAX(1) = JCMAX(NRC)
         IF (NGBCA.GT.0) THEN
            II    = NGPC(NRC)-1
            ISIGN = 1
         ELSE
            II    = NGPC(NRC) + JCMAX(NRC)
            ISIGN = -1
         ENDIF
         DO 10 JJ=1,JBMAX(1)
            IC       = II + ISIGN*JJ
            XB(JJ,1) = XC(IC)
            YB(JJ,1) = YC(IC)
            ZB(JJ,1) = ZC(IC)
 10      CONTINUE
      ENDIF
C
      IF ( (IBCJB.EQ.15) .OR. (IBCJB.EQ.20) ) THEN
         NRC      = ABS(NGBCB)
         JBMAX(2) = JCMAX(NRC)
         IF (NGBCB.GT.0) THEN
            II    = NGPC(NRC)-1
            ISIGN = 1
         ELSE
            II    = NGPC(NRC) + JCMAX(NRC)
            ISIGN = -1
         ENDIF
         DO 20 JJ=1,JBMAX(2)
            IC       = II + ISIGN*JJ
            XB(JJ,2) = XC(IC)
            YB(JJ,2) = YC(IC)
            ZB(JJ,2) = ZC(IC)
 20      CONTINUE
      ENDIF
C
C   Check for consistency of number of points for BC type 20
C
      IF ( (IBCJA.EQ.20) .AND. (JBMAX(1).NE.KMAX) ) THEN
        WRITE(*,*)'Inconsistent no. of points between input parameter',
     &            ' and supplied reference curve. Program terminated.'
        STOP 'INITS'
      ENDIF
      IF ( (IBCJB.EQ.20) .AND. (JBMAX(2).NE.KMAX) ) THEN
        WRITE(*,*)'Inconsistent no. of points between input parameter',
     &            ' and supplied reference curve. Program terminated.'
        STOP 'INITS'
      ENDIF
C
C   Initialize logical for hyperbolic step or interpolation step.
C
      IF (IBCJA.GE.31 .AND. IBCJA.LE.34) THEN
         HYSTEP = .FALSE.
      ELSE
         HYSTEP = .TRUE.
      ENDIF
C
C   Set up constant/symmetry plane arrays.
C
      DO 40 I=1,3
         JPLNA(I) = 1
         JPLNB(I) = 1
 40   CONTINUE
      IF (IBCJA.EQ.1 .OR. IBCJA.EQ.4 .OR. IBCJA.EQ.21) JPLNA(1) = 0
      IF (IBCJA.EQ.2 .OR. IBCJA.EQ.5 .OR. IBCJA.EQ.22) JPLNA(2) = 0
      IF (IBCJA.EQ.3 .OR. IBCJA.EQ.6 .OR. IBCJA.EQ.23) JPLNA(3) = 0
      IF (IBCJB.EQ.1 .OR. IBCJB.EQ.4 .OR. IBCJB.EQ.21) JPLNB(1) = 0
      IF (IBCJB.EQ.2 .OR. IBCJB.EQ.5 .OR. IBCJB.EQ.22) JPLNB(2) = 0
      IF (IBCJB.EQ.3 .OR. IBCJB.EQ.6 .OR. IBCJB.EQ.23) JPLNB(3) = 0
C
C   Set up JPER.
C
      JPER   = 0
      IF (IBCJA.EQ.10) JPER   = 1
C
C   Set up J1 and J2.
C
      J1     = 1
      IF      (JPER.EQ.0) THEN
         J2     = JMAX
      ELSE IF (JPER.EQ.1) THEN
         J2     = JMAX-1
      ENDIF
      IF (IBCJA.GE.4 .AND. IBCJA.LE.6) J1     = 2
      IF (IBCJB.GE.4 .AND. IBCJB.LE.6) J2     = JMAX-1
C
C   Set JJP,JJR indices.
C
      DO 50 J = 1,JMAX
         JJP(J)      = J+1
         JJR(J)      = J-1
 50   CONTINUE
      IF      (JPER.EQ.1) THEN
         JJP(JMAX-1) = 1
         JJP(JMAX)   = 2
         JJR(1)      = JMAX-1
      ELSE IF (JPER.EQ.0) THEN
         JJP(JMAX)   = JMAX
         JJR(1)      = 1
      ENDIF
C
C   Set up spacing for marching in eta (hyperbolic tangent stretching).
C   Get spacing from boundary curves if BC type 20.
C
      IF ( (IBCJA.NE.20) .AND. (IBCJB.NE.20) ) THEN
C
       IF      (NNOD.EQ.1) THEN
          CALL SFUNS (ISF,ZERO,ETAMX(1),DETA(1),DFAR(1),
     &                KMAX,RMAX,M1D,DX0A,DX1A,TMP1)
          DO 60 K = 1,KMAX
          DO 60 J = 1,JMAX
             R(J,K) = TMP1(K)
 60       CONTINUE
C
       ELSE IF (NNOD.GT.1) THEN
          CALL VARETA(M1D,JMAX,KMAX,JPER,NNOD,JNOD,ETAMX,DETA,DFAR,
     &                ETAVAR,TMP1,X,Y,Z)
          DO 70 J = 1,JMAX
            CALL SFUNS (ISF,ZERO,ETAVAR(J,1),ETAVAR(J,2),ETAVAR(J,3),
     &                  KMAX,RMAX,M1D,DX0A,DX1A,TMP1)
            DO 80 K=1,KMAX
               R(J,K) = TMP1(K)
 80         CONTINUE
 70       CONTINUE
       ENDIF
C
      ELSE
C
       CALL SFBNDY(M1D,JMAX,KMAX,IBCJA,IBCJB,X,Y,Z,R,XB,YB,ZB,TMP1)
C
      ENDIF
C
C    Check stretching function and report min/max stretching ratio.
C
      CALL SFCHEK(JMAX,KMAX,R)
C
C  -----------------------------------------------------------------
C  NOTE: The order of the following must be preserved, i.e., read in
C        user supplied boundary points in subroutine INIGRD first
C        (if any on either or both boundaries) before initializing
C        from supplied surface grid.
C  -----------------------------------------------------------------
C
      IF (IRSTYP.EQ.1) THEN
C
C      Initialize ISEG.
C
        ISEG(1) = 1
        ISEG(2) = 1
C
C      Initialize boundary points from supplied surface grid.
C
        IB = 1
        IF ((IBCJA.GE.11).AND.(IBCJA.LE.14)) THEN
         CALL INIFIX(M1D,NRGRD,NTOTR,JMAX,KMAX,JKD,NGPR,JRMAX,KRMAX,
     &               JRPER,KRPER,JRAXSA,JRAXSB,J1,IBCJA,IAFAM,NGBCA,
     &               IB,JBMAX,JRP,JRR,KRP,KRR,
     &               X,Y,Z,XR,YR,ZR,
     &               IGPB,IRFAM,XB,YB,ZB)
        ELSE IF (IBCJA.EQ.15) THEN
         XP = X(1,1)
         YP = Y(1,1)
         ZP = Z(1,1)
         CALL LOCSEG(M1D,JBMAX(1),XB,YB,ZB,
     &               XP,YP,ZP,IB,TMP1,ISEG(1))
        ENDIF
C
        IB = 2
        IF ((IBCJB.GE.11).AND.(IBCJB.LE.14)) THEN
         CALL INIFIX(M1D,NRGRD,NTOTR,JMAX,KMAX,JKD,NGPR,JRMAX,KRMAX,
     &               JRPER,KRPER,JRAXSA,JRAXSB,J2,IBCJB,IAFAM,NGBCB,
     &               IB,JBMAX,JRP,JRR,KRP,KRR,
     &               X,Y,Z,XR,YR,ZR,
     &               IGPB,IRFAM,XB,YB,ZB)
        ELSE IF (IBCJB.EQ.15) THEN
         XP = X(JMAX,1)
         YP = Y(JMAX,1)
         ZP = Z(JMAX,1)
         CALL LOCSEG(M1D,JBMAX(2),XB,YB,ZB,
     &               XP,YP,ZP,IB,TMP1,ISEG(2))
        ENDIF
C
      ELSE IF (IRSTYP.EQ.2) THEN
C
C      Determine boundary points from NIGES data.
C
      ENDIF
C
C
      IF (IRSTYP.EQ.1) THEN
C
       IF (HYSTEP) THEN
C
C       Find grid and cell index to start marching.
C
         IF ((IBCJA.GE.11).AND.(IBCJA.LE.14)) THEN
            IGSTAR = NGBCA
         ELSE
            IGSTAR = 0
         ENDIF
C
         CALL FNDIQD(M1D,NRGRD,NTOTR,JMAX,KMAX,NGPR,JRMAX,KRMAX,
     &               J1,J2,I1,NS,JS,KS,JPER,JRPER,KRPER,JRAXSA,JRAXSB,
     &               IGSTAR,JRP,JRR,KRP,KRR,XR,YR,ZR,IGPB,
     &               IRFAM,IAFAM,JKIH,X,Y,Z,R)
C
C       Find surface normals for initial curve.
C
         CALL FNDSPN(M1D,NRGRD,NTOTR,JMAX,KMAX,NGPR,NGP1,JRMAX,KRMAX,
     &               J1,J2,I1,JPER,JRPER,KRPER,JRAXSA,JRAXSB,
     &               JRP,JRR,KRP,KRR,X,Y,Z,R,XR,YR,ZR,
     &               XNRU,YNRU,ZNRU,IGPB,IRFAM,IAFAM,GNV,
     &               GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,JKIH)
C
c*wdh
c         Project onto the surface
         k=1
c*         call mappro( jmax,kmax,k,x(1,k),y(1,k),z(1,k),gnv,dels )
c        don't project initial points onto the surface!
         call mappro( jmax,kmax,k,xsnr,ysnr,zsnr,gnv,dels )
c*wdh

       ELSE
C
C       Determine xi_s and eta_s for initial curve.
C
         CALL INIINT(M1D,NRGRD,NTOTR,JKD,JMAX,KMAX,NGPR,JRMAX,KRMAX,
     &               IBCJA,IAFAM,NGBCA,JRPER,KRPER,JRAXSA,JRAXSB,
     &               JRP,JRR,KRP,KRR,XR,YR,ZR,X,Y,Z,IGPB,
     &               IRFAM,JBMAX,ICUR,XI,YI,ZI)
C
       ENDIF
C
      ELSE IF (IRSTYP.EQ.2) THEN
C
       IF (HYSTEP) THEN
C
C       Find local surface normals for initial curve.
C
c*wdh
c         Project onto the surface
         iproject=.true.
         k=1
         if( iproject )then
c          do project initial points onto the surface 
           call mappro( jmax,kmax,k,x(1,k),y(1,k),z(1,k),gnv,dels )
         end if
         do j=1,jmax
           xsnr(j)=x(j,1)
           ysnr(j)=y(j,1)
           zsnr(j)=z(j,1)
         end do
         if( .not. iproject )then
c          don't project initial points onto the surface!
           call mappro( jmax,kmax,k,xsnr,ysnr,zsnr,gnv,dels )
         end if
         do j=1,jmax
          gsn1(j)=gnv(j,k,1)
          gsn2(j)=gnv(j,k,2)
          gsn3(j)=gnv(j,k,3) 
         end do
c*wdh
#ifdef SUPERLIB
         CALL IGSPRN(JMAX,1,X(1,1),Y(1,1),Z(1,1),GSN1,GSN2,GSN3,
     &               DELS(1,1),DELS(1,2),IMODEL)
         DO 90 J=1,JMAX
          GNV(J,K,1) = GSN1(J)
          GNV(J,K,2) = GSN2(J)
          GNV(J,K,3) = GSN3(J)
 90      CONTINUE
#endif
C
       ELSE
C
C       Determine xi_s and eta_s for initial curve.
C
       ENDIF
C
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INITSZ(NTOTR,NTOTC,NTOT,NRGRD,M2D,M1D,
     >                  IPXR,IPSN,IPX,IPRC,WORK,IWORK)
C
#include "precis.h"
C
C    Get memory for grid arrays.
C
      __POINTER IPXR,IPSN,IPX,IPRC
      CHARACTER*80 TEXT
      DIMENSION WORK(*), IWORK(*)
C
      TEXT = '** ERROR ** MEMORY ALLOCATION REQUEST FAILED'
      NTEXT = 1
C
C    Get space for reference grid arrays
C
      MRG = NTOTR*6
      WRITE(*,101) MRG
 101  FORMAT('REQUESTING ',I10,' WORDS FOR REFERENCE GRID ARRAYS')
      CALL GETARR( MRG,WORK,IPXR )
      IF ( IPXR.EQ.0 ) THEN
         CALL ERRMSG(TEXT,NTEXT)
         CALL ERREND
         STOP 'INITSZ'
      ENDIF
C
C    Get space for surface normal arrays
C
      MSN = NTOTR*6
      WRITE(*,102) MSN
 102  FORMAT('REQUESTING ',I10,' WORDS FOR SURFACE NORMAL ARRAYS')
      CALL GETARR( MSN,WORK,IPSN )
      IF ( IPSN.EQ.0 ) THEN
         CALL ERRMSG(TEXT,NTEXT)
         CALL ERREND
         STOP 'INITSZ'
      ENDIF
C
C    Get space for active grid arrays
C
      MAG  = NTOT*3
      WRITE(*,103) MAG
 103  FORMAT('REQUESTING ',I10,' WORDS FOR ACTIVE GRID ARRAYS')
      CALL GETARR( MAG,WORK,IPX )
      IF ( IPX.EQ.0 ) THEN
         CALL ERRMSG(TEXT,NTEXT)
         CALL ERREND
         STOP 'INITSZ'
      ENDIF
C
C    Get space for referece curve arrays
C
      MRC  = NTOTC*3
      WRITE(*,104) MRC
 104  FORMAT('REQUESTING ',I10,' WORDS FOR REFERENCE CURVE ARRAYS')
      CALL GETARR( MRC,WORK,IPRC )
      IF ( IPRC.EQ.0 ) THEN
         CALL ERRMSG(TEXT,NTEXT)
         CALL ERREND
         STOP 'INITSZ'
      ENDIF
C
C    Write dynamic, static, total memory needed. 
C
      IMDTOT = MRG + MSN + MAG + MRC
      WRITE(*,131) IMDTOT
 131  FORMAT('TOTAL DYNAMIC MEMORY REQUESTED = ',I12)
C
C      IMSTOT = 46*MSUB + 8*MGRID
C      WRITE(*,132) IMSTOT
C 132  FORMAT('TOTAL STATIC MEMORY REQUIRED   = ',I12)
C
C      WRITE(*,133) IMDTOT+IMSTOT
C 133  FORMAT('TOTAL MEMORY REQUIRED          = ',I12)
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INTAXS(NRGRD,NTOTR,NGPR,JMAX,KMAX,JRMAX,KRMAX,J1,J2,K,
     &                  JRAXSA,JRAXSB,X,Y,Z,XR,YR,ZR,
     &                  GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,JKIH)
C
C   Do special interpolation for points on the initial curve that fall
C   on an axis point.
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD),
     &          JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX),
     &          XSNR(JMAX), YSNR(JMAX), ZSNR(JMAX), JKIH(JMAX,3)
C
C
      DO 10 J = J1,J2
         NS     = JKIH(J,3)
         IS     = NGPR(NS)
         IF (JRAXSA(NS).EQ.1)
     &    CALL CHKAXS(JRMAX(NS),KRMAX(NS),JMAX,KMAX,1,J,K,
     &                XR(IS),YR(IS),ZR(IS),X,Y,Z,
     &                GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR)
         IF (JRAXSB(NS).EQ.1)
     &    CALL CHKAXS(JRMAX(NS),KRMAX(NS),JMAX,KMAX,JRMAX(NS),J,K,
     &                XR(IS),YR(IS),ZR(IS),X,Y,Z,
     &                GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR)
   10    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE INTERP(NJ,NK,JS,KS,DELXI,DELET,XS,YS,ZS,
     &                  XSN,YSN,ZSN,XSNP,YSNP,ZSNP,XSP,YSP,ZSP)
C
#include "precis.h"
C
C   Do interpolation of surface normal and coordinates.
C   (XS,YS,ZS) and (XSN,YSN,ZSN) are the surface grid and normals,
C   resp., to be interpolated from.  (XSP,YSP,ZSP) and (XSNP,YSNP,ZSNP)
C   are the returned physical location and surface normal at grid cell
C   (JS,KS), offset (DELXI,DELET).
C
      PARAMETER (ONE=1.0)
      DIMENSION XS(NJ,NK), YS(NJ,NK), ZS(NJ,NK)
      DIMENSION XSN(NJ,NK), YSN(NJ,NK), ZSN(NJ,NK)
C
C
      C11    = DELXI*DELET
      C10    = DELXI - C11
      C01    = DELET - C11
      C00    = ONE - DELXI - DELET + C11
      JP     = JS+1
      KP     = KS+1
      XSNP   = XSN(JS,KS)*C00 + XSN(JP,KS)*C10
     &       + XSN(JS,KP)*C01 + XSN(JP,KP)*C11
      YSNP   = YSN(JS,KS)*C00 + YSN(JP,KS)*C10
     &       + YSN(JS,KP)*C01 + YSN(JP,KP)*C11
      ZSNP   = ZSN(JS,KS)*C00 + ZSN(JP,KS)*C10
     &       + ZSN(JS,KP)*C01 + ZSN(JP,KP)*C11
      XSP    = XS(JS,KS)*C00 + XS(JP,KS)*C10
     &       + XS(JS,KP)*C01 + XS(JP,KP)*C11
      YSP    = YS(JS,KS)*C00 + YS(JP,KS)*C10
     &       + YS(JS,KP)*C01 + YS(JP,KP)*C11
      ZSP    = ZS(JS,KS)*C00 + ZS(JP,KS)*C10
     &       + ZS(JS,KP)*C01 + ZS(JP,KP)*C11
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE LOCSEG(M1D,JBMAX,XB,YB,ZB,XP,YP,ZP,IB,DOT,ISEG)
C
#include "precis.h"
C
      DIMENSION XB(M1D,2), YB(M1D,2), ZB(M1D,2), DOT(M1D)
C
C    Locate segment (i,i+1) on supplied boundary curve on which an
C    initial end point lies.
C
      DO 10 I=1,JBMAX-1
       X1 = XB(I,IB)-XP
       Y1 = YB(I,IB)-YP
       Z1 = ZB(I,IB)-ZP
       X2 = XB(I+1,IB)-XP
       Y2 = YB(I+1,IB)-YP
       Z2 = ZB(I+1,IB)-ZP
       D1 = X1**2 + Y1**2 + Z1**2
       D2 = X2**2 + Y2**2 + Z2**2
       IF (D1.EQ.0.0) THEN
        DOT(I) = -1.0
       ELSE IF (D2.EQ.0.0) THEN
        DOT(I) = 1.0
       ELSE
        DOT(I) = (X1*X2 + Y1*Y2 + Z1*Z2)/(SQRT(D1*D2))
       ENDIF
       IF ( (I.EQ.JBMAX-1) .AND. (D2.EQ.0.0) ) DOT(I) = -1.0
 10   CONTINUE
C
      ISEG = 1
      DOTM = DOT(1)
      DO 20 I=2,JBMAX-1
       IF (DOT(I).LT.DOTM) THEN
        DOTM = DOT(I)
        ISEG = I
       ENDIF
 20   CONTINUE
C
c      write(*,*)'jbmax,ib,iseg ',jbmax,ib,iseg
c
      RETURN
      END
C***********************************************************************
      SUBROUTINE LUDEC(A)
C
#include "precis.h"
C
      DIMENSION A(3,3)
      COMMON/LUD/ L11,L21,L22,L31,L32,L33,V1,V2,V3,U12,U13,U23
      __REAL       L11,L21,L22,L31,L32,L33
C
C    Compute L-U decomposition elements
C
      L11 = A(1,1)
       V1 = 1./L11
      U12 =  V1*A(1,2)
      U13 =  V1*A(1,3)
      L21 = A(2,1)
      L22 = A(2,2) - L21*U12
       V2 = 1./L22
      U23 = ( A(2,3) -L21*U13)* V2
      L31 = A(3,1)
      L32 = A(3,2) - L31*U12
      L33 = A(3,3) - L31*U13 -L32*U23
       V3 = 1./L33
      RETURN
      END
C***********************************************************************
      SUBROUTINE METBLN(JMAX,KMAX,K,IBCJA,IBCJB,X,Y,Z,XE,YE,ZE,
     &                  GSN1,GSN2,GSN3,JPLNA,JPLNB,XBP,YBP,ZBP)
C
C    Rotate eta metrics for marching depending on boundary condition
C
#include "precis.h"
C
      PARAMETER (ONE=1.0, I5=5, P5=0.5, P8=0.8, TOLB=0.99)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION XE(JMAX), YE(JMAX), ZE(JMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX)
      DIMENSION JPLNA(3), JPLNB(3), XBP(2), YBP(2), ZBP(2)
C
      PI     = 4.*ATAN(ONE)
      DOTA   = ONE
      DOTB   = ONE
C
C    Determine how many points the blending should affect
C    The number of affected points is weighted by the change in angle
C
      IBLENA = 0
      IBLENB = 0
      IF (     (IBCJA.GE.1  .AND. IBCJA.LE.3 )
     &    .OR. (IBCJA.GE.11 .AND. IBCJA.LE.20) ) IBLENA = 1
      IF (     (IBCJB.GE.1  .AND. IBCJB.LE.3 )
     &    .OR. (IBCJB.GE.11 .AND. IBCJB.LE.20) ) IBLENB = 1
C
      IF (IBLENA.EQ.1 .OR. IBLENB.EQ.1) THEN
C
         IF (IBLENA.EQ.1) THEN
            J      = 1
            IF (IBCJA.LE.3) THEN
               XBA    = JPLNA(1)*XE(J+1)
               YBA    = JPLNA(2)*YE(J+1)
               ZBA    = JPLNA(3)*ZE(J+1)
            ENDIF
            IF (IBCJA.GE.11) THEN
               XBA    = XBP(1)-X(J,K)
               YBA    = YBP(1)-Y(J,K)
               ZBA    = ZBP(1)-Z(J,K)
            ENDIF
            BDMA   = SQRT(XBA*XBA + YBA*YBA + ZBA*ZBA)
            EMA    = SQRT(XE(J+1)**2 + YE(J+1)**2 + ZE(J+1)**2)
            DOTA   = (XE(J+1)*XBA + YE(J+1)*YBA + ZE(J+1)*ZBA)
     &              /(EMA*BDMA)
         ENDIF
C
         IF (IBLENB.EQ.1) THEN
            J      = JMAX
            IF (IBCJB.LE.3) THEN
               XBB    = JPLNB(1)*XE(J-1)
               YBB    = JPLNB(2)*YE(J-1)
               ZBB    = JPLNB(3)*ZE(J-1)
            ENDIF
            IF (IBCJB.GE.11) THEN
               XBB    = XBP(2)-X(J,K)
               YBB    = YBP(2)-Y(J,K)
               ZBB    = ZBP(2)-Z(J,K)
            ENDIF
            BDMB   = SQRT(XBB*XBB + YBB*YBB + ZBB*ZBB)
            EMB    = SQRT(XE(J-1)**2 + YE(J-1)**2 + ZE(J-1)**2)
            DOTB   = (XE(J-1)*XBB + YE(J-1)*YBB + ZE(J-1)*ZBB)
     &              /(EMB*BDMB)
         ENDIF
C
         IF      (DOTA.GE.TOLB .AND. DOTB.GE.TOLB) THEN
            JTA    = 1
            JTB    = 1
         ELSE IF (DOTA.LT.TOLB .AND. DOTB.GE.TOLB) THEN
            JTA    = MIN( INT( I5*(ONE-DOTA)*JMAX ), INT(P8*JMAX) )
            JTB    = 1
         ELSE IF (DOTA.GE.TOLB .AND. DOTB.LT.TOLB) THEN
            JTA    = 1
            JTB    = MIN( INT( I5*(ONE-DOTB)*JMAX ), INT(P8*JMAX) )
         ELSE IF (DOTA.LT.TOLB .AND. DOTB.LT.TOLB) THEN
            JTA    = MIN( INT( I5*(ONE-DOTA)*JMAX ), INT(P5*JMAX) )
            JTB    = MIN( INT( I5*(ONE-DOTB)*JMAX ), INT(P5*JMAX) )
         ENDIF
C
      ENDIF
C
C    Blend eta metrics according to boundary direction
C
      IF (IBLENA.EQ.1) THEN
         JT     = JTA
         DO 10 J = 1,JT
            EM     = SQRT(XE(J)**2 + YE(J)**2 + ZE(J)**2)
            EP     = SQRT( FLOAT(JT-J+1)/FLOAT(JT) )
            EPM    = ONE - EP
            XN     = EP*XBA/BDMA + EPM*XE(J)/EM
            YN     = EP*YBA/BDMA + EPM*YE(J)/EM
            ZN     = EP*ZBA/BDMA + EPM*ZE(J)/EM
            DN     = EM/SQRT(XN**2 + YN**2 + ZN**2)
            XE(J)  = XN*DN
            YE(J)  = YN*DN
            ZE(J)  = ZN*DN
   10       CONTINUE
      ENDIF
C
      IF (IBLENB.EQ.1) THEN
         JT     = JTB
         DO 20 J = JMAX,JMAX-JT+1,-1
            EM     = SQRT(XE(J)**2 + YE(J)**2 + ZE(J)**2)
            EP     = SQRT( FLOAT(J-JMAX+JT)/FLOAT(JT) )
            EPM    = ONE - EP
            XN     = EP*XBB/BDMB + EPM*XE(J)/EM
            YN     = EP*YBB/BDMB + EPM*YE(J)/EM
            ZN     = EP*ZBB/BDMB + EPM*ZE(J)/EM
            DN     = EM/SQRT(XN**2 + YN**2 + ZN**2)
            XE(J)  = XN*DN
            YE(J)  = YN*DN
            ZE(J)  = ZN*DN
   20       CONTINUE
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE METCOR(JMAX,KMAX,JKD,JS,JE,K,JJP,JJR,JPER,X,Y,Z,
     &                  XX,YX,ZX,GSN1,GSN2,GSN3,DAREA,U,XE,YE,ZE)
C
#include "precis.h"
C
      DIMENSION JJP(JMAX), JJR(JMAX)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION XX(JMAX),YX(JMAX),ZX(JMAX),XE(JMAX),YE(JMAX),ZE(JMAX)
      DIMENSION GSN1(JMAX),GSN2(JMAX),GSN3(JMAX),DAREA(JMAX),U(JKD,3,3)
C
C    Do metric correction procedure to introduce angle bisecting property
C
      IF (K.EQ.1) THEN
       ZSC = 1.0
      ELSE IF ((K.GT.1).AND.(K.LE.9)) THEN
       ZSC = 1.0/(2.0**(K-1))
      ELSE IF (K.GT.9) THEN
       ZSC = 0.0
      ENDIF
      ZSCM = 1.0-ZSC
      DO 10 J = JS,JE
       JP = JJP(J)
       JR = JJR(J)
       VJP = SQRT( (X(JP,K)-X(J,K))**2 + (Y(JP,K)-Y(J,K))**2 +
     &             (Z(JP,K)-Z(J,K))**2 )
       VJM = SQRT( (X(JR,K)-X(J,K))**2 + (Y(JR,K)-Y(J,K))**2 +
     &             (Z(JR,K)-Z(J,K))**2 )
       VJAV = 0.5*(VJP+VJM)
       VJPN = VJAV/VJP
       VJMN = VJAV/VJM
       VJDF = VJMN-VJPN
       XXN = 0.5*( VJPN*X(JP,K) - VJMN*X(JR,K) + VJDF*X(J,K) )
       YXN = 0.5*( VJPN*Y(JP,K) - VJMN*Y(JR,K) + VJDF*Y(J,K) )
       ZXN = 0.5*( VJPN*Z(JP,K) - VJMN*Z(JR,K) + VJDF*Z(J,K) )
       RXRX = XXN**2 + YXN**2 + ZXN**2
       SNRX = GSN1(J)*XXN +GSN2(J)*YXN + GSN3(J)*ZXN
       RD = 1./( RXRX - SNRX**2)
       U1 =  (GSN2(J)*ZXN - GSN3(J)*YXN)*RD
       U2 =  (GSN3(J)*XXN - GSN1(J)*ZXN)*RD
       U3 =  (GSN1(J)*YXN - GSN2(J)*XXN)*RD
       XE(J) = ZSC*U1*DAREA(J) + ZSCM*XE(J)
       YE(J) = ZSC*U2*DAREA(J) + ZSCM*YE(J)
       ZE(J) = ZSC*U3*DAREA(J) + ZSCM*ZE(J)
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE METRET(JMAX,KMAX,JKD,JS,JE,K,JJP,JJR,JPER,X,Y,Z,
     &                  XX,YX,ZX,GSN1,GSN2,GSN3,DAREA,U,XE,YE,ZE)
C
#include "precis.h"
C
      DIMENSION JJP(JMAX), JJR(JMAX)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION XX(JMAX),YX(JMAX),ZX(JMAX),XE(JMAX),YE(JMAX),ZE(JMAX)
      DIMENSION GSN1(JMAX),GSN2(JMAX),GSN3(JMAX),DAREA(JMAX),U(JKD,3,3)
C
C    Compute eta metrics using xi metrics and pde
C
      DO 10 J = JS,JE
       XE(J) = U(J,1,2)*DAREA(J)
       YE(J) = U(J,2,2)*DAREA(J)
       ZE(J) = U(J,3,2)*DAREA(J)
 10   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE METRXI(JMAX,KMAX,JS,JE,K,JJP,JJR,JPER,X,Y,Z,XX,YX,ZX)
C
#include "precis.h"
C
      DIMENSION JJP(JMAX), JJR(JMAX)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION XX(JMAX), YX(JMAX), ZX(JMAX)
C
C    Compute xi metrics
C
      DO 21 J=JS,JE
       JP = JJP(J)
       JR = JJR(J)
       XX(J) = ( X(JP,K) - X(JR,K))*.5
       YX(J) = ( Y(JP,K) - Y(JR,K))*.5
       ZX(J) = ( Z(JP,K) - Z(JR,K))*.5
 21   CONTINUE
      IF (JPER .EQ. 0) THEN
       J = JS
        XX(J) = -X(J,K) + X(J+1,K)
        YX(J) = -Y(J,K) + Y(J+1,K)
        ZX(J) = -Z(J,K) + Z(J+1,K)
       J = JE
        XX(J) = X(J,K) -X(J-1,K)
        YX(J) = Y(J,K) -Y(J-1,K)
        ZX(J) = Z(J,K) -Z(J-1,K)
      ELSE
       XX(JMAX) = XX(1)
       YX(JMAX) = YX(1)
       ZX(JMAX) = ZX(1)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE POSTBC(M1D,JMAX,KMAX,JKD,NRGRD,NTOTR,
     &                  NGPR,NGNR,NGP1,JRMAX,KRMAX,
     &                  IRSTYP,IBCJA,IBCJB,JRPER,KRPER,JRAXSA,JRAXSB,
     &                  JPLNA,JPLNB,JRP,JRR,KRP,KRR,J1,J2,K,
     &                  X,Y,Z,XR,YR,ZR,
     &                  XNR,YNR,ZNR,R,IGPB,JKIH,IRFAM,IAFAM,
     &                  GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,XB,YB,ZB,ISEG)
C
C   Post process x,y,z at boundaries to ensure that the boundary points
C   lie on the surface and the constant plane or symmetry plane
C   conditions are satisfied.
C
#include "precis.h"
C
      DIMENSION JPLNA(3), JPLNB(3)
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), NGP1(NRGRD),
     &          JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XNR(NTOTR), YNR(NTOTR), ZNR(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD), JKIH(JMAX,3)
C
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), R(JMAX,KMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX),
     &          XSNR(JMAX), YSNR(JMAX), ZSNR(JMAX)
      DIMENSION XB(M1D,2), YB(M1D,2), ZB(M1D,2), ISEG(2)
      DIMENSION XBP(2), YBP(2), ZBP(2)
C
      PARAMETER (I0=0,I1=1)
      PARAMETER (NIT=50,TOLER=1.0E-3)
C
C
      JAPROD    = JPLNA(1)*JPLNA(2)*JPLNA(3)
      JBPROD    = JPLNB(1)*JPLNB(2)*JPLNB(3)
C
      IF (IBCJA.GE.1 .AND. IBCJA.LE.6) THEN
C
         J         = J1
         X(J,K)    = X(J,K)*JPLNA(1) + X(J,K-1)*(1-JPLNA(1))
         Y(J,K)    = Y(J,K)*JPLNA(2) + Y(J,K-1)*(1-JPLNA(2))
         Z(J,K)    = Z(J,K)*JPLNA(3) + Z(J,K-1)*(1-JPLNA(3))
         JS        = JKIH(J,1)
         KS        = JKIH(J,2)
         NS        = JKIH(J,3)
C
         DO 100 N = 1,NIT
C
C   Push point onto surface.
C
           IF (IRSTYP.EQ.1) THEN
C
#ifdef DEBUG
            WRITE(*,*)
            WRITE(*,*) 'calling cells from postbc'
            WRITE(*,*) ' initial point (j,k)        ',J,K
            WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
            CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &                  X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &                  IGPB,I1,IQSTAT)
#ifdef DEBUG
            WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
            WRITE(*,*)
#endif
C
C   Unit normals assumed to be at points.
C
            IS  = NGPR(NS)
            IS1 = NGP1(NS)
            CALL INTERP(JRMAX(NS),KRMAX(NS),JS,KS,
     &                  DELXI,DELET,XR(IS),YR(IS),ZR(IS),
     &                  XNR(IS1),YNR(IS1),ZNR(IS1),
     &                  GSN1(J),GSN2(J),GSN3(J),XSNR(J),YSNR(J),ZSNR(J))
C
            DELR      = SQRT((X(J,K)-XSNR(J))**2 + (Y(J,K)-YSNR(J))**2
     &                                           + (Z(J,K)-ZSNR(J))**2 )
            X(J,K)    = XSNR(J)*JPLNA(1) + X(J,K-1)*(1-JPLNA(1))
            Y(J,K)    = YSNR(J)*JPLNA(2) + Y(J,K-1)*(1-JPLNA(2))
            Z(J,K)    = ZSNR(J)*JPLNA(3) + Z(J,K-1)*(1-JPLNA(3))
            IF (DELR.LT.TOLER) GOTO 110
            IF (N.EQ.NIT) WRITE(*,99) J,K,DELR
   99       FORMAT(' NIT exceeded at J=',I3,' K=',I3,' with delr=',
     &             E14.5)
C
          ELSE IF (IRSTYP.EQ.2) THEN
C
c* wdh ************** ?
C
          ENDIF
C
  100       CONTINUE
  110    CONTINUE
C
      ENDIF
C
      IF (IBCJB.GE.1 .AND. IBCJB.LE.6) THEN
C
         J         = J2
         X(J,K)    = X(J,K)*JPLNB(1) + X(J,K-1)*(1-JPLNB(1))
         Y(J,K)    = Y(J,K)*JPLNB(2) + Y(J,K-1)*(1-JPLNB(2))
         Z(J,K)    = Z(J,K)*JPLNB(3) + Z(J,K-1)*(1-JPLNB(3))
         JS        = JKIH(J,1)
         KS        = JKIH(J,2)
         NS        = JKIH(J,3)
C
         DO 200 N = 1,NIT
C
C   Push point onto surface.
C
          IF (IRSTYP.EQ.1) THEN
C
#ifdef DEBUG
            WRITE(*,*)
            WRITE(*,*) 'calling cells from postbc x2'
            WRITE(*,*) ' initial point (j,k)        ',J,K
            WRITE(*,*) ' close point (j,k,grid)     ',JS,KS,NS
#endif
            CALL CELLS (M1D,NRGRD,NTOTR,NGPR,JRMAX,KRMAX,XR,YR,ZR,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,IRFAM,IAFAM,
     &                  X(J,K),Y(J,K),Z(J,K),NS,JS,KS,DELXI,DELET,
     &                  IGPB,I1,IQSTAT)
#ifdef DEBUG
            WRITE(*,*) ' returned (j,k,grid,status) ',JS,KS,NS,IQSTAT
            WRITE(*,*)
#endif
C
C   Unit normals assumed to be at points.
C
            IS  = NGPR(NS)
            IS1 = NGP1(NS)
            CALL INTERP(JRMAX(NS),KRMAX(NS),JS,KS,
     &                  DELXI,DELET,XR(IS),YR(IS),ZR(IS),
     &                  XNR(IS1),YNR(IS1),ZNR(IS1),
     &                  GSN1(J),GSN2(J),GSN3(J),XSNR(J),YSNR(J),ZSNR(J))
C
            DELR      = SQRT((X(J,K)-XSNR(J))**2 + (Y(J,K)-YSNR(J))**2
     &                                           + (Z(J,K)-ZSNR(J))**2)
            X(J,K)    = XSNR(J)*JPLNB(1) + X(J,K-1)*(1-JPLNB(1))
            Y(J,K)    = YSNR(J)*JPLNB(2) + Y(J,K-1)*(1-JPLNB(2))
            Z(J,K)    = ZSNR(J)*JPLNB(3) + Z(J,K-1)*(1-JPLNB(3))
            IF (DELR.LT.TOLER) GOTO 210
            IF (N.EQ.NIT) WRITE(*,99) J,K,DELR
C
          ELSE IF (IRSTYP.EQ.2) THEN
C
C
          ENDIF
C
  200       CONTINUE
  210    CONTINUE
C
      ENDIF
C
C   Corrector step for constrained floating bc.
C
      IF (IRSTYP.EQ.1) THEN
C
      IMODE     = 2
      IF (IBCJA.GE.11 .AND. IBCJA.LE.15) THEN
         J         = J1
         IB        = 1
         CALL CMPFIX(JMAX,KMAX,M1D,J,K,IB,X,Y,Z,
     &               XB,YB,ZB,XBP,YBP,ZBP,R,ISEG,IMODE)
         X(J,K)    = XBP(1)
         Y(J,K)    = YBP(1)
         Z(J,K)    = ZBP(1)
      ENDIF
      IF (IBCJB.GE.11 .AND. IBCJB.LE.15) THEN
         J         = J2
         IB        = 2
         CALL CMPFIX(JMAX,KMAX,M1D,J,K,IB,X,Y,Z,
     &               XB,YB,ZB,XBP,YBP,ZBP,R,ISEG,IMODE)
         X(J,K)    = XBP(2)
         Y(J,K)    = YBP(2)
         Z(J,K)    = ZBP(2)
      ENDIF
C
      ELSE IF (IRSTYP.EQ.2) THEN
C
C
      ENDIF
C
C   Reflect to get extra plane at end points for symmetry plane
C   conditions.
C
      IF (IBCJA.GE.4 .AND. IBCJA.LE.6) THEN
         J         = J1-1
         X(J,K)    = X(J+2,K)*(2.0*JPLNA(1)-1.0)
         Y(J,K)    = Y(J+2,K)*(2.0*JPLNA(2)-1.0)
         Z(J,K)    = Z(J+2,K)*(2.0*JPLNA(3)-1.0)
      ENDIF
      IF (IBCJB.GE.4 .AND. IBCJB.LE.6) THEN
         J         = J2+1
         X(J,K)    = X(J-2,K)*(2.0*JPLNB(1)-1.0)
         Y(J,K)    = Y(J-2,K)*(2.0*JPLNB(2)-1.0)
         Z(J,K)    = Z(J-2,K)*(2.0*JPLNB(3)-1.0)
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE RDFBPA(MRGRD,NRGRD,NAGRD,IRFAM,IAFAM,NGBCA,NGBCB,
     &                  JRAXSA,JRAXSB,JRPER,KRPER,NSPBC,IAUC,NFAM)
C
#include "precis.h"
C
C    Read input parameters in final block. NSPBC and IAUC are read before
C    entering this routine.
C
      DIMENSION IRFAM(MRGRD), IAFAM(MRGRD), NGBCA(MRGRD), NGBCB(MRGRD),
     &          JRAXSA(MRGRD), JRAXSB(MRGRD), JRPER(MRGRD), KRPER(MRGRD)
C
      CHARACTER*80 TEXT(3)
C
C    Check concatenation option and set flags to zero.
C
      IF ((IAUC.NE.1).AND.(IAUC.NE.-1)) IAUC = 0
      WRITE(*,102) NSPBC, IAUC
C
      DO 20 N = 1,NRGRD
         JRAXSA(N) = 0
         IRFAM(N)  = 0
 20   CONTINUE
C
C    Store switches temporarily in JRAXSA, JRPER, KRPER.
C
c*wdh nspbc=0 for next loop
      DO 30 N = 1,NSPBC
c*wdh         READ(*,*,ERR=1010,END=1020) NG,IBCJS,IBCKS
         IF (NG.GE.1 .AND. NG.LE.NRGRD) THEN
            JRAXSA(NG) = 1
            JRPER(NG)  = IBCJS
            KRPER(NG)  = IBCKS
         ELSE
            TEXT(1)= 'Invalid grid number. Program terminated.'
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1050
         ENDIF
 30   CONTINUE
C
C    Read reference grid families info (temp storage in JRAXSB).
C
c*wdh      READ(*,*,ERR=1010,END=1020) NFAM
      WRITE(*,104) NFAM
C
c*wdh      DO 40 N = 1,NFAM
c*wdh         READ(*,*,ERR=1010,END=1020) IFNUM,NGFAM
c*wdh         IF (IFNUM.LE.0 .OR. IFNUM.GT.NFAM) THEN
c*wdh            TEXT(1)= 'Invalid family number.'
c*wdh            NTEXT  = 1
c*wdh            CALL ERRMSG(TEXT,NTEXT)
c*wdh            GOTO 1050
c*wdh         ENDIF
c*wdh         IF (NGFAM.LE.0 .OR. NGFAM.GT.NRGRD) THEN
c*wdh            TEXT(1)= 'Invalid number of grids for family.'
c*wdh            NTEXT  = 1
c*wdh            CALL ERRMSG(TEXT,NTEXT)
c*wdh            GOTO 1050
c*wdh         ENDIF
c*wdhC
c*wdh         READ(*,*,ERR=1010,END=1020) (JRAXSB(NG),NG=1,NGFAM)
c*wdh         DO 50 NG = 1,NGFAM
c*wdh            IGNUM  = JRAXSB(NG)
c*wdh            IF (IGNUM.LE.0 .OR. IGNUM.GT.NRGRD) THEN
c*wdh               TEXT(1)= 'Invalid reference grid number.'
c*wdh               NTEXT  = 1
c*wdh               CALL ERRMSG(TEXT,NTEXT)
c*wdh               GOTO 1050
c*wdh            ENDIF
c*wdh            IRFAM(IGNUM) = IFNUM
c*wdh 50      CONTINUE
c*wdh 40   CONTINUE

C
C    Assume all reference grids in one family if NFAM is zero or negative.
C
      IF (NFAM.LE.0) THEN
         NFAM = 1
         DO 60 N = 1,NRGRD
            IRFAM(N) = 1
 60      CONTINUE
         DO 70 N = 1,NAGRD
            IAFAM(N) = 1
 70      CONTINUE
      ENDIF
C
C    Do checks on families.
C
      DO 80 N = 1,NAGRD
         IF (IAFAM(N).LE.0 .OR. IAFAM(N).GT.NFAM) THEN
            WRITE(TEXT,112) N
            NTEXT  = 2
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1050
         ENDIF
c         IF (NGBCA(N).GT.0 .AND. IRFAM(NGBCA(N)).NE.IAFAM(N)) THEN
c            WRITE(*,114) N
c            IAFAM(N) = IRFAM(NGBCA(N))
c         ENDIF
c         IF (NGBCB(N).GT.0 .AND. IRFAM(NGBCB(N)).NE.IAFAM(N)) THEN
c            WRITE(*,116) N
c            IAFAM(N) = IRFAM(NGBCB(N))
c         ENDIF
 80   CONTINUE
C
 102  FORMAT('  NSPBC = ',I5,'     IAUC  = ',I5)
 104  FORMAT('  NFAM  = ',I5,/)
 112  FORMAT(' Invalid family number for active grid',I5/
     &       ' Program terminated due to invalid input.')
 114  FORMAT(' IAFAM reset to be consistent with NGBCA for grid',I5)
 116  FORMAT(' IAFAM reset to be consistent with NGBCB for grid',I5)
C
      RETURN
C
C   Error on input.
C
 1010 CONTINUE
      TEXT(1)= 'Error on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Premature end-of-file on input.
C
 1020 CONTINUE
      TEXT(1)= 'Premature end-of-file on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Stop due to error.
C
 1050 CONTINUE
      CALL ERREND
      STOP 'RDFBPA'
C
      END
C***********************************************************************
      SUBROUTINE RDFNAM(REFSFN,REFCFN,OUTGFN,FIRSTL,LINE)
C
#include "precis.h"
C
C    Parse first 3 lines of input file to read input/output filenames.
C    If strings are not found, default filenames will be used.
C
      LOGICAL FIRSTL, DEFAUL, FILEX
      CHARACTER*80 REFSFN, REFCFN, OUTGFN
      CHARACTER*80 TEXT
      CHARACTER*120 LINE
C
      DEFAUL = .TRUE.
C
C    Read from beginning of file until we get to a non-empty line.
C
 100  CONTINUE
      READ(*,'(A)',ERR=1010,END=1020) LINE
C
C    Find first non-space, non-tab character.
C
      I1 = 0
      DO 10 I=1,LEN(LINE)
         IF ((LINE(I:I).NE.' ').AND.(LINE(I:I).NE.'	')) THEN
            I1 = I
            GO TO 15
         ENDIF
 10   CONTINUE
 15   CONTINUE
C
C    Go read next line if the entire line consists of spaces.
C
      IF (I1.EQ.0) GO TO 100
C
C    Find location of next space or comma or tab.
C
      I2 = I1+1
      DO 20 I=I1+1,LEN(LINE)
         IF ((LINE(I:I).EQ.' ').OR.(LINE(I:I).EQ.',').OR.
     &       (LINE(I:I).EQ.'	')) THEN   
            I2 = I
            GO TO 25
         ENDIF
 20   CONTINUE
 25   CONTINUE
C
C    Determine if first non-space character is a minus sign.
C
      IF (LINE(I1:I1).EQ.'-') THEN
         ITS = I1+1
         IF (I1.EQ.I2-1) DEFAUL = .FALSE.
      ELSE
         ITS = I1
      ENDIF
C
C    Determine if first field contains non-numeric characters.
C
      DO 30 I=ITS,I2-1
         IF (INDEX('1234567890',LINE(I:I)).EQ.0) THEN
            DEFAUL = .FALSE.
            GO TO 35
         ENDIF
 30   CONTINUE
 35   CONTINUE
C
C    Set input/output filenames.
C
      IF (DEFAUL) THEN
C
C       Use default filenames.
C
         REFSFN = 'surgrd.body'
         REFCFN = 'surgrd.ixyz'
         OUTGFN = 'surgrd.xyz'
C
      ELSE
C
C       Read filenames from input parameters file.
C
         REFSFN = LINE(I1:I2-1)
         READ(*,'(A)',ERR=1010,END=1020) REFCFN
         READ(*,'(A)',ERR=1010,END=1020) OUTGFN
C
      ENDIF
C
C      WRITE(*,'(A)') REFSFN
C      WRITE(*,'(A)') REFCFN
C      WRITE(*,'(A)') OUTGFN
C
C    Check existence of input files.
C
      INQUIRE(FILE=REFSFN, EXIST=FILEX)
      IF (.NOT.FILEX) GO TO 1030
      INQUIRE(FILE=REFCFN, EXIST=FILEX)
      IF (.NOT.FILEX) GO TO 1040
c      INQUIRE(FILE=OUTGFN, EXIST=FILEX)
c      IF (FILEX) WRITE(*,201) OUTGFN
c 201  FORMAT('Warning: File ',A,' will be overwritten.')
C
C    Set logical for first line.
C
      FIRSTL = DEFAUL
C
      RETURN
C
C   Error on input.
C
 1010 CONTINUE
      TEXT  = 'Error on input.'
      NTEXT = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Premature end-of-file on input.
C
 1020 CONTINUE
      TEXT  = 'Premature end-of-file on input.'
      NTEXT = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Input reference surface file does not exist.
C
 1030 CONTINUE
      TEXT  = 'Cannot find input reference surface file.'
      NTEXT = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Input reference curves file does not exist.
C
 1040 CONTINUE
      TEXT  = 'Cannot find input reference curves file.'
      NTEXT = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Stop due to error.
C
 1050 CONTINUE
      CALL ERREND
      STOP 'RDINPA'
C
      END
C***********************************************************************
      SUBROUTINE RDGRPA(MRGRD,MNOD,NRGRD,NAGRD,NRCUR,
     &                  INIC,JMAX,JCMAX,IBCJA,IBCJB,IAFAM,NGBCA,NGBCB,
     &                  KMAX,NNOD,JNOD,ETAMX,DETA,DFAR,SMU,TIM,ITSVOL)
C
#include "precis.h"
C
C    Read grid parameters block for one grid.
C
      DIMENSION INIC(MRGRD), JMAX(MRGRD), JCMAX(NRCUR),
     &          IBCJA(MRGRD), IBCJB(MRGRD), IAFAM(MRGRD),
     &          NGBCA(MRGRD), NGBCB(MRGRD), KMAX(MRGRD), NNOD(MRGRD),
     &          JNOD(MNOD,MRGRD), ETAMX(MNOD,MRGRD), DETA(MNOD,MRGRD),
     &          DFAR(MNOD,MRGRD), SMU(MRGRD), TIM(MRGRD), ITSVOL(MRGRD)
C
      CHARACTER*80 TEXT(3)
C
      N = NAGRD
C
C       Check initial curve number.
C
         IF ( (INIC(N).EQ.0) .OR. (ABS(INIC(N)).GT.NRCUR) ) THEN
           WRITE(TEXT,202)
           NTEXT  = 1
           CALL ERRMSG(TEXT,NTEXT)
           GOTO 1050
         ENDIF
C
C       Set JMAX for active grid.
C
         ICNUM = ABS(INIC(N))
         JMAX(N) = JCMAX(ICNUM)
         WRITE(*,102) N
         IF (INIC(N).GT.0) THEN
            WRITE(*,104) INIC(N)
         ELSE
            WRITE(*,106) ABS(INIC(N))
         ENDIF
C
C       Read boundary conditions, family number.
C
c*wdh         READ(*,*,ERR=1010,END=1020) IBCJA(N),IBCJB(N),IAFAM(N)
         WRITE(*,112) IBCJA(N),IBCJB(N),IAFAM(N)
         IF ( ((IBCJA(N).GE.11).AND.(IBCJA(N).LE.15)) .OR.
     &        ((IBCJB(N).GE.11).AND.(IBCJB(N).LE.15)) .OR.
     &        (IBCJA(N).EQ.20) .OR. (IBCJB(N).EQ.20)  .OR.
     &        ((IBCJA(N).GE.31).AND.(IBCJA(N).LE.34)) .OR.
     &        ((IBCJB(N).GE.31).AND.(IBCJB(N).LE.34)) ) THEN
c*wdh            READ(*,*,ERR=1010,END=1020) NGBCA(N),NGBCB(N)
            WRITE(*,114) NGBCA(N),NGBCB(N)
         ELSE
            NGBCA(N) = 0
            NGBCB(N) = 0
         ENDIF
C
C       Check NGBCA/B.
C
         IF ( ((IBCJA(N).GE.11).AND.(IBCJA(N).LE.14)) .OR.
     &        ((IBCJA(N).GE.31).AND.(IBCJA(N).LE.34)) ) THEN
C
            IF (NGBCA(N).LT.0 .OR. NGBCA(N).GT.NRGRD) THEN
               TEXT(1)= 'Invalid reference grid number for NGBCA.'
               NTEXT  = 1
               CALL ERRMSG(TEXT,NTEXT)
               GOTO 1050
            ENDIF
C
         ELSE IF ((IBCJA(N).EQ.15).OR.(IBCJA(N).EQ.20)) THEN
C
            IF (ABS(NGBCA(N)).GT.NRCUR) THEN
              WRITE(TEXT,203)
              NTEXT  = 1
              CALL ERRMSG(TEXT,NTEXT)
              GOTO 1050
            ENDIF
C
         ENDIF
C
         IF ( ((IBCJB(N).GE.11).AND.(IBCJB(N).LE.14)) .OR.
     &        ((IBCJB(N).GE.31).AND.(IBCJB(N).LE.34)) ) THEN
C
            IF (NGBCB(N).LT.0 .OR. NGBCB(N).GT.NRGRD) THEN
               TEXT(1)= 'Invalid reference grid number for NGBCB.'
               NTEXT  = 1
               CALL ERRMSG(TEXT,NTEXT)
               GOTO 1050
            ENDIF
C
         ELSE IF ((IBCJB(N).EQ.15).OR.(IBCJB(N).EQ.20)) THEN
C
            IF (ABS(NGBCB(N)).GT.NRCUR) THEN
              WRITE(TEXT,203)
              NTEXT  = 1
              CALL ERRMSG(TEXT,NTEXT)
              GOTO 1050
            ENDIF
C
         ENDIF
C
C       Read number of points in marching direction and number of nodes.
C
c*wdh         READ(*,*,ERR=1010,END=1020) KMAX(N),NNOD(N)
         IF (NNOD(N).LT.1) NNOD(N) = 1
         WRITE(*,116) KMAX(N),NNOD(N)
         IF (NNOD(N).GT.MNOD) THEN
            WRITE(TEXT,204) NNOD(N)
            NTEXT  = 2
            CALL ERRMSG(TEXT,NTEXT)
            GOTO 1050
         ENDIF
C
C       Read end grid spacings, marching distance for each node.
C
         DO 10 NN = 1,NNOD(N)
c*wdh            READ(*,*,ERR=1010,END=1020)
c*wdh     &       JNOD(NN,N),ETAMX(NN,N),DETA(NN,N),DFAR(NN,N)
            WRITE(*,118) JNOD(NN,N),ETAMX(NN,N),DETA(NN,N),DFAR(NN,N)
 10      CONTINUE
C
C       Read smoothing parameters.
C
c*wdh         READ(*,*,ERR=1010,END=1020) SMU(N),TIM(N),ITSVOL(N)
         WRITE(*,120) SMU(N),TIM(N),ITSVOL(N)
C
 102  FORMAT(' Grid number ',I5)
 104  FORMAT(' Initial curve is reference curve # ',I5)
 106  FORMAT(' Initial curve is reverse of reference curve # ',I5)
 112  FORMAT('  IBCJA = ',I5,'     IBCJB = ',I5,'     IAFAM = ',I5)
 114  FORMAT('  NGBCA = ',I5,'     NGBCB = ',I5)
 116  FORMAT('  KMAX  = ',I5,'     NNOD  = ',I5)
 118  FORMAT('  JNOD  = ',I5,'  ETAMX = ',F13.5,
     &       '  DETA  = ',F13.5,'  DFAR  = ',F13.5)
 120  FORMAT('  SMU   = ',F13.5,'  TIM   = ',F13.5,'  ITSVOL= ',I5,/)
 202  FORMAT(' Invalid reference curve number in input.')
 203  FORMAT(' Invalid reference curve number in prescribed bndy. BC.')
 204  FORMAT(' Max number of nodes exceeded.'/
     &       ' Recompile with MNOD = ',I10)
C
      RETURN
C
C   Error on input.
C
 1010 CONTINUE
      TEXT(1)= 'Error on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Premature end-of-file on input.
C
 1020 CONTINUE
      TEXT(1)= 'Premature end-of-file on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Stop due to error.
C
 1050 CONTINUE
      CALL ERREND
      STOP 'RDGRPA'
C
      END
C***********************************************************************
      SUBROUTINE RDINPA(FIRSTL,LINE,MRGRD,MNOD,NRGRD,NRCUR,NAGRD,
     &                  INIC,JMAX,JCMAX,IRFAM,JRAXSA,JRAXSB,JRPER,KRPER,
     &                  IBCJA,IBCJB,IAFAM,NGBCA,NGBCB,
     &                  KMAX,NNOD,JNOD,ETAMX,DETA,DFAR,SMU,TIM,ITSVOL,
     &                  NSPBC,IAUC,NFAM)
C
#include "precis.h"
C
      DIMENSION INIC(MRGRD), JMAX(MRGRD), JCMAX(NRCUR), IRFAM(MRGRD), 
     &          JRAXSA(MRGRD),JRAXSB(MRGRD),JRPER(MRGRD),KRPER(MRGRD),
     &          IBCJA(MRGRD), IBCJB(MRGRD), IAFAM(MRGRD),
     &          NGBCA(MRGRD), NGBCB(MRGRD), KMAX(MRGRD), NNOD(MRGRD),
     &          JNOD(MNOD,MRGRD), ETAMX(MNOD,MRGRD), DETA(MNOD,MRGRD),
     &          DFAR(MNOD,MRGRD), SMU(MRGRD), TIM(MRGRD), ITSVOL(MRGRD)
C
      LOGICAL FIRSTL
      CHARACTER*120 LINE
      CHARACTER*80 TEXT(3)
      CHARACTER*4 FMT
C
C-----------------------------------------------------------------------
C    Read input parameters from file.
C-----------------------------------------------------------------------
C
C   IRSTYP = 1  Reference surface is in PLOT3D multiple grid format
C          = 2  Reference surface is in NIGES format
C
C   INIC   > 0  Use reference curve number IC as initial curve for current grid
C          < 0  Use reference curve number IC in reverse direction as initial
C               curve for current grid
C
C   Boundary condition types for J=1
C   IBCJA  = -1 Free floating condition
C          < -1 Splay (range = -1000 to -2)
C          =  1 Constant plane condition in X, float Y and Z
C          =  2 Constant plane condition in Y, float X and Z
C          =  3 Constant plane condition in Z, float X and Y
C          =  4 Reflected symmetry condition in X
C          =  5 Reflected symmetry condition in Y
C          =  6 Reflected symmetry condition in Z
C          = 10 Periodic condition (*)
C          = 11 Floating condition along KS=const line in +JS direction
C          = 12 Floating condition along KS=const line in -JS direction
C          = 13 Floating condition along JS=const line in +KS direction
C          = 14 Floating condition along JS=const line in -KS direction
C          = 15 Floating condition along user supplied line of points
C          = 20 Exact coordinates of boundary points prescribed by user
C          = 21 Constant X plane for all J from 1 to JMAX (*)
C          = 22 Constant Y plane for all J from 1 to JMAX (*)
C          = 23 Constant Z plane for all J from 1 to JMAX (*)
C          = 31 Float along KS=const line in +JS direction for all J (*)
C          = 32 Float along KS=const line in -JS direction for all J (*)
C          = 33 Float along JS=const line in +KS direction for all J (*)
C          = 34 Float along JS=const line in -KS direction for all J (*)
C
C   (*) Must also apply for IBCJB
C
C   Similarly for IBCJB at J=JMAX
C
C   IAFAM >0  Family number of family on which to project grid to
C
C   If IBCJA,IBCJB is 11-14 or 31-34 then enter NGBCA,NGBCB (**)
C   NGBCA = Reference grid number for floating constant JS,KS bc at J=1
C   NGBCB = Reference grid number for floating constant JS,KS bc at J=JMAX
C
C   If IBCJA,IBCJB is 15 or 20 then enter NGBCA,NGBCB (**)
C   NGBCA = Reference curve number at J=1
C   NGBCB = Reference curve number at J=JMAX
C
C   (**) If only one of IBCJA and IBCJB is in the above range, the NGBCA/NGBCB
C        corresponding to the other boundary is disregarded.
C
C   KMAX = Number of points in eta (marching direction)
C   NNOD <=1  Constant far field, initial/end grid spacing.
C        > 1  Number of nodes of piece-wise linear function for variable far
C             field distance, initial/end grid spacing.
C             (must be in range 2 < = NNOD < = MNOD, a constant far field,
C              distance, initial/end grid spacing is specified with NNOD=1)
C
C   Repeat the following for each node
C   Do N=1,NNOD
C    JNOD(N) = Node index of Nth node (-1 = last node, disregarded if NNOD=1)
C    ETAMX(N) = Far field distance at node N
C    DETA(N) = First grid point spacing in marching direction from initial curve
C              (or 0 for no spacing control)
C    DFAR(N) = Last grid point spacing in marching direction from initial curve
C              (or 0 for no spacing control)
C   Enddo
C
C   SMU    = Explicit smoothing coefficient O(1)
C   TIM    = TIM factor for smoothing in marching direction (0<=TIM<=3)
C   ITSVOL = Number of times prescribed areas DAREA(J) is smoothed
C
C   NSPBC  = 0 bc's of all reference grids to be determined automatically
C          > 0 number of grids to specify bc
C   IAUC   = 0  Do not automatically concatenate consecutive grids that share
C               a common initial curve
C          = 1  Automatically concatenate consecutive grids that share
C               a common initial curve
C          = -1 Do not do projection
C
C   Repeat the following for each reference grid that require bc specification.
C   Bc's of grids not specified here will be determined automatically.
C
C   IG     =     grid number of reference grid
C   IBCJS  = 0   non-periodic condition with no axis pts in J
C          = 1   axis point at J=1 only
C          = 2   axis point at J=jmax only
C          = 3   axis point at both J=1 and jmax
C          = 10  periodic condition in J
C   IBCKS  = 0   non-periodic condition with no axis pts in K
C          = 10  periodic condition in K
C
C   NFAM   > 0   Number of reference grid families
C          = 0   All input reference grids belong to same family, i.e family #1
C
C  If NFAM>0 read the following
C
C   IFNUM  = Family number
C   NGFAM  = Number of grids in IFNUM family
C   IGNUM  = Reference grid number in IFNUM family
C
C
      NAGRD = 0
c*wdh
        goto 60     
C
 100  CONTINUE
c*wdhC
c*wdhC    Parse first line of each block of input to determine if it is the
c*wdhC    last block.
c*wdhC
c*wdh      IF (.NOT.FIRSTL) READ(*,201,ERR=1010,END=1020) LINE
c*wdh      FIRSTL = .FALSE.
c*wdhC
c*wdhC    Convert all commas and tabs to spaces.
c*wdhC
c*wdh      DO 10 I=1,LEN(LINE)
c*wdh         IF ((LINE(I:I).EQ.',').OR.(LINE(I:I).EQ.'	'))
c*wdh     &        LINE(I:I) = ' '
c*wdh 10   CONTINUE
c*wdhC
c*wdhC    Find first non-space character.
c*wdhC
c*wdh      I1 = 0
c*wdh      DO 20 I=1,LEN(LINE)
c*wdh         IF (LINE(I:I).NE.' ') THEN
c*wdh            I1 = I
c*wdh            GO TO 25
c*wdh         ENDIF
c*wdh 20   CONTINUE
c*wdh 25   CONTINUE
c*wdhC
c*wdhC    Go read next line if the entire line consists of spaces.
c*wdhC
c*wdh      IF (I1.EQ.0) GO TO 100
c*wdhC
c*wdhC    Find location of next space.
c*wdhC
c*wdh      DO 30 I=I1+1,LEN(LINE)
c*wdh         IF (LINE(I:I).EQ.' ') THEN
c*wdh            I2 = I
c*wdh            GO TO 35
c*wdh         ENDIF
c*wdh 30   CONTINUE
c*wdh 35   CONTINUE
c*wdhC
c*wdhC    Find location of next non-space character.
c*wdhC
c*wdh      I3 = 0
c*wdh      DO 40 I=I2+1,LEN(LINE)
c*wdh         IF (LINE(I:I).NE.' ') THEN
c*wdh            I3 = I
c*wdh            GO TO 45
c*wdh         ENDIF
c*wdh 40   CONTINUE
c*wdh 45   CONTINUE
c*wdhC
c*wdh      IF (I3.NE.0) THEN
c*wdhC
c*wdhC      Find location of next space.
c*wdhC
c*wdh        I4 = LEN(LINE)
c*wdh        DO 50 I=I3+1,LEN(LINE)
c*wdh         IF (LINE(I:I).EQ.' ') THEN
c*wdh            I4 = I-1
c*wdh            GO TO 55
c*wdh         ENDIF
c*wdh 50     CONTINUE
c*wdh 55     CONTINUE
c*wdhC
c*wdhC      Read next record and see if it is an integer.
c*wdhC
c*wdh        NC = I4-I3+1
c*wdh        WRITE(FMT,202) '(I',NC,')'
c*wdh        READ( LINE(I3:I4), FMT, ERR=60 ) IAUC
c*wdhC
c*wdhC      Two integers in first line. Read final block.
c*wdh
c*wdh        NC = I2-I1
c*wdh        WRITE(FMT,202) '(I',NC,')'
c*wdh        READ( LINE(I1:I2-1), FMT ) NSPBC
        CALL RDFBPA(MRGRD,NRGRD,NAGRD,IRFAM,IAFAM,NGBCA,NGBCB,
     &              JRAXSA,JRAXSB,JRPER,KRPER,NSPBC,IAUC,NFAM)
C
C      Done reading input file. Exiting.
C
        GO TO 200
C
c*wdh      ENDIF
C
C    Only one integer in first line. Read grid parameters block.
C
 60   CONTINUE
      NAGRD = NAGRD + 1
      IF (NAGRD.GT.MRGRD) THEN
         WRITE(TEXT,210) NAGRD
         NTEXT  = 2
         CALL ERRMSG(TEXT,NTEXT)
         GOTO 1050
      ENDIF
      NC    = I2-I1
      WRITE(FMT,202) '(I',NC,')'
c*wdh      READ( LINE(I1:I2-1), FMT ) INIC(NAGRD)
      CALL RDGRPA(MRGRD,MNOD,NRGRD,NAGRD,NRCUR,
     &            INIC,JMAX,JCMAX,IBCJA,IBCJB,IAFAM,NGBCA,NGBCB,
     &            KMAX,NNOD,JNOD,ETAMX,DETA,DFAR,SMU,TIM,ITSVOL)
      GO TO 100
C
 200  CONTINUE
C
 201  FORMAT(A)
 202  FORMAT(A2,I1,A1)
 210  FORMAT(' Max number of active grids exceeded.'/
     &       ' Recompile with MRGRD = ',I6)
C
      RETURN
C
C   Error on input.
C
 1010 CONTINUE
      TEXT(1)= 'Error on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Premature end-of-file on input.
C
 1020 CONTINUE
      TEXT(1)= 'Premature end-of-file on input.'
      NTEXT  = 1
      CALL ERRMSG(TEXT,NTEXT)
      GOTO 1050
C
C   Stop due to error.
C
 1050 CONTINUE
      CALL ERREND
      STOP 'RDINPA'
C
      END
C***********************************************************************
      SUBROUTINE RHS(JMAX,KMAX,J1,J2,K,JPER,JJP,JJR,SMU,XX,YX,ZX,
     &               XE,YE,ZE,X,Y,Z,RH,R,XID,CAXI,DFN)
C
#include "precis.h"
C
      DIMENSION JJP(JMAX), JJR(JMAX)
      DIMENSION XX(JMAX),YX(JMAX),ZX(JMAX),XE(JMAX),YE(JMAX),ZE(JMAX)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), RH(JMAX,3)
      DIMENSION R(JMAX,KMAX), XID(JMAX,3), CAXI(JMAX), DFN(JMAX)
C
C    Form RHS forcing function and numerical dissipation
C
      DO 10 J = J1,J2
       RH(J,1)= XE(J)
       RH(J,2)= YE(J)
       RH(J,3)= ZE(J)
 10   CONTINUE
C
C    Compute rhs for angle-bisecting predictor at convex corners
C      DO 1 J=1,2
C      JP = JJP(J)
C      JR = JJR(J)
C      XJP = X(J,K)-X(JP,K)
C      YJP = Y(J,K)-Y(JP,K)
C      ZJP = Z(J,K)-Z(JP,K)
C      XJR = X(J,K)-X(JR,K)
C      YJR = Y(J,K)-Y(JR,K)
C      ZJR = Z(J,K)-Z(JR,K)
C      VJPN = SQRT(XJP**2 + YJP**2 + ZJP**2)
C      VJRN = SQRT(XJR**2 + YJR**2 + ZJR**2)
C      XM = XJP/VJPN + XJR/VJRN
C      YM = YJP/VJPN + YJR/VJRN
C      ZM = ZJP/VJPN + ZJR/VJRN
C      IF (CAXI(J).LT.0.0) THEN
C       SCA = 1.0
C      ELSE
C       SCA = SQRT(1.0-CAXI(J)**2)
C      ENDIF
C      write(*,*) j,k,caxi(j),sca
C      DEL = (R(J,K+1)-R(J,K))*SCA*0.1
C      VMN = DEL/SQRT(XM**2 + YM**2 + ZM**2)
C      RH(J,1) = XM*VMN
C      RH(J,2) = YM*VMN
C      RH(J,3) = ZM*VMN
C 1    CONTINUE
C
C    Compute explicit dissipation
C
      DO 30 J = J1+1-JPER,J2-1+JPER
       JP = JJP(J)
       JR = JJR(J)
       XID(J,1) = X(JP,K) - 2.*X(J,K) + X(JR,K)
       XID(J,2) = Y(JP,K) - 2.*Y(J,K) + Y(JR,K)
       XID(J,3) = Z(JP,K) - 2.*Z(J,K) + Z(JR,K)
 30   CONTINUE
      IF (JPER .EQ. 0) THEN
       DO 35 N=1,3
        XID(J1,N) = 0.
        XID(J2,N) = 0.
 35    CONTINUE
      ENDIF
C
C    Compute distance ratio function, aspect ratio factor, and
C    angle function for smoothing
C
      SMC = SMU*SQRT(FLOAT(K)/FLOAT(KMAX-1))
      KM1 = MAX(1,K-1)
      DO 40 J = J1+1-JPER,J2-1+JPER
       JP = JJP(J)
       JR = JJR(J)
       VRX = X(JR,K)-X(J,K)
       VRY = Y(JR,K)-Y(J,K)
       VRZ = Z(JR,K)-Z(J,K)
       VPX = X(JP,K)-X(J,K)
       VPY = Y(JP,K)-Y(J,K)
       VPZ = Z(JP,K)-Z(J,K)
       DM = SQRT( VRX**2 + VRY**2 + VRZ**2 )
       DP = SQRT( VPX**2 + VPY**2 + VPZ**2 )
       DM1 = SQRT( (X(JR,KM1)-X(J,KM1))**2 + (Y(JR,KM1)-Y(J,KM1))**2 +
     &             (Z(JR,KM1)-Z(J,KM1))**2 )
       DP1 = SQRT( (X(JP,KM1)-X(J,KM1))**2 + (Y(JP,KM1)-Y(J,KM1))**2 +
     &             (Z(JP,KM1)-Z(J,KM1))**2 )
       RAT = (DM1+DP1)/(DM+DP)
       DX =  XX(J)**2 + YX(J)**2 +ZX(J)**2
       DE =  XE(J)**2 + YE(J)**2 +ZE(J)**2
       XIDS = SQRT(DE/DX)
       IF (K.GE.2) THEN
        XN = X(J,K)-X(J,K-1)
        YN = Y(J,K)-Y(J,K-1)
        ZN = Z(J,K)-Z(J,K-1)
        DN = SQRT(XN*XN + YN*YN + ZN*ZN)
        DOTR = (VRX*XN +VRY*YN + VRZ*ZN)/(DM*DN)
        DOTP = (VPX*XN +VPY*YN + VPZ*ZN)/(DP*DN)
        COSA = 0.5*(DOTR+DOTP)
        IF (COSA.LE.0.0) THEN
         ANGF = 1.0
        ELSE
         ANGF = 1.0/(1.0-(COSA*COSA))
        ENDIF
       ELSE
        ANGF = 1.0
       ENDIF
       DFN(J) = ( MAX( RAT**(2.0/SMC), 0.1 ) )*XIDS*ANGF*SMC
 40   CONTINUE
C
      DO 60 J = J1+1-JPER,J2-1+JPER
C
C       DNN = XE(J)**2 + YE(J)**2 + ZE(J)**2
C       DND = XX(J)**2 + YX(J)**2 + ZX(J)**2
C       DN = SQRT(DNN/DND)
C
       RH(J,1) = RH(J,1) + DFN(J)*XID(J,1)
       RH(J,2) = RH(J,2) + DFN(J)*XID(J,2)
       RH(J,3) = RH(J,3) + DFN(J)*XID(J,3)
 60   CONTINUE
C
C    Implicit averaging for convex corners
C
      CVEX = -0.5
      DO 90 N = 1,3
      DO 90 J = J1+1-JPER,J2-1+JPER
       IF (CAXI(J).LT.CVEX) THEN
C
C        write(*,*) 'convex corner at j,k=',j,k
C
        RH(J,N) = 0.0
       ENDIF
 90   CONTINUE
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SFBNDY(M1D,JMAX,KMAX,IBCJA,IBCJB,X,Y,Z,R,XB,YB,ZB,TMP)
C
#include "precis.h"
C
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), R(JMAX,KMAX)
      DIMENSION XB(M1D,2), YB(M1D,2), ZB(M1D,2), TMP(M1D)
C
C    Construct stretching function from supplied boundary curves.
C
      IF ( (IBCJA.EQ.20) .AND. (IBCJB.EQ.20) ) THEN
C
C      First compute fractional arc length along initial curve.
       ARCT   = 0.0
       TMP(1) = 0.0
       DO 10 J = 2,JMAX
         ARC = SQRT( (X(J,1)-X(J-1,1))**2 + (Y(J,1)-Y(J-1,1))**2
     >             + (Z(J,1)-Z(J-1,1))**2 )
         ARCT   = ARCT + ARC
         TMP(J) = TMP(J-1) + ARC
 10    CONTINUE
       DO 20 J=2,JMAX-1
        TMP(J) = TMP(J)/ARCT
 20    CONTINUE
       TMP(JMAX) = 1.0
C
C      Blend stretching function from the 2 ends based on arc length.
       DO 30 J=1,JMAX
        R(J,1) = 0.0
 30    CONTINUE
       DA = 0.0
       DB = 0.0
       DO 40 K=2,KMAX
        ARCA = SQRT( (XB(K,1)-XB(K-1,1))**2 + (YB(K,1)-YB(K-1,1))**2 + 
     >               (ZB(K,1)-ZB(K-1,1))**2 )
        ARCB = SQRT( (XB(K,2)-XB(K-1,2))**2 + (YB(K,2)-YB(K-1,2))**2 + 
     >               (ZB(K,2)-ZB(K-1,2))**2 )
        DA   = DA + ARCA
        DB   = DB + ARCB
        DO 50 J=1,JMAX
         R(J,K) = (1.0-TMP(J))*DA + TMP(J)*DB
 50     CONTINUE
 40    CONTINUE
C
      ELSE
C
       D = 0.0
       J = 1
       R(J,1) = D
C
       IF ( (IBCJA.EQ.20) .AND. (IBCJB.NE.20) ) THEN
C
        DO 60 K=2,KMAX
         ARC = SQRT( (XB(K,1)-XB(K-1,1))**2 + (YB(K,1)-ZB(K-1,1))**2 + 
     >               (ZB(K,1)-ZB(K-1,1))**2 )
         D   = D + ARC
         R(J,K) = D
 60     CONTINUE
C
       ELSE IF ( (IBCJA.EQ.20) .AND. (IBCJB.NE.20) ) THEN
C
        DO 70 K=2,KMAX
         ARC = SQRT( (XB(K,2)-XB(K-1,2))**2 + (YB(K,2)-ZB(K-1,2))**2 + 
     >               (ZB(K,2)-ZB(K-1,2))**2 )
         D   = D + ARC
         R(J,K) = D
 70     CONTINUE
C
       ENDIF
C
       DO 80 J=2,JMAX
       DO 80 K=1,KMAX
        R(J,K) = R(1,K)
 80    CONTINUE
C
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SFCHEK(JMAX,KMAX,R)
C
#include "precis.h"
C
      PARAMETER (ONE=1.0)
      DIMENSION R(JMAX,KMAX)
C
C    Check stretching function
C
      JN = 1
      KN = 2
      JX = 1
      KX = 2
C
      IF (KMAX.LE.2) THEN
C
       RMIN = ONE
       RMAX = ONE
C
      ELSE
C
       RAT  = (R(1,3)-R(1,2))/(R(1,2)-R(1,1))
       RATA = MAX( RAT, ONE/RAT )
       RMIN = RATA
       RMAX = RATA
C
       DO 10 J=1,JMAX
       DO 10 K=2,KMAX-1
        RAT  = (R(J,K+1)-R(J,K))/(R(J,K)-R(J,K-1))
        RATA = MAX( RAT, ONE/RAT )
        IF (RATA.LT.RMIN) THEN
         RMIN = RATA
         JN   = J
         KN   = K
        ENDIF
        IF (RATA.GT.RMAX) THEN
         RMAX = RATA
         JX   = J
         KX   = K
        ENDIF
 10    CONTINUE
C
      ENDIF
C
      WRITE(*,101) RMIN,JN,KN
      WRITE(*,102) RMAX,JX,KX
 101  FORMAT('Min. stretching ratio = ',F12.5,' at ',2I5)
 102  FORMAT('Max. stretching ratio = ',F12.5,' at ',2I5)
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SMONOR(NJN,NKN,NJ,NK,JN,KN,J,K,
     &                  XNNU,YNNU,ZNNU,XNU,YNU,ZNU,XN,YN,ZN)
C
C   Modify normal at point (J,K) on boundary of grid N to blend with
C   normals of cell (JN,KN) of neighboring grid NN. Blending is not
C   performed if the normals are pointing in opposite directions
C   (specified by dot product tolerance), e.g., at airfoil trailing edges.
C
#include "precis.h"
C
      DIMENSION XNNU(NJN,NKN), YNNU(NJN,NKN), ZNNU(NJN,NKN)
      DIMENSION XNU(NJ,NK), YNU(NJ,NK), ZNU(NJ,NK)
      DIMENSION XN(NJ,NK), YN(NJ,NK), ZN(NJ,NK)
C
      PARAMETER (DOTBL = -0.7)
C
C
C   First compute average normal of neighboring cell.
C
      JNP       = MIN(JN+1,NJN)
      KNP       = MIN(KN+1,NKN)
      XNN       =
     & 0.25*(XNNU(JN,KN) + XNNU(JNP,KN) + XNNU(JN,KNP) + XNNU(JNP,KNP))
      YNN       =
     & 0.25*(YNNU(JN,KN) + YNNU(JNP,KN) + YNNU(JN,KNP) + YNNU(JNP,KNP))
      ZNN       =
     & 0.25*(ZNNU(JN,KN) + ZNNU(JNP,KN) + ZNNU(JN,KNP) + ZNNU(JNP,KNP))
      DNN       = SQRT(XNN**2 + YNN**2 + ZNN**2)
      IF (DNN.NE.0.) DNN       = 1./DNN
      XNN       = XNN*DNN
      YNN       = YNN*DNN
      ZNN       = ZNN*DNN
C
C   Then compute average of normal of neighboring cell with local
C   normal.
C
      DOT       = XNN*XNU(J,K) + YNN*YNU(J,K) + ZNN*ZNU(J,K)
      IF (DOT.GE.DOTBL) THEN
         XMOD      = 0.5*(XNN + XNU(J,K))
         YMOD      = 0.5*(YNN + YNU(J,K))
         ZMOD      = 0.5*(ZNN + ZNU(J,K))
         DMOD      = SQRT(XMOD**2 + YMOD**2 + ZMOD**2)
         IF (DMOD.NE.0.) DMOD      = 1./DMOD
         XN(J,K)   = XMOD*DMOD
         YN(J,K)   = YMOD*DMOD
         ZN(J,K)   = ZMOD*DMOD
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE STEP  (M1D,JKD,JMAX,KMAX,JPER,IBCJA,IBCJB,
     &                  JPLNA,JPLNB,JJP,JJR,J1,J2,K,
     &                  SMU,TIM,ITSVOL,
     &                  X,Y,Z,R,RH,DAREA,
     &                  XX,YX,ZX,XE,YE,ZE,A,B,C,F,H,U,
     &                  CAXI,XID,AVOL,TMP1,CSN1,CSN2,CSN3,
     &                  GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,XB,YB,ZB,ISEG,EV)
C
C   --------------------------------------------------------------------
C    March equations from K-1 to K using implicit scheme.
C   --------------------------------------------------------------------
C
#include "precis.h"
C
      DIMENSION JPLNA(3), JPLNB(3), JJP(JMAX), JJR(JMAX)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION R(JMAX,KMAX), RH(JMAX,3), DAREA(JMAX)
      DIMENSION XX(JMAX),YX(JMAX),ZX(JMAX),XE(JMAX),YE(JMAX),ZE(JMAX)
      DIMENSION A(JKD,3,3), B(JKD,3,3), C(JKD,3,3), F(JKD,3),
     &          H(JKD,3,3), U(JKD,3,3)
      DIMENSION CAXI(JMAX), XID(JMAX,3), AVOL(JMAX), TMP1(JMAX)
      DIMENSION CSN1(JMAX,KMAX), CSN2(JMAX,KMAX), CSN3(JMAX,KMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX),
     &          XSNR(JMAX), YSNR(JMAX), ZSNR(JMAX)
      DIMENSION XB(M1D,2), YB(M1D,2), ZB(M1D,2), ISEG(2),
     &          EV(JMAX,KMAX,3)
      DIMENSION XBP(2), YBP(2), ZBP(2)
C
C
       IF ( ( (IBCJA.GE.11) .AND. (IBCJA.LE.20) ) .OR.
     &      ( (IBCJB.GE.11) .AND. (IBCJB.LE.20) ) ) THEN
C
C       Compute grid points on boundaries for fixed boundary BC
C
         IMODE = 1
         IF ((IBCJA.GE.11).AND.(IBCJA.LE.15)) THEN
          IB = 1
          CALL CMPFIX(JMAX,KMAX,M1D,J1,K,IB,X,Y,Z,
     &                XB,YB,ZB,XBP,YBP,ZBP,R,ISEG,IMODE)
         ELSE IF (IBCJA.EQ.20) THEN
          XBP(1) = XB(K,1)
          YBP(1) = YB(K,1)
          ZBP(1) = ZB(K,1)
         ENDIF
         IF ((IBCJB.GE.11).AND.(IBCJB.LE.15)) THEN
          IB = 2
          CALL CMPFIX(JMAX,KMAX,M1D,J2,K,IB,X,Y,Z,
     &                XB,YB,ZB,XBP,YBP,ZBP,R,ISEG,IMODE)
         ELSE IF (IBCJB.EQ.20) THEN
          XBP(2) = XB(K,2)
          YBP(2) = YB(K,2)
          ZBP(2) = ZB(K,2)
         ENDIF
C
       ENDIF
C
C    Compute metrics and rhs
C
      CALL METRXI(JMAX,KMAX,J1,J2,K-1,JJP,JJR,JPER,X,Y,Z,XX,YX,ZX)
C
      CALL BINV(JMAX,JKD,J1,J2,XX,YX,ZX,GSN1,GSN2,GSN3,U)
C
      CALL CONARE(JMAX,KMAX,K,JPER,ITSVOL,JJP,JJR,
     &            XX,YX,ZX,R,DAREA,AVOL)
C
      CALL METRET(JMAX,KMAX,JKD,J1,J2,K-1,JJP,JJR,JPER,X,Y,Z,
     &            XX,YX,ZX,GSN1,GSN2,GSN3,DAREA,U,XE,YE,ZE)
C
      CALL METCOR(JMAX,KMAX,JKD,J1+1-JPER,J2-1+JPER,K-1,JJP,JJR,JPER,
     &            X,Y,Z,XX,YX,ZX,GSN1,GSN2,GSN3,DAREA,U,XE,YE,ZE)
C
      CALL METBLN(JMAX,KMAX,K-1,IBCJA,IBCJB,X,Y,Z,XE,YE,ZE,
     &            GSN1,GSN2,GSN3,JPLNA,JPLNB,XBP,YBP,ZBP)
C
      DO 99 J=1,JMAX
       EV(J,K-1,1) = XE(J)
       EV(J,K-1,2) = YE(J)
       EV(J,K-1,3) = ZE(J)
 99   CONTINUE
C
      CALL ANGLES(JMAX,KMAX,J1+1-JPER,J2-1+JPER,K-1,JJP,JJR,
     &            GSN1,GSN2,GSN3,X,Y,Z,CAXI)
C
      CALL RHS(JMAX,KMAX,J1,J2,K-1,JPER,JJP,JJR,SMU,XX,YX,ZX,
     &         XE,YE,ZE,X,Y,Z,RH,R,XID,CAXI,TMP1)
C
C    Fill matrices in interior
C
      CALL AMATRX(JMAX,JKD,J1,J2,GSN1,GSN2,GSN3,XE,YE,ZE,A)
C
      CALL BINVA(JKD,J1,J2,A,U,H)
C
      CALL FILTRX(JMAX,JKD,J1,J2,SMU,TIM,CAXI,H,RH,TMP1,A,B,C,F)
C
C    Fill matrices at boundaries and call block solvers
C
      IF (JPER.EQ.0) THEN
C
       EXTJA = 0
       EXTJB = 0
       IF (IBCJA.LT.-1) EXTJA = -FLOAT(IBCJA)/1000.0
       IF (IBCJB.LT.-1) EXTJB = -FLOAT(IBCJB)/1000.0
C
       CALL BCMAT(JMAX,KMAX,JKD,IBCJA,IBCJB,J1,J2,K,JPLNA,JPLNB,
     &            JFLAGA,JFLAGB,EXTJA,EXTJB,X,Y,Z,A,B,C,F,
     &            XBP,YBP,ZBP)
C
       CALL BTRI(JKD,J1,J2,JFLAGA,JFLAGB,A,B,C,F,H,U)
C
      ELSE IF (JPER.EQ.1) THEN
C
       CALL BTRIP(JKD,J1,J2,A,B,C,F,H,U)
C
      ENDIF
C
C    Update
C
      IF ( (IBCJA.GE.21).AND.(IBCJA.LE.23) ) THEN
       DO 30 J=J1,J2
        X(J,K) = F(J,1)*JPLNA(1) + X(J,K-1)
        Y(J,K) = F(J,2)*JPLNA(2) + Y(J,K-1)
        Z(J,K) = F(J,3)*JPLNA(3) + Z(J,K-1)
 30    CONTINUE
      ELSE
       DO 40 J=J1,J2
        X(J,K) = F(J,1) + X(J,K-1)
        Y(J,K) = F(J,2) + Y(J,K-1)
        Z(J,K) = F(J,3) + Z(J,K-1)
 40    CONTINUE
      ENDIF
      IF (JPER.EQ.1) THEN
       X(JMAX,K) = X(1,K)
       Y(JMAX,K) = Y(1,K)
       Z(JMAX,K) = Z(1,K)
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE STEPI(JMAX,KMAX,JKD,IRSTYP,K,R,X,Y,Z,JBMAX,ICUR,
     &                 XI,YI,ZI)
C
#include "precis.h"
C
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
      DIMENSION R(JMAX,KMAX)
      DIMENSION JBMAX(JMAX), ICUR(JMAX)
      DIMENSION XI(JKD,JMAX), YI(JKD,JMAX), ZI(JKD,JMAX)
C
C   March one step along constant parameter curves.
C
      IF (IRSTYP.EQ.1) THEN
C
C      March along constant xi_s or eta_s curves.
C
       DO 20 J = 1,JMAX
         DS     = R(J,K) - R(J,K-1)
         I      = ICUR(J)
         XCUR   = X(J,K-1)
         YCUR   = Y(J,K-1)
         ZCUR   = Z(J,K-1)
C
   10    CONTINUE
         DXN    = XI(I+1,J) - XCUR
         DYN    = YI(I+1,J) - YCUR
         DZN    = ZI(I+1,J) - ZCUR
         DNODE  = SQRT(DXN*DXN + DYN*DYN + DZN*DZN)
C
         IF (DS.GT.DNODE) THEN
            DS     = DS - DNODE
            XCUR   = XI(I+1,J)
            YCUR   = YI(I+1,J)
            ZCUR   = ZI(I+1,J)
            ICUR(J)= I+1
            I      = I+1
            GOTO 10
         ENDIF
C
         DM     = DS/DNODE
         X(J,K) = XCUR + DM*DXN
         Y(J,K) = YCUR + DM*DYN
         Z(J,K) = ZCUR + DM*DZN
   20  CONTINUE
C
      ELSE IF (IRSTYP.EQ.2) THEN
C
C      March along isoparametric curve of NURBS surface.
C
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SURBCS(M1D,NRGRD,NTOTR,NGPR,NGNR,JRMAX,KRMAX,
     &                  JRPER,KRPER,JRAXSA,JRAXSB,JJP,JJM,KKP,KKM,IRFAM,
     &                  XR,YR,ZR)
C
C   Determine BC's for reference surface grids and swap indices and
C   reverse if needed, e.g., if axis in K is found, make it J.
C
C   Use the second level of arrays XR,YR,ZR as temporary space in case
C   we need to swap J and K here.  
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JJP(M1D,NRGRD), JJM(M1D,NRGRD),
     &          KKP(M1D,NRGRD), KKM(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION IRFAM(NRGRD)
C
      LOGICAL JPER,JAXSA,JAXSB
      LOGICAL KPER,KAXSA,KAXSB
      PARAMETER (I1=1)
C
C
C   Put out a table of topology information.
C
      WRITE(*,*)
      WRITE(*,*) 'Reference surface information table'
      WRITE(*,*)
      WRITE(*,*) 'GRID #  JRPER  KRPER  JRAXSA  JRAXSB  ISWAP  IRFAM'
C
      DO 10 N = 1,NRGRD
         ISWAP  = 0
         IS     = NGPR(N)
         IS2    = IS + NGNR(N)
C
C   Check the grid topology.
C
         IF (JRPER(N).EQ.-1) THEN
            CALL TOPLGY(JRMAX(N),KRMAX(N),XR(IS),YR(IS),ZR(IS),
     &                  JJP(1,N),JJM(1,N),KKP(1,N),KKM(1,N),
     &                  JPER,JAXSA,JAXSB,JSYMA,JSYMB,
     &                  KPER,KAXSA,KAXSB,KSYMA,KSYMB)
C
C   We can't handle axes in K.  Swap J and K and reset the topology.
C
            IF (KAXSA.OR.KAXSB) THEN
               ISWAP  = 1
               CALL SWAPR (JRMAX(N),KRMAX(N),XR(IS),YR(IS),ZR(IS),
     &                     XR(IS2),YR(IS2),ZR(IS2))
               CALL TOPLGY(JRMAX(N),KRMAX(N),XR(IS),YR(IS),ZR(IS),
     &                     JJP(1,N),JJM(1,N),KKP(1,N),KKM(1,N),
     &                     JPER,JAXSA,JAXSB,JSYMA,JSYMB,
     &                     KPER,KAXSA,KAXSB,KSYMA,KSYMB)
            ENDIF
C
C   Convert the logical flags to 0/1 flags.
C
            JRPER(N)  = 0
            IF (JPER)  JRPER(N)  = 1
            JRAXSA(N) = 0
            IF (JAXSA) JRAXSA(N) = 1
            JRAXSB(N) = 0
            IF (JAXSB) JRAXSB(N) = 1
            KRPER(N)  = 0
            IF (KPER)  KRPER(N)  = 1
C
C   Use the specified topology as-is.
C
         ELSE
C
C   Make sure boundary is periodic (don't fiddle with the other
C   attributes).
C
            JPER   = .FALSE.
            IF (JRPER(N).EQ.1)  JPER   = .TRUE.
            KPER   = .FALSE.
            IF (KRPER(N).EQ.1)  KPER   = .TRUE.
            JAXSA  = .FALSE.
            IF (JRAXSA(N).EQ.1) JAXSA  = .TRUE.
            JAXSB  = .FALSE.
            IF (JRAXSB(N).EQ.1) JAXSB  = .TRUE.
            KAXSA  = .FALSE.
            KAXSB  = .FALSE.
            JSYMA  = 0
            JSYMB  = 0
            KSYMA  = 0
            KSYMB  = 0
            CALL RSTTOP(JRMAX(N),KRMAX(N),I1,XR(IS),YR(IS),ZR(IS),
     &                  JJP(1,N),JJM(1,N),KKP(1,N),KKM(1,N),
     &                  JPER,JAXSA,JAXSB,JSYMA,JSYMB,
     &                  KPER,KAXSA,KAXSB,KSYMA,KSYMB)
         ENDIF
C
C   Write the information for this grid.
C
         WRITE(*,1) N,JRPER(N),KRPER(N),JRAXSA(N),JRAXSB(N),ISWAP,
     &                IRFAM(N)
    1    FORMAT(I5,2I7,2I8,2I7)
   10    CONTINUE
      WRITE(*,*)
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SURCPY(NJ,NK,XNRU,YNRU,ZNRU,XNR,YNR,ZNR)
C
C   Copy normals from (XNRU,YNRU,ZNRU) to (XNR,YNR,ZNR).
C
#include "precis.h"
C
      DIMENSION XNRU(NJ,NK), YNRU(NJ,NK), ZNRU(NJ,NK)
      DIMENSION XNR(NJ,NK), YNR(NJ,NK), ZNR(NJ,NK)
C
C
      DO 10 K = 1,NK
      DO 10 J = 1,NJ
         XNR(J,K) = XNRU(J,K)
         YNR(J,K) = YNRU(J,K)
         ZNR(J,K) = ZNRU(J,K)
   10    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SURFE (M1D,NRGRD,NTOTR,JMAX,KMAX,NGPR,NGNR,NGP1,
     &                  JRMAX,KRMAX,J1,J2,K,
     &                  IRSTYP,IMODEL,JRPER,KRPER,JRAXSA,JRAXSB,
     &                  IBCJA,IBCJB,IAFAM,JPER,JRP,JRR,KRP,KRR,
     &                  X,Y,Z,R,XR,YR,ZR,
     &                  XNR,YNR,ZNR,IGPB,IRFAM,GSN1,GSN2,GSN3,GNV,
     &                  XSNR,YSNR,ZSNR,JKIH,DELS,ISSTAT)
C
C   Project down to the surface(?).
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), NGP1(NRGRD),
     &          JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION JRP(M1D,NRGRD),JRR(M1D,NRGRD),
     &          KRP(M1D,NRGRD),KRR(M1D,NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XNR(NTOTR), YNR(NTOTR), ZNR(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
C
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX), R(JMAX,KMAX)
      DIMENSION GSN1(JMAX), GSN2(JMAX), GSN3(JMAX),
     &          XSNR(JMAX), YSNR(JMAX), ZSNR(JMAX)
      DIMENSION JKIH(JMAX,3), DELS(JMAX,3), GNV(JMAX,KMAX,3)
c*wdh
      external mappro !$pragma C( mappro )
c*wdh
C
C
      IF (IRSTYP.EQ.1) THEN
C
C      Project on to multiple panel networks
C
        CALL FNDSPN(M1D,NRGRD,NTOTR,JMAX,KMAX,NGPR,NGP1,JRMAX,KRMAX,
     &              J1,J2,K,JPER,JRPER,KRPER,JRAXSA,JRAXSB,
     &              JRP,JRR,KRP,KRR,X,Y,Z,R,XR,YR,ZR,
     &              XNR,YNR,ZNR,IGPB,IRFAM,IAFAM,GNV,
     &              GSN1,GSN2,GSN3,XSNR,YSNR,ZSNR,JKIH)
C
        JBEG      = J1
        JEND      = J2
        IF ( (IBCJA.GE.15).AND.(IBCJA.LE.20) ) JBEG      = J1+1
        IF ( (IBCJB.GE.15).AND.(IBCJB.LE.20) ) JEND      = J2-1
        IF (ISSTAT.EQ.2) THEN
           JBEG      = J1+1
           JEND      = J2-1
        ENDIF
        DO 20 J = JBEG,JEND
          X(J,K)    = XSNR(J)
          Y(J,K)    = YSNR(J)
          Z(J,K)    = ZSNR(J)
 20     CONTINUE
        IF (JPER.EQ.1) THEN
          X(JMAX,K) = X(1,K)
          Y(JMAX,K) = Y(1,K)
          Z(JMAX,K) = Z(1,K)
        ENDIF

c*wdh
c       Project onto the surface
c dels -- unit square coords -- not used!
       call mappro( jmax,kmax,k,x(1,k),y(1,k),z(1,k),gnv,dels )
c*wdh

C
      ELSE IF (IRSTYP.EQ.2) THEN
C
C      Project on to NURBS surfaces
C
c*wdh
c       Project onto the surface
c dels -- unit square coords -- not used!
         call mappro( jmax,kmax,k,x(1,k),y(1,k),z(1,k),gnv,dels )
         do j=1,jmax
            gsn1(j)=gnv(j,k,1)
            gsn2(j)=gnv(j,k,2)
            gsn3(j)=gnv(j,k,3) 
         end do
c*wdh
#ifdef SUPERLIB
        CALL IGSPRN(JMAX,K,X(1,K),Y(1,K),Z(1,K),GSN1,GSN2,GSN3,
     &              DELS(1,1),DELS(1,2),IMODEL)

        DO 30 J=1,JMAX
         GNV(J,K,1) = GSN1(J)
         GNV(J,K,2) = GSN2(J)
         GNV(J,K,3) = GSN3(J)
 30     CONTINUE
#endif
C
      ENDIF
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SURNHB(M1D,NRGRD,NTOTR,NGPR,NGNR,NGP1,
     &                  JRMAX,KRMAX,JRPER,KRPER,JRAXSA,JRAXSB,XR,YR,ZR,
     &                  XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX,
     &                  XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX,IGPB,IRFAM)
C
C    Set up pointer array IGPB: for each point on the boundary of each
C    grid, the grid number and J,K cell index of the neighboring grid is
C    stored. A zero is stored if there is no neighboring grid, i.e.,
C    true grid boundary is reached, or if it is a periodic boundary.
C
C    IGPB first index  : J or K
C    IGPB second index : 1 - J=1, 2 - J=JMAX, 3 - K=1, 4 - K=KMAX,
C    IGPB third index  : 1 - grid no., 2 - J index, 3 - K index
C
#include "precis.h"
C
      DIMENSION NGPR(NRGRD), NGNR(NRGRD), NGP1(NRGRD),
     &          JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION JRPER(NRGRD), KRPER(NRGRD), JRAXSA(NRGRD), JRAXSB(NRGRD)
      DIMENSION XR(NTOTR*2), YR(NTOTR*2), ZR(NTOTR*2)
      DIMENSION XBMIN(NRGRD), YBMIN(NRGRD), ZBMIN(NRGRD)
      DIMENSION XBMAX(NRGRD), YBMAX(NRGRD), ZBMAX(NRGRD)
      DIMENSION XSMIN(NTOTR), YSMIN(NTOTR), ZSMIN(NTOTR)
      DIMENSION XSMAX(NTOTR), YSMAX(NTOTR), ZSMAX(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
C
      PARAMETER (I1=1,I2=2,I3=3,I4=4)
C
C
C   Initialize everything to zero first.
C
      DO 10 N = 1,NRGRD
      DO 10 II = 1,3
      DO 10 IB = 1,4
      DO 10 J = 1,M1D
         IGPB(J,IB,II,N) = 0
   10    CONTINUE
C
C   We only have to check connections if we have more than one reference
C   grid.
C
      IF (NRGRD.GT.1) THEN
C
C   Compute big minmax box for each grid.
C
         CALL CMPBMM(NRGRD,NTOTR,NGPR,NGNR,JRMAX,KRMAX,XR,YR,ZR,
     &               XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX)
C
C   Compute small minmax box for each cell of each grid.
C
         CALL CMPSMM(NRGRD,NTOTR,NGPR,NGP1,JRMAX,KRMAX,XR,YR,ZR,
     &               XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX)
C
C   Go through each non-periodic boundary of each grid and fill IGPB.
C
         DO 100 N = 1,NRGRD
C
            ISM = NGPR(N) - 1
C
            IF (JRPER(N).NE.1) THEN
               J      = 1
               DO 20 K = 1,KRMAX(N)-1
                  IP1    = (K-1)*JRMAX(N) + J + ISM
                  IP2    = (K  )*JRMAX(N) + J + ISM
                  CALL FNDGJK(M1D,NRGRD,NTOTR,NGP1,JRMAX,KRMAX,
     &                        IP1,IP2,K,I1,N,XR,YR,ZR,
     &                        XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX,
     &                        XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX,
     &                        IGPB,IRFAM)
   20             CONTINUE
C
               J      = JRMAX(N)
               DO 30 K = 1,KRMAX(N)-1
                  IP1    = (K-1)*JRMAX(N) + J + ISM
                  IP2    = (K  )*JRMAX(N) + J + ISM
                  CALL FNDGJK(M1D,NRGRD,NTOTR,NGP1,JRMAX,KRMAX,
     &                        IP1,IP2,K,I2,N,XR,YR,ZR,
     &                        XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX,
     &                        XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX,
     &                        IGPB,IRFAM)
   30             CONTINUE
            ENDIF
C
            IF (KRPER(N).NE.1) THEN
               K      = 1
               DO 40 J = 1,JRMAX(N)-1
                  IP1    = (K-1)*JRMAX(N) + J + ISM
                  IP2    = IP1 + 1
                  CALL FNDGJK(M1D,NRGRD,NTOTR,NGP1,JRMAX,KRMAX,
     &                        IP1,IP2,J,I3,N,XR,YR,ZR,
     &                        XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX,
     &                        XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX,
     &                        IGPB,IRFAM)
   40             CONTINUE
C
               K      = KRMAX(N)
               DO 50 J = 1,JRMAX(N)-1
                  IP1    = (K-1)*JRMAX(N) + J + ISM
                  IP2    = IP1 + 1
                  CALL FNDGJK(M1D,NRGRD,NTOTR,NGP1,JRMAX,KRMAX,
     &                        IP1,IP2,J,I4,N,XR,YR,ZR,
     &                        XBMIN,YBMIN,ZBMIN,XBMAX,YBMAX,ZBMAX,
     &                        XSMIN,YSMIN,ZSMIN,XSMAX,YSMAX,ZSMAX,
     &                        IGPB,IRFAM)
   50             CONTINUE
            ENDIF
C
C   Fill in corners of periodic boundary.
C
C   Don't really think I should do this...the section below essentially
C   undoes it anyway.
C
C           IF (JRPER(N).EQ.1) THEN
C              DO 60 II = 1,3
C                 IGPB(1,1,II,N)          = IGPB(1,3,II,N)
C                 IGPB(KRMAX(N)-1,1,II,N) = IGPB(1,4,II,N)
C                 IGPB(1,2,II,N)          = IGPB(JRMAX(N)-1,3,II,N)
C                 IGPB(KRMAX(N)-1,2,II,N) = IGPB(JRMAX(N)-1,4,II,N)
C  60             CONTINUE
C           ENDIF
C           IF (KRPER(N).EQ.1) THEN
C              DO 70 II = 1,3
C                 IGPB(1,3,II,N)          = IGPB(1,1,II,N)
C                 IGPB(JRMAX(N)-1,3,II,N) = IGPB(1,2,II,N)
C                 IGPB(1,4,II,N)          = IGPB(KRMAX(N)-1,1,II,N)
C                 IGPB(JRMAX(N)-1,4,II,N) = IGPB(KRMAX(N)-1,2,II,N)
C  70             CONTINUE
C           ENDIF
  100       CONTINUE
C
C   Fix up the corners as follows: if IGPB is set for a corner point and
C   it points to the same cell as the corresponding corner point from
C   the neighboring edge, AND IGPB for the next point in is zero, set
C   IGPB for the current corner to zero.
C
         DO 200 N = 1,NRGRD
            J1     = 2
            IBJ    = 1
            K1     = 1
            KK1    = K1+1
            IBK    = 3
            IF (IGPB(K1,IBJ,1,N).NE.0 .AND. IGPB(J1,IBK,1,N).NE.0
     &                                .AND. IGPB(KK1,IBJ,1,N).EQ.0) THEN
               IGPB(K1,IBJ,1,N) = 0
               IGPB(K1,IBJ,2,N) = 0
               IGPB(K1,IBJ,3,N) = 0
            ENDIF
            K1     = KRMAX(N)-1
            KK1    = K1-1
            IBK    = 4
            IF (IGPB(K1,IBJ,1,N).NE.0 .AND. IGPB(J1,IBK,1,N).NE.0
     &                                .AND. IGPB(KK1,IBJ,1,N).EQ.0) THEN
               IGPB(K1,IBJ,1,N) = 0
               IGPB(K1,IBJ,2,N) = 0
               IGPB(K1,IBJ,3,N) = 0
            ENDIF
            J1     = JRMAX(N)-2
            IBJ    = 2
            K1     = 1
            KK1    = K1+1
            IBK    = 3
            IF (IGPB(K1,IBJ,1,N).NE.0 .AND. IGPB(J1,IBK,1,N).NE.0
     &                                .AND. IGPB(KK1,IBJ,1,N).EQ.0) THEN
               IGPB(K1,IBJ,1,N) = 0
               IGPB(K1,IBJ,2,N) = 0
               IGPB(K1,IBJ,3,N) = 0
            ENDIF
            K1     = KRMAX(N)-1
            KK1    = K1-1
            IBK    = 4
            IF (IGPB(K1,IBJ,1,N).NE.0 .AND. IGPB(J1,IBK,1,N).NE.0
     &                                .AND. IGPB(KK1,IBJ,1,N).EQ.0) THEN
               IGPB(K1,IBJ,1,N) = 0
               IGPB(K1,IBJ,2,N) = 0
               IGPB(K1,IBJ,3,N) = 0
            ENDIF
C
            K1     = 2
            IBK    = 3
            J1     = 1
            JJ1    = J1+1
            IBJ    = 1
            IF (IGPB(J1,IBK,1,N).NE.0 .AND. IGPB(K1,IBJ,1,N).NE.0
     &                                .AND. IGPB(JJ1,IBK,1,N).EQ.0) THEN
               IGPB(J1,IBK,1,N) = 0
               IGPB(J1,IBK,2,N) = 0
               IGPB(J1,IBK,3,N) = 0
            ENDIF
            J1     = JRMAX(N)-1
            JJ1    = J1-1
            IBJ    = 2
            IF (IGPB(J1,IBK,1,N).NE.0 .AND. IGPB(K1,IBJ,1,N).NE.0
     &                                .AND. IGPB(JJ1,IBK,1,N).EQ.0) THEN
               IGPB(J1,IBK,1,N) = 0
               IGPB(J1,IBK,2,N) = 0
               IGPB(J1,IBK,3,N) = 0
            ENDIF
            K1     = KRMAX(N)-2
            IBK    = 4
            J1     = 1
            JJ1    = J1+1
            IBJ    = 1
            IF (IGPB(J1,IBK,1,N).NE.0 .AND. IGPB(K1,IBJ,1,N).NE.0
     &                                .AND. IGPB(JJ1,IBK,1,N).EQ.0) THEN
               IGPB(J1,IBK,1,N) = 0
               IGPB(J1,IBK,2,N) = 0
               IGPB(J1,IBK,3,N) = 0
            ENDIF
            J1     = JRMAX(N)-1
            JJ1    = J1-1
            IBJ    = 2
            IF (IGPB(J1,IBK,1,N).NE.0 .AND. IGPB(K1,IBJ,1,N).NE.0
     &                                .AND. IGPB(JJ1,IBK,1,N).EQ.0) THEN
               IGPB(J1,IBK,1,N) = 0
               IGPB(J1,IBK,2,N) = 0
               IGPB(J1,IBK,3,N) = 0
            ENDIF
  200       CONTINUE
      ENDIF
#ifdef DEBUG
C
C   Count number of zeros at each boundary and report questionable info.
C
      OPEN(UNIT=50,FILE='surnhb.dbg',STATUS='UNKNOWN',FORM='FORMATTED')
      DO 500 N = 1,NRGRD
         IF (JRPER(N).NE.1) THEN
            IF (JRAXSA(N).NE.1) THEN
               N0     = 0
               DO 410 K = 1,KRMAX(N)-1
                  IF (IGPB(K,1,1,N).EQ.0) N0     = N0+1
  410             CONTINUE
               IF (N0.NE.KRMAX(N)-1 .AND. N0.NE.0) WRITE(50,411) N,N0
  411          FORMAT('Questionable boundary at J=1   , grid ',I5,
     &                '  zeros = ',I5)
            ENDIF
            IF (JRAXSB(N).NE.1) THEN
               N0     = 0
               DO 420 K = 1,KRMAX(N)-1
                  IF (IGPB(K,2,1,N).EQ.0) N0     = N0+1
  420             CONTINUE
               IF (N0.NE.KRMAX(N)-1 .AND. N0.NE.0) WRITE(50,421) N,N0
  421          FORMAT('Questionable boundary at J=JMAX, grid ',I5,
     &                '  zeros = ',I5)
            ENDIF
         ENDIF
C
         IF (KRPER(N).NE.1) THEN
               N0     = 0
               DO 430 J = 1,JRMAX(N)-1
                  IF (IGPB(J,3,1,N).EQ.0) N0     = N0+1
  430             CONTINUE
               IF (N0.NE.JRMAX(N)-1 .AND. N0.NE.0) WRITE(50,431) N,N0
  431          FORMAT('Questionable boundary at K=1   , grid ',I5,
     &                '  zeros = ',I5)
               N0     = 0
               DO 440 J = 1,JRMAX(N)-1
                  IF (IGPB(J,4,1,N).EQ.0) N0     = N0+1
  440             CONTINUE
               IF (N0.NE.JRMAX(N)-1 .AND. N0.NE.0) WRITE(50,441) N,N0
  441          FORMAT('Questionable boundary at K=KMAX, grid ',I5,
     &                '  zeros = ',I5)
         ENDIF
  500    CONTINUE
C
      WRITE(50,*)
      WRITE(50,*)
      DO 600 N = 1,NRGRD
         WRITE(50,*) 'grid # ',N
         WRITE(50,*) 'j=1'
         DO 510 K = 1,KRMAX(N)-1
            WRITE(50,*) IGPB(K,1,1,N),IGPB(K,1,2,N),IGPB(K,1,3,N)
  510       CONTINUE
         WRITE(50,*) 'j=jmax'
         DO 520 K = 1,KRMAX(N)-1
            WRITE(50,*) IGPB(K,2,1,N),IGPB(K,2,2,N),IGPB(K,2,3,N)
  520       CONTINUE
         WRITE(50,*) 'k=1'
         DO 530 J = 1,JRMAX(N)-1
            WRITE(50,*) IGPB(J,3,1,N),IGPB(J,3,2,N),IGPB(J,3,3,N)
  530       CONTINUE
         WRITE(50,*) 'k=kmax'
         DO 540 J = 1,JRMAX(N)-1
            WRITE(50,*) IGPB(J,4,1,N),IGPB(J,4,2,N),IGPB(J,4,3,N)
  540       CONTINUE
  600    CONTINUE
      CLOSE(50)
#endif
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SURNMM(NRGRD,M1D,NTOTR,NGP1,JRMAX,KRMAX,JSPER,KSPER,
     &                  JSAXSA,JSAXSB,N,XNRU,YNRU,ZNRU,XNR,YNR,ZNR,
     &                  IGPB,IRFAM)
C
C   Modify surface normals at grid boundaries to match with neighbors.
C
#include "precis.h"
C
      DIMENSION NGP1(NRGRD), JRMAX(NRGRD), KRMAX(NRGRD)
      DIMENSION XNRU(NTOTR), YNRU(NTOTR), ZNRU(NTOTR)
      DIMENSION XNR(NTOTR), YNR(NTOTR), ZNR(NTOTR)
      DIMENSION IGPB(M1D,4,3,NRGRD), IRFAM(NRGRD)
C
C
C   Average normals for axis points (J-direction only).
C
      IS1 = NGP1(N)
C
      IF (JSAXSA.EQ.1) THEN
         J      = 1
         JJ     = J+1
         CALL AVGNOR(JRMAX(N),KRMAX(N),J,JJ,
     &               XNRU(IS1),YNRU(IS1),ZNRU(IS1))
         CALL AVGNOR(JRMAX(N),KRMAX(N),J,JJ,
     &               XNR(IS1),YNR(IS1),ZNR(IS1))
      ENDIF
      IF (JSAXSB.EQ.1) THEN
         J      = JRMAX(N)
         JJ     = J-1
         CALL AVGNOR(JRMAX(N),KRMAX(N),J,JJ,
     &               XNRU(IS1),YNRU(IS1),ZNRU(IS1))
         CALL AVGNOR(JRMAX(N),KRMAX(N),J,JJ,
     &               XNR(IS1),YNR(IS1),ZNR(IS1))
      ENDIF
C
C   IGPB values are set up for grid cell faces, not grid points.  For
C   example there are KRMAX-1 values supplied for the J=1 edge.  For the
C   KRMAX point, therefore, we will use the IGPB value in index KRMAX-1.
C   In addition, for edges that only partially meet up with another
C   grid, a nonzero value of IGPB at some index K followed by a zero
C   value at K+1 means that we should still average the normal at K+1.
C
C   Do J boundaries.
C
      IF (JSPER.EQ.0) THEN
         J      = 1
         NNPREV = 0
         DO 10 K = 1,KRMAX(N)
            KK     = MIN(K,KRMAX(N)-1)
            NN     = IGPB(KK,1,1,N)
            IF (NN.EQ.0 .AND. K.GT.1 .AND. NNPREV.NE.0) THEN
               KK     = KK-1
               NN     = NNPREV
            ENDIF
            IF ( (NN.GT.0) .AND. (IRFAM(N).EQ.IRFAM(NN)) ) THEN
             ISNN = NGP1(NN)
             CALL SMONOR(JRMAX(NN),KRMAX(NN),JRMAX(N),KRMAX(N),
     &                   IGPB(KK,1,2,N),IGPB(KK,1,3,N),J,K,
     &                   XNRU(ISNN),YNRU(ISNN),ZNRU(ISNN),
     &                   XNRU(IS1),YNRU(IS1),ZNRU(IS1),
     &                   XNR(IS1),YNR(IS1),ZNR(IS1))
            ENDIF
            NNPREV = NN
   10       CONTINUE
         J      = JRMAX(N)
         NNPREV = 0
         DO 20 K = 1,KRMAX(N)
            KK     = MIN(K,KRMAX(N)-1)
            NN     = IGPB(KK,2,1,N)
            IF (NN.EQ.0 .AND. K.GT.1 .AND. NNPREV.NE.0) THEN
               KK     = KK-1
               NN     = NNPREV
            ENDIF
            IF ( (NN.GT.0) .AND. (IRFAM(N).EQ.IRFAM(NN)) ) THEN
             ISNN = NGP1(NN)
             CALL SMONOR(JRMAX(NN),KRMAX(NN),JRMAX(N),KRMAX(N),
     &                   IGPB(KK,2,2,N),IGPB(KK,2,3,N),J,K,
     &                   XNRU(ISNN),YNRU(ISNN),ZNRU(ISNN),
     &                   XNRU(IS1),YNRU(IS1),ZNRU(IS1),
     &                   XNR(IS1),YNR(IS1),ZNR(IS1))
            ENDIF
            NNPREV = NN
   20       CONTINUE
      ENDIF
C
C   Do K boundaries.
C
      IF (KSPER.EQ.0) THEN
         K      = 1
         NNPREV = 0
         DO 30 J = 1,JRMAX(N)
            JJ     = MIN(J,JRMAX(N)-1)
            NN     = IGPB(JJ,3,1,N)
            IF (NN.EQ.0 .AND. J.GT.1 .AND. NNPREV.NE.0) THEN
               JJ     = JJ-1
               NN     = NNPREV
            ENDIF
            IF ( (NN.GT.0) .AND. (IRFAM(N).EQ.IRFAM(NN)) ) THEN
             ISNN = NGP1(NN)
             CALL SMONOR(JRMAX(NN),KRMAX(NN),JRMAX(N),KRMAX(N),
     &                   IGPB(JJ,3,2,N),IGPB(JJ,3,3,N),J,K,
     &                   XNRU(ISNN),YNRU(ISNN),ZNRU(ISNN),
     &                   XNRU(IS1),YNRU(IS1),ZNRU(IS1),
     &                   XNR(IS1),YNR(IS1),ZNR(IS1))
            ENDIF
            NNPREV = NN
   30       CONTINUE
         K      = KRMAX(N)
         NNPREV = 0
         DO 40 J = 1,JRMAX(N)
            JJ     = MIN(J,JRMAX(N)-1)
            NN     = IGPB(JJ,4,1,N)
            IF (NN.EQ.0 .AND. J.GT.1 .AND. NNPREV.NE.0) THEN
               JJ     = JJ-1
               NN     = NNPREV
            ENDIF
            IF ( (NN.GT.0) .AND. (IRFAM(N).EQ.IRFAM(NN)) ) THEN
             ISNN = NGP1(NN)
             CALL SMONOR(JRMAX(NN),KRMAX(NN),JRMAX(N),KRMAX(N),
     &                   IGPB(JJ,4,2,N),IGPB(JJ,4,3,N),J,K,
     &                   XNRU(ISNN),YNRU(ISNN),ZNRU(ISNN),
     &                   XNRU(IS1),YNRU(IS1),ZNRU(IS1),
     &                   XNR(IS1),YNR(IS1),ZNR(IS1))
            ENDIF
            NNPREV = NN
   40       CONTINUE
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE SWAPR (NJ,NK,X,Y,Z,XTMP,YTMP,ZTMP)
C
C   Swap J and K, and reverse K for this surface grid.
C   Swap NJ and NK too.
C
#include "precis.h"
C
      DIMENSION X(NJ*NK), Y(NJ*NK), Z(NJ*NK)
      DIMENSION XTMP(NJ,NK), YTMP(NJ,NK), ZTMP(NJ,NK)
C
C
C   Copy the grid into XTMP,YTMP,ZTMP.
C
      I         = 0
      DO 10 K = 1,NK
      DO 10 J = 1,NJ
         I         = I+1
         XTMP(J,K) = X(I)
         YTMP(J,K) = Y(I)
         ZTMP(J,K) = Z(I)
   10    CONTINUE
C
C   Swap J and K and reverse K.
C
      I         = 0
      DO 20 J = 1,NJ
      DO 20 K = NK,1,-1
         I         = I+1
         X(I)    = XTMP(J,K)
         Y(I)    = YTMP(J,K)
         Z(I)    = ZTMP(J,K)
   20    CONTINUE
C
C   Swap dimensions.
C
      NTMP   = NJ
      NJ     = NK
      NK     = NTMP
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE VARETA(M1D,JMAX,KMAX,JPER,NNOD,JNOD,ETAMX,DETA,DFAR,
     &                  ETAVAR,TMP1,X,Y,Z)
C
C    Check inputs for piece-wise linear variable stretching function
C
#include "precis.h"
C
      DIMENSION JNOD(NNOD), ETAMX(NNOD), DETA(NNOD), DFAR(NNOD),
     &          ETAVAR(JMAX,3), TMP1(M1D)
      DIMENSION X(JMAX,KMAX), Y(JMAX,KMAX), Z(JMAX,KMAX)
C
      CHARACTER*80 TEXT
      PARAMETER (ISF=2,RMAX=0.)
      PARAMETER (ZERO=0.)
C
C
      IOK    = 1
      DO 10 N = 1,NNOD
         IF (JNOD(N).LE.-1) JNOD(N) = JMAX+JNOD(N)+1
   10    CONTINUE
      IF (JNOD(1).NE.1) WRITE(*,*) 'JNOD(1) is reset to 1'
      IF (JNOD(NNOD).NE.JMAX)
     & WRITE(*,*) 'JNOD(',NNOD,') is reset to ',JMAX
C
      DO 20 N = 1,NNOD
         IF (JNOD(N).LE.0) THEN
            TEXT   = 'JNOD is not positive. Please correct input.'
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            IOK    = 0
         ENDIF
         IF (N.GT.1) THEN
            IF (JNOD(N).LE.JNOD(N-1)) THEN
               TEXT   = 'JNOD sequence is not increasing.'//
     &                  ' Please correct input.'
               NTEXT  = 1
               CALL ERRMSG(TEXT,NTEXT)
               IOK    = 0
            ENDIF
         ENDIF
         IF (ETAMX(N).LE.0.0) THEN
            TEXT   = 'ETAMX is not positive. Please correct input.'
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            IOK    = 0
         ENDIF
         IF (DETA(N).LT.0.0) THEN
            TEXT   = 'DETA cannot be negative. Please correct input.'
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            IOK    = 0
         ENDIF
         IF (DFAR(N).LT.0.0) THEN
            TEXT   = 'DFAR cannot be negative. Please correct input.'
            NTEXT  = 1
            CALL ERRMSG(TEXT,NTEXT)
            IOK    = 0
         ENDIF
   20    CONTINUE
C
      IF (IOK.EQ.0) THEN
         WRITE(*,*) 'Program terminated due to invalid input.'
         CALL ERREND
         STOP 'VARETA'
      ENDIF
C
C    Modify DETA and DFAR if necessary
C
      DO 30 N = 1,NNOD
C
C       Compute DETA and DFAR for uniform spacing
C
         IF      (DETA(N).EQ.0. .AND. DFAR(N).EQ.0.) THEN
            DEL     = ETAMX(N)/FLOAT(KMAX-1)
            DETA(N) = DEL
            DFAR(N) = DEL
C
C       Initial spacing fixed, far field spacing not specified
C
         ELSE IF (DETA(N).GT.0. .AND. DFAR(N).EQ.0.) THEN
            CALL SFUNS (ISF,ZERO,ETAMX(N),DETA(N),DFAR(N),
     &                  KMAX,RMAX,M1D,DX0A,DX1A,TMP1)
            DFAR(N) = DX1A
C
C       Initial spacing not specified, far field spacing fixed
C
         ELSE IF (DETA(N).EQ.0. .AND. DFAR(N).GT.0.) THEN
            CALL SFUNS (ISF,ZERO,ETAMX(N),DETA(N),DFAR(N),
     &                  KMAX,RMAX,M1D,DX0A,DX1A,TMP1)
            DETA(N) = DX0A
         ENDIF
C
   30    CONTINUE
C
C    Compute arc lengths along initial curve
C
      ARCT    = 0.
      TMP1(1) = 0.
      DO 40 J = 2,JMAX
         ARC     = SQRT( (X(J,1)-X(J-1,1))**2 + (Y(J,1)-Y(J-1,1))**2
     &                                        + (Z(J,1)-Z(J-1,1))**2 )
         ARCT    = ARCT + ARC
         TMP1(J) = TMP1(J-1) + ARC
   40    CONTINUE
C
C    Construct piece-wise linear functions based on arc length along
C    initial curve and store in ETAVAR
C
      DO 60 N = 2,NNOD
         RINT        = TMP1(JNOD(N)) - TMP1(JNOD(N-1))
         DO 50 J = JNOD(N-1),JNOD(N)
            EPS         = (TMP1(J) - TMP1(JNOD(N-1)))/RINT
            ETAVAR(J,1) = (1.-EPS)*ETAMX(N-1) + EPS*ETAMX(N)
            ETAVAR(J,2) = (1.-EPS)*DETA(N-1)  + EPS*DETA(N)
            ETAVAR(J,3) = (1.-EPS)*DFAR(N-1)  + EPS*DFAR(N)
   50       CONTINUE
   60    CONTINUE
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE WRGRID(IAUC,NAGRD,NAGRDN,NTOT,NGP,NGN,JMAX,KMAX,
     &                  JPER,ICON,NGO,ISGRI,KTMP,X,Y,Z)
C
C   Locate consecutive grids with common initial curve before writing
C   out.
C
#include "precis.h"
C
      DIMENSION NGP(NAGRD), NGN(NAGRD), JMAX(NAGRD), KMAX(NAGRD),
     &          ICON(NAGRD), NGO(NAGRD), KTMP(NAGRD)
      DIMENSION X(NTOT), Y(NTOT), Z(NTOT)
C
      PARAMETER (I1=1)
C
C
C   Look for common initial lines.
C
      NAGRDN    = NAGRD
      IF (IAUC.EQ.1) THEN
         DO 10 N = 1,NAGRD
            KTMP(N)   = KMAX(N)
   10       CONTINUE
C
         ICON(1)   = 0
         DO 30 N = 2,NAGRD
            ICON(N)   = 0
            IF (JMAX(N).EQ.JMAX(N-1)) THEN
               ISM       = NGP(N-1)
               IS        = NGP(N)
               DC        = 0.
               DO 20 J=1,JMAX(N)
                  JJ        = IS + JMAX(N) - J
                  JM        = ISM + J - 1
                  DC        = DC + (X(JJ) - X(JM))**2
     &                           + (Y(JJ) - Y(JM))**2
     &                           + (Z(JJ) - Z(JM))**2
   20             CONTINUE
C
               IF (DC.EQ.0.) THEN
                  ICON(N)   = 1
                  NAGRDN    = NAGRDN-1
                  KTMP(N-1) = KMAX(N-1)+KMAX(N)-1
               ENDIF
            ENDIF
   30       CONTINUE
      ENDIF
C
C   No grids are to be concatenated.  Write out all grids.
C
      IF (NAGRD.EQ.NAGRDN) THEN
C
C         IF (ISGRI.EQ.0) WRITE(3) NAGRD
         IF (NAGRD.GT.1) WRITE(3) NAGRD
         WRITE(3) (JMAX(N),KMAX(N),I1,N=1,NAGRD)
         DO 40 N = 1,NAGRD
            IS = NGP(N)
            IE = IS + NGN(N) - 1
            WRITE(3) (X(I),I=IS,IE),(Y(I),I=IS,IE),(Z(I),I=IS,IE)
   40       CONTINUE
C
C   Some grids will be concatenated.
C
      ELSE
C
         NN        = 0
         DO 50 N = 1,NAGRD
            IF (ICON(N).EQ.0) THEN
               NN        = NN+1
               NGO(NN)   = N
            ENDIF
   50       CONTINUE
C
C   Write in single or multiple grid format depending on number of
C   output grids.
C
C         IF (ISGRI.EQ.0) WRITE(3) NAGRDN
         IF (NAGRDN.GT.1) WRITE(3) NAGRDN
         WRITE(3) (JMAX(NGO(N)),KTMP(NGO(N)),I1,N=1,NAGRDN)
         DO 60 N = 1,NAGRD-1
            IF (ICON(N).EQ.0) THEN
               IF      (ICON(N+1).EQ.0) THEN
                  IS = NGP(N)
                  IE = IS + NGN(N) - 1
                  WRITE(3) (X(I),I=IS,IE),(Y(I),I=IS,IE),(Z(I),I=IS,IE)
               ELSE IF (ICON(N+1).EQ.1) THEN
                  IS  = NGP(N)
                  IE  = IS + NGN(N) - 1
                  ISN = NGP(N+1) + JMAX(N+1)
                  IEN = NGP(N+1) + NGN(N+1) - 1
                  WRITE(3) (X(I),I=IEN,ISN,-1),(X(I),I=IS,IE),
     &                     (Y(I),I=IEN,ISN,-1),(Y(I),I=IS,IE),
     &                     (Z(I),I=IEN,ISN,-1),(Z(I),I=IS,IE)
                  WRITE(*,51) N,N+1
   51             FORMAT(' Grid',I5,' and grid',I5,
     &                   ' have been concatenated.')
               ENDIF
            ENDIF
   60       CONTINUE
         N         = NAGRD
         IF (ICON(N).EQ.0) THEN
            IS = NGP(N)
            IE = IS + NGN(N) - 1
            WRITE(3) (X(I),I=IS,IE),(Y(I),I=IS,IE),(Z(I),I=IS,IE)
         ENDIF
C
         WRITE(*,61) NAGRD
   61    FORMAT(1X,'Total number of active grids   = ',I5)
         WRITE(*,62) NAGRDN
   62    FORMAT(1X,'Total number of output grids   = ',I5)
C
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE WRINOR(NAGRD,N,JMAX,KMAX,NJ,NK,EV,GNV,FILEV,FILGNV)
C
C   Write fake Q files of (1) active grid eta metrics and (2) active
C   grid (interpolated) surface normals.
C
C   Files are always in multiple grid format.
C
#include "precis.h"
C
      CHARACTER*(*) FILEV,FILGNV
      DIMENSION JMAX(NAGRD),KMAX(NAGRD)
      DIMENSION EV(NJ,NK,3), GNV(NJ,NK,3)
C
      PARAMETER (I1=1)
      PARAMETER (ZERO=0.,ONE=1.)
C
C
C   First time through open the files.
C
      IF (N.EQ.1) THEN
         OPEN(UNIT=11,FILE=FILEV,STATUS='UNKNOWN',FORM='UNFORMATTED')
         OPEN(UNIT=21,FILE=FILGNV,STATUS='UNKNOWN',FORM='UNFORMATTED')
         WRITE(11) NAGRD
         WRITE(11) (JMAX(I),KMAX(I),I1,I=1,NAGRD)
         WRITE(21) NAGRD
         WRITE(21) (JMAX(I),KMAX(I),I1,I=1,NAGRD)
      ENDIF
C
C   Fill in metrics for the last line of points.
C
      DO 10 J = 1,NJ
         EV(J,NK,1) = EV(J,NK-1,1)
         EV(J,NK,2) = EV(J,NK-1,2)
         EV(J,NK,3) = EV(J,NK-1,3)
   10    CONTINUE
C
C   Write the data for this grid.  Include the fake Q header.
C
      WRITE(11) ONE,ZERO,ZERO,ZERO
      WRITE(11) ((ONE,J=1,NJ),K=1,NK),
     &          (((EV(J,K,NX),J=1,NJ),K=1,NK),NX=1,3),
     &          ((ONE,J=1,NJ),K=1,NK)
      WRITE(21) ONE,ZERO,ZERO,ZERO
      WRITE(21) ((ONE,J=1,NJ),K=1,NK),
     &          (((GNV(J,K,NX),J=1,NJ),K=1,NK),NX=1,3),
     &          ((ONE,J=1,NJ),K=1,NK)
C
C   Last time through close the files.
C
      IF (N.EQ.NAGRD) THEN
         CLOSE(11)
         CLOSE(21)
      ENDIF
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE WRIREF(NG,NTOTR,NGP,NGN,NJ,NK,NL,FILE,X,Y,Z)
C
C   Write the two-layer version of the reference grids for debugging
C   purposes.
C
C   File is always in multiple grid format.
C
#include "precis.h"
C
      CHARACTER*(*) FILE
      DIMENSION NGP(NG), NGN(NG), NJ(NG) ,NK(NG)
      DIMENSION X(NTOTR*2), Y(NTOTR*2), Z(NTOTR*2)
C
C
      OPEN(UNIT=11,FILE=FILE,STATUS='UNKNOWN',FORM='UNFORMATTED')
      WRITE(*,*) 'Writing ',FILE
      WRITE(*,*) NG,' grids, dimensions'
      WRITE(11) NG
      WRITE(11) (NJ(N),NK(N),NL,N=1,NG)
      DO 10 N = 1,NG
         IS = NGP(N)
         IE = IS + 2*NGN(N) - 1
         WRITE(*,*) NJ(N),NK(N),NL
         WRITE(11) (X(I),I=IS,IE),(Y(I),I=IS,IE),(Z(I),I=IS,IE)
   10    CONTINUE
      CLOSE(11)
C
C
      RETURN
      END
C***********************************************************************
      SUBROUTINE WRPCOM(REFSFN,REFCFN,OUTGFN,NRGRD,NRCUR,NAGRD,NAGRDN,
     &                  NTOT,NGP,JMAX,KMAX,ISGRI,ISGRB,IRSTYP,X,Y,Z)
C
#include "precis.h"
C
C   Write out command files for viewing surface grids:
C
C     refs.com      - reference surface    (refsfn)
C     refc.com      - reference curves     (refcfn)
C     surg.com      - output surface grids (outgfn)
C     minmax.com    - set minmax
C     nowalls.com   - reset to no walls
C
      CHARACTER*80 REFSFN, REFCFN, OUTGFN, REFSCF, REFCCF, OUTGCF, MMCF,
     &             NWCF
      DIMENSION NGP(NAGRD), JMAX(NAGRD), KMAX(NAGRD)
      DIMENSION X(NTOT), Y(NTOT), Z(NTOT)
C
      PARAMETER (MCOLOR=50)
      CHARACTER*15 COLOR(MCOLOR)
      PARAMETER (I1=1)
C
C   First set up color sequence.
C
      NCOLOR    = 20
      COLOR( 1) = 'blue        '
      COLOR( 2) = 'green       '
      COLOR( 3) = 'red         '
      COLOR( 4) = 'cyan        '
      COLOR( 5) = 'mag         '
      COLOR( 6) = 'rgb 1. .5 .5'
      COLOR( 7) = 'rgb .5 1. .5'
      COLOR( 8) = 'rgb .5 .5 1.'
      COLOR( 9) = 'rgb 1. .5 0.'
      COLOR(10) = 'rgb 1. 0. .5'
      COLOR(11) = 'rgb 0. .5 1.'
      COLOR(12) = 'rgb 0. 1. .5'
      COLOR(13) = 'rgb .5 1. 0.'
      COLOR(14) = 'rgb .5 0. 1.'
      COLOR(15) = 'rgb .5 0. 0.'
      COLOR(16) = 'rgb 0. .5 0.'
      COLOR(17) = 'rgb 0. 0. .5'
      COLOR(18) = 'rgb 0. .5 .5'
      COLOR(19) = 'rgb .5 0. .5'
      COLOR(20) = 'rgb .5 .5 0.'
C
C   Set command filenames.
C
      REFSCF = 'refs.com'
      REFCCF = 'refc.com'
      OUTGCF = 'surg.com'
      MMCF   = 'minmax.com'
      NWCF   = 'nowalls.com'
C
C   Write command file for reference surface grids (panel networks).
C
      IF (IRSTYP.EQ.1) THEN
C
       OPEN (UNIT=9,FILE=REFSCF,STATUS='UNKNOWN',FORM='FORMATTED')
        WRITE(9,110) MMCF
        IF (ISGRB.EQ.1) THEN
           WRITE(9,103) REFSFN
        ELSE
           WRITE(9,105) REFSFN
        ENDIF
        DO 10 N = 1,NRGRD
           WRITE(9,112) N,'black'
 10     CONTINUE
        WRITE(9,191)
       CLOSE(9)
C
      ENDIF
C
C   Write command file for reference curves.
C
      OPEN (UNIT=9,FILE=REFCCF,STATUS='UNKNOWN',FORM='FORMATTED')
       WRITE(9,110) MMCF
       IF (ISGRI.EQ.1) THEN
          WRITE(9,103) REFCFN
       ELSE
          WRITE(9,105) REFCFN
       ENDIF
       DO 20 N = 1,NRCUR
          NN = MOD(N,NCOLOR)
          IF (NN.EQ.0) NN = NCOLOR
          WRITE(9,114) N,COLOR(NN),4
 20    CONTINUE
       WRITE(9,191)
      CLOSE(9)
C
C   Write command file for surface grids.
C
      OPEN (UNIT=9,FILE=OUTGCF,STATUS='UNKNOWN',FORM='FORMATTED')
       WRITE(9,110) MMCF
       IF (NAGRDN.EQ.1) THEN
          WRITE(9,103) OUTGFN
       ELSE
          WRITE(9,105) OUTGFN
       ENDIF
       DO 30 N = 1,NAGRDN
          NN = MOD(N,NCOLOR)
          IF (NN.EQ.0) NN = NCOLOR
          WRITE(9,112) N,COLOR(NN)
 30    CONTINUE
       WRITE(9,191)
      CLOSE(9)
C
C   Write command file for nowalls.
C
      NWALLS = MAX(NRGRD,NRCUR,NAGRD)
      OPEN (UNIT=9,FILE=NWCF,STATUS='UNKNOWN',FORM='FORMATTED')
       DO 40 N = 1,NWALLS
          WRITE(9,111) N
 40    CONTINUE
      CLOSE(9)
C
C   Write minmax box to minmax.com.
C
      OPEN (UNIT=9,FILE='minmax.com',STATUS='UNKNOWN',FORM='FORMATTED')
       DO 50 N = 1,NAGRD
          IS = NGP(N)
          CALL SUBMMS(JMAX(N),KMAX(N),I1,X(IS),Y(IS),Z(IS),
     &                XMN,XMX,YMN,YMX,ZMN,ZMX)
          IF (N.EQ.1) THEN
             XMIN = XMN
             XMAX = XMX
             YMIN = YMN
             YMAX = YMX
             ZMIN = ZMN
             ZMAX = ZMX
          ELSE
             XMIN = MIN(XMIN,XMN)
             XMAX = MAX(XMAX,XMX)
             YMIN = MIN(YMIN,YMN)
             YMAX = MAX(YMAX,YMX)
             ZMIN = MIN(ZMIN,ZMN)
             ZMAX = MAX(ZMAX,ZMX)
          ENDIF
 50    CONTINUE
       DIAG = SQRT( (XMAX-XMIN)**2 + (YMAX-YMIN)**2 + (ZMAX-ZMIN)**2 )
       DIST = 2.5*DIAG
       WRITE(9,101) XMIN,XMAX,YMIN,YMAX,ZMIN,ZMAX
       WRITE(9,102) DIST
      CLOSE(9)
C
 101  FORMAT('min ',6E11.3)
 102  FORMAT('vp/a 150 20 ',E11.3)
 103  FORMAT('re/u/x',/,A,/,'fu 0')
 105  FORMAT('re/u/mg/x',/,A/,'fu 0')
 110  FORMAT('@',A)
 111  FORMAT('w/g=',I5,//)
 112  FORMAT('w/add/g=',I5,/,'a',//,'a',//,'1',///,A,//////)
 114  FORMAT('w/add/g=',I5,/,'a',//,'1',//,'1',///,A,///,I2,///)
 191  FORMAT('p/noax/noti/noad/noba/back=white/o')
C
      RETURN
      END
