#beginMacro definePlaneMaterialInterfaceMacros(LANG)

#If #LANG eq "FORTRAN"
c ------------ macros for the plane material interface -------------------------

c Incident + reflected: 
#defineMacro PMIex(x,y,z,t) (pmc( 0)*cos(twoPi*(pmc(19)*(x-pmc(28))+pmc(20)*(y-pmc(29))+pmc(21)*(z-pmc(30))-pmc(18)*(t))) + \
                             pmc( 1)*cos(twoPi*(pmc(22)*(x-pmc(28))+pmc(23)*(y-pmc(29))+pmc(24)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIey(x,y,z,t) (pmc( 2)*cos(twoPi*(pmc(19)*(x-pmc(28))+pmc(20)*(y-pmc(29))+pmc(21)*(z-pmc(30))-pmc(18)*(t))) + \
                             pmc( 3)*cos(twoPi*(pmc(22)*(x-pmc(28))+pmc(23)*(y-pmc(29))+pmc(24)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIez(x,y,z,t) (pmc( 4)*cos(twoPi*(pmc(19)*(x-pmc(28))+pmc(20)*(y-pmc(29))+pmc(21)*(z-pmc(30))-pmc(18)*(t))) + \
                             pmc( 5)*cos(twoPi*(pmc(22)*(x-pmc(28))+pmc(23)*(y-pmc(29))+pmc(24)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIhx(x,y,z,t) (pmc( 6)*cos(twoPi*(pmc(19)*(x-pmc(28))+pmc(20)*(y-pmc(29))+pmc(21)*(z-pmc(30))-pmc(18)*(t))) + \
                             pmc( 7)*cos(twoPi*(pmc(22)*(x-pmc(28))+pmc(23)*(y-pmc(29))+pmc(24)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIhy(x,y,z,t) (pmc( 8)*cos(twoPi*(pmc(19)*(x-pmc(28))+pmc(20)*(y-pmc(29))+pmc(21)*(z-pmc(30))-pmc(18)*(t))) + \
                             pmc( 9)*cos(twoPi*(pmc(22)*(x-pmc(28))+pmc(23)*(y-pmc(29))+pmc(24)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIhz(x,y,z,t) (pmc(10)*cos(twoPi*(pmc(19)*(x-pmc(28))+pmc(20)*(y-pmc(29))+pmc(21)*(z-pmc(30))-pmc(18)*(t))) + \
                             pmc(11)*cos(twoPi*(pmc(22)*(x-pmc(28))+pmc(23)*(y-pmc(29))+pmc(24)*(z-pmc(30))-pmc(18)*(t))))

c Transmitted: 
#defineMacro PMITex(x,y,z,t) (pmc(12)*cos(twoPi*(pmc(25)*(x-pmc(28))+pmc(26)*(y-pmc(29))+pmc(27)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMITey(x,y,z,t) (pmc(13)*cos(twoPi*(pmc(25)*(x-pmc(28))+pmc(26)*(y-pmc(29))+pmc(27)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMITez(x,y,z,t) (pmc(14)*cos(twoPi*(pmc(25)*(x-pmc(28))+pmc(26)*(y-pmc(29))+pmc(27)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIThx(x,y,z,t) (pmc(15)*cos(twoPi*(pmc(25)*(x-pmc(28))+pmc(26)*(y-pmc(29))+pmc(27)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIThy(x,y,z,t) (pmc(16)*cos(twoPi*(pmc(25)*(x-pmc(28))+pmc(26)*(y-pmc(29))+pmc(27)*(z-pmc(30))-pmc(18)*(t))))
#defineMacro PMIThz(x,y,z,t) (pmc(17)*cos(twoPi*(pmc(25)*(x-pmc(28))+pmc(26)*(y-pmc(29))+pmc(27)*(z-pmc(30))-pmc(18)*(t))))

#Elif #LANG eq "C"

#defineMacro PMIex(x,y,z,t) (pmc[ 0]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                             pmc[ 1]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIey(x,y,z,t) (pmc[ 2]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                             pmc[ 3]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIez(x,y,z,t) (pmc[ 4]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                             pmc[ 5]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIhx(x,y,z,t) (pmc[ 6]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                             pmc[ 7]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIhy(x,y,z,t) (pmc[ 8]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                             pmc[ 9]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIhz(x,y,z,t) (pmc[10]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                             pmc[11]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))

#defineMacro PMITex(x,y,z,t) (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMITey(x,y,z,t) (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMITez(x,y,z,t) (pmc[14]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThx(x,y,z,t) (pmc[15]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThy(x,y,z,t) (pmc[16]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThz(x,y,z,t) (pmc[17]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))



#Else
   ERROR
#End

#endMacro

#beginMacro setPlaneMaterialInterfaceMacro(OPTION,J1,J2,J3)
// ------------ macro for the plane material interface -------------------------
// OPTION: initialCondition, error, boundaryCondition
// -----------------------------------------------------------------------------
  int i1,i2,i3;

  real tm=t-dt,x,y,z;

  if( numberOfDimensions==2 )
  {
   z=0.;
   if( grid==0 )
   { // incident plus reflected wave.
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      x = XEP(i1,i2,i3,0);
      y = XEP(i1,i2,i3,1);

      real u1 = PMIex(x,y,z,t);
      real u2 = PMIey(x,y,z,t);
      real u3 = PMIhz(x,y,z,t);

      #If #OPTION eq "initialCondition"
       UEX(i1,i2,i3)= u1;
       UEY(i1,i2,i3)= u2;
       UHZ(i1,i2,i3)= u3;
 
       UMEX(i1,i2,i3)= PMIex(x,y,z,tm);
       UMEY(i1,i2,i3)= PMIey(x,y,z,tm);
       UMHZ(i1,i2,i3)= PMIhz(x,y,z,tm);
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,hz)= u3;
      #End
    }
   }
   else
   {
    // transmitted wave
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      x = XEP(i1,i2,i3,0);
      y = XEP(i1,i2,i3,1);

      real u1 = PMITex(x,y,z,t);
      real u2 = PMITey(x,y,z,t);
      real u3 = PMIThz(x,y,z,t);

      #If #OPTION eq "initialCondition"
       UEX(i1,i2,i3)= u1;
       UEY(i1,i2,i3)= u2;
       UHZ(i1,i2,i3)= u3;
 
       UMEX(i1,i2,i3)= PMITex(x,y,z,tm);
       UMEY(i1,i2,i3)= PMITey(x,y,z,tm);
       UMHZ(i1,i2,i3)= PMIThz(x,y,z,tm);
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,hz)= u3;
      #End
    }
   }
  }
  else // --- 3D -- 
  {
   if( grid==0 )
   { // incident plus reflected wave.
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      x = XEP(i1,i2,i3,0);
      y = XEP(i1,i2,i3,1);
      z = XEP(i1,i2,i3,2);

      real u1 = PMIex(x,y,z,t);
      real u2 = PMIey(x,y,z,t);
      real u3 = PMIez(x,y,z,t);

      #If #OPTION eq "initialCondition"
       UEX(i1,i2,i3)= u1;
       UEY(i1,i2,i3)= u2;
       UEZ(i1,i2,i3)= u3;
 
       UMEX(i1,i2,i3)= PMIex(x,y,z,tm);
       UMEY(i1,i2,i3)= PMIey(x,y,z,tm);
       UMEZ(i1,i2,i3)= PMIez(x,y,z,tm);
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERREZ(i1,i2,i3)=UEZ(i1,i2,i3)-u3;
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,ez)= u3;
      #End
    }
   }
   else
   {
    // transmitted wave
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      x = XEP(i1,i2,i3,0);
      y = XEP(i1,i2,i3,1);
      z = XEP(i1,i2,i3,2);

      real u1 = PMITex(x,y,z,t);
      real u2 = PMITey(x,y,z,t);
      real u3 = PMITez(x,y,z,t);

      #If #OPTION eq "initialCondition"
       UEX(i1,i2,i3)= u1;
       UEY(i1,i2,i3)= u2;
       UEZ(i1,i2,i3)= u3;
 
       UMEX(i1,i2,i3)= PMITex(x,y,z,tm);
       UMEY(i1,i2,i3)= PMITey(x,y,z,tm);
       UMEZ(i1,i2,i3)= PMITez(x,y,z,tm);
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERREZ(i1,i2,i3)=UEZ(i1,i2,i3)-u3;
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,ez)= u3;
      #End
    }
   }


  }

#endMacro
