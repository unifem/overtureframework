#  ============== turbine+tower1 ======================
$xTowerShift=0.;  $yTowerShift=0;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=0.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
# -- tower base that joins to the ground ---
include towerBase.h
