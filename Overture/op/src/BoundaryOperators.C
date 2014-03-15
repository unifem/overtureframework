#include "MappedGridOperators.h"
#include "UnstructuredOperators.h"
#include "SparseRep.h"
#include "conversion.h"
#include "display.h"
#include "ParallelUtility.h"
#include "AssignInterpNeighbours.h"

// extern realMappedGridFunction Overture::nullRealMappedGridFunction();

#define extrapInterpNeighboursOpt EXTERN_C_NAME(extrapinterpneighboursopt)
extern "C"
{
  void extrapInterpNeighboursOpt(const int&nd, 
    const int&ndu1a,const int&ndu1b,const int&ndu2a,const int&ndu2b,const int&ndu3a,const int&ndu3b,
    const int&ndu4a,const int&ndu4b,
    const int&nda1a,const int&nda1b,const int&ndd1a,const int&ndd1b,
    const int&ia,const int&id, const int &vew, real & u,const int&ca,const int&cb, const int& ipar, const real&rpar );
}



//========================================================================================
//  This file defines the Boundary condition routines for Vertex Based Grid Functions
//
//  Notes:
//     See the documentation for further details
//========================================================================================

#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

int MappedGridOperators::
getVelocityComponents(int & n1,
		      int & n2,
		      int & n3,
		      int & m1,
		      int & m2,
		      int & m3, 
		      realMappedGridFunction & u,
		      const BoundaryConditionParameters & bcParameters,
		      const aString & bcName, const IntegerArray & uC, const IntegerArray & fC )
// 
// This is a utility routine for applyBoundaryCondition
//   n1,n2,n3 (output) : components of velocity for certain boundary conditions, extracted from uC
//   m1,m2,m3 (output) : values taken from fC that correspond to n1,n2,n3
{
  if( uC.getLength(0)<mappedGrid.numberOfDimensions() )
  {
    printf("MappedGridOperators::ERROR: Not enough components specified for a boundary condition\n");
    Overture::abort("error");
  }
  
  int i=uC.getBase(0);
  n1=uC(i);
  n2=mappedGrid.numberOfDimensions()>1 ? uC(i+1) : n1;
  n3=mappedGrid.numberOfDimensions()>2 ? uC(i+2) : n2;
  
  m1=fC(i);
  m2=mappedGrid.numberOfDimensions()>1 ? fC(i+1) : m1;
  m3=mappedGrid.numberOfDimensions()>2 ? fC(i+2) : m2;
  if( min(min(n1,n2),n3)<u.getComponentBase(0) || max(max(n1,n2),n3) > u.getComponentBound(0) )
  {
    cout << "MappedGridOperators::applyBoundaryConditions:ERROR applying a " << (const char*) bcName << "  BC\n";
    printf("There is an invalid component, component0=%i, component1=%i",n1,n2);
    if( mappedGrid.numberOfDimensions()==3 )
      printf(", component2=%i ",n3);
    printf("\nu.getComponentBase(0) = %i, u.getComponentBound(0)=%i \n",	    
	   u.getComponentBase(0),u.getComponentBound(0));
    bcParameters.uComponents.display("Here is bcParameters.uComponents");
    bcParameters.fComponents.display("Here is bcParameters.fComponents");
    uC.display("Here is uC");
    fC.display("Here is fC");
    Overture::abort("error");
  }
  return 0;
}


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{applyBoundaryCondition}}  
void MappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType,  /* = BCTypes::dirichlet */
		       const int & bc,                   /* = BCTypes::allBoundaries */
		       const real & forcing,             /* =0. */
		       const real & time,                /* =0. */
		       const BoundaryConditionParameters & 
		       bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid /* =0 */ )
//=======================================================================================
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  RealArray *forcinga[2][3]={NULL,NULL,NULL,NULL,NULL,NULL};
  applyBoundaryCondition(u,Components,bcType,bc,forcing,Overture::nullRealArray(),Overture::nullRealArray(),forcinga,
                         Overture::nullRealMappedGridFunction(),time,bcParameters,scalarForcing,grid);
}


//\begin{>>MappedGridOperatorsInclude.tex}{}
void MappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType, 
		       const int & bc,                  
		       const RealArray & forcing,
		       const real & time,              /* =0. */
		       const BoundaryConditionParameters & 
		       bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid /* =0 */)
//=======================================================================================
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  real forcing1;
  RealArray *forcinga[2][3]={NULL,NULL,NULL,NULL,NULL,NULL};
  
#ifndef USE_PPP
  // *wdh* 050411 --- need to fix this ----
//   applyBoundaryCondition(u,Components,bcType,bc,forcing1,forcing,Overture::nullRealArray(),forcinga,
//                          Overture::nullRealMappedGridFunction(),time,
//                          bcParameters,arrayForcing,grid);
  // *wdh* 050528
//   applyBoundaryCondition(u,Components,bcType,bc,forcing1,forcing,forcing,forcinga,
//                          Overture::nullRealMappedGridFunction(),time,
//                          bcParameters,arrayForcing,grid);

  if( bcParameters.getBoundaryConditionForcingOption()==BoundaryConditionParameters::arrayForcing )
  {
    // array forcing must be explicitly specified:
    applyBoundaryCondition(u,Components,bcType,bc,forcing1,Overture::nullRealArray(),forcing,forcinga,
			   Overture::nullRealMappedGridFunction(),time, bcParameters,arrayForcing,grid);
  }
  else
  {
    // vector forcing:
    // *** we first need to go through all BC's and add the vector forcing option ****
    applyBoundaryCondition(u,Components,bcType,bc,forcing1,forcing,Overture::nullRealArray(),forcinga,
			   Overture::nullRealMappedGridFunction(),time, bcParameters,arrayForcing,grid);
    // *******	   Overture::nullRealMappedGridFunction(),time, bcParameters,vectorForcing,grid);
  }

#else
  // array forcing in parallel must be a distributed array so we must be using vector forcing:
  applyBoundaryCondition(u,Components,bcType,bc,forcing1,forcing,Overture::nullRealArray(),forcinga,
                         Overture::nullRealMappedGridFunction(),time, bcParameters,arrayForcing,grid);
#endif

  
}

//\begin{>>MappedGridOperatorsInclude.tex}{}
void MappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType, 
		       const int & bc,                  
		       const RealArray & forcing,
		       RealArray *forcinga[2][3],
		       const real & time,              /* =0. */
		       const BoundaryConditionParameters & 
		       bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid /* =0 */)
//=======================================================================================
// /Description:
//  If forcinga[side][axis] !=NULL then use this array, otherwise use forcing.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  real forcing1;
  applyBoundaryCondition(u,Components,bcType,bc,forcing1,forcing,Overture::nullRealArray(),forcinga,
                         Overture::nullRealMappedGridFunction(),time,
                         bcParameters,arrayForcing,grid);
}


#ifdef USE_PPP
// void MappedGridOperators::
// applyBoundaryCondition(realMappedGridFunction & u, 
// 		       const Index & Components,
// 		       const BCTypes::BCNames & bcType, 
// 		       const int & bc,                  
// 		       const RealArray & forcing,
// 		       const real & time,              /* =0. */
// 		       const BoundaryConditionParameters & 
// 		       bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
// 		       const int & grid /* =0 */)
// //=======================================================================================
// // /Description:
// //   This version takes a distributed array as forcing (only used in parallel).
// //=======================================================================================
// {
//   real forcing1;
//   RealArray *forcinga[2][3]={NULL,NULL,NULL,NULL,NULL,NULL};
//   applyBoundaryCondition(u,Components,bcType,bc,forcing1,Overture::nullRealArray(),forcing,forcinga,
//                          Overture::nullRealMappedGridFunction(),time,
//                          bcParameters,arrayForcing,grid);
// }
#endif

//\begin{>>MappedGridOperatorsInclude.tex}{}
void MappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,                        
		       const realMappedGridFunction & forcing,
		       const real & time,                      /* =0. */
		       const BoundaryConditionParameters & 
		       bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid /* =0 */)
//=======================================================================================
// /Description:
//  Apply a boundary condition to a grid function.
//  This routine implements every boundary condition known to man (ha!)
//
// /u (input/output): apply boundary conditions to this grid function.
// /Components (input): apply to these components
// /bcType  (input): the name of the boundary condition to apply (dirichlet, neumann,...)
// /bc (input): apply the boundary condition on all sides of the grid where the
//     boundaryCondition array (in the MappedGrid) is equal to this value. By default
//     {\tt bc=BCTypes allBoundaries} apply to all boundaries (with a positive value for boundaryCondition).
//     To apply a boundary condition to a specified side use
//         \begin{itemize}
//           \item {\tt bc=BCTypes::boundary1} for $(side,axis)=(0,0)$
//           \item {\tt bc=BCTypes::boundary2} for $(side,axis)=(1,0)$
//           \item {\tt bc=BCTypes::boundary3} for $(side,axis)=(0,1)$
//           \item {\tt bc=BCTypes::boundary4} for $(side,axis)=(1,1)$
//           \item {\tt bc=BCTypes::boundary5} for $(side,axis)=(0,2)$
//           \item {\tt bc=BCTypes::boundary6} for $(side,axis)=(1,2)$
//         \end{itemize}
//     or use {\tt bc=BCTypes::boundary1+side+2*axis} for given values of $(side,axis)$ (this 
//    could be used in a loop, for example).
// /forcing (input): This value is used as a forcing for the boundary condition, if needed. 
// /time (input): apply boundary conditions at this time (used by twilightZoneFlow)
// /bcParameters (input): optional parameters are passed using this object.
//   See the examples for how to pass parameters with this argument.
// /Limitations:
//  only second order accurate.
//
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  real forcing1=0.;
  RealArray *forcinga[2][3]={NULL,NULL,NULL,NULL,NULL,NULL};
  applyBoundaryCondition(u,Components,bcType,bc,forcing1,Overture::nullRealArray(),Overture::nullRealArray(),
                         forcinga,forcing,time,bcParameters,gridFunctionForcing,grid);
}






// Private BC routine
void MappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & C0,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const real & scalarData,
		       const RealArray & arrayData,
		       const RealArray & arrayDataD_,
                       RealArray *forcinga[2][3],
		       const realMappedGridFunction & gfData,
		       const real & t,
		       const BoundaryConditionParameters & bcParameters,
		       const BoundaryConditionOption bcOption,
		       const int & grid /* =0 */  )
// =============================================================================================
// This is the main BC routine that farms out the work to difference BC routines
// =============================================================================================
{
  real time0=getCPU();

  if( numberOfDimensions==0 )
  {
    cout << "MappedGridOperators::ERROR: you must assign a MappedGrid before applyBoundaryConditions! \n";
    return;
  }
  assert( numberOfDimensions==1 || numberOfDimensions==2 || numberOfDimensions==3 );


  int side,axis,n,m;

// --- define these in the Class ***
//  Range R[3];
//  Index I1,I2,I3,I1m,I2m,I3m,I1p,I2p,I3p,I1e,I2e,I3e,M;
//  RealDistributedArray coeff, opX,opY,opZ,norm;
//  real b0,b1,b2,b3;

  // If C0 is an null Range then use all components
  Range CC = C0.length()>0 ? C0 : Index(u.getComponentBase(0),u.getComponentDimension(0));
  Range Cgf = CC-CC.getBase()+gfData.getComponentBase(0); // Range for gfData
  

  // *wdh* 000905  MappedGrid & c = mappedGrid; 
  MappedGrid & c = *u.getMappedGrid();

  RealDistributedArray & uA = u;             // use this reference to simplify some statements A=array

  IntegerDistributedArray mask;

  e = twilightZoneFlowFunction;   // "e"xact solution  
  if( twilightZoneFlow )
    assert( twilightZoneFlowFunction != NULL );


  // uC and fC will hold the components that we operate on
  //  uC =  C            : if neither uComponents nor fComponents is specified
  //     =  uComponents  : if uComponents is given
  //     =  b,b+1,...    : if fComponents is specified but not uComponents, b=u.getComponentBase(0)
  // 
  //  fC =  C            : if neither uComponents nor fComponents is specified
  //     =  b,b+1,...    : if as above case but with grid function forcing, b=forcing.getComponentBase(0)
  //     =  fComponents  : if fComponents is given 
  //     =  b,b+1,...    : if uComponents is specified but not fComponents, b=forcing.getComponentBase(0)
  // IntegerArray uC, fC;
  bool uComponentsSpecified=bcParameters.uComponents.getLength(0) > 0;
  bool fComponentsSpecified=bcParameters.fComponents.getLength(0) > 0;

  IntegerArray uC,fC;
  if( uComponentsSpecified )
  {
    uC.redim(0);
    uC=bcParameters.uComponents;
  }
  else
  {
    if( fComponentsSpecified )
    { // uC = b,b+1,...
      uC.redim(Range(Index(u.getComponentBase(0),bcParameters.fComponents.getLength(0))));
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
        uC(n)=n;
    }
    else
    { // uC = C 
      uC.redim(Range(CC));
      for( n=CC.getBase(); n<=CC.getBound(); n+=CC.getStride() )
        uC(n)=n;
    }
  }
  if( fComponentsSpecified )
  {
    fC.redim(0);
    fC=bcParameters.fComponents;
  }
  else
  {
    if( uComponentsSpecified || bcOption==gridFunctionForcing )
    { // fC = b,b+1,...
      fC.redim(Range(uC.getBase(0),uC.getBound(0))); // *wdh* 970729: fC should index the same way as uC
      if( bcOption==gridFunctionForcing && !twilightZoneFlow )
      {
        for( n=Cgf.getBase(); n<=Cgf.getBound(); n+=Cgf.getStride() )
          fC(n-Cgf.getBase()+uC.getBase(0))=n;  // *wdh* 970729: fC should index the same way as uC
      }
      else
      {
        fC=uC;
      }
    }
    else
    { // fC = C
      fC.redim(Range(CC));
      for( n=CC.getBase(); n<=CC.getBound(); n+=CC.getStride() )
        fC(n)=n;
    }
  }

  
  // kkc defer to UnstructuredOperators::applyBoundaryCondition
  if ( u.getMappedGrid()->getGridType()==GenericGrid::unstructuredGrid )
    {
      UnstructuredOperators uop(mappedGrid);
      int bcop = int(bcOption);
      uop.applyBoundaryCondition(u, 
				 CC,
				 bcType,
				 bc,
				 scalarData,
				 arrayData,
				 arrayDataD_,
				 forcinga,
				 gfData,
				 t,
				 uC, fC, mask,
				 bcParameters,
				 bcop, // BoundaryConditionOption is protected for now
				 grid  );
      return;
    }


//   Range all;
//   uDotN.updateToMatchGrid( c,all,all,all); // ****************************************  why is this here?


  timeToSetupBoundaryConditions+=getCPU()-time0;
  
  if( bcType==BCTypes::extrapolateInterpolationNeighbours )
  {

    // Extrapolate the unused points that lie next to interpolation points
    // 
    //             e e e e
    //           e e I I I       e=extrapolate
    //           e I I X X       I= interpolation pt
    //           e I X X X       X= discretaization pt
    //           e I X X X
    real time1=getCPU();

    if( true )
    {
      // *wdh* 091123 -- new way --
      if( assignInterpNeighbours==NULL )
        assignInterpNeighbours = new AssignInterpNeighbours;

      if( interpolationPoint!=NULL )
        assignInterpNeighbours->setInterpolationPoint( *interpolationPoint );
      assignInterpNeighbours->assign( u, CC, bcParameters );
    }
    else
    {
      // *wdh* 091123 -- old way --

      if( !extrapolateInterpolationNeighboursIsInitialized )
      {

	findInterpolationNeighbours();

	if( errorStatus==errorInFindInterpolationNeighbours )
	{
	  printf("MappedGridOperators::applyBoundaryCondition:ERROR: error return from findInterpolationNeighbours\n");
	  return;
	}
      
      }
    
    
      bool useOpt=true;
      if( useOpt )
      {
	if( numberOfInterpolationNeighboursNew>0 )
	{
	  const IntegerArray & ia = *extrapolateInterpolationNeighbourPoints;
	  const IntegerArray & id = *extrapolateInterpolationNeighboursDirection;
	  const int *pia=getDataPointer(ia);
	  bool useVariableExtrapolationWidth=extrapolateInterpolationNeighboursVariableWidth!=NULL;
	  const int *pvew = useVariableExtrapolationWidth ? 
	    getDataPointer(*extrapolateInterpolationNeighboursVariableWidth) : pia;
	
#ifdef USE_PPP
  	  realSerialArray uLocal; ::getLocalArrayWithGhostBoundaries(uA,uLocal);
#else
  	  const realSerialArray & uLocal = uA;
#endif

	  int extrapOrder=bcParameters.orderOfExtrapolation;
	  if( extrapOrder > maximumWidthToExtrapolationInterpolationNeighbours-1 )
	  {
	    extrapOrder=maximumWidthToExtrapolationInterpolationNeighbours-1;
	    printF("MGOP:extrapInterpNeighbours:INFO: reducing order of extrapolation to %i\n",extrapOrder);
	  }
	  int ipar[]={maximumWidthToExtrapolationInterpolationNeighbours,
		      extrapOrder, 
		      (int)bcParameters.extrapolationOption,
		      (int)useVariableExtrapolationWidth
	  };//
	  const real uEps=1000.*REAL_MIN; // for limited extrapolation
	  real rpar[]={bcParameters.extrapolateWithLimiterParameters[0],
		       bcParameters.extrapolateWithLimiterParameters[1],
		       uEps}; //

	  extrapInterpNeighboursOpt(c.numberOfDimensions(), 
				    uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
				    uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
				    ia.getBase(0),ia.getBound(0),id.getBase(0),id.getBound(0),
				    *pia,*getDataPointer(id),*pvew, *getDataPointer(uLocal),
				    CC.getBase(),CC.getBound(),ipar[0],rpar[0] );
	}
      }
      else
      {
	// *** old way ****

	if( numberOfInterpolationNeighboursNew>0 )
	{
	  const IntegerArray & ia = *extrapolateInterpolationNeighbourPoints;
	  const IntegerArray & id = *extrapolateInterpolationNeighboursDirection;

	  // *** new way ***
	  // printf("extrapolateInterpolationNeighbours: *NEW* extrap to order %i. numberOfInterpolationNeighboursNew=%i\n",
	  //      bcParameters.orderOfExtrapolation,numberOfInterpolationNeighboursNew );
	  if( false )
	  {
	    printf("extrapolateInterpolationNeighbourPoints : grid=%i \n",grid);
	    display(ia,"extrapolateInterpolationNeighbourPoints","%3i");
	    display(id,"extrapolateInterpolationNeighboursDirection","%3i");
	  }
      

	  Range I=ia.dimension(0);
	  int i2=c.dimension(Start,axis2),i3=c.dimension(Start,axis3);

	  if( bcParameters.orderOfExtrapolation==3 || bcParameters.orderOfExtrapolation<=0 )
	  {
	    if( c.numberOfDimensions()==2 )
	    {
//           for( int i=0; i<numberOfInterpolationNeighboursNew; i++ )
// 	  {
// 	    if( ia(i,0)+3*id(i,0) < c.dimension(0,0) )
// 	    {
// 	      printf("ERROR: i=%i, ia=(%i,%i) id=(%i,%i) \n",i,ia(i,0),ia(i,1), id(i,0),id(i,1));
// 	    }
// 	  }
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),ia(I,1),i3,c0)=(3.*uA(ia(I,0)+  id(I,0),ia(I,1)+  id(I,1),i3,c0)-
					   3.*uA(ia(I,0)+2*id(I,0),ia(I,1)+2*id(I,1),i3,c0)+
					   uA(ia(I,0)+3*id(I,0),ia(I,1)+3*id(I,1),i3,c0));
	    }
	    else if( c.numberOfDimensions()==3 )
	    {
//           for( int i=0; i<numberOfInterpolationNeighboursNew; i++ )
// 	  {
// 	    if( ia(i,0)+3*id(i,0) < c.dimension(0,0) )
// 	    {
// 	      printf("ERROR: i=%i, ia=(%i,%i,%i) id=(%i,%i,%i) \n",i,ia(i,0),ia(i,1),ia(i,2),
// 		     id(i,0),id(i,1),id(i,2));
// 	    }
// 	  }
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),ia(I,1),ia(I,2),c0)=(3.*uA(ia(I,0)+  id(I,0),ia(I,1)+  id(I,1),ia(I,2)+  id(I,2),c0)-
						3.*uA(ia(I,0)+2*id(I,0),ia(I,1)+2*id(I,1),ia(I,2)+2*id(I,2),c0)+
						uA(ia(I,0)+3*id(I,0),ia(I,1)+3*id(I,1),ia(I,2)+3*id(I,2),c0));
	    }
	    else
	    {
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),i2,i3,c0)=(3.*uA(ia(I,0)+  id(I,0),i2,i3,c0)-
				      3.*uA(ia(I,0)+2*id(I,0),i2,i3,c0)+
				      uA(ia(I,0)+3*id(I,0),i2,i3,c0));
	    }
	  }
	  else if( bcParameters.orderOfExtrapolation==2 )
	  {
	    if( c.numberOfDimensions()==2 )
	    {
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),ia(I,1),i3,c0)=(2.*uA(ia(I,0)+  id(I,0),ia(I,1)+  id(I,1),i3,c0)-
					   uA(ia(I,0)+2*id(I,0),ia(I,1)+2*id(I,1),i3,c0));
	    }
	    else if( c.numberOfDimensions()==3 )
	    {
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),ia(I,1),ia(I,2),c0)=(2.*uA(ia(I,0)+  id(I,0),ia(I,1)+  id(I,1),ia(I,2)+  id(I,2),c0)-
						uA(ia(I,0)+2*id(I,0),ia(I,1)+2*id(I,1),ia(I,2)+2*id(I,2,c0)));

	    }
	    else
	    {
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),i2,i3,c0)=(2.*uA(ia(I,0)+  id(I,0),i2,i3,c0)-
				      uA(ia(I,0)+2*id(I,0),i2,i3,c0));

	    }
	  }
	  else if( bcParameters.orderOfExtrapolation==1 )
	  {
	    if( c.numberOfDimensions()==2 )
	    {
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),ia(I,1),i3,c0)=uA(ia(I,0)+  id(I,0),ia(I,1)+  id(I,1),i3,c0);
	    }
	    else if( c.numberOfDimensions()==3 )
	    {
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),ia(I,1),ia(I,2),c0)=uA(ia(I,0)+  id(I,0),ia(I,1)+  id(I,1),ia(I,2)+  id(I,2),c0);
	    }
	    else
	    {
	      for( int c0=CC.getBase(); c0<=CC.getBound(); c0++ )
		uA(ia(I,0),i2,i3,c0)=uA(ia(I,0)+  id(I,0),i2,i3,c0);
	    }
	  }
	  else
	  {
	    printf("extrapolateInterpolationNeighbours:ERROR: un-implemented order of extrapolation=%i\n"
		   " only orders 1,2,3 are available\n",bcParameters.orderOfExtrapolation==1 );
	    Overture::abort("error");
	  }
	
	}
      
      }
    } // end old way 
    
    timeForExtrapolateInterpolationNeighbours+=getCPU()-time1;
    

    // *now* do refinement grids (do this after the above points)
    if( c.isRefinementGrid() )
    {
      // **************************** fix this if we interpolate 2 ghost lines ************************
      // For refinement grids we must extrapolate the 2nd ghost line on interpolation boundaries
      real time1=getCPU();
      
      // cast away const:
      BoundaryConditionParameters & extrapParams = (BoundaryConditionParameters &)bcParameters;
      // extrapParams.ghostLineToAssign=1;  // *** this is correct, it will get the 2nd ghost line
      // extrapParams.orderOfExtrapolation=bcParameters.orderOfExtrapolation;

      const int ghostLineToAssignSave=bcParameters.ghostLineToAssign;
      // extrapParams.ghostLineToAssign=1;  // *** this is correct, it will get the 2nd ghost line
      // *wdh* 100813 -- change this to 2 and also change op/bc/extrapolate.bC to work correct at bc==0 boundaries.
      extrapParams.ghostLineToAssign=2;  // *** this is correct, it will get the 2nd ghost line

      for( axis=0; axis<c.numberOfDimensions(); axis++ )
      {
	for( side=0; side<=1; side++ )
	{
	  if( c.boundaryCondition(side,axis)==0 )
	  {
	    // printf(" applyBC: grid=%i, (refinement grid) extrapolate 2nd ghost line of (side,axis)=(%i,%i)\n",
            //     grid,side,axis);
            applyBCextrapolate(u, side,axis,CC,BCTypes::extrapolate,bc,scalarData,arrayData,arrayDataD_,gfData,t,
				uC,fC,mask,extrapParams,bcOption,grid);
	  }
	}
      }
      
      // fix up corners -- we need to temporarily turn interpolation boundaries (bc==0) into physical boundaries.
      MappedGrid & mg = *u.getMappedGrid();
      IntegerArray & mgbc = (IntegerArray&) mg.boundaryCondition(); // cast away const
      IntegerArray bc;
      bc=mgbc; // save a copy
      where( bc==0 )
        mgbc=123;

      const int lineToAssignSave=extrapParams.lineToAssign;
      extrapParams.lineToAssign=1;   // only fix corners for ghost line > 1
      fixBoundaryCorners(u,extrapParams,CC);
      

      mgbc=bc;  // reset
      extrapParams.ghostLineToAssign=ghostLineToAssignSave; // reset
      extrapParams.lineToAssign=lineToAssignSave;  // reset
      
      timeForExtrapolateRefinementBoundaries+=getCPU()-time1;
    } // end if c.isRefinementGrid
    
    
    timeForAllBoundaryConditions+=getCPU()-time0;
    return;   // ******
  } // end extrpolateInterpolationNeighbours
  else if(  bcType==BCTypes::extrapolateRefinementBoundaries )
  {
    if( c.isRefinementGrid() )
    {
      // For refinement grids we must extrapolate the 2nd ghost line on interpolation boundaries
      // cast away const:
      BoundaryConditionParameters & extrapParams = (BoundaryConditionParameters &)bcParameters;

      // extrapParams.ghostLineToAssign=0;  // *** this is correct, it will get the 1st ghost line
      // extrapParams.orderOfExtrapolation=bcParameters.orderOfExtrapolation;

      const int ghostLineToAssignSave=bcParameters.ghostLineToAssign;
      // extrapParams.ghostLineToAssign=0;  // *** this is correct, it will get the 1st ghost line
      // *wdh* 100813 -- change this to 1 and also change op/bc/extrapolate.bC to work correct at bc==0 boundaries.
      extrapParams.ghostLineToAssign=1; 

      for( axis=0; axis<c.numberOfDimensions(); axis++ )
      {
	for( side=0; side<=1; side++ )
	{
	  if( c.boundaryCondition(side,axis)==0 )
	  {
	    // printf(" applyBC: grid=%i, (refinement grid) extrapolate 2nd ghost line of (side,axis)=(%i,%i)\n",
            //     grid,side,axis);
            applyBCextrapolate(u, side,axis,CC,BCTypes::extrapolate,bc,scalarData,arrayData,arrayDataD_,gfData,t,
			       uC,fC,mask,extrapParams,bcOption,grid);
	  }
	}
      }
      extrapParams.ghostLineToAssign=ghostLineToAssignSave; // reset
    }
    timeForAllBoundaryConditions+=getCPU()-time0;
    return ;
  }
  

  int sideStart=0, sideEnd=1;
  int axisStart=0, axisEnd=c.numberOfDimensions()-1;
  bool doOnlyOneSide=FALSE;
  if( bc>=BCTypes::boundary1 && bc<=BCTypes::boundary6 )
  {
    // do only one side
    doOnlyOneSide=TRUE;
    sideStart=(bc-BCTypes::boundary1) % 2;
    sideEnd=sideStart;
    axisStart=(bc-BCTypes::boundary1)/2;
    axisEnd=axisStart;
    // printf(" sideStart=%i, axisStart=%i, bc-BCTypes::boundary1=%i\n",sideStart,axisStart,bc,BCTypes::boundary1);
    
  }

  Index I1m,I2m,I3m;
  
  for( axis=axisStart; axis<=axisEnd; axis++ )
  for( side=sideStart; side<=sideEnd; side++ )
  {
    if( c.boundaryCondition()(side,axis) > 0 && 
        ( doOnlyOneSide || c.boundaryCondition()(side,axis)==bc || bc==allBoundaries ) )
    {
      // getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign);
      
      getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParameters.extraInTangentialDirections); // first ghost line
      // getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-1); // first line in
      
      const RealArray & arrayDataD = forcinga[side][axis]!=NULL ? *forcinga[side][axis] : arrayDataD_;
      
      // The mask array indicates where to apply the BC. There are two possible mask arrays, one is
      // user defined and the other is needed for interior boundary points. We can usually just make
      // a reference to one or the other mask array, unless both are turned on and then we make a separate
      // copy that includes the effect of both.
      if( bcParameters.getUseMask() || ( useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) )
      {
	Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2]; // mask is evaluated at these points.
	switch ( bcType )
	{
	case dirichlet: 
	case normalComponent:
	case tangentialComponent0:
	case tangentialComponent1:
	case tangentialComponent:
	case aDotU:  
	  // *** J1=I1; J2=I2; J3=I3;  // take mask from boundary
          J1=I1m; J2=I2m; J3=I3m;  // take mask from first ghost line
	  break;
	case normalDotScalarGrad:
	case neumann:
	case mixed:
	case aDotGradU:
	case normalDerivativeOfNormalComponent:
	case normalDerivativeOfTangentialComponent0:
	case normalDerivativeOfTangentialComponent1:
	case evenSymmetry:
	case oddSymmetry:
	case vectorSymmetry:
	case generalMixedDerivative:  
	case generalizedDivergence:           
	  J1=I1m; J2=I2m; J3=I3m;  // take mask from first ghost line
	  break;
	case extrapolate:
	case extrapolateNormalComponent:
	case extrapolateTangentialComponent0:
	case extrapolateTangentialComponent1:
	  // *wdh* 000310 getGhostIndex( c.indexRange(),side,axis,J1,J2,J3,bcParameters.ghostLineToAssign);
	  getGhostIndex( c.extendedIndexRange(),side,axis,J1,J2,J3,bcParameters.ghostLineToAssign,bcParameters.extraInTangentialDirections);
          // *wdh* 091123: make sure that J1,J2,J3 are not too large in the tangential directions
          if( bcParameters.extraInTangentialDirections>0 )
	  {
            for( int dir=0; dir<c.numberOfDimensions(); dir++ )
	    {
              if( dir!=axis )
		Jv[dir]=Range(max(c.dimension(0,dir),Jv[dir].getBase()),min(c.dimension(1,dir),Jv[dir].getBound()));
	    }
	  }
	  
	  break;
	default:
	  printF("applyBoundaryCondition: unknown or un-implemented boundary condition = %i\n",(int)bcType);
	  Overture::abort("MappedGridOperators::applyBoundaryCondition: fatal error! \n");
	}

        const IntegerDistributedArray & cmask = c.mask();
        const IntegerDistributedArray & bmask = ((BoundaryConditionParameters&)bcParameters).mask();
        #ifndef USE_PPP
          mask.redim(J1,J2,J3);
        #else
          // in parallel we build a full mask array so the mask local arrays match with the solution
          if( mask.elementCount()==0 )
	  {
	    mask.partition(c.getPartition());
	    mask.redim(c.mask());
	  }
	#endif

	// *** fix me for parallel -- use maskLocal 

	if( !bcParameters.getUseMask() )
	{
          // we should NOT apply a BC at interpolation points or interiorBoundaryPoint
	  // mask=cmask(J1,J2,J3)>0 ||
  	  //     cmask(J1,J2,J3)< (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint);
          // *wdh* 990915 : use bit ops
	  mask(J1,J2,J3)=cmask(J1,J2,J3)>0 && 
            !( cmask(J1,J2,J3) & (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint));
	}
	else if( !(useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) )
	  mask(J1,J2,J3)=bmask(J1,J2,J3);
	else
	{
          // we should NOT apply a BC at interpolation points or interiorBoundaryPoint
	  // mask=(cmask(J1,J2,J3)>0 ||
	  // cmask(J1,J2,J3)< (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint) ) &&
	  //   ((BoundaryConditionParameters&)bcParameters).mask()(J1,J2,J3);
          // *wdh* 990915 : use bit ops
          mask(J1,J2,J3)=cmask(J1,J2,J3)>0 &&
            !( cmask(J1,J2,J3) & (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint)) && 
	     bmask(J1,J2,J3);
	}
        // display(mask,"applyBoundaryCondition: useMask on boundary","%2i");
      }

      switch ( bcType )
      {
      case dirichlet: 
        applyBCdirichlet(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,
                         uC,fC,mask,bcParameters,bcOption,grid);
	break;
      case normalDotScalarGrad:
        applyBCnormalDotScalarGrad(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;
      case neumann:
      case mixed:
	// 
	// Apply a Neumann BC or mixed boundary condition, (b0 + b1 n.grad) u = g
        applyBCneumann(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      case normalDerivativeOfNormalComponent:
      case normalDerivativeOfTangentialComponent0:
      case normalDerivativeOfTangentialComponent1:
        applyBCnormalDerivative(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      case extrapolate:
      case extrapolateNormalComponent:
      case extrapolateTangentialComponent0:
      case extrapolateTangentialComponent1:
        applyBCextrapolate(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      case normalComponent:
	//
	// to set the normal component to g:
	//       u <- u + (g-(n.u)) n
	//
        applyBCnormalComponent(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      case tangentialComponent0:
      case tangentialComponent1:
      case tangentialComponent:
        applyBCtangentialComponent(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      case evenSymmetry:
	//
	// Apply an even symmetry condition to a scalar, u(-) = u(+)
	//
      case oddSymmetry:
	//
	// Apply an odd symmetry condition to a scalar, u(-) = -u(+)
	//
      case vectorSymmetry:
	//
	// Apply a symmetry condition to a vector u=(u1,u2,u3)
	//    n.u is odd
	//    t.u is even
	applyBCsymmetry(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;
	
      case aDotU:  
	//
	// to set the component along a to g:
	//       u <- u + (g-(a.u)) a/<a,a>
	//
        applyBCaDotU(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      case generalMixedDerivative:  // give b(0)*u + b(1)*u.x + b(2)*u.y = g
        applyBCgeneralMixedDerivative(u, side,axis,C0,bcType,bc,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      case generalizedDivergence:           
	//
	// --- div( a::u ) ---
	//
	applyBCGenDiv(u,side,axis,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
        break;

      case aDotGradU:  
        applyBCaDotGradU(u, side,axis,scalarData,arrayData,arrayDataD,gfData,t,uC,fC,mask,bcParameters,bcOption,grid);
	break;

      default:
	printF("applyBoundaryCondition: unknown or un-implemented boundary condition = %i\n",(int)bcType);
	Overture::abort("MappedGridOperators::applyBoundaryCondition: fatal error! \n");
      }
    }
  }

  timeForAllBoundaryConditions+=getCPU()-time0;

}

// void MappedGridOperators:: 
// applyBCaDotGradU(realMappedGridFunction & u, 
// 		 const int side,
// 		 const int axis,
// 		 const real & scalarData,
// 		 const RealArray & arrayData,
// 		 const RealArray & arrayDataD,
// 		 const realMappedGridFunction & gfData,
// 		 const real & t,
//                  const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
// 		 const BoundaryConditionParameters & bcParameters,
// 		 const BoundaryConditionOption bcOption,
// 		 const int & grid  )
// {
//   Overture::abort("MappedGridOperators::applyBCaDotGradU:ERROR: This BC not implemented yet");
// }

