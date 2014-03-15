      subroutine getUserDefinedEOS( r,p,e, ipar,rpar )
c==============================================================
c  Evaulate an Equation of State
c
c  r,p,e : rho, pressure, internal energy
c  eosID = ipar(1) : identifies the EOS
c  option = ipar(2) : 
c           option=0 : evaluate p given r and e
c           option=1 : evaluate e given r and p
c  derivOption=ipar(3)
c           derivOption=1 : evaluate dp/dr (e=const)
c           derivOption=2 : evaluate dp/de (rho=const)
c           derivOption=3 : evaluate dp/dr and dp/de
c
c
c==============================================================
      implicit none
      real r,p,e
      integer ipar(*)
      real rpar(*)
c
      integer eosID,option,derivOption
c
      integer strongShockEOS,ionizingEOS
      parameter( strongShockEOS=0, ionizingEOS=1 )
c
      integer init
      save init
      data init/0/

      eosID=ipar(1)
      option=ipar(2)
      derivOption=ipar(3)

      if( eosID.eq.strongShockEOS )then
        ! Strong Shock EOS from a polynomial

        if( init.eq.0 )then
          call sseosInit()
          init=1
        end if

        if( option.eq.0 )then
          call ssEosGetP( r,p,e, ipar,rpar )
        else if( option.eq.1 )then
          call ssEosGetE( r,p,e, ipar,rpar )
        else
          write(*,'("userDefinedEOS:ERROR: unknown option=",i6)') option
          stop 4020
        end if

      else if( eosID.eq.ionizingEOS )then

        write(*,'("userDefinedEOS:ERROR:not done: eosID=",i6)') eosID
        stop 4022

      else
        write(*,'("userDefinedEOS:ERROR:unknown eosID=",i6)') eosID
        stop 4021
      end if

      return
      end
      

