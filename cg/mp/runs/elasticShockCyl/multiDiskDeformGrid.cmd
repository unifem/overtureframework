#
# Create the initial grid for a deforming disk. 
# Use this grid with cgmp for a fluid-structure example.
#
# Usage:
#         ogen [-noplot] multiDiskDeformGrid [options]
# where options are
#     -factor=<num>     : grid spacing is .1 divided by this factor
#     -interp=[e/i]     : implicit or explicit interpolation
#     -name=<string>    : over-ride the default name  
#     -case=[inner|outer] : only build a grid for the inner or outer domain
#     -nExtra          : add extra lines in the normal direction on the boundary fitted grids
#
# Examples:
#
#   ogen -noplot multiDiskDeformGrid -factor=2 -width=.4
#   ogen -noplot multiDiskDeformGrid -interp=e -factor=4
#   ogen -noplot multiDiskDeformGrid -interp=e -factor=8
#   ogen -noplot multiDiskDeformGrid -interp=e -factor=16
#
#
$prefix="multiDiskDeformGrid"; 
$factor=1; $name=""; $case=""; 
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; 
$xa=-3.0; $xb=3.0; $ya=-3.0; $yb=3.0; $nExtra=0; 
$width=.25; 
# 
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"case=s"=> \$case,\
           "xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,"nExtra=i"=>\$nExtra,"width=f"=> \$width);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "$prefix$case" . "$interp$factor" . ".hdf";}
if( $factor<1. ){ $width = .25*$factor; }
#
#
$Pi=4.*atan2(1.,1.);
#
$ds0 = .1; 
# target grid spacing:
$ds = $ds0/$factor;
#
# 
create mappings
#
  rectangle
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    mappingName
      outerSquare
    exit
#
$diskNumber=1;
$diskNames ="#"; 
# -----------------------------------------------------------------------
# ------------------------Start Disk 1 ----------------------------------
# -----------------------------------------------------------------------
  $bcInterface=100;  # bc for interfaces
  $shareInterface=100;        # share value for interfaces
  $xc=-.25; $yc=-1.; # center of the disk 
  $rad=1.; 
# ---------------  
  rectangle
   # make the inner square bigger for deforming grid problems
    $expand=.5; 
    $xa = $xc - $rad - $expand; 
    $xb = $xc + $rad + $expand*2.;
    $ya = $yc - $rad - $expand; 
    $yb = $yc + $rad + $expand;
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      0 0 0 0 
    mappingName
      $mapName = "innerSquare$diskNumber";  $diskNames .= "\n $mapName"; 
      $mapName
    exit
#
# Create a start curve for the interface
#
    $innerRadius=$rad; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.;
# 
  spline
    $n=51;
    enter spline points
      $n 
    $commands="";
    for( $i=0; $i<$n; $i++ ){ $theta=2.*$Pi*$i/($n-1.); $x0=$xc + $rad*cos($theta); $y0=$yc + $rad*sin($theta); \
                              $commands = $commands . "$x0 $y0\n"; }
    $commands
    lines
      $nr = int( 2.*$Pi*$rad/$ds+1.5 );
      $nr
    periodicity
      2
 # pause
    exit
#  
   $width = $width + $ds0*$nExtra;
# 
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 );  if( $ns<3 ){ $ns=3; }
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface
    Share Value: bottom $shareInterface
    close mapping dialog
    $mapName = "outerInterface$diskNumber";  $diskNames .= "\n $mapName"; 
    name $mapName
 # pause
    exit
#  
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 ); if( $ns<3 ){ $ns=3; }
    backward
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface
    Share Value: bottom $shareInterface
    close mapping dialog
    $mapName = "innerInterface$diskNumber";  $diskNames .= "\n $mapName"; 
    name $mapName
    exit
#
# -----------------------------------------------------------------------
# ------------------------Start Disk 2 ----------------------------------
# -----------------------------------------------------------------------
  $diskNumber+=1; 
  $bcInterface+=1;  # bc for interfaces
  $shareInterface+=1;        # share value for interfaces
  $xc=.5; $yc=1.25; # center of the disk 
  $rad=.8; 
  $width=.25*$rad; if( $factor<1. ){ $width = .25*$factor; }
# ---------------  
  rectangle
    # make the inner square bigger for deforming grid problems
    $expand=.5; 
    $xa = $xc - $rad - $expand; 
    $xb = $xc + $rad + $expand*2.;
    $ya = $yc - $rad - $expand; 
    $yb = $yc + $rad + $expand;
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      0 0 0 0 
    mappingName
      $mapName = "innerSquare$diskNumber";  $diskNames .= "\n $mapName"; 
      $mapName
    exit
#
# Create a start curve for the interface
#
    $innerRadius=$rad; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.;
# 
  spline
    $n=51;
    enter spline points
      $n 
    $commands="";
    for( $i=0; $i<$n; $i++ ){ $theta=2.*$Pi*$i/($n-1.); $x0=$xc + $rad*cos($theta); $y0=$yc + $rad*sin($theta); \
                              $commands = $commands . "$x0 $y0\n"; }
    $commands
    lines
      $nr = int( 2.*$Pi*$rad/$ds+1.5 );
      $nr
    periodicity
      2
 # pause
    exit
#  
   $width = $width + $ds0*$nExtra;
# 
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 );  if( $ns<3 ){ $ns=3; }
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface
    Share Value: bottom $shareInterface
    close mapping dialog
    $mapName = "outerInterface$diskNumber";  $diskNames .= "\n $mapName"; 
    name $mapName
 # pause
    exit
#  
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $dist = $width/$factor;     
    $ns = int( $width/$ds0 +1.5 ); if( $ns<3 ){ $ns=3; }
    backward
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    generate
    mapping parameters
    Boundary Condition: bottom  $bcInterface
    Share Value: bottom $shareInterface
    close mapping dialog
    $mapName = "innerInterface$diskNumber";  $diskNames .= "\n $mapName"; 
    name $mapName
    exit
#
#
  exit this menu
#
generate an overlapping grid
  outerSquare
  $diskNames
  done choosing mappings
# 
  change parameters 
 # define the domains -- these will behave like independent overlapping grids
   specify a domain
    outerDomain
     outerSquare
     outerInterface1
     outerInterface2
    done
   specify a domain
    innerDomain1
      innerSquare1
      innerInterface1
    done
   specify a domain
    innerDomain2
      innerSquare2
      innerInterface2
    done
# 
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit 
#
  # open graphics
  #  plot
  compute overlap
#
exit
#
save an overlapping grid
  $name
  multiDiskDeform
exit
