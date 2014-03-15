****************************************************
** ogen command file: multiple buildings
****************************************************
** Boundary conditions:
*   1= noSlipWall
*   2= slipWall
*   3=inflow
*   4=outflow
***************************************************************************
* scale number of grid points in each direction by the following factor
* $factor=1;
* Here we get twice as many points:
$factor=2.**(1./3.); printf(" factor=$factor\n");
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2,$n3)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); $nz=int(($n3-1)*$factor+1.5);\
}
*
**************************************************************************
*
create mappings 
* 
*
*************************************************************************
*   Make the roundedCylinder buildings
*************************************************************************
*
include buildRoundedCylinder.cmd
*
*************************************************************************
*   Make the poly-building - using a smoothedPolygon as the cross-section
*************************************************************************
*
include buildPolyBuilding.cmd
*
***************************************************************************
*   Now take the basic building and scale/shift it to create new buildings
***************************************************************************
*
* ============== rounded building 1 ==============================
  rotate/scale/shift
    transform which mapping?
    roundedCylinderGrid
    shift
      .25 0 0.
    scale
     .5 1. 1.
   mappingName
    roundedCylinderGrid1
  exit
*
  rotate/scale/shift
    transform which mapping?
    roundedCylinderTop
    shift
      .25 0 0.
    scale
     .5 1. 1.
   mappingName
    roundedCylinderTop1
  exit
*
* ============== rounded building 2 ==============================
  rotate/scale/shift
    transform which mapping?
    roundedCylinderGrid
    shift
      .85 0. .6 
    scale
     1. 1.5 .75 
   mappingName
    roundedCylinderGrid2
  exit
*
  rotate/scale/shift
    transform which mapping?
    roundedCylinderTop
    shift
      .85 0. .6 
    scale
     1. 1.5 .75 
   mappingName
    roundedCylinderTop2
  exit
* ============== rounded building 3 ==============================
  rotate/scale/shift
    transform which mapping?
    roundedCylinderGrid
    shift
      .85 0. -.6 
    scale
     1. 1.25 1.
   mappingName
    roundedCylinderGrid3
  exit
*
  rotate/scale/shift
    transform which mapping?
    roundedCylinderTop
    shift
      .85 0. -.6 
    scale
     1. 1.25 1.
   mappingName
    roundedCylinderTop3
  exit
* ===================================================================
* ============== poly building 1 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      -.45 0 .5 
    scale
     1. .75 .75 
   mappingName
    polyBuilding1
  exit
*
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      -.45 0 .5 
    scale
     1. .75 .75 
   mappingName
    polyTopBox1
  exit
* ============== poly building 2 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      -.75 0 -1.0 
    scale
     .55 1.25 .55  
   mappingName
    polyBuilding2
  exit
*
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      -.75 0 -1.0 
    scale
     .55 1.25 .55  
   mappingName
    polyTopBox2
  exit
* ============== poly building 3 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      .25 0 1.25
    scale
     .75 1. .75    
   mappingName
    polyBuilding3
  exit
*
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      .25 0 1.25  
    scale
     .75 1. .75 
   mappingName
    polyTopBox3
  exit
* ============== poly building 4 ==============================
  rotate/scale/shift
    transform which mapping?
    polyBuilding
    shift
      3.00 0 -.5 
    scale
     .55 1.25 1. 
   mappingName
    polyBuilding4
  exit
*
  rotate/scale/shift
    transform which mapping?
    polyTopBox
    shift
      3.00 0 -.5 
    scale
     .55 1.25 1. 
   mappingName
    polyTopBox4
  exit
*
*
* ==================================================================
*   ** build the tower **
include buildTower.cmd
*
*
* Now shift and scale the tower 
*
  rotate/scale/shift
    transform which mapping?
    towerPod
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerPod1
  exit
  rotate/scale/shift
    transform which mapping?
    towerPodTop
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerPodTop1
  exit
  rotate/scale/shift
    transform which mapping?
    tower
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    tower1
  exit
  rotate/scale/shift
    transform which mapping?
      towerSpike
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerSpike1
  exit
  rotate/scale/shift
    transform which mapping?
      towerSpikeCap
    scale
     .75 .75 .75 
    shift
      -.95 0 -.125
   mappingName
    towerSpikeCap1
  exit
* ===================================================================
*
*
* Here is the fine box around the buildings
*
Box
  set corners
*     -.5 1.5 0. 2.0 -.5 1.5 
   -2. 2. 0. 2.5 -1.5 1.5 
  lines
    * 81 65 65
    getGridPoints(81,65,65);
    $nx $ny $nz
  boundary conditions
    3 0 1 2 2 2
  share
    0 0 2 0 0 0
  mappingName
    backGround
  exit
*
* Here are coarser boxes to extend the domain
*
Box
  set corners
   2.00  5.00   0. 2.5  -1.5 1.5 
  lines
    * 33 33 33
    getGridPoints(33,33,33);
    $nx $ny $nz
  boundary conditions
    0 4 1 2 2 2
  share
    0 0 2 0 0 0
  mappingName
    backGround1
  exit
**
exit
generate an overlapping grid
  backGround1
  backGround
  roundedCylinderTop1
  roundedCylinderGrid1
  roundedCylinderTop2
  roundedCylinderGrid2
  roundedCylinderTop3
  roundedCylinderGrid3
*
  polyBuilding1
  polyTopBox1
  polyBuilding2
  polyTopBox2
  polyBuilding3
  polyTopBox3
  polyBuilding4
  polyTopBox4
*
  tower1
  towerPod1
*  towerPodTop1
  towerSpike1
  towerSpikeCap1
  done
*
  change the plot
    toggle grid 0 0
    toggle grid 1 0
    plot block boundaries (toggle) 1
   exit this menu
* display intermediate results
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
* pause
  compute overlap 
* pause
  exit
save a grid (compressed)
multiBuildings.hdf
multiBuildings
exit

