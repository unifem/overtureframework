#! /bin/csh -f
#
#
echo '-------------------------------------------------------'
echo '        Square for OGMG                           '
echo '-------------------------------------------------------'


echo '>>>Enter option: '
echo ' sq10[F]  : Poisson, Dirichlet BCs [F=fourth order]'
echo ' sq10n[F] : Poisson, Neumann BCs [F=Fourth order]'
echo ' sq10m[F] : Poisson, Mixed BCs, Neumann and Dirichlet [F=Fourth order]'
set option = $<

if( $option == "sq10" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../togmg << EOF
square10mg.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=9.,fratio=10.,idcges=0,
      epsz=1.e-6
 &end
EOF
else if( $option == "sq10F" )then

echo '                                                       '
echo '-------------------------------------------------------'
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=4  : Fourth Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../togmg << EOF
square10mg.dat
 &inp itest=0,iord=4,eps=1.e-2,zratio=9.,fratio=15,
      epsz=1.e-6
 &end
EOF

else if( $option == "sq10n" )then

echo '----------------------------------------------------'
echo 'itest=1  : Laplace, Neuman BC                        '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../togmg << EOF
square10mg.dat
 &inp itest=1,intbc=1,iord=2,zratio=10.,fratio=15.,ipcm=1,
      epsz=1.e-6
 &end
EOF

else if( $option == "sq10nF" )then

echo '----------------------------------------------------'
echo 'itest=1  : Laplace, Neuman BC                        '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../togmg << EOF
square10mg.dat
 &inp itest=1,intbc=1,iord=4,zratio=10.,fratio=20.,ipcm=1,
      epsz=1.e-6,itmax=8
 &end
EOF

else if( $option == "sq10m" )then


echo '----------------------------------------------------'
echo 'itest=17 : Mixed BCs - bc=4 : Neumann, Dirichlet otherwise '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'bc0      : specify BCs on each side of the grid'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../togmg << EOF
square10mg.dat
 &inp itest=17,intbc=1,iord=2,zratio=10.,fratio=15.,ipcm=1,
      bc0(1,1,1)=4,bc0(1,2,1)=1,bc0(2,1,1)=1,bc0(2,2,1)=1,
      bc0(3,1,1)=1,bc0(3,2,1)=1,
      epsz=1.e-6
    &end
EOF

else if( $option == "sq10mF" )then


echo '----------------------------------------------------'
echo 'itest=17 : Mixed BCs - bc=4 : Neumann, Dirichlet otherwise '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'bc0      : specify BCs on each side of the grid'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../togmg << EOF
square10mg.dat
 &inp itest=17,intbc=1,iord=4,zratio=10.,fratio=20.,ipcm=1,
      epsz=1.e-6,itmax=8,
      bc0(1,1,1)=4,bc0(1,2,1)=1,bc0(2,1,1)=1,bc0(2,2,1)=1,
      bc0(3,1,1)=1,bc0(3,2,1)=1,
    &end
EOF

else
  echo " Invalid option = $option"
endif

exit









