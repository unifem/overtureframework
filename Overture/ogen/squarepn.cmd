* square pn:  not-periodic along axis1, periodic along axis2
*
$xa=-1.; $xb=1.; $ya=-1.; $yb=1.; 
$ds=1./32;
$ghost="2 2 2 2 2 2";
$order = "fourth order";
* $gridName= "square64pn.order4.hdf"; $ds=1./32.; 
$gridName= "square128pnx2y4.order4.hdf"; $ya=-2.; $yb=2.; $ds=1./64; 
# $gridName= "square8pn.hdf"; $ds=1./8.; $order="second order"; $xa=0.; $ya=0.;
* $gridName= "square10pn.hdf"; $ds=.1;  $order="second order"; $xa=0.; $ya=0.; 
* $gridName= "square20pn.hdf"; $ds=.05; $order="second order"; $xa=0.; $ya=0.; 
* $gridName= "square40pn.hdf"; $ds=.025 $order="second order"; $xa=0.; $ya=0.; 
*
create mappings
  rectangle
    mappingName
      square
    set corners
       $nx=int( ($xb-$xa)/$ds + 1.5 );
       $ny=int( ($yb-$ya)/$ds + 1.5 );
       $xa $xb $ya $yb 
    lines
      $nx $ny
    boundary conditions
       -1 -1 1 1 
  exit
exit
*
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
      $ghost
    order of accuracy
     $order
  exit
  compute overlap
exit
*
save an overlapping grid
  $gridName
  squarepn
exit
