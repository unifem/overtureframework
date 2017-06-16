# Grid for an artificial heart valve
#   -xa, -xb, -ya, -yb, -za, -zb : bounding box
# 
# Usage:
#    ogen [-noplot] heartValveGrid.cmd -factor=<not support yet> -interp=[e|i] 
#    -theta1=<> -rotationOffset=<> -order=<> -ml=<>  -pause=<0|1> -box=<0|1>
#    -leaflets=<1|2>
#
# Options:
#   -xa, -xb, -ya, -yb, -za, -zb : bounding box
#   -theta1: rotated angle
#   -box : in a box chammber
#   -pause : pause at various places
#   -leaflets : =1 for one leaflet case
#
# Examples:
#
#  ogen -noplot heartValveGrid.cmd -interp=i -factor=1 [OK 
#  ogen -noplot heartValveGrid.cmd -interp=i -theta1=-20 -factor=1 [OK 
#  ogen -noplot heartValveGrid.cmd -interp=i -theta1=-20 -factor=2 [OK 
#  ogen -noplot heartValveGrid.cmd -interp=i -theta1=-20 -factor=4 [OK 
#  ogen -noplot heartValveGrid.cmd -interp=i -theta1=-20 -factor=1 -leaflets=1 [OK 
#
#
$prefix="heartValveGrid"; 
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; 
$theta1=0.; 
$pause="#";
$xa=-.5; $xb=1.25; # axial bounds of pipe along x-axis (default values are consistent with 2d case)
$radius=1.1;
$rotationOffset=.15; # rotation point is offset this amount from the origin: (0,$rotationOffset,0)
$leaflets=2;
$box=0;
$distInner="";
$ya="";
$yb="";
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"blf=f"=>\$blf,"refinementBox=i"=>\$refinementBox,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"prefix=s"=> \$prefix,\
            "widthX=f"=> \$widthX,"widthY=f"=> \$widthY,"widthZ=f"=> \$widthZ,\
            "rotationOffset=f"=> \$rotationOffset,"box=i"=> \$box,"leaflets=i"=> \$leaflets,\
            "theta1=f"=>\$theta1,"pause=i"=>\$pause,"radius=f"=>\$radius,"distInner=f"=>\$distInner,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,\
            "xac=f"=>\$xac,"xbc=f"=>\$xbc,"yac=f"=>\$yac,"ybc=f"=>\$ybc,"zac=f"=>\$zac,"zbc=f"=>\$zbc );
# 
if( $pause eq 1 ){ $pause="pause"; }\
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if ($leaflets eq 1){ $prefix="oneLeaflet";}
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
#
$ds=.025/$factor;   # target grid spacing 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$pi = 4.*atan2(1.,1.);
#
#note there is no mapping/rescaling; the size of the leaflet comes from igs info.
create mappings
  read from a file...
  read iges file
    heartValve.igs
    continue
    choose all
# what is CSUP: CSUP make sure the command is in the right location
    CSUP:determine topology
    deltaS 0.005
    maximum area 0.000025
    compute topology
    exit
    CSUP:mappingName heartValveSurface
    exit
$pause
#the above command can be first determined by playing in the windows
#the above command got the topology of the whole surface
  builder...
    reset:0
    set view:0 0 0 0 1 0.908784 0.210478 -0.360293 -0.408499 0.624857 -0.665344 0.0850911 0.751833 0.65384
    target grid spacing $ds, $ds (tang,norm)(<0 : use default)
#
#   Grid on top surface
#
    create surface grid...
      choose edge curve 7 9.000000e-02 2.485138e-03 5.000000e-02
    done
    Start curve parameter bounds .025 .975 #choose it is slightly smaller
    backward
    equidistribution 0.1 (in [0,1])
      BC: left (forward) outward splay
      BC: right (forward) outward splay
      BC: left (backward) outward splay
      BC: right (backward) outward splay
      normal blending 11 11(lines: left, right)
      #outward splay -.1 -.1(left, right for outward splay BC)
      outward splay -.05 -.05(left, right for outward splay BC)
    uniform dissipation 0.5
    $lines=33*$factor;
    lines to march $lines
    $pause
    generate
    name heartValveSurfaceTop
  exit
  $pause
#
#   Grid on bottom surface
#
    create surface grid...
      choose edge curve 10 9.000000e-02 -2.532066e-03 -5.000000e-02 
    done
    Start curve parameter bounds .025 .975
    backward
    equidistribution 0.1 (in [0,1])
      BC: left (forward) outward splay
      BC: right (forward) outward splay
      BC: left (backward) outward splay
      BC: right (backward) outward splay
      normal blending 11 11(lines: left, right)
      outward splay -.05 -.05(left, right for outward splay BC)
    uniform dissipation 0.5
    lines to march $lines 
    generate
    name heartValveSurfaceBottom
  exit
#
#  Edge grid
#
    $dsEdge=$ds/1.5;
    create surface grid... 
      target grid spacing $dsEdge $dsEdge (tang,norm)(<0 : use default) 
      choose edge curve 14 5.000000e-02 1.110223e-15 1.000000e-02 
      choose edge curve 21 6.001588e-02 -9.885207e-01 1.000000e-02 
      choose edge curve 0 7.427406e-01 -6.695792e-01 1.000000e-02 
      choose edge curve 5 7.427406e-01 6.695792e-01 1.000000e-02 
      choose edge curve 24 6.001588e-02 9.885207e-01 1.000000e-02 
      done 
      $pause
      forward and backward 
      #lines to march 28, 33 (forward,backward)  
      $lines=10*$factor; $linesb=12*$factor;
      lines to march $lines, $linesb (forward,backward)  
      equidistribution 0.05 (in [0,1])
      volume smooths 200
      generate
      name heartValveEdgeSurface
      #lines 999 40
      #$pause
      #if($factor eq 1){$cmds="lines 251 18";}elsif($factor eq 2){$cmds="lines 502 36";}
      #$cmds
      $pause
   exit
#
#  Edge Volume grid
#
    create volume grid...
      backward
      target grid spacing -1, $dsEdge (tang,normal, <0 : use default)
      spacing: geometric
      geometric stretch factor 1.05
      lines to march 8
      generate
      name heartValveEdge
    exit
#
#  Top face volume grid
#
    active grid:heartValveSurfaceTop
    create volume grid...
      forward
      #here just let the grid size consistent with its surface grid size
      target grid spacing -1, $dsEdge (tang,normal, <0 : use default) 
      spacing: geometric
      geometric stretch factor 1.05
      lines to march 8
      generate
      name heartValveTop
    exit
#
#  Bottom face volume grid
#
    active grid:heartValveSurfaceBottom
    create volume grid...
      forward
      target grid spacing -1, $dsEdge (tang,normal, <0 : use default)
      spacing: geometric
      geometric stretch factor 1.05
      lines to march 8
      generate
      name heartValveBottom
    exit    
# assign BC and share 
    assign BC and share values 
      boundary condition: 1 
      shared boundary flag: 1 
      plot lines on non-physical boundaries 0
      #set BC order: grid side axis bc share
      set BC and share 1 0 2 1 1
      set view:0 0 0 0 1 0.939025 0.0169536 -0.343431 -0.340449 0.185951 -0.921693 0.0482351 0.982413 0.180384
      set BC and share 0 0 2 1 1
      set view:0 0 0 0 1 0.929785 -0.176161 -0.323213 -0.366138 -0.352001 -0.861417 0.0379769 0.919273 -0.391785
      set BC and share 2 0 2 1 1
$pause
    exit
#
  exit
#
 reset:0
#
#  Pipe Wall
#
$outerRadius=$radius;  # make outer radius a bit larger than vale radius=1
$nr=8;  # number of radial grid points 
$innerRadius = $outerRadius - ($nr-1)*$dsEdge; 
  cylinder
    centre for cylinder
      0 0 0
    orientation
      1 2 0
    bounds on the radial variable
      $innerRadius $outerRadius
    bounds on the axial variable
      $xa $xb
    lines
      $nTheta = intmg( .5*($innerRadius+$outerRadius)*2.*$pi/$dsEdge + 1.5 );
      $nAxial = intmg( ($xb-$xa)/$dsEdge + 1.5 );
      $nTheta $nAxial $nr 
    boundary conditions
      -1 -1 4 5 0 3
    share
       0 0 4 5 0 0
    mappingName
      pipe
$pause
 exit
#
# core Cartesian grid for the interior of the pipe
# 
  box
    set corners
      if($distInner eq ""){$distInner=$innerRadius+$ds;}
      $xac=$xa; $xbc=$xb; 
      $zac=-$distInner; $zbc=$distInner; 
      if($ya eq ""){$yac=$zac;}else{$yac=$ya;}
      if($yb eq ""){$ybc=$zbc;}else{$ybc=$yb;}
      $xac $xbc $yac $ybc $zac $zbc
    lines
      $nx = intmg( ($xbc-$xac)/$ds + 1.5 );
      $ny = intmg( ($ybc-$yac)/$ds + 1.5 );
      $nz = intmg( ($zbc-$zac)/$ds + 1.5 );
      $nx $ny $nz
    boundary conditions
      $cmds="4 5 0 0 0 0";
      if ($box eq 1){$cmds="4 5 6 7 8 9";}
      $cmds
      $cmds="share \n 4 5 0 0 0 0";
      if ($box eq 1){$cmds="#";}
      $cmds
    mappingName
      core
$pause
    exit
#
# Subroutine to convert to Nurbs Mapping and rotate/shift
#
#   NOTE: Always first rotate about y and x axes to make channel axis the x-axis
#      1. rotate 90 degrees about y axis
#      2. rotate $angleXaxis about the x-axis
#
#  ROTATE about the point (x0,y0,z0)
#
$numGhost=$ng;
sub convertToNurbs\
{ local($old,$new,$angle,$rotationAxis,$x0,$y0,$z0,$angleXaxis,$share)=@_; \
$cmds = "nurbs \n" . \
"interpolate from mapping with options\n" . \
" $old \n" . \
" parameterize by index (uniform)\n" . \
" number of ghost points to include\n $numGhost\n" . \
" choose degree\n" . \
"  3 \n" . \
" # number of points to interpolate\n" . \
" #  11 21 5 \n" . \
"done\n" . \
"# First rotate about y and x axes to make channel axis the x-axis \n" . \
"rotate \n" . \
" 90 1 \n" . \
" 0. 0. 0.\n" . \
"rotate \n" . \
" $angleXaxis 0 \n" . \
" 0. 0. 0.\n" . \
"# user defined additional rotation: \n" . \
"rotate \n" . \
" $angle $rotationAxis \n" . \
" $x0 $y0 $z0\n" . \
"share \n" . \
"  0 0 0 0 $share 0 \n" . \
"mappingName\n" . \
" $new\n" . \
"exit"; \
}
#
# ------ ROTATE VALVE GRIDS ---------
#
#    Valves rotate about the z-axis 
#
# -------- UPPER VALVE ------
#$theta1=-50.; $theta2=-$theta1; # rotate valves by this many degrees
$theta2=-$theta1; # rotate valves by this many degrees
# Assume rotation axis for each valve is offset: 
#     rotation offset = half gap + half width = .05 + .05
$angle=$theta1; $rotationAxis=2; $x0=0.; $y0=$rotationOffset; $z0=0.; $angleXaxis=90; $share=1; 
convertToNurbs(heartValveTop,heartValveTopUpper,$angle,$rotationAxis,$x0,$y0,$z0,$angleXaxis,$share);
$cmds
convertToNurbs(heartValveBottom,heartValveBottomUpper,$angle,$rotationAxis,$x0,$y0,$z0,$angleXaxis,$share);
$cmds
convertToNurbs(heartValveEdge,heartValveEdgeUpper,$angle,$rotationAxis,$x0,$y0,$z0,$angleXaxis,$share);
$cmds
# -------- LOWER VALVE ------
$angle=$theta2; $rotationAxis=2; $x0=0.; $y0=-$rotationOffset; $z0=0.; $angleXaxis=-90; $share=2; 
if ($leaflets eq 2){convertToNurbs(heartValveTop,heartValveTopLower,$angle,$rotationAxis,$x0,$y0,$z0,$angleXaxis,$share);}\
else{$cmds="#";}
$cmds
if ($leaflets eq 2){convertToNurbs(heartValveBottom,heartValveBottomLower,$angle,$rotationAxis,$x0,$y0,$z0,$angleXaxis,$share);}
$cmds
if ($leaflets eq 2){convertToNurbs(heartValveEdge,heartValveEdgeLower,$angle,$rotationAxis,$x0,$y0,$z0,$angleXaxis,$share);}
$cmds
#
exit
#
# the heartValveTop are facing each other in the grid
# wrong order of overlapping grid may lead to failure of grid generator
#
# grids from top to bottom: BottomUpper, TopUpper, TopLower, BottomLower
#
generate an overlapping grid
core
$cmds="pipe"; if ($box eq 1){ $cmds="#";}
$cmds
#
heartValveBottomUpper
$cmds="heartValveBottomLower"; if ($leaflets eq 1){ $cmds="#";}
$cmds
#
heartValveTopUpper
$cmds="heartValveTopLower"; if ($leaflets eq 1){ $cmds="#";}
$cmds
#
heartValveEdgeUpper
$cmds="heartValveEdgeLower"; if ($leaflets eq 1){ $cmds="#";}
$cmds
#
done choosing mappings
# 
change the plot
toggle boundary 1 2 1 0
toggle boundary 1 0 0 0
toggle boundary 1 1 1 0
toggle boundary 0 0 0 0
toggle boundary 0 0 1 0
toggle boundary 0 1 1 0
exit
change parameters
# choose implicit or explicit interpolation
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
#
# save an overlapping grid
save a grid (compressed)
$name
heartValveGrid
exit

