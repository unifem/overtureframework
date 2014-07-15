       # ----- BEAM 2 -----
       deforming body
         user defined deforming body
           elastic beam
           $I=1.; $E=10.; $rhoBeam=100.; $length=1.; $thick=.2; $pNorm=1.; 
           $x0=2.; 
           $angle=90.; # $Pi*.5; 
           elastic beam parameters...
             name: beam2
             number of elements: 11
             area moment of inertia: $I
             elastic modulus: $E
             density: $rhoBeam
             thickness: $thick
             length: $length
             pressure norm: $pNorm
             initial declination: $angle (degrees)
             position: $x0, 0, 0 (x0,y0,z0)
             bc left:clamped
             bc right:free
             initial conditions... 
               zero initial conditions
             exit
             debug: 0
           exit
           # ----
           boundary parameterization
              1
           BC left: Dirichlet
           BC right: Dirichlet
           BC bottom: Dirichlet
           BC top: Dirichlet
         #
         done
         choose grids by share flag
           101
      done
