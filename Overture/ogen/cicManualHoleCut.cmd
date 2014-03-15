*
* circle in a channel
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
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
*  centre
*    0. 1.
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
*    turn on phantom hole cutting for the annulus
    phantom hole cutting
      Annulus
      all
    done
*    cut a hole in the square:
    manual hole cutting
      square
        15 16 15 16 0 0
      done
    ghost points
      all
      2 2 2 2 2 2
  exit
  display intermediate results
  compute overlap
  continue
  continue

  exit
*
save an overlapping grid
cicManualHoleCut.hdf
cicManualHoleCut
exit

