*
* Create a grid for the SLAC geometry
*
* scale number of grid points in each direction by the following factor
$factor=1.;
* Here we get twice as many points:
* $factor=2.**(1./3.); printf(" factor=$factor\n");
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2,$n3)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); $nz=int(($n3-1)*$factor+1.5);\
}
*
* ==== define ghost points for 2nd or fourth order
$numGhost=1;
*
* --- this next include file builds the grids ----
include createSlac1.cmd
*
*
generate an overlapping grid
  mainBox
  coreBox
  slac1-volume1
  slac1-volume2
  done 
  change parameters
    * interpolation type
    *  explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  set view:0 0.525501 -0.197041 0 1.66828 0.667139 -0.00441374 -0.74492 -0.582539 0.620174 -0.525387 0.464299 0.784452 0.411171
* pause
  compute overlap
* pause
exit
*
save an overlapping grid
slac1.hdf
slac1
exit    

