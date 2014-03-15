      ! This function can be called from fortran to lookup int's or enums in 
      ! a Cgins data base
      integer function getIntCgins(pdb,name,x)
      ! -- add the length of the string as an extra arg --
      implicit none
      double precision pdb
      character *(*) name
      integer x,getIntFromDataBaseCgins

      getIntCgins = getIntFromDataBaseCgins(pdb,name,x,len(name))
      return 
      end

