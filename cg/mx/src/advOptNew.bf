!
! Optimized advance routines for cgmx
!
! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


! ---------------------------------------------------------------------------
! Macro : beginLoopsMask
! ---------------------------------------------------------------------------
#beginMacro beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
#endMacro

! ---------------------------------------------------------------------------
! Macro : endLoopsMask
! ---------------------------------------------------------------------------
#beginMacro endLoopsMask()
    end if
  end do
  end do
  end do
#endMacro


#beginMacro loopse6(e1,e2,e3,e4,e5,e6)
if( useWhereMask.ne.0 )then
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
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
      e5
      e6
  end do
  end do
  end do
end if
#endMacro


#beginMacro loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
if( useWhereMask.ne.0 )then
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
   e7
   e8
   e9
  end if
 end do
 end do
 end do
else
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  e1
  e2
  e3
  e4
  e5
  e6
  e7
  e8
  e9
 end do
 end do
 end do
end if
#endMacro

#beginMacro loopse18(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12,e13,e14,e15,e16,e17,e18)
if( useWhereMask.ne.0 )then
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
      e7
      e8
      e9
      e10
      e11
      e12
      e13
      e14
      e15
      e16
      e17
      e18
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
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
      e11
      e12
      e13
      e14
      e15
      e16
      e17
      e18
  end do
  end do
  end do
end if
#endMacro


! This macro is used for variable dissipation in 2D
#beginMacro loopse6VarDis(e1,e2,e3,e4,e5,e6)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. .and. mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
!     write(*,'(" i=",3i3," varDis=",e10.2," diss=",3e10.2)') i1,i2,i3,varDis(i1,i2,i3),dis(i1,i2,i3,ex),\
!         dis(i1,i2,i3,ey),dis(i1,i2,i3,ez)
    else
      e4
      e5
      e6
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. )then
      e1
      e2
      e3
    else
      e4
      e5
      e6
    end if
  end do
  end do
  end do
end if
#endMacro

! This macro is used for variable dissipation in 3D
#beginMacro loopse12VarDis(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. .and. mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e7
      e8
      e9
    else
      e4
      e5
      e6
      e10
      e11
      e12
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. )then
      e1
      e2
      e3
      e7
      e8
      e9
    else
      e4
      e5
      e6
      e10
      e11
      e12
    end if
  end do
  end do
  end do
end if
#endMacro

! This macro is used for variable dissipation in 3D
#beginMacro loopsVarDis3D(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)

 if( solveForE.ne.0 .and. solveForH.ne.0 )then
   loopse12VarDis(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)
 else if( solveForE.ne.0 ) then
   loopse6VarDis(e1,e2,e3,e4,e5,e6)
 else
   loopse6VarDis(h1,h2,h3,h4,h5,h6)
 end if

#endMacro



! Optionally add the forcing terms
#beginMacro loopsF2D(f1,f2,f3,e1,e2,e3,e4,e5,e6,e7,e8,e9)
if( addForcing.eq.0 )then
  loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
else
! add forcing to the first 3 equations
  loopse9(e1+f1,e2+f2,e3+f3,e4,e5,e6,e7,e8,e9)
end if
#endMacro

! Optionally add the forcing terms
! Optionally solve for E or H or both
#beginMacro loopsF3D(fe1,fe2,fe3,e1,e2,e3,e4,e5,e6,e7,e8,e9,fh1,fh2,fh3,h1,h2,h3,h4,h5,h6,h7,h8,h9)
if( addForcing.eq.0 )then

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1,e2,e3,e4,e5,e6,e7,e8,e9,h1,h2,h3,h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
  else
    loopse9(h1,h2,h3,h4,h5,h6,h7,h8,h9)
  end if

else
! add forcing to the equations

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9,h1+fh1,h2+fh2,h3+fh3,h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9)
  else
    loopse9(h1+fh1,h2+fh2,h3+fh3,h4,h5,h6,h7,h8,h9)
  end if

end if
#endMacro


! Optionally add the dissipation and or forcing terms
#beginMacro loopsF2DD(f1,f2,f3,e1,e2,e3,e4,e5,e6,e7,e8,e9)
if( addForcing.eq.0 .and. .not.addDissipation )then
  loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
else if( addForcing.ne.0 .and. .not.addDissipation )then
! add forcing to the first 3 equations
  loopse9(e1+f1,e2+f2,e3+f3,e4,e5,e6,e7,e8,e9)
else if( addForcing.eq.0 .and. addDissipation )then
! add dissipation to the first 3 equations
  loopse9(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e3+dis(i1,i2,i3,hz),e4,e5,e6,e7,e8,e9)
else
!  add forcing and dissipation
  loopse9(e1+f1+dis(i1,i2,i3,ex),e2+f2+dis(i1,i2,i3,ey),e3+f3+dis(i1,i2,i3,hz),e4,e5,e6,e7,e8,e9)  
end if
#endMacro


! Optionally add the dissipation and or forcing terms
! Optionally solve for E or H or both
#beginMacro loopsF3DD(fe1,fe2,fe3,e1,e2,e3,e4,e5,e6,e7,e8,e9,fh1,fh2,fh3,h1,h2,h3,h4,h5,h6,h7,h8,h9)
if( addForcing.eq.0 .and. .not.addDissipation )then

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    ! stop 6654
    loopse18(e1,e2,e3,e4,e5,e6,e7,e8,e9,h1,h2,h3,h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
  else
    stop 9987
!***    loopse9(h1,h2,h3,h4,h5,h6,h7,h8,h9)
  end if

else if( addForcing.ne.0 .and. .not.addDissipation )then
! add forcing to the equations

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
   ! stop 6654
    loopse18(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9,h1+fh1,h2+fh2,h3+fh3,h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9)
  else
    stop 9987
!***    loopse9(h1+fh1,h2+fh2,h3+fh3,h4,h5,h6,h7,h8,h9)
  end if

else if( addForcing.eq.0 .and. addDissipation )then
! add dissipation to the equations

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    ! stop 6654
    loopse18(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e3+dis(i1,i2,i3,ez),e4,e5,e6,e7,e8,e9,h1+dis(i1,i2,i3,hx),h2+dis(i1,i2,i3,hy),h3+dis(i1,i2,i3,hz),h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e3+dis(i1,i2,i3,ez),e4,e5,e6,e7,e8,e9)
  else
    stop 6654
!***    loopse9(h1+dis(i1,i2,i3,hx),h2+dis(i1,i2,i3,hy),h3+dis(i1,i2,i3,hz),h4,h5,h6,h7,h8,h9)
  end if

else
! add dissipation and forcing to the equations

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    ! stop 6654
    loopse18(e1+fe1+dis(i1,i2,i3,ex),e2+fe2+dis(i1,i2,i3,ey),e3+fe3+dis(i1,i2,i3,ez),e4,e5,e6,e7,e8,e9,h1+fh1+dis(i1,i2,i3,hx),h2+fh2+dis(i1,i2,i3,hy),h3+fh3+dis(i1,i2,i3,hz),h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1+fe1+dis(i1,i2,i3,ex),e2+fe2+dis(i1,i2,i3,ey),e3+fe3+dis(i1,i2,i3,ez),e4,e5,e6,e7,e8,e9)
  else
    stop 6654
!****    loopse9(h1+fh1+dis(i1,i2,i3,hx),h2+fh2+dis(i1,i2,i3,hy),h3+fh3+dis(i1,i2,i3,hz),h4,h5,h6,h7,h8,h9)
  end if

end if
#endMacro

! The next macro is used for curvilinear girds where the Laplacian term is precomputed.
#beginMacro loopsFC(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)

if( nd.eq.2 )then
  ! This next line assumes we solve for ex,ey and hz
  loopse9(e1,e2,e4,e5,h3,h6,,,)

else

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6,,,,,,)
  else if( solveForE.ne.0 ) then
    loopse9(e1,e2,e3,e4,e5,e6,,,)
  else
    loopse9(h1,h2,h3,h4,h5,h6,,,)
  end if

end if
#endMacro

! -------------------------------------------------------------------------------------------
! The next macro is used for curvilinear girds where the Laplacian term is precomputed.
! Optionally add dissipation too
! -------------------------------------------------------------------------------------------
#beginMacro loopsFCD(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)

if( .not.addDissipation )then
 if( nd.eq.2 )then
  ! This next line assumes we solve for ex,ey and hz
  loopse9(e1,e2,e4,e5,h3,h6,,,)
 else
  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6,,,,,,)
  else if( solveForE.ne.0 ) then
    loopse9(e1,e2,e3,e4,e5,e6,,,)
  else
    loopse9(h1,h2,h3,h4,h5,h6,,,)
  end if
 end if
else ! add dissipation too
 if( nd.eq.2 )then
  ! This next line assumes we solve for ex,ey and hz
  loopse9(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e4,e5,h3+dis(i1,i2,i3,hz),h6,,,)
 else
  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e3+dis(i1,i2,i3,ez),e4,e5,e6,h1+dis(i1,i2,i3,hx),h2+dis(i1,i2,i3,hy),h3+dis(i1,i2,i3,hz),h4,h5,h6,,,,,,)
  else if( solveForE.ne.0 ) then
    loopse9(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e3+dis(i1,i2,i3,ez),e4,e5,e6,,,)
  else
    loopse9(h1+dis(i1,i2,i3,hx),h2+dis(i1,i2,i3,hy),h3+dis(i1,i2,i3,hz),h4,h5,h6,,,)
  end if
 end if
end if
#endMacro


! -------------------------------------------------------------------------------------------
! The next macro is used for curvilinear girds where the Laplacian term is precomputed.
! Optionally add dissipation too
! -------------------------------------------------------------------------------------------
#beginMacro loopsFCD2DF(e1,e2,h3)
if( .not.addDissipation )then
  ! This next line assumes we solve for ex,ey and hz
  loopse9(e1,e2,h3,,,,,,)
else ! add dissipation too
  ! This next line assumes we solve for ex,ey and hz
  loopse9(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),h3+dis(i1,i2,i3,hz),,,,,,)
end if
#endMacro

! -------------------------------------------------------------------------------------------
! The next macro is used for curvilinear girds where the Laplacian term is precomputed.
! Optionally add dissipation too
! -------------------------------------------------------------------------------------------
#beginMacro loopsFCD3DF(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)

if( .not.addDissipation )then
  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6,,,,,,)
  else if( solveForE.ne.0 ) then
    loopse9(e1,e2,e3,e4,e5,e6,,,)
  else
    loopse9(h1,h2,h3,h4,h5,h6,,,)
  end if
else ! add dissipation too
  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e3+dis(i1,i2,i3,ez),e4,e5,e6,h1+dis(i1,i2,i3,hx),h2+dis(i1,i2,i3,hy),h3+dis(i1,i2,i3,hz),h4,h5,h6,,,,,,)
  else if( solveForE.ne.0 ) then
    loopse9(e1+dis(i1,i2,i3,ex),e2+dis(i1,i2,i3,ey),e3+dis(i1,i2,i3,ez),e4,e5,e6,,,)
  else
    loopse9(h1+dis(i1,i2,i3,hx),h2+dis(i1,i2,i3,hy),h3+dis(i1,i2,i3,hz),h4,h5,h6,,,)
  end if
end if
#endMacro



#defineMacro LAP2D2(U,i1,i2,i3,c) \
                       (U(i1+1,i2,i3,c)-2.*U(i1,i2,i3,c)+U(i1-1,i2,i3,c))*dxsqi\
                      +(U(i1,i2+1,i3,c)-2.*U(i1,i2,i3,c)+U(i1,i2-1,i3,c))*dysqi
#defineMacro LAP3D2(U,i1,i2,i3,c) \
                       (U(i1+1,i2,i3,c)-2.*U(i1,i2,i3,c)+U(i1-1,i2,i3,c))*dxsqi\
                      +(U(i1,i2+1,i3,c)-2.*U(i1,i2,i3,c)+U(i1,i2-1,i3,c))*dysqi\
                      +(U(i1,i2,i3+1,c)-2.*U(i1,i2,i3,c)+U(i1,i2,i3-1,c))*dzsqi

#defineMacro LAP2D2POW2(U,i1,i2,i3,c) ( 6.*U(i1,i2,i3,c)   \
                      - 4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))    \
                      +(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxi4 \
                      +( 6.*U(i1,i2,i3,c)    \
                      -4.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))    \
                      +(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dyi4  \
                      +( 8.*U(i1,i2,i3,c)     \
                      -4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)+U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))   \
                      +2.*(U(i1+1,i2+1,i3,c)+U(i1-1,i2+1,i3,c)+U(i1+1,i2-1,i3,c)+U(i1-1,i2-1,i3,c)) )*dxdyi2

#defineMacro LAP3D2POW2(U,i1,i2,i3,c) ( 6.*U(i1,i2,i3,c)   \
        - 4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))    \
            +(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxi4 \
       +(  +6.*U(i1,i2,i3,c)    \
         -4.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))    \
            +(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dyi4\
       +(  +6.*U(i1,i2,i3,c)    \
         -4.*(U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))    \
            +(U(i1,i2,i3+2,c)+U(i1,i2,i3-2,c)) )*dzi4\
        +(8.*U(i1,i2,i3,c)     \
         -4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)+U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))   \
         +2.*(U(i1+1,i2+1,i3,c)+U(i1-1,i2+1,i3,c)+U(i1+1,i2-1,i3,c)+U(i1-1,i2-1,i3,c)) )*dxdyi2 \
        +(8.*U(i1,i2,i3,c)     \
         -4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)+U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))   \
         +2.*(U(i1+1,i2,i3+1,c)+U(i1-1,i2,i3+1,c)+U(i1+1,i2,i3-1,c)+U(i1-1,i2,i3-1,c)) )*dxdzi2 \
        +(8.*U(i1,i2,i3,c)     \
         -4.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c)+U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))   \
         +2.*(U(i1,i2+1,i3+1,c)+U(i1,i2-1,i3+1,c)+U(i1,i2+1,i3-1,c)+U(i1,i2-1,i3-1,c)) )*dydzi2 

#defineMacro LAP2D4(U,i1,i2,i3,c) ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))     \
            -(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxsq12i + \
       ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))     \
            -(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dysq12i

#defineMacro LAP3D4(U,i1,i2,i3,c) ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))     \
            -(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxsq12i + \
       ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))     \
            -(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dysq12i+ \
       ( -30.*U(i1,i2,i3,c)      \
        +16.*(U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))      \
            -(U(i1,i2,i3+2,c)+U(i1,i2,i3-2,c)) )*dzsq12i

#defineMacro LAP2D6(U,i1,i2,i3,c) \
               c00lap2d6*U(i1,i2,i3,c)     \
              +c10lap2d6*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)) \
              +c01lap2d6*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c)) \
              +c20lap2d6*(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) \
              +c02lap2d6*(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) \
              +c30lap2d6*(U(i1+3,i2,i3,c)+U(i1-3,i2,i3,c)) \
              +c03lap2d6*(U(i1,i2+3,i3,c)+U(i1,i2-3,i3,c))

#defineMacro LAP3D6(U,i1,i2,i3,c) \
               c000lap3d6*U(i1,i2,i3,c) \
              +c100lap3d6*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)) \
              +c010lap3d6*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c)) \
              +c001lap3d6*(U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c)) \
              +c200lap3d6*(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) \
              +c020lap3d6*(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) \
              +c002lap3d6*(U(i1,i2,i3+2,c)+U(i1,i2,i3-2,c)) \
              +c300lap3d6*(U(i1+3,i2,i3,c)+U(i1-3,i2,i3,c)) \
              +c030lap3d6*(U(i1,i2+3,i3,c)+U(i1,i2-3,i3,c)) \
              +c003lap3d6*(U(i1,i2,i3+3,c)+U(i1,i2,i3-3,c))


! ** evaluate the laplacian on the 9 points centred at (i1,i2,i3)
#beginMacro getLapValues2dOrder2(n)
 uLap(-1,-1,n) = uLaplacian22(i1-1,i2-1,i3,n)
 uLap( 0,-1,n) = uLaplacian22(i1  ,i2-1,i3,n)
 uLap(+1,-1,n) = uLaplacian22(i1+1,i2-1,i3,n)

 uLap(-1, 0,n) = uLaplacian22(i1-1,i2  ,i3,n)
 uLap( 0, 0,n) = uLaplacian22(i1  ,i2  ,i3,n)
 uLap(+1, 0,n) = uLaplacian22(i1+1,i2  ,i3,n)

 uLap(-1,+1,n) = uLaplacian22(i1-1,i2+1,i3,n)
 uLap( 0,+1,n) = uLaplacian22(i1  ,i2+1,i3,n)
 uLap(+1,+1,n) = uLaplacian22(i1+1,i2+1,i3,n)
#endMacro


! ** evaluate the square of the Laplacian for a component ****
#beginMacro evalLapSq2dOrder2(n)
 getLapValues2dOrder2(n)
 uLaprr2 = (uLap(+1, 0,n)-2.*uLap( 0, 0,n)+uLap(-1, 0,n))/(dr(0)**2)
 uLapss2 = (uLap( 0,+1,n)-2.*uLap( 0, 0,n)+uLap( 0,-1,n))/(dr(1)**2)
 uLaprs2 = (uLap(+1,+1,n)-uLap(-1,+1,n)-uLap(+1,-1,n)+uLap(-1,-1,n))/(4.*dr(0)*dr(1))
 uLapr2  = (uLap(+1, 0,n)-uLap(-1, 0,n))/(2.*dr(0))
 uLaps2  = (uLap( 0,+1,n)-uLap( 0,-1,n))/(2.*dr(1))

 uLapSq(n) =(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*uLaprr2\
        +2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*uLaprs2\
        +(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*uLapss2\
        +(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*uLapr2\
        +(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*uLaps2
 ! write(*,'(" n : uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2=",5f6.2)') uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2
#endMacro

! ** evaluate the square of the Laplacian for [ex,ey,hz] ****
#beginMacro getLapSq2dOrder2()
 evalLapSq2dOrder2(ex)
 evalLapSq2dOrder2(ey)
 evalLapSq2dOrder2(hz)
 ! write(*,'("addForcing,adc=",i2,f5.2,", uLapSq(n)=",3e9.2)') addForcing,adc,uLapSq(ex),uLapSq(ey),uLapSq(hz)
#endMacro

! ==== loops for curvilinear, with forcing, dissipation in 2D
! Optionally add the dissipation and or forcing terms
#beginMacro loopsFCD2D(expr0,f1,f2,f3,expr1,expr2,expr3)
if( addForcing.eq.0 .and. .not.addDissipation )then
 loopse9(expr0,expr1,expr2,expr3,,,,,)
else if( addForcing.ne.0 .and. .not.addDissipation )then
! add forcing to the first 3 equations
 loopse9(expr0,expr1+f1,expr2+f2,expr3+f3,,,,,)
else if( addForcing.eq.0 .and. addDissipation )then
! add dissipation to the first 3 equations
 loopse9(expr0,expr1+dis(i1,i2,i3,ex),expr2+dis(i1,i2,i3,ey),expr3+dis(i1,i2,i3,hz),,,,,)
else
!  add forcing and dissipation
 loopse9(expr0,expr1+f1+dis(i1,i2,i3,ex),expr2+f2+dis(i1,i2,i3,ey)+dis(i1,i2,i3,hz),expr3+f3,,,,,)  
end if
#endMacro

! ==== loops for curvilinear, with forcing, dissipation in 2D
! Optionally add the dissipation and or forcing terms
#beginMacro loopsFCD2DA(expr0,f1,expr1)
if( addForcing.eq.0 .and. .not.addDissipation )then
 loopse9(expr0,expr1,,,,,,,)
else if( addForcing.ne.0 .and. .not.addDissipation )then
! add forcing to the first 3 equations
 loopse9(expr0,expr1+f1,,,,,,,)
else if( addForcing.eq.0 .and. addDissipation )then
! add dissipation to the first 3 equations
 loopse9(expr0,expr1+dis(i1,i2,i3,ex),,,,,,,)
else
!  add forcing and dissipation
 loopse9(expr0,expr1+f1+dis(i1,i2,i3,ex),,,,,,,)  
end if
#endMacro

! ===========================================================================================
! Macro: compute the coefficients in the sosup dissipation for curvilinear grids
! ===========================================================================================
#beginMacro getSosupDissipationCoeff2d(adxSosup)
 do dir=0,1
   ! diss-coeff ~= 1/(change in x along direction r(dir) )
   ! Assuming a nearly orthogonal grid gives ||dx|| = || grad_x(r_i) || / dr_i 
   adxSosup(dir) = adSosup*sqrt( rsxy(i1,i2,i3,dir,0)**2 + rsxy(i1,i2,i3,dir,1)**2 )/dr(dir) 
 end do
#endMacro


! **********************************************************************************
! Macro ADV_MAXWELL:
!  NAME: name of the subroutine
!  DIM : 2 or 3
!  ORDER : 2 ,4, 6 or 8
!  GRIDTYPE : rectangular, curvilinear
! **********************************************************************************
#beginMacro ADV_MAXWELL(NAME,DIM,ORDER,GRIDTYPE)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                 mask,rsxy,  um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
!======================================================================
!   Advance a time step for Maxwells equations
!     OPTIMIZED version for rectangular grids.
! nd : number of space dimensions
!
! ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
!                           =1 - AD only
!
!  dis(i1,i2,i3) : temp space to hold artificial dissipation
!  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
!======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real fa(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b,0:*)  ! forcings at different times
 real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real vvt2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real vvt4(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut5(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut6(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut7(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

 integer ipar(0:*)
 real rpar(0:*)
      
!     ---- local variables -----
 integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime,axis,dir
 integer addForcing,orderOfDissipation,option
 integer useWhereMask,useWhereMaskSave,solveForE,solveForH,grid,useVariableDissipation
 integer useCurvilinearOpt,useConservative,combineDissipationWithAdvance,useDivergenceCleaning
 integer useNewForcingMethod,numberOfForcingFunctions,fcur,fnext,fprev
 integer ex,ey,ez, hx,hy,hz
 real t,cc,dt,dy,dz,cdt,cdtdx,cdtdy,cdtdz,adc,adcdt,add,adddt
 real dt4by12
 real eps,mu,sigmaE,sigmaH,kx,ky,kz,divergenceCleaningCoefficient
 logical addDissipation

 real dx(0:2),dr(0:2)

 real dx2i,dy2i,dz2i,dxsqi,dysqi,dzsqi,dxi,dyi,dzi
 real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,dyz4,time0,time1

 real dxi4,dyi4,dzi4,dxdyi2,dxdzi2,dydzi2

 real uLap(-1:1,-1:1,0:5),uLapSq(0:5)
 real uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2

 real c0,c1,csq,dtsq,cdtsq,cdtsq12,lap(0:20),cdtSqBy12
 real c40,c41,c42,c43
 real c60,c61,c62,c63,c64,c65
 real c80,c81,c82,c83,c84,c85,c86,c87

 real c00lap2d6,c10lap2d6,c01lap2d6,c20lap2d6,c02lap2d6,c30lap2d6,c03lap2d6
 real c00lap2d8,c10lap2d8,c01lap2d8,c20lap2d8,c02lap2d8,c30lap2d8,c03lap2d8,c40lap2d8,c04lap2d8
 real c000lap3d6,c100lap3d6,c010lap3d6,c001lap3d6,\
                 c200lap3d6,c020lap3d6,c002lap3d6,\
                 c300lap3d6,c030lap3d6,c003lap3d6
 real c000lap3d8,c100lap3d8,c010lap3d8,c001lap3d8,\
                 c200lap3d8,c020lap3d8,c002lap3d8,\
                 c300lap3d8,c030lap3d8,c003lap3d8,\
                 c400lap3d8,c040lap3d8,c004lap3d8

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer timeSteppingMethod
 integer defaultTimeStepping,adamsSymmetricOrder3,rungeKuttaFourthOrder,\
         stoermerTimeStepping,modifiedEquationTimeStepping
 parameter(defaultTimeStepping=0,adamsSymmetricOrder3=1,\
           rungeKuttaFourthOrder=2,stoermerTimeStepping=3,modifiedEquationTimeStepping=4)
 ! Dispersion models
 integer noDispersion,drude
 parameter( noDispersion=0, drude=1 )

!...........start statement function
 integer kd,m
 real rx,ry,rz,sx,sy,sz,tx,ty,tz

 declareDifferenceOrder2(u,RX)
 declareDifferenceOrder2(un,none)
 declareDifferenceOrder2(v,none)
 declareDifferenceOrder2(um,none)
 declareDifferenceOrder2(ff,none)

 declareDifferenceOrder4(u,RX)
 declareDifferenceOrder4(un,none)
 declareDifferenceOrder4(v,none)

 real maxwell2dr,maxwell3dr,maxwellr44,maxwellr66,maxwellr88
 real maxwellc22,maxwellc44,maxwellc66,maxwellc88
 real maxwell2dr44me,maxwell2dr66me,maxwell2dr88me
 real maxwell3dr44me,maxwell3dr66me,maxwell3dr88me
 real maxwellc44me,maxwellc66me,maxwellc88me
 real max2dc44me,max2dc44me2,max3dc44me
 real mxdc2d2Ex,mxdc2d2Ey,mxdc2d4Ex,mxdc2d4Ey, mxdc2d4cEx,mxdc2d4cEy
 real mxdc2d2cEx,mxdc2d2cEy
 real mxdc3d2Ex,mxdc3d2Ey,mxdc3d2Ez,mxdc3d2Hx,mxdc3d2Hy,mxdc3d2Hz
 real mxdc3d2cEx,mxdc3d2cEy,mxdc3d2cEz,mxdc3d2cHx,mxdc3d2cHy,mxdc3d2cHz
 real mxdc2d4cConsEx,mxdc2d4cConsEy,mxdc2d4cConsEz
 real mxdc3d4Ex,mxdc3d4Ey,mxdc3d4Ez,mxdc3d4Hx,mxdc3d4Hy,mxdc3d4Hz

 real maxwell2drSosup, maxwell2dr44meSosup
 real dum,sosupDiss2d4r,sosupDiss2d4c,sosupDiss2d6r,sosupDiss2d6c

! real vr2,vs2,vrr2,vss2,vrs2,vLaplacian22

 real cdt4by360,cdt6by20160

 real lap2d2,lap3d2,lap2d4,lap3d4,lap2d6,lap3d6,lap2d8,lap3d8,lap2d2Pow2,lap3d2Pow2,lap2d2Pow3,lap3d2Pow3,\
      lap2d2Pow4,lap3d2Pow4,lap2d4Pow2,lap3d4Pow2,lap2d4Pow3,lap3d4Pow3,lap2d6Pow2,lap3d6Pow2
 real lap2d2m,lap3d2m
 real du,fd22d,fd23d,fd42d,fd43d,fd62d,fd63d,fd82d,fd83d

 ! forcing correction functions: 
 real lap2d2f,f2drme44, lap3d2f, f3drme44, f2dcme44, f3dcme44, ff

 real cdSosupx,cdSosupy,cdSosupz, adSosup,sosupParameter, adSosupCurv4, adSosupCurv6, adxSosup(0:2)
 integer useSosupDissipation,sosupDissipationOption
 integer updateSolution,updateDissipation 

 ! div cleaning: 
 real dc,dcp,cdc0,cdc1,cdcxx,cdcyy,cdczz,cdcEdx,cdcEdy,cdcEdz,cdcHdx,cdcHdy,cdcHdz,cdcf
 real cdcE,cdcELap,cdcELapsq,cdcELapm,cdcHzxLap,cdcHzyLap
 real cdcH,cdcHLap,cdcHLapsq,cdcHLapm

 ! dispersion
 integer dispersionModel,pxc,pyc,pzc,qxc,qyc,qzc,rxc,ryc,rzc
 integer ec,pc
 real gamma,omegap
 real gammaDt,omegapDtSq,ptt, fe,fp

! real unxx22r,unyy22r,unxy22r,unx22r

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

!     The next macro will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)

 defineDifferenceOrder2Components1(un,none)
 defineDifferenceOrder4Components1(un,none)

 defineDifferenceOrder2Components1(v,none)
 defineDifferenceOrder4Components1(v,none)

 defineDifferenceOrder2Components1(um,none)

 defineDifferenceOrder2Components1(ff,none)


 ! 2nd-order in space and time
 maxwell2dr(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+\
            cdtdx*(u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n)-2.*u(i1,i2,i3,n))+\
            cdtdy*(u(i1,i2-1,i3,n)+u(i1,i2+1,i3,n)-2.*u(i1,i2,i3,n))


 du(i1,i2,i3,c)=u(i1,i2,i3,c)-um(i1,i2,i3,c)

 ! 4th-order SOSUP dissipation -- 2D, curvilinear grid 
 adSosupCurv4(i1,i2,i3,n)=\
              ( -6.*du(i1,i2,i3,n)     \
                +4.*(du(i1+1,i2,i3,n)+du(i1-1,i2,i3,n))     \
                   -(du(i1+2,i2,i3,n)+du(i1-2,i2,i3,n)) )*adxSosup(0) + \
              ( -6.*du(i1,i2,i3,n)     \
                +4.*(du(i1,i2+1,i3,n)+du(i1,i2-1,i3,n))     \
                   -(du(i1,i2+2,i3,n)+du(i1,i2-2,i3,n)) )*adxSosup(1)

 ! 6th-order SOSUP dissipation -- 2D, curvilinear grid 
 adSosupCurv6(i1,i2,i3,n)=\
             (-20.*du(i1,i2,i3,n)     \
               +15.*(du(i1+1,i2,i3,n)+du(i1-1,i2,i3,n))     \
                -6.*(du(i1+2,i2,i3,n)+du(i1-2,i2,i3,n))     \
                   +(du(i1+3,i2,i3,n)+du(i1-3,i2,i3,n))  )*adxSosup(0) + \
              (-20.*du(i1,i2,i3,n)     \
               +15.*(du(i1,i2+1,i3,n)+du(i1,i2-1,i3,n))     \
                -6.*(du(i1,i2+2,i3,n)+du(i1,i2-2,i3,n))     \
                   +(du(i1,i2+3,i3,n)+du(i1,i2-3,i3,n))  )*adxSosup(1)

 ! 2nd-order in space and time with sosup dissipation
 maxwell2drSosup(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+\
            cdtdx*(u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n)-2.*u(i1,i2,i3,n))+\
            cdtdy*(u(i1,i2-1,i3,n)+u(i1,i2+1,i3,n)-2.*u(i1,i2,i3,n))+\
              ( -6.*du(i1,i2,i3,n)     \
                +4.*(du(i1+1,i2,i3,n)+du(i1-1,i2,i3,n))     \
                   -(du(i1+2,i2,i3,n)+du(i1-2,i2,i3,n)) )*cdSosupx + \
              ( -6.*du(i1,i2,i3,n)     \
                +4.*(du(i1,i2+1,i3,n)+du(i1,i2-1,i3,n))     \
                   -(du(i1,i2+2,i3,n)+du(i1,i2-2,i3,n)) )*cdSosupy


 ! add sosup dissipation (4th-order, rectangular grid)) to current time (using previous two levels)
 ! assume un holds u(t-2*dt) on input:  (note factor of .5 moved from dum to cdSosupx)
 ! Here is D0t:
 dum(i1,i2,i3,n)=u(i1,i2,i3,n)-un(i1,i2,i3,n)
 ! Try D-t : 
 ! dum(i1,i2,i3,n)=(u(i1,i2,i3,n)-um(i1,i2,i3,n))*2.
 sosupDiss2d4r(i1,i2,i3,n)=u(i1,i2,i3,n)+\
              ( -6.* dum(i1,i2,i3,n)     \
                +4.*(dum(i1+1,i2,i3,n)+dum(i1-1,i2,i3,n))     \
                   -(dum(i1+2,i2,i3,n)+dum(i1-2,i2,i3,n)) )*cdSosupx*.5 + \
              ( -6.*dum(i1,i2,i3,n)     \
                +4.*(dum(i1,i2+1,i3,n)+dum(i1,i2-1,i3,n))     \
                   -(dum(i1,i2+2,i3,n)+dum(i1,i2-2,i3,n)) )*cdSosupy*.5

 ! add sosup dissipation (4th-order, curvilinear grid) to current time (using previous two levels)
 sosupDiss2d4c(i1,i2,i3,n)=u(i1,i2,i3,n)+\
              ( -6.* dum(i1,i2,i3,n)     \
                +4.*(dum(i1+1,i2,i3,n)+dum(i1-1,i2,i3,n))     \
                   -(dum(i1+2,i2,i3,n)+dum(i1-2,i2,i3,n)) )*adxSosup(0)*.5 + \
              ( -6.*dum(i1,i2,i3,n)     \
                +4.*(dum(i1,i2+1,i3,n)+dum(i1,i2-1,i3,n))     \
                   -(dum(i1,i2+2,i3,n)+dum(i1,i2-2,i3,n)) )*adxSosup(1)*.5

 ! add sosup dissipation (6th-order, rectangular grid) to current time (using previous two levels)
 sosupDiss2d6r(i1,i2,i3,n)=u(i1,i2,i3,n)+\
             (-20.*dum(i1,i2,i3,n)     \
               +15.*(dum(i1+1,i2,i3,n)+dum(i1-1,i2,i3,n))     \
                -6.*(dum(i1+2,i2,i3,n)+dum(i1-2,i2,i3,n))     \
                   +(dum(i1+3,i2,i3,n)+dum(i1-3,i2,i3,n))  )*cdSosupx*.5 + \
              (-20.*dum(i1,i2,i3,n)     \
               +15.*(dum(i1,i2+1,i3,n)+dum(i1,i2-1,i3,n))     \
                -6.*(dum(i1,i2+2,i3,n)+dum(i1,i2-2,i3,n))     \
                   +(dum(i1,i2+3,i3,n)+dum(i1,i2-3,i3,n))  )*cdSosupy*.5

 ! add sosup dissipation (6th-order, curvilinear grid) to current time (using previous two levels)
 sosupDiss2d6c(i1,i2,i3,n)=u(i1,i2,i3,n)+\
             (-20.*dum(i1,i2,i3,n)     \
               +15.*(dum(i1+1,i2,i3,n)+dum(i1-1,i2,i3,n))     \
                -6.*(dum(i1+2,i2,i3,n)+dum(i1-2,i2,i3,n))     \
                   +(dum(i1+3,i2,i3,n)+dum(i1-3,i2,i3,n))  )*adxSosup(0)*.5 + \
              (-20.*dum(i1,i2,i3,n)     \
               +15.*(dum(i1,i2+1,i3,n)+dum(i1,i2-1,i3,n))     \
                -6.*(dum(i1,i2+2,i3,n)+dum(i1,i2-2,i3,n))     \
                   +(dum(i1,i2+3,i3,n)+dum(i1,i2-3,i3,n))  )*adxSosup(1)*.5

 maxwell3dr(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+\
            cdtdx*(u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n)-2.*u(i1,i2,i3,n))+\
            cdtdy*(u(i1,i2-1,i3,n)+u(i1,i2+1,i3,n)-2.*u(i1,i2,i3,n))+\
            cdtdz*(u(i1,i2,i3-1,n)+u(i1,i2,i3+1,n)-2.*u(i1,i2,i3,n))

 maxwellc22(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*f(i1,i2,i3,n)

 ! 2D, 2nd-order, div cleaning:
 !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )

 ! - rectangular: 
 mxdc2d2Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
            +cdcxx*(u(i1-1,i2,i3,ex)+u(i1+1,i2,i3,ex)-2.*u(i1,i2,i3,ex))\
            +cdcyy*(u(i1,i2-1,i3,ex)+u(i1,i2+1,i3,ex)-2.*u(i1,i2,i3,ex))\
            +cdcEdy*( u(i1,i2+1,i3,hz)-u(i1,i2-1,i3,hz) )

 mxdc2d2Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
            +cdcxx*(u(i1-1,i2,i3,ey)+u(i1+1,i2,i3,ey)-2.*u(i1,i2,i3,ey))\
            +cdcyy*(u(i1,i2-1,i3,ey)+u(i1,i2+1,i3,ey)-2.*u(i1,i2,i3,ey))\
            -cdcEdx*( u(i1+1,i2,i3,hz)-u(i1-1,i2,i3,hz) )

 ! - 2D curvilinear:  (assumes f contains Delta u )
 mxdc2d2cEx(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
                       + cdcf*f(i1,i2,i3,ex)\
                       + cdcE*uy22(i1,i2,i3,hz)

 mxdc2d2cEy(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
                       + cdcf*f(i1,i2,i3,ey)\
                       - cdcE*ux22(i1,i2,i3,hz)

#If #DIM eq "3"
 ! 3D, 2nd-order, div cleaning:
 !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )

 ! - 3D rectangular: 
 mxdc3d2Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
            +cdcxx*(u(i1-1,i2,i3,ex)+u(i1+1,i2,i3,ex)-2.*u(i1,i2,i3,ex))\
            +cdcyy*(u(i1,i2-1,i3,ex)+u(i1,i2+1,i3,ex)-2.*u(i1,i2,i3,ex))\
            +cdczz*(u(i1,i2,i3-1,ex)+u(i1,i2,i3+1,ex)-2.*u(i1,i2,i3,ex))\
            +cdcEdy*( u(i1,i2+1,i3,hz)-u(i1,i2-1,i3,hz) )\
            -cdcEdz*( u(i1,i2,i3+1,hy)-u(i1,i2,i3-1,hy) )

 mxdc3d2Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
            +cdcxx*(u(i1-1,i2,i3,ey)+u(i1+1,i2,i3,ey)-2.*u(i1,i2,i3,ey))\
            +cdcyy*(u(i1,i2-1,i3,ey)+u(i1,i2+1,i3,ey)-2.*u(i1,i2,i3,ey))\
            +cdczz*(u(i1,i2,i3-1,ey)+u(i1,i2,i3+1,ey)-2.*u(i1,i2,i3,ey))\
            +cdcEdz*( u(i1,i2,i3+1,hx)-u(i1,i2,i3-1,hx) )\
            -cdcEdx*( u(i1+1,i2,i3,hz)-u(i1-1,i2,i3,hz) )

 mxdc3d2Ez(i1,i2,i3) = cdc0*u(i1,i2,i3,ez)+cdc1*um(i1,i2,i3,ez)\
            +cdcxx*(u(i1-1,i2,i3,ez)+u(i1+1,i2,i3,ez)-2.*u(i1,i2,i3,ez))\
            +cdcyy*(u(i1,i2-1,i3,ez)+u(i1,i2+1,i3,ez)-2.*u(i1,i2,i3,ez))\
            +cdczz*(u(i1,i2,i3-1,ez)+u(i1,i2,i3+1,ez)-2.*u(i1,i2,i3,ez))\
            +cdcEdx*( u(i1+1,i2,i3,hy)-u(i1-1,i2,i3,hy) )\
            -cdcEdy*( u(i1,i2+1,i3,hx)-u(i1,i2-1,i3,hx) )

 mxdc3d2Hx(i1,i2,i3) = cdc0*u(i1,i2,i3,hx)+cdc1*um(i1,i2,i3,hx)\
            +cdcxx*(u(i1-1,i2,i3,hx)+u(i1+1,i2,i3,hx)-2.*u(i1,i2,i3,hx))\
            +cdcyy*(u(i1,i2-1,i3,hx)+u(i1,i2+1,i3,hx)-2.*u(i1,i2,i3,hx))\
            +cdczz*(u(i1,i2,i3-1,hx)+u(i1,i2,i3+1,hx)-2.*u(i1,i2,i3,hx))\
            -cdcHdy*( u(i1,i2+1,i3,ez)-u(i1,i2-1,i3,ez) )\
            +cdcHdz*( u(i1,i2,i3+1,ey)-u(i1,i2,i3-1,ey) )

 mxdc3d2Hy(i1,i2,i3) = cdc0*u(i1,i2,i3,hy)+cdc1*um(i1,i2,i3,hy)\
            +cdcxx*(u(i1-1,i2,i3,hy)+u(i1+1,i2,i3,hy)-2.*u(i1,i2,i3,hy))\
            +cdcyy*(u(i1,i2-1,i3,hy)+u(i1,i2+1,i3,hy)-2.*u(i1,i2,i3,hy))\
            +cdczz*(u(i1,i2,i3-1,hy)+u(i1,i2,i3+1,hy)-2.*u(i1,i2,i3,hy))\
            -cdcHdz*( u(i1,i2,i3+1,ex)-u(i1,i2,i3-1,ex) )\
            +cdcHdx*( u(i1+1,i2,i3,ez)-u(i1-1,i2,i3,ez) )

 mxdc3d2Hz(i1,i2,i3) = cdc0*u(i1,i2,i3,hz)+cdc1*um(i1,i2,i3,hz)\
            +cdcxx*(u(i1-1,i2,i3,hz)+u(i1+1,i2,i3,hz)-2.*u(i1,i2,i3,hz))\
            +cdcyy*(u(i1,i2-1,i3,hz)+u(i1,i2+1,i3,hz)-2.*u(i1,i2,i3,hz))\
            +cdczz*(u(i1,i2,i3-1,hz)+u(i1,i2,i3+1,hz)-2.*u(i1,i2,i3,hz))\
            -cdcHdx*( u(i1+1,i2,i3,ey)-u(i1-1,i2,i3,ey) )\
            +cdcHdy*( u(i1,i2+1,i3,ex)-u(i1,i2-1,i3,ex) )

 ! - 3D curvilinear:  (assumes f contains Delta u )
 mxdc3d2cEx(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
                       + cdcf*f(i1,i2,i3,ex)\
                       + cdcE*( uy23(i1,i2,i3,hz)\
                               -uz23(i1,i2,i3,hy))
 mxdc3d2cEy(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
                       + cdcf*f(i1,i2,i3,ey)\
                       + cdcE*( uz23(i1,i2,i3,hx)\
                               -ux23(i1,i2,i3,hz))
 mxdc3d2cEz(i1,i2,i3) = cdc0*u(i1,i2,i3,ez)+cdc1*um(i1,i2,i3,ez)\
                       + cdcf*f(i1,i2,i3,ez)\
                       + cdcE*( ux23(i1,i2,i3,hy)\
                               -uy23(i1,i2,i3,hx))

 mxdc3d2cHx(i1,i2,i3) = cdc0*u(i1,i2,i3,hx)+cdc1*um(i1,i2,i3,hx)\
                       + cdcf*f(i1,i2,i3,hx)\
                       + cdcH*(-uy23(i1,i2,i3,ez)\
                               +uz23(i1,i2,i3,ey))
 mxdc3d2cHy(i1,i2,i3) = cdc0*u(i1,i2,i3,hy)+cdc1*um(i1,i2,i3,hy)\
                       + cdcf*f(i1,i2,i3,hy)\
                       + cdcH*(-uz23(i1,i2,i3,ex)\
                               +ux23(i1,i2,i3,ez))
 mxdc3d2cHz(i1,i2,i3) = cdc0*u(i1,i2,i3,hz)+cdc1*um(i1,i2,i3,hz)\
                       + cdcf*f(i1,i2,i3,hz)\
                       + cdcH*(-ux23(i1,i2,i3,ey)\
                               +uy23(i1,i2,i3,ex))


#End



 ! Stoermer: 4th order in space and 4th order in time:
 maxwellr44(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+\
    c40*lap(n)+c41*v(I1,I2,I3,n)+c42*vvt2(I1,I2,I3,n)+c43*ut3(I1,I2,I3,n)

 maxwellc44(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+\
    c40*f(i1,i2,i3,n)+c41*v(I1,I2,I3,n)+c42*vvt2(I1,I2,I3,n)+c43*ut3(I1,I2,I3,n)

 ! Stoermer: 6th order in space and 6th order in time:
 maxwellr66(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+\
    c60*lap(n)+c61*v(I1,I2,I3,n)+c62*vvt2(I1,I2,I3,n)+c63*ut3(I1,I2,I3,n)+\
    c64*vvt4(I1,I2,I3,n)+c65*ut5(I1,I2,I3,n)

 maxwellc66(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+\
    c60*f(i1,i2,i3,n)+c61*v(I1,I2,I3,n)+c62*vvt2(I1,I2,I3,n)+c63*ut3(I1,I2,I3,n)+\
    c64*vvt4(I1,I2,I3,n)+c65*ut5(I1,I2,I3,n)

 ! Stoermer: 8th order in space and 8th order in time:
 maxwellr88(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+\
    c80*lap(n)+c81*v(I1,I2,I3,n)+c82*vvt2(I1,I2,I3,n)+c83*ut3(I1,I2,I3,n)+\
    c84*vvt4(I1,I2,I3,n)+c85*ut5(I1,I2,I3,n)+c86*ut6(I1,I2,I3,n)+c87*ut7(I1,I2,I3,n)

 maxwellc88(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+\
    c80*f(i1,i2,i3,n)+c81*v(I1,I2,I3,n)+c82*vvt2(I1,I2,I3,n)+c83*ut3(I1,I2,I3,n)+\
    c84*vvt4(I1,I2,I3,n)+c85*ut5(I1,I2,I3,n)+c86*ut6(I1,I2,I3,n)+c87*ut7(I1,I2,I3,n)


 !    *** 2nd order ***
 lap2d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi\
                   +(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))*dysqi

 lap2d2m(i1,i2,i3,c)=(um(i1+1,i2,i3,c)-2.*um(i1,i2,i3,c)+um(i1-1,i2,i3,c))*dxsqi\
                    +(um(i1,i2+1,i3,c)-2.*um(i1,i2,i3,c)+um(i1,i2-1,i3,c))*dysqi

 lap3d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi\
                   +(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))*dysqi\
                   +(u(i1,i2,i3+1,c)-2.*u(i1,i2,i3,c)+u(i1,i2,i3-1,c))*dzsqi

 lap3d2m(i1,i2,i3,c)=(um(i1+1,i2,i3,c)-2.*um(i1,i2,i3,c)+um(i1-1,i2,i3,c))*dxsqi\
                    +(um(i1,i2+1,i3,c)-2.*um(i1,i2,i3,c)+um(i1,i2-1,i3,c))*dysqi\
                    +(um(i1,i2,i3+1,c)-2.*um(i1,i2,i3,c)+um(i1,i2,i3-1,c))*dzsqi

 lap2d2f(i1,i2,i3,c,m)=(fa(i1+1,i2,i3,c,m)-2.*fa(i1,i2,i3,c,m)+fa(i1-1,i2,i3,c,m))*dxsqi\
                      +(fa(i1,i2+1,i3,c,m)-2.*fa(i1,i2,i3,c,m)+fa(i1,i2-1,i3,c,m))*dysqi

 lap3d2f(i1,i2,i3,c,m)=(fa(i1+1,i2,i3,c,m)-2.*fa(i1,i2,i3,c,m)+fa(i1-1,i2,i3,c,m))*dxsqi\
                      +(fa(i1,i2+1,i3,c,m)-2.*fa(i1,i2,i3,c,m)+fa(i1,i2-1,i3,c,m))*dysqi\
                      +(fa(i1,i2,i3+1,c,m)-2.*fa(i1,i2,i3,c,m)+fa(i1,i2,i3-1,c,m))*dzsqi


 ! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
 lap2d2Pow2(i1,i2,i3,c)= ( 6.*u(i1,i2,i3,c)   \
   - 4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))    \
       +(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*dxi4 \
   +( 6.*u(i1,i2,i3,c)    \
    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))    \
       +(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dyi4  \
   +( 8.*u(i1,i2,i3,c)     \
    -4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   \
    +2.*(u(i1+1,i2+1,i3,c)+u(i1-1,i2+1,i3,c)+u(i1+1,i2-1,i3,c)+u(i1-1,i2-1,i3,c)) )*dxdyi2

 ! 3D laplacian squared = u.xxxx + u.yyyy + u.zzzz + 2 (u.xxyy + u.xxzz + u.yyzz )
 lap3d2Pow2(i1,i2,i3,c)= ( 6.*u(i1,i2,i3,c)   \
   - 4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))    \
       +(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*dxi4 \
  +(  +6.*u(i1,i2,i3,c)    \
    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))    \
       +(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dyi4\
  +(  +6.*u(i1,i2,i3,c)    \
    -4.*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))    \
       +(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) )*dzi4\
   +(8.*u(i1,i2,i3,c)     \
    -4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   \
    +2.*(u(i1+1,i2+1,i3,c)+u(i1-1,i2+1,i3,c)+u(i1+1,i2-1,i3,c)+u(i1-1,i2-1,i3,c)) )*dxdyi2 \
   +(8.*u(i1,i2,i3,c)     \
    -4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))   \
    +2.*(u(i1+1,i2,i3+1,c)+u(i1-1,i2,i3+1,c)+u(i1+1,i2,i3-1,c)+u(i1-1,i2,i3-1,c)) )*dxdzi2 \
   +(8.*u(i1,i2,i3,c)     \
    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))   \
    +2.*(u(i1,i2+1,i3+1,c)+u(i1,i2-1,i3+1,c)+u(i1,i2+1,i3-1,c)+u(i1,i2-1,i3-1,c)) )*dydzi2 

 lap2d2Pow3(i1,i2,i3,c)=LAP2D2(lap2d2Pow2,i1,i2,i3,c)

 lap3d2Pow3(i1,i2,i3,c)=LAP3D2(lap3d2Pow2,i1,i2,i3,c)

 lap2d2Pow4(i1,i2,i3,c)=LAP2D2POW2(lap2d2Pow2,i1,i2,i3,c)
 lap3d2Pow4(i1,i2,i3,c)=LAP3D2POW2(lap3d2Pow2,i1,i2,i3,c)
 
!    ** 4th order ****

 lap2d4(i1,i2,i3,c)=( -30.*u(i1,i2,i3,c)     \
   +16.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     \
       -(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*dxsq12i + \
  ( -30.*u(i1,i2,i3,c)     \
   +16.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))     \
       -(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dysq12i 

 lap3d4(i1,i2,i3,c)=lap2d4(i1,i2,i3,c)+ \
  ( -30.*u(i1,i2,i3,c)      \
   +16.*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))      \
       -(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) )*dzsq12i 

 lap2d4Pow2(i1,i2,i3,c)=LAP2D4(lap2d4,i1,i2,i3,c)
 lap3d4Pow2(i1,i2,i3,c)=LAP3D4(lap3d4,i1,i2,i3,c)

 lap2d4Pow3(i1,i2,i3,c)=LAP2D4(lap2d4Pow2,i1,i2,i3,c)
 lap3d4Pow3(i1,i2,i3,c)=LAP3D4(lap3d4Pow2,i1,i2,i3,c)

!     *** 6th order ***

 lap2d6(i1,i2,i3,c)= \
          c00lap2d6*u(i1,i2,i3,c)     \
         +c10lap2d6*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)) \
         +c01lap2d6*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
         +c20lap2d6*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) \
         +c02lap2d6*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
         +c30lap2d6*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c)) \
         +c03lap2d6*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) 

 lap3d6(i1,i2,i3,c)=\
          c000lap3d6*u(i1,i2,i3,c) \
         +c100lap3d6*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)) \
         +c010lap3d6*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
         +c001lap3d6*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
         +c200lap3d6*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) \
         +c020lap3d6*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
         +c002lap3d6*(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) \
         +c300lap3d6*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c)) \
         +c030lap3d6*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) \
         +c003lap3d6*(u(i1,i2,i3+3,c)+u(i1,i2,i3-3,c))

 lap2d6Pow2(i1,i2,i3,c)=LAP2D6(lap2d6,i1,i2,i3,c)
 lap3d6Pow2(i1,i2,i3,c)=LAP3D6(lap3d6,i1,i2,i3,c)


!     *** 8th order ***

 lap2d8(i1,i2,i3,c)=c00lap2d8*u(i1,i2,i3,c)      \
          +c10lap2d8*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     \
          +c01lap2d8*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
          +c20lap2d8*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c))  \
          +c02lap2d8*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
          +c30lap2d8*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c))  \
          +c03lap2d8*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) \
          +c40lap2d8*(u(i1+4,i2,i3,c)+u(i1-4,i2,i3,c))  \
          +c04lap2d8*(u(i1,i2+4,i3,c)+u(i1,i2-4,i3,c))

 lap3d8(i1,i2,i3,c)=c000lap3d8*u(i1,i2,i3,c)      \
          +c100lap3d8*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     \
          +c010lap3d8*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
          +c001lap3d8*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
          +c200lap3d8*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c))  \
          +c020lap3d8*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
          +c002lap3d8*(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) \
          +c300lap3d8*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c))  \
          +c030lap3d8*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) \
          +c003lap3d8*(u(i1,i2,i3+3,c)+u(i1,i2,i3-3,c)) \
          +c400lap3d8*(u(i1+4,i2,i3,c)+u(i1-4,i2,i3,c))  \
          +c040lap3d8*(u(i1,i2+4,i3,c)+u(i1,i2-4,i3,c)) \
          +c004lap3d8*(u(i1,i2,i3+4,c)+u(i1,i2,i3-4,c))

! ******* artificial dissipation ******

!      (2nd difference)
 fd22d(i1,i2,i3,c)= \
 (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
  -4.*du(i1,i2,i3,c) )
!
 fd23d(i1,i2,i3,c)=\
 (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
   -6.*du(i1,i2,i3,c) )

!     -(fourth difference)
 fd42d(i1,i2,i3,c)= \
 (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) \
   +4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
  -12.*du(i1,i2,i3,c) )
!
 fd43d(i1,i2,i3,c)=\
 (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
   +4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
  -18.*du(i1,i2,i3,c) )

 ! (sixth  difference)
 fd62d(i1,i2,i3,c)= \
 (     ( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c) ) \
   -6.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) \
  +15.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
  -40.*du(i1,i2,i3,c) )

 fd63d(i1,i2,i3,c)=\
 (     ( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c)+du(i1,i2,i3-3,c)+du(i1,i2,i3+3,c) ) \
   -6.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
  +15.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
  -60.*du(i1,i2,i3,c) )

 ! -(eighth  difference)
 fd82d(i1,i2,i3,c)= \
 (    -( du(i1-4,i2,i3,c)+du(i1+4,i2,i3,c)+du(i1,i2-4,i3,c)+du(i1,i2+4,i3,c) ) \
   +8.*( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c) ) \
  -28.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) \
  +56.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
 -140.*du(i1,i2,i3,c) )

 fd83d(i1,i2,i3,c)=\
 (    -( du(i1-4,i2,i3,c)+du(i1+4,i2,i3,c)+du(i1,i2-4,i3,c)+du(i1,i2+4,i3,c)+du(i1,i2,i3-4,c)+du(i1,i2,i3+4,c) ) \
   +8.*( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c)+du(i1,i2,i3-3,c)+du(i1,i2,i3+3,c) ) \
  -28.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
  +56.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
 -210.*du(i1,i2,i3,c) )


!     **** Modified equation method: ****

 maxwell2dr44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*lap2d4(i1,i2,i3,n)\
                            +cdtsq12*lap2d2Pow2(i1,i2,i3,n)
 maxwell3dr44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*lap3d4(i1,i2,i3,n)\
                            +cdtsq12*lap3d2Pow2(i1,i2,i3,n)

 ! Order=4, 2D, rectangular, sosup-dissipation **FINISH ME**
 maxwell2dr44meSosup(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*lap2d4(i1,i2,i3,n)\
                            +cdtsq12*lap2d2Pow2(i1,i2,i3,n)+\
              (-20.*du(i1,i2,i3,n)     \
               +15.*(du(i1+1,i2,i3,n)+du(i1-1,i2,i3,n))     \
                -6.*(du(i1+2,i2,i3,n)+du(i1-2,i2,i3,n))     \
                   +(du(i1+3,i2,i3,n)+du(i1-3,i2,i3,n))  )*cdSosupx + \
              (-20.*du(i1,i2,i3,n)     \
               +15.*(du(i1,i2+1,i3,n)+du(i1,i2-1,i3,n))     \
                -6.*(du(i1,i2+2,i3,n)+du(i1,i2-2,i3,n))     \
                   +(du(i1,i2+3,i3,n)+du(i1,i2-3,i3,n))  )*cdSosupy


 maxwell2dr66me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*lap2d6(i1,i2,i3,n)\
                            +cdtsq12  *lap2d4Pow2(i1,i2,i3,n)\
                            +cdt4by360*lap2d2Pow3(i1,i2,i3,n)
 maxwell3dr66me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*lap3d6(i1,i2,i3,n)\
                            +cdtsq12*  lap3d4Pow2(i1,i2,i3,n)\
                            +cdt4by360*lap3d2Pow3(i1,i2,i3,n)

 maxwell2dr88me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*lap2d8(i1,i2,i3,n)\
                            +cdtsq12*lap2d6Pow2(i1,i2,i3,n)\
                            +cdt4by360*lap2d4Pow3(i1,i2,i3,n)+cdt6by20160*lap2d2Pow4(i1,i2,i3,n)
 maxwell3dr88me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*lap3d8(i1,i2,i3,n)\
                            +cdtsq12*lap3d6Pow2(i1,i2,i3,n)\
                            +cdt4by360*lap3d4Pow3(i1,i2,i3,n)+cdt6by20160*lap3d2Pow4(i1,i2,i3,n)

! *********NEW forcing method (for user defined forcing)**********
!    -- forcing correction for modified equation method ---
!        RHS = f + (dt^2/12)*( c^2 * Delta f + f_tt )
!  Approximate the term in brackets to 2nd-order
! ---- Cartesian grids:
! f2drme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)
! f2drme44(i1,i2,i3,n) = f(i1,i2,i3,n)
f2drme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*lap2d2f(i1,i2,i3,n,fcur) \
                       +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,i3,n,fcur)+fa(i1,i2,i3,n,fprev))/(12.)

f3drme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*lap3d2f(i1,i2,i3,n,fcur) \
                       +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,i3,n,fcur)+fa(i1,i2,i3,n,fprev))/(12.)

! ---- Curvilinear grids
ff(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)
f2dcme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*ffLaplacian22(i1,i2,i3,n) \
                       +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,i3,n,fcur)+fa(i1,i2,i3,n,fprev))/(12.)

f3dcme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*ffLaplacian23(i1,i2,i3,n) \
                       +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,i3,n,fcur)+fa(i1,i2,i3,n,fprev))/(12.)


 ! f  = csq*Lap4(u)+f,  v= (csq*Lap2)**2
 maxwellc44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*f(i1,i2,i3,n)+dt4by12*v(I1,I2,I3,n)
 ! these next are only valid for second order accuracy in time:
 maxwellc66me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*f(i1,i2,i3,n)
 maxwellc88me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*f(i1,i2,i3,n)

 ! for non-conservative modified-equation:
 max2dc44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*uLaplacian42(i1,i2,i3,n)+cdtsq12*uLapSq(n)

 ! This version for the 2-stage computation:

!$$$ vr2(i1,i2,i3,kd)=(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))*d12(0)
!$$$ vs2(i1,i2,i3,kd)=(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))*d12(1)
!$$$
!$$$ vrr2(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1+1,i2,i3,kd)+v(i1-1,i2,i3,kd)) )*d22(0)
!$$$ vss2(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1,i2+1,i3,kd)+v(i1,i2-1,i3,kd)) )*d22(1)
!$$$ vrs2(i1,i2,i3,kd)=(vr2(i1,i2+1,i3,kd)-vr2(i1,i2-1,i3,kd))*d12(1)
!$$$
!$$$ vlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*\
!$$$      vrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*\
!$$$      sy(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**\
!$$$      2)*vss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*vr2(i1,\
!$$$      i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vs2(i1,i2,i3,kd)

 max2dc44me2(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*uLaplacian42(i1,i2,i3,n)\
                                                       +cdtsq12*vLaplacian22(i1,i2,i3,n)

 max3dc44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*uLaplacian43(i1,i2,i3,n)\
                                                      +cdtsq12*vLaplacian23(i1,i2,i3,n)

 ! 2D, 4th-order, div cleaning:
 ! We could further optimize the D0x(lap2d2) ...
!!$ mxdc2d4Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
!!$            +cdcLap*lap2d4(i1,i2,i3,ex)\
!!$            +cdcLapsq*lap2d2Pow2(i1,i2,i3,ex)\
!!$            +cdcHz*uy42r(i1,i2,i3,hz)\
!!$            +cdcHzyLap*( lap2d2(i1,i2+1,i3,hz)-lap2d2(i1,i2-1,i3,hz) )
!!$
!!$ mxdc2d4Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
!!$            +cdcLap*lap2d4(i1,i2,i3,ey)\
!!$            +cdcLapsq*lap2d2Pow2(i1,i2,i3,ey)\
!!$            -cdcHz*ux42r(i1,i2,i3,hz)\
!!$            -cdcHzxLap*( lap2d2(i1+1,i2,i3,hz)-lap2d2(i1-1,i2,i3,hz) )
#If #DIM eq "2"
 ! 2D, 4th-order, rectangular, div cleaning:
 ! new version : here we replace curl( Delta H ) by an approx. to E_ttt that uses Delta E_t 
 mxdc2d4Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
            +cdcELap*lap2d4(i1,i2,i3,ex)\
            +cdcELapsq*lap2d2Pow2(i1,i2,i3,ex)\
            +cdcE*uy42r(i1,i2,i3,hz)\
            +cdcELapm*( lap2d2m(i1,i2,i3,ex) )

 mxdc2d4Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
            +cdcELap*lap2d4(i1,i2,i3,ey)\
            +cdcELapsq*lap2d2Pow2(i1,i2,i3,ey)\
            -cdcE*ux42r(i1,i2,i3,hz)\
            +cdcELapm*( lap2d2m(i1,i2,i3,ey) )

 ! 2d, 4th order, curvilinear (conservative), div cleaning (f=Lap(E), v=Lapsq(E))
 mxdc2d4cConsEx(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
            +cdcELap*f(i1,i2,i3,ex)\
            +cdcELapsq*v(i1,i2,i3,ex)\
            +cdcE*uy42(i1,i2,i3,hz)\
            +cdcELapm*( umLaplacian22(i1,i2,i3,ex) )

 mxdc2d4cConsEy(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
            +cdcELap*f(i1,i2,i3,ey)\
            +cdcELapsq*v(i1,i2,i3,ey)\
            -cdcE*ux42(i1,i2,i3,hz)\
            +cdcELapm*( umLaplacian22(i1,i2,i3,ey) )


 ! 2D, 4th-order, curvilinear, div cleaning: **check me**
 mxdc2d4cEx(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
            +cdcELap*uLaplacian42(i1,i2,i3,ex)\
            +cdcELapsq*vLaplacian22(i1,i2,i3,ex)\
            +cdcE*uy42(i1,i2,i3,hz)\
            +cdcELapm*( umLaplacian22(i1,i2,i3,ex) )

 mxdc2d4cEy(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
            +cdcELap*uLaplacian42(i1,i2,i3,ey)\
            +cdcELapsq*vLaplacian22(i1,i2,i3,ey)\
            -cdcE*ux42(i1,i2,i3,hz)\
            +cdcELapm*( umLaplacian22(i1,i2,i3,ey) )
#End
#If #DIM eq "3"
 ! 3D, 4th-order, rectangular, div cleaning:
 mxdc3d4Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)\
            +cdcELap*lap3d4(i1,i2,i3,ex)\
            +cdcELapsq*lap3d2Pow2(i1,i2,i3,ex)\
            +cdcE*( uy43r(i1,i2,i3,hz) \
                   -uz43r(i1,i2,i3,hy) )\
            +cdcELapm*( lap3d2m(i1,i2,i3,ex) )

 mxdc3d4Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)\
            +cdcELap*lap3d4(i1,i2,i3,ey)\
            +cdcELapsq*lap3d2Pow2(i1,i2,i3,ey)\
            +cdcE*( uz43r(i1,i2,i3,hx) \
                   -ux43r(i1,i2,i3,hz) )\
            +cdcELapm*( lap3d2m(i1,i2,i3,ey) )

 mxdc3d4Ez(i1,i2,i3) = cdc0*u(i1,i2,i3,ez)+cdc1*um(i1,i2,i3,ez)\
            +cdcELap*lap3d4(i1,i2,i3,ez)\
            +cdcELapsq*lap3d2Pow2(i1,i2,i3,ez)\
            +cdcE*( ux43r(i1,i2,i3,hy) \
                   -uy43r(i1,i2,i3,hx) )\
            +cdcELapm*( lap3d2m(i1,i2,i3,ez) )

 mxdc3d4Hx(i1,i2,i3) = cdc0*u(i1,i2,i3,hx)+cdc1*um(i1,i2,i3,hx)\
            +cdcHLap*lap3d4(i1,i2,i3,hx)\
            +cdcHLapsq*lap3d2Pow2(i1,i2,i3,hx)\
            +cdcH*(-uy43r(i1,i2,i3,ez) \
                   +uz43r(i1,i2,i3,ey) )\
            +cdcHLapm*( lap3d2m(i1,i2,i3,hx) )

 mxdc3d4Hy(i1,i2,i3) = cdc0*u(i1,i2,i3,hy)+cdc1*um(i1,i2,i3,hy)\
            +cdcHLap*lap3d4(i1,i2,i3,hy)\
            +cdcHLapsq*lap3d2Pow2(i1,i2,i3,hy)\
            +cdcH*(-uz43r(i1,i2,i3,ex) \
                   +ux43r(i1,i2,i3,ez) )\
            +cdcHLapm*( lap3d2m(i1,i2,i3,hy) )

 mxdc3d4Hz(i1,i2,i3) = cdc0*u(i1,i2,i3,hz)+cdc1*um(i1,i2,i3,hz)\
            +cdcHLap*lap3d4(i1,i2,i3,hz)\
            +cdcHLapsq*lap3d2Pow2(i1,i2,i3,hz)\
            +cdcH*(-ux43r(i1,i2,i3,ey) \
                   +uy43r(i1,i2,i3,ex) )\
            +cdcHLapm*( lap3d2m(i1,i2,i3,ez) )

#End
!...........end   statement functions


 ! write(*,*) 'Inside advMaxwell...'

 cc    =rpar(0)  ! this is c
 dt    =rpar(1)
 dx(0) =rpar(2)
 dx(1) =rpar(3)
 dx(2) =rpar(4)
 adc   =rpar(5)  ! coefficient of artificial dissipation
 add   =rpar(6)  ! coefficient of divergence damping    
 dr(0) =rpar(7)
 dr(1) =rpar(8)
 dr(2) =rpar(9)
 eps   =rpar(10)
 mu    =rpar(11) 
 kx    =rpar(12) 
 ky    =rpar(13) 
 kz    =rpar(14) 
 sigmaE=rpar(15)  ! electric conductivity (for lossy materials, complex index of refraction)
 sigmaH=rpar(16)  ! magnetic conductivity
 divergenceCleaningCoefficient=rpar(17)
 t     =rpar(18)

 rpar(20)=0.  ! return the time used for adding dissipation

 ! Drude-Lorentz dispersion model:
 gamma= rpar(21)
 omegap=rpar(22)

 sosupParameter=rpar(23)

 dy=dx(1)  ! Are these needed?
 dz=dx(2)

 ! timeForArtificialDissipation=rpar(6) ! return value

 option             =ipar(0)
 gridType           =ipar(1)
 orderOfAccuracy    =ipar(2)
 orderInTime        =ipar(3)
 addForcing         =ipar(4)
 orderOfDissipation =ipar(5)
 ex                 =ipar(6)
 ey                 =ipar(7)
 ez                 =ipar(8)
 hx                 =ipar(9)
 hy                 =ipar(10)
 hz                 =ipar(11)
 solveForE          =ipar(12)
 solveForH          =ipar(13)
 useWhereMask       =ipar(14)
 timeSteppingMethod =ipar(15)
 useVariableDissipation        =ipar(16)
 useCurvilinearOpt             =ipar(17)
 useConservative               =ipar(18)   
 combineDissipationWithAdvance =ipar(19)
 useDivergenceCleaning         =ipar(20)
 useNewForcingMethod           =ipar(21)
 numberOfForcingFunctions      =ipar(22)
 fcur                          =ipar(23) 
 dispersionModel     =ipar(24)
 pxc                 =ipar(25)
 pyc                 =ipar(26)
 pzc                 =ipar(27)
 qxc                 =ipar(28)
 qyc                 =ipar(29)
 qzc                 =ipar(30)
 rxc                 =ipar(31)
 ryc                 =ipar(32)
 rzc                 =ipar(33)

 useSosupDissipation   =ipar(34)
 sosupDissipationOption=ipar(35)
 updateSolution        =ipar(36)
 updateDissipation     =ipar(37)

 fprev = mod(fcur-1+numberOfForcingFunctions,max(1,numberOfForcingFunctions))
 fnext = mod(fcur+1                         ,max(1,numberOfForcingFunctions))
 

 ! addDissipation=.true. if we add the dissipation in the dis(i1,i2,i3,c) array
 !  if combineDissipationWithAdvance.ne.0 we compute the dissipation on the fly in the time step
 !  rather than pre-computing it in diss(i1,i2,i3,c)
 addDissipation = adc.gt.0. .and. combineDissipationWithAdvance.eq.0
 adcdt=adc*dt

 csq=cc**2
 dtsq=dt**2

 cdt=cc*dt

 cdtsq=(cc**2)*(dt**2)
 cdtsq12=cdtsq*cdtsq/12.  ! c^4 dt^4 /14 
 cdt4by360=(cdt)**4/360.
 cdt6by20160=cdt**6/(8.*7.*6.*5.*4.*3.)

 cdtSqBy12= cdtsq/12.   ! c^2*dt*2/12

 dt4by12=dtsq*dtsq/12.

 cdtdx = (cc*dt/dx(0))**2
 cdtdy = (cc*dt/dy)**2
 cdtdz = (cc*dt/dz)**2

 dxsqi=1./(dx(0)**2)
 dysqi=1./(dy**2)
 dzsqi=1./(dz**2)

 dxsq12i=1./(12.*dx(0)**2)
 dysq12i=1./(12.*dy**2)
 dzsq12i=1./(12.*dz**2)

 dxi4=1./(dx(0)**4)
 dyi4=1./(dy**4)
 dxdyi2=1./(dx(0)*dx(0)*dy*dy)

 dzi4=1./(dz**4)
 dxdzi2=1./(dx(0)*dx(0)*dz*dz)
 dydzi2=1./(dy*dy*dz*dz)

 gammaDt=gamma*dt
 omegapDtSq=(omegap*dt)**2
 if( t.eq.0. .and. dispersionModel.ne.noDispersion )then
    write(*,'("--advOpt-- dispersionModel=",i4," px,py,pz=",3i2)') dispersionModel,pxc,pyc,pzc
 end if

 if( useSosupDissipation.ne.0 )then
 
  ! Coefficients in the sosup dissipation from Jordan Angel
  if( orderOfAccuracy.eq.2 )then
   adSosup=cc*dt*1./8.
  else if( orderOfAccuracy.eq.4 )then 
    adSosup=cc*dt*5./288.
  else
    stop 1005
  end if

  ! sosupParameter=gamma in sosup scheme  0<= gamma <=1   0=centered scheme
  adSosup=sosupParameter*adSosup 
  if( t.le.2*dt )then
    write(*,'("advOPT: useSosup dissipation, t,dt,adSosup=",3e10.2)') t,dt,adSosup
    write(*,'("advOPT: sosupDissipationOption=",i2)') sosupDissipationOption
    write(*,'("advOPT: updateDissipation=",i2)') updateDissipation
    write(*,'("advOPT: updateSolution=",i2)') updateSolution
    write(*,'("advOPT: useNewForcingMethod=",i2)') useNewForcingMethod
  end if
  ! Coefficients of the sosup dissipation with Cartesian grids:
  cdSosupx= adSosup/dx(0)
  cdSosupy= adSosup/dx(1)
  cdSosupz= adSosup/dx(2)


 end if

 if( useDivergenceCleaning.eq.1 )then
   ! Here are the coefficients that define the div cleaning formulae
   !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )
   if( orderOfAccuracy.eq.2 )then
     ! 2D, 2nd-order, div cleaning:
     dc = divergenceCleaningCoefficient
     dcp = 1. + dc*dt*.5
     cdc0 = 2./dcp
     cdc1 = -(1.-dc*dt*.5)/dcp

     cdcxx = (cc*dt)**2/(dx(0)**2)/dcp
     cdcyy = (cc*dt)**2/(dx(1)**2)/dcp
     cdczz = (cc*dt)**2/(dx(2)**2)/dcp
     ! for div(H) damping in E eqn:
     cdcEdx= dc*dt**2/(eps*2.*dx(0))/dcp
     cdcEdy= dc*dt**2/(eps*2.*dx(1))/dcp
     cdcEdz= dc*dt**2/(eps*2.*dx(2))/dcp
     ! for div(E) damping in H eqn:
     cdcHdx= dc*dt**2/(mu*2.*dx(0))/dcp
     cdcHdy= dc*dt**2/(mu*2.*dx(1))/dcp
     cdcHdz= dc*dt**2/(mu*2.*dx(2))/dcp

     ! These next two are for the curvilinear case:
     cdcf = (cc*dt)**2/dcp
     cdcE = dc*dt**2/(eps)/dcp    
     cdcH = dc*dt**2/(mu )/dcp    

     if( t.eq.0. )then
       write(*,'(" advOpt: order=2 : div clean: dc,cc,dt,eps,mu=",5e10.2)') dc,cc,dt,eps,mu
       write(*,'(" advOpt: div clean: cdc0,cdc1,cdcxx,cdcyy,cdcHdy,cdcHdx=",6e10.2)') cdc0,cdc1,cdcxx,cdcyy,cdcHdy,cdcHdx
     end if
   else if( orderOfAccuracy.eq.4 )then

     dc = divergenceCleaningCoefficient
     dcp = 1. + dc*dt*.5
     cdc0 = 2./dcp
     cdc1 = -(1.-dc*dt*.5)/dcp

     cdcE= dc*dt**2/(eps)/dcp
     cdcELap= ((cc*dt)**2/dcp)*( 1. + dc*dt/(6.*eps) )
     cdcELapsq = ((cc*dt)**4/12./dcp)*( 1. + dc*dt/eps )
     cdcELapm = ((cc*dt)**2/dcp)*( - dc*dt/(6.*eps) )
     
     cdcH= dc*dt**2/(mu )/dcp
     cdcHLap= ((cc*dt)**2/dcp)*( 1. + dc*dt/(6.*mu) )
     cdcHLapsq = ((cc*dt)**4/12./dcp)*( 1. + dc*dt/mu )
     cdcHLapm = ((cc*dt)**2/dcp)*( - dc*dt/(6.*mu ) )

     if( t.eq.0. )then
       write(*,'(" advOpt: order=4 :  div clean: dc,cc,dt,eps,mu=",5e10.2)') dc,cc,dt,eps,mu
       write(*,'(" advOpt: div clean: cdc0,cdc1,cdcELap,cdcELapsq,cdcE,cdcELapm=",8e10.2)') cdc0,cdc1,cdcELap,cdcELapsq,cdcE,cdcELapm
     end if




   else
    write(*,'(" advOpt.bf: un-implemented orderOfAccuracy for div-cleaning")') 
    stop 2277
   end if
 end if

 if( orderOfAccuracy.eq.6 )then
   if( nd.eq.2 )then
     c00lap2d6=csq*(-49./18.)*(1./dx(0)**2+1./dy**2)
     c10lap2d6=csq*(1.5     )*(1./dx(0)**2)
     c01lap2d6=csq*(1.5     )*(1./dy**2)
     c20lap2d6=csq*(-3./20. )*(1./dx(0)**2)
     c02lap2d6=csq*(-3./20. )*(1./dy**2)
     c30lap2d6=csq*(1./90.  )*(1./dx(0)**2)
     c03lap2d6=csq*(1./90.  )*(1./dy**2)
   else
     c000lap3d6=csq*(-49./18.)*(1./dx(0)**2+1./dy**2+1./dz**2)
     c100lap3d6=csq*(1.5     )*(1./dx(0)**2)
     c010lap3d6=csq*(1.5     )*(1./dy**2)
     c001lap3d6=csq*(1.5     )*(1./dz**2)
     c200lap3d6=csq*(-3./20. )*(1./dx(0)**2)
     c020lap3d6=csq*(-3./20. )*(1./dy**2)
     c002lap3d6=csq*(-3./20. )*(1./dz**2)
     c300lap3d6=csq*(1./90.  )*(1./dx(0)**2)
     c030lap3d6=csq*(1./90.  )*(1./dy**2)
     c003lap3d6=csq*(1./90.  )*(1./dz**2)
   end if
 end if
 if( orderOfAccuracy.eq.8 )then
   if( nd.eq.2 )then
     c00lap2d8=csq*(-205./72.)*(1./dx(0)**2+1./dy**2)
     c10lap2d8=csq*(8./5.    )*(1./dx(0)**2)
     c01lap2d8=csq*(8./5.    )*(1./dy**2)
     c20lap2d8=csq*(-1./5.   )*(1./dx(0)**2)
     c02lap2d8=csq*(-1./5.   )*(1./dy**2)
     c30lap2d8=csq*(8./315.  )*(1./dx(0)**2)
     c03lap2d8=csq*(8./315.  )*(1./dy**2)
     c40lap2d8=csq*(-1./560. )*(1./dx(0)**2)
     c04lap2d8=csq*(-1./560. )*(1./dy**2)
   else
     c000lap3d8=csq*(-205./72.)*(1./dx(0)**2+1./dy**2+1./dz**2)
     c100lap3d8=csq*(8./5.    )*(1./dx(0)**2)
     c010lap3d8=csq*(8./5.    )*(1./dy**2)
     c001lap3d8=csq*(8./5.    )*(1./dz**2)
     c200lap3d8=csq*(-1./5.   )*(1./dx(0)**2)
     c020lap3d8=csq*(-1./5.   )*(1./dy**2)
     c002lap3d8=csq*(-1./5.   )*(1./dz**2)
     c300lap3d8=csq*(8./315.  )*(1./dx(0)**2)
     c030lap3d8=csq*(8./315.  )*(1./dy**2)
     c003lap3d8=csq*(8./315.  )*(1./dz**2)
     c400lap3d8=csq*(-1./560. )*(1./dx(0)**2)
     c040lap3d8=csq*(-1./560. )*(1./dy**2)
     c004lap3d8=csq*(-1./560. )*(1./dz**2)
   end if
 end if

 if( orderInTime.eq.4 )then
   c40=( 7./6. )*dtsq
   c41=(-5./12.)*dtsq
   c42=( 1./3. )*dtsq
   c43=(-1./12.)*dtsq
 else if( orderInTime.eq.6 )then
   c60=( 317./240.)*dtsq    ! from stoermer.maple
   c61=(-266./240.)*dtsq
   c62=( 374./240.)*dtsq
   c63=(-276./240.)*dtsq
   c64=( 109./240.)*dtsq
   c65=( -18./240.)*dtsq
 else if( orderInTime.eq.8 )then 

!     g := 1/60480 (236568 fv[4] + 88324 fv[0] - 121797 fv[1] + 245598 fv[2] 
!     + 33190 fv[6] - 4125 fv[7] - 300227 fv[3] - 117051 fv[5])

   c80=(  88324./60480.)*dtsq ! from stoermer.maple
   c81=(-121797./60480.)*dtsq
   c82=( 245598./60480.)*dtsq
   c83=(-300227./60480.)*dtsq
   c84=( 236568./60480.)*dtsq
   c85=(-117051./60480.)*dtsq
   c86=(  33190./60480.)*dtsq
   c87=(  -4125./60480.)*dtsq
 end if


  ! This next function will:
  !   (1) optionally compute the dissipation and fill in the diss array 
  !            if: (adc.gt.0. .and. combineDissipationWithAdvance.eq.0
  !   (2) add the divergence damping
  !         if( add.gt.0. )
 if( nd.eq.2 .and. orderOfAccuracy.eq.2 )then
   call advMxDiss2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,\
     nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,\
     vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
 else if(  nd.eq.2 .and. orderOfAccuracy.eq.4 )then
   call advMxDiss2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,\
     nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,\
     vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
 else if( nd.eq.3 .and. orderOfAccuracy.eq.2 )then
   call advMxDiss3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,\
     nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,\
     vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
 else if(  nd.eq.3 .and. orderOfAccuracy.eq.4 )then
   call advMxDiss3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,\
     nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,\
     vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
 else
   if( (adc.gt.0. .and. combineDissipationWithAdvance.eq.0) .or. add.gt.0. )then
     stop 1116
   end if
 end if


 if( option.eq.1 ) then
   return
 end if


! write(*,'(" advMaxwell: timeSteppingMethod=",i2)') timeSteppingMethod
 if( timeSteppingMethod.eq.defaultTimeStepping )then
  write(*,'(" advMaxwell:ERROR: timeSteppingMethod=defaultTimeStepping -- this should be set")')
    ! '
  stop 83322
 end if

 if( gridType.eq.rectangular )then

 #If #GRIDTYPE eq "rectangular"

!       **********************************************
!       *************** rectangular ******************
!       **********************************************

 #If #ORDER eq "2" 

   #If #DIM eq "2"
    if( dispersionModel.ne.noDispersion )then
      ! --dispersion model --


      write(*,'("--advOpt-- advance 2D dispersive model")') 
      fp=0
      fe=0.
      beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
 
        ! Advance Hz first:
        ! For now solve H_t = -(1/mu)*(  (E_y)_x - (E_x)_y )
        !   USE AB2 -- note: this is just a quadrature so stability is not an inssue
        un(i1,i2,i3,hz) = u(i1,i2,i3,hz) -(dt/mu)*( 1.5*ux22r(i1,i2,i3,ey) -.5*umx22r(i1,i2,i3,ey) \
                                                   -1.5*uy22r(i1,i2,i3,ex) +.5*umy22r(i1,i2,i3,ex) )
        if( addForcing.ne.0 )then
          un(i1,i2,i3,hz) = un(i1,i2,i3,hz) + dt*f(i1,i2,i3,hz) ! first order only **FIX ME**
        end if

        !   H_tt = c^2 Delta(H) + c^2 curl( P_t)  -- equation for H , *check me*
        !  **finish me**
        !  if( addForcing.eq.0 )then
        !    un(i1,i2,i3,hz)=maxwell2dr(i1,i2,i3,hz)
        !  else
        !    un(i1,i2,i3,hz)=maxwell2dr(i1,i2,i3,hz)+dtsq*f(i1,i2,i3,hz)
        !  end if

        ! scheme from Jeff: 
        
        do m=0,1
         pc=pxc+m
         ec=ex+m

         if( addForcing.ne.0 )then
           fp = dtsq*f(i1,i2,i3,pc) 
           fe = dtsq*f(i1,i2,i3,ec) 
         end if 
         un(i1,i2,i3,pc)=( 2.*u(i1,i2,i3,pc)- (1.-gammaDt*.5)*um(i1,i2,i3,pc) + omegapDtSq*u(i1,i2,i3,ec) + fp )/(1.+gammaDt*.5)
         ptt = un(i1,i2,i3,pc)-2.*u(i1,i2,i3,pc)+um(i1,i2,i3,pc)
         ! write(*,'(" ptt=",e10.2)') ptt
         un(i1,i2,i3,ec)=maxwell2dr(i1,i2,i3,ec)+ fe - ptt/eps

        end do


      endLoopsMask()

    else if( useSosupDissipation.ne.0 )then

     ! FD22 (rectangular grid) with Sosup (wide stencil dissiption)
     if( sosupDissipationOption.eq.0 )then
      ! advance + sosup dissipation: 
      loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
               un(i1,i2,i3,ex)=maxwell2drSosup(i1,i2,i3,ex),\
               un(i1,i2,i3,ey)=maxwell2drSosup(i1,i2,i3,ey),\
               un(i1,i2,i3,hz)=maxwell2drSosup(i1,i2,i3,hz),,,,,,)
     else if( sosupDissipationOption.eq.1 )then
      ! --- TWO STAGES ---

      ! apply sosup dissipation to time n-1 using times n-1 and n-3
      ! assume un holds u(t-2*dt) on input 
      if( updateDissipation.eq.1 )then
        loopse6(u(i1,i2,i3,ex)=sosupDiss2d4r(i1,i2,i3,ex),\
                u(i1,i2,i3,ey)=sosupDiss2d4r(i1,i2,i3,ey),\
                u(i1,i2,i3,hz)=sosupDiss2d4r(i1,i2,i3,hz),,,)      
      end if
      ! advance to time n+1
      if( updateSolution.eq.1 )then
        loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,ex)=maxwell2dr(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=maxwell2dr(i1,i2,i3,ey),\
                un(i1,i2,i3,hz)=maxwell2dr(i1,i2,i3,hz),,,,,,)
      end if
     else
       write(*,'("advOpt: ERROR: unexpected sosupDissipationOption=",i2)') sosupDissipationOption
       stop 1010
     end if

    else if( useDivergenceCleaning.eq.0 )then

     ! FD22 with no dissipation
     loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=maxwell2dr(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwell2dr(i1,i2,i3,ey),\
              un(i1,i2,i3,hz)=maxwell2dr(i1,i2,i3,hz),,,,,,)
    else
     ! 2D, 2nd-order, div cleaning:
     !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )
     write(*,'("advMaxwell: advance 2D, 2nd-order, rectangular, div cleaning... t=",e10.2)') t
     loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=mxdc2d2Ex(i1,i2,i3),\
              un(i1,i2,i3,ey)=mxdc2d2Ey(i1,i2,i3),\
              un(i1,i2,i3,hz)=maxwell2dr(i1,i2,i3,hz),,,,,,)
    endif 
   #Else
    if( useDivergenceCleaning.eq.0 )then
     loopsF3DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
              un(i1,i2,i3,ex)=maxwell3dr(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwell3dr(i1,i2,i3,ey),\
              un(i1,i2,i3,ez)=maxwell3dr(i1,i2,i3,ez),,,,,,,\
              dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,hx)=maxwell3dr(i1,i2,i3,hx),\
              un(i1,i2,i3,hy)=maxwell3dr(i1,i2,i3,hy),\
              un(i1,i2,i3,hz)=maxwell3dr(i1,i2,i3,hz),,,,,,)
    else
     ! 3D, 2nd-order, div cleaning:
     write(*,'("advMaxwell: advance 3D, 2nd-order, rectangular, div cleaning... t=",e10.2)') t
     loopsF3DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
              un(i1,i2,i3,ex)=mxdc3d2Ex(i1,i2,i3),\
              un(i1,i2,i3,ey)=mxdc3d2Ey(i1,i2,i3),\
              un(i1,i2,i3,ez)=mxdc3d2Ez(i1,i2,i3),,,,,,,\
              dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,hx)=mxdc3d2Hx(i1,i2,i3),\
              un(i1,i2,i3,hy)=mxdc3d2Hy(i1,i2,i3),\
              un(i1,i2,i3,hz)=mxdc3d2Hz(i1,i2,i3),,,,,,)
    end if
   #End

 #Elif #ORDER eq "4" 

   ! ======================================================================================
   ! ==================   4th order in space and 4th order in time: =======================
   ! ======================================================================================
   ! write(*,*) 'Inside advMaxwell order=4...'

   if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

     #If #DIM eq "2"

      ! ------------------------------------------------------------------------------
      !    2D : 4th order modified equation (rectangular)
      ! ------------------------------------------------------------------------------

      if( useDivergenceCleaning.eq.0 )then

       if( useSosupDissipation.ne.0 )then

         ! ---- use sosup dissipation (wider stencil) ---

         if( useNewForcingMethod.ne.0 )then
          write(*,'(" finish me: useSosupDissipation && useNewForcingMethod")')
          stop 7733
         end if 


        ! FD22 (rectangular grid) with Sosup (wide stencil dissiption)
        if( sosupDissipationOption.eq.0 )then
         ! advance + sosup dissipation: 
         loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,ex)=maxwell2dr44meSosup(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=maxwell2dr44meSosup(i1,i2,i3,ey),\
                un(i1,i2,i3,hz)=maxwell2dr44meSosup(i1,i2,i3,hz),,,,,,) 
        else if( sosupDissipationOption.eq.1 )then
         ! --- TWO STAGES ---

         ! apply sosup dissipation to time n-1 using times n-1 and n-3
         ! assume un holds u(t-2*dt) on input 
         if( updateDissipation.eq.1 )then
          loopse6(u(i1,i2,i3,ex)=sosupDiss2d6r(i1,i2,i3,ex),\
                  u(i1,i2,i3,ey)=sosupDiss2d6r(i1,i2,i3,ey),\
                  u(i1,i2,i3,hz)=sosupDiss2d6r(i1,i2,i3,hz),,,)      
         end if
         ! advance to time n+1
         if( updateSolution.eq.1 )then
           loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,ex)=maxwell2dr44me(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=maxwell2dr44me(i1,i2,i3,ey),\
                un(i1,i2,i3,hz)=maxwell2dr44me(i1,i2,i3,hz),,,,,,) 
         end if
        else
          write(*,'("advOpt: ERROR: unexpected sosupDissipationOption=",i2)') sosupDissipationOption
          stop 1010
        end if


       else if( useNewForcingMethod.eq.1 ) then

         ! fix forcing for ME scheme to be 4th-order
         if( combineDissipationWithAdvance.eq.0 )then
          write(*,*) 'advOpt: 2d, rect, 4th-order fix-force modified equation'
          loopsF2DD(dtsq*f2drme44(i1,i2,i3,ex),dtsq*f2drme44(i1,i2,i3,ey),dtsq*f2drme44(i1,i2,i3,hz),\
                 un(i1,i2,i3,ex)=maxwell2dr44me(i1,i2,i3,ex),\
                 un(i1,i2,i3,ey)=maxwell2dr44me(i1,i2,i3,ey),\
                 un(i1,i2,i3,hz)=maxwell2dr44me(i1,i2,i3,hz),,,,,,) 

         else
          ! modified equation and dissipation in one loop
          write(*,*) 'advOpt: 2d, rect, 4th-order, diss, fix-force modified equation'
          loopsF2D(dtsq*f2drme44(i1,i2,i3,ex),dtsq*f2drme44(i1,i2,i3,ey),dtsq*f2drme44(i1,i2,i3,hz),\
                   un(i1,i2,i3,ex)=maxwell2dr44me(i1,i2,i3,ex)+adcdt*fd42d(i1,i2,i3,ex),\
                   un(i1,i2,i3,ey)=maxwell2dr44me(i1,i2,i3,ey)+adcdt*fd42d(i1,i2,i3,ey),\
                   un(i1,i2,i3,hz)=maxwell2dr44me(i1,i2,i3,hz)+adcdt*fd42d(i1,i2,i3,hz),,,,,,)
         end if

       else if( combineDissipationWithAdvance.eq.0 )then
        ! write(*,*) 'advOpt: 2d, rect, modified equation'
        loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
               un(i1,i2,i3,ex)=maxwell2dr44me(i1,i2,i3,ex),\
               un(i1,i2,i3,ey)=maxwell2dr44me(i1,i2,i3,ey),\
               un(i1,i2,i3,hz)=maxwell2dr44me(i1,i2,i3,hz),,,,,,) 

       else
        ! modified equation and dissipation in one loop
        loopsF2D(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
                 un(i1,i2,i3,ex)=maxwell2dr44me(i1,i2,i3,ex)+adcdt*fd42d(i1,i2,i3,ex),\
                 un(i1,i2,i3,ey)=maxwell2dr44me(i1,i2,i3,ey)+adcdt*fd42d(i1,i2,i3,ey),\
                 un(i1,i2,i3,hz)=maxwell2dr44me(i1,i2,i3,hz)+adcdt*fd42d(i1,i2,i3,hz),,,,,,)
       end if
      else
       ! 2D, 4th-order, div cleaning:
       !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )
       write(*,'("advMaxwell: advance 2D, 4th-order, rect, div cleaning... t=",e10.2,", adcdt=",e10.2 )') t,adcdt

       if( useNewForcingMethod.eq.1 ) then
         write(*,'(" advOpt: FINISH ME")')
         stop 10123
       end if

       if( combineDissipationWithAdvance.eq.0 )then
         loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,ex)=mxdc2d4Ex(i1,i2,i3),\
                un(i1,i2,i3,ey)=mxdc2d4Ey(i1,i2,i3),\
                un(i1,i2,i3,hz)=maxwell2dr44me(i1,i2,i3,hz),,,,,,)
       else
         loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,ex)=mxdc2d4Ex(i1,i2,i3)+adcdt*fd42d(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=mxdc2d4Ey(i1,i2,i3)+adcdt*fd42d(i1,i2,i3,ey),\
                un(i1,i2,i3,hz)=maxwell2dr44me(i1,i2,i3,hz)+adcdt*fd42d(i1,i2,i3,hz),,,,,,)
       end if

      end if

     #Else

      ! ------------------------------------------------------------------------------
      !    3D : 4th order modified equation (rectangular)
      ! ------------------------------------------------------------------------------


       if( useDivergenceCleaning.eq.0 )then

        if( useNewForcingMethod.eq.1 ) then

         if( combineDissipationWithAdvance.eq.0 )then
          ! 4th order modified equation 
          loopsF3DD(dtsq*f3drme44(i1,i2,i3,ex),dtsq*f3drme44(i1,i2,i3,ey),dtsq*f3drme44(i1,i2,i3,ez),\
                un(i1,i2,i3,ex)=maxwell3dr44me(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=maxwell3dr44me(i1,i2,i3,ey),\
                un(i1,i2,i3,ez)=maxwell3dr44me(i1,i2,i3,ez),,,,,,,\
                dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,hx)=maxwell3dr44me(i1,i2,i3,hx),\
                un(i1,i2,i3,hy)=maxwell3dr44me(i1,i2,i3,hy),\
                un(i1,i2,i3,hz)=maxwell3dr44me(i1,i2,i3,hz),,,,,,)
          else
           ! 4th order modified equation and dissipation in one loop
           loopsF3D(dtsq*f3drme44(i1,i2,i3,ex),dtsq*f3drme44(i1,i2,i3,ey),dtsq*f3drme44(i1,i2,i3,ez),\
                un(i1,i2,i3,ex)=maxwell3dr44me(i1,i2,i3,ex)+adcdt*fd43d(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=maxwell3dr44me(i1,i2,i3,ey)+adcdt*fd43d(i1,i2,i3,ey),\
                un(i1,i2,i3,ez)=maxwell3dr44me(i1,i2,i3,ez)+adcdt*fd43d(i1,i2,i3,ez),,,,,,,\
                dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,hx)=maxwell3dr44me(i1,i2,i3,hx)+adcdt*fd43d(i1,i2,i3,hx),\
                un(i1,i2,i3,hy)=maxwell3dr44me(i1,i2,i3,hy)+adcdt*fd43d(i1,i2,i3,hy),\
                un(i1,i2,i3,hz)=maxwell3dr44me(i1,i2,i3,hz)+adcdt*fd43d(i1,i2,i3,hz),,,,,,)
          end if

        else

         if( combineDissipationWithAdvance.eq.0 )then
          ! 4th order modified equation 
          loopsF3DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
                un(i1,i2,i3,ex)=maxwell3dr44me(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=maxwell3dr44me(i1,i2,i3,ey),\
                un(i1,i2,i3,ez)=maxwell3dr44me(i1,i2,i3,ez),,,,,,,\
                dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,hx)=maxwell3dr44me(i1,i2,i3,hx),\
                un(i1,i2,i3,hy)=maxwell3dr44me(i1,i2,i3,hy),\
                un(i1,i2,i3,hz)=maxwell3dr44me(i1,i2,i3,hz),,,,,,)
          else
           ! 4th order modified equation and dissipation in one loop
           loopsF3D(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
                un(i1,i2,i3,ex)=maxwell3dr44me(i1,i2,i3,ex)+adcdt*fd43d(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=maxwell3dr44me(i1,i2,i3,ey)+adcdt*fd43d(i1,i2,i3,ey),\
                un(i1,i2,i3,ez)=maxwell3dr44me(i1,i2,i3,ez)+adcdt*fd43d(i1,i2,i3,ez),,,,,,,\
                dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,hx)=maxwell3dr44me(i1,i2,i3,hx)+adcdt*fd43d(i1,i2,i3,hx),\
                un(i1,i2,i3,hy)=maxwell3dr44me(i1,i2,i3,hy)+adcdt*fd43d(i1,i2,i3,hy),\
                un(i1,i2,i3,hz)=maxwell3dr44me(i1,i2,i3,hz)+adcdt*fd43d(i1,i2,i3,hz),,,,,,)
          end if
         end if 

      else
         ! -- div clean
        write(*,'("advMaxwell: advance 3D, 4th-order, rect, div cleaning... t=",e10.2,", adcdt=",e10.2 )') t,adcdt

        if( useNewForcingMethod.eq.1 ) then
          write(*,'(" advOpt: FINISH ME")')
          stop 10124
        end if

        if( combineDissipationWithAdvance.eq.0 )then
         ! 4th order modified equation 
         loopsF3DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
               un(i1,i2,i3,ex)=mxdc3d4Ex(i1,i2,i3),\
               un(i1,i2,i3,ey)=mxdc3d4Ey(i1,i2,i3),\
               un(i1,i2,i3,ez)=mxdc3d4Ez(i1,i2,i3),,,,,,,\
               dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
               un(i1,i2,i3,hx)=mxdc3d4Hx(i1,i2,i3),\
               un(i1,i2,i3,hy)=mxdc3d4Hy(i1,i2,i3),\
               un(i1,i2,i3,hz)=mxdc3d4Hz(i1,i2,i3),,,,,,)
         else
!         ! 4th order modified equation and dissipation in one loop
          loopsF3D(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
               un(i1,i2,i3,ex)=mxdc3d4Ex(i1,i2,i3)+adcdt*fd43d(i1,i2,i3,ex),\
               un(i1,i2,i3,ey)=mxdc3d4Ey(i1,i2,i3)+adcdt*fd43d(i1,i2,i3,ey),\
               un(i1,i2,i3,ez)=mxdc3d4Ez(i1,i2,i3)+adcdt*fd43d(i1,i2,i3,ez),,,,,,,\
               dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
               un(i1,i2,i3,hx)=mxdc3d4Hx(i1,i2,i3)+adcdt*fd43d(i1,i2,i3,hx),\
               un(i1,i2,i3,hy)=mxdc3d4Hy(i1,i2,i3)+adcdt*fd43d(i1,i2,i3,hy),\
               un(i1,i2,i3,hz)=mxdc3d4Hz(i1,i2,i3)+adcdt*fd43d(i1,i2,i3,hz),,,,,,)
         end if
       end if

     #End

   else  ! not modified equation

     #If #DIM eq "2"
       ! 4th order in space and 4th order Stoermer
       loopsF2D(f(i1,i2,i3,ex),f(i1,i2,i3,ey),f(i1,i2,i3,hz),\
              lap(ex)=csq*lap2d4(i1,i2,i3,ex),\
              lap(ey)=csq*lap2d4(i1,i2,i3,ey),\
              lap(hz)=csq*lap2d4(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=maxwellr44(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwellr44(i1,i2,i3,ey),\
              un(i1,i2,i3,hz)=maxwellr44(i1,i2,i3,hz),\
              ut3(i1,i2,i3,ex)=lap(ex),\
              ut3(i1,i2,i3,ey)=lap(ey),\
              ut3(i1,i2,i3,hz)=lap(hz))
     #Else
       ! 4th order in space and 4th order Stoermer
       stop 55555
       ! comment this ou to shorten the code
!$$$       loopsF3D(f(i1,i2,i3,ex),f(i1,i2,i3,ey),f(i1,i2,i3,ez),\
!$$$              lap(ex)=csq*lap3d4(i1,i2,i3,ex),\
!$$$              lap(ey)=csq*lap3d4(i1,i2,i3,ey),\
!$$$              lap(ez)=csq*lap3d4(i1,i2,i3,ez),\
!$$$              un(i1,i2,i3,ex)=maxwellr44(i1,i2,i3,ex),\
!$$$              un(i1,i2,i3,ey)=maxwellr44(i1,i2,i3,ey),\
!$$$              un(i1,i2,i3,ez)=maxwellr44(i1,i2,i3,ez),\
!$$$              ut3(i1,i2,i3,ex)=lap(ex),\
!$$$              ut3(i1,i2,i3,ey)=lap(ey),\
!$$$              ut3(i1,i2,i3,ez)=lap(ez),\
!$$$              f(i1,i2,i3,hx),f(i1,i2,i3,hy),f(i1,i2,i3,hz),\
!$$$              lap(hx)=csq*lap3d4(i1,i2,i3,hx),\
!$$$              lap(hy)=csq*lap3d4(i1,i2,i3,hy),\
!$$$              lap(hz)=csq*lap3d4(i1,i2,i3,hz),\
!$$$              un(i1,i2,i3,hx)=maxwellr44(i1,i2,i3,hx),\
!$$$              un(i1,i2,i3,hy)=maxwellr44(i1,i2,i3,hy),\
!$$$              un(i1,i2,i3,hz)=maxwellr44(i1,i2,i3,hz),\
!$$$              ut3(i1,i2,i3,hx)=lap(hx),\
!$$$              ut3(i1,i2,i3,hy)=lap(hy),\
!$$$              ut3(i1,i2,i3,hz)=lap(hz))

     #End
   end if

 #Elif #ORDER eq "6" 
   ! *** else if( orderOfAccuracy.eq.6 .and. orderInTime.eq.6 )then

   ! 6th order in space and 6th order in time:
   ! write(*,*) 'Inside advMaxwell order=6...'
   if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

     #If #DIM eq "2"
       ! 6th order modified equation 


       ! write(*,*) 'advOpt: 2d, rect, modified equation'
       loopsF2D(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=maxwell2dr66me(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwell2dr66me(i1,i2,i3,ey),\
              un(i1,i2,i3,hz)=maxwell2dr66me(i1,i2,i3,hz),,,,,,)

     #Else
       ! 6th order modified equation 
       loopsF3D(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
              un(i1,i2,i3,ex)=maxwell3dr66me(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwell3dr66me(i1,i2,i3,ey),\
              un(i1,i2,i3,ez)=maxwell3dr66me(i1,i2,i3,ez),,,,,,,\
              dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,hx)=maxwell3dr66me(i1,i2,i3,hx),\
              un(i1,i2,i3,hy)=maxwell3dr66me(i1,i2,i3,hy),\
              un(i1,i2,i3,hz)=maxwell3dr66me(i1,i2,i3,hz),,,,,,)

     #End

   else

     #If #DIM eq "2"
       ! 6th order in space and 6th order Stoermer
       loopsF2D(f(i1,i2,i3,ex),f(i1,i2,i3,ey),f(i1,i2,i3,hz),\
              lap(ex)=csq*lap2d6(i1,i2,i3,ex),\
              lap(ey)=csq*lap2d6(i1,i2,i3,ey),\
              lap(hz)=csq*lap2d6(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=maxwellr66(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwellr66(i1,i2,i3,ey),\
              un(i1,i2,i3,hz)=maxwellr66(i1,i2,i3,hz),\
              ut5(i1,i2,i3,ex)=lap(ex),\
              ut5(i1,i2,i3,ey)=lap(ey),\
              ut5(i1,i2,i3,hz)=lap(hz))
     #Else
       ! 6th order in space and 6th order Stoermer
       loopsF3D(f(i1,i2,i3,ex),f(i1,i2,i3,ey),f(i1,i2,i3,ez),\
              lap(ex)=csq*lap3d6(i1,i2,i3,ex),\
              lap(ey)=csq*lap3d6(i1,i2,i3,ey),\
              lap(ez)=csq*lap3d6(i1,i2,i3,ez),\
              un(i1,i2,i3,ex)=maxwellr66(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwellr66(i1,i2,i3,ey),\
              un(i1,i2,i3,ez)=maxwellr66(i1,i2,i3,ez),\
              ut5(i1,i2,i3,ex)=lap(ex),\
              ut5(i1,i2,i3,ey)=lap(ey),\
              ut5(i1,i2,i3,ez)=lap(ez),\
              f(i1,i2,i3,hx),f(i1,i2,i3,hy),f(i1,i2,i3,hz),\
              lap(hx)=csq*lap3d6(i1,i2,i3,hx),\
              lap(hy)=csq*lap3d6(i1,i2,i3,hy),\
              lap(hz)=csq*lap3d6(i1,i2,i3,hz),\
              un(i1,i2,i3,hx)=maxwellr66(i1,i2,i3,hx),\
              un(i1,i2,i3,hy)=maxwellr66(i1,i2,i3,hy),\
              un(i1,i2,i3,hz)=maxwellr66(i1,i2,i3,hz),\
              ut5(i1,i2,i3,hx)=lap(hx),\
              ut5(i1,i2,i3,hy)=lap(hy),\
              ut5(i1,i2,i3,hz)=lap(hz))
     #End
   end if

 #Elif #ORDER eq "8"
   ! *** else if( orderOfAccuracy.eq.8 .and. orderInTime.eq.8 )then
   
   ! 8th order in space and 8th order in time:
   ! write(*,*) 'Inside advMaxwell order=8...'
   if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

     #If #DIM eq "2"
       ! 8th order modified equation 


       ! write(*,*) 'advOpt: 2d, rect, modified equation'
       loopsF2D(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=maxwell2dr88me(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwell2dr88me(i1,i2,i3,ey),\
              un(i1,i2,i3,hz)=maxwell2dr88me(i1,i2,i3,hz),,,,,,)

     #Else
       ! 8th order modified equation 
       loopsF3D(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
              un(i1,i2,i3,ex)=maxwell3dr88me(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwell3dr88me(i1,i2,i3,ey),\
              un(i1,i2,i3,ez)=maxwell3dr88me(i1,i2,i3,ez),,,,,,,\
              dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,hx)=maxwell3dr88me(i1,i2,i3,hx),\
              un(i1,i2,i3,hy)=maxwell3dr88me(i1,i2,i3,hy),\
              un(i1,i2,i3,hz)=maxwell3dr88me(i1,i2,i3,hz),,,,,,)

     #End

   else

     #If #DIM eq "2"
       ! 8th order in space and 8th order Stoermer
       loopsF2D(f(i1,i2,i3,ex),f(i1,i2,i3,ey),f(i1,i2,i3,hz),\
              lap(ex)=csq*lap2d8(i1,i2,i3,ex),\
              lap(ey)=csq*lap2d8(i1,i2,i3,ey),\
              lap(hz)=csq*lap2d8(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=maxwellr88(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwellr88(i1,i2,i3,ey),\
              un(i1,i2,i3,hz)=maxwellr88(i1,i2,i3,hz),\
              ut7(i1,i2,i3,ex)=lap(ex),\
              ut7(i1,i2,i3,ey)=lap(ey),\
              ut7(i1,i2,i3,hz)=lap(hz))
     #Else
       ! 8th order in space and 8th order Stoermer
       loopsF3D(f(i1,i2,i3,ex),f(i1,i2,i3,ey),f(i1,i2,i3,ez),\
              lap(ex)=csq*lap3d8(i1,i2,i3,ex),\
              lap(ey)=csq*lap3d8(i1,i2,i3,ey),\
              lap(ez)=csq*lap3d8(i1,i2,i3,ez),\
              un(i1,i2,i3,ex)=maxwellr88(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwellr88(i1,i2,i3,ey),\
              un(i1,i2,i3,ez)=maxwellr88(i1,i2,i3,ez),\
              ut7(i1,i2,i3,ex)=lap(ex),\
              ut7(i1,i2,i3,ey)=lap(ey),\
              ut7(i1,i2,i3,ez)=lap(ez),\
              f(i1,i2,i3,hx),f(i1,i2,i3,hy),f(i1,i2,i3,hz),\
              lap(hx)=csq*lap3d8(i1,i2,i3,hx),\
              lap(hy)=csq*lap3d8(i1,i2,i3,hy),\
              lap(hz)=csq*lap3d8(i1,i2,i3,hz),\
              un(i1,i2,i3,hx)=maxwellr88(i1,i2,i3,hx),\
              un(i1,i2,i3,hy)=maxwellr88(i1,i2,i3,hy),\
              un(i1,i2,i3,hz)=maxwellr88(i1,i2,i3,hz),\
              ut7(i1,i2,i3,hx)=lap(hx),\
              ut7(i1,i2,i3,hy)=lap(hy),\
              ut7(i1,i2,i3,hz)=lap(hz))
     #End
   end if

 #Else
   write(*,*) 'advMaxwell:ERROR orderOfAccuracy,orderInTime=',orderOfAccuracy,orderInTime
   stop 1

 #End

 #End

 else               

 #If #GRIDTYPE eq "curvilinear"

!       **********************************************
!       *************** curvilinear ******************
!       **********************************************

   if( useCurvilinearOpt.eq.1 .and. useConservative.eq.0 )then

    ! ****************************************************************************
    ! *************** OPTIMIZED-CURVILINEAR AND NON-CONSERVATIVE *****************    
    ! ****************************************************************************

    #If #ORDER eq "2" 

      ! --- Todo: non-conservative operators could be inlined here ---
      !   -- these might be faster than precomputing 

!$$$     loopsFCD(un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex),\
!$$$              un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey),\
!$$$              un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez),\
!$$$               ,,,\
!$$$              un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx),\
!$$$              un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy),\
!$$$              un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz),\
!$$$              ,,)

     stop 88044

   #Elif #ORDER eq "4"
     
     if( useSosupDissipation.ne.0 )then

       ! ---- use sosup dissipation (wider stencil) ---

      write(*,'(" finish me: FD44 non-cons && useSosupDissipation")')
      stop 4486


       if( useNewForcingMethod.ne.0 )then
        write(*,'(" finish me: dispersion && useNewForcingMethod")')
        stop 4487
       end if

     else if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

       ! ----------------------------------------------------------
       ! ----- 4th order in space and 4th order in time ------------
       ! ---- Modified equation, NON-CONSERVATIVE difference ----
       ! ----------------------------------------------------------


       !   cdtsq*uLaplacian42(i1,i2,i3,n)+cdtsq12*uLapSq(n)
       ! write(*,*) 'advOpt: 2d, curv, FULL modified equation'

      #If #DIM eq "2"

!$$$ loopsFCD2D($$getLapSq2dOrder2(),\
!$$$                  dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
!$$$                  un(i1,i2,i3,ex)=max2dc44me(i1,i2,i3,ex),\
!$$$                  un(i1,i2,i3,ey)=max2dc44me(i1,i2,i3,ey),\
!$$$                  un(i1,i2,i3,hz)=max2dc44me(i1,i2,i3,hz))

       ! do one at a time -- is this faster ? NO

!$$$ loopsFCD2DA($$evalLapSq2dOrder2(ex),\
!$$$             dtsq*f(i1,i2,i3,ex),\
!$$$             un(i1,i2,i3,ex)=max2dc44me(i1,i2,i3,ex))
!$$$
!$$$ loopsFCD2DA($$evalLapSq2dOrder2(ey),\
!$$$             dtsq*f(i1,i2,i3,ey),\
!$$$             un(i1,i2,i3,ey)=max2dc44me(i1,i2,i3,ey))
!$$$
!$$$ loopsFCD2DA($$evalLapSq2dOrder2(hz),\
!$$$             dtsq*f(i1,i2,i3,hz),\
!$$$             un(i1,i2,i3,hz)=max2dc44me(i1,i2,i3,hz))


        ! first evaluate Laplacian to 2nd-order
       ! *** need to evaluate on one additional line ***
       n1a=n1a-1
       n1b=n1b+1
       n2a=n2a-1
       n2b=n2b+1
       ! **** for this first loop we cannot use the mask -- 
       useWhereMaskSave=useWhereMask
       useWhereMask=0
       loopse9(v(i1,i2,i3,ex)=uLaplacian22(i1,i2,i3,ex),\
               v(i1,i2,i3,ey)=uLaplacian22(i1,i2,i3,ey),\
               v(i1,i2,i3,hz)=uLaplacian22(i1,i2,i3,hz),,,,,,)        

       ! write(*,*) 'advOpt: 2d, rect, modified equation'
       n1a=n1a+1
       n1b=n1b-1
       n2a=n2a+1
       n2b=n2b-1
       useWhereMask=useWhereMaskSave
       
       if( useDivergenceCleaning.eq.0 )then
        if( useNewForcingMethod.eq.1 ) then
          ! fix forcing for ME scheme to be 4th-order
          loopsF2DD(dtsq*f2dcme44(i1,i2,i3,ex),dtsq*f2dcme44(i1,i2,i3,ey),dtsq*f2dcme44(i1,i2,i3,hz),\
                un(i1,i2,i3,ex)=max2dc44me2(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=max2dc44me2(i1,i2,i3,ey),\
                un(i1,i2,i3,hz)=max2dc44me2(i1,i2,i3,hz),,,,,,)
        else
          loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
                un(i1,i2,i3,ex)=max2dc44me2(i1,i2,i3,ex),\
                un(i1,i2,i3,ey)=max2dc44me2(i1,i2,i3,ey),\
                un(i1,i2,i3,hz)=max2dc44me2(i1,i2,i3,hz),,,,,,)
        end if
       else
        if( useNewForcingMethod.eq.1 ) then
          write(*,'(" advOpt: FINISH ME")')
          stop 10134
        end if
        loopsF2DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,ex)=mxdc2d4cEx(i1,i2,i3),\
              un(i1,i2,i3,ey)=mxdc2d4cEy(i1,i2,i3),\
              un(i1,i2,i3,hz)=max2dc44me2(i1,i2,i3,hz),,,,,,)
       end if


      #Elif #DIM == "3"

       ! *** need to evaluate on one additional line ***
       n1a=n1a-1
       n1b=n1b+1
       n2a=n2a-1
       n2b=n2b+1
       n3a=n3a-1
       n3b=n3b+1
       ! **** for this first loop we cannot use the mask -- 
       useWhereMaskSave=useWhereMask
       useWhereMask=0
       if( solveForE.ne.0 .and. solveForH.ne.0 )then
         stop 6666
!$$$        loopse9(v(i1,i2,i3,ex)=uLaplacian23(i1,i2,i3,ex),\
!$$$                v(i1,i2,i3,ey)=uLaplacian23(i1,i2,i3,ey),\
!$$$                v(i1,i2,i3,ez)=uLaplacian23(i1,i2,i3,ez),\
!$$$                v(i1,i2,i3,hx)=uLaplacian23(i1,i2,i3,hx),\
!$$$                v(i1,i2,i3,hy)=uLaplacian23(i1,i2,i3,hy),\
!$$$                v(i1,i2,i3,hz)=uLaplacian23(i1,i2,i3,hz),,,)
       else if( solveForE.ne.0 )then
        loopse9(v(i1,i2,i3,ex)=uLaplacian23(i1,i2,i3,ex),\
                v(i1,i2,i3,ey)=uLaplacian23(i1,i2,i3,ey),\
                v(i1,i2,i3,ez)=uLaplacian23(i1,i2,i3,ez),,,,,,)        
       else
!$$$        loopse9(v(i1,i2,i3,hx)=uLaplacian23(i1,i2,i3,hx),\
!$$$                v(i1,i2,i3,hy)=uLaplacian23(i1,i2,i3,hy),\
!$$$                v(i1,i2,i3,hz)=uLaplacian23(i1,i2,i3,hz),,,,,,)        
       end if

       ! write(*,*) 'advOpt: 2d, rect, modified equation'
       n1a=n1a+1
       n1b=n1b-1
       n2a=n2a+1
       n2b=n2b-1
       n3a=n3a+1
       n3b=n3b-1
       useWhereMask=useWhereMaskSave
       ! 4th order modified equation 

       if( useDivergenceCleaning.eq.0 )then
         loopsF3DD(dtsq*f(i1,i2,i3,ex),dtsq*f(i1,i2,i3,ey),dtsq*f(i1,i2,i3,ez),\
              un(i1,i2,i3,ex)=max3dc44me(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=max3dc44me(i1,i2,i3,ey),\
              un(i1,i2,i3,ez)=max3dc44me(i1,i2,i3,ez),,,,,,,\
              dtsq*f(i1,i2,i3,hx),dtsq*f(i1,i2,i3,hy),dtsq*f(i1,i2,i3,hz),\
              un(i1,i2,i3,hx)=max3dc44me(i1,i2,i3,hx),\
              un(i1,i2,i3,hy)=max3dc44me(i1,i2,i3,hy),\
              un(i1,i2,i3,hz)=max3dc44me(i1,i2,i3,hz),,,,,,)
       else
         ! finish me
         stop 1005
       end if
      #End

     else
 
      stop 22743

     end if

   #Else
     write(*,'("MX: advOpt: curv-grid order=ORDER not implemeted")')
     stop 11155
   #End


   else if( useCurvilinearOpt.eq.1 .and. useConservative.eq.1 )then

    ! *************** conservative *****************    

    stop 94422



   else

     ! **********************************************************************************
     ! **************** USE PRE-COMPUTED SPATIAL OPERATORS ******************************
     ! **********************************************************************************

     !  --> The Laplacian and Laplacian squared have already been computed by the calling program 
     !  --> For example, mainly when using conservative operators


   #If #ORDER eq "2" 

    if( useSosupDissipation.ne.0 )then

      ! ---- use sosup dissipation (wider stencil) ---

      if( t.le.2.*dt )then
        write(*,'(" advOpt: FD22 + sosup-dissipation for curvilinear")')
      end if 

      if( useNewForcingMethod.ne.0 )then
       write(*,'(" finish me: useSosupDissipation && useNewForcingMethod")')
       stop 7739
      end if 

      ! FD22 (curvilinear grid) with Sosup (wide stencil dissiption)
      if( sosupDissipationOption.eq.0 )then
       ! advance + sosup dissipation: 
       ! note: forcing is already added to the rhs.
       beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
        getSosupDissipationCoeff2d(adxSosup)
        do m=0,2 ! ex, ey, hz
          ec=ex+m
          un(i1,i2,i3,ec)=maxwellc22(i1,i2,i3,ec)+adSosupCurv4(i1,i2,i3,ec)
        end do
       endLoopsMask()

      else if( sosupDissipationOption.eq.1 )then
       ! --- TWO STAGES ---

       if( t.le.2.*dt )then
         write(*,'(" advOpt: FD22 + sosup-dissipation for curvilinear 2 STAGE")')
       end if 

       ! apply sosup dissipation to time n-1 using times n-1 and n-3
       ! assume un holds u(t-2*dt) on input 
       if( updateDissipation.eq.1 )then
        beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
         getSosupDissipationCoeff2d(adxSosup)
         do m=0,2 ! ex, ey, hz
           ec=ex+m
           u(i1,i2,i3,ec)=sosupDiss2d4c(i1,i2,i3,ec)
         end do
        endLoopsMask()
       end if
       ! advance to time n+1
       ! note: forcing is already added to the rhs.
       if( updateSolution.eq.1 )then
        beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
         do m=0,2 ! ex, ey, hz
           ec=ex+m
           un(i1,i2,i3,ec)=maxwellc22(i1,i2,i3,ec)
         end do
        endLoopsMask()
       end if
      else
        write(*,'("advOpt: ERROR: unexpected sosupDissipationOption=",i2)') sosupDissipationOption
        stop 2020
      end if


    else if( dispersionModel.ne.noDispersion )then

      ! --dispersive model --

      write(*,'("--advOpt-- advance 2D curvilinear: dispersive model")') 
      if( addDissipation )then
        write(*,'(" -- finish me : dispersion and AD")')
        stop 8256
      end if
      if( useNewForcingMethod.ne.0 )then
       write(*,'(" finish me: dispersion && useNewForcingMethod")')
       stop 7733
      end if 

      fp=0.

      fe=0.
      beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
 
        ! scheme from Jeff: 

        ! Advance Hz first:
        ! For now solve H_t = -(1/mu)*(  (E_y)_x - (E_x)_y )
        !   USE AB2 -- note: this is just a quadrature so stability is not an inssue
        un(i1,i2,i3,hz) = u(i1,i2,i3,hz) -(dt/mu)*( 1.5*ux22(i1,i2,i3,ey) -.5*umx22(i1,i2,i3,ey) \
                                                   -1.5*uy22(i1,i2,i3,ex) +.5*umy22(i1,i2,i3,ex) )
        if( addForcing.ne.0 )then
          un(i1,i2,i3,hz) = un(i1,i2,i3,hz) + dt*f(i1,i2,i3,hz) ! first order only **FIX ME**
        end if        

        !  --- advance E and P ---
        do m=0,1
         pc=pxc+m
         ec=ex+m

         if( addForcing.ne.0 )then ! forcing in E equation already added to f 
           fp = dtsq*f(i1,i2,i3,pc) 
         end if 
         un(i1,i2,i3,pc)=( 2.*u(i1,i2,i3,pc)- (1.-gammaDt*.5)*um(i1,i2,i3,pc) + omegapDtSq*u(i1,i2,i3,ec) + fp )/(1.+gammaDt*.5)
         ptt = un(i1,i2,i3,pc)-2.*u(i1,i2,i3,pc)+um(i1,i2,i3,pc)
         ! write(*,'(" ptt=",e10.2)') ptt
         un(i1,i2,i3,ec)=maxwellc22(i1,i2,i3,ec) - ptt/eps
         ! test: un(i1,i2,i3,ec)=maxwellc22(i1,i2,i3,ec) 

        end do

      endLoopsMask()

    else if( useDivergenceCleaning.eq.0 )then

     ! --- currently 2nd-order conservative and non-conservative opertaors are done here ---
     ! --- non-dispersive ---

     loopsFCD(un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex),\
              un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey),\
              un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez),\
              ,,,\
              un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx),\
              un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy),\
              un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz),\
              ,,)
    else
       ! 2D, 2nd-order, curvilinear, div cleaning:
       !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )
     #If #DIM eq "2"
      write(*,'("advMaxwell: 2D, 2nd-order, curv, div cleaning... t=",e10.2,", adcdt=",e10.2 )') t,adcdt
      loopsFCD(un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3),\
               un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3),\
               un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez),\
               ,,,\
               un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx),\
               un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy),\
               un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz),\
               ,,)

     #Else
      write(*,'("advMaxwell: 3D, 2nd-order, curv, div cleaning... t=",e10.2,", adcdt=",e10.2 )') t,adcdt
      loopsFCD(un(i1,i2,i3,ex)=mxdc3d2cEx(i1,i2,i3),\
               un(i1,i2,i3,ey)=mxdc3d2cEy(i1,i2,i3),\
               un(i1,i2,i3,ez)=mxdc3d2cEz(i1,i2,i3),\
               ,,,\
               un(i1,i2,i3,hx)=mxdc3d2cHx(i1,i2,i3),\
               un(i1,i2,i3,hy)=mxdc3d2cHy(i1,i2,i3),\
               un(i1,i2,i3,hz)=mxdc3d2cHz(i1,i2,i3),\
               ,,)
     #End

    end if

   #Elif #ORDER eq "4"
     
     if( useSosupDissipation.ne.0 )then

       ! ---- use sosup dissipation (wider stencil) ---

       if( t.le.2.*dt )then
         write(*,'(" advOpt: FD44 + sosup-dissipation for curvilinear")')
       end if 

       if( useNewForcingMethod.ne.0 )then
        write(*,'(" finish me: FD44 + sosup-dissipation && useNewForcingMethod")')
        stop 4487
       end if

      ! FD44 (curvilinear grid) with Sosup (wide stencil dissiption)
      if( sosupDissipationOption.eq.0 )then
       ! advance + sosup dissipation: 
       ! note: forcing is already added to the rhs.
       ! note: forcing is already added to the rhs.
       beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
        getSosupDissipationCoeff2d(adxSosup)
        do m=0,2 ! ex, ey, hz
          ec=ex+m
          un(i1,i2,i3,ec)=maxwellc44me(i1,i2,i3,ec)+adSosupCurv6(i1,i2,i3,ec)
        end do
       endLoopsMask()

      else if( sosupDissipationOption.eq.1 )then
       ! --- TWO STAGES ---

       ! apply sosup dissipation to time n-1 using times n-1 and n-3
       ! assume un holds u(t-2*dt) on input 
       if( updateDissipation.eq.1 )then
        beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
         getSosupDissipationCoeff2d(adxSosup)
         do m=0,2 ! ex, ey, hz
           ec=ex+m
           u(i1,i2,i3,ec)=sosupDiss2d6c(i1,i2,i3,ec)
         end do
        endLoopsMask()       
       end if
       ! advance to time n+1
       ! note: forcing is already added to the rhs.
       if( updateSolution.eq.1 )then
        beginLoopsMask(i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b)
         do m=0,2 ! ex, ey, hz
           ec=ex+m
           un(i1,i2,i3,ec)=maxwellc44me(i1,i2,i3,ec)
         end do
        endLoopsMask()
       end if

      else
        write(*,'("advOpt: ERROR: unexpected sosupDissipationOption=",i2)') sosupDissipationOption
        stop 2020
      end if

     else if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

       ! ******* 4th order in space and 4th order in time ********
       ! **************** CURVILINEAR *****************************
       ! **************** CONSERVATIVE DIFFERENCE *****************
  
       ! write(*,*) 'advOpt: 2d, curv, modified equation'

       if( useDivergenceCleaning.eq.0 )then
        if( useNewForcingMethod.eq.1 ) then

         if( addForcing.eq.0 )then
          ! NOTE: in 2D the ez,hx and hy entries are ignored
          loopsFCD(un(i1,i2,i3,ex)=maxwellc44me(i1,i2,i3,ex),\
                   un(i1,i2,i3,ey)=maxwellc44me(i1,i2,i3,ey),\
                   un(i1,i2,i3,ez)=maxwellc44me(i1,i2,i3,ez),,,,\
                   un(i1,i2,i3,hx)=maxwellc44me(i1,i2,i3,hx),\
                   un(i1,i2,i3,hy)=maxwellc44me(i1,i2,i3,hy),\
                   un(i1,i2,i3,hz)=maxwellc44me(i1,i2,i3,hz),,,)
         else
          ! Add forcing to ME scheme 
          if( nd.eq.2 )then
           loopsFCD2DF(un(i1,i2,i3,ex)=maxwellc44me(i1,i2,i3,ex)+dtsq*f2dcme44(i1,i2,i3,ex),\
                       un(i1,i2,i3,ey)=maxwellc44me(i1,i2,i3,ey)+dtsq*f2dcme44(i1,i2,i3,ey),\
                       un(i1,i2,i3,hz)=maxwellc44me(i1,i2,i3,hz)+dtsq*f2dcme44(i1,i2,i3,hz))
          else
           loopsFCD3DF(un(i1,i2,i3,ex)=maxwellc44me(i1,i2,i3,ex)+dtsq*f3dcme44(i1,i2,i3,ex),\
                       un(i1,i2,i3,ey)=maxwellc44me(i1,i2,i3,ey)+dtsq*f3dcme44(i1,i2,i3,ey),\
                       un(i1,i2,i3,ez)=maxwellc44me(i1,i2,i3,ez)+dtsq*f3dcme44(i1,i2,i3,ez),,,,\
                       un(i1,i2,i3,hx)=maxwellc44me(i1,i2,i3,hx)+dtsq*f3dcme44(i1,i2,i3,hx),\
                       un(i1,i2,i3,hy)=maxwellc44me(i1,i2,i3,hy)+dtsq*f3dcme44(i1,i2,i3,hy),\
                       un(i1,i2,i3,hz)=maxwellc44me(i1,i2,i3,hz)+dtsq*f3dcme44(i1,i2,i3,hz),,,)
          end if
         
         end if
        else
         loopsFCD(un(i1,i2,i3,ex)=maxwellc44me(i1,i2,i3,ex),\
                  un(i1,i2,i3,ey)=maxwellc44me(i1,i2,i3,ey),\
                  un(i1,i2,i3,ez)=maxwellc44me(i1,i2,i3,ez),,,,\
                  un(i1,i2,i3,hx)=maxwellc44me(i1,i2,i3,hx),\
                  un(i1,i2,i3,hy)=maxwellc44me(i1,i2,i3,hy),\
                  un(i1,i2,i3,hz)=maxwellc44me(i1,i2,i3,hz),,,)
        end if       
       else
        ! 2D, 4th-order, curvilinear, div cleaning:
        write(*,'("advMaxwell: 2D, 4th-order, curv, cons, div cleaning... t=",e10.2 )') t

        if( useNewForcingMethod.eq.1 ) then
          write(*,'(" advOpt: FINISH ME")')
          stop 10138
        end if

        #If #DIM eq "2"
         loopsFCD(un(i1,i2,i3,ex)=mxdc2d4cConsEx(i1,i2,i3),\
                  un(i1,i2,i3,ey)=mxdc2d4cConsEy(i1,i2,i3),\
                  un(i1,i2,i3,ez)=maxwellc44me(i1,i2,i3,ez),,,,\
                  un(i1,i2,i3,hx)=maxwellc44me(i1,i2,i3,hx),\
                  un(i1,i2,i3,hy)=maxwellc44me(i1,i2,i3,hy),\
                  un(i1,i2,i3,hz)=maxwellc44me(i1,i2,i3,hz),,,)
        #Else
          stop 4481
        #End
       end if
     else
       ! write(*,*) 'Inside advMaxwell curv, order=4...'

       ! 4th order in space and 4th order Stoermer
       loopsFCD(un(i1,i2,i3,ex)=maxwellc44(i1,i2,i3,ex),\
             un(i1,i2,i3,ey)=maxwellc44(i1,i2,i3,ey),\
             un(i1,i2,i3,ez)=maxwellc44(i1,i2,i3,ez),\
             ut3(i1,i2,i3,ex)=f(i1,i2,i3,ex),\
             ut3(i1,i2,i3,ey)=f(i1,i2,i3,ey),\
             ut3(i1,i2,i3,ez)=f(i1,i2,i3,ez),\
             un(i1,i2,i3,hx)=maxwellc44(i1,i2,i3,hx),\
             un(i1,i2,i3,hy)=maxwellc44(i1,i2,i3,hy),\
             un(i1,i2,i3,hz)=maxwellc44(i1,i2,i3,hz),\
             ut3(i1,i2,i3,hx)=f(i1,i2,i3,hx),\
             ut3(i1,i2,i3,hy)=f(i1,i2,i3,hy),\
             ut3(i1,i2,i3,hz)=f(i1,i2,i3,hz))

     end if

   #Elif #ORDER eq "6"

     ! 6th order in space and 6th order in time:
     ! write(*,*) 'Inside advMaxwell order=6...'

     if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

       if( orderInTime.ne.2 )then
          write(*,'("MX: advOpt:ERROR curv-grid conservative orderInTime.ne.2")')
          stop 77155
       end if


       loopsFCD(un(i1,i2,i3,ex)=maxwellc66me(i1,i2,i3,ex),\
               un(i1,i2,i3,ey)=maxwellc66me(i1,i2,i3,ey),\
               un(i1,i2,i3,ez)=maxwellc66me(i1,i2,i3,ez),,,,\
               un(i1,i2,i3,hx)=maxwellc66me(i1,i2,i3,hx),\
               un(i1,i2,i3,hy)=maxwellc66me(i1,i2,i3,hy),\
               un(i1,i2,i3,hz)=maxwellc66me(i1,i2,i3,hz),,,)

     else
       ! 6th order in space and 6th order Stoermer
       loopsFCD(un(i1,i2,i3,ex)=maxwellc66(i1,i2,i3,ex),\
             un(i1,i2,i3,ey)=maxwellc66(i1,i2,i3,ey),\
             un(i1,i2,i3,ez)=maxwellc66(i1,i2,i3,ez),\
             ut5(i1,i2,i3,ex)=f(i1,i2,i3,ex),\
             ut5(i1,i2,i3,ey)=f(i1,i2,i3,ey),\
             ut5(i1,i2,i3,ez)=f(i1,i2,i3,ez),\
             un(i1,i2,i3,hx)=maxwellc66(i1,i2,i3,hx),\
             un(i1,i2,i3,hy)=maxwellc66(i1,i2,i3,hy),\
             un(i1,i2,i3,hz)=maxwellc66(i1,i2,i3,hz),\
             ut5(i1,i2,i3,hx)=f(i1,i2,i3,hx),\
             ut5(i1,i2,i3,hy)=f(i1,i2,i3,hy),\
             ut5(i1,i2,i3,hz)=f(i1,i2,i3,hz))

     end if

   #Elif #ORDER eq "8"
     
     ! 8th order in space and 8th order in time:
     ! write(*,*) 'Inside advMaxwell order=8...'

     if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

       ! ** for now we just do 2nd-order in time **
       if( orderInTime.ne.2 )then
          stop 88188
       end if


       loopsFCD(un(i1,i2,i3,ex)=maxwellc88me(i1,i2,i3,ex),\
               un(i1,i2,i3,ey)=maxwellc88me(i1,i2,i3,ey),\
               un(i1,i2,i3,ez)=maxwellc88me(i1,i2,i3,ez),,,,\
               un(i1,i2,i3,hx)=maxwellc88me(i1,i2,i3,hx),\
               un(i1,i2,i3,hy)=maxwellc88me(i1,i2,i3,hy),\
               un(i1,i2,i3,hz)=maxwellc88me(i1,i2,i3,hz),,,)

     else
       loopsFCD(un(i1,i2,i3,ex)=maxwellc88(i1,i2,i3,ex),\
             un(i1,i2,i3,ey)=maxwellc88(i1,i2,i3,ey),\
             un(i1,i2,i3,ez)=maxwellc88(i1,i2,i3,ez),\
             ut7(i1,i2,i3,ex)=f(i1,i2,i3,ex),\
             ut7(i1,i2,i3,ey)=f(i1,i2,i3,ey),\
             ut7(i1,i2,i3,ez)=f(i1,i2,i3,ez),\
             un(i1,i2,i3,hx)=maxwellc88(i1,i2,i3,hx),\
             un(i1,i2,i3,hy)=maxwellc88(i1,i2,i3,hy),\
             un(i1,i2,i3,hz)=maxwellc88(i1,i2,i3,hz),\
             ut7(i1,i2,i3,hx)=f(i1,i2,i3,hx),\
             ut7(i1,i2,i3,hy)=f(i1,i2,i3,hy),\
             ut7(i1,i2,i3,hz)=f(i1,i2,i3,hz))

     end if
  #Else
     write(*,*) 'advMaxwell:ERROR orderOfAccuracy,orderInTime=',orderOfAccuracy,orderInTime
     stop 2
  #End

  end if

 #End
 end if

 return
 end

#endMacro


 
#beginMacro buildFile(NAME,DIM,ORDER,GRIDTYPE)
#beginFile NAME.f
 ADV_MAXWELL(NAME,DIM,ORDER,GRIDTYPE)
#endFile
#endMacro

      buildFile(advMx2dOrder2r,2,2,rectangular)

! -- Most of these are built with advOpt.bf

!      buildFile(advMx3dOrder2r,3,2,rectangular)
!
      buildFile(advMx2dOrder2c,2,2,curvilinear)
!      buildFile(advMx3dOrder2c,3,2,curvilinear)
!
      buildFile(advMx2dOrder4r,2,4,rectangular)
!      buildFile(advMx3dOrder4r,3,4,rectangular)
!
      buildFile(advMx2dOrder4c,2,4,curvilinear)
!      buildFile(advMx3dOrder4c,3,4,curvilinear)
!
!      buildFile(advMx2dOrder6r,2,6,rectangular)
!      buildFile(advMx3dOrder6r,3,6,rectangular)
!
!       ! build these for testing symmetric operators -- BC's not implemented yet
!      buildFile(advMx2dOrder6c,2,6,curvilinear)
!      buildFile(advMx3dOrder6c,3,6,curvilinear)
!
!      buildFile(advMx2dOrder8r,2,8,rectangular)
!      buildFile(advMx3dOrder8r,3,8,rectangular)
!
!       ! build these for testing symmetric operators -- BC's not implemented yet
!      buildFile(advMx2dOrder8c,2,8,curvilinear)
!      buildFile(advMx3dOrder8c,3,8,curvilinear)



! build an empty version of high order files so we do not have to compile the full version
#beginMacro ADV_MAXWELL_NULL(NAME,DIM,ORDER,GRIDTYPE)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                 mask,rsxy,  um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
!======================================================================
!   Advance a time step for Maxwells eqution
!     OPTIMIZED version for rectangular grids.
! nd : number of space dimensions
!
! ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
!                           =1 - AD only
!
!  dis(i1,i2,i3) : temp space to hold artificial dissipation
!  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
!======================================================================
  write(*,'("ERROR: null version of NAME called")')
  stop 9922
  return
  end
#endMacro  


#beginMacro buildFileNull(NAME,DIM,ORDER,GRIDTYPE)
#beginFile NAME ## Null.f
 ADV_MAXWELL_NULL(NAME,DIM,ORDER,GRIDTYPE)
#endFile
#endMacro

!      buildFileNull(advMx2dOrder6r,2,6,rectangular)
!      buildFileNull(advMx3dOrder6r,3,6,rectangular)
!
!      buildFileNull(advMx2dOrder6c,2,6,curvilinear)
!      buildFileNull(advMx3dOrder6c,3,6,curvilinear)
!
!      buildFileNull(advMx2dOrder8r,2,8,rectangular)
!      buildFileNull(advMx3dOrder8r,3,8,rectangular)
!
!      buildFileNull(advMx2dOrder8c,2,8,curvilinear)
!      buildFileNull(advMx3dOrder8c,3,8,curvilinear)



      subroutine advMaxwell(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                            mask,rx,  um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
!======================================================================
!   Advance a time step for Maxwells eqution
!     OPTIMIZED version for rectangular grids.
! nd : number of space dimensions
!
! ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
!                           =1 - AD only
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real fa(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b,0:*)  ! forcings at different times
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real vvt2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real vvt4(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut5(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut6(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut7(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)
      
!     ---- local variables -----
      integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime
      integer addForcing,orderOfDissipation,option
      integer useWhereMask,solveForE,solveForH,grid
      integer ex,ey,ez, hx,hy,hz

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
!...........end   statement functions


      ! write(*,*) 'Inside advMaxwell...'

      orderOfAccuracy    =ipar(2)
      gridType           =ipar(1)

      if( orderOfAccuracy.eq.2 )then

        if( nd.eq.2 .and. gridType.eq.rectangular ) then
          call advMx2dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.2 .and. gridType.eq.curvilinear ) then
          call advMx2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.rectangular ) then
          call advMx3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                             mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.curvilinear ) then
          call advMx3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                             mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else
          stop 2271
        end if

      else if( orderOfAccuracy.eq.4 ) then
        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advMx2dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advMx2dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advMx3dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advMx3dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

!
      else if( orderOfAccuracy.eq.6 ) then
        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advMx2dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advMx2dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advMx3dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advMx3dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

      else if( orderOfAccuracy.eq.8 ) then

        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advMx2dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advMx2dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advMx3dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advMx3dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx, um,u,un,f,fa, v,vvt2,ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

      else
        write(*,'(" advMaxwell:ERROR: un-implemented order of accuracy =",i6)') orderOfAccuracy
          ! '
        stop 11122
      end if

      return
      end








