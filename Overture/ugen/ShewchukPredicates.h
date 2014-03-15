#ifndef __SHEW_PREDICATES__
#define __SHEW_PREDICATES__

// interface to Jonathan Shewchuk's robust predicates, compiled in TriangleSource.c

extern "C" {
/*    orient2d(pa, pb, pc)                                                   */
extern double orient2d(double *, double*, double*);

/*    orient2dfast(pa, pb, pc)                                               */
extern double orient2dfast(double *, double*, double*);

/*    orient3d(pa, pb, pc, pd)                                               */
extern double orient3d(double *, double *, double *, double *);

/*    orient3dfast(pa, pb, pc, pd)                                           */
extern double orient3d(double *, double *, double *, double *);

/*    incircle(pa, pb, pc, pd)                                               */
extern double incircle(double *, double *, double *, double *);

/*    incirclefast(pa, pb, pc, pd)                                           */
extern double iancirclefast(double *, double *, double *,double *);

/*    insphere(pa, pb, pc, pd, pe)                                           */
extern double insphere(double *, double *, double *, double *, double *);

/*    inspherefast(pa, pb, pc, pd, pe)                                       */
extern double inspherefast(double *, double *, double *, double *, double *);
}

// single precision c++ interface for overture

inline double orient2d(float *pa_, float *pb_, float *pc_)
{
  double pa[2],pb[2],pc[2];
  for ( int i=0; i<2; i++ )
    {
      pa[i] = pa_[i];
      pb[i] = pb_[i];
      pc[i] = pc_[i];
    }
  return orient2d(&pa[0],&pb[0],&pc[0]);
}

/*    orient3d(pa, pb, pc, pd)                                               */
inline double orient3d(float *pa_, float *pb_, float *pc_, float *pd_)
{
  double pa[3],pb[3],pc[3],pd[3];
  for ( int i=0; i<3; i++ )
    {
      pa[i] = pa_[i];
      pb[i] = pb_[i];
      pc[i] = pc_[i];
      pd[i] = pd_[i];
    }
  return orient3d(&pa[0],&pb[0],&pc[0],&pd[0]);
}

/*    incircle(pa, pb, pc, pd)                                               */
inline double incircle(float *pa_, float *pb_, float *pc_, float *pd_)
{
  double pa[2],pb[2],pc[2],pd[2];
  for ( int i=0; i<2; i++ )
    {
      pa[i] = pa_[i];
      pb[i] = pb_[i];
      pc[i] = pc_[i];
      pd[i] = pd_[i];
    }
  return incircle(&pa[0],&pb[0],&pc[0],&pd[0]);
}

/*    insphere(pa, pb, pc, pd, pe)                                           */
inline double insphere(float *pa_, float *pb_, float *pc_, float *pd_, float *pe_)
{
  double pa[3],pb[3],pc[3],pd[3],pe[3];
  for ( int i=0; i<3; i++ )
    {
      pa[i] = pa_[i];
      pb[i] = pb_[i];
      pc[i] = pc_[i];
      pd[i] = pd_[i];
      pe[i] = pe_[i];
    }
  return insphere(&pa[0],&pb[0],&pc[0],&pd[0],&pe[0]);
}

#endif
