// ============================================================================
//    Routines to access the TZ functions from fortran or c 
// ============================================================================
#include "OGFunction.h"

#define OGF EXTERN_C_NAME(ogf)
#define OGDERIV EXTERN_C_NAME(ogderiv)
#define INSBFU2D EXTERN_C_NAME(insbfu2d)
#define INSBFV2D EXTERN_C_NAME(insbfv2d)
#define INSBFU3D EXTERN_C_NAME(insbfu3d)
#define INSBFV3D EXTERN_C_NAME(insbfv3d)
#define INSBFW3D EXTERN_C_NAME(insbfw3d)
#define EXX EXTERN_C_NAME(exx)


extern "C"
{

/* Here are functions for TZ flow that can be called from fortran */
/*  TZ boundary forcing for the INS equations -- NO time derivative --- */

real
OGF(OGFunction *&e, const real &x, const real &y,const real &z, const int & c, const real & t )
{
  return (*e)(x,y,z,c,t);
}


/* return a general derivative */
void
OGDERIV(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
         const real &x, const real &y, const real &z, const real & t, const int & n, real & ud )
{
  ud=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n,t);
}


real
INSBFU2D(OGFunction *&ep, const real &x, const real &y,const real &z, const real & t,
       const real & nu, const int & pc, const int & uc, const int & vc )
{
  OGFunction & e = *ep;
  return  -nu*(e.xx(x,y,z,uc,t)+e.yy(x,y,z,uc,t)) 
    /* +(e(x,y,z,uc,t)*e.x(x,y,z,uc,t)+e(x,y,z,vc,t)*e.y(x,y,z,uc,t))  */
          +e.x(x,y,z,pc,t) /*  +e.t(x,y,z,uc,t) */;
}
real
INSBFV2D(OGFunction *&ep, const real &x, const real &y,const real &z, const real & t,
       const real & nu, const int & pc, const int & uc, const int & vc )
{
  OGFunction & e = *ep;
  return  -nu*(e.xx(x,y,z,vc,t)+e.yy(x,y,z,vc,t)) 
          /* +(e(x,y,z,uc,t)*e.x(x,y,z,vc,t)+e(x,y,z,vc,t)*e.y(x,y,z,vc,t))  */
          +e.y(x,y,z,pc,t) /*  +e.t(x,y,z,vc,t) */;

}

real
INSBFU3D(OGFunction *&ep, const real &x, const real &y,const real &z, const real & t,
       const real & nu, const int & pc, const int & uc, const int & vc, const int & wc )
{
  OGFunction & e = *ep;
  return /* e.t(x,y,z,uc,t) */
   /* +    (e(x,y,z,uc,t)*e.x(x,y,z,uc,t) 
        +e(x,y,z,vc,t)*e.y(x,y,z,uc,t) 
        +e(x,y,z,wc,t)*e.z(x,y,z,uc,t)) */
   + e.x(x,y,z,pc,t)  
   - nu*( e.xx(x,y,z,uc,t)+e.yy(x,y,z,uc,t)+e.zz(x,y,z,uc,t) ) ;
}
real
INSBFV3D(OGFunction *&ep, const real &x, const real &y,const real &z, const real & t,
       const real & nu, const int & pc, const int & uc, const int & vc, const int & wc )
{
  OGFunction & e = *ep;
  return /* e.t(x,y,z,vc,t) */ 
   /* +    (e(x,y,z,uc,t)*e.x(x,y,z,vc,t)  
        +e(x,y,z,vc,t)*e.y(x,y,z,vc,t)  
        +e(x,y,z,wc,t)*e.z(x,y,z,vc,t))  */
   + e.y(x,y,z,pc,t)  
    - nu*( e.xx(x,y,z,vc,t)+e.yy(x,y,z,vc,t)+e.zz(x,y,z,vc,t) ) ;
  

}
real
INSBFW3D(OGFunction *&ep, const real &x, const real &y,const real &z, const real & t,
       const real & nu, const int & pc, const int & uc, const int & vc, const int & wc )
{
  OGFunction & e = *ep;
  return /* e.t(x,y,z,wc,t) */
   /* +    (e(x,y,z,uc,t)*e.x(x,y,z,wc,t)  
        +e(x,y,z,vc,t)*e.y(x,y,z,wc,t)  
        +e(x,y,z,wc,t)*e.z(x,y,z,wc,t))  */
   + e.z(x,y,z,pc,t) 
    - nu*( e.xx(x,y,z,wc,t)+e.yy(x,y,z,wc,t)+e.zz(x,y,z,wc,t) ) ;
  

}


real
EXX(OGFunction *&e, const real &x, const real &y,const real &z, const int & c, const real & t )
{
  real value=(*e).xx(x,y,z,c,t);
  printf("exx: x=(%8.2e,%8.2e,%8.2e) c=%i t=%8.2e ...exx=%8.2e \n",x,y,z,c,t,value);
  return value;
}

}

