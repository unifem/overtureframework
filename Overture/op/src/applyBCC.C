#include "MappedGridOperators.h"
#include "SparseRep.h"
#include "conversion.h"
#include "display.h"

#include "MappedGridOperatorsInclude.h"
#include "ParallelUtility.h"

// extern realMappedGridFunction Overture::nullRealMappedGridFunction();

//========================================================================================
//  This file defines the Boundary condition routines for Vertex Based Grid Functions
//
//  Notes:
//     See the documentation for further details
//========================================================================================

#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )



// These are for indexing into a coefficient matrix
#undef M2
#define M2(m1,m2) ((m1)+1+3*((m2)+1))
#undef M3
#define M3(m1,m2,m3) ((m1)+1+3*((m2)+1+3*((m3)+1)))





// MERGE0 : use for A++ operations when the first index is a scalar
//      a(i0,I1,I2,I3)=
// you must define the following in your code
//  int dum;
//  Range aR0,aR1,aR2,aR3;
#define MERGE0(a,i0,I1,I2,I3) \
  for(  \
      aR0=Range(a.getBase(0),a.getBound(0)),   \
      aR1=Range(a.getBase(1),a.getBound(1)),   \
      aR2=Range(a.getBase(2),a.getBound(2)),   \
      aR3=Range(a.getBase(3),a.getBound(3)),   \
      a.reshape(Range(0,aR0.length()*aR1.length()-1),aR2,aR3), \
      dum=0; dum<1; dum++,  \
      a.reshape(aR0,aR1,aR2,aR3) ) \
    a(Index(i0-aR0.getBase()+aR0.length()*(I1.getBase()-aR1.getBase()),   \
      I1.length(),aR0.length()),I2,I3)


// Use this for indexing into coefficient matrices representing systems of equations
#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))

#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilLength0*(n))


#define OPX(m1,m2,m3,n,I1,I2,I3) MERGE0(opX,M123N(m1,m2,m3,n),I1,I2,I3)
#define OPY(m1,m2,m3,n,I1,I2,I3) MERGE0(opY,M123N(m1,m2,m3,n),I1,I2,I3)
#define OPZ(m1,m2,m3,n,I1,I2,I3) MERGE0(opZ,M123N(m1,m2,m3,n),I1,I2,I3)

#define ForStencil(m1,m2,m3)   \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1; m1++) 

#define ForStencilN(n,m1,m2,m3)   \
    for( n=0; n<numberOfComponents; n++) \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1; m1++) 


#define FOR_4D(m,i1,i2,i3,M,I1,I2,I3) \
int mBase=M.getBase(), mBound=M.getBound(); \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++) \
for(m=mBase; m<=mBound; m++)

#define FOR_4(m,i1,i2,i3,M,I1,I2,I3) \
mBase=M.getBase(), mBound=M.getBound(); \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++) \
for(m=mBase; m<=mBound; m++)

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


static real extrapCoeff[10][10] = 
                   {
                       {1.,-1.,0.,0.,0.,0.,0.,0.,0.,0.},     // order 1
                       {1.,-2.,1.,0.,0.,0.,0.,0.,0.,0.},     // order 2		       
                       {1.,-3.,3.,-1.,0.,0.,0.,0.,0.,0.},    // order 3		       
                       {1.,-4.,6.,-4.,1.,0.,0.,0.,0.,0.},		       
                       {1.,-5.,10.,-10.,5.,-1.,0.,0.,0.,0.},		       
                       {1.,-6.,15.,-20.,15.,-6.,1.,0.,0.,0.},		       
                       {1.,-7.,21.,-35.,35.,-21.,7.,-1.,0.,0.},		       
                       {1.,-8.,28.,-56.,70.,-56.,28.,-8.,1.,0.},		       
		       {1.,-9.,36.,-84.,126.,-126.,84.,-36.,9.,-1.}
		     };


bool
getLocalIndex( intArray & u, intSerialArray & uLocal, 
               Index & I1, Index & I2, Index & I3,
               Index & J1, Index & J2, Index & J3 )
// ============================================================================================
//  /Description:
//     Return the Index's local to the processor we are on.
// ============================================================================================
{
  const int n1a = max(I1.getBase() , uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
  const int n1b = min(I1.getBound(),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));

  const int n2a = max(I2.getBase() , uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
  const int n2b = min(I2.getBound(),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));

  const int n3a = max(I3.getBase() , uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
  const int n3b = min(I3.getBound(),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));

  if( n1a>n1b || n2a>n2b || n3a>n3b ) return false; 

  J1 = Range(n1a,n1b); J2 = Range(n2a,n2b); J3 = Range(n3a,n3b);
  return true;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{applyBoundaryConditionCoefficients}}  
void MappedGridOperators::
applyBoundaryConditionCoefficients(realMappedGridFunction & uCoeff, 
				   const Index & E,
				   const Index & C,
				   const BCTypes::BCNames & 
                                         bcType,                     /* = BCTypes::dirichlet */
				   const int & bc,                   /* = allBoundaries */
				   const BoundaryConditionParameters & 
				        bcParams /* = Overture::defaultBoundaryConditionParameters() */,
                                   const int & grid /* =0 */ )
//=======================================================================================
// /Description:
//  Fill in the coefficients of the boundary conditions.
//
// /uCoeff (input/output): grid function to hold the coefficients of the BC.
// /E (input): apply to these equations (for a system of equations)
// /C (input): apply to these components
// /t (input): apply boundary conditions at this time.
// /Notes:
//    If you supply Range objects for {\tt E} and {\tt C} then the boundary conditions
//   are filled in for all equations and components indicated by the Ranges 
//   and NOT just the "diagonal" entries (as might be first expected). Thus normally you
//   will want to specify {\tt E} and {\tt C} to just be int's.
// /Limitations:
//  too many to write down.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
//  real time=getCPU();  // keep track of the cpu time spent in this routine

  if( numberOfDimensions==0 )
  {
    cout << "MappedGridOperators::ERROR: you must assign a MappedGrid before "
            " applyBoundaryConditionCoefficients! \n";
    return;
  }
  if( uCoeff.sparse==NULL )
  {
    cout << "MappedGridOperators::applyBoundaryConditionCoefficients:ERROR: The coeff " 
            "realMappedGridFunction is not a coefficient-matrix\n";
    cout << "You should call uCoeff.setIsACoefficientMatrix(TRUE); \n";
    Overture::abort("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR");
  }

  if( false )
    printF("\n\n *** applyBCC grid=%i ***\n\n",grid);

  MappedGrid & mg = mappedGrid; 
  int side,axis;
  const bool isRectangular=mg.isRectangular();

  int sideStart=0, sideEnd=1;
  int axisStart=0, axisEnd=mg.numberOfDimensions()-1;
  bool doOnlyOneSide=FALSE;
  if( bc>=BCTypes::boundary1 && bc<=BCTypes::boundary6 )
  {
    // do only one side
    doOnlyOneSide=TRUE;
    sideStart=(bc-BCTypes::boundary1) % 2;
    sideEnd=sideStart;
    axisStart=(bc-BCTypes::boundary1)/2;
    axisEnd=axisStart;
  }

  // ** first determine if we need to apply the BC at all
  bool boundaryConditionFound=false;
  for( axis=axisStart; axis<=axisEnd; axis++ )
  for( side=sideStart; side<=sideEnd && !boundaryConditionFound; side++ )
  {
    if( mg.boundaryCondition(side,axis) > 0 && 
        ( doOnlyOneSide || mg.boundaryCondition(side,axis)==bc || bc==allBoundaries ) )
    {
      boundaryConditionFound=true;
      break;
    }
  }
  if( !boundaryConditionFound )
  {
    return;
  }

  if( !boundaryNormalsUsed && bcType!=BCTypes::dirichlet && bcType!=BCTypes::extrapolate &&
      ( (bcType==BCTypes::neumann && !isRectangular) || 
        (bcType==BCTypes::mixed && !isRectangular) || 
       bcType==BCTypes::normalComponent ||
       bcType==BCTypes::normalDotScalarGrad  || 
       bcType==BCTypes::vectorSymmetry  || 
       bcType==BCTypes::normalDerivativeOfADotU ||
       bcType==BCTypes::normalDerivativeOfNormalComponent ||  
       bcType==BCTypes::normalDerivativeOfTangentialComponent0 ||
       bcType==BCTypes::normalDerivativeOfTangentialComponent1  ||  
       bcType==BCTypes::extrapolateNormalComponent) )
  {
    boundaryNormalsUsed=true;
    #ifndef USE_PPP
      mappedGrid.update(MappedGrid::THEvertexBoundaryNormal);
    #else
      mappedGrid.update(MappedGrid::THEvertexBoundaryNormal);
      // *wdh* 100412 -- Do we still need this: 
      mappedGrid.update(MappedGrid::THEinverseVertexDerivative);
    #endif
  }
  if( !boundaryTangentsUsed && bcType!=BCTypes::dirichlet && bcType!=BCTypes::extrapolate &&
      ( bcType==BCTypes::normalDerivativeOfTangentialComponent0 || 
	bcType==BCTypes::normalDerivativeOfTangentialComponent1 ||
	bcType==BCTypes::tangentialComponent0 || 
	bcType==BCTypes::tangentialComponent1  || 
	bcType==BCTypes::tangentialComponent ||
	bcType==BCTypes::extrapolateTangentialComponent0 || 
        bcType==BCTypes::extrapolateTangentialComponent1 ) )
  {
    boundaryTangentsUsed=TRUE;
    mappedGrid.update(MappedGrid::THEcenterBoundaryTangent);
  }



  int mm[3], &m1=mm[0], &m2=mm[1], &m3=mm[2];
  int dir[3];

  const int & ghostLineToAssign = bcParams.ghostLineToAssign;
  int dum;
  Range aR0,aR1,aR2,aR3;
  int n=0, stencilLength0=stencilSize;  // ******* should use stencilSize from sparse ******

  const int halfWidth=orderOfAccuracy/2;

  int c,ee;
  int e0 = E.getBase(), e1=e0+1, e2=e1+1;
  int c0 = C.getBase();
  int nt,ndt;
  int v0,v1,v2;
  real a[3], &a1=a[0], &a2=a[1], &a3=a[2];
  real aNorm;

#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define ForStencil(m1,m2,m3)   \
	for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
	for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
	for( m1=-halfWidth1; m1<=halfWidth1; m1++) 

  Index M(0,stencilSize);
  int stencilDim=stencilSize*numberOfComponentsForCoefficients;
  Index M0(CE(c0,e0),stencilSize);
  Index M1(CE(c0,e0),stencilDim);
  // ME : this Index points to the coefficients of the equation(s) we are assigning
  Index ME(E.getBase()*stencilDim,E.length()*stencilDim);  
  Index Mv = Range(0,stencilSize*numberOfDimensions-1);

  realSerialArray coeffSave;

  Index I1,I2,I3;
  Index I1m,I2m,I3m;
  Index I1p,I2p,I3p;
  Index I1e,I2e,I3e;
  real b0,b1,b2,b3;
  
  // realArray & uA = uCoeff;
  #ifdef USE_PPP
    realSerialArray coeff; getLocalArrayWithGhostBoundaries(uCoeff,coeff);
    realSerialArray rsxy; 
    if( !rectangular ) getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rsxy); 
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
  #else
    realSerialArray & coeff = uCoeff; 
    realSerialArray rsxy; 
    intSerialArray & maskLocal = mg.mask();
  #endif
  
  assert( uCoeff.sparse!=NULL );
  SparseRepForMGF & sparse = *uCoeff.sparse;
  
  real *coeffp = coeff.Array_Descriptor.Array_View_Pointer3;
  const int coeffDim0=coeff.getRawDataSize(0);
  const int coeffDim1=coeff.getRawDataSize(1);
  const int coeffDim2=coeff.getRawDataSize(2);
  #undef COEFF
  #define COEFF(i0,i1,i2,i3) coeffp[i0+coeffDim0*(i1+coeffDim1*(i2+coeffDim2*(i3)))]
  int i1,i2,i3,m;

  bool normalIsNeeded=( (bcType==BCTypes::neumann && !isRectangular) || 
			(bcType==BCTypes::mixed && !isRectangular) || 
			bcType==BCTypes::normalDotScalarGrad ||
			bcType==BCTypes::normalComponent ||
			bcType==BCTypes::vectorSymmetry || 
			bcType==BCTypes::normalDerivativeOfNormalComponent || 
			bcType==BCTypes::normalDerivativeOfTangentialComponent0 || 
			bcType==BCTypes::normalDerivativeOfTangentialComponent1 || 
			bcType==BCTypes::extrapolateNormalComponent ); 

  bool tangentIsNeeded=( bcType==BCTypes::tangentialComponent0 ||
			 bcType==BCTypes::tangentialComponent1 || 
			 bcType==BCTypes::vectorSymmetry || 
			 bcType==BCTypes::normalDerivativeOfTangentialComponent0 || 
			 bcType==BCTypes::normalDerivativeOfTangentialComponent1 ||
			 bcType==BCTypes::extrapolateTangentialComponent0 ||
			 bcType==BCTypes::extrapolateTangentialComponent1 ); 


  const bool useNewVersion=true;  // avoid use of the merge macro

  for( axis=axisStart; axis<=axisEnd; axis++ )
  for( side=sideStart; side<=sideEnd; side++ )
  {
    if( mg.boundaryCondition(side,axis) > 0 && 
        ( doOnlyOneSide || mg.boundaryCondition(side,axis)==bc || bc==allBoundaries ) )
    {


      getGhostIndex( mg.indexRange(),side,axis,I1,I2,I3,bcParams.lineToAssign,bcParams.extraInTangentialDirections);    // boundary 
      getGhostIndex( mg.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParams.extraInTangentialDirections); // first ghost line
      getGhostIndex( mg.indexRange(),side,axis,I1p,I2p,I3p,-1,bcParams.extraInTangentialDirections); // first line in

      // Remember the full size Index's:
      Index J1=I1, J2=I2, J3=I3;
      Index J1m=I1m, J2m=I2m, J3m=I3m;
      Index J1p=I1p, J2p=I2p, J3p=I3p;
      
      bool ok=getLocalIndex(mg.mask(),maskLocal,I1,I2,I3,I1,I2,I3); 

      if( !ok ) continue;  // there are no points on this processor
      
      getLocalIndex(mg.mask(),maskLocal,I1m,I2m,I3m,I1m,I2m,I3m); 
      getLocalIndex(mg.mask(),maskLocal,I1p,I2p,I3p,I1p,I2p,I3p); 

      #ifndef USE_PPP
        realSerialArray & normal  = normalIsNeeded? mg.vertexBoundaryNormal(side,axis) : rsxy; 
        realSerialArray & tangent = tangentIsNeeded? mg.centerBoundaryTangent(side,axis): rsxy;
      #else
        realSerialArray & normal  = normalIsNeeded ? mg.vertexBoundaryNormalArray(side,axis) : rsxy; 
        realSerialArray & tangent = tangentIsNeeded ? mg.centerBoundaryTangentArray(side,axis) : rsxy; 
      #endif

      // The vect array holds either the normal or the tangent
      realSerialArray & vect = (bcType==BCTypes::normalDerivativeOfNormalComponent ||
		   	        bcType==BCTypes::extrapolateNormalComponent )  ? normal : tangent;


      // save boundary points to be reset if this is a mixed boundary condition (i.e. if some points
      // on the boundary are physical Bc and others interp) OR if the user has specified a BC mask ti use.
      const bool saveBoundaryPoints = (mg.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary &&
	bcParams.lineToAssign==0 && useWhereMaskOnBoundary[axis][side] && bcParams.useMixedBoundaryMask) || 
           bcParams.getUseMask();
      
//       if( true )
//       {
//         int temp=saveBoundaryPoints;
// 	printf("applyBCC: side,axis=%i,%i, boundaryFlag=%i bcParams.lineToAssign=%i \n"
//                "           useWhereMaskOnBoundary=%i useMixedBoundaryMask=%i --> saveBoundaryPoints= %i \n",
//                 side,axis,mg.boundaryFlag(side,axis),bcParams.lineToAssign,(int)useWhereMaskOnBoundary[axis][side],
//                 (int)bcParams.useMixedBoundaryMask,temp);
//         cout << "saveBoundaryPoints=" << saveBoundaryPoints << endl;
//       }
      
      bool resetBoundaryPoints=FALSE;
      

      const int n1=0, n2=1, n3=2;

      switch ( bcType )
      {

      case dirichlet:

	if( saveBoundaryPoints )
	{
          // save coefficients so we can reset some points where the BC should NOT be applied
          resetBoundaryPoints=TRUE;  // we reset the points after the switch statement
          coeffSave.redim(ME,I1,I2,I3);
          coeffSave=coeff(ME,I1,I2,I3);  
	}

        if( numberOfComponentsForCoefficients > 1 )
          coeff(ME,I1,I2,I3)=0.;  // zero out boundary equation

// #ifndef USE_PPP
//         coefficients(identityOperator,uCoeff,I1,I2,I3,e0,c0);
// #else
//         uCoeff(M0,I1,I2,I3)=identityCoefficients(I1,I2,I3,e0,c0)(M0,I1,I2,I3);   // *** this is inefficient ****
// #endif     
        assignCoefficients(identityOperator,coeff,I1,I2,I3,e0,c0);
	
	for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
            if( c!=c0 || ee!=e0 )
	      coeff(M+CE(c,ee),I1,I2,I3)=coeff(M0,I1,I2,I3);
        if( bcParams.lineToAssign>0 )
	{
	  sparse.setClassify(SparseRepForMGF::ghost1,J1,J2,J3,E);
	}
	break;
      case neumann:
      case mixed:
      {

        // const int myid=max(0,Communication_Manager::My_Process_Number);
	// printf("applyBCC: myid=%i orderOfAccuracy=%i stencilSize=%i\n",myid,orderOfAccuracy,stencilSize);


	realSerialArray  opX,opY,opZ;
	// *************** Neumann or Mixed *******************
	opX.redim(Range(M.getBase(),M.getBound()),            // dimension (to get base correct)
		  Range(I1.getBase(),I1.getBound()),
		  Range(I2.getBase(),I2.getBound()),
		  Range(I3.getBase(),I3.getBound()));
        opX=0.;
        if( !rectangular || axis==axis1 )
	{
          // in the rectangular case we only need to evaluate on derivative from x, y or z
	  assignCoefficients(xDerivative,opX,I1,I2,I3,0,0);
	}
//         display(opX,"opX ***new***");
// 	opX=xCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
//         display(opX,"opX  ***old****");

	if( numberOfDimensions>1 )
	{
          if( rectangular && axis==axis2 )
	  {
            assignCoefficients(yDerivative,opX,I1,I2,I3,0,0); // just save y-derivative in opX
	  }
	  else if( !rectangular )
	  {
	    opY.redim(opX);
	    opY=0.;
	    assignCoefficients(yDerivative,opY,I1,I2,I3,0,0);
	  }
	}
	if( numberOfDimensions>2 )
	{
          if( rectangular && axis==axis3 )
	  {
            assignCoefficients(zDerivative,opX,I1,I2,I3,0,0); // just save z-derivative in opX
	  }
	  else if( !rectangular )
	  {
	    opZ.redim(opX);
	    opZ=0.;
	    assignCoefficients(zDerivative,opZ,I1,I2,I3,0,0);
	  }
	}
        // display(normal(I1,I2,I3,axis1),"normal(I1,I2,I3,axis1)");
	
        real *normalp = normal.Array_Descriptor.Array_View_Pointer3;
        const int normalDim0=normal.getRawDataSize(0);
        const int normalDim1=normal.getRawDataSize(1);
        const int normalDim2=normal.getRawDataSize(2);
        #undef NORMAL
        #define NORMAL(i0,i1,i2,i3) normalp[i0+normalDim0*(i1+normalDim1*(i2+normalDim2*(i3)))]

        real *opXp = opX.Array_Descriptor.Array_View_Pointer3;
        const int opXDim0=opX.getRawDataSize(0);
        const int opXDim1=opX.getRawDataSize(1);
        const int opXDim2=opX.getRawDataSize(2);
        #undef OPXS
        #define OPXS(i0,i1,i2,i3) opXp[i0+opXDim0*(i1+opXDim1*(i2+opXDim2*(i3)))]

        real *opYp = opY.Array_Descriptor.Array_View_Pointer3;
        const int opYDim0=opY.getRawDataSize(0);
        const int opYDim1=opY.getRawDataSize(1);
        const int opYDim2=opY.getRawDataSize(2);
        #undef OPYS
        #define OPYS(i0,i1,i2,i3) opYp[i0+opYDim0*(i1+opYDim1*(i2+opYDim2*(i3)))]

        real *opZp = opZ.Array_Descriptor.Array_View_Pointer3;
        const int opZDim0=opZ.getRawDataSize(0);
        const int opZDim1=opZ.getRawDataSize(1);
        const int opZDim2=opZ.getRawDataSize(2);
        #undef OPZS
        #define OPZS(i0,i1,i2,i3) opZp[i0+opZDim0*(i1+opZDim1*(i2+opZDim2*(i3)))]

        n=0;
        if( !isRectangular )
	{ // multiply by the normal
          if( useNewVersion )
	  {
            if( numberOfDimensions==1 )
	    {
	      FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	      {
		OPXS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
	      }
	    }
	    else if( numberOfDimensions==2 )
	    {
	      FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	      {
		OPXS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
		OPYS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n2);
	      }
	    }
	    else
	    {
	      FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	      {
		OPXS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
		OPYS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n2);
		OPZS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n3);
	      }
	    }
	  }
	  else
	  {
	    ForStencil(m1,m2,m3)
	    {
	      OPX(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,n1);
	      if( numberOfDimensions>1 )
		OPY(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,n2);
	      if( numberOfDimensions>2 )
		OPZ(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,n3);
	    }
	  }

	}
	else
	{ // rectangular case:
	  if( side==0 ) 
	    opX*=-1.;   // multiply by the normal
	}
	
        if( numberOfComponentsForCoefficients > 1 )
          coeff(ME,I1m,I2m,I3m)=0.;  // zero out boundary equation

	if( bcType==BCTypes::neumann )
	  { 
	  if( numberOfDimensions==1 || rectangular )
  	    coeff(M0,I1m,I2m,I3m)=opX;
	  else if( numberOfDimensions==2 )
  	    coeff(M0,I1m,I2m,I3m)=opX+opY;
	  else if( numberOfDimensions==3 )
  	    coeff(M0,I1m,I2m,I3m)=opX+opY+opZ;

	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	    for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
              if( c!=c0 || ee!=e0 )
		coeff(M+CE(c,ee),I1m,I2m,I3m)=coeff(M0,I1m,I2m,I3m);
	}
	else
	{
	  // mixed BC: alpha*u + beta*u.n
          real alpha, beta;
	  //kkc 060816 sometimes alpha and beta are specified for each side
	  if ( bcParams.a.getLength(1)>1 && bcParams.a.getLength(0)>=2 )
          {
	    alpha = bcParams.a(0,side,axis);
	    beta  = bcParams.a(1,side,axis);
	  }
          else if( bcParams.a.getLength(0)>=2 )
	  {
	    alpha = bcParams.a(0);
	    beta  = bcParams.a(1);
	  }
	  else
	  {
	    printf("MappedGridOperators::applyBoundaryConditionCoefficients ERROR applying mixed BC\n");
	    printf(" The coefficients for `a' must be set in the BoundaryConditionParameters\n");
	    exit(1);
	  }

          assignCoefficients(identityOperator,coeff,I1m,I2m,I3m,e0,c0);
	  if( numberOfDimensions==1 || rectangular )
	    coeff(M0,I1m,I2m,I3m)=beta*opX+alpha*coeff(M0,I1m,I2m,I3m);
	  else if( numberOfDimensions==2 )
	    coeff(M0,I1m,I2m,I3m)=beta*(opX+opY)+alpha*coeff(M0,I1m,I2m,I3m);
	  else
	    coeff(M0,I1m,I2m,I3m)=beta*(opX+opY+opZ)+alpha*coeff(M0,I1m,I2m,I3m);

	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	    for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
              if( c!=c0 || ee!=e0 )
		coeff(M+CE(c,ee),I1m,I2m,I3m)=coeff(M0,I1m,I2m,I3m);
	}

        // fix up equation numbers -- stencil is centred around the boundary point
	for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    ForStencil(m1,m2,m3)  
	      sparse.setCoefficientIndex(M123CE(m1,m2,m3,c,ee), ee,J1m,J2m,J3m, c,(J1+m1),(J2+m2),(J3+m3) );  
	  }
	  sparse.setClassify(SparseRepForMGF::ghost1,J1m,J2m,J3m,ee);
	}
        // sparse.classify.display("Here is classify after applyBoundaryConditionCoefficients");
	break;
	
      }
      
      case normalDotScalarGrad:
      {

	realSerialArray  opX,opY,opZ;
	// *************** normalDotScalarGrad *******************
	opX.redim(Mv,   // holds a gradient (i.e. a vector)
		  Range(I1.getBase(),I1.getBound()),
		  Range(I2.getBase(),I2.getBound()),
		  Range(I3.getBase(),I3.getBound()));

        if( bcParams.getVariableCoefficients(grid)==0 )
	{
	  printf("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR: The BoundaryConditionParameters \n"
                 " do not have a variableCoefficient grid function defined in them. \n"
                 "The normalDotScalarGrad boundary condition requires a grid function to use for the scalar.\n");
	  Overture::abort("error");
	}
        #ifndef USE_PPP
          RealMappedGridFunction & scalar = *bcParams.getVariableCoefficients(grid);
          opX=scalarGradCoefficients(scalar,I1,I2,I3,0,0)(Mv,I1,I2,I3);
        #else
	  Overture::abort("ERROR: finish this Bill!");
	#endif

        // for( axis=0; axis<numberOfDimensions; axis++ )
        if( useNewVersion )
	{
          real *normalp = normal.Array_Descriptor.Array_View_Pointer3;
          const int normalDim0=normal.getRawDataSize(0);
          const int normalDim1=normal.getRawDataSize(1);
          const int normalDim2=normal.getRawDataSize(2);
          #undef NORMAL
          #define NORMAL(i0,i1,i2,i3) normalp[i0+normalDim0*(i1+normalDim1*(i2+normalDim2*(i3)))]
  
          real *opXp = opX.Array_Descriptor.Array_View_Pointer3;
          const int opXDim0=opX.getRawDataSize(0);
          const int opXDim1=opX.getRawDataSize(1);
          const int opXDim2=opX.getRawDataSize(2);
          #undef OPXS
          #define OPXS(i0,i1,i2,i3) opXp[i0+opXDim0*(i1+opXDim1*(i2+opXDim2*(i3)))]

          const int mv1=stencilSize, mv2=stencilSize*2;
	  if( numberOfDimensions==1 )
	  {
	    FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	    {
	      OPXS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
	    }
	  }
	  else if( numberOfDimensions==2 )
	  {
	    FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	    {
	      OPXS(m    ,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
	      OPXS(m+mv1,i1,i2,i3)*=NORMAL(i1,i2,i3,n2);
	    }
	  }
	  else
	  {
	    FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	    {
	      OPXS(m    ,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
	      OPXS(m+mv1,i1,i2,i3)*=NORMAL(i1,i2,i3,n2);
	      OPXS(m+mv2,i1,i2,i3)*=NORMAL(i1,i2,i3,n3);
	    }
	  }

	}
	else
	{
	  ForStencil(m1,m2,m3)
	  {
	    OPX(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,axis1);
	    if( numberOfDimensions>1 )
	      OPX(m1,m2,m3,n+1,I1,I2,I3)*=normal(I1,I2,I3,axis2);
	    if( numberOfDimensions>2 )
	      OPX(m1,m2,m3,n+2,I1,I2,I3)*=normal(I1,I2,I3,axis3);
	  }
	}
	
        if( numberOfComponentsForCoefficients > 1 )
          coeff(ME,I1m,I2m,I3m)=0.;  // zero out boundary equation

	if( numberOfDimensions==1 )
	  coeff(M0,I1m,I2m,I3m)=opX(M0,I1,I2,I3);
	else if( numberOfDimensions==2 )
	  coeff(M0,I1m,I2m,I3m)=opX(M0,I1,I2,I3)+opX(M0+stencilSize,I1,I2,I3);
	else if( numberOfDimensions==3 )
	  coeff(M0,I1m,I2m,I3m)=opX(M0,I1,I2,I3)+opX(M0+stencilSize,I1,I2,I3)+opX(M0+2*stencilSize,I1,I2,I3);

        for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    ForStencil(m1,m2,m3)  
	      sparse.setCoefficientIndex(M123CE(m1,m2,m3,c,ee), ee,J1m,J2m,J3m, c,(J1+m1),(J2+m2),(J3+m3) );  
	  }
	  sparse.setClassify(SparseRepForMGF::ghost1,J1m,J2m,J3m,ee);
	}
        // sparse.classify.display("Here is classify after applyBoundaryConditionCoefficients");

	
	}
	break;

      case normalComponent:

        if( numberOfComponentsForCoefficients < numberOfDimensions || C.length()!=numberOfDimensions )
	{
	  cout << "applyBoundaryConditionCoefficients:ERROR: Cannot apply a normalComponent boundary condition\n";
          if(  C.length()!=numberOfDimensions )
            printf("The number of components (length of argument `C') must equal the number of space dimensions\n");
	  else
            printf("numberOfComponentsForCoefficients < numberOfDimensions\n");
	  Overture::abort("error");
	}
	if( saveBoundaryPoints )
	{
          // save coefficients so we can reset some points where the BC should NOT be applied
          resetBoundaryPoints=TRUE;  // we reset the points after the switch statement
          coeffSave.redim(ME,I1,I2,I3);
          coeffSave=coeff(ME,I1,I2,I3);  
	}
	
        coeff(ME,I1,I2,I3)=0.; // set all coefficients to zero.
        normal.reshape(1,normal.dimension(0),normal.dimension(1),normal.dimension(2),normal.dimension(3));
	m1=m2=m3=0;
	for( c=C.getBase(); c<=C.getBound(); c++ )                        
	{
	  for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	  {
            coeff(M123CE(m1,m2,m3,c,ee),I1,I2,I3)=normal(0,I1,I2,I3,c-C.getBase());
	  }
	}
        normal.reshape(normal.dimension(1),normal.dimension(2),normal.dimension(3),normal.dimension(4));
	break;
      case tangentialComponent0:
      case tangentialComponent1:
	if( saveBoundaryPoints )
	{
          // save coefficients so we can reset some points where the BC should NOT be applied
          resetBoundaryPoints=TRUE;  // we reset the points after the switch statement
          coeffSave.redim(ME,I1,I2,I3);
          coeffSave=coeff(ME,I1,I2,I3);  
	}

	nt= bcType==BCTypes::tangentialComponent0 ? 0 : 1;
	ndt=nt*numberOfDimensions;  // for indexing into the tangent array (last two components are merged)
	if( numberOfDimensions==1 )
	{
	  cout << "applyBoundaryConditionCoefficients::ERROR: cannot apply tangentialComponent" << nt << " BC in 1D\n";
	  Overture::abort("error");
	}
        if( numberOfComponentsForCoefficients < numberOfDimensions || C.length()!=numberOfDimensions )
	{
	  cout << "applyBoundaryConditionCoefficients:ERROR: Cannoy apply a normalComponent boundary condition\n";
	  Overture::abort("error");
	}
	
        coeff(ME,I1,I2,I3)=0.; // set all coefficients to zero.
        tangent.reshape(1,tangent.dimension(0),tangent.dimension(1),tangent.dimension(2),tangent.dimension(3));
	for( c=C.getBase(); c<=C.getBound(); c++ )                        
	{
	  for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	  {
	    m1=m2=m3=0;
            coeff(M123CE(m1,m2,m3,c,ee),I1,I2,I3)=tangent(0,I1,I2,I3,c-C.getBase()+ndt);
	  }
	}
        tangent.reshape(tangent.dimension(1),tangent.dimension(2),tangent.dimension(3),tangent.dimension(4));
	break;

      case generalMixedDerivative:
      {
	if( bcParams.a.getLength(0)<numberOfDimensions+1 )
	{
	  printf("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR applying the "
		 "generalMixedDerivative BC\n");
	  printf(" The coefficients for `a' must be set in the BoundaryConditionParameters\n");
	  exit(1);
	}
        b0=bcParams.a(0);
	b1=bcParams.a(1);
	b2= numberOfDimensions>1 ? bcParams.a(2) : 0.;
	b3= numberOfDimensions>2 ? bcParams.a(3) : 0.;        if( numberOfComponentsForCoefficients > 1 )

        if( numberOfComponentsForCoefficients > 1 )
          coeff(ME,I1m,I2m,I3m)=0.;  // zero out boundary equation

	realSerialArray  opX; 
	opX.redim(Range(M.getBase(),M.getBound()),   
		  Range(I1.getBase(),I1.getBound()),
		  Range(I2.getBase(),I2.getBound()),
		  Range(I3.getBase(),I3.getBound()));

        opX=0.;  // *wdh* 060403
	assignCoefficients(identityOperator,opX,I1,I2,I3,0,0);
        coeff(M0,I1m,I2m,I3m)=b0*opX(M,I1,I2,I3);

	assignCoefficients(xDerivative,opX,I1,I2,I3,0,0);
        coeff(M0,I1m,I2m,I3m)+=b1*opX;

	assignCoefficients(yDerivative,opX,I1,I2,I3,0,0);
        coeff(M0,I1m,I2m,I3m)+=b2*opX;
	if( numberOfDimensions==3 )
	{
	  assignCoefficients(zDerivative,opX,I1,I2,I3,0,0);
	  coeff(M0,I1m,I2m,I3m)+=b3*opX;
	}
	

// 	if( numberOfDimensions==2 )
// 	{
// 	  coeff(M0,I1m,I2m,I3m)=b0*identityCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3) 
//                             + b1*xCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3)
// 	                    + b2*yCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
// 	}
// 	else
// 	{
// 	  coeff(M0,I1m,I2m,I3m)=b0*identityCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3) 
//                 + b1*xCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3)
// 	        + b2*yCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3)
//                 + b3*zCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
// 	}
	for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	    if( c!=c0 || ee!=e0 )
	      coeff(M+CE(c,ee),I1m,I2m,I3m)=coeff(M0,I1m,I2m,I3m);
        for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    ForStencil(m1,m2,m3)  
	      sparse.setCoefficientIndex(M123CE(m1,m2,m3,c,ee), ee,J1m,J2m,J3m, c,(J1+m1),(J2+m2),(J3+m3) );  
	  }
	  sparse.setClassify(SparseRepForMGF::ghost1,J1m,J2m,J3m,ee);
	}
	break;
      }
      case aDotU:
	if( saveBoundaryPoints )
	{
          // save coefficients so we can reset some points where the BC should NOT be applied
          resetBoundaryPoints=TRUE;  // we reset the points after the switch statement
          coeffSave.redim(ME,I1,I2,I3);
          coeffSave=coeff(ME,I1,I2,I3);  
	}
	if( bcParams.a.getLength(0)<numberOfDimensions )
	{
	  printf("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR applying the aDotU BC\n");
	  printf(" The coefficients for `a' must be set in the BoundaryConditionParameters\n");
	  exit(1);
	}
	a1=bcParams.a(0);
	a2= numberOfDimensions>1 ? bcParams.a(1) : 0.;
	a3= numberOfDimensions>2 ? bcParams.a(2) : 0.;
	aNorm=a1*a1+a2*a2+a3*a3;
	if( aNorm==0. )
	{
	  printf("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR applying a aDotU BC\n");
	  printf(" The coefficients for `a' are all zero! side=%i, axis=%i\n",side,axis);
	  exit(1);
	}
	aNorm=1./SQRT(aNorm); 
	a1*=aNorm;  a2*=aNorm;  a3*=aNorm;  // Normalize "a"
        coeff(ME,I1,I2,I3)=0.; // set all coefficients to zero.
	m1=m2=m3=0;
	for( c=C.getBase(); c<=C.getBound(); c++ )                        
	{
	  for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	  {
            coeff(M123CE(m1,m2,m3,c,ee),I1,I2,I3)=a[c-C.getBase()];
	  }
	}
	break;
      case aDotGradU:
        cout << "applyBoundaryConditionCoefficients: aDotGradU boundary condition is not implemented yet\n";
        break;
      case evenSymmetry:
      case oddSymmetry:
	// u(ghost)=u(-ghost)
	if( ghostLineToAssign > 3 || ghostLineToAssign < 0 )
	  cout << "applyBoundaryConditionCoefficients::ERROR? extrapolating ghost line " << ghostLineToAssign << endl; 

        getGhostIndex( mg.indexRange(),side,axis,I1m,I2m,I3m, ghostLineToAssign,bcParams.extraInTangentialDirections);
        getGhostIndex( mg.indexRange(),side,axis,I1p,I2p,I3p,-ghostLineToAssign,bcParams.extraInTangentialDirections);

        // --> fixed 100413 *wdh* : 
	// Remember the full size Index's: 
	J1m=I1m, J2m=I2m, J3m=I3m;
	J1p=I1p, J2p=I2p, J3p=I3p;
	ok=getLocalIndex(mg.mask(),maskLocal,I1m,I2m,I3m,I1m,I2m,I3m); 
	if( !ok ) continue;  // there are no points on this processor
	ok=getLocalIndex(mg.mask(),maskLocal,I1p,I2p,I3p,I1p,I2p,I3p); 
        // <--- 

	coeff(ME,I1m,I2m,I3m)=0.;
	coeff(0,I1m,I2m,I3m)= 1.;
        if( bcType==BCTypes::BCNames(evenSymmetry) )
   	  coeff(1,I1m,I2m,I3m)=-1.;
        else
   	  coeff(1,I1m,I2m,I3m)=+1.;
	  
	for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	    if( c!=c0 || ee!=e0 )
	      coeff(M+CE(c,ee),I1m,I2m,I3m)=coeff(M0,I1m,I2m,I3m);
        for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    sparse.setCoefficientIndex(CE(c,ee)+0, ee,J1m,J2m,J3m, c,J1m,J2m,J3m );  
	    sparse.setCoefficientIndex(CE(c,ee)+1, ee,J1m,J2m,J3m, c,J1p,J2p,J3p );  
	  }
  	  sparse.setClassify(SparseRepForMGF::extrapolation,J1m,J2m,J3m,ee);
	}
        break;
      case vectorSymmetry:
      {
        //
	// Apply a symmetry condition to a vector u=(u1,u2,u3)
	//    n.u is odd  
	//    t.u is even 
        assert( ghostLineToAssign==1 );
	
        if( C.length()!=numberOfDimensions || E.length()!=numberOfDimensions )
	{
          printf("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR applying a vectorSymmetry BC\n");
	  printf(" C.length()!=numberOfDimensions || E.length()!=numberOfDimensions\n");
	  Overture::abort("error");
	}
	coeff(ME,I1m,I2m,I3m)=0.;
        normal.reshape(1,normal.dimension(0),normal.dimension(1),normal.dimension(2),normal.dimension(3));

        // The last two dimensions of the tangent are merged
        assert( tangent.getLength(3) == numberOfDimensions*(numberOfDimensions-1) );
        tangent.reshape(1,tangent.dimension(0),tangent.dimension(1),tangent.dimension(2),tangent.dimension(3));
        
	if( false )
	{
          // old way: this could result in a zero diagonal element
  	  //    n.u is odd  (equation 0)
	  //    t.u is even  (equation 1,[2])
	  m1=m2=m3=0;
	  for( c=C.getBase(); c<=C.getBound(); c++ )  
	  {
	    mm[axis]=2*side-1;  // note: mm[0:2] == {m1,m2,m3}
	    coeff(M123CE(m1,m2,m3,c,e0),I1m,I2m,I3m)= normal(0,I1,I2,I3,c-C.getBase());
	    coeff(M123CE(m1,m2,m3,c,e1),I1m,I2m,I3m)=tangent(0,I1,I2,I3,c-C.getBase());
	    if( numberOfDimensions==3 )
	      coeff(M123CE(m1,m2,m3,c,e2),I1m,I2m,I3m)=tangent(0,I1,I2,I3,c+3-C.getBase());
	    mm[axis]=-mm[axis];
	    coeff(M123CE(m1,m2,m3,c,e0),I1m,I2m,I3m)=  normal(0,I1,I2,I3,c-C.getBase());
	    coeff(M123CE(m1,m2,m3,c,e1),I1m,I2m,I3m)=-tangent(0,I1,I2,I3,c-C.getBase());
	    if( numberOfDimensions==3 )
	      coeff(M123CE(m1,m2,m3,c,e2),I1m,I2m,I3m)=-tangent(0,I1,I2,I3,c+3-C.getBase());
	  }
	}
	else
	{
          // new way: this will always have a diagonal element (of 1)
          // equation d:  u_d(-1) + (n.u)(1) n_d - (t^1.u)(1) t^1_d - (t^2.u)(1) t^2_d = 0 
          // printf(" ***** applyBCC: fill in coeff for vectorSymmetry *****\n");
	  
          m1=m2=m3=0;
          mm[axis]=2*side-1;  // note: mm[0:2] == {m1,m2,m3}
          const int c1=c0+1, c2=c0+2;
          if( numberOfDimensions==2 )
	  {
            for( int d=0; d<numberOfDimensions; d++ )
	    { // assign equation e0+d: 
              // coeff of the ghost point (m1,m2,m3) is for u_d:  
	      coeff(M123CE( m1, m2, m3,c0+d,e0+d),I1m,I2m,I3m)= 1.;
              // coeff's of the first line in -(m1,m2,m3) for u_m, m=c0,c1
	      coeff(M123CE(-m1,-m2,-m3,c0,e0+d),I1m,I2m,I3m)=(  normal(0,I1,I2,I3,0)* normal(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,0)*tangent(0,I1,I2,I3,d));
	      coeff(M123CE(-m1,-m2,-m3,c1,e0+d),I1m,I2m,I3m)=(  normal(0,I1,I2,I3,1)* normal(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,1)*tangent(0,I1,I2,I3,d));
	    }
	  }
	  else if( numberOfDimensions==3 )
	  {
            for( int d=0; d<numberOfDimensions; d++ )
	    { // assign equation e0+d: 
              // coeff of the ghost point (m1,m2,m3) is for u_d: 
	      coeff(M123CE( m1, m2, m3,c0+d,e0+d),I1m,I2m,I3m)= 1.;
              // coeff's of the first line in -(m1,m2,m3) for u_m, m=c0,c1,c2
	      coeff(M123CE(-m1,-m2,-m3,c0,e0+d),I1m,I2m,I3m)=(  normal(0,I1,I2,I3,0)* normal(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,0)*tangent(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,3)*tangent(0,I1,I2,I3,3+d));
	      coeff(M123CE(-m1,-m2,-m3,c1,e0+d),I1m,I2m,I3m)=(  normal(0,I1,I2,I3,1)* normal(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,1)*tangent(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,4)*tangent(0,I1,I2,I3,3+d));
	      coeff(M123CE(-m1,-m2,-m3,c2,e0+d),I1m,I2m,I3m)=(  normal(0,I1,I2,I3,2)* normal(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,2)*tangent(0,I1,I2,I3,d)
							      -tangent(0,I1,I2,I3,5)*tangent(0,I1,I2,I3,3+d));
	    }
	  }
	  else
	  { // in 1D: u(-1) = -u(1) 
            coeff(M123CE( m1, m2, m3,c0,e0),I1m,I2m,I3m)= 1.;
            coeff(M123CE(-m1,-m2,-m3,c0,e0),I1m,I2m,I3m)= 1.;
	  }
	}
	
        for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    ForStencil(m1,m2,m3)  
	      sparse.setCoefficientIndex(M123CE(m1,m2,m3,c,ee), ee,J1m,J2m,J3m, c,(J1+m1),(J2+m2),(J3+m3) );  
	  }
	  sparse.setClassify(SparseRepForMGF::ghost1,J1m,J2m,J3m,ee);
	}
        normal.reshape(normal.dimension(1),normal.dimension(2),normal.dimension(3),normal.dimension(4));
        tangent.reshape(tangent.dimension(1),tangent.dimension(2),tangent.dimension(3),tangent.dimension(4));
        break;
      }
      
      case generalizedDivergence:
      {
        //   \av\cdot\uv = a(0) u(0)_x + a(1) u(1)_y + a(2) u(2)_z = g
	if( bcParams.a.getLength(0) >= numberOfDimensions )
	{
	  b1=bcParams.a(0);
	  b2= numberOfDimensions>1 ? bcParams.a(1) : 0.;
	  b3= numberOfDimensions>2 ? bcParams.a(2) : 0.;
	}
	else
	{
	  b1=b2=b3=1.;  // default values
	}
	if( fabs(b1)==0. && (numberOfDimensions<2 || fabs(b2)==0.) && (numberOfDimensions<3 || fabs(b3)==0.) )
	{
	  printf("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR applying generalizedDivergence BC\n");
	  printf(" The elements of a in generalizedDivergence are all zero!\n");
	  Overture::abort("MappedGridOperators::applyBoundaryConditions:ERROR applying generalizedDivergence BC");
	}
        if( C.length()!=numberOfDimensions )
	{
          printf("MappedGridOperators::applyBoundaryConditionCoefficients:ERROR applying generalizedDivergence BC\n");
          printf("C.length()!=numberOfDimensions\n");
          Overture::abort("error");
	}
	realSerialArray  opX,opY,opZ;
	opX.redim(Range(M.getBase(),M.getBound()),   
		  Range(I1.getBase(),I1.getBound()),
		  Range(I2.getBase(),I2.getBound()),
		  Range(I3.getBase(),I3.getBound()));

        opX=0.;  // *wdh* 060403
	assignCoefficients(xDerivative,opX,I1,I2,I3,0,0);

	if( numberOfDimensions>=2 )
	{
	  opY.redim(opX); 
          opY=0.;  // *wdh* 060403
	  assignCoefficients(yDerivative,opY,I1,I2,I3,0,0);
	}
	
	if( numberOfDimensions==3 )
	{
	  opZ.redim(opX); 
          opZ=0.;  // *wdh* 060403
	  assignCoefficients(zDerivative,opZ,I1,I2,I3,0,0);
	}

        coeff(ME,I1m,I2m,I3m)=0.; // set all coefficients to zero.
	m1=m2=m3=0;
        for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
          c=C.getBase();
          if( b1==1. )
            coeff(M+CE(c,ee),I1m,I2m,I3m)=opX(M,I1,I2,I3);
          else if( b1!=0. )
            coeff(M+CE(c,ee),I1m,I2m,I3m)=b1*opX(M,I1,I2,I3);
          if( numberOfDimensions>1 )
	  {
            c++;
            if( b2==1. )
              coeff(M+CE(c,ee),I1m,I2m,I3m)=opY(M,I1,I2,I3);
            else if( b2!=0. )
              coeff(M+CE(c,ee),I1m,I2m,I3m)=b2*opY(M,I1,I2,I3);
	  }
          if( numberOfDimensions>2 )
	  {
            c++;
            if( b3==1. )
              coeff(M+CE(c,ee),I1m,I2m,I3m)=opZ(M,I1,I2,I3);
            else
              coeff(M+CE(c,ee),I1m,I2m,I3m)=b3*opZ(M,I1,I2,I3);
	  }
	}
        // fix up equation numbers:
        for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    ForStencil(m1,m2,m3)  
	      sparse.setCoefficientIndex(M123CE(m1,m2,m3,c,ee), ee,J1m,J2m,J3m, c,(J1+m1),(J2+m2),(J3+m3) );  
	  }
	  sparse.setClassify(SparseRepForMGF::ghost1,J1m,J2m,J3m,ee);
	}
        break;
      }
      
      case normalDerivativeOfADotU:
        cout << "applyBoundaryConditionCoefficients: normalDerivativeOfADotU: BC is not implemented yet\n";
	break;
      case normalDerivativeOfNormalComponent:
      case normalDerivativeOfTangentialComponent0:
      case normalDerivativeOfTangentialComponent1:
      {
	realSerialArray  opX,opY,opZ;

	if( bcType==BCTypes::normalDerivativeOfNormalComponent )
	{
	  v0=0; v1=1; v2=2;
	}
	else
	{
	  nt= bcType==BCTypes::normalDerivativeOfTangentialComponent0 ? 0 : 1;
	  v0=nt*numberOfDimensions; v1=v0+1; v2=v1+1;
	  if( numberOfDimensions<2+nt )
	  {
	    cout << "applyBoundaryCondition::ERROR: cannot apply extrapolateTangentialComponent" << nt 
		 << " BC in " << numberOfDimensions << "D\n";
	    Overture::abort("error");
	  }
	}

	opX.redim(Range(M.getBase(),M.getBound()),            // dimension (to get base correct)
		  Range(I1.getBase(),I1.getBound()),
		  Range(I2.getBase(),I2.getBound()),
		  Range(I3.getBase(),I3.getBound()));
	// opX=xCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
        opX=0.;  // *wdh* 060403
        assignCoefficients(xDerivative,opX,I1,I2,I3,0,0);
	if( numberOfDimensions>1 )
	{
	  opY.redim(opX);
          opY=0.;  // *wdh* 060403
          assignCoefficients(yDerivative,opY,I1,I2,I3,0,0);
	}
	if( numberOfDimensions>2 )
	{
	  opZ.redim(opX);
          opZ=0.;  // *wdh* 060403
	  assignCoefficients(zDerivative,opZ,I1,I2,I3,0,0);
	}

        n=0;

        real *normalp = normal.Array_Descriptor.Array_View_Pointer3;
        const int normalDim0=normal.getRawDataSize(0);
        const int normalDim1=normal.getRawDataSize(1);
        const int normalDim2=normal.getRawDataSize(2);
        #undef NORMAL
        #define NORMAL(i0,i1,i2,i3) normalp[i0+normalDim0*(i1+normalDim1*(i2+normalDim2*(i3)))]

        real *opXp = opX.Array_Descriptor.Array_View_Pointer3;
        const int opXDim0=opX.getRawDataSize(0);
        const int opXDim1=opX.getRawDataSize(1);
        const int opXDim2=opX.getRawDataSize(2);
        #undef OPXS
        #define OPXS(i0,i1,i2,i3) opXp[i0+opXDim0*(i1+opXDim1*(i2+opXDim2*(i3)))]

        real *opYp = opY.Array_Descriptor.Array_View_Pointer3;
        const int opYDim0=opY.getRawDataSize(0);
        const int opYDim1=opY.getRawDataSize(1);
        const int opYDim2=opY.getRawDataSize(2);
        #undef OPYS
        #define OPYS(i0,i1,i2,i3) opYp[i0+opYDim0*(i1+opYDim1*(i2+opYDim2*(i3)))]

        real *opZp = opZ.Array_Descriptor.Array_View_Pointer3;
        const int opZDim0=opZ.getRawDataSize(0);
        const int opZDim1=opZ.getRawDataSize(1);
        const int opZDim2=opZ.getRawDataSize(2);
        #undef OPZS
        #define OPZS(i0,i1,i2,i3) opZp[i0+opZDim0*(i1+opZDim1*(i2+opZDim2*(i3)))]

        n=0;
	if( useNewVersion )
	{
	  if( numberOfDimensions==1 )
	  {
	    FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	    {
	      OPXS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
	    }
	  }
	  else if( numberOfDimensions==2 )
	  {
	    FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	    {
	      OPXS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
	      OPYS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n2);
	    }
	  }
	  else
	  {
	    FOR_4D(m,i1,i2,i3,M,I1,I2,I3)
	    {
	      OPXS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n1);
	      OPYS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n2);
	      OPZS(m,i1,i2,i3)*=NORMAL(i1,i2,i3,n3);
	    }
	  }
	}
	else
	{
	  ForStencil(m1,m2,m3)
	  {
	    OPX(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,n1);
	    if( numberOfDimensions>1 )
	      OPY(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,n2);
	    if( numberOfDimensions>2 )
	      OPZ(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,n3);
	  }
	}

// 	ForStencil(m1,m2,m3)
// 	{
// 	  OPX(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,axis1);
// 	  if( numberOfDimensions>1 )
//   	    OPY(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,axis2);
// 	  if( numberOfDimensions>2 )
// 	    OPZ(m1,m2,m3,n,I1,I2,I3)*=normal(I1,I2,I3,axis3);
// 	}


        if( numberOfComponentsForCoefficients > 1 )
          coeff(ME,I1m,I2m,I3m)=0.;  // zero out boundary equation

        // The last two dimensions of the tangent are merged
        if( (bcType==BCTypes::normalDerivativeOfTangentialComponent0 ||
             bcType==BCTypes::normalDerivativeOfTangentialComponent1 ) &&
             vect.getLength(3) != numberOfDimensions*(numberOfDimensions-1) )
	{
	  printf("applyBCC:ERROR: vect.getLength(3)=%i != numberOfDimensions*(numberOfDimensions-1)=%i\n"
                 "   vect.getLength(4)=%i\n",
		 vect.getLength(3),numberOfDimensions*(numberOfDimensions-1),vect.getLength(4));
	  Overture::abort("error");
	}
	
	vect.reshape(1,vect.dimension(0),vect.dimension(1),vect.dimension(2),vect.dimension(3));

	if( numberOfDimensions==2 )
          opX(M,I1,I2,I3)=opX(M,I1,I2,I3)+opY(M,I1,I2,I3);
        else
          opX(M,I1,I2,I3)=opX(M,I1,I2,I3)+opY(M,I1,I2,I3)+opZ(M,I1,I2,I3);

	ForStencil(m1,m2,m3)
	{
	  coeff(M123CE(m1,m2,m3,c0,e0),I1m,I2m,I3m)=vect(0,I1,I2,I3,v0)*opX(M123(m1,m2,m3),I1,I2,I3);
	  if( numberOfDimensions>1 )
	    coeff(M123CE(m1,m2,m3,c0+1,e0),I1m,I2m,I3m)=vect(0,I1,I2,I3,v1)*opX(M123(m1,m2,m3),I1,I2,I3);
	  if( numberOfDimensions>2 )
	    coeff(M123CE(m1,m2,m3,c0+2,e0),I1m,I2m,I3m)=vect(0,I1,I2,I3,v2)*opX(M123(m1,m2,m3),I1,I2,I3);
	}
	
	vect.reshape(vect.dimension(1),vect.dimension(2),vect.dimension(3),vect.dimension(4));

        for( ee=E.getBase()+1; ee<=E.getBound(); ee++ )                        
	{
          for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
            coeff(M0+CE(c,ee),I1m,I2m,I3m)=coeff(M0+CE(c,e0),I1m,I2m,I3m);
	  }
        }
        // fix up equation numbers:
        for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    ForStencil(m1,m2,m3)  
	      sparse.setCoefficientIndex(M123CE(m1,m2,m3,c,ee), ee,J1m,J2m,J3m, c,(J1+m1),(J2+m2),(J3+m3) );  
	  }
	  sparse.setClassify(SparseRepForMGF::ghost1,J1m,J2m,J3m,ee);
	}
	break;
      }
      
      case tangentialComponent:
        cout << "applyBoundaryConditionCoefficients: tangentialComponent boundary condition is not implemented yet\n";
	if( saveBoundaryPoints )
	{
          // save coefficients so we can reset some points where the BC should NOT be applied
          resetBoundaryPoints=TRUE;  // we reset the points after the switch statement
          coeffSave.redim(ME,I1,I2,I3);
          coeffSave=coeff(ME,I1,I2,I3);  
	}
        break;

      case extrapolate:
      {
	if( ghostLineToAssign > 3 || ghostLineToAssign < 0 )
	  cout << "applyBoundaryConditionCoefficients::ERROR? extrapolating ghost line " << ghostLineToAssign << endl; 
	getGhostIndex( mg.indexRange(),side,axis,I1e,I2e,I3e,ghostLineToAssign,bcParams.extraInTangentialDirections); 

        int orderOfExtrap=bcParams.orderOfExtrapolation<0 ? orderOfAccuracy+1 : bcParams.orderOfExtrapolation;
	const int nx=mg.gridIndexRange(End,axis)-mg.gridIndexRange(Start,axis)+1;
	if( orderOfExtrap >= nx+ghostLineToAssign )
	{
	  // reduce order of extrap if we only have a few grid points
	  // The extrap formula uses "orderOfExtrap+1" grid points -- we should not be coupled to the ghost
	  //   points on the opposite boundary
	  orderOfExtrap = nx+ghostLineToAssign-1;
	  printf("MappedGridOperators::extrapolate: WARNING: reducing order of extrapolation to %i "
		 "since number of grid points =%i\n",orderOfExtrap,nx);
	}

        for( ee=E.getBase(); ee<=E.getBound(); ee++ ) // ****** fix this for c and e *****
          setExtrapolationCoefficients(uCoeff,ee,I1e,I2e,I3e,orderOfExtrap);
        break;
      }
      case extrapolateNormalComponent:
      case extrapolateTangentialComponent0:
      case extrapolateTangentialComponent1:
      {
        int orderOfExtrap=bcParams.orderOfExtrapolation<0 ? orderOfAccuracy+1 : bcParams.orderOfExtrapolation;
	if( orderOfExtrap*numberOfDimensions >= stencilSize*numberOfComponentsForCoefficients )
	{
	  cout << "applyBoundaryConditionCoefficients::ERROR: in extrapolate[Normal/Tangential]Component\n";
          cout << " There is not enough room in the stencil to hold this order of extrapolation\n";
          cout << " orderOfExtrapolation*numberOfDimensions >= stencilSize*numberOfComponentsForCoefficients \n";
	  Overture::abort("error");
	}
	if( ghostLineToAssign > 3 || ghostLineToAssign < 0 )
	  cout << "applyBoundaryConditionCoefficients::ERROR? extrapolating ghost line " << ghostLineToAssign << endl; 
        // line to extrapolate:
	getGhostIndex(mg.indexRange(),side,axis,I1e,I2e,I3e,ghostLineToAssign,bcParams.extraInTangentialDirections);

	const int nx=mg.gridIndexRange(End,axis)-mg.gridIndexRange(Start,axis)+1;
	if( orderOfExtrap >= nx+ghostLineToAssign )
	{
	  // reduce order of extrap if we only have a few grid points
	  // The extrap formula uses "orderOfExtrap+1" grid points -- we should not be coupled to the ghost
	  //   points on the opposite boundary
	  orderOfExtrap = nx+ghostLineToAssign-1;
	  printf("MappedGridOperators::extrapolate: WARNING: reducing order of extrapolation to %i "
		 "since number of grid points =%i\n",orderOfExtrap,nx);
	}

        bool ok=getLocalIndex(mg.mask(),maskLocal,I1e,I2e,I3e, I1e,I2e,I3e); 
        if( !ok ) continue;  // there are no points on this processor

	coeff(ME,I1e,I2e,I3e)=0.;
	vect.reshape(1,vect.dimension(0),vect.dimension(1),vect.dimension(2),vect.dimension(3));
	// NOTE: store at the start of the equation (for Oges) ---
        dir[0]=dir[1]=dir[2]=0;
	dir[axis]=1-2*side;  // extrapolate in this direction
	for( ee=E.getBase(); ee<=E.getBound(); ee++ )  // *** there is probably an error if E.length()>1 !
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ ) 
	  {
	    for( int i=0; i<=orderOfExtrap; i++ )
	    {
	      coeff(i+CE(c,ee),I1e,I2e,I3e)=extrapCoeff[orderOfExtrap-1][i] * vect(0,I1,I2,I3,c-C.getBase()); //kkc 070831 add -C.getBase() so the code works when C is a range other than 0,1,2    
  	      sparse.setCoefficientIndex(i+CE(c,ee), ee,I1e,I2e,I3e, c,
						 (I1e+dir[0]*i),(I2e+dir[1]*i),(I3e+dir[2]*i) );  
	    }
	  }
	  sparse.setClassify(SparseRepForMGF::extrapolation,I1e,I2e,I3e,ee);
	}
	vect.reshape(vect.dimension(1),vect.dimension(2),vect.dimension(3),vect.dimension(4));

        break;
      }
      default:
	cout << "applyBoundaryConditionCoefficients: unknown or un-implemented boundary conditon = " << 
	  bcType << endl;
      } // end switch

      if( resetBoundaryPoints )
      {
	// reset some points where the BC should NOT be applied
        // we only need to worry about this for BC's that alter the point on the boundary

        #ifdef USE_PPP
          intSerialArray cmask; getLocalArrayWithGhostBoundaries(mg.mask(),cmask);
          intSerialArray bcMask; 
          getLocalArrayWithGhostBoundaries(((BoundaryConditionParameters&)bcParams).mask(),bcMask);
        #else
          const intSerialArray & cmask = mg.mask();
          const intSerialArray & bcMask = ((BoundaryConditionParameters&)bcParams).mask();
        #endif

        intSerialArray mask(I1,I2,I3);
	if( !bcParams.getUseMask() )
	{
          // we should NOT apply a BC at interpolation points or interiorBoundaryPoint
	  // mask=cmask(I1,I2,I3)>0 ||
  	  //     cmask(I1,I2,I3)< (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint);
          // *wdh* 990915 : use bit ops
	  mask=cmask(I1,I2,I3)>0 &&
            !( cmask(I1,I2,I3) & (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint));
	}
	else if( !(useWhereMaskOnBoundary[axis][side] && bcParams.useMixedBoundaryMask) )
	  mask=bcMask(I1,I2,I3);
	else
	{
          // we should NOT apply a BC at interpolation points or interiorBoundaryPoint
	  // mask=(cmask(I1,I2,I3)>0 ||
	  // cmask(I1,I2,I3)< (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint) ) &&
	  //   ((BoundaryConditionParameters&)bcParams).mask()(I1,I2,I3);
          // *wdh* 990915 : use bit ops
          mask=cmask(I1,I2,I3)>0 &&
            !( cmask(I1,I2,I3) & (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint)) &&
	    bcMask(I1,I2,I3);
	}

//         if( true )
// 	{
//           aString buff;
// 	  displayMask(mask,sPrintF(buff,"*** applyBCC grid=%i mask: mixed boundary is mask!=0 ***",grid));
// 	}
	
	mask.reshape(1,mask.dimension(0),mask.dimension(1),mask.dimension(2));
        where( !mask(0,I1,I2,I3) )
	{
	  for( int m=ME.getBase(); m<=ME.getBound(); m++ )
            coeff(m,I1,I2,I3)=coeffSave(m,I1,I2,I3);  
	}
	mask.reshape(mask.dimension(1),mask.dimension(2),mask.dimension(3));
      }

    } // if( mg.boundaryCondition(side,axis)==bc  
    
  }

//  tm(4)+=getCPU()-time; // keep track of the cpu time spent in this routine

}

#undef MERGE0
#undef M123
#undef M123N
#undef OPX
#undef OPY
#undef OPZ

