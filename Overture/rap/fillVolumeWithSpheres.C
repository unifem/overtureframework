#include "ModelBuilder.h"
#include "nurbsCurveEditor.h"
#include "ArraySimple.h"
#include "RevolutionMapping.h"
#include "SphereMapping.h"
#include "MappingProjectionParameters.h"
#include "CompositeSurface.h"
#include "UnstructuredMapping.h"
#include "CompositeTopology.h"
#include "MappingGeometry.h"
#include "RandomSampling.h"

static real timeForIsOverlapping=0.;

static bool
isOverlapping(const RealArray &oneSphere, const double &radius, int myPlace,
              const RealArray &allSpheres, const RealArray &radii, int nPlaced)
// ===================================================================================
//   Check to see if a given sphere (myPlace) overlaps any of the other spheres
//   in the list
// /Notes:
//     If spheres were sorted by the x-coordinate, then searching could be made fast.
// ===================================================================================
{
  real time0=getCPU();

  double dr;
  double dx;
  double dy;
  double dz;
  double distSq;

  for (int i = 0; i < myPlace; i++) {
    dr = radius + radii(i);
    dx = oneSphere(0,0) - allSpheres(i,0);
    dy = oneSphere(0,1) - allSpheres(i,1);
    dz = oneSphere(0,2) - allSpheres(i,2);
    distSq = dx*dx + dy*dy + dz*dz;

    if (distSq <= dr*dr) {
      timeForIsOverlapping+=getCPU()-time0;
      return true;
    }
  }

  for (int i = myPlace+1; i < nPlaced; i++) {
    dr = radius + radii(i);
    dx = oneSphere(0,0) - allSpheres(i,0);
    dy = oneSphere(0,1) - allSpheres(i,1);
    dz = oneSphere(0,2) - allSpheres(i,2);
    distSq = dx*dx + dy*dy + dz*dz;

    if (distSq <= dr*dr) {
      timeForIsOverlapping+=getCPU()-time0;
      return true;
    }
  }
  timeForIsOverlapping+=getCPU()-time0;
  return false;
}

static double
dsign(double arg, double signP) {
  if (signP < 0.0) {
    return -abs(arg);
  }
  else {
    return abs(arg);
  }
}


int
fillVolumeWithSpheres( CompositeSurface & model, GenericGraphicsInterface& gi, SphereLoading & sphereDist,
                       int & nsp, int & nsr, real & sphereProbability, real & volume, real & volumeFraction,
                       int & numberOfSpheres, RealArray & sr, RealArray & sp, int & RNGSeed, int debug )
// ====================================================================================
// /Description:
//    Fill a volume with a distribution of spheres. 
//    Use a jiggling algorithm to fit the spheres in.
//  /Return value: 0=Ok, 1=error
//  /Author: Dale Slone.
//
//  /Notes:
//       We could keep a 3D "locator" array that gets filled in as we find spheres that
//   are inside or outside
//            locator(0:n1,0:n2,0:n3) : corresponds to a grid point (or cell center?) x=xa+i1*h1, y=ya+i2*h2, z=za+i3*h3 
//            locator(i1,i2,i3) = -dist : this grid point is outside by at least a distance "dist"
//                              =  0 : unknown
//                              = dist>0 : this point is inside and a distance "dist" from the boundary
//    This array could be used to keep/discard potential spheres before using the more expensive insideOrOutside
// 
// ===================================================================================
{
  #ifndef USE_PPP
  RealArray & spheres = sphereDist.sphereCenter;
  RealArray & sphereRadii = sphereDist.sphereRadius; 

  // get bounds on the model
  real boundingBoxp[6];
  #define boundingBox(side,axis) boundingBoxp[(side)+2*(axis)]
  real volumeOfBoundingBox=1.;
  int axis;
  for( axis=0; axis<model.getRangeDimension(); axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      boundingBox(side,axis)=(real)model.getRangeBound(side,axis);
    }
    volumeOfBoundingBox*= (boundingBox(1,axis)-boundingBox(0,axis));
  }
     
  // This next object is used to pass and return additional parameters to the "project" function.
  MappingProjectionParameters mpParams;
  typedef MappingProjectionParameters MPP;
  realArray & surfaceNormal  = mpParams.getRealArray(MPP::normal);
  intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  realArray & xOld           = mpParams.getRealArray(MPP::x);  // this could be used as an initial guess
  intArray & elementIndex    = mpParams.getIntArray(MPP::elementIndex);
     
     // Allocate space for the normal to indicate that it should be computed
  int numberOfPointsToProject=1;  // we only project 1 point at a time here
  surfaceNormal.redim(numberOfPointsToProject,3);
  surfaceNormal=0.;       
     
//          IntegerArray inside(1); inside=-1;
//  	cs.insideOrOutside(x,inside);

  realArray x(1,3),xP(1,3); // arrays to hold initial and projected points
  x=0.;

  int numberOfRadii;
  if (sphereDist.fromGUI) {
    sphereDist.RNGSeed = RNGSeed;
    if (nsp == nsr) {
      numberOfRadii = nsp;
      sphereDist.volumeFraction = volumeFraction;
      sphereDist.updateDistribution(numberOfRadii, sr, sp);
    }
    else {
      cerr << "# of sphere radii [" << nsr
	   << "] .NE. # of probabilities [" << nsp << "]" << endl;
      return 1;
    }
  }
  else {
    numberOfRadii = sphereDist.sphereDistribution.getLength(0);
  }
  intArray numberOfParticles(numberOfRadii);
  realArray volumeAvailableForRadii(numberOfRadii);
  realArray cumulativeProbability(numberOfRadii);
  double volumeOfParticle;
  double tv = 0.0;
  double tvf = 0.0;
  const double distTolerance = 0.01;
  volumeFraction = abs(sphereDist.volumeFraction);

  for (int j = 0; j < numberOfRadii; j++) {
    tvf += sphereDist.sphereDistribution(j,1);
    cumulativeProbability(j) = tvf;
    numberOfParticles(j) = 0;
  }
  if (abs(1.0 - tvf) > distTolerance) {
    printf("WARNING: volume fraction probabilities don't sum to 1 [%lg]\n",
	   tvf);
  }

  // set up the Random Number Generator with the given seed
  RandomSampling *rng = new RandomSampling();
  for (int irng = 0; irng < sphereDist.RNGSeed; irng++) {
    rng->RandomDouble();
  }

  // Generate distribution of sphere based solely on radius distribution.
  double av = volume * volumeFraction;
  bool more = true;
  while (more) {
    double prob = rng->RandomDouble();
    int rIndex = -1;
    int j = 0;
    while (j < numberOfRadii && rIndex < 0) {
      if (prob >= cumulativeProbability(j)) {
	j++;
      }
      else {
	rIndex = j;
      }
    }
    if (rIndex == -1) {
      cout << "Can't find appropriate bin for probabilitY " << prob << endl;
    }
    double thisRadius = sphereDist.sphereDistribution(rIndex, 0);
    double thisVolume = 4./3.*(Pi)*pow(thisRadius,3.);
    if ((tv + thisVolume) < av) {
      tv += thisVolume;
      numberOfParticles(rIndex)++;
      numberOfSpheres++;
    }
    else {
      more = false;
    }
  }

  // attempt to fill spheres
  real timea=getCPU();
  real timeForProject=0., timeForInsideOutside=0.;

  int ts = 0;
  real dr;
  const real radiusFraction = 0.5;
  double *cosines;
  realArray keepSpheres(numberOfSpheres, 3);
  int       numberOfKeptSpheres = 0;
  realArray testSpheres(numberOfSpheres, 3);
  // realArray sphereRadii(numberOfSpheres);
  sphereRadii.redim(numberOfSpheres);
  
  intArray  testInside(numberOfSpheres);
  IntegerArray inside(numberOfSpheres);
  //inside.resize(numberOfSpheres);
  int soFar;
  int toDo;
  int sphereIndex;
  int nJiggle;
  int nFail = 0;
  const int maxFail = 100 * numberOfSpheres;
  realArray oneSphere(1, 3);
  IntegerArray oneInside(1);
  real radius;
  const int jiggleFactor = 10;

  for (int ns = 0; ns < numberOfRadii; ns++) {

    // add spheres of this new radius:
    radius = sphereDist.sphereDistribution(ns, 0);
    real xFace = boundingBox(1,0) - radius;

    if (ns > 0) {

      // jiggle existing spheres

      nFail = 0;
      nJiggle = jiggleFactor * ts;
      soFar = 0;
      while ( soFar < nJiggle && nFail < maxFail) {

	sphereIndex = rng->RandomInteger(ts);
        real radiusOfJiggledSphere=sphereRadii(sphereIndex); // *wdh* 

	dr = radius * rng->RandomDouble() * radiusFraction;  // *wdh* what radius to use here?
	cosines = rng->RandomCosines();
	for (int c = 0; c < 3; c++) {
	  oneSphere(0, c) = keepSpheres(sphereIndex, c) +
	    dr * cosines[c];
	}

        real time0=getCPU();
	model.insideOrOutside(oneSphere, oneInside);
        timeForInsideOutside+=getCPU()-time0;

	nFail++;
	if (oneInside(0) == 1) {
	  // *wdh* if( soFar>0 ) elementIndex=-1; // force a global search for the nearest boundary element
          elementIndex=-1; // force a global search for the nearest boundary element
       
	  //xP=oneSphere;
	  for (int c = 0; c < 3; c++) {
	    xP(0,c) = oneSphere(0,c);
	  }

          time0=getCPU();
	  model.project( xP,mpParams );  // project xP to the closet point on the surface
          timeForProject+=getCPU()-time0;

	  // compute the distance to the surface 
	  real dist = sqrt( SQR(xP(0,0)-oneSphere(0,0))+
			    SQR(xP(0,1)-oneSphere(0,1))+
			    SQR(xP(0,2)-oneSphere(0,2)) );
          // **** wdh the dot produect should not be used ****
	  // the dot-product will indicate whether we are inside or outside
	  // real dot =( (xP(0,0)-oneSphere(0,0))*surfaceNormal(0,0) +
	  //             (xP(0,1)-oneSphere(0,1))*surfaceNormal(0,1) +
	  //             (xP(0,2)-oneSphere(0,2))*surfaceNormal(0,2));
	  // dot = dot/max(REAL_MIN*100.,dist);
	  // if (dot >= 0.0 && dist <= radius && oneSphere(0,0) <= xFace) {
	  if ( dist >= radiusOfJiggledSphere ) {
	    // check for overlap with previous spheres
	    if (!isOverlapping(oneSphere, sphereRadii(sphereIndex),
			       sphereIndex,
			       keepSpheres, sphereRadii, ts)) {
	      for (int c = 0; c < 3; c++) {
		keepSpheres(sphereIndex, c) = oneSphere(0, c);
	      }
	      soFar++;
	      nFail--;
	    }
	  }
	}
      }
    }
    if (nFail == maxFail) {
      printf("ERROR - too many jiggle failures\n");
      continue;
    }

    soFar = 0;
    toDo = numberOfParticles(ns);

    real xab[2][3];
    xab[0][0]=boundingBox(0,0)+radius; xab[1][0]=boundingBox(1,0)-radius;
    xab[0][1]=boundingBox(0,1)+radius; xab[1][1]=boundingBox(1,1)-radius;
    xab[0][2]=boundingBox(0,2)+radius; xab[1][2]=boundingBox(1,2)-radius;
                     //
    while (soFar < numberOfParticles(ns)) {
      while (toDo > 0) {
	testSpheres.resize(toDo, 3);
	testInside.resize(toDo);
	// RNG coordinates
	for (int nts = 0; nts < toDo; nts++) {
	  for (int c = 0; c < 3; c++) {
	    testSpheres(nts, c) = xab[0][c] + (xab[1][c]-xab[0][c]) * rng->RandomDouble();
	  }
	}

	// find which spheres are inside by ray-tracing
        real time0=getCPU();
	model.insideOrOutside(testSpheres,testInside);
        timeForInsideOutside+=getCPU()-time0;


	for (int nts = 0; nts < toDo; nts++) {
	  if (testInside(nts) == 1) {
	    for (int c = 0; c < 3; c++) {
	      //x(0, c) = testSpheres(nts, c);
	      xP(0, c) = testSpheres(nts, c);
	    }
	    // *wdh* if( nts>0 ) elementIndex=-1; // force a global search for the nearest boundary element
            elementIndex=-1; // force a global search for the nearest boundary element
       
	    //xP=x;
	    time0=getCPU();
	    model.project( xP,mpParams );  // project xP to the closet point on the surface
	    timeForProject+=getCPU()-time0;

	    // compute the distance to the surface 
	    real dist = sqrt( SQR(xP(0,0)-testSpheres(nts,0))+
			      SQR(xP(0,1)-testSpheres(nts,1))+
			      SQR(xP(0,2)-testSpheres(nts,2)) );

            // ***wdh* the dot product is not safe to check at corners ****
	    // the dot-product will indicate whether we are inside or outside
//  	    real dot =( (xP(0,0)-x(0,0))*surfaceNormal(0,0) +
//  			(xP(0,1)-x(0,1))*surfaceNormal(0,1) +
//  			(xP(0,2)-x(0,2))*surfaceNormal(0,2));
//  	    dot = dot/max(REAL_MIN*100.,dist);
//  	    if( false && dot<0. )
//  	    {
//  	      printf("WARNING: There is a discrepancy between insideOustide and the dot-product "
//  		     " testInside(nts)=%i, dot=%8.2e\n"
//  		     ">>> point=(%8.2e,%8.2e,%8.2e), projected point=(%8.2e,%8.2e,%8.2e), \n"
//  		     "       surfaceNormal=(%8.2e,%8.2e,%8.2e), dist=%8.2e, dot=%8.2e subSurface=%i \n",
//  		     testInside(nts),dot, x(0,0),x(0,1),x(0,2),xP(0,0),xP(0,1),xP(0,2),
//  		     surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2),
//  		     dist,dot, subSurfaceIndex(0));
//  	    }

            
	    // *wdh* if (dot >= 0.0 && dist <= radius && testSpheres(nts,0) <= xFace) { 
	    if ( dist >= radius ) { 
	      // check for overlap with previous spheres
	      for (int c = 0; c < 3; c++) {
		oneSphere(0, c) = testSpheres(nts, c);
	      }
	      if (!isOverlapping(oneSphere, radius, 
				 numberOfKeptSpheres,
				 keepSpheres, 
				 sphereRadii, 
				 numberOfKeptSpheres)) {
		for (int c = 0; c < 3; c++) {
		  keepSpheres(numberOfKeptSpheres, c) = 
		    testSpheres(nts, c);
		}
		sphereRadii(numberOfKeptSpheres) = radius;
		numberOfKeptSpheres++;
		soFar++;

		if( numberOfKeptSpheres<200 )
		{
                  if( debug>0 )
		  printf("\n >>> point=(%8.2e,%8.2e,%8.2e), projected point=(%8.2e,%8.2e,%8.2e), \n"
			 "       surfaceNormal=(%8.2e,%8.2e,%8.2e), dist=%8.2e, subSurface=%i \n",
			 testSpheres(nts,0),testSpheres(nts,1),testSpheres(nts,2),xP(0,0),xP(0,1),xP(0,2),
			 surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2),
			 dist, subSurfaceIndex(0));


		  printf(" ***Keep sphere %3i, center (%9.2e,%9.2e,%9.2e), radius=%8.2e (dist to surface=%8.2e)\n",
		      numberOfKeptSpheres-1,testSpheres(nts,0),testSpheres(nts,1),testSpheres(nts,2),radius,dist);     
		}
	      }
	    }
	  }
	}
	toDo = numberOfParticles(ns) - soFar;
      }
    }
    ts += numberOfParticles(ns);
  }

  // This next function will resize the array spheres(==sphereCenter), sphereRadii (==sphereRadius)
  sphereDist.resize(numberOfSpheres);
  Range I=numberOfSpheres, R3=3;
  spheres(I,R3)=keepSpheres(I,R3);

//   spheres.resize(numberOfSpheres,3);

// *wdh* ---- this next section has been moved to linerGeometry.C

//    real m = boundingBox(1,1) / (boundingBox(1,0) - boundingBox(0,0));

//    for (int s = 0; s < numberOfSpheres; s++) {
//      sphereDist.sphereRadius(s) = sphereRadii(s);

//      /*
//        we need to rotate axis since the assumption is:
//        Cone opens right to left with face at z=0 and axis of rotation along z.
//        Radius below is outer radius of cone, Height is tip-face distance. 
//        M  := Radius/Height > 0 convergent on z axis.
//        Radius/Height = 0 Parrallel to z axis.
//        Radius/Height < 0 Divergent from z axis.
          
//        Rotational symmetry about z and motion from right to left assumed.
//      */

//      sphereDist.sphereCenter(s,0) = keepSpheres(s,1);
//      sphereDist.sphereCenter(s,1) = keepSpheres(s,2);
//      sphereDist.sphereCenter(s,2) = keepSpheres(s,0);
//      for (int c = 0; c < 3; c++) {
//        spheres(s,c)=keepSpheres(s,c);
//      }

//      if (m == 0.0) {
//        sphereDist.sphereVelocity(s,0) = 0.0;
//        sphereDist.sphereVelocity(s,1) = 0.0;
//        sphereDist.sphereVelocity(s,2) = 1.0;
//      }
//      else {
//        real nrm = -dsign(1.0, m) / sqrt(1.0 + m*m);
//        sphereDist.sphereVelocity(s,2) = -m * nrm;

//        real r2_2D = 
//  	sphereDist.sphereCenter(s,0)*sphereDist.sphereCenter(s,0) +
//  	sphereDist.sphereCenter(s,1)*sphereDist.sphereCenter(s,1);
//        if (r2_2D > 0.0) {
//  	nrm /= sqrt(r2_2D);
//  	sphereDist.sphereVelocity(s,0) = 
//  	  sphereDist.sphereCenter(s,0) * nrm;
//  	sphereDist.sphereVelocity(s,1) = 
//  	  sphereDist.sphereCenter(s,1) * nrm;
//        }
//        else {
//  	sphereDist.sphereVelocity(s,0) = 0.0;
//  	sphereDist.sphereVelocity(s,1) = 0.0;
//  	sphereDist.sphereVelocity(s,2) = dsign(1.0, m);
//        }
//      }
//      sphereDist.sphereStartTime(s) =  
//        sqrt( sphereDist.sphereCenter(s,2) );
//    }

  real totalTime=getCPU()-timea;

  printf(" =========================== Timings=====================================\n"
         "Total time for filling spheres........................%8.2e (%5.2f%%)\n"
         "Time for inside/outside...............................%8.2e (%5.2f%%)\n"
         "Time for project......................................%8.2e (%5.2f%%)\n"
         "Time for isOverlapping................................%8.2e (%5.2f%%)\n"
         "=========================================================================\n",
	 totalTime,100.*(totalTime/totalTime),
         timeForProject,100.*(timeForProject/totalTime),
         timeForInsideOutside,100.*(timeForInsideOutside/totalTime),
         timeForIsOverlapping,100.*(timeForIsOverlapping/totalTime));
  
     
  printf(" -----------------------------------------------------------------------------\n"
	 " *** There are %i spheres inside the volume, the liner is %4.1f %% filled *** \n",
	 numberOfSpheres, 100.*tv/max(REAL_MIN, volume));
  for (int c = 0; c < sphereDist.sphereDistribution.getLength(0); c++) {
    printf("   radius - %13.6lg, #particles - %6d, proportion - %lg %%\n", 
	   sphereDist.sphereDistribution(c,0), numberOfParticles(c),
	   100.*numberOfParticles(c)/numberOfSpheres);
  }
  printf(" -----------------------------------------------------------------------------\n");

  #undef boundingBox

  #endif
  return 0;
  
}




int
fillVolumeWithUniformlySpacedSpheres(CompositeSurface & model, GenericGraphicsInterface& gi, PointList & points,
				     SphereLoading & sphereLoading, real & volume,
                                     int & numberOfSpheres, real & sphereRadius, int debug)
// =========================================================================
//  This version fills the volume with a set of evenly spaced spheres 
// /Who to blame: WDH
// =========================================================================
{
  #ifndef USE_PPP
  // get bounds on the model
  real boundingBoxp[6];
  #define boundingBox(side,axis) boundingBoxp[(side)+2*(axis)]
  real volumeOfBoundingBox=1.;
  int axis;
  for( axis=0; axis<model.getRangeDimension(); axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      boundingBox(side,axis)=(real)model.getRangeBound(side,axis);
    }
    volumeOfBoundingBox*= (boundingBox(1,axis)-boundingBox(0,axis));
  }

  // estimate the number of spheres needed
  real volumeOfSphere = 4./3.*(Pi)*pow(sphereRadius,3.);
  numberOfSpheres = max(1,int(4.*volumeOfBoundingBox/volumeOfSphere+.5));
     
  RealArray & spheres = sphereLoading.sphereCenter;
  
  spheres.redim(numberOfSpheres,3);
  sphereLoading.sphereRadius.redim(numberOfSpheres);
  sphereLoading.sphereRadius=sphereRadius; // for now all spheres have the same radius

  real xc[3];
  for( axis=0; axis<3; axis++ )
  {
    xc[axis]=boundingBox(0,axis); // position of the first sphere
  }
  int i;
  for( i=0; i<numberOfSpheres; i++ )
  {
    spheres(i,0)=xc[0];
    spheres(i,1)=xc[1];
    spheres(i,2)=xc[2];
       
    // determine the center of the next sphere to check
    xc[0]+=2.*sphereRadius;
    if( xc[0]>boundingBox(1,0) )
    {
      xc[0]=boundingBox(0,0);
      xc[1]+=2.*sphereRadius;
      if( xc[1]>boundingBox(1,1) )
      {
	xc[1]=boundingBox(0,1);
	xc[2]+=2.*sphereRadius;
	if( xc[2]>boundingBox(1,2) )
	{
	  numberOfSpheres=i+1;
	  break;
	}
      }
    }
  }

  spheres.resize(numberOfSpheres,3);

  IntegerArray inside(numberOfSpheres);

  // find which spheres are inside by ray-tracing
  model.insideOrOutside(spheres,inside);
           

  // This next object is used to pass and return additional parameters to the "project" function.
  MappingProjectionParameters mpParams;
  typedef MappingProjectionParameters MPP;
  realArray & surfaceNormal  = mpParams.getRealArray(MPP::normal);
  intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  realArray & xOld           = mpParams.getRealArray(MPP::x);  // this could be used as an initial guess
  intArray & elementIndex    = mpParams.getIntArray(MPP::elementIndex);
     
     // Allocate space for the normal to indicate that it should be computed
  int numberOfPointsToProject=1;  // we only project 1 point at a time here
  surfaceNormal.redim(numberOfPointsToProject,3);
  surfaceNormal=0.;       
     
//          IntegerArray inside(1); inside=-1;
//  	cs.insideOrOutside(x,inside);

  realArray x(1,3),xP(1,3); // arrays to hold initial and projected points
  x=0.;

  // attempt to fill spheres
  real timea=getCPU();
  int j=0;
  for( i=0; i<numberOfSpheres; i++ )
  {
       
    if( inside(i)==1 ) // dot>0. 
    {
      // *** the center of this sphere is inside ***

      x(0,0)=spheres(i,0);
      x(0,1)=spheres(i,1);
      x(0,2)=spheres(i,2);
       
      if( i>0 ) elementIndex=-1; // force a global search for the nearest boundary element
       
      xP=x;
      model.project( xP,mpParams );  // project xP to the closet point on the surface

      // compute the distance to the surface 
      real dist = sqrt( SQR(xP(0,0)-x(0,0))+SQR(xP(0,1)-x(0,1))+SQR(xP(0,2)-x(0,2)) );
      // the dot-product will indicate whether we are inside or outside
      real dot =( (xP(0,0)-x(0,0))*surfaceNormal(0,0) +
		  (xP(0,1)-x(0,1))*surfaceNormal(0,1) +
		  (xP(0,2)-x(0,2))*surfaceNormal(0,2));
      dot = dot/max(REAL_MIN*100.,dist);
       
      if( numberOfSpheres<100 )
      {
	printf("\n >>> point=(%8.2e,%8.2e,%8.2e), projected point=(%8.2e,%8.2e,%8.2e), \n"
	       "       surfaceNormal=(%8.2e,%8.2e,%8.2e), dist=%8.2e, dot=%8.2e subSurface=%i \n",
	       x(0,0),x(0,1),x(0,2),xP(0,0),xP(0,1),xP(0,2),
	       surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2),
	       dist,dot, subSurfaceIndex(0));


	printf(" ***Keep the sphere with center (%8.2e,%8.2e,%8.2e) \n",x(0,0),x(0,1),x(0,2));
      }

      if( dist < sphereRadius )
      {
	// skip this sphere since it is outside.
	if( debug & 4 )
	{
	  printf(" >>> Sphere intersects the surface, sphereRadius/2=%8.2e, dist=%8.2e\n"
		 "       point=(%8.2e,%8.2e,%8.2e), projected point=(%8.2e,%8.2e,%8.2e), \n"
		 "       surfaceNormal=(%8.2e,%8.2e,%8.2e), dist=%8.2e, dot=%8.2e subSurface=%i \n",
		 sphereRadius/2.,dist,x(0,0),x(0,1),x(0,2),xP(0,0),xP(0,1),xP(0,2),
		 surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2),
		 dist,dot, subSurfaceIndex(0));
	}
	   
	continue;
      }
	 
      spheres(j,0)=spheres(i,0);   
      spheres(j,1)=spheres(i,1); 
      spheres(j,2)=spheres(i,2);
      j++;

      if( false && dot<0. )
      {
	printf("WARNING: There is a discrepancy between insideOustide and the dot-product "
	       " insideOutside=%i, dot=%8.2e\n"
	       ">>> point=(%8.2e,%8.2e,%8.2e), projected point=(%8.2e,%8.2e,%8.2e), \n"
	       "       surfaceNormal=(%8.2e,%8.2e,%8.2e), dist=%8.2e, dot=%8.2e subSurface=%i \n",
	       inside(i),dot, x(0,0),x(0,1),x(0,2),xP(0,0),xP(0,1),xP(0,2),
	       surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2),
	       dist,dot, subSurfaceIndex(0));
      }

    }
       
  } // end for i
  numberOfSpheres=j;
  spheres.resize(numberOfSpheres,3);
  timea=getCPU()-timea;
  printf("Time for computing nearest surface point for each sphere = %8.2e \n",timea);

  printf(" -----------------------------------------------------------------------------\n"
	 " *** There are %i spheres inside the volume, the liner is %4.1f %% filled *** \n"
	 " -----------------------------------------------------------------------------\n",
	 numberOfSpheres,100.*numberOfSpheres*volumeOfSphere/max(REAL_MIN,volume));
     
#undef boundingBox

  #endif  
  return 0;
}
