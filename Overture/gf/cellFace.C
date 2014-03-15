#include "Overture.h"
#include "display.h"

//================================================================================
//
//  Test the cell-centre and face-centre features of the gridFunction classes
//
//================================================================================

//--------------------------------------------------------------------------------
//  This function determines whether a MappedGridFunction is one of the standard
//  face centering types.
//--------------------------------------------------------------------------------
void determineFaceCentering( realMappedGridFunction & u )
{
  cout << "==========determineFaceCentering=========\n";
  display(u.isCellCentered,"------Here is u.isCellCentered--------","%4.1f ");
  switch (u.getFaceCentering())
  {
  case GridFunctionParameters::none :
    cout << " this is not a standard face centered variable \n";
    break;
  case GridFunctionParameters::all :
    cout << "this function has components that are face centred in all space dimensions\n";
    cout << "The value u.positionOfFaceCentering() = " << u.positionOfFaceCentering() << ", "
            "gives the position of the faceRange \n";
    break;
  case GridFunctionParameters::direction0 :
    cout << "all components are face centered along direction=0 \n";
    break;
  case GridFunctionParameters::direction1 :
    cout << "all components are face centered along direction=1 \n";
    break;
  case GridFunctionParameters::direction2 :
    cout << "all components are face centered along direction=2 \n";
    break;
  default:
    cout << "Unknown face-centering type! This case should not occur! \n";
  };
  cout << "numberOfComponents = " << u.getNumberOfComponents() << endl;
}

//--------------------------------------------------------------------------------
//  This function determines whether a MappedGridFunction is one of the standard
//  face centering types.
//--------------------------------------------------------------------------------
void determineFaceCentering( realCompositeGridFunction & u )
{
  cout << "==========determineFaceCentering=========\n";
  for( int grid=0; grid<u.numberOfComponentGrids(); grid++ )
    display(u[grid].isCellCentered,"------Here is u.isCellCentered--------","%4.1f ");
  switch (u.getFaceCentering())
  {
  case GridFunctionParameters::none :
    cout << " this is not a standard face centered variable \n";
    break;
  case GridFunctionParameters::all :
    cout << "this function has components that are face centred in all space dimensions\n";
    cout << "The value u.positionOfFaceCentering() = " << u.positionOfFaceCentering() << ", "
            "gives the position of the faceRange \n";
    break;
  case GridFunctionParameters::direction0 :
    cout << "all components are face centered along direction=0 \n";
    break;
  case GridFunctionParameters::direction1 :
    cout << "all components are face centered along direction=1 \n";
    break;
  case GridFunctionParameters::direction2 :
    cout << "all components are face centered along direction=2 \n";
    break;
  default:
    cout << "Unknown face-centering type! This case should not occur! \n";
  };
  cout << "numberOfComponents = " << u.getNumberOfComponents() << endl;
}


int 
main(int argc, char **argv)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Range all;                  // a null Range is used when constructing grid functions, it indicates
                              // the positions of the coordinate axes


  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5CC", "cicCC", "sibCC" };
    
  if( argc > 1 )
  { 
    numberOfGridsToTest=1;
    gridName[0]=argv[1];
  }
  else
    cout << "Usage: `cellFace [<gridName>]' \n";


  int errorCount=0;
  Index I1,I2,I3;

  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];
    cout << "\n *****************************************************************\n";
    cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
    cout << " *****************************************************************\n\n";


    CompositeGrid og;
    getFromADataBase(og,nameOfOGFile);
    og.update();

    MappedGrid & mg = og[0];
  
    cout << "\n ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n";
    realMappedGridFunction ua(mg,GridFunctionParameters::faceCenteredAxis2,Range(0,1),Range(1,1)),ub;
    cout << "realMappedGridFunction ua(mg,realCompositeGridFunction::faceCenteredAxis2,0:1,1:1);\n";
    if( ua.getGridFunctionType()!=GridFunctionParameters::faceCenteredAxis2 )
    {
      errorCount++;
      cout << "ERROR: ua.getGridFunctionType=" << ua.getGridFunctionType() 
	   << ", faceCenteredAxis2 = " << GridFunctionParameters::faceCenteredAxis2 << endl;
    }
    if( ua.getGridFunctionTypeWithComponents() != GridFunctionParameters::faceCenteredAxis2With2Components )
    {
      errorCount++;
      cout << "ERROR: ua.getGridFunctionTypeWithComponents=" << ua.getGridFunctionTypeWithComponents()
	   << ", faceCenteredAxis2With2Component = " << GridFunctionParameters::faceCenteredAxis2With2Components  << endl;
      cout << "ua.getNumberOfComponents=" << ua.getNumberOfComponents() << endl;
    }
    if( ua.getFaceCentering() != GridFunctionParameters::direction1 )
    {
      errorCount++;
      cout << "ERROR: ua should be face centered in direction1! \n";
      determineFaceCentering( ua );
    }
    if( ua.getGridFunctionType(0) != GridFunctionParameters::faceCenteredAxis2 )
    {
      errorCount++;
      cout << "ERROR: ua.getGridFunctionType(0)=" << ua.getGridFunctionType(0) << ", should be " 
	   << GridFunctionParameters::faceCenteredAxis2 << endl;
    }
    if( ua.getGridFunctionType(1,1)     != GridFunctionParameters::faceCenteredAxis2 )
    {
      errorCount++;
      cout << "ERROR: ua.getGridFunctionType(1,1)=" << ua.getGridFunctionType(1,1) << ", should be " 
	   << GridFunctionParameters::faceCenteredAxis2 << endl;
    }
    if( ua.getGridFunctionType(all,all) != GridFunctionParameters::faceCenteredAxis2 )
    {
      errorCount++;
      cout << "ERROR: ua.getGridFunctionType(all,all)=" << ua.getGridFunctionType(all,all) << ", should be " 
	   << GridFunctionParameters::faceCenteredAxis2 << endl;
    }    
    if( ua.getGridFunctionTypeWithComponents(0) != GridFunctionParameters::faceCenteredAxis2With1Component )
    {
      errorCount++;
      cout << "ERROR: ua.getGridFunctionTypeWithComponents(0)=" << 
	ua.getGridFunctionTypeWithComponents(0) << ", should be " 
	   << GridFunctionParameters::faceCenteredAxis2With1Component << endl;
    }
    display(mg.indexRange(),"indexRange");
    display(mg.gridIndexRange(),"gridIndexRange");
    display(ua.isCellCentered,"ua.isCellCentered");
  
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	getBoundaryIndex(ua,side,axis,I1,I2,I3);
	printf("getBoundaryIndex: (side,axis)=(%i,%i) I1=(%i,%i), I2=(%i,%i) \n",side,axis
	       ,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
	getGhostIndex(ua,0,side,axis,I1,I2,I3,0);
	printf("getGhostIndex: (side,axis)=(%i,%i) I1=(%i,%i), I2=(%i,%i) \n",side,axis
	       ,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
      }
    }
  

    


    ub=ua;
    cout << "ub=ua;\n";
    if( ub.getGridFunctionType() != GridFunctionParameters::faceCenteredAxis2  )
    {
      errorCount++;
      cout << "ERROR: ub.getGridFunctionType=" << ub.getGridFunctionType() 
	   << ", faceCenteredAxis2 = " << GridFunctionParameters::faceCenteredAxis2 << endl;
    }
    if( ub.getGridFunctionTypeWithComponents() != GridFunctionParameters::faceCenteredAxis2With2Components )
    {
      errorCount++;
      cout << "ERROR: ub.getGridFunctionTypeWithComponents=" << ub.getGridFunctionTypeWithComponents()
	   << ", faceCenteredAxis2With2Component = " << GridFunctionParameters::faceCenteredAxis2With2Components << endl;
    }
    if( ub.getNumberOfComponents() != 2 )
    {
      errorCount++;
      cout << "ERROR: ub.getNumberOfComponents=" << ub.getNumberOfComponents() << ", should be 2" << endl;
    }
    if(  ub.getFaceCentering() != GridFunctionParameters::direction1 )
    {
      errorCount++;
      cout << "ERROR: ub should be face centered in direction 1! \n";
      determineFaceCentering( ub );
    }

    realMappedGridFunction uc;
    cout << "realMappedGridFunction uc; uc.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAxis1) \n";
    uc.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAxis1);
    if( uc.getGridFunctionType() != GridFunctionParameters::faceCenteredAxis1  )
    {
      errorCount++;
      cout << "ERROR: uc.getGridFunctionType=" << uc.getGridFunctionType() 
	   << ", faceCenteredAxis2 = " << GridFunctionParameters::faceCenteredAxis1 << endl;
    }


    realMappedGridFunction u(mg,all,all,all,Range(0,1));
    cout << "realMappedGridFunction u(mg,all,all,all,Range(0,1)); \n";
    if( u.getGridFunctionType() != mg.center().getGridFunctionType() )
    {
      errorCount++;
      cout << "ERROR: u.getGridFunctionType() != mg.center().getGridFunctionType() \n";
    }
    if(  u.getFaceCentering() != GridFunctionParameters::none )
    {
      errorCount++;
      cout << "ERROR: u.getFaceCentering() should be `none' ! \n";
      determineFaceCentering( u );
    }
    if( u.getGridFunctionType(1) != mg.center().getGridFunctionType() )
    {
      errorCount++;
      cout << "ERROR: u.getGridFunctionType(1) != mg.center().getGridFunctionType() \n";
    }

    u.setFaceCentering(axis1);               // u(I1,I2,I3,0:1) : all components face centred along direction 0
    cout << "u.setFaceCentering(axis1); \n";
    if( u.getGridFunctionType() != GridFunctionParameters::faceCenteredAxis1  )
    {
      errorCount++;
      cout << "ERROR: u.getGridFunctionType=" << u.getGridFunctionType() 
	   << ", faceCenteredAxis1 = " << GridFunctionParameters::faceCenteredAxis1 << endl;
    } 
    if(  u.getFaceCentering() != GridFunctionParameters::direction0 )
    {
      errorCount++;
      cout << "ERROR: u.getFaceCentering() should be direction0 ! \n";
      determineFaceCentering( u );
    }

    display(u.isCellCentered,"u.isCellCentered");
  
    for( axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	getBoundaryIndex(u,side,axis,I1,I2,I3);
	printf("getBoundaryIndex: (side,axis)=(%i,%i) I1=(%i,%i), I2=(%i,%i) \n",side,axis
	       ,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
	getGhostIndex(u,0,side,axis,I1,I2,I3,0);
	printf("getGhostIndex: (side,axis)=(%i,%i) I1=(%i,%i), I2=(%i,%i) \n",side,axis
	       ,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
      }
    }
  

    u.updateToMatchGrid(Range(0,2),all,all,all);
    u.setFaceCentering(axis2);               // u(0:2,I1,I2,I3) : all components face centred along direction 1
    cout << "u.updateToMatchGrid(Range(0,2),all,all,all); \n";
    cout << "u.setFaceCentering(axis2); \n";
    display(u.isCellCentered,"Here is u.isCellCentered");
    if( u.getGridFunctionType() != GridFunctionParameters::faceCenteredAxis2  )
    {
      errorCount++;
      cout << "ERROR: u.getGridFunctionType=" << u.getGridFunctionType() 
	   << ", faceCenteredAxis2 = " << GridFunctionParameters::faceCenteredAxis2 << endl;
    } 
    if(  u.getFaceCentering() != GridFunctionParameters::direction1 )
    {
      errorCount++;
      cout << "ERROR: u.getFaceCentering() should be direction1 ! \n";
      determineFaceCentering( u );
    }

    realMappedGridFunction u2(mg,all,all,all,Range(0,1),faceRange),u3;  
    u2=1.;
    // u2(I1,I2,I3,0:1,0)  : these components are face-centred along direction 0
    // u2(I1,I2,I3,0:1,1)  : these components are face-centred along direction 1
    // u2(I1,I2,I3,0:1,2)  : these components are face-centred along direction 2 (if the grid is 3D)
    cout << "realMappedGridFunction u2(mg,all,all,all,Range(0,1),faceRange); \n";
    // u2.isCellCentered.display("Here is u2.isCellCentered");
    if( u2.getGridFunctionType() != GridFunctionParameters::faceCenteredAll  )
    {
      errorCount++;
      cout << "ERROR: u2.getGridFunctionType=" << u.getGridFunctionType() 
	   << ", faceCenteredAll = " << GridFunctionParameters::faceCenteredAll << endl;
      display(u2,"here is u2(mg,all,all,all,Range(0,1),faceRange)","%4.1f ");
    } 
    for( axis=axis1; axis<mg.numberOfDimensions(); axis++ )
    {
      getIndex(u2,axis,I1,I2,I3);
      printf("faceCenteredAll: getIndex(u,%i,...) I1=(%i,%i), I2=(%i,%i), I3=(%i,%i) \n",axis
	     ,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
    }

    if(  u2.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: u2.getFaceCentering() should be all ! \n";
      determineFaceCentering( u2 );
    }
    if( u2.getGridFunctionType(0)     != GridFunctionParameters::faceCenteredAll ||
	u2.getGridFunctionType(1)     != GridFunctionParameters::faceCenteredAll ||
	u2.getGridFunctionType(all)   != GridFunctionParameters::faceCenteredAll ||
	u2.getGridFunctionType(all,0) != GridFunctionParameters::faceCenteredAxis1 ||
	u2.getGridFunctionType(1,1)   != GridFunctionParameters::faceCenteredAxis2 )
    {
      errorCount++;
      cout << "ERROR: u2.getGridFunctionType(0)=" << u2.getGridFunctionType(0) << ", should be " 
	   << GridFunctionParameters::faceCenteredAll << endl;
      cout << "ERROR: u2.getGridFunctionType(1)=" << u2.getGridFunctionType(1) << ", should be " 
	   << GridFunctionParameters::faceCenteredAll << endl;
      cout << "ERROR: u2.getGridFunctionType(all)=" << u2.getGridFunctionType(all) << ", should be " 
	   << GridFunctionParameters::faceCenteredAll << endl;
      cout << "ERROR: u2.getGridFunctionType(all,0)=" << u2.getGridFunctionType(all,0) << ", should be " 
	   << GridFunctionParameters::faceCenteredAxis1 << endl;
      cout << "ERROR: u2.getGridFunctionType(1,1)=" << 
	u2.getGridFunctionType(1,1) << ", should be " 
	   << GridFunctionParameters::faceCenteredAxis2 << endl;
    }    

    u3=u2;
    cout << "Face centering for u3=u2: \n";
    if(  u3.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: u3.getFaceCentering() should be all ! \n";
      determineFaceCentering( u3 );
    }

    u2.updateToMatchGrid(mg,all,all,all,faceRange,Range(0,0),Range(0,2));
    // u2(I1,I2,I3,0,0:0,0:2)  : these components are face-centred along direction 0
    // u2(I1,I2,I3,1,0:0,0:2)  : these components are face-centred along direction 1
    // u2(I1,I2,I3,2,0:0,0:2)  : these components are face-centred along direction 2 (if the grid is 3D)
    cout << "u2.updateToMatchGrid(mg,all,all,all,faceRange,Range(0,0),Range(0,2)); \n";
    if(  u2.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: u2.getFaceCentering() should be all ! \n";
      display(u2,"here is u2.updateToMatchGrid(mg,all,all,all,faceRange,Range(0,0),Range(0,2))","%4.1f ");
      determineFaceCentering( u2 );
    }

    u2.updateToMatchGrid(mg,all,all,all,Range(0,1),faceRange);  
    if(  u2.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: u2.getFaceCentering() should be all ! \n";
      display(u2,"here is u2.updateToMatchGrid(mg,all,all,all,Range(0,1),faceRange)","%4.1f ");
      determineFaceCentering( u2 );
    }

    cout << endl << "========= test CompositeGridFunctions ========== " << endl;

    realCompositeGridFunction v(og,all,all,all,Range(0,1));
    v.setFaceCentering(axis1);
    cout << "realCompositeGridFunction v(og,all,all,all,Range(0,1)); \n";
    cout << "v.setFaceCentering(axis1); \n";
    if(  v.getFaceCentering() != GridFunctionParameters::direction0 )
    {
      errorCount++;
      cout << "ERROR: v.getFaceCentering() should be direction0 ! \n";
      determineFaceCentering( v );
    }
    
    v.updateToMatchGrid(Range(0,2),all,all,all);
    v.setFaceCentering(axis2);               // u(0:2,I1,I2,I3) : all components face centred along direction 1
    cout << "v.updateToMatchGrid(Range(0,2),all,all,all); \n";
    cout << "v.setFaceCentering(axis2); \n";
    if(  v.getFaceCentering() != GridFunctionParameters::direction1 )
    {
      errorCount++;
      cout << "ERROR: v.getFaceCentering() should be direction0 ! \n";
      determineFaceCentering( v );
    }

    realCompositeGridFunction v2(og,all,all,all,Range(0,1),faceRange), v3;  
    cout << "realCompositeGridFunction v2(og,all,all,all,Range(0,1),faceRange); \n";
    if(  v2.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: v2.getFaceCentering() should be all ! \n";
      determineFaceCentering( v2 );
    }

    v2.updateToMatchGrid(og,all,all,all,faceRange,Range(0,0),Range(0,2));
    cout << "v2.updateToMatchGrid(og,all,all,all,faceRange,Range(0,0),Range(0,2)); \n";
    if(  v2.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: v2.getFaceCentering() should be all ! \n";
      determineFaceCentering( v2 );
    }

    v2=3.;
    v3=v2;
    cout << "*** Here is face centering for v3=v2\n";
    if(  v3.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: v3.getFaceCentering() should be all ! \n";
      determineFaceCentering( v3 );
    }
    
    cout << "\n ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n";
    realCompositeGridFunction u5(og,GridFunctionParameters::faceCenteredAll,Range(0,1));
    cout << "realCompositeGridFunction u5(og,GridFunctionParameters::faceCenteredAll,2);\n";
    display(u5[0].isCellCentered,"here is isCellCentered");
    
    if( u5.getGridFunctionType() != GridFunctionParameters::faceCenteredAll )
    {
      errorCount++;
      cout << "ERROR: u5.getGridFunctionType=" << u5.getGridFunctionType() 
	   << ", faceCenteredAll = " << GridFunctionParameters::faceCenteredAll << endl;
    }
    if( u5.getGridFunctionTypeWithComponents() != GridFunctionParameters::faceCenteredAllWith1Component )
    {
      errorCount++;
      cout << "ERROR: u5.getGridFunctionTypeWithComponents=" << u5.getGridFunctionTypeWithComponents()
	   << ", faceCenteredAllWith1Component = " << GridFunctionParameters::faceCenteredAllWith1Component  << endl;
    }
    if( u5.getNumberOfComponents() != 1 )
    {
      errorCount++;
      cout << "ERROR: u5.getNumberOfComponents=" << u5.getNumberOfComponents() << endl;
    }    
    if(  u5.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: u5.getFaceCentering() should be all ! \n";
      determineFaceCentering( u5 );
    }

    realCompositeGridFunction u5Link;
    u5Link.link(u5,Range(0,0));
    cout << "u5Link.link(u5,Range(0,0));\n";
    if( u5Link.getGridFunctionType() != GridFunctionParameters::faceCenteredAxis1 )
    {
      errorCount++;
      cout << "ERROR: u5Link.getGridFunctionType=" << u5Link.getGridFunctionType() 
	   << ", faceCenteredAxis1 = " << GridFunctionParameters::faceCenteredAxis1 << endl;
    }
    if( u5Link.getGridFunctionTypeWithComponents() != GridFunctionParameters::faceCenteredAxis1With1Component )
    {
      errorCount++;
      cout << "ERROR: u5Link.getGridFunctionTypeWithComponents=" << u5Link.getGridFunctionTypeWithComponents()
	   << ", faceCenteredAxis1With1Component = " << GridFunctionParameters::faceCenteredAxis1With1Component<< endl;
    }
    if( u5Link.getNumberOfComponents() != 1 )
    {
      errorCount++;
      cout << "ERROR: u5Link.getNumberOfComponents=" << u5Link.getNumberOfComponents() << endl;
    }   
    if(  u5Link.getFaceCentering() != GridFunctionParameters::direction0 )
    {
      errorCount++;
      cout << "ERROR: u5Link.getFaceCentering() should be all ! \n";
      determineFaceCentering( u5Link );
    }

    realCompositeGridFunction u6(og,GridFunctionParameters::faceCenteredAxis2,Range(0,0),Range(0,1));
    cout << "realCompositeGridFunction u6(og,GridFunctionParameters::faceCenteredAxis2,1,2); \n";
    if(  u6.getGridFunctionType() !=  GridFunctionParameters::faceCenteredAxis2 )
    {
      errorCount++;
      cout << "ERROR: u6.getGridFunctionType=" << u6.getGridFunctionType() 
	   << ", faceCenteredAxis2 = " << GridFunctionParameters::faceCenteredAxis2 << endl;
    }
    if(  u6.getGridFunctionTypeWithComponents() != GridFunctionParameters::faceCenteredAxis2With2Components )
    {
      errorCount++;
      cout << "ERROR: u6.getGridFunctionTypeWithComponents=" << u6.getGridFunctionTypeWithComponents()
	   << ", faceCenteredAxis2With2Components = " << GridFunctionParameters::faceCenteredAxis2With2Components<< endl;
    }
    if(  u6.getNumberOfComponents() != 2 )
    {
      errorCount++;
      cout << "ERROR: u6.getNumberOfComponents=" << u6.getNumberOfComponents() << endl;
    }
    if(  u6.getFaceCentering() != GridFunctionParameters::direction1 )
    {
      errorCount++;
      cout << "ERROR: u6.getFaceCentering() should be direction1 ! \n";
      determineFaceCentering( u6 );
    }

    realCompositeGridFunction u7(og,GridFunctionParameters::cellCentered,Range(0,0));
    cout << "realCompositeGridFunction u7(og,cellCentered,Range(0,0)); \n";
    if(  u7.getGridFunctionType() !=  GridFunctionParameters::cellCentered )
    {
      errorCount++;
      cout << "ERROR: u7.getGridFunctionType=" << u7.getGridFunctionType() 
	   << ", cellCentered = " << GridFunctionParameters::cellCentered << endl;
    }
    if( u7.getGridFunctionTypeWithComponents() != GridFunctionParameters::cellCenteredWith1Component )
    {
      errorCount++;
      cout << "ERROR: u7.getGridFunctionTypeWithComponents=" << u7.getGridFunctionTypeWithComponents()
	   << ", cellCenteredWith1Component = " << GridFunctionParameters::cellCenteredWith1Component  << endl;
    }
    if( u7.getNumberOfComponents() != 1 )
    {
      errorCount++;
      cout << "ERROR: u7.getNumberOfComponents=" << u7.getNumberOfComponents() << endl;
    }
    if(  u7.getFaceCentering() != GridFunctionParameters::none )
    {
      errorCount++;
      cout << "ERROR: u7.getFaceCentering() should be none ! \n";
      determineFaceCentering( u7 );
    }

    // Change the grid function type but keep the number of components the same
    u7.updateToMatchGrid(og,GridFunctionParameters::faceCenteredAll);
    cout << "u7.updateToMatchGrid(og,GridFunctionParameters::faceCenteredAll); \n";
    if( u7.getGridFunctionType() != GridFunctionParameters::faceCenteredAll )
    {
      errorCount++;
      cout << "ERROR: u7.getGridFunctionType=" << u7.getGridFunctionType() 
	   << ", faceCenteredAll = " << GridFunctionParameters::faceCenteredAll << endl;
    }
    if( u7.getGridFunctionTypeWithComponents() !=  GridFunctionParameters::faceCenteredAllWith1Component )
    {
      errorCount++;
      cout << "ERROR: u7.getGridFunctionTypeWithComponents=" << u7.getGridFunctionTypeWithComponents()
	   << ", faceCenteredAllWith0Components = " << GridFunctionParameters::faceCenteredAllWith0Components  << endl;
    }
    if( u7.getNumberOfComponents() != 1 )
    {
      errorCount++;
      cout << "ERROR: u7.getNumberOfComponents=" << u7.getNumberOfComponents() << endl;
    }
    if(  u7.getFaceCentering() != GridFunctionParameters::all )
    {
      errorCount++;
      cout << "ERROR: u7.getFaceCentering() should be all ! \n";
      determineFaceCentering( u7 );
    }

    realCompositeGridFunction u8(og,3);
    cout << "realCompositeGridFunction u8(og,3) \n";
    if( (int&)og[0].isAllVertexCentered() )
    {    
      if(  u8.getGridFunctionType() != GridFunctionParameters::vertexCentered )
      {
	errorCount++;
	cout << "ERROR: u8.getGridFunctionType=" << u8.getGridFunctionType() 
	     << ", vertexCentered = " << GridFunctionParameters::vertexCentered << endl;
      }
      if( u8.getGridFunctionTypeWithComponents() != GridFunctionParameters::vertexCenteredWith1Component )
      {
	errorCount++;
	cout << "ERROR: u8.getGridFunctionTypeWithComponents=" << u8.getGridFunctionTypeWithComponents()
	     << ", vertexCenteredWith1Component = " << GridFunctionParameters::vertexCenteredWith1Component << endl;
      }
    }
    else
    {
      if(  u8.getGridFunctionType() != GridFunctionParameters::cellCentered )
      {
	errorCount++;
	cout << "ERROR: u8.getGridFunctionType=" << u8.getGridFunctionType() 
	     << ", cellCentered = " << GridFunctionParameters::cellCentered << endl;
      }
      if( u8.getGridFunctionTypeWithComponents() != GridFunctionParameters::cellCenteredWith1Component )
      {
	errorCount++;
	cout << "ERROR: u8.getGridFunctionTypeWithComponents=" << u8.getGridFunctionTypeWithComponents()
	     << ", cellCenteredWith1Component = " << GridFunctionParameters::cellCenteredWith1Component << endl;
      }
    }
    if( u8.getNumberOfComponents() != 1 )    
    {
      errorCount++;
      cout << "ERROR: u8.getNumberOfComponents=" << u8.getNumberOfComponents() << endl;
    }
    if(  u8.getFaceCentering() != GridFunctionParameters::none )
    {
      errorCount++;
      cout << "ERROR: u8.getFaceCentering() should be none ! \n";
      determineFaceCentering( u8 );
    }

    u8=1.;
    realCompositeGridFunction u9 = u8;
    u9.updateToMatchGrid(og);
    cout << "realCompositeGridFunction u9 = u8; \n";
    if( (int&)og[0].isAllVertexCentered() )
    {    
      if(  u9.getGridFunctionType() != GridFunctionParameters::vertexCentered )
      {
	errorCount++;
	cout << "ERROR: u9.getGridFunctionType=" << u9.getGridFunctionType() 
	     << ", vertexCentered = " << GridFunctionParameters::vertexCentered << endl;
      }
      if( u9.getGridFunctionTypeWithComponents() != GridFunctionParameters::vertexCenteredWith1Component )
      {
	errorCount++;
	cout << "ERROR: u9.getGridFunctionTypeWithComponents=" << u9.getGridFunctionTypeWithComponents()
	     << ", vertexCenteredWith1Component = " << GridFunctionParameters::vertexCenteredWith1Component << endl;
      }
    }
    else
    {
      if(  u9.getGridFunctionType() != GridFunctionParameters::cellCentered )
      {
	errorCount++;
	cout << "ERROR: u9.getGridFunctionType=" << u9.getGridFunctionType() 
	     << ", cellCentered = " << GridFunctionParameters::cellCentered << endl;
      }
      if( u9.getGridFunctionTypeWithComponents() != GridFunctionParameters::cellCenteredWith1Component )
      {
	errorCount++;
	cout << "ERROR: u9.getGridFunctionTypeWithComponents=" << u9.getGridFunctionTypeWithComponents()
	     << ", cellCenteredWith1Component = " << GridFunctionParameters::cellCenteredWith1Component << endl;
      }
    }
    if( u9.getNumberOfComponents() != 1 )    
    {
      errorCount++;
      cout << "ERROR: u9.getNumberOfComponents=" << u9.getNumberOfComponents() << endl;
    }
    if(  u9.getFaceCentering() != GridFunctionParameters::none )
    {
      errorCount++;
      cout << "ERROR: u9.getFaceCentering() should be none ! \n";
      determineFaceCentering( u9 );
    }
    
    realCompositeGridFunction u11(og,all,all,all,Range(0,1),Range(0,1));
    u11=1.;
    realCompositeGridFunction u12,u13;
    u12=u11;  
    if( u12.getNumberOfComponents() != 2 )
    {
      errorCount++;
      cout << "ERROR: u12.getNumberOfComponents = " << u12.getNumberOfComponents() << endl;
    }
    if( u12.getComponentDimension(0) != 2 )
    {
      errorCount++;
      cout << "ERROR: u12.getComponentDimension(0) = " << u12.getComponentDimension(0) << endl;
    }
    u13.updateToMatchGridFunction(u12);
    u13=u12-u11;
    if( u13.getNumberOfComponents() != 2 )
    {
      errorCount++;
      cout << "ERROR: u13.getNumberOfComponents = " << u13.getNumberOfComponents() << endl;
    }
    if( u13.getComponentDimension(0) != 2 )
    {
      errorCount++;
      cout << "ERROR: u13.getComponentDimension(0) = " << u13.getComponentDimension(0) << endl;
    }
  
    realCompositeGridFunction u14;
    u14.updateToMatchGrid(og,all,all,all);
    u14=0.;
    cout << "realCompositeGridFunction u14.updateToMatchGrid(og,all,all,all); \n";
    if( (int&)og[0].isAllVertexCentered() )
    {    
      if(  u14.getGridFunctionType() != GridFunctionParameters::vertexCentered )
      {
	errorCount++;
	cout << "ERROR: u14.getGridFunctionType=" << u14.getGridFunctionType() 
	     << ", vertexCentered = " << GridFunctionParameters::vertexCentered << endl;
      }
      if( u14.getGridFunctionTypeWithComponents() != GridFunctionParameters::vertexCenteredWith0Components )
      {
	errorCount++;
	cout << "ERROR: u14.getGridFunctionTypeWithComponents=" << u14.getGridFunctionTypeWithComponents()
	     << ", vertexCenteredWith0Components = " << GridFunctionParameters::vertexCenteredWith0Components
	     << endl;
      }
    }
    else
    {
      if(  u14.getGridFunctionType() != GridFunctionParameters::cellCentered )
      {
	errorCount++;
	cout << "ERROR: u14.getGridFunctionType=" << u14.getGridFunctionType() 
	     << ", cellCentered = " << GridFunctionParameters::cellCentered << endl;
      }
      if( u14.getGridFunctionTypeWithComponents() != GridFunctionParameters::cellCenteredWith0Components )
      {
	errorCount++;
	cout << "ERROR: u14.getGridFunctionTypeWithComponents=" << u14.getGridFunctionTypeWithComponents()
	     << ", cellCenteredWith0Components = " << GridFunctionParameters::cellCenteredWith0Components
	     << endl;
      }
    }
    if( u14.getNumberOfComponents()!=0 )
    {
      errorCount++;
      cout << "ERROR: u14.getNumberOfComponents=" << u14.getNumberOfComponents() << " =0? \n";
    }
    if(  u14.getFaceCentering() != GridFunctionParameters::none )
    {
      errorCount++;
      cout << "ERROR: u14.getFaceCentering() should be none ! \n";
      determineFaceCentering( u14 );
    }

/* ----
  realCompositeGridFunction u15(og,GridFunctionParameters::cellCentered,2), u16(og), u15a, u15b;
  u15=0.;
  u15a.link(u15,Range(0,0));
  u15b.link(u15,Range(1,1));
  u16=1.;
  u15a=u16;
  u15b=2.*u16;
  u15b.display("Here is u15b, should be 2");
  u15.display("Here is u15, should be 1,2");
---- */  

  } // end loop over grids
  
  if( errorCount==0 )
    cout << "$$$$$$$ Test successfully completed $$$$$$$\n";
  else    
    cout << "XXXXXX Test FAILED, there were " << errorCount << " errors detected XXXXXXX\n";

  return 0;
}
