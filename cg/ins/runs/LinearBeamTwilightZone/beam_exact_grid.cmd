#
# Create the initial grid for the simulation of a square cavity with a flexible beam on top 
# (twilight zone flow) 
# Use this grid with cgins.
#
# Usage:
#         ogen [noplot] beam_exact [options]
# where options are
#     -factor=<num>          : grid spacing factor
#     -interp=[e/i]          : implicit or explicit interpolation
#     -name=<string>         : over-ride the default name
#
# Examples:
#
#      ogen noplot beam_exact_grid -interp=e -factor=2
#
#
$factor=1; $name="";
$interp="i"; $interpType = "implicit for all grids";
$order=2; $orderOfAccuracy = "second order"; $ng=2;
$nExtra=0;
$H=0.3;
$L=0.3;
#
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp);
#
$ds0=0.01;
$ds=$ds0/$factor;
$nl=$H/$ds+1;
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "beam_exact_grid" . "$interp$factor" . ".hdf";}
create mappings 
  $hh=$H-$ds*4.0;
  rectangle 
  set corners
  0,$L,0,$hh
  lines
  $nl,$nl
  share 
  1,2,4,0
  boundary conditions 
  1,2,4,0
  exit
  line (2D)
    set end points 
      0,$L,$H,$H
    exit
  hyperbolic
*    $stretchFactor=1.0;
    forward
    $dm=6.9*$ds;
    distance to march $dm
    $nll=$nl-1;
    lines to march 8
    points on initial curve $nl
    # wdh: 
    geometric stretch factor 1.0
    generate 
    mapping parameters
    Boundary Condition: left 1
    Boundary Condition: right 2
    Boundary Condition: bottom 3
    Boundary Condition: top 0
    Share Value: left 1
    Share Value: right 2
    Share Value: bottom 3
    Share Value: top 0
    close mapping dialog
*    boundary condition options...
    BC: left fix x, float y and z  
    BC: right fix x, float y and z
*    BC: left periodic
*    BC: right periodic
    close marching options
    generate 
    name rec-hyperbolic
    exit
  exit this menu
#create mappings
generate an overlapping grid
  square
  rec-hyperbolic
  done
  change parameters
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all 
      $ng $ng $ng $ng $ng $ng
  exit
  compute overlap
  exit
save an overlapping grid
$name
beam_exact_grid
exit
