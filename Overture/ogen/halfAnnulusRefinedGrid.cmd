#
#  Test for the explicit hole cutting option
# 
#  ogen halfAnnulusRefinedGrid.cmd -factor=2   [requires hole cutter
#
#
$xa=-1.; $xb=1.; $ya=0.; $yb=1.; 
$interpType="implicit for all grids";
$order=2; $factor=2; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$suffix = ".order$order"; 
$name = "halfAnnulusRefinedGrid" . "$factor" . $suffix . ".hdf";
# 
# $factor=1.; $name="halfAnnulusRefinedGrid.hdf"; 
# $factor=1.; $name="halfAnnulusRefinedGrid1e.hdf"; $interpType="explicit for all grids";
# $factor=2.; $name="halfAnnulusRefinedGrid2.hdf"; 
# $factor=2.; $name="halfAnnulusRefinedGrid2e.hdf"; $interpType="explicit for all grids";
# $factor=4.; $name="halfAnnulusRefinedGrid4.hdf"; 
# $factor=4.; $name="halfAnnulusRefinedGrid4e.hdf"; $interpType="explicit for all grids";
# $factor=8.; $name="halfAnnulusRefinedGrid8e.hdf"; $interpType="explicit for all grids";
# $factor=16.; $name="halfAnnulusRefinedGrid16e.hdf"; $interpType="explicit for all grids";
#
$ds=1./20./$factor;
$pi=3.141592653;
# 
create mappings
  Annulus
    start and end angles
      $theta0=0.; $theta1=.5; 
      $theta0 $theta1
    $deltaR=.25/$factor; 
    $ra=.35; $rb=$ra+$deltaR; 
    inner radius
      $ra
    outer radius
      $rb
    boundary conditions
      3 3 5 0
    share
      3 3 0 0
    lines
      $nTheta=int( $pi*($ra+$rb)*($theta1-$theta0)/$ds+1.5 );
      $nr = int( ($rb-$ra)/$ds + 1.5 );
      $nTheta $nr 
    mappingName
      halfAnnulusRefinedGrid
    exit
#
  rectangle
    set corners
      $xa $xb $ya $yb
    lines
     $dsc=$ds*2.; # coarser grid 
     $nx = int( ($xb-$xa)/$dsc+1.5 );
     $ny = int( ($yb-$ya)/$dsc+1.5 );
     $nx $ny
    boundary conditions
      1 2 3 4 
    share
     0 0 3 0
    mappingName
      backGround
    exit
#  refinement grid 
  rectangle
    set corners
      $xar=-.65; $xbr=.65; $yar=0.; $ybr=.65; 
      $xar $xbr $yar $ybr
    lines
     $nx = int( ($xbr-$xar)/$ds+1.5 );
     $ny = int( ($ybr-$yar)/$ds+1.5 );
     $nx $ny
    boundary conditions
      0 0 3 0 
    share
     0 0 3 0
    mappingName
      refinement
    exit
# 
#  --- create an explicit hole cutter ---
  Annulus
    $r1=0.; $r2=$ra-$ds*.05; 
    inner radius
      $r1
    outer radius
      $r2
    boundary conditions
      -1 -1 1 1 
    lines
      31 7 
    mappingName
      annulusHoleCutter
    exit
#
exit this menu
#
  generate an overlapping grid
    backGround
    refinement
    halfAnnulusRefinedGrid
    done
    change parameters
      ghost points
        all
        2 2 2 2 2 2
    interpolation type
      $interpType
    # Define an explicit hole cutter
    create explicit hole cutter
      Hole cutter:annulusHoleCutter
      name: annulusHoleCutter
      # show parameters
      # prevent hole cutting
      #   halfAnnulusRefinedGrid
      # done
    exit
  exit
 open graphics

  DISPLAY AXES:0 0
  line width scale factor:0 4
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048

    compute overlap
    exit
#
save an overlapping grid
  $name
  halfAnnulusRefinedGrid
exit
