#include "GridEvolution.h"
#include "ParallelUtility.h"
#include "PlotStuff.h"

int GridEvolution::debug=0;

// ============================================================================================
/// \brief GridEvolution contructor.
// ============================================================================================
GridEvolution::GridEvolution()
{
  maximumNumberOfTimeLevels=4; // keep at most this many time levels
  numberOfTimeLevels=0;
  current=-1;

  accelerationOrderOfAccuracy=1;  // order of accuracy for the acceleration computation
  velocityOrderOfAccuracy=1;      // order of accurcy for the velocity computation

  time.redim(maximumNumberOfTimeLevels);
  time=0.;

  specifiedMotion=noSpecifiedMotion;
  
  remainingRestartGrids = 0;

  restartGrids = NULL;
}

// ============================================================================================
/// \brief GridEvolution destructor
// ============================================================================================
GridEvolution::~GridEvolution()
{
}

// ============================================================================================
/// \brief return the number of time levels current saved
// ============================================================================================
int GridEvolution::
getNumberOfTimeLevels() const
{
  return numberOfTimeLevels;
}


// ============================================================================================
/// \brief return the order of accuracy used to compute the acceleration.
// ============================================================================================
int GridEvolution::
getAccelerationOrderOfAccuracy() const
{
  return accelerationOrderOfAccuracy;
}

// ============================================================================================
/// \brief return the order of accuracy used to compute the velocity.
// ============================================================================================
int GridEvolution::
getVelocityOrderOfAccuracy() const
{
  return velocityOrderOfAccuracy;
}


// ============================================================================================
/// \brief set the order of accuracy used to compute the acceleration.
/// \param order (input) : a positive integer
// ============================================================================================
int GridEvolution::
setAccelerationOrderOfAccuracy( int order )
{
  accelerationOrderOfAccuracy=order;
  return 0;
}

// ============================================================================================
/// \brief set the order of accuracy used to compute the velocity.
/// \param order (input) : a positive integer
// ============================================================================================
int GridEvolution::
setVelocityOrderOfAccuracy( int order )
{
  velocityOrderOfAccuracy=order;
  return 0;
}




// ===================================================================================
/// \brief Add a grid to the time history
///
/// \param x (input) : normally x is the vertex array from a MappedGrid
/// \param t (input) : time to associate with this grid.
// ===================================================================================
// add a new grid at time t
int GridEvolution::
addGrid( const realArray & x, real t )
{
  // debug=3;  // ****
  
  if( numberOfTimeLevels!=0 )
  {
    // if( t <= time(current) && t<=0. ) // *wdh* aded t<=0 2015/07/04
    if( t <= time(current) ) 
    {
      // -- This could be a grid with t<0 for starting a simulation. --
      //    Add it to the start of the list

      // NOTE: We should generalize this to  adding any number of previous grids
      // NOTE: What happens if we want to replace a grid with an improved one ??
         
      if( t <time(current) && time(current)==0. ) //  && numberOfTimeLevels==1 && current==0 )
      {

	if( numberOfTimeLevels==maximumNumberOfTimeLevels )
	{
	  printF("--GE-- addGrid:ERROR: attempting to add a past time grid at t=%12.6e (time(current)=%12.6e)"
                 " but numberOfTimeLevels==maximumNumberOfTimeLevels=%i\n",
		 t, time(current), numberOfTimeLevels);
	  return 1;
	}

        // find where to put the next past time level 
        // we will insert the past time level at position=prev
        int prev=current;    //  start here and check backwards..
        for( int i=0; i<numberOfTimeLevels-1; i++ )
	{
          int prevm1 = (prev -1 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;
	  if( t<time(prevm1) )
            prev= prevm1;
	  else
	    break;
	}
	
	printF("--GE-- addGrid: add a past time grid at t=%8.2e to position prev=%i\n",t,prev);
        gridList.addElement(x,prev); // insert into the list here 

        // Shift the times:
        int prev0=prev;  // save insertion position 
	while( prev!=current )
	{
          int next=(prev +1 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;
          time(next)=time(prev);
	  prev=next;
  	}
        time(prev0)=t;

	current = (current +1) % maximumNumberOfTimeLevels;
        
        numberOfTimeLevels=min(numberOfTimeLevels+1,maximumNumberOfTimeLevels);

	if( debug & 4 )
	{
	  printF("GridEvolution::addGrid:INFO: adding a grid for t<0 in the list at position=%i, t=%9.3e, "
                 "(current=%i, t=%9.3e, numberOfTimeLevels=%i)\n", prev0,t,
		 current,time(current),numberOfTimeLevels);
	
	  ::display(time,"---GE-- times","%8.2e ");
	}
	
        return 1;
      }
      else if( t==time(current) && time(current)==0. )
      {
        if( true || debug & 4 )
          printF("--GE-- addGrid: REPLACING current grid at t=%8.2e\n",t);
	// gridList[current]=x;
	// replace existing element:
	gridList.deleteElement(current);
	gridList.addElement(x,current);
	return 1;
      }
      

      printF("GridEvolution::addGrid:ERROR: t=%10.4e is less than or equal to time(current)=%10.4e \n",
            t,time(current));
      printF("GridEvolution::addGrid:... skipping this grid...\n");
      return 0;

    } // end if t <= time(current)
    
  }

  current = (current +1) % maximumNumberOfTimeLevels;
  
  if( numberOfTimeLevels<maximumNumberOfTimeLevels )
  {
    gridList.addElement(x,current);
  }
  else
  { // replace existing element:
   gridList.deleteElement(current);
   gridList.addElement(x,current);
  }
  
  time(current)=t;

  numberOfTimeLevels=min(numberOfTimeLevels+1,maximumNumberOfTimeLevels);
  
  if( debug & 4 )
    printF(" *** GridEvolution::addGrid t=%9.3e, current=%i, numberOfTimeLevels=%i\n",t,current,numberOfTimeLevels);
  
  
  if (remainingRestartGrids) {

    --remainingRestartGrids;
    if (remainingRestartGrids == 0) {

      delete [] restartGrids;
      restartGrids = NULL;
    }
  }

  return 0;
}

// ==================================================================================================
/// \brief Display properties of the class.
// ==================================================================================================
int GridEvolution::
display( FILE *file /* = stdout */ ) const
{

  fPrintF(file,"\n --------------------------------------------------------------------------------------\n");
  fPrintF(file,"--- Grid Evolution: numberOfTimeLevels=%i, maximumNumberOfTimeLevels=%i, current=%i\n",
	  numberOfTimeLevels,  maximumNumberOfTimeLevels,  current);
  
  for( int level=0; level<gridList.getLength(); level++ )
  {
    ::display(gridList[level],sPrintF("Grid evolution: gridList[%i] time=%9.3e",level,time(level)),file,"%9.2e ");
  }
  fPrintF(file," --------------------------------------------------------------------------------------\n\n");

  return 0;
}



// ===========================================================================================
/// \brief Get the grid from time t
/// \para,m x (output) : a reference to the grid at tine t (if return = 0)
/// \Return 0=success, 1=not found
// ==================================================================================================
int GridEvolution::
getGrid( realArray & x, const real t ) const
{
  int level=-1;
  for(int j=0; j<numberOfTimeLevels; j++ )
  {
    int l = ovmod( current-j ,maximumNumberOfTimeLevels);
    if( fabs(time(l)-t) <= REAL_EPSILON*100.*(1+fabs(t)) )
    {
      level=l;
      break;
    }
  }
  if( level>=0 )
  {
    // mathing timne level found 
    printF("GridEvolution::getGrid: grid at t=%8.2e found: level=%i\n",t,level);
  }
  else 
  {
    // find the closest grid -- we could interpolate/extrapolate ... 
    real minDiff=REAL_MAX;
    for(int j=0; j<numberOfTimeLevels; j++ )
    {
      int l = ovmod( current-j, maximumNumberOfTimeLevels);
      if( fabs(time(l)-t) < minDiff )
      {
        minDiff=fabs(time(l)-t);
        level=l;
      }
    }
    printF("GridEvolution::getGrid: WARNING - no grid for time t=%8.2e was found. Using grid at time=%8.2e\n",
           t,time(level));

  }
  assert( level>=0 );

  if( level>=0 )
  {
    x.redim(0);
    x.reference(gridList[level]);
    return 0;
  }
  else
  {
    OV_ABORT("This should no happen");
    return 1;
  }

}




// ===========================================================================================
/// \brief Compute the grid velocity from a set of grids over time.
/// \details The GridEvolution class keeps a sequence of past grids and computes the time derivative 
/// of the grid motion using these grids. 
// ==================================================================================================
int GridEvolution::
getVelocity( real t, realSerialArray & gridVelocity, 
	     const Index &I1, const Index &I2, const Index &I3 ) const
{
  if( numberOfTimeLevels>=2 )
  {
    assert( velocityOrderOfAccuracy>0 );

    int i=(current-1 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;
    for( int j=0; j<numberOfTimeLevels-1; j++ )
    {
      if( time(i)<=t )
      {
	break;
      }
      i = (i-1 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;
    }
    
    // we should have time(i) < t < time(i+1) 


    Range Rx = gridVelocity.dimension(3);
    if( velocityOrderOfAccuracy==1 || numberOfTimeLevels<=2 )
    {
      if( velocityOrderOfAccuracy != 1 )
	printF("GridEvolution::getVelocity:WARNING: t=%9.3e : only computing the grid velocity to 1st order accuracy\n"
	       " since there are only 2 time levels available, requested accuracy=%i\n",t,velocityOrderOfAccuracy);

      const int ip1 = ovmod(i+1,maximumNumberOfTimeLevels);
      OV_GET_SERIAL_ARRAY(real,gridList[i  ],x0);
      OV_GET_SERIAL_ARRAY(real,gridList[ip1],x1);

      real dt0= time(ip1)-time(i);
      assert( dt0>0. );

      printF("--GE-- gridVelocity: using levels (%i,%9.3e) (%i,%9.3e) t=%9.3e (first-order)\n",
             i,time(i),ip1,time(ip1),t);

      gridVelocity(I1,I2,I3,Rx) = (x1(I1,I2,I3,Rx) - x0(I1,I2,I3,Rx))/dt0;

    }
    else if( velocityOrderOfAccuracy==2 || numberOfTimeLevels<=3 )
    {
      assert( numberOfTimeLevels>=3 );
      if( velocityOrderOfAccuracy != 2 )
	printF("GridEvolution::getVelocity:WARNING: only computing the grid  velocity to 2nd order accuracy"
	       " requested accuracy=%i\n",velocityOrderOfAccuracy);

      const int im1 = ovmod(i-1,maximumNumberOfTimeLevels);
      const int ip1 = ovmod(i+1,maximumNumberOfTimeLevels);

      OV_GET_SERIAL_ARRAY(real,gridList[im1],x0);
      OV_GET_SERIAL_ARRAY(real,gridList[i  ],x1);
      OV_GET_SERIAL_ARRAY(real,gridList[ip1],x2);

      const real t0=time(im1), t1=time(i), t2=time(ip1);
      real dt0= t1-t0, dt1= t2-t1;
      assert( dt0>0. && dt1>0. );

      // Compute the time derivative of the Lagrange polynomial: 
      //    x(t) = l0(t)*x0 + l1(t)*x1 + l2(t)*x2 
      // where 
      //   l0 = (t-t1)*(t-t2)/( (t0-t1)*(t0-t2) );
      //   l1 = (t-t2)*(t-t0)/( (t1-t2)*(t1-t0) );
      //   l2 = (t-t0)*(t-t1)/( (t2-t0)*(t2-t1) );
      
      real l0t = (2.*t-(t1+t2))/( (t0-t1)*(t0-t2) );
      real l1t = (2.*t-(t2+t0))/( (t1-t2)*(t1-t0) );
      real l2t = (2.*t-(t0+t1))/( (t2-t0)*(t2-t1) );
      
      printF("--GE-- gridVelocity: using levels (%i,%9.3e) (%i,%9.3e) (%i,%9.3e) t=%9.3e current=%i (second-order)\n",
             im1,t0,i,t1,ip1,t2,t,current);

      gridVelocity(I1,I2,I3,Rx) = l0t*x0(I1,I2,I3,Rx) + l1t*x1(I1,I2,I3,Rx) + l2t*x2(I1,I2,I3,Rx);
      
    }
    else 
    {
      assert( numberOfTimeLevels>=4 );
      if( velocityOrderOfAccuracy != 3 )
	printF("GridEvolution::getVelocity:WARNING: only computing the grid velocity to 3rd order accuracy"
	       " requested accuracy=%i\n",velocityOrderOfAccuracy);

      const int im2 = ovmod(i-2,maximumNumberOfTimeLevels);
      const int im1 = ovmod(i-1,maximumNumberOfTimeLevels);
      const int ip1 = ovmod(i+1,maximumNumberOfTimeLevels);
    
      OV_GET_SERIAL_ARRAY(real,gridList[im2],x0);
      OV_GET_SERIAL_ARRAY(real,gridList[im1],x1);
      OV_GET_SERIAL_ARRAY(real,gridList[i  ],x2);
      OV_GET_SERIAL_ARRAY(real,gridList[ip1],x3);
 
      const real t0=time(im2), t1=time(im1), t2=time(i), t3=time(ip1);
      real dt0= t1-t0, dt1= t2-t1, dt2= t3-t2;
      assert( dt0>0. && dt1>0. && dt2>0. );

      // Compute the time derivative of the Lagrange polynomial: 
      real l0t = ( (t-t2)*(t-t3) + (t-t1)*(t-t3) + (t-t1)*(t-t2) )/( (t0-t1)*(t0-t2)*(t0-t3) );
      real l1t = ( (t-t3)*(t-t0) + (t-t2)*(t-t0) + (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3)*(t1-t0) );
      real l2t = ( (t-t0)*(t-t1) + (t-t3)*(t-t1) + (t-t3)*(t-t0) )/( (t2-t3)*(t2-t0)*(t2-t1) );
      real l3t = ( (t-t1)*(t-t2) + (t-t0)*(t-t2) + (t-t0)*(t-t1) )/( (t3-t0)*(t3-t1)*(t3-t2) );

      printF("--GE-- gridVelocity: using levels (%i,%9.3e) (%i,%9.3e) (%i,%9.3e) (%i,%9.3e) "
             "t=%9.3e current=%i (3rd-order)\n",
             im2,t0,im1,t1,i,t2,ip1,t3,t,current);

      gridVelocity(I1,I2,I3,Rx) = l0t*x0(I1,I2,I3,Rx) + l1t*x1(I1,I2,I3,Rx) + l2t*x2(I1,I2,I3,Rx)+ l3t*x3(I1,I2,I3,Rx);
      
    }


  }
  else
  {
    printF("GridEvolution::getVelocity:WARNING: there are only %i time levels (t=%9.3e), setting gridVelocity=0.\n",
	   numberOfTimeLevels,t);

    Range Rx=gridVelocity.dimension(3);
    gridVelocity(I1,I2,I3,Rx)=0.;
  }
  
  // -- piston problem
  if( specifiedMotion==linearMotion )
  {
    // F(t) = - (a/p)*t^p 
    // gt = - a*t^{p-1}
    // gtt = -a*(p-1)*t^{p-2}
    // real ap=1., pp=4.;
    // real ap=1., pp=1.;
    // real ap=-1., pp=1.;
    // real gt = -ap*pow(t,pp-1.);

    // F(t) = a*t^p 
    const real ap=specifiedMotionParameters[0], pp=specifiedMotionParameters[1];
    real gt = ap*pp*pow(t,pp-1.);

    printF(" GridEvolution:velocity: ap=%3.1f pp=%3.1f t=%9.3e,  max=%9.3e, min=%9.3e true=%9.3e\n",ap,pp,
	   t, min(gridVelocity(I1,I2,I3,0)), max(gridVelocity(I1,I2,I3,0)), gt );

    Range all;
    
    gridVelocity(all,all,all,0)=gt; // ***********************************************************************

  }

  return 0;
}

// ========================================================================================
///  \brief Compute the grid acceleration.
// ========================================================================================
int GridEvolution::
getAcceleration( real t, realSerialArray & gridAcceleration, 
		 const Index &I1, const Index &I2, const Index &I3 ) const
{
  if( numberOfTimeLevels>=3 )
  {
    assert( accelerationOrderOfAccuracy>0 );

    int i=(current-1 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;
    for( int j=0; j<numberOfTimeLevels-1; j++ )
    {
      if( time(i)<=t )
      {
	break;
      }
      i = (i-1 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;
    }
    
    // we should have time(i) <= t < time(i+1) 

    int ip1 = (i+1                            ) % maximumNumberOfTimeLevels;
    int im1 = (i-1 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;
    int im2 = (i-2 + maximumNumberOfTimeLevels) % maximumNumberOfTimeLevels;

    real d1=time(ip1)-t;
    real d2=time(i  )-t;
    real d3=time(im1)-t;

    OV_GET_SERIAL_ARRAY_CONST(real,gridList[ip1],x1);
    OV_GET_SERIAL_ARRAY_CONST(real,gridList[i  ],x2);
    OV_GET_SERIAL_ARRAY_CONST(real,gridList[im1],x3);
    
    if( false )
      printF(" GridEvolution: accel: current=%i (ip1,i,im1)=(%i,%i,%i) (tp1,t0,tm1)=(%9.3e,%9.3e,%9.3e) t=%9.3e\n",
	     current,ip1,i,im1,time(ip1),time(i),time(im1),t);


    // Range Rx = gridAcceleration.dimension(3);
    Range Rx = x1.dimension(3); // *wdh* 101021   The gridAcceleration array may be bigger sometimes to also hold gttt
    

    // -- For approximations to the acceleration: see cgDoc/mp/fluidStructure/taylor.maple

    if( accelerationOrderOfAccuracy>=2 && numberOfTimeLevels>=4 )
    {
      // Second order approximation using 4 values      
      OV_GET_SERIAL_ARRAY_CONST(real,gridList[im2],x4);

      real d4=time(im2)-t;

      real d12=d1*d1, d13=d1*d1*d1, d22=d2*d2, d32=d3*d3, d42=d4*d4;

      real c1=-2*(d2+d3+d4)/(-d3*d4*d2+d2*d3*d1-d12*d2+d2*d4*d1+d13+d3*d4*d1-d12*d3-d12*d4);
      real c2=2*(d1+d3+d4)/(d1-d2)/(d3*d4-d3*d2+d22-d4*d2);
      real c3=-2*(d1+d4+d2)/(d1-d3)/(-d4*d2+d3*d2+d3*d4-d32);
      real c4=2*(d1+d3+d2)/(d1-d4)/(-d4*d2+d3*d2+d42-d3*d4);

      gridAcceleration(I1,I2,I3,Rx) = c1*x1(I1,I2,I3,Rx) + c2*x2(I1,I2,I3,Rx) + c3*x3(I1,I2,I3,Rx) + c4*x4(I1,I2,I3,Rx);

      if( accelerationOrderOfAccuracy != 2 )
      {
	printF("GridEvolution::getAcceleration:WARNING: only computing the acceleration to 2nd order accuracy"
	       " requested order =%i\n",accelerationOrderOfAccuracy);
      }

      if( false ) 
         printF("GridEvolution::getAcceleration:INFO t=%9.3e max(accel)=%8.2e (dt=%8.2e)\n",
                t,max(fabs( gridAcceleration(I1,I2,I3,Rx))),time(ip1)-time(i));
      

    }
    else
    {
      // First order approximation using 3 values      

      real d12=d1*d1, d22=d2*d2, d32=d3*d3;

      real c1=2/(d2*d3-d2*d1+d12-d3*d1);
      real c2=-2/(-d3*d1+d2*d1-d22+d2*d3);
      real c3=2/(-d3*d1+d2*d1+d32-d2*d3);


      gridAcceleration(I1,I2,I3,Rx) = c1*x1(I1,I2,I3,Rx) + c2*x2(I1,I2,I3,Rx) + c3*x3(I1,I2,I3,Rx);
 
      if( accelerationOrderOfAccuracy > 1  )
      {
	printF("GridEvolution::getAcceleration:WARNING: only computing the acceleration to 1st order accuracy"
	       " requested order =%i. t=%9.3e\n",accelerationOrderOfAccuracy,t);
      }

    }

//     gridAcceleration(I1,I2,I3,Rx) = ( (x2(I1,I2,I3,Rx) - x1(I1,I2,I3,Rx))/dt1
// 				     -(x1(I1,I2,I3,Rx) - x0(I1,I2,I3,Rx))/dt0 )/(.5*(dt0+dt1));
    



    
    if( false )
      printf("GridEvolution::getAcceleration: t=%9.3e, current=%i, im1,i,ip1=%i,%i,%i min(acel)=%8.2e "
	     " max(accel)=%9.2e\n",
	     t, current,im1,i,ip1,min(gridAcceleration(I1,I2,I3,Rx)),max(gridAcceleration(I1,I2,I3,Rx)));
    
  }
  else
  {
    printF("GridEvolution::getAcceleration:WARNING: there are only %i time levels (t=%9.3e). Setting accel. to ZERO.\n",
	   numberOfTimeLevels,t);

    Range Rx=gridAcceleration.dimension(3);
    gridAcceleration(I1,I2,I3,Rx)=0.;
  }
  

  // -- piston problem
  if( specifiedMotion==linearMotion )
  {
    // F(t) = - (a/p)*t^p 
    // gt = - a*t^{p-1}
    // gtt = -a*(p-1)*t^{p-2}
    // real ap=1., pp=4.;
    // real gtt = -ap*(pp-1.)*pow(t,pp-2.);
    // real ap=1., pp=1., gtt=0.;
    // real ap=-1., pp=1., gtt=0.;
    // const real ap=specifiedMotionParameters[0], pp=specifiedMotionParameters[1];
    // real gtt = -ap*(pp-1.)*pow(t,max(pp-2.,0.));

    // F(t) = a*t^p 
    const real ap=specifiedMotionParameters[0], pp=specifiedMotionParameters[1];
    real gtt = ap*pp*(pp-1.)*pow(t,max(pp-2.,0.));


    printF(" GridEvolution:acceleration: ap=%8.2e, pp=%8.2e, t=%9.3e  max=%9.3e, min=%9.3e true=%9.3e\n",
	   ap,pp,t, min(gridAcceleration(I1,I2,I3,0)), max(gridAcceleration(I1,I2,I3,0)), gtt );

    // gridAcceleration(I1,I2,I3,0)=gtt; // ***********************************************************************
    Range all;
    gridAcceleration(all,all,all,0)=gtt; // ***********************************************************************

  }
      

  return 0;
}


// =======================================================================================
/// \brief Interactively update the GridEvolution parameters
// =======================================================================================
int GridEvolution::
update(GenericGraphicsInterface & gi )
{

  // Build a dialog menu for changing parameters
  GUIState gui;
  DialogData & dialog=gui;

  dialog.setWindowTitle("GridEvolution");
  dialog.setExitCommand("exit", "exit");

  dialog.setOptionMenuColumns(1);
  aString specifiedMotionLabel[] = {"no specified motion", "linear motion", "" };
  dialog.addOptionMenu("specified motion:", specifiedMotionLabel, specifiedMotionLabel,(int)specifiedMotion );

//   aString pbLabels[] = {"no specified motion",
//                         "linear motion",
// 			""};
//   int numRows=1;
//   dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

//   aString tbCommands[] = {"save show file",
//                            ""};
//   int tbState[10];
//   tbState[0] = saveShowFile==true;
//   int numColumns=1;
//   dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  // ----- Text strings ------
  const int numberOfTextStrings=5;
  aString textCommands[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "velocity order:"; sPrintF(textStrings[nt], "%i",velocityOrderOfAccuracy);  nt++; 
  textCommands[nt] = "acceleration order:"; sPrintF(textStrings[nt], "%i",accelerationOrderOfAccuracy);  nt++; 

  // null strings terminal list
  textCommands[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);

  gi.pushGUI(gui);
  aString answer;
  int len=0;
  for(;;) 
  {
    gi.getAnswer(answer,"");      
    if( answer=="exit" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"velocity order:","%i",velocityOrderOfAccuracy) )
    {
      printF("Setting the order of accuracy for computing the velocity to %i\n",velocityOrderOfAccuracy);
    }
    else if( dialog.getTextValue(answer,"acceleration order:","%i",accelerationOrderOfAccuracy) )
    {
      printF("Setting the order of accuracy for computing the acceleration to %i\n",accelerationOrderOfAccuracy);
    }
    else if( answer=="no specified motion" )
    {
      printF("Setting specified motion option to `no specified motion'.\n");
    }
    else if( answer=="linear motion" )
    {
      specifiedMotion=linearMotion;

      printF("Setting specified motion option to `linear motion'.\n"
             "Define the velocity and acceleration from a specified motion.\n"
             "The linear motion is defined as F(t) = a*t^p\n");
      real ap=1., pp=1.;
      gi.inputString(answer,"Enter a,p");
      sScanF(answer,"%e %e",&ap,&pp);
      printF("Setting a=%9.3e, p=%9.3e\n",ap,pp);
      specifiedMotionParameters[0]=ap;
      specifiedMotionParameters[1]=pp;
      
    }
    else
    {
      printF("GridEvolution::update: ERROR: unknown answer=%s\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }

  gi.popGUI();  // pop dialog
  return 0;
}

int GridEvolution::get(const GenericDataBase & dir, const aString & name) {

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"GridEvolution");

  aString className;
  subDir.get( className,"className" );

  subDir.get( maximumNumberOfTimeLevels, "maximumNumberOfTimeLevels");
  subDir.get( numberOfTimeLevels, "numberOfTimeLevels");
  subDir.get( current, "current");
  subDir.get( accelerationOrderOfAccuracy, "accelerationOrderOfAccuracy");
  subDir.get( velocityOrderOfAccuracy, "velocityOrderOfAccuracy");

  int sz;
  subDir.get( sz, "gridList_size");
  restartGrids = new RealDistributedArray[sz];
  remainingRestartGrids = sz;
  for (int i = 0; i < sz; ++i) {
    
    subDir.getDistributed(restartGrids[i], sPrintF("gridList%i", i));
    gridList.addElement(restartGrids[i]);
  }

  subDir.get( time, "time");

  subDir.get( specifiedMotionParameters, "specifiedMotionParameters",10);

  delete &subDir;

  return 0;

}

int GridEvolution::put( GenericDataBase & dir, const aString & name) const {

  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"GridEvolution");                 // create a sub-directory 
  aString className="GridEvolution";
  subDir.put( className,"className" );
  
  subDir.put( maximumNumberOfTimeLevels, "maximumNumberOfTimeLevels");
  subDir.put( numberOfTimeLevels, "numberOfTimeLevels");
  subDir.put( current, "current");
  subDir.put( accelerationOrderOfAccuracy, "accelerationOrderOfAccuracy");
  subDir.put( velocityOrderOfAccuracy, "velocityOrderOfAccuracy");

  subDir.put( gridList.listLength(), "gridList_size");
  for (int i = 0; i < gridList.listLength(); ++i) {

    subDir.putDistributed(gridList[i], sPrintF("gridList%i", i));
  }

  subDir.put( time, "time");

  subDir.put( specifiedMotionParameters, "specifiedMotionParameters",10);

  delete &subDir;

  return 0;

}
