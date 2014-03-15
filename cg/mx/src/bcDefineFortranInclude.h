! define BC parameters for fortran routines
! boundary conditions
      integer dirichlet,perfectElectricalConductor,perfectMagneticConductor,planeWaveBoundaryCondition,\
        interfaceBC,symmetryBoundaryCondition,abcEM2,abcPML,abc3,abc4,abc5,rbcNonLocal,rbcLocal,lastBC
      parameter( dirichlet=1,perfectElectricalConductor=2,perfectMagneticConductor=3,\
            planeWaveBoundaryCondition=4,symmetryBoundaryCondition=5,interfaceBC=6,\
            abcEM2=7,abcPML=8,abc3=9,abc4=10,abc5=11,rbcNonLocal=12,rbcLocal=13,lastBC=13 )
