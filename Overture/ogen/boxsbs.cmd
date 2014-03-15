*
* box side-by-side with a box
*
$interpolation="explicit";
* $interpolation="implicit";
*
$factor=1; $gridName="boxsbs1.hdf";  $interpolation="implicit";
* $factor=1; $gridName="boxsbse.hdf";  $interpolation="explicit";
*
$ds=.25/$factor;
*
create mappings
  Box
    set corners
      $xa=-1.; $xb=.5*$ds; 
*       $ya=-$ds; $yb=$ds; $za=-$ds; $zb=$ds; 
*      $ya=-2.*$ds; $yb=2.*$ds; $za=-$ds; $zb=$ds; 
      $ya=-1.; $yb=1.; $za=-1.; $zb=1.; 
      $xa $xb $ya $yb $za $zb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
     $nx $ny $nz
    * periodicity
    *  0 0 1
    boundary conditions
      1 0 1 1 1 1 
    share 
      0 0 1 2 3 4 
    mappingName
      left-box
  exit
  Box
    $ds=$ds*2./3.;
    set corners
      $xa=-.5*$ds; $xb=1.; 
      $xa $xb $ya $yb $za $zb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
*      $ny = int( ($yb-$ya)/$ds + 1.5 );
     $nz = int( ($zb-$za)/$ds + 1.5 );
     $nx $ny $nz
    mappingName
      right-box
    boundary conditions
      0 1 1 1 1 1 
    share 
      0 0 1 2 3 4 
  exit
exit
*
generate an overlapping grid
  left-box
  right-box
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    interpolation type
      $interpType = "$interpolation for all grids";
      $interpType
  exit
*  pause
  compute overlap
exit
save an overlapping grid
$gridName
boxsbs
exit



*
* overlapping grid for two boxes side-by-side
*
create mappings
Box
specify corners
0 0 0 .65 1. 1.
lines
11 6 6 
boundary conditions
1 0 1 1 1 
y+r
y-r
y-r
x+r
mappingName
left-box
exit
*
Box
specify corners
.35 0 0 1 1 1
lines
11 6 6
boundary conditions
0 1 1 1 1 
mappingName
right-box
exit
exit
*
make an overlapping grid
2
left-box
right-box
PLOT
EXIT
save an overlapping grid
boxsbs.hdf
boxes
exit
