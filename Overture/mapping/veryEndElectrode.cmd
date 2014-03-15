*
*  make a hyperbolic surface grid for the end of the electrode
* 
open a data-base
  veryEndElectrode.hdf
open an old file read-only
get from the data-base
  veryEndElectrode
hyperbolic surface
    choose the initial curve
    create a curve from the surface

    choose an edge
    y-r
    y-r
    x-r
    x-r
    mogl-select 4 
          137 857869888 859680256  144 856239680 858687424  156 857869888 858687424  
          161 857870016 858687424  
    mogl-pick
    curve 2 (on)
    done
    exit
    edit initial curve

    mogl-select 4 
          130 835742976 836172224  137 835826432 836654784  151 835830656 836172224  
          156 835826432 836173440  
    mogl-pick
    curve 1 (on)
    done
    exit
    set debug
      3
    grow surface grid in opposite direction
    far field distance (ETAMX)
      .2
    number of lines in marching direction (KMAX)
      3
    set debug
      3
