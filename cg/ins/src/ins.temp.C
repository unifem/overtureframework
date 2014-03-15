#include "OB_CompositeGridSolver.h"
#include "OB_MappedGridSolver.h"
#include "Parameters.h"
#include "MappedGridOperators.h"
#include "SparseRep.h"
#include "Ogmg.h"
#include "display.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"

#include "turbulenceModels.h"
#include "turbulenceParameters.h"

#include "viscoPlasticMacrosCpp.h"

#define insdt insdt_
#define insdts insdts_
extern "C"
{
 void insdt(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b,
      const int&mask, const real& xy, const real& rx, 
      const real& u, const real& uu, real&ut, real&uti, const real&gv,  
      const real & dw, const int&bc, const int&ipar, const real&rpar, const int&ierr );

 void insdts(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b,
      const int&mask, const real& xy, const real& rx, const real& u, const real& uu, const real&gv,  
      const real & dw, const real & divDamp, const real & dtVar,
      const int&bc, const int&ipar, const real&rpar, const int&ierr );
}


#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

int 
outputCompositeGrid( CompositeGrid & cg, 
		     const aString & gridFileName )
// =======================================================================================
// /Description:
//   This function will save a CompositeGrid to a file for debugging 
// /cg(input) : the grid.
// /gridFileName (input) : grid file name, such as "cic.hdf".
// ========================================================================================
{

  if( Communication_Manager::My_Process_Number<=0 )
    printf("Saving the CompositeGrid in %s\n",(const char*)gridFileName);

  HDF_DataBase dataFile;
  dataFile.mount(gridFileName,"I");

  int streamMode=1; // save in compressed form.
  dataFile.put(streamMode,"streamMode");
  if( !streamMode )
    dataFile.setMode(GenericDataBase::noStreamMode); // this is now the default
  else
  {
    dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
  }
           
  if( cg.numberOfGrids() > 1 || cg.numberOfInterpolationPoints(0)>0 )
    cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
  else
    cg.destroy(CompositeGrid::EVERYTHING);
      

  const aString gridName="bugGrid";
  cg.put(dataFile,gridName);
  dataFile.unmount();



  return 0;
}






// incompressible Navier Stokes
// 
//   OB_MappedGridSolver::updateToMatchGrid: defines  derivatives to evaluate
//   saveShow.C : show file stuff
//   twilightZone: Parameters.C

#define U(c)     u(I1,I2,I3,c)   
#define UU(c)   uu(I1,I2,I3,c)
#define UX(c)   ux(I1,I2,I3,c)
#define UY(c)   uy(I1,I2,I3,c)
#define UZ(c)   uz(I1,I2,I3,c)
#define UXX(c) uxx(I1,I2,I3,c)
#define UXY(c) uxy(I1,I2,I3,c)
#define UXZ(c) uxz(I1,I2,I3,c)
#define UYY(c) uyy(I1,I2,I3,c)
#define UYZ(c) uyz(I1,I2,I3,c)
#define UZZ(c) uzz(I1,I2,I3,c)

// --------- define the artificial diffusions ------------
//
//       [ad21+ad22* |grad\uv|] ( D+rD-r(u) dr**2 + D+sD-s(u) ds**2 )
//           cd22=ad22/nd**2
//    ---2D:

#define AD2(kd) (  \
        (ad21 + cd22*    \
         ( fabs(UX(uc))+fabs(UY(uc))    \
          +fabs(UX(vc))+fabs(UY(vc)) ) )    \
         *(u(I1+1,I2,I3,kd)-4.*u(I1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
          +u(I1,I2+1,I3,kd)                  +u(I1,I2-1,I3,kd))    \
                         )    \
// This is for the eddy viscosity:
#define AD2N(kd) (  \
        (ad21 + cd22*( fabs(UX(nc))+fabs(UY(nc)) ) )\
         *(u(I1+1,I2,I3,kd)-4.*u(I1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
          +u(I1,I2+1,I3,kd)                  +u(I1,I2-1,I3,kd))    \
                         )    \
                 
//  ---3D:
#define  AD23(kd)  (    \
        (ad21 + cd22*    \
         ( fabs(UX(uc))+fabs(UY(uc))+fabs(UZ(uc))    \
          +fabs(UX(vc))+fabs(UY(vc))+fabs(UZ(vc))    \
          +fabs(UX(wc))+fabs(UY(wc))+fabs(UZ(wc)) ) )    \
         *(u(I1+1,I2,I3,kd)-6.*u(I1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
          +u(I1,I2+1,I3,kd)                   +u(I1,I2-1,I3,kd)    \
          +u(I1,I2,I3+1,kd)                   +u(I1,I2,I3-1,kd))    \
                            )
                       
// This is for the eddy viscosity:
#define  AD23N(kd)  (    \
        (ad21 + cd22*( fabs(UX(nc))+fabs(UY(nc))+fabs(UZ(nc)) ) )\
         *(u(I1+1,I2,I3,kd)-6.*u(I1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
          +u(I1,I2+1,I3,kd)                   +u(I1,I2-1,I3,kd)    \
          +u(I1,I2,I3+1,kd)                   +u(I1,I2,I3-1,kd))    \
                            )
                       
//  ---fourth-order artficial diffusion in 2D
#define AD4(kd) (    \
        (ad41 + cd42*    \
         ( fabs(UX(uc))+fabs(UY(uc))    \
          +fabs(UX(vc))+fabs(UY(vc)) ) )    \
         *(   -u(I1+2,I2,I3,kd)-u(I1-2,I2,I3,kd)    \
              -u(I1,I2+2,I3,kd)-u(I1,I2-2,I3,kd)    \
          +4.*(u(I1+1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
              +u(I1,I2+1,I3,kd)+u(I1,I2-1,I3,kd))    \
           -12.*u(I1,I2,I3,kd) )    \
                         )
//   ---fourth-order artficial diffusion in 3D
#define AD43(kd) (    \
        (ad41 + cd42*    \
         ( fabs(UX(uc))+fabs(UY(uc))+fabs(UZ(uc))    \
          +fabs(UX(vc))+fabs(UY(vc))+fabs(UZ(vc))    \
          +fabs(UX(wc))+fabs(UY(wc))+fabs(UZ(wc)) ) )    \
         *(   -u(I1+2,I2,I3,kd)-u(I1-2,I2,I3,kd)    \
              -u(I1,I2+2,I3,kd)-u(I1,I2-2,I3,kd)    \
              -u(I1,I2,I3+2,kd)-u(I1,I2,I3-2,kd)    \
          +4.*(u(I1+1,I2,I3,kd)+u(I1-1,I2,I3,kd)    \
              +u(I1,I2+1,I3,kd)+u(I1,I2-1,I3,kd)    \
              +u(I1,I2,I3+1,kd)+u(I1,I2,I3-1,kd))    \
           -18.*u(I1,I2,I3,kd) )    \
                          )
                       
#define RX(i1,i2,i3,m,n) inverseVertexDerivative(i1,i2,i3,m+numberOfDimensions*(n))

// ***** artificial diffusion in self-adjoint form **********
#define uxmzzR(i1,i2,i3,c) (u(i1,i2,i3,c)-u(i1-1,i2,i3,c))*dxi
#define uymzzR(i1,i2,i3,c) (u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,i3,c))*dyi*.25
#define uzmzzR(i1,i2,i3,c) (u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+u(i1-1,i2,i3+1,c)-u(i1-1,i2,i3-1,c))*dzi*.25

#define uxzmzR(i1,i2,i3,c) (u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,i3,c))*dxi*.25
#define uyzmzR(i1,i2,i3,c) (u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*dyi
#define uzzmzR(i1,i2,i3,c) (u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+u(i1,i2-1,i3+1,c)-u(i1,i2-1,i3-1,c))*dzi*.25

#define uxzzmR(i1,i2,i3,c) (u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+u(i1+1,i2,i3-1,c)-u(i1-1,i2,i3-1,c))*dxi*.25
#define uyzzmR(i1,i2,i3,c) (u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+u(i1,i2+1,i3-1,c)-u(i1,i2-1,i3-1,c))*dyi*.25
#define uzzzmR(i1,i2,i3,c) (u(i1,i2,i3,c)-u(i1,i2,i3-1,c))*dzi

// curvilinear grid
#define udmzC(i1,i2,i3,m,c) (RX(i1,i2,i3,0,m)+RX(i1-1,i2,i3,0,m))*(u(i1,i2,i3,c)-u(i1-1,i2  ,i3,c))*dr2i +\
                            (RX(i1,i2,i3,1,m)+RX(i1-1,i2,i3,1,m))*(\
                                u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,i3,c))*dsi*.125
#define udzmC(i1,i2,i3,m,c) (RX(i1,i2,i3,1,m)+RX(i1,i2-1,i3,1,m))*(u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*ds2i +\
                            (RX(i1,i2,i3,0,m)+RX(i1,i2-1,i3,0,m))*(\
                                u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,i3,c))*dri*.125

#define udmzzC(i1,i2,i3,m,c) (RX(i1,i2,i3,0,m)+RX(i1-1,i2,i3,0,m))*(u(i1,i2,i3,c)-u(i1-1,i2  ,i3,c))*dr2i +\
                             (RX(i1,i2,i3,1,m)+RX(i1-1,i2,i3,1,m))*(\
                                u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1-1,i2+1,i3,c)-u(i1-1,i2-1,i3,c))*dsi*.125+\
                             (RX(i1,i2,i3,2,m)+RX(i1-1,i2,i3,2,m))*(\
                                u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+ u(i1-1,i2,i3+1,c)-u(i1-1,i2,i3-1,c))*dti*.125
#define udzmzC(i1,i2,i3,m,c) (RX(i1,i2,i3,1,m)+RX(i1,i2-1,i3,1,m))*(u(i1,i2,i3,c)-u(i1,i2-1,i3,c))*ds2i +\
                             (RX(i1,i2,i3,0,m)+RX(i1,i2-1,i3,0,m))*(\
                                u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,i3,c))*dri*.125+\
                             (RX(i1,i2,i3,2,m)+RX(i1,i2-1,i3,2,m))*(\
                                u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)+ u(i1,i2-1,i3+1,c)-u(i1,i2-1,i3-1,c))*dti*.125

#define udzzmC(i1,i2,i3,m,c) (RX(i1,i2,i3,2,m)+RX(i1,i2,i3-1,2,m))*(u(i1,i2,i3,c)-u(i1,i2,i3-1,c))*dt2i +\
                             (RX(i1,i2,i3,0,m)+RX(i1,i2,i3-1,0,m))*(\
                                u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)+ u(i1+1,i2,i3-1,c)-u(i1-1,i2,i3-1,c))*dri*.125+\
                             (RX(i1,i2,i3,1,m)+RX(i1,i2,i3-1,1,m))*(\
                                u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)+ u(i1,i2+1,i3-1,c)-u(i1,i2-1,i3-1,c))*dsi*.125

//  Coefficients of the artificial diffusion for the momentum equations
//  2D - rectangular
#define admzR(i1,i2,i3) ad21+cd22*( fabs(uxmzzR(i1,i2,i3,uc))+fabs(uxmzzR(i1,i2,i3,vc))+\
                                    fabs(uymzzR(i1,i2,i3,uc))+fabs(uymzzR(i1,i2,i3,vc)) )

#define adzmR(i1,i2,i3) ad21+cd22*( fabs(uxzmzR(i1,i2,i3,uc))+fabs(uxzmzR(i1,i2,i3,vc))+\
                                    fabs(uyzmzR(i1,i2,i3,uc))+fabs(uyzmzR(i1,i2,i3,vc)) )

// 3D
#define admzzR(i1,i2,i3) \
          ad21+cd22*( fabs(uxmzzR(i1,i2,i3,uc))+fabs(uxmzzR(i1,i2,i3,vc))+fabs(uxmzzR(i1,i2,i3,wc))+\
                      fabs(uymzzR(i1,i2,i3,uc))+fabs(uymzzR(i1,i2,i3,vc))+fabs(uymzzR(i1,i2,i3,wc))+\
                      fabs(uzmzzR(i1,i2,i3,uc))+fabs(uzmzzR(i1,i2,i3,vc))+fabs(uzmzzR(i1,i2,i3,wc)) )

#define adzmzR(i1,i2,i3) \
          ad21+cd22*( fabs(uxzmzR(i1,i2,i3,uc))+fabs(uxzmzR(i1,i2,i3,vc))+fabs(uxzmzR(i1,i2,i3,wc))+\
                      fabs(uyzmzR(i1,i2,i3,uc))+fabs(uyzmzR(i1,i2,i3,vc))+fabs(uyzmzR(i1,i2,i3,wc))+\
                      fabs(uzzmzR(i1,i2,i3,uc))+fabs(uzzmzR(i1,i2,i3,vc))+fabs(uzzmzR(i1,i2,i3,wc)) )

#define adzzmR(i1,i2,i3) \
          ad21+cd22*( fabs(uxzzmR(i1,i2,i3,uc))+fabs(uxzzmR(i1,i2,i3,vc))+fabs(uxzzmR(i1,i2,i3,wc))+\
                      fabs(uyzzmR(i1,i2,i3,uc))+fabs(uyzzmR(i1,i2,i3,vc))+fabs(uyzzmR(i1,i2,i3,wc))+\
                      fabs(uzzzmR(i1,i2,i3,uc))+fabs(uzzzmR(i1,i2,i3,vc))+fabs(uzzzmR(i1,i2,i3,wc)) )
//  2D - curvilinear
#define admzC(i1,i2,i3)\
         ad21+cd22*( fabs(udmzC(i1,i2,i3,0,uc))+fabs(udmzC(i1,i2,i3,0,vc))+ \
                     fabs(udmzC(i1,i2,i3,1,uc))+fabs(udmzC(i1,i2,i3,1,vc)) )

#define adzmC(i1,i2,i3)\
        ad21+cd22*( fabs(udzmC(i1,i2,i3,0,uc))+fabs(udzmC(i1,i2,i3,0,vc))+ \
                    fabs(udzmC(i1,i2,i3,1,uc))+fabs(udzmC(i1,i2,i3,1,vc)) )

// 3D
#define admzzC(i1,i2,i3)\
        ad21+cd22*( fabs(udmzzC(i1,i2,i3,0,uc))+fabs(udmzzC(i1,i2,i3,0,vc))+fabs(udmzzC(i1,i2,i3,0,wc))+\
                    fabs(udmzzC(i1,i2,i3,1,uc))+fabs(udmzzC(i1,i2,i3,1,vc))+fabs(udmzzC(i1,i2,i3,1,wc))+ \
                    fabs(udmzzC(i1,i2,i3,2,uc))+fabs(udmzzC(i1,i2,i3,2,vc))+fabs(udmzzC(i1,i2,i3,2,wc)) )

#define adzmzC(i1,i2,i3)\
        ad21+cd22*( fabs(udzmzC(i1,i2,i3,0,uc))+fabs(udzmzC(i1,i2,i3,0,vc))+fabs(udzmzC(i1,i2,i3,0,wc))+\
                    fabs(udzmzC(i1,i2,i3,1,uc))+fabs(udzmzC(i1,i2,i3,1,vc))+fabs(udzmzC(i1,i2,i3,1,wc))+ \
                    fabs(udzmzC(i1,i2,i3,2,uc))+fabs(udzmzC(i1,i2,i3,2,vc))+fabs(udzmzC(i1,i2,i3,2,wc)) )

#define adzzmC(i1,i2,i3)\
        ad21+cd22*( fabs(udzzmC(i1,i2,i3,0,uc))+fabs(udzzmC(i1,i2,i3,0,vc))+fabs(udzzmC(i1,i2,i3,0,wc))+\
                    fabs(udzzmC(i1,i2,i3,1,uc))+fabs(udzzmC(i1,i2,i3,1,vc))+fabs(udzzmC(i1,i2,i3,1,wc))+ \
                    fabs(udzzmC(i1,i2,i3,2,uc))+fabs(udzzmC(i1,i2,i3,2,vc))+fabs(udzzmC(i1,i2,i3,2,wc)) )


//  **** self-adjoint artificial diffusion for the Spalart-Allmaras TM
// 2D - rectangular
#define admzRSA(i1,i2,i3) ad21+cd22*( fabs(uxmzzR(i1,i2,i3,nc))+fabs(uymzzR(i1,i2,i3,nc)) )
#define adzmRSA(i1,i2,i3) ad21+cd22*( fabs(uxzmzR(i1,i2,i3,nc))+fabs(uyzmzR(i1,i2,i3,nc)) )
// 3D
#define admzzRSA(i1,i2,i3) ad21+cd22*( fabs(uxmzzR(i1,i2,i3,nc))+fabs(uymzzR(i1,i2,i3,nc))+fabs(uzmzzR(i1,i2,i3,nc)) )
#define adzmzRSA(i1,i2,i3) ad21+cd22*( fabs(uxzmzR(i1,i2,i3,nc))+fabs(uyzmzR(i1,i2,i3,nc))+fabs(uzzmzR(i1,i2,i3,nc)) )
#define adzzmRSA(i1,i2,i3) ad21+cd22*( fabs(uxzzmR(i1,i2,i3,nc))+fabs(uyzzmR(i1,i2,i3,nc))+fabs(uzzzmR(i1,i2,i3,nc)) )

//  2D - curvilinear
#define admzCSA(i1,i2,i3) ad21+cd22*( fabs(udmzC(i1,i2,i3,0,nc))+fabs(udmzC(i1,i2,i3,1,nc)) )
#define adzmCSA(i1,i2,i3) ad21+cd22*( fabs(udzmC(i1,i2,i3,0,nc))+fabs(udzmC(i1,i2,i3,1,nc)) )
// 3D
#define admzzCSA(i1,i2,i3)\
         ad21+cd22*( fabs(udmzzC(i1,i2,i3,0,nc))+fabs(udmzzC(i1,i2,i3,1,nc))+fabs(udmzzC(i1,i2,i3,2,nc)) )
#define adzmzCSA(i1,i2,i3)\
         ad21+cd22*( fabs(udzmzC(i1,i2,i3,0,nc))+fabs(udzmzC(i1,i2,i3,1,nc))+fabs(udzmzC(i1,i2,i3,2,nc)) )
#define adzzmCSA(i1,i2,i3)\
         ad21+cd22*( fabs(udzzmC(i1,i2,i3,0,nc))+fabs(udzzmC(i1,i2,i3,1,nc))+fabs(udzzmC(i1,i2,i3,2,nc)) )


#define AD2F(m) adf0(I1+1,I2,I3)*(u(I1+1,I2,I3,m)-u(I1,I2,I3,m))-adf0(I1,I2,I3)*(u(I1,I2,I3,m)-u(I1-1,I2,I3,m))+\
   	        adf1(I1,I2+1,I3)*(u(I1,I2+1,I3,m)-u(I1,I2,I3,m))-adf1(I1,I2,I3)*(u(I1,I2,I3,m)-u(I1,I2-1,I3,m))

#define AD3F(m) adf0(I1+1,I2,I3)*(u(I1+1,I2,I3,m)-u(I1,I2,I3,m))-adf0(I1,I2,I3)*(u(I1,I2,I3,m)-u(I1-1,I2,I3,m))+\
   	        adf1(I1,I2+1,I3)*(u(I1,I2+1,I3,m)-u(I1,I2,I3,m))-adf1(I1,I2,I3)*(u(I1,I2,I3,m)-u(I1,I2-1,I3,m))+\
   	        adf2(I1,I2,I3+1)*(u(I1,I2,I3+1,m)-u(I1,I2,I3,m))-adf2(I1,I2,I3)*(u(I1,I2,I3,m)-u(I1,I2,I3-1,m))

//\begin{>>MappedGridSolverInclude.tex}{\subsection{getUtINS}}
int OB_MappedGridSolver::
getUtINS(const realMappedGridFunction & v, 
	 const realMappedGridFunction & gridVelocity_, 
	 realMappedGridFunction & dvdt, 
         int iparam[], real rparam[],
         realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
	 MappedGrid *pmg2 /* =NULL */,
	 const realMappedGridFunction *pGridVelocity2 /* = NULL */ )
//========================================================================================================
// /Description:
//   Compute $u_t$ on a component grid for the Incompressible NS: 2D, 2nd or 4th order
//
// /t (input) : current time.
// /v (input) : current solution.
// /gridVelocity\_ (input) : grid velocity, used for moving grid problems only.
// /dvdt (output) : return $u_t$ in this grid function.
// /grid (input) : the component grid number if this MappedGrid is part of a GridCollection or CompositeGrid.
// /tForce (input) : apply the forcing at this time (this could be $t+\Delta/2$ for example).
// /dvdtImplicit (input) : for implicit time stepping, the time derivative is split into two parts,
//     $u_t=u_t^E + u_t^I$. The explicit part, $u_t^E$, is returned in dvdt while the implicit part, $u_t^I$,
//   is returned in dvdtImplicit. 
// /tImplicit (input) : for implicit time stepping, apply forcing for the implicit part at his 
//     time.
//  /pmg2 (input) : pointer to the grid at the next time level (this is needed by some methods for moving grids)
//  /pGridVelocity2 (input) : pointer to the grid velocity at the next time level (this is needed by some
//                             methods for moving grids)
//\end{MappedGridSolverInclude.tex}  
//=======================================================================================================
{
  if( debug() & 8 && parameters.dbase.get<int >("myid")==0 )
    printf("OB_MappedGridSolver::getUtINS: pde = %i \n",parameters.dbase.get<Parameters::PDE >("pde"));

  const real & t=rparam[0];
  real tForce   =rparam[1];
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];
  const int level=iparam[1];
  const int numberOfStepsTaken = iparam[2];
  
  MappedGrid & mg = *(v.getMappedGrid());

  const int numberOfDimensions = mg.numberOfDimensions();
  
  Index I1,I2,I3;
//   getIndex(extendedGridIndexRange(mg),I1,I2,I3);  // ***** 020902 *** WHY???
  getIndex(mg.gridIndexRange(),I1,I2,I3);  // ***** 030305 : we don't want to evaluate du/dt on ghost points

  const realArray & u = v;                               // **** array *****
  const realArray & gridVelocity = gridVelocity_;
  realArray & ut = dvdt;
  realArray & uti = dvdtImplicit;

  MappedGridOperators & op = *v.getOperators();

  const int isRectangular=op.isRectangular(); // ********************
  // const int isRectangular=mg.isRectangular(); // ********************
  
  real nu = parameters.dbase.get<real >("nu");  // this is a locally copy
  if( parameters.getGridIsImplicit(grid) )
    nu*=(1.-parameters.dbase.get<real >("implicitFactor"));
  
  const real nuI = nu; // parameters.dbase.get<real >("nu");  // use this value for implicit terms

  const real adcPassiveScalar=1.; // coeff or linear artificial diffusion for the passive scalar ** add to params

  // only apply fourth-order AD here if it is explicit
  const bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
                                                !parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion");
  const bool & gridIsMoving = parameters.gridIsMoving(grid);

  bool useOpt=true;

  if( useOpt )
  {
    int useWhereMask=false; // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 

    int *pMask;
    if( pmg2!=NULL )
    {
      // only need to evaluate du/dt at mask>0 in this case -- we have the mask at the new time level
      useWhereMask=true;
      #ifdef USE_PPP
        pMask = pmg2->mask().getLocalArray().getDataPointer();
      #else
        pMask = pmg2->mask().getDataPointer();
      #endif

      // printf(" ***getUtINS: useWhereMask=%i \n",useWhereMask);
    }
    else
    {
      #ifdef USE_PPP
        pMask = mg.mask().getLocalArray().getDataPointer(); 
      #else
        pMask = mg.mask().getDataPointer();
      #endif
    }

    real dx[3]={1.,1.,1.};
    if( isRectangular )
      mg.getDeltaX(dx);
    else
     mg.update(MappedGrid::THEinverseVertexDerivative);
   
    const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
    const int gridType= isRectangular ? 0 : 1;

    #ifdef USE_PPP
      realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
    #else
      const realSerialArray & uLocal = u;
    #endif
    const real *pu = u.getDataPointer();

    // For now we need the center array for the axisymmetric case:
    const realArray & xy = parameters.isAxisymmetric() ? mg.center() : u;
    if( parameters.isAxisymmetric() ) 
    {
      assert( mg.center().getLength(0)>0 );
    }
    const realArray & rsxy = isRectangular ? u :  mg.inverseVertexDerivative();
    // For non-moving grids u==uu, otherwise uu is a temp space to hold (u-gv)
    const realArray & uu = parameters.gridIsMoving(grid) ? get(WorkSpace::uu) : u;
    const realArray & dw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? u : 
      (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];
    

    #ifdef USE_PPP
      const real *pxy = xy.getLocalArray().getDataPointer(); 
      const real *prsxy = rsxy.getLocalArray().getDataPointer(); 
      real *put = ut.getLocalArray().getDataPointer(); 
      real *puti = uti.getLocalArray().getDataPointer(); 
      const real *pgv = gridIsMoving ? gridVelocity.getLocalArray().getDataPointer() : pu;
      const real *puu = uu.getLocalArray().getDataPointer(); 
      const real *pdw = dw.getLocalArray().getDataPointer();
    #else
      const real *pxy = xy.getDataPointer(); 
      const real *prsxy = rsxy.getDataPointer(); 
      real *put = ut.getDataPointer(); 
      real *puti = uti.getDataPointer(); 
      const real *pgv = gridIsMoving ? gridVelocity.getDataPointer() : pu;
      const real *puu = uu.getDataPointer(); 
      const real *pdw = dw.getDataPointer();
    #endif 

    getIndex(mg.gridIndexRange(),I1,I2,I3);  // *wdh* 030220  - evaluate du/dt here

    #ifdef USE_PPP
     // loop bounds for this boundary:
     const int n1a = max(I1.getBase() , uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
     const int n1b = min(I1.getBound(),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));
     const int n2a = max(I2.getBase() , uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
     const int n2b = min(I2.getBound(),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));
     const int n3a = max(I3.getBase() , uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
     const int n3b = min(I3.getBound(),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));
    #else
     // loop bounds for this boundary:
     const int n1a=I1.getBase(); const int n1b=I1.getBound();
     const int n2a=I2.getBase(); const int n2b=I2.getBound();
     const int n3a=I3.getBase(); const int n3b=I3.getBound();
    #endif

    if( n1a>n1b || n2a>n2b || n3a>n3b ) return 0;

    I1=Range(n1a,n1b);
    I2=Range(n2a,n2b);
    I3=Range(n3a,n3b);

    const int gridIsImplicit=parameters.getGridIsImplicit(grid);
    real adcBoussinesq=0.; // coefficient of artificial diffusion for Boussinesq T equation 
    real thermalExpansivity=1.;

    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("adcBoussinesq",adcBoussinesq);

    // declare and lookup visco-plastic parameters (macro)
    declareViscoPlasticParameters;

    int ipar[] ={parameters.dbase.get<int >("pc"),
                 parameters.dbase.get<int >("uc"),
                 parameters.dbase.get<int >("vc"),
                 parameters.dbase.get<int >("wc"),
                 parameters.dbase.get<int >("kc"),
                 parameters.dbase.get<int >("sc"),
                 parameters.dbase.get<int >("tc"),
                 grid,
		 orderOfAccuracy,
                 (int)parameters.gridIsMoving(grid),
		 useWhereMask,
                 (int)gridIsImplicit,
                 (int)parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod"),
		 (int)parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),
                 (int)parameters.isAxisymmetric(),
                 (int)parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion"),
                 (int)useFourthOrderArtificialDiffusion,
                 (int)parameters.dbase.get<bool >("advectPassiveScalar"),
                 gridType,
                 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),
                 (int)parameters.dbase.get<Parameters::PDEModel >("pdeModel")
                 };

    const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
    real rpar[]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),
                 dx[0],dx[1],dx[2],
                 nu,
                 parameters.dbase.get<real >("ad21"),
                 parameters.dbase.get<real >("ad22"),
                 parameters.dbase.get<real >("ad41"),
                 parameters.dbase.get<real >("ad42"),
                 parameters.dbase.get<real >("nuPassiveScalar"),
                 adcPassiveScalar,
                 parameters.dbase.get<real >("ad21n"),
                 parameters.dbase.get<real >("ad22n"),
                 parameters.dbase.get<real >("ad41n"),
                 parameters.dbase.get<real >("ad42n"),
                 yEps,                    // 17
                 parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],
                 parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1],
                 parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2],
                 thermalExpansivity,
                 adcBoussinesq,
                 parameters.dbase.get<real >("kThermal"),
                 nuViscoPlastic,           // 24
                 etaViscoPlastic,
                 yieldStressViscoPlastic,
                 exponentViscoPlastic,
                 epsViscoPlastic
                 };

    int ierr=0;

    insdt(mg.numberOfDimensions(),
	  I1.getBase(),I1.getBound(),
	  I2.getBase(),I2.getBound(),
	  I3.getBase(),I3.getBound(),
	  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
	  uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
	  *pMask, *pxy, *prsxy,
          *pu, *puu, *put, 
          *puti,*pgv, *pdw,
          *mg.boundaryCondition().getDataPointer(), ipar[0], rpar[0], ierr );


    return 0;
  }



//    if( parameters.dbase.get<int >("orderOfAccuracy")==4 )
//      op.setOrderOfAccuracy(parameters.dbase.get<int >("orderOfAccuracy"));  // set here for now until BC are fixed for 4th order

  realArray & uu = get(WorkSpace::uu);
  realArray & ux = get(WorkSpace::ux);
  realArray & uy = get(WorkSpace::uy);
  realArray & uz = get(WorkSpace::uz);
  realArray & uxx= get(WorkSpace::uxx);
  realArray & uyy= get(WorkSpace::uyy);
  realArray & uzz= get(WorkSpace::uzz);
    

  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");
  const int pc = parameters.dbase.get<int >("pc");
  const int kc = parameters.dbase.get<int >("kc");
  const int epsc = parameters.dbase.get<int >("epsc");
  const int sc = parameters.dbase.get<int >("sc");
  const int nc=kc;

  // const int radialAxis = parameters.dbase.get<int >("radialAxis");


  const real ad21 = parameters.dbase.get<real >("ad21");
  const real ad22 = parameters.dbase.get<real >("ad22");
  const real ad41 = parameters.dbase.get<real >("ad41");
  const real ad42 = parameters.dbase.get<real >("ad42");
  const real advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");


//  Range N = mg.numberOfDimensions()==2 ? Range(pc,vc) : Range(pc,wc);   // *****
  Range N = Range(pc,parameters.dbase.get<Range >("Rt").getBound());
  MappedGridOperators & operators = *(v.getOperators());
  operators.getDerivatives(v,I1,I2,I3,N);

  MappedGridSolverWorkSpace::resize(uu,I1,I2,I3,parameters.dbase.get<Range >("Ru")); // *** added 040223 ***

  if( nu < 0. || ad21 < 0. || ad22 < 0. || ad41 < 0. || ad42 < 0.  )
  {
    if( parameters.dbase.get<int >("myid")==0 )
    {
      cout << " dudt: Invalid parameter..." << endl;
      printf(" numberOfComponents=%3i, nu=%e, ad21=%e, ad22=%e, ad41=%e, ad42=%e \n",
	     numberOfComponents,nu,ad21,ad22,ad41,ad42 );
    }
    throw "error";
  }

  // *******************************************
//    realArray uta;
//    if( parameters.isAxisymmetric() )
//    {
//      // save for testing
//      uta.redim(I1,I2,I3,N);
//      uta(I1,I2,I3,N)=ut(I1,I2,I3,N);
//    }
    // *******************************************
  


  const real dri=1./mg.gridSpacing(0);
  const real dsi=1./mg.gridSpacing(1);
  const real dti=1./mg.gridSpacing(2);
  const real dr2i=dri/2., ds2i=dsi/2., dt2i=dti/2.;

  real dx[3]={1.,1.,1.};
  if( isRectangular )
    mg.getDeltaX(dx);
  
  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel )
    mg.update(MappedGrid::THEvertex);

  const realArray & vertex = mg.vertex();
    

  const real dxi=1./dx[0];
  const real dyi=1./dx[1];
  const real dzi=1./dx[2];
  const real dx2i=dxi/2., dy2i=dyi/2., dz2i=dzi/2.;

  if( mg.numberOfDimensions()==1 )
  {
    //  ---------------------------------------     
    //  ---1D : Evaluate the equations --------
    //  ---------------------------------------     
    if( gridIsMoving )
      uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-advectionCoefficient*U(uc);
    else
      uu(I1,I2,I3,uc)=(-advectionCoefficient)*U(uc);

    // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 
    ut(I1,I2,I3,uc)= UU(uc)*UX(uc)-UX(pc)+nu*(UXX(uc));
    if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
    { // --- add 2nd order artificial diffusion
      real cd22=ad22/SQR(mg.numberOfDimensions());
      ut(I1,I2,I3,uc)+=AD2(uc);
    }
    if( useFourthOrderArtificialDiffusion && (ad41!=0. || ad42!=0.) )
    { // --- add 4th order artificial diffusion  ******* extra points needed ********
      real cd42=ad42/SQR(mg.numberOfDimensions());
      ut(I1,I2,I3,uc)+=AD4(uc);
    }
  }
  else if( mg.numberOfDimensions()==2 )
  {
    //  ---------------------------------------     
    //  ---2D : Evaluate the equations --------
    //  ---------------------------------------     


    if( gridIsMoving )
    {
      uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-advectionCoefficient*U(uc);
      uu(I1,I2,I3,vc)=gridVelocity(I1,I2,I3,1)-advectionCoefficient*U(vc);
    }
    else
    {
      uu(I1,I2,I3,uc)=(-advectionCoefficient)*U(uc);
      uu(I1,I2,I3,vc)=(-advectionCoefficient)*U(vc);
    }
    // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 

    ut(I1,I2,I3,uc)= UU(uc)*UX(uc)+UU(vc)*UY(uc)-UX(pc);
    ut(I1,I2,I3,vc)= UU(uc)*UX(vc)+UU(vc)*UY(vc)-UY(pc);


    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
    {
      if( nu!=0. )
      {
	realArray urOverR, vrOverR;
	if( parameters.isAxisymmetric() && ( !parameters.getGridIsImplicit(grid) ||
					     parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately ) )
	{
	  // y corresponds to the radial direction
	  // y=0 is the axis of symmetry
	  realArray radiusInverse;
// ****	  radiusInverse=1./max(REAL_MIN,vertex(I1,I2,I3,axis2));
	  const real eps = sqrt(REAL_EPSILON);
	  radiusInverse=1./max(eps,vertex(I1,I2,I3,axis2));

	  //   nu*(  u.xx + u.yy + (1/y) u.y )
	  //   nu*(  v.xx + v.yy + (1/y) v.y - v/y^2 ) 
	  urOverR=UY(uc)*radiusInverse;
	  vrOverR=(UY(vc)-U(vc)*radiusInverse)*radiusInverse;
	  // fix points on axis
	  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    for( int side=Start; side<=End; side++ )
	    {
	      if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
	      {
		Index Ib1,Ib2,Ib3;
		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		urOverR(Ib1,Ib2,Ib3)=uyy(Ib1,Ib2,Ib3,uc);      // u.y/y -> u.yy on y=0
		vrOverR(Ib1,Ib2,Ib3)=.5*uyy(Ib1,Ib2,Ib3,vc);  // (1/y) v.y - v/y^2 -> v.yy - .5*v.yy = .5*v.yy
	      }
	    }
	  }
	}
	if( !parameters.getGridIsImplicit(grid) )
	{ // explicit time stepping:
	  ut(I1,I2,I3,uc)+= nu*(UXX(uc)+UYY(uc));
	  ut(I1,I2,I3,vc)+= nu*(UXX(vc)+UYY(vc));
	  if( parameters.isAxisymmetric() )
	  {
	    ut(I1,I2,I3,uc)+= nu*urOverR;
	    ut(I1,I2,I3,vc)+= nu*vrOverR;

//              real diffut,diffvt;
//              where( mg.mask()(I1,I2,I3)>0 )
//  	    {
//  	      diffut=max(fabs(ut(I1,I2,I3,uc)-uta(I1,I2,I3,uc)));
//  	      diffvt=max(fabs(ut(I1,I2,I3,vc)-uta(I1,I2,I3,vc)));
//  	    }
//  	    printf(" grid=%i diffut=%8.2e diffvt=%8.2e\n",grid,diffut,diffvt);
//  	    if( diffut>1.e-7 )
//  	    {
//                realArray diffu(I1,I2,I3); diffu=0;
//  	      where( mg.mask()(I1,I2,I3)>0 )
//  	      {
//  		diffu(I1,I2,I3)=ut(I1,I2,I3,uc)-uta(I1,I2,I3,uc);
//                }
//  	      ::display(ut(I1,I2,I3,uc),"ut",parameters.dbase.get<FILE* >("debugFile"),"%5.1f ");
//  	      ::display(uta(I1,I2,I3,uc),"uta",parameters.dbase.get<FILE* >("debugFile"),"%5.1f ");
//  	      ::display(diffu(I1,I2,I3),"diffu",parameters.dbase.get<FILE* >("debugFile"),"%8.2e ");
//  	    }
	    

	  }
	}
	else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	{
	  uti(I1,I2,I3,uc)=nuI*(UXX(uc)+UYY(uc));
	  uti(I1,I2,I3,vc)=nuI*(UXX(vc)+UYY(vc));
	  if( parameters.isAxisymmetric() )
	  {
	    uti(I1,I2,I3,uc)+= nuI*urOverR;
	    uti(I1,I2,I3,vc)+= nuI*vrOverR;
	  }
	}
      }
      else if(parameters.getGridIsImplicit(grid) &&
	      parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately ) 
      {
	uti(I1,I2,I3,uc)=0.;
	uti(I1,I2,I3,vc)=0.;
      }


    }
    else 
    {

      // *** add in contributions for turbulence models
      bool useTurbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel;
    
      realArray nuT;
      turbulenceModelsINS(nuT,mg,u,uu,ut,ux,uy,uz,uxx,uyy,uzz,I1,I2,I3,parameters,nu,mg.numberOfDimensions(),grid,t );
      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
	const intArray & mask = mg.mask();
      
	real cv1=7.1, cv1e3=pow(cv1,3.);

        realArray chi3,nuTd,nuTx,nuTy;
	chi3=U(nc)/nu;

        // chi3=0.;  // *******************

	chi3=chi3*chi3*chi3;
	
	nuT = nu+U(nc)*chi3/(chi3+cv1e3);
	nuTd= chi3*(chi3+4.*cv1e3)/SQR(chi3+cv1e3);
	nuTx=UX(nc)*nuTd;
	nuTy=UY(nc)*nuTd;

	ut(I1,I2,I3,uc)+= nuT*(UXX(uc)+UYY(uc)) +nuTx*(2.*UX(uc)    ) +nuTy*(UY(uc)+UX(vc));
	ut(I1,I2,I3,vc)+= nuT*(UXX(vc)+UYY(vc)) +nuTx*(UY(uc)+UX(vc)) +nuTy*(2.*UY(vc));

        if( debug() & 2 && !parameters.dbase.get<bool >("twilightZoneFlow") )
	{
	  // compute the first grid spacing at walls in terms of units of y+
	  //    y+ = y * u_tau/nu
	  //    u_tau = sqrt{ tau_w/rho }
	  //    tau_w = wall shear stress 
	  //    y+ = y * sqrt{ |grad u|/nu }
	  //
	  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    for( int side=Start; side<=End; side++ )
	    {
	      if( mg.boundaryCondition(side,axis)==Parameters::noSlipWall )
	      {
		const realArray & normal = mg.vertexBoundaryNormal(side,axis);
	    
		Index Ib1,Ib2,Ib3;
		Index Ip1,Ip2,Ip3;
		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line inside
	    
		realArray dn;
		dn = (SQR(vertex(Ip1,Ip2,Ip3,0)-vertex(Ib1,Ib2,Ib3,0)) +
		      SQR(vertex(Ip1,Ip2,Ip3,1)-vertex(Ib1,Ib2,Ib3,1)));
	    
		realArray gradu;
		gradu = (fabs(ux(Ib1,Ib2,Ib3,uc))+fabs(ux(Ib1,Ib2,Ib3,vc))+
			 fabs(uy(Ib1,Ib2,Ib3,uc))+fabs(uy(Ib1,Ib2,Ib3,vc)));
	    
		dn = sqrt( dn* fabs(gradu)*(1./nu) );
	    
		real dnMin=0.,dnMax=0.,yPlus=0.,yPlusMax=0.,graduMax=0.,graduMin=0.;
		where( mask(Ib1,Ib2,Ib3)>0 )
		{
		  dnMin=min(dn);
		  dnMax=max(dn);
		  graduMin=min(fabs(gradu));
		  graduMax=max(fabs(gradu));
		  yPlus=1./max(REAL_MIN*100.,sqrt( graduMax/nu ));
		  yPlusMax=1./max(REAL_MIN*100.,sqrt( graduMin/nu ));
		}
                if( parameters.dbase.get<int >("myid")==0 )
		  printf(" grid=%i (%i,%i) first line is [%8.2e - %8.2e] y+ units. y+=[%8.2e - %8.2e] "
			 " |grad(u)_w|=[%8.2e - %8.2e]\n",
			 grid,side,axis,dnMin,dnMax,yPlus,yPlusMax,graduMin,graduMax);

	      }
	  
	    }
	  }
	}
	
      }
      else
      {
	Overture::abort("Unknown turbulence model");
      }
      
    }
    
    // printf(" *************** inside insut ******************** \n");
    
    // if( !parameters.getGridIsImplicit(grid) && (ad21!=0. || ad22!=0.) )
    if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
    { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
      
      const realArray & inverseVertexDerivative = mg.inverseVertexDerivative();

      real cd22=ad22/SQR(mg.numberOfDimensions());

      Range J1(I1.getBase(),I1.getBound()+1);// J1= I1 plus one extra point
      Range J2(I2.getBase(),I2.getBound()+1);
      realArray adf0(J1,I2,I3),adf1(I1,J2,I3);
      if( isRectangular )
      {
	adf0=admzR(J1,I2,I3);  // coefficient at i+1/2
	adf1=adzmR(I1,J2,I3);  // coefficient at j+1/2
      }
      else
      {
	adf0=admzC(J1,I2,I3);  // coefficient at i+1/2
	adf1=adzmC(I1,J2,I3);  // coefficient at j+1/2
      }

//        ::display(adf0,"adf0","%6.2f ");
//        ::display(adf1,"adf1","%6.2f ");
//        adf0=0.;
//        adf1=0.;

      ut(I1,I2,I3,uc)+=AD2F(uc);
      ut(I1,I2,I3,vc)+=AD2F(vc);
      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
        // const real cd22n=ad22/mg.numberOfDimensions();
        cd22=ad22/mg.numberOfDimensions();
	if( isRectangular )
	{
	  adf0=admzRSA(J1,I2,I3);  // coefficient at i+1/2
	  adf1=adzmRSA(I1,J2,I3);  // coefficient at j+1/2
	}
	else
	{
	  adf0=admzCSA(J1,I2,I3);  // coefficient at i+1/2
	  adf1=adzmCSA(I1,J2,I3);  // coefficient at j+1/2
	}
        ut(I1,I2,I3,nc)+=AD2F(nc);
      }
      
    }
    if( useFourthOrderArtificialDiffusion && (ad41!=0. || ad42!=0.) )
    { // --- add 4th order artificial diffusion  ******* extra points needed ********
      real cd42=ad42/SQR(mg.numberOfDimensions());
      ut(I1,I2,I3,uc)+=AD4(uc);
      ut(I1,I2,I3,vc)+=AD4(vc);
    }

    if( parameters.dbase.get<bool >("useSmagorinskyEddyViscosity") )
    {
      // add the Smagorinsky eddy viscosity
      

    }
    
  }
  else if( mg.numberOfDimensions()==3 )
  {
    //  ---------------------------------------     
    //  ---3D : Evaluate the equations --------
    //  ---------------------------------------     
    if( gridIsMoving )
    {
      uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-advectionCoefficient*U(uc);
      uu(I1,I2,I3,vc)=gridVelocity(I1,I2,I3,1)-advectionCoefficient*U(vc);
      uu(I1,I2,I3,wc)=gridVelocity(I1,I2,I3,2)-advectionCoefficient*U(wc);
    }
    else
    {
      uu(I1,I2,I3,uc)=-advectionCoefficient*U(uc);
      uu(I1,I2,I3,vc)=-advectionCoefficient*U(vc);
      uu(I1,I2,I3,wc)=-advectionCoefficient*U(wc);
    }
    ut(I1,I2,I3,uc)=UU(uc)*UX(uc)+UU(vc)*UY(uc)+UU(wc)*UZ(uc)-UX(pc);
    ut(I1,I2,I3,vc)=UU(uc)*UX(vc)+UU(vc)*UY(vc)+UU(wc)*UZ(vc)-UY(pc);
    ut(I1,I2,I3,wc)=UU(uc)*UX(wc)+UU(vc)*UY(wc)+UU(wc)*UZ(wc)-UZ(pc);


    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
    {
      if( nu!=0. )
      {
	if( !parameters.getGridIsImplicit(grid) )
	{ // explicit time stepping:
	  ut(I1,I2,I3,uc)+=nu*(UXX(uc)+UYY(uc)+UZZ(uc));
	  ut(I1,I2,I3,vc)+=nu*(UXX(vc)+UYY(vc)+UZZ(vc));
	  ut(I1,I2,I3,wc)+=nu*(UXX(wc)+UYY(wc)+UZZ(wc));
	}
	else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	{
	  uti(I1,I2,I3,uc)=nuI*(UXX(uc)+UYY(uc)+UZZ(uc));
	  uti(I1,I2,I3,vc)=nuI*(UXX(vc)+UYY(vc)+UZZ(vc));
	  uti(I1,I2,I3,wc)=nuI*(UXX(wc)+UYY(wc)+UZZ(wc));
	}
      }
      else if(parameters.getGridIsImplicit(grid) &&
	      parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately ) 
      {
	uti(I1,I2,I3,uc)=0.;
	uti(I1,I2,I3,vc)=0.;
	uti(I1,I2,I3,wc)=0.;
      }
    }
    else
    {
      // *** add in contributions for turbulence models 
      realArray nuT;
      turbulenceModelsINS(nuT,mg,u,uu,ut,ux,uy,uz,uxx,uyy,uzz,I1,I2,I3,parameters,nu,mg.numberOfDimensions(),grid,t );
      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
	real cv1=7.1, cv1e3=pow(cv1,3.);

        realArray chi3,nuTd,nuTx,nuTy,nuTz;
	chi3=U(nc)/nu;
	chi3=chi3*chi3*chi3;
	
	nuT = nu+U(nc)*chi3/(chi3+cv1e3);
	nuTd= chi3*(chi3+4.*cv1e3)/SQR(chi3+cv1e3);
	nuTx=UX(nc)*nuTd;
	nuTy=UY(nc)*nuTd;
	nuTz=UZ(nc)*nuTd;

	ut(I1,I2,I3,uc)+= nuT*(UXX(uc)+UYY(uc)+UZZ(uc)) 
                           +nuTx*(2.*UX(uc)    ) +nuTy*(UY(uc)+UX(vc)) +nuTz*(UZ(uc)+UX(wc));
	ut(I1,I2,I3,vc)+= nuT*(UXX(vc)+UYY(vc)+UZZ(vc)) 
                           +nuTx*(UY(uc)+UX(vc)) +nuTy*(2.*UY(vc)) +nuTz*(UZ(vc)+UY(wc));
	ut(I1,I2,I3,wc)+= nuT*(UXX(wc)+UYY(wc)+UZZ(wc)) 
                           +nuTx*(UZ(uc)+UX(wc)) +nuTy*(UZ(vc)+UY(wc)) +nuTz*(2.*UZ(wc));
      }
      else
      {
	Overture::abort("Unknown turbulence model");
      }

    }
    


    if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
    { // --- add 2nd order artificial diffusion
      const realArray & inverseVertexDerivative = mg.inverseVertexDerivative();
      
      real cd22=ad22/SQR(mg.numberOfDimensions());
      Range J1(I1.getBase(),I1.getBound()+1);// J1= I1 plus one extra point
      Range J2(I2.getBase(),I2.getBound()+1);
      Range J3(I3.getBase(),I3.getBound()+1);
      realArray adf0(J1,I2,I3),adf1(I1,J2,I3),adf2(I1,I2,J3);
      if( isRectangular )
      {
	adf0=admzzR(J1,I2,I3);  // coefficient at i+1/2
	adf1=adzmzR(I1,J2,I3);  // coefficient at j+1/2
	adf2=adzzmR(I1,I2,J3);  // coefficient at k+1/2
      }
      else
      {
	adf0=admzzC(J1,I2,I3);  // coefficient at i+1/2
	adf1=adzmzC(I1,J2,I3);  // coefficient at j+1/2
	adf2=adzzmC(I1,I2,J3);  // coefficient at k+1/2
      }
      
      ut(I1,I2,I3,uc)+=AD3F(uc);
      ut(I1,I2,I3,vc)+=AD3F(vc);
      ut(I1,I2,I3,wc)+=AD3F(wc);

      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
        // const real cd22n=ad22/mg.numberOfDimensions();
        cd22=ad22/mg.numberOfDimensions();
	if( isRectangular )
	{
	  adf0=admzzRSA(J1,I2,I3);  // coefficient at i+1/2
	  adf1=adzmzRSA(I1,J2,I3);  // coefficient at j+1/2
	  adf2=adzzmRSA(I1,I2,J3);  // coefficient at k+1/2
	}
	else
	{
	  adf0=admzzCSA(J1,I2,I3);  // coefficient at i+1/2
	  adf1=adzmzCSA(I1,J2,I3);  // coefficient at j+1/2
	  adf2=adzzmCSA(I1,I2,J3);  // coefficient at k+1/2
	}
        ut(I1,I2,I3,nc)+=AD3F(nc);
      }
    }
    if( useFourthOrderArtificialDiffusion && (ad41!=0. || ad42!=0.) )
    { // --- add 4th order artificial diffusion  ******* extra points needed ********
      real cd42=ad42/SQR(mg.numberOfDimensions());
      ut(I1,I2,I3,uc)+=AD43(uc);
      ut(I1,I2,I3,vc)+=AD43(vc);
      ut(I1,I2,I3,wc)+=AD43(wc);
    }
  }

  

  if( parameters.dbase.get<bool >("advectPassiveScalar") )  // advect passive scalar, add artificial diffusion
  {
    if( mg.numberOfDimensions()==2 )
      ut(I1,I2,I3,sc)=   UU(uc)*UX(sc) + UU(vc)*UY(sc) + 
                  parameters.dbase.get<real >("nuPassiveScalar")*(UXX(sc)+UYY(sc));
    else
      ut(I1,I2,I3,sc)=   UU(uc)*UX(sc) + UU(vc)*UY(sc) +UU(wc)*UZ(sc) + 
                      parameters.dbase.get<real >("nuPassiveScalar")*(UXX(sc)+UYY(sc)+UZZ(sc));

    if( true || parameters.dbase.get<real >("nuPassiveScalar")==0. )
    {
      // add some linear artificial diffusion
      if( mg.numberOfDimensions()==2 )
	ut(I1,I2,I3,sc)+=
	  adcPassiveScalar*(u(I1+1,I2,I3,sc)+u(I1-1,I2,I3,sc)+u(I1,I2+1,I3,sc)+u(I1,I2-1,I3,sc)-4.*u(I1,I2,I3,sc));
      else if( mg.numberOfDimensions()==3 )
	ut(I1,I2,I3,sc)+=
	  adcPassiveScalar*( u(I1+1,I2,I3,sc)+u(I1-1,I2,I3,sc)+u(I1,I2+1,I3,sc)+u(I1,I2-1,I3,sc)
		+u(I1,I2,I3+1,sc)+u(I1,I2,I3-1,sc)  -6.*u(I1,I2,I3,sc));
      else
	ut(I1,I2,I3,sc)+= adcPassiveScalar*(u(I1+1,I2,I3,sc)+u(I1-1,I2,I3,sc)-2.*u(I1,I2,I3,sc));

    }
//      else if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
//      { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
//        real cd22=ad22/SQR(mg.numberOfDimensions());
//        ut(I1,I2,I3,sc)+=AD2(sc);
//      }
  }


//    if( parameters.dbase.get<int >("orderOfAccuracy")==4 )
//      op.setOrderOfAccuracy(2); // reset

  return 0;
}


#define uv(m)   e   (mg,I1,I2,I3,m,t)
#define uvt(m)  e.t (mg,I1,I2,I3,m,t)
#define uvx(m)  e.x (mg,I1,I2,I3,m,t)
#define uvy(m)  e.y (mg,I1,I2,I3,m,t)
#define uvz(m)  e.z (mg,I1,I2,I3,m,t)
#define uvxx(m) e.xx(mg,I1,I2,I3,m,t)
#define uvxy(m) e.xy(mg,I1,I2,I3,m,t)
#define uvxz(m) e.xz(mg,I1,I2,I3,m,t)
#define uvyy(m) e.yy(mg,I1,I2,I3,m,t)
#define uvyz(m) e.yz(mg,I1,I2,I3,m,t)
#define uvzz(m) e.zz(mg,I1,I2,I3,m,t)



//\begin{>>MappedGridSolverInclude.tex}{\subsection{addForcingINS}}
void OB_MappedGridSolver::
addForcingINS(MappedGrid & mg, 
	      realMappedGridFunction & dvdt, 
	      int iparam[], real rparam[],
	      realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */ )
//========================================================================================================
// /Description:
//   Add the forcing term to $u_t$ on a component grid for the Incompressible NS.
// Here is where we added the analytic derivatives for twilight-zone flow.
//
// /mg (input) : grid
// /dvdt (intput/output) : return $u_t$ in this grid function.
// /t (input) : current time.
// /grid (input) : the component grid number if this MappedGrid is part of a GridCollection or CompositeGrid.
// /dvdtImplicit (input) : for implicit time stepping, the time derivative is split into two parts,
//     $u_t=u_t^E + u_t^I$. The explicit part, $u_t^E$, is returned in dvdt while the implicit part, $u_t^I$,
//   is returned in dvdtImplicit. This splitting does NOT depend on whether we are using backward Euler or
//   Crank-Nicolson since this weighting is applied elsewhere. 
// /tImplicit (input) : for implicit time stepping, apply forcing for the implicit part at his 
//     time.
//\end{MappedGridSolverInclude.tex}  
//=======================================================================================================
{
  if( debug() & 8 && parameters.dbase.get<int >("myid")==0 )
  {
    printf(" addForcingINS: start...\n");
  }

  const real & t0=rparam[0];
  real t         =rparam[1];          // this is realy tForce
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];
  const int level=iparam[1];
  const int numberOfStepsTaken = iparam[2];
  const bool gridIsMoving = parameters.gridIsMoving(grid);
  const int numberOfDimensions=mg.numberOfDimensions();
  

  if( parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    realArray & ut = dvdt;
    realArray & uti = dvdtImplicit;

    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
    const int & pc = parameters.dbase.get<int >("pc");
    const int & tc = parameters.dbase.get<int >("tc");
    const int & kc = parameters.dbase.get<int >("kc");
    const int & epsc = parameters.dbase.get<int >("epsc");
    const int nc=kc;

    const int ec = kc+1; 

    real nu = parameters.dbase.get<real >("nu");  // note: we make a local copy so we can scale it.
    real nuI=nu;    // implicit nu
    real nuE=nu;    // explicit nu
    if( parameters.getGridIsImplicit(grid) ) // && parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::crankNicolson )
    {
      nuE*=(1.-parameters.dbase.get<real >("implicitFactor"));
      nuI*=parameters.dbase.get<real >("implicitFactor");
    }
    const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");
    const real kThermal = parameters.dbase.get<real >("kThermal");

    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    Index I1,I2,I3;

    const real ad21 = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad21") : 0.;
    const real cd22 = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad22")/SQR(mg.numberOfDimensions()) : 0.;

    const real ad41 = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad41") : 0.;
    const real cd42 = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") ? parameters.dbase.get<real >("ad42")/SQR(mg.numberOfDimensions()) : 0.;

    const real ad21n = parameters.dbase.get<real >("ad21n");
    const real cd22n = parameters.dbase.get<real >("ad22n")/mg.numberOfDimensions();


    // ---add forcing for twlight-zone flow---

    #ifdef USE_PPP
      realSerialArray utLocal; getLocalArrayWithGhostBoundaries(ut,utLocal);
      realSerialArray utiLocal; getLocalArrayWithGhostBoundaries(uti,utiLocal);
      // intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
    #else
      const realSerialArray & utLocal = ut;
      const realSerialArray & utiLocal = uti;
      // const intSerialArray & mask = mg.mask();
    #endif  

    const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

    if( !isRectangular )
      mg.update(MappedGrid::THEcenter);

    realArray & x= mg.center();
    #ifdef USE_PPP
      realSerialArray xLocal; 
      if( !isRectangular ) 
        getLocalArrayWithGhostBoundaries(x,xLocal);
    #else
      const realSerialArray & xLocal = x;
    #endif



    // **** note: the tzForcing arrays are shared with addForcingToPressureEquation
    const int numberOfTZArrays=mg.numberOfDimensions()==1 ? 1 : mg.numberOfDimensions()==2 ? 10 : 14;

    bool evaluteTZ=tzTimeStart1==REAL_MAX;  // set to true if we need to evaluate the TZ functions

    if( tzForcing==NULL )
    {
      evaluteTZ=true;  // evaluate the TZ functions
      
      tzForcing = new realSerialArray [numberOfTZArrays];
      int extra=1;
      getIndex(extendedGridIndexRange(mg),I1,I2,I3,extra);  // allocate space to hold  BC forcing in ghost points
      bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,I1,I2,I3); 
      if( ok )
      {
	tzForcing[0].redim(I1,I2,I3);
	tzForcing[1].redim(I1,I2,I3);
	tzForcing[2].redim(I1,I2,I3);
      }
    }
    // we cannot use the opt evaluation for moving grids since the grid points change
    if( gridIsMoving )
      evaluteTZ=true;  // we are forced to re-evaluate the TZ functions every time step
    
    getIndex(mg.extendedIndexRange(),I1,I2,I3);

   
    real scaleFactor=1., scaleFactorT=1.;
    if( evaluteTZ )
    {
      tzTimeStart1=t;  // save the time at which the TZ functions were evaluated
    }
    else 
    {
      // This is not the first time through -- compute scale factors for stored TZ values

      // Here we assume that the TZ function is a tensor product of a spatial function
      // times a function of time. In this case we just need to scale the TZ function
      // by the new value of the time function
      real xa=.123,ya=.456,za= mg.numberOfDimensions()==2 ? .789 : 0.;
      real ta=tzTimeStart1;
	
      real ua = e(xa,ya,za,uc,ta);
      assert( fabs(ua) > 1.e-3 );
      scaleFactor = e(xa,ya,za,uc,t)/e(xa,ya,za,uc,ta); // we assume all time functions are the same
      real scaleFactorv = e(xa,ya,za,vc,t)/e(xa,ya,za,vc,ta); // we assume all time functions are the same
      real scaleFactorp = e(xa,ya,za,pc,t)/e(xa,ya,za,pc,ta); // we assume all time functions are the same

      real sfta=e.t(xa,ya,za,uc,ta);
      if( fabs(sfta)>REAL_EPSILON*100. )
        scaleFactorT=e.t(xa,ya,za,uc,t)/sfta;
      else
        scaleFactorT=1.;
      
      assert( fabs(scaleFactor)<1.e10 && fabs(scaleFactorv)<1.e10 && 
              fabs(scaleFactorp)<1.e10 && fabs(scaleFactorT)<1.e10 );
      
      // printf(" scaleFactoru=%e, scaleFactorv=%e, scaleFactorp=%e\n",scaleFactor,scaleFactorv,scaleFactorp);
	
    }


    real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0;
    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, 
                                             cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0);

    real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI;
    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
      getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI );

    getIndex( mg.gridIndexRange(),I1,I2,I3);


    #ifdef USE_PPP
      bool useOpt=true;
    #else
      bool useOpt=false || parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel 
                        || parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel;
    #endif
    if( useOpt )
    {


      // loop bounds for this boundary:
      int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2];
      #ifdef USE_PPP
        n1a=max(I1.getBase(),utLocal.getBase(0)); n1b=min(I1.getBound(),utLocal.getBound(0));
        n2a=max(I2.getBase(),utLocal.getBase(1)); n2b=min(I2.getBound(),utLocal.getBound(1));
        n3a=max(I3.getBase(),utLocal.getBase(2)); n3b=min(I3.getBound(),utLocal.getBound(2));
      #else
        n1a=I1.getBase(); n1b=I1.getBound();
        n2a=I2.getBase(); n2b=I2.getBound();
        n3a=I3.getBase(); n3b=I3.getBound();
      #endif
  
  
      if( n1a>n1b || n2a>n2b || n3a>n3b ) return;
  
      I1=Range(n1a,n1b);
      I2=Range(n2a,n2b);
      I3=Range(n3a,n3b);
  
	
      if( mg.numberOfDimensions()==1 )
      {
        realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0xx(I1,I2,I3);
        realSerialArray p0x(I1,I2,I3); 

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);

	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);


	utLocal(I1,I2,I3,uc)+=u0t+advectionCoefficient*(u0*u0x) + p0x; // -nu*u0xx; 
	if( !parameters.getGridIsImplicit(grid) )
	{ // explicit time stepping:
	  utLocal(I1,I2,I3,uc)-= nu*(u0xx);
	}
	else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	{
	  // Here we add on explicit AND implicit forcing
	  utiLocal(I1,I2,I3,uc)-= nuE*(u0xx);
	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,tImplicit);
	  utiLocal(I1,I2,I3,uc)-= nuI*(u0xx);
	}
	
      }
      else if( mg.numberOfDimensions()==2 )
      {

	realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3);
	realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	
        realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3);
        realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3);

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);


        utLocal(I1,I2,I3,uc)+=u0t+advectionCoefficient*(u0*u0x+v0*u0y) + p0x; // -nu*(u0xx+u0yy);
        utLocal(I1,I2,I3,vc)+=v0t+advectionCoefficient*(u0*v0x+v0*v0y) + p0y; // -nu*(v0xx+v0yy);

        if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::standardModel )
	{
	  if( !parameters.getGridIsImplicit(grid) )
	  { // explicit time stepping:
	    utLocal(I1,I2,I3,uc)-= nu*(u0xx+u0yy);
	    utLocal(I1,I2,I3,vc)-= nu*(v0xx+v0yy);
	    if( parameters.isAxisymmetric() )
	    {
	      Overture::abort("finish this");
//  	      ut(I1,I2,I3,uc)-= nuUrOverR;
//  	      ut(I1,I2,I3,vc)-= nuVrOverR;
	    }
	  }
	  else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	  {
	    // Here we add on explicit AND implicit forcing
	    utiLocal(I1,I2,I3,uc)-= nuE*(u0xx+u0yy);
	    e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,tImplicit);
	    e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,tImplicit);
	    utiLocal(I1,I2,I3,uc)-= nuI*(u0xx+u0yy);

	    utiLocal(I1,I2,I3,vc)-= nuE*(v0xx+v0yy);
	    e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,tImplicit);
	    e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,tImplicit);
	    utiLocal(I1,I2,I3,vc)-= nuI*(v0xx+v0yy);
	    
	    if( parameters.isAxisymmetric() )
	    {
	      Overture::abort("finish this");
// 	      uti(I1,I2,I3,uc)-= nuUrOverR;
// 	      uti(I1,I2,I3,vc)-= nuVrOverR;
	    }
	  }
	}
	
        if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{ 	
	  // visco-plastic model

          // declare and lookup visco-plastic parameters (macro)
          declareViscoPlasticParameters;

	  realSerialArray nuT(I1,I2,I3),nuTx(I1,I2,I3),nuTy(I1,I2,I3),nuTd(I1,I2,I3); 
	  realSerialArray eDotNorm(I1,I2,I3),exp0(I1,I2,I3); 
	  realSerialArray u0xy(I1,I2,I3),v0xy(I1,I2,I3); 

          printf(" **addForcing: nuViscoPlastic=%10.2e\n",nuViscoPlastic);
	  
	  e.gd( u0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,uc,t);
	  e.gd( v0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,vc,t);

	  // eDotNorm = sqrt( u0x*u0x + u0y*u0y + v0x*v0x + v0y*v0y )+ epsViscoPlastic;
          eDotNorm=strainRate2d();

          defineViscoPlasticCoefficients(eDotNorm);
	  
// 	  nuT = nu+ nuViscoPlastic*eDotNormSq;   // fake form 
// 	  nuTd=nuViscoPlastic;   // d(nuT)/d(eDotNormSq)

 	  // nuTx=nuTd*2.*( u0x*u0xx + u0y*u0xy + v0x*v0xx + v0y*v0xy ); 
 	  // nuTy=nuTd*2.*( u0x*u0xy + u0y*u0yy + v0x*v0xy + v0y*v0yy ); 
 	  nuTx=nuTd*strainRate2dSqx(); 
 	  nuTy=nuTd*strainRate2dSqy();

          utLocal(I1,I2,I3,uc)-=nuT*(u0xx+u0yy)+nuTx*(2.*u0x )+nuTy*(u0y+v0x);
          utLocal(I1,I2,I3,vc)-=nuT*(v0xx+v0yy)+nuTx*(u0y+v0x)+nuTy*(2.*v0y);

	}


        // *** this must be done last since we over-write u0x, ... ***
        if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel || 
            parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{ // add terms for Boussinesq approximation
	  real thermalExpansivity=1.;
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);

          assert( tc>=0 );

          // Evaluate the derivative of T and save in u0x, ...
	  e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
	  e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
  	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);

          utLocal(I1,I2,I3,tc)+=u0t+u0*u0x+v0*u0y - kThermal*(u0xx+u0yy);

          // evaluate T and save in u0 
          e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);

          utLocal(I1,I2,I3,uc)+=thermalExpansivity*parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0]*u0; 
          utLocal(I1,I2,I3,vc)+=thermalExpansivity*parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]*u0; 
	}

	
      }
      else if( mg.numberOfDimensions()==3 )
      {

	realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3),u0z(I1,I2,I3);
	realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3),v0z(I1,I2,I3);
	realSerialArray w0(I1,I2,I3),w0t(I1,I2,I3),w0x(I1,I2,I3),w0y(I1,I2,I3),w0z(I1,I2,I3);
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3),p0z(I1,I2,I3);
	
        realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0zz(I1,I2,I3);
        realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0zz(I1,I2,I3);
        realSerialArray w0xx(I1,I2,I3),w0yy(I1,I2,I3),w0zz(I1,I2,I3);

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);
	e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,uc,t);

	e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	e.gd( v0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,vc,t);
	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);
	e.gd( v0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,vc,t);

	e.gd( w0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,wc,t);
	e.gd( w0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,wc,t);
	e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
	e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
	e.gd( w0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,wc,t);
	e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,t);
	e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,t);
	e.gd( w0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,wc,t);


	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);
	e.gd( p0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,pc,t);


        utLocal(I1,I2,I3,uc)+=u0t+advectionCoefficient*(u0*u0x+v0*u0y+w0*u0z) + p0x; // -nu*(u0xx+u0yy+u0zz);
        utLocal(I1,I2,I3,vc)+=v0t+advectionCoefficient*(u0*v0x+v0*v0y+w0*v0z) + p0y; // -nu*(v0xx+v0yy+v0zz);
        utLocal(I1,I2,I3,wc)+=w0t+advectionCoefficient*(u0*w0x+v0*w0y+w0*w0z) + p0z; // -nu*(w0xx+w0yy+w0zz);

	if( !parameters.getGridIsImplicit(grid) )
	{ // explicit time stepping:
	  utLocal(I1,I2,I3,uc)-= nu*(u0xx+u0yy+u0zz);
	  utLocal(I1,I2,I3,vc)-= nu*(v0xx+v0yy+v0zz);
	  utLocal(I1,I2,I3,wc)-= nu*(w0xx+w0yy+w0zz);
	}
	else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	{
	  // Here we add on explicit AND implicit forcing
	  utiLocal(I1,I2,I3,uc)-= nuE*(u0xx+u0yy+u0zz);
	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,tImplicit);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,tImplicit);
	  e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,uc,tImplicit);
	  utiLocal(I1,I2,I3,uc)-= nuI*(u0xx+u0yy+u0zz);

	  utiLocal(I1,I2,I3,vc)-= nuE*(v0xx+v0yy+v0zz);
	  e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,tImplicit);
	  e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,tImplicit);
	  e.gd( v0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,vc,tImplicit);
	  utiLocal(I1,I2,I3,vc)-= nuI*(v0xx+v0yy+v0zz);
	    
	  utiLocal(I1,I2,I3,wc)-= nuE*(w0xx+w0yy+w0zz);
	  e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,tImplicit);
	  e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,tImplicit);
	  e.gd( w0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,wc,tImplicit);
	  utiLocal(I1,I2,I3,wc)-= nuI*(w0xx+w0yy+w0zz);
	}
	
	if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{     
          Overture::abort("viscoPlasticModel in 3D: Option no implemented yet");
	}

        // *** this must be done last since we over-write u0x, ... ***
        if( parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::BoussinesqModel || 
            parameters.dbase.get<Parameters::PDEModel >("pdeModel")==Parameters::viscoPlasticModel )
	{ // add terms for Boussinesq approximation
	  real thermalExpansivity=1.;
	  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);

          assert( tc>=0 );

          // Evaluate the derivative of T and save in u0x, ...
	  e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
	  e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
	  e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
	  e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,tc,t);
	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);
	  e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,tc,t);

	  utLocal(I1,I2,I3,tc)+=u0t+u0*u0x+v0*u0y+w0*u0z-kThermal*(u0xx+u0yy+u0zz);


          // evaluate T and save in u0 
          e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);

          utLocal(I1,I2,I3,uc)+=thermalExpansivity*parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0]*u0; 
          utLocal(I1,I2,I3,vc)+=thermalExpansivity*parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]*u0; 
          utLocal(I1,I2,I3,wc)+=thermalExpansivity*parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]*u0; 
	}


	
      }
      else
      {
	Overture::abort("error");
      }
      

      return;
    }
    



    if( mg.numberOfDimensions()==1 )
    {
      // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 
      where( mg.mask()(I1,I2,I3) != 0 )
      {
	ut(I1,I2,I3,uc)+= uvt(uc)+advectionCoefficient*(uv(uc)*uvx(uc)) +uvx(pc)-nu*(uvxx(uc));
      }
    }
    else if( mg.numberOfDimensions()==2 )
    {
      if( evaluteTZ )
      { // evaluate the TZ functions
        
	realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3);
	realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
	realSerialArray p0x(I1,I2,I3),p0y(I1,I2,I3);
	
        realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3);
        realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3);

	e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
	e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
	e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
	e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
	e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

	e.gd( p0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,pc,t);
	e.gd( p0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,pc,t);


	tzForcing[0](I1,I2,I3) = u0t; 
	tzForcing[1](I1,I2,I3) = u0*u0x+v0*u0y;
	tzForcing[2](I1,I2,I3) = p0x;

	tzForcing[3] = v0t; 
	tzForcing[4] = u0*v0x+v0*v0y;
	tzForcing[5] = p0y;
       

	tzForcing[6] = u0xx+u0yy; 
	tzForcing[7] = v0xx+v0yy; 

      }
      

      // **NOTE** for  moving grids we may need to evaluate at more points than just mask >0 
      // where( mg.mask()(I1,I2,I3) != 0 )

//        ut(I1,I2,I3,uc)+= uvt(uc)+advectionCoefficient*(u0*u0x+v0*u0y) +p0x;
//        ut(I1,I2,I3,vc)+= uvt(vc)+advectionCoefficient*(u0*v0x+v0*v0y) +p0y;

      // fprintf(parameters.dbase.get<FILE* >("debugFile")," *** evaluteTZ=%i *****\n",evaluteTZ);
      // display(mg.vertex()(I1,I2,I3,0),sPrintF(" mg.vertex(I1,I2,I3,0) at t=%e",t),parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");

      // display(ut(I1,I2,I3,uc),sPrintF("ut(I1,I2,I3,uc) before adding TZ at t=%e\n",t),parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
      // display(tzForcing[1](I1,I2,I3)+tzForcing[2](I1,I2,I3),
      //           sPrintF("u*ux+px (TZ) at t=%e\n",t),parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
      

      utLocal(I1,I2,I3,uc)+= scaleFactorT*tzForcing[0](I1,I2,I3)+
                        (advectionCoefficient*SQR(scaleFactor))*tzForcing[1](I1,I2,I3)+
                        scaleFactor*tzForcing[2](I1,I2,I3);
      utLocal(I1,I2,I3,vc)+= scaleFactorT*tzForcing[3]
                       +(advectionCoefficient*SQR(scaleFactor))*tzForcing[4]+scaleFactor*tzForcing[5];

// *********************** start 123
#ifndef USE_PPP
      if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ||
          parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
      {
	const realArray & u0  = e   (mg,I1,I2,I3,uc,t); // this could be optimized
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);

//  	realArray adc;
//          adc = ad21+cd22*(fabs(u0x)+fabs(u0y)+fabs(v0x)+fabs(v0y)); // ** coeff of art. dissipation

//  	ut(I1,I2,I3,uc)-=adc*(e(mg,I1-1,I2,I3,uc,t)+e(mg,I1+1,I2,I3,uc,t)+
//                                e(mg,I1,I2-1,I3,uc,t)+e(mg,I1,I2+1,I3,uc,t)-4.*u0);
	
//  	ut(I1,I2,I3,vc)-=adc*(e(mg,I1-1,I2,I3,vc,t)+e(mg,I1+1,I2,I3,vc,t)+
//                                e(mg,I1,I2-1,I3,vc,t)+e(mg,I1,I2+1,I3,vc,t)-4.*v0);
	realArray adc;
        adc = fabs(u0x)+fabs(u0y)+fabs(v0x)+fabs(v0y);
        if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
	{
#undef ADTZ2
#define ADTZ2(cc,q0) adc2*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		           e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t)+\
			    -4.*q0 )
          realArray adc2;
	  adc2 = ad21+cd22*adc;
	  ut(I1,I2,I3,uc)-=ADTZ2(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ2(vc,v0);
	}
        if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
	{
#undef ADTZ4
#define ADTZ4(cc,q0) adc4*(-(e(mg,I1-2,I2,I3,cc,t)+e(mg,I1+2,I2,I3,cc,t)+\
		 	     e(mg,I1,I2-2,I3,cc,t)+e(mg,I1,I2+2,I3,cc,t))\
		        +4.*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		             e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t))\
			    -12.*q0 )

          realArray adc4;
	  adc4 = ad41+cd42*adc;
	  ut(I1,I2,I3,uc)-=ADTZ4(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ4(vc,v0);
	}
      }
      

      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
      {
	realArray radiusInverse;
	realArray & vertex = mg.vertex();
	if( nu!=0. )
	{
	  realArray nuUrOverR, nuVrOverR;
	  if( parameters.isAxisymmetric() && 
	      ( !parameters.getGridIsImplicit(grid) ||
		parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately ) )
	  {
	    const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);

	    const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	    const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);

  	    const realArray & u0yy= e.yy(mg,I1,I2,I3,uc,t);
  	    const realArray & v0yy= e.yy(mg,I1,I2,I3,vc,t);

	    // y corresponds to the radial direction
	    // y=0 is the axis of symmetry
	    realArray radiusInverse;
	    radiusInverse=1./max(REAL_MIN,vertex(I1,I2,I3,axis2));

	    if( !parameters.getGridIsImplicit(grid) )
	    {
	      nuUrOverR=nu*u0y*radiusInverse;
	      nuVrOverR=nu*(v0y-v0*radiusInverse)*radiusInverse;
	    }
	    else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	    {
	      nuUrOverR=(nuE*u0y+nuI*e.y(mg,I1,I2,I3,uc,tImplicit))*radiusInverse;
	      nuVrOverR=(nuE*(v0y-v0*radiusInverse)+
			 nuI*(e.y(mg,I1,I2,I3,vc,tImplicit)-e(mg,I1,I2,I3,vc,tImplicit)*radiusInverse) )*radiusInverse;
	    }
	    // fix points on axis
	    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
		if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
		{
		  Index Ib1,Ib2,Ib3;
		  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  if( !parameters.getGridIsImplicit(grid) )
		  {
		    nuUrOverR(Ib1,Ib2,Ib3)=nu*u0yy(Ib1,Ib2,Ib3);
		    nuVrOverR(Ib1,Ib2,Ib3)=.5*nu*v0yy(Ib1,Ib2,Ib3);
		  }
		  else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
		  {
		    nuUrOverR(Ib1,Ib2,Ib3)=nuE*u0yy(Ib1,Ib2,Ib3)+nuI*e.yy(mg,Ib1,Ib2,Ib3,uc,tImplicit); 
		    nuVrOverR(Ib1,Ib2,Ib3)=.5*nuE*v0yy(Ib1,Ib2,Ib3)+.5*nuI*e.yy(mg,Ib1,Ib2,Ib3,vc,tImplicit); 
		  }
	  
		}
	      }
	    }
	  }

	  if( !parameters.getGridIsImplicit(grid) )
	  { // explicit time stepping:
	    ut(I1,I2,I3,uc)-= (nu*scaleFactor)*tzForcing[6];
	    ut(I1,I2,I3,vc)-= (nu*scaleFactor)*tzForcing[7];
	    if( parameters.isAxisymmetric() )
	    {
	      ut(I1,I2,I3,uc)-= nuUrOverR;
	      ut(I1,I2,I3,vc)-= nuVrOverR;
	    }
	  }
	  else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	  {
	    // Here we add on explicit AND implicit forcing
	    // printf(" +++++++++++++++ addForcing: computeImplicitTermsSeparately \n");
	    uti(I1,I2,I3,uc)-=(nuE*scaleFactor)*tzForcing[6]
                             +nuI*(e.xx(mg,I1,I2,I3,uc,tImplicit)+e.yy(mg,I1,I2,I3,uc,tImplicit));
	    uti(I1,I2,I3,vc)-=(nuE*scaleFactor)*tzForcing[7]
                             +nuI*(e.xx(mg,I1,I2,I3,vc,tImplicit)+e.yy(mg,I1,I2,I3,vc,tImplicit));
	    if( parameters.isAxisymmetric() )
	    {
	      uti(I1,I2,I3,uc)-= nuUrOverR;
	      uti(I1,I2,I3,vc)-= nuVrOverR;
	    }
	  }
	}
      }
      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
        // here are the SA TM equations 

        assert( !parameters.isAxisymmetric() );
	assert( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL );
	const realArray & d = (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];
	
        const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
        const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
        const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);

        const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
        const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
        const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);

	const realArray & n0   = e  (mg,I1,I2,I3,nc,t);
	const realArray & n0x  = e.x(mg,I1,I2,I3,nc,t);
	const realArray & n0y  = e.y(mg,I1,I2,I3,nc,t);
        

        realArray nuT,chi3,nuTx,nuTy,nuTd;
        chi3 = pow(n0/nu,3.);

        // chi3=0.;  // *******************

	nuT = nu+n0*(chi3/(chi3+cv1e3)); // ******************
        nuTd=chi3*(chi3+4.*cv1e3)/pow(chi3+cv1e3,2.);
	nuTx= n0x*nuTd;// ******************
	nuTy= n0y*nuTd;// ******************
	
	ut(I1,I2,I3,uc)-=nuT*(e.laplacian(mg,I1,I2,I3,uc,t))+nuTx*(2.*u0x )+nuTy*(u0y+v0x);
	ut(I1,I2,I3,vc)-=nuT*(e.laplacian(mg,I1,I2,I3,vc,t))+nuTx*(u0y+v0x)+nuTy*(2.*v0y);
	
	realArray s, r,g,fw,fnu1(I1,I2,I3),fnu2(I1,I2,I3), chi, dSq(I1,I2,I3);

	chi=n0/nu;
	fnu1=chi*chi*chi/( chi*chi*chi+cv1e3);
	fnu2=1.-chi/(1.+chi*fnu1);

	if( mg.numberOfDimensions()==2 )
	{
	  s= fabs(u0y-v0x); // turbulence source term
	}
	else
	{
	  // s=SQRT( SQR(u0y-v0x) + SQR(v0z-w0y) + SQR(w0x-u0z) );
	}
    
	// const real epsD=1.e-20;
	// d(I1,I2,I3)=max(d(I1,I2,I3),epsD);
	dSq=(d(I1,I2,I3)+cd0)*(d(I1,I2,I3)+cd0);
      
	s+= n0*fnu2/( dSq*(kappa*kappa) );
      
        // we could assume that d is set to a nonzero value on the boundary.
	// ** r= n0/( max( s*dSq*(kappa*kappa), cr0 ) );

	// we assume that g reaches a constant for r large enough
	r = min( n0/( s*dSq*(kappa*kappa) ), cr0 );

	g=r+cw2*(pow(r,6.)-r);
	fw=g*pow( (1.+cw3e6)/(pow(g,6.)+cw3e6), 1./6.);

	realArray nSqBydSq;
	nSqBydSq=cw1*fw*(n0*n0/dSq);

        if( debug() & 8 )
	{
	  printf("addTZ: max(cb1*s*n0)=%9.2e, max(nSqBydSq)=%9.2e max(ut(nc))=%9.2e (before)",
                max(fabs(cb1*s*n0)),max(fabs(nSqBydSq)),max(fabs(ut(I1,I2,I3,nc))));
	}
	
	
	ut(I1,I2,I3,nc)+=e.t(mg,I1,I2,I3,nc,t) +u0*n0x+v0*n0y
	  - sigmai*(nu+n0)*(e.laplacian(mg,I1,I2,I3,nc,t))
          - ((1.+cb2)*sigmai)*(n0x*n0x+n0y*n0y)
          - cb1*s*n0 + nSqBydSq 
          -( (ad21n+cd22n*( fabs(n0x)+fabs(n0y) ))*(
	    e(mg,I1-1,I2,I3,nc,t)+e(mg,I1+1,I2,I3,nc,t)+e(mg,I1,I2-1,I3,nc,t)+e(mg,I1,I2+1,I3,nc,t)-4*n0) );
	

        if( debug() & 8 )
	{
	  printf("  max(ut(nc))=%9.2e (after)\n",max(fabs(ut(I1,I2,I3,nc))));
	}

      }
      else  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
      {
        assert( !parameters.isAxisymmetric() );
	assert( kc>0 && ec>0 );

        const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
        const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
        const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
        const realArray & u0Lap= e.laplacian(mg,I1,I2,I3,uc,t);

        const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
        const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
        const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
        const realArray & v0Lap= e.laplacian(mg,I1,I2,I3,vc,t);

	const realArray & k0   = e  (mg,I1,I2,I3,kc,t);
	const realArray & k0x  = e.x(mg,I1,I2,I3,kc,t);
	const realArray & k0y  = e.y(mg,I1,I2,I3,kc,t);
        const realArray & k0Lap= e.laplacian(mg,I1,I2,I3,kc,t);

	const realArray & e0   = e  (mg,I1,I2,I3,ec,t);
	const realArray & e0x  = e.x(mg,I1,I2,I3,ec,t);
	const realArray & e0y  = e.y(mg,I1,I2,I3,ec,t);
        const realArray & e0Lap= e.laplacian(mg,I1,I2,I3,ec,t);
        

        realArray nuT,nuTx,nuTy,nuP,e02,prod;

	e02=e0*e0;
	
	nuT = cMu*k0*k0/e0;
	nuP=nu+nuT;
	nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e02;
	nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e02;


	ut(I1,I2,I3,uc)-=nuP*u0Lap+nuTx*(2.*u0x )+nuTy*(u0y+v0x);
	ut(I1,I2,I3,vc)-=nuP*v0Lap+nuTx*(u0y+v0x)+nuTy*(2.*v0y);
	
	prod = nuT*( 2.*(u0x*u0x+v0y*v0y) + SQR(v0x+u0y) );

//          printf("insTZ: kc,ec,epsc=%2i,%2i,%2i cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,\n",
//  	       kc,ec,epsc,cMu,cEps1,cEps2,sigmaEpsI,sigmaKI);
	
	ut(I1,I2,I3,kc)-=-e.t(mg,I1,I2,I3,kc,t) -u0*k0x-v0*k0y +prod -e0
            +(nu+sigmaKI*nuT)*k0Lap+sigmaKI*(nuTx*k0x+nuTy*k0y);
   
	ut(I1,I2,I3,ec)-=-e.t(mg,I1,I2,I3,ec,t) -u0*e0x-v0*e0y
           +cEps1*(e0/k0)*prod-cEps2*(e02/k0) +(nu+sigmaEpsI*nuT)*e0Lap+sigmaEpsI*(nuTx*e0x+nuTy*e0y);
   
      }
      else 
      {
	if( parameters.dbase.get<int >("myid")==0 )
          printf("Unknown turbulenceModel!\n");
	Overture::abort();
      }
    }
    else if( mg.numberOfDimensions()==3 )
    {
      if( evaluteTZ )
      {
	const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);

	const realArray & p0x = e.x (mg,I1,I2,I3,pc,t);
	const realArray & p0y = e.y (mg,I1,I2,I3,pc,t);
	const realArray & p0z = e.z (mg,I1,I2,I3,pc,t);

	tzForcing[0](I1,I2,I3) = e.t (mg,I1,I2,I3,uc,t);
	tzForcing[1](I1,I2,I3) = u0*u0x+v0*u0y+w0*u0z;
	tzForcing[2](I1,I2,I3) = p0x;

	tzForcing[3] = e.t (mg,I1,I2,I3,vc,t);
	tzForcing[4] = u0*v0x+v0*v0y+w0*v0z;
	tzForcing[5] = p0y;

	tzForcing[6] = e.t (mg,I1,I2,I3,wc,t);
	tzForcing[7] = u0*w0x+v0*w0y+w0*w0z;
	tzForcing[8] = p0z;
       
	tzForcing[ 9] = e.laplacian(mg,I1,I2,I3,uc,t);
	tzForcing[10] = e.laplacian(mg,I1,I2,I3,vc,t);
	tzForcing[11] = e.laplacian(mg,I1,I2,I3,wc,t);

      }
      
//        ut(I1,I2,I3,uc)+= uvt(uc)+advectionCoefficient*(u0*u0x+v0*u0y+w0*u0z) +p0x;
//        ut(I1,I2,I3,vc)+= uvt(vc)+advectionCoefficient*(u0*v0x+v0*v0y+w0*v0z) +p0y;
//        ut(I1,I2,I3,wc)+= uvt(wc)+advectionCoefficient*(u0*w0x+v0*w0y+w0*w0z) +p0z;

      ut(I1,I2,I3,uc)+= scaleFactorT*tzForcing[0](I1,I2,I3)+
                       (advectionCoefficient*SQR(scaleFactor))*tzForcing[1](I1,I2,I3)+
                        scaleFactor*tzForcing[2](I1,I2,I3);
      ut(I1,I2,I3,vc)+= scaleFactorT*tzForcing[3]
                       +(advectionCoefficient*SQR(scaleFactor))*tzForcing[4]+scaleFactor*tzForcing[5];
      ut(I1,I2,I3,wc)+= scaleFactorT*tzForcing[6]
                       +(advectionCoefficient*SQR(scaleFactor))*tzForcing[7]+scaleFactor*tzForcing[8];



      if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") ||
          parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
      {
	const realArray & u0  = e   (mg,I1,I2,I3,uc,t); // this could be optimized
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);

	realArray adc;
        adc = (fabs(u0x)+fabs(u0y)+fabs(u0z)+
	       fabs(v0x)+fabs(v0y)+fabs(v0z)+
	       fabs(w0x)+fabs(w0y)+fabs(w0z));
        if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
	{
#undef ADTZ2
#define ADTZ2(cc,q0) adc2*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		           e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t)+\
			   e(mg,I1,I2,I3-1,cc,t)+e(mg,I1,I2,I3+1,cc,t)\
			    -6.*q0 )
          realArray adc2;
	  adc2 = ad21+cd22*adc;
	  ut(I1,I2,I3,uc)-=ADTZ2(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ2(vc,v0);
	  ut(I1,I2,I3,wc)-=ADTZ2(wc,w0);
	  
	}
        if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
	{
#undef ADTZ4
#define ADTZ4(cc,q0) adc4*(-(e(mg,I1-2,I2,I3,cc,t)+e(mg,I1+2,I2,I3,cc,t)+\
		 	     e(mg,I1,I2-2,I3,cc,t)+e(mg,I1,I2+2,I3,cc,t)+\
			     e(mg,I1,I2,I3-2,cc,t)+e(mg,I1,I2,I3+2,cc,t))\
		        +4.*(e(mg,I1-1,I2,I3,cc,t)+e(mg,I1+1,I2,I3,cc,t)+\
		             e(mg,I1,I2-1,I3,cc,t)+e(mg,I1,I2+1,I3,cc,t)+\
			     e(mg,I1,I2,I3-1,cc,t)+e(mg,I1,I2,I3+1,cc,t))\
			    -18.*q0 )

          realArray adc4;
	  adc4 = ad41+cd42*adc;
	  ut(I1,I2,I3,uc)-=ADTZ4(uc,u0);
	  ut(I1,I2,I3,vc)-=ADTZ4(vc,v0);
	  ut(I1,I2,I3,wc)-=ADTZ4(wc,w0);
	}
      }
      if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
      {
	if( nu!=0. )
	{
	  if( !parameters.getGridIsImplicit(grid) )
	  { // explicit time stepping:
	    ut(I1,I2,I3,uc)-= (nu*scaleFactor)*tzForcing[ 9];
	    ut(I1,I2,I3,vc)-= (nu*scaleFactor)*tzForcing[10];
	    ut(I1,I2,I3,wc)-= (nu*scaleFactor)*tzForcing[11];
	  }
	  else if( parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")==Parameters::computeImplicitTermsSeparately )
	  {
	    uti(I1,I2,I3,uc)-= (nuE*scaleFactor)*tzForcing[ 9]+nuI*e.laplacian(mg,I1,I2,I3,uc,tImplicit);
	    uti(I1,I2,I3,vc)-= (nuE*scaleFactor)*tzForcing[10]+nuI*e.laplacian(mg,I1,I2,I3,vc,tImplicit);
	    uti(I1,I2,I3,wc)-= (nuE*scaleFactor)*tzForcing[11]+nuI*e.laplacian(mg,I1,I2,I3,wc,tImplicit);
	  }
	}
      }
      else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
      {
        // here are the SA TM equations without all the source terms -- these are just turned off for TZ
	assert( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL );
	const realArray & d = (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];

	const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);

	const realArray & n0   = e  (mg,I1,I2,I3,nc,t);
	const realArray & n0x  = e.x(mg,I1,I2,I3,nc,t);
	const realArray & n0y  = e.y(mg,I1,I2,I3,nc,t);
	const realArray & n0z  = e.z(mg,I1,I2,I3,nc,t);

        realArray nuT,chi3,nuTx,nuTy,nuTz,nuTd;
        chi3 = pow(n0/nu,3.);
	
	nuT = nu+n0*(chi3/(chi3+cv1e3));
        nuTd=chi3*(chi3+4.*cv1e3)/pow(chi3+cv1e3,2.);

	nuTx= n0x*nuTd;
	nuTy= n0y*nuTd;
	nuTz= n0z*nuTd;
	
	ut(I1,I2,I3,uc)-=nuT*(e.laplacian(mg,I1,I2,I3,uc,t))+nuTx*(2.*u0x )+nuTy*(u0y+v0x)+nuTz*(u0z+w0x);
	ut(I1,I2,I3,vc)-=nuT*(e.laplacian(mg,I1,I2,I3,vc,t))+nuTx*(u0y+v0x)+nuTy*(2.*v0y )+nuTz*(v0z+w0y);
	ut(I1,I2,I3,wc)-=nuT*(e.laplacian(mg,I1,I2,I3,wc,t))+nuTx*(u0z+w0x)+nuTy*(v0z+w0y)+nuTz*(2.*w0z );

	realArray s, r,g,fw,fnu1(I1,I2,I3),fnu2(I1,I2,I3), chi, dSq(I1,I2,I3);

	chi=n0/nu;
	fnu1=chi*chi*chi/( chi*chi*chi+cv1e3);
	fnu2=1.-chi/(1.+chi*fnu1);

	s=SQRT( SQR(u0y-v0x) + SQR(v0z-w0y) + SQR(w0x-u0z) );
    
//	const real epsD=1.e-20;
//	d(I1,I2,I3)=max(d(I1,I2,I3),epsD);
//	dSq=d(I1,I2,I3)*d(I1,I2,I3);
	dSq=(d(I1,I2,I3)+cd0)*(d(I1,I2,I3)+cd0);
	
	s+= n0*fnu2/( dSq*(kappa*kappa) );
      
        // we could assume that d is set to a nonzero value on the boundary.
	// ** r= n0/( max( s*dSq*(kappa*kappa), cr0 ) );

       	// we assume that g reaches a constant for r large enough
        r = min( n0/( s*dSq*(kappa*kappa) ), cr0 );

	g=r+cw2*(pow(r,6.)-r);
	fw=g*pow( (1.+cw3e6)/(pow(g,6.)+cw3e6), 1./6.);

	realArray nSqBydSq;
	nSqBydSq=cw1*fw*(n0*n0/dSq);

        if( debug() & 2 )
	{
	  printf("addTZ: max(cb1*s*n0)=%9.2e, max(nSqBydSq)=%9.2e max(ut(nc))=%9.2e (before)",
                max(fabs(cb1*s*n0)),max(fabs(nSqBydSq)),max(fabs(ut(I1,I2,I3,nc))));
	}
	
	ut(I1,I2,I3,nc)+=e.t(mg,I1,I2,I3,nc,t) +u0*n0x+v0*n0y+w0*n0z
	  - sigmai*(nu+n0)*(e.xx(mg,I1,I2,I3,nc,t)+e.yy(mg,I1,I2,I3,nc,t)+e.zz(mg,I1,I2,I3,nc,t))
          - ((1.+cb2)*sigmai)*(n0x*n0x+n0y*n0y+n0z*n0z)
          - cb1*s*n0 + nSqBydSq
          -( (ad21n+cd22n*( fabs(n0x)+fabs(n0y)+fabs(n0z) ))*(
	    e(mg,I1-1,I2,I3,nc,t)+e(mg,I1+1,I2,I3,nc,t)+
            e(mg,I1,I2-1,I3,nc,t)+e(mg,I1,I2+1,I3,nc,t)+
            e(mg,I1,I2,I3-1,nc,t)+e(mg,I1,I2,I3-1,nc,t)
                                          -8.*n0) );


        if( debug() & 2 )
	{
	  printf("  max(ut(nc))=%9.2e (after)",max(fabs(ut(I1,I2,I3,nc))));
	}

      }
      else  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
      {
        assert( !parameters.isAxisymmetric() );
	assert( kc>0 && ec>0 );

        const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
        const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
        const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
        const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);
        const realArray & u0Lap= e.laplacian(mg,I1,I2,I3,uc,t);

        const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
        const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
        const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
        const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);
        const realArray & v0Lap= e.laplacian(mg,I1,I2,I3,vc,t);

        const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
        const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
        const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
        const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);
        const realArray & w0Lap= e.laplacian(mg,I1,I2,I3,wc,t);

	const realArray & k0   = e  (mg,I1,I2,I3,kc,t);
	const realArray & k0x  = e.x(mg,I1,I2,I3,kc,t);
	const realArray & k0y  = e.y(mg,I1,I2,I3,kc,t);
	const realArray & k0z  = e.z(mg,I1,I2,I3,kc,t);
        const realArray & k0Lap= e.laplacian(mg,I1,I2,I3,kc,t);

	const realArray & e0   = e  (mg,I1,I2,I3,ec,t);
	const realArray & e0x  = e.x(mg,I1,I2,I3,ec,t);
	const realArray & e0y  = e.y(mg,I1,I2,I3,ec,t);
	const realArray & e0z  = e.z(mg,I1,I2,I3,ec,t);
        const realArray & e0Lap= e.laplacian(mg,I1,I2,I3,ec,t);
        

        realArray nuT,nuTx,nuTy,nuTz,nuP,e02,prod;

	e02=e0*e0;
	
	nuT = cMu*k0*k0/e0;
	nuP=nu+nuT;
	nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e02;
	nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e02;
	nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e02;


	ut(I1,I2,I3,uc)-=nuP*u0Lap+nuTx*(2.*u0x )+nuTy*(u0y+v0x)+nuTz*(u0z+w0x);
	ut(I1,I2,I3,vc)-=nuP*v0Lap+nuTx*(u0y+v0x)+nuTy*(2.*v0y )+nuTz*(v0z+w0y);
	ut(I1,I2,I3,wc)-=nuP*w0Lap+nuTx*(u0z+w0x)+nuTy*(v0z+w0y)+nuTz*(2.*w0z );

	prod = nuT*( 2.*(u0x*u0x+v0y*v0y+w0z*w0z) + SQR(v0x+u0y)+ SQR(w0y+v0z)+ SQR(u0z+w0x) );
	
  	ut(I1,I2,I3,kc)-=-e.t(mg,I1,I2,I3,kc,t) -u0*k0x-v0*k0y-w0*k0z+prod -e0
  	  +(nu+sigmaKI*nuT)*k0Lap+sigmaKI*(nuTx*k0x+nuTy*k0y+nuTz*k0z);
   
  	ut(I1,I2,I3,ec)-=-e.t(mg,I1,I2,I3,ec,t) -u0*e0x-v0*e0y-w0*e0z+cEps1*(e0/k0)*prod-cEps2*(e02/k0)
  	  +(nu+sigmaEpsI*nuT)*e0Lap+sigmaEpsI*(nuTx*e0x+nuTy*e0y+nuTz*e0z);
   
      }
      else 
      {
	printf("Unknown turbulenceModel!\n");
	Overture::abort();
      }

#endif
// *********************** end 123
    }
    else
    {
      cout << "OB_MappedGridSolver::addForcingINS:ERROR: unknown dimension\n";
      throw "error";
    }
  }

}


//\begin{>>MappedGridSolverInclude.tex}{\subsection{getTimeSteppingEigenvalue}} 
void OB_MappedGridSolver::
getTimeSteppingEigenvalueINS(MappedGrid & mg,
			     realMappedGridFunction & u0, 
			     realMappedGridFunction & gridVelocity,  
			     real & reLambda,
			     real & imLambda,
                             const int & grid)
//=====================================================================================================
// /Description:
//   Determine the real and imaginary parts of the eigenvalues for time stepping.
//
// /Author: WDH
//
//\end{MappedGridSolverInclude.tex}  
// ===================================================================================================
{

  bool useOpt=true;

  // printf("****** ins:get lambda: parameters.isAxisymmetric()=%i ******\n",(int)parameters.isAxisymmetric());

  if( useOpt 
      // && !parameters.dbase.get<int >("useLocalTimeStepping") 
      // && !parameters.isAxisymmetric()
      // && parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel 
            )
  {
    MappedGridOperators & op = *u0.getOperators();

    const int isRectangular=op.isRectangular(); // trouble when moving ??
    // const int isRectangular=mg.isRectangular(); 
  
    real nu = parameters.dbase.get<real >("nu");
    if( parameters.dbase.get<bool >("advectPassiveScalar") ) 
      nu=max(nu,parameters.dbase.get<real >("nuPassiveScalar"));   // could do better than this

    // only apply fourth-order AD here if it is explicit
    const bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
      !parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion");

    const real adcPassiveScalar=1.; // coeff or linear artificial diffusion for the passive scalar ** add to params

    const bool & gridIsMoving = parameters.gridIsMoving(grid);
    const int gridIsImplicit=parameters.getGridIsImplicit(grid);

    Index I1,I2,I3;
    getIndex( mg.extendedIndexRange(),I1,I2,I3);

    int useWhereMask=true;
    real dx[3]={1.,1.,1.};
    real xab[2][3]={0.,1.,0.,1.,0.,1.};
    if( isRectangular )
      mg.getRectangularGridParameters( dx, xab );
   
    const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
    const int gridType= isRectangular ? 0 : 1;


    // For non-moving grids u==uu, otherwise uu is a temp space to hold (u-gv)
    if( parameters.gridIsMoving(grid) )
    {
      // *** added 040825 ****  uu was being allocated
      // *note* uu doesn't have as many components as u but fortran routines currently assume the
      // dimensions of uu are the same as for u (note: uc=1, vc=2 for INS)
      realArray & uu = get(WorkSpace::uu);
      MappedGridSolverWorkSpace::resize(uu,u0.dimension(0),u0.dimension(1),u0.dimension(2),
                                           u0.dimension(3)); 
    }
    #ifdef USE_PPP
      realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0,u0Local);
      const real *pu = u0Local.getDataPointer();
      const real *puu = parameters.gridIsMoving(grid) ? get(WorkSpace::uu).getLocalArray().getDataPointer() : pu;
      const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
                         ((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getLocalArray().getDataPointer();

      const real *pDivDamp = divergenceDampingWeight().getLocalArray().getDataPointer();
      const real *pVariableDt = pdtVar !=NULL ? pdtVar->getLocalArray().getDataPointer() : pDivDamp;
  
      // For now we need the center array for the axisymmetric case:
      const real *pxy = parameters.isAxisymmetric() ? mg.center().getLocalArray().getDataPointer() : pu;
      const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getLocalArray().getDataPointer();
      const int *pmask = mg.mask().getLocalArray().getDataPointer();
      const real *pgv = gridIsMoving ? gridVelocity.getLocalArray().getDataPointer() : pu;
    #else  
      const realSerialArray & u0Local = u0;
      const real *pu = u0.getDataPointer();
      const real *puu = parameters.gridIsMoving(grid) ? get(WorkSpace::uu).getDataPointer() : pu;
  
      const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
                         ((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getDataPointer();
  
      const real *pDivDamp = divergenceDampingWeight().getDataPointer();
      const real *pVariableDt = pdtVar !=NULL ? pdtVar->getDataPointer() : pDivDamp;
  
      // For now we need the center array for the axisymmetric case:
      if( parameters.isAxisymmetric() ) 
      {
        assert( mg.center().getLength(0)>0 );
      }
      const real *pxy = parameters.isAxisymmetric() ? mg.center().getDataPointer() : pu;
      const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getDataPointer();
      const int *pmask = mg.mask().getDataPointer();
      const real *pgv = gridIsMoving ? gridVelocity.getDataPointer() : pu;
    #endif

    // declare and lookup visco-plastic parameters (macro)
    declareViscoPlasticParameters;

    int i1a=mg.gridIndexRange(0,0);
    int i2a=mg.gridIndexRange(0,1);
    int i3a=mg.gridIndexRange(0,2);
    int ipar[] ={parameters.dbase.get<int >("pc"),
                 parameters.dbase.get<int >("uc"),
                 parameters.dbase.get<int >("vc"),
                 parameters.dbase.get<int >("wc"),
                 parameters.dbase.get<int >("kc"),
                 parameters.dbase.get<int >("sc"),
                 grid,
		 orderOfAccuracy,
                 (int)parameters.gridIsMoving(grid),
		 useWhereMask,
                 (int)gridIsImplicit, // parameters.gridIsImplicit(grid),
                 (int)parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod"),
		 (int)parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),
                 (int)parameters.isAxisymmetric(),
                 (int)parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion"),
                 (int)useFourthOrderArtificialDiffusion,
                 (int)parameters.dbase.get<bool >("advectPassiveScalar"),
                 gridType,
                 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),
                 (int)parameters.dbase.get<int >("useLocalTimeStepping"),
                 i1a,i2a,i3a,
                 (int)parameters.dbase.get<Parameters::PDEModel >("pdeModel")
                 };

    reLambda=0.;
    imLambda=0.;

    const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
    real rpar[]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),
                 dx[0],dx[1],dx[2],   // 3,4,5
                 nu,
                 parameters.dbase.get<real >("ad21"),
                 parameters.dbase.get<real >("ad22"),
                 parameters.dbase.get<real >("ad41"),
                 parameters.dbase.get<real >("ad42"),
                 parameters.dbase.get<real >("nuPassiveScalar"),  // 11
                 adcPassiveScalar,
                 reLambda,
                 imLambda,
                 parameters.dbase.get<real >("cDt"),
                 parameters.dbase.get<real >("cdv"),
                 parameters.dbase.get<real >("dtMax"),
                 parameters.dbase.get<real >("ad21n"),
                 parameters.dbase.get<real >("ad22n"),
                 parameters.dbase.get<real >("ad41n"),  // 20
                 parameters.dbase.get<real >("ad42n"),
                 xab[0][0],xab[0][1],xab[0][2],yEps,
                 nuViscoPlastic,     // 26
                 etaViscoPlastic,
                 yieldStressViscoPlastic,
                 exponentViscoPlastic,
                 epsViscoPlastic  };

    int ierr=0;
    
    getIndex(mg.gridIndexRange(),I1,I2,I3);  // *wdh* 030220  - evaluate du/dt here
    bool ok = ParallelUtility::getLocalArrayBounds(u0,u0Local,I1,I2,I3);  

    if( ok )
    {
      insdts(mg.numberOfDimensions(),
	     I1.getBase(),I1.getBound(),
	     I2.getBase(),I2.getBound(),
	     I3.getBase(),I3.getBound(),
	     u0Local.getBase(0),u0Local.getBound(0),u0Local.getBase(1),u0Local.getBound(1),
	     u0Local.getBase(2),u0Local.getBound(2),u0Local.getBase(3),u0Local.getBound(3),
	     *pmask, *pxy, *prsxy,
	     *pu, *puu, *pgv, *pdw, *pDivDamp, *pVariableDt,
	     mg.boundaryCondition(0,0), ipar[0], rpar[0], ierr );

      reLambda=rpar[13];
      imLambda=rpar[14];
    }
    else
    {
      reLambda=0.;
      imLambda=0.;
    }
    
    if( debug() & 4 ) 
    {
     printf("insdts: NEW: (reLambda,imLambda)=(%9.3e,%9.3e) (p=%i) hMin=%e\n",
             reLambda,imLambda,parameters.dbase.get<int >("myid"),hMin); 
     
//      ::display(divergenceDampingWeight(),sPrintF(" insdts: divergenceDampingWeight() grid=%i ",grid),
// 	       parameters.dbase.get<FILE* >("debugFile"));
     
    }
    
    // *** For now only use the new way for turbulence models until we check all other cases
    if( true ||  parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel 
        //   && !parameters.dbase.get<int >("useLocalTimeStepping") 
       )
       return;
  }


  // ********** OLD WAY ********************
//    printf("****** ins:get lambda: compute dt the old way ******\n");
//    u0.display("****** ins:get lambda:u0");
  

  realArray & u = u0;
  realArray & uu = get(WorkSpace::uu);
  realArray & ux = get(WorkSpace::ux);
  realArray & uy = get(WorkSpace::uy);
  realArray & uz = get(WorkSpace::uz);
  realArray & uxx= get(WorkSpace::uxx);
  realArray & uyy= get(WorkSpace::uyy);
  realArray & uzz= get(WorkSpace::uzz);

  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  // const int & pc = parameters.dbase.get<int >("pc");
  const bool & gridIsMoving = parameters.gridIsMoving(grid);

  const real & ad21 = parameters.dbase.get<real >("ad21");
  const real & ad22 = parameters.dbase.get<real >("ad22");
  const real & ad41 = parameters.dbase.get<real >("ad41");
  const real & ad42 = parameters.dbase.get<real >("ad42");
  const real & cdv  = parameters.dbase.get<real >("cdv");
  const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");

  real nu = parameters.dbase.get<real >("nu");
  if( parameters.dbase.get<bool >("advectPassiveScalar") ) 
    nu=max(nu,parameters.dbase.get<real >("nuPassiveScalar"));   // could do better than this

  // only apply fourth-order AD here if it is explicit
  const bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
                                                !parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion");

  // define an alias:
  realMappedGridFunction & rx = mg.inverseVertexDerivative();
  // Get Index's for the interior+boundary points
  Index I1,I2,I3;
  getIndex( mg.extendedIndexRange(),I1,I2,I3);

  MappedGridSolverWorkSpace::resize(uu,I1,I2,I3,parameters.dbase.get<Range >("Ru")); // *** added 040223 ***

  //  realArray & xy = mg.center();
  MappedGridOperators & mappedGridOperators = *u0.getOperators();
  if( ad21 > 0. || ad22 > 0. || ad41 > 0. || ad42 > 0.  )
    mappedGridOperators.getDerivatives(u0,I1,I2,I3,parameters.dbase.get<Range >("Ru")); // compute u.x for artificial diffussion
  

  if( gridIsMoving )
  {
    uu(I1,I2,I3,uc)=advectionCoefficient*U(uc)-gridVelocity(I1,I2,I3,0);
    uu(I1,I2,I3,vc)=advectionCoefficient*U(vc)-gridVelocity(I1,I2,I3,1);
    if( mg.numberOfDimensions()==3 )
      uu(I1,I2,I3,wc)=advectionCoefficient*U(wc)-gridVelocity(I1,I2,I3,2);
  }
  else
  {
    uu(I1,I2,I3,uc)=advectionCoefficient*U(uc);
    uu(I1,I2,I3,vc)=advectionCoefficient*U(vc);
    if( mg.numberOfDimensions()==3 )
      uu(I1,I2,I3,wc)=advectionCoefficient*U(wc);
  }

  int axis;
  if( mg.numberOfDimensions()==2 )
  {
    realArray a1;
    // Grid spacings on unit square:
    real dr1 = mg.gridSpacing()(axis1);
    real dr2 = mg.gridSpacing()(axis2);
    if( mg.isRectangular() )
    {
      real dxv[3], &dx1=dxv[0], &dx2=dxv[1];
      mg.getDeltaX(dxv);

      // *** compute imLambda ***
      // **scLC
      if (parameters.timeStepType(grid)==1)
      {
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambda=max(abs(UU(uc))*(1./dx1));
	cout << "getTimeSteppingEigenvalueINS:Line-implicit rectangular grid imag part.\n";
      }
      else
      {//implicit and explicit 
	if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
	{
	  assert( pdtVar !=NULL );
	  realArray & dtVar = *pdtVar;
	  dtVar(I1,I2,I3)=abs(UU(uc))*(1./dx1)+abs(UU(vc))*(1./dx2);
	  where( mg.mask()(I1,I2,I3)>0 )
            imLambda=max(dtVar(I1,I2,I3));
	  // dtVar=1./(max(1./parameters.dbase.get<real >("dtMax"),dtVar));
	}
	else
	{
	  where( mg.mask()(I1,I2,I3)>0 )
            imLambda=max(abs(UU(uc))*(1./dx1)+abs(UU(vc))*(1./dx2));// This line was here before!

//        printf(" **** ins:get lambda: imLambda=%8.2e\n",imLambda);
//        display(UU(uc),"ins:get lambda:UU(uc) for imLambda","%8.2e ");
//        display(abs(UU(uc))*(1./dx1)+abs(UU(vc))*(1./dx2),"ins:get lambda:a1 for imLambda","%8.2e ");


	}
	  
	if( debug() & 2 )
	  cout << "getTimeSteppingEigenvalueINS:Explicit or implicit rectangular grid imag part.\n";
      }
      //**ecLC

      
      // save reLambda(I1,I2,I3) in a1, we need to add extra terms next.
      a1.redim(I1,I2);
//**scLC I changed from !parameters.getGridIsImplicit(grid).
      if( nu!=0. && parameters.getGridIsImplicit(grid)!=1 )
	//**ecLC
      {
//**scLC
	if (parameters.timeStepType(grid)==0)
	{
          if( debug() & 2 )
  	    cout << "getTimeSteppingEigenvalueINS:Explicit rectangular grid real part.\n";
	  //**ecLc 
	  a1= nu*(4./(dx1*dx1)) + nu*(4./(dx2*dx2));
	  if( parameters.isAxisymmetric() )
	  {
	    //   nu*(  u.xx + u.yy + (1/y) u.y )
	    //   nu*(  v.xx + v.yy + (1/y) v.y - v/y^2 ) 
	    realArray radiusInverse;
	    radiusInverse=1./max(REAL_MIN,mg.vertex()(I1,I2,I3,axis2));
	    realArray urOverR(I1,I2,I3);
        
	    urOverR=radiusInverse*( (nu/dx2) );
	    // fix points on axis
	    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
		if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
		{
		  Index Ib1,Ib2,Ib3;
		  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  // u.y/y -> u.yy on y=0
		  // .5*v.yy
		  urOverR(Ib1,Ib2,Ib3)=nu*(4./(dx2*dx2));
		}
	      }
	    }
	    a1+=urOverR;
	    
	  }
	  if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
	  {
	    assert( pdtVar !=NULL );
	    realArray & dtVar = *pdtVar;
            // ***** assumes stability region at 1 **************************************
	    if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::lineImplicit  )  // do not add viscous term if line implicit
	      dtVar(I1,I2,I3)= 1./(max(1./parameters.dbase.get<real >("dtMax"),dtVar(I1,I2,I3)));
            else 
	      dtVar(I1,I2,I3)= 1./SQRT( a1*a1 + dtVar(I1,I2,I3)*dtVar(I1,I2,I3) );  // ** assumes stability region at 1
	  }
	  //**scLC
	}//end parameters.getGridIsImplicit(grid)==0
	else // line-implicit case
	{
	  a1= nu*(4./(dx1*dx1));  
	  cout << "getTimeSteppingEigenvalueINS:Line-implicit rectangular grid real part.\n";
	}
	//**ecLC

      }
      else
      {
	// if( parameters.getGridIsImplicit(grid) )
	//  printf("+++getTimeSteppingEigenvalueINS: get eigenvalues for implicit +++\n");
	a1=0.;
      }
    }
    else
    {//curvii-linear case
      rx.setOperators(mappedGridOperators);
      
      realArray b1,nu11,nu12,nu22;
      // a1 = u*r.x + v*r.y - nu ( r1.xx + r1.yy )     b1 = u*s.x + v*s.y - nu ( r2.xx + r2.yy )
      a1   = UU(uc)*rx(I1,I2,I3,0,0)+UU(vc)*rx(I1,I2,I3,0,1);
      b1   = UU(uc)*rx(I1,I2,I3,1,0)+UU(vc)*rx(I1,I2,I3,1,1);

      // if( nu!=0. )
//**scLC I changed from !parameters.getGridIsImplicit(grid).
      if( nu!=0. && parameters.getGridIsImplicit(grid)!=1 ) // ??? is this correct for implicit??????
	//**ecLC
      {
	a1-=nu*( rx.x(I1,I2,I3,0,0)(I1,I2,I3,0,0)  + rx.y(I1,I2,I3,0,1)(I1,I2,I3,0,1) );
	b1-=nu*( rx.x(I1,I2,I3,1,0)(I1,I2,I3,1,0)  + rx.y(I1,I2,I3,1,1)(I1,I2,I3,1,1) );
      }
    
      // *** compute imLambda ***
//**scLC
	if (parameters.timeStepType(grid)==1)
	{
	  where( mg.mask()(I1,I2,I3)>0 )
            imLambda=max(abs(a1)*(1./dr1));
	  cout << "getTimeSteppingEigenvalueINS:Line-implicit curvii grid imag part.\n";
	}
	else
	{//implicit and explicit 
          if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
	  {
	    assert( pdtVar !=NULL );
	    realArray & dtVar = *pdtVar;
            dtVar(I1,I2,I3)=abs(a1)*(1./dr1)+abs(b1)*(1./dr2);
	    where( mg.mask()(I1,I2,I3)>0 )
              imLambda=max(dtVar(I1,I2,I3));
            // dtVar=1./(max(1./parameters.dbase.get<real >("dtMax"),dtVar));
	  }
	  else
	  {
	    where( mg.mask()(I1,I2,I3)>0 )
              imLambda=max(abs(a1)*(1./dr1)+abs(b1)*(1./dr2));// This line was here before!
	  }
	  
          if( debug() & 2 )
	    cout << "getTimeSteppingEigenvalueINS:Explicit or implicit curvii grid imag part.\n";
	}
//          printf(" **** ins:get lambda: imLambda=%8.2e\n",imLambda);
//          display(a1,"ins:get lambda:a1 for imLambda","%8.2e ");
//          display(b1,"ins:get lambda:b1 for imLambda","%8.2e ");
	//**ecLC
      
      // save reLambda(I1,I2,I3) in a1, we need to add extra terms next.
//**scLC I changed from !parameters.getGridIsImplicit(grid).
      if( nu!=0. && parameters.getGridIsImplicit(grid)!=1 )
	//**ecLC
      {
//**scLC
	if (parameters.timeStepType(grid)==0)
	{
          if( debug() & 2 )
	    cout << "getTimeSteppingEigenvalueINS:Explicit curvii grid real part.\n";
	  //**ecLc 
	  // nu11 = nu*( r1.x*r1.x + r1.y*r1.y )
	  // nu12 = nu*( r1.x*r2.x + r1.y*r2.y )*2 
	  // nu22 = nu*( r2.x*r2.x + r2.y*r2.y ) 
	  nu11 = nu*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,0,0) + rx(I1,I2,I3,0,1)*rx(I1,I2,I3,0,1) );
	  nu12 = nu*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,1,0) + rx(I1,I2,I3,0,1)*rx(I1,I2,I3,1,1) )*2.;
	  nu22 = nu*( rx(I1,I2,I3,1,0)*rx(I1,I2,I3,1,0) + rx(I1,I2,I3,1,1)*rx(I1,I2,I3,1,1) );
	  a1= nu11 *(4./(dr1*dr1)) +abs(nu12)*(1./(dr1*dr2)) +nu22 *(4./(dr2*dr2));
	  if( parameters.isAxisymmetric() )
	  {
	    //   nu*(  u.xx + u.yy + (1/y) u.y )
	    //   nu*(  v.xx + v.yy + (1/y) v.y - v/y^2 ) 
	    realArray radiusInverse;
	    radiusInverse=1./max(REAL_MIN,mg.vertex()(I1,I2,I3,axis2));
	    realArray urOverR(I1,I2,I3);
        
	    urOverR=nu*radiusInverse*( fabs(rx(I1,I2,I3,0,1))*(1./dr1) + fabs(rx(I1,I2,I3,1,1))*(1./dr2) ); // ().y/y
	    // fix points on axis
	    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
		if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
		{
		  Index Ib1,Ib2,Ib3;
		  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  // u.y/y -> u.yy on y=0
		  // .5*v.yy
		  urOverR(Ib1,Ib2,Ib3)=nu*( 
		    (rx(Ib1,Ib2,Ib3,0,1)*rx(Ib1,Ib2,Ib3,0,1))*(4./(dr1*dr1))+
		    fabs(rx(Ib1,Ib2,Ib3,0,1)*rx(Ib1,Ib2,Ib3,1,1))*(2.*(1./(dr1*dr2)))+
		    (rx(Ib1,Ib2,Ib3,1,1)*rx(Ib1,Ib2,Ib3,1,1))*(4./(dr2*dr2)) 
		    );
		}
	      }
	    }
	    a1+=urOverR;
            // display(a1,"ins:get dt: a1 after adding axisymmetric correction","%8.2e ");
	    
	  }//end isAxisymmetric

	  if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
          {
	    assert( pdtVar !=NULL );
	    realArray & dtVar = *pdtVar;
	    if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::lineImplicit  )  // do not add viscous term if line implicit
	      dtVar(I1,I2,I3)= 1./(max(1./parameters.dbase.get<real >("dtMax"),dtVar(I1,I2,I3)));
	    else
	      dtVar(I1,I2,I3)= 1./SQRT( a1*a1 + dtVar(I1,I2,I3)*dtVar(I1,I2,I3) );  // ** assumes stability region at 1
	  }

	  //**scLC
	}//end parameters.getGridIsImplicit(grid)==0
	else // line-implicit case
	{
	  nu11 = nu*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,0,0) + rx(I1,I2,I3,0,1)*rx(I1,I2,I3,0,1) );
	  nu12 = nu*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,1,0) + rx(I1,I2,I3,0,1)*rx(I1,I2,I3,1,1) )*2.;
	  a1= nu11 *(4./(dr1*dr1)) +abs(nu12)*(1./(dr1*dr2));
	  cout << "getTimeSteppingEigenvalueINS:Line-implicit curvii grid real part.\n";
	}
	//**ecLC
      }
      else
      {
	// if( parameters.getGridIsImplicit(grid) )
	//  printf("+++getTimeSteppingEigenvalueINS: get eigenvalues for implicit +++\n");
	a1=0.;
      }
      
    } // end curvilinear case
    
    if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
    { // --- add terms from 2nd order artificial diffusion

      //  ---artificial diffusion:  8 = (1+2+1)*2
      if( !parameters.getGridIsImplicit(grid) && 
          !( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta && 
             parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::lineImplicit) )
      {
        real cd22=ad22/SQR(mg.numberOfDimensions());
	a1+=(8.*ad21) + (8.*cd22)*( abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)) );
      }
      
    }
    if( useFourthOrderArtificialDiffusion && (ad41!=0. || ad42!=0.) )
    {
      //  ---artificial diffusion:  32=(1+4+6+4+1)*2  
      real cd42=ad42/SQR(mg.numberOfDimensions());
      a1+=(32.*ad41) + (32.*cd42)*(abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)));
    }

    if( cdv!=0. )
    { // correct for divergence damping term
      // this is an over estimate ****

      
      // const real scaleFactor = parameters.isAxisymmetric() ? 2. : 1.;

      real scaleFactor = parameters.dbase.get<real >("divDampingImplicitTimeStepReductionFactor"); // *wdh* 040228
      if( parameters.isAxisymmetric() )
        scaleFactor*=.25;

      if( !parameters.getGridIsImplicit(grid) )
      {
//	real cdvnu=cdv*max(nu,hMin)*4;   // same as in insp
//	real correction=1.; // .5; // .25         // adhoc fudge
//	a1+=(cdvnu*correction)*divergenceDampingWeight(I1,I2,I3);

	a1+=scaleFactor*divergenceDampingWeight()(I1,I2,I3);
      }
      else
      { // in the implicit case we limit the size of the divergence damping by cdt/dt
        real factor=1.5*scaleFactor;
        a1+=imLambda*parameters.dbase.get<real >("cDt")*factor;
        imLambda*=(1.+parameters.dbase.get<real >("cDt")*factor);
      }
    }

    // *** compute reLambda ***
    where( mg.mask()(I1,I2,I3)>0 )
      reLambda=max(a1);


  }
  else if( mg.numberOfDimensions()==3 )
  {
    // Grid spacings on unit square:
    real dr1 = mg.gridSpacing()(axis1);
    real dr2 = mg.gridSpacing()(axis2);
    real dr3 = mg.gridSpacing()(axis3);

    realArray a[3];
    if( mg.isRectangular() )
    {
      real dxv[3], &dx1=dxv[0], &dx2=dxv[1], &dx3=dxv[2];
      mg.getDeltaX(dxv);

      // *** compute imLambda ***
      if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
      {
	assert( pdtVar !=NULL );
	realArray & dtVar = *pdtVar;
	dtVar(I1,I2,I3)=abs(UU(uc))*(1./dx1)+abs(UU(vc))*(1./dx2)+abs(UU(wc))*(1./dx3);
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambda=max(dtVar(I1,I2,I3));
      }
      else
      {
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambda=max(abs(UU(uc))*(1./dx1)+abs(UU(vc))*(1./dx2)+abs(UU(wc))*(1./dx3));
      }
      
      // save reLambda(I1,I2,I3) in a[0], we need to add extra terms next.
      a[0].redim(I1,I2,I3);
      if( nu!=0. && !parameters.getGridIsImplicit(grid) )
      {
	a[0]=nu*(4./(dx1*dx1)) +nu*(4./(dx2*dx2))  +nu*(4./(dx3*dx3));
      }
      else
	a[0]=0.;

      if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
      {
	assert( pdtVar !=NULL );
	realArray & dtVar = *pdtVar;
	if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::lineImplicit  )  // do not add viscous term if line implicit
	  dtVar(I1,I2,I3)= 1./(max(1./parameters.dbase.get<real >("dtMax"),dtVar(I1,I2,I3)));
	else
	  dtVar(I1,I2,I3)= 1./SQRT( a[0]*a[0] + dtVar(I1,I2,I3)*dtVar(I1,I2,I3) );  // ** assumes stability region at 1
      }

    }
    else // curvilinear
    {
      rx.setOperators(mappedGridOperators);
      realArray nuA[3][3];
      // a[0] = u*r.x + v*r.y + w*r.z - nu ( r1.xx + r1.yy +r1.zz ) 
      for( axis=axis1; axis<mg.numberOfDimensions(); axis++ )
	a[axis]=UU(uc)*rx(I1,I2,I3,axis,0)
	  +UU(vc)*rx(I1,I2,I3,axis,1)
	  +UU(wc)*rx(I1,I2,I3,axis,2);
      if( nu!=0. && !parameters.getGridIsImplicit(grid) )
      {
	for( axis=axis1; axis<mg.numberOfDimensions(); axis++ )
	  a[axis]-=
	    nu*( rx.x(I1,I2,I3,axis,0)(I1,I2,I3,axis,0)  
		 +rx.y(I1,I2,I3,axis,1)(I1,I2,I3,axis,1)   
		 +rx.z(I1,I2,I3,axis,2)(I1,I2,I3,axis,2) );
      }
    

      // *** compute imLambda ***
      if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
      {
	assert( pdtVar !=NULL );
	realArray & dtVar = *pdtVar;
	dtVar(I1,I2,I3)=abs(a[0])*(1./dr1)+abs(a[1])*(1./dr2)+abs(a[2])*(1./dr3);
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambda=max(dtVar(I1,I2,I3));
      }
      else
      {
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambda=max(abs(a[0])*(1./dr1)+abs(a[1])*(1./dr2)+abs(a[2])*(1./dr3));
      }
      
    // save reLambda(I1,I2,I3) in a[0], we need to add extra terms next.
      if( nu!=0. && !parameters.getGridIsImplicit(grid) )
      {
	for( axis=axis1; axis<mg.numberOfDimensions(); axis++ )
	  a[axis]+=
	    nu*( rx.x(I1,I2,I3,axis,0)(I1,I2,I3,axis,0)  
		 +rx.y(I1,I2,I3,axis,1)(I1,I2,I3,axis,1)   
		 +rx.z(I1,I2,I3,axis,2)(I1,I2,I3,axis,2) );
	// nu11 = nu*( r1.x*r1.x + r1.y*r1.y )
	// nu12 = nu*( r1.x*r2.x + r1.y*r2.y )*2 
	// nu22 = nu*( r2.x*r2.x + r2.y*r2.y ) 
	for( axis=axis1; axis<mg.numberOfDimensions(); axis++ )
	{
	  for( int dir=axis1; dir<=axis; dir++ )  // only compute for dir<=axis
	  {
	    nuA[axis][dir] = nu*( rx(I1,I2,I3,axis,0)*rx(I1,I2,I3,dir,0) 
				  + rx(I1,I2,I3,axis,1)*rx(I1,I2,I3,dir,1) 
				  + rx(I1,I2,I3,axis,2)*rx(I1,I2,I3,dir,2) );
	  }
	}
      
	a[0]=nuA[0][0] *(4./(dr1*dr1)) 
	  +nuA[1][1] *(4./(dr2*dr2))
	  +nuA[2][2] *(4./(dr3*dr3))
	  +abs(nuA[1][0])*(2./(dr2*dr1))   // 2 from nuA[1][0]+nuA[0][1]
	  +abs(nuA[2][0])*(2./(dr3*dr1)) 
	  +abs(nuA[2][1])*(2./(dr3*dr2)) ;
      }
      else
	a[0]=0.;
      
    }
    
    if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
    { // --- add terms from 2nd order artificial diffusion

      real cd22=ad22/SQR(mg.numberOfDimensions());
      //  ---artificial diffusion:  12= (1+2+1)*3  
      if( !parameters.getGridIsImplicit(grid) && 
          !( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta && 
             parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::lineImplicit) )
      {
	a[0]+=(12.*ad21) + (12.*cd22)*(
	  abs(UX(uc))+abs(UX(vc))+abs(UX(wc))+abs(UY(uc))+abs(UY(vc))+abs(UY(wc))+abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc)));
      }
    }
    if( useFourthOrderArtificialDiffusion && (ad41!=0. || ad42!=0.) )
    {
      //  ---artificial diffusion:  48=(1+4+6+4+1)*3
      real cd42=ad42/SQR(mg.numberOfDimensions());
      a[0]+=(48.*ad41) + (48.*cd42)*(
                             abs(UX(uc))+abs(UX(vc))+abs(UX(wc))
			    +abs(UY(uc))+abs(UY(vc))+abs(UY(wc))
			    +abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc)) );
    }

    if( cdv!=0. )
    { // correct for divergence damping term
      // this is an over estimate ****
      if( !parameters.getGridIsImplicit(grid) )
      {
//	real cdvnu=cdv*max(nu,hMin)*4;   // same as in insp
//	real correction=1.; // .5; // .25         // adhoc fudge
//	a[0]+=(cdvnu*correction)*divergenceDampingWeight(I1,I2,I3);

	a[0]+=divergenceDampingWeight()(I1,I2,I3);
      }
      else
      { // in the implicit case we limit the size of the divergence damping by cdt/dt
        real factor=1.5;
        a[0]+=imLambda*parameters.dbase.get<real >("cDt")*factor;
        imLambda*=(1.+parameters.dbase.get<real >("cDt")*factor);
      }
    }
    // *** compute reLambda ***
    where( mg.mask()(I1,I2,I3)>0 )
      reLambda=max(a[0]);

    if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
    {
      assert( pdtVar !=NULL );
      realArray & dtVar = *pdtVar;
      if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::lineImplicit  )  // do not add viscous term if line implicit
	dtVar(I1,I2,I3)= 1./(max(1./parameters.dbase.get<real >("dtMax"),dtVar(I1,I2,I3)));
      else
	dtVar(I1,I2,I3)= 1./SQRT( a[0]*a[0] + dtVar(I1,I2,I3)*dtVar(I1,I2,I3) );  // ** assumes stability region at 1
    }
  }
  else
  {
    cout << "OB_MappedGridSolver::getTimeSteppingEigenvalue:ERROR: unknown dimension\n";
    throw "error";
  }
  if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
  {
    assert( pdtVar !=NULL );
    realArray & dtVar = *pdtVar;
    real dtVarMin,dtVarMax;
    where( mg.mask()(I1,I2,I3)>0 )
    {
      dtVarMin=min(dtVar(I1,I2,I3));
      dtVarMax=max(dtVar(I1,I2,I3));
    }
    printf(" >>>> grid=%i dtVar: [min,max]=[%8.2e,%8.2e]\n",grid,dtVarMin,dtVarMax);
    if( debug() & 16 )
    {
      ::display(dtVar,"dtVar",parameters.dbase.get<FILE* >("debugFile"),"%8.2e ");
    }
    
  }
 


  if( true || debug() & 4 )
    printf("getTimeSteppingEigenvalueINS: reLambda=%e, imLambda=%e \n",reLambda,imLambda);
}

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{solveForTimeIndependentVariablesINS}} 
void OB_CompositeGridSolver::
solveForTimeIndependentVariablesINS( GridFunction & cgf )
//=========================================================================================
// /Description:
//   Solve for the pressure given the velocity
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  real cpu0,cpu1;
  if( debug() & 8 )
    cpu0 = getCPU();
  
  if( debug() & 8 )
    printf("solveForTimeIndependentVariablesINS...\n");
  // real time=getCPU();  // keep track of the cpu time spent in this routine

  checkArrays(" solveForTimeIndependentVariablesINS: start");

  real & t = cgf.t;
  realCompositeGridFunction & u = cgf.u;
  
  const int & pc = parameters.dbase.get<int >("pc");

  CompositeGrid & cg = cgf.cg;
  Index I1,I2,I3;
  
  // realCompositeGridFunction pressure;
  p().link(u,Range(pc,pc));   // can we avoid this link?? (although is doesn't take much time).

  if( debug() & 32 )
    printf("solveForTimeIndependentVariablesINS: time to link = %e \n",getCPU()-cpu0);

  if( debug() & 32 )
  {
    u.display("Before assignPressureRHS: u",parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
  }

  cpu1=getCPU();
  assignPressureRHS( cgf,pressureRightHandSide );
  parameters.timing(Parameters::timeForAssignPressureRHS)+=getCPU()-cpu1;

  if( debug() & 8 )
    printf("after assignPressureRHS: total time = %e \n",getCPU()-cpu0);
  if( debug() & 8 )
  {
    aString buff;
    u.display(sPrintF(buff,"Before pressure solve: u at t=%8.2e",t),parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");

//     pressureRightHandSide.display(sPrintF(buff,"Before solve: pressure rhs at t=%8.2e",t),
//              parameters.dbase.get<FILE* >("debugFile"),"%8.5f "); // "%8.1e ");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
//       ::display(pressureRightHandSide[grid],sPrintF("Before solve: pressure rhs at t=%8.2e, grid=%i",t,grid),
//                 parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
      ::display(pressureRightHandSide[grid],sPrintF("Before solve: pressure rhs at t=%8.2e, grid=%i",t,grid),
                parameters.dbase.get<FILE* >("debugFile"),"%8.1e ");
    }
    
  }

  cpu1=getCPU();
  // printf("solve for the pressure...\n");

  checkArrays(" solveForTimeIndependentVariablesINS: before poisson->solve");

  // **************************************************************
  // *****************Pressure Solve*******************************
  // **************************************************************
  bool done=false;
  while( !done )
  {
    if( Parameters::checkForFloatingPointErrors )
      checkSolution(cgf.u,"Before pressure solve");

    poisson->solve( p(),pressureRightHandSide );

    if( debug() & 8 )
      p().display("After pressure solve p()",parameters.dbase.get<FILE* >("debugFile"),"%6.2f "); // "%8.1e ");
 
    // if( parameters.dbase.get<int >("myid")==0 ) printf("****After pressure solve *****\n");
    

    if( (globalStepNumber % 2)==0 && poisson->isSolverIterative() )
    { // This is needed for MG 
      RealArray value(1);
      value=1.;
      const int numberOfGhostLines=2;
      p().fixupUnusedPoints(value,numberOfGhostLines);
    }

    // if( parameters.dbase.get<int >("myid")==0 ) printf("****After pressure solve and fixupUnusedPoints *****\n");

    if( Parameters::checkForFloatingPointErrors )
      checkSolution(cgf.u,"After pressure solve and fixup");
  
    done=true;

    checkArrays(" solveForTimeIndependentVariablesINS: after poisson->solve");

    parameters.dbase.get<int >("numberOfIterationsForConstraints")+=poisson->getNumberOfIterations();
//     if( parameters.dbase.get<int >("myid")==0 ) 
//         printf(" ** iter's to solve p eqn = %i\n",poisson->getNumberOfIterations());
    
    parameters.dbase.get<int >("numberOfSolvesForConstraints")++;
    if( poisson->isSolverIterative() )
    {
      // if( parameters.dbase.get<int >("myid")==0 ) printf(" ** after pressure solve 1a\n"); 

      real absoluteTolerance,relativeTolerance;
      poisson->get(OgesParameters::THEabsoluteTolerance,absoluteTolerance);
      poisson->get(OgesParameters::THErelativeTolerance,relativeTolerance);
      real maxResidual=poisson->getMaximumResidual();
    
      if( parameters.dbase.get<int >("myid")==0 ) 
	printf(" ** iter's to solve p eqn = %i (t=%9.3e, dt=%8.1e, step=%i, max res=%8.2e "
	       "rel-tol=%7.1e, abs-tol=%7.1e)\n",
	       poisson->getNumberOfIterations(),t,dt,globalStepNumber,maxResidual,relativeTolerance,absoluteTolerance);

      if( parameters.dbase.get<int >("enforceAbsoluteToleranceForIterativeSolvers") )
      {
	if( maxResidual>absoluteTolerance*1.1 )
	{
	  // resolve the poisson equation with a smaller tolerance
          done=false;
      
	  relativeTolerance *= min(.9,absoluteTolerance/maxResidual);
	  poisson->set(OgesParameters::THErelativeTolerance,relativeTolerance);
	  printf(" ...absolute-tol not met, resolve with a new rel-tol of %8.2e\n",relativeTolerance);
	  
	}
	else if( maxResidual<absoluteTolerance*10. )
	{ // slowly increase the relative tolerance if we have are solving too accurately
	  relativeTolerance *= min(1.1,absoluteTolerance/maxResidual); // increase slowly
	  poisson->set(OgesParameters::THErelativeTolerance,relativeTolerance);
	  printf(" ...absolute-tol met too well, increase rel-tol to %8.2e\n",relativeTolerance);
   
	}
      }

      // printf(" poisson->parameters.getSolverTypeName()=[%s]\n",(const char*)poisson->parameters.getSolverTypeName());
      
      if( poisson->getNumberOfIterations()>45 && poisson->parameters.getSolverTypeName()=="multigrid" )
      {
        printf(" *****ERROR solving the pressure equation -- I am going to output the grid for debugging ****\n");
        outputCompositeGrid(cgf.cg,"multigridBug.hdf"); 
        Overture::abort("error");
      }
      


    }
  }
  // if( parameters.dbase.get<int >("myid")==0 ) printf(" ** after pressure solve 2a\n"); 
    
  if( Parameters::checkForFloatingPointErrors || debug() & 4 )
  {
    #ifndef USE_PPP
    // max(p()) hung with P++
    if( parameters.dbase.get<int >("myid")==0 )
      printf(" -->> After pressure solve: max(p)=%9.2e min(p)=%9.2e \n",max(p()),min(p()));
    #endif
  }

//     parameters.dbase.get<GenericGraphicsInterface* >("ps")->erase();
//     parameters.dbase.get<GraphicsParameters >("psp").set(GI_TOP_LABEL,"solve for p");
//     parameters.dbase.get<GenericGraphicsInterface* >("ps")->contour(p,parameters.dbase.get<GraphicsParameters >("psp"));


  // if( parameters.dbase.get<int >("myid")==0 ) printf(" ** after pressure solve 2b\n"); 

  if( debug() & 4 )
  {
    if( parameters.dbase.get<int >("myid")==0 )
      fprintf(parameters.dbase.get<FILE* >("debugFile")," After pressure solve: compatibilityConstraint=%i, numberOfExtraEquations = %i\n",
	    poisson->getCompatibilityConstraint(),poisson->numberOfExtraEquations);
    if( poisson->getCompatibilityConstraint() )
    {
      int ne,i1e,i2e,i3e,gride;
      poisson->equationToIndex( poisson->extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
      if( parameters.dbase.get<int >("myid")==0 )
        fprintf(parameters.dbase.get<FILE* >("debugFile")," After pressure solve: value of constraint = %e\n",p()[gride](i1e,i2e,i3e));
    }
  }

  // if( parameters.dbase.get<int >("myid")==0 ) printf(" ** after pressure solve 3\n"); 

  if( poisson->getCompatibilityConstraint() )
  {
    // The solver may have trouble satisfying the compatability constraint (true for yale and ins/annulus.tz)
    // so we explicitly enforce it here by just shifting the solution by a constant 
    // *** note that we over-write the value of the constraint, p[gride](i1e,i2e,i3e)
    real nullVectorDotP=0., sumOfNullVector=0.;
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      nullVectorDotP+=sum(poisson->rightNullVector[grid](I1,I2,I3)*p()[grid](I1,I2,I3));
      sumOfNullVector+=sum(poisson->rightNullVector[grid](I1,I2,I3));
    }
    int ne,i1e,i2e,i3e,gride;
    poisson->equationToIndex( poisson->extraEquationNumber(0),ne,i1e,i2e,i3e,gride);

    if( debug() & 2 )
    {
      real diff=nullVectorDotP-pressureRightHandSide[gride](i1e,i2e,i3e);
      if( parameters.dbase.get<int >("myid")==0 )
	fprintf(parameters.dbase.get<FILE* >("debugFile"),"After solve: compatibility sum(null*p)= %14.10e,  nullVectorDotP-rhs=%e \n",
	      nullVectorDotP,diff);
    }
    
    p()+=(pressureRightHandSide[gride](i1e,i2e,i3e)-nullVectorDotP)/(max(1.,sumOfNullVector));

  }
  if( debug() & 8 )
    p().display("After solve: here is the pressure",parameters.dbase.get<FILE* >("debugFile"),"%10.7f "); // "%8.1e ");
  if( debug() & 4 )
  {
    determineErrors( cgf,sPrintF(" After pressure solve: errors at t=%e \n",t) ) ;

  }

  parameters.timing(Parameters::timeForPressureSolve)+=getCPU()-cpu1;

  if( debug() & 8 && parameters.dbase.get<int >("myid")==0 )
    printf("after solve: total time = %e \n",getCPU()-cpu0);


  if( debug() & 4 )
  {
    real pMax= max(fabs(p())); 
    real rhsMax= max(fabs(pressureRightHandSide));
    if( parameters.dbase.get<int >("myid")==0 )
      fprintf(parameters.dbase.get<FILE* >("debugFile"),"******** max(|pressure|)=%e, max(|pressureRightHandSide|) = %e \n",pMax,rhsMax); 
    // p.display("pressure",parameters.dbase.get<FILE* >("debugFile"));
    // pressureRightHandSide.display("pressure rhs",parameters.dbase.get<FILE* >("debugFile"));
  }
  
  if( debug() & 64 )
  {
    fPrintF(parameters.dbase.get<FILE* >("debugFile"),"\n\n\n ==================================================== \n");
// ******
    real mean = sum(p()[0]);
    printF(" After pressure solve sum(p) = %e \n",mean);
// *****
  }


  
  if( debug() & 8 )
    printF("solveForTimeIndependentVariablesINS: total time = %e \n",getCPU()-cpu0);

}


//\begin{>>MappedGridSolverInclude.tex}{\subsection{formImplicitTimeSteppingMatrixINS}} 
int OB_MappedGridSolver::
formImplicitTimeSteppingMatrixINS(realMappedGridFunction & coeff,
                                  const real & dt0, 
                                  int scalarSystem,
                                  realMappedGridFunction & u0,
                                  const int & grid )
// ==========================================================================================
// /Description:
//    Form the implicit time steping matrix for the INS equations.
//
// If the form of the boundary conditions for the different components of $\uv$ are the same
// then we can build a single scalar matrix that can be used to advance each component, one after
// the other. If the boundary conditions are not of the same form then we build a matrix for
// a system of equations for the velocity components $(u,v,w)$.
//
// /coeff (input/output) : fill-in this coefficient matrix.
// /dt0 (input) : time step used to build the implicit matrix.
// /scalarSystem (input) : If true then the same matrix is used to solve for all components (e.g. all velocity
//   components could have the same form of the matrix).
// /u0 (input) : current best approximation to the solution. Used for linearization.
// 
//\end{MappedGridSolverInclude.tex}  
// ==========================================================================================
{
  realArray & ux = get(WorkSpace::ux);
  realArray & uy = get(WorkSpace::uy);
  realArray & uz = get(WorkSpace::uz);

  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");

  MappedGrid & mg = *coeff.getMappedGrid();
  const int numberOfDimensions = mg.numberOfDimensions();
  // const int numberOfComponents = scalarSystem ? 1 : numberOfDimensions;
  
  // Form the (scalar) matrix I - implicitFactor*nu*dt* Laplacian
  Index I1,I2,I3;
  Range e0(0,0), e1(1,1), e2(2,2);  // e0 = first equation, e1=second equation
  Range c0(0,0), c1(1,1), c2(2,2);  // c0 = first component, c1 = second component
  Range Rx(0,numberOfDimensions-1);
  int n;
    
  real nuDt = parameters.dbase.get<real >("implicitFactor")*parameters.dbase.get<real >("nu")*dt0;
  const real ad21 = parameters.dbase.get<real >("ad21");
  const real ad22 = parameters.dbase.get<real >("ad22");
  const RealArray & gridSpacing = mg.gridSpacing();
  MappedGridOperators & op = *u0.getOperators();
    
  getIndex(mg.gridIndexRange(),I1,I2,I3);

  realArray ad; 
  if( parameters.getGridIsImplicit(grid) )
  {
    if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
    {
      // here we compute ad : the coefficient of the aritificial diffusion.
	    
      op.getDerivatives(u0,I1,I2,I3,parameters.dbase.get<Range >("Ru")); // compute u.x for artificial diffussion
      real cd22=ad22/SQR(numberOfDimensions);
      const real implicitFactor=1.5*dt0;      // implicit diffusion is bigger than explicit for safety ******
	
      ad.redim(u0.dimension(0),u0.dimension(1),u0.dimension(2));
      ad=0.;
      if( numberOfDimensions==2 )
      {
	ad(I1,I2,I3)=(-implicitFactor*ad21)+(-implicitFactor*cd22)*(abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)));
      }
      else if( numberOfDimensions==3 ) 
      {
	ad(I1,I2,I3)=(-implicitFactor*ad21)+
	  (-implicitFactor*cd22)*(abs(UX(uc))+abs(UX(vc))+abs(UX(wc))+abs(UY(uc))+abs(UY(vc))+
				  abs(UY(wc))+abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc)));
      }
      // printf(" ------->> max(fabs(ad))=%e \n",max(fabs(ad(I1,I2,I3)/dt0)));
    }
  }
    
  #ifndef USE_PPP
    bool useOpt=false;
  #else
    bool useOpt=!scalarSystem;
  #endif
  if( useOpt )
  {
    if( scalarSystem )
    {
      Overture::abort("ERROR: finish this");
    }
    else
    {
#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilSize*(n))

// Use this for indexing into coefficient matrices representing systems of equations
#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))

// M123 with a fixed offset
#define MCE(m1,m2,m3) (M123(m1,m2,m3)+CE(c0,e0))
      
      const int numberOfComponentsForCoefficients=numberOfDimensions;
      const int stencilSize=coeff.sparse->stencilSize;
      const int width = parameters.dbase.get<int >("orderOfAccuracy")+1; 
      const int halfWidth1 = (width-1)/2;
      const int halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
      const int halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;
      
      coeff=0.;

      if( parameters.getGridIsImplicit(grid) )
      {
	if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
	{
	  Overture::abort("ERROR: finish this");
	}
      
	for( int m=0; m<numberOfComponentsForCoefficients; m++ )
	{
	  Range e(m,m), c(m,m);
	  op.coefficients(MappedGridOperators::laplacianOperator,coeff,I1,I2,I3,e,c);
	}
	coeff*=-nuDt; // form  (-nu*dt)*Delta
	for( int m=0; m<numberOfComponentsForCoefficients; m++ )
	{
	  int md=M123CE(0,0,0,m,m); // diagonal term 
	  coeff(md,I1,I2,I3)+=1.; 
	}
      }
      else
      {
	for( int m=0; m<numberOfComponentsForCoefficients; m++ )
	{
	  Range e(m,m), c(m,m);
	  op.coefficients(MappedGridOperators::identityOperator,coeff,I1,I2,I3,e,c);
	}
     
      }
      
    }

#undef M123
#undef M123N
#undef CE
#undef M123CE
#undef MCE

  }
  else
  {
    // **old way**
    if( scalarSystem )
    {
      if( parameters.getGridIsImplicit(grid) )
      {
	if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
	{
	  if( debug() & 4 && parameters.dbase.get<int >("myid")==0 ) printf("***Add artificial diffusion to the implicit matrix\n");
	  // add artificial viscosity to matrix   ***** need an undivided difference laplacian operator *****
	  if( numberOfDimensions==2 )
	    coeff=SQR(gridSpacing(axis1))*op.r1r1Coefficients()+SQR(gridSpacing(axis2))*op.r2r2Coefficients();
	  else
	    coeff=(SQR(gridSpacing(axis1))*op.r1r1Coefficients()+
		   SQR(gridSpacing(axis2))*op.r2r2Coefficients()+
		   SQR(gridSpacing(axis3))*op.r3r3Coefficients());
	  
	  coeff=multiply(ad,coeff);
	  coeff+=op.identityCoefficients()-nuDt*op.laplacianCoefficients(); 
	}
	else
	  coeff=op.identityCoefficients()-nuDt*op.laplacianCoefficients(); 

      }
      else
	coeff=op.identityCoefficients();
    }
    else
    {
      if( parameters.getGridIsImplicit(grid) )
      {
	if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.))
	{
	  if( parameters.dbase.get<int >("myid")==0 )
	    printf("***Add artificial diffusion to the implicit matrix\n");
	  // add artificial viscosity to matrix   ***** need an undivided difference laplacian operator *****
	  if( numberOfDimensions==2 )
	    coeff=(SQR(gridSpacing(axis1))*(op.r1r1Coefficients(I1,I2,I3,e0,c0)+op.r1r1Coefficients(I1,I2,I3,e1,c1))+
		   SQR(gridSpacing(axis2))*(op.r2r2Coefficients(I1,I2,I3,e0,c0)+op.r2r2Coefficients(I1,I2,I3,e1,c1)));
	  else
	    coeff=(SQR(gridSpacing(axis1))*(op.r1r1Coefficients(I1,I2,I3,e0,c0)+op.r1r1Coefficients(I1,I2,I3,e1,c1))+
		   SQR(gridSpacing(axis2))*(op.r2r2Coefficients(I1,I2,I3,e0,c0)+op.r2r2Coefficients(I1,I2,I3,e1,c1))+
		   SQR(gridSpacing(axis3))*(op.r3r3Coefficients(I1,I2,I3,e0,c0)+op.r3r3Coefficients(I1,I2,I3,e1,c1)));
	  
	  coeff=multiply(ad,coeff);

	  coeff+=(op.identityCoefficients(I1,I2,I3,e0,c0)-nuDt*op.laplacianCoefficients(I1,I2,I3,e0,c0)+
		  op.identityCoefficients(I1,I2,I3,e1,c1)-nuDt*op.laplacianCoefficients(I1,I2,I3,e1,c1));
	}
	else
	{
	  coeff=(op.identityCoefficients(I1,I2,I3,e0,c0)-nuDt*op.laplacianCoefficients(I1,I2,I3,e0,c0)+
		 op.identityCoefficients(I1,I2,I3,e1,c1)-nuDt*op.laplacianCoefficients(I1,I2,I3,e1,c1));
	}
	if( numberOfDimensions==3 )
	  coeff+=op.identityCoefficients(I1,I2,I3,e2,c2)-nuDt*op.laplacianCoefficients(I1,I2,I3,e2,c2);



	if( numberOfDimensions==2 && parameters.isAxisymmetric() )
	{
	  // add on corrections for a axisymmetric problem
	  //  nu*( u.xx + u.yy + (1/y) u.y )
	  //  nu*( v.xx + v.yy + (1/y) v.y - v/r^2 )
	  realArray radiusInverse;
	  radiusInverse=1./max(REAL_MIN,mg.vertex()(nullRange,nullRange,nullRange,axis2));

	  int side,axis;
	  ForBoundary(side,axis)
	  {
	    if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
	    {
	      Index Ib1,Ib2,Ib3;
	      getBoundaryIndex( mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); 
	      coeff-=nuDt*(op.yyCoefficients(Ib1,Ib2,Ib3,e0,c0)+.5*op.yyCoefficients(Ib1,Ib2,Ib3,e1,c1));
	      radiusInverse(Ib1,Ib2,Ib3)=0.;  // this will remove terms on the axis below
	    }
	  }
	  // add axisymmetric terms
	  realMappedGridFunction aCoeff; // *** fix this ****
	  aCoeff.updateToMatchGridFunction(coeff);
	  aCoeff=op.yCoefficients(I1,I2,I3,e0,c0)+op.yCoefficients(I1,I2,I3,e1,c1)-
	    multiply(radiusInverse,op.identityCoefficients(I1,I2,I3,e1,c1));
	  coeff-=nuDt*multiply( radiusInverse,aCoeff );
	}
      }
      else
      {
	coeff=op.identityCoefficients(I1,I2,I3,e0,c0)+op.identityCoefficients(I1,I2,I3,e1,c1);
	if( numberOfDimensions==3 )
	  coeff+=op.identityCoefficients(I1,I2,I3,e2,c2);
      }
    }
  }
  
  // fill in the coefficients for the boundary conditions
  Parameters::BoundaryCondition noSlipWall                = Parameters::noSlipWall;
  Parameters::BoundaryCondition inflowWithVelocityGiven   = Parameters::inflowWithVelocityGiven;
  Parameters::BoundaryCondition slipWall                  = Parameters::slipWall;
  Parameters::BoundaryCondition dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
  Parameters::BoundaryCondition interfaceBoundaryCondition= Parameters::interfaceBoundaryCondition;
    
  if( parameters.getGridIsImplicit(grid) )
  {
    if( scalarSystem )
    {
      coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::dirichlet,noSlipWall);
      coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::dirichlet,inflowWithVelocityGiven);
      coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::dirichlet,dirichletBoundaryCondition);
      coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::extrapolate,BCTypes::allBoundaries);
      coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::dirichlet,interfaceBoundaryCondition);
    }
    else
    {
      for( n=0; n<numberOfDimensions; n++ )
      {
	coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,noSlipWall);
	coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,inflowWithVelocityGiven);
	coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,dirichletBoundaryCondition);
	coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::extrapolate,BCTypes::allBoundaries);
	coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,interfaceBoundaryCondition);
      }
      if( numberOfDimensions==2 && parameters.isAxisymmetric() )
      {
        // u.y = v = v.yy = 0
	coeff.applyBoundaryConditionCoefficients(e0,c0,BCTypes::neumann,Parameters::axisymmetric);
	coeff.applyBoundaryConditionCoefficients(e1,c1,BCTypes::dirichlet,Parameters::axisymmetric);

	BoundaryConditionParameters extrapParams;
	extrapParams.lineToAssign=1;
	extrapParams.orderOfExtrapolation=2;
	coeff.applyBoundaryConditionCoefficients(e1,c1,BCTypes::extrapolate,Parameters::axisymmetric);
      }
      
      // slip wall
      // *** for now we only handle the case when the normal on a slip side is...
/* ---------
   for( n=0; n<numberOfDimensions; n++ )
   coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::extrapolate,slipWall);
   n=numberOfDimensions-1;
   // put this last to try and get around a null pivot in the matrix
   -------------- */

//   **** *wdh* 040228 
//        // This works for a "horizontal" slip wall:
//        coeff.applyBoundaryConditionCoefficients(e0,Rx,BCTypes::normalDerivativeOfTangentialComponent0,
//  					       slipWall);  // ~ u.x
//        coeff.applyBoundaryConditionCoefficients(e1,Rx,BCTypes::normalComponent,slipWall);  // ~ v=
//        coeff.applyBoundaryConditionCoefficients(e1,Rx,BCTypes::extrapolateNormalComponent,slipWall);
//        if( numberOfDimensions==3 )
//  	coeff.applyBoundaryConditionCoefficients(e2,Rx,BCTypes::normalDerivativeOfTangentialComponent1,
//  						 Parameters::slipWall);

      for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) // *wdh* added 040228 
      {
	for( int side=0; side<=1; side++ )
	{
	  if( mg.boundaryCondition(side,axis)==slipWall )
	  {
            if( axis==0 )
	    {
	      // This works for a "vertical" slip wall:
              // We need to order the BC's to avoid a null pivot --
              //    Put an equation for u first, then v 
	      coeff.applyBoundaryConditionCoefficients(e0,Rx,BCTypes::normalComponent,
                                                       BCTypes::boundary1+side+2*axis);  // ~ u=
	      coeff.applyBoundaryConditionCoefficients(e0,Rx,BCTypes::extrapolateNormalComponent,
                                                       BCTypes::boundary1+side+2*axis);   // Extrap "u"
	      coeff.applyBoundaryConditionCoefficients(e1,Rx,BCTypes::normalDerivativeOfTangentialComponent0,
						       BCTypes::boundary1+side+2*axis);  // ~ v.x
	      if( numberOfDimensions==3 )
		coeff.applyBoundaryConditionCoefficients(e2,Rx,BCTypes::normalDerivativeOfTangentialComponent1,
							 BCTypes::boundary1+side+2*axis);
	    }
	    else if( axis==1 )
	    {
	      // This works for a "horizontal" slip wall:
              // We need to order the BC's to avoid a null pivot --
              //    Put an equation for u first, then v 
	      coeff.applyBoundaryConditionCoefficients(e0,Rx,BCTypes::normalDerivativeOfTangentialComponent0,
						       BCTypes::boundary1+side+2*axis);  // ~ u.y
	      coeff.applyBoundaryConditionCoefficients(e1,Rx,BCTypes::normalComponent,
                                                       BCTypes::boundary1+side+2*axis);  // ~ v=
	      coeff.applyBoundaryConditionCoefficients(e1,Rx,BCTypes::extrapolateNormalComponent,
                                                       BCTypes::boundary1+side+2*axis); // Extrap "v"
	      if( numberOfDimensions==3 )
		coeff.applyBoundaryConditionCoefficients(e2,Rx,BCTypes::normalDerivativeOfTangentialComponent1,
							 BCTypes::boundary1+side+2*axis);
	    }
	    else
	    {
	      coeff.applyBoundaryConditionCoefficients(e0,Rx,BCTypes::normalDerivativeOfTangentialComponent0,
						       BCTypes::boundary1+side+2*axis);  // ~ u.z
	      coeff.applyBoundaryConditionCoefficients(e1,Rx,BCTypes::normalDerivativeOfTangentialComponent1,
						       BCTypes::boundary1+side+2*axis);  // ~ v.z
              coeff.applyBoundaryConditionCoefficients(e2,Rx,BCTypes::normalComponent,
                                                       BCTypes::boundary1+side+2*axis);  //  "w=" 
	      coeff.applyBoundaryConditionCoefficients(e2,Rx,BCTypes::extrapolateNormalComponent,
                                                       BCTypes::boundary1+side+2*axis); // Extrap "w"

	    }
	  }
	}
      }
      
    }
  }
  else
  {
    if( scalarSystem )
      coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,BCTypes::allBoundaries);
    else
    {
      for( n=0; n<numberOfDimensions; n++ )
      {
	coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::dirichlet,  BCTypes::allBoundaries);
	coeff.applyBoundaryConditionCoefficients(n,n,BCTypes::extrapolate,BCTypes::allBoundaries);
      }
    }
  }
  // *** don't do here?  coeff.finishBoundaryConditions();

  return 0;
}





//\begin{>>MappedGridSolverInclude.tex}{\subsection{applyBoundaryConditionsForImplicitTimeSteppingINS}} 
int OB_MappedGridSolver::
applyBoundaryConditionsForImplicitTimeSteppingINS(realMappedGridFunction & u, 
                                                  realMappedGridFunction & gridVelocity,
                                                  real t,
                                                  int scalarSystem,
                                                  int grid )
// ======================================================================================
//  /Description:
//      Apply boundary conditions to the rhs side grid function used in the implicit solve.
// /u (input/output) : apply boundary conditions to this grid function.
// /gridVelocity (input) : for BC's on moving grids.
// /t (input) : time
// /scalarSystem (input) : 
// /grid (input) : component grid number.
//
//\end{MappedGridSolverInclude.tex}  
// ==========================================================================================
{
  // const real & t = cgf.t;
  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables

  typedef Parameters::BoundaryCondition BoundaryCondition;
  
  const BoundaryCondition & noSlipWall = Parameters::noSlipWall;
  const BoundaryCondition & slipWall   = Parameters::slipWall;
  const BoundaryCondition & inflowWithVelocityGiven = Parameters::inflowWithVelocityGiven;
  const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
               = Parameters::inflowWithPressureAndTangentialVelocityGiven;
  const BoundaryCondition & outflow = Parameters::outflow;
  const BoundaryCondition & symmetry = Parameters::symmetry;
  const BoundaryCondition & dirichletBoundaryCondition = Parameters::dirichletBoundaryCondition;
  const Parameters::BoundaryCondition & interfaceBoundaryCondition= Parameters::interfaceBoundaryCondition;
  
  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
  //  neumann               = BCTypes::neumann,
  //   extrapolate           = BCTypes::extrapolate,
  //   aDotU                 = BCTypes::aDotU,
  //   generalizedDivergence = BCTypes::generalizedDivergence,
  //   tangentialComponent   = BCTypes::tangentialComponent,
  //   vectorSymmetry        = BCTypes::vectorSymmetry,
  //   allBoundaries         = BCTypes::allBoundaries,
    normalComponent       = BCTypes::normalComponent;

  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");

  // *** assign boundary conditions for the implicit method ***** 

  if( parameters.getGridIsImplicit(grid) )
  {
    // ** Note that we are assigning the RHS for the implicit solve ***
    if( parameters.gridIsMoving(grid) )
    {
      u.applyBoundaryCondition(N,dirichlet,noSlipWall,gridVelocity,t);
      u.applyBoundaryCondition(N,dirichlet,dirichletBoundaryCondition,gridVelocity,t);
      u.applyBoundaryCondition(N,dirichlet,interfaceBoundaryCondition,gridVelocity,t);
    }
    else
    {
      u.applyBoundaryCondition(N,dirichlet,noSlipWall,bcData,t,Overture::defaultBoundaryConditionParameters(),grid);
      u.applyBoundaryCondition(N,dirichlet,dirichletBoundaryCondition,bcData,t,
				Overture::defaultBoundaryConditionParameters(),grid);
      u.applyBoundaryCondition(N,dirichlet,interfaceBoundaryCondition,bcData,t,
                               Overture::defaultBoundaryConditionParameters(),grid);
    }
    

    u.applyBoundaryCondition(N,dirichlet,inflowWithVelocityGiven,
			      parameters.dbase.get<RealArray >("bcData"),t,Overture::defaultBoundaryConditionParameters(),grid);

    if( parameters.isAxisymmetric() )
      u.applyBoundaryCondition(vc,dirichlet,Parameters::axisymmetric,0,t);

    // slip wall:   n.u=
    //              (t.u).n = 0
    Range V(uc,uc+parameters.dbase.get<int >("numberOfDimensions")-1);
//      u.applyBoundaryCondition(V,normalComponent,slipWall             ,0.,t);
//      u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0, slipWall,0.,t);
//      if( parameters.dbase.get<int >("numberOfDimensions")==3 )
//        u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1, slipWall,0.,t);

    MappedGrid & mg = *u.getMappedGrid();
    const bool isRectangular=mg.isRectangular();
    #ifndef USE_PPP
      if( !isRectangular )
        mg.update(MappedGrid::THEcenterBoundaryTangent | MappedGrid::THEcenterBoundaryNormal); 
    #endif

    #ifdef USE_PPP
      const realSerialArray & uLocal = u.getLocalArray();
    #else
      realSerialArray & uLocal = u; 
    #endif

    const bool rectangular= mg.isRectangular() && !parameters.dbase.get<bool >("twilightZoneFlow");
      
    realArray & x= mg.center();
    #ifdef USE_PPP
      realSerialArray xLocal; 
      if( !rectangular ) 
        getLocalArrayWithGhostBoundaries(x,xLocal);
    #else
      const realSerialArray & xLocal = x;
    #endif
    
    int side,axis;
    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
    for( axis=0; axis<mg.numberOfDimensions(); axis++ ) 
    {
      for( side=0; side<=1; side++ )
      {
	if( false &&   // set this to true for debugging
            mg.boundaryCondition(side,axis)==noSlipWall && parameters.gridIsMoving(grid) )
	{
          getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
          const intArray & mask = mg.mask();
          real uMin=0., uMax=0., vMin=0., vMax=0., guMin=0., guMax=0., gvMin=0., gvMax=0., err=0.;
          assert( gridVelocity.getBase(3)==0 && gridVelocity.getBound(3)==mg.numberOfDimensions()-1 );
	  where( mask(Ib1,Ib2,Ib3)>0 )
	  {
	    uMin = min(u(Ib1,Ib2,Ib3,uc));
	    uMax = max(u(Ib1,Ib2,Ib3,uc));
	    vMin = min(u(Ib1,Ib2,Ib3,vc));
	    vMax = max(u(Ib1,Ib2,Ib3,vc));
            guMin=min(gridVelocity(Ib1,Ib2,Ib3,0));
	    guMax=max(gridVelocity(Ib1,Ib2,Ib3,0));
            gvMin=min(gridVelocity(Ib1,Ib2,Ib3,1));
	    gvMax=max(gridVelocity(Ib1,Ib2,Ib3,1));
            err = max(fabs(u(Ib1,Ib2,Ib3,vc)-gridVelocity(Ib1,Ib2,Ib3,1)));
	  }
	  if( parameters.dbase.get<int >("myid")==0 )
	  {
	    printf("implicitBC: t=%9.3e: (grid,side,axis)=(%i,%i,%i) (uMin,uMax)=(%9.2e,%9.2e) (vMin,vMax)=(%9.2e,%9.2e)"
		   "\n   (guMin,guMax)=(%9.2e,%9.2e) (gvMin,gvMax)=(%9.2e,%9.2e) err=%8.2e <<<< \n",
		   t,grid,side,axis,uMin,uMax,vMin,vMax,guMin,guMax,gvMin,gvMax,err);
	  }
	  
	}
	else if( mg.boundaryCondition(side,axis)==outflow )
	{
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	  if( !parameters.dbase.get<bool >("twilightZoneFlow") )
	  {
	    u(Ig1,Ig2,Ig3,V)=0.;   // for extrapolation (or Neumann)
	  }
	  else
	  {
	    assert( isRectangular );
  	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
            // should be extrapolation at outflow -- for now is Neumann
            
            
            if( true )
	    {
	      // * new way **
	      int nxd[3]={0,0,0}; //
	      nxd[axis]=1;  // x,y, or z derivative
	      e.gd(u,0,nxd[0],nxd[1],nxd[2],Ib1,Ib2,Ib3,V, Ig1,Ig2,Ig3,V,t);  
              u(Ig1,Ig2,Ig3,V)*=(2*side-1);
	    }
            else 
	    { // old way
	      if( axis==0 )
	      {
		// **  e.gd(fn[nab][grid],0,0,0,0,I1,I2,I3,parameters.dbase.get<int >("pc"),tp);
		u(Ig1,Ig2,Ig3,uc)=e.x(mg,Ib1,Ib2,Ib3,uc,t)*(2*side-1);
		u(Ig1,Ig2,Ig3,vc)=e.x(mg,Ib1,Ib2,Ib3,vc,t)*(2*side-1);
		if( mg.numberOfDimensions()==3 )
		  u(Ig1,Ig2,Ig3,wc)=e.x(mg,Ib1,Ib2,Ib3,wc,t)*(2*side-1);


		// display(u,"u After outflow BC","%5.2f ");
		
	      }
	      else if( axis==1 )
	      {
		u(Ig1,Ig2,Ig3,uc)=e.y(mg,Ib1,Ib2,Ib3,uc,t)*(2*side-1);
		u(Ig1,Ig2,Ig3,vc)=e.y(mg,Ib1,Ib2,Ib3,vc,t)*(2*side-1);
		if( mg.numberOfDimensions()==3 )
		  u(Ig1,Ig2,Ig3,wc)=e.y(mg,Ib1,Ib2,Ib3,wc,t)*(2*side-1);

	      }
	      else
	      {
		u(Ig1,Ig2,Ig3,uc)=e.z(mg,Ib1,Ib2,Ib3,uc,t)*(2*side-1);
		u(Ig1,Ig2,Ig3,vc)=e.z(mg,Ib1,Ib2,Ib3,vc,t)*(2*side-1);
		u(Ig1,Ig2,Ig3,wc)=e.z(mg,Ib1,Ib2,Ib3,wc,t)*(2*side-1);
	      }
	    }
	    
	  }
	  
	}
	else if( mg.boundaryCondition(side,axis)==slipWall )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);


          const int unc=uc+axis;  // normal component
	  if( !parameters.dbase.get<bool >("twilightZoneFlow") )
	  {
	    u(Ig1,Ig2,Ig3,V)=0.;   // (t.u).n = 0
	    u(Ib1,Ib2,Ib3,unc)=0.;   // n.u=   *********************** should be grid velocity *****
	  }
	  else
	  {
	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
            if( isRectangular )
	    {
	      u(Ig1,Ig2,Ig3,unc)=0.; // extrap normal component  // *** should use u.x+v.y=0
	      if( scalarSystem )
	      {
		// u(Ib1,Ib2,Ib3,unc)=e(mg,Ib1,Ib2,Ib3,unc,t);  // just dirichlet, not \nv\cdot\uv
                e.gd(u,0,0,0,0,Ib1,Ib2,Ib3,unc,t);  
	      }
	      else 
	      {
		// u(Ib1,Ib2,Ib3,unc)=e(mg,Ib1,Ib2,Ib3,unc,t)*(2*side-1);
                e.gd(u,0,0,0,0,Ib1,Ib2,Ib3,unc,t); 
                u(Ib1,Ib2,Ib3,unc)*=(2*side-1);
	      }
	      
		
              if( true )
	      { // *new way* 050418
		int nxd[3]={0,0,0}; //
		nxd[axis]=1;  // x,y, or z derivative
		for( int m=0; m<mg.numberOfDimensions()-1; m++ )
		{
		  int c= uc + ((axis+1+m) % mg.numberOfDimensions());

                  // ::display( u(Ig1,Ig2,Ig3,c), "ghost value before implicit BC", "%5.2f ");

                  e.gd(u,0,nxd[0],nxd[1],nxd[2],Ib1,Ib2,Ib3,c, Ig1,Ig2,Ig3,c,t);  
                  u(Ig1,Ig2,Ig3,c)*=(2*side-1);

                  // ::display( u(Ig1,Ig2,Ig3,c), "ghost value after implicit BC", "%5.2f ");

		}
	      }
	      else
	      {
		// old way 
		if( axis==0 )
		{
		  u(Ig1,Ig2,Ig3,vc)=e.x(mg,Ib1,Ib2,Ib3,vc,t)*(2*side-1);
		  if( mg.numberOfDimensions()==3 )
		    u(Ig1,Ig2,Ig3,wc)=e.x(mg,Ib1,Ib2,Ib3,wc,t)*(2*side-1);
		}
		else if( axis==1 )
		{
		  u(Ig1,Ig2,Ig3,uc)=e.y(mg,Ib1,Ib2,Ib3,uc,t)*(2*side-1);
		  if( mg.numberOfDimensions()==3 )
		    u(Ig1,Ig2,Ig3,wc)=e.y(mg,Ib1,Ib2,Ib3,wc,t)*(2*side-1);

		}
		else
		{
		  u(Ig1,Ig2,Ig3,uc)=e.z(mg,Ib1,Ib2,Ib3,uc,t)*(2*side-1);
		  u(Ig1,Ig2,Ig3,vc)=e.z(mg,Ib1,Ib2,Ib3,vc,t)*(2*side-1);
		}
	      }
	      
	    }
	    else
	    {
              // **** not rectangular ****
	      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
              if( !ok ) continue;
	      ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3);
	      
	      const int ut1c=uc + ((axis+1)%mg.numberOfDimensions());
	      const int ut2c=uc + ((axis+2)%mg.numberOfDimensions());
	  
              #ifdef USE_PPP
// 	        assert( mg.rcData->pVertexBoundaryNormal[axis][side]!=NULL );
// 	        assert( mg.rcData->pVertexBoundaryTangent[axis][side]!=NULL );
	      
//                 const realSerialArray & normal = *mg.rcData->pVertexBoundaryNormal[axis][side];
//                 const realSerialArray & tangent= *mg.rcData->pVertexBoundaryTangent[axis][side];
  	        const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
	        const realSerialArray & tangent = mg.centerBoundaryTangentArray(side,axis);
              #else
  	        const realArray & normal  = mg.vertexBoundaryNormal(side,axis);
	        const realArray & tangent = mg.centerBoundaryTangent(side,axis);
	      #endif


	      if( mg.numberOfDimensions()==2 )
	      {
		uLocal(Ig1,Ig2,Ig3,unc)=0.; // extrap normal component

                realSerialArray ue(Ib1,Ib2,Ib3,V), &uex=ue, uey(Ib1,Ib2,Ib3,V); 
                e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,V,t);
                
		// uLocal(Ib1,Ib2,Ib3,unc)= (normal(Ib1,Ib2,Ib3,0)*e(mg,Ib1,Ib2,Ib3,uc,t)+
		//		             normal(Ib1,Ib2,Ib3,1)*e(mg,Ib1,Ib2,Ib3,vc,t));
		uLocal(Ib1,Ib2,Ib3,unc)= (normal(Ib1,Ib2,Ib3,0)*ue(Ib1,Ib2,Ib3,uc)+
				          normal(Ib1,Ib2,Ib3,1)*ue(Ib1,Ib2,Ib3,vc));

// 		uLocal(Ig1,Ig2,Ig3,ut1c)=
// 		  tangent(Ib1,Ib2,Ib3,0)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,uc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,uc,t)
// 		    )
// 		  +tangent(Ib1,Ib2,Ib3,1)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,vc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,vc,t)
// 		    );

		e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
		e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);
		uLocal(Ig1,Ig2,Ig3,ut1c)=(tangent(Ib1,Ib2,Ib3,0)*(
					    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
					    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc)
					    )
					  +tangent(Ib1,Ib2,Ib3,1)*(
					    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
					    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc)
					    ));

	      }
	      else
	      {
		u(Ig1,Ig2,Ig3,unc)=0.; // extrap normal component


// 		u(Ib1,Ib2,Ib3,unc)= (normal(Ib1,Ib2,Ib3,0)*e(mg,Ib1,Ib2,Ib3,uc,t)
// 				     +normal(Ib1,Ib2,Ib3,1)*e(mg,Ib1,Ib2,Ib3,vc,t)
// 				     +normal(Ib1,Ib2,Ib3,2)*e(mg,Ib1,Ib2,Ib3,wc,t));
// 		u(Ig1,Ig2,Ig3,ut1c)=
// 		  tangent(Ib1,Ib2,Ib3,0)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,uc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,uc,t)
// 		    +normal(Ib1,Ib2,Ib3,2)*e.z(mg,Ib1,Ib2,Ib3,uc,t)
// 		    )
// 		  +tangent(Ib1,Ib2,Ib3,1)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,vc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,vc,t)
// 		    +normal(Ib1,Ib2,Ib3,2)*e.z(mg,Ib1,Ib2,Ib3,vc,t)
// 		    )
// 		  +tangent(Ib1,Ib2,Ib3,2)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,wc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,wc,t)
// 		    +normal(Ib1,Ib2,Ib3,2)*e.z(mg,Ib1,Ib2,Ib3,wc,t)
// 		    );
// 		u(Ig1,Ig2,Ig3,ut2c)=
// 		  tangent(Ib1,Ib2,Ib3,3)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,uc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,uc,t)
// 		    +normal(Ib1,Ib2,Ib3,2)*e.z(mg,Ib1,Ib2,Ib3,uc,t)
// 		    )
// 		  +tangent(Ib1,Ib2,Ib3,4)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,vc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,vc,t)
// 		    +normal(Ib1,Ib2,Ib3,2)*e.z(mg,Ib1,Ib2,Ib3,vc,t)
// 		    )
// 		  +tangent(Ib1,Ib2,Ib3,5)*(
// 		    normal(Ib1,Ib2,Ib3,0)*e.x(mg,Ib1,Ib2,Ib3,wc,t)
// 		    +normal(Ib1,Ib2,Ib3,1)*e.y(mg,Ib1,Ib2,Ib3,wc,t)
// 		    +normal(Ib1,Ib2,Ib3,2)*e.z(mg,Ib1,Ib2,Ib3,wc,t)
// 		    );

                realSerialArray ue(Ib1,Ib2,Ib3,V), &uex=ue, uey(Ib1,Ib2,Ib3,V), uez(Ib1,Ib2,Ib3,V); 
                e.gd( ue,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,V,t);

		uLocal(Ib1,Ib2,Ib3,unc)= (normal(Ib1,Ib2,Ib3,0)*ue(Ib1,Ib2,Ib3,uc)+
					  normal(Ib1,Ib2,Ib3,1)*ue(Ib1,Ib2,Ib3,vc)+
					  normal(Ib1,Ib2,Ib3,2)*ue(Ib1,Ib2,Ib3,wc));

		e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,V,t);
		e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,V,t);
		e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,V,t);

		uLocal(Ig1,Ig2,Ig3,ut1c)=
		  tangent(Ib1,Ib2,Ib3,0)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,uc)
		    )
		  +tangent(Ib1,Ib2,Ib3,1)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,vc)
		    )
		  +tangent(Ib1,Ib2,Ib3,2)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,wc)
		    );
		uLocal(Ig1,Ig2,Ig3,ut2c)=
		  tangent(Ib1,Ib2,Ib3,3)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,uc)
		    )
		  +tangent(Ib1,Ib2,Ib3,4)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,vc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,vc)
		    )
		  +tangent(Ib1,Ib2,Ib3,5)*(
		    normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,wc)+
		    normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3,wc)
		    );

	      }
	    }
	  }
	}
	else if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )
	{
	  // u.n =  v.n=
          assert( mg.numberOfDimensions()==2 );
	  
          #ifdef USE_PPP
	    assert( mg.rcData->pVertexBoundaryNormal[axis][side]!=NULL );
            const realSerialArray & normal = *mg.rcData->pVertexBoundaryNormal[axis][side];
          #else
  	    const realArray & normal  = mg.vertexBoundaryNormal(side,axis);
	  #endif


	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

	  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
	  if( !ok ) continue;
	  ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3);

	  if( !parameters.dbase.get<bool >("twilightZoneFlow") )
	  {
	    uLocal(Ig1,Ig2,Ig3,uc)=0.; 
	    uLocal(Ig1,Ig2,Ig3,vc)=0.; 
	  }
	  else
	  {
            assert( axis==1 );
	    
	    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	    realSerialArray uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 

	    e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,uc,t);
	    e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,uc,t);


	    uLocal(Ig1,Ig2,Ig3,uc)=(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3,uc)+
			            normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3,uc));
	    uLocal(Ig1,Ig2,Ig3,vc)=0.; // v.yy=0
	  }
	}
      }
    }
  }

  return 0;
}

#undef DAI2
#undef DAI3
