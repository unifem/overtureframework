#
# 3D Wing with a rounded tip using Joukowsky cross-sections 
#  The cross-sections are defined using embedded perl evaluations
#
# usage: ogen [noplot] wingj.cmd -factor=<num> -order=[2/4/6/8] -interp=[e/i] -wingSpan=<num>
#
# examples:
#     ogen noplot wingj.cmd -factor=1 
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$wingSpan=2.; $chord =1.5; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"wingSpan=f"=> \$wingSpan);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
$name = "wingj" . "$interp$factor" . $suffix . ".hdf";
# 
# 
$ds=.025/$factor;
$pi = 4.*atan2(1.,1.);
# 
create mappings
# 
#  Cross sections 
#
#  joukowsky(s, chord,a,d,delta) 
#    s= arclength
#    chord, a,d,delta (input) : 
#  Output: ($xj,$yj)
#
sub joukowsky\
{ local($s,$chord,$thick,$a,$d,$delta)=@_; local($cc,$ss,$amR,$amIm,$wNormI,$zRe,$zIm); \
  $cc = cos(2.*$pi*$s); $ss = sin(2.*$pi*$s); \
  $amRe=-$d*sin($delta); $amIm= $d*cos($delta);\
  $wRe=$a*$cc+$amRe; $wIm=(-$a)*$ss+$amIm;\
  $wNormI=1./($wRe*$wRe+$wIm*$wIm);\
  $zRe=$wRe+$wRe*$wNormI;  $zIm=$wIm-$wIm*$wNormI;\
  $xj=$zRe*$chord*.25; $yj=$zIm*$chord*.25; \
}
# airfoil parameters
$a=.85; $d=.15; $delta=(-15.)*$pi/180;   
# ellipse: $d=0.; $delta=0.; 
$degree=3;    # degree of the NURBS
$csNames="#"; # holds list of cross section names
#
# Here is a function to create a Joukowsky airfoil as a Nurbs
#  joukowskyNurbs(zj,angle,x0,y0,z0,xs,ys,zs)
#    zj = z-location
#    angle : rotation angle in degrees about z-axis
#    x0,y0,z0 : rotate about this point
#    xs,ys,zs : shift
sub joukowskyNurbs{ local($zj,$angle,$x0,$y0,$z0,$xs,$ys,$zs)=@_; local($cmdj,$i);\
$n=51; \
for( $i=0; $i<$n; $i++){ $s=$i/($n-1.); \
  joukowsky($s, $chord,$thick,$a,$d,$delta); $yj1=$yj+$ys; $cmd .= "$xj $yj1 $zj \n"; } \
}
# Here is a formula for the wing chord as a function of $r in [0,1]
# Here is a formula for the wing chord as a function of $r in [0,1]
$chordBase=2.; $chordTip=1.5; 
sub wingChord{ local( $r )=@_; local($chord); \
   $chord = $chordBase + $r*($chordTip-$chordBase); return $chord; }
# 
#  - here is the chord at the wing tip
sub wingTipChord{ local( $r )=@_; local($chord,$s); \
  $chord = $chordTip*sqrt( 1.-$r*$r ); }
# -- define the twist in the wing: 
$angleStart=0.; $angleEnd=0.; 
sub wingTwist{ local( $r )=@_; local( $angle ); $angle=$angleStart + ($angleEnd-$angleStart)*$r; \
              return $angle; }
# -- define the flex in the wing -- y value of the cross section
$yStart=0.; $yEnd=.0;  # we should match the center of mass of the original X-section ? 
sub wingFlex{ local( $r )=@_; local( $y ); $y=$yStart + ($yEnd-$yStart)*$r**3; \
              return $y; }
$yStartTip=$yEnd; $yEndTip=$yStartTip+.1;  # we should match the center of mass of the original X-section ? 
sub wingTipFlex{ local( $r )=@_; local( $y ); $y=$yStartTip + ($yEndTip-$yStartTip)*$r**3; \
              return $y; }
# 
# --- Make a set of cross sections for the main section of the wing ----
$wingLength=2.; # wing length (without tip)
$cs=0;  # labels cross-sections
$ncs=15; # number of cross-sections
$dr=1./($ncs-1); 
$zj=0; # current axial position
$x0=0.; $y0=0; $z0=0; $xs=0.; $ys=0.; $zs=0.;
$cmd=""; 
for( $j=0; $j<$ncs; $j++ ){ $rr=$j/($ncs-1.); \
  if( $rr < $ra ){ $r=0; }else{ $r=($rr-$ra)/(1.-$ra); } \
  $chord=wingChord($r); \
  $zj=$zj + $wingLength*$dr; $angle=wingTwist($r); $ys=wingFlex($r); \
  joukowskyNurbs($zj,$angle,$x0,$y0,$z0,$xs,$ys,$zs); \
}
# --- Here is the wing tip section ---
#     We reduce the chord to zero to round off the tip
$wingTipLength=.2; # wing tip length
$ncsTip=15; # number of cross-sections in wing tip
$ncs=$ncs+$ncsTip-1;
$drTip=1./($ncsTip-1.);
$d0=$d; $delta0=$delta; 
$ra=.0; # cross-sections begin to converge at r=$ra
# -- note: start at $j=1 so the wingTipChord matches
for( $j=1; $j<$ncsTip; $j++ ){ $rr=$j/($ncsTip-1.); \
  if( $rr < $ra ){ $r=0; }else{ $r=($rr-$ra)/(1.-$ra+1.e-14); } \
  $chord=wingTipChord($r); \
  $zj=$zj+$wingTipLength*$drTip; $angle=wingTwist($r); $ys=wingTipFlex($r); \
  $d=$d0*(1-$r*$r); $delta=$delta0*(1-$r*$r); \
  joukowskyNurbs($zj,$angle,$x0,$y0,$z0,$xs,$ys,$zs); \
}
$cmd .= "#";
#
#   --- Build a surface for the wing
  nurbs (surface)
    set domain dimension
    2
    set range dimension
    3
    periodicity
     2 0
    enter points
    $order=2; 
    $n $ncs $order
      $cmd
    lines 
      $nTheta = int( 2.5*$chordBase/$ds+1.5); 
      $nAxial = int( ($wingLength+$wingTipLength)/$ds+1.5 );
      $nTheta $nAxial
    mappingName
     wingSurface
#  pause
   exit
#
#
# 
  reparameterize
    transform which mapping?
      wingSurface
    restrict parameter space
     set corners
      0. .5 .7 1.
      exit
    mappingName
      capTop
    exit
  reparameterize
    transform which mapping?
      wingSurface
    restrict parameter space
      set corners
      .5 1. .7 1.
      exit
    mappingName
     capBottom
    exit
  composite surface
    CSUP:add a mapping capTop
    CSUP:add a mapping capBottom
    CSUP:determine topology
     deltaS 0.01
     maximum area 1.e-5
     compute topology
# pause
    exit
    CSUP:mappingName cap
    exit
# -- build a volume grid for the main part of the wing ---
#    -- first remove the tip:
  reparameterize
    transform which mapping?
    wingSurface
    restrict parameter space
      exit
    set corners
      0 1 0 .85
    mappingName
      wingSurfaceNoTip
    exit
# 
  hyperbolic
    target grid spacing .02 .02 (tang,norm)((<0 : use default)
    BC: bottom fix z, float x and y
      lines to march 11  
    generate
    name wing
    share
      0 0 2 0 1 0
    boundary conditions
      -1 -1 2 0 1 0
    exit
#
#  -- now build a grid on the wing tip 
 builder 
    Start curve:wingSurface
    target grid spacing .02 .02 (tang,norm)((<0 : use default)
    build curve on surface 
      plane point 1 -.1 -0.209381 2.2 
      plane point 2 -.1 0.435916 2.2 
      plane point 3 -.1 -0.209381 2.51446 
      cut with plane 
      exit 
    create surface grid... 
      choose boundary curve 0
      done
      forward and backward
      lines to march 35, 28 (forward,backward)  
      generate
      exit
    create volume grid...
      backward
      generate
      share
        0 0 0 0 1 0
      boundary conditions
        0 0 0 0 1 0
      name wingTip
      exit
    build a box grid
      x bounds: -1.5 2.
      y bounds: -0.5, .7
      z bounds: 0.142857, 3
      bc 3 4 5 6 2 7 (l r b t b f)
      share 0 0 0 0 2 0 (l r b t b f)
      name backGround
      exit
    exit
  exit this menu
#
#  -- create the overlapping grid --
#
generate an overlapping grid
  backGround
  wing
  wingTip
  done choosing mappings
  compute overlap
# 
  change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  display intermediate results
 compute overlap
exit
# save an overlapping grid
save a grid (compressed)
$name
wingj
exit
