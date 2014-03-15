#! /bin/csh -f
#
#
echo '-------------------------------------------------------'
echo '          Circle in a Square                           '
echo '        2nd order, 2 MG levels                  '
echo '-------------------------------------------------------'


echo '>>>Enter option: '
echo ' cis    : Poisson, Dirichlet BCs'
echo ' cisn   : Poisson, Neumann BCs '
echo ' cism   : Poisson, Mixed BCs, Neumann and Dirichlet'
set option = $<

if( $option == "cis" )then

echo '                                                       '
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
../cgmgt << EOF
../../cguser/cismg.dat
 &inp itest=0,iord=2,eps=1.e-2,zratio=9.,fratio=20. &end
EOF

else if( $option == "cisn" )then

echo '----------------------------------------------------'
echo 'itest=1  : Laplace, Neuman BC                        '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'

set ipcm = "1"   # 1:precondition BC's

../cgmgt << EOF
../../cguser/cismg.dat
 &inp itest=1,intbc=1,iord=2,zratio=12.,fratio=30.,ipcm=$ipcm &end
EOF

else if( $option == "cism" )then


echo '----------------------------------------------------'
echo '            4th order, 2 MG levels                  '
echo '                                                       '
echo 'itest=17 : Mixed BCs - bc=4 : Neumann, Dirichlet otherwise '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'ipcm=1   : Precondition equations at boundary  '
echo 'bc0      : specify BCs on each side of the grid'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
../cgmgt << EOF
../../cguser/cismg.dat
 &inp itest=17,intbc=1,iord=2,zratio=12.,fratio=40.,ipcm=1,
      bc0(1,1,1)=4,bc0(1,2,1)=1,bc0(2,1,1)=1,bc0(2,2,1)=1 &end
EOF

else
  echo " Invalid option = $option"
endif

exit









