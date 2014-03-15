// #undef BOUNDS_CHECK

#include "Overture.h"
#include "SquareMapping.h"
#include "display.h"

// ** MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end

void initOvertureGlobalVariables();

/* ----
RealArray Overture::nullRealArray();  
MappingParameters Overture::nullMappingParameters()=MappingParameters(TRUE); 
Mapping::LinkedList Mapping::staticMapList();  // list of Mappings for makeMapping

const aString nullString="";                     // null string for default arguments
const aString blankString=" ";                   // blank string for default arguments
const Index nullIndex;
const Range nullRange;
const Range faceRange=Range(0,-3);     // for specifying the special "axis" Range for a face 
                                       //   centred grid function

---- */

// const Range faceRange=Range(0,-3);     // for specifying the special "axis" Range for a face 

void 
passByValue( realMappedGridFunction u, realCompositeGridFunction u0 )
{
  display(u,"passByValue: u:","%4.1f ");
  u0.display("passByValue u0:","%4.1f ");
}  

void 
passByReference( const realCompositeGridFunction & u )
{
  real maxValue = max(u);
  cout << "passByReference: max(u) = " << maxValue << endl;
}



//================================================================================
//  Test the gridFunction classes
//
//================================================================================

bool
equals(const real a, const real b )
{
  if( fabs(a-b)/(fabs(a)+fabs(b)+1.) < REAL_EPSILON*100. )
    return TRUE;
  else
    return FALSE;
}

int
printPositionInfo( realMappedGridFunction & u )
{
  
  printf("u.positionOfComponent=");
  int i;
  for( i=0; i<u.numberOfComponents(); i++ )
    printf(" %i,",u.positionOfComponent(i));
  printf("\n");
  
  printf("u.positionOfCoordinate=");
  for( i=0; i<3; i++ )
    printf(" %i,",u.positionOfCoordinate(i));
  printf("\n");
  
  return 0;
}



int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture


  const int maxNumberOfGridsToTest=4;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "square5CC", "cic", "cicCC" };
    
  if( argc > 1 )
  { 
    numberOfGridsToTest=1;
    gridName[0]=argv[1];
  }
  else
    cout << "Usage: `tgf [<gridName>]' \n";


  Range all;                  // a null Range is used when constructing grid functions, it indicates
                              // the positions of the coordinate axes

  assert( equals(pow(2,3),8) );
  assert( equals(pow(-2,3),-8) );
  assert( equals(pow(2,-3),1./8.) );
  assert( equals(pow(-2,-3),-1./8.) );
  assert( equals(pow(2,4),16) );
  assert( equals(pow(-2,4),16) );
  assert( equals(pow(3.,4.),81.) );
  assert( equals(pow(-3.,4.),81.) );
  

  assert( fabs(Pi-3.1415926)< 1.e-5 );
  assert( nullRange.getBase()==0 && nullRange.getBound()==-1 && nullRange.length()==0 );
  nullRange.display("Here is nullRange");
  faceRange.display("Here is faceRange");
  assert( blankString==" " );
  assert( nullString=="" );
  cout << "nullString=[" << nullString << "]\n";
  cout << "blankString=[" << blankString << "]\n";
  

  Index I1,I2,I3;
  int axis;


  // Make a mapped grid:
  SquareMapping square;
  square.setGridDimensions(axis1,11);
  square.setGridDimensions(axis2,11);
  MappedGrid cg(square);
  cg.update();

  MappedGrid cg2(square);  // make another to test memory leak...
  cg2.update();

  // make a Mapped Grid Function:
  realMappedGridFunction u(cg),v,v2,v3,v4,uc;

  assert( u.getName()==blankString );
  // cout << "----here is u.getName: " << u.getName() << " (before setting)" << endl;

  u.setName("u");
  // cout << "----here is u.getName: " << u.getName() << endl;
  assert( u.getName()=="u" );
  IntegerArray dim(2,3);
  dim=0;
  dim(0,0)=-1;  dim(1,0)=11;
  dim(0,1)=-1;  dim(1,1)=11;
  assert( max(abs(u.getMappedGrid()->dimension()-dim))==0 );
  // u.mappedGrid->dimension().display("Here is u.mappedGrid->dimension");


  printPositionInfo(u);

  u=1.;
  uc=u;                    
  assert( max(fabs(uc-1.))==0. && max(fabs(uc-u))==0. );
  // uc.display("here is uc=u (test the deep copy)");

  v.reference(v2);
  Range R(1,9);
  u=1;
  u(R)=2;
  display(u,"u","%4.1f ");
  u.periodicUpdate();
  display(u,"u after periodic swap on non-periodic array","%4.1f ");
  cg.isPeriodic()(axis1)=TRUE;
  u.periodicUpdate();
  display(u,"u after periodic swap (periodic in direction axis1","%4.1f ");

  v4=u;
  v4.periodicUpdate();  

  u.updateToMatchGrid( cg,all,all,Range(0,1));
  u=8.;
  display(u,"===========Here is u.updateToMatchGrid( cg,all,all,Range(0,1)) (should=8)======","%4.1f "); 

  u.updateToMatchGrid( cg,all,all,all,Range(0,1));   // 2 components, positionOfComponent=3
  u=9.;
  display(u,"===========Here is u.updateToMatchGrid( cg,all,all,all,Range(0,1)) (should=9)======","%4.1f "); 

  u.setName("u"); u.setName("u.0",0); u.setName("u.1",1);
  cout << "+++ names for u: " << u.getName() << "," << u.getName(0) << "," << u.getName(1) << endl;

  u.updateToMatchGrid(all,all,all,2);          // do again!
  u=1.;
  u(R,0,0,0)=2.;
  u(R,0,0,1)=3.;
  display(u,"u after updateToMatchGrid( cg,all,all,all,2) (should be u(R,0,0,0:1)=(2,3) ","%4.1f ");
  printPositionInfo(u);

  u.periodicUpdate();
  display(u,"u after periodic swap (periodic in direction axis1","%4.1f ");
  
  
  v3.updateToMatchGrid( cg ); 
  v3=22.;
  display(v3,"Here is v3.updateToMatchGrid( cg )","%4.1f ");

  v3.updateToMatchGrid( cg,all,Range(0,2) ); 
  v3=33.;
  display(v3,"Here is v3.updateToMatchGrid( cg,all,Range(0,2) )","%4.1f ");

  v3.updateToMatchGrid( cg,Range(0,1));
  v3=22.;
  display(v3,"Here is v3.updateToMatchGrid( cg,Range(0,1) )","%4.1f ");


  u.updateToMatchGrid(all,1);
  u=0.;
  display(u,"u after updateToMatchGrid(all,1)","%4.1f ");
  v.reference(u);
  display(v,"v after v.reference(u)","%4.1f ");
  v=10.;     
  display(v,"v after v=10.","%4.1f ");
  display(u,"u after v=10.","%4.1f ");
     
  u.updateToMatchGrid(all,all,2);
  u=8.;
  display(u,"u after updateToMatchGrid(all,all,2)","%4.1f ");
  printPositionInfo(u);

  v.reference(u);
  v.updateToMatchGrid(cg,all,2);
  v=9.;
  display(v,"v after v.updateToMatchGrid(cg,all,2) (should be 9)","%4.1f ");
  display(u,"u after v.updateToMatchGrid(cg,all,2) (should be 9)","%4.1f ");
  printPositionInfo(u);

  cout << "Now test the link function and updateToMatchGrid...\n";
  u.breakReference();
  u.updateToMatchGrid(all,all,all,2);
  u=8.;
  display(u,"u after updateToMatchGrid(all,all,all,2)","%4.1f ");
  printPositionInfo(u);

  v.link(u,Range(0,1));
// *not allowed anymore**  v.updateToMatchGrid(cg,all,2);
  v=9.;
  display(v,"v after link and v=9 (should be 9)","%4.1f ");
  display(u,"u after v=9 (should be 9)","%4.1f ");
  printPositionInfo(u);

  u.setIsFaceCentered(axis2,0);
  for( axis=axis1; axis<u.numberOfDimensions(); axis++ )
    cout << "u.getIsFaceCentered(" << axis << ",0) = " << u.getIsFaceCentered(axis,0) << endl;

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  cout << "Call u.destroy\n";
  u.destroy();
  display(u,"Here is u1 after destroy","%4.1f ");

  u.updateToMatchGridFunction(v); // **** added 981024
  
  v=u;

  // ---------------------------------------------------------------------------------------

  aString nameOfDirectory = ".";

  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];

    CompositeGrid og;
    cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;
    getFromADataBase(og,nameOfOGFile);
    cout << "++++++++update and compute the geometry of  og \n";
    og.update(MappedGrid::THEvertex | 
	      MappedGrid::THEcenter | 
	      MappedGrid::THEmask | 
	      MappedGrid::THEvertexDerivative | 
	      MappedGrid::THEinverseVertexDerivative | 
	      CompositeGrid::THEvertexBoundaryNormal);

    MappedGrid & mg = og[0];
  
//  og.update(CompositeGrid::EVERYTHING);

    cout << " og.numberOfComponentGrids() = " << og.numberOfComponentGrids() << endl;
    cout << " og.numberOfDimensions() = " << og.numberOfDimensions() << endl;
    display(og.numberOfInterpolationPoints,"Here is og.numberOfInterpolationPoints");
    int grid;
    for( grid=0; grid<og.numberOfComponentGrids(); grid++)
    {
      
      display(og[grid].gridSpacing(),"Here is og[grid].gridSpacing()");
      displayMask(og[grid].mask(),"Here is og[grid].mask()");
      display(og[grid].sharedBoundaryFlag(),"Here is og[grid].sharedBoundaryFlag()");

      if( og.numberOfInterpolationPoints(grid)>0 )
      {
	display(og.interpolationCoordinates[grid],"Here is og.interpolationCoordinates[grid]");
	display(og.interpoleeGrid[grid],"Here is og.interpoleeGrid[grid]");
	display(og.interpoleeLocation[grid],"Here is og.interpoleeLocation[grid]");
      }
    }
    cout << " og[0].numberOfDimensions() = " << og[0].numberOfDimensions() << endl;
    display(og[0].dimension()," og[0].dimension() ");
    display(og[0].indexRange()," og[0].indexRange() ");
    display(og[0].gridIndexRange()," og[0].gridIndexRange() ");
    display(og[0].isPeriodic()," og[0].isPeriodic() ");
    display(og[0].boundaryCondition()," og[0].boundaryCondition() ");
    display(og[0].discretizationWidth()," og[0].discretizationWidth() ");

    display(og[0].vertex(),"Here is og[0].vertex()","%4.1f ");
    display(og[0].center(),"Here is og[0].center()","%4.1f ");
    display(og[0].boundaryCondition(),"Here is og[0].boundaryCondition()");
    display(og[0].inverseVertexDerivative(),"Here is og[0].inverseVertexDerivative()","%4.1f ");
  
    axis=0;
    int side=0;
    display(og[0].vertexBoundaryNormal(side,axis),"Here is og[0,axis).vertexBoundaryNormal","%4.1f ");
    getBoundaryIndex(og[0].gridIndexRange(),0,0,I1,I2,I3);
    realArray normal;
    normal=og[0].vertexBoundaryNormal(side,axis)(I1,I2,I3,0)+1.;
    og[0].vertexBoundaryNormal(side,axis)(I1,I2,I3,0)+=og[0].vertexBoundaryNormal(side,axis)(I1,I2,I3,1);


    // Krister's bug.
    cout << "%%%%%%%%%%%%%%%%% Test Krister's bug %%%%%%%%%%%%%%%%%%%%%%%%%%% \n";
    realCompositeGridFunction uu1(og), uu2(og);
    uu2=1.;
    uu1.destroy();
    uu1.updateToMatchGridFunction(uu2,all,all,all,all);
    uu1=uu2;

    cout << "%%%%%%%%%%%%%%%%% Test David's bug %%%%%%%%%%%%%%%%%%%%%%%%%%% \n";
    realCompositeGridFunction a1(og,all,all,all,2),a2(og,all,all,all);
    a2=3.;
    cout << " before: a1.getComponentDimension(0) = " << a1.getComponentDimension(0) << ", =2? \n";
    a1.destroy();
    a1=a2;
    cout << " after: a1.getComponentDimension(0) = " << a1.getComponentDimension(0) <<  ", =1? \n";


    realCompositeGridFunction a(og,all,all,all,2,faceRange),b(og,all,all,all,2,faceRange),c;
    a=1.; b=2.;
    cout << " a.getFaceCentering() = " << a.getFaceCentering() << endl;
    cout << " b.getFaceCentering() = " << b.getFaceCentering() << endl;
    c=a-b;
    cout << " (a-b).getFaceCentering() = " << (a-b).getFaceCentering() << endl;
    cout << " c=a-b; c.getFaceCentering() = " << c.getFaceCentering() << endl;

    realCompositeGridFunction u0(og), u0c;  
    u0=3.;
    u0.display("Here is u0=3.","%4.1f ");
    u0.setName("u0");
    cout << "here is u0.getName: " << u0.getName() << endl;
    u0=5.;
    u0.display("Here is u0=5.","%4.1f ");
    display(u0[0],"Here is u0[0]","%4.1f ");

    cout << "Test evaulate...(max should be 10)\n";
    passByReference(evaluate(u0+u0));   // test evaluate

    realMappedGridFunction & u00 = u0[0];
    display(u00,"here is u00: realMappedGridFunction & u00 = u0[0]","%4.1f ");
    u0c=u0;                    
    u0c.display("here is u0c=u0 (test the deep copy)","%4.1f ");

    realMappedGridFunction ucc=u00;  // this should call the copy constructor
    display(ucc,"Here is realMappedGridFunction ucc=u (test copy constructor)","%4.1f ");

    realCompositeGridFunction u0cc=u0;  // this should call the copy constructor
    u0cc.display("Here is realCompositeGridFunction u0cc=u0","%4.1f ");

    cout << "Pass gridFunctions by value " << endl;
    passByValue( u00,u0 );
  

    realArray fa;
    fa=u0[0];
    getIndex(og[0].indexRange(),I1,I2,I3);
    fa=-7.;
    u0[0](I1,I2,I3)=-fa(I1,I2,I3);
    display(u0[0],"Here is u0[0](I1,I2,I3)=-fa(I1,I2,I3) (should = 7)","%4.1f ");

    realCompositeGridFunction u1,u2,u3,u4,u5;
    u1.updateToMatchGrid( og );
    u1=55.;
    u1.display("Here is u1.updateToMatchGrid( og ) (=55?)","%4.1f ");

    evaluate(u1/55.+u1/55.).display("Here is evaluate(u1/55+u1/55) (=2.?)","%4.1f ");


    // Make a matrix grid function:
/* ---
   u2.updateToMatchGrid(og,all,all,Range(-2,0),Range(5,6));
   getIndex(og[0].dimension(),I1,I2,I3);
   u2[0](I1,I2,-2,5)=1.;
   u2[0](I1,I2,-1,5)=2.;
   u2[0](I1,I2, 0,5)=3.;
   u2[0](I1,I2,-2,6)=4.;
   u2[0](I1,I2,-1,6)=5.;
   u2[0](I1,I2, 0,6)=6.;
   --- */
    u2.updateToMatchGrid(og,all,all,all,Range(-2,0),Range(5,6));
    getIndex(og[0].dimension(),I1,I2,I3);
    u2=0.;
    u2[0](I1,I2,I3,-2,5)=1.;
    u2[0](I1,I2,I3,-1,5)=2.;
    u2[0](I1,I2,I3, 0,5)=3.;
    u2[0](I1,I2,I3,-2,6)=4.;
    u2[0](I1,I2,I3,-1,6)=5.;
    u2[0](I1,I2,I3, 0,6)=6.;

    u2.display("Here is u2.updateToMatchGrid(all,all,Range(-2,0),Range(5,6))","%4.1f ");
    aString u2Name = blankString;
    cout << "Here is blankString = " << blankString << " or = " << u2Name <<  endl;

    u2Name = u2.getName();
    cout << "Here is u2.getName() (no name assigned) = " << u2Name << endl;
    cout << "Here is u2.getName() (no name assigned) = " << u2.getName() << endl;

    u3.link(u2,Range(-2,-1),Range(6,6));
    u3.display("here is u3.link(u2,Range(-2,-1),Range(6,6))   (should be =4,5)","%4.1f ");

    u3.link(u2,Range(-1,-1),Range(5,5),Range(0,0),Range(0,0));
    u3.display("here is u3.link(u2,Range(-1,-1),Range(5,5),Range(0,0),Range(0,0),Range(0,0)) (should be =2)","%4.1f ");

    u3.link(u2,Range(-2,-1),Range(6,6));
    u3.display("here is u3.link(u2,Range(-2,-1),Range(6,6) (should be =4,5)","%4.1f ");

    u3.link(u2,Range(-2,0),Range(5,6));
    u3.display("here is u3.link(u2,Range(-2,0),Range(5,6) (should be =1,2,3,4,5,6)","%4.1f ");


  // Make a grid function with 5 Indicies
    u2.updateToMatchGrid(og,all,all,all,Range(0,1),Range(1,2));
    u2=0.;
    getIndex(og[0].dimension(),I1,I2,I3);
    u2[0](I1,I2,I3,0,1)=1.;
    u2[0](I1,I2,I3,1,1)=2.;
    u2[0](I1,I2,I3,0,2)=3.;
    u2[0](I1,I2,I3,1,2)=4.;

    u2.display("Here is u2.updateToMatchGrid(og,all,all,all,Range(0,1),Range(1,2))","%4.1f ");


    display(u2[0](I1,I2,I3,1,1),"Here is u2[0](I1,I2,I3,1,1) (should=2)","%4.1f ");
    cout << "Here is u2[0](0,0,0,1,1) = " << u2[0](0,0,0,1,1) << " (should be 2))\n";
    display(u2[0](I1,I2,I3,1,2),"Here is u2[0](I1,I2,I3,1,2) (should=4)","%4.1f ");
    cout << "Here is u2[0](0,0,0,1,2) = " << u2[0](0,0,0,1,2) << " (should be 4))\n";

    u2.updateToMatchGrid( og,all,all,Range(0,1) );     // 2 components
    u2=-3.;
    u2.display("Here is u2.updateToMatchGrid( og,all,all,Range(0,1) (should be -3))","%4.1f ");

    u2.setName("u2"); u2.setName("u2.0",0); u2.setName("u2.1",1);
    cout << "+++ names for u2: " << u2.getName() << "," << u2.getName(0) << "," << u2.getName(1) << endl;

    u2.updateToMatchGrid( og,Range(1,1),all,all,all );  
    u2.updateToMatchGrid();  
    u2=22.;
    u2.display("Here is u2.updateToMatchGrid( og,Range(1,1),all,all,all ) and updateToMatchGrid","%4.1f ");

    u2.updateToMatchGrid( og,all,Range(2,3) ); 
    u2=33.;
    u2.display("Here is u2.updateToMatchGrid( og,all,Range(2,3) ) )","%4.1f ");

    u2.updateToMatchGrid( og,all,all,all,Range(0,1) ); 
    u2=33.;
    u2.display("Here is u2.updateToMatchGrid( og,all,all,all,Range(0,1) )","%4.1f ");

    for(grid=0; grid<u2.numberOfComponentGrids(); grid++ )
    {
      getIndex( og[grid].dimension(),I1,I2,I3 );
      u2[grid](I1,I2,I3,0)= real(grid+1);
      u2[grid](I1,I2,I3,1)=-real(grid+1);
    }
  
    cout << "here is max(u2) = " << max(u2) << " = 2?" << endl;
    cout << "here is min(u2) = " << min(u2) << " =-2?" << endl;

    u5=u1;
    u5.periodicUpdate();

    cout << "\n\n";
    u2.display("Here is u2 before we test the link function","%4.1f ");

    u4.link(u2,Range(0,0));
    u4.display("Here is u4.link(u2,Range(0,0))","%4.1f ");

    u4.link(u2,Range(1,1));
    u4.display("Here is u4.link(u2,Range(1,1))","%4.1f ");

    int number=2;
    u4.link(u2,Range(number-1,number-1));
    u4.display("Here is u4.link(u2,Range(number-1,number-11))","%4.1f ");

    u4.link(u2,Range(0,1));
    u4.display("Here is u4.link(u2,Range(0,1))","%4.1f ");
  
    Interpolant interpolant( og );
    cout << " Call interpolant.interpolate( u1 )..." << endl;
    interpolant.interpolate( u1 );
    cout << " Call u1.interpolate..." << endl;
    u1.interpolate();


/* ---
  realMultigridCompositeGridFunction umg( mgcog );
  umg=321.;
  umg.display("\n Here is a multigrid function (umg=321)");
--- */
  
    cout << "test the reference: cg3.reference(og) \n";
    CompositeGrid cg3;
    cg3.reference(og);
    cout << " og.numberOfComponentGrids() = " << cg3.numberOfComponentGrids() << endl;
    display(cg3[0].vertex(),"Here is cg3[0].vertex()","%4.1f ");
    display(cg3[0].vertexDerivative(),"Here is cg3[0].vertexDerivative()","%4.1f ");


    cout << "Test the deep copy: MappedGrid mapg = og[0]; \n";
    MappedGrid mapg = og[0];
    display(mapg.vertex(),"Here is mapg.vertex()","%4.1f ");

    cout << "++++++++++++Test MappedGrid reference and compute geometry+++++++++++" << endl;
    cout << "update original og[0] \n";
    og[0].update(MappedGrid::EVERYTHING,MappedGrid::COMPUTEgeometry);
    cout << "now update the reference \n";
    mapg.reference(og[0]);
    mapg.update(MappedGrid::EVERYTHING,MappedGrid::COMPUTEgeometry);
    cout << "now update a deep copy \n";
    mapg=og[0];
    mapg.update(MappedGrid::EVERYTHING,MappedGrid::COMPUTEgeometry);


    cout << "Set one null CompositeGridFunction = to another \n";
    realCompositeGridFunction un1,un2,un3;
    un2=un1;
    un3=un2;

/* ---
  cout << "Set one MultigridCompositeGrid equal to another \n";
  MultigridCompositeGrid mgcg2;
  mgcg2=mgcog;  // deep copy
  MultigridCompositeGrid mgcg3 = mgcog;
--- */

    cout << "Reference one GridCollection to a CompositeGrid \n";
    GridCollection cgReference;
    cgReference.reference(og);
    cout << "Here is cgReference.numberOfComponentGrids() = "
	 << cgReference.numberOfComponentGrids() << endl;

    cout << "Reference one MappedGrid to another \n";
    MappedGrid mgReference;
    mgReference.reference(og[0]);
    cout << "Here is mg.Reference.numberOfDimensions() = "
	 << cgReference.numberOfDimensions() << endl;


    getIndex(u1[0],I1,I2,I3);
    display(u1[0].getMappedGrid()->indexRange(),"Here is indexRange");
    display(u1[0].getMappedGrid()->gridIndexRange(),"Here is gridIndexRange");
    I1.display("I1 after getIndex(u,...)");
    I2.display("I2 after getIndex(u,...)");

    display(u1[0](I1,I2,I3),"u1[0] after get Index","%4.1f ");
    u1.setIsFaceCentered( axis1,0 );
    getIndex(u1[0],I1,I2,I3);
    display(u1[0](I1,I2,I3),"u1[0] after get Index (face centered,","%4.1f ");

    Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    cout << "Call u1.destroy\n";
    u1.destroy();
    u1.display("Here is u1 after destroy");
    u2=u1;


    cout << "test Lotta's bug...\n";

    realCompositeGridFunction q(og,all,all,all,3),q1,q2,q3,q4,dq;
    q=1.;
    q.display("here is q (=1)","%4.1f ");
    (-q).display("here is -q (=-1)","%4.1f ");
    (+q).display("here is +q (=-1)","%4.1f ");
    (-q+3.*q).display("here is -q+3*q (=2)","%4.1f ");
    dq=.1*q;
    dq.display("here is dq (=.1*q)","%4.1f ");

    q4=q+5.*dq;
    q4.interpolate();

    q1.link(q,Range(0,0));
    q2.link(q,Range(1,1));
    q3.link(q,Range(2,2));

    q1.display("here is q1.link(q,Range(0,0)) (should be =1)","%4.1f ");
    q=q+5.;
    q1.display("here is q1 after q=q+5 (should be =6)","%4.1f ");

    dq=.1*q;
    q4=q+5.*dq;
    q4.interpolate();
    q1.link(q4,Range(0,0));
    q2.link(q4,Range(1,1));
    q3.link(q4,Range(2,2));


    // This next loop makes sure we don't have a memory leak
    real qValue=2., q1Value=1.;
    q=qValue;
    q1.destroy(); // break the link
    q1=q;  q1=q1Value;

    q2.destroy();
    q2=q1+2.*q+q1*q-q1*3.+q/2.;

    real qResult=q1Value+2.*qValue+q1Value*qValue-q1Value*3.+qValue/2.;
    cout << " qResult = " << qResult << ", equals? max(q2)= " << max(q2) 
	 << ", and min(q2)=" << min(q2) << endl;


    q.destroy();
    cout << "Loop to check memory leak... \n";
    for(int iter=0; iter<150; iter++)
    {
      cout << ".";
      q=q1+2.*q2+q1*q2-q1*3.+q2/2.;
      q=q1+3.*q1;
      q=q1-q2/q1;
      q=q1+q2;
      q.updateToMatchGrid();
      if( iter % 10 == 0 )
	printf("\n **** Number of A++ arrays = %i ",GET_NUMBER_OF_ARRAYS);
    }
    cout << "done\n";

    realMappedGridFunction::edgeGridFunctionValues leftSide =realMappedGridFunction::startingGridIndex;
    realMappedGridFunction::edgeGridFunctionValues rightSide = realMappedGridFunction::endingGridIndex;
    
    Range Left(leftSide,leftSide);
    realMappedGridFunction uLeft(mg,Left,all,all);
    uLeft=1.;
    display(uLeft,"Here is uLeft(mg,Left,all,all)","%4.1f ");

    uLeft.updateToMatchGrid(mg,all,Range(leftSide-1,leftSide+1));
    uLeft=2.;
    display(uLeft,"uLeft.updateToMatchGrid(mg,all,Range(leftSide-1,leftSide+1))","%4.1f ");

    Range Right(rightSide,rightSide);
    realMappedGridFunction uRight(mg,Right,all,all);
    uRight=3.;
    display(uRight,"Here is uRight(mg,Right,all,all)","%4.1f ");

    realMappedGridFunction uLeftRight(mg,Left,Right,all);
    uLeftRight=4.;
    display(uLeftRight,"Here is uLeftRight(mg,Left,Right,all)","%4.1f ");
    
  }
  

    // root.unmount();
  Overture::finish();          
  printf ("Program Terminated Normally! \n");
  return 0;
}
