*
* Sliding plug in a channel for testing cgins and implicit time stepping
*
*  Examples:
*     ogen noplot slider -factor=1
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
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
if( $name eq "" ){$name = "slider" . "$interp$factor" . $suffix . ".hdf";}
* 
$ds=.1/$factor;
*
create mappings
  rectangle
    set corners
    $xa=0.; $xb=1.5; $ya=0.; $yb=1.; 
      $xa $xb $ya $yb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nx $ny
    boundary conditions
      3 0 2 2 
    share
      0 0 2 3
    mappingName
      backGround
    exit
*
  rectangle
    set corners
      $xa=.25; $xb=.5; $ya=0.; $yb=1.;
      $xa $xb $ya $yb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nx $ny 
    boundary conditions
      0 1 2 2 
    share
      0 0 2 3
    mappingName
      slider
    exit
  exit
*
generate an overlapping grid
  backGround
  slider
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng
  exit
  compute overlap
*
exit
save an overlapping grid
$name
slider
exit

