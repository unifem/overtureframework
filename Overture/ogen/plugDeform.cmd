*
* Put a (deforming) plug in a channel for moving grid tests
* 
*    ogen noplot plugDeform -factor=1
*    ogen noplot plugDeform -factor=2
*    ogen noplot plugDeform -factor=4
*    ogen noplot plugDeform -factor=8
*    ogen noplot plugDeform -factor=16
*    ogen noplot plugDeform -factor=32
*
*
*
$factor=1;
GetOptions( "factor=i"=>\$factor,"bc=s"=>\$bc );
$name = "plugDeform$factor$bc.hdf"; 
$bc="d"; 
* 
$ds = 1./10./$factor; 
*
create mappings
  rectangle
    set corners
    $xa=-.75; $xb=1.5; $ya=0.; $yb=1.; 
      $xa $xb $ya $yb 
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny =5; # NOTE
      $nx $ny
    boundary conditions 
      $bcRight = "0 2 3  4"; 
      if( $bc eq "p" ){ $bcRight ="0 2 -1 -1"; }
      $bcRight
    share
      0 0 2 3
    mappingName
      square
    exit
*
*
*  here is the left moving grid
*
  line (2D)
    set end points
      0 0 $ya $yb
    lines
      $ny 
    if( $bc eq "p" ){ $per ="1"; }else{ $per=0; }
    periodicity 
      $per
    exit
* 
  hyperbolic
    $dsi = $dsx; 
    target grid spacing -1, $ds  (tang,normal, <0 : use default)
    ** lines to march 7  
    # ----------------------- for comparing to nonPlug: 
    # lines to march 40
    # -- keep the grid width constant:
    $xbh = .5; $xah=0.; $lines = int( ($xbh-$xah)/$ds + .5 );
    lines to march $lines
    * backward
    forward
    marching options...
    if( $bc eq "p" ){ $hypeBC="*"; }else{ $hypeBC="BC: left fix y, float x and z\nBC: right fix y, float x and z"; }
*    BC: left fix y, float x and z
*    BC: right fix y, float x and z
    $hypeBC
    *$ortho=1.; # default: .5; 
    *orthogonal factor for mapping BC
    *   $ortho
    * -- added 081107 : 
    apply boundary conditions to start curve 1
    generate
*    show parameters
* 
    fourth order
    boundary conditions
      if( $bc eq "p" ){ $bci ="-1 -1 100 0"; }else{ $bci ="2 3 100 0"; }
      $bci
    share
      2 3 100 0 
    name plug
    exit
exit
*
generate an overlapping grid
  square
  plug
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      implicit for all grids
      * explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
*
exit
save an overlapping grid
$name
plugDeform
exit

