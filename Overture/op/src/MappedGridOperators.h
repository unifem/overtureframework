/*  -*-Mode: c++; -*-  */

#ifndef MAPPED_GRID_OPERATORS
#define MAPPED_GRID_OPERATORS

#include "GenericMappedGridOperators.h"
#include "BoundaryConditionParameters.h"
// include "Overture.h"
// include "OGFunction.h"

// extern BoundaryConditionParameters Overture::defaultBoundaryConditionParameters();

//===========================================================================================
// This class defines spatial derivatives and Boundary Conditions of a MappedGridFunction
//
// Notes:
//  o This class defines derivatives by finite differences and the "mapping method"
//       thus  u.x = (r.x)*u.r + (s.x)*u.s + (t.x)*u.t
//       where u.r = second or fourth order finite differences
//  o This class provides the "forward" derivatives of grid functions, member functions
//    x,y,z,xx,xy,... 
//  o This class also provides functions that return the difference operator as a matrix,
//    member functions X,Y,Z,XX,XY,....  These matrices can be used by sparse solvers
//    such as the Oges Class.
//  o To define the derivatives in a different way you should derive from this class
//    and redefine any functions that you want to. If you provide the virtualConstructor
//    member function then your derived class can be used by the GridCollectionOperators
//    and CompositeGridOperators classes which define derivatives for GridCollectionFunction's
//    and CompositeGridFunction's.
//
//  o Boundary conditions are described in more detail in BoundaryOperators.C

//  o see also the grid function documentation for more details
//
//  Who to blame: Bill Henshaw, CIC-19, henshaw@lanl.gov
//  Date of last revision: 95/04/05
//===========================================================================================

class MappedGridOperators;
class GridFunctionParameters;
class FourierOperators;

// This typedef is used to define the derivativeFunction array which keeps pointers
// to functions that compute derivatives
typedef void (*DerivativeFunctionPointer)(const realMappedGridFunction & u,   
					  RealDistributedArray & derivative,             
					  const Index & I1,   
					  const Index & I2,   
					  const Index & I3,   
					  const Index & N,   
					  MappedGridOperators & mgop );
// This typedef is used to define the derivativeFunction array which keeps pointers
// to functions that compute the coefficients of derivatives
typedef void (*DerivCoefficientsFunctionPointer)(RealDistributedArray & derivative,             
						 const Index & I1,   
						 const Index & I2,   
						 const Index & I3,   
						 const Index & E,   
						 const Index & C,   
						 MappedGridOperators & mgop );

class MappedGridOperators : public GenericMappedGridOperators  
{

 public:

  // This enumerator lists values that are used for default arguments
  enum         
  {
    forAll=realMappedGridFunction::forAll,
    undefinedValue=realMappedGridFunction::undefinedValue,
    defaultValue=realMappedGridFunction::defaultValue,
    allBoundaries=GenericMappedGridOperators::allBoundaries
  };
  // This enumerator contains a list of all the derivatives that we know how to evaluate
  enum derivativeTypes
  {
    xDerivative,
    yDerivative,
    zDerivative,
    xxDerivative,
    xyDerivative,
    xzDerivative,
    yxDerivative,
    yyDerivative,
    yzDerivative,
    zxDerivative,
    zyDerivative,
    zzDerivative,
    laplacianOperator,
    r1Derivative,
    r2Derivative,
    r3Derivative,
    r1r1Derivative,
    r1r2Derivative,
    r1r3Derivative,
    r2r2Derivative,
    r2r3Derivative,
    r3r3Derivative,
    gradient,
    divergence,
    divergenceScalarGradient,
    scalarGradient,
    identityOperator,
    vorticityOperator,
    xDerivativeScalarXDerivative,
    xDerivativeScalarYDerivative,
    xDerivativeScalarZDerivative,
    yDerivativeScalarXDerivative,
    yDerivativeScalarYDerivative,
    yDerivativeScalarZDerivative,
    zDerivativeScalarXDerivative,
    zDerivativeScalarYDerivative,
    zDerivativeScalarZDerivative,
    divVectorScalarDerivative,
    divergenceTensorGradient, // *new* 050920
    numberOfDifferentDerivatives   // counts number of entries in this list
  };


  // -------------- public member functions ----------------------------
  MappedGridOperators();
  MappedGridOperators( MappedGrid & mg );                   // use this MappedGrid
  MappedGridOperators( const MappedGridOperators & mgfd );  // copy constructor
  virtual GenericMappedGridOperators* virtualConstructor() const;        // create a new object of this class
  
  virtual ~MappedGridOperators();

  virtual MappedGridOperators & operator= ( const MappedGridOperators & dmgf );
  virtual GenericMappedGridOperators & operator= ( const GenericMappedGridOperators & mgo );

  virtual void useConservativeApproximations(bool trueOrFalse = TRUE );

  // return size of this object  
  virtual real sizeOf(FILE *file = NULL ) const;

  virtual void updateToMatchGrid( MappedGrid & mg );                  // here is a new MappedGrid
  virtual void updateToMatchUnstructuredGrid( MappedGrid & mg ); // kkc 030404

  FourierOperators* getFourierOperators(const bool abortIfNull=TRUE) const;    // return a pointer to the Fourier operators

  // get from a database file
  virtual int get( const GenericDataBase & dir, const aString & name);  
  // put to a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;  

  void setOrderOfAccuracy( const int & orderOfAccuracy );
  
  // Indicate if twilight-zone forcing should be added to boundary conditions  
  virtual void setTwilightZoneFlow( const int & TwilightZoneFlow );

#define FUNCTION(type) \
  virtual realMappedGridFunction type(const realMappedGridFunction & u,  \
				      const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );    \
                                                                         \
  virtual realMappedGridFunction type(const realMappedGridFunction & u,  \
                                      const GridFunctionParameters & gfType,   \
				      const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );     

#define FUNCTION_COEFFICIENTS(type) \
  virtual realMappedGridFunction type(const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );    \
                                                                         \
  virtual realMappedGridFunction type(const GridFunctionParameters & gfType,   \
                                      const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );


  // parametric derivatives in the r1,r2,r3 directions
  FUNCTION(r1)
  FUNCTION_COEFFICIENTS(r1Coefficients)
  FUNCTION(r2)
  FUNCTION_COEFFICIENTS(r2Coefficients)
  FUNCTION(r3)
  FUNCTION_COEFFICIENTS(r3Coefficients)
  FUNCTION(r1r1)
  FUNCTION_COEFFICIENTS(r1r1Coefficients)
  FUNCTION(r1r2)
  FUNCTION_COEFFICIENTS(r1r2Coefficients)
  FUNCTION(r1r3)
  FUNCTION_COEFFICIENTS(r1r3Coefficients)
  FUNCTION(r2r2)
  FUNCTION_COEFFICIENTS(r2r2Coefficients)
  FUNCTION(r2r3)
  FUNCTION_COEFFICIENTS(r2r3Coefficients)
  FUNCTION(r3r3)
  FUNCTION_COEFFICIENTS(r3r3Coefficients)

  // FUNCTIONs in the x,y,z directions
  FUNCTION(x)
  FUNCTION_COEFFICIENTS(xCoefficients)
  FUNCTION(y)
  FUNCTION_COEFFICIENTS(yCoefficients)
  FUNCTION(z)
  FUNCTION_COEFFICIENTS(zCoefficients)
  FUNCTION(xx)
  FUNCTION_COEFFICIENTS(xxCoefficients)
  FUNCTION(xy)
  FUNCTION_COEFFICIENTS(xyCoefficients)
  FUNCTION(xz)
  FUNCTION_COEFFICIENTS(xzCoefficients)
  FUNCTION(yy)
  FUNCTION_COEFFICIENTS(yyCoefficients)
  FUNCTION(yz)
  FUNCTION_COEFFICIENTS(yzCoefficients)
  FUNCTION(zz)
  FUNCTION_COEFFICIENTS(zzCoefficients)

  // other forms of derivatives

  FUNCTION(div)
  FUNCTION_COEFFICIENTS(divCoefficients)

  FUNCTION(grad)
  FUNCTION_COEFFICIENTS(gradCoefficients)

  FUNCTION(identity)
  FUNCTION_COEFFICIENTS(identityCoefficients)

  FUNCTION(laplacian)
  FUNCTION_COEFFICIENTS(laplacianCoefficients)

  FUNCTION(vorticity)
  // FUNCTION_COEFFICIENTS(gradCoefficients)
#undef FUNCTION
#undef FUNCTION_COEFFICIENTS

#define SCALAR_FUNCTION(type) \
  virtual realMappedGridFunction type(const realMappedGridFunction & u,  \
				      const realMappedGridFunction & s,  \
				      const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );    \
                                                                         \
  virtual realMappedGridFunction type(const realMappedGridFunction & u,  \
                                      const GridFunctionParameters & gfType,   \
				      const realMappedGridFunction & s,  \
				      const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );  

#define SCALAR_FUNCTION_COEFFICIENTS(type) \
  virtual realMappedGridFunction type(const realMappedGridFunction & s,  \
				      const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );    \
                                                                         \
  virtual realMappedGridFunction type(const GridFunctionParameters & gfType,   \
				      const realMappedGridFunction & s,  \
                                      const Index & I1 = nullIndex,      \
				      const Index & I2 = nullIndex,      \
				      const Index & I3 = nullIndex,      \
				      const Index & I4 = nullIndex,      \
				      const Index & I5 = nullIndex,      \
				      const Index & I6 = nullIndex,      \
				      const Index & I7 = nullIndex,      \
				      const Index & I8 = nullIndex );
SCALAR_FUNCTION(divScalarGrad)
SCALAR_FUNCTION_COEFFICIENTS(divScalarGradCoefficients)
SCALAR_FUNCTION(scalarGrad)
SCALAR_FUNCTION_COEFFICIENTS(scalarGradCoefficients)
SCALAR_FUNCTION(divVectorScalar)
SCALAR_FUNCTION_COEFFICIENTS(divVectorScalarCoefficients)

#undef SCALAR_FUNCTION
#undef SCALAR_FUNCTION_COEFFICIENTS

  // scalar times identityCoefficients
  virtual realMappedGridFunction scalarCoefficients(				
			const realMappedGridFunction & s,
			const Index & I1 = nullIndex,
			const Index & I2 = nullIndex,
			const Index & I3 = nullIndex,
			const Index & I4 = nullIndex,
			const Index & I5 = nullIndex, 
			const Index & I6 = nullIndex, 
			const Index & I7 = nullIndex, 
			const Index & I8 = nullIndex
			);

  // scalar array times identityCoefficients -- use this to multiply an array of values
  // times a coefficient matrix that represents a system of equations
  virtual realMappedGridFunction scalarCoefficients( const RealDistributedArray & s );




  virtual realMappedGridFunction derivativeScalarDerivative(const realMappedGridFunction & u,  
							    const realMappedGridFunction & s,  
							    const int & direction1,
							    const int & direction2,
							    const Index & I1 = nullIndex,      
							    const Index & I2 = nullIndex,      
							    const Index & I3 = nullIndex,      
							    const Index & I4 = nullIndex,      
							    const Index & I5 = nullIndex,      
							    const Index & I6 = nullIndex,      
							    const Index & I7 = nullIndex,      
							    const Index & I8 = nullIndex );    
                                                                         
  virtual realMappedGridFunction derivativeScalarDerivative(const realMappedGridFunction & u,  
							    const GridFunctionParameters & gfType,   
							    const realMappedGridFunction & s, 
							    const int & direction1,
							    const int & direction2,
							    const Index & I1 = nullIndex,      
							    const Index & I2 = nullIndex,      
							    const Index & I3 = nullIndex,      
							    const Index & I4 = nullIndex,      
							    const Index & I5 = nullIndex,      
							    const Index & I6 = nullIndex,      
							    const Index & I7 = nullIndex,      
							    const Index & I8 = nullIndex ); 

  virtual realMappedGridFunction derivativeScalarDerivativeCoefficients(const realMappedGridFunction & s,  
									const int & direction1,
									const int & direction2,
									const Index & I1 = nullIndex,      
									const Index & I2 = nullIndex,      
									const Index & I3 = nullIndex,      
									const Index & I4 = nullIndex,      
									const Index & I5 = nullIndex,      
									const Index & I6 = nullIndex,      
									const Index & I7 = nullIndex,      
									const Index & I8 = nullIndex );    
                                                                         
  virtual realMappedGridFunction derivativeScalarDerivativeCoefficients(const GridFunctionParameters & gfType,   
									const realMappedGridFunction & s, 
									const int & direction1,
									const int & direction2,
									const Index & I1 = nullIndex,      
									const Index & I2 = nullIndex,      
									const Index & I3 = nullIndex,      
									const Index & I4 = nullIndex,      
									const Index & I5 = nullIndex,      
									const Index & I6 = nullIndex,      
									const Index & I7 = nullIndex,      
									const Index & I8 = nullIndex );


  // Here are functions used to evaluate a whole set of derivatives at a time (for efficiency)
  // Make a list of derivatives to be evaluated and supply arrays to save the results in
  virtual void setNumberOfDerivativesToEvaluate( const int & numberOfDerivatives );
  virtual void setDerivativeType(const int & index, 
				 const derivativeTypes & derivativeType, 
				 RealDistributedArray & ux1x2 );
  virtual void getDerivatives(const realMappedGridFunction & u, 
			      const Index & I1 = nullIndex, 
			      const Index & I2 = nullIndex, 
			      const Index & I3 = nullIndex, 
			      const Index & I4 = nullIndex,
                              const Index & Evalute = nullIndex );   // evaluate these

  // return true if the grid is rectangular
  virtual bool isRectangular();

  // Indicate the stencil size for functions returning coefficients
  virtual void setStencilSize(const int stencilSize);

  // evaluate the coefficients for a derivative 
  // this is an efficient version (in both memory and speed).
  virtual int coefficients(const derivativeTypes & derivativeType,
			   realMappedGridFunction & coeff, 
			   const Index & I1 = nullIndex, 
			   const Index & I2 = nullIndex, 
			   const Index & I3 = nullIndex, 
			   const Index & E =nullIndex,   
			   const Index & C =nullIndex );

  // efficient version for derivatives requiring a scalar too.
  virtual int coefficients(const derivativeTypes & derivativeType,
			   realMappedGridFunction & coeff, 
			   const realMappedGridFunction & scalar, 
			   const Index & I1 = nullIndex, 
			   const Index & I2 = nullIndex, 
			   const Index & I3 = nullIndex, 
			   const Index & E =nullIndex,   
			   const Index & C =nullIndex );

  // evaluate the coefficients for a derivative *** this version fills in an array ***
  // this is an efficient version (in both memory and speed).
  virtual int assignCoefficients(const derivativeTypes & derivativeType,
			   realArray & coeff, 
			   const Index & I1 = nullIndex, 
			   const Index & I2 = nullIndex, 
			   const Index & I3 = nullIndex, 
			   const Index & E =nullIndex,   
			   const Index & C =nullIndex );

  // efficient version for derivatives requiring a scalar too.*** this version fills in an array ***
  virtual int assignCoefficients(const derivativeTypes & derivativeType,
			   realArray & coeff, 
			   const realArray & scalar, 
			   const Index & I1 = nullIndex, 
			   const Index & I2 = nullIndex, 
			   const Index & I3 = nullIndex, 
			   const Index & E =nullIndex,   
			   const Index & C =nullIndex );
  // evaluate a derivative
  // this is an efficient version (in both memory and speed).
  virtual int derivative(const derivativeTypes & derivativeType,
			 const realArray & u, 
			 realArray  & ux,
			 const Index & I1 = nullIndex, 
			 const Index & I2 = nullIndex, 
			 const Index & I3 = nullIndex, 
			 const Index & C =nullIndex );
  // version that takes a scalar
  virtual int derivative(const derivativeTypes & derivativeType,
			 const realArray & u, 
			 const realArray & scalar, 
			 realArray  & ux,
			 const Index & I1 = nullIndex, 
			 const Index & I2 = nullIndex, 
			 const Index & I3 = nullIndex, 
			 const Index & C =nullIndex );
 #ifdef USE_PPP
  // *** these versions fill in a serial array ***
  // this is an efficient version (in both memory and speed).
  virtual int assignCoefficients(const derivativeTypes & derivativeType,
			   realSerialArray & coeff, 
			   const Index & I1 = nullIndex, 
			   const Index & I2 = nullIndex, 
			   const Index & I3 = nullIndex, 
			   const Index & E =nullIndex,   
			   const Index & C =nullIndex );

  // efficient version for derivatives requiring a scalar too.*** this version fills in an array ***
  virtual int assignCoefficients(const derivativeTypes & derivativeType,
			   realSerialArray & coeff, 
			   const realSerialArray & scalar, 
			   const Index & I1 = nullIndex, 
			   const Index & I2 = nullIndex, 
			   const Index & I3 = nullIndex, 
			   const Index & E =nullIndex,   
			   const Index & C =nullIndex );
  // evaluate a derivative
  // this is an efficient version (in both memory and speed).
  virtual int derivative(const derivativeTypes & derivativeType,
			 const realSerialArray & u, 
			 realSerialArray  & ux,
			 const Index & I1 = nullIndex, 
			 const Index & I2 = nullIndex, 
			 const Index & I3 = nullIndex, 
			 const Index & C =nullIndex );
  // version that takes a scalar
  virtual int derivative(const derivativeTypes & derivativeType,
			 const realSerialArray & u, 
			 const realSerialArray & scalar, 
			 realSerialArray  & ux,
			 const Index & I1 = nullIndex, 
			 const Index & I2 = nullIndex, 
			 const Index & I3 = nullIndex, 
			 const Index & C =nullIndex );

 #endif

  int derivativeInternal(const derivativeTypes & derivativeType_,
			 const realSerialArray & u, 
			 const realSerialArray & scalar, 
			 realSerialArray & ux, 
			 const Index & I1_ = nullIndex, 
			 const Index & I2_ = nullIndex, 
			 const Index & I3_ = nullIndex, 
			 const Index & C =nullIndex );

  int assignCoefficientsInternal(const derivativeTypes & derivativeType_,
				 realSerialArray & coeff, 
				 const realSerialArray & scalar, 
				 const Index & I1  = nullIndex , 
				 const Index & I2  = nullIndex , 
				 const Index & I3  = nullIndex , 
				 const Index & E  = nullIndex ,   
				 const Index & C  = nullIndex  );


protected:
//   enum BoundaryConditionOption
//   {
//     scalarForcing=0,
//     arrayForcing,
//     gridFunctionForcing
//   };
  // Here are different ways we can assign the data for boundary conditions -- these are the same as
  // in BoundaryConditionsParameters -- 
  enum BoundaryConditionOption
  {
    unSpecifiedForcing=-1,
    scalarForcing=0,
    vectorForcing,
    vectorByFaceForcing,
    arrayForcing,
    gridFunctionForcing
  };


  void setup();  // used by constructors

  // This routine computes the general derivative (protected because it returns by reference)
  virtual realMappedGridFunction xi(const derivativeTypes & derivativeType,  
				      const realMappedGridFunction & u,
				      const Index & I1 = nullIndex, 
				      const Index & I2 = nullIndex, 
				      const Index & I3 = nullIndex, 
				      const Index & C1 = nullIndex,
				      const Index & C2 = nullIndex,
				      const Index & C3 = nullIndex );

  virtual realMappedGridFunction xiCoefficients(const derivativeTypes & derivativeType, 
						  const Index & I1 = nullIndex, 
						  const Index & I2 = nullIndex, 
						  const Index & I3 = nullIndex,
						  const Index & E  = nullIndex,
						  const Index & C  = nullIndex );

  virtual void computeDerivatives(const int & numberOfDerivatives,
				  const IntegerArray & derivativesToEvaluate,  
				  RealDistributedArray *derivative[],
				  const realMappedGridFunction & u, 
				  const Index & I1,
				  const Index & I2, 
				  const Index & I3, 
				  const Index & N,
				  const bool & checkArrayDimensions = TRUE );

  // compute pseudo-spectral derivatives
  virtual void spectralDerivatives(const int & numberOfDerivatives,
				   const IntegerArray & derivativesToEvaluate,  
				   RealDistributedArray *derivative[],
				   const realMappedGridFunction & u, 
				   const Range & R1,
				   const Range & R2, 
				   const Range & R3, 
				   const Range & R4);

  virtual void updateDerivativeFunctions();
  

  // ------------- Here we define the Boundary Conditions ---------------

 public:
    

  // new BC interface:
  void applyBoundaryCondition(realMappedGridFunction & u, 
                              const Index & Components,
			      const BCTypes::BCNames & boundaryConditionType=BCTypes::dirichlet,
                              const int & boundaryCondition = allBoundaries,
                              const real & forcing = 0.,
                              const real & time = 0.,
                              const BoundaryConditionParameters & bcParameters 
			      = Overture::defaultBoundaryConditionParameters(),
			      const int & grid=0 );
  

  void applyBoundaryCondition(realMappedGridFunction & u, 
                              const Index & Components,
                              const BCTypes::BCNames & boundaryConditionType,
                              const int & boundaryCondition,
                              const RealArray & forcing,
                              const real & time = 0.,
                              const BoundaryConditionParameters & bcParameters 
			      = Overture::defaultBoundaryConditionParameters(),
			      const int & grid=0 );
  
  virtual void applyBoundaryCondition(realMappedGridFunction & u, 
				      const Index & Components,
				      const BCTypes::BCNames & boundaryConditionType,
				      const int & boundaryCondition,
				      const RealArray & forcing,
				            RealArray *forcinga[2][3],
				      const real & time = 0.,
				      const BoundaryConditionParameters & bcParameters 
				                                     = Overture::defaultBoundaryConditionParameters(),
                                      const int & grid=0 );
  
  void applyBoundaryCondition(realMappedGridFunction & u, 
                              const Index & Components,
                              const BCTypes::BCNames & boundaryConditionType,
                              const int & boundaryCondition,
                              const realMappedGridFunction & forcing,
                              const real & time = 0.,
                              const BoundaryConditionParameters & bcParameters 
			      = Overture::defaultBoundaryConditionParameters(),
			      const int & grid=0 );
  
#ifdef USE_PPP
  // this version takes a distributed array "forcing"
//   void applyBoundaryCondition(realMappedGridFunction & u, 
//                               const Index & Components,
//                               const BCTypes::BCNames & boundaryConditionType,
//                               const int & boundaryCondition,
//                               const RealDistributedArray & forcing,
//                               const real & time = 0.,
//                               const BoundaryConditionParameters & bcParameters 
// 			      = Overture::defaultBoundaryConditionParameters(),
// 			      const int & grid=0 );
#endif
  // fill in the BC's for a coefficient grid function
  void applyBoundaryConditionCoefficients(realMappedGridFunction & coeff, 
				          const Index & Equations,
					  const Index & Components,
					  const BCTypes::BCNames & boundaryConditionType=BCTypes::dirichlet,
					  const int & boundaryCondition= allBoundaries,
					  const BoundaryConditionParameters & bcParameters 
                                          = Overture::defaultBoundaryConditionParameters(),
					  const int & grid=0 );

  // fix corners and periodic update:


  // private function to update arrays to be the correct size
  void updateBoundaryConditionArrays(const int newNumberOfBoundaryConditions, 
				     const int newNumberOfComponents );

  // private boundary condition routine:
  void applyBoundaryCondition(realMappedGridFunction & u, 
                              const Index & Components,
                              const BCTypes::BCNames & boundaryConditionType,
                              const int & boundaryCondition,
                              const real & forcing1,
                              const RealArray & forcing2,
                              const RealArray & forcing2d,
                                    RealArray *forcinga[2][3],
                              const realMappedGridFunction & forcing3,
                              const real & time,
                              const BoundaryConditionParameters & bcParameters,
                              const BoundaryConditionOption bcOption,
			      const int & grid=0   );

  // Here are functions that implement particular boundary conditions

  int getVelocityComponents(int & n1,
			    int & n2,
			    int & n3,
			    int & m1,
			    int & m2,
			    int & m3, 
			    realMappedGridFunction & u,
			    const BoundaryConditionParameters & bcParameters,
			    const aString & bcName, const IntegerArray & uC, const IntegerArray & fC );
  

  void applyBCaDotU(realMappedGridFunction & u, 
		    const int side,
		    const int axis,
		    const Index & Components,
		    const BCTypes::BCNames & boundaryConditionType,
		    const int & boundaryCondition,
		    const real & forcing1,
		    const RealArray & forcing2,
                    const RealArray & forcing2d,
		    const realMappedGridFunction & forcing3,
		    const real & time,
                    const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
		    const BoundaryConditionParameters & bcParameters,
		    const BoundaryConditionOption bcOption,
		    const int & grid=0  );

  void applyBCaDotGradU(realMappedGridFunction & u, 
			const int side,
			const int axis,
			const Index & Components,
			const BCTypes::BCNames & boundaryConditionType,
			const int & boundaryCondition,
			const real & forcing1,
			const RealArray & forcing2,
			const RealArray & forcing2d,
			const realMappedGridFunction & forcing3,
			const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
			const BoundaryConditionParameters & bcParameters,
			const BoundaryConditionOption bcOption,
			const int & grid=0  );

  void applyBCdirichlet(realMappedGridFunction & u, 
			const int side,
			const int axis,
			const Index & Components,
			const BCTypes::BCNames & boundaryConditionType,
			const int & boundaryCondition,
			const real & forcing1,
			const RealArray & forcing2,
			const RealArray & forcing2d,
			const realMappedGridFunction & forcing3,
			const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask, 
			const BoundaryConditionParameters & bcParameters,
			const BoundaryConditionOption bcOption,
			const int & grid=0  );

  void applyBCextrapolate(realMappedGridFunction & u, 
			  const int side,
			  const int axis,
			  const Index & Components,
			  const BCTypes::BCNames & boundaryConditionType,
			  const int & boundaryCondition,
			  const real & forcing1,
			  const RealArray & forcing2,
			  const RealArray & forcing2d,
			  const realMappedGridFunction & forcing3,
			  const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
			  const BoundaryConditionParameters & bcParameters,
			  const BoundaryConditionOption bcOption,
			  const int & grid=0  );

  void applyBCgeneralMixedDerivative(realMappedGridFunction & u, 
				     const int side,
				     const int axis,
				     const Index & Components,
				     const BCTypes::BCNames & boundaryConditionType,
				     const int & boundaryCondition,
				     const real & forcing1,
				     const RealArray & forcing2,
				     const RealArray & forcing2d,
				     const realMappedGridFunction & forcing3,
				     const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, IntegerDistributedArray & mask,
				     const BoundaryConditionParameters & bcParameters,
				     const BoundaryConditionOption bcOption,
				     const int & grid=0  );

  void applyBCnormalComponent(realMappedGridFunction & u, 
			      const int side,
			      const int axis,
			      const Index & Components,
			      const BCTypes::BCNames & boundaryConditionType,
			      const int & boundaryCondition,
			      const real & forcing1,
			      const RealArray & forcing2,
                              const RealArray & forcing2d,
			      const realMappedGridFunction & forcing3,
			      const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
			      const BoundaryConditionParameters & bcParameters,
			      const BoundaryConditionOption bcOption,
			      const int & grid=0  );

  void applyBCtangentialComponent(realMappedGridFunction & u, 
				  const int side,
				  const int axis,
				  const Index & Components,
				  const BCTypes::BCNames & boundaryConditionType,
				  const int & boundaryCondition,
				  const real & forcing1,
				  const RealArray & forcing2,
				  const RealArray & forcing2d,
				  const realMappedGridFunction & forcing3,
				  const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
				  const BoundaryConditionParameters & bcParameters,
				  const BoundaryConditionOption bcOption,
				  const int & grid=0  );

  void applyBCnormalDerivative(realMappedGridFunction & u, 
			       const int side,
			       const int axis,
			       const Index & Components,
			       const BCTypes::BCNames & boundaryConditionType,
			       const int & boundaryCondition,
			       const real & forcing1,
			       const RealArray & forcing2,
			       const RealArray & forcing2d,
			       const realMappedGridFunction & forcing3,
			       const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
			       const BoundaryConditionParameters & bcParameters,
			       const BoundaryConditionOption bcOption,
			       const int & grid=0  );

  void applyBCneumann(realMappedGridFunction & u, 
		      const int side,
		      const int axis,
		      const Index & Components,
		      const BCTypes::BCNames & boundaryConditionType,
		      const int & boundaryCondition,
		      const real & forcing1,
		      const RealArray & forcing2,
		      const RealArray & forcing2d,
		      const realMappedGridFunction & forcing3,
		      const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, IntegerDistributedArray & mask,
		      const BoundaryConditionParameters & bcParameters,
		      const BoundaryConditionOption bcOption ,
		      const int & grid=0 );

  void applyBCsymmetry(realMappedGridFunction & u, 
		       const int side,
		       const int axis,
		       const Index & Components,
		       const BCTypes::BCNames & boundaryConditionType,
		       const int & boundaryCondition,
		       const real & forcing1,
		       const RealArray & forcing2,
		       const RealArray & forcing2d,
		       const realMappedGridFunction & forcing3,
		       const real & time,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
		       const BoundaryConditionParameters & bcParameters,
		       const BoundaryConditionOption bcOption,
		       const int & grid=0  );


  void applyBCaDotGradU(realMappedGridFunction & u, 
			const int side,
			const int axis,
			const real & scalarData,
			const RealArray & arrayData,
			const RealArray & forcing2d,
			const realMappedGridFunction & gfData,
			const real & t,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
			const BoundaryConditionParameters & bcParameters,
			const BoundaryConditionOption bcOption,
			const int & grid=0  );

  void applyBCGenDiv(realMappedGridFunction & u, 
		     const int side,
		     const int axis,
		     const real & scalarData,
		     const RealArray & arrayData,
		     const RealArray & forcing2d,
		     const realMappedGridFunction & gfData,
		     const real & t,
                        const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
		     const BoundaryConditionParameters & bcParameters,
		     const BoundaryConditionOption bcOption,
		     const int & grid=0  );

  void applyBCnormalDotScalarGrad(realMappedGridFunction & u, 
				  const int side,
				  const int axis,
				  const Index & Components,
				  const BCTypes::BCNames & boundaryConditionType,
				  const int & boundaryCondition,
				  const real & forcing1,
				  const RealArray & forcing2,
				  const RealArray & forcing2d,
				  const realMappedGridFunction & forcing3,
				  const real & time,
                                  const IntegerArray & uC, const IntegerArray & fC, IntegerDistributedArray & mask,
				  const BoundaryConditionParameters & bcParameters,
				  const BoundaryConditionOption bcOption ,
				  const int & grid=0 );


  //! Build the coefficient matrix used to apply the neumann BC
void buildNeumannCoefficientMatrix(MappedGridOperators & op,
				   realSerialArray & nmCoeff, Index & M, Index &I1, Index &I2,Index &I3,
				   realSerialArray & normalLocal, real b0, real b1, int numberOfDimensions,
				   int side, int axis, const BoundaryConditionParameters & bcParameters );


 public:
  //  RealDistributedArray ur,us,ut;
  RealDistributedArray *urp, *usp, *utp;
  real dx[3];  // holds grid spacing for rectangular grids
  int width,halfWidth1,halfWidth2,halfWidth3;
  int useNewOperators; // use new optimized operators

 protected:

  // allocate coeff. matrix for a boundary condition is necessary
  bool createBoundaryMatrix(const int & side, 
                            const int & axis,
                            const BCTypes::BCNames & boundaryConditionType);

  int numberOfDimensions;
  bool rectangular;            // true if the grid is rectangular


  int numberOfDerivativesToEvaluate;                          // holds user specified values
  RealDistributedArray *derivativeArray[numberOfDifferentDerivatives];   // holds user specified values
  IntegerArray derivativeType;                                    // holds user specified values

  bool boundaryData;                       // true if a rhs is supplied for the boundary conditions

  // This next array holds pointers to derivative functions
  //  derivativeFunction[0]= a function to compute the x derivative
  //  derivativeFunction[1]= a function to compute the y derivative, etc.
  DerivativeFunctionPointer derivativeFunction[numberOfDifferentDerivatives];
  DerivCoefficientsFunctionPointer derivCoefficientsFunction[numberOfDifferentDerivatives];


  realSerialArray *neumannCoeff[3];
  int nCoeffIsSet[3][2];           // true if neumannCoeff is used on the boundary
  realSerialArray *mixedDerivativeCoeff[3];
  int mCoeffIsSet[3][2];                   real mCoeffValues[3*2*2]; // we save the coefficents that define the mixed BC
  realSerialArray *aDotGradUCoeff[3];
  int aCoeffIsSet[3][2];                   real aCoeffValues[3*2*3];
  realSerialArray *generalMixedDerivativeCoeff[3];
  int gCoeffIsSet[3][2];                   real gCoeffValues[3*2*4];
  realSerialArray *generalizedDivergenceCoeff[3];
  int gdCoeffIsSet[3][2];                 
  realSerialArray *normalDotScalarGradCoeff[3];
  int normalDotScalarGradCoeffIsSet[3][2];
  
  FourierOperators *fourierOperators;

  bool boundaryNormalsUsed;     // indicates if any BC's use the vertexBoundaryNormal
  bool boundaryTangentsUsed;    // indicates if any BC's use the centerBoundaryTangent vectors
  
  OGFunction *e;
  
  real useWhereMaskOnBoundary[3][2];   // TRUE if we should use a where mask on a given boundary
  

 public:

/* ----
  // ****** These are from the OLD BC interface -- keep here in case we want to resurrect them *****

  // indicate how many boundary conditions are applied on each side
  void setNumberOfBoundaryConditions(const int & number, 
				     const int & side=forAll, 
                                     const int & axis=forAll );
    
  // associate a boundary condition with a given boundary
  void setBoundaryCondition(const int & index,          // boundary condition number
			    const int & side,           // which boundary side
			    const int & axis,           // which boundary axis
			    const boundaryConditionTypes & boundaryConditionType, 
			    const int & component,      // apply to this component
                            const int & parameter1=defaultValue,   // optional parameters
                            const int & parameter2=defaultValue,
                            const int & parameter3=defaultValue );

  // associate a boundary condition with a given boundary
  // use this function when a boundary condition involves more than one component such
  // as setting the normal component of a vector
  void setBoundaryCondition(const int & index,          // boundary condition number
			    const int & side,           // which boundary side
			    const int & axis,           // which boundary axis
			    const boundaryConditionTypes & boundaryConditionType, 
			    const IntegerArray & component );   // apply to these components
  

  // set all boundary conditions to the given type
  void setBoundaryCondition(const boundaryConditionTypes & boundaryConditionType ); 

  // General mixed derivative boundary condition with constant coefficients:
  //   B(0)*u + B(1)*u.x + B(2)*u.y + B(3)*u.z = 
  void setBoundaryCondition(const int & index,          // boundary condition number
			    const int & side,           // which boundary side
			    const int & axis,           // which boundary axis
			    const boundaryConditionTypes & boundaryConditionType, 
                            const int & component,      // apply to this component
			    const RealArray & B );
    
  // supply scale values b(i) and multiple components, use this for aDotGradU
  void setBoundaryCondition(const int & index,          
			    const int & side,           
			    const int & axis,           
			    const boundaryConditionTypes & boundaryConditionType, 
			    const IntegerArray & component, 
			    const RealArray & b );


  // General mixed derivative boundary condition with non-constant coefficients:
  //   B(0,I1,I2,I3)*u + B(1,I1,I2,I3)*u.x + B(2,I1,I2,I3)*u.y + B(3,I1,I2,I3)*u.z = 
  void setBoundaryCondition(const int & index,          // boundary condition number
			    const int & side,           // which boundary side
			    const int & axis,           // which boundary axis
			    const boundaryConditionTypes & boundaryConditionType, 
                            const int & component,      // apply to this component
			    const realMappedGridFunction & B );
    
  //
  // Supply a boundary value for a boundary condition (or turn off this option)
  // 
  void setBoundaryConditionValue(const real & value,  
				 const int & index, 
                                 const int & side, 
                                 const int & axis,
                                 const bool & trueOrFalse=TRUE);
                                 

  // Indicate which faces should use the boundaryConditionRightHandSide array
  void setBoundaryConditionRightHandSide(const int & index, 
					 const int & side, 
					 const int & axis,
					 const bool & trueOrFalse=TRUE);
                                 
  // supply a grid function that gives the "data" for the boundary condition
  void setBoundaryConditionRightHandSide( const realMappedGridFunction & boundaryConditionRightHandSide );



  void applyBoundaryConditions(realMappedGridFunction & u, const real & time = 0.);
  void ApplyBoundaryConditions(realMappedGridFunction & coeff, const real & time = 0.); // fill in coefficients
//   void fixBoundaryCorners(realMappedGridFunction & u);

----------- */
};






#endif
