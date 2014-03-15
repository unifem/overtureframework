      integer function getInt(pdb,name,x)
      ! -- add the length of the string as an extra arg --
      double precision pdb
      character *(*) name
      integer x,getIntFromDataBase

      getInt = getIntFromDataBase(pdb,name,x,len(name))
      return 
      end

      integer function getReal(pdb,name,x)
      ! -- add the length of the string as an extra arg --
      double precision pdb
      character *(*) name
      real x
      integer getRealFromDataBase

      getReal = getRealFromDataBase(pdb,name,x,len(name))
      return 
      end
