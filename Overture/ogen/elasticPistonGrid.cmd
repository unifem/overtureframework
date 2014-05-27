#
# Grid for the elastic piston
# 
#    ogen -noplot elasticPistonGrid -factor=1
#    ogen -noplot elasticPistonGrid -factor=2
#    ogen -noplot elasticPistonGrid -factor=4
#    ogen -noplot elasticPistonGrid -factor=8
#    ogen -noplot elasticPistonGrid -factor=16
#    ogen -noplot elasticPistonGrid -factor=32
#
# -- finer grid in y-direction:
#    ogen -noplot elasticPistonGrid -factor=8 -ny=80
#
# -- match x and y spacings:
#    ogen -noplot elasticPistonGrid -factor=2  -yfactor=2
#    ogen -noplot elasticPistonGrid -factor=4  -yfactor=4
#    ogen -noplot elasticPistonGrid -factor=8  -yfactor=8
#    ogen -noplot elasticPistonGrid -factor=16 -yfactor=16
#
#
$factor=1; $interpType = "implicit for all grids"; $orderOfAccuracy = "second order";
$ny =5; # NOTE
$yfactor=-1; # if positive then use this for y-spacing 
GetOptions( "factor=i"=>\$factor,"bc=s"=>\$bc,"ny=i"=>\$ny,"yfactor=i"=>\$yfactor );
if( $yfactor eq "-1" ){ $prefix="elasticPistonGrid$factor$bc"; }else{$prefix="elasticPistonGridfx$factor" . "fy$yfactor$bc"; }
if( $ny ne "5" ){ $prefix .= "ny$ny"; }
$name = "$prefix.hdf"; 
$bc="d"; 
# 
$ds = 1./10./$factor; 
$dsx=$ds; 
$dsy= 1./10./$yfactor; 
#
create mappings
#
#  here is the left fixed grid
#
  $xa=-1.; $xb=1.; $ya=0.;  $yb=1.; 
  rectangle
    $xal=$xa;  $xbl=0.; 
    set corners
     $xal $xbl $ya $yb 
    lines
      $nx=int( ($xbl-$xal)/$dsx+1.5 );
      if( $yfactor ne -1 ){ $ny=int( ($yb-$ya)/$dsy+1.5 ); }
      $nx $ny
    boundary conditions
      if( $bc eq "p" ){ $bcLeft ="1 100 -1 -1"; }else{ $bcLeft ="1 100 3 4"; }
      $bcLeft
    share
 # for now interfaces are marked with share>=100 
      0 100 0 0 
    mappingName
      leftSquare
  exit
# -- background grid for the right domain --
  rectangle
    set corners
      $xa=-.75; $xb=1.5; $ya=0.; $yb=1.; 
      $xa $xb $ya $yb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $nx $ny
    boundary conditions 
      $bcRight = "0 2 3  4"; 
      if( $bc eq "p" ){ $bcRight ="0 2 -1 -1"; }
      $bcRight
    share
      0 0 2 3
    mappingName
      rightSquare
    exit
#
#
#  -- grid for the interface of the right domain ---
#
  line (2D)
    set end points
      0 0 $ya $yb
    lines
      $ny 
    if( $bc eq "p" ){ $per ="1"; }else{ $per=0; }
    periodicity 
      $per
    exit
# 
  hyperbolic
    $dsi = $dsx; 
    target grid spacing -1, $ds  (tang,normal, <0 : use default)
 #* lines to march 7  
    # ----------------------- for comparing to nonPlug: 
    # lines to march 40
    # -- keep the grid width constant:
    $xbh = .5; $xah=0.; $lines = int( ($xbh-$xah)/$ds + .5 );
    lines to march $lines
 # backward
    forward
    marching options...
    if( $bc eq "p" ){ $hypeBC="*"; }else{ $hypeBC="BC: left fix y, float x and z\nBC: right fix y, float x and z"; }
#    BC: left fix y, float x and z
#    BC: right fix y, float x and z
    $hypeBC
 #$ortho=1.; # default: .5; 
 #orthogonal factor for mapping BC
 #   $ortho
 # -- added 081107 : 
    apply boundary conditions to start curve 1
    generate
#    show parameters
# 
    fourth order
    boundary conditions
      if( $bc eq "p" ){ $bci ="-1 -1 100 0"; }else{ $bci ="3 4 100 0"; }
      $bci
    share
      # *wdh* 2014/05/09 2 3 100 0 
      2 3 100 0 
    name interface
    exit
exit
#
#
generate an overlapping grid
  leftSquare
  rightSquare
  interface
  done 
  change parameters 
 # define the domains -- these will behave like independent overlapping grids
    specify a domain
 # domain name:
      rightDomain 
 # grids in the domain:
      rightSquare
      interface
    done
    specify a domain
 # domain name:
      leftDomain 
 # grids in the domain:
      leftSquare
      done
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    ghost points 
      all 
      2 2 2 2 
    order of accuracy
     $orderOfAccuracy
# 
 # show parameter values
    exit 
# 
#   debug
#     15
#   display intermediate results
  compute overlap
# pause
  exit
#
save an overlapping grid
  $name
  elasticPiston
exit


