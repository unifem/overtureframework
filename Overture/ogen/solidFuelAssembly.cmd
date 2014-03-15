*************************************************************************
*
*   A lattice of 3D solid cylinders (inside and outside) in a hexgonal container
*
* Usage: 
*         ogen [noplot] solidFuelAssembly [options]
* where options are
*     -factor=<num>     : grid spacing is .05 divided by this factor
*     -order=[2/4/6/8]  : order of accuracy 
*     -interp=[e/i]     : implicit or explicit interpolation
*     -npins=<num>      : number of pins across the center horizontal diameter 
*     -zb=<num>         : length of the cylnders in the z-direction (axial)
*     -name=<string>    : over-ride the default name  
* 
* Examples: 
*
*    ogen noplot solidFuelAssembly.cmd -npins=1 -factor=1 -zb=.5 -interp=e 
*    ogen noplot solidFuelAssembly.cmd -npins=1 -factor=1.5 -interp=e -zb=.5  (solidFuelAssembly1pinse1.5.order2.hdf)
* 
*    ogen noplot solidFuelAssembly.cmd -npins=1 -factor=1 -zb=2 -interp=e -name="solidFuelAssembly1pinsl2e1.hdf"
* 
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1 -zb=.5     ->solidFuelAssembly3pinsi1.order2.hdf
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1 -zb=.5 -interp=e
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1.5 -interp=e -zb=.5      (solidFuelAssembly3pinse1.5.order2.hdf)
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=2 -zb=1 -interp=e -name="solidFuelAssembly3pinsl2e2.hdf"
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=2 -zb=4 -interp=e -name="solidFuelAssembly3pinsl4e2.hdf"
* 
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1 -zb=2 -interp=e -name="solidFuelAssembly3pinsl2e1.hdf"
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1 -zb=4 -interp=e -name="solidFuelAssembly3pinsl4e1.hdf"
* 
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1 -zb=5 -interp=e -name="solidFuelAssembly3pinsl5e1.hdf"
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=2 -zb=5 -interp=e -name="solidFuelAssembly3pinsl5e2.hdf"
* 
*    ogen noplot solidFuelAssembly.cmd -npins=5 -factor=1 -zb=.5    
*    ogen noplot solidFuelAssembly.cmd -npins=5 -factor=2 -zb=.5    
*      30M pts, 2.6G memory: 
*    ogen noplot solidFuelAssembly.cmd -npins=5 -factor=2 -zb=5.  -name="solidFuelAssembly5pinsi2z5.hdf"
* 
*    ogen noplot solidFuelAssembly -factor=1 -npins=1 -name="temp.hdf"  ( -> solidFuelAssembly1pinsi1.order2.hdf)
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1 -zb=2     ->solidFuelAssembly3pinsi1.order2.hdf
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=1 -zb=10 -name="solidFuelAssembly3pinsl10i1.hdf"
*    ogen noplot solidFuelAssembly.cmd -npins=3 -factor=2 -zb=5   ( -> solidFuelAssembly3pins2i.order2.hdf )
* 
*    mpirun -np 2 ogen noplot solidFuelAssembly.cmd -factor=2 -npins=3 -interp=e
*    srun -N1 -n2 -ppdebug ogen noplot solidFuelAssembly.cmd -factor=2 -npins=3 -zb=2
*
***************************************************************************
$pi=4.*atan2(1.,1.); # 3.1415926535897932384626433;
$factor=1;  $ds0=.05; 
$order=2; $orderOfAccuracy="second order"; $ng=2; 
$interp="i"; $interpType = "implicit for all grids"; 
* 
$npins=5;  # number of pins along the diameter
$pinRadius=.5; $pinDiameter=2.*$pinRadius; 
$nStretch=3.; # stretching near the cylinder in the fluid domain
$helixHeight = 10.*$pinRadius; # 1e8; # 10.;  # helix rotates once over this vertical distance 
$pinGap=.3; # gap between pins
$containerWidth=.15;  # width of the outer container
$dist=1.;
$za=0.; $zb=1.; $zc=0.; 
$name=""; 
*
* get command line arguments
GetOptions("name=s"=> \$name,"zb=f"=>\$zb,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"npins=i"=> \$npins);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "solidFuelAssembly" . "$npins" ."pins" . "$interp$factor" . $suffix . ".hdf";}
* 
* domain parameters:  
$ds = $ds0/$factor; # target grid spacing
* 
$nsides=6; # hexagon
$deltaTheta=2.*$pi/($nsides);
* $outerGap: gap between last pin and the assembly wall  -- add a bit extra since wall curves
$outerGap=($pinRadius+$pinGap)/cos($deltaTheta/2.)-$pinRadius + 2.*$ds;
$pinSpacing=$pinDiameter+$pinGap; 
$radiusAssembly=($npins-1)*($pinSpacing/2.)+$pinRadius+$outerGap;
* 
***************************************************************************
$wallBC=2; $inflowBC=3; $outflowBC=4; 
$containerBC=5;        # bc at the outer wall of the metal container 
$ductBC=$wallBC;       # bc at interface between the fluid duct and the outer container
$rodBC=$wallBC;        # bc at the interface between rods and the fluid
$ishare=100;           # share value for interfaces start at 100
*
$nsides=6; # hexagon
$theta=0.;
*
*
create mappings 
*
* ----------------------  assembly wall -----------------------------------
  smoothedPolygon
    vertices
     $nv=$nsides+2;
     $nv
     $theta=0.; 
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
      * increase sharpness with number of pins since polygon gets larger ?
*       $sharp=100.+$factor*100.; 
      $sharp=$factor*30 +($npins-1)*20.; 
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
      $nr=7; 
      $nDist=$ds*($nr-3); 
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
      $fact=1.1;  # add a few more lines to account for tangential stretching of grids lines
      $nTheta=int( $fact*$outerDist/$ds+1.5 );
      $nTheta $nr
    mappingName
      assemblyWall2d
    boundary conditions
      -1 -1 1 0
    exit
* 
  sweep
    extrude
      $za $zb
    choose reference mapping
    assemblyWall
    lines
      $nz=int( ($zb-$za)/$ds+1.5 ); 
      $nTheta $nr $nz
    boundary conditions
      -1 -1 $ductBC 0 $inflowBC $outflowBC
    share
       0  0 $ishare 0 $inflowBC $outflowBC
    mappingName
      assemblyWall
    exit
* 
* ----------------------  outer hexagonal container -------------------------------
  smoothedPolygon
    vertices
     $nv=$nsides+2;
     $nv
     $theta=0.; 
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
      * increase sharpness with number of pins since polygon gets larger ?
*       $sharp=100.+$factor*100.; 
      $sharp=$factor*30 +($npins-1)*20.; 
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
      $nr=7; 
      $nDist=-$containerWidth;
      $nDist
    n-stretch
      1 2. 0
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
      $fact=1.1;  # add a few more lines to account for tangential stretching of grids lines
      $nTheta=int( $fact*$outerDist/$ds+1.5 );
      $nTheta $nr
    mappingName
      container2d
    boundary conditions
      -1 -1 1 0
    exit
* 
  sweep
    extrude
      $za $zb
    choose reference mapping
      container2d
    lines
      $nz=int( ($zb-$za)/$ds+1.5 ); 
      $nTheta $nr $nz
    boundary conditions
      -1 -1 $ductBC $containerBC $inflowBC $outflowBC
    share
       0  0 $ishare      0      $inflowBC $outflowBC
    mappingName
      container
    exit
* 
*
*
$count=0;  # number the disks as 1,2,3...
$fluidDomainGrids = "backGround\n assemblyWall\n"; 
$rodDomainGrids ="";
*
* ======================================================
* Define a function to build a stretched-cylinder
* usage:
*   makeDisk(radius,xCenter,yCenter)
* NOTES:
*   $deltaRadius : reduce this for stretching
* Result: $makeDiskCommands
* =====================================================
sub makePins\
{ local($radius,$xc,$yc)=@_; \
  $count = $count + 1; \
  $outerCyl="outerAnnulus$count";     \
  $ishare = $ishare + 1;     \
  $fluidDomainGrids .= "   $outerCyl\n"; \
  $nr = 6+$ng; \
  $deltaRadius=$ds*$nr*.45; \
  $innerRadius=$radius; \
  $outerRadius=$innerRadius+$deltaRadius; \
  $nTheta=int( (2.*$pi*($innerRadius+$outerRadius)*.5)/$ds+1.5 ); \
  $nz=int( ($zb-$za)/$ds+1.5 ); \
  $makeDiskCommands = \
  "*\n" . \
  "  cylinder \n" . \
  "    mappingName \n" . \
  "      $outerCyl-unstretched \n" . \
  "    centre for cylinder\n" . \
  "      $xc $yc $zc\n" .   \
  "    bounds on the axial variable \n" . \
  "      $za $zb                \n" . \
  "    bounds on the radial variable \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    lines \n" . \
  "      $nTheta $nz $nr\n" . \
  "    boundary conditions \n" . \
  "      -1 -1 $inflowBC $outflowBC  $rodBC 0  \n" . \
  "    share\n" . \
  "      0 0 $inflowBC $outflowBC $ishare 0  \n" . \
  "    exit \n" . \
  "  stretch coordinates\n" . \
  "    Stretch r3:itanh\n" . \
  "    STP:stretch r3 itanh: layer 0 1. $nStretch 0 (id>=0,weight,exponent,position)\n" . \
  "    stretch grid\n" . \
  "    STRT:name $outerCyl\n" . \
  "    exit\n"; \
  $innerBox="innerBox$count";     \
  $rodDomainGrids .= "   $innerBox\n"; \
  $innerCyl="innerCylinder$count";     \
  $rodDomainGrids .= "   $innerCyl\n"; \
  $nr = 3+$ng; \
  $deltaRadius=$ds*$nr*.75; \
  $outerRadius=$radius; \
  $innerRadius=$outerRadius-$deltaRadius; \
  $xai=$xc-$innerRadius;  $xbi=$xc+$innerRadius; \
  $yai=$yc-$innerRadius;  $ybi=$yc+$innerRadius; \
  $nxi=int( ($xbi-$xai)/$ds+1.5 ); \
  $nyi=int( ($ybi-$yai)/$ds+1.5 ); \
  $makeDiskCommands .=  \
  "  box \n" . \
  "    mappingName\n" . \
  "      $innerBox\n" . \
  "    set corners\n" . \
  "     $xai $xbi $yai $ybi $za $zb  \n" . \
  "    lines\n" . \
  "      $nxi $nyi $nz\n" . \
  "    boundary conditions\n" . \
  "      0 0 0 0 3 4 \n" . \
  "    share\n" . \
  "      0 0 0 0 1 2 \n" . \
  "    exit \n"; \
  $makeDiskCommands .=  \
  "*\n" . \
  "  cylinder \n" . \
  "    mappingName \n" . \
  "      $innerCyl-unstretched \n" . \
  "    centre for cylinder\n" . \
  "      $xc $yc $zc\n" .   \
  "    bounds on the axial variable \n" . \
  "      $za $zb                \n" . \
  "    bounds on the radial variable \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    lines \n" . \
  "      $nTheta $nz $nr\n" . \
  "    boundary conditions \n" . \
  "      -1 -1 3 4 0 $rodBC  \n" . \
  "    share\n" . \
  "      0 0  1 2 0 $ishare \n" . \
  "    exit \n" . \
  "  stretch coordinates\n" . \
  "    Stretch r3:itanh\n" . \
  "    STP:stretch r3 itanh: layer 0 .5 $nStretch 1. (id>=0,weight,exponent,position)\n" . \
  "    stretch grid\n" . \
  "    STRT:name $innerCyl\n" . \
  "    exit\n"; \
}
* ===================================================
*   Make an array of cylinders
*     makeDiskArray(radius,nCylx,nCyly,x0,dx0,y0,dy0)
*     
* Make cylinders at centers
*      (x0+i*dx0,y0+j*dy0)  i=0,..,nx0, j=0,..,ny0
* 
* Result: $makePinArrayCommands
* =====================================================
sub makePinArray \
{ local($radius,$nCylx,$nCyly,$x0,$dx0,$y0,$dy0)=@_; \
  local $cmds; $cmds=""; \
  $xa=$x0-$npinsb2*$dx0;  \
  $makePinArrayCommands=""; \
  for( $j=0; $j<=$npinsb2; $j++ ){ \
    $na=$npins-$j;      \
    for( $i=0; $i<$na; $i++ ){ \
      makePins($radius,$xa+$i*$dx0,$y0+$j*$dy0); \
      $makePinArrayCommands = $makePinArrayCommands . $makeDiskCommands; \
    }\
    $xa=$xa+$dx0/2.; \
  }\
  $xa=$x0-$npinsb2*$dx0+$dx0/2.;  \
  for( $j=1; $j<=$npinsb2; $j++ ){ \
    $na=$npins-$j;      \
    for( $i=0; $i<$na; $i++ ){ \
      makePins($radius,$xa+$i*$dx0,$y0-$j*$dy0); \
      $makePinArrayCommands = $makePinArrayCommands . $makeDiskCommands; \
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
makePinArray($pinRadius,$nCylx,$nCyly,$x0,$deltaX,$y0,$deltaY);
$makePinArrayCommands
*
*open graphics
*view mappings
* choose all
*
*
*
  box
    mappingName
      backGround
    set corners
      * $nDist=-.2; 
      $deltab = $nDist - $ds*$ng;
      $xa=-$radiusAssembly+$deltab; $xb=-$xa; 
      $ya=-$radiusAssembly*sin($deltaTheta)+$deltab; $yb=-$ya;
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      0 0 0 0 $inflowBC $outflowBC
    share
      0 0 0 0 $inflowBC $outflowBC
    exit 
*
  exit this menu 
*
* $fluidDomainGrids = "backGround\n assemblyWall\n $fluidDomainGrids";
generate an overlapping grid 
  $fluidDomainGrids
  $rodDomainGrids
  container
  done 
*
  change parameters 
    specify a domain
      * domain name:
      fluidDomain 
      * grids in the domain:
      $fluidDomainGrids
      done
    specify a domain
      * domain name:
      rodDomain 
      * grids in the domain:
      $rodDomainGrids
      done
    specify a domain
      * domain name:
      hexagonalContainer
      * grids in the domain:
      container
      done
    order of accuracy
     $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    interpolation type
      $interpType
*     interpolation width
*       all
*       all
*       2 2 2 
    exit 
*    display intermediate results
* pause
*
*  open graphics
*
  compute overlap
*
*
*  query a point
*    interpolate point 1
*    pt: grid,i1,i2,i3: 1 15 3 1
*
* pause
  exit
maximum number of parallel sub-files
  8 
save a grid (compressed)
$name
fuelAssembly
exit


