*
*  Grid for a quarter cylinder
*
*
* usage: ogen [noplot] quarterCyl -factor=<num> -order=[2/4/6/8] 
* 
* examples:
*     ogen noplot quarterCyl -factor=1 -order=2
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "quarterCyl" . "$factor" . $suffix . ".hdf";}
* 
$ds=.2/$factor;
$pi=4.*atan2(1.,1.);
* 
create mappings
*
cylinder
  bounds on theta
    0 .25
  bounds on the axial variable
    $za=0.; $zb=1.; 
    $za $zb
  bounds on the radial variable
    $innerRad=1.; $outerRad=2.; $deltaRad=$outerRad-$innerRad;
    $innerRad $outerRad
  lines
    $nt=int( .25*2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nz=int( ($zb-$za)/$ds + 1.5 );
    $nr=int( $deltaRad/$ds+1.5 );
    $nt $nz $nr 
  boundary conditions
    1 1 2 2 3 4
  mappingName
    quarterCylinder
exit
*
exit
generate an overlapping grid
    quarterCylinder
  done
  change parameters
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  compute overlap
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
quarterCylinder
exit
