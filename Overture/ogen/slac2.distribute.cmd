create mappings 
  * 
  open a data-base 
  slac2Grids.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  * 
  change a mapping
    box1
    lines
     45 45 30 
    exit
exit this menu
*
generate an overlapping grid
  box1
  box2
  box3
  box4
*  box5
  slac2-volume5
  slac2-volume6
  slac2-volume7
  slac2-volume8
  slac2-volume9
  slac2-volume10
  x+r:0 120
  y+r:0 20
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    prevent hole cutting
      all
      all
      done
    allow hole cutting
      all
      box1
      all
      box2
      all
      box3
      all
      box4
*      all
*      box5
      done
    exit
* pause
*  display intermediate results
  compute overlap
exit
*
save an overlapping grid
slac2.hdf
slac2
exit    

    

generate an overlapping grid
  box1
  slac2-volume7
  done
  change parameters
    prevent hole cutting
      box1
      all
    done
  exit
  x+r:0 120
  y+r:0 20
  display intermediate results
  compute overlap



generate an overlapping grid
  box1
  box2
  box3
  box4
  slac2-volume5
  slac2-volume6
  slac2-volume7
  slac2-volume8
  slac2-volume9
  slac2-volume10
  box5
  change parameters
    prevent hole cutting
      box1
      all
      box2
      all
      box3
      all
      box4
      all
      box5
      all
      slac2-volume7
       all
      slac2-volume8
       all
      done
    exit
  display intermediate results
  compute overlap


