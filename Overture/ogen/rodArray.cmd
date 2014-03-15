**************************************************************************************************************
*
*  An array (lattice) of 3D solid cylinders with inner and outer regions 
* 
*   (see also cylArray.cmd for hollow cylinders and 
*     latticeCyl.cmd for solid cylinders but not stretched)
*    
* Usage: 
*         ogen [noplot] rodArray [options]
* where options are
*     -factor=<num>     : grid spacing is .05 divided by this factor
*     -order=[2/4/6/8]  : order of accuracy 
*     -interp=[e/i]     : implicit or explicit interpolation
*     -nCylx=<num>      : number of cylinders in the x direction  
*     -nCyly=<num>      : number of cylinders in the y direction
*     -rad=<num>        : radius of the cylinder
*     -dist=<num>       : distance between centers
*     -zb=<num>         : length of the cylnders in the z-direction (axial)
* 
* Examples: 
*    ogen noplot rodArray -factor=1 -nCylx=1 -nCyly=1 -interp=e  (creates rodArray1x1ye1.order2.hdf)
*    ogen noplot rodArray -factor=1 -nCylx=2 -nCyly=2 -interp=e  (creates rodArray2x2ye1.order2.hdf)
*    ogen noplot rodArray -factor=1 -nCylx=3 -nCyly=3 -interp=e  (creates rodArray3x3ye1.order2.hdf)
*    ogen noplot rodArray -factor=1 -nCylx=2 -nCyly=2 -interp=e -zb=3. -rad=.3    (creates rodArray2x2ye1.order2.hdf)
*    ogen noplot rodArray -factor=1 -nCylx=3 -nCyly=3 -zb=3. -rad=.3   (creates rodArray3x3yi1.order2.hdf)
*    ogen noplot rodArray -factor=1 -nCylx=3 -nCyly=3 -interp=e -zb=3. -rad=.3   (creates rodArray2x2ye1.order2.hdf)
* 
*    ogen noplot rodArray -factor=1 -nCylx=2 -nCyly=2 -interp=e -zb=2. (creates rodArray2x2ye1.order2.hdf)
*    ogen noplot rodArray -factor=2 -nCylx=2 -nCyly=2 -interp=e -zb=2. (creates rodArray2x2ye2.order2.hdf)
*    ogen noplot rodArray -factor=4 -nCylx=2 -nCyly=2 -interp=e -zb=2. (creates rodArray2x2ye4.order2.hdf)
* 
*    ogen noplot rodArray -factor=1 -nCylx=3 -nCyly=3 -interp=e -zb=2. (creates rodArray3x3ye1.order2.hdf)
*    ogen noplot rodArray -factor=2 -nCylx=3 -nCyly=3 -interp=e -zb=2. (creates rodArray3x3ye2.order2.hdf)
*    ogen noplot rodArray -factor=4 -nCylx=3 -nCyly=3 -interp=e -zb=2. (creates rodArray3x3ye4.order2.hdf)
*
* parallel:
*   srun -N1 -n2 -ppdebug ogen noplot -writeCollective rodArray -nCylx=2 -nCyly=2 -interp=e -factor=4
***************************************************************************************************************
$factor=1;  $ds0=.05; 
$order=2; $orderOfAccuracy="second order"; $ng=2; 
$interp="i"; $interpType = "implicit for all grids"; 
$nCylx=2; $nCyly=2;   # number of cylinders in each direction
$rad=.3; $nStretch=3.; 
$dist=1.;
$za=0.; $zb=1.; 
$zc=0.; 
*
* get command line arguments
GetOptions("zb=f"=>\$zb,"rad=f"=>\$rad,"dist=f"=>\$dist,"order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"nCylx=i"=> \$nCylx,"nCyly=i"=> \$nCyly);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
$name = "rodArray" . "$nCylx" ."x" . "$nCyly" ."y" . "$interp$factor" . $suffix . ".hdf";
* 
$deltaY=$dist; $deltaX=$dist;   # spacing between cylinder centres
$xb = $nCylx*$deltaX/2.+$rad*0.; $xa=-$xb;
$yb = $nCyly*$deltaY/2.+$rad*0.; $ya=-$yb;
* 
***************************************************************************
*
*
* domain parameters:  
$ds = $ds0/$factor; # target grid spacing
$pi=4.*atan2(1.,1.);
*
*
*
create mappings 
*
$count=0;  # number the disks as 1,2,3...
*
* ======================================================
* Define a function to build an inner-cylinder, outer-cyl and inner box
* usage:
*   makeDisk(radius,xCenter,yCenter)
* NOTES:
*   $deltaRadius : reduce this for stretching
*   The bc for all interfaces is 100
*   The share value for interfaces is $ishare (which differs for different interfaces)
* =====================================================
sub makeDisk\
{ local($radius,$xc,$yc)=@_; \
  $count = $count + 1; \
  $outerCyl="outerCylinder$count";     \
  $outerDomainGrids = $outerDomainGrids . "   $outerCyl\n"; \
  $nr = 3+$ng; \
  $deltaRadius=$ds*$nr*.75; \
  $innerRadius=$radius; \
  $outerRadius=$innerRadius+$deltaRadius; \
  $nTheta=int( (2.*$pi*$innerRadius)/$ds+1.5 ); \
  $nz=int( ($zb-$za)/$ds+1.5 ); \
  $ishare=100+$count; \
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
  "      -1 -1 3 4 100 0  \n" . \
  "    share\n" . \
  "      0 0  1 2  $ishare 0  \n" . \
  "    exit \n" . \
  "  stretch coordinates\n" . \
  "    Stretch r3:itanh\n" . \
  "    STP:stretch r3 itanh: layer 0 .5 $nStretch 0 (id>=0,weight,exponent,position)\n" . \
  "    stretch grid\n" . \
  "    STRT:name $outerCyl\n" . \
  "    exit\n"; \
  $innerBox="innerBox$count";     \
  $innerDomainGrids = $innerDomainGrids . "   $innerBox\n"; \
  $innerCyl="innerCylinder$count";     \
  $innerDomainGrids = $innerDomainGrids . "   $innerCyl\n"; \
  $nr = 3+$ng; \
  $deltaRadius=$ds*$nr*.75; \
  $outerRadius=$radius; \
  $innerRadius=$outerRadius-$deltaRadius; \
  $xai=$xc-$innerRadius;  $xbi=$xc+$innerRadius; \
  $yai=$yc-$innerRadius;  $ybi=$yc+$innerRadius; \
  $nxi=int( ($xbi-$xai)/$ds+1.5 ); \
  $nyi=int( ($ybi-$yai)/$ds+1.5 ); \
  $makeDiskCommands = $makeDiskCommands . \
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
  $makeDiskCommands = $makeDiskCommands . \
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
  "      -1 -1 3 4 0 100  \n" . \
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
* Result: $commands
* =====================================================
$commands="";
sub makeDiskArray \
{ local($radius,$nCylx,$nCyly,$x0,$dx0,$y0,$dy0)=@_; \
  for( $j=0; $j<$nCyly; $j++ ){ \
  for( $i=0; $i<$nCylx; $i++ ){ \
    makeDisk($radius,$x0+$i*$dx0,$y0+$j*$dy0); \
    $commands = $commands . $makeDiskCommands; \
  }}\
}
*
$x0=-($nCylx-1)*$deltaX*.5; $y0=-($nCyly-1)*$deltaY*.5;  
makeDiskArray($rad,$nCylx,$nCyly,$x0,$deltaX,$y0,$deltaY);
$commands
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
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      1 1 2 2 3 4
    share
      0 0 0 0 1 2
    exit 
*
  exit this menu 
*
generate an overlapping grid 
  backGround
  $outerDomainGrids
  $innerDomainGrids
  done 
*
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
      $outerDomainGrids
      done
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
      $innerDomainGrids
      done
* 
    order of accuracy
     $orderOfAccuracy
* 
    interpolation type
      $interpType
*
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit 
*    display intermediate results
* pause
*
    compute overlap
* pause
  exit
save a grid (compressed)
$name
cylArray
exit


