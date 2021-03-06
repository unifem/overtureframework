// This file automatically generated from divergence.bC with bpp.
#include "Cgsm.h"
#include "UnstructuredMapping.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"
#include "SmParameters.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

void Cgsm::
getMaxDivAndCurl( const int current, real t, 
                                    realCompositeGridFunction *pdiv /* =NULL */, int component /* =0 */,
                                    realCompositeGridFunction *pvor /* =NULL */, int vorComponent /* =0 */,
                                    realCompositeGridFunction *pDensity /* = NULL */, 
                                    int rhoComponent /* =0 */,
                                    bool computeMaxNorms /* = true */ )
// ======================================================================================
// Compute the maximum divergence, vorticity AND the max-norm of the solution
// 
//  /pdiv,component (input) : save the divergence in 'component' if this pointer is non-NULL.
//  /pvor,component (input) : save the vorticity starting at 'vorComponent' if this pointer is non-NULL.
//
// /pCharge, rhoComponent (input) : if there is a charge density then supply rho
//   in component "rhoComponent" of the grid function *pDensity
// ======================================================================================
{
    int & debug = parameters.dbase.get<int >("debug");
    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

    Range all;

    int u1c,u2c,u3c;
    if( parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation") == SmParameters::hemp )
    {
        u1c = parameters.dbase.get<int >("u1c");
        u2c = parameters.dbase.get<int >("u2c");
        u3c = parameters.dbase.get<int >("u3c");
    }
    else
    {
        u1c = parameters.dbase.get<int >("uc");
        u2c = parameters.dbase.get<int >("vc");
        u3c = parameters.dbase.get<int >("wc");
    }

    const int numberOfDimensions = cg.numberOfDimensions();
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  u1c;
    const int & vc =  u2c;
    const int & wc =  u3c;
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");
    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

//   if( useChargeDensity && pDensity==NULL )
//   {
//     // Compute the charge density if it is not supplied
//     if( pRho==NULL )
//       pRho=new realCompositeGridFunction(cg);

//     getChargeDensity(current,t,*pRho );
//     pDensity=pRho;
//     rhoComponent=0;
//   }


    Range C=numberOfComponents;
    if( computeMaxNorms )
    {
        solutionNorm.redim(C); 
        solutionNorm=0.;

        vorUMax=0.;
        divUMax=0.;
        gradUMax=0.;  // holds the max(|ux|,|vy|,|wz|) 
    }
    
    int i1Max=0, i2Max=0, i3Max=0, gridMax=0;  // keeps point where max occurs

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        realMappedGridFunction & u = gf[current].u[grid];


//      #ifdef USE_PPP
//        u.updateGhostBoundaries();
//      #endif

        MappedGrid & mg = cg[grid];
        const intArray & mask = mg.mask();
        #ifdef USE_PPP
            intSerialArray  maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
            realSerialArray uLocal;    getLocalArrayWithGhostBoundaries(u,uLocal);
        #else
            const intSerialArray & maskLocal = mask;
            const realSerialArray & uLocal   = u;
        #endif

        getIndex(mg.gridIndexRange(),I1,I2,I3);
    // Here is the box where we apply the interior equations when there is a PML
        getBoundsForPML( mg,Iv ); 

        int includeGhost=0; // do not include ghost
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);  // 
        if( !ok ) continue;  // no communication allowed after this point

        Range V = mg.numberOfDimensions()==2 ? 1 : 3;
        realSerialArray divLocal(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2)), 
                                        vorLocal(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2),V); 
        
        MappedGridOperators & mgop = (*cgop)[grid];

        int i1,i2,i3;


        const int ng=orderOfAccuracyInSpace/2;
        const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
            
//     Index J1 = Range(max(I1.getBase(),divLocal.getBase(0)+ng ),min(I1.getBound(),divLocal.getBound(0)-ng ));
//     Index J2 = Range(max(I2.getBase(),divLocal.getBase(1)+ng ),min(I2.getBound(),divLocal.getBound(1)-ng ));
//     Index J3 = Range(max(I3.getBase(),divLocal.getBase(2)+ng3),min(I3.getBound(),divLocal.getBound(2)-ng3));

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

        real *vorp = vorLocal.Array_Descriptor.Array_View_Pointer3;
        const int vorDim0=vorLocal.getRawDataSize(0);
        const int vorDim1=vorLocal.getRawDataSize(1);
        const int vorDim2=vorLocal.getRawDataSize(2);
        const int vd1=vorDim0, vd2=vd1*vorDim1, vd3=vd2*vorDim2; 
#define VOR(i0,i1,i2,i3) vorp[(i0)+(i1)*vd1+(i2)*vd2+(i3)*vd3]

        real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

        if( computeMaxNorms )
        {
            mgop.useConservativeApproximations(false);  // turn off since there are no conservative

            Range E(uc,uc+mg.numberOfDimensions()-1);
            realSerialArray udLocal(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2),E);

            real *udp = udLocal.Array_Descriptor.Array_View_Pointer3;
            const int udDim0=udLocal.getRawDataSize(0);
            const int udDim1=udLocal.getRawDataSize(1);
            const int udDim2=udLocal.getRawDataSize(2);
#undef UD
#define UD(i0,i1,i2,i3) udp[i0+udDim0*(i1+udDim1*(i2+udDim2*(i3)))]


            if( true )
            {

      	if( debug & 64 )
      	{
        	  display(uLocal,sPrintF("getMaxDivAndCurl: uLocal p=%i, t=%9.3e",myid,t),pDebugFile," %10.2e ");
      	}
            
      	divLocal=0.;
      	vorLocal=0.;
            
      	mgop.derivative(MappedGridOperators::xDerivative,uLocal,udLocal,I1,I2,I3,E);
	//display(div(I1,I2,I3)," compute div: ux","%6.2f ");
      	if( mg.numberOfDimensions()==2 )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    if( MASK(i1,i2,i3)>0 )
          	    { // find max of x-derivatives:
            	      gradUMax=max(gradUMax,max(fabs(UD(i1,i2,i3,uc)),fabs(UD(i1,i2,i3,vc))));
            	      DIV(i1,i2,i3)=UD(i1,i2,i3,uc);
                            VOR(i1,i2,i3,0)=UD(i1,i2,i3,vc); // v.x 
          	    }
        	  }
      	}
      	else
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    if( MASK(i1,i2,i3)>0 )
          	    { // find max of x-derivatives:
            	      gradUMax=max(gradUMax,max(fabs(UD(i1,i2,i3,uc)),fabs(UD(i1,i2,i3,vc)),fabs(UD(i1,i2,i3,wc))));
            	      DIV(i1,i2,i3)=UD(i1,i2,i3,uc);
                            VOR(i1,i2,i3,1)=-UD(i1,i2,i3,wc); 
                            VOR(i1,i2,i3,2)= UD(i1,i2,i3,vc); 
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
            	      gradUMax=max(gradUMax,max(fabs(UD(i1,i2,i3,uc)),fabs(UD(i1,i2,i3,vc))));
            	      DIV(i1,i2,i3)+=UD(i1,i2,i3,vc);
                            VOR(i1,i2,i3,0)-=UD(i1,i2,i3,uc); // -u.y 
          	    }
        	  }
      	}
      	else
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    if( MASK(i1,i2,i3)>0 )
          	    { // include max of y-derivatives:
            	      gradUMax=max(gradUMax,max(fabs(UD(i1,i2,i3,uc)),fabs(UD(i1,i2,i3,vc)),fabs(UD(i1,i2,i3,wc))));
            	      DIV(i1,i2,i3)+=UD(i1,i2,i3,vc);

                            VOR(i1,i2,i3,0) =UD(i1,i2,i3,wc);
                            VOR(i1,i2,i3,2)-=UD(i1,i2,i3,uc); 
          	    }
        	  }

        	  mgop.derivative(MappedGridOperators::zDerivative,uLocal,udLocal,I1,I2,I3,E);
        	  FOR_3(i1,i2,i3,I1,I2,I3)
        	  { 
          	    if( MASK(i1,i2,i3)>0 )
          	    { // include max of z-derivatives:
            	      gradUMax=max(gradUMax,fabs(UD(i1,i2,i3,uc)),fabs(UD(i1,i2,i3,vc)),fabs(UD(i1,i2,i3,wc)));
            	      DIV(i1,i2,i3)+=UD(i1,i2,i3,wc);

                            VOR(i1,i2,i3,0)-=UD(i1,i2,i3,vc);
                            VOR(i1,i2,i3,1)+=UD(i1,i2,i3,uc); 
          	    }
        	  }
      	}

      	if( debug & 64 )
      	{
        	  display(divLocal,sPrintF("getMaxDivAndCurl: divLocal p=%i, t=%9.3e",myid,t),pDebugFile," %10.2e ");
      	}
            }
            else
            {
      	divLocal=0.; 
      	mgop.derivative(MappedGridOperators::divergence,uLocal,divLocal,I1,I2,I3);
            }
            
        } // end if compute max norms
        
        bool useConservativeDivergence=false;
        if( !computeMaxNorms || useConservativeDivergence )
        {
      // just compute div(E) -- no max norms
            if( useConservativeDivergence )
      	mgop.useConservativeApproximations(useConservative);
            else
                mgop.useConservativeApproximations(false); 

            if( (debug & 4) && useConservativeDivergence )
                printF("getMaxDivergence: get conservative divergence t=%9.3e, useConservative=%i\n",t,useConservative);
            mgop.derivative(MappedGridOperators::divergence,uLocal,divLocal,I1,I2,I3);
            
        }
        mgop.useConservativeApproximations(useConservative);  // reset
        

    // display(div(I1,I2,I3),"div","%8.2e ");
        
        
    // display(div(I1,I2,I3)," div"," %10.2e ");

        if( computeMaxNorms )
        {
            real uMax[3]={0.,0.,0.}; //
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	if( MASK(i1,i2,i3)>0 )
      	{
        	  if( fabs(DIV(i1,i2,i3))>divUMax )
        	  {
          	    i1Max=i1; i2Max=i2; i3Max=i3; gridMax=grid;
          	    divUMax=fabs(DIV(i1,i2,i3));
        	  }
        	  if( fabs(VOR(i1,i2,i3,0))>vorUMax )
        	  {
          	    vorUMax=fabs(VOR(i1,i2,i3,0));
        	  }
                    for( int dir=0; dir<numberOfDimensions; dir++ )
          	    uMax[dir]=max(fabs(U(i1,i2,i3,uc+dir)),uMax[dir]);
      	}
      	else
      	{
        	  DIV(i1,i2,i3)=0.;
      	}
            
            }
            for( int dir=0; dir<numberOfDimensions; dir++ )
      	solutionNorm(uc+dir)=max(solutionNorm(uc+dir),uMax[dir]);
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
        if( pvor!=NULL )
        {
      // save the vorticity (for plotting probably)
            #ifdef USE_PPP
                realSerialArray vor; getLocalArrayWithGhostBoundaries((*pvor)[grid],vor);
            #else
                realSerialArray & vor =(*pvor)[grid];
            #endif
            vor(I1,I2,I3,V+vorComponent)=vorLocal(I1,I2,I3,V);  // we could avoid storage for vor in this case
        }
        

        if( computeMaxNorms && debug & 4 )
            printF("+++ processor=%i divUMax=%8.2e max at i=(%i,%i,%i) grid=%i \n",myid,
           	     divUMax,i1Max,i2Max,i3Max,gridMax);
            

    // Communication_Manager::Sync();
        
    } // end for grid
    if( computeMaxNorms )
    {
        divUMax=ParallelUtility::getMaxValue(divUMax);  // get max over all processors
        vorUMax=ParallelUtility::getMaxValue(vorUMax);  // get max over all processors
        gradUMax=ParallelUtility::getMaxValue(gradUMax);  // get max over all processors
    
        for( int c=C.getBase(); c<=C.getBound(); c++ )
            solutionNorm(c)=ParallelUtility::getMaxValue(solutionNorm(c)); // get max over all processors
    }
    

#undef DIV
#undef MASK
#undef U
#undef UD
}


void Cgsm::
getVelocityAndStress( const int current, real t, 
                  		      realCompositeGridFunction *pv /* =NULL */, int vComponent /* =0 */,
                  		      realCompositeGridFunction *ps /* =NULL */, int sComponent /* =0 */,
                  		      bool computeMaxNorms /* = true */ )
// ======================================================================================
/// \brief Compute the velocity and stress by differencing the displacements.
/// 
///  /pv,vComponent (input) : save the velocity in 'vComponent' if this pointer is non-NULL.
///  /ps,sComponent (input) : save the stress starting at 'sComponent' if this pointer is non-NULL.
///
// ======================================================================================
{
    int & debug = parameters.dbase.get<int >("debug");
    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

    Range all;

    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");

    const int s11c = parameters.dbase.get<int >("s11c");
    const int s12c = parameters.dbase.get<int >("s12c");
    const int s13c = parameters.dbase.get<int >("s13c");
    const int s21c = parameters.dbase.get<int >("s21c");
    const int s22c = parameters.dbase.get<int >("s22c");
    const int s23c = parameters.dbase.get<int >("s23c");
    const int s31c = parameters.dbase.get<int >("s31c");
    const int s32c = parameters.dbase.get<int >("s32c");
    const int s33c = parameters.dbase.get<int >("s33c");

    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

  // For the SVK model we compute the Cauchy stress
    const bool svkStressStrain = parameters.dbase.get<int >("pdeTypeForGodunovMethod")==2;
    const bool linearStressStrain = !svkStressStrain;

  // if( svkStressStrain )
  //   printF("getVelocityAndStress: **** SVK stress strain is ON ****\n");
    
    const int numberOfDimensions = cg.numberOfDimensions();
    int numberOfStressComponents = numberOfDimensions==2 ? 3 : 6;
  // For SVK we plot all components of the Cauchy stress (to check symmetry)
    if( svkStressStrain ) numberOfStressComponents = numberOfDimensions*numberOfDimensions;


    Range C=numberOfComponents;
    real velocityNorm=0., stressNorm=0.;
    
    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    real dti = deltaT>0. ? 1./deltaT : 1.;

    if( false )
        printF("\n --SM-- getVelocityAndStress: t=%9.3e, deltaT=%9.3e dti=%9.3e, tcur=%9.3e tprev=%9.3e\n",
                      t,deltaT,dti,gf[current].t,gf[prev].t);

    if( t==0. && gf[prev].t==0 )
    {
    // At t=0 the previous solution may not be assigned yet (For SOS this is done only after dt is computed)
    // --> Evaluate the solution at some previous time
        printF("--SM-- getVelocityAndStress:INFO: initial time t=%9.3e -- EVAL solution at past time for computing the velocity\n",t);

        real dt=1.e-4;  // what should this be?
        dti=1/dt;
        gf[prev].t=t-dt;
        assignInitialConditions(prev);
    }
    

    real & rho=parameters.dbase.get<real>("rho");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        realMappedGridFunction & u  = gf[current].u[grid];
        realMappedGridFunction & up = gf[prev   ].u[grid];

        MappedGrid & mg = cg[grid];
        const intArray & mask = mg.mask();

        #ifdef USE_PPP
            intSerialArray  maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
            realSerialArray uLocal;    getLocalArrayWithGhostBoundaries(u,uLocal);
            realSerialArray upLocal;   getLocalArrayWithGhostBoundaries(up,upLocal);
        #else
            const intSerialArray & maskLocal = mask;
            const realSerialArray & uLocal   = u;
            const realSerialArray & upLocal  = up;
        #endif

        getIndex(mg.gridIndexRange(),I1,I2,I3);
    // Here is the box where we apply the interior equations when there is a PML
        getBoundsForPML( mg,Iv ); 

        int includeGhost=0; // do not include ghost
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);  // 
        if( !ok ) continue;  // no communication allowed after this point

        Range D1=uLocal.dimension(0), D2=uLocal.dimension(1), D3=uLocal.dimension(2);

        Range V = mg.numberOfDimensions();         // velocity components
        Range S = numberOfStressComponents;        // stress components 
        Range E(uc,uc+mg.numberOfDimensions()-1);  // displacements

        realSerialArray vLocal(D1,D2,D3,V), 
                                        sLocal(D1,D2,D3,S); 
        
        MappedGridOperators & mgop = (*cgop)[grid];

    // *wdh* 090825 -- the conservative derivatives don't work below --
        mgop.useConservativeApproximations(false);   

        int i1,i2,i3;

        const real lambda = lambdaGrid(grid);
        const real mu = muGrid(grid);


        const int ng=orderOfAccuracyInSpace/2;
        const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
            
        const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
        const int maskDim0=maskLocal.getRawDataSize(0);
        const int maskDim1=maskLocal.getRawDataSize(1);
        const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

        real *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
        const int vDim0=vLocal.getRawDataSize(0);
        const int vDim1=vLocal.getRawDataSize(1);
        const int vDim2=vLocal.getRawDataSize(2);
#undef V
#define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]

        real *sp = sLocal.Array_Descriptor.Array_View_Pointer3;
        const int sDim0=sLocal.getRawDataSize(0);
        const int sDim1=sLocal.getRawDataSize(1);
        const int sDim2=sLocal.getRawDataSize(2);
#undef S
#define S(i0,i1,i2,i3) sp[i0+sDim0*(i1+sDim1*(i2+sDim2*(i3)))]

        real *pu = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) pu[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

        real *upp = upLocal.Array_Descriptor.Array_View_Pointer3;
        const int upDim0=upLocal.getRawDataSize(0);
        const int upDim1=upLocal.getRawDataSize(1);
        const int upDim2=upLocal.getRawDataSize(2);
#undef UP
#define UP(i0,i1,i2,i3) upp[i0+upDim0*(i1+upDim1*(i2+upDim2*(i3)))]


        realSerialArray ux(D1,D2,D3,E),uy(D1,D2,D3,E), uz;


        vLocal=0.;
        sLocal=0.;
            
        mgop.derivative(MappedGridOperators::xDerivative,uLocal,ux,I1,I2,I3,E);
        mgop.derivative(MappedGridOperators::yDerivative,uLocal,uy,I1,I2,I3,E);

        if( false )
        {
            printF("Get stress: lambda=%e mu=%e\n",lambda,mu);
            ::display(ux,sPrintF(" ux on grid=%i\n",grid),"%9.2e ");
            ::display(uy,sPrintF(" uy on grid=%i\n",grid),"%9.2e ");
        }

        if( mg.numberOfDimensions()==2 )
        {
            if( false )
                printF("--SM-- Plot velocity by time differences of the displacement...\n");
            
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	if( MASK(i1,i2,i3)>0 )
      	{ 
	  // velocity: approximate by differences -- this is wrong at t=0 if there is no prev solution
          // NOTE: For schemes that update the velocity this next option is not normally used (but could be turned on)
        	  V(i1,i2,i3,0) = (U(i1,i2,i3,uc) - UP(i1,i2,i3,uc))*dti;
        	  V(i1,i2,i3,1) = (U(i1,i2,i3,vc) - UP(i1,i2,i3,vc))*dti;
        	  
	  // Stress:
        	  if( linearStressStrain )
        	  {
          	    real div = ux(i1,i2,i3,uc) + uy(i1,i2,i3,vc);
          	    S(i1,i2,i3,0) = lambda*div + 2.*mu*(ux(i1,i2,i3,uc));  // s11
          	    S(i1,i2,i3,1) = mu*(uy(i1,i2,i3,uc)+ux(i1,i2,i3,vc));  // s12
          	    S(i1,i2,i3,2) = lambda*div + 2.*mu*(uy(i1,i2,i3,vc));  // s22

          	    stressNorm  =max(stressNorm  ,fabs(S(i1,i2,i3,0)),fabs(S(i1,i2,i3,1)),fabs(S(i1,i2,i3,2)));
        	  }
        	  else if( svkStressStrain )
        	  {
	    // SVK model: 

            // sigma = J^{-1} F S F^T
            // F = I + du/dx 
            // E = .5*( F^T F - I )
            // E_ij = .5*( du_i/dx_j + du_j/dx_i + du_k/dx_i * du_k/dx_j )
            // E_11 = u.x  + .5*( u.x^2 + v.x^2 )
            // 
                        real ux0=ux(i1,i2,i3,uc), uy0=uy(i1,i2,i3,uc), vx0=ux(i1,i2,i3,vc), vy0=uy(i1,i2,i3,vc);

// 	    real E11 = ux0 + .5*( ux0*ux0 + vx0*vx0 );
// 	    real E12 = .5*( uy0 + vx0 +  ux0*uy0 + vx0*vy0 );
// 	    real E21 = E12;
// 	    real E22 = vy0 + .5*( uy0*uy0 + vy0*vy0 );
//             real traceE = E11 + E22;

//             // PKII stress "S"
// 	    real S11 = lambda*traceE + 2.*mu*( E11 );
// 	    real S12 = 2.*mu*( E12 ); 
// 	    real S22 = lambda*traceE + 2.*mu*( E22 );

//             // Cauchy Stress -- finish me 


            // Cauchy Stress from nominal stress P
            // sigma = J^{-1} F P 

            // F_ij = delta_ij + du_i/dx_j 
                        real F11 = 1. + ux0, F12 = uy0, F21 = vx0, F22=1. + vy0;
          	    real aJi = F11*F22 - F12*F21;
                        if( aJi!=0. ) aJi=1./aJi;
            // nominal stress P: 
                        real P11 = u(i1,i2,i3,s11c), P12 = u(i1,i2,i3,s12c);
                        real P21 = u(i1,i2,i3,s21c), P22 = u(i1,i2,i3,s22c);
          	    
          	    S(i1,i2,i3,0) = (F11*P11 + F12*P21)*aJi; // Sigma_11
          	    S(i1,i2,i3,1) = (F11*P12 + F12*P22)*aJi; // Sigma_12
          	    
          	    S(i1,i2,i3,2) = (F21*P11 + F22*P21)*aJi; // Sigma_21
          	    S(i1,i2,i3,3) = (F21*P12 + F22*P22)*aJi; // Sigma_22

          	    stressNorm  =max(stressNorm  ,fabs(S(i1,i2,i3,0)),fabs(S(i1,i2,i3,1)),
                                                                                    fabs(S(i1,i2,i3,2)),fabs(S(i1,i2,i3,3)));

        	  }
        	  
        	  velocityNorm=max(velocityNorm,fabs(V(i1,i2,i3,0)),fabs(V(i1,i2,i3,1)));
      	}
            }
        }
        else
        {
            if( svkStressStrain )
            {
                OV_ABORT("FINISH ME FOR SVK");
            }
            

            uz.redim(D1,D2,D3,E);
            mgop.derivative(MappedGridOperators::zDerivative,uLocal,uz,I1,I2,I3,E);

      // printP("getVelocityAndStress:ERROR: compute stress - finish me for 3D\n");
            
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	if( MASK(i1,i2,i3)>0 )
      	{ 
	  // velocity: approximate by differences -- this is wrong at t=0 if there is no prev solution
        	  V(i1,i2,i3,0) = (U(i1,i2,i3,uc) - UP(i1,i2,i3,uc))*dti;
        	  V(i1,i2,i3,1) = (U(i1,i2,i3,vc) - UP(i1,i2,i3,vc))*dti;
        	  V(i1,i2,i3,2) = (U(i1,i2,i3,wc) - UP(i1,i2,i3,wc))*dti;
        	  
	  // Stress:
        	  real div = ux(i1,i2,i3,uc) + uy(i1,i2,i3,vc) + uz(i1,i2,i3,wc);
        	  S(i1,i2,i3,0) = lambda*div + 2.*mu*(ux(i1,i2,i3,uc));  // s11
        	  S(i1,i2,i3,1) = mu*(uy(i1,i2,i3,uc)+ux(i1,i2,i3,vc));  // s12
        	  S(i1,i2,i3,2) = mu*(ux(i1,i2,i3,wc)+uz(i1,i2,i3,uc));  // s13

        	  S(i1,i2,i3,3) = lambda*div + 2.*mu*(uy(i1,i2,i3,vc));  // s22
        	  S(i1,i2,i3,4) = mu*(uy(i1,i2,i3,wc)+uz(i1,i2,i3,vc));  // s23

        	  S(i1,i2,i3,5) = lambda*div + 2.*mu*(uz(i1,i2,i3,wc));  // s33

        	  velocityNorm=max(velocityNorm,fabs(V(i1,i2,i3,0)),fabs(V(i1,i2,i3,1)),fabs(V(i1,i2,i3,2)));
        	  stressNorm  =max(max(stressNorm  ,fabs(S(i1,i2,i3,0)),fabs(S(i1,i2,i3,1)),fabs(S(i1,i2,i3,2))),
                     			   max(fabs(S(i1,i2,i3,3)),fabs(S(i1,i2,i3,4)),fabs(S(i1,i2,i3,5))));

      	}
            }
        }
            
        if( false )
        {
            ::display(sLocal,sPrintF(" stress on grid=%i\n",grid),"%9.2e ");
        }

        if( pv!=NULL )
        {
      // save the velocity (for plotting probably)
            #ifdef USE_PPP
                realSerialArray v; getLocalArrayWithGhostBoundaries((*pv)[grid],v);
            #else
                realSerialArray & v =(*pv)[grid];
            #endif
            
            v(I1,I2,I3,V+vComponent)=vLocal(I1,I2,I3,V); // we could avoid storage for div in this case
        }
        if( ps!=NULL )
        {
      // save the stress (for plotting probably)
            #ifdef USE_PPP
                realSerialArray s; getLocalArrayWithGhostBoundaries((*ps)[grid],s);
            #else
                realSerialArray & s =(*ps)[grid];
            #endif
            s(I1,I2,I3,S+sComponent)=sLocal(I1,I2,I3,S);  // we could avoid storage 
        }
        

        mgop.useConservativeApproximations(useConservative);  // reset

    // Communication_Manager::Sync();
        
    } // end for grid

    if( computeMaxNorms )
    {
        velocityNorm=ParallelUtility::getMaxValue(velocityNorm);  // get max over all processors
        stressNorm  =ParallelUtility::getMaxValue(stressNorm);  // get max over all processors
    }

    if( computeMaxNorms && (true || debug & 2) )
    {
        printF("getVelocityAndStress: |velocity|=%8.2e, |stress|=%8.2e at t=%8.2e \n",velocityNorm,stressNorm,t);
    }
    
    

#undef MASK
#undef U
#undef UP
#undef S
#undef V

}

void Cgsm::
checkDisplacementAndStress( const int current, real t )
// ======================================================================================
/// \brief For the FOS, determine difference between the stress components s11c, .. and
///   the stress computed from the displacement. 
///
/// \details: This function can be used, for example, to check the consistency of the 
///   initial conditions for the FOS.
///
// ======================================================================================
{
  // The check we perform is not appropriate for TZ
    if( forcingOption==twilightZoneForcing ) return;  


    int & debug = parameters.dbase.get<int >("debug");
    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

    Range all;

    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");
    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

    const int v1c = parameters.dbase.get<int >("v1c");
    const int v2c = parameters.dbase.get<int >("v2c");
    const int v3c = parameters.dbase.get<int >("v3c");

    const bool assignVelocities= v1c>=0 ;
    const int s11c = parameters.dbase.get<int >("s11c");
    const int s12c = parameters.dbase.get<int >("s12c");
    const int s13c = parameters.dbase.get<int >("s13c");
    const int s21c = parameters.dbase.get<int >("s21c");
    const int s22c = parameters.dbase.get<int >("s22c");
    const int s23c = parameters.dbase.get<int >("s23c");
    const int s31c = parameters.dbase.get<int >("s31c");
    const int s32c = parameters.dbase.get<int >("s32c");
    const int s33c = parameters.dbase.get<int >("s33c");
    const bool assignStress = s11c >=0 ;

    const int pc = parameters.dbase.get<int >("pc");
        

  // We do nothing if we are not computing the stress
    if( !assignStress )
        return;

    printF("\n\n ********************** checkDisplacementAndStress \n\n");


    Range C=numberOfComponents;

    const int numberOfDimensions = gf[current].cg.numberOfDimensions();
    const int numberOfStressComponents = numberOfDimensions*numberOfDimensions;

  // const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;

    real & rho=parameters.dbase.get<real>("rho");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");

    real stressErr[9]={0.,0.,0.,0.,0.,0.,0.,0.,0.};
    real stressNorm[9]={0.,0.,0.,0.,0.,0.,0.,0.,0.};

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        realMappedGridFunction & u  = gf[current].u[grid];
    // realMappedGridFunction & up = gf[prev   ].u[grid];

        MappedGrid & mg = cg[grid];
        const intArray & mask = mg.mask();

        #ifdef USE_PPP
            intSerialArray  maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
            realSerialArray uLocal;    getLocalArrayWithGhostBoundaries(u,uLocal);
      // realSerialArray upLocal;   getLocalArrayWithGhostBoundaries(up,upLocal);
        #else
            const intSerialArray & maskLocal = mask;
            const realSerialArray & uLocal   = u;
      // const realSerialArray & upLocal  = up;
        #endif

        getIndex(mg.gridIndexRange(),I1,I2,I3);

        int includeGhost=0; // do not include ghost
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);  // 
        if( !ok ) continue;  // no communication allowed after this point

        Range D1=uLocal.dimension(0), D2=uLocal.dimension(1), D3=uLocal.dimension(2);

        Range V = mg.numberOfDimensions();             // velocity components
        Range S = mg.numberOfDimensions()==2 ? 3 : 6;  // stress components (assumes symmetric tensor)
        Range E(uc,uc+mg.numberOfDimensions()-1);      // displacements

        MappedGridOperators & mgop = (*cgop)[grid];

    // *wdh* 090825 -- the conservative derivatives don't work below --
        mgop.useConservativeApproximations(false);   

        int i1,i2,i3;

        const real lambda = lambdaGrid(grid);
        const real mu = muGrid(grid);

        const int ng=orderOfAccuracyInSpace/2;
        const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
            
        const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
        const int maskDim0=maskLocal.getRawDataSize(0);
        const int maskDim1=maskLocal.getRawDataSize(1);
        const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

        real *pu = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) pu[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

//     real *upp = upLocal.Array_Descriptor.Array_View_Pointer3;
//     const int upDim0=upLocal.getRawDataSize(0);
//     const int upDim1=upLocal.getRawDataSize(1);
//     const int upDim2=upLocal.getRawDataSize(2);
// #undef UP
// #define UP(i0,i1,i2,i3) upp[i0+upDim0*(i1+upDim1*(i2+upDim2*(i3)))]


        realSerialArray ux(D1,D2,D3,E),uy(D1,D2,D3,E), uz;


        mgop.derivative(MappedGridOperators::xDerivative,uLocal,ux,I1,I2,I3,E);
        mgop.derivative(MappedGridOperators::yDerivative,uLocal,uy,I1,I2,I3,E);

        if( false )
        {
            printF("Get stress: lambda=%e mu=%e\n",lambda,mu);
            ::display(ux,sPrintF(" ux on grid=%i\n",grid),"%9.2e ");
            ::display(uy,sPrintF(" uy on grid=%i\n",grid),"%9.2e ");
        }

        real u1x,u1y,u1z, u2x,u2y,u2z, u3x,u3y,u3z,div;
        real s11,s12,s13,s21,s22,s23,s31,s32,s33;

        if( mg.numberOfDimensions()==2 )
        {
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	if( MASK(i1,i2,i3)>0 )
      	{ 
	  // Compute the stress from the displacement
                    u1x = ux(i1,i2,i3,uc), u1y=uy(i1,i2,i3,uc);
                    u2x = ux(i1,i2,i3,vc), u2y=uy(i1,i2,i3,vc);
        	  
        	  div = u1x + u2y;
        	  s11 = lambda*div + 2.*mu*u1x;
        	  s12 = mu*( u1y+u2x );
        	  s21 = s12;
        	  s22 = lambda*div + 2.*mu*u2y;
                    
                    if( false )
                    {
                        printF(" i1,i2=(%3i,%3i) s11=%9.3e, computed-from-u=%9.3e, diff=%8.2e\n",
           	     i1,i2,U(i1,i2,i3,s11c),s11,fabs(s11-U(i1,i2,i3,s11c)));
                    }
                    
                    stressErr[0] = max( stressErr[0], fabs(s11-U(i1,i2,i3,s11c)) );
                    stressErr[1] = max( stressErr[1], fabs(s12-U(i1,i2,i3,s12c)) );
                    stressErr[2] = max( stressErr[2], fabs(s21-U(i1,i2,i3,s21c)) );
                    stressErr[3] = max( stressErr[3], fabs(s22-U(i1,i2,i3,s22c)) );

                    stressNorm[0]=max(stressNorm[0],fabs(s11));
                    stressNorm[1]=max(stressNorm[1],fabs(s12));
                    stressNorm[2]=max(stressNorm[2],fabs(s21));
                    stressNorm[3]=max(stressNorm[3],fabs(s22));
                    
      	}
            }
        }
        else
        {
            uz.redim(D1,D2,D3,E);
            mgop.derivative(MappedGridOperators::zDerivative,uLocal,uz,I1,I2,I3,E);

      // printF(" ************* checkDisplacementAndStress : grid = %i ****************\n",grid);

            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	if( MASK(i1,i2,i3)>0 )
      	{ 
          // Compute the stress from the displacement

                    u1x = ux(i1,i2,i3,uc), u1y=uy(i1,i2,i3,uc), u1z=uz(i1,i2,i3,uc);
                    u2x = ux(i1,i2,i3,vc), u2y=uy(i1,i2,i3,vc), u2z=uz(i1,i2,i3,vc);
                    u3x = ux(i1,i2,i3,wc), u3y=uy(i1,i2,i3,wc), u3z=uz(i1,i2,i3,wc);

        	  div = u1x+u2y+u3z;
        	  s11 = lambda*div + 2.*mu*u1x;
        	  s12 = mu*( u1y+u2x );
        	  s13 = mu*( u1z+u3x );
        	  s21 = s12;
        	  s22 = lambda*div + 2.*mu*u2y;
        	  s23 = mu*( u2z + u3y );
        	  s31 = s13;
        	  s32 = s23;
        	  s33 = lambda*div + 2.*mu*u3z;

          // printF(" i1,i2,i3,=(%3i,%3i,%3i) s11=%9.3e, computed-from-u=%9.3e, diff=%8.2e\n",
	  //       i1,i2,i3,U(i1,i2,i3,s11c),s11,fabs(s11-U(i1,i2,i3,s11c)));


                    stressErr[0] = max( stressErr[0], fabs(s11-U(i1,i2,i3,s11c)) );
                    stressErr[1] = max( stressErr[1], fabs(s12-U(i1,i2,i3,s12c)) );
                    stressErr[2] = max( stressErr[2], fabs(s13-U(i1,i2,i3,s13c)) );
                                      			      		  
                    stressErr[3] = max( stressErr[3], fabs(s21-U(i1,i2,i3,s21c)) );
                    stressErr[4] = max( stressErr[4], fabs(s22-U(i1,i2,i3,s22c)) );
                    stressErr[5] = max( stressErr[5], fabs(s23-U(i1,i2,i3,s23c)) );
                                      			      		  
                    stressErr[6] = max( stressErr[6], fabs(s31-U(i1,i2,i3,s31c)) );
                    stressErr[7] = max( stressErr[7], fabs(s32-U(i1,i2,i3,s32c)) );
                    stressErr[8] = max( stressErr[8], fabs(s33-U(i1,i2,i3,s33c)) );

                    stressNorm[0]=max(stressNorm[0],fabs(s11));
                    stressNorm[1]=max(stressNorm[1],fabs(s12));
                    stressNorm[2]=max(stressNorm[2],fabs(s13));
                    stressNorm[3]=max(stressNorm[3],fabs(s21));
                    stressNorm[4]=max(stressNorm[4],fabs(s22));
                    stressNorm[5]=max(stressNorm[5],fabs(s23));
                    stressNorm[6]=max(stressNorm[6],fabs(s31));
                    stressNorm[7]=max(stressNorm[7],fabs(s32));
                    stressNorm[8]=max(stressNorm[8],fabs(s33));

      	}
            }
        }
            
        mgop.useConservativeApproximations(useConservative);  // reset

    // Communication_Manager::Sync();
        
    } // end for grid

    if( true || debug & 2 )
    {
        ParallelUtility::getMaxValues( stressErr, stressErr, numberOfStressComponents, 0 );  // get max over all processors

        printF("checkDisplacementAndStress: t=%9.3e : Max diff between stress and stress computed from u\n",t);
        for( int i=0; i<numberOfStressComponents; i++ )
        {
            printF(" max diff stress[%i]  = %9.3e, norm=%8.2e relative-diff =%9.3e \n",
                        i,stressErr[i],stressNorm[i],stressErr[i]/max(1000.*REAL_MIN,stressNorm[i]));
        }
        
    }
    

#undef MASK
#undef U
#undef UP

}


