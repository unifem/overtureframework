*
* 8 small cylinders in a disk -- for a converging shock calculation
*
* $factor=.5; $grid="convOct0.hdf"; 
$factor=2; $grid="convOct.hdf"; 
* $factor=4; $grid="convOct2.hdf"; 
*
* target grid spacing:
$ds=1./$factor;
*
$xbox=60.;
$outerRad=75.;
$pi=3.141592653;
create mappings
*
  rectangle
    set corners
    -$xbox $xbox -$xbox $xbox
    mapping parameters
    mappingName background
    $n=int(2*$xbox/$ds+1.5);
    lines $n $n
    Boundary Condition: left    0
    Boundary Condition: right   0
    Boundary Condition: bottom  0
    Boundary Condition: top     0
    close mapping dialog
    exit
*
  annulus
    centre for annulus
    0. 0.
    inner and outer radii
    $innerRad=$xbox-2*$ds;
    $innerRad $outerRad
    lines
    $nt=int(2*$pi*$innerRad/$ds+1.5);
    $nr=int(($outerRad-$innerRad)/$ds+1.5);
    $nt $nr
    boundary conditions
    -1 -1 0 1
    mappingName
    perimeter
    exit
*
  annulus
    centre for annulus
    $c=0.92387953251129;
    $s=0.38268343236509;
    $rc=43.5;
    $xc1=$rc*$c;
    $yc1=$rc*$s;
    $xc1 $yc1
    inner and outer radii
    $smallRad=7.5;
    $smallOuterRad=$smallRad+4*$ds;
    $smallRad $smallOuterRad
    lines
    $nrs=int(2*$pi*$smallOuterRad/$ds+1.5);
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl1
    exit
*
  annulus
    centre for annulus
    $xc2=$rc*$s;
    $yc2=$rc*$c;
    $xc2 $yc2
    inner and outer radii
    $smallRad $smallOuterRad
    lines
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl2
    exit
*
  annulus
    centre for annulus
    $xc=-$rc*$s;
    $yc=$rc*$c;
    $xc $yc
    inner and outer radii
    $smallRad $smallOuterRad
    lines
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl3
    exit
*
  annulus
    centre for annulus
    $xc=-$rc*$c;
    $yc=$rc*$s;
    $xc $yc
    inner and outer radii
    $smallRad $smallOuterRad
    lines
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl4
    exit
*
  annulus
    centre for annulus
    $xc=-$rc*$c;
    $yc=-$rc*$s;
    $xc $yc
    inner and outer radii
    $smallRad $smallOuterRad
    lines
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl5
    exit
*
  annulus
    centre for annulus
    $xc=-$rc*$s;
    $yc=-$rc*$c;
    $xc $yc
    inner and outer radii
    $smallRad $smallOuterRad
    lines
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl6
    exit
*
  annulus
    centre for annulus
    $xc=$rc*$s;
    $yc=-$rc*$c;
    $xc $yc
    inner and outer radii
    $smallRad $smallOuterRad
    lines
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl7
    exit
*
  annulus
    centre for annulus
    $xc=$rc*$c;
    $yc=-$rc*$s;
    $xc $yc
    inner and outer radii
    $smallRad $smallOuterRad
    lines
    $nrs 5
    boundary conditions
    -1 -1 1 0
    mappingName
    cyl8
    exit
  exit this menu
generate an overlapping grid
  background
  perimeter
  cyl1
  cyl2
  cyl3
  cyl4
  cyl5
  cyl6
  cyl7
  cyl8
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
    exit
  compute overlap
exit
*
save an overlapping grid
  $grid
  convOct
exit

