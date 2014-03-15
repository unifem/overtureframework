#beginMacro loops(expression)
if( useWhereMask.ne.0 )then
  do k3=n3a,n3b,n3c
  do k2=n2a,n2b,n2c
  do k1=n1a,n1b,n1c
    if( mask(k1,k2,k3).ne.0 )then
      do m=m1a,m1b ! ghost lines
        expression
      end do
    end if
  end do
  end do
  end do
else
  do k3=n3a,n3b,n3c
  do k2=n2a,n2b,n2c
  do k1=n1a,n1b,n1c
    do m=m1a,m1b ! ghost lines
      expression
    end do
  end do
  end do
  end do
end if
#endMacro

c ===============================================================================================
#beginMacro symmetryCondition(DIM,SPECIES)

c  symmetry boundary condition 

c  loops(u(i1-m*(is1),i2-m*(is2),i3-m*(is3),c)=u(i1+m*(is1),i2+m*(is2),i3+m*(is3),c))

   i1=k1-m*(is1)
   i2=k2-m*(is2)
   i3=k3-m*(is3)
   j1=k1+m*(is1)
   j2=k2+m*(is2)
   j3=k3+m*(is3)
       
   u(i1,i2,i3,rc)=u(j1,j2,j3,rc)
   u(i1,i2,i3,tc)=u(j1,j2,j3,tc)
 #If #SPECIES eq "species"
   do s=0,ns-1
     u(i1,i2,i3,s)=u(j1,j2,j3,s)
   end do
 #End     
 #If #DIM eq "2"
   u(i1,i2,i3,uc)=u(j1,j2,j3,uc)
   u(i1,i2,i3,vc)=u(j1,j2,j3,vc)
     
   ! velocity: n.u is odd
   !  n.u(-1) = - n.u(+1)
   ! u(-1) <- u(-1) - (n.u)(-1)*n -(n.u)(+1)*n
   nv0=normal(k1,k2,k3,0)
   nv1=normal(k1,k2,k3,1)

   ndum=nv0*u(i1,i2,i3,uc)+nv1*u(i1,i2,i3,vc)
   ndup=nv0*u(j1,j2,j3,uc)+nv1*u(j1,j2,j3,vc)
   u(i1,i2,i3,uc)-=(ndup+ndum)*nv0   
   u(i1,i2,i3,vc)-=(ndup+ndum)*nv1
 #Else
   u(i1,i2,i3,uc)=u(j1,j2,j3,uc)
   u(i1,i2,i3,vc)=u(j1,j2,j3,vc)
   u(i1,i2,i3,wc)=u(j1,j2,j3,wc)
     
   ! velocity: n.u is odd
   !  n.u(-1) = - n.u(+1)
   ! u(-1) <- u(-1) - (n.u)(-1)*n -(n.u)(+1)*n
   nv0=normal(k1,k2,k3,0)
   nv1=normal(k1,k2,k3,1)
   nv2=normal(k1,k2,k3,2)

   ndum=nv0*u(i1,i2,i3,uc)+nv1*u(i1,i2,i3,vc)+nv2*u(i1,i2,i3,wc)
   ndup=nv0*u(j1,j2,j3,uc)+nv1*u(j1,j2,j3,vc)+nv2*u(j1,j2,j3,wc)
   u(i1,i2,i3,uc)-=(ndup+ndum)*nv0   
   u(i1,i2,i3,vc)-=(ndup+ndum)*nv1
   u(i1,i2,i3,wc)-=(ndup+ndum)*nv2
 #End


#beginMacro assignSymmetryCorners(side1,side2,side3)
#endMacro



      subroutine cnsSymmetryBoundaryCorners( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask, ca,cb, useWhereMask, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, orderOfExtrapolation )    
c======================================================================
c  Assign symmetry boundary at edges and corners 
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb

      integer extrapolateCorner,symmetryCorner,taylor2ndOrder
      parameter(extrapolateCorner=0,symmetryCorner=1,taylor2ndOrder=2)

c     --- local variables 
      integer side1,side2,side3,is1,is2,is3,i1,i2,i3  
      integer n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c
  
      
c        --- assign values outside edges by symmetry ---

      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 )then
c     ...Do the four edges parallel to i3
        n3a=indexRange(0,2)
        n3b=indexRange(1,2)
        n3c=1
        side3=2 ! this means we are on an edge
        is3=0
        do side1=0,1
          symmetry1 = bc(side1,0).eq.slipWall .or. bc(side1,0).eq.symmetry
          if( symmetry1 )then
            is1=1-2*side1
          else 
            is1=0
          end if
          do side2=0,1
            symmetry2 = bc(side2,1).eq.slipWall .or. bc(side2,1).eq.symmetry
            if( symmetry1 .or. symmetry2 )then
              if( .not.symmetry1 )then
   	        is2=1-2*side2
              else
                is2=0
              end if
              ! loop from inside to outside -- not really necessary
              n2a=indexRange(side2,1)-is2
              n2b=dimension(side2,1)
              n2c=-is2
	      n1a=indexRange(side1,0)-is1
              n1b=dimension(side1,0)
              n1c=-is1

	      assignSymmetryCorners()
            end if
          end do
        end do
 
      end if
      if( nd.le.2 )then
        return
      end if

      if( isPeriodic(0).eq.0 .and. isPeriodic(2).eq.0 )then
c     ...Do the four edges parallel to i2
        n2a=indexRange(0,1)
        n2b=indexRange(1,1)
        n2c=1
        side2=2 ! this means we are on an edge
        is2=0
        do side1=0,1
          is1=1-2*side1
          do side3=0,1
            is3=1-2*side3
            if( bc(side1,0).gt.0 .or. bc(side3,2).gt.0 )then
              ! loop from inside to outside
              n3a=indexRange(side3,2)-is3
              n3b=dimension(side3,2)
              n3c=-is3
	      n1a=indexRange(side1,0)-is1 
              n1b=dimension(side1,0)
              n1c=-is1
	      assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if

      if( isPeriodic(1).eq.0 .and. isPeriodic(2).eq.0 )then
c          ...Do the four edges parallel to i1
        n1a=indexRange(0,0)
        n1b=indexRange(1,0)
        n1c=1
        side1=2 ! this means we are on an edge
        is1=0
        do side2=0,1
          is2=1-2*side2
          ! loop from inside to outside
          n2a=indexRange(side2,1)-is2
          n2b=dimension(side2,1)
          n2c=-is2
          do side3=0,1
            is3=1-2*side3
            if( bc(side2,1).gt.0 .or. bc(side3,2).gt.0 )then
c             We have to loop over i3 from inside to outside since later points depend on previous ones.
	      n3a=indexRange(side3,2)-is3 
              n3b=dimension(side3,2)
              n3c=-is3
              assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if
  
      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 .and. 
     &    isPeriodic(2).eq.0 )then
c           ...Do the points outside vertices in 3D
        do side1=0,1
          is1=1-2*side1
          n1a=indexRange(side1,0)-is1 
          n1b=dimension(side1,0)
          n1c=-is1
          do side2=0,1 
            is2=1-2*side2
            ! loop from inside to outside
            n2a=indexRange(side2,1)-is2
            n2b=dimension(side2,1)
            n2c=-is2
            do side3=0,1
              is3=1-2*side3
              if( bc(side1,0).gt.0 .or.
     &            bc(side2,1).gt.0 .or.
     &            bc(side3,2).gt.0 )then

                n3a=indexRange(side3,2)-is3 
                n3b=dimension(side3,2)
                n3c=-is3
c     write(*,'(''n1a,n1b,n2a,n2b,n3a,n3b,n3c='',6i4,'' cornerBC='',i4)') n1a,n1b,n2a,n2b,n3a,n3b,n3c,cornerBC(side1,side2,side3)                
c     write(*,'(''orderOfExtrapolation='',i4)') orderOfExtrapolation
                assignCorners(side1,side2,side3)
              end if
            end do
          end do
        end do
      end if


      return
      end
