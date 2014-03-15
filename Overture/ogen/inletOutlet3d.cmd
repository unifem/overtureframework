create mappings
  box
    set corners
    0 1 0 1 0 1 
    lines
    51 51 51
    set view:0 0 0 0 1 0.914544 0.0348415 -0.402984 0.0786643 0.961941 0.261691 0.396764 -0.271029 0.876996
    mappingName
    box
    exit
*
  box
    set corners
    -.2 0.  .4 .6 .4 .6
    lines
      11 11 11
    boundary conditions
    2 0 1 1 1 1
    set view:0 0 0 0 1 0.978297 -0.0102269 0.206953 -0.0116001 0.994512 0.10398 -0.206881 -0.104124 0.97281
    mappingName
    inlet
    exit
*
  box
    set corners
      1. 1.2 .4 .6 .4 .6
    lines
      11 11 11
    boundary conditions
      0 3 1 1 1 1
    mappingName
     outlet
    exit
  exit this menu
*
generate an overlapping grid
   box
   inlet
   outlet
   done choosing mappings
  change parameters
    prevent hole cutting
      box
        all
      inlet
        all
      outlet
        all
    done
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
pause
  exit
save an overlapping grid
inletOutlet3d.hdf
inletOutlet3d
exit
