*************************************************************************
*
*       A lattice of 3D hollow cylinders
*
* Usage: 
*         ogen [noplot] cylArray [options]
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
*    ogen noplot cylArray -factor=1 -nCylx=2 -nCyly=2    (creates cylArray2x2yi1.order2.hdf)
*    ogen noplot cylArray -factor=1 -nCylx=2 -nCyly=2 -interp=e -zb=3. -rad=.3    (creates cylArray2x2ye1.order2.hdf)
*    ogen noplot cylArray -factor=1 -nCylx=3 -nCyly=3 -zb=3. -rad=.3   (creates cylArray2x2yi1.order2.hdf)
*    ogen noplot cylArray -factor=1 -nCylx=3 -nCyly=3 -interp=e -zb=3. -rad=.3   (creates cylArray2x2ye1.order2.hdf)
* 
***************************************************************************
$pi=3.1415926535897932384626433;
$factor=1;  $ds0=.05; 
$order=2; $orderOfAccuracy="second order"; $ng=2; 
$nCylx=2; $nCyly=2;   # number of cylinders in each direction
$rad=.25; $nStretch=3.; 
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
$name = "cylArray" . "$nCylx" ."x" . "$nCyly" ."y" . "$interp$factor" . $suffix . ".hdf";
* 
$deltaY=$dist; $deltaX=$dist;   # spacing between cylinder centres
$xb = $nCylx*$deltaX/2.+$rad/2.; $xa=-$xb;
$yb = $nCyly*$deltaY/2.+$rad/2.; $ya=-$yb;
* 
*
***************************************************************************
*
*
* domain parameters:  
$ds = $ds0/$factor; # target grid spacing
*
*
*
create mappings 
*
$count=0;  # number the disks as 1,2,3...
*
* ======================================================
* Define a function to build a stretched-cylinder
* usage:
*   makeDisk(radius,xCenter,yCenter)
* NOTES:
*   $deltaRadius : reduce this for stretching
* =====================================================
sub makeDisk\
{ local($radius,$xc,$yc)=@_; \
  $count = $count + 1; \
  $outerCyl="outerAnnulus$count";     \
  $mappingNames = $mappingNames . "   $outerCyl\n"; \
  $nr = 3+$ng; \
  $deltaRadius=$ds*$nr*.75; \
  $innerRadius=$radius; \
  $outerRadius=$innerRadius+$deltaRadius; \
  $nTheta=int( (2.*$pi*($innerRadius+$outerRadius)*.5)/$ds+1.5 ); \
  $nz=int( ($zb-$za)/$ds+1.5 ); \
  $makeDiskCommands =  \
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
  "      -1 -1 3 4 5 0  \n" . \
  "    share\n" . \
  "      0 0  1 2 4 0  \n" . \
  "    exit \n" . \
  "  stretch coordinates\n" . \
  "    Stretch r3:itanh\n" . \
  "    STP:stretch r3 itanh: layer 0 .5 $nStretch 0 (id>=0,weight,exponent,position)\n" . \
  "    stretch grid\n" . \
  "    STRT:name $outerCyl\n" . \
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
  $mappingNames
  done 
*
  change parameters 
    order of accuracy
     $orderOfAccuracy
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


