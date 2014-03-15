*
* Create a grid for the appolo crew re-entry vehicle
*
* usage: ogen [noplot] crv -factor=<num> -interp=[e/i] -dw=<num> -iw=<> 
*  -dw : discretization width : for 2 layers of interpolation points set the dw to 5 
*  -iw : interpolation width 
* examples:
*     ogen noplot crv -factor=1 
*     ogen noplot crv -factor=2 
*     ogen noplot crv -factor=2 -dw=5 -iw=5 -interp=e -name="crve2.order4.hdf"
*     ogen noplot crv -factor=3 -dw=5 -iw=5 -interp=e -name="crve3.order4.hdf"
*     ogen noplot crv -factor=4 -dw=5 -iw=5 -interp=e -name="crve4.order4.hdf"
*
* srun -N1 -n2 -ppdebug $ogenp noplot crv
* 
$order=2; $factor=1;  $interp="e"; $interpType = "explicit for all grids"; # default values
$orderOfAccuracy = "second order"; $ng=2; $dw=3; $iw=3; $loadBalance=0; 
$name=""; 
$xa=-2.; $xb=5.; $ya=-3.5; $yb=3.5; $za=-3.5; $zb=3.5; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "dw=i"=> \$dw,"iw=i"=> \$iw,"interp=s"=> \$interp,"name=s"=> \$name,"loadBalance=i"=>\$loadBalance);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $interp eq "i" ){ $interpType = "implicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "crv" . "$interp$factor" . $suffix . ".hdf";}
* 
$ds=.2/$factor;  # target grid spacing
$pi = 4.*atan2(1.,1.);
* 
* parallel ghost lines: for ogen we need at least:
*       .5*( iw -1 )   : implicit interpolation 
*       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
* ---------------------------------------
turn off graphics
$loadBalanceCmd = $loadBalance ? "load balance" : "*";
$loadBalanceCmd
* ---------------------------------------
create mappings
*
  Box
    mappingName
      backGround
    set corners
     $xa $xb $ya $yb $za $zb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
      $nx $ny $nz
      * 112 32 36  98 28 32   84 24 27   71 21 24
    * physical boundary conditions can be any positive number 
    * left, right, bottom, top, front back 
    boundary conditions
      1 2 3 4 5 6 
    exit
*
* First create the outline of the hull.
* Go all the away round so the shape is
* symmetric at the back and front
  spline
    mappingName
    hull-profile
  enter spline points
   31
   0.0000000000000000e+00   0.0000000000000000e+00
   1.5917874396135265e-02   3.8478688524590166e-01
   1.2734299516908212e-01   1.0260983606557377e+00
   2.7060386473429954e-01   1.5551803278688525e+00
   3.8202898550724640e-01   1.8598032786885246e+00
   4.9345410628019321e-01   1.9560000000000000e+00
   6.5263285024154594e-01   1.9239344262295082e+00
   7.0038647342995164e-01   1.8918688524590164e+00
   9.7099033816425118e-01   1.7155081967213115e+00
   1.7509661835748791e+00   1.2184918032786884e+00
   2.4035990338164250e+00   8.0163934426229511e-01
   3.0084782608695653e+00   4.0081967213114755e-01
   3.1676570048309176e+00   2.8859016393442621e-01
   3.2313285024154590e+00   2.0842622950819673e-01
   3.2790821256038645e+00   9.6196721311475414e-02
   3.2949999999999999e+00   0.0000000000000000e+00
   3.2790821256038645e+00  -9.6196721311475414e-02
   3.2313285024154590e+00  -2.0842622950819673e-01
   3.1676570048309176e+00  -2.8859016393442621e-01
   3.0084782608695653e+00  -4.0081967213114755e-01
   2.4035990338164250e+00  -8.0163934426229511e-01
   1.7509661835748791e+00  -1.2184918032786884e+00
   9.7099033816425118e-01  -1.7155081967213115e+00
   7.0038647342995164e-01  -1.8918688524590164e+00
   6.5263285024154594e-01  -1.9239344262295082e+00
   4.9345410628019321e-01  -1.9560000000000000e+00
   3.8202898550724640e-01  -1.8598032786885246e+00
   2.7060386473429954e-01  -1.5551803278688525e+00
   1.2734299516908212e-01  -1.0260983606557377e+00
   1.5917874396135265e-02  -3.8478688524590166e-01
   0.0000000000000000e+00   0.0000000000000000e+00
    lines
      51
    * pause
    periodicity
      2
    exit
*
*  Take the top half of the curve hull
*
  reparameterize
    restrict parameter space
      specify corners
        0. .5
      exit
*      pause
    mappingName
     reparameterized-hull-profile 
    exit
* Stretch the grid lines
  stretch coordinates
    transform which mapping?
    reparameterized-hull-profile
    stretch
      specify stretching along axis=0 (x1)
        layers
          2
          1. 7. .35
          1. 5. 1.
        exit
      exit
    mappingName
      stretched-reparameterized-hull-profile
    exit
*
  mapping from normals
    extend normals from which mapping?
    stretched-reparameterized-hull-profile
    normal distance
     *  $nDist=1.1/$factor; 
     $nr = 7 + $dw;
     $nDist=($nr-2)*$ds;
      -$nDist
*    pause
    exit
* 
*  -- create a body of revolution ---
  body of revolution
    mappingName
      hull-full
    tangent of line to revolve about
      1. 0. 0.
    lines
      $factor=2.; # increase grid lines due to stretching 
      $arcLength=6.; # approximate arcLength of the hull cross-section 
      $radius=2.;     # approximate radius of the hull
      $ns = int( $arcLength*$factor/$ds + 1.5 );
      $nTheta = int( 2.*$pi*$radius/$ds + 1.5 );
      $nr = int( $nDist/$ds + 1.5 );
      $ns $nTheta $nr
      * 65 31 11  51 31 11
    boundary conditions
      0  0 -1 -1 1 0
    share
      0 0 0 0 1 0
*    pause
    exit
*
  reparameterize
    orthographic
      specify sa,sb
        * sa and sb specify how big the orthographic patch is 
        $sa=.3; $sb=$sa; 
        $sa $sb
      exit
    lines
      $width = 3.; # approximate width of the cap grid 
      $nx = int( $width/$ds + 1.5 );
      $ny = int( $width/$ds + 1.5 );
      $nz = int( $nDist/$ds + 1.5 );
      $nx $ny $nz 
      * 11 11 11
    share
      0 0 0 0 1 0
    mappingName
      hull-bow-cap
*    pause
    exit
*
  reparameterize
    orthographic
      specify sa,sb
        * sa and sb specify how big the orthographic patch is 
        $sa=.25; $sb=$sa; 
        $sa $sb
      choose north or south pole
        -1
      exit
    lines
      $factor=3.; # add extra grid lines since the grid lines are stretched near the bow
      $width = .8*$factor; # approximate width of the cap grid 
      $nx = int( $width/$ds + 1.5 );
      $ny = int( $width/$ds + 1.5 );
      $nz = int( $nDist/$ds + 1.5 );
      $nx $ny $nz 
      * 11 11 11
    share
      0 0 0 0 1
    mappingName
      hull-stern-cap
*    pause
    exit
*
* -- now remove the singular ends from the hull-full
  reparameterize
    transform which mapping?
    hull-full
    set corners
      .05 .95 0 1 0 1.
    mappingName
      hull
  exit
*
* 
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
convertToNurbs("hull-bow-cap","hull-bow-cap-nurbs",0.);
$commands
convertToNurbs("hull-stern-cap","hull-stern-cap-nurbs",0.);
$commands
convertToNurbs("hull","hull-nurbs",0.);
$commands
exit
*
generate an overlapping grid
     backGround
     hull-nurbs
     hull-bow-cap-nurbs
     hull-stern-cap-nurbs
*    hull-bow-cap
*    hull-stern-cap
  done choosing mappings
*   change the plot
*     toggle grids on and off
*     0 : backGround is (on)
*     exit this menu
*   exit this menu
*
    change parameters
      interpolation type
        $interpType
*    -- for 2 layers of interpolation points set the dw to 5 
     discretization width
       all
       $dw $dw $dw
*     -- iw=2 : linear interpolation 
      interpolation width 
        all
        all
        $iw $iw $iw
    exit
  * pause
*  display intermediate
*
  compute overlap
  output inverse statistics
  * pause
exit
*
maximum number of parallel sub-files
  8
save an overlapping grid
$name
crv
exit
