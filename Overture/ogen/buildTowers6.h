#  ============== turbine+tower1 ======================
$xTowerShift=3.5;  $yTowerShift=-2.5;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=0.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
# -- tower base that joins to the ground ---
include towerBase.h
#  ============== turbine+tower2 ======================
$xTowerShift=-3.5;  $yTowerShift=-2.75;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=20.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
include towerBase.h
#  ============== turbine+tower3 ======================
$xTowerShift=-.5;  $yTowerShift=+1.;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=35.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
include towerBase.h
#  ============== turbine+tower4 ======================
$xTowerShift=6.5;  $yTowerShift=+2.5;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=55.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
include towerBase.h
#  ============== turbine+tower5 ======================
$xTowerShift=-2.5;  $yTowerShift=+4.0;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=60.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
include towerBase.h
#  ============== turbine+tower6 ======================
$xTowerShift=4.0;  $yTowerShift=+6.5;  $zTowerShift=groundLevel($xTowerShift,$yTowerShift)+.5;
$bladeTheta=75.; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
include turbineTower.h
include towerBase.h
