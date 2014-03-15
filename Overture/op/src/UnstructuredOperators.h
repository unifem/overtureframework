#ifndef __OV_UNSTRUCTUREDOPERATORS_H__
#define __OV_UNSTRUCTUREDOPERATORS_H__

#include "MappedGrid.h"
#include "MappedGridOperators.h"
#include "UnstructuredMapping.h"

class UnstructuredOperators 
{
public:
  UnstructuredOperators() : mg(0) {  }
  UnstructuredOperators(MappedGrid &mg_) : mg(&mg_) {  }

  virtual ~UnstructuredOperators() { } 

  virtual int derivative(const MappedGridOperators::derivativeTypes &derivativeType,
			 const realArray &u,
			 const realArray &scalar,
			 realArray &ux,
			 const Index & I1  = nullIndex, 
			 const Index & C  = nullIndex);

  virtual int undividedDerivative(const MappedGridOperators::derivativeTypes &derivativeType,
				  const realArray &u,
				  const realArray &scalar,
				  realArray &ux,
				  const Index & I1  = nullIndex, 
				  const Index & C  = nullIndex);

  virtual int assignCoefficients(const MappedGridOperators::derivativeTypes &derivativeType,
				 realArray &coeff,
				 const realArray &scalar,
				 const Index & I1  = nullIndex, 
				 const Index & E   = nullIndex,   
				 const Index & C  = nullIndex);
  
  virtual int applyBoundaryCondition(realMappedGridFunction & u, 
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
				     const int & grid  =0   );

protected:

  virtual int applyBCdirichlet(realMappedGridFunction & u, 
			       const Index & Components,
			       const BCTypes::BCNames & boundaryConditionType,
			       const int & bc,
			       const real & scalarData,
			       const RealArray & arrayData,
			       const RealArray & arrayDataD,
			       const realMappedGridFunction & gfData,
			       const real & t,
			       const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
			       const BoundaryConditionParameters & bcParameters,
			       const int bcOption,
			       const int & grid  );

private:
  MappedGrid *mg;

};

namespace UNSTRUCTURED_OPS_FV2 {

  void divergence_uFV2(const UnstructuredMapping &umap,
		       const realArray &u, 
		       const UnstructuredMapping::EntityTypeEnum u_centering, 
		       const UnstructuredMapping::EntityTypeEnum surface_centering,
		       const UnstructuredMapping::EntityTypeEnum ux_centering,
		       const realArray &surfaceNormals, const realArray &surfaceAreas, const realArray &volumes, 
		       const realArray &scalar, const Index &C,
		       realArray &ux);

  void xi_uFV2(int d, const UnstructuredMapping &umap,
	       const realArray &u, 
	       const UnstructuredMapping::EntityTypeEnum u_centering, 
	       const UnstructuredMapping::EntityTypeEnum surface_centering,
	       const UnstructuredMapping::EntityTypeEnum ux_centering,
	       const realArray &surfaceNormals, const realArray &surfaceAreas, const realArray &volumes, 
	       const realArray &scalar, const Index &C,
	       realArray &ux);

  void xixj_uFV2(int d1, int d2, const UnstructuredMapping &umap,
		 const realArray &u, 
		 const UnstructuredMapping::EntityTypeEnum u_centering, 
		 const UnstructuredMapping::EntityTypeEnum surface_centering,
		 const UnstructuredMapping::EntityTypeEnum ux_centering,
		 const realArray &surfaceNormals, const realArray &surfaceAreas, const realArray &volumes, 
		 const realArray &scalar,const Index &C, 
		 realArray &uxx);
    
  void u_xi_uFV2(int d, const UnstructuredMapping &umap,
		 const realArray &u, 
		 const UnstructuredMapping::EntityTypeEnum u_centering, 
		 const UnstructuredMapping::EntityTypeEnum surface_centering,
		 const UnstructuredMapping::EntityTypeEnum ux_centering,
		 const realArray &surfaceNormals, const realArray &surfaceAreas, 
		 const realArray &scalar, const Index &C,
		 realArray &ux);

  void u_xixj_uFV2(int d1, int d2, const UnstructuredMapping &umap,
		   const realArray &u, 
		   const UnstructuredMapping::EntityTypeEnum u_centering, 
		   const UnstructuredMapping::EntityTypeEnum surface_centering,
		   const UnstructuredMapping::EntityTypeEnum ux_centering,
		   const realArray &scalar,const Index &C, 
		   realArray &uxx);

  void u_divergence_uFV2(const UnstructuredMapping &umap,
			 const realArray &u, 
			 const UnstructuredMapping::EntityTypeEnum u_centering, 
			 const UnstructuredMapping::EntityTypeEnum surface_centering,
			 const UnstructuredMapping::EntityTypeEnum ux_centering,
			 const realArray &surfaceNormals, 
			 const realArray &scalar, const Index &C,
			 realArray &ux);
}

#endif
