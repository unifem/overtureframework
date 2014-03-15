#beginMacro initFactorOrSolve(OPT,AXIS)
#If #AXIS == "0"
 nka=n3a
 nkb=n3b
 nkc=n3c
 nja=n2a
 njb=n2b
 njc=n2c
 nia=n1a
 nib=n1b
 #defineMacro A(i) a(i,j,k)
 #defineMacro B(i) b(i,j,k)
 #defineMacro C(i) c(i,j,k)
 #defineMacro D(i) d(i,j,k)
 #defineMacro E(i) e(i,j,k)
 #defineMacro F(i) f(i,j,k)
 #defineMacro S(i) s(i,j,k)
 #defineMacro T(i) t(i,j,k)
 #defineMacro U(i) u(i,j,k)
 #defineMacro V(i) v(i,j,k)
#Elif #AXIS == "1"
 nka=n3a
 nkb=n3b
 nkc=n3c
 nja=n1a
 njb=n1b
 njc=n1c
 nia=n2a
 nib=n2b
 #defineMacro A(i) a(j,i,k)
 #defineMacro B(i) b(j,i,k)
 #defineMacro C(i) c(j,i,k)
 #defineMacro D(i) d(j,i,k)
 #defineMacro E(i) e(j,i,k)
 #defineMacro F(i) f(j,i,k)
 #defineMacro S(i) s(j,i,k)
 #defineMacro T(i) t(j,i,k)
 #defineMacro U(i) u(j,i,k)
 #defineMacro V(i) v(j,i,k)
#Elif #AXIS == "2"
 nka=n2a
 nkb=n2b
 nkc=n2c
 nja=n1a
 njb=n1b
 njc=n1c
 nia=n3a
 nib=n3b
 #defineMacro A(i) a(j,k,i)
 #defineMacro B(i) b(j,k,i)
 #defineMacro C(i) c(j,k,i)
 #defineMacro D(i) d(j,k,i)
 #defineMacro E(i) e(j,k,i)
 #defineMacro F(i) f(j,k,i)
 #defineMacro S(i) s(j,k,i)
 #defineMacro T(i) t(j,k,i)
 #defineMacro U(i) u(j,k,i)
 #defineMacro V(i) v(j,k,i)
#Else
  stop 7
#End
 
c check the macro args
#If #OPT == "factor"
#Elif #OPT == "solve"
#Else
 stop 8
#End
#endMacro


c *** for axis==0 we put the j,k loops on the outside
#beginMacro startOuterLoop(AXIS)
#If #AXIS == "0"
 do k = nka,nkb,nkc
 do j = nja,njb,njc
#End
#endMacro
#beginMacro endOuterLoop(AXIS)
#If #AXIS == "0"
 end do
 end do
#End
#endMacro


c *** for axis!=0 we put the j,k loops on the inside
#beginMacro startInnerLoop(AXIS)
#If #AXIS != "0"
 do k = nka,nkb,nkc
 do j = nja,njb,njc
#End
#endMacro
#beginMacro endInnerLoop(AXIS)
#If #AXIS != "0"
 end do
 end do
#End
#endMacro



c ****************************************************************
c *************** Normal System **********************************
c ****************************************************************
#beginMacro factorOrSolveNormal(OPT,AXIS)

 initFactorOrSolve(OPT,AXIS)

 if( nib-nia+1.lt.2 )then
   write(*,'(''pentaFactor:ERROR too few points. Need at least 2 points.'')')
   stop 3
 end if

 n=nib

 startOuterLoop(AXIS)
 do i = nia+1,nib-1
  startInnerLoop(AXIS)
    #If #OPT == "factor"
     tmp1 = B(i)/C(i-1)
     B(i)=tmp1  ! save pivots here
     C(i) = C(i) - tmp1 * D(i-1)
     D(i) = D(i) - tmp1 * E(i-1) 
     tmp2 = A(i+1)/C(i-1)
     A(i)=tmp2 
     B(i+1) = B(i+1) - tmp2 * D(i-1)
     C(i+1) = C(i+1) - tmp2 * E(i-1)
    #Else
     F(i) = F(i) - B(i) * F(i-1)
     F(i+1) = F(i+1) - A(i) * F(i-1)
    #End
  endInnerLoop(AXIS)
 end do

 startInnerLoop(AXIS)
  #If #OPT == "factor"
   tmp1 = B(n) / C(n-1)
   B(n)=tmp1
   C(n) = C(n) - tmp1 * D(n-1)
  #Else
   F(n) = F(n) - B(n) * F(n-1)
  #End
 endInnerLoop(AXIS)

 endOuterLoop(AXIS)


 #If #OPT == "solve"
 ! Back Substitution
 startOuterLoop(AXIS)

 startInnerLoop(AXIS)
   F(n) = F(n) / C(n)
   F(n-1) = ( F(n-1) - D(n-1)*F(n)  )/C(n-1)
 endInnerLoop(AXIS)

 do i=nib-2,nia,-1
  startInnerLoop(AXIS)
    F(i) = ( F(i) - D(i)*F(i+1) - E(i)*F(i+2) ) / C(i)
  endInnerLoop(AXIS)
 end do

 endOuterLoop(AXIS)

 #End
#endMacro


c ****************************************************************
c *************** Extended System ********************************
c ****************************************************************
#beginMacro factorOrSolveExtended(OPT,AXIS)

 initFactorOrSolve(OPT,AXIS)

 if( nib-nia+1.lt.6 )then
   write(*,'(''pentaFactor:ERROR too few points. Need at least 6 points.'')')
   stop 3
 end if

 n=nib

c             | c d e a b             |  0 
c             | b c d e a             |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n  
 startOuterLoop(AXIS)

  ! *** the first 2 iterations of the loop below are special cases
  startInnerLoop(AXIS)
  i = nia+1
 #If #OPT == "factor"
   tmp1 = B(i)/C(i-1) ! eliminate b(1)
   B(i)=tmp1         ! save pivots in the element that we eliminate
   C(i) = C(i) - tmp1 * D(i-1)
   D(i) = D(i) - tmp1 * E(i-1) 
   E(i) = E(i) - tmp1 * A(i-1) 
   A(i) = A(i) - tmp1 * B(i-1) 
   
   tmp2 = A(i+1)/C(i-1) ! eliminate a(2)
   A(i+1)=tmp2       ! save pivots in the element that we eliminate 
   B(i+1) = B(i+1) - tmp2 * D(i-1)
   C(i+1) = C(i+1) - tmp2 * E(i-1)
   D(i+1) = D(i+1) - tmp2 * A(i-1)
   E(i+1) = E(i+1) - tmp2 * B(i-1)
 #Else
   F(i) = F(i) - B(i) * F(i-1) 
   F(i+1) = F(i+1) - A(i+1) * F(i-1)
 #End

  i = nia+2
 #If #OPT == "factor"
   tmp1 = B(i)/C(i-1)  ! eliminate b(2)
   B(i)=tmp1         ! save pivots in the element that we eliminate
   C(i) = C(i) - tmp1 * D(i-1)
   D(i) = D(i) - tmp1 * E(i-1) 
   E(i) = E(i) - tmp1 * A(i-1) 
   
   tmp2 = A(i+1)/C(i-1)  ! eliminate a(3)
   A(i+1)=tmp2       ! save pivots in the element that we eliminate 
   B(i+1) = B(i+1) - tmp2 * D(i-1)
   C(i+1) = C(i+1) - tmp2 * E(i-1)
   D(i+1) = D(i+1) - tmp2 * A(i-1)
 #Else
   F(i) = F(i) - B(i) * F(i-1) 
   F(i+1) = F(i+1) - A(i+1) * F(i-1)
 #End
 endInnerLoop(AXIS)

  do i = nia+3,nib-3
   startInnerLoop(AXIS)
    #If #OPT == "factor"
     tmp1 = B(i)/C(i-1)
     B(i)=tmp1                 ! save pivots in the element that we eliminate
     C(i) = C(i) - tmp1 * D(i-1)
     D(i) = D(i) - tmp1 * E(i-1) 

     tmp2 = A(i+1)/C(i-1)
     A(i+1)=tmp2               ! save pivots in the element that we eliminate 
     B(i+1) = B(i+1) - tmp2 * D(i-1)
     C(i+1) = C(i+1) - tmp2 * E(i-1)
    #Else
     F(i) = F(i) - B(i) * F(i-1) 
     F(i+1) = F(i+1) - A(i+1) * F(i-1)
    #End
   endInnerLoop(AXIS)
  end do
      
c         Here is what the matrix looks like now:
c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n

  startInnerLoop(AXIS)
  i=nib-2
  #If #OPT == "factor"
   tmp1=B(i) / C(i-1)  ! eliminate b(n-2)
   B(i)=tmp1
   C(i) = C(i) - tmp1 * D(i-1)
   D(i) = D(i) - tmp1 * E(i-1) 
  #Else
   F(i) = F(i) - B(i) * F(i-1)
  #End

  i=nib-1
  #If #OPT == "factor"
   tmp1=E(i) / C(i-3) ! eliminate e(n-1)
   E(i)=tmp1
   A(i) = A(i) - tmp1 * D(i-3)
   B(i) = B(i) - tmp1 * E(i-3)
  #Else
   F(i) = F(i) - E(i) * F(i-3)
  #End

   i=nib
  #If #OPT == "factor"
   tmp1=D(i) / C(i-4)    ! eliminate d(n)
   D(i)=tmp1
   E(i) = E(i) - tmp1 * D(i-4)
   A(i) = A(i) - tmp1 * E(i-4)
  #Else
   F(i) = F(i) - D(i) * F(i-4)
  #End

c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 0 c d e |  n-2 
c             |             0 a b c d |  n-1 
c             |             0 e a b c |  n

   i=nib-1
  #If #OPT == "factor"
   tmp1=A(i) / C(i-2)  ! eliminate a(n-1)
   A(i)=tmp1
   B(i) = B(i) - tmp1 * D(i-2)
   C(i) = C(i) - tmp1 * E(i-2)

   tmp1=B(i) / C(i-1)  ! eliminate b(n-1)
   B(i)=tmp1
   C(i) = C(i) - tmp1 * D(i-1)
   D(i) = D(i) - tmp1 * E(i-1)
  #Else
   F(i) = F(i) - A(i) * F(i-2)
   F(i) = F(i) - B(i) * F(i-1)
  #End

   i=nib
  #If #OPT == "factor"
   tmp1=E(i) / C(i-3)  ! eliminate e(n)
   E(i)=tmp1
   A(i) = A(i) - tmp1 * D(i-3)
   B(i) = B(i) - tmp1 * E(i-3)

   tmp1=A(i) / C(i-2)  ! eliminate a(n)
   A(i)=tmp1
   B(i) = B(i) - tmp1 * D(i-2)
   C(i) = C(i) - tmp1 * E(i-2)

   tmp1=B(i) / C(i-1)    ! eliminate b(n)
   B(i)=tmp1
   C(i) = C(i) - tmp1 * D(i-1)
  #Else
   F(i) = F(i) - E(i) * F(i-3)
   F(i) = F(i) - A(i) * F(i-2)
   F(i) = F(i) - B(i) * F(i-1)
  #End
  endInnerLoop(AXIS)

 endOuterLoop(AXIS)

 #If #OPT == "solve"
 ! back substitution
 startOuterLoop(AXIS)

  startInnerLoop(AXIS)
   F(n) = F(n) / C(n)
   F(n-1) = ( F(n-1) - D(n-1)*F(n) )/C(n-1)
  endInnerLoop(AXIS)

  do i = n-2, nia+2, -1
    startInnerLoop(AXIS)
      F(i) = ( F(i) - D(i)*F(i+1) - E(i)*F(i+2) ) / C(i)
    endInnerLoop(AXIS)
  end do
  startInnerLoop(AXIS)
   i=nia+1
   F(i) = ( F(i) - D(i)*F(i+1) - E(i)*F(i+2)- A(i)*F(i+3) ) / C(i)
   i=nia
   F(i) = ( F(i) - D(i)*F(i+1) - E(i)*F(i+2)\
                          - A(i)*F(i+3) - B(i)*F(i+4) ) / C(i)
  endInnerLoop(AXIS)

 endOuterLoop(AXIS)
 #End

#endMacro



c ****************************************************************
c *************** Periodic System ********************************
c ****************************************************************
#beginMacro factorOrSolvePeriodic(OPT,AXIS)

 initFactorOrSolve(OPT,AXIS)

 if( nib-nia+1.lt.6 )then
   write(*,'(''pentaFactor:ERROR too few points. Need at least 6 points.'')')
   stop 3
 end if

 n=nib


c       Here is the matrix in the periodic case:
c
c             | c d e             a b |  0 
c             | b c d e             a |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | e             a b c d |  n-1 
c             | d e             a b c |  n  
c
 startOuterLoop(AXIS)

 #If #OPT == "factor"
  startInnerLoop(AXIS)
   i=nia
   S(i)=A(i)    ! replace a,b,a in upper right corner
   T(i)=B(i)
   i=nia+1
   S(i)=0.
   T(i)=A(i)
   i=nia
   U(i  )=E(n-1)  ! replace e,d,e in lower left corner
   U(i+1)=0.
   V(i  )=D(n)
   V(i+1)=E(n)
  endInnerLoop(AXIS)
 #End
          
c             | c d e             s t |  0 
c             | b c d e           s t |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | u u           a b c d |  n-1 
c             | v v             a b c |  n  

  do i = nia+1,n-5
 
   startInnerLoop(AXIS)
    #If #OPT == "factor"
     tmp1 = B(i)/C(i-1)    ! eliminate b(i)
     B(i)=tmp1                 ! save pivots in the element that we eliminate

     C(i) = C(i) - tmp1 * D(i-1)
     D(i) = D(i) - tmp1 * E(i-1) 
     S(i) = S(i) - tmp1 * S(i-1) 
     T(i) = T(i) - tmp1 * T(i-1) 

     tmp2 = A(i+1)/C(i-1)  ! eliminate a(i+1)
     A(i+1)=tmp2               ! save pivots in the element that we eliminate 
     B(i+1) = B(i+1) - tmp2 * D(i-1)
     C(i+1) = C(i+1) - tmp2 * E(i-1)
     S(i+1) =            - tmp2 * S(i-1)
     T(i+1) =            - tmp2 * T(i-1)
    #Else
     F(i) = F(i) - B(i) * F(i-1) 
     F(i+1) = F(i+1) - A(i+1) * F(i-1)
    #End
   endInnerLoop(AXIS)

  end do
      
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 b c d e s t |  n-4
c             |             a b c d e   |  n-3
c             |               a b c d e |  n-2 
c             | e               a b c d |  n-1 
c             | d e               a b c |  n
   
   !  b(n-4), a(n-3)
   !  b(n-3), a(n-2)
   !  b(n-2)
  startInnerLoop(AXIS)
   i=n-4
  #If #OPT == "factor"
   tmp1= B(i)/C(i-1)     ! eliminate b(n-4)
   B(i)=tmp1
   C(i) = C(i) - tmp1 * D(i-1)
   D(i) = D(i) - tmp1 * E(i-1)
   S(i) = S(i) - tmp1 * S(i-1)
   T(i) = T(i) - tmp1 * T(i-1)
  #Else
   F(i) = F(i) - B(i) * F(i-1)
  #End
  
   i=n-3
  #If #OPT == "factor"
   tmp1= A(i)/C(i-2)     ! eliminate a(n-3)
   A(i)=tmp1
   B(i) = B(i) - tmp1 * D(i-2)
   C(i) = C(i) - tmp1 * E(i-2)
   E(i) = E(i) - tmp1 * S(i-2)
   T(i) =          - tmp1 * T(i-2)

   tmp1= B(i)/C(i-1)    ! eliminate b(n-3)
   B(i)=tmp1
   C(i) = C(i) - tmp1 * D(i-1)
   D(i) = D(i) - tmp1 * E(i-1)
   E(i) = E(i) - tmp1 * S(i-1)
   T(i) = T(i) - tmp1 * T(i-1)
  #Else
   F(i) = F(i) - A(i) * F(i-2)
   F(i) = F(i) - B(i) * F(i-1)
  #End

   i=n-2
  #If #OPT == "factor"
   tmp1= A(i)/C(i-2)    ! eliminate a(n-2)
   A(i)=tmp1
   B(i) = B(i) - tmp1 * D(i-2)
   C(i) = C(i) - tmp1 * E(i-2)
   D(i) = D(i) - tmp1 * S(i-2)
   E(i) = E(i) - tmp1 * T(i-2)

   tmp1= B(i)/C(i-1)  ! eliminate b(n-2)
   B(i)=tmp1
   C(i) = C(i) - tmp1 * D(i-1)
   D(i) = D(i) - tmp1 * E(i-1)
   E(i) = E(i) - tmp1 * T(i-1)
  #Else
   F(i) = F(i) - A(i) * F(i-2)
   F(i) = F(i) - B(i) * F(i-1)
  #End
 endInnerLoop(AXIS)

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | u u             a b c d |  n-1 
c             | v v               a b c |  n



c         *** eliminate e(n-1), d(n) --> save pivots in u,v
   do i=nia,n-6
    startInnerLoop(AXIS)

    #If #OPT == "factor"
     tmp1=U(i)/C(i)   ! eliminate u(i)
     U(i)=tmp1
     U(i+1)=U(i+1)-tmp1*D(i)
     U(i+2)=          -tmp1*E(i)
     C(n-1)=C(n-1)-tmp1*S(i)    
     D(n-1)=D(n-1)-tmp1*T(i)


     tmp2=V(i)/C(i)           ! eliminate v(i)
     V(i)=tmp2
     V(i+1)=V(i+1)-tmp2*D(i)
     V(i+2)=          -tmp2*E(i)
     B(n)  =B(n)  -tmp2*S(i)
     C(n)  =C(n)  -tmp2*T(i)
    #Else
     F(n-1)=F(n-1)-U(i)*F(i)
     F(n)=F(n)-V(i)*F(i)
    #End

   endInnerLoop(AXIS)
  end do
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 u u a b c d |  n-1 
c             | 0 0 0 ... 0 v v 0 a b c |  n
c               0 1 2 ...  n-5        n
c
  startInnerLoop(AXIS)
   i=n-5
  #If #OPT == "factor"
   tmp1=U(i)/C(i)  ! eliminate u(n-5)
   U(i)=tmp1
   U(i+1)=U(i+1)-tmp1*D(i)
   A(n-1)=A(n-1)-tmp1*E(i)
   C(n-1)=C(n-1)-tmp1*S(i)    
   D(n-1)=D(n-1)-tmp1*T(i)

   tmp2=V(i)/C(i)        ! eliminate v(n-5)
   V(i)=tmp2
   V(i+1)=V(i+1)-tmp2*D(i)
   V(i+2)=          -tmp2*E(i)
   B(n)  =B(n)  -tmp2*S(i)
   C(n)  =C(n)  -tmp2*T(i)
  #Else
   F(n-1)=F(n-1)-U(i)*F(i)
   F(n)=F(n)  -V(i)*F(i)
  #End


c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 u a b c d |  n-1 
c             | 0 0 0 ... 0 0 v v a b c |  n
c               0 1 2 ...    n-4      n
c
   i=n-4
  #If #OPT == "factor"
   tmp1=U(i)/C(i)  ! eliminate u(n-4)
   U(i)=tmp1
   A(n-1)=A(n-1)-tmp1*D(i)
   B(n-1)=B(n-1)-tmp1*E(i)    
   C(n-1)=C(n-1)-tmp1*S(i)    
   D(n-1)=D(n-1)-tmp1*T(i)

   tmp2=V(i)/C(i)        ! eliminate v(n-4)
   V(i)=tmp2
   V(i+1)=V(i+1)-tmp2*D(i)
   A(n)  =A(n)  -tmp2*E(i)
   B(n)  =B(n)  -tmp2*S(i)
   C(n)  =C(n)  -tmp2*T(i)
  #Else
   F(n-1)=F(n-1)-U(i)*F(i)
   F(n)=F(n)  -V(i)*F(i)
  #End

   i=n-3
  #If #OPT == "factor"
   tmp1=A(n-1)/C(i)  ! eliminate a(n-1)
   A(n-1)=tmp1
   B(n-1)=B(n-1)-tmp1*D(i)    
   C(n-1)=C(n-1)-tmp1*E(i)    
   D(n-1)=D(n-1)-tmp1*T(i)

   tmp2=V(i)/C(i)        ! eliminate v(n-3)
   V(i)=tmp2
   A(n)  =A(n)  -tmp2*D(i)
   B(n)  =B(n)  -tmp2*E(i)
   C(n)  =C(n)  -tmp2*T(i)
  #Else
   F(n-1)=F(n-1)-A(n-1)*F(i)
   F(n)=F(n)  -V(i)*F(i)
  #End

c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 b c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 a b c |  n
c               0 1 2 ...        n-2  n
c
   i=n-2
  #If #OPT == "factor"
   tmp1=B(n-1)/C(i)  ! eliminate b(n-1)
   B(n-1)=tmp1
   C(n-1)=C(n-1)-tmp1*D(i)    
   D(n-1)=D(n-1)-tmp1*E(i)

   tmp1=A(n)/C(i)  ! eliminate a(n)
   A(n)=tmp1
   B(n)=B(n)-tmp1*D(i)    
   C(n)=C(n)-tmp1*E(i)    
  #Else
   F(n-1)=F(n-1)-B(n-1)*F(i)
   F(n)=F(n)-A(n)*F(i)
  #End

   i=n-1
  #If #OPT == "factor"
   tmp1=B(n)/C(i)  ! eliminate b(n)
   B(n)=tmp1
   C(n)=C(n)-tmp1*D(i)    
  #Else
   F(n)=F(n)-B(n)*F(i)
  #End

  endInnerLoop(AXIS)
  endOuterLoop(AXIS)

 ! back substitution
c         Here is what the matrix looks at the end
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 0 c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 0 0 c |  n
c               0 1 2 ...  n-5        n
c
 #If #OPT == "solve"
 startOuterLoop(AXIS)

   startInnerLoop(AXIS)
   F(n)=F(n)/C(n)
   F(n-1)=(F(n-1)-D(n-1)*F(n))/C(n-1)
   F(n-2)=(F(n-2)-D(n-2)*F(n-1)-E(n-2)*F(n))/C(n-2)
   F(n-3)=(F(n-3)-D(n-3)*F(n-2)-E(n-3)*F(n-1)-T(n-3)*F(n))/C(n-3)
   endInnerLoop(AXIS)

   do i=n-4,nia,-1
    startInnerLoop(AXIS)
      F(i)=(F(i)-D(i)*F(i+1)-E(i)*F(i+2)-S(i)*F(n-1)-T(i)*F(n))/C(i)
    endInnerLoop(AXIS)
   end do

 endOuterLoop(AXIS)
 #End

#endMacro






      subroutine pentaFactor(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
           ipar, a, b, c, d, e, s,t,u,v )
c ========================================================================
c   Factor a penta-diagonal matrix (no pivoting)
c 
c             | c[0] d[0] e[0]                     |
c             | b[1] c[1] d[1] e[1]                |
c         A = | a[2] b[2] c[2] d[2] e[2]           |
c             |      a[3] b[3] c[3] d[3] e[3]      |
c             |            .    .    .             |
c             |           a[.] b[.] c[.] d[.] e[.] |
c             |                a[.] b[.] c[.] d[.] |
c             |                     a[n] b[n] c[n] |
c 
c  Input:  a, b, c, d, e : arrays denoting the 5 diagonals.
c  Input: s,t,u,v : work-space for the periodic case (otherwise not used)
c                   These same work-spaces must be passed to the oslve function
c ========================================================================

      implicit none
c
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      real a(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real b(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real d(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real e(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real t(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*)
c ...... local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,systemType
      integer nia,nib,nja,njb,njc,nka,nkb,nkc

      integer i,j,k,nx,axis,n
      real tmp1,tmp2

      integer normal,extended,periodic
      parameter( normal=0,extended=1,periodic=2 )

c.........................

      n1a        = ipar(0)
      n1b        = ipar(1)
      n1c        = ipar(2) ! stride
      n2a        = ipar(3)
      n2b        = ipar(4)
      n2c        = ipar(5)
      n3a        = ipar(6)
      n3b        = ipar(7)
      n3c        = ipar(8)
      systemType = ipar(9)
      axis       = ipar(10)

      nx=n1b
      n=n1b
      if( systemType.eq.normal )then

        if( axis.eq.0 )then
          factorOrSolveNormal(factor,0)
        else if( axis.eq.1 )then
          factorOrSolveNormal(factor,1)
        else 
          factorOrSolveNormal(factor,2)
        end if

      else if( systemType.eq.extended )then
        if( axis.eq.0 )then
          factorOrSolveExtended(factor,0)
        else if( axis.eq.1 )then
          factorOrSolveExtended(factor,1)
        else 
          factorOrSolveExtended(factor,2)
        end if

      else if( systemType.eq.periodic )then

        ! ************************************************************
        ! ******************PERIODIC**********************************
        ! ************************************************************

        if( axis.eq.0 )then
          factorOrSolvePeriodic(factor,0)
        else if( axis.eq.1 )then
          factorOrSolvePeriodic(factor,1)
        else 
          factorOrSolvePeriodic(factor,2)
        end if

      else
        write(*,*) 'pentaFactor:ERROR: invalid system type=',systemType
        stop 7
      end if
 
      return
      end

      subroutine pentaSolve(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
           ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
           ipar, a, b, c, d, e, f, s,t,u,v )
c ========================================================================
c   Solve a penta-diagonal matrix
c             A x = f
c
c  Input: s,t,u,v : work-space for the periodic case (otherwise not used)
c                   These same work-spaces must be passed to the oslve function
c f : input/output: on input the RHS, on output the solution
c ========================================================================

      implicit none
c
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      real a(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real b(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real d(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real e(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real t(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b
      real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b)

      integer ipar(0:*)

c ...... local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,systemType,axis
      integer nia,nib,nja,njb,njc,nka,nkb,nkc
      integer i,j,k,nx,n
      real tmp1,tmp2

      integer normal,extended,periodic
      parameter( normal=0,extended=1,periodic=2 )
c.........................

      n1a        = ipar(0)
      n1b        = ipar(1)
      n1c        = ipar(2) ! stride
      n2a        = ipar(3)
      n2b        = ipar(4)
      n2c        = ipar(5)
      n3a        = ipar(6)
      n3b        = ipar(7)
      n3c        = ipar(8)
      systemType = ipar(9)
      axis       = ipar(10)

      nx=n1b

      if( systemType.eq.normal )then

        if( axis.eq.0 )then
          factorOrSolveNormal(solve,0)
        else if( axis.eq.1 )then
          factorOrSolveNormal(solve,1)
        else 
          factorOrSolveNormal(solve,2)
        end if

      else if( systemType.eq.extended )then

        if( axis.eq.0 )then
          factorOrSolveExtended(solve,0)
        else if( axis.eq.1 )then
          factorOrSolveExtended(solve,1)
        else 
          factorOrSolveExtended(solve,2)
        end if

      else if( systemType.eq.periodic )then

        ! **********************************************************
        ! **************PERIODIC************************************
        ! **********************************************************

        if( axis.eq.0 )then
          factorOrSolvePeriodic(solve,0)
        else if( axis.eq.1 )then
          factorOrSolvePeriodic(solve,1)
        else 
          factorOrSolvePeriodic(solve,2)
        end if

      else
        write(*,*) 'pentaSolve:ERROR: invalid system type=',systemType
        stop 6
      end if

      return
      end




