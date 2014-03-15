***************************************************************************
*
*  Create a fuel assembly consisting of rods in an hexagonal domain
* 
*   (see also rodArray, cylArray.cmd for hollow cylinders and 
*     latticeCyl.cmd for solid cylinders but not stretched)
*    
* Usage: 
*         ogen [noplot] fuelAssembly [options]
* where options are
*     -factor=<num>     : grid spacing is .05 divided by this factor
*     -order=[2/4/6/8]  : order of accuracy 
*     -interp=[e/i]     : implicit or explicit interpolation
*     -npins=<num>      : number of pins across the center horizontal diameter 
*     -pinRadius=<num>  : radius of each pin
*     -sharp=<num>      : corner sharpness exponent
*     -zb=<num>         : length of the cylnders in the z-direction (axial)
* 
* Examples: 
*    ogen noplot fuelAssembly -factor=1 -npins=3  
*    ogen noplot fuelAssembly -factor=1 -npins=5 -sharp=100
*    ogen noplot fuelAssembly -factor=1 -npins=17
***************************************************************************
$factor=1;  $ds0=.05; 
$order=2; $orderOfAccuracy="second order"; $ng=2; 
$interp="i"; $interpType = "implicit for all grids";
$npins=1;  # number of pins along the diameter
$ductWallThickness=.1;
$pinRadius=.5; $pinDiameter=2.*$pinRadius; 
$pinGap=.2; # gap between pins
$outerGap=.4;  # gap between last pin and the assembly wall
* 
$rad=.3;
$nStretch=3.; 
$dist=1.;
$za=0.; $zb=1.; 
$zc=0.; 
$sharp=""; 
*
* get command line arguments
GetOptions("zb=f"=>\$zb,"pinRadius=f"=>\$pinRadius,"sharp=f"=>\$sharp,"order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"npins=i"=> \$npins);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
$name = "fuelAssembly" . "$npins" ."pins" . "$interp$factor" . $suffix . ".hdf";
* 
if( $sharp eq "" ){ $sharp=$factor*30.+($npins-1)*20.; }
* 
* $deltaY=$dist; $deltaX=$dist;   # spacing between cylinder centres
* $xb = $nCylx*$deltaX/2.+$rad*0.; $xa=-$xb;
* $yb = $nCyly*$deltaY/2.+$rad*0.; $ya=-$yb;
* 
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
}
*
***************************************************************************
*
*
* domain parameters:  
$ds = $ds0/$factor; # target grid spacing
$pi=3.1415926535897932384626433;
*
*
*
create mappings 
*
$interfaceBC=100; $outerDuctBC=3; 
$ishare=100; 
*
$count=0;  # number the disks as 1,2,3...
*
*
$pinSpacing=$pinDiameter+$pinGap; 
$radiusAssembly=($npins-1)*($pinSpacing/2.)+$pinRadius+$outerGap;
*
$nsides=6; # hexagon
*
  smoothedPolygon
    vertices
     $nv=$nsides+2;
     $nv
     $theta=0.; $deltaTheta=2.*$pi/($nsides);
     $xc=0.; # .14/2.;  # shift the assembly left to account for smooth corners -- fix this 
     * start the curve on the bottom flat portion
     $x0=$radiusAssembly*cos($theta)-$xc; $y0=$radiusAssembly*sin($theta); $theta=$theta+$deltaTheta;
     $x1=$radiusAssembly*cos($theta)-$xc; $y1=$radiusAssembly*sin($theta); $theta=$theta+$deltaTheta;
     $x2=$radiusAssembly*cos($theta)-$xc; $y2=$radiusAssembly*sin($theta); $theta=$theta+$deltaTheta;
     $x3=$radiusAssembly*cos($theta)-$xc; $y3=$radiusAssembly*sin($theta); $theta=$theta+$deltaTheta;
     $x4=$radiusAssembly*cos($theta)-$xc; $y4=$radiusAssembly*sin($theta); $theta=$theta+$deltaTheta;
     $x5=$radiusAssembly*cos($theta)-$xc; $y5=$radiusAssembly*sin($theta); $theta=$theta+$deltaTheta;
* 
     $xa=.5*($x4+$x5); $ya=.5*($y4+$y5);
*
     $xa $ya
     $x5 $y5
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
     $x4 $y4
     $xa $ya
* 
    sharpness
      * sharpen the corners as the grid is refined.
      $sharp
      $sharp
      $sharp
      $sharp
      $sharp
      $sharp
      $sharp
      $sharp
    n-dist
     fixed normal distance
      $nr=6; 
      $nDist=$ds*$nr*.5; 
      $nDist
    n-stretch
      1 4. 0
    t-stretch
      $ta=.1/$nsides; $tb=5.;
      0.  1. 
      $ta $tb
      $ta $tb
      $ta $tb
      $ta $tb
      $ta $tb
      $ta $tb
      $ta $tb
    lines
      $outerDist = $nsides*(2.*$radiusAssembly*cos($deltaTheta)); 
      $fact=1.0;  # add a few more lines to account for tangential stretching of grids lines
      $nTheta=int( $fact*$outerDist/$ds+1.5 );
      $nTheta $nr
    mappingName
      assemblyWall
    boundary conditions
      -1 -1 $interfaceBC 0
*wdh      -1 -1 $outerDuctBC 0
    share 
       0 0  $ishare 0 
    exit
* 
* Here is the metal duct that surrounds the assembly
* 
  copy a mapping
   assemblyWall
    n-dist
    fixed normal distance
      -$ductWallThickness
    mappingName
     duct
    boundary conditions
      -1 -1 $interfaceBC $outerDuctBC
    share 
      0 0 $ishare 0 
    exit
* 
rectangle
  set corners
    $xa=-$radiusAssembly+$nDist; $xb=-$xa; 
    $ya=-$radiusAssembly*sin($deltaTheta)+$nDist; $yb=-$ya;
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); $ny=$nx; 
    $nx $ny
  boundary conditions
    0 0 0 0 
*wdh    1 1 1 1 
  mappingName
   backGround
exit
* 
** -------------- make the fundamental stretched annulus  ----------------
$deltaRadius=$ds*5; 
$ishare=$ishare+1; 
$innerRadius=$pinRadius;
$outerRadius=$innerRadius+$deltaRadius; 
$nx=int( (2.*$pi*($innerRadius+$outerRadius)/2.)/$ds+1.5 ); 
$ny=int( ($deltaRadius)/$ds+1.5 ); 
annulus  
  mappingName  
    outerAnnulus-unstretched  
  inner and outer radii  
    $innerRadius $outerRadius 
  centre 
    $xc $yc   
  lines  
    $nx $ny 
  boundary conditions  
    -1 -1 $interfaceBC 0  
  share 
   * material interfaces are marked by share>=100 
    0 0 $ishare 0    
  exit  
* 
stretch coordinates 
  Stretch r2:itanh 
  STP:stretch r2 itanh: layer 0 1. $nStretch 0 (id>=0,weight,exponent,position) 
  stretch grid 
  STRT:name outerAnnulus 
  exit
* 
*  ----------- fundamental inner annulus ------------
$nStretchInner=2.; 
$deltaRadius=$ds*5;
$outerRadius=$pinRadius; 
$innerRadius=$outerRadius-$deltaRadius;
* $nx=int( (2.*$pi*($innerRadius+$outerRadius)/2.)/$ds+1.5 ); 
$ny=int( ($deltaRadius)/$ds+1.5 ); 
annulus  
  mappingName  
    innerAnnulus-unstretched  
  inner and outer radii  
    $innerRadius $outerRadius 
  centre 
    $xc $yc   
  lines  
    $nx $ny 
  boundary conditions  
    -1 -1 0 $interfaceBC
  share 
   * material interfaces are marked by share>=100 
    0 0 0 $ishare     
  exit  
* 
stretch coordinates 
  Stretch r2:itanh 
  STP:stretch r2 itanh: layer 0 1. $nStretchInner 0 (id>=0,weight,exponent,position) 
  stretch grid 
  STRT:name innerAnnulus 
  exit
*
$count=0;  # number the disks as 1,2,3...
$fuelPinGrids=                                 ;
$fluidGrids="";
* $mappingNames="";
*
* ====================================================================
* This function will define a disk centered at (xCenter,yCenter) by
* translating the fundamental disk
* usage:
*   makeDisk(xCenter,yCenter)
* ===================================================================
sub makeDisk\
{ local($xc,$yc)=@_; \
  local $xa, $xb; \
  $count = $count + 1; \
  $outerAnnulus="outerAnnulus$count";     \
  $fluidGrids = $fluidGrids . "   $outerAnnulus\n" ; \
  $innerSquare="innerSquare$count";     \
  $innerAnnulus="innerAnnulus$count";     \
  $fuelPinGrids = $fuelPinGrids . "   $innerSquare\n" . "   $innerAnnulus\n" ; \
  $ishare=$ishare+1; \
  $xa=$xc-$innerRadius+$ds;  $xb=$xc+$innerRadius-$ds; \
  $ya=$yc-$innerRadius+$ds;  $yb=$yc+$innerRadius-$ds; \
  $nx=int( ($xb-$xa)/$ds+1.5 ); \
  $ny=int( ($yb-$ya)/$ds+1.5 ); \
  $makeDiskCommands =  \
  "*\n" . \
  " rotate/scale/shift \n" . \
  "  transform which mapping? \n" . \
  "   outerAnnulus \n" . \
  "   shift \n" . \
  "    $xc $yc \n" . \
  "   share \n" . \
  "    0 0 $ishare 0 \n" . \
  "   mappingName \n" . \
  "    $outerAnnulus \n" . \
  "    exit\n" . \
  "*\n" . \
  "  rectangle \n" . \
  "    mappingName\n" . \
  "      $innerSquare\n" . \
  "    set corners\n" . \
  "     $xa $xb $ya $yb \n" . \
  "    lines\n" . \
  "      $nx $ny\n" . \
  "    boundary conditions\n" . \
  "      0 0 0 0\n" . \
  "    exit \n" . \
  "*\n" . \
  " rotate/scale/shift \n" . \
  "  transform which mapping? \n" . \
  "   innerAnnulus \n" . \
  "   shift \n" . \
  "    $xc $yc \n" . \
  "   share \n" . \
  "    0 0 0 $ishare\n" . \
  "   mappingName \n" . \
  "    $innerAnnulus \n" . \
  "    exit\n"; \
}
*
*
* ===================================================
*   Make an array of cylinders
*     makeDiskArray(nCylx,nCyly,x0,dx0,y0,dy0)
*     
* Make cylinders at centers
*      (x0+i*dx0,y0+j*dy0)  i=0,..,nx0, j=0,..,ny0
* 
* Result: $commands
* =====================================================
sub makeDiskArray \
{ local($nCylx,$nCyly,$x0,$dx0,$y0,$dy0)=@_; \
  $xa=$x0-$npinsb2*$dx0;  \
  for( $j=0; $j<=$npinsb2; $j++ ){ \
    $na=$npins-$j;      \
    for( $i=0; $i<$na; $i++ ){ \
      makeDisk($xa+$i*$dx0,$y0+$j*$dy0); \
      $commands = $commands . $makeDiskCommands; \
    }\
    $xa=$xa+$dx0/2.; \
  }\
  $xa=$x0-$npinsb2*$dx0+$dx0/2.;  \
  for( $j=1; $j<=$npinsb2; $j++ ){ \
    $na=$npins-$j;      \
    for( $i=0; $i<$na; $i++ ){ \
      makeDisk($xa+$i*$dx0,$y0-$j*$dy0); \
      $commands = $commands . $makeDiskCommands; \
    }\
    $xa=$xa+$dx0/2.; \
  }\
}
*
$nCyly=$npins; 
$nCylx=$npins; 
$npinsb2=($npins-1)/2; 
$yPin=-$npinsb2*$pinSpacing;
* 
$rad=$pinRadius;
$deltaX=$pinSpacing;
$deltaY=$pinSpacing*cos($deltaTheta/2.);
$x0=0.; $y0=0.;
makeDiskArray($nCylx,$nCyly,$x0,$deltaX,$y0,$deltaY);
$commands
*
* 
  exit this menu
* 
$fluidGrids = "backGround\n" . "$fluidGrids" . "assemblyWall\n";
*wdh $fluidGrids = "backGround\n" . "$fluidGrids";
* For now include the outer duct with the fuel pins domain
$fuelPinGrids = "duct\n" . $fuelPinGrids;
* 
generate an overlapping grid
* 
  $fluidGrids
  $fuelPinGrids
*  backGround
*  $mappingNames 
*  assemblyWall
  done
*
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      fluidDomain 
      * grids in the domain:
      $fluidGrids
     done
    specify a domain
      * domain name:
      fuelPinDomain 
      * grids in the domain:
      $fuelPinGrids
     done
* 
    order of accuracy
     $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    interpolation type
      $interpType
    exit 
*    display intermediate results
*
* open graphics
    compute overlap
* pause
  exit
save a grid (compressed)
$name
fuelAssembly
exit
