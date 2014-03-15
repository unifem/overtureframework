# ************ Define a turbine Blade *******************
# -- flatten and rotate to make a blade
#  $bladeAngle=-25.; $bladeFlatten=.5; 
#  $bladeTheta=0.; 
#  $bladeShare=5;
#  $xBladeShift = $radius - $cylStart;  # shift so the axis is (0,0,0)
#  $yBladeShift=0.; $zBladeShift=0.; 
# 
  ## $numNurbGhost=1; ## something is wrong here using this option -- see grid in ogen at branch cut 
  $numNurbGhost=0;
  nurbs 
    interpolate from mapping with options
      cyl
     parameterize by index (uniform)
     number of ghost points to include
      $numNurbGhost
    done
    scale
      1 1 $bladeFlatten
    rotate
      $bladeAngle 0
      0 0 0
    shift 
      $xHubShift 0 0 
    rotate 
      $bladeTheta 1
      0 0 0
    shift 
      $xBladeShift $yBladeShift $zBladeShift
    boundary conditions
      0 0 -1 -1 $bladeBC 0 
    share 
      0 0 0 0 $bladeShare 0 
    mappingName
     $numberOfBlades++;
     $bladeName="blade$numberOfBlades";
     $gridNames.="\n$bladeName"; 
     $bladeName
    exit
# ------------
  nurbs 
    interpolate from mapping with options
      endCap1
     parameterize by index (uniform)
     number of ghost points to include
      $numNurbGhost
    done
    scale
      1 1 $bladeFlatten
    rotate
      $bladeAngle 0
      0 0 0
    shift 
      $xHubShift 0 0 
    rotate 
      $bladeTheta 1
      0 0 0
    shift 
      $xBladeShift $yBladeShift $zBladeShift
    boundary conditions
      0 0 0 0 $bladeBC 0 
    share 
      0 0 0 0 $bladeShare 0 
    mappingName
      $capName = "endCap1$bladeName"; 
      $gridNames.="\n$capName"; 
      $capName
    exit
# ------------
  nurbs 
    interpolate from mapping with options
      endCap2
     parameterize by index (uniform)
     number of ghost points to include
      $numNurbGhost
    done
    scale
      1 1 $bladeFlatten
    rotate
      $bladeAngle 0
      0 0 0
    shift 
      $xHubShift 0 0 
    rotate 
      $bladeTheta 1
      0 0 0
    shift 
      $xBladeShift $yBladeShift $zBladeShift
    boundary conditions
      0 0 0 0 $bladeBC 0 
    share 
      0 0 0 0 $bladeShare 0 
    mappingName
      $capName = "endCap2$bladeName"; 
      $gridNames.="\n$capName"; 
      $capName
    exit
# ------------
