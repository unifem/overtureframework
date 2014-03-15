#
#  Grid for a mars capsule -- geometry courtsey of Anima Dinesh
#
# Usage
#    ogen [-noplot] marsCapsule -factor=<num> -interp=[i|e]
# Options:
#   -factor = resolution factor (integer) ***NOT implemented yet***
#   -interp = implicit or explicit interpolation
#
#   Examples:
#     ogen noplot marsCapsule -interp=i -factor=1
#     ogen noplot marsCapsule -interp=e -factor=1
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
$name = "marsCapsule" . "$interp$factor" . $suffix . ".hdf";
# 
#  Here is the target grid spacing -- finish me --
$ds=.2/$factor;
# 
create mappings
  read iges file
    # was called prt0003.igs : 
    marsCapsule.igs
    continue
    choose all
    CSUP:determine topology
    deltaS 0.1
    maximum area .01
    compute topology
    exit
    CSUP:mappingName marsSurface
    exit
# 
# NOTE: The CAD surfaces on the nose of the vehicle cannot be correctly
#    evaluated at the singular point on the tip -- so we build a grid
#    over the cap by making a new surface for the nose.
# 
#  -- extract a cross-section curve from the nose surface
  reduce domain dimension
    reduce the domain dimension of which mapping?
    trimmed-nurbs 44
    mappingName
    nose_crossSection
    exit
# make a new nose as a body of revolution
  body of revolution
    revolve which mapping?
      nose_crossSection
    choose a point on the line to revolve about
      0 0 0
    mappingName
      nose_surface_singular
    exit
# create an orthographic patch to cover the polar singularity
  reparameterize
    orthographic
      specify sa,sb
        2.5 2.5 
      exit
    mappingName
     nose_surface
    lines 
      25 25
    exit
# grow a volume grid for the nose
  hyperbolic
    target grid spacing .2 .2 (tang,normal, <0 : use default)
    backward
    lines to march 9
    generate
    share
      0 0 0 0 1 0
    name nose
    exit
#
#  -- now we can build a grid on the body
#
  builder
   Start curve:marsSurface
    *
    * Set the target grid spacing:
    * 
    target grid spacing .2 .2  (tang,normal, <0 : use default)
    create surface grid...
* 
      choose edge curve 9 -4.998836e+00 1.254231e+01 -1.250638e-12 
      choose edge curve 11 4.998836e+00 1.254231e+01 1.250638e-12 
      done
      forward and backward
      lines to march 16, 67 (forward,backward)
      generate
      exit
*
    create volume grid...
      backward
      lines to march 9
      generate
      share
        0 0 0 0 1 0
      name body
    exit
#
#    Now make a grid on the base of the capsule
#
    create surface grid...
      surface grid options...
      initial curve:points on surface
      choose point on surface 8 -2.4 1.299980e+01 2.5
      choose point on surface 8  2.4 1.299980e+01 2.5
      done
      backward
      lines to march 26
      generate
      name base_surface
      exit
#
    create volume grid...
      lines to march 9 
      generate
      share
        0 0 0 0 1 0
      name base
      exit
#  Now build a back ground grid
    build a box grid
      x bounds: -7 7
      y bounds: -2, 16
      z bounds: -7 7
      bc 1 1 1 1 1 1 (l r b t b f)
      name backGround
      exit
    exit
  exit this menu
#
generate an overlapping grid
  backGround
  body
  nose
  base
 done choosing mappings
  change parameters
 # improve quality of interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
 compute overlap
exit
# save an overlapping grid
save a grid (compressed)
$name
marsCapsule
exit
