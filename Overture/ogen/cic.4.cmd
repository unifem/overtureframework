*
* circle in a channel, for fourth order accuracy. This
* can be used with primer/wave
*
*  -- the next few lines are for parallel: 
$dw=5; $iw=5; $interp="e"; 
* parallel ghost lines: for ogen we need at least:
*       .5*( iw -1 )   : implicit interpolation 
*       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    129 129 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    161 9
  outer radius
    .75
  boundary conditions
    -1 -1 1 0
exit
*
exit
generate an overlapping grid
    square
    Annulus
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      * implicit for all grids
      explicit for all grids
    ghost points
      all
      2 2 2 2
    order of accuracy
      fourth order
*   we could also do the following:
*     discretization width
*      all
*      5 5 
*     interpolation width
*      all
*      all
*      5 5 
  exit
  compute overlap
  exit
*
save an overlapping grid
cic.4.hdf
cic4
exit

