*
* Create the initial grid for a deforming cylinder
* Use this grid with the cg/cns/cmd circleDeform.cmd 
*
* Usage:
*         ogen [noplot] cylDeform [options]
* where options are
*     -factor=<num>     : grid spacing is .1 divided by this factor
*     -interp=[e/i]     : implicit or explicit interpolation
*     -name=<string>    : over-ride the default name  
*
* Examples:
*
*      ogen noplot cylDeform -factor=1
*      ogen noplot cylDeform -factor=2
*      ogen noplot cylDeform -factor=1 -interp=e
*      ogen noplot cylDeform -factor=2 -interp=e
* 
$factor=1; $name=""
$interp="i"; $interpType = "implicit for all grids"; 
* 
* get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp);
* 
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "cylDeform" . "$interp$factor" . ".hdf";}
*
$Pi=4.*atan2(1.,1.);
*
$ds0 = 1./10.; 
* target grid spacing:
$ds = $ds0/$factor;
*
create mappings
*
  $xa=-1.5; $xb=1.5; $ya=-1.5; $yb=1.5;   $za=0.; $zb=2.; 
* 
*  Here is the undeformed cylinder
* 
  Cylinder
    $width=4*$ds; # .25/$factor; 
    $innerRadius=.5; $outerRadius=$innerRadius + $width; 
    $averageRadius=($innerRadius+$outerRadius)/2.; 
* 
    bounds on the radial variable
      $innerRadius $outerRadius
    bounds on theta
      -.3333 .3333
* 
    bounds on the axial variable
      $za $zb
* 
    $nTheta = int( 2.*$Pi*$averageRadius*(2./3.)/$ds+1.5 ); 
    $nr = int( $width/$ds +2.5 );
    $nz = int( ($zb-$za)/$ds +1.5 );
    lines
      $nTheta $nz $nr
    boundary conditions
      * 
      0 0 2 3 1 0
    mappingName
      cylinder
    share
      0 0 2 3 1 0
* 
    exit
*
*  Here is the front face that deforms
* 
  nurbs (surface)
    $rad=.5; 
    $x0=0.; $y0=1.*$rad;
    $theta=$Pi* 5./180.; $x1=-$rad*sin($theta); $y1=$rad*cos($theta);
    $theta=$Pi*10./180.; $x2=-$rad*sin($theta); $y2=$rad*cos($theta);
    $theta=$Pi*15./180.; $x3=-$rad*sin($theta); $y3=$rad*cos($theta);
    $theta=$Pi*20./180.; $x4=-$rad*sin($theta); $y4=$rad*cos($theta);
    $theta=$Pi*50./180.; $x5=-$rad*sin($theta); $y5=$rad*cos($theta);
    $theta=$Pi*90./180.; $x6=-$rad*sin($theta); $y6=$rad*cos($theta);
* 
*    $x5=$x5-$rad*.1; $y5=$y5+.025*$rad; 
*    $x6=$x6+$rad*.1;
*
    $y0a=-$y0; $y1a=-$y1; $y2a=-$y2; $y3a=-$y3; $y4a=-$y4; $y5a=-$y5;
* 
    enter points
      13 4  3
*
    $z0=$za; 
    $x0 $y0 $z0
    $x1 $y1 $z0
    $x2 $y2 $z0
    $x3 $y3 $z0
    $x4 $y4 $z0
    $x5 $y5 $z0
    $x6 $y6 $z0
*
    $x5 $y5a $z0
    $x4 $y4a $z0
    $x3 $y3a $z0
    $x2 $y2a $z0
    $x1 $y1a $z0
    $x0 $y0a $z0
*   ---------------------
    $z0=$za+($zb-$za)*(1./3.); 
    $x0 $y0 $z0
    $x1 $y1 $z0
    $x2 $y2 $z0
    $x3 $y3 $z0
    $x4 $y4 $z0
    $x5 $y5 $z0
    $x6 $y6 $z0
*
    $x5 $y5a $z0
    $x4 $y4a $z0
    $x3 $y3a $z0
    $x2 $y2a $z0
    $x1 $y1a $z0
    $x0 $y0a $z0
*   -----------------------
    $z0=$za+($zb-$za)*(2./3.); 
    $x0 $y0 $z0
    $x1 $y1 $z0
    $x2 $y2 $z0
    $x3 $y3 $z0
    $x4 $y4 $z0
    $x5 $y5 $z0
    $x6 $y6 $z0
*
    $x5 $y5a $z0
    $x4 $y4a $z0
    $x3 $y3a $z0
    $x2 $y2a $z0
    $x1 $y1a $z0
    $x0 $y0a $z0
*   -----------------------
    $z0=$zb;
    $x0 $y0 $z0
    $x1 $y1 $z0
    $x2 $y2 $z0
    $x3 $y3 $z0
    $x4 $y4 $z0
    $x5 $y5 $z0
    $x6 $y6 $z0
*
    $x5 $y5a $z0
    $x4 $y4a $z0
    $x3 $y3a $z0
    $x2 $y2a $z0
    $x1 $y1a $z0
    $x0 $y0a $z0
    lines
      * increase lines in theta direction to account for a later deformation
      $nTheta = int( 1.3*$nTheta +1.5 ); 
      $nTheta $nz $nr
* pause
    mappingName
     deformSurface
    exit
* 
  hyperbolic
    Start curve:deformSurface
**    target grid spacing $ds $ds (tang,normal, <0 : use default)
    marching options...
    BC: bottom fix z, float x and y
    BC: top fix z, float x and y
    * Note: For some reason the distance to march needs to be smaller by $ds ??
    $dist=$width-$ds; 
    * $dist=$width; 
    distance to march $dist
    $nrm=$nr-1; 
    lines to march $nrm
* 
    generate
    boundary conditions
      0 0 2 3 1 0
    share
      0 0 2 3 1 0
    name deformVolume
    exit
* 
  box
    set corners
      $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds+1.5 );
      $ny = int( ($yb-$ya)/$ds+1.5 );
      $nz = int( ($zb-$za)/$ds+1.5 );
       $nx $ny $nz
    boundary conditions
      1 1 1 1 2 3 
    share
      0 0 0 0 2 3
    mappingName
     backGround
*  pause
  exit
exit this menu
*
generate an overlapping grid
  backGround
  cylinder
  deformVolume
  done choosing mappings
  change parameters
    shared sides may cut holes
      deformVolume
      cylinder
      done
    interpolation type
      $interpType
    ghost points
      all
      2 2 2 2 2 2
    exit
*
  compute overlap
*
exit
*
save an overlapping grid
  $name
  cylDeform
exit
