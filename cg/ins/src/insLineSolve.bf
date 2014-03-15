c***********************************************************************************************
c 
c   Steady-state line-solver routines for the incompressible NS plus some turbulence models
c
c***********************************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

#Include "commonMacros.h"
c ** Include "defineSelfAdjointMacros.h"

c ===========================================================================================
c dim : number of dimensions 2,3
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c equations e1,e2,...,e10 are for the matrix
c equations e11,e12,... are for the RHS
c ===========================================================================================
#beginMacro triLoops(dim,EQN, e1,e2,e3,e4,e5,e6,e7,e8,e9,e10, e11,e12,e13)
if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
   e1
   e2
   e3
   e4
   e5
   e6
   e7
   e8
   e9
   e10
c rhs equations:
   e11
   e12
   e13
  else 
c for interpolation points or unused:
   am(i1,i2,i3)=0.
   bm(i1,i2,i3)=1.
   cm(i1,i2,i3)=0.
#If #EQN == "TEMPERATURE"
   f(i1,i2,i3,fct)=uu(tc)
#Else
   f(i1,i2,i3,fcu)=uu(uc)
   f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
   f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
   f(i1,i2,i3,nc)=uu(nc)
#End
  end if
 end do
 end do
 end do

else if( computeMatrix.eq.1 )then

 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
   e1
   e2
   e3
   e4
   e5
   e6
   e7
   e8
   e9
   e10
  else 
c for interpolation points or unused:
   am(i1,i2,i3)=0.
   bm(i1,i2,i3)=1.
   cm(i1,i2,i3)=0.
  end if
 end do
 end do
 end do

else if( computeRHS.eq.1 )then

 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
   e11
   e12
   e13
  else 
c for interpolation points or unused:
#If #EQN == "TEMPERATURE"
   f(i1,i2,i3,fct)=uu(tc)
#Else
   f(i1,i2,i3,fcu)=uu(uc)
   f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
   f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
   f(i1,i2,i3,nc)=uu(nc)
#End

  end if
 end do
 end do
 end do
end if
#endMacro



c ===========================================================================================
c Loops: 4th-order version
c
c dim : number of dimensions 2,3
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c equations e1,e2,...,e10 are for the matrix
c equations e11,e12,... are for the RHS
c ===========================================================================================
#beginMacro triLoops4(dim,EQN, e1,e2,e3,e4,e5,e6,e7,e8,e9,e10, e11,e12,e13)
if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
      e1
      e2
      e3
      e4
      e5
      e6
      e7
      e8
      e9
      e10
c rhs equations:
      e11
      e12
      e13
    else 
c for interpolation points or unused:
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=0.
      cm(i1,i2,i3)=1.
      dm(i1,i2,i3)=0.
      em(i1,i2,i3)=0.

#If #EQN == "TEMPERATURE"
      f(i1,i2,i3,fct)=uu(tc)
#Else
      f(i1,i2,i3,fcu)=uu(uc)
      f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
      f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
      f(i1,i2,i3,nc)=uu(nc)
#End
    end if
  end do
  end do
  end do

else if( computeMatrix.eq.1 )then

  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
      e1
      e2
      e3
      e4
      e5
      e6
      e7
      e8
      e9
      e10
    else 
c for interpolation points or unused:
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=0.
      cm(i1,i2,i3)=1.
      dm(i1,i2,i3)=0.
      em(i1,i2,i3)=0.
    end if
  end do
  end do
  end do

else if( computeRHS.eq.1 )then

  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
      e11
      e12
      e13
    else 
c for interpolation points or unused:
#If #EQN == "TEMPERATURE"
      f(i1,i2,i3,fct)=uu(tc)
#Else
      f(i1,i2,i3,fcu)=uu(uc)
      f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
      f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
      f(i1,i2,i3,nc)=uu(nc)
#End
    end if
  end do
  end do
  end do
end if
#endMacro





#beginMacro loops(e1,e2,e3, e4,e5,e6)
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
      e5
      e6
    end if
  end do
  end do
  end do
#endMacro

c ***********************************************************************
c Fill in the matrix and RHS on the boundary
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c ***********************************************************************
#beginMacro loopsBC(dim,EQN, e1,e2,e3,e4,e5,e6)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
   e1
   e2
   e3
   e4
   e5
   e6
  else
c for interpolation points or unused:
   am(i1,i2,i3)=0.
   bm(i1,i2,i3)=1.
   cm(i1,i2,i3)=0.

#If #EQN == "TEMPERATURE"
   f(i1,i2,i3,fct)=uu(tc)
#Else
   f(i1,i2,i3,fcu)=uu(uc)
   f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
   f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
   f(i1,i2,i3,nc)=uu(nc)
#End      

  end if
 end do
 end do
 end do
#endMacro

c ***********************************************************************
c Fill in the matrix and RHS on the boundary
c ***********************************************************************
#beginMacro loopsMatrixBC(e1,e2,e3,e4,e5,e6)
  if( nd.eq.2 )then
    if( option.eq.assignINS )then
      loopsBC(2,INS,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignTemperature )then
      loopsBC(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignSpalartAllmaras )then
      loopsBC(2,SA,e1,e2,e3, e4,e5,e6)
    end if
  else
    if( option.eq.assignINS )then
      loopsBC(3,INS,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignTemperature )then
      loopsBC(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignSpalartAllmaras )then
      loopsBC(3,SA,e1,e2,e3, e4,e5,e6)
    end if
  end if
#endMacro


c ***********************************************************************
c Fill in the matrix and RHS on the boundary, fourth-order version
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c ***********************************************************************
#beginMacro loopsBC4(dim,EQN, e1,e2,e3,e4,e5,e6)
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
      e5
      e6
    else
c for interpolation points or unused:
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=0.
      cm(i1,i2,i3)=1.
      dm(i1,i2,i3)=0.
      em(i1,i2,i3)=0.
      am(i1-is1,i2-is2,i3-is3)=0.
      bm(i1-is1,i2-is2,i3-is3)=0.
      cm(i1-is1,i2-is2,i3-is3)=1.
      dm(i1-is1,i2-is2,i3-is3)=0.
      em(i1-is1,i2-is2,i3-is3)=0.

#If #EQN == "TEMPERATURE"
      f(i1,i2,i3,fct)=uu(tc)
      f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,i3-is3,tc)
#Else
      f(i1,i2,i3,fcu)=uu(uc)
      f(i1,i2,i3,fcv)=uu(vc)
      f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,i3-is3,uc)
      f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,i3-is3,vc)
#If #dim == "3"
      f(i1,i2,i3,fcw)=uu(wc)
      f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,i3-is3,wc)
#End
#End
#If #EQN == "SA"
      f(i1,i2,i3,nc)=uu(nc)
      f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,i3-is3,nc)
#End    

    end if
  end do
  end do
  end do
#endMacro

c ***********************************************************************
c Fill in the matrix and RHS on the boundary -- fourth-order version
c ***********************************************************************
#beginMacro loopsMatrixBC4(e1,e2,e3,e4,e5,e6)
  if( nd.eq.2 )then
    if( option.eq.assignINS )then
      loopsBC4(2,INS,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignTemperature )then
      loopsBC4(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignSpalartAllmaras )then
      loopsBC4(2,SA,e1,e2,e3, e4,e5,e6)
    end if
  else
    if( option.eq.assignINS )then
      loopsBC4(3,INS,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignTemperature )then
      loopsBC4(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
    else if( option.eq.assignSpalartAllmaras )then
      loopsBC4(3,SA,e1,e2,e3, e4,e5,e6)
    end if
  end if
#endMacro


c macro for assigning the RHS
c e1,e2,e3 : statements for u,v,w
c e4       : statement for the turbulence model
c$$$#beginMacro loopsRHS(e1,e2,e3, e4)
c$$$if( turbulenceModel.eq.noTurbulenceModel )then
c$$$  if( nd.eq.2 )then
c$$$    loops(e1,e2,,,,)
c$$$  else if( nd.eq.3 )then
c$$$    loops(e1,e2,e3,,,)
c$$$  end if
c$$$else
c$$$  if( nd.eq.2 )then
c$$$    loops(e1,e2,e4,,,)
c$$$  else if( nd.eq.3 )then
c$$$    loops(e1,e2,e3,e4,,)
c$$$  end if
c$$$end if
c$$$#endMacro



c Define the artificial diffusion coefficients
c gt should be R or C
c tb should be blank or SA (for Splarat-Allmaras)
#beginMacro defineArtificialDiffusionCoefficients(dim,dir,gt,tb)
  #If #dim == "2" 
    cdmz=admz ## gt ## tb(i1  ,i2  ,i3)
    cdpz=admz ## gt ## tb(i1+1,i2  ,i3)
    cdzm=adzm ## gt ## tb(i1  ,i2  ,i3)
    cdzp=adzm ## gt ## tb(i1  ,i2+1,i3)
    ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
    ! cdmz=0.
    ! cdpz=0.
    ! cdzm=0.
    ! cdzp=0.
    cdDiag=cdmz+cdpz+cdzm+cdzp
    #If #dir == "0" 
      cdm=cdmz
      cdp=cdpz
    #Elif #dir == "1"
      cdm=cdzm
      cdp=cdzp
    #Else
      stop 1234
    #End
  #Elif #dim == "3"
    cdmzz=admzz ## gt ## tb(i1  ,i2  ,i3  )
    cdpzz=admzz ## gt ## tb(i1+1,i2  ,i3  )
    cdzmz=adzmz ## gt ## tb(i1  ,i2  ,i3  )
    cdzpz=adzmz ## gt ## tb(i1  ,i2+1,i3  )
    cdzzm=adzzm ## gt ## tb(i1  ,i2  ,i3  )
    cdzzp=adzzm ## gt ## tb(i1  ,i2  ,i3+1)
    cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
    #If #dir == "0" 
      cdm=cdmzz
      cdp=cdpzz
    #Elif #dir == "1"
      cdm=cdzmz
      cdp=cdzpz
    #Elif #dir == "2"
      cdm=cdzzm
      cdp=cdzzp
    #Else 
      stop 9876
    #End
  #Else
    stop 888
  #End
#endMacro


c Define the turbulent eddy viscosity and it's derivatives given chi3=chi^3 
#beginMacro defineSADerivatives(dim,gt)
  nuT = nu+u(i1,i2,i3,nc)*chi3/(chi3+cv1e3)
  nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
  #If #gt == "rectangular"
    nuTx(0)=ux2(nc)*nuTd
    nuTx(1)=uy2(nc)*nuTd
    #If #dim == "3" 
      nuTx(2)=uz2(nc)*nuTd
    #End
  #Else
    #If #dim == "2" 
      nuTx(0)=ux2c(nc)*nuTd
      nuTx(1)=uy2c(nc)*nuTd
    #Else
      nuTx(0)=ux3c(nc)*nuTd
      nuTx(1)=uy3c(nc)*nuTd
      nuTx(2)=uz3c(nc)*nuTd
    #End
  #End
#endMacro

c Define the turbulent eddy viscosity and it's derivatives for the BL model 
#beginMacro defineBLDerivatives(dim,gt)
  nuT = nu +u(i1,i2,i3,nc)
  #If #gt == "rectangular"
    nuTx(0)=ux2(nc)
    nuTx(1)=uy2(nc)
    #If #dim == "3" 
      nuTx(2)=uz2(nc)
    #End
  #Else
    #If #dim == "2" 
      nuTx(0)=ux2c(nc)
      nuTx(1)=uy2c(nc)
    #Else
      nuTx(0)=ux3c(nc)
      nuTx(1)=uy3c(nc)
      nuTx(2)=uz3c(nc)
    #End
  #End
#endMacro

c Define the turbulent eddy viscosity and it's derivatives  
#beginMacro defineValuesSA(dim,gt)
  chi3 = (u(i1,i2,i3,nc)/nu)**3
  ! chi3=0.
  defineSADerivatives(dim,gt)
#endMacro

c Define the turbulent eddy viscosity and it's derivatives  
#beginMacro defineValuesBL(dim,gt)
  ! chi3=0.
  defineBLDerivatives(dim,gt)
#endMacro

c=======================================================================
c Define the stuff needed for 2nd-order + 4th-order artificial dissipation
c define: adCoeff2, adCoeff4 and the inline macro ade(cc) (for the rhs)
c=======================================================================
#beginMacro defineAD24(ADTYPE,DIM,DIR)
 #If #DIM == "2"
  #If #DIR == "0"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)) \
              + adCoeff4*(-u(i1,i2-2,i3,cc)-u(i1,i2+2,i3,cc)+4.*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)))
  #Elif #DIR == "1"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)) \
              + adCoeff4*(-u(i1-2,i2,i3,cc)-u(i1+2,i2,i3,cc)+4.*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)))
  #Else
    stop 676
  #End

 #Elif #DIM == "3"

  #If #DIR == "0"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)) \
              + adCoeff4*(-u(i1,i2-2,i3,cc)-u(i1,i2+2,i3,cc)-u(i1,i2,i3-2,cc)-u(i1,i2,i3+2,cc)\
                      +4.*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)))
  #Elif #DIR == "1"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)) \
              + adCoeff4*(-u(i1-2,i2,i3,cc)-u(i1+2,i2,i3,cc)-u(i1,i2,i3-2,cc)-u(i1,i2,i3+2,cc)\
                      +4.*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)))
  #Elif #DIR == "2"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)) \
              + adCoeff4*(-u(i1-2,i2,i3,cc)-u(i1+2,i2,i3,cc)-u(i1,i2-2,i3,cc)-u(i1,i2+2,i3,cc)\
                      +4.*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)))
  #Else
    stop 676
  #End

 #Else
   stop 677
 #End

#endMacro

c =======================================================================================
c =======================================================================================
#beginMacro defineSelfAdjointDiffusionCoefficients(DIM,DIR,UV,nuT)

#If #DIM == "2"

 #If #UV == "U"
  getSelfAdjointDiffusionCoefficients(UV,i1,i2,i3,nuT,cu,cv)
  #If #DIR == "0"
   dsam    = cu(-1,0)
   dsaDiag = cu( 0,0)
   dsap    = cu( 1,0)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Elif #DIR == "1"
   dsam    = cu( 0,-1)
   dsaDiag = cu( 0, 0)
   dsap    = cu( 0, 1)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Else
    stop 101
  #End
 #Elif #UV == "V"
   ! note reverse order of cu and cv :
   getSelfAdjointDiffusionCoefficients(UV,i1,i2,i3,nuT,cv,cu)
   dsam    = cv(-1,0)
   dsaDiag = cv( 0,0)
   dsap    = cv( 1,0)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Elif #DIR == "1"
   dsam    = cv( 0,-1)
   dsaDiag = cv( 0, 0)
   dsap    = cv( 0, 1)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Else
    stop 101
  #End
 #Else
   stop 111
 #End

#Elif #DIM == "3"

  stop 777

#Else
  stop 123
#End

#endMacro

c ===================================================================================
c  SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE 
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsRectangularGrid(SOLVER,dir)

#If #SOLVER == "INS"
c  ****** Incompressible NS, No turbulence model *****
if( nd.eq.2 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
  defineDerivativeMacros(2,2,rectangular)

  if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then
    triLoops(2,INS,\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir), \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*nu*(dxvsqi(0)+dxvsqi(1)), \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir),,,,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d ## dir(uc)-ux2(pc),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d ## dir(vc)-uy2(pc),)
  else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
    triLoops(2,INS,\
             $defineArtificialDiffusionCoefficients(2,dir,R,),\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdm , \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*nu*(dxvsqi(0)+dxvsqi(1)) +cdDiag, \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdp,,,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d ## dir(uc)-ux2(pc) +adE ## dir(i1,i2,i3,uc),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d ## dir(vc)-uy2(pc) +adE ## dir(i1,i2,i3,vc),)
  else if( use4thOrderAD.eq.1 )then
    defineAD24(AD24,2,dir)
    defineArtificialDissipationMacro(AD24,2,INS)
    triLoops4(2,INS,\
             $getArtificialDissipationCoeff(AD24,2,INS),\
             am(i1,i2,i3)= adCoeff4,\
             bm(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -adCoeff2 -4.*adCoeff4, \
             cm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*nu*(dxvsqi(0)+dxvsqi(1)) +4.*adCoeff2 + 12.*adCoeff4, \
             dm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -adCoeff2 -4.*adCoeff4,\
             em(i1,i2,i3)= adCoeff4 ,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d ## dir(uc)-ux2(pc) + ade(uc),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d ## dir(vc)-uy2(pc) + ade(vc),)
  else
    stop 111
  end if
else if( nd.eq.3 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
  defineDerivativeMacros(3,2,rectangular)

  if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then
    triLoops(3,INS,\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir), \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nu*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)), \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir),,,,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d ## dir(uc)-ux2(pc),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d ## dir(vc)-uy2(pc),\
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d ## dir(wc)-uz2(pc))
  else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
    triLoops(3,INS,\
             $defineArtificialDiffusionCoefficients(3,dir,R,),\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdm, \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nu*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +cdDiag, \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdp,,,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d ## dir(uc)-ux2(pc) +adE3d ## dir(i1,i2,i3,uc),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d ## dir(vc)-uy2(pc) +adE3d ## dir(i1,i2,i3,vc),\
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d ## dir(wc)-uz2(pc) +adE3d ## dir(i1,i2,i3,wc))
  else if( use4thOrderAD.eq.1 )then
    defineAD24(AD24,3,dir)
    defineArtificialDissipationMacro(AD24,3,INS)
    triLoops4(3,INS,\
             $getArtificialDissipationCoeff(AD24,3,INS),\
             am(i1,i2,i3)= adCoeff4,\
             bm(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir)  -adCoeff2 -4.*adCoeff4, \
             cm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nu*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +6.*adCoeff2 + 18.*adCoeff4, \
             dm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir)  -adCoeff2 -4.*adCoeff4,\
             em(i1,i2,i3)= adCoeff4,,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d ## dir(uc)-ux2(pc) + ade(uc),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d ## dir(vc)-uy2(pc) + ade(vc),\
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d ## dir(wc)-uz2(pc) + ade(wc))
  else
    stop 111
  end if
else
  stop 888
end if

#Elif #SOLVER == "INS_TEMPERATURE"
c  ****** INS Temperature equation **********
if( nd.eq.2 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
  defineDerivativeMacros(2,2,rectangular)

  triLoops(2,TEMPERATURE,\
           am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir), \
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*kThermal*(dxvsqi(0)+dxvsqi(1)), \
           cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir),,,,,,,,\
           f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr2d ## dir(tc),,)

else if( nd.eq.3 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
  defineDerivativeMacros(3,2,rectangular)
  triLoops(3,TEMPERATURE,\
           am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir), \
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*kThermal*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)), \
           cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir),,,,,,,,\
           f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftr3d ## dir(tc),,)
else
  stop 889
end if

#Elif #SOLVER == "INSSPAL"

c ******* SA turbulence model (always with AD) *********

if( nd.eq.2 )then
  ! write(*,*) 'fillEquations:case: turbulenceModel.eq.spalartAllmaras'
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(2,SA,\
             $defineArtificialDiffusionCoefficients(2,dir,R,),\
             $defineValuesSA(2,rectangular),\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdm, \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*nuT*(dxvsqi(0)+dxvsqi(1)) +cdDiag, \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdp,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusar2d ## dir(uc)-ux2(pc) +adE ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc)),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusar2d ## dir(vc)-uy2(pc) +adE ## dir(i1,i2,i3,vc)\
                             +nuTx(0)*(uy2(uc)+ux2(vc)) +nuTx(1)*(2.*uy2(vc)),,)
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
c$$$    triLoops(2,SA,\
c$$$             $defineArtificialDiffusionCoefficients(2,dir,R,),\
c$$$             $defineSelfAdjointDiffusionCoefficients(DIM,UV,nuTSA),\
c$$$             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir) -dsam    -cdm, \
c$$$             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +dsaDiag +cdDiag, \
c$$$             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir) -dsap    -cdp,,,,,\
c$$$             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusar2d ## dir(uc)-ux2(pc) + dsau() +adE ## dir(i1,i2,i3,uc), \
c$$$             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusar2d ## dir(vc)-uy2(pc) + dsav() +adE ## dir(i1,i2,i3,vc),,)


  end if
else if( nd.eq.3 )then
  ! SA turbulence model (always with AD)
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(3,SA,\
             $defineArtificialDiffusionCoefficients(3,dir,R,),\
             $defineValuesSA(3,rectangular),\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdm, \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nuT*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +cdDiag, \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdp,,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusar3d ## dir(uc)-ux2(pc) +adE3d ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc))+nuTx(2)*(uz2(uc)+ux2(wc)),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusar3d ## dir(vc)-uy2(pc) +adE3d ## dir(i1,i2,i3,vc) \
                             +nuTx(0)*(uy2(uc)+ux2(vc))+nuTx(1)*(2.*uy2(vc))+nuTx(2)*(uz2(vc)+uy2(wc)),\
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fusar3d ## dir(wc)-uz2(pc) +adE3d ## dir(i1,i2,i3,wc) \
                             +nuTx(0)*(uz2(uc)+ux2(wc))+nuTx(1)*(uz2(vc)+uy2(wc))+nuTx(2)*(2.*uz2(wc)) )
                             
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
  end if
else
  stop 888
end if

#Elif #SOLVER == "INSBL"

c  ***** BL turbulence model (always with AD) *****

if( nd.eq.2 )then
  ! write(*,*) 'fillEquations:case: turbulenceModel.eq.baldwinLomax'
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(2,INS,\
             $defineArtificialDiffusionCoefficients(2,dir,R,),\
             $defineValuesBL(2,rectangular),\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdm, \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*nuT*(dxvsqi(0)+dxvsqi(1)) +cdDiag, \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdp,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusar2d ## dir(uc)-ux2(pc) +adE ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc)),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusar2d ## dir(vc)-uy2(pc) +adE ## dir(i1,i2,i3,vc)\
                             +nuTx(0)*(uy2(uc)+ux2(vc)) +nuTx(1)*(2.*uy2(vc)),,)
                             
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
  end if
else if( nd.eq.3 )then
  ! BL turbulence model (always with AD)
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(3,INS,\
             $defineArtificialDiffusionCoefficients(3,dir,R,),\
             $defineValuesBL(3,rectangular),\
             am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdm, \
             bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nuT*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +cdDiag, \
             cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nuT*dxvsqi(dir) -cdp,,,,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusar3d ## dir(uc)-ux2(pc) +adE3d ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc))+nuTx(2)*(uz2(uc)+ux2(wc)),\
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusar3d ## dir(vc)-uy2(pc) +adE3d ## dir(i1,i2,i3,vc) \
                             +nuTx(0)*(uy2(uc)+ux2(vc))+nuTx(1)*(2.*uy2(vc))+nuTx(2)*(uz2(vc)+uy2(wc)),\
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fusar3d ## dir(wc)-uz2(pc) +adE3d ## dir(i1,i2,i3,wc) \
                             +nuTx(0)*(uz2(uc)+ux2(wc))+nuTx(1)*(uz2(vc)+uy2(wc))+nuTx(2)*(2.*uz2(wc)) )
                             
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
  end if
else
  stop 888
end if

#Else
  write(*,'("ERROR: unknown solver=SOLVER")')
  stop 276
#End
                     
#endMacro

c ===================================================================================
c  SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE 
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsCurvilinearGrid(SOLVER,dir)
dirp1=mod(dir+1,nd)
dirp2=mod(dir+2,nd)

#If #SOLVER == "INS"

c  ****** Incompressible NS, No turbulence model *****

if( nd.eq.2 )then
  defineDerivativeMacros(2,2,curvilinear)
  if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then
    triLoops(2,INS,\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nu*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir),\
             t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+nu*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) ),\
             cm(i1,i2,i3)=  t1-t2,,,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d ## dir(uc)-ux2c(pc), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d ## dir(vc)-uy2c(pc),)
  else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
    triLoops(2,INS,\
             $defineArtificialDiffusionCoefficients(2,dir,C,),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nu*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir),\
             t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2 -cdm,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+nu*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )+cdDiag,\
             cm(i1,i2,i3)=  t1-t2 -cdp,,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d ## dir(uc)-ux2c(pc) +adE ## dir(i1,i2,i3,uc), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d ## dir(vc)-uy2c(pc) +adE ## dir(i1,i2,i3,vc),)
  else if( use4thOrderAD.eq.1 )then
    defineAD24(AD24,2,dir)
    defineArtificialDissipationMacro(AD24,2,INS)
    triLoops4(2,INS,\
             $getArtificialDissipationCoeff(AD24,2,INS),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nu*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir),\
             t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir), \
             am(i1,i2,i3)= adCoeff4,\
             bm(i1,i2,i3)= -t1-t2  -adCoeff2 -4.*adCoeff4,\
             cm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+nu*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )\
                                                               +4.*adCoeff2 + 12.*adCoeff4,\
             dm(i1,i2,i3)=  t1-t2  -adCoeff2 -4.*adCoeff4,\
             em(i1,i2,i3)= adCoeff4 ,,,\
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d ## dir(uc)-ux2c(pc) + ade(uc), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d ## dir(vc)-uy2c(pc) + ade(vc),)
  else
    stop 2222
  end if
else if( nd.eq.3 )then
  defineDerivativeMacros(3,2,curvilinear)

  if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then
    triLoops(3,INS,\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                     -nu*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)))*drv2i(dir),\
             t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) \
               +2.*(t2+nu*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                            (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )),\
             cm(i1,i2,i3)=  t1-t2,,,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d ## dir(uc)-ux3c(pc), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d ## dir(vc)-uy3c(pc), \
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d ## dir(wc)-uz3c(pc))
  else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
    triLoops(3,INS,\
             $defineArtificialDiffusionCoefficients(3,dir,C,),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                     -nu*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)))*drv2i(dir),\
             t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2 -cdm,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) \
               +2.*(t2+nu*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                            (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )) +cdDiag,\
             cm(i1,i2,i3)=  t1-t2 -cdp,,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d ## dir(uc)-ux3c(pc) +adE3d ## dir(i1,i2,i3,uc), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d ## dir(vc)-uy3c(pc) +adE3d ## dir(i1,i2,i3,vc), \
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d ## dir(wc)-uz3c(pc) +adE3d ## dir(i1,i2,i3,wc))
  else if( use4thOrderAD.eq.1 )then
    defineAD24(AD24,3,dir)
    defineArtificialDissipationMacro(AD24,3,INS)
    triLoops4(3,INS,\
             $getArtificialDissipationCoeff(AD24,3,INS),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                     -nu*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)))*drv2i(dir),\
             t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
             am(i1,i2,i3)= adCoeff4,\
             bm(i1,i2,i3)= -t1-t2 -adCoeff2 -4.*adCoeff4,\
             cm(i1,i2,i3)= dtScale/dt(i1,i2,i3) \
               +2.*(t2+nu*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                            (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )) \
                               +6.*adCoeff2 + 18.*adCoeff4,\
             dm(i1,i2,i3)=  t1-t2 -adCoeff2   -4.*adCoeff4, \
             em(i1,i2,i3)=  adCoeff4,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d ## dir(uc)-ux3c(pc) +ade(uc), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d ## dir(vc)-uy3c(pc) +ade(vc), \
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d ## dir(wc)-uz3c(pc) +ade(wc))
  else
    stop 33333
  end if
else
  stop 222
end if

#Elif #SOLVER == "INS_TEMPERATURE"

c  ****** Temperature Equation *****

if( nd.eq.2 )then
  defineDerivativeMacros(2,2,curvilinear)
  triLoops(2,TEMPERATURE,\
           t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-kThermal*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir),\
           t2=kThermal*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir), \
           am(i1,i2,i3)= -t1-t2,\
           bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+kThermal*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) ),\
           cm(i1,i2,i3)=  t1-t2,,,,,, \
           f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc2d ## dir(tc),,)
else if( nd.eq.3 )then
  defineDerivativeMacros(3,2,curvilinear)

  triLoops(3,TEMPERATURE,\
           t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                   -kThermal*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)))*drv2i(dir),\
           t2=kThermal*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
           am(i1,i2,i3)= -t1-t2,\
           bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) \
             +2.*(t2+kThermal*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                                (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )),\
           cm(i1,i2,i3)=  t1-t2,,,,,, \
           f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ftc3d ## dir(tc),,)
else
  stop 223
end if

#Elif #SOLVER == "INSSPAL"

c ******* SA turbulence model (always with AD) *********

if( nd.eq.2 )then
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(2,SA,\
             $defineArtificialDiffusionCoefficients(2,dir,C,),\
             $defineValuesSA(2,curvilinear),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nuT*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir),\
             t2=nuT*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2 -cdm,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+nuT*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )+cdDiag,\
             cm(i1,i2,i3)=  t1-t2 -cdp,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusac2d ## dir(uc)-ux2c(pc) +adE ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux2c(uc))+nuTx(1)*(uy2c(uc)+ux2c(vc)), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusac2d ## dir(vc)-uy2c(pc) +adE ## dir(i1,i2,i3,vc)\
                             +nuTx(0)*(uy2c(uc)+ux2c(vc)) +nuTx(1)*(2.*uy2c(vc)),)
                             
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
  end if
else if( nd.eq.3 )then
    ! SA turbulence model (always with AD)
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(3,SA,\
             $defineArtificialDiffusionCoefficients(3,dir,C,),\
             $defineValuesSA(3,curvilinear),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                     -nuT*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)) )*drv2i(dir),\
             t2=nuT*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2 -cdm,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) \
               +2.*(t2+nuT*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                             (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )) +cdDiag,\
             cm(i1,i2,i3)=  t1-t2 -cdp,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusac3d ## dir(uc)-ux3c(pc) +adE3d ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux3c(uc))+nuTx(1)*(uy3c(uc)+ux3c(vc))+nuTx(2)*(uz3c(uc)+ux3c(wc)), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusac3d ## dir(vc)-uy3c(pc) +adE3d ## dir(i1,i2,i3,vc) \
                             +nuTx(0)*(uy3c(uc)+ux3c(vc))+nuTx(1)*(2.*uy3c(vc))+nuTx(2)*(uz3c(vc)+uy3c(wc)), \
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fusac3d ## dir(wc)-uz3c(pc) +adE3d ## dir(i1,i2,i3,wc) \
                             +nuTx(0)*(uz3c(uc)+ux3c(wc))+nuTx(1)*(uz3c(vc)+uy3c(wc))+nuTx(2)*(2.*uz3c(wc)))
                             
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
  end if
else
  stop 222
end if

#Elif #SOLVER == "INSBL"

c  ***** BL turbulence model (always with AD) *****

if( nd.eq.2 )then
  ! baldwinLomax turbulence model (always with AD)
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(2,INS,\
             $defineArtificialDiffusionCoefficients(2,dir,C,),\
             $defineValuesBL(2,curvilinear),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nuT*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir),\
             t2=nuT*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2 -cdm,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+nuT*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )+cdDiag,\
             cm(i1,i2,i3)=  t1-t2 -cdp,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusac2d ## dir(uc)-ux2c(pc) +adE ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux2c(uc))+nuTx(1)*(uy2c(uc)+ux2c(vc)), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusac2d ## dir(vc)-uy2c(pc) +adE ## dir(i1,i2,i3,vc)\
                             +nuTx(0)*(uy2c(uc)+ux2c(vc)) +nuTx(1)*(2.*uy2c(vc)),)
                             
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
  end if
else if( nd.eq.3 )then
  ! baldwinLomax turbulence model (always with AD)
  if( useSelfAdjointDiffusion.eq.0 )then
    triLoops(3,INS,\
             $defineArtificialDiffusionCoefficients(3,dir,C,),\
             $defineValuesBL(3,curvilinear),\
             t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                     -nuT*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)) )*drv2i(dir),\
             t2=nuT*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
             am(i1,i2,i3)= -t1-t2 -cdm,\
             bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) \
               +2.*(t2+nuT*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                             (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )) +cdDiag,\
             cm(i1,i2,i3)=  t1-t2 -cdp,,,, \
             f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fusac3d ## dir(uc)-ux3c(pc) +adE3d ## dir(i1,i2,i3,uc) \
                             +nuTx(0)*(2.*ux3c(uc))+nuTx(1)*(uy3c(uc)+ux3c(vc))+nuTx(2)*(uz3c(uc)+ux3c(wc)), \
             f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fusac3d ## dir(vc)-uy3c(pc) +adE3d ## dir(i1,i2,i3,vc) \
                             +nuTx(0)*(uy3c(uc)+ux3c(vc))+nuTx(1)*(2.*uy3c(vc))+nuTx(2)*(uz3c(vc)+uy3c(wc)), \
             f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fusac3d ## dir(wc)-uz3c(pc) +adE3d ## dir(i1,i2,i3,wc) \
                             +nuTx(0)*(uz3c(uc)+ux3c(wc))+nuTx(1)*(uz3c(vc)+uy3c(wc))+nuTx(2)*(2.*uz3c(wc)))
                             
  else ! self-adjoint form
    write(*,'("self-adjoint form not implemented yet")')
    stop 11
  end if
else
  stop 222
end if

#Else
  stop 345
#End
#endMacro

c Macro to define the set of computations required to compute values for the SA turbulence model.
c used in the macros below
#beginMacro setupSA(dim,gt)
 chi=uu(nc)/nu
 chi3=chi**3
 fnu1=chi3/( chi3+cv1e3)
 fnu2=1.-chi/(1.+chi*fnu1)
 dd = dw(i1,i2,i3)+cd0  
 dKappaSq=(dd*kappa)**2
#If #dim == "2"
  #If #gt == "rectangular"
    s=abs(uy2(uc)-ux2(vc))+ uu(nc)*fnu2/dKappaSq ! turbulence source term 
  #Else
    s=abs(uy2c(uc)-ux2c(vc))+ uu(nc)*fnu2/dKappaSq ! turbulence source term 
  #End
#Else
  #If #gt == "rectangular"
    s=uu(nc)*fnu2/dKappaSq \
     +sqrt( (uy2(uc)-ux2(vc))**2 + (uz2(vc)-uy2(wc))**2 + (ux2(wc)-uz2(uc))**2 )
  #Else
    s=uu(nc)*fnu2/dKappaSq \
     +sqrt( (uy3c(uc)-ux3c(vc))**2 + (uz3c(vc)-uy3c(wc))**2 + (ux3c(wc)-uz3c(uc))**2 )
  #End
#End
 r= min( uu(nc)/( s*dKappaSq ), cr0 )   !  r= uu(nc)/( max( s*dKappaSq, 1.e-20) )
 g=r+cw2*(r**6-r)
 fw=g*( (1.+cw3e6)/(g**6+cw3e6) )**(1./6.)
 ! We use Newton to linearize the quadratic term: y*y -> 2*y*y0 - y0**2
 nSqBydSq=cw1*fw*(uu(nc)/dd)**2     ! for rhs
 nBydSqLhs=2.*cw1*fw*(uu(nc)/dd**2) ! for lhs
 nutb=sigmai*(nu+uu(nc))
#If #dim == "2"
  #If #gt == "rectangular"
   dndx(0)=ux2(nc)
   dndx(1)=uy2(nc)
  #Else
   dndx(0)=ux2c(nc)
   dndx(1)=uy2c(nc)
  #End 
#Else
  #If #gt == "rectangular"
   dndx(0)=ux2(nc)
   dndx(1)=uy2(nc)
   dndx(2)=uz2(nc)
  #Else
   dndx(0)=ux3c(nc)
   dndx(1)=uy3c(nc)
   dndx(2)=uz3c(nc)
  #End 
#End
#endMacro



c Macro for the SA TM on rectangular grids
c Only the equation for the turbulence eddy viscosity is done here
#beginMacro fillSAEquationsRectangularGrid(dir)
if( nd.eq.2 )then
  triLoops(2,SA,\
           $defineArtificialDiffusionCoefficients(2,dir,R,SA),\
           $setupSA(2,rectangular),\
           t1=-(1.+cb2)*sigmai*dndx(dir),\
           am(i1,i2,i3)= -(uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nutb*(dxvsqi(0)+dxvsqi(1)) +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  (uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdp,,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsa2d ## dir(nc) + nSqBydSq + adE ## dir(i1,i2,i3,nc),,)
else ! 3d
  triLoops(3,SA,\
           $defineArtificialDiffusionCoefficients(3,dir,R,SA),\
           $setupSA(3,rectangular),\
           t1=-(1.+cb2)*sigmai*dndx(dir),\
           am(i1,i2,i3)= -(uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nutb*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  (uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdp,,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsa3d ## dir(nc) + nSqBydSq +adE3d ## dir(i1,i2,i3,nc),,)
end if
#endMacro


#beginMacro fillSAEquationsCurvilinearGrid(dir)
dirp1=mod(dir+1,nd)
dirp2=mod(dir+2,nd)
if( nd.eq.2 )then
  triLoops(2,SA,\
           $defineArtificialDiffusionCoefficients(2,dir,C,SA),\
           $setupSA(2,curvilinear),\
           t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nutb*(rxx(dir,0)+rxy(dir,1))\
                  -(1.+cb2)*sigmai*(dndx(0)*rxi(dir,0)+dndx(1)*rxi(dir,1)) )*drv2i(dir),\
           t2=nutb*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir),\
           am(i1,i2,i3)= -t1-t2 -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*(t2+nutb*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )\
                           +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  t1-t2 -cdp,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsac2d ## dir(nc) + nSqBydSq +adE ## dir(i1,i2,i3,nc),,)
else ! 3d
  triLoops(3,SA,\
           $defineArtificialDiffusionCoefficients(3,dir,C,SA),\
           $setupSA(3,curvilinear),\
           t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                     -nutb*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)) \
                     -(1.+cb2)*sigmai*(dndx(0)*rxi(dir,0)+dndx(1)*rxi(dir,1)+dndx(2)*rxi(dir,2)) )*drv2i(dir),\
           t2=nutb*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
           am(i1,i2,i3)= -t1-t2 -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)\
               +2.*(t2+nutb*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                              (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) ))\
                           +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  t1-t2 -cdp,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsac3d ## dir(nc) + nSqBydSq +adE3d ## dir(i1,i2,i3,nc),,)
end if
#endMacro


c **************************************************************
c   Macro to compute Baldwin-Lomax Turbulent viscosity
c **************************************************************
#beginMacro computeBLNuT()

      maxvt=0
      indexRange(0,0)=n1a
      indexRange(1,0)=n1b
      indexRange(0,1)=n2a
      indexRange(1,1)=n2b
      indexRange(0,2)=n3a
      indexRange(1,2)=n3b
      ! assign loop variables to correspond to the boundary

      do axis=0,nd-1
      do side=0,1
c         write(*,*) "SIDE, AXIS, BC ",side,axis,boundaryCondition(side,axis)
         if( boundaryCondition(side,axis).eq.noslipwall )then
            is1=0
            is2=0
            is3=0
            if( axis.eq.0 )then
               is1=1-2*side
               n1a=indexRange(side,axis) !-is1 ! boundary is 1 pt outside
               n1b=n1a
            else if( axis.eq.1 )then
               is2=1-2*side
               n2a=indexRange(side,axis) !-is2
               n2b=n2a
            else
               is3=1-2*side
               n3a=indexRange(side,axis) !-is3
               n3b=n3a
            end if

            io(1)=0
            io(2)=0
            io(3)=0
            io(axis+1)=1-2*side

            ibb=indexRange(0,axis)
            ibe=indexRange(1,axis)-1
c            write(*,*) ibb,ibe

            do ii3=n3a,n3b
            do ii2=n2a,n2b
            do ii1=n1a,n1b

            if ( ii3.ge.ktrip .and. ii2.ge.jtrip .and. ii1.ge.itrip ) then
             i1 = ii1
             i2 = ii2
             i3 = ii3

             if ( nd.eq.2 ) then
                if ( axis.eq.0 ) then
                   ditrip = ii2-jtrip
                else 
                   ditrip = ii1-itrip
                endif
             else
                if ( axis.eq.0 ) then
                   ditrip = min((ii3-ktrip),(ii2-jtrip))
                else if ( axis.eq.1 ) then
                   ditrip = min((ii1-itrip),(ii3-ktrip))
                else
                   ditrip = min((ii1-itrip),(ii2-jtrip))
                endif
             endif

             ctrans = (1-exp(-ditrip/3.))**2
c             ctrans=1
c             write(*,*) i1,i2,i3,ctrans
            norm(1) = 0
            norm(2) = 0
            norm(3) = 0

            norm(1) = rxi(axis,0)
            norm(2) = rxi(axis,1)
            if ( nd.eq.3 )norm(3) = rxi(axis,2)

            nmag=sqrt(norm(1)*norm(1)+norm(2)*norm(2)+norm(3)*norm(3))

            norm(1) = norm(1)/nmag
            norm(2) = norm(2)/nmag
            norm(3) = norm(3)/nmag

            ftan(1) = 0
            ftan(2) = 0
            ftan(3) = 0

            if ( nd.eq.2 ) then

               if ( gridType.eq.rectangular ) then

                ftan(1) = 2*norm(1)*ux2(uc) + norm(2)*(ux2(vc)+uy2(uc))
                ftan(2) = norm(1)*(uy2(uc)+ux2(vc)) + 2*norm(2)*uy2(vc)

               else

             ftan(1) = 2*norm(1)*ux2c(uc) + norm(2)*(ux2c(vc)+uy2c(uc))
             ftan(2) = norm(1)*(uy2c(uc)+ux2c(vc)) + 2*norm(2)*uy2c(vc)

               end if
               
            else
               
               if ( gridType.eq.rectangular ) then

                 ftan(1)=2*norm(1)*ux2(uc)+norm(2)*(ux2(vc)+uy2(uc)) + norm(3)*(ux2(wc)+uz2(uc))

                 ftan(2)=norm(1)*(ux2(vc)+uy2(uc)) + 2*norm(2)*uy2(vc) + norm(3)*(uy2(wc)+uz2(vc))

                 ftan(3)=norm(1)*(ux2(wc)+uz2(uc)) + norm(2)*(uy2(wc)+uz2(vc)) + 2*norm(3)*uz2(wc)

               else

                  ftan(1)=2*norm(1)*ux3c(uc)+ norm(2)*(ux3c(vc)+uy3c(uc)) + norm(3)*(ux3c(wc)+uz3c(uc))
                  
                  ftan(2)=norm(1)*(ux3c(vc)+uy3c(uc)) + 2*norm(2)*uy3c(vc) +  norm(3)*(uy3c(wc)+uz3c(vc))
                  
                  ftan(3)=norm(1)*(ux3c(wc)+uz3c(uc)) + norm(2)*(uy3c(wc)+uz3c(vc)) + 2*norm(3)*uz3c(wc)
                  
               end if

            end if

            fdotn = ftan(1)*norm(1)+ftan(2)*norm(2)+ftan(3)*norm(3)

            
            ftan(1) = ftan(1) - norm(1)*fdotn
            ftan(2) = ftan(2) - norm(2)*fdotn
            ftan(3) = ftan(3) - norm(3)*fdotn

          tauw=nu*sqrt(ftan(1)*ftan(1)+ftan(2)*ftan(2)+ftan(3)*ftan(3))
             
c         yplus = y*yscale
          yscale = sqrt(tauw)/nu ! assuming density=1 here...

          ymax=0
          lmixmax=0
          lmix2max=0

          maxumag=0
          ulmax=0

          do i=ibb,ibe

             i1 = ii1 + io(1)*i
             i2 = ii2 + io(2)*i
             i3 = ii3 + io(3)*i
             u(i1,i2,i3,nc) = 0

             if (gridType.eq.rectangular) then
                if (nd.eq.2) then
                   vort = abs(ux2(vc)-uy2(uc))
                else
                   vort = sqrt( (uy2(wc)-uz2(vc))*(uy2(wc)-uz2(vc)) - (ux2(wc)-uz2(uc))*(ux2(wc)-uz2(uc)) + (ux2(vc)-uy2(uc))*(ux2(vc)-uy2(uc)) )
                end if
             else
                if (nd.eq.2) then
                   vort = abs(ux2c(vc)-uy2c(uc))
                else
                   vort = sqrt( (uy3c(wc)-uz3c(vc))*(uy3c(wc)-uz3c(vc))- (ux3c(wc)-uz3c(uc))*(ux3c(wc)-uz3c(uc))+ (ux3c(vc)-uy3c(uc))*(ux3c(vc)-uy3c(uc)))
                end if
             end if                

             yplus = dw(i1,i2,i3)*yscale
             lmixw = vort* kbl*kbl*dw(i1,i2,i3)*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))**2
             
c             write(*,*) "yplus, vort ",yplus, vort
c             write(*,*) "dw, yscale, yplus, lmixw  is ",dw(i1,i2,i3),"  ",yscale," ",yplus," " ,lmixw
             magu = u(i1,i2,i3,uc)*u(i2,i2,i3,uc) + u(i1,i2,i3,vc)*u(i1,i2,i3,vc) 

             if ( nd.eq.3 ) magu = magu + u(i1,i2,i3,wc)*u(i1,i2,i3,wc)
             
             magumax = max(magu,maxumag)

             if ( (vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))).gt.lmixmax ) then
                ymax = dw(i1,i2,i3)
                ulmax = magu
                lmixmax = vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))
                lmix2max = lmixw
c                write(*,*) "--",i,ymax,lmixmax,lmix2max
             end if
                
             u(i1,i2,i3,nc) = lmixw

          end do ! i=ibb,ibe
   
c         now that we know lmixmax, ulmax and maxumag we can compute the eddy viscosity

          magumax = sqrt(magumax)
          ulmax = sqrt(ulmax)

c          write(*,*) "ymax is ",ymax," lmix2max ",lmix2max
          iswitch=0
          do i=ibb,ibe
             
             i1 = ii1 + io(1)*i
             i2 = ii2 + io(2)*i
             i3 = ii3 + io(3)*i

             vto = alpha*ccp*min(ymax*lmixmax/kbl, cwk*ymax*(maxumag-ulmax)*(maxumag-ulmax)*kbl/lmixmax) / (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
c             vto = alpha*ccp*ymax*lmixmax/kbl/(1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
c             write(*,*) ymax,dw(i1,i2,i3)
c             write(*,*) (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
c             write(*,*) "i,j,k, yplus, vti, vto ",i1,i2,i3,dw(i1,i2,i3)*yscale,u(i1,i2,i3,nc), vto

c             write(*,*) yscale*dw(i1,i2,i3),u(i1,i2,i3,nc),vto,iswitch
             if ( (iswitch.eq.0 .and. vto.lt.u(i1,i2,i3,nc)).or. iswitch.gt.0 ) then
c                write(*,*) "switched at ",i, u(i1,i2,i3,nc), vto
                u(i1,i2,i3,nc) = vto 

                if ( iswitch.eq.0 ) iswitch = i
             endif

             u(i1,i2,i3,nc) = ctrans*u(i1,i2,i3,nc)
             maxvt = max(maxvt,u(i1,i2,i3,nc))

          end do ! i=ibb,ibe

          ! smooth the eddy viscosity a bit near the switch from inner to outter solutions
          do i=max(ibb+1,iswitch-5),min(iswitch+5,ibe-2)

             i1 = ii1 + io(1)*i
             i2 = ii2 + io(2)*i
             i3 = ii3 + io(3)*i

c            yes, the relaxation coeff. is 1.  I'm just setting it equal to the neighbors now
c            yes, the i+1 node uses the updated version of the i node's value             
             u(i1,i2,i3,nc) = .5*(u(i1+io(1),i2+io(2),i3+io(3),nc)+u(i1-io(1),i2-io(2),i3-io(3),nc))

c            also, it seems the region for this smoothing should increase as the boundary
c            layer increases in order to improve convergence.  +- 5 was chosen through trial and
c            error but could be made a function of iswitch or ymax for instance.
          enddo

          else
             do i=ibb,ibe
                i1 = ii1 + io(1)*i
                i2 = ii2 + io(2)*i
                i3 = ii3 + io(3)*i

                u(i1,i2,i3,nc) = 0
             end do
          end if

          end do ! i3=i3a,i3b
          end do ! i2=i2a,2b
          end do ! i1=i1a,i1b

            ! reset values
            if( axis.eq.0 )then
               n1a=indexRange(0,axis)
               n1b=indexRange(1,axis)
            else if( axis.eq.1 )then
               n2a=indexRange(0,axis)
               n2b=indexRange(1,axis)
            else
               n3a=indexRange(0,axis)
               n3b=indexRange(1,axis)
            end if
            
         end if                 !bc
         
      end do                    ! do side
      end do                    ! do axis
      
c      write(*,*) "maxvt is ",maxvt
#endMacro

c Here are the statements we use to initialize the main subroutines below
#beginMacro INITIALIZE()
      pc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      grid              =ipar(4)
      orderOfAccuracy   =ipar(5)
      gridIsMoving      =ipar(6)
      useWhereMask      =ipar(7)
      gridIsImplicit    =ipar(8)
      implicitMethod    =ipar(9)
      implicitOption    =ipar(10)
      isAxisymmetric    =ipar(11)
      use2ndOrderAD     =ipar(12)
      use4thOrderAD     =ipar(13)
      gridType          =ipar(14)

      computeMatrix     =ipar(15)
      computeRHS        =ipar(16)
      computeMatrixBC   =ipar(17)
      fc                =ipar(18)
      fcu=fc
      fcv=fc+1
      fcw=fc+2
      fcn=fc+nd
      fct=fc+nd
      orderOfExtrapolation=ipar(19)
      ibc               = ipar(20)

      option            = ipar(21)
      nc                = ipar(22)
      turbulenceModel   = ipar(23)
      twilightZoneFlow  = ipar(24)

      useSelfAdjointDiffusion=ipar(25)
      fourthOrder       = ipar(26)
      pdeModel          = ipar(27)
      tc                = ipar(28)
      numberOfComponents= ipar(29)

      dx(0)            =rpar(0)
      dx(1)            =rpar(1)
      dx(2)            =rpar(2)
      nu                =rpar(3)
      ad21              =rpar(4)
      ad22              =rpar(5)
      ad41              =rpar(6)
      ad42              =rpar(7)

      dr(0)             =rpar(8)
      dr(1)             =rpar(9)
      dr(2)             =rpar(10)
      cfl               =rpar(11)
      ad21n             =rpar(12)
      ad22n             =rpar(13)
      ad41n             =rpar(14)
      ad42n             =rpar(15)
      kThermal          =rpar(16)
      
      thermalExpansivity=rpar(17)
      gravity(0)        =rpar(18)
      gravity(1)        =rpar(19)
      gravity(2)        =rpar(20)


      computeTemperature = 0
      if( pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel )then
        computeTemperature=1
      else
        tc=uc ! give this default value to tc so we can always add a gravity term, even if there is no T equation
        thermalExpansivity=0.   ! set to zero to turn off the gravity term 
      end if


      do m=0,2
       dxv2i(m)=1./(2.*dx(m))
       dxvsqi(m)=1./(dx(m)**2)
       drv2i(m)=1./(2.*dr(m))
       drvsqi(m)=1./(dr(m)**2)
      end do

      dx0=dx(0)
      dy=dx(1)
      dz=dx(2)
      dx2i=1./(2.*dx0)
      dy2i=1./(2.*dy)
      dz2i=1./(2.*dz)
      dxsqi=1./(dx0*dx0)
      dysqi=1./(dy*dy)
      dzsqi=1./(dz*dz)

      dr2i=1./(2.*dr(0))
      ds2i=1./(2.*dr(1))
      dt2i=1./(2.*dr(2))
      drsqi=1./(dr(0)**2)
      dssqi=1./(dr(1)**2)
      dtsqi=1./(dr(2)**2)
 
      dxi=1./dx0
      dyi=1./dy
      dzi=1./dz
      dri=1./dr(0)
      dsi=1./dr(1)
      dti=1./dr(2)

      if( orderOfAccuracy.eq.4 )then
        dx12i=1./(12.*dx0)
        dy12i=1./(12.*dy)
        dz12i=1./(12.*dz)
        dxsq12i=1./(12.*dx0**2)
        dysq12i=1./(12.*dy**2)
        dzsq12i=1./(12.*dz**2)
      end if

      cd22=ad22/(nd**2) 
      cd42=ad42/(nd**2)

      cd22n=ad22n/nd     ! for the SA TM model
      cd42n=ad42n/nd

c      write(*,*) 'insLineSolve: use2ndOrderAD,ad21,cd22=',
c     & use2ndOrderAD,ad21,cd22

      dtScale=1./cfl

      if( fourthOrder.eq.1 .and. turbulenceModel.ne.noTurbulenceModel )then
        write(*,'("insLineSolve: ERROR: fourth-order only available for INS")')
        ! " '
        stop 6543
      end if

      if( turbulenceModel.eq.spalartAllmaras )then
        call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, \
           cv1e3, cd0, cr0)
      else if( turbulenceModel.eq.kEpsilon )then

c**        call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )

      else if( turbulenceModel.ne.noTurbulenceModel )then
        stop 88
      end if


      if( turbulenceModel.eq.baldwinLomax )then
         ! assign constants for baldwin-lomax
         kbl=.4
         alpha=.0168
         a0p=26.
c         ccp=1.6
         ccp=2.6619
         ckleb=0.3
         cwk=.25
c         cwk=1
      end if

      itrip = ipar(50)
      jtrip = ipar(51)
      ktrip = ipar(52)


#endMacro

c ===========================================================
c SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE
c GRIDTYPE: rectangular, curvilinear
c ===========================================================
#beginMacro fillEquations(SOLVER)
 if( gridType.eq.rectangular )then
   ! *******************************************
   ! ************** rectangular  ***************
   ! *******************************************
   if( orderOfAccuracy.eq.2 )then
     if( dir.eq.0 )then
      fillEquationsRectangularGrid(SOLVER,0)
     else if( dir.eq.1 )then
      fillEquationsRectangularGrid(SOLVER,1)
    else ! dir.eq.2
      fillEquationsRectangularGrid(SOLVER,2)
    end if ! end dir
   else ! order==4
   end if
 else if( gridType.eq.curvilinear )then
   ! *******************************************
   ! ************** curvilinear  ***************
   ! *******************************************
   if( orderOfAccuracy.eq.2 )then
     if( dir.eq.0 )then
      fillEquationsCurvilinearGrid(SOLVER,0)
     else if( dir.eq.1 )then
      fillEquationsCurvilinearGrid(SOLVER,1)
    else ! dir.eq.2
      fillEquationsCurvilinearGrid(SOLVER,2)
    end if ! end dir
   else ! order==4
   end if
 else
   stop 111
 end if

#endMacro

#beginMacro assignDirichletFourthOrder()
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
 am(i1,i2,i3)=0.
 bm(i1,i2,i3)=0.
 cm(i1,i2,i3)=1.
 dm(i1,i2,i3)=0.
 em(i1,i2,i3)=0.
 am(i1-is1,i2-is2,i3-is3)=0.
 bm(i1-is1,i2-is2,i3-is3)=0.
 cm(i1-is1,i2-is2,i3-is3)=1.
 dm(i1-is1,i2-is2,i3-is3)=0.
 em(i1-is1,i2-is2,i3-is3)=0.
#endMacro

#beginMacro assignFourthOrder()
 am(i1,i2,i3)=cexa
 bm(i1,i2,i3)=cexb
 cm(i1,i2,i3)=cexc
 dm(i1,i2,i3)=cexd
 em(i1,i2,i3)=cexe
 am(i1-is1,i2-is2,i3-is3)=c4exa
 bm(i1-is1,i2-is2,i3-is3)=c4exb
 cm(i1-is1,i2-is2,i3-is3)=c4exc
 dm(i1-is1,i2-is2,i3-is3)=c4exd
 em(i1-is1,i2-is2,i3-is3)=c4exe
#endMacro


c =======================================================================
c Define the subroutine that builds the tridiagonal matrxi for a 
c given solver
c
c SOLVER: INS, INSSPAL, INSBL
c=======================================================================
#beginMacro INS_LINE_SETUP(SOLVER,NAME)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
      md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw,\
      dir,am,bm,cm,dm,em,  bc, boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c 
c dw: distance to wall for SA TM
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
  md1a,md1b,md2a,md2b,md3a,md3b,dir

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

 real am(md1a:md1b,md2a:md2b,md3a:md3b)
 real bm(md1a:md1b,md2a:md2b,md3a:md3b)
 real cm(md1a:md1b,md2a:md2b,md3a:md3b)
 real dm(md1a:md1b,md2a:md2b,md3a:md3b)
 real em(md1a:md1b,md2a:md2b,md3a:md3b)

 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

 real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr
 real dtScale,cfl

 ! bcData(component+numberOfComponents*(0),side,axis,grid)
 integer numberOfComponents
 integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
 real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

 integer ipar(0:*)
 real rpar(0:*)
 
 !     ---- local variables -----
 integer m,n,c,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask
 integer gridIsImplicit,implicitOption,implicitMethod,ibc,\
 isAxisymmetric,use2ndOrderAD,use4thOrderAD,useSelfAdjointDiffusion,\
 orderOfExtrapolation,fourthOrder,dirp1,dirp2
 integer pc,uc,vc,wc,tc,fc,fcu,fcv,fcw,fcn,fct,grid,side,gridType
 integer computeMatrix,computeRHS,computeMatrixBC
 integer twilightZoneFlow,computeTemperature
 integer indexRange(0:1,0:2),is1,is2,is3
 real nu,kThermal,thermalExpansivity,gravity(0:2)
 real dx(0:2),dx0,dy,dz,dxi,dyi,dzi,dri,dsi,dti
 real dxv2i(0:2),dx2i,dy2i,dz2i
 real dxvsqi(0:2),dxsqi,dysqi,dzsqi
 real drv2i(0:2),dr2i,ds2i,dt2i
 real drvsqi(0:2),drsqi,dssqi,dtsqi
 real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,dyz4i
 real ad21,ad22,ad41,ad42,cd22,cd42,adc,sn
 real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
 real dr(0:2)

 real adCoeff2,adCoeff4
 real cexa,cexb,cexc,cexd,cexe
 real c4exa,c4exb,c4exc,c4exd,c4exe

 integer option
 integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
 parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

 real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
 real dd,dndx(0:2)

 integer axis,kd
 real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
 real magu,magumax,ymax,ulmax,lmixw,lmixmax,lmix2max,vto,vort,fdotn,tawu ! baldwin-lomax tmp variables
 real yscale,yplus,nmag,ftan(3),norm(3),tauw,maxumag,maxvt,ctrans,ditrip ! more baldwin-lomax tmp variables
 integer iswitch, ibb, ibe, i, ii1,ii2,ii3,io(3) ! baldwin-lomax loop variables
 integer itrip,jtrip,ktrip !baldwin-lomax trip location
 real chi,fnu1,fnu2,s,r,g,fw,dKappaSq,nBydSqLhs,nSqBydSq,nutb
 real nuTilde,nuT,nuTx(0:2),fv1,fv1x,fv1y,fv1z
 real nuTSA,chi3,nuTd

 real urr0,uss0,utt0

 integer nc
 real fsa2d0,fsa2d1,fsa2d2
 real fsa3d0,fsa3d1,fsa3d2
 real fsac2d0,fsac2d1,fsac2d2
 real fsac3d0,fsac3d1,fsac3d2

 real fusar2d0,fusar2d1,fusar2d2
 real fusar3d0,fusar3d1,fusar3d2
 real fusac2d0,fusac2d1,fusac2d2
 real fusac3d0,fusac3d1,fusac3d2


 integer \
     noSlipWall,\
     outflow,\
     convectiveOutflow,\
     tractionFree,\
     inflowWithPandTV,\
     dirichletBoundaryCondition,\
     symmetry,\
     axisymmetric
 parameter( noSlipWall=1,outflow=5,convectiveOutflow=14,tractionFree=15,\
  inflowWithPandTV=3,dirichletBoundaryCondition=12,symmetry=11,axisymmetric=13 )

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer interpolate,dirichlet,neumann,extrapolate
 parameter( interpolate=0, dirichlet=1, neumann=2, extrapolate=3 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

 !     --- begin statement functions
 real t1,t2
 real fr2d0,fr2d1,fr2d2,fc2d0,fc2d1,fc2d2
 ! real resr2d0,resr2d1
 real fr3d0,fr3d1,fr3d2,fc3d0,fc3d1,fc3d2
 real ftr2d0,ftr2d1,ftr2d2,ftc2d0,ftc2d1,ftc2d2
 real ftr3d0,ftr3d1,ftr3d2,ftc3d0,ftc3d1,ftc3d2
 real uAve0,uAve1,uAve2,uAve3d0,uAve3d1,uAve3d2
 real ad2Coeff,ad2,ad23Coeff,ad23,ad4Coeff,ad4,ad43Coeff,ad43
 real ad2cCoeff,ad23cCoeff
 real ad2nCoeff,ad23nCoeff,ad2cnCoeff,ad23cnCoeff

 real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
 real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
 real udmzC,udzmC,udmzzC,udzmzC,udzzmC
 real admzR,adzmR,admzzR,adzmzR,adzzmR
 real admzC,adzmC,admzzC,adzmzC,adzzmC
 real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
 real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
 real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f
 real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
 real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

 real rxi,rxr,rxs,rxt,rxx,rxy,ryy,rxx3,rxy3,rxz3
 real ur,us,ut,urs,urt,ust,urr,uss,utt
 real uxx0,uyy0,uzz0,ux2c,uy2c,ux3c,uy3c,uz3c
 real lap2d2c,lap3d2c

 real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
 real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,uyz4,uyy4,uzz4

 real mixedRHS,mixedCoeff,mixedNormalCoeff,a0,a1

 real rx,ry,rz,sx,sy,sz,tx,ty,tz

! include 'declareDiffOrder2f.h'
! include 'declareDiffOrder4f.h'
 declareDifferenceOrder2(u,RX)
 declareDifferenceOrder4(u,RX)

 !.......statement functions for jacobian
 rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
 ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
 rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
 sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
 sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
 sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
 tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
 ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
 tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

 !     The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)

 !*      include 'insDeriv.h'
 !*      include 'insDerivc.h'


 uu(c)    = u(i1,i2,i3,c)

 ux2(c)   = ux22r(i1,i2,i3,c)
 uy2(c)   = uy22r(i1,i2,i3,c)
 uz2(c)   = uz23r(i1,i2,i3,c)
 uxy2(c)  = uxy22r(i1,i2,i3,c)
 uxz2(c)  = uxz23r(i1,i2,i3,c) 
 uyz2(c)  = uyz23r(i1,i2,i3,c) 
 uxx2(c)  = uxx22r(i1,i2,i3,c) 
 uyy2(c)  = uyy22r(i1,i2,i3,c) 
 uzz2(c)  = uzz23r(i1,i2,i3,c) 
 lap2d2(c)= ulaplacian22r(i1,i2,i3,c)
 lap3d2(c)= ulaplacian23r(i1,i2,i3,c)

 ux4(c)   = ux42r(i1,i2,i3,c)
 uy4(c)   = uy42r(i1,i2,i3,c)
 uz4(c)   = uz43r(i1,i2,i3,c)
 uxy4(c)  = uxy42r(i1,i2,i3,c)
 uxz4(c)  = uxz43r(i1,i2,i3,c) 
 uyz4(c)  = uyz43r(i1,i2,i3,c) 
 uxx4(c)  = uxx42r(i1,i2,i3,c) 
 uyy4(c)  = uyy42r(i1,i2,i3,c) 
 uzz4(c)  = uzz43r(i1,i2,i3,c) 
 lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
 lap3d4(c)= ulaplacian43r(i1,i2,i3,c)

 rxi(m,n) = rsxy(i1,i2,i3,m,n)
 rxr(m,n) = (rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*dr2i
 rxs(m,n) = (rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*ds2i
 rxt(m,n) = (rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))*dt2i
 rxx(m,n) = rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)
 rxy(m,n) = rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)
 ryy(m,n) = rxy(m,n)

 rxx3(m,n)= rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)+rxi(2,0)*rxt(m,n)
 rxy3(m,n)= rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)+rxi(2,1)*rxt(m,n)
 rxz3(m,n)= rxi(0,2)*rxr(m,n)+rxi(1,2)*rxs(m,n)+rxi(2,2)*rxt(m,n)

 ur(m) = ur2(i1,i2,i3,m)
 us(m) = us2(i1,i2,i3,m) 
 ut(m) = ut2(i1,i2,i3,m) 
 urs(m)= urs2(i1,i2,i3,m) 
 urt(m)= urt2(i1,i2,i3,m) 
 ust(m)= ust2(i1,i2,i3,m) 
 urr(m)= urr2(i1,i2,i3,m)
 uss(m)= uss2(i1,i2,i3,m)
 utt(m)= utt2(i1,i2,i3,m)

 ux2c(m) = ux22(i1,i2,i3,m)
 uy2c(m) = uy22(i1,i2,i3,m)

 ux3c(m) = ux23(i1,i2,i3,m)
 uy3c(m) = uy23(i1,i2,i3,m)
 uz3c(m) = uz23(i1,i2,i3,m)

 lap2d2c(m) = ulaplacian22(i1,i2,i3,m)
 lap3d2c(m) = ulaplacian23(i1,i2,i3,m)                            



 !      ux(c) = (u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c))*dx2i
 !      uy(c) = (u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c))*dy2i
 !      uxx(c) = (u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi
 !      uyy(c) = (u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))*dysqi
 uxx0(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi  ! without diagonal term
 uyy0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))*dysqi  ! without diagonal term
 uzz0(c) = (u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))*dzsqi  ! without diagonal term

 urr0(m)  = (u(i1+1,i2,i3,m)+u(i1-1,i2,i3,m))*drsqi  ! without diagonal term
 uss0(m)  = (u(i1,i2+1,i3,m)+u(i1,i2-1,i3,m))*dssqi  ! without diagonal term
 utt0(m)  = (u(i1,i2,i3+1,m)+u(i1,i2,i3-1,m))*dtsqi  ! without diagonal term

 uAve0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
 uAve1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))
 uAve2(c) = 0.

 uAve3d0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
 uAve3d1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
 uAve3d2(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))

! resr2d0(m) = f(i1,i2,i3,0) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*lap2d2(m)
! resr2d1(m) = f(i1,i2,i3,1) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*lap2d2(m)

 ! INS - RHS forcing for rectangular grids, directions=0,1,2 (do NOT include grad(p) terms, since then macro is valid for m=uc,vc,wc)
 fr2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+nu*uyy0(m) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 fr2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+nu*uxx0(m) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 fr2d2(m) = 0.

 fr3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+nu*(uyy0(m)+uzz0(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 fr3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+nu*(uxx0(m)+uzz0(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 fr3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*(uxx0(m)+uyy0(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)

 ! Temperature - RHS, forcing for rectangular grids, directions=0,1,2
 ftr2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+kThermal*uyy0(m)
 ftr2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+kThermal*uxx0(m)
 ftr2d2(m) = 0.

 ftr3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+kThermal*(uyy0(m)+uzz0(m))
 ftr3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+kThermal*(uxx0(m)+uzz0(m))
 ftr3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+kThermal*(uxx0(m)+uyy0(m))

 ! INS - RHS forcing for curvilinear grids, directions=0,1,2  (do NOT include grad(p) terms)
 fc2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+nu*(rxx(1,0)+ryy(1,1)))*us(m)+\
  nu*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)

 fc2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+nu*(rxx(0,0)+ryy(0,1)))*ur(m)+\
  nu*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 fc2d2(m) = 0.

 fc3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nu*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)

 fc3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nu*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)

 fc3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  nu*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)

 ! Temperature - RHS forcing for curvilinear grids, directions=0,1,2
 ftc2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+kThermal*(rxx(1,0)+ryy(1,1)))*us(m)+\
  kThermal*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))

 ftc2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+kThermal*(rxx(0,0)+ryy(0,1)))*ur(m)+\
  kThermal*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))
 ftc2d2(m) = 0.

 ftc3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  kThermal*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 ftc3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  kThermal*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 ftc3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  kThermal*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 !     --- for SA TM ---
 nuTSA(i1,i2,i3)=u(i1,i2,i3,nc)*(u(i1,i2,i3,nc)/nu)**3/((u(i1,i2,i3,nc)/nu)**3+cv1e3)

 fusar2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+nuT*uyy0(m)
 fusar2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+nuT*uxx0(m) 
 fusar2d2(m) = 0.

 fusar3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+nuT*(uyy0(m)+uzz0(m)) 
 fusar3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+nuT*(uxx0(m)+uzz0(m))
 fusar3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nuT*(uxx0(m)+uyy0(m)) 

 fsa2d0(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*dndx(1)+nutb*uyy0(nc)+(1.+cb2)*sigmai*dndx(1)**2
 fsa2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)+nutb*uxx0(nc)+(1.+cb2)*sigmai*dndx(0)**2
 fsa2d2(m)=0.

 fsa3d0(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*dndx(1)-uu(wc)*dndx(2)+nutb*(uyy0(nc)+uzz0(nc))\
          +(1.+cb2)*sigmai*(dndx(1)**2+dndx(2)**2)
 fsa3d1(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)-uu(wc)*dndx(2)+nutb*(uxx0(nc)+uzz0(nc))\
          +(1.+cb2)*sigmai*(dndx(0)**2+dndx(2)**2)
 fsa3d2(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)-uu(vc)*dndx(1)+nutb*(uxx0(nc)+uyy0(nc))\
          +(1.+cb2)*sigmai*(dndx(0)**2+dndx(1)**2)

 !     --- SA TM curvilinear grid case ----    

 ! The momentum equations are the same as above but with nu replaced by nuT
 fusac2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+nuT*(rxx(1,0)+ryy(1,1)))*us(m)+\
  nuT*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))

 fusac2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+nuT*(rxx(0,0)+ryy(0,1)))*ur(m)+\
  nuT*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))
 fusac2d2(m) = 0.

 fusac3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nuT*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nuT*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nuT*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 fusac3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nuT*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nuT*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nuT*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 fusac3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nuT*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nuT*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  nuT*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 fsac2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(1,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(1,1)\
        +   nutb*(rxx(1,0)+ryy(1,1)) )*us(m)\
        +  nutb*( (rxi(1,0)**2+rxi(1,1)**2)*uss0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m) )
 fsac2d1(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(0,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(0,1)\
        +   nutb*(rxx(0,0)+ryy(0,1)) )*ur(m)\
        +  nutb*( (rxi(0,0)**2+rxi(0,1)**2)*urr0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m) )
 fsac2d2(m)=0.

 fsac3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(1,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(1,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(1,2)\
        +     nutb*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)) )*us(m)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(2,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(2,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(2,2)\
        +     nutb*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)) )*ut(m) \
        +  nutb*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)\
        +         (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)\
        +         2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)\
        +         2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m) )
 fsac3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(0,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(0,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(0,2)\
        +     nutb*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)) )*ur(m)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(2,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(2,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(2,2)\
        +     nutb*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)) )*ut(m) \
        +  nutb*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)\
        +         (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)\
        +         2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)\
        +         2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m) )
 fsac3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(0,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(0,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(0,2)\
        +     nutb*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)) )*ur(m)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(1,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(1,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(1,2)\
        +     nutb*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)) )*us(m) \
        +  nutb*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)\
        +         (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)\
        +         2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)\
        +         2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m) )

 !    --- 2nd order 2D artificial diffusion ---
 ad2Coeff()=(ad21 + cd22* \
     ( abs(ux2(uc))+abs(uy2(uc))  \
      +abs(ux2(vc))+abs(uy2(vc)) ) )
 ad2cCoeff()=(ad21 + cd22* \
     ( abs(ux2c(uc))+abs(uy2c(uc))  \
      +abs(ux2c(vc))+abs(uy2c(vc)) ) )
 ad2nCoeff() =(ad21 + cd22*( abs(ux2(nc)) +abs(uy2(nc)) ) ) ! for eddy viscosity
 ad2cnCoeff()=(ad21 + cd22*( abs(ux2c(nc))+abs(uy2c(nc)) ) )
 ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
            +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

 !    --- 2nd order 3D artificial diffusion ---
 ad23Coeff()=(ad21 + cd22*   \
     ( abs(ux2(uc))+abs(uy2(uc))+abs(uz2(uc)) \
      +abs(ux2(vc))+abs(uy2(vc))+abs(uz2(vc))  \
      +abs(ux2(wc))+abs(uy2(wc))+abs(uz2(wc)) ) )
 ad23cCoeff()=(ad21 + cd22*   \
     ( abs(ux3c(uc))+abs(uy3c(uc))+abs(uz3c(uc)) \
      +abs(ux3c(vc))+abs(uy3c(vc))+abs(uz3c(vc))  \
      +abs(ux3c(wc))+abs(uy3c(wc))+abs(uz3c(wc)) ) )
 ad23nCoeff() =(ad21 + cd22*( abs(ux2(nc)) +abs(uy2(nc)) +abs(uz2(nc)) ) ) ! for eddy viscosity
 ad23cnCoeff()=(ad21 + cd22*( abs(ux3c(nc))+abs(uy3c(nc))+abs(uz3c(nc)) ) )
 ad23(c)=adc\
     *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
      +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) \
      +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                  
 !     ---fourth-order artficial diffusion in 2D
 ad4Coeff()=(ad41 + cd42*    \
     ( abs(ux2(uc))+abs(uy2(uc))    \
      +abs(ux2(vc))+abs(uy2(vc)) ) )
 ad4(c)=adc\
     *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    \
          -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    \
      +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    \
          +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   \
       -12.*u(i1,i2,i3,c) ) 
 !     ---fourth-order artficial diffusion in 3D
 ad43Coeff()=\
    (ad41 + cd42*    \
     ( abs(ux2(uc))+abs(uy2(uc))+abs(uz2(uc))    \
      +abs(ux2(vc))+abs(uy2(vc))+abs(uz2(vc))    \
      +abs(ux2(wc))+abs(uy2(wc))+abs(uz2(wc)) ) )
 ad43(c)=adc\
     *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
          -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
          -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   \
      +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
          +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   \
          +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  \
       -18.*u(i1,i2,i3,c) )


 #Include "selfAdjointArtificialDiffusion.h"

! statement functions to access coefficients of mixed-boundary conditions
 mixedRHS(c,side,axis,grid)         =bcData(c+numberOfComponents*(0),side,axis,grid)
 mixedCoeff(c,side,axis,grid)       =bcData(c+numberOfComponents*(1),side,axis,grid)
 mixedNormalCoeff(c,side,axis,grid) =bcData(c+numberOfComponents*(2),side,axis,grid)
 !     --- end statement functions

 ierr=0
 ! write(*,*) 'Inside insLineSolve'

 INITIALIZE()



 if ( option.eq.setupSweep ) then
  #If #SOLVER == "INSBL"
   if ( turbulenceModel.eq.baldwinLomax ) then
     computeBLNuT()
   end if
  #Else
    stop 825
  #End 

 else if( option.eq.assignINS )then

  ! **************************************************************************
  ! Fill in the tridiagonal matrix for the momentum equations for the INS plus
  ! artificial dissipation and/or turbulence model
  ! ***************************************************************************

  #If #SOLVER == "INS"
    fillEquations(INS)
  #Elif #SOLVER == "INSSPAL"
    fillEquations(INSSPAL)
  #Elif #SOLVER == "INSBL"
    fillEquations(INSBL)
  #Else
    stop 555
  #End

 else if( option.eq.assignTemperature )then

  ! **************************************************************************
  ! Fill in the tridiagonal matrix for the INS Temperature equation 
  ! ***************************************************************************
  #If #SOLVER == "INS"
   fillEquations(INS_TEMPERATURE)
  #Else
    stop 556 
  #End

 else if( option.eq.assignSpalartAllmaras )then

  ! **************************************************************************
  ! Fill in the tridiagonal matrix for the turbulent eddy viscosity eqution for 
  ! the Spalart Almaras TM
  ! ***************************************************************************

  #If #SOLVER == "INSSPAL"
   if( gridType.eq.rectangular )then
     if( dir.eq.0 )then
       fillSAEquationsRectangularGrid(0)
     else if( dir.eq.1 )then
       fillSAEquationsRectangularGrid(1)
     else ! dir.eq.2
       fillSAEquationsRectangularGrid(2)
     end if ! end dir
   else
     if( dir.eq.0 )then
       fillSAEquationsCurvilinearGrid(0)
     else if( dir.eq.1 )then
       fillSAEquationsCurvilinearGrid(1)
     else ! dir.eq.2
       fillSAEquationsCurvilinearGrid(2)
     end if ! end dir
   end if
  #Else
   stop 777
  #End
 else
   write(*,*) 'Unknown option=',option
   stop 8
 end if ! option

 ! ****** Boundary Conditions ******
 indexRange(0,0)=n1a
 indexRange(1,0)=n1b
 indexRange(0,1)=n2a
 indexRange(1,1)=n2b
 indexRange(0,2)=n3a
 indexRange(1,2)=n3b
 ! assign loop variables to correspond to the boundary
 
 do side=0,1
   is1=0
   is2=0
   is3=0
   if( dir.eq.0 )then
     is1=1-2*side
     n1a=indexRange(side,dir)-is1    ! boundary is 1 pt outside
     n1b=n1a
   else if( dir.eq.1 )then
     is2=1-2*side
     n2a=indexRange(side,dir)-is2
     n2b=n2a
   else
     is3=1-2*side
     n3a=indexRange(side,dir)-is3
     n3b=n3a
   end if
    

  sn=2*side-1 ! sign for normal
  ! write(*,*) '$$$$$ side,bc = ',side,bc(side,ibc)
  if( bc(side,ibc).eq.dirichlet )then
    if( computeMatrixBC.eq.1 )then
      if( fourthOrder.eq.0 )then
        loopsMatrixBC( am(i1,i2,i3)=0.,\
                       bm(i1,i2,i3)=1.,\
                       cm(i1,i2,i3)=0.,,,)
      else
        loopsMatrixBC4($$assignDirichletFourthOrder(),,,,,)
      end if
    end if   

  else if( bc(side,ibc).eq.neumann )then 

    ! apply a neumann BC on this side.
    !             | b[0] c[0] a[0]                |
    !             | a[1] b[1] c[1]                |
    !         A = |      a[2] b[2] c[2]           |
    !             |            .    .    .        |
    !             |                a[.] b[.] c[.] |
    !             |                c[n] a[n] b[n] |
    if( computeMatrixBC.eq.1 )then

      if( computeTemperature.ne.0 )then
        a0 = mixedCoeff(tc,side,dir,grid)
        a1 = mixedNormalCoeff(tc,side,dir,grid)
        write(*,'(" insLineSolve: T BC: (a0,a1)=(",f3.1,",",f3.1,") for side,dir,grid=",3i3)') a0,a1,side,dir,grid
        ! '
      end if
      if( fourthOrder.eq.0 )then
        if( side.eq.0 )then
          loopsMatrixBC(bm(i1,i2,i3)= 1.,\
                        cm(i1,i2,i3)=0.,\
                        am(i1,i2,i3)=-1.,,,)
        else
          loopsMatrixBC(cm(i1,i2,i3)=-1.,\
                        am(i1,i2,i3)=0.,\
                        bm(i1,i2,i3)= 1.,,,)
        end if
      else
        ! use +-D0 and D+D-D0
        if( side.eq.0 )then
          cexa= 0.
          cexb= 0.
          cexc= 1.
          cexd= 0.
          cexe=-1.

          c4exa= 2.
          c4exb=-1.
          c4exc= 1.
          c4exd=-2.
          c4exe= 0.
        else
          cexa=-1.
          cexb= 0.
          cexc= 1.
          cexd= 0.
          cexe= 0.
          c4exa= 0.
          c4exb=-2.
          c4exc= 1.
          c4exd=-1.
          c4exe= 2.
        end if
        loopsMatrixBC4($$assignFourthOrder(),,,,,)
      end if
    end if

  else if( bc(side,ibc).eq.extrapolate )then 

    if( computeMatrixBC.eq.1 )then

      if( fourthOrder.eq.0 )then
        ! **** second order ****
        if( orderOfExtrapolation.eq.2 )then
          if( side.eq.0 )then
            cexa= 1.
            cexb= 1.
            cexc=-2.
          else
            cexa=-2.
            cexb= 1.
            cexc= 1.
          end if
        else if( orderOfExtrapolation.eq.3 )then 
          if( side.eq.0 )then
            cexa= 3.
            cexb= 1.
            cexc=-3.
          else
            cexa=-3.
            cexb= 1.
            cexc= 3.
          end if
        else
          write(*,*) 'ERROR: not implemeted: orderOfExtrapolation=',orderOfExtrapolation
          stop 1111
        end if
        loopsMatrixBC( am(i1,i2,i3)=cexa,\
                       bm(i1,i2,i3)=cexb,\
                       cm(i1,i2,i3)=cexc,,,)

      else 
        ! **** fourth order ****
        if( orderOfExtrapolation.eq.2 )then
          if( side.eq.0 )then
            cexa= 0.
            cexb= 0.
            cexc= 1.
            cexd=-2.
            cexe= 1.
          else
            cexa= 1.
            cexb=-2.
            cexc= 1.
            cexd= 0.
            cexe= 0.
          end if
        else if( orderOfExtrapolation.eq.3 )then 
          if( side.eq.0 )then
            cexa=-1.
            cexb= 0.
            cexc= 1.
            cexd=-3.
            cexe= 3.
          else
            cexa= 3.
            cexb=-3.
            cexc= 1.
            cexd= 0.
            cexe=-1.
          end if
        else
          write(*,*) 'ERROR: not implemeted: orderOfExtrapolation=',orderOfExtrapolation
          stop 1111
        end if
        if( side.eq.0 )then
          c4exa=-4.
          c4exb= 1.
          c4exc= 1.
          c4exd=-4.
          c4exe= 6.
        else
          c4exa=+6.
          c4exb=-4.
          c4exc= 1.
          c4exd= 1.
          c4exe=-4.
        end if
        loopsMatrixBC4($$assignFourthOrder(),,,,,)

      end if
    end if

  end if

  ! reset values
  if( dir.eq.0 )then
    n1a=indexRange(0,dir)
    n1b=indexRange(1,dir)
  else if( dir.eq.1 )then
    n2a=indexRange(0,dir)
    n2b=indexRange(1,dir)
  else
    n3a=indexRange(0,dir)
    n3b=indexRange(1,dir)
  end if
 end do ! do side


 return
 end
#endMacro

#beginMacro INS_LINE_SETUP_NULL(SOLVER,NAME)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
      md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw,\
      dir,am,bm,cm,dm,em,  bc, boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c        ****** NULL version **********
c 
c Used if we don't want to compile the real file for a given case
c======================================================================
 return
 end
#endMacro


#beginMacro buildFile(SOLVER,NAME)
#beginFile src/NAME.f
 INS_LINE_SETUP(SOLVER,NAME)
#endFile
#beginFile src/NAME ## Null.f
 INS_LINE_SETUP_NULL(SOLVER,NAME)
#endFile
#endMacro


      buildFile(INS,lineSolveINS)
      buildFile(INSSPAL,lineSolveINSSPAL)
      buildFile(INSBL,lineSolveINSBL)



      subroutine insLineSetup(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
       md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
       ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c         ************* INS Line Solver Function ***************
c
c This function can:
c  (1) Fill in the matrix coefficents for line solvers
c  (2) Assign the right-hand-side values in f
c  (3) Compute the residual  
c
c NOTES:
c   Fill in the interior equation for points (n1a:n1b,n2a:n2b,n3a:n3b)
c   Fill in the BC equations for points outside this (along the line solver direction)
c   
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & md1a,md1b,md2a,md2b,md3a,md3b,
     & dir

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real am(md1a:md1b,md2a:md2b,md3a:md3b)
      real bm(md1a:md1b,md2a:md2b,md3a:md3b)
      real cm(md1a:md1b,md2a:md2b,md3a:md3b)
      real dm(md1a:md1b,md2a:md2b,md3a:md3b)
      real em(md1a:md1b,md2a:md2b,md3a:md3b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr

      ! bcData(component+numberOfComponents*(0),side,axis,grid)  
      integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
      real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

      integer ipar(0:*)
      real rpar(0:*)
      
c     ---- local variables -----

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

c     --- end statement functions

      ierr=0
      ! write(*,*) 'Inside insLineSolve'

      turbulenceModel   = ipar(23)

      if( turbulenceModel.eq.noTurbulenceModel )then
        call lineSolveINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )

      else if( turbulenceModel.eq.spalartAllmaras )then
        call lineSolveINSSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
      else if( turbulenceModel.eq.baldwinLomax )then
        call lineSolveINSBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
      else
        write(*,*) 'insLineSetup:Unknown turbulenceModel=',turbulenceModel
        stop 444
      end if      

      return
      end





#beginMacro resLoops(e1,e2,e3,e4,e5,e6)
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
      e5
      e6
    else 
    end if
  end do
  end do
  end do
#endMacro


c====================================================================
c Define first derivatives and the coeffciients adc2 and adc4 for the 
c artficial dissipation
c====================================================================
#beginMacro getDerivativesAndDissipation(DIM,ORDER,GRIDTYPE)

 defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

 u0x=UX(uc)
 u0y=UY(uc)
 v0x=UX(vc)
 v0y=UY(vc)

#If #DIM == "2"
 adc = abs(u0x)+abs(u0y)+abs(v0x)+abs(v0y)
#Elif #DIM == "3"
 u0z=UZ(uc)
 v0z=UZ(vc)
 w0x=UX(wc)
 w0y=UY(wc)
 w0z=UZ(wc)
 adc = abs(u0x)+abs(u0y)+abs(u0z)+abs(v0x)+abs(v0y)+abs(v0z)+abs(w0x)+abs(w0y)+abs(w0z)
#Else
  stop 7654
#End

 adc2= ad21 + cd22*adc
 adc4= ad41 + cd42*adc

#endMacro



      subroutine computeResidual(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,rsxy,  u,gv,dt,f,dw, residual,
     & bc, ipar, rpar, ierr )
c======================================================================
c
c  *********** Compute the residual *****************
c
c nd : number of space dimensions
c
c u : input - current solution
c f : input rhs forcing
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real residual(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),ierr
      real dtScale,cfl

      integer ipar(0:*)
      real rpar(0:*)

c     ---- local variables -----
      integer m,n,c,kd,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask
      integer gridIsImplicit,implicitOption,implicitMethod,ibc,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD,useSelfAdjointDiffusion,
     & orderOfExtrapolation,fourthOrder,dirp1,dirp2
      integer pc,uc,vc,wc,tc, fc,fcu,fcv,fcw,fcn,fct,grid,side,gridType
      integer computeMatrix,computeRHS,computeMatrixBC,computeTemperature
      integer twilightZoneFlow
      integer indexRange(0:1,0:2),is1,is2,is3
      real nu,kThermal,thermalExpansivity,gravity(0:2)
      real dx(0:2),dx0,dy,dz,dxi,dyi,dzi,dri,dsi,dti
      real dxv2i(0:2),dx2i,dy2i,dz2i
      real dxvsqi(0:2),dxsqi,dysqi,dzsqi
      real drv2i(0:2),dr2i,ds2i,dt2i
      real drvsqi(0:2),drsqi,dssqi,dtsqi
      real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,
     & dxy4i,dxz4i,dyz4i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc,sn,adc2,adc4
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real dr(0:2)

      integer option
      integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
      parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

      real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
      real dd,dndx(0:2)

      real chi,fnu1,fnu2,s,r,g,fw,dKappaSq,nBydSqLhs,nSqBydSq,nutb
      real nuTilde,nuT,nuTx(0:2),fv1,fv1x,fv1y,fv1z
      real nuTSA,chi3,nuTd
      real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
      integer itrip,jtrip,ktrip !baldwin-lomax trip location

      integer numberOfComponents
      integer nc

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

      real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
      real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
      real udmzC,udzmC,udmzzC,udzmzC,udzzmC
      real admzR,adzmR,admzzR,adzmzR,adzzmR
      real admzC,adzmC,admzzC,adzmzC,adzzmC
      real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
      real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
      real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f
      real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
      real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

      real rxi,rxr,rxs,rxt,rxx,rxy,ryy,rxx3,rxy3,rxz3
      real ur,us,ut,urs,urt,ust,urr,uss,utt
      real uxx0,uyy0,uzz0,ux2c,uy2c,ux3c,uy3c,uz3c
      real lap2d2c,lap3d2c

      real u0,u0x,u0y,u0z
      real v0,v0x,v0y,v0z
      real w0,w0x,w0y,w0z

c     ------------ start statement functions -------------------
      real rx,ry,rz,sx,sy,sz,tx,ty,tz

      real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
      real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,uyz4,uyy4,uzz4

      real  ad2Coeff,ad2rCoeff,ad2,ad23Coeff,ad23rCoeff,ad23,ad4Coeff,ad4rCoeff,ad4,ad43Coeff,ad43rCoeff,ad43

      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'
      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

      !.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

      !     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
      defineDifferenceOrder4Components1(u,RX)

      !*      include 'insDeriv.h'
      !*      include 'insDerivc.h'

      uu(c)    = u(i1,i2,i3,c)

      ux2(c)   = ux22r(i1,i2,i3,c)
      uy2(c)   = uy22r(i1,i2,i3,c)
      uz2(c)   = uz23r(i1,i2,i3,c)
      uxy2(c)  = uxy22r(i1,i2,i3,c)
      uxz2(c)  = uxz23r(i1,i2,i3,c) 
      uyz2(c)  = uyz23r(i1,i2,i3,c) 
      uxx2(c)  = uxx22r(i1,i2,i3,c) 
      uyy2(c)  = uyy22r(i1,i2,i3,c) 
      uzz2(c)  = uzz23r(i1,i2,i3,c) 
      lap2d2(c)= ulaplacian22r(i1,i2,i3,c)
      lap3d2(c)= ulaplacian23r(i1,i2,i3,c)

      ux4(c)   = ux42r(i1,i2,i3,c)
      uy4(c)   = uy42r(i1,i2,i3,c)
      uz4(c)   = uz43r(i1,i2,i3,c)
      uxy4(c)  = uxy42r(i1,i2,i3,c)
      uxz4(c)  = uxz43r(i1,i2,i3,c) 
      uyz4(c)  = uyz43r(i1,i2,i3,c) 
      uxx4(c)  = uxx42r(i1,i2,i3,c) 
      uyy4(c)  = uyy42r(i1,i2,i3,c) 
      uzz4(c)  = uzz43r(i1,i2,i3,c) 
      lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
      lap3d4(c)= ulaplacian43r(i1,i2,i3,c)

      ux2c(m) = ux22(i1,i2,i3,m)
      uy2c(m) = uy22(i1,i2,i3,m)

      ux3c(m) = ux23(i1,i2,i3,m)
      uy3c(m) = uy23(i1,i2,i3,m)
      uz3c(m) = uz23(i1,i2,i3,m)

      lap2d2c(m) = ulaplacian22(i1,i2,i3,m)
      lap3d2c(m) = ulaplacian23(i1,i2,i3,m)  

c    --- 2nd order 2D artificial diffusion ---
      ad2Coeff()=(ad21 + cd22* 
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))  
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad2rCoeff()=(ad21 + cd22* 
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))  
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad2(adc,c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &               +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
c    --- 2nd order 3D artificial diffusion ---
      ad23Coeff()=(ad21 + cd22*   
     &    ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc)) 
     &     +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))  
     &     +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,i3,wc)) ) )
      ad23rCoeff()=(ad21 + cd22*   
     &    ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc)) 
     &     +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,vc))  
     &     +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,wc)) ) )
      ad23(adc,c)=adc
     &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) 
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                       
c     ---fourth-order artificial diffusion in 2D
      ad4Coeff()=(ad41 + cd42*    
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))    
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad4rCoeff()=(ad41 + cd42*    
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))    
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad4(adc,c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   
     &      -12.*u(i1,i2,i3,c) ) 
c     ---fourth-order artificial diffusion in 3D
      ad43Coeff()=
     &   (ad41 + cd42*    
     &    ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))    
     &     +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))    
     &     +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,i3,wc)) ) )
      ad43rCoeff()=
     &   (ad41 + cd42*    
     &    ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))    
     &     +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,vc))    
     &     +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,wc)) ) )
      ad43(adc,c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  
     &      -18.*u(i1,i2,i3,c) )


#Include "selfAdjointArtificialDiffusion.h"

c ------------ end statement functions -------------------


      INITIALIZE()


      if( turbulenceModel.eq.noTurbulenceModel )then
       ! *******************************************
       ! **********Incompressible NS ***************
       ! *******************************************


      if( gridType.eq.rectangular )then

       ! *******************************************
       ! ************** rectangular  ***************
       ! *******************************************
c       write(*,*) 'Inside insLineSolve: rectangular, use2ndOrderAD= ',
c     &    use2ndOrderAD
       if( orderOfAccuracy.eq.2 )then

         ! computeResidualRectangularGrid

         if( nd.eq.2 )then

          if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then
           resLoops(residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                                        -u(i1,i2,i3,vc)*uy2(uc)\
                                                    -ux2(pc)+nu*lap2d2(uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                                        -u(i1,i2,i3,vc)*uy2(vc)\
                                                    -uy2(pc)+nu*lap2d2(vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),,,,)
          else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
           resLoops(residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                                        -u(i1,i2,i3,vc)*uy2(uc)\
                                                    -ux2(pc)+nu*lap2d2(uc)+adSelfAdjoint2dR(i1,i2,i3,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                                        -u(i1,i2,i3,vc)*uy2(vc)\
                                                    -uy2(pc)+nu*lap2d2(vc)+adSelfAdjoint2dR(i1,i2,i3,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),,,,)
          else if( use4thOrderAD.eq.1 )then
           ! apply 2nd and 4th order AD
           resLoops(getDerivativesAndDissipation(2,2,rectangular),\
                    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*u0x\
                                                        -u(i1,i2,i3,vc)*u0y\
                                                    -ux2(pc)+nu*lap2d2(uc)+ad2(adc2,uc)+ad4(adc4,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*v0x\
                                                        -u(i1,i2,i3,vc)*v0y\
                                                    -uy2(pc)+nu*lap2d2(vc)+ad2(adc2,vc)+ad4(adc4,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),,,)
          end if

          if( computeTemperature.ne.0 )then
           resLoops(residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2(tc)\
                                                        -u(i1,i2,i3,vc)*uy2(tc)\
                                                        +kThermal*lap2d2(tc),,,,,)
          end if



         else if( nd.eq.3 )then

          if( use4thOrderAD.eq.0 )then
           resLoops(residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                                      -u(i1,i2,i3,vc)*uy2(uc)\
                                                      -u(i1,i2,i3,wc)*uz2(uc)\
                                                    -ux2(pc)+nu*lap3d2(uc)+adSelfAdjoint3dR(i1,i2,i3,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                                      -u(i1,i2,i3,vc)*uy2(vc)\
                                                      -u(i1,i2,i3,wc)*uz2(vc)\
                                                    -uy2(pc)+nu*lap3d2(vc)+adSelfAdjoint3dR(i1,i2,i3,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),\
                  residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux2(wc)\
                                                      -u(i1,i2,i3,vc)*uy2(wc)\
                                                      -u(i1,i2,i3,wc)*uz2(wc)\
                                                    -uz2(pc)+nu*lap3d2(wc)+adSelfAdjoint3dR(i1,i2,i3,wc)\
                                                    -thermalExpansivity*gravity(2)*u(i1,i2,i3,tc),,,)
          else
           resLoops(getDerivativesAndDissipation(3,2,rectangular),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*u0x\
                                                      -u(i1,i2,i3,vc)*u0y\
                                                      -u(i1,i2,i3,wc)*u0z\
                                                    -ux2(pc)+nu*lap3d2(uc)+ad23(adc2,uc)+ad43(adc4,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*v0x\
                                                      -u(i1,i2,i3,vc)*v0y\
                                                      -u(i1,i2,i3,wc)*v0z\
                                                    -uy2(pc)+nu*lap3d2(vc)+ad23(adc2,vc)+ad43(adc4,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),\
                  residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*w0x\
                                                      -u(i1,i2,i3,vc)*w0y\
                                                      -u(i1,i2,i3,wc)*w0z\
                                                    -uz2(pc)+nu*lap3d2(wc)+ad23(adc2,wc)+ad43(adc4,wc)\
                                                    -thermalExpansivity*gravity(2)*u(i1,i2,i3,tc),,)

          end if

          if( computeTemperature.ne.0 )then
           resLoops(residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2(tc)\
                                                        -u(i1,i2,i3,vc)*uy2(tc)\
                                                        -u(i1,i2,i3,wc)*uz2(tc)\
                                                        +kThermal*lap3d2(tc),,,,,)
          end if

         end if



       else ! order==4
       end if

      else 
       ! *******************************************
       ! ************** curvilinear  ***************
       ! *******************************************
        
      ! *wdh* 070830 if( orderOfAccuracy.eq.2 .and. use4thOrderAD.eq.0 )then
      if( orderOfAccuracy.eq.2 )then

        ! computeResidualCurvilinearGrid
         if( nd.eq.2 )then
          if( use4thOrderAD.eq.0 )then
           resLoops(residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2c(uc)\
                                                        -u(i1,i2,i3,vc)*uy2c(uc)\
                                                    -ux2c(pc)+nu*lap2d2c(uc)+adSelfAdjoint2dC(i1,i2,i3,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(vc)\
                                                        -u(i1,i2,i3,vc)*uy2c(vc)\
                                                    -uy2c(pc)+nu*lap2d2c(vc)+adSelfAdjoint2dC(i1,i2,i3,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),,,,)
          else 
           resLoops(getDerivativesAndDissipation(2,2,curvilinear),\
                    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*u0x\
                                                        -u(i1,i2,i3,vc)*u0y\
                                                    -ux2c(pc)+nu*lap2d2c(uc)+ad2(adc2,uc)+ad4(adc4,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*v0x\
                                                        -u(i1,i2,i3,vc)*v0y\
                                                    -uy2c(pc)+nu*lap2d2c(vc)+ad2(adc2,vc)+ad4(adc4,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),,,)
          end if

          if( computeTemperature.ne.0 )then
           resLoops(residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2c(tc)\
                                                        -u(i1,i2,i3,vc)*uy2c(tc)\
                                                        +kThermal*lap2d2c(tc),,,,,)
          end if

         else if( nd.eq.3 )then

          if( use4thOrderAD.eq.0 )then
           resLoops(residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux3c(uc)\
                                                        -u(i1,i2,i3,vc)*uy3c(uc)\
                                                        -u(i1,i2,i3,wc)*uz3c(uc)\
                                                    -ux3c(pc)+nu*lap3d2c(uc)+adSelfAdjoint3dC(i1,i2,i3,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux3c(vc)\
                                                        -u(i1,i2,i3,vc)*uy3c(vc)\
                                                        -u(i1,i2,i3,wc)*uz3c(vc)\
                                                    -uy3c(pc)+nu*lap3d2c(vc)+adSelfAdjoint3dC(i1,i2,i3,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux3c(wc)\
                                                        -u(i1,i2,i3,vc)*uy3c(wc)\
                                                        -u(i1,i2,i3,wc)*uz3c(wc)\
                                                    -uz3c(pc)+nu*lap3d2c(wc)+adSelfAdjoint3dC(i1,i2,i3,wc)\
                                                    -thermalExpansivity*gravity(2)*u(i1,i2,i3,tc),,,)

          else
           resLoops(getDerivativesAndDissipation(3,2,curvilinear),\
                    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*u0x\
                                                        -u(i1,i2,i3,vc)*u0y\
                                                        -u(i1,i2,i3,wc)*u0z\
                                                    -ux3c(pc)+nu*lap3d2c(uc)+ad23(adc2,uc)+ad43(adc4,uc)\
                                                    -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*v0x\
                                                        -u(i1,i2,i3,vc)*v0y\
                                                        -u(i1,i2,i3,wc)*v0z\
                                                    -uy3c(pc)+nu*lap3d2c(vc)+ad23(adc2,vc)+ad43(adc4,vc)\
                                                    -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc),\
                    residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*w0x\
                                                        -u(i1,i2,i3,vc)*w0y\
                                                        -u(i1,i2,i3,wc)*w0z\
                                                    -uz3c(pc)+nu*lap3d2c(wc)+ad23(adc2,wc)+ad43(adc4,wc)\
                                                    -thermalExpansivity*gravity(2)*u(i1,i2,i3,tc),,)

          end if

          if( computeTemperature.ne.0 )then
           resLoops(residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux3c(tc)\
                                                        -u(i1,i2,i3,vc)*uy3c(tc)\
                                                        -u(i1,i2,i3,wc)*uz3c(tc)\
                                                        +kThermal*lap3d2c(tc),,,,,)
          end if

         end if

                                    

      else ! order==4
      end if ! end order of accuracy

      end if ! end curvilinear


      else if( turbulenceModel.eq.spalartAllmaras )then

       ! *******************************************
       ! **********Spalart Allmaras TM *************
       ! *******************************************

        if( gridType.eq.rectangular )then

          ! computeSAResidualRectangularGrid()

         if( nd.eq.2 )then

         resLoops($setupSA(2,rectangular),\
                  $defineSADerivatives(2,rectangular),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                                      -u(i1,i2,i3,vc)*uy2(uc)\
                         -ux2(pc)+nuT*lap2d2(uc)+nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc))\
                                             +adSelfAdjoint2dR(i1,i2,i3,uc),\
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                                      -u(i1,i2,i3,vc)*uy2(vc)\
                         -uy2(pc)+nuT*lap2d2(vc)+nuTx(0)*(uy2(uc)+ux2(vc)) +nuTx(1)*(2.*uy2(vc))\
                                             +adSelfAdjoint2dR(i1,i2,i3,vc),\
                  residual(i1,i2,i3,nc)=f(i1,i2,i3,nc)-u(i1,i2,i3,uc)*ux2(nc)\
                                                      -u(i1,i2,i3,vc)*uy2(nc)\
                                     +nutb*lap2d2(nc) + cb1*s*u(i1,i2,i3,nc) - nSqBydSq \
                                     + adSelfAdjoint2dRSA(i1,i2,i3,nc)\
                                     +(1.+cb2)*sigmai*(dndx(0)**2+dndx(1)**2),)

         else if( nd.eq.3 )then

         resLoops($setupSA(3,rectangular),\
                  $defineSADerivatives(3,rectangular),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                                      -u(i1,i2,i3,vc)*uy2(uc)\
                                                      -u(i1,i2,i3,wc)*uz2(uc)\
                                                      -ux2(pc)+nuT*lap3d2(uc)+adSelfAdjoint3dR(i1,i2,i3,uc)\
                             +nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc))+nuTx(2)*(uz2(uc)+ux2(wc)), \
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                                      -u(i1,i2,i3,vc)*uy2(vc)\
                                                      -u(i1,i2,i3,wc)*uz2(vc)\
                                                      -uy2(pc)+nuT*lap3d2(vc)+adSelfAdjoint3dR(i1,i2,i3,vc)\
                             +nuTx(0)*(uy2(uc)+ux2(vc))+nuTx(1)*(2.*uy2(vc))+nuTx(2)*(uz2(vc)+uy2(wc)), \
                  residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux2(wc)\
                                                      -u(i1,i2,i3,vc)*uy2(wc)\
                                                      -u(i1,i2,i3,wc)*uz2(wc)\
                                                      -uz2(pc)+nuT*lap3d2(wc)+adSelfAdjoint3dR(i1,i2,i3,wc)\
                             +nuTx(0)*(uz2(uc)+ux2(wc))+nuTx(1)*(uz2(vc)+uy2(wc))+nuTx(2)*(2.*uz2(wc)),\
                  residual(i1,i2,i3,nc)=f(i1,i2,i3,nc)-u(i1,i2,i3,uc)*ux2(nc)\
                                                      -u(i1,i2,i3,vc)*uy2(nc)\
                                                      -u(i1,i2,i3,wc)*uz2(nc)\
                                     +nutb*lap3d2(nc)+ cb1*s*u(i1,i2,i3,nc) - nSqBydSq \
                                     +adSelfAdjoint3dRSA(i1,i2,i3,nc)\
                                     +(1.+cb2)*sigmai*(dndx(0)**2+dndx(1)**2+dndx(2)**2))


         end if


        else

          ! *******************************************
          ! ********** curvilinear ********************
          ! *******************************************

         if( nd.eq.2 )then

         resLoops($setupSA(2,curvilinear),\
                  $defineSADerivatives(2,curvilinear),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2c(uc)\
                                                      -u(i1,i2,i3,vc)*uy2c(uc)\
                         -ux2c(pc)+nuT*lap2d2c(uc)+nuTx(0)*(2.*ux2c(uc))+nuTx(1)*(uy2c(uc)+ux2c(vc))\
                                             +adSelfAdjoint2dC(i1,i2,i3,uc),\
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(vc)\
                                                      -u(i1,i2,i3,vc)*uy2c(vc)\
                         -uy2c(pc)+nuT*lap2d2c(vc)+nuTx(0)*(uy2c(uc)+ux2c(vc)) +nuTx(1)*(2.*uy2c(vc))\
                                             +adSelfAdjoint2dC(i1,i2,i3,vc),\
                  residual(i1,i2,i3,nc)=f(i1,i2,i3,nc)-u(i1,i2,i3,uc)*ux2c(nc)\
                                                      -u(i1,i2,i3,vc)*uy2c(nc)\
                                     +nutb*lap2d2c(nc) + cb1*s*u(i1,i2,i3,nc) - nSqBydSq \
                                     + adSelfAdjoint2dCSA(i1,i2,i3,nc)\
                                     +(1.+cb2)*sigmai*(dndx(0)**2+dndx(1)**2),)

         else if( nd.eq.3 )then

         resLoops($setupSA(3,curvilinear),\
                  $defineSADerivatives(3,curvilinear),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux3c(uc)\
                                                      -u(i1,i2,i3,vc)*uy3c(uc)\
                                                      -u(i1,i2,i3,wc)*uz3c(uc)\
                                                      -ux3c(pc)+nuT*lap3d2c(uc)+adSelfAdjoint3dC(i1,i2,i3,uc)\
                             +nuTx(0)*(2.*ux3c(uc))+nuTx(1)*(uy3c(uc)+ux3c(vc))+nuTx(2)*(uz3c(uc)+ux3c(wc)), \
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux3c(vc)\
                                                      -u(i1,i2,i3,vc)*uy3c(vc)\
                                                      -u(i1,i2,i3,wc)*uz3c(vc)\
                                                      -uy3c(pc)+nuT*lap3d2c(vc)+adSelfAdjoint3dC(i1,i2,i3,vc)\
                             +nuTx(0)*(uy3c(uc)+ux3c(vc))+nuTx(1)*(2.*uy3c(vc))+nuTx(2)*(uz3c(vc)+uy3c(wc)), \
                  residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux3c(wc)\
                                                      -u(i1,i2,i3,vc)*uy3c(wc)\
                                                      -u(i1,i2,i3,wc)*uz3c(wc)\
                                                      -uz3c(pc)+nuT*lap3d2c(wc)+adSelfAdjoint3dC(i1,i2,i3,wc)\
                             +nuTx(0)*(uz3c(uc)+ux3c(wc))+nuTx(1)*(uz3c(vc)+uy3c(wc))+nuTx(2)*(2.*uz3c(wc)),\
                  residual(i1,i2,i3,nc)=f(i1,i2,i3,nc)-u(i1,i2,i3,uc)*ux3c(nc)\
                                                      -u(i1,i2,i3,vc)*uy3c(nc)\
                                                      -u(i1,i2,i3,wc)*uz3c(nc)\
                                     +nutb*lap3d2c(nc)+ cb1*s*u(i1,i2,i3,nc) - nSqBydSq\
                                     +adSelfAdjoint3dCSA(i1,i2,i3,nc)\
                                     +(1.+cb2)*sigmai*(dndx(0)**2+dndx(1)**2+dndx(2)**2))


         end if
        end if

      else if( turbulenceModel.eq.baldwinLomax )then

       ! *******************************************
       ! **********Baldwin Lomax TM *************
       ! *******************************************

        if( gridType.eq.rectangular )then

          ! computeSAResidualRectangularGrid()

         if( nd.eq.2 )then

         resLoops(,\
                  $defineBLDerivatives(2,rectangular),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                                      -u(i1,i2,i3,vc)*uy2(uc)\
                         -ux2(pc)+nuT*lap2d2(uc)+nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc))\
                                             +adSelfAdjoint2dR(i1,i2,i3,uc),\
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                                      -u(i1,i2,i3,vc)*uy2(vc)\
                         -uy2(pc)+nuT*lap2d2(vc)+nuTx(0)*(uy2(uc)+ux2(vc)) +nuTx(1)*(2.*uy2(vc))\
                                             +adSelfAdjoint2dR(i1,i2,i3,vc),,)

         else if( nd.eq.3 )then

         resLoops(,\
                  $defineBLDerivatives(3,rectangular),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                                      -u(i1,i2,i3,vc)*uy2(uc)\
                                                      -u(i1,i2,i3,wc)*uz2(uc)\
                                                      -ux2(pc)+nuT*lap3d2(uc)+adSelfAdjoint3dR(i1,i2,i3,uc)\
                             +nuTx(0)*(2.*ux2(uc))+nuTx(1)*(uy2(uc)+ux2(vc))+nuTx(2)*(uz2(uc)+ux2(wc)), \
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                                      -u(i1,i2,i3,vc)*uy2(vc)\
                                                      -u(i1,i2,i3,wc)*uz2(vc)\
                                                      -uy2(pc)+nuT*lap3d2(vc)+adSelfAdjoint3dR(i1,i2,i3,vc)\
                             +nuTx(0)*(uy2(uc)+ux2(vc))+nuTx(1)*(2.*uy2(vc))+nuTx(2)*(uz2(vc)+uy2(wc)), \
                  residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux2(wc)\
                                                      -u(i1,i2,i3,vc)*uy2(wc)\
                                                      -u(i1,i2,i3,wc)*uz2(wc)\
                                                      -uz2(pc)+nuT*lap3d2(wc)+adSelfAdjoint3dR(i1,i2,i3,wc)\
                             +nuTx(0)*(uz2(uc)+ux2(wc))+nuTx(1)*(uz2(vc)+uy2(wc))+nuTx(2)*(2.*uz2(wc)),)


         end if


        else

          ! *******************************************
          ! ********** curvilinear ********************
          ! *******************************************

         if( nd.eq.2 )then

         resLoops(,\
                  $defineBLDerivatives(2,curvilinear),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2c(uc)\
                                                      -u(i1,i2,i3,vc)*uy2c(uc)\
                         -ux2c(pc)+nuT*lap2d2c(uc)+nuTx(0)*(2.*ux2c(uc))+nuTx(1)*(uy2c(uc)+ux2c(vc))\
                                             +adSelfAdjoint2dC(i1,i2,i3,uc),\
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(vc)\
                                                      -u(i1,i2,i3,vc)*uy2c(vc)\
                         -uy2c(pc)+nuT*lap2d2c(vc)+nuTx(0)*(uy2c(uc)+ux2c(vc)) +nuTx(1)*(2.*uy2c(vc))\
                                             +adSelfAdjoint2dC(i1,i2,i3,vc),,)

         else if( nd.eq.3 )then

         resLoops(,\
                  $defineBLDerivatives(3,curvilinear),\
                  residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux3c(uc)\
                                                      -u(i1,i2,i3,vc)*uy3c(uc)\
                                                      -u(i1,i2,i3,wc)*uz3c(uc)\
                                                      -ux3c(pc)+nuT*lap3d2c(uc)+adSelfAdjoint3dC(i1,i2,i3,uc)\
                             +nuTx(0)*(2.*ux3c(uc))+nuTx(1)*(uy3c(uc)+ux3c(vc))+nuTx(2)*(uz3c(uc)+ux3c(wc)), \
                  residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux3c(vc)\
                                                      -u(i1,i2,i3,vc)*uy3c(vc)\
                                                      -u(i1,i2,i3,wc)*uz3c(vc)\
                                                      -uy3c(pc)+nuT*lap3d2c(vc)+adSelfAdjoint3dC(i1,i2,i3,vc)\
                             +nuTx(0)*(uy3c(uc)+ux3c(vc))+nuTx(1)*(2.*uy3c(vc))+nuTx(2)*(uz3c(vc)+uy3c(wc)), \
                  residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux3c(wc)\
                                                      -u(i1,i2,i3,vc)*uy3c(wc)\
                                                      -u(i1,i2,i3,wc)*uz3c(wc)\
                                                      -uz3c(pc)+nuT*lap3d2c(wc)+adSelfAdjoint3dC(i1,i2,i3,wc)\
                             +nuTx(0)*(uz3c(uc)+ux3c(wc))+nuTx(1)*(uz3c(vc)+uy3c(wc))+nuTx(2)*(2.*uz3c(wc)),)


         end if
        end if

      else
        write(*,*) 'Unknown turbulenceModel=',turbulenceModel
        stop 8
      end if ! option


      return
      end

