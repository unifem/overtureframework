# -- create a surface from smoothed afm surface data ---
#    This file is included by interfaceBump3d.cmd 
  nurbs (surface)
    enter points
#    include /home/henshaw.0/cgDoc/nif/afm/afm.smallMiddlePatch.dat
    include $afm3dSurface
    mappingName
      interfaceCurve
# pause
    exit
#
