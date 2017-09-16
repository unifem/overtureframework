      ! Fortran include file for dispersive Maxwells equations

      ! variables for the dispersion model:
      ! P equation is : P_tt + ap*P_t + bp*P = cp*E
      ! real ap,bp,cp
      real kk,ck2,sNormSq,sNorm4, pc,ps,hfactor,hs,hc
      real si,sr,expt,sinxi,cosxi
      real sinxip,cosxip, sinxid, cosxid, sinxid2, cosxid2, sinxid3, cosxid3
      real amph,sint,cost,sintp,costp,hr,hi

      integer maxNumberOfPolarizationVectors
      parameter( maxNumberOfPolarizationVectors=20 )
      real psir(0:maxNumberOfPolarizationVectors-1), psii(0:maxNumberOfPolarizationVectors-1)

      ! Dispersion models
      integer noDispersion,drude
      parameter( noDispersion=0, drude=1 )
