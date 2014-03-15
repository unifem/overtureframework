// This file automatically generated from dbAccess.bC with bpp.
#include "OvertureTypes.h"
#include <list>

#define KK_DEBUG
#include "DBase.hh"

using namespace std;
using namespace DBase;


DataBase* getSubDir( DataBase & db, const string & fullPathName, string & name)
// ===================================================================================
//   Given a full path name, locate the lowest level sub-directory and the
//   name of the base variable.
//
//       Example fullPathName="mydir/var
// 
// /db (input):
// /fullPathName (input):
// /name (output) : name of variable
// /Return value: pointer to the lowest level sub-directory, return NULL if not found
// ==================================================================================
{
    DataBase *pDataBase = &db;
    name = fullPathName;
    int d = name.find_first_of('/');
    while( d!=string::npos )
    {
        string dirName(name,0,d);
        printf(">>>getSubDir: directory found: dir=%s\n",dirName.c_str());

        if( pDataBase->has_key(dirName) )
        {
            DataBase subDir =pDataBase->get<DataBase>(dirName);
            printf("\n+++ getSubDir: Entries in subDir +++\n");
            for( DataBase::iterator e=subDir.begin(); e!=subDir.end(); e++ )
            {
      	cout << "name="<< (*e).first <<endl;
            }

            pDataBase = &(pDataBase->get<DataBase>(dirName));
        }
        else
        {
            printf("getSubDir:WARNING: %s is not in the data-base (directory %s is NOT found)\n",name.c_str(),
                                dirName.c_str());
            return NULL;
        }
        name.erase(0,dirName.size()+1); // remove "dirName/" 

        d = name.find_first_of('/');   // search for another director path
    }

    return pDataBase;
}


enum MyEnum
{
    enum0=0,
    enum1=1,
    enum2=2,
    enum3=3
};


#define getIntFromDataBase EXTERN_C_NAME(getintfromdatabase)
#define getRealFromDataBase EXTERN_C_NAME(getrealfromdatabase)


extern "C"
{

int getIntFromDataBase( DataBase *pdb, char *name_, int & value, int & nameLength)
// =======================================================================
// Use this routine from fortran to look-up a variable in the data base
//
// /Return value: 1=found, 
//                0=not found,
//               -1=name found but not the correct type
//
// /Note: value is left unchanged if it was not found. 
// =======================================================================
{
    const int debug=0;
    string name(name_,0,nameLength);
  // remove trailing blanks
    int i= name.find_last_not_of(" "); // position of last non-blank character
    name.erase(i+1,name.size()-i);
//   int i=name.size()-1;
//   while( i>0 && name[i]==' ' ) i--;
//   if( i<name.size()-1 )
//     name.erase(i+1,name.size()-i);
    if( debug & 1 ) printf("getIntFromDataBase: nameLength=%i, name=[%s]\n",nameLength,name.c_str());
    DataBase *pDataBase=pdb; // start from here and then look for sub-directories
  // look for a directory prefix such as name = "dir/var"
    int d = name.find_first_of('/');
    while( d!=string::npos )
    {
        string dirName(name,0,d);
        if( debug & 1 ) printf(">>>getIntFromDataBase: directory found: dir=%s\n",dirName.c_str());
        if( pDataBase->has_key(dirName) )
        {
            DataBase subDir =pDataBase->get<DataBase>(dirName);
            if( debug & 1 ) 
            {
      	printf("+++ getIntFromDataBase: Entries in subDir +++\n");
      	for( DataBase::iterator e=subDir.begin(); e!=subDir.end(); e++ )
      	{
        	  cout << "name="<< (*e).first <<endl;
      	}
            }
            pDataBase = &(pDataBase->get<DataBase>(dirName));
        }
        else
        {
            printf("getIntFromDataBase:WARNING: %s is not in the data-base (directory %s is NOT found)\n",name.c_str(),
                                dirName.c_str());
        }
        name.erase(0,dirName.size()+1); // remove "dirName/" 
        d = name.find_first_of('/');   // search for another directory path
    }
    DataBase & db = *pDataBase;
    if( debug & 1 )
    {
        printf("\n=== getIntFromDataBase: Entries in db ====\n");
        for( DataBase::iterator e=db.begin(); e!=db.end(); e++ )
        {
            cout << "name="<< (*e).first <<endl;
        }
    }
    if( db.has_key(name) )
    {
        KK::sptr<Entry> ep = db.getEntry(name);
        if( DBase::can_cast_entry<int>(*ep) )
        {
            if( debug & 1 ) printf("getIntFromDataBase: entry is of type int\n");
            value=db.get<int>(name);
            return 1;
        }
//     else if( DBase::can_cast_entry<MyEnum>(*ep) )
//     {
//       if( debug & 1 ) printf("getIntFromDataBase: entry is of type MyEnum\n");
//       value=(int)db.get<MyEnum>(name);
//       return 1;
//     }
        else
        {
            printf("getIntFromDataBase:WARNING: %s is in the data-base but is NOT of type 'int'.\n",name.c_str());
      //value= **( (KK::sptr_dynamic_cast< SpecializedEntry<int> >(ep)));
      //printf(" value=%i\n",value);
      //return 1;
            return -1;
        }
    }
    else
    {
        printf("getIntFromDataBase:WARNING: %s is not in the data-base\n",name.c_str());
    }
    return 0;
}
int getRealFromDataBase( DataBase *pdb, char *name_, real & value, int & nameLength)
// =======================================================================
// Use this routine from fortran to look-up a variable in the data base
//
// /Return value: 1=found, 
//                0=not found,
//               -1=name found but not the correct type
//
// /Note: value is left unchanged if it was not found. 
// =======================================================================
{
    const int debug=0;
    string name(name_,0,nameLength);
  // remove trailing blanks
    int i= name.find_last_not_of(" "); // position of last non-blank character
    name.erase(i+1,name.size()-i);
//   int i=name.size()-1;
//   while( i>0 && name[i]==' ' ) i--;
//   if( i<name.size()-1 )
//     name.erase(i+1,name.size()-i);
    if( debug & 1 ) printf("getRealFromDataBase: nameLength=%i, name=[%s]\n",nameLength,name.c_str());
    DataBase *pDataBase=pdb; // start from here and then look for sub-directories
  // look for a directory prefix such as name = "dir/var"
    int d = name.find_first_of('/');
    while( d!=string::npos )
    {
        string dirName(name,0,d);
        if( debug & 1 ) printf(">>>getRealFromDataBase: directory found: dir=%s\n",dirName.c_str());
        if( pDataBase->has_key(dirName) )
        {
            DataBase subDir =pDataBase->get<DataBase>(dirName);
            if( debug & 1 ) 
            {
      	printf("+++ getRealFromDataBase: Entries in subDir +++\n");
      	for( DataBase::iterator e=subDir.begin(); e!=subDir.end(); e++ )
      	{
        	  cout << "name="<< (*e).first <<endl;
      	}
            }
            pDataBase = &(pDataBase->get<DataBase>(dirName));
        }
        else
        {
            printf("getRealFromDataBase:WARNING: %s is not in the data-base (directory %s is NOT found)\n",name.c_str(),
                                dirName.c_str());
        }
        name.erase(0,dirName.size()+1); // remove "dirName/" 
        d = name.find_first_of('/');   // search for another directory path
    }
    DataBase & db = *pDataBase;
    if( debug & 1 )
    {
        printf("\n=== getRealFromDataBase: Entries in db ====\n");
        for( DataBase::iterator e=db.begin(); e!=db.end(); e++ )
        {
            cout << "name="<< (*e).first <<endl;
        }
    }
    if( db.has_key(name) )
    {
        KK::sptr<Entry> ep = db.getEntry(name);
        if( DBase::can_cast_entry<real>(*ep) )
        {
            if( debug & 1 ) printf("getRealFromDataBase: entry is of type real\n");
            value=db.get<real>(name);
            return 1;
        }
//     else if( DBase::can_cast_entry<MyEnum>(*ep) )
//     {
//       if( debug & 1 ) printf("getRealFromDataBase: entry is of type MyEnum\n");
//       value=(real)db.get<MyEnum>(name);
//       return 1;
//     }
        else
        {
            printf("getRealFromDataBase:WARNING: %s is in the data-base but is NOT of type 'real'.\n",name.c_str());
      //value= **( (KK::sptr_dynamic_cast< SpecializedEntry<real> >(ep)));
      //printf(" value=%i\n",value);
      //return 1;
            return -1;
        }
    }
    else
    {
        printf("getRealFromDataBase:WARNING: %s is not in the data-base\n",name.c_str());
    }
    return 0;
}

  
} // end extern "C"


