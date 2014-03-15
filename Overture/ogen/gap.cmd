***************************************************************************
*
*  Builds grids for a half an annulus for an axi-symmetric problem
*
* usage: ogen [noplot] hio -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot gap -factor=1
*
***************************************************************************
$order=2; $factor=35; $interp="e"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "gap" . "$interp$factor" . $suffix . ".hdf";}
* 
*
* domain parameters:  
$ds = .05/$factor; # target grid spacing
*
*
$pi=4.*atan2(1.,1.);
* 
create mappings 
*
 annulus 
    mappingName 
    shell 
    $shellThickness=0.1;
    $innerRadius=0.9;
    $outerRadius=$innerRadius+$shellThickness;
    inner and outer radii 
      $innerRadius $outerRadius
    start and end angles
      0. 0.5
    lines 
      $nx=int( ($pi*$outerRadius)/$ds+1.5 );
      $ny=int( ($shellThickness)/$ds+2.5 );
      $nx $ny
    boundary conditions 
      13 13 1 1
*      1 13 1 1
    exit 
  exit this menu 
*
generate an overlapping grid 
  shell	
  done 
*
  change parameters 
    ghost points 
      all 
      2 2 2 2 2 2
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    exit 
    compute overlap
  exit
save a grid
$name
gap
exit
