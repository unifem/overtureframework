#
# Usage:
#         ogen [noplot] deformingEllipseGrid [options]
# where options are
#     -factor=<num>       : grid spacing factor
#     -interp=[e/i]       : implicit or explicit interpolation
#     -name=<string>      : over-ride the default name  
#     -nExtra             : add extra lines in the normal direction on the boundary fitted grids
#     -width              : width of hyperbolic grid
#
# Examples:
#
#      ogen -noplot deformingEllipseGrid -interp=e -width=.25 -factor=2 
#      ogen -noplot deformingEllipseGrid -interp=e -width=.25 -factor=4 
#      ogen -noplot deformingEllipseGrid -interp=e -width=.25 -factor=8 
#      ogen -noplot deformingEllipseGrid -interp=e -width=.25 -factor=16
#
#
$factor=1; $name=""; 
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; 
#$xa=-3; $xb=3.0; $ya=-3; $yb=2; $nExtra=0; 
# $xa=-5; $xb=10.0; $ya=-5; $yb=5; 
$xa=-4; $xb=5.; $ya=-4; $yb=4; 
$nExtra=1; #wdh add extra lines in radial direction
$width=.25; 
# 
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,\
           "nExtra=i"=>\$nExtra,"width=f"=> \$width);
# 
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "deformingEllipseGrid" . "$interp$factor" . ".hdf";}
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
# ------------------------Start Ellipse 1 -------------------------------
# -----------------------------------------------------------------------
  $bcInterface=100;  # bc for interfaces
  $shareInterface=100;        # share value for interfaces
  $xc=-1; $yc=0; # center of the ellipse
  $phi = -45.0;
  $rada = 1.25;
  $radb = .5;
  include $ENV{CG}/mp/runs/shockEllipse/deformingEllipseGrid.h
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
    done
   specify a domain
    innerDomain1
      innerSquare1
      innerInterface1
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
#    plot
#    pause
  compute overlap
    plot
#    pause
#
exit
#
save an overlapping grid
  $name
  dE
exit
