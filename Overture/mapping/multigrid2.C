#include "multigrid2.h"
#include "blockTridiag2d.h"

static int TEST_RESIDUAL=0;

int **multigrid2::
 IntArray2d(int istart, int iend, int jstart, int jend){
  int **t;
  int i,nrow=iend-istart+1, ncolumn=jend-jstart+1;

  t = new int*[nrow];
  for (i=0;i<nrow;i++){
   t[i]=new int[ncolumn];
  }
  return t;
}

void multigrid2::
 DeleteIntArray2d(int **t, int istart, int iend, int jstart, int jend){
  int i, nrow=iend-istart+1, ncolumn=jend-jstart+1;
  for (i=0; i<nrow; i++) delete[] t[i], t[i]=NULL;
  delete[] t, t=NULL;
}

realArray **multigrid2::
 RealArray2d(int istart, int iend, int jstart, int jend){
  realArray **t;
  int i,nrow=iend-istart+1, ncolumn=jend-jstart+1;

  t = new realArray*[nrow];
  for (i=0;i<nrow;i++){
   t[i]=new realArray[ncolumn];
  }
  return t;
}

void multigrid2::
 DeleteRealArray2d(realArray **t, int istart, int iend, int jstart, int jend){
  int i, nrow=iend-istart+1, ncolumn=jend-jstart+1;
  for (i=0; i<nrow; i++) delete[] t[i], t[i]=NULL;
  delete[] t, t=NULL;
}

multigrid2::
   multigrid2(void){
   //Default constructor
 }

void multigrid2::
   setup(const int ndim, const int nlev, const real a0x,
         const real b0x, const int idim, const real a0y,
         const real b0y, const int jdim, const real a0z,
         const real b0z, const int kdim){
   int itmp=idim-1, jtmp=jdim-1,i;

   alpha=0.0;
   //chooseMethod(itmp);
   a=new real[ndimension];
   b=new real[ndimension];
   dim=new int[ndimension];
   dim[0]=idim;
   a[0]=a0x;
   b[0]=b0x;
   if (ndimension>1){
    a[1]=a0y;
    b[1]=b0y;
    dim[1]=jdim;
    if (ndimension>2){
      a[2]=a0z;
      b[2]=b0z;
      dim[2]=kdim;
    }
   }

   // Prepare working place

   if (ndimension==1) grid  =  LineMapping(a[0], b[0]);
   else if (ndimension==2) sq_grid =  SquareMapping(a[0],b[0],a[1],b[1]);
   niter = 1;
   mg    = new MappedGrid[nlevel];
   u     = new realMappedGridFunction[nlevel];
   Source= new realArray[nlevel];
   RHS   = new realArray[nlevel];
   w     = new realArray[nlevel];
   utmp  = new realArray[nlevel];
   Ig1   = new Index[nlevel];
   Ig2   = new Index[nlevel];
   Ig3   = new Index[nlevel];
   I1    = new Index[nlevel];
   I2    = new Index[nlevel];
   I3    = new Index[nlevel];
   Iint1 = new Index[nlevel];
   Iint2 = new Index[nlevel];
   Iint3 = new Index[nlevel];
   dx    = new real[nlevel];
   for (i=0;i<4;i++){
    P[i]= new realArray[nlevel];
    Q[i]= new realArray[nlevel];
    CoeffInterp[i] = new realArray[nlevel];
   }
   for (int side=0;side<=1;side++){
    Xee0[side]= new realArray[nlevel];
    Xcc0[side]= new realArray[nlevel];
   }
   if (ndimension>1){
     dy = new real[nlevel];
     if (ndimension > 2) dz=new real[nlevel];
   }
 }

 multigrid2::
   ~multigrid2(){
    int i;

    delete[] mg,      mg=NULL;
    delete[] u,       u=NULL;
    delete[] RHS,     RHS=NULL;
    delete[] Source,  Source=NULL;
    delete[] w  ,     w =NULL;
    delete[] utmp ,   utmp =NULL;
    delete[] Ig1,     Ig1=NULL;
    delete[] Ig2,     Ig2=NULL;
    delete[] Ig3,     Ig3=NULL;
    delete[] I1,      I1=NULL;
    delete[] I2,      I2=NULL;
    delete[] I3,      I3=NULL;
    delete[] Iint1,   Iint1=NULL;
    delete[] Iint2,   Iint2=NULL;
    delete[] Iint3,   Iint3=NULL;
    delete[] dx,      dx=NULL;
    delete[] Xee0[0],   Xee0[0]=NULL;
    delete[] Xee0[1],   Xee0[1]=NULL;
    delete[] Xcc0[0],   Xcc0[0]=NULL;
    delete[] Xcc0[1],   Xcc0[1]=NULL;
    for (i=0;i<4;i++){
     delete[] P[i],  P[i]=NULL;
     delete[] Q[i],  Q[i]=NULL;
     delete[] CoeffInterp[i], CoeffInterp[i]=NULL;
    }
    if (ndimension>1){
      delete[] dy, dy=NULL;
      if (ndimension > 2) delete[] dz, dz=NULL;
    }
    if (numberOfPointattractions>0){
      if (ndimension==1)
        DeleteIntArray2d(PointAttraction,0,numberOfPointattractions-1,0,0);
      else if (ndimension == 2)
        DeleteIntArray2d(PointAttraction,0,numberOfPointattractions-1,0,1);
      else if (ndimension == 3)
        DeleteIntArray2d(PointAttraction,0,numberOfPointattractions-1,0,2);
      delete[] APointcoeff, APointcoeff=NULL;
      delete[] CPointcoeff, CPointcoeff=NULL;
    }
    if (numberOfIlineattractions>0){
      delete[] ILineAttraction, ILineAttraction=NULL;
      delete[] AIlinecoeff, AIlinecoeff=NULL;
      delete[] CIlinecoeff, CIlinecoeff=NULL;
    }
    if (numberOfJlineattractions>0){
      delete[] JLineAttraction, JLineAttraction=NULL;
      delete[] AJlinecoeff, AJlinecoeff=NULL;
      delete[] CJlinecoeff, CJlinecoeff=NULL;
    }
    if (numberOfKlineattractions>0){
      delete[] KLineAttraction, KLineAttraction=NULL;
      delete[] AKlinecoeff, AKlinecoeff=NULL;
      delete[] CKlinecoeff, CKlinecoeff=NULL;
    }
  }

realArray multigrid2::
SignOf(realArray uarray){
  realArray u1;

  u1.redim(uarray);
  u1=0.0;
  where(uarray>0.0) u1=1.0;
  elsewhere(uarray<0.0) u1=-1.0;

  return u1;
 }

 void multigrid2::
   chooseMethod(int itmp){
   //This routine not used for the moment.
   //It was for setting some options
     int nlevelmax=nlevel;
     aString name(80), answer(80);
     char line[80];
     NameList nl;

     smoothingMethod=1;
     switch (ndimension){
      case 1: 
	     omega=2.0/3.0;
	     break;

      case 2: 
	     omega=4.0/5.0;
	     break;
   
      default:
	     printf("Untreated case\n");
	     exit(1);
     }
  printf(
   "Parameters for Multigrid: \n"
   "------------------------- \n"
   " Name                     type        default  \n"
   "Maxiter                   (int)        %i      \n"
   "Multigrid_levels          (int)        %i      (1,...,%i)\n"
   "smoothingMethod           (int)        %i      (1:Jac 2:RB 3:Line 4:Zebra)\n"
   , Maxiter,nlevel,nlevel,smoothingMethod);

   for (;;){
    cout <<" Change parameters, exit to continue \n";
    cin >> answer;
    if (answer == "exit") break;
    nl.getVariableName(answer, name);
    if ((name == "smoothingMethod")||(name == "smoothing")) {
      smoothingMethod=nl.intValue(answer);
      if ((smoothingMethod == 1)||(smoothingMethod ==2)){
	if (smoothingMethod == 2){
	  omega = 1.0;
	  /*if (ndimension==1) Maxiter=1;*/
          if ((nlevel == nlevelmax)&&(itmp != 1)){
           printf("\n\nCannot use RedBlack or Zebra at "
                  "this level:%i\n",nlevel);
           printf("Using 1:Jacobi\n\n");
           smoothingMethod=1, omega=4.0/5.0;
          }
        }
	cout <<"Enter Jacobi under-relaxation coefficient: (default = "<<omega<<")\n";
	fgets(line, sizeof(line),stdin);
        sScanF(line, "%f",&omega); 	
      }
      if (smoothingMethod==4){
        if ((nlevel == nlevelmax)&&(itmp != 1)){
          printf("\n\n!!! Cannot use RedBlack or Zebra at "
               "this level:%i!!!\n"
	       "Using LineSolver\n\n",nlevel);
          smoothingMethod=3;
        }
      }
      if ((smoothingMethod > 2) && (ndimension == 1)){
        nlevel=1;
        Maxiter=1;
	if (smoothingMethod == 4){
	  cout <<" Cannot use Zebra for 1D problem\n";
	  cout <<" Using LineSolver\n";
	  smoothingMethod=3;
        }
      }
      if (smoothingMethod > 2){
	omega=1.0;
	//cout <<"Enter under-relaxation coefficient: (default = "<<omega<<")\n";
	//fgets(line, sizeof(line),stdin);
        //sScanF(line, "%f",&omega); 	
      }
      if ((smoothingMethod != 1)&&(smoothingMethod != 2)&&(smoothingMethod !=3)&&
	  (smoothingMethod != 4)){
       cout<<"Unknown Method "<<smoothingMethod<<". Using 1:Jacobi\n";
       smoothingMethod=1;
      }
    }
    else if ((name=="Multigrid_levels")||(name=="nlevel")){ 
       nlevel=nl.intValue(answer);
       if (nlevel==1) printf("\n \nMultigrid Method is not used\n"
                             "Using one level method \n \n \n");
    }
    else if (name == "Maxiter") Maxiter=nl.intValue(answer);
    else cout << "Unknown response: ["<<name<<"]\n";
   }
   //cout <<" Enter debug mode (0 or 1) : default = "<<TEST_RESIDUAL<<"\n";
   //fgets(line,sizeof(line),stdin);
   //if (line !="") sscanf(line, "%i ",&TEST_RESIDUAL);
     if (smoothingMethod>=3){
       cout <<" Use BlockTridiagonal (0 or 1) : default="<<useBlockTridiag<<"\n";
       fgets(line,sizeof(line),stdin);
       if (line !="") sscanf(line, "%i ",&useBlockTridiag);
       if ((useBlockTridiag==1)&&(smoothingMethod<3)){
         printf("!!! Block tridiagonal is used only for Line !!!\n"
	        "!!! Solver or Zebra !!!\n");
         printf("!!! Using Line solver !!!\n");
         smoothingMethod=3;
       }
     }
   if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
       (gridBc(1,0)==3)||(gridBc(1,1)==3)){
     cout <<"Enter underelaxation coefficient for source terms: default="<<omega1<<"\n";
     fgets(line,sizeof(line),stdin);
     if (line !="") sScanF(line, "%f",&omega1);
   }
 }

void multigrid2::
 get2DBoundaryResidual(int i, realArray &resid1, realArray *up){
   // Computes the residual on the boundary in the case
   // of orthogonal grid boundary condition
   realArray Xc_e, Yc_e;
   Index I11, I22, I33;
   int istart, istop, jstart,jstop, kstart,kstop;
   IntegerArray index(2,3);
   int is[3] = {0,0,0}; // To control the sign according to side
   int icdif[3] = {0,0,0}; // To control centered differences

   index(0,0)=I1[i].getBase();
   istart=index(0,0);
   index(1,0)=I1[i].getBound();
   istop=index(1,0);
   index(0,1)=I2[i].getBase();
   jstart=index(0,1);
   index(1,1)=I2[i].getBound();
   jstop=index(1,1);
   index(0,2)=I3[i].getBase();
   kstart=index(0,2);
   index(1,2)=I3[i].getBound();
   kstop=index(1,2);

   for (int axis=0; axis<2; axis++)
    for (int side=0; side<=1; side++){
      getBoundaryIndex(index,side,axis,I11,I22,I33);
      if (gridBc(side,axis)==2){
	is[0]=0, is[1]=0, is[2]=0;
	is[axis]=1-2*side;
	if (axis==0) {
	  icdif[0]=0; 
	  icdif[1]=1; 
	  //if (rangeDimension==3) icdif[2]=1;
	  //else icdif[2]=0;
        }
	else if (axis==1){
	 icdif[0]=1;
	 icdif[1]=0;
	 //if (rangeDimension==3) icdif[2]=1;
	 //else icdif[2]=0;
        }
	Xc_e.redim(I11,I22,I33), Yc_e.redim(I11,I22,I33);
	Xc_e(I11,I22,I33)=uprev[i](I11+icdif[0],I22+icdif[1],I33,0)-
	                  uprev[i](I11-icdif[0],I22-icdif[1],I33,0);
	Yc_e(I11,I22,I33)=uprev[i](I11+icdif[0],I22+icdif[1],I33,1)-
	                  uprev[i](I11-icdif[0],I22-icdif[1],I33,1);
        if (useBlockTridiag != 1){
          if (i==0){
            resid1(I11,I22,I33,0)=
               -Xc_e(I11,I22,I33)*u[i](I11+is[0],I22+is[1],I33,0)-
                Yc_e(I11,I22,I33)*u[i](I11+is[0],I22+is[1],I33,1)+
                Xc_e(I11,I22,I33)*u[i](I11,I22,I33,0)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,1);
            resid1(I11,I22,I33,1)=
                Xc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,1)-
	        Yc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,0)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0)-
	        Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1);
          }
          else {
            resid1(I11,I22,I33,0)=
	       RHS[i](I11,I22,I33,0)-
	          Xc_e(I11,I22,I33)*(u[i](I11+is[0],I22+is[1],I33,0)-
			        u[i](I11,I22,I33,0))-
	          Yc_e(I11,I22,I33)*(u[i](I11+is[0],I22+is[1],I33,1)-
			        u[i](I11,I22,I33,1));
            resid1(I11,I22,I33,1)=
	       RHS[i](I11,I22,I33,1)-
	          Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	          Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
          }
        }
        else{
          if (i==0){
            where (fabs(Xc_e(I11,I22,I33))>0.00001){
	      resid1(I11,I22,I33,0)=
	       -Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
			      u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
		              u[i](I11+is[0],I22+is[1],I33,1));
              resid1(I11,I22,I33,1)=
	       -Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0)+
	        Xc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,1)-
	        Yc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,0);
            }
            elsewhere(){
	      resid1(I11,I22,I33,0)=
	       -Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0)+
	        Xc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,1)-
	        Yc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,0);
              resid1(I11,I22,I33,1)=
	       -Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
			      u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
	                      u[i](I11+is[0],I22+is[1],I33,1));
            }
          }
          else{
	    where (fabs(Xc_e(I11,I22,I33))>0.00001){
	      resid1(I11,I22,I33,0)=
	        RHS[i](I11,I22,I33,0)-
	        Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
			      u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
			      u[i](I11+is[0],I22+is[1],I33,1));
              resid1(I11,I22,I33,1)=
	        RHS[i](I11,I22,I33,1)-
	        Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
	    }
	    elsewhere(){
	      resid1(I11,I22,I33,0)=
	        RHS[i](I11,I22,I33,0)-
	        Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
              resid1(I11,I22,I33,1)=
	        RHS[i](I11,I22,I33,1)-
	        Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
			      u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
			      u[i](I11+is[0],I22+is[1],I33,1));
	    }
          }
        }
      }
      where((GRIDBC[2*axis+side][i](I11,I22,I33)==1)||
	    (GRIDBC[2*axis+side][i](I11,I22,I33)==10)){
 	resid1(I11,I22,I33,0)=0.;
  	resid1(I11,I22,I33,1)=0.;
      }
    }
    resid1(istart,jstart,kstart,Range(0,1))=0.;
    resid1(istop,jstart,kstart,Range(0,1))=0.;
    resid1(istart,jstop,kstart,Range(0,1))=0.;
    resid1(istop,jstop,kstart,Range(0,1))=0.;
 }


void multigrid2::
   getResidual(realArray &resid1, int i, realArray* up){
   //Computes the residual only at interior points
    realArray coeff1, coeff2, coeff3,coeff4, coeff5;
    int j, istart, istop, jstart, jstop;

    resid1=0.0;
    switch (ndimension){
     case 1:
	 resid1(Iint1[i],Iint2[i],Iint3[i],0)=
	 -((u[i](Iint1[i]+1,Iint2[i],Iint3[i],0)-2.0*
	    u[i](Iint1[i],Iint2[i],Iint3[i],0)+
	    u[i](Iint1[i]-1,Iint2[i],Iint3[i],0))/(dx[i]*dx[i])+
           (u[i](Iint1[i]+1,Iint2[i],Iint3[i],0)-
	    u[i](Iint1[i]-1,Iint2[i],Iint3[i],0))/(2.0*dx[i])*
	    Source[i](Iint1[i],Iint2[i],Iint3[i],0))+
	    RHS[i](Iint1[i],Iint2[i],Iint3[i],0);
         break;

     case 2:
	 coeff1.redim(I1[i], I2[i], I3[i]);
	 coeff2.redim(I1[i], I2[i], I3[i]);
	 coeff3.redim(I1[i], I2[i], I3[i]);
	 getSource(i,0);
	 find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,
	       coeff5,I1[i],I2[i],I3[i],i,0);
	 if (useBlockTridiag != 1){
	   for (j=0;j<ndimension;j++){
	     resid1(Iint1[i], Iint2[i], Iint3[i],j) = 
	     -(coeff1(Iint1[i],Iint2[i],Iint3[i])*
	       ((u[i](Iint1[i]+1,Iint2[i],Iint3[i],j)-2.0*
	         u[i](Iint1[i],Iint2[i],Iint3[i],j)+
		 u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
	       Source[i](Iint1[i], Iint2[i], Iint3[i],0)*
	        (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j)-
	         u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/(2.0*dx[i]))+
	       coeff2(Iint1[i],Iint2[i],Iint3[i])*
               ((u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-2.0*
	         u[i](Iint1[i],Iint2[i],Iint3[i],j)+
	         u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
		Source[i](Iint1[i], Iint2[i], Iint3[i],1)*
		(u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-
		 u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/(2.0*dy[i]))-
	       coeff3(Iint1[i], Iint2[i], Iint3[i])*
	       (u[i](Iint1[i]+1,Iint2[i]+1,Iint3[i],j)-
	        u[i](Iint1[i]-1,Iint2[i]+1,Iint3[i],j)-
                u[i](Iint1[i]+1,Iint2[i]-1,Iint3[i],j)+
	        u[i](Iint1[i]-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]))+
	     RHS[i](Iint1[i], Iint2[i],Iint3[i],j);
	     if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic){
	      istart=I1[i].getBase(), istop=I1[i].getBound();
	      resid1(istart,Iint2[i], Iint3[i],j) =
	      -(coeff1(istart,Iint2[i],Iint3[i])*
	        ((u[i](istart+1,Iint2[i],Iint3[i],j)-2.0*
	          u[i](istart,Iint2[i],Iint3[i],j)+
	 	 u[i](istop-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
	        Source[i](istart, Iint2[i], Iint3[i],0)*
	         (u[i](istart+1,Iint2[i],Iint3[i],j)-
	          u[i](istop-1,Iint2[i],Iint3[i],j))/(2.0*dx[i]))+
	        coeff2(istart,Iint2[i],Iint3[i])*
                ((u[i](istart,Iint2[i]+1,Iint3[i],j)-2.0*
	          u[i](istart,Iint2[i],Iint3[i],j)+
	          u[i](istart,Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
	 	Source[i](istart, Iint2[i], Iint3[i],1)*
	 	(u[i](istart,Iint2[i]+1,Iint3[i],j)-
	 	 u[i](istart,Iint2[i]-1,Iint3[i],j))/(2.0*dy[i]))-
	        coeff3(istart, Iint2[i], Iint3[i])*
	        (u[i](istart+1,Iint2[i]+1,Iint3[i],j)-
	         u[i](istop-1,Iint2[i]+1,Iint3[i],j)-
                 u[i](istart+1,Iint2[i]-1,Iint3[i],j)+
	         u[i](istop-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]))+
	      RHS[i](istart, Iint2[i],Iint3[i],j);

	      resid1(istop,Iint2[i], Iint3[i],j) =
		 resid1(istart,Iint2[i], Iint3[i],j);
	     }

	     if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic){
	      jstart=I2[i].getBase(), jstop=I2[i].getBound();
	      resid1(Iint1[i],jstart, Iint3[i],j) =
	      -(coeff1(Iint1[i],jstart,Iint3[i])*
	        ((u[i](Iint1[i]+1,jstart,Iint3[i],j)-2.0*
	          u[i](Iint1[i],jstart,Iint3[i],j)+
	 	  u[i](Iint1[i]-1,jstart,Iint3[i],j))/(dx[i]*dx[i])+
	        Source[i](Iint1[i],jstart, Iint3[i],0)*
	         (u[i](Iint1[i]+1,jstart,Iint3[i],j)-
	          u[i](Iint1[i]-1,jstart,Iint3[i],j))/(2.0*dx[i]))+
	        coeff2(Iint1[i],jstart,Iint3[i])*
                ((u[i](Iint1[i],jstart+1,Iint3[i],j)-2.0*
	          u[i](Iint1[i],jstart,Iint3[i],j)+
	          u[i](Iint1[i],jstop-1,Iint3[i],j))/(dy[i]*dy[i])+
	 	Source[i](Iint1[i], jstart, Iint3[i],1)*
	 	(u[i](Iint1[i],jstart+1,Iint3[i],j)-
	 	 u[i](Iint1[i],jstop-1,Iint3[i],j))/(2.0*dy[i]))-
	        coeff3(Iint1[i], jstart, Iint3[i])*
	        (u[i](Iint1[i]+1,jstart+1,Iint3[i],j)-
	         u[i](Iint1[i]-1,jstart+1,Iint3[i],j)-
                 u[i](Iint1[i]+1,jstop-1,Iint3[i],j)+
	         u[i](Iint1[i]-1,jstop-1,Iint3[i],j))/(4.0*dx[i]*dy[i]))+
	      RHS[i](Iint1[i], jstart,Iint3[i],j);

	      resid1(Iint1[i],jstop, Iint3[i],j) =
		 resid1(Iint1[i],jstart, Iint3[i],j);
	     }
            }
	   }
	   else {
	   //printf("Dans getResidual\n");
	   findPQ(u[i],coeff1,coeff2,I1[i],I2[i],I3[i],i,1,0);
       //for (int j1=0;j1<=I2[i].getBound();j1++)
	//for (int i1=0; i1<=I1[i].getBound(); i1++){
	 //if (j1<=2) printf("i=%i\t j=%i\t P=%g\t Q=%g\t P11=%g\t P12=%g\t Q11=%g\t Q12=%g\n", i1,j1,P[2][i](i1,j1,0), Q[2][i](i1,j1,0), P1[2](i1,j1,0,0),P1[2](i1,j1,0,1),Q1[2](i1,j1,0,0),Q1[2](i1,j1,0,1));
	 //else printf("i=%i\t j=%i\t P=%g\t Q=%g\n", i1,j1,P[2][i](i1,j1,0), Q[2][i](i1,j1,0));
	 //}
	   //for (int j1=I2[i].getBase(); j1<=I2[i].getBound();j1++)
	     //for (int i1=I1[i].getBase(); i1<=I1[i].getBound();i1++)
	       //printf("i=%i\t j=%i\t P=%g\t Q=%g\n",i1,j1,P[2][i](i1,j1,0),
		  //Q[2][i](i1,j1,0));
	   for (j=0;j<ndimension;j++){
	    resid1(Iint1[i], Iint2[i], Iint3[i],j) =
	     -(coeff1(Iint1[i],Iint2[i],Iint3[i])*
	       (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j)-2.0*
		 u[i](Iint1[i],Iint2[i],Iint3[i],j)+
		 u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
                (coeff1(Iint1[i],Iint2[i],Iint3[i])*
		 Source[i](Iint1[i],Iint2[i],Iint3[i],0)/(2.0*dx[i])-
		 (P[0][i](Iint1[i],Iint2[i],Iint3[i])+
		  P[1][i](Iint1[i],Iint2[i],Iint3[i])+
		  P[2][i](Iint1[i],Iint2[i],Iint3[i])+
		  P[3][i](Iint1[i],Iint2[i],Iint3[i])))*
		(u[i](Iint1[i]+1,Iint2[i],Iint3[i],j)-
		 u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))+
               coeff2(Iint1[i],Iint2[i],Iint3[i])*
               (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-2.0*
		 u[i](Iint1[i],Iint2[i],Iint3[i],j)+
		 u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
		(coeff2(Iint1[i],Iint2[i],Iint3[i])*
		 Source[i](Iint1[i],Iint2[i],Iint3[i],1)/(2.0*dy[i])-
		 (Q[0][i](Iint1[i],Iint2[i],Iint3[i])+
		  Q[1][i](Iint1[i],Iint2[i],Iint3[i])+
		  Q[2][i](Iint1[i],Iint2[i],Iint3[i])+
		  Q[3][i](Iint1[i],Iint2[i],Iint3[i])))*
		(u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-
		 u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))-
               coeff3(Iint1[i],Iint2[i],Iint3[i])*
		(u[i](Iint1[i]+1,Iint2[i]+1,Iint3[i],j)-
		 u[i](Iint1[i]-1,Iint2[i]+1,Iint3[i],j)-
		 u[i](Iint1[i]+1,Iint2[i]-1,Iint3[i],j)+
		 u[i](Iint1[i]-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]))+
              RHS[i](Iint1[i], Iint2[i],Iint3[i],j);

             if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic){
	      istart=I1[i].getBase(), istop=I1[i].getBound();
	      resid1(istart,Iint2[i], Iint3[i],j) =
	      -(coeff1(istart,Iint2[i],Iint3[i])*
	        (u[i](istart+1,Iint2[i],Iint3[i],j)-2.0*
	         u[i](istart,Iint2[i],Iint3[i],j)+
	 	 u[i](istop-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
	        (coeff1(istart,Iint2[i],Iint3[i])*
		  Source[i](istart, Iint2[i], Iint3[i],0)/(2.0*dx[i])-
                  (P[0][i](istart,Iint2[i],Iint3[i])+
                   P[1][i](istart,Iint2[i],Iint3[i])+
                   P[2][i](istart,Iint2[i],Iint3[i])+
                   P[3][i](istart,Iint2[i],Iint3[i])))*
	           (u[i](istart+1,Iint2[i],Iint3[i],j)-
	            u[i](istop-1,Iint2[i],Iint3[i],j))+
	        coeff2(istart,Iint2[i],Iint3[i])*
                (u[i](istart,Iint2[i]+1,Iint3[i],j)-2.0*
	          u[i](istart,Iint2[i],Iint3[i],j)+
	          u[i](istart,Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
	 	(coeff2(istart,Iint2[i],Iint3[i])*
		  Source[i](istart, Iint2[i], Iint3[i],1)/(2.0*dy[i])-
                   (Q[0][i](istart,Iint2[i],Iint3[i])+
                    Q[1][i](istart,Iint2[i],Iint3[i])+
                    Q[2][i](istart,Iint2[i],Iint3[i])+
                    Q[3][i](istart,Iint2[i],Iint3[i])))*
	 	  (u[i](istart,Iint2[i]+1,Iint3[i],j)-
	 	   u[i](istart,Iint2[i]-1,Iint3[i],j))-
	        coeff3(istart, Iint2[i], Iint3[i])*
	        (u[i](istart+1,Iint2[i]+1,Iint3[i],j)-
	         u[i](istop-1,Iint2[i]+1,Iint3[i],j)-
                 u[i](istart+1,Iint2[i]-1,Iint3[i],j)+
	         u[i](istop-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]))+
	      RHS[i](istart, Iint2[i],Iint3[i],j);

	      resid1(istop,Iint2[i], Iint3[i],j) =
		 resid1(istart,Iint2[i], Iint3[i],j);
	     }

	     if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic){
	      jstart=I2[i].getBase(), jstop=I2[i].getBound();
	      resid1(Iint1[i],jstart, Iint3[i],j) =
	      -(coeff1(Iint1[i],jstart,Iint3[i])*
	        (u[i](Iint1[i]+1,jstart,Iint3[i],j)-2.0*
	         u[i](Iint1[i],jstart,Iint3[i],j)+
	 	 u[i](Iint1[i]-1,jstart,Iint3[i],j))/(dx[i]*dx[i])+
	        (coeff1(Iint1[i],jstart,Iint3[i])*
		 Source[i](Iint1[i],jstart, Iint3[i],0)/(2.0*dx[i])-
                 (P[0][i](Iint1[i],jstart,Iint3[i])+
                  P[1][i](Iint1[i],jstart,Iint3[i])+
                  P[2][i](Iint1[i],jstart,Iint3[i])+
                  P[3][i](Iint1[i],jstart,Iint3[i])))*
	         (u[i](Iint1[i]+1,jstart,Iint3[i],j)-
	          u[i](Iint1[i]-1,jstart,Iint3[i],j))+
	        coeff2(Iint1[i],jstart,Iint3[i])*
                (u[i](Iint1[i],jstart+1,Iint3[i],j)-2.0*
	         u[i](Iint1[i],jstart,Iint3[i],j)+
	         u[i](Iint1[i],jstop-1,Iint3[i],j))/(dy[i]*dy[i])+
	 	(coeff2(Iint1[i],jstart,Iint3[i])*
		 Source[i](Iint1[i], jstart, Iint3[i],1)/(2.0*dy[i])-
                 (Q[0][i](Iint1[i],jstart,Iint3[i])+
                  Q[1][i](Iint1[i],jstart,Iint3[i])+
                  Q[2][i](Iint1[i],jstart,Iint3[i])+
                  Q[3][i](Iint1[i],jstart,Iint3[i])))*
	 	(u[i](Iint1[i],jstart+1,Iint3[i],j)-
	 	 u[i](Iint1[i],jstop-1,Iint3[i],j))-
	        coeff3(Iint1[i], jstart, Iint3[i])*
	        (u[i](Iint1[i]+1,jstart+1,Iint3[i],j)-
	         u[i](Iint1[i]-1,jstart+1,Iint3[i],j)-
                 u[i](Iint1[i]+1,jstop-1,Iint3[i],j)+
	         u[i](Iint1[i]-1,jstop-1,Iint3[i],j))/(4.0*dx[i]*dy[i]))+
	      RHS[i](Iint1[i], jstart,Iint3[i],j);

	      resid1(Iint1[i],jstop, Iint3[i],j) =
		 resid1(Iint1[i],jstart, Iint3[i],j);
	     }
           }
	   /*** OLD FASHION 
	    realArray Xe, Xc, Xee, Xcc, normXe, normXc, Sign;
	    Xc.redim(I11,ndimension), Xcc.redim(I11,ndimension);
	    Xe.redim(I11,ndimension), Xee.redim(I11,ndimension);
	    Sign.redim(I11);
 
    Xc=0.0, Xe=0.0, Xcc=0.0, Xee=0.0, Sign=0.0, normXc=0.0, normXe=0.0;
    for (j=0;j<ndimension;j++){
      Xc(Iint1[i],j)=(u[i](Iint1[i]+1,jstart,Iint3[i],j)-
                    u[i](Iint1[i]-1,jstart,Iint3[i],j))/(2.*dx[i]);
      Xe(Iint1[i],j)=(u[i](Iint1[i],jstart+1,Iint3[i],j)-
                    u[i](Iint1[i],jstart,Iint3[i],j))/dy[i];
      Xcc(Iint1[i],j)=(u[i](Iint1[i]+1,jstart,Iint3[i],j)-
                 2.0*u[i](Iint1[i],jstart,Iint3[i],j)+
                     u[i](Iint1[i]-1,jstart,Iint3[i],j))/(dx[i]*dx[i]);
    }
    normXc(Iint1[i])=Xc(Iint1[i],0)*Xc(Iint1[i],0)+Xc(Iint1[i],1)*Xc(Iint1[i],1);
    Sign=SignOf(Xe(Iint1[i],0)*Xc(Iint1[i],1)-Xe(Iint1[i],1)*Xc(Iint1[i],0));
 
    Xe(Iint1[i],0)=dB(0,1)*Sign(Iint1[i])*Xc(Iint1[i],1)/sqrt(normXc(Iint1[i]));
    Xe(Iint1[i],1)=-dB(0,1)*Sign(Iint1[i])*Xc(Iint1[i],0)/sqrt(normXc(Iint1[i]));
 
    Xe0=Xe;
    for (j=0;j<ndimension;j++)
      Xee(Iint1[i],j)=(-7.*u[i](Iint1[i],jstart,Iint3[i],j) +
                      8.*u[i](Iint1[i],jstart+1,Iint3[i],j) -
                      u[i](Iint1[i],jstart+2,Iint3[i],j)-6.0*dy[i]*Xe(Iint1[i],j))/
                      (2.*dy[i]*dy[i]);
    for (j=0;j<ndimension;j++){
     resid1(Iint1[i],1,Iint3[i] 
    }
	    OLD  FASHION ****/
          }
	  /********TRY
	   //Upwindind is used here
	   realArray Mat1(Iint1[i],Iint2[i],Iint3[i]), 
		     Mat2(Iint1[i],Iint2[i],Iint3[i]);
	   realArray Mat3(Iint1[i],Iint2[i],Iint3[i]), 
		     Den(Iint1[i],Iint2[i],Iint3[i]);

	   Mat1(Iint1[i],Iint2[i],Iint3[i])=coeff1(Iint1[i],Iint2[i],Iint3[i])*
			    (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j)-2.*
			     u[i](Iint1[i],Iint2[i],Iint3[i],j)+
			     u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/
			     (dx[i]*dx[i]);
	   Mat2(Iint1[i],Iint2[i],Iint3[i])=coeff2(Iint1[i],Iint2[i],Iint3[i])*
			    (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-2.*
			     u[i](Iint1[i],Iint2[i],Iint3[i],j)+
			     u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/
			     (dy[i]*dy[i]);
           Mat3(Iint1[i],Iint2[i],Iint3[i])=-coeff3(Iint1[i],Iint2[i],Iint3[i])*
			   (u[i](Iint1[i]+1,Iint2[i]+1,Iint3[i],j)-
			    u[i](Iint1[i]-1,Iint2[i]+1,Iint3[i],j)-
			    u[i](Iint1[i]+1,Iint2[i]-1,Iint3[i],j)+
			    u[i](Iint1[i]-1,Iint2[i]-1,Iint3[i],j))/
			    (4.0*dx[i]*dy[i]);
           where (Source[i](Iint1[i],Iint2[i],Iint3[i],0)>=0){
	     Mat1(Iint1[i],Iint2[i],Iint3[i]) += 
		    coeff1(Iint1[i],Iint2[i],Iint3[i])*
		    Source[i](Iint1[i],Iint2[i],Iint3[i],0)*
		    (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j)-
		     u[i](Iint1[i],Iint2[i],Iint3[i],j))/dx[i];
           }
           elsewhere (Source[i](Iint1[i],Iint2[i],Iint3[i],0)<0){
	     Mat1(Iint1[i],Iint2[i],Iint3[i]) += 
		    coeff1(Iint1[i],Iint2[i],Iint3[i])*
		    Source[i](Iint1[i],Iint2[i],Iint3[i],0)*
		    (u[i](Iint1[i],Iint2[i],Iint3[i],j)-
		     u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/dx[i];
           }
           where (Source[i](Iint1[i],Iint2[i],Iint3[i],1)>=0){
	     Mat2(Iint1[i],Iint2[i],Iint3[i]) += 
		    coeff2(Iint1[i],Iint2[i],Iint3[i])*
		    Source[i](Iint1[i],Iint2[i],Iint3[i],1)*
		    (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-
		     u[i](Iint1[i],Iint2[i],Iint3[i],j))/dy[i];
           }
           elsewhere (Source[i](Iint1[i],Iint2[i],Iint3[i],1)<0){
	     Mat2(Iint1[i],Iint2[i],Iint3[i]) += 
		    coeff2(Iint1[i],Iint2[i],Iint3[i])*
		    Source[i](Iint1[i],Iint2[i],Iint3[i],1)*
		    (u[i](Iint1[i],Iint2[i],Iint3[i],j)-
		     u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/dy[i];
           }

	   resid1(Iint1[i], Iint2[i], Iint3[i],j) =
	     -(Mat1(Iint1[i],Iint2[i],Iint3[i]) +
	       Mat2(Iint1[i],Iint2[i],Iint3[i]) +
	       Mat3(Iint1[i],Iint2[i],Iint3[i]))+
	       RHS[i](Iint1[i], Iint2[i],Iint3[i],j);
	   }
         }
	 TRY **************/
         if ((gridBc(0,0)==2)||(gridBc(0,1)==2)||(gridBc(1,0)==2)||
	     (gridBc(1,1)==2)) get2DBoundaryResidual(i,resid1,uprev);
         break;

      default:
       printf("Untreated condition\n");
       exit(1);
     }
   }

 int multigrid2::
   make2Power( int n){
    int i=0;
    if ((n<=1)&&(i==0)) return (1);
    else{
      while (!(((pow(2,i)+1)<n)&&((pow(2,i+1)+1)>=n)))
      i ++;
      return(int(pow(2,i+1))+1);
    }
  }

 void multigrid2::
   getSource(int i, int ichange){
    //Computes source terms when the simultaneous solver is
    //not used
    realArray IJKSrc(I1[i],I2[i],I3[i],ndimension);
    realArray ptemp(I1[i],I2[i],I3[i],3), ptemp2norm(I1[i],I2[i],I3[i]);
    realArray Ireal(I1[i]);
    int i11,j11,j,k,ipmax, jpmax,iqmax, jqmax,nperiod;
    int i0, i1, i2, j0, j1, j2, k0, k1, k2, np,isharp;
    real ptemp1,qtemp1;

    i0=I1[i].getBase(), i1=I1[i].getBound(), i2=I1[i].getStride();
    j0=I2[i].getBase(), j1=I2[i].getBound(), j2=I2[i].getStride();
    k0=I3[i].getBase(), k1=I3[i].getBound(), k2=I3[i].getStride();
    
    IJKSrc=0.0, ptemp=0.0, Ireal=0.0;
    Ireal.seqAdd(real(i0),real(i2)); 
    for (j=j0; j<=j1; j += j2) 
      IJKSrc(I1[i],j,k0,0)=Ireal(I1[i]);
    for (j=k0+k2; j<=k1; j += k2)
      IJKSrc(I1[i],I2[i],j,0)=IJKSrc(I1[i],I2[i],k0,0);
    
    if (ndimension>1){
     Ireal.redim(1,I2[i]);
     Ireal.seqAdd(real(j0),real(j2));
     for (j=i0; j<=i1; j += i2)
      IJKSrc(j,I2[i],k0,1)=Ireal(0,I2[i]);
     for (j=k0+k2; j<=k1; j += k2)
      IJKSrc(I1[i],I2[i],j,1)=IJKSrc(I1[i],I2[i],k0,1);
     if (ndimension>2){
      Ireal.redim(1,1,I3[i]);
      Ireal.seqAdd(real(k0),real(k2));
      for (j=i0; j<=i1; j += i2)
       IJKSrc(j,j0,I3[i],2)=Ireal(0,0,I3[i]);
      for (j=j0+j2;j<=j1; j += j2)
       IJKSrc(I1[i],j,I3[i],2)=IJKSrc(I1[i],j0,I3[i],2);
     }
    }

    Source[i]=0.0;

    for (j=0; j<numberOfPointattractions;j++){
      //First find P
      if ((gridBc(0,0)==-1)||(gridBc(1,0)==-1)) nperiod=numberOfPeriods;
      else nperiod=0;

     for (np=-nperiod;np<=nperiod;np++){
       ptemp2norm=0.0;
       // Compute \xi-\xi_i and eventually \eta - \eta_j and \zeta-\zeta_k
        ptemp(I1[i],I2[i],I3[i],0)=(IJKSrc(I1[i],I2[i],I3[i],0)-
				    (PointAttraction[j][0]-np*(dim[0]-1.))/
				     pow(2.0,real(i)))/
				      ((dim[0]-1.0)/pow(2.0,real(i)));
        ptemp(I1[i],I2[i],I3[i],1)=(IJKSrc(I1[i],I2[i],I3[i],1)-
				    PointAttraction[j][1]/pow(2.0,real(i)))/
				      ((dim[1]-1.0)/pow(2.0,real(i)));

       for (k=0;k<ndimension;k++) 
        ptemp2norm(I1[i],I2[i],I3[i]) += ptemp(I1[i],I2[i],I3[i],k)*
					   ptemp(I1[i],I2[i],I3[i],k);

       ptemp2norm = sqrt(ptemp2norm);

      Source[i](I1[i],I2[i],I3[i],0) +=
         -APointcoeff[j]*SignOf(ptemp(I1[i],I2[i],I3[i],0))*
	                               exp(-CPointcoeff[j]*ptemp2norm);
     }

     //Then find Q
     if ((gridBc(0,1)==-1)||(gridBc(1,1)==-1)) nperiod=numberOfPeriods;
     else nperiod=0;

     for (np=-nperiod;np<=nperiod;np++){
       ptemp2norm=0.0;
       // Compute \xi-\xi_i and eventually \eta - \eta_j and \zeta-\zeta_k
        ptemp(I1[i],I2[i],I3[i],1)=(IJKSrc(I1[i],I2[i],I3[i],1)-
				    (PointAttraction[j][1]-np*(dim[1]-1.))/
				     pow(2.0,real(i)))/
				      ((dim[1]-1.0)/pow(2.0,real(i)));
        ptemp(I1[i],I2[i],I3[i],0)=(IJKSrc(I1[i],I2[i],I3[i],0)-
				    PointAttraction[j][0]/pow(2.0,real(i)))/
				      ((dim[0]-1.0)/pow(2.0,real(i)));

       for (k=0;k<ndimension;k++) 
        ptemp2norm(I1[i],I2[i],I3[i]) += ptemp(I1[i],I2[i],I3[i],k)*
					   ptemp(I1[i],I2[i],I3[i],k);

       ptemp2norm = sqrt(ptemp2norm);

      Source[i](I1[i],I2[i],I3[i],1) +=
         -APointcoeff[j]*SignOf(ptemp(I1[i],I2[i],I3[i],1))*
	                               exp(-CPointcoeff[j]*ptemp2norm);
     }
    }
    if (ndimension==2){
     if ((gridBc(0,0)==-1)||(gridBc(1,0)==-1)) nperiod=numberOfPeriods;
     else nperiod=0.0;
     for (k=0;k<numberOfIlineattractions;k++){
       for (np=-nperiod;np<=nperiod;np++){
         ptemp(I1[i],I2[i],I3[i],0)=(IJKSrc(I1[i],I2[i],I3[i],0)-
				    (ILineAttraction[k]-np*(dim[0]-1))/
				     pow(2.0,real(i)))/
				     ((dim[0]-1.0)/pow(2.0,real(i)));
         Source[i](I1[i],I2[i],I3[i],0) += -AIlinecoeff[k]*
	                           SignOf(ptemp(I1[i],I2[i],I3[i],0))*
                  exp(-CIlinecoeff[k]*fabs(ptemp(I1[i],I2[i],I3[i],0)));
       }
     }

     if ((gridBc(0,1)==-1)||(gridBc(1,1)==-1)) nperiod=numberOfPeriods;
     else nperiod=0;
     for (k=0;k<numberOfJlineattractions;k++){
       for (np=-nperiod;np<=nperiod;np++){
         ptemp(I1[i],I2[i],I3[i],1)=(IJKSrc(I1[i],I2[i],I3[i],1)-
				     (JLineAttraction[k]-np*(dim[1]-1.))/
				     pow(2.0,real(i)))/
				     ((dim[1]-1.0)/pow(2.0,real(i)));
         Source[i](I1[i],I2[i],I3[i],1) += -AJlinecoeff[k]*
	                           SignOf(ptemp(I1[i],I2[i],I3[i],1))*
                  exp(-CJlinecoeff[k]*fabs(ptemp(I1[i],I2[i],I3[i],1)));
       }
     }
    } 

    if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||(gridBc(1,0)==3)||
       (gridBc(1,1)==3)){
      if (gridBc(0,0)==3){
       CoeffInterp[0][i](I1[i],I2[i],I3[i]) = 
         ((real(i1)-IJKSrc(I1[i],I2[i],I3[i],0))/real(i1))*
         exp(-lambda*IJKSrc(I1[i],I2[i],I3[i],0)/real(i1));
        }
      if (gridBc(0,1)==3){
       CoeffInterp[2][i](I1[i],I2[i],I3[i])=
		  ((real(j1)-IJKSrc(I1[i],I2[i],I3[i],1))/real(j1))*
		  exp(-lambda*IJKSrc(I1[i],I2[i],I3[i],1)/real(j1));
      }
      if (gridBc(1,0)==3){
       CoeffInterp[1][i](I1[i],I2[i],I3[i]) = 
	      (IJKSrc(I1[i],I2[i],I3[i],0)/real(i1))*
	      exp(-lambda*(real(i1)-IJKSrc(I1[i],I2[i],I3[i],0))/real(i1));
        }
      if (gridBc(1,1)==3){
       CoeffInterp[3][i](I1[i],I2[i],I3[i]) = 
	      (IJKSrc(I1[i],I2[i],I3[i],1)/real(j1))*
	      exp(-lambda*(real(j1)-IJKSrc(I1[i],I2[i],I3[i],1))/real(j1));
      }
      if (useBlockTridiag==0){
        realArray Xc, Xe, Xcc, Xee, normXc, normXe, Sign;
        realArray ptmp;
        static realArray ptmp0[4];
	intArray index(2,3);
	Index I111,I222,I333;

	index(0,0)=I1[i].getBase(), index(1,0)=I1[i].getBound();
	index(0,1)=I2[i].getBase(), index(1,1)=I2[i].getBound();
	index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
	int is[3], icdif[3], axis,side;
        Index Rtmp=Range(0,1);
	for (axis=0;axis<=1;axis++)
	 for (side=0;side<=1;side++){
	  is[0]=0, is[1]=0, is[2]=0;
	  is[axis]=1-2*side;
	  if (axis==0) icdif[0]=0, icdif[1]=1;
	  if (axis==1) icdif[0]=1, icdif[1]=0;
	  getBoundaryIndex(index,side,axis,I111,I222,I333);
	// omega0=1.0; For under relaxed jacobi
          if (gridBc(side,axis)==3){
            Xc.redim(I111,I222,I333,ndimension);
	    Xe.redim(I111,I222,I333,ndimension);
            normXc.redim(I111,I222,I333);
	    normXe.redim(I111,I222,I333);
            Xcc.redim(I111,I222,I333,ndimension);
	    Xee.redim(I111,I222,I333,ndimension);
            Sign.redim(I111,I222,I333);
            ptmp.redim(I1[i],I2[i],I3[i],ndimension);
	   
	    /**** For underelaxed jacobi ****/
	    if (iter<=0){
	      ptmp0[i].redim(I1[i],I2[i],I3[i],ndimension);
	      ptmp0[i]=1.0;
	      ptmp=ptmp0[i];
	    }
            else if (ichange != 0){
	     /***** For underelaxed jacobi *********/ 
	      Xc=0., Xe=0., Xcc=0., Xee=0.,Sign=0., ptmp=0.;
              if (axis==1){
	        Xc(I111,I222,I333,Rtmp)=(u[i](I111+1,I222,I333,Rtmp)-
	         u[i](I111-1,I222,I333,Rtmp))/(2.*dx[i]);
	        Xe(I111,I222,I333,Rtmp)=
			real(is[1])*(u[i](I111,I222+is[1],I333,Rtmp)-
	         u[i](I111,I222,I333,Rtmp))/dy[i];
	        Xcc(I111,I222,I333,Rtmp)=(u[i](I111+1,I222,I333,Rtmp)-
		             2.0*u[i](I111,I222,I333,Rtmp)+
		             u[i](I111-1,I222,I333,Rtmp))/(dx[i]*dx[i]);

                normXc(I111,I222,I333)=sqrt(Xc(I111,I222,I333,0)*
					 Xc(I111,I222,I333,0)+
					 Xc(I111,I222,I333,1)*
					 Xc(I111,I222,I333,1));
              Sign=SignOf(Xe(I111,I222,I333,0)*Xc(I111,I222,I333,1)-
			 Xe(I111,I222,I333,1)*Xc(I111,I222,I333,0));

              Xe(I111,I222,I333,0)=dB(side,axis)*Sign(I111,I222,I333)*
			  Xc(I111,I222,I333,1)/normXc(I111,I222,I333);
              Xe(I111,I222,I333,1)=-dB(side,axis)*Sign(I111,I222,I333)*
			  Xc(I111,I222,I333,0)/normXc(I111,I222,I333);

              Xee(I111,I222,I333,Rtmp)=(-7.*u[i](I111,I222,I333,Rtmp) + 
			         8.*u[i](I111,I222+is[1],I333,Rtmp) -
			         u[i](I111,I222+2*is[1],I333,Rtmp)-
                                 real(is[1])*6.0*dy[i]*
				 Xe(I111,I222,I333,Rtmp))/(2.*dy[i]*dy[i]);
	      /********
	      ((Xc(I1[i],0)*Xcc(I1[i],0)+Xc(I1[i],1)*Xcc(I1[i],1))/
	       (normXc(I1[i])*normXc(I1[i]))).display("Le premier");
              ((Xc(I1[i],0)*Xee(I1[i],0)+Xc(I1[i],1)*Xee(I1[i],1))/
	       (dB(1,0)*dB(1,0))).display("Le deuxieme");
	      ((Xe(I1[i],0)*Xcc(I1[i],0)+Xe(I1[i],1)*Xcc(I1[i],1))/
	       (normXc(I1[i])*normXc(I1[i]))).display("Le troisieme");
              ((Xe(I1[i],0)*Xee(I1[i],0)+Xe(I1[i],1)*Xee(I1[i],1))/
	       (dB(1,0)*dB(1,0))).display("Le quatrieme");
	       ********/

               ptmp(I111,I222,I333,0)=
		       -(Xc(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
                         Xc(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
	                (normXc(I111,I222,I333)*normXc(I111,I222,I333))-
                        (Xc(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		         Xc(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
			    (dB(side,axis)*dB(side,axis));
               ptmp(I111,I222,I333,1)=
		       -(Xe(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
                         Xe(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		        (normXc(I111,I222,I333)*normXc(I111,I222,I333))-
                        (Xe(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		         Xe(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
			    (dB(side,axis)*dB(side,axis));
              }
	      else if (axis==0){
	       Xe(I111,I222,I333,Rtmp)=(u[i](I111, I222+1,I333,Rtmp)-
		             u[i](I111,I222-1,I333,Rtmp))/(2.*dy[i]);
	       Xc(I111,I222,I333,Rtmp)=
		real(is[0])*(u[i](I111+is[0],I222,I333,Rtmp)-
		             u[i](I111,I222,I333,Rtmp))/dx[i];
	        Xee(I111,I222,I333,Rtmp)=(u[i](I111,I222+1,I333,Rtmp)-
		             2.0*u[i](I111,I222,I333,Rtmp)+
		             u[i](I111,I222-1,I333,Rtmp))/(dy[i]*dy[i]);
              //Xc.display("C'est Xc");
              //Xcc.display("C'est Xcc");

              normXe(I111,I222,I333)=sqrt(Xe(I111,I222,I333,0)*
				       Xe(I111,I222,I333,0)+
				       Xe(I111,I222,I333,1)*
				       Xe(I111,I222,I333,1));
              Sign=SignOf(Xc(I111,I222,I333,0)*Xe(I111,I222,I333,1)-
		       Xc(I111,I222,I333,1)*Xe(I111,I222,I333,0));

              Xc(I111,I222,I333,0)=dB(side,axis)*Sign(I111,I222,I333)*
			  Xe(I111,I222,I333,1)/normXe(I111,I222,I333);
              Xc(I111,I222,I333,1)=-dB(side,axis)*Sign(I111,I222,I333)*
			  Xe(I111,I222,I333,0)/normXe(I111,I222,I333);
              
	      Xcc(I111,I222,I333,Rtmp)=(-7.*u[i](I111,I222,I333,Rtmp) + 
			  8.*u[i](I111+is[0],I222,I333,Rtmp) -
			  u[i](I111+2*is[0],I222,I333,Rtmp)-
                          real(is[0])*6.0*dx[i]*Xc(I111,I222,I333,Rtmp))/
			  (2.*dx[i]*dx[i]);
             
	   /********
	   ((Xc(I1[i],0)*Xcc(I1[i],0)+Xc(I1[i],1)*Xcc(I1[i],1))/
	    (normXc(I1[i])*normXc(I1[i]))).display("Le premier");
           ((Xc(I1[i],0)*Xee(I1[i],0)+Xc(I1[i],1)*Xee(I1[i],1))/
	    (dB(1,0)*dB(1,0))).display("Le deuxieme");
	   ((Xe(I1[i],0)*Xcc(I1[i],0)+Xe(I1[i],1)*Xcc(I1[i],1))/
	    (normXc(I1[i])*normXc(I1[i]))).display("Le troisieme");
           ((Xe(I1[i],0)*Xee(I1[i],0)+Xe(I1[i],1)*Xee(I1[i],1))/
	    (dB(1,0)*dB(1,0))).display("Le quatrieme");
	    ********/

            ptmp(I111,I222,I333,0)=
		 -(Xc(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
                   Xc(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		       (dB(side,axis)*dB(side,axis))-
                  (Xc(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		   Xc(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
		(normXe(I111,I222,I333)*normXe(I111,I222,I333));
            ptmp(I111,I222,I333,1)=
		 -(Xe(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
                   Xe(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		       (dB(side,axis)*dB(side,axis))-
                  (Xe(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		   Xe(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
		(normXe(I111,I222,I333)*normXe(I111,I222,I333));
	   }
           
	   for (j11=I222.getBase();j11<=I222.getBound();j11++)
	    for (i11=I111.getBase();i11<=I111.getBound();i11++)
	      if (((u[i](i11+icdif[0],j11+icdif[1],0,0)-u[i](i11,j11,0,0))*
		   (u[i](i11-icdif[0],j11-icdif[1],0,0)-u[i](i11,j11,0,0))+
		   (u[i](i11+icdif[0],j11,0,1)-u[i](i11,j11,0,1))* 
		   (u[i](i11-icdif[0],j11,0,1)-u[i](i11,j11,0,1)))>0.000){
              if (((i11<i1)&&(i11>i0))||((j11<j1)&&(j11>j0))){
	        ptmp(i11,j11,0,0)=
		      0.5*(ptmp(i11+icdif[0],j11+icdif[1],0,0)+
			   ptmp(i11-icdif[0],j11-icdif[1],0,0));
	        ptmp(i11,j11,0,1)=
		      0.5*(ptmp(i11+icdif[0],j11+icdif[1],0,1)+
			   ptmp(i11-icdif[0],j11-icdif[1],0,1));
	      }
              else if (((i11==i0)&&(axis==1))||((j11==j0)&&(axis==0))){
	        if (axis==1){
		  ptmp(i11,j11,0,0)=
		      0.5*(ptmp(i11+2,j11,0,0)+
			   ptmp(i1-2,j11,0,0));
                  ptmp(i11+1,j11,0,0)=ptmp(i11,j11,0,0);
	          ptmp(i11,j11,0,1)=0.5*(ptmp(i11+2,j11,0,1)+
				      ptmp(i1-2,j11,0,1));
	          ptmp(i11+1,j11,0,1)=ptmp(i11,j11,0,1);
                }
	        if (axis==0){
		  ptmp(i11,j11,0,0)=
		      0.5*(ptmp(i11,j11+2,0,0)+
			   ptmp(i11,j1-2,0,0));
                  ptmp(i11,j11+1,0,0)=ptmp(i11,j11,0,0);
	          ptmp(i11,j11,0,1)=0.5*(ptmp(i11,j11+2,0,1)+
				      ptmp(i11,j1-2,0,1));
	          ptmp(i11,j11+1,0,1)=ptmp(i11,j11,0,1);
                }
	      }
              else if (((i11==i1)&&(axis==1))||((j11==j1)&&(axis==0))){
	        if (axis==1){
		  ptmp(i11,j11,0,0)=0.5*(ptmp(i11-2,j11,0,0)+
					 ptmp(i0+2,j11,0,0));
                  ptmp(i11-1,j11,0,0)=ptmp(i11,j11,0,0);
	          ptmp(i11,j11,0,1)=0.5*(ptmp(i11-2,j11,0,1)+
					ptmp(i0+2,j11,0,1));
                  ptmp(i11-1,j11,0,1)=ptmp(i11,j11,0,1);
                }
	        if (axis==0){
		  ptmp(i11,j11,0,0)=0.5*(ptmp(i11,j11-2,0,0)+
					 ptmp(i11,j0+2,0,0));
                  ptmp(i11,j11-1,0,0)=ptmp(i11,j11,0,0);
	          ptmp(i11,j11,0,1)=0.5*(ptmp(i11,j11-2,0,1)+
					ptmp(i11,j0+2,0,1));
                  ptmp(i11,j11-1,0,1)=ptmp(i11,j11,0,1);
                }
	      }
          }

	  if (axis==1){
	    int j00=I222.getBase();
	    for (j=j0; j<=j1; j+=j2)
	      ptmp(I1[i],j,I3[i],Rtmp)=ptmp(I1[i],j00,I3[i],Rtmp);
          }
	  else if (axis==0){
	    int i00=I111.getBase();
	    for (j=i0; j<=i1; j+=i2)
	      ptmp(j,I2[i],I3[i],Rtmp)=ptmp(i00,I2[i],I3[i],Rtmp);
	  }

	  //if (iter==300) TEST_RESIDUAL=1;
	  ptmp0[i](I1[i],I2[i],I3[i],Rtmp) =
	      (1-omega1)*ptmp0[i](I1[i],I2[i],I3[i],Rtmp)+
	          omega1*ptmp(I1[i],I2[i],I3[i],Rtmp);
	 }

	 for (j=0;j<ndimension;j++)
	  Source[i](I1[i],I2[i],I3[i],j) += ptmp0[i](I1[i],I2[i],I3[i],j)*
		    CoeffInterp[2*axis+side][i](I1[i],I2[i],I3[i]);
         
	 /********
	 ptemp1=0., qtemp1=0., ipmax=-1, jpmax=-1, iqmax=-1, jqmax=-1;
         for (j=I2[i].getBase();j<=I2[i].getBound();j++)
	  for (i11=I1[i].getBase();i11<=I1[i].getBound();i11++){
	   if (fabs(Source[i](i11,j,0,0))>ptemp1){
	     ptemp1=fabs(Source[i](i11,j,0,0));
	     ipmax=i11, jpmax=j;
           }
	   if (fabs(Source[i](i11,j,0,1))>qtemp1){
	     qtemp1=fabs(Source[i](i11,j,0,1));
	     iqmax=i11, jqmax=j;
           }
         }
         //printf("i=%i\t Pmax=%g\t at i=%i\t j=%i\n",i,ptemp1,ipmax,jpmax);
	 //printf("\t Xc=%g\t Yc=%g\t Xe=%g\t Ye=%g\n",Xc(ipmax,0),
	 //Xc(ipmax,1), Xe(ipmax,0), Xe(ipmax,1));
	 //printf("\t Xcc=%g\t Ycc=%g\t Xee=%g\t Yee=%g\n",Xcc(ipmax,0),
	 //Xcc(ipmax,1), Xee(ipmax,0), Xee(ipmax,1));
         //printf("i=%i\t Qmax=%g\t at i=%i\t j=%i\n",i,qtemp1,iqmax,jqmax);
        //if (TEST_RESIDUAL==1) printf("Changed Source\n");
	//printf("Source: %i::x=%g  y=%g\t %i::x=%g  y=%g\t %i::x=%g  y=%g\n",i0+2,Source[i](i0+2,0,0,0),Source[i](i0+2,0,0,1),i0+1,Source[i](i0+1,0,0,0),Source[i](i0+1,0,0,1),i0,Source[i](i0,0,0,0),Source[i](i0,0,0,1));
	//printf("Source: %i::x=%g  y=%g\t %i::x=%g  y=%g\t %i::x=%g  y=%g\n",i1-2,Source[i](i1-2,0,0,0),Source[i](i1-2,0,0,1),i1-1,Source[i](i1-1,0,0,0),Source[i](i1-1,0,0,1),i1,Source[i](i1,0,0,0),Source[i](i1,0,0,1));
	 ********/
	}
	}
      }
    }
  //Source[0](I1[0],I2[0],I3[0],0).display("Source0");
  //Source[0](I1[0],I2[0],I3[0],1).display("Source1");
  //for (int j11=0;j11<=I2[i].getBound();j11++)
   //for (i11=0;i11<=I1[i].getBound();i11++)
    //printf("i=%i\t j=%i\t P=%g\t Q=%g\n",i11,j11,Source[i](i11,j11,0,0),
    //Source[i](i11,j11,0,1));
  }

void multigrid2::
 Initialize(realMappedGridFunction *u1, const realArray &u0, 
	    realArray *up, Index Igrid, Index Jgrid, 
	    Index Kgrid, Index Rr){
  int i;

  u1[0](Igrid,Jgrid,Kgrid,Rr)=u0(Igrid,Jgrid,Kgrid,Rr);
   if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic){
    u1[0](-1,I2[0],I3[0],Rr)=u1[0](I1[0].getBound()-1,I2[0],I3[0],Rr);
    u1[0](I1[0].getBound()+1,I2[0],I3[0],Rr)=u1[0](1,I2[0],I3[0],Rr);
   }
   else{
    u1[0](-1,I2[0],I3[0],Rr)=2.*u1[0](0,I2[0],I3[0],Rr)-
				u1[0](1,I2[0],I3[0],Rr);
    u1[0](I1[0].getBound()+1,I2[0],I3[0],Rr)=
			     2.*u1[0](I1[0].getBound(),I2[0],I3[0],Rr)-
			     u1[0](I1[0].getBound()-1,I2[0],I3[0],Rr);
   }
   if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic){
    u1[0](I1[0],-1,I3[0],Rr)=u1[0](I1[0],I2[0].getBound()-1,I3[0],Rr);
    u1[0](I1[0],I2[0].getBound()+1,I3[0],Rr)=u1[0](I1[0],1,I3[0],Rr);
   }
   else{
    u1[0](I1[0],-1,I3[0],Rr)=2.*u1[0](I1[0],0,I3[0],Rr)-
				u1[0](I1[0],1,I3[0],Rr);
    u1[0](I1[0],I2[0].getBound()+1,I3[0],Rr)=
			     2.*u1[0](I1[0],I2[0].getBound(),I3[0],Rr)-
			     u1[0](I1[0],I2[0].getBound()-1,I3[0],Rr);
   }
   //up[0]=u1[0];

  for (i=1;i<nlevel;i++){
    Interpolate(i-1,i,u1[i-1],u1[i],2);
    Interpolate(i-1,i,uprev[i-1],uprev[i],2);
  }
  for (i=1;i<nlevel;i++){
   if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic){
    u1[i](-1,I2[i],I3[i],Rr)=u1[i](I1[i].getBound()-1,I2[i],I3[i],Rr);
    u1[i](I1[i].getBound()+1,I2[i],I3[i],Rr)=u1[i](1,I2[i],I3[i],Rr);
    up[i](-1,I2[i],I3[i],Rr)=up[i](I1[i].getBound()-1,I2[i],I3[i],Rr);
    up[i](I1[i].getBound()+1,I2[i],I3[i],Rr)=up[i](1,I2[i],I3[i],Rr);
   }
   else{
    u1[i](-1,I2[i],I3[i],Rr)=2.*u1[i](0,I2[i],I3[i],Rr)-
				u1[i](1,I2[i],I3[i],Rr);
    u1[i](I1[i].getBound()+1,I2[i],I3[i],Rr)=
			     2.*u1[i](I1[i].getBound(),I2[i],I3[i],Rr)-
			     u1[i](I1[i].getBound()-1,I2[i],I3[i],Rr);
    up[i](-1,I2[i],I3[i],Rr)=2.*up[i](0,I2[i],I3[i],Rr)-
				up[i](1,I2[i],I3[i],Rr);
    up[i](I1[i].getBound()+1,I2[i],I3[i],Rr)=
			     2.*up[i](I1[i].getBound(),I2[i],I3[i],Rr)-
			     up[i](I1[i].getBound()-1,I2[i],I3[i],Rr);
   }
   if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic){
    u1[i](I1[i],-1,I3[i],Rr)=u1[i](I1[i],I2[i].getBound()-1,I3[i],Rr);
    u1[i](I1[i],I2[i].getBound()+1,I3[i],Rr)=u1[i](I1[i],1,I3[i],Rr);
    up[i](I1[i],-1,I3[i],Rr)=up[i](I1[i],I2[i].getBound()-1,I3[i],Rr);
    up[i](I1[i],I2[i].getBound()+1,I3[i],Rr)=up[i](I1[i],1,I3[i],Rr);
   }
   else{
    u1[i](I1[i],-1,I3[i],Rr)=2.*u1[i](I1[i],0,I3[i],Rr)-
				u1[i](I1[i],1,I3[i],Rr);
    u1[i](I1[i],I2[i].getBound()+1,I3[i],Rr)=
			     2.*u1[i](I1[i],I2[i].getBound(),I3[i],Rr)-
			     u1[i](I1[i],I2[i].getBound()-1,I3[i],Rr);
    up[i](I1[i],-1,I3[i],Rr)=2.*up[i](I1[i],0,I3[i],Rr)-
				up[i](I1[i],1,I3[i],Rr);
    up[i](I1[i],I2[i].getBound()+1,I3[i],Rr)=
			     2.*up[i](I1[i],I2[i].getBound(),I3[i],Rr)-
			     up[i](I1[i],I2[i].getBound()-1,I3[i],Rr);
   }
  }
}

 void multigrid2::
   updateRHS(int i, realArray *up){
   realArray coeff1,coeff2,coeff3,coeff4,coeff5;
   realArray Xc_e, Yc_e;
   int j, istart,istop,jstart,jstop;

   switch (ndimension){
     case 1:
       RHS[i](Iint1[i],Iint2[i],Iint3[i],0) +=
	(u[i](Iint1[i]+1,Iint2[i],Iint3[i],0) - 2.0*
         u[i](Iint1[i],Iint2[i],Iint3[i],0)+
         u[i](Iint1[i]-1,Iint2[i],Iint3[i],0))/(dx[i]*dx[i])+
	(u[i](Iint1[i]+1,Iint2[i],Iint3[i],0)-
	 u[i](Iint1[i]-1,Iint2[i],Iint3[i],0))*
	 Source[i](Iint1[i],Iint2[i],Iint3[i],0)/(2.0*dx[i]);
       break;

     case 2:
       coeff1.redim(I1[i],I2[i],I3[i]);
       coeff2.redim(I1[i],I2[i],I3[i]);
       coeff3.redim(I1[i],I2[i],I3[i]);
       getSource(i,0);
       find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
		          I1[i],I2[i],I3[i],i,j);
       if (useBlockTridiag != 1){
         for (j=0;j<ndimension;j++){
           RHS[i](Iint1[i],Iint2[i],Iint3[i],j) += 
	   coeff1(Iint1[i],Iint2[i],Iint3[i])*
	   ((u[i](Iint1[i]+1,Iint2[i],Iint3[i],j) - 2.0*
	     u[i](Iint1[i],Iint2[i],Iint3[i],j)+
	     u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
            Source[i](Iint1[i],Iint2[i],Iint3[i],0)*
	    (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j) -
	     u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/(2.0*dx[i]))+
           coeff2(Iint1[i],Iint2[i],Iint3[i])*
	   ((u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-2.0*
	     u[i](Iint1[i],Iint2[i],Iint3[i],j)+
	     u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
	    Source[i](Iint1[i],Iint2[i],Iint3[i],1)*
	    (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-
	     u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/(2.0*dy[i]))-
	   coeff3(Iint1[i],Iint2[i],Iint3[i])*
	    (u[i](Iint1[i]+1,Iint2[i]+1,Iint3[i],j)-
	     u[i](Iint1[i]+1,Iint2[i]-1,Iint3[i],j)-
	     u[i](Iint1[i]-1,Iint2[i]+1,Iint3[i],j)+
	     u[i](Iint1[i]-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]);
           if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic){
	    istart=I1[i].getBase(), istop=I1[i].getBound();
           RHS[i](istart,Iint2[i],Iint3[i],j) += 
	   coeff1(istart,Iint2[i],Iint3[i])*
	   ((u[i](istart+1,Iint2[i],Iint3[i],j) - 2.0*
	     u[i](istart,Iint2[i],Iint3[i],j)+
	     u[i](istop-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
            Source[i](istart,Iint2[i],Iint3[i],0)*
	    (u[i](istart+1,Iint2[i],Iint3[i],j) -
	     u[i](istop-1,Iint2[i],Iint3[i],j))/(2.0*dx[i]))+
           coeff2(istart,Iint2[i],Iint3[i])*
	   ((u[i](istart,Iint2[i]+1,Iint3[i],j)-2.0*
	     u[i](istart,Iint2[i],Iint3[i],j)+
	     u[i](istart,Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
	    Source[i](istart,Iint2[i],Iint3[i],1)*
	    (u[i](istart,Iint2[i]+1,Iint3[i],j)-
	     u[i](istart,Iint2[i]-1,Iint3[i],j))/(2.0*dy[i]))-
	   coeff3(istart,Iint2[i],Iint3[i])*
	    (u[i](istart+1,Iint2[i]+1,Iint3[i],j)-
	     u[i](istart+1,Iint2[i]-1,Iint3[i],j)-
	     u[i](istop-1,Iint2[i]+1,Iint3[i],j)+
	     u[i](istop-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]);

           RHS[i](istop,Iint2[i],Iint3[i],j)=
	       RHS[i](istart,Iint2[i],Iint3[i],j);
	   }

           if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic){
	    jstart=I2[i].getBase(), jstop=I2[i].getBound();
           RHS[i](Iint1[i],jstart,Iint3[i],j) += 
	   coeff1(Iint1[i],jstart,Iint3[i])*
	   ((u[i](Iint1[i]+1,jstart,Iint3[i],j) - 2.0*
	     u[i](Iint1[i],jstart,Iint3[i],j)+
	     u[i](Iint1[i]-1,jstart,Iint3[i],j))/(dx[i]*dx[i])+
            Source[i](Iint1[i],jstart,Iint3[i],0)*
	    (u[i](Iint1[i]+1,jstart,Iint3[i],j) -
	     u[i](Iint1[i]-1,jstart,Iint3[i],j))/(2.0*dx[i]))+
           coeff2(Iint1[i],jstart,Iint3[i])*
	   ((u[i](Iint1[i],jstart+1,Iint3[i],j)-2.0*
	     u[i](Iint1[i],jstart,Iint3[i],j)+
	     u[i](Iint1[i],jstop-1,Iint3[i],j))/(dy[i]*dy[i])+
	    Source[i](Iint1[i],jstart,Iint3[i],1)*
	    (u[i](Iint1[i],jstart+1,Iint3[i],j)-
	     u[i](Iint1[i],jstop-1,Iint3[i],j))/(2.0*dy[i]))-
	   coeff3(Iint1[i],jstart,Iint3[i])*
	    (u[i](Iint1[i]+1,jstart+1,Iint3[i],j)-
	     u[i](Iint1[i]+1,jstop-1,Iint3[i],j)-
	     u[i](Iint1[i]-1,jstart+1,Iint3[i],j)+
	     u[i](Iint1[i]-1,jstop-1,Iint3[i],j))/(4.0*dx[i]*dy[i]);

           RHS[i](Iint1[i],jstop,Iint3[i],j)=
	       RHS[i](Iint1[i],jstart,Iint3[i],j);
	   }
          }
         }
	 else{
	   //printf("Dans updateRHS\n");
	   findPQ(u[i],coeff1,coeff2,I1[i],I2[i],I3[i],i,1,0);
	   for (j=0;j<ndimension;j++){
	     RHS[i](Iint1[i],Iint2[i],Iint3[i],j) +=
	     coeff1(Iint1[i],Iint2[i],Iint3[i])*
	     (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j) - 2.0*
	       u[i](Iint1[i],Iint2[i],Iint3[i],j)+
	       u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
	      (coeff1(Iint1[i],Iint2[i],Iint3[i])*
	       Source[i](Iint1[i],Iint2[i],Iint3[i],0)/(2.*dx[i])-
	       (P[0][i](Iint1[i],Iint2[i],Iint3[i])+
		P[1][i](Iint1[i],Iint2[i],Iint3[i])+
		P[2][i](Iint1[i],Iint2[i],Iint3[i])+
		P[3][i](Iint1[i],Iint2[i],Iint3[i])))*
              (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j)-
	       u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))+
             coeff2(Iint1[i],Iint2[i],Iint3[i])*
	     (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j) - 2.0*
	       u[i](Iint1[i],Iint2[i],Iint3[i],j)+
	       u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
	      (coeff2(Iint1[i],Iint2[i],Iint3[i])*
	       Source[i](Iint1[i],Iint2[i],Iint3[i],1)/(2.0*dy[i])-
	       (Q[0][i](Iint1[i],Iint2[i],Iint3[i])+
		Q[1][i](Iint1[i],Iint2[i],Iint3[i])+
		Q[2][i](Iint1[i],Iint2[i],Iint3[i])+
		Q[3][i](Iint1[i],Iint2[i],Iint3[i])))*
	      (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j) - 
	       u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))-
              coeff3(Iint1[i],Iint2[i],Iint3[i])*
	      (u[i](Iint1[i]+1,Iint2[i]+1,Iint3[i],j)-
	       u[i](Iint1[i]+1,Iint2[i]-1,Iint3[i],j)-
	       u[i](Iint1[i]-1,Iint2[i]+1,Iint3[i],j)+
	       u[i](Iint1[i]-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]);
	       
           if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic){
	    istart=I1[i].getBase(), istop=I1[i].getBound();
           RHS[i](istart,Iint2[i],Iint3[i],j) += 
	   coeff1(istart,Iint2[i],Iint3[i])*
	   (u[i](istart+1,Iint2[i],Iint3[i],j) - 2.0*
	    u[i](istart,Iint2[i],Iint3[i],j)+
	    u[i](istop-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
            (coeff1(istart,Iint2[i],Iint3[i])*
	     Source[i](istart,Iint2[i],Iint3[i],0)/(2.0*dx[i])-
             (P[0][i](istart,Iint2[i],Iint3[i])+
              P[1][i](istart,Iint2[i],Iint3[i])+
              P[2][i](istart,Iint2[i],Iint3[i])+
              P[3][i](istart,Iint2[i],Iint3[i])))*
	    (u[i](istart+1,Iint2[i],Iint3[i],j) -
	     u[i](istop-1,Iint2[i],Iint3[i],j))+
           coeff2(istart,Iint2[i],Iint3[i])*
	   (u[i](istart,Iint2[i]+1,Iint3[i],j)-2.0*
	    u[i](istart,Iint2[i],Iint3[i],j)+
	    u[i](istart,Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])+
	    (coeff2(istart,Iint2[i],Iint3[i])* 
	     Source[i](istart,Iint2[i],Iint3[i],1)/(2.0*dy[i])-
             (Q[0][i](istart,Iint2[i],Iint3[i])+
              Q[1][i](istart,Iint2[i],Iint3[i])+
              Q[2][i](istart,Iint2[i],Iint3[i])+
              Q[3][i](istart,Iint2[i],Iint3[i])))*
	    (u[i](istart,Iint2[i]+1,Iint3[i],j)-
	     u[i](istart,Iint2[i]-1,Iint3[i],j))-
	   coeff3(istart,Iint2[i],Iint3[i])*
	    (u[i](istart+1,Iint2[i]+1,Iint3[i],j)-
	     u[i](istart+1,Iint2[i]-1,Iint3[i],j)-
	     u[i](istop-1,Iint2[i]+1,Iint3[i],j)+
	     u[i](istop-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]);

           RHS[i](istop,Iint2[i],Iint3[i],j)=
	       RHS[i](istart,Iint2[i],Iint3[i],j);
	   }

           if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic){
	    jstart=I2[i].getBase(), jstop=I2[i].getBound();
           RHS[i](Iint1[i],jstart,Iint3[i],j) += 
	   coeff1(Iint1[i],jstart,Iint3[i])*
	   (u[i](Iint1[i]+1,jstart,Iint3[i],j) - 2.0*
	    u[i](Iint1[i],jstart,Iint3[i],j)+
	    u[i](Iint1[i]-1,jstart,Iint3[i],j))/(dx[i]*dx[i])+
            (coeff1(Iint1[i],jstart,Iint3[i])*
	     Source[i](Iint1[i],jstart,Iint3[i],0)/(2.0*dx[i])-
             (P[0][i](Iint1[i],jstart,Iint3[i])+
              P[1][i](Iint1[i],jstart,Iint3[i])+
              P[2][i](Iint1[i],jstart,Iint3[i])+
              P[3][i](Iint1[i],jstart,Iint3[i])))*
	    (u[i](Iint1[i]+1,jstart,Iint3[i],j) -
	     u[i](Iint1[i]-1,jstart,Iint3[i],j))+
           coeff2(Iint1[i],jstart,Iint3[i])*
	   (u[i](Iint1[i],jstart+1,Iint3[i],j)-2.0*
	    u[i](Iint1[i],jstart,Iint3[i],j)+
	    u[i](Iint1[i],jstop-1,Iint3[i],j))/(dy[i]*dy[i])+
	    (coeff2(Iint1[i],jstart,Iint3[i])*    
	     Source[i](Iint1[i],jstart,Iint3[i],1)/(2.0*dy[i])-
             (Q[0][i](Iint1[i],jstart,Iint3[i])+
              Q[1][i](Iint1[i],jstart,Iint3[i])+
              Q[2][i](Iint1[i],jstart,Iint3[i])+
              Q[3][i](Iint1[i],jstart,Iint3[i])))*
	    (u[i](Iint1[i],jstart+1,Iint3[i],j)-
	     u[i](Iint1[i],jstop-1,Iint3[i],j))-
	   coeff3(Iint1[i],jstart,Iint3[i])*
	    (u[i](Iint1[i]+1,jstart+1,Iint3[i],j)-
	     u[i](Iint1[i]+1,jstop-1,Iint3[i],j)-
	     u[i](Iint1[i]-1,jstart+1,Iint3[i],j)+
	     u[i](Iint1[i]-1,jstop-1,Iint3[i],j))/(4.0*dx[i]*dy[i]);

           RHS[i](Iint1[i],jstop,Iint3[i],j)=
	       RHS[i](Iint1[i],jstart,Iint3[i],j);
	   }
           }
          }
	  /*******TRY
	  // For upwinding
	  RHS[i](Iint1[i],Iint2[i],Iint3[i],j) +=
	    coeff1(Iint1[i],Iint2[i],Iint3[i])*
	     (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j) - 2.0*
	      u[i](Iint1[i],Iint2[i],Iint3[i],j)+
	      u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/(dx[i]*dx[i])+
            coeff2(Iint1[i],Iint2[i],Iint3[i])*
	     (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j)-2.0*
	      u[i](Iint1[i],Iint2[i],Iint3[i],j)+
	      u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/(dy[i]*dy[i])-
            coeff3(Iint1[i],Iint2[i],Iint3[i])*
	     (u[i](Iint1[i]+1,Iint2[i]+1,Iint3[i],j)-
	      u[i](Iint1[i]+1,Iint2[i]-1,Iint3[i],j)-
              u[i](Iint1[i]-1,Iint2[i]+1,Iint3[i],j)+
	      u[i](Iint1[i]-1,Iint2[i]-1,Iint3[i],j))/(4.0*dx[i]*dy[i]);
          where(Source[i](Iint1[i],Iint2[i],Iint3[i],0)>=0){
	    RHS[i](Iint1[i],Iint2[i],Iint3[i],j) += 
	     coeff1(Iint1[i],Iint2[i],Iint3[i])*
	     Source[i](Iint1[i],Iint2[i],Iint3[i],0)*
	     (u[i](Iint1[i]+1,Iint2[i],Iint3[i],j) -
	      u[i](Iint1[i],Iint2[i],Iint3[i],j))/dx[i];
          }
          elsewhere(Source[i](Iint1[i],Iint2[i],Iint3[i],0)<0) {
	    RHS[i](Iint1[i],Iint2[i],Iint3[i],j) += 
		coeff1(Iint1[i],Iint2[i],Iint3[i])*
		Source[i](Iint1[i],Iint2[i],Iint3[i],0)*
		      (u[i](Iint1[i],Iint2[i],Iint3[i],j) -
		       u[i](Iint1[i]-1,Iint2[i],Iint3[i],j))/dx[i];
	  }
          where(Source[i](Iint1[i],Iint2[i],Iint3[i],1)>=0){
	    RHS[i](Iint1[i],Iint2[i],Iint3[i],j) += 
	     coeff2(Iint1[i],Iint2[i],Iint3[i])*
	     Source[i](Iint1[i],Iint2[i],Iint3[i],1)*
	     (u[i](Iint1[i],Iint2[i]+1,Iint3[i],j) -
	      u[i](Iint1[i],Iint2[i],Iint3[i],j))/dy[i];
          }
          elsewhere(Source[i](Iint1[i],Iint2[i],Iint3[i],1)<0) {
	    RHS[i](Iint1[i],Iint2[i],Iint3[i],j) += 
		coeff2(Iint1[i],Iint2[i],Iint3[i])*
		Source[i](Iint1[i],Iint2[i],Iint3[i],1)*
		      (u[i](Iint1[i],Iint2[i],Iint3[i],j) -
		       u[i](Iint1[i],Iint2[i]-1,Iint3[i],j))/dy[i];
	  }
         TRY******/
       
       {
       int axis, side;
       intArray index(2,3);
       Index I11,I22,I33;
       int is[3]={0,0,0}, icdif[3];

       index(0,0)=I1[i].getBase(), index(1,0)=I1[i].getBound();
       index(0,1)=I2[i].getBase(), index(1,1)=I2[i].getBound();
       index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
       for (axis=0; axis<=1; axis++)
	for (side=0; side<=1; side++){
	 is[0]=0, is[1]=0, is[2]=0;
	 if (axis==0) icdif[0]=0, icdif[1]=1;
	 else if (axis==1) icdif[0]=1, icdif[1]=0;
	 is[axis]=1-2*side;
         getBoundaryIndex(index,side,axis,I11,I22,I33);
	 if (I11.getBase() != I11.getBound()) I11=Iint1[i];
	 if (I22.getBase() != I22.getBound()) I22=Iint2[i];
	 if (I33.getBase() != I33.getBound()) I33=Iint3[i];
         if (gridBc(side,axis)==2){
	   Xc_e.redim(I11,I22,I33), Yc_e.redim(I11,I22,I33);
	   Xc_e(I11,I22,I33)=uprev[i](I11+icdif[0],I22+icdif[1],I33,0)-
		        uprev[i](I11-icdif[0],I22-icdif[1],I33,0);
	   Yc_e(I11,I22,I33)=uprev[i](I11+icdif[0],I22+icdif[1],I33,1)-
		        uprev[i](I11-icdif[0],I22-icdif[1],I33,1);
	   if (useBlockTridiag != 1){
	     RHS[i](I11,I22,I33,0) +=
		 Xc_e(I11,I22,I33)*(u[i](I11+is[0],I22+is[1],I33,0)-
			       u[i](I11,I22,I33,0))+
		 Yc_e(I11,I22,I33)*(u[i](I11+is[0],I22+is[1],I33,1)-
			       u[i](I11,I22,I33,1));
             RHS[i](I11,I22,I33,1) +=
		 Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)-
		 Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
           }
	   else{
	     where (fabs(Xc_e(I11,I22,I33))>0.00001){
	       RHS[i](I11,I22,I33,0) +=
		 Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
		             u[i](I11+is[0],I22+is[1],I33,0))+
		 Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
		             u[i](I11+is[0],I22+is[1],I33,1));
               RHS[i](I11,I22,I33,1) +=
		 Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)-
		 Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
	     }
	     elsewhere(){
	       RHS[i](I11,I22,I33,0) +=
		 Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)-
		 Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
               RHS[i](I11,I22,I33,1) +=
		 Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
		             u[i](I11+is[0],I22+is[1],I33,0))+
		 Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
		             u[i](I11+is[0],I22+is[1],I33,1));
	     }
	   }
        }
       }
      }
      break;	
    }
   }

 void multigrid2::
   find2Dcoefficients(realArray &a1, realArray &b1, realArray &c1,
		      realArray &d1, realArray &e1, Index I11, 
		      Index I22, Index I33, int i, int j){
     realArray Xc,Xe,Yc,Ye;
     int j11, istart, istop, jstart, jstop;

     Xc.redim(I11,I22,I33);
     Yc.redim(I11,I22,I33);
     Xe.redim(I11,I22,I33);
     Ye.redim(I11,I22,I33);

     Yc(I11,I22,I33)=(u[i](I11+1,I22,I33,1)-
		      u[i](I11-1,I22,I33,1))/(2*dx[i]);
     Ye(I11,I22,I33)=(u[i](I11,I22+1,I33,1)-
		      u[i](I11,I22-1,I33,1))/(2*dy[i]);
     Xc(I11,I22,I33)=(u[i](I11+1,I22,I33,0)-
		      u[i](I11-1,I22,I33,0))/(2*dx[i]);
     Xe(I11,I22,I33)=(u[i](I11,I22+1,I33,0)-
		      u[i](I11,I22-1,I33,0))/(2*dy[i]);
     /******
     if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic){
       istart=I1[i].getBase(), istop=I1[i].getBound();
       Xc(istart,I22,I33)=(u[i](istart+1,I22,I33,0)-
			   u[i](istop-1,I22,I33,0))/(2.*dx[i]);
       Yc(istart,I22,I33)=(u[i](istart+1,I22,I33,1)-
			   u[i](istop-1,I22,I33,1))/(2.*dx[i]);
       Xc(istop,I22,I33)=Xc(istart,I22,I33);
       Yc(istop,I22,I33)=Yc(istart,I22,I33);
     } 
     if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic){
       jstart=I2[i].getBase(), jstop=I2[i].getBound();
       Xe(I11,jstart,I33)=(u[i](I11,jstart+1,I33,0)-
			   u[i](I11,jstop-1,I33,0))/(2.*dy[i]);
       Ye(I11,jstart,I33)=(u[i](I11,jstart+1,I33,1)-
			   u[i](I11,jstop-1,I33,1))/(2.*dy[i]);
       Xe(I11,jstop,I33)=Xe(I11,jstart,I33);
       Ye(I11,jstop,I33)=Ye(I11,jstart,I33);
     } 
     *******/
     //if ((gridBc(0,0)!=3)&&(gridBc(0,1)!=3)&&(gridBc(1,0)!=3)&&
	 //(gridBc(1,1)!=3)){
       a1(I11,I22,I33)=Xe(I11,I22,I33)*
		       Xe(I11,I22,I33)+
		       Ye(I11,I22,I33)*
		       Ye(I11,I22,I33);
       b1(I11,I22,I33)=Xc(I11,I22,I33)*
		       Xc(I11,I22,I33)+
		       Yc(I11,I22,I33)*
		       Yc(I11,I22,I33);
       c1(I11,I22,I33)=2.0*(Xc(I11,I22,I33)*
			    Xe(I11,I22,I33)+
			    Yc(I11,I22,I33)*
			    Ye(I11,I22,I33));
     /******TRY
     }
     else{
      int i0, i1, i2, j0, j1, j2, k0, k1;
      realArray Xcc(I1[i],I2[i],I3[i]), Xee(I1[i],I2[i],I3[i]);
      realArray Ycc(I1[i],I2[i],I3[i]), Yee(I1[i],I2[i],I3[i]);
      realArray normXc(I1[i],I2[i],I3[i]), normXe(I1[i],I2[i],I3[i]);
      realArray normXc0(I1[i],I2[i],I3[i]), normXe0(I1[i],I2[i],I3[i]); 
      realArray normXc1(I1[i],I2[i],I3[i]), normXe1(I1[i],I2[i],I3[i]); 
      realArray normXc2(I1[i],I2[i],I3[i]), normXe2(I1[i],I2[i],I3[i]); 
      realArray normXc3(I1[i],I2[i],I3[i]), normXe3(I1[i],I2[i],I3[i]); 

      normXe0=1.0, normXc0=1.0;
      normXe1=1.0, normXc1=1.0;
      normXe2=1.0, normXc2=1.0; 
      normXe3=1.0, normXc3=1.0;
      i0=I1[i].getBase(), i1=I1[i].getBound(), i2=I1[i].getStride();
      j0=I2[i].getBase(), j1=I2[i].getBound(), j2=I2[i].getStride();
      Xcc(I11,I22,I33)=(u[i](I11+1,I22,I33,0)-2.0*
			u[i](I11,I22,I33,0)+
			u[i](I11-1,I22,I33,0))/(dx[i]*dx[i]);
      Ycc(I11,I22,I33)=(u[i](I11+1,I22,I33,1)-2.0*
			u[i](I11,I22,I33,1)+
			u[i](I11-1,I22,I33,1))/(dx[i]*dx[i]);
      Xee(I11,I22,I33)=(u[i](I11,I22+1,I33,0)-2.0*
			u[i](I11,I22,I33,0)+
			u[i](I11,I22-1,I33,0))/(dy[i]*dy[i]);
      Yee(I11,I22,I33)=(u[i](I11,I22+1,I33,1)-2.0*
			u[i](I11,I22,I33,1)+
			u[i](I11,I22-1,I33,1))/(dy[i]*dy[i]);
      if (gridBc(0,0)==3){
       normXc0=dB(0,0)*dB(0,0);
       normXe0(i0,I22,I33)=
	     Xe(i0,I22,I33)*Xe(i0,I22,I33)+
	     Ye(i0,I22,I33)*Ye(i0,I22,I33);
       for (j11=i0+1;j11<=i1;j11+=i2)
	normXe0(j11,I22,I33)=normXe0(i0,I22,I33);
      }

      if (gridBc(1,0)==3){
       normXc1=dB(1,0)*dB(1,0);
       normXe1(i1,I22,I33)=
	     Xe(i1,I22,I33)*Xe(i1,I22,I33)+
	     Ye(i1,I22,I33)*Ye(i1,I22,I33);
       for (j11=i1-1;j11>=i0;j11-=i2)
	normXe1(j11,I22,I33)=normXe1(i1,I22,I33);
      }

      if (gridBc(0,1)==3){
       realArray Xc1(I1[i]), Yc1(I1[i]);
       normXe2=dB(0,1)*dB(0,1);
       Xc1(I11)=(u[i](I11+1,j0,I33,0)-u[i](I11-1,j0,I33,0))/(2.*dx[i]);
       Yc1(I11)=(u[i](I11+1,j0,I33,1)-u[i](I11-1,j0,I33,1))/(2.*dx[i]);
       normXc2(I11,j0,I33)=
	     Xc1(I11,j0,I33)*Xc1(I11,j0,I33)+
	     Yc1(I11,j0,I33)*Yc1(I11,j0,I33);
       for (j11=j0+1;j11<=j1;j11+=j2)
	normXc2(I11,j11,I33)=normXc2(I11,j0,I33);
      }

      if (gridBc(1,1)==3){
       normXe3=dB(1,1)*dB(1,1);
       normXc3(I11,j1,I33)=
	     Xc(I11,j1,I33)*Xc(I11,j1,I33)+
	     Yc(I11,j1,I33)*Yc(I11,j1,I33);
       for (j11=j0;j11<j1;j11+=j2)
	normXc3(I11,j11,I33)=normXc3(I11,j1,I33);
      }

      normXc(I11,I22,I33)=Xc*Xc+Yc*Yc, normXe(I11,I22,I33)=Xe*Xe+Ye*Ye;
      if (j==0){
	a1=normXe(I11,I22,I33)-
		  (CoeffInterp0[i](I11,I22,I33)/normXc0(I11,I22,I33)+
		   CoeffInterp1[i](I11,I22,I33)/normXc1(I11,I22,I33)+
		   CoeffInterp2[i](I11,I22,I33)/normXc2(I11,I22,I33)+
		   CoeffInterp3[i](I11,I22,I33)/normXc3(I11,I22,I33))*
		    (normXe(I11,I22,I33)*Xc*Xc+normXc(I11,I22,I33)*Xe*Xe);
        b1=normXc(I11,I22,I33)-
		  (CoeffInterp0[i](I11,I22,I33)/normXe0(I11,I22,I33)+
		   CoeffInterp1[i](I11,I22,I33)/normXe1(I11,I22,I33)+
		   CoeffInterp2[i](I11,I22,I33)/normXe2(I11,I22,I33)+
		   CoeffInterp3[i](I11,I22,I33)/normXe3(I11,I22,I33))*
		    (normXe(I11,I22,I33)*Xc*Xc+normXc(I11,I22,I33)*Xe*Xe);
        c1=(Source[i](I11,I22,I33,0)-
		 (CoeffInterp0[i](I11,I22,I33)/normXc0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXc1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXc2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXc3(I11,I22,I33))*
		  Yc(I11,I22,I33)*Ycc(I11,I22,I33)-
                 (CoeffInterp0[i](I11,I22,I33)/normXe0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXe1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXe2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXe3(I11,I22,I33))*
		  Yc(I11,I22,I33)*Yee(I11,I22,I33))*normXe(I11,I22,I33);
        d1=(Source[i](I11,I22,I33,1)-
		 (CoeffInterp0[i](I11,I22,I33)/normXc0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXc1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXc2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXc3(I11,I22,I33))*
		  Ye(I11,I22,I33)*Ycc(I11,I22,I33)-
                 (CoeffInterp0[i](I11,I22,I33)/normXe0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXe1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXe2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXe3(I11,I22,I33))*
		  Ye(I11,I22,I33)*Yee(I11,I22,I33))*normXc(I11,I22,I33);
        e1=2.0*(Xc*Xe+Yc*Ye);
      }
      else {
        a1=normXe(I11,I22,I33)-
		  (CoeffInterp0[i](I11,I22,I33)/normXc0(I11,I22,I33)+
		   CoeffInterp1[i](I11,I22,I33)/normXc1(I11,I22,I33)+
		   CoeffInterp2[i](I11,I22,I33)/normXc2(I11,I22,I33)+
		   CoeffInterp3[i](I11,I22,I33)/normXc3(I11,I22,I33))*
		    (normXe(I11,I22,I33)*Yc*Yc+normXc(I11,I22,I33)*Ye*Ye);
        b1=normXc(I11,I22,I33)-
		  (CoeffInterp0[i](I11,I22,I33)/normXe0(I11,I22,I33)+
		   CoeffInterp1[i](I11,I22,I33)/normXe1(I11,I22,I33)+
		   CoeffInterp2[i](I11,I22,I33)/normXe2(I11,I22,I33)+
		   CoeffInterp3[i](I11,I22,I33)/normXe3(I11,I22,I33))*
		    (normXe(I11,I22,I33)*Yc*Yc+normXc(I11,I22,I33)*Ye*Ye);
        c1=(Source[i](I11,I22,I33,0)-
		 (CoeffInterp0[i](I11,I22,I33)/normXc0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXc1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXc2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXc3(I11,I22,I33))*
		  Xc(I11,I22,I33)*Xcc(I11,I22,I33)-
                 (CoeffInterp0[i](I11,I22,I33)/normXe0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXe1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXe2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXe3(I11,I22,I33))*
		  Xc(I11,I22,I33)*Xee(I11,I22,I33))*normXe(I11,I22,I33);
        d1=(Source[i](I11,I22,I33,1)-
		 (CoeffInterp0[i](I11,I22,I33)/normXc0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXc1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXc2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXc3(I11,I22,I33))*
		  Xe(I11,I22,I33)*Xcc(I11,I22,I33)-
                 (CoeffInterp0[i](I11,I22,I33)/normXe0(I11,I22,I33)+
		  CoeffInterp1[i](I11,I22,I33)/normXe1(I11,I22,I33)+
		  CoeffInterp2[i](I11,I22,I33)/normXe2(I11,I22,I33)+
		  CoeffInterp3[i](I11,I22,I33)/normXe3(I11,I22,I33))*
		  Xe(I11,I22,I33)*Xee(I11,I22,I33))*normXc(I11,I22,I33);
        e1=2.0*(Xc*Xe+Yc*Ye);
      }
    }    
    TRY******/
   }

 void multigrid2::
   getAlpha(void){
   int i,j;
   realArray a1(I1[0],I2[0],I3[0]), b1(I1[0],I2[0],I3[0]);
   realArray c1(I1[0],I2[0],I3[0]), d1, e1;
   real a00, b00, c00=1.0;

   a1=0.0, b1=0.0, c1=0.0;
   getSource(0,0);
   find2Dcoefficients(a1,b1,c1,d1,e1,Iint1[0],Iint2[0],Iint3[0],0,0);
   for (int j2=1;j2<I2[0].getBound();j2++){
    real maxdiff, maxquotient;
    int imax, jmax;
    maxdiff=0., maxquotient=1.0, imax=1, jmax=1;
    for (int i2 = 1; i2<I1[0].getBound(); i2++){
     a00=0.2*(dx[0]*dx[0]*b1(i2,j2,0)+dy[0]*dy[0]*a1(i2,j2,0))-
	 0.5*dx[0]*dy[0]*fabs(c1(i2,j2,0));
     b00=fabs(0.5*dx[0]*dy[0]*dy[0]*a1(i2,j2,0)*Source[0](i2,j2,0,0))+
	 fabs(0.5*dx[0]*dx[0]*dy[0]*b1(i2,j2,0)*Source[0](i2,j2,0,1));
     if ((a00/b00)<c00) c00=a00/b00;
    }
   }
   alpha=c00;
  printf("alpha= %g\t",alpha);
 }

 void multigrid2::
   applyOrthogonalBoundaries(Index I11, Index I22, Index I33, int j, 
		       int i, realArray &u1, realArray *up){
   //Applies the orthogonal boundary conditions when the
   //simultaneous solver is not used
     real omega2=1.0;
     int istart,istop,jstart,jstop,isharp=0, i1, j1;
     realArray Xc_e,Yc_e,normXc_e,X2,Y2,XY,Xe,Ye,normXe;
     Index I111, I222, I333;
     intArray index(2,3);
     int is[3]={0,0,0}, icdif[3]={0,0,0};
     int axis, side,i0;

     jstart=I2[i].getBase();
     jstop=I2[i].getBound();
     istart=I1[i].getBase();
     istop=I1[i].getBound();
     index(0,0)=I1[i].getBase(), index(1,0)=I1[i].getBound();
     index(0,1)=I2[i].getBase(), index(1,1)=I2[i].getBound();
     index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
     for (axis=0;axis<=1;axis++)
      for (side=0;side<=1;side++){
	is[0]=0, is[1]=0, is[2]=0;
	if (axis==0) icdif[0]=0, icdif[1]=1, icdif[2]=0;
	else if (axis==1) icdif[0]=1,icdif[1]=0, icdif[2]=0;
	is[axis]=1-2*side;
	i0=2*axis+side;
	getBoundaryIndex(index,side,axis,I111,I222,I333);
	if (I111.getBase() != I111.getBound()) I111=I11;
	if (I222.getBase() != I222.getBound()) I222=I22;
        if (gridBc(side,axis)==2){
         Xc_e.redim(I111,I222,I333), X2.redim(I111,I222,I333);
         Yc_e.redim(I111,I222,I333), Y2.redim(I111,I222,I333);
         normXc_e.redim(I111,I222,I333), XY.redim(I111,I222,I333);
         Xc_e(I111,I222,I333)=uprev[i](I111+icdif[0],I222+icdif[1],I33,0)-
	                      uprev[i](I111-icdif[0],I222-icdif[1],I33,0);
         Yc_e(I111,I222,I333)=uprev[i](I111+icdif[0],I222+icdif[1],I33,1)-
	                      uprev[i](I111-icdif[0],I222-icdif[1],I33,1);
         X2(I111,I222,I333)=Xc_e(I111,I222,I333)*Xc_e(I111,I222,I333);
         Y2(I111,I222,I333)=Yc_e(I111,I222,I333)*Yc_e(I111,I222,I333);
         XY(I111,I222,I333)=Xc_e(I111,I222,I333)*Yc_e(I111,I222,I333);
         normXc_e(I111,I222,I333)=X2(I111,I222,I333)+Y2(I111,I222,I333);
         if (i==0){
           u1(I111,I222,I333,0)=
	      (1.-omega2)*u1(I111,I222,I333,0)+
	      omega2*(X2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
                      XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1)-
                      XY(I111,I222,I333)*
			    uprev[i](I111-icdif[0],I222-icdif[1],I33,1)+
	              Y2(I111,I222,I333)*
			    uprev[i](I111-icdif[0],I222-icdif[1],I33,0))/
						     normXc_e(I111,I222,I333);
           u1(I111,I222,I333,1)=
	      (1-omega2)*u1(I111,I222,I333,1)+
	      omega2*(X2(I111,I222,I333)*
			    uprev[i](I111-icdif[0],I222-icdif[1],I33,1)-
	              XY(I111,I222,I333)*
			    uprev[i](I111-icdif[0],I222-icdif[1],I33,0)+
	              XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
	              Y2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1))/
						    normXc_e(I111,I222,I333);
         }
         else {
          u1(I111,I222,I333,0)=
	     (1.-omega2)*u1(I111,I222,I333,0)+
	     omega2*(X2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
		     XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1)-
		     Xc_e(I111,I222,I333)*RHS[i](I111,I222,I333,0)-
		     Yc_e(I111,I222,I333)*RHS[i](I111,I222,I333,1))/normXc_e(I111,I222,I333);
          u1(I111,I222,I333,1)=
	     (1-omega2)*u1(I111,I222,I333,1)+
	     omega2*(Xc_e(I111,I222,I333)*RHS[i](I111,I222,I333,1)+
	             XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
	             Y2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1)-
		     Yc_e(I111,I222,I333)*RHS[i](I111,I222,I333,0))/normXc_e(I111,I222,I333);
         }
         jstart=I2[i].getBase();
         jstop=I2[i].getBound();
         istart=I1[i].getBase();
         istop=I1[i].getBound();
         u1(istart,jstart,I33,Range(0,1))=uprev[i](istart,jstart,I33,Range(0,1));
         u1(istop,jstart,I33,Range(0,1))=uprev[i](istop,jstart,I33,Range(0,1));
         u1(istart,jstop,I33,Range(0,1))=uprev[i](istart,jstop,I33,Range(0,1));
         u1(istop,jstop,I33,Range(0,1))=uprev[i](istop,jstop,I33,Range(0,1));
        
	 // The remainder deals with sharp corners on the boundary
	 if (I111.getBase() != I111.getBound()) I111=I1[i];
	 if (I222.getBase() != I222.getBound()) I222=I2[i];
         istart=I111.getBase(), istop=I111.getBound();
         jstart=I222.getBase(), jstop=I222.getBound();
         if (i==0){
           if (smoothingMethod<3){
	     isharp=0;
           for (j1=jstart;j1<=jstop;j1++)
	    for (i1=istart; i1<=istop; i1++){
             if (((utmp[i](i1+icdif[0],j1+icdif[1],0,0)-utmp[i](i1,j1,0,0))*
                  (utmp[i](i1-icdif[0],j1-icdif[1],0,0)-utmp[i](i1,j1,0,0))+
                  (utmp[i](i1+icdif[0],j1+icdif[1],0,1)-utmp[i](i1,j1,0,1))*
                  (utmp[i](i1-icdif[0],j1-icdif[1],0,1)-utmp[i](i1,j1,0,1)))>0.000){
               isharp=1;
               break;
             }
            }
           if (isharp==1){
             where(((utmp[i](I111+icdif[0],I222+icdif[1],I333,0)-
		      u1(I111,I222,I333,0))*
                    (utmp[i](I111-icdif[0],I222-icdif[1],I333,0)-
		      u1(I111,I222,I333,0))+
                    (utmp[i](I111+icdif[0],I222+icdif[1],I333,1)-
		      u1(I111,I222,I333,1))*
                    (utmp[i](I111-icdif[0],I222-icdif[1],I333,1)-
		      u1(I111,I222,I333,1)))>10.*REAL_EPSILON){
              u1(I111,I222,I333,0)=utmp[i](I111,I222,I333,0);
              u1(I111,I222,I333,1)=utmp[i](I111,I222,I333,1);
	      GRIDBC[i0][i](I111,I222,I333)=1;
             }

	     where(((uprev[i](I111+icdif[0],I222+icdif[1],I333,0)-
		     uprev[i](I111,I222,I333,0))*
                    (uprev[i](I111-icdif[0],I222-icdif[1],I333,0)-uprev[i](I111,I222,I333,0))>10.*REAL_EPSILON)||
                   ((uprev[i](I111+icdif[0],I222+icdif[1],I333,1)-uprev[i](I111,I222,I333,1))*
                    (uprev[i](I111-icdif[0],I222-icdif[1],I333,1)-uprev[i](I111,I222,I333,1))>10.*REAL_EPSILON)){
	      GRIDBC[i0][i](I111,I222,I333)=10;
              u1(I111,I222,I333,0)=uprev[i](I111,I222,I333,0);
              u1(I111,I222,I333,1)=uprev[i](I111,I222,I333,1);
	      GRIDBC[i0][i](I111+icdif[0],I222+icdif[1],I333)=10;
              u1(I111+icdif[0],I222+icdif[1],I333,0)=uprev[i](I111+icdif[0],I222+icdif[1],I333,0);
              u1(I111+icdif[0],I222+icdif[1],I333,1)=uprev[i](I111+icdif[0],I222+icdif[1],I333,1);
	      GRIDBC[i0][i](I111-icdif[0],I222-icdif[1],I33)=10;
              u1(I111-icdif[0],I222-icdif[1],I333,0)=uprev[i](I111-icdif[0],I222-icdif[1],I333,0);
              u1(I111-icdif[0],I222-icdif[1],I333,1)=uprev[i](I111-icdif[0],I222-icdif[1],I333,1);
             }
           }  
         }
         else {
          where(GRIDBC[i0][i](I111,I222,I333)==1){
	     u1(I111,I222,I333,0)=utmp[i](I111,I222,I333,0);
	     u1(I111,I222,I333,1)=utmp[i](I111,I222,I333,1);
	   }
          where (GRIDBC[i0][i](I111,I222,I333)==10){         
	    u1(I111,I222,I333,0)=uprev[i](I111,I222,I333,0);
	    u1(I111,I222,I333,1)=uprev[i](I111,I222,I333,1);
	   } 
          }
         }
         else {
	   for (j1=I222.getBase();j1<=I222.getBound();j1++)
	    for (i1=I111.getBase();i1<=I111.getBound();i1++)
	      GRIDBC[i0][i](i1,j1,0)=GRIDBC[i0][i-1](2*i1,2*j1,0);

           if ((axis==1)&&(side==0)){
	      GRIDBC[i0][i](-1,jstart,0)=
		     GRIDBC[i0][i-1](-1,2*jstart,0);
              GRIDBC[i0][i](istop+1,jstart,0)=
		     GRIDBC[i0][i-1](2*istop+1,2*jstart,0);
           }
           if ((side==0)&&(axis==0)){
	      GRIDBC[i0][i](istart,-1,0)=
		     GRIDBC[i0][i-1](2*istart,-1,0);
              GRIDBC[i0][i](istart,jstop+1,0)=
		     GRIDBC[i0][i-1](2*istart,2*jstop+1,0);
           }
           if ((side==1)&&(axis==1)){
	      GRIDBC[i0][i](-1,jstop,0)=
		     GRIDBC[i0][i-1](-1,2*jstop,0);
              GRIDBC[i0][i](istop+1,jstop,0)=
		     GRIDBC[i0][i-1](2*istop+1,2*jstop,0);
           }
           if ((side==1)&&(axis==0)){
	      GRIDBC[i0][i](istop,-1,0)=
		     GRIDBC[i0][i-1](2*istop,-1,0);
              GRIDBC[i0][i](istop,jstop+1,0)=
		     GRIDBC[i0][i-1](2*istop,2*jstop+1,0);
           }

	   where(GRIDBC[i0][i](I111,I222,I333)==1){
	    u1(I111,I222,I333,0)=utmp[i](I111,I222,I333,0);
	    u1(I111,I222,I333,1)=utmp[i](I111,I222,I333,1);
	   }
	   elsewhere(GRIDBC[i0][i](I111,I222,I333)==10){
	    u1(I111,I222,I333,0)=uprev[i](I111,I222,I333,0);
	    u1(I111,I222,I333,1)=uprev[i](I111,I222,I333,1);
	   }
         }
        }
     }
   }

 void multigrid2::
   jacobi2Dsolve(const realArray &a1, const realArray &b1, 
                 const realArray &c1, const realArray &d1,
		 const realArray &e1, Index I11, Index I22, 
		 Index I33, int j, int i, realArray &u1, 
		 realArray *up, int periodicCorrection){ 
   int istart, jstart, istop, jstop;
   real omega2=1.0;

    //Apply first the Boundary condition
    applyOrthogonalBoundaries(I11,I22,I33,j,i,u1,uprev);
    if (TEST_RESIDUAL){
     printf("Dans solve apres BC i=%i\t j=%i\n",i,j);
     int imax=0,jmax=0,i1max=0, j1max=0;
     real resmaxx=0.0, resmaxy=0.0;
     realArray restemp1;
     restemp1.redim(I1[i],I2[i],I3[i],ndimension);
     restemp1=0.0;
     getResidual(restemp1,i,uprev);
    //(u1(I1[i],Range(0,2),I3[i],Range(0,1))).display("u after BC");
    //Source[i](I1[i],Range(0,2),I3[i],Range(0,1)).display("SOURCE");
    //restemp1(I1[i],Range(0,2),I3[i],Range(0,1)).display("restemp1 before source");
     for (int j1=0; j1<=I2[i].getBound(); j1++)
       for (int i1=0; i1<=I1[i].getBound(); i1++){
	 if (fabs(restemp1(i1,j1,0,0))>resmaxx)
	   imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
         if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	    i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
        }
      printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
		    imax,jmax,resmaxx,i1max,j1max,resmaxy);
}
       // Regular method
     if ((smoothingMethod==1)||((smoothingMethod==2)&&
	 (periodicCorrection != 1)))
       u1(I11,I22,I33,j)=
	 (1.0-omega)*u1(I11,I22,I33,j)+
	 omega*(dy[i]*dy[i]*a1(I11,I22,I33)*
           (u1(I11+1,I22,I33,j)+
	    u1(I11-1,I22,I33,j))+
                dx[i]*dx[i]*b1(I11,I22,I33)*
	   (u1(I11,I22+1,I33,j)+
	    u1(I11,I22-1,I33,j))+
                dy[i]*dy[i]*dx[i]*a1(I11,I22,I33)*
			Source[i](I11,I22,I33,0)*
	   (u1(I11+1,I22,I33,j)-
	    u1(I11-1,I22,I33,j))/2.0+
                dx[i]*dx[i]*dy[i]*b1(I11,I22,I33)*
			Source[i](I11,I22,I33,1)*
	   (u1(I11,I22+1,I33,j)-
	    u1(I11,I22-1,I33,j))/2.0-
                dx[i]*dy[i]*c1(I11,I22,I33)*
	   (u1(I11+1,I22+1,I33,j)-
	    u1(I11-1,I22+1,I33,j)-
	    u1(I11+1,I22-1,I33,j)+
	    u1(I11-1,I22-1,I33,j))/4.0-
                dx[i]*dx[i]*dy[i]*dy[i]*
		       RHS[i](I11,I22,I33,j))/
		(2.0*a1(I11,I22,I33)*dy[i]*dy[i]+
		 2.0*b1(I11,I22,I33)*dx[i]*dx[i]);
     if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic){
       istart=I1[i].getBase(), istop=I1[i].getBound();
       if ((smoothingMethod==1)||((smoothingMethod==2)&&
	   (periodicCorrection==1))){
         u1(istart,I22,I33,j)=
	   (1.0-omega)*u1(istart,I22,I33,j)+
	   omega*(dy[i]*dy[i]*a1(istart,I22,I33)*
             (u1(istart+1,I22,I33,j)+
	      u1(istop-1,I22,I33,j))+
                  dx[i]*dx[i]*b1(istart,I22,I33)*
	     (u1(istart,I22+1,I33,j)+
	      u1(istart,I22-1,I33,j))+
                  dy[i]*dy[i]*dx[i]*a1(istart,I22,I33)*
			  Source[i](istart,I22,I33,0)*
	     (u1(istart+1,I22,I33,j)-
	      u1(istop-1,I22,I33,j))/2.0+
                  dx[i]*dx[i]*dy[i]*b1(istart,I22,I33)*
			  Source[i](istart,I22,I33,1)*
	     (u1(istart,I22+1,I33,j)-
	      u1(istart,I22-1,I33,j))/2.0-
                  dx[i]*dy[i]*c1(istart,I22,I33)*
	     (u1(istart+1,I22+1,I33,j)-
	      u1(istop-1,I22+1,I33,j)-
	      u1(istart+1,I22-1,I33,j)+
	      u1(istop-1,I22-1,I33,j))/4.0-
                  dx[i]*dx[i]*dy[i]*dy[i]*
		         RHS[i](istart,I22,I33,j))/
		  (2.0*a1(istart,I22,I33)*dy[i]*dy[i]+
		   2.0*b1(istart,I22,I33)*dx[i]*dx[i]);

         u1(istop,I22,I33,j)=u1(istart,I22,I33,j);
	 //ghostpoints
	 u1(istart-1,I22,I33,j)=u1(istop-1,I22,I33,j);
	 u1(istop+1,I22,I33,j)=u1(istart+1,I22,I33,j);
       }
     }
     if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic){
       jstart=I2[i].getBase(), jstop=I2[i].getBound();
       if ((smoothingMethod==1)||((smoothingMethod==2)&&
	   (periodicCorrection==1))){
         u1(I11,jstart,I33,j)=
	   (1.0-omega)*u1(I11,jstart,I33,j)+
	   omega*(dy[i]*dy[i]*a1(I11,jstart,I33)*
             (u1(I11+1,jstart,I33,j)+
	      u1(I11-1,jstart,I33,j))+
                  dx[i]*dx[i]*b1(I11,jstart,I33)*
	     (u1(I11,jstart+1,I33,j)+
	      u1(I11,jstop-1,I33,j))+
                  dy[i]*dy[i]*dx[i]*a1(I11,jstart,I33)*
			  Source[i](I11,jstart,I33,0)*
	     (u1(I11+1,jstart,I33,j)-
	      u1(I11-1,jstart,I33,j))/2.0+
                  dx[i]*dx[i]*dy[i]*b1(I11,jstart,I33)*
			  Source[i](I11,jstart,I33,1)*
	     (u1(I11,jstart+1,I33,j)-
	      u1(I11,jstop-1,I33,j))/2.0-
                  dx[i]*dy[i]*c1(I11,jstart,I33)*
	     (u1(I11+1,jstart+1,I33,j)-
	      u1(I11-1,jstart+1,I33,j)-
	      u1(I11+1,jstop-1,I33,j)+
	      u1(I11-1,jstop-1,I33,j))/4.0-
                  dx[i]*dx[i]*dy[i]*dy[i]*
		         RHS[i](I11,jstart,I33,j))/
		  (2.0*a1(I11,jstart,I33)*dy[i]*dy[i]+
		   2.0*b1(I11,jstart,I33)*dx[i]*dx[i]);

         u1(I11,jstop,I33,j)=u1(I11,jstart,I33,j);
	 //ghostpoints
	 u1(I11,jstart-1,I33,j)=u1(I11,jstop-1,I33,j);
	 u1(I11,jstop+1,I33,j)=u1(I11,jstart+1,I33,j);
       }
     }
    /*****TRY
    else{
     // putting x_kk x_ee into solution
     u1(I11,I22,I33,j)=
	 (1.0-omega)*u1(I11,I22,I33,j)+
	 omega*(dy[i]*dy[i]*a1(I11,I22,I33)*
	         (u1(I11+1,I22,I33,j)+u1(I11-1,I22,I33,j))+
                dx[i]*dx[i]*b1(I11,I22,I33)*
		 (u1(I11,I22+1,I33,j)+u1(I11,I22-1,I33,j))+
                dy[i]*dy[i]*dx[i]*c1(I11,I22,I33)*
		 (u1(I11+1,I22,I33,j)-u1(I11-1,I22,I33,j))/2.0+
                dx[i]*dx[i]*dy[i]*d1(I11,I22,I33)*
		 (u1(I11,I22+1,I33,j)-u1(I11,I22-1,I33,j))/2.0-
                dx[i]*dy[i]*e1(I11,I22,I33)*
		 (u1(I11+1,I22+1,I33,j)-u1(I11-1,I22+1,I33,j)-
		  u1(I11+1,I22-1,I33,j)+u1(I11-1,I22-1,I33,j))/4.0-
                dx[i]*dx[i]*dy[i]*dy[i]*RHS[i](I11,I22,I33,j))/
		(2.0*a1(I11,I22,I33)*dy[i]*dy[i]+
		 2.0*b1(I11,I22,I33)*dx[i]*dx[i]);
   // Using upwinding
   realArray Mat1(I11,I22,I33), Mat2(I11,I22,I33);
   realArray Mat3(I11,I22,I33), Den(I11,I22,I33);

   Mat1(I11,I22,I33)=dy[i]*dy[i]*a1(I11,I22,I33)*
		     (u1(I11+1,I22,I33,j)+u1(I11-1,I22,I33,j));
   Mat2(I11,I22,I33)=dx[i]*dx[i]*b1(I11,I22,I33)*
		     (u1(I11,I22+1,I33,j)+u1(I11,I22-1,I33,j));
   Mat3(I11,I22,I33)=-dx[i]*dy[i]*c1(I11,I22,I33)*
           (u1(I11+1,I22+1,I33,j)-u1(I11-1,I22+1,I33,j)-
	    u1(I11+1,I22-1,I33,j)+u1(I11-1,I22-1,I33,j))/4.0-
	    dx[i]*dx[i]*dy[i]*dy[i]*RHS[i](I11,I22,I33,j);
   Den(I11,I22,I33)=2.0*a1(I11,I22,I33)*dy[i]*dy[i]+
		    2.0*b1(I11,I22,I33)*dx[i]*dx[i];
   where(Source[i](I11,I22,I33,0)>= 0.){
     Mat1(I11,I22,I33) += dy[i]*dy[i]*dx[i]*a1(I11,I22,I33)*
			  Source[i](I11,I22,I33,0)*
			  u1(I11+1,I22,I33,j);
     Den(I11,I22,I33) += dy[i]*dy[i]*dx[i]*a1(I11,I22,I33)*
			 Source[i](I11,I22,I33,0);
   }
   elsewhere(Source[i](I11,I22,I33,0)< 0.){
     Mat1(I11,I22,I33) -= dy[i]*dy[i]*dx[i]*a1(I11,I22,I33)*
			  Source[i](I11,I22,I33,0)*
			  u1(I11-1,I22,I33,j);
     Den(I11,I22,I33) -= dy[i]*dy[i]*dx[i]*a1(I11,I22,I33)*
			 Source[i](I11,I22,I33,0);
   }
   where(Source[i](I11,I22,I33,1)>= 0.){
      Mat2(I11,I22,I33) += dx[i]*dx[i]*dy[i]*b1(I11,I22,I33)*
			   Source[i](I11,I22,I33,1)*
			   u1(I11,I22+1,I33,j);
      Den(I11,I22,I33) += dx[i]*dx[i]*dy[i]*b1(I11,I22,I33)*
			  Source[i](I11,I22,I33,1);
   }
   elsewhere(Source[i](I11,I22,I33,1)< 0.){
      Mat2(I11,I22,I33) -= dx[i]*dx[i]*dy[i]*b1(I11,I22,I33)*
			   Source[i](I11,I22,I33,1)*
			   u1(I11,I22-1,I33,j);
      Den(I11,I22,I33) -= dx[i]*dx[i]*dy[i]*b1(I11,I22,I33)*
			  Source[i](I11,I22,I33,1);
   }

  u1(I11,I22,I33,j) = (1-omega)*u1(I11,I22,I33,j) +
		      omega*(Mat1(I11,I22,I33)+
			     Mat2(I11,I22,I33)+
			     Mat3(I11,I22,I33))/Den(I11,I22,I33);
   }
   TRY*******/

/**********
 if (TEST_RESIDUAL==1){
 for (int j2=1;j2<I2[i].getBound();j2++){
  real maxdiff, maxquotient;
  int imax, jmax;
  maxdiff=0., maxquotient=1.0, imax=1, jmax=1;
 for (int i2 = 1; i2<I1[i].getBound(); i2++){
  real a00,b00;
  a00=fabs(-dx[i]*dy[i]*c1(i2,j2,0)/4.)+fabs(dx[i]*dx[i]*b1(i2,j2,0))+
      fabs(0.5*dx[i]*dx[i]*dy[i]*b1(i2,j2,0)*
      Source[i](i2,j2,0,1))+fabs(dx[i]*dy[i]*c1(i2,j2,0)/4.)+
      fabs(dy[i]*dy[i]*a1(i2,j2,0)+0.5*dy[i]*dy[i]*dx[i]*a1(i2,j2,0)*
	   Source[i](i2,j2,0,0))+
      fabs(dy[i]*dy[i]*a1(i2,j2,0)-0.5*dy[i]*dy[i]*dx[i]*a1(i2,j2,0)*
	   Source[i](i2,j2,0,0))+
      fabs(dx[i]*dy[i]*c1(i2,j2,0)/4.)+
      fabs(dx[i]*dx[i]*b1(i2,j2,0)-0.5*dx[i]*dx[i]*dy[i]*b1(i2,j2,0)*
           Source[i](i2,j2,0,1))+
      fabs(dx[i]*dy[i]*c1(i2,j2,0)/4.);
  b00=fabs( 2.0*a1(i2,j2,0)*dy[i]*dy[i]+2.0*b1(i2,j2,0,0)*dx[i]*dx[i]);

  if (fabs(a00-b00) > maxdiff){
      maxdiff=fabs(b00-a00);
      imax=i2, jmax=j2, maxquotient=a00/b00;
      }
	   }
  printf("j=%i\t maxdiff=%g\t maxquotient=%g\t at i=%i  j=%i\n",
	  j2, maxdiff, maxquotient,imax,jmax);
  }
 }
 *********/

    if (TEST_RESIDUAL){
     printf("Dans solve apres interior i=%i\t j=%i\n",i,j);
     int imax=0,jmax=0,i1max=0, j1max=0;
     real resmaxx=0.0, resmaxy=0.0;
     realArray restemp1;
     restemp1.redim(I1[i],I2[i],I3[i],ndimension);
     restemp1=0.0;
     getResidual(restemp1,i,uprev);
     for (int j1=0; j1<=I2[i].getBound(); j1++)
       for (int i1=0; i1<=I1[i].getBound(); i1++){
	 if (fabs(restemp1(i1,j1,0,0))>resmaxx)
	   imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
         if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	    i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
        }
      printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
		    imax,jmax,resmaxx,i1max,j1max,resmaxy);
  //restemp1(I1[i],Range(0,3),0,Range(0,1)).display("C'est restemp1");
  }
  //applyOrthogonalBoundaries(I11,I22,I33,j,i,u1,uprev);
  //printf("x33=%g   y33=%g   :: %g    %g\t <> %g   %g\n",u[i](33,0,0,0),u[i](33,0,0,1),u1(33,0,0,0),u1(33,0,0,1), uprev[i](33,0,0,0), uprev[i](33,0,0,1));
  //printf("x32=%g   y32=%g\t :: %g    %g\n",u[i](32,0,0,0), u[i](32,0,0,1), uprev[i](32,0,0,0), uprev[i](32,0,0,1));
  //printf("x31=%g   y31=%g\t :: %g    %g\n",u[i](31,0,0,0), u[i](31,0,0,1), uprev[i](31,0,0,0), uprev[i](31,0,0,1));
 }

 void multigrid2::
   line2Dsolve(realArray &a1, realArray &b1, 
	       realArray &c1, realArray &d1,
	       Index I11, Index I22, Index I33, 
	       const realArray &coeff1, const realArray &coeff2, 
	       const realArray &coeff3, const realArray &coeff4,
	       const realArray &coeff5, Index Ic1, Index Ic2, 
	       Index Ic3, int j, int i, realArray &u1, int isweep, 
	       realArray *up){
   TridiagonalSolver tri;
   realArray Xc, Yc, Xe, Ye, X2, Y2, XY, normXc, normXe;
   int i1, j1,istart, jstart, istop,jstop, isharp=0;
   jstart=I2[i].getBase(), jstop=I2[i].getBound();
   istart=I1[i].getBase(), istop=I1[i].getBound();

    /*****VOYONS
    printf("\n variable=%i\t level=%i\t isweep=%i\n\n", j,i,isweep);
    //if (i==1){
     for (j1=jstart;j1<=jstop;j1++)
     for (i1=istart;i1<=istop;i1++)
      printf("Before i=%i\t j=%i\t x=%g\t y=%g\t xp=%g\t yp=%g\n",i1,j1,u1(i1,j1,0,0),u1(i1,j1,0,1),uprev[i](i1,j1,0,0),uprev[i](i1,j1,0,1));
    //}
   VOYONS****/
   a1=0.0, b1=0.0, c1=0.0, d1=0.0;
   b1(Ic1,Ic2,Ic3)=-2.0*coeff1(Ic1,Ic2,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(Ic1,Ic2,Ic3)/(dy[i]*dy[i]);
   int axis=isweep, side, i0, j0;
   Index I111,I222,I333;
   intArray index(2,3);
   int is[3], icdif[3];

   index(0,0)=I1[i].getBase(), index(1,0)=I1[i].getBound();
   index(0,1)=I2[i].getBase(), index(1,1)=I2[i].getBound();
   index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
   if (isweep==0){
    istart=I11.getBase(), istop=I11.getBound();
    a1(Ic1,Ic2,Ic3)=coeff1(Ic1,Ic2,Ic3)*(1.0/(dx[i]*dx[i])-
	                       Source[i](Ic1,Ic2,Ic3,0)/(2*dx[i]));
    c1(Ic1,Ic2,Ic3)=coeff1(Ic1,Ic2,Ic3)*(1.0/(dx[i]*dx[i])+
			       Source[i](Ic1,Ic2,Ic3,0)/(2*dx[i]));
    d1(Ic1,Ic2,Ic3)=RHS[i](Ic1,Ic2,Ic3,j)+
		    coeff3(Ic1,Ic2,Ic3)*(u1(Ic1+1,Ic2+1,Ic3,j)-
					 u1(Ic1+1,Ic2-1,Ic3,j)-
					 u1(Ic1-1,Ic2+1,Ic3,j)+
					 u1(Ic1-1,Ic2-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                    coeff2(Ic1,Ic2,Ic3)*((u1(Ic1,Ic2+1,Ic3,j)+
		    u1(Ic1,Ic2-1,Ic3,j))/(dy[i]*dy[i])+
		    Source[i](Ic1,Ic2,Ic3,1)*
		    (u1(Ic1,Ic2+1,Ic3,j)-u1(Ic1,Ic2-1,Ic3,j))/(2*dy[i]));
    if (userMap->getIsPeriodic(0) != Mapping::functionPeriodic){
      for (side=0;side<2;side++){
       i0=2*axis+side;
       is[axis]=1-2*side;
       getBoundaryIndex(index,side,axis,I111,I222,I333);
       if ((gridBc(side,axis)==1)||(gridBc(side,axis)==3)){
        b1(I111,I22,I33)=1.0;
        d1(I111,I22,I33)=u1(I111,I22,I33,j);
       }
       else if (gridBc(side,axis)==2){
        Xe.redim(I111,I22,I333), Ye.redim(I111,I22,I333);
        X2.redim(I111,I22,I333), Y2.redim(I111,I22,I333);
        XY.redim(I111,I22,I333), normXe.redim(I111,I22,I333);

        Xe(I111,I22,I333)=uprev[i](I111,I22+1,I33,0)-
	          uprev[i](I111,I22-1,I33,0);
        Ye(I111,I22,I333)=uprev[i](I111,I22+1,I33,1)-
	          uprev[i](I111,I22-1,I33,1);
        X2(I111,I22,I333)=Xe(I111,I22,I333)*Xe(I111,I22,I333);
        Y2(I111,I22,I333)=Ye(I111,I22,I333)*Ye(I111,I22,I333);
        XY(I111,I22,I333)=Xe(I111,I22,I333)*Ye(I111,I22,I333);
        normXe(I111,I22,I333)=X2(I111,I22,I333)+Y2(I111,I22,I333);

        if (side==0) a1(I111,I22,I33)= 0.0;
	else if (side==1) c1(I111,I22,I33)=0.0;
        b1(I111,I22,I33)= normXe(I111,I22,I333);
        if (i==0){
         if (j==0){
          if (side==0) c1(I111,I22,I33)= -X2(I111,I22,I33);
	  else if (side==1) a1(I111,I22,I33) = -X2(I111,I22,I33);
	  d1(I111,I22,I33)=
	     XY(I111,I22,I333)*u1(I111+is[0],I22,I33,1)-
	     XY(I111,I22,I333)*uprev[i](I111,I22-1,I33,1)+
	     Y2(I111,I22,I333)*uprev[i](I111,I22-1,I33,0);
         }
         else{
          if (side==0) c1(I111,I22,I33)= -Y2(I111,I22,I333);
	  else if (side==1) a1(I111,I22,I33)= -Y2(I111,I22,I333);
	  d1(I111,I22,I33)= 
	     X2(I111,I22,I333)*uprev[i](I111,I22-1,I33,1)-
	     XY(I111,I22,I333)*uprev[i](I111,I22-1,I33,0)+
	     XY(I111,I22,I333)*u1(I111+is[0],I22,I33,0);
         }
        }
        else{
         if (j==0){
          if (side==0) c1(I111,I22,I33)= -X2(I111,I22,I333);
	  else if (side==1) a1(I111,I22,I333)=-X2(I111,I22,I333);
	  d1(I111,I22,I33)= 
	     XY(I111,I22,I333)*u1(I111+is[0],I22,I33,1)-
	     Xe(I111,I22,I333)*RHS[i](I111,I22,I33,0)-
	     Ye(I111,I22,I333)*RHS[i](I111,I22,I33,1);
         }
         else{
          if (side==0) c1(I111,I22,I33)= -Y2(I111,I22,I333);
	  else if (side==1) a1(I111,I22,I33)= -Y2(I111,I22,I333);
	  d1(I111,I22,I33)=
	     Xe(I111,I22)*RHS[i](I111,I22,I33,1)+
	     XY(I111,I22)*u1(I111+is[0],I22,I33,0)-
	     Ye(I111,I22)*RHS[i](I111,I22,I33,0);
         }
       }
       if (I22.getBase()==jstart){
        a1(I111,jstart,I33)=0.0;
	c1(I111,jstart,I33)=0.0;  
	d1(I111,jstart,I33)=u1(I111,jstart,I33,j);
	b1(I111,jstart,I33)=1.0;
       }
       if (I22.getBound()==jstop){
        a1(I111,jstop,I33)=0.0;
	c1(I111,jstop,I33)=0.0;  
	d1(I111,jstop,I33)=u1(I111,jstop,I33,j);
	b1(I111,jstop,I33)=1.0;
       }
       /******VOYONS****/
       if (i==0){
	if (smoothingMethod==3){
          isharp=0;
	  for (i1=I111.getBase();i1<=I111.getBound();i1++)
           for (j1=I22.getBase();j1<=I22.getBound();j1++)
	    if (((u1(i1,j1+1,0,0)-u1(i1,j1,0,0))*
	        (u1(i1,j1-1,0,0)-u1(i1,j1,0,0))+
	        (u1(i1,j1+1,0,1)-u1(i1,j1,0,1))* 
	        (u1(i1,j1-1,0,1)-u1(i1,j1,0,1)))>10.*REAL_EPSILON){
	       isharp=1;
	       //printf("sharp at i1=%i\t j1=%i\t voyons=%g\n",i1,j1,
	       //(u1(i1,j1+1,0,0)-u1(i1,j1,0,0))*
			       //(u1(i1,j1-1,0,0)-u1(i1,j1,0,0))+
			       //(u1(i1,j1+1,0,1)-u1(i1,j1,0,1))*
			       //(u1(i1,j1-1,0,1)-u1(i1,j1,0,1)));
                //printf("i1, j1+1 :: x=%g  <>  %g\t y=%g  <>  %g\n",u1(i1,j1+1,0,0), uprev[i](i1,j1+1,0,0), u1(i1,j1+1,0,1), uprev[i](i1,j1+1,0,1));
                //printf("i1, j1 :: x=%g  <>  %g\t y=%g  <>  %g\n",u1(i1,j1,0,0), uprev[i](i1,j1,0,0),u1(i1,j1,0,1),uprev[i](i1,j1,0,1));
                //printf("i1, j1-1 :: x=%g  <>  %g\t y=%g  <>  %g\n",u1(i1,j1-1,0,0),uprev[i](i1,j1-1,0,0), u1(i1,j1-1,0,1),uprev[i](i1,j1-1,0,1));
	       break;
	   }
          if (isharp==1)
	   for (i1=I111.getBase();i1<=I111.getBound();i1++)
	    for (j1=I22.getBase();j1<=I22.getBound();j1+=I22.getStride()){
	      if (((uprev[i](i1,j1+1,0,0)-uprev[i](i1,j1,0,0))*
	         (uprev[i](i1,j1-1,0,0)-uprev[i](i1,j1,0,0))>0.)||
	         ((uprev[i](i1,j1+1,0,1)-uprev[i](i1,j1,0,1))* 
	         (uprev[i](i1,j1-1,0,1)-uprev[i](i1,j1,0,1))>0.)||
	         (((utmp[i](i1,j1+1,0,0)-u1(i1,j1,0,0))*
	         (utmp[i](i1,j1-1,0,0)-u1(i1,j1,0,0)))+
	         ((utmp[i](i1,j1+1,0,1)-u1(i1,j1,0,1))* 
	         (utmp[i](i1,j1-1,0,1)-u1(i1,j1,0,1)))>0.)){
	     GRIDBC[i0][i](i1,j1,0)=1;
             a1(i1,j1,0)=0.0;
	     c1(i1,j1,0)=0.0;
	     b1(i1,j1,0)=1.0;
	     d1(i1,j1,0)=uprev[i](i1,j1,0,j);
	     if (j1<I22.getBound()){
	      GRIDBC[i0][i](i1,j1+1,0)=1;
	      a1(i1,j1+1,0)=0.0;
	      c1(i1,j1+1,0)=0.0;
	      b1(i1,j1+1,0)=1.0;
	      d1(i1,j1+1,0)=uprev[i](i1,j1+1,0,j);
            }
	    if (j1>I22.getBase()){
	      GRIDBC[i0][i](i1,j1-1,0)=1;
	      a1(i1,j1-1,0)=0.0;
	      c1(i1,j1-1,0)=0.0;
	      b1(i1,j1-1,0)=1.0;
	      d1(i1,j1-1,0)=uprev[i](i1,j1-1,0,j);
	    }
           }
	   else if (GRIDBC[i0][i](i1,j1,0)==1){
             a1(i1,j1,0)=0.0;
	     c1(i1,j1,0)=0.0;
	     b1(i1,j1,0)=1.0;
	     d1(i1,j1,0)=uprev[i](i1,j1,0,j);
	   }
         }
       /****VOYONS****/
	}
	else {
	 where (GRIDBC[i0][i](I111,I22,I333)==1){
	  a1(I111,I22,0)=0.0;
	  c1(I111,I22,0)=0.0;
	  b1(I111,I22,0)=1.0;
	  d1(I111,I22,0)=uprev[i](I111,I22,0,j);
	 }
	}
       }
       else {
	for (i1=I111.getBase();i1<=I111.getBound();i1++){
	 for (j1=jstart;j1<=jstop;j1++)
	   GRIDBC[i0][i](i1,j1,0)=GRIDBC[i0][i-1](2*i1,2*j1,0);

	   GRIDBC[i0][i](i1,-1,0)=GRIDBC[i0][i-1](2*i1,-1,0);
	   GRIDBC[i0][i](i1,jstop+1,0)=GRIDBC[i0][i-1](2*i1,I2[i-1].getBound()+1,0);
       }

       where (GRIDBC[i0][i](I111,I22,0)==1){
        a1(I111,I22,0)=0.0;
        c1(I111,I22,0)=0.0;
        b1(I111,I22,0)=1.0;
        d1(I111,I22,0)=uprev[i](I111,I22,0,j);
       }
      }
       /****VOYONS****/
     }
    }

    if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic){
     jstart=I2[i].getBase(), jstop=I2[i].getBound();
     b1(Ic1,jstart,Ic3)=-2.0*coeff1(Ic1,jstart,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(Ic1,jstart,Ic3)/(dy[i]*dy[i]);
     a1(Ic1,jstart,Ic3)=coeff1(Ic1,jstart,Ic3)*(1.0/(dx[i]*dx[i])-
                       Source[i](Ic1,jstart,Ic3,0)/(2*dx[i]));
     c1(Ic1,jstart,Ic3)=coeff1(Ic1,jstart,Ic3)*(1.0/(dx[i]*dx[i])+
		       Source[i](Ic1,jstart,Ic3,0)/(2*dx[i]));
     d1(Ic1,jstart,Ic3)=RHS[i](Ic1,jstart,Ic3,j)+
	    coeff3(Ic1,jstart,Ic3)*(u1(Ic1+1,jstart+1,Ic3,j)-
				 u1(Ic1-1,jstart+1,Ic3,j)-
				 u1(Ic1+1,jstop-1,Ic3,j)+
				 u1(Ic1-1,jstop-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
            coeff2(Ic1,jstart,Ic3)*((u1(Ic1,jstart+1,Ic3,j)+
		    u1(Ic1,jstop-1,Ic3,j))/(dy[i]*dy[i])+
		    Source[i](Ic1,jstart,Ic3,1)*
		    (u1(Ic1,jstart+1,Ic3,j)-u1(Ic1,jstop-1,Ic3,j))/(2*dy[i]));
       
       b1(Ic1,jstop,Ic3)=-2.0*coeff1(Ic1,jstop,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(Ic1,jstop,Ic3)/(dy[i]*dy[i]);
       a1(Ic1,jstop,Ic3)=coeff1(Ic1,jstop,Ic3)*(1.0/(dx[i]*dx[i])-
	                       Source[i](Ic1,jstop,Ic3,0)/(2*dx[i]));
       c1(Ic1,jstop,Ic3)=coeff1(Ic1,jstop,Ic3)*(1.0/(dx[i]*dx[i])+
			       Source[i](Ic1,jstop,Ic3,0)/(2*dx[i]));
       d1(Ic1,jstop,Ic3)=RHS[i](Ic1,jstop,Ic3,j)+
		    coeff3(Ic1,jstop,Ic3)*(u1(Ic1+1,jstart+1,Ic3,j)-
					 u1(Ic1-1,jstart+1,Ic3,j)-
					 u1(Ic1+1,jstop-1,Ic3,j)+
					 u1(Ic1-1,jstop-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                    coeff2(Ic1,jstop,Ic3)*((u1(Ic1,jstart+1,Ic3,j)+
		    u1(Ic1,jstop-1,Ic3,j))/(dy[i]*dy[i])+
		    Source[i](Ic1,jstop,Ic3,1)*
		    (u1(Ic1,jstart+1,Ic3,j)-u1(Ic1,jstop-1,Ic3,j))/(2*dy[i]));
      }
      tri.factor(a1,b1,c1,TridiagonalSolver::normal,axis1);
      tri.solve(d1,I11,I22,I33);
      u1(I11,I22,I33,j)=(1.0-omega)*u1(I11,I22,I33,j)+
		     omega*d1(I11,I22,I33);
     }
     else if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic){
       realArray a11,b11,c11,d11;
       Index I111;

       I111=Range(I11.getBase(),I11.getBound()-1);
       a11.redim(I111,I22,I33), a11=0.0;
       b11.redim(I111,I22,I33), b11=0.0;
       c11.redim(I111,I22,I33), c11=0.0;
       d11.redim(I111,I22,I33), d11=0.0;

       b1(istart,Ic2,Ic3)=-2.0*coeff1(istart,Ic2,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(istart,Ic2,Ic3)/(dy[i]*dy[i]);
       a1(istart,Ic2,Ic3)=coeff1(istart,Ic2,Ic3)*(1.0/(dx[i]*dx[i])-
	                       Source[i](istart,Ic2,Ic3,0)/(2*dx[i]));
       c1(istart,Ic2,Ic3)=coeff1(istart,Ic2,Ic3)*(1.0/(dx[i]*dx[i])+
			       Source[i](istart,Ic2,Ic3,0)/(2*dx[i]));
       d1(istart,Ic2,Ic3)=RHS[i](istart,Ic2,Ic3,j)+
		 coeff3(istart,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
					 u1(istart+1,Ic2-1,Ic3,j)-
					 u1(istop-1,Ic2+1,Ic3,j)+
					 u1(istop-1,Ic2-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                 coeff2(istart,Ic2,Ic3)*((u1(istart,Ic2+1,Ic3,j)+
		 u1(istart,Ic2-1,Ic3,j))/(dy[i]*dy[i])+
		 Source[i](istart,Ic2,Ic3,1)*
		 (u1(istart,Ic2+1,Ic3,j)-u1(istart,Ic2-1,Ic3,j))/(2*dy[i]));
       /*****VOYONS
       if (i>0){
	for (i1=istart;i1<=istop;i1++)
	 if ((((u1(i1+1,jstart,0,0)-u1(i1,jstart,0,0))*
	      (u1(i1-1,jstart,0,0)-u1(i1,jstart,0,0))+
	      (u1(i1+1,jstart,0,1)-u1(i1,jstart,0,1))*   
	      (u1(i1-1,jstart,0,1)-u1(i1,jstart,0,1)))>0.0)||
	     ((u1(i1+1,jstart,0,0)-u1(i1,jstart,0,0))*
	      (u1(i1-1,jstart,0,0)-u1(i1,jstart,0,0))>0.0)||
	     ((u1(i1+1,jstart,0,1)-u1(i1,jstart,0,1))*   
	      (u1(i1-1,jstart,0,1)-u1(i1,jstart,0,1))>0.0)){ 
           a1(i1,jstart+1,0)=0.0;
           c1(i1,jstart+1,0)=0.0;
           b1(i1,jstart+1,0)=1.0;
           d1(i1,jstart+1,0)=u1(i1,jstart+1,0,j);
	   if (i1<istop){
             a1(i1+1,jstart+1,0)=0.0;
             c1(i1+1,jstart+1,0)=0.0;
             b1(i1+1,jstart+1,0)=1.0;
             d1(i1+1,jstart+1,0)=u1(i1+1,jstart+1,0,j);
	   }
	   if (i1>istart){
             a1(i1-1,jstart+1,0)=0.0;
             c1(i1-1,jstart+1,0)=0.0;
             b1(i1-1,jstart+1,0)=1.0;
             d1(i1-1,jstart+1,0)=u1(i1-1,jstart+1,0,j);
	   }
         }
	for (i1=istart;i1<=istop;i1++)
	 if (((u1(i1+1,jstop,0,0)-u1(i1,jstop,0,0))*
	      (u1(i1-1,jstop,0,0)-u1(i1,jstop,0,0))+
	      (u1(i1+1,jstop,0,1)-u1(i1,jstop,0,1))*   
	      (u1(i1-1,jstop,0,1)-u1(i1,jstop,0,1)))>0.0){ 
           a1(i1,jstop-1,0)=0.0;
           c1(i1,jstop-1,0)=0.0;
           b1(i1,jstop-1,0)=1.0;
           d1(i1,jstop-1,0)=u1(i1,jstop-1,0,j);
	   if (i1<istop){
             a1(i1+1,jstop-1,0)=0.0;
             c1(i1+1,jstop-1,0)=0.0;
             b1(i1+1,jstop-1,0)=1.0;
             d1(i1+1,jstop-1,0)=u1(i1+1,jstop-1,0,j);
	   }
	   if (i1>istart){
             a1(i1-1,jstop-1,0)=0.0;
             c1(i1-1,jstop-1,0)=0.0;
             b1(i1-1,jstop-1,0)=1.0;
             d1(i1-1,jstop-1,0)=u1(i1-1,jstop-1,0,j);
	   }
         }
       }
       VOYONS*******/

       a11(I111,I22,I33)=a1(I111,I22,I33);
       b11(I111,I22,I33)=b1(I111,I22,I33);
       c11(I111,I22,I33)=c1(I111,I22,I33);
       d11(I111,I22,I33)=d1(I111,I22,I33);

       /**** SHOULD NOT BE INCLUDED
       b1(istop,Ic2,Ic3)=-2.0*coeff1(istop,Ic2,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(istop,Ic2,Ic3)/(dy[i]*dy[i]);
       a1(istop,Ic2,Ic3)=coeff1(istop,Ic2,Ic3)*(1.0/(dx[i]*dx[i])-
	                       Source[i](istop,Ic2,Ic3,0)/(2*dx[i]));
       c1(istop,Ic2,Ic3)=coeff1(istop,Ic2,Ic3)*(1.0/(dx[i]*dx[i])+
			       Source[i](istop,Ic2,Ic3,0)/(2*dx[i]));
       d1(istop,Ic2,Ic3)=RHS[i](istop,Ic2,Ic3,j)+
		 coeff3(istop,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
					 u1(istart+1,Ic2-1,Ic3,j)-
					 u1(istop-1,Ic2+1,Ic3,j)+
					 u1(istop-1,Ic2-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                 coeff2(istop,Ic2,Ic3)*((u1(istop,Ic2+1,Ic3,j)+
		 u1(istop,Ic2-1,Ic3,j))/(dy[i]*dy[i])+
		 Source[i](istop,Ic2,Ic3,1)*
		 (u1(istop,Ic2+1,Ic3,j)-u1(istop,Ic2-1,Ic3,j))/(2*dy[i]));
        SHOULD NOT BE INCLUDED *****/

      tri.factor(a11,b11,c11,TridiagonalSolver::periodic,axis1);
      tri.solve(d11,I111,I22,I33);
      u1(I111,I22,I33,j)=d11(I111,I22,I33);
      u1(istop,I22,I33,j)=u1(istart,I22,I33,j);
     }
     u1(istart,jstart,0,Range(0,1))=uprev[i](istart,jstart,0,Range(0,1));
     u1(istop,jstart,0,Range(0,1))=uprev[i](istop,jstart,0,Range(0,1));
     u1(istart,jstop,0,Range(0,1))=uprev[i](istart,jstop,0,Range(0,1));
     u1(istop,jstop,0,Range(0,1))=uprev[i](istop,jstop,0,Range(0,1));
   fflush(stdout);
   }
   else if (isweep==1){
    jstart=I22.getBase(), jstop=I22.getBound();
    a1(Ic1,Ic2,Ic3)=coeff2(Ic1,Ic2,Ic3)*(1.0/(dy[i]*dy[i])-
	                       Source[i](Ic1,Ic2,Ic3,1)/(2*dy[i]));
    c1(Ic1,Ic2,Ic3)=coeff2(Ic1,Ic2,Ic3)*(1.0/(dy[i]*dy[i])+
			       Source[i](Ic1,Ic2,Ic3,1)/(2*dy[i]));
    d1(Ic1,Ic2,Ic3)=RHS[i](Ic1,Ic2,Ic3,j)+
		    coeff3(Ic1,Ic2,Ic3)*(u1(Ic1+1,Ic2+1,Ic3,j)-
					 u1(Ic1+1,Ic2-1,Ic3,j)-
					 u1(Ic1-1,Ic2+1,Ic3,j)+
					 u1(Ic1-1,Ic2-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                    coeff1(Ic1,Ic2,Ic3)*((u1(Ic1+1,Ic2,Ic3,j)+
		    u1(Ic1-1,Ic2,Ic3,j))/(dx[i]*dx[i])+
		    Source[i](Ic1,Ic2,Ic3,0)*
		    (u1(Ic1+1,Ic2,Ic3,j)-u1(Ic1-1,Ic2,Ic3,j))/(2*dx[i]));
    if (userMap->getIsPeriodic(1) != Mapping::functionPeriodic){
      for (side=0;side<2;side++){
       i0=2*axis+side;
       is[axis]=1-2*side;
       getBoundaryIndex(index,side,axis,I111,I222,I333);
       if ((gridBc(side,axis)==1)||(gridBc(side,axis)==3)){
        b1(I11,I222,I33)=1.0;
        d1(I11,I222,I33)=u1(I11,I222,I33,j);
      }
      else if (gridBc(side,axis)==2){
       Xc.redim(I11,I222,I333), Yc.redim(I11,I222,I333);
       X2.redim(I11,I222,I333), Y2.redim(I11,I222,I333);
       XY.redim(I11,I222,I333), normXc.redim(I11,I222,I333);

       Xc(I11,I222,I333)=uprev[i](I11+1,I222,I33,0)-
	           uprev[i](I11-1,I222,I33,0);
       Yc(I11,I222,I333)=uprev[i](I11+1,I222,I33,1)-
	           uprev[i](I11-1,I222,I33,1);
       X2(I11,I222,I333)=Xc(I11,I222,I333)*Xc(I11,I222,I333);
       Y2(I11,I222,I333)=Yc(I11,I222,I333)*Yc(I11,I222,I333);
       XY(I11,I222,I333)=Xc(I11,I222,I333)*Yc(I11,I222,I333);
       normXc(I11,I222,I333)=X2(I11,I222,I333)+Y2(I11,I222,I333);
  
       if (side==0) a1(I11,I222,I33)= 0.0;
       else if (side==1) c1(I11,I222,I33)=0.0;
       b1(I11,I222,I33)= normXc(I11,I222,I33);
       if (i==0){
         if (j==0){
          if (side==0) c1(I11,I222,I33)= -X2(I11,I222,I33);
          else if (side==1) a1(I11,I222,I33)= -X2(I11,I222,I33);
          d1(I11,I222,I33)=
             XY(I11,I222,I33)*u1(I11,I222+1,I33,1)-
             XY(I11,I222,I33)*uprev[i](I11-1,I222,I33,1)+
             Y2(I11,I222,I33)*uprev[i](I11-1,I222,I33,0);
         }
         else{
          if (side==0) c1(I11,I222,I33)= -Y2(I11,I222,I33);
          else if (side==1) a1(I11,I222,I33)= -Y2(I11,I222,I33);
          d1(I11,I222,I33)= 
             X2(I11,I222,I33)*uprev[i](I11-1,I222,I33,1)-
             XY(I11,I222,I33)*uprev[i](I11-1,I222,I33,0)+
             XY(I11,I222,I33)*u1(I11,I222+is[1],I33,0);
         }
       }
       else{
         if (j==0){
          if (side==0) c1(I11,I222,I33)= -X2(I11,I222,I33);
          else if (side==1) a1(I11,I222,I33)= -X2(I11,I222,I33);
          d1(I11,I222,I33)= 
             XY(I11,I222,I33)*u1(I11,I222+is[1],I33,1)-
             Xc(I11,I222,I33)*RHS[i](I11,I222,I33,0)-
             Yc(I11,I222,I33)*RHS[i](I11,I222,I33,1);
         }
         else{
          if (side==0) c1(I11,I222,I33)= -Y2(I11,I222,I33);
          else if (side==1) a1(I11,I222,I33)= -Y2(I11,I222,I33);
          d1(I11,I222,I33)=
             Xc(I11,I222,I33)*RHS[i](I11,I222,I33,1)+
             XY(I11,I222,I33)*u1(I11,I222+is[1],I33,0)-
             Yc(I11,I222,I33)*RHS[i](I11,I222,I33,0);
         }
       }
       if (I11.getBase()==istart){
        a1(istart,I222,I33)=0.0;
	c1(istart,I222,I33)=0.0;  
	d1(istart,I222,I33)=uprev[i](istart,I222,I33,j);
	b1(istart,I222,I33)=1.0;
       }
       if (I11.getBound()==istop){
        a1(istop,I222,I33)=0.0;
	c1(istop,I222,I33)=0.0;  
	d1(istop,I222,I33)=uprev[i](istop,I222,I33,j);
	b1(istop,I222,I33)=1.0;
       }

       /******VOYONS****/
       if (i==0){
	if (smoothingMethod==3){
          isharp=0;
          for (j1=I222.getBase();j1<=I222.getBound();j1++)
	   for (i1=I11.getBase();i1<=I11.getBound();i1++)
	    if (((u1(i1+1,j1,0,0)-u1(i1,j1,0,0))*
	       (u1(i1-1,j1,0,0)-u1(i1,j1,0,0))+
	       (u1(i1+1,j1,0,1)-u1(i1,j1,0,1))* 
	       (u1(i1-1,j1,0,1)-u1(i1,j1,0,1)))>10.*REAL_EPSILON){
	       isharp=1;
	       break;
	   }
          if (isharp==1)
	    for (j1=I222.getBase();j1<=I222.getBound();j1++)
	     for (i1=I11.getBase();i1<=I11.getBound();i1+=I11.getStride()){
	     if (((uprev[i](i1+1,j1,0,0)-uprev[i](i1,j1,0,0))*
	        (uprev[i](i1-1,j1,0,0)-uprev[i](i1,j1,0,0))>0.)||
	        ((uprev[i](i1+1,j1,0,1)-uprev[i](i1,j1,0,1))* 
	        (uprev[i](i1-1,j1,0,1)-uprev[i](i1,j1,0,1))>0.)||
	        (((utmp[i](i1+1,j1,0,0)-u1(i1,j1,0,0))*
	        (utmp[i](i1-1,j1,0,0)-u1(i1,j1,0,0)))+
	        ((utmp[i](i1+1,j1,0,1)-u1(i1,j1,0,1))* 
	        (utmp[i](i1-1,j1,0,1)-u1(i1,j1,0,1)))>0.)){
	      GRIDBC[i0][i](i1,j1,0)=1;
              a1(i1,j1,0)=0.0;
	      c1(i1,j1,0)=0.0;
	      b1(i1,j1,0)=1.0;
	      d1(i1,j1,0)=uprev[i](i1,j1,0,j);
	      if (i1<I11.getBound()){
	       GRIDBC[i0][i](i1+1,j1,0)=1;
	       a1(i1+1,j1,0)=0.0;
	       c1(i1+1,j1,0)=0.0;
	       b1(i1+1,j1,0)=1.0;
	       d1(i1+1,j1,0)=uprev[i](i1+1,j1,0,j);
              }
	      if (i1>I11.getBase()){
	       GRIDBC[i0][i](i1-1,j1,0)=1;
	       a1(i1-1,j1,0)=0.0;
	       c1(i1-1,j1,0)=0.0;
	       b1(i1-1,j1,0)=1.0;
	       d1(i1-1,j1,0)=uprev[i](i1-1,j1,0,j);
	     }
            }
	    else if (GRIDBC[i0][i](i1,j1,0)==1){
              a1(i1,j1,0)=0.0;
	      c1(i1,j1,0)=0.0;
	      b1(i1,j1,0)=1.0;
	      d1(i1,j1,0)=uprev[i](i1,j1,0,j);
	    }
          }
       /****VOYONS****/
	}
	else {
	 where (GRIDBC[i0][i](I11,I222,I333)==1){
	  a1(I11,jstart,0)=0.0;
	  c1(I11,jstart,0)=0.0;
	  b1(I11,jstart,0)=1.0;
	  d1(I11,jstart,0)=uprev[i](I11,jstart,0,j);
	 }
	}
       }
       else {
	for (j1=I222.getBase();j1<=I222.getBound();j1++){
	 for (i1=istart;i1<=istop;i1++)
	  GRIDBC[i0][i](i1,j1,0)=GRIDBC[i0][i-1](2*i1, 2*j1, 0);

	 GRIDBC[i0][i](-1,j1,0)=GRIDBC[i0][i-1](-1,2*j1,0);
	 GRIDBC[i0][i](istop+1,j1,0)=
		    GRIDBC[i0][i-1](I1[i-1].getBound()+1,2*j1,0);
        }

	where (GRIDBC[i0][i](I11,I222,0)==1){
	  a1(I11,I222,0)=0.0;
	  c1(I11,I222,0)=0.0;
	  b1(I11,I222,0)=1.0;
	  d1(I11,I222,0)=uprev[i](I11,I222,0,j);
	 }
       }
      }
     }

     if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic){
      istart=I1[i].getBase(), istop=I1[i].getBound();
      b1(istart,Ic2,Ic3)=-2.0*coeff1(istart,Ic2,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(istart,Ic2,Ic3)/(dy[i]*dy[i]);
      a1(istart,Ic2,Ic3)=coeff2(istart,Ic2,Ic3)*(1.0/(dy[i]*dy[i])-
	                       Source[i](istart,Ic2,Ic3,1)/(2*dy[i]));
      c1(istart,Ic2,Ic3)=coeff2(istart,Ic2,Ic3)*(1.0/(dy[i]*dy[i])+
			       Source[i](istart,Ic2,Ic3,1)/(2*dy[i]));
      d1(istart,Ic2,Ic3)=RHS[i](istart,Ic2,Ic3,j)+
		    coeff3(istart,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
					 u1(istart+1,Ic2-1,Ic3,j)-
					 u1(istop-1,Ic2+1,Ic3,j)+
					 u1(istop-1,Ic2-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                    coeff1(istart,Ic2,Ic3)*((u1(istart+1,Ic2,Ic3,j)+
		    u1(istop-1,Ic2,Ic3,j))/(dx[i]*dx[i])+
		    Source[i](istart,Ic2,Ic3,0)*
		    (u1(istart+1,Ic2,Ic3,j)-u1(istop-1,Ic2,Ic3,j))/(2*dx[i]));

       b1(istop,Ic2,Ic3)=-2.0*coeff1(istop,Ic2,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(istop,Ic2,Ic3)/(dy[i]*dy[i]);
       a1(istop,Ic2,Ic3)=coeff2(istop,Ic2,Ic3)*(1.0/(dy[i]*dy[i])-
	                       Source[i](istop,Ic2,Ic3,1)/(2*dy[i]));
       c1(istop,Ic2,Ic3)=coeff2(istop,Ic2,Ic3)*(1.0/(dy[i]*dy[i])+
			       Source[i](istop,Ic2,Ic3,1)/(2*dy[i]));
       d1(istop,Ic2,Ic3)=RHS[i](istop,Ic2,Ic3,j)+
		    coeff3(istop,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
					 u1(istart+1,Ic2-1,Ic3,j)-
					 u1(istop-1,Ic2+1,Ic3,j)+
					 u1(istop-1,Ic2-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                    coeff1(istop,Ic2,Ic3)*((u1(istart+1,Ic2,Ic3,j)+
		    u1(istop-1,Ic2,Ic3,j))/(dx[i]*dx[i])+
		    Source[i](istop,Ic2,Ic3,0)*
		    (u1(istart+1,Ic2,Ic3,j)-u1(istop-1,Ic2,Ic3,j))/(2*dx[i]));
      }

      tri.factor(a1,b1,c1,TridiagonalSolver::normal,axis2);
      tri.solve(d1,I11,I22,I33);
      u1(I11,I22,I33,j)=(1.0-omega)*u1(I11,I22,I33,j)+
		     omega*d1(I11,I22,I33);
     }
     else if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic){
       realArray a11,b11,c11,d11;
       Index I222;

       I222=Range(I22.getBase(),I22.getBound()-1);
       a11.redim(I11,I222,I33), a11=0.0;
       b11.redim(I11,I222,I33), b11=0.0;
       c11.redim(I11,I222,I33), c11=0.0;
       d11.redim(I11,I222,I33), d11=0.0;

       b1(Ic1,jstart,Ic3)=-2.0*coeff1(Ic1,jstart,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(Ic1,jstart,Ic3)/(dy[i]*dy[i]);
       a1(Ic1,jstart,Ic3)=coeff2(Ic1,jstart,Ic3)*(1.0/(dy[i]*dy[i])-
	                       Source[i](Ic1,jstart,Ic3,1)/(2*dy[i]));
       c1(Ic1,jstart,Ic3)=coeff2(Ic1,jstart,Ic3)*(1.0/(dy[i]*dy[i])+
			       Source[i](Ic1,jstart,Ic3,1)/(2*dy[i]));
       d1(Ic1,jstart,Ic3)=RHS[i](Ic1,jstart,Ic3,j)+
		    coeff3(Ic1,jstart,Ic3)*(u1(Ic1+1,jstart+1,Ic3,j)-
					 u1(Ic1+1,jstop-1,Ic3,j)-
					 u1(Ic1-1,jstart+1,Ic3,j)+
					 u1(Ic1-1,jstop-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                    coeff1(Ic1,jstart,Ic3)*((u1(Ic1+1,jstart,Ic3,j)+
		    u1(Ic1-1,jstart,Ic3,j))/(dx[i]*dx[i])+
		    Source[i](Ic1,jstart,Ic3,0)*
		    (u1(Ic1+1,jstart,Ic3,j)-u1(Ic1-1,jstart,Ic3,j))/(2*dx[i]));

       /*****VOYONS
       if (i>0){
	for (j1=jstart;j1<=jstop;j1++)
	 if (((u1(istart,j1+1,0,0)-u1(istart,j1,0,0))*
	      (u1(istart,j1-1,0,0)-u1(istart,j1,0,0))+
	      (u1(istart,j1+1,0,1)-u1(istart,j1,0,1))*   
	      (u1(istart,j1-1,0,1)-u1(istart,j1,0,1)))>0.0){ 
           a1(istart+1,j1,0)=0.0;
           c1(istart+1,j1,0)=0.0;
           b1(istart+1,j1,0)=1.0;
           d1(istart+1,j1,0)=u1(istart+1,j1,0,j);
	   if (j1<jstop){
             a1(istart+1,j1+1,0)=0.0;
             c1(istart+1,j1+1,0)=0.0;
             b1(istart+1,j1+1,0)=1.0;
             d1(istart+1,j1+1,0)=u1(istart+1,j1+1,0,j);
	   }
	   if (j1>jstart){
             a1(istart+1,j1-1,0)=0.0;
             c1(istart+1,j1-1,0)=0.0;
             b1(istart+1,j1-1,0)=1.0;
             d1(istart+1,j1-1,0)=u1(istart+1,j1-1,0,j);
	   }
         }
	for (j1=jstart;j1<=jstop;j1++)
	 if (((u1(istop,j1+1,0,0)-u1(istop,j1,0,0))*
	      (u1(istop,j1-1,0,0)-u1(istop,j1,0,0))+
	      (u1(istop,j1+1,0,1)-u1(istop,j1,0,1))*   
	      (u1(istop,j1-1,0,1)-u1(istop,j1,0,1)))>0.0){ 
           a1(istop-1,j1,0)=0.0;
           c1(istop-1,j1,0)=0.0;
           b1(istop-1,j1,0)=1.0;
           d1(istop-1,j1,0)=u1(istop-1,j1,0,j);
	   if (j1<jstop){
             a1(istop-1,j1+1,0)=0.0;
             c1(istop-1,j1+1,0)=0.0;
             b1(istop-1,j1+1,0)=1.0;
             d1(istop-1,j1+1,0)=u1(istop-1,j1+1,0,j);
	   }
	   if (j1>jstart){
             a1(istop-1,j1-1,0)=0.0;
             c1(istop-1,j1-1,0)=0.0;
             b1(istop-1,j1-1,0)=1.0;
             d1(istop-1,j1-1,0)=u1(istop-1,j1-1,0,j);
	   }
         }
       }
       VOYONS*****/

       a11(I11,I222,I33)=a1(I11,I222,I33);
       b11(I11,I222,I33)=b1(I11,I222,I33);
       c11(I11,I222,I33)=c1(I11,I222,I33);
       d11(I11,I222,I33)=d1(I11,I222,I33);

       /*****SHOULD NOT BE USED
       b1(Ic1,jstop,Ic3)=-2.0*coeff1(Ic1,jstop,Ic3)/(dx[i]*dx[i])-
	            2.0*coeff2(Ic1,jstop,Ic3)/(dy[i]*dy[i]);
       a1(Ic1,jstop,Ic3)=coeff2(Ic1,jstop,Ic3)*(1.0/(dy[i]*dy[i])-
	                       Source[i](Ic1,jstop,Ic3,1)/(2*dy[i]));
       c1(Ic1,jstop,Ic3)=coeff2(Ic1,jstop,Ic3)*(1.0/(dy[i]*dy[i])+
			       Source[i](Ic1,jstop,Ic3,1)/(2*dy[i]));
       d1(Ic1,jstop,Ic3)=RHS[i](Ic1,jstop,Ic3,j)+
		    coeff3(Ic1,jstop,Ic3)*(u1(Ic1+1,jstart+1,Ic3,j)-
					 u1(Ic1+1,jstop-1,Ic3,j)-
					 u1(Ic1-1,jstart+1,Ic3,j)+
					 u1(Ic1-1,jstop-1,Ic3,j))/
					 (4.0*dx[i]*dy[i])-
                    coeff1(Ic1,jstop,Ic3)*((u1(Ic1+1,jstop,Ic3,j)+
		    u1(Ic1-1,jstop,Ic3,j))/(dx[i]*dx[i])+
		    Source[i](Ic1,jstop,Ic3,0)*
		    (u1(Ic1+1,jstop,Ic3,j)-u1(Ic1-1,jstop,Ic3,j))/(2*dx[i]));
       SHOULD NOT BE USED *****/

      tri.factor(a11,b11,c11,TridiagonalSolver::periodic,axis2);
      tri.solve(d11,I11,I222,I33);
      u1(I11,I222,I33,j)=(1.0-omega)*u1(I11,I222,I33,j)+
		     omega*d1(I11,I222,I33);
      u1(I11,jstop,I33,j)=u1(I11,jstart,I33,j);
     }
     u1(istart,jstart,0,Range(0,1))=uprev[i](istart,jstart,0,Range(0,1));
     u1(istop,jstart,0,Range(0,1))=uprev[i](istop,jstart,0,Range(0,1));
     u1(istart,jstop,0,Range(0,1))=uprev[i](istart,jstop,0,Range(0,1));
     u1(istop,jstop,0,Range(0,1))=uprev[i](istop,jstop,0,Range(0,1));
    fflush(stdout);
   }

   /*****VOYONS 
   if (j==1){
     jstart=I2[i].getBase(), jstop=I2[i].getBound();
     istart=I1[i].getBase(), istop=I1[i].getBound();
     if (gridBc(0,1)==2){
       where(((u1(I1[i]+1,jstart,I33,0)-u1(I1[i],jstart,I33,0))*
	      (u1(I1[i]-1,jstart,I33,0)-u1(I1[i],jstart,I33,0))+
	      (u1(I1[i]+1,jstart,I33,1)-u1(I1[i],jstart,I33,1))* 
	      (u1(I1[i]-1,jstart,I33,1)-u1(I1[i],jstart,I33,1)))>10.*REAL_EPSILON){
        u1(I1[i],jstart,I33,0)=utmp[i](I1[i],jstart,I33,0);
        u1(I1[i],jstart,I33,1)=utmp[i](I1[i],jstart,I33,1);
       }


       where(((u1(I1[i]+1,jstart,I33,0)-u1(I1[i],jstart,I33,0))*
	      (u1(I1[i]-1,jstart,I33,0)-u1(I1[i],jstart,I33,0))+
	      (u1(I1[i]+1,jstart,I33,1)-u1(I1[i],jstart,I33,1))* 
	      (u1(I1[i]-1,jstart,I33,1)-u1(I1[i],jstart,I33,1)))>10.*REAL_EPSILON){
        u1(I1[i],jstart,I33,0)=uprev[i](I1[i],jstart,I33,0);
        u1(I1[i],jstart,I33,1)=uprev[i](I1[i],jstart,I33,1);
        u1(I1[i]+1,jstart,I33,0)=uprev[i](I1[i]+1,jstart,I33,0);
        u1(I1[i]+1,jstart,I33,1)=uprev[i](I1[i]+1,jstart,I33,1);
        u1(I1[i]-1,jstart,I33,0)=uprev[i](I1[i]-1,jstart,I33,0);
        u1(I1[i]-1,jstart,I33,1)=uprev[i](I1[i]-1,jstart,I33,1);
       }
     }
     if (gridBc(1,1)==2){
       where(((u1(I1[i]+1,jstop,I33,0)-u1(I1[i],jstop,I33,0))*
	      (u1(I1[i]-1,jstop,I33,0)-u1(I1[i],jstop,I33,0))+
	      (u1(I1[i]+1,jstop,I33,1)-u1(I1[i],jstop,I33,1))* 
	      (u1(I1[i]-1,jstop,I33,1)-u1(I1[i],jstop,I33,1)))>10.*REAL_EPSILON){
        u1(I1[i],jstop,I33,0)=utmp[i](I1[i],jstop,I33,0);
        u1(I1[i],jstop,I33,1)=utmp[i](I1[i],jstop,I33,1);
       }


       where(((u1(I1[i]+1,jstop,I33,0)-u1(I1[i],jstop,I33,0))*
	      (u1(I1[i]-1,jstop,I33,0)-u1(I1[i],jstop,I33,0))+
	      (u1(I1[i]+1,jstop,I33,1)-u1(I1[i],jstop,I33,1))* 
	      (u1(I1[i]-1,jstop,I33,1)-u1(I1[i],jstop,I33,1)))>10.*REAL_EPSILON){
        u1(I1[i],jstop,I33,0)=uprev[i](I1[i],jstop,I33,0);
        u1(I1[i],jstop,I33,1)=uprev[i](I1[i],jstop,I33,1);
        u1(I1[i]+1,jstop,I33,0)=uprev[i](I1[i]+1,jstop,I33,0);
        u1(I1[i]+1,jstop,I33,1)=uprev[i](I1[i]+1,jstop,I33,1);
        u1(I1[i]-1,jstop,I33,0)=uprev[i](I1[i]-1,jstop,I33,0);
        u1(I1[i]-1,jstop,I33,1)=uprev[i](I1[i]-1,jstop,I33,1);
       }
     }
     if (gridBc(0,0)==2){
       where(((u1(istart,I2[i]+1,I33,0)-u1(istart,I2[i],I33,0))*
	      (u1(istart,I2[i]-1,I33,0)-u1(istart,I2[i],I33,0))+
	      (u1(istart,I2[i]+1,I33,1)-u1(istart,I2[i],I33,1))* 
	      (u1(istart,I2[i]-1,I33,1)-u1(istart,I2[i],I33,1)))>10.*REAL_EPSILON){
        u1(istart,I2[i],I33,0)=utmp[i](istart,I2[i],I33,0);
        u1(istart,I2[i],I33,1)=utmp[i](istart,I2[i],I33,1);
       }


       where(((u1(istart,I2[i]+1,I33,0)-u1(istart,I2[i],I33,0))*
	      (u1(istart,I2[i]-1,I33,0)-u1(istart,I2[i],I33,0))+
	      (u1(istart,I2[i]+1,I33,1)-u1(istart,I2[i],I33,1))* 
	      (u1(istart,I2[i]-1,I33,1)-u1(istart,I2[i],I33,1)))>10.*REAL_EPSILON){
        u1(istart,I2[i],I33,0)=uprev[i](istart,I2[i],I33,0);
        u1(istart,I2[i],I33,1)=uprev[i](istart,I2[i],I33,1);
        u1(istart,I2[i]+1,I33,0)=uprev[i](istart,I2[i]+1,I33,0);
        u1(istart,I2[i]+1,I33,1)=uprev[i](istart,I2[i]+1,I33,1);
        u1(istart,I2[i]-1,I33,0)=uprev[i](istart,I2[i]-1,I33,0);
        u1(istart,I2[i]-1,I33,1)=uprev[i](istart,I2[i]-1,I33,1);
       }
     }
     if (gridBc(1,0)==2){
       where(((u1(istop,I2[i]+1,I33,0)-u1(istop,I2[i],I33,0))*
	      (u1(istop,I2[i]-1,I33,0)-u1(istop,I2[i],I33,0))+
	      (u1(istop,I2[i]+1,I33,1)-u1(istop,I2[i],I33,1))* 
	      (u1(istop,I2[i]-1,I33,1)-u1(istop,I2[i],I33,1)))>10.*REAL_EPSILON){
        u1(istop,I2[i],I33,0)=utmp[i](istop,I2[i],I33,0);
        u1(istop,I2[i],I33,1)=utmp[i](istop,I2[i],I33,1);
       }


       where(((u1(istop,I2[i]+1,I33,0)-u1(istop,I2[i],I33,0))*
	      (u1(istop,I2[i]-1,I33,0)-u1(istop,I2[i],I33,0))+
	      (u1(istop,I2[i]+1,I33,1)-u1(istop,I2[i],I33,1))* 
	      (u1(istop,I2[i]-1,I33,1)-u1(istop,I2[i],I33,1)))>10.*REAL_EPSILON){
        u1(istop,I2[i],I33,0)=uprev[i](istop,I2[i],I33,0);
        u1(istop,I2[i],I33,1)=uprev[i](istop,I2[i],I33,1);
        u1(istop,I2[i]+1,I33,0)=uprev[i](istop,I2[i]+1,I33,0);
        u1(istop,I2[i]+1,I33,1)=uprev[i](istop,I2[i]+1,I33,1);
        u1(istop,I2[i]-1,I33,0)=uprev[i](istop,I2[i]-1,I33,0);
        u1(istop,I2[i]-1,I33,1)=uprev[i](istop,I2[i]-1,I33,1);
       }
     }
  }
   VOYONS******/
    /*****VOYONS
    printf("\n variable=%i\t level=%i\n\n", j,i);
    //if (i==1){
     for (j1=jstart;j1<=jstop;j1++)
     for (i1=istart;i1<=istop;i1++)
      printf("After i=%i\t j=%i\t x=%g\t y=%g\t xp=%g\t yp=%g\n",i1,j1,u1(i1,j1,0,0),u1(i1,j1,0,1),uprev[i](i1,j1,0,0),uprev[i](i1,j1,0,1));
    //}
    VOYONS***/
  if ((j==1)&&(iter<3)) applyOrthogonalBoundaries(I1[i],I2[i],I3[i],1,i,u[i],uprev);
 }


 void multigrid2::
   solve(int i, int niter1, int imethod, realArray *up, int ichange){
   //Handles the different solution methods
    int subiter,j, j22,istart,istop,jstart,jstop;
    static int ich1=0;
    Index Itemp1,Itemp2, Jtemp1, Jtemp2;
    TridiagonalSolver tri;
    realArray a1, b1, c1, d1, e1, f1;
    //_k : derivative with respect to xi
    //-e : derivative with respect to eta
    //_d : derivative with respect to dzeta
    //
    // The equations to be solved are:
    //     1D case: x_kk + P*x_k = 0;
    //     2D case: g22*(X_kk+PX_k)+g11*(X_ee+QX_e)-2g12X_ke=0
    //              with g22=x_e*x_e+y_e*y_e
    //                   g11=x_k*x_k+y_k*y_k
    //                   g12=x_k*x_e+y_k*y_e

    switch (imethod){
     case 1:       //underelaxed Jacobi
       switch (ndimension){
         case 1:
           for (subiter=0; subiter<niter1;subiter++){
	     u[i](Iint1[i],Iint2[i],Iint3[i],0)=
	         (1-omega)*u[i](Iint1[i],Iint2[i],Iint3[i],0)+
	         omega*(u[i](Iint1[i]+1,Iint2[i],Iint3[i],0)+
		        u[i](Iint1[i]-1,Iint2[i],Iint3[i],0)+
	                dx[i]*(u[i](Iint1[i]+1,Iint2[i],Iint3[i],0)-
			       u[i](Iint1[i]-1,Iint2[i],Iint3[i],0))*
			Source[i](Iint1[i],Iint2[i],Iint3[i],0)/2.0 -
			dx[i]*dx[i]*RHS[i](Iint1[i],Iint2[i],Iint3[i],0))/2.0;
           }
           break;
  
	 case 2:
	   a1.redim(I1[i],I2[i],I3[i]);
	   b1.redim(I1[i],I2[i],I3[i]);
	   c1.redim(I1[i],I2[i],I3[i]);
	   if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
	       (gridBc(1,0)==3)||(gridBc(1,1)==3)){
             d1.redim(I1[i],I2[i],I3[i]);
	     e1.redim(I1[i],I2[i],I3[i]);
           }
	   for (subiter=0; subiter<niter1; subiter++){
	    for (j=0;j<ndimension;j++){
	     getSource(i,ichange);
	     find2Dcoefficients(a1,b1,c1,d1,e1,I1[i],I2[i],I3[i],i,j);
             if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic)
	      jacobi2Dsolve(a1,b1,c1,d1,e1,I1[i],Iint2[i],Iint3[i],
			   j,i,u[i],uprev,0);
             else if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic)
	      jacobi2Dsolve(a1,b1,c1,d1,e1,Iint1[i],I2[i],Iint3[i],
			   j,i,u[i],uprev,0);
             else 
	      jacobi2Dsolve(a1,b1,c1,d1,e1,Iint1[i],Iint2[i],Iint3[i],
			   j,i,u[i],uprev,0);
            }
           }
           break;

         default:
	   printf("Untreated condition\n");
	   exit(1);
       }
       break;

     case 2: // Red-Black
       switch (ndimension){
	 case 1:
	   Itemp1=Range(Iint1[i].getBase(), Iint1[i].getBound(), 2);
	   if ((Iint1[i].getBound()-1) < (Iint1[i].getBase()+1))
	     Itemp2=Iint1[i];
           else
             Itemp2=Range(Iint1[i].getBase()+1, Iint1[i].getBound()-1, 2);
           for (subiter=0; subiter<niter1;subiter++){
	     u[i](Itemp1,Iint2[i],Iint3[i],0) =
	         (1.0-omega)*u[i](Itemp1,Iint2[i],Iint3[i],0)+
	         omega*(u[i](Itemp1+1,Iint2[i],Iint3[i],0)+
		        u[i](Itemp1-1,Iint2[i],Iint3[i],0)+
		        dx[i]*(u[i](Itemp1+1,Iint2[i],Iint3[i],0)-
			       u[i](Itemp1-1,Iint2[i],Iint3[i],0))*
			Source[i](Itemp1,Iint2[i],Iint3[i],0)/2.0 -
			dx[i]*dx[i]*RHS[i](Itemp1,Iint2[i],Iint3[i],0))/2.0;
	     u[i](Itemp2,Iint2[i],Iint3[i],0) =
	         (1.0-omega)*u[i](Itemp2,Iint2[i],Iint3[i],0)+
	         omega*(u[i](Itemp2+1,Iint2[i],Iint3[i],0)+
		        u[i](Itemp2-1,Iint2[i],Iint3[i],0)+
		        dx[i]*(u[i](Itemp2+1,Iint2[i],Iint3[i],0)-
			       u[i](Itemp2-1,Iint2[i],Iint3[i],0))*
			Source[i](Itemp2,Iint2[i],Iint3[i],0)/2.0-
			dx[i]*dx[i]*RHS[i](Itemp2,Iint2[i],Iint3[i],0))/2.0;
           }
	   break;

	 case 2:
	   Itemp1=Range(Iint1[i].getBase(), Iint1[i].getBound(), 2);
	   Jtemp1=Range(Iint2[i].getBase(), Iint2[i].getBound(), 2);
	   if ((Iint1[i].getBound()-1) < (Iint1[i].getBase()+1))
						      Itemp2=Iint1[i];
           else Itemp2=Range(Iint1[i].getBase()+1,Iint1[i].getBound()-1,2);
	   if ((Iint2[i].getBound()-1) < (Iint2[i].getBase()+1))
	                                              Jtemp2=Iint2[i];
           else Jtemp2=Range(Iint2[i].getBase()+1,Iint2[i].getBound()-1,2);

           /****
	   if ((gridBc(0,0)==2)||(gridBc(0,1)==2)||
	       (gridBc(1,0)==2)||(gridBc(1,1)==2))
             applyOrthogonalBoundaries(I1[i],I2[i],I3[i],0,i,u[i],uprev);
           ****/


	   for (subiter=0; subiter<niter1; subiter++){
             if ((userMap->getIsPeriodic(0) == Mapping::functionPeriodic)||
		 (userMap->getIsPeriodic(1) == Mapping::functionPeriodic)){
	        a1.redim(I1[i],I2[i],I3[i]);
		b1.redim(I1[i],I2[i],I3[i]);
		c1.redim(I1[i],I2[i],I3[i]);
	        if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
	            (gridBc(1,0)==3)||(gridBc(1,1)==3)){
                  d1.redim(I1[i],I2[i],I3[i]);
	          e1.redim(I1[i],I2[i],I3[i]);
                }
	       for (j=0;j<ndimension;j++){
                 getSource(i,0);
	         find2Dcoefficients(a1,b1,c1,d1,e1,I1[i],I2[i],I3[i],i,j);
	         jacobi2Dsolve(a1,b1,c1,d1,e1,Iint1[i],Iint2[i],
			       Iint3[i],j,i,u[i],uprev,1);
               }
             }
	     a1.redim(Itemp1,Jtemp1,Iint3[i]);
	     b1.redim(Itemp1,Jtemp1,Iint3[i]);
	     c1.redim(Itemp1,Jtemp1,Iint3[i]);
	     if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
	         (gridBc(1,0)==3)||(gridBc(1,1)==3)){
               d1.redim(Itemp1,Jtemp1,Iint3[i]);
	       e1.redim(Itemp1,Jtemp1,Iint3[i]);
             }
	     for (j=0;j<ndimension;j++){
	       getSource(i,ichange);
	       find2Dcoefficients(a1,b1,c1,d1,e1,Itemp1,Jtemp1,Iint3[i],i,j);
	       jacobi2Dsolve(a1,b1,c1,d1,e1,Itemp1,Jtemp1,
			     Iint3[i],j,i,u[i],uprev,0);
             }
	     a1.redim(Itemp2,Jtemp2,Iint3[i]);
	     b1.redim(Itemp2,Jtemp2,Iint3[i]);
	     c1.redim(Itemp2,Jtemp2,Iint3[i]);
	     if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
		 (gridBc(1,0)==3)||(gridBc(1,1)==3)){
               d1.redim(Itemp2,Jtemp2,Iint3[i]);
	       e1.redim(Itemp2,Jtemp2,Iint3[i]);
             }
	     for (j=0;j<ndimension;j++){
	       getSource(i,0);
	       find2Dcoefficients(a1,b1,c1,d1,e1,Itemp2,Jtemp2,Iint3[i],i,j);
	       jacobi2Dsolve(a1,b1,c1,d1,e1,Itemp2,Jtemp2,
			     Iint3[i],j,i,u[i],uprev,0);
             }
	     a1.redim(Itemp2,Jtemp1,Iint3[i]);
	     b1.redim(Itemp2,Jtemp1,Iint3[i]);
	     c1.redim(Itemp2,Jtemp1,Iint3[i]);
	     if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
		 (gridBc(1,0)==3)||(gridBc(1,1)==3)){
	       d1.redim(Itemp2,Jtemp1,Iint3[i]);
	       e1.redim(Itemp2,Jtemp1,Iint3[i]);
	     }
	     for (j=0;j<ndimension;j++){
	       getSource(i,0);
	       find2Dcoefficients(a1,b1,c1,d1,e1,Itemp2,Jtemp1,Iint3[i],i,j);
	       jacobi2Dsolve(a1,b1,c1,d1,e1,Itemp2,Jtemp1,Iint3[i],
			     j,i,u[i],uprev,0);
             }
	     a1.redim(Itemp1,Jtemp2,Iint3[i]);
	     b1.redim(Itemp1,Jtemp2,Iint3[i]);
	     c1.redim(Itemp1,Jtemp2,Iint3[i]);
	     if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
		 (gridBc(1,0)==3)||(gridBc(1,1)==3)){
	       d1.redim(Itemp1,Jtemp2,Iint3[i]);
	       e1.redim(Itemp1,Jtemp2,Iint3[i]);
	     }
	     for (j=0;j<ndimension;j++){
	       getSource(i,0);
	       find2Dcoefficients(a1,b1,c1,d1,e1,Itemp1,Jtemp2,Iint3[i],i,j);
	       jacobi2Dsolve(a1,b1,c1,d1,e1,Itemp1,Jtemp2,Iint3[i],
					  j,i,u[i],uprev,0);
             }
             if ((userMap->getIsPeriodic(0) == Mapping::functionPeriodic)||
		 (userMap->getIsPeriodic(1) == Mapping::functionPeriodic)){
	        a1.redim(I1[i],I2[i],I3[i]);
		b1.redim(I1[i],I2[i],I3[i]);
		c1.redim(I1[i],I2[i],I3[i]);
	        if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
	            (gridBc(1,0)==3)||(gridBc(1,1)==3)){
                  d1.redim(I1[i],I2[i],I3[i]);
	          e1.redim(I1[i],I2[i],I3[i]);
                }
	       for (j=0;j<ndimension;j++){
                 getSource(i,0);
	         find2Dcoefficients(a1,b1,c1,d1,e1,I1[i],I2[i],I3[i],i,j);
	         jacobi2Dsolve(a1,b1,c1,d1,e1,Iint1[i],Iint2[i],
			       Iint3[i],j,i,u[i],uprev,1);
               }
             }
           }
           //if ((gridBc(0,0)==2)||(gridBc(0,1)==2)||
	       //(gridBc(1,0)==2)||(gridBc(1,1)==2))
             //applyOrthogonalBoundaries(I1[i],I2[i],I3[i],0,i,u[i],utmp);
	   break;

	 default:
	    printf("Untreated case\n");
	    exit(1);
       }
       break;

     case 3: //LineSolver
       switch (ndimension){
	 case 1:
           a1.redim(I1[i],Iint2[i],Iint3[i]);
           b1.redim(I1[i],Iint2[i],Iint3[i]);
           c1.redim(I1[i],Iint2[i],Iint3[i]);
           d1.redim(I1[i],Iint2[i],Iint3[i]);
	   for (subiter=0; subiter<niter1; subiter++){
	     a1=0.0, b1=0.0, c1=0.0, d1=0.0;
	     a1(Iint1[i],Iint2[i],Iint3[i])=1.0/(dx[i]*dx[i])-
	        1.0/(2.0*dx[i])*Source[i](Iint1[i],Iint2[i],Iint3[i],0);
	     b1(Iint1[i],Iint2[i],Iint3[i])=-2.0/(dx[i]*dx[i]);
	     c1(Iint1[i],Iint2[i],Iint3[i])=1.0/(dx[i]*dx[i])+
		1.0/(2.0*dx[i])*Source[i](Iint1[i],Iint2[i],Iint3[i],0);

	     // Have dirichlet BC;
	     b1(I1[i].getBase(),Iint2[i],Iint3[i])=1.0;
	     b1(I1[i].getBound(),Iint2[i],Iint3[i])=1.0;
	     d1(I1[i].getBase(),Iint2[i],Iint3[i])=
				 u[i](I1[i].getBase(),Iint2[i],Iint3[i],0);
	     d1(I1[i].getBound(),Iint2[i],Iint3[i])=
				 u[i](I1[i].getBound(),Iint2[i],Iint3[i],0);
	     tri.factor(a1,b1,c1,TridiagonalSolver::normal,axis1);
	     tri.solve(d1,I1[i],Iint2[i],Iint3[i]);
	     u[i](I1[i],Iint2[i],Iint3[i],0)=d1(I1[i],Iint2[i],Iint3[i]);
           }
	   break;

	 case 2:
	  {
	   int istart,istop,jstart,jstop;
	   realArray coeff1, coeff2, coeff3,coeff4,coeff5;
	   coeff1.redim(I1[i],I2[i],I3[i]);
	   coeff2.redim(I1[i],I2[i],I3[i]);
	   coeff3.redim(I1[i],I2[i],I3[i]);
	   for (subiter=0;subiter<niter1;subiter++){
	     // When there are no combined boundary
	     if (useBlockTridiag != 1){
	       //Sweep in the i direction
	       /*****TRY****/
	       istart=I1[i].getBase();
	       istop=I1[i].getBound();
	       if (userMap -> getIsPeriodic(1) != Mapping::functionPeriodic){
	         a1.redim(I1[i],Iint2[i],Iint3[i]);
	         b1.redim(I1[i],Iint2[i],Iint3[i]);
	         c1.redim(I1[i],Iint2[i],Iint3[i]);
	         d1.redim(I1[i],Iint2[i],Iint3[i]);
	       }
	       else{
	         a1.redim(I1[i],I2[i],Iint3[i]);
	         b1.redim(I1[i],I2[i],Iint3[i]);
	         c1.redim(I1[i],I2[i],Iint3[i]);
	         d1.redim(I1[i],I2[i],Iint3[i]);
	       }
	      for (j=0;j<ndimension;j++){
	        j22=(subiter+iter+j)%2;
	        find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
		               I1[i],I2[i],I3[i],i,j);
                getSource(i,ichange);
                if (userMap -> getIsPeriodic(1) != Mapping::functionPeriodic)
		  line2Dsolve(a1,b1,c1,d1,I1[i],Iint2[i],Iint3[i],coeff1,
			      coeff2,coeff3,coeff4,coeff5,Iint1[i],
			      Iint2[i],Iint3[i],j22,i,u[i],0,uprev);
                else
		  line2Dsolve(a1,b1,c1,d1,I1[i],I2[i],Iint3[i],coeff1,
			      coeff2,coeff3,coeff4,coeff5,Iint1[i],
			      Iint2[i],Iint3[i],j22,i,u[i],0,uprev);

               // The ghost points
               if (userMap->getIsPeriodic(0)!=Mapping::functionPeriodic){
	         u[i](istart-1,I2[i],I3[i],j)=2.*u[i](istart,I2[i],I3[i],j)-
				      u[i](istart+1,I2[i],I3[i],j);
                 u[i](istop+1,I2[i],I3[i],j)=2.*u[i](istop,I2[i],I3[i],j)-
				      u[i](istop-1,I2[i],I3[i],j);
               }
	       else{
                 u[i](istart-1,I2[i],I3[i],j)=u[i](istop-1,I2[i],I3[i],j);
                 u[i](istop+1,I2[i],I3[i],j)=u[i](istart+1,I2[i],I3[i],j);
	       }
              }
	      /*****TRY******/

              //printf("i=%i\t APRES LINESOLVE i SWEEP\n",i);
              //for (j1=-1;j1<=I2[i].getBound()+1;j1++)
		 //for (int i1=-1;i1<=I1[i].getBound()+1;i1++)
		   //printf("j1=%i\t i1=%i\t x=%g\t y=%g\n",j1,i1,
		   //u[i](i1,j1,0,0),u[i](i1,j1,0,1));
	       //Sweep in the j direction
	       /********TRY****/
	       if (userMap -> getIsPeriodic(0) != Mapping::functionPeriodic){
	         a1.redim(Iint1[i],I2[i],Iint3[i]);
	         b1.redim(Iint1[i],I2[i],Iint3[i]);
	         c1.redim(Iint1[i],I2[i],Iint3[i]);
	         d1.redim(Iint1[i],I2[i],Iint3[i]);
	       }
	       else {
	         a1.redim(I1[i],I2[i],Iint3[i]);
	         b1.redim(I1[i],I2[i],Iint3[i]);
	         c1.redim(I1[i],I2[i],Iint3[i]);
	         d1.redim(I1[i],I2[i],Iint3[i]);
	       }
	       jstart=I2[i].getBase();
	       jstop=I2[i].getBound();
	      for (j=0;j<ndimension;j++){
	        j22=(subiter+iter+j)%2;
	        find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
				   I1[i],I2[i],I3[i],i,j);
                getSource(i,0);
	        if (userMap -> getIsPeriodic(0) != Mapping::functionPeriodic)
                  line2Dsolve(a1,b1,c1,d1,Iint1[i],I2[i],Iint3[i],coeff1,
			      coeff2,coeff3,coeff4,coeff5,Iint1[i],
			      Iint2[i],Iint3[i],j22,i,u[i],1,uprev);
                else
                  line2Dsolve(a1,b1,c1,d1,I1[i],I2[i],Iint3[i],coeff1,
			      coeff2,coeff3,coeff4,coeff5,Iint1[i],
			      Iint2[i],Iint3[i],j22,i,u[i],1,uprev);
                
		//The ghost points
		if (userMap->getIsPeriodic(1)!=Mapping::functionPeriodic){
                  u[i](I1[i],jstart-1,I3[i],j)=2.*u[i](I1[i],jstart,I3[i],j)-
					    u[i](I1[i],jstart+1,I3[i],j);
                  u[i](I1[i],jstop+1,I3[i],j)=2.*u[i](I1[i],jstop,I3[i],j)-
					    u[i](I1[i],jstop-1,I3[i],j);
		}
		else{
                  u[i](I1[i],jstart-1,I3[i],j)=u[i](I1[i],jstop-1,I3[i],j);
                  u[i](I1[i],jstop+1,I3[i],j)=u[i](I1[i],jstart+1,I3[i],j);
		}
               }
	       /****TRY*****/
              //printf("i=%i\t APRES LINESOLVE j SWEEP\n",i);
		 //for (int i1=-1;i1<=I1[i].getBound()+1;i1++)
              //for (int j1=-1;j1<=I2[i].getBound()+1;j1++)
		   //printf("j1=%i\t i1=%i\t x=%g\t y=%g\n",j1,i1,
		   //u[i](i1,j1,0,0),u[i](i1,j1,0,1));
              //exit(1);
             }
	     else{
	      //if ((gridBc(0,0)!=3)&&(gridBc(1,0)!=3)&&
		 //(gridBc(0,1)!=3)&&(gridBc(1,1)!=3)){
		 //printf("Block tridiagonal used when at least\n"
			//"one of the grid boundary condition is\n"
			//"combined!!!   Exiting\n");
                 //exit(1);
              //}
	      // There are combined boundary conditions
	      /***TRY1*****/
	      // Sweep in the i direction
	       Index I22;
               getSource(i,0);
	       if (userMap -> getIsPeriodic(1) != Mapping::functionPeriodic)
                 I22=Iint2[i];
	       else
		 I22=I2[i];
	       a1.redim(2,2,I1[i],I22,Iint3[i]);
	       b1.redim(2,2,I1[i],I22,Iint3[i]);
	       c1.redim(2,2,I1[i],I22,Iint3[i]);
	       d1.redim(2,I1[i],I22,Iint3[i]);
	       
	       //if (i>0) RHS[i]=0.0;
	       find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
			   I1[i],I2[i],I3[i],i,j);
	       //printf("Avant solve \nlevel=%i\t sweep0\n",i);
               findPQ(u[i],coeff1,coeff2,I1[i],I2[i],I3[i],i,0,ichange);
               blockLine2Dsolve(a1,b1,c1,d1,I1[i],I22,Iint3[i],coeff1,
	                        coeff2,coeff3,coeff4,coeff5,Iint1[i],
	                        Iint2[i],Iint3[i],i,u[i],0,uprev);
	      //printf("APRES BLOCK \nlevel=%i\t sweep0\n",i);
	      //if (i !=0){
		//for (int i11=0;i11<=I1[i].getBound();i11++)
	         //for (int j11=0;j11<=I2[i].getBound();j11++)
		    //printf("i=%i\t j=%i\t x=%g\t y=%g\n",i11,
		    //j11, u[i](i11,j11,0,0), u[i](i11,j11,0,1));
              //}
	      /*****TRY1*****/
    if (TEST_RESIDUAL){
      realArray restemp1;
      printf("Apres isweep=0\t level=%i\n",i);
      int imax=0,jmax=0,i1max=0, j1max=0;
      real resmaxx=0.0, resmaxy=0.0;
      restemp1.redim(I1[i],I2[i],I3[i],ndimension);
      restemp1=0.0;
      getResidual(restemp1,i,uprev);
    //u[i](I1[i],Range(0,2),I3[i],Range(0,1)).display("U before Source");
    //Source[i](I1[i],Range(0,2),I3[i],Range(0,1)).display("SOURCE before source ");
    //restemp1(I1[i],Range(0,2),I3[i],Range(0,1)).display("restemp1 before source");
 
      for (int j1=0; j1<=I2[i].getBound(); j1++)
        for (int i1=0; i1<=I1[i].getBound(); i1++){
          if (fabs(restemp1(i1,j1,0,0))>resmaxx)
            imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
          if (fabs(restemp1(i1,j1,0,1))>resmaxy)
             i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
        //if (i==1){
         //printf("i=%i\t j=%i\t Xerror=%g\t Yerror=%g\t rhs=%g::%g\n",i1,j1,restemp1(i1,j1,0,0),restemp1(i1,j1,0,1),RHS[i](i1,j1,0,0),RHS[i](i1,j1,0,1));
        //}
        }
      printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
              imax,jmax,resmaxx,i1max,j1max,resmaxy);
  //restemp1(I1[i],Range(0,3),0,Range(0,1)).display("C'est restemp1");
  //u[i](I1[i],Range(0,3),0,Range(0,1)).display("C'est u");
    }

               /*****TRY1***/
	       //Sweep in the j direction
	       Index I11;
               getSource(i,0);
               if (userMap->getIsPeriodic(0)!=Mapping::functionPeriodic)
                 I11=Iint1[i];
               else 
		 I11=I1[i];
               a1.redim(2,2,I11,I2[i],Iint3[i]);
               b1.redim(2,2,I11,I2[i],Iint3[i]);
               c1.redim(2,2,I11,I2[i],Iint3[i]);
               d1.redim(2,I11,I2[i],Iint3[i]);
               
                //j22=(subiter+iter+j)%2;
                find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
                                   I1[i],I2[i],I3[i],i,j);
	        //printf("Avant solve \nlevel=%i\t sweep1\n",i);
		findPQ(u[i],coeff1,coeff2,I1[i],I2[i],I3[i],i,1,0);
	       //printf("\nlevel=%i\t sweep1\n",i);
	       //if (i !=0){
	       //for (int j11=0;j11<=I2[i].getBound();j11++)
		//for (int i11=0;i11<=I1[i].getBound();i11++)
		  //printf("i=%i\t j=%i\t p0=%g\t p1=%g\t p2=%g\t p3=%g\n",
		    //i11,j11,P[0](i11,j11,0), P[1](i11,j11,0), P[2](i11,j11,0), P[3](i11,j11,0));
		    //}
	       //printf("AVANT BLOCK \nlevel=%i\t sweep1\n",i);
	       //for (int j11=0;j11<=I2[i].getBound();j11++)
	        //for (int i11=0;i11<=I1[i].getBound();i11++)
		  //printf("i=%i\t j=%i\t x=%g\t y=%g\n",i11,
		  //j11, u[i](i11,j11,0,0), u[i](i11,j11,0,1));
               blockLine2Dsolve(a1,b1,c1,d1,I11,I2[i],Iint3[i],coeff1,
	                        coeff2,coeff3,coeff4,coeff5,Iint1[i],
	                        Iint2[i],Iint3[i],i,u[i],1,uprev);
	       //printf("APRES BLOCK \nlevel=%i\t sweep1\n",i);
	       //if (i !=0){
	       //for ( j11=0;j11<=I2[i].getBound();j11++)
	        //for (int i11=0;i11<=I1[i].getBound();i11++)
		  //printf("i=%i\t j=%i\t x=%g\t y=%g\n",i11,
		  //j11, u[i](i11,j11,0,0), u[i](i11,j11,0,1));
                //if (iter==1) exit(1);
               //}
	       /****TRY1****/
             }
           }
          }
	  break;

         default:
	   printf("Untreated case\n");
	   exit(1);
      }
      break;

   case 4:
      switch (ndimension){
	case 1:
	  printf("Cannot use Zebra for 1D problem. Exiting\n");
	  exit(1);
	  break;

	case 2:
	 {
	  int istart, istop, jstart, jstop;
	  realArray coeff1, coeff2, coeff3, coeff4, coeff5;
	  if (userMap->getIsPeriodic(1) != Mapping::functionPeriodic){
	    Jtemp1=Range(Iint2[i].getBase(), Iint2[i].getBound(), 2);
	    if ((Iint2[i].getBound()-1)<(Iint2[i].getBase()+1)) Jtemp2=Iint2[i];
	    else Jtemp2=Range(Iint2[i].getBase()+1,Iint2[i].getBound()-1,2);
	  }
	  else {
	    Jtemp1=Range(I2[i].getBase(), I2[i].getBound(), 2);
	    Jtemp2=Range(Iint2[i].getBase(),Iint2[i].getBound(),2);
	  }
	  if (userMap->getIsPeriodic(0) != Mapping::functionPeriodic){
            Itemp1=Range(Iint1[i].getBase(),Iint1[i].getBound(),2);
	    if ((Iint1[i].getBase()+1)>(Iint1[i].getBound()-1)) Itemp2=Iint1[i];
	    else Itemp2=Range(Iint1[i].getBase()+1,Iint1[i].getBound()-1,2);
	  }
	  else{
            Itemp1=Range(I1[i].getBase(),I1[i].getBound(),2);
	    Itemp2=Range(Iint1[i].getBase(),Iint1[i].getBound(),2);
	  }

          if (i==0){
           int isharp=0;
	   jstart=I2[i].getBase(), jstop=I2[i].getBound();
	   istart=I1[i].getBase(), istop=I1[i].getBound();
           for (int i1=I1[i].getBase();i1<=I1[i].getBound();i1++)
	    if (((u[i](i1+1,jstart,0,0)-u[i](i1,jstart,0,0))*
	        (u[i](i1-1,jstart,0,0)-u[i](i1,jstart,0,0))+
	        (u[i](i1+1,jstart,0,1)-u[i](i1,jstart,0,1))* 
	        (u[i](i1-1,jstart,0,1)-u[i](i1,jstart,0,1)))>10.*REAL_EPSILON){
	        isharp=1;
	        break;
	    }
           if (isharp==1)
	    for (i1=I1[i].getBase();i1<=I1[i].getBound();i1+=I1[i].getStride()){
	     if (((uprev[i](i1+1,jstart,0,0)-uprev[i](i1,jstart,0,0))*
	        (uprev[i](i1-1,jstart,0,0)-uprev[i](i1,jstart,0,0))>0.)||
	        ((uprev[i](i1+1,jstart,0,1)-uprev[i](i1,jstart,0,1))* 
	        (uprev[i](i1-1,jstart,0,1)-uprev[i](i1,jstart,0,1))>0.)||
	        (((utmp[i](i1+1,jstart,0,0)-u[i](i1,jstart,0,0))*
	        (utmp[i](i1-1,jstart,0,0)-u[i](i1,jstart,0,0)))+
	        ((utmp[i](i1+1,jstart,0,1)-u[i](i1,jstart,0,1))* 
	        (utmp[i](i1-1,jstart,0,1)-u[i](i1,jstart,0,1)))>0.)){
	      GRIDBC[2][i](i1)=1;
	      if (i1<istop) GRIDBC[2][i](i1+1)=1;
	     if (i1>istart) GRIDBC[2][i](i1-1)=1;
            }
           }
          }
	  if (useBlockTridiag!=1){
	    a1.redim(I1[i],I2[i],Iint3[i]);
	    b1.redim(I1[i],I2[i],Iint3[i]);
	    c1.redim(I1[i],I2[i],Iint3[i]);
	    d1.redim(I1[i],I2[i],Iint3[i]);
	    coeff1.redim(I1[i],I2[i],I3[i]);
	    coeff2.redim(I1[i],I2[i],I3[i]);
	    coeff3.redim(I1[i],I2[i],I3[i]);
	    for (subiter=0;subiter<niter1;subiter++){
	     // Sweep in the i direction
	     // First, the first group
	     a1(I1[i],Jtemp1,Iint3[i])=0.0; 
	     b1(I1[i],Jtemp1,Iint3[i])=0.0; 
	     c1(I1[i],Jtemp1,Iint3[i])=0.0; 
	     d1(I1[i],Jtemp1,Iint3[i])=0.0;
	     istart=I1[i].getBase();
	     istop=I1[i].getBound();
           for (j=0;j<ndimension;j++){
            getSource(i,ichange);
	    j22=(subiter+iter+j)%2;
	    find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
			       I1[i],Jtemp1,Iint3[i],i,j);
            line2Dsolve(a1,b1,c1,d1,I1[i],Jtemp1,Iint3[i],coeff1,
			coeff2,coeff3,coeff4,coeff5,Iint1[i],
			Jtemp1,Iint3[i],j22,i,u[i],0,uprev);

            // The boundary points
            if (userMap->getIsPeriodic(0)!=Mapping::functionPeriodic){
	      u[i](istart-1,I2[i],I3[i],j)=2.*u[i](istart,I2[i],I3[i],j)-
				       u[i](istart+1,I2[i],I3[i],j);
              u[i](istop+1,I2[i],I3[i],j)=2.*u[i](istop,I2[i],I3[i],j)-
				       u[i](istop-1,I2[i],I3[i],j);
            }
	    else{
              u[i](istart-1,I2[i],I3[i],j)=u[i](istop-1,I2[i],I3[i],j);
              u[i](istop+1,I2[i],I3[i],j)=u[i](istart+1,I2[i],I3[i],j);
	    }
           }

	   // The second group
	   a1(I1[i],Jtemp2,Iint3[i])=0.0; 
	   b1(I1[i],Jtemp2,Iint3[i])=0.0; 
	   c1(I1[i],Jtemp2,Iint3[i])=0.0; 
	   d1(I1[i],Jtemp2,Iint3[i])=0.0;
           for (j=0;j<ndimension;j++){
            getSource(i,0);
	    j22=(subiter+iter+j)%2;
	    find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
			       I1[i],Jtemp2,Iint3[i],i,j);
            line2Dsolve(a1,b1,c1,d1,I1[i],Jtemp2,Iint3[i],coeff1,
			coeff2,coeff3,coeff4,coeff5,Iint1[i],Jtemp2,
			Iint3[i],j22,i,u[i],0,uprev);

            // The boundary points
            if (userMap->getIsPeriodic(0)!=Mapping::functionPeriodic){
	      u[i](istart-1,I2[i],I3[i],j)=2.*u[i](istart,I2[i],I3[i],j)-
				       u[i](istart+1,I2[i],I3[i],j);
              u[i](istop+1,I2[i],I3[i],j)=2.*u[i](istop,I2[i],I3[i],j)-
				       u[i](istop-1,I2[i],I3[i],j);
            }
	    else{
              u[i](istart-1,I2[i],I3[i],j)=u[i](istop-1,I2[i],I3[i],j);
              u[i](istop+1,I2[i],I3[i],j)=u[i](istart+1,I2[i],I3[i],j);
	    }
           }

              //printf("BEFORE LINESOLVE\n");
              //for (int j1=-1;j1<=I2[i].getBound()+1;j1++)
		 //for (int i1=-1;i1<=I1[i].getBound()+1;i1++)
		   //printf("j1=%i\t i1=%i\t x=%g\t y=%g\n",j1,i1,
		   //u[i](i1,j1,0,0),u[i](i1,j1,0,1));
	   // Sweep in the j direction
	   // The first group
	   jstart=I2[i].getBase();
	   jstop=I2[i].getBound();
           for (j=0;j<ndimension;j++){
            getSource(i,0);
	    j22=(subiter+iter+j)%2;
	    find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
			       Itemp1,I2[i],Iint3[i],i,j);
            line2Dsolve(a1,b1,c1,d1,Itemp1,I2[i],Iint3[i],coeff1,
			coeff2,coeff3,coeff4,coeff5,Itemp1,Iint2[i],
			Iint3[i],j22,i,u[i],1,uprev);
                
	    //The Boundary points
	    if (userMap->getIsPeriodic(1)!=Mapping::functionPeriodic){
              u[i](I1[i],jstart-1,I3[i],j)=2.*u[i](I1[i],jstart,I3[i],j)-
				       u[i](I1[i],jstart+1,I3[i],j);
              u[i](I1[i],jstop+1,I3[i],j)=2.*u[i](I1[i],jstop,I3[i],j)-
				       u[i](I1[i],jstop-1,I3[i],j);
	    }
	    else{
              u[i](I1[i],jstart-1,I3[i],j)=u[i](I1[i],jstop-1,I3[i],j);
              u[i](I1[i],jstop+1,I3[i],j)=u[i](I1[i],jstart+1,I3[i],j);
	    }
           }

              //printf("APRES 1GROUPE LINESOLVE jsweep\n");
		 //for (int i1=-1;i1<=I1[i].getBound()+1;i1++)
              //for (int j1=-1;j1<=I2[i].getBound()+1;j1++)
		   //printf("j1=%i\t i1=%i\t x=%g\t y=%g\n",j1,i1,
		   //u[i](i1,j1,0,0),u[i](i1,j1,0,1));
              //exit(1);
	   // The other group
           for (j=0;j<ndimension;j++){
            getSource(i,0);
	    j22=(subiter+iter+j)%2;
	    find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
			       Itemp2,I2[i],Iint3[i],i,j);
            line2Dsolve(a1,b1,c1,d1,Itemp2,I2[i],Iint3[i],coeff1,
			coeff2,coeff3,coeff4,coeff5,Itemp2,Iint2[i],
			Iint3[i],j22,i,u[i],1,uprev);
                
	    //The Boundary points
	    if (userMap->getIsPeriodic(1)!=Mapping::functionPeriodic){
              u[i](I1[i],jstart-1,I3[i],j)=2.*u[i](I1[i],jstart,I3[i],j)-
				        u[i](I1[i],jstart+1,I3[i],j);
              u[i](I1[i],jstop+1,I3[i],j)=2.*u[i](I1[i],jstop,I3[i],j)-
				       u[i](I1[i],jstop-1,I3[i],j);
	    }
	    else{
              u[i](I1[i],jstart-1,I3[i],j)=u[i](I1[i],jstop-1,I3[i],j);
              u[i](I1[i],jstop+1,I3[i],j)=u[i](I1[i],jstart+1,I3[i],j);
	    }
           }
              //printf("APRES 2GROUPE LINESOLVE isweep\n");
		 //for (int i1=0;i1<=I1[i].getBound();i1++)
              //for (int j1=0;j1<=I2[i].getBound();j1++)
		   //printf("j1=%i\t i1=%i\t x=%g\t y=%g\n",j1,i1,
		   //u[i](i1,j1,0,0),u[i](i1,j1,0,1));
            //exit(1);
           }
	  }
	  else {
	    Index Rtmp=Range(0,1);
	    for (subiter=0;subiter<niter1;subiter++){
	     // Sweep in the i direction
	     // First, the first group
	     a1.redim(2,2,I1[i],I2[i],Iint3[i]);
	     b1.redim(2,2,I1[i],I2[i],Iint3[i]);
	     c1.redim(2,2,I1[i],I2[i],Iint3[i]);
	     d1.redim(2,I1[i],I2[i],Iint3[i]);
	     coeff1.redim(I1[i],I2[i],Iint3[i]);
	     coeff2.redim(I1[i],I2[i],Iint3[i]);
	     coeff3.redim(I1[i],I2[i],Iint3[i]);

	     a1(Rtmp,Rtmp,I1[i],Jtemp1,Iint3[i])=0.0; 
	     b1(Rtmp,Rtmp,I1[i],Jtemp1,Iint3[i])=0.0; 
	     c1(Rtmp,Rtmp,I1[i],Jtemp1,Iint3[i])=0.0; 
	     d1(Rtmp,I1[i],Jtemp2,Iint3[i])=0.0;
           
            getSource(i,0);
	    find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
			       I1[i],I2[i],Iint3[i],i,j);
            findPQ(u[i],coeff1,coeff2,I1[i],I2[i],I3[i],i,0,ichange);
            blockLine2Dsolve(a1,b1,c1,d1,I1[i],Jtemp1,Iint3[i],coeff1,
	                     coeff2,coeff3,coeff4,coeff5,Iint1[i],
	                     Iint2[i],Iint3[i],i,u[i],0,uprev);

	   // The second group
	   a1(Rtmp,Rtmp,I1[i],Jtemp2,Iint3[i])=0.0; 
	   b1(Rtmp,Rtmp,I1[i],Jtemp2,Iint3[i])=0.0; 
	   c1(Rtmp,Rtmp,I1[i],Jtemp2,Iint3[i])=0.0; 
	   d1(Rtmp,I1[i],Jtemp2,Iint3[i])=0.0;

            blockLine2Dsolve(a1,b1,c1,d1,I1[i],Jtemp2,Iint3[i],coeff1,
	                     coeff2,coeff3,coeff4,coeff5,Iint1[i],
	                     Iint2[i],Iint3[i],i,u[i],0,uprev);

	   // Sweep in the j direction
	   // The first group
	   a1(Rtmp,Rtmp,Itemp1,I2[i],Iint3[i])=0.0; 
	   b1(Rtmp,Rtmp,Itemp1,I2[i],Iint3[i])=0.0; 
	   c1(Rtmp,Rtmp,Itemp1,I2[i],Iint3[i])=0.0; 
	   d1(Rtmp,Itemp1,I2[i],Iint3[i])=0.0;
          
           getSource(i,0);
	   j22=(subiter+iter+j)%2;
	   find2Dcoefficients(coeff1,coeff2,coeff3,coeff4,coeff5,
			       I1[i],I2[i],Iint3[i],i,j);
           findPQ(u[i],coeff1,coeff2,I1[i],I2[i],I3[i],i,1,ichange);
           blockLine2Dsolve(a1,b1,c1,d1,Itemp1,I2[i],Iint3[i],coeff1,
	                     coeff2,coeff3,coeff4,coeff5,Iint1[i],
	                     Iint2[i],Iint3[i],i,u[i],1,uprev);

	   // The other group
	   a1(Rtmp,Rtmp,Itemp2,I2[i],Iint3[i])=0.0; 
	   b1(Rtmp,Rtmp,Itemp2,I2[i],Iint3[i])=0.0; 
	   c1(Rtmp,Rtmp,Itemp2,I2[i],Iint3[i])=0.0; 
	   d1(Rtmp,Itemp2,I2[i],Iint3[i])=0.0;
           
           blockLine2Dsolve(a1,b1,c1,d1,Itemp2,I2[i],Iint3[i],coeff1,
	                     coeff2,coeff3,coeff4,coeff5,Iint1[i],
	                     Iint2[i],Iint3[i],i,u[i],1,uprev);
	  }
         }
	}
	break;

        default:
	  printf("Untreated case. Exiting\n");
	  exit(1);
        }
	break;

    default:
       printf("Unknown method. Exiting\n");
       exit(1);
    }
//for(i1=I1[3].getBase(); i1<=I1[3].getBound(); i1+=I1[3].getStride())
 //for (int j1=I2[3].getBase(); j1<= I2[3].getBound(); j1+=I2[3].getStride())
  //printf("after i=%i\t j=%i\t u[3]=%g\t u[3]=%g\n",i1,j1,u[3](i1,j1,0,0),
//	 u[3](i1,j1,0,1));
//	 exit(1);
  }

 void multigrid2::
   Interpolate(int i1, int i2, realArray &u1, realArray &u2, int jmax){
     Index Idouble, I, Jdouble, J;
     int j;

    switch (ndimension){
      case 1:
	 for (j=0;j<jmax;j++){
           if ((i1>i2)&&(I1[i1].getBound()<I1[i2].getBound())){
             // interpolation from coarser to finer
             Idouble=Range(I1[i2].getBase(), I1[i2].getBound(),2);
             u2(Idouble,I2[i2],I3[i2],j)=u1(I1[i1],I2[i1],I3[i1],j);
             Idouble=Range(I1[i2].getBase(), I1[i2].getBound()-2,2);
             I=Range(I1[i1].getBase(), I1[i1].getBound()-1);
             u2(Idouble+1,I2[i2],I3[i2],j)=0.5*(u1(I,I2[i1],I3[i1],j)+
					    u1(I+1,I2[i1],I3[i1],j));
           }
           else if ((i1<i2)&&(I1[i1].getBound()>I1[i2].getBound())){
             // interpolation from finer to coarser
             Idouble=Range(2,I1[i1].getBound()-2,2);
             I=Range(1,I1[i2].getBound()-1);
             u2(I1[i2].getBase(),I2[i2],I3[i2],j)=
			    u1(I1[i1].getBase(),I2[i1],I3[i1],j);
             u2(I1[i2].getBound(),I2[i2],I3[i2],j)=
			    u1(I1[i1].getBound(),I2[i1],I3[i1],j);
             u2(I,I2[i2],I3[i2],j)=0.25*(u1(Idouble-1,I2[i1],I3[i1],j)+
	         2.0*u1(Idouble,I2[i1],I3[i1],j)+u1(Idouble+1,I2[i1],I3[i1],j));
           }
           else fprintf(stdout,"\n Wrong interpolation order \n"), exit(1);
         }
	 break;

      case 2:
	 for (j=0;j<jmax;j++){
	   if ((i1>i2)&&(I1[i1].getBound()<I1[i2].getBound())&&
	     (I2[i1].getBound()<I2[i2].getBound())){
	     // Interpolate from coarser to finer
	     // start with the 2i,2j points
	     Idouble=Range(I1[i2].getBase(), I1[i2].getBound(),2);
	     Jdouble=Range(I2[i2].getBase(), I2[i2].getBound(),2);
	     u2(Idouble,Jdouble,I3[i2],j)=u1(I1[i1],I2[i1],I3[i1],j);

	     // i is even (2i) and j is odd (2j+1)
	     Idouble=Range(I1[i2].getBase(), I1[i2].getBound(),2);
             Jdouble=Range(I2[i2].getBase()+1, I2[i2].getBound()-1,2);
	     I=Range(I1[i1].getBase(), I1[i1].getBound());
	     J=Range(I2[i1].getBase(), I2[i1].getBound()-1);
	     u2(Idouble,Jdouble,I3[i2],j)=
			  0.5*(u1(I,J,I3[i1],j)+u1(I,J+1,I3[i1],j));

             // i is odd (2i+1) and j is even (2j)
	     Idouble=Range(I1[i2].getBase()+1, I1[i2].getBound()-1,2);
	     Jdouble=Range(I2[i2].getBase(),I2[i2].getBound(),2);
	     I=Range(I1[i1].getBase(), I1[i1].getBound()-1);
	     J=Range(I2[i1].getBase(), I2[i1].getBound());
	     u2(Idouble,Jdouble,I3[i2],j)=
			  0.5*(u1(I,J,I3[i1],j)+u1(I+1,J,I3[i1],j));

	     // i is odd (2i+1) and j is odd (2j+1)
	     Idouble=Range(I1[i2].getBase()+1, I1[i2].getBound()-1,2);
	     Jdouble=Range(I2[i2].getBase()+1, I2[i2].getBound()-1,2);
	     I=Range(I1[i1].getBase(), I1[i1].getBound()-1);
	     J=Range(I2[i1].getBase(), I2[i1].getBound()-1);
	     u2(Idouble,Jdouble,I3[i2],j)=
	                 0.25*(u1(I,J,I3[i1],j)+u1(I+1,J,I3[i1],j)+
			       u1(I,J+1,I3[i1],j)+u1(I+1,J+1,I3[i1],j));
           }
           else if ((i1<i2)&&(I1[i1].getBound()>I1[i2].getBound())&&
		  (I2[i1].getBound()>I2[i2].getBound())){
             // interpolation from finer to coarser
	     // Interior point use 9 points full weighted
             Idouble=Range(I1[i1].getBase()+2, I1[i1].getBound()-2,2);
	     Jdouble=Range(I2[i1].getBase()+2, I2[i1].getBound()-2,2);
	     I=Range(I1[i2].getBase()+1, I1[i2].getBound()-1);
	     J=Range(I2[i2].getBase()+1, I2[i2].getBound()-1);
	     u2(I,J,I3[i2],j)=(1.0/16.0)*(u1(Idouble-1, Jdouble-1, I3[i1],j)+
				    2.0*u1(Idouble,   Jdouble-1, I3[i1],j)+
					u1(Idouble+1, Jdouble-1, I3[i1],j)+
                                    2.0*u1(Idouble-1, Jdouble,   I3[i1],j)+
				    4.0*u1(Idouble,   Jdouble,   I3[i1],j)+
				    2.0*u1(Idouble+1, Jdouble,   I3[i1],j)+
					u1(Idouble-1, Jdouble+1, I3[i1],j)+
                                    2.0*u1(Idouble,   Jdouble+1, I3[i1],j)+
					u1(Idouble+1, Jdouble+1, I3[i1],j));

             //Use also full weighting on the boundary points
	     //and same values in the 4 corners
	     Idouble=Range(I1[i1].getBase()+2,I1[i1].getBound()-2,2);
	     Jdouble=Range(I2[i1].getBase()+2,I2[i1].getBound()-2,2);
	     I=Range(I1[i2].getBase()+1, I1[i2].getBound()-1);
	     J=Range(I2[i2].getBase()+1, I2[i2].getBound()-1);
	     u2(I,I2[i2].getBase(),I3[i2],j)=
				0.25*(   u1(Idouble-1,I2[i1].getBase(),I3[i1],j)+
				     2.0*u1(Idouble,  I2[i1].getBase(),I3[i1],j)+
					 u1(Idouble+1,I2[i1].getBase(),I3[i1],j));
	     u2(I,I2[i2].getBound(),I3[i2],j)=
				0.25*(   u1(Idouble-1,I2[i1].getBound(),I3[i1],j)+
				     2.0*u1(Idouble,  I2[i1].getBound(),I3[i1],j)+
					 u1(Idouble+1,I2[i1].getBound(),I3[i1],j));
	     u2(I1[i2].getBase(),J,I3[i2],j)=
				0.25*(   u1(I1[i1].getBase(),Jdouble-1,I3[i1],j)+
				     2.0*u1(I1[i1].getBase(),Jdouble,  I3[i1],j)+
					 u1(I1[i1].getBase(),Jdouble+1,I3[i1],j));
	     u2(I1[i2].getBound(),J,I3[i2],j)=
				0.25*(   u1(I1[i1].getBound(),Jdouble-1,I3[i1],j)+
				     2.0*u1(I1[i1].getBound(),Jdouble,  I3[i1],j)+
					 u1(I1[i1].getBound(),Jdouble+1,I3[i1],j));
             u2(I1[i2].getBase(), I2[i2].getBase(),I3[i2],j)=
			u1(I1[i1].getBase(),I2[i1].getBase(),I3[i1],j);
             u2(I1[i2].getBound(), I2[i2].getBase(),I3[i2],j)=
			u1(I1[i1].getBound(),I2[i1].getBase(),I3[i1],j);
             u2(I1[i2].getBase(), I2[i2].getBound(),I3[i2],j)=
			u1(I1[i1].getBase(),I2[i1].getBound(),I3[i1],j);
             u2(I1[i2].getBound(), I2[i2].getBound(),I3[i2],j)=
			u1(I1[i1].getBound(),I2[i1].getBound(),I3[i1],j);
           }
           else fprintf(stdout,"\n Wrong interpolation order \n"), exit(1);
         }
	 break;
       
       default:
        printf("Untreated condition\n");
	exit(1);
      }
  }

 void multigrid2::
   multigridVcycle(int i, int ifiner, int icoarser,realArray *up){
    realArray restemp1,restemp2;
    int j;
    if (i==ifiner) RHS[i]=0.0;
    if (TEST_RESIDUAL){
      printf("Before solve1\t level=%i\n",i);
      int imax=0,jmax=0,i1max=0, j1max=0;
      real resmaxx=0.0, resmaxy=0.0;
      restemp1.redim(I1[i],I2[i],I3[i],ndimension);
      restemp1=0.0;
      getResidual(restemp1,i,uprev);
    //u[i](I1[i],Range(0,2),I3[i],Range(0,1)).display("U before Source");
    //Source[i](I1[i],Range(0,2),I3[i],Range(0,1)).display("SOURCE before source");
    //restemp1(I1[i],Range(0,2),I3[i],Range(0,1)).display("restemp1 before source");

      for (int j1=0; j1<=I2[i].getBound(); j1++)
        for (int i1=0; i1<=I1[i].getBound(); i1++){
          if (fabs(restemp1(i1,j1,0,0))>resmaxx) 
	    imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
          if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	     i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
        //if (i==1){
	 //printf("i=%i\t j=%i\t Xerror=%g\t Yerror=%g\t Xy=%g::%g\n",i1,j1,restemp1(i1,j1,0,0),restemp1(i1,j1,0,1),u[i](i1,j1,0,0),u[i](i1,j1,0,1));
	//}
        }
      printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
	      imax,jmax,resmaxx,i1max,j1max,resmaxy);
  //restemp1(I1[i],Range(0,3),0,Range(0,1)).display("C'est restemp1");
  //u[i](I1[i],Range(0,3),0,Range(0,1)).display("C'est u");
    }
    //if (i==0)
      //project_u(u[0],userMap,I1[0],I2[0],I3[0],Range(0,ndimension-1));
    if (i==icoarser) solve(i,niter,smoothingMethod,uprev,1);
    else solve(i,niter,smoothingMethod,uprev,1);

    if (TEST_RESIDUAL){
      printf("Apres solve1\t level=%i\n",i);
      int imax=0,jmax=0,i1max=0, j1max=0;
      real resmaxx=0.0, resmaxy=0.0;
      restemp1.redim(I1[i],I2[i],I3[i],ndimension);
      restemp1=0.0;
      getResidual(restemp1,i,uprev);
      for (int j1=0; j1<=I2[i].getBound(); j1++)
        for (int i1=0; i1<=I1[i].getBound(); i1++){
          if (fabs(restemp1(i1,j1,0,0))>resmaxx) 
	    imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
          if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	     i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
        //if (i==1){
	//printf("i1=%i\t j1=%i\t Xerror=%g\t Yerror=%g\t rhs=%g::%g\n",i1,j1,
	//restemp1(i1,j1,0,0),restemp1(i1,j1,0,1),u[i](i1,j1,0,0),u[i](i1,j1,0,1));
	//}
        }
      printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
	      imax,jmax,resmaxx,i1max,j1max,resmaxy);
	//if (i==1) exit(1);
    }
    if (i != icoarser){
      i++;
      restemp1.redim(I1[i-1],I2[i-1],I3[i-1],ndimension);
      restemp1=0.0;
      w[i]=0.0;
      getResidual(restemp1,i-1,uprev);
      Interpolate(i-1,i,restemp1,RHS[i],2);
      Interpolate(i-1,i,u[i-1],w[i],2);
      for (j=0;j<ndimension;j++)
        u[i](I1[i],I2[i],I3[i],j)=w[i](I1[i],I2[i],I3[i],j);
      updateRHS(i,uprev);
      multigridVcycle(i,ifiner,icoarser,uprev);

      restemp1.redim(I1[i-1],I2[i-1],I3[i-1],ndimension);
      restemp1=0.0;
      restemp2.redim(I1[i],I2[i],I3[i],ndimension);
      for (j=0;j<ndimension;j++)
	restemp2(I1[i],I2[i],I3[i],j) = u[i](I1[i],I2[i],I3[i],j) - 
					w[i](I1[i],I2[i],I3[i],j);
      Interpolate(i,i-1,restemp2,restemp1,2);
      for (j=0;j<ndimension;j++)
        u[i-1](I1[i-1],I2[i-1],I3[i-1],j) +=  
	                restemp1(I1[i-1],I2[i-1],I3[i-1],j);

       //printf("\nAPRES UP INTERPOLATION\n");
       //for (int j1=0;j1<=I2[i-1].getBound();j1++)
	//for (int i1=0; i1<=I1[i-1].getBound(); i1++)
	 //printf("i=%i\t j=%i\t x=%g\t y=%g\t correctX=%g\t correctY=%g\n", i1,j1,u[i-1](i1,j1,0,0), u[i-1](i1,j1,0,1), restemp1(i1,j1,0,0),restemp1(i1,j1,0,1));
      i--;
    }
    if (TEST_RESIDUAL){
      printf("Avant solve2\t level=%i\n",i);
      int imax=0,jmax=0,i1max=0, j1max=0;
      real resmaxx=0.0, resmaxy=0.0;
      restemp1.redim(I1[i],I2[i],I3[i],ndimension);
      restemp1=0.0;
      getResidual(restemp1,i,uprev);
      for (int j1=0; j1<=I2[i].getBound(); j1++)
        for (int i1=0; i1<=I1[i].getBound(); i1++){
          if (fabs(restemp1(i1,j1,0,0))>resmaxx) 
	    imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
          if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	     i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
        }
      printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
	      imax,jmax,resmaxx,i1max,j1max,resmaxy);
       //printf("\nAPRES UP INTERPOLATION\n");
       //for (j1=0;j1<=I2[i].getBound();j1++)
	//for (int i1=0; i1<=I1[i].getBound(); i1++){
	 //if (j1<=2) printf("i=%i\t j=%i\t P=%g\t Q=%g\t P11=%g\t P12=%g\t Q11=%g\t Q12=%g\n", i1,j1,P[2][i](i1,j1,0,0), Q[2][i](i1,j1,0,1), P1[2](i1,j1,0,0),P1[2](i1,j1,0,1),Q1[2](i1,j1,0,0),Q1[2](i1,j1,0,1));
	 //else printf("i=%i\t j=%i\t P=%g\t Q=%g\n", i1,j1,P[2][i](i1,j1,0,0), Q[2][i](i1,j1,0,1));
	 //}
    }
    //if (i==0)
      //project_u(u[0],userMap,I1[0],I2[0],I3[0],Range(0,ndimension-1));
    /*****TRY***/
    if (i==icoarser) solve(i,niter,smoothingMethod,uprev,0);
    else solve(i,niter,smoothingMethod,uprev,0);
    /***TRY****/
    if (TEST_RESIDUAL){
      printf("Apres solve2\t level=%i\n",i);
      int imax=0,jmax=0,i1max=0, j1max=0;
      real resmaxx=0.0, resmaxy=0.0;
      restemp1.redim(I1[i],I2[i],I3[i],ndimension);
      restemp1=0.0;
      getResidual(restemp1,i,uprev);
      for (int j1=0; j1<=I2[i].getBound(); j1++)
        for (int i1=0; i1<=I1[i].getBound(); i1++){
          if (fabs(restemp1(i1,j1,0,0))>resmaxx) 
	    imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
          if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	     i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
        //if (i==1){
	//if (iter>6)
	 //printf("i=%i\t j=%i\t Xerror=%g\t Yerror=%g\t X=%g\t y=%g\n",i1,j1,restemp1(i1,j1,0,0),restemp1(i1,j1,0,1),u[i](i1,j1,0,0), u[i](i1,j1,0,1));
	//}
        }
      printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
	      imax,jmax,resmaxx,i1max,j1max,resmaxy);
    }
  }

 void multigrid2::
  project_u(realMappedGridFunction &u1,Index I11, Index J11, 
	    Index K11, Index Rr){
   realArray r, x;
   int isize, jsize, ksize;
   x.redim(I11,J11,K11,Rr);
   r.redim(I11,J11,K11,Rr);
   x(I11,J11,K11,Rr)=u1(I11,J11,K11,Rr);

   isize=(I11.getBound()-I11.getBase())/(I11.getStride())+1;
   jsize=(J11.getBound()-J11.getBase())/(J11.getStride())+1;
   ksize=(K11.getBound()-K11.getBase())/(K11.getStride())+1;
   x.reshape(isize*jsize*ksize,Rr);
   r.reshape(isize*jsize*ksize,Rr);
   userMap->inverseMap(x,r);
   r.reshape(I11,J11,K11,Rr);
   r(I11,J11.getBase(),K11,axis2)=0.;
   r(I11,J11.getBound(),K11,axis2)=1.;
   r(I11.getBase(),J11,K11,axis1)=0.;
   r(I11.getBound(),J11,K11,axis1)=1.;
   r.reshape(isize*jsize*ksize,Rr);
   userMap->map(r,x);
   x.reshape(I11,J11,K11,Rr);
   u1(I11,J11,K11,Rr)=x(I11,J11,K11,Rr);
  }

 void multigrid2::
  applyMultigrid(const realArray &u0, Index Igrid, Index Jgrid,
		 Index Kgrid, Index Rr){
   //Main routine to prepare computational space
   //and call solution routines
   int i, j,*idimension,*jdimension,*kdimension;
    idimension=new int[nlevel];
    if (ndimension>1) jdimension=new int[nlevel];
    if (ndimension>2) kdimension=new int[nlevel];
    real resid1=0.0, residprev1=0.0;
    real resid2=0.0, residprev2=0.0;
    //realArray residtemp1,residtemp2, *uprev;
    //uprev= new realArray[nlevel];
    realArray residtemp1,residtemp2;

    Range all;
      // Initialize working arrays
    for (i=0;i<nlevel;i++){
     if (i==0){
       idimension[i]=dim[0];
       if (ndimension>1) jdimension[i]=dim[1];
       if (ndimension>2) kdimension[i]=dim[2];
     }
     else {
       idimension[i]=dim[0]/(pow(2,i))+1;
       if (ndimension>1) jdimension[i]=dim[1]/(pow(2,i))+1;
       if (ndimension>2) kdimension[i]=dim[2]/(pow(2,i))+1;
     }
     if (ndimension==1) grid.setGridDimensions(axis1,idimension[i]);
     if (ndimension==2) {
       sq_grid.setGridDimensions(axis1,idimension[i]);
       sq_grid.setGridDimensions(axis2,jdimension[i]);
     }
     if (ndimension==1) mg[i]=MappedGrid(grid);
     if (ndimension==2) mg[i]=MappedGrid(sq_grid);
   
     mg[i].update();
     u[i]=realMappedGridFunction(mg[i],all,all,all,ndimension);
     u[i]=0.0;
     getIndex(mg[i].dimension(),Ig1[i],Ig2[i],Ig3[i]);
     if (Ig1[i].getBase() == -1){
       I1[i]=Range(Ig1[i].getBase()+1, Ig1[i].getBound()-1,
             Ig1[i].getStride());
       Iint1[i]=Range(Ig1[i].getBase()+2, Ig1[i].getBound()-2,
                Ig1[i].getStride());
     }
     else I1[i]=Ig1[i], Iint1[i]=Ig1[i];

     if (Ig2[i].getBase() == -1){
       I2[i]=Range(Ig2[i].getBase()+1, Ig2[i].getBound()-1,
             Ig2[i].getStride());
       Iint2[i]=Range(Ig2[i].getBase()+2, Ig2[i].getBound()-2,
                Ig2[i].getStride());
     }
     else I2[i]=Ig2[i], Iint2[i]=Ig2[i];

     if (Ig3[i].getBase() == -1){
       I3[i]=Range(Ig3[i].getBase()+1, Ig3[i].getBound()-1,
             Ig3[i].getStride());
       Iint3[i]=Range(Ig3[i].getBase()+2, Ig3[i].getBound()-2,
                Ig3[i].getStride());
     }
     else I3[i]=Ig3[i], Iint3[i]=Ig3[i];
     Source[i].redim(I1[i],I2[i],I3[i],ndimension);
     utmp[i].redim(Ig1[i],Ig2[i],Ig3[i],ndimension);
     Source[i]=0.0;
     utmp[i]=0.0;
     //if ((gridBc(0,0)==3)||(gridBc(1,0)==3)||(gridBc(0,1)==3)||
	 //(gridBc(1,1)==3)){
     if (useBlockTridiag==1){
      P[0][i].redim(I1[i],I2[i],I3[i]);
      Q[0][i].redim(I1[i],I2[i],I3[i]);
      P[1][i].redim(I1[i],I2[i],I3[i]);
      Q[1][i].redim(I1[i],I2[i],I3[i]);
      P[2][i].redim(I1[i],I2[i],I3[i]);
      Q[2][i].redim(I1[i],I2[i],I3[i]);
      P[3][i].redim(I1[i],I2[i],I3[i]);
      Q[3][i].redim(I1[i],I2[i],I3[i]);
      P[0][i]=0.,P[1][i]=0.,P[2][i]=0.,P[3][i]=0.;
      Q[0][i]=0.,Q[1][i]=0.,Q[2][i]=0.,Q[3][i]=0.;
     }
     if (gridBc(0,0)==3){
       Xcc0[0][i].redim(Range(I1[i].getBase(),I1[i].getBase()),
			I2[i],1,Range(0,ndimension-1));
       Xcc0[0][i]=0.0;
       CoeffInterp[0][i].redim(I1[i],I2[i],I3[i]);
       CoeffInterp[0][i]=0.0;
     }
     if (gridBc(1,0)==3){
       Xcc0[1][i].redim(Range(I1[i].getBase(),I1[i].getBase()),
			I2[i],1,Range(0,ndimension-1));
       Xcc0[1][i]=0.0;
       CoeffInterp[1][i].redim(I1[i],I2[i],I3[i]);
       CoeffInterp[1][i]=0.0;
     }
     if (gridBc(0,1)==3){
       Xee0[0][i].redim(I1[i],Range(I2[i].getBase(), I2[i].getBound()),
			1,Range(0,ndimension-1));
       Xee0[0][i]=0.0;
       CoeffInterp[2][i].redim(I1[i],I2[i],I3[i]);
       CoeffInterp[2][i]=0.0;
     }
     if (gridBc(1,1)==3){
       Xee0[1][i].redim(I1[i],Range(I2[i].getBound(),I2[i].getBound()),
			1,Range(0,ndimension-1));
       Xee0[1][i]=0.0;
       CoeffInterp[3][i].redim(I1[i],I2[i],I3[i]);
       CoeffInterp[3][i]=0.0;
     }
     RHS[i].redim(I1[i],I2[i],I3[i],ndimension);
     RHS[i]=0.0;
     if (i>0){
       w[i].redim(I1[i],I2[i],I3[i],ndimension);
       w[i]=0.0;
     }
     dx[i]=1.0/(I1[i].getBound()-I1[i].getBase());
     if (ndimension>1) dy[i]=1.0/(I2[i].getBound()-I2[i].getBase());
     if (ndimension>2) dz[i]=1.0/(I3[i].getBound()-I3[i].getBase());
     //uprev[i].redim(u[i]);
     //uprev[i]=0.0;
    }
      // Set the dimensions of the residual vector
    residtemp1.redim(I1[0],I2[0],I3[0],ndimension);
    residtemp1=0.0;
      // Get initial condition for u[0]
     Initialize(u,u0,uprev,Igrid,Jgrid,Kgrid,Rr);

    iter=0;
    if ((numberOfPointattractions>0)||(numberOfIlineattractions>0)||
	(numberOfJlineattractions>0)||(numberOfKlineattractions>0)||
	(gridBc(0,0)==3)||(gridBc(0,1)==3)||(gridBc(1,0)==3)||
	(gridBc(1,1)==3)){
     for (i=0;i<nlevel;i++) getSource(i,0);
    } 
    /***** Skip Full Multigrid
    // Do the full Multigrid Vcycle to get initial condition
    for (i=(nlevel-1); i>=0;i--){
      multigridVcycle(i,i,(nlevel-1),uprev,userMap);
      if ((i-1) >= 0){
	Interpolate(i, i-1, u[i],u[i-1],2);
      }
    }
    *************/
    for (int axis=0;axis<2;axis++)
     for (int side=0; side<=1; side++)
       GRIDBC[2*axis+side] = new intArray[nlevel];

    IntegerArray index(2,3);
    Index I11,I22,I33;

    for (i=0;i<nlevel;i++){
     index(0,0)=I1[i].getBase();
     index(1,0)=I1[i].getBound();
     index(0,1)=I2[i].getBase();
     index(1,1)=I2[i].getBound();
     index(0,2)=I3[i].getBase();
     index(1,2)=I3[i].getBound();
     for (axis=0;axis<2;axis++)
      for (int side=0;side<=1;side++){
       getBoundaryIndex(index,side,axis,I11,I22,I33);
       if (I11.getBase() != I11.getBound()) I11=Ig1[i];
       if (I22.getBase() != I22.getBound()) I22=Ig2[i];
       GRIDBC[2*axis+side][i].redim(I11,I22,I33);
       GRIDBC[2*axis+side][i]=0;
      }
    }

    // Do few iterations of the V-cycle
    real WU, wcount=0.0, acount;
    real a1, b1,ratio=0.5, restmp=1.,restmp1=1.0, residlow;
    int changedLevel=0, nlevelTmp=nlevel;
    int itry=0;

    //nlevel=1;
    a1 = 1.0/(I1[0].getBound()-I1[0].getBase());
    if (ndimension==2)
     b1=1.0/(I2[0].getBound()-I2[0].getBase());

    WU=4*(1.0-pow(0.5,nlevel))+1;
    WU=1.0+11./24.+1.3333+0.3333;
    for (iter=0; iter<Maxiter;iter++){
    GRIDBC[0][0]=gridBc(0,0);
    GRIDBC[1][0]=gridBc(1,0);
    GRIDBC[2][0]=gridBc(0,1);
    GRIDBC[3][0]=gridBc(1,1);
     acount=0.;
     if (nlevel==1) acount=1.0;
     else if (nlevel==2) acount=1.0+1./4.+11./24.;
     else if (nlevel==3) acount=1.0+1./4.+11./24.+11./96.+1./16.;
     else acount=1.0+1./4.+11./24.+11./96.+1./16.+11./384+1./64.;
     //if ((changedLevel==0)&&(iter>4)&&(ratio<0.99)){
       //changedLevel=1;
       //nlevel=nlevelTmp;
       //for (int il=1;il<nlevel;il++)
	//Interpolate(il-1,il,u[il-1],u[il],2);
     //}
      //if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||
          //(gridBc(1,0)==3)||(gridBc(1,1)==3)) getAlpha();
      //if (iter==0) alphaPrev=alpha;
      //if (iter>0){
	//if (((resid1/residprev1)<0.98)&&(itry==0)){
	  //if (alpha>1.15*alphaPrev) alpha=1.15*alphaPrev;
          //alphaPrev=alpha;
        //}
	//else {
	  //alpha=alphaPrev;
	  //itry++;
	  //if (itry%10 == 0) itry=0;
        //}
      //}
      if (ratio<1.00){
	if (iter<3)
	 for (int i1=0;i1<nlevel;i1++)
	  utmp[i1]=u[i1];
        if (iter==1) restmp1=resid1;
        if (iter==2) restmp=resid1;
	if (iter==3) residlow=min(resid1,min(restmp,restmp1));
	if (iter>=3){
	  residlow=min(resid1,residlow);
	  //if ((restmp1<restmp)&&(resid1<=residlow))
	  if (restmp1<restmp)
	   for (int i1=0;i1<nlevel;i1++)
	     utmp[i1]=u[i1];
	  //printf("restmp1=%g\t restmp=%g\t resid1=%g\t residlow=%g\n",restmp1,restmp,resid1,residlow);

	    restmp1=restmp;
	    restmp=resid1;
        }
       }
      if (iter>0) residprev1=resid1;
      //printf("used alpha=%g\t itry=%i\n\n",alpha,itry);
      multigridVcycle(0,0,(nlevel-1),uprev);
      getResidual(residtemp1,0,uprev);
      //residtemp1.display("residual after multigrid");
      resid1=max(fabs(residtemp1));
      if (iter==0) residprev1=2.0*resid1;
      if (iter==0) residlow=resid1;
      ratio=resid1/residprev1;
      //if ((Maxiter==1)||(iter==0)) printf("residual=%g\n",resid1);
      if (iter>=0){
	//fprintf(stdout,"iter=%i\t resid1=%g\t residprev1=%g\t"
                //"ratio=%g\t eff ratio=%g\n",iter, resid1,
	       //residprev1,resid1/residprev1,pow(resid1/residprev1,1.0/WU));
	/***VOYONS
	printf("iter=%i\t resid=%g\t ",iter, resid1);
	if (useBlockTridiag != 1)
	  printf("Pmax=%g\t Qmax=%g\t ",max(fabs(Source[0](Iint1[0],Iint2[0],Iint3[0],0))), max(fabs(Source[0](Iint1[0],Iint2[0],Iint3[0],1))));
        else 
	  printf("Pmax=%g\t Qmax=%g\t ",
	    max(max(fabs(P[0][0])),
		max(max(fabs(P[1][0])), 
		    max(max(fabs(P[2][0])),
			max(fabs(P[3][0]))))),
	    max(max(fabs(Q[0][0])),
		max(max(fabs(Q[1][0])), 
		    max(max(fabs(Q[2][0])),
			max(fabs(Q[3][0]))))));
	printf("Maxdiff=%g\t",max(fabs(u[0]-utmp[0])));
	if (gridBc(0,1)==3) 
	  printf("dist2=%g\t ",
	   max(sqrt((u[0](I1[0],I2[0].getBase(),0,0)-
		     u[0](I1[0],I2[0].getBase()+1,0,0))*
		    (u[0](I1[0],I2[0].getBase(),0,0)-
		     u[0](I1[0],I2[0].getBase()+1,0,0))+
		    (u[0](I1[0],I2[0].getBase(),0,1)-
		     u[0](I1[0],I2[0].getBase()+1,0,1))*
		    (u[0](I1[0],I2[0].getBase(),0,1)-
		     u[0](I1[0],I2[0].getBase()+1,0,1)))));
       if (gridBc(1,1)==3)
	  printf("dist3=%g\t ",
	   max(sqrt((u[0](I1[0],I2[0].getBound(),0,0)-
		     u[0](I1[0],I2[0].getBound()-1,0,0))*
		    (u[0](I1[0],I2[0].getBound(),0,0)-
		     u[0](I1[0],I2[0].getBound()-1,0,0))+
		    (u[0](I1[0],I2[0].getBound(),0,1)-
		     u[0](I1[0],I2[0].getBound()-1,0,1))*
		    (u[0](I1[0],I2[0].getBound(),0,1)-
		     u[0](I1[0],I2[0].getBound()-1,0,1)))));
        if (gridBc(0,0)==3)
	  printf("dist0=%g\t ",
	   max(sqrt((u[0](I1[0].getBase(),I2[0],0,0)-
		     u[0](I1[0].getBase()+1,I2[0],0,0))*
		    (u[0](I1[0].getBase(),I2[0],0,0)-
		     u[0](I1[0].getBase()+1,I2[0],0,0))+
		    (u[0](I1[0].getBase(),I2[0],0,1)-
		     u[0](I1[0].getBase()+1,I2[0],0,1))*
		    (u[0](I1[0].getBase(),I2[0],0,1)-
		     u[0](I1[0].getBase()+1,I2[0],0,1)))));
        if (gridBc(1,0)==3)
	  printf("dist1=%g\t ",
	   max(sqrt((u[0](I1[0].getBound(),I2[0],0,0)-
		     u[0](I1[0].getBound()-1,I2[0],0,0))*
		    (u[0](I1[0].getBound(),I2[0],0,0)-
		     u[0](I1[0].getBound()-1,I2[0],0,0))+
		    (u[0](I1[0].getBound(),I2[0],0,1)-
		     u[0](I1[0].getBound()-1,I2[0],0,1))*
		    (u[0](I1[0].getBound(),I2[0],0,1)-
		     u[0](I1[0].getBound()-1,I2[0],0,1)))));
	printf("ratio=%g\t nlevel=%i\n",ratio,nlevel);
	VOYONS***/
	wcount += acount;
	printf("%g\t %g\n",wcount,log10(resid1));
	fflush(stdout);
	if ((ratio>1.)&&(iter>5)) nlevel--;
	if (nlevel<1) nlevel=1;
      }
    }
    //for (int j11=0;j11<=I2[0].getBound();j11++)
     //for (int i11=0;i11<=I1[0].getBound();i11++)
      //printf("i=%i\t j=%i\t xerror=%g\t yerror=%g\n",i11,j11,
       //residtemp1(i11,j11,0,0),residtemp1(i11,j11,0,1));


  //applyOrthogonalBoundaries(I1[0],I2[0],I3[0],1,0,u[0],uprev);
  delete[] idimension;
  if (ndimension>1) delete[] jdimension;
  if (ndimension>2) delete[] kdimension;
  delete[] GRIDBC[0];
  delete[] GRIDBC[1];
  delete[] GRIDBC[2];
  delete[] GRIDBC[3];
 }
