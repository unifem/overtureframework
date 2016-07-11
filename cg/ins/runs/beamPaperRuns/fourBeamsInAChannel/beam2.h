       # ----- BEAM 2 -----
       deforming body
         user defined deforming body
           elastic beam
           $beamModel2
           $I=1.; $length=$beamLength2; $thick=.2; $pNorm=1.;
           $nElem=int($numElem*$length);
           $x0=1.5; 
           $angle=90.; # $Pi*.5; 
           elastic beam parameters...
	     predictor: $ps
	     corrector: $cs
	     cfl: $cfls
	     use same stencil size for FD $useSameStencilSize
             name: beam2
             number of elements: $nElem
             area moment of inertia: $I
             elastic modulus: $E2
             density: $rhoBeam2
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
	    #Longfei: use implicit predictor option removed
	    #         now select time-stepping methods using
	    #         predictor, corrector option menus
            #use implicit predictor 1
            # -- for TP scheme 
            relax correction steps $useTP
            added mass relaxation: $addedMassRelaxation
            added mass tol: $addedMassTol
            #
            smooth solution $smoothBeam
            number of smooths: $numberOfBeamSmooths
	    debug:0
            # probe location in [0,1]
            probe position: $probePosition
            probe file name: $beamProbeFileName2
            probe file save frequency: 10
            save probe file $saveProbe
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
