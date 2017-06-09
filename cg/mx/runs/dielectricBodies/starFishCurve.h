#
#  Curve for a star fish 
#    NOTE: go in a counter clockwise direction
#
$cmd="#";
$ra=.4;  # inner radius
$rb=.6; # half height of arms
if( $shape eq "starFish" ){ $radX=$ra+$rb; $radY=$radX; }
$nArms=6;  # number of arms
$alpha0=1.; 
$ns=$nArms*10*$factor+1;  # number of spline points
$alpha=$alpha0*$pi/$nArms;  # alpha=pi/nArms : peaks are shifted to positions of troughs
$arcLength=0.;
for( $i=0; $i<$ns; $i++ ){ $s=2.*$pi*($i-1.)/($ns-1.); $y= (.5*(1.+sin($nArms*$s)) )**2; $x = $s + $alpha*$y**2; $r=$ra+$rb*$y; $xx=$r*cos($x); $yy=$r*sin($x); \
 if( $i > 0 ){ $arcLength=$arcLength + sqrt( ($xx-$x0)**2 + ($yy-$y0)**2 );} $x0=$xx; $y0=$yy; \
$cmd .= "\n $xx $yy"; }
#
spline
  #
  enter spline points
    $ns
    $cmd
  lines
    $ns
    periodicity
      2
  mappingName
    curveBoundary
 exit
