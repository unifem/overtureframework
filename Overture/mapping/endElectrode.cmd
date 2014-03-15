*
*  make a hyperbolic surface grid on the end of the electrode.
* 
open a data-base
  endElectrode.hdf
open an old file read-only
get from the data-base
  endElectrode
*
hyperbolic 
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      0
    done
    choose an edge
      specify edge curves
        2
      done
    done
    exit
*     edit initial curve
       * reparameterize
       *  0. 1.  .0001 .9999
*        exit
    grow grid in opposite direction
    distance to march
      4.
    lines to march
      10
    boundary conditions for marching
    left
    fix y, float x and z
    right
    fix y, float x and z
    bottom
      free
    exit
    project ghost points
      left   (side=0,axis=0)
      do not project ghost points
      right  (side=1,axis=0)
      do not project ghost points
      exit
    debug
     0
    * plot reference surface (toggle)
pause
    generate

    exit
exit
exit



    edit initial curve
      reparameterize
        .1 .9
      lines
        31 
      exit



    edit initial curve
      lines
        31 
      exit

    boundary conditions for marching
    set side=0, axis=0 (left)
    fix y, float x and z
    set side=1, axis=0 (right)
    fix y, float x and z
    exit
*
    
    curvature speed
      0.
    equidistribution weight
      0.


    choose an edge
    mogl-select 2 
          130 806592704 812973376  183 806597888 807378944  
    mogl-pick
    done
    pause
    exit
    number of lines in marching direction (KMAX)
      101
    boundary condition (IBCJA,IBCJB)
      2 2
    far field distance (ETAMX)
      71.
    edit initial curve
      reparameterize
        .01 .99
      lines
        31 
      exit

    
