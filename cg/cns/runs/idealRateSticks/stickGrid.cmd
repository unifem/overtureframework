$xa = 0; 
$xb = 6; 
$ya = 0; 
$yb = 20;
*
$N = 320;
*$Nx = $N*($xb-$xa)+1;
*$Ny = $N*($yb-$ya)+1;
$Ny = $N+1;
$Nx = $N*6/20+1;
*
$name = "stickGrid" . $N;
$hdfName = $name . ".hdf";
*
* make a simple square
create mappings
  rectangle
    set corners
      $xa $xb $ya $yb
    mappingName
      square
    lines
      $Nx $Ny
    boundary conditions
      1 1 1 1
  exit
exit
*
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
      2 2 2 3 2 2
  exit
  compute overlap
exit
*
save an overlapping grid
  $hdfName
  $name
exit

