#
# --- make the ellipse using a spline
#    NOTE: go in a counter clockwise direction
#
$scmd="#";
$ns=50*$factor; $arcLength=0.;
for( $i=1; $i<=$ns; $i++ ){ $s=2.*$pi*($i-1.)/($ns-1.); $x=$radX*cos($s)+$cx; $y=$radY*sin($s)+$cy; \
   if( $i > 1 ){ $arcLength=$arcLength + sqrt( ($x-$x0)**2 + ($y-$y0)**2 );} $x0=$x; $y0=$y; \
   $scmd .= "\n $x $y"; }
# 
spline
  #
  enter spline points
    $ns
    $scmd
  lines
    $ns
    periodicity
      2
  mappingName
    curveBoundary
 exit
