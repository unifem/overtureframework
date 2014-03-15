#ifdef __sgi

#include <string.h>

#endif

#include "EntityTag.h"

#include "GenericDataBase.h"

std::string EntityTag::className = std::string("EntityTag");

int 
EntityTag::
setData( const std::string name, const void *data, const bool copy_data, const int data_size )
{
  destroy();

  tagName = name;
  
  if ( copy_data )
    {
      if ( !data_size )
	return 1;
      
      tagData = (void *) new char[data_size];
      if (!tagData) abort();
      memcpy(this->tagData, data, data_size);
    }
  else
    this->tagData = (void *)data;

  copyData = copy_data;
  dataSize = data_size;

  return 0;
}

int 
EntityTag::
getData( void * &data, std::string &name )
{
  data = tagData;
  name = tagName;
  return 0;
}

int 
EntityTag::
get( const GenericDataBase & dir, const aString & name )
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir, name, "EntityTag");
  aString classname;
  subDir.get( classname, "className" );

  if ( classname!=aString(EntityTag::className) )
    {
      cerr<<"Tag::get ERROR in className!"<<endl;
      return 1;
    }

  subDir.get(copyData, "copyData");
  subDir.get(dataSize, "dataSize");
  if ( copyData && dataSize )
    {
      // really aught to extend GenericDataBase...
      int size = dataSize/sizeof(int) + 1;
      int *buf = new int[size];
      buf[size] ='\0';
      subDir.get(buf, "data", size);
      tagData = (void *)new char[dataSize];
      memcpy(tagData, (void *)buf, dataSize);
      delete [] buf;
    }
  else
    {
      // this method to get around problems on the sgi is as yet untested
      //    kkc 030306
      //char *buf = new char[sizeof(void*)+1];
      //      buf[sizeof(void*)] ='\0';
      //      aString sbuf;//(buf);
      int td = (intptr_t)tagData;
      subDir.get(td, "data");//,(int)sizeof(void*));
      tagData = (void*)td;
      //      tagData=0;
      //      memcpy(&tagData, (void *)sbuf.c_str(), std::min(sizeof(void*),(unsigned int)sbuf.length()));

      //      subDir.get((int&)tagData, "data");
    }


  aString tag_name;
  subDir.get(tag_name,"tagName");
  tagName = tag_name.c_str();

  delete &subDir;
  return 0;
}

int 
EntityTag::
put( GenericDataBase & dir, const aString & name) const
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.create(subDir, name, "EntityTag");
  
  subDir.put(aString(EntityTag::className), "className");
  subDir.put(copyData,"copyData");
  subDir.put(dataSize,"dataSize");
  
  if ( copyData && dataSize )
    {
      int size = dataSize/sizeof(int) + 1;
      int *buf = new int[size];
      buf[size] = '\0';
      memcpy((void *)buf, this->tagData, dataSize);
      subDir.put(buf,"data",size);
      delete [] buf;
    }
  else
    {
      // this method to get around problems on the sgi is as yet untested
      //    kkc 030306
//       char *buf = new char[sizeof(void*)+1];
//       buf[sizeof(void*)]='\0';
//       memcpy((void *)buf, &tagData, sizeof(void*));
//       subDir.put(buf, "data");
//       delete [] buf;
      int idata = (intptr_t)tagData;
      subDir.put(idata, "data");
    }

  subDir.put(aString(tagName), "tagName");
  
  delete &subDir;
  
  return 0;
}
  
