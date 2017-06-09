# ----- Cross ----
#    Start on a flat edge since the BC is clamped. 
#    NOTE: go in a counter clockwise direction
#
#   h        11----10
#             |    | 
#             |    | 
#             |    | 
#  +w  13-----12   9------8
#       |                 |
#      1,14     0
#       |                 |
#  -w   2-----3    6------7
#             |    | 
#             |    | 
#             |    | 
#             4----5 
#            -w    w
$degree=3;  # degree of Nurbs 
$w=.15; $h=.75    # w=halfwidth and h=length of the arm
$radX=$w+$h -3.*$ds; $radY=$radX; # for inner background grid 
$nc=14;  # number of control points 
$numPerSegment=2; # number of control points per segment -- increase to make corners sharper
@xc=(-$h,-$h,-$w,-$w, $w, $w, $h, $h, $w, $w,-$w,-$w,-$h,-$h);
@yc=( 0.,-$w,-$w,-$h,-$h,-$w,-$w, $w, $w, $h, $h, $w, $w, 0.);
if( $shape eq "cross" ){ $cmd="#";  $ns=0;  $arcLength=0.; \
for( $ic=0; $ic<$nc-1; $ic++ ){ $numpt=$numPerSegment; if( $ic == $nc-2 ){ $numpt=$numPerSegment+1;} \
for( $i=0; $i<$numpt; $i++ ){ $s=($i)/($numPerSegment); $ns=$ns+1;  \
   $x=(1.-$s)*$xc[$ic]+$s*$xc[$ic+1]; \
   $y=(1.-$s)*$yc[$ic]+$s*$yc[$ic+1];   \
   if( $i > 0 ){ $arcLength=$arcLength + sqrt( ($x-$x0)**2 + ($y-$y0)**2 );} $x0=$x; $y0=$y; \
   $cmd .= "\n $x $y 1."; } }\
  $knots="#"; for( $i=$degree-1; $i<$ns-($degree-1); $i++ ){ $s=$i/($ns-2); $knots .= "\n $s"; } \
}
#
nurbs (curve)
  periodicity
   2
  enter control points
    $degree
    $ns
    $knots
    $cmd 
 parameterize by chord length
 #
 lines
  $lines=intmg($arcLength/$ds + 1.5 );
  $lines
 mappingName
   curveBoundaryInitial
exit
# -- interpolate the initial NURBS so that we have an arclength parameterization --
nurbs (curve)
  interpolate from a mapping
    curveBoundary
  mappingName
   curveBoundary
exit 

   
