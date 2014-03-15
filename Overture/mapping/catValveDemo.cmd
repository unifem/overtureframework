  read iges file
    /home/henshaw/Overture/mapping/cat2.igs
    continue
*    choose some
*     0 -1   100 40  25   0 15 
    choose a list
    * here is a valve
     206 207 208 209 210 211 212 213 214 215 216
    done
     CSUP:determine topology
    mappingName
      cat
    exit
    builder
    x+r:0
    x+r:0
pause
    create surface grid...
      mogl-select:0 2 
            185 1173310848 1173342080  120 1177706624 1177975296  
      mogl-coordinates 4.985075e-01 8.264463e-01 1.173311e+09 6.955352e+01 6.240934e+01 1.470563e+02
pause
      mogl-select:0 1 
            120 1177265792 1177323776  
      mogl-coordinates 5.044776e-01 8.567493e-01 1.177266e+09 7.011895e+01 6.238940e+01 1.379191e+02
      lines to march 90
      distance to march 80. 
pause
      forward
      generate
      pause
      lines to step 20
      step
      x-r:0
      x-r:0
      x-r:0
      x-r:0
      x-r:0
pause
      exit
    create volume grid...
      backward
      marching options...
      uniform dissipation .01
      lines to march 11
      distance to march 3
      generate












*
*  make a hyperbolic surface grid for a valve
* 
open a data-base
  catValve.hdf
open an old file read-only
get from the data-base
  valve
hyperbolic
  plot shaded boundaries on reference surface
  plot boundary lines on reference surface (toggle)
pause
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      5 10
    done
    choose an edge
pause
      specify edge curves
        3 7 
      done
pause
    done
  exit
  edit initial curve
    lines
     43 35 31 21
  exit
  * the implicit coeff must be turned on or else we we hit the marching CFL.
  implicit coeff
    .5
  distance to march
    25.
  lines to march 
    11 16  21 26
  * set bc at top to prevent projection of ghost line
  boundary conditions
    -1 -1 1 0
pause
*  plot shaded boundaries on reference surface
  generate
  pause
  step 30 45  60 75
  pause
  plot reference surface (toggle)

  mappingName
    valveSurface
  exit
* Now create the volume grid
hyperbolic
  start from which curve/surface?
    valveSurface
  grow grid in opposite direction
  implicit coeff
   0
  uniform dissipation coefficient
   .01
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
   distance to march 
     5. 6. 5. 4. 4.5 5. 4.
   lines to march 
     13 11 9 7 6 5
  geometric stretching, specified ratio
    1.2
  generate
  pause
  boundary conditions
    -1 -1 0 0 1 0
  share
     0  0 0 0 6 0
  mappingName
    valveVolume
  exit
*
  open a data-base
   valveVolume.hdf
     open a new file
   put to the data-base
     valveVolume
   close the data-base
exit


