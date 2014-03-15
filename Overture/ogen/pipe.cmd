#
#   Grid for a cylindrical pipe
#
# usage: ogen [noplot] pipe -factor=<num> -order=[2/4/6/8] -interp=[e/i]  -rgd=[fixed|var] -name= ...
#                           -sa=<f> -sb=<f> -radius=<f> -axial=[x|y|z]
# 
#   [sa,sb] : bounds on the axial length of the pipe
#   radius  : radius of the pipe
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -axial : x,y or z indicates axial axis (default is "x")
# 
# examples:
#     ogen -noplot pipe -order=2 -interp=e -factor=1 
#     ogen -noplot pipe -order=2 -interp=e -factor=2 
#     ogen -noplot pipe -order=2 -interp=e -factor=4 
#     ogen -noplot pipe -order=2 -interp=e -factor=2 -sa=0. -sb=4. -name="pipe2eL4.hdf"
#     ogen -noplot pipe -order=4 -interp=e -factor=2 -sa=0. -sb=4. -name="pipe2eL4.order4.hdf"
#     ogen -noplot pipe -order=4 -interp=e -factor=3 -sa=0. -sb=4. -name="pipe3eL4.order4.hdf"
#
# -- Fixed width 
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -factor=1  
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -factor=2  
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -factor=4  
# 
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -factor=1  
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -factor=2  
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -factor=4  
# 
# -- Fixed width axial=y
#
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -axial=y -factor=1  
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -axial=y -factor=2  
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -axial=y -factor=4  
# 
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -axial=y -factor=1  
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -axial=y -factor=2  
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -axial=y -factor=4  
# 
# -- Fixed width axial=z
#
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -axial=z -factor=1  
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -axial=z -factor=2  
#    ogen -noplot pipe -order=2 -interp=e -rgd=fixed -axial=z -factor=4  
# 
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -axial=z -factor=1  
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -axial=z -factor=2  
#    ogen -noplot pipe -order=4 -interp=e -rgd=fixed -axial=z -factor=4  
# 
# -- set default parameter values:
$axial="x"; # axial axis
$sa=0.; $sb=2.; $radius=1.; $rgd="var"; 
$stretchFactor=2; # make BL grid spacing this many times finer
# 
$order=2; $factor=1; $interp="i"; $name="";
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
# 
# get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=>\$factor,"interp=s"=>\$interp,"sa=f"=>\$sa,"sb=f"=> \$sb,"radius=f"=>\$radius,\
           "name=s"=>\$name,"stretchFactor=f"=> \$stretchFactor,"rgd=s"=>\$rgd,"axial=s"=>\$axial );
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
$prefix="pipe"; 
if( $axial ne "x" ){ $prefix .= $axial; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf"; }
# 
# 
$ds = .1/$factor;
$pi=4.*atan2(1.,1.);
# 
create mappings
#
$width=$radius*min(.5,.75/$factor) + $ds*($order-2);  # width of cylindrial grid in the radial direction
if( $rgd eq "fixed" ){ $width=.5*$radius; }
#
 box
  $delta = $width - $ds*($order-2); 
  if( $axial eq "x" ){ $xa=$sa; $xb=$sb; $ya=-$radius+$delta; $yb=$radius-$delta; $za=$ya; $zb=$yb; $bc="1 2 0 0 0 0"; }
  if( $axial eq "y" ){ $ya=$sa; $yb=$sb; $za=-$radius+$delta; $zb=$radius-$delta; $xa=$za; $xb=$zb; $bc="0 0 1 2 0 0"; }
  if( $axial eq "z" ){ $za=$sa; $zb=$sb; $xa=-$radius+$delta; $xb=$radius-$delta; $ya=$xa; $yb=$xb; $bc="0 0 0 0 1 2";}
  set corners
    $xa $xb  $ya $yb  $za $zb
  lines
   $nx = int( ($xb-$xa)/$ds + 1.5 ); $ny = int( ($yb-$ya)/$ds + 1.5 ); $nz = int( ($zb-$za)/$ds + 1.5 );
   $nx $ny $nz
  boundary conditions
    $bc
  share 
    $bc
  mappingName
     box
# pause
  exit
# ----------------
  cylinder
    orientation
    #  orientation: 0,1,2 : axial direction is 2, cylinder is along the z-axis
    #               1,2,0 : axial direction is 0, cylinder is along the x-axis.
    #               2,0,1 : axial direction is 1, cylinder is along the y-axis.
    if( $axial eq "z" ){ $cmd="0,1,2"; }elsif( $axial eq "x" ){ $cmd="1 2 0"; }else{ $cmd="2 0 1"; }  
    $cmd
    centre for cylinder
     0. 0. 0.
    bounds on the axial variable
      $sa $sb
    bounds on the radial variable
      $rb=$radius;  $ra=$rb-$width;
      $ra $rb 
    boundary conditions
      -1 -1 1 2 0 3
    share 
       0  0 1 2 0 0
    lines
      $nTheta = int( 2.*$pi*($ra+$rb)/2./$ds + 1.5 );  $nr = int( 2*$width/$ds + 1.5 ); 
      $nTheta $nx  $nr 
  mappingName
     unstretched-cylinder
# pause
  exit
#
# cluster grid lines
# 
  stretch coordinates
    # STRT:multigrid levels 0
    Stretch r3:exp to linear
    $dsBL = $ds/$stretchFactor;
    STP:stretch r3 expl: position 1.
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    # OLD: 2013/09/30: 
    # Stretch r3:itanh
    #   $minGridSpacing = $ds/4.; 
    # STP:stretch r3 itanh: position and min dx 1 $minGridSpacing
    # stretch grid
    STRT:name cylinder
    exit
# 
#
#
exit
generate an overlapping grid
    box
    cylinder
  done
  change parameters
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
    exit
#   display intermediate results
# 
  compute overlap
#  pause
  exit
#
save an overlapping grid
$name
pipe
exit
