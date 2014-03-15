*
*  Simple annulus 
*
GetOptions("N=i"=>\$Nt);
create mappings
*
annulus
*    inner radius
*      1.
*    outer radius
*      .5
$Nr = (($Nt-1)/10)*6 + 1;
  lines
  $Nt $Nr
  boundary conditions
    -1 -1 1 2
exit
*
exit
generate an overlapping grid
    Annulus
  done
  change parameters
    ghost points
      all
      4 4 4 4 4 4
    order of accuracy
    fourth order
  exit
  compute overlap
  exit
*
*save an overlapping grid
*annulus.hdf
*annulus
exit

