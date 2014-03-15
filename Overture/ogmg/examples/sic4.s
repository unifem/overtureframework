#! /bin/sh -f
#
#
echo '-------------------------------------------------------'
echo '          Square in a Circle (sicmg.dat)               '
echo '            Fourth order, 2 MG levels                  '
echo '                                                       '
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=4  : Fourth order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
echo '>enter to continue'
read enter
../cgmgt << EOF
../../cguser/sicmg.dat
 &inp itest=0,iord=4,eps=1.e-3,zratio=8.,fratio=10. &end
EOF
echo '>enter to continue'
read enter
echo '----------------------------------------------------'
echo '          Square in a Circle (sicmg.dat)               '
echo '            Fourth order, 2 MG levels                  '
echo '                                                       '
echo 'itest=1  : Laplace, Neuman BC                        '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'iord=4   : Fourth Order'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
echo '>enter to continue'
read enter
../cgmgt << EOF
../../cguser/sicmg.dat
 &inp itest=1,intbc=1,iord=4,zratio=11.,fratio=25.,ipcm=1 &end
EOF













