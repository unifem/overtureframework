* OLD VERSION -- see benardGrid.cmd
* Make grids for the Rayleigh Benard problem
* 
***************************************************************************
$orderOfAccuracy = "second order";
    $xa=0.;  $xb=4.0; 
    $ya=0.;  $yb=1.0; 
*
* scale number of grid points in each direction by $factor
* $factor=1; $name = "benard1.hdf";
* $factor=2; $name = "benard2.hdf"; 
$factor=4; $name = "benard4.hdf"; 
* $factor=6; $name = "benard6.hdf"; 
* $factor=8; $name = "benard8.hdf"; 
* $factor=16; $name = "benard16.hdf"; 
*
* -- fourth-order accurate ---
* $factor=.5; $name = "benard0.order4.hdf";  $orderOfAccuracy = "fourth order"; $bc = "1 2 1 1";
* $factor=1; $name = "benard1.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=2; $name = "benard2.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=4; $name = "benard4.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=8; $name = "benard8.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=16; $name = "benard16.order4.hdf";  $orderOfAccuracy = "fourth order";
*
*
****************************************************************************
$ds = .1/$factor; 
*
create mappings 
*
  rectangle 
    mappingName
      backGround
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    exit 
  exit this menu 
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