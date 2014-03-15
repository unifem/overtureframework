
// =============================================================
//  Assign initial conditions for the Gaussian pulse
//
// GRIDTYPE: curvilinear or rectangular
// ==============================================================
#beginMacro assignGaussianPulseInitialConditions(GRIDTYPE)
if( mg.numberOfDimensions()==2 )
{
  J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
  J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
  J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
  FOR_3D(i1,i2,i3,J1,J2,J3)
  {
    #If #GRIDTYPE eq "curvilinear"
      real xe = XEP(i1,i2,i3,0)-x0;
      real ye = XEP(i1,i2,i3,1)-y0;
    #Else
      real xe = X0(i1,i2,i3)-x0;
      real ye = X1(i1,i2,i3)-y0;
    #End

    real temp=exp( -pow(beta*(xe*xe+ye*ye),exponent) );

    UEX(i1,i2,i3) =c0*UEX(i1,i2,i3)-ye*scale*temp;   // Ex = -C (Hz).y
    UEY(i1,i2,i3) =c0*UEY(i1,i2,i3)+xe*scale*temp;   // Ey =  C (Hz).x

    UMEX(i1,i2,i3)=UEX(i1,i2,i3);
    UMEY(i1,i2,i3)=UEY(i1,i2,i3);

  }

  J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
  J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
  J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
  FOR_3(i1,i2,i3,J1,J2,J3)
  {
    #If #GRIDTYPE eq "curvilinear"
      real xh = XHP(i1,i2,i3,0)-x0;
      real yh = XHP(i1,i2,i3,1)-y0;
    #Else
      real xh = X0(i1,i2,i3)-x0;
      real yh = X1(i1,i2,i3)-y0;
    #End

    real temp=exp( -pow(beta*(xh*xh+yh*yh),exponent) );

    UHZ(i1,i2,i3) =c0*UHZ(i1,i2,i3) + exp( -pow(beta*(xh*xh+yh*yh),exponent) ); 
    UMHZ(i1,i2,i3)=UHZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
  }
}
else
{
  J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
  J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
  J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
  FOR_3D(i1,i2,i3,J1,J2,J3)
  {
    #If #GRIDTYPE eq "curvilinear"
      real xe = XEP(i1,i2,i3,0)-x0;
      real ye = XEP(i1,i2,i3,1)-y0;
      real ze = XEP(i1,i2,i3,2)-z0;

    #Else
      real xe = X0(i1,i2,i3)-x0;
      real ye = X1(i1,i2,i3)-y0;
      real ze = X2(i1,i2,i3)-z0;

    #End

      // XXX NONE of the 3D part of this macro has really been implemented for staggered grids
    real rsq = xe*xe+ye*ye+ze*ze;

    // E = curl( phi ), phi = (phix,phiy,phiz)
    // real phix = constant*exp( -pow(beta*rsq,exponent) );

    real dphix = scale*exp( -pow(beta*rsq,exponent) ); // exponent*pow(beta*rsq,exponent-1.);
    real dphiy = dphix;
    real dphiz = dphix; 

    UEX(i1,i2,i3)=c0*UEX(i1,i2,i3)+  ye*dphiz -ze*dphiy;        //  (phiz).y - (phiy).z
    UEY(i1,i2,i3)=c0*UEX(i1,i2,i3)+  ze*dphix -xe*dphiz ;        // 
    UEZ(i1,i2,i3)=c0*UEX(i1,i2,i3)+  xe*dphiy -ye*dphix ;
    UMEX(i1,i2,i3)=UEX(i1,i2,i3);
    UMEY(i1,i2,i3)=UEY(i1,i2,i3);
    UMEZ(i1,i2,i3)=UEZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
  }
  
  
  J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
  J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
  J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
  FOR_3(i1,i2,i3,J1,J2,J3)
  {

#If #GRIDTYPE eq "curvilinear"
      real xh = XHP(i1,i2,i3,0)-x0;
      real yh = XHP(i1,i2,i3,1)-y0;
      real zh = XHP(i1,i2,i3,2)-z0;
#Else
      real xe = X0(i1,i2,i3)-x0;
      real ye = X1(i1,i2,i3)-y0;
      real ze = X2(i1,i2,i3)-z0;

      real xh = xe;// Cartesian grid stuff not implemented for staggered grids yet (Yee, DSI)
      real yh = ye;
      real zh = ze;
#End

      // XXX NONE of the 3D part of this macro has really been properly implemented for staggered grids
    real rsq = xh*xh+yh*yh+zh*zh;

    // E = curl( phi ), phi = (phix,phiy,phiz)
    // real phix = constant*exp( -pow(beta*rsq,exponent) );

    real dphix = scale*exp( -pow(beta*rsq,exponent) ); // exponent*pow(beta*rsq,exponent-1.);
    real dphiy = dphix;
    real dphiz = dphix; 

    UHX(i1,i2,i3)=c0*UHX(i1,i2,i3) +  dphix;
    UHY(i1,i2,i3)=c0*UHY(i1,i2,i3) +  dphiy;
    UHZ(i1,i2,i3)=c0*UHZ(i1,i2,i3) +  dphiz;
    UMHX(i1,i2,i3)=UHX(i1,i2,i3);
    UMHY(i1,i2,i3)=UHY(i1,i2,i3);
    UMHZ(i1,i2,i3)=UHZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
  }
}
#endMacro
