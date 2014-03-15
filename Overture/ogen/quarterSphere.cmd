#
#  Ogen: Quarter of a sphere in a box
#
# usage: ogen [noplot] quarterSphere -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
#
#  ml = number of (extra) multigrid levels to support
#  xa, xb, ya, yb, za, zb : bounds on the channel
# 
# examples:
#     ogen noplot quarterSphere -order=2 -factor=1
#     ogen noplot quarterSphere -interp=e -order=2 -factor=1
#     ogen noplot quarterSphere -interp=e -order=2 -factor=2
#
# -- multigrid:
#     ogen noplot quarterSphere -order=2 -interp=e -factor=1 -ml=1
#     ogen noplot quarterSphere -order=2 -interp=e -factor=2 -ml=2
#     ogen noplot quarterSphere -order=2 -interp=e -factor=4 -ml=3
# 
# -- order=4 + MG
#     ogen noplot quarterSphere -order=4 -interp=e -factor=2 -ml=2
#
$xa=-2.5; $xb=2.5; $ya=0.; $yb=2.5; $za=0.; $zb=2.5; $nrExtra=0; $loadBalance=0; $ml=0;
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "za=f"=> \$za,"zb=f"=> \$zb,"ml=i"=>\$ml);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; $nrExtra=2.; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; $nrExtra=3.;  }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; $nrExtra=4.; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$name = "quarterSphere" . "$interp$factor" . $suffix . ".hdf";
# 
$ds=.1/$factor;
$pi=4.*atan2(1.,1.);
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
* 
*  --- OLD: 
* 
* $xa=-2.5; $xb=2.5; $ya=0.; $yb=2.5; $za=0.; $zb=2.5; 
* 
* $interpType="implicit for all grids";
* 
* $factor=.5; $name="quarterSphere0e.hdf"; $interpType="explicit for all grids"; 
* $factor=1.; $name="quarterSphere1e.hdf"; $interpType="explicit for all grids"; 
* $factor=2.; $name="quarterSphere2e.hdf"; $interpType="explicit for all grids";
* $factor=3.; $name="quarterSphere3e.hdf"; $interpType="explicit for all grids";
* $factor=3.; $name="quarterSphere3i.hdf"; $interpType="implicit for all grids";
* $factor=4.; $name="quarterSphere4e.hdf"; $interpType="explicit for all grids";
* 16M pts:
* $factor=8.; $name="quarterSphere8e.hdf"; $interpType="explicit for all grids";
* 130M pts:  
* $factor=16.; $name="quarterSphere16e.hdf"; $interpType="explicit for all grids";
* $factor=1.; $name="quarterSphere.hdf"; 
* $factor=2.; $name="quarterSphere2.hdf"; 
* $factor=4.; $name="quarterSphere4.hdf"; 
*
*$ds=1./10./$factor;
*$pi=3.141592653;
*
create mappings 
  * 
  Box 
    set corners 
*     0. 2.5 0. 2.5 -4. 5.
      $xa $xb $ya $yb $za $zb
    lines 
     $nx = intmg( ($xb-$xa)/$ds+1.5 );
     $ny = intmg( ($yb-$ya)/$ds+1.5 );
     $nz = intmg( ($zb-$za)/$ds+1.5 );
     $nx $ny $nz 
*      21 21 71   51 51 91
    boundary conditions
      3 4 2 2 2 2
    share
      0 0 3 0 2 0
    mappingName
      channel
    exit
  * 
  sphere
    $rad=1.; $deltaRad=(6.+$nrExtra)*$ds; 
    inner radius
      $rad
    outer radius
      $outerRad=$rad+$deltaRad;
      $outerRad
    lines
      $nPhi = intmg( $pi*$rad/$ds+1.5 );
      $nTheta = intmg( 2.*$pi*$rad/$ds+1.5 );
      $nr = intmg( $deltaRad/$ds + 1.5 );
      $nPhi $nTheta $nr
* 
    share
      0 0 0 0 1 0
    mappingName
      sphere-unrotated
    exit
* 
  rotate/scale/shift
    rotate
      -90 1
      0 0 0
    mappingName
      sphere-rotated
* pause
    exit
* 
  reparameterize
    transform which mapping?
      sphere-rotated
    orthographic
      choose north or south pole
      1
      specify sa,sb
        $sa=.6; $sb=$sa; 
        $sa $sb
      exit
    mappingName
    northPole-full
    exit
* 
  reparameterize
    transform which mapping?
      sphere-rotated
    orthographic
      choose north or south pole
        -1
      specify sa,sb
        $sa $sb
      exit
    mappingName
    southPole-full
    share
      0 0 0 0 1 0
    exit
*
  reparameterize
    transform which mapping?
      northPole-full
    restrict parameter space
      set corners
      .5 1. .5 1. 0. 1.
      exit
    boundary conditions
      2 0 2 0 1 0
    share
      2 0 3 0 1 0
    lines 
      $nx = intmg( .2*$pi*$rad/$ds+1.5 ); $ny=$nx; 
      $nx $ny $nr
    mappingName
      northPole
    exit
*
  reparameterize
    transform which mapping?
    southPole-full
    restrict parameter space
      set corners
      .5 1. 0. .5 0. 1.
      exit
    boundary conditions
      2 0 0 2 1 0
    share
      2 0 0 3 1 0
    lines 
      $nx = intmg( .2*$pi*$rad/$ds+1.5 ); $ny=$nx; 
      $nx $ny $nr
    mappingName
      southPole
    exit
*
  reparameterize
    transform which mapping?
      sphere-rotated
    restrict parameter space
      set corners
       $ra=.15; $rb=.85; $sa=0.; $sb=.25; 
       # .15 .85   0. .25  0. 1.
       $ra $rb $sa $sb 0. 1. 
      exit
    mappingName
      sphere
    boundary conditions
      0 0 2 2 1 0 
    share
      0 0 3 2 1 0 
    lines
      $nPhi = intmg( ($rb-$ra)*$pi*($rad+$deltaRad*.5)/$ds+1.5 );
      $nTheta = intmg( ($sb-$sa)*2.*$pi*($rad+$deltaRad*.5)/$ds+1.5 );
      $nr = intmg( $deltaRad/$ds + 1.5 );
      $nPhi $nTheta $nr
    exit
*
exit this menu
*
generate an overlapping grid
  channel
  sphere
  northPole
  southPole
 done
 change parameters
  interpolation type
    $interpType
  order of accuracy 
    $orderOfAccuracy
  ghost points
    all
    $ng $ng $ng $ng $ng $ng 
 exit
* 
* 
  compute overlap
 exit
*
* save an overlapping grid
save a grid (compressed)
  $name
  quarterSphere
exit

