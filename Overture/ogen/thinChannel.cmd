* make a rectangle
*
$xa=0.; $xb=1.; 
* $name = "thinChannel.hdf"; $xb=1.; $ds=1./50.;
* $name = "thinChannel2.hdf"; $xb=2.; $ds=1./50.;
*$name = "thinChannel4.hdf"; $xb=4.; $ds=1./50.;
* $name = "thinChannel8.hdf"; $xb=8.; $ds=1./50.;
$name = "thinChannel16.hdf"; $xb=8.; $ds=1./100.;
*
create mappings
  rectangle
    mappingName
      channel
    specify corners
      $xa 0. $xb 1.
    lines
      $nx = int( ($xb-$xa)/$ds +1.5); $ny=3;
      $nx $ny
    boundary conditions
      1 1 1 1
  exit
exit
*
generate an overlapping grid
  channel
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
*
save an overlapping grid
  $name
  channel
exit
