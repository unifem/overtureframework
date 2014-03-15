*
*
* cic2.stretched.order4
* cic4.stretched
* cic2.stretched
* oneDrop-slider256.hdf
* twoCircles
* 4.4M :
* cic.bbmg7.hdf
cic.bbmg6.hdf
* 2.1M:
* cic.bbmg6a
* cilc3.hdf
* cic.bbmg5.hdf
* cic.bbmg4.hdf
* cic.bbmg3.hdf
* cic.bbmg2.hdf
* cic.bbmg.hdf
* valvee.hdf
* sismg.hdf
* cic.bp.hdf
* cic.hdf
* cic.bbmg1.hdf
* this next one is for the "mask" figures in the paper
* cic.bbmg0.hdf
* cicSplit
********************
* pause
* turn on polynomial
*
*
* divScalarGrad (predefined)
laplace (predefined)
* heat equation (predefined)
*
* turn off twilight zone
turn on trigonometric
* dirichlet=1 neumann=2 mixed=3
* bc(0,0,0)=2
* bc(1,0,0)=3
* bc(0,1,0)=2
* bc(1,1,0)=2
* bc(0,1,1)=2
*
*****************************************************
change parameters
*
do not use error estimate in convergence test
*
** use new auto sub-smooth
**
 use full multigrid
*
*****
** now the default: use new fine to coarse BC
***********************************************
 number of boundary layers to smooth
   0 3  1
 number of boundary smooth iterations
   5 3 1 3  1 3 1 9 7 6 5 3 2 3 1 5 3
 number of levels for boundary smooths
   1 5 2 1
*************************
 number of interpolation layers to smooth
    4 3 2 2 1 2 1 3 0 2 1 3 2 1
 number of interpolation smooth iterations
   2 2 2 3 5 2 1 3 1 3  1 3 1 9 7 6 5 3 2 3 1 5 3
 number of levels for interpolation smooths
   1 5 2 1
**************************
*
* do not use locally optimal omega
*  omega=1. is better for the finer grids (?)
* omega red-black
*   1.03 1.05 1.0 1.05 1.16 1.05 1.0 1.07 1.0 1.05 1.05 1.1 1.09  1.1 1.07
* omega line-zebra
*   1.2 1.1
***********
* do not use split step line solver
***********
residual tolerance
  1.e-14 1.e-13 1.e-12
error tolerance
  1.e-10 1.e-13
maximum number of iterations
  6  9 7 9 15 8 10 15 12 8 10 9 8 10
* maximum number of levels
*        5 4 3 2
*
* do not interpolate after smoothing
*
* works better if we interpolate the defect!
* do not interpolate the defect
* show smoothing rates
* do not use automatic sub-smooth determination
*
*   do not interpolate the defect
*  jacobi
* alternating zebra
* alternating 
* ld1
line zebra direction 1
* line jacobi direction 2
smoother(0)=rb
**********
* oges smoother
*   1
* done
*oges smoother parameters
* number of incomplete LU levels
*   0
* exit
****************
* smoother(0)=jacobi
* number of cycles=2 : W cycle
*
number of cycles
  2
***********************
*use an F cycle
************************
number of smooths
 1 1 
* output a matlab file
* do not use optimized version
exit
debug
  3 15 3 15 3
exit






