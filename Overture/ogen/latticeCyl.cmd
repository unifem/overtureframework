**************************************************************************
*
*  A lattice of 3D cylinders with inner and outer regions 
*
***************************************************************************
$orderOfAccuracy = "second order";
$nCylx=8; $nCyly=8;   # number of cylinders in each direction
*
* scale number of grid points in each direction by $factor
$factor=1; $name = "latticeCyl.hdf";
* $factor=1.5;  $name = "latticeCyl2.hdf"; 
* $factor=4; $name = "latticeCyl4.hdf"; 
* $factor=8; $name = "latticeCyl8.hdf"; 
* $factor=16; $name = "latticeCyl16.hdf"; 
*
* -- fourth-order accurate ---
* $factor=.5; $name = "latticeCyl0.order4.hdf";  $orderOfAccuracy = "fourth order"; $bc = "1 2 1 1";
* $factor=1; $name = "latticeCyl1.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=2; $name = "latticeCyl2.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=4; $name = "latticeCyl4.order4.hdf";  $orderOfAccuracy = "fourth order";
* $factor=8; $name = "latticeCyl8.order4.hdf";  $orderOfAccuracy = "fourth order";
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
$ds = .05/$factor; # target grid spacing
$pi=3.141592653;
*
$xa=-5.5; $xb=5.5;  $ya=-5.5; $yb=5.5; 
$xa=-2.5; $xb=2.5; $ya=-2.5; $yb=2.5; 
$xa=-4.5; $xb=4.5;  $ya=-4.5; $yb=4.5; 
$za=0.; $zb=2.; 
$zc=0.; 
*
*
create mappings 
*
$count=0;  # number the disks as 1,2,3...
*
* ======================================================
* Define a function to build an inner-cylinder, outer-cylinder and inner box.
* usage:
*   makeDisk(radius,xCenter,yCenter)
* =====================================================
sub makeDisk\
{ local($radius,$xc,$yc)=@_; \
  $count = $count + 1; \
  $innerAnnulus="innerAnnulus$count";     \
  $innerSquare="innerSquare$count";     \
  $outerAnnulus="outerAnnulus$count";     \
  $outerSquare="outerSquare$count";     \
  $mappingNames = $mappingNames . "   $outerAnnulus\n" . "   $innerSquare\n". "   $innerAnnulus\n"; \
  $allowHoleCutting = $allowHoleCutting . "   $outerAnnulus\n". "   backGround\n" . "   $innerAnnulus\n". "   $innerSquare\n"; \
  $allowInterpolation = $allowInterpolation . " $innerAnnulus\n" . " $innerSquare\n" . " $innerSquare\n" . " $innerAnnulus\n" . " $outerAnnulus\n" . " backGround\n" . " backGround\n" . " $outerAnnulus\n"; \
  $deltaRadius=$radius*.75/$factor; \
  $outerRadius=$radius; $innerRadius=$outerRadius-$deltaRadius; \
  $nx=int( (2.*$pi*$outerRadius)/$ds+1.5 ); \
  $ny=int( ($deltaRadius)/$ds+1.5 ); \
  $nz=int( ($zb-$za)/$ds+1.5 ); \
  $nTheta=$nx; \
  $share=100+$count; \
  $makeDiskCommands = \
  "  cylinder \n" . \
  "    mappingName \n" . \
  "      $innerAnnulus \n" . \
  "    centre for cylinder\n" . \
  "     $xc $yc $zc\n" .   \
  "    bounds on the axial variable \n" . \
  "      $za $zb                \n" . \
  "    bounds on the radial variable \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    lines \n" . \
  "      $nx $nz $ny  \n" . \
  "    boundary conditions \n" . \
  "      -1 -1 3 4 0 3  \n" . \
  "    share\n" . \
  "     * material interfaces are marked by share>=100\n" . \
  "      0 0 1 2 0 $share \n" . \
  "    exit \n"; \
  $xai=$xc-$innerRadius;  $xbi=$xc+$innerRadius; \
  $yai=$yc-$innerRadius;  $ybi=$yc+$innerRadius; \
  $nx=int( ($xbi-$xai)/$ds+1.5 ); \
  $ny=int( ($ybi-$yai)/$ds+1.5 ); \
  $makeDiskCommands = $makeDiskCommands . \
  "  box \n" . \
  "    mappingName\n" . \
  "      $innerSquare\n" . \
  "    set corners\n" . \
  "     $xai $xbi $yai $ybi $za $zb  \n" . \
  "    lines\n" . \
  "      $nx $ny $nz\n" . \
  "    boundary conditions\n" . \
  "      0 0 0 0 3 4 \n" . \
  "    share\n" . \
  "      0 0 0 0 1 2 \n" . \
  "    exit \n"; \
  $innerRadius=$outerRadius; \
  $outerRadius=$innerRadius+$deltaRadius; \
  $nx=$nTheta; \
  $ny=int( ($deltaRadius)/$ds+1.5 ); \
  $makeDiskCommands = $makeDiskCommands . \
  "*\n" . \
  "  cylinder \n" . \
  "    mappingName \n" . \
  "      $outerAnnulus \n" . \
  "    centre for cylinder\n" . \
  "      $xc $yc $zc\n" .   \
  "    bounds on the axial variable \n" . \
  "      $za $zb                \n" . \
  "    bounds on the radial variable \n" . \
  "      $innerRadius $outerRadius\n" . \
  "    lines \n" . \
  "      $nx $nz $ny\n" . \
  "    boundary conditions \n" . \
  "      -1 -1  1 2 3 0 \n" . \
  "    share\n" . \
  "     * material interfaces are marked by share>=100\n" . \
  "      0 0  1 2 $share 0  \n" . \
  "    exit \n"; \
}
* ========================================================
*
  $rad=.25; $deltaX=1.;  $deltaY=1.;
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
*   We must prevent hole cutting and interpolation between
*   the inner and outer grids.  
    prevent hole cutting
      all
      all
    done
    allow hole cutting
      $allowHoleCutting
    done
    prevent interpolation
      all
      all
    done
    allow interpolation 
      $allowInterpolation
    done
    ghost points 
      all 
      2 2 2 2 
    order of accuracy
     $orderOfAccuracy
    exit 
*    display intermediate results
* pause
*
    compute overlap
* pause
  exit
save a grid (compressed)
$name
latticeCyl
exit






* --------------------------------------------------
  $yDisk=-4;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,0.0,$yDisk);
*-   $commands
*-   makeDisk($rad,1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
  $yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,0.0,$yDisk);
*-   $commands
*-   makeDisk($rad,1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
  $yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
 makeDisk($rad,-2.0,$yDisk);
 $commands
 makeDisk($rad,-1.0,$yDisk);
 $commands
 makeDisk($rad,0.0,$yDisk);
 $commands
 makeDisk($rad,1.0,$yDisk);
 $commands
 makeDisk($rad,2.0,$yDisk);
 $commands
*- makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
 $yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
 makeDisk($rad,-2.0,$yDisk);
 $commands
 makeDisk($rad,-1.0,$yDisk);
 $commands
 makeDisk($rad,0.0,$yDisk);
 $commands
 makeDisk($rad,1.0,$yDisk);
 $commands
 makeDisk($rad,2.0,$yDisk);
 $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
 $yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
 makeDisk($rad,-2.0,$yDisk);
 $commands
 makeDisk($rad,-1.0,$yDisk);
 $commands
 makeDisk($rad,0.0,$yDisk);
 $commands
 makeDisk($rad,1.0,$yDisk);
 $commands
 makeDisk($rad,2.0,$yDisk);
 $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
$yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
 makeDisk($rad,-2.0,$yDisk);
 $commands
 makeDisk($rad,-1.0,$yDisk);
 $commands
 makeDisk($rad,0.0,$yDisk);
 $commands
 makeDisk($rad,1.0,$yDisk);
 $commands
 makeDisk($rad,2.0,$yDisk);
 $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
$yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
 makeDisk($rad,-2.0,$yDisk);
 $commands
 makeDisk($rad,-1.0,$yDisk);
 $commands
 makeDisk($rad,0.0,$yDisk);
 $commands
 makeDisk($rad,1.0,$yDisk);
 $commands
 makeDisk($rad,2.0,$yDisk);
 $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
*= $yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,0.0,$yDisk);
*-   $commands
*-   makeDisk($rad,1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
*- *
*= $yDisk=$yDisk+$deltaY;
*-   makeDisk($rad,-4.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,-1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,0.0,$yDisk);
*-   $commands
*-   makeDisk($rad,1.0,$yDisk);
*-   $commands
*-   makeDisk($rad,2.0,$yDisk);
*-   $commands
*-   makeDisk($rad,3.0,$yDisk);
*-   $commands
*-   makeDisk($rad,4.0,$yDisk);
*-   $commands
