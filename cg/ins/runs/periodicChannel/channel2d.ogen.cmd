################################################################################
## ogen command file for a simple 2d periodic channel, to be used with the cgins file periodicChannel.cmd
## 
## $stretchFactor : make spacing at walls equal to $dx/$stretchFactor
# 
## 130213: kkc initial version
## 2013/11/23 : wdh changed stretching to use exp-to-linear
##
## Set defaults: 
if( $stretchFactor eq "" ){ $stretchFactor=5.; }
if( $ml eq "" ){ $ml=1; }
if( $order eq "" ){ $order=4; }
if( $Nx eq "" ){ $Nx=65; }
if( $Ny eq "" ){ $Ny=33; }
if( $xa eq "" ){ $xa=0.; }
if( $xb eq "" ){ $xb=2.; }
if( $ya eq "" ){ $ya=0.; }
if( $yb eq "" ){ $yb=1.; }
if( $bc eq "" ){ $bc="noSlipWall"; }
$orderOfAccuracyStr = "second order"; $ng=2;
if( $order eq 4 ){ $orderOfAccuracyStr="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracyStr="sixth order"; $ng=4; };
##
setMultigridLevels($ml);
#
$gridFilename = "channel2d.$Nx.$Ny.order${order}.ml${ml}";
create mappings
#
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $Nxml = intmg($Nx);
    $Nyml = intmg($Ny);
    $Nxml $Nyml
#  periodicity
#    1  0
  boundary conditions
$bcnumber = ($bc eq "wallModel" ? 1 : ($bc eq "noSlipWall" ? 2 : 3)); 
    -1 -1 $bcnumber $bcnumber
  mappingName
    unstretched-channel
exit
#
# -- we stretch twice : once for lower and once for upper wall --
$dx = ($xb-$xa)/($Nx-1.);  # base grid spcing on dx 
$dyBL= $dx/$stretchFactor; # dy at wall
$dyFarfield = $dx*2.5;  # adjust $dy for far field since we stretch twice
#
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
# 
# old stretching 
#- stretch coordinates
#- 
#-   Stretch r2:itanh
#-     STP:stretch r2 itanh: layer 0 1 $ystr 0 (id>=0,weight,exponent,position)
#-     STP:stretch r2 itanh: layer 1 1 $ystr 1 (id>=0,weight,exponent,position)
#-     stretch grid
#-   mappingName
#-    channel
#- exit
#
exit
#
generate an overlapping grid
  channel
  done
change parameters
  ghost points
  all
    $ngp=$ng+1;
  $ng $ng $ng $ngp
   order of accuracy
      $orderOfAccuracyStr
  exit
  compute overlap
#pause
  exit
#
save a grid (compressed)
$gridFilename
channel
exit

