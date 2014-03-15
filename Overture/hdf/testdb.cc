#include <iostream>
#include <list>

#include "DBase.hh"

#include "kk_synch.hh"

using namespace std;

/** test elements of the database class
 */

namespace {

  class A { 
    int i;
  };
  
  size_t nFoundWithFunction;
  
  void writeEntryName(string name, DBase::Entry &e) { nFoundWithFunction++; cout<<name<<endl; }
  
  typedef std::list<A> AListT;
  
  class WriteEntryName {
  public:
    WriteEntryName() : nFound(0) {}
    void operator()(string name, DBase::Entry &e) { nFound++; cout<<name<<endl; }
    size_t nFound;
  };
  
}

int main(void)
{
  GANTLET_START(-1);

  DBase::DataBase db;
  
  int i=10;

  try {
    GANTLET_BEGIN_TEST;
    db.put("i",i);
    GANTLET_PASS_TEST("put an integer into the db");
  } 
  catch ( DBase::DBErr &e ) {
    GANTLET_FAIL_TEST("could not add int to the db : "+e.repr());
  }
  catch (...) {
    GANTLET_FAIL_TEST("could not add int to the db");
  }

#ifdef KK_DEBUG
  try {
    GANTLET_BEGIN_TEST;
    db.put<AListT>("alist");
    GANTLET_PASS_TEST("put a list into the db");
  }
  catch (DBase::DBErr &e) {
    GANTLET_FAIL_TEST("could not put a list into the db : "+e.repr());
  }
  catch (...) {
    GANTLET_FAIL_TEST("could not put a list into the db");
  }    

  try {
    GANTLET_BEGIN_TEST;
    db.get<int>("j");
    GANTLET_FAIL_TEST("did not catch missing entry request error ");
  } 
  catch (DBase::DBErr &e) {
    GANTLET_PASS_TEST("caught missing entry request error ");
  } 
  catch (...) {
    GANTLET_FAIL_TEST("unknown missing entry request error ");
  } 

  try {
    //    db.put<int>(i,"i");
    GANTLET_BEGIN_TEST;
    db.put<int>("i",i);
    GANTLET_FAIL_TEST("did not catch duplicate put error ");
  } 
  catch (DBase::DBErr &e) {
    GANTLET_PASS_TEST("caught duplicate put error ");
  }
  catch (...) {
    GANTLET_FAIL_TEST("unknown duplicate put error ");
  }

#endif

  /// make sure we get the data we put in
  //  int ii=db.get<int>("i");
  //  cout<<ii<<endl;
  GANTLET_ASSERT(db.get<int>("i")==i,"");

  /// test toggleing and untoggling of traits
  GANTLET_ASSERT(!db.traitActive("i",DBase::persistent),"");
//   if ( db.traitActive("i",DBase::persistent) )
//     cout<<"trait active"<<endl;
//   else
//     cout<<"trait inactive"<<endl;

  /// toggle persistence on
  db.toggleTrait("i",DBase::persistent);
  GANTLET_ASSERT(db.traitActive("i",DBase::persistent),"");

//   if ( db.traitActive("i",DBase::persistent) )
//     cout<<"trait active"<<endl;
//   else
//     cout<<"trait inactive"<<endl;

  db.toggleTrait("i", DBase::distributed);
  GANTLET_ASSERT(db.traitActive("i",DBase::distributed),"");
  GANTLET_ASSERT(db.traitActive("i",DBase::persistent),"");
  
  //  if ( db.traitActive("i",DBase::persistent) )
  //    cout<<"persistent active"<<endl;
  
  //  if ( db.traitActive("i",DBase::distributed) )
  //    cout<<"distributed active"<<endl;

  db.toggleTrait("i",DBase::distributed);
  GANTLET_ASSERT(!db.traitActive("i",DBase::distributed),"");
  GANTLET_ASSERT(db.traitActive("i",DBase::persistent),"");
  //  if ( !db.traitActive("i",DBase::distributed) )
  //    cout<<"distributed inactive"<<endl;

  //  if ( db.traitActive("i",DBase::persistent) )
  //    cout<<"persistent active"<<endl;

  db.toggleTrait("i",DBase::persistent);
  GANTLET_ASSERT(!db.traitActive("i",DBase::distributed),"");
  GANTLET_ASSERT(!db.traitActive("i",DBase::persistent),"");

  //  if ( !db.traitActive("i",DBase::persistent) )
  //    cout<<"persistent inactive"<<endl;

  //  if ( db.traitActive("i",DBase::distributed) )
  //    cout<<"distributed active"<<endl;

  /// try to remove i
  try {
    GANTLET_BEGIN_TEST;
    db.remove("i");
    GANTLET_PASS_TEST("remove entry");
  } 
  catch ( DBase::DBErr &e ) {
    GANTLET_FAIL_TEST("remove entry : "+e.repr());
  }
  catch ( ... ) {
    GANTLET_FAIL_TEST("remove entry");
  }

  //  cout<<"successfully removed i"<<endl;

#ifdef KK_DEBUG
  /// try to remove i again, catching the failure
  try {
    GANTLET_BEGIN_TEST;
    db.remove("i");
    GANTLET_FAIL_TEST("remove missing entry error");
  } 
  catch ( DBase::DBErr &e ) {
    GANTLET_PASS_TEST("remove missing entry error");
  }
  catch ( ... ) {
    GANTLET_FAIL_TEST("unknown remove missing entry error");
  }
#endif
      
  int &j = db.put<int>("j");

  j = 200;
  GANTLET_ASSERT(db.get<int>("j")==200,"");
  //  cout<<"j from database should be 200, is "<<db.get<int>("j")<<endl;

  //  A & a = db.put<A>("a");

  /// test the database processing methods
  //  cout<<"processing entries with function"<<endl;
  nFoundWithFunction=0;
  db.processEntries(writeEntryName);
  GANTLET_ASSERT(nFoundWithFunction==db.size(),"process entries with function");
  //  cout<<"processing entries with function class"<<endl;
  WriteEntryName w;
  db.processEntries<WriteEntryName>(w);
  GANTLET_ASSERT(w.nFound==db.size(),"process entries with function class");

  GANTLET_FINISH;

  return 0;
    
}
