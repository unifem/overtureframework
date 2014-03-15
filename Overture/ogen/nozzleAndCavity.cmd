#
#  Make grids for a nozzle with a cavity (geometry courtesy of EDF)
#  Usage:
#     ogen [-noplot] nozzleAndCavity -factor=<> -ng=<> -dw=<> -iw=<> interp=[e|i] -smoothCorner=[0|1]
#  Options:
#    factor = resolution factor, positive integer
#    ng, dw, iw : specify number of ghost points, discretization width and interpolation width
#    smoothCorner : if 1 then smooth the convex corner where the nozzle joins the cavity
#  
#    ogen -noplot nozzleAndCavity -factor=1
#    ogen -noplot nozzleAndCavity -factor=1 -smoothCorner=1   
#    ogen -noplot nozzleAndCavity -ng=6 -dw=5 -iw=4 -factor=2 [ok
#    ogen -noplot nozzleAndCavity -ng=6 -dw=7 -iw=4 -factor=2 [ok]
#    ogen -noplot nozzleAndCavity -ng=6 -dw=7 -iw=4 -factor=2 -interp=e [ok, safari]
#    ogen -noplot nozzleAndCavity -ng=6 -dw=7 -iw=4 -factor=2 -interp=e -smoothCorner=1 [ok, safari]
#
$order=2; $orderOfAccuracy = "second order"; $ng=2; 
$interp="i"; $interpType = "implicit for all grids"; $dse=0.; $ml=0;
$iw=-1; $dw=-1; $nrExtra=0; $factor=1; $smoothCorner=0; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"smoothCorner=i"=>\$smoothCorner,\
            "loadBalance=i"=>\$loadBalance,"ml=i"=>\$ml,"dw=i"=>\$dw,"iw=i"=>\$iw,"ng=i"=>\$ng);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
# nn = number of points to march in normal direction for most grids
$nn=5; 
if( $dw > 0 ){ $nn = $nn + int( $dw/2 ); }
$suffix = ".order.$order"; 
if( $iw gt 0 ){ $suffix = ".dw$dw.iw$iw"; }else{ $suffix = ".order$order"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $smoothCorner == 1 ){ $suffix .= ".sc"; }
$name = "nozzleAndCavity" . "$interp$factor" . $suffix . ".hdf";
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
# 
$ds=.01/$factor;
# 
create mappings
  # read the geometry from an IGES file:
  read iges file
    nozzleAndCavity.igs
    continue
    choose all
    # compute the topology and build a global triangulation
    CSUP:determine topology
    merge tolerance 0.001
    deltaS 0.01
    maximum area 1.e-4
    compute topology
    exit
    CSUP:refine plot
    refine surface 7
    refine surface 6
    refine surface 4
    refine surface 5
    exit
  builder
    target grid spacing $ds $ds (tang,norm)((<0 : use default)
# left nozzle:
    create surface grid...
      choose boundary curve 1
      done
      $ndist=.46;
      $nx = int($ndist/$ds+1.5);
      lines to march $nx
      generate
      name left_nozzle_surface
      exit
    create volume grid...
      marching options...
      BC: bottom fix x, float y and z
      BC: top fix x, float y and z
      backward
      lines to march $nn
      generate
      name left_nozzle
      Boundary Condition: bottom  3
      Boundary Condition: top     0
      Share Value: back    2
      Share Value: bottom  3
      exit
# left corner of the cavity where the nozzle joins the cavity
    create surface grid...
      DISPLAY WIRE FRAME ROTATION:0 0
      choose edge curve 3 -3.004469e-02 1.442169e-01 1.502356e-01 
      choose edge curve 2 -3.004469e-02 1.442169e-01 -1.502356e-01 
      done
      forward and backward
      lines to march $nn $nn (forward,backward)  
      generate
      # now (optionally) smooth the corner:
      smoothing...
      GSM:number of laplacian smooths 1
      GSM:number of elliptic smooths 1
      GSM:number of iterations 25
      GSM:relaxation coeff .1 (for elliptic smooths)
      GSM:project smoothed grid onto reference surface 0
      $cmd="#";
      if( $smoothCorner == 1 ){ $cmd="GSM:smooth grid"; }
      $cmd
      name left_corner_surface
      exit
#
    create volume grid...
      lines to march $nn
      generate
      Share Value: back    2
      name left_corner
      exit
# cavity grid (front)
    create surface grid... 
      picking:create boundary curve
      choose edge curve 1 -3.004469e-02 1.377747e-01 -2.099918e-01 
      choose edge curve 0 -3.004469e-02 1.460720e-01 2.103106e-01 
      done
      choose edge curve 7 3.002488e-02 1.377748e-01 2.099913e-01 
      choose edge curve 6 3.002488e-02 1.460720e-01 -2.103101e-01 
      done
      picking:choose initial curve
      choose edge curve 5 -6.016860e-03 3.603470e-01 0.000000e+00 
      done
      $rdist=.66+3*$ds;
      $nr = int($rdist/$ds+1.5);
      * lines to march $nr
      * we march backward a bit so we have enough overlap
      forward and backward
      lines to march $nr 5 (forward,backward)
#
      boundary offset 0, 0, 0 0 (l r b t)
      generate
      name nozzle_base_front_surface
      exit
#
    create volume grid...
      forward
      BC: left fix x, float y and z
      BC: right fix x, float y and z
      * depth of the cavity grid: 
      $depth=.03; 
      $lines = int($depth/$ds+1.5);
      lines to march $lines
      generate
      Share Value: left    2
      Share Value: right   2
      Boundary Condition: left    2
      Boundary Condition: right   2
      Boundary Condition: back    4
      Share Value: back    4
      name nozzle_base_front
      exit
#   cavity grid (back)
    create surface grid...
      picking:create boundary curve
      choose edge curve 1 -3.004469e-02 1.377747e-01 -2.099918e-01 
      choose edge curve 0 -3.004469e-02 1.460720e-01 2.103106e-01 
      done
      choose edge curve 6 3.002488e-02 1.460720e-01 -2.103101e-01 
      choose edge curve 7 3.002488e-02 1.377748e-01 2.099913e-01 
      done
      picking:choose initial curve
      choose edge curve 5 -6.016860e-03 3.603470e-01 0.000000e+00 
      done
      backward
      $rdist=.66+3*$ds;
      $nr = int($rdist/$ds+1.5);
      lines to march $nr
      boundary offset 0, 0, 0 0 (l r b t)
      generate
      name nozzle_base_back_surface
      exit
#
    create volume grid...
      marching options...
      BC: left fix x, float y and z
      BC: right fix x, float y and z
      lines to march $lines
      backward
      generate
      Boundary Condition: left    2
      Boundary Condition: right   2
      Share Value: left    2
      Share Value: right   2
      Boundary Condition: back    4
      Share Value: back    4
      name nozzle_base_back
      exit
# right corner where nozzle joins the cavity
    create surface grid...
      choose edge curve 8 3.002488e-02 1.442169e-01 1.502351e-01 
      choose edge curve 9 3.002488e-02 1.400239e-01 -1.500157e-01 
      done
      forward and backward
      lines to march $nn $nn (forward,backward)  
      generate
      # now (optionally) smooth the corner:
      smoothing...
      GSM:number of laplacian smooths 1
      GSM:number of elliptic smooths 1
      GSM:number of iterations 25
      GSM:relaxation coeff .1 (for elliptic smooths)
      GSM:project smoothed grid onto reference surface 0
      $cmd="#";
      if( $smoothCorner == 1 ){ $cmd="GSM:smooth grid"; }
      $cmd
      name right_corner_surface
      exit
    create volume grid...
      lines to march $nn 
      generate
      Share Value: back    2
      name right_corner
    exit
#
    create surface grid...
      choose boundary curve 0
      done
      forward
      $ndist=.46;
      $nx = int($ndist/$ds+1.5);
      lines to march $nx
      generate
      name right_nozzle_surface
      exit
#
    create volume grid...
      lines to march $nn
      Boundary Condition: bottom  3
      Share Value: back    2
      Share Value: bottom  3
      BC: bottom fix x, float y and z
      BC: top fix x, float y and z
      generate
      name right_nozzle
    exit
#
    build a box grid
      $ya = -.1+5*$ds; $yb=.4-5*$ds;
      set y min $ya
      set y max $yb
      $za = .25-$ds*4; 
      set z min -$za
      set z max  $za
      bc 3 3 0 0 0 0 (l r b t b f)
      share 3 3 0 0 0 0 (l r b t b f)
      name core
     exit 
    exit
  exit this menu
#
generate an overlapping grid
  core
  left_nozzle
  right_nozzle
  left_corner
  right_corner
  nozzle_base_back
  nozzle_base_front
  done choosing mappings
# 
  change parameters
    * improve quality of interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    $cmd="*";
    if( $dw > 0 ){ $cmd = "discretization width\n all\n $dw $dw $dw"; }
    $cmd
    $cmd="*";
    if( $iw > 0 ){ $cmd = "interpolation width\n all\n all\n $iw $iw $iw"; }
    $cmd
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  compute overlap
exit
# save an overlapping grid
save a grid (compressed)
$name
nozzleAndCavity
exit

