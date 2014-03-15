*
* two circles in a long channel
*
$interpType="explicit for all grids";
*
$gridName="tcilc2e.order4.hdf"; $factor=2; 
* $gridName="tcilc3e.order4.hdf"; $factor=4; 
* $gridName="tcilc4e.order4.hdf"; $factor=8; 
* $gridName="tcilc5e.order4.hdf"; $factor=16; 
* $gridName="tcilc6e.order4.hdf"; $factor=32; 
* 
* 
$ds = 1./10./$factor;
$pi = 3.141592653;
*
create mappings
  rectangle
    $xa=-2.5; $xb=7.5; $ya=-2.5; $yb=2.5; 
    set corners
     $xa $xb $ya $yb 
     * -2.5  7.5  -2.5 2.5
    lines
      $nx = int( ($xb-$xa)/$ds +1.5 ); $ny = int( ($yb-$ya)/$ds +1.5 );
      $nx $ny
      * 401 201  801 401  201 101  101 51
    boundary conditions
      1 1 1 1
    mappingName
    square
    exit
*
  Annulus
    $innerRadius=.5; $deltaRadius=1.2/$factor; 
    $outerRadius=$innerRadius+$deltaRadius; 
    inner radius
      $innerRadius
    outer radius
      $outerRadius
    centre for annulus
      -.6 .6
    lines
     $nr = int( 2.*$pi*($innerRadius+$outerRadius)/2./$ds+1.5);
     $nTheta=17; 
     $nr $nTheta
     * 241 17  301 17   337 33   673 33  169 17  169 33  85 17
    boundary conditions
      -1 -1 1 0
    mappingName
      unstretched-annulus1
    exit
  * stretch the annulus *********
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
      unstretched-annulus1
    stretch
      $bStretch=7.; 
      specify stretching along axis=1
        layers
        1
        1. $bStretch 0.
        exit
      exit
    mappingName
    annulus1
    exit
  *
*
  Annulus
    inner radius
      $innerRadius
    outer radius
      $outerRadius
    centre for annulus
      +.6 -.6
    lines
      $nr $nTheta
    boundary conditions
      -1 -1 1 0
    mappingName
      unstretched-annulus2
    exit
  * stretch the annulus *********
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    unstretched-annulus2
    stretch
      specify stretching along axis=1
        layers
        1
        1. $bStretch 0.
        exit
      exit
    mappingName
    annulus2
    exit
  *
  exit
  generate an overlapping grid
    square
    annulus1
    annulus2
    done
    change parameters
      interpolation type
        $interpType
      ghost points
        all
        2 2 2 2 2 2
     order of accuracy
       fourth order
    exit
    compute overlap
    exit
  save an overlapping grid
  $gridName
  tcilc
  exit




