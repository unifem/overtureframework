#include "MappedGridFiniteVolumeOperators.h"
#include "billsMergeMacro.h"
#include "laplacian.h"
#include <xDC.h>

#define LAP_MERGED(m1,m2,m3,c,e,I1,I2,I3) MERGE0(returnedValue,M123CE(m1,m2,m3,c,e),I1,I2,I3)

REALMappedGridFunction MappedGridFiniteVolumeOperators::
identityOperator (const Index & I1, const Index & I2, const Index & I3){
  return (identityCoefficients(I1,I2,I3));
}
 
// ================================================================================
//\begin{>Laplacian.tex}{\subsection{identityCoefficients}}   
REALMappedGridFunction MappedGridFiniteVolumeOperators::
identityCoefficients(const Index & I1,   // = nullIndex
		     const Index & I2,   // = nullIndex
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     const Index & I6,
		     const Index & I7,
		     const Index & I8)    // = nullIndex
//
// /Purpose:
//  implements the identity operator (implicit) for this class
//
// /I1,I2,I3: if these are not defaulted, the identity operator coefficients
//             are only returned for cells specified by these Index'es
// /E:      specifies the (subset of) equations for which the identityCoefficients
//          are to be computed
// /C:      specifies the (subset of) components for which the identityCoefficients
//          are to be computed.
// 
//
// /Author:				D.L.Brown
//\end{Laplacian.tex}  
  // 960802: changes made for E,C
  //========================================
{
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "identityCoefficients: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "identityCoefficients: FATAL ERROR";
  }

  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  REALMappedGridFunction returnedValue(mappedGrid, stencilDimension, all, all, all);;
  returnedValue = 0.;
  
  int c0 = C.getBase();
  int e0 = E.getBase();
    
  Range aR0, aR1, aR2, aR3;
  int dum; 

  Index I,J,K;
  int extra = -max(mappedGrid.numberOfGhostPoints());  
  
  int cmp[5] = {0,0,0,0,0};
//  getDefaultIndex (returnedValue, cmp[0], cmp[1], cmp[2], cmp[3], cmp[4], I, J, K, extra, extra, extra, I1, I2, I3);
//981229: don't assume equal number of ghost points in each direction

  int extra0 = -mappedGrid.numberOfGhostPoints(Start,axis1);
  int extra1 = -mappedGrid.numberOfGhostPoints(Start,axis2);
  int extra2 = -mappedGrid.numberOfGhostPoints(Start,axis3);

  getDefaultIndex (returnedValue, cmp[0], cmp[1], cmp[2], cmp[3], cmp[4], I, J, K, 
		   extra0, extra1, extra2, I1, I2, I3);

  // ... identity: set central coeff = 1
  LAP_MERGED (0,0,0,c0,e0,I,J,K) = 1.;

  // ... now copy to all the rest of the Components and Equations
  Index M(0,stencilSize);
  Index M0(CE(c0,e0),stencilSize);

  for (int c=C.getBase(); c<=C.getBound(); c++)
    for (int e=E.getBase(); e<=E.getBound(); e++)
      if ( c!=c0 || e!=e0 )
	returnedValue(M+CE(c,e),I,J,K) = returnedValue(M0+CE(c,e),I,J,K);
  

/*  
  if (numberOfDimensions == 2) {
    
#undef RETURNED_VALUE
#define RETURNED_VALUE(l,m,I,J,K) MERGE0(returnedValue,coeff(l,m),I,J,K)
    
    RETURNED_VALUE(0,0,I,J,K) = 1.;
   
  } else { // if (numberOfDimensions == 3)

#undef RETURNED_VALUE
#define RETURNED_VALUE(l,m,n,I,J,K) MERGE0(returnedValue,coeff(l,m,n),I,J,K)

    RETURNED_VALUE(0,0,0,I,J,K) = 1.;

  }
*/

  return (returnedValue);
}

//==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators:: 
genericLaplacianCoefficientsForSystems(const LaplacianType lt,
				       const realMappedGridFunction & scalar,
				       const Index & I1,	// = nullIndex
				       const Index & I2,	// = nullIndex
				       const Index & I3,	// = nullIndex
				       const Index & E,       // = nullIndex
				       const Index & C       // = nullIndex
				       )
{
  
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "genericLaplacianCoefficientsForSystems: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "GeneralLaplacian: FATAL ERROR";
  }

  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction returnedValue (mappedGrid, stencilDimension, all, all, all);
  returnedValue = 0.;
  
//  int m1,m2,m3;
  Range aR0,aR1,aR2,aR3;
//  int dum;
  
  int c0 = C.getBase();
  int e0 = E.getBase();

  Index M(0,stencilSize);
  Index M0(CE(c0,e0),stencilSize);

  Index I,J,K;
  int extra = -max(mappedGrid.numberOfGhostPoints());

// ... this isn't going to work unless we first find the correct Index's to evaluate at
  int cmp[5] = {0,0,0,0,0};
//  getDefaultIndex (returnedValue, cmp[0], cmp[1], cmp[2], cmp[3], cmp[4], I, J, K, extra, extra, extra, I1, I2, I3);

//981229: don't assume equal number of ghost points in each direction
  int extra0 = -mappedGrid.numberOfGhostPoints(Start,axis1);
  int extra1 = -mappedGrid.numberOfGhostPoints(Start,axis2);
  int extra2 = -mappedGrid.numberOfGhostPoints(Start,axis3);

  getDefaultIndex (returnedValue, cmp[0], cmp[1], cmp[2], cmp[3], cmp[4], I, J, K, 
		   extra0, extra1, extra2, I1, I2, I3);


//
// ... evaluate the laplacianCoefficients and put them in the lowest
// ... (component,equation) pair
// 

//  returnedValue (M0, I, J, K) = GeneralLaplacian(lt, scalar, I, J, K)(M0, I, J, K);
//990104:try this
  returnedValue (M0, I, J, K) = GeneralLaplacian(lt, scalar, I, J, K)(M, I, J, K);
  
//
// ... if more than one (component,equation) pair are being set, copy the
// ... coefficients to those locations
//

  for (int c=C.getBase(); c<=C.getBound(); c++)
    for (int e=E.getBase(); e<=E.getBound(); e++)
      if ( c!=c0 || e!=e0 )
	returnedValue(M+CE(c,e),I,J,K) = returnedValue(M+CE(c0,e0),I,J,K);

  return (returnedValue);
}

// ================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
GeneralLaplacian(                             				// laplacian operator
		 const LaplacianType laplacianType,
		 const REALMappedGridFunction & scalar,
		 const Index & I1,
		 const Index & I2,
		 const Index & I3
                        )
// ================================================================================
	//========================================
	// Author:		D.L.Brown
	// Date Created:	950509
	// Date Modified:	950712
	//
	// Purpose:
	// private function
	// compute Laplacian or other elliptic operator coefficients for a finite-volume grid
	// In this version, the coefficients are indexed in the FIRST (0th) index of the array
	//
	// Interface: (inputs)
	//
	//   LaplacianType laplacianType:	one of the following:
	//    enum LaplacianType
	//    {
	//      constCoeff,					regular const coeff Laplacian
	//      inverseScalarVariableCoefficients,		div(1/s grad())
	//      variableCoefficients				dif(s grad())
	//    };
	//
	//   REALMappedGridFunction scalar	"scalar" array used in variable coefficient operators
	//   Index I1,I2,I3			if != nullIndex, the computation of the coefficients
	//					will be restricted to the ranges specified by I1,I2,I3
	//	
	// Interface: (output)
	//	returns 
	//	REALMappedGridFunction GeneralLaplacian   the coefficients for this MappedGrid 
	//						to be sent to an elliptic
	//						solver
	//
	// Status and Warnings:
	//  950712: doesnt currently do the multi-component case (i.e. Index N is ignored)
	// 
	//========================================

{

  if (debug) LaplacianDisplay.interactivelySetInteractiveDisplay ("Laplacian initialization");

	// ========================================
	// Initialize returnedValue
	// ========================================

  REALMappedGridFunction returnedValue;

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "GeneralLaplacian: FATAL ERROR";
  }

//int numberOfComponentsRV = (numberOfDimensions == 2) ? 10 : 28;  
  int numberOfComponentsRV = stencilSize;
//  cout << "GeneralLaplacian: stencilSize = " << stencilSize << endl;
  
  
//  int positionOfComponentRV  = 0;
//returnedValue.updateToMatchGrid (mappedGrid, numberOfComponentsRV, positionOfComponentRV);

  returnedValue.updateToMatchGrid (mappedGrid, numberOfComponentsRV, all, all, all);
  returnedValue = 0.;

	// ========================================
	// These are required for the MERGE0 macro
	// ========================================

  Range aR0,aR1,aR2,aR3;
  int dum;


  Index I,J,K;
  int extra = -max(mappedGrid.numberOfGhostPoints());

  int cmp[5] = {0,0,0,0,0};
//getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], I, J, K, extra, extra, extra, I1, I2, I3);

//981229: don't assume equal number of ghost points in each direction
  int extra0 = -mappedGrid.numberOfGhostPoints(Start,axis1);
  int extra1 = -mappedGrid.numberOfGhostPoints(Start,axis2);
  int extra2 = -mappedGrid.numberOfGhostPoints(Start,axis3);

  getDefaultIndex (returnedValue, cmp[0], cmp[1], cmp[2], cmp[3], cmp[4], I, J, K, 
		   extra0, extra1, extra2, I1, I2, I3);



  Range ND;
  ND = Range(0,numberOfDimensions-1);

  // ========================================
  //		            t
  // L = div . T ( (1/vol) T  grad ())
  //				 t
  // or  div . T (( scalar/vol) T grad ())
  //				 t
  // or div . T ((1/vol/scalar) T Grad ())
  //
  // ========================================

  // ========================================
  //  Massage the scalar into the right place
  // for the appropriate version of the Laplacian.
  //
  // For laplacianType = constCoeff, the scalar is ignored
  // For laplacianType = scalar, the scalar and cellVolume
  //		are averaged to faces and then divided
  // For laplacianType = inverseScalar, the scalar and cellVolume
  //		are both averaged to faces, multiplied and then
  //		inverted.
  // ========================================

  REALMappedGridFunction faceScalar;;
//  faceScalar.updateToMatchGridFunction(cellVolume, all, all, all, numberOfDimensions);
  faceScalar.updateToMatchGrid (mappedGrid, all, all, all, numberOfDimensions);
  faceScalar.setIsFaceCentered();
  faceScalar = 0.;

  Index If,Jf,Kf;
  int dimension;
  int extraf = max(mappedGrid.numberOfGhostPoints())-1;

  int extraf0 = mappedGrid.numberOfGhostPoints(Start,axis1)-1;
  int extraf1 = mappedGrid.numberOfGhostPoints(Start,axis2)-1;
  int extraf2 = mappedGrid.numberOfGhostPoints(Start,axis3)-1;



// ========================================
// Constant coefficient Laplacian
// ========================================
  if (laplacianType == constCoeff)

  {
    faceScalar = cellsToFaces (cellVolume);
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
//      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
//981229: don't assume same number of ghost points in each direction
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf0, extraf1, extraf2);
      faceScalar(If,Jf,Kf,dimension) = 1./faceScalar(If,Jf,Kf,dimension);
    }
  if (debug) LaplacianDisplay.display (faceScalar, "Laplacian: faceScalar constCoeff:");
  }
// ========================================
// Inverse Variable Coefficient Laplacian
// ========================================

  if (laplacianType == inverseScalarVariableCoefficients)
  {
    // 950628 change scalar averaging to be arithmetic.

    if (debug) LaplacianDisplay.display (scalar, "Laplacian: input scalar inverseScalarVariableCoefficients");

    REALMappedGridFunction scaledValue;
    scaledValue.updateToMatchGrid (mappedGrid);
    scaledValue.setIsCellCentered(TRUE);
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      int extra = 1;
      getIndex (mappedGrid.indexRange(),  If, Jf, Kf, extra);
      scaledValue(If,Jf,Kf) = 1./scalar(If,Jf,Kf);
    }
    faceScalar = cellsToFaces (scaledValue);

    REALMappedGridFunction faceCellVolume;
    faceCellVolume.updateToMatchGrid (mappedGrid, all, all, all, numberOfDimensions);
    faceCellVolume = cellsToFaces(cellVolume);
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) /= faceCellVolume(If,Jf,Kf,dimension);
    }
  if (debug) LaplacianDisplay.display (faceScalar, "Laplacian: faceScalar inverseScalarVariableCoefficients:");
  }
// ========================================
// Variable Coefficient Laplacian
// ========================================

  if (laplacianType == variableCoefficients)
  {
  if (debug) LaplacianDisplay.display (scalar, "Laplacian: input scalar variableCoefficients");
    faceScalar = cellsToFaces(scalar);
    REALMappedGridFunction faceCellVolume;
    faceCellVolume.updateToMatchGrid (mappedGrid,all, all, all, numberOfDimensions); 
    faceCellVolume = cellsToFaces(cellVolume);

  if (debug) LaplacianDisplay.display (faceCellVolume, "Laplacian: faceCellVolume variableCoefficients:");
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) /= faceCellVolume(If,Jf,Kf,dimension);
    }
  if (debug) LaplacianDisplay.display (faceScalar, "Laplacian: faceScalar variableCoeffients:");
  }

  if (numberOfDimensions == 2) {
    
#undef RETURNED_VALUE
#define RETURNED_VALUE(l,m,I,J,K) MERGE0(returnedValue,coeff(l,m),I,J,K)

    int i0, i1, j0, j1;

    if (useCMPGRDGeometryArrays) {

      if (debug) cout << "XXXXX Laplacian: using CMPGRD geometry arrays..." << endl;

    ForDplus(i0){ ForDplus(j0) { ForDminus(i1) { ForDminus(j1) {

      int i = i0+i1;
      int j = j0+j1;

      RETURNED_VALUE(i,j,I,J,K) +=
	Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(


	  (SQR(rX(I+i0,J,K)) + SQR(rY(I+i0,J,K)))
	  * Delta(j0,0)
	  * Dminus(i1)
	  * Delta(j1,0)


	  + (rX(I+i0,J,K) * RAverage(i1,j0)*sX(I+i0+i1,J+j0,K) +  rY(I+i0,J,K) * RAverage(i1,j0)*sY(I+i0+i1,J+j0,K))
	  * Dminus(j1)
	   );


    }}}}

    if (debug) {
      LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 1");
      LaplacianDisplay.display (rX, "Laplacian: rX");
      LaplacianDisplay.display (sX, "Laplacian: sX");
      LaplacianDisplay.display (rY, "Laplacian: rY");
      LaplacianDisplay.display (sY, "Laplacian: sY");
    }
    
      

    ForDplus(j0) { ForDminus(j1) { ForDplus (i0) { ForDminus (i1) {

      int i = i0+i1;
      int j = j0+j1;
      RETURNED_VALUE(i,j,I,J,K) +=
	Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(


	  (SQR(sX(I,J+j0,K)) + SQR(sY(I,J+j0,K)))
	  * Delta(i0,0)
	  * Dminus(j1)
	  * Delta(i1,0)


	  +(sX(I,J+j0,K) * SAverage(i0,j1)*rX(I+i0,J+j0+j1,K) + sY(I,J+j0,K) * SAverage(i0,j1)*rY(I+i0,J+j0+j1,K))
	  * Dminus(i1)
	   );

    }}}}

    if (debug)       LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 2");

    }else{

    ForDplus(i0){ ForDplus(j0) { ForDminus(i1) { ForDminus(j1) {

      int i = i0+i1;
      int j = j0+j1;

      RETURNED_VALUE(i,j,I,J,K) +=
	Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(


	  (SQR(Rx(I+i0,J,K)) + SQR(Ry(I+i0,J,K)))
	  * Delta(j0,0)
	  * Dminus(i1)
	  * Delta(j1,0)


	  + (Rx(I+i0,J,K) * RAverage(i1,j0)*Sx(I+i0+i1,J+j0,K) +  Ry(I+i0,J,K) * RAverage(i1,j0)*Sy(I+i0+i1,J+j0,K))
	  * Dminus(j1)
	   );

    }}}}

    if (debug)    LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 1");

    ForDplus(j0) { ForDminus(j1) { ForDplus (i0) { ForDminus (i1) {

      int i = i0+i1;
      int j = j0+j1;
      RETURNED_VALUE(i,j,I,J,K) +=
	Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(


	  (SQR(Sx(I,J+j0,K)) + SQR(Sy(I,J+j0,K)))
	  * Delta(i0,0)
	  * Dminus(j1)
	  * Delta(i1,0)


	  +(Sx(I,J+j0,K) * SAverage(i0,j1)*Rx(I+i0,J+j0+j1,K) + Sy(I,J+j0,K) * SAverage(i0,j1)*Ry(I+i0,J+j0+j1,K))
	  * Dminus(i1)
	   );

    }}}}

    if (debug)    LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 2");
  }

  // ========================================
  // Optionally scale the coefficients by inverse volume
  // ========================================

    if (debug) 
      LaplacianDisplay.display (cellVolume, "cellVolume in GeneralLaplacian");
    // int component;
    if (!isVolumeScaled){

      for (int j=-1; j<2; j++){
	for (int i=-1; i<2; i++){
	  RETURNED_VALUE(i,j,I,J,K) /= cellVolume(I,J,K);
	}
      }

    if (debug)
      LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 3");

    }

  } else { // if (numberOfDimensions == 3)

#undef RETURNED_VALUE
#define RETURNED_VALUE(l,m,n,I,J,K) MERGE0(returnedValue,coeff(l,m,n),I,J,K)

    int i0, i1, j0, j1, k0, k1;

   if (useCMPGRDGeometryArrays) 
   {
    ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){
      
      int i = i0+i1;
      int j = j0+j1;
      int k = k0+k1;
      RETURNED_VALUE(i,j,k,I,J,K) +=
	
	Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(

	  (SQR(rX(I+i0,J,K)) + SQR(rY(I+i0,J,K)) + SQR(rZ(I+i0,J,K)))*Dminus(i1)
	* Delta(j0,0)
	* Delta(j1,0)
	* Delta(k0,0)
	* Delta(k1,0)

	+ (rX(I+i0,J,K)*SRAverage(i1,j0,k0)*sX(I+i0+i1,J+j0,K+k0)
	 + rY(I+i0,J,K)*SRAverage(i1,j0,k0)*sY(I+i0+i1,J+j0,K+k0)
	 + rZ(I+i0,J,K)*SRAverage(i1,j0,k0)*sZ(I+i0+i1,J+j0,K+k0))
	* Delta(k1,0)
	* Dminus(j1)

	+ (rX(I+i0,J,K)*TRAverage(i1,j0,k0)*tX(I+i0+i1,J+j0,K+k0)
	 + rY(I+i0,J,K)*TRAverage(i1,j0,k0)*tY(I+i0+i1,J+j0,K+k0)
	 + rZ(I+i0,J,K)*TRAverage(i1,j0,k0)*tZ(I+i0+i1,J+j0,K+k0))
	* Delta(j1,0)
	* Dminus(k1)

	);

      }}}}}}

	 
     ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){
      
      int i = i0+i1;
      int j = j0+j1;
      int k = k0+k1;
      RETURNED_VALUE(i,j,k,I,J,K) +=

	Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(

	  (SQR(sX(I,J+j0,K)) + SQR(sY(I,J+j0,K)) + SQR(sZ(I,J+j0,K)))*Dminus(j1)
	* Delta(k0,0)
	* Delta(k1,0)
	* Delta(i0,0)
	* Delta(i1,0)

	+ (sX(I,J+j0,K)*RSAverage(i0,j1,k0)*rX(I+i0,J+j0+j1,K+k0)
	 + sY(I,J+j0,K)*RSAverage(i0,j1,k0)*rY(I+i0,J+j0+j1,K+k0)
	 + sZ(I,J+j0,K)*RSAverage(i0,j1,k0)*rZ(I+i0,J+j0+j1,K+k0))
	* Delta(k1,0)
	* Dminus(i1)

	+ (sX(I,J+j0,K)*TSAverage(i0,j1,k0)*rX(I+i0,J+j0+j1,K+k0)
	 + sY(I,J+j0,K)*TSAverage(i0,j1,k0)*rY(I+i0,J+j0+j1,K+k0)
	 + sZ(I,J+j0,K)*TSAverage(i0,j1,k0)*rZ(I+i0,J+j0+j1,K+k0))
	* Delta(i1,0)
	* Dminus(k1)

	);
      
      }}}}}}
	 
     ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){
      
      int i = i0+i1;
      int j = j0+j1;
      int k = k0+k1;
      RETURNED_VALUE(i,j,k,I,J,K) +=

	Dplus(k0)*faceScalar(I,J,K+k0,tAxis)*(

	  (SQR(tX(I,J,K+k0)) + SQR(tY(I,J,K+k0)) + SQR(tZ(I,J,K+k0)))*Dminus(k1)
	* Delta(i0,0)
	* Delta(i1,0)
	* Delta(j0,0)
	* Delta(j1,0)

        + (tX(I,J,K+k0)*RTAverage(i0,j0,k1)*rX(I+i0,J+j0,K+k0+k1)
         + tY(I,J,K+k0)*RTAverage(i0,j0,k1)*rY(I+i0,J+j0,K+k0+k1)
         + tZ(I,J,K+k0)*RTAverage(i0,j0,k1)*rZ(I+i0,J+j0,K+k0+k1))
	* Delta(j1,0)
	* Dminus(i1)

	+ (tX(I,J,K+k0)*STAverage(i0,j0,k1)*sX(I+i0,J+j0,K+k0+k1)
	 + tY(I,J,K+k0)*STAverage(i0,j0,k1)*sY(I+i0,J+j0,K+k0+k1)
	 + tZ(I,J,K+k0)*STAverage(i0,j0,k1)*sZ(I+i0,J+j0,K+k0+k1))
	* Delta(i1,0)
	* Dminus(j1)

	);

      }}}}}}

   } else {

    ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){
      
      int i = i0+i1;
      int j = j0+j1;
      int k = k0+k1;
      RETURNED_VALUE(i,j,k,I,J,K) +=
	
	Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(

	  (SQR(Rx(I+i0,J,K)) + SQR(Ry(I+i0,J,K)) + SQR(Rz(I+i0,J,K)))*Dminus(i1)
	* Delta(j0,0)
	* Delta(j1,0)
	* Delta(k0,0)
	* Delta(k1,0)

	+ (Rx(I+i0,J,K)*SRAverage(i1,j0,k0)*sX(I+i0+i1,J+j0,K+k0)
	 + Ry(I+i0,J,K)*SRAverage(i1,j0,k0)*Sy(I+i0+i1,J+j0,K+k0)
	 + Rz(I+i0,J,K)*SRAverage(i1,j0,k0)*Sz(I+i0+i1,J+j0,K+k0))
	* Delta(k1,0)
	* Dminus(j1)

	+ (Rx(I+i0,J,K)*TRAverage(i1,j0,k0)*tX(I+i0+i1,J+j0,K+k0)
	 + Ry(I+i0,J,K)*TRAverage(i1,j0,k0)*Ty(I+i0+i1,J+j0,K+k0)
	 + Rz(I+i0,J,K)*TRAverage(i1,j0,k0)*Tz(I+i0+i1,J+j0,K+k0))
	* Delta(j1,0)
	* Dminus(k1)

	);

      }}}}}}

	 
     ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){
      
      int i = i0+i1;
      int j = j0+j1;
      int k = k0+k1;
      RETURNED_VALUE(i,j,k,I,J,K) +=

	Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(

	  (SQR(Sx(I,J+j0,K)) + SQR(Sy(I,J+j0,K)) + SQR(Sz(I,J+j0,K)))*Dminus(j1)
	* Delta(k0,0)
	* Delta(k1,0)
	* Delta(i0,0)
	* Delta(i1,0)

	+ (Sx(I,J+j0,K)*RSAverage(i0,j1,k0)*Rx(I+i0,J+j0+j1,K+k0)
	 + Sy(I,J+j0,K)*RSAverage(i0,j1,k0)*Ry(I+i0,J+j0+j1,K+k0)
	 + Sz(I,J+j0,K)*RSAverage(i0,j1,k0)*Rz(I+i0,J+j0+j1,K+k0))
	* Delta(k1,0)
	* Dminus(i1)

	+ (Sx(I,J+j0,K)*TSAverage(i0,j1,k0)*Rx(I+i0,J+j0+j1,K+k0)
	 + Sy(I,J+j0,K)*TSAverage(i0,j1,k0)*Ry(I+i0,J+j0+j1,K+k0)
	 + Sz(I,J+j0,K)*TSAverage(i0,j1,k0)*Rz(I+i0,J+j0+j1,K+k0))
	* Delta(i1,0)
	* Dminus(k1)

	);
      
      }}}}}}
	 
     ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){
      
      int i = i0+i1;
      int j = j0+j1;
      int k = k0+k1;
      RETURNED_VALUE(i,j,k,I,J,K) +=

	Dplus(k0)*faceScalar(I,J,K+k0,tAxis)*(

	  (SQR(Tx(I,J,K+k0)) + SQR(Ty(I,J,K+k0)) + SQR(Tz(I,J,K+k0)))*Dminus(k1)
	* Delta(i0,0)
	* Delta(i1,0)
	* Delta(j0,0)
	* Delta(j1,0)

        + (Tx(I,J,K+k0)*RTAverage(i0,j0,k1)*Rx(I+i0,J+j0,K+k0+k1)
         + Ty(I,J,K+k0)*RTAverage(i0,j0,k1)*Ry(I+i0,J+j0,K+k0+k1)
         + Tz(I,J,K+k0)*RTAverage(i0,j0,k1)*Rz(I+i0,J+j0,K+k0+k1))
	* Delta(j1,0)
	* Dminus(i1)

	+ (Tx(I,J,K+k0)*STAverage(i0,j0,k1)*Sx(I+i0,J+j0,K+k0+k1)
	 + Ty(I,J,K+k0)*STAverage(i0,j0,k1)*Sy(I+i0,J+j0,K+k0+k1)
	 + Tz(I,J,K+k0)*STAverage(i0,j0,k1)*Sz(I+i0,J+j0,K+k0+k1))
	* Delta(i1,0)
	* Dminus(j1)

	);

      }}}}}}


   }

  // ========================================
  // Optionally scale the coefficients by inverse volume
  // ========================================

    if (debug) 
    LaplacianDisplay.display (cellVolume, "cellVolume in GeneralLaplacian");
    // int component;
    if (!isVolumeScaled){

      for (int k=-1; k<2; k++){
	for (int j=-1; j<2; j++){
	  for (int i=-1; i<2; i++){
	    RETURNED_VALUE(i,j,k,I,J,K) /= cellVolume(I,J,K);
	  }
	}
      }
      if (debug) LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 3");
    }
  }

  if (debug) LaplacianDisplay.display (returnedValue, "Laplacian: final value before return");
  

  return (returnedValue);
}


// ================================================================================
//\begin{>>Laplacian.tex}{\subsection{divInverseScalarGradCoefficients}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
divInverseScalarGradCoefficients(
				 const REALMappedGridFunction & scalar,
				 const Index & I1,	// = nullIndex
				 const Index & I2,	// = nullIndex
				 const Index & I3,	// = nullIndex
				 const Index & E,       // = nullIndex
				 const Index & C,       // = nullIndex
				 const Index & I6,      // = nullIndex
				 const Index & I7,      // = nullIndex
				 const Index & I8) 	// = nullIndex

//
// /Purpose:
//  	Compute coefficients for Div((1/scalar)*Grad()) on a MappedGrid
//	used to provide input to elliptic solver class Oges. 
//
// /scalar (input):
//			reference to the cellCentered REALMappedGridFunction
//			containing the scalar in the 
//			Div((1/scalar)*grad()) operator.
//
// /I1,I2,I3 (input):	if != nullIndex, the coefficients are computed only 
//			for the ranges described by I1,I2,I3. If I1,I2,I3
//			are not given they default to "nullIndex", and the
//			coefficients will be computed at all interior points
//			of the MappedGrid
// /E:  specifies the (subset of) equations for which the operator is to 
//      be computed.
// /C:  specifies the (subset of) components for which the operator is to
//      be computed.
//
//
// /divInverseScalarGradCoefficients (output):
//			The returned value is the coefficient array for
//			the MappedGrid associated with the MappedGridFiniteVolumeOperators
//			object.
//
// /3D: yes
//
// /Author:		D.L.Brown
//\end{Laplacian.tex} 
  //========================================
{
  LaplacianType lt = inverseScalarVariableCoefficients;
  return (genericLaplacianCoefficientsForSystems (lt, scalar, I1, I2, I3, E, C));
//  return (GeneralLaplacian (lt, scalar, I1, I2, I3));
}


// ================================================================================
//\begin{>>Laplacian.tex}{\subsection{laplacianCoefficients}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
laplacianCoefficients(const Index & I1,	// = nullIndex
		      const Index & I2,	// = nullIndex
		      const Index & I3,	// = nullIndex
		      const Index & E,  // = nullIndex
		      const Index & C,  // = nullIndex
		      const Index & I6, // = nullIndex
		      const Index & I7, // = nullIndex
		      const Index & I8) // = nullIndex
//
// /Purpose:
//  	Compute constant coefficient Laplacian coefficients on a MappedGrid
//	used to provide input to elliptic solver class Oges.
//
// /I1,I2,I3(input):
//	                if != nullIndex, the coefficients are computed only 
//			for the ranges described by I1,I2,I3. If I1,I2,I3
//			are not given they default to "nullIndex", and the
//			coefficients will be computed at all interior points
//			of the MappedGrid
// /E:  specifies the (subset of) equations for which the operator is to 
//      be computed.
// /C:  specifies the (subset of) components for which the operator is to
//      be computed.
//
//
// /laplacianCoefficients (output):
//			The returned value is the coefficient array for
//			the MappedGrid associated with the MappedGridFiniteVolumeOperators
//			object.
// /3D: yes
//
// Author:		D.L.Brown
//\end{Laplacian.tex} 
  //========================================
{
  REALMappedGridFunction scalar; //dummy scalar
  LaplacianType lt = constCoeff;
  return (genericLaplacianCoefficientsForSystems (lt, scalar, I1, I2, I3, E, C));
//  return (GeneralLaplacian (lt, scalar, I1, I2, I3));
  
}

// ================================================================================
//\begin{>>Laplacian.tex}{\subsection{divScalarGradCoefficients}}  
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
divScalarGradCoefficients(const REALMappedGridFunction & scalar,
			  const Index & I1,	// = nullIndex
			  const Index & I2,	// = nullIndex
			  const Index & I3,	// = nullIndex
			  const Index & E,	// = nullIndex
			  const Index & C,	// = nullIndex
			  const Index & I6,	// = nullIndex
			  const Index & I7,	// = nullIndex
			  const Index & I8) 	// = nullIndex

//
//
// /Purpose:
//  	Compute coefficients for Div(scalar*Grad()) on a MappedGrid
//	used to provide input to elliptic solver class Oges.
//
// /scalar (input):
//			reference to the cellCentered REALMappedGridFunction
//			containing the scalar in the 
//			Div(scalar*grad()) operator.
//
// /I1,I2,I3 (input):	if $\neq$ nullIndex, the coefficients are computed only 
//			for the ranges described by I1,I2,I3. If I1,I2,I3
//			are not given they default to "nullIndex", and the
//			coefficients will be computed at all interior points
//			of the MappedGrid
// /E:  specifies the (subset of) equations for which the operator is to 
//      be computed.
// /C:  specifies the (subset of) components for which the operator is to
//      be computed.
//
// /divScalarGradCoefficients (output):
//			The returned value is the coefficient array for
//			the MappedGrid associated with the MappedGridFiniteVolumeOperators
//			object.
// /3D: yes
//
// /Author: D. L. Brown
//\end{Laplacian.tex} 
  //========================================
{
  LaplacianType lt = variableCoefficients;
  return (genericLaplacianCoefficientsForSystems (lt, scalar, I1, I2, I3, E, C));
//  return (GeneralLaplacian (lt, scalar, I1, I2, I3));
  
}




