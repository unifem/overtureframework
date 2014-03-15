// ====================================================================================
///  \file tdb.C
///  \brief test Kyle's DBase class.
// ===================================================================================


#include <iostream>
#include <list>

typedef double real;


#define EXTERN_C_NAME(x) x ## _
#define KK_DEBUG

#include "DBase.hh"

// #include "kk_synch.hh"

using namespace std;
using namespace DBase;

/** test elements of the database class
 */

namespace {

class A 
{ 
public:
int i;
};
  
string typeName(DBase::Entry &e)
{
 if( DBase::can_cast_entry<int>(e) )
   return "int";
 else  if( DBase::can_cast_entry<real>(e) )
   return "real";
 else if( DBase::can_cast_entry<A>(e) )
   return "A";
 else
   return "unknown";
}




size_t nFoundWithFunction;
  
void writeEntryName(string name, DBase::Entry &e) 
{ nFoundWithFunction++; cout<<name<<", type="<<typeName(e)<<endl; }
  
typedef std::list<A> AListT;
  
class WriteEntryName {
public:
WriteEntryName() : nFound(0) {}
void operator()(string name, DBase::Entry &e) { nFound++; cout<<name<<endl; }
size_t nFound;
};
  

enum MyEnum
{
  enum0=0,
  enum1=1,
  enum2=2,
  enum3=3
};


enum MyEnum2
{
  enum5=0,
  enum6=1,
  enum7=2,
  enum8=3
};


}



#define ut EXTERN_C_NAME(ut)
// #define getIntFromDataBase getintfromdatabase_
extern "C"
{
  void ut(DataBase *pdb);

// int getIntFromDataBase( DataBase *pdb, char *name_, int & num, int & nameLength)
// // =======================================================================
// // Use this routine from fortran to look-up a variable in the data base
// //
// // /Return value: 1=found, 0=not found, -1=name found but not the correct type
// // =======================================================================
// {
//   string name(name_,0,nameLength);
//   // remove trailing blanks

//   int i=name.size()-1;
//   while( i>0 && name[i]==' ' ) i--;
//   if( i<name.size()-1 )
//     name.erase(i+1,name.size()-i);
  
//   cout << "getInt: name=[" << name << "]\n";
//   printf("getInt: nameLength=%i, name=[%s]\n",nameLength,name.c_str());

//   // cout << "getInt: pdb="<< pdb << endl;

//   DataBase & db = *pdb;
//   printf("db: `a' is %s\n",(db.has_key("a") ? "found" : "not found"));

//   if( db.has_key(name) )
//   {
//     KK::sptr<Entry> ep = db.getEntry(name);
//     if( DBase::can_cast_entry<int>(*ep) )
//     {
//       printf("getInt: entry is of type int\n");
//       num=db.get<int>(name);
//       return 1;
//     }
//     else
//     {
//       printf("getInt:WARNING: %s is in the data-base but is NOT an 'int'.\n",name.c_str());
//       num=0;
//       return -1;
//     }
   

//   }
//   else
//   {
//     printf("getInt:WARNING: %s is not in the data-base\n",name.c_str());
//     num=0;
//   }
//   return 0;
// }
 
} // end extern "C"


int 
main(int argc, char** argv)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  DBase::DataBase db;
  
  int i=10;
  db.put<int>("i",i);
  
  int ii=db.get<int>("i");

  printf(" i=%i, ii=%i\n",i,ii);
  
  A a;
  a.i=5;
  db.put<A>("a",a);

  A aa;
  aa = db.get<A>("a");
  printf(" a.i=%i, aa.i=%i\n",a.i,aa.i);

  // Note how we create a sub-directory:
  db.put<DBase::DataBase>("subDir");
  DBase::DataBase & subDir=db.get<DBase::DataBase>("subDir");;
  
  subDir.put<int>("i",i);
  subDir.put<real>("Harvard",10.);

  printf("Entries in subDir:\n");
  nFoundWithFunction=0;
  db.processEntries(writeEntryName);

  DBase::DataBase & subDir2=db.get<DBase::DataBase>("subDir");
  printf("$$$$$$$$$ Entries in subDir2: $$$$$$$$$$$$\n");
  subDir2.processEntries(writeEntryName);


  int j;
  try 
  {
    j = db.get<int>("j");
    printf(" j=%i\n",j);
  }
  catch (DBase::DBErr &e)
  {
    printf("Error looking for j\n");
  }
  
  printf("db: `a' is %s\n",(db.has_key("a") ? "found" : "not found"));
  printf("db: `b' is %s\n",(db.has_key("b") ? "found" : "not found"));
  


  printf("Entries in db:\n");
  nFoundWithFunction=0;
  db.processEntries(writeEntryName);

  printf("\nEntries in subDir:\n");
  subDir.processEntries(writeEntryName);

  
  printf("\nEntries in db from my loop:\n");
  for( DataBase::iterator e=db.begin(); e!=db.end(); e++ )
  {
    cout << "name="<< (*e).first <<", type= "<<typeName(*((*e).second))<<endl;
  }
  

  // *** now call a fortran routine that will access the db ****

  int hello=123;

  // string name="hello";
  db.put<int>("hello",hello);
  printf("db: `hello' is %s\n",(db.has_key("hello") ? "found" : "not found"));
  
  real mu=.1;
  db.put<real>("mu",mu);




  MyEnum myEnum= enum3;
  MyEnum2 myEnum2= enum7;

  // typeid(myEnum) myEnum3;
  
  // cout << "typeid(myEnum)=" << typeid(myEnum) << endl; //  << ", name=[" << typeid(myEnum).name() << "].\n";
  
  printf(" typeid(myEnum)=[%s]\n",typeid(myEnum).name());
  

  db.put<MyEnum>("myEnum",myEnum);
  db.get<MyEnum >("myEnum")=myEnum;


  DataBase *pdb = &db;
  cout << "main: pdb="<< pdb << endl;


  ut(pdb);  // fortran routine

  return 0;
    
}
