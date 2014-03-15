*
* box in a box
*
$interpolation="explicit";
* $interpolation="implicit";
*
$factor=1; $gridName="bib.hdf"; 
* $factor=2; $gridName="bib2.hdf"; 
*
$ds=.1/$factor;
*
create mappings
  Box
    set corners
      $xa=-1.; $xb=1.; $ya=$xa; $yb=$xb; $za=$xa; $zb=$xb; 
      $xa $xb $ya $yb $za $zb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
     $nx $ny $nz
    * periodicity
    *  0 0 1
    mappingName
      outer-box
  exit
  Box
    set corners
      $xa=-.5; $xb=.5; $ya=$xa; $yb=$xb; $za=$xa; $zb=$xb; 
      $xa $xb $ya $yb $za $zb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
     $nx $ny $nz
    mappingName
      inner-box
    boundary conditions
      0 0 0 0 0 0
  exit
exit
*
generate an overlapping grid
  outer-box
  inner-box
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
bib
exit

