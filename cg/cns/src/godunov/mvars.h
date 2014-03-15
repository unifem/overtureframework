      ! NOTE: This common block is used in setUserDefinedParameters.C 
      ! NOTE: gfortran wants the real variables in the common to be aligned properly, so
      !       to be safe put these in a separate common 
      integer acousticSwitch 
      real*8 acm,  rparEOS(5) 
      ! opaque pointer to use EOS class:
      integer iparEOS(5),eosOption,eosDerivOption
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix,
     *                 ivisco,iupwind,ilimit,ides,ifour,
     *                 acousticSwitch
      common / mrvars / acm, rparEOS, iparEOS
      integer primSlope,conSlope,arrhenius,pressure,
     *        chainAndBranching, ignitionAndGrowth,
     *        igDesensitization
      ! *wdh* 2012/03/25 changed chainAndBranch to chainAndBranching
      parameter( primSlope=1, conSlope=0, 
     *           noRxn=0, arrhenius=1, pressure=7,
     *           chainAndBranching=2, ignitionAndGrowth=3,
     *           igDesensitization=8)
