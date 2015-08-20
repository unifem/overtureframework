#**************************************************************************
#
#  A grid for an interface calculation between two split boxes
#
# Usage: 
#         ogen [noplot] twoBoxesSplitInterfaceGrid [options]
# where options are
#     -factor=<num>      : use this factor for all directions (if given)
#     -xFactor=<num>     : default grid spacing in x-direction is multiplied by this factor
#     -yFactor=<num>     : default grid spacing in y-direction is multiplied by this factor
#     -zFactor=<num>     : default grid spacing in z-direction is multiplied by this factor
#     -order=[2/4/6/8]  : order of accuracy 
#     -interp=[e/i]     : implicit or explicit interpolation
#     -bc=[d|p]
# Examples: 
#    ogen -noplot twoBoxesSplitInterfaceGrid -xFactor=1 -order=2 -interp=e    (creates twoBoxesSplitInterfaceGride111.order2.hdf)
#    ogen -noplot twoBoxesSplitInterfaceGrid -order=2 -interp=e -factor=1 
#    ogen -noplot twoBoxesSplitInterfaceGrid -order=2 -interp=e -factor=2
#
#OLD: 
# -- periodic
#    ogen -noplot twoBoxesSplitInterfaceGrid -order=4 -interp=e -bc=p -factor=2
# -- rotated:
#    ogen -noplot twoBoxesSplitInterfaceGrid -factor=1 -order=2 -angle=45 -name="twoBoxesSplitInterfaceGridRotated1.order2.hdf" 
#    ogen -noplot twoBoxesSplitInterfaceGrid -factor=1 -order=4 -angle=45 -name="twoBoxesSplitInterfaceGridRotated1.order4.hdf" 
# 
#**************************************************************************
$order=2; $bc="d"; 
$factor=-1; $xFactor=1; $yFactor=1; $zFactor=1; 
$interp="i"; $interpType = "implicit for all grids";
$angle=0.;
#
$ya= 0.;   $yb=.5; 
$za= 0.;   $zb=.5; 
#
# 
# get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=> \$factor,"xFactor=i"=> \$xFactor,"yFactor=i"=> \$yFactor,"zFactor=i"=> \$zFactor,"interp=s"=> \$interp,"angle=f"=> \$angle,"name=s"=>\$name,"bc=s"=>\$bc);
# 
if( $factor>0 ){ $xFactor=$factor; $yFactor=$factor; $zFactor=$factor; }
if( $order eq 2 ){ $orderOfAccuracy="second order"; $ng=2; }\
elsif( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $bc eq "p" ){ $suffix .= "p"; } # periodic
if( $name eq "" ){ $name = "twoBoxesSplitInterfaceGrid" . "$interp$xFactor$yFactor$zFactor" . $suffix . ".hdf"; }
#
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
#
#
#**************************************************************************
#
# domain parameters:  
$dsx= .1/$zFactor; # target grid spacing in the x direction
$dsy= .1/$yFactor;          # target grid spacing in the y direction
$dsz= .1/$zFactor;          # target grid spacing in the y direction
#
create mappings
#
#  here is the left grid
#
  $yDelta = .5*($order-2+.5)*$dsy; # adjust two grids NOT to match
# 
#   -- left box bottom ---
#
  Box
    $xa=-1.0;  $xb=0.0; 
    $ya= 0.;   $yb=.5 + $yDelta; 
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $ny=int( ($yb-$ya)/$dsy+1.5 );
      $nz=int( ($zb-$za)/$dsz+1.5 );
      $nx $ny $nz
    boundary conditions
      $bcLeft = "1 100 3 0 5 6"; 
      if( $bc eq "p" ){ $bcLeft = "1 100 3 0 -1 -1 "; }  # periodic in the z-direction
      $bcLeft
    share
       # for now interfaces are marked with share>=100 
      1 100 0 0 0 0
    mappingName
      leftBoxBottom0
  exit
# 
#   -- left box top ---
#
  Box
    $xa=-1.0;  $xb=0.0; 
    $ya=.5 - $yDelta; $yb=1.; 
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $ny=int( ($yb-$ya)/$dsy+1.5 );
      $nz=int( ($zb-$za)/$dsz+1.5 );
      $nx $ny $nz
    boundary conditions
      $bcLeft = "1 100 0 4 5 6";  
      if( $bc eq "p" ){ $bcLeft = "1 101 0 4 -1 -1 "; }  # periodic in the z-direction
      $bcLeft
    share
       # for now interfaces are marked with share>=100 
      1 101 0 0 0 0
    mappingName
      leftBoxTop0
  exit
#
#  -- right box bottom ---
#
  Box
    $xa= 0.0;  $xb=1.0; 
    $ya= 0.;   $yb=.5 + $yDelta;
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $nx $ny $nz
    boundary conditions
      $bcRight = "100 2 3 0 5 6";  
      if( $bc eq "p" ){ $bcRight = "100 2 3 0 -1 -1 "; }  # periodic in the z-direction
      $bcRight
    share
      # for now interfaces are marked with share>=100 
      100 2 0 0 0 0 
    mappingName
      rightBoxBottom0
  exit
#
#  -- right box top ---
#
  Box
    $xa= 0.0;  $xb=1.0; 
    $ya=.5 - $yDelta; $yb=1.;
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$dsx+1.5 );
      $nx $ny $nz
    boundary conditions
      $bcRight = "100 2 0 4 5 6";  
      if( $bc eq "p" ){ $bcRight = "101 2 0 4 -1 -1 "; }  # periodic in the z-direction
      $bcRight
    share
      # for now interfaces are marked with share>=100 
      101 2 0 0 0 0 
    mappingName
      rightBoxTop0
  exit
#
#
  rotate/scale/shift
    transform which mapping?
      leftBoxBottom0
    rotate
     # rotate about the z-axis
     $angle 2 
     $yr = ($ya+$yb)*.5; 
     0. $yr 0.
    mappingName
      leftBoxBottom
    exit
#
  rotate/scale/shift
    transform which mapping?
      leftBoxTop0
    rotate
     # rotate about the z-axis
     $angle 2 
     $yr = ($ya+$yb)*.5; 
     0. $yr 0.
    mappingName
      leftBoxTop
    exit
#
  rotate/scale/shift
    transform which mapping?
      rightBoxBottom0
    rotate
     $angle 2
     0. $yr 0.
    mappingName
      rightBoxBottom
    exit
#
  rotate/scale/shift
    transform which mapping?
      rightBoxTop0
    rotate
     $angle 2
     0. $yr 0.
    mappingName
      rightBoxTop
    exit
#
  exit this menu
#
generate an overlapping grid
  leftBoxBottom
  leftBoxTop
  rightBoxBottom
  rightBoxTop
  done 
  change parameters
 # define the domains -- these will behave like independent overlapping grids
    specify a domain
      # domain name:
      leftDomain 
       # grids in the domain:
      leftBoxBottom
      leftBoxTop
      done
    specify a domain
      # domain name:
      rightDomain 
       # grids in the domain:
      rightBoxBottom
      rightBoxTop
      done
    order of accuracy
     $orderOfAccuracy
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
# pause
  # open graphics
  compute overlap
# pause
  exit
#
save an overlapping grid
  $name
  twoBoxesSplitInterfaceGrid
exit
