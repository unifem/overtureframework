
c$$$      integer function getInt(pdb,name,x)
c$$$      ! -- add the length of the string as an extra arg --
c$$$      double precision pdb
c$$$      character *(*) name
c$$$      integer x,getIntFromDataBase
c$$$
c$$$      getInt = getIntFromDataBase(pdb,name,x,len(name))
c$$$      return 
c$$$      end
c$$$
c$$$      integer function getReal(pdb,name,x)
c$$$      ! -- add the length of the string as an extra arg --
c$$$      double precision pdb
c$$$      character *(*) name
c$$$      real x
c$$$      integer getRealFromDataBase
c$$$
c$$$      getReal = getRealFromDataBase(pdb,name,x,len(name))
c$$$      return 
c$$$      end
c$$$

      subroutine ut(pdb)
c==========================================================================
c==========================================================================

      double precision pdb
      character *10 name

      integer ok,getInt,getReal,num
      real mu

      name ='hello'
      ok = getInt(pdb,name,num)

      if( ok.eq.1 )then
        write(*,'("*** ut: name=",a10,", num=",i4)') name,num
      else
        write(*,'("*** ut: name=",a10,", NOT FOUND")') name
      end if

      name='goodbye'
      ok = getInt(pdb,'goodbye',num)  

      if( ok.eq.1 )then
        write(*,'("*** ut: name=",a10,", num=",i4)') name,num
      else
        write(*,'("*** ut: name=",a10," NOT FOUND")') name
      end if

      name ='mu'
      ok = getReal(pdb,'mu',mu)  

      if( ok.eq.1 )then
        write(*,'("*** ut: name=",a10,", mu=",f8.4)') name,mu
      else
        write(*,'("*** ut: name=",a10," NOT FOUND")') name
      end if

      ! get an enum:
      name='myEnum'
      ok = getInt(pdb,name,num)  

      if( ok.eq.1 )then
        write(*,'("*** ut: name=",a10,", num=",i4)') name,num
      else
        write(*,'("*** ut: name=",a10," NOT FOUND")') name
      end if

      name='subDir/i'
      ok = getInt(pdb,name,num)  

      if( ok.eq.1 )then
        write(*,'("*** ut: name=",a10,", num=",i4)') name,num
      else
        write(*,'("*** ut: name=",a10," NOT FOUND")') name
      end if

      return 
      end

     
