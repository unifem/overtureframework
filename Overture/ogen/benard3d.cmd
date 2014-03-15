*
* Make grids for the Rayleigh Benard problem in 3D
* 
***************************************************************************
$orderOfAccuracy = "second order";
    $xa=0.;  $xb=4.0; 
    $ya=0.;  $yb=1.0; 
    $za=0.;  $zb=4.0;
*
* scale number of grid points in each direction by $factor
* $factor=1; $name = "benard3d1.hdf";
$factor=2; $name = "benard3d2.hdf"; 
* $factor=4; $name = "benard3d4.hdf"; 
* $factor=6; $name = "benard3d6.hdf"; 
* $factor=8; $name = "benard3d8.hdf"; 
* $factor=16; $name = "benard3d16.hdf"; 
*
* -- fourth-order accurate ---
* $factor=.5; $name = "benard3d0.order4.hdf";  $orderOfAccuracy = "fourth order"; $bc = "1 2 1 1";
* $factor=1; $name = "benard3d1.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=2; $name = "benard3d2.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=4; $name = "benard3d4.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=8; $name = "benard3d8.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=16; $name = "benard3d16.order4.hdf";  $orderOfAccuracy = "fourth order";
*
*
****************************************************************************
$ds = .1/$factor; 
*
create mappings 
*
create mappings
  Box
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nz=int( ($zb-$za)/$ds+1.5 );
      $nx $ny $nz
    mappingName
      backGround
  exit
exit  
*
generate an overlapping grid
  backGround
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
  $name.hdf
  $name
exit