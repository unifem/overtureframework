*
* Put a plug in a channel for moving grid tests
* 
*    ogen noplot plug -factor=1
*    ogen noplot plug -factor=2
*    ogen noplot plug -factor=4
*    ogen noplot plug -factor=8
*    ogen noplot plug -factor=16
*    ogen noplot plug -factor=32
*
*
* $gridName="plug1.hdf"; $factor=1.;
* $gridName="plug2.hdf"; $factor=2.;
* $gridName="plug4.hdf"; $factor=4.;
* $gridName="plug8.hdf"; $factor=8.;
* $gridName="plug16.hdf"; $factor=16.;
* $gridName="plug32.hdf"; $factor=32.;
*
$factor=1;
GetOptions( "factor=i"=>\$factor);
$name = "plug$factor.hdf"; 
* 
$ds = 1./10./$factor; 
*
create mappings
  rectangle
    set corners
    $xa=-.75; $xb=1.5; $ya=0.; $yb=1.; 
      $xa $xb $ya $yb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
     ## $ny =5; 
      $nx $ny
    boundary conditions
      0 2 3 4
    share
      0 0 2 3
    mappingName
      square
    exit
*
  rectangle
    set corners
      $xa=0.; $xb=.5; $ya=0.; $yb=1.;
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $xa $xb $ya $yb 
      * -.5 0 -.5 .5 
    lines
      $nx $ny 
    boundary conditions
     * fudge: choose bc>100 to turn off face from rigid body
     * this IS currently needed to treat the corners properly
     *  1 0 101 101 
     *wdh* 101101 -- new way specify face by share flag
     100 0 2 3 
    share
      100 0 2 3
    mappingName
      plug
    exit
  exit
*
generate an overlapping grid
  square
  plug
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      implicit for all grids
      * explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
*
exit
save an overlapping grid
$name
plug
exit

