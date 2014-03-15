#ifndef Interpolate_h
#define Interpolate_h

#include <Overture.h>
#include <NameList.h>
//#include <AMR++.h>

#include "InterpolateParameters.h"

//extern InterpolateParameters defaultInterpolateParameters;

class Interpolate
{
 public:

  bool debug;
  bool timing;

  enum InterpolateOptionEnum
  {
    injection=0,
    fullWeighting100,  // full weighting along the x-axis
    fullWeighting010,  // full weighting along the y-axis
    fullWeighting001,  // full weighting along the z-axis
    fullWeighting110,  // full weighting in the x-y plane
    fullWeighting101,  // full weighting in the x-z plane
    fullWeighting011,  // full weighting in the y-z plane
    fullWeighting111   // full weighting in 3D
  };

  enum GeneralNamesEnum
  {
    useDefaultTransferWidth=-12345
  };


  enum MaskOptionEnum
  {
    doNotUseMask=0,
    maskGreaterThanZero=1,
    maskEqualZero=2
  };



  Interpolate();

  Interpolate (const InterpolateParameters& interpParams_, 
	       const bool timing=LogicalFalse);
  
  ~Interpolate();

  int initialize (const InterpolateParameters& interpParams_,
		  const bool timing=LogicalFalse);
		  

  int interpolateCoarseToFine (realArray&                   fineGridArray,
			       const Index                  Iv[3],
			       const realArray&             coarseGridArray,
			       const IntegerArray&          amrRefinementRatio_=Overture::nullIntArray());

    
  int interpolateFineToCoarse (realArray&                   coarseGridArray,
			       const Index                  Iv[3],
			       const realArray&             fineGridArray,
			       const IntegerArray&          amrRefinementRatio_=Overture::nullIntArray());
  

  // ******** here are the new versions using optimised functions ******************
  // interpolate fine from coarse
  int interpolateFineFromCoarse(realArray&                   fineGridArray,
				const Index                  Iv[3],
				const realArray&             coarseGridArray,
				const IntegerArray&          amrRefinementRatio_=Overture::nullIntArray(),
				const int update=0,
                                const int transferWidth=useDefaultTransferWidth );

  // interpolate fine from coarse where mask > 0
  int interpolateFineFromCoarse(realArray&                   fineGridArray,
				const intSerialArray&        mask,  
				const Index                  Iv[3],
				const realArray&             coarseGridArray,
				const IntegerArray&          amrRefinementRatio_=Overture::nullIntArray(),
				const int update=0,
                                const int transferWidth=useDefaultTransferWidth,
                                const MaskOptionEnum maskOption=maskGreaterThanZero );

  // interpolate coarse from fine
  int interpolateCoarseFromFine(realArray&                   coarseGridArray,
				const Index                  Iv[3],
				const realArray&             fineGridArray,
				const IntegerArray&          amrRefinementRatio_=Overture::nullIntArray(),
				const InterpolateOptionEnum  interpOption=injection,
				const int update=0,
                                const int transferWidth=useDefaultTransferWidth );

  // interpolate coarse from fine where mask > 0
  int interpolateCoarseFromFine(realArray&                   coarseGridArray,
				const intSerialArray&        mask,
				const Index                  Iv[3],
				const realArray&             fineGridArray,
				const IntegerArray&          amrRefinementRatio_=Overture::nullIntArray(),
				const InterpolateOptionEnum  interpOption=injection,
				const int update=0,
                                const int transferWidth=useDefaultTransferWidth,
                                const MaskOptionEnum maskOption=maskGreaterThanZero );

 protected:


  RealArray coeff;  //interpolation coefficients
  int interpolateOrder;
  int numberOfDimensions;
  bool preComputeAllCoefficients;
  bool useGeneralInterpolationFormula;

  IntegerArray amrRefinementRatio;
  GridFunctionParameters::GridFunctionType gridCentering;
  InterpolateParameters::InterpolateType interpolateType;

  int initializeCoefficients (const int maxRefinementRatio_, 
			      const int interpolateOrder_, 
			      const int numberOfDimensions_,
			      const int *interpolateOffset_);

  int computeIndexes (const realArray& uc,
		      int* lm, Index* Jf, Index* Jc, const Index* If,
		      const int* lmr, const int R, const int* A,
		      const int* r, const int* stride, const int* extra, const int* offset,
		      InterpolateParameters::InterpolateOffsetDirection* iod);

  int displayEverything (const int* r, const int* extra, const int* offset, const int* lm, const int* lmr,
			 const Index* Iv, const Index* If, const Index* Jf, const Index* Jc);
  
};

  
#endif
