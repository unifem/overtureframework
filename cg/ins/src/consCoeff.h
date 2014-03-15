! ========== This include file contains conservative approximations to coefficient operators ================
!
!    ajac2d(i1,i2,i3)
!    ajac3d(i1,i2,i3
!    getCoeffForDxADxPlusDyBDy(au, azmz,amzz,azzz,apzz,azpz, bzmz,bmzz,bzzz,bpzz,bzpz )
!    getCoeffForDyADx( au, azmz,amzz,azzz,apzz,azpz )
!    getCoeffForDxADy( au, azmz,amzz,azzz,apzz,azpz )
!    setDivTensorGradCoeff2d(cmp,eqn,a11ph,a11mh,a22ph,a22mh,a12pzz,a12mzz,a21zpz,a21zmz)
!    scaleCoefficients( a11ph,a11mh,a22ph,a22mh,a12pzz,a12mzz,a21zpz,a21zmz )
!    getCoeffForDxADxPlusDyBDyPlusDzCDz(au, azzm,azmz,amzz,azzz,apzz,azpz,azzp,...)
!    getCoeffForDxADy3d(au, X, Y, azzm,azmz,amzz,azzz,apzz,azpz,azzp )

#defineMacro ajac2d(i1,i2,i3) (1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))
#defineMacro ajac3d(i1,i2,i3) (1./((rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))*tz(i1,i2,i3)+\
                               (ry(i1,i2,i3)*sz(i1,i2,i3)-rz(i1,i2,i3)*sy(i1,i2,i3))*tx(i1,i2,i3)+\
                               (rz(i1,i2,i3)*sx(i1,i2,i3)-rx(i1,i2,i3)*sz(i1,i2,i3))*ty(i1,i2,i3)))

! =============================================================================================================
!  Declare variables used in compute the coefficients for a non-linear viscosity
! =============================================================================================================
#beginMacro declareNonLinearViscosityVariables()

 real nu0ph,nu0mh,nu1ph,nu1mh,nu2ph,nu2mh
 real nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp
 real ajzzm,ajzmz,ajmzz,ajzzz,ajpzz,ajzpz,ajzzp

 real a11ph,a11mh,a22ph,a22mh,a33ph,a33mh,a11mzz,a11zzz,a11pzz,a22zmz,a22zzz,a22zpz,a33zzm,a33zzz,a33zzp,\
      a12pzz,a12zzz,a12mzz,a13pzz,a13zzz,a13mzz,a21zpz,a21zzz,a21zmz,a23zpz,a23zzz,a23zmz,\
      a31zzp,a31zzz,a31zzm,a32zzp,a32zzz,a32zzm
 real b11ph,b11mh,b22ph,b22mh,b33ph,b33mh,b11mzz,b11zzz,b11pzz,b22zmz,b22zzz,b22zpz,b33zzm,b33zzz,b33zzp,\
      b12pzz,b12zzz,b12mzz,b13pzz,b13zzz,b13mzz,b21zpz,b21zzz,b21zmz,b23zpz,b23zzz,b23zmz,\
      b31zzp,b31zzz,b31zzm,b32zzp,b32zzz,b32zzm
 real c11ph,c11mh,c22ph,c22mh,c33ph,c33mh,c11mzz,c11zzz,c11pzz,c22zmz,c22zzz,c22zpz,c33zzm,c33zzz,c33zzp,\
      c12pzz,c12zzz,c12mzz,c13pzz,c13zzz,c13mzz,c21zpz,c21zzz,c21zmz,c23zpz,c23zzz,c23zmz,\
      c31zzp,c31zzz,c31zzm,c32zzp,c32zzz,c32zzm

 real au11ph,au11mh,au22ph,au22mh,au33ph,au33mh,au11mzz,au11zzz,au11pzz,au22zmz,au22zzz,au22zpz,au33zzm,au33zzz,au33zzp,\
      au12pzz,au12zzz,au12mzz,au13pzz,au13zzz,au13mzz,au21zpz,au21zzz,au21zmz,au23zpz,au23zzz,au23zmz,\
      au31zzp,au31zzz,au31zzm,au32zzp,au32zzz,au32zzm
 real av11ph,av11mh,av22ph,av22mh,av33ph,av33mh,av11mzz,av11zzz,av11pzz,av22zmz,av22zzz,av22zpz,av33zzm,av33zzz,av33zzp,\
      av12pzz,av12zzz,av12mzz,av13pzz,av13zzz,av13mzz,av21zpz,av21zzz,av21zmz,av23zpz,av23zzz,av23zmz,\
      av31zzp,av31zzz,av31zzm,av32zzp,av32zzz,av32zzm
 real aw11ph,aw11mh,aw22ph,aw22mh,aw33ph,aw33mh,aw11mzz,aw11zzz,aw11pzz,aw22zmz,aw22zzz,aw22zpz,aw33zzm,aw33zzz,aw33zzp,\
      aw12pzz,aw12zzz,aw12mzz,aw13pzz,aw13zzz,aw13mzz,aw21zpz,aw21zzz,aw21zmz,aw23zpz,aw23zzz,aw23zmz,\
      aw31zzp,aw31zzz,aw31zzm,aw32zzp,aw32zzz,aw32zzm

 real bu11ph,bu11mh,bu22ph,bu22mh,bu33ph,bu33mh,bu11mzz,bu11zzz,bu11pzz,bu22zmz,bu22zzz,bu22zpz,bu33zzm,bu33zzz,bu33zzp,\
      bu12pzz,bu12zzz,bu12mzz,bu13pzz,bu13zzz,bu13mzz,bu21zpz,bu21zzz,bu21zmz,bu23zpz,bu23zzz,bu23zmz,\
      bu31zzp,bu31zzz,bu31zzm,bu32zzp,bu32zzz,bu32zzm
 real bv11ph,bv11mh,bv22ph,bv22mh,bv33ph,bv33mh,bv11mzz,bv11zzz,bv11pzz,bv22zmz,bv22zzz,bv22zpz,bv33zzm,bv33zzz,bv33zzp,\
      bv12pzz,bv12zzz,bv12mzz,bv13pzz,bv13zzz,bv13mzz,bv21zpz,bv21zzz,bv21zmz,bv23zpz,bv23zzz,bv23zmz,\
      bv31zzp,bv31zzz,bv31zzm,bv32zzp,bv32zzz,bv32zzm
 real bw11ph,bw11mh,bw22ph,bw22mh,bw33ph,bw33mh,bw11mzz,bw11zzz,bw11pzz,bw22zmz,bw22zzz,bw22zpz,bw33zzm,bw33zzz,bw33zzp,\
      bw12pzz,bw12zzz,bw12mzz,bw13pzz,bw13zzz,bw13mzz,bw21zpz,bw21zzz,bw21zmz,bw23zpz,bw23zzz,bw23zmz,\
      bw31zzp,bw31zzz,bw31zzm,bw32zzp,bw32zzz,bw32zzm

 real cu11ph,cu11mh,cu22ph,cu22mh,cu33ph,cu33mh,cu11mzz,cu11zzz,cu11pzz,cu22zmz,cu22zzz,cu22zpz,cu33zzm,cu33zzz,cu33zzp,\
      cu12pzz,cu12zzz,cu12mzz,cu13pzz,cu13zzz,cu13mzz,cu21zpz,cu21zzz,cu21zmz,cu23zpz,cu23zzz,cu23zmz,\
      cu31zzp,cu31zzz,cu31zzm,cu32zzp,cu32zzz,cu32zzm
 real cv11ph,cv11mh,cv22ph,cv22mh,cv33ph,cv33mh,cv11mzz,cv11zzz,cv11pzz,cv22zmz,cv22zzz,cv22zpz,cv33zzm,cv33zzz,cv33zzp,\
      cv12pzz,cv12zzz,cv12mzz,cv13pzz,cv13zzz,cv13mzz,cv21zpz,cv21zzz,cv21zmz,cv23zpz,cv23zzz,cv23zmz,\
      cv31zzp,cv31zzz,cv31zzm,cv32zzp,cv32zzz,cv32zzm
 real cw11ph,cw11mh,cw22ph,cw22mh,cw33ph,cw33mh,cw11mzz,cw11zzz,cw11pzz,cw22zmz,cw22zzz,cw22zpz,cw33zzm,cw33zzz,cw33zzp,\
      cw12pzz,cw12zzz,cw12mzz,cw13pzz,cw13zzz,cw13mzz,cw21zpz,cw21zzz,cw21zmz,cw23zpz,cw23zzz,cw23zmz,\
      cw31zzp,cw31zzz,cw31zzm,cw32zzp,cw32zzz,cw32zzm


#endMacro



! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dx( a*Dx ) + Dy( b*Dy ) 
! 
!   L   = (1/J)*[ Dr( J*(rx,ry).(aDx,bDy)) + Ds( J*(sx,sy).(aDx,bDy)) ] 
!       = (1/J)*[ Dr( J*a*rx(rx*Dr + sx*Ds) + J*b*ry*(ry*Dr + sy*Ds)
!                +Ds( J*a*sx(rx*Dr + sx*Ds) + J*b*sy*(ry*Dr + sy*Ds) ] 
!       = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Ds( a21 Dr) + Ds( a22 Ds) ]
! where 
!  a11 = J ( a rx^2 + b ry^2 )
!  a12 = J ( a rx*sx + b ry*sy )
!  a21 = a12 
!  a22 = J ( a sx^2 + b sy^2 )
!  a = a(i1,i2,i3), b=b(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azmz,amzz,azzz,apzz,azpz : a(i1,i2-1,i2),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3)
!   bzmz,bmzz,bzzz,bpzz,bzpz : b(i1,i2-1,i2),b(i1-1,i2,i3),b(i1,i2,i3),b(i1+1,i2,i3),b(i1,i2+1,i3)
! The following jacobian values should also be defined:
!    ajzmz,ajmzz,ajzzz,ajpzz,ajzpz : aj(i1,i2-1,i2),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3)
! ==================================================================================================
#beginMacro getCoeffForDxADxPlusDyBDy(au, azmz,amzz,azzz,apzz,azpz, bzmz,bmzz,bzzz,bpzz,bzpz )
 au ## 11mzz = ajmzz*( (amzz)*rx(i1-1,i2,i3)*rx(i1-1,i2,i3) + (bmzz)*ry(i1-1,i2,i3)*ry(i1-1,i2,i3) )
 au ## 11zzz = ajzzz*( (azzz)*rx(i1  ,i2,i3)*rx(i1  ,i2,i3) + (bzzz)*ry(i1  ,i2,i3)*ry(i1  ,i2,i3) )
 au ## 11pzz = ajpzz*( (apzz)*rx(i1+1,i2,i3)*rx(i1+1,i2,i3) + (bpzz)*ry(i1+1,i2,i3)*ry(i1+1,i2,i3) )
 au ## 11ph = .5*( au ## 11zzz+au ## 11pzz )
 au ## 11mh = .5*( au ## 11zzz+au ## 11mzz )

 au ## 22zmz = ajzmz*( (azmz)*sx(i1,i2-1,i3)*sx(i1,i2-1,i3) + (bzmz)*sy(i1,i2-1,i3)*sy(i1,i2-1,i3) )
 au ## 22zzz = ajzzz*( (azzz)*sx(i1,i2  ,i3)*sx(i1,i2  ,i3) + (bzzz)*sy(i1,i2  ,i3)*sy(i1,i2  ,i3) )
 au ## 22zpz = ajzpz*( (azpz)*sx(i1,i2+1,i3)*sx(i1,i2+1,i3) + (bzpz)*sy(i1,i2+1,i3)*sy(i1,i2+1,i3) )
 au ## 22ph = .5*( au ## 22zzz+au ## 22zpz )
 au ## 22mh = .5*( au ## 22zzz+au ## 22zmz )

 au ## 12mzz = ajmzz*( (amzz)*rx(i1-1,i2,i3)*sx(i1-1,i2,i3) + (bmzz)*ry(i1-1,i2,i3)*sy(i1-1,i2,i3) )
 au ## 12zzz = ajzzz*( (azzz)*rx(i1  ,i2,i3)*sx(i1  ,i2,i3) + (bzzz)*ry(i1  ,i2,i3)*sy(i1  ,i2,i3) )
 au ## 12pzz = ajpzz*( (apzz)*rx(i1+1,i2,i3)*sx(i1+1,i2,i3) + (bpzz)*ry(i1+1,i2,i3)*sy(i1+1,i2,i3) )

 au ## 21zmz = ajzmz*( (azmz)*sx(i1,i2-1,i3)*rx(i1,i2-1,i3) + (bzmz)*sy(i1,i2-1,i3)*ry(i1,i2-1,i3) )
 au ## 21zzz = ajzzz*( (azzz)*sx(i1,i2  ,i3)*rx(i1,i2  ,i3) + (bzzz)*sy(i1,i2  ,i3)*ry(i1,i2  ,i3) )
 au ## 21zpz = ajzpz*( (azpz)*sx(i1,i2+1,i3)*rx(i1,i2+1,i3) + (bzpz)*sy(i1,i2+1,i3)*ry(i1,i2+1,i3) )
#endMacro

! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dy( a*Dx ) 
!  L = div( (0,aDx) ) 
!  L = (1/J)*[ Dr( J*(rx,ry).(0,aDx)) + Ds( J*(sx,sy).(0,aDx)) ]
!    = (1/J)*[ Dr( J*a*ry*(rx*Dr + sx*Ds)) + Ds( J*a*sy*(rx*Dr + sx*Ds) ) ]
!    = (1/J)*( Dr( a11 Dr) + Dr( a12 Ds) + Ds( a21 Dr) + Ds( a22 Ds) ) u 
! where
!  a11 = J ( a ry*rx )
!  a12 = J ( a ry*sx )
!  a21 = J ( a sy*rx )
!  a22 = J ( a sy*sx )
!  a = a(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azmz,amzz,azzz,apzz,azpz : a(i1,i2-1,i2),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3)
! The following jacobian values should also be defined:
!    ajzmz,ajmzz,ajzzz,ajpzz,ajzpz : aj(i1,i2-1,i2),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3)
! ==================================================================================================
#beginMacro getCoeffForDyADx( au, azmz,amzz,azzz,apzz,azpz )

 au ## 11mzz = ajmzz*( (amzz)*ry(i1-1,i2,i3)*rx(i1-1,i2,i3) )
 au ## 11zzz = ajzzz*( (azzz)*ry(i1  ,i2,i3)*rx(i1  ,i2,i3) )
 au ## 11pzz = ajpzz*( (apzz)*ry(i1+1,i2,i3)*rx(i1+1,i2,i3) )
 au ## 11ph = .5*( au ## 11zzz+au ## 11pzz )
 au ## 11mh = .5*( au ## 11zzz+au ## 11mzz )

 au ## 22zmz = ajzmz*( (azmz)*sy(i1,i2-1,i3)*sx(i1,i2-1,i3) )
 au ## 22zzz = ajzzz*( (azzz)*sy(i1,i2  ,i3)*sx(i1,i2  ,i3) )
 au ## 22zpz = ajzpz*( (azpz)*sy(i1,i2+1,i3)*sx(i1,i2+1,i3) )
 au ## 22ph = .5*( au ## 22zzz+au ## 22zpz )
 au ## 22mh = .5*( au ## 22zzz+au ## 22zmz )

 au ## 12mzz = ajmzz*( (amzz)*ry(i1-1,i2,i3)*sx(i1-1,i2,i3) )
 au ## 12zzz = ajzzz*( (azzz)*ry(i1  ,i2,i3)*sx(i1  ,i2,i3) )
 au ## 12pzz = ajpzz*( (apzz)*ry(i1+1,i2,i3)*sx(i1+1,i2,i3) )

 au ## 21zmz = ajzmz*( (azmz)*sy(i1,i2-1,i3)*rx(i1,i2-1,i3) )
 au ## 21zzz = ajzzz*( (azzz)*sy(i1,i2  ,i3)*rx(i1,i2  ,i3) )
 au ## 21zpz = ajzpz*( (azpz)*sy(i1,i2+1,i3)*rx(i1,i2+1,i3) )

#endMacro

! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dx( a*Dy ) 
!
!  L = div( (aDy,0) ) 
!  L = (1/J)*[ Dr( J*(rx,ry).(aDy,0)) + Ds(J*(sx,sy).(aDy,0)) ]
!    = (1/J)*[ Dr( J*a*rx*(ry*Dr + sy*Ds)) + Ds( J*a*sx*(ry*Dr + sy*Ds) ) ]
!    = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Ds( a21 Dr) + Ds( a22 Ds) ]
! where
!  a11 = J ( a rx*ry )
!  a12 = J ( a rx*sy )
!  a21 = J ( a sx*ry )
!  a22 = J ( a sx*sy )
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azmz,amzz,azzz,apzz,azpz : a(i1,i2-1,i2),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3)
! The following jacobian values should also be defined:
!    ajzmz,ajmzz,ajzzz,ajpzz,ajzpz : aj(i1,i2-1,i2),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3)
! ==================================================================================================
#beginMacro getCoeffForDxADy( au, azmz,amzz,azzz,apzz,azpz )

 au ## 11mzz = ajmzz*( (amzz)*rx(i1-1,i2,i3)*ry(i1-1,i2,i3) )
 au ## 11zzz = ajzzz*( (azzz)*rx(i1  ,i2,i3)*ry(i1  ,i2,i3) )
 au ## 11pzz = ajpzz*( (apzz)*rx(i1+1,i2,i3)*ry(i1+1,i2,i3) )
 au ## 11ph = .5*( au ## 11zzz+au ## 11pzz )
 au ## 11mh = .5*( au ## 11zzz+au ## 11mzz )

 au ## 22zmz = ajzmz*( (azmz)*sx(i1,i2-1,i3)*sy(i1,i2-1,i3) )
 au ## 22zzz = ajzzz*( (azzz)*sx(i1,i2  ,i3)*sy(i1,i2  ,i3) )
 au ## 22zpz = ajzpz*( (azpz)*sx(i1,i2+1,i3)*sy(i1,i2+1,i3) )
 au ## 22ph = .5*( au ## 22zzz+au ## 22zpz )
 au ## 22mh = .5*( au ## 22zzz+au ## 22zmz )

 au ## 12mzz = ajmzz*( (amzz)*rx(i1-1,i2,i3)*sy(i1-1,i2,i3) )
 au ## 12zzz = ajzzz*( (azzz)*rx(i1  ,i2,i3)*sy(i1  ,i2,i3) )
 au ## 12pzz = ajpzz*( (apzz)*rx(i1+1,i2,i3)*sy(i1+1,i2,i3) )

 au ## 21zmz = ajzmz*( (azmz)*sx(i1,i2-1,i3)*ry(i1,i2-1,i3) )
 au ## 21zzz = ajzzz*( (azzz)*sx(i1,i2  ,i3)*ry(i1,i2  ,i3) )
 au ## 21zpz = ajzpz*( (azpz)*sx(i1,i2+1,i3)*ry(i1,i2+1,i3) )

#endMacro


! =============================================================================================================
! Assign the coefficients for a component of the conservative discretization of the div(tensor grad) operator
! =============================================================================================================
#beginMacro setDivTensorGradCoeff2d(cmp,eqn,a11ph,a11mh,a22ph,a22mh,a12pzz,a12mzz,a21zpz,a21zmz)
  coeff(MCE(-1,-1,0,cmp,eqn),i1,i2,i3)=  a12mzz+a21zmz
  coeff(MCE( 0,-1,0,cmp,eqn),i1,i2,i3)=                      a22mh 
  coeff(MCE( 1,-1,0,cmp,eqn),i1,i2,i3)= -a12pzz-a21zmz 
  coeff(MCE(-1, 0,0,cmp,eqn),i1,i2,i3)=         a11mh
  coeff(MCE( 0, 0,0,cmp,eqn),i1,i2,i3)= -a11ph-a11mh -a22ph -a22mh
  coeff(MCE( 1, 0,0,cmp,eqn),i1,i2,i3)=  a11ph
  coeff(MCE(-1, 1,0,cmp,eqn),i1,i2,i3)= -a12mzz-a21zpz
  coeff(MCE( 0, 1,0,cmp,eqn),i1,i2,i3)=               a22ph
  coeff(MCE( 1, 1,0,cmp,eqn),i1,i2,i3)=  a12pzz+a21zpz
#endMacro


! =======================================================================================================
! This macro scaled the coefficients that appear in the discretization of the div(tensor grad) operator
! =======================================================================================================
#beginMacro scaleCoefficients( a11ph,a11mh,a22ph,a22mh,a12pzz,a12mzz,a21zpz,a21zmz )
 a11ph=a11ph*dr0i
 a11mh=a11mh*dr0i
 a22ph=a22ph*dr1i
 a22mh=a22mh*dr1i
 a12pzz=a12pzz*dr0dr1
 a12mzz=a12mzz*dr0dr1
 a21zpz=a21zpz*dr0dr1
 a21zmz=a21zmz*dr0dr1
#endMacro



! ****************************************************************
! ****************** THREE DIMENSIONS ****************************
! ****************************************************************


! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dx( a*Dx ) + Dy( b*Dy ) + Dz( c*Dz )
! 
!   L   = (1/J)*[ Dr( J*(rx,ry,rz).(aDx,bDy,cDz)) + Ds( J*(sx,sy,sz).(aDx,bDy,cDz)) + Dt( J*(tx,ty,tz).(aDx,bDy,cDz)) ] 
!       = (1/J)*[ Dr( J*a*rx(rx*Dr + sx*Ds+ tx*Dt) + J*b*ry*(ry*Dr + sy*Ds+ ty*Dt) + J*c*rz*(rz*Dr + sz*Ds+ tz*Dt)
!                +Ds( J*a*sx(rx*Dr + sx*Ds+ tx*Dt) + J*b*sy*(ry*Dr + sy*Ds+ ty*Dt) + J*c*sz*(rz*Dr + sz*Ds+ tz*Dt)
!                +Dt( J*a*tx(rx*Dr + sx*Ds+ tx*Dt) + J*b*ty*(ry*Dr + sy*Ds+ ty*Dt) + J*c*tz*(rz*Dr + sz*Ds+ tz*Dt)
!       = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Dr( a13 Dt) + Ds( a21 Dr) + Ds( a22 Ds) + Ds( a23 Dt) + Dt( a31 Dr) + Dt( a32 Ds) + Dt( a33 Dt) ]
! where 
!  a11 = J ( a rx^2 + b ry^2 + c rz^2 )
!  a12 = J ( a rx*sx + b ry*sy + b rz*sz )
!  a13 = J ( a rx*tx + b ry*ty + b rz*tz )
!  a21 = a12 
!  a22 = J ( a sx^2 + b sy^2 + c sz^2 )
!  a23 = J ( a sx*tx + b sy*ty + b sz*tz )
!  a31 = a13
!  a32 = a23 
!  a33 = J ( a tx^2 + b ty^2 + c tz^2 )
!  a = a(i1,i2,i3), b=b(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azzm,azmz,amzz,azzz,apzz,azpz,azzp : a(i1,i2,i3-1),a(i1,i2-1,i3),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3),a(i1,i2,i3+1)
!   bzzm,bzmz,bmzz,bzzz,bpzz,bzpz,bzzp : b(i1,i2,i3-1),b(i1,i2-1,i3),b(i1-1,i2,i3),b(i1,i2,i3),b(i1+1,i2,i3),b(i1,i2+1,i3),b(i1,i2,i3+1)
!   czzm,czmz,cmzz,czzz,cpzz,czpz,czzp : c(i1,i2,i3-1),c(i1,i2-1,i3),c(i1-1,i2,i3),c(i1,i2,i3),c(i1+1,i2,i3),c(i1,i2+1,i3),c(i1,i2,i3+1)
! The following jacobian values should also be defined:
!    ajzmz,ajzzm,ajmzz,ajzzz,ajpzz,ajzpz,ajzzp : aj(i1,i2,i3-1),aj(i1,i2-1,i3),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3),aj(i1,i2,i3+1)
! ==================================================================================================
#beginMacro getCoeffForDxADxPlusDyBDyPlusDzCDz(au, azzm,azmz,amzz,azzz,apzz,azpz,azzp,\
                                                   bzzm,bzmz,bmzz,bzzz,bpzz,bzpz,bzzp,\
                                                   czzm,czmz,cmzz,czzz,cpzz,czpz,czzp )
 au ## 11mzz = ajmzz*( (amzz)*rx(i1-1,i2,i3)*rx(i1-1,i2,i3) + (bmzz)*ry(i1-1,i2,i3)*ry(i1-1,i2,i3) + (cmzz)*rz(i1-1,i2,i3)*rz(i1-1,i2,i3) )
 au ## 11zzz = ajzzz*( (azzz)*rx(i1  ,i2,i3)*rx(i1  ,i2,i3) + (bzzz)*ry(i1  ,i2,i3)*ry(i1  ,i2,i3) + (czzz)*rz(i1  ,i2,i3)*rz(i1  ,i2,i3) )
 au ## 11pzz = ajpzz*( (apzz)*rx(i1+1,i2,i3)*rx(i1+1,i2,i3) + (bpzz)*ry(i1+1,i2,i3)*ry(i1+1,i2,i3) + (cpzz)*rz(i1+1,i2,i3)*rz(i1+1,i2,i3) )
 au ## 11ph = .5*( au ## 11zzz+au ## 11pzz )
 au ## 11mh = .5*( au ## 11zzz+au ## 11mzz )

 au ## 22zmz = ajzmz*( (azmz)*sx(i1,i2-1,i3)*sx(i1,i2-1,i3) + (bzmz)*sy(i1,i2-1,i3)*sy(i1,i2-1,i3) + (czmz)*sz(i1,i2-1,i3)*sz(i1,i2-1,i3) )
 au ## 22zzz = ajzzz*( (azzz)*sx(i1,i2  ,i3)*sx(i1,i2  ,i3) + (bzzz)*sy(i1,i2  ,i3)*sy(i1,i2  ,i3) + (czzz)*sz(i1,i2  ,i3)*sz(i1,i2  ,i3) )
 au ## 22zpz = ajzpz*( (azpz)*sx(i1,i2+1,i3)*sx(i1,i2+1,i3) + (bzpz)*sy(i1,i2+1,i3)*sy(i1,i2+1,i3) + (czpz)*sz(i1,i2+1,i3)*sz(i1,i2+1,i3) )
 au ## 22ph = .5*( au ## 22zzz+au ## 22zpz )
 au ## 22mh = .5*( au ## 22zzz+au ## 22zmz )

 au ## 33zzm = ajzzm*( (azzm)*tx(i1,i2,i3-1)*tx(i1,i2,i3-1) + (bzzm)*ty(i1,i2,i3-1)*ty(i1,i2,i3-1) + (czzm)*tz(i1,i2,i3-1)*tz(i1,i2,i3-1) )
 au ## 33zzz = ajzzz*( (azzz)*tx(i1,i2,i3  )*tx(i1,i2,i3  ) + (bzzz)*ty(i1,i2,i3  )*ty(i1,i2,i3  ) + (czzz)*tz(i1,i2,i3  )*tz(i1,i2,i3  ) )
 au ## 33zzp = ajzzp*( (azzp)*tx(i1,i2,i3+1)*tx(i1,i2,i3+1) + (bzzp)*ty(i1,i2,i3+1)*ty(i1,i2,i3+1) + (czzp)*tz(i1,i2,i3+1)*tz(i1,i2,i3+1) )
 au ## 33ph = .5*( au ## 33zzz+au ## 33zzp )
 au ## 33mh = .5*( au ## 33zzz+au ## 33zzm )


 au ## 12mzz = ajmzz*( (amzz)*rx(i1-1,i2,i3)*sx(i1-1,i2,i3) + (bmzz)*ry(i1-1,i2,i3)*sy(i1-1,i2,i3) + (cmzz)*rz(i1-1,i2,i3)*sz(i1-1,i2,i3) )
 au ## 12zzz = ajzzz*( (azzz)*rx(i1  ,i2,i3)*sx(i1  ,i2,i3) + (bzzz)*ry(i1  ,i2,i3)*sy(i1  ,i2,i3) + (czzz)*rz(i1  ,i2,i3)*sz(i1  ,i2,i3) )
 au ## 12pzz = ajpzz*( (apzz)*rx(i1+1,i2,i3)*sx(i1+1,i2,i3) + (bpzz)*ry(i1+1,i2,i3)*sy(i1+1,i2,i3) + (cpzz)*rz(i1+1,i2,i3)*sz(i1+1,i2,i3) )

 au ## 13mzz = ajmzz*( (amzz)*rx(i1-1,i2,i3)*tx(i1-1,i2,i3) + (bmzz)*ry(i1-1,i2,i3)*ty(i1-1,i2,i3) + (cmzz)*rz(i1-1,i2,i3)*tz(i1-1,i2,i3) )
 au ## 13zzz = ajzzz*( (azzz)*rx(i1  ,i2,i3)*tx(i1  ,i2,i3) + (bzzz)*ry(i1  ,i2,i3)*ty(i1  ,i2,i3) + (czzz)*rz(i1  ,i2,i3)*tz(i1  ,i2,i3) )
 au ## 13pzz = ajpzz*( (apzz)*rx(i1+1,i2,i3)*tx(i1+1,i2,i3) + (bpzz)*ry(i1+1,i2,i3)*ty(i1+1,i2,i3) + (cpzz)*rz(i1+1,i2,i3)*tz(i1+1,i2,i3) )

 au ## 21zmz = ajzmz*( (azmz)*sx(i1,i2-1,i3)*rx(i1,i2-1,i3) + (bzmz)*sy(i1,i2-1,i3)*ry(i1,i2-1,i3) + (czmz)*sz(i1,i2-1,i3)*rz(i1,i2-1,i3) )
 au ## 21zzz = ajzzz*( (azzz)*sx(i1,i2  ,i3)*rx(i1,i2  ,i3) + (bzzz)*sy(i1,i2  ,i3)*ry(i1,i2  ,i3) + (czzz)*sz(i1,i2  ,i3)*rz(i1,i2  ,i3) )
 au ## 21zpz = ajzpz*( (azpz)*sx(i1,i2+1,i3)*rx(i1,i2+1,i3) + (bzpz)*sy(i1,i2+1,i3)*ry(i1,i2+1,i3) + (czpz)*sz(i1,i2+1,i3)*rz(i1,i2+1,i3) )

 au ## 23zmz = ajzmz*( (azmz)*sx(i1,i2-1,i3)*tx(i1,i2-1,i3) + (bzmz)*sy(i1,i2-1,i3)*ty(i1,i2-1,i3) + (czmz)*sz(i1,i2-1,i3)*tz(i1,i2-1,i3) )
 au ## 23zzz = ajzzz*( (azzz)*sx(i1,i2  ,i3)*tx(i1,i2  ,i3) + (bzzz)*sy(i1,i2  ,i3)*ty(i1,i2  ,i3) + (czzz)*sz(i1,i2  ,i3)*tz(i1,i2  ,i3) )
 au ## 23zpz = ajzpz*( (azpz)*sx(i1,i2+1,i3)*tx(i1,i2+1,i3) + (bzpz)*sy(i1,i2+1,i3)*ty(i1,i2+1,i3) + (czpz)*sz(i1,i2+1,i3)*tz(i1,i2+1,i3) )


 au ## 31zzm = ajzzm*( (azzm)*tx(i1,i2,i3-1)*rx(i1,i2,i3-1) + (bzzm)*ty(i1,i2,i3-1)*ry(i1,i2,i3-1) + (czzm)*tz(i1,i2,i3-1)*rz(i1,i2,i3-1) )
 au ## 31zzz = ajzzz*( (azzz)*tx(i1,i2,i3  )*rx(i1,i2,i3  ) + (bzzz)*ty(i1,i2,i3  )*ry(i1,i2,i3  ) + (czzz)*tz(i1,i2,i3  )*rz(i1,i2,i3  ) )
 au ## 31zzp = ajzzp*( (azzp)*tx(i1,i2,i3+1)*rx(i1,i2,i3+1) + (bzzp)*ty(i1,i2,i3+1)*ry(i1,i2,i3+1) + (czzp)*tz(i1,i2,i3+1)*rz(i1,i2,i3+1) )

 au ## 32zzm = ajzzm*( (azzm)*tx(i1,i2,i3-1)*sx(i1,i2,i3-1) + (bzzm)*ty(i1,i2,i3-1)*sy(i1,i2,i3-1) + (czzm)*tz(i1,i2,i3-1)*sz(i1,i2,i3-1) )
 au ## 32zzz = ajzzz*( (azzz)*tx(i1,i2,i3  )*sx(i1,i2,i3  ) + (bzzz)*ty(i1,i2,i3  )*sy(i1,i2,i3  ) + (czzz)*tz(i1,i2,i3  )*sz(i1,i2,i3  ) )
 au ## 32zzp = ajzzp*( (azzp)*tx(i1,i2,i3+1)*sx(i1,i2,i3+1) + (bzzp)*ty(i1,i2,i3+1)*sy(i1,i2,i3+1) + (czzp)*tz(i1,i2,i3+1)*sz(i1,i2,i3+1) )

#endMacro

! ==========================================================================================================================================
! 
! Define the coefficients in the conservative discretization of any mixed or non-mixed derivative: 
! 
!     L = D_X( a*D_Y )  where X=x, y, or z and Y=x, y, or z
!
! Example: 
!  L = div( (aDy,0,0) ) 
!  L = (1/J)*[ Dr( J*(rx,ry,rx).(aDy,0,0)) + Ds(J*(sx,sy,sz).(aDy,0,0))+ Dt(J*(tx,ty,tz).(aDy,0,0))  ]
!    = (1/J)*[ Dr( J*a*rx(ry*Dr + sy*Ds+ ty*Dt) 
!             +Ds( J*a*sx(ry*Dr + sy*Ds+ ty*Dt)
!             +Dt( J*a*tx(ry*Dr + sy*Ds+ ty*Dt) ]
!    = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Dr( a13 Dt) + Ds( a21 Dr) + Ds( a22 Ds) + Ds( a23 Dt) + Dt( a31 Dr) + Dt( a32 Ds) + Dt( a33 Dt) ]
! where 
!  a11 = J ( a rx*ry )
!  a12 = J ( a rx*sy )
!  a13 = J ( a rx*ty )
!  a21 = J ( a sx*ry )
!  a22 = J ( a sx*sy )
!  a23 = J ( a sx*ty )
!  a31 = J ( a tx*ry )
!  a32 = J ( a tx*sy )
!  a33 = J ( a tx*ty )
!  a = a(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   X,Y : X=[x,y,z] and Y=[x,y,z] to compute the coeffcients of D_X( a D_Y )
!   azzm,azmz,amzz,azzz,apzz,azpz,azzp : a(i1,i2,i3-1),a(i1,i2-1,i3),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3),a(i1,i2,i3+1)
! The following jacobian values should also be defined:
!    ajzmz,ajzzm,ajmzz,ajzzz,ajpzz,ajzpz,ajzzp : aj(i1,i2,i3-1),aj(i1,i2-1,i3),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3),aj(i1,i2,i3+1)
! ===========================================================================================================================================
#beginMacro getCoeffForDxADy3d(au, X, Y, azzm,azmz,amzz,azzz,apzz,azpz,azzp )

 au ## 11mzz = ajmzz*( (amzz)*r ## X(i1-1,i2,i3)*r ## Y(i1-1,i2,i3) )
 au ## 11zzz = ajzzz*( (azzz)*r ## X(i1  ,i2,i3)*r ## Y(i1  ,i2,i3) )
 au ## 11pzz = ajpzz*( (apzz)*r ## X(i1+1,i2,i3)*r ## Y(i1+1,i2,i3) )
 au ## 11ph = .5*( au ## 11zzz+au ## 11pzz )
 au ## 11mh = .5*( au ## 11zzz+au ## 11mzz )

 au ## 22zmz = ajzmz*( (azmz)*s ## X(i1,i2-1,i3)*s ## Y(i1,i2-1,i3) )
 au ## 22zzz = ajzzz*( (azzz)*s ## X(i1,i2  ,i3)*s ## Y(i1,i2  ,i3) )
 au ## 22zpz = ajzpz*( (azpz)*s ## X(i1,i2+1,i3)*s ## Y(i1,i2+1,i3) )
 au ## 22ph = .5*( au ## 22zzz+au ## 22zpz )
 au ## 22mh = .5*( au ## 22zzz+au ## 22zmz )

 au ## 33zzm = ajzzm*( (azzm)*t ## X(i1,i2,i3-1)*t ## Y(i1,i2,i3-1) )
 au ## 33zzz = ajzzz*( (azzz)*t ## X(i1,i2,i3  )*t ## Y(i1,i2,i3  ) )
 au ## 33zzp = ajzzp*( (azzp)*t ## X(i1,i2,i3+1)*t ## Y(i1,i2,i3+1) )
 au ## 33ph = .5*( au ## 33zzz+au ## 33zzp )
 au ## 33mh = .5*( au ## 33zzz+au ## 33zzm )


 au ## 12mzz = ajmzz*( (amzz)*r ## X(i1-1,i2,i3)*s ## Y(i1-1,i2,i3) )
 au ## 12zzz = ajzzz*( (azzz)*r ## X(i1  ,i2,i3)*s ## Y(i1  ,i2,i3) )
 au ## 12pzz = ajpzz*( (apzz)*r ## X(i1+1,i2,i3)*s ## Y(i1+1,i2,i3) )

 au ## 13mzz = ajmzz*( (amzz)*r ## X(i1-1,i2,i3)*t ## Y(i1-1,i2,i3) )
 au ## 13zzz = ajzzz*( (azzz)*r ## X(i1  ,i2,i3)*t ## Y(i1  ,i2,i3) )
 au ## 13pzz = ajpzz*( (apzz)*r ## X(i1+1,i2,i3)*t ## Y(i1+1,i2,i3) )

 au ## 21zmz = ajzmz*( (azmz)*s ## X(i1,i2-1,i3)*r ## Y(i1,i2-1,i3) )
 au ## 21zzz = ajzzz*( (azzz)*s ## X(i1,i2  ,i3)*r ## Y(i1,i2  ,i3) )
 au ## 21zpz = ajzpz*( (azpz)*s ## X(i1,i2+1,i3)*r ## Y(i1,i2+1,i3) )

 au ## 23zmz = ajzmz*( (azmz)*s ## X(i1,i2-1,i3)*t ## Y(i1,i2-1,i3) )
 au ## 23zzz = ajzzz*( (azzz)*s ## X(i1,i2  ,i3)*t ## Y(i1,i2  ,i3) )
 au ## 23zpz = ajzpz*( (azpz)*s ## X(i1,i2+1,i3)*t ## Y(i1,i2+1,i3) )


 au ## 31zzm = ajzzm*( (azzm)*t ## X(i1,i2,i3-1)*r ## Y(i1,i2,i3-1) )
 au ## 31zzz = ajzzz*( (azzz)*t ## X(i1,i2,i3  )*r ## Y(i1,i2,i3  ) )
 au ## 31zzp = ajzzp*( (azzp)*t ## X(i1,i2,i3+1)*r ## Y(i1,i2,i3+1) )

 au ## 32zzm = ajzzm*( (azzm)*t ## X(i1,i2,i3-1)*s ## Y(i1,i2,i3-1) )
 au ## 32zzz = ajzzz*( (azzz)*t ## X(i1,i2,i3  )*s ## Y(i1,i2,i3  ) )
 au ## 32zzp = ajzzp*( (azzp)*t ## X(i1,i2,i3+1)*s ## Y(i1,i2,i3+1) )

#endMacro



! ================================================================================================================================================
!    Assign the coefficients for a component of the conservative 3D discretization of the div(tensor grad) operator
! 
!  L  = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Dr( a13 Dt) + Ds( a21 Dr) + Ds( a22 Ds) + Ds( a23 Dt) + Dt( a31 Dr) + Dt( a32 Ds) + Dt( a33 Dt) ]
!     Dr( a11 Dr)u = a11pzz*( u(i1+1)-u(i1) ) - a11mzz*( u(i1)-u(i1-1))
!                  = a11pzz*u(i1+1) -(a11pzz+a11mzz)*u(i1) +a11mzz*u(i1-1) 
!     Dr( a13 Dt ) = D0r( a13 D0t ) = a13pzz*( u(i1+1,i2,i3+1)-u(i1+1,i2,i3-1)) - a13mzz*( u(i1-1,i2,i3+1)-u(i1-1,i2,i3-1) )
!     Ds( a23 Dt ) = D0s( a23 D0t ) = a23zpz*( u(i1,i2+1,i3+1)-u(i1,i2+1,i3-1)) - a23zmz*( u(i1,i2-1,i3+1)-u(i1,i2-1,i3-1) )
!
!     Dt( a31 Dr ) = D0t( a31 D0r ) = a31zzp*( u(i1+1,i2,i3+1)-u(i1-1,i2,i3+1)) - a31zzm*( u(i1+1,i2,i3-1)-u(i1-1,i2,i3-1) )
!     Dt( a32 Ds ) = D0t( a32 D0s ) = a32zzp*( u(i1,i2+1,i3+1)-u(i1,i2-1,i3+1)) - a32zzm*( u(i1,i2+1,i3-1)-u(i1,i2-1,i3-1) )
!
! Macro args:
!   cmp,eqn : component and equation
!   A : generic name for coefficients
! =================================================================================================================================================
#beginMacro setDivTensorGradCoeff3d(cmp,eqn,A)
  coeff(MCE(-1,-1,-1,cmp,eqn),i1,i2,i3)= 0.
  coeff(MCE( 0,-1,-1,cmp,eqn),i1,i2,i3)=         A ## 23zmz+A ## 32zzm
  coeff(MCE( 1,-1,-1,cmp,eqn),i1,i2,i3)= 0.
  coeff(MCE(-1, 0,-1,cmp,eqn),i1,i2,i3)=  A ## 13mzz+A ## 31zzm
  coeff(MCE( 0, 0,-1,cmp,eqn),i1,i2,i3)=                                    A ## 33mh
  coeff(MCE( 1, 0,-1,cmp,eqn),i1,i2,i3)= -A ## 13pzz-A ## 31zzm
  coeff(MCE(-1, 1,-1,cmp,eqn),i1,i2,i3)= 0.
  coeff(MCE( 0, 1,-1,cmp,eqn),i1,i2,i3)=        -A ## 23zpz-A ## 32zzm
  coeff(MCE( 1, 1,-1,cmp,eqn),i1,i2,i3)= 0.

  coeff(MCE(-1,-1, 0,cmp,eqn),i1,i2,i3)=  A ## 12mzz+A ## 21zmz
  coeff(MCE( 0,-1, 0,cmp,eqn),i1,i2,i3)=                      A ## 22mh 
  coeff(MCE( 1,-1, 0,cmp,eqn),i1,i2,i3)= -A ## 12pzz-A ## 21zmz 
  coeff(MCE(-1, 0, 0,cmp,eqn),i1,i2,i3)=         A ## 11mh
  coeff(MCE( 0, 0, 0,cmp,eqn),i1,i2,i3)= -A ## 11ph-A ## 11mh -A ## 22ph -A ## 22mh -A ## 33ph -A ## 33mh
  coeff(MCE( 1, 0, 0,cmp,eqn),i1,i2,i3)=  A ## 11ph
  coeff(MCE(-1, 1, 0,cmp,eqn),i1,i2,i3)= -A ## 12mzz-A ## 21zpz
  coeff(MCE( 0, 1, 0,cmp,eqn),i1,i2,i3)=               A ## 22ph
  coeff(MCE( 1, 1, 0,cmp,eqn),i1,i2,i3)=  A ## 12pzz+A ## 21zpz

  coeff(MCE(-1,-1, 1,cmp,eqn),i1,i2,i3)= 0.
  coeff(MCE( 0,-1, 1,cmp,eqn),i1,i2,i3)=       -A ## 23zmz-A ## 32zzp
  coeff(MCE( 1,-1, 1,cmp,eqn),i1,i2,i3)= 0.
  coeff(MCE(-1, 0, 1,cmp,eqn),i1,i2,i3)= -A ## 13mzz-A ## 31zzp
  coeff(MCE( 0, 0, 1,cmp,eqn),i1,i2,i3)=                            A ## 33ph
  coeff(MCE( 1, 0, 1,cmp,eqn),i1,i2,i3)=  A ## 13pzz+A ## 31zzp
  coeff(MCE(-1, 1, 1,cmp,eqn),i1,i2,i3)= 0.
  coeff(MCE( 0, 1, 1,cmp,eqn),i1,i2,i3)=        A ## 23zpz+A ## 32zzp 
  coeff(MCE( 1, 1, 1,cmp,eqn),i1,i2,i3)= 0.
#endMacro


! =======================================================================================================
! This macro scaled the coefficients that appear in the discretization of the div(tensor grad) operator
! =======================================================================================================
#beginMacro scaleCoefficients3d( a11ph,a11mh,a22ph,a22mh,a33ph,a33mh,a12pzz,a12mzz,a13pzz,a13mzz,a21zpz,a21zmz,a23zpz,a23zmz,a31zzp,a31zzm,a32zzp,a32zzm )
 a11ph=a11ph*dr0i
 a11mh=a11mh*dr0i
 a22ph=a22ph*dr1i
 a22mh=a22mh*dr1i
 a33ph=a33ph*dr2i
 a33mh=a33mh*dr2i

 a12pzz=a12pzz*dr0dr1
 a12mzz=a12mzz*dr0dr1
 a13pzz=a13pzz*dr0dr2
 a13mzz=a13mzz*dr0dr2

 a21zpz=a21zpz*dr0dr1
 a21zmz=a21zmz*dr0dr1
 a23zpz=a23zpz*dr1dr2
 a23zmz=a23zmz*dr1dr2

 a31zzp=a31zzp*dr0dr2
 a31zzm=a31zzm*dr0dr2
 a32zzp=a32zzp*dr1dr2
 a32zzm=a32zzm*dr1dr2

#endMacro
