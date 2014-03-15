#! /bin/csh -f
#
#
echo '>enter option:'
echo '  d : solve dirichlet problem'
echo '  n : solve Neumann problem'
set option = $<

set file = ../../cguser/sicmg.dat

if( $option == "d" )then

echo '-------------------------------------------------------'
echo '         CGMG:'
echo '            Second order, 2 MG levels                  '
echo "  using file $file "
echo '                                                       '
echo 'itest=0 : Laplace, Dirichlet BC'
echo 'iord=2  : Second Order'
echo 'eps     : Convergence tolerance'
echo 'zratio  : for allocating storage for A'
echo 'fratio  : fillin ratio for allocating storage for LU'
echo '-------------------------------------------------------'
echo '>enter to continue'
set enter = $<
../cgmgt << EOF
$file
 &inp itest=0,iord=2,eps=1.e-2,zratio=6.,fratio=8. &end
EOF

else 

echo '----------------------------------------------------'
echo '   CGMG '
echo '            Second order, 2 MG levels                  '
echo "  using file $file "
echo '                                                       '
echo 'itest=1  : Laplace, Neuman BC                        '
echo 'intbc=1  : apply eqn on boundary, BC on line -1'
echo 'iord=2   : Second Order'
echo 'zratio   : for allocating storage for A'
echo 'fratio   : fillin ratio for allocating storage for LU'
echo '----------------------------------------------------'
echo '>enter to continue'
set enter = $<

set ipcm = "0"    # do not precondition boundary

../cgmgt << EOF
$file
 &inp itest=1,intbc=1,iord=2,zratio=9.,fratio=20.,ipcm=$ipcm &end
EOF

endif

exit











