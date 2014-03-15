*********************************************************************************************
*                 Cylinder with rounded end caps
* Input:
*    $radius : radius of the wire
*    $ds : target grid spacing
*    $dsBL : target boundary layer grid spacing
*    $cylShare : shared boundary flag
*    $rDist : radial distance for grids 
*    $nr : number of point in the radial direction
*    $cylStart,$cylEnd : start and end of the cylinder
*
* These commands create:
*
*      end-cap1, end-cap2 : end caps 
*      cyl : cylinder with rounded ends
********************************************************************************************
$pi=4.*atan2(1.,1.);
if( $dsBL eq "" ){ $ds=$dsBL; }  #
*$radius=.25; $ds=.05; 
*create mappings
*
*  -- build a cross-section curve that will define a cylinder with a spherical end ---
*    Go all the away round so the shape is symmetric at the back and front
  spline
    mappingName
      rounded-cyl-profile
  enter spline points
    $r0=$radius; 
    $m=int( ($cylEnd-$cylStart)/$ds + 1.5); 
    $xac=$cylStart+$r0; $xbc=$cylEnd-$r0; $xbac=$xbc-$xac; $yac=$r0;  $x0=$cylEnd-$r0; $y0=0.; 
    $c="";
    * half circle on left: 
    $n=int( .5*$pi*$radius/$ds + 1.5 ); 
    $n2=2*$n; 
    for( $i=0; $i<$n; $i++ ){ $theta=$pi - $pi*$i/$n2; $x=$r0*cos($theta)+$xac; $y=$r0*sin($theta)+$y0; $c=$c . "$x $y\n"; }
    * top straight section: 
    for( $i=0; $i<$m; $i++ ){ $x=$xac+$xbac*$i/($m-1); $y=$yac; $c=$c . "$x $y\n"; }
    * circle on right: 
    for( $i=1; $i<$n2; $i++ ){ $theta=$pi*.5 - $pi*$i/$n2; $x=$r0*cos($theta)+$xbc; $y=$r0*sin($theta)+$y0; $c=$c . "$x $y\n"; }
    * bottom straight section: 
    for( $i=$m-1; $i>=0; $i-- ){ $x=$xac+$xbac*$i/($m-1); $y=-$yac; $c=$c . "$x $y\n"; }
    * half circle on left: 
    for( $i=1; $i<=$n; $i++ ){ $theta=$pi*1.5 - $pi*$i/$n2; $x=$r0*cos($theta)+$xac; $y=$r0*sin($theta)+$y0; $c=$c . "$x $y\n"; }
*
    $npts=2*$m+$n+$n+$n2-1;
    $npts
    $c
    periodicity
      2 
    lines
      $npts
    * pause
    mappingName
      rounded-cyl-cross-section
    * pause
    exit
*
*  Take the top half of the curve 
*
  reparameterize
    transform which mapping?
      rounded-cyl-cross-section
    restrict parameter space
      specify corners
        0. .5 
      exit
*      pause
    mappingName
     reparameterized-rounded-cyl-cross-section
     * pause
    exit
*
  mapping from normals
    extend normals from which mapping?
*      stretched-end-cross-section
      reparameterized-rounded-cyl-cross-section
    normal distance
      -$rDist
    mappingName
     rounded-cyl-2d
     * pause
    exit
* 
  body of revolution
    mappingName
       rounded-cyl
    tangent of line to revolve about
      1. 0. 0.
    lines
      $nTheta = intmg( 2.*$pi*$r0/$ds + 1.5 );
      $bladeFactor=2.; # increase pts in axial direction on blades
      $nz = intmg( ( $bladeFactor*($cylEnd-$cylStart-$r0*2.) + $pi*$r0)/$ds + 1.5 );
      $nz $nTheta $nr
    boundary conditions
      0  0 -1 -1 1 0
    share
      0 0 0 0 $cylShare 0
***********************************************
    ** parameter axes
    ** 0 1 2
    * pause
    exit
*
  reparameterize
    orthographic
      choose north or south pole
        -1
      specify sa,sb
        $sa=.2; 
        $sa $sa 
      exit
    lines
      $nx = intmg( .5*$pi*($r0+$rDist*.25)/$ds +1.5 );
      $nx $nx $nr
    share
      0 0 0 0 $cylShare 0
    mappingName
      endCap1-unstretched
*   pause
    exit
*
  reparameterize
    orthographic
      choose north or south pole
        +1
      specify sa,sb
        $sa $sa 
      exit
    lines
      $nx $nx $nr
    share
      0 0 0 0 $cylShare 0
    mappingName
      endCap2-unstretched
     * pause
    exit
*
*  -- remove the singular part of the end-section --
  reparameterize
    transform which mapping?
      rounded-cyl
    restrict parameter space
      set corners
       .05 .95  0 1. 0. 1.
      exit
    lines
      $nTheta = intmg( 2.*$pi*$r0/$ds + 1.5 );
      $bladeFactor=2.; # increase pts in axial direction on blades
      $nz = intmg( ( $bladeFactor*($cylEnd-$cylStart-$r0*2.) + $pi*$r0)/$ds + 1.5 );
      $nz $nTheta $nr
    mappingName
     cyl-unstretched
    * pause
   exit
#
  stretch coordinates
    transform which mapping?
      endCap1-unstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name endCap1
  exit
#
  stretch coordinates
    transform which mapping?
      endCap2-unstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name endCap2
  exit
#
  stretch coordinates
    transform which mapping?
      cyl-unstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name cyl
  exit
*
***********************************************************************
