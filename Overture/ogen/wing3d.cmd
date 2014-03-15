*
* 3D Wing from cross-sections
*
* usage: ogen [noplot] wing3d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -wingSpan=<num>
*
* examples:
*     ogen noplot wing3d -factor=1 
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$wingSpan=2.; $chord =1.5; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"wingSpan=f"=> \$wingSpan);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
* 
$suffix = ".order$order"; 
$name = "wing3d" . "$interp$factor" . $suffix . ".hdf";
* 
* 
$ds=.05/$factor;
* 
create mappings
* 
* ----- Build the wing from cross-sections ----
*   NOTE: The cap grids work best if the cross-sections approach an ellipse at the ends.
*
  Circle or ellipse (3D)
    mappingName
      ellipse0
    specify axes of the ellipse
      .7 .2 
    specify centre
      $z0=-$wingSpan*.5; 
      0 0. $z0
    exit
*
  Circle or ellipse (3D)
    mappingName
      ellipse1
    specify axes of the ellipse
      .7 .2 
    specify centre
      0 0 0.
    exit
*
  Circle or ellipse (3D)
    mappingName
      ellipse2
    specify axes of the ellipse
      .7 .2 
    specify centre
      $z1=$wingSpan*.5; 
      0 0 $z1
    exit
*
  CrossSection
    mappingName
      wingSurface
    general
      3
    ellipse0
    ellipse1
    ellipse2
    polar singularity at start
    polar singularity at end
*    specify how rapidly the cross-sections converge to a point at the ends: 
    polar singularity factor
      3.
*    pause
  exit
* -- add a cap to the end of the wing 
  reparameterize
    orthographic
      specify sa,sb
        * sa=edge-direction
         $sa=.6; $sb=.5; 
         $sa $sb
      exit
    mappingName
      cap1-surface
    exit
* -- add a cap to the other end
  reparameterize
    orthographic
      specify sa,sb
        * sa=edge-direction
        $sa $sb
      choose north or south pole
      -1
      exit
    mappingName
      cap2-surface
    exit
* 
*  -- remove the singular ends from the wing --
* 
  reparameterize
    transform which mapping?
      wingSurface
    restrict parameter space
      set corners
        0. 1. .02 .98
      exit
    mappingName
      windSurfaceWithoutEnds
    exit
* 
* -- build volume grid for the wing --
  mapping from normals
    extend normals from which mapping?
      windSurfaceWithoutEnds
    normal distance
      $nDist=$ds*5.; # fixed normal distance
      $nDist
    lines
      $length=$wingSpan; 
      $ns = int( $length/$ds + 1.5 );
      $nTheta=int( 2.5*$chord/$ds+1.5 );
      $nr = int( $nDist/$ds + 5.5 ); 
      * 61 141 11
      $nTheta $ns $nr
    boundary conditions
      -1 -1 0 0 1 0       
    share
      0 0 0 0 2 0
    mappingName
      wing-unstretched
    exit
* 
*  -- build volume grids for the cap 
  mapping from normals
    extend normals from which mapping?
     cap1-surface
    normal distance
      $nDist
    share
      0 0 0 0 2 0
    mappingName
      cap1-unstretched
    lines
      $na = int( $chord/$ds + 1.5 );  # along edge of tip
      $nb = int( .5*$chord/$ds + 1.5 );  # around the tip from top to bottom
      * 11 21 11
      $na $nb $nr 
    exit
* 
  mapping from normals
    extend normals from which mapping?
     cap2-surface
    normal distance
      $nDist
    share
      0 0 0 0 2 0
    mappingName
      cap2-unstretched
    lines
      $na $nb $nr
    exit
* -- stretch the grid lines to be better spaced ---
  stretch coordinates
    transform which mapping?
      cap1-unstretched
    Stretch r1:itanh
    STP:stretch r1 itanh: layer 0 1. 5. .5 (id>=0,weight,exponent,position)
    Stretch r2:itanh
    STP:stretch r2 itanh: layer 1 1. 5. .5 (id>=0,weight,exponent,position)
    Stretch r3:itanh
    STP:stretch r3 itanh: layer 1 1. 5. 0. (id>=0,weight,exponent,position)
    stretch grid
    mappingName
     cap1
    exit
* -- stretch the grid lines to be better spaced ---
  stretch coordinates
    transform which mapping?
      cap2-unstretched
    Stretch r1:itanh
    STP:stretch r1 itanh: layer 0 1. 5. .5 (id>=0,weight,exponent,position)
    Stretch r2:itanh
    STP:stretch r2 itanh: layer 1 1. 5. .5 (id>=0,weight,exponent,position)
    Stretch r3:itanh
    STP:stretch r3 itanh: layer 1 1. 5. 0. (id>=0,weight,exponent,position)
    stretch grid
    mappingName
     cap2
    exit
* -- stretch the grid lines to be better spaced ---
  stretch coordinates
    transform which mapping?
      wing-unstretched
    Stretch r3:itanh
    STP:stretch r3 itanh: layer 1 1. 5. 0. (id>=0,weight,exponent,position)
    stretch grid
    mappingName
     wing
    exit
*
* Here is the box
*
Box
  set corners
    $xa=-1.; $xb=1.; $ya=-.5; $yb=.5; $za=-.75*$wingSpan; $zb=-$za;
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  mappingName
    box
  exit
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
*
* -- it is faster to evaluate the Nurbs than the original cap patches --
convertToNurbs("cap1","cap1-nurbs",0.);
$commands
convertToNurbs("cap2","cap2-nurbs",0.);
$commands
convertToNurbs("wing","wing-nurbs",0.);
$commands
* 
exit
generate an overlapping grid
  box
  wing-nurbs
  cap1-nurbs
  cap2-nurbs
  done choosing mappings
* 
 compute overlap
exit
* save an overlapping grid
save a grid (compressed)
$name
wing3d
exit
