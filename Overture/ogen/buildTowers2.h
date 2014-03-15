#  ============== turbine+tower1 ======================
$xTowerShift=1.;  $yTowerShift=-2.5;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=0.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
# -- tower base that joins to the ground ---
include towerBase.h
#  ============== turbine+tower2 ======================
$xTowerShift=-1.;  $yTowerShift=+1.;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=20.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
include towerBase.h
