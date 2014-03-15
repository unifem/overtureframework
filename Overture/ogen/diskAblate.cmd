*
* Create the initial grid for an ablating disk 
*
* Usage:
*         ogen [noplot] diskAblate [options]
* where options are
*     -factor=<num>     : grid spacing is .1 divided by this factor
*     -interp=[e/i]     : implicit or explicit interpolation
*     -name=<string>    : over-ride the default name  
*
* Examples:
*
*      ogen noplot diskAblate -factor=1
*      ogen noplot diskAblate -factor=2
*      ogen noplot diskAblate -factor=1 -interp=e
*      ogen noplot diskAblate -factor=2 -interp=e
* 
$factor=1; $name=""
$interp="i"; $interpType = "implicit for all grids"; 
* 
* get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp);
* 
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "diskAblate" . "$interp$factor" . ".hdf";}
*
$bcInterface0=100;  # bc for interfaces
$bcInterface1=101;  
$shareInterface=100;        # share value for interfaces
*
$Pi=4.*atan2(1.,1.);
*
$ds0 = 1./20.; 
* target grid spacing:
$ds = $ds0/$factor;
*
create mappings
*
* -- grids for the outer region --
* 
  annulus
    $width=.25; 
    $innerRadius=.5; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=$innerRadius; # ($innerRadius+$outerRadius)/2.; 
    inner and outer radii
      $innerRadius $outerRadius
    start and end angles
     * the annulus goes from approx. -pi/2 to pi/2 
     $dTheta=2.*$ds/(2.*$Pi*$averageRadius);  # overlap region 
     $theta0 = -.25 - $dTheta; $theta1=.25+$dTheta; 
     $theta0 $theta1
     * -.3333 .3333
    $nr = int( 2.*$Pi*$averageRadius*($theta1-$theta0)/$ds+1.5 ); 
    $ns = int( $width/$ds0 +1.5 );
    lines
      $nr $ns
    boundary conditions
      0 0 $bcInterface0 0
    mappingName
     outerAnnulus
    share
      0 0 $shareInterface 0
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
     outerBackGround
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
* 
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
    Boundary Condition: bottom  $bcInterface1
    Share Value: bottom  $shareInterface
    close mapping dialog
    name outerIce
    exit
*
*
* -- grids for the inner region --
* 
  annulus
    $width2=.2; 
    $outerRadius2=$innerRadius; $innerRadius2=$outerRadius2 - $width/$factor; 
    $averageRadius2=$outerRadius2; # ($innerRadius2+$outerRadius2)/2.; 
    inner and outer radii
      $innerRadius2 $outerRadius2
    start and end angles
      $theta0 $theta1
      * -.3333 .3333
    $nr = int( 2.*$Pi*$averageRadius2*($theta1-$theta0)/$ds+1.5 ); 
    $ns = int( $width2/$ds0 +1.5 );
    lines
      $nr $ns
    boundary conditions
      0 0 0 $bcInterface0
    mappingName
     innerAnnulus
    share
     0 0 0 $shareInterface
    exit
* 
  hyperbolic
    * add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $nr = int( 2.*$Pi*$stretchFactor*$averageRadius2*(.5)/$ds+1.5 ); 
    $dist = $width2/$factor;     
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    backward
    generate
    mapping parameters
      Boundary Condition: bottom  $bcInterface1
      Share Value: bottom $shareInterface
    close mapping dialog
    name innerIce
    exit
*
*
  rectangle
    * make the inner square big enough to allow the cylinder to deform 
    $xb = $innerRadius2*1.5; 
    $xa=-$xb; $ya=$xa; $yb=$xb; 
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      0 0 0 0 
    mappingName
     innerBackGround
    exit
*
exit this menu
*
generate an overlapping grid
  outerBackGround
  outerAnnulus
  outerIce
  innerBackGround
  innerAnnulus
  innerIce
  done choosing mappings
* 
  change parameters
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
      outerBackGround
      outerAnnulus
      outerIce
      done
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
      innerBackGround
      innerAnnulus
      innerIce
      done
* 
    shared sides may cut holes
      outerIce
      outerAnnulus
     * 
      innerIce
      innerAnnulus
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
  diskAblate
exit
