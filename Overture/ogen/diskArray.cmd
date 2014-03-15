*
*  A lattice of disks with grids inside and outside -- for flow past heated cylinders
*
* usage: 
* ogen [noplot] diskArray -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nCylx=<num> -nCyly=<num> -rad=<num> -dist=<num>
* 
*   nCylx : number of cylinders in the x direction
*   nCyly : number of cylinders in the y direction
*   rad   : radius of the cylinder
*   dist  : distance between centers
* 
* examples:
*     ogen noplot diskArray -factor=1 -order=2 -nCylx=3 -nCyly=3
* 
*     ogen noplot diskArray -factor=1 -interp=e -nCylx=2 -nCyly=2    (creates diskArray2x2ye1.order2.hdf)
*     ogen noplot diskArray -factor=2 -interp=e -nCylx=2 -nCyly=2    (creates diskArray2x2ye2.order2.hdf)
*     ogen noplot diskArray -factor=4 -interp=e -nCylx=2 -nCyly=2    (creates diskArray2x2ye4.order2.hdf)
* 
*     ogen noplot diskArray -factor=1 -interp=e -order=2 -nCylx=5 -nCyly=5 -rad=.4  (diskArray5x5ye1.order2.hdf)
*     ogen noplot diskArray -factor=2 -interp=e -order=2 -nCylx=5 -nCyly=5 -rad=.4  (diskArray5x5ye2.order2.hdf)
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$nCylx=2; $nCyly=2;       # number of cylinders in each direction
$rad=.3;                  # radius of each cylinder
$dist=1.;                 # spacing between cylinder centres
$bc="1 1 1 1"; # for the backGround grid
* 
* get command line arguments
GetOptions( "rad=f"=>\$rad,"dist=f"=>\$dist,"order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"nCylx=i"=> \$nCylx,"nCyly=i"=> \$nCyly);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
$name = "diskArray" . "$nCylx" ."x" . "$nCyly" ."y" . "$interp$factor" . $suffix . ".hdf";
* 
$deltaY=$dist; $deltaX=$dist;   # spacing between cylinder centres
* 
*
* -- back-ground square: 
$rxa=-($nCylx/2.+.5); $rxb=-$rxa;
$rya=-($nCyly/2.+.5); $ryb=-$rya;
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
$ds = .025/$factor; # target grid spacing
$pi=3.141592653;
$bcInterface=100;  # bc for interfaces
*
*
create mappings 
*
$count=0;  # number the disks as 1,2,3...
*
*
* ======================================================
* Define a function to build an inner-annulus, outer-annulus and inner square.
* usage:
*   makeDisk(radius,xCenter,yCenter)
* =====================================================
sub makeDisk\
{ local($radius,$xc,$yc)=@_; \
  $count = $count + 1; \
  $innerAnnulus="innerAnnulus$count";     \
  $innerSquare="innerSquare$count";     \
  $outerAnnulus="outerAnnulus$count";     \
  $innerMappingNames = $innerMappingNames . "   $innerSquare\n". "   $innerAnnulus\n"; \
  $outerMappingNames = $outerMappingNames . "   $outerAnnulus\n"; \
  $nr = 3+$ng; \
  $deltaRadius=$ds*$nr; \
  $outerRadius=$radius; $innerRadius=$outerRadius-$deltaRadius; \
  $nx=int( (2.*$pi*$outerRadius)/$ds+1.5 ); \
  $ny=int( ($deltaRadius)/$ds+1.5 ); \
  $nTheta=$nx; \
  $share=100+$count; \
  $commands = \
  "  annulus \n" . \
  "    mappingName \n" . \
  "      $innerAnnulus \n" . \
  "    centre\n" . \
  "     $xc $yc\n" .   \
  "    boundary conditions \n" . \
  "      -1 -1 0 $bcInterface \n" . \
  "    share\n" . \
  "     * material interfaces are marked by share>=100\n" . \
  "      0 0 0 $share \n" . \
  "    inner and outer radii \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    lines \n" . \
  "      $nx $ny \n" . \
  "    exit \n"; \
  $xa=$xc-$innerRadius;  $xb=$xc+$innerRadius; \
  $ya=$yc-$innerRadius;  $yb=$yc+$innerRadius; \
  $nx=int( ($xb-$xa)/$ds+1.5 ); \
  $ny=int( ($yb-$ya)/$ds+1.5 ); \
  $commands = $commands . \
  "  rectangle \n" . \
  "    mappingName\n" . \
  "      $innerSquare\n" . \
  "    set corners\n" . \
  "     $xa $xb $ya $yb \n" . \
  "    lines\n" . \
  "      $nx $ny\n" . \
  "    boundary conditions\n" . \
  "      0 0 0 0\n" . \
  "    exit \n"; \
  $innerRadius=$outerRadius; \
  $outerRadius=$innerRadius+$deltaRadius; \
  $nx=$nTheta; \
  $ny=int( ($deltaRadius)/$ds+1.5 ); \
  $commands = $commands . \
  "*\n" . \
  "  annulus \n" . \
  "    mappingName \n" . \
  "      $outerAnnulus \n" . \
  "    inner and outer radii \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    centre\n" . \
  "      $xc $yc\n" .   \
  "    lines \n" . \
  "      $nx $ny\n" . \
  "    boundary conditions \n" . \
  "      -1 -1 $bcInterface 0 \n" . \
  "    share\n" . \
  "     * material interfaces are marked by share>=100\n" . \
  "      0 0 $share 0   \n" . \
  "    exit \n"; \
}
* ========================================================
*
*
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
  local $cmds; $cmds=""; \
  for( $j=0; $j<$nCyly; $j++ ){ \
  for( $i=0; $i<$nCylx; $i++ ){ \
    makeDisk($radius,$x0+$i*$dx0,$y0+$j*$dy0); \
    $cmds = $cmds . $commands; \
  }}\
  $commands=$cmds; \
}
*
* 
$x0=-($nCylx-1)*$deltaX*.5; $y0=-($nCyly-1)*$deltaY*.5;  
makeDiskArray($rad,$nCylx,$nCyly,$x0,$deltaX,$y0,$deltaY);
$commands
* 
*
  rectangle 
    mappingName
      backGround
    $outerMappingNames = "backGround\n" . $outerMappingNames;
    set corners
     $rxa $rxb $rya $ryb 
    lines
      $nx=int( ($rxb-$rxa)/$ds+1.5 );
      $ny=int( ($ryb-$rya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      $bc
    exit 
*
  exit this menu 
*
generate an overlapping grid 
  backGround
  $mappingNames= $innerMappingNames . $outerMappingNames;
  $mappingNames
  done 
*
  change parameters 
*   We must prevent hole cutting and interpolation between
*   the inner and outer grids.  
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
      $outerMappingNames
      done
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
      $innerMappingNames
      done
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit 
*    display intermediate results
* pause
*
* 
    compute overlap
* pause
  exit
save a grid (compressed)
$name
diskArray
exit
