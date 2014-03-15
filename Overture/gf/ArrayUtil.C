#include "ArrayUtil.h"
#include "ParallelUtility.h"
#include "display.h"

/* --- these next use some altered versions of P++ files :
#ifdef USE_PPP
  extern bool automaticCommunication;  // *wdh* 
#else
  static bool automaticCommunication;  // *wdh* 
#endif
----------- */

// ** do this for now **
static bool automaticCommunication=true;

void ArrayUtil::
turnOnAutomaticCommunication(){automaticCommunication=true;}  //

void ArrayUtil::
turnOffAutomaticCommunication(){automaticCommunication=false;}  //


bool ArrayUtil::getAutomaticCommunication(){ return automaticCommunication;}  //


static int sent=0, received=0; 

int ArrayUtil::
getSent()
{
  int sentNew=Diagnostic_Manager::getNumberOfMessagesSent()-sent;
  sent+=sentNew;
  return sentNew;
}

int ArrayUtil::
getReceived()
{
  int receivedNew=Diagnostic_Manager::getNumberOfMessagesReceived()-received;
  received+=receivedNew;
  return receivedNew;
}

void ArrayUtil::
printMessageInfo( const char* msg, FILE *file /* =stdout */ )
{
  if( Communication_Manager::My_Process_Number==0 )
  {
    fprintf(file,"%s\n",msg);
    fprintf(file," new messages sent=%i, new messages received=%i\n",getSent(),getReceived());
    fprintf(file," total messages sent=%i, total messages received=%i\n",Diagnostic_Manager::getNumberOfMessagesSent(),
                   Diagnostic_Manager::getNumberOfMessagesReceived());
    fprintf(file," total number of ghost boundary updates=%i\n",Diagnostic_Manager::getNumberOfGhostBoundaryUpdates());
  }
}


// Put this here for now -- should be moved to OGFunction
int ArrayUtil::
assignGridFunction( OGFunction & exact, 
		    realMappedGridFunction & u, 
                    const Index &I1, const Index&I2, const Index&I3, const Index & N, const real t)
{


  MappedGrid & mg = *u.getMappedGrid();
  
  const bool isRectangular = mg.isRectangular();
  
  if( !isRectangular )
    mg.update(MappedGrid::THEcenter );

  const realArray & center = !isRectangular ? mg.center() : u;

  #ifdef USE_PPP 
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(center,xLocal);
  #else
    const realSerialArray & uLocal = u; 
    const realSerialArray & xLocal = center; 
  #endif

  Index J1=I1, J2=I2, J3=I3, J4=N;
  Index nullIndex;
  if( J1==nullIndex ) J1=u.dimension(0);
  if( J2==nullIndex ) J2=u.dimension(1);
  if( J3==nullIndex ) J3=u.dimension(2);
  if( J4==nullIndex ) J4=u.dimension(3);
  

//   const int n1a = max(J1.getBase() , uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
//   const int n1b = min(J1.getBound(),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));

//   const int n2a = max(J2.getBase() , uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
//   const int n2b = min(J2.getBound(),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));

//   const int n3a = max(J3.getBase() , uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
//   const int n3b = min(J3.getBound(),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));

  // assign parallel ghost boundaries too

  const int n1a = max(J1.getBase() , uLocal.getBase(0));
  const int n1b = min(J1.getBound(),uLocal.getBound(0));

  const int n2a = max(J2.getBase() , uLocal.getBase(1));
  const int n2b = min(J2.getBound(),uLocal.getBound(1));

  const int n3a = max(J3.getBase() , uLocal.getBase(2));
  const int n3b = min(J3.getBound(),uLocal.getBound(2));

  if( n1a>n1b || n2a>n2b || n3a>n3b ) return 0; 

  J1 = Range(n1a,n1b), J2 = Range(n2a,n2b), J3 = Range(n3a,n3b);

  // ::display(xLocal,"assign grid function: xLocal","%6.2f ");
  

  realSerialArray & u0 = (realSerialArray&)uLocal;
  for( int n=J4.getBase(); n<=J4.getBound(); n++ )
    exact.gd( u0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,J1,J2,J3,n,t);

  return 0;

}


void ArrayUtil::
assign( realGridCollectionFunction & u, const realGridCollectionFunction & v )
// Assign two grid collection functions without communication
{
  GridCollection & gc = *u.getGridCollection();
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    assign( u[grid],v[grid] );
  }
  

}

void ArrayUtil::
assign( realArray & u, const realArray & v )
// Assign two arrays without communication
{
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);

    uLocal=vLocal;
  #else
    u=v;
  #endif

  
}

void ArrayUtil::
assign( realArray & u, const realArray & v, 
        const Index & I1, const Index & I2, const Index & I3, const Index & I4 )
// Assign two arrays without communication
{
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);

    realSerialArray & uu = (realSerialArray&) uLocal;


    Index J1=I1, J2=I2, J3=I3, J4=I4;
    Index nullIndex;
    if( J1==nullIndex ) J1=u.dimension(0);
    if( J2==nullIndex ) J2=u.dimension(1);
    if( J3==nullIndex ) J3=u.dimension(2);
    if( J4==nullIndex ) J4=u.dimension(3);


    // assign parallel ghost boundaries too

    const int n1a = max(J1.getBase() , uLocal.getBase(0));
    const int n1b = min(J1.getBound(),uLocal.getBound(0));

    const int n2a = max(J2.getBase() , uLocal.getBase(1));
    const int n2b = min(J2.getBound(),uLocal.getBound(1));

    const int n3a = max(J3.getBase() , uLocal.getBase(2));
    const int n3b = min(J3.getBound(),uLocal.getBound(2));

    if( n1a>n1b || n2a>n2b || n3a>n3b ) return; 

    J1 = Range(n1a,n1b), J2 = Range(n2a,n2b), J3 = Range(n3a,n3b);


    uu(J1,J2,J3,J4)=vLocal(J1,J2,J3,J4);



  #else
    u(I1,I2,I3,I4)=v(I1,I2,I3,I4);
  #endif

  
}

void ArrayUtil::
assign( realArray & u, const Index & I1_, const Index & I2_, const Index & I3_, const Index & I4_,
        const realArray & v, const Index & J1_, const Index & J2_, const Index & J3_, const Index & J4_ )
// =====================================================================================================
// /Description: Assign two arrays without communication (it is assumed that they have the same parallel
//       distribution
//
//            u(I1,I2,I3,I4)=v(J1,J2,J3,J4)
// 
// =====================================================================================================
{
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);


    Index I1=I1_, I2=I2_, I3=I3_, I4=I4_;
    Index nullIndex;
    if( I1==nullIndex ) I1=u.dimension(0);
    if( I2==nullIndex ) I2=u.dimension(1);
    if( I3==nullIndex ) I3=u.dimension(2);
    if( I4==nullIndex ) I4=u.dimension(3);

    Index J1=J1_, J2=J2_, J3=J3_, J4=J4_;
    if( J1==nullIndex ) J1=u.dimension(0);
    if( J2==nullIndex ) J2=u.dimension(1);
    if( J3==nullIndex ) J3=u.dimension(2);
    if( J4==nullIndex ) J4=u.dimension(3);


    // assign parallel ghost boundaries too

    int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);  

    if( ok )
    {
      ok = ParallelUtility::getLocalArrayBounds(v,vLocal,J1,J2,J3,includeGhost);  
      assert( ok );
      
      uLocal(I1,I2,I3,I4)=vLocal(J1,J2,J3,J4);
    }
    


  #else
    u(I1_,I2_,I3_,I4_)=v(J1_,J2_,J3_,J4_);
  #endif

  
}

void ArrayUtil::
assign( realGridCollectionFunction & u, real value )
// ==================================================================================
//    Assign u=value  (including parallel ghost boundaries)
// ==================================================================================
{
  GridCollection & gc = *u.getGridCollection();
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    assign( u[grid],value );
  }
  

}

void ArrayUtil::
assign( realArray & u, real value, 
        const Index & I1, const Index & I2, const Index & I3, const Index & I4 )
// ==================================================================================
//   Assign u(I1,I2,I3,I4)=value
// ==================================================================================
{
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray & uu = (realSerialArray&) uLocal;

    Index J1=I1, J2=I2, J3=I3, J4=I4;
    Index nullIndex;
    if( J1==nullIndex ) J1=Range(uLocal.getBase(0),uLocal.getBound(0)); 
    if( J2==nullIndex ) J2=Range(uLocal.getBase(1),uLocal.getBound(1));
    if( J3==nullIndex ) J3=Range(uLocal.getBase(2),uLocal.getBound(2));
    if( J4==nullIndex ) J4=Range(uLocal.getBase(3),uLocal.getBound(3));


    // assign parallel ghost boundaries too

    const int n1a = max(J1.getBase() , uLocal.getBase(0));
    const int n1b = min(J1.getBound(),uLocal.getBound(0));

    const int n2a = max(J2.getBase() , uLocal.getBase(1));
    const int n2b = min(J2.getBound(),uLocal.getBound(1));

    const int n3a = max(J3.getBase() , uLocal.getBase(2));
    const int n3b = min(J3.getBound(),uLocal.getBound(2));

    if( n1a>n1b || n2a>n2b || n3a>n3b ) return; 

    J1 = Range(n1a,n1b), J2 = Range(n2a,n2b), J3 = Range(n3a,n3b);


    uu(J1,J2,J3,J4)=value; 



  #else
    u(I1,I2,I3,I4)=value; 
  #endif

  
}

extern int APP_Global_Array_ID;

static int numberOfArrayIDs=0;
void ArrayUtil::checkArrayIDs(const aString & label, bool printNumber /*= false */ )
{
  if( APP_Global_Array_ID>numberOfArrayIDs )
  {
    if( Communication_Manager::My_Process_Number==0 )
      printf("%s: number of array ID's has increased to %i\n",(const char *)label,APP_Global_Array_ID);
    numberOfArrayIDs=APP_Global_Array_ID;
  }
  else if( APP_Global_Array_ID<numberOfArrayIDs )
  {
    if( Communication_Manager::My_Process_Number==0 )
      printf("%s: number of array ID's has decreased to %i\n",(const char *)label,APP_Global_Array_ID);
    numberOfArrayIDs=APP_Global_Array_ID;
  }
  else if( printNumber )
  {
    if( Communication_Manager::My_Process_Number==0 )
      printf("%s: number of array ID's is %i\n",(const char *)label,APP_Global_Array_ID);
  }
  
}



