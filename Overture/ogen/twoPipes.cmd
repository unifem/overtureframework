*
* Create a grid for the interior region of two intersecting pipes.
* Use an analytically defined fillet grid to accurately join the pipes. 
*
* usage: ogen [noplot] twoPipes -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot twoPipes -factor=1 -order=2 
*     ogen noplot twoPipes -factor=2 -interp=e
*     ogen noplot twoPipes -factor=4 -interp=e
*
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
* 
$suffix = ".order$order"; 
$name = "twoPipes" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
$Pi=4.*atan2(1.,1.);
* 
* Cyl1 : radius = $rad1
* Cyl2 : radius = $rad2 
* fillet : $width = half-width side of the fillet 
*  NOTE: $rad1 >= $rad2 - $width (required by current defintion of the fillet)
* $rad1=1.; $rad2=.75; $width=.15; 
* $rad1=1.; $rad2=.5; $width=.2; 
$rad1=1.; $rad2=.7; $width=.25;   # explicit-interp, factor=2 needs width=.25 
if( $factor eq 4 ){ $width=.25; } 
*
create mappings
*
*
* -- build a special fillet grid that connects two cylinders 
  user defined 1
    fillet for two cylinders
      *  Note: rad1 >= rad2 - d 
      $rad1 $rad2 $width
    lines
      $fact=1.2; 
      $nTheta = int( $fact*2.*$Pi*($rad1+$rad2)*.5/$ds +1.5 );
      $sfact=1.5; # extra lines for later stretching 
      $ns = int( $sfact*2.*$width/$ds + 1.5 );
      $nTheta $ns
    mappingName
     fillet-surface
  exit
*
*   stretch fillet in tangential direction to put more points near the corner region
  stretch coordinates
    Stretch r2:itanh
    STP:stretch r2 itanh: layer 0 1 5 0.5 (id>=0,weight,exponent,position)
    stretch grid
    STRT:name stretched-fillet-surface
    exit
* 
*   --- volume fillet grid 
  mapping from normals
    extend normals from which mapping?
      stretched-fillet-surface
    normal distance
      $nr = 7;  # always keep this many points in the normal direction
      $ndist= ($nr-1)*$ds;
      -$ndist
    lines
      $nTheta $ns $nr
    boundary conditions
      -1 -1 0 0 3 0
    share 
      0 0  0 0  3 0 
    mappingName
      fillet-volume
    * pause
  exit
* For faster evaluation -- interpolate to a Nurbs
  nurbs (surface)
    interpolate from a mapping
      fillet-volume
    mappingName
      fillet
   exit 
* 
*
*  ---  horizontal pipe parallel to the x-axis ---
  Cylinder
    mappingName
      main-pipe
    orientation
      1 2 0
    bounds on the axial variable
      $xa $xb 
    bounds on the radial variable
      $dr = ($nr-1)*$ds; $innerRad1=$rad1-$dr; 
      $innerRad1 $rad1 
    boundary conditions
      -1 -1 1 2 0 3 
    lines
      $nAxial = int( ($xb-$xa)/$ds +1.5 );
      $nTheta = int( 2.*$Pi*($innerRad1+$rad1)*.5/$ds +1.5 );
      $nTheta $nAxial $nr
    share
      0 0 1 2 0 3 
    exit
*
*  --- vertical pipe parallel to the z-axis ---
  Cylinder
    mappingName
      top-pipe
    orientation
      2 0 1
    bounds on the radial variable
      $innerRad2=$rad2-$dr; 
      $innerRad2 $rad2
    bounds on the axial variable
      $yac=$rad1-3.*$ds; $ybc=$yb; 
      $yac $ybc 
    boundary conditions
      -1 -1 0 4 0 3 
    lines
      $nAxial = int( ($ybc-$yac)/$ds +1.5 );
      $nTheta = int( 2.*$Pi*($innerRad2+$rad2)*.5/$ds +1.5 );
      $nTheta $nAxial $nr
    share
      0 0 0 4 0 3
    exit
*
* Here are the boxes that cover the interiors
*
Box
  mappingName
    main-pipe-box
  set corners
    $xa1=$xa; $xb1=$xb;
    $yb1 = $rad1; $ya1=-$yb1;
    $za1=$ya1; $zb1=$yb1; 
    $xa1 $xb1 $ya1 $yb1 $za1 $zb1
  lines
    $nx1 = int( ($xb1-$xa1)/$ds +1.5);
    $ny1 = int( ($yb1-$ya1)/$ds +1.5);
    $nz1 = int( ($zb1-$za1)/$ds +1.5);
    $nx1 $ny1 $nz1
  boundary conditions
    1 2  0 0  0 0 
  share
    1 2  0 0  0 0 
  exit  
*
Box
  mappingName
    top-pipe-box
  set corners
    $xb2 = $rad2; $xa2=-$xb2;
    $ya2=0.; $yb2=$ybc; 
    $za2=$xa2; $zb2=$xb2; 
    $xa2 $xb2 $ya2 $yb2 $za2 $zb2
  lines
    $nx2 = int( ($xb2-$xa2)/$ds +1.5);
    $ny2 = int( ($yb2-$ya2)/$ds +1.5);
    $nz2 = int( ($zb2-$za2)/$ds +1.5);
    $nx2 $ny2 $nz2
  boundary conditions
    0 0  0 4  0 0 
  share
    0 0  0 4  0 0 
  exit  
* 
 exit
* 
generate an overlapping grid
  main-pipe-box
  main-pipe
  * NOTE: put the top-pipe-box as higher priority than the main-pipe so that the top-pipe boundary
  *  points inside the the top-pipe are removed (rather than being interpolated)
  top-pipe-box
  top-pipe
  fillet
  done
  change parameters
    * The pipes should not cut holes the box grids of the opposite pipe
    prevent hole cutting
      top-pipe
      main-pipe-box
      * 
      main-pipe
      top-pipe-box
      done
*    manual hole cutting
*      main-pipe
*      9 13 35 45 3 3
*      done
    interpolation type
      $interpType
    exit
  compute overlap
*  open graphics
*  plot
*  pause
exit
*
save a grid (compressed)
$name
twoPipes
exit


  change parameters
    * OLD way using shared sides:
    * turn on hole cutting for a shared side
*   shared sides may cut holes
*   top-cylinder
*   main-cylinder
*   main-cylinder
*   top-cylinder
*   done
*
*    prevent hole cutting
*     cylinderFillet
*     all
*    done
*
*   NEW way using manual shared sides.
    specify shared boundaries
      cylinderFillet
        front  (side=0,axis=2)
      0 30 -1 5 0 0 
      main-cylinder
        front  (side=0,axis=2)
      normal matching angle
        30.
      done
      cylinderFillet
         front  (side=0,axis=2)
       0 30 6 12 0 0
       top-cylinder
         front  (side=0,axis=2)
       done
      main-cylinder
        front  (side=0,axis=2)
      0 30 0 20 0 0
      cylinderFillet
        front  (side=0,axis=2)
      done
      top-cylinder
        front  (side=0,axis=2)
      0 24 -1 14 0 0
      cylinderFillet
        front  (side=0,axis=2)
      done
      done
    ghost points
      all
      2 2 2 2 2 2
  exit
*
  change the plot
    toggle grid 0 0
    exit this menu
  x+r:0 30
  y+r:0
*  display intermediate results
  compute overlap
*  pause
exit
*
save an overlapping grid
filletTwoCyl.hdf
filletTwoCyl
exit
