#include "FEMBeamModel.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "TravelingWaveFsi.h"
#include "TridiagonalSolver.h"

// Matrix-matrix multiple routine "mult" is now here: 
#include "RigidBodyMotion.h" 


#define  FOR_3(i1,i2,i3,I1,I2,I3)					\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();	\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();	\
  for( i3=I3Base; i3<=I3Bound; i3++ )					\
    for( i2=I2Base; i2<=I2Bound; i2++ )					\
      for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)					\
  int I1Base,I2Base,I3Base;						\
  int I1Bound,I2Bound,I3Bound;						\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();	\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();	\
  for( i3=I3Base; i3<=I3Bound; i3++ )					\
    for( i2=I2Base; i2<=I2Bound; i2++ )					\
      for( i1=I1Base; i1<=I1Bound; i1++ )



//static member
int FEMBeamModel::
FEMBeamCounter=0;



FEMBeamModel::
FEMBeamModel()
  :BeamModel()
{
  beamType = "FEMBeamModel";
  FEMBeamCounter++;
  printF("-- BM%i -- construct an %s\n",getBeamID(),beamType.c_str());

  // t = 0.0;  //Longfei: leave this for FEMBeamModel for now, since its too much tedius to change it


  //set some default parameters for FEMBeamModel
  dbase.get<int>("numberOfGhostPoints")=0; // numOfGhost on each side
  dbase.get<int>("numberOfMotionDirections")=1; // FEMBeamModel can only move in 1 direction....FOR NOW
  dbase.get<bool>("isCubicHermiteFEM")=true;  // Hermite FEM needs x derivatives


  // extra parameters for FEMBeamModel:
 
  // old way:
  // Element stiffness and mass matrices.  
  // Note that in this model they are constant in time
  // and the same for every element
  //
  // elementK.redim(4,4);
  // elementM.redim(4,4);
  // RealArray & elementB = dbase.put<RealArray>("elementB");  // holds "damping element matrix"
  // elementB.redim(4,4);

  // Longfei 20160321: moved to base class
  //dbase.put<RealArray*>("elementK")=new RealArray(4,4);  // holds "stiffness element matrix"
  //dbase.put<RealArray*>("elementM")=new RealArray(4,4);  // holds "mass element matrix"
  //dbase.put<RealArray*>("elementB")=new RealArray(4,4);  // holds "damping element matrix"

  //???Longfei: not sure what is this? put this parameter in dbase FOR NOW
  dbase.put<real>("leftCantileverMoment") = 0.0; 
  
 
    

  if(false)
    { 
      // check num of nodes ect...
      const int & numElem = dbase.get<int>("numElem");
      printF("numElem = %d for FEMBeamModel\n", numElem);
      printF("numOfGhost = %d for FEMBeamModel\n", dbase.get<int>("numOfGhost"));
    }

}

FEMBeamModel::
~FEMBeamModel()
{

}





// =================================================================================================
/// /brief Assign the force on the beam
/// /param x0 (input) : array of (undeformed) locations on the beam surface
/// /param traction (input) : traction on the deformed surface
/// /param normal (input) : normal to the deformed surface 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
// =================================================================================================
void FEMBeamModel::
addForce(const real & tf, const RealArray & x0, const RealArray & traction, const RealArray & normal,  
	 const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  RealArray & fc = f[current];  // force at current time
  
  // compute the surfaceForce load vector
  setSurfaceForce(tf, x0, traction, normal, Ib1, Ib2, Ib3 );
  
  fc = dbase.get<RealArray>("surfaceForce"); // pass the updated surface force to fc

  if( false )
    {
      const RealArray &time=dbase.get<RealArray>("time");
      ::display(fc,sPrintF("-- BM -- addForce : fc after addToElementIntegral, t=%9.3e\n",time(current)),"%9.2e ");
    }
  

  return;


  // // ** old way**

  // // Jb1, Jb2, Jb3 : for looping over cells instead of grid points -- decrease by 1 along active axis
  // Index Jb1=Ib1, Jb2=Ib2, Jb3=Ib3;
  // Index Jg1=Ib1, Jg2=Ib2, Jg3=Ib3;
  // int ia, ib; // index for boundary points
  // int iga, igb; // index for ghost points
  // int axis=-1, is1=0, is2=0, is3=0;
  // if( Jb1.getLength()>1 )
  // { // grid points on boundary are along axis=0
  //   axis=0; is1=1;
  //   ia=Ib1.getBase(); ib=Ib1.getBound();
  //   Jb1=Range(Jb1.getBase(),Jb1.getBound()-1); // decrease length by 1
  //   assert( Jb2.getLength()==1 );
  //   Jg1=Range(Jg1.getBase()-1,Jg1.getBound()+1); // add ghost
  //   iga = Jg1.getBase(); igb=Jg1.getBound();
  // }
  // else
  // { // grid points on boundary are along axis=1
  //   axis=1; is2=1;
  //   ia=Ib2.getBase(); ib=Ib2.getBound();
  //   Jb2=Range(Jb2.getBase(),Jb2.getBound()-1); // decrease length by 1
  //   assert( Jb1.getLength()==1 );
  //   Jg2=Range(Jg2.getBase()-1,Jg2.getBound()+1); // add ghost
  //   iga = Jg2.getBase(); igb=Jg2.getBound();
  // }
  // assert( axis>=0 );
  
  
  // const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  // const int orderOfAccuracyForDerivative=orderOfGalerkinProjection;

  // RealArray fDotN(Jg1,Jg2,Jg3); // add ghost

  // int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  // FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  // {
  //   fDotN(iv[axis])= (traction(i1,i2,i3,0)*normal(i1,i2,i3,0)+
  // 	              traction(i1,i2,i3,1)*normal(i1,i2,i3,1) );
  // }
  // // extrapolate ghost
  // if( orderOfAccuracyForDerivative==4 && igb > iga+5 )
  // { // this is needed for fourth-order accuracy 
  //   fDotN(iga)=5.*fDotN(iga+1)-10.*fDotN(iga+2)+10.*fDotN(iga+3)-5.*fDotN(iga+4)+fDotN(iga+5);
  //   fDotN(igb)=5.*fDotN(igb-1)-10.*fDotN(igb-2)+10.*fDotN(igb-3)-5.*fDotN(igb-4)+fDotN(igb-5);
  // }
  // else if( orderOfAccuracyForDerivative==4 &&  igb > iga+4 )
  // {
  //   fDotN(iga)=4.*fDotN(iga+1)-6.*fDotN(iga+2)+4.*fDotN(iga+3)-fDotN(iga+4);
  //   fDotN(igb)=4.*fDotN(igb-1)-6.*fDotN(igb-2)+4.*fDotN(igb-3)-fDotN(igb-4);
  // }
  // else if( igb > iga+3 )
  // {
  //   fDotN(iga)=3.*fDotN(iga+1)-3.*fDotN(iga+2)+fDotN(iga+3);
  //   fDotN(igb)=3.*fDotN(igb-1)-3.*fDotN(igb-2)+fDotN(igb-3);
  // }
  // else
  // {
  //   assert( igb> iga+2 );
  //   fDotN(iga)=2.*fDotN(iga+1)-fDotN(iga+2);
  //   fDotN(igb)=2.*fDotN(igb-1)-fDotN(igb-2);
  // }
 
  //   // printF("--BM-- addForce : tf=%9.3e (p1,p2)=(%8.2e,%8.2e)\n",tf,p1,p2);
  // real beamLength=L;
  // const real dx = beamLength/numElem;  
  // FOR_3(i1,i2,i3,Jb1,Jb2,Jb3)
  // {
  //   int i1p=i1+is1, i2p=i2+is2, i3p=i3+is3;
  //   real f1x, f2x;
  //   if( orderOfAccuracyForDerivative==2 )
  //   {
  //     f1x = (fDotN(iv[axis]+1)-fDotN(iv[axis]-1))/(2.*dx);
  //     f2x = (fDotN(iv[axis]+2)-fDotN(iv[axis]  ))/(2.*dx);
  //   }
  //   else
  //   {
  //     int i=iv[axis];
  //     if( i-2 >= iga && i+2 <= igb )
  //       f1x = ( 8.*(fDotN(i+1)-fDotN(i-1)) - fDotN(i+2) +fDotN(i-2) )/(12.*dx);
  //     else if( i==ia && i+4 <=igb )
  //     { // fourth-order one-sided
  //       f1x = ( -(25./12.)*fDotN(i) + 4.*fDotN(i+1) -3.*fDotN(i+2) + (4./3.)*fDotN(i+3) -.25*fDotN(i+4) )/dx;
  //     }
  //     else
  // 	f1x = (fDotN(i+1)-fDotN(i-1))/(2.*dx);
      
  //     i=iv[axis]+1;
  //     if( i-2 >= iga && i+2 <= igb )
  //       f2x = ( 8.*(fDotN(i+1)-fDotN(i-1)) - fDotN(i+2) +fDotN(i-2) )/(12.*dx);
  //     else if( i==ib && i-4 >=iga )
  //     { // fourth-order one-sided
  //       f2x = -( -(25./12.)*fDotN(i) + 4.*fDotN(i-1) -3.*fDotN(i-2) + (4./3.)*fDotN(i-3) -.25*fDotN(i-4) )/dx;
  //     }
  //     else
  // 	f2x = (fDotN(i+1)-fDotN(i-1))/(2.*dx);
  //   }
    
  //   printF("--BM-- addForce: x0=[%8.2e,%8.2e] f1=%8.2e f1x=%8.2e, x0=[%8.2e,%8.2e] f2=%8.2e f2x=%8.2e\n",
  // 	   x0(i1,i2,i3,0),x0(i1,i2,i3,1),  fDotN(iv[axis]), f1x, 
  //          x0(i1p,i2p,i3p,0),x0(i1p,i2p,i3p,1), fDotN(iv[axis]+1), f2x );    

  //   addForce(tf,
  // 	     x0(i1,i2,i3,0), 
  // 	     x0(i1,i2,i3,1),  fDotN(iv[axis]), f1x,
  // 	     normal(i1,i2,i3,0), normal(i1,i2,i3,1),
  // 	     x0(i1p,i2p,i3p,0), 
  // 	     x0(i1p,i2p,i3p,1), fDotN(iv[axis]+1), f2x, 
  // 	     normal(i1p,i2p,i3p,0), normal(i1p,i2p,i3p,1));
  // }
  
  // if( false )
  // {
  //   const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  //   const int & current = dbase.get<int>("current"); 

  //   RealArray & time = dbase.get<RealArray>("time");
  //   std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  //   RealArray & fc = f[current];  // force at current time
  //   ::display(fc,sPrintF("--BM-- addForce:END: : fc at t=%9.3e",tf),"%8.2e ");
  // }
  

}



// ???Longfei 20160329: this function seems to be unused... remove for now
// ======================================================================================
// Accumulate a pressure force to the beam from the fluid element whose 
// undeformed location is X1 = (x0_1, y0_1), X2 = (x0_2, y0_2).
// The pressure is p(X1) = p1, p(X2) = p2
/// \param tf : add force at this time 
// x0_1: undeformed location of the point on the surface of the beam (x1)  
// y0_1: undeformed location of the point on the surface of the beam (y1)
// p1:   pressure at the point (x1,y1)
// nx_1: normal at x1 (x) [unused]
// ny_1: normal at x1 (y) [unused]
// x0_2: undeformed location of the point on the surface of the beam (x2)  
// y0_2: undeformed location of the point on the surface of the beam (y2)  
// p2:   pressure at the point (x2,y2)
// nx_2: normal at x2 (x) [unused]
// ny_2: normal at x2 (y) [unused]
//
// ======================================================================================
// void FEMBeamModel::
// addForce( const real & tf,
// 	  const real& x0_1, const real& y0_1,
// 	  real p1, real p1x, const real& nx_1,const real& ny_1,
// 	  const real& x0_2, const real& y0_2,
// 	  real p2, real p2x, const real& nx_2,const real& ny_2)
// {


//   // new way *wdh* 2015/01/13
//   real x1[2]={ x0_1,y0_1}, nv1[2]={nx_1,ny_1}; //
//   real x2[2]={ x0_2,y0_2}, nv2[2]={nx_2,ny_2}; //

//   std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
//   const int & current = dbase.get<int>("current"); 
//   RealArray & fc = f[current];  // force at current time
//   bool addToForce=true;
  
//   addToElementIntegral( tf,x1,p1,p1x,nv1, x2,p2,p2x,nv2,fc,addToForce );


//   return;
    
//   //  // * OLD WAY **

//   //  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
//   //  const int & current = dbase.get<int>("current"); 

//   //  RealArray & time = dbase.get<RealArray>("time");
//   //  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

//   //  RealArray & fc = f[current];  // force at current time
//   //  if( fabs(time(current)-tf) > 1.e-10*(1.+tf) )
//   //  {
//   //    printF("--BM-- BeamModel::addForce:ERROR: tf=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
//   //        tf,time(current),current);
//   //    OV_ABORT("ERROR");
//   //  }

//   //  int elem1,elem2;
//   //  real eta1,eta2,t1,t2;

//   //  real p11, p22, p11x, p22x;

//   //  //std::cout << x0_1 << " " << p1 << std::endl;
  
//   // // if (p1 != p1/* || p1 > 100.0*/) {
//   // //   //std::cout << "Found nan!" << std::endl;
//   // // }

//   //  //std::cout << getExactPressure(t,x0_1) << " " << p1 << std::endl;
//   //  //
  
//   //  //p1 = getExactPressure(t,x0_1)*1000.0;
//   //  //p2 = getExactPressure(t,x0_2)*1000.0;
  
  
//   //  // (xll,yll) = point_1 - beam_0 
//   //  real xll = x0_1-beamX0;
//   //  real yll = y0_1-beamY0;

//   //  // (xl,yl) = relative coordinates along (rotated) beam 
//   //  real xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
//   //  real yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

//   //  // myx0 = relative beam coordinate in [0,L] of point_1
//   //  real myx0 = xl;

//   //  // (xll,yll) = point_2 - beam_0
//   //  xll = x0_2-beamX0;
//   //  yll = y0_2-beamY0;
//   //  //  // (xl,yl) = relative coordinates along (rotated) beam 
//   //  xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
//   //  yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

//   //  // myx1 = relative beam coordinate in [0,L] of point_2
//   //  real myx1 = xl;

//   //  // point_1 lies in elem1, offset eta1
//   //  // point_2 lies in elem2, offset eta2
//   //  if (myx1 > myx0) 
//   //  {
//   //    projectPoint(x0_1,y0_1,elem1, eta1,t1); 
//   //    projectPoint(x0_2,y0_2,elem2, eta2,t2);   
//   //    p11 = p1*pressureNorm;  p11x = p1x*pressureNorm; 
//   //    p22 = p2*pressureNorm;  p22x = p2x*pressureNorm;
//   //  } 
//   //  else 
//   //  {
//   //    projectPoint(x0_1,y0_1,elem2, eta2,t2); 
//   //    projectPoint(x0_2,y0_2,elem1, eta1,t1);  
//   //    p22 = p1*pressureNorm; p22x = p1x*pressureNorm;
//   //    p11 = p2*pressureNorm; p11x = p2x*pressureNorm; 
//   //    std::swap<real>(myx0,myx1);
//   //  }

//   //  //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << p1 << " " << p2 << std::endl;

//   //  const real dx12 = max(fabs(myx0-myx1),REAL_MIN*100.); // distance between point_1 and point_2

//   //  const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  
//   //  //                 elem1            elem2
//   //  //        +----------+--X------+-----X--+-----
//   //  //                      myx0        myx1
//   //  RealArray lt(4);
//   //  for (int i = elem1; i <= elem2; ++i) 
//   //  {

//   //    real a = eta1, b = eta2;
//   //    real pa = p11, pax=p11x, pb = p22, pbx=p22x;
//   //    real x0 = myx0, x1 = myx1;

//   //    if (i != elem1) 
//   //    {
//   //      // We have moved to the next element from the first
//   //      a = -1.0;  // xi value
//   //      x0 = le*i; // x-value 
//   //      if( orderOfGalerkinProjection==2 )
//   //      {
//   //        pa = p11 + (p22-p11)*(x0-myx0)/dx12;
//   //      }
//   //      else
//   //      {
//   //        // -- evaluate an Hermite interpolant fit to (myx0,p11)--(myx1,p22) --
//   //        //         p11,p11x            p22,p22x
//   //        //           X-------+-----------X
//   //        //          myx0                myx1
//   //        //           -1      xi          +1
//   //        real xi = -1. + 2.*(x0-myx0)/dx12;
//   //        // ** CHECK ME ***
//   //        real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
//   //        real N2 = .125*dx12*(1.-xi)*(1.-xi)*(1.+xi);
//   //        real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
//   //        real N4 = .125*dx12*(1.+xi)*(1.+xi)*(xi-1.);
	
//   //        real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx12);
//   //        real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
//   //        real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx12) ;
//   //        real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;
	
//   //        pa = p11*N1 + p11x*N2 + p22*N3 + p22x*N4; 
//   //        pax = p11*N1x + p11x*N2x + p22*N3x + p22x*N4x; 

//   //      }
      
//   //    }
//   //    // -- right end is not on a node -- adjust pb,pbx --
//   //    if (i != elem2) 
//   //    {
//   //      b = 1.0;
//   //      x1 = le*(i+1);
//   //      if( orderOfGalerkinProjection==2 )
//   //      {
//   //        pb = p11 + (p22-p11)*(x1-myx0)/dx12;
//   //      }
//   //      else
//   //      {
//   //        // -- evaluate the Hermite interpolant --
//   //        real xi = -1. + 2.*(x1-myx0)/dx12;

//   //        real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
//   //        real N2 = .125*dx12*(1.-xi)*(1.-xi)*(1.+xi);
//   //        real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
//   //        real N4 = .125*dx12*(1.+xi)*(1.+xi)*(xi-1.);
	
//   //        real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx12);
//   //        real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
//   //        real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx12) ;
//   //        real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;

//   //        pb = p11*N1 + p11x*N2 + p22*N3 + p22x*N4; 
//   //        pbx = p11*N1x + p11x*N2x + p22*N3x + p22x*N4x; 
//   //      }
//   //    }
    
//   //    // printF("--AF-- x0=%7.5f pa=%7.5f pax=%7.5f, x1=%7.5f pb=%7.5f pbx=%7.5f [a,b]=[%7.4f,%7.4f]\n",
//   //    //       x0,pa,pax,x1,pb,pbx,a,b);
    
//   //    Index idx(i*2,4);
//   //    if (t1 > 0) 
//   //    { // Flip sign of force to account for the normal 
//   //      pa = -pa; pax = -pax;
//   //      pb = -pb; pbx = -pbx;
//   //    }
    
      
//   //    //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << 
//   //    //  p1 << " " << p2 << std::endl;
  

//   //    if (fabs(b-a) > 1.0e-10)  // *WDH* FIX ME -- is this needed?
//   //    {
//   //      // -- compute (N,p)_[a,b] = int_a^b N(xi) p(xi) J dxi 
//   //      if( orderOfGalerkinProjection==2 )
//   //        computeProjectedForce(pa,pb, a,b, lt);
//   //      else
//   //        computeGalerkinProjection(pa,pax, pb,pbx,  a,b, lt);

//   //      //    std::cout << "a = " << a << " b = " << b << std::endl;
//   //      fc(idx) += lt;

//   //      real gradp = 1.0;
//   //      totalPressureForce += (lt(0)+lt(2));
//   //      totalPressureMoment += (lt(0)*(le*i-0.5*L)+lt(1)*gradp+lt(2)*(le*(i+1)-0.5*L) + lt(3)*gradp);
//   //    }
//   //    //printArray(lt,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);

//   //  }

// }






// ======================================================================================
/// \brief initialize the beam model
/// the FEMBeamModel version calls base version initialize + initialize FEM matrices:
///  elementK, elementM, elementB
// ======================================================================================
void FEMBeamModel::
initialize()
{

  //call base initialization
  BeamModel::initialize();
 

 
  // Longfei 20160121: new way of handling parameters  
  RealArray & elementT = *dbase.get<RealArray*>("elementT");
  RealArray & elementK = *dbase.get<RealArray*>("elementK");
  RealArray & elementM = *dbase.get<RealArray*>("elementM");

  RealArray *& pB =  dbase.get<RealArray*>("elementB");
  if(pB==NULL)
    pB=new RealArray(elementT);  // use the same size as elementT
  RealArray & elementB= *pB;


  //const real Abar =density*thickness*breadth;

    
  //build FEM matrices


  // physical parameters
  const real & Abar = dbase.get<real>("massPerUnitLength");
  const real & K0 = dbase.get<real>("K0");
  const real & T = dbase.get<real>("tension");
  const real & EI = dbase.get<real>("EI");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");

  if( false && Kxxt!=0. )
    {
      OV_ABORT("--BeamModel: ERROR: Kt!=0 or Kxxt!=0 -- this term not implemented yet");
    }
  
  
  // Build FEM matrices
  // *wdh* 2014/06/17 -- tension term added
  // Tension matrix from (v_x,w_x)
  // elementT(0,0) = 6./(5.*le);    
  // elementT(0,1) = 1./10.; 
  // elementT(0,2) = -elementT(0,0); 
  // elementT(0,3) = elementT(0,1);
  // elementT(1,0) = elementT(0,1);  
  // elementT(1,1) = le*2./15.; 
  // elementT(1,2) = -elementT(0,1); 
  // elementT(1,3) = - le/30.;
  // elementT(2,0) = elementT(0,2); 
  // elementT(2,1) = elementT(1,2); 
  // elementT(2,2) = elementT(0,0); 
  // elementT(2,3) = elementT(1,2);
  
  // elementT(3,0) = elementT(0,1); 
  // elementT(3,1) = elementT(1,3); 
  // elementT(3,2) = elementT(2,3);
  // elementT(3,3) = elementT(1,1);


  // Stiffness element matrix from beam term: (v_xx, EI w_xx) 
  elementK *= EI;

  // elementK(0,0) = EI*12./le3;
  // elementK(0,1) = EI*6./le2;
  // elementK(0,2) = -elementK(0,0); 
  // elementK(0,3) = elementK(0,1);

  // elementK(1,0) = elementK(0,1);  
  // elementK(1,1) = EI*4./le;
  // elementK(1,2) = -elementK(0,1); 
  // elementK(1,3) = EI*2./le;

  // elementK(2,0) = elementK(0,2); 
  // elementK(2,1) = elementK(1,2); 
  // elementK(2,2) = elementK(0,0); 
  // elementK(2,3) = elementK(1,2);
  
  // elementK(3,0) = elementK(0,1); 
  // elementK(3,1) = elementK(1,3); 
  // elementK(3,2) = elementK(2,3);
  // elementK(3,3) = elementK(1,1);
  
  // Scaled element mass matrix (v,w):
  // elementM(0,0) = elementM(2,2) = 13./35.*le;
  // elementM(0,1) = elementM(1,0) = 11./210.*le2;
  // elementM(0,2) = elementM(2,0) = 9./70.*le;
  // elementM(0,3) = elementM(3,0) = -13./420.*le2;
  // elementM(1,1) = elementM(3,3) = 1./105.*le3;
  // elementM(1,2) = elementM(2,1) = 13./420.*le2;
  // elementM(1,3) = elementM(3,1) = -1./140.*le3;
  // elementM(3,2) = elementM(2,3) = -11./210.*le2;

  // Add linear stiffness term : K0*(v,w) 
  // *wdh* 2014/12/25 -- Stiffness term -K0*w added,
  //    Stiffness matrix entries from :  -K0*(v,w)   (like Mass matrix)
  elementK += K0*elementM;

  // Element damping matrix B:
  elementB = Kt*elementM + Kxxt*elementT;

  // Actual mass matrix: 
  //const real Abar =density*thickness*breadth; //old way to get Abar
  // elementM *= Abar;

  // Stiffness element matrix including "tension term":
  elementK += T*elementT;

}







// =======================================================================================
/// /brief  Compute the internal force in the beam, f = -B*v -K*u
/// /param u (input) : position of the beam 
/// /param v (input) : velocity of the beam
/// /param f (output) :internal force [out]
// =======================================================================================
void FEMBeamModel::
computeInternalForce(const RealArray& u, const RealArray& v, RealArray& f) 
{
  const int & numElem = dbase.get<int>("numElem");
  const RealArray & elementK = *dbase.get<RealArray*>("elementK");
  const RealArray & elementB = *dbase.get<RealArray*>("elementB");
  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft =  boundaryConditions[0];
  const BoundaryCondition & bcRight =  boundaryConditions[1];

  RealArray elementU(4);
  RealArray elementForce(4);

  f = 0.0;
  for( int i = 0; i < numElem; ++i )
    {
      // elementU = [ u_i, ux_i, u_{i+1} ux_{i+1} ]
      for (int k = 0; k < 4; ++k)
	elementU(k) = u(i*2+k);
    
      elementForce = RigidBodyMotion::mult(elementK, elementU);
      for( int k = 0; k < 4; ++k )
	f(i*2+k) -= elementForce(k);
    }

  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");
  if( Kt!=0. || Kxxt!=0. )
    {
      // add damping terms to internal force
      RealArray & elementV = elementU; // reuse space
      for( int i = 0; i < numElem; ++ i)
	{
	  // elementV = [ v_i, vx_i, v_{i+1} vx_{i+1} ]
	  for (int k = 0; k < 4; ++k )
	    elementV(k) = v(i*2+k);
    
	  elementForce = RigidBodyMotion::mult(elementB, elementV);
	  for( int k = 0; k < 4; ++k )
	    f(i*2+k) -= elementForce(k);
	}

    }
  

  const bool isPeriodic = bcLeft==periodic;
  if( isPeriodic )
    {
      Index Is(0,2), Ie(2*numElem,2);
      f(Is) += f(Ie);
      f(Ie) = f(Is);
    }

}




//==============================================================================================
/// \brief Multiply a vector w by the mass matrix
/// w:  vector
/// Mw: M*w [out]
///
//==============================================================================================
void FEMBeamModel::
multiplyByMassMatrix(const RealArray& w, RealArray& Mw)
{
  const int & numElem = dbase.get<int>("numElem");
  const RealArray & elementM = *dbase.get<RealArray*>("elementM");

  RealArray elementU(4);
  RealArray tmpv(4);

  Mw = w;
  Mw = 0.0;

  for (int i = 0; i < numElem; ++i) 
    {
      // elementu = [ w_i wx_i w_{i+1} wx_{i+1} ]  
      for (int k = 0; k < 4; ++k)
	elementU(k) = w(i*2+k);
    
      tmpv = RigidBodyMotion::mult(elementM, elementU);

      for (int k = 0; k < 4; ++k)
	Mw(i*2+k) += tmpv(k);
    }

  
}






//================================================================================================
/// \brief Compute the acceleration of the beam.
///
/// \param u (input):               current beam position (For Newmark this is un + dt*vn + .5*dt^2*(1-2*beta)*an )
/// \param  (input)v:               current beam velocity (NOT USED CURRENTLY)
/// \param f (input):               external force on the beam
/// \param A (input):               matrix by which the acceleration is multiplied
///                  (e.g., in the newmark beta correction step it is 
///                   M+beta*dt^2*K)
/// \param a (input):               beam acceleration [out]
/// \param linAcceleration (input): acceleration of the CoM of the beam (for free motion) [out]
/// \param omegadd (input):         angular acceleration of the beam (for free motion) [out]
/// \param dt (input):              time step
/// \param alpha (input) :  coeff of K in  (M + alphaB*B + alpha*K)*a = RHS
/// \param alphaB (input) : coeff of B in  (M + alphaB*B + alpha*K)*a = RHS
/// \param tridiagonalSolverName (input) : 
//
//================================================================================================
// void FEMBeamModel::
// computeAcceleration(const real t,
// 		    const RealArray& u, const RealArray& v, 
// 		    const RealArray& f,
// 		    const RealArray& A,
// 		    RealArray& a,
// 		    real linAcceleration[2],
// 		    real& omegadd,
// 		    real dt,
//                     const real alpha, const real alphaB,
//                     const aString & tridiagonalSolverName )
// {
  
//   // Longfei 20160121: new way of handling parameters  
//   //const real EI = elasticModulus*areaMomentOfInertia;
//   const real & EI = dbase.get<real>("EI");
//   const real & density = dbase.get<real>("density");
//   const real & thickness = dbase.get<real>("thickness");
//   const real & breadth = dbase.get<real>("breadth");
//   const real & dx = dbase.get<real>("elementLength");
//   const real & L = dbase.get<real>("length");
//   //const real beamLength=L;
//   //const real dx = beamLength/numElem;  
//   const int & numElem = dbase.get<int>("numElem");
//   const real & buoyantMass= dbase.get<real>("buoyantMass"); 
//   const real & totalMass=  dbase.get<real>("totalMass");
//   const real & totalInertia=  dbase.get<real>("totalInertia");
//   const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
//   const BoundaryCondition & bcLeft =  boundaryConditions[0];
//   const BoundaryCondition & bcRight =  boundaryConditions[1];
//   const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");

  
//   if( debug & 2 )
//     printF("--BM-- BeamModel::computeAcceleration, t=%8.2e, dt=%8.2e\n",t,dt);
  
//   const int & current = dbase.get<int>("current"); 
//   std::vector<RealArray> & ua = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
//   RealArray & uc = ua[current];  // current displacement 

//   RealArray rhs(numElem*2+2); // *wdh* 2014/06/19

//   // Compute:   rhs = -B*v -K*u 
//   computeInternalForce(u, v, rhs);


//   if( debug & 2 )
//     {
//       rhs.reshape(2,numElem+1);
//       ::display(rhs,"-- BM -- computeAcceleration: rhs after computeInternalForce","%9.2e ");
//       rhs.reshape(numElem*2+2);
//     }
  
//   rhs += f;

//   if( !allowsFreeMotion ) 
//     {
//       // --- Apply boundary conditions to f - Ku  ----

//       // --- If the boundary degrees of freedom are given  (e.g. w(0,t)=g0(t) or wx(0,t)=h0(t))
//       //     then we eliminate the corresponding equation from the matrix equation (by setting it to the indentity)

//       // Get two time derivatives of the boundary functions for "acceleration BC"
//       RealArray gtt;
//       int ntd=2;  
//       getBoundaryValues( t, gtt, ntd );



    
//       // For natural BC's we need EI*wxx(0,t)
//       const real & T = dbase.get<real>("tension");
//       const real & Kxxt = dbase.get<real>("Kxxt");

//       RealArray g;
//       getBoundaryValues( t, g );

//       real accelerationScaleFactor=1.;
//       if( tridiagonalSolverName=="rhsSolver" )
// 	{
// 	  // when we compute the RHS directly we are solving for rho*hs*b utt (not utt )
// 	  accelerationScaleFactor=density*thickness*breadth;
// 	}
    
//       for( int side=0; side<=1; side++ )
// 	{
// 	  BoundaryCondition bc = side==0 ? bcLeft : bcRight;
// 	  const int ia = side==0 ? 0 : numElem*2;
// 	  const int ib = side==0 ? ia+2 : ia-2;
// 	  const int is = 1-2*side;
      
// 	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
// 	  if( bc==clamped && EI==0. ) bc=pinned;

// 	  // real x = side==0 ? 0 : L;
// 	  if( bc == clamped ) 
// 	    {
// 	      // First two equations in the matrix are
// 	      //       w_tt = given
// 	      //       wx_tt = given 
// 	      if( false )
// 		{
// 		  printF("--BM-- side=%i set clamped BC gtt=%e, gttx=%e, accelerationScaleFactor=%8.2e\n",
// 			 gtt(0,side),gtt(1,side),accelerationScaleFactor);
// 		}
	
// 	      rhs(ia  )=gtt(0,side)*accelerationScaleFactor;   // w_tt is given
// 	      rhs(ia+1)=gtt(1,side)*accelerationScaleFactor;   // wxtt is given 
// 	    }
// 	  else if( bc==pinned )
// 	    {
// 	      rhs(ia)=gtt(0,side)*accelerationScaleFactor;   // w_tt is given

// 	      if( bc == pinned && EI != 0.) 
// 		{
// 		  // Boundary term is of the form:  -EI* v_x*w_xx
// 		  // -- correct for natural BC:  E*I*w_xx = +/- g(2,side)
// 		  if( debug & 1 )	printF("-- BM -- set rhs for pinned BC wxx = g(2,side)=%8.2e, EI=%g\n",g(2,side),EI);
// 		  rhs(ia+1) += -(is)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)
// 		}
// 	    }
// 	  else if( bc==slideBC )
// 	    {
// 	      // ---- slide BC ---
// 	      // Equation 2 in matrix:    wx_tt = given 
// 	      rhs(ia+1)=gtt(1,side)*accelerationScaleFactor;   // wxtt is given 

// 	      // Equation 1 is adjusted:
// 	      rhs(ia  ) +=  (is)*EI*g(3,side);                 // add : -E*I*wxxx(0,t) * N(0)
// 	      rhs(ia  ) += -(is)*T *g(1,side);                 // add :  T wx(0,t)*N(0) 
// 	      rhs(ia  ) += -(is)*Kxxt*g(2,side);               // add :  Kxxt*wxt(0,t)
// 	    }
// 	  else if( bc==freeBC )
// 	    {
// 	      // Boundary terms are of the form:  T*v*w_x  -EI* v*w_xxx - EI* v_x*w_xx
// 	      // Free BC: wxx=EI* g(2,side), w_xxx= EI*g(3,side)

// 	      // printF("-- BM -- set rhs for free BC, g(2)=%e, g(3)=%e, T*u(ia+1)=%8.2e \n",g(2,side),g(3,side),T*u(ia+1));

// 	      rhs(ia  ) +=  (is)*EI*g(3,side);   // add : -E*I*wxxx(0,t) * N(0)
// 	      rhs(ia+1) += -(is)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)

// 	      // The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
// 	      // ***CHECK ME*** IS THIS RIGHT?
// 	      rhs(ia  ) += -(is)*T*u(ia+1);         // add : T*N1(0)*Np_1_x(0)*w'_1
// 	      rhs(ia  ) += -(is)*Kxxt*v(ia+1);      // add : T*N1(0)*Np_1_x(0)*wxt_1

// 	    }
// 	  else if( bc==internalForceBC )
// 	    { // BC used when computing the "internal force"  F = L(u,v) + f , given (u,v)
// 	      rhs(ia  ) += -(is)*T*u(ia+1);      // add : T*N1(0)*Np_1_x(0)*w'_1
// 	      rhs(ia  ) += -(is)*Kxxt*v(ia+1);      // add : T*N1(0)*Np_1_x(0)*w'_1

// 	      // evaluate wxx and wxxx on the boundary

// 	      real wxxx, wxx;
// 	      if( FALSE )  
// 		{
// 		  const real dxidx = 2./dx;  // d(xi)/dx
// 		  // find the 2nd and third derivatives of the basis functions at the ends
// 		  real phijxx = -1.5*dxidx*dxidx, phijxxx=1.5*dxidx*dxidx*dxidx;
// 		  real psijxx = -dx*dxidx*dxidx,  psijxxx=.75*dx*dxidx*dxidx*dxidx;
	
// 		  // Here is wxx to 2nd-order and wxxx to first order accuracy
// 		  // This formula is the same as found from Taylor series
// 		  wxx  = (u(ia)- u(ib))*phijxx  + (is)*( u(ia+1) +.5*u(ib+1) )*psijxx;
// 		  wxxx = (is)*(u(ia)- u(ib) )*phijxxx + (u(ia+1)+u(ib+1))*psijxxx;

// 		  if( false )
// 		    {
// 		      printF(" side=%i: phijxx=%9.3e, phijxxx=%9.3e  dx=%8.2e, 1/dx=%8.2e\n",side,phijxx,phijxxx,dx,1/dx);
// 		      printF(" side=%i: psijxx=%9.3e, psijxxx=%9.3e\n",side,psijxx,psijxxx);
// 		      printF(" side=%i: (u,u')(ia)=(%e,%e) (u,u')(ib)=%e,%e)\n",u(ia),u(ia+1),u(ib),u(ib+1));
// 		      printF(" side=%i: wxx=%9.3e, wxxx=%9.3e\n",side,wxx,wxxx);
// 		    }
// 		}
// 	      else
// 		{
// 		  // **NEW** 
// 		  // From cgDoc/moving/codes/beam/interp.maple
// 		  // 4-order in upp, 2nd-order in uppp: 
// 		  const int ic = ib+2*is; // 2nd point inside
// 		  real h = is*dx, h2=h*h, h3=h2*h;
// 		  real u0=u(ia),     u1=u(ib),    u2=u(ic);
// 		  real up0=u(ia+1), up1=u(ib+1), up2=u(ic+1);
// 		  // wxx =-1/50.*(244*h*up0+176*h*up1-6*h*up2+407*u0-400*u1-7*u2)/h2;
// 		  // wxxx=3/50.*(189*h*up0+256*h*up1-11*h*up2+417*u0-400*u1-17*u2)/h3;
// 		  wxx =-1/2.*(12*h*up0+16*h*up1+2*h*up2+23*u0-16*u1-7*u2)/h2;
// 		  wxxx=3/2.*(13*h*up0+32*h*up1+5*h*up2+33*u0-16*u1-17*u2)/h3;
	
// 		  if( FALSE ) 
// 		    {
// 		      // *** THIS DOES NOT WORK: WHY???
// 		      printF("--BM-- internalForceBC:  side=%i: OLD: wxx=%9.2e, wxxx=%9.2e",side,wxx,wxxx);	
// 		      // these formulas assume u=ux=0 and uxxxx=uxxxxx=0 
// 		      // wxx = -1./2.*(7.*u0-8.*u1+u2)/h2;
// 		      // wxxx = 3./2.*(3.*u0-4.*u1+u2)/h3;

// 		      wxx = -1./194*(704*h*up1+34*h*up2+1491*u0-1344*u1-147*u2)/h2;
// 		      wxxx = 3./194*(832*h*up1+49*h*up2+1233*u0-1024*u1-209*u2)/h3;
	    

// 		      printF(", new: wxx=%9.2e, wxxx=%9.2e (u0=%8.2e, up0=%8.2e)\n",wxx,wxxx,u0,up0);	

// 		      // wxx=0.; wxxx=0.;
// 		    }
// 		}
	

// 	      rhs(ia  ) +=  (is)*EI*wxxx;   // add : -E*I*wxxx(0,t) * N(0)
// 	      rhs(ia+1) += -(is)*EI*wxx;    // add : -E*I*wxx(0,t) * Np_x(0)
	
// 	    }
      

      
// 	}

//     }

//   if( allowsFreeMotion ) 
//     {
//       // --- free body motion ---
//       const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
//       const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
//       const real * bodyForce =  dbase.get<real[2]>("bodyForce");

//       //std::cout << "Total pressure force = " << totalPressureForce << std::endl;
//       linAcceleration[0] = totalPressureForce*normal[0] / totalMass + bodyForce[0] * buoyantMass / totalMass;
//       linAcceleration[1] = totalPressureForce*normal[1] / totalMass + bodyForce[1] * buoyantMass / totalMass;
//       omegadd = totalPressureMoment / totalInertia;

//       if (bcLeft == BeamModel::pinned ||
// 	  bcLeft == BeamModel::clamped) {

// 	real wend,wendslope;
// 	int elem = 0;
// 	real eta = -1.0;
// 	interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
// 	real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
// 		       centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
// 	linAcceleration[0] -= penalty*(end[0]-initialEndLeft[0])/totalMass;
// 	linAcceleration[1] -= penalty*(end[1]-initialEndLeft[1])/totalMass;
      
// 	real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/-normal[0]*L*0.5)+
// 			    (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/-normal[1]*L*0.5));
// 	omegadd -= mom / totalInertia;

// 	real shear = penalty*((end[0]-initialEndLeft[0])*(-normal[0])+
// 			      (end[1]-initialEndLeft[1])*(-normal[1]));
      
// 	rhs(0) += shear;
//       }
    
//       if (bcRight == BeamModel::pinned ||
// 	  bcRight == BeamModel::clamped) {

// 	real wend,wendslope;
// 	int elem = numElem-1;
// 	real eta = 1.0;
// 	interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
// 	real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
// 		       centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
// 	linAcceleration[0] -= penalty*(end[0]-initialEndRight[0])/totalMass;
// 	linAcceleration[1] -= penalty*(end[1]-initialEndRight[1])/totalMass;
      
// 	real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/normal[0]*L*0.5)+
// 			    (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/normal[1]*L*0.5));
// 	omegadd -= mom / totalInertia;

// 	real shear = penalty*((end[0]-initialEndRight[0])*(normal[0])+
// 			      (end[1]-initialEndRight[1])*(normal[1]));
      
// 	rhs(numElem*2) += shear;
//       }

//       if (bcLeft == BeamModel::clamped) {

// 	real wend,wendslope;
// 	int elem = 0;
// 	real eta = -1.0;
// 	interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
// 	real slopeend[2] = {normal[0]*wendslope+tangent[0],
// 			    normal[1]*wendslope+tangent[1]};
      
// 	real proj = (-tangent[0]*wendslope+initialBeamNormal[0])*normal[0] + 
// 	  (-tangent[1]*wendslope+initialBeamNormal[1])*normal[1] ;

// 	real err = slopeend[0]*initialBeamNormal[0] + 
// 	  slopeend[1]*initialBeamNormal[1] ;

// 	real mom = 0.1*penalty*err*proj;
      
// 	real rf = 1.0;
// 	real & leftCantileverMoment = dbase.get<real>("leftCantileverMoment");
// 	leftCantileverMoment = mom*rf + (1.0-rf)*leftCantileverMoment;
// 	omegadd -= leftCantileverMoment / totalInertia;

// 	std::cout << "End slope error = " << err << std::endl;
      
// 	//rhs(1) += mom; 
//       }

//       RealArray ones = f,res;
//       ones = 0.0;
//       for (int i = 0; i < numElem*2+2; i+=2)
// 	ones(i) = linAcceleration[0]*normal[0]+linAcceleration[1]*normal[1];

//       //printArray(ones,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    
//       multiplyByMassMatrix(ones, res);
//       //res *= 1.0/massPerUnitLength*totalMass;
//       rhs -= res;

//       multiplyByMassMatrix(u, res);
//       rhs += angularVelocityTilde*angularVelocityTilde*res;

//       for (int i = 0; i < numElem*2+2; i+=2) 
// 	{
// 	  ones(i) = -0.5*L+dx*(i/2); //Longfei 20160121: replace le with dx.
// 	  ones(i+1) = 1.0;
// 	}
    
//       //printArray(ones,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    
    
//       //real R = 0.0;
//       //for (int i = 0; i < numElem*2+2; ++i) 
//       //  R += f(i)*ones(i);
//       //std::cout << "R = " << R << std::endl;
    
//       //std::cout << "computed pressure moments = " << mult(evaluate(transpose(ones)),f)(0) << " " << totalPressureMoment << std::endl;

//       multiplyByMassMatrix(ones, res);

//       //std::cout << "inertias = " << mult(evaluate(transpose(ones)),res)(0) << " " << totalInertia << std::endl;

//       rhs -= res*omegadd;

//     } // end if allows free motion
  

//   if( debug & 2 )
//     {
//       rhs.reshape(2,numElem+1);
//       ::display(rhs,"-- BM -- computeAcceleration: rhs before solve Ma=rhs","%11.4e ");
//       rhs.reshape(numElem*2+2);
//     }

//   // Solve M a = rhs 
//   solveBlockTridiagonal(A, rhs, a, alpha, alphaB,tridiagonalSolverName );

//   if( debug & 2 )
//     {
//       a.reshape(2,numElem+1);
//       ::display(a,"-- BM -- computeAcceleration: solution a after solve","%11.4e ");
//       a.reshape(numElem*2+2);
//     }

//   // solveBlockTridiagonal(A, rhs, a, bcLeft,bcRight,allowsFreeMotion);
  
// }


//================================================================================================
/// \brief Compute the acceleration of the beam. (Longfei 20160210: new)
///  rhs = -B*v -K*u + f
/// \param u (input):               current beam position (For Newmark this is un + dt*vn + .5*dt^2*(1-2*beta)*an )
/// \param  (input)v:               current beam velocity (NOT USED CURRENTLY)
/// \param f (input):               external force on the beam
/// \param a (input):               beam acceleration [out]
/// \param linAcceleration (input): acceleration of the CoM of the beam (for free motion) [out]
/// \param omegadd (input):         angular acceleration of the beam (for free motion) [out]
/// \param dt (input):              time step
/// \param solverName (input) :  solverName can be "explicitSolver" or "implicitNewmarkSolver"
//
//================================================================================================
void FEMBeamModel::
computeAcceleration(const real t,
		    const RealArray& u, const RealArray& v, 
		    const RealArray& f,
		    RealArray& a,
		    real linAcceleration[2],
		    real& omegadd,
		    real dt,
                    const aString & solverName )
{
  
  // Longfei 20160121: new way of handling parameters  
  //const real EI = elasticModulus*areaMomentOfInertia;
  const real & EI = dbase.get<real>("EI");
  const real & dx = dbase.get<real>("elementLength");
  const real & L = dbase.get<real>("length");
  // For natural BC's we need EI*wxx(0,t)
  const real & T = dbase.get<real>("tension");
  const real & Kxxt = dbase.get<real>("Kxxt");

  //const real beamLength=L;
  //const real dx = beamLength/numElem;  
  const int & numElem = dbase.get<int>("numElem");
  const real & buoyantMass= dbase.get<real>("buoyantMass"); 
  const real & totalMass=  dbase.get<real>("totalMass");
  const real & totalInertia=  dbase.get<real>("totalInertia");
  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft =  boundaryConditions[0];
  const BoundaryCondition & bcRight =  boundaryConditions[1];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const real & Abar = dbase.get<real>("massPerUnitLength");

  
  
  if( debug & 2 )
    printF("-- BM%i -- BeamModel::computeAcceleration, t=%8.2e, dt=%8.2e\n",getBeamID(),t,dt);
  

  RealArray rhs=u; // Longfei 20160215: same size as u

  // Compute:   rhs = -B*v -K*u 
  computeInternalForce(u, v, rhs);


  if(debug & 2 )
    {
      rhs.reshape(2,numElem+1);
      ::display(rhs,"-- BM -- computeAcceleration: rhs after computeInternalForce","%9.2e ");
      rhs.reshape(numElem*2+2);
    }
  
  rhs += f;

  aString tridiagonalSolverName;

  if( !allowsFreeMotion ) 
    {
      // --- Apply boundary conditions to f - Ku  ----

      // --- If the boundary degrees of freedom are given  (e.g. w(0,t)=g0(t) or wx(0,t)=h0(t))
      //     then we eliminate the corresponding equation from the matrix equation (by setting it to the indentity)

      // Get two time derivatives of the boundary functions for "acceleration BC"
      RealArray g,gt,gtt;
      getBoundaryValues( t, g,  0 );
      getBoundaryValues( t, gt, 1 );
      getBoundaryValues( t, gtt,2 );

      real accelerationScaleFactor=1.;
      if( solverName=="explicitSolver")
	{
	  // explicitSolver inverts mass matrix
	  // it  is solving for Abar utt (not utt )
	  tridiagonalSolverName="massMatrixSolver";
	  accelerationScaleFactor=Abar;
	}
      else
	{
	  tridiagonalSolverName=solverName;
	}
      // Longfei 20160208: no longer need this
      // real accelerationScaleFactor=1.;
      // if( tridiagonalSolverName=="rhsSolver" )
      // 	{
      // 	  // when we compute the RHS directly we are solving for rho*hs*b utt (not utt )
      // 	  accelerationScaleFactor=density*thickness*breadth;
      // 	}
    
      for( int side=0; side<=1; side++ )
	{
	  BoundaryCondition bc = side==0 ? bcLeft : bcRight;
	  const int ia = side==0 ? 0 : numElem*2;
	  const int ib = side==0 ? ia+2 : ia-2;
	  const int is = 1-2*side;
      
	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
	  if( bc==clamped && EI==0. ) bc=pinned;

	  // real x = side==0 ? 0 : L;
	  if( bc == clamped ) 
	    {
	      // First two equations in the matrix are
	      //       w_tt = given
	      //       wx_tt = given 
	      if( false )
		{
		  printF("-- BM%i -- side=%i set clamped BC gtt=%e, gttx=%e, accelerationScaleFactor=%8.2e\n",
			 getBeamID(),gtt(0,side),gtt(1,side),accelerationScaleFactor);
		}
	
	      rhs(ia  )=gtt(0,side)*accelerationScaleFactor;   // w_tt is given
	      rhs(ia+1)=gtt(1,side)*accelerationScaleFactor;   // wxtt is given 
	    }
	  else if( bc==pinned )
	    {
	      rhs(ia)=gtt(0,side)*accelerationScaleFactor;   // w_tt is given

	      if( bc == pinned && EI != 0.) 
		{
		  // Boundary term is of the form:  -EI* v_x*w_xx
		  // -- correct for natural BC:  E*I*w_xx = +/- g(2,side)
		  if( debug & 1 )	printF("-- BM%i -- set rhs for pinned BC wxx = g(2,side)=%8.2e, EI=%g\n",getBeamID(),g(2,side),EI);
		  rhs(ia+1) += -(is)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)
		}
	    }
	  else if( bc==slideBC )
	    {
	      // ---- slide BC ---
	      // Equation 2 in matrix:    wx_tt = given 
	      rhs(ia+1)=gtt(1,side)*accelerationScaleFactor;   // wxtt is given 

	      // Equation 1 is adjusted:
	      rhs(ia  ) +=  (is)*EI*g(3,side);                 // add : -E*I*wxxx(0,t) * N(0)
	      rhs(ia  ) += -(is)*T *g(1,side);                 // add :  T wx(0,t)*N(0) 
	      rhs(ia  ) += -(is)*Kxxt*gt(1,side);               // add :  Kxxt*wxt(0,t)
	    }
	  else if( bc==freeBC )
	    {
	      // Boundary terms are of the form:  T*v*w_x  -EI* v*w_xxx - EI* v_x*w_xx
	      // Free BC: wxx=EI* g(2,side), w_xxx= EI*g(3,side)

	      // printF("-- BM -- set rhs for free BC, g(2)=%e, g(3)=%e, T*u(ia+1)=%8.2e \n",g(2,side),g(3,side),T*u(ia+1));

	      rhs(ia  ) +=  (is)*EI*g(3,side);   // add : -E*I*wxxx(0,t) * N(0)
	      rhs(ia+1) += -(is)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)

	      // The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
	      // ***CHECK ME*** IS THIS RIGHT?
	      rhs(ia  ) += -(is)*T*u(ia+1);         // add : T*N1(0)*Np_1_x(0)*w'_1
	      rhs(ia  ) += -(is)*Kxxt*v(ia+1);      // add : T*N1(0)*Np_1_x(0)*wxt_1

	    }
	  else if( bc==internalForceBC )
	    { // BC used when computing the "internal force"  F = L(u,v) + f , given (u,v)
	      rhs(ia  ) += -(is)*T*u(ia+1);      // add : T*N1(0)*Np_1_x(0)*w'_1
	      rhs(ia  ) += -(is)*Kxxt*v(ia+1);      // add : T*N1(0)*Np_1_x(0)*w'_1

	      // evaluate wxx and wxxx on the boundary
	      // Longfei 20160210: dx is already avaible.
	      // const real beamLength=L;
	      // const real dx = beamLength/numElem;  
	      real wxxx, wxx;
	      if( FALSE )  
		{
		  const real dxidx = 2./dx;  // d(xi)/dx
		  // find the 2nd and third derivatives of the basis functions at the ends
		  real phijxx = -1.5*dxidx*dxidx, phijxxx=1.5*dxidx*dxidx*dxidx;
		  real psijxx = -dx*dxidx*dxidx,  psijxxx=.75*dx*dxidx*dxidx*dxidx;
	
		  // Here is wxx to 2nd-order and wxxx to first order accuracy
		  // This formula is the same as found from Taylor series
		  wxx  = (u(ia)- u(ib))*phijxx  + (is)*( u(ia+1) +.5*u(ib+1) )*psijxx;
		  wxxx = (is)*(u(ia)- u(ib) )*phijxxx + (u(ia+1)+u(ib+1))*psijxxx;

		  if( false )
		    {
		      printF(" side=%i: phijxx=%9.3e, phijxxx=%9.3e  dx=%8.2e, 1/dx=%8.2e\n",side,phijxx,phijxxx,dx,1/dx);
		      printF(" side=%i: psijxx=%9.3e, psijxxx=%9.3e\n",side,psijxx,psijxxx);
		      printF(" side=%i: (u,u')(ia)=(%e,%e) (u,u')(ib)=%e,%e)\n",u(ia),u(ia+1),u(ib),u(ib+1));
		      printF(" side=%i: wxx=%9.3e, wxxx=%9.3e\n",side,wxx,wxxx);
		    }
		}
	      else
		{
		  // **NEW** 
		  // From cgDoc/moving/codes/beam/interp.maple
		  // 4-order in upp, 2nd-order in uppp: 
		  const int ic = ib+2*is; // 2nd point inside
		  real h = is*dx, h2=h*h, h3=h2*h;
		  real u0=u(ia),     u1=u(ib),    u2=u(ic);
		  real up0=u(ia+1), up1=u(ib+1), up2=u(ic+1);
		  // wxx =-1/50.*(244*h*up0+176*h*up1-6*h*up2+407*u0-400*u1-7*u2)/h2;
		  // wxxx=3/50.*(189*h*up0+256*h*up1-11*h*up2+417*u0-400*u1-17*u2)/h3;
		  wxx =-1/2.*(12*h*up0+16*h*up1+2*h*up2+23*u0-16*u1-7*u2)/h2;
		  wxxx=3/2.*(13*h*up0+32*h*up1+5*h*up2+33*u0-16*u1-17*u2)/h3;
	
		  if( FALSE ) 
		    {
		      // *** THIS DOES NOT WORK: WHY???
		      printF("-- BM%i -- internalForceBC:  side=%i: OLD: wxx=%9.2e, wxxx=%9.2e",getBeamID(),side,wxx,wxxx);	
		      // these formulas assume u=ux=0 and uxxxx=uxxxxx=0 
		      // wxx = -1./2.*(7.*u0-8.*u1+u2)/h2;
		      // wxxx = 3./2.*(3.*u0-4.*u1+u2)/h3;

		      wxx = -1./194*(704*h*up1+34*h*up2+1491*u0-1344*u1-147*u2)/h2;
		      wxxx = 3./194*(832*h*up1+49*h*up2+1233*u0-1024*u1-209*u2)/h3;
	    

		      printF(", new: wxx=%9.2e, wxxx=%9.2e (u0=%8.2e, up0=%8.2e)\n",wxx,wxxx,u0,up0);	

		      // wxx=0.; wxxx=0.;
		    }
		}
	

	      rhs(ia  ) +=  (is)*EI*wxxx;   // add : -E*I*wxxx(0,t) * N(0)
	      rhs(ia+1) += -(is)*EI*wxx;    // add : -E*I*wxx(0,t) * Np_x(0)
	
	    }
      

      
	}

    }

  if( allowsFreeMotion ) 
    {
      const int & current = dbase.get<int>("current"); 
      std::vector<RealArray> & ua = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
      RealArray & uc = ua[current];  // current displacement 

      // --- free body motion ---
      const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
      const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
      const real * bodyForce =  dbase.get<real[2]>("bodyForce");

      //std::cout << "Total pressure force = " << totalPressureForce << std::endl;
      linAcceleration[0] = totalPressureForce*normal[0] / totalMass + bodyForce[0] * buoyantMass / totalMass;
      linAcceleration[1] = totalPressureForce*normal[1] / totalMass + bodyForce[1] * buoyantMass / totalMass;
      omegadd = totalPressureMoment / totalInertia;

      if (bcLeft == BeamModel::pinned ||
	  bcLeft == BeamModel::clamped) {

	real wend,wendslope;
	int elem = 0;
	real eta = -1.0;
	interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
	real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		       centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
	linAcceleration[0] -= penalty*(end[0]-initialEndLeft[0])/totalMass;
	linAcceleration[1] -= penalty*(end[1]-initialEndLeft[1])/totalMass;
      
	real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/-normal[0]*L*0.5)+
			    (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/-normal[1]*L*0.5));
	omegadd -= mom / totalInertia;

	real shear = penalty*((end[0]-initialEndLeft[0])*(-normal[0])+
			      (end[1]-initialEndLeft[1])*(-normal[1]));
      
	rhs(0) += shear;
      }
    
      if (bcRight == BeamModel::pinned ||
	  bcRight == BeamModel::clamped) {

	real wend,wendslope;
	int elem = numElem-1;
	real eta = 1.0;
	interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
	real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		       centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
	linAcceleration[0] -= penalty*(end[0]-initialEndRight[0])/totalMass;
	linAcceleration[1] -= penalty*(end[1]-initialEndRight[1])/totalMass;
      
	real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/normal[0]*L*0.5)+
			    (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/normal[1]*L*0.5));
	omegadd -= mom / totalInertia;

	real shear = penalty*((end[0]-initialEndRight[0])*(normal[0])+
			      (end[1]-initialEndRight[1])*(normal[1]));
      
	rhs(numElem*2) += shear;
      }

      if (bcLeft == BeamModel::clamped) {

	real wend,wendslope;
	int elem = 0;
	real eta = -1.0;
	interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
	real slopeend[2] = {normal[0]*wendslope+tangent[0],
			    normal[1]*wendslope+tangent[1]};
      
	real proj = (-tangent[0]*wendslope+initialBeamNormal[0])*normal[0] + 
	  (-tangent[1]*wendslope+initialBeamNormal[1])*normal[1] ;

	real err = slopeend[0]*initialBeamNormal[0] + 
	  slopeend[1]*initialBeamNormal[1] ;

	real mom = 0.1*penalty*err*proj;
      
	real rf = 1.0;
	real & leftCantileverMoment = dbase.get<real>("leftCantileverMoment");
	leftCantileverMoment = mom*rf + (1.0-rf)*leftCantileverMoment;
	omegadd -= leftCantileverMoment / totalInertia;

	std::cout << "End slope error = " << err << std::endl;
      
	//rhs(1) += mom; 
      }

      RealArray ones = f,res;
      ones = 0.0;
      for (int i = 0; i < numElem*2+2; i+=2)
	ones(i) = linAcceleration[0]*normal[0]+linAcceleration[1]*normal[1];

      //printArray(ones,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    
      multiplyByMassMatrix(ones, res);
      //res *= 1.0/massPerUnitLength*totalMass;
      rhs -= res;

      multiplyByMassMatrix(u, res);
      rhs += angularVelocityTilde*angularVelocityTilde*res;

      for (int i = 0; i < numElem*2+2; i+=2) 
	{
	  ones(i) = -0.5*L+dx*(i/2); //Longfei 20160121: replace le with dx.
	  ones(i+1) = 1.0;
	}
    
      //printArray(ones,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    
    
      //real R = 0.0;
      //for (int i = 0; i < numElem*2+2; ++i) 
      //  R += f(i)*ones(i);
      //std::cout << "R = " << R << std::endl;
    
      //std::cout << "computed pressure moments = " << mult(evaluate(transpose(ones)),f)(0) << " " << totalPressureMoment << std::endl;

      multiplyByMassMatrix(ones, res);

      //std::cout << "inertias = " << mult(evaluate(transpose(ones)),res)(0) << " " << totalInertia << std::endl;

      rhs -= res*omegadd;

    } // end if allows free motion
  

  if( debug & 2 )
    {
      rhs.reshape(2,numElem+1);
      ::display(rhs,"-- BM -- computeAcceleration: rhs before solve Ma=rhs","%11.4e ");
      rhs.reshape(numElem*2+2);
    }

  // Solve M a = rhs, M is associated with  tridiagonalSolverName
  solveBlockTridiagonal(rhs, a, tridiagonalSolverName );
  if(tridiagonalSolverName=="massMatrixSolver")
    {
      a/=Abar; // massMatrixSolver solves Abar*utt
    }

  
  if( debug & 2 )
    {
      a.reshape(2,numElem+1);
      ::display(a,"-- BM -- computeAcceleration: solution a after solve","%11.4e ");
      a.reshape(numElem*2+2);
    }

  // solveBlockTridiagonal(A, rhs, a, bcLeft,bcRight,allowsFreeMotion);
  
}







// ===================================================================================================
/// \brief  Return the nodal force values on the beam reference line. These are computed from
///   the current local element force integrals: (phi_i,f) and (psi,f) 
///  \param force (output) : force components on the beam reference-line.
// ====================================================================================================
void FEMBeamModel::
getForceOnBeam( const real t, RealArray & force )
{
  //Longfei 20160121: new way of handling parameters
  const real & le = dbase.get<real>("elementLength");
  //real le = L / numElem;
  const int & numElem = dbase.get<int>("numElem");
  const int & Abar = dbase.get<real>("massPerUnitLength");
  BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  BoundaryCondition & bcLeft = boundaryConditions[0];
  BoundaryCondition & bcRight = boundaryConditions[1];
  
  
  const int & current = dbase.get<int>("current"); 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force in elemnt integral form 

  RealArray & fc = f[current];  // force at current time
  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
    {
      printF("-- BM%i -- BeamModel::getForceOnBeam:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     getBeamID(),t,time(current),current);
      OV_ABORT("ERROR");
    }

  // On entry:
  //    fc holds : (phi_i,f) (psi_i,f)

  // Compute: 
  //  force  = SUM_j {  f_j phi_j(x) + f_j' psi_j(x) }
  // by solving
  //     SUM_j {  f_j (phi_i(x),phi_j(x)) + f_j' (phi_i(x),psi_j(x)) }  = (phi_i,f)
  //     SUM_j {  f_j (psi_i(x),phi_j(x)) + f_j' (psi_i(x),psi_j(x)) }  = (psi_i,f)

  // Longfei 20160208: this is redundant. elementMass = elementM/massPerUnitLength
  // old: no need for this. factorTridiangonalSolver takes care of this now
  // real le2 = le*le;
  // real le3 = le2*le;
  // RealArray elementMass(4,4);
  
  // elementMass(0,0) = elementMass(2,2) = 13./35.*le;
  // elementMass(0,1) = elementMass(1,0) = 11./210.*le2;
  // elementMass(0,2) = elementMass(2,0) = 9./70.*le;
  // elementMass(0,3) = elementMass(3,0) = -13./420.*le2;
  // elementMass(1,1) = elementMass(3,3) = 1./105.*le3;
  // elementMass(1,2) = elementMass(2,1) = 13./420.*le2;
  // elementMass(1,3) = elementMass(3,1) = -1./140.*le3;
  // elementMass(3,2) = elementMass(2,3) = -11./210.*le2;

  // ---- Boundary Conditions ----
  BoundaryCondition bcLeftSave =bcLeft;  // save current 
  BoundaryCondition bcRightSave=bcRight;

  // We want no boundary conditions to be applied so set:
  // Longfei 20160209:
  // factorTridiangonalSolver makes sure the makes not changed for this case
  // make bcLeft, bcRight= unknonwBC to make sure the RHS do not get changed
  bcLeft=unknownBC;
  bcRight=unknownBC;

  // Note: We use a separate TridiagonalSolver for this Galerkin projection: 
  RealArray ff(2*numElem+2);
  // real alpha=0., alphaB=0.; // coefficients of stiffness and damping matrices
  //solveBlockTridiagonal(elementMass, fc, ff, alpha,alphaB, "galerkinProjection" );
  //Longfei20160209: new
  solveBlockTridiagonal(fc, ff, "massMatrixSolverNoBC" );

  bcLeft =bcLeftSave;   // reset 
  bcRight=bcRightSave; 


  force.redim(numElem+1);
  force=ff(Range(0,2*numElem,2));  // extract out nodal force values, every second value
  
  if( false )
    {
      ::display(force,sPrintF("-- BM -- getForceOnBeam: force at t=%9.3e",t),"%8.2e ");
    }
  
}






// ================================================================================
/// \brief Solve A u = f
/// 
/// \param Ae (input) : "element" matrix for A 
/// \param alpha (input) coefficient of Ke in A (used in adjusting the matrix for boundary terms)
/// \param alphaB (input) coefficient of Be in A (used in adjusting the matrix for boundary terms)
/// \param tridiagonalSolverName (input) : name of the tridiagonal solver
//
//     Ae = Me + alphaB*Be + alpha*Ke 
//     Me = element mass matrix 
//     Ke = element stiffness matrix 
// ================================================================================
// void FEMBeamModel::
// solveBlockTridiagonal(const RealArray& Ae, const RealArray& f, RealArray& u, 
//                       const real alpha, const real alphaB, const aString & tridiagonalSolverName )
// {

//   const bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
//   bool & refactor = dbase.get<bool>("refactor");
//   const real & EI = dbase.get<real>("EI");
//   const int & numElem = dbase.get<int>("numElem");
//   const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
//   const BoundaryCondition & bcLeft = boundaryConditions[0];
//   const BoundaryCondition & bcRight = boundaryConditions[1];
//   const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  


//   const bool isPeriodic = bcLeft==periodic;
//   if( isPeriodic ) 
//     { // consistency check:
//       assert( bcRight==periodic );
//     }
  
//   bool checkResidual= (debug & 1 ) && refactor;  // for testing block tridiagonal solver 
//   bool useBoth=(debug & 1 ) && useNewTridiagonalSolver && !isPeriodic;  // check new solver with old

//   const real & T = dbase.get<real>("tension");
//   const real & Kxxt = dbase.get<real>("Kxxt");

//   RealArray lower(2,2), upper(2,2), diag1(2,2), diag2(2,2);

//   // int numElem = f.getLength(0)/2-1;

//   //  Ae = [ a00 a01 | a02 a03 ]
//   //       [ a10 a11 | a12 a13 ]
//   //       [ -------- ---------]  
//   //       [ a20 a21 | a22 a23 ]
//   //       [ a30 a31 | a32 a33 ]

//   // upper = upper right quad
//   upper(0,0) = Ae(0,2); upper(0,1) = Ae(0,3);
//   upper(1,0) = Ae(1,2); upper(1,1) = Ae(1,3);

//   // lower = lower left quad (= upper^T  since Me = Me^T
//   lower(0,0) = Ae(2,0); lower(0,1) = Ae(2,1);
//   lower(1,0) = Ae(3,0); lower(1,1) = Ae(3,1);

//   // RealArray upperT(2,2);
//   // upperT = trans(upper);

//   // ::display(upper ,"-- BM -- solveBlockTridiagonal: upper","%8.2e ");
//   // ::display(upperT,"-- BM -- solveBlockTridiagonal: upperT","%8.2e ");

//   diag1(0,0) = Ae(0,0); diag1(0,1) = Ae(0,1);
//   diag1(1,0) = Ae(1,0); diag1(1,1) = Ae(1,1);

//   diag2(0,0) = Ae(2,2); diag2(0,1) = Ae(2,3);
//   diag2(1,0) = Ae(3,2); diag2(1,1) = Ae(3,3);
  
//   RealArray dd(2,2);
//   dd = diag1+diag2;

//   // Solve the block tridiagonal system: 
//   //     [ D2[0] D3[0]                         ]
//   //     [ D1[0] D2[1] D3[1]                   ]
//   //     [       D1[1] D2[2] D3[2]             ]
//   //     [                                     ]
//   //     [                  ...    ...         ]
//   //     [          D1[ne-1] D2[ne-1] D3[ne-1] ]
//   //     [                   D1[ne-1] D2[ne]   ]

//   RealArray uNew;
  
//   if( useBoth || useNewTridiagonalSolver )
//     {
//       // -- For periodic systems we do not include the last point in the system --
//       //       x----+----+----+   ... ----+----x
//       //       0    1    2               nTri  numElem
//       //                                  
//       //    u(0) = u(numElem)
//       // 
//       int nTri = numElem;
//       if( isPeriodic ) nTri=numElem-1;

//       Index  I1=Range(0,nTri), I2=Range(0,0);
//       // Range I1=Range(0,numElem), I2=Range(0,0);
//       // Range I1(0,numElem), I2(0,0);
//       const int ndof=2;  // number of degrees of freedom per node 

//       // TridiagonalSolver::periodic
//       const TridiagonalSolver::SystemType systemType = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::normal;
    
//       // TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>("tridiagonalSolver");
//       TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>(tridiagonalSolverName);
//       if( pTri==NULL )
// 	{
// 	  pTri = new TridiagonalSolver();
// 	  refactor=true;
// 	}
    
//       assert( pTri!=NULL );

//       TridiagonalSolver & tri = *pTri;

//       RealArray at0(ndof,ndof,I1,I2), bt0(ndof,ndof,I1,I2), ct0(ndof,ndof,I1,I2); // save for checking
//       if( refactor )
// 	{
// 	  if( true || debug & 1 )
// 	    printF("-- BM -- solveBlockTridiagonal : name=[%s] form block tridiagonal system and factor, isPeriodic=%i\n",
// 		   (const char*)tridiagonalSolverName, (int)isPeriodic);
      
// 	  RealArray at(ndof,ndof,I1,I2), bt(ndof,ndof,I1,I2), ct(ndof,ndof,I1,I2);

// 	  Index D=Range(0,1); // =ndof;
// 	  for( int i=0; i<=nTri; i++ ) 
// 	    {
// 	      if( i>0 || isPeriodic )
// 		at(D,D,i,0) = lower;  // lower diagonal 
// 	      else 
// 		at(D,D,i,0)=0.;   

// 	      // diagonal :
// 	      if( i==0 && !isPeriodic )
// 		{
// 		  bt(D,D,i,0) = diag1;
// 		}
// 	      else if( i==nTri && !isPeriodic )
// 		{
// 		  bt(D,D,i,0) = diag2;
// 		}
// 	      else
// 		{
// 		  bt(D,D,i,0) = dd;      
// 		}
	
// 	      if( i<nTri || isPeriodic )
// 		ct(D,D,i,0) = upper;   // upper diagonal 
// 	      else 
// 		ct(D,D,i,0) = 0.;     


// 	    }  // end for i 
      
// 	  // -- Boundary fixup ---
// 	  if( !allowsFreeMotion )
// 	    {
// 	      // --- Boundary conditions ---
// 	      // Adjust the matrix for essential BC's -- these will set the DOF's at boundaries
// 	      for( int side=0; side<=1; side++ )
// 		{
// 		  BoundaryCondition bc = side==0 ? bcLeft : bcRight;
// 		  int ia = side==0 ? 0 : numElem;

// 		  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
// 		  // const real EI = elasticModulus*areaMomentOfInertia;
// 		  if( bc==clamped && EI==0. ) bc=pinned;
	  
// 		  if( bc == clamped ) 
// 		    {
// 		      // Replace first block of equations by the identity
// 		      bt(0,0,ia,0)=1.; bt(0,1,ia,0)=0.;
// 		      bt(1,0,ia,0)=0.; bt(1,1,ia,0)=1.; 
// 		      if( side==0 )
// 			ct(D,D,ia,0)=0.;
// 		      else
// 			at(D,D,ia,0)=0.;
// 		    }
// 		  else if( bc == pinned ) 
// 		    {
// 		      // replace first equation in first 2x2 block by the identity
// 		      if( side==0 )
// 			ct(0,D,ia,0)=0.; 
// 		      else
// 			at(0,D,ia,0)=0.;
	    
// 		      bt(0,0,ia,0)=1.; bt(0,1,ia,0)=0.; 
// 		    }
// 		  else if( bc == slideBC ) 
// 		    {
// 		      // replace second equation in first 2x2 block by the identity
// 		      if( side==0 )
// 			ct(1,D,ia,0)=0.; 
// 		      else
// 			at(1,D,ia,0)=0.;
	    
// 		      bt(1,0,ia,0)=0.; bt(1,1,ia,0)=1.; 
// 		    }
// 		  else if( bc == freeBC )
// 		    {
// 		      // --- correct the stiffnes matrix for a free BC
// 		      // The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
// 		      //  T*N1(0)*Np_1_x(0)*w'_1
            
// 		      // -- freeBC is not allowed with string model ---
// 		      if(  EI==0. )
// 			{
// 			  printF("-- BM -- ERROR: A `free' BC is not allowed with the string model for a beam, EI=0\n");
// 			  OV_ABORT("ERROR");
// 			}
	    
// 		      bt(0,1,ia,0) +=  (1-2*side)*T*alpha;

// 		      // The boundary term K_xxt*v*w_xt also contributes
// 		      bt(0,1,ia,0) +=  (1-2*side)*Kxxt*alphaB;

// 		    }
	  

// 		}
// 	    }

// 	  at0=at; bt0=bt; ct0=ct;
      
// 	  // Factor the block tridiagonal system:
// 	  tri.factor(at,bt,ct,systemType,axis1,ndof);

// 	} // end factor
    
//       // -- rhs --

//       RealArray xTri(ndof,I1,I2);
//       for( int i=0; i<=nTri; i++ ) 
// 	{
// 	  xTri(0,i,0)=f(i*2);
// 	  xTri(1,i,0)=f(i*2+1);
// 	}
      
//       // solve the block tridiagonal system: 
//       tri.solve(xTri);

//       // assign the solution 
//       for( int i = 0; i<=nTri; i++ ) 
// 	{
// 	  u(i*2) = xTri(0,i,0);
// 	  u(i*2+1)=xTri(1,i,0);
// 	}
//       if( isPeriodic )
// 	{ // -- assign values on last node
// 	  int i=numElem;
// 	  u(i*2) = u(0); u(i*2+1) = u(1);
// 	}
    

//       if( useBoth )
// 	uNew=u;
    
//       if( refactor && checkResidual )
// 	{
// 	  // double check solution:
// 	  real resid=0.;
// 	  for( int i = 0; i<=nTri; i++ ) 
// 	    {
// 	      // resid = at*u[i-1] + bt*u[i] + ct*u[i+1];
// 	      int im1=max(0,i-1), ip1=min(i+1,numElem);
// 	      if( i==0       && isPeriodic ){ im1=numElem-1; } // perioidic case : i=0 <-> i=numElem
// 	      if( i==numElem && isPeriodic ){ ip1=1; }         // perioidic case : i=0 <-> i=numElem

// 	      real r=0.;
// 	      r += at0(0,0,i)*u(2*im1) + at0(0,1,i)*u(2*im1+1) + bt0(0,0,i)*u(2*i) + bt0(0,1,i)*u(2*i+1) + ct0(0,0,i)*u(2*ip1) + ct0(0,1,i)*u(2*ip1+1) -f(2*i);
// 	      r += at0(1,0,i)*u(2*im1) + at0(1,1,i)*u(2*im1+1) + bt0(1,0,i)*u(2*i) + bt0(1,1,i)*u(2*i+1) + ct0(1,0,i)*u(2*ip1) + ct0(1,1,i)*u(2*ip1+1) -f(2*i+1);

// 	      // printF("BT: i=%i resid=%e\n",i,r);
	
// 	      resid=max(resid,r);
// 	    }
// 	  printF("--BM-- BLOCK-TRI : max-residual =%8.2e\n",resid);
// 	  if( resid > REAL_EPSILON*1000.*SQR(numElem) )
// 	    {
// 	      OV_ABORT("error");
// 	    }
      

// 	}
    
//       refactor=false;

//     }

//   if( useBoth || !useNewTridiagonalSolver )
//     {
//       // ** old way ***

//       u = f;

//       std::vector< RealArray > diagonal(numElem+1,dd),
// 	superdiagonal(numElem,upper),subdiagonal(numElem, lower);
  
//       diagonal[0] = diag1;
//       diagonal[numElem] = diag2;

//       if( !allowsFreeMotion )
// 	{
// 	  // --- Boundary conditions ---
// 	  //const real EI = elasticModulus*areaMomentOfInertia;
// 	  BoundaryCondition bc = bcLeft;
// 	  if( bc==clamped && EI==0. ) bc=pinned;

// 	  if( bc == BeamModel::clamped )
// 	    {
// 	      diagonal[0](0,0) = diagonal[0](1,1) = 1.0;
// 	      diagonal[0](0,1) = diagonal[0](1,0) = 0.0;
// 	      superdiagonal[0](0,0) = superdiagonal[0](0,1) = 0.0;
// 	      superdiagonal[0](1,1) = superdiagonal[0](1,0) = 0.0;

// 	    }
// 	  else if ( bc == BeamModel::pinned ) 
// 	    {
// 	      // replace first equation in first 2x2 block by the identity
// 	      diagonal[0](0,0) = 1.0;
// 	      diagonal[0](0,1) = 0.0;
// 	      superdiagonal[0](0,0) = 0.0;
// 	      superdiagonal[0](0,1) = 0.0;
// 	    }
// 	  if( bc == freeBC )
// 	    {
// 	      // --- correct the stiffnes matrix for a free BC
// 	      // The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
// 	      //  T*N1(0)*Np_1_x(0)*w'_1
            
// 	      diagonal[0](0,1) +=  T*alpha;
// 	      diagonal[0](0,1) +=  Kxxt*alphaB;
// 	    }
 
// 	  bc = bcRight;
// 	  if( bc==clamped && EI==0. ) bc=pinned;

// 	  if (bc == BeamModel::clamped ) 
// 	    {
// 	      diagonal[numElem](0,0) = diagonal[numElem](1,1) = 1.0;
// 	      diagonal[numElem](0,1) = diagonal[numElem](1,0) = 0.0;
// 	      subdiagonal[numElem-1](0,0) = subdiagonal[numElem-1](0,1) = 0.0;
// 	      subdiagonal[numElem-1](1,1) = subdiagonal[numElem-1](1,0) = 0.0;

// 	    }
// 	  if (bc == pinned ) 
// 	    {
// 	      // replace "first" equation in last 2x2 block by the identity
// 	      diagonal[numElem](0,0) = 1.0;
// 	      diagonal[numElem](0,1) = 0.0;
// 	      subdiagonal[numElem-1](0,0) = 0.0;
// 	      subdiagonal[numElem-1](0,1) = 0.0;
// 	    }
// 	  if( bc == freeBC )
// 	    {
// 	      // --- correct the stiffnes matrix for a free BC
// 	      // The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
// 	      //  T*N1(0)*Np_1_x(0)*w'_1

// 	      // printF("-- BM -- solveBlock : add correction term, alpha=%g,  T*alpha = %8.2e\n",alpha, -T*alpha);
	
// 	      diagonal[numElem](0,1) +=  -T*alpha;
// 	      diagonal[numElem](0,1) +=  -Kxxt*alphaB;
// 	    }


// 	}
  

//       RealArray inv;

//       Index i2x2(0,2);
  
//       for (int i = 0; i < numElem; ++i) 
// 	{

// 	  inverse2x2(diagonal[i], inv);
// 	  superdiagonal[i] = mult(inv, superdiagonal[i]);
// 	  u(i2x2) = mult(inv, u(i2x2) );
// 	  u(i2x2+2) -= mult(subdiagonal[i],u(i2x2));
// 	  diagonal[i+1] -= mult(subdiagonal[i],superdiagonal[i]);   

// 	  //   if (augmented) {

// 	  // 	(*augmentedCol)(i2x2) = mult(inv , (*augmentedCol)(i2x2));
// 	  // 	(*augmentedCol)(i2x2+2) -= mult(subdiagonal[i], (*augmentedCol)(i2x2));

// 	  // 	(*augmentedRow)(i2x2+2) -= mult((*augmentedRow)(i2x2),superdiagonal[i]);

// 	  // 	*augmentedDiagonal -= mult( (*augmentedRow)(i2x2), (*augmentedCol)(i2x2) )(0);
// 	  // 	*augmentedRHS -= mult( (*augmentedRow)(i2x2), u(i2x2))(0);
// 	  // }

// 	  i2x2 += 2;
// 	}
//       // if (augmented) {
//       //   *augmentedSolution = *augmentedRHS / *augmentedDiagonal;

//       //   for (int i = numElem*2+1; i >= 0; --i) {

//       // 	u(i) -= (*augmentedCol)(i)*(*augmentedSolution);
//       //   }
//       //  }

//       inverse2x2(diagonal[numElem], inv);
//       u(i2x2) = mult(inv, u(i2x2) );
  
//       i2x2 -= 2;

//       for (int i = numElem-1; i >= 0; --i) {

// 	u(i2x2) -= mult(superdiagonal[i], u(i2x2+2));

// 	i2x2 -= 2;
//       }
//     }

//   if( useBoth )
//     {
//       real err = max(fabs(u-uNew));
//       printF("--BM-- Block tridiagonal |new - old|=%8.2e\n",err);
    
//       if( err >  REAL_EPSILON*1000.*SQR(numElem) )
// 	{
// 	  printF("*********** ERROR: old and new are different! ***************\n");
// 	  printF(" NOTE: this difference could be due to the boundary conditions not being\n"
// 		 "       fully implemented in the old scheme\n");
// 	  OV_ABORT("error");
// 	}
//     }
  


// }






// =========================================================================================
/// \brief Add internal forces such as buoyancy and TZ forces
///
/// add the internal forces to  the element force vectors 
// =========================================================================================
void FEMBeamModel::
addInternalForces( const real t, RealArray & f )
{
  //Longfei 20160214: new way

  //call the base version to get nodal values of f;
  RealArray fNodal=f; 
  BeamModel::addInternalForces( t,  fNodal ); 


  const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  const int & numElem = dbase.get<int>("numElem");
  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft = boundaryConditions[0];
  const BoundaryCondition & bcRight = boundaryConditions[1];
  
  // convert nodal values to FEM load vector:
  RealArray lt(4); // local traction
  for ( int i = 0; i<numElem; i++ )
    {
      // computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);
      
      if( orderOfGalerkinProjection==2  || exactSolutionOption=="travelingWaveFSI")
	computeProjectedForce( fNodal(2*i),fNodal(2*(i+1)), -1.0,1.0, lt);
      else
	computeGalerkinProjection( fNodal(2*i),fNodal(2*i+1), fNodal(2*(i+1)),fNodal(2*(i+1)+1),   -1.0,1.0, lt);
      
      Index idx(i*2,4);
      f(idx) += lt;
    }
  
  const bool isPeriodic = bcLeft==periodic;
  if( isPeriodic )
    {
      Index Is(0,2), Ie(2*numElem,2);
      f(Is) += f(Ie);
      f(Ie) = f(Is);
    }
  

  // Old way:
  // // if( false )
  // // { 
  // //   f=0.; // ******************************************************* TEST 
  // // }

  // //Longfei 20160121: new way of handling parameters
  // //const real beamLength=L;
  // const real &beamLength = dbase.get<real>("length");
  // const real &dx = dbase.get<real>("elementLength");
  // //const real EI = elasticModulus*areaMomentOfInertia;
  // const real & EI = dbase.get<real>("EI");
  // const real & density = dbase.get<real>("density");
  // const real & thickness = dbase.get<real>("thickness");
  // const int & numElem = dbase.get<int>("numElem");
  // const real & buoyantMassPerUnitLength = dbase.get<real>("buoyantMassPerUnitLength");
  // const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  // const BoundaryCondition & bcLeft = boundaryConditions[0];
  // const BoundaryCondition & bcRight = boundaryConditions[1];
  // const real & projectedBodyForce= dbase.get<real>("projectedBodyForce");
  // const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  
  // if( exactSolutionOption=="travelingWaveFSI" )
  //   {
  //     // add forces for the FSI traveling wave solution
  //     Index I1,I2,I3;
  //     I1=Range(0,numElem); I2=0; I3=0;

  //     RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
    
  //     // Longfei 20160121: old way
  //     //const real beamLength=L;
  //     //const real dx=beamLength/numElem;
    
  //     real heightFluidRegion=1.;
  //     for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
  // 	{
  // 	  x(i1,0,0,0) = i1*dx; 
  // 	  x(i1,0,0,1) = heightFluidRegion;    // should match value in travelingWaveFsi
  // 	}

  //     assert( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")!=NULL );
  //     TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

  //     RealArray ufe(I1,I2,I3,3);  // holds (p,v1f,v2f)

  //     // Evaluate the exact fluid solution on the interface
  //     travelingWaveFsi.getExactFluidSolution( ufe, t, x, I1,I2,I3 );

  //     // ::display(ufe(I1,0,0,0),sPrintF(" Exact fluid pressure at t=%8.2e",t),"%8.2e ");

  //     RealArray lt(4); // local traction
  //     const int pc=0;
  //     for ( int i = 0; i<numElem; i++ )
  // 	{
  // 	  real p0=ufe(i,0,0,pc), p1=ufe(i+1,0,0,pc);
  // 	  computeProjectedForce( p0,p1, -1.0,1.0, lt);
  // 	  Index idx(i*2,4);
  // 	  f(idx) += lt;
  // 	}

  //   }
  

  // const int & domainDimension = dbase.get<int>("domainDimension");
  // const bool & twilightZone = dbase.get<bool>("twilightZone");
  // if( twilightZone )
  //   {
  //     OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");
  //     Index I1,I2,I3;
  //     I1=Range(0,numElem); I2=0; I3=0;

  //     RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
  //     //const real dx=beamLength/numElem; // Longfei 20160120: dx  already defined in this function
  //     for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
  // 	{
  // 	  x(i1,0,0,0) = i1*dx; 
  // 	  x(i1,0,0,1) = 0.;    // should this be y0 ?
  // 	}



  //     const real & T = dbase.get<real>("tension");
  //     const real & K0 = dbase.get<real>("K0");
  //     const real & Kt = dbase.get<real>("Kt");
  //     const real & Kxxt = dbase.get<real>("Kxxt");

  //     RealArray ue(I1,I2,I3,1), utte(I1,I2,I3,1), uxxe(I1,I2,I3,1), uxxxxe(I1,I2,I3,1);
  //     int isRectangular=0;
  //     const int wc=0;
  //     exact.gd( ue     ,x,domainDimension,isRectangular,0,0,0,0,I1,I2,I3,wc,t );
  //     exact.gd( utte   ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );
  //     exact.gd( uxxe   ,x,domainDimension,isRectangular,0,2,0,0,I1,I2,I3,wc,t );
  //     exact.gd( uxxxxe ,x,domainDimension,isRectangular,0,4,0,0,I1,I2,I3,wc,t );

  //     // printF("-- BM -- addInternalForce: t=%9.3e max(fabs(f))=%8.2e, |utte|=%8.2e |u_xxxxe|=%8.2e\n",
  //     //        t,max(fabs(f)),max(fabs(utte)),max(fabs(uxxxxe)));

  //     RealArray ftz(I1,I2,I3,1), ftzx(I1,I2,I3,1); 
  //     ftz = (density*thickness)*utte + K0*ue - (T)*uxxe + (EI)*uxxxxe;

  //     const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  //     if( orderOfGalerkinProjection==4 )
  // 	{
  // 	  RealArray uxe(I1,I2,I3,1), uttxe(I1,I2,I3,1), uxxxe(I1,I2,I3,1), uxxxxxe(I1,I2,I3,1);
  // 	  exact.gd( uxe     ,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
  // 	  exact.gd( uttxe   ,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );
  // 	  exact.gd( uxxxe   ,x,domainDimension,isRectangular,0,3,0,0,I1,I2,I3,wc,t );
  // 	  exact.gd( uxxxxxe ,x,domainDimension,isRectangular,0,5,0,0,I1,I2,I3,wc,t );

  // 	  ftzx = (density*thickness)*uttxe + K0*uxe - (T)*uxxxe + (EI)*uxxxxxe;  // x-derivative of the TZ force
  // 	}
    
  //     if( Kt!=0. )
  // 	{
  // 	  RealArray & ute = uxxe;  // re-use space
  // 	  exact.gd( ute, x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
  // 	  ftz += Kt*ute;
  // 	  if( orderOfGalerkinProjection==4 )
  // 	    {
  // 	      RealArray & utxe = uxxe;  // re-use space
  // 	      exact.gd( utxe, x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
  // 	      ftzx += Kt*utxe;
  // 	    }
  // 	}
  //     if( Kxxt!=0. )
  // 	{
  // 	  RealArray & utxxe = uxxe;  // re-use space
  // 	  exact.gd( utxxe, x,domainDimension,isRectangular,1,2,0,0,I1,I2,I3,wc,t );
  // 	  ftz += (-Kxxt)*utxxe;
  // 	  if( orderOfGalerkinProjection==4 )
  // 	    {
  // 	      RealArray & utxxxe = uxxe;  // re-use space
  // 	      exact.gd( utxxxe, x,domainDimension,isRectangular,1,3,0,0,I1,I2,I3,wc,t );
  // 	      ftzx += (-Kxxt)*utxxxe;
  // 	    }
      
  // 	}
    
  //     // ::display(utte,"utte","%8.2e ");
  //     // ::display(uxxe,"uxxe","%8.2e ");
  //     // ::display(ftz,"ftz","%8.2e ");
    
  //     RealArray lt(4); // local traction
  //     for ( int i = 0; i<numElem; i++ )
  // 	{
  // 	  // computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);

  // 	  if( orderOfGalerkinProjection==2 )
  // 	    computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);
  // 	  else
  // 	    computeGalerkinProjection( ftz(i),ftzx(i), ftz(i+1),ftzx(i+1),   -1.0,1.0, lt);

  // 	  Index idx(i*2,4);
  // 	  f(idx) += lt;
  // 	}
   
  //   }

  // if( projectedBodyForce*buoyantMassPerUnitLength!=0. )
  //   {
  //     // --- add buyouncy force
  //     RealArray lt(4);
  //     for (int i = 0; i < numElem; ++i) 
  // 	{
  // 	  // -- compute (N_i, . )
  // 	  computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
  // 				-1.0,1.0, lt);
  // 	  Index idx(i*2,4);
  // 	  f(idx) += lt;
  // 	}
  //   }
  
  // const bool isPeriodic = bcLeft==periodic;
  // if( isPeriodic )
  //   {
  //     Index Is(0,2), Ie(2*numElem,2);
  //     f(Is) += f(Ie);
  //     f(Ie) = f(Is);
  //   }
  

}







// ====================================================================================
/// \brief Smooth the Hermite solution with a fourth-order filter.
///   *** NOTE: THIS IS NOT REALLY WORKING YET****
/// \param t (input) : current time
/// \param w (input/output) : Hermite solution to smooth (u or v)
/// \param label (input) : label for debug output.
// ====================================================================================
void FEMBeamModel::
smooth( const real t, RealArray & w, const aString & label )
{
  const int & numElem = dbase.get<int>("numElem");
  const bool & smoothSolution = dbase.get<bool>("smoothSolution");
  const BoundaryCondition * bc = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft =  bc[0];
  const BoundaryCondition & bcRight =  bc[1];
  
  if( !smoothSolution )
    return;
  
  const int & numberOfSmooths = dbase.get<int>("numberOfSmooths");
  const int & smoothOrder     = dbase.get<int>("smoothOrder");

  // const int & current = dbase.get<int>("current"); 
  // std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  // std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF


  // add ghost points so we add apply filter up to boundary if needed
  int numberOfGhost = smoothOrder/2;    // 2 for 4th-order, 3 for 6th order filter
  int base =0, bound = numElem;
  RealArray w1(Range(base-numberOfGhost,bound+numberOfGhost),2);  // compute filtered solution here 

  // -- copy input into w1 
  for( int i=base; i<=bound; i++ )
  {
    w1(i,0)= w(2*i  );    //  u (or v)
    w1(i,1)= w(2*i+1);    //  u_x (or v_x)
  }
  
  const bool isPeriodic = bcLeft==periodic;
  
  const int orderOfExtrapolation=smoothOrder+1; 

  // we need at least this many elements to apply the smoother:
  assert( numElem >= orderOfExtrapolation );

  const real & dt = dbase.get<real>("dt"); 
  const real omega= dbase.get<real>("smoothOmega");  // parameter in smoother (=1 : kill plus minus mode)
  if( t < 3.*dt )
  {
    printF("--BM-- smooth %s, numberOfSmooths=%i (%ith order filter), omega=%9.3e isPeriodic=%i t=%8.2e.\n",
	   (const char*)label,numberOfSmooths,smoothOrder,omega,(int)isPeriodic,t );
	  
  }


//  omega *=dt ;  /// **TRY THIS **

  //const int bc[2]={bcLeft,bcRight};  // 

  // I : smooth these points for u or v . Keep the boundary points fixed, except for
  //  periodic 
  //  slide 
  // int freeEnd = freeBC;
  int freeEnd = -10; // turn off smooth on the boundary pts for a free end

  const int i1a= (isPeriodic || bc[0]==freeEnd || bc[0]==slideBC ) ? base  : base+1;
  const int i1b= (isPeriodic || bc[1]==freeEnd || bc[1]==slideBC ) ? bound : bound-1;

  // J : smooth these points of u_x or v_x . Keep the boundary points fixed, except for 
  //  periodic
  //  freeBC
  //  pinned: u_xx=0 : smooth u_x on the boundary
  //  slide
  const int j1a= (isPeriodic || bc[0]==pinned || bc[0]==freeEnd || bc[0]==slideBC ) ? base  : base+1;
  const int j1b= (isPeriodic || bc[1]==pinned || bc[1]==freeEnd || bc[1]==slideBC  ) ? bound : bound-1;

  Range I(i1a,i1b), J(j1a,j1b);

  smoothBoundaryConditions( w1, base, bound, numberOfGhost,orderOfExtrapolation );

  for( int smooth=0; smooth<numberOfSmooths; smooth++ )
  {
    // smooth interior pts (and boundary pts sometimes): 

    if( smoothOrder==4 )
    {
      // 4th order filter: 
      w1(I,0)= w1(I,0) + (omega/16.)*(-w1(I-2,0) + 4.*w1(I-1,0) -6.*w1(I,0) + 4.*w1(I+1,0) -w1(I+2,0) );
      w1(J,1)= w1(J,1) + (omega/16.)*(-w1(J-2,1) + 4.*w1(J-1,1) -6.*w1(J,1) + 4.*w1(J+1,1) -w1(J+2,1) );
    }
    else if( smoothOrder==6 )
    {
      // 6th order filter: 
      // 1 4 6 4 1 
      // 1 5 10 10 5 1
      // 1 6 15 20 15 6 1 
      w1(I,0)= w1(I,0) + (omega/64.)*(w1(I-3,0) - 6.*w1(I-2,0) +15.*w1(I-1,0) -20.*w1(I,0)
				      + 15.*w1(I+1,0) -6.*w1(I+2,0) + w1(I+3,0) );
      w1(J,1)= w1(J,1) + (omega/64.)*(w1(J-3,1) - 6.*w1(J-2,1) +15.*w1(J-1,1) -20.*w1(J,1)
				      + 15.*w1(J+1,1) -6.*w1(J+2,1) + w1(J+3,1) );
    }
    else
    {
      printF("BeamModel::smooth:ERROR: not implemented for smoothOrder=%i.\n",smoothOrder);
      OV_ABORT("error");
    }
    
    smoothBoundaryConditions( w1, base, bound, numberOfGhost,orderOfExtrapolation );

  } // end smooths


  // copy smoothed solution back to w
  for( int i=0; i<=numElem; i++ )
  {
    w(2*i  ) = w1(i,0);
    w(2*i+1) = w1(i,1);
  }


}



// ====================================================================================
/// \brief Apply boundary conditions for the smooth function.
// ====================================================================================
int FEMBeamModel::
smoothBoundaryConditions( RealArray & w1, int base, int bound,
                          int numberOfGhost, int orderOfExtrapolation )
{
    
  // -- boundary conditions --
  const BoundaryCondition * bc = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft =  bc[0];
  const BoundaryCondition & bcRight =  bc[1];
  const bool isPeriodic = bcLeft==periodic;
  Range R2(0,1);

  if( isPeriodic )
  {
    w1(bound,R2)=w1(base,R2);
    for( int g=1; g<=numberOfGhost; g++ )
    {
      w1(base -g,R2)=w1(bound-g,R2);
      w1(bound+g,R2)=w1(base +g,R2);
    }
  }
  else
  {
    // --- Assign Ghost ---
    for( int side=0; side<=1; side++ )
    {
      int ib  = side==0 ? base : bound; // boundary point
      int is = side==0 ? 1 : -1;
      if( bc[side]==freeBC )
      {
        // results from cgDoc/moving/codes/beam/beambc.maple

	// expansion for u when uxx=uxxx=0  u^(6)=0 u^(7)=0 
        //   u := x -> u0 + x*ux + x^4/(4!)*ux4 + x^5/(5!)*ux5;

	assert( numberOfGhost==2 || numberOfGhost==3 );

	real u0  = w1(ib     ,0);
	real up1 = w1(ib+  is,0);
	real up2 = w1(ib+2*is,0);
	real up3 = w1(ib+3*is,0);
	real up4 = w1(ib+4*is,0);
	  
        // N.B. : set boundary value too:
	w1(ib     ,0)=368./145*up1-318./145*up2+112./145*up3-17./145*up4;
	w1(ib-  is,0)=122./29*up1-136./29*up2+51./29*up3-8./29*up4;
	w1(ib-2*is,0)=208./29*up1-297./29*up2+144./29*up3-26./29*up4;
        if( numberOfGhost>=3 )
	  w1(ib-3*is,0)=455./29*up1-840./29*up2+518./29*up3-104./29*up4;
	
	// w1(ib-  is,0)=40./17.*u0-30./17.*up1+8./17.*up2-1./17.*up3;
	// w1(ib-2*is,0)=130./17.*u0-208./17.*up1+111./17.*up2-16./17.*up3;
	// if( numberOfGhost>=3 )
	//   w1(ib-3*is,0)=520./17.*u0-1053./17.*up1+648./17.*up2-98./17.*up3;
	
        // // w1(ib-  is,0)=2.*u0-up1;
        // w1(ib-  is,0)=3.*u0-3.*up1+up2;
        // w1(ib-2*is,0)=2.*u0-up2;
	
	// w=ux: 
        // expansion for w=ux when wx=0 wxx=0 w^(5)=0 w^(6)=0 
        //  w := x -> w0 + x^3/(3!)*wx3 + x^4/(4!)*wx4 + x^7/(7!)*wx7;
	real w0  = w1(ib     ,1);
	real wp1 = w1(ib+  is,1);
	real wp2 = w1(ib+2*is,1);
	real wp3 = w1(ib+3*is,1);
	real wp4 = w1(ib+4*is,1);

        // set boundary value too:
	w1(ib     ,1)=6336./4795*wp1-1944./4795*wp2+64./685*wp3-9./959*wp4;
	w1(ib-  is,1)=1898./959*wp1-1258./959*wp2+51./137*wp3-38./959*wp4;
	w1(ib-2*is,1)=7696./959*wp1-9423./959*wp2+432./137*wp3-338./959*wp4;
        if( numberOfGhost>=3 )
  	  w1(ib-3*is,1)=4095./137*wp1-5670./137*wp2+1946./137*wp3-234./137*wp4;

	// w1(ib-  is,1)=38./9.*w0-18./5.*wp1+2./5.*wp2-1./45.*wp3;
	// w1(ib-2*is,1)=338./9.*w0-208./5.*wp1+27./5.*wp2-16./45.*wp3;
	// if( numberOfGhost>=3 )
	//   w1(ib-3*is,1)=182.*w0-1053./5.*wp1+162./5.*wp2-14./5.*wp3;
	
	// // TEST: 
        // // w1(ib-  is,1)=wp1;
        // // w1(ib-2*is,1)=2.*w0-wp2;
        // w1(ib-  is,1)=2.*w0-wp1;
        // w1(ib-2*is,1)=2.*w0-wp2;

	// // Free BC: w_xx = 0    -> D_+^2 w_{-1} =0  IS THIS ACCURATE ENOUGH ??
	// //          w_xxx = 0   -> D_+^3 w_{-2} =0 
	// assert( numberOfGhost==2 );
	
	// int ig = ib - is; // 1st ghost point 
	// w1(ig,0) = 2.*w1(ig+is,0) -w1(ig+2*is,0);
	// w1(ig,1) = 3.*w1(ig+is,1) - 3.*w1(ig+2*is,1) + w1(ig+3*is,1);
	// ig = ib - 2*is;  // 2nd ghost point 
	// w1(ig,0) = 3.*w1(ig+is,0) - 3.*w1(ig+2*is,0) + w1(ig+3*is,0);
	// w1(ig,1) = 4.*w1(ig+is,1) -6.*w1(ig+2*is,1) + 4.*w1(ig+3*is,1) - w1(ig+4*is,1);  
      }
      else if( bc[side]==pinned )
      {
        //  Pinned: u=u_xx=0  -> u_xxxx=u_xxxxxx = 0  etc.
        //  u is an odd function 
        //  u_x is an even function 
	for( int g=1; g<=numberOfGhost; g++ )
	{
	  int ig = ib - g*is; // ghost point 
   	  w1(ig,0)=2.*w1(ib,0) - w1(ib+g*is,0);  // u (or v) is odd
   	  w1(ig,1)=   w1(ib+g*is,1);             // u_x or v_x is even
	}

      }
      else if( bc[side]==slideBC )
      {
        //  Slide: u_x=u_xxx=0  -> all odd derivatives are zero
        //  u is an even function 
        //  u_x is an odd function 
	for( int g=1; g<=numberOfGhost; g++ )
	{
	  int ig = ib - g*is; // ghost point 
   	  w1(ig,0)= w1(ib+g*is,0) ;               // u (or v) is even
   	  w1(ig,1)=2.*w1(ib,1) - w1(ib+g*is,1);   // u_x or v_x is odd
	}

      }
      else if( bc[side]==clamped )
      {
        //  Clamped: u=u_x=0   --> u_xxxx=0, u_xxxxx=0, ...

        // results from cgDoc/moving/codes/beam/beambc.maple

	// expansion for u when u=0, ux=0, uxxxx=0, uxxxxx=0, ...
        // u := x -> u0 + x^2/2*uxx + x^3/6*uxxx + x^6/(6!)*ux6 + x^7/(7!)*ux7 
        
	assert( numberOfGhost==2 || numberOfGhost==3 );

	real u0  = w1(ib     ,0);
	real up1 = w1(ib+  is,0);
	real up2 = w1(ib+2*is,0);
	real up3 = w1(ib+3*is,0);
	real up4 = w1(ib+4*is,0);
	  
        w1(ib     ,0)=0.;
	w1(ib-  is,0)=-385./174.*u0+122./29.*up1-34./29.*up2+17./87.*up3-1./58.*up4;
	w1(ib-2*is,0)=-1127./58.*u0+832./29.*up1-297./29.*up2+64./29.*up3-13./58.*up4;
	if( numberOfGhost>=3 )
    	  w1(ib-3*is,0)=-5271./58.*u0+4095./29.*up1-1890./29.*up2+518./29.*up3-117./58.*up4;
	  
	// w=ux: 
        //  expansion for w=ux when wxxx=0 wxxxx=0 w^(7)=0 w^(8)=0 
        // w := x -> w0 + x*wx + x^2/2*wxx + x^5/(5!)*wx5 + x^6/(6!)*wx6;

	real w0  = w1(ib     ,1);
	real wp1 = w1(ib+  is,1);
	real wp2 = w1(ib+2*is,1);
	real wp3 = w1(ib+3*is,1);
	real wp4 = w1(ib+4*is,1);

        w1(ib     ,1)=0.;
	w1(ib-  is,1)=98./29.*w0-122./29.*wp1+68./29.*wp2-17./29.*wp3+2./29.*wp4;
	w1(ib-2*is,1)=231./29.*w0-416./29.*wp1+297./29.*wp2-96./29.*wp3+13./29.*wp4;
	if( numberOfGhost>=3 )
  	  w1(ib-3*is,1)=574./29.*w0-1365./29.*wp1+1260./29.*wp2-518./29.*wp3+78./29.*wp4;


        // 4th-order filter: Obtain 2 ghost from
        //   u: 
        //        u_x = 0  
        //        u_xxxx = 0 
        //   w=u_x:
        //        w_xxx = 0 
        //        w_xxxx = 0 
        //
        // 6th-order filter: obtain 3 ghost from
        //   u:  (do not smooth boundary) 
        //       u_x = 0
        //       u_xxxx = 0
        //       u_xxxxx = 0 
        //   w=u_x: (do not smooth boundary) 
        //        w_xxx = 0 
        //        w_xxxx = 0 
        //        D_x^7 u = 0    Dz (D+D-)^3 

        // results from cgDoc/moving/codes/beam/beambc.maple
        // if( numberOfGhost==2 )
	// {
        //   // // Clamped: u:
        //   // // u(i-2) = 16*u(i+1)-3*u(i+2)-12*u(i)
        //   // // u(i-1) = 3*u(i+1)-1/2*u(i+2)-3/2*u(i)
	//   // w1(ib-2*is,0) = 16.*w1(ib+is,0)-3.*w1(ib+2*is,0)-12.*w1(ib,0);
	//   // w1(ib-  is,0) =  3.*w1(ib+is,0)-.5*w1(ib+2*is,0)-1.5*w1(ib,0);

        //   // // Clamped: u_x:
        //   // // u(i-2) = -8*u(i+1)+3*u(i+2)+6*u(i)
        //   // // u(i-1) = -3*u(i+1)+u(i+2)+3*u(i)
	//   // w1(ib-2*is,1) = -8.*w1(ib+is,1)+3.*w1(ib+2*is,1)+6.*w1(ib,1);
        //   // w1(ib-1*is,1) = -3.*w1(ib+is,1)+   w1(ib+2*is,1)+3.*w1(ib,1);

	//   OV_ABORT("finish me");
	// }
	// // for( int g=1; g<=numberOfGhost; g++ )
	// {
	//   int ig = ib - g*is; // ghost point 
   	//   w1(ig,0)=2.*w1(ib,0) - w1(ib+g*is,0);  // u (or v) is odd
   	//   w1(ig,1)=   w1(ib+g*is,1);             // u_x or v_x is even
	// }

      }
      else
      {
	// -- just extrapolate for now *FIX ME*
	for( int g=1; g<=numberOfGhost; g++ )
	{
	  int ig = ib - g*is; // ghost point 
	  if( orderOfExtrapolation==5 )
	    w1(ig,R2) = 5.*w1(ig+is,R2) -10.*w1(ig+2*is,R2) + 10.*w1(ig+3*is,R2) - 5.*w1(ig+4*is,R2) + w1(ig+5*is,R2);  
	  else if( orderOfExtrapolation==4 )
	    w1(ig,R2) = 4.*w1(ig+is,R2) -6.*w1(ig+2*is,R2) + 4.*w1(ig+3*is,R2) - w1(ig+4*is,R2);  
	  else if( orderOfExtrapolation==3 )
	    w1(ig,R2) = 3.*w1(ig+is,R2) - 3.*w1(ig+2*is,R2) + w1(ig+3*is,R2);
	  else if( orderOfExtrapolation==2 )
	    w1(ig,R2) = 2.*w1(ig+is,R2) -w1(ig+2*is,R2);
	  else
	  {
	    OV_ABORT("error: finish me");
	  }
	    
	}
      }
	
    }
  }
}






