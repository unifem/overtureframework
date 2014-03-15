#include "Oges.h"
#ifdef GETLENGTH
#define GET_LENGTH dimension
#else
#define GET_LENGTH getLength
#endif

//=======================================================================
// Define discrete coefficients for Oges
//
//=======================================================================

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


#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilLength0*(n))

#define COEFF0(m1,m2,m3,n,I1,I2,I3) coeff0(M123N(m1,m2,m3,n),I1,I2,I3)
#define EQUATIONNUMBER0(m1,m2,m3,n,I1,I2,I3) equationNumber0(M123N(m1,m2,m3,n),I1,I2,I3)

// Use this for A++ index operations:
#undef  COEFF
#define COEFF(m1,m2,m3,n,I1,I2,I3) MERGE0(coeff0,M123N(m1,m2,m3,n),I1,I2,I3)
#undef  EQUATIONNUMBER
#define EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3) MERGE0(equationNumber0,M123N(m1,m2,m3,n),I1,I2,I3)
// Scalar indexing:
#define COEFFS(m1,m2,m3,n,I1,I2,I3) coeff0(M123N(m1,m2,m3,n),I1,I2,I3)
#define EQUATIONNUMBERS(m1,m2,m3,n,I1,I2,I3) equationNumber0(M123N(m1,m2,m3,n),I1,I2,I3)


// include "ogesux2.h"  // define 2nd order derivatives for vertex centred
// include "ogesux4.h"  // define 4th order derivatives for vertex centred

#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define ForStencil(m1,m2,m3)   \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2;  m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1;  m1++) 

#define ForStencilN(n,m1,m2,m3)   \
    for( n=0; n<numberOfComponents; n++) \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1; m1++) 

#define ForAllGridPoints( i1,i2,i3 ) \
  for( i3=c.dimension()(Start,axis3); i3<=c.dimension()(End,axis3); i3++ ) \
  for( i2=c.dimension()(Start,axis2); i2<=c.dimension()(End,axis2); i2++ ) \
  for( i1=c.dimension()(Start,axis1); i1<=c.dimension()(End,axis1); i1++ )

 
static int halfWidth1, halfWidth2;  // *** add to class ***

void Oges::
defineVertexDifferences( MappedGrid & c )
{
  //------------------------------------------------------------
  // Define some difference operators (in matrix form)
  // These are used in the include file ogesux2.h
  //------------------------------------------------------------
  printObsoleteMessage("defineVertexDifferences",1); 


  width=orderOfAccuracy+1;
  halfWidth=width/2;
  halfWidth1 = numberOfDimensions>0 ? halfWidth : 0;
  halfWidth2 = numberOfDimensions>1 ? halfWidth : 0;
  halfWidth3 = numberOfDimensions>2 ? halfWidth : 0;

  delta.redim(Range(-halfWidth,-halfWidth+width-1));    
  delta=0.;
  delta(0)=1.;

}


void Oges::
laplaceDirichletVertexNonConservative( const int grid )
{
  //================================================================
  // Define Laplace's equation with Dirichlet Boundary Conditions
  //   vertex centred, non conservation form
  //================================================================
  printObsoleteMessage("laplaceDirichletVertexNonConservative",1); 

  cout << "**********Oges::laplaceDirichletVertexNonConservative ********\n";

  RealDistributedArray & coeff0 = Oges::coeff[grid];

  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];

  int stencilLength0 = coeff0.GET_LENGTH(axis1);

  // Check to see if there is enough space in the arrays
  if( stencilLength0 < pow(3,numberOfDimensions)*numberOfComponents )
  {
    cerr << "laplaceDirichletVertexNonConservative:ERROR stencilLength0 is too small! " << endl;
    exit(1);
  }

  defineVertexDifferences( c );

  int m1,m2,m3,n;
  int side,axis;
  Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;

  coeff0=0.; // set all coefficients to zero

  Index I1,I2,I3,R[3];
  getIndex(c.indexRange(),I1,I2,I3);

  MappedGridOperators & op = operators[grid];   // operators for this grid

  // Assign equation Numbers - default values:
  ForStencilN(n,m1,m2,m3)  
    EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3)=
             equationNo(n,(I1+m1),(I2+m2),(I3+m3),grid);


  Index M(0,pow(width,numberOfDimensions));
  coeff0(M,I1,I2,I3)=op.laplacianCoefficients()(M,I1,I2,I3);
  ForBoundary(side,axis)
  {
    if( c.boundaryCondition()(side,axis) > 0 )
    {
      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

      cout << "discreteVertex: side,axis = " << side << "," << axis << endl;
      // op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3).display("Here is op.identityCoeff");
       
      coeff0(M,Ib1,Ib2,Ib3)=op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  // Dirichlet BC is the Identity operator

      // Apply equation on ghost line!
      getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points


      coeff0(M,Ig1,Ig2,Ig3)=op.laplacianCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  

      ForStencilN(n,m1,m2,m3)
      {
	//assign eqn numbers (different from default)
	EQUATIONNUMBER(m1,m2,m3,n,Ig1,Ig2,Ig3)=
	  equationNo(n,(Ib1+m1),(Ib2+m2),(Ib3+m3),grid);
      }
    }
  }
  

  if( debug & 128 )
  {
    coeff0.display("laplaceDirichletVertexNonConservative: coeff0");
    equationNumber0.display("laplaceDirichletVertexNonConservative: equationNumber0");
    // d12.display("laplaceDirichletVertexNonConservative: dr12");
    // d22.display("laplaceDirichletVertexNonConservative: dr22");
  }	
}

//================================================================
// Define Laplace's equation with Neumann Boundary Conditions
//   vertex centred, non conservation form
//================================================================
void Oges::
laplaceNeumannVertexNonConservative( const int grid )
{

  printObsoleteMessage("laplaceNeumannVertexNonConservative",1);

  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];
  int stencilLength0 = coeff0.GET_LENGTH(axis1);

  // Check to see if there is enough space in the arrays
  if( stencilLength0 < pow(3,numberOfDimensions)*numberOfComponents )
  {
    cerr << "laplaceNeumannVertexNonConservative:ERROR stencilLength0 is too small! " << endl;
    exit(1);
  }

  defineVertexDifferences( c );

  int m1,m2,m3,n;
  int side,axis;
  Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;

  n=0;  // component number, assume only one component for now

  coeff0=0.; // set all coefficients to zero

  Index I1,I2,I3,R[3];
  getIndex( c.indexRange(),I1,I2,I3 );

  // Assign equation Numbers - default values:
  ForStencil(m1,m2,m3)  
    EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3)=equationNo(n,(I1+m1),(I2+m2),(I3+m3),grid);


  MappedGridOperators & op = operators[grid];   // operators for this grid

  Index M(0,pow(width,numberOfDimensions));

  coeff0(M,I1,I2,I3)=op.laplacianCoefficients()(M,I1,I2,I3);    // fill in interior points

  ForBoundary(side,axis)
  {
    if( c.boundaryCondition()(side,axis) > 0 )
    {
      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points

      RealDistributedArray & normal = c.vertexBoundaryNormal(side,axis);

/* ----- This should work when A++ is fixed
      coeff0(M,Ig1,Ig2,Ig3)=normal(Ib1,Ib2,Ib3,axis1)*op.xCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3)
                           +normal(Ib1,Ib2,Ib3,axis2)*op.yCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
      if( numberOfDimensions==3 )
        coeff0(M,Ig1,Ig2,Ig3)+=normal(Ib1,Ib2,Ib3,axis3)*op.zCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
------ */

     // ============  Here is a temporary fix =======
#define OPX(m1,m2,m3,n,I1,I2,I3) MERGE0(opX,M123N(m1,m2,m3,n),I1,I2,I3)
#define OPY(m1,m2,m3,n,I1,I2,I3) MERGE0(opY,M123N(m1,m2,m3,n),I1,I2,I3)
#define OPZ(m1,m2,m3,n,I1,I2,I3) MERGE0(opZ,M123N(m1,m2,m3,n),I1,I2,I3)
      RealDistributedArray opX,opY,opZ;
      opX.redim(Range(M.getBase(),M.getBound()),            // dimension (to get base correct)
		Range(Ib1.getBase(),Ib1.getBound()),
		Range(Ib2.getBase(),Ib2.getBound()),
		Range(Ib3.getBase(),Ib3.getBound()));
      opX=op.xCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
      if( numberOfDimensions>1 )
      {
	opY.redim(opX);
	opY=op.yCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
      }
      if( numberOfDimensions>2 )
      {
	opZ.redim(opX);
	opZ=op.zCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
      }
      ForStencil(m1,m2,m3)
      {
	OPX(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis1);
	if( numberOfDimensions>1 )
	  OPY(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis2);
	if( numberOfDimensions>2 )
	  OPZ(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis3);
      }
      if( numberOfDimensions==1 )
	coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3);
      else if( numberOfDimensions==2 )
	coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3);
      else if( numberOfDimensions==3 )
	coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3)+opZ(M,Ib1,Ib2,Ib3);

      // ==========================================================


      ForStencil(m1,m2,m3)
      {
	//assign eqn numbers (different from default)
	EQUATIONNUMBER(m1,m2,m3,n,Ig1,Ig2,Ig3)=
	  equationNo(n,(Ib1+m1),(Ib2+m2),(Ib3+m3),grid);
      }
    }
  }


  // add coefficient for constraint: u_xx+u_yy + r
  // Put the coefficient after the stencil that is already there
  m1=halfWidth1;  m2=halfWidth2;  m3=halfWidth3;
  for( n=0; n<numberOfComponents; n++) \
    COEFF(m1+1,m2,m3,n,I1,I2,I3)=rightNullVector[grid](I1,I2,I3);
  // extra equations are assigned to unused grid points 
  // Here is the equation number for that point	
  cout << "discrete: extraequationNumber = " << extraEquationNumber(0) << endl;
  for( n=0; n<numberOfComponents; n++) \
    EQUATIONNUMBER(m1+1,m2,m3,n,I1,I2,I3)=extraEquationNumber(0);

  // ...Return the coeff's corresponding to the null vector constraint
  // These coefficients are returned as a dense matrix
  cout << "getDiscreteCoefficients: Set dense Equations *********** " << endl;
  coefficientsOfDenseExtraEquations = &rightNullVector;
}

//-------------------------------------------------------------------------------------
//  Laplace Equation with Mixed Boundary Conditions
//    
//   The boundary conditions are determined from the operators
//-------------------------------------------------------------------------------------
void Oges::
laplaceMixedVertexNonConservative( const int grid )
{
  printObsoleteMessage("laplaceMixedVertexNonConservative",1); 

  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];
  int stencilLength0 = coeff0.GET_LENGTH(axis1);

  // Check to see if there is enough space in the arrays
  if( stencilLength0 < pow(3,numberOfDimensions)*numberOfComponents )
  {
    cerr << "laplaceNeumannVertexNonConservative:ERROR stencilLength0 is too small! " << endl;
    exit(1);
  }

  defineVertexDifferences( c );

  int m1,m2,m3,n;
  int side,axis;
  Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;

  coeff0=0.; // set all coefficients to zero

  Index I1,I2,I3,R[3];
  getIndex( c.indexRange(),I1,I2,I3 );

  // Assign equation Numbers - default values:
  ForStencilN(n,m1,m2,m3)  
    EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3)=equationNo(n,(I1+m1),(I2+m2),(I3+m3),grid);


  MappedGridOperators & op = operators[grid];   // operators for this grid

  Index M(0,pow(width,numberOfDimensions));

  coeff0(M,I1,I2,I3)=op.laplacianCoefficients()(M,I1,I2,I3);    // fill in interior points

  ForBoundary(side,axis)
  {
    if( c.boundaryCondition()(side,axis) > 0 )
    { 
      if(  op.boundaryCondition()(side,axis,0)==MappedGridOperators::dirichlet )
      { // ********** Dirichlet ********************
        printf("***discreteVertex: setting bc on grid %i, side %i, axis %i to Dirichlet...\n",grid,side,axis);
	getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	coeff0(M,Ib1,Ib2,Ib3)=op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  // Dirichlet BC is the Identity operator
	// Apply equation on ghost line!
	getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points
	coeff0(M,Ig1,Ig2,Ig3)=op.laplacianCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  
	ForStencilN(n,m1,m2,m3)
	{
	  //assign eqn numbers (different from default)
	  EQUATIONNUMBER(m1,m2,m3,n,Ig1,Ig2,Ig3)=
	    equationNo(n,(Ib1+m1),(Ib2+m2),(Ib3+m3),grid);
	}
      }
      else if( op.boundaryCondition()(side,axis,0)==MappedGridOperators::neumann ||
               op.boundaryCondition()(side,axis,0)==MappedGridOperators::mixed )
      { // *************** Neumann or Mixed *******************
        printf("***discreteVertex: setting bc on grid %i, side %i, axis %i to Neumann...\n",grid,side,axis);
	getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points

	RealDistributedArray & normal = c.vertexBoundaryNormal(side,axis);
	RealDistributedArray opX,opY,opZ;
	opX.redim(Range(M.getBase(),M.getBound()),            // dimension (to get base correct)
		  Range(Ib1.getBase(),Ib1.getBound()),
		  Range(Ib2.getBase(),Ib2.getBound()),
		  Range(Ib3.getBase(),Ib3.getBound()));
	opX=op.xCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	if( numberOfDimensions>1 )
	{
  	  opY.redim(opX);
  	  opY=op.yCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	}
	if( numberOfDimensions>2 )
	{
	  opZ.redim(opX);
	  opZ=op.zCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	}
	ForStencilN(n,m1,m2,m3)
	{
	  OPX(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis1);
          if( numberOfDimensions>1 )
            OPY(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis2);
	  if( numberOfDimensions>2 )
	    OPZ(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis3);
	}
        if( op.boundaryCondition()(side,axis,0)==MappedGridOperators::neumann )
	{

          if( numberOfDimensions==1 )
    	    coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3);
          else if( numberOfDimensions==2 )
    	    coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3);
	  else if( numberOfDimensions==3 )
	    coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3)+opZ(M,Ib1,Ib2,Ib3);
        }
	else
	{
           // mixed BC: alpha*u + beta*u.n
          real alpha = op.constantCoefficient(0,side,axis,0);
          real beta  = op.constantCoefficient(1,side,axis,0);

          printf("discreteVertex: setting mixed derivative BC: alpha=%e, beta=%e\n",alpha,beta);
	   
          if( numberOfDimensions==1 )
  	    coeff0(M,Ig1,Ig2,Ig3)=beta*opX(M,Ib1,Ib2,Ib3)
	            +alpha*op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
          else if( numberOfDimensions==2 )
  	    coeff0(M,Ig1,Ig2,Ig3)=beta*(opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3))
                   +alpha*op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	  else if( numberOfDimensions==3 )
  	    coeff0(M,Ig1,Ig2,Ig3)=beta*(opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3)+opZ(M,Ib1,Ib2,Ib3))
                   +alpha*op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	}
	ForStencilN(n,m1,m2,m3)
	{
	  //assign eqn numbers (different from default)
	  EQUATIONNUMBER(m1,m2,m3,n,Ig1,Ig2,Ig3)=
	    equationNo(n,(Ib1+m1),(Ib2+m2),(Ib3+m3),grid);
	}
      }
    }
  }

  if( parameters.compatibilityConstraint )
  {
    // add coefficient for constraint: u_xx+u_yy + r
    // Put the coefficient after the stencil that is already there
    m1=halfWidth1;  m2=halfWidth2;  m3=halfWidth3;
    for( n=0; n<numberOfComponents; n++) \
      COEFF(m1+1,m2,m3,n,I1,I2,I3)=rightNullVector[grid](I1,I2,I3);
    // extra equations are assigned to unused grid points 
    // Here is the equation number for that point	
    cout << "discrete: extraequationNumber = " << extraEquationNumber(0) << endl;
    for( n=0; n<numberOfComponents; n++) \
      EQUATIONNUMBER(m1+1,m2,m3,n,I1,I2,I3)=extraEquationNumber(0);

    // ...Return the coeff's corresponding to the null vector constraint
    // These coefficients are returned as a dense matrix
    cout << "getDiscreteCoefficients: Set dense Equations *********** " << endl;
    coefficientsOfDenseExtraEquations = &rightNullVector;
  }
}

//--------------------------------------------------------------
// biharmonic is defined as a system
//   u.xx+u.yy - v = 
//   v.xx+v.yy     =
// Real boundary conditions are
//   u=
//   u.n=
// Numerically we give Boundary conditions:
//      u= and v_xx+v_yy=     on boundary
//      u.n= and u_xx+u_yy-v= on ghostline
//--------------------------------------------------------------
void Oges::
biharmonicDirichletVertexNonConservative( const int grid )
{
  printObsoleteMessage("biharmonicDirichletVertexNonConservative",1); 
 
  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];

  // stencilLength0 is used in some of the macros as the offset for different components
  int stencilLength0 = int(pow(3,numberOfDimensions)+1);
  int offsetForV=stencilLength0;

  // Check to see if there is enough space in the arrays
  if( coeff0.GET_LENGTH(0) < pow(3,numberOfDimensions)*2 )
  {
    cerr << "biharmonicDirichletVertexNonConservative:ERROR the first dimension of the coefficient"
            " array is too small! " << endl;
    exit(1);
  }

  int nu=0;  // component number for u
  int nv=1;  // component number for v


  defineVertexDifferences( c );

  int m1,m2,m3,n;
  int side,axis;
  Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;

  coeff0=0.; // set all coefficients to zero

  Index I1,I2,I3,R[3];
  getIndex( c.indexRange(),I1,I2,I3 ); 

  MappedGridOperators & op = operators[grid];   // operators for this grid

  // Assign equation Numbers - default values:
  ForStencilN(n,m1,m2,m3)  
    EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3)=equationNo(n,(I1+m1),(I2+m2),(I3+m3),grid);

  int is1 = c.isPeriodic()(0) ? 0 : 1;
  int is2 = c.isPeriodic()(1) ? 0 : 1;
  int is3 = c.isPeriodic()(2) ? 0 : 1;
     
  getIndex( c.indexRange(),I1,I2,I3,-is1,-is2,-is3 );  // do not do boundaries

  Index M(0,pow(width,numberOfDimensions));

  coeff0(M,I1,I2,I3)=op.laplacianCoefficients()(M,I1,I2,I3);            // Laplacian(u)
  coeff0(M+offsetForV,I1,I2,I3)=coeff0(M,I1,I2,I3);     // Laplacian(v)
  
  // now add -v to first equation
  m1=halfWidth1; m2=halfWidth2;  m3=halfWidth3;
  COEFF(m1+1,m2,m3,nu,I1,I2,I3)=-1.;
  EQUATIONNUMBER(m1+1,m2,m3,nu,I1,I2,I3)=equationNo(nv,I1,I2,I3,grid);

  // Boundary conditions:
  //      u= and v_xx+v_yy=     on boundary
  //      u.n= and u_xx+u_yy-v= on ghostline
  ForBoundary(side,axis)
  {
    if( c.boundaryCondition()(side,axis) > 0 )
    {
      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points

      coeff0(M,Ib1,Ib2,Ib3)=op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  // Dirichlet BC's for u
      coeff0(M+offsetForV,Ib1,Ib2,Ib3)=op.laplacianCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  // Lap(v)

      // 
      // Now add the Neumann Boundary Condition
      //   We would like to say:
      //      coeff0(M,Ig1,Ig2,Ig3)=
      //             normal(Ib1,Ib2,Ib3,axis1)*op.xCoefficients(Ib1,Ib2,Ib3(M,Ib1,Ib2,Ib3))
      //	    +normal(Ib1,Ib2,Ib3,axis2)*op.yCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
      // But this is not a conformable operation, thus we have to reshape the normal
      //
      RealDistributedArray normal1; normal1.redim(c.vertexBoundaryNormal(side,axis)(Ib1,Ib2,Ib3,0));
      normal1(Ib1,Ib2,Ib3) = c.vertexBoundaryNormal(side,axis)(Ib1,Ib2,Ib3,0);
      RealDistributedArray normal2; normal2.redim(c.vertexBoundaryNormal(side,axis)(Ib1,Ib2,Ib3,1));
      normal2(Ib1,Ib2,Ib3)=c.vertexBoundaryNormal(side,axis)(Ib1,Ib2,Ib3,1);
      normal1.reshape(Range(0,0),
		      Range(normal1.getBase(0),normal1.getBound(0)),
		      Range(normal1.getBase(1),normal1.getBound(1)),
		      Range(normal1.getBase(2),normal1.getBound(2)));
      normal2.reshape(Range(0,0),
		      Range(normal2.getBase(0),normal2.getBound(0)),
		      Range(normal2.getBase(1),normal2.getBound(1)),
		      Range(normal2.getBase(2),normal2.getBound(2)));
      
      RealDistributedArray opX; opX.reference(op.xCoefficients(Ib1,Ib2,Ib3));
      RealDistributedArray opY; opY.reference(op.yCoefficients(Ib1,Ib2,Ib3));

      ForStencil(m1,m2,m3)
      {
	coeff0(M123(m1,m2,m3),Ig1,Ig2,Ig3)=
  	     normal1*opX(M123(m1,m2,m3),Ib1,Ib2,Ib3)
	    +normal2*opY(M123(m1,m2,m3),Ib1,Ib2,Ib3);
      }


      coeff0(M+offsetForV,Ig1,Ig2,Ig3)=op.laplacianCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  // Lap(v)


      ForStencil(m1,m2,m3)
      {
//      COEFF(m1,m2,m3,nu,Ib1,Ib2,Ib3)=delta(m1)*delta(m2);
//      COEFF(m1,m2,m3,nv,Ib1,Ib2,Ib3)=UXX2(Ib1,Ib2,Ib3,m1,m2,m3)
//                                    +UYY2(Ib1,Ib2,Ib3,m1,m2,m3);
        EQUATIONNUMBER(m1,m2,m3,nv,Ib1,Ib2,Ib3)=equationNo(nu,Ib1+m1,Ib2+m2,Ib3+m3,grid);

//      COEFF(m1,m2,m3,nu,Ig1,Ig2,Ig3)=
//           normal(Ib1,Ib2,Ib3,axis1)*UX2(Ib1,Ib2,Ib3,m1,m2,m3)
//	    +normal(Ib1,Ib2,Ib3,axis2)*UY2(Ib1,Ib2,Ib3,m1,m2,m3);
        EQUATIONNUMBER(m1,m2,m3,nu,Ig1,Ig2,Ig3)=equationNo(nu,Ib1+m1,Ib2+m2,Ib3+m3,grid);
//      COEFF(m1,m2,m3,nv,Ig1,Ig2,Ig3)=UXX2(Ib1,Ib2,Ib3,m1,m2,m3)
//                                    +UYY2(Ib1,Ib2,Ib3,m1,m2,m3);
        EQUATIONNUMBER(m1,m2,m3,nv,Ig1,Ig2,Ig3)=equationNo(nv,Ib1+m1,Ib2+m2,Ib3+m3,grid);
      }
      m1=halfWidth1; m2=halfWidth2;  m3=halfWidth3;
      COEFF(m1+1,m2,m3,nv,Ib1,Ib2,Ib3)=-1.;
      EQUATIONNUMBER(m1+1,m2,m3,nv,Ib1,Ib2,Ib3)=equationNo(nv,Ib1,Ib2,Ib3,grid);
    }
  }

}

void Oges::
addBoundaryConditionsToMatrix( const int grid )
//===================================================================================
// /Purpose: Add boundary conditions to the matrix according to the information
//  found in the operators member.
//  For sides with Dirichlet BC's the ghostline equations are set to extrapolation
//  if the ghostlines are being used (ie. set with setGhostLineOption)
// 
//===================================================================================
{
  printObsoleteMessage("addBoundaryConditionsToMatrix",1); 

  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];
  int stencilLength0 = coeff0.GET_LENGTH(axis1);

  defineVertexDifferences( c );

  int m1,m2,m3,n;
  int side,axis;
  Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
  Index R[3];

  MappedGridOperators & op = operators[grid];   // operators for this grid

  Index M(0,pow(width,numberOfDimensions));

  ForBoundary(side,axis)
  {
    if( c.boundaryCondition()(side,axis) > 0 )
    { 
      if(  op.boundaryCondition()(side,axis,0)==MappedGridOperators::dirichlet )
      { // ********** Dirichlet ********************
        printf("***addBoundaryConditions: setting bc on grid %i, side %i, axis %i to Dirichlet...\n",
               grid,side,axis);
	getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	coeff0(M,Ib1,Ib2,Ib3)=op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);  // Dirichlet BC is the Identity operator

        if(ghostLineOption[grid](side,axis,1,0)==useGhostLine             ||
           ghostLineOption[grid](side,axis,1,0)==useGhostLineExceptCorner ||
	   ghostLineOption[grid](side,axis,1,0)==useGhostLineExceptCornerAndNeighbours )
	{
	  // Extrapolate the ghost line!  
          // change the classify array to be extrapolation
          getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,1);   // Index's for ghost points+extra=1
          where( classify[grid](Ig1,Ig2,Ig3)>0 )
	  {
	    classify[grid](Ig1,Ig2,Ig3)=extrapolation;
	  }
	}
/* -------
	getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points
	for( int i=0; i<stencilLength0; i++ )
	  coeff0(M,Ig1,Ig2,Ig3)=0.;
	ForStencilN(n,m1,m2,m3)
	{
	  EQUATIONNUMBER(m1,m2,m3,n,Ig1,Ig2,Ig3)=0;
	}
	// extrapolate
	int iordex=orderOfAccuracy+1;   // extrapolate to this order
        int n=0;
        IntegerArray iv(3);
	int & i1 = iv(0);
	int & i2 = iv(1);
	int & i3 = iv(2);
        for( i3=Ig3.getBase(); i3<=I3g.getBound(); i3++ )
        for( i2=Ig2.getBase(); i2<=I3g.getBound(); i2++ )
        for( i1=Ig1.getBase(); i1<=I3g.getBound(); i1++ )
  	  getExtrapCoeff( iv,grid,n,iordex,coeff0,equationNumber0 );
--------- */

      }
      else if( op.boundaryCondition()(side,axis,0)==MappedGridOperators::neumann ||
               op.boundaryCondition()(side,axis,0)==MappedGridOperators::mixed )
      { // *************** Neumann or Mixed *******************
        printf("***addBoundaryConditions: setting bc on grid %i, side %i, axis %i to Neumann...\n",
               grid,side,axis);
        if(ghostLineOption[grid](side,axis,1,0)==extrapolateGhostLine )
	{
          cout << "Oges::addBoundaryConditionsToMatrix:Error: attempting to add Neumann BC \n";
	  cout << "but the first ghost line is defined to be extrapolation \n";
	  cout << "you should use setGhostLineOption (and setNumberOfGhostLines) to fix this\n";
	  Overture::abort("Oges::addBoundaryConditionsToMatrix:Error");
	}

	getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points

	RealDistributedArray & normal = c.vertexBoundaryNormal(side,axis);
	RealDistributedArray opX,opY,opZ;
	opX.redim(Range(M.getBase(),M.getBound()),            // dimension (to get base correct)
		  Range(Ib1.getBase(),Ib1.getBound()),
		  Range(Ib2.getBase(),Ib2.getBound()),
		  Range(Ib3.getBase(),Ib3.getBound()));
	opX=op.xCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	if( numberOfDimensions>1 )
	{
  	  opY.redim(opX);
	  opY=op.yCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	}
	if( numberOfDimensions==3 )
	{
	  opZ.redim(opX);
	  opZ=op.zCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	}
	ForStencilN(n,m1,m2,m3)
	{
	  if( numberOfDimensions>1 )
	  {
  	    OPX(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis1);
  	    OPY(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis2);
	    if( numberOfDimensions==3 )
	      OPZ(m1,m2,m3,n,Ib1,Ib2,Ib3)*=normal(Ib1,Ib2,Ib3,axis3);
	  }
	  else
  	    OPX(m1,m2,m3,n,Ib1,Ib2,Ib3)*=(2*side-1); // outward normal in 1d
	}
        if( op.boundaryCondition()(side,axis,0)==MappedGridOperators::neumann )
	{
	  if( numberOfDimensions>1 )
	  {
    	    coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3);
  	    if( numberOfDimensions==3 )
	      coeff0(M,Ig1,Ig2,Ig3)+=opZ(M,Ib1,Ib2,Ib3);
	  }
	  else
    	    coeff0(M,Ig1,Ig2,Ig3)=opX(M,Ib1,Ib2,Ib3);
        }
	else
	{
           // mixed BC: alpha*u + beta*u.n
          real alpha = op.constantCoefficient(0,side,axis,0);
          real beta  = op.constantCoefficient(1,side,axis,0);

          printf("addBoundaryConditions: setting mixed derivative BC: alpha=%e, beta=%e\n",alpha,beta);
	   
          if( numberOfDimensions>1 )
	  {
	    coeff0(M,Ig1,Ig2,Ig3)=beta*(opX(M,Ib1,Ib2,Ib3)+opY(M,Ib1,Ib2,Ib3))
	      +alpha*op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	    if( numberOfDimensions==3 )
	      coeff0(M,Ig1,Ig2,Ig3)+=beta*opZ(M,Ib1,Ib2,Ib3);
	  }
	  else
	    coeff0(M,Ig1,Ig2,Ig3)=beta*opX(M,Ib1,Ib2,Ib3)
         	      +alpha*op.identityCoefficients(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
	}
	ForStencilN(n,m1,m2,m3)
	{
	  //assign eqn numbers (different from default)
	  EQUATIONNUMBER(m1,m2,m3,n,Ig1,Ig2,Ig3)=
	    equationNo(n,(Ib1+m1),(Ib2+m2),(Ib3+m3),grid);
	}
      }
    }
  }
}

#undef OPX
#undef OPY
#undef OPZ


void Oges::
userSuppliedCoefficients( const int grid )
{
  printObsoleteMessage("userSuppliedCoefficients",1); 
  //================================================================
  // For user supplied coefficients, assign equation numbers
  //================================================================

  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];
  int stencilLength0 = coeff0.GET_LENGTH(axis1);

  // Check to see if there is enough space in the arrays
  if( stencilLength0 < pow(3,numberOfDimensions)*numberOfComponents )
  {
    cerr << "userSuppliedCoefficients:ERROR stencilLength0 is too small! " << endl;
    exit(1);
  }

  Index I1,I2,I3,R[3];
  // assign equation numbers for ALL lines including the last ghostline (this will produce
  // some inavlid equation numbers -- but they won't be used)
  getIndex(c.dimension(),I1,I2,I3);

  // Assign equation Numbers - default values:
  width=orderOfAccuracy+1;
  halfWidth=width/2;
  halfWidth1 = numberOfDimensions>0 ? halfWidth : 0;
  halfWidth2 = numberOfDimensions>1 ? halfWidth : 0;
  halfWidth3 = numberOfDimensions>2 ? halfWidth : 0;
  int m1,m2,m3,n;
  ForStencilN(n,m1,m2,m3)  
    EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3)=
        equationNo(n,(I1+m1),(I2+m2),(I3+m3),grid);

  if( addBoundaryConditions )
  { // Add boundary conditions based on the operators
    addBoundaryConditionsToMatrix(grid);
  }
  else
  {
     // User has supplied boundary conditions
    // assign equation numbers on last line on all sides
    // Make sure that there are no non-zero coefficients that multiply points that are
    // not on the grid
    int axis,side,i1,i2,i3;
    ForBoundary(side,axis)
    {
      getBoundaryIndex(c.dimension(),side,axis,I1,I2,I3);

      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
        int m1a= (axis==0 && side==End)   ?  1 : -halfWidth1;
        int m1b= (axis==0 && side==Start) ? -1 : +halfWidth1;
        int m2a= (axis==1 && side==End)   ?  1 : -halfWidth2;
        int m2b= (axis==1 && side==Start) ? -1 : +halfWidth2;
        int m3a= (axis==2 && side==End)   ?  1 : -halfWidth3;
        int m3b= (axis==2 && side==Start) ? -1 : +halfWidth3;
      
      
        for( n=0; n<numberOfComponents; n++) 
        for( m3=m3a; m3<=m3b; m3++)
        for( m2=m2a; m2<=m2b; m2++)
        for( m1=m1a; m1<=m1b; m1++) 
        {
	  if( COEFF0(m1,m2,m3,n,i1,i2,i3)!=0. && classify[grid](i1,i2,i3,n)>0 )
	  {
	    cout << "Oges::Error in setting userSupplied Coefficients \n";
	    cout << "For the equation on the last ghost line there is a non-zero coefficient mutliplying\n"
	      " a point which is not on the grid!\n";
	    printf(" grid=%i, (i1,i2,i3)=(%i,%i,%i), (m1,m2,m3)=(%i,%i,%i) \n",grid,i1,i2,i3,m1,m2,m3);
	    cout << "I am setting this coefficient to zero!! \n";
	    COEFF0(m1,m2,m3,n,i1,i2,i3)=0.;
	  }
	  EQUATIONNUMBER0(m1,m2,m3,n,i1,i2,i3)=1;  // use a bogus number, the coeff will be zero anyway
	}
      }
    }
  }
  

  // Add in the compatibility constraint:
  if( parameters.compatibilityConstraint )
  {
    getIndex(c.dimension(),I1,I2,I3);
    // add coefficient for constraint: u_xx+u_yy + r
    // Put the coefficient after the stencil that is already there
    m1=halfWidth1;  m2=halfWidth2;  m3=halfWidth3; n=0;
    COEFF(m1+1,m2,m3,n,I1,I2,I3)=rightNullVector[grid](I1,I2,I3);
    // extra equations are assigned to unused grid points 
    // Here is the equation number for that point	
    cout << "discrete: extraEquationNumber = " << extraEquationNumber(0) << endl;
    EQUATIONNUMBER(m1+1,m2,m3,n,I1,I2,I3)=extraEquationNumber(0);

    // ...Return the coeff's corresponding to the null vector constraint
    // These coefficients are returned as a dense matrix
    cout << "getDiscreteCoefficients: Set dense Equations *********** " << endl;
    coefficientsOfDenseExtraEquations = &rightNullVector;
 }
  
}


void Oges::
implicitInterpolation( const int grid )
{
  printObsoleteMessage("implicitInterpolation",1); 

  //================================================================
  // Interpolation (implicit)
  //  Define the equations for implicit interpolation, this is
  //  simply the identity operator
  //================================================================

  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];
  int stencilLength0 = coeff0.GET_LENGTH(axis1);

  // Check to see if there is enough space in the arrays
  if( stencilLength0 < pow(3,numberOfDimensions)*numberOfComponents )
  {
    cerr << "laplaceDirichletVertexNonConservative:ERROR stencilLength0 is too small! " << endl;
    exit(1);
  }

  defineVertexDifferences( c );

  int m1,m2,m3,n;

  coeff0=0.; // set all coefficients to zero

  Index I1,I2,I3,R[3];
  getIndex(c.dimension(),I1,I2,I3);

  // Assign equation Numbers : only one non-zero coefficient, put in first position
  m1=-halfWidth1;
  m2=-halfWidth2;
  m3=-halfWidth3;
  for( n=0; n<numberOfComponents; n++ )
  {
    EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3)=equationNo(n,I1,I2,I3,grid);
    COEFF(m1,m2,m3,n,I1,I2,I3)=1.;
  }
  
}




