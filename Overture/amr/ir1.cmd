#
#  ir ir1.cmd -nd=[2|3] -ratio=[2|4] -test=[pi|cf|fc]
#
* cmd file for the "ir" test function
$test="pi"; $nd=2; $ratio=2; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"nd=i"=>\$nd,"ratio=i"=>\$ratio,"test=s"=>\$test );
# -------------------------------------------------------------------------------------------------
if( $test eq "pi" ){ $test="test parallel interpolation"; }
if( $test eq "cf" ){ $test="test coarse from fine"; }
if( $test eq "fc" ){ $test="test fine from coarse"; }
nd (dimensions for interp test)
 $nd
ratio
  $ratio
# test parallel interpolation
$test
exit


* test fine from coarse
* test interpolate refinement boundaries
exit
