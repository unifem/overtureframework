#
#  Test ghost point placement 
#
#  Examples:
#     ../hype hypeCurveMatch -nr=31 -freq=3. -xb=0. -yb=1.  # acute corner
#     ../hype hypeCurveMatch -nr=31 -freq=3. -xb=-1. -yb=.3  # convex corner 
#
# Default parameters
$freq=3.; $nr=31;  $ns=11;  $xb=0.; $yb=1.; 
#
# get command line arguments
GetOptions( "nr=i"=>\$nr,"ns=i"=>\$ns,"freq=f"=> \$freq,"xb=f"=> \$xb,"yb=f"=> \$yb );
#
$pi = 4.*atan2(1.,1.);
#
  line (2D)
   # line at angle to match while marching 
    specify end points
      # (xa,ya) (xb,yb)
      0. 0. $xb $yb
    # 
    #   1 0 2 1
    # acute corner:
    #      1 0 .5 1
    mappingName
      matchingBoundary
    exit
# Start curve: 
  nurbs (curve)
    $amp=.1; $cmd="#"; 
    for( $i=0; $i<$nr; $i++ ){ $x = $i/($nr-1); $y=$amp*sin($freq*$pi*$x); $cmd .= "\n $x $y"; }
    enter points
      $nr 3
      $cmd
    lines
      $nr 
    mappingName
      startCurve
    exit
*
  hyperbolic 
    BC: left match to a mapping
      matchingBoundary
    #
    lines to march $ns
    distance to march .15
    plot cell quality
    plot bad cells 1
    # use new ghost point option
    GSM:ghost point option 1
    # this next option will set ghost values on the start line using the BC's
    # apply boundary conditions to start curve 1 
    apply boundary conditions to start curve 0
    debug 
      3
    backward
    generate
#
    fourth order
    ghost lines to plot: 2
    smoothing...
    GSM:number of iterations 1

    GSM:smooth grid

    GSM:smooth grid
