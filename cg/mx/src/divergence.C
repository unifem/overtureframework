// This file automatically generated from divergence.bC with bpp.
#include "Maxwell.h"
#include "UnstructuredMapping.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// ===================================================================================================
//  This macro will compute the divergence (or E or H) as well as the max |grad(E)| or |grad(H)|
// Input:
//  ex,ey,ez,E,gradEMax,divEMax : replane "e" by "h" for magnetic field
//  component : save the div here for plotting
// ===================================================================================================



void Maxwell::
getMaxDivergence( const int current, real t, realCompositeGridFunction *pdiv /* =NULL */, 
                                    int component /* =0 */,
                                    realCompositeGridFunction *pDensity /* = NULL */, 
                                    int rhoComponent /* =0 */,
                                    bool computeMaxNorms /* = true */ )
// ======================================================================================
// Compute the maximum divergence AND the max-norm of the solution
// 
//  /pdiv,component (input) : save the divergence in 'component' if this pointer is non-NULL.
//
// /pCharge, rhoComponent (input) : if there is a charge density then supply rho
//   in component "rhoComponent" of the grid function *pDensity
// ======================================================================================
{

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;
    Range all;

    if( useChargeDensity && pDensity==NULL )
    {
    // Compute the charge density if it is not supplied
        if( pRho==NULL )
            pRho=new realCompositeGridFunction(cg);

        getChargeDensity(current,t,*pRho );
        pDensity=pRho;
        rhoComponent=0;
    }


    Range C(ex,hz);
    if( method==sosup )
    {
        C = cgfields[0][0].getLength(3);
    }
    
    if( computeMaxNorms )
    {
        solutionNorm.redim(C); 
        solutionNorm=0.;

        divEMax=0.;
        gradEMax=0.;  // holds the max(|ux|,|vy|,|wz|) 

        divHMax=0.;
        gradHMax=0.;
    }
    
    int i1Max=0, i2Max=0, i3Max=0, gridMax=0;  // keeps point where max occurs

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        realMappedGridFunction & u = mgp!=NULL ? fields[current] : getCGField(HField,current)[grid];


        MappedGrid & mg = cg[grid];

        const intArray & mask = mg.mask();
        #ifdef USE_PPP
            intSerialArray  maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
            realSerialArray uLocal;    getLocalArrayWithGhostBoundaries(u,uLocal);
        #else
            const intSerialArray & maskLocal = mask;
            const realSerialArray & uLocal   = u;
        #endif


        Index D1,D2,D3;
        getIndex(mg.dimension(),D1,D2,D3);
        getIndex(mg.gridIndexRange(),I1,I2,I3);
    // Here is the box where we apply the interior equations when there is a PML
        getBoundsForPML( mg,Iv ); 

        int includeGhost=0; // do not include ghost
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);  // 
        if( !ok ) continue;  // no communication allowed after this point

        realSerialArray divLocal(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2));
        
        assert( mgp==NULL || op!=NULL );
        MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];

        int i1,i2,i3;

        const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
        const int maskDim0=maskLocal.getRawDataSize(0);
        const int maskDim1=maskLocal.getRawDataSize(1);
        const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

        real *divp = divLocal.Array_Descriptor.Array_View_Pointer2;
        const int divDim0=divLocal.getRawDataSize(0);
        const int divDim1=divLocal.getRawDataSize(1);
        const int d1=divDim0, d2=d1*divDim1; 
#define DIV(i0,i1,i2) divp[(i0)+(i1)*d1+(i2)*d2]

        real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]


        if( debug & 8 )
        {
            display(uLocal,sPrintF("getMaxDivergence: grid=%i u p=%i, t=%9.3e",grid,myid,t), pDebugFile," %10.2e ");
        }
            

        if( solveForMagneticField && cg.numberOfDimensions()==3 )
        {
            Range H(hx,hx+mg.numberOfDimensions()-1);
              if( computeMaxNorms )
              {
                  mgop.useConservativeApproximations(false);  // turn off since there are no conservative
                  realSerialArray udLocal(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2),H);
                  real *udp = udLocal.Array_Descriptor.Array_View_Pointer3;
                  const int udDim0=udLocal.getRawDataSize(0);
                  const int udDim1=udLocal.getRawDataSize(1);
                  const int udDim2=udLocal.getRawDataSize(2);
            #undef UD
            #define UD(i0,i1,i2,i3) udp[i0+udDim0*(i1+udDim1*(i2+udDim2*(i3)))]
                  divLocal=0.;
                  mgop.derivative(MappedGridOperators::xDerivative,uLocal,udLocal,I1,I2,I3,H);
         //display(div(I1,I2,I3)," compute div: ux","%6.2f ");
                  if( mg.numberOfDimensions()==2 )
                  {
                        FOR_3D(i1,i2,i3,I1,I2,I3)
                        {
                            if( MASK(i1,i2,i3)>0 )
                            { // find max of x-derivatives:
                                gradHMax=max(gradHMax,max(fabs(UD(i1,i2,i3,hx)),fabs(UD(i1,i2,i3,hy))));
                                DIV(i1,i2,i3)=UD(i1,i2,i3,hx);
                            }
                        }
                  }
                  else
                  {
                        FOR_3D(i1,i2,i3,I1,I2,I3)
                        {
                            if( MASK(i1,i2,i3)>0 )
                            { // find max of x-derivatives:
                                gradHMax=max(gradHMax,max(fabs(UD(i1,i2,i3,hx)),fabs(UD(i1,i2,i3,hy)),fabs(UD(i1,i2,i3,hz))));
                                DIV(i1,i2,i3)=UD(i1,i2,i3,hx);
                            }
                        }
                  }
                  mgop.derivative(MappedGridOperators::yDerivative,uLocal, udLocal,I1,I2,I3,H);
         //display(ud(I1,I2,I3)," compute div: uy","%6.2f ");
                  if( mg.numberOfDimensions()==2 )
                  {
                        FOR_3D(i1,i2,i3,I1,I2,I3)
                        {
                            if( MASK(i1,i2,i3)>0 )
                            {  // include max of y-derivatives:
                                gradHMax=max(gradHMax,max(fabs(UD(i1,i2,i3,hx)),fabs(UD(i1,i2,i3,hy))));
                                DIV(i1,i2,i3)+=UD(i1,i2,i3,hy);
                            }
                      }
                  }
                  else
                  {
                        FOR_3D(i1,i2,i3,I1,I2,I3)
                        {
                            if( MASK(i1,i2,i3)>0 )
                            { // include max of y-derivatives:
                                gradHMax=max(gradHMax,max(fabs(UD(i1,i2,i3,hx)),fabs(UD(i1,i2,i3,hy)),fabs(UD(i1,i2,i3,hz))));
                                DIV(i1,i2,i3)+=UD(i1,i2,i3,hy);
                            }
                        }
                        mgop.derivative(MappedGridOperators::zDerivative,uLocal,udLocal,I1,I2,I3,H);
                        FOR_3(i1,i2,i3,I1,I2,I3)
                        { 
                            if( MASK(i1,i2,i3)>0 )
                            { // include max of z-derivatives:
                                gradHMax=max(gradHMax,fabs(UD(i1,i2,i3,hx)),fabs(UD(i1,i2,i3,hy)),fabs(UD(i1,i2,i3,hz)));
                                DIV(i1,i2,i3)+=UD(i1,i2,i3,hz);
                            }
                        }
                  }
                  if( debug & 8 )
                  {
                      display(divLocal,sPrintF(" divLocal p=%i, t=%9.3e",myid,t), pDebugFile," %10.2e ");
                  }
              } // end if compute max norms
              if( !computeMaxNorms || useConservativeDivergence )
              {
         // just compute div(H) -- no max norms
                  if( useConservativeDivergence )
                  	mgop.useConservativeApproximations(useConservative);
                  else
                      mgop.useConservativeApproximations(false); 
                  if( (debug & 4) && useConservativeDivergence )
                      printF("getMaxDivergence: get conservative divergence t=%9.3e, useConservative=%i\n",t,useConservative);
                  mgop.derivative(MappedGridOperators::divergence,uLocal,divLocal,I1,I2,I3,H);
              }
              mgop.useConservativeApproximations(useConservative);  // reset
              if( useChargeDensity )
              {
         // --- Subtract off rho from div(H) ----
                  assert( pDensity!=NULL );
                  realMappedGridFunction & rhog = (*pDensity)[grid]; 
                  #ifdef USE_PPP
                      realSerialArray rhoLocal; getLocalArrayWithGhostBoundaries(rhog,rhoLocal);
                  #else
                      const realSerialArray & rhoLocal = rhog;
                  #endif
                  real *rhop = rhoLocal.Array_Descriptor.Array_View_Pointer3;
                  const int rhoDim0=rhoLocal.getRawDataSize(0);
                  const int rhoDim1=rhoLocal.getRawDataSize(1);
                  const int rhoDim2=rhoLocal.getRawDataSize(2);
                  const int rhod3=rhoDim2*(rhoComponent);
                  #define RHO(i0,i1,i2) rhop[i0+rhoDim0*(i1+rhoDim1*(i2+rhod3))]
                  FOR_3D(i1,i2,i3,I1,I2,I3)
                  {
                      if( MASK(i1,i2,i3)>0 )
                      {  
                            DIV(i1,i2,i3)-=RHO(i1,i2,i3);
                      }
                      else
                      {
                          DIV(i1,i2,i3)=0.; 
                      }
                  }
              }
              if( computeMaxNorms )
              {
                  real uMax[3]={0.,0.,0.}; //
                  FOR_3D(i1,i2,i3,I1,I2,I3)
                  {
                      if( MASK(i1,i2,i3)>0 )
                      {
                          if( fabs(DIV(i1,i2,i3))>divHMax )
                          {
                              i1Max=i1; i2Max=i2; i3Max=i3; gridMax=grid;
                              divHMax=fabs(DIV(i1,i2,i3));
                          }
                          uMax[0]=max(fabs(U(i1,i2,i3,hx)),uMax[0]);
                          uMax[1]=max(fabs(U(i1,i2,i3,hy)),uMax[1]);
                          uMax[2]=max(fabs(U(i1,i2,i3,hz)),uMax[2]);  // hz=hz for 3D
                      }
                      else
                      {
                          DIV(i1,i2,i3)=0.;
                      }
                  }
                  solutionNorm(hx)=max(solutionNorm(hx),uMax[0]);
                  solutionNorm(hy)=max(solutionNorm(hy),uMax[1]);
                  solutionNorm(hz)=max(solutionNorm(hz),uMax[2]);  // hz=hz for 3D
              }
                if( pdiv!=NULL )
                {
          // save the divergence (for plotting probably)
                    #ifdef USE_PPP
                        realSerialArray div; getLocalArrayWithGhostBoundaries((*pdiv)[grid],div);
                    #else
                        realSerialArray & div =(*pdiv)[grid];
                    #endif
                    div(I1,I2,I3,component+1)=divLocal(I1,I2,I3);  // we could avoid storage for div in this case
                }
            if( computeMaxNorms && debug & 4 )
                fprintf(pDebugFile,"+++ divHMax=%8.2e max at i=(%i,%i,%i) grid=%i \n",divHMax,i1Max,i2Max,i3Max,gridMax);
        }

        Range E(ex,ex+mg.numberOfDimensions()-1);
          if( computeMaxNorms )
          {
              mgop.useConservativeApproximations(false);  // turn off since there are no conservative
              realSerialArray udLocal(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2),E);
              real *udp = udLocal.Array_Descriptor.Array_View_Pointer3;
              const int udDim0=udLocal.getRawDataSize(0);
              const int udDim1=udLocal.getRawDataSize(1);
              const int udDim2=udLocal.getRawDataSize(2);
        #undef UD
        #define UD(i0,i1,i2,i3) udp[i0+udDim0*(i1+udDim1*(i2+udDim2*(i3)))]
              divLocal=0.;
              mgop.derivative(MappedGridOperators::xDerivative,uLocal,udLocal,I1,I2,I3,E);
       //display(div(I1,I2,I3)," compute div: ux","%6.2f ");
              if( mg.numberOfDimensions()==2 )
              {
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        if( MASK(i1,i2,i3)>0 )
                        { // find max of x-derivatives:
                            gradEMax=max(gradEMax,max(fabs(UD(i1,i2,i3,ex)),fabs(UD(i1,i2,i3,ey))));
                            DIV(i1,i2,i3)=UD(i1,i2,i3,ex);
                        }
                    }
              }
              else
              {
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        if( MASK(i1,i2,i3)>0 )
                        { // find max of x-derivatives:
                            gradEMax=max(gradEMax,max(fabs(UD(i1,i2,i3,ex)),fabs(UD(i1,i2,i3,ey)),fabs(UD(i1,i2,i3,ez))));
                            DIV(i1,i2,i3)=UD(i1,i2,i3,ex);
                        }
                    }
              }
              mgop.derivative(MappedGridOperators::yDerivative,uLocal, udLocal,I1,I2,I3,E);
       //display(ud(I1,I2,I3)," compute div: uy","%6.2f ");
              if( mg.numberOfDimensions()==2 )
              {
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        if( MASK(i1,i2,i3)>0 )
                        {  // include max of y-derivatives:
                            gradEMax=max(gradEMax,max(fabs(UD(i1,i2,i3,ex)),fabs(UD(i1,i2,i3,ey))));
                            DIV(i1,i2,i3)+=UD(i1,i2,i3,ey);
                        }
                  }
              }
              else
              {
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        if( MASK(i1,i2,i3)>0 )
                        { // include max of y-derivatives:
                            gradEMax=max(gradEMax,max(fabs(UD(i1,i2,i3,ex)),fabs(UD(i1,i2,i3,ey)),fabs(UD(i1,i2,i3,ez))));
                            DIV(i1,i2,i3)+=UD(i1,i2,i3,ey);
                        }
                    }
                    mgop.derivative(MappedGridOperators::zDerivative,uLocal,udLocal,I1,I2,I3,E);
                    FOR_3(i1,i2,i3,I1,I2,I3)
                    { 
                        if( MASK(i1,i2,i3)>0 )
                        { // include max of z-derivatives:
                            gradEMax=max(gradEMax,fabs(UD(i1,i2,i3,ex)),fabs(UD(i1,i2,i3,ey)),fabs(UD(i1,i2,i3,ez)));
                            DIV(i1,i2,i3)+=UD(i1,i2,i3,ez);
                        }
                    }
              }
              if( debug & 8 )
              {
                  display(divLocal,sPrintF(" divLocal p=%i, t=%9.3e",myid,t), pDebugFile," %10.2e ");
              }
          } // end if compute max norms
          if( !computeMaxNorms || useConservativeDivergence )
          {
       // just compute div(E) -- no max norms
              if( useConservativeDivergence )
              	mgop.useConservativeApproximations(useConservative);
              else
                  mgop.useConservativeApproximations(false); 
              if( (debug & 4) && useConservativeDivergence )
                  printF("getMaxDivergence: get conservative divergence t=%9.3e, useConservative=%i\n",t,useConservative);
              mgop.derivative(MappedGridOperators::divergence,uLocal,divLocal,I1,I2,I3,E);
          }
          mgop.useConservativeApproximations(useConservative);  // reset
          if( useChargeDensity )
          {
       // --- Subtract off rho from div(E) ----
              assert( pDensity!=NULL );
              realMappedGridFunction & rhog = (*pDensity)[grid]; 
              #ifdef USE_PPP
                  realSerialArray rhoLocal; getLocalArrayWithGhostBoundaries(rhog,rhoLocal);
              #else
                  const realSerialArray & rhoLocal = rhog;
              #endif
              real *rhop = rhoLocal.Array_Descriptor.Array_View_Pointer3;
              const int rhoDim0=rhoLocal.getRawDataSize(0);
              const int rhoDim1=rhoLocal.getRawDataSize(1);
              const int rhoDim2=rhoLocal.getRawDataSize(2);
              const int rhod3=rhoDim2*(rhoComponent);
              #define RHO(i0,i1,i2) rhop[i0+rhoDim0*(i1+rhoDim1*(i2+rhod3))]
              FOR_3D(i1,i2,i3,I1,I2,I3)
              {
                  if( MASK(i1,i2,i3)>0 )
                  {  
                        DIV(i1,i2,i3)-=RHO(i1,i2,i3);
                  }
                  else
                  {
                      DIV(i1,i2,i3)=0.; 
                  }
              }
          }
          if( computeMaxNorms )
          {
              real uMax[3]={0.,0.,0.}; //
              FOR_3D(i1,i2,i3,I1,I2,I3)
              {
                  if( MASK(i1,i2,i3)>0 )
                  {
                      if( fabs(DIV(i1,i2,i3))>divEMax )
                      {
                          i1Max=i1; i2Max=i2; i3Max=i3; gridMax=grid;
                          divEMax=fabs(DIV(i1,i2,i3));
                      }
                      uMax[0]=max(fabs(U(i1,i2,i3,ex)),uMax[0]);
                      uMax[1]=max(fabs(U(i1,i2,i3,ey)),uMax[1]);
                      uMax[2]=max(fabs(U(i1,i2,i3,hz)),uMax[2]);  // hz=ez for 3D
                  }
                  else
                  {
                      DIV(i1,i2,i3)=0.;
                  }
              }
              solutionNorm(ex)=max(solutionNorm(ex),uMax[0]);
              solutionNorm(ey)=max(solutionNorm(ey),uMax[1]);
              solutionNorm(hz)=max(solutionNorm(hz),uMax[2]);  // hz=ez for 3D
          }
            if( pdiv!=NULL )
            {
        // save the divergence (for plotting probably)
                #ifdef USE_PPP
                    realSerialArray div; getLocalArrayWithGhostBoundaries((*pdiv)[grid],div);
                #else
                    realSerialArray & div =(*pdiv)[grid];
                #endif
                div(I1,I2,I3,component)=divLocal(I1,I2,I3);  // we could avoid storage for div in this case
            }

        if( computeMaxNorms && debug & 4 )
            fprintf(pDebugFile,"+++ divEMax=%8.2e max at i=(%i,%i,%i) grid=%i \n",divEMax,i1Max,i2Max,i3Max,gridMax);
            

    // Communication_Manager::Sync();
        
    } // end for grid
    if( computeMaxNorms )
    {
        divEMax=getMaxValue(divEMax);  // get max over all processors
        gradEMax=getMaxValue(gradEMax);  // get max over all processors

        if( solveForMagneticField && cg.numberOfDimensions()==3 )
        {
            divHMax=getMaxValue(divHMax);  // get max over all processors
            gradHMax=getMaxValue(gradHMax);  // get max over all processors
        }
        
        for( int c=C.getBase(); c<=C.getBound(); c++ )
            solutionNorm(c)=getMaxValue(solutionNorm(c)); // get max over all processors
    }
    

#undef DIV
#undef MASK
#undef U
#undef UD
}




// ===========================================================================
//     Gaussian Charge Pulse
//
//      rho = a*exp( - [beta* | xv - vv*t | ]^p ) )
//
//  DIM : 2,3
//  GRIDTYPE : curvilinear, rectangular
// ===========================================================================



void Maxwell::
getChargeDensity( int current, real t, realCompositeGridFunction & ucg, int component /* =0 */ )
// ======================================================================================
// Determine the charge density and save in u(i1,i2,i3,component)
// 
// ======================================================================================
{
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;
    Range all;
    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        getChargeDensity( t, ucg[grid], component );
            
    }

}

void Maxwell::
getChargeDensity( real t, realMappedGridFunction & u, int component /* =0 */ )
// ======================================================================================
// Determine the charge density and save in u(i1,i2,i3,component)
// 
// ======================================================================================
{
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

    MappedGrid & mg = *u.getMappedGrid(); 

    const bool isRectangular = mg.isRectangular();

    const intArray & mask = mg.mask();

    #ifdef USE_PPP
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
        intSerialArray  maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
    #else
        const realSerialArray & uLocal   = u;
        const intSerialArray & maskLocal = mask;
    #endif


    int extra=orderOfAccuracyInSpace/2;
    getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

    int includeGhost=0; // do not include ghost
    bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);  // 

    if( !ok ) return;  // no communication allowed after this point

    if( forcingOption==twilightZoneForcing )
    {
        mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
            
        #ifdef USE_PPP
            realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
        #else
            const realSerialArray & xLocal = mg.vertex(); // fix this 
        #endif

        assert( tz!=NULL );
        OGFunction & e = *tz;
        realSerialArray rho(I1,I2,I3); // ****************

        e.gd( rho,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,rc,t);
        uLocal(I1,I2,I3,component)=rho;
    }
    else 
    {

        real dx[3],xab[2][3];
        if( isRectangular )
            mg.getRectangularGridParameters( dx, xab );
        else
            mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );

        const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
        const int maskDim0=maskLocal.getRawDataSize(0);
        const int maskDim1=maskLocal.getRawDataSize(1);
        const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

        real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

        #ifdef USE_PPP
            realSerialArray xLocal; if( isRectangular ) getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
        #else
            const realArray & xLocal = isRectangular ? u : mg.center();
        #endif

        real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
        const int xDim0=xLocal.getRawDataSize(0);
        const int xDim1=xLocal.getRawDataSize(1);
        const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

        int i1,i2,i3;

        real x0,x1,x2;
            
        if( forcingOption==gaussianChargeSource )
        {
      //   rho = a*exp( -beta* | xv - vv*t |^2 ) )

            real rad;
            real amplitude,beta,p,xp0,xp1,xp2,vp0,vp1,vp2;
      	
            int ngcs=0;  // only assign one source for now

            amplitude=gaussianChargeSourceParameters[ngcs][0];
            beta     =gaussianChargeSourceParameters[ngcs][1];
            p        =gaussianChargeSourceParameters[ngcs][2];
            xp0      =gaussianChargeSourceParameters[ngcs][3];
            xp1      =gaussianChargeSourceParameters[ngcs][4];
            xp2      =gaussianChargeSourceParameters[ngcs][5];
            vp0      =gaussianChargeSourceParameters[ngcs][6];
            vp1      =gaussianChargeSourceParameters[ngcs][7];
            vp2      =gaussianChargeSourceParameters[ngcs][8];
      	
            if( mg.numberOfDimensions()==2 && isRectangular )
            {
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
              if( MASK(i1,i2,i3)!=0 )  
              {
                      x0 = xab[0][0]+i1*dx[0] -xp0;
                      x1 = xab[0][1]+i2*dx[1] -xp1;
                      rad = sqrt( SQR( x0-vp0*t ) + SQR( x1-vp1*t ) ); 
                U(i1,i2,i3,component)=amplitude*exp(- pow(beta*rad,p) );
              }
            }
            }
            else if( mg.numberOfDimensions()==2 && !isRectangular )
            {
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
              if( MASK(i1,i2,i3)!=0 )  
              {
                      x0 = X(i1,i2,i3,0)-xp0;
                      x1 = X(i1,i2,i3,1)-xp1;
                      rad = sqrt( SQR( x0-vp0*t ) + SQR( x1-vp1*t ) ); 
                U(i1,i2,i3,component)=amplitude*exp(- pow(beta*rad,p) );
              }
            }
            }
            else if( mg.numberOfDimensions()==3 && isRectangular )
            {
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
              if( MASK(i1,i2,i3)!=0 )  
              {
                      x0 = xab[0][0]+i1*dx[0] -xp0;
                      x1 = xab[0][1]+i2*dx[1] -xp1;
                          x2 = xab[0][2]+i3*dx[2] -xp2;
                      rad = sqrt( SQR( x0-vp0*t ) + SQR( x1-vp1*t ) + SQR( x2-vp2*t ) );
                U(i1,i2,i3,component)=amplitude*exp(- pow(beta*rad,p) );
              }
            }
            }
            else if( mg.numberOfDimensions()==3 && !isRectangular )
            {
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
              if( MASK(i1,i2,i3)!=0 )  
              {
                      x0 = X(i1,i2,i3,0)-xp0;
                      x1 = X(i1,i2,i3,1)-xp1;
                          x2 = X(i1,i2,i3,2)-xp2;
                      rad = sqrt( SQR( x0-vp0*t ) + SQR( x1-vp1*t ) + SQR( x2-vp2*t ) );
                U(i1,i2,i3,component)=amplitude*exp(- pow(beta*rad,p) );
              }
            }
            }
            else
            {
      	Overture::abort("getChargeDensity:error: This shouldn't happen.");
            }
      	
        }
        else
        {
            printF("getChargeDensity:ERROR: unexpected forcingOption=%i\n",forcingOption); 
            Overture::abort("error");
        }
            
    }
}

