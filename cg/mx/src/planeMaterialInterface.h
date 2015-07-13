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

// -- incident wave ---
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

//  --- time derivative of incident ---
#defineMacro PMIext(x,y,z,t) pmct*(pmc[ 0]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                                   pmc[ 1]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIeyt(x,y,z,t) pmct*(pmc[ 2]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                                   pmc[ 3]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIezt(x,y,z,t) pmct*(pmc[ 4]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                                   pmc[ 5]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIhxt(x,y,z,t) pmct*(pmc[ 6]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                                   pmc[ 7]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIhyt(x,y,z,t) pmct*(pmc[ 8]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                                   pmc[ 9]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIhzt(x,y,z,t) pmct*(pmc[10]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t))) + \
                                   pmc[11]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))))

// -- transmitted wave ---
#defineMacro PMITex(x,y,z,t) (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMITey(x,y,z,t) (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMITez(x,y,z,t) (pmc[14]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThx(x,y,z,t) (pmc[15]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThy(x,y,z,t) (pmc[16]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThz(x,y,z,t) (pmc[17]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))

//  --- time derivative of transmitted wave ---
#defineMacro PMIText(x,y,z,t) (pmct*pmc[12]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMITeyt(x,y,z,t) (pmct*pmc[13]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMITezt(x,y,z,t) (pmct*pmc[14]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThxt(x,y,z,t) (pmct*pmc[15]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThyt(x,y,z,t) (pmct*pmc[16]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))
#defineMacro PMIThzt(x,y,z,t) (pmct*pmc[17]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))))


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
  const real pmct=pmc[18]*twoPi; // for time derivative of exact solution
 
  if( numberOfDimensions==2 )
  {
   z=0.;
   if( grid < numberOfComponentGrids/2 )
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
       if( method==nfdtd )
       {
         UMEX(i1,i2,i3)= PMIex(x,y,z,tm);
         UMEY(i1,i2,i3)= PMIey(x,y,z,tm);
         UMHZ(i1,i2,i3)= PMIhz(x,y,z,tm);
       }
       else if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIext(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMIeyt(x,y,z,t);
	 uLocal(i1,i2,i3,hzt) = PMIhzt(x,y,z,t);
       }
       
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
       if( method==sosup )
       {
	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-PMIext(x,y,z,t);
	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-PMIeyt(x,y,z,t);
	 errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt)-PMIhzt(x,y,z,t);
       } 
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,hz)= u3;
       if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIext(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMIeyt(x,y,z,t);
	 uLocal(i1,i2,i3,hzt) = PMIhzt(x,y,z,t);
       }
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
       if( method==nfdtd )
       {
	 UMEX(i1,i2,i3)= PMITex(x,y,z,tm);
	 UMEY(i1,i2,i3)= PMITey(x,y,z,tm);
	 UMHZ(i1,i2,i3)= PMIThz(x,y,z,tm);
       }
       else if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIText(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMITeyt(x,y,z,t);
	 uLocal(i1,i2,i3,hzt) = PMIThzt(x,y,z,t);
       }

      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
       if( method==sosup )
       {
	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-PMIText(x,y,z,t);
	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-PMITeyt(x,y,z,t);
	 errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt)-PMIThzt(x,y,z,t);
       }       
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,hz)= u3;
       if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIText(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMITeyt(x,y,z,t);
	 uLocal(i1,i2,i3,hzt) = PMIThzt(x,y,z,t);
       }
      #End
    }
   }
  }
  else // --- 3D -- 
  {
   if( grid < numberOfComponentGrids/2 )
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
       if( method==nfdtd )
       {
	 UMEX(i1,i2,i3)= PMIex(x,y,z,tm);
	 UMEY(i1,i2,i3)= PMIey(x,y,z,tm);
	 UMEZ(i1,i2,i3)= PMIez(x,y,z,tm);
       }
       else if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIext(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMIeyt(x,y,z,t);
	 uLocal(i1,i2,i3,ezt) = PMIezt(x,y,z,t);
       }
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERREZ(i1,i2,i3)=UEZ(i1,i2,i3)-u3;
       if( method==sosup )
       {
	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-PMIext(x,y,z,t);
	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-PMIeyt(x,y,z,t);
	 errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt)-PMIezt(x,y,z,t);
       } 
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,ez)= u3;
       if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIext(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMIeyt(x,y,z,t);
	 uLocal(i1,i2,i3,ezt) = PMIezt(x,y,z,t);
       }
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
       if( method==nfdtd )
       {
	 UMEX(i1,i2,i3)= PMITex(x,y,z,tm);
	 UMEY(i1,i2,i3)= PMITey(x,y,z,tm);
	 UMEZ(i1,i2,i3)= PMITez(x,y,z,tm);
       }
       else if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIText(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMITeyt(x,y,z,t);
	 uLocal(i1,i2,i3,ezt) = PMITezt(x,y,z,t);
       }
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERREZ(i1,i2,i3)=UEZ(i1,i2,i3)-u3;
       if( method==sosup )
       {
	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-PMIText(x,y,z,t);
	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-PMITeyt(x,y,z,t);
	 errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt)-PMITezt(x,y,z,t);
       } 
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,ez)= u3;
       if( method==sosup )
       {
	 uLocal(i1,i2,i3,ext) = PMIText(x,y,z,t);
	 uLocal(i1,i2,i3,eyt) = PMITeyt(x,y,z,t);
	 uLocal(i1,i2,i3,ezt) = PMITezt(x,y,z,t);
       }
      #End
    }
   }


  }

#endMacro
