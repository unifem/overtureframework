*
* ***** Generate figures for ogenerator.tex ****
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    32 32 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    33 7
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
    * do not interpolate ghost
    * choose implicit or explicit interpolation
    interpolation type
      implicit for all grids
      * explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  display intermediate
  compute overlap
  continue
  save postscript
    cicCutHoles.ps
  continue
  save postscript
    cicRemoveExterior.ps
  continue
  save postscript
    cicImproper.ps
  continue
  save postscript
    cicProper.ps
  continue
  save postscript
    cicAll.ps
  continue
  save postscript
    cicDone.ps
  pause  


