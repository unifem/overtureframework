#include "ArrayEvolution.h"


// ===========================================================================
///  \brief Constructor for the ArrayEvolution class which keeps a time
///       history an array (e.g. interface values)
// ===========================================================================
ArrayEvolution::
ArrayEvolution()
{
  current=-1;                   // current time level in vector
  maximumNumberOfTimeLevels=4;  // max levels to store in vectors 
  orderOfTimeAccuracy=2;        // default order of accuracy 
}

// ===========================================================================
///  \brief Destructor.
// ===========================================================================
ArrayEvolution::
~ArrayEvolution()
{
}


// ===========================================================================
///  \brief Add (or replace) a time level.
/// \param t (input) : time
/// \param x (input) : array
// ===========================================================================
int ArrayEvolution::add( real t, RealArray & x )
{
  int numberOfTimesLevels=times.size();
  const real epst = REAL_EPSILON*10.*(1.+fabs(t)); // epsilon for checking equivalent times. 
  
  if( numberOfTimesLevels==0 )
  {
    if( true )
      printF("ArrayEvolution::add:INFO: add initial entry x at t=%9.3e\n",t);
    current=0;
    times.push_back(t);
    timeHistory.push_back(x);
    return 0;
  }

  int next = ovmod(current+1,maximumNumberOfTimeLevels);
  if( t > times[current] )
  {
    // -- a new time level has been supplied ---
      if( true )
        printF("ArrayEvolution::add:INFO: add a new entry x at t=%9.3e current=%i next=%i\n",t,current,next);
    if( numberOfTimesLevels<maximumNumberOfTimeLevels )
    {
      times.push_back(t);
      timeHistory.push_back(x);
    }
    else
    {
      times[next]=t;
      timeHistory[next]=x;
    }

    current=next;
  }
  else if( fabs(t-times[current])< epst )
  {
    // replace the current time
    if( true )
      printF("ArrayEvolution::add:INFO: replace the current entry with new data, t=%9.3e, t[current=%i]=%9.3e\n",
             t,current,times[current]);
    times[current]=t;
    timeHistory[current]=x;
  }
  else
  {
    printF("ArrayEvolution::add:ERROR t=%9.3 < t[current]=%9.3. IGNORING entry\n",t,times[current]);
    return 1;
  }
  
  
  return 0;
}


// =====================================================================================
///  \brief  Evaluate the array values (or time-derivatives) at a given time t. Use the
///         time history to interpolate/extrapolate as needed.
/// \params numberOfDerivatives (input) : evaluate this time derivative
/// \params orderOfAccuracy (input) : evaluate to this order of accuracy, -1= use default
// ========================================================================================
int ArrayEvolution::eval( real t, RealArray & x, int numberOfDerivatives /* =0 */, int orderOfAccuracy /* =-1 */ )
{
  const real tEps = REAL_EPSILON*10.*(1. + fabs(t));  // epsilon for checking equivalent times 

  const int numberOfTimesLevels=times.size();

  // -- find the nearest time level with t >= times[level]
  int level=current;
  for( int k=0; k<numberOfTimesLevels-1; k++ )
  {
    if( t >= times[level]-tEps ) break;
    level = ovmod(level-1,maximumNumberOfTimeLevels);
  }
  assert( level>=0 && level<numberOfTimesLevels );

  if( true )
    printF("ArrayEvolution::eval:INFO: numDerivs=%i t=%9.3e t[level=%i]=%9.3e, t[cur=%i]=%9.3e numberOfTimesLevels=%i\n",
           numberOfDerivatives,t,level,times[level],current,times[current],numberOfTimesLevels);

  if( orderOfAccuracy<0 ) orderOfAccuracy=orderOfTimeAccuracy; // use default 
  
  if( numberOfDerivatives==0 )
  {
    // ---------------------------------------------
    // ----------- EVAL THE ARRAY VALUES -----------
    // ---------------------------------------------

    // ****** CHECK ME*******

    bool timesMatch = fabs(t-times[level])<tEps;
    if( timesMatch || orderOfAccuracy==1 || numberOfTimesLevels<=1 )
    {
      x = timeHistory[level]; 
    }
    else
    {
      // Extrapolate or interpolate to time t
       
      // We should probably use the closest times to t 
      // int numLevelsNeeded=orderOfAccuracy;

      // do this for now, assume t is closest to current time:
      int l1=      current;
      int l2=ovmod(current-1,maximumNumberOfTimeLevels);
      int l3=ovmod(current-2,maximumNumberOfTimeLevels);
      int l4=ovmod(current-3,maximumNumberOfTimeLevels);
      if( numberOfTimesLevels>1 )
      {
        // check that t is closest to times[l1] 
        assert( fabs(t-times[l1]) <= fabs(t-times[l2]) );
      }

      // Linear interpolation in time:
      if( orderOfAccuracy==2 || numberOfTimesLevels<=2 ) 
      {
        if( orderOfAccuracy>2 )
          printF("ArrayEvolution:eval:WARNING: t=%9.3e: requested orderOfAccuracy=%i "
                 "but only computing to order=2 since there are only %i time-levels\n",
                 t,orderOfAccuracy,numberOfTimesLevels);
        // Lagrange interpolation in time, 2 time levels 
        real t1=times[l1], t2=times[l2];
        assert( fabs(t1-t2) > tEps );
        real c1 = (t-t2)/(t1-t2);
        real c2 = (t-t1)/(t2-t1);
        x = c1*timeHistory[l1] + c2*timeHistory[l2];

      }     
      else if( orderOfAccuracy==3 || numberOfTimesLevels<=3 )
      {
        if( orderOfAccuracy>3 )
          printF("ArrayEvolution:eval:WARNING: t=%9.3e: requested orderOfAccuracy=%i "
                 "but only computing to order=3 since there are only %i time-levels\n",
                 t,orderOfAccuracy,numberOfTimesLevels);
        // Lagrange interpolation in time, 3 time levels 
        real t1=times[l1], t2=times[l2], t3=times[l3];
        assert( fabs(t1-t2) > tEps && fabs(t2-t3)>tEps );
        real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
        real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
        real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );
        x = c1*timeHistory[l1] + c2*timeHistory[l2] + c3*timeHistory[l3];
      }
      else if( orderOfAccuracy==4 || numberOfTimesLevels<=3 ) 
      {
        if( orderOfAccuracy>4 )
          printF("ArrayEvolution:eval:WARNING: t=%9.3e: requested orderOfAccuracy=%i "
                 "but only computing to order=4 since there are only %i time-levels\n",
                 t,orderOfAccuracy,numberOfTimesLevels);
        // Lagrange interpolation in time, 4 time levels
        real t1=times[l1], t2=times[l2], t3=times[l3], t4=times[l4];
        assert( fabs(t1-t2) > tEps && fabs(t2-t3)>tEps && fabs(t3-t4)>tEps );
        real c1 = ( (t-t2)*(t-t3)*(t-t4) )/( (t1-t2)*(t1-t3)*(t1-t4) );
        real c2 = ( (t-t3)*(t-t4)*(t-t1) )/( (t2-t3)*(t2-t4)*(t2-t1) );
        real c3 = ( (t-t4)*(t-t1)*(t-t2) )/( (t3-t4)*(t3-t1)*(t3-t2) );
        real c4 = ( (t-t1)*(t-t2)*(t-t3) )/( (t4-t1)*(t4-t2)*(t4-t3) );
        x = c1*timeHistory[l1] + c2*timeHistory[l2] + c3*timeHistory[l3] + c4*timeHistory[l4];
      }
      else
      {
        printF("ArrayEvolution:eval:ERROR: orderOfAccuracy=%i not implemented\n",orderOfAccuracy);
        OV_ABORT("finish me");
      }
    }
     
  }
  else if( numberOfDerivatives==1 )
  {
    // ---------------------------------------------
    // -------- EVAL THE FIRST DERIVATIVE ----------
    // ---------------------------------------------
    
    // ****** CHECK ME*******

    // We should probably use the closest times to t 
    // do this for now:
    int l1=current;
    int l2=ovmod(current-1,maximumNumberOfTimeLevels);
    int l3=ovmod(current-2,maximumNumberOfTimeLevels);
    int l4=ovmod(current-3,maximumNumberOfTimeLevels);
    if( numberOfTimesLevels>1 )
    {
      // check that t is closest to times[l1] 
      assert( fabs(t-times[l1]) <= fabs(t-times[l2]) );
    }

    if( numberOfTimesLevels<=1 )
    {
      printF("--ArrayEvolution: ERROR: numberOfTimesLevels=%i -- not enough to compute numberOfDerivatives=%i\n",
             numberOfTimesLevels,numberOfDerivatives);
      OV_ABORT("error");
    }
    else if( orderOfAccuracy==1 || numberOfTimesLevels<=2 ) 
    {
      if( orderOfAccuracy>1 )
        printF("ArrayEvolution:eval:WARNING: computing 1st derivative, t=%9.3e: requested orderOfAccuracy=%i "
               "but only computing to order=1 since there are only %i time-levels\n",
               t,orderOfAccuracy,numberOfTimesLevels);
      // Compute the derivative of the Lagrange polynomial
      real t1=times[l1], t2=times[l2];
      assert( fabs(t1-t2) > tEps );
      // real c1 = (t-t2)/(t1-t2);
      // real c2 = (t-t1)/(t2-t1);
      real c1t = (1.)/(t1-t2);
      real c2t = (1.)/(t2-t1);

      x = c1t*timeHistory[l1] + c2t*timeHistory[l2];
    }  
    else if( orderOfAccuracy==2 || numberOfTimesLevels<=3 )
    {
      if( orderOfAccuracy>2 )
        printF("ArrayEvolution:eval:WARNING: computing 1st derivative, t=%9.3e: requested orderOfAccuracy=%i "
               "but only computing to order=2 since there are only %i time-levels\n",
               t,orderOfAccuracy,numberOfTimesLevels);
      // Compute the derivative of the Lagrange polynomial
      real t1=times[l1], t2=times[l2], t3=times[l3];
      assert( fabs(t1-t2) > tEps && fabs(t2-t3)>tEps );
      // real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
      // real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
      // real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );
      real c1t = ( (t-t2) + (t-t3) )/( (t1-t2)*(t1-t3) );
      real c2t = ( (t-t3) + (t-t1) )/( (t2-t3)*(t2-t1) );
      real c3t = ( (t-t1) + (t-t2) )/( (t3-t1)*(t3-t2) );
      x = c1t*timeHistory[l1] + c2t*timeHistory[l2] + c3t*timeHistory[l3];
    }
    else if( orderOfAccuracy==3 || numberOfTimesLevels<=4 )
    {
      if( orderOfAccuracy>2 )
        printF("ArrayEvolution:eval:WARNING: computing 1st derivative, t=%9.3e: requested orderOfAccuracy=%i "
               "but only computing to order=3 since there are only %i time-levels\n",
               t,orderOfAccuracy,numberOfTimesLevels);

      // Compute the derivative of the Lagrange polynomial
      real t1=times[l1], t2=times[l2], t3=times[l3], t4=times[l4];
      assert( fabs(t1-t2) > tEps && fabs(t2-t3)>tEps && fabs(t3-t4)>tEps );
      // real c1 = ( (t-t2)*(t-t3)*(t-t4) )/( (t1-t2)*(t1-t3)*(t1-t4) );
      // real c2 = ( (t-t3)*(t-t4)*(t-t1) )/( (t2-t3)*(t2-t4)*(t2-t1) );
      // real c3 = ( (t-t4)*(t-t1)*(t-t2) )/( (t3-t4)*(t3-t1)*(t3-t2) );
      // real c4 = ( (t-t1)*(t-t2)*(t-t3) )/( (t4-t1)*(t4-t2)*(t4-t3) );
      real c1t = ( (t-t3)*(t-t4) + (t-t2)*(t-t4) + (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3)*(t1-t4) );
      real c2t = ( (t-t4)*(t-t1) + (t-t3)*(t-t1) + (t-t3)*(t-t4) )/( (t2-t3)*(t2-t4)*(t2-t1) );
      real c3t = ( (t-t1)*(t-t2) + (t-t4)*(t-t2) + (t-t4)*(t-t1) )/( (t3-t4)*(t3-t1)*(t3-t2) );
      real c4t = ( (t-t2)*(t-t3) + (t-t1)*(t-t3) + (t-t1)*(t-t2) )/( (t4-t1)*(t4-t2)*(t4-t3) );
      x = c1t*timeHistory[l1] + c2t*timeHistory[l2] + c3t*timeHistory[l3] + c4t*timeHistory[l4];

    }
    else
    {
      printF("ArrayEvolution:eval:ERROR: orderOfAccuracy=%i, numberOfDerivatives=%i not implemented\n",
             orderOfAccuracy,numberOfDerivatives);
        OV_ABORT("finish me");
    }
    
  }
  else if( numberOfDerivatives>1 )
  {
    printF("--ArrayEvolution:eval:ERROR: numberOfDerivatives=%i REQUESTED. FINISH ME\n",numberOfDerivatives);
    OV_ABORT("finish me");
  }

  
  
  return 0;
}


// ===========================================================================
///  \brief Return the number of time levels currently stored:
// ===========================================================================
int ArrayEvolution::getNumberOfTimeLevels() const
{
  return timeHistory.size();
}


// ===========================================================================
///  \brief Set the maximum number of time levels.
// ===========================================================================
int ArrayEvolution::setMaximumNumberOfTimeLevels(int maxLevels )
{
  maximumNumberOfTimeLevels=maxLevels;
  return 0;
}


// ===========================================================================
///  \brief Set the default order of accuracy for evaluations
// ===========================================================================
int ArrayEvolution::setOrderOfAccuracy(int order )
{
  orderOfTimeAccuracy=order;
  return 0;
}

