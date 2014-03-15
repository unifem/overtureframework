c This fortran include file defines the names of equations of state
c idealEOS: ideal gas
c jwlEOS: 
c mgEOS: Mie-Gruneisen
c userDefinedEOS: user defined
c
      integer idealGasEOS,jwlEOS,mieGruneisenEOS,numberOfEOS,
     &  userDefinedEOS,stiffenedGasEOS,taitEOS
      parameter( idealGasEOS=0, jwlEOS=1, mieGruneisenEOS=2, 
     &  userDefinedEOS=3, stiffenedGasEOS=4, taitEOS=5,
     &  numberOfEOS=6 )
