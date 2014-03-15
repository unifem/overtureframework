*
* Circle in a channel (taking arguments)
*
*
* usage: ogen [noplot] cicNurbs -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot cicNurbs -order=2 -interp=e -factor=8
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
if( $name eq "" ){$name = "cicNurbs" . "$interp$factor" . $suffix . ".hdf";}
* 
$ds=.1/$factor;
* 
create mappings
*
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 1 1 1
  mappingName
  square0
exit
*
  nurbs
    set domain dimension
    2
    set range dimension
    2
    enter points
    2 2 3
    $xa $ya
    $xb $ya
    $xa $yb
    $xb $yb
    lines
     $nx $ny
    mappingName
     square
 exit
*
Annulus
  $nr = 5+$ng;
  $nr = .3/$ds +$ng;
  $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 0
  mappingName
   annulus0
exit
#----
Annulus
  $innerRad=.4; $outerRad = $innerRad + ($nr-1)*$ds;
  centre for annulus
    -1. 1.
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 0
  mappingName
   annulus2
exit
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs \n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
*
*
*convertToNurbs("square0","square",0.);
*$commands
convertToNurbs("annulus0","Annulus",0.);
$commands
convertToNurbs("annulus2","Annulus2",0.);
$commands
*
exit
generate an overlapping grid
    square
    Annulus
    Annulus2
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
*  display intermediate results
#   debug 
#     15
#   compute overlap
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
#   continue
**  display computed geometry
* 
  compute overlap
#  output inverse statistics
* 
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
cicNurbs
exit

