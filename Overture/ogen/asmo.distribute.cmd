create mappings 
  * 
  open a data-base 
    asmoBody.hdf
  open an old file read-only 
  get all mappings from the data-base 
  * 
  open a data-base 
    asmoWheels.hdf
  open an old file read-only 
  get all mappings from the data-base 
  * 
  open a data-base 
    asmoBackWheel.hdf
  open an old file read-only 
  get all mappings from the data-base 
  * 
* 
* We turn on the robust inverse for some of the nasty looking grids
*
  change a mapping
   frontWheelBase
   use robust inverse
  exit
*
  change a mapping
   frontWheelJoin
   use robust inverse
  exit
*
  change a mapping
   rearEdge
   use robust inverse
  exit
exit this menu
*
generate an overlapping grid
  mainBoxForWheels
  back
  body
  front
  rearEdge
  frontWheelBase
  frontWheelJoin
  backWheelBase
  backWheelJoin
  change the plot
    toggle grid 0 0
    exit this menu
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
* pause
* pause
  compute overlap
* pause
exit
*
save an overlapping grid
asmo.hdf
asmo
exit    



exit this menu
*
generate an overlapping grid
  box1
  back
  body
  rearEdge
  front
  done
  change the plot
    toggle grid 0 0
    x-r:0
    x-r:0
    exit this menu
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
*  debug
*    3
  compute overlap
exit
*
save an overlapping grid
asmoWheels.hdf
asmo
exit    



