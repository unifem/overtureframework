      ! Face centered derivatives for the self-adjoint artificial diffusion
      !     p=plus, m=minus, z=zero
      ! Rectangular grid
      uxmzzR(i1,i2,i3,c)=(u(i1,i2,i3,c)-u(i1-1,i2,i3,c))*dxi
      uymzzR(i1,i2,i3,c)=(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,i3,c))*dyi*.25
      uzmzzR(i1,i2,i3,c)=(u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+u(i1-1,i2,i3+1,c)-u(i1-1,i2,i3-1,c))*dzi*.25

      uxzmzR(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,i3,c))*dxi*.25
      uyzmzR(i1,i2,i3,c)=(u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*dyi
      uzzmzR(i1,i2,i3,c)=(u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+u(i1,i2-1,i3+1,c)-u(i1,i2-1,i3-1,c))*dzi*.25

      uxzzmR(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+u(i1+1,i2,i3-1,c)-u(i1-1,i2,i3-1,c))*dxi*.25
      uyzzmR(i1,i2,i3,c)=(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+u(i1,i2+1,i3-1,c)-u(i1,i2-1,i3-1,c))*dyi*.25
      uzzzmR(i1,i2,i3,c)=(u(i1,i2,i3,c)-u(i1,i2,i3-1,c))*dzi

      ! curvilinear grid
      udmzC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,0,m)+rsxy(i1-1,i2,i3,0,m))*(u(i1,i2,i3,c)-u(i1-1,i2  ,i3,c))*dr2i +
     &                    (rsxy(i1,i2,i3,1,m)+rsxy(i1-1,i2,i3,1,m))*(
     &                          u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,i3,c))*dsi*.125
      udzmC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,1,m)+rsxy(i1,i2-1,i3,1,m))*(u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*ds2i +
     &                    (rsxy(i1,i2,i3,0,m)+rsxy(i1,i2-1,i3,0,m))*(
     &                          u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,i3,c))*dri*.125

      udmzzC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,0,m)+rsxy(i1-1,i2,i3,0,m))*(u(i1,i2,i3,c)-u(i1-1,i2  ,i3,c))*dr2i +
     &                     (rsxy(i1,i2,i3,1,m)+rsxy(i1-1,i2,i3,1,m))*(
     &                          u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,i3,c))*dsi*.125+
     &                     (rsxy(i1,i2,i3,2,m)+rsxy(i1-1,i2,i3,2,m))*(
     &                          u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+ u(i1-1,i2,i3+1,c)-u(i1-1,i2,i3-1,c))*dti*.125
      udzmzC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,1,m)+rsxy(i1,i2-1,i3,1,m))*(u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*ds2i +
     &                     (rsxy(i1,i2,i3,0,m)+rsxy(i1,i2-1,i3,0,m))*(
     &                          u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,i3,c))*dri*.125+
     &                     (rsxy(i1,i2,i3,2,m)+rsxy(i1,i2-1,i3,2,m))*(
     &                          u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+ u(i1,i2-1,i3+1,c)-u(i1,i2-1,i3-1,c))*dti*.125

      udzzmC(i1,i2,i3,m,c)=(rsxy(i1,i2,i3,2,m)+rsxy(i1,i2,i3-1,2,m))*(u(i1,i2,i3,c)-u(i1,i2,i3-1,c))*dt2i +
     &                     (rsxy(i1,i2,i3,0,m)+rsxy(i1,i2,i3-1,0,m))*(
     &                          u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2,i3-1,c)-u(i1-1,i2,i3-1,c))*dri*.125+
     &                     (rsxy(i1,i2,i3,1,m)+rsxy(i1,i2,i3-1,1,m))*(
     &                          u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1,i2+1,i3-1,c)-u(i1,i2-1,i3-1,c))*dsi*.125

      ! Coefficients of the artificial diffusion for the momentum equations
      ! 2D - rectangular
      admzR(i1,i2,i3)=ad21+cd22*( abs(uxmzzR(i1,i2,i3,uc))+abs(uxmzzR(i1,i2,i3,vc))+
     &                            abs(uymzzR(i1,i2,i3,uc))+abs(uymzzR(i1,i2,i3,vc)) )

      adzmR(i1,i2,i3)=ad21+cd22*( abs(uxzmzR(i1,i2,i3,uc))+abs(uxzmzR(i1,i2,i3,vc))+
     &                            abs(uyzmzR(i1,i2,i3,uc))+abs(uyzmzR(i1,i2,i3,vc)) )

      ! 3D
      admzzR(i1,i2,i3)=ad21+cd22*( abs(uxmzzR(i1,i2,i3,uc))+abs(uxmzzR(i1,i2,i3,vc))+abs(uxmzzR(i1,i2,i3,wc))+
     &                             abs(uymzzR(i1,i2,i3,uc))+abs(uymzzR(i1,i2,i3,vc))+abs(uymzzR(i1,i2,i3,wc))+
     &                             abs(uzmzzR(i1,i2,i3,uc))+abs(uzmzzR(i1,i2,i3,vc))+abs(uzmzzR(i1,i2,i3,wc)) )

      adzmzR(i1,i2,i3)=ad21+cd22*( abs(uxzmzR(i1,i2,i3,uc))+abs(uxzmzR(i1,i2,i3,vc))+abs(uxzmzR(i1,i2,i3,wc))+
     &                             abs(uyzmzR(i1,i2,i3,uc))+abs(uyzmzR(i1,i2,i3,vc))+abs(uyzmzR(i1,i2,i3,wc))+
     &                             abs(uzzmzR(i1,i2,i3,uc))+abs(uzzmzR(i1,i2,i3,vc))+abs(uzzmzR(i1,i2,i3,wc)) )

      adzzmR(i1,i2,i3)=ad21+cd22*( abs(uxzzmR(i1,i2,i3,uc))+abs(uxzzmR(i1,i2,i3,vc))+abs(uxzzmR(i1,i2,i3,wc))+
     &                             abs(uyzzmR(i1,i2,i3,uc))+abs(uyzzmR(i1,i2,i3,vc))+abs(uyzzmR(i1,i2,i3,wc))+
     &                             abs(uzzzmR(i1,i2,i3,uc))+abs(uzzzmR(i1,i2,i3,vc))+abs(uzzzmR(i1,i2,i3,wc)) )
      ! 2D - curvilinear
      admzC(i1,i2,i3)=ad21+cd22*( abs(udmzC(i1,i2,i3,0,uc))+abs(udmzC(i1,i2,i3,0,vc))+
     &                            abs(udmzC(i1,i2,i3,1,uc))+abs(udmzC(i1,i2,i3,1,vc)) )

      adzmC(i1,i2,i3)=ad21+cd22*( abs(udzmC(i1,i2,i3,0,uc))+abs(udzmC(i1,i2,i3,0,vc))+
     &                            abs(udzmC(i1,i2,i3,1,uc))+abs(udzmC(i1,i2,i3,1,vc)) )

      ! 3D
      admzzC(i1,i2,i3)=ad21+cd22*( abs(udmzzC(i1,i2,i3,0,uc))+abs(udmzzC(i1,i2,i3,0,vc))+abs(udmzzC(i1,i2,i3,0,wc))+
     &                             abs(udmzzC(i1,i2,i3,1,uc))+abs(udmzzC(i1,i2,i3,1,vc))+abs(udmzzC(i1,i2,i3,1,wc))+
     &                             abs(udmzzC(i1,i2,i3,2,uc))+abs(udmzzC(i1,i2,i3,2,vc))+abs(udmzzC(i1,i2,i3,2,wc)) )

      adzmzC(i1,i2,i3)=ad21+cd22*( abs(udzmzC(i1,i2,i3,0,uc))+abs(udzmzC(i1,i2,i3,0,vc))+abs(udzmzC(i1,i2,i3,0,wc))+
     &                             abs(udzmzC(i1,i2,i3,1,uc))+abs(udzmzC(i1,i2,i3,1,vc))+abs(udzmzC(i1,i2,i3,1,wc))+
     &                             abs(udzmzC(i1,i2,i3,2,uc))+abs(udzmzC(i1,i2,i3,2,vc))+abs(udzmzC(i1,i2,i3,2,wc)) )

      adzzmC(i1,i2,i3)=ad21+cd22*( abs(udzzmC(i1,i2,i3,0,uc))+abs(udzzmC(i1,i2,i3,0,vc))+abs(udzzmC(i1,i2,i3,0,wc))+
     &                             abs(udzzmC(i1,i2,i3,1,uc))+abs(udzzmC(i1,i2,i3,1,vc))+abs(udzzmC(i1,i2,i3,1,wc))+
     &                             abs(udzzmC(i1,i2,i3,2,uc))+abs(udzzmC(i1,i2,i3,2,vc))+abs(udzzmC(i1,i2,i3,2,wc)) )

      ! Coefficients of the artificial diffusion for the SA turbulence model
      ! 2D - rectangular
      admzRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxmzzR(i1,i2,i3,nc))+abs(uymzzR(i1,i2,i3,nc)) )
      adzmRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxzmzR(i1,i2,i3,nc))+abs(uyzmzR(i1,i2,i3,nc)) )
      ! 3D
      admzzRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxmzzR(i1,i2,i3,nc))+abs(uymzzR(i1,i2,i3,nc))+abs(uzmzzR(i1,i2,i3,nc)) )
      adzmzRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxzmzR(i1,i2,i3,nc))+abs(uyzmzR(i1,i2,i3,nc))+abs(uzzmzR(i1,i2,i3,nc)) )
      adzzmRSA(i1,i2,i3)=ad21n+cd22n*( abs(uxzzmR(i1,i2,i3,nc))+abs(uyzzmR(i1,i2,i3,nc))+abs(uzzzmR(i1,i2,i3,nc)) )
      ! 2D - curvilinear
      admzCSA(i1,i2,i3)=ad21n+cd22n*( abs(udmzC(i1,i2,i3,0,nc))+abs(udmzC(i1,i2,i3,1,nc)) )
      adzmCSA(i1,i2,i3)=ad21n+cd22n*( abs(udzmC(i1,i2,i3,0,nc))+abs(udzmC(i1,i2,i3,1,nc)) )
      ! 3D
      admzzCSA(i1,i2,i3)=ad21n+cd22n*( abs(udmzzC(i1,i2,i3,0,nc))+abs(udmzzC(i1,i2,i3,1,nc))+abs(udmzzC(i1,i2,i3,2,nc)))
      adzmzCSA(i1,i2,i3)=ad21n+cd22n*( abs(udzmzC(i1,i2,i3,0,nc))+abs(udzmzC(i1,i2,i3,1,nc))+abs(udzmzC(i1,i2,i3,2,nc)))
      adzzmCSA(i1,i2,i3)=ad21n+cd22n*( abs(udzzmC(i1,i2,i3,0,nc))+abs(udzzmC(i1,i2,i3,1,nc))+abs(udzzmC(i1,i2,i3,2,nc)))


      ! Here are the parts of the artificial diffusion that are explicit (appear on the RHS)
      adE0(i1,i2,i3,c) = cdzm*u(i1,i2-1,i3,c)+cdzp*u(i1,i2+1,i3,c)
      adE1(i1,i2,i3,c) = cdmz*u(i1-1,i2,i3,c)+cdpz*u(i1+1,i2,i3,c)
      adE2(i1,i2,i3,c) = 0.

      adE3d0(i1,i2,i3,c) = cdzmz*u(i1,i2-1,i3,c)+cdzpz*u(i1,i2+1,i3,c)+cdzzm*u(i1,i2,i3-1,c)+cdzzp*u(i1,i2,i3+1,c)
      adE3d1(i1,i2,i3,c) = cdmzz*u(i1-1,i2,i3,c)+cdpzz*u(i1+1,i2,i3,c)+cdzzm*u(i1,i2,i3-1,c)+cdzzp*u(i1,i2,i3+1,c)
      adE3d2(i1,i2,i3,c) = cdmzz*u(i1-1,i2,i3,c)+cdpzz*u(i1+1,i2,i3,c)+cdzmz*u(i1,i2-1,i3,c)+cdzpz*u(i1,i2+1,i3,c)

      ad2f(i1,i2,i3,m)= -cdDiag*u(i1,i2,i3,m)+cdmz*u(i1-1,i2,i3,m)+cdpz*u(i1+1,i2,i3,m)+
     &                                        cdzm*u(i1,i2-1,i3,m)+cdzp*u(i1,i2+1,i3,m)   

      ad3f(i1,i2,i3,m)= -cdDiag*u(i1,i2,i3,m)+cdmzz*u(i1-1,i2,i3,m)+cdpzz*u(i1+1,i2,i3,m)+
     &                                        cdzmz*u(i1,i2-1,i3,m)+cdzpz*u(i1,i2+1,i3,m)+
     &                                        cdzzm*u(i1,i2,i3-1,m)+cdzzp*u(i1,i2,i3+1,m)

      ! Here are the full artificial diffusion terms 
      adSelfAdjoint2dR(i1,i2,i3,c)=admzR(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                             admzR(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmR(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmR(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dR(i1,i2,i3,c)=admzzR(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                             admzzR(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmzR(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmzR(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     &                             adzzmR(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+  
     &                             adzzmR(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))


      adSelfAdjoint2dC(i1,i2,i3,c)=admzC(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                             admzC(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmC(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmC(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dC(i1,i2,i3,c)=admzzC(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                             admzzC(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmzC(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                             adzmzC(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     &                             adzzmC(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+  
     &                             adzzmC(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))

      ! Here are versions for the turbulence model
      adSelfAdjoint2dRSA(i1,i2,i3,c)=admzRSA(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                               admzRSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmRSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmRSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dRSA(i1,i2,i3,c)=admzzRSA(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                               admzzRSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmzRSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmzRSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     &                               adzzmRSA(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+  
     &                               adzzmRSA(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))


      adSelfAdjoint2dCSA(i1,i2,i3,c)=admzCSA(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                               admzCSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmCSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmCSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))

      adSelfAdjoint3dCSA(i1,i2,i3,c)=admzzCSA(i1  ,i2  ,i3  )*(u(i1-1,i2,i3,c)-u(i1,i2,i3,c))+
     &                               admzzCSA(i1+1,i2  ,i3  )*(u(i1+1,i2,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmzCSA(i1  ,i2  ,i3  )*(u(i1,i2-1,i3,c)-u(i1,i2,i3,c))+  
     &                               adzmzCSA(i1  ,i2+1,i3  )*(u(i1,i2+1,i3,c)-u(i1,i2,i3,c))+
     &                               adzzmCSA(i1  ,i2  ,i3  )*(u(i1,i2,i3-1,c)-u(i1,i2,i3,c))+  
     &                               adzzmCSA(i1  ,i2  ,i3+1)*(u(i1,i2,i3+1,c)-u(i1,i2,i3,c))


