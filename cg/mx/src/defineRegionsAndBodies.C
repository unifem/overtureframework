#include "Maxwell.h"
#include "PlotStuff.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


// ===================================================================================
/// \brief Assign the mask that denotes the locations of bodies for the Yee scheme
/// 
///  Define material regions and bodies that are defined by a mask
// ===================================================================================
int Maxwell::
defineRegionsAndBodies()
{
  real time0=getCPU();

  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  assert( numberOfComponentGrids==1 );

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  const int grid=0;
  
  MappedGrid & mg = cg[grid];

  const bool isRectangular = mg.isRectangular();
  assert( isRectangular );

  mg.update(MappedGrid::THEmask );
  
  intArray & mask = mg.mask();
  #ifdef USE_PPP
    intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
  #else
    intSerialArray & maskLocal = mask; 
  #endif

  if( pBodyMask==NULL )
    pBodyMask = new intSerialArray(maskLocal.dimension(0),maskLocal.dimension(1),maskLocal.dimension(2));
    
  intSerialArray & bodyMask = *pBodyMask;

  getIndex(mg.gridIndexRange(),I1,I2,I3);
  int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);

  int i1,i2,i3;

  real dx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  mg.getRectangularGridParameters( dx, xab );

  const int i0a=mg.gridIndexRange(0,0);
  const int i1a=mg.gridIndexRange(0,1);
  const int i2a=mg.gridIndexRange(0,2);

  const real xa=xab[0][0], dx0=dx[0];
  const real ya=xab[0][1], dy0=dx[1];
  const real za=xab[0][2], dz0=dx[2];

  #define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
  #define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
  #define X2(i0,i1,i2) (za+dz0*(i2-i2a))



  enum BodyOptionsEnum
    {
      PECcylinder=0,
      dielectricCylinder=1,
      dielectricSphere,
      planeMaterialInterface
    } option;
  


  assert( gip !=NULL );
  GenericGraphicsInterface & gi = *gip;

  // here is a menu of possible initial conditions
  aString menu[]=  
  {
    "PEC cylinder",
    "dielectric cylinder",
    "dielectric sphere",
    "plane material interface",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">define bodies");

  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="PEC cylinder" )
    {
      // define a PEC cylinder
      option=PECcylinder;
      maskBodies=true;

      real x0=.0, y0=.0, z0=.0;
      real radius=.5;

      gi.inputString(answer2,"Enter radius and center of the cylinder: radius,x0,y0,z0");
      sScanF(answer2,"%e %e %e %e %e",&radius,&x0,&y0,&z0);  
      printF("PEC cylinder: using radius=%e, center=(%e,%e,%e)\n",radius,x0,y0,z0);

      const real radiusSquared = SQR(radius);
      if( ok )
      {
	bodyMask=0;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  // cell-center: 
	  real xc=X0(i1,i2,i3)+.5*dx[0];
	  real yc=X1(i1,i2,i3)+.5*dx[1];
      
	  real rad = SQR(xc-x0)+SQR(yc-y0);
	  if( rad < radiusSquared )
	  {
	    bodyMask(i1,i2,i3)=1;
	  }
	}
      }
      
    }
    else if( answer=="dielectric cylinder" ||
             answer=="dielectric sphere")
    {
      // define a dielectric cylinder or sphere
      if( answer=="dielectric cylinder" )
        option=dielectricCylinder;
      else
	option=dielectricSphere;

      real mum=mu, epsm=eps, sigmaEm, sigmaHm;
      real x0=.0, y0=.0, z0=.0;
      real radius=.5;

      gi.inputString(answer2,"Enter radius and center : radius,x0,y0,z0");
      sScanF(answer2,"%e %e %e %e %e",&radius,&x0,&y0,&z0);  
      printF("Dielectric cyl/sphere: using radius=%e, center=(%e,%e,%e)\n",radius,x0,y0,z0);

      gi.inputString(answer2,"Enter eps,mu,sigmaE,sigmaH");
      sScanF(answer2,"%e %e %e %e",&epsm,&mum,&sigmaEm,&sigmaHm);  
      printF("Dielectric cyl/sphere: using eps=%e mu=%e sigmaE=%e, sigmaH=%e\n",epsm,mum,sigmaEm,sigmaHm);

      const int nr = numberOfMaterialRegions;
      numberOfMaterialRegions++;
      
      if( ok && media.getLength(0)==0 )
      {
	// -- set default values for the media if they have not already been set --

	media.redim(maskLocal.dimension(0),maskLocal.dimension(1),maskLocal.dimension(2));
	media=0;  // the background domain is material 0 

	epsv.redim(numberOfMaterialRegions); 
        muv.redim(numberOfMaterialRegions); 
	sigmaEv.redim(numberOfMaterialRegions); 
        sigmaHv.redim(numberOfMaterialRegions);
        
	// set region 0 values:
	assert( numberOfMaterialRegions==2 ); // sanity check
	epsv=eps;
	muv=mu;
	sigmaEv=0.;
	sigmaHv=0.;

      }
      epsv.resize(numberOfMaterialRegions); muv.resize(numberOfMaterialRegions); 
      sigmaEv.resize(numberOfMaterialRegions); sigmaHv.resize(numberOfMaterialRegions);
      

      epsv(nr)=epsm;
      muv(nr)=mum;
      sigmaEv(nr)=sigmaEm;
      sigmaHv(nr)=sigmaHm;

      const real radiusSquared = SQR(radius);
      if( ok )
      {
	if( option==dielectricCylinder )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    // cell-center: 
	    real xc=X0(i1,i2,i3)+.5*dx[0];
	    real yc=X1(i1,i2,i3)+.5*dx[1];
      
	    real rad = SQR(xc-x0)+SQR(yc-y0);
	    if( rad < radiusSquared )
	    {
	      media(i1,i2,i3)=nr;
	    }
	  }
	}
	else if( option==dielectricSphere )
	{
	  printF(" *** Assign materials for the dielectricSphere ***\n");
	  
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    // cell-center: 
	    real xc=X0(i1,i2,i3)+.5*dx[0];
	    real yc=X1(i1,i2,i3)+.5*dx[1];
	    real zc=X2(i1,i2,i3)+.5*dx[2];
      
	    real rad = SQR(xc-x0)+SQR(yc-y0)+SQR(zc-z0);
	    if( rad < radiusSquared )
	    {
	      media(i1,i2,i3)=nr;
	    }
	  }
	}
	else
	{
          OV_ABORT("error");
	}
	
      }
      
    }
    else if( answer=="plane material interface" )
    {
      // define a planar interface between two materials

      option=planeMaterialInterface;

      real mum=mu, epsm=eps, sigmaEm, sigmaHm;
      real x0=.0, y0=.0, z0=.0;
      real nv[3]={1.,0.,0.};

      gi.inputString(answer2,"Enter the normal and a pt on the interface : n0,n1,n2, x0,y0,z0");
      sScanF(answer2,"%e %e %e %e %e %e",&nv[0],&nv[1],&nv[2],&x0,&y0,&z0);  
      real nNorm = max(REAL_MIN*100.,sqrt( nv[0]*nv[0]+nv[1]*nv[1]+nv[2]*nv[2] ));
      nv[0]/=nNorm;
      nv[1]/=nNorm;
      nv[2]/=nNorm;
      printF("Plane material interface: normal=(%e,%e,%e), point=(%e,%e,%e)\n",nv[0],nv[1],nv[2],x0,y0,z0);

      x0PlaneMaterialInterface[0]=x0;
      x0PlaneMaterialInterface[1]=y0;
      x0PlaneMaterialInterface[2]=z0;
      normalPlaneMaterialInterface[0]=nv[0];
      normalPlaneMaterialInterface[1]=nv[1];
      normalPlaneMaterialInterface[2]=nv[2];
      

      gi.inputString(answer2,"Enter eps,mu,sigmaE,sigmaH for the positive normal side");
      sScanF(answer2,"%e %e %e %e",&epsm,&mum,&sigmaEm,&sigmaHm);  
      printF("Plane material interface: eps=%e mu=%e sigmaE=%e, sigmaH=%e\n",epsm,mum,sigmaEm,sigmaHm);

      const int nr = numberOfMaterialRegions;
      numberOfMaterialRegions++;
      
      if( ok && media.getLength(0)==0 )
      {
	// -- set default values for the media if they have not already been set --

	media.redim(maskLocal.dimension(0),maskLocal.dimension(1),maskLocal.dimension(2));
	media=0;  // the background domain is material 0 

	epsv.redim(numberOfMaterialRegions); 
        muv.redim(numberOfMaterialRegions); 
	sigmaEv.redim(numberOfMaterialRegions); 
        sigmaHv.redim(numberOfMaterialRegions);
        
	// set region 0 values:
	assert( numberOfMaterialRegions==2 ); // sanity check
	epsv=eps;
	muv=mu;
	sigmaEv=0.;
	sigmaHv=0.;

      }
      epsv.resize(numberOfMaterialRegions); muv.resize(numberOfMaterialRegions); 
      sigmaEv.resize(numberOfMaterialRegions); sigmaHv.resize(numberOfMaterialRegions);
      

      epsv(nr)=epsm;
      muv(nr)=mum;
      sigmaEv(nr)=sigmaEm;
      sigmaHv(nr)=sigmaHm;

      if( ok )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  // cell-center: 
	  real xc=X0(i1,i2,i3)+.5*dx[0];
	  real yc=X1(i1,i2,i3)+.5*dx[1];
	  real zc=X2(i1,i2,i3)+.5*dx[2];
      
	  real nDotX = nv[0]*( xc-x0 ) + nv[1]*( yc-y0 ) + nv[2]*( zc-z0 );
	    
	  if( nDotX > 0. )
	  {
	    media(i1,i2,i3)=nr;
	  }
	}
      }
      
      // initializePlaneMaterialInterface();

//       // ------------------------------------------------------------
//       // Here we compute the coefficients in the exact solution
//       // 
//       // E(x,y) = (a1,a2,a3)*cos( k.(x-x0)-wt) + r*(b1,b2,b3)*cos( kr.(x-x0) - w t )
//       //        =                              tau*(d1,d2,d3)*cos( kappa.(x-x0) - w t )
//       //
//       // H(x,y) = (e1,e2,e3)*cos( k.(x-x0)-wt) + r*(f1,f2,f3)*cos( kr.(x-x0) - w t )
//       //        =                              tau*(g1,g2,g3)*cos( kappa.(x-x0) - w t )
//       //
//       // ------------------------------------------------------------
//       //   ** this could be put somewhere else **
//       // ------------------------------------------------------------
//       if( kz!=0 )
//       {
// 	printF("ERROR: plane material interface: kz!=0 \n");
// 	Overture::abort("error");
//       }
      
//       const real c1=1./sqrt(epsv(0)*muv(0));  // incident 
//       const real c2=1./sqrt(epsv(1)*muv(1));  // transmitted
//       const real cr =c2/c1;                   // relative index of refraction
//       const real mu1=muv(0);// incident 
//       const real mu2=muv(1);// transmitted
      
//       const real kNorm = sqrt(kx*kx+ky*ky+kz*kz);
//       assert( kNorm>0. );
//       const real kDotN = kx*nv[0]+ky*nv[1]+kz*nv[2];
      
//       // kr : reflected wave number:
//       //   kr.nv = - k.nv 
//       real kr[3]={kx,ky,kz};
//       for( int axis=0; axis<3; axis++ )
// 	kr[axis] = kr[axis] - 2.*kDotN*nv[axis];

//       const real krNorm=sqrt( SQR(kr[0])+SQR(kr[1])+SQR(kr[2]) );
//       assert( krNorm>0. );

//       // kappa: transmitted wave number
//       //   kappa.t = k.t 
//       real kappa[3]={kx,ky,kz};     
//       real kappatSq = kNorm*kNorm - kDotN*kDotN;        // tangential component of kappa = tang. comp of k (sign doesn't matter)
//       real arg = (kNorm*kNorm)/(cr*cr) - kappatSq;
//       if( arg<0. )
//       {
// 	printF("ERROR: computing the plane material interface solution: angle of incident is too close to 90 degrees\n");
// 	printF("       This case is not supported.\n");
// 	OV_ABORT("error");
//       }
//       real kappan = sqrt( arg );   // normal comp. of kappa jumps 
//       for( int axis=0; axis<3; axis++ )
// 	kappa[axis] = kappa[axis] + (kappan- kDotN)*nv[axis];  // subtract off k.n and add on kappa.n
//       printF(" (kx,ky,kz)=(%8.2e,%8.2e,%8.2e) kr=(%8.2e,%8.2e,%8.2e), kappa=(%8.2e,%8.2e,%8.2e), nv=\(%8.2e,%8.2e,%8.2e)\n",
// 	     kx,ky,kz,kr[0],kr[1],kr[2],kappa[0],kappa[1],kappa[2],nv[0],nv[1],nv[2]);
//       printF("kappatSq=%e, kappan=%e, kDotN=%e, arg=%e\n",kappatSq,kappan,kDotN,arg);
      
//       const real kappaNorm=sqrt( SQR(kappa[0])+SQR(kappa[1])+SQR(kappa[2]) );
//       assert( kappaNorm>0. );
//       const real cosTheta1=kDotN/kNorm;
//       const real cosTheta2=kappan/kappaNorm;

//       // reflection and transmission coefficients
//       const real r = (c1*cosTheta1-c2*cosTheta2)/(c1*cosTheta1+c2*cosTheta2);
//       const real tau = (2.*c2*cosTheta1)/(c1*cosTheta1+c2*cosTheta2);

//       printF("PMI: reflection-coeff=%8.2e, transmission-coeff=%8.2e\n",r,tau);

//       // E: (amplitude of incident=1)
//       const real av[3]={-ky/kNorm, kx/kNorm,0.};         // incident : we have different choices here in 3d 
//       const real bv[3]={-kr[1]/krNorm, kr[0]/krNorm,0.}; // reflected : this depends on av
//       const real dv[3]={-kappa[1]/kappaNorm, kappa[0]/kappaNorm,0.}; // transmitted : this depends on av

// //       // Incident wave defined by the plane wave amplitudes:
// //       av[0]=pwc[0]; av[1]=pwc[1]; av[2]=pwc[2];
// //       // reflected: 
// //       for( int axis=0; axis<3; axis++ )
// //   	bv[axis]=av[axis];
      
      
      

//       // H: (can be computed directly from E)
//       //   mu*H_t = -curl( E )
//       //  -mu*w*Hx = - [ D_y(Ez) - D_z(Ey) ]
//       //  -mu*w*Hy = - [ D_z(Ex) - D_x(Ez) ]
//       //  -mu*w*Hz = - [ D_x(Ey) - D_y(Ex) ]
//       real ev[3]; // incident H: this depends on av
//       real fv[3]; // reflected H
//       real gv[3]; // transmitted H

//       const real w = c1*kNorm;  // omega 
//       const real w1=w*mu1, w2=w*mu2;
//       ev[0] = (ky*av[2]-kz*av[1])/w1;
//       ev[1] = (kz*av[0]-kx*av[2])/w1;
//       ev[2] = (kx*av[1]-ky*av[0])/w1;
      
//       fv[0] = (kr[1]*bv[2]-kr[2]*bv[1])/w1;
//       fv[1] = (kr[2]*bv[0]-kr[0]*bv[2])/w1;
//       fv[2] = (kr[0]*bv[1]-kr[1]*bv[0])/w1;

//       gv[0] = (kappa[1]*dv[2]-kappa[2]*dv[1])/w2;
//       gv[1] = (kappa[2]*dv[0]-kappa[0]*dv[2])/w2;
//       gv[2] = (kappa[0]*dv[1]-kappa[1]*dv[0])/w2;

//       // Now fill in the constants that define the solution (see planeMaterialInterface.h)
      
//       // E : Incident+reflected:
//       pmc[ 0]=av[0]; pmc[ 1]=bv[0]*r;
//       pmc[ 2]=av[1]; pmc[ 3]=bv[1]*r;
//       pmc[ 4]=av[2]; pmc[ 5]=bv[2]*r;
//       // E : Transmitted
//       pmc[12]=dv[0]*tau; 
//       pmc[13]=dv[1]*tau; 
//       pmc[14]=dv[2]*tau; 

//       // H : Incident+reflected:
//       pmc[ 6]=ev[0]; pmc[ 7]=fv[0]*r;
//       pmc[ 8]=ev[1]; pmc[ 9]=fv[1]*r;
//       pmc[10]=ev[2]; pmc[11]=fv[2]*r;

//       // H : Transmitted
//       pmc[15]=gv[0]*tau; 
//       pmc[16]=gv[1]*tau; 
//       pmc[17]=gv[2]*tau; 
      
//       pmc[18]=w;  // omega 
//       pmc[19]=kx; pmc[20]=ky; pmc[21]=kz;
//       pmc[22]=kr[0]; pmc[23]=kr[1]; pmc[24]=kr[2];
//       pmc[25]=kappa[0]; pmc[26]=kappa[1]; pmc[27]=kappa[2];
//       pmc[28]=x0PlaneMaterialInterface[0]; pmc[29]=x0PlaneMaterialInterface[1]; pmc[30]=x0PlaneMaterialInterface[2];
//       pmc[30]=normalPlaneMaterialInterface[0]; pmc[31]=normalPlaneMaterialInterface[1]; pmc[32]=normalPlaneMaterialInterface[2];


    }
    else
    {
      printF("Unknown response: [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
  } // end for ( ;; )
  


  if( ok && option==PECcylinder )
  {
    
    assert( numberOfDimensions==2 );  // fix me 

    // We use zero for outside bodies so that we can use different values inside bodies,
    //     bodyMask(i1,i2,i3) = 1 : PEC 

    // Set the grid mask for plotting -- note the bodyMask indicates cells
    int includeGhost=0;
    ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
    assert( ok );
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      if( bodyMask(i1,i2,i3)!=0 && bodyMask(i1-1,i2,i3)!=0 &&  bodyMask(i1,i2-1,i3)!=0 &&  bodyMask(i1-1,i2-1,i3)!=0 )
      {
        // this node is surrounded by all empty cells
	maskLocal(i1,i2,i3)=0;
      }
    }
    
    // ::display(bodyMask,"Mask for bodies","%3i");

  }

  gi.unAppendTheDefaultPrompt();

  timing(timeForInitialize)+=getCPU()-time0;
  return 0;
}
