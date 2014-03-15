! getmixeos computes either: p(rho,se,alam) and p's derivatives or se(rho,p,alam)
! I/O: rho,se(small E),ppartials(p and partials),alam (mass ratio: material2/total),pi's and gamma's
!      dir (direction=1:p->se ; direction=2:se->p ; direction=3:se->p + p's partials), 
!      mthd: (method=1 new temp eqn, method=2 old temp eqn. Should matter only in mixed regions)
!      ier(=0 if no error)
!      "i" variables are the 1-material, "r" are the 2-material

      subroutine getmixeos (rho,se,ppartials,alam,dir,mthod,ier)

         implicit none
         integer dir,mthod,ier
         real*8 rho,se,alam,gami,cvi,pii,gamr,cvr,pir
         real*8 ppartials(4)

         real*8 alamInv,denom,numer
         real*8 MG,rhoSE,numer2
         real*8 stff1, stff2           
         real*8 MGNum,dMGNdR,dMGNdRL,dMGDdR,dMGDdRL,dMGdR,dMGdRL,MGDenom
   
         integer istiff
         include 'multiDat.h'

         stff1   = gami*pii
         stff2   = gamr*pir                  
         alamInv  = 1.0d0-alam 
         rhoSE=rho*se

         denom=cvi*alamInv + cvr*alam
         numer=cvr*alam*stff2 + cvi*alamInv*stff1

         !Mixture gamma
         MG=(gami*alamInv*cvi+ gamr*alam*cvr)/ 
     .                              (alamInv*cvi + (alam*cvr))

         !now computations
         if(dir .eq. 1) then
            se=(ppartials(1)+(numer/denom))/(rho*(MG-1.0d0))
!            write(*,*)'se',se,ppartials(1),numer,denom,rho,MG,gami,gamr
         else if(dir .eq. 2) then
            ppartials(1)=rhoSE*(MG-1.0d0) - (numer/denom)
!            write(*,*)'p',se,ppartials(1),numer,denom,rho,MG,gami,gamr
         else if(dir .eq. 3) then

            numer2=cvi*cvr*(stff1-stff2)

            MGNum  =rho*(gami*alamInv*cvi + gamr*alam*cvr)         
            MGDenom=rho*(alamInv*cvi + alam*cvr)
            dMGNdR =  cvi*gami
            dMGNdRL= -cvi*gami+gamr*cvr
            dMGDdR =  cvi
            dMGDdRL= -cvi+cvr  
            dMGdR  = (MGDenom*dMGNdR - MGNum*dMGDdR)/(MGDenom**2)
            dMGdRL = (MGDenom*dMGNdRL - MGNum*dMGDdRL)/(MGDenom**2)
            
            ppartials(1)=rhoSE*(MG-1.0d0) - (numer/denom)
            ppartials(2)=dMGdR*rhoSE - (numer2*alam/denom**2)/rho
            ppartials(3)=MG-1.0d0
            ppartials(4)=dMGdRL*rhoSE+(numer2/denom**2)/rho
!            write(6,*)"full",rho,se, ppartials(1),numer,denom,rhoSE,MG
         else
            write(*,*)'getmixeos.f: nothing found'
            stop
         end if

         ier=0
      end


      subroutine getmixeos_2 (rho,se,ppartials,alam,dir,mthod,ier)

         implicit none
         integer dir,mthod,ier
         real*8 rho,se,alam,gami,cvi,pii,gamr,cvr,pir
         real*8 ppartials(4)

         real*8 alamInv,denom,numer
         real*8 MG,rhoSE,numer2
         real*8 stff1, stff2           
         real*8 MGNum,dMGNdR,dMGNdRL,dMGDdR,dMGDdRL,dMGdR,dMGdRL,MGDenom

         real*8 omega,gamiM1,gamrM1,gamM1R
   
         integer istiff
         include 'multiDat.h'


         stff1   = gami*pii
         stff2   = gamr*pir                  
         alamInv  = 1.0d0-alam 
         rhoSE=rho*se
         gamiM1=gami-1.0d0
         gamrM1=gamr-1.0d0
         gamM1R=gamiM1/gamrM1

!         omega=gamrM1*cvr/(gamiM1*cvi)
         omega=10.0d0 
 
         denom=alamInv + alam*omega*gamM1R
         numer=rhoSE*gamiM1*(alamInv+omega*alam)-
     .             (alamInv*stff1+alam*omega*gamM1R*stff2)

         !now computations
         if(dir .eq. 1) then
            se=(alamInv*gamrM1*(ppartials(1)+stff1) + 
     .           alam*omega*gamiM1*(ppartials(1)+stff2))/
     .               (rho*gamrM1*gamiM1*(alamInv+omega*alam))
!            write(*,*)'se',se,ppartials(1),numer,denom,rho,MG,gami,gamr
         else if(dir .eq. 2) then
            ppartials(1)=numer/denom
         else if(dir .eq. 3) then
            
            !Slightly misleading MG no longer means mixture gamma
            MGNum  = rho*numer         
            MGDenom= rho*denom
            dMGNdR = rhoSE*gamiM1-stff1
            dMGNdRL= rhoSE*gamiM1*(omega-1.0d0)-
     .                     (omega*gamM1R*stff2-stff1)
            dMGDdR = 1.0d0
            dMGDdRL= omega*gamM1R-1.0d0
            dMGdR  = (MGDenom*dMGNdR - MGNum*dMGDdR)/(MGDenom**2)
            dMGdRL = (MGDenom*dMGNdRL - MGNum*dMGDdRL)/(MGDenom**2)
            
            ppartials(1)=numer/denom
            ppartials(2)=dMGdR
            ppartials(3)=gamiM1*(alamInv+omega*alam)/denom
            ppartials(4)=dMGdRL
         else
            write(*,*)'getmixeos.f: nothing found'
            stop
         end if

         ier=0
      end
