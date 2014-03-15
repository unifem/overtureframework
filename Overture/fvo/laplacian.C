#include "MappedGridFiniteVolumeOperators.h"
#include "laplacian.h"

//==============================================================================
//\begin{>laplacian.tex}{\subsection{laplacian}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
laplacian (const REALMappedGridFunction & u,
           const Index & I1 ,		// = nullIndex
           const Index & I2 ,		// = nullIndex
           const Index & I3 ,		// = nullIndex
           const Index & N,
	   const Index & I5,
	   const Index & I6,
	   const Index & I7,
	   const Index & I8) 		// = nullIndex
//
// /Purpose:
//         returns the (discrete) Laplacian of a REALMappedGridFunction, or a
//         subset of the components of a REALMappedGridFunction.
//
//     /u (input):             the REALMappedGridFunction to take the Laplacian of
//     /I1,I2,I3 (input):      (optional)    Laplacian to be returned for cells defined by these 
//                             Index'es. If = nullIndex, defaults to all possible
//                             cells on the grid
//
//     /N:               (optional) Laplacian returned for subset of components (4th index)
//                       of u defined by this Index. If = nullIndex (default),
//                       compute and return Laplacian for all components; see note below
//                       on components of the returned function.
//
//     /laplacian:         a REALMappedGridFunction 
//                       containing approximate  values of the elliptic function
//                       if N != null index, dimension is (all,all,all,Index(N.getBase(), N.getBound()))
//                        otherwise                       (all,all,all,Index(u.getBase(3), u.getBound()))
//
// /Author:				D.L.Brown
//\end{laplacian.tex} 
//==============================================================================
{

  if (debug) laplacianDisplay.interactivelySetInteractiveDisplay ("laplacianDisplay");

  REALMappedGridFunction dummy;
  standardOrderingErrorMessage (u, "laplacian");
  LaplacianType lt = constCoeff;

  return (generalLaplacian(lt, u, dummy, I1, I2, I3, N));
}

//==============================================================================
//\begin{>>laplacian.tex}{\subsection{divScalarGrad}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
divScalarGrad(const REALMappedGridFunction & u,
              const REALMappedGridFunction & s,
              const Index & I1 ,		// = nullIndex
              const Index & I2 ,		// = nullIndex
              const Index & I3 ,		// = nullIndex
              const Index & N,
	      const Index & I5,
	      const Index & I6,
	      const Index & I7,
	      const Index & I8)  		// = nullIndex
//
// /Purpose:
//         returns the discrete elliptic operator div.(scalar.grad(u)), for all or a
//         subset of the components of a REALMappedGridFunction.
//
//     /u (input):             the REALMappedGridFunction to take the Laplacian of
//     /I1,I2,I3 (input/optional):      
//                       Laplacian to be returned for cells defined by these 
//                       Index'es. If = nullIndex, defaults to all possible
//                       cells on the grid
//     /N (input/optional):             
//                       Laplacian returned for subset of components (4th index)
//                       of u defined by this Index. If = nullIndex,
//                       compute and return Laplacian for all components
//
//     /divScalarGrad (output):
//                       a REALMappedGridFunction 
//                       containing approximate  values of the elliptic function
//                       if N != null index, dimension is (all,all,all,Index(N.getBase(), N.getBound()))
//                        otherwise                       (all,all,all,Index(u.getBase(3), u.getBound()))
//
//
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	951011
//\end{laplacian.tex} 
//========================================
{
  standardOrderingErrorMessage (u, "divScalarGrad");
  LaplacianType lt = variableCoefficients;
  return (generalLaplacian (lt, u, s, I1, I2, I3, N));
}

//==============================================================================
//\begin{>>laplacian.tex}{\subsection{divInverseScalarGrad}} 
REALMappedGridFunction MappedGridFiniteVolumeOperators::
divInverseScalarGrad(const  REALMappedGridFunction & u,
                     const REALMappedGridFunction & s,
                     const Index & I1 ,		// = nullIndex
                     const Index & I2 ,		// = nullIndex
                     const Index & I3 ,		// = nullIndex
                     const Index & N,
		     const Index & I5,
		     const Index & I6,
		     const Index & I7,
		     const Index & I8)  		// = nullIndex
//
// /Purpose:
//         returns the discrete elliptic operator div((1/scalar)grad(u) for all or a
//         subset of the components of a REALMappedGridFunction.
//
//     /u:             the REALMappedGridFunction to take the Laplacian of
//     /I1,I2,I3:      Laplacian to be returned for cells defined by these 
//                       Index'es. If = nullIndex, defaults to all possible
//                       cells on the grid
//     /N:             Laplacian returned for subset of components (4th index)
//                       of u defined by this Index. If = nullIndex,
//                       compute and return Laplacian for all components
//
//     /divInverseScalarGrad:     a REALMappedGridFunction 
//                       containing approximate  values of the elliptic function
//                       if N != null index, dimension is (all,all,all,Index(N.getBase(), N.getBound()))
//                        otherwise                       (all,all,all,Index(u.getBase(3), u.getBound()))
// 
//
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	951011
//\end{laplacian.tex} 
//========================================
{
  standardOrderingErrorMessage (u, "divScalarGrad");
  LaplacianType lt = inverseScalarVariableCoefficients;
  return (generalLaplacian (lt, u, s, I1, I2, I3, N));
}


// ================================================================================
REALMappedGridFunction MappedGridFiniteVolumeOperators::  
generalLaplacian(                             				// laplacian operator
		 const LaplacianType laplacianType,
		 const REALMappedGridFunction & u,
		 const REALMappedGridFunction & scalar,
		 const Index & I1,		// = nullIndex
		 const Index & I2,		// = nullIndex
		 const Index & I3,		// = nullIndex
		 const Index & N 		// = nullIndex
                        )
// ================================================================================
	//========================================
	// Author:		D.L.Brown
	// Date Created:	950628
	// Date Modified:	950628
	//
	// Purpose:
	// compute Laplacian for a finite-volume grid
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
  Range NRet;
  Range returnedComponentRange;
  
  int numberOfVelocityComponents, offset;
  
    if (N.length() != 0) // we are (potentially) only interested in a subset of the components
  {
    numberOfVelocityComponents = N.getBound() - N.getBase() + 1;
    offset = N.getBase();  //we are possibly not starting with the first component and need to take this into acct.
    returnedComponentRange = Range(N.getBase(),N.getBound());
    
  }
  else // compute Laplacian(u) for all components
  {
    numberOfVelocityComponents = u.getComponentDimension(0);
    offset = 0;
    returnedComponentRange = Range(u.getComponentBase(0),u.getComponentBound(0));
  }

  NRet = Range(0,numberOfVelocityComponents-1);

  if (debug) laplacianDisplay.interactivelySetInteractiveDisplay ("laplacian initialization");

	// ========================================
	// Initialize returnedValue
	// ========================================

  REALMappedGridFunction returnedValue;
  returnedValue.updateToMatchGrid (mappedGrid, all, all, all,NRet);
  returnedValue = 0.;

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
  // for the appropriate version of the laplacian.
  //
  // For laplacianType = constCoeff, the scalar is ignored
  // For laplacianType = scalar, the scalar and cellVolume
  //		are averaged to faces and then divided
  // For laplacianType = inverseScalar, the inverse scalar and cellVolume
  //		are both averaged to faces, multiplied and then
  //		inverted.
  // ========================================

  REALMappedGridFunction faceScalar;;

//  realMappedGridFunction cv (mappedGrid, GridFunctionParameters::defaultCentering, 1);
//  realMappedGridFunction cv;
//  cv  = mappedGrid.cellVolume();
//  cv = cellVolume;
//  laplacianDisplay.display (cv, "cv in laplacian");
  
//  faceScalar.updateToMatchGridFunction(cv, all, all, all, faceRange);
  faceScalar.updateToMatchGridFunction(cellVolume, all, all, all, faceRange);
  faceScalar.setFaceCentering (GridFunctionParameters::all);
  faceScalar = 0.;

  if (debug) laplacianDisplay.display (faceScalar, "laplacian: faceScalar initialization");

  Index If,Jf,Kf;
  int dimension;
  int extraf = max(mappedGrid.numberOfGhostPoints())-1;


  if (laplacianType == constCoeff)
  {
//    faceScalar = cellsToFaces (cv);
    faceScalar = cellsToFaces (cellVolume);
     if (debug) laplacianDisplay.display (faceScalar, "laplacian: faceScalar for constCoeff");
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) = 1./faceScalar(If,Jf,Kf,dimension);
    }
  }
  if (laplacianType == inverseScalarVariableCoefficients)
  {
    // 950628 change scalar averaging to be arithmetic.

    REALMappedGridFunction scaledValue;
//    scaledValue.updateToMatchGridFunction (cellVolume);
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
//    faceCellVolume.updateToMatchGridFunction (cellVolume, all, all, all, faceRange);
    faceCellVolume.updateToMatchGrid (mappedGrid, all, all, all, faceRange);
    faceCellVolume.setFaceCentering (GridFunctionParameters::all);
    faceCellVolume = cellsToFaces(mappedGrid.cellVolume());
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) /= faceCellVolume(If,Jf,Kf,dimension);
    }
     if (debug) laplacianDisplay.display (faceScalar, "laplacian: faceScalar for inverseScalarVariableCoefficients");
  }
  if (laplacianType == variableCoefficients)
  {
    faceScalar = cellsToFaces(scalar);
    REALMappedGridFunction faceCellVolume;
//    faceCellVolume.updateToMatchGridFunction (cellVolume,numberOfDimensions);
//    faceCellVolume.updateToMatchGridFunction (cellVolume, all, all, all, numberOfDimensions);

    faceCellVolume.updateToMatchGrid (mappedGrid, all, all, all, numberOfDimensions);
    faceCellVolume = cellsToFaces(mappedGrid.cellVolume());
    for (dimension=0; dimension<numberOfDimensions; dimension++)
    {
      getIndex (faceScalar, dimension, If, Jf, Kf, extraf);
      faceScalar(If,Jf,Kf,dimension) /= faceCellVolume(If,Jf,Kf,dimension);
    }
  if (debug) laplacianDisplay.display (faceScalar, "laplacian: faceScalar for variableCoeffients:");
  }

  int component, uComponent;
  
  if (numberOfDimensions == 2) {
    

    int i0, i1, j0, j1;

    if (useCMPGRDGeometryArrays) 
    {
      if (debug) cout << "XXXXX laplacian: using CMPGRD geometry arrays " << endl;
      
      ForAllVelocityComponents (component)
      {
	uComponent = component + offset;
	ForDplus(i0){ ForDplus(j0) { ForDminus(i1) { ForDminus(j1) {

	int i = i0+i1;
	int j = j0+j1;

	returnedValue(I,J,K,component) +=
	  Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(


	    (SQR(rX(I+i0,J,K)) + SQR(rY(I+i0,J,K)))
	    * Delta(j0,0)
	    * Dminus(i1)
	    * Delta(j1,0)
            * u(I+i,J+j,K,uComponent)


	    + (rX(I+i0,J,K) * RAverage(i1,j0)*sX(I+i0+i1,J+j0,K) +  rY(I+i0,J,K) * RAverage(i1,j0)*sY(I+i0+i1,J+j0,K))
	    * Dminus(j1)
		* u(I+i,J+j,K,uComponent)
	     );

      }}}}
				  }
      

      if (debug) laplacianDisplay.display (returnedValue, "laplacian: intermediate value 1");

      ForAllVelocityComponents (component) 
      {
	uComponent = component + offset;
	ForDplus(j0) { ForDminus(j1) { ForDplus (i0) { ForDminus (i1) {

	int i = i0+i1;
	int j = j0+j1;

	returnedValue(I,J,K,component) +=
	  Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(


	    (SQR(sX(I,J+j0,K)) + SQR(sY(I,J+j0,K)))
	    * Delta(i0,0)
	    * Dminus(j1)
	    * Delta(i1,0)
	    * u(I+i,J+j,K,uComponent)


	    +(sX(I,J+j0,K) * SAverage(i0,j1)*rX(I+i0,J+j0+j1,K) + sY(I,J+j0,K) * SAverage(i0,j1)*rY(I+i0,J+j0+j1,K))
	    * Dminus(i1)
	    * u(I+i,J+j,K,uComponent)
	     );

      }}}}
				   }
      

      if (debug) laplacianDisplay.display (returnedValue, "laplacian: intermediate value 2");

    } else {

      ForAllVelocityComponents (component) 
      { 
	uComponent = component + offset;
	ForDplus(i0){ ForDplus(j0) { ForDminus(i1) { ForDminus(j1) {

	int i = i0+i1;
	int j = j0+j1;

	returnedValue(I,J,K,component) +=
	  Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(


	    (SQR(Rx(I+i0,J,K)) + SQR(Ry(I+i0,J,K)))
	    * Delta(j0,0)
	    * Dminus(i1)
	    * Delta(j1,0)
	    * u(I+i,J+j,K,uComponent)


	    + (Rx(I+i0,J,K) * RAverage(i1,j0)*Sx(I+i0+i1,J+j0,K) +  Ry(I+i0,J,K) * RAverage(i1,j0)*Sy(I+i0+i1,J+j0,K))
	    * Dminus(j1)
	    * u(I+i,J+j,K,uComponent)

	     );

      }}}}
				   }
      

      if (debug) laplacianDisplay.display (returnedValue, "laplacian: intermediate value 1");

      ForAllVelocityComponents (component) 
      { 
	uComponent = component + offset;
	ForDplus(j0) { ForDminus(j1) { ForDplus (i0) { ForDminus (i1) {

	int i = i0+i1;
	int j = j0+j1;

	returnedValue(I,J,K,component) +=
	  Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(


	    (SQR(Sx(I,J+j0,K)) + SQR(Sy(I,J+j0,K)))
	    * Delta(i0,0)
	    * Dminus(j1)
	    * Delta(i1,0)
	    * u(I+i,J+j,K,uComponent)


	    +(Sx(I,J+j0,K) * SAverage(i0,j1)*Rx(I+i0,J+j0+j1,K) + Sy(I,J+j0,K) * SAverage(i0,j1)*Ry(I+i0,J+j0+j1,K))
	    * Dminus(i1)
	    * u(I+i,J+j,K,uComponent)
	     );

      }}}}
				   }
      
			       
      if (debug) laplacianDisplay.display (returnedValue, "laplacian: intermediate value 2");

    }

  } else { // if (numberOfDimensions == 3)

    int i0, i1, j0, j1, k0, k1;

    if (useCMPGRDGeometryArrays)
    {

      ForAllVelocityComponents (component) {
	uComponent = component + offset;
	
        ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){
	
	int i = i0+i1;
	int j = j0+j1;
	int k = k0+k1;
	returnedValue(I,J,K,component) +=
	  
	  Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(

	    (SQR(rX(I+i0,J,K)) + SQR(rY(I+i0,J,K)) + SQR(rZ(I+i0,J,K)))*Dminus(i1)
	  * Delta(j0,0)
	  * Delta(j1,0)
	  * Delta(k0,0)
	  * Delta(k1,0)
	  * u(I+i,J+j,K+k,uComponent)

	  + (rX(I+i0,J,K)*SRAverage(i1,j0,k0)*sX(I+i0+i1,J+j0,K+k0)
	   + rY(I+i0,J,K)*SRAverage(i1,j0,k0)*sY(I+i0+i1,J+j0,K+k0)
	   + rZ(I+i0,J,K)*SRAverage(i1,j0,k0)*sZ(I+i0+i1,J+j0,K+k0))
	  * Delta(k1,0)
	  * Dminus(j1)
	  * u(I+i,J+j,K+k,uComponent)

	  + (rX(I+i0,J,K)*TRAverage(i1,j0,k0)*tX(I+i0+i1,J+j0,K+k0)
	   + rY(I+i0,J,K)*TRAverage(i1,j0,k0)*tY(I+i0+i1,J+j0,K+k0)
	   + rZ(I+i0,J,K)*TRAverage(i1,j0,k0)*tZ(I+i0+i1,J+j0,K+k0))
	  * Delta(j1,0)
	  * Dminus(k1)
	  * u(I+i,J+j,K+k,uComponent)

	  );

      }}}}}}
      }
      

	
      ForAllVelocityComponents (component)
      {
	uComponent = component + offset;
       ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){
	
	int i = i0+i1;
	int j = j0+j1;
	int k = k0+k1;
	returnedValue(I,J,K,component) +=

	  Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(

	    (SQR(sX(I,J+j0,K)) + SQR(sY(I,J+j0,K)) + SQR(sZ(I,J+j0,K)))*Dminus(j1)
	  * Delta(k0,0)
	  * Delta(k1,0)
	  * Delta(i0,0)
	  * Delta(i1,0)
	  * u(I+i,J+j,K+k,uComponent)

	  + (sX(I,J+j0,K)*RSAverage(i0,j1,k0)*rX(I+i0,J+j0+j1,K+k0)
	   + sY(I,J+j0,K)*RSAverage(i0,j1,k0)*rY(I+i0,J+j0+j1,K+k0)
	   + sZ(I,J+j0,K)*RSAverage(i0,j1,k0)*rZ(I+i0,J+j0+j1,K+k0))
	  * Delta(k1,0)
	  * Dminus(i1)
	  * u(I+i,J+j,K+k,uComponent)

	  + (sX(I,J+j0,K)*TSAverage(i0,j1,k0)*rX(I+i0,J+j0+j1,K+k0)
	   + sY(I,J+j0,K)*TSAverage(i0,j1,k0)*rY(I+i0,J+j0+j1,K+k0)
	   + sZ(I,J+j0,K)*TSAverage(i0,j1,k0)*rZ(I+i0,J+j0+j1,K+k0))
	  * Delta(i1,0)
	  * Dminus(k1)
	  * u(I+i,J+j,K+k,uComponent)

	  );
	
      }}}}}}
     }
      
      ForAllVelocityComponents (component) 
      {
	uComponent = component + offset;
       ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){
	
	int i = i0+i1;
	int j = j0+j1;
	int k = k0+k1;
	returnedValue(I,J,K,component) +=

	  Dplus(k0)*faceScalar(I,J,K+k0,tAxis)*(

	    (SQR(tX(I,J,K+k0)) + SQR(tY(I,J,K+k0)) + SQR(tZ(I,J,K+k0)))*Dminus(k1)
	  * Delta(i0,0)
	  * Delta(i1,0)
	  * Delta(j0,0)
	  * Delta(j1,0)
	  * u(I+i,J+j,K+k,uComponent)

	  + (tX(I,J,K+k0)*RTAverage(i0,j0,k1)*rX(I+i0,J+j0,K+k0+k1)
	   + tY(I,J,K+k0)*RTAverage(i0,j0,k1)*rY(I+i0,J+j0,K+k0+k1)
	   + tZ(I,J,K+k0)*RTAverage(i0,j0,k1)*rZ(I+i0,J+j0,K+k0+k1))
	  * Delta(j1,0)
	  * Dminus(i1)
	  * u(I+i,J+j,K+k,uComponent)

	  + (tX(I,J,K+k0)*STAverage(i0,j0,k1)*sX(I+i0,J+j0,K+k0+k1)
	   + tY(I,J,K+k0)*STAverage(i0,j0,k1)*sY(I+i0,J+j0,K+k0+k1)
	   + tZ(I,J,K+k0)*STAverage(i0,j0,k1)*sZ(I+i0,J+j0,K+k0+k1))
	  * Delta(i1,0)
	  * Dminus(j1)
	  * u(I+i,J+j,K+k,uComponent)

	  );

      }}}}}}
     }
      

    } else {

      ForAllVelocityComponents (component) 
      {
	uComponent = component + offset;
      ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){
	
	int i = i0+i1;
	int j = j0+j1;
	int k = k0+k1;
	returnedValue(I,J,K,component) +=
	  
	  Dplus(i0)*faceScalar(I+i0,J,K,rAxis)*(

	    (SQR(Rx(I+i0,J,K)) + SQR(Ry(I+i0,J,K)) + SQR(Rz(I+i0,J,K)))*Dminus(i1)
	  * Delta(j0,0)
	  * Delta(j1,0)
	  * Delta(k0,0)
	  * Delta(k1,0)
	  * u(I+i,J+j,K+k,uComponent)

	  + (Rx(I+i0,J,K)*SRAverage(i1,j0,k0)*Sx(I+i0+i1,J+j0,K+k0)
	   + Ry(I+i0,J,K)*SRAverage(i1,j0,k0)*Sy(I+i0+i1,J+j0,K+k0)
	   + Rz(I+i0,J,K)*SRAverage(i1,j0,k0)*Sz(I+i0+i1,J+j0,K+k0))
	  * Delta(k1,0)
	  * Dminus(j1)
	  * u(I+i,J+j,K+k,uComponent)

	  + (Rx(I+i0,J,K)*TRAverage(i1,j0,k0)*Tx(I+i0+i1,J+j0,K+k0)
	   + Ry(I+i0,J,K)*TRAverage(i1,j0,k0)*Ty(I+i0+i1,J+j0,K+k0)
	   + Rz(I+i0,J,K)*TRAverage(i1,j0,k0)*Tz(I+i0+i1,J+j0,K+k0))
	  * Delta(j1,0)
	  * Dminus(k1)
	  * u(I+i,J+j,K+k,uComponent)

	  );

      }}}}}}
    }
      

      ForAllVelocityComponents (component) 
      {
	uComponent = component + offset;
       ForDplus(j0){ ForDminus(j1){ ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){
	
	int i = i0+i1;
	int j = j0+j1;
	int k = k0+k1;
	returnedValue(I,J,K,component) +=

	  Dplus(j0)*faceScalar(I,J+j0,K,sAxis)*(

	    (SQR(Sx(I,J+j0,K)) + SQR(Sy(I,J+j0,K)) + SQR(Sz(I,J+j0,K)))*Dminus(j1)
	  * Delta(k0,0)
	  * Delta(k1,0)
	  * Delta(i0,0)
	  * Delta(i1,0)
	  * u(I+i,J+j,K+k,uComponent)

	  + (Sx(I,J+j0,K)*RSAverage(i0,j1,k0)*Rx(I+i0,J+j0+j1,K+k0)
	   + Sy(I,J+j0,K)*RSAverage(i0,j1,k0)*Ry(I+i0,J+j0+j1,K+k0)
	   + Sz(I,J+j0,K)*RSAverage(i0,j1,k0)*Rz(I+i0,J+j0+j1,K+k0))
	  * Delta(k1,0)
	  * Dminus(i1)
	  * u(I+i,J+j,K+k,uComponent)

	  + (Sx(I,J+j0,K)*TSAverage(i0,j1,k0)*Rx(I+i0,J+j0+j1,K+k0)
	   + Sy(I,J+j0,K)*TSAverage(i0,j1,k0)*Ry(I+i0,J+j0+j1,K+k0)
	   + Sz(I,J+j0,K)*TSAverage(i0,j1,k0)*Rz(I+i0,J+j0+j1,K+k0))
	  * Delta(i1,0)
	  * Dminus(k1)
	  * u(I+i,J+j,K+k,uComponent)

	  );
	
      }}}}}}
      }
      
      ForAllVelocityComponents (component) 
      {
	uComponent = component + offset;
       ForDplus(k0){ ForDminus(k1){ ForDplus(i0){ ForDminus(i1){ ForDplus(j0){ ForDminus(j1){
	
	int i = i0+i1;
	int j = j0+j1;
	int k = k0+k1;
	returnedValue(I,J,K,component) +=

	  Dplus(k0)*faceScalar(I,J,K+k0,tAxis)*(

	    (SQR(Tx(I,J,K+k0)) + SQR(Ty(I,J,K+k0)) + SQR(Tz(I,J,K+k0)))*Dminus(k1)
	  * Delta(i0,0)
	  * Delta(i1,0)
	  * Delta(j0,0)
	  * Delta(j1,0)
	  * u(I+i,J+j,K+k,uComponent)

	  + (Tx(I,J,K+k0)*RTAverage(i0,j0,k1)*Rx(I+i0,J+j0,K+k0+k1)
	   + Ty(I,J,K+k0)*RTAverage(i0,j0,k1)*Ry(I+i0,J+j0,K+k0+k1)
	   + Tz(I,J,K+k0)*RTAverage(i0,j0,k1)*Rz(I+i0,J+j0,K+k0+k1))
	  * Delta(j1,0)
	  * Dminus(i1)
	  * u(I+i,J+j,K+k,uComponent)

	  + (Tx(I,J,K+k0)*STAverage(i0,j0,k1)*Sx(I+i0,J+j0,K+k0+k1)
	   + Ty(I,J,K+k0)*STAverage(i0,j0,k1)*Sy(I+i0,J+j0,K+k0+k1)
	   + Tz(I,J,K+k0)*STAverage(i0,j0,k1)*Sz(I+i0,J+j0,K+k0+k1))
	  * Delta(i1,0)
	  * Dminus(j1)
	  * u(I+i,J+j,K+k,uComponent)

	  );

      }}}}}}
      }
      
      }


  }

  // ========================================
  // Optionally scale the coefficients by inverse volume
  // ========================================

  if (!isVolumeScaled) {
    ForAllVelocityComponents (component) {
      returnedValue(I,J,K,component) /= mappedGrid.cellVolume()(I,J,K);
    }
  }

      if (debug) laplacianDisplay.display (returnedValue, "laplacian: intermediate value 3");
  

/* ***970304: if no compositeGrid, then no mask array; this should be left up to the user
  where (mappedGrid.mask()(I,J,K) <= 0)
  {
    ForAllVelocityComponents (component) returnedValue(I,J,K,component) = 0.;
  }
  */  

  returnedValue.periodicUpdate();

      if (debug) laplacianDisplay.display (returnedValue, "laplacian: intermediate value 4");

//  ... redim so that the components are numbered correctly as per documentation

  returnedValue.updateToMatchGrid (mappedGrid, all, all, all, returnedComponentRange);

        if (debug) laplacianDisplay.display (returnedValue, "laplacian: intermediate value 5");
  
  return (returnedValue);
}

