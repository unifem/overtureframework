 #include "multigrid2.h"
 #include "blockTridiag2d.h"

//Used when simultaneous solver is needed. Only
//for line solver or zebra.
realArray SignOf1(realArray uarray);

/* ----
realArray SignOf1(realArray uarray){
  realArray u1;
   
 u1.redim(uarray);
 u1=0.0;
 where(uarray>0.0) u1=1.0;
 elsewhere(uarray<0.0) u1=-1.0;
    
  return u1;
 }
--- */

 void multigrid2::
  findPQ(const realArray &u1, const realArray &coeff1, 
	 const realArray &coeff2, Index I11, Index I22, 
	 Index I33, int i, int isweep, int ichangePQ){
   realArray Xc, Xe, X2, XY, normXc, normXe;
   realArray Xcc,Xee,Sign;
   int i1, j1,istart, jstart, istop, jstop, ichange,jchange, isharp;
   Index Rtmp=Range(0,1);
   Index Itmp,Jtmp,Ktmp;
   Index I111,I222,I333;
   intArray index(2,3);
   int axis, side, icdif[3], is[3], i0;
   istart=I1[i].getBase(), istop=I1[i].getBound();
   jstart=I2[i].getBase(), jstop=I2[i].getBound();
   index(0,0)=istart, index(1,0)=istop;
   index(0,1)=jstart, index(1,1)=jstop;
   index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
   for (axis=0;axis<=1;axis++)
    for (side=0;side<=1;side++){
     i0=2*axis+side;
     P[i0][i]=0., Q[i0][i]=0.;
     if (gridBc(side,axis)==3){
       getBoundaryIndex(index,side,axis,I111,I222,I333);
       is[0]=0, is[1]=0, is[2]=0;
       if (axis==0) icdif[0]=0, icdif[1]=1;
       else if (axis==1) icdif[0]=1, icdif[1]=0;
       is[axis]=1-2*side;

     //real omega1=0.1;
       if (axis==0){
	XC[side].redim(I111,I222,I333,ndimension);
	XCC[side].redim(I111,I222,I333,ndimension);
        if (side==0){
	  Itmp=Range(istart,istart+1);
	  Jtmp=I222, Ktmp=I333;
        }
        else {
          Itmp=Range(istop-1,istop);
          Jtmp=I222, Ktmp=I333;
        }
       }
       else if (axis==1){
	XE[side].redim(I111,I222,I333,ndimension);
	XEE[side].redim(I111,I222,I333,ndimension);
        if (side==0){
	  Jtmp=Range(jstart,jstart+1);
	  Itmp=I111, Ktmp=I333;
        }
        else {
          Jtmp=Range(jstop-1,jstop);
          Itmp=I111, Ktmp=I333;
        }
       }
       P1[i0].redim(Itmp,Jtmp,Ktmp,Rtmp);
       P2[i0].redim(Itmp,Jtmp,Ktmp,Rtmp);
       Q1[i0].redim(Itmp,Jtmp,Ktmp,Rtmp);
       Q2[i0].redim(Itmp,Jtmp,Ktmp,Rtmp);
       P1[i0]=0.;
       Q1[i0]=0.;
       P2[i0]=0.;
       Q2[i0]=0.;
   
       // This corresponds to j=0. so Xc and Xcc are known
       Xc.redim(I111,I222,I333,Rtmp), Xcc.redim(I111,I222,I333,Rtmp);
       Xe.redim(I111,I222,I333,Rtmp), Xee.redim(I111,I222,I333,Rtmp);
       normXc.redim(I111,I222,I333), normXe.redim(I111,I222,I333);
       Sign.redim(I111,I222,I333);

       Xc=0.0, Xe=0.0, Xcc=0.0, Xee=0.0, Sign=0.0, normXc=0.0, normXe=0.0;
       if (axis==1){
         Xc(I111,I222,I333,Rtmp)=
	     (u[i](I111+icdif[0],I222,I333,Rtmp)-
	      u[i](I111-icdif[0],I222,I333,Rtmp))/(2.*dx[i]);
         Xe(I111,I222,I333,Rtmp)= real(is[1])*
	     (u[i](I111,I222+is[1],I333,Rtmp)-
	      u[i](I111,I222,I333,Rtmp))/dy[i];
         Xcc(I111,I222,I333,Rtmp)=
	     (u[i](I111+icdif[0],I222,I333,Rtmp)-
          2.0*u[i](I111,I222,I333,Rtmp)+
	      u[i](I111-icdif[0],I222,I333,Rtmp))/(dx[i]*dx[i]);

         normXc(I111,I222,I333)=
	     Xc(I111,I222,I333,0)*Xc(I111,I222,I333,0)+
	     Xc(I111,I222,I333,1)*Xc(I111,I222,I333,1);
         normXe=(dB(side,axis)*dB(side,axis));

         Sign=SignOf1(Xe(I111,I222,I333,0)*Xc(I111,I222,I333,1)-
		      Xe(I111,I222,I333,1)*Xc(I111,I222,I333,0));

         Xe(I111,I222,I333,0)=dB(side,axis)*Sign(I111,I222,I333)*
	       Xc(I111,I222,I333,1)/sqrt(normXc(I111,I222,I333));
         Xe(I111,I222,I333,1)=-dB(side,axis)*Sign(I111,I222,I333)*
	       Xc(I111,I222,I333,0)/sqrt(normXc(I111,I222,I333));

         XE[side]=Xe;
         Xee(I111,I222,I333,Rtmp)=(-7.*u[i](I111,I222,I333,Rtmp)+ 
		        8.*u[i](I111,I222+is[1],I333,Rtmp) -
                        u[i](I111,I222+2*is[1],I333,Rtmp)-
			6.0*real(is[1])*dy[i]*Xe(I111,I222,I333,Rtmp))/
			(2.*dy[i]*dy[i]);
        XEE[side]=Xee;
        if (ichangePQ==1){
	  if (i==0){
	    if (iter<1) Xee0[side][i]=0.0;
            else Xee0[side][i](I111,I222,I333,Rtmp) = 
	        (1.0-omega1)*Xee0[side][i](I111,I222,I333,Rtmp)+
		     omega1*Xee(I111,I222,I333,Rtmp);
          }
          else{
             j1=I222.getBase();
	     for (i1=I11.getBase(); i1<=I11.getBound(); i1++)
	       Xee0[side][i](i1,j1,I333,Rtmp)=
		      Xee0[side][i-1](2*i1,2*j1,I333,Rtmp);
	  }
        } 
       }
       else if (axis==0){
         Xe(I111,I222,I333,Rtmp)=
	     (u[i](I111,I222+icdif[1],I333,Rtmp)-
	      u[i](I111,I222-icdif[1],I333,Rtmp))/(2.*dy[i]);
         Xc(I111,I222,I333,Rtmp)= real(is[0])*
	     (u[i](I111+is[0],I222,I333,Rtmp)-
	      u[i](I111,I222,I333,Rtmp))/dx[i];
         Xee(I111,I222,I333,Rtmp)=
	     (u[i](I111,I222+icdif[1],I333,Rtmp)-
          2.0*u[i](I111,I222,I333,Rtmp)+
	      u[i](I111,I222-icdif[1],I333,Rtmp))/(dy[i]*dy[i]);

         normXe(I111,I222,I333)=
	     Xe(I111,I222,I333,0)*Xe(I111,I222,I333,0)+
	     Xe(I111,I222,I333,1)*Xe(I111,I222,I333,1);
         normXc=(dB(side,axis)*dB(side,axis));
         Sign=SignOf1(Xe(I111,I222,I333,0)*Xc(I111,I222,I333,1)-
		      Xe(I111,I222,I333,1)*Xc(I111,I222,I333,0));

         Xc(I111,I222,I333,0)=dB(side,axis)*Sign(I111,I222,I333)*
	       Xe(I111,I222,I333,1)/sqrt(normXe(I111,I222,I333));
         Xc(I111,I222,I333,1)=-dB(side,axis)*Sign(I111,I222,I333)*
	       Xe(I111,I222,I333,0)/sqrt(normXe(I111,I222,I333));

         XC[side]=Xc;
         Xcc(I111,I222,I333,Rtmp)=(-7.*u[i](I111,I222,I333,Rtmp)+ 
		        8.*u[i](I111+is[0],I222,I333,Rtmp)-
                        u[i](I111+2*is[0],I222,I333,Rtmp)-
			6.0*real(is[0])*dx[i]*Xc(I111,I222,I333,Rtmp))/
			(2.*dx[i]*dx[i]);
        XCC[side]=Xcc;
        if (ichangePQ==1){
          if (i==0){
            if (iter<1) Xcc0[side][i]=0.0;
            else Xcc0[side][i](I111,I222,I333,Rtmp)=
               (1.0-omega1)*Xcc0[side][i](I111,I222,I333,Rtmp)+
               omega1*Xcc(I111,I222,I333,Rtmp);
          }
          else {
            i1=I111.getBase();
            for (j1=I22.getBase(); j1<=I22.getBound(); j1++)
                 Xcc0[side][i](i1,j1,I333,Rtmp)=
                      Xcc0[side][i-1](2*i1,2*j1,I333,Rtmp);
          }
        }
       }
   
       P[i0][i](I111,I222,I333)=
            (Xc(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
             Xc(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
                   normXc(I111,I222,I333)+
            (Xc(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
             Xc(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
                   normXe(I111,I222,I333);
       Q[i0][i](I111,I222,I333)= 
            (Xe(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
             Xe(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
                   normXc(I111,I222,I333)+
            (Xe(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
             Xe(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
                   normXe(I111,I222,I333);

       // P and Q away from the boundary i.e. at jstart+2  or
       // jstop-2 when axis==1 or at istart+2 or istop-2 when
       // axis==0
       if (axis==1){
	 P[i0][i](I111,I222+2*is[1],I333)=
	    (Xc(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
	     Xc(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		normXc(I111,I222,I333)+
	    (Xc(I111,I222,I333,0)*Xee0[side][i](I111,I222,I333,0)+
	     Xc(I111,I222,I333,1)*Xee0[side][i](I111,I222,I333,1))/
	        normXe(I111,I222,I333);
         Q[i0][i](I111,I222+2*is[1],I333)=
	    (Xe(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
	     Xe(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
	        normXc(I111,I222,I333)+
	    (Xe(I111,I222,I333,0)*Xee0[side][i](I111,I222,I333,0)+
             Xe(I111,I222,I333,1)*Xee0[side][i](I111,I222,I333,1))/
	        normXe(I111,I222,I333);
         jchange=I222.getBase()+2*is[1];
        }
       if (axis==0){
	 P[i0][i](I111+2*is[0],I222,I333)=
	    (Xc(I111,I222,I333,0)*Xcc0[side][i](I111,I222,I333,0)+
	     Xc(I111,I222,I333,1)*Xcc0[side][i](I111,I222,I333,1))/
		normXc(I111,I222,I333)+
	    (Xc(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
	     Xc(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
	        normXe(I111,I222,I333);
         Q[i0][i](I111+2*is[0],I222,I333)=
	    (Xe(I111,I222,I333,0)*Xcc0[side][i](I111,I222,I333,0)+
	     Xe(I111,I222,I333,1)*Xcc0[side][i](I111,I222,I333,1))/
	        normXc(I111,I222,I333)+
	    (Xe(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
             Xe(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
	        normXe(I111,I222,I333);
             ichange=I111.getBase()+2*is[0];
        }

       //Find the P1 P2 Q1 and Q2
       P1[i0](I111,I222,I333,0)=Xc(I111,I222,I333,0)/
				 (dB(side,axis)*dB(side,axis));
       P1[i0](I111,I222,I333,1)=P1[i0](I111,I222,I333,0);
       P2[i0](I111,I222,I333,0)=Xc(I111,I222,I333,1)/
				  (dB(side,axis)*dB(side,axis));
       P2[i0](I111,I222,I333,1)=P2[i0](I111,I222,I333,0);
       Q1[i0](I111,I222,I333,0)=Xe(I111,I222,I333,0)/
				  (dB(side,axis)*dB(side,axis));
       Q1[i0](I111,I222,I333,1)=Q1[i0](I111,I222,I333,0);
       Q2[i0](I111,I222,I333,0)=Xe(I111,I222,I333,1)/
				  (dB(side,axis)*dB(side,axis));
       Q2[i0](I111,I222,I333,1)=Q2[i0](I111,I222,I333,0);

       isharp=0;
       for (j1=I222.getBase();j1<=I222.getBound();j1++)
        for (i1=I111.getBase();i1<=I111.getBound();i1++){
         if (((utmp[i](i1+icdif[0],j1+icdif[1],0,0)-utmp[i](i1,j1,0,0))*
              (utmp[i](i1-icdif[0],j1-icdif[1],0,0)-utmp[i](i1,j1,0,0))+
              (utmp[i](i1+icdif[0],j1+icdif[1],0,1)-utmp[i](i1,j1,0,1))*
              (utmp[i](i1-icdif[0],j1-icdif[1],0,1)-utmp[i](i1,j1,0,1)))>0.000){
           isharp=1;
           break;
         }
       }
       if (isharp==1){
       int i11, j11;
       for (j1=I222.getBase();j1<=I222.getBound();j1++)
        for (i1=I111.getBase();i1<=I111.getBound();i1++)
           if (((u[i](i1+icdif[0],j1+icdif[1],0,0)-u[i](i1,j1,0,0))*
                (u[i](i1-icdif[0],j1-icdif[1],0,0)-u[i](i1,j1,0,0))+
                (u[i](i1+icdif[0],j1+icdif[1],0,1)-u[i](i1,j1,0,1))*
                (u[i](i1-icdif[0],j1-icdif[1],0,1)-u[i](i1,j1,0,1)))>10.*REAL_EPSILON){
              if (((axis==1)&&(i1<istop)&&(i1>istart))||
		  ((axis==0)&&(j1<jstop)&&(j1>jstart))){
               //i11=i1+2*is[0], j11=j1+2*is[1];
               P[i0][i](i1,j1,0)=0.5*(P[i0][i](i1+icdif[0],j1+icdif[1],0)+
                                      P[i0][i](i1-icdif[0],j1+icdif[1],0));
               Q[i0][i](i1,j1,0)=0.5*(Q[i0][i](i1+icdif[0],j1+icdif[1],0)+
                                      Q[i0][i](i1-icdif[0],j1+icdif[1],0));
	       if (axis==1) {
		 P[i0][i](i1,jchange,0)=0.5*(P[i0][i](i1+1,jchange,0)+
                                          P[i0][i](i1-1,jchange,0));
                 Q[i0][i](i1,jchange,0)=0.5*(Q[i0][i](i1+1,jchange,0)+
                                          Q[i0][i](i1-1,jchange,0));
               }
	       else if (axis==0){
		 P[i0][i](ichange,j1,0)=0.5*(P[i0][i](ichange,j1+1,0)+
                                          P[i0][i](ichange,j1-1,0));
                 Q[i0][i](ichange,j1,0)=0.5*(Q[i0][i](ichange,j1+1,0)+
                                          Q[i0][i](ichange,j1-1,0));
	       }
               P1[i0](i1,j1,0,Rtmp)=
			0.5*(P1[i0](i1+icdif[0],j1+icdif[1],0,Rtmp)+
                             P1[i0](i1-icdif[0],j1-icdif[1],0,Rtmp));
               P2[i0](i1,j1,0,Rtmp)=
			0.5*(P2[i0](i1+icdif[0],j1+icdif[1],0,Rtmp)+
                             P2[i0](i1-icdif[0],j1-icdif[1],0,Rtmp));
               Q1[i0](i1,j1,0,Rtmp)=
			0.5*(Q1[i0](i1+icdif[0],j1+icdif[1],0,Rtmp)+
                             Q1[i0](i1-icdif[0],j1-icdif[1],0,Rtmp));
               Q2[i0](i1,j1,0,Rtmp)=
			0.5*(Q2[i0](i1+icdif[0],j1+icdif[1],0,Rtmp)+
                             Q2[i0](i1-icdif[0],j1-icdif[1],0,Rtmp));
              }
	      else if (axis==1){
		if ((i1==istart)||(i1==istop)){
		  int iss;
		  if (i1==istart) iss=1;
		  else iss=-1;
                  P[i0][i](i1,j1,0)=
		     0.5*(P[i0][i](istart+2,j1,0)+P[i0][i](istop-2,j1,0));
	          P[i0][i](i1+iss,j1,0)=P[i0][i](i1,j1,0);
                  Q[i0][i](i1,j1,0)=
		     0.5*(Q[i0][i](istart+2,j1,0)+Q[i0][i](istop-2,j1,0));
	          Q[i0][i](i1+iss,j1,0)=Q[i0][i](i1,j1,0);
                  P[i0][i](i1,jchange,0)=0.5*(P[i0][i](istart+2,jchange,0)+
                                          P[i0][i](istop-2,jchange,0));
	          P[i0][i](i1+iss,jchange,0)=P[i0][i](i1,jchange,0);
                  Q[i0][i](i1,jchange,0)=0.5*(Q[i0][i](istart+2,jchange,0)+
                                          Q[i0][i](istop-2,jchange,0)); 
	          Q[i0][i](i1+iss,jchange,0)=Q[i0][i](i1,jchange,0);
                  P1[i0](i1,j1,0,Rtmp)=0.5*(P1[i0](istart+2,j1,0,Rtmp)+
                                            P1[i0](istop-2,j1,0,Rtmp));
	          P1[i0](i1+iss,j1,0,Rtmp)=P1[i0](i1,j1,0,Rtmp);
                  P2[i0](i1,j1,0,Rtmp)=0.5*(P2[i0](istart+2,j1,0,Rtmp)+
                                         P2[i0](istop-2,j1,0,Rtmp));
	          P2[i0](i1+iss,j1,0,Rtmp)=P2[i0](i1,j1,0,Rtmp);
                  Q1[i0](i1,j1,0,Rtmp)=0.5*(Q1[i0](istart+2,j1,0,Rtmp)+
                                            Q1[i0](istop-2,j1,0,Rtmp));
	          Q1[i0](i1+iss,j1,0,Rtmp)=Q1[i0](i1,j1,0,Rtmp);
                  Q2[i0](i1,j1,0,Rtmp)=0.5*(Q2[i0](istart+2,j1,0,Rtmp)+
                                         Q2[i0](istop-2,j1,0,Rtmp));
	          Q2[i0](i1+iss,j1,0,Rtmp)=Q2[i0](i1,j1,0,Rtmp);
                }
             }
	     else if (axis==0){
		if ((j1==jstart)||(j1==jstop)){
		  int jss;
		  if (j1==jstart) jss=1;
		  else jss=-1;
                  P[i0][i](i1,j1,0)=
		     0.5*(P[i0][i](i1,jstart+2,0)+P[i0][i](i1,jstop-2,0));
	          P[i0][i](i1,j1+jss,0)=P[i0][i](i1,j1,0);
                  Q[i0][i](i1,j1,0)=
		     0.5*(Q[i0][i](i1,jstart+2,0)+Q[i0][i](i1,jstop-2,0));
	          Q[i0][i](i1,j1+jss,0)=Q[i0][i](i1,j1,0);
                  P[i0][i](ichange,j1,0)=0.5*(P[i0][i](ichange,jstart+2,0)+
                                          P[i0][i](ichange,jstop-2,0));
	          P[i0][i](ichange,j1+jss,0)=P[i0][i](ichange,j1,0);
                  Q[i0][i](ichange,j1,0)=0.5*(Q[i0][i](ichange,jstart+2,0)+
                                          Q[i0][i](ichange,jstop-2,0)); 
	          Q[i0][i](ichange,j1+jss,0)=Q[i0][i](ichange,j1,0);
                  P1[i0](i1,j1,0,Rtmp)=0.5*(P1[i0](i1,jstart+2,0,Rtmp)+
                                            P1[i0](i1,jstop-2,0,Rtmp));
	          P1[i0](i1,j1+jss,0,Rtmp)=P1[i0](i1,j1,0,Rtmp);
                  P2[i0](i1,j1,0,Rtmp)=0.5*(P2[i0](i1,jstart+2,0,Rtmp)+
                                         P2[i0](i1,jstop-2,0,Rtmp));
	          P2[i0](i1,j1+jss,0,Rtmp)=P2[i0](i1,j1,0,Rtmp);
                  Q1[i0](i1,j1,0,Rtmp)=0.5*(Q1[i0](i1,jstart+2,0,Rtmp)+
                                            Q1[i0](i1,jstop-2,0,Rtmp));
	          Q1[i0](i1,j1+jss,0,Rtmp)=Q1[i0](i1,j1,0,Rtmp);
                  Q2[i0](i1,j1,0,Rtmp)=0.5*(Q2[i0](i1,jstart+2,0,Rtmp)+
                                            Q2[i0](i1,jstop-2,0,Rtmp));
	          Q2[i0](i1,j1+jss,0,Rtmp)=Q2[i0](i1,j1,0,Rtmp);
                }
	     }
           }
       }

       if (axis==1){
         if (side==0){
	   for (j1=jstart+1; j1<=jstop; j1++){
             if (j1<jchange){
	       P[2][i](I11,j1,I33)=P[2][i](I11,jstart,I33);
               Q[2][i](I11,j1,I33)=Q[2][i](I11,jstart,I33);
	       P1[2](I11,j1,I33,Rtmp)=P1[2](I11,jstart,I33,Rtmp);
	       P2[2](I11,j1,I33,Rtmp)=P2[2](I11,jstart,I33,Rtmp);
	       Q1[2](I11,j1,I33,Rtmp)=Q1[2](I11,jstart,I33,Rtmp);
	       Q2[2](I11,j1,I33,Rtmp)=Q2[2](I11,jstart,I33,Rtmp);
             }
	     else if (j1>jchange){
	       P[2][i](I11,j1,I33)=P[2][i](I11,jchange,I33);
               Q[2][i](I11,j1,I33)=Q[2][i](I11,jchange,I33);
	     }
           }
	 }
	 else if (side==1){
          for (j1=jstart; j1<jstop; j1++){
            if (j1>jchange){
              P[3][i](I11,j1,I33)=P[3][i](I11,jstop,I33);
              Q[3][i](I11,j1,I33)=Q[3][i](I11,jstop,I33);
              P1[3](I11,j1,I33,Rtmp)=P1[3](I11,jstop,I33,Rtmp);
              P2[3](I11,j1,I33,Rtmp)=P2[3](I11,jstop,I33,Rtmp);
              Q1[3](I11,j1,I33,Rtmp)=Q1[3](I11,jstop,I33,Rtmp);
              Q2[3](I11,j1,I33,Rtmp)=Q2[3](I11,jstop,I33,Rtmp);
            }
            else if (j1<jchange){
              P[3][i](I11,j1,I33)=P[3][i](I11,jchange,I33);
              Q[3][i](I11,j1,I33)=Q[3][i](I11,jchange,I33);
            }
          }  
	 }
       }
       if (axis==0){
         if (side==0){
	   for (i1=istart+1; i1<=istop; i1++){
             if (i1<ichange){
	       P[2][i](i1,I22,I33)=P[2][i](istart,I22,I33);
               Q[2][i](i1,I22,I33)=Q[2][i](istart,I22,I33);
	       P1[2](i1,I22,I33,Rtmp)=P1[2](istart,I22,I33,Rtmp);
	       P2[2](i1,I22,I33,Rtmp)=P2[2](istart,I22,I33,Rtmp);
	       Q1[2](i1,I22,I33,Rtmp)=Q1[2](istart,I22,I33,Rtmp);
	       Q2[2](i1,I22,I33,Rtmp)=Q2[2](istart,I22,I33,Rtmp);
             }
	     else if (i1>ichange){
	       P[2][i](i1,I22,I33)=P[2][i](ichange,I22,I33);
               Q[2][i](i1,I22,I33)=Q[2][i](ichange,I22,I33);
	     }
           }
	 }
	 else if (side==1){
          for (i1=istart; i1<istop; i1++){
            if (i1>ichange){
              P[3][i](i1,I22,I33)=P[3][i](istop,I22,I33);
              Q[3][i](i1,I22,I33)=Q[3][i](istop,I22,I33);
              P1[3](i1,I22,I33,Rtmp)=P1[3](istop,I22,I33,Rtmp);
              P2[3](i1,I22,I33,Rtmp)=P2[3](istop,I22,I33,Rtmp);
              Q1[3](i1,I22,I33,Rtmp)=Q1[3](istop,I22,I33,Rtmp);
              Q2[3](i1,I22,I33,Rtmp)=Q2[3](istop,I22,I33,Rtmp);
            }
            else if (i1<ichange){
              P[3][i](i1,I22,I33)=P[3][i](ichange,I22,I33);
              Q[3][i](i1,I22,I33)=Q[3][i](ichange,I22,I33);
            }
          }  
	 }
       }

       //printf("\n\n !!!level=%i !!!\n",i);
       //Xe.display("C'est Xe");
       //Xee.display("C'est Xee");
       //Xc.display("C'est Xc");
       //Xcc.display("C'est Xcc");
       //CoeffInterp[2][i].display("C'est CoeffInterp[2]");
       //coeff1.display("C'est coeff1");
       //coeff2.display("C'est coeff2");

       //Multiply each by the interplation coefficient
       //and the appropriate coefficient
       P[i0][i](I11,I22,I33) *= 
	       CoeffInterp[i0][i](I11,I22,I33)*coeff1(I11,I22,I33);
       Q[i0][i](I11,I22,I33) *= 
	       CoeffInterp[i0][i](I11,I22,I33)*coeff2(I11,I22,I33);
       P[i0][i] /= (2.*dx[i]), Q[i0][i] /= (2*dy[i]);
       //P[2].display("C'est P apres");
       //Q[2].display("C'est Q apres");
       //for (int j11=I22.getBase();j11<=I22.getBound();j11++)
	//for (int i11=I11.getBase();i11<=I11.getBound();i11++)
	 //printf("i=%i\t j=%i\t p=%g\t q=%g\n", i11, j11, P[2](i11,j11,0), Q[2](i11,j11,0)); 
       Ktmp=I33;
       if (axis==1){
         Itmp=I11;
         if (side==0) Jtmp=Range(jstart+1,jchange-1);
         else Jtmp=Range(jchange+1,jstop-1);
       }
       else if (axis==0){
         Jtmp=I22;
         if (side==0) Itmp=Range(istart+1,ichange-1);
         else Itmp=Range(ichange+1,istop-1);
       }

       for (j1=0;j1<ndimension;j1++){
	P1[i0](Itmp,Jtmp,Ktmp,j1) *= CoeffInterp[i0][i](Itmp,Jtmp,Ktmp)*
				  coeff1(Itmp,Jtmp,Ktmp);
        P2[i0](Itmp,Jtmp,Ktmp,j1) *= CoeffInterp[i0][i](Itmp,Jtmp,Ktmp)*
				  coeff1(Itmp,Jtmp,Ktmp);
	Q1[i0](Itmp,Jtmp,Ktmp,j1) *= CoeffInterp[i0][i](Itmp,Jtmp,Ktmp)*
				  coeff2(Itmp,Jtmp,Ktmp);
        Q2[i0](Itmp,Jtmp,Ktmp,j1) *= CoeffInterp[i0][i](Itmp,Jtmp,Ktmp)*
				  coeff2(Itmp,Jtmp,Ktmp);
       }

       //Finally multiply P1 and P2 by U1_c and Q1 and Q2 by u1_e
       P1[i0](Itmp,Jtmp,Ktmp,Rtmp) *= (u1(Itmp+1,Jtmp,Ktmp,Rtmp)-
		    u1(Itmp-1,Jtmp,Ktmp,Rtmp))/(2.*dx[i]);
       P2[i0](Itmp,Jtmp,Ktmp,Rtmp) *= (u1(Itmp+1,Jtmp,Ktmp,Rtmp)-
		    u1(Itmp-1,Jtmp,Ktmp,Rtmp))/(2.*dx[i]);
       Q1[i0](Itmp,Jtmp,Ktmp,Rtmp) *= (u1(Itmp,Jtmp+1,Ktmp,Rtmp)-
		    u1(Itmp,Jtmp-1,Ktmp,Rtmp))/(2.*dy[i]);
       Q2[i0](Itmp,Jtmp,Ktmp,Rtmp) *= (u1(Itmp,Jtmp+1,Ktmp,Rtmp)-
		    u1(Itmp,Jtmp-1,Ktmp,Rtmp))/(2.*dy[i]);

      }
    }
 }
 
 void multigrid2::
   blockLine2Dsolve(realArray &a1, realArray &b1, realArray &c1, realArray &d1,
	       Index I11, Index I22, Index I33, realArray &coeff1, 
	       realArray &coeff2, realArray &coeff3, realArray &coeff4,
	       realArray &coeff5, Index Ic1, Index Ic2, Index Ic3, 
	       int i, realArray &u1, int isweep, realArray *up){
   blockTridiag2d btri;
   realArray Xc, Xe, X2, XY, normXc, normXe;
   realArray Xcc,Xee,Sign;
   realArray a11, c11;
   int i1, j1,istart, jstart, istop,jstop;
   Index I111, I222, I333;
   intArray index(2,3); 
   int is[3], axis, side, i0;
   Index Rtmp,J22;
   //real omega1=0.1;

   a1=0.0, b1=0.0, c1=0.0, d1=0.0;
   jstart=I2[i].getBase(), jstop=I2[i].getBound();
   istart=I1[i].getBase(), istop=I1[i].getBound();
   index(0,0)=istart, index(1,0)=istop;
   index(0,1)=jstart, index(1,1)=jstop;
   index(0,2)=0,      index(1,2)=0;
   /*********
   for (int j11=jstart;j11<=jstop;j11++)
    for (int i11=istart;i11<=istop;i11++){
     printf("\n i=%i\t j=%i\t coeff1=%g\t coeff2=%g\t coeff3=%g\n",
     i11,j11,coeff1(i11,j11,0),coeff2(i11,j11,0),coeff3(i11,j11,0));
     printf("P0=%g\t Q0=%g\t P1=%g\t Q1=%g\n", P[0](i11,j11,0),
     Q[0](i11,j11,0),P[1](i11,j11,0),Q[1](i11,j11,0));
     printf("P2=%g\t Q2=%g\t P3=%g\t Q3=%g\n", P[2](i11,j11,0),
     Q[2](i11,j11,0),P[3](i11,j11,0),Q[3](i11,j11,0));
     if (i11<3){
      printf("P10x=%g\t P20x=%g\t Q10x=%g\t Q20x=%g\n",P1[0](i11,j11,0,0),
      P2[0](i11,j11,0,0),Q1[0](i11,j11,0,0),Q2[0](i11,j11,0,0));
      printf("P10y=%g\t P20y=%g\t Q10y=%g\t Q20y=%g\n",P1[0](i11,j11,0,1),
      P2[0](i11,j11,0,1),Q1[0](i11,j11,0,1),Q2[0](i11,j11,0,1));
     }
     if (i11>istop-3){
      printf("P11x=%g\t P21x=%g\t Q11x=%g\t Q21x=%g\n",P1[1](i11,j11,0,0),
      P2[1](i11,j11,0,0),Q1[1](i11,j11,0,0),Q2[1](i11,j11,0,0));
      printf("P11y=%g\t P21y=%g\t Q11y=%g\t Q21y=%g\n",P1[1](i11,j11,0,1),
      P2[1](i11,j11,0,1),Q1[1](i11,j11,0,1),Q2[1](i11,j11,0,1));
     }
     if (j11<3){
      printf("P12x=%g\t P22x=%g\t Q12x=%g\t Q22x=%g\n",P1[2](i11,j11,0,0),
      P2[2](i11,j11,0,0),Q1[2](i11,j11,0,0),Q2[2](i11,j11,0,0));
      printf("P12y=%g\t P22y=%g\t Q12y=%g\t Q22y=%g\n",P1[2](i11,j11,0,1),
      P2[2](i11,j11,0,1),Q1[2](i11,j11,0,1),Q2[2](i11,j11,0,1));
     }
     if (j11>jstop-3){
      printf("P13x=%g\t P23x=%g\t Q13x=%g\t Q23x=%g\n",P1[3](i11,j11,0,0),
      P2[3](i11,j11,0,0),Q1[3](i11,j11,0,0),Q2[3](i11,j11,0,0));
      printf("P13y=%g\t P23y=%g\t Q13y=%g\t Q23y=%g\n",P1[3](i11,j11,0,1),
      P2[3](i11,j11,0,1),Q1[3](i11,j11,0,1),Q2[3](i11,j11,0,1));
     }
    }
    ****************/
   if (isweep==0){
     coeff1.reshape(1,1,coeff1.dimension(0),coeff1.dimension(1),
		  coeff1.dimension(2));
     coeff2.reshape(1,1,coeff2.dimension(0),coeff2.dimension(1),
		  coeff2.dimension(2));
     Source[i].reshape(1,1,Source[i].dimension(0),
                       Source[i].dimension(1),
		       Source[i].dimension(2),
		       Source[i].dimension(3));
     for (j1=0;j1<4;j1++)
      P[j1][i].reshape(1,1,P[j1][i].dimension(0), P[j1][i].dimension(1),
		    P[j1][i].dimension(2));

     b1(0,0,I11,I22,I33)=-2.0*coeff1(0,0,I11,I22,I33)/(dx[i]*dx[i])-
	                2.0*coeff2(0,0,I11,I22,I33)/(dy[i]*dy[i]);
     b1(1,1,I11,I22,I33)=b1(0,0,I11,I22,I33);
     a1(0,0,I11,I22,I33)=coeff1(0,0,I11,I22,I33)*(1.0/(dx[i]*dx[i])-
	                 Source[i](0,0,I11,I22,I33,0)/(2*dx[i]))+
			 (P[0][i](0,0,I11,I22,I33)+P[1][i](0,0,I11,I22,I33)+
			  P[2][i](0,0,I11,I22,I33)+P[3][i](0,0,I11,I22,I33));
     a1(1,1,I11,I22,I33)=a1(0,0,I11,I22,I33);
     c1(0,0,I11,I22,I33)=coeff1(0,0,I11,I22,I33)*(1.0/(dx[i]*dx[i])+
			 Source[i](0,0,I11,I22,I33,0)/(2*dx[i]))-
			 (P[0][i](0,0,I11,I22,I33)+P[1][i](0,0,I11,I22,I33)+
			  P[2][i](0,0,I11,I22,I33)+P[3][i](0,0,I11,I22,I33));
     c1(1,1,I11,I22,I33)=c1(0,0,I11,I22,I33);

//ADJUST THE COEFFICIENTS
     for (axis=0;axis<=1;axis++)
      for (side=0;side<=1;side++){
       is[0]=0, is[1]=0, is[2]=0;
       i0=2*axis+side;
       if (gridBc(side,axis)==3){
         getBoundaryIndex(index,side,axis,I111,I222,I333);
         if (I111.getBase() != I111.getBound()) I111=I11;
         if (I222.getBase() != I222.getBound()) I222=I22; 
         is[axis]=1-2*side;
         P1[i0].reshape(1,1,P1[i0].dimension(0), P1[i0].dimension(1),
		    P1[i0].dimension(i0),P1[i0].dimension(3));
         Q1[i0].reshape(1,1,Q1[i0].dimension(0), Q1[i0].dimension(1),
		    Q1[i0].dimension(2),Q1[i0].dimension(3));
         P2[i0].reshape(1,1,P2[i0].dimension(0), P2[i0].dimension(1),
		    P2[i0].dimension(2),P2[i0].dimension(3));
         Q2[i0].reshape(1,1,Q2[i0].dimension(0), Q2[i0].dimension(1),
		    Q2[i0].dimension(2),Q2[i0].dimension(3));
         if (axis==1){
           b1(0,0,I111,I222+is[1],I333) -= 
              8.*(P1[i0](0,0,I111,I222+is[1],I333,0)+
                  Q1[i0](0,0,I111,I222+is[1],I333,0))/(2*dy[i]*dy[i]);
           b1(0,1,I111,I222+is[1],I333) -= 
              8.*(P2[i0](0,0,I111,I222+is[1],I333,0)+
                  Q2[i0](0,0,I111,I222+is[1],I333,0))/(2*dy[i]*dy[i]);
           b1(1,0,I111,I222+is[1],I333) -= 
              8.*(P1[i0](0,0,I111,I222+is[1],I333,1)+
                  Q1[i0](0,0,I111,I222+is[1],I333,1))/(2*dy[i]*dy[i]);
           b1(1,1,I111,I222+is[1],I333) -= 
              8.*(P2[i0](0,0,I111,I222+is[1],I333,1)+
                  Q2[i0](0,0,I111,I222+is[1],I333,1))/(2*dy[i]*dy[i]);
         }
         if (axis==0){
           if (side==0) a11.reference(a1), c11.reference(c1);
           else a11.reference(c1), c11.reference(a1);
          a11(0,0,I111+is[0],I222,I333)+=
              7.*(P1[i0](0,0,I111+is[0],I222,I333,0)+
		  Q1[i0](0,0,I111+is[0],I222,I333,0))/(2.*dx[i]*dx[i]);
          a11(0,1,I111+is[0],I222,I333)+=
              7.*(P2[i0](0,0,I111+is[0],I222,I333,0)+
		  Q2[i0](0,0,I111+is[0],I222,I333,0))/(2.*dx[i]*dx[i]);
          a11(1,0,I111+is[0],I222,I333)+=
              7.*(P1[i0](0,0,I111+is[0],I222,I333,1)+
		  Q1[i0](0,0,I111+is[0],I222,I333,1))/(2.*dx[i]*dx[i]);
          a11(1,1,I111+is[0],I222,I333)+=
              7.*(P2[i0](0,0,I111+is[0],I222,I333,1)+
		  Q2[i0](0,0,I111+is[0],I222,I333,1))/(2.*dx[i]*dx[i]);
          b1(0,0,I111+is[0],I222,I333)-=
              8.*(P1[i0](0,0,I111+is[0],I222,I333,0)+
		  Q1[i0](0,0,I111+is[0],I222,I333,0))/(2.*dx[i]*dx[i]);
          b1(0,1,I111+is[0],I222,I333)-=
              8.*(P2[i0](0,0,I111+is[0],I222,I333,0)+
		  Q2[i0](0,0,I111+is[0],I222,I333,0))/(2.*dx[i]*dx[i]);
          b1(1,0,I111+is[0],I222,I333)-=
              8.*(P1[i0](0,0,I111+is[0],I222,I333,1)+
		  Q1[i0](0,0,I111+is[0],I222,I333,1))/(2.*dx[i]*dx[i]);
          b1(1,1,I111+is[0],I222,I333)-=
              8.*(P2[i0](0,0,I111+is[0],I222,I333,1)+
		  Q2[i0](0,0,I111+is[0],I222,I333,1))/(2.*dx[i]*dx[i]);
          c11(0,0,I111+is[0],I222,I333)+=
                 (P1[i0](0,0,I111+is[0],I222,I333,0)+
		  Q1[i0](0,0,I111+is[0],I222,I333,0))/(2.*dx[i]*dx[i]);
          c11(0,1,I111+is[0],I222,I333)+=
                 (P2[i0](0,0,I111+is[0],I222,I333,0)+
		  Q2[i0](0,0,I111+is[0],I222,I333,0))/(2.*dx[i]*dx[i]);
          c11(1,0,I111+is[0],I222,I333)+=
                 (P1[i0](0,0,I111+is[0],I222,I333,1)+
		  Q1[i0](0,0,I111+is[0],I222,I333,1))/(2.*dx[i]*dx[i]);
          c11(1,1,I111+is[0],I222,I333)+=
                 (P2[i0](0,0,I111+is[0],I222,I333,1)+
		  Q2[i0](0,0,I111+is[0],I222,I333,1))/(2.*dx[i]*dx[i]);
         }
       }
     }

     //Now the right hand side
     coeff1.reshape(1,coeff1.dimension(2),coeff1.dimension(3),
		    coeff1.dimension(4));
     coeff2.reshape(1,coeff2.dimension(2),coeff2.dimension(3),
		    coeff2.dimension(4));
     coeff3.reshape(1,coeff3.dimension(0),coeff3.dimension(1),
		    coeff3.dimension(2));
     Source[i].reshape(1,Source[i].dimension(2),
                       Source[i].dimension(3),
		       Source[i].dimension(4),
		       Source[i].dimension(5));
     RHS[i].reshape(1,RHS[i].dimension(0),RHS[i].dimension(1),
		    RHS[i].dimension(2),RHS[i].dimension(3));
     u1.reshape(1,u1.dimension(0),u1.dimension(1),
		u1.dimension(2),u1.dimension(3));

     for (j1=0;j1<=3;j1++)
      Q[j1][i].reshape(1,Q[j1][i].dimension(0), Q[j1][i].dimension(1),
                    Q[j1][i].dimension(2));

      for (side=0;side<=1;side++){
        XE[side].reshape(1,XE[side].dimension(0),XE[side].dimension(1),
           XE[side].dimension(2),XE[side].dimension(3));
        XEE[side].reshape(1,XEE[side].dimension(0),XEE[side].dimension(1),
           XEE[side].dimension(2),XEE[side].dimension(3));
        XC[side].reshape(1,XC[side].dimension(0),XC[side].dimension(1),
           XC[side].dimension(2),XC[side].dimension(3));
        XCC[side].reshape(1,XCC[side].dimension(0),XCC[side].dimension(1),
           XCC[side].dimension(2),XCC[side].dimension(3));
      }

     d1(0,I11,I22,I33)=RHS[i](0,I11,I22,I33,0)+coeff3(0,I11,I22,I33)*
		     (u1(0,I11+1,I22+1,I33,0)-u1(0,I11+1,I22-1,I33,0)-
		      u1(0,I11-1,I22+1,I33,0)+u1(0,I11-1,I22-1,I33,0))/
		      (4.*dx[i]*dy[i])+
                       (Q[0][i](0,I11,I22,I33)+Q[1][i](0,I11,I22,I33)+
                        Q[2][i](0,I11,I22,I33)+Q[3][i](0,I11,I22,I33)-
                        coeff2(0,I11,I22,I33)*
			Source[i](0,I11,I22,I33,1)/(2*dy[i]))*
                       (u1(0,I11,I22+1,I33,0)-u1(0,I11,I22-1,I33,0))-
                       coeff2(0,I11,I22,I33)*(u1(0,I11,I22+1,I33,0)+
		               u1(0,I11,I22-1,I33,0))/(dy[i]*dy[i]);
     d1(1,I11,I22,I33)=RHS[i](0,I11,I22,I33,1)+coeff3(0,I11,I22,I33)*
		     (u1(0,I11+1,I22+1,I33,1)-u1(0,I11+1,I22-1,I33,1)-
		      u1(0,I11-1,I22+1,I33,1)+u1(0,I11-1,I22-1,I33,1))/
		      (4.*dx[i]*dy[i])-
                      coeff2(0,I11,I22,I33)*(u1(0,I11,I22+1,I33,1)+
		    u1(0,I11,I22-1,I33,1))/(dy[i]*dy[i])+
		    (Q[0][i](0,I11,I22,I33)+Q[1][i](0,I11,I22,I33)+
                     Q[2][i](0,I11,I22,I33)+Q[3][i](0,I11,I22,I33)-
                     coeff2(0,I11,I22,I33)*
		     Source[i](0,I11,I22,I33,1)/(2*dy[i]))*
		    (u1(0,I11,I22+1,I33,1)-u1(0,I11,I22-1,I33,1));

     //Adjust the right hand side
     realArray X_C, X_E, X_CC, X_EE;
     for (axis=0;axis<=1;axis++)
      for (side=0;side<=1;side++){
       is[0]=0, is[1]=0, is[2]=0;
       if (gridBc(side,axis)==3){
        i0=2*axis+side;
        is[axis]=1-2*side;
        getBoundaryIndex(index,side,axis,I111,I222,I333);
        if (I111.getBase() != I111.getBound()) I111=I11;
        if (I222.getBase() != I222.getBound()) I222=I22;
        P1[i0].reshape(1,P1[i0].dimension(2), P1[i0].dimension(3),
                    P1[i0].dimension(4),P1[i0].dimension(5));
        Q1[i0].reshape(1,Q1[i0].dimension(2), Q1[i0].dimension(3),
                    Q1[i0].dimension(4),Q1[i0].dimension(5));
        P2[i0].reshape(1,P2[i0].dimension(2), P2[i0].dimension(3),
                    P2[i0].dimension(4),P2[i0].dimension(5));
        Q2[i0].reshape(1,Q2[i0].dimension(2), Q2[i0].dimension(3),
                    Q2[i0].dimension(4),Q2[i0].dimension(5));

        X_C.reference(XC[side]), X_E.reference(XE[side]);
        X_CC.reference(XCC[side]), X_EE.reference(XEE[side]);
        if (axis==1){
          d1(0,I111,I222+is[1],I333) -= 
             (P1[i0](0,I111,I222+is[1],I333,0)+
              Q1[i0](0,I111,I222+is[1],I333,0))*
                  ((7.*u1(0,I111,I222,I333,0)+
                       u1(0,I111,I222+2*is[1],I333,0))/(2.*dy[i]*dy[i])+
                    real(is[1])*3.*X_E(0,I111,I222,I333,0)/dy[i])+
             (P2[i0](0,I111,I222+is[1],I333,0)+
              Q2[i0](0,I111,I222+is[1],I333,0))*
                  ((7.*u1(0,I111,I222,I333,1)+
                       u1(0,I111,I222+2*is[1],I333,1))/(2.*dy[i]*dy[i])+
                    real(is[1])*3.*X_E(0,I111,I222,I333,1)/dy[i])+
             (P1[i0](0,I111,I222+is[1],I333,0)+
	      Q1[i0](0,I111,I222+is[1],I333,0))*X_EE(0,I111,I222,I333,0)+
             (P2[i0](0,I111,I222+is[1],I333,0)+
	      Q2[i0](0,I111,I222+is[1],I333,0))*X_EE(0,I111,I222,I333,1);

          d1(1,I111,I222+is[1],I333) -= 
             (P1[i0](0,I111,I222+is[1],I333,1)+
              Q1[i0](0,I111,I222+is[1],I333,1))*
                  ((7.*u1(0,I111,I222,I333,0)+
                       u1(0,I111,I222+2*is[1],I333,0))/(2.*dy[i]*dy[i])+
                    real(is[1])*3.*X_E(0,I111,I222,I333,0)/dy[i])+
             (P2[i0](0,I111,I222+is[1],I333,1)+
              Q2[i0](0,I111,I222+is[1],I333,1))*
                  ((7.*u1(0,I111,I222,I333,1)+
                       u1(0,I111,I222+2*is[1],I333,1))/(2.*dy[i]*dy[i])+
                    real(is[1])*3.*X_E(0,I111,I222,I333,1)/dy[i])+
             (P1[i0](0,I111,I222+is[1],I333,1)+
	      Q1[i0](0,I111,I222+is[1],I333,1))*X_EE(0,I111,I222,I333,0)+
	     (P2[i0](0,I111,I222+is[1],I333,1)+
	      Q2[i0](0,I111,I222+is[1],I333,1))*X_EE(0,I111,I222,I333,1);
        }
        if (axis==0){
          d1(0,I111+is[0],I222,I333) -= real(is[0])*
            3.*((P1[0](0,I111+is[0],I222,I333,0)+
                 Q1[0](0,I111+is[0],I222,I333,0))*
                      X_C(0,I111,I222,I333,0)+
                (P2[0](0,I111+is[0],I222,I333,0)+
                 Q2[0](0,I111+is[0],I222,I333,0))*
                      X_C(0,I111,I222,I333,1))/dx[i]+
            (P1[0](0,I111+is[0],I222,I333,0)+
             Q1[0](0,I111+is[0],I222,I333,0))*X_CC(0,I111,I222,I333,0)+
            (P2[0](0,I111+is[0],I222,I333,0)+
             Q2[0](0,I111+is[0],I222,I333,0))*X_CC(0,I111,I222,I333,1);
          d1(1,I111+is[0],I222,I333) -= real(is[0])*
            3.*((P1[0](0,I111+is[0],I222,I333,1)+
                 Q1[0](0,I111+is[0],I222,I333,1))*
                      X_C(0,I111,I222,I333,0)+
                (P2[0](0,I111+is[0],I222,I333,1)+
                 Q2[0](0,I111+is[0],I222,I333,1))*
                      X_C(0,I111,I222,I333,1))/dx[i]+
            (P1[0](0,I111+is[0],I222,I333,1)+
             Q1[0](0,I111+is[0],I222,I333,1))*X_CC(0,I111,I222,I333,0)+
            (P2[0](0,I111+is[0],I222,I333,1)+
             Q2[0](0,I111+is[0],I222,I333,1))*X_CC(0,I111,I222,I333,1);
        }
      P1[i0].reshape(P1[i0].dimension(1), P1[i0].dimension(2),
                    P1[i0].dimension(3),P1[i0].dimension(4));
      Q1[i0].reshape(Q1[i0].dimension(1), Q1[i0].dimension(2),
                    Q1[i0].dimension(3),Q1[i0].dimension(4));
      P2[i0].reshape(P2[i0].dimension(1), P2[i0].dimension(2),
                    P2[i0].dimension(3),P2[i0].dimension(4));
      Q2[i0].reshape(Q2[i0].dimension(1), Q2[i0].dimension(2),
                    Q2[i0].dimension(3),Q2[i0].dimension(4));
     }
    }

     //Back to original shapes
     coeff1.reshape(coeff1.dimension(1),coeff1.dimension(2),
		    coeff1.dimension(3));
     coeff2.reshape(coeff2.dimension(1),coeff2.dimension(2),
		    coeff2.dimension(3));
     coeff3.reshape(coeff3.dimension(1),coeff3.dimension(2),
		    coeff3.dimension(3));
     Source[i].reshape(Source[i].dimension(1),
                       Source[i].dimension(2),
		       Source[i].dimension(3),
		       Source[i].dimension(4));
     RHS[i].reshape(RHS[i].dimension(1),RHS[i].dimension(2),
		    RHS[i].dimension(3),RHS[i].dimension(4));
     u1.reshape(u1.dimension(1),u1.dimension(2),
		u1.dimension(3),u1.dimension(4));
     for (j1=0;j1<4;j1++){
      P[j1][i].reshape(P[j1][i].dimension(2), P[j1][i].dimension(3),
                    P[j1][i].dimension(4));
      Q[j1][i].reshape(Q[j1][i].dimension(1), Q[j1][i].dimension(2),
                    Q[j1][i].dimension(3));
     }
     for (side=0;side<=1;side++){
     XE[side].reshape(XE[side].dimension(1),XE[side].dimension(2),
                      XE[side].dimension(3),XE[side].dimension(4));
     XEE[side].reshape(XEE[side].dimension(1),XEE[side].dimension(2),
                       XEE[side].dimension(3),XEE[side].dimension(4));
     XC[side].reshape(XC[side].dimension(1),XC[side].dimension(2),
                      XC[side].dimension(3),XC[side].dimension(4));
     XCC[side].reshape(XCC[side].dimension(1),XCC[side].dimension(2),
                       XCC[side].dimension(3),XCC[side].dimension(4));
     }

     if (userMap->getIsPeriodic(0)!=Mapping::functionPeriodic){
        //The last two boundary points
       if ((gridBc(0,0)==1)||(gridBc(0,0)==3)){
	 u1.reshape(1,u1.dimension(0),u1.dimension(1),
		     u1.dimension(2),u1.dimension(3));
         a1(Range(0,1),Range(0,1),istart,I22,I33)=0.0;
	 b1(Range(0,1),Range(0,1),istart,I22,I33)=0.0;
	 c1(Range(0,1),Range(0,1),istart,I22,I33)=0.0;
	 b1(0,0,istart,I22,I33)=1.0;
	 b1(1,1,istart,I22,I33)=1.0;
	 d1(0,istart,I22,I33)=u1(0,istart,I22,I33,0);
	 d1(1,istart,I22,I33)=u1(0,istart,I22,I33,1);

	 u1.reshape(u1.dimension(1),u1.dimension(2),
		     u1.dimension(3),u1.dimension(4));
       }
       else if (gridBc(0,0)==2){
         Xe.redim(1,1,1,I22,2);
	 Xe=0.0;

	 uprev[i].reshape(1,1,uprev[i].dimension(0),uprev[i].dimension(1),
			  uprev[i].dimension(2),uprev[i].dimension(3));

         Xe(0,0,0,I22,0)=uprev[i](0,0,istart,I22+1,I33,0)-
	             uprev[i](0,0,istart,I22-1,I33,0);
         Xe(0,0,0,I22,1)=uprev[i](0,0,istart,I22+1,I33,1)-
	             uprev[i](0,0,istart,I22-1,I33,1);

	 a1(Range(0,1),Range(0,1),istart,I22,I33)=0.0;
	 c1(Range(0,1),Range(0,1),istart,I22,I33)=0.0;
         where(fabs(Xe(0,0,0,I22,0))>0.00001){
	   b1(0,0,istart,I22,I33)=Xe(0,0,0,I22,0);
           b1(0,1,istart,I22,I33)=Xe(0,0,0,I22,1);
           b1(1,0,istart,I22,I33)=-Xe(0,0,0,I22,1);
           b1(1,1,istart,I22,I33)=Xe(0,0,0,I22,0);
           c1(0,0,istart,I22,I33)=-Xe(0,0,0,I22,0);
           c1(0,1,istart,I22,I33)=-Xe(0,0,0,I22,1);
         }
	 elsewhere(){
	   b1(0,0,istart,I22,I33)=-Xe(0,0,0,I22,1);
	   b1(0,1,istart,I22,I33)=Xe(0,0,0,I22,0);
	   b1(1,0,istart,I22,I33)=Xe(0,0,0,I22,0);
	   b1(1,1,istart,I22,I33)=Xe(0,0,0,I22,1);
	   c1(1,0,istart,I22,I33)=-Xe(0,0,0,I22,0);
	   c1(1,1,istart,I22,I33)=-Xe(0,0,0,I22,1);
	 }

	 Xe.reshape(1,1,Xe.dimension(3),Xe.dimension(4));
	 uprev[i].reshape(1,uprev[i].dimension(2),uprev[i].dimension(3),
			  uprev[i].dimension(4),uprev[i].dimension(5));
         RHS[i].reshape(1,RHS[i].dimension(0),RHS[i].dimension(1),
			  RHS[i].dimension(2),RHS[i].dimension(3));

         if (i==0){
	   where(fabs(Xe(0,0,I22,0))>0.00001){
	     d1(0,istart,I22,I33)=0;
             d1(1,istart,I22,I33)=Xe(0,0,I22,0)*uprev[i](0,istart,I22-1,I33,1)-
                                Xe(0,0,I22,1)*uprev[i](0,istart,I22-1,I33,0);
           }
	   elsewhere(){
	     d1(0,istart,I22,I33)=Xe(0,0,I22,0)*uprev[i](0,istart,I22-1,I33,1)-
	                          Xe(0,0,I22,1)*uprev[i](0,istart,I22-1,I33,0);
             d1(1,istart,I22,I33)=0.0;
	   }
	 }
	 else{
	   d1(0,istart,I22,I33)=RHS[i](0,istart,I22,I33,0);
	   d1(1,istart,I22,I33)=RHS[i](0,istart,I22,I33,1);
	 }

	 uprev[i].reshape(uprev[i].dimension(1),uprev[i].dimension(2),
			  uprev[i].dimension(3),uprev[i].dimension(4));
         RHS[i].reshape(RHS[i].dimension(1),RHS[i].dimension(2),
			RHS[i].dimension(3),RHS[i].dimension(4));
      }

      if ((gridBc(1,0)==1)||(gridBc(1,0)==3)){

	  u1.reshape(1,u1.dimension(0),u1.dimension(1),
		     u1.dimension(2),u1.dimension(3));

        a1(Range(0,1),Range(0,1),istop,I22,I33)=0.0;
	b1(Range(0,1),Range(0,1),istop,I22,I33)=0.0;
	c1(Range(0,1),Range(0,1),istop,I22,I33)=0.0;
        b1(0,0,istop,I22,I33)=1.0;
        b1(1,1,istop,I22,I33)=1.0;
        d1(0,istop,I22,I33)=u1(0,istop,I22,I33,0);
        d1(1,istop,I22,I33)=u1(0,istop,I22,I33,1);

	  u1.reshape(u1.dimension(1),u1.dimension(2),
		     u1.dimension(3),u1.dimension(4));
      }
      else if (gridBc(1,0)==2){
	  Xe.redim(1,1,1,I22,2);
          Xe=0.0;

	 uprev[i].reshape(1,1,uprev[i].dimension(0),uprev[i].dimension(1),
			  uprev[i].dimension(2),uprev[i].dimension(3));

	  Xe(0,0,0,I22,0)=uprev[i](0,0,istop,I22+1,I33,0)-
		      uprev[i](0,0,istop,I22-1,I33,0);
          Xe(0,0,0,I22,1)=uprev[i](0,0,istop,I22+1,I33,1)-
		      uprev[i](0,0,istop,I22-1,I33,1);

          a1(Range(0,1),Range(0,1),istop,I22,I33)=0.0;
	  c1(Range(0,1),Range(0,1),istop,I22,I33)=0.0;
	  where(fabs(Xe(0,0,0,I22,0))>0.00001){
	    b1(0,0,istop,I22,I33)=Xe(0,0,0,I22,0);
	    b1(0,1,istop,I22,I33)=Xe(0,0,0,I22,1);
	    b1(1,0,istop,I22,I33)=-Xe(0,0,0,I22,1);
	    b1(1,1,istop,I22,I33)=Xe(0,0,0,I22,0);
	    a1(0,0,istop,I22,I33)=-Xe(0,0,0,I22,0);
	    a1(0,1,istop,I22,I33)=-Xe(0,0,0,I22,1);
          }
	  elsewhere(){
	    b1(1,0,istop,I22,I33)=Xe(0,0,0,I22,0);
	    b1(1,1,istop,I22,I33)=Xe(0,0,0,I22,1);
	    b1(0,0,istop,I22,I33)=-Xe(0,0,0,I22,1);
	    b1(0,1,istop,I22,I33)=Xe(0,0,0,I22,0);
	    a1(1,0,istop,I22,I33)=-Xe(0,0,0,I22,0);
	    a1(1,1,istop,I22,I33)=-Xe(0,0,0,I22,1);
	  }

	  Xe.reshape(1,1,Xe.dimension(3),Xe.dimension(4));
	  uprev[i].reshape(1,uprev[i].dimension(2),uprev[i].dimension(3),
	 		  uprev[i].dimension(4),uprev[i].dimension(5));
          RHS[i].reshape(1,RHS[i].dimension(0),RHS[i].dimension(1),
			  RHS[i].dimension(2),RHS[i].dimension(3));

	  if (i==0){
	    where(fabs(Xe(0,0,I22,0))>0.00001){
	      d1(0,istop,I22,I33)=0;
	      d1(1,istop,I22,I33)=Xe(0,0,I22,0)*uprev[i](0,istop,I22-1,I33,1)-
	                        Xe(0,0,I22,1)*uprev[i](0,istop,I22-1,I33,0);
            }
	    elsewhere(){
	      d1(1,istop,I22,I33)=0;
	      d1(0,istop,I22,I33)=Xe(0,0,I22,0)*uprev[i](0,istop,I22-1,I33,1)-
	                        Xe(0,0,I22,1)*uprev[i](0,istop,I22-1,I33,0);
	    }
	  }
	  else{
	    d1(0,istop,I22,I33)=RHS[i](0,istop,I22,I33,0);
	    d1(1,istop,I22,I33)=RHS[i](0,istop,I22,I33,1);
	  }

	 uprev[i].reshape(uprev[i].dimension(1),uprev[i].dimension(2),
			  uprev[i].dimension(3),uprev[i].dimension(4));
         RHS[i].reshape(RHS[i].dimension(1),RHS[i].dimension(2),
			RHS[i].dimension(3),RHS[i].dimension(4));
      }

      //Make sure the four corners of the domain are fixed
      if ((jstart==I22.getBase())&&(jstop==I22.getBound())){
       a1(Range(0,1),Range(0,1),istart,jstart,I33)=0.0;
       c1(Range(0,1),Range(0,1),istart,jstart,I33)=0.0;
       b1(0,0,istart,jstart,I33)=1.0;
       b1(0,1,istart,jstart,I33)=0.0;
       b1(1,0,istart,jstart,I33)=0.0;
       b1(1,1,istart,jstart,I33)=1.0;
       d1(0,istart,jstart,0)=uprev[i](istart,jstart,0,0);
       d1(1,istart,jstart,0)=uprev[i](istart,jstart,0,1);
       a1(Range(0,1),Range(0,1),istart,jstop,I33)=0.0;
       c1(Range(0,1),Range(0,1),istart,jstop,I33)=0.0;
       b1(0,0,istart,jstop,I33)=1.0;
       b1(0,1,istart,jstop,I33)=0.0;
       b1(1,0,istart,jstop,I33)=0.0;
       b1(1,1,istart,jstop,I33)=1.0;
       d1(0,istart,jstop,0)=uprev[i](istart,jstop,0,0);
       d1(1,istart,jstop,0)=uprev[i](istart,jstop,0,1);
       a1(Range(0,1),Range(0,1),istop,jstart,I33)=0.0;
       c1(Range(0,1),Range(0,1),istop,jstart,I33)=0.0;
       b1(0,0,istop,jstart,I33)=1.0;
       b1(0,1,istop,jstart,I33)=0.0;
       b1(1,0,istop,jstart,I33)=0.0;
       b1(1,1,istop,jstart,I33)=1.0;
       d1(0,istop,jstart,0)=uprev[i](istop,jstart,0,0);
       d1(1,istop,jstart,0)=uprev[i](istop,jstart,0,1);
       a1(Range(0,1),Range(0,1),istop,jstop,I33)=0.0;
       c1(Range(0,1),Range(0,1),istop,jstop,I33)=0.0;
       b1(0,0,istop,jstop,I33)=1.0;
       b1(0,1,istop,jstop,I33)=0.0;
       b1(1,0,istop,jstop,I33)=0.0;
       b1(1,1,istop,jstop,I33)=1.0;
       d1(0,istop,jstop,0)=uprev[i](istop,jstop,0,0);
       d1(1,istop,jstop,0)=uprev[i](istop,jstop,0,1);
      }

      int isharp=0;
      for (j1=I22.getBase();j1<=I22.getBound();j1++)
	if (((u1(istart,j1+1,0,0)-u1(istart,j1,0,0))*
	     (u1(istart,j1-1,0,0)-u1(istart,j1,0,0))+
	     (u1(istart,j1+1,0,1)-u1(istart,j1,0,1))* 
	     (u1(istart,j1-1,0,1)-u1(istart,j1,0,1)))>10.*REAL_EPSILON) {
          isharp=1;
          break;
        }
         
       if (isharp==1){
        for (j1=I22.getBase();j1<=I22.getBound();j1++){
         if (((u1(istart,j1+1,0,0)-u1(istart,j1,0,0))*
              (u1(istart,j1-1,0,0)-u1(istart,j1,0,0))>10.*REAL_EPSILON)||
             ((u1(istart,j1+1,0,1)-u1(istart,j1,0,1))* 
              (u1(istart,j1-1,0,1)-u1(istart,j1,0,1))>10.*REAL_EPSILON)) {
          a1(Range(0,1),Range(0,1),istart,j1,0)=0.0;
          c1(Range(0,1),Range(0,1),istart,j1,0)=0.0;
          b1(0,0,istart,j1,0)=1.0;
          b1(0,1,istart,j1,0)=0.0;
          b1(1,0,istart,j1,0)=0.0;
          b1(1,1,istart,j1,0)=1.0;
          d1(0,istart,j1,0)=utmp[i](istart,j1,0,0);
          d1(1,istart,j1,0)=utmp[i](istart,j1,0,1);
         if (j1<jstop){
           a1(Range(0,1),Range(0,1),istart,j1+1,0)=0.0;
           c1(Range(0,1),Range(0,1),istart,j1+1,0)=0.0;
           b1(0,0,istart,j1+1,0)=1.0;
           b1(0,1,istart,j1+1,0)=0.0;
           b1(1,0,istart,j1+1,0)=0.0;
           b1(1,1,istart,j1+1,0)=1.0;
           d1(0,istart,j1+1,0)=utmp[i](istart,j1+1,0,0);
           d1(1,istart,j1+1,0)=utmp[i](istart,j1+1,0,1);
         }
         if (j1>jstart){
           a1(Range(0,1),Range(0,1),istart,j1-1,0)=0.0;
           c1(Range(0,1),Range(0,1),istart,j1-1,0)=0.0;
           b1(0,0,istart,j1-1,0)=1.0;
           b1(1,0,istart,j1-1,0)=0.0;
           b1(0,1,istart,j1-1,0)=0.0;
           b1(1,1,istart,j1-1,0)=1.0;
           d1(0,istart,j1-1,0)=utmp[i](istart,j1-1,0,0);
           d1(1,istart,j1-1,0)=utmp[i](istart,j1-1,0,1);
         }
         }
        }
       }

       isharp=0;
       for (j1=I22.getBase();j1<=I22.getBound();j1++)
        if (((u1(istop,j1+1,0,0)-u1(istop,j1,0,0))*
             (u1(istop,j1-1,0,0)-u1(istop,j1,0,0))+
             (u1(istop,j1+1,0,1)-u1(istop,j1,0,1))*
             (u1(istop,j1-1,0,1)-u1(istop,j1,0,1)))>10.*REAL_EPSILON) {
         isharp=1;
         break;  
        }
       if (isharp==1)
        for (j1=I22.getBase();j1<=I22.getBound();j1++){
         if (((u1(istop,j1+1,0,0)-u1(istop,j1,0,0))*
              (u1(istop,j1-1,0,0)-u1(istop,j1,0,0))>10.*REAL_EPSILON)||
             ((u1(istop,j1+1,0,1)-u1(istop,j1,0,1))*
              (u1(istop,j1-1,0,1)-u1(istop,j1,0,1))>10.*REAL_EPSILON)) {
          a1(Range(0,1),Range(0,1),istop,j1,0)=0.0;
          c1(Range(0,1),Range(0,1),istop,j1,0)=0.0;
          b1(0,0,istop,j1,0)=1.0;
          b1(1,0,istop,j1,0)=0.0;
          b1(0,1,istop,j1,0)=0.0;
          b1(1,1,istop,j1,0)=1.0;
          d1(0,istop,j1,0)=u1(istop,j1,0,0);
          d1(1,istop,j1,0)=u1(istop,j1,0,1);
         if (j1<jstop){
           a1(Range(0,1),Range(0,1),istop,j1+1,0)=0.0;
           c1(Range(0,1),Range(0,1),istop,j1+1,0)=0.0;
           b1(0,0,istop,j1+1,0)=1.0;
           b1(1,0,istop,j1+1,0)=0.0;
           b1(0,1,istop,j1+1,0)=0.0;
           b1(1,1,istop,j1+1,0)=1.0;
           d1(0,istop,j1+1,0)=utmp[i](istop,j1+1,0,0);
           d1(1,istop,j1+1,0)=utmp[i](istop,j1+1,0,1);
         }
         if (j1>jstart){ 
           a1(Range(0,1),Range(0,1),istop,j1-1,0)=0.0; 
           c1(Range(0,1),Range(0,1),istop,j1-1,0)=0.0;
           b1(0,0,istop,j1-1,0)=1.0;
           b1(1,0,istop,j1-1,0)=0.0;
           b1(0,1,istop,j1-1,0)=0.0;
           b1(1,1,istop,j1-1,0)=1.0;
           d1(0,istop,j1-1,0)=utmp[i](istop,j1-1,0,0);
           d1(1,istop,j1-1,0)=utmp[i](istop,j1-1,0,1);
         }
        }
       }

      btri.factor(a1,b1,c1,blockTridiag2d::normal,isweep);
      btri.solve(d1,blockTridiag2d::normal);
      u1.reshape(1,u1.dimension(0),u1.dimension(1),
		 u1.dimension(2), u1.dimension(3));
      for (j1=0;j1<ndimension;j1++)
       u1(0,I11,I22,I33,j1)=d1(j1,I11,I22,I33);

      //The orthogonal boundaries in the j-direction
      if (i==0){
        if (gridBc(0,1)==2) updateBC(i,2,0,u1,uprev);
        if (gridBc(1,1)==2) updateBC(i,3,0,u1,uprev);
      }

       // The ghost points
      for (j1=0;j1<ndimension;j1++){
       u1(0,istart-1,I22,I33,j1)=2.*u1(0,istart,I22,I33,j1)-
			       u1(0,istart+1,I22,I33,j1);
       u1(0,istop+1,I22,I33,j1)=2.*u1(0,istop,I22,I33,j1)-
				 u1(0,istop-1,I22,I33,j1);
       if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic){
	 u1(0,I11,jstart-1,I33,j1)=u1(0,I11,jstop-1,I33,j1);
	 u1(0,I11,jstop+1,I33,j1)=u1(0,I11,jstart+1,I33,j1);
       }
      }
      u1.reshape(u1.dimension(1),u1.dimension(2),
		 u1.dimension(3), u1.dimension(4));
      if ((jstart==I22.getBase())&&(jstop==I22.getBound())){
       u1(I11,jstart,I33,Range(0,1))=
	       0.5*(u1(I11,jstart,I33,Range(0,1))+
		    u1(I11,jstop,I33,Range(0,1)));
       u1(I11,jstop,I33,Range(0,1))=u1(I11,jstart,I33,Range(0,1));
      }
     }
     else if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic){
       if (i>0){
        for (i1=istart;i1<=istop;i1++)
         if (((u1(i1+1,jstart,0,0)-u1(i1,jstart,0,0))*
              (u1(i1-1,jstart,0,0)-u1(i1,jstart,0,0))+
              (u1(i1+1,jstart,0,1)-u1(i1,jstart,0,1))*
              (u1(i1-1,jstart,0,1)-u1(i1,jstart,0,1)))>0.0){
           a1(Range(0,1),Range(0,1),i1,jstart+1,0)=0.0;
           c1(Range(0,1),Range(0,1),i1,jstart+1,0)=0.0;
           b1(0,0,i1,jstart+1,0)=1.0;
           b1(1,0,i1,jstart+1,0)=0.0;
           b1(0,1,i1,jstart+1,0)=0.0;
           b1(1,1,i1,jstart+1,0)=1.0;
           d1(0,i1,jstart+1,0)=u1(i1,jstart+1,0,0);
           d1(1,i1,jstart+1,0)=u1(i1,jstart+1,0,1);
           if (i1<istop){
             a1(Range(0,1),Range(0,1),i1+1,jstart+1,0)=0.0;
             c1(Range(0,1),Range(0,1),i1+1,jstart+1,0)=0.0;
             b1(0,0,i1+1,jstart+1,0)=1.0;
             b1(1,0,i1+1,jstart+1,0)=0.0;
             b1(0,1,i1+1,jstart+1,0)=0.0;
             b1(1,1,i1+1,jstart+1,0)=1.0;
             d1(0,i1+1,jstart+1,0)=u1(i1+1,jstart+1,0,0);
             d1(1,i1+1,jstart+1,0)=u1(i1+1,jstart+1,0,1);
           }
           if (i1>istart){
             a1(Range(0,1),Range(0,1),i1-1,jstart+1,0)=0.0;
             c1(Range(0,1),Range(0,1),i1-1,jstart+1,0)=0.0;
             b1(0,0,i1-1,jstart+1,0)=1.0;
             b1(1,0,i1-1,jstart+1,0)=0.0;
             b1(0,1,i1-1,jstart+1,0)=0.0;
             b1(0,0,i1-1,jstart+1,0)=1.0;
             d1(0,i1-1,jstart+1,0)=u1(i1-1,jstart+1,0,0);
             d1(1,i1-1,jstart+1,0)=u1(i1-1,jstart+1,0,1);
           }
         }
        for (i1=istart;i1<=istop;i1++)
         if (((u1(i1+1,jstop,0,0)-u1(i1,jstop,0,0))*
              (u1(i1-1,jstop,0,0)-u1(i1,jstop,0,0))+
              (u1(i1+1,jstop,0,1)-u1(i1,jstop,0,1))*
              (u1(i1-1,jstop,0,1)-u1(i1,jstop,0,1)))>0.0){
           a1(Range(0,1),Range(0,1),i1,jstop-1,0)=0.0;
           c1(Range(0,1),Range(0,1),i1,jstop-1,0)=0.0;
           b1(0,0,i1,jstop-1,0)=1.0;
           b1(1,0,i1,jstop-1,0)=0.0;
           b1(0,1,i1,jstop-1,0)=0.0;
           b1(1,1,i1,jstop-1,0)=1.0;
           d1(0,i1,jstop-1,0)=u1(i1,jstop-1,0,0);
           d1(1,i1,jstop-1,0)=u1(i1,jstop-1,0,1);
           if (i1<istop){
             a1(Range(0,1),Range(0,1),i1+1,jstop-1,0)=0.0;
             c1(Range(0,1),Range(0,1),i1+1,jstop-1,0)=0.0;
             b1(0,0,i1+1,jstop-1,0)=1.0;
             b1(1,0,i1+1,jstop-1,0)=0.0;
             b1(0,1,i1+1,jstop-1,0)=0.0;
             b1(1,1,i1+1,jstop-1,0)=1.0;
             d1(0,i1+1,jstop-1,0)=u1(i1+1,jstop-1,0,0);
             d1(1,i1+1,jstop-1,0)=u1(i1+1,jstop-1,0,1);
           }
           if (i1>istart){
             a1(Range(0,1),Range(0,1),i1-1,jstop-1,0)=0.0;
             c1(Range(0,1),Range(0,1),i1-1,jstop-1,0)=0.0;
             b1(0,0,i1-1,jstop-1,0)=1.0;
             b1(1,0,i1-1,jstop-1,0)=0.0;
             b1(0,1,i1-1,jstop-1,0)=0.0;
             b1(1,1,i1-1,jstop-1,0)=1.0;
             d1(0,i1-1,jstop-1,0)=u1(i1-1,jstop-1,0,0);
             d1(1,i1-1,jstop-1,0)=u1(i1-1,jstop-1,0,1);
           }
         }
       }

      btri.factor(a1,b1,c1,blockTridiag2d::periodic,isweep);
      btri.solve(d1,blockTridiag2d::periodic);
      u1.reshape(1,u1.dimension(0),u1.dimension(1),
		 u1.dimension(2), u1.dimension(3));
       //printf("Avant resultat\n");
       //for (j1=0;j1<=jstop;j1++)
	//printf("j=%i\t X0=%g::%g\t Y0=%g::%g\t Xf=%g::%g\t Yf=%g::%g\n",j1,u1(istart,j1,0,0),d1(0,istart,j1,0),
	        //u1(istart,j1,0,1),d1(1,istart,j1,0),u1(istop,j1,0,0),d1(0,istop,j1,0),u1(istop,j1,0,1),d1(1,istop,j1,0));
      for (j1=0;j1<ndimension;j1++)
       u1(0,I11,I22,I33,j1)=d1(j1,I11,I22,I33);
       // The ghost points
      u1.reshape(u1.dimension(1),u1.dimension(2),
		 u1.dimension(3), u1.dimension(4));
      for (j1=0;j1<ndimension;j1++){
       u1(istart-1,I22,I33,j1)= u1(istop-1,I22,I33,j1);
       u1(istop+1,I22,I33,j1)=u1(istart+1,I22,I33,j1);
      }
       //printf("Apres isweep\n");
       //for (j1=0;j1<=jstop;j1++)
	//printf("j=%i\t X0=%g::%g\t Y0=%g::%g\t Xf=%g::%g\t Yf=%g::%g\n",j1,u1(istart,j1,0,0),d1(0,istart,j1,0),
	        //u1(istart,j1,0,1),d1(1,istart,j1,0),u1(istop,j1,0,0),d1(0,istop,j1,0),u1(istop,j1,0,1),d1(1,istop,j1,0));
      //exit(1);
     }
     u1(istart,jstart,0,Range(0,1))=u1(istart,jstart,0,Range(0,1));
     u1(istop,jstart,0,Range(0,1))=u1(istop,jstart,0,Range(0,1));
     u1(istart,jstop,0,Range(0,1))=u1(istart,jstop,0,Range(0,1));
     u1(istop,jstop,0,Range(0,1))=u1(istop,jstop,0,Range(0,1));
    }
    else if (isweep==1){
     coeff1.reshape(1,1,coeff1.dimension(0),coeff1.dimension(1),
		  coeff1.dimension(2));
     coeff2.reshape(1,1,coeff2.dimension(0),coeff2.dimension(1),
		  coeff2.dimension(2));
     Source[i].reshape(1,1,Source[i].dimension(0),
                       Source[i].dimension(1),
		       Source[i].dimension(2),
		       Source[i].dimension(3));
     for (j1=0;j1<4;j1++)
      Q[j1][i].reshape(1,1,Q[j1][i].dimension(0), Q[j1][i].dimension(1),
		    Q[j1][i].dimension(2));

     b1(0,0,I11,I22,I33)=-2.0*coeff1(0,0,I11,I22,I33)/(dx[i]*dx[i])-
	                2.0*coeff2(0,0,I11,I22,I33)/(dy[i]*dy[i]);
     b1(1,1,I11,I22,I33)=b1(0,0,I11,I22,I33);
     a1(0,0,I11,I22,I33)=coeff2(0,0,I11,I22,I33)*(1.0/(dy[i]*dy[i])-
	                 Source[i](0,0,I11,I22,I33,1)/(2*dy[i]))+
			 (Q[0][i](0,0,I11,I22,I33)+Q[1][i](0,0,I11,I22,I33)+
			  Q[2][i](0,0,I11,I22,I33)+Q[3][i](0,0,I11,I22,I33));
     a1(1,1,I11,I22,I33)=a1(0,0,I11,I22,I33);
     c1(0,0,I11,I22,I33)=coeff2(0,0,I11,I22,I33)*(1.0/(dy[i]*dy[i])+
			 Source[i](0,0,I11,I22,I33,1)/(2*dy[i]))-
			 (Q[0][i](0,0,I11,I22,I33)+Q[1][i](0,0,I11,I22,I33)+
			  Q[2][i](0,0,I11,I22,I33)+Q[3][i](0,0,I11,I22,I33));
     c1(1,1,I11,I22,I33)=c1(0,0,I11,I22,I33);

     //Adjust the coefficients
     for (axis=0;axis<=1;axis++)
      for (side=0;side<=1;side++){
       is[0]=0, is[1]=0, is[2]=0;
       i0=2*axis+side;
       if (gridBc(side,axis)==3){
         getBoundaryIndex(index,side,axis,I111,I222,I333);
         if (I111.getBase() != I111.getBound()) I111=I11;
         if (I222.getBase() != I222.getBound()) I222=I22;
         is[axis]=1-2*side;
         P1[i0].reshape(1,1,P1[i0].dimension(0), P1[i0].dimension(1),
		    P1[i0].dimension(2),P1[i0].dimension(3));
         Q1[i0].reshape(1,1,Q1[i0].dimension(0), Q1[i0].dimension(1),
		    Q1[i0].dimension(2),Q1[i0].dimension(3));
         P2[i0].reshape(1,1,P2[i0].dimension(0), P2[i0].dimension(1),
		    P2[i0].dimension(2),P2[i0].dimension(3));
         Q2[i0].reshape(1,1,Q2[i0].dimension(0), Q2[i0].dimension(1),
		    Q2[i0].dimension(2),Q2[i0].dimension(3));
         if (axis==1){
           if (side==0) a11.reference(a1), c11.reference(c1);
           else if (side==1) a11.reference(c1), c11.reference(a1);
           a11(0,0,I111,I222+is[1],I333)+=
               7.*(P1[i0](0,0,I111,I222+is[1],I333,0)+
		   Q1[i0](0,0,I111,I222+is[1],I333,0))/(2.*dy[i]*dy[i]);
           a11(0,1,I111,I222+is[1],I333)+=
               7.*(P2[i0](0,0,I111,I222+is[1],I333,0)+
		   Q2[i0](0,0,I111,I222+is[1],I333,0))/(2.*dy[i]*dy[i]);
           a11(1,0,I111,I222+is[1],I333)+=
               7.*(P1[i0](0,0,I111,I222+is[1],I333,1)+
		   Q1[i0](0,0,I111,I222+is[1],I333,1))/(2.*dy[i]*dy[i]);
           a11(1,1,I111,I222+is[1],I333)+=
               7.*(P2[i0](0,0,I111,I222+is[1],I333,1)+
		   Q2[i0](0,0,I111,I222+is[1],I333,1))/(2.*dy[i]*dy[i]);
           b1(0,0,I111,I222+is[1],I333)-=
               8.*(P1[i0](0,0,I111,I222+is[1],I333,0)+
		   Q1[i0](0,0,I111,I222+is[1],I333,0))/(2.*dy[i]*dy[i]);
           b1(0,1,I111,I222+is[1],I333)-=
               8.*(P2[i0](0,0,I111,I222+is[1],I333,0)+
		   Q2[i0](0,0,I111,I222+is[1],I333,0))/(2.*dy[i]*dy[i]);
           b1(1,0,I111,I222+is[1],I333)-=
               8.*(P1[i0](0,0,I111,I222+is[1],I333,1)+
		   Q1[i0](0,0,I111,I222+is[1],I333,1))/(2.*dy[i]*dy[i]);
           b1(1,1,I111,I222+is[1],I333)-=
               8.*(P2[i0](0,0,I111,I222+is[1],I333,1)+
		   Q2[i0](0,0,I111,I222+is[1],I333,1))/(2.*dy[i]*dy[i]);
           c11(0,0,I111,I222+is[1],I333)+=
                  (P1[i0](0,0,I111,I222+is[1],I333,0)+
		   Q1[i0](0,0,I111,I222+is[1],I333,0))/(2.*dy[i]*dy[i]);
           c11(0,1,I111,I222+is[1],I333)+=
                  (P2[i0](0,0,I111,I222+is[1],I333,0)+
		   Q2[i0](0,0,I111,I222+is[1],I333,0))/(2.*dy[i]*dy[i]);
           c11(1,0,I111,I222+is[1],I333)+=
                  (P1[i0](0,0,I111,I222+is[1],I333,1)+
		   Q1[i0](0,0,I111,I222+is[1],I333,1))/(2.*dy[i]*dy[i]);
           c11(1,1,I111,I222+is[1],I333)+=
                  (P2[i0](0,0,I111,I222+is[1],I333,1)+
		   Q2[i0](0,0,I111,I222+is[1],I333,1))/(2.*dy[i]*dy[i]);
         }
         if (axis==0){
           b1(0,0,I111+is[0],I222,I333) -= 
               8.*(P1[i0](0,0,I111+is[0],I222,I333,0)+
                   Q1[i0](0,0,I111+is[0],I222,I333,0))/(2*dx[i]*dx[i]);
           b1(0,1,I111+is[0],I222,I333) -= 
               8.*(P2[i0](0,0,I111+is[0],I222,I333,0)+
                   Q2[i0](0,0,I111+is[0],I222,I333,0))/(2*dx[i]*dx[i]);
           b1(1,0,I111+is[0],I222,I333) -= 
               8.*(P1[i0](0,0,I111+is[0],I222,I333,1)+
                   Q1[i0](0,0,I111+is[0],I222,I333,1))/(2*dx[i]*dx[i]);
           b1(1,1,I111+is[0],I222,I333) -= 
               8.*(P2[i0](0,0,I111+is[0],I222,I333,1)+
                   Q2[i0](0,0,I111+is[0],I222,I333,1))/(2*dx[i]*dx[i]);
         }
       }
     }

     //The right hand side
     coeff1.reshape(1,coeff1.dimension(2),coeff1.dimension(3),
		    coeff1.dimension(4));
     coeff2.reshape(1,coeff2.dimension(2),coeff2.dimension(3),
		    coeff2.dimension(4));
     coeff3.reshape(1,coeff3.dimension(0),coeff3.dimension(1),
		    coeff3.dimension(2));
     Source[i].reshape(1,Source[i].dimension(2),
                       Source[i].dimension(3),
		       Source[i].dimension(4),
		       Source[i].dimension(5));
     RHS[i].reshape(1,RHS[i].dimension(0),RHS[i].dimension(1),
		    RHS[i].dimension(2),RHS[i].dimension(3));
     u1.reshape(1,u1.dimension(0),u1.dimension(1),
		u1.dimension(2),u1.dimension(3));

     for (j1=0;j1<=3;j1++)
      P[j1][i].reshape(1,P[j1][i].dimension(0), P[j1][i].dimension(1),
                    P[j1][i].dimension(2));

     for (side=0;side<=1;side++){
      XE[side].reshape(1,XE[side].dimension(0),XE[side].dimension(1),
                       XE[side].dimension(2),XE[side].dimension(3));
      XEE[side].reshape(1,XEE[side].dimension(0),XEE[side].dimension(1),
                       XEE[side].dimension(2),XEE[side].dimension(3));
      XC[side].reshape(1,XC[side].dimension(0),XC[side].dimension(1),
                       XC[side].dimension(2),XC[side].dimension(3));
      XCC[side].reshape(1,XCC[side].dimension(0),XCC[side].dimension(1),
                       XCC[side].dimension(2),XCC[side].dimension(3));
     }

     d1(0,I11,I22,I33)=RHS[i](0,I11,I22,I33,0)+coeff3(0,I11,I22,I33)*
		     (u1(0,I11+1,I22+1,I33,0)-u1(0,I11+1,I22-1,I33,0)-
		      u1(0,I11-1,I22+1,I33,0)+u1(0,I11-1,I22-1,I33,0))/
		      (4.*dx[i]*dy[i])+
                       (P[0][i](0,I11,I22,I33)+P[1][i](0,I11,I22,I33)+
                        P[2][i](0,I11,I22,I33)+P[3][i](0,I11,I22,I33)-
                        coeff1(0,I11,I22,I33)*
			Source[i](0,I11,I22,I33,0)/(2*dx[i]))*
                       (u1(0,I11+1,I22,I33,0)-u1(0,I11-1,I22,I33,0))-
                       coeff1(0,I11,I22,I33)*(u1(0,I11+1,I22,I33,0)+
		               u1(0,I11-1,I22,I33,0))/(dx[i]*dx[i]);
     d1(1,I11,I22,I33)=RHS[i](0,I11,I22,I33,1)+coeff3(0,I11,I22,I33)*
		     (u1(0,I11+1,I22+1,I33,1)-u1(0,I11+1,I22-1,I33,1)-
		      u1(0,I11-1,I22+1,I33,1)+u1(0,I11-1,I22-1,I33,1))/
		      (4.*dx[i]*dy[i])-
                      coeff1(0,I11,I22,I33)*(u1(0,I11+1,I22,I33,1)+
		    u1(0,I11-1,I22,I33,1))/(dx[i]*dx[i])+
		    (P[0][i](0,I11,I22,I33)+P[1][i](0,I11,I22,I33)+
                     P[2][i](0,I11,I22,I33)+P[3][i](0,I11,I22,I33)-
                     coeff1(0,I11,I22,I33)*
		     Source[i](0,I11,I22,I33,0)/(2*dx[i]))*
		    (u1(0,I11+1,I22,I33,1)-u1(0,I11-1,I22,I33,1));

     //Adjust the right hand side
     realArray X_C, X_E, X_CC, X_EE;
     for (axis=0;axis<=1;axis++)
      for (side=0;side<=1;side++){
       is[0]=0, is[1]=0, is[2]=0;
       if (gridBc(side,axis)==3){
        i0=2*axis+side;
        is[axis]=1-2*side;
        getBoundaryIndex(index,side,axis,I111,I222,I333);
        if (I111.getBase() != I111.getBound()) I111=I11;
        if (I222.getBase() != I222.getBound()) I222=I22;
        P1[i0].reshape(1,P1[i0].dimension(2), P1[i0].dimension(3),
                    P1[i0].dimension(4),P1[i0].dimension(5));
        Q1[i0].reshape(1,Q1[i0].dimension(2), Q1[i0].dimension(3),
                    Q1[i0].dimension(4),Q1[i0].dimension(5));
        P2[i0].reshape(1,P2[i0].dimension(2), P2[i0].dimension(3),
                    P2[i0].dimension(4),P2[i0].dimension(5));
        Q2[i0].reshape(1,Q2[i0].dimension(2), Q2[i0].dimension(3),
                    Q2[i0].dimension(4),Q2[i0].dimension(5));

        X_E.reference(XE[side]), X_EE.reference(XEE[side]); 
        X_C.reference(XC[side]), X_CC.reference(XCC[side]); 
        if (axis==1){
          d1(0,I111,I222+is[1],I333) -=real(is[1])*
            3.*((P1[i0](0,I111,I222+is[1],I333,0)+
		 Q1[i0](0,I111,I222+is[1],I333,0))*
		      X_E(0,I111,I222,I333,0)+
                (P2[i0](0,I111,I222+is[1],I333,0)+
		 Q2[i0](0,I111,I222+is[1],I333,0))*
		      X_E(0,I111,I222,I333,1))/dy[i]+
		(P1[i0](0,I111,I222+is[1],I333,0)+
		 Q1[i0](0,I111,I222+is[1],I333,0))*X_EE(0,I111,I222,I333,0)+
		(P2[i0](0,I111,I222+is[1],I333,0)+
		 Q2[i0](0,I111,I222+is[1],I333,0))*X_EE(0,I111,I222,I333,1);
          d1(1,I111,I222+is[1],I333) -=real(is[1])*
            3.*((P1[i0](0,I111,I222+is[1],I333,1)+
		 Q1[i0](0,I111,I222+is[1],I333,1))*
		      X_E(0,I111,I222,I333,0)+
                (P2[i0](0,I111,I222+is[1],I333,1)+
		 Q2[i0](0,I111,I222+is[1],I333,1))*
		      X_E(0,I111,I222,I333,1))/dy[i]+
		(P1[i0](0,I111,I222+is[1],I333,1)+
		 Q1[i0](0,I111,I222+is[1],I333,1))*X_EE(0,I111,I222,I333,0)+
		(P2[i0](0,I111,I222+is[1],I333,1)+
		 Q2[i0](0,I111,I222+is[1],I333,1))*X_EE(0,I111,I222,I333,1);
       }
       if (axis==0){
         d1(0,I111+is[0],I222,I333) -= 
            (P1[i0](0,I111+is[0],I222,I333,0)+
             Q1[i0](0,I111+is[0],I222,I333,0))*
                ((7.*u1(0,I111,I222,I333,0)+
                     u1(0,I111+2*is[0],I222,I333,0))/(2.*dx[i]*dx[i])+
                  real(is[0])*3.*X_C(0,I111,I222,I333,0)/dx[i])+
            (P2[i0](0,I111+is[0],I222,I333,0)+
             Q2[i0](0,I111+is[0],I222,I333,0))*
                ((7.*u1(0,I111,I222,I333,1)+
                     u1(0,I111+2*is[0],I222,I333,1))/(2.*dx[i]*dx[i])+
                  real(is[0])*3.*X_C(0,I111,I222,I333,1)/dx[i])+
	    (P1[i0](0,I111+is[0],I222,I333,0)+
	     Q1[i0](0,I111+is[0],I222,I333,0))*X_CC(0,I111,I222,I333,0)+
	    (P2[i0](0,I111+is[0],I222,I333,0)+
	     Q2[i0](0,I111+is[0],I222,I333,0))*X_CC(0,I111,I222,I333,1);

         d1(1,I111+is[0],I222,I333) -= 
            (P1[i0](0,I111+is[0],I222,I333,1)+
             Q1[i0](0,I111+is[0],I222,I333,1))*
                ((7.*u1(0,I111,I222,I333,0)+
                     u1(0,I111+2*is[0],I222,I333,0))/(2.*dx[i]*dx[i])+
                  real(is[0])*3.*X_C(0,I111,I222,I333,0)/dx[i])+
            (P2[i0](0,I111+is[0],I222,I333,1)+
             Q2[i0](0,I111+is[0],I222,I333,1))*
                ((7.*u1(0,I111,I222,I333,1)+
                     u1(0,I111+2*is[0],I222,I333,1))/(2.*dx[i]*dx[i])+
                  real(is[0])*3.*X_C(0,I111,I222,I333,1)/dx[i])+
	    (P1[i0](0,I111+is[0],I222,I333,1)+
	     Q1[i0](0,I111+is[0],I222,I333,1))*X_CC(0,I111,I222,I333,0)+
	    (P2[i0](0,I111+is[0],I222,I333,1)+
	     Q2[i0](0,I111+is[0],I222,I333,1))*X_CC(0,I111,I222,I333,1);
       }
      P1[i0].reshape(P1[i0].dimension(1), P1[i0].dimension(2),
                    P1[i0].dimension(3), P1[i0].dimension(4));
      Q1[i0].reshape(Q1[i0].dimension(1), Q1[i0].dimension(2),
                    Q1[i0].dimension(3), Q1[i0].dimension(4));
      P2[i0].reshape(P2[i0].dimension(1), P2[i0].dimension(2),
                    P2[i0].dimension(3), P2[i0].dimension(4));
      Q2[i0].reshape(Q2[i0].dimension(1), Q2[i0].dimension(2),
                    Q2[i0].dimension(3), Q2[i0].dimension(4));
     }
    }

     //Back to original shapes
     coeff1.reshape(coeff1.dimension(1),coeff1.dimension(2),
		    coeff1.dimension(3));
     coeff2.reshape(coeff2.dimension(1),coeff2.dimension(2),
		    coeff2.dimension(3));
     coeff3.reshape(coeff3.dimension(1),coeff3.dimension(2),
		    coeff3.dimension(3));
     Source[i].reshape(Source[i].dimension(1),
                       Source[i].dimension(2),
		       Source[i].dimension(3),
		       Source[i].dimension(4));
     RHS[i].reshape(RHS[i].dimension(1),RHS[i].dimension(2),
		    RHS[i].dimension(3),RHS[i].dimension(4));
     u1.reshape(u1.dimension(1),u1.dimension(2),
		u1.dimension(3),u1.dimension(4));
     for (j1=0;j1<4;j1++){
      P[j1][i].reshape(P[j1][i].dimension(1), P[j1][i].dimension(2),
                    P[j1][i].dimension(3));
      Q[j1][i].reshape(Q[j1][i].dimension(2), Q[j1][i].dimension(3),
                    Q[j1][i].dimension(4));
     }
     for (side=0;side<=1;side++){
       XE[side].reshape(XE[side].dimension(1),XE[side].dimension(2),
                        XE[side].dimension(3),XE[side].dimension(4));
       XEE[side].reshape(XEE[side].dimension(1),XEE[side].dimension(2),
                         XEE[side].dimension(3),XEE[side].dimension(4));
       XC[side].reshape(XC[side].dimension(1),XC[side].dimension(2),
                        XC[side].dimension(3),XC[side].dimension(4));
       XCC[side].reshape(XCC[side].dimension(1),XCC[side].dimension(2),
                         XCC[side].dimension(3),XCC[side].dimension(4));
     }

     if (userMap->getIsPeriodic(1)!=Mapping::functionPeriodic){
        //The last two boundary points
       if ((gridBc(0,1)==1)||(gridBc(0,1)==3)){

	  u1.reshape(1,u1.dimension(0),u1.dimension(1),
		     u1.dimension(2),u1.dimension(3));

	  a1(Range(0,1),Range(0,1),I11,jstart,I33)=0.0;
	  b1(Range(0,1),Range(0,1),I11,jstart,I33)=0.0;
	  c1(Range(0,1),Range(0,1),I11,jstart,I33)=0.0;
	  b1(0,0,I11,jstart,I33)=1.0;
	  b1(1,1,I11,jstart,I33)=1.0;
	  d1(0,I11,jstart,I33)=u1(0,I11,jstart,I33,0);
	  d1(1,I11,jstart,I33)=u1(0,I11,jstart,I33,1);

	  u1.reshape(u1.dimension(1),u1.dimension(2),
		     u1.dimension(3),u1.dimension(4));
       }
       else if (gridBc(0,1)==2){
         Xc.redim(1,1,I11,2);
	 Xc=0.0;

	 uprev[i].reshape(1,1,uprev[i].dimension(0),uprev[i].dimension(1),
			  uprev[i].dimension(2),uprev[i].dimension(3));

         Xc(0,0,I11,0)=uprev[i](0,0,I11+1,jstart,I33,0)-
	             uprev[i](0,0,I11-1,jstart,I33,0);
         Xc(0,0,I11,1)=uprev[i](0,0,I11+1,jstart,I33,1)-
	             uprev[i](0,0,I11-1,jstart,I33,1);
         a1(Range(0,1),Range(0,1),I11,jstart,I33)=0.0;
         c1(Range(0,1),Range(0,1),I11,jstart,I33)=0.0;

	 where(fabs(Xc(0,0,I11,0))>0.00001){
           b1(0,0,I11,jstart,I33)=Xc(0,0,I11,0);
           b1(0,1,I11,jstart,I33)=Xc(0,0,I11,1);
           b1(1,0,I11,jstart,I33)=-Xc(0,0,I11,1);
           b1(1,1,I11,jstart,I33)=Xc(0,0,I11,0);
           c1(0,0,I11,jstart,I33)=-Xc(0,0,I11,0);
           c1(0,1,I11,jstart,I33)=-Xc(0,0,I11,1);
         }
	 elsewhere(){
           b1(1,0,I11,jstart,I33)=Xc(0,0,I11,0);
           b1(1,1,I11,jstart,I33)=Xc(0,0,I11,1);
           b1(0,0,I11,jstart,I33)=-Xc(0,0,I11,1);
           b1(0,1,I11,jstart,I33)=Xc(0,0,I11,0);
           c1(1,0,I11,jstart,I33)=-Xc(0,0,I11,0);
           c1(1,1,I11,jstart,I33)=-Xc(0,0,I11,1);
	 }

	 Xc.reshape(1,Xc.dimension(2),Xc.dimension(3));
	 uprev[i].reshape(1,uprev[i].dimension(2),uprev[i].dimension(3),
			  uprev[i].dimension(4),uprev[i].dimension(5));
         RHS[i].reshape(1,RHS[i].dimension(0), RHS[i].dimension(1),
			RHS[i].dimension(2), RHS[i].dimension(3));

         if (i==0){
	   where(fabs(Xc(0,I11,0))>0.00001){
	    d1(0,I11,jstart,I33)=0;
            d1(1,I11,jstart,I33)=Xc(0,I11,0)*uprev[i](0,I11-1,jstart,I33,1)-
                               Xc(0,I11,1)*uprev[i](0,I11-1,jstart,I33,0);
           }
	   elsewhere(){
	    d1(1,I11,jstart,I33)=0;
            d1(0,I11,jstart,I33)=Xc(0,I11,0)*uprev[i](0,I11-1,jstart,I33,1)-
                               Xc(0,I11,1)*uprev[i](0,I11-1,jstart,I33,0);
	   }
	 }
	 else{
	  d1(0,I11,jstart,I33)=RHS[i](0,I11,jstart,I33,0);
	  d1(1,I11,jstart,I33)=RHS[i](0,I11,jstart,I33,1);
	 }

	 uprev[i].reshape(uprev[i].dimension(1),uprev[i].dimension(2),
			  uprev[i].dimension(3),uprev[i].dimension(4));
         RHS[i].reshape(RHS[i].dimension(1),RHS[i].dimension(2),
	                RHS[i].dimension(3),RHS[i].dimension(4));
      }

      if ((gridBc(1,1)==1)||(gridBc(1,1)==3)){

	u1.reshape(1,u1.dimension(0),u1.dimension(1),
		     u1.dimension(2),u1.dimension(3));

        a1(Range(0,1),Range(0,1),I11,jstop,I33)=0.0;
        b1(Range(0,1),Range(0,1),I11,jstop,I33)=0.0;
        c1(Range(0,1),Range(0,1),I11,jstop,I33)=0.0;
        b1(0,0,I11,jstop,I33)=1.0;
        b1(1,1,I11,jstop,I33)=1.0;
        d1(0,I11,jstop,I33)=u1(0,I11,jstop,I33,0);
        d1(1,I11,jstop,I33)=u1(0,I11,jstop,I33,1);

	  u1.reshape(u1.dimension(1),u1.dimension(2),
		     u1.dimension(3),u1.dimension(4));
      }
      else if (gridBc(1,1)==2){
	Xc.redim(1,1,I11,2);
        Xc=0.0;

	uprev[i].reshape(1,1,uprev[i].dimension(0),uprev[i].dimension(1),
			  uprev[i].dimension(2),uprev[i].dimension(3));
        RHS[i].reshape(1,RHS[i].dimension(0), RHS[i].dimension(1),
			RHS[i].dimension(2), RHS[i].dimension(3));

	Xc(0,0,I11,0)=uprev[i](0,0,I11+1,jstop,I33,0)-
		      uprev[i](0,0,I11-1,jstop,I33,0);
        Xc(0,0,I11,1)=uprev[i](0,0,I11+1,jstop,I33,1)-
		      uprev[i](0,0,I11-1,jstop,I33,1);

        a1(Range(0,1),Range(0,1),I11,jstop,I33)=0.0;
        c1(Range(0,1),Range(0,1),I11,jstop,I33)=0.0;
	where(fabs(Xc(0,0,I11,0))>0.00001){
	  b1(0,0,I11,jstop,I33)=Xc(0,0,I11,0);
	  b1(0,1,I11,jstop,I33)=Xc(0,0,I11,1);
	  b1(1,0,I11,jstop,I33)=-Xc(0,0,I11,1);
	  b1(1,1,I11,jstop,I33)=Xc(0,0,I11,0);
	  a1(0,0,I11,jstop,I33)=-Xc(0,0,I11,0);
	  a1(0,1,I11,jstop,I33)=-Xc(0,0,I11,1);
	}
	elsewhere(){
	  b1(1,0,I11,jstop,I33)=Xc(0,0,I11,0);
	  b1(1,1,I11,jstop,I33)=Xc(0,0,I11,1);
	  b1(0,0,I11,jstop,I33)=-Xc(0,0,I11,1);
	  b1(0,1,I11,jstop,I33)=Xc(0,0,I11,0);
	  a1(1,0,I11,jstop,I33)=-Xc(0,0,I11,0);
	  a1(1,1,I11,jstop,I33)=-Xc(0,0,I11,1);
	}

	Xc.reshape(1,Xc.dimension(2),Xc.dimension(3));
	uprev[i].reshape(1,uprev[i].dimension(2),uprev[i].dimension(3),
			  uprev[i].dimension(4),uprev[i].dimension(5));

	if (i==0){
	  where(fabs(Xc(0,I11,0))>0.00001){
	   d1(0,I11,jstop,I33)=0;
	   d1(1,I11,jstop,I33)=Xc(0,I11,0)*uprev[i](0,I11-1,jstop,I33,1)-
	                       Xc(0,I11,1)*uprev[i](0,I11-1,jstop,I33,0);
          }
	  elsewhere(){
	   d1(1,I11,jstop,I33)=0;
	   d1(0,I11,jstop,I33)=Xc(0,I11,0)*uprev[i](0,I11-1,jstop,I33,1)-
	                       Xc(0,I11,1)*uprev[i](0,I11-1,jstop,I33,0);
	  }
	}
	else{
	  d1(0,I11,jstop,I33)=RHS[i](0,I11,jstop,I33,0);
	  d1(1,I11,jstop,I33)=RHS[i](0,I11,jstop,I33,1);
	}

	uprev[i].reshape(uprev[i].dimension(1),uprev[i].dimension(2),
			  uprev[i].dimension(3),uprev[i].dimension(4));
        RHS[i].reshape(RHS[i].dimension(1),RHS[i].dimension(2),
	                RHS[i].dimension(3),RHS[i].dimension(4));
      }

      if ((istart==I11.getBase())&&(istop==I11.getBound())){
       a1(Range(0,1),Range(0,1),istart,jstart,I33)=0.0;
       c1(Range(0,1),Range(0,1),istart,jstart,I33)=0.0;
       b1(0,0,istart,jstart,I33)=1.0;
       b1(0,1,istart,jstart,I33)=0.0;
       b1(1,0,istart,jstart,I33)=0.0;
       b1(1,1,istart,jstart,I33)=1.0;
       d1(0,istart,jstart,0)=uprev[i](istart,jstart,0,0);
       d1(1,istart,jstart,0)=uprev[i](istart,jstart,0,1);
       a1(Range(0,1),Range(0,1),istart,jstop,I33)=0.0;
       c1(Range(0,1),Range(0,1),istart,jstop,I33)=0.0;
       b1(0,0,istart,jstop,I33)=1.0;
       b1(0,1,istart,jstop,I33)=0.0;
       b1(1,0,istart,jstop,I33)=0.0;
       b1(1,1,istart,jstop,I33)=1.0;
       d1(0,istart,jstop,0)=uprev[i](istart,jstop,0,0);
       d1(1,istart,jstop,0)=uprev[i](istart,jstop,0,1);
       a1(Range(0,1),Range(0,1),istop,jstart,I33)=0.0;
       c1(Range(0,1),Range(0,1),istop,jstart,I33)=0.0;
       b1(0,0,istop,jstart,I33)=1.0;
       b1(0,1,istop,jstart,I33)=0.0;
       b1(1,0,istop,jstart,I33)=0.0;
       b1(1,1,istop,jstart,I33)=1.0;
       d1(0,istop,jstart,0)=uprev[i](istop,jstart,0,0);
       d1(1,istop,jstart,0)=uprev[i](istop,jstart,0,1);
       a1(Range(0,1),Range(0,1),istop,jstop,I33)=0.0;
       c1(Range(0,1),Range(0,1),istop,jstop,I33)=0.0;
       b1(0,0,istop,jstop,I33)=1.0;
       b1(0,1,istop,jstop,I33)=0.0;
       b1(1,0,istop,jstop,I33)=0.0;
       b1(1,1,istop,jstop,I33)=1.0;
       d1(0,istop,jstop,0)=uprev[i](istop,jstop,0,0);
       d1(1,istop,jstop,0)=uprev[i](istop,jstop,0,1);
      }

      int isharp=0;
       for (i1=I11.getBase();i1<=I11.getBound();i1++)
        if (((u1(i1+1,jstart,0,0)-u1(i1,jstart,0,0))*
            (u1(i1-1,jstart,0,0)-u1(i1,jstart,0,0))+
            (u1(i1+1,jstart,0,1)-u1(i1,jstart,0,1))*
            (u1(i1-1,jstart,0,1)-u1(i1,jstart,0,1)))>10.*REAL_EPSILON){
            isharp=1;
            break;
        }
       if (isharp==1)
        for (i1=I11.getBase();i1<=I11.getBound();i1++){
         if (((u1(i1+1,jstart,0,0)-u1(i1,jstart,0,0))*
            (u1(i1-1,jstart,0,0)-u1(i1,jstart,0,0))>10.*REAL_EPSILON)||
            ((u1(i1+1,jstart,0,1)-u1(i1,jstart,0,1))*
            (u1(i1-1,jstart,0,1)-u1(i1,jstart,0,1))>10.*REAL_EPSILON)) {
          a1(Range(0,1),Range(0,1),i1,jstart,0)=0.0;
          c1(Range(0,1),Range(0,1),i1,jstart,0)=0.0;
          b1(0,0,i1,jstart,0)=1.0;
          b1(1,0,i1,jstart,0)=0.0;
          b1(0,1,i1,jstart,0)=0.0;
          b1(1,1,i1,jstart,0)=1.0;
          d1(0,i1,jstart,0)=uprev[i](i1,jstart,0,0);
          d1(1,i1,jstart,0)=uprev[i](i1,jstart,0,1);
          if (i1<istop){
           a1(Range(0,1),Range(0,1),i1+1,jstart,0)=0.0;
           c1(Range(0,1),Range(0,1),i1+1,jstart,0)=0.0;
           b1(0,0,i1+1,jstart,0)=1.0;
           b1(1,0,i1+1,jstart,0)=0.0;
           b1(0,1,i1+1,jstart,0)=0.0;
           b1(1,1,i1+1,jstart,0)=1.0;
           d1(0,i1+1,jstart,0)=uprev[i](i1+1,jstart,0,0);
           d1(1,i1+1,jstart,0)=uprev[i](i1+1,jstart,0,1);
         }
         if (i1>istart){
           a1(Range(0,1),Range(0,1),i1-1,jstart,0)=0.0;
           c1(Range(0,1),Range(0,1),i1-1,jstart,0)=0.0;
           b1(0,0,i1-1,jstart,0)=1.0;
           b1(1,0,i1-1,jstart,0)=0.0;
           b1(0,1,i1-1,jstart,0)=0.0;
           b1(1,1,i1-1,jstart,0)=1.0;
           d1(0,i1-1,jstart,0)=uprev[i](i1-1,jstart,0,0);
           d1(1,i1-1,jstart,0)=uprev[i](i1-1,jstart,0,1);
         }
        }
       } 

       isharp=0;
       for (i1=I11.getBase();i1<=I11.getBound();i1++)
        if (((u1(i1+1,jstop,0,0)-u1(i1,jstop,0,0))*
             (u1(i1-1,jstop,0,0)-u1(i1,jstop,0,0))+
             (u1(i1+1,jstop,0,1)-u1(i1,jstop,0,1))*
             (u1(i1-1,jstop,0,1)-u1(i1,jstop,0,1)))>10.*REAL_EPSILON){
          isharp=1;
          break;
        }
      if (isharp==1){
        for (i1=I11.getBase();i1<=I11.getBound();i1++)
          if (((u1(i1+1,jstop,0,0)-u1(i1,jstop,0,0))*
             (u1(i1-1,jstop,0,0)-u1(i1,jstop,0,0))>10.*REAL_EPSILON)||
            ((u1(i1+1,jstop,0,1)-u1(i1,jstop,0,1))*
             (u1(i1-1,jstop,0,1)-u1(i1,jstop,0,1))>10.*REAL_EPSILON)) {
         a1(Range(0,1),Range(0,1),i1,jstop,0)=0.0;
         c1(Range(0,1),Range(0,1),i1,jstop,0)=0.0;
         b1(0,0,i1,jstop,0)=1.0;
         b1(1,0,i1,jstop,0)=0.0;
         b1(0,1,i1,jstop,0)=0.0;
         b1(1,1,i1,jstop,0)=1.0;
         d1(0,i1,jstop,0)=uprev[i](i1,jstop,0,0);
         d1(1,i1,jstop,0)=uprev[i](i1,jstop,0,1);
         if (i1<istop){
           a1(Range(0,1),Range(0,1),i1+1,jstop,0)=0.0;
           c1(Range(0,1),Range(0,1),i1+1,jstop,0)=0.0;
           b1(0,0,i1+1,jstop,0)=1.0;
           b1(1,0,i1+1,jstop,0)=0.0;
           b1(0,1,i1+1,jstop,0)=0.0;
           b1(1,1,i1+1,jstop,0)=1.0;
           d1(0,i1+1,jstop,0)=uprev[i](i1+1,jstop,0,0);
           d1(1,i1+1,jstop,0)=uprev[i](i1+1,jstop,0,1);
         }
         if (i1>istart){
           a1(Range(0,1),Range(0,1),i1-1,jstop,0)=0.0;
           c1(Range(0,1),Range(0,1),i1-1,jstop,0)=0.0;
           b1(0,0,i1-1,jstop,0)=1.0;
           b1(1,0,i1-1,jstop,0)=0.0;
           b1(0,1,i1-1,jstop,0)=0.0;
           b1(1,1,i1-1,jstop,0)=1.0;
           d1(0,i1-1,jstop,0)=uprev[i](i1-1,jstop,0,0);
           d1(1,i1-1,jstop,0)=uprev[i](i1-1,jstop,0,1);
         }
        }
       } 

      btri.factor(a1,b1,c1,blockTridiag2d::normal,isweep);
      btri.solve(d1,blockTridiag2d::normal);
      u1.reshape(1,u1.dimension(0),u1.dimension(1),
		 u1.dimension(2), u1.dimension(3));
      for (j1=0;j1<ndimension;j1++)
       u1(0,I11,I22,I33,j1)=d1(j1,I11,I22,I33);

      // the orthogonal boundaries in the i direction
      if (i==0){
	if (gridBc(0,0)==2) updateBC(i,0,1,u1,uprev);
        if (gridBc(1,0)==2) updateBC(i,1,1,u1,uprev);
      }

       // The ghost points
      for (j1=0;j1<ndimension;j1++){
       u1(0,I11,jstart-1,I33,j1)=2.*u1(0,I11,jstart,I33,j1)-
			       u1(0,I11,jstart+1,I33,j1);
       u1(0,I11,jstop+1,I33,j1)=2.*u1(0,I11,jstop,I33,j1)-
				 u1(0,I11,jstop-1,I33,j1);
       if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic){
	 u1(0,istart-1,I22,I33,j1)=u1(0,istop-1,I22,I33,j1);
	 u1(0,istop+1,I22,I33,j1)=u1(0,istart+1,I22,I33,j1);
       }
      }
       //printf("Apres jsweep\n");
       //for (j1=0;j1<=jstop;j1++)
	//printf("j=%i\t X0=%g::%g\t Y0=%g::%g\t Xf=%g::%g\t Yf=%g::%g\n",j1,u1(0,istart,j1,0,0),d1(0,istart,j1,0),
	        //u1(0,istart,j1,0,1),d1(1,istart,j1,0),u1(0,istop,j1,0,0),d1(0,istop,j1,0),u1(0,istop,j1,0,1),d1(1,istop,j1,0));
       //exit(1);
       u1.reshape(u1.dimension(1),u1.dimension(2),
		 u1.dimension(3), u1.dimension(4));
      if ((istart==I11.getBase())&&(istop==I11.getBound())){
       u1(istart,I22,I33,Range(0,1))=
	       0.5*(u1(istart,I22,I33,Range(0,1))+
		    u1(istop,I22,I33,Range(0,1)));
       u1(istop,I22,I33,Range(0,1))=u1(istart,I22,I33,Range(0,1));
      }
     }
     else if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic){
       if (i>0){
        for (j1=jstart;j1<=jstop;j1++)
         if (((u1(istart,j1+1,0,0)-u1(istart,j1,0,0))*
              (u1(istart,j1-1,0,0)-u1(istart,j1,0,0))+
              (u1(istart,j1+1,0,1)-u1(istart,j1,0,1))*
              (u1(istart,j1-1,0,1)-u1(istart,j1,0,1)))>0.0){
           a1(Range(0,1),Range(0,1),istart+1,j1,0)=0.0;
           c1(Range(0,1),Range(0,1),istart+1,j1,0)=0.0;
           b1(0,0,istart+1,j1,0)=1.0;
           b1(0,1,istart+1,j1,0)=0.0;
           b1(1,0,istart+1,j1,0)=0.0;
           b1(1,1,istart+1,j1,0)=1.0;
           d1(0,istart+1,j1,0)=u1(istart+1,j1,0,0);
           d1(1,istart+1,j1,0)=u1(istart+1,j1,0,1);
           if (j1<jstop){
             a1(Range(0,1),Range(0,1),istart+1,j1+1,0)=0.0;
             c1(Range(0,1),Range(0,1),istart+1,j1+1,0)=0.0;
             b1(0,0,istart+1,j1+1,0)=1.0;
             b1(1,0,istart+1,j1+1,0)=0.0;
             b1(0,1,istart+1,j1+1,0)=0.0;
             b1(1,1,istart+1,j1+1,0)=1.0;
             d1(0,istart+1,j1+1,0)=u1(istart+1,j1+1,0,0);
             d1(1,istart+1,j1+1,0)=u1(istart+1,j1+1,0,1);
           }
           if (j1>jstart){
             a1(Range(0,1),Range(0,1),istart+1,j1-1,0)=0.0;
             c1(Range(0,1),Range(0,1),istart+1,j1-1,0)=0.0;
             b1(0,0,istart+1,j1-1,0)=1.0;
             b1(1,0,istart+1,j1-1,0)=0.0;
             b1(0,1,istart+1,j1-1,0)=0.0;
             b1(1,1,istart+1,j1-1,0)=1.0;
             d1(0,istart+1,j1-1,0)=u1(istart+1,j1-1,0,0);
             d1(1,istart+1,j1-1,0)=u1(istart+1,j1-1,0,1);
           }
         }
        for (j1=jstart;j1<=jstop;j1++)
         if (((u1(istop,j1+1,0,0)-u1(istop,j1,0,0))*
              (u1(istop,j1-1,0,0)-u1(istop,j1,0,0))+
              (u1(istop,j1+1,0,1)-u1(istop,j1,0,1))*
              (u1(istop,j1-1,0,1)-u1(istop,j1,0,1)))>0.0){
           a1(Range(0,1),Range(0,1),istop-1,j1,0)=0.0;
           c1(Range(0,1),Range(0,1),istop-1,j1,0)=0.0;
           b1(0,0,istop-1,j1,0)=1.0;
           b1(1,0,istop-1,j1,0)=0.0;
           b1(0,1,istop-1,j1,0)=0.0;
           b1(1,1,istop-1,j1,0)=1.0;
           d1(0,istop-1,j1,0)=u1(istop-1,j1,0,0);
           d1(1,istop-1,j1,0)=u1(istop-1,j1,0,1);
           if (j1<jstop){
             a1(Range(0,1),Range(0,1),istop-1,j1+1,0)=0.0;
             c1(Range(0,1),Range(0,1),istop-1,j1+1,0)=0.0;
             b1(0,0,istop-1,j1+1,0)=1.0;
             b1(1,0,istop-1,j1+1,0)=0.0;
             b1(0,1,istop-1,j1+1,0)=0.0;
             b1(1,1,istop-1,j1+1,0)=1.0;
             d1(0,istop-1,j1+1,0)=u1(istop-1,j1+1,0,0);
             d1(1,istop-1,j1+1,0)=u1(istop-1,j1+1,0,1);
           }
           if (j1>jstart){
             a1(0,0,istop-1,j1-1,0)=0.0;
             c1(0,0,istop-1,j1-1,0)=0.0;
             b1(0,0,istop-1,j1-1,0)=1.0;
             b1(1,0,istop-1,j1-1,0)=0.0;
             b1(0,1,istop-1,j1-1,0)=0.0;
             b1(1,1,istop-1,j1-1,0)=1.0;
             d1(0,istop-1,j1-1,0)=u1(istop-1,j1-1,0,0);
             d1(1,istop-1,j1-1,0)=u1(istop-1,j1-1,0,1);
           }
         }
       } 

      btri.factor(a1,b1,c1,blockTridiag2d::periodic,isweep);
      btri.solve(d1,blockTridiag2d::periodic);
      u1.reshape(1,u1.dimension(0),u1.dimension(1),
		 u1.dimension(2), u1.dimension(3));
      for (j1=0;j1<ndimension;j1++)
       u1(0,I11,I22,I33,j1)=d1(j1,I11,I22,I33);
       // The ghost points
       u1.reshape(u1.dimension(1),u1.dimension(2),
		 u1.dimension(3), u1.dimension(4));
      for (j1=0;j1<ndimension;j1++){
       u1(I11,jstart-1,I33,j1)= u1(I11,jstop-1,I33,j1);
       u1(I11,jstop+1,I33,j1)=u1(I11,jstart+1,I33,j1);
      }
     }
     u1(istart,jstart,0,Range(0,1))=uprev[i](istart,jstart,0,Range(0,1));
     u1(istop,jstart,0,Range(0,1))=uprev[i](istop,jstart,0,Range(0,1));
     u1(istart,jstop,0,Range(0,1))=uprev[i](istart,jstop,0,Range(0,1));
     u1(istop,jstop,0,Range(0,1))=uprev[i](istop,jstop,0,Range(0,1));
    }
     /*****
     for (int k11=0;k11<=I33.getBound();k11++)
      for (int j11=I22.getBase(); j11<=I22.getBound();j11++)
       for (int i11=I11.getBase();i11<=I11.getBound();i11++){
       printf("i=%i\t j=%i\t k=%i\t",i11,j11,k11);
       printf("x=%g\t y=%g\n",
              u1(i11,j11,k11,0),u1(i11,j11,k11,1));
      }
   exit(1);
 printf("Je termine block\n");
  *****/
 }

void multigrid2::
 updateBC(int i, int iside, int isweep, realArray &u1, realArray *up){
   //This subroutine updates the orthogonal boundary condition
   //0 corresponds to i=0, 1 to i=imax, 2 to j=0 and 3 to j=jmax
   //The argument i is for the level

  realArray a1, b1, c1, d1;
  realArray Xc, Xe;
  blockTridiag2d btri1;

  if ((iside==0)||(iside==1)){
    a1.redim(2,2,1,Iint2[i],Iint3[i]);
    b1.redim(2,2,1,Iint2[i],Iint3[i]);
    c1.redim(2,2,1,Iint2[i],Iint3[i]);
    d1.redim(2,1,Iint2[i],Iint3[i]);
  }
  else if ((iside==2)||(iside==3)){
    a1.redim(2,2,Iint1[i],1,Iint3[i]);
    b1.redim(2,2,Iint1[i],1,Iint3[i]);
    c1.redim(2,2,Iint1[i],1,Iint3[i]);
    d1.redim(2,Iint1[i],1,Iint3[i]);
  }

  a1=0.0, b1=0.0, c1=0.0, d1=0.0;
  if ((gridBc(0,1)==2)&&(iside==2)){
    Xc.redim(Iint1[i],1,1,2);
    Xc=0.0;
    Xc(Iint1[i],0,0,Range(0,1))=
	uprev[i](Iint1[i]+1,0,0,Range(0,1)) -
	uprev[i](Iint1[i]-1,0,0,Range(0,1));
      
    Xc.reshape(1,1,Xc.dimension(0),Xc.dimension(3));
    a1(Range(0,1),Range(0,1),Iint1[i],0,0)=0.0;
    c1(Range(0,1),Range(0,1),Iint1[i],0,0)=0.0;
    where (fabs(Xc(0,0,Iint1[i],0))>0.00001){
      b1(0,0,Iint1[i],0,0)=Xc(0,0,Iint1[i],0);
      b1(0,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],1);
      b1(1,0,Iint1[i],0,0)=-Xc(0,0,Iint1[i],1);
      b1(1,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],0);
    }
    elsewhere(){
      b1(1,0,Iint1[i],0,0)=Xc(0,0,Iint1[i],0);
      b1(1,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],1);
      b1(0,0,Iint1[i],0,0)=-Xc(0,0,Iint1[i],1);
      b1(0,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],0);
    }

    for (int i1=Iint1[i].getBase();i1<=Iint1[i].getBound();i1++){
      if (fabs(Xc(0,0,i1,0))>0.00001){
        d1(0,i1,0,0)=Xc(0,0,i1,0)*u1(0,i1,1,0,0)+
		       Xc(0,0,i1,1)*u1(0,i1,1,0,1);
        d1(1,i1,0,0)=Xc(0,0,i1,0)*uprev[i](i1-1,0,0,1)-
                       Xc(0,0,i1,1)*uprev[i](i1-1,0,0,0);
      }
      else{
        d1(1,i1,0,0)=Xc(0,0,i1,0)*u1(0,i1,1,0,0)+
		       Xc(0,0,i1,1)*u1(0,i1,1,0,1);
        d1(0,i1,0,0)=Xc(0,0,i1,0)*uprev[i](i1-1,0,0,1)-
	               Xc(0,0,i1,1)*uprev[i](i1-1,0,0,0);
     }
    }

    btri1.factor(a1,b1,c1,blockTridiag2d::normal,0);
    btri1.solve(d1,blockTridiag2d::normal);
    for (i1=Iint1[i].getBase();i1<=Iint1[i].getBound();i1++){
     u1(0,i1,0,0,0)=d1(0,i1,0,0);
     u1(0,i1,0,0,1)=d1(1,i1,0,0);
    }
   }

  if ((gridBc(1,1)==2)&&(iside==3)){
    int jtop = I2[i].getBound();
    Xc.redim(Iint1[i],1,1,2);
    Xc=0.0;
    Xc(Iint1[i],0,0,Range(0,1))=
	uprev[i](Iint1[i]+1,jtop,0,Range(0,1)) -
	uprev[i](Iint1[i]-1,jtop,0,Range(0,1));
      
    Xc.reshape(1,1,Xc.dimension(0),Xc.dimension(3));
    a1(Range(0,1),Range(0,1),Iint1[i],0,0)=0.0;
    c1(Range(0,1),Range(0,1),Iint1[i],0,0)=0.0;
    where (fabs(Xc(0,0,Iint1[i],0))>0.00001){
      b1(0,0,Iint1[i],0,0)=Xc(0,0,Iint1[i],0);
      b1(0,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],1);
      b1(1,0,Iint1[i],0,0)=-Xc(0,0,Iint1[i],1);
      b1(1,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],0); 
    }
    elsewhere(){
      b1(1,0,Iint1[i],0,0)=Xc(0,0,Iint1[i],0);
      b1(1,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],1);
      b1(0,0,Iint1[i],0,0)=-Xc(0,0,Iint1[i],1);
      b1(0,1,Iint1[i],0,0)=Xc(0,0,Iint1[i],0);
    }

    for (int i1=Iint1[i].getBase();i1<=Iint1[i].getBound();i1++){
      if (fabs(Xc(0,0,i1,0))>0.00001){
        d1(0,i1,0,0)=Xc(0,0,i1,0)*u1(0,i1,jtop-1,0,0)+
		       Xc(0,0,i1,1)*u1(0,i1,jtop-1,0,1);
        d1(1,i1,0,0)=Xc(0,0,i1,0)*uprev[i](i1-1,jtop,0,1)-
                       Xc(0,0,i1,1)*uprev[i](i1-1,jtop,0,0);
      }
      else{
        d1(1,i1,0,0)=Xc(0,0,i1,0)*u1(0,i1,jtop-1,0,0)+
		       Xc(0,0,i1,1)*u1(0,i1,jtop-1,0,1);
        d1(0,i1,0,0)=Xc(0,0,i1,0)*uprev[i](i1-1,jtop,0,1)-
	               Xc(0,0,i1,1)*uprev[i](i1-1,jtop,0,0);
     }
    }

    btri1.factor(a1,b1,c1,blockTridiag2d::normal,0);
    btri1.solve(d1,blockTridiag2d::normal);
    for (i1=Iint1[i].getBase();i1<=Iint1[i].getBound();i1++){
     u1(0,i1,jtop,0,0)=d1(0,i1,0,0);
     u1(0,i1,jtop,0,1)=d1(1,i1,0,0);
    }
   }

   if ((gridBc(0,0)==2)&&(iside==0)){
     Xe.redim(1,Iint2[i],1,2);
     Xe=0.0;
     Xe(0,Iint2[i],0,Range(0,1))=
	 uprev[i](0,Iint2[i]+1,0,Range(0,1)) -
	 uprev[i](0,Iint2[i]-1,0,Range(0,1));
      
     Xe.reshape(1,1,1,Xe.dimension(1),Xe.dimension(3));
     a1(Range(0,1),Range(0,1),0,Iint2[i],0)=0.0;
     c1(Range(0,1),Range(0,1),0,Iint2[i],0)=0.0;
     where (fabs(Xe(0,0,0,Iint2[i],0))>0.00001){
	  b1(0,0,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
	  b1(0,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],1);
	  b1(1,0,0,Iint2[i],0)=-Xe(0,0,0,Iint2[i],1);
	  b1(1,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
     }
     elsewhere(){
	  b1(1,0,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
	  b1(1,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],1);
	  b1(0,0,0,Iint2[i],0)=-Xe(0,0,0,Iint2[i],1);
	  b1(0,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
    }

    Xe.reshape(1,1,Xe.dimension(3),Xe.dimension(4));
    for (int j1=Iint2[i].getBase();j1<=Iint2[i].getBound();j1++){
     if (fabs(Xe(0,0,j1,0))>0.00001){
       d1(0,0,j1,0)=Xe(0,0,j1,0)*u1(0,1,j1,0,0)+
                           Xe(0,0,j1,1)*u1(0,1,j1,0,1);
       d1(1,0,j1,0)=Xe(0,0,j1,0)*uprev[i](0,j1-1,0,1)-
	                   Xe(0,0,j1,1)*uprev[i](0,j1-1,0,0);
     }
     else{
       d1(1,0,j1,0)=Xe(0,0,j1,0)*u1(0,1,j1,0,0)+
		           Xe(0,0,j1,1)*u1(0,1,j1,0,1);
       d1(0,0,j1,0)=Xe(0,0,j1,0)*uprev[i](0,j1-1,0,1)-
                           Xe(0,0,j1,1)*uprev[i](0,j1-1,0,0);
     }
    }

    btri1.factor(a1,b1,c1,blockTridiag2d::normal,1);
    btri1.solve(d1,blockTridiag2d::normal);
    for (j1=Iint2[i].getBase();j1<=Iint2[i].getBound();j1++){
     u1(0,0,j1,0,0)=d1(0,0,j1,0);
     u1(0,0,j1,0,1)=d1(1,0,j1,0);
    }
  }

   if ((gridBc(1,0)==2)&&(iside==1)){
     int itop=I1[i].getBound();
     Xe.redim(1,Iint2[i],1,2);
     Xe=0.0;
     Xe(0,Iint2[i],0,Range(0,1))=
	 uprev[i](itop,Iint2[i]+1,0,Range(0,1)) -
	 uprev[i](itop,Iint2[i]-1,0,Range(0,1));
      
     Xe.reshape(1,1,1,Xe.dimension(1),Xe.dimension(3));
     a1(Range(0,1),Range(0,1),0,Iint2[i],0)=0.0;
     c1(Range(0,1),Range(0,1),0,Iint2[i],0)=0.0;
     where (fabs(Xe(0,0,0,Iint2[i],0))>0.00001){
	  b1(0,0,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
	  b1(0,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],1);
	  b1(1,0,0,Iint2[i],0)=-Xe(0,0,0,Iint2[i],1);
	  b1(1,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
     }
     elsewhere(){
	  b1(1,0,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
	  b1(1,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],1);
	  b1(0,0,0,Iint2[i],0)=-Xe(0,0,0,Iint2[i],1);
	  b1(0,1,0,Iint2[i],0)=Xe(0,0,0,Iint2[i],0);
    }

    Xe.reshape(1,1,Xe.dimension(3),Xe.dimension(4));
    for (int j1=Iint2[i].getBase();j1<=Iint2[i].getBound();j1++){
     if (fabs(Xe(0,0,j1,0))>0.00001){
       d1(0,0,j1,0)=Xe(0,0,j1,0)*u1(0,itop-1,j1,0,0)+
                           Xe(0,0,j1,1)*u1(0,itop-1,j1,0,1);
       d1(1,0,j1,0)=Xe(0,0,j1,0)*uprev[i](itop,j1-1,0,1)-
	                   Xe(0,0,j1,1)*uprev[i](itop,j1-1,0,0);
     }
     else{
       d1(1,0,j1,0)=Xe(0,0,j1,0)*u1(0,itop-1,j1,0,0)+
		           Xe(0,0,j1,1)*u1(0,itop-1,j1,0,1);
       d1(0,0,j1,0)=Xe(0,0,j1,0)*uprev[i](itop,j1-1,0,1)-
                           Xe(0,0,j1,1)*uprev[i](itop,j1-1,0,0);
     }
    }

    btri1.factor(a1,b1,c1,blockTridiag2d::normal,1);
    btri1.solve(d1,blockTridiag2d::normal);
    for (j1=Iint2[i].getBase();j1<=Iint2[i].getBound();j1++){
     u1(0,itop,j1,0,0)=d1(0,0,j1,0);
     u1(0,itop,j1,0,1)=d1(1,0,j1,0);
    }
  }
 }
