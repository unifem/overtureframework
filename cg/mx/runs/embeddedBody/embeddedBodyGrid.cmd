#**************************************************************************
#
#  Builds grids for a body embedded in a dielectric region:
#
#
#    y=yb ---------------------------------------
#         |                                     |
#         |                                     |
#         |                                     |
#         |                                     |
#    y=ym  -------------------------------------- -
#         |                                     |
#         |                                     |
#         |           ------------              | -- y=ym - depth
#         |           |          |              |
#         |           |          |  height      |
#         |           ------------              |
#         |              width                  |
#         |                                     |
#     ya  ---------------------------------------
#        xa                                    xb
#
#  Usage:
#    ogen -noplot embeddedBodyGrid -factor=[1|2...] -interp=[e,i] -order=[2,4,6,8] -per=[0|1] -theta=[] ..
#          -width=<> -height=<> -depth=<>  -body=[0|1]    
# 
#    -per = 1 : make grids periodic in the x-direction
#    -theta (degrees) : when per=1, adjust [xa,xb] to be periodic for a wave with this angle of incidence from y axis.
# 
# Examples:
#   ogen -noplot embeddedBodyGrid -interp=e -order=2 -factor=2
#   ogen -noplot embeddedBodyGrid -interp=e -order=2 -factor=4
#   ogen -noplot embeddedBodyGrid -interp=e -order=2 -factor=8
#   ogen -noplot embeddedBodyGrid -interp=e -order=2 -factor=16
#
#   ogen -noplot embeddedBodyGrid -interp=e -order=4 -factor=4
#   ogen -noplot embeddedBodyGrid -interp=e -order=4 -factor=8
#   ogen -noplot embeddedBodyGrid -interp=e -order=4 -factor=16
# 
#   ogen -noplot embeddedBodyGrid -interp=e -numGhost=3 -order=4 -factor=4
#   ogen -noplot embeddedBodyGrid -interp=e -numGhost=3 -order=4 -factor=8
#   ogen -noplot embeddedBodyGrid -interp=e -numGhost=3 -order=4 -factor=16
# 
# -- Periodic (also adjust length so L*sin(theta) = integer)
#   ogen -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=4
#   ogen -noplot embeddedBodyGrid -interp=e -order=2 -per=1 -theta=60 -factor=8
#
# -- No Body:
#   ogen -noplot embeddedBodyGrid -interp=e -order=4 -prefix=embeddedNoBodyGrid -body=0 -factor=4
#
# -- No Body and periodic :
#   ogen -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -prefix=embeddedNoBodyGrid -body=0 -factor=4
#   ogen -noplot embeddedBodyGrid -interp=e -order=4 -per=1 -theta=60 -prefix=embeddedNoBodyGrid -body=0 -factor=8
#
#**************************************************************************
#
$prefix="embeddedBodyGrid"; 
$xa=-3; $xb=3; $ya=-2; $yb=2; $ym=0;  # inner domain bounds
$xca=-5; $xcb=5; $yca=-4; $ycb=4; $ycm=0;  # outer (coarse grid) domain bounds
$width=1.; $height=.5; $body=1; 
$depth=1./6.; # depth of the body 
$per=0; # per=1 : periodic BC's 
$theta=60; 
#
$sharp=20; $tStretch=5; # sharp = sharpnesst factor for corners
# 
$order=2; $factor=1; $interp="i"; $name=""; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
#
$numGhost=-1;  # if this value is set, then use this number of ghost points
GetOptions( "order=i"=>\$order,"factor=f"=>\$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "xba=f"=> \$xba,"xbb=f"=> \$xbb,"yba=f"=> \$yba,"ybb=f"=> \$ybb,"interp=s"=> \$interp,\
            "prefix=s"=> \$prefix,"name=s"=> \$name,"numGhost=i"=>\$numGhost,"per=i"=>\$per,"theta=i"=>\$theta,\
            "body=i"=>\$body,"sharp=f"=> \$sharp );
#
$xba=-.5*$width; $xbb=$xba+$width; $ybb=-$depth; $yba=$ybb-$height; # corners of embedded body
#
$pi=4.*atan2(1.,1.);
# Incident wave is u(x,y,t) = F(2*pi*[ -c*t + x*sin(theta) + y*cos(theta) ])
#   Length L=(xb-xa) should be adjusted so u(x+L,y,t)=u(x,y,t) => L*sin=integer
$L=$xcb-$xca; $sint=sin($theta*$pi/180); $delta=0.; 
if( $theta ne 0 ){ $iScale=int($L*$sint+.5); $L0=$iScale/$sint; $delta=$L0-$L; } # adjust L so L0*sin = integer
# 
if( $per eq 1 ){ printf("Adjust length: theta=%6.2f, L=%10.3e --> L0=%10.3e L0*sin(theta)=%6.2f \n",$theta,$L,$L0,$L0*$sint); }
#
if( ($per eq 1) && ($theta ne 0) ){ $xca = $xca-.5*$delta; $xcb=$xcb+.5*$delta; } # adjust length
#
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $per eq 1 ){ $suffix .= "p"; } # periodic
if( ($per eq 1) && ($theta ne 0) ){ $suffix .= "$theta"; } # angle
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $name eq "" ){$name = "$prefix" . "$interp$factor" . $suffix . ".hdf";}
#
#
# domain parameters:  
$ds = .1/$factor; # target grid spacing
$dsc = 2.*$ds; # coarse grid spacing 
#
#
$bcInterface=100;  # bc for interfaces
$ishare=100;
$bcInterfaceCoarse=101; 
$ishareCoarse=101;
# 
create mappings 
#
#    **** INNER FINE GRIDS ******
# ---- Upper half space ----
#
  rectangle 
    mappingName
      upperHalfSpace
    set corners
      $xa $xb $ym $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ym)/$ds+1.5 );
      $nx $ny
    boundary conditions
      # 1 2 $bcInterface 4
      0 0  $bcInterface 0
    share
      # material interfaces are marked by share>=100
      1  2 $ishare 0
   exit 
#
# ---- Lower half space ----
#
  rectangle 
    mappingName
      lowerHalfSpace
    set corners
      $xa $xb $ya $ym 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($ym-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      # 1 2 3 $bcInterface
      0 0 0 $bcInterface
    share
      # material interfaces are marked by share>=100
      1 2 0 $ishare
    exit 
#
#    **** OUTER COARSE GRIDS ******
# ---- Upper half space ----
#
  rectangle 
    mappingName
      upperHalfSpaceCoarse
    set corners
      $xca $xcb $ycm $ycb 
    lines
      $nx=int( ($xcb-$xca)/$dsc+1.5 );
      $ny=int( ($ycb-$ycm)/$dsc+1.5 );
      $nx $ny
    boundary conditions
      if( $per eq 1 ){ $bcmd = "-1 -1 $bcInterfaceCoarse 4"; }else{ $bcmd = "1 2 $bcInterfaceCoarse 4"; }
      $bcmd
    share
      # material interfaces are marked by share>=100
      1 2 $ishareCoarse 0
   exit 
#
# ---- Lower half space ----
#
  rectangle 
    mappingName
      lowerHalfSpaceCoarse
    set corners
      $xca $xcb $yca $ycm 
    lines
      $nx=int( ($xcb-$xca)/$dsc+1.5 );
      $ny=int( ($ycm-$yca)/$dsc+1.5 );
      $nx $ny
    boundary conditions
      if( $per eq 1 ){ $bcmd = "-1 -1 3 $bcInterfaceCoarse"; }else{ $bcmd = "1 2 3 $bcInterfaceCoarse"; }
      $bcmd
      # 1 2 3 $bcInterfaceCoarse
    share
      # material interfaces are marked by share>=100
      1 2 0 $ishareCoarse
    exit 
#
#  --- Circular body ----
# 
  annulus 
    mappingName 
      bodyAnnulus
    boundary conditions 
      -1 -1 7 0 
    $innerRadius=.25; $outerRadius=.5; $deltaRadius=$outerRadius-$innerRadius; 
    inner and outer radii 
      $innerRadius $outerRadius
    $cx=0.; $cy=-1.; 
    center: $cx $cy
    lines 
      $nx=int( (2.*$pi*$outerRadius)/$ds+1.5 );
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nTheta=$nx;
      $nx $ny 
    exit 
#
#  -- rectangular body ---
#
$nr = 6+$order;
# $nr = 12+$order;
SmoothedPolygon
  # start on a side so that the polygon is symmetric
  vertices 
    $xbm=.5*($xba+$xbb); # mid-point on horizontal face
    $ybm=.5*($yba+$ybb); # mid-point on vertical face
    6
    # --- start curve on bottom in middle if a wdie body ---
    $xbm   $yba
    $xbb   $yba
    $xbb   $ybb
    $xba   $ybb
    $xba   $yba
    $xbm   $yba
  n-stretch
   1. 1.5 0.
  n-dist
    fixed normal distance
    $nDist = ($nr-3)*$ds; 
    -$nDist
  periodicity
    2
  lines
    $stretchFactor=1.25; # add more lines in the tangential direction due to stretching at corners
    $length=2*( $xbb-$xba + $ybb-$yba ); # perimeter length 
    $nTheta = int( $stretchFactor*$length/$ds +1.5 ); 
    $nTheta $nr
  t-stretch
    0. 1.
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    0. 1.
  # set sharpness of corners
  sharpness
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
  boundary conditions
    -1 -1 7 0
  share 
     0  0 0 0
  mappingName
    bodySquare
  exit
#
#
# -- finished with mappings
  exit 
#
#
generate an overlapping grid 
  # $bodies = "bodyAnnulus"; # names of embedded bodies
  $bodies = "bodySquare"; # names of embedded bodies
  if( $body eq 0 ){ $bodies="#"; } # remove the body
  lowerHalfSpaceCoarse
  upperHalfSpaceCoarse
  upperHalfSpace
  lowerHalfSpace
  $bodies
  done 
#
  change parameters 
 # define the domains -- these will behave like independent overlapping grids
    specify a domain
      # domain name:
      upperDomain 
      # grids in the domain:
      upperHalfSpaceCoarse
      upperHalfSpace
    done
    specify a domain
      # domain name:
      lowerDomain 
      # grids in the domain:
      lowerHalfSpaceCoarse
      lowerHalfSpace
      $bodies
    done
    ghost points 
      all 
      $ng $ng $ng $ng $ng $ng
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    exit 
    #    display intermediate results
    # open graphics
    # pause
    compute overlap
    # pause
  exit
save a grid
$name
embeddedBodyGrid
exit
