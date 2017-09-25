#
#  plotStuff plotGrid -show=radialElasticPistonGride2.order2.hdf -name=radialElasticPistonGrid
#  plotStuff plotGrid -show=elasticPistonGrid2.order2.hdf -name=elasticPistonGrid
#
$show="elasticPistonGrid2.order2.hdf";
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show,"name=s"=>\$name  );
#
$show
#
  # bigger
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
#
 colour boundaries by grid number
if( $name eq "elasticPistonGrid" ){ \
  $cmd="colour grid lines from chosen name\n grid colour 0 RED\n grid colour 1 BLUE\n grid colour 2 GREEN"; }else{ $cmd="#"; }
  $cmd
#
  line width scale factor:0 3
  plot interpolation points 1
  # colour interpolation points 1
#
  hardcopy file name:0 $name.ps
  hardcopy save:0
  hardcopy save:0
