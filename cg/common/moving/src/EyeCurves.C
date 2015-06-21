#include "EyeCurves.h"  
#include "display.h"
#include "PlotStuff.h"
#include "NurbsMapping.h"

//===============================================================================================
/// \brief EyeCurves constructor.
// ==============================================================================================
EyeCurves::
EyeCurves()
{
  xScale =100.; // scale coordinates in space by this amount

  /* Initialize necessary values for data gathering */
  numSpaceModes = 15;    numSequences = 2*numSpaceModes + 1; /* Numbers for numSpaceModes and numTimeModes */
  numTimeModes = 5;      numTimes = 2*numTimeModes + 1;      /* reflect what was used in MATLAB            */
  totalStartSize = (numSequences)*(numTimes);

  xSmooths=NULL;
  ySmooths=NULL;
}

//===============================================================================================
/// \brief EyeCurves destructor.
// ==============================================================================================
EyeCurves::
~EyeCurves()
{
  delete [] xSmooths;
  delete [] ySmooths;
}

//===============================================================================================
/// \brief Read Fourier coefficients from the data files.
// ==============================================================================================
void EyeCurves::
readFile()
{

  real power = pow(10,-25); /* Data stored in MATLAB was multiplied by pow(10,25) */

  // -- read files on first call --
  xSmooths = new real[totalStartSize];
  ySmooths = new real[totalStartSize];

  /* Initialize storage for data gathering */

  aString xFileName = "xEyeCurveFourierCoefficients.txt";
  aString yFileName = "yEyeCurveFourierCoefficients.txt";
  char *cgenv = getenv("CG");
  if ( cgenv )
  { // look for the data files in cg/ad/runs/eyeDeform
    xFileName = aString(cgenv)+"/ad/runs/eyeDeform/"+xFileName;
    yFileName = aString(cgenv)+"/ad/runs/eyeDeform/"+yFileName;
  }

  FILE *xFile = fopen((const char*)xFileName, "r");
  FILE *yFile = fopen((const char*)yFileName, "r");
  if( xFile==NULL )
  {
    printF("EyeCurves::ERROR: could not read data file =[%s]\n",(const char*)xFileName);
    OV_ABORT("error");
  }
  if( yFile==NULL )
  {
    printF("EyeCurves::ERROR: could not read data file =[%s]\n",(const char*)yFileName);
    OV_ABORT("error");
  }

  printF("EyeCurves::INFO: read data file =[%s]\n",(const char*)xFileName);
  printF("EyeCurves::INFO: read data file =[%s]\n",(const char*)yFileName);
    
    
  /* Gather and store data from the text file */
  int i;
  for(i = 0; i < (totalStartSize); i++)
  {
    float x,y;
    fscanf(xFile, "%f\n", &x);
    fscanf(yFile, "%f\n", &y);
    xSmooths[i] = x*power/xScale;
    ySmooths[i] = y*power/xScale;
  }
  fclose(xFile);
  fclose(yFile);
    
}



// ============================================================================================
/// \brief compute the eye lid boundary at time t.
/// \param curve (output) : curve(i,0:1) are the (x,y) coordinates on the boundary of the eye lid.
/// \param time (input) : time to evaluate the curve between 0 and 2*pi
/// \param numberOfPoints (input) : evaluate at this many points around the eye-lid.
// ============================================================================================
int EyeCurves::
getEyeCurve( RealArray & curve , real time, int numPoints  )
{
  /* DATA GATHERING AND STORAGE:                 (i) */
  if( xSmooths==NULL )
  { 
    readFile();
  }

  int totalSpaces=numPoints;  // Number of grid points 
  real spaceStep = twoPi/(totalSpaces-1);

  // -- save the curve points in "curve" ---
  curve.redim(totalSpaces,2);

  // --- initialize coefficients that depend on time ---
  real Xstorage[numSequences];
  real Ystorage[numSequences];
  initializeCoefficients(xSmooths, time,Xstorage );
  initializeCoefficients(ySmooths, time,Ystorage );
  
  /* Calculate the (x,y) coordinates and save into curve(m,0:1) */
  for( int m=0; m<totalSpaces; m++ )
  {
    real space=m*spaceStep;
    curve(m,0) =  coords(Xstorage, space);
    curve(m,1) = -coords(Ystorage, space); // Note "-" : curve is upside down
  }
  /* Ensure that the last row of coefficients matches the first row */
  int m = totalSpaces-1;
  curve(m,Range(0,1))=curve(0,Range(0,1));


  return 0;


}

//========================================================================================
/// \brief Initialize coefficients that depend on time : 
//========================================================================================
void EyeCurves::
initializeCoefficients(real *xSmoothsPtr, real time, real *Xstorage )
{
    
  /* Initialize storage for each P_k(time) */
  real coeff1;
    
  /* Calculation of smoothing coefficients for a certain time */
  int i = 0; int j; int index = 0;
  while(i < totalStartSize)
  {
    coeff1 = xSmoothsPtr[i];
    i++;
    for(j = 1; j <= numTimeModes; j++)
    {
      coeff1 = coeff1 + xSmoothsPtr[i]*cos(j*time);
      i++;
    }
    for(j = (numTimeModes + 1); j <= 2*numTimeModes; j++)
    {
      coeff1 = coeff1 + xSmoothsPtr[i]*sin((j - numTimeModes)*time);
      i++;
    }
    Xstorage[index] = coeff1;
    index++;
        
    /* DEBUG COEFFICIENTS - Check P_k(time) by comparing Xstorage to Xstorage.mat in MATLAB */
    //printf("%f\n", Xstorage[0]);
  }
    
}

// ===================================================================================
/// \brief Evaluate the curve given the time-coefficients 
// ===================================================================================
real EyeCurves::
coords(real *Xstorage, real space)
{
    
  /* (iii) */
  /* Initialize storage for X(theta,t) */
  real xPara;
    
  /* Calculate X */
  xPara = Xstorage[0];
  for(int i = 1; i <= numSpaceModes; i++)
  {
    xPara = xPara + (Xstorage[i])*(cos(i*space));
  }
  for( int i = (numSpaceModes + 1); i <= (2*numSpaceModes); i++)
  {
    xPara = xPara + (Xstorage[i])*(sin((i - numSpaceModes)*space));
  }
    
  /* DEBUG X - Check individual values with MATLAB */
  //printf("%f\n", xPara);
    
  return xPara;
}


// ============================================================================================
/// \brief Output points on an eye-curve to a file.
/// \param t (input) : time to evaluate the curve between 0 and 2*pi
/// \param numberOfPoints (input) : evaluate at this many points around the eye-lid.
/// \param fileName (input) : file name
// ============================================================================================
void EyeCurves::
saveEyeCurve( real t, int numPoints, aString & fileName )
{
  RealArray curve;
  getEyeCurve( curve,t,numPoints );

  FILE *file = fopen((const char*)fileName, "w");

  // output the file that can be included in an ogen script

  fprintf(file,"%i \$nurbsDegree\n",numPoints);
  for( int i=0; i<numPoints; i++ )
  {
    fprintf(file,"%18.14e %12.14e\n",curve(i,0),curve(i,1));
  }

  fclose(file);
}
