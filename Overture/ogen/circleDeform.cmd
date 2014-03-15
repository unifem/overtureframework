*
* Create the initial grid for a deforming circle.
* Use this grid with cg/cns/cmd circleDeform.cmd 
*
*
* $factor=1.; $gridName = "circleDeform.hdf"; 
$factor=2.; $gridName = "circleDeform2.hdf"; 
* $factor=4.; $gridName = "circleDeform4.hdf"; 
*
$Pi=4.*atan2(1.,1.);
*
$ds0 = 1./20.; 
* target grid spacing:
$ds = $ds0/$factor;
*
create mappings
  annulus
    $width=.25; 
    $innerRadius=.5; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.; 
    inner and outer radii
      $innerRadius $outerRadius
    start and end angles
     -.3333 .3333
    $nr = int( 2.*$Pi*$averageRadius*(2./3.)/$ds+1.5 ); 
    $ns = int( $width/$ds0 +1.5 );
    lines
      $nr $ns
    boundary conditions
      0 0 1 0
    mappingName
    annulus
    share
    0 0 1 0
    exit
*
  rectangle
    $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    mappingName
     backGround
    exit
*
  nurbs (curve)
    enter points
      13
    $rad=.5; 
    $x0=0.; $y0=1.*$rad;
    $theta=$Pi* 5./180.; $x1=-$rad*sin($theta); $y1=$rad*cos($theta);
    $theta=$Pi*10./180.; $x2=-$rad*sin($theta); $y2=$rad*cos($theta);
    $theta=$Pi*15./180.; $x3=-$rad*sin($theta); $y3=$rad*cos($theta);
    $theta=$Pi*20./180.; $x4=-$rad*sin($theta); $y4=$rad*cos($theta);
    $theta=$Pi*50./180.; $x5=-$rad*sin($theta); $y5=$rad*cos($theta);
    $theta=$Pi*90./180.; $x6=-$rad*sin($theta); $y6=$rad*cos($theta);
    $x0 $y0
    $x1 $y1
    $x2 $y2
    $x3 $y3
    $x4 $y4
    $x5 $y5
    $x6 $y6
    $y0=-$y0; $y1=-$y1; $y2=-$y2; $y3=-$y3; $y4=-$y4; $y5=-$y5;
    $x5 $y5
    $x4 $y4
    $x3 $y3
    $x2 $y2
    $x1 $y1
    $x0 $y0
    lines
      31
* pause
    exit
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $nr = int( 2.*$Pi*$stretchFactor*$averageRadius*(.5)/$ds+1.5 ); 
    $dist = $width/$factor;     
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    generate
    mapping parameters
    Share Value: bottom  1
    close mapping dialog
    name ice
    exit
  exit this menu
*
generate an overlapping grid
  backGround
  annulus
  ice
  done choosing mappings
  change parameters
    shared sides may cut holes
      ice
      annulus
      done
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
  $gridName
  circleDeform
exit
