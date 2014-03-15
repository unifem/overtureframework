*
* Create a patch with transfinite interpolation
*
* create a line for the top boundary
line
  number of dimensions 
   2
  specify end points
    -.8 1. .8 1.
exit
* create a spline for the bottom boundary
spline
  enter spline points
    5
    -1. 0.
    -.5 0.
    0. .25
    .5 .0
    1. 0.
exit
* create a tfi patch
tfi
  choose
    spline
    line
    done
  pause
exit

