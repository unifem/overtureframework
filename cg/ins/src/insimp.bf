
! *****************************************************************************************
! Define the main routine that calls
!              insImpINS
!              insImpVP
!
! NOTES:
!  - See insImp.h for the main template that defines the insImpXXX routines.
!  - See insImpINS.bf, insImpVP.bf, .. for particular implementaions (that include insImp.h)
!
! *****************************************************************************************

      subroutine insimp(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                      mask,xy,rsxy,radiusInverse,  u, ndc, coeff, fe,fi,ul, gv,gvl,dw, \
                      ndMatProp,matIndex,matValpc,matVal, bc, \
                      boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData,\
                      nde, equationNumber, classify, \
                      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b, \
                      ipar, rpar, pdb, ierr )
c======================================================================
c 
c             Incompressible Navier Stokes IMPlicit 
c             -------------------------------------
c
c    1. Build the coefficient matrix for implicit methods
c    2. Evaluate the right-hand-side and residual 
c
c nd : number of space dimensions
c nd1a,nd1b,nd2a,nd2b,nd3a,nd3b : array dimensions
c
c mask : 
c xy : 
c rsxy : 
c coeff(m,i1,i2,i3) : array holding the matrix coefficients
c u : holds the current solution, used to form the coeff matrix.
c fe : holds the explicit part when evaluating the RHS
c fi : holds the implicit part when evaluating the RHS
c ul : holds the linearized solution, used when evaluating the linearized operator and RHS
c gv : gridVelocity for moving grids
c gvl: linearized gridVelocity for moving grids
c dw : distance to the wall for some turbulence models
c 
c======================================================================
      implicit none
      integer nd, ndc, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      integer nde,nr1a,nr1b,nr2a,nr2b,nr3a,nr3b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real coeff(0:ndc-1,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real fe(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real fi(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real ul(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real gvl(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),boundaryCondition(0:1,0:2),indexRange(0:1,0:2),ierr

      integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
      real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

      integer equationNumber(0:nde-1,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer classify(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

      integer ipar(0:*)
      real rpar(0:*)
      
      double precision pdb  ! pointer to data base

      ! -- arrays for variable material properties --
      integer constantMaterialProperties
      integer piecewiseConstantMaterialProperties
      integer variableMaterialProperties
      parameter( constantMaterialProperties=0,\
                 piecewiseConstantMaterialProperties=1,\
                 variableMaterialProperties=2 )
      integer materialFormat,ndMatProp
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

c     ---- local variables -----
      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )


      ierr=0

      turbulenceModel    =ipar(27)
      pdeModel           =ipar(28)  
      materialFormat    =ipar(51)

      ! write(*,'(" INSIMP: pdeModel=",i3," turbulenceModel=",i3," materialFormat=",i3)') pdeModel,turbulenceModel,materialFormat

      if( (pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel ).and. \
          turbulenceModel.eq.noTurbulenceModel )then
        call insImpINS(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                       mask,xy,rsxy,radiusInverse,  u, ndc, coeff, fe,fi,ul, gv,gvl,dw, ndMatProp,matIndex,matValpc,matVal, bc, \
                       boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData,\
                       nde, equationNumber, classify, \
                       nr1a,nr1b,nr2a,nr2b,nr3a,nr3b, \
                       ipar, rpar, pdb, ierr )
      else if( pdeModel.eq.viscoPlasticModel .or. (pdeModel.eq.BoussinesqModel .and. turbulenceModel.eq.largeEddySimulation) )then
        ! For now the VP option builds a temperature equation

        if( materialFormat.ne.constantMaterialProperties )then
         write(*,'(" insimp.bf -- finish this option for variable material properties")')
         stop 6206
        end if
        call insImpVP(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                      mask,xy,rsxy,radiusInverse,  u, ndc, coeff, fe,fi,ul, gv,gvl,dw, ndMatProp,matIndex,matValpc,matVal, bc, \
                      boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData,\
                      nde, equationNumber, classify, \
                      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b, \
                      ipar, rpar, pdb, ierr )
      else if( turbulenceModel.eq.baldwinLomax )then
        if( materialFormat.ne.constantMaterialProperties )then
         write(*,'(" insimp.bf -- finish this option for variable material properties")')
         stop 6206
        end if
        call insImpBL(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                      mask,xy,rsxy,radiusInverse,  u, ndc, coeff, fe,fi,ul, gv,gvl,dw, ndMatProp,matIndex,matValpc,matVal, bc, \
                      boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData,\
                      nde, equationNumber, classify, \
                      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b, \
                      ipar, rpar, pdb, ierr )
      else if( turbulenceModel.eq.kEpsilon )then
        if( materialFormat.ne.constantMaterialProperties )then
         write(*,'(" insimp.bf -- finish this option for variable material properties")')
         stop 6206
        end if
        call insImpKE(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                      mask,xy,rsxy,radiusInverse,  u, ndc, coeff, fe,fi,ul, gv,gvl,dw, ndMatProp,matIndex,matValpc,matVal, bc, \
                      boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData,\
                      nde, equationNumber, classify, \
                      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b, \
                      ipar, rpar, pdb, ierr )
      else if( pdeModel.eq.twoPhaseFlowModel )then
        if( materialFormat.ne.constantMaterialProperties )then
         write(*,'(" insimp.bf -- finish this option for variable material properties")')
         stop 6206
        end if
        call insImpTP(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                      mask,xy,rsxy,radiusInverse,  u, ndc, coeff, fe,fi,ul, gv,gvl,dw, ndMatProp,matIndex,matValpc,matVal, bc, \
                      boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData,\
                      nde, equationNumber, classify, \
                      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b, \
                      ipar, rpar, pdb, ierr )
      else if( pdeModel.eq.BoussinesqModel )then

       write(*,'(" insimp:ERROR: unimplemented pdeModel=",i6)') pdeModel
       stop 4040

      else
       write(*,'(" insimp:ERROR: unimplemented:  pdeModel,turbulenceModel=",2i6)') pdeModel,turbulenceModel
        ! '
       stop 4041
      end if

      return
      end
