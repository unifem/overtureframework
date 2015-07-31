# ========================================================================================
#
#  A lattice of circular regions used for a photonic band gap simulation
#
# usage: ogen [noplot] latticeGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -name=<>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the annulus
#
# Examples:
# 
#   ogen -noplot latticeGrid -order=2 -interp=e -nCylx=2 -nCyl=2 -factor=2 
#   ogen -noplot latticeGrid -order=2 -interp=e -nCylx=2 -nCyl=2 -factor=4
#
# -- fourth order
#   ogen -noplot latticeGrid -order=4 -interp=e -nCylx=2 -nCyly=2 -factor=2
# 
# -- 3 x 3 lattice
#   ogen -noplot latticeGrid -order=4 -interp=e -nCylx=3 -nCyly=3 -factor=2
#
# ========================================================================================
# 
# Default values for the parameters
$prefix="latticeGrid"; 
$orderOfAccuracy = "second order";
$nCylx=2; $nCyly=2;   # number of cylinders in each direction
$bc = "1 2 -1 -1";
# 
#
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"nCylx=i"=>\$nCylx,"nCyly=i"=>\$nCyly, "prefix=s"=> \$prefix );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$nCylx\x$nCyly\y" "$interp$factor" . $suffix . ".hdf";}
#
# -- back-ground grid:
$rxa=-($nCylx/2.+1.); $rxb=-$rxa;
# $rya=-($nCyly/2.+1.); $ryb=-$rya;
# Make the lower and upper boundaries consistent with a periodic array
$rya=-($nCyly/2.); $ryb=-$rya;
#
# Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
}
#
#**************************************************************************
#
#
# domain parameters:  
$ds = .05/$factor; # target grid spacing
$pi=3.141592653;
#
#
create mappings 
#
$count=0;  # number the disks as 1,2,3...
#
# ======================================================
# Define a function to build an inner-annulus, outer-annulus and inner square.
# usage:
#   makeDisk(radius,xCenter,yCenter)
# =====================================================
sub makeDisk\
{ local($radius,$xc,$yc)=@_; \
  $count = $count + 1; \
  $innerAnnulus="innerAnnulus$count";     \
  $innerSquare="innerSquare$count";     \
  $outerAnnulus="outerAnnulus$count";     \
  $outerSquare="outerSquare$count";     \
  $mappingNames = $mappingNames . "   $outerAnnulus\n" . "   $innerSquare\n". "   $innerAnnulus\n"; \
  $allowHoleCutting = $allowHoleCutting . "   $outerAnnulus\n". "   backGround\n" . "   $innerAnnulus\n". "   $innerSquare\n"; \
  $allowInterpolation = $allowInterpolation . " $innerAnnulus\n" . " $innerSquare\n" . " $innerSquare\n" . " $innerAnnulus\n" . " $outerAnnulus\n" . " backGround\n" . " backGround\n" . " $outerAnnulus\n"; \
  $deltaRadius=$radius*.75/$factor; \
  $outerRadius=$radius; $innerRadius=$outerRadius-$deltaRadius; \
  $nx=int( (2.*$pi*$outerRadius)/$ds+1.5 ); \
  $ny=int( ($deltaRadius)/$ds+1.5 ); \
  $nTheta=$nx; \
  $share=100+$count; \
  $commands = \
  "  annulus \n" . \
  "    mappingName \n" . \
  "      $innerAnnulus \n" . \
  "    centre\n" . \
  "     $xc $yc\n" .   \
  "    boundary conditions \n" . \
  "      -1 -1 0 1 \n" . \
  "    share\n" . \
  "     * material interfaces are marked by share>=100\n" . \
  "      0 0 0 $share \n" . \
  "    inner and outer radii \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    lines \n" . \
  "      $nx $ny \n" . \
  "    exit \n"; \
  $xa=$xc-$innerRadius;  $xb=$xc+$innerRadius; \
  $ya=$yc-$innerRadius;  $yb=$yc+$innerRadius; \
  $nx=int( ($xb-$xa)/$ds+1.5 ); \
  $ny=int( ($yb-$ya)/$ds+1.5 ); \
  $commands = $commands . \
  "  rectangle \n" . \
  "    mappingName\n" . \
  "      $innerSquare\n" . \
  "    set corners\n" . \
  "     $xa $xb $ya $yb \n" . \
  "    lines\n" . \
  "      $nx $ny\n" . \
  "    boundary conditions\n" . \
  "      0 0 0 0\n" . \
  "    exit \n"; \
  $innerRadius=$outerRadius; \
  $outerRadius=$innerRadius+$deltaRadius; \
  $nx=$nTheta; \
  $ny=int( ($deltaRadius)/$ds+1.5 ); \
  $commands = $commands . \
  "*\n" . \
  "  annulus \n" . \
  "    mappingName \n" . \
  "      $outerAnnulus \n" . \
  "    inner and outer radii \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    centre\n" . \
  "      $xc $yc\n" .   \
  "    lines \n" . \
  "      $nx $ny\n" . \
  "    boundary conditions \n" . \
  "      -1 -1 2 0 \n" . \
  "    share\n" . \
  "     * material interfaces are marked by share>=100\n" . \
  "      0 0 $share 0   \n" . \
  "    exit \n"; \
}
# ===================================================
#   Make an array of cylinders
#     makeDiskArray(radius,nCylx,nCyly,x0,dx0,y0,dy0)
#     
# Make cylinders at centers
#      (x0+i*dx0,y0+j*dy0)  i=0,..,nx0, j=0,..,ny0
# 
# Result: $commands
# =====================================================
sub makeDiskArray \
{ local($radius,$nCylx,$nCyly,$x0,$dx0,$y0,$dy0)=@_; \
  local $cmds; $cmds=""; \
  for( $j=0; $j<$nCyly; $j++ ){ \
  for( $i=0; $i<$nCylx; $i++ ){ \
    makeDisk($radius,$x0+$i*$dx0,$y0+$j*$dy0); \
    $cmds = $cmds . $commands; \
  }}\
  $commands=$cmds; \
}
#
$rad=.25; $deltaY=1.; $deltaX=1.;
# 
$x0=-($nCylx-1)*$deltaX*.5; $y0=-($nCyly-1)*$deltaY*.5;  
makeDiskArray($rad,$nCylx,$nCyly,$x0,$deltaX,$y0,$deltaY);
$commands
# 
#
  rectangle 
    mappingName
      backGround
    set corners
     $rxa $rxb $rya $ryb 
    lines
      $nx=int( ($rxb-$rxa)/$ds+1.5 );
      $ny=int( ($ryb-$rya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      $bc
    exit 
#
  exit this menu 
#
generate an overlapping grid 
  backGround
  $mappingNames
  done 
#
  change parameters 
#   We must prevent hole cutting and interpolation between
#   the inner and outer grids.  
    prevent hole cutting
      all
      all
    done
    allow hole cutting
      $allowHoleCutting
    done
    prevent interpolation
      all
      all
    done
    allow interpolation 
      $allowInterpolation
    done
    ghost points 
      all 
      2 2 2 2 
    order of accuracy
     $orderOfAccuracy
    exit 
#    display intermediate results
# pause
#
    compute overlap
# pause
  exit
save a grid (compressed)
$name
latticeGrid
exit
