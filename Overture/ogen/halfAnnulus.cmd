*
*
*
$xa=-1.; $xb=1.; $ya=0.; $yb=1.; 
$interpType="implicit for all grids";
* 
* $factor=1.; $name="halfAnnulus.hdf"; 
* $factor=1.; $name="halfAnnulus1e.hdf"; $interpType="explicit for all grids";
$factor=2.; $name="halfAnnulus2.hdf"; 
* $factor=2.; $name="halfAnnulus2e.hdf"; $interpType="explicit for all grids";
* $factor=4.; $name="halfAnnulus4.hdf"; 
* $factor=4.; $name="halfAnnulus4e.hdf"; $interpType="explicit for all grids";
* $factor=8.; $name="halfAnnulus8e.hdf"; $interpType="explicit for all grids";
* $factor=16.; $name="halfAnnulus16e.hdf"; $interpType="explicit for all grids";
*
$ds=1./20./$factor;
$pi=3.141592653;
* 
create mappings
  Annulus
    start and end angles
      $theta0=0.; $theta1=.5; 
      $theta0 $theta1
    $deltaR=.25/$factor; 
    $ra=.25; $rb=$ra+$deltaR; 
    inner radius
      $ra
    outer radius
      $rb
    boundary conditions
      3 4 1 0
    share
      1 1 0 0
    lines
      $nTheta=int( $pi*($ra+$rb)*($theta1-$theta0)/$ds+1.5 );
      $nr = int( ($rb-$ra)/$ds + 1.5 );
      $nTheta $nr 
    exit
*
  rectangle
    set corners
      $xa $xb $ya $yb
    lines
     $nx = int( ($xb-$xa)/$ds+1.5 );
     $ny = int( ($yb-$ya)/$ds+1.5 );
     $nx $ny
    share
     0 0 1 3
    exit
*
exit this menu
*
  generate an overlapping grid
    square
    Annulus
    done
    change parameters
      ghost points
        all
        2 2 2 2 2 2
    interpolation type
      $interpType
    exit
    compute overlap
    exit
*
save an overlapping grid
  $name
  halfAnnulus
exit
