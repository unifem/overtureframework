*
*  Grid for an orthographic projection on a sphere
*
*
* usage: ogen [noplot] orthoSphere -factor=<num> -order=[2/4/6/8] -ml=<>
* 
* examples:
*     ogen noplot orthoSphere -factor=1 -order=2
#     ogen noplot orthoSphere -factor=2 -order=2 -ml=1
#
#     ogen noplot orthoSphere -factor=1 -order=4 -ml=1
#     ogen noplot orthoSphere -factor=2 -order=4 -ml=2
*
$order=2; $factor=1; $interp="i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
* 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "orthoSphere" . "$factor" . $suffix . ".hdf";}
* 
$ds=.1/$factor;
$pi=4.*atan2(1.,1.);
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
* *
*
create mappings
* first make a sphere
Sphere
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    specify sa,sb
    * 2.5 2.5
      1.75 1.75 
  exit
  lines
    $nx = intmg( 1.4/$ds + 1.5 );
    $ny = intmg( 1.4/$ds + 1.5 );
    $nz = intmg(  .5/$ds + 1.5 );
    $nx $ny $nz 
*   15 15 7   
  boundary conditions
    1 2 3 4 5 6
  mappingName
    north-pole
  exit
exit
*
generate an overlapping grid
  north-pole
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
save an overlapping grid
$name
orthoSphere
exit
