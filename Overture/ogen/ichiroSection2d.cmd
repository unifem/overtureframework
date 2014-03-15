*
* Build a 2D grid for a section of the ILC super-conducting linear collider
*
$orderOfAccuracy = "second order";
$interpolation="explicit";
* $interpolation="implicit";
$nDist=.5; 
*
$factor=1; $name = "ichiroSection2d1.hdf"; 
* $factor=2; $name = "ichiroSection2d2.hdf"; 
* $factor=2; $name = "ichiroSection2d2e.hdf"; 
* $factor=4; $name = "ichiroSection2d4.hdf"; 
* -- fourth-order accurate ---
* $factor=2; $name = "ichiroSection2d2.order4.hdf";  $orderOfAccuracy = "fourth order"; $nDist=.85;
* $factor=4; $name = "ichiroSection2d4.order4.hdf";  $orderOfAccuracy = "fourth order"; $nDist=.85;
*
$ds=.15/$factor;
$pi=3.141592653; 
*
create mappings
* 
  read iges file 
    /home/henshaw/Overture/mapping/ichiroPart4.igs 
    continue 
    choose a list 
      * 131 135 134 137 136 
      * 131 135  134 
      131 135 134 137 136 
      * 135 
      done 
    exit 
  builder 
    $xa=1.e-15; 
    build curve on surface 
      plane point 1 $xa 0.0     38 
      plane point 2 $xa 4.66416 38 
      plane point 3 $xa 0.0     47.3799 
      * 
      cut with plane
      set view:0 0 0 0 1 -0.00829787 -0.144883 0.989414 0.0311697 0.98893 0.145074 -0.99948 0.0320435 -0.00369004
      add last curve to mapping list
      exit
    exit
  clear all:0
  change a mapping
  nurbsMapping
    mappingName
      crossSection
     reset:0
    project to 2d
      0 0 1 0 1 0
    restrict the domain
      .195 .805
    lines
      81
* pause
    exit
* 
  stretch coordinates
    Stretch r1:itanh
    STP:stretch r1 itanh: layer 0 .5 5 0.5 (id>=0,weight,exponent,position)
    stretch grid
    close r1 stretching parameters
    mappingName
     crossSection-stretched
    exit
*
  mapping from normals
    extend normals from which mapping?
      crossSection-stretched
    boundary conditions
      1 2 3 0
    share
      1 2 0 0
    $dist=$nDist/$factor; 
    normal distance
      $dist
    lines
      $length=7.; # approx arclength in axial direction
      $nx=int( $length/$ds+1.5 );
      $ny=int( $dist/$ds+1.5 );
      * 51 81 7 
      $nx $ny 
    mappingName
      outerShellNormalMapping
*pause
    exit
*
* convert to a nurbs for faster evaluation
  nurbs (surface)
    interpolate from mapping with options
      outerShellNormalMapping
    parameterize by index (uniform)
    done
    mappingName
      outerShell
   exit
*
*
  rectangle
      $xa=40.1006; $xb=44.6430; $ya=0.; $yb=3.8; 
    set corners
      $xa $xb $ya $yb
      $nx = int( ($xb-$xa)/$ds+1.5 );
      $ny = int( ($yb-$ya)/$ds+1.5 );
    lines
      $nx $ny
    boundary conditions
      1 2 5 0
    share
      1 2 0 0
    mappingName
     coreBox
  exit
* 
  exit this menu
generate an overlapping grid
  coreBox
  outerShell
  done choosing mappings
  change parameters
    ghost points
      all
      2 2 2 2 2 2
* 
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType = "$interpolation for all grids";
      $interpType
  exit
* 
*   display intermediate results
*pause
* 
  compute overlap
*pause
  exit
* 
save a grid (compressed)
$name
ichiro
exit