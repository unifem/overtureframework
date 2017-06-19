#
# make 2D heartValve grid (modified based on stir grid)
#
#     ogen -noplot heartValve2dGrid.cmd -interp=i -factor=2
#     ogen -noplot heartValve2dGrid.cmd -interp=e -factor=2
#
# G1 may be too coarse for some tests
#
$order=2; 
$orderOfAccuracy = "second order"; 
$ng=2;   # number of ghost points 
$interp="i"; 
$sharp=40.; 
$nStretch=4.; 
$hfStir=0.025; #half size of the leaflet
$rStir=0.6;    #size of stir
$ycenter=0.15;
$yb=1.1; $ya="";
$xa=-0.5; $xb=1.25;
#leaflet shift:
$yshift=0; $xshift=0;
#
$factor=1;
$offSet=0; #contral the offset of left and right edge of leaflet
#
$blfc=1;  # grid lines on Channel-grid are this much finer near the boundary (1=unstretched)
$prefix="heartValve2d";
$leaflets=2;
$name=""; 
$channelBaseGrid="backGround";
$stretchBase="";
#
GetOptions( "order=i"=>\$order,"yshift=f"=>\$yshift,"ds=f"=>\$ds,"ycenter=f"=>\$ycenter,\
"degree=f"=>\$degree,"xa=f"=>\$xa,"xb=f"=>\$xb,"yb=f"=>\$yb,"ya=f"=>\$ya, "xshift=f"=>\$xshift,\
"rStir=f"=> \$rStir,"offSet=f"=> \$offSet,\
"leaflets=i"=> \$leaflets,\
"blfc=f"=> \$blfc,"factor=f"=> \$factor,\
"interp=s"=> \$interp,"name=s"=> \$name,"prefix=s"=> \$prefix,"sharp=f"=>\$sharp,"nStretch=f"=>\$nStretch );
#
if( $order eq "4" ){ $orderOfAccuracy = "fourth order"; }
if( $interp eq "i" ){ $interpType="implicit for all grids"; }
if( $interp eq "e" ){ $interpType="explicit for all grids"; }
if( $ya eq "" ){$ya=-$yb; }
#if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; } # normal distance is fixed as 0.05 for now
if( $leaflets eq 1){$prefix="oneLeaflet";}
$suffix = ".order$order"; 
if( $name eq "" ){$name = $prefix . "$interp$factor" . "$suffix" . ".hdf";}
#
#
#
$ds=.025/$factor;
$pi = 4.*atan2(1.,1.);
#
#no multigrid for now
$channelBaseGrid="backGround"; $channelStretched="backGround-streched";
if( $blfc ne "1" ){ $channelBaseGrid="backGround-unstreched"; $channelStretched="backGround"; }
#
create mappings
  rectangle
    set corners
    $xa $xb $ya $yb
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    lines
      $nx $ny
    boundary conditions
      1 2 3 4
    mappingName
      $channelBaseGrid
  exit
*
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    6
    $hfStirn=-$hfStir;
    $dy0=0.05;
    $dy1=0.05+$rStir/2.;
    $dy2=0.05+$rStir;
    $dy0p=$dy0+$offSet;
    $dy2p=$dy2+$offSet;
    #polygon
    $hfStirn  $dy1
    $hfStirn  $dy2
     $hfStir  $dy2p
     $hfStir  $dy0p
    $hfStirn  $dy0
    $hfStirn  $dy1
  n-stretch
   1. $nStretch 0.
  n-dist
    fixed normal distance
    $drfixed=0.05;
    $drfixed
  periodicity
    2
  lines
  $nr = int( 0.05/$ds +5 ); 
  $nTheta = int(3*($dy2-$dy0)/$ds+5);
  $nTheta $nr
  t-stretch
    0. 1.
    1. 9.
    1. 9.
    1. 9.
    1. 9.
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
    -1 -1 5 0
  share
     0 0 100 0
  mappingName
    stir1original
  exit
#the second stir (fully symmetric)
SmoothedPolygon
# start on a side so that the polygon is symmetric
  vertices 
    6
    $dy0=-$dy0;
    $dy1=-$dy1;
    $dy2=-$dy2;
    $dy0p=-$dy0p;
    $dy2p=-$dy2p;
    $hfStirn $dy1
    $hfStirn $dy0
     $hfStir $dy0p
     $hfStir $dy2p
    $hfStirn $dy2
    $hfStirn $dy1
 n-stretch
   1. $nStretch 0.
  n-dist
    fixed normal distance
    $drfixed
  periodicity
    2
  lines
    $nTheta $nr
  t-stretch
    0. 1.
    1. 9.
    1. 9.
    1. 9.
    1. 9.
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
    -1 -1 6 0
  share
     0 0 200 0
  mappingName
    stir2original
  exit
#
#**************************************************************************
# optionally stretch the back-ground grid
#**************************************************************************
# 
 stretch coordinates 
  transform which mapping? 
    $channelBaseGrid
  multigrid levels 0
  #pause 
  #tanh stretch can be used here but I do not think simply streching in one dimension will make it stable
  $stretchResolution = 1.2;
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
  # bottom
   $dir="r2"; $pos=0.; 
   Stretch $dir:exp to linear
   STP:stretch $dir expl: position $pos
   $dxMin = $ds/$blfc; 
   STP:stretch $dir expl: min dx, max dx $dxMin $ds
  STRT:name channelStreched1
 exit
## 
 stretch coordinates 
  transform which mapping? 
    channelStreched1
  multigrid levels 0
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
  # top
   $dir="r2"; $pos=1.; 
   Stretch $dir:exp to linear
   STP:stretch $dir expl: position $pos
   STP:stretch $dir expl: min dx, max dx $dxMin $ds
   STRT:name $channelStretched
   #STRT:name channelStretched2
  exit
## 
# stretch coordinates 
#  transform which mapping? 
#    channelStretched2
#  multigrid levels 0
#  stretch resolution factor  $stretchResolution
#  # exponential to linear stretching: 
#  # middle
#   $dir="r2"; $pos=0.5; 
#   Stretch $dir:exp to linear
#   STP:stretch $dir expl: position $pos
#   STP:stretch $dir expl: min dx, max dx $dxMin $ds
#  STRT:name $channelStretched
# exit
#
#**************************************************************************
#   Now take the extra box and rotate it (uncomment it if no rotation)
#**************************************************************************
#
  rotate/scale/shift
    transform which mapping?
    stir1original
    rotate
    $degree 0 0
    0 $ycenter 0
    shift
    #$xshift=-0.2;
    $xshift $yshift 0
   mappingName
    stir1
  exit
  rotate/scale/shift
    transform which mapping?
    stir2original
    $ycenter=-$ycenter;
    $degree=-$degree;
    rotate
    $degree 0 0
    0 $ycenter 0
    shift
    $yshift=-$yshift;
    $xshift $yshift 0
   mappingName
    stir2
  exit
exit
#
# now make an overlapping grid
#
generate an overlapping grid
  backGround
  stir1
  if ($leaflets eq 1){$cmd="#";}else{$cmd="stir2";}
  $cmd
  done
* 
  change parameters
   order of accuracy
    $orderOfAccuracy
   interpolation type
     $interpType
   ghost points
     all
     $ng $ng $ng $ng $ng $ng
  exit
  *   display intermediate
  compute overlap
  #  continue
  #  pause
exit
save an overlapping grid
$name
stir
exit
