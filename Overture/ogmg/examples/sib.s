#! /bin/csh -f
#
#
echo '-------------------------------------------------------'
echo '      Sphere in a Box for CGMG                           '
echo '-------------------------------------------------------'


echo '>>>Enter option: '
echo ' sib[F]  : Poisson, Dirichlet BCs [F=fourth order]'
echo ' sibn[F] : Poisson, Neumann BCs [F=Fourth order]'
echo ' sibm[F] : Poisson, Mixed BCs, Neumann and Dirichlet [F=Fourth order]'
set option = $<

if( $option == "sib" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../cgmgt << EOF
../../cguser/sib2.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=7.,fratio=15.,
      flags='C',tol=1.e-2,
      epsz=1.e-6 
 &end
EOF
exit
../../cguser/pipes2.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=5.,fratio=25.,
      flags='C',tol=1.e-2,idcges=7,info=7,
      epsz=1.e-6
 &end
EOF
exit
../../cguser/sib2.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=7.,fratio=15.,
      flags='C',tol=1.e-2,
      epsz=1.e-6 
 &end
EOF

else if( $option == "sibn" )then

echo '----------------------------------------------------'
echo 'itest=1  : Laplace, Neuman BC                        '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../cgmgt << EOF
../../cguser/sib2.dat
 &inp itest=1,intbc=1,iord=2,zratio=8.,fratio=15.,ipcm=1,
      flags='C',tol=1.e-2,
      epsz=1.e-6
 &end
EOF

else if( $option == "sibm" )then

echo '----------------------------------------------------'
echo 'itest=17 : Mixed BCs - bc=4 : Neumann, Dirichlet otherwise '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'bc0      : specify BCs on each side of the grid'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../cgmgt << EOF
../../cguser/sib2.dat
 &inp itest=17,intbc=1,iord=2,zratio=8.,fratio=15.,ipcm=1,
      bc0(1,1,1)=4,bc0(1,2,1)=1,bc0(2,1,1)=1,bc0(2,2,1)=1,
      bc0(3,1,1)=1,bc0(3,2,1)=1,
      flags='C',tol=1.e-2,
      epsz=1.e-6
    &end
EOF

else if( $option == "sibF" )then

echo '----------------------------------------------------'
echo 'itest=1  : Laplace, Neuman BC                        '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../cgmgt << EOF
../../cguser/sphere2.dat
 &inp itest=0,iord=4,zratio=20.,fratio=30.,ipcm=0,
      epsz=1.e-6,itmax=8,flags='C',tol=1.e-2,
      smth(1,1)=1,smth(2,1)=1,smth(3,1)=1
 &end
EOF

else if( $option == "sibnF" )then

echo '----------------------------------------------------'
echo 'itest=1  : Neumann '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'bc0      : specify BCs on each side of the grid'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../cgmgt << EOF
../../cguser/sphere2.dat
 &inp itest=1,intbc=1,iord=4,zratio=20.,fratio=30.,ipcm=1,idcges=3,
      epsz=1.e-6,itmax=8,flags='C',tol=1.e-2
    &end
EOF

else if( $option == "sibmF" )then

echo '----------------------------------------------------'
echo 'itest=17 : Mixed BCs - bc=4 : Neumann, Dirichlet otherwise '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'bc0      : specify BCs on each side of the grid'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../cgmgt << EOF
../../cguser/sphere2.dat
 &inp itest=17,intbc=1,iord=4,zratio=19.,fratio=30.,ipcm=1,idcges=3,
      epsz=1.e-6,itmax=8,flags='Y',
      bc0(1,1,1)=4,bc0(1,2,1)=1,bc0(2,1,1)=1,bc0(2,2,1)=1,
      bc0(3,1,1)=1,bc0(3,2,1)=1
    &end
EOF

else
  echo " Invalid option = $option"
endif

exit









