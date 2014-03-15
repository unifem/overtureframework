create mappings 
  * 
  open a data-base 
  /home/henshaw/Overture/hype/cmd/truck/cabTenderGrids.hdf
    open an old file read-only 
    get all mappings from the data-base 
  close the data-base
*
  open a data-base 
  /home/henshaw/Overture/hype/cmd/truck/frontLeftWheel.hdf
    open an old file read-only 
    get all mappings from the data-base 
  close the data-base
*
  open a data-base 
  /home/henshaw/Overture/hype/cmd/truck/frontRightWheel.hdf
    open an old file read-only 
    get all mappings from the data-base 
  close the data-base
*
  open a data-base 
  /home/henshaw/Overture/hype/cmd/truck/tenderRearWheels.hdf
    open an old file read-only 
    get all mappings from the data-base 
  close the data-base
*
  exit this menu
*
generate an overlapping grid
  boxBehindCab
  boxBehindTender
  topBox
  leftBox
  rightBox
  frontBox
  tenderBox
  bottomBox
  hoodBox
  backBox
*
  cabTop
  body
  hood
  front
  windshield
  tender
  backTender
  backCabEdge
  backCabBottomEdge
  backCabMiddleEdge
  leftCabCorner
  rightCabCorner
  frontLeftWheel
  frontLeftWheelJoin
  frontRightWheel
  frontRightWheelJoin
  tenderLeftRear2Wheel
  tenderLeftRear2WheelJoin
  done
*
  change the plot
    toggle grid 3 0
    toggle grid 4 0
    bigger:0
    bigger:0
    exit this menu
*
  change parameters
*     
    ghost points
      all
      2 2 2 2 2 2
   default shared boundary normal tolerance
     .4 .2
    exit
*  display intermediate results
* pause
   compute overlap
* pause
 exit
*
save an overlapping grid
cabTender.hdf
cabTender
exit 






