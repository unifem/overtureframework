#include "UnstructuredOperators.h"
#include "UnstructuredMapping.h"

int
UnstructuredOperators::
applyBCdirichlet(realMappedGridFunction & u, 
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
		 const int & grid  )
{

  // 040429 kkc TODO : 1. rewrite to use arrays from MG::getUnstructuredBCInfo() DONE 050124
  //                   2. add tests for bc (only allBoundaries works right now)
  //                   3. get the centering from the gridfunction, we might be
  //                      applying bcs on face or edge centered functions
  //                   4. make optimized fortran versions

  real startTime = getCPU();

  MappedGrid &mg=*u.getMappedGrid();
  UnstructuredMapping &umap = *((UnstructuredMapping*)(mg->mapping.mapPointer));

  UnstructuredMapping::EntityTypeEnum cellType;
  if ( mg.isAllCellCentered() )
    cellType = UnstructuredMapping::EntityTypeEnum(mg.domainDimension());
  else
    cellType = UnstructuredMapping::Vertex;

  realArray &cVert = mg.center();

  RealDistributedArray &U = u;

  //  UnstructuredMapping::tag_entity_iterator cell, cell_end;

  //  std::string bdyTag = std::string("boundary ")+UnstructuredMapping::EntityTypeStrings[cellType].c_str();
  //  cell_end = umap.tag_entity_end(bdyTag);

  const IntegerArray *ubcp = mg.getUnstructuredBCInfo( int(cellType) );
  if ( !ubcp || !ubcp->getLength(0))
    return 0;

  const IntegerArray &ubc = *ubcp;

  OGFunction *twilightzoneFlow = ((GenericMappedGridOperators *)u.getOperators())->twilightZoneFlowFunction;

#define FORCELL for ( int cell=0; cell<ubc.getLength(0); cell++ )

  if ( twilightzoneFlow )
    {
      OGFunction &tw = *twilightzoneFlow;
//       for ( UnstructuredMapping::tag_entity_iterator cell=umap.tag_entity_begin(bdyTag);
// 	    cell!=cell_end;
// 	    cell++ )
      FORCELL
	for ( int c=Components.getBase();
	      c<=Components.getBound();
	      c++ )
	  U(ubc(cell,0),0,0,c) = tw(cVert(ubc(cell,0),0,0,0),cVert(ubc(cell,0),0,0,1),cVert(ubc(cell,0),0,0,2),c,t);
    }
  else if ( bcOption==0 ) // scalarForcing
    {
//       for ( UnstructuredMapping::tag_entity_iterator cell=umap.tag_entity_begin(bdyTag);
// 	    cell!=cell_end;
// 	    cell++ )
      FORCELL
	for ( int c=Components.getBase();
	      c<=Components.getBound();
	      c++ )
	  U(ubc(cell,0),0,0,c) = scalarData;
    }
  else if ( bcOption==1 ) // arrayForcing
    {
//       for ( UnstructuredMapping::tag_entity_iterator cell=umap.tag_entity_begin(bdyTag);
// 	    cell!=cell_end;
// 	    cell++ )
      FORCELL
	for ( int c=uC.getBase(0); c<=uC.getBound(0); c++ )
	  U(ubc(cell,0),0,0,c) = arrayData(fC(c));
    }
  else if ( bcOption==2 ) // gridFunctionForcing
    {
//       for ( UnstructuredMapping::tag_entity_iterator cell=umap.tag_entity_begin(bdyTag);
// 	    cell!=cell_end;
// 	    cell++ )
      FORCELL
	for ( int c=uC.getBase(0); c<=uC.getBound(0); c++ )
	  U(ubc(cell,0),0,0,c) = gfData(ubc(cell,0),0,0,fC(c));
    }
  else
    {
      cout << "applyBoundaryCondition: (dirichlet): ERROR: Invalid value for bcOption = " << bcOption << endl;
      {throw "Invalid value for bcOption!";} // ugh!
    }
  return 0;
}
