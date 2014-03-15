#********************************************************************************************
#           Grids for a cylindrical tower with a rounded cap
# Input:
#    $towerRadius : radius of the wire
#    $ds : target grid spacing
#    $dsBL : target boundary layer grid spacing
#    $towerShare : shared boundary flag
#    $rDist : radial distance for grids 
#    $nr : number of point in the radial direction
#    $towerStart,$towerEnd : start and end of the cylinder
#    $capExtent : parameter (sa) for cap orthographic transform, default = .25
#
# These commands create:
#
#      tower
#      towerCap
#*******************************************************************************************
$pi=4.*atan2(1.,1.);
if( $capExtent eq "" ){ $capExtent=.25; }  #
if( $dsBL eq "" ){ $ds=$dsBL; }  #
#$towerRadius=.25; $ds=.05; 
#create mappings
#
#  -- build a cross-section curve that will define a cylinder with a spherical end ---
#    Go all the away round so the shape is symmetric at the back and front
  spline
   enter spline points
    $r0=$towerRadius; 
    $m=intmg( ($towerEnd-$towerStart-$towerRadius)/$ds + 1.5); 
    $xac=$towerStart; $xbc=$towerEnd-$r0; $xbac=$xbc-$xac; $yac=$r0;  $x0=$towerEnd-$r0; $y0=0.; 
    $c="";
    $n=intmg( .5*$pi*$towerRadius/$ds + 1.5 ); 
    $n2=2*$n; 
    * top straight section: 
    for( $i=0; $i<$m; $i++ ){ $x=$xac+$xbac*$i/($m-1); $y=$yac; $c=$c . "$x $y\n"; }
    * circle on right: 
    for( $i=1; $i<$n2; $i++ ){ $theta=$pi*.5 - $pi*$i/$n2; $x=$r0*cos($theta)+$xbc; $y=$r0*sin($theta)+$y0; $c=$c . "$x $y\n"; }
    * bottom straight section: 
    for( $i=$m-1; $i>=0; $i-- ){ $x=$xac+$xbac*$i/($m-1); $y=-$yac; $c=$c . "$x $y\n"; }
#
    $npts=2*$m+$n2-1;
    $npts
    $c
    lines
      $npts
    * pause
    mappingName
      rounded-tower-cross-section
    *  pause
    exit
#
#  Take the top half of the curve 
#
  reparameterize
    transform which mapping?
      rounded-tower-cross-section
    restrict parameter space
      specify corners
        0. .5 
      exit
#      pause
    mappingName
     reparameterized-rounded-tower-cross-section
     * pause
    exit
#
  mapping from normals
    extend normals from which mapping?
      reparameterized-rounded-tower-cross-section
    normal distance
      -$rDist
    mappingName
     rounded-tower-2d
    * pause
    exit
# 
  body of revolution
    mappingName
       rounded-tower
    tangent of line to revolve about
      1. 0. 0.
    lines
      $nTheta = intmg( 2.*$pi*$r0/$ds + 1.5 );
      $nz = intmg( ( ($towerEnd-$towerStart-$r0) + $pi*$r0)/$ds + 1.5 );
      $nz $nTheta $nr
    boundary conditions
      $towerBottomBC  0 -1 -1 $towerBC 0
    share
      $groundShare 0 0 0 $towerShare 0
#**********************************************
    ** parameter axes
    ** 0 1 2
    * pause
  exit
#
  reparameterize
    orthographic
      choose north or south pole
        -1
      specify sa,sb
        $sa=$capExtent;
        $sa $sa 
      exit
    lines
      $nx = intmg( .8*$pi*($r0+$rDist*.25)/$ds +1.5 );
      $nx $nx $nr
    share
      0 0 0 0 $towerShare 0
    mappingName
      towerCap-unstretched
     * pause
    exit
#
#  -- remove the singular part of the end-section --
  reparameterize
    transform which mapping?
      rounded-tower
    restrict parameter space
      set corners
       0. .95  0 1. 0. 1.
      exit
    lines
      $nTheta = intmg( 2.*$pi*$r0/$ds + 1.5 );
      $nz = intmg( ( ($towerEnd-$towerStart-$r0) + $pi*$r0)/$ds + 1.5 );
      $nz $nTheta $nr
#
    mappingName
     tower-unstretched
    * pause
   exit
#
  stretch coordinates
    transform which mapping?
      towerCap-unstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name towerCap
  exit
#
  stretch coordinates
    transform which mapping?
      tower-unstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name tower
  exit
#
#**********************************************************************
