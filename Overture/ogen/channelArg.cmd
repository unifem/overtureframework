#
# Grid for a periodic channel
#
#
# usage: ogen [noplot] channelArg -factor=<num> -order=[2/4/6/8] -ml=<> -per=[0|1] -stretchFactor=<f>
# 
#  -per : 1=periodic channel (default), 0=not periodic
#  -ml = number of (extra) multigrid levels to support
#  -stretchFactor  make spacing at walls equal to $dx/$stretchFactor
# 
# examples:
#     ogen -noplot channelArg -factor=1
#     ogen -noplot channelArg -factor=2
#     ogen -noplot channelArg -factor=4
#     ogen -noplot channelArg -factor=5
#     ogen -noplot channelArg -factor=10
#     ogen -noplot channelArg -factor=20
#     ogen -noplot channelArg -factor=40
#
# Non-periodic: (for pipe flow)
#     ogen -noplot channelArg -order=2 -length=2. -per=0 -factor=1
#     ogen -noplot channelArg -order=2 -length=2. -per=0 -factor=2
#						 								  
#     ogen -noplot channelArg -order=4 -length=2. -per=0 -factor=1
#     ogen -noplot channelArg -order=4 -length=2. -per=0 -factor=2
# 						 								  
# Fourth-order: (non-periodic)			 					  
#     ogen -noplot channelArg -order=4 -length=3. -per=0 -factor=1
#     ogen -noplot channelArg -order=4 -length=3. -per=0 -factor=2
#
# Fourth-order multigrid (periodic channel):
#     ogen -noplot channelArg -order=4 -factor=2 -ml=2
#     ogen -noplot channelArg -order=4 -factor=4 -ml=3
#     ogen -noplot channelArg -order=4 -factor=8 -ml=4
#     ogen -noplot channelArg -order=4 -factor=16 -ml=5
#     ogen -noplot channelArg -order=4 -factor=32 -ml=5
#     ogen -noplot channelArg -order=4 -factor=64 -ml=6
# 
# multigrid:
#     ogen -noplot channelArg -factor=2 -ml=2
#     ogen -noplot channelArg -factor=5 -ml=3 
#     ogen -noplot channelArg -factor=10 -ml=3
#     ogen -noplot channelArg -factor=20 -ml=3   (channel20.order2.ml3.hdf, 1.2M pts)
#     ogen -noplot channelArg -factor=40 -ml=4   (channel40.order2.ml4.hdf, 5M pts)
# 
# longer channel
#     ogen -noplot channelArg -factor=5  -length=12. -name=channelLength12f5.hdf
#     ogen -noplot channelArg -factor=10 -length=12. -name=channelL12f10.hdf
#     ogen -noplot channelArg -factor=20 -length=12 -ml=3 -name=channelL12f20.hdf
#
$pi=4.*atan2(1.,1.); $per=1; 
$order=2; $factor=1;  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
$length=2.*$pi; # length of channel)/(2*pi)
$xa=0.; $xb=$length; 
$ya=-1.; $yb=1.; 
$stretchFactor=5.; 
$bStretch=7.; $nyFactor=2.5; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=>\$factor,"ml=i"=>\$ml,"length=f"=>\$length,\
            "name=s"=> \$name,"per=i"=>\$per,"stretchFactor=f"=>\$stretchFactor,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb );
$xb=$length; 
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = "channel" . "$interp$factor" . $suffix . ".hdf"; }
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
# 
$ds=.1/$factor;
#
# 
create mappings
#
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 );
    $ny = intmg( $nyFactor*($yb-$ya)/$ds +1.5 );
    $nx $ny
  boundary conditions
    if( $per eq 1 ){ $cmd="-1 -1 3 4"; }else{ $cmd="1 2 3 4"; }
    $cmd
  mappingName
   unstretched-channel
exit
# -- we stretch twice : once for lower and once for upper wall --
$dx = ($xb-$xa)/($nx-1.);  # base grid spcing on dx 
$dyBL= $dx/$stretchFactor; # dy at wall
$dyFarfield = $dx*2.5;  # adjust $dy for far field since we stretch twice
#
# stretch for lower wall: 
# stretch for lower wall: 
stretch coordinates 
  Stretch r2:exp to linear
  STRT:multigrid levels $ml
  STP:stretch r2 expl: position 0
  STP:stretch r2 expl: min dx, max dx $dyBL $dyFarfield
  mappingName
   channelStretch1
exit
# -- stretch for upper wall: 
stretch coordinates 
  Stretch r2:exp to linear
  STRT:multigrid levels $ml
  STP:stretch r2 expl: position 1.
  STP:stretch r2 expl: min dx, max dx $dyBL $dyFarfield
  mappingName
   channel
exit
# old ---
#   stretch coordinates
#     Stretch r2:itanh
#     STP:stretch r2 itanh: layer 0 1 $bStretch 0 (id>=0,weight,exponent,position)
#     STP:stretch r2 itanh: layer 1 1 $bStretch 1 (id>=0,weight,exponent,position)
#     stretch grid
#   mappingName
#    channel
#   exit
# 
#
exit
generate an overlapping grid
   # unstretched-channel
   channel
  done
  change parameters
    ghost points
      all
       $ngp=$ng+1; 
       $ng $ng $ng $ngp $ng $ng 
    order of accuracy
      $orderOfAccuracy
  exit
  compute overlap
  exit
#
save a grid (compressed)
$name
channel
exit

