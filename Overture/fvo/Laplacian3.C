#include "MappedGridFiniteVolumeOperators.h"
#include "laplacian.h"
// ================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
Laplacian3(    					// laplacian operator
                        const Index & I1,
                        const Index & I2,
                        const Index & I3,
                        const Index & N 
                        )
// ================================================================================
{
  REALMappedGridFunction dummy;
  LaplacianType lt = constCoeff;

  return (GeneralLaplacian3(lt, dummy, I1, I2, I3, N));

}

// ================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
DivScalarGrad3(    					// laplacian operator
			const REALMappedGridFunction & scalar,
                        const Index & I1,
                        const Index & I2,
                        const Index & I3,
                        const Index & N 
                        )
// ================================================================================
{
  LaplacianType lt = variableCoefficients;
  return ( GeneralLaplacian3 (lt, scalar, I1, I2, I3, N));
}
// ================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
DivInverseScalarGrad3(    					// laplacian operator
			const REALMappedGridFunction & scalar,
                        const Index & I1,
                        const Index & I2,
                        const Index & I3,
                        const Index & N 
                        )
// ================================================================================
{
  LaplacianType lt = inverseScalarVariableCoefficients;
  return ( GeneralLaplacian3 (lt, scalar, I1, I2, I3, N));
}
// ================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
GeneralLaplacian3(                             				// laplacian operator
			const LaplacianType laplacianType,
			const REALMappedGridFunction & scalar,
                        const Index & I1,
                        const Index & I2,
                        const Index & I3,
                        const Index & N 
                        )
// ================================================================================
	//========================================
	// Author:		D.L.Brown
	// Date Created:	950509
	// Date Modified:	950712
	//
	// Purpose:
	// compute Laplacian coefficients for a finite-volume grid
	// In this version, the coefficients are indexed in the LAST (3rd) index of the array
	//
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================

{
  if (N.length() != 0) 
  {
    cout << "Laplacian not implemented for anything but the scalar case at this point" << endl;
    exit (-1);
  }

  LaplacianDisplay.interactivelySetInteractiveDisplay ("Laplacian initialization");

	// ========================================
	// Initialize returnedValue
	// ========================================

  REALMappedGridFunction returnedValue;
  int numberOfComponentsRV = (numberOfDimensions == 2) ? 9 : 27;
  int positionOfComponentRV  = 3;
  returnedValue.updateToMatchGrid (mappedGrid, all, all, all, numberOfComponentsRV);
  returnedValue = 0.;

#undef RX
#define RX(i,j,k,l,m) faceNormal(i,j,k,ndnd(l,m))

	// ========================================
	// might make sense to move these defs into the class.
	// ========================================

//  REALMappedGridFunction Rx,Ry,Rz,Sx,Sy,Sz,Tx,Ty,Tz;
//
//  Rx.link (faceNormal, ndnd(rAxis,xAxis));
//  Ry.link (faceNormal, ndnd(rAxis,yAxis));
//
//  Sx.link (faceNormal, ndnd(sAxis,xAxis));
//  Sy.link (faceNormal, ndnd(sAxis,yAxis));
//
//  if (numberOfDimensions == 3) {
//    Rz.link (faceNormal, ndnd(rAxis,zAxis));
//    Sz.link (faceNormal, ndnd(sAxis,zAxis));
//
//    Tx.link (faceNormal, ndnd(tAxis,xAxis));
//    Ty.link (faceNormal, ndnd(tAxis,yAxis));
//    Tz.link (faceNormal, ndnd(tAxis,zAxis));
//  }

  Index I,J,K;
  int extra = -max(mappedGrid.numberOfGhostPoints());

  int c[5] = {0,0,0,0,0};
  getDefaultIndex (returnedValue, c[0], c[1], c[2], c[3], c[4], I, J, K, extra, extra, extra, I1, I2, I3);

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
  faceScalar.updateToMatchGridFunction(cellVolume, all, all, all, numberOfDimensions);
  faceScalar.setIsFaceCentered();
  faceScalar = 0.;

  Index If,Jf,Kf;
  int dimension;
  int extraf = max(mappedGrid.numberOfGhostPoints())-1;

  if (laplacianType == constCoeff)
  {
    faceScalar = cellsToFaces (cellVolume);
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) = 1./faceScalar(If,Jf,Kf,dimension);
    }
  LaplacianDisplay.display (faceScalar, "Laplacian: faceScalar constCoeff:");
  }
  if (laplacianType == inverseScalarVariableCoefficients)
  {
    // 950628 change scalar averaging to be arithmetic.

    REALMappedGridFunction scaledValue;
    scaledValue.updateToMatchGridFunction (cellVolume);
    scaledValue.setIsCellCentered(TRUE);
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      scaledValue(If,Jf,Kf) = 1./scalar(If,Jf,Kf);
    }
    faceScalar = cellsToFaces (scaledValue);
    REALMappedGridFunction faceCellVolume;
    faceCellVolume.updateToMatchGridFunction (cellVolume, all, all, all, numberOfDimensions);
    faceCellVolume = cellsToFaces(cellVolume);
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) /= faceCellVolume(If,Jf,Kf,dimension);
    }
  LaplacianDisplay.display (faceScalar, "Laplacian: faceScalar inverseScalarVariableCoefficients:");
  }
  if (laplacianType == variableCoefficients)
  {
    faceScalar = cellsToFaces(scalar);
    REALMappedGridFunction faceCellVolume;
    faceCellVolume.updateToMatchGridFunction (cellVolume, all, all, all, numberOfDimensions);
    faceCellVolume = cellsToFaces(cellVolume);
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) /= faceCellVolume(If,Jf,Kf,dimension);
    }
  LaplacianDisplay.display (faceScalar, "Laplacian: faceScalar variableCoefficients:");



   
  }

  LaplacianDisplay.display (faceScalar, "Laplacian: faceScalar:");

  if (numberOfDimensions == 2) {
    
#undef RETURNED_VALUE
#define RETURNED_VALUE(i,j,k,l,m) returnedValue(i,j,k,coeff(l,m))

    int i0, i1, i2, j0, j1, j2;

    ForDplus(i0){ ForDplus(j0) { ForDminus(i1) { ForDminus(j1) {

      int i = i0+i1;
      int j = j0+j1;
      RETURNED_VALUE(I,J,K,i,j) +=
	Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(

	  //(Sqr(RX(I+i0,J,K,rAxis,xAxis)) + Sqr(RX(I+i0,J,K,rAxis,yAxis)))

	  (Sqr(Rx(I+i0,J,K)) + Sqr(Ry(I+i0,J,K)))
	  * Delta(j0,0)
	  * Dminus(i1)
	  * Delta(j1,0)

	  //+ (RX(I+i0,J,K,rAxis,xAxis) * RAverage(i1,j0)*RX(I+i0+i1,J+j0,K,sAxis,xAxis)
	  //+  RX(I+i0,J,K,rAxis,yAxis) * RAverage(i1,j0)*RX(I+i0+i1,J+j0,K,sAxis,yAxis))

	  + (Rx(I+i0,J,K) * RAverage(i1,j0)*Sx(I+i0+i1,J+j0,K) +  Ry(I+i0,J,K) * RAverage(i1,j0)*Sy(I+i0+i1,J+j0,K))
	  * Dminus(j1)
	   );

    }}}}

    LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 1");

    ForDplus(j0) { ForDminus(j1) { ForDplus (i0) { ForDminus (i1) {

      int i = i0+i1;
      int j = j0+j1;
      RETURNED_VALUE(I,J,K,i,j) +=
	Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(

	  //(Sqr(RX(I,J+j0,K,sAxis,xAxis)) + Sqr(RX(I,J+j0,K,sAxis,yAxis)))

	  (Sqr(Sx(I,J+j0,K)) + Sqr(Sy(I,J+j0,K)))
	  * Delta(i0,0)
	  * Dminus(j1)
	  * Delta(i1,0)

	  //+(RX(I,J+j0,K,sAxis,xAxis) * SAverage(i0,j1)*RX(I+i0,J+j0+j1,K,rAxis,xAxis)
	  //+ RX(I,J+j0,K,sAxis,yAxis) * SAverage(i0,j1)*RX(I+i0,J+j0+j1,K,rAxis,yAxis))

	  +(Sx(I,J+j0,K) * SAverage(i0,j1)*Rx(I+i0,J+j0+j1,K) + Sy(I,J+j0,K) * SAverage(i0,j1)*Ry(I+i0,J+j0+j1,K))
	  * Dminus(i1)
	   );

    }}}}

    LaplacianDisplay.display (returnedValue, "Laplacian: intermediate value 2");

  } else { // if (numberOfDimensions == 3)

#undef RETURNED_VALUE
#define RETURNED_VALUE(i,j,k,l,m,n) returnedValue(i,j,k,coeff(l,m,n))

    int i0, i1, i2, j0, j1, j2, k0, k1, k2;

    ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){
      
      int i = i0+i1;
      int j = j0+j1;
      int k = k0+k1;
      RETURNED_VALUE(I,J,K,i,j,k) +=
	
	Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(

	  (Sqr(Rx(I+i0,J,K)) + Sqr(Ry(I+i0,J,K)) + Sqr(Rz(I+i0,J,K)))*Dminus(i1)
	* Delta(j0,0)
	* Delta(j1,0)
	* Delta(k0,0)
	* Delta(k1,0)

	+ (Rx(I+i0,J,K)*SRAverage(i1,j0,k0)*Sx(I+i0+i1,J+j0,K+k0)
	 + Ry(I+i0,J,K)*SRAverage(i1,j0,k0)*Sy(I+i0+i1,J+j0,K+k0)
	 + Rz(I+i0,J,K)*SRAverage(i1,j0,k0)*Sz(I+i0+i1,J+j0,K+k0))
	* Delta(k1,0)
	* Dminus(j1)

	+ (Rx(I+i0,J,K)*TRAverage(i1,j0,k0)*Tx(I+i0+i1,J+j0,K+k0)
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
      RETURNED_VALUE(I,J,K,i,j,k) +=

	Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(

	  (Sqr(Sx(I,J+j0,K)) + Sqr(Sy(I,J+j0,K)) + Sqr(Sz(I,J+j0,K)))*Dminus(j1)
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
      RETURNED_VALUE(I,J,K,i,j,k) +=

	Dplus(k0)*faceScalar(I,J,K+k0,tAxis)*(

	  (Sqr(Tx(I,J,K+k0)) + Sqr(Ty(I,J,K+k0)) + Sqr(Tz(I,J,K+k0)))*Dminus(k1)
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

  int component;
  if (!isVolumeScaled){
    for (component=0; component<numberOfComponentsRV; component++){
    
      returnedValue(I,J,K,component) /= cellVolume(I,J,K);
    }
  }

  return (returnedValue);
}

