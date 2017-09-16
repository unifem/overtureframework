#
# Grid for a rectangular dielectric block in a channel
#
#
# usage: ogen [noplot] dielectricBlockGrid2d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -angle=[degrees] -numStir=[1|2] -tStretch=<>
# 
#  -ml = number of (extra) multigrid levels to support
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#
#                       periodic 
#           yb  +-----------------------+
#               |        |     |        |
#               |        |     |        |
#               |        |     |        |
#               |        |     |        |
#               |        |     |        |
#               |        |     |        |
#               |        |width|        |
#           ya  +-----------------------+
#               xa      periodic       xb
# 
# examples:
#     ogen -noplot dielectricBlockGrid2d -order=2 -interp=e -factor=4
#
#
#
$prefix="dielectricBlockGrid2d";  $rgd="var"; $angle=0.; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
$width=.5;  # width of the block
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.5; $xb=2.5; $ya=-.5; $yb=.5; 
$cx=0.; $cy=0.;  # center of the block 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml, "prefix=s"=> \$prefix,"numGhost=i"=> \$numGhost,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"angle=f"=>\$angle,"numStir=i"=>\$numStir,\
            "height=f"=>\$height,"width=f"=>\$width,"tStretch=f"=> \$tStretch,"sharp=f"=>\$sharp );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $numStir eq 1 ){ $prefix = $prefix . "1"; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
create mappings
#
# ------- Left background grid -----
#
rectangle
  set corners
    $xal=$xa; $xbl=$cx -$width*.5; 
    $xal $xbl $ya $yb
  lines
    $nx = intmg( ($xbl-$xal)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 100 -1 -1 
  share 
    0  100 0 0
  mappingName
   leftBackGround
exit
#
# ------- Right background grid -----
#
rectangle
  set corners
    $xal=$cx + $width*.5; $xbl= $xb;
    $xal $xbl $ya $yb
  lines
    $nx = intmg( ($xbl-$xal)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    101 2 -1 -1 
  share 
    101 0 0 0
  mappingName
   rightBackGround
exit
##
# ------- Middle Block grid -----
#
rectangle
  set corners
    $xal=$cx -$width*.5; $xbl=$cx + $width*.5;
    $xal $xbl $ya $yb
  lines
    $nx = intmg( ($xbl-$xal)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    100 101 -1 -1 
  share 
    100 101 0 0
  mappingName
   middleBlock
exit
#
exit
#
#  --- generate the overlapping grid ---
#
generate an overlapping grid
    leftBackGround
    middleBlock
    rightBackGround
  done
  change parameters
    specify a domain
      innerDomain 
        middleBlock
    done
    specify a domain
      outerDomain
        leftBackGround
        rightBackGround
    done
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ngp = $ng+1;
      $ng $ng $ng $ngp $ng $ng
  exit
  # open graphics
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
dielectricBlock
exit



