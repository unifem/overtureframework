#include "Maxwell.h"
#include "PlotStuff.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "ParallelUtility.h"
#include "App.h"
#include "Ogshow.h"

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


namespace
{
int numInSum=-1;
real integralOne=0.; // for testing, integrate a constant function, f(t)=1, in time 
}

  

// =======================================================================================
/// \brief Compute the time averaged intensity OR compute the real and imaginary components
//    of a time harmonic field. 
/// \details Compute the time averaged intensity to be output at t=nextTimeToPlot
///          See the notes in mx3d.pdf for details on the formulae for computing the
///          intensity given the solution at two times.   
/// ======================================================================================
int Maxwell::
computeIntensity(int current, real t, real dt, int stepNumber, real nextTimeToPlot)
{
  if( !plotIntensity && !plotHarmonicElectricFieldComponents )
    return 1;

  real time1=getCPU();

  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;

 // *** NOTE *** The intensity will not be correct if we are using the scattered fields *******

  
  // exTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-ky/(eps*cc))

  // int gridp=0;  // ** fix this *** we need to know which grid to get c from in order to compute the period.

  // const real c = cGrid(gridp);
  // const real cc= c*sqrt( kx*kx+ky*ky +kz*kz);
  // real period = 1./cc;

  real period = 1./omegaTimeHarmonic;
  

  if( intensityOption==0 && period > tPlot )
  {
    printF("computeIntensity:ERROR: period=%8.2e > tPlot=%8.2e : unable to compute the time averages since"
           " period > tPlot\n",
           period,tPlot);
    return 1;
  }
  
  if( plotIntensity && pIntensity==NULL )
  {
    pIntensity = new realCompositeGridFunction(cg);
    numInSum=-1;
  }

  if( plotHarmonicElectricFieldComponents && pHarmonicElectricField==NULL )
  {
    Range all;
    pHarmonicElectricField = new realCompositeGridFunction(cg,all,all,all,2*cg.numberOfDimensions());
    numInSum=-1;
  }
  

  if( intensityOption==0 )
  {
    // =====   compute intensity using H and a time average ===========

    if( plotHarmonicElectricFieldComponents )
    {
      printF("computeIntesnity::ERROR: cannot compute harmoinc field components if intensityOption==0\n"
             " You you choose intensityOption=1\n");
    }

    const real ta = nextTimeToPlot-period;
    const real tb=nextTimeToPlot;
    if( t+dt <= ta )
    {
      numInSum=-1;
      return 0;
    }
  
    //           <------- period ----->
    // +------+--|--+-----+-----+-----+
    //           ta       <-dt->      tb 
    //        t0    t1    t2 ...      tm

    //  Integral = .5*( fa+f1)*(t1-ta) + .5*( f1+f2 )*dt + .5*( f2+f3 ) + ... 
    //             .5*( (1-alpha)*f0 + alpha*f1 + f1)*(t1-ta) + .5*( f1+f2 )*dt + .5*( f2+f3 ) + ...
    //      fa = (1-alpha)*f0 + alpha*f1
    //      alpha = (ta-t0)/dt 

    real weight=dt;
    if( numInSum<0 )
    {
      // This is the first time through
      numInSum=0;
      integralOne=0.;

      if( plotIntensity )
	assign(*pIntensity,0.);   // intensity=0
      
      real t0=t, t1=t+dt;
      real alpha=(ta-t0)/dt;
      weight = .5*(1.-alpha)*(t1-ta);
      printF("computeIntensity:(1) t=%8.2e: ta=%8.2e, tb=%8.2e, alpha=%8.2e\n",t,ta,tb,alpha);
    }
    else if( numInSum==1 )
    {
      // second time through 
      real t0=t-dt, t1=t;
      real alpha=(ta-t0)/dt;
      weight= .5*(1.+alpha)*(t1-ta) +.5*dt;
      printF("computeTimeAveragedIntensity:(2) t=%8.2e: ta=%8.2e, tb=%8.2e, alpha=%8.2e\n",t,ta,tb,alpha);
    }
    if( fabs(t-nextTimeToPlot)<dt*.01 )
    {
      printF("computeTimeAveragedIntensity:(3) t=%8.2e: nextTimeToPlot=%8.2e, dt=%8.2e\n",t,nextTimeToPlot,dt);
      weight=.5*dt;  // last entry in sum
    } 

    numInSum++;
  
    realCompositeGridFunction & intensity = *pIntensity;

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const real eps = epsGrid(grid);
      const real mu = muGrid(grid);
      // const real cc= c*sqrt( kx*kx+ky*ky +kz*kz);

      MappedGrid & mg = cg[grid];
      realMappedGridFunction & u = cgfields[current][grid];
      realMappedGridFunction & v = intensity[grid];

#ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
#else
      const realSerialArray & uLocal = u;
      const realSerialArray & vLocal = v;
#endif


      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      getIndex(mg.dimension(),I1,I2,I3);
      const int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost); 

      real factor = weight*cGrid(grid)*eps/period;

      integralOne += weight/period;

      if( ok )
      {
	
	if( mg.numberOfDimensions()==2 )
	{
	  // vLocal(I1,I2,I3)+= factor*( SQR(uLocal(I1,I2,I3,ex))+SQR(uLocal(I1,I2,I3,ey)) );

	  // compute the intensity as (eps/2)*E^2 + (mu/2)*H^2 
	  vLocal(I1,I2,I3)+= (.5*factor)*( SQR(uLocal(I1,I2,I3,ex))+SQR(uLocal(I1,I2,I3,ey)) ) +
	    (.5*factor*mu/eps)*( SQR(uLocal(I1,I2,I3,hz) ) );
	}
	else
	{
	  Overture::abort("finish me -- we need H here");
	  vLocal(I1,I2,I3)+= factor*( SQR(uLocal(I1,I2,I3,ex))+SQR(uLocal(I1,I2,I3,ey))+
				      SQR(uLocal(I1,I2,I3,ez)) );
	}
      } // end if ok 
    } // end for grid
  
    if( fabs(t-nextTimeToPlot)<dt*.01 )
    {
      printF("computeIntensity: t=%8.2e: period=%8.2e, integralOne=%8.2e, err=%8.2e\n",
	     t,period,integralOne,fabs(1.-integralOne));
    }
    return 0;
    
  }
  else
  {
    //  === Compute the intensity or harmonic components directly assuming a time harmonic solution  ===

    // Harmonic components: 
    // For a time harmonic solution each component of the electric field is of the form
    //
    //   E(x,t) = Er(x)*cos(w*t) + Ei(x)*sin(w*t)
    //
    // Given the solution at two times, t1 and t2 we can compute Er and Ei (assuming we know w)
    //   Er = ( sin(w*t2)*E(x,t1) - sin(w*t1)*E(x,t2) )/sin(w*(t2-t1)
    //   Ei = (-cos(w*t2)*E(x,t1) + cos(w*t1)*E(x,t2) )/sin(w*(t2-t1)

    const int numberOfDimensions = cg.numberOfDimensions();
    
    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;

    // If we don't need to compute intensity, just have intensity point to the current solution
    realCompositeGridFunction & intensity = pIntensity!=NULL ? *pIntensity : cgfields[current];
    realCompositeGridFunction & hef = pHarmonicElectricField!=NULL ? *pHarmonicElectricField : cgfields[current];

    if( omegaTimeHarmonic <= 0. )
    {
      printF("Maxwell::computeIntensity: ERROR: You must set the 'time harmonic omega' in order to compute\n"
             "   the intensity or harmonic components of E\n"
             "   This should be the omega/(2 pi) and is normally equal to c*|k|^2 \n");
      OV_ABORT("Maxwell::computeIntensity: ERROR");
    }

    // Average the computed intensity over some fraction or more of a period.
    // Note that when tPlot is small, we may average over fewer steps.
    // *note* in principle we could keep even older values in the average and weight them less.
    const int numberOfTimesToAverage=max(5, intensityAveragingInterval*period/dt);  

    const real ta = nextTimeToPlot-numberOfTimesToAverage*dt;
    if( t < ta )
    {
      numInSum=-1;
      return 0;
    }
    else if( numInSum==-1 )
    {
      numInSum=0;
    }
    

    real omega = twoPi*omegaTimeHarmonic; 

    numInSum++;
    if( debug & 4 )
      printF(" *** compute intensity from 2 values in time, t=%9.3e, dt=%9.3e, omega=%8.2e omega/(2 pi)=%8.2e numInSum=%i ***\n",t,dt,omega,omega/twoPi,numInSum);

    const bool averageTheIntensity = t>nextTimeToPlot || fabs(t-nextTimeToPlot)<dt*.1;  // if true then perform the average on this step.
    if( averageTheIntensity )
    {
      printF("computeIntensity: t=%8.2e: the intensity or harmonic components were averaged over %i values"
             " in time.\n",t,numInSum);
    }


    const real t1=t-dt, t2=t;
    const real wt1=omega*t1, wt2=omega*t2;
    const real sin12 = sin(omega*dt);
    const real a1 = sin(wt2)/sin12;
    const real a2 =-sin(wt1)/sin12;
    const real b1 =-cos(wt2)/sin12;
    const real b2 = cos(wt1)/sin12;

    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const real eps = epsGrid(grid);
      const real mu = muGrid(grid);
      const real c = cGrid(grid);

      const real cEpsBy4 = c*eps/4.;
      const real cMuBy4 =  c*mu/4.;
      const real cByMu4 =  c/(mu*omega*omega*4.);

      MappedGrid & mg = cg[grid];
      realMappedGridFunction & up = cgfields[prev   ][grid];
      realMappedGridFunction & u  = cgfields[current][grid];
      realMappedGridFunction & v = intensity[grid];

#ifdef USE_PPP
      realSerialArray upLocal; getLocalArrayWithGhostBoundaries(up,upLocal);
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
      realSerialArray hefLocal; getLocalArrayWithGhostBoundaries(hef[grid],hefLocal);
#else
      const realSerialArray & upLocal = up;
      const realSerialArray & uLocal = u;
      realSerialArray & vLocal = v;
      realSerialArray & hefLocal = hef[grid];
#endif

      assert( mgp==NULL || op!=NULL );
      MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];

      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      getIndex(mg.dimension(),I1,I2,I3);
      const int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost); 

      if( ok )
      {

       // In 2d there is only one component of the curl: 
	
        const int numberOfVorticityComponents= numberOfDimensions==2 ? 1 : 3;

        Range E(ex,ex+numberOfDimensions-1);
        Range C(ex,ex+numberOfVorticityComponents-1);
	realSerialArray curlu(I1,I2,I3,C);  // we could probably use u[next] as a work space here 
	realSerialArray curlup(I1,I2,I3,C);

        // If we assign ghost and interp-neighbours then we can evaluate the curl of u
        // at all interior, boundary and interp points 
        //  ... OR just interp the intensity and extrap ghost 
	if( plotIntensity )
	{
	  mgop.derivative(MappedGridOperators::vorticityOperator,uLocal,curlu,I1,I2,I3,E);
	  mgop.derivative(MappedGridOperators::vorticityOperator,upLocal,curlup,I1,I2,I3,E);
	}
	
	// ::display(uLocal,"uLocal");
	// ::display(curlu,"curlu");

	if( numInSum==1 )
	{
	  if( plotIntensity )
	    vLocal=0.;   // first time, zero out the intensity
	  
	  if( plotHarmonicElectricFieldComponents )
	    hefLocal=0.;

	}
	
        real er,ei,hr,hi;
        int i1,i2,i3;
	if( numberOfDimensions==2 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    for( int axis=0; axis<numberOfDimensions; axis++ )
	    {
	      int n=ex+axis;

	      er = a1*upLocal(i1,i2,i3,n) + a2*uLocal(i1,i2,i3,n);
	      ei = b1*upLocal(i1,i2,i3,n) + b2*uLocal(i1,i2,i3,n);
	  
	      // vLocal(i1,i2,i3) += ei;
              if( plotIntensity )
	      {
		vLocal(i1,i2,i3) += cEpsBy4*( SQR(er) + SQR(ei) );
	      }
	      if( plotHarmonicElectricFieldComponents )
	      {
                int m=2*axis;
                hefLocal(i1,i2,i3,m  ) += er;
                hefLocal(i1,i2,i3,m+1) += ei;

                // Test: 
                // hefLocal(i1,i2,i3,0) += cEpsBy4*( SQR(er) + SQR(ei) );
	      }
	      

	    }
	    if( false )
	    { // for testing use Hz 
	      const int n=hz;
	      hr = a1*upLocal(i1,i2,i3,n) + a2*uLocal(i1,i2,i3,n);
	      hi = b1*upLocal(i1,i2,i3,n) + b2*uLocal(i1,i2,i3,n);

	      vLocal(i1,i2,i3) += cMuBy4* ( SQR(hr) + SQR(hi) );
	    }
	    else if( plotIntensity )
	    {
	      const int n=0;
	      hr = a1*curlup(i1,i2,i3,n) + a2*curlu(i1,i2,i3,n);
	      hi = b1*curlup(i1,i2,i3,n) + b2*curlu(i1,i2,i3,n);

	      // TEST:
              // hefLocal(i1,i2,i3,1) += cByMu4*( SQR(hr) + SQR(hi) );

	      // vLocal(i1,i2,i3) = hr;
	      vLocal(i1,i2,i3) += cByMu4* ( SQR(hr) + SQR(hi) );
	    }
	    
	  }
	}
	else
	{
	  if( false )
            printF("Compute intensity: grid=%i, |curl(u)|=%8.2e, |curl(up)|=%8.2e,\n",grid,max(fabs(curlu)),max(fabs(curlup)));

	  // --- 3D ---
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    for( int axis=0; axis<numberOfDimensions; axis++ )
	    {
	      int n=ex+axis;

	      er = a1*upLocal(i1,i2,i3,n) + a2*uLocal(i1,i2,i3,n);
	      ei = b1*upLocal(i1,i2,i3,n) + b2*uLocal(i1,i2,i3,n);
	      if( plotIntensity )
	      {
		hr = a1*curlup(i1,i2,i3,n) + a2*curlu(i1,i2,i3,n);
		hi = b1*curlup(i1,i2,i3,n) + b2*curlu(i1,i2,i3,n);

		vLocal(i1,i2,i3) += (cEpsBy4*( SQR(er) + SQR(ei) )  +
				     cByMu4* ( SQR(hr) + SQR(hi) ) );

	      }
	      if( plotHarmonicElectricFieldComponents )
	      {
                int m=2*axis;
                hefLocal(i1,i2,i3,m  ) += er;
                hefLocal(i1,i2,i3,m+1) += ei;
	      }

	    }
	  }
	}
	
	if( averageTheIntensity )
	{
          const real averageFactor=1./numInSum;
	  if( plotIntensity )
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      vLocal(i1,i2,i3)*=averageFactor;
	    }
	  }
	  if( plotHarmonicElectricFieldComponents )
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      for( int m=0; m<numberOfDimensions*2; m++ )
		hefLocal(i1,i2,i3,m)*=averageFactor;
	    }
	  }
	  
	}
	


      } // end if ok 
    } // end for grid

    if( averageTheIntensity )
    {
      numInSum=-1;  // reset for the next average 

      // We need to interpolate the intensity since it uses spatial derivatives which will not be defined
      // at interpolation points.
      if( plotIntensity )
      {
	
	intensity.interpolate();  

        // TEST:
	realCompositeGridFunction & hef = pHarmonicElectricField!=NULL ? *pHarmonicElectricField : cgfields[current];
	hef.interpolate();  
      }
      

    }

  }
  
  timing(timeForIntensity)+=getCPU()-time1;

}



