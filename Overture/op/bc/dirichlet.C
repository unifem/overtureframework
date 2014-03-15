#include "MappedGridOperators.h"

#include "MappedGridOperatorsInclude.h"
#include "ParallelUtility.h"


void MappedGridOperators::
applyBCdirichlet(realMappedGridFunction & u, 
		 const int side,
		 const int axis,
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
		 const BoundaryConditionOption bcOption,
		 const int & grid  )
{
  real time=getCPU();
  MappedGrid & c = mappedGrid;
  int n;

  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);


  #ifdef USE_PPP
    realSerialArray uA; getLocalArrayWithGhostBoundaries(u,uA);
    const realSerialArray & arrayDataDLocal=arrayDataD;
    realSerialArray gfDataLocal; getLocalArrayWithGhostBoundaries(gfData,gfDataLocal);

//     const realSerialArray & uA = u.getLocalArray();
//     const realSerialArray & arrayDataDLocal = arrayDataD.getLocalArray();
//     const realSerialArray & gfDataLocal = gfData.getLocalArray();


    int n1a = max(I1.getBase() , uA.getBase(0));
    int n1b = min(I1.getBound(),uA.getBound(0));
    					    
    int n2a = max(I2.getBase() , uA.getBase(1));
    int n2b = min(I2.getBound(),uA.getBound(1));
    					    
    int n3a = max(I3.getBase() , uA.getBase(2));
    int n3b = min(I3.getBound(),uA.getBound(2));

    if( n1a>n1b || n2a>n2b || n3a>n3b ) return;

    I1=Range(n1a,n1b);
    I2=Range(n2a,n2b);
    I3=Range(n3a,n3b);

  #else
    realSerialArray & uA = u;
    const realSerialArray & arrayDataDLocal = arrayDataD;
    const realSerialArray & gfDataLocal = gfData;
  #endif

  // *wdh* 070828 -- check dimensions of arrayDataD after restricting to local processor bounds
  //   -- this should be fixed so we know for certain whether to use arrayDataD !!
  bool useArrayDataD=false;
  if( bcOption==arrayForcing &&
      arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
      arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
      arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
      arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
  {
    useArrayDataD=true;
  }


  if( twilightZoneFlow )
  {
   #ifdef USE_PPP
    // Overture::abort("finish this");
    // evaluate the TZ solution and save in v

    // const realSerialArray & xLocal = c.center().getLocalArrayWithGhostBoundaries();
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(c.center(),xLocal);

    if( (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) || bcParameters.getUseMask() )
    {
      realSerialArray v(I1,I2,I3,1); 
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      {
	v.setBase(fC(n),3);  // base should match fC(n)
	bool isRectangular=false; // do this for now
	(*e).gd( v,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,fC(n),t);
	where( mask )
	{
          uA(I1,I2,I3,uC(n))=v(I1,I2,I3,fC(n));
	}
      }
    }
    else
    {
      // *** need an extra arg -- fill in which args of u
      // (*e).gd( uLocal,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,fC(n),t);

      // ** do this for now ***
      realSerialArray v(I1,I2,I3,1); 
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      {
	v.setBase(fC(n),3);  // base should match fC(n)
	bool isRectangular=false; // do this for now
	(*e).gd( v,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,fC(n),t);
        uA(I1,I2,I3,uC(n))=v(I1,I2,I3,fC(n));
      }
    }
    
   #else
    for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      WHERE_MASK( uA(I1,I2,I3,uC(n))=(*e)(c,I1,I2,I3,fC(n),t); )
   #endif
  }	    
  else if( bcOption==scalarForcing )
  {
    for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      WHERE_MASK( uA(I1,I2,I3,uC(n))=scalarData; )
  }
  else if( bcOption==arrayForcing )
  {
    if( useArrayDataD )
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1,I2,I3,uC(n))=arrayDataDLocal(I1,I2,I3,fC(n)); )
    else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1,I2,I3,uC(n))=arrayData(fC(n),side,axis,grid); )
    else
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1,I2,I3,uC(n))=arrayData(fC(n)); )
  }
  else if( bcOption==gridFunctionForcing )
  {  
    for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      WHERE_MASK( uA(I1,I2,I3,uC(n))=gfDataLocal(I1,I2,I3,fC(n)); )
  }
  else
  {
    cout << "applyBoundaryCondition: (dirichlet): ERROR: Invalid value for bcOption = " << bcOption << endl;
    {throw "Invalid value for bcOption!";}
  }
  timeForDirichlet+=getCPU()-time;
}
