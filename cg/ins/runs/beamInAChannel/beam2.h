       # ----- BEAM 2 -----
       deforming body
         user defined deforming body
           elastic beam
           $I=1.; $length=1.; $thick=.2; $pNorm=1.; 
           $x0=2.; 
           $angle=90.; # $Pi*.5; 
           elastic beam parameters...
             name: beam2
             number of elements: $numElem
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
              Initial conditions:zero
             exit
            # 
            order of Galerkin projection: $orderOfProjection
            fluid on two sides $fluidOnTwoSides
            #
            use implicit predictor 1
            # -- for TP scheme 
            relax correction steps $useTP
            added mass relaxation: $addedMassRelaxation
            added mass tol: $addedMassTol
            #
            smooth solution $smoothBeam
            number of smooths: $numberOfBeamSmooths
            #
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
