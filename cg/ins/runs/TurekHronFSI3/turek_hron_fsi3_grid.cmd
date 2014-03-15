#
# Create the initial grid for the FSI test case from the Turek & Hron benchmark paper (case 2)
# Use this grid with cgins.
#
# Usage:
#         ogen [noplot] turek_hron_fsi2_grid [options]
# where options are
#     -factor=<num>          : grid spacing factor
#     -interp=[e/i]          : implicit or explicit interpolation
#     -name=<string>         : over-ride the default name
#     -sharpness=<num>       : sharpness parameter
#
# Examples:
#
#      ogen noplot turek_hron_fsi2_grid -interp=e -factor=4
#
#
$factor=1; $name="";
$interp="i"; $interpType = "implicit for all grids";
$order=2; $orderOfAccuracy = "second order"; $ng=2;
$sharpness=40;
$sharpness4=$sharpness*4;
$sharp=40;
$channelAspectRatio=4.0;
$beamX=0.05;
$beamThickness=0.02;
$beamLength=0.35;
#
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,\
           "sharpness=f"=> \$sharpness);
#
$ds0=0.02;
$ds=$ds0/$factor;
$nl=int((($beamThickness+0.01)+($beamLength*2+0.02))/$ds);
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "turek_hron_fsi3" . "$interp$factor" . ".hdf";}
$L=2.5;
$H=0.41;
$blf=2.0;
$nl=int((($beamThickness+0.01)+($beamLength*2+0.02))/$ds);
create mappings 
  rectangle 
    $nx=int( ($L/2.0000000)/$ds+1.5 );
    $ny=int( ($H)/$ds+1.5 );
    $ya=0.0;
    $yb=$H;
    $xb=$L/1.99;
    set corners 
    0.0,$xb,0.0,$yb
    lines 
    $nx,$ny 
    share 
    3,0,2,5
    boundary conditions 
    3,0,2,2 
    exit 
  exit this menu 
create mappings 
  rectangle 
    $nx=int( $L/$ds/2.0000000/2.0+1.5 );
    $ny=int( ($H)/$ds/2.0+1.5 );
    $ya=0.0;
    $yb=$H;
    $xb=$L/1.99;
    $xa=$L/2.01;
    $xb=$L;
    set corners 
    $xa,$xb,$ya,$yb
    lines 
    $nx,$ny 
    share 
    0,4,2,5 
    boundary conditions 
    0,4,2,2 
    mappingName
    rectangle2
    exit 
  exit this menu 
create mappings
  annulus
   $ro=0.05+$ds*7;
   $npts=int( 2.0*3.14159*$ro/$ds+1.5);
   inner radius
    0.05
    outer radius
    $ro
    lines
    $npts,7
    share 
    6,6,1,0
    boundary conditions 
    -1,-1,1,0 
    mappingName
    unstretched-cylinder
    centre for annulus
    0.2 0.2
    exit
  stretch coordinates
    Stretch r2:exp to linear
    #STRT:multigrid levels $ml
    $dsMin=$ds/$blf; # normal grid spacing on the cylinder
    STP:stretch r2 expl: min dx, max dx $dsMin $ds
    STRT:name cylinder 
    stretch grid
    share 
    6,6,1,0
    boundary conditions 
    -1,-1,1,0 
  exit
  nurbs (curve)
    enter control points
    2
    9
    0.25
    0.25
    0.5
    0.5
    0.75
    0.75
    0.25 0.2 1
    0.25 0.25 0.7071
    0.2 0.25 1
    0.15 0.25 0.7071
    0.15 0.2 1
    0.15 0.15 0.7071
    0.2 0.15 1
    0.25 0.15 0.7071
    0.25 0.2 1
    mappingName
    cylinder_boundary
    exit
  exit this menu
create mappings
  smoothedPolygon
    $locxa=sqrt(0.05*0.05-(0.01)*0.01)+0.2;
    $locxaa=sqrt(0.05*0.05-(0.01)*0.01)+0.2+0.05;
    $locya=0.01+0.2;
    $locxb=sqrt(0.05*0.05-(0.01)*0.01)+0.2+0.35;
    $locyb=-0.01+0.2;
    $nx=$nl;
#    $ny=int(7);
#    $ndist=($ny-1)*$ds;
    # wdh:
    $ny=int(9);
    $ndist=($ny-3)*$ds;
    vertices
    6
    $locxa,$locyb
    $locxaa,$locyb
    $locxb,$locyb
    $locxb,$locya
    $locxaa,$locya
    $locxa,$locya
    curve or area (toggle)
    lines 
    $nl
    sharpness
      $sharpness4
      $sharpness4
      $sharpness4
      $sharpness4
      $sharpness4
      $sharpness4
    t-stretch
      0.1,30
      0.1,10
      0.1,10
      0.1,10
      0.1,10
      0.1,30
*    n-stretch
*      1 5 0
    boundary conditions
    1,1,7,0 
    share 
    1,1,7,0 
    exit 
  hyperbolic
    $stretchFactor=1.25;
    forward
    distance to march $ndist
    lines to march $ny
    points on initial curve $nx
    # wdh: 
    geometric stretch factor 1.0
    generate 
    mapping parameters
    Boundary Condition: left 1
    Boundary Condition: right 1
    Boundary Condition: bottom 7
    Boundary Condition: top 0
    Share Value: left 1
    Share Value: right 1
    Share Value: bottom 7
    Share Value: top 0
    close mapping dialog
    boundary condition options...    
    BC: left match to a mapping
    cylinder_boundary   
    BC: right match to a mapping
    cylinder_boundary
    normal blending 10,10 (lines: left, right)
    close marching options
    generate 
    name hyp-smoothedPolygon
    exit
  exit this menu
#create mappings
generate an overlapping grid
  square
  rectangle2
  cylinder
  hyp-smoothedPolygon
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
turek_hron_fsi3
exit
