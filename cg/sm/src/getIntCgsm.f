      ! This function can be called from fortran to lookup int's or enums in 
      ! a Cgsm data base
      integer function getIntCgsm(pdb,name,x)
      ! -- add the length of the string as an extra arg --
      implicit none
      double precision pdb
      character *(*) name
      integer x,getIntFromDataBaseCgsm

      getIntCgsm = getIntFromDataBaseCgsm(pdb,name,x,len(name))
      return 
      end
