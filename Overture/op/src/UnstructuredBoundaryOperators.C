#include "UnstructuredOperators.h"

int
UnstructuredOperators::
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
		       const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
		       const BoundaryConditionParameters & bcParameters,
		       const int bcOption,
		       const int & grid /* =0 */  )
{
  if ( !u.getOperators() )
    {
      cout<<"UnstructuredOperators::applyBoundaryCondition : ERROR : you must specify Operators for u"<<endl;
      return 1;
    }

  const MappedGridOperators &mgo = *u.getOperators();
  
  switch ( bcType )
    {
    case MappedGridOperators::dirichlet: 
      applyBCdirichlet(u,
		       C0,
		       bcType,
		       bc,
		       scalarData,
		       arrayData,
		       arrayDataD_,
		       gfData,
		       t, 
		       uC, fC, mask,
		       bcParameters,
		       bcOption,
		       grid);
      break;
    case MappedGridOperators::normalDotScalarGrad:
    case MappedGridOperators::neumann:
    case MappedGridOperators::mixed:
      // 
      // Apply a Neumann BC or mixed boundary condition, (b0 + b1 n.grad) u = g
    case MappedGridOperators::normalDerivativeOfNormalComponent:
    case MappedGridOperators::normalDerivativeOfTangentialComponent0:
    case MappedGridOperators::normalDerivativeOfTangentialComponent1:
    case MappedGridOperators::extrapolate:
    case MappedGridOperators::extrapolateNormalComponent:
    case MappedGridOperators::extrapolateTangentialComponent0:
    case MappedGridOperators::extrapolateTangentialComponent1:
    case MappedGridOperators::normalComponent:
      //
      // to set the normal component to g:
      //       u <- u + (g-(n.u)) n
      //
    case MappedGridOperators::tangentialComponent0:
    case MappedGridOperators::tangentialComponent1:
    case MappedGridOperators::tangentialComponent:
    case MappedGridOperators::evenSymmetry:
      //
      // Apply an even symmetry condition to a scalar, u(-) = u(+)
      //
    case MappedGridOperators::vectorSymmetry:
      //
      // Apply a symmetry condition to a vector u=(u1,u2,u3)
      //    n.u is odd
      //    t.u is even
    case MappedGridOperators::aDotU:  
      //
      // to set the component along a to g:
      //       u <- u + (g-(a.u)) a/<a,a>
      //
    case MappedGridOperators::generalMixedDerivative:  // give b(0)*u + b(1)*u.x + b(2)*u.y = g
    case MappedGridOperators::generalizedDivergence:           
      //
      // --- div( a::u ) ---
      //
    default:
      cout << "UnstructuredOperators::applyBoundaryCondition: unknown or un-implemented boundary conditon = " 
	   << bcType << endl;
      Overture::abort("MappedGridOperators::applyBoundaryCondition: fatal error! \n");
    }

  return 0;
}
